
/** @file
  M3KTI_PMON.h

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


#ifndef _M3KTI_PMON_h
#define _M3KTI_PMON_h
#include <Base.h>

/* VID_1_M3KTI_PMON_REG supported on:                                           */
/*      SPRA0 (0x11e29000)                                                      */
/*      SPRB0 (0x11e29000)                                                      */
/*      SPRHBM (0x11e29000)                                                     */
/*      SPRC0 (0x11e29000)                                                      */
/*      SPRMCC (0x11e29000)                                                     */
/*      SPRUCC (0x11e4b000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Vendor ID Register
*/


#define VID_1_M3KTI_PMON_REG 0x0A110000

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

} VID_1_M3KTI_PMON_STRUCT;

/* DID_1_M3KTI_PMON_REG supported on:                                           */
/*      SPRA0 (0x11e29002)                                                      */
/*      SPRB0 (0x11e29002)                                                      */
/*      SPRHBM (0x11e29002)                                                     */
/*      SPRC0 (0x11e29002)                                                      */
/*      SPRMCC (0x11e29002)                                                     */
/*      SPRUCC (0x11e4b002)                                                     */
/* Register default value on SPRA0: 0x00003246                                  */
/* Register default value on SPRB0: 0x00003246                                  */
/* Register default value on SPRHBM: 0x00003246                                 */
/* Register default value on SPRC0: 0x00003246                                  */
/* Register default value on SPRMCC: 0x00003246                                 */
/* Register default value on SPRUCC: 0x00003246                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Device Identification Number
*/


#define DID_1_M3KTI_PMON_REG 0x0A110002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00003246*/

                            /*
                               The value is assigned by each IP/function owner
                               as a unique identifier.
                            */

  } Bits;
  UINT16 Data;

} DID_1_M3KTI_PMON_STRUCT;

/* PCICMD_1_M3KTI_PMON_REG supported on:                                        */
/*      SPRA0 (0x11e29004)                                                      */
/*      SPRB0 (0x11e29004)                                                      */
/*      SPRHBM (0x11e29004)                                                     */
/*      SPRC0 (0x11e29004)                                                      */
/*      SPRMCC (0x11e29004)                                                     */
/*      SPRUCC (0x11e4b004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Command Register
*/


#define PCICMD_1_M3KTI_PMON_REG 0x0A110004

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

} PCICMD_1_M3KTI_PMON_STRUCT;

/* PCISTS_1_M3KTI_PMON_REG supported on:                                        */
/*      SPRA0 (0x11e29006)                                                      */
/*      SPRB0 (0x11e29006)                                                      */
/*      SPRHBM (0x11e29006)                                                     */
/*      SPRC0 (0x11e29006)                                                      */
/*      SPRMCC (0x11e29006)                                                     */
/*      SPRUCC (0x11e4b006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Status Register
*/


#define PCISTS_1_M3KTI_PMON_REG 0x0A110006

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

} PCISTS_1_M3KTI_PMON_STRUCT;

/* RID_CCR_1_M3KTI_PMON_REG supported on:                                       */
/*      SPRA0 (0x21e29008)                                                      */
/*      SPRB0 (0x21e29008)                                                      */
/*      SPRHBM (0x21e29008)                                                     */
/*      SPRC0 (0x21e29008)                                                      */
/*      SPRMCC (0x21e29008)                                                     */
/*      SPRUCC (0x21e4b008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_1_M3KTI_PMON_REG 0x0A120008

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

} RID_CCR_1_M3KTI_PMON_STRUCT;

/* CLSR_1_M3KTI_PMON_REG supported on:                                          */
/*      SPRA0 (0x1e2900c)                                                       */
/*      SPRB0 (0x1e2900c)                                                       */
/*      SPRHBM (0x1e2900c)                                                      */
/*      SPRC0 (0x1e2900c)                                                       */
/*      SPRMCC (0x1e2900c)                                                      */
/*      SPRUCC (0x1e4b00c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Cache Line Size Register
*/


#define CLSR_1_M3KTI_PMON_REG 0x0A10000C

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

} CLSR_1_M3KTI_PMON_STRUCT;

/* PLAT_1_M3KTI_PMON_REG supported on:                                          */
/*      SPRA0 (0x1e2900d)                                                       */
/*      SPRB0 (0x1e2900d)                                                       */
/*      SPRHBM (0x1e2900d)                                                      */
/*      SPRC0 (0x1e2900d)                                                       */
/*      SPRMCC (0x1e2900d)                                                      */
/*      SPRUCC (0x1e4b00d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Latency Timer
*/


#define PLAT_1_M3KTI_PMON_REG 0x0A10000D

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

} PLAT_1_M3KTI_PMON_STRUCT;

/* HDR_1_M3KTI_PMON_REG supported on:                                           */
/*      SPRA0 (0x1e2900e)                                                       */
/*      SPRB0 (0x1e2900e)                                                       */
/*      SPRHBM (0x1e2900e)                                                      */
/*      SPRC0 (0x1e2900e)                                                       */
/*      SPRMCC (0x1e2900e)                                                      */
/*      SPRUCC (0x1e4b00e)                                                      */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Header Type
*/


#define HDR_1_M3KTI_PMON_REG 0x0A10000E

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

} HDR_1_M3KTI_PMON_STRUCT;

/* BIST_1_M3KTI_PMON_REG supported on:                                          */
/*      SPRA0 (0x1e2900f)                                                       */
/*      SPRB0 (0x1e2900f)                                                       */
/*      SPRHBM (0x1e2900f)                                                      */
/*      SPRC0 (0x1e2900f)                                                       */
/*      SPRMCC (0x1e2900f)                                                      */
/*      SPRUCC (0x1e4b00f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI BIST Register
*/


#define BIST_1_M3KTI_PMON_REG 0x0A10000F

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

} BIST_1_M3KTI_PMON_STRUCT;

/* SVID_1_M3KTI_PMON_REG supported on:                                          */
/*      SPRA0 (0x11e2902c)                                                      */
/*      SPRB0 (0x11e2902c)                                                      */
/*      SPRHBM (0x11e2902c)                                                     */
/*      SPRC0 (0x11e2902c)                                                      */
/*      SPRMCC (0x11e2902c)                                                     */
/*      SPRUCC (0x11e4b02c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem Vendor ID Register
*/


#define SVID_1_M3KTI_PMON_REG 0x0A11002C

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

} SVID_1_M3KTI_PMON_STRUCT;

/* SDID_1_M3KTI_PMON_REG supported on:                                          */
/*      SPRA0 (0x11e2902e)                                                      */
/*      SPRB0 (0x11e2902e)                                                      */
/*      SPRHBM (0x11e2902e)                                                     */
/*      SPRC0 (0x11e2902e)                                                      */
/*      SPRMCC (0x11e2902e)                                                     */
/*      SPRUCC (0x11e4b02e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem ID Register
*/


#define SDID_1_M3KTI_PMON_REG 0x0A11002E

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

} SDID_1_M3KTI_PMON_STRUCT;

/* CAPPTR_1_M3KTI_PMON_REG supported on:                                        */
/*      SPRA0 (0x1e29034)                                                       */
/*      SPRB0 (0x1e29034)                                                       */
/*      SPRHBM (0x1e29034)                                                      */
/*      SPRC0 (0x1e29034)                                                       */
/*      SPRMCC (0x1e29034)                                                      */
/*      SPRUCC (0x1e4b034)                                                      */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Capability Pointer Register
*/


#define CAPPTR_1_M3KTI_PMON_REG 0x0A100034

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

} CAPPTR_1_M3KTI_PMON_STRUCT;

/* INTL_1_M3KTI_PMON_REG supported on:                                          */
/*      SPRA0 (0x1e2903c)                                                       */
/*      SPRB0 (0x1e2903c)                                                       */
/*      SPRHBM (0x1e2903c)                                                      */
/*      SPRC0 (0x1e2903c)                                                       */
/*      SPRMCC (0x1e2903c)                                                      */
/*      SPRUCC (0x1e4b03c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Line Register
*/


#define INTL_1_M3KTI_PMON_REG 0x0A10003C

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

} INTL_1_M3KTI_PMON_STRUCT;

/* INTPIN_1_M3KTI_PMON_REG supported on:                                        */
/*      SPRA0 (0x1e2903d)                                                       */
/*      SPRB0 (0x1e2903d)                                                       */
/*      SPRHBM (0x1e2903d)                                                      */
/*      SPRC0 (0x1e2903d)                                                       */
/*      SPRMCC (0x1e2903d)                                                      */
/*      SPRUCC (0x1e4b03d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Pin Register
*/


#define INTPIN_1_M3KTI_PMON_REG 0x0A10003D

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

} INTPIN_1_M3KTI_PMON_STRUCT;

/* MINGNT_1_M3KTI_PMON_REG supported on:                                        */
/*      SPRA0 (0x1e2903e)                                                       */
/*      SPRB0 (0x1e2903e)                                                       */
/*      SPRHBM (0x1e2903e)                                                      */
/*      SPRC0 (0x1e2903e)                                                       */
/*      SPRMCC (0x1e2903e)                                                      */
/*      SPRUCC (0x1e4b03e)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Min Grant Register
*/


#define MINGNT_1_M3KTI_PMON_REG 0x0A10003E

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

} MINGNT_1_M3KTI_PMON_STRUCT;

/* MAXLAT_1_M3KTI_PMON_REG supported on:                                        */
/*      SPRA0 (0x1e2903f)                                                       */
/*      SPRB0 (0x1e2903f)                                                       */
/*      SPRHBM (0x1e2903f)                                                      */
/*      SPRC0 (0x1e2903f)                                                       */
/*      SPRMCC (0x1e2903f)                                                      */
/*      SPRUCC (0x1e4b03f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Max Latency Register
*/


#define MAXLAT_1_M3KTI_PMON_REG 0x0A10003F

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

} MAXLAT_1_M3KTI_PMON_STRUCT;

/* PXPCAP_1_M3KTI_PMON_REG supported on:                                        */
/*      SPRA0 (0x21e29040)                                                      */
/*      SPRB0 (0x21e29040)                                                      */
/*      SPRHBM (0x21e29040)                                                     */
/*      SPRC0 (0x21e29040)                                                      */
/*      SPRMCC (0x21e29040)                                                     */
/*      SPRUCC (0x21e4b040)                                                     */
/* Register default value on SPRA0: 0x00920010                                  */
/* Register default value on SPRB0: 0x00920010                                  */
/* Register default value on SPRHBM: 0x00920010                                 */
/* Register default value on SPRC0: 0x00920010                                  */
/* Register default value on SPRMCC: 0x00920010                                 */
/* Register default value on SPRUCC: 0x00920010                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Capability
*/


#define PXPCAP_1_M3KTI_PMON_REG 0x0A120040

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

} PXPCAP_1_M3KTI_PMON_STRUCT;

/* PCIE_RSVD0_1_M3KTI_PMON_REG supported on:                                    */
/*      SPRA0 (0x21e29044)                                                      */
/*      SPRB0 (0x21e29044)                                                      */
/*      SPRHBM (0x21e29044)                                                     */
/*      SPRC0 (0x21e29044)                                                      */
/*      SPRMCC (0x21e29044)                                                     */
/*      SPRUCC (0x21e4b044)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD0_1_M3KTI_PMON_REG 0x0A120044

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

} PCIE_RSVD0_1_M3KTI_PMON_STRUCT;

/* PCIE_RSVD1_1_N0_M3KTI_PMON_REG supported on:                                 */
/*      SPRA0 (0x21e29048)                                                      */
/*      SPRB0 (0x21e29048)                                                      */
/*      SPRHBM (0x21e29048)                                                     */
/*      SPRC0 (0x21e29048)                                                      */
/*      SPRMCC (0x21e29048)                                                     */
/*      SPRUCC (0x21e4b048)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_1_N0_M3KTI_PMON_REG 0x0A120048

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

} PCIE_RSVD1_1_N0_M3KTI_PMON_STRUCT;

/* PCIE_RSVD1_1_N1_M3KTI_PMON_REG supported on:                                 */
/*      SPRA0 (0x21e2904c)                                                      */
/*      SPRB0 (0x21e2904c)                                                      */
/*      SPRHBM (0x21e2904c)                                                     */
/*      SPRC0 (0x21e2904c)                                                      */
/*      SPRMCC (0x21e2904c)                                                     */
/*      SPRUCC (0x21e4b04c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_1_N1_M3KTI_PMON_REG 0x0A12004C

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

} PCIE_RSVD1_1_N1_M3KTI_PMON_STRUCT;

/* PCIE_RSVD2_1_N0_M3KTI_PMON_REG supported on:                                 */
/*      SPRA0 (0x21e29050)                                                      */
/*      SPRB0 (0x21e29050)                                                      */
/*      SPRHBM (0x21e29050)                                                     */
/*      SPRC0 (0x21e29050)                                                      */
/*      SPRMCC (0x21e29050)                                                     */
/*      SPRUCC (0x21e4b050)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_1_N0_M3KTI_PMON_REG 0x0A120050

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

} PCIE_RSVD2_1_N0_M3KTI_PMON_STRUCT;

/* PCIE_RSVD2_1_N1_M3KTI_PMON_REG supported on:                                 */
/*      SPRA0 (0x21e29054)                                                      */
/*      SPRB0 (0x21e29054)                                                      */
/*      SPRHBM (0x21e29054)                                                     */
/*      SPRC0 (0x21e29054)                                                      */
/*      SPRMCC (0x21e29054)                                                     */
/*      SPRUCC (0x21e4b054)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_1_N1_M3KTI_PMON_REG 0x0A120054

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

} PCIE_RSVD2_1_N1_M3KTI_PMON_STRUCT;

/* PCIE_RSVD3_1_N0_M3KTI_PMON_REG supported on:                                 */
/*      SPRA0 (0x21e29058)                                                      */
/*      SPRB0 (0x21e29058)                                                      */
/*      SPRHBM (0x21e29058)                                                     */
/*      SPRC0 (0x21e29058)                                                      */
/*      SPRMCC (0x21e29058)                                                     */
/*      SPRUCC (0x21e4b058)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_1_N0_M3KTI_PMON_REG 0x0A120058

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

} PCIE_RSVD3_1_N0_M3KTI_PMON_STRUCT;

/* PCIE_RSVD3_1_N1_M3KTI_PMON_REG supported on:                                 */
/*      SPRA0 (0x21e2905c)                                                      */
/*      SPRB0 (0x21e2905c)                                                      */
/*      SPRHBM (0x21e2905c)                                                     */
/*      SPRC0 (0x21e2905c)                                                      */
/*      SPRMCC (0x21e2905c)                                                     */
/*      SPRUCC (0x21e4b05c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_1_N1_M3KTI_PMON_REG 0x0A12005C

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

} PCIE_RSVD3_1_N1_M3KTI_PMON_STRUCT;

/* PCIE_RSVD4_1_N0_M3KTI_PMON_REG supported on:                                 */
/*      SPRA0 (0x21e29060)                                                      */
/*      SPRB0 (0x21e29060)                                                      */
/*      SPRHBM (0x21e29060)                                                     */
/*      SPRC0 (0x21e29060)                                                      */
/*      SPRMCC (0x21e29060)                                                     */
/*      SPRUCC (0x21e4b060)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_1_N0_M3KTI_PMON_REG 0x0A120060

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

} PCIE_RSVD4_1_N0_M3KTI_PMON_STRUCT;

/* PCIE_RSVD4_1_N1_M3KTI_PMON_REG supported on:                                 */
/*      SPRA0 (0x21e29064)                                                      */
/*      SPRB0 (0x21e29064)                                                      */
/*      SPRHBM (0x21e29064)                                                     */
/*      SPRC0 (0x21e29064)                                                      */
/*      SPRMCC (0x21e29064)                                                     */
/*      SPRUCC (0x21e4b064)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_1_N1_M3KTI_PMON_REG 0x0A120064

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

} PCIE_RSVD4_1_N1_M3KTI_PMON_STRUCT;

/* PCIE_RSVD5_1_N0_M3KTI_PMON_REG supported on:                                 */
/*      SPRA0 (0x21e29068)                                                      */
/*      SPRB0 (0x21e29068)                                                      */
/*      SPRHBM (0x21e29068)                                                     */
/*      SPRC0 (0x21e29068)                                                      */
/*      SPRMCC (0x21e29068)                                                     */
/*      SPRUCC (0x21e4b068)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_1_N0_M3KTI_PMON_REG 0x0A120068

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

} PCIE_RSVD5_1_N0_M3KTI_PMON_STRUCT;

/* PCIE_RSVD5_1_N1_M3KTI_PMON_REG supported on:                                 */
/*      SPRA0 (0x21e2906c)                                                      */
/*      SPRB0 (0x21e2906c)                                                      */
/*      SPRHBM (0x21e2906c)                                                     */
/*      SPRC0 (0x21e2906c)                                                      */
/*      SPRMCC (0x21e2906c)                                                     */
/*      SPRUCC (0x21e4b06c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_1_N1_M3KTI_PMON_REG 0x0A12006C

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

} PCIE_RSVD5_1_N1_M3KTI_PMON_STRUCT;

/* PCIE_RSVD6_1_N0_M3KTI_PMON_REG supported on:                                 */
/*      SPRA0 (0x21e29070)                                                      */
/*      SPRB0 (0x21e29070)                                                      */
/*      SPRHBM (0x21e29070)                                                     */
/*      SPRC0 (0x21e29070)                                                      */
/*      SPRMCC (0x21e29070)                                                     */
/*      SPRUCC (0x21e4b070)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_1_N0_M3KTI_PMON_REG 0x0A120070

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

} PCIE_RSVD6_1_N0_M3KTI_PMON_STRUCT;

/* PCIE_RSVD6_1_N1_M3KTI_PMON_REG supported on:                                 */
/*      SPRA0 (0x21e29074)                                                      */
/*      SPRB0 (0x21e29074)                                                      */
/*      SPRHBM (0x21e29074)                                                     */
/*      SPRC0 (0x21e29074)                                                      */
/*      SPRMCC (0x21e29074)                                                     */
/*      SPRUCC (0x21e4b074)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_1_N1_M3KTI_PMON_REG 0x0A120074

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

} PCIE_RSVD6_1_N1_M3KTI_PMON_STRUCT;

/* PCIE_RSVD7_1_M3KTI_PMON_REG supported on:                                    */
/*      SPRA0 (0x21e29078)                                                      */
/*      SPRB0 (0x21e29078)                                                      */
/*      SPRHBM (0x21e29078)                                                     */
/*      SPRC0 (0x21e29078)                                                      */
/*      SPRMCC (0x21e29078)                                                     */
/*      SPRUCC (0x21e4b078)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD7_1_M3KTI_PMON_REG 0x0A120078

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

} PCIE_RSVD7_1_M3KTI_PMON_STRUCT;

/* PMONPRIVCTRL0_M3KTI_PMON_REG supported on:                                   */
/*      SPRA0 (0x21e2909c)                                                      */
/*      SPRB0 (0x21e2909c)                                                      */
/*      SPRHBM (0x21e2909c)                                                     */
/*      SPRC0 (0x21e2909c)                                                      */
/*      SPRMCC (0x21e2909c)                                                     */
/*      SPRUCC (0x21e4b09c)                                                     */
/* Register default value on SPRA0: 0x08642000                                  */
/* Register default value on SPRB0: 0x08642000                                  */
/* Register default value on SPRHBM: 0x08642000                                 */
/* Register default value on SPRC0: 0x08642000                                  */
/* Register default value on SPRMCC: 0x08642000                                 */
/* Register default value on SPRUCC: 0x08642000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONPRIVCTRL0
*/


#define PMONPRIVCTRL0_M3KTI_PMON_REG 0x0A12009C

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
    UINT32 pmoniioid0 : 4;

                            /* Bits[12:9], Access Type=RW, default=0x00000000*/

                            /*
                               This field allows us to select the appropriate
                               IIO stack for monitoring. There are six such
                               fields, four of which allow all stack IDs and
                               two allow stack IDs 0-7.
                            */
    UINT32 pmoniioid1 : 4;

                            /* Bits[16:13], Access Type=RW, default=0x00000001*/

                            /*
                               This field allows us to select the appropriate
                               IIO stack for monitoring. There are six such
                               fields, four of which allow all stack IDs and
                               two allow stack IDs 0-7.
                            */
    UINT32 pmoniioid2 : 4;

                            /* Bits[20:17], Access Type=RW, default=0x00000002*/

                            /*
                               This field allows us to select the appropriate
                               IIO stack for monitoring. There are six such
                               fields, four of which allow all stack IDs and
                               two allow stack IDs 0-7.
                            */
    UINT32 pmoniioid3 : 4;

                            /* Bits[24:21], Access Type=RW, default=0x00000003*/

                            /*
                               This field allows us to select the appropriate
                               IIO stack for monitoring. There are six such
                               fields, four of which allow all stack IDs and
                               two allow stack IDs 0-7.
                            */
    UINT32 pmoniioid4 : 3;

                            /* Bits[27:25], Access Type=RW, default=0x00000004*/

                            /*
                               This field allows us to select the appropriate
                               IIO stack for monitoring. There are six such
                               fields, four of which allow all stack IDs and
                               two allow stack IDs 0-7.
                            */
    UINT32 pmoniioid5 : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               This field allows us to select the appropriate
                               IIO stack for monitoring. There are six such
                               fields, four of which allow all stack IDs and
                               two allow stack IDs 0-7.
                            */
    UINT32 rsvd : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMONPRIVCTRL0_M3KTI_PMON_STRUCT;

/* PMONUNITCTRL0_M3KTI_PMON_REG supported on:                                   */
/*      SPRA0 (0x21e290a0)                                                      */
/*      SPRB0 (0x21e290a0)                                                      */
/*      SPRHBM (0x21e290a0)                                                     */
/*      SPRC0 (0x21e290a0)                                                      */
/*      SPRMCC (0x21e290a0)                                                     */
/*      SPRUCC (0x21e4b0a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONUNITCTRL0
*/


#define PMONUNITCTRL0_M3KTI_PMON_REG 0x0A1200A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 freezecounters : 1;

                            /* Bits[0:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               This bit is written to when the counters should
                               be frozen. If this bit is written to and freeze
                               is enabled, the counters in the unit will stop
                               counting.
                            */
    UINT32 rsvd : 7;

                            /* Bits[7:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 resetcounterconfigs : 1;

                            /* Bits[8:8], Access Type=WO, default=0x00000000*/

                            /*
                               When this bit is written to, the counter
                               configuration registers will be reset. This does
                               not effect the values in the counters.
                            */
    UINT32 resetcounters : 1;

                            /* Bits[9:9], Access Type=WO, default=0x00000000*/

                            /*
                               When this bit is written to, the counters data
                               fields will be reset. The configuration values
                               will not be reset. Note: The counters should be
                               frozen (KTIPMONUNITCTRL.freezecounters=1) before
                               writing resetcounters=1.
                            */
    UINT32 rsvd_10 : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMONUNITCTRL0_M3KTI_PMON_STRUCT;

/* PMONCNTRLOWER0_0_M3KTI_PMON_REG supported on:                                */
/*      SPRA0 (0x21e290a8)                                                      */
/*      SPRB0 (0x21e290a8)                                                      */
/*      SPRHBM (0x21e290a8)                                                     */
/*      SPRC0 (0x21e290a8)                                                      */
/*      SPRMCC (0x21e290a8)                                                     */
/*      SPRUCC (0x21e4b0a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRLOWER0_0
*/


#define PMONCNTRLOWER0_0_M3KTI_PMON_REG 0x0A1200A8

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

} PMONCNTRLOWER0_0_M3KTI_PMON_STRUCT;

/* PMONCNTRUPPER0_0_M3KTI_PMON_REG supported on:                                */
/*      SPRA0 (0x21e290ac)                                                      */
/*      SPRB0 (0x21e290ac)                                                      */
/*      SPRHBM (0x21e290ac)                                                     */
/*      SPRC0 (0x21e290ac)                                                      */
/*      SPRMCC (0x21e290ac)                                                     */
/*      SPRUCC (0x21e4b0ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRUPPER0_0
*/


#define PMONCNTRUPPER0_0_M3KTI_PMON_REG 0x0A1200AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmonctrdata : 16;

                            /* Bits[15:0], Access Type=RW/V, default=0x00000000*/

                            /* Pmon control data upper */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMONCNTRUPPER0_0_M3KTI_PMON_STRUCT;

/* PMONCNTRLOWER0_1_M3KTI_PMON_REG supported on:                                */
/*      SPRA0 (0x21e290b0)                                                      */
/*      SPRB0 (0x21e290b0)                                                      */
/*      SPRHBM (0x21e290b0)                                                     */
/*      SPRC0 (0x21e290b0)                                                      */
/*      SPRMCC (0x21e290b0)                                                     */
/*      SPRUCC (0x21e4b0b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRLOWER0_1
*/


#define PMONCNTRLOWER0_1_M3KTI_PMON_REG 0x0A1200B0

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

} PMONCNTRLOWER0_1_M3KTI_PMON_STRUCT;

/* PMONCNTRUPPER0_1_M3KTI_PMON_REG supported on:                                */
/*      SPRA0 (0x21e290b4)                                                      */
/*      SPRB0 (0x21e290b4)                                                      */
/*      SPRHBM (0x21e290b4)                                                     */
/*      SPRC0 (0x21e290b4)                                                      */
/*      SPRMCC (0x21e290b4)                                                     */
/*      SPRUCC (0x21e4b0b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRUPPER0_1
*/


#define PMONCNTRUPPER0_1_M3KTI_PMON_REG 0x0A1200B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmonctrdata : 16;

                            /* Bits[15:0], Access Type=RW/V, default=0x00000000*/

                            /* Pmon control data upper_1 */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMONCNTRUPPER0_1_M3KTI_PMON_STRUCT;

/* PMONCNTRLOWER0_2_M3KTI_PMON_REG supported on:                                */
/*      SPRA0 (0x21e290b8)                                                      */
/*      SPRB0 (0x21e290b8)                                                      */
/*      SPRHBM (0x21e290b8)                                                     */
/*      SPRC0 (0x21e290b8)                                                      */
/*      SPRMCC (0x21e290b8)                                                     */
/*      SPRUCC (0x21e4b0b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRLOWER0_2
*/


#define PMONCNTRLOWER0_2_M3KTI_PMON_REG 0x0A1200B8

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

} PMONCNTRLOWER0_2_M3KTI_PMON_STRUCT;

/* PMONCNTRUPPER0_2_M3KTI_PMON_REG supported on:                                */
/*      SPRA0 (0x21e290bc)                                                      */
/*      SPRB0 (0x21e290bc)                                                      */
/*      SPRHBM (0x21e290bc)                                                     */
/*      SPRC0 (0x21e290bc)                                                      */
/*      SPRMCC (0x21e290bc)                                                     */
/*      SPRUCC (0x21e4b0bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRUPPER0_2
*/


#define PMONCNTRUPPER0_2_M3KTI_PMON_REG 0x0A1200BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmonctrdata : 16;

                            /* Bits[15:0], Access Type=RW/V, default=0x00000000*/

                            /* Pmon control data upper_2 */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMONCNTRUPPER0_2_M3KTI_PMON_STRUCT;

/* PMONCNTRLOWER0_3_M3KTI_PMON_REG supported on:                                */
/*      SPRA0 (0x21e290c0)                                                      */
/*      SPRB0 (0x21e290c0)                                                      */
/*      SPRHBM (0x21e290c0)                                                     */
/*      SPRC0 (0x21e290c0)                                                      */
/*      SPRMCC (0x21e290c0)                                                     */
/*      SPRUCC (0x21e4b0c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRLOWER0_3
*/


#define PMONCNTRLOWER0_3_M3KTI_PMON_REG 0x0A1200C0

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

} PMONCNTRLOWER0_3_M3KTI_PMON_STRUCT;

/* PMONCNTRUPPER0_3_M3KTI_PMON_REG supported on:                                */
/*      SPRA0 (0x21e290c4)                                                      */
/*      SPRB0 (0x21e290c4)                                                      */
/*      SPRHBM (0x21e290c4)                                                     */
/*      SPRC0 (0x21e290c4)                                                      */
/*      SPRMCC (0x21e290c4)                                                     */
/*      SPRUCC (0x21e4b0c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRUPPER0_3
*/


#define PMONCNTRUPPER0_3_M3KTI_PMON_REG 0x0A1200C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmonctrdata : 16;

                            /* Bits[15:0], Access Type=RW/V, default=0x00000000*/

                            /* Pmon control data upper_3 */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMONCNTRUPPER0_3_M3KTI_PMON_STRUCT;

/* PMONCNTRCFG0_0_N0_M3KTI_PMON_REG supported on:                               */
/*      SPRA0 (0x21e290d8)                                                      */
/*      SPRB0 (0x21e290d8)                                                      */
/*      SPRHBM (0x21e290d8)                                                     */
/*      SPRC0 (0x21e290d8)                                                      */
/*      SPRMCC (0x21e290d8)                                                     */
/*      SPRUCC (0x21e4b0d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRCFG0_0
*/


#define PMONCNTRCFG0_0_N0_M3KTI_PMON_REG 0x0A1200D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 eventselect : 8;

                            /* Bits[7:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               This field is used to decode the PerfMon event
                               which is selected.
                            */
    UINT32 unitmask : 8;

                            /* Bits[15:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This mask selects the sub-events to be selected
                               for creation of the event (for applicable
                               events). See Pmon events for details.
                            */
    UINT32 rsvd_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 counterreset : 1;

                            /* Bits[17:17], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When this bit is set, the corresponding counter
                               will be reset to 0. This allows for a quick
                               reset of the counter when changing event
                               encodings.
                            */
    UINT32 edgedetect : 1;

                            /* Bits[18:18], Access Type=RW/V, default=0x00000000*/

                            /*
                               Edge Detect allows counting either 0 to 1 (if
                               invert=0) or 1 to 0 (if invert=1) transitions of
                               a given event. For example, there is an event
                               that counts the number of cycles in L1 power
                               state in Intel UPI. By using edge detect, one
                               can count the number of times that we entered L1
                               state (by detecting the rising edge by setting
                               invert=0). Edge detect only works in conjunction
                               with threshholding. This is true even for events
                               that can only increment by 1 in a given cycle
                               (like the L1 example above). In this case, one
                               should set a threshold of 1 (with invert=0). One
                               can also use Edge Detect with queue occupancy
                               events. For example, if one wanted to count the
                               number of times when the TXQ occupancy was
                               greater than or equal to 5, one would select the
                               TXQ occupancy event with a threshold of 5 and
                               set the Edge Detect bit (and invert=0).
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 frz_on_ov : 1;

                            /* Bits[20:20], Access Type=RW/V, default=0x00000000*/

                            /*
                               This bit will be sent in the GPSB message on a
                               pmon overflow to indicate whether or not to
                               freeze when an overflow occurs.
                            */
    UINT32 rsvd : 1;

                            /* Bits[21:21], Access Type=RW/V, default=0x00000000*/

                            /*
                               Bit was defined as Internal control bit, but
                               this function is dropped in SKX, and bit is now
                               Reserved.
                            */
    UINT32 rsvd_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 invert : 1;

                            /* Bits[23:23], Access Type=RW/V, default=0x00000000*/

                            /*
                               This bit indicates how the threshold field will
                               be compared to the incoming event. When 0, the
                               comparison that will be done is threshold
                               greater than or equal to event. When set to 1,
                               the comparison that will be done is inverted
                               from the case where this bit is set to 0, i.e.,
                               threshold less than event. The invert bit only
                               works when Threshold != 0. So, if one would like
                               to invert a non-occupancy event, one needs to
                               set the threshold to 1.
                            */
    UINT32 thresh : 8;

                            /* Bits[31:24], Access Type=RW/V, default=0x00000000*/

                            /*
                               This field is compared directly against an
                               incoming event value for events that can
                               increment by 1 or more in a given cycle. The
                               result of the comparison is effectively a 1 bit
                               wide event, i.e., the counter will be
                               incremented by 1 when the comparison is true
                               (the type of comparison depends on the setting
                               of the invert bit - see bit 23) no matter how
                               wide the original event was. When this field is
                               zero, threshold comparison is disabled and the
                               event is passed without modification.
                            */

  } Bits;
  UINT32 Data;

} PMONCNTRCFG0_0_N0_M3KTI_PMON_STRUCT;

/* PMONCNTRCFG0_0_N1_M3KTI_PMON_REG supported on:                               */
/*      SPRA0 (0x21e290dc)                                                      */
/*      SPRB0 (0x21e290dc)                                                      */
/*      SPRHBM (0x21e290dc)                                                     */
/*      SPRC0 (0x21e290dc)                                                      */
/*      SPRMCC (0x21e290dc)                                                     */
/*      SPRUCC (0x21e4b0dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRCFG0_0
*/


#define PMONCNTRCFG0_0_N1_M3KTI_PMON_REG 0x0A1200DC

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

} PMONCNTRCFG0_0_N1_M3KTI_PMON_STRUCT;

/* PMONCNTRCFG0_1_N0_M3KTI_PMON_REG supported on:                               */
/*      SPRA0 (0x21e290e0)                                                      */
/*      SPRB0 (0x21e290e0)                                                      */
/*      SPRHBM (0x21e290e0)                                                     */
/*      SPRC0 (0x21e290e0)                                                      */
/*      SPRMCC (0x21e290e0)                                                     */
/*      SPRUCC (0x21e4b0e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRCFG0_1
*/


#define PMONCNTRCFG0_1_N0_M3KTI_PMON_REG 0x0A1200E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 eventselect : 8;

                            /* Bits[7:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               This field is used to decode the PerfMon event
                               which is selected.
                            */
    UINT32 unitmask : 8;

                            /* Bits[15:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This mask selects the sub-events to be selected
                               for creation of the event (for applicable
                               events). See Pmon events for details.
                            */
    UINT32 rsvd_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 counterreset : 1;

                            /* Bits[17:17], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When this bit is set, the corresponding counter
                               will be reset to 0. This allows for a quick
                               reset of the counter when changing event
                               encodings.
                            */
    UINT32 edgedetect : 1;

                            /* Bits[18:18], Access Type=RW/V, default=0x00000000*/

                            /*
                               Edge Detect allows counting either 0 to 1 (if
                               invert=0) or 1 to 0 (if invert=1) transitions of
                               a given event. For example, there is an event
                               that counts the number of cycles in L1 power
                               state in Intel UPI. By using edge detect, one
                               can count the number of times that we entered L1
                               state (by detecting the rising edge by setting
                               invert=0). Edge detect only works in conjunction
                               with threshholding. This is true even for events
                               that can only increment by 1 in a given cycle
                               (like the L1 example above). In this case, one
                               should set a threshold of 1 (with invert=0). One
                               can also use Edge Detect with queue occupancy
                               events. For example, if one wanted to count the
                               number of times when the TXQ occupancy was
                               greater than or equal to 5, one would select the
                               TXQ occupancy event with a threshold of 5 and
                               set the Edge Detect bit (and invert=0).
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 frz_on_ov : 1;

                            /* Bits[20:20], Access Type=RW/V, default=0x00000000*/

                            /*
                               This bit will be sent in the GPSB message on a
                               pmon overflow to indicate whether or not to
                               freeze when an overflow occurs.
                            */
    UINT32 rsvd : 1;

                            /* Bits[21:21], Access Type=RW/V, default=0x00000000*/

                            /*
                               Bit was defined as Internal control bit, but
                               this function is dropped in SKX, and bit is now
                               Reserved.
                            */
    UINT32 rsvd_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 invert : 1;

                            /* Bits[23:23], Access Type=RW/V, default=0x00000000*/

                            /*
                               This bit indicates how the threshold field will
                               be compared to the incoming event. When 0, the
                               comparison that will be done is threshold
                               greater than or equal to event. When set to 1,
                               the comparison that will be done is inverted
                               from the case where this bit is set to 0, i.e.,
                               threshold less than event. The invert bit only
                               works when Threshold != 0. So, if one would like
                               to invert a non-occupancy event, one needs to
                               set the threshold to 1.
                            */
    UINT32 thresh : 8;

                            /* Bits[31:24], Access Type=RW/V, default=0x00000000*/

                            /*
                               This field is compared directly against an
                               incoming event value for events that can
                               increment by 1 or more in a given cycle. The
                               result of the comparison is effectively a 1 bit
                               wide event, i.e., the counter will be
                               incremented by 1 when the comparison is true
                               (the type of comparison depends on the setting
                               of the invert bit - see bit 23) no matter how
                               wide the original event was. When this field is
                               zero, threshold comparison is disabled and the
                               event is passed without modification.
                            */

  } Bits;
  UINT32 Data;

} PMONCNTRCFG0_1_N0_M3KTI_PMON_STRUCT;

/* PMONCNTRCFG0_1_N1_M3KTI_PMON_REG supported on:                               */
/*      SPRA0 (0x21e290e4)                                                      */
/*      SPRB0 (0x21e290e4)                                                      */
/*      SPRHBM (0x21e290e4)                                                     */
/*      SPRC0 (0x21e290e4)                                                      */
/*      SPRMCC (0x21e290e4)                                                     */
/*      SPRUCC (0x21e4b0e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRCFG0_1
*/


#define PMONCNTRCFG0_1_N1_M3KTI_PMON_REG 0x0A1200E4

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

} PMONCNTRCFG0_1_N1_M3KTI_PMON_STRUCT;

/* PMONCNTRCFG0_2_N0_M3KTI_PMON_REG supported on:                               */
/*      SPRA0 (0x21e290e8)                                                      */
/*      SPRB0 (0x21e290e8)                                                      */
/*      SPRHBM (0x21e290e8)                                                     */
/*      SPRC0 (0x21e290e8)                                                      */
/*      SPRMCC (0x21e290e8)                                                     */
/*      SPRUCC (0x21e4b0e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRCFG0_2
*/


#define PMONCNTRCFG0_2_N0_M3KTI_PMON_REG 0x0A1200E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 eventselect : 8;

                            /* Bits[7:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               This field is used to decode the PerfMon event
                               which is selected.
                            */
    UINT32 unitmask : 8;

                            /* Bits[15:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This mask selects the sub-events to be selected
                               for creation of the event (for applicable
                               events). See Pmon events for details.
                            */
    UINT32 rsvd_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 counterreset : 1;

                            /* Bits[17:17], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When this bit is set, the corresponding counter
                               will be reset to 0. This allows for a quick
                               reset of the counter when changing event
                               encodings.
                            */
    UINT32 edgedetect : 1;

                            /* Bits[18:18], Access Type=RW/V, default=0x00000000*/

                            /*
                               Edge Detect allows counting either 0 to 1 (if
                               invert=0) or 1 to 0 (if invert=1) transitions of
                               a given event. For example, there is an event
                               that counts the number of cycles in L1 power
                               state in Intel UPI. By using edge detect, one
                               can count the number of times that we entered L1
                               state (by detecting the rising edge by setting
                               invert=0). Edge detect only works in conjunction
                               with threshholding. This is true even for events
                               that can only increment by 1 in a given cycle
                               (like the L1 example above). In this case, one
                               should set a threshold of 1 (with invert=0). One
                               can also use Edge Detect with queue occupancy
                               events. For example, if one wanted to count the
                               number of times when the TXQ occupancy was
                               greater than or equal to 5, one would select the
                               TXQ occupancy event with a threshold of 5 and
                               set the Edge Detect bit (and invert=0).
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 frz_on_ov : 1;

                            /* Bits[20:20], Access Type=RW/V, default=0x00000000*/

                            /*
                               This bit will be sent in the GPSB message on a
                               pmon overflow to indicate whether or not to
                               freeze when an overflow occurs.
                            */
    UINT32 rsvd : 1;

                            /* Bits[21:21], Access Type=RW/V, default=0x00000000*/

                            /*
                               Bit was defined as Internal control bit, but
                               this function is dropped in SKX, and bit is now
                               Reserved.
                            */
    UINT32 rsvd_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 invert : 1;

                            /* Bits[23:23], Access Type=RW/V, default=0x00000000*/

                            /*
                               This bit indicates how the threshold field will
                               be compared to the incoming event. When 0, the
                               comparison that will be done is threshold
                               greater than or equal to event. When set to 1,
                               the comparison that will be done is inverted
                               from the case where this bit is set to 0, i.e.,
                               threshold less than event. The invert bit only
                               works when Threshold != 0. So, if one would like
                               to invert a non-occupancy event, one needs to
                               set the threshold to 1.
                            */
    UINT32 thresh : 8;

                            /* Bits[31:24], Access Type=RW/V, default=0x00000000*/

                            /*
                               This field is compared directly against an
                               incoming event value for events that can
                               increment by 1 or more in a given cycle. The
                               result of the comparison is effectively a 1 bit
                               wide event, i.e., the counter will be
                               incremented by 1 when the comparison is true
                               (the type of comparison depends on the setting
                               of the invert bit - see bit 23) no matter how
                               wide the original event was. When this field is
                               zero, threshold comparison is disabled and the
                               event is passed without modification.
                            */

  } Bits;
  UINT32 Data;

} PMONCNTRCFG0_2_N0_M3KTI_PMON_STRUCT;

/* PMONCNTRCFG0_2_N1_M3KTI_PMON_REG supported on:                               */
/*      SPRA0 (0x21e290ec)                                                      */
/*      SPRB0 (0x21e290ec)                                                      */
/*      SPRHBM (0x21e290ec)                                                     */
/*      SPRC0 (0x21e290ec)                                                      */
/*      SPRMCC (0x21e290ec)                                                     */
/*      SPRUCC (0x21e4b0ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRCFG0_2
*/


#define PMONCNTRCFG0_2_N1_M3KTI_PMON_REG 0x0A1200EC

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

} PMONCNTRCFG0_2_N1_M3KTI_PMON_STRUCT;

/* PMONCNTRCFG0_3_N0_M3KTI_PMON_REG supported on:                               */
/*      SPRA0 (0x21e290f0)                                                      */
/*      SPRB0 (0x21e290f0)                                                      */
/*      SPRHBM (0x21e290f0)                                                     */
/*      SPRC0 (0x21e290f0)                                                      */
/*      SPRMCC (0x21e290f0)                                                     */
/*      SPRUCC (0x21e4b0f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRCFG0_3
*/


#define PMONCNTRCFG0_3_N0_M3KTI_PMON_REG 0x0A1200F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 eventselect : 8;

                            /* Bits[7:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               This field is used to decode the PerfMon event
                               which is selected.
                            */
    UINT32 unitmask : 8;

                            /* Bits[15:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This mask selects the sub-events to be selected
                               for creation of the event (for applicable
                               events). See Pmon events for details.
                            */
    UINT32 rsvd_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 counterreset : 1;

                            /* Bits[17:17], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When this bit is set, the corresponding counter
                               will be reset to 0. This allows for a quick
                               reset of the counter when changing event
                               encodings.
                            */
    UINT32 edgedetect : 1;

                            /* Bits[18:18], Access Type=RW/V, default=0x00000000*/

                            /*
                               Edge Detect allows counting either 0 to 1 (if
                               invert=0) or 1 to 0 (if invert=1) transitions of
                               a given event. For example, there is an event
                               that counts the number of cycles in L1 power
                               state in Intel UPI. By using edge detect, one
                               can count the number of times that we entered L1
                               state (by detecting the rising edge by setting
                               invert=0). Edge detect only works in conjunction
                               with threshholding. This is true even for events
                               that can only increment by 1 in a given cycle
                               (like the L1 example above). In this case, one
                               should set a threshold of 1 (with invert=0). One
                               can also use Edge Detect with queue occupancy
                               events. For example, if one wanted to count the
                               number of times when the TXQ occupancy was
                               greater than or equal to 5, one would select the
                               TXQ occupancy event with a threshold of 5 and
                               set the Edge Detect bit (and invert=0).
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 frz_on_ov : 1;

                            /* Bits[20:20], Access Type=RW/V, default=0x00000000*/

                            /*
                               This bit will be sent in the GPSB message on a
                               pmon overflow to indicate whether or not to
                               freeze when an overflow occurs.
                            */
    UINT32 rsvd : 1;

                            /* Bits[21:21], Access Type=RW/V, default=0x00000000*/

                            /*
                               Bit was defined as Internal control bit, but
                               this function is dropped in SKX, and bit is now
                               Reserved.
                            */
    UINT32 rsvd_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 invert : 1;

                            /* Bits[23:23], Access Type=RW/V, default=0x00000000*/

                            /*
                               This bit indicates how the threshold field will
                               be compared to the incoming event. When 0, the
                               comparison that will be done is threshold
                               greater than or equal to event. When set to 1,
                               the comparison that will be done is inverted
                               from the case where this bit is set to 0, i.e.,
                               threshold less than event. The invert bit only
                               works when Threshold != 0. So, if one would like
                               to invert a non-occupancy event, one needs to
                               set the threshold to 1.
                            */
    UINT32 thresh : 8;

                            /* Bits[31:24], Access Type=RW/V, default=0x00000000*/

                            /*
                               This field is compared directly against an
                               incoming event value for events that can
                               increment by 1 or more in a given cycle. The
                               result of the comparison is effectively a 1 bit
                               wide event, i.e., the counter will be
                               incremented by 1 when the comparison is true
                               (the type of comparison depends on the setting
                               of the invert bit - see bit 23) no matter how
                               wide the original event was. When this field is
                               zero, threshold comparison is disabled and the
                               event is passed without modification.
                            */

  } Bits;
  UINT32 Data;

} PMONCNTRCFG0_3_N0_M3KTI_PMON_STRUCT;

/* PMONCNTRCFG0_3_N1_M3KTI_PMON_REG supported on:                               */
/*      SPRA0 (0x21e290f4)                                                      */
/*      SPRB0 (0x21e290f4)                                                      */
/*      SPRHBM (0x21e290f4)                                                     */
/*      SPRC0 (0x21e290f4)                                                      */
/*      SPRMCC (0x21e290f4)                                                     */
/*      SPRUCC (0x21e4b0f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCNTRCFG0_3
*/


#define PMONCNTRCFG0_3_N1_M3KTI_PMON_REG 0x0A1200F4

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

} PMONCNTRCFG0_3_N1_M3KTI_PMON_STRUCT;

/* PMONCTRSTATUS0_M3KTI_PMON_REG supported on:                                  */
/*      SPRA0 (0x21e290f8)                                                      */
/*      SPRB0 (0x21e290f8)                                                      */
/*      SPRHBM (0x21e290f8)                                                     */
/*      SPRC0 (0x21e290f8)                                                      */
/*      SPRMCC (0x21e290f8)                                                     */
/*      SPRUCC (0x21e4b0f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMONCTRSTATUS0
*/


#define PMONCTRSTATUS0_M3KTI_PMON_REG 0x0A1200F8

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

} PMONCTRSTATUS0_M3KTI_PMON_STRUCT;

/* PXPENHCAP_1_M3KTI_PMON_REG supported on:                                     */
/*      SPRA0 (0x21e29100)                                                      */
/*      SPRB0 (0x21e29100)                                                      */
/*      SPRHBM (0x21e29100)                                                     */
/*      SPRC0 (0x21e29100)                                                      */
/*      SPRMCC (0x21e29100)                                                     */
/*      SPRUCC (0x21e4b100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F1      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F1           */
/* Struct generated from SPRA0 BDF: 30_5_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Enhanced Capability
*/


#define PXPENHCAP_1_M3KTI_PMON_REG 0x0A120100

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

} PXPENHCAP_1_M3KTI_PMON_STRUCT;
#endif /* _M3KTI_PMON_h */
