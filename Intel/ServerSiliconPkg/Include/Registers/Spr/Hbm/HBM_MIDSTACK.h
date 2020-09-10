
/** @file
  HBM_MIDSTACK.h

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

/* SPRB0 Security Policy Groups:                                                */

/* SPRHBM Security Policy Groups:                                               */
/* HBMIO                                                                        */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    HOSTIA_BOOT_SAI | OOB_MSM_SAI                                             */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRC0 Security Policy Groups:                                                */

/* SPRMCC Security Policy Groups:                                               */

/* SPRUCC Security Policy Groups:                                               */


#ifndef _HBM_MIDSTACK_h
#define _HBM_MIDSTACK_h
#include <Base.h>

#ifdef SPRHBM_HOST
/* HBMIO_FUSE_TUNING0_CREG_HBM_MIDSTACK_REG supported on:                       */
/*      SPRHBM (0x201082f8)                                                     */
/* Register default value on SPRHBM: 0xA1CAB580                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch01_mcast_dword_aword_creg/mem_hbmio_fuse_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Fuse Controls for tuning HBMIO Analog circuits. Refer Fuse RDL/XLS for more details
*/


#define HBMIO_FUSE_TUNING0_CREG_HBM_MIDSTACK_REG 0x152282F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vref_ctrl : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000080*/

                            /* Vref Setting Control */
    UINT32 dll_bwctrl : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000005*/

                            /* DLL Bandwidth Tuning Control */
    UINT32 dll_cben : 2;

                            /* Bits[13:12], Access Type=RW/P, default=0x00000003*/

                            /* DLL CB Tuning */
    UINT32 vsshi_bias : 2;

                            /* Bits[15:14], Access Type=RW/P, default=0x00000002*/

                            /* VSSHI Bias Control */
    UINT32 vsshi_target : 7;

                            /* Bits[22:16], Access Type=RW/P, default=0x0000004A*/

                            /* VSSHI Target */
    UINT32 dll_picb : 5;

                            /* Bits[27:23], Access Type=RW/P, default=0x00000003*/

                            /* DLL PI CB Tuning */
    UINT32 dq_rxbias : 2;

                            /* Bits[29:28], Access Type=RW/P, default=0x00000002*/

                            /* DQ RX Bias Control */
    UINT32 dqs_rxbias : 2;

                            /* Bits[31:30], Access Type=RW/P, default=0x00000002*/

                            /* DQS RX Bias Control */

  } Bits;
  UINT32 Data;

} HBMIO_FUSE_TUNING0_CREG_HBM_MIDSTACK_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBMIO_FUSE_TUNING1_CREG_HBM_MIDSTACK_REG supported on:                       */
/*      SPRHBM (0x201082fc)                                                     */
/* Register default value on SPRHBM: 0x64992082                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch01_mcast_dword_aword_creg/mem_hbmio_fuse_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Fuse Controls for tuning HBMIO Analog circuits. Refer Fuse RDL/XLS for more details
*/


#define HBMIO_FUSE_TUNING1_CREG_HBM_MIDSTACK_REG 0x152282FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dq_tx_dcc_sel : 6;

                            /* Bits[5:0], Access Type=RW/P, default=0x00000002*/

                            /* DQ TX Duty Cycle Control */
    UINT32 dqs_tx_dcc_sel : 6;

                            /* Bits[11:6], Access Type=RW/P, default=0x00000002*/

                            /* DQS TX Duty Cycle Control */
    UINT32 dq_txoutpu_sel : 5;

                            /* Bits[16:12], Access Type=RW/P, default=0x00000012*/

                            /* DQ TX Driver Pull Up Control */
    UINT32 dq_txoutpd_sel : 5;

                            /* Bits[21:17], Access Type=RW/P, default=0x0000000C*/

                            /* DQ TX Driver Pull Down Control */
    UINT32 dqs_txoutpu_sel : 5;

                            /* Bits[26:22], Access Type=RW/P, default=0x00000012*/

                            /* DQS TX Driver Pull Up Control */
    UINT32 dqs_txoutpd_sel : 5;

                            /* Bits[31:27], Access Type=RW/P, default=0x0000000C*/

                            /* DQS TX Driver Pull Down Control */

  } Bits;
  UINT32 Data;

} HBMIO_FUSE_TUNING1_CREG_HBM_MIDSTACK_STRUCT;
#endif /* (SPRHBM_HOST) */
#endif /* _HBM_MIDSTACK_h */
