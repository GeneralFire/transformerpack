
/** @file
  S3M_TREG.h

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


#ifndef _S3M_TREG_h
#define _S3M_TREG_h
#include <Base.h>

/* S3M_MAILBOX_FLOW_STATUS_N0_S3M_TREG_REG supported on:                        */
/*      SPRA0 (0x200dc200)                                                      */
/*      SPRB0 (0x200dc200)                                                      */
/*      SPRHBM (0x200dc200)                                                     */
/*      SPRC0 (0x200dc200)                                                      */
/*      SPRMCC (0x200dc200)                                                     */
/*      SPRUCC (0x200dc200)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* WARNING:  This has to be RW/P to be settable by FW. Find what the right write SAI should be.  SAI should not allow setting over SB.   Indicates the status of the flow initiated through S3M_COMMAND register. Not for EDS: Implementing the status out from the hardware when run_busy is set, enables S3M to not interrupt FW until it has completed the flow. We should further filter out any data/command transactions from other IPs/Cores/Threads to ensure we dont allow the FW to be interrupted while doing the flow. This enhancement can be chosen to turn off using the FW. 
*/


#define S3M_MAILBOX_FLOW_STATUS_N0_S3M_TREG_REG 0x2D02C200

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mailbox_busy : 1;

                            /* Bits[0:0], Access Type=RW/V/L, default=0x00000000*/

                            /*
                               Set when the mailbox is granted to the
                               requestor. Cleared when the flow is completed
                               and the status register is read
                            */
    UINT32 mailbox_flow_status_encoding : 3;

                            /* Bits[3:1], Access Type=RW/V/L, default=0x00000000*/

                            /*
                               '000 - Idle '001 - DATA_AUTH_FAILURE '010 -
                               FLOW_FAILURE `011 - FLOW_SUCCESS `111 -
                               UPDATE_IN_PROGRESS
                            */
    UINT32 mailbox_flow_encoding : 28;

                            /* Bits[31:4], Access Type=RW/V/L, default=0x00000000*/

                            /*
                               SOCKET_ONLINING = 0x1 CPU_STRAP_PROVISION = 0x2
                               SSKU_LICENSE_KEY_PROVISION = 0x4
                               SSKU_LICENSE_BLOB_PROVISION = 0x8
                               VABD_OEM_KEY_PROVISION = 0x10
                               VCM_KEY_HASH_PROVISION = 0x20
                               S3M_FW_CFR_PROVISION_NO_SVN_COMMIT = 0x40
                               S3M_FW_CFR_PROVISION_SVN_COMMIT = 0x41
                               PUCODE_CFR_PROVISION_NO_SVN_COMMIT = 0x80
                               PUCODE_CFR_PROVISION_SVN_COMMIT = 0x81
                               VAD_TOKEN_PROCESS = 0x200 ***0x400 is
                               reserved*** SOCKET_OFFLINED_INDICATION = 0x800
                            */

  } Bits;
  UINT32 Data;

} S3M_MAILBOX_FLOW_STATUS_N0_S3M_TREG_STRUCT;

/* S3M_MAILBOX_FLOW_STATUS_N1_S3M_TREG_REG supported on:                        */
/*      SPRA0 (0x200dc204)                                                      */
/*      SPRB0 (0x200dc204)                                                      */
/*      SPRHBM (0x200dc204)                                                     */
/*      SPRC0 (0x200dc204)                                                      */
/*      SPRMCC (0x200dc204)                                                     */
/*      SPRUCC (0x200dc204)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* WARNING:  This has to be RW/P to be settable by FW. Find what the right write SAI should be.  SAI should not allow setting over SB.   Indicates the status of the flow initiated through S3M_COMMAND register. Not for EDS: Implementing the status out from the hardware when run_busy is set, enables S3M to not interrupt FW until it has completed the flow. We should further filter out any data/command transactions from other IPs/Cores/Threads to ensure we dont allow the FW to be interrupted while doing the flow. This enhancement can be chosen to turn off using the FW. 
*/


#define S3M_MAILBOX_FLOW_STATUS_N1_S3M_TREG_REG 0x2D02C204

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mailbox_flow_encoding : 4;

                            /* Bits[3:0], Access Type=RW/V/L, default=0x00000000*/

                            /*
                               SOCKET_ONLINING = 0x1 CPU_STRAP_PROVISION = 0x2
                               SSKU_LICENSE_KEY_PROVISION = 0x4
                               SSKU_LICENSE_BLOB_PROVISION = 0x8
                               VABD_OEM_KEY_PROVISION = 0x10
                               VCM_KEY_HASH_PROVISION = 0x20
                               S3M_FW_CFR_PROVISION_NO_SVN_COMMIT = 0x40
                               S3M_FW_CFR_PROVISION_SVN_COMMIT = 0x41
                               PUCODE_CFR_PROVISION_NO_SVN_COMMIT = 0x80
                               PUCODE_CFR_PROVISION_SVN_COMMIT = 0x81
                               VAD_TOKEN_PROCESS = 0x200 ***0x400 is
                               reserved*** SOCKET_OFFLINED_INDICATION = 0x800
                            */
    UINT32 rsvd : 1;

                            /* Bits[4:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mailbox_lock_timer_expired : 1;

                            /* Bits[5:5], Access Type=RW/V/L, default=0x00000000*/

                            /*
                               Set when the mailbox is not naturally cleared by
                               read of the completion status and the mailbox is
                               force released from the lock to eliminate any
                               flow deadlock scenarios
                            */
    UINT32 rsvd_6 : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} S3M_MAILBOX_FLOW_STATUS_N1_S3M_TREG_STRUCT;

/* S3M_ERR_STS_N0_S3M_TREG_REG supported on:                                    */
/*      SPRA0 (0x200dc218)                                                      */
/*      SPRB0 (0x200dc218)                                                      */
/*      SPRHBM (0x200dc218)                                                     */
/*      SPRC0 (0x200dc218)                                                      */
/*      SPRMCC (0x200dc218)                                                     */
/*      SPRUCC (0x200dc218)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Reports the error status of various hardware components housed by S3M
*/


#define S3M_ERR_STS_N0_S3M_TREG_REG 0x2D02C218

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_fw_err_cor_sts : 1;

                            /* Bits[0:0], Access Type=RW/V/P/L, default=0x00000000*/

                            /* S3M FW ERROR COR STATUS */
    UINT32 s3m_fw_err_nonfatal_sts : 1;

                            /* Bits[1:1], Access Type=RW/V/P/L, default=0x00000000*/

                            /* S3M FW ERROR NONFATAL STATUS */
    UINT32 s3m_fw_err_fatal_sts : 1;

                            /* Bits[2:2], Access Type=RW/V/P/L, default=0x00000000*/

                            /* S3M FW ERROR FATAL STATUS */
    UINT32 s3m_hw_err_cor_sts : 1;

                            /* Bits[3:3], Access Type=RW/V/P/L, default=0x00000000*/

                            /* S3M HW ERROR COR STATUS */
    UINT32 s3m_hw_err_nonfatal_sts : 1;

                            /* Bits[4:4], Access Type=RW/V/P/L, default=0x00000000*/

                            /* S3M HW ERROR NONFATAL STATUS */
    UINT32 s3m_hw_err_fatal_sts : 1;

                            /* Bits[5:5], Access Type=RW/V/P/L, default=0x00000000*/

                            /* S3M HW ERROR FATAL STATUS */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} S3M_ERR_STS_N0_S3M_TREG_STRUCT;

/* S3M_ERR_STS_N1_S3M_TREG_REG supported on:                                    */
/*      SPRA0 (0x200dc21c)                                                      */
/*      SPRB0 (0x200dc21c)                                                      */
/*      SPRHBM (0x200dc21c)                                                     */
/*      SPRC0 (0x200dc21c)                                                      */
/*      SPRMCC (0x200dc21c)                                                     */
/*      SPRUCC (0x200dc21c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Reports the error status of various hardware components housed by S3M
*/


#define S3M_ERR_STS_N1_S3M_TREG_REG 0x2D02C21C

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

} S3M_ERR_STS_N1_S3M_TREG_STRUCT;

/* S3M_NVRAM_CONTENT_AUTH_ERR_STS_N0_S3M_TREG_REG supported on:                 */
/*      SPRA0 (0x200dc220)                                                      */
/*      SPRB0 (0x200dc220)                                                      */
/*      SPRHBM (0x200dc220)                                                     */
/*      SPRC0 (0x200dc220)                                                      */
/*      SPRMCC (0x200dc220)                                                     */
/*      SPRUCC (0x200dc220)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Reports the error status of any FW triggered errors
*/


#define S3M_NVRAM_CONTENT_AUTH_ERR_STS_N0_S3M_TREG_REG 0x2D02C220

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 nvram_s3m_fw_auth_error : 1;

                            /* Bits[0:0], Access Type=RW/V/P/L, default=0x00000000*/

                            /* RSA Authentication Failure of S3M FW image */
    UINT32 nvram_fuse_content_auth_error : 1;

                            /* Bits[1:1], Access Type=RW/V/P/L, default=0x00000000*/

                            /* Fuse offload image ICV check failure */
    UINT32 nvram_pucode_auth_error : 1;

                            /* Bits[2:2], Access Type=RW/V/P/L, default=0x00000000*/

                            /* NVRAM PUCODE AUTHENTICATION ERROR */
    UINT32 nvram_ssku_content_auth_error : 1;

                            /* Bits[3:3], Access Type=RW/V/P/L, default=0x00000000*/

                            /* NVRAM SSKU CONTENT AUTHENTICATION ERROR */
    UINT32 nvram_fuse_decryption_error : 1;

                            /* Bits[4:4], Access Type=RW/V/P/L, default=None*/

                            /* Fuse Decryption Error failure */
    UINT32 nvram_ssku_decryption_error : 1;

                            /* Bits[5:5], Access Type=RW/V/P/L, default=None*/

                            /* SSKU decryption failure */
    UINT32 nvram_pucode_content_decryption_error : 1;

                            /* Bits[6:6], Access Type=RW/V/P/L, default=None*/

                            /* PUCODE content decryption failure */
    UINT32 nvram_vad_content_auth_error : 1;

                            /* Bits[7:7], Access Type=RW/V/P/L, default=None*/

                            /* ICV check failure of VAD pin content region */
    UINT32 nvram_vabd_content_auth_error : 1;

                            /* Bits[8:8], Access Type=RW/V/P/L, default=None*/

                            /* ICV check failure of VAB/VAD region */
    UINT32 reserved : 7;

                            /* Bits[15:9], Access Type=RW/V/P/L, default=0x00000000*/

                            /* Open: Is it OK if this is not sticky? */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} S3M_NVRAM_CONTENT_AUTH_ERR_STS_N0_S3M_TREG_STRUCT;

/* S3M_NVRAM_CONTENT_AUTH_ERR_STS_N1_S3M_TREG_REG supported on:                 */
/*      SPRA0 (0x200dc224)                                                      */
/*      SPRB0 (0x200dc224)                                                      */
/*      SPRHBM (0x200dc224)                                                     */
/*      SPRC0 (0x200dc224)                                                      */
/*      SPRMCC (0x200dc224)                                                     */
/*      SPRUCC (0x200dc224)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Reports the error status of any FW triggered errors
*/


#define S3M_NVRAM_CONTENT_AUTH_ERR_STS_N1_S3M_TREG_REG 0x2D02C224

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

} S3M_NVRAM_CONTENT_AUTH_ERR_STS_N1_S3M_TREG_STRUCT;

/* S3M_MULTI_SKT_N0_S3M_TREG_REG supported on:                                  */
/*      SPRA0 (0x200dc228)                                                      */
/*      SPRB0 (0x200dc228)                                                      */
/*      SPRHBM (0x200dc228)                                                     */
/*      SPRC0 (0x200dc228)                                                      */
/*      SPRMCC (0x200dc228)                                                     */
/*      SPRUCC (0x200dc228)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Reports the multi-socket configuration as observed by the S3M during boot. The register is sticky but will be updated after every partition detection flow. This means a non-responsive socket's socket ID should be cleared
*/


#define S3M_MULTI_SKT_N0_S3M_TREG_REG 0x2D02C228

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 legacy_socket_skt_id : 4;

                            /* Bits[3:0], Access Type=RW/P/L, default=0x00000000*/

                            /* LEGACY SOCKET Socket ID */
    UINT32 sbb1_connected_skt_id : 4;

                            /* Bits[7:4], Access Type=RW/P/L, default=0x00000000*/

                            /* SBB1 CONNECTED Socket ID */
    UINT32 sbb2_connected_skt_id : 4;

                            /* Bits[11:8], Access Type=RW/P/L, default=0x00000000*/

                            /* SBB2 CONNECTED Socket ID */
    UINT32 sbb3_connected_skt_id : 4;

                            /* Bits[15:12], Access Type=RW/P/L, default=0x00000000*/

                            /* SBB3 CONNECTED Socket ID */
    UINT32 sbb4_connected_skt_id : 4;

                            /* Bits[19:16], Access Type=RW/P/L, default=0x00000000*/

                            /* SBB4 CONNECTED Socket ID */
    UINT32 sbb5_connected_skt_id : 4;

                            /* Bits[23:20], Access Type=RW/P/L, default=0x00000000*/

                            /* SBB5 CONNECTED Socket ID */
    UINT32 sbb6_connected_skt_id : 4;

                            /* Bits[27:24], Access Type=RW/P/L, default=0x00000000*/

                            /* SBB6 CONNECTED Socket ID */
    UINT32 sbb7_connected_skt_id : 4;

                            /* Bits[31:28], Access Type=RW/P/L, default=0x00000000*/

                            /* SBB7 CONNECTED Socket ID */

  } Bits;
  UINT32 Data;

} S3M_MULTI_SKT_N0_S3M_TREG_STRUCT;

/* S3M_MULTI_SKT_N1_S3M_TREG_REG supported on:                                  */
/*      SPRA0 (0x200dc22c)                                                      */
/*      SPRB0 (0x200dc22c)                                                      */
/*      SPRHBM (0x200dc22c)                                                     */
/*      SPRC0 (0x200dc22c)                                                      */
/*      SPRMCC (0x200dc22c)                                                     */
/*      SPRUCC (0x200dc22c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Reports the multi-socket configuration as observed by the S3M during boot. The register is sticky but will be updated after every partition detection flow. This means a non-responsive socket's socket ID should be cleared
*/


#define S3M_MULTI_SKT_N1_S3M_TREG_REG 0x2D02C22C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 socket_type : 5;

                            /* Bits[4:0], Access Type=RW/P/L, default=0x00000000*/

                            /* {LEGACY_AGENT, FW_AGENT, 0, DIE_ID[1:0]} */
    UINT32 reserved : 5;

                            /* Bits[9:5], Access Type=RW/P/L, default=0x00000000*/

                            /* Reserved */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} S3M_MULTI_SKT_N1_S3M_TREG_STRUCT;

/* S3M_MSB_STATUS_N0_S3M_TREG_REG supported on:                                 */
/*      SPRA0 (0x200dc230)                                                      */
/*      SPRB0 (0x200dc230)                                                      */
/*      SPRHBM (0x200dc230)                                                     */
/*      SPRC0 (0x200dc230)                                                      */
/*      SPRMCC (0x200dc230)                                                     */
/*      SPRUCC (0x200dc230)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Reports the status of multi-socket handshakes as observed by the S3M during boot
*/


#define S3M_MSB_STATUS_N0_S3M_TREG_REG 0x2D02C230

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sbb1_skt_active : 1;

                            /* Bits[0:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Indication if this specific SBLINK connected
                               socket is active/inactive. Please note that a
                               BIOS indication that a socket specific to SBLINK
                               is offlined can also reset this bit
                            */
    UINT32 sbb1_prtn_rdy_to : 1;

                            /* Bits[1:1], Access Type=RW/P/L, default=0x00000000*/

                            /* Partition Ready messaging caused a timeout */
    UINT32 sbb1_tsc_flow_to : 1;

                            /* Bits[2:2], Access Type=RW/P/L, default=0x00000000*/

                            /* TSC 1588 flow caused a timeout */
    UINT32 sbb1_reset_sync_to : 1;

                            /* Bits[3:3], Access Type=RW/P/L, default=0x00000000*/

                            /* CHILD_RESET_SYNC message caused a timeout */
    UINT32 sbb1_parent_child_link : 1;

                            /* Bits[4:4], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Indicates if the SBLINK is being
                               controlled/mastered by this S3M - Value of '1
                               indicates that the S3M is mastering the link and
                               has gone through partition flow successfully
                            */
    UINT32 sbb1_skt_upd_ack : 1;

                            /* Bits[5:5], Access Type=RW/P/L, default=0x00000000*/

                            /* SKT_UPD_ACK_SB message was received */
    UINT32 sbb1_rsvd : 1;

                            /* Bits[6:6], Access Type=RW/P/L, default=0x00000000*/

                            /* SBB 1 Reserved */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sbb2_skt_active : 1;

                            /* Bits[8:8], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Indication if this specific SBLINK connected
                               socket is active/inactive. Please note that a
                               BIOS indication that a socket specific to SBLINK
                               is offlined can also reset this bit
                            */
    UINT32 sbb2_prtn_rdy_to : 1;

                            /* Bits[9:9], Access Type=RW/P/L, default=0x00000000*/

                            /* Partition Ready messaging caused a timeout */
    UINT32 sbb2_tsc_flow_to : 1;

                            /* Bits[10:10], Access Type=RW/P/L, default=0x00000000*/

                            /* TSC 1588 flow caused a timeout */
    UINT32 sbb2_reset_sync_to : 1;

                            /* Bits[11:11], Access Type=RW/P/L, default=0x00000000*/

                            /* CHILD_RESET_SYNC message caused a timeout */
    UINT32 sbb2_parent_child_link : 1;

                            /* Bits[12:12], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Indicates if the SBLINK is being
                               controlled/mastered by this S3M - Value of '1
                               indicates that the S3M is mastering the link and
                               has gone through partition flow successfully
                            */
    UINT32 sbb2_skt_upd_ack : 1;

                            /* Bits[13:13], Access Type=RW/P/L, default=0x00000000*/

                            /* SKT_UPD_ACK_SB message was received */
    UINT32 sbb2_rsvd : 1;

                            /* Bits[14:14], Access Type=RW/P/L, default=0x00000000*/

                            /* SBB 2 Reserved */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sbb3_skt_active : 1;

                            /* Bits[16:16], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Indication if this specific SBLINK connected
                               socket is active/inactive. Please note that a
                               BIOS indication that a socket specific to SBLINK
                               is offlined can also reset this bit
                            */
    UINT32 sbb3_prtn_rdy_to : 1;

                            /* Bits[17:17], Access Type=RW/P/L, default=0x00000000*/

                            /* Partition Ready messaging caused a timeout */
    UINT32 sbb3_tsc_flow_to : 1;

                            /* Bits[18:18], Access Type=RW/P/L, default=0x00000000*/

                            /* TSC 1588 flow caused a timeout */
    UINT32 sbb3_reset_sync_to : 1;

                            /* Bits[19:19], Access Type=RW/P/L, default=0x00000000*/

                            /* CHILD_RESET_SYNC message caused a timeout */
    UINT32 sbb3_parent_child_link : 1;

                            /* Bits[20:20], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Indicates if the SBLINK is being
                               controlled/mastered by this S3M - Value of '1
                               indicates that the S3M is mastering the link and
                               has gone through partition flow successfully
                            */
    UINT32 sbb3_skt_upd_ack : 1;

                            /* Bits[21:21], Access Type=RW/P/L, default=0x00000000*/

                            /* SKT_UPD_ACK_SB message was received */
    UINT32 sbb3_rsvd : 1;

                            /* Bits[22:22], Access Type=RW/P/L, default=0x00000000*/

                            /* SBB 3 Reserved */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sbb4_skt_active : 1;

                            /* Bits[24:24], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Indication if this specific SBLINK connected
                               socket is active/inactive. Please note that a
                               BIOS indication that a socket specific to SBLINK
                               is offlined can also reset this bit
                            */
    UINT32 sbb4_prtn_rdy_to : 1;

                            /* Bits[25:25], Access Type=RW/P/L, default=0x00000000*/

                            /* Partition Ready messaging caused a timeout */
    UINT32 sbb4_tsc_flow_to : 1;

                            /* Bits[26:26], Access Type=RW/P/L, default=0x00000000*/

                            /* TSC 1588 flow caused a timeout */
    UINT32 sbb4_reset_sync_to : 1;

                            /* Bits[27:27], Access Type=RW/P/L, default=0x00000000*/

                            /* CHILD_RESET_SYNC message caused a timeout */
    UINT32 sbb4_parent_child_link : 1;

                            /* Bits[28:28], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Indicates if the SBLINK is being
                               controlled/mastered by this S3M - Value of '1
                               indicates that the S3M is mastering the link and
                               has gone through partition flow successfully
                            */
    UINT32 sbb4_skt_upd_ack : 1;

                            /* Bits[29:29], Access Type=RW/P/L, default=0x00000000*/

                            /* SKT_UPD_ACK_SB message was received */
    UINT32 sbb4_rsvd : 1;

                            /* Bits[30:30], Access Type=RW/P/L, default=0x00000000*/

                            /* SBB 4 Reserved */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} S3M_MSB_STATUS_N0_S3M_TREG_STRUCT;

/* S3M_MSB_STATUS_N1_S3M_TREG_REG supported on:                                 */
/*      SPRA0 (0x200dc234)                                                      */
/*      SPRB0 (0x200dc234)                                                      */
/*      SPRHBM (0x200dc234)                                                     */
/*      SPRC0 (0x200dc234)                                                      */
/*      SPRMCC (0x200dc234)                                                     */
/*      SPRUCC (0x200dc234)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Reports the status of multi-socket handshakes as observed by the S3M during boot
*/


#define S3M_MSB_STATUS_N1_S3M_TREG_REG 0x2D02C234

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sbb5_skt_active : 1;

                            /* Bits[0:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Indication if this specific SBLINK connected
                               socket is active/inactive. Please note that a
                               BIOS indication that a socket specific to SBLINK
                               is offlined can also reset this bit
                            */
    UINT32 sbb5_prtn_rdy_to : 1;

                            /* Bits[1:1], Access Type=RW/P/L, default=0x00000000*/

                            /* Partition Ready messaging caused a timeout */
    UINT32 sbb5_tsc_flow_to : 1;

                            /* Bits[2:2], Access Type=RW/P/L, default=0x00000000*/

                            /* TSC 1588 flow caused a timeout */
    UINT32 sbb5_reset_sync_to : 1;

                            /* Bits[3:3], Access Type=RW/P/L, default=0x00000000*/

                            /* CHILD_RESET_SYNC message caused a timeout */
    UINT32 sbb5_parent_child_link : 1;

                            /* Bits[4:4], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Indicates if the SBLINK is being
                               controlled/mastered by this S3M - Value of '1
                               indicates that the S3M is mastering the link and
                               has gone through partition flow successfully
                            */
    UINT32 sbb5_skt_upd_ack : 1;

                            /* Bits[5:5], Access Type=RW/P/L, default=0x00000000*/

                            /* SKT_UPD_ACK_SB message was received */
    UINT32 sbb5_rsvd : 1;

                            /* Bits[6:6], Access Type=RW/P/L, default=0x00000000*/

                            /* SBB 5 Reserved */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sbb6_skt_active : 1;

                            /* Bits[8:8], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Indication if this specific SBLINK connected
                               socket is active/inactive. Please note that a
                               BIOS indication that a socket specific to SBLINK
                               is offlined can also reset this bit
                            */
    UINT32 sbb6_prtn_rdy_to : 1;

                            /* Bits[9:9], Access Type=RW/P/L, default=0x00000000*/

                            /* Partition Ready messaging caused a timeout */
    UINT32 sbb6_tsc_flow_to : 1;

                            /* Bits[10:10], Access Type=RW/P/L, default=0x00000000*/

                            /* TSC 1588 flow caused a timeout */
    UINT32 sbb6_reset_sync_to : 1;

                            /* Bits[11:11], Access Type=RW/P/L, default=0x00000000*/

                            /* CHILD_RESET_SYNC message caused a timeout */
    UINT32 sbb6_parent_child_link : 1;

                            /* Bits[12:12], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Indicates if the SBLINK is being
                               controlled/mastered by this S3M - Value of '1
                               indicates that the S3M is mastering the link and
                               has gone through partition flow successfully
                            */
    UINT32 sbb6_skt_upd_ack : 1;

                            /* Bits[13:13], Access Type=RW/P/L, default=0x00000000*/

                            /* SKT_UPD_ACK_SB message was received */
    UINT32 sbb6_rsvd : 1;

                            /* Bits[14:14], Access Type=RW/P/L, default=0x00000000*/

                            /* SBB 6 Reserved */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sbb7_skt_active : 1;

                            /* Bits[16:16], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Indication if this specific SBLINK connected
                               socket is active/inactive. Please note that a
                               BIOS indication that a socket specific to SBLINK
                               is offlined can also reset this bit
                            */
    UINT32 sbb7_prtn_rdy_to : 1;

                            /* Bits[17:17], Access Type=RW/P/L, default=0x00000000*/

                            /* Partition Ready messaging caused a timeout */
    UINT32 sbb7_tsc_flow_to : 1;

                            /* Bits[18:18], Access Type=RW/P/L, default=0x00000000*/

                            /* TSC 1588 flow caused a timeout */
    UINT32 sbb7_reset_sync_to : 1;

                            /* Bits[19:19], Access Type=RW/P/L, default=0x00000000*/

                            /* CHILD_RESET_SYNC message caused a timeout */
    UINT32 sbb7_parent_child_link : 1;

                            /* Bits[20:20], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Indicates if the SBLINK is being
                               controlled/mastered by this S3M - Value of '1
                               indicates that the S3M is mastering the link and
                               has gone through partition flow successfully
                            */
    UINT32 sbb7_skt_upd_ack : 1;

                            /* Bits[21:21], Access Type=RW/P/L, default=0x00000000*/

                            /* SKT_UPD_ACK_SB message was received */
    UINT32 sbb7_rsvd : 1;

                            /* Bits[22:22], Access Type=RW/P/L, default=0x00000000*/

                            /* SBB 7 Reserved */
    UINT32 local_punit_skt_upd_ack : 1;

                            /* Bits[23:23], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               SKT_UPD_ACK legacy message was received from
                               Local Punit
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} S3M_MSB_STATUS_N1_S3M_TREG_STRUCT;

/* S3M_ENABLED_FEATURES_N0_S3M_TREG_REG supported on:                           */
/*      SPRA0 (0x200dc238)                                                      */
/*      SPRB0 (0x200dc238)                                                      */
/*      SPRHBM (0x200dc238)                                                     */
/*      SPRC0 (0x200dc238)                                                      */
/*      SPRMCC (0x200dc238)                                                     */
/*      SPRUCC (0x200dc238)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M features that are enabled. Values of these are reported based on the fuses and any opt-in conditions. 1 indicates an enabled feature
*/


#define S3M_ENABLED_FEATURES_N0_S3M_TREG_REG 0x2D02C238

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 integrated_boot_mode : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Integrated boot is supported */
    UINT32 multi_skt_support : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /* MSB is supported */
    UINT32 reserved : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /* Reserved Field */
    UINT32 ssku : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /* SSKU is supported */
    UINT32 pcode_cfr : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /* PCODE CFR is supported. */
    UINT32 ucode_cfr : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /* UCODE CFR is supported */
    UINT32 s3m_fw_cfr : 1;

                            /* Bits[6:6], Access Type=RW/L, default=0x00000000*/

                            /* S3M FW CFR is supported */
    UINT32 vad : 1;

                            /* Bits[7:7], Access Type=RW/L, default=0x00000000*/

                            /* VAD is enabled */
    UINT32 vab : 1;

                            /* Bits[8:8], Access Type=RW/L, default=0x00000000*/

                            /* VAB or Hash Verification is enabled */
    UINT32 dpp : 1;

                            /* Bits[9:9], Access Type=RW/L, default=0x00000000*/

                            /* DPP support enabled */
    UINT32 cpu_attestation : 1;

                            /* Bits[10:10], Access Type=RW/L, default=0x00000000*/

                            /* CPU Attestation Feature Enabled */
    UINT32 pfr_attestation : 1;

                            /* Bits[11:11], Access Type=RW/L, default=0x00000000*/

                            /* PFR Attestation/Mutual Authentication Enabled */
    UINT32 ssku_attestation : 1;

                            /* Bits[12:12], Access Type=RW/L, default=0x00000000*/

                            /* SSKU Attestation Enabled */
    UINT32 rsvd : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} S3M_ENABLED_FEATURES_N0_S3M_TREG_STRUCT;

/* S3M_ENABLED_FEATURES_N1_S3M_TREG_REG supported on:                           */
/*      SPRA0 (0x200dc23c)                                                      */
/*      SPRB0 (0x200dc23c)                                                      */
/*      SPRHBM (0x200dc23c)                                                     */
/*      SPRC0 (0x200dc23c)                                                      */
/*      SPRMCC (0x200dc23c)                                                     */
/*      SPRUCC (0x200dc23c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M features that are enabled. Values of these are reported based on the fuses and any opt-in conditions. 1 indicates an enabled feature
*/


#define S3M_ENABLED_FEATURES_N1_S3M_TREG_REG 0x2D02C23C

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

} S3M_ENABLED_FEATURES_N1_S3M_TREG_STRUCT;

/* S3M_S3MFW_CFR_SVN_N0_S3M_TREG_REG supported on:                              */
/*      SPRA0 (0x200dc240)                                                      */
/*      SPRB0 (0x200dc240)                                                      */
/*      SPRHBM (0x200dc240)                                                     */
/*      SPRC0 (0x200dc240)                                                      */
/*      SPRMCC (0x200dc240)                                                     */
/*      SPRUCC (0x200dc240)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M FW SVN is reported using this register. 
Open: Is it OK for OS to see the SVNs? We should implement this register in hardware so that S3M FW is not interrupted by Rogue software.Otherwise we do a BOOT_BIOS or BIOS_SMM SAI checking in hardware always and only allow these SAIs to interrupt the FW controller.
*/


#define S3M_S3MFW_CFR_SVN_N0_S3M_TREG_REG 0x2D02C240

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_s3mfw_uncommited_svn : 16;

                            /* Bits[15:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               All '0s indicate invalid/no uncommitted FWs
                               exist
                            */
    UINT32 s3m_s3mfw_uncommited_svn_valid : 1;

                            /* Bits[16:16], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Allocating a valid bit to indicate the SVN field
                               validity
                            */
    UINT32 rsvd : 15;

                            /* Bits[31:17], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} S3M_S3MFW_CFR_SVN_N0_S3M_TREG_STRUCT;

/* S3M_S3MFW_CFR_SVN_N1_S3M_TREG_REG supported on:                              */
/*      SPRA0 (0x200dc244)                                                      */
/*      SPRB0 (0x200dc244)                                                      */
/*      SPRHBM (0x200dc244)                                                     */
/*      SPRC0 (0x200dc244)                                                      */
/*      SPRMCC (0x200dc244)                                                     */
/*      SPRUCC (0x200dc244)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M FW SVN is reported using this register. 
Open: Is it OK for OS to see the SVNs? We should implement this register in hardware so that S3M FW is not interrupted by Rogue software.Otherwise we do a BOOT_BIOS or BIOS_SMM SAI checking in hardware always and only allow these SAIs to interrupt the FW controller.
*/


#define S3M_S3MFW_CFR_SVN_N1_S3M_TREG_REG 0x2D02C244

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_s3mfw_commited_svn : 16;

                            /* Bits[15:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               All '0s indicate invalid/no uncommitted FWs
                               exist. There will always be a value in this once
                               CPLD NVRAM is programmed
                            */
    UINT32 s3m_s3mfw_commited_svn_valid : 1;

                            /* Bits[16:16], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Allocating a valid bit to indicate the SVN field
                               validity
                            */
    UINT32 rsvd : 15;

                            /* Bits[31:17], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} S3M_S3MFW_CFR_SVN_N1_S3M_TREG_STRUCT;

/* S3M_PUCODE_CFR_SVN_N0_S3M_TREG_REG supported on:                             */
/*      SPRA0 (0x200dc248)                                                      */
/*      SPRB0 (0x200dc248)                                                      */
/*      SPRHBM (0x200dc248)                                                     */
/*      SPRC0 (0x200dc248)                                                      */
/*      SPRMCC (0x200dc248)                                                     */
/*      SPRUCC (0x200dc248)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* PCODE SVN is reported using this register. 
Open: Is it OK for OS to see the SVNs? We should implement this register in hardware so that S3M FW is not interrupted by Rogue software.Otherwise we do a BOOT_BIOS or BIOS_SMM SAI checking in hardware always and only allow these SAIs to interrupt the FW controller.
*/


#define S3M_PUCODE_CFR_SVN_N0_S3M_TREG_REG 0x2D02C248

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_pucode_uncommited_svn : 16;

                            /* Bits[15:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               All '0s indicate invalid/no uncommitted FWs
                               exist
                            */
    UINT32 s3m_pucode_uncommited_svn_valid : 1;

                            /* Bits[16:16], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Allocating a valid bit to indicate the SVN field
                               validity
                            */
    UINT32 rsvd : 15;

                            /* Bits[31:17], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} S3M_PUCODE_CFR_SVN_N0_S3M_TREG_STRUCT;

/* S3M_PUCODE_CFR_SVN_N1_S3M_TREG_REG supported on:                             */
/*      SPRA0 (0x200dc24c)                                                      */
/*      SPRB0 (0x200dc24c)                                                      */
/*      SPRHBM (0x200dc24c)                                                     */
/*      SPRC0 (0x200dc24c)                                                      */
/*      SPRMCC (0x200dc24c)                                                     */
/*      SPRUCC (0x200dc24c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* PCODE SVN is reported using this register. 
Open: Is it OK for OS to see the SVNs? We should implement this register in hardware so that S3M FW is not interrupted by Rogue software.Otherwise we do a BOOT_BIOS or BIOS_SMM SAI checking in hardware always and only allow these SAIs to interrupt the FW controller.
*/


#define S3M_PUCODE_CFR_SVN_N1_S3M_TREG_REG 0x2D02C24C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_pucode_commited_svn : 16;

                            /* Bits[15:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               All '0s indicate invalid/no uncommitted FWs
                               exist
                            */
    UINT32 s3m_pucode_commited_svn_valid : 1;

                            /* Bits[16:16], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Allocating a valid bit to indicate the SVN field
                               validity
                            */
    UINT32 rsvd : 15;

                            /* Bits[31:17], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} S3M_PUCODE_CFR_SVN_N1_S3M_TREG_STRUCT;

/* S3M_NVRAM_CODE_SE_SVN_N0_S3M_TREG_REG supported on:                          */
/*      SPRA0 (0x200dc250)                                                      */
/*      SPRB0 (0x200dc250)                                                      */
/*      SPRHBM (0x200dc250)                                                     */
/*      SPRC0 (0x200dc250)                                                      */
/*      SPRMCC (0x200dc250)                                                     */
/*      SPRUCC (0x200dc250)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* SE SVN is reported using this register. 
Open: Is it OK for OS to see the SVNs? We should implement this register in hardware so that S3M FW is not interrupted by Rogue software.Otherwise we do a BOOT_BIOS or BIOS_SMM SAI checking in hardware always and only allow these SAIs to interrupt the FW controller.
*/


#define S3M_NVRAM_CODE_SE_SVN_N0_S3M_TREG_REG 0x2D02C250

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_pucode_se_svn : 16;

                            /* Bits[15:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               This field reports the SE SVN of the *Code that
                               was authenticated and made available to PCODE &
                               UCODE to download
                            */
    UINT32 s3m_s3mfw_se_svn : 16;

                            /* Bits[31:16], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               This field reports the SE SVN of the NVRAM S3M
                               FW code that S3M FW engine is running
                            */

  } Bits;
  UINT32 Data;

} S3M_NVRAM_CODE_SE_SVN_N0_S3M_TREG_STRUCT;

/* S3M_NVRAM_CODE_SE_SVN_N1_S3M_TREG_REG supported on:                          */
/*      SPRA0 (0x200dc254)                                                      */
/*      SPRB0 (0x200dc254)                                                      */
/*      SPRHBM (0x200dc254)                                                     */
/*      SPRC0 (0x200dc254)                                                      */
/*      SPRMCC (0x200dc254)                                                     */
/*      SPRUCC (0x200dc254)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* SE SVN is reported using this register. 
Open: Is it OK for OS to see the SVNs? We should implement this register in hardware so that S3M FW is not interrupted by Rogue software.Otherwise we do a BOOT_BIOS or BIOS_SMM SAI checking in hardware always and only allow these SAIs to interrupt the FW controller.
*/


#define S3M_NVRAM_CODE_SE_SVN_N1_S3M_TREG_REG 0x2D02C254

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

} S3M_NVRAM_CODE_SE_SVN_N1_S3M_TREG_STRUCT;

/* S3M_KEY_PROVISION_STATUS_N0_S3M_TREG_REG supported on:                       */
/*      SPRA0 (0x200dc258)                                                      */
/*      SPRB0 (0x200dc258)                                                      */
/*      SPRHBM (0x200dc258)                                                     */
/*      SPRC0 (0x200dc258)                                                      */
/*      SPRMCC (0x200dc258)                                                     */
/*      SPRUCC (0x200dc258)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Report Customer Key provisioned and License Key provisioned Status. Design note: We should implement this register in hardware so that S3M FW is not interrupted by Rogue software.Otherwise we do a BOOT_BIOS or BIOS_SMM SAI checking in hardware always and only allow these SAIs to interrupt the FW controller.
*/


#define S3M_KEY_PROVISION_STATUS_N0_S3M_TREG_REG 0x2D02C258

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* UNUSED/RESERVED */
    UINT32 ssku_license_key_provisioned : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               OEM key for SSKU feature is provisioned which is
                               used to process the license
                            */
    UINT32 vab_vad_oem_key_provisioned : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               OEM key for vendor authorized features is
                               provisioned
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} S3M_KEY_PROVISION_STATUS_N0_S3M_TREG_STRUCT;

/* S3M_KEY_PROVISION_STATUS_N1_S3M_TREG_REG supported on:                       */
/*      SPRA0 (0x200dc25c)                                                      */
/*      SPRB0 (0x200dc25c)                                                      */
/*      SPRHBM (0x200dc25c)                                                     */
/*      SPRC0 (0x200dc25c)                                                      */
/*      SPRMCC (0x200dc25c)                                                     */
/*      SPRUCC (0x200dc25c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Report Customer Key provisioned and License Key provisioned Status. Design note: We should implement this register in hardware so that S3M FW is not interrupted by Rogue software.Otherwise we do a BOOT_BIOS or BIOS_SMM SAI checking in hardware always and only allow these SAIs to interrupt the FW controller.
*/


#define S3M_KEY_PROVISION_STATUS_N1_S3M_TREG_REG 0x2D02C25C

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

} S3M_KEY_PROVISION_STATUS_N1_S3M_TREG_STRUCT;

/* S3M_PROVISIONING_AUTH_FAILURE_COUNT_N0_S3M_TREG_REG supported on:            */
/*      SPRA0 (0x200dc260)                                                      */
/*      SPRB0 (0x200dc260)                                                      */
/*      SPRHBM (0x200dc260)                                                     */
/*      SPRC0 (0x200dc260)                                                      */
/*      SPRMCC (0x200dc260)                                                     */
/*      SPRUCC (0x200dc260)                                                     */
/* Register default value on SPRA0: 0x02410410                                  */
/* Register default value on SPRB0: 0x02410410                                  */
/* Register default value on SPRHBM: 0x02410410                                 */
/* Register default value on SPRC0: 0x02410410                                  */
/* Register default value on SPRMCC: 0x02410410                                 */
/* Register default value on SPRUCC: 0x02410410                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Captures how many authentication failures will be allowed per feature in a given host power cycle per feature. 
Open: Undecided on the default values being fuse vs CPU straps
*/


#define S3M_PROVISIONING_AUTH_FAILURE_COUNT_N0_S3M_TREG_REG 0x2D02C260

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ssku_license_key_auth_failure_count : 3;

                            /* Bits[2:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Number of times SSKU License Key Authentication
                               Failed in a power cycle
                            */
    UINT32 ssku_license_key_auth_failure_threshold : 3;

                            /* Bits[5:3], Access Type=RW/P/L, default=0x00000002*/

                            /* SSKU License key Authentication Threshold */
    UINT32 ssku_license_auth_failure_count : 3;

                            /* Bits[8:6], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Number of times SSKU License Blob Authentication
                               Failed in a power cycle against the SSKU License
                               key
                            */
    UINT32 ssku_license_auth_failure_threshold : 3;

                            /* Bits[11:9], Access Type=RW/P/L, default=0x00000002*/

                            /*
                               Threshold of SSKU License blob authentication
                               against the SSKU license key
                            */
    UINT32 vabd_key_updates_available : 3;

                            /* Bits[14:12], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Number of available VABD Key provision in a
                               given power cycle
                            */
    UINT32 vabd_key_updates_threshold : 3;

                            /* Bits[17:15], Access Type=RW/P/L, default=0x00000002*/

                            /* Threshold of VABD Key provision */
    UINT32 vad_token_auth_failure_count : 3;

                            /* Bits[20:18], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Shows how many VAD token authentication failed
                               in a given host power cycle.
                            */
    UINT32 vad_token_auth_failure_threshold : 3;

                            /* Bits[23:21], Access Type=RW/P/L, default=0x00000002*/

                            /*
                               Threshold of allowed number of failing VAD Token
                               Authentications
                            */
    UINT32 reserved : 8;

                            /* Bits[31:24], Access Type=RW/P/L, default=0x00000002*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} S3M_PROVISIONING_AUTH_FAILURE_COUNT_N0_S3M_TREG_STRUCT;

/* S3M_PROVISIONING_AUTH_FAILURE_COUNT_N1_S3M_TREG_REG supported on:            */
/*      SPRA0 (0x200dc264)                                                      */
/*      SPRB0 (0x200dc264)                                                      */
/*      SPRHBM (0x200dc264)                                                     */
/*      SPRC0 (0x200dc264)                                                      */
/*      SPRMCC (0x200dc264)                                                     */
/*      SPRUCC (0x200dc264)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Captures how many authentication failures will be allowed per feature in a given host power cycle per feature. 
Open: Undecided on the default values being fuse vs CPU straps
*/


#define S3M_PROVISIONING_AUTH_FAILURE_COUNT_N1_S3M_TREG_REG 0x2D02C264

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

} S3M_PROVISIONING_AUTH_FAILURE_COUNT_N1_S3M_TREG_STRUCT;

/* S3M_PROVISIONING_AVAILABILITY_N0_S3M_TREG_REG supported on:                  */
/*      SPRA0 (0x200dc268)                                                      */
/*      SPRB0 (0x200dc268)                                                      */
/*      SPRHBM (0x200dc268)                                                     */
/*      SPRC0 (0x200dc268)                                                      */
/*      SPRMCC (0x200dc268)                                                     */
/*      SPRUCC (0x200dc268)                                                     */
/* Register default value on SPRA0: 0x12412002                                  */
/* Register default value on SPRB0: 0x12412002                                  */
/* Register default value on SPRHBM: 0x12412002                                 */
/* Register default value on SPRC0: 0x12412002                                  */
/* Register default value on SPRMCC: 0x12412002                                 */
/* Register default value on SPRUCC: 0x12412002                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Captures how many NVRAM updates are available in a given host power cycle per feature. Undecided on the default values being fuse vs CPU straps
*/


#define S3M_PROVISIONING_AVAILABILITY_N0_S3M_TREG_REG 0x2D02C268

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 3;

                            /* Bits[2:0], Access Type=RW/P/L, default=0x00000002*/

                            /* Reserved Field */
    UINT32 rsvd : 9;

                            /* Bits[11:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ucode_cfr_updates_available : 3;

                            /* Bits[14:12], Access Type=RW/P/L, default=0x00000002*/

                            /*
                               UCODE CFR UPDATES AVAILABLE. PCODE & UCODE
                               fields report the same since they are the same
                               image
                            */
    UINT32 ucode_cfr_updates_threshold : 3;

                            /* Bits[17:15], Access Type=RW/P/L, default=0x00000002*/

                            /*
                               UCODE CFR UPDATES THRESHOLD. PCODE & UCODE
                               fields report the same since they are the same
                               image
                            */
    UINT32 ucode_cfr_update_auth_failure_count : 3;

                            /* Bits[20:18], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               UCODE CFR UPDATE AUTH FAILURE COUNT. PCODE &
                               UCODE fields report the same since they are the
                               same image
                            */
    UINT32 ucode_cfr_update_auth_failure_threshold : 3;

                            /* Bits[23:21], Access Type=RW/P/L, default=0x00000002*/

                            /*
                               UCODE CFR UPDATE AUTH FAILURE THRESHOLD. PCODE &
                               UCODE fields report the same since they are the
                               same image
                            */
    UINT32 pcode_cfr_updates_available : 3;

                            /* Bits[26:24], Access Type=RW/P/L, default=0x00000002*/

                            /*
                               PCODE CFR UPDATES AVAILABLE. PCODE & UCODE
                               fields report the same since they are the same
                               image
                            */
    UINT32 pcode_cfr_updates_threshold : 3;

                            /* Bits[29:27], Access Type=RW/P/L, default=0x00000002*/

                            /*
                               PCODE CFR UPDATES THRESHOLD. PCODE & UCODE
                               fields report the same since they are the same
                               image
                            */
    UINT32 pcode_cfr_update_auth_failure_count : 2;

                            /* Bits[31:30], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               PCODE CFR UPDATE AUTH FAILURE COUNT. PCODE &
                               UCODE fields report the same since they are the
                               same image
                            */

  } Bits;
  UINT32 Data;

} S3M_PROVISIONING_AVAILABILITY_N0_S3M_TREG_STRUCT;

/* S3M_PROVISIONING_AVAILABILITY_N1_S3M_TREG_REG supported on:                  */
/*      SPRA0 (0x200dc26c)                                                      */
/*      SPRB0 (0x200dc26c)                                                      */
/*      SPRHBM (0x200dc26c)                                                     */
/*      SPRC0 (0x200dc26c)                                                      */
/*      SPRMCC (0x200dc26c)                                                     */
/*      SPRUCC (0x200dc26c)                                                     */
/* Register default value on SPRA0: 0x00124124                                  */
/* Register default value on SPRB0: 0x00124124                                  */
/* Register default value on SPRHBM: 0x00124124                                 */
/* Register default value on SPRC0: 0x00124124                                  */
/* Register default value on SPRMCC: 0x00124124                                 */
/* Register default value on SPRUCC: 0x00124124                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Captures how many NVRAM updates are available in a given host power cycle per feature. Undecided on the default values being fuse vs CPU straps
*/


#define S3M_PROVISIONING_AVAILABILITY_N1_S3M_TREG_REG 0x2D02C26C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pcode_cfr_update_auth_failure_count : 1;

                            /* Bits[0:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               PCODE CFR UPDATE AUTH FAILURE COUNT. PCODE &
                               UCODE fields report the same since they are the
                               same image
                            */
    UINT32 pcode_cfr_update_auth_failure_threshold : 3;

                            /* Bits[3:1], Access Type=RW/P/L, default=0x00000002*/

                            /*
                               PCODE CFR UPDATE AUTH FAILURE THRESHOLD. PCODE &
                               UCODE fields report the same since they are the
                               same image
                            */
    UINT32 s3m_fw_updates_available : 3;

                            /* Bits[6:4], Access Type=RW/P/L, default=0x00000002*/

                            /*
                               Number of S3M FW attempts available to be
                               provisioned into NVRAM
                            */
    UINT32 s3m_fw_updates_threshold : 3;

                            /* Bits[9:7], Access Type=RW/P/L, default=0x00000002*/

                            /*
                               Threshold of S3M FW CFR attempts available to be
                               provisioned into NVRAM
                            */
    UINT32 s3m_fw_cfr_update_auth_failure_count : 3;

                            /* Bits[12:10], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Number of times S3M FW CFR Authentication Failed
                               in a power cycle
                            */
    UINT32 s3m_fw_cfr_update_auth_failure_threshold : 3;

                            /* Bits[15:13], Access Type=RW/P/L, default=0x00000002*/

                            /* S3M FW CFR Authentication Threshold */
    UINT32 ssku_updates_available : 3;

                            /* Bits[18:16], Access Type=RW/P/L, default=0x00000002*/

                            /*
                               Number of SSKU License Region available to be
                               provisioned into NVRAM
                            */
    UINT32 ssku_updates_threshold : 3;

                            /* Bits[21:19], Access Type=RW/P/L, default=0x00000002*/

                            /*
                               Threshold of SSKU License Region attempts
                               available to be provisioned into NVRAM
                            */
    UINT32 rsvd : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} S3M_PROVISIONING_AVAILABILITY_N1_S3M_TREG_STRUCT;

/* S3M_MAILBOX_GRANT_DATA_N0_S3M_TREG_REG supported on:                         */
/*      SPRA0 (0x200dc278)                                                      */
/*      SPRB0 (0x200dc278)                                                      */
/*      SPRHBM (0x200dc278)                                                     */
/*      SPRC0 (0x200dc278)                                                      */
/*      SPRMCC (0x200dc278)                                                     */
/*      SPRUCC (0x200dc278)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Captures the requestor's information to whom the mailbox was granted/locked to.
*/


#define S3M_MAILBOX_GRANT_DATA_N0_S3M_TREG_REG 0x2D02C278

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dest : 8;

                            /* Bits[7:0], Access Type=RW/V/L, default=0x00000000*/

                            /*
                               Requester PortID[7:0], captured from destination
                               ID field of outgoing completion. Lower 8b of the
                               16b global portID
                            */
    UINT32 source : 8;

                            /* Bits[15:8], Access Type=RW/V/L, default=0x00000000*/

                            /*
                               S3M PortID[7:0] captured from source ID field of
                               outgoing completion. Lower 8b of the 16b global
                               portID
                            */
    UINT32 opcode : 8;

                            /* Bits[23:16], Access Type=RW/V/L, default=0x00000000*/

                            /* Response OPCODE */
    UINT32 tag : 3;

                            /* Bits[26:24], Access Type=RW/V/L, default=0x00000000*/

                            /* Tag captured for outgoing message */
    UINT32 misc : 4;

                            /* Bits[30:27], Access Type=RW/V/L, default=0x00000000*/

                            /* Miscellaneous */
    UINT32 eh : 1;

                            /* Bits[31:31], Access Type=RW/V/L, default=0x00000000*/

                            /* Extended header */

  } Bits;
  UINT32 Data;

} S3M_MAILBOX_GRANT_DATA_N0_S3M_TREG_STRUCT;

/* S3M_MAILBOX_GRANT_DATA_N1_S3M_TREG_REG supported on:                         */
/*      SPRA0 (0x200dc27c)                                                      */
/*      SPRB0 (0x200dc27c)                                                      */
/*      SPRHBM (0x200dc27c)                                                     */
/*      SPRC0 (0x200dc27c)                                                      */
/*      SPRMCC (0x200dc27c)                                                     */
/*      SPRUCC (0x200dc27c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Captures the requestor's information to whom the mailbox was granted/locked to.
*/


#define S3M_MAILBOX_GRANT_DATA_N1_S3M_TREG_REG 0x2D02C27C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dest16 : 8;

                            /* Bits[7:0], Access Type=RW/V/L, default=0x00000000*/

                            /*
                               Requester PortID[15:8], captured from
                               destination ID field of outgoing completion.
                               Upper 8b of the 16b global portID
                            */
    UINT32 source16 : 8;

                            /* Bits[15:8], Access Type=RW/V/L, default=0x00000000*/

                            /*
                               S3M PortID[15:8]. Captured from source ID field
                               of outgoing completion. Upper 8b of the 16b
                               global portID
                            */
    UINT32 bar : 3;

                            /* Bits[18:16], Access Type=RW/V/L, default=0x00000000*/

                            /* BAR field of the mailbox granted IP */
    UINT32 fid : 8;

                            /* Bits[26:19], Access Type=RW/V/L, default=0x00000000*/

                            /* FID field of the mailbox granted IP */
    UINT32 reserved : 5;

                            /* Bits[31:27], Access Type=RW/V/L, default=0x00000000*/

                            /* Reserved Field */

  } Bits;
  UINT32 Data;

} S3M_MAILBOX_GRANT_DATA_N1_S3M_TREG_STRUCT;

/* S3M_NVRAM_UPDATE_LIMIT_N0_S3M_TREG_REG supported on:                         */
/*      SPRA0 (0x200dc280)                                                      */
/*      SPRB0 (0x200dc280)                                                      */
/*      SPRHBM (0x200dc280)                                                     */
/*      SPRC0 (0x200dc280)                                                      */
/*      SPRMCC (0x200dc280)                                                     */
/*      SPRUCC (0x200dc280)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Captures the limit reached per allocated NVRAM region. No updates are allowed once we hit the 90 PCT LIMIT. We should ensure we call out in platform spec that any NVRAM update requires a cold reset with power cycle. Close this Russ
*/


#define S3M_NVRAM_UPDATE_LIMIT_N0_S3M_TREG_REG 0x2D02C280

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vabd_vcm_key_provision_90_pct_lmt_reached : 1;

                            /* Bits[0:0], Access Type=RW/P/L, default=0x00000000*/

                            /* VABD VCM KEY PROVISION 90 PCT LIMIT REACHED */
    UINT32 vabd_vcm_key_provision_75_pct_lmt_reached : 1;

                            /* Bits[1:1], Access Type=RW/P/L, default=0x00000000*/

                            /* VABD VCM KEY PROVISION 75 PCT LIMIT REACHED */
    UINT32 vabd_vcm_key_provision_50_pct_lmt_reached : 1;

                            /* Bits[2:2], Access Type=RW/P/L, default=0x00000000*/

                            /* VABD VCM KEY PROVISION 50 PCT LIMIT REACHED */
    UINT32 s3mfw_provision_90_pct_lmt_reached : 1;

                            /* Bits[3:3], Access Type=RW/P/L, default=0x00000000*/

                            /* S3MFW PROVISION 90 PCT LIMIT REACHED */
    UINT32 s3mfw_provision_75_pct_lmt_reached : 1;

                            /* Bits[4:4], Access Type=RW/P/L, default=0x00000000*/

                            /* S3MFW PROVISION 75 PCT LIMIT REACHED */
    UINT32 s3mfw_provision_50_pct_lmt_reached : 1;

                            /* Bits[5:5], Access Type=RW/P/L, default=0x00000000*/

                            /* S3MFW PROVISION 50 PCT LIMIT REACHED */
    UINT32 pcode_cfr_provision_90_pct_lmt_reached : 1;

                            /* Bits[6:6], Access Type=RW/P/L, default=0x00000000*/

                            /* PCODE CFR PROVISION 90 PCT LIMIT REACHED */
    UINT32 pcode_cfr_provision_75_pct_lmt_reached : 1;

                            /* Bits[7:7], Access Type=RW/P/L, default=0x00000000*/

                            /* PCODE CFR PROVISION 75 PCT LIMIT REACHED */
    UINT32 pcode_cfr_provision_50_pct_lmt_reached : 1;

                            /* Bits[8:8], Access Type=RW/P/L, default=0x00000000*/

                            /* PCODE CFR PROVISION 50 PCT LIMIT REACHED */
    UINT32 ucode_cfr_provision_90_pct_lmt_reached : 1;

                            /* Bits[9:9], Access Type=RW/P/L, default=0x00000000*/

                            /* UCODE CFR PROVISION 90 PCT LIMIT REACHED */
    UINT32 ucode_cfr_provision_75_pct_lmt_reached : 1;

                            /* Bits[10:10], Access Type=RW/P/L, default=0x00000000*/

                            /* UCODE CFR PROVISION 75 PCT LIMIT REACHED */
    UINT32 ucode_cfr_provision_50_pct_lmt_reached : 1;

                            /* Bits[11:11], Access Type=RW/P/L, default=0x00000000*/

                            /* UCODE CFR PROVISION 50 PCT LIMIT REACHED */
    UINT32 ssku_provision_90_pct_lmt_reached : 1;

                            /* Bits[12:12], Access Type=RW/P/L, default=0x00000000*/

                            /* SSKU PROVISION 90 PCT LIMIT REACHED */
    UINT32 ssku_provision_75_pct_lmt_reached : 1;

                            /* Bits[13:13], Access Type=RW/P/L, default=0x00000000*/

                            /* SSKU PROVISION 75 PCT LIMIT REACHED */
    UINT32 ssku_provision_50_pct_lmt_reached : 1;

                            /* Bits[14:14], Access Type=RW/P/L, default=0x00000000*/

                            /* SSKU PROVISION 50 PCT LIMIT REACHED */
    UINT32 temp_candidate_90_pct_lmt_reached : 1;

                            /* Bits[15:15], Access Type=RW/P/L, default=0x00000000*/

                            /* TEMP CANDIDATE 90 PCT LIMIT REACHED */
    UINT32 temp_candidate_75_pct_lmt_reached : 1;

                            /* Bits[16:16], Access Type=RW/P/L, default=0x00000000*/

                            /* TEMP CANDIDATE 75 PCT LIMIT REACHED */
    UINT32 temp_candidate_50_pct_lmt_reached : 1;

                            /* Bits[17:17], Access Type=RW/P/L, default=0x00000000*/

                            /* TEMP CANDIDATE 50 PCT LIMIT REACHED */
    UINT32 reserved : 14;

                            /* Bits[31:18], Access Type=RW/P/L, default=0x00000000*/

                            /* Reserved Field */

  } Bits;
  UINT32 Data;

} S3M_NVRAM_UPDATE_LIMIT_N0_S3M_TREG_STRUCT;

/* S3M_NVRAM_UPDATE_LIMIT_N1_S3M_TREG_REG supported on:                         */
/*      SPRA0 (0x200dc284)                                                      */
/*      SPRB0 (0x200dc284)                                                      */
/*      SPRHBM (0x200dc284)                                                     */
/*      SPRC0 (0x200dc284)                                                      */
/*      SPRMCC (0x200dc284)                                                     */
/*      SPRUCC (0x200dc284)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* Captures the limit reached per allocated NVRAM region. No updates are allowed once we hit the 90 PCT LIMIT. We should ensure we call out in platform spec that any NVRAM update requires a cold reset with power cycle. Close this Russ
*/


#define S3M_NVRAM_UPDATE_LIMIT_N1_S3M_TREG_REG 0x2D02C284

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

} S3M_NVRAM_UPDATE_LIMIT_N1_S3M_TREG_STRUCT;

/* S3M_HW_ERR_FATAL_STATUS_N0_S3M_TREG_REG supported on:                        */
/*      SPRA0 (0x200dc288)                                                      */
/*      SPRB0 (0x200dc288)                                                      */
/*      SPRHBM (0x200dc288)                                                     */
/*      SPRC0 (0x200dc288)                                                      */
/*      SPRMCC (0x200dc288)                                                     */
/*      SPRUCC (0x200dc288)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M_HW_ERR_FATAL_STATUS Reg
*/


#define S3M_HW_ERR_FATAL_STATUS_N0_S3M_TREG_REG 0x2D02C288

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 5;

                            /* Bits[4:0], Access Type=RW/V/P/L, default=0x00000000*/

                            /* RESERVED FIELD */
    UINT32 s3m_cryptos_parity_error : 1;

                            /* Bits[5:5], Access Type=RW/V/P/L, default=0x00000000*/

                            /*
                               HW parity error in the crypto RF, but reported
                               by FW. If FW reports a crypto parity error it
                               must also update the S3M_HW_ERR_FATA_STS bit.
                            */
    UINT32 s3m_cryptos_error : 8;

                            /* Bits[13:6], Access Type=RW/V/P/L, default=0x00000000*/

                            /*
                               HW error in the crypto, but reported by FW. If
                               FW reports a crypto error it must also update
                               the S3M_HW_ERR_FATA_STS bit.
                            */
    UINT32 rsvd : 5;

                            /* Bits[18:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 s3m_arc_dbg_tf_err : 1;

                            /* Bits[19:19], Access Type=RW/V/P/L, default=0x00000000*/

                            /*
                               ARC triple fault error. Updated by HW. If set it
                               it automatically puts IBL bypass mux in bypass
                               mode.
                            */
    UINT32 s3m_arc_ecc_exception : 1;

                            /* Bits[20:20], Access Type=RW/V/P/L, default=0x00000000*/

                            /*
                               ARC double fault error. Updated by HW. If set it
                               it automatically puts IBL bypass mux in bypass
                               mode.
                            */
    UINT32 s3m_arc_ecc_dccm_adr_err : 1;

                            /* Bits[21:21], Access Type=RW/V/P/L, default=0x00000000*/

                            /*
                               ARC ECC address error on DCCM bus. Updated by
                               HW. If set it it automatically puts IBL bypass
                               mux in bypass mode.
                            */
    UINT32 s3m_arc_ecc_dccm_db_err : 1;

                            /* Bits[22:22], Access Type=RW/V/P/L, default=0x00000000*/

                            /*
                               ARC ECC double bit error on DCCM bus. Updated by
                               HW. If set it it automatically puts IBL bypass
                               mux in bypass mode.
                            */
    UINT32 s3m_arc_ecc_iccm_adr_err : 1;

                            /* Bits[23:23], Access Type=RW/V/P/L, default=0x00000000*/

                            /*
                               ARC ECC address error on ICCM bus. Updated by
                               HW. If set it it automatically puts IBL bypass
                               mux in bypass mode.
                            */
    UINT32 s3m_arc_ecc_iccm_db_err : 1;

                            /* Bits[24:24], Access Type=RW/V/P/L, default=0x00000000*/

                            /*
                               ARC ECC double bit error on ICCM bus. Updated by
                               HW.
                            */
    UINT32 s3m_arc_wdt_timeout_err : 1;

                            /* Bits[25:25], Access Type=RW/V/P/L, default=0x00000000*/

                            /*
                               ARC watchdog timer error. Connected to
                               wdt_ext_timeout_r pin on the ARC.. If set it it
                               automatically puts IBL bypass mux in bypass
                               mode.
                            */
    UINT32 s3m_sbb6_parity_error : 1;

                            /* Bits[26:26], Access Type=RW/V/P/L, default=0x00000000*/

                            /* SBL 6 parity error. Updated by HW. */
    UINT32 s3m_sbb5_parity_error : 1;

                            /* Bits[27:27], Access Type=RW/V/P/L, default=0x00000000*/

                            /* SBL 5 parity error. Updated by HW. */
    UINT32 s3m_sbb4_parity_error : 1;

                            /* Bits[28:28], Access Type=RW/V/P/L, default=0x00000000*/

                            /* SBL 4 parity error. Updated by HW. */
    UINT32 s3m_sbb3_parity_error : 1;

                            /* Bits[29:29], Access Type=RW/V/P/L, default=0x00000000*/

                            /* SBL 3 parity error. Updated by HW. */
    UINT32 s3m_sbb2_parity_error : 1;

                            /* Bits[30:30], Access Type=RW/V/P/L, default=0x00000000*/

                            /* SBL 2 parity error. Updated by HW. */
    UINT32 s3m_sbb1_parity_error : 1;

                            /* Bits[31:31], Access Type=RW/V/P/L, default=0x00000000*/

                            /* SBL 1 parity error. Updated by HW. */

  } Bits;
  UINT32 Data;

} S3M_HW_ERR_FATAL_STATUS_N0_S3M_TREG_STRUCT;

/* S3M_HW_ERR_FATAL_STATUS_N1_S3M_TREG_REG supported on:                        */
/*      SPRA0 (0x200dc28c)                                                      */
/*      SPRB0 (0x200dc28c)                                                      */
/*      SPRHBM (0x200dc28c)                                                     */
/*      SPRC0 (0x200dc28c)                                                      */
/*      SPRMCC (0x200dc28c)                                                     */
/*      SPRUCC (0x200dc28c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M_HW_ERR_FATAL_STATUS Reg
*/


#define S3M_HW_ERR_FATAL_STATUS_N1_S3M_TREG_REG 0x2D02C28C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_sbb0_parity_error : 1;

                            /* Bits[0:0], Access Type=RW/V/P/L, default=0x00000000*/

                            /* SBL 0 parity error. Updated by HW. */
    UINT32 s3m_sbb_pch_parity_error : 1;

                            /* Bits[1:1], Access Type=RW/V/P/L, default=0x00000000*/

                            /* SBL PCH parity error. Updated by HW. */
    UINT32 s3m_sbb_punit_parity_error : 1;

                            /* Bits[2:2], Access Type=RW/V/P/L, default=0x00000000*/

                            /* SBL PUNIT parity error. Updated by HW. */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} S3M_HW_ERR_FATAL_STATUS_N1_S3M_TREG_STRUCT;

/* S3M_HW_ERR_COR_STATUS_N0_S3M_TREG_REG supported on:                          */
/*      SPRA0 (0x200dc290)                                                      */
/*      SPRB0 (0x200dc290)                                                      */
/*      SPRHBM (0x200dc290)                                                     */
/*      SPRC0 (0x200dc290)                                                      */
/*      SPRMCC (0x200dc290)                                                     */
/*      SPRUCC (0x200dc290)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M_HW_ERR_COR_STATUS Reg
*/


#define S3M_HW_ERR_COR_STATUS_N0_S3M_TREG_REG 0x2D02C290

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_arc_hw_err_cor_count : 32;

                            /* Bits[31:0], Access Type=RW/V/P/L, default=0x00000000*/

                            /* S3M ARC HW ERROR COR COUNT */

  } Bits;
  UINT32 Data;

} S3M_HW_ERR_COR_STATUS_N0_S3M_TREG_STRUCT;

/* S3M_HW_ERR_COR_STATUS_N1_S3M_TREG_REG supported on:                          */
/*      SPRA0 (0x200dc294)                                                      */
/*      SPRB0 (0x200dc294)                                                      */
/*      SPRHBM (0x200dc294)                                                     */
/*      SPRC0 (0x200dc294)                                                      */
/*      SPRMCC (0x200dc294)                                                     */
/*      SPRUCC (0x200dc294)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M_HW_ERR_COR_STATUS Reg
*/


#define S3M_HW_ERR_COR_STATUS_N1_S3M_TREG_REG 0x2D02C294

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_arc_ecc_adr_sb_err : 1;

                            /* Bits[0:0], Access Type=RW/V/P/L, default=0x00000000*/

                            /* S3M ARC ECC ADR SIDEBAND ERROR */
    UINT32 s3m_arc_ecc_dccm_sb_err : 1;

                            /* Bits[1:1], Access Type=RW/V/P/L, default=0x00000000*/

                            /* S3M ARC ECC DCCM SIDEBAND ERROR */
    UINT32 s3m_arc_ecc_iccm_sb_err : 1;

                            /* Bits[2:2], Access Type=RW/V/P/L, default=0x00000000*/

                            /* S3M ARC ECC ICCM SIDEBAND ERROR */
    UINT32 s3m_crypto_err_count : 16;

                            /* Bits[18:3], Access Type=RW/V/P/L, default=0x00000000*/

                            /* Count of crypto error + crypto parity errors */
    UINT32 reserved : 13;

                            /* Bits[31:19], Access Type=RW/V/P/L, default=0x00000000*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} S3M_HW_ERR_COR_STATUS_N1_S3M_TREG_STRUCT;

/* S3M_FW_ERR_FATAL_STATUS_N0_S3M_TREG_REG supported on:                        */
/*      SPRA0 (0x200dc298)                                                      */
/*      SPRB0 (0x200dc298)                                                      */
/*      SPRHBM (0x200dc298)                                                     */
/*      SPRC0 (0x200dc298)                                                      */
/*      SPRMCC (0x200dc298)                                                     */
/*      SPRUCC (0x200dc298)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M_FW_ERR_FATAL_STATUS Reg
*/


#define S3M_FW_ERR_FATAL_STATUS_N0_S3M_TREG_REG 0x2D02C298

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_misc_fw_err_fatal_sts : 32;

                            /* Bits[31:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               FW misc fatal error status. If FW reports an
                               error they must also update the
                               S3M_FW_ERR_FATA_STS bit.
                            */

  } Bits;
  UINT32 Data;

} S3M_FW_ERR_FATAL_STATUS_N0_S3M_TREG_STRUCT;

/* S3M_FW_ERR_FATAL_STATUS_N1_S3M_TREG_REG supported on:                        */
/*      SPRA0 (0x200dc29c)                                                      */
/*      SPRB0 (0x200dc29c)                                                      */
/*      SPRHBM (0x200dc29c)                                                     */
/*      SPRC0 (0x200dc29c)                                                      */
/*      SPRMCC (0x200dc29c)                                                     */
/*      SPRUCC (0x200dc29c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M_FW_ERR_FATAL_STATUS Reg
*/


#define S3M_FW_ERR_FATAL_STATUS_N1_S3M_TREG_REG 0x2D02C29C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_arc_fw_err_fatal_sts : 32;

                            /* Bits[31:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               FW fatal error status. If FW reports an error
                               they must also update the S3M_FW_ERR_FATA_STS
                               bit.
                            */

  } Bits;
  UINT32 Data;

} S3M_FW_ERR_FATAL_STATUS_N1_S3M_TREG_STRUCT;

/* S3M_TSC_TRANSFER_OFFSET_N0_S3M_TREG_REG supported on:                        */
/*      SPRA0 (0x200dc2a0)                                                      */
/*      SPRB0 (0x200dc2a0)                                                      */
/*      SPRHBM (0x200dc2a0)                                                     */
/*      SPRC0 (0x200dc2a0)                                                      */
/*      SPRMCC (0x200dc2a0)                                                     */
/*      SPRUCC (0x200dc2a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M_TSC_TRANSFER_OFFSET Reg
*/


#define S3M_TSC_TRANSFER_OFFSET_N0_S3M_TREG_REG 0x2D02C2A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_tsc_transfer_offset : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               BIOS configures through strap provisining flow
                               into CPLD NVRAM. Used for any adjustment. BIOS
                               never updates this register directly. It is done
                               through CPU Strap provisioning mailbox command.
                               This is just a report out. RW/L is for hardware
                               implementation purposes only
                            */

  } Bits;
  UINT32 Data;

} S3M_TSC_TRANSFER_OFFSET_N0_S3M_TREG_STRUCT;

/* S3M_TSC_TRANSFER_OFFSET_N1_S3M_TREG_REG supported on:                        */
/*      SPRA0 (0x200dc2a4)                                                      */
/*      SPRB0 (0x200dc2a4)                                                      */
/*      SPRHBM (0x200dc2a4)                                                     */
/*      SPRC0 (0x200dc2a4)                                                      */
/*      SPRMCC (0x200dc2a4)                                                     */
/*      SPRUCC (0x200dc2a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M_TSC_TRANSFER_OFFSET Reg
*/


#define S3M_TSC_TRANSFER_OFFSET_N1_S3M_TREG_REG 0x2D02C2A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_tsc_transfer_offset : 31;

                            /* Bits[30:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               BIOS configures through strap provisining flow
                               into CPLD NVRAM. Used for any adjustment. BIOS
                               never updates this register directly. It is done
                               through CPU Strap provisioning mailbox command.
                               This is just a report out. RW/L is for hardware
                               implementation purposes only
                            */
    UINT32 rsvd : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} S3M_TSC_TRANSFER_OFFSET_N1_S3M_TREG_STRUCT;

/* S3M_INTEG_BOOT_CPU_HW_TIMER_N0_S3M_TREG_REG supported on:                    */
/*      SPRA0 (0x200dc2c0)                                                      */
/*      SPRB0 (0x200dc2c0)                                                      */
/*      SPRHBM (0x200dc2c0)                                                     */
/*      SPRC0 (0x200dc2c0)                                                      */
/*      SPRMCC (0x200dc2c0)                                                     */
/*      SPRUCC (0x200dc2c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M_INTEG_BOOT_CPU_HW_TIMER_CFG Reg
*/


#define S3M_INTEG_BOOT_CPU_HW_TIMER_N0_S3M_TREG_REG 0x2D02C2C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 adr_p1_timer_tov : 8;

                            /* Bits[7:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               ADR Timer Base This field determines the Timeout
                               value used for the ADR timer. The timescale is
                               determined by ADR_TIME_SCALE field. A value of
                               zero bypasses the timer.
                            */
    UINT32 adr_p1_timer_scale : 3;

                            /* Bits[10:8], Access Type=RW/L, default=0x00000000*/

                            /*
                               ADR Timer Scale This field specifies the tick
                               frequency upon which the timer will increment.
                               Encodings: - 000b: 1 us - 001b: 10 us - 010b:
                               100 us - 011b: 1 ms - 100b: 10 ms - 101b: 100 ms
                               - 110b: 1 s - 111b: 10 s
                            */
    UINT32 adr_p2_timer_tov : 8;

                            /* Bits[18:11], Access Type=RW/L, default=0x00000000*/

                            /*
                               ADR Timer Base This field determines the Timeout
                               value used for the ADR timer. The timescale is
                               determined by ADR_P2_TIME_SCALE field. A value
                               of zero bypasses the timer.
                            */
    UINT32 adr_p2_timer_scale : 3;

                            /* Bits[21:19], Access Type=RW/L, default=0x00000000*/

                            /*
                               ADR Timer Scale This field specifies the tick
                               frequency upon which the timer will increment.
                               Encodings: 000b: 1 us - 001b: 10 us - 010b: 100
                               us - 011b: 1 ms - 100b: 10 ms - 101b: 100 ms -
                               110b: 1 s - 111b: 10 s
                            */
    UINT32 reserved : 6;

                            /* Bits[27:22], Access Type=RW/L, default=0x00000000*/

                            /* Reserved */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} S3M_INTEG_BOOT_CPU_HW_TIMER_N0_S3M_TREG_STRUCT;

/* S3M_INTEG_BOOT_CPU_HW_TIMER_N1_S3M_TREG_REG supported on:                    */
/*      SPRA0 (0x200dc2c4)                                                      */
/*      SPRB0 (0x200dc2c4)                                                      */
/*      SPRHBM (0x200dc2c4)                                                     */
/*      SPRC0 (0x200dc2c4)                                                      */
/*      SPRMCC (0x200dc2c4)                                                     */
/*      SPRUCC (0x200dc2c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M_INTEG_BOOT_CPU_HW_TIMER_CFG Reg
*/


#define S3M_INTEG_BOOT_CPU_HW_TIMER_N1_S3M_TREG_REG 0x2D02C2C4

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

} S3M_INTEG_BOOT_CPU_HW_TIMER_N1_S3M_TREG_STRUCT;

/* S3M_ERR_FATAL_MASK_N0_S3M_TREG_REG supported on:                             */
/*      SPRA0 (0x200dc2c8)                                                      */
/*      SPRB0 (0x200dc2c8)                                                      */
/*      SPRHBM (0x200dc2c8)                                                     */
/*      SPRC0 (0x200dc2c8)                                                      */
/*      SPRMCC (0x200dc2c8)                                                     */
/*      SPRUCC (0x200dc2c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M_ERR_FATAL_MASK Reg
*/


#define S3M_ERR_FATAL_MASK_N0_S3M_TREG_REG 0x2D02C2C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_hw_err_fatal_mask : 1;

                            /* Bits[0:0], Access Type=RW/P/L, default=0x00000000*/

                            /* Mask all HW fatal errors */
    UINT32 s3m_fw_err_fatal_mask : 1;

                            /* Bits[1:1], Access Type=RW/P/L, default=0x00000000*/

                            /* Mask all FW fatal errors */
    UINT32 reserved : 6;

                            /* Bits[7:2], Access Type=RW/P/L, default=0x00000000*/

                            /* Reserved */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} S3M_ERR_FATAL_MASK_N0_S3M_TREG_STRUCT;

/* S3M_ERR_FATAL_MASK_N1_S3M_TREG_REG supported on:                             */
/*      SPRA0 (0x200dc2cc)                                                      */
/*      SPRB0 (0x200dc2cc)                                                      */
/*      SPRHBM (0x200dc2cc)                                                     */
/*      SPRC0 (0x200dc2cc)                                                      */
/*      SPRMCC (0x200dc2cc)                                                     */
/*      SPRUCC (0x200dc2cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M_ERR_FATAL_MASK Reg
*/


#define S3M_ERR_FATAL_MASK_N1_S3M_TREG_REG 0x2D02C2CC

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

} S3M_ERR_FATAL_MASK_N1_S3M_TREG_STRUCT;

/* S3M_S3MFW_REVID_N0_S3M_TREG_REG supported on:                                */
/*      SPRA0 (0x200dc2d8)                                                      */
/*      SPRB0 (0x200dc2d8)                                                      */
/*      SPRHBM (0x200dc2d8)                                                     */
/*      SPRC0 (0x200dc2d8)                                                      */
/*      SPRMCC (0x200dc2d8)                                                     */
/*      SPRUCC (0x200dc2d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M FW SVN is reported using this register. 
Open: Is it OK for OS to see the SVNs? We should implement this register in hardware so that S3M FW is not interrupted by Rogue software.Otherwise we do a BOOT_BIOS or BIOS_SMM SAI checking in hardware always and only allow these SAIs to interrupt the FW controller.
*/


#define S3M_S3MFW_REVID_N0_S3M_TREG_REG 0x2D02C2D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_s3mfw_uncommited_revid : 32;

                            /* Bits[31:0], Access Type=RW/P/L, default=0x00000000*/

                            /* RevID of the S3M uncommited FW image */

  } Bits;
  UINT32 Data;

} S3M_S3MFW_REVID_N0_S3M_TREG_STRUCT;

/* S3M_S3MFW_REVID_N1_S3M_TREG_REG supported on:                                */
/*      SPRA0 (0x200dc2dc)                                                      */
/*      SPRB0 (0x200dc2dc)                                                      */
/*      SPRHBM (0x200dc2dc)                                                     */
/*      SPRC0 (0x200dc2dc)                                                      */
/*      SPRMCC (0x200dc2dc)                                                     */
/*      SPRUCC (0x200dc2dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M FW SVN is reported using this register. 
Open: Is it OK for OS to see the SVNs? We should implement this register in hardware so that S3M FW is not interrupted by Rogue software.Otherwise we do a BOOT_BIOS or BIOS_SMM SAI checking in hardware always and only allow these SAIs to interrupt the FW controller.
*/


#define S3M_S3MFW_REVID_N1_S3M_TREG_REG 0x2D02C2DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_s3mfw_commited_revid : 32;

                            /* Bits[31:0], Access Type=RW/P/L, default=0x00000000*/

                            /* RevID of the S3M commited FW image */

  } Bits;
  UINT32 Data;

} S3M_S3MFW_REVID_N1_S3M_TREG_STRUCT;

/* S3M_PUCODE_REVID_N0_S3M_TREG_REG supported on:                               */
/*      SPRA0 (0x200dc2e0)                                                      */
/*      SPRB0 (0x200dc2e0)                                                      */
/*      SPRHBM (0x200dc2e0)                                                     */
/*      SPRC0 (0x200dc2e0)                                                      */
/*      SPRMCC (0x200dc2e0)                                                     */
/*      SPRUCC (0x200dc2e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M FW SVN is reported using this register. 
Open: Is it OK for OS to see the SVNs? We should implement this register in hardware so that S3M FW is not interrupted by Rogue software.Otherwise we do a BOOT_BIOS or BIOS_SMM SAI checking in hardware always and only allow these SAIs to interrupt the FW controller.
*/


#define S3M_PUCODE_REVID_N0_S3M_TREG_REG 0x2D02C2E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_pucode_uncommited_revid : 32;

                            /* Bits[31:0], Access Type=RW/P/L, default=0x00000000*/

                            /* RevID of the uncommited PUCODE NVRAM image */

  } Bits;
  UINT32 Data;

} S3M_PUCODE_REVID_N0_S3M_TREG_STRUCT;

/* S3M_PUCODE_REVID_N1_S3M_TREG_REG supported on:                               */
/*      SPRA0 (0x200dc2e4)                                                      */
/*      SPRB0 (0x200dc2e4)                                                      */
/*      SPRHBM (0x200dc2e4)                                                     */
/*      SPRC0 (0x200dc2e4)                                                      */
/*      SPRMCC (0x200dc2e4)                                                     */
/*      SPRUCC (0x200dc2e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRB0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRHBM Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* SPRC0 Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio    */
/* SPRMCC Register File:    sprsp_top/ibl_arch_treg/ibl_treg/ibl_treg_mmio      */
/* SPRUCC Register File:    sprsp_top/ibl_arch_treg[0]/ibl_treg/ibl_treg_mmio   */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRB0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRHBM Security PolicyGroup: HSP_BOOT                                        */
/* SPRC0 Security PolicyGroup: HSP_BOOT                                         */
/* SPRMCC Security PolicyGroup: HSP_BOOT                                        */
/* SPRUCC Security PolicyGroup: HSP_BOOT                                        */
/* S3M FW SVN is reported using this register. 
Open: Is it OK for OS to see the SVNs? We should implement this register in hardware so that S3M FW is not interrupted by Rogue software.Otherwise we do a BOOT_BIOS or BIOS_SMM SAI checking in hardware always and only allow these SAIs to interrupt the FW controller.
*/


#define S3M_PUCODE_REVID_N1_S3M_TREG_REG 0x2D02C2E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s3m_pucode_commited_revid : 32;

                            /* Bits[31:0], Access Type=RW/P/L, default=0x00000000*/

                            /* RevID of the commited PUCODE NVRAM image */

  } Bits;
  UINT32 Data;

} S3M_PUCODE_REVID_N1_S3M_TREG_STRUCT;
#endif /* _S3M_TREG_h */
