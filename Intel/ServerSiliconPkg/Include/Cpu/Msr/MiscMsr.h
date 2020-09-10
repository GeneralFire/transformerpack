/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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
**/

#ifndef _MISC_MSRS_HEADER_
#define _MISC_MSRS_HEADER_

//
// Bit definition for  EFI_MSR_SMM_PROT_MODE_BASE (0x9D)
//
#define  SMM_PROT_MODE_BASE_PADDR_MASK         0x00000000FFFFFF00ULL

//
// MSR EFI_IA32_MTRR_CAP (0x000000FE)
//
#define   B_EFI_IA32_MTRR_VARIABLE_SUPPORT      0xFF
#define   B_EFI_IA32_MTRR_CAP_FIXED_SUPPORT     BIT8
#define   V_EFI_FIXED_MTRR_NUMBER                     11

// MSR_BIOS_INFO_FLAGS (0x0000011F)
#define   B_MSR_BIOS_INFO_FLAGS_WPE             BIT0

//
// Bit definition for  MSR_FEATURE_CONFIG  (0x13C)
//
#define B_MSR_FEATURE_CONFIG_LOCK                   BIT0
#define MCA_MISC_LSB_MASK (0x3f)

#define MCA_FIRMWARE_UPDATED_BIT                    BIT37
#define MCA_STATUS_ADDRV_BIT                        BIT58

#define MSR_TRACE_HUB_STH_ACPIBAR_BASE                                0x00000080
#define   B_MSR_TRACE_HUB_STH_ACPIBAR_BASE_LOCK                         BIT0
#define   V_MSR_TRACE_HUB_STH_ACPIBAR_BASE_MASK                         0x0003FFFF

//
// SNR Max number of CPU Modules
//
#define SNR_MAX_TMT_MODULES                           0x6

// MSR_MISC_PWR_MGMT                     0x01AA
#define   B_MSR_EIST_HW_COORDINATION_DISABLE          BIT0

#define  SNR_ADDR_SHIFT_BIT                           20
#define  ICX_ADDR_SHIFT_BIT                           12

//
// MSR EFI_CACHE_IA32_MTRR_DEF_TYPE
//
#define   B_EFI_CACHE_MTRR_VALID                BIT11

//
// ICX define wrong location for now, temp fix here.
//
#define   N_MSR_LMCE_ON_BIT                           20
#define   N_MSR_FEATURE_CONTROL_LOCK                  0

#define EFI_MSR_HASWELL_SMM_FEATURE_CONTROL    0x4E0
#define  SMM_FEATURE_CONTROL_LOCK_BIT          BIT0
#define  SMM_CPU_SAVE_EN_BIT                   BIT1
#define  SMM_CODE_CHK_EN_BIT                   BIT2

#define EFI_MSR_HASWELL_SMM_DELAYED            0x4E2
#define EFI_MSR_HASWELL_SMM_BLOCKED            0x4E3

#define MSR_VIRTUAL_MSR_LLC_PREFETCH          0x972
#define   B_VIRTUAL_MSR_LLC_PREFETCH_DISABLE  BIT0
#define   B_RFO_TRAIN_DISABLE                 BIT3
#define   B_ENABLE_DBP_FOR_F                  BIT5
#define   N_ENABLE_DBP_FOR_F                  5
#define   B_BI_2IFU_4_F_VICTIMS_EN            BIT6
#define   N_BI_2IFU_4_F_VICTIMS_EN            6

#define SKX_MSR_CPU_BUSNUMBER                 0x00000300

//
// Structure define used for MSR_PLAT_FRMW_PROT_TRIG_PARAM parsing.
//
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Status : 16;

                            /* Bits[0:15], Access Type=RW, default=0x00000000*/

                            /* Trigger Parameter Status */
    UINT32 Data : 16;

                            /* Bits[16:31], Access Type=RW, default=0x00000000*/

                            /* Trigger Parameter Data */
    UINT32 Terminal : 16;

                            /* Bits[32:47], Access Type=RW, default=0x00000000*/

                            /* Trigger Parameter Terminal */
    UINT32 Reserved48 : 14;

                            /* Bits[48:61], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 SE : 1;

                            /* Bits[62:62], Access Type=RW, default=0x00000000*/

                            /* SE */
    UINT32 Reserved63 : 1;

                            /* Bits[63:63], Access Type=RW, default=0x00000000*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PLAT_FRMW_PROT_TRIG_PARAM_REGISTER;

//
// NOTE: Override ICX_MSR_CORE_UARCH_CTL_REGISTER definition in IcxMsr.h for BIT1.
//       Since IcxMsr.h was generated from CRIFF automatically, we'd better
//       to not update it manually.
//       This override can be removed after new IcxMsr.h is generated from
//       new CRIFF.
//
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 DcuScrubEn : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /* L1 scrubbing enable */
    UINT32 Snpq_Disable_M2i_Rsp : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               when set to 1 mtoi off, scrubbing on
                               internally ML4_CR_SNPQ_DEBUG.SNPQ_DISABLE_M2I_RSP = 1
                            */
    UINT32 Rsvd1 : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} OVERRIDE_ICX_MSR_CORE_UARCH_CTL_REGISTER;

//
// NOTE: Override ICX_MSR_QMC_CNTR_CORRECTION_REGISTER definition in IcxMsr.h.
//       Since IcxMsr.h was generated from CRIFF automatically, we'd better
//       to not update it manually.
//       This override can be removed after new IcxMsr.h is generated from
//       new CRIFF.
//
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ChasPerCluster : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* see desc */
    UINT32 ChaThresholdWithinCluster : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* see desc */
    UINT32 CorrectionFactorHi : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* see desc */
    UINT32 CorrectionFactorLo : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /* see desc */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} OVERRIDE_ICX_MSR_QMC_CNTR_CORRECTION_REGISTER;

#endif  // _MISC_MSRS_HEADER_
