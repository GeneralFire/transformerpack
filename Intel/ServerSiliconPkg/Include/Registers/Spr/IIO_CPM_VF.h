
/** @file
  IIO_CPM_VF.h

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

/* SPRB0 Security Policy Groups:                                                */
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

/* SPRHBM Security Policy Groups:                                               */

/* SPRC0 Security Policy Groups:                                                */
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

/* SPRMCC Security Policy Groups:                                               */

/* SPRUCC Security Policy Groups:                                               */
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


#ifndef _IIO_CPM_VF_h
#define _IIO_CPM_VF_h
#include <Base.h>

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VVID_IIO_CPM_VF_REG supported on:                                            */
/*      SPRA0 (0x10d01000)                                                      */
/*      SPRB0 (0x10d01000)                                                      */
/*      SPRC0 (0x10d01000)                                                      */
/*      SPRUCC (0x10d01000)                                                     */
/* Register default value on SPRA0: 0x0000FFFF                                  */
/* Register default value on SPRB0: 0x0000FFFF                                  */
/* Register default value on SPRC0: 0x0000FFFF                                  */
/* Register default value on SPRUCC: 0x0000FFFF                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF Vendor Identification Register
*/


#define VVID_IIO_CPM_VF_REG 0x21310000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 vid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000FFFF*/

                            /*
                               Vendor ID: The SR-IOV specification requires
                               this field to return 0xFFFF.
                            */

  } Bits;
  UINT16 Data;

} VVID_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VDID_IIO_CPM_VF_REG supported on:                                            */
/*      SPRA0 (0x10d01002)                                                      */
/*      SPRB0 (0x10d01002)                                                      */
/*      SPRC0 (0x10d01002)                                                      */
/*      SPRUCC (0x10d01002)                                                     */
/* Register default value on SPRA0: 0x0000FFFF                                  */
/* Register default value on SPRB0: 0x0000FFFF                                  */
/* Register default value on SPRC0: 0x0000FFFF                                  */
/* Register default value on SPRUCC: 0x0000FFFF                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF Device Identification Register
*/


#define VDID_IIO_CPM_VF_REG 0x21310002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 vdid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000FFFF*/

                            /*
                               Device ID. The SR-IOV Specification requires
                               this field to return FFFFH. This field is
                               hardwired to 0xFFFF.
                            */

  } Bits;
  UINT16 Data;

} VDID_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPCICMD_IIO_CPM_VF_REG supported on:                                         */
/*      SPRA0 (0x10d01004)                                                      */
/*      SPRB0 (0x10d01004)                                                      */
/*      SPRC0 (0x10d01004)                                                      */
/*      SPRUCC (0x10d01004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF Device Command Register
*/


#define VPCICMD_IIO_CPM_VF_REG 0x21310004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 io : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               I/O Space Enable: The SR-IOV specification
                               requires that this field is hardwired to 0b for
                               all VFs.
                            */
    UINT16 mem : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Memory Enable: The SR-IOV specification requires
                               that this field is hardwired to 0b for all VFs.
                            */
    UINT16 bm : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Bus Master Enable: When cleared, the CPM is
                               prevented from issuing any memory or I/O
                               read/write requests. Requests other than memory
                               or I/O requests are not controlled by this
                               bit.The CPM will initiate a completion
                               transaction regardless of the
                               setting.Transactions for a VF that has its Bus
                               Master Enable set must not be blocked by
                               transactions for VFs that have their Bus Master
                               Enable cleared.
                            */
    UINT16 ss : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Special Cycle Enable: Does not apply to PCI
                               Express. Hard-wired to 0.
                            */
    UINT16 mwe : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Memory Write and Invalidate Enable : Does not
                               apply to PCI Express. Hard-wired to 0.
                            */
    UINT16 vps : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               VGA Palette Snoop Enable: Does not apply to PCI
                               Express. Hard-wired to 0.
                            */
    UINT16 per : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Parity Error Response : The SR-IOV specification
                               requires that this field is hardwired to 0b for
                               all VFs. In addition the functionality
                               associated with the setting of this bit in the
                               PF will apply to all VFs.
                            */
    UINT16 reserved0 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Address/Data Stepping Control: Does not apply to
                               PCI Express. Hard-wired to 0.
                            */
    UINT16 ser : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               SERR# Enable: The SR-IOV specification requires
                               that this field is hardwired to 0b for all VFs.
                               In addition the functionality associated with
                               the setting of this bit in the PF will apply to
                               all VFs.
                            */
    UINT16 fbtb : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Fast Back to Back Enable: Does not apply to PCI
                               Express. Hard-wired to 0
                            */
    UINT16 intd : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Interrupt Disable: SR-IOV specification requires
                               that this field is hardwired to 0b for all VFs.
                               This bit does not apply to VFs.
                            */
    UINT16 reserved : 5;

                            /* Bits[15:11], Access Type=RO, default=0x00000000*/

                            /* Reserved0 */

  } Bits;
  UINT16 Data;

} VPCICMD_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPCISTS_IIO_CPM_VF_REG supported on:                                         */
/*      SPRA0 (0x10d01006)                                                      */
/*      SPRB0 (0x10d01006)                                                      */
/*      SPRC0 (0x10d01006)                                                      */
/*      SPRUCC (0x10d01006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF PCI Device Status Register
*/


#define VPCISTS_IIO_CPM_VF_REG 0x21310006

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
                               Interrupt Status: The SR-IOV specification
                               requires that this field is hardwired to 0b for
                               all VFs.This bit does not apply to VFs.
                            */
    UINT16 cl : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               Capabilities List: All PCI Express devices are
                               required to implement the PCI Express capability
                               structure.Hard-wired to 1.
                            */
    UINT16 mc66 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               66 MHz Capable (C66):Does not apply to PCI
                               Express. Hard-wired to 0
                            */
    UINT16 reserved1 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */
    UINT16 fb2b : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Fast Back-to-Back :Does not apply to PCI
                               Express.Hard-wired to 0.
                            */
    UINT16 mdpe : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Master Data Parity Error: This bit is set by the
                               CPM if its Parity Error Enable bit is set and
                               either of the following two conditions
                               occurs:This bit is set under the following
                               conditions.CPM receives a Poisoned Completion
                               for an Outbound Read RequestCPM transmits a
                               Poisoned TLP for an Outbound Write Request.If
                               the Parity Error Response bit is cleared in the
                               this bit is never set.
                            */
    UINT16 dst : 2;

                            /* Bits[10:9], Access Type=RO, default=0x00000000*/

                            /*
                               HASH(0x105b460) Timing: DEVSEL# Does not apply
                               to PCI Express.Hard-wired to 0.
                            */
    UINT16 sta : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Signaled Target Abort: set when the device
                               completes a Request using Completer Abort
                               Completion Status. CPM does not support
                               returning CA.
                            */
    UINT16 rta : 1;

                            /* Bits[12:12], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Received Target Abort: set when the CPM receives
                               a completion with Completer Abort Completion
                               Status.
                            */
    UINT16 rma : 1;

                            /* Bits[13:13], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Received Master Abort: set when the CPM receives
                               a completion with Unsupported Request Completion
                               Status.
                            */
    UINT16 sse : 1;

                            /* Bits[14:14], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               SERR# Asserted: set when the CPM sends an ERR
                               FATAL or ERR NONFATAL message, and the SERR
                               Enable bit in the PPCICMD register is 1.
                            */
    UINT16 dpe : 1;

                            /* Bits[15:15], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Detected Parity Error: set when the CPM receives
                               a poisoned TLP regardless of the state of the
                               Parity Error Response in the VPCICMD register.
                            */

  } Bits;
  UINT16 Data;

} VPCISTS_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VRID_IIO_CPM_VF_REG supported on:                                            */
/*      SPRA0 (0x20d01008)                                                      */
/*      SPRB0 (0x20d01008)                                                      */
/*      SPRC0 (0x20d01008)                                                      */
/*      SPRUCC (0x20d01008)                                                     */
/* Register default value on SPRA0: 0x0B400000                                  */
/* Register default value on SPRB0: 0x0B400000                                  */
/* Register default value on SPRC0: 0x0B400000                                  */
/* Register default value on SPRUCC: 0x0B400000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF Revision ID Register and VF Class Code Registe
*/


#define VRID_IIO_CPM_VF_REG 0x21320008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rid : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               CPM Revision: The SR-IOV specification states
                               that this field should be viewed as a Vendor
                               Defined Extension to the Device ID. This field
                               may be different than the PF Revision ID, but
                               the same value must be reported by all VFs. This
                               value is loaded from fuses and shared the same
                               value as PRID value.
                            */
    UINT32 cc : 24;

                            /* Bits[31:8], Access Type=RW/O/P, default=0x000B4000*/

                            /*
                               Class Code: The SR-IOV specification requires
                               that this field return the same value as the PF
                               for all VFs.
                            */

  } Bits;
  UINT32 Data;

} VRID_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VHDR_IIO_CPM_VF_REG supported on:                                            */
/*      SPRA0 (0xd0100e)                                                        */
/*      SPRB0 (0xd0100e)                                                        */
/*      SPRC0 (0xd0100e)                                                        */
/*      SPRUCC (0xd0100e)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF Header Type Register
*/


#define VHDR_IIO_CPM_VF_REG 0x2130000E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 hdr : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Header Type: The SR-IOV specification states
                               that this field must be set to 00H for VFs.
                            */

  } Bits;
  UINT8 Data;

} VHDR_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VSVID_IIO_CPM_VF_REG supported on:                                           */
/*      SPRA0 (0x10d0102c)                                                      */
/*      SPRB0 (0x10d0102c)                                                      */
/*      SPRC0 (0x10d0102c)                                                      */
/*      SPRUCC (0x10d0102c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF Subsystem Vendor ID Register
*/


#define VSVID_IIO_CPM_VF_REG 0x2131002C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 svid : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00008086*/

                            /*
                               Subsystem Vendor ID: The SR-IOV specification
                               requires that when read, this read only register
                               must return the same value as the PF for all
                               VFs. Value driven by PSVID.
                            */

  } Bits;
  UINT16 Data;

} VSVID_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VSID_IIO_CPM_VF_REG supported on:                                            */
/*      SPRA0 (0x10d0102e)                                                      */
/*      SPRB0 (0x10d0102e)                                                      */
/*      SPRC0 (0x10d0102e)                                                      */
/*      SPRUCC (0x10d0102e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF Subsystem ID Register
*/


#define VSID_IIO_CPM_VF_REG 0x2131002E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 sid : 16;

                            /* Bits[15:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               Subsystem ID: The SR-IOV specification requires
                               that this read only register returns the same
                               value for all VFs. This VF value may be a
                               different than the PF. Value is reset to 0x0.
                            */

  } Bits;
  UINT16 Data;

} VSID_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VCP_IIO_CPM_VF_REG supported on:                                             */
/*      SPRA0 (0xd01034)                                                        */
/*      SPRB0 (0xd01034)                                                        */
/*      SPRC0 (0xd01034)                                                        */
/*      SPRUCC (0xd01034)                                                       */
/* Register default value on SPRA0: 0x00000050                                  */
/* Register default value on SPRB0: 0x00000050                                  */
/* Register default value on SPRC0: 0x00000050                                  */
/* Register default value on SPRUCC: 0x00000050                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF Capabilities Pointer Register
*/


#define VCP_IIO_CPM_VF_REG 0x21300034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 cp : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000050*/

                            /*
                               Capability List Pointer: This field provides an
                               offset into the configuration space pointing to
                               the first item in the capability list which is
                               PCIe capability.
                            */

  } Bits;
  UINT8 Data;

} VCP_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VIRQL_IIO_CPM_VF_REG supported on:                                           */
/*      SPRA0 (0xd0103c)                                                        */
/*      SPRB0 (0xd0103c)                                                        */
/*      SPRC0 (0xd0103c)                                                        */
/*      SPRUCC (0xd0103c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF Interrupt Line Register
*/


#define VIRQL_IIO_CPM_VF_REG 0x2130003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 irql : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Interrupt Assigned: This field does not apply to
                               VFs and is hardwired to Zero.
                            */

  } Bits;
  UINT8 Data;

} VIRQL_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VIRQP_IIO_CPM_VF_REG supported on:                                           */
/*      SPRA0 (0xd0103d)                                                        */
/*      SPRB0 (0xd0103d)                                                        */
/*      SPRC0 (0xd0103d)                                                        */
/*      SPRUCC (0xd0103d)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF Interrupt Pin Register
*/


#define VIRQP_IIO_CPM_VF_REG 0x2130003D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 irqp : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Interrupt Assigned:This field does not apply to
                               VFs and is hardwired to Zero.
                            */

  } Bits;
  UINT8 Data;

} VIRQP_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VFFUSECTL0_IIO_CPM_VF_REG supported on:                                      */
/*      SPRA0 (0x20d01040)                                                      */
/*      SPRB0 (0x20d01040)                                                      */
/*      SPRC0 (0x20d01040)                                                      */
/*      SPRUCC (0x20d01040)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Fuse Control and Status Register 0. Access to disabled units within CPM will result in undefined behavior,  causing System Hang.
*/


#define VFFUSECTL0_IIO_CPM_VF_REG 0x21320040

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

} VFFUSECTL0_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VFFUSECTL1_IIO_CPM_VF_REG supported on:                                      */
/*      SPRA0 (0x20d01044)                                                      */
/*      SPRB0 (0x20d01044)                                                      */
/*      SPRC0 (0x20d01044)                                                      */
/*      SPRUCC (0x20d01044)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Fuse Control and Status Register 1. Access to disabled units within CPM will result in undefined behavior,  causing System Hang.
*/


#define VFFUSECTL1_IIO_CPM_VF_REG 0x21320044

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

} VFFUSECTL1_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VFFUSECTL2_IIO_CPM_VF_REG supported on:                                      */
/*      SPRA0 (0x20d0104c)                                                      */
/*      SPRB0 (0x20d0104c)                                                      */
/*      SPRC0 (0x20d0104c)                                                      */
/*      SPRUCC (0x20d0104c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Fuse Control Register 2
*/


#define VFFUSECTL2_IIO_CPM_VF_REG 0x2132004C

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

} VFFUSECTL2_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPCID_IIO_CPM_VF_REG supported on:                                           */
/*      SPRA0 (0xd01050)                                                        */
/*      SPRB0 (0xd01050)                                                        */
/*      SPRC0 (0xd01050)                                                        */
/*      SPRUCC (0xd01050)                                                       */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF PCI Express Capability ID Register
*/


#define VPCID_IIO_CPM_VF_REG 0x21300050

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 pciecid : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000010*/

                            /*
                               Cap Id: This field identifies this item in the
                               linked list of Extended Capability Headers as
                               being the PCI Express capability registers.
                            */

  } Bits;
  UINT8 Data;

} VPCID_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPCP_IIO_CPM_VF_REG supported on:                                            */
/*      SPRA0 (0xd01051)                                                        */
/*      SPRB0 (0xd01051)                                                        */
/*      SPRC0 (0xd01051)                                                        */
/*      SPRUCC (0xd01051)                                                       */
/* Register default value on SPRA0: 0x00000090                                  */
/* Register default value on SPRB0: 0x00000090                                  */
/* Register default value on SPRC0: 0x00000090                                  */
/* Register default value on SPRUCC: 0x00000090                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF PCI Express Next Capability Pointer Register
*/


#define VPCP_IIO_CPM_VF_REG 0x21300051

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 mcp : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000090*/

                            /* Next Capability Pointer: Last capability. */

  } Bits;
  UINT8 Data;

} VPCP_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPCR_IIO_CPM_VF_REG supported on:                                            */
/*      SPRA0 (0x10d01052)                                                      */
/*      SPRB0 (0x10d01052)                                                      */
/*      SPRC0 (0x10d01052)                                                      */
/*      SPRUCC (0x10d01052)                                                     */
/* Register default value on SPRA0: 0x00000092                                  */
/* Register default value on SPRB0: 0x00000092                                  */
/* Register default value on SPRC0: 0x00000092                                  */
/* Register default value on SPRUCC: 0x00000092                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF PCI Express Capabilities Register
*/


#define VPCR_IIO_CPM_VF_REG 0x21310052

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
                               numberCPM supports version 2h.
                            */
    UINT16 dpt : 4;

                            /* Bits[7:4], Access Type=RO, default=0x00000009*/

                            /*
                               Device/Port Type: Indicates the type of PCI
                               Express logical device - RCiEP.
                            */
    UINT16 si : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Slot Implemented: Indicates that the PCI Express
                               Link associated with this port is connected to a
                               slot.Only valid for root complex and switch
                               downstream ports.Hard-wired to 0
                            */
    UINT16 imn : 5;

                            /* Bits[13:9], Access Type=RO, default=0x00000000*/

                            /*
                               Interrupt Message Number : This only applies to
                               Root Complex and Switch devices.This register is
                               hardcoded to 0.
                            */
    UINT16 reserved : 2;

                            /* Bits[15:14], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} VPCR_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPDCAP_IIO_CPM_VF_REG supported on:                                          */
/*      SPRA0 (0x20d01054)                                                      */
/*      SPRB0 (0x20d01054)                                                      */
/*      SPRC0 (0x20d01054)                                                      */
/*      SPRUCC (0x20d01054)                                                     */
/* Register default value on SPRA0: 0x10008062                                  */
/* Register default value on SPRB0: 0x10008062                                  */
/* Register default value on SPRC0: 0x10008062                                  */
/* Register default value on SPRUCC: 0x10008062                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF PCI Express Device Capabilities Register
*/


#define VPDCAP_IIO_CPM_VF_REG 0x21320054

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
                               TLPs. This value is set to indicate 256B. The
                               defined encodings are: 000b = 128B max payload
                               size 001b = 256 bytes max payload size 010b =
                               512 bytes max payload size (Max supported) 011b
                               = 1KB max payload size 100b = 2KB max payload
                               size 101b = 4KB max payload size
                            */
    UINT32 pfs : 2;

                            /* Bits[4:3], Access Type=RO, default=0x00000000*/

                            /*
                               Phantom Functions Supported: The SR-IOV
                               specification requires that this field is
                               hardwired to 00b.
                            */
    UINT32 etfs : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000001*/

                            /*
                               Extended Tag Field Supported: Indicates the
                               maximum supported size of the Tag field as a
                               Requester. CPM does generate 8-bit Tags and
                               supports 8-bit Tags as a completer. Supports 256
                               outstanding requests.
                            */
    UINT32 el0l : 3;

                            /* Bits[8:6], Access Type=RO, default=0x00000001*/

                            /*
                               Endpoint L0 Acceptable Latency: Total acceptable
                               latency that the CPM can withstand due to a
                               transition from L0s to L0 state.
                            */
    UINT32 el1l : 3;

                            /* Bits[11:9], Access Type=RO, default=0x00000000*/

                            /*
                               Endpoint L1 Acceptable Latency: CPM does not
                               support L1 active state power management.
                            */
    UINT32 attn : 3;

                            /* Bits[14:12], Access Type=RO, default=0x00000000*/

                            /* Reserved: Undefined - Treated as Reserved */
    UINT32 rbep : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000001*/

                            /*
                               Role-Based Error Reporting : this bit is set to
                               indicate that this device implements the Role
                               Base Error Reporting defined in , Revision 2.0.
                               PCI Express Base Specification
                            */
    UINT32 reserved1 : 2;

                            /* Bits[17:16], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */
    UINT32 cspv : 8;

                            /* Bits[25:18], Access Type=RO, default=0x00000000*/

                            /*
                               Captured Slot Power Limit Value : The SR-IOV
                               specification states that this field is
                               undefined for all VFs.
                            */
    UINT32 csps : 2;

                            /* Bits[27:26], Access Type=RO, default=0x00000000*/

                            /*
                               Captured Slot Power Limit Scale: The SR-IOV
                               specification states that this field is
                               undefined for all VFs.
                            */
    UINT32 flr : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000001*/

                            /*
                               FLR Cap: Function Level Reset Capability is
                               required for all VFs and PFs according to the
                               SR-IOV specification.
                            */
    UINT32 reserved : 3;

                            /* Bits[31:29], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} VPDCAP_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPDC_IIO_CPM_VF_REG supported on:                                            */
/*      SPRA0 (0x10d01058)                                                      */
/*      SPRB0 (0x10d01058)                                                      */
/*      SPRC0 (0x10d01058)                                                      */
/*      SPRUCC (0x10d01058)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF PCI Express Device Control Register
*/


#define VPDC_IIO_CPM_VF_REG 0x21310058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 cere : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Correctable Error Reporting Enable : The SR-IOV
                               specification requires that this field is
                               hardwired to all zeros. In addition the
                               functionality associated with this bit setting
                               is controlled by the corresponding bit in PF,
                               and will apply to all VFs.
                            */
    UINT16 nere : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Non-Fatal Error Reporting Enable : The SR-IOV
                               specification requires that this field is
                               hardwired to all zeros. In addition the
                               functionality associated with this bit setting
                               is controlled by the corresponding bit in PF,
                               and will apply to all VFs.
                            */
    UINT16 fere : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Fatal Error Reporting Enable: The SR-IOV
                               specification requires that this field is
                               hardwired to all zeros. In addition the
                               functionality associated with this bit setting
                               is controlled by the corresponding bit in PF,
                               and will apply to all VFs.
                            */
    UINT16 urro : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Unsupported Request Reporting Enable (URRE): The
                               SR-IOV specification requires that this field is
                               hardwired to all zeros. In addition the
                               functionality associated with this bit setting
                               is controlled by the corresponding bit in PF,
                               and will apply to all VFs.
                            */
    UINT16 ero : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Enable Relaxed Ordering: The SR-IOV
                               specification requires that this field is
                               hardwired to all zeros.
                            */
    UINT16 mps : 3;

                            /* Bits[7:5], Access Type=RO, default=0x00000000*/

                            /*
                               Max Payload Size: The SR-IOV specification
                               requires that this field is hardwired to all
                               zeros.
                            */
    UINT16 etfe : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Extended Tag Field Enable: The SR-IOV
                               specification requires that this field is
                               hardwired to all zeros.
                            */
    UINT16 pfe : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Phantom Functions Enable: The SR-IOV
                               specification requires that this field is
                               hardwired to all zeros.
                            */
    UINT16 apme : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Aux Power PM Enable : The SR-IOV specification
                               requires that this field is hardwired to all
                               zeros.
                            */
    UINT16 ens : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Enable No Snoop: The SR-IOV specification
                               requires that this field is hardwired to all
                               zeros.
                            */
    UINT16 mrs : 3;

                            /* Bits[14:12], Access Type=RO, default=0x00000000*/

                            /*
                               Max Read Request Size: The SR-IOV specification
                               requires that this field is hardwired to all
                               zeros.
                            */
    UINT16 startflr : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Initiate Function Level Reset: A write of 1b to
                               this bit initiates Function Level Reset to the
                               VF. The value is always read as 0b.
                            */

  } Bits;
  UINT16 Data;

} VPDC_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPDS_IIO_CPM_VF_REG supported on:                                            */
/*      SPRA0 (0x10d0105a)                                                      */
/*      SPRB0 (0x10d0105a)                                                      */
/*      SPRC0 (0x10d0105a)                                                      */
/*      SPRUCC (0x10d0105a)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF PCI Express Device Status Register
*/


#define VPDS_IIO_CPM_VF_REG 0x2131005A

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
                               Register.For a multi-function device, each
                               function indicates status of errors as perceived
                               by the respective function.
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
                               Register.For a multi-function device, each
                               function indicates status of errors as perceived
                               by the respective function.
                            */
    UINT16 fed : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Fatal Error Detected: This bit indicates status
                               of Fatal errors detected.A one indicates that an
                               error was detected since the last time this bit
                               was cleared.Errors are updated in this field
                               regardless of whether the error reporting bit is
                               set in the Device Control Register.For a multi-
                               function device, each function indicates status
                               of errors as perceived by the respective
                               function.
                            */
    UINT16 urd : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Unsupported Request Detected: This bit indicates
                               that the device received an Unsupported Request.
                               Errors are logged in this register regardless of
                               whether error reporting is enabled or not in the
                               Device Control Register.For a multi-function
                               device, each function indicates status of errors
                               as perceived by the respective function.
                            */
    UINT16 apd : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               AUX Power Detected: The SR-IOV specification
                               requires that this field is hardwired to all
                               zeros.
                            */
    UINT16 tp : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               Transactions Pending: This bit when set
                               indicates that a device has issued Non-Posted
                               Requests which have not been completed. A device
                               reports this bit cleared only when all
                               Completions for any outstanding Non-Posted
                               Requests have been received.
                            */
    UINT16 reserved : 10;

                            /* Bits[15:6], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} VPDS_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VLCR_IIO_CPM_VF_REG supported on:                                            */
/*      SPRA0 (0x20d0105c)                                                      */
/*      SPRB0 (0x20d0105c)                                                      */
/*      SPRC0 (0x20d0105c)                                                      */
/*      SPRUCC (0x20d0105c)                                                     */
/* Register default value on SPRA0: 0x00000011                                  */
/* Register default value on SPRB0: 0x00000011                                  */
/* Register default value on SPRC0: 0x00000011                                  */
/* Register default value on SPRUCC: 0x00000011                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Link Capabilities Register
*/


#define VLCR_IIO_CPM_VF_REG 0x2132005C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 maxspeed : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000001*/

                            /*
                               Max Link Speed - Indicates Maximum supported
                               Link Speed. Defined encodings are :0001b -
                               2.5Gbs Link speed supported (Gen 1)0010b -
                               5.0Gbs Link speed supported (Gen 2)EP indicates
                               a max Link Speed of 5.0 Gbs.Not applicable for
                               IOSF. Default to x1
                            */
    UINT32 linkw : 6;

                            /* Bits[9:4], Access Type=RO, default=0x00000001*/

                            /*
                               Max Link Width- Indicates the max link
                               width.Relevant encoding:000000b -
                               Reserved000001b - x1000010b - x2000100b -
                               x4001000b - x8001100b - x12010000b - x16100000b
                               - x32EP value depends on SKU. However the max
                               link width is x16.Not applicable for IOSF.
                               Default to x1
                            */
    UINT32 aslpm : 2;

                            /* Bits[11:10], Access Type=RO, default=0x00000000*/

                            /*
                               Active State Link PM Support - Indicates the
                               level of active state power management supported
                               in EP. Defined encodings are:00b - Reserved01b -
                               L0s Entry Supported10b - Reserved11b - L0s and
                               L1 SupportedNot applicable for IOSF. Default to
                               Reserved
                            */
    UINT32 l0el : 3;

                            /* Bits[14:12], Access Type=RO, default=0x00000000*/

                            /*
                               L0s Exit Latency- Indicates the exit latency
                               from L0s to L0 state.000b - Less than 64ns001b -
                               64ns - 128ns010b - 128ns - 256ns011b - 256ns -
                               512ns100b - 512ns - 1 s101b - 1 s - 2 s110b - 2
                               s - 4 s111b - ReservedNot applicable for IOSF.
                               Default to min Value
                            */
    UINT32 l1el : 3;

                            /* Bits[17:15], Access Type=RO, default=0x00000000*/

                            /*
                               L1 Exit Latency- Indicates the exit latency from
                               L1 to L0 state. EP does not support L1
                               transition000b - Less than 1 s001b - 1 s - 2
                               s010b - 2 s - 4 s011b - 4 s - 8 s100b - 8 s - 16
                               s101b - 16 s - 32 s110b - 32 s - 64 s111b - L1
                               transition not supportedNot applicable for IOSF.
                               Default to min Value
                            */
    UINT32 reserved3 : 3;

                            /* Bits[20:18], Access Type=RO, default=0x00000000*/

                            /* Reserved3 */
    UINT32 reserved2 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /* Reserved2 */
    UINT32 reserved1 : 2;

                            /* Bits[23:22], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */
    UINT32 portnum : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /*
                               Port Number: Assigned by EP after link training
                               phase.
                            */

  } Bits;
  UINT32 Data;

} VLCR_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VLCNTLR_IIO_CPM_VF_REG supported on:                                         */
/*      SPRA0 (0x10d01060)                                                      */
/*      SPRB0 (0x10d01060)                                                      */
/*      SPRC0 (0x10d01060)                                                      */
/*      SPRUCC (0x10d01060)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Link Control Register
*/


#define VLCNTLR_IIO_CPM_VF_REG 0x21310060

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 aspmc : 2;

                            /* Bits[1:0], Access Type=RO, default=0x00000000*/

                            /*
                               Active State PM Control: The SR-IOV
                               specification requires that this field is
                               hardwired to all zeros.
                            */
    UINT16 reserved : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /* Reserved4 */
    UINT16 rcb : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Read Completion Boundary (RCB) Control: The SR-
                               IOV specification requires that this field is
                               hardwired to all zeros.
                            */
    UINT16 linkdis : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Link Disable: Not Applicable to endpoints. Hard-
                               wired to 0
                            */
    UINT16 retrain : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Retrain Link : Not Applicable to endpoints.
                               Hard-wired to 0
                            */
    UINT16 cclkcfg : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Common Clock Configuration: The SR-IOV
                               specification requires that this field is
                               hardwired to all zeros.
                            */
    UINT16 extsync : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Extended Synch : The SR-IOV specification
                               requires that this field is hardwired to all
                               zeros.
                            */
    UINT16 eclkpm : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               The SR-IOV specification requires that this
                               field is hardwired to all zeros.
                            */
    UINT16 wd : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Reserved: The SR-IOV specification requires that
                               this field is hardwired to all zeros.
                            */
    UINT16 lbwminte : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /* Reserved3 */
    UINT16 lbwinte : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /* Reserved2 */
    UINT16 reserved1 : 4;

                            /* Bits[15:12], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */

  } Bits;
  UINT16 Data;

} VLCNTLR_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VLSR_IIO_CPM_VF_REG supported on:                                            */
/*      SPRA0 (0x10d01062)                                                      */
/*      SPRB0 (0x10d01062)                                                      */
/*      SPRC0 (0x10d01062)                                                      */
/*      SPRUCC (0x10d01062)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Link Status Register
*/


#define VLSR_IIO_CPM_VF_REG 0x21310062

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 reserved : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} VLSR_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VDCAPR2_IIO_CPM_VF_REG supported on:                                         */
/*      SPRA0 (0x20d01074)                                                      */
/*      SPRB0 (0x20d01074)                                                      */
/*      SPRC0 (0x20d01074)                                                      */
/*      SPRUCC (0x20d01074)                                                     */
/* Register default value on SPRA0: 0x00710012                                  */
/* Register default value on SPRB0: 0x00710012                                  */
/* Register default value on SPRC0: 0x00710012                                  */
/* Register default value on SPRUCC: 0x00710012                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Device Capabilities 2 Register
*/


#define VDCAPR2_IIO_CPM_VF_REG 0x21320074

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
                               in less than 10 ms.The SR-IOV specification
                               states that this field must be identical as the
                               PF value.
                            */
    UINT32 ctods : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               Completion Timeout Disable Supported.A value of
                               1b indicates support for the completion timeout
                               disable mechanism. The SR-IOV specification
                               states that this field must be identical as the
                               PF value.
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

} VDCAPR2_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VLCR2_IIO_CPM_VF_REG supported on:                                           */
/*      SPRA0 (0x20d0107c)                                                      */
/*      SPRB0 (0x20d0107c)                                                      */
/*      SPRC0 (0x20d0107c)                                                      */
/*      SPRUCC (0x20d0107c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Link Capabilities 2 Register
*/


#define VLCR2_IIO_CPM_VF_REG 0x2132007C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} VLCR2_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VLCNTLR2_IIO_CPM_VF_REG supported on:                                        */
/*      SPRA0 (0x10d01080)                                                      */
/*      SPRB0 (0x10d01080)                                                      */
/*      SPRC0 (0x10d01080)                                                      */
/*      SPRUCC (0x10d01080)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Link Control Register
*/


#define VLCNTLR2_IIO_CPM_VF_REG 0x21310080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 reserved : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /* Reserved4 */

  } Bits;
  UINT16 Data;

} VLCNTLR2_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VLSR2_IIO_CPM_VF_REG supported on:                                           */
/*      SPRA0 (0x10d01082)                                                      */
/*      SPRB0 (0x10d01082)                                                      */
/*      SPRC0 (0x10d01082)                                                      */
/*      SPRUCC (0x10d01082)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Link Status 2 Register
*/


#define VLSR2_IIO_CPM_VF_REG 0x21310082

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 reserved : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} VLSR2_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VMSICID_IIO_CPM_VF_REG supported on:                                         */
/*      SPRA0 (0xd01090)                                                        */
/*      SPRB0 (0xd01090)                                                        */
/*      SPRC0 (0xd01090)                                                        */
/*      SPRUCC (0xd01090)                                                       */
/* Register default value on SPRA0: 0x00000005                                  */
/* Register default value on SPRB0: 0x00000005                                  */
/* Register default value on SPRC0: 0x00000005                                  */
/* Register default value on SPRUCC: 0x00000005                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Message Signalled Interrupt Capability ID Register
*/


#define VMSICID_IIO_CPM_VF_REG 0x21300090

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

} VMSICID_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VMSINCP_IIO_CPM_VF_REG supported on:                                         */
/*      SPRA0 (0xd01091)                                                        */
/*      SPRB0 (0xd01091)                                                        */
/*      SPRC0 (0xd01091)                                                        */
/*      SPRUCC (0xd01091)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Message Signalled Interrupt Next Capability Pointer Register
*/


#define VMSINCP_IIO_CPM_VF_REG 0x21300091

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 mcp : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Next Capability Pointer: Next Capability is PCI
                               Express.
                            */

  } Bits;
  UINT8 Data;

} VMSINCP_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VMSICTL_IIO_CPM_VF_REG supported on:                                         */
/*      SPRA0 (0x10d01092)                                                      */
/*      SPRB0 (0x10d01092)                                                      */
/*      SPRC0 (0x10d01092)                                                      */
/*      SPRUCC (0x10d01092)                                                     */
/* Register default value on SPRA0: 0x00000180                                  */
/* Register default value on SPRB0: 0x00000180                                  */
/* Register default value on SPRC0: 0x00000180                                  */
/* Register default value on SPRUCC: 0x00000180                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Message Signalled Interrupt Control Register
*/


#define VMSICTL_IIO_CPM_VF_REG 0x21310092

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
                               0 corresponds to one message. CPM supports only
                               one message.
                            */
    UINT16 c64 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000001*/

                            /*
                               64 bit Address Capable: Hardwired to 1 to
                               indicate the device does generate 64b message
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

} VMSICTL_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VMSILADDR_IIO_CPM_VF_REG supported on:                                       */
/*      SPRA0 (0x20d01094)                                                      */
/*      SPRB0 (0x20d01094)                                                      */
/*      SPRC0 (0x20d01094)                                                      */
/*      SPRUCC (0x20d01094)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Message Signalled Interrupt Lower Address Register
*/


#define VMSILADDR_IIO_CPM_VF_REG 0x21320094

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

} VMSILADDR_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VMSIUADDR_IIO_CPM_VF_REG supported on:                                       */
/*      SPRA0 (0x20d01098)                                                      */
/*      SPRB0 (0x20d01098)                                                      */
/*      SPRC0 (0x20d01098)                                                      */
/*      SPRUCC (0x20d01098)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Message Signalled Interrupt Upper Address Register
*/


#define VMSIUADDR_IIO_CPM_VF_REG 0x21320098

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

} VMSIUADDR_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VMSIDATA_IIO_CPM_VF_REG supported on:                                        */
/*      SPRA0 (0x10d0109c)                                                      */
/*      SPRB0 (0x10d0109c)                                                      */
/*      SPRC0 (0x10d0109c)                                                      */
/*      SPRUCC (0x10d0109c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Message Signalled Interrupt Data Register
*/


#define VMSIDATA_IIO_CPM_VF_REG 0x2131009C

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

} VMSIDATA_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VMSIMSK_IIO_CPM_VF_REG supported on:                                         */
/*      SPRA0 (0x20d010a0)                                                      */
/*      SPRB0 (0x20d010a0)                                                      */
/*      SPRC0 (0x20d010a0)                                                      */
/*      SPRUCC (0x20d010a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Message Signalled Interrupt Mask Register
*/


#define VMSIMSK_IIO_CPM_VF_REG 0x213200A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Mask Bits: Only one bit defined. Refer to the
                               interrupt section.
                            */
    UINT32 reserved : 31;

                            /* Bits[31:1], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} VMSIMSK_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VMSIPND_IIO_CPM_VF_REG supported on:                                         */
/*      SPRA0 (0x20d010a4)                                                      */
/*      SPRB0 (0x20d010a4)                                                      */
/*      SPRC0 (0x20d010a4)                                                      */
/*      SPRUCC (0x20d010a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Message Signalled Interrupt Pending Register
*/


#define VMSIPND_IIO_CPM_VF_REG 0x213200A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pndb : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Pending Bits: Only one bit defined. Refer to the
                               interrupt section.
                            */
    UINT32 reserved : 31;

                            /* Bits[31:1], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} VMSIPND_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPCIEAERCAPID_IIO_CPM_VF_REG supported on:                                   */
/*      SPRA0 (0x20d01100)                                                      */
/*      SPRB0 (0x20d01100)                                                      */
/*      SPRC0 (0x20d01100)                                                      */
/*      SPRUCC (0x20d01100)                                                     */
/* Register default value on SPRA0: 0x14820001                                  */
/* Register default value on SPRB0: 0x14820001                                  */
/* Register default value on SPRC0: 0x14820001                                  */
/* Register default value on SPRUCC: 0x14820001                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF PCI Express AER Capability ID Register
*/


#define VPCIEAERCAPID_IIO_CPM_VF_REG 0x21320100

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
                               Next PCI Express Extended Capability Pointer:
                               This is hardwired to 138H to point to the
                               Alternative Routing ID extended capability.
                            */

  } Bits;
  UINT32 Data;

} VPCIEAERCAPID_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPAERUCS_IIO_CPM_VF_REG supported on:                                        */
/*      SPRA0 (0x20d01104)                                                      */
/*      SPRB0 (0x20d01104)                                                      */
/*      SPRC0 (0x20d01104)                                                      */
/*      SPRUCC (0x20d01104)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF PCI Express AER Uncorrectable Error Status Register
*/


#define VPAERUCS_IIO_CPM_VF_REG 0x21320104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved2 : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000000*/

                            /* Reserved2 . */
    UINT32 dlpe : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Data Link Protocol Error : As a non-function
                               specific error, the SR-IOV specification
                               requires that this field be hardwired to all
                               zeros.
                            */
    UINT32 sdes : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Surprise Down Error: As a non-function specific
                               error, the SR-IOV specification requires that
                               this field be hardwired to all zeros.Not
                               supported.
                            */
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
                               Flow Control Protocol Error Status: As a non-
                               function specific error, the SR-IOV
                               specification requires that this field be
                               hardwired to all zeros.Not supported.
                            */
    UINT32 ct : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Completion Timeout: As a requester, set whenever
                               an outbound Non Posted Request does not receive
                               a completion within 16-32ms.
                            */
    UINT32 ca : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /* Completer Abort : HI does not support CA. */
    UINT32 ec : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Unexpected Completion : As a receiver, set
                               whenever a completion is received that does not
                               match the CPM requestor ID or outstanding Tag.
                               The Header is logged.
                            */
    UINT32 ro : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Receiver Overflow: As a non-function specific
                               error, the SR-IOV specification requires that
                               this field be hardwired to all zeros.
                            */
    UINT32 mtlp : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Malformed TLP: As a non-function specific error,
                               the SR-IOV specification requires that this
                               field be hardwired to all zeros.
                            */
    UINT32 ecrcc : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               ECRC Check: As a non-function specific error,
                               the SR-IOV specification requires that this
                               field be hardwired to all zeros.
                            */
    UINT32 ur : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Unsupported Request Error Status: As a receiver,
                               Set whenever an unsupported request is detected.
                               The Header is logged.
                            */
    UINT32 reserved : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 ieunc : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Uncorrectable Internal Error Status: supported
                               by PF only.
                            */
    UINT32 reserved0 : 9;

                            /* Bits[31:23], Access Type=RO, default=0x00000000*/

                            /* Reserved0: */

  } Bits;
  UINT32 Data;

} VPAERUCS_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPAERUCM_IIO_CPM_VF_REG supported on:                                        */
/*      SPRA0 (0x20d01108)                                                      */
/*      SPRB0 (0x20d01108)                                                      */
/*      SPRC0 (0x20d01108)                                                      */
/*      SPRUCC (0x20d01108)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF PCI Express AER Uncorrectable Error Mask Register
*/


#define VPAERUCM_IIO_CPM_VF_REG 0x21320108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved3 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /* Reserved3 */
    UINT32 reserved2 : 3;

                            /* Bits[3:1], Access Type=RO, default=0x00000000*/

                            /* Reserved2 */
    UINT32 dlpe : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Data Link Protocol Error Mask: As a non-function
                               specific error, the SR-IOV specification
                               requires that this field be hardwired to all
                               zeros.
                            */
    UINT32 sdes : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Surprise Down Error Mask : As a non-function
                               specific error, the SR-IOV specification
                               requires that this field be hardwired to all
                               zeros. Not supported.
                            */
    UINT32 reserved1 : 6;

                            /* Bits[11:6], Access Type=RO, default=0x00000000*/

                            /* Reserved1. */
    UINT32 ptlpr : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Poisoned TLP Received Error Mask: The SR-IOV
                               specification requires that this field be
                               hardwired to all zeros and that the setting of
                               this mask in the PF applies to all of the VFs.
                            */
    UINT32 fcpes : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Flow Control Protocol Error Mask: As a non-
                               function specific error, the SR-IOV
                               specification requires that this field be
                               hardwired to all zeros. Not supported.
                            */
    UINT32 ct : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Completion Time Out Error Mask: The SR-IOV
                               specification requires that this field be
                               hardwired to all zeros and that the setting of
                               this mask in the PF applies to all of the VFs.
                            */
    UINT32 ca : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Completer Abort Error Mask : The SR-IOV
                               specification requires that this field be
                               hardwired to all zeros and that the setting of
                               this mask in the applies to all of the VFs.
                            */
    UINT32 ec : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Unexpected Completion Error Mask: The SR-IOV
                               specification requires that this field be
                               hardwired to all zeros and that the setting of
                               this mask in the applies to all of the VFs.
                            */
    UINT32 ro : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Receiver Overflow Error Mask: As a non-function
                               specific error, the SR-IOV specification
                               requires that this field be hardwired to all
                               zeros.
                            */
    UINT32 mtlp : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Malformed TLP Error Mask: As a non-function
                               specific error, the SR-IOV specification
                               requires that this field be hardwired to all
                               zeros.
                            */
    UINT32 ecrcc : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               ECRC Check Error Mask: As a non-function
                               specific error, the SR-IOV specification
                               requires that this field be hardwired to all
                               zeros.
                            */
    UINT32 ur : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Unsupported Request Error Mask: The SR-IOV
                               specification requires that this field be
                               hardwired to all zeros and that the setting of
                               this mask in the applies to all of the VFs.
                            */
    UINT32 acsvem : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Violation Error Mask: The SR-IOV
                               specification requires that this field be
                               hardwired to all zeros and that the setting of
                               this mask in the applies to all of the VFs.
                            */
    UINT32 ieunc : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /* Uncorrectable Internal Error Status: */
    UINT32 reserved0 : 9;

                            /* Bits[31:23], Access Type=RO, default=0x00000000*/

                            /* Reserved0 */

  } Bits;
  UINT32 Data;

} VPAERUCM_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPAERUCSEV_IIO_CPM_VF_REG supported on:                                      */
/*      SPRA0 (0x20d0110c)                                                      */
/*      SPRB0 (0x20d0110c)                                                      */
/*      SPRC0 (0x20d0110c)                                                      */
/*      SPRUCC (0x20d0110c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF PCI Express AER Uncorrectable Error Severity Register
*/


#define VPAERUCSEV_IIO_CPM_VF_REG 0x2132010C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved4 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /* R eserved4 */
    UINT32 reserved3 : 3;

                            /* Bits[3:1], Access Type=RO, default=0x00000000*/

                            /* Reserved3 */
    UINT32 dlpe : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Data Link Protocol Error Severity: As a non-
                               function specific error, the SR-IOV
                               specification requires that this field be
                               hardwired to all zeros.
                            */
    UINT32 sdes : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Surprise Down Error Severity: As a non-function
                               specific error, the SR-IOV specification
                               requires that this field be hardwired to all
                               zeros.Not supported.
                            */
    UINT32 reserved2 : 6;

                            /* Bits[11:6], Access Type=RO, default=0x00000000*/

                            /* Reserved2 */
    UINT32 ptlpr : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Poisoned TLP Received Error Field: The SR-IOV
                               specification requires that this field be
                               hardwired to all zeros and that the setting of
                               this field in the applies to all of the VFs.
                            */
    UINT32 fcpes : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Flow Control Protocol Error Severity: As a non-
                               function specific error, the SR-IOV
                               specification requires that this field be
                               hardwired to all zeros.Not supported.
                            */
    UINT32 ct : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Completion Time Out Error Severity: The SR-IOV
                               specification requires that this field be
                               hardwired to all zeros and that the setting of
                               this field in the applies to all of the VFs.
                            */
    UINT32 ca : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Completer Abort Error Severity: The SR-IOV
                               specification requires that this field be
                               hardwired to all zeros and that the setting of
                               this field in the applies to all of the VFs.
                            */
    UINT32 ec : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Unexpected Completion Error Severity: The SR-IOV
                               specification requires that this field be
                               hardwired to all zeros and that the setting of
                               this field in the applies to all of the VFs.
                            */
    UINT32 ro : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Receiver Overflow Error Severity: As a non-
                               function specific error, the SR-IOV
                               specification requires that this field be
                               hardwired to all zeros.
                            */
    UINT32 mtlp : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Malformed TLP Error Severity: As a non-function
                               specific error, the SR-IOV specification
                               requires that this field be hardwired to all
                               zeros.
                            */
    UINT32 ecrcc : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               ECRC Check Error Severity : As a non-function
                               specific error, the SR-IOV specification
                               requires that this field be hardwired to all
                               zeros.
                            */
    UINT32 ur : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Unsupported Request Error Severity: The SR-IOV
                               specification requires that this field be
                               hardwired to all zeros and that the setting of
                               this field in the applies to all of the VFs.
                            */
    UINT32 aves : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Violation Error Severity: The SR-IOV
                               specification requires that this field be
                               hardwired to all zeros and that the setting of
                               this field in the PF applies to all of the VFs.
                            */
    UINT32 ieunc : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /* Uncorrectable Internal Error status severity */
    UINT32 reserved0 : 9;

                            /* Bits[31:23], Access Type=RO, default=0x00000000*/

                            /* reserved0 */

  } Bits;
  UINT32 Data;

} VPAERUCSEV_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPAERCS_IIO_CPM_VF_REG supported on:                                         */
/*      SPRA0 (0x20d01110)                                                      */
/*      SPRB0 (0x20d01110)                                                      */
/*      SPRC0 (0x20d01110)                                                      */
/*      SPRUCC (0x20d01110)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF PCI Express AER Correctable Error Status Register
*/


#define VPAERCS_IIO_CPM_VF_REG 0x21320110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 res : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Receiver Error Status: As a non-function
                               specific error, the SR-IOV specification
                               requires that this field be hardwired to all
                               zeros.
                            */
    UINT32 reserved2 : 5;

                            /* Bits[5:1], Access Type=RO, default=0x00000000*/

                            /* Reserved 2. */
    UINT32 dlpe : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Bad TLP Status: As a non-function specific
                               error, the SR-IOV specification requires that
                               this field be hardwired to all zeros.
                            */
    UINT32 bdllps : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Bad DLLP Status: As a non-function specific
                               error, the SR-IOV specification requires that
                               this field be hardwired to all zeros.
                            */
    UINT32 rnrs : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               REPLAY NUM Rollover Status: As a non-function
                               specific error, the SR-IOV specification
                               requires that this field be hardwired to all
                               zeros.
                            */
    UINT32 reserved1 : 3;

                            /* Bits[11:9], Access Type=RO, default=0x00000000*/

                            /* Reserved 1. */
    UINT32 rtts : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Replay Timer Timeout Status: As a non-function
                               specific error, the SR-IOV specification
                               requires that this field be hardwired to all
                               zeros.
                            */
    UINT32 anfes : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Advisory Non-Fatal Error Status: */
    UINT32 reserved : 18;

                            /* Bits[31:14], Access Type=RO, default=0x00000000*/

                            /* Reserved . */

  } Bits;
  UINT32 Data;

} VPAERCS_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPAERCM_IIO_CPM_VF_REG supported on:                                         */
/*      SPRA0 (0x20d01114)                                                      */
/*      SPRB0 (0x20d01114)                                                      */
/*      SPRC0 (0x20d01114)                                                      */
/*      SPRUCC (0x20d01114)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF PCI Express AER Correctable Error Mask Register
*/


#define VPAERCM_IIO_CPM_VF_REG 0x21320114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 res : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Receiver Error Mask: The SR-IOV specification
                               requires that this field be treated as Reserved
                               and that the setting of the corresponding bit in
                               the PF will apply to the VFs.
                            */
    UINT32 reserved2 : 5;

                            /* Bits[5:1], Access Type=RO, default=0x00000000*/

                            /* Reserved2. */
    UINT32 dlpe : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Bad TLP Mask: The SR-IOV specification requires
                               that this field be treated as Reserved and that
                               the setting of the corresponding bit in the PF
                               will apply to the VFs.
                            */
    UINT32 bdllps : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Bad DLLP Mask: The SR-IOV specification requires
                               that this field be treated as Reserved and that
                               the setting of the corresponding bit in the PF
                               will apply to the VFs.
                            */
    UINT32 rnrs : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               REPLAY NUM Rollover Mask: The SR-IOV
                               specification requires that this field be
                               treated as Reserved and that the setting of the
                               corresponding bit in the PF will apply to the
                               VFs.
                            */
    UINT32 reserved1 : 3;

                            /* Bits[11:9], Access Type=RO, default=0x00000000*/

                            /* Reserved1. */
    UINT32 rtts : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Replay Timer Timeout Mask: The SR-IOV
                               specification requires that this field be
                               treated as Reserved and that the setting of the
                               corresponding bit in the PF will apply to the
                               VFs.
                            */
    UINT32 anfes : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Advisory Non-Fatal Error Mask: The SR-IOV
                               specification requires that this field be
                               treated as reserved and that the setting of the
                               corresponding bit in the will apply to the VFs.
                            */
    UINT32 reserved : 18;

                            /* Bits[31:14], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} VPAERCM_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPAERCTLCAP_IIO_CPM_VF_REG supported on:                                     */
/*      SPRA0 (0x20d01118)                                                      */
/*      SPRB0 (0x20d01118)                                                      */
/*      SPRC0 (0x20d01118)                                                      */
/*      SPRUCC (0x20d01118)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF PCI Express AER Control and Capability Register
*/


#define VPAERCTLCAP_IIO_CPM_VF_REG 0x21320118

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
                               register.This register will not update until all
                               bits in the ERRUNC STS register are cleared.
                            */
    UINT32 ecrcgc : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               ECRC Generation Capable: Indicates the CPM is
                               not capable of generating ECRC.
                            */
    UINT32 ecrcge : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               ECRC Generation Enable: The SR-IOV specification
                               requires that this field be hardwired to all
                               zeros and that the setting of this field in the
                               PF applies to all of the VFs.
                            */
    UINT32 ecrccc : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               ECRC Check Capable: Indicates the CPM is not
                               capable of checking ECRC.
                            */
    UINT32 ecrcce : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               ECRC Check Enable: The SR-IOV specification
                               requires that this field be hardwired to all
                               zeros and that the setting of this field in the
                               PF applies to all of the VFs.
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

                            /* Reserved */

  } Bits;
  UINT32 Data;

} VPAERCTLCAP_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPAERHDRLOG0_IIO_CPM_VF_REG supported on:                                    */
/*      SPRA0 (0x20d0111c)                                                      */
/*      SPRB0 (0x20d0111c)                                                      */
/*      SPRC0 (0x20d0111c)                                                      */
/*      SPRUCC (0x20d0111c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF PCI Express AER Header Log 0 Register
*/


#define VPAERHDRLOG0_IIO_CPM_VF_REG 0x2132011C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdrlogdw0 : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

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

} VPAERHDRLOG0_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPAERHDRLOG1_IIO_CPM_VF_REG supported on:                                    */
/*      SPRA0 (0x20d01120)                                                      */
/*      SPRB0 (0x20d01120)                                                      */
/*      SPRC0 (0x20d01120)                                                      */
/*      SPRUCC (0x20d01120)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF PCI Express AER Header Log 1 Register
*/


#define VPAERHDRLOG1_IIO_CPM_VF_REG 0x21320120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdrlogdw1 : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

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

} VPAERHDRLOG1_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPAERHDRLOG2_IIO_CPM_VF_REG supported on:                                    */
/*      SPRA0 (0x20d01124)                                                      */
/*      SPRB0 (0x20d01124)                                                      */
/*      SPRC0 (0x20d01124)                                                      */
/*      SPRUCC (0x20d01124)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF PCI Express AER Header Log 2 Register
*/


#define VPAERHDRLOG2_IIO_CPM_VF_REG 0x21320124

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdrlogdw2 : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

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

} VPAERHDRLOG2_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPAERHDRLOG3_IIO_CPM_VF_REG supported on:                                    */
/*      SPRA0 (0x20d01128)                                                      */
/*      SPRB0 (0x20d01128)                                                      */
/*      SPRC0 (0x20d01128)                                                      */
/*      SPRUCC (0x20d01128)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF PCI Express AER Header Log 3 Register
*/


#define VPAERHDRLOG3_IIO_CPM_VF_REG 0x21320128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdrdwlog3 : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               4th DWord of the Header for the PCI Express
                               packet in error (HDRDWLOG3): Once an error is
                               logged in this register, it remains locked for
                               further error logging until the time the
                               software clears the status bit that cause the
                               header log i.e. the error pointer is rearmed to
                               log again.
                            */

  } Bits;
  UINT32 Data;

} VPAERHDRLOG3_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VPAERTPLOG_IIO_CPM_VF_REG supported on:                                      */
/*      SPRA0 (0x20d01138)                                                      */
/*      SPRB0 (0x20d01138)                                                      */
/*      SPRC0 (0x20d01138)                                                      */
/*      SPRUCC (0x20d01138)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PCI Express AER TLP Prefix Log Register
*/


#define VPAERTPLOG_IIO_CPM_VF_REG 0x21320138

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

} VPAERTPLOG_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VARIDHDR_IIO_CPM_VF_REG supported on:                                        */
/*      SPRA0 (0x20d01148)                                                      */
/*      SPRB0 (0x20d01148)                                                      */
/*      SPRC0 (0x20d01148)                                                      */
/*      SPRUCC (0x20d01148)                                                     */
/* Register default value on SPRA0: 0x1B01000E                                  */
/* Register default value on SPRB0: 0x1B01000E                                  */
/* Register default value on SPRC0: 0x1B01000E                                  */
/* Register default value on SPRUCC: 0x1B01000E                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF Alternative Routing ID Capability Header
*/


#define VARIDHDR_IIO_CPM_VF_REG 0x21320148

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 aric : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000000E*/

                            /*
                               PCI Express Extended Capability ID : The PCI SIG
                               has assigned 000Eh to the ARI extended
                               capability.
                            */
    UINT32 aricv : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               Capability Version : This is set to 1h for the
                               most current version of the specification.
                            */
    UINT32 arinco : 12;

                            /* Bits[31:20], Access Type=RO, default=0x000001B0*/

                            /*
                               Next Capability Offset : This field contains
                               pointer to next Capability.
                            */

  } Bits;
  UINT32 Data;

} VARIDHDR_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VARIDCAP_IIO_CPM_VF_REG supported on:                                        */
/*      SPRA0 (0x10d0114c)                                                      */
/*      SPRB0 (0x10d0114c)                                                      */
/*      SPRC0 (0x10d0114c)                                                      */
/*      SPRUCC (0x10d0114c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF ARI Capabilities Register
*/


#define VARIDCAP_IIO_CPM_VF_REG 0x2131014C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 mfvc : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               MFVC Functional Groups Capability : CPM does not
                               supported.
                            */
    UINT16 acs : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Functional Groups Capability : CPM does not
                               support.
                            */
    UINT16 reserved1 : 6;

                            /* Bits[7:2], Access Type=RO, default=0x00000000*/

                            /* Reserved. */
    UINT16 nfn : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*
                               Next Function Number: This field is undefined
                               for VFs in SR-IOV.
                            */

  } Bits;
  UINT16 Data;

} VARIDCAP_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VARIDCTL_IIO_CPM_VF_REG supported on:                                        */
/*      SPRA0 (0x10d0114e)                                                      */
/*      SPRB0 (0x10d0114e)                                                      */
/*      SPRC0 (0x10d0114e)                                                      */
/*      SPRUCC (0x10d0114e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* VF Alternative Routing ID Control Register
*/


#define VARIDCTL_IIO_CPM_VF_REG 0x2131014E

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
                               Function Group : Hardwired to Zero as CPM does
                               not support Function Groups.
                            */
    UINT16 reserved : 9;

                            /* Bits[15:7], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} VARIDCTL_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VACSCAPID_IIO_CPM_VF_REG supported on:                                       */
/*      SPRA0 (0x10d011b0)                                                      */
/*      SPRB0 (0x10d011b0)                                                      */
/*      SPRC0 (0x10d011b0)                                                      */
/*      SPRUCC (0x10d011b0)                                                     */
/* Register default value on SPRA0: 0x0000000D                                  */
/* Register default value on SPRB0: 0x0000000D                                  */
/* Register default value on SPRC0: 0x0000000D                                  */
/* Register default value on SPRUCC: 0x0000000D                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* This is ACS capability for vacscapid
*/


#define VACSCAPID_IIO_CPM_VF_REG 0x213101B0

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

} VACSCAPID_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VACSCVNC_IIO_CPM_VF_REG supported on:                                        */
/*      SPRA0 (0x10d011b2)                                                      */
/*      SPRB0 (0x10d011b2)                                                      */
/*      SPRC0 (0x10d011b2)                                                      */
/*      SPRUCC (0x10d011b2)                                                     */
/* Register default value on SPRA0: 0x00001B81                                  */
/* Register default value on SPRB0: 0x00001B81                                  */
/* Register default value on SPRC0: 0x00001B81                                  */
/* Register default value on SPRUCC: 0x00001B81                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* ACS Capability Version
*/


#define VACSCVNC_IIO_CPM_VF_REG 0x213101B2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 acscv : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000001*/

                            /*
                               Capability Version - This is set to 1h for the
                               SR-IOV.
                            */
    UINT16 acsco : 12;

                            /* Bits[15:4], Access Type=RO, default=0x000001B8*/

                            /* Next Capability Offset is ATS Capability. */

  } Bits;
  UINT16 Data;

} VACSCVNC_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VACSCAP_IIO_CPM_VF_REG supported on:                                         */
/*      SPRA0 (0x20d011b4)                                                      */
/*      SPRB0 (0x20d011b4)                                                      */
/*      SPRC0 (0x20d011b4)                                                      */
/*      SPRUCC (0x20d011b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* ACS Capabilities Register
*/


#define VACSCAP_IIO_CPM_VF_REG 0x213201B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acssv : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Source Validation (V) Hard-wired to zero,
                               not supported in Bell Creek.
                            */
    UINT32 acstb : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Translation Blocking (B) Hard-wired to zero,
                               not supported in Bell Creek..
                            */
    UINT32 acsp2prr : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               ACS P2P Request Redirect (R) Hard-wired to zero,
                               not supported in Bell Creek.
                            */
    UINT32 acsp2pcr : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               ACS P2P Completion Redirect (C) Hard-wired to
                               zero, not supported in Bell Creek..
                            */
    UINT32 acsuf : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Upstream Forwarding (U) Hard-wired to zero,
                               not supported in Bell Creek..
                            */
    UINT32 acsp2pec : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               ACS P2P Egress Control (E) Hard-wired to zero,
                               not supported in Bell Creek..
                            */
    UINT32 acsdtp2p : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Direct Translated P2P (T) Hard-wired to
                               zero, not supported in Bell Creek..
                            */
    UINT32 reserved1 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /* Reserved.1 */
    UINT32 acsecvs : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*
                               Egress Control Vector Size Hard-wired to zero,
                               not supported in Bell Creek..
                            */
    UINT32 acssve : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Source Validation Enable (V) Hard-wired to
                               zero, not supported in Bell Creek..
                            */
    UINT32 acstbe : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Translation Blocking Enable (B) Hard-wired
                               to zero, not supported in Bell Creek.
                            */
    UINT32 acsp2prre : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               ACS P2P Request Redirect Enable (R) Hard-wired
                               to zero, not supported in Bell Creek..
                            */
    UINT32 acsp2pcre : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               ACS P2P Completion Redirect Enable (C) Hard-
                               wired to zero, not supported in Bell Creek..
                            */
    UINT32 acsufe : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Upstream Forwarding Enable (U) Hard-wired to
                               zero, not supported in Bell Creek..
                            */
    UINT32 acsp2pece : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               ACS P2P Egress Control Enable (E) Hard-wired to
                               zero, not supported in Bell Creek..
                            */
    UINT32 acsdtp2pe : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Direct Translated P2P Enable (T) Hard-wired
                               to zero, not supported in Bell Creek..
                            */
    UINT32 reserved : 9;

                            /* Bits[31:23], Access Type=RO, default=0x00000000*/

                            /* Reserved0 */

  } Bits;
  UINT32 Data;

} VACSCAP_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VFATSHDR_IIO_CPM_VF_REG supported on:                                        */
/*      SPRA0 (0x20d011b8)                                                      */
/*      SPRB0 (0x20d011b8)                                                      */
/*      SPRC0 (0x20d011b8)                                                      */
/*      SPRUCC (0x20d011b8)                                                     */
/* Register default value on SPRA0: 0x0001000F                                  */
/* Register default value on SPRB0: 0x0001000F                                  */
/* Register default value on SPRC0: 0x0001000F                                  */
/* Register default value on SPRUCC: 0x0001000F                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* ATS Capability Header
*/


#define VFATSHDR_IIO_CPM_VF_REG 0x213201B8

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

                            /* Bits[31:20], Access Type=RO, default=0x00000000*/

                            /*
                               Next Capability Offset. This is the last
                               Capability.
                            */

  } Bits;
  UINT32 Data;

} VFATSHDR_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VFATSCAP_IIO_CPM_VF_REG supported on:                                        */
/*      SPRA0 (0x10d011bc)                                                      */
/*      SPRB0 (0x10d011bc)                                                      */
/*      SPRC0 (0x10d011bc)                                                      */
/*      SPRUCC (0x10d011bc)                                                     */
/* Register default value on SPRA0: 0x00000060                                  */
/* Register default value on SPRB0: 0x00000060                                  */
/* Register default value on SPRC0: 0x00000060                                  */
/* Register default value on SPRUCC: 0x00000060                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* ATS Capability Register
*/


#define VFATSCAP_IIO_CPM_VF_REG 0x213101BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 iqd : 5;

                            /* Bits[4:0], Access Type=RO, default=0x00000000*/

                            /*
                               Invalidate Queue Depth The number of Invalidate
                               Requests that the Function can accept before
                               putting backpressure on the upstream connection.
                               If 0 0000b, the Function can accept 32
                               Invalidate Requests Hardwired to 0 for VFs.
                               Depth of shared PF input queue.
                            */
    UINT16 ar : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000001*/

                            /*
                               Page Aligned Request If Set, indicates the
                               Untranslated Address is always aligned to a 4096
                               byte boundary. Setting this bit is recommended.
                               This bit permits software to distinguish between
                               implementations compatible with earlier version
                               of this specification that permitted a requester
                               to supply anything in bits [11:2].
                            */
    UINT16 gis : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000001*/

                            /*
                               Global Invalidate Supported - If Set, the
                               Function supports Invalidation Requests that
                               have the Global Invalidate bit Set. If Clear,
                               the Function ignores the Global Invalidate bit
                               in all Invalidate Requests. This bit is 0b if
                               the Function does not support the PASID TLP
                               Prefix.
                            */
    UINT16 reserved1 : 9;

                            /* Bits[15:7], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} VFATSCAP_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VFATSCTL_IIO_CPM_VF_REG supported on:                                        */
/*      SPRA0 (0x10d011be)                                                      */
/*      SPRB0 (0x10d011be)                                                      */
/*      SPRC0 (0x10d011be)                                                      */
/*      SPRUCC (0x10d011be)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* ATS Control Register
*/


#define VFATSCTL_IIO_CPM_VF_REG 0x213101BE

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 stu : 5;

                            /* Bits[4:0], Access Type=RO, default=0x00000000*/

                            /*
                               Smallest Translation Unit (STU) This value
                               indicates to the Function the minimum number of
                               4096-byte blocks that is indicated in a
                               Translation Completions or Invalidate Requests.
                               This is a power of 2 multiplier and the number
                               of blocks is 2STU. A value of 0 0000b indicates
                               one block and a value of 1 1111b indicates 231
                               blocks (or 8 TB total). Default value is 0
                               0000b. Hardwired to 0s for VFs. PF value applies
                               to all VFs.
                            */
    UINT16 rsvd : 10;

                            /* Bits[14:5], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT16 en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Enable - When Set, the Function is enabled to
                               cache translations. Default value is 0b.
                            */

  } Bits;
  UINT16 Data;

} VFATSCTL_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VFFUSECTL4_IIO_CPM_VF_REG supported on:                                      */
/*      SPRA0 (0x20d011c4)                                                      */
/*      SPRB0 (0x20d011c4)                                                      */
/*      SPRC0 (0x20d011c4)                                                      */
/*      SPRUCC (0x20d011c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Fuse Control Register 3
*/


#define VFFUSECTL4_IIO_CPM_VF_REG 0x213201C4

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

} VFFUSECTL4_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* VFFUSECTL5_IIO_CPM_VF_REG supported on:                                      */
/*      SPRA0 (0x20d011c8)                                                      */
/*      SPRB0 (0x20d011c8)                                                      */
/*      SPRC0 (0x20d011c8)                                                      */
/*      SPRUCC (0x20d011c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_vf_i[0]/cpm_csr_vf_i*/
/* Struct generated from SPRA0 BDF: 13_0_1                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Fuse Control Register 3
*/


#define VFFUSECTL5_IIO_CPM_VF_REG 0x213201C8

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

} VFFUSECTL5_IIO_CPM_VF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */
#endif /* _IIO_CPM_VF_h */
