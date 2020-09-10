
/** @file
  M3KTI_PMON1.h

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


#ifndef _M3KTI_PMON1_h
#define _M3KTI_PMON1_h
#include <Base.h>

/* VID_2_M3KTI_PMON1_REG supported on:                                          */
/*      SPRA0 (0x11e2a000)                                                      */
/*      SPRB0 (0x11e2a000)                                                      */
/*      SPRHBM (0x11e2a000)                                                     */
/*      SPRC0 (0x11e2a000)                                                      */
/*      SPRMCC (0x11e2a000)                                                     */
/*      SPRUCC (0x11e4c000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Vendor ID Register
*/


#define VID_2_M3KTI_PMON1_REG 0x0A210000

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

} VID_2_M3KTI_PMON1_STRUCT;

/* DID_2_M3KTI_PMON1_REG supported on:                                          */
/*      SPRA0 (0x11e2a002)                                                      */
/*      SPRB0 (0x11e2a002)                                                      */
/*      SPRHBM (0x11e2a002)                                                     */
/*      SPRC0 (0x11e2a002)                                                      */
/*      SPRMCC (0x11e2a002)                                                     */
/*      SPRUCC (0x11e4c002)                                                     */
/* Register default value on SPRA0: 0x00003247                                  */
/* Register default value on SPRB0: 0x00003247                                  */
/* Register default value on SPRHBM: 0x00003247                                 */
/* Register default value on SPRC0: 0x00003247                                  */
/* Register default value on SPRMCC: 0x00003247                                 */
/* Register default value on SPRUCC: 0x00003247                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Device Identification Number
*/


#define DID_2_M3KTI_PMON1_REG 0x0A210002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00003247*/

                            /*
                               The value is assigned by each IP/function owner
                               as a unique identifier.
                            */

  } Bits;
  UINT16 Data;

} DID_2_M3KTI_PMON1_STRUCT;

/* PCICMD_2_M3KTI_PMON1_REG supported on:                                       */
/*      SPRA0 (0x11e2a004)                                                      */
/*      SPRB0 (0x11e2a004)                                                      */
/*      SPRHBM (0x11e2a004)                                                     */
/*      SPRC0 (0x11e2a004)                                                      */
/*      SPRMCC (0x11e2a004)                                                     */
/*      SPRUCC (0x11e4c004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Command Register
*/


#define PCICMD_2_M3KTI_PMON1_REG 0x0A210004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 io_space_enable : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Hardwired to 0 since these devices dont decode
                               any IO BARs
                            */
    UINT16 memory_space_enable : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Hardwired to 0 since these devices dont decode
                               any memory BARs
                            */
    UINT16 bus_master_enable : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Hardwired to 0 since these devices dont generate
                               any transactions
                            */
    UINT16 special_cycle_enable : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /* Not applicable. Hardwired to 0. */
    UINT16 memory_write_and_invalidate_enable : 1;

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
    UINT16 parity_error_response : 1;

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
    UINT16 serr_enable : 1;

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
    UINT16 intx_disable : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /* N/A for these devices */
    UINT16 rsvd : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PCICMD_2_M3KTI_PMON1_STRUCT;

/* PCISTS_2_M3KTI_PMON1_REG supported on:                                       */
/*      SPRA0 (0x11e2a006)                                                      */
/*      SPRB0 (0x11e2a006)                                                      */
/*      SPRHBM (0x11e2a006)                                                     */
/*      SPRC0 (0x11e2a006)                                                      */
/*      SPRMCC (0x11e2a006)                                                     */
/*      SPRUCC (0x11e4c006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Status Register
*/


#define PCISTS_2_M3KTI_PMON1_REG 0x0A210006

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
                               Reflects the state of the INTA# signal at the
                               input of the enable/disable circuit. This bit is
                               set by HW to 1 when the INTA# is asserted. This
                               bit is reset by HW to 0 after the interrupt is
                               cleared (independent of the state of the
                               Interrupt Disable bit in the PCICMD register).
                               Hardwired to 0 on the processor
                            */
    UINT16 capabilities_list : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               This bit indicates the presence of a
                               capabilities list structure. When set to 1,
                               indicates the register at 34h provides an offset
                               into the function.
                            */
    UINT16 x66mhz_capable : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /* Not applicable to PCI Express. Hardwired to 0. */
    UINT16 reserved : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT16 fast_back_to_back : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /* Not applicable to PCI Express. Hardwired to 0. */
    UINT16 master_data_parity_error : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /* Hardwired to 0 */
    UINT16 devsel_timing : 2;

                            /* Bits[10:9], Access Type=RO, default=0x00000000*/

                            /* Not applicable to PCI Express. Hardwired to 0. */
    UINT16 signaled_target_abort : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /* Hardwired to 0 */
    UINT16 received_target_abort : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /* Hardwired to 0 */
    UINT16 received_master_abort : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /* Hardwired to 0 */
    UINT16 signaled_system_error : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /* Hardwired to 0 */
    UINT16 detected_parity_error : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               This bit is set when the device receives a
                               packet on the primary side with an uncorrectable
                               data error (including a packet with poison bit
                               set) or an uncorrectable address/control parity
                               error. The setting of this bit is regardless of
                               the Parity Error Response bit (PERRE) in the
                               PCICMD register.
                            */

  } Bits;
  UINT16 Data;

} PCISTS_2_M3KTI_PMON1_STRUCT;

/* RID_CCR_2_M3KTI_PMON1_REG supported on:                                      */
/*      SPRA0 (0x21e2a008)                                                      */
/*      SPRB0 (0x21e2a008)                                                      */
/*      SPRHBM (0x21e2a008)                                                     */
/*      SPRC0 (0x21e2a008)                                                      */
/*      SPRMCC (0x21e2a008)                                                     */
/*      SPRUCC (0x21e4c008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_2_M3KTI_PMON1_REG 0x0A220008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 revision_id : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Reflects the Uncore Revision ID after reset.
                               Reflects the Compatibility Revision ID if BIOS
                               updates.
                            */
    UINT32 register_level_programming_interface : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /* Register Level Programming Interface */
    UINT32 sub_class : 8;

                            /* Bits[23:16], Access Type=RO, default=0x00000080*/

                            /* Sub class code */
    UINT32 base_class : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000008*/

                            /* Base class code */

  } Bits;
  UINT32 Data;

} RID_CCR_2_M3KTI_PMON1_STRUCT;

/* CLSR_2_M3KTI_PMON1_REG supported on:                                         */
/*      SPRA0 (0x1e2a00c)                                                       */
/*      SPRB0 (0x1e2a00c)                                                       */
/*      SPRHBM (0x1e2a00c)                                                      */
/*      SPRC0 (0x1e2a00c)                                                       */
/*      SPRMCC (0x1e2a00c)                                                      */
/*      SPRUCC (0x1e4c00c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Cache Line Size Register
*/


#define CLSR_2_M3KTI_PMON1_REG 0x0A20000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 cacheline_size : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Size of Cacheline */

  } Bits;
  UINT8 Data;

} CLSR_2_M3KTI_PMON1_STRUCT;

/* PLAT_2_M3KTI_PMON1_REG supported on:                                         */
/*      SPRA0 (0x1e2a00d)                                                       */
/*      SPRB0 (0x1e2a00d)                                                       */
/*      SPRHBM (0x1e2a00d)                                                      */
/*      SPRC0 (0x1e2a00d)                                                       */
/*      SPRMCC (0x1e2a00d)                                                      */
/*      SPRUCC (0x1e4c00d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Latency Timer
*/


#define PLAT_2_M3KTI_PMON1_REG 0x0A20000D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 primary_latency_timer : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Not applicable to PCI-Express. Hardwired to 00h. */

  } Bits;
  UINT8 Data;

} PLAT_2_M3KTI_PMON1_STRUCT;

/* HDR_2_M3KTI_PMON1_REG supported on:                                          */
/*      SPRA0 (0x1e2a00e)                                                       */
/*      SPRB0 (0x1e2a00e)                                                       */
/*      SPRHBM (0x1e2a00e)                                                      */
/*      SPRC0 (0x1e2a00e)                                                       */
/*      SPRMCC (0x1e2a00e)                                                      */
/*      SPRUCC (0x1e4c00e)                                                      */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Header Type
*/


#define HDR_2_M3KTI_PMON1_REG 0x0A20000E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 configuration_layout : 7;

                            /* Bits[6:0], Access Type=RO, default=0x00000000*/

                            /* Type 0 header */
    UINT8 multi_function_device : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000001*/

                            /*
                               This bit defaults to 1b since all these devices
                               are multi-function
                            */

  } Bits;
  UINT8 Data;

} HDR_2_M3KTI_PMON1_STRUCT;

/* BIST_2_M3KTI_PMON1_REG supported on:                                         */
/*      SPRA0 (0x1e2a00f)                                                       */
/*      SPRB0 (0x1e2a00f)                                                       */
/*      SPRHBM (0x1e2a00f)                                                      */
/*      SPRC0 (0x1e2a00f)                                                       */
/*      SPRMCC (0x1e2a00f)                                                      */
/*      SPRUCC (0x1e4c00f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI BIST Register
*/


#define BIST_2_M3KTI_PMON1_REG 0x0A20000F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 bist_tests : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Not supported. Hardwired to 00h */

  } Bits;
  UINT8 Data;

} BIST_2_M3KTI_PMON1_STRUCT;

/* SVID_2_M3KTI_PMON1_REG supported on:                                         */
/*      SPRA0 (0x11e2a02c)                                                      */
/*      SPRB0 (0x11e2a02c)                                                      */
/*      SPRHBM (0x11e2a02c)                                                     */
/*      SPRC0 (0x11e2a02c)                                                      */
/*      SPRMCC (0x11e2a02c)                                                     */
/*      SPRUCC (0x11e4c02c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem Vendor ID Register
*/


#define SVID_2_M3KTI_PMON1_REG 0x0A21002C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 subsystem_vendor_identification_number : 16;

                            /* Bits[15:0], Access Type=RW/O, default=0x00008086*/

                            /*
                               The default value specifies Intel but can be set
                               to any value once after reset.
                            */

  } Bits;
  UINT16 Data;

} SVID_2_M3KTI_PMON1_STRUCT;

/* SDID_2_M3KTI_PMON1_REG supported on:                                         */
/*      SPRA0 (0x11e2a02e)                                                      */
/*      SPRB0 (0x11e2a02e)                                                      */
/*      SPRHBM (0x11e2a02e)                                                     */
/*      SPRC0 (0x11e2a02e)                                                      */
/*      SPRMCC (0x11e2a02e)                                                     */
/*      SPRUCC (0x11e4c02e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem ID Register
*/


#define SDID_2_M3KTI_PMON1_REG 0x0A21002E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 subsystem_identification_number : 16;

                            /* Bits[15:0], Access Type=RW/O, default=0x00000000*/

                            /*
                               Assigned by the subsystem vendor to uniquely
                               identify the subsystem
                            */

  } Bits;
  UINT16 Data;

} SDID_2_M3KTI_PMON1_STRUCT;

/* CAPPTR_2_M3KTI_PMON1_REG supported on:                                       */
/*      SPRA0 (0x1e2a034)                                                       */
/*      SPRB0 (0x1e2a034)                                                       */
/*      SPRHBM (0x1e2a034)                                                      */
/*      SPRC0 (0x1e2a034)                                                       */
/*      SPRMCC (0x1e2a034)                                                      */
/*      SPRUCC (0x1e4c034)                                                      */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Capability Pointer Register
*/


#define CAPPTR_2_M3KTI_PMON1_REG 0x0A200034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 capability_pointer : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000040*/

                            /*
                               Points to the first capability structure for the
                               device which is the PCIe capability.
                            */

  } Bits;
  UINT8 Data;

} CAPPTR_2_M3KTI_PMON1_STRUCT;

/* INTL_2_M3KTI_PMON1_REG supported on:                                         */
/*      SPRA0 (0x1e2a03c)                                                       */
/*      SPRB0 (0x1e2a03c)                                                       */
/*      SPRHBM (0x1e2a03c)                                                      */
/*      SPRC0 (0x1e2a03c)                                                       */
/*      SPRMCC (0x1e2a03c)                                                      */
/*      SPRUCC (0x1e4c03c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Line Register
*/


#define INTL_2_M3KTI_PMON1_REG 0x0A20003C

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

} INTL_2_M3KTI_PMON1_STRUCT;

/* INTPIN_2_M3KTI_PMON1_REG supported on:                                       */
/*      SPRA0 (0x1e2a03d)                                                       */
/*      SPRB0 (0x1e2a03d)                                                       */
/*      SPRHBM (0x1e2a03d)                                                      */
/*      SPRC0 (0x1e2a03d)                                                       */
/*      SPRMCC (0x1e2a03d)                                                      */
/*      SPRUCC (0x1e4c03d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Pin Register
*/


#define INTPIN_2_M3KTI_PMON1_REG 0x0A20003D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 interrupt_pin : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               N/A since these devices do not generate any
                               interrupt on their own
                            */

  } Bits;
  UINT8 Data;

} INTPIN_2_M3KTI_PMON1_STRUCT;

/* MINGNT_2_M3KTI_PMON1_REG supported on:                                       */
/*      SPRA0 (0x1e2a03e)                                                       */
/*      SPRB0 (0x1e2a03e)                                                       */
/*      SPRHBM (0x1e2a03e)                                                      */
/*      SPRC0 (0x1e2a03e)                                                       */
/*      SPRMCC (0x1e2a03e)                                                      */
/*      SPRUCC (0x1e4c03e)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Min Grant Register
*/


#define MINGNT_2_M3KTI_PMON1_REG 0x0A20003E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 mgv : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               The device does not burst as a PCI compliant
                               master.
                            */

  } Bits;
  UINT8 Data;

} MINGNT_2_M3KTI_PMON1_STRUCT;

/* MAXLAT_2_M3KTI_PMON1_REG supported on:                                       */
/*      SPRA0 (0x1e2a03f)                                                       */
/*      SPRB0 (0x1e2a03f)                                                       */
/*      SPRHBM (0x1e2a03f)                                                      */
/*      SPRC0 (0x1e2a03f)                                                       */
/*      SPRMCC (0x1e2a03f)                                                      */
/*      SPRUCC (0x1e4c03f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Max Latency Register
*/


#define MAXLAT_2_M3KTI_PMON1_REG 0x0A20003F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 mlv : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               The device has no specific requirements for how
                               often it needs to access the PCI bus.
                            */

  } Bits;
  UINT8 Data;

} MAXLAT_2_M3KTI_PMON1_STRUCT;

/* PXPCAP_2_M3KTI_PMON1_REG supported on:                                       */
/*      SPRA0 (0x21e2a040)                                                      */
/*      SPRB0 (0x21e2a040)                                                      */
/*      SPRHBM (0x21e2a040)                                                     */
/*      SPRC0 (0x21e2a040)                                                      */
/*      SPRMCC (0x21e2a040)                                                     */
/*      SPRUCC (0x21e4c040)                                                     */
/* Register default value on SPRA0: 0x00920010                                  */
/* Register default value on SPRB0: 0x00920010                                  */
/* Register default value on SPRHBM: 0x00920010                                 */
/* Register default value on SPRC0: 0x00920010                                  */
/* Register default value on SPRMCC: 0x00920010                                 */
/* Register default value on SPRUCC: 0x00920010                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Capability
*/


#define PXPCAP_2_M3KTI_PMON1_REG 0x0A220040

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

} PXPCAP_2_M3KTI_PMON1_STRUCT;

/* PCIE_RSVD0_2_M3KTI_PMON1_REG supported on:                                   */
/*      SPRA0 (0x21e2a044)                                                      */
/*      SPRB0 (0x21e2a044)                                                      */
/*      SPRHBM (0x21e2a044)                                                     */
/*      SPRC0 (0x21e2a044)                                                      */
/*      SPRMCC (0x21e2a044)                                                     */
/*      SPRUCC (0x21e4c044)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD0_2_M3KTI_PMON1_REG 0x0A220044

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD0_2_M3KTI_PMON1_STRUCT;

/* PCIE_RSVD1_2_N0_M3KTI_PMON1_REG supported on:                                */
/*      SPRA0 (0x21e2a048)                                                      */
/*      SPRB0 (0x21e2a048)                                                      */
/*      SPRHBM (0x21e2a048)                                                     */
/*      SPRC0 (0x21e2a048)                                                      */
/*      SPRMCC (0x21e2a048)                                                     */
/*      SPRUCC (0x21e4c048)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_2_N0_M3KTI_PMON1_REG 0x0A220048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD1_2_N0_M3KTI_PMON1_STRUCT;

/* PCIE_RSVD1_2_N1_M3KTI_PMON1_REG supported on:                                */
/*      SPRA0 (0x21e2a04c)                                                      */
/*      SPRB0 (0x21e2a04c)                                                      */
/*      SPRHBM (0x21e2a04c)                                                     */
/*      SPRC0 (0x21e2a04c)                                                      */
/*      SPRMCC (0x21e2a04c)                                                     */
/*      SPRUCC (0x21e4c04c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_2_N1_M3KTI_PMON1_REG 0x0A22004C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD1_2_N1_M3KTI_PMON1_STRUCT;

/* PCIE_RSVD2_2_N0_M3KTI_PMON1_REG supported on:                                */
/*      SPRA0 (0x21e2a050)                                                      */
/*      SPRB0 (0x21e2a050)                                                      */
/*      SPRHBM (0x21e2a050)                                                     */
/*      SPRC0 (0x21e2a050)                                                      */
/*      SPRMCC (0x21e2a050)                                                     */
/*      SPRUCC (0x21e4c050)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_2_N0_M3KTI_PMON1_REG 0x0A220050

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD2_2_N0_M3KTI_PMON1_STRUCT;

/* PCIE_RSVD2_2_N1_M3KTI_PMON1_REG supported on:                                */
/*      SPRA0 (0x21e2a054)                                                      */
/*      SPRB0 (0x21e2a054)                                                      */
/*      SPRHBM (0x21e2a054)                                                     */
/*      SPRC0 (0x21e2a054)                                                      */
/*      SPRMCC (0x21e2a054)                                                     */
/*      SPRUCC (0x21e4c054)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_2_N1_M3KTI_PMON1_REG 0x0A220054

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD2_2_N1_M3KTI_PMON1_STRUCT;

/* PCIE_RSVD3_2_N0_M3KTI_PMON1_REG supported on:                                */
/*      SPRA0 (0x21e2a058)                                                      */
/*      SPRB0 (0x21e2a058)                                                      */
/*      SPRHBM (0x21e2a058)                                                     */
/*      SPRC0 (0x21e2a058)                                                      */
/*      SPRMCC (0x21e2a058)                                                     */
/*      SPRUCC (0x21e4c058)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_2_N0_M3KTI_PMON1_REG 0x0A220058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD3_2_N0_M3KTI_PMON1_STRUCT;

/* PCIE_RSVD3_2_N1_M3KTI_PMON1_REG supported on:                                */
/*      SPRA0 (0x21e2a05c)                                                      */
/*      SPRB0 (0x21e2a05c)                                                      */
/*      SPRHBM (0x21e2a05c)                                                     */
/*      SPRC0 (0x21e2a05c)                                                      */
/*      SPRMCC (0x21e2a05c)                                                     */
/*      SPRUCC (0x21e4c05c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_2_N1_M3KTI_PMON1_REG 0x0A22005C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD3_2_N1_M3KTI_PMON1_STRUCT;

/* PCIE_RSVD4_2_N0_M3KTI_PMON1_REG supported on:                                */
/*      SPRA0 (0x21e2a060)                                                      */
/*      SPRB0 (0x21e2a060)                                                      */
/*      SPRHBM (0x21e2a060)                                                     */
/*      SPRC0 (0x21e2a060)                                                      */
/*      SPRMCC (0x21e2a060)                                                     */
/*      SPRUCC (0x21e4c060)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_2_N0_M3KTI_PMON1_REG 0x0A220060

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD4_2_N0_M3KTI_PMON1_STRUCT;

/* PCIE_RSVD4_2_N1_M3KTI_PMON1_REG supported on:                                */
/*      SPRA0 (0x21e2a064)                                                      */
/*      SPRB0 (0x21e2a064)                                                      */
/*      SPRHBM (0x21e2a064)                                                     */
/*      SPRC0 (0x21e2a064)                                                      */
/*      SPRMCC (0x21e2a064)                                                     */
/*      SPRUCC (0x21e4c064)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_2_N1_M3KTI_PMON1_REG 0x0A220064

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD4_2_N1_M3KTI_PMON1_STRUCT;

/* PCIE_RSVD5_2_N0_M3KTI_PMON1_REG supported on:                                */
/*      SPRA0 (0x21e2a068)                                                      */
/*      SPRB0 (0x21e2a068)                                                      */
/*      SPRHBM (0x21e2a068)                                                     */
/*      SPRC0 (0x21e2a068)                                                      */
/*      SPRMCC (0x21e2a068)                                                     */
/*      SPRUCC (0x21e4c068)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_2_N0_M3KTI_PMON1_REG 0x0A220068

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD5_2_N0_M3KTI_PMON1_STRUCT;

/* PCIE_RSVD5_2_N1_M3KTI_PMON1_REG supported on:                                */
/*      SPRA0 (0x21e2a06c)                                                      */
/*      SPRB0 (0x21e2a06c)                                                      */
/*      SPRHBM (0x21e2a06c)                                                     */
/*      SPRC0 (0x21e2a06c)                                                      */
/*      SPRMCC (0x21e2a06c)                                                     */
/*      SPRUCC (0x21e4c06c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_2_N1_M3KTI_PMON1_REG 0x0A22006C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD5_2_N1_M3KTI_PMON1_STRUCT;

/* PCIE_RSVD6_2_N0_M3KTI_PMON1_REG supported on:                                */
/*      SPRA0 (0x21e2a070)                                                      */
/*      SPRB0 (0x21e2a070)                                                      */
/*      SPRHBM (0x21e2a070)                                                     */
/*      SPRC0 (0x21e2a070)                                                      */
/*      SPRMCC (0x21e2a070)                                                     */
/*      SPRUCC (0x21e4c070)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_2_N0_M3KTI_PMON1_REG 0x0A220070

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD6_2_N0_M3KTI_PMON1_STRUCT;

/* PCIE_RSVD6_2_N1_M3KTI_PMON1_REG supported on:                                */
/*      SPRA0 (0x21e2a074)                                                      */
/*      SPRB0 (0x21e2a074)                                                      */
/*      SPRHBM (0x21e2a074)                                                     */
/*      SPRC0 (0x21e2a074)                                                      */
/*      SPRMCC (0x21e2a074)                                                     */
/*      SPRUCC (0x21e4c074)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_2_N1_M3KTI_PMON1_REG 0x0A220074

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD6_2_N1_M3KTI_PMON1_STRUCT;

/* PCIE_RSVD7_2_M3KTI_PMON1_REG supported on:                                   */
/*      SPRA0 (0x21e2a078)                                                      */
/*      SPRB0 (0x21e2a078)                                                      */
/*      SPRHBM (0x21e2a078)                                                     */
/*      SPRC0 (0x21e2a078)                                                      */
/*      SPRMCC (0x21e2a078)                                                     */
/*      SPRUCC (0x21e4c078)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD7_2_M3KTI_PMON1_REG 0x0A220078

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD7_2_M3KTI_PMON1_STRUCT;

/* PMONPRIVCTRL1_M3KTI_PMON1_REG supported on:                                  */
/*      SPRA0 (0x21e2a09c)                                                      */
/*      SPRB0 (0x21e2a09c)                                                      */
/*      SPRHBM (0x21e2a09c)                                                     */
/*      SPRC0 (0x21e2a09c)                                                      */
/*      SPRMCC (0x21e2a09c)                                                     */
/*      SPRUCC (0x21e4c09c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONPRIVCTRL1
*/


#define PMONPRIVCTRL1_M3KTI_PMON1_REG 0x0A22009C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmonselrtncbncs : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Mux-select to select whether we want to monitor
                               NCS or NCB route-through credit empty events of
                               the selected counter. Value of 0 implies we pick
                               the NCS channel and setting this bit to a 1 will
                               select the NCB channel
                            */
    UINT32 pmoncrdid : 7;

                            /* Bits[7:1], Access Type=RW, default=0x00000000*/

                            /*
                               This field allows us to select the appropriate
                               local or route-through credit counter empty
                               signal that we want for monitoring
                            */
    UINT32 sellocincagt : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this to 1 will allow us to bypass the
                               increment coming from the CMS and instead feed
                               the agent0 local increment into the PMON
                               counters
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMONPRIVCTRL1_M3KTI_PMON1_STRUCT;

/* PMONCNTRLOWER1_0_M3KTI_PMON1_REG supported on:                               */
/*      SPRA0 (0x21e2a0a8)                                                      */
/*      SPRB0 (0x21e2a0a8)                                                      */
/*      SPRHBM (0x21e2a0a8)                                                     */
/*      SPRC0 (0x21e2a0a8)                                                      */
/*      SPRMCC (0x21e2a0a8)                                                     */
/*      SPRUCC (0x21e4c0a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRLOWER1_0
*/


#define PMONCNTRLOWER1_0_M3KTI_PMON1_REG 0x0A2200A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmonctrdata : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* Pmon control data 0 */

  } Bits;
  UINT32 Data;

} PMONCNTRLOWER1_0_M3KTI_PMON1_STRUCT;

/* PMONCNTRUPPER1_0_M3KTI_PMON1_REG supported on:                               */
/*      SPRA0 (0x21e2a0ac)                                                      */
/*      SPRB0 (0x21e2a0ac)                                                      */
/*      SPRHBM (0x21e2a0ac)                                                     */
/*      SPRC0 (0x21e2a0ac)                                                      */
/*      SPRMCC (0x21e2a0ac)                                                     */
/*      SPRUCC (0x21e4c0ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRUPPER1_0
*/


#define PMONCNTRUPPER1_0_M3KTI_PMON1_REG 0x0A2200AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmonctrdata : 16;

                            /* Bits[15:0], Access Type=RW/V, default=0x00000000*/

                            /* Pmon control data upper1_0 */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMONCNTRUPPER1_0_M3KTI_PMON1_STRUCT;

/* PMONCNTRLOWER1_1_M3KTI_PMON1_REG supported on:                               */
/*      SPRA0 (0x21e2a0b0)                                                      */
/*      SPRB0 (0x21e2a0b0)                                                      */
/*      SPRHBM (0x21e2a0b0)                                                     */
/*      SPRC0 (0x21e2a0b0)                                                      */
/*      SPRMCC (0x21e2a0b0)                                                     */
/*      SPRUCC (0x21e4c0b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRLOWER1_1
*/


#define PMONCNTRLOWER1_1_M3KTI_PMON1_REG 0x0A2200B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmonctrdata : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* Pmon control data 1 */

  } Bits;
  UINT32 Data;

} PMONCNTRLOWER1_1_M3KTI_PMON1_STRUCT;

/* PMONCNTRUPPER1_1_M3KTI_PMON1_REG supported on:                               */
/*      SPRA0 (0x21e2a0b4)                                                      */
/*      SPRB0 (0x21e2a0b4)                                                      */
/*      SPRHBM (0x21e2a0b4)                                                     */
/*      SPRC0 (0x21e2a0b4)                                                      */
/*      SPRMCC (0x21e2a0b4)                                                     */
/*      SPRUCC (0x21e4c0b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRUPPER1_1
*/


#define PMONCNTRUPPER1_1_M3KTI_PMON1_REG 0x0A2200B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmonctrdata : 16;

                            /* Bits[15:0], Access Type=RW/V, default=0x00000000*/

                            /* Pmon control data upper1_1 */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMONCNTRUPPER1_1_M3KTI_PMON1_STRUCT;

/* PMONCNTRLOWER1_2_M3KTI_PMON1_REG supported on:                               */
/*      SPRA0 (0x21e2a0b8)                                                      */
/*      SPRB0 (0x21e2a0b8)                                                      */
/*      SPRHBM (0x21e2a0b8)                                                     */
/*      SPRC0 (0x21e2a0b8)                                                      */
/*      SPRMCC (0x21e2a0b8)                                                     */
/*      SPRUCC (0x21e4c0b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRLOWER1_2
*/


#define PMONCNTRLOWER1_2_M3KTI_PMON1_REG 0x0A2200B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmonctrdata : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* Pmon control data 2 */

  } Bits;
  UINT32 Data;

} PMONCNTRLOWER1_2_M3KTI_PMON1_STRUCT;

/* PMONCNTRUPPER1_2_M3KTI_PMON1_REG supported on:                               */
/*      SPRA0 (0x21e2a0bc)                                                      */
/*      SPRB0 (0x21e2a0bc)                                                      */
/*      SPRHBM (0x21e2a0bc)                                                     */
/*      SPRC0 (0x21e2a0bc)                                                      */
/*      SPRMCC (0x21e2a0bc)                                                     */
/*      SPRUCC (0x21e4c0bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRUPPER1_2
*/


#define PMONCNTRUPPER1_2_M3KTI_PMON1_REG 0x0A2200BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmonctrdata : 16;

                            /* Bits[15:0], Access Type=RW/V, default=0x00000000*/

                            /* Pmon control data upper1_2 */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMONCNTRUPPER1_2_M3KTI_PMON1_STRUCT;

/* PMONCNTRLOWER1_3_M3KTI_PMON1_REG supported on:                               */
/*      SPRA0 (0x21e2a0c0)                                                      */
/*      SPRB0 (0x21e2a0c0)                                                      */
/*      SPRHBM (0x21e2a0c0)                                                     */
/*      SPRC0 (0x21e2a0c0)                                                      */
/*      SPRMCC (0x21e2a0c0)                                                     */
/*      SPRUCC (0x21e4c0c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRLOWER1_3
*/


#define PMONCNTRLOWER1_3_M3KTI_PMON1_REG 0x0A2200C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmonctrdata : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* Pmon control data 3 */

  } Bits;
  UINT32 Data;

} PMONCNTRLOWER1_3_M3KTI_PMON1_STRUCT;

/* PMONCNTRUPPER1_3_M3KTI_PMON1_REG supported on:                               */
/*      SPRA0 (0x21e2a0c4)                                                      */
/*      SPRB0 (0x21e2a0c4)                                                      */
/*      SPRHBM (0x21e2a0c4)                                                     */
/*      SPRC0 (0x21e2a0c4)                                                      */
/*      SPRMCC (0x21e2a0c4)                                                     */
/*      SPRUCC (0x21e4c0c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRUPPER1_3
*/


#define PMONCNTRUPPER1_3_M3KTI_PMON1_REG 0x0A2200C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmonctrdata : 16;

                            /* Bits[15:0], Access Type=RW/V, default=0x00000000*/

                            /* Pmon control data upper1_3 */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMONCNTRUPPER1_3_M3KTI_PMON1_STRUCT;

/* PMONCTRSTATUS1_M3KTI_PMON1_REG supported on:                                 */
/*      SPRA0 (0x21e2a0f8)                                                      */
/*      SPRB0 (0x21e2a0f8)                                                      */
/*      SPRHBM (0x21e2a0f8)                                                     */
/*      SPRC0 (0x21e2a0f8)                                                      */
/*      SPRMCC (0x21e2a0f8)                                                     */
/*      SPRUCC (0x21e4c0f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCTRSTATUS1
*/


#define PMONCTRSTATUS1_M3KTI_PMON1_REG 0x0A2200F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter0ovf : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /* Counter 0 OVF */
    UINT32 counter1ovf : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /* Counter 1 OVF */
    UINT32 counter2ovf : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /* Counter 2 OVF */
    UINT32 counter3ovf : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /* Counter 3 OVF */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMONCTRSTATUS1_M3KTI_PMON1_STRUCT;

/* PXPENHCAP_2_M3KTI_PMON1_REG supported on:                                    */
/*      SPRA0 (0x21e2a100)                                                      */
/*      SPRB0 (0x21e2a100)                                                      */
/*      SPRHBM (0x21e2a100)                                                     */
/*      SPRC0 (0x21e2a100)                                                      */
/*      SPRMCC (0x21e2a100)                                                     */
/*      SPRUCC (0x21e4c100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F2      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F2           */
/* Struct generated from SPRA0 BDF: 30_5_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Enhanced Capability
*/


#define PXPENHCAP_2_M3KTI_PMON1_REG 0x0A220100

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

} PXPENHCAP_2_M3KTI_PMON1_STRUCT;
#endif /* _M3KTI_PMON1_h */
