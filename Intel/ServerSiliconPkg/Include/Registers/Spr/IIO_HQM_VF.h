
/** @file
  IIO_HQM_VF.h

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


#ifndef _IIO_HQM_VF_h
#define _IIO_HQM_VF_h
#include <Base.h>

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_HDR_VENDOR_ID_IIO_HQM_VF_REG supported on:                       */
/*      SPRA0 (0x10e01000)                                                      */
/*      SPRB0 (0x10e01000)                                                      */
/*      SPRC0 (0x10e01000)                                                      */
/*      SPRMCC (0x10e01000)                                                     */
/*      SPRUCC (0x10e01000)                                                     */
/* Register default value on SPRA0: 0x0000FFFF                                  */
/* Register default value on SPRB0: 0x0000FFFF                                  */
/* Register default value on SPRC0: 0x0000FFFF                                  */
/* Register default value on SPRMCC: 0x0000FFFF                                 */
/* Register default value on SPRUCC: 0x0000FFFF                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Vendor Identificaion Register
*/


#define HQM_VF_PCIE_HDR_VENDOR_ID_IIO_HQM_VF_REG 0x21510000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 vid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000FFFF*/

                            /*
                               Vendor Identification: This register field
                               contains the PCI standard identification for
                               Intel, 8086h.
                            */

  } Bits;
  UINT16 Data;

} HQM_VF_PCIE_HDR_VENDOR_ID_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_HDR_DEVICE_ID_IIO_HQM_VF_REG supported on:                       */
/*      SPRA0 (0x10e01002)                                                      */
/*      SPRB0 (0x10e01002)                                                      */
/*      SPRC0 (0x10e01002)                                                      */
/*      SPRMCC (0x10e01002)                                                     */
/*      SPRUCC (0x10e01002)                                                     */
/* Register default value on SPRA0: 0x0000FFFF                                  */
/* Register default value on SPRB0: 0x0000FFFF                                  */
/* Register default value on SPRC0: 0x0000FFFF                                  */
/* Register default value on SPRMCC: 0x0000FFFF                                 */
/* Register default value on SPRUCC: 0x0000FFFF                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Device Identification Register
*/


#define HQM_VF_PCIE_HDR_DEVICE_ID_IIO_HQM_VF_REG 0x21510002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 did : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000FFFF*/

                            /*
                               Device Identification Number - is always 0xffff
                               for a virtual function header.
                            */

  } Bits;
  UINT16 Data;

} HQM_VF_PCIE_HDR_DEVICE_ID_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_HDR_DEVICE_COMMAND_IIO_HQM_VF_REG supported on:                  */
/*      SPRA0 (0x10e01004)                                                      */
/*      SPRB0 (0x10e01004)                                                      */
/*      SPRC0 (0x10e01004)                                                      */
/*      SPRMCC (0x10e01004)                                                     */
/*      SPRUCC (0x10e01004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Device Command Register
*/


#define HQM_VF_PCIE_HDR_DEVICE_COMMAND_IIO_HQM_VF_REG 0x21510004

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

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

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

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

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

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

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

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

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

} HQM_VF_PCIE_HDR_DEVICE_COMMAND_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_HDR_DEVICE_STATUS_IIO_HQM_VF_REG supported on:                   */
/*      SPRA0 (0x10e01006)                                                      */
/*      SPRB0 (0x10e01006)                                                      */
/*      SPRC0 (0x10e01006)                                                      */
/*      SPRMCC (0x10e01006)                                                     */
/*      SPRUCC (0x10e01006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* device status register
*/


#define HQM_VF_PCIE_HDR_DEVICE_STATUS_IIO_HQM_VF_REG 0x21510006

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
                               Interrupt Status: Revision 1.1 requires that
                               this field is hardwired to 0b for all VFs.
                               Single Root I/O Virtualization and Sharing
                               Specification. This bit does not apply to VFs.
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

} HQM_VF_PCIE_HDR_DEVICE_STATUS_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_HDR_REVISION_ID_CLASS_CODE_IIO_HQM_VF_REG supported on:          */
/*      SPRA0 (0x20e01008)                                                      */
/*      SPRB0 (0x20e01008)                                                      */
/*      SPRC0 (0x20e01008)                                                      */
/*      SPRMCC (0x20e01008)                                                     */
/*      SPRUCC (0x20e01008)                                                     */
/* Register default value on SPRA0: 0x0B400000                                  */
/* Register default value on SPRB0: 0x0B400000                                  */
/* Register default value on SPRC0: 0x0B400000                                  */
/* Register default value on SPRMCC: 0x0B400000                                 */
/* Register default value on SPRUCC: 0x0B400000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Revision ID register and class code Register
*/


#define HQM_VF_PCIE_HDR_REVISION_ID_CLASS_CODE_IIO_HQM_VF_REG 0x21520008

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

} HQM_VF_PCIE_HDR_REVISION_ID_CLASS_CODE_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_HDR_CACHE_LINE_SIZE_IIO_HQM_VF_REG supported on:                 */
/*      SPRA0 (0xe0100c)                                                        */
/*      SPRB0 (0xe0100c)                                                        */
/*      SPRC0 (0xe0100c)                                                        */
/*      SPRMCC (0xe0100c)                                                       */
/*      SPRUCC (0xe0100c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Cache Line size Register
*/


#define HQM_VF_PCIE_HDR_CACHE_LINE_SIZE_IIO_HQM_VF_REG 0x2150000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 cls : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Cache Line Size: Not used by PCI Express
                               Endpoint. Left for compatility reason only.
                            */

  } Bits;
  UINT8 Data;

} HQM_VF_PCIE_HDR_CACHE_LINE_SIZE_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_HDR_HEADER_TYPE_IIO_HQM_VF_REG supported on:                     */
/*      SPRA0 (0xe0100e)                                                        */
/*      SPRB0 (0xe0100e)                                                        */
/*      SPRC0 (0xe0100e)                                                        */
/*      SPRMCC (0xe0100e)                                                       */
/*      SPRUCC (0xe0100e)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header type Register
*/


#define HQM_VF_PCIE_HDR_HEADER_TYPE_IIO_HQM_VF_REG 0x2150000E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 hdr : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               PCI Header Type: The header type of the EP
                               device.00h (Bit 7 = 0b) indicates a single-
                               function device with standard header layout.
                            */

  } Bits;
  UINT8 Data;

} HQM_VF_PCIE_HDR_HEADER_TYPE_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_HDR_SUBSYSTEM_VENDOR_ID_IIO_HQM_VF_REG supported on:             */
/*      SPRA0 (0x10e0102c)                                                      */
/*      SPRB0 (0x10e0102c)                                                      */
/*      SPRC0 (0x10e0102c)                                                      */
/*      SPRMCC (0x10e0102c)                                                     */
/*      SPRUCC (0x10e0102c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VF Subsystem Vendor ID Register
*/


#define HQM_VF_PCIE_HDR_SUBSYSTEM_VENDOR_ID_IIO_HQM_VF_REG 0x2151002C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 svid : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00008086*/

                            /*
                               Subsystem Vendor ID: This field is hardwired to
                               ID assigned to Intel.
                            */

  } Bits;
  UINT16 Data;

} HQM_VF_PCIE_HDR_SUBSYSTEM_VENDOR_ID_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_HDR_SUBSYSTEM_ID_IIO_HQM_VF_REG supported on:                    */
/*      SPRA0 (0x10e0102e)                                                      */
/*      SPRB0 (0x10e0102e)                                                      */
/*      SPRC0 (0x10e0102e)                                                      */
/*      SPRMCC (0x10e0102e)                                                     */
/*      SPRUCC (0x10e0102e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VF Subsystem ID Register
*/


#define HQM_VF_PCIE_HDR_SUBSYSTEM_ID_IIO_HQM_VF_REG 0x2151002E

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

} HQM_VF_PCIE_HDR_SUBSYSTEM_ID_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_HDR_CAP_PTR_IIO_HQM_VF_REG supported on:                         */
/*      SPRA0 (0xe01034)                                                        */
/*      SPRB0 (0xe01034)                                                        */
/*      SPRC0 (0xe01034)                                                        */
/*      SPRMCC (0xe01034)                                                       */
/*      SPRUCC (0xe01034)                                                       */
/* Register default value on SPRA0: 0x0000006C                                  */
/* Register default value on SPRB0: 0x0000006C                                  */
/* Register default value on SPRC0: 0x0000006C                                  */
/* Register default value on SPRMCC: 0x0000006C                                 */
/* Register default value on SPRUCC: 0x0000006C                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VF Capabilities Pointer Register
*/


#define HQM_VF_PCIE_HDR_CAP_PTR_IIO_HQM_VF_REG 0x21500034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 cp : 8;

                            /* Bits[7:0], Access Type=RO, default=0x0000006C*/

                            /* Capability Pointer (MSI Capabilities) */

  } Bits;
  UINT8 Data;

} HQM_VF_PCIE_HDR_CAP_PTR_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_HDR_INT_LINE_IIO_HQM_VF_REG supported on:                        */
/*      SPRA0 (0xe0103c)                                                        */
/*      SPRB0 (0xe0103c)                                                        */
/*      SPRC0 (0xe0103c)                                                        */
/*      SPRMCC (0xe0103c)                                                       */
/*      SPRUCC (0xe0103c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VF Interrupt Line Register
*/


#define HQM_VF_PCIE_HDR_INT_LINE_IIO_HQM_VF_REG 0x2150003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 irql : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Interrupt Line */

  } Bits;
  UINT8 Data;

} HQM_VF_PCIE_HDR_INT_LINE_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_HDR_INT_PIN_IIO_HQM_VF_REG supported on:                         */
/*      SPRA0 (0xe0103d)                                                        */
/*      SPRB0 (0xe0103d)                                                        */
/*      SPRC0 (0xe0103d)                                                        */
/*      SPRMCC (0xe0103d)                                                       */
/*      SPRUCC (0xe0103d)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VF Interrupt Pin Register
*/


#define HQM_VF_PCIE_HDR_INT_PIN_IIO_HQM_VF_REG 0x2150003D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 irqp : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Interrupt Pin */

  } Bits;
  UINT8 Data;

} HQM_VF_PCIE_HDR_INT_PIN_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_CAP_PCIE_CAP_ID_IIO_HQM_VF_REG supported on:                     */
/*      SPRA0 (0xe0106c)                                                        */
/*      SPRB0 (0xe0106c)                                                        */
/*      SPRC0 (0xe0106c)                                                        */
/*      SPRMCC (0xe0106c)                                                       */
/*      SPRUCC (0xe0106c)                                                       */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VF PCI Express Capability Register
*/


#define HQM_VF_PCIE_CAP_PCIE_CAP_ID_IIO_HQM_VF_REG 0x2150006C

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

} HQM_VF_PCIE_CAP_PCIE_CAP_ID_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_CAP_PCIE_CAP_NEXT_CAP_PTR_IIO_HQM_VF_REG supported on:           */
/*      SPRA0 (0xe0106d)                                                        */
/*      SPRB0 (0xe0106d)                                                        */
/*      SPRC0 (0xe0106d)                                                        */
/*      SPRMCC (0xe0106d)                                                       */
/*      SPRUCC (0xe0106d)                                                       */
/* Register default value on SPRA0: 0x000000B0                                  */
/* Register default value on SPRB0: 0x000000B0                                  */
/* Register default value on SPRC0: 0x000000B0                                  */
/* Register default value on SPRMCC: 0x000000B0                                 */
/* Register default value on SPRUCC: 0x000000B0                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VF PCI Express Next Capability Pointer Register
*/


#define HQM_VF_PCIE_CAP_PCIE_CAP_NEXT_CAP_PTR_IIO_HQM_VF_REG 0x2150006D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 cap_ptr : 8;

                            /* Bits[7:0], Access Type=RO, default=0x000000B0*/

                            /* Next Capability Pointer (MSI Capabilities) */

  } Bits;
  UINT8 Data;

} HQM_VF_PCIE_CAP_PCIE_CAP_NEXT_CAP_PTR_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_CAP_PCIE_CAP_IIO_HQM_VF_REG supported on:                        */
/*      SPRA0 (0x10e0106e)                                                      */
/*      SPRB0 (0x10e0106e)                                                      */
/*      SPRC0 (0x10e0106e)                                                      */
/*      SPRMCC (0x10e0106e)                                                     */
/*      SPRUCC (0x10e0106e)                                                     */
/* Register default value on SPRA0: 0x00000092                                  */
/* Register default value on SPRB0: 0x00000092                                  */
/* Register default value on SPRC0: 0x00000092                                  */
/* Register default value on SPRMCC: 0x00000092                                 */
/* Register default value on SPRUCC: 0x00000092                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VF PCI Express Capabilities Register
*/


#define HQM_VF_PCIE_CAP_PCIE_CAP_IIO_HQM_VF_REG 0x2151006E

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
                               number.EP is PCIe 3.0 Specification Compliant.
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

} HQM_VF_PCIE_CAP_PCIE_CAP_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_CAP_PCIE_CAP_DEVICE_CAP_IIO_HQM_VF_REG supported on:             */
/*      SPRA0 (0x20e01070)                                                      */
/*      SPRB0 (0x20e01070)                                                      */
/*      SPRC0 (0x20e01070)                                                      */
/*      SPRMCC (0x20e01070)                                                     */
/*      SPRUCC (0x20e01070)                                                     */
/* Register default value on SPRA0: 0x10008062                                  */
/* Register default value on SPRB0: 0x10008062                                  */
/* Register default value on SPRC0: 0x10008062                                  */
/* Register default value on SPRMCC: 0x10008062                                 */
/* Register default value on SPRUCC: 0x10008062                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VF PCI Express Device Capabilities Register
*/


#define HQM_VF_PCIE_CAP_PCIE_CAP_DEVICE_CAP_IIO_HQM_VF_REG 0x21520070

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

} HQM_VF_PCIE_CAP_PCIE_CAP_DEVICE_CAP_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_CAP_PCIE_CAP_DEVICE_CONTROL_IIO_HQM_VF_REG supported on:         */
/*      SPRA0 (0x10e01074)                                                      */
/*      SPRB0 (0x10e01074)                                                      */
/*      SPRC0 (0x10e01074)                                                      */
/*      SPRMCC (0x10e01074)                                                     */
/*      SPRUCC (0x10e01074)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VF PCI Express Device Control Register
*/


#define HQM_VF_PCIE_CAP_PCIE_CAP_DEVICE_CONTROL_IIO_HQM_VF_REG 0x21510074

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 cere_rsvdp : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Correctable Error Reporting Enable: This bit, in
                               conjunction with other bits, controls sending
                               ERR_COR Messages to the root port. When clear
                               disables sending of error messages.
                            */
    UINT16 nere_rsvdp : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Non-Fatal Error Reporting Enable: This bit, in
                               conjunction with other bits, controls sending
                               ERR_NONFATAL Messages to the root port. When
                               clear disables sending of error messages.
                            */
    UINT16 fere_rsvdp : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Fatal Error Reporting Enable: This bit, in
                               conjunction with other bits, controls sending
                               ERR_FATAL Messages to the root port. When clear
                               disables sending of error messages.
                            */
    UINT16 urro_rsvdp : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Unsupported Request Reporting Enable: This bit,
                               in conjunction with other bits, controls the
                               signaling of Unsupported Requests by sending
                               Error Messages to the root port. When clear it
                               disables sending of error messages.
                            */
    UINT16 ero_rsvdp : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Enable Relaxed Ordering: If this bit is set, EP
                               is permitted to set the Relaxed Ordering bit in
                               the Attributes field of transactions it
                               initiates.Note that seting this bit does not
                               cause the EP to set the RO on every transaction
                               it issues.
                            */
    UINT16 mps_rsvdp : 3;

                            /* Bits[7:5], Access Type=RO, default=0x00000000*/

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
    UINT16 etfe_rsvdp : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Extended Tag Field Enable: When set, this bit
                               enables a device to use an 8-bit Tag field as a
                               requester. Supports 256 outstanding requests.
                            */
    UINT16 pfe_rsvdp : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Phantom Functions Enable: When set, this bit
                               enables a device to use unclaimed functions as
                               Phantom Functions. Does not apply to EP.
                            */
    UINT16 apme_rsvdp : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Auxiliary (AUX) Power PM Enable: This bit when
                               set enables a device to draw AUX power
                               independent of PME AUX power. Does not apply to
                               EP
                            */
    UINT16 ens_rsvdp : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

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
    UINT16 mrs_rsvdp : 3;

                            /* Bits[14:12], Access Type=RO, default=0x00000000*/

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

} HQM_VF_PCIE_CAP_PCIE_CAP_DEVICE_CONTROL_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_CAP_PCIE_CAP_DEVICE_STATUS_IIO_HQM_VF_REG supported on:          */
/*      SPRA0 (0x10e01076)                                                      */
/*      SPRB0 (0x10e01076)                                                      */
/*      SPRC0 (0x10e01076)                                                      */
/*      SPRMCC (0x10e01076)                                                     */
/*      SPRUCC (0x10e01076)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VF PCI Express Device Status Register
*/


#define HQM_VF_PCIE_CAP_PCIE_CAP_DEVICE_STATUS_IIO_HQM_VF_REG 0x21510076

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

} HQM_VF_PCIE_CAP_PCIE_CAP_DEVICE_STATUS_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_CAP_PCIE_CAP_DEVICE_CAP_2_IIO_HQM_VF_REG supported on:           */
/*      SPRA0 (0x20e01090)                                                      */
/*      SPRB0 (0x20e01090)                                                      */
/*      SPRC0 (0x20e01090)                                                      */
/*      SPRMCC (0x20e01090)                                                     */
/*      SPRUCC (0x20e01090)                                                     */
/* Register default value on SPRA0: 0x00710010                                  */
/* Register default value on SPRB0: 0x00710010                                  */
/* Register default value on SPRC0: 0x00710010                                  */
/* Register default value on SPRMCC: 0x00710010                                 */
/* Register default value on SPRUCC: 0x00710010                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Device Capabilities 2 Register
*/


#define HQM_VF_PCIE_CAP_PCIE_CAP_DEVICE_CAP_2_IIO_HQM_VF_REG 0x21520090

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

} HQM_VF_PCIE_CAP_PCIE_CAP_DEVICE_CAP_2_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_CAP_PCIE_CAP_DEVICE_CONTROL_2_IIO_HQM_VF_REG supported on:       */
/*      SPRA0 (0x10e01094)                                                      */
/*      SPRB0 (0x10e01094)                                                      */
/*      SPRC0 (0x10e01094)                                                      */
/*      SPRMCC (0x10e01094)                                                     */
/*      SPRUCC (0x10e01094)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Device Control 2 Register
*/


#define HQM_VF_PCIE_CAP_PCIE_CAP_DEVICE_CONTROL_2_IIO_HQM_VF_REG 0x21510094

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ctov_rsvdp : 4;

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
    UINT16 ctodis_rsvdp : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

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
    UINT16 eido_rsvdp : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

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

} HQM_VF_PCIE_CAP_PCIE_CAP_DEVICE_CONTROL_2_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_MSI_CAP_MSI_CAP_ID_IIO_HQM_VF_REG supported on:                     */
/*      SPRA0 (0xe010b0)                                                        */
/*      SPRB0 (0xe010b0)                                                        */
/*      SPRC0 (0xe010b0)                                                        */
/*      SPRMCC (0xe010b0)                                                       */
/*      SPRUCC (0xe010b0)                                                       */
/* Register default value on SPRA0: 0x00000005                                  */
/* Register default value on SPRB0: 0x00000005                                  */
/* Register default value on SPRC0: 0x00000005                                  */
/* Register default value on SPRMCC: 0x00000005                                 */
/* Register default value on SPRUCC: 0x00000005                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Message Signalled Interrupt Capability ID Register
*/


#define HQM_PCIE_MSI_CAP_MSI_CAP_ID_IIO_HQM_VF_REG 0x215000B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 msi : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000005*/

                            /*
                               Capability ID: PCI SIG assigned capability
                               record ID (05h, MSI capability). 05h identifies
                               the EP as a device that is MSI capable.
                            */

  } Bits;
  UINT8 Data;

} HQM_PCIE_MSI_CAP_MSI_CAP_ID_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_MSI_CAP_MSI_CAP_NEXT_CAP_PTR_IIO_HQM_VF_REG supported on:           */
/*      SPRA0 (0xe010b1)                                                        */
/*      SPRB0 (0xe010b1)                                                        */
/*      SPRC0 (0xe010b1)                                                        */
/*      SPRMCC (0xe010b1)                                                       */
/*      SPRUCC (0xe010b1)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* MSI Next Capability Pointer Register
*/


#define HQM_PCIE_MSI_CAP_MSI_CAP_NEXT_CAP_PTR_IIO_HQM_VF_REG 0x215000B1

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

} HQM_PCIE_MSI_CAP_MSI_CAP_NEXT_CAP_PTR_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_MSI_CAP_MSI_CAP_MSG_CONTROL_IIO_HQM_VF_REG supported on:            */
/*      SPRA0 (0x10e010b2)                                                      */
/*      SPRB0 (0x10e010b2)                                                      */
/*      SPRC0 (0x10e010b2)                                                      */
/*      SPRMCC (0x10e010b2)                                                     */
/*      SPRUCC (0x10e010b2)                                                     */
/* Register default value on SPRA0: 0x0000018A                                  */
/* Register default value on SPRB0: 0x0000018A                                  */
/* Register default value on SPRC0: 0x0000018A                                  */
/* Register default value on SPRMCC: 0x0000018A                                 */
/* Register default value on SPRUCC: 0x0000018A                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Message Signalled Interrupt Message Control Register
*/


#define HQM_PCIE_MSI_CAP_MSI_CAP_MSG_CONTROL_IIO_HQM_VF_REG 0x215100B2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 msi_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               MSI Enable: System software sets this bit to
                               enable MSI signaling. A device driver is
                               prohibited from writing this bit to mask a
                               device??s service request.If 1, the device can
                               use an MSI to request service.If 0, the device
                               cannot use an MSI to request service.
                            */
    UINT16 multi_msg_cap : 3;

                            /* Bits[3:1], Access Type=RO, default=0x00000005*/

                            /*
                               Multiple Message Capable: System software reads
                               this field to determine the number of requested
                               messages.Hardwired to 0 to request one message.
                            */
    UINT16 multi_msg_en : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Multiple Message Enable: System software writes
                               to this field to indicate the number of
                               allocated messages (less than or equal to the
                               number of requested messages in MMC). A value of
                               0 corresponds to one message.
                            */
    UINT16 addr64 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000001*/

                            /*
                               64 bit Address Capable: set to 1 if device can
                               generate 64b message addresses.
                            */
    UINT16 vec_mask_cap : 1;

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

} HQM_PCIE_MSI_CAP_MSI_CAP_MSG_CONTROL_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_MSI_CAP_MSI_CAP_ADDR_L_IIO_HQM_VF_REG supported on:                 */
/*      SPRA0 (0x20e010b4)                                                      */
/*      SPRB0 (0x20e010b4)                                                      */
/*      SPRC0 (0x20e010b4)                                                      */
/*      SPRMCC (0x20e010b4)                                                     */
/*      SPRUCC (0x20e010b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Message Signalled Interrupt Lower Address Register
*/


#define HQM_PCIE_MSI_CAP_MSI_CAP_ADDR_L_IIO_HQM_VF_REG 0x215200B4

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
                               Message Address: Written by the system to
                               indicate the lower 30-bits of the address to use
                               for the MSI memory write transaction.
                            */

  } Bits;
  UINT32 Data;

} HQM_PCIE_MSI_CAP_MSI_CAP_ADDR_L_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_MSI_CAP_MSI_CAP_ADDR_U_IIO_HQM_VF_REG supported on:                 */
/*      SPRA0 (0x20e010b8)                                                      */
/*      SPRB0 (0x20e010b8)                                                      */
/*      SPRC0 (0x20e010b8)                                                      */
/*      SPRMCC (0x20e010b8)                                                     */
/*      SPRUCC (0x20e010b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Message Signalled Interrupt Upper Address Register
*/


#define HQM_PCIE_MSI_CAP_MSI_CAP_ADDR_U_IIO_HQM_VF_REG 0x215200B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Message Address: Written by the system to
                               indicate the lower 32-bits of the address to use
                               for the MSI memory write transaction. The lower
                               two bits will always be written as 0.
                            */

  } Bits;
  UINT32 Data;

} HQM_PCIE_MSI_CAP_MSI_CAP_ADDR_U_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_MSI_CAP_MSI_CAP_MSG_DATA_IIO_HQM_VF_REG supported on:               */
/*      SPRA0 (0x10e010bc)                                                      */
/*      SPRB0 (0x10e010bc)                                                      */
/*      SPRC0 (0x10e010bc)                                                      */
/*      SPRMCC (0x10e010bc)                                                     */
/*      SPRUCC (0x10e010bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Message Signalled Interrupt Data Register
*/


#define HQM_PCIE_MSI_CAP_MSI_CAP_MSG_DATA_IIO_HQM_VF_REG 0x215100BC

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

} HQM_PCIE_MSI_CAP_MSI_CAP_MSG_DATA_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_MSI_CAP_MSI_CAP_MSG_MASK_IIO_HQM_VF_REG supported on:               */
/*      SPRA0 (0x20e010c0)                                                      */
/*      SPRB0 (0x20e010c0)                                                      */
/*      SPRC0 (0x20e010c0)                                                      */
/*      SPRMCC (0x20e010c0)                                                     */
/*      SPRUCC (0x20e010c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Message Signalled Interrupt Mask Register
*/


#define HQM_PCIE_MSI_CAP_MSI_CAP_MSG_MASK_IIO_HQM_VF_REG 0x215200C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 msg_mask : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Mask Bits: 32 vectors supported. */

  } Bits;
  UINT32 Data;

} HQM_PCIE_MSI_CAP_MSI_CAP_MSG_MASK_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_MSI_CAP_MSI_CAP_MSG_PEND_IIO_HQM_VF_REG supported on:               */
/*      SPRA0 (0x20e010c4)                                                      */
/*      SPRB0 (0x20e010c4)                                                      */
/*      SPRC0 (0x20e010c4)                                                      */
/*      SPRMCC (0x20e010c4)                                                     */
/*      SPRUCC (0x20e010c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Message Signalled Interrupt Pending Register
*/


#define HQM_PCIE_MSI_CAP_MSI_CAP_MSG_PEND_IIO_HQM_VF_REG 0x215200C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 msg_pend : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* Pending Bits: 32 vectors supported. */

  } Bits;
  UINT32 Data;

} HQM_PCIE_MSI_CAP_MSI_CAP_MSG_PEND_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_ACS_CAP_ACS_CAP_ID_IIO_HQM_VF_REG supported on:                     */
/*      SPRA0 (0x10e01100)                                                      */
/*      SPRB0 (0x10e01100)                                                      */
/*      SPRC0 (0x10e01100)                                                      */
/*      SPRMCC (0x10e01100)                                                     */
/*      SPRUCC (0x10e01100)                                                     */
/* Register default value on SPRA0: 0x0000000D                                  */
/* Register default value on SPRB0: 0x0000000D                                  */
/* Register default value on SPRC0: 0x0000000D                                  */
/* Register default value on SPRMCC: 0x0000000D                                 */
/* Register default value on SPRUCC: 0x0000000D                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ACS capability
*/


#define HQM_PCIE_ACS_CAP_ACS_CAP_ID_IIO_HQM_VF_REG 0x21510100

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

} HQM_PCIE_ACS_CAP_ACS_CAP_ID_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_ACS_CAP_ACS_CAP_VERSION_NEXT_PTR_IIO_HQM_VF_REG supported on:       */
/*      SPRA0 (0x10e01102)                                                      */
/*      SPRB0 (0x10e01102)                                                      */
/*      SPRC0 (0x10e01102)                                                      */
/*      SPRMCC (0x10e01102)                                                     */
/*      SPRUCC (0x10e01102)                                                     */
/* Register default value on SPRA0: 0x00001481                                  */
/* Register default value on SPRB0: 0x00001481                                  */
/* Register default value on SPRC0: 0x00001481                                  */
/* Register default value on SPRMCC: 0x00001481                                 */
/* Register default value on SPRUCC: 0x00001481                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ACS Capability Version
*/


#define HQM_PCIE_ACS_CAP_ACS_CAP_VERSION_NEXT_PTR_IIO_HQM_VF_REG 0x21510102

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

                            /* Bits[15:4], Access Type=RO, default=0x00000148*/

                            /* Next Capability Pointer (AER Capabilities) */

  } Bits;
  UINT16 Data;

} HQM_PCIE_ACS_CAP_ACS_CAP_VERSION_NEXT_PTR_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_ACS_CAP_ACS_CAP_IIO_HQM_VF_REG supported on:                        */
/*      SPRA0 (0x10e01104)                                                      */
/*      SPRB0 (0x10e01104)                                                      */
/*      SPRC0 (0x10e01104)                                                      */
/*      SPRMCC (0x10e01104)                                                     */
/*      SPRUCC (0x10e01104)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ACS Capabilities Register
*/


#define HQM_PCIE_ACS_CAP_ACS_CAP_IIO_HQM_VF_REG 0x21510104

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

} HQM_PCIE_ACS_CAP_ACS_CAP_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_PCIE_ACS_CAP_ACS_CAP_CONTROL_IIO_HQM_VF_REG supported on:                */
/*      SPRA0 (0x10e01106)                                                      */
/*      SPRB0 (0x10e01106)                                                      */
/*      SPRC0 (0x10e01106)                                                      */
/*      SPRMCC (0x10e01106)                                                     */
/*      SPRUCC (0x10e01106)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ACS Capabilities Register
*/


#define HQM_PCIE_ACS_CAP_ACS_CAP_CONTROL_IIO_HQM_VF_REG 0x21510106

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

} HQM_PCIE_ACS_CAP_ACS_CAP_CONTROL_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_AER_CAP_AER_CAP_ID_IIO_HQM_VF_REG supported on:                  */
/*      SPRA0 (0x10e01148)                                                      */
/*      SPRB0 (0x10e01148)                                                      */
/*      SPRC0 (0x10e01148)                                                      */
/*      SPRMCC (0x10e01148)                                                     */
/*      SPRUCC (0x10e01148)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Capability ID Register
*/


#define HQM_VF_PCIE_AER_CAP_AER_CAP_ID_IIO_HQM_VF_REG 0x21510148

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

} HQM_VF_PCIE_AER_CAP_AER_CAP_ID_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_AER_CAP_AER_CAP_VERSION_NEXT_PTR_IIO_HQM_VF_REG supported on:    */
/*      SPRA0 (0x10e0114a)                                                      */
/*      SPRB0 (0x10e0114a)                                                      */
/*      SPRC0 (0x10e0114a)                                                      */
/*      SPRMCC (0x10e0114a)                                                     */
/*      SPRUCC (0x10e0114a)                                                     */
/* Register default value on SPRA0: 0x00000002                                  */
/* Register default value on SPRB0: 0x00000002                                  */
/* Register default value on SPRC0: 0x00000002                                  */
/* Register default value on SPRMCC: 0x00000002                                 */
/* Register default value on SPRUCC: 0x00000002                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Capability ID Register
*/


#define HQM_VF_PCIE_AER_CAP_AER_CAP_VERSION_NEXT_PTR_IIO_HQM_VF_REG 0x2151014A

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

} HQM_VF_PCIE_AER_CAP_AER_CAP_VERSION_NEXT_PTR_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_AER_CAP_AER_CAP_UNCORR_ERR_STATUS_IIO_HQM_VF_REG supported on:   */
/*      SPRA0 (0x20e0114c)                                                      */
/*      SPRB0 (0x20e0114c)                                                      */
/*      SPRC0 (0x20e0114c)                                                      */
/*      SPRMCC (0x20e0114c)                                                     */
/*      SPRUCC (0x20e0114c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Uncorrectable Error Status Register
*/


#define HQM_VF_PCIE_AER_CAP_AER_CAP_UNCORR_ERR_STATUS_IIO_HQM_VF_REG 0x2152014C

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

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Malformed TLP: As a receiver, set whenever a
                               malformed TLP is detected. The Header is logged.
                            */
    UINT32 ecrcc : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

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

} HQM_VF_PCIE_AER_CAP_AER_CAP_UNCORR_ERR_STATUS_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_AER_CAP_AER_CAP_UNCORR_ERR_MASK_IIO_HQM_VF_REG supported on:     */
/*      SPRA0 (0x20e01150)                                                      */
/*      SPRB0 (0x20e01150)                                                      */
/*      SPRC0 (0x20e01150)                                                      */
/*      SPRMCC (0x20e01150)                                                     */
/*      SPRUCC (0x20e01150)                                                     */
/* Register default value on SPRA0: 0x00400000                                  */
/* Register default value on SPRB0: 0x00400000                                  */
/* Register default value on SPRC0: 0x00400000                                  */
/* Register default value on SPRMCC: 0x00400000                                 */
/* Register default value on SPRUCC: 0x00400000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Uncorrectable Error Mask Register
*/


#define HQM_VF_PCIE_AER_CAP_AER_CAP_UNCORR_ERR_MASK_IIO_HQM_VF_REG 0x21520150

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved2 : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000000*/

                            /* Reserved2 */
    UINT32 dlpe : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

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

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

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

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Completion Time Out Error Mask : When 1 error
                               reporting is masked.
                            */
    UINT32 ca : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Completer Abort Error Mask : When 1 error
                               reporting is masked.
                            */
    UINT32 ec : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

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

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Malformed TLP Error Mask : When 1 error
                               reporting is masked.
                            */
    UINT32 ecrcc : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               ECRC Check Error Mask : When 1 error reporting
                               is masked.
                            */
    UINT32 ur : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

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

} HQM_VF_PCIE_AER_CAP_AER_CAP_UNCORR_ERR_MASK_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_AER_CAP_AER_CAP_UNCORR_ERR_SEV_IIO_HQM_VF_REG supported on:      */
/*      SPRA0 (0x20e01154)                                                      */
/*      SPRB0 (0x20e01154)                                                      */
/*      SPRC0 (0x20e01154)                                                      */
/*      SPRMCC (0x20e01154)                                                     */
/*      SPRUCC (0x20e01154)                                                     */
/* Register default value on SPRA0: 0x00400000                                  */
/* Register default value on SPRB0: 0x00400000                                  */
/* Register default value on SPRC0: 0x00400000                                  */
/* Register default value on SPRMCC: 0x00400000                                 */
/* Register default value on SPRUCC: 0x00400000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Uncorrectable Error Severity Register
*/


#define HQM_VF_PCIE_AER_CAP_AER_CAP_UNCORR_ERR_SEV_IIO_HQM_VF_REG 0x21520154

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved2 : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000000*/

                            /* reserved2 */
    UINT32 dlpe : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

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

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /* poisoned tlp received severity: */
    UINT32 fcpes : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               flow control protocol error severity:not
                               supported.Not applicable for IOSF.
                            */
    UINT32 ct : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /* completion time out severity: */
    UINT32 ca : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /* completer abort severity: */
    UINT32 ec : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /* unexpected completion severity: */
    UINT32 ro : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               receiver overflow severity:Not applicable for
                               IOSF.
                            */
    UINT32 mtlp : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /* malformed tlp severity: */
    UINT32 ecrcc : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /* ecrc check severity: */
    UINT32 ur : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

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

} HQM_VF_PCIE_AER_CAP_AER_CAP_UNCORR_ERR_SEV_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_AER_CAP_AER_CAP_CORR_ERR_STATUS_IIO_HQM_VF_REG supported on:     */
/*      SPRA0 (0x20e01158)                                                      */
/*      SPRB0 (0x20e01158)                                                      */
/*      SPRC0 (0x20e01158)                                                      */
/*      SPRMCC (0x20e01158)                                                     */
/*      SPRUCC (0x20e01158)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Correctable Error Register
*/


#define HQM_VF_PCIE_AER_CAP_AER_CAP_CORR_ERR_STATUS_IIO_HQM_VF_REG 0x21520158

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

} HQM_VF_PCIE_AER_CAP_AER_CAP_CORR_ERR_STATUS_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_AER_CAP_AER_CAP_CORR_ERR_MASK_IIO_HQM_VF_REG supported on:       */
/*      SPRA0 (0x20e0115c)                                                      */
/*      SPRB0 (0x20e0115c)                                                      */
/*      SPRC0 (0x20e0115c)                                                      */
/*      SPRMCC (0x20e0115c)                                                     */
/*      SPRUCC (0x20e0115c)                                                     */
/* Register default value on SPRA0: 0x00004000                                  */
/* Register default value on SPRB0: 0x00004000                                  */
/* Register default value on SPRC0: 0x00004000                                  */
/* Register default value on SPRMCC: 0x00004000                                 */
/* Register default value on SPRUCC: 0x00004000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Correctable Error Mask Register
*/


#define HQM_VF_PCIE_AER_CAP_AER_CAP_CORR_ERR_MASK_IIO_HQM_VF_REG 0x2152015C

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

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Bad TLP Mask:Not supported since HQM is an IOSF
                               device
                            */
    UINT32 bdllps : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Bad DLLP Mask:Not supported since HQM is an IOSF
                               device
                            */
    UINT32 rnrs : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

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

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

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

} HQM_VF_PCIE_AER_CAP_AER_CAP_CORR_ERR_MASK_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_AER_CAP_AER_CAP_CONTROL_IIO_HQM_VF_REG supported on:             */
/*      SPRA0 (0x20e01160)                                                      */
/*      SPRB0 (0x20e01160)                                                      */
/*      SPRC0 (0x20e01160)                                                      */
/*      SPRMCC (0x20e01160)                                                     */
/*      SPRUCC (0x20e01160)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Control and Capability Register
*/


#define HQM_VF_PCIE_AER_CAP_AER_CAP_CONTROL_IIO_HQM_VF_REG 0x21520160

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

} HQM_VF_PCIE_AER_CAP_AER_CAP_CONTROL_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_AER_CAP_AER_CAP_HEADER_LOG_0_IIO_HQM_VF_REG supported on:        */
/*      SPRA0 (0x20e01164)                                                      */
/*      SPRB0 (0x20e01164)                                                      */
/*      SPRC0 (0x20e01164)                                                      */
/*      SPRMCC (0x20e01164)                                                     */
/*      SPRUCC (0x20e01164)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Header Log 0 Register
*/


#define HQM_VF_PCIE_AER_CAP_AER_CAP_HEADER_LOG_0_IIO_HQM_VF_REG 0x21520164

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

} HQM_VF_PCIE_AER_CAP_AER_CAP_HEADER_LOG_0_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_AER_CAP_AER_CAP_HEADER_LOG_1_IIO_HQM_VF_REG supported on:        */
/*      SPRA0 (0x20e01168)                                                      */
/*      SPRB0 (0x20e01168)                                                      */
/*      SPRC0 (0x20e01168)                                                      */
/*      SPRMCC (0x20e01168)                                                     */
/*      SPRUCC (0x20e01168)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Header Log 1 Register
*/


#define HQM_VF_PCIE_AER_CAP_AER_CAP_HEADER_LOG_1_IIO_HQM_VF_REG 0x21520168

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

} HQM_VF_PCIE_AER_CAP_AER_CAP_HEADER_LOG_1_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_AER_CAP_AER_CAP_HEADER_LOG_2_IIO_HQM_VF_REG supported on:        */
/*      SPRA0 (0x20e0116c)                                                      */
/*      SPRB0 (0x20e0116c)                                                      */
/*      SPRC0 (0x20e0116c)                                                      */
/*      SPRMCC (0x20e0116c)                                                     */
/*      SPRUCC (0x20e0116c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Header Log 2 Register
*/


#define HQM_VF_PCIE_AER_CAP_AER_CAP_HEADER_LOG_2_IIO_HQM_VF_REG 0x2152016C

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

} HQM_VF_PCIE_AER_CAP_AER_CAP_HEADER_LOG_2_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_AER_CAP_AER_CAP_HEADER_LOG_3_IIO_HQM_VF_REG supported on:        */
/*      SPRA0 (0x20e01170)                                                      */
/*      SPRB0 (0x20e01170)                                                      */
/*      SPRC0 (0x20e01170)                                                      */
/*      SPRMCC (0x20e01170)                                                     */
/*      SPRUCC (0x20e01170)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Header Log 3 Register
*/


#define HQM_VF_PCIE_AER_CAP_AER_CAP_HEADER_LOG_3_IIO_HQM_VF_REG 0x21520170

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

} HQM_VF_PCIE_AER_CAP_AER_CAP_HEADER_LOG_3_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_AER_CAP_AER_CAP_ROOT_ERROR_COMMAND_IIO_HQM_VF_REG supported on:  */
/*      SPRA0 (0x20e01174)                                                      */
/*      SPRB0 (0x20e01174)                                                      */
/*      SPRC0 (0x20e01174)                                                      */
/*      SPRMCC (0x20e01174)                                                     */
/*      SPRUCC (0x20e01174)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Root Error Command Register
*/


#define HQM_VF_PCIE_AER_CAP_AER_CAP_ROOT_ERROR_COMMAND_IIO_HQM_VF_REG 0x21520174

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

} HQM_VF_PCIE_AER_CAP_AER_CAP_ROOT_ERROR_COMMAND_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_AER_CAP_AER_CAP_ROOT_ERROR_STATUS_IIO_HQM_VF_REG supported on:   */
/*      SPRA0 (0x20e01178)                                                      */
/*      SPRB0 (0x20e01178)                                                      */
/*      SPRC0 (0x20e01178)                                                      */
/*      SPRMCC (0x20e01178)                                                     */
/*      SPRUCC (0x20e01178)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Root Error Status Register
*/


#define HQM_VF_PCIE_AER_CAP_AER_CAP_ROOT_ERROR_STATUS_IIO_HQM_VF_REG 0x21520178

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

} HQM_VF_PCIE_AER_CAP_AER_CAP_ROOT_ERROR_STATUS_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_AER_CAP_AER_CAP_ERROR_SOURCE_IDENT_IIO_HQM_VF_REG supported on:  */
/*      SPRA0 (0x20e0117c)                                                      */
/*      SPRB0 (0x20e0117c)                                                      */
/*      SPRC0 (0x20e0117c)                                                      */
/*      SPRMCC (0x20e0117c)                                                     */
/*      SPRUCC (0x20e0117c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER Error Source Identification Register
*/


#define HQM_VF_PCIE_AER_CAP_AER_CAP_ERROR_SOURCE_IDENT_IIO_HQM_VF_REG 0x2152017C

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

} HQM_VF_PCIE_AER_CAP_AER_CAP_ERROR_SOURCE_IDENT_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_0_IIO_HQM_VF_REG supported on:    */
/*      SPRA0 (0x20e01180)                                                      */
/*      SPRB0 (0x20e01180)                                                      */
/*      SPRC0 (0x20e01180)                                                      */
/*      SPRMCC (0x20e01180)                                                     */
/*      SPRUCC (0x20e01180)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER TLP Prefix Log 0 Register
*/


#define HQM_VF_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_0_IIO_HQM_VF_REG 0x21520180

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

} HQM_VF_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_0_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_1_IIO_HQM_VF_REG supported on:    */
/*      SPRA0 (0x20e01184)                                                      */
/*      SPRB0 (0x20e01184)                                                      */
/*      SPRC0 (0x20e01184)                                                      */
/*      SPRMCC (0x20e01184)                                                     */
/*      SPRUCC (0x20e01184)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER TLP Prefix Log 1 Register
*/


#define HQM_VF_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_1_IIO_HQM_VF_REG 0x21520184

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

} HQM_VF_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_1_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_2_IIO_HQM_VF_REG supported on:    */
/*      SPRA0 (0x20e01188)                                                      */
/*      SPRB0 (0x20e01188)                                                      */
/*      SPRC0 (0x20e01188)                                                      */
/*      SPRMCC (0x20e01188)                                                     */
/*      SPRUCC (0x20e01188)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER TLP Prefix Log 2 Register
*/


#define HQM_VF_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_2_IIO_HQM_VF_REG 0x21520188

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

} HQM_VF_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_2_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HQM_VF_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_3_IIO_HQM_VF_REG supported on:    */
/*      SPRA0 (0x20e0118c)                                                      */
/*      SPRB0 (0x20e0118c)                                                      */
/*      SPRC0 (0x20e0118c)                                                      */
/*      SPRMCC (0x20e0118c)                                                     */
/*      SPRUCC (0x20e0118c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRB0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRC0 Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG  */
/* SPRMCC Register File:    sprsp_top/hqm_map/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG    */
/* SPRUCC Register File:    sprsp_top/hqm_map[0]/hqm_vf_cfg_i[0]/hqm_vf_cfg_CFG */
/* Struct generated from SPRA0 BDF: 14_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express AER TLP Prefix Log 3 Register
*/


#define HQM_VF_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_3_IIO_HQM_VF_REG 0x2152018C

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

} HQM_VF_PCIE_AER_CAP_AER_CAP_TLP_PREFIX_LOG_3_IIO_HQM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */
#endif /* _IIO_HQM_VF_h */
