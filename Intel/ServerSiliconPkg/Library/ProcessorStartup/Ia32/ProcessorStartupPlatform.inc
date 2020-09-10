; @file ProcessorStartupPlatform.inc
;
; @copyright
; INTEL CONFIDENTIAL
; Copyright 1999 - 2017 Intel Corporation. <BR>
;
; The source code contained or described herein and all documents related to the
; source code ("Material") are owned by Intel Corporation or its suppliers or
; licensors. Title to the Material remains with Intel Corporation or its suppliers
; and licensors. The Material may contain trade secrets and proprietary and
; confidential information of Intel Corporation and its suppliers and licensors,
; and is protected by worldwide copyright and trade secret laws and treaty
; provisions. No part of the Material may be used, copied, reproduced, modified,
; published, uploaded, posted, transmitted, distributed, or disclosed in any way
; without Intel's prior express written permission.
;
; No license under any patent, copyright, trade secret or other intellectual
; property right is granted to or conferred upon you by disclosure or delivery
; of the Materials, either expressly, by implication, inducement, estoppel or
; otherwise. Any license under such intellectual property rights must be
; express and approved by Intel in writing.
;
; Unless otherwise agreed by Intel in writing, you may not remove or alter
; this notice or any other notice embedded in Materials by Intel or
; Intel's suppliers or licensors in any way.
;

%define PCIEX_BASE_ADDRESS 0x80000000          ; MMCFG / PCIe Config Base Address, used as cold boot default only.
                                                            ; Use MMCFG_BASE_ADDRESS in PlatformHost.h file to
                                                            ; define platform choice of PCIe config base address

%define SB_RCBA 0xfed1c000          ; South Bridge PCH RCBA Address

; IO Port 80h  POST CODE used in SEC phase
%define POWER_ON_POST_CODE 0x1
%define MICROCODE_POST_CODE 0x2
%define NEM_CACHE_INIT_POST_CODE 0x3
%define NEM_CACHE_ENABLED_POST_CODE 0x4
%define SBSP_PBSP_BRANCH_POST_CODE 0x5
%define SEC_CPU_INIT_END_POST_CODE 0x6

; IO Port 80h  Fatal Error CODE used in SEC phase
%define NON_INTEL_CPU_ERROR_CODE 0xCA
%define VCU_COMMAND_TIMEOUT_ERROR_CODE 0xCB
%define ILLEGAL_SOCKET_ID_ERROR_CODE 0xCC
%define MCU_LOAD_ERROR_CODE 0xCD
%define MMCFG_NOT_YET_ENABLED_ERROR_CODE 0xCE
%define NEM_NO_FREE_MTRR_ERROR_CODE 0xD0
%define NEM_DATA_RW_TEST_ERROR_CODE 0xD1

%define LOCAL_APIC_BASE 0xfee00000

%define BIT0 0x1
%define BIT1 0x2
%define BIT2 0x4
%define BIT3 0x8
%define BIT4 0x10
%define BIT5 0x20
%define BIT6 0x40
%define BIT7 0x80
%define BIT8 0x100
%define BIT9 0x200
%define BIT10 0x400
%define BIT11 0x800
%define BIT12 0x1000
%define BIT13 0x2000
%define BIT14 0x4000
%define BIT15 0x8000
%define BIT16 0x10000
%define BIT17 0x20000
%define BIT18 0x40000
%define BIT19 0x80000
%define BIT20 0x100000
%define BIT21 0x200000
%define BIT22 0x400000
%define BIT23 0x800000
%define BIT24 0x1000000
%define BIT25 0x2000000
%define BIT26 0x4000000
%define BIT27 0x8000000
%define BIT28 0x10000000
%define BIT29 0x20000000
%define BIT30 0x40000000
%define BIT31 0x80000000

%define MICROCODEFV_PATCHDATA_OFFSET 0x90 ; consider Fv header, padding & FFS header