
/** @file
  S3M_VREG.h

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
/* HSP_BOOT                                                                     */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI   */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
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
/* HSP_BOOT                                                                     */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI   */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
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
/* HSP_BOOT                                                                     */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI   */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
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
/* HSP_BOOT                                                                     */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI   */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
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
/* HSP_BOOT                                                                     */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI   */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
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
/* HSP_BOOT                                                                     */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI   */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */


#ifndef _S3M_VREG_h
#define _S3M_VREG_h
#include <Base.h>

/* S3M_MAILBOX_COMMAND_N0_S3M_VREG_REG supported on:                            */
/*      SPRA0 (0x200dc208)                                                      */
/*      SPRB0 (0x200dc208)                                                      */
/*      SPRHBM (0x200dc208)                                                     */
/*      SPRC0 (0x200dc208)                                                      */
/*      SPRMCC (0x200dc208)                                                     */
/*      SPRUCC (0x200dc208)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_vreg/ibl_vreg/ibl_vreg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* FW Virtual Register. Indicates the FW on the flow that is being triggered by an external IP.   Any access to this register should be dropped by hardware without interrupting the FW if the Secure mailbox is NOT taken.
*/


#define S3M_MAILBOX_COMMAND_N0_S3M_VREG_REG 0x2D12C208

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 command_encoding : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               NO_FLOW = 0x0 SOCKET_ONLINING = 0x1
                               CPU_STRAP_PROVISION = 0x2 (BIOS_W policy SAIs
                               are allowed) SSKU_LICENSE_KEY_PROVISION = 0x4
                               (HSP_BOOT policy SAIs are allowed)
                               SSKU_LICENSE_BLOB_PROVISION = 0x8 (HSP_BOOT
                               policy SAIs are allowed) VABD_OEM_KEY_PROVISION
                               = 0x10 (BIOS_W policy SAIs are allowed)
                               VCM_KEY_HASH_PROVISION = 0x20 (BIOS_W policy
                               SAIs are allowed)
                               S3M_FW_CFR_PROVISION_NO_SVN_COMMIT = 0x40
                               (BIOS_W policy SAIs are allowed)
                               S3M_FW_CFR_PROVISION_SVN_COMMIT = 0x41 (BIOS_W
                               policy SAIs are allowed)
                               PUCODE_CFR_PROVISION_NO_SVN_COMMIT = 0x80
                               (BIOS_W policy SAIs are allowed)
                               PUCODE_CFR_PROVISION_SVN_COMMIT = 0x81 (BIOS_W
                               policy SAIs are allowed) VAD_TOKEN_PROCESS =
                               0x200 (BIOS_W policy SAIs are allowed) ***0x400
                               is reserved*** SOCKET_OFFLINED_INDICATION =
                               0x800 (BIOS_W policy SAIs are allowed) Note that
                               BIOS_W SAI policy is a subset of HSP_BOOT
                               policies otherwise
                            */

  } Bits;
  UINT32 Data;

} S3M_MAILBOX_COMMAND_N0_S3M_VREG_STRUCT;

/* S3M_MAILBOX_COMMAND_N1_S3M_VREG_REG supported on:                            */
/*      SPRA0 (0x200dc20c)                                                      */
/*      SPRB0 (0x200dc20c)                                                      */
/*      SPRHBM (0x200dc20c)                                                     */
/*      SPRC0 (0x200dc20c)                                                      */
/*      SPRMCC (0x200dc20c)                                                     */
/*      SPRUCC (0x200dc20c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_vreg/ibl_vreg/ibl_vreg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* FW Virtual Register. Indicates the FW on the flow that is being triggered by an external IP.   Any access to this register should be dropped by hardware without interrupting the FW if the Secure mailbox is NOT taken.
*/


#define S3M_MAILBOX_COMMAND_N1_S3M_VREG_REG 0x2D12C20C

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

} S3M_MAILBOX_COMMAND_N1_S3M_VREG_STRUCT;

/* S3M_MAILBOX_DATA_BUFFER_N0_S3M_VREG_REG supported on:                        */
/*      SPRA0 (0x200dc210)                                                      */
/*      SPRB0 (0x200dc210)                                                      */
/*      SPRHBM (0x200dc210)                                                     */
/*      SPRC0 (0x200dc210)                                                      */
/*      SPRMCC (0x200dc210)                                                     */
/*      SPRUCC (0x200dc210)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_vreg/ibl_vreg/ibl_vreg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Data register for the incoming stream of data upon which S3M will perform an operation.   Any access to this register should be dropped by hardware without interrupting the FW if the Secure mailbox is NOT taken. Not for EDS: Write to this register is honored ONLY if the mailbox is locked, else hardware should drop the request. This is to ensure FW is not interrupt erroneously causing a DOS
Open: We need some optimization here to ensure FW is NOT interrupted on every write but potentially only in chunks.
*/


#define S3M_MAILBOX_DATA_BUFFER_N0_S3M_VREG_REG 0x2D12C210

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW, default=None*/

                            /*
                               Incoming stream of data upon which S3M will
                               perform an operation
                            */

  } Bits;
  UINT32 Data;

} S3M_MAILBOX_DATA_BUFFER_N0_S3M_VREG_STRUCT;

/* S3M_MAILBOX_DATA_BUFFER_N1_S3M_VREG_REG supported on:                        */
/*      SPRA0 (0x200dc214)                                                      */
/*      SPRB0 (0x200dc214)                                                      */
/*      SPRHBM (0x200dc214)                                                     */
/*      SPRC0 (0x200dc214)                                                      */
/*      SPRMCC (0x200dc214)                                                     */
/*      SPRUCC (0x200dc214)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_vreg/ibl_vreg/ibl_vreg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Data register for the incoming stream of data upon which S3M will perform an operation.   Any access to this register should be dropped by hardware without interrupting the FW if the Secure mailbox is NOT taken. Not for EDS: Write to this register is honored ONLY if the mailbox is locked, else hardware should drop the request. This is to ensure FW is not interrupt erroneously causing a DOS
Open: We need some optimization here to ensure FW is NOT interrupted on every write but potentially only in chunks.
*/


#define S3M_MAILBOX_DATA_BUFFER_N1_S3M_VREG_REG 0x2D12C214

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW, default=None*/

                            /*
                               Incoming stream of data upon which S3M will
                               perform an operation
                            */

  } Bits;
  UINT32 Data;

} S3M_MAILBOX_DATA_BUFFER_N1_S3M_VREG_STRUCT;

/* S3M_CFG_SETUP_N0_S3M_VREG_REG supported on:                                  */
/*      SPRA0 (0x200dc270)                                                      */
/*      SPRB0 (0x200dc270)                                                      */
/*      SPRHBM (0x200dc270)                                                     */
/*      SPRC0 (0x200dc270)                                                      */
/*      SPRMCC (0x200dc270)                                                     */
/*      SPRUCC (0x200dc270)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_vreg/ibl_vreg/ibl_vreg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M Configuration that can be set up by BIOS
*/


#define S3M_CFG_SETUP_N0_S3M_VREG_REG 0x2D12C270

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mailbox_lock_timer_cfg : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Config register for Mailbox Lock Timer */

  } Bits;
  UINT32 Data;

} S3M_CFG_SETUP_N0_S3M_VREG_STRUCT;

/* S3M_CFG_SETUP_N1_S3M_VREG_REG supported on:                                  */
/*      SPRA0 (0x200dc274)                                                      */
/*      SPRB0 (0x200dc274)                                                      */
/*      SPRHBM (0x200dc274)                                                     */
/*      SPRC0 (0x200dc274)                                                      */
/*      SPRMCC (0x200dc274)                                                     */
/*      SPRUCC (0x200dc274)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_vreg/ibl_vreg/ibl_vreg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M Configuration that can be set up by BIOS
*/


#define S3M_CFG_SETUP_N1_S3M_VREG_REG 0x2D12C274

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mailbox_lock_timer_cfg : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Config register for Mailbox Lock Timer */

  } Bits;
  UINT32 Data;

} S3M_CFG_SETUP_N1_S3M_VREG_STRUCT;

/* S3M_STATUS_REQUEST_N0_S3M_VREG_REG supported on:                             */
/*      SPRA0 (0x200dc2a8)                                                      */
/*      SPRB0 (0x200dc2a8)                                                      */
/*      SPRHBM (0x200dc2a8)                                                     */
/*      SPRC0 (0x200dc2a8)                                                      */
/*      SPRMCC (0x200dc2a8)                                                     */
/*      SPRUCC (0x200dc2a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_vreg/ibl_vreg/ibl_vreg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* BIOS or Ucode should write appropriate bit to '1 so that the appropriate register data is released.
*/


#define S3M_STATUS_REQUEST_N0_S3M_VREG_REG 0x2D12C2A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_cfg_data : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Mutually Exclusive with Bit [1] */
    UINT32 cpu_strap_set_data : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Write to this bit required for the data in
                               S3M_CPU_STRAP_SET_DATA to be valid. Mutually
                               Exclusive with Bit [0]
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} S3M_STATUS_REQUEST_N0_S3M_VREG_STRUCT;

/* S3M_STATUS_REQUEST_N1_S3M_VREG_REG supported on:                             */
/*      SPRA0 (0x200dc2ac)                                                      */
/*      SPRB0 (0x200dc2ac)                                                      */
/*      SPRHBM (0x200dc2ac)                                                     */
/*      SPRC0 (0x200dc2ac)                                                      */
/*      SPRMCC (0x200dc2ac)                                                     */
/*      SPRUCC (0x200dc2ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_vreg/ibl_vreg/ibl_vreg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* BIOS or Ucode should write appropriate bit to '1 so that the appropriate register data is released.
*/


#define S3M_STATUS_REQUEST_N1_S3M_VREG_REG 0x2D12C2AC

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

} S3M_STATUS_REQUEST_N1_S3M_VREG_STRUCT;

/* S3M_BIOS_W_DATA_OUTPUT_REG_CONFIG_N0_S3M_VREG_REG supported on:              */
/*      SPRA0 (0x200dc2b0)                                                      */
/*      SPRB0 (0x200dc2b0)                                                      */
/*      SPRHBM (0x200dc2b0)                                                     */
/*      SPRC0 (0x200dc2b0)                                                      */
/*      SPRMCC (0x200dc2b0)                                                     */
/*      SPRUCC (0x200dc2b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_vreg/ibl_vreg/ibl_vreg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* S3M writes the address that is about to be read out of S3M_GENERIC_DATA_OUT_REG
*/


#define S3M_BIOS_W_DATA_OUTPUT_REG_CONFIG_N0_S3M_VREG_REG 0x2D12C2B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_generic_data_output_size : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               Length of the data/content that exists within
                               S3M, it decrements this.
                            */

  } Bits;
  UINT32 Data;

} S3M_BIOS_W_DATA_OUTPUT_REG_CONFIG_N0_S3M_VREG_STRUCT;

/* S3M_BIOS_W_DATA_OUTPUT_REG_CONFIG_N1_S3M_VREG_REG supported on:              */
/*      SPRA0 (0x200dc2b4)                                                      */
/*      SPRB0 (0x200dc2b4)                                                      */
/*      SPRHBM (0x200dc2b4)                                                     */
/*      SPRC0 (0x200dc2b4)                                                      */
/*      SPRMCC (0x200dc2b4)                                                     */
/*      SPRUCC (0x200dc2b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_vreg/ibl_vreg/ibl_vreg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* S3M writes the address that is about to be read out of S3M_GENERIC_DATA_OUT_REG
*/


#define S3M_BIOS_W_DATA_OUTPUT_REG_CONFIG_N1_S3M_VREG_REG 0x2D12C2B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_generic_data_output_type : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Encoding of the type of data that is being
                               streamed out
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 s3m_data_output_valid : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Set as valid until the last 64 bits of the data
                               output register is read
                            */

  } Bits;
  UINT32 Data;

} S3M_BIOS_W_DATA_OUTPUT_REG_CONFIG_N1_S3M_VREG_STRUCT;

/* S3M_BIOS_W_GENERIC_DATA_OUT_REG_N0_S3M_VREG_REG supported on:                */
/*      SPRA0 (0x200dc2d0)                                                      */
/*      SPRB0 (0x200dc2d0)                                                      */
/*      SPRHBM (0x200dc2d0)                                                     */
/*      SPRC0 (0x200dc2d0)                                                      */
/*      SPRMCC (0x200dc2d0)                                                     */
/*      SPRUCC (0x200dc2d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_vreg/ibl_vreg/ibl_vreg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Outputs different types of data as defined by mailbox flows
*/


#define S3M_BIOS_W_GENERIC_DATA_OUT_REG_N0_S3M_VREG_REG 0x2D12C2D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_generic_data_out_reg : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Streaming register output */

  } Bits;
  UINT32 Data;

} S3M_BIOS_W_GENERIC_DATA_OUT_REG_N0_S3M_VREG_STRUCT;

/* S3M_BIOS_W_GENERIC_DATA_OUT_REG_N1_S3M_VREG_REG supported on:                */
/*      SPRA0 (0x200dc2d4)                                                      */
/*      SPRB0 (0x200dc2d4)                                                      */
/*      SPRHBM (0x200dc2d4)                                                     */
/*      SPRC0 (0x200dc2d4)                                                      */
/*      SPRMCC (0x200dc2d4)                                                     */
/*      SPRUCC (0x200dc2d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_vreg/ibl_vreg/ibl_vreg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_vreg[0]/ibl_vreg/ibl_vreg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Outputs different types of data as defined by mailbox flows
*/


#define S3M_BIOS_W_GENERIC_DATA_OUT_REG_N1_S3M_VREG_REG 0x2D12C2D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_generic_data_out_reg : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Streaming register output */

  } Bits;
  UINT32 Data;

} S3M_BIOS_W_GENERIC_DATA_OUT_REG_N1_S3M_VREG_STRUCT;
#endif /* _S3M_VREG_h */
