
/** @file
  IIO_HQM_PF.h

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


#ifndef _IIO_HQM_PF_h
#define _IIO_HQM_PF_h
#include <Base.h>

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* VENDOR_ID_IIO_HQM_PF_REG supported on:                                       */
/*      SPRA0 (0x10e00000)                                                      */
/*      SPRB0 (0x10e00000)                                                      */
/*      SPRC0 (0x10e00000)                                                      */
/*      SPRMCC (0x10e00000)                                                     */
/*      SPRUCC (0x10e00000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Vendor Identificaion Register
*/


#define VENDOR_ID_IIO_HQM_PF_REG 0x21410000

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

} VENDOR_ID_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* DEVICE_ID_IIO_HQM_PF_REG supported on:                                       */
/*      SPRA0 (0x10e00002)                                                      */
/*      SPRB0 (0x10e00002)                                                      */
/*      SPRC0 (0x10e00002)                                                      */
/*      SPRMCC (0x10e00002)                                                     */
/*      SPRUCC (0x10e00002)                                                     */
/* Register default value on SPRA0: 0x00002710                                  */
/* Register default value on SPRB0: 0x00002710                                  */
/* Register default value on SPRC0: 0x00002710                                  */
/* Register default value on SPRMCC: 0x00002710                                 */
/* Register default value on SPRUCC: 0x00002710                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Device Identification Register
*/


#define DEVICE_ID_IIO_HQM_PF_REG 0x21410002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 did : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00002710*/

                            /* Device Identification Number. */

  } Bits;
  UINT16 Data;

} DEVICE_ID_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* DEVICE_COMMAND_IIO_HQM_PF_REG supported on:                                  */
/*      SPRA0 (0x10e00004)                                                      */
/*      SPRB0 (0x10e00004)                                                      */
/*      SPRC0 (0x10e00004)                                                      */
/*      SPRMCC (0x10e00004)                                                     */
/*      SPRUCC (0x10e00004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Device Command Register
*/


#define DEVICE_COMMAND_IIO_HQM_PF_REG 0x21410004

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
                               Bus Master Enable: Controls the ability of EP to
                               issue Memory Read/Write Requests.Clearing (0)
                               this bit prevents EP from issuing any Memory
                               Requests. Note that as MSIs are in-band memory
                               writes, disabling the bus master enable bit
                               disables MSI as well. PCIe messages are not
                               affected by this bit. Please refer to for more
                               details on how the EP handles CPP commands when
                               this bit is clear.
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
                               conditions occurs: If the EP receives a poisoned
                               Completion from the RC If the EP poisons a write
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
                               Fast Back-to-Back Enable: EP does not implement
                               this functionality and it is not applicable to
                               PCIe devices. The bit is hardwired to 0.
                            */
    UINT16 intd : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Interrupt Disable: Setting this bit disables
                               generation of INTX messages by the EP.. Default
                               value is 0 which enables the INTX message
                               generation.
                            */
    UINT16 reserved : 5;

                            /* Bits[15:11], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} DEVICE_COMMAND_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* DEVICE_STATUS_IIO_HQM_PF_REG supported on:                                   */
/*      SPRA0 (0x10e00006)                                                      */
/*      SPRB0 (0x10e00006)                                                      */
/*      SPRC0 (0x10e00006)                                                      */
/*      SPRMCC (0x10e00006)                                                     */
/*      SPRUCC (0x10e00006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* device status register
*/


#define DEVICE_STATUS_IIO_HQM_PF_REG 0x21410006

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 reserved2 : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000000*/

                            /* Reserved2 */
    UINT16 intsts : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               Interrupt Status: Indicates that the EP has
                               transmitted a INTX message and is awaiting
                               servicing. This bit does not includes MSIs
                               generated by the EP.
                            */
    UINT16 cl : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               Capabilities List: This bit is hardwired to 1 to
                               indicate that EP has a capabilities list.
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
                               set by EP, as a Requestor if the Parity Error
                               Enable bit in the Command register is 1b and
                               either of the following two conditions occurs:If
                               the Parity Error Enable bit is 0b, this bit is
                               never set.Default value of this field is 0.
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
                               when EP completes a Request using Completer
                               Abort Completion Status.Default value of this
                               field is 0.
                            */
    UINT16 rta : 1;

                            /* Bits[12:12], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Received Target Abort Status: This bit is set
                               when EP, as a Requestor receives a Completion
                               with Completer Abort Completion Status.Default
                               value of this field is 0.
                            */
    UINT16 rma : 1;

                            /* Bits[13:13], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Received Master Abort Status: This bit is set
                               when EP, as a Requestor receives a Completion
                               with Unsupported Request Completion
                               Status.Default value of this field is 0.
                            */
    UINT16 sse : 1;

                            /* Bits[14:14], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Signaled System Error: This bit is set by the EP
                               when it sends a ERR_FATAL or ERR_NONFATAL
                               message and the SERR bit in the Device Command
                               register bit is set.Default value of this field
                               is 0.
                            */
    UINT16 dpe : 1;

                            /* Bits[15:15], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Detected Parity Error: This bit is set by EP
                               whenever it receives a Poisoned TLP, regardless
                               of the state the Parity Error Enable bit in the
                               Command register.Default value of this field is
                               0.
                            */

  } Bits;
  UINT16 Data;

} DEVICE_STATUS_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* REVISION_ID_CLASS_CODE_IIO_HQM_PF_REG supported on:                          */
/*      SPRA0 (0x20e00008)                                                      */
/*      SPRB0 (0x20e00008)                                                      */
/*      SPRC0 (0x20e00008)                                                      */
/*      SPRMCC (0x20e00008)                                                     */
/*      SPRUCC (0x20e00008)                                                     */
/* Register default value on SPRA0: 0x0B400000                                  */
/* Register default value on SPRB0: 0x0B400000                                  */
/* Register default value on SPRC0: 0x0B400000                                  */
/* Register default value on SPRMCC: 0x0B400000                                 */
/* Register default value on SPRUCC: 0x0B400000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Revision ID register and class code Register
*/


#define REVISION_ID_CLASS_CODE_IIO_HQM_PF_REG 0x21420008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ridl : 4;

                            /* Bits[3:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Minor Revision:Incremented for each stepping
                               which does not modify all masks. Reset for
                               eachmajor revision.00b: x0 stepping01b: x1
                               stepping10b: x2 stepping11b: x3 stepping
                            */
    UINT32 ridu : 4;

                            /* Bits[7:4], Access Type=RO/V, default=0x00000000*/

                            /*
                               Major Revision:Steppings which require all masks
                               to be regenerated.00b: A stepping (Current
                               Stepping)01b: B stepping10b: C stepping11b: D
                               stepping
                            */
    UINT32 cc : 24;

                            /* Bits[31:8], Access Type=RO, default=0x000B4000*/

                            /*
                               Class Code: This value indicates the base class,
                               subclass, and interface.0B4000h = Base class:
                               Processor, Sub-class Co-processor, no specific
                               register level programming interfaces are
                               defined.
                            */

  } Bits;
  UINT32 Data;

} REVISION_ID_CLASS_CODE_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* CACHE_LINE_SIZE_IIO_HQM_PF_REG supported on:                                 */
/*      SPRA0 (0xe0000c)                                                        */
/*      SPRB0 (0xe0000c)                                                        */
/*      SPRC0 (0xe0000c)                                                        */
/*      SPRMCC (0xe0000c)                                                       */
/*      SPRUCC (0xe0000c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Cache Line size Register
*/


#define CACHE_LINE_SIZE_IIO_HQM_PF_REG 0x2140000C

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

} CACHE_LINE_SIZE_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HEADER_TYPE_IIO_HQM_PF_REG supported on:                                     */
/*      SPRA0 (0xe0000e)                                                        */
/*      SPRB0 (0xe0000e)                                                        */
/*      SPRC0 (0xe0000e)                                                        */
/*      SPRMCC (0xe0000e)                                                       */
/*      SPRUCC (0xe0000e)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header type Register
*/


#define HEADER_TYPE_IIO_HQM_PF_REG 0x2140000E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 hdr : 7;

                            /* Bits[6:0], Access Type=RO, default=0x00000000*/

                            /*
                               PCI Header Type: The header type of the EP
                               device. This is a type 0 configuration space
                               header.
                            */
    UINT8 mfd : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               Multi-function device indicator: A value of 1
                               indicates a multi-function device.
                            */

  } Bits;
  UINT8 Data;

} HEADER_TYPE_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* FUNC_BAR_L_IIO_HQM_PF_REG supported on:                                      */
/*      SPRA0 (0x20e00010)                                                      */
/*      SPRB0 (0x20e00010)                                                      */
/*      SPRC0 (0x20e00010)                                                      */
/*      SPRMCC (0x20e00010)                                                     */
/*      SPRUCC (0x20e00010)                                                     */
/* Register default value on SPRA0: 0x0000000C                                  */
/* Register default value on SPRB0: 0x0000000C                                  */
/* Register default value on SPRC0: 0x0000000C                                  */
/* Register default value on SPRMCC: 0x0000000C                                 */
/* Register default value on SPRUCC: 0x0000000C                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF Function Memory Lower Base Address Register
*/


#define FUNC_BAR_L_IIO_HQM_PF_REG 0x21420010

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

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Prefetchable: Hardwired to 1 to indicate that
                               the region is prefetchable.0: non-prefetchable
                               1: prefetchable
                            */
    UINT32 zero : 22;

                            /* Bits[25:4], Access Type=RO, default=0x00000000*/

                            /* Lower Bits: Hardwired to 0 (64MB region) */
    UINT32 addr_l : 6;

                            /* Bits[31:26], Access Type=RW, default=0x00000000*/

                            /* Lower Programmable Base Address bits */

  } Bits;
  UINT32 Data;

} FUNC_BAR_L_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* FUNC_BAR_U_IIO_HQM_PF_REG supported on:                                      */
/*      SPRA0 (0x20e00014)                                                      */
/*      SPRB0 (0x20e00014)                                                      */
/*      SPRC0 (0x20e00014)                                                      */
/*      SPRMCC (0x20e00014)                                                     */
/*      SPRUCC (0x20e00014)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF Function Memory Upper Base Address Register
*/


#define FUNC_BAR_U_IIO_HQM_PF_REG 0x21420014

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

} FUNC_BAR_U_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* CSR_BAR_L_IIO_HQM_PF_REG supported on:                                       */
/*      SPRA0 (0x20e00018)                                                      */
/*      SPRB0 (0x20e00018)                                                      */
/*      SPRC0 (0x20e00018)                                                      */
/*      SPRMCC (0x20e00018)                                                     */
/*      SPRUCC (0x20e00018)                                                     */
/* Register default value on SPRA0: 0x0000000C                                  */
/* Register default value on SPRB0: 0x0000000C                                  */
/* Register default value on SPRC0: 0x0000000C                                  */
/* Register default value on SPRMCC: 0x0000000C                                 */
/* Register default value on SPRUCC: 0x0000000C                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF CSR Memory Lower Base Address Register
*/


#define CSR_BAR_L_IIO_HQM_PF_REG 0x21420018

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

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Prefetchable: Hardwired to 1 to indicate that
                               the region is prefetchable.0: non-prefetchable
                               1: prefetchable
                            */
    UINT32 zero : 28;

                            /* Bits[31:4], Access Type=RO, default=0x00000000*/

                            /* Lower Bits: Hardwired to 0 (4GB region). */

  } Bits;
  UINT32 Data;

} CSR_BAR_L_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* CSR_BAR_U_IIO_HQM_PF_REG supported on:                                       */
/*      SPRA0 (0x20e0001c)                                                      */
/*      SPRB0 (0x20e0001c)                                                      */
/*      SPRC0 (0x20e0001c)                                                      */
/*      SPRMCC (0x20e0001c)                                                     */
/*      SPRUCC (0x20e0001c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF CSR Memory Upper Base Address Register
*/


#define CSR_BAR_U_IIO_HQM_PF_REG 0x2142001C

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

} CSR_BAR_U_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* SUBSYSTEM_VENDOR_ID_IIO_HQM_PF_REG supported on:                             */
/*      SPRA0 (0x10e0002c)                                                      */
/*      SPRB0 (0x10e0002c)                                                      */
/*      SPRC0 (0x10e0002c)                                                      */
/*      SPRMCC (0x10e0002c)                                                     */
/*      SPRUCC (0x10e0002c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF Subsystem Vendor ID Register
*/


#define SUBSYSTEM_VENDOR_ID_IIO_HQM_PF_REG 0x2141002C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 svid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00008086*/

                            /*
                               Subsystem Vendor ID: This field is hardwired to
                               ID assigned to Intel.
                            */

  } Bits;
  UINT16 Data;

} SUBSYSTEM_VENDOR_ID_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* SUBSYSTEM_ID_IIO_HQM_PF_REG supported on:                                    */
/*      SPRA0 (0x10e0002e)                                                      */
/*      SPRB0 (0x10e0002e)                                                      */
/*      SPRC0 (0x10e0002e)                                                      */
/*      SPRMCC (0x10e0002e)                                                     */
/*      SPRUCC (0x10e0002e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF Subsystem ID Register
*/


#define SUBSYSTEM_ID_IIO_HQM_PF_REG 0x2141002E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 sid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /*
                               Subsystem ID: Vendor supplied device ID. Default
                               is 0h.
                            */

  } Bits;
  UINT16 Data;

} SUBSYSTEM_ID_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* CAP_PTR_IIO_HQM_PF_REG supported on:                                         */
/*      SPRA0 (0xe00034)                                                        */
/*      SPRB0 (0xe00034)                                                        */
/*      SPRC0 (0xe00034)                                                        */
/*      SPRMCC (0xe00034)                                                       */
/*      SPRUCC (0xe00034)                                                       */
/* Register default value on SPRA0: 0x00000060                                  */
/* Register default value on SPRB0: 0x00000060                                  */
/* Register default value on SPRC0: 0x00000060                                  */
/* Register default value on SPRMCC: 0x00000060                                 */
/* Register default value on SPRUCC: 0x00000060                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF Capabilities Pointer Register
*/


#define CAP_PTR_IIO_HQM_PF_REG 0x21400034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 cp : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000060*/

                            /* Capability Pointer (MSI-X Capabilities) */

  } Bits;
  UINT8 Data;

} CAP_PTR_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* INT_LINE_IIO_HQM_PF_REG supported on:                                        */
/*      SPRA0 (0xe0003c)                                                        */
/*      SPRB0 (0xe0003c)                                                        */
/*      SPRC0 (0xe0003c)                                                        */
/*      SPRMCC (0xe0003c)                                                       */
/*      SPRUCC (0xe0003c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF Interrupt Line Register
*/


#define INT_LINE_IIO_HQM_PF_REG 0x2140003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 irql : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               Interrupt Line: BIOS writes the interrupt
                               routing information to this register to indicate
                               which input of the interrupt controller this
                               device is connected to. The device itself does
                               not use this information.
                            */

  } Bits;
  UINT8 Data;

} INT_LINE_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* INT_PIN_IIO_HQM_PF_REG supported on:                                         */
/*      SPRA0 (0xe0003d)                                                        */
/*      SPRB0 (0xe0003d)                                                        */
/*      SPRC0 (0xe0003d)                                                        */
/*      SPRMCC (0xe0003d)                                                       */
/*      SPRUCC (0xe0003d)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF Interrupt Pin Register
*/


#define INT_PIN_IIO_HQM_PF_REG 0x2140003D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 irqp : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Interrupt Pin: Set to 00h to indicate that EP
                               does not support legacy interrupts.
                            */

  } Bits;
  UINT8 Data;

} INT_PIN_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_MSIX_CAP_MSIX_CAP_ID_IIO_HQM_PF_REG supported on:                   */
/*      SPRA0 (0xe00060)                                                        */
/*      SPRB0 (0xe00060)                                                        */
/*      SPRC0 (0xe00060)                                                        */
/*      SPRMCC (0xe00060)                                                       */
/*      SPRUCC (0xe00060)                                                       */
/* Register default value on SPRA0: 0x00000011                                  */
/* Register default value on SPRB0: 0x00000011                                  */
/* Register default value on SPRC0: 0x00000011                                  */
/* Register default value on SPRMCC: 0x00000011                                 */
/* Register default value on SPRUCC: 0x00000011                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF Message Signalled Interrupt X Capability ID Register
*/


#define HQM_PCIE_MSIX_CAP_MSIX_CAP_ID_IIO_HQM_PF_REG 0x21400060

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
                               identifies the EP as a device that is MSI-X
                               capable.
                            */

  } Bits;
  UINT8 Data;

} HQM_PCIE_MSIX_CAP_MSIX_CAP_ID_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_MSIX_CAP_MSIX_CAP_NEXT_CAP_PTR_IIO_HQM_PF_REG supported on:         */
/*      SPRA0 (0xe00061)                                                        */
/*      SPRB0 (0xe00061)                                                        */
/*      SPRC0 (0xe00061)                                                        */
/*      SPRMCC (0xe00061)                                                       */
/*      SPRUCC (0xe00061)                                                       */
/* Register default value on SPRA0: 0x0000006C                                  */
/* Register default value on SPRB0: 0x0000006C                                  */
/* Register default value on SPRC0: 0x0000006C                                  */
/* Register default value on SPRMCC: 0x0000006C                                 */
/* Register default value on SPRUCC: 0x0000006C                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* MSIX Next Capability Pointer Register
*/


#define HQM_PCIE_MSIX_CAP_MSIX_CAP_NEXT_CAP_PTR_IIO_HQM_PF_REG 0x21400061

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 cap_ptr : 8;

                            /* Bits[7:0], Access Type=RO, default=0x0000006C*/

                            /* Next Capability Pointer (PCIe Capabilities) */

  } Bits;
  UINT8 Data;

} HQM_PCIE_MSIX_CAP_MSIX_CAP_NEXT_CAP_PTR_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_MSIX_CAP_MSIX_CAP_CONTROL_IIO_HQM_PF_REG supported on:              */
/*      SPRA0 (0x10e00062)                                                      */
/*      SPRB0 (0x10e00062)                                                      */
/*      SPRC0 (0x10e00062)                                                      */
/*      SPRMCC (0x10e00062)                                                     */
/*      SPRUCC (0x10e00062)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF Message Signalled Interrupt X Control Register
*/


#define HQM_PCIE_MSIX_CAP_MSIX_CAP_CONTROL_IIO_HQM_PF_REG 0x21410062

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ts : 11;

                            /* Bits[10:0], Access Type=RO, default=0x00000040*/

                            /*
                               MSI-X Table Size: Number of vectors (encoded as
                               N-1) supported by the EP. The EP supports 65
                               vectors.
                            */
    UINT16 reserved : 3;

                            /* Bits[13:11], Access Type=RO, default=0x00000000*/

                            /* Always returns 0s when read. */
    UINT16 fm : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Function Mask: This bit controls the masking of
                               all vectors implemented in the EP.When this bit
                               is 0, each vectors mask bit determines whether
                               the vector is masked or not.Whent this bit is 1,
                               all vecotrs are masked regardless of the per
                               vector masking bit.
                            */
    UINT16 msixen : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               MSI-X Enable: This bit enables the EP to
                               generates interrupts using the MSI-X tables
                               instead of the legacy INTx messages.When this
                               bit is set to 1, the EP will not generate INTx
                               messages and must use the MSI-X to signal
                               interrupts.The device driver should not clear
                               this bit to mask interrupts. This bit will be
                               set by the system PCI device manager.
                            */

  } Bits;
  UINT16 Data;

} HQM_PCIE_MSIX_CAP_MSIX_CAP_CONTROL_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_MSIX_CAP_MSIX_CAP_TABLE_OFFSET_BIR_IIO_HQM_PF_REG supported on:     */
/*      SPRA0 (0x20e00064)                                                      */
/*      SPRB0 (0x20e00064)                                                      */
/*      SPRC0 (0x20e00064)                                                      */
/*      SPRMCC (0x20e00064)                                                     */
/*      SPRUCC (0x20e00064)                                                     */
/* Register default value on SPRA0: 0x01000000                                  */
/* Register default value on SPRB0: 0x01000000                                  */
/* Register default value on SPRC0: 0x01000000                                  */
/* Register default value on SPRMCC: 0x01000000                                 */
/* Register default value on SPRUCC: 0x01000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF MSI-X Table Offset & Table BIR Register
*/


#define HQM_PCIE_MSIX_CAP_MSIX_CAP_TABLE_OFFSET_BIR_IIO_HQM_PF_REG 0x21420064

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tbir : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000000*/

                            /*
                               Table BAR Indicator Register: The BIR points to
                               the FUNC_PF register (10h)
                            */
    UINT32 to : 29;

                            /* Bits[31:3], Access Type=RO, default=0x00200000*/

                            /*
                               Table Offset: Offset to a location in one of the
                               EPs BARs (indicated by TBIR) that points to the
                               location of the base of the MSI-X Table.The EP
                               MSI-X Table maps to an offset of 1024KB in
                               PFUNCBAR.
                            */

  } Bits;
  UINT32 Data;

} HQM_PCIE_MSIX_CAP_MSIX_CAP_TABLE_OFFSET_BIR_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_MSIX_CAP_MSIX_CAP_PBA_OFFSET_BIR_IIO_HQM_PF_REG supported on:       */
/*      SPRA0 (0x20e00068)                                                      */
/*      SPRB0 (0x20e00068)                                                      */
/*      SPRC0 (0x20e00068)                                                      */
/*      SPRMCC (0x20e00068)                                                     */
/*      SPRUCC (0x20e00068)                                                     */
/* Register default value on SPRA0: 0x01001000                                  */
/* Register default value on SPRB0: 0x01001000                                  */
/* Register default value on SPRC0: 0x01001000                                  */
/* Register default value on SPRMCC: 0x01001000                                 */
/* Register default value on SPRUCC: 0x01001000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF MSI-X Pending Bit Array & BIR Offset Register
*/


#define HQM_PCIE_MSIX_CAP_MSIX_CAP_PBA_OFFSET_BIR_IIO_HQM_PF_REG 0x21420068

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pbabir : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000000*/

                            /*
                               Pending Bit Array BAR Indicator Register: The
                               PBABIR points to the FUNC_BAR register (10h)
                            */
    UINT32 pbao : 29;

                            /* Bits[31:3], Access Type=RO, default=0x00200200*/

                            /*
                               Pending Bit Array Offset: Offset to a location
                               in the FUNC_PF BAR (indicated by PBABIR) that
                               points to the location of the base of the MSI-X
                               PBA.
                            */

  } Bits;
  UINT32 Data;

} HQM_PCIE_MSIX_CAP_MSIX_CAP_PBA_OFFSET_BIR_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_CAP_PCIE_CAP_ID_IIO_HQM_PF_REG supported on:                        */
/*      SPRA0 (0xe0006c)                                                        */
/*      SPRB0 (0xe0006c)                                                        */
/*      SPRC0 (0xe0006c)                                                        */
/*      SPRMCC (0xe0006c)                                                       */
/*      SPRUCC (0xe0006c)                                                       */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF PCI Express Capability Register
*/


#define HQM_PCIE_CAP_PCIE_CAP_ID_IIO_HQM_PF_REG 0x2140006C

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

} HQM_PCIE_CAP_PCIE_CAP_ID_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_CAP_PCIE_CAP_NEXT_CAP_PTR_IIO_HQM_PF_REG supported on:              */
/*      SPRA0 (0xe0006d)                                                        */
/*      SPRB0 (0xe0006d)                                                        */
/*      SPRC0 (0xe0006d)                                                        */
/*      SPRMCC (0xe0006d)                                                       */
/*      SPRUCC (0xe0006d)                                                       */
/* Register default value on SPRA0: 0x000000B0                                  */
/* Register default value on SPRB0: 0x000000B0                                  */
/* Register default value on SPRC0: 0x000000B0                                  */
/* Register default value on SPRMCC: 0x000000B0                                 */
/* Register default value on SPRUCC: 0x000000B0                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF PCI Express Next Capability Pointer Register
*/


#define HQM_PCIE_CAP_PCIE_CAP_NEXT_CAP_PTR_IIO_HQM_PF_REG 0x2140006D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 cap_ptr : 8;

                            /* Bits[7:0], Access Type=RO, default=0x000000B0*/

                            /*
                               Next Capability Pointer (Power Management
                               Capabilities)
                            */

  } Bits;
  UINT8 Data;

} HQM_PCIE_CAP_PCIE_CAP_NEXT_CAP_PTR_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_CAP_PCIE_CAP_IIO_HQM_PF_REG supported on:                           */
/*      SPRA0 (0x10e0006e)                                                      */
/*      SPRB0 (0x10e0006e)                                                      */
/*      SPRC0 (0x10e0006e)                                                      */
/*      SPRMCC (0x10e0006e)                                                     */
/*      SPRUCC (0x10e0006e)                                                     */
/* Register default value on SPRA0: 0x00000092                                  */
/* Register default value on SPRB0: 0x00000092                                  */
/* Register default value on SPRC0: 0x00000092                                  */
/* Register default value on SPRMCC: 0x00000092                                 */
/* Register default value on SPRUCC: 0x00000092                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF PCI Express Capabilities Register
*/


#define HQM_PCIE_CAP_PCIE_CAP_IIO_HQM_PF_REG 0x2141006E

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
                               number.EP is PCIe 4.0 Specification Compliant.
                            */
    UINT16 dpt : 4;

                            /* Bits[7:4], Access Type=RO/V, default=0x00000009*/

                            /*
                               Device/Port Type: Indicates the type of PCI
                               Express logical device. Hardwired to 1001b (Root
                               Complex Integrated Endpoint)
                            */
    UINT16 si : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Slot Implemented: This bit when set indicates
                               that the PCI Express Link associated with this
                               port is connected to a slot. Hardwired to 0 for
                               EP.
                            */
    UINT16 imn : 5;

                            /* Bits[13:9], Access Type=RO, default=0x00000000*/

                            /*
                               Interrupt Message Number: This field indicates
                               which MSI vector is used for the interrupt
                               message generated in association with the status
                               bits in either the Slot Status field of this
                               capability structure (applies to only RC or
                               Switch)Not applicable to EP.
                            */
    UINT16 reserved : 2;

                            /* Bits[15:14], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} HQM_PCIE_CAP_PCIE_CAP_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_CAP_PCIE_CAP_DEVICE_CAP_IIO_HQM_PF_REG supported on:                */
/*      SPRA0 (0x20e00070)                                                      */
/*      SPRB0 (0x20e00070)                                                      */
/*      SPRC0 (0x20e00070)                                                      */
/*      SPRMCC (0x20e00070)                                                     */
/*      SPRUCC (0x20e00070)                                                     */
/* Register default value on SPRA0: 0x10008062                                  */
/* Register default value on SPRB0: 0x10008062                                  */
/* Register default value on SPRC0: 0x10008062                                  */
/* Register default value on SPRMCC: 0x10008062                                 */
/* Register default value on SPRUCC: 0x10008062                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF PCI Express Device Capabilities Register
*/


#define HQM_PCIE_CAP_PCIE_CAP_DEVICE_CAP_IIO_HQM_PF_REG 0x21420070

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mps : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000002*/

                            /*
                               Max_Payload_Size Supported: This field indicates
                               the maximum payload size that EP can support for
                               TLPs. This value is set to indicate 512B. The
                               defined encodings are: 000b = 128B max payload
                               size 001b = 256B max payload size(Max supported)
                               010b = 512 bytes max payload size 011b = 1KB max
                               payload size 100b = 2KB max payload size 101b =
                               4KB max payload size
                            */
    UINT32 pfs : 2;

                            /* Bits[4:3], Access Type=RO, default=0x00000000*/

                            /*
                               Phantom Functions Supported: This field
                               indicates the support for use of unclaimed
                               function numbers to extend the number of
                               outstanding transactions allowed. EP does not
                               use this capability.
                            */
    UINT32 etfs : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000001*/

                            /*
                               Extended Tag Field Supported: This field
                               indicates the maximum supported size of the Tag
                               field as a Requester. When Clear indicates 5-bit
                               Tag field is supported. When set 8-bit Tag field
                               is supported. Supports 256 outstanding read
                               requests.
                            */
    UINT32 el0l : 3;

                            /* Bits[8:6], Access Type=RO, default=0x00000001*/

                            /*
                               Endpoint L0s Acceptable Latency: This field
                               indicates the acceptable latency that an
                               Endpoint can withstand due to the transition
                               from L1 state to the L0 state. This value is
                               hardcoded to the latency of 64-128ns.Since HQM
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
                               lowest value of 1us.Since HQM does not have a
                               physical layer, this functionality is not
                               implemented.
                            */
    UINT32 attn : 3;

                            /* Bits[14:12], Access Type=RO, default=0x00000000*/

                            /*
                               Attention Button/Indicator Present and Power
                               Indicator Present. None of these are implemented
                               in the EP.
                            */
    UINT32 rbep : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000001*/

                            /*
                               Role-Based Error Reporting: Indicates that EP
                               conforms to Role based error reporting ECN for
                               PCIe 1.0a and which was subsequently rolled in
                               PCIe 1.1 and future revisions.
                            */
    UINT32 reserved1 : 2;

                            /* Bits[17:16], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */
    UINT32 cspv : 8;

                            /* Bits[25:18], Access Type=RO, default=0x00000000*/

                            /*
                               Captured Slot Power Limit Value: Does not apply
                               to EP.
                            */
    UINT32 csps : 2;

                            /* Bits[27:26], Access Type=RO, default=0x00000000*/

                            /*
                               Captured Slot Power Limit Scale: Does not apply
                               to EP.
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

} HQM_PCIE_CAP_PCIE_CAP_DEVICE_CAP_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_CAP_PCIE_CAP_DEVICE_CONTROL_IIO_HQM_PF_REG supported on:            */
/*      SPRA0 (0x10e00074)                                                      */
/*      SPRB0 (0x10e00074)                                                      */
/*      SPRC0 (0x10e00074)                                                      */
/*      SPRMCC (0x10e00074)                                                     */
/*      SPRUCC (0x10e00074)                                                     */
/* Register default value on SPRA0: 0x00002910                                  */
/* Register default value on SPRB0: 0x00002910                                  */
/* Register default value on SPRC0: 0x00002910                                  */
/* Register default value on SPRMCC: 0x00002910                                 */
/* Register default value on SPRUCC: 0x00002910                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF PCI Express Device Control Register
*/


#define HQM_PCIE_CAP_PCIE_CAP_DEVICE_CONTROL_IIO_HQM_PF_REG 0x21410074

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
                               Enable Relaxed Ordering: If this bit is set, EP
                               is permitted to set the Relaxed Ordering bit in
                               the Attributes field of transactions it
                               initiates.Note that seting this bit does not
                               cause the EP to set the RO on every transaction
                               it issues.
                            */
    UINT16 mps : 3;

                            /* Bits[7:5], Access Type=RW, default=0x00000000*/

                            /*
                               Max_Payload_Size (MPS): This field sets maximum
                               TLP payload for EP. As a Receiver, the EP must
                               handle TLPs as large as the set value; as a
                               Transmitter, the EP must not generate TLPs
                               exceeding the set value.The EP is capable of
                               generating up to 512B MPS. However requests
                               generated by the EP will be limited by the
                               programmed value in this field.It is expected
                               that the root complexes that the EP will be
                               paired up with will be limited to 512B MPS.000b
                               = 128B(Default)001b = 256B010b = 512BOthers
                               values not supported by EP
                            */
    UINT16 etfe : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Extended Tag Field Enable: When set, this bit
                               enables a device to use an 8-bit Tag field as a
                               requester. Supports 256 outstanding requests.
                            */
    UINT16 pfe : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Phantom Functions Enable: When set, this bit
                               enables a device to use unclaimed functions as
                               Phantom Functions. Does not apply to EP.
                            */
    UINT16 apme : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Auxiliary (AUX) Power PM Enable: This bit when
                               set enables a device to draw AUX power
                               independent of PME AUX power. Does not apply to
                               EP
                            */
    UINT16 ens : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Enable No Snoop (NS): If this bit is set to 1,
                               EP is permitted to set the No Snoop bit in the
                               Requester Attributes of transactions it
                               initiates. When clear all transactions will have
                               the No Snoop bit clear.Note that setting this
                               bit will not cause the EP to set the No Snoop
                               attribute on every memory requests that it
                               initiates.
                            */
    UINT16 mrs : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000002*/

                            /*
                               Max_Read_Request_Size: This field sets the
                               maximum Read Request size for the EP as a
                               Requester.The EP is capable for generating up to
                               512B read requests. However requests generated
                               by the EP will be limited by the programmed
                               value in this field.Defined encodings for this
                               field are:000b = 128B max read request size001b
                               = 256B max read request size010b = 512B max read
                               request size (Default)011b = 1024B max read
                               request size100b = 2048B max read request
                               size101b = 4096B max read request sizeFor HQM
                               valid setting for this field is upto 512B.
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

} HQM_PCIE_CAP_PCIE_CAP_DEVICE_CONTROL_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_CAP_PCIE_CAP_DEVICE_STATUS_IIO_HQM_PF_REG supported on:             */
/*      SPRA0 (0x10e00076)                                                      */
/*      SPRB0 (0x10e00076)                                                      */
/*      SPRC0 (0x10e00076)                                                      */
/*      SPRMCC (0x10e00076)                                                     */
/*      SPRUCC (0x10e00076)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF PCI Express Device Status Register
*/


#define HQM_PCIE_CAP_PCIE_CAP_DEVICE_STATUS_IIO_HQM_PF_REG 0x21410076

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
                               that EP received an Unsupported Request.A one
                               indicates that an error was detected since the
                               last time this bit was cleared.Errors are
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
                               indicates that EP has issued Non-Posted Requests
                               which have not been completed either with a
                               completion packet or completion timeout
                               mechanism.
                            */
    UINT16 reserved : 10;

                            /* Bits[15:6], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} HQM_PCIE_CAP_PCIE_CAP_DEVICE_STATUS_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_CAP_PCIE_CAP_DEVICE_CAP_2_IIO_HQM_PF_REG supported on:              */
/*      SPRA0 (0x20e00090)                                                      */
/*      SPRB0 (0x20e00090)                                                      */
/*      SPRC0 (0x20e00090)                                                      */
/*      SPRMCC (0x20e00090)                                                     */
/*      SPRUCC (0x20e00090)                                                     */
/* Register default value on SPRA0: 0x00710010                                  */
/* Register default value on SPRB0: 0x00710010                                  */
/* Register default value on SPRC0: 0x00710010                                  */
/* Register default value on SPRMCC: 0x00710010                                 */
/* Register default value on SPRUCC: 0x00710010                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Device Capabilities 2 Register
*/


#define HQM_PCIE_CAP_PCIE_CAP_DEVICE_CAP_2_IIO_HQM_PF_REG 0x21420090

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctors : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000000*/

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
    UINT32 reserved : 11;

                            /* Bits[15:5], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 cmp10btags : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000001*/

                            /*
                               Completer 10b Tag Supported.A value of 1b
                               indicates support for 10b tags as a completer.
                            */
    UINT32 req10btags : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Requester 10b Tag Supported.A value of 1b
                               indicates support for 10b tags as a requester.
                            */
    UINT32 reserved2 : 2;

                            /* Bits[19:18], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 effs : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000001*/

                            /*
                               Extended Fmt Field Supported. If Set, the
                               Function supports the 3-bit definition of the
                               Fmt field. If Clear, the Function supports a
                               2-bit definition of the Fmt field.
                            */
    UINT32 e2etlpps : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000001*/

                            /*
                               End-End TLP Prefix Supported.A value of 1b
                               indicates support for end to end TLP prefixes.
                            */
    UINT32 maxe2etlpp : 2;

                            /* Bits[23:22], Access Type=RO, default=0x00000001*/

                            /*
                               Maximum End-End TLP Prefixes Supported.A value
                               of 0 indicates support for 4 end to end TLP
                               prefixes, else indicates the number of end to
                               end TLP prefixes supported.
                            */
    UINT32 reserved3 : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} HQM_PCIE_CAP_PCIE_CAP_DEVICE_CAP_2_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_CAP_PCIE_CAP_DEVICE_CONTROL_2_IIO_HQM_PF_REG supported on:          */
/*      SPRA0 (0x10e00094)                                                      */
/*      SPRB0 (0x10e00094)                                                      */
/*      SPRC0 (0x10e00094)                                                      */
/*      SPRMCC (0x10e00094)                                                     */
/*      SPRUCC (0x10e00094)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Device Control 2 Register
*/


#define HQM_PCIE_CAP_PCIE_CAP_DEVICE_CONTROL_2_IIO_HQM_PF_REG 0x21410094

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ctov : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000000*/

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
    UINT16 reserved0 : 3;

                            /* Bits[7:5], Access Type=RO, default=0x00000000*/

                            /* Reserved0 */
    UINT16 eido : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Enable ID Based Ordering: If this bit is set, EP
                               is permitted to set the ID Based Ordering bit in
                               the Attributes field of transactions it
                               initiates.Note that seting this bit does not
                               cause the EP to set the IDO on every transaction
                               it issues.
                            */
    UINT16 reserved1 : 7;

                            /* Bits[15:9], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */

  } Bits;
  UINT16 Data;

} HQM_PCIE_CAP_PCIE_CAP_DEVICE_CONTROL_2_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_PM_CAP_PM_CAP_ID_IIO_HQM_PF_REG supported on:                       */
/*      SPRA0 (0xe000b0)                                                        */
/*      SPRB0 (0xe000b0)                                                        */
/*      SPRC0 (0xe000b0)                                                        */
/*      SPRMCC (0xe000b0)                                                       */
/*      SPRUCC (0xe000b0)                                                       */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF Power Management Capabilities ID Register
*/


#define HQM_PCIE_PM_CAP_PM_CAP_ID_IIO_HQM_PF_REG 0x214000B0

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

} HQM_PCIE_PM_CAP_PM_CAP_ID_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_PM_CAP_PM_CAP_NEXT_CAP_PTR_IIO_HQM_PF_REG supported on:             */
/*      SPRA0 (0xe000b1)                                                        */
/*      SPRB0 (0xe000b1)                                                        */
/*      SPRC0 (0xe000b1)                                                        */
/*      SPRMCC (0xe000b1)                                                       */
/*      SPRUCC (0xe000b1)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF Power Management Next Capability Pointer Register
*/


#define HQM_PCIE_PM_CAP_PM_CAP_NEXT_CAP_PTR_IIO_HQM_PF_REG 0x214000B1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 cap_ptr : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Next Capability Pointer (ACS capabilities) */

  } Bits;
  UINT8 Data;

} HQM_PCIE_PM_CAP_PM_CAP_NEXT_CAP_PTR_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_PM_CAP_PM_CAP_IIO_HQM_PF_REG supported on:                          */
/*      SPRA0 (0x10e000b2)                                                      */
/*      SPRB0 (0x10e000b2)                                                      */
/*      SPRC0 (0x10e000b2)                                                      */
/*      SPRMCC (0x10e000b2)                                                     */
/*      SPRUCC (0x10e000b2)                                                     */
/* Register default value on SPRA0: 0x00000023                                  */
/* Register default value on SPRB0: 0x00000023                                  */
/* Register default value on SPRC0: 0x00000023                                  */
/* Register default value on SPRMCC: 0x00000023                                 */
/* Register default value on SPRUCC: 0x00000023                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF Power Management Capabilities Register
*/


#define HQM_PCIE_PM_CAP_PM_CAP_IIO_HQM_PF_REG 0x214100B2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 pmv : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000003*/

                            /*
                               Version. This field is set to 3 to indicate that
                               the EP is compliant with the PCI bus Power
                               Management Interface Specification Rev 1.2.
                            */
    UINT16 pmc : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /* PME clock. Does not apply to PCIe. */
    UINT16 iror2d0 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Immediate_Readiness_on_Return_to_D0: If this bit
                               set, this Function is guaranteed to be ready to
                               successfully complete valid accesses immediately
                               after being set to D0. These accesses include
                               Configuration, Memory, and I/O Cycles. When this
                               bit set, for accesses to this Function, software
                               is exempt from all requirements to delay
                               accesses following a transition to D0, including
                               but not limited to the 10 ms delay
                            */
    UINT16 dsi : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000001*/

                            /*
                               Device specific initialization: A one in this
                               bit indicates that immediately after entry into
                               the D0 Uninitialized state, the function
                               requires additional configuration above and
                               beyond setup of its PCI configuration Header
                               registers before the Class driver can use the
                               function.For the EP this bit is set to 1.
                            */
    UINT16 ac : 3;

                            /* Bits[8:6], Access Type=RO, default=0x00000000*/

                            /* Aux current: Not relevant for EP. */
    UINT16 d1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /* D1 Support. EP does not support D1 state. */
    UINT16 d2 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /* D2 Support. EP does not support D2 state. */
    UINT16 pme : 5;

                            /* Bits[15:11], Access Type=RO, default=0x00000000*/

                            /*
                               PME Support: Set to indicate that the EP will
                               not assert PME# in any state.
                            */

  } Bits;
  UINT16 Data;

} HQM_PCIE_PM_CAP_PM_CAP_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_PM_CAP_PM_CAP_CONTROL_STATUS_IIO_HQM_PF_REG supported on:           */
/*      SPRA0 (0x20e000b4)                                                      */
/*      SPRB0 (0x20e000b4)                                                      */
/*      SPRC0 (0x20e000b4)                                                      */
/*      SPRMCC (0x20e000b4)                                                     */
/*      SPRUCC (0x20e000b4)                                                     */
/* Register default value on SPRA0: 0x00000008                                  */
/* Register default value on SPRB0: 0x00000008                                  */
/* Register default value on SPRC0: 0x00000008                                  */
/* Register default value on SPRMCC: 0x00000008                                 */
/* Register default value on SPRUCC: 0x00000008                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PF Power Management Control and Status Register
*/


#define HQM_PCIE_PM_CAP_PM_CAP_CONTROL_STATUS_IIO_HQM_PF_REG 0x214200B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ps : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               Power State. This field is used to determine the
                               current power state of EP and to set a new power
                               state.If software selects a Power state that is
                               not supported by the EP (D2/D1), the writes must
                               complete normally on PCIe, but the write data is
                               discarded and no state change occurs.
                            */
    UINT32 reserved2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /* Reserved2 */
    UINT32 nsr : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               No_Soft_Reset: The EP will not maintain
                               functional context in the D3_hot state. So when
                               the EP is transitioned from D3_hot to D0 it will
                               be in the D0_uniniitialized state and therefore
                               would require software to initialize it before
                               it can transition to the D0_initialized
                               state.When set (1), this bit indicates that
                               devices transitioning from D3hot to D0 because
                               of Power State commands do not perform an
                               internal reset. Configuration Context is
                               preserved. Upon transition from the D3_hot to
                               the D0_Initialized state, no additional
                               operating system intervention is required to
                               preserve Configuration Context beyond writing
                               the PowerState bits.When clear (0), devices do
                               perform an internal reset upon transitioning
                               from D3_hot to D0 via software control of the
                               Power State bits. Configuration Context is lost
                               when performing the soft reset. Upon transition
                               from the D3_hot to the D0 state, full
                               reinitialization sequence is needed to return
                               the device to D0_initialized state
                            */
    UINT32 reserved1 : 4;

                            /* Bits[7:4], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */
    UINT32 pme : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               PME Enable: Hardwired to 0 since EP does not
                               generate a PME.
                            */
    UINT32 dsel : 4;

                            /* Bits[12:9], Access Type=RO, default=0x00000000*/

                            /*
                               Data Select: Set to 0 for EP because the EP does
                               not implement the Data register (The Data
                               register is an optional, 8-bit read-only
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
                               Data Scale: Set to 0 because the EP does not
                               implement the Data register.If the Data register
                               has not been implemented, this field must return
                               00b when the PMCSR is read.
                            */
    UINT32 pms : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               PME Status: Set to 0 since EP will not generate
                               PME.
                            */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* Reserved/NA/Not Supported for EP. */

  } Bits;
  UINT32 Data;

} HQM_PCIE_PM_CAP_PM_CAP_CONTROL_STATUS_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_ACS_CAP_ACS_CAP_ID_IIO_HQM_PF_REG supported on:                     */
/*      SPRA0 (0x10e00100)                                                      */
/*      SPRB0 (0x10e00100)                                                      */
/*      SPRC0 (0x10e00100)                                                      */
/*      SPRMCC (0x10e00100)                                                     */
/*      SPRUCC (0x10e00100)                                                     */
/* Register default value on SPRA0: 0x0000000D                                  */
/* Register default value on SPRB0: 0x0000000D                                  */
/* Register default value on SPRC0: 0x0000000D                                  */
/* Register default value on SPRMCC: 0x0000000D                                 */
/* Register default value on SPRUCC: 0x0000000D                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ACS capability
*/


#define HQM_PCIE_ACS_CAP_ACS_CAP_ID_IIO_HQM_PF_REG 0x21410100

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

} HQM_PCIE_ACS_CAP_ACS_CAP_ID_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_ACS_CAP_ACS_CAP_VERSION_NEXT_PTR_IIO_HQM_PF_REG supported on:       */
/*      SPRA0 (0x10e00102)                                                      */
/*      SPRB0 (0x10e00102)                                                      */
/*      SPRC0 (0x10e00102)                                                      */
/*      SPRMCC (0x10e00102)                                                     */
/*      SPRUCC (0x10e00102)                                                     */
/* Register default value on SPRA0: 0x00001081                                  */
/* Register default value on SPRB0: 0x00001081                                  */
/* Register default value on SPRC0: 0x00001081                                  */
/* Register default value on SPRMCC: 0x00001081                                 */
/* Register default value on SPRUCC: 0x00001081                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ACS Capability Version
*/


#define HQM_PCIE_ACS_CAP_ACS_CAP_VERSION_NEXT_PTR_IIO_HQM_PF_REG 0x21410102

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 acscv : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000001*/

                            /*
                               Capability Version - This is set to 1h for the
                               Single Root I/O Virtualization and Sharing
                               Specification, Revision 1.1.
                            */
    UINT16 cap_ptr : 12;

                            /* Bits[15:4], Access Type=RO, default=0x00000108*/

                            /* Next Capability Pointer (SR-IOV Capabilties) */

  } Bits;
  UINT16 Data;

} HQM_PCIE_ACS_CAP_ACS_CAP_VERSION_NEXT_PTR_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_ACS_CAP_ACS_CAP_IIO_HQM_PF_REG supported on:                        */
/*      SPRA0 (0x10e00104)                                                      */
/*      SPRB0 (0x10e00104)                                                      */
/*      SPRC0 (0x10e00104)                                                      */
/*      SPRMCC (0x10e00104)                                                     */
/*      SPRUCC (0x10e00104)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ACS Capabilities Register
*/


#define HQM_PCIE_ACS_CAP_ACS_CAP_IIO_HQM_PF_REG 0x21410104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 acssv : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Source Validation (V) Hard-wired to zero,
                               not supported in HQM.
                            */
    UINT16 acstb : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Translation Blocking (B) Hard-wired to zero,
                               not supported in HQM..
                            */
    UINT16 acsp2prr : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               ACS P2P Request Redirect (R) Hard-wired to zero,
                               not supported in HQM.
                            */
    UINT16 acsp2pcr : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               ACS P2P Completion Redirect (C) Hard-wired to
                               zero, not supported in HQM..
                            */
    UINT16 acsuf : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Upstream Forwarding (U) Hard-wired to zero,
                               not supported in HQM..
                            */
    UINT16 acsp2pec : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               ACS P2P Egress Control (E) Hard-wired to zero,
                               not supported in HQM..
                            */
    UINT16 acsdtp2p : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Direct Translated P2P (T) Hard-wired to
                               zero, not supported in HQM..
                            */
    UINT16 reserved1 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /* Reserved.1 */
    UINT16 acsecvs : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*
                               Egress Control Vector Size Hard-wired to zero,
                               not supported in HQM..
                            */

  } Bits;
  UINT16 Data;

} HQM_PCIE_ACS_CAP_ACS_CAP_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_ACS_CAP_ACS_CAP_CONTROL_IIO_HQM_PF_REG supported on:                */
/*      SPRA0 (0x10e00106)                                                      */
/*      SPRB0 (0x10e00106)                                                      */
/*      SPRC0 (0x10e00106)                                                      */
/*      SPRMCC (0x10e00106)                                                     */
/*      SPRUCC (0x10e00106)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ACS Capabilities Register
*/


#define HQM_PCIE_ACS_CAP_ACS_CAP_CONTROL_IIO_HQM_PF_REG 0x21410106

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 acssve : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Source Validation Enable (V) Hard-wired to
                               zero, not supported in HQM..
                            */
    UINT16 acstbe : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Translation Blocking Enable (B) Hard-wired
                               to zero, not supported in HQM.
                            */
    UINT16 acsp2prre : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               ACS P2P Request Redirect Enable (R) Hard-wired
                               to zero, not supported in HQM..
                            */
    UINT16 acsp2pcre : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               ACS P2P Completion Redirect Enable (C) Hard-
                               wired to zero, not supported in HQM..
                            */
    UINT16 acsufe : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Upstream Forwarding Enable (U) Hard-wired to
                               zero, not supported in HQM..
                            */
    UINT16 acsp2pece : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               ACS P2P Egress Control Enable (E) Hard-wired to
                               zero, not supported in HQM..
                            */
    UINT16 acsdtp2pe : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Direct Translated P2P Enable (T) Hard-wired
                               to zero, not supported in HQM..
                            */
    UINT16 reserved : 9;

                            /* Bits[15:7], Access Type=RO, default=0x00000000*/

                            /* Reserved0 */

  } Bits;
  UINT16 Data;

} HQM_PCIE_ACS_CAP_ACS_CAP_CONTROL_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* SRIOV_CAP_ID_IIO_HQM_PF_REG supported on:                                    */
/*      SPRA0 (0x10e00108)                                                      */
/*      SPRB0 (0x10e00108)                                                      */
/*      SPRC0 (0x10e00108)                                                      */
/*      SPRMCC (0x10e00108)                                                     */
/*      SPRUCC (0x10e00108)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SR IOV Capability ID Register
*/


#define SRIOV_CAP_ID_IIO_HQM_PF_REG 0x21410108

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

} SRIOV_CAP_ID_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* SRIOV_CAP_VERSION_NEXT_PTR_IIO_HQM_PF_REG supported on:                      */
/*      SPRA0 (0x10e0010a)                                                      */
/*      SPRB0 (0x10e0010a)                                                      */
/*      SPRC0 (0x10e0010a)                                                      */
/*      SPRMCC (0x10e0010a)                                                     */
/*      SPRUCC (0x10e0010a)                                                     */
/* Register default value on SPRA0: 0x00001481                                  */
/* Register default value on SPRB0: 0x00001481                                  */
/* Register default value on SPRC0: 0x00001481                                  */
/* Register default value on SPRMCC: 0x00001481                                 */
/* Register default value on SPRUCC: 0x00001481                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SRIOV Capability Version and Next Capability Pointer Register
*/


#define SRIOV_CAP_VERSION_NEXT_PTR_IIO_HQM_PF_REG 0x2141010A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 sriovcv : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000001*/

                            /*
                               Capability Version - This is set to 1h for the
                               Single Root I/O Virtualization and Sharing
                               Specification, Revision 1.1.
                            */
    UINT16 cap_ptr : 12;

                            /* Bits[15:4], Access Type=RO, default=0x00000148*/

                            /* Next Capability Pointer (PASID Capabilties) */

  } Bits;
  UINT16 Data;

} SRIOV_CAP_VERSION_NEXT_PTR_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* SRIOV_CAP_IIO_HQM_PF_REG supported on:                                       */
/*      SPRA0 (0x20e0010c)                                                      */
/*      SPRB0 (0x20e0010c)                                                      */
/*      SPRC0 (0x20e0010c)                                                      */
/*      SPRMCC (0x20e0010c)                                                     */
/*      SPRUCC (0x20e0010c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SRIOV Capabilities Register 
*/


#define SRIOV_CAP_IIO_HQM_PF_REG 0x2142010C

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
    UINT32 vfarichp : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /*
                               ARI Capable Hierarchy Preserved - If Set, the
                               ARI Capable Hierarchy bit is preserved across
                               certain power state transitions.
                            */
    UINT32 vfreq10btagc : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               VF 10-Bit Tag Requester Supported - If Set, all
                               VFs must support 10-Bit Tag Requester
                               capability. If Clear, VFs must not support
                               10-Bit Tag Requester capability. If the 10-Bit
                               Tag Requester Supported bit in the PF's Device
                               Capabilities 2 Register is Clear, this bit must
                               be Clear.
                            */
    UINT32 reserved : 18;

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

} SRIOV_CAP_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* SRIOV_CAP_CONTROL_STATUS_IIO_HQM_PF_REG supported on:                        */
/*      SPRA0 (0x20e00110)                                                      */
/*      SPRB0 (0x20e00110)                                                      */
/*      SPRC0 (0x20e00110)                                                      */
/*      SPRMCC (0x20e00110)                                                     */
/*      SPRUCC (0x20e00110)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SRIOV Control and Status Register
*/


#define SRIOV_CAP_CONTROL_STATUS_IIO_HQM_PF_REG 0x21420110

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
                               number.EP supports ARI and will locate the
                               VF1-VF16 in Function 1 through Function 16.
                            */
    UINT32 reserved : 27;

                            /* Bits[31:5], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SRIOV_CAP_CONTROL_STATUS_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* SRIOV_CAP_INITIAL_VF_CNT_IIO_HQM_PF_REG supported on:                        */
/*      SPRA0 (0x10e00114)                                                      */
/*      SPRB0 (0x10e00114)                                                      */
/*      SPRC0 (0x10e00114)                                                      */
/*      SPRMCC (0x10e00114)                                                     */
/*      SPRUCC (0x10e00114)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SRIOV Initial VFs Register
*/


#define SRIOV_CAP_INITIAL_VF_CNT_IIO_HQM_PF_REG 0x21410114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 initvf : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000010*/

                            /*
                               InitialVFs indicates the number of VFs that are
                               initially associated with the PF.Since EP does
                               not support VF migration this field must be
                               equal to the TOTVF field
                            */

  } Bits;
  UINT16 Data;

} SRIOV_CAP_INITIAL_VF_CNT_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* SRIOV_CAP_TOTAL_VF_CNT_IIO_HQM_PF_REG supported on:                          */
/*      SPRA0 (0x10e00116)                                                      */
/*      SPRB0 (0x10e00116)                                                      */
/*      SPRC0 (0x10e00116)                                                      */
/*      SPRMCC (0x10e00116)                                                     */
/*      SPRUCC (0x10e00116)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SRIOV Total VFs Register
*/


#define SRIOV_CAP_TOTAL_VF_CNT_IIO_HQM_PF_REG 0x21410116

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 totvf : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000010*/

                            /*
                               TotalVFs indicates the maximum number of VFs
                               that could be associated with the PF.Since EP
                               does not support VF migration this field must be
                               equal to the INITVF field
                            */

  } Bits;
  UINT16 Data;

} SRIOV_CAP_TOTAL_VF_CNT_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* SRIOV_CAP_NUM_VF_IIO_HQM_PF_REG supported on:                                */
/*      SPRA0 (0x20e00118)                                                      */
/*      SPRB0 (0x20e00118)                                                      */
/*      SPRC0 (0x20e00118)                                                      */
/*      SPRMCC (0x20e00118)                                                     */
/*      SPRUCC (0x20e00118)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SRIOV Number of VFs Register
*/


#define SRIOV_CAP_NUM_VF_IIO_HQM_PF_REG 0x21420118

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
                               value between 1 and TotalVFs (16 for EP) as part
                               of the process of creating VFs.NumVFs VFs shall
                               be visible in the PCI-Express fabric after both
                               NumVFs is set to a valid value and VF Enable is
                               set to one. Visible in the PCI-Express fabric
                               means that the VF shall respond to PCI Express
                               transactions targeting the VF, following all
                               other rules defined by this specification and
                               the base specification.
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

} SRIOV_CAP_NUM_VF_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* SRIOV_CAP_FIRST_VF_OFFSET_IIO_HQM_PF_REG supported on:                       */
/*      SPRA0 (0x10e0011c)                                                      */
/*      SPRB0 (0x10e0011c)                                                      */
/*      SPRC0 (0x10e0011c)                                                      */
/*      SPRMCC (0x10e0011c)                                                     */
/*      SPRUCC (0x10e0011c)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SRIOV First VF Offset Register
*/


#define SRIOV_CAP_FIRST_VF_OFFSET_IIO_HQM_PF_REG 0x2141011C

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

} SRIOV_CAP_FIRST_VF_OFFSET_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* SRIOV_CAP_VF_STRIDE_IIO_HQM_PF_REG supported on:                             */
/*      SPRA0 (0x10e0011e)                                                      */
/*      SPRB0 (0x10e0011e)                                                      */
/*      SPRC0 (0x10e0011e)                                                      */
/*      SPRMCC (0x10e0011e)                                                     */
/*      SPRUCC (0x10e0011e)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SRIOV VF Stride Register
*/


#define SRIOV_CAP_VF_STRIDE_IIO_HQM_PF_REG 0x2141011E

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

} SRIOV_CAP_VF_STRIDE_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* SRIOV_CAP_VF_DEVICE_ID_IIO_HQM_PF_REG supported on:                          */
/*      SPRA0 (0x10e00122)                                                      */
/*      SPRB0 (0x10e00122)                                                      */
/*      SPRC0 (0x10e00122)                                                      */
/*      SPRMCC (0x10e00122)                                                     */
/*      SPRUCC (0x10e00122)                                                     */
/* Register default value on SPRA0: 0x00002711                                  */
/* Register default value on SPRB0: 0x00002711                                  */
/* Register default value on SPRC0: 0x00002711                                  */
/* Register default value on SPRMCC: 0x00002711                                 */
/* Register default value on SPRUCC: 0x00002711                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SRIOV VF Device ID Register
*/


#define SRIOV_CAP_VF_DEVICE_ID_IIO_HQM_PF_REG 0x21410122

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 vfdid151 : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00002711*/

                            /*
                               VF Device ID - The Device ID that is presented
                               to the OS for every VF.
                            */

  } Bits;
  UINT16 Data;

} SRIOV_CAP_VF_DEVICE_ID_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* SRIOV_CAP_SUPP_PAGE_SIZE_IIO_HQM_PF_REG supported on:                        */
/*      SPRA0 (0x20e00124)                                                      */
/*      SPRB0 (0x20e00124)                                                      */
/*      SPRC0 (0x20e00124)                                                      */
/*      SPRMCC (0x20e00124)                                                     */
/*      SPRUCC (0x20e00124)                                                     */
/* Register default value on SPRA0: 0x00000553                                  */
/* Register default value on SPRB0: 0x00000553                                  */
/* Register default value on SPRC0: 0x00000553                                  */
/* Register default value on SPRMCC: 0x00000553                                 */
/* Register default value on SPRUCC: 0x00000553                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SRIOV Supported Page Size Register
*/


#define SRIOV_CAP_SUPP_PAGE_SIZE_IIO_HQM_PF_REG 0x21420124

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 firstvfo : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000553*/

                            /*
                               This field must define the supported page sizes.
                               This PF supports a page size of 2^(n+12) if bit
                               n is Set. For example, if bit 0 is Set, the PF
                               supports 4k byte page sizes. PFs are required to
                               support 4k, 8k, 64k, 256k, 1M and 4M page sizes.
                               All other page sizes are optional.A page size
                               describes the minimum alignment requirements for
                               VF BAR resources as described inSystem Page
                               Size.
                            */

  } Bits;
  UINT32 Data;

} SRIOV_CAP_SUPP_PAGE_SIZE_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* SRIOV_CAP_SYS_PAGE_SIZE_IIO_HQM_PF_REG supported on:                         */
/*      SPRA0 (0x20e00128)                                                      */
/*      SPRB0 (0x20e00128)                                                      */
/*      SPRC0 (0x20e00128)                                                      */
/*      SPRMCC (0x20e00128)                                                     */
/*      SPRUCC (0x20e00128)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SRIOV System Page Size Register
*/


#define SRIOV_CAP_SYS_PAGE_SIZE_IIO_HQM_PF_REG 0x21420128

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
                               Supported Page Sizes field.As with Supported
                               Page Sizes, if bit n is Set in System Page Size,
                               the VFs associated with this PF are required to
                               support a page size of 2^(n+12). For example, if
                               bit 1 is Set, the system is using an 8k byte
                               page size. The results are undefined if more
                               than one bit is set in System Page Size. The
                               results are undefined if a bit is Set in System
                               Page Size that is not Set in Supported Page
                               Sizes.When System Page Size is set, the VF
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

} SRIOV_CAP_SYS_PAGE_SIZE_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* SRIOV_CAP_FUNC_BAR_L_IIO_HQM_PF_REG supported on:                            */
/*      SPRA0 (0x20e0012c)                                                      */
/*      SPRB0 (0x20e0012c)                                                      */
/*      SPRC0 (0x20e0012c)                                                      */
/*      SPRMCC (0x20e0012c)                                                     */
/*      SPRUCC (0x20e0012c)                                                     */
/* Register default value on SPRA0: 0x0000000C                                  */
/* Register default value on SPRB0: 0x0000000C                                  */
/* Register default value on SPRC0: 0x0000000C                                  */
/* Register default value on SPRMCC: 0x0000000C                                 */
/* Register default value on SPRUCC: 0x0000000C                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SRIOV Function Lower Base Address Register
*/


#define SRIOV_CAP_FUNC_BAR_L_IIO_HQM_PF_REG 0x2142012C

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

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Prefetchable: Hardwired to 1 to indicate that
                               the region is prefetchable.0: non-prefetchable
                               1: prefetchable
                            */
    UINT32 vfzero : 22;

                            /* Bits[25:4], Access Type=RO, default=0x00000000*/

                            /* Lower Bits: Hardwired to 0 (64MB region) */
    UINT32 vfbar0_l : 6;

                            /* Bits[31:26], Access Type=RW, default=0x00000000*/

                            /* Lower Programmable Base Address bits */

  } Bits;
  UINT32 Data;

} SRIOV_CAP_FUNC_BAR_L_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* SRIOV_CAP_FUNC_BAR_U_IIO_HQM_PF_REG supported on:                            */
/*      SPRA0 (0x20e00130)                                                      */
/*      SPRB0 (0x20e00130)                                                      */
/*      SPRC0 (0x20e00130)                                                      */
/*      SPRMCC (0x20e00130)                                                     */
/*      SPRUCC (0x20e00130)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SRIOV Upper BAR0 Register
*/


#define SRIOV_CAP_FUNC_BAR_U_IIO_HQM_PF_REG 0x21420130

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vfbar0 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Upper SRIOV Base Address 0: These bits are used
                               to define the memory window for Virtual
                               Functions. This window is used by Virtual
                               Functions to address the producer port, mailbox,
                               and VF provisioning addresses.
                            */

  } Bits;
  UINT32 Data;

} SRIOV_CAP_FUNC_BAR_U_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* SRIOV_CAP_VF_MIGRATION_IIO_HQM_PF_REG supported on:                          */
/*      SPRA0 (0x20e00144)                                                      */
/*      SPRB0 (0x20e00144)                                                      */
/*      SPRC0 (0x20e00144)                                                      */
/*      SPRMCC (0x20e00144)                                                     */
/*      SPRUCC (0x20e00144)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SRIOV VF Migration Array Register
*/


#define SRIOV_CAP_VF_MIGRATION_IIO_HQM_PF_REG 0x21420144

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

} SRIOV_CAP_VF_MIGRATION_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_PASID_CAP_PASID_CAP_ID_IIO_HQM_PF_REG supported on:                 */
/*      SPRA0 (0x10e00148)                                                      */
/*      SPRB0 (0x10e00148)                                                      */
/*      SPRC0 (0x10e00148)                                                      */
/*      SPRMCC (0x10e00148)                                                     */
/*      SPRUCC (0x10e00148)                                                     */
/* Register default value on SPRA0: 0x0000001B                                  */
/* Register default value on SPRB0: 0x0000001B                                  */
/* Register default value on SPRC0: 0x0000001B                                  */
/* Register default value on SPRMCC: 0x0000001B                                 */
/* Register default value on SPRUCC: 0x0000001B                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PASID Capability ID Register
*/


#define HQM_PCIE_PASID_CAP_PASID_CAP_ID_IIO_HQM_PF_REG 0x21410148

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 cap_id : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000001B*/

                            /*
                               Capability ID: PCI SIG assigned a capability
                               record ID for PASID
                            */

  } Bits;
  UINT16 Data;

} HQM_PCIE_PASID_CAP_PASID_CAP_ID_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_PASID_CAP_PASID_CAP_VERSION_NEXT_PTR_IIO_HQM_PF_REG supported on:   */
/*      SPRA0 (0x10e0014a)                                                      */
/*      SPRB0 (0x10e0014a)                                                      */
/*      SPRC0 (0x10e0014a)                                                      */
/*      SPRMCC (0x10e0014a)                                                     */
/*      SPRUCC (0x10e0014a)                                                     */
/* Register default value on SPRA0: 0x00001501                                  */
/* Register default value on SPRB0: 0x00001501                                  */
/* Register default value on SPRC0: 0x00001501                                  */
/* Register default value on SPRMCC: 0x00001501                                 */
/* Register default value on SPRUCC: 0x00001501                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PASID Capability Version and Next Capability Pointer Register
*/


#define HQM_PCIE_PASID_CAP_PASID_CAP_VERSION_NEXT_PTR_IIO_HQM_PF_REG 0x2141014A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 sriovcv : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000001*/

                            /* Capability Version */
    UINT16 cap_ptr : 12;

                            /* Bits[15:4], Access Type=RO, default=0x00000150*/

                            /*
                               Next Capability Pointer (SCIOV (DVSEC)
                               Capabilties)
                            */

  } Bits;
  UINT16 Data;

} HQM_PCIE_PASID_CAP_PASID_CAP_VERSION_NEXT_PTR_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_PASID_CAP_PASID_CAP_IIO_HQM_PF_REG supported on:                    */
/*      SPRA0 (0x10e0014c)                                                      */
/*      SPRB0 (0x10e0014c)                                                      */
/*      SPRC0 (0x10e0014c)                                                      */
/*      SPRMCC (0x10e0014c)                                                     */
/*      SPRUCC (0x10e0014c)                                                     */
/* Register default value on SPRA0: 0x00001400                                  */
/* Register default value on SPRB0: 0x00001400                                  */
/* Register default value on SPRC0: 0x00001400                                  */
/* Register default value on SPRMCC: 0x00001400                                 */
/* Register default value on SPRUCC: 0x00001400                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PASID Capabilities Register 
*/


#define HQM_PCIE_PASID_CAP_PASID_CAP_IIO_HQM_PF_REG 0x2141014C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 reserved0 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /* Reserved0. */
    UINT16 exec_perm_supp : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Execute Permission Supported - If set, the
                               Endpoint supports sending TLPs that have the
                               Execute Requested bit set. If Clear, the
                               Endpoint will never Set the Execute Requested
                               bit.
                            */
    UINT16 priv_mode_supp : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Privileged Mode Supported - If set, the Endpoint
                               supports operating in Privileged and Non-
                               Privileged modes, and supports sending requests
                               that have the Privileged Mode Requested bit set.
                               If Clear, the Endpoint will never Set the
                               Privileged Mode Requested bit.
                            */
    UINT16 reserved1 : 5;

                            /* Bits[7:3], Access Type=RO, default=0x00000000*/

                            /* Reserved1. */
    UINT16 max_pasid_width : 5;

                            /* Bits[12:8], Access Type=RO, default=0x00000014*/

                            /*
                               Maximum PASID width - Indicates the width of the
                               PASID field supported by the Endpoint. A value N
                               indicates support for PASID values 0 through
                               (2**N)-1. The value 0 indicates support for a
                               single PASID (0). The value 20 indicates support
                               for all PASID values (20 bits). This field must
                               be between 0 and 20 (inclusive)
                            */
    UINT16 reserved2 : 3;

                            /* Bits[15:13], Access Type=RO, default=0x00000000*/

                            /* Reserved2. */

  } Bits;
  UINT16 Data;

} HQM_PCIE_PASID_CAP_PASID_CAP_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_PASID_CAP_PASID_CONTROL_IIO_HQM_PF_REG supported on:                */
/*      SPRA0 (0x10e0014e)                                                      */
/*      SPRB0 (0x10e0014e)                                                      */
/*      SPRC0 (0x10e0014e)                                                      */
/*      SPRMCC (0x10e0014e)                                                     */
/*      SPRUCC (0x10e0014e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PASID Control Register
*/


#define HQM_PCIE_PASID_CAP_PASID_CONTROL_IIO_HQM_PF_REG 0x2141014E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 pasid_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* PASID Enable. */
    UINT16 exec_perm_enable : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Execute Permission Enable - HQM does not support
                               Execute Permission.
                            */
    UINT16 priv_mode_enable : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Privileged Mode Enable - HQM does not support
                               Privileged Mode.
                            */
    UINT16 reserved : 13;

                            /* Bits[15:3], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} HQM_PCIE_PASID_CAP_PASID_CONTROL_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_SCIOV_CAP_DVSEC_CAP_ID_IIO_HQM_PF_REG supported on:                 */
/*      SPRA0 (0x10e00150)                                                      */
/*      SPRB0 (0x10e00150)                                                      */
/*      SPRC0 (0x10e00150)                                                      */
/*      SPRMCC (0x10e00150)                                                     */
/*      SPRUCC (0x10e00150)                                                     */
/* Register default value on SPRA0: 0x00000023                                  */
/* Register default value on SPRB0: 0x00000023                                  */
/* Register default value on SPRC0: 0x00000023                                  */
/* Register default value on SPRMCC: 0x00000023                                 */
/* Register default value on SPRUCC: 0x00000023                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SVSEC Capability ID Register
*/


#define HQM_PCIE_SCIOV_CAP_DVSEC_CAP_ID_IIO_HQM_PF_REG 0x21410150

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 cap_id : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000023*/

                            /*
                               Capability ID: Designated Vendor-Specific
                               Extended Capability (used to advertise SCIOV
                               support)
                            */

  } Bits;
  UINT16 Data;

} HQM_PCIE_SCIOV_CAP_DVSEC_CAP_ID_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_SCIOV_CAP_DVSEC_CAP_VERSION_NEXT_PTR_IIO_HQM_PF_REG supported on:   */
/*      SPRA0 (0x10e00152)                                                      */
/*      SPRB0 (0x10e00152)                                                      */
/*      SPRC0 (0x10e00152)                                                      */
/*      SPRMCC (0x10e00152)                                                     */
/*      SPRUCC (0x10e00152)                                                     */
/* Register default value on SPRA0: 0x00001681                                  */
/* Register default value on SPRB0: 0x00001681                                  */
/* Register default value on SPRC0: 0x00001681                                  */
/* Register default value on SPRMCC: 0x00001681                                 */
/* Register default value on SPRUCC: 0x00001681                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* DVSEC Capability Version and Next Capability Pointer Register
*/


#define HQM_PCIE_SCIOV_CAP_DVSEC_CAP_VERSION_NEXT_PTR_IIO_HQM_PF_REG 0x21410152

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 dvseccv : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000001*/

                            /* Capability Version */
    UINT16 cap_ptr : 12;

                            /* Bits[15:4], Access Type=RO, default=0x00000168*/

                            /* Next Capability Pointer (AER Capabilties) */

  } Bits;
  UINT16 Data;

} HQM_PCIE_SCIOV_CAP_DVSEC_CAP_VERSION_NEXT_PTR_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_SCIOV_CAP_DVSEC_HDR1_IIO_HQM_PF_REG supported on:                   */
/*      SPRA0 (0x20e00154)                                                      */
/*      SPRB0 (0x20e00154)                                                      */
/*      SPRC0 (0x20e00154)                                                      */
/*      SPRMCC (0x20e00154)                                                     */
/*      SPRUCC (0x20e00154)                                                     */
/* Register default value on SPRA0: 0x01808086                                  */
/* Register default value on SPRB0: 0x01808086                                  */
/* Register default value on SPRC0: 0x01808086                                  */
/* Register default value on SPRMCC: 0x01808086                                 */
/* Register default value on SPRUCC: 0x01808086                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* DVSEC Header 1 Register 
*/


#define HQM_PCIE_SCIOV_CAP_DVSEC_HDR1_IIO_HQM_PF_REG 0x21420154

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dvsec_vendor_id : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00008086*/

                            /* DVSEC Vendor ID */
    UINT32 dvsec_revision : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000000*/

                            /* DVSEC Revision */
    UINT32 dvsec_length : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000018*/

                            /*
                               DVSEC Length - Number of bytes in the entire
                               DVSEC feature, including the PCI Express
                               Extended Capability header, DVSEC Header 1,
                               DVSEC Header 2, and DVSEC Vendor-Specific
                               registers
                            */

  } Bits;
  UINT32 Data;

} HQM_PCIE_SCIOV_CAP_DVSEC_HDR1_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_SCIOV_CAP_DVSEC_HDR2_IIO_HQM_PF_REG supported on:                   */
/*      SPRA0 (0x10e00158)                                                      */
/*      SPRB0 (0x10e00158)                                                      */
/*      SPRC0 (0x10e00158)                                                      */
/*      SPRMCC (0x10e00158)                                                     */
/*      SPRUCC (0x10e00158)                                                     */
/* Register default value on SPRA0: 0x00000005                                  */
/* Register default value on SPRB0: 0x00000005                                  */
/* Register default value on SPRC0: 0x00000005                                  */
/* Register default value on SPRMCC: 0x00000005                                 */
/* Register default value on SPRUCC: 0x00000005                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* DVSEC Header 2 Register
*/


#define HQM_PCIE_SCIOV_CAP_DVSEC_HDR2_IIO_HQM_PF_REG 0x21410158

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 dvsec_id : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000005*/

                            /*
                               DVSEC ID: Designated Vendor-Specific Extended
                               Capability ID register - SCIOV ID
                            */

  } Bits;
  UINT16 Data;

} HQM_PCIE_SCIOV_CAP_DVSEC_HDR2_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_SCIOV_CAP_SCIOV_CAP_IIO_HQM_PF_REG supported on:                    */
/*      SPRA0 (0x10e0015a)                                                      */
/*      SPRB0 (0x10e0015a)                                                      */
/*      SPRC0 (0x10e0015a)                                                      */
/*      SPRMCC (0x10e0015a)                                                     */
/*      SPRUCC (0x10e0015a)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SCIOV Capabilities Register 
*/


#define HQM_PCIE_SCIOV_CAP_SCIOV_CAP_IIO_HQM_PF_REG 0x2141015A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 reserved0 : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Function Dependency Link. */
    UINT16 ais_dependent : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /* AIs Dependent */
    UINT16 homogen_func : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /* Homogeneous Function */
    UINT16 reserved2 : 6;

                            /* Bits[15:10], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT16 Data;

} HQM_PCIE_SCIOV_CAP_SCIOV_CAP_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_SCIOV_CAP_SCIOV_SUPP_PGSZ_IIO_HQM_PF_REG supported on:              */
/*      SPRA0 (0x20e0015c)                                                      */
/*      SPRB0 (0x20e0015c)                                                      */
/*      SPRC0 (0x20e0015c)                                                      */
/*      SPRMCC (0x20e0015c)                                                     */
/*      SPRUCC (0x20e0015c)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SCIOV Supported Page Size Register
*/


#define HQM_PCIE_SCIOV_CAP_SCIOV_SUPP_PGSZ_IIO_HQM_PF_REG 0x2142015C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 supp_pgsz : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000001*/

                            /* SCIOV Supported Page Size. */
    UINT32 reserved : 31;

                            /* Bits[31:1], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} HQM_PCIE_SCIOV_CAP_SCIOV_SUPP_PGSZ_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_SCIOV_CAP_SCIOV_SYS_PGSZ_IIO_HQM_PF_REG supported on:               */
/*      SPRA0 (0x20e00160)                                                      */
/*      SPRB0 (0x20e00160)                                                      */
/*      SPRC0 (0x20e00160)                                                      */
/*      SPRMCC (0x20e00160)                                                     */
/*      SPRUCC (0x20e00160)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SCIOV System Page Size Register
*/


#define HQM_PCIE_SCIOV_CAP_SCIOV_SYS_PGSZ_IIO_HQM_PF_REG 0x21420160

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sys_pgsz : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* SCIOV Supported Page Size. */
    UINT32 reserved : 31;

                            /* Bits[31:1], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} HQM_PCIE_SCIOV_CAP_SCIOV_SYS_PGSZ_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_SCIOV_CAP_SCIOV_IMS_IIO_HQM_PF_REG supported on:                    */
/*      SPRA0 (0x20e00164)                                                      */
/*      SPRB0 (0x20e00164)                                                      */
/*      SPRC0 (0x20e00164)                                                      */
/*      SPRMCC (0x20e00164)                                                     */
/*      SPRUCC (0x20e00164)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SCIOV IMS Register
*/


#define HQM_PCIE_SCIOV_CAP_SCIOV_IMS_IIO_HQM_PF_REG 0x21420164

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ims_supp : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000001*/

                            /* SCIOV IMS Support. */
    UINT32 reserved : 31;

                            /* Bits[31:1], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} HQM_PCIE_SCIOV_CAP_SCIOV_IMS_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_AER_CAP_AER_CAP_ID_IIO_HQM_PF_REG supported on:                     */
/*      SPRA0 (0x10e00168)                                                      */
/*      SPRB0 (0x10e00168)                                                      */
/*      SPRC0 (0x10e00168)                                                      */
/*      SPRMCC (0x10e00168)                                                     */
/*      SPRUCC (0x10e00168)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Capability ID Register
*/


#define HQM_PCIE_AER_CAP_AER_CAP_ID_IIO_HQM_PF_REG 0x21410168

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 pcieaercid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000001*/

                            /*
                               Advanced Error Capability ID: PCI Express
                               Extended Capability ID indicating Advanced Error
                               Reporting Capability.
                            */

  } Bits;
  UINT16 Data;

} HQM_PCIE_AER_CAP_AER_CAP_ID_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_AER_CAP_AER_CAP_VERSION_NEXT_PTR_IIO_HQM_PF_REG supported on:       */
/*      SPRA0 (0x10e0016a)                                                      */
/*      SPRB0 (0x10e0016a)                                                      */
/*      SPRC0 (0x10e0016a)                                                      */
/*      SPRMCC (0x10e0016a)                                                     */
/*      SPRUCC (0x10e0016a)                                                     */
/* Register default value on SPRA0: 0x00000002                                  */
/* Register default value on SPRB0: 0x00000002                                  */
/* Register default value on SPRC0: 0x00000002                                  */
/* Register default value on SPRMCC: 0x00000002                                 */
/* Register default value on SPRUCC: 0x00000002                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Capability ID Register
*/


#define HQM_PCIE_AER_CAP_AER_CAP_VERSION_NEXT_PTR_IIO_HQM_PF_REG 0x2141016A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 pcieaercvn : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000002*/

                            /*
                               Advanced Error Capability Version Number: PCI
                               Express Advanced Error Reporting Extended
                               Capability Version Number.
                            */
    UINT16 cap_ptr : 12;

                            /* Bits[15:4], Access Type=RO/V, default=0x00000000*/

                            /*
                               Next Capability Pointer (This is the last
                               extended capability structure)
                            */

  } Bits;
  UINT16 Data;

} HQM_PCIE_AER_CAP_AER_CAP_VERSION_NEXT_PTR_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_AER_CAP_AER_CAP_UNCORR_ERR_STATUS_IIO_HQM_PF_REG supported on:      */
/*      SPRA0 (0x20e0016c)                                                      */
/*      SPRB0 (0x20e0016c)                                                      */
/*      SPRC0 (0x20e0016c)                                                      */
/*      SPRMCC (0x20e0016c)                                                     */
/*      SPRUCC (0x20e0016c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Uncorrectable Error Status Register
*/


#define HQM_PCIE_AER_CAP_AER_CAP_UNCORR_ERR_STATUS_IIO_HQM_PF_REG 0x2142016C

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

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Completer Abort: As a completer, set whenever an
                               internal agent signals a data abort. The header
                               is logged.
                            */
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
                               buffers overflow.Not applicable for IOSF.
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
    UINT32 reserved3 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /* Reserved3 */
    UINT32 ieunc : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Uncorrectable Internal Error Status: */
    UINT32 reserved0 : 9;

                            /* Bits[31:23], Access Type=RO, default=0x00000000*/

                            /* Reserved0: */

  } Bits;
  UINT32 Data;

} HQM_PCIE_AER_CAP_AER_CAP_UNCORR_ERR_STATUS_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_AER_CAP_AER_CAP_UNCORR_ERR_MASK_IIO_HQM_PF_REG supported on:        */
/*      SPRA0 (0x20e00170)                                                      */
/*      SPRB0 (0x20e00170)                                                      */
/*      SPRC0 (0x20e00170)                                                      */
/*      SPRMCC (0x20e00170)                                                     */
/*      SPRUCC (0x20e00170)                                                     */
/* Register default value on SPRA0: 0x00400000                                  */
/* Register default value on SPRB0: 0x00400000                                  */
/* Register default value on SPRC0: 0x00400000                                  */
/* Register default value on SPRMCC: 0x00400000                                 */
/* Register default value on SPRUCC: 0x00400000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Uncorrectable Error Mask Register
*/


#define HQM_PCIE_AER_CAP_AER_CAP_UNCORR_ERR_MASK_IIO_HQM_PF_REG 0x21420170

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

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               Completer Abort Error Mask : When 1 error
                               reporting is masked.
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
    UINT32 reserved3 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /* Reserved3 */
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

} HQM_PCIE_AER_CAP_AER_CAP_UNCORR_ERR_MASK_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_AER_CAP_AER_CAP_UNCORR_ERR_SEV_IIO_HQM_PF_REG supported on:         */
/*      SPRA0 (0x20e00174)                                                      */
/*      SPRB0 (0x20e00174)                                                      */
/*      SPRC0 (0x20e00174)                                                      */
/*      SPRMCC (0x20e00174)                                                     */
/*      SPRUCC (0x20e00174)                                                     */
/* Register default value on SPRA0: 0x00440000                                  */
/* Register default value on SPRB0: 0x00440000                                  */
/* Register default value on SPRC0: 0x00440000                                  */
/* Register default value on SPRMCC: 0x00440000                                 */
/* Register default value on SPRUCC: 0x00440000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Uncorrectable Error Severity Register
*/


#define HQM_PCIE_AER_CAP_AER_CAP_UNCORR_ERR_SEV_IIO_HQM_PF_REG 0x21420174

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

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /* completer abort severity: */
    UINT32 ec : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* unexpected completion severity: */
    UINT32 ro : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

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
    UINT32 reserved3 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /* Reserved3 */
    UINT32 ieunc : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000001*/

                            /* Uncorrectable Internal Error status sevirity */
    UINT32 reserved0 : 9;

                            /* Bits[31:23], Access Type=RO, default=0x00000000*/

                            /* reserved0 */

  } Bits;
  UINT32 Data;

} HQM_PCIE_AER_CAP_AER_CAP_UNCORR_ERR_SEV_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_AER_CAP_AER_CAP_CORR_ERR_STATUS_IIO_HQM_PF_REG supported on:        */
/*      SPRA0 (0x20e00178)                                                      */
/*      SPRB0 (0x20e00178)                                                      */
/*      SPRC0 (0x20e00178)                                                      */
/*      SPRMCC (0x20e00178)                                                     */
/*      SPRUCC (0x20e00178)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Correctable Error Register
*/


#define HQM_PCIE_AER_CAP_AER_CAP_CORR_ERR_STATUS_IIO_HQM_PF_REG 0x21420178

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
                               supported since HQM is an IOSF device.
                            */
    UINT32 reserved2 : 5;

                            /* Bits[5:1], Access Type=RO, default=0x00000000*/

                            /* Reserved2 */
    UINT32 dlpe : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Bad TLP Status : Sets this bit on CRC errors or
                               sequence number out of range on TLP.Not
                               supported since HQM is an IOSF device.
                            */
    UINT32 bdllps : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Bad DLLP Status : Sets this bit on CRC errors on
                               DLLP.Not supported since HQM is an IOSF device.
                            */
    UINT32 rnrs : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               REPLAY NUM Rollover Status : Set whenever the
                               replay number rolls over from 11 to 00.Not
                               supported since HQM is an IOSF device.
                            */
    UINT32 reserved1 : 3;

                            /* Bits[11:9], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */
    UINT32 rtts : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Replay Timer Timeout Status : Set whenever a
                               replay timer timeout occurs.Not supported since
                               HQM is an IOSF device.
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

} HQM_PCIE_AER_CAP_AER_CAP_CORR_ERR_STATUS_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_AER_CAP_AER_CAP_CORR_ERR_MASK_IIO_HQM_PF_REG supported on:          */
/*      SPRA0 (0x20e0017c)                                                      */
/*      SPRB0 (0x20e0017c)                                                      */
/*      SPRC0 (0x20e0017c)                                                      */
/*      SPRMCC (0x20e0017c)                                                     */
/*      SPRUCC (0x20e0017c)                                                     */
/* Register default value on SPRA0: 0x00006000                                  */
/* Register default value on SPRB0: 0x00006000                                  */
/* Register default value on SPRC0: 0x00006000                                  */
/* Register default value on SPRMCC: 0x00006000                                 */
/* Register default value on SPRUCC: 0x00006000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Correctable Error Mask Register
*/


#define HQM_PCIE_AER_CAP_AER_CAP_CORR_ERR_MASK_IIO_HQM_PF_REG 0x2142017C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 res : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Receiver Error Mask:Not supported since HQM is
                               an IOSF device
                            */
    UINT32 reserved2 : 5;

                            /* Bits[5:1], Access Type=RO, default=0x00000000*/

                            /* Reserved2 */
    UINT32 dlpe : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               Bad TLP Mask:Not supported since HQM is an IOSF
                               device
                            */
    UINT32 bdllps : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               Bad DLLP Mask:Not supported since HQM is an IOSF
                               device
                            */
    UINT32 rnrs : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               REPLAY NUM Rollover Mask:Not supported since HQM
                               is an IOSF device
                            */
    UINT32 reserved1 : 3;

                            /* Bits[11:9], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */
    UINT32 rtts : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Replay Timer Timeout MaskNot supported since HQM
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

} HQM_PCIE_AER_CAP_AER_CAP_CORR_ERR_MASK_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_AER_CAP_AER_CAP_CONTROL_IIO_HQM_PF_REG supported on:                */
/*      SPRA0 (0x20e00180)                                                      */
/*      SPRB0 (0x20e00180)                                                      */
/*      SPRC0 (0x20e00180)                                                      */
/*      SPRMCC (0x20e00180)                                                     */
/*      SPRUCC (0x20e00180)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Control and Capability Register
*/


#define HQM_PCIE_AER_CAP_AER_CAP_CONTROL_IIO_HQM_PF_REG 0x21420180

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tfep : 5;

                            /* Bits[4:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               The First Error Pointer: Identifies the bit
                               position of the first error reported in theThis
                               register will not update until all bits in the
                               ERRUNC STS register are cleared.
                            */
    UINT32 ecrcgc : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               ECRC Generation Capable : Indicates the EP is
                               not capable of generating ECRC.
                            */
    UINT32 ecrcge : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               ECRC Generation Enable : We do not support ECRC
                               generation, so this bit is hard-wired to 0.
                            */
    UINT32 ecrccc : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               ECRC Check Capable : Indicates EP is not capable
                               of checking ECRC.
                            */
    UINT32 ecrcce : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               ECRC Check Enable : We do not support ECRC
                               checking, so this bit is hard-wired to 0.
                            */
    UINT32 multhdrrc : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Multiple Header Recording Capable : We do not
                               support recording multiple error headers, so
                               this bit is hard-wired to 0.
                            */
    UINT32 reserved : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /* Reserved0 */
    UINT32 tlppflogp : 1;

                            /* Bits[11:11], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               TLP Prefix Log Present: If set, indicates the
                               presence of a valid TLP Prefix Log entry.
                            */
    UINT32 reserved1 : 20;

                            /* Bits[31:12], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */

  } Bits;
  UINT32 Data;

} HQM_PCIE_AER_CAP_AER_CAP_CONTROL_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_AER_CAP_AER_CAP_HEADER_LOG_0_IIO_HQM_PF_REG supported on:           */
/*      SPRA0 (0x20e00184)                                                      */
/*      SPRB0 (0x20e00184)                                                      */
/*      SPRC0 (0x20e00184)                                                      */
/*      SPRMCC (0x20e00184)                                                     */
/*      SPRUCC (0x20e00184)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Header Log 0 Register
*/


#define HQM_PCIE_AER_CAP_AER_CAP_HEADER_LOG_0_IIO_HQM_PF_REG 0x21420184

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
                               log again.
                            */

  } Bits;
  UINT32 Data;

} HQM_PCIE_AER_CAP_AER_CAP_HEADER_LOG_0_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_AER_CAP_AER_CAP_HEADER_LOG_1_IIO_HQM_PF_REG supported on:           */
/*      SPRA0 (0x20e00188)                                                      */
/*      SPRB0 (0x20e00188)                                                      */
/*      SPRC0 (0x20e00188)                                                      */
/*      SPRMCC (0x20e00188)                                                     */
/*      SPRUCC (0x20e00188)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Header Log 1 Register
*/


#define HQM_PCIE_AER_CAP_AER_CAP_HEADER_LOG_1_IIO_HQM_PF_REG 0x21420188

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
                               log again.
                            */

  } Bits;
  UINT32 Data;

} HQM_PCIE_AER_CAP_AER_CAP_HEADER_LOG_1_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_AER_CAP_AER_CAP_HEADER_LOG_2_IIO_HQM_PF_REG supported on:           */
/*      SPRA0 (0x20e0018c)                                                      */
/*      SPRB0 (0x20e0018c)                                                      */
/*      SPRC0 (0x20e0018c)                                                      */
/*      SPRMCC (0x20e0018c)                                                     */
/*      SPRUCC (0x20e0018c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Header Log 2 Register
*/


#define HQM_PCIE_AER_CAP_AER_CAP_HEADER_LOG_2_IIO_HQM_PF_REG 0x2142018C

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
                               log again.
                            */

  } Bits;
  UINT32 Data;

} HQM_PCIE_AER_CAP_AER_CAP_HEADER_LOG_2_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_AER_CAP_AER_CAP_HEADER_LOG_3_IIO_HQM_PF_REG supported on:           */
/*      SPRA0 (0x20e00190)                                                      */
/*      SPRB0 (0x20e00190)                                                      */
/*      SPRC0 (0x20e00190)                                                      */
/*      SPRMCC (0x20e00190)                                                     */
/*      SPRUCC (0x20e00190)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Header Log 3 Register
*/


#define HQM_PCIE_AER_CAP_AER_CAP_HEADER_LOG_3_IIO_HQM_PF_REG 0x21420190

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdrlogdw3 : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               4th DWord of the Header for the PCI Express
                               packet in error (HDRLOGDW3): Once an error is
                               logged in this register, it remains locked for
                               further error logging until the time the
                               software clears the status bit that cause the
                               header log i.e. the error pointer is rearmed to
                               log again.
                            */

  } Bits;
  UINT32 Data;

} HQM_PCIE_AER_CAP_AER_CAP_HEADER_LOG_3_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_AER_CAP_AER_CAP_ROOT_ERROR_COMMAND_IIO_HQM_PF_REG supported on:     */
/*      SPRA0 (0x20e00194)                                                      */
/*      SPRB0 (0x20e00194)                                                      */
/*      SPRC0 (0x20e00194)                                                      */
/*      SPRMCC (0x20e00194)                                                     */
/*      SPRUCC (0x20e00194)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Root Error Command Register
*/


#define HQM_PCIE_AER_CAP_AER_CAP_ROOT_ERROR_COMMAND_IIO_HQM_PF_REG 0x21420194

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cere : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Correctable Error Reporting Enable - When Set,
                               this bit enables the generation of an interrupt
                               when a correctable error is reported by any of
                               the Functions in the Hierarchy Domain associated
                               with this Root Port.
                            */
    UINT32 nere : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Non-Fatal Error Reporting Enable - When Set,
                               this bit enables the generation of an interrupt
                               when a non-fatal error is reported by any of the
                               Functions in the Hierarchy Domain associated
                               with this Root Port.
                            */
    UINT32 fere : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Fatal Error Reporting Enable - When Set, this
                               bit enables the generation of an interrupt when
                               a fatal error is reported by any of the
                               Functions in the Hierarchy Domain associated
                               with this Root Port.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} HQM_PCIE_AER_CAP_AER_CAP_ROOT_ERROR_COMMAND_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_AER_CAP_AER_CAP_ROOT_ERROR_STATUS_IIO_HQM_PF_REG supported on:      */
/*      SPRA0 (0x20e00198)                                                      */
/*      SPRB0 (0x20e00198)                                                      */
/*      SPRC0 (0x20e00198)                                                      */
/*      SPRMCC (0x20e00198)                                                     */
/*      SPRUCC (0x20e00198)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Root Error Status Register
*/


#define HQM_PCIE_AER_CAP_AER_CAP_ROOT_ERROR_STATUS_IIO_HQM_PF_REG 0x21420198

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cer : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Correctable Error Received - Set when a ERR_COR
                               error message is received
                            */
    UINT32 mcer : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Multiple Correctable Errors Received - Set when
                               a ERR_COR error message is received and CER is
                               already set
                            */
    UINT32 fner : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Fatal or Non-Fatal Error Received - Set when a
                               ERR_FATAL or ERR_NONFATAL error message is
                               received
                            */
    UINT32 mfner : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Multiple Fatal or Non-Fatal Errors Received -
                               Set when a ERR_FATAL or ERR_NONFATAL error
                               message is received and FNER is already set
                            */
    UINT32 fuf : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Set when the first uncorrectable error message
                               received is for a fatal error
                            */
    UINT32 ner : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Non-Fatal Error Received - Set when a
                               ERR_NONFATAL error message is received
                            */
    UINT32 fer : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Fatal Error Received - Set when a ERR_FATAL
                               error message is received
                            */
    UINT32 rsvd : 20;

                            /* Bits[26:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 aeimn : 5;

                            /* Bits[31:27], Access Type=RO, default=0x00000000*/

                            /*
                               Advanced Error Interrupt Message Number - This
                               register indicates which MSI/MSI-X vector is
                               used for the interrupt message generated in
                               association with any of the status bits of this
                               capability
                            */

  } Bits;
  UINT32 Data;

} HQM_PCIE_AER_CAP_AER_CAP_ROOT_ERROR_STATUS_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_AER_CAP_AER_CAP_ERROR_SOURCE_IDENT_IIO_HQM_PF_REG supported on:     */
/*      SPRA0 (0x20e0019c)                                                      */
/*      SPRB0 (0x20e0019c)                                                      */
/*      SPRC0 (0x20e0019c)                                                      */
/*      SPRMCC (0x20e0019c)                                                     */
/*      SPRUCC (0x20e0019c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Error Source Identification Register
*/


#define HQM_PCIE_AER_CAP_AER_CAP_ERROR_SOURCE_IDENT_IIO_HQM_PF_REG 0x2142019C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 csid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /*
                               ERR_COR Source Identification - Loaded with the
                               Requester ID indicated in the received ERR_COR
                               Message when the ERR_COR Received bit is not
                               already set
                            */
    UINT32 fnsid : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*
                               ERR_FATAL/NONFATAL Source Identification -
                               Loaded with the Requester ID indicated in the
                               received ERR_FATAL or ERR_NONFATAL Message when
                               the ERR_FATAL/NONFATAL Received bit is not
                               already set
                            */

  } Bits;
  UINT32 Data;

} HQM_PCIE_AER_CAP_AER_CAP_ERROR_SOURCE_IDENT_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_0_IIO_HQM_PF_REG supported on:       */
/*      SPRA0 (0x20e001a0)                                                      */
/*      SPRB0 (0x20e001a0)                                                      */
/*      SPRC0 (0x20e001a0)                                                      */
/*      SPRMCC (0x20e001a0)                                                     */
/*      SPRUCC (0x20e001a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER TLP Prefix Log 0 Register
*/


#define HQM_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_0_IIO_HQM_PF_REG 0x214201A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tlppflog0 : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               1st TLP Prefix for the PCI Express packet in
                               error (TLPPFLOG0): Once an error is logged in
                               this register, it remains locked for further
                               error logging until the time the software clears
                               the status bit that cause the header log i.e.
                               the error pointer is rearmed to log again.
                            */

  } Bits;
  UINT32 Data;

} HQM_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_0_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_1_IIO_HQM_PF_REG supported on:       */
/*      SPRA0 (0x20e001a4)                                                      */
/*      SPRB0 (0x20e001a4)                                                      */
/*      SPRC0 (0x20e001a4)                                                      */
/*      SPRMCC (0x20e001a4)                                                     */
/*      SPRUCC (0x20e001a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER TLP Prefix Log 1 Register
*/


#define HQM_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_1_IIO_HQM_PF_REG 0x214201A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tlppflog1 : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               2nd TLP Prefix for the PCI Express packet in
                               error (TLPPFLOG1): Once an error is logged in
                               this register, it remains locked for further
                               error logging until the time the software clears
                               the status bit that cause the header log i.e.
                               the error pointer is rearmed to log again.
                            */

  } Bits;
  UINT32 Data;

} HQM_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_1_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_2_IIO_HQM_PF_REG supported on:       */
/*      SPRA0 (0x20e001a8)                                                      */
/*      SPRB0 (0x20e001a8)                                                      */
/*      SPRC0 (0x20e001a8)                                                      */
/*      SPRMCC (0x20e001a8)                                                     */
/*      SPRUCC (0x20e001a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER TLP Prefix Log 2 Register
*/


#define HQM_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_2_IIO_HQM_PF_REG 0x214201A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tlppflog2 : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               3rd TLP Prefix for the PCI Express packet in
                               error (TLPPFLOG2): Once an error is logged in
                               this register, it remains locked for further
                               error logging until the time the software clears
                               the status bit that cause the header log i.e.
                               the error pointer is rearmed to log again.
                            */

  } Bits;
  UINT32 Data;

} HQM_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_2_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_3_IIO_HQM_PF_REG supported on:       */
/*      SPRA0 (0x20e001ac)                                                      */
/*      SPRB0 (0x20e001ac)                                                      */
/*      SPRC0 (0x20e001ac)                                                      */
/*      SPRMCC (0x20e001ac)                                                     */
/*      SPRUCC (0x20e001ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG     */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_pf_cfg_i/hqm_pf_cfg_CFG       */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_pf_cfg_i/hqm_pf_cfg_CFG    */
/* Struct generated from SPRA0 BDF: 14_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER TLP Prefix Log 3 Register
*/


#define HQM_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_3_IIO_HQM_PF_REG 0x214201AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tlppflog3 : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               4th TLP Prefix for the PCI Express packet in
                               error (TLPPFLOG3): Once an error is logged in
                               this register, it remains locked for further
                               error logging until the time the software clears
                               the status bit that cause the header log i.e.
                               the error pointer is rearmed to log again.
                            */

  } Bits;
  UINT32 Data;

} HQM_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_3_IIO_HQM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */
#endif /* _IIO_HQM_PF_h */
