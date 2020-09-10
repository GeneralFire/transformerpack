
/** @file
  SB_PRVT_FBLP.h

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
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* DFX2                                                                         */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OPEN                                                                         */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OS_W                                                                         */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRB0 Security Policy Groups:                                                */
/* BOOT_W                                                                       */
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
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* DFX2                                                                         */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OPEN                                                                         */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OS_W                                                                         */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRHBM Security Policy Groups:                                               */
/* BOOT_W                                                                       */
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
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* DFX2                                                                         */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OPEN                                                                         */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OS_W                                                                         */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRC0 Security Policy Groups:                                                */
/* BOOT_W                                                                       */
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
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* DFX2                                                                         */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OPEN                                                                         */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OS_W                                                                         */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRMCC Security Policy Groups:                                               */
/* BOOT_W                                                                       */
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
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* DFX2                                                                         */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OPEN                                                                         */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OS_W                                                                         */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRUCC Security Policy Groups:                                               */
/* BOOT_W                                                                       */
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
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* DFX2                                                                         */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OPEN                                                                         */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OS_W                                                                         */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */


#ifndef _SB_PRVT_FBLP_h
#define _SB_PRVT_FBLP_h
#include <Base.h>

/* PPBIFCTL_PRIV_SB_PRVT_FBLP_REG supported on:                                 */
/*      SPRA0 (0x200000c0)                                                      */
/*      SPRB0 (0x200000c0)                                                      */
/*      SPRHBM (0x200000c0)                                                     */
/*      SPRC0 (0x200000c0)                                                      */
/*      SPRMCC (0x200000c0)                                                     */
/*      SPRUCC (0x200000c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRB0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRHBM Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRC0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRMCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRUCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BTSMM_W                                          */
/* SPRB0 Security PolicyGroup: BTSMM_W                                          */
/* SPRHBM Security PolicyGroup: BTSMM_W                                         */
/* SPRC0 Security PolicyGroup: BTSMM_W                                          */
/* SPRMCC Security PolicyGroup: BTSMM_W                                         */
/* SPRUCC Security PolicyGroup: BTSMM_W                                         */
/* Port Configuration
Bifurcation Control for MLW (x16) vs Port number
*/


#define PPBIFCTL_PRIV_SB_PRVT_FBLP_REG 0x2C1200C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bifctl0 : 3;

                            /* Bits[2:0], Access Type=RW/V/L, default=None*/

                            /*
                               111-101: Reserved ------P3 P2 P1 P0 100: __ __
                               __ x16 011: __ x8 __ x8 (15:8 operates as x8,
                               7:0 operates as x8) 010: __ x8 x4 x4 (15:8
                               operates as x8, 7:4 operates as x4, 3:0 operates
                               as x4) 001: x4 x4 __ x8 (15:12 operates as x4,
                               11:8 operates as x4, 7:0 operates as x8) 000: x4
                               x4 x4 x4 (15:12 operates as x4, 11:8 operates as
                               x4, 7:4 operates as x4 and 3:0 operates as x4) A
                               write of '1' on the CL will lock this register
                               field down as bifurcation control status
                               information. Note: Lock Key bit is located in
                               the PPBIFCTL_PRIV Register .
                            */
    UINT32 rsvd : 28;

                            /* Bits[30:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cl : 1;

                            /* Bits[31:31], Access Type=RW/L, default=None*/

                            /*
                               Lock key bit for all RW-L bits of PPBIFCTL
                               register only. 1b: Lock 0b: Unlocked Note: Lock
                               Key bit is located in the PPBIFCTL_PRIV Register
                               . This bit is self-locking. Once this bit is set
                               to a 1b, this key bit can not be unlocked.
                               Writing a 0b has no effect on this bit.
                            */

  } Bits;
  UINT32 Data;

} PPBIFCTL_PRIV_SB_PRVT_FBLP_STRUCT;













/* ACRDPR3_N0_SB_PRVT_FBLP_REG supported on:                                    */
/*      SPRA0 (0x20000530)                                                      */
/*      SPRB0 (0x20000530)                                                      */
/*      SPRHBM (0x20000530)                                                     */
/*      SPRC0 (0x20000530)                                                      */
/*      SPRMCC (0x20000530)                                                     */
/*      SPRUCC (0x20000530)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRB0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRHBM Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRC0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRMCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRUCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Access Control Read Policy
*/


#define ACRDPR3_N0_SB_PRVT_FBLP_REG 0x2C120530

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Read Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has read access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               read access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACRDPR3_N0_SB_PRVT_FBLP_STRUCT;

/* ACRDPR3_N1_SB_PRVT_FBLP_REG supported on:                                    */
/*      SPRA0 (0x20000534)                                                      */
/*      SPRB0 (0x20000534)                                                      */
/*      SPRHBM (0x20000534)                                                     */
/*      SPRC0 (0x20000534)                                                      */
/*      SPRMCC (0x20000534)                                                     */
/*      SPRUCC (0x20000534)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRB0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRHBM Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRC0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRMCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRUCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Access Control Read Policy
*/


#define ACRDPR3_N1_SB_PRVT_FBLP_REG 0x2C120534

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Read Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has read access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               read access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACRDPR3_N1_SB_PRVT_FBLP_STRUCT;

/* ACWRPR3_N0_SB_PRVT_FBLP_REG supported on:                                    */
/*      SPRA0 (0x20000538)                                                      */
/*      SPRB0 (0x20000538)                                                      */
/*      SPRHBM (0x20000538)                                                     */
/*      SPRC0 (0x20000538)                                                      */
/*      SPRMCC (0x20000538)                                                     */
/*      SPRUCC (0x20000538)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRB0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRHBM Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRC0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRMCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRUCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Access Control Write Policy
*/


#define ACWRPR3_N0_SB_PRVT_FBLP_REG 0x2C120538

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Write Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has write access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               write access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACWRPR3_N0_SB_PRVT_FBLP_STRUCT;

/* ACWRPR3_N1_SB_PRVT_FBLP_REG supported on:                                    */
/*      SPRA0 (0x2000053c)                                                      */
/*      SPRB0 (0x2000053c)                                                      */
/*      SPRHBM (0x2000053c)                                                     */
/*      SPRC0 (0x2000053c)                                                      */
/*      SPRMCC (0x2000053c)                                                     */
/*      SPRUCC (0x2000053c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRB0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRHBM Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRC0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRMCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRUCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Access Control Write Policy
*/


#define ACWRPR3_N1_SB_PRVT_FBLP_REG 0x2C12053C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Write Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has write access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               write access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACWRPR3_N1_SB_PRVT_FBLP_STRUCT;

/* ACCRPR3_N0_SB_PRVT_FBLP_REG supported on:                                    */
/*      SPRA0 (0x20000540)                                                      */
/*      SPRB0 (0x20000540)                                                      */
/*      SPRHBM (0x20000540)                                                     */
/*      SPRC0 (0x20000540)                                                      */
/*      SPRMCC (0x20000540)                                                     */
/*      SPRUCC (0x20000540)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRB0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRHBM Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRC0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRMCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRUCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Access Control CR Policy
*/


#define ACCRPR3_N0_SB_PRVT_FBLP_REG 0x2C120540

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC CR Policy value. The SAI on incoming
                               configuration requests is hashed down to 6 bits
                               and used as an index into the 64-bit of three AC
                               policy registers If bit[n] is set, SAIn has both
                               read and write access to all the corresponding
                               policy group registers, i.e. used to limit write
                               access to the RAC, WAC and CP registers. (read
                               access to these registers is always allowed)
                            */

  } Bits;
  UINT32 Data;

} ACCRPR3_N0_SB_PRVT_FBLP_STRUCT;

/* ACCRPR3_N1_SB_PRVT_FBLP_REG supported on:                                    */
/*      SPRA0 (0x20000544)                                                      */
/*      SPRB0 (0x20000544)                                                      */
/*      SPRHBM (0x20000544)                                                     */
/*      SPRC0 (0x20000544)                                                      */
/*      SPRMCC (0x20000544)                                                     */
/*      SPRUCC (0x20000544)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRB0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRHBM Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRC0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRMCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRUCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Access Control CR Policy
*/


#define ACCRPR3_N1_SB_PRVT_FBLP_REG 0x2C120544

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC CR Policy value. The SAI on incoming
                               configuration requests is hashed down to 6 bits
                               and used as an index into the 64-bit of three AC
                               policy registers If bit[n] is set, SAIn has both
                               read and write access to all the corresponding
                               policy group registers, i.e. used to limit write
                               access to the RAC, WAC and CP registers. (read
                               access to these registers is always allowed)
                            */

  } Bits;
  UINT32 Data;

} ACCRPR3_N1_SB_PRVT_FBLP_STRUCT;

/* ACRDPR4_N0_SB_PRVT_FBLP_REG supported on:                                    */
/*      SPRA0 (0x20000548)                                                      */
/*      SPRB0 (0x20000548)                                                      */
/*      SPRHBM (0x20000548)                                                     */
/*      SPRC0 (0x20000548)                                                      */
/*      SPRMCC (0x20000548)                                                     */
/*      SPRUCC (0x20000548)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRB0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRHBM Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRC0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRMCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRUCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OPEN                                             */
/* SPRB0 Security PolicyGroup: OPEN                                             */
/* SPRHBM Security PolicyGroup: OPEN                                            */
/* SPRC0 Security PolicyGroup: OPEN                                             */
/* SPRMCC Security PolicyGroup: OPEN                                            */
/* SPRUCC Security PolicyGroup: OPEN                                            */
/* Access Control Read Policy
*/


#define ACRDPR4_N0_SB_PRVT_FBLP_REG 0x2C120548

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Read Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has read access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               read access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACRDPR4_N0_SB_PRVT_FBLP_STRUCT;

/* ACRDPR4_N1_SB_PRVT_FBLP_REG supported on:                                    */
/*      SPRA0 (0x2000054c)                                                      */
/*      SPRB0 (0x2000054c)                                                      */
/*      SPRHBM (0x2000054c)                                                     */
/*      SPRC0 (0x2000054c)                                                      */
/*      SPRMCC (0x2000054c)                                                     */
/*      SPRUCC (0x2000054c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRB0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRHBM Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRC0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRMCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRUCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OPEN                                             */
/* SPRB0 Security PolicyGroup: OPEN                                             */
/* SPRHBM Security PolicyGroup: OPEN                                            */
/* SPRC0 Security PolicyGroup: OPEN                                             */
/* SPRMCC Security PolicyGroup: OPEN                                            */
/* SPRUCC Security PolicyGroup: OPEN                                            */
/* Access Control Read Policy
*/


#define ACRDPR4_N1_SB_PRVT_FBLP_REG 0x2C12054C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Read Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has read access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               read access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACRDPR4_N1_SB_PRVT_FBLP_STRUCT;

/* ACWRPR4_N0_SB_PRVT_FBLP_REG supported on:                                    */
/*      SPRA0 (0x20000550)                                                      */
/*      SPRB0 (0x20000550)                                                      */
/*      SPRHBM (0x20000550)                                                     */
/*      SPRC0 (0x20000550)                                                      */
/*      SPRMCC (0x20000550)                                                     */
/*      SPRUCC (0x20000550)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRB0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRHBM Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRC0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRMCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRUCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OPEN                                             */
/* SPRB0 Security PolicyGroup: OPEN                                             */
/* SPRHBM Security PolicyGroup: OPEN                                            */
/* SPRC0 Security PolicyGroup: OPEN                                             */
/* SPRMCC Security PolicyGroup: OPEN                                            */
/* SPRUCC Security PolicyGroup: OPEN                                            */
/* Access Control Write Policy
*/


#define ACWRPR4_N0_SB_PRVT_FBLP_REG 0x2C120550

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Write Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has write access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               write access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACWRPR4_N0_SB_PRVT_FBLP_STRUCT;

/* ACWRPR4_N1_SB_PRVT_FBLP_REG supported on:                                    */
/*      SPRA0 (0x20000554)                                                      */
/*      SPRB0 (0x20000554)                                                      */
/*      SPRHBM (0x20000554)                                                     */
/*      SPRC0 (0x20000554)                                                      */
/*      SPRMCC (0x20000554)                                                     */
/*      SPRUCC (0x20000554)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRB0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRHBM Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRC0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRMCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRUCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OPEN                                             */
/* SPRB0 Security PolicyGroup: OPEN                                             */
/* SPRHBM Security PolicyGroup: OPEN                                            */
/* SPRC0 Security PolicyGroup: OPEN                                             */
/* SPRMCC Security PolicyGroup: OPEN                                            */
/* SPRUCC Security PolicyGroup: OPEN                                            */
/* Access Control Write Policy
*/


#define ACWRPR4_N1_SB_PRVT_FBLP_REG 0x2C120554

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Write Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has write access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               write access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACWRPR4_N1_SB_PRVT_FBLP_STRUCT;

/* ACCRPR4_N0_SB_PRVT_FBLP_REG supported on:                                    */
/*      SPRA0 (0x20000558)                                                      */
/*      SPRB0 (0x20000558)                                                      */
/*      SPRHBM (0x20000558)                                                     */
/*      SPRC0 (0x20000558)                                                      */
/*      SPRMCC (0x20000558)                                                     */
/*      SPRUCC (0x20000558)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRB0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRHBM Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRC0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRMCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRUCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OPEN                                             */
/* SPRB0 Security PolicyGroup: OPEN                                             */
/* SPRHBM Security PolicyGroup: OPEN                                            */
/* SPRC0 Security PolicyGroup: OPEN                                             */
/* SPRMCC Security PolicyGroup: OPEN                                            */
/* SPRUCC Security PolicyGroup: OPEN                                            */
/* Access Control CR Policy
*/


#define ACCRPR4_N0_SB_PRVT_FBLP_REG 0x2C120558

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC CR Policy value. The SAI on incoming
                               configuration requests is hashed down to 6 bits
                               and used as an index into the 64-bit of three AC
                               policy registers If bit[n] is set, SAIn has both
                               read and write access to all the corresponding
                               policy group registers, i.e. used to limit write
                               access to the RAC, WAC and CP registers. (read
                               access to these registers is always allowed)
                            */

  } Bits;
  UINT32 Data;

} ACCRPR4_N0_SB_PRVT_FBLP_STRUCT;

/* ACCRPR4_N1_SB_PRVT_FBLP_REG supported on:                                    */
/*      SPRA0 (0x2000055c)                                                      */
/*      SPRB0 (0x2000055c)                                                      */
/*      SPRHBM (0x2000055c)                                                     */
/*      SPRC0 (0x2000055c)                                                      */
/*      SPRMCC (0x2000055c)                                                     */
/*      SPRUCC (0x2000055c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRB0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRHBM Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRC0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRMCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRUCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OPEN                                             */
/* SPRB0 Security PolicyGroup: OPEN                                             */
/* SPRHBM Security PolicyGroup: OPEN                                            */
/* SPRC0 Security PolicyGroup: OPEN                                             */
/* SPRMCC Security PolicyGroup: OPEN                                            */
/* SPRUCC Security PolicyGroup: OPEN                                            */
/* Access Control CR Policy
*/


#define ACCRPR4_N1_SB_PRVT_FBLP_REG 0x2C12055C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC CR Policy value. The SAI on incoming
                               configuration requests is hashed down to 6 bits
                               and used as an index into the 64-bit of three AC
                               policy registers If bit[n] is set, SAIn has both
                               read and write access to all the corresponding
                               policy group registers, i.e. used to limit write
                               access to the RAC, WAC and CP registers. (read
                               access to these registers is always allowed)
                            */

  } Bits;
  UINT32 Data;

} ACCRPR4_N1_SB_PRVT_FBLP_STRUCT;

/* ACRDPR5_N0_SB_PRVT_FBLP_REG supported on:                                    */
/*      SPRA0 (0x20000560)                                                      */
/*      SPRB0 (0x20000560)                                                      */
/*      SPRHBM (0x20000560)                                                     */
/*      SPRC0 (0x20000560)                                                      */
/*      SPRMCC (0x20000560)                                                     */
/*      SPRUCC (0x20000560)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRB0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRHBM Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRC0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRMCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRUCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Access Control Read Policy
*/


#define ACRDPR5_N0_SB_PRVT_FBLP_REG 0x2C120560

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Read Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has read access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               read access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACRDPR5_N0_SB_PRVT_FBLP_STRUCT;

/* ACRDPR5_N1_SB_PRVT_FBLP_REG supported on:                                    */
/*      SPRA0 (0x20000564)                                                      */
/*      SPRB0 (0x20000564)                                                      */
/*      SPRHBM (0x20000564)                                                     */
/*      SPRC0 (0x20000564)                                                      */
/*      SPRMCC (0x20000564)                                                     */
/*      SPRUCC (0x20000564)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRB0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRHBM Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRC0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRMCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRUCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Access Control Read Policy
*/


#define ACRDPR5_N1_SB_PRVT_FBLP_REG 0x2C120564

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Read Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has read access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               read access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACRDPR5_N1_SB_PRVT_FBLP_STRUCT;

/* ACWRPR5_N0_SB_PRVT_FBLP_REG supported on:                                    */
/*      SPRA0 (0x20000568)                                                      */
/*      SPRB0 (0x20000568)                                                      */
/*      SPRHBM (0x20000568)                                                     */
/*      SPRC0 (0x20000568)                                                      */
/*      SPRMCC (0x20000568)                                                     */
/*      SPRUCC (0x20000568)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRB0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRHBM Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRC0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRMCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRUCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Access Control Write Policy
*/


#define ACWRPR5_N0_SB_PRVT_FBLP_REG 0x2C120568

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Write Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has write access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               write access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACWRPR5_N0_SB_PRVT_FBLP_STRUCT;

/* ACWRPR5_N1_SB_PRVT_FBLP_REG supported on:                                    */
/*      SPRA0 (0x2000056c)                                                      */
/*      SPRB0 (0x2000056c)                                                      */
/*      SPRHBM (0x2000056c)                                                     */
/*      SPRC0 (0x2000056c)                                                      */
/*      SPRMCC (0x2000056c)                                                     */
/*      SPRUCC (0x2000056c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRB0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRHBM Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRC0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRMCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRUCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Access Control Write Policy
*/


#define ACWRPR5_N1_SB_PRVT_FBLP_REG 0x2C12056C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Write Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has write access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               write access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACWRPR5_N1_SB_PRVT_FBLP_STRUCT;

/* ACCRPR5_N0_SB_PRVT_FBLP_REG supported on:                                    */
/*      SPRA0 (0x20000570)                                                      */
/*      SPRB0 (0x20000570)                                                      */
/*      SPRHBM (0x20000570)                                                     */
/*      SPRC0 (0x20000570)                                                      */
/*      SPRMCC (0x20000570)                                                     */
/*      SPRUCC (0x20000570)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRB0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRHBM Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRC0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRMCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRUCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Access Control CR Policy
*/


#define ACCRPR5_N0_SB_PRVT_FBLP_REG 0x2C120570

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC CR Policy value. The SAI on incoming
                               configuration requests is hashed down to 6 bits
                               and used as an index into the 64-bit of three AC
                               policy registers If bit[n] is set, SAIn has both
                               read and write access to all the corresponding
                               policy group registers, i.e. used to limit write
                               access to the RAC, WAC and CP registers. (read
                               access to these registers is always allowed)
                            */

  } Bits;
  UINT32 Data;

} ACCRPR5_N0_SB_PRVT_FBLP_STRUCT;

/* ACCRPR5_N1_SB_PRVT_FBLP_REG supported on:                                    */
/*      SPRA0 (0x20000574)                                                      */
/*      SPRB0 (0x20000574)                                                      */
/*      SPRHBM (0x20000574)                                                     */
/*      SPRC0 (0x20000574)                                                      */
/*      SPRMCC (0x20000574)                                                     */
/*      SPRUCC (0x20000574)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRB0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRHBM Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRC0 Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16           */
/* SPRMCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* SPRUCC Register File:    sprsp_top/pcie_ivyrn4x16[0]/rp0/msg_inst16          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Access Control CR Policy
*/


#define ACCRPR5_N1_SB_PRVT_FBLP_REG 0x2C120574

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC CR Policy value. The SAI on incoming
                               configuration requests is hashed down to 6 bits
                               and used as an index into the 64-bit of three AC
                               policy registers If bit[n] is set, SAIn has both
                               read and write access to all the corresponding
                               policy group registers, i.e. used to limit write
                               access to the RAC, WAC and CP registers. (read
                               access to these registers is always allowed)
                            */

  } Bits;
  UINT32 Data;

} ACCRPR5_N1_SB_PRVT_FBLP_STRUCT;







#endif /* _SB_PRVT_FBLP_h */
