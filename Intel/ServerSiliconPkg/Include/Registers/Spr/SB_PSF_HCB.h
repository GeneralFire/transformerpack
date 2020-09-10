
/** @file
  SB_PSF_HCB.h

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

/* SPRC0 Security Policy Groups:                                                */

/* SPRMCC Security Policy Groups:                                               */
/* HCX_PSF_2_SAI0                                                               */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | OOB_MSM_SAI                                                             */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | OOB_MSM_SAI                                                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* HCX_PSF_2_SAI1                                                               */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRUCC Security Policy Groups:                                               */


#ifndef _SB_PSF_HCB_h
#define _SB_PSF_HCB_h
#include <Base.h>

#ifdef SPRMCC_HOST
/* PSF_2_BUS_SHADOW_RS0_SB_PSF_HCB_REG supported on:                            */
/*      SPRMCC (0x20000000)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* In Implicit/Substractive decode mode, this register contains the shadowed PCI busnumber where all agents on a PSF segment are enumerated on
        Details: InstName=psf_2, reg_name=psf_2_BUS_SHADOW_RS0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_shdw.i_psf20_shadow_wrapper.i_psf20_bus_num_shadow_reg_rs0, size=8, reset=0, address=0x00000000, mask=0xFFFFFFFF
*/


#define PSF_2_BUS_SHADOW_RS0_SB_PSF_HCB_REG 0x24220000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 busnumber : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, Contains the bus number for a
                               particular root space. The bus number for a root
                               space is captured during each CFGWR0 on that
                               root space.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_BUS_SHADOW_RS0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_BUS_SHADOW_RS3_SB_PSF_HCB_REG supported on:                            */
/*      SPRMCC (0x20000004)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* In Implicit/Substractive decode mode, this register contains the shadowed PCI busnumber where all agents on a PSF segment are enumerated on
        Details: InstName=psf_2, reg_name=psf_2_BUS_SHADOW_RS3 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_shdw.i_psf20_shadow_wrapper.i_psf20_bus_num_shadow_reg_rs1, size=8, reset=0, address=0x00000004, mask=0xFFFFFFFF
*/


#define PSF_2_BUS_SHADOW_RS3_SB_PSF_HCB_REG 0x24220004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 busnumber : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, Contains the bus number for a
                               particular root space. The bus number for a root
                               space is captured during each CFGWR0 on that
                               root space.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_BUS_SHADOW_RS3_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_BUS_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG supported on:  */
/*      SPRMCC (0x20002008)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Controls the PCI secondary and subordinate bus ranges
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_BUS_CPM0_RS0_D6_F0_OFFSET64, address=0x2008
*/


#define PSF_2_AGNT_T1_SHDW_BUS_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG 0x24222008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 primbus : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */
    UINT32 secbus : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */
    UINT32 subbus : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_BUS_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_MEMRANGE_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x20002010)                                                     */
/* Register default value on SPRMCC: 0x0000FFF0                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Defines a 16-bit address range
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_MEMRANGE_CPM0_RS0_D6_F0_OFFSET64, address=0x2010
*/


#define PSF_2_AGNT_T1_SHDW_MEMRANGE_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG 0x24222010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addrbase : 12;

                            /* Bits[15:4], Access Type=RW, default=0x00000FFF*/

                            /* Default value=0xFFF, */
    UINT32 rsvd_16 : 4;

                            /* Bits[19:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addrlimit : 12;

                            /* Bits[31:20], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_MEMRANGE_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x20002014)                                                     */
/* Register default value on SPRMCC: 0x0000FFF0                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Defines a 16-bit address range
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_CPM0_RS0_D6_F0_OFFSET64, address=0x2014
*/


#define PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG 0x24222014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addrbase : 12;

                            /* Bits[15:4], Access Type=RW, default=0x00000FFF*/

                            /* Default value=0xFFF, */
    UINT32 rsvd_16 : 4;

                            /* Bits[19:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addrlimit : 12;

                            /* Bits[31:20], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x20002018)                                                     */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* PCI Base Address Register
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_CPM0_RS0_D6_F0_OFFSET64, address=0x2018
*/


#define PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG 0x24222018

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addrbase : 32;

                            /* Bits[31:0], Access Type=RW, default=0xFFFFFFFF*/

                            /* Default value=0xFFFFFFFF, */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x2000201c)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* PCI Base Address Register
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_CPM0_RS0_D6_F0_OFFSET64, address=0x201C
*/


#define PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG 0x2422201C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addrbase : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PMCSR_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x2000202c)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Controls the power settings of a PCI device
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PMCSR_CPM0_RS0_D6_F0_OFFSET64, address=0x202C
*/


#define PSF_2_AGNT_T1_SHDW_PMCSR_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG 0x2422202C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pwrst : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, 2'b11 */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PMCSR_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PCIEN_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x2000203c)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Controls the T1 PCI memory and IO space settings
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PCIEN_CPM0_RS0_D6_F0_OFFSET64, address=0x203C
*/


#define PSF_2_AGNT_T1_SHDW_PCIEN_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG 0x2422203C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ioen : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, This bit is writable through
                               both IOSF SB interface and through cfgWr
                               transactions on IOSF primary. This bit can be
                               compile time configured to be RO, and fixed to
                               1'b1
                            */
    UINT32 memen : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, This bit is writable through
                               both IOSF SB interface and through cfgWr
                               transactions on IOSF primary. This bit can be
                               compile time configured to be RO, and fixed to
                               1'b1
                            */
    UINT32 rsvd : 6;

                            /* Bits[7:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 fundis : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, When set, transactions
                               targeting this PCI function will not be
                               positively decoded, and cfgWr transactions to
                               this function will not be shadowed. This bit is
                               writable through IOSF SB interface only.
                            */
    UINT32 rsvd_9 : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PCIEN_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_BUS_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG supported on:  */
/*      SPRMCC (0x20002088)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Controls the PCI secondary and subordinate bus ranges
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_BUS_CPM1_RS0_D7_F0_OFFSET65, address=0x2088
*/


#define PSF_2_AGNT_T1_SHDW_BUS_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG 0x24222088

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 primbus : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */
    UINT32 secbus : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */
    UINT32 subbus : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_BUS_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_MEMRANGE_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x20002090)                                                     */
/* Register default value on SPRMCC: 0x0000FFF0                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Defines a 16-bit address range
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_MEMRANGE_CPM1_RS0_D7_F0_OFFSET65, address=0x2090
*/


#define PSF_2_AGNT_T1_SHDW_MEMRANGE_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG 0x24222090

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addrbase : 12;

                            /* Bits[15:4], Access Type=RW, default=0x00000FFF*/

                            /* Default value=0xFFF, */
    UINT32 rsvd_16 : 4;

                            /* Bits[19:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addrlimit : 12;

                            /* Bits[31:20], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_MEMRANGE_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x20002094)                                                     */
/* Register default value on SPRMCC: 0x0000FFF0                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Defines a 16-bit address range
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_CPM1_RS0_D7_F0_OFFSET65, address=0x2094
*/


#define PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG 0x24222094

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addrbase : 12;

                            /* Bits[15:4], Access Type=RW, default=0x00000FFF*/

                            /* Default value=0xFFF, */
    UINT32 rsvd_16 : 4;

                            /* Bits[19:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addrlimit : 12;

                            /* Bits[31:20], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x20002098)                                                     */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* PCI Base Address Register
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_CPM1_RS0_D7_F0_OFFSET65, address=0x2098
*/


#define PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG 0x24222098

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addrbase : 32;

                            /* Bits[31:0], Access Type=RW, default=0xFFFFFFFF*/

                            /* Default value=0xFFFFFFFF, */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x2000209c)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* PCI Base Address Register
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_CPM1_RS0_D7_F0_OFFSET65, address=0x209C
*/


#define PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG 0x2422209C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addrbase : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PMCSR_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x200020ac)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Controls the power settings of a PCI device
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PMCSR_CPM1_RS0_D7_F0_OFFSET65, address=0x20AC
*/


#define PSF_2_AGNT_T1_SHDW_PMCSR_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG 0x242220AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pwrst : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, 2'b11 */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PMCSR_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PCIEN_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x200020bc)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Controls the T1 PCI memory and IO space settings
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PCIEN_CPM1_RS0_D7_F0_OFFSET65, address=0x20BC
*/


#define PSF_2_AGNT_T1_SHDW_PCIEN_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG 0x242220BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ioen : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, This bit is writable through
                               both IOSF SB interface and through cfgWr
                               transactions on IOSF primary. This bit can be
                               compile time configured to be RO, and fixed to
                               1'b1
                            */
    UINT32 memen : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, This bit is writable through
                               both IOSF SB interface and through cfgWr
                               transactions on IOSF primary. This bit can be
                               compile time configured to be RO, and fixed to
                               1'b1
                            */
    UINT32 rsvd : 6;

                            /* Bits[7:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 fundis : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, When set, transactions
                               targeting this PCI function will not be
                               positively decoded, and cfgWr transactions to
                               this function will not be shadowed. This bit is
                               writable through IOSF SB interface only.
                            */
    UINT32 rsvd_9 : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PCIEN_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_BUS_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG supported on:  */
/*      SPRMCC (0x20002108)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Controls the PCI secondary and subordinate bus ranges
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_BUS_HQM0_RS0_D8_F0_OFFSET66, address=0x2108
*/


#define PSF_2_AGNT_T1_SHDW_BUS_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG 0x24222108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 primbus : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */
    UINT32 secbus : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */
    UINT32 subbus : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_BUS_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_MEMRANGE_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x20002110)                                                     */
/* Register default value on SPRMCC: 0x0000FFF0                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Defines a 16-bit address range
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_MEMRANGE_HQM0_RS0_D8_F0_OFFSET66, address=0x2110
*/


#define PSF_2_AGNT_T1_SHDW_MEMRANGE_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG 0x24222110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addrbase : 12;

                            /* Bits[15:4], Access Type=RW, default=0x00000FFF*/

                            /* Default value=0xFFF, */
    UINT32 rsvd_16 : 4;

                            /* Bits[19:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addrlimit : 12;

                            /* Bits[31:20], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_MEMRANGE_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x20002114)                                                     */
/* Register default value on SPRMCC: 0x0000FFF0                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Defines a 16-bit address range
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_HQM0_RS0_D8_F0_OFFSET66, address=0x2114
*/


#define PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG 0x24222114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addrbase : 12;

                            /* Bits[15:4], Access Type=RW, default=0x00000FFF*/

                            /* Default value=0xFFF, */
    UINT32 rsvd_16 : 4;

                            /* Bits[19:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addrlimit : 12;

                            /* Bits[31:20], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x20002118)                                                     */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* PCI Base Address Register
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_HQM0_RS0_D8_F0_OFFSET66, address=0x2118
*/


#define PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG 0x24222118

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addrbase : 32;

                            /* Bits[31:0], Access Type=RW, default=0xFFFFFFFF*/

                            /* Default value=0xFFFFFFFF, */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x2000211c)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* PCI Base Address Register
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_HQM0_RS0_D8_F0_OFFSET66, address=0x211C
*/


#define PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG 0x2422211C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addrbase : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PMCSR_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x2000212c)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Controls the power settings of a PCI device
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PMCSR_HQM0_RS0_D8_F0_OFFSET66, address=0x212C
*/


#define PSF_2_AGNT_T1_SHDW_PMCSR_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG 0x2422212C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pwrst : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, 2'b11 */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PMCSR_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PCIEN_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x2000213c)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Controls the T1 PCI memory and IO space settings
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PCIEN_HQM0_RS0_D8_F0_OFFSET66, address=0x213C
*/


#define PSF_2_AGNT_T1_SHDW_PCIEN_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG 0x2422213C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ioen : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, This bit is writable through
                               both IOSF SB interface and through cfgWr
                               transactions on IOSF primary. This bit can be
                               compile time configured to be RO, and fixed to
                               1'b1
                            */
    UINT32 memen : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, This bit is writable through
                               both IOSF SB interface and through cfgWr
                               transactions on IOSF primary. This bit can be
                               compile time configured to be RO, and fixed to
                               1'b1
                            */
    UINT32 rsvd : 6;

                            /* Bits[7:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 fundis : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, When set, transactions
                               targeting this PCI function will not be
                               positively decoded, and cfgWr transactions to
                               this function will not be shadowed. This bit is
                               writable through IOSF SB interface only.
                            */
    UINT32 rsvd_9 : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PCIEN_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_BUS_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG supported on:  */
/*      SPRMCC (0x20002188)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Controls the PCI secondary and subordinate bus ranges
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_BUS_HQM1_RS0_D9_F0_OFFSET67, address=0x2188
*/


#define PSF_2_AGNT_T1_SHDW_BUS_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG 0x24222188

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 primbus : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */
    UINT32 secbus : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */
    UINT32 subbus : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_BUS_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_MEMRANGE_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x20002190)                                                     */
/* Register default value on SPRMCC: 0x0000FFF0                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Defines a 16-bit address range
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_MEMRANGE_HQM1_RS0_D9_F0_OFFSET67, address=0x2190
*/


#define PSF_2_AGNT_T1_SHDW_MEMRANGE_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG 0x24222190

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addrbase : 12;

                            /* Bits[15:4], Access Type=RW, default=0x00000FFF*/

                            /* Default value=0xFFF, */
    UINT32 rsvd_16 : 4;

                            /* Bits[19:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addrlimit : 12;

                            /* Bits[31:20], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_MEMRANGE_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x20002194)                                                     */
/* Register default value on SPRMCC: 0x0000FFF0                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Defines a 16-bit address range
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_HQM1_RS0_D9_F0_OFFSET67, address=0x2194
*/


#define PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG 0x24222194

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addrbase : 12;

                            /* Bits[15:4], Access Type=RW, default=0x00000FFF*/

                            /* Default value=0xFFF, */
    UINT32 rsvd_16 : 4;

                            /* Bits[19:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addrlimit : 12;

                            /* Bits[31:20], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x20002198)                                                     */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* PCI Base Address Register
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_HQM1_RS0_D9_F0_OFFSET67, address=0x2198
*/


#define PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG 0x24222198

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addrbase : 32;

                            /* Bits[31:0], Access Type=RW, default=0xFFFFFFFF*/

                            /* Default value=0xFFFFFFFF, */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x2000219c)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* PCI Base Address Register
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_HQM1_RS0_D9_F0_OFFSET67, address=0x219C
*/


#define PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG 0x2422219C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addrbase : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PMCSR_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x200021ac)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Controls the power settings of a PCI device
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PMCSR_HQM1_RS0_D9_F0_OFFSET67, address=0x21AC
*/


#define PSF_2_AGNT_T1_SHDW_PMCSR_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG 0x242221AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pwrst : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, 2'b11 */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PMCSR_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGNT_T1_SHDW_PCIEN_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x200021bc)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Controls the T1 PCI memory and IO space settings
        Details: InstName=psf_2, reg_name=PSF_2_AGNT_T1_SHDW_PCIEN_HQM1_RS0_D9_F0_OFFSET67, address=0x21BC
*/


#define PSF_2_AGNT_T1_SHDW_PCIEN_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG 0x242221BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ioen : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, This bit is writable through
                               both IOSF SB interface and through cfgWr
                               transactions on IOSF primary. This bit can be
                               compile time configured to be RO, and fixed to
                               1'b1
                            */
    UINT32 memen : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, This bit is writable through
                               both IOSF SB interface and through cfgWr
                               transactions on IOSF primary. This bit can be
                               compile time configured to be RO, and fixed to
                               1'b1
                            */
    UINT32 rsvd : 6;

                            /* Bits[7:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 fundis : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, When set, transactions
                               targeting this PCI function will not be
                               positively decoded, and cfgWr transactions to
                               this function will not be shadowed. This bit is
                               writable through IOSF SB interface only.
                            */
    UINT32 rsvd_9 : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_AGNT_T1_SHDW_PCIEN_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_PSF_GLOBAL_CONFIG_SB_PSF_HCB_REG supported on:                         */
/*      SPRMCC (0x20004000)                                                     */
/* Register default value on SPRMCC: 0x00018360                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Miscellaneous control bits for the global PSF segment
        Details: InstName=psf_2, reg_name=psf_2_PSF_GLOBAL_CONFIG : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_aon.i_psf20_pwrgt.i_psf20_reg_psf_global_cfg, size=32, reset=0x00018360, address=0x4000, mask=0x0001ffff
*/


#define PSF_2_PSF_GLOBAL_CONFIG_SB_PSF_HCB_REG 0x24224000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rbden : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, 1 */
    UINT32 routetotam : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set, all transactions
                               entering the PSF on any segment will be source-
                               decoded directly to a fixed destination ID,
                               except for transactions originating from this
                               fixed destination ID. This ID can be associated
                               with a PSF port that hosts a Test Access Module
                               (TAM).
                            */
    UINT32 idlenak : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set, all fabric ports on
                               the PSF will NAK any IDLE_REQ from agents
                            */
    UINT32 enlcg : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set to 1, all PSF ports
                               will internally clock gate whenever the ISM is
                               idle, and the main PSF logic will internally
                               clock gate whenever all port ISMs are idle. If
                               set to 0, PSF internal clocks will be
                               permanently on, and any cg_en signal output will
                               remain asserted
                            */
    UINT32 entcg : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set, PSF will use its
                               clkreq/ack signals to request and maintain a
                               clock for a transaction. If set to 0, PSF will
                               permantently assert all clkreq signals.
                            */
    UINT32 enerrmsi : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set, anytime an error is
                               logged in an EHCR register by the Error Handler,
                               a Message with expanded header, with opcode
                               UNCORERCTABLE_ERROR (0x65) is send out from the
                               IOSF SB interface. The endpoint to which this
                               message is sent is compile time configurable
                            */
    UINT32 sleepenable : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to 0, then PSF will
                               never asset Sleep to any retention flops (if
                               present). If set, then PSF may assert Sleep
                               during power gating.
                            */
    UINT32 eneei : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set to 0, none of the
                               ports on this PSF instance will do Early Exit
                               From Idle (EEI). In this case an egress port ISM
                               will only change from IDLE to ACTIVE_REQ when an
                               incoming transaction on any ingress port is
                               decoded and confirmed to go to this egress port.
                               Note that enabling EEI may cause the PSF port to
                               violate IOSF 1.1 section 4.1.2.2 rule 2, since
                               its ISM may remain ACTIVE indefinitely without
                               doing any transactions.
                            */
    UINT32 fastgnten : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to 1, the fast
                               transaction grant feature is enabled, allowing
                               fast PSF ports (such as buffered PSF ports) to
                               receive a grant from the central arbiter as soon
                               as possible. If set to 0, all transaction grants
                               to fast PSF ports are always delayed to match
                               the cmd-to-grant delay of the slowest port on
                               the PSF segment.
                            */
    UINT32 enp2p : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to 1, peer-to-peer
                               transactions (including all source decoded
                               transactions) will be routed to their intended
                               destination. If set to 0, peer-to-peer
                               transactions (including all source decoded
                               transactions) will be routed as follows: If
                               (SOURCE_DECODE) (Route to Error Handler( else if
                               (not downstream) (Route upstream( else (route to
                               Destination(
                            */
    UINT32 encmdpardetingress : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set to 0, command parity
                               detection is disabled. In this case, upon
                               handling a transaction with odd command parity,
                               PSF will not stall any arbiter, and the
                               transaction will not be sent to the Error
                               Handler. This field will only be present if PSF
                               is comfigured for parity functionality.
                            */
    UINT32 encmdpardetegress : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set to 0, command parity
                               detection is disabled. In this case, upon
                               handling a transaction with odd command parity,
                               PSF will not stall any arbiter, and the
                               transaction will not be sent to the Error
                               Handler. This field will only be present if PSF
                               is comfigured for parity functionality.
                            */
    UINT32 enpcg : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set, PSF ports will use
                               its cg_en signals to enable a remote interface
                               clock for a transaction. If set to 0, PSF will
                               permantently assert all cg_en signals.
                            */
    UINT32 disp2ptam : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set, routing of source-
                               decoded peer-2-peer transactions to TAM in
                               routeToTam mode is disabled. This means all
                               source-decoded transactions will always be
                               routed to the intended destination, regardless
                               of routeToTam mode
                            */
    UINT32 envcg : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set, PSF will internally
                               (locally) clock gate all VISa related logic when
                               not in use.
                            */
    UINT32 fabrichangenable : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set, PSF will freeze when
                               command parity error is detected.
                            */
    UINT32 sbparitydef : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, This register bit is
                               connected to the endpoints sb_parity_def strap.
                               If set, IOSF-SB parity checking is disabled. If
                               not set, IOSF-SB parity checking is enabled
                            */
    UINT32 rsvd : 15;

                            /* Bits[31:17], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_PSF_GLOBAL_CONFIG_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_PSF_CLKREQ_HYS_SB_PSF_HCB_REG supported on:                            */
/*      SPRMCC (0x20004004)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Controls the hysteresis value for trunk clock gating
        Details: InstName=psf_2, reg_name=psf_2_PSF_CLKREQ_HYS : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_aon.i_psf20_pwrgt.i_psf20_reg_psf_cg_hsyt, size=16, reset=0x0000, address=0x4004, mask=0xffff
*/


#define PSF_2_PSF_CLKREQ_HYS_SB_PSF_HCB_REG 0x24224004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 clkreqhys : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, This field contains the
                               number of cycles for which a clkreq signal will
                               remain asserted after port ISMs have entered
                               IDLE. These bits are not synchronized, so they
                               should only be written when there are no IOSF
                               Primary transactions ongoing on PSF.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_PSF_CLKREQ_HYS_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_RC_OWNER_RS0_SB_PSF_HCB_REG supported on:                              */
/*      SPRMCC (0x20004008)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register specifies a full PSF port destination ID in the format psf_id:port_group_id:port_id:channel_id.
        Details: InstName=psf_2, reg_name=psf_2_RC_OWNER_RS0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.psfcfg_rs[0].psfcfg_active_rs.i_psf20_reg_psf_rcown, size=32, reset=0x00000000, address=0x4008, mask=0x80ffffff
*/


#define PSF_2_RC_OWNER_RS0_SB_PSF_HCB_REG 0x24224008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 channelid : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */
    UINT32 portid : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, Since 1'b0 indicates source-
                               decode, the port ID needs to be larger than 0
                            */
    UINT32 portgroupid : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, 1'b1 */
    UINT32 psfid : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */
    UINT32 rsvd : 7;

                            /* Bits[30:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 chanmap : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If this bit is set, the
                               ChannelID field is ignored and looked up in the
                               CHANMAP register set that belongs to
                               PortGroupID:PortID
                            */

  } Bits;
  UINT32 Data;

} PSF_2_RC_OWNER_RS0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_RC_OWNER_RS3_SB_PSF_HCB_REG supported on:                              */
/*      SPRMCC (0x2000400c)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register specifies a full PSF port destination ID in the format psf_id:port_group_id:port_id:channel_id.
        Details: InstName=psf_2, reg_name=psf_2_RC_OWNER_RS3 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.psfcfg_rs[1].psfcfg_active_rs.i_psf20_reg_psf_rcown, size=32, reset=0x00000000, address=0x400c, mask=0x80ffffff
*/


#define PSF_2_RC_OWNER_RS3_SB_PSF_HCB_REG 0x2422400C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 channelid : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */
    UINT32 portid : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, Since 1'b0 indicates source-
                               decode, the port ID needs to be larger than 0
                            */
    UINT32 portgroupid : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, 1'b1 */
    UINT32 psfid : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */
    UINT32 rsvd : 7;

                            /* Bits[30:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 chanmap : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If this bit is set, the
                               ChannelID field is ignored and looked up in the
                               CHANMAP register set that belongs to
                               PortGroupID:PortID
                            */

  } Bits;
  UINT32 Data;

} PSF_2_RC_OWNER_RS3_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_ROOTSPACE_CONFIG_RS0_SB_PSF_HCB_REG supported on:                      */
/*      SPRMCC (0x20004010)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Miscellaneous control bits for a Root Space
        Details: InstName=psf_2, reg_name=psf_2_ROOTSPACE_CONFIG_RS0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.rscfg_num_rs[0].i_psf20_reg_psf_rscfg, size=2, reset=0x2, address=0x4010, mask=0x3
*/


#define PSF_2_ROOTSPACE_CONFIG_RS0_SB_PSF_HCB_REG 0x24224010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vtden : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set Intel Virtualization
                               Technology for Directed I/O is enabled for this
                               root space. This bit should reflect the system-
                               wide vt-d setting, and is typically maintained
                               by BIOS
                            */
    UINT32 enaddrp2p : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set, address-based p2p
                               transactions are allowed for this root space.
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_ROOTSPACE_CONFIG_RS0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_ROOTSPACE_CONFIG_RS3_SB_PSF_HCB_REG supported on:                      */
/*      SPRMCC (0x20004014)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Miscellaneous control bits for a Root Space
        Details: InstName=psf_2, reg_name=psf_2_ROOTSPACE_CONFIG_RS3 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.rscfg_num_rs[1].i_psf20_reg_psf_rscfg, size=2, reset=0x2, address=0x4014, mask=0x3
*/


#define PSF_2_ROOTSPACE_CONFIG_RS3_SB_PSF_HCB_REG 0x24224014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vtden : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set Intel Virtualization
                               Technology for Directed I/O is enabled for this
                               root space. This bit should reflect the system-
                               wide vt-d setting, and is typically maintained
                               by BIOS
                            */
    UINT32 enaddrp2p : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set, address-based p2p
                               transactions are allowed for this root space.
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_ROOTSPACE_CONFIG_RS3_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_PSF_PORT_CONFIG_PG0_PORT0_SB_PSF_HCB_REG supported on:                 */
/*      SPRMCC (0x20004018)                                                     */
/* Register default value on SPRMCC: 0x00000038                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Miscellaneous control bits for individual PSF ports on the segment
        Details: InstName=psf_2, reg_name=psf_2_PSF_PORT_CONFIG_PG0_PORT0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.portcfg_pg[0].port[0].gen.i_psf20_reg_psf_portcfg, size=9, reset=0x038, address=0x4018, mask=0x07e
*/


#define PSF_2_PSF_PORT_CONFIG_PG0_PORT0_SB_PSF_HCB_REG 0x24224018

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cplpreallocen : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the port will not
                               send out a NP request unless there are
                               sufficient resources for completions that will
                               be returned
                            */
    UINT32 ingressfro : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set, IOSF relaxed ordering
                               will be forced for incoming transactions,
                               regardless of the relaxed ordering (RO)
                               attribute on the IOSF interface.
                            */
    UINT32 egressfro : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set, IOSF relaxed ordering
                               will be forced for outgoing transactions,
                               regardless of the relaxed ordering (RO)
                               attribute on the IOSF interface. Egress Force
                               Relax Ordering is only applicable for PSF port
                               types with buffering (agent port, buffered
                               fabric port). For other ports, ordering is
                               affecting only the ingress PSF port.
                            */
    UINT32 instfwd : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to '1', an ingress
                               buffered PSF port use full store-and-forward.
                               The ingress port will wait until all the
                               required cmd and data for the transaction is
                               available in the port buffers, before initiating
                               a transaction to the PSF Central Arbiter. If set
                               to '0', the port will initiate a transaction
                               immediately when the cmd is received. This bit
                               can be set to 0 when an ingress port connects to
                               an interface on the same or faster clock domain
                            */
    UINT32 egstfwd : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to '1', an egress
                               buffered PSF port use full store-and-forward.
                               The egress port will wait until all the required
                               cmd and data for the transaction is available in
                               the port buffers, before initiating a
                               transaction to an external agent. If set to '0',
                               the port will initiate a transaction immediately
                               when the cmd is received. This bit can be set to
                               0 when an egress port connects to an interface
                               on the same or faster clock domain
                            */
    UINT32 ismcpltracken : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to '1', the PSF port
                               ISM(s) will not return to IDLE unless all
                               outstanding NP read transactions sent from an
                               egress port have been followed by their
                               corresponding CPL transactions on the ingress
                               port
                            */
    UINT32 earlcredrtrn : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set to '1', an ingress
                               target port will return cmd and data credit as
                               soon as the transaction is granted by the PSF
                               central arbiter. If set to '0', the credit is
                               returned only when the full transaction is done.
                               This bit can be set to 1 when an ingress target
                               port connects to an interface on the same or
                               slower clock domain
                            */
    UINT32 endatapardetingress : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, If set to '1', ingress data
                               parity detection is enabled, and the number of
                               ones on the data and dparity signals on an
                               fabric master interface or an agent target
                               interface is an odd number. PSF will flag a
                               dparity error to the error handler
                            */
    UINT32 endatapardetegress : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, If set to '1', egress data
                               parity detection is enabled, and the number of
                               ones on the data and dparity signals on an
                               fabric target interface or an agent master
                               interface is an odd number. PSF will flag a
                               dparity error to the error handler.
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_PSF_PORT_CONFIG_PG0_PORT0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_PSF_PORT_CONFIG_PG1_PORT0_SB_PSF_HCB_REG supported on:                 */
/*      SPRMCC (0x2000401c)                                                     */
/* Register default value on SPRMCC: 0x00000038                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Miscellaneous control bits for individual PSF ports on the segment
        Details: InstName=psf_2, reg_name=psf_2_PSF_PORT_CONFIG_PG1_PORT0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.portcfg_pg[1].port[0].gen.i_psf20_reg_psf_portcfg, size=9, reset=0x038, address=0x401c, mask=0x07e
*/


#define PSF_2_PSF_PORT_CONFIG_PG1_PORT0_SB_PSF_HCB_REG 0x2422401C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cplpreallocen : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the port will not
                               send out a NP request unless there are
                               sufficient resources for completions that will
                               be returned
                            */
    UINT32 ingressfro : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set, IOSF relaxed ordering
                               will be forced for incoming transactions,
                               regardless of the relaxed ordering (RO)
                               attribute on the IOSF interface.
                            */
    UINT32 egressfro : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set, IOSF relaxed ordering
                               will be forced for outgoing transactions,
                               regardless of the relaxed ordering (RO)
                               attribute on the IOSF interface. Egress Force
                               Relax Ordering is only applicable for PSF port
                               types with buffering (agent port, buffered
                               fabric port). For other ports, ordering is
                               affecting only the ingress PSF port.
                            */
    UINT32 instfwd : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to '1', an ingress
                               buffered PSF port use full store-and-forward.
                               The ingress port will wait until all the
                               required cmd and data for the transaction is
                               available in the port buffers, before initiating
                               a transaction to the PSF Central Arbiter. If set
                               to '0', the port will initiate a transaction
                               immediately when the cmd is received. This bit
                               can be set to 0 when an ingress port connects to
                               an interface on the same or faster clock domain
                            */
    UINT32 egstfwd : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to '1', an egress
                               buffered PSF port use full store-and-forward.
                               The egress port will wait until all the required
                               cmd and data for the transaction is available in
                               the port buffers, before initiating a
                               transaction to an external agent. If set to '0',
                               the port will initiate a transaction immediately
                               when the cmd is received. This bit can be set to
                               0 when an egress port connects to an interface
                               on the same or faster clock domain
                            */
    UINT32 ismcpltracken : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to '1', the PSF port
                               ISM(s) will not return to IDLE unless all
                               outstanding NP read transactions sent from an
                               egress port have been followed by their
                               corresponding CPL transactions on the ingress
                               port
                            */
    UINT32 earlcredrtrn : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set to '1', an ingress
                               target port will return cmd and data credit as
                               soon as the transaction is granted by the PSF
                               central arbiter. If set to '0', the credit is
                               returned only when the full transaction is done.
                               This bit can be set to 1 when an ingress target
                               port connects to an interface on the same or
                               slower clock domain
                            */
    UINT32 endatapardetingress : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, If set to '1', ingress data
                               parity detection is enabled, and the number of
                               ones on the data and dparity signals on an
                               fabric master interface or an agent target
                               interface is an odd number. PSF will flag a
                               dparity error to the error handler
                            */
    UINT32 endatapardetegress : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, If set to '1', egress data
                               parity detection is enabled, and the number of
                               ones on the data and dparity signals on an
                               fabric target interface or an agent master
                               interface is an odd number. PSF will flag a
                               dparity error to the error handler.
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_PSF_PORT_CONFIG_PG1_PORT0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_PSF_PORT_CONFIG_PG1_PORT1_SB_PSF_HCB_REG supported on:                 */
/*      SPRMCC (0x20004020)                                                     */
/* Register default value on SPRMCC: 0x00000038                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Miscellaneous control bits for individual PSF ports on the segment
        Details: InstName=psf_2, reg_name=psf_2_PSF_PORT_CONFIG_PG1_PORT1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.portcfg_pg[1].port[1].gen.i_psf20_reg_psf_portcfg, size=9, reset=0x038, address=0x4020, mask=0x07e
*/


#define PSF_2_PSF_PORT_CONFIG_PG1_PORT1_SB_PSF_HCB_REG 0x24224020

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cplpreallocen : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the port will not
                               send out a NP request unless there are
                               sufficient resources for completions that will
                               be returned
                            */
    UINT32 ingressfro : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set, IOSF relaxed ordering
                               will be forced for incoming transactions,
                               regardless of the relaxed ordering (RO)
                               attribute on the IOSF interface.
                            */
    UINT32 egressfro : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set, IOSF relaxed ordering
                               will be forced for outgoing transactions,
                               regardless of the relaxed ordering (RO)
                               attribute on the IOSF interface. Egress Force
                               Relax Ordering is only applicable for PSF port
                               types with buffering (agent port, buffered
                               fabric port). For other ports, ordering is
                               affecting only the ingress PSF port.
                            */
    UINT32 instfwd : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to '1', an ingress
                               buffered PSF port use full store-and-forward.
                               The ingress port will wait until all the
                               required cmd and data for the transaction is
                               available in the port buffers, before initiating
                               a transaction to the PSF Central Arbiter. If set
                               to '0', the port will initiate a transaction
                               immediately when the cmd is received. This bit
                               can be set to 0 when an ingress port connects to
                               an interface on the same or faster clock domain
                            */
    UINT32 egstfwd : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to '1', an egress
                               buffered PSF port use full store-and-forward.
                               The egress port will wait until all the required
                               cmd and data for the transaction is available in
                               the port buffers, before initiating a
                               transaction to an external agent. If set to '0',
                               the port will initiate a transaction immediately
                               when the cmd is received. This bit can be set to
                               0 when an egress port connects to an interface
                               on the same or faster clock domain
                            */
    UINT32 ismcpltracken : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to '1', the PSF port
                               ISM(s) will not return to IDLE unless all
                               outstanding NP read transactions sent from an
                               egress port have been followed by their
                               corresponding CPL transactions on the ingress
                               port
                            */
    UINT32 earlcredrtrn : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set to '1', an ingress
                               target port will return cmd and data credit as
                               soon as the transaction is granted by the PSF
                               central arbiter. If set to '0', the credit is
                               returned only when the full transaction is done.
                               This bit can be set to 1 when an ingress target
                               port connects to an interface on the same or
                               slower clock domain
                            */
    UINT32 endatapardetingress : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, If set to '1', ingress data
                               parity detection is enabled, and the number of
                               ones on the data and dparity signals on an
                               fabric master interface or an agent target
                               interface is an odd number. PSF will flag a
                               dparity error to the error handler
                            */
    UINT32 endatapardetegress : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, If set to '1', egress data
                               parity detection is enabled, and the number of
                               ones on the data and dparity signals on an
                               fabric target interface or an agent master
                               interface is an odd number. PSF will flag a
                               dparity error to the error handler.
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_PSF_PORT_CONFIG_PG1_PORT1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_PSF_PORT_CONFIG_PG1_PORT2_SB_PSF_HCB_REG supported on:                 */
/*      SPRMCC (0x20004024)                                                     */
/* Register default value on SPRMCC: 0x00000038                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Miscellaneous control bits for individual PSF ports on the segment
        Details: InstName=psf_2, reg_name=psf_2_PSF_PORT_CONFIG_PG1_PORT2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.portcfg_pg[1].port[2].gen.i_psf20_reg_psf_portcfg, size=9, reset=0x038, address=0x4024, mask=0x07e
*/


#define PSF_2_PSF_PORT_CONFIG_PG1_PORT2_SB_PSF_HCB_REG 0x24224024

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cplpreallocen : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the port will not
                               send out a NP request unless there are
                               sufficient resources for completions that will
                               be returned
                            */
    UINT32 ingressfro : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set, IOSF relaxed ordering
                               will be forced for incoming transactions,
                               regardless of the relaxed ordering (RO)
                               attribute on the IOSF interface.
                            */
    UINT32 egressfro : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set, IOSF relaxed ordering
                               will be forced for outgoing transactions,
                               regardless of the relaxed ordering (RO)
                               attribute on the IOSF interface. Egress Force
                               Relax Ordering is only applicable for PSF port
                               types with buffering (agent port, buffered
                               fabric port). For other ports, ordering is
                               affecting only the ingress PSF port.
                            */
    UINT32 instfwd : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to '1', an ingress
                               buffered PSF port use full store-and-forward.
                               The ingress port will wait until all the
                               required cmd and data for the transaction is
                               available in the port buffers, before initiating
                               a transaction to the PSF Central Arbiter. If set
                               to '0', the port will initiate a transaction
                               immediately when the cmd is received. This bit
                               can be set to 0 when an ingress port connects to
                               an interface on the same or faster clock domain
                            */
    UINT32 egstfwd : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to '1', an egress
                               buffered PSF port use full store-and-forward.
                               The egress port will wait until all the required
                               cmd and data for the transaction is available in
                               the port buffers, before initiating a
                               transaction to an external agent. If set to '0',
                               the port will initiate a transaction immediately
                               when the cmd is received. This bit can be set to
                               0 when an egress port connects to an interface
                               on the same or faster clock domain
                            */
    UINT32 ismcpltracken : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to '1', the PSF port
                               ISM(s) will not return to IDLE unless all
                               outstanding NP read transactions sent from an
                               egress port have been followed by their
                               corresponding CPL transactions on the ingress
                               port
                            */
    UINT32 earlcredrtrn : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set to '1', an ingress
                               target port will return cmd and data credit as
                               soon as the transaction is granted by the PSF
                               central arbiter. If set to '0', the credit is
                               returned only when the full transaction is done.
                               This bit can be set to 1 when an ingress target
                               port connects to an interface on the same or
                               slower clock domain
                            */
    UINT32 endatapardetingress : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, If set to '1', ingress data
                               parity detection is enabled, and the number of
                               ones on the data and dparity signals on an
                               fabric master interface or an agent target
                               interface is an odd number. PSF will flag a
                               dparity error to the error handler
                            */
    UINT32 endatapardetegress : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, If set to '1', egress data
                               parity detection is enabled, and the number of
                               ones on the data and dparity signals on an
                               fabric target interface or an agent master
                               interface is an odd number. PSF will flag a
                               dparity error to the error handler.
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_PSF_PORT_CONFIG_PG1_PORT2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_PSF_PORT_CONFIG_PG1_PORT3_SB_PSF_HCB_REG supported on:                 */
/*      SPRMCC (0x20004028)                                                     */
/* Register default value on SPRMCC: 0x00000038                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Miscellaneous control bits for individual PSF ports on the segment
        Details: InstName=psf_2, reg_name=psf_2_PSF_PORT_CONFIG_PG1_PORT3 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.portcfg_pg[1].port[3].gen.i_psf20_reg_psf_portcfg, size=9, reset=0x038, address=0x4028, mask=0x07e
*/


#define PSF_2_PSF_PORT_CONFIG_PG1_PORT3_SB_PSF_HCB_REG 0x24224028

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cplpreallocen : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the port will not
                               send out a NP request unless there are
                               sufficient resources for completions that will
                               be returned
                            */
    UINT32 ingressfro : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set, IOSF relaxed ordering
                               will be forced for incoming transactions,
                               regardless of the relaxed ordering (RO)
                               attribute on the IOSF interface.
                            */
    UINT32 egressfro : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set, IOSF relaxed ordering
                               will be forced for outgoing transactions,
                               regardless of the relaxed ordering (RO)
                               attribute on the IOSF interface. Egress Force
                               Relax Ordering is only applicable for PSF port
                               types with buffering (agent port, buffered
                               fabric port). For other ports, ordering is
                               affecting only the ingress PSF port.
                            */
    UINT32 instfwd : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to '1', an ingress
                               buffered PSF port use full store-and-forward.
                               The ingress port will wait until all the
                               required cmd and data for the transaction is
                               available in the port buffers, before initiating
                               a transaction to the PSF Central Arbiter. If set
                               to '0', the port will initiate a transaction
                               immediately when the cmd is received. This bit
                               can be set to 0 when an ingress port connects to
                               an interface on the same or faster clock domain
                            */
    UINT32 egstfwd : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to '1', an egress
                               buffered PSF port use full store-and-forward.
                               The egress port will wait until all the required
                               cmd and data for the transaction is available in
                               the port buffers, before initiating a
                               transaction to an external agent. If set to '0',
                               the port will initiate a transaction immediately
                               when the cmd is received. This bit can be set to
                               0 when an egress port connects to an interface
                               on the same or faster clock domain
                            */
    UINT32 ismcpltracken : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to '1', the PSF port
                               ISM(s) will not return to IDLE unless all
                               outstanding NP read transactions sent from an
                               egress port have been followed by their
                               corresponding CPL transactions on the ingress
                               port
                            */
    UINT32 earlcredrtrn : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set to '1', an ingress
                               target port will return cmd and data credit as
                               soon as the transaction is granted by the PSF
                               central arbiter. If set to '0', the credit is
                               returned only when the full transaction is done.
                               This bit can be set to 1 when an ingress target
                               port connects to an interface on the same or
                               slower clock domain
                            */
    UINT32 endatapardetingress : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, If set to '1', ingress data
                               parity detection is enabled, and the number of
                               ones on the data and dparity signals on an
                               fabric master interface or an agent target
                               interface is an odd number. PSF will flag a
                               dparity error to the error handler
                            */
    UINT32 endatapardetegress : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, If set to '1', egress data
                               parity detection is enabled, and the number of
                               ones on the data and dparity signals on an
                               fabric target interface or an agent master
                               interface is an odd number. PSF will flag a
                               dparity error to the error handler.
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_PSF_PORT_CONFIG_PG1_PORT3_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_PSF_PORT_CONFIG_PG1_PORT4_SB_PSF_HCB_REG supported on:                 */
/*      SPRMCC (0x2000402c)                                                     */
/* Register default value on SPRMCC: 0x00000038                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Miscellaneous control bits for individual PSF ports on the segment
        Details: InstName=psf_2, reg_name=psf_2_PSF_PORT_CONFIG_PG1_PORT4 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.portcfg_pg[1].port[4].gen.i_psf20_reg_psf_portcfg, size=9, reset=0x038, address=0x402c, mask=0x07e
*/


#define PSF_2_PSF_PORT_CONFIG_PG1_PORT4_SB_PSF_HCB_REG 0x2422402C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cplpreallocen : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the port will not
                               send out a NP request unless there are
                               sufficient resources for completions that will
                               be returned
                            */
    UINT32 ingressfro : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set, IOSF relaxed ordering
                               will be forced for incoming transactions,
                               regardless of the relaxed ordering (RO)
                               attribute on the IOSF interface.
                            */
    UINT32 egressfro : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set, IOSF relaxed ordering
                               will be forced for outgoing transactions,
                               regardless of the relaxed ordering (RO)
                               attribute on the IOSF interface. Egress Force
                               Relax Ordering is only applicable for PSF port
                               types with buffering (agent port, buffered
                               fabric port). For other ports, ordering is
                               affecting only the ingress PSF port.
                            */
    UINT32 instfwd : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to '1', an ingress
                               buffered PSF port use full store-and-forward.
                               The ingress port will wait until all the
                               required cmd and data for the transaction is
                               available in the port buffers, before initiating
                               a transaction to the PSF Central Arbiter. If set
                               to '0', the port will initiate a transaction
                               immediately when the cmd is received. This bit
                               can be set to 0 when an ingress port connects to
                               an interface on the same or faster clock domain
                            */
    UINT32 egstfwd : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to '1', an egress
                               buffered PSF port use full store-and-forward.
                               The egress port will wait until all the required
                               cmd and data for the transaction is available in
                               the port buffers, before initiating a
                               transaction to an external agent. If set to '0',
                               the port will initiate a transaction immediately
                               when the cmd is received. This bit can be set to
                               0 when an egress port connects to an interface
                               on the same or faster clock domain
                            */
    UINT32 ismcpltracken : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, If set to '1', the PSF port
                               ISM(s) will not return to IDLE unless all
                               outstanding NP read transactions sent from an
                               egress port have been followed by their
                               corresponding CPL transactions on the ingress
                               port
                            */
    UINT32 earlcredrtrn : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set to '1', an ingress
                               target port will return cmd and data credit as
                               soon as the transaction is granted by the PSF
                               central arbiter. If set to '0', the credit is
                               returned only when the full transaction is done.
                               This bit can be set to 1 when an ingress target
                               port connects to an interface on the same or
                               slower clock domain
                            */
    UINT32 endatapardetingress : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, If set to '1', ingress data
                               parity detection is enabled, and the number of
                               ones on the data and dparity signals on an
                               fabric master interface or an agent target
                               interface is an odd number. PSF will flag a
                               dparity error to the error handler
                            */
    UINT32 endatapardetegress : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, If set to '1', egress data
                               parity detection is enabled, and the number of
                               ones on the data and dparity signals on an
                               fabric target interface or an agent master
                               interface is an odd number. PSF will flag a
                               dparity error to the error handler.
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_PSF_PORT_CONFIG_PG1_PORT4_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CHANMAP_SC_PG0_PORT0_CHAN0_RS0_CHMAP1_SB_PSF_HCB_REG supported on:     */
/*      SPRMCC (0x20004030)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to map a source channel to a IOSF Channel.Each register maps a source channel to a unique IOSF channel in a specific root space. If a source channel is not mapped, the transaction will use TC-based channel mapping. This field width is variable, and dependent on PSF configuration parameters.
        Details: InstName=psf_2, reg_name=psf_2_CHANMAP_SC_PG0_PORT0_CHAN0_RS0_CHMAP1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.chmapregs.pchmap_pg[0].port[0].pchmap.chan[0].gen.rsi[0].chmap_num[1].chmaptcsc.i_psf20_reg_psf_port_chmap, size=32, reset=0x00000000, address=0x4030, mask=0x1100000f
*/


#define PSF_2_CHANMAP_SC_PG0_PORT0_CHAN0_RS0_CHMAP1_SB_PSF_HCB_REG 0x24224030

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sc_pg0_p0_ch0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC0 that is mapped to SC [0]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC1 that is mapped to SC [1]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC2 that is mapped to SC [2]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC3 that is mapped to SC [3]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch0 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC4 that is mapped to SC [4]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch1 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC5 that is mapped to SC [5]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch2 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC6 that is mapped to SC [6]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch3 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC7 that is mapped to SC [7]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch0 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC8 that is mapped to SC [8]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC9 that is mapped to SC [9]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch2 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC10 that is mapped to SC [10]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p2_ch3 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC11 that is mapped to SC [11]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch0 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC12 that is mapped to SC [12]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch1 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC13 that is mapped to SC [13]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch2 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC14 that is mapped to SC [14]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch3 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC15 that is mapped to SC [15]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch0 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC16 that is mapped to SC [16]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch1 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC17 that is mapped to SC [17]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch2 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC18 that is mapped to SC [18]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch3 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC19 that is mapped to SC [19]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch0 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC20 that is mapped to SC [20]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch1 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC21 that is mapped to SC [21]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch2 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC22 that is mapped to SC [22]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch3 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC23 that is mapped to SC [23]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch0 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC24 that is mapped to SC [24]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch1 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC25 that is mapped to SC [25]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch2 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC26 that is mapped to SC [26]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch3 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC27 that is mapped to SC [27]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch0 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC28 that is mapped to SC [28]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch1 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC29 that is mapped to SC [29]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch2 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC30 that is mapped to SC [30]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch3 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC31 that is mapped to SC [31]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CHANMAP_SC_PG0_PORT0_CHAN0_RS0_CHMAP1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CHANMAP_SC_PG0_PORT0_CHAN0_RS0_CHMAP2_SB_PSF_HCB_REG supported on:     */
/*      SPRMCC (0x20004034)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to map a source channel to a IOSF Channel.Each register maps a source channel to a unique IOSF channel in a specific root space. If a source channel is not mapped, the transaction will use TC-based channel mapping. This field width is variable, and dependent on PSF configuration parameters.
        Details: InstName=psf_2, reg_name=psf_2_CHANMAP_SC_PG0_PORT0_CHAN0_RS0_CHMAP2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.chmapregs.pchmap_pg[0].port[0].pchmap.chan[0].gen.rsi[0].chmap_num[2].chmaptcsc.i_psf20_reg_psf_port_chmap, size=32, reset=0x00000000, address=0x4034, mask=0x00001111
*/


#define PSF_2_CHANMAP_SC_PG0_PORT0_CHAN0_RS0_CHMAP2_SB_PSF_HCB_REG 0x24224034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sc_pg1_p2_ch0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC0 that is mapped to SC [0]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC1 that is mapped to SC [1]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC2 that is mapped to SC [2]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC3 that is mapped to SC [3]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch0 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC4 that is mapped to SC [4]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch1 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC5 that is mapped to SC [5]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch2 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC6 that is mapped to SC [6]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch3 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC7 that is mapped to SC [7]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch0 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC8 that is mapped to SC [8]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC9 that is mapped to SC [9]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch2 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC10 that is mapped to SC [10]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p4_ch3 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC11 that is mapped to SC [11]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch0 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC12 that is mapped to SC [12]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch1 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC13 that is mapped to SC [13]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch2 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC14 that is mapped to SC [14]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch3 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC15 that is mapped to SC [15]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc16 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC16 that is mapped to SC [16]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc17 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC17 that is mapped to SC [17]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc18 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC18 that is mapped to SC [18]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc19 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC19 that is mapped to SC [19]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC20 that is mapped to SC [20]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc21 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC21 that is mapped to SC [21]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC22 that is mapped to SC [22]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc23 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC23 that is mapped to SC [23]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC24 that is mapped to SC [24]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc25 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC25 that is mapped to SC [25]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc26 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC26 that is mapped to SC [26]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc27 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC27 that is mapped to SC [27]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc28 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC28 that is mapped to SC [28]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc29 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC29 that is mapped to SC [29]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc30 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC30 that is mapped to SC [30]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC31 that is mapped to SC [31]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CHANMAP_SC_PG0_PORT0_CHAN0_RS0_CHMAP2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CHANMAP_SC_PG0_PORT0_CHAN0_RS3_CHMAP1_SB_PSF_HCB_REG supported on:     */
/*      SPRMCC (0x20004038)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to map a source channel to a IOSF Channel.Each register maps a source channel to a unique IOSF channel in a specific root space. If a source channel is not mapped, the transaction will use TC-based channel mapping. This field width is variable, and dependent on PSF configuration parameters.
        Details: InstName=psf_2, reg_name=psf_2_CHANMAP_SC_PG0_PORT0_CHAN0_RS3_CHMAP1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.chmapregs.pchmap_pg[0].port[0].pchmap.chan[0].gen.rsi[1].chmap_num[1].chmaptcsc.i_psf20_reg_psf_port_chmap, size=32, reset=0x00000000, address=0x4038, mask=0x1100000f
*/


#define PSF_2_CHANMAP_SC_PG0_PORT0_CHAN0_RS3_CHMAP1_SB_PSF_HCB_REG 0x24224038

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sc_pg0_p0_ch0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC0 that is mapped to SC [0]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC1 that is mapped to SC [1]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC2 that is mapped to SC [2]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC3 that is mapped to SC [3]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch0 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC4 that is mapped to SC [4]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch1 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC5 that is mapped to SC [5]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch2 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC6 that is mapped to SC [6]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch3 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC7 that is mapped to SC [7]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch0 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC8 that is mapped to SC [8]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC9 that is mapped to SC [9]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch2 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC10 that is mapped to SC [10]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p2_ch3 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC11 that is mapped to SC [11]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch0 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC12 that is mapped to SC [12]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch1 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC13 that is mapped to SC [13]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch2 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC14 that is mapped to SC [14]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch3 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC15 that is mapped to SC [15]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch0 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC16 that is mapped to SC [16]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch1 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC17 that is mapped to SC [17]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch2 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC18 that is mapped to SC [18]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch3 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC19 that is mapped to SC [19]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch0 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC20 that is mapped to SC [20]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch1 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC21 that is mapped to SC [21]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch2 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC22 that is mapped to SC [22]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch3 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC23 that is mapped to SC [23]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch0 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC24 that is mapped to SC [24]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch1 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC25 that is mapped to SC [25]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch2 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC26 that is mapped to SC [26]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch3 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC27 that is mapped to SC [27]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch0 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC28 that is mapped to SC [28]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch1 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC29 that is mapped to SC [29]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch2 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC30 that is mapped to SC [30]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch3 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC31 that is mapped to SC [31]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CHANMAP_SC_PG0_PORT0_CHAN0_RS3_CHMAP1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CHANMAP_SC_PG0_PORT0_CHAN0_RS3_CHMAP2_SB_PSF_HCB_REG supported on:     */
/*      SPRMCC (0x2000403c)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to map a source channel to a IOSF Channel.Each register maps a source channel to a unique IOSF channel in a specific root space. If a source channel is not mapped, the transaction will use TC-based channel mapping. This field width is variable, and dependent on PSF configuration parameters.
        Details: InstName=psf_2, reg_name=psf_2_CHANMAP_SC_PG0_PORT0_CHAN0_RS3_CHMAP2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.chmapregs.pchmap_pg[0].port[0].pchmap.chan[0].gen.rsi[1].chmap_num[2].chmaptcsc.i_psf20_reg_psf_port_chmap, size=32, reset=0x00000000, address=0x403c, mask=0x00001111
*/


#define PSF_2_CHANMAP_SC_PG0_PORT0_CHAN0_RS3_CHMAP2_SB_PSF_HCB_REG 0x2422403C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sc_pg1_p2_ch0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC0 that is mapped to SC [0]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC1 that is mapped to SC [1]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC2 that is mapped to SC [2]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC3 that is mapped to SC [3]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch0 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC4 that is mapped to SC [4]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch1 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC5 that is mapped to SC [5]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch2 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC6 that is mapped to SC [6]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch3 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC7 that is mapped to SC [7]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch0 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC8 that is mapped to SC [8]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC9 that is mapped to SC [9]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch2 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC10 that is mapped to SC [10]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p4_ch3 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC11 that is mapped to SC [11]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch0 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC12 that is mapped to SC [12]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch1 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC13 that is mapped to SC [13]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch2 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC14 that is mapped to SC [14]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch3 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC15 that is mapped to SC [15]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc16 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC16 that is mapped to SC [16]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc17 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC17 that is mapped to SC [17]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc18 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC18 that is mapped to SC [18]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc19 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC19 that is mapped to SC [19]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC20 that is mapped to SC [20]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc21 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC21 that is mapped to SC [21]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC22 that is mapped to SC [22]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc23 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC23 that is mapped to SC [23]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC24 that is mapped to SC [24]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc25 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC25 that is mapped to SC [25]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc26 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC26 that is mapped to SC [26]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc27 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC27 that is mapped to SC [27]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc28 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC28 that is mapped to SC [28]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc29 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC29 that is mapped to SC [29]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc30 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC30 that is mapped to SC [30]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC31 that is mapped to SC [31]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CHANMAP_SC_PG0_PORT0_CHAN0_RS3_CHMAP2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CHANMAP_SC_PG0_PORT0_CHAN1_RS0_CHMAP1_SB_PSF_HCB_REG supported on:     */
/*      SPRMCC (0x20004040)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to map a source channel to a IOSF Channel.Each register maps a source channel to a unique IOSF channel in a specific root space. If a source channel is not mapped, the transaction will use TC-based channel mapping. This field width is variable, and dependent on PSF configuration parameters.
        Details: InstName=psf_2, reg_name=psf_2_CHANMAP_SC_PG0_PORT0_CHAN1_RS0_CHMAP1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.chmapregs.pchmap_pg[0].port[0].pchmap.chan[1].gen.rsi[0].chmap_num[1].chmaptcsc.i_psf20_reg_psf_port_chmap, size=32, reset=0x00000000, address=0x4040, mask=0x1100000f
*/


#define PSF_2_CHANMAP_SC_PG0_PORT0_CHAN1_RS0_CHMAP1_SB_PSF_HCB_REG 0x24224040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sc_pg0_p0_ch0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC0 that is mapped to SC [0]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC1 that is mapped to SC [1]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC2 that is mapped to SC [2]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC3 that is mapped to SC [3]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch0 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC4 that is mapped to SC [4]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch1 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC5 that is mapped to SC [5]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch2 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC6 that is mapped to SC [6]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch3 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC7 that is mapped to SC [7]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch0 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC8 that is mapped to SC [8]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC9 that is mapped to SC [9]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch2 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC10 that is mapped to SC [10]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p2_ch3 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC11 that is mapped to SC [11]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch0 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC12 that is mapped to SC [12]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch1 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC13 that is mapped to SC [13]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch2 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC14 that is mapped to SC [14]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch3 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC15 that is mapped to SC [15]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch0 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC16 that is mapped to SC [16]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch1 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC17 that is mapped to SC [17]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch2 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC18 that is mapped to SC [18]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch3 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC19 that is mapped to SC [19]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch0 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC20 that is mapped to SC [20]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch1 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC21 that is mapped to SC [21]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch2 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC22 that is mapped to SC [22]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch3 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC23 that is mapped to SC [23]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch0 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC24 that is mapped to SC [24]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch1 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC25 that is mapped to SC [25]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch2 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC26 that is mapped to SC [26]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch3 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC27 that is mapped to SC [27]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch0 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC28 that is mapped to SC [28]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch1 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC29 that is mapped to SC [29]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch2 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC30 that is mapped to SC [30]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch3 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC31 that is mapped to SC [31]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CHANMAP_SC_PG0_PORT0_CHAN1_RS0_CHMAP1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CHANMAP_SC_PG0_PORT0_CHAN1_RS0_CHMAP2_SB_PSF_HCB_REG supported on:     */
/*      SPRMCC (0x20004044)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to map a source channel to a IOSF Channel.Each register maps a source channel to a unique IOSF channel in a specific root space. If a source channel is not mapped, the transaction will use TC-based channel mapping. This field width is variable, and dependent on PSF configuration parameters.
        Details: InstName=psf_2, reg_name=psf_2_CHANMAP_SC_PG0_PORT0_CHAN1_RS0_CHMAP2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.chmapregs.pchmap_pg[0].port[0].pchmap.chan[1].gen.rsi[0].chmap_num[2].chmaptcsc.i_psf20_reg_psf_port_chmap, size=32, reset=0x00000001, address=0x4044, mask=0x00001111
*/


#define PSF_2_CHANMAP_SC_PG0_PORT0_CHAN1_RS0_CHMAP2_SB_PSF_HCB_REG 0x24224044

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sc_pg1_p2_ch0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, One-hot encoded source
                               channel SC0 that is mapped to SC [0]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC1 that is mapped to SC [1]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC2 that is mapped to SC [2]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC3 that is mapped to SC [3]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch0 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC4 that is mapped to SC [4]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch1 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC5 that is mapped to SC [5]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch2 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC6 that is mapped to SC [6]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch3 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC7 that is mapped to SC [7]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch0 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC8 that is mapped to SC [8]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC9 that is mapped to SC [9]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch2 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC10 that is mapped to SC [10]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p4_ch3 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC11 that is mapped to SC [11]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch0 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC12 that is mapped to SC [12]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch1 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC13 that is mapped to SC [13]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch2 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC14 that is mapped to SC [14]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch3 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC15 that is mapped to SC [15]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc16 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC16 that is mapped to SC [16]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc17 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC17 that is mapped to SC [17]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc18 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC18 that is mapped to SC [18]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc19 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC19 that is mapped to SC [19]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC20 that is mapped to SC [20]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc21 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC21 that is mapped to SC [21]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC22 that is mapped to SC [22]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc23 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC23 that is mapped to SC [23]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC24 that is mapped to SC [24]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc25 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC25 that is mapped to SC [25]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc26 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC26 that is mapped to SC [26]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc27 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC27 that is mapped to SC [27]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc28 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC28 that is mapped to SC [28]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc29 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC29 that is mapped to SC [29]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc30 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC30 that is mapped to SC [30]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC31 that is mapped to SC [31]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CHANMAP_SC_PG0_PORT0_CHAN1_RS0_CHMAP2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CHANMAP_SC_PG0_PORT0_CHAN1_RS3_CHMAP1_SB_PSF_HCB_REG supported on:     */
/*      SPRMCC (0x20004048)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to map a source channel to a IOSF Channel.Each register maps a source channel to a unique IOSF channel in a specific root space. If a source channel is not mapped, the transaction will use TC-based channel mapping. This field width is variable, and dependent on PSF configuration parameters.
        Details: InstName=psf_2, reg_name=psf_2_CHANMAP_SC_PG0_PORT0_CHAN1_RS3_CHMAP1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.chmapregs.pchmap_pg[0].port[0].pchmap.chan[1].gen.rsi[1].chmap_num[1].chmaptcsc.i_psf20_reg_psf_port_chmap, size=32, reset=0x00000000, address=0x4048, mask=0x1100000f
*/


#define PSF_2_CHANMAP_SC_PG0_PORT0_CHAN1_RS3_CHMAP1_SB_PSF_HCB_REG 0x24224048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sc_pg0_p0_ch0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC0 that is mapped to SC [0]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC1 that is mapped to SC [1]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC2 that is mapped to SC [2]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC3 that is mapped to SC [3]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch0 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC4 that is mapped to SC [4]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch1 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC5 that is mapped to SC [5]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch2 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC6 that is mapped to SC [6]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch3 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC7 that is mapped to SC [7]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch0 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC8 that is mapped to SC [8]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC9 that is mapped to SC [9]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch2 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC10 that is mapped to SC [10]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p2_ch3 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC11 that is mapped to SC [11]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch0 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC12 that is mapped to SC [12]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch1 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC13 that is mapped to SC [13]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch2 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC14 that is mapped to SC [14]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch3 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC15 that is mapped to SC [15]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch0 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC16 that is mapped to SC [16]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch1 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC17 that is mapped to SC [17]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch2 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC18 that is mapped to SC [18]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch3 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC19 that is mapped to SC [19]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch0 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC20 that is mapped to SC [20]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch1 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC21 that is mapped to SC [21]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch2 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC22 that is mapped to SC [22]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch3 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC23 that is mapped to SC [23]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch0 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC24 that is mapped to SC [24]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch1 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC25 that is mapped to SC [25]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch2 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC26 that is mapped to SC [26]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch3 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC27 that is mapped to SC [27]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch0 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC28 that is mapped to SC [28]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch1 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC29 that is mapped to SC [29]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch2 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC30 that is mapped to SC [30]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch3 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC31 that is mapped to SC [31]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CHANMAP_SC_PG0_PORT0_CHAN1_RS3_CHMAP1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CHANMAP_SC_PG0_PORT0_CHAN1_RS3_CHMAP2_SB_PSF_HCB_REG supported on:     */
/*      SPRMCC (0x2000404c)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to map a source channel to a IOSF Channel.Each register maps a source channel to a unique IOSF channel in a specific root space. If a source channel is not mapped, the transaction will use TC-based channel mapping. This field width is variable, and dependent on PSF configuration parameters.
        Details: InstName=psf_2, reg_name=psf_2_CHANMAP_SC_PG0_PORT0_CHAN1_RS3_CHMAP2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.chmapregs.pchmap_pg[0].port[0].pchmap.chan[1].gen.rsi[1].chmap_num[2].chmaptcsc.i_psf20_reg_psf_port_chmap, size=32, reset=0x00000000, address=0x404c, mask=0x00001111
*/


#define PSF_2_CHANMAP_SC_PG0_PORT0_CHAN1_RS3_CHMAP2_SB_PSF_HCB_REG 0x2422404C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sc_pg1_p2_ch0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC0 that is mapped to SC [0]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC1 that is mapped to SC [1]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC2 that is mapped to SC [2]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC3 that is mapped to SC [3]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch0 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC4 that is mapped to SC [4]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch1 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC5 that is mapped to SC [5]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch2 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC6 that is mapped to SC [6]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch3 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC7 that is mapped to SC [7]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch0 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC8 that is mapped to SC [8]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC9 that is mapped to SC [9]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch2 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC10 that is mapped to SC [10]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p4_ch3 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC11 that is mapped to SC [11]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch0 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC12 that is mapped to SC [12]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch1 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC13 that is mapped to SC [13]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch2 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC14 that is mapped to SC [14]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch3 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC15 that is mapped to SC [15]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc16 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC16 that is mapped to SC [16]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc17 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC17 that is mapped to SC [17]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc18 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC18 that is mapped to SC [18]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc19 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC19 that is mapped to SC [19]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC20 that is mapped to SC [20]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc21 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC21 that is mapped to SC [21]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC22 that is mapped to SC [22]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc23 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC23 that is mapped to SC [23]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC24 that is mapped to SC [24]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc25 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC25 that is mapped to SC [25]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc26 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC26 that is mapped to SC [26]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc27 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC27 that is mapped to SC [27]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc28 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC28 that is mapped to SC [28]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc29 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC29 that is mapped to SC [29]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc30 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC30 that is mapped to SC [30]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC31 that is mapped to SC [31]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CHANMAP_SC_PG0_PORT0_CHAN1_RS3_CHMAP2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CHANMAP_SC_PG0_PORT0_CHAN2_RS0_CHMAP1_SB_PSF_HCB_REG supported on:     */
/*      SPRMCC (0x20004050)                                                     */
/* Register default value on SPRMCC: 0x10000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to map a source channel to a IOSF Channel.Each register maps a source channel to a unique IOSF channel in a specific root space. If a source channel is not mapped, the transaction will use TC-based channel mapping. This field width is variable, and dependent on PSF configuration parameters.
        Details: InstName=psf_2, reg_name=psf_2_CHANMAP_SC_PG0_PORT0_CHAN2_RS0_CHMAP1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.chmapregs.pchmap_pg[0].port[0].pchmap.chan[2].gen.rsi[0].chmap_num[1].chmaptcsc.i_psf20_reg_psf_port_chmap, size=32, reset=0x10000000, address=0x4050, mask=0x1100000f
*/


#define PSF_2_CHANMAP_SC_PG0_PORT0_CHAN2_RS0_CHMAP1_SB_PSF_HCB_REG 0x24224050

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sc_pg0_p0_ch0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC0 that is mapped to SC [0]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC1 that is mapped to SC [1]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC2 that is mapped to SC [2]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC3 that is mapped to SC [3]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch0 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC4 that is mapped to SC [4]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch1 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC5 that is mapped to SC [5]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch2 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC6 that is mapped to SC [6]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch3 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC7 that is mapped to SC [7]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch0 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC8 that is mapped to SC [8]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC9 that is mapped to SC [9]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch2 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC10 that is mapped to SC [10]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p2_ch3 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC11 that is mapped to SC [11]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch0 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC12 that is mapped to SC [12]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch1 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC13 that is mapped to SC [13]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch2 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC14 that is mapped to SC [14]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch3 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC15 that is mapped to SC [15]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch0 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC16 that is mapped to SC [16]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch1 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC17 that is mapped to SC [17]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch2 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC18 that is mapped to SC [18]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch3 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC19 that is mapped to SC [19]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch0 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC20 that is mapped to SC [20]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch1 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC21 that is mapped to SC [21]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch2 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC22 that is mapped to SC [22]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch3 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC23 that is mapped to SC [23]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch0 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC24 that is mapped to SC [24]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch1 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC25 that is mapped to SC [25]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch2 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC26 that is mapped to SC [26]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch3 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC27 that is mapped to SC [27]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch0 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, One-hot encoded source
                               channel SC28 that is mapped to SC [28]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch1 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC29 that is mapped to SC [29]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch2 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC30 that is mapped to SC [30]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch3 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC31 that is mapped to SC [31]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CHANMAP_SC_PG0_PORT0_CHAN2_RS0_CHMAP1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CHANMAP_SC_PG0_PORT0_CHAN2_RS0_CHMAP2_SB_PSF_HCB_REG supported on:     */
/*      SPRMCC (0x20004054)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to map a source channel to a IOSF Channel.Each register maps a source channel to a unique IOSF channel in a specific root space. If a source channel is not mapped, the transaction will use TC-based channel mapping. This field width is variable, and dependent on PSF configuration parameters.
        Details: InstName=psf_2, reg_name=psf_2_CHANMAP_SC_PG0_PORT0_CHAN2_RS0_CHMAP2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.chmapregs.pchmap_pg[0].port[0].pchmap.chan[2].gen.rsi[0].chmap_num[2].chmaptcsc.i_psf20_reg_psf_port_chmap, size=32, reset=0x00000000, address=0x4054, mask=0x00001111
*/


#define PSF_2_CHANMAP_SC_PG0_PORT0_CHAN2_RS0_CHMAP2_SB_PSF_HCB_REG 0x24224054

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sc_pg1_p2_ch0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC0 that is mapped to SC [0]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC1 that is mapped to SC [1]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC2 that is mapped to SC [2]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC3 that is mapped to SC [3]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch0 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC4 that is mapped to SC [4]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch1 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC5 that is mapped to SC [5]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch2 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC6 that is mapped to SC [6]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch3 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC7 that is mapped to SC [7]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch0 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC8 that is mapped to SC [8]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC9 that is mapped to SC [9]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch2 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC10 that is mapped to SC [10]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p4_ch3 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC11 that is mapped to SC [11]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch0 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC12 that is mapped to SC [12]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch1 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC13 that is mapped to SC [13]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch2 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC14 that is mapped to SC [14]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch3 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC15 that is mapped to SC [15]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc16 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC16 that is mapped to SC [16]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc17 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC17 that is mapped to SC [17]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc18 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC18 that is mapped to SC [18]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc19 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC19 that is mapped to SC [19]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC20 that is mapped to SC [20]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc21 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC21 that is mapped to SC [21]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC22 that is mapped to SC [22]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc23 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC23 that is mapped to SC [23]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC24 that is mapped to SC [24]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc25 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC25 that is mapped to SC [25]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc26 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC26 that is mapped to SC [26]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc27 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC27 that is mapped to SC [27]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc28 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC28 that is mapped to SC [28]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc29 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC29 that is mapped to SC [29]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc30 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC30 that is mapped to SC [30]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC31 that is mapped to SC [31]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CHANMAP_SC_PG0_PORT0_CHAN2_RS0_CHMAP2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CHANMAP_SC_PG0_PORT0_CHAN2_RS3_CHMAP1_SB_PSF_HCB_REG supported on:     */
/*      SPRMCC (0x20004058)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to map a source channel to a IOSF Channel.Each register maps a source channel to a unique IOSF channel in a specific root space. If a source channel is not mapped, the transaction will use TC-based channel mapping. This field width is variable, and dependent on PSF configuration parameters.
        Details: InstName=psf_2, reg_name=psf_2_CHANMAP_SC_PG0_PORT0_CHAN2_RS3_CHMAP1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.chmapregs.pchmap_pg[0].port[0].pchmap.chan[2].gen.rsi[1].chmap_num[1].chmaptcsc.i_psf20_reg_psf_port_chmap, size=32, reset=0x00000000, address=0x4058, mask=0x1100000f
*/


#define PSF_2_CHANMAP_SC_PG0_PORT0_CHAN2_RS3_CHMAP1_SB_PSF_HCB_REG 0x24224058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sc_pg0_p0_ch0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC0 that is mapped to SC [0]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC1 that is mapped to SC [1]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC2 that is mapped to SC [2]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC3 that is mapped to SC [3]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch0 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC4 that is mapped to SC [4]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch1 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC5 that is mapped to SC [5]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch2 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC6 that is mapped to SC [6]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch3 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC7 that is mapped to SC [7]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch0 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC8 that is mapped to SC [8]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC9 that is mapped to SC [9]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch2 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC10 that is mapped to SC [10]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p2_ch3 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC11 that is mapped to SC [11]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch0 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC12 that is mapped to SC [12]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch1 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC13 that is mapped to SC [13]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch2 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC14 that is mapped to SC [14]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch3 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC15 that is mapped to SC [15]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch0 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC16 that is mapped to SC [16]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch1 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC17 that is mapped to SC [17]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch2 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC18 that is mapped to SC [18]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch3 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC19 that is mapped to SC [19]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch0 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC20 that is mapped to SC [20]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch1 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC21 that is mapped to SC [21]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch2 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC22 that is mapped to SC [22]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch3 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC23 that is mapped to SC [23]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch0 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC24 that is mapped to SC [24]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch1 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC25 that is mapped to SC [25]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch2 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC26 that is mapped to SC [26]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch3 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC27 that is mapped to SC [27]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch0 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC28 that is mapped to SC [28]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch1 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC29 that is mapped to SC [29]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch2 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC30 that is mapped to SC [30]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch3 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC31 that is mapped to SC [31]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CHANMAP_SC_PG0_PORT0_CHAN2_RS3_CHMAP1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CHANMAP_SC_PG0_PORT0_CHAN2_RS3_CHMAP2_SB_PSF_HCB_REG supported on:     */
/*      SPRMCC (0x2000405c)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to map a source channel to a IOSF Channel.Each register maps a source channel to a unique IOSF channel in a specific root space. If a source channel is not mapped, the transaction will use TC-based channel mapping. This field width is variable, and dependent on PSF configuration parameters.
        Details: InstName=psf_2, reg_name=psf_2_CHANMAP_SC_PG0_PORT0_CHAN2_RS3_CHMAP2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.chmapregs.pchmap_pg[0].port[0].pchmap.chan[2].gen.rsi[1].chmap_num[2].chmaptcsc.i_psf20_reg_psf_port_chmap, size=32, reset=0x00000000, address=0x405c, mask=0x00001111
*/


#define PSF_2_CHANMAP_SC_PG0_PORT0_CHAN2_RS3_CHMAP2_SB_PSF_HCB_REG 0x2422405C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sc_pg1_p2_ch0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC0 that is mapped to SC [0]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC1 that is mapped to SC [1]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC2 that is mapped to SC [2]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC3 that is mapped to SC [3]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch0 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC4 that is mapped to SC [4]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch1 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC5 that is mapped to SC [5]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch2 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC6 that is mapped to SC [6]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch3 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC7 that is mapped to SC [7]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch0 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC8 that is mapped to SC [8]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC9 that is mapped to SC [9]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch2 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC10 that is mapped to SC [10]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p4_ch3 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC11 that is mapped to SC [11]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch0 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC12 that is mapped to SC [12]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch1 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC13 that is mapped to SC [13]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch2 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC14 that is mapped to SC [14]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch3 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC15 that is mapped to SC [15]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc16 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC16 that is mapped to SC [16]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc17 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC17 that is mapped to SC [17]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc18 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC18 that is mapped to SC [18]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc19 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC19 that is mapped to SC [19]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC20 that is mapped to SC [20]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc21 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC21 that is mapped to SC [21]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC22 that is mapped to SC [22]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc23 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC23 that is mapped to SC [23]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC24 that is mapped to SC [24]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc25 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC25 that is mapped to SC [25]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc26 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC26 that is mapped to SC [26]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc27 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC27 that is mapped to SC [27]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc28 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC28 that is mapped to SC [28]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc29 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC29 that is mapped to SC [29]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc30 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC30 that is mapped to SC [30]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC31 that is mapped to SC [31]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CHANMAP_SC_PG0_PORT0_CHAN2_RS3_CHMAP2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CHANMAP_SC_PG0_PORT0_CHAN3_RS0_CHMAP1_SB_PSF_HCB_REG supported on:     */
/*      SPRMCC (0x20004060)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to map a source channel to a IOSF Channel.Each register maps a source channel to a unique IOSF channel in a specific root space. If a source channel is not mapped, the transaction will use TC-based channel mapping. This field width is variable, and dependent on PSF configuration parameters.
        Details: InstName=psf_2, reg_name=psf_2_CHANMAP_SC_PG0_PORT0_CHAN3_RS0_CHMAP1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.chmapregs.pchmap_pg[0].port[0].pchmap.chan[3].gen.rsi[0].chmap_num[1].chmaptcsc.i_psf20_reg_psf_port_chmap, size=32, reset=0x00000000, address=0x4060, mask=0x1100000f
*/


#define PSF_2_CHANMAP_SC_PG0_PORT0_CHAN3_RS0_CHMAP1_SB_PSF_HCB_REG 0x24224060

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sc_pg0_p0_ch0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC0 that is mapped to SC [0]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC1 that is mapped to SC [1]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC2 that is mapped to SC [2]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC3 that is mapped to SC [3]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch0 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC4 that is mapped to SC [4]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch1 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC5 that is mapped to SC [5]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch2 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC6 that is mapped to SC [6]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch3 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC7 that is mapped to SC [7]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch0 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC8 that is mapped to SC [8]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC9 that is mapped to SC [9]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch2 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC10 that is mapped to SC [10]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p2_ch3 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC11 that is mapped to SC [11]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch0 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC12 that is mapped to SC [12]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch1 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC13 that is mapped to SC [13]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch2 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC14 that is mapped to SC [14]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch3 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC15 that is mapped to SC [15]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch0 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC16 that is mapped to SC [16]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch1 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC17 that is mapped to SC [17]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch2 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC18 that is mapped to SC [18]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch3 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC19 that is mapped to SC [19]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch0 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC20 that is mapped to SC [20]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch1 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC21 that is mapped to SC [21]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch2 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC22 that is mapped to SC [22]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch3 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC23 that is mapped to SC [23]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch0 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC24 that is mapped to SC [24]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch1 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC25 that is mapped to SC [25]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch2 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC26 that is mapped to SC [26]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch3 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC27 that is mapped to SC [27]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch0 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC28 that is mapped to SC [28]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch1 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC29 that is mapped to SC [29]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch2 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC30 that is mapped to SC [30]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch3 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC31 that is mapped to SC [31]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CHANMAP_SC_PG0_PORT0_CHAN3_RS0_CHMAP1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CHANMAP_SC_PG0_PORT0_CHAN3_RS0_CHMAP2_SB_PSF_HCB_REG supported on:     */
/*      SPRMCC (0x20004064)                                                     */
/* Register default value on SPRMCC: 0x00000010                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to map a source channel to a IOSF Channel.Each register maps a source channel to a unique IOSF channel in a specific root space. If a source channel is not mapped, the transaction will use TC-based channel mapping. This field width is variable, and dependent on PSF configuration parameters.
        Details: InstName=psf_2, reg_name=psf_2_CHANMAP_SC_PG0_PORT0_CHAN3_RS0_CHMAP2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.chmapregs.pchmap_pg[0].port[0].pchmap.chan[3].gen.rsi[0].chmap_num[2].chmaptcsc.i_psf20_reg_psf_port_chmap, size=32, reset=0x00000010, address=0x4064, mask=0x00001111
*/


#define PSF_2_CHANMAP_SC_PG0_PORT0_CHAN3_RS0_CHMAP2_SB_PSF_HCB_REG 0x24224064

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sc_pg1_p2_ch0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC0 that is mapped to SC [0]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC1 that is mapped to SC [1]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC2 that is mapped to SC [2]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC3 that is mapped to SC [3]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch0 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, One-hot encoded source
                               channel SC4 that is mapped to SC [4]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch1 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC5 that is mapped to SC [5]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch2 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC6 that is mapped to SC [6]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch3 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC7 that is mapped to SC [7]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch0 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC8 that is mapped to SC [8]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC9 that is mapped to SC [9]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch2 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC10 that is mapped to SC [10]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p4_ch3 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC11 that is mapped to SC [11]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch0 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC12 that is mapped to SC [12]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch1 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC13 that is mapped to SC [13]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch2 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC14 that is mapped to SC [14]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch3 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC15 that is mapped to SC [15]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc16 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC16 that is mapped to SC [16]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc17 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC17 that is mapped to SC [17]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc18 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC18 that is mapped to SC [18]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc19 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC19 that is mapped to SC [19]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC20 that is mapped to SC [20]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc21 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC21 that is mapped to SC [21]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC22 that is mapped to SC [22]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc23 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC23 that is mapped to SC [23]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC24 that is mapped to SC [24]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc25 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC25 that is mapped to SC [25]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc26 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC26 that is mapped to SC [26]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc27 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC27 that is mapped to SC [27]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc28 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC28 that is mapped to SC [28]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc29 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC29 that is mapped to SC [29]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc30 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC30 that is mapped to SC [30]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC31 that is mapped to SC [31]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CHANMAP_SC_PG0_PORT0_CHAN3_RS0_CHMAP2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CHANMAP_SC_PG0_PORT0_CHAN3_RS3_CHMAP1_SB_PSF_HCB_REG supported on:     */
/*      SPRMCC (0x20004068)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to map a source channel to a IOSF Channel.Each register maps a source channel to a unique IOSF channel in a specific root space. If a source channel is not mapped, the transaction will use TC-based channel mapping. This field width is variable, and dependent on PSF configuration parameters.
        Details: InstName=psf_2, reg_name=psf_2_CHANMAP_SC_PG0_PORT0_CHAN3_RS3_CHMAP1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.chmapregs.pchmap_pg[0].port[0].pchmap.chan[3].gen.rsi[1].chmap_num[1].chmaptcsc.i_psf20_reg_psf_port_chmap, size=32, reset=0x00000000, address=0x4068, mask=0x1100000f
*/


#define PSF_2_CHANMAP_SC_PG0_PORT0_CHAN3_RS3_CHMAP1_SB_PSF_HCB_REG 0x24224068

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sc_pg0_p0_ch0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC0 that is mapped to SC [0]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC1 that is mapped to SC [1]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC2 that is mapped to SC [2]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p0_ch3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC3 that is mapped to SC [3]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch0 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC4 that is mapped to SC [4]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch1 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC5 that is mapped to SC [5]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch2 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC6 that is mapped to SC [6]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p1_ch3 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC7 that is mapped to SC [7]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch0 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC8 that is mapped to SC [8]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC9 that is mapped to SC [9]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg0_p2_ch2 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC10 that is mapped to SC [10]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p2_ch3 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC11 that is mapped to SC [11]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch0 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC12 that is mapped to SC [12]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch1 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC13 that is mapped to SC [13]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch2 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC14 that is mapped to SC [14]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p3_ch3 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC15 that is mapped to SC [15]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch0 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC16 that is mapped to SC [16]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch1 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC17 that is mapped to SC [17]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch2 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC18 that is mapped to SC [18]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p4_ch3 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC19 that is mapped to SC [19]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch0 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC20 that is mapped to SC [20]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch1 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC21 that is mapped to SC [21]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch2 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC22 that is mapped to SC [22]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg0_p5_ch3 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC23 that is mapped to SC [23]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch0 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC24 that is mapped to SC [24]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch1 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC25 that is mapped to SC [25]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch2 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC26 that is mapped to SC [26]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p0_ch3 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC27 that is mapped to SC [27]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch0 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC28 that is mapped to SC [28]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch1 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC29 that is mapped to SC [29]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch2 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC30 that is mapped to SC [30]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p1_ch3 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC31 that is mapped to SC [31]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CHANMAP_SC_PG0_PORT0_CHAN3_RS3_CHMAP1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CHANMAP_SC_PG0_PORT0_CHAN3_RS3_CHMAP2_SB_PSF_HCB_REG supported on:     */
/*      SPRMCC (0x2000406c)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to map a source channel to a IOSF Channel.Each register maps a source channel to a unique IOSF channel in a specific root space. If a source channel is not mapped, the transaction will use TC-based channel mapping. This field width is variable, and dependent on PSF configuration parameters.
        Details: InstName=psf_2, reg_name=psf_2_CHANMAP_SC_PG0_PORT0_CHAN3_RS3_CHMAP2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.chmapregs.pchmap_pg[0].port[0].pchmap.chan[3].gen.rsi[1].chmap_num[2].chmaptcsc.i_psf20_reg_psf_port_chmap, size=32, reset=0x00000000, address=0x406c, mask=0x00001111
*/


#define PSF_2_CHANMAP_SC_PG0_PORT0_CHAN3_RS3_CHMAP2_SB_PSF_HCB_REG 0x2422406C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sc_pg1_p2_ch0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC0 that is mapped to SC [0]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC1 that is mapped to SC [1]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC2 that is mapped to SC [2]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p2_ch3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC3 that is mapped to SC [3]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch0 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC4 that is mapped to SC [4]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch1 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC5 that is mapped to SC [5]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch2 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC6 that is mapped to SC [6]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p3_ch3 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC7 that is mapped to SC [7]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch0 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC8 that is mapped to SC [8]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC9 that is mapped to SC [9]. This field
                               implementation is dependent on PSF configuration
                               parameters.
                            */
    UINT32 sc_pg1_p4_ch2 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC10 that is mapped to SC [10]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p4_ch3 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC11 that is mapped to SC [11]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch0 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC12 that is mapped to SC [12]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch1 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC13 that is mapped to SC [13]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch2 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC14 that is mapped to SC [14]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc_pg1_p5_ch3 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC15 that is mapped to SC [15]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc16 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC16 that is mapped to SC [16]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc17 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC17 that is mapped to SC [17]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc18 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC18 that is mapped to SC [18]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc19 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC19 that is mapped to SC [19]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC20 that is mapped to SC [20]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc21 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC21 that is mapped to SC [21]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC22 that is mapped to SC [22]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc23 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC23 that is mapped to SC [23]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC24 that is mapped to SC [24]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc25 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC25 that is mapped to SC [25]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc26 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC26 that is mapped to SC [26]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc27 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC27 that is mapped to SC [27]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc28 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC28 that is mapped to SC [28]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc29 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC29 that is mapped to SC [29]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc30 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC30 that is mapped to SC [30]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */
    UINT32 sc31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, One-hot encoded source
                               channel SC31 that is mapped to SC [31]. This
                               field implementation is dependent on PSF
                               configuration parameters.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CHANMAP_SC_PG0_PORT0_CHAN3_RS3_CHMAP2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_T1_AGENT_FUNCTION_CONFIG_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x20004070)                                                     */
/* Register default value on SPRMCC: 0x00000090                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register controls the PCI configuration header of a PCI function
        Details: InstName=psf_2, reg_name=psf_2_T1_AGENT_FUNCTION_CONFIG_hqm1_RS0_D9_F0_OFFSET67 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.t1devfun[0].gen.i_psf20_reg_t1func, size=9, reset=0x090, address=0x4070, mask=0x1fe
*/


#define PSF_2_T1_AGENT_FUNCTION_CONFIG_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG 0x24224070

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ifr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If written as a '1', all
                               shadow registers associated with this function
                               will be reset. This bit self-clears after the
                               reset sequence has finished. This bit always
                               reads as 0.
                            */
    UINT32 function : 3;

                            /* Bits[3:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, Function number of the PCI
                               function. This field resets to the function
                               number as configured at compile time
                            */
    UINT32 device : 5;

                            /* Bits[8:4], Access Type=RW, default=0x00000009*/

                            /*
                               Default value=0x9, Device number of the PCI
                               function. This field resets to the device number
                               as configured at compile time
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_T1_AGENT_FUNCTION_CONFIG_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_T1_AGENT_FUNCTION_CONFIG_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x20004074)                                                     */
/* Register default value on SPRMCC: 0x00000080                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register controls the PCI configuration header of a PCI function
        Details: InstName=psf_2, reg_name=psf_2_T1_AGENT_FUNCTION_CONFIG_hqm0_RS0_D8_F0_OFFSET66 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.t1devfun[1].gen.i_psf20_reg_t1func, size=9, reset=0x080, address=0x4074, mask=0x1fe
*/


#define PSF_2_T1_AGENT_FUNCTION_CONFIG_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG 0x24224074

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ifr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If written as a '1', all
                               shadow registers associated with this function
                               will be reset. This bit self-clears after the
                               reset sequence has finished. This bit always
                               reads as 0.
                            */
    UINT32 function : 3;

                            /* Bits[3:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, Function number of the PCI
                               function. This field resets to the function
                               number as configured at compile time
                            */
    UINT32 device : 5;

                            /* Bits[8:4], Access Type=RW, default=0x00000008*/

                            /*
                               Default value=0x8, Device number of the PCI
                               function. This field resets to the device number
                               as configured at compile time
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_T1_AGENT_FUNCTION_CONFIG_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_T1_AGENT_FUNCTION_CONFIG_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x20004078)                                                     */
/* Register default value on SPRMCC: 0x00000070                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register controls the PCI configuration header of a PCI function
        Details: InstName=psf_2, reg_name=psf_2_T1_AGENT_FUNCTION_CONFIG_cpm1_RS0_D7_F0_OFFSET65 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.t1devfun[2].gen.i_psf20_reg_t1func, size=9, reset=0x070, address=0x4078, mask=0x1fe
*/


#define PSF_2_T1_AGENT_FUNCTION_CONFIG_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG 0x24224078

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ifr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If written as a '1', all
                               shadow registers associated with this function
                               will be reset. This bit self-clears after the
                               reset sequence has finished. This bit always
                               reads as 0.
                            */
    UINT32 function : 3;

                            /* Bits[3:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, Function number of the PCI
                               function. This field resets to the function
                               number as configured at compile time
                            */
    UINT32 device : 5;

                            /* Bits[8:4], Access Type=RW, default=0x00000007*/

                            /*
                               Default value=0x7, Device number of the PCI
                               function. This field resets to the device number
                               as configured at compile time
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_T1_AGENT_FUNCTION_CONFIG_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_T1_AGENT_FUNCTION_CONFIG_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x2000407c)                                                     */
/* Register default value on SPRMCC: 0x00000060                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register controls the PCI configuration header of a PCI function
        Details: InstName=psf_2, reg_name=psf_2_T1_AGENT_FUNCTION_CONFIG_cpm0_RS0_D6_F0_OFFSET64 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.t1devfun[3].gen.i_psf20_reg_t1func, size=9, reset=0x060, address=0x407c, mask=0x1fe
*/


#define PSF_2_T1_AGENT_FUNCTION_CONFIG_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG 0x2422407C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ifr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If written as a '1', all
                               shadow registers associated with this function
                               will be reset. This bit self-clears after the
                               reset sequence has finished. This bit always
                               reads as 0.
                            */
    UINT32 function : 3;

                            /* Bits[3:1], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, Function number of the PCI
                               function. This field resets to the function
                               number as configured at compile time
                            */
    UINT32 device : 5;

                            /* Bits[8:4], Access Type=RW, default=0x00000006*/

                            /*
                               Default value=0x6, Device number of the PCI
                               function. This field resets to the device number
                               as configured at compile time
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_T1_AGENT_FUNCTION_CONFIG_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_RS_IFR_SB_PSF_HCB_REG supported on:                                    */
/*      SPRMCC (0x20004080)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to reset all functions in a particular Root Space simultaneously.
        Details: InstName=psf_2, reg_name=psf_2_RS_IFR : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.i_psf20_rs_ifr, size=4, reset=0x00000000, address=0x4080, mask=0xFFFFFFFF
*/


#define PSF_2_RS_IFR_SB_PSF_HCB_REG 0x24224080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsifr : 4;

                            /* Bits[3:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If RSIFR [n] is written as a
                               '1', all shadow registers associated with all
                               functions in Root Space n will be reset. These
                               bits self-clear after the reset sequence has
                               finished. These bits always read as 0. This
                               field width is variable, and dependent on PSF
                               configuration parameters.
                            */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_RS_IFR_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_DEV_GNTCNT_RELOAD_DGCR0_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x20004084)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_DEV_GNTCNT_RELOAD_DGCR0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.dgcr[0].i_psf20_reg_devgcnt_p, size=6, reset=0x02, address=4084, mask=0x3f
*/


#define PSF_2_DEV_GNTCNT_RELOAD_DGCR0_SB_PSF_HCB_REG 0x24224084

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_DEV_GNTCNT_RELOAD_DGCR0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_DEV_GNTCNT_RELOAD_DGCR1_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x20004088)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_DEV_GNTCNT_RELOAD_DGCR1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.dgcr[1].i_psf20_reg_devgcnt_p, size=6, reset=0x02, address=4088, mask=0x3f
*/


#define PSF_2_DEV_GNTCNT_RELOAD_DGCR1_SB_PSF_HCB_REG 0x24224088

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_DEV_GNTCNT_RELOAD_DGCR1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_DEV_GNTCNT_RELOAD_DGCR2_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x2000408c)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_DEV_GNTCNT_RELOAD_DGCR2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.dgcr[2].i_psf20_reg_devgcnt_p, size=6, reset=0x02, address=408c, mask=0x3f
*/


#define PSF_2_DEV_GNTCNT_RELOAD_DGCR2_SB_PSF_HCB_REG 0x2422408C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_DEV_GNTCNT_RELOAD_DGCR2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_DEV_GNTCNT_RELOAD_DGCR3_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x20004090)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_DEV_GNTCNT_RELOAD_DGCR3 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.dgcr[3].i_psf20_reg_devgcnt_p, size=6, reset=0x02, address=4090, mask=0x3f
*/


#define PSF_2_DEV_GNTCNT_RELOAD_DGCR3_SB_PSF_HCB_REG 0x24224090

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_DEV_GNTCNT_RELOAD_DGCR3_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_DEV_GNTCNT_RELOAD_DGCR4_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x20004094)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_DEV_GNTCNT_RELOAD_DGCR4 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.dgcr[4].i_psf20_reg_devgcnt_p, size=6, reset=0x02, address=4094, mask=0x3f
*/


#define PSF_2_DEV_GNTCNT_RELOAD_DGCR4_SB_PSF_HCB_REG 0x24224094

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_DEV_GNTCNT_RELOAD_DGCR4_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_DEV_GNTCNT_RELOAD_DGCR5_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x20004098)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_DEV_GNTCNT_RELOAD_DGCR5 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.dgcr[5].i_psf20_reg_devgcnt_p, size=6, reset=0x02, address=4098, mask=0x3f
*/


#define PSF_2_DEV_GNTCNT_RELOAD_DGCR5_SB_PSF_HCB_REG 0x24224098

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_DEV_GNTCNT_RELOAD_DGCR5_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_DEV_GNTCNT_RELOAD_DGCR6_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x2000409c)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_DEV_GNTCNT_RELOAD_DGCR6 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.dgcr[6].i_psf20_reg_devgcnt_p, size=6, reset=0x02, address=409c, mask=0x3f
*/


#define PSF_2_DEV_GNTCNT_RELOAD_DGCR6_SB_PSF_HCB_REG 0x2422409C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_DEV_GNTCNT_RELOAD_DGCR6_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_DEV_GNTCNT_RELOAD_DGCR7_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x200040a0)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_DEV_GNTCNT_RELOAD_DGCR7 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.dgcr[7].i_psf20_reg_devgcnt_p, size=6, reset=0x02, address=40a0, mask=0x3f
*/


#define PSF_2_DEV_GNTCNT_RELOAD_DGCR7_SB_PSF_HCB_REG 0x242240A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_DEV_GNTCNT_RELOAD_DGCR7_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_DEV_GNTCNT_RELOAD_DGCR8_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x200040a4)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_DEV_GNTCNT_RELOAD_DGCR8 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.dgcr[8].i_psf20_reg_devgcnt_p, size=6, reset=0x02, address=40a4, mask=0x3f
*/


#define PSF_2_DEV_GNTCNT_RELOAD_DGCR8_SB_PSF_HCB_REG 0x242240A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_DEV_GNTCNT_RELOAD_DGCR8_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_P_GNTCNT_RELOAD_PG0_TGT0_SB_PSF_HCB_REG supported on:            */
/*      SPRMCC (0x200040a8)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_P_GNTCNT_RELOAD_PG0_TGT0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[0].max_tgt[0].cgcnt_rld_p.i_psf20_reg_classgcnt_p, size=6, reset=0x02, address=40a8, mask=0x3f
*/


#define PSF_2_CLASS_P_GNTCNT_RELOAD_PG0_TGT0_SB_PSF_HCB_REG 0x242240A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_P_GNTCNT_RELOAD_PG0_TGT0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_NP_GNTCNT_RELOAD_PG0_TGT0_SB_PSF_HCB_REG supported on:           */
/*      SPRMCC (0x200040ac)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_NP_GNTCNT_RELOAD_PG0_TGT0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[0].max_tgt[0].cgcnt_rld_p_np.i_psf20_reg_classgcnt_np, size=6, reset=0x02, address=40ac, mask=0x3f
*/


#define PSF_2_CLASS_NP_GNTCNT_RELOAD_PG0_TGT0_SB_PSF_HCB_REG 0x242240AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_NP_GNTCNT_RELOAD_PG0_TGT0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG0_TGT0_SB_PSF_HCB_REG supported on:          */
/*      SPRMCC (0x200040b0)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_CPL_GNTCNT_RELOAD_PG0_TGT0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[0].max_tgt[0].cgcnt_rld_c.i_psf20_reg_classgcnt_c, size=6, reset=0x02, address=40b0, mask=0x3f
*/


#define PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG0_TGT0_SB_PSF_HCB_REG 0x242240B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG0_TGT0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_P_GNTCNT_RELOAD_PG0_TGT1_SB_PSF_HCB_REG supported on:            */
/*      SPRMCC (0x200040b4)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_P_GNTCNT_RELOAD_PG0_TGT1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[0].max_tgt[1].cgcnt_rld_p.i_psf20_reg_classgcnt_p, size=6, reset=0x02, address=40b4, mask=0x3f
*/


#define PSF_2_CLASS_P_GNTCNT_RELOAD_PG0_TGT1_SB_PSF_HCB_REG 0x242240B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_P_GNTCNT_RELOAD_PG0_TGT1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_NP_GNTCNT_RELOAD_PG0_TGT1_SB_PSF_HCB_REG supported on:           */
/*      SPRMCC (0x200040b8)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_NP_GNTCNT_RELOAD_PG0_TGT1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[0].max_tgt[1].cgcnt_rld_p_np.i_psf20_reg_classgcnt_np, size=6, reset=0x02, address=40b8, mask=0x3f
*/


#define PSF_2_CLASS_NP_GNTCNT_RELOAD_PG0_TGT1_SB_PSF_HCB_REG 0x242240B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_NP_GNTCNT_RELOAD_PG0_TGT1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG0_TGT1_SB_PSF_HCB_REG supported on:          */
/*      SPRMCC (0x200040bc)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_CPL_GNTCNT_RELOAD_PG0_TGT1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[0].max_tgt[1].cgcnt_rld_c.i_psf20_reg_classgcnt_c, size=6, reset=0x02, address=40bc, mask=0x3f
*/


#define PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG0_TGT1_SB_PSF_HCB_REG 0x242240BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG0_TGT1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_P_GNTCNT_RELOAD_PG0_TGT2_SB_PSF_HCB_REG supported on:            */
/*      SPRMCC (0x200040c0)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_P_GNTCNT_RELOAD_PG0_TGT2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[0].max_tgt[2].cgcnt_rld_p.i_psf20_reg_classgcnt_p, size=6, reset=0x02, address=40c0, mask=0x3f
*/


#define PSF_2_CLASS_P_GNTCNT_RELOAD_PG0_TGT2_SB_PSF_HCB_REG 0x242240C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_P_GNTCNT_RELOAD_PG0_TGT2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_NP_GNTCNT_RELOAD_PG0_TGT2_SB_PSF_HCB_REG supported on:           */
/*      SPRMCC (0x200040c4)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_NP_GNTCNT_RELOAD_PG0_TGT2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[0].max_tgt[2].cgcnt_rld_p_np.i_psf20_reg_classgcnt_np, size=6, reset=0x02, address=40c4, mask=0x3f
*/


#define PSF_2_CLASS_NP_GNTCNT_RELOAD_PG0_TGT2_SB_PSF_HCB_REG 0x242240C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_NP_GNTCNT_RELOAD_PG0_TGT2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG0_TGT2_SB_PSF_HCB_REG supported on:          */
/*      SPRMCC (0x200040c8)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_CPL_GNTCNT_RELOAD_PG0_TGT2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[0].max_tgt[2].cgcnt_rld_c.i_psf20_reg_classgcnt_c, size=6, reset=0x02, address=40c8, mask=0x3f
*/


#define PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG0_TGT2_SB_PSF_HCB_REG 0x242240C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG0_TGT2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_P_GNTCNT_RELOAD_PG0_TGT3_SB_PSF_HCB_REG supported on:            */
/*      SPRMCC (0x200040cc)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_P_GNTCNT_RELOAD_PG0_TGT3 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[0].max_tgt[3].cgcnt_rld_p.i_psf20_reg_classgcnt_p, size=6, reset=0x02, address=40cc, mask=0x3f
*/


#define PSF_2_CLASS_P_GNTCNT_RELOAD_PG0_TGT3_SB_PSF_HCB_REG 0x242240CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_P_GNTCNT_RELOAD_PG0_TGT3_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_NP_GNTCNT_RELOAD_PG0_TGT3_SB_PSF_HCB_REG supported on:           */
/*      SPRMCC (0x200040d0)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_NP_GNTCNT_RELOAD_PG0_TGT3 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[0].max_tgt[3].cgcnt_rld_p_np.i_psf20_reg_classgcnt_np, size=6, reset=0x02, address=40d0, mask=0x3f
*/


#define PSF_2_CLASS_NP_GNTCNT_RELOAD_PG0_TGT3_SB_PSF_HCB_REG 0x242240D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_NP_GNTCNT_RELOAD_PG0_TGT3_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG0_TGT3_SB_PSF_HCB_REG supported on:          */
/*      SPRMCC (0x200040d4)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_CPL_GNTCNT_RELOAD_PG0_TGT3 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[0].max_tgt[3].cgcnt_rld_c.i_psf20_reg_classgcnt_c, size=6, reset=0x02, address=40d4, mask=0x3f
*/


#define PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG0_TGT3_SB_PSF_HCB_REG 0x242240D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG0_TGT3_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT0_SB_PSF_HCB_REG supported on:            */
/*      SPRMCC (0x200040d8)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[1].max_tgt[0].cgcnt_rld_p.i_psf20_reg_classgcnt_p, size=6, reset=0x02, address=40d8, mask=0x3f
*/


#define PSF_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT0_SB_PSF_HCB_REG 0x242240D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT0_SB_PSF_HCB_REG supported on:           */
/*      SPRMCC (0x200040dc)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[1].max_tgt[0].cgcnt_rld_p_np.i_psf20_reg_classgcnt_np, size=6, reset=0x02, address=40dc, mask=0x3f
*/


#define PSF_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT0_SB_PSF_HCB_REG 0x242240DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT0_SB_PSF_HCB_REG supported on:          */
/*      SPRMCC (0x200040e0)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[1].max_tgt[0].cgcnt_rld_c.i_psf20_reg_classgcnt_c, size=6, reset=0x02, address=40e0, mask=0x3f
*/


#define PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT0_SB_PSF_HCB_REG 0x242240E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT4_SB_PSF_HCB_REG supported on:            */
/*      SPRMCC (0x200040e4)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT4 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[1].max_tgt[4].cgcnt_rld_p.i_psf20_reg_classgcnt_p, size=6, reset=0x02, address=40e4, mask=0x3f
*/


#define PSF_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT4_SB_PSF_HCB_REG 0x242240E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT4_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT4_SB_PSF_HCB_REG supported on:           */
/*      SPRMCC (0x200040e8)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT4 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[1].max_tgt[4].cgcnt_rld_p_np.i_psf20_reg_classgcnt_np, size=6, reset=0x02, address=40e8, mask=0x3f
*/


#define PSF_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT4_SB_PSF_HCB_REG 0x242240E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT4_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT4_SB_PSF_HCB_REG supported on:          */
/*      SPRMCC (0x200040ec)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT4 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[1].max_tgt[4].cgcnt_rld_c.i_psf20_reg_classgcnt_c, size=6, reset=0x02, address=40ec, mask=0x3f
*/


#define PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT4_SB_PSF_HCB_REG 0x242240EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT4_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT8_SB_PSF_HCB_REG supported on:            */
/*      SPRMCC (0x200040f0)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT8 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[1].max_tgt[8].cgcnt_rld_p.i_psf20_reg_classgcnt_p, size=6, reset=0x02, address=40f0, mask=0x3f
*/


#define PSF_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT8_SB_PSF_HCB_REG 0x242240F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT8_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT8_SB_PSF_HCB_REG supported on:           */
/*      SPRMCC (0x200040f4)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT8 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[1].max_tgt[8].cgcnt_rld_p_np.i_psf20_reg_classgcnt_np, size=6, reset=0x02, address=40f4, mask=0x3f
*/


#define PSF_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT8_SB_PSF_HCB_REG 0x242240F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT8_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT8_SB_PSF_HCB_REG supported on:          */
/*      SPRMCC (0x200040f8)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT8 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[1].max_tgt[8].cgcnt_rld_c.i_psf20_reg_classgcnt_c, size=6, reset=0x02, address=40f8, mask=0x3f
*/


#define PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT8_SB_PSF_HCB_REG 0x242240F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT8_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT12_SB_PSF_HCB_REG supported on:           */
/*      SPRMCC (0x200040fc)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT12 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[1].max_tgt[12].cgcnt_rld_p.i_psf20_reg_classgcnt_p, size=6, reset=0x02, address=40fc, mask=0x3f
*/


#define PSF_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT12_SB_PSF_HCB_REG 0x242240FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT12_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT12_SB_PSF_HCB_REG supported on:          */
/*      SPRMCC (0x20004100)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT12 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[1].max_tgt[12].cgcnt_rld_p_np.i_psf20_reg_classgcnt_np, size=6, reset=0x02, address=4100, mask=0x3f
*/


#define PSF_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT12_SB_PSF_HCB_REG 0x24224100

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT12_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT12_SB_PSF_HCB_REG supported on:         */
/*      SPRMCC (0x20004104)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT12 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[1].max_tgt[12].cgcnt_rld_c.i_psf20_reg_classgcnt_c, size=6, reset=0x02, address=4104, mask=0x3f
*/


#define PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT12_SB_PSF_HCB_REG 0x24224104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT12_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT16_SB_PSF_HCB_REG supported on:           */
/*      SPRMCC (0x20004108)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT16 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[1].max_tgt[16].cgcnt_rld_p.i_psf20_reg_classgcnt_p, size=6, reset=0x02, address=4108, mask=0x3f
*/


#define PSF_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT16_SB_PSF_HCB_REG 0x24224108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT16_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT16_SB_PSF_HCB_REG supported on:          */
/*      SPRMCC (0x2000410c)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT16 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[1].max_tgt[16].cgcnt_rld_p_np.i_psf20_reg_classgcnt_np, size=6, reset=0x02, address=410c, mask=0x3f
*/


#define PSF_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT16_SB_PSF_HCB_REG 0x2422410C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT16_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT16_SB_PSF_HCB_REG supported on:         */
/*      SPRMCC (0x20004110)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT16 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[1].max_tgt[16].cgcnt_rld_c.i_psf20_reg_classgcnt_c, size=6, reset=0x02, address=4110, mask=0x3f
*/


#define PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT16_SB_PSF_HCB_REG 0x24224110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT16_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT20_SB_PSF_HCB_REG supported on:           */
/*      SPRMCC (0x20004114)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT20 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[1].max_tgt[20].cgcnt_rld_p.i_psf20_reg_classgcnt_p, size=6, reset=0x02, address=4114, mask=0x3f
*/


#define PSF_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT20_SB_PSF_HCB_REG 0x24224114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_P_GNTCNT_RELOAD_PG1_TGT20_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT20_SB_PSF_HCB_REG supported on:          */
/*      SPRMCC (0x20004118)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT20 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[1].max_tgt[20].cgcnt_rld_p_np.i_psf20_reg_classgcnt_np, size=6, reset=0x02, address=4118, mask=0x3f
*/


#define PSF_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT20_SB_PSF_HCB_REG 0x24224118

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_NP_GNTCNT_RELOAD_PG1_TGT20_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT20_SB_PSF_HCB_REG supported on:         */
/*      SPRMCC (0x2000411c)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT20 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.cgcnt_tgt_pg[1].max_tgt[20].cgcnt_rld_c.i_psf20_reg_classgcnt_c, size=6, reset=0x02, address=411c, mask=0x3f
*/


#define PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT20_SB_PSF_HCB_REG 0x2422411C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_CLASS_CPL_GNTCNT_RELOAD_PG1_TGT20_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_TARGET_GNTCNT_RELOAD_PG0_TGT0_SB_PSF_HCB_REG supported on:             */
/*      SPRMCC (0x20004120)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_TARGET_GNTCNT_RELOAD_PG0_TGT0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.tgcnt_tgt_pg[0].max_tgt[0].tgcnt_rld.i_psf20_reg_targetgcnt, size=6, reset=0x02, address=4120, mask=0x3f
*/


#define PSF_2_TARGET_GNTCNT_RELOAD_PG0_TGT0_SB_PSF_HCB_REG 0x24224120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_TARGET_GNTCNT_RELOAD_PG0_TGT0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_TARGET_GNTCNT_RELOAD_PG0_TGT1_SB_PSF_HCB_REG supported on:             */
/*      SPRMCC (0x20004124)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_TARGET_GNTCNT_RELOAD_PG0_TGT1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.tgcnt_tgt_pg[0].max_tgt[1].tgcnt_rld.i_psf20_reg_targetgcnt, size=6, reset=0x02, address=4124, mask=0x3f
*/


#define PSF_2_TARGET_GNTCNT_RELOAD_PG0_TGT1_SB_PSF_HCB_REG 0x24224124

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_TARGET_GNTCNT_RELOAD_PG0_TGT1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_TARGET_GNTCNT_RELOAD_PG0_TGT2_SB_PSF_HCB_REG supported on:             */
/*      SPRMCC (0x20004128)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_TARGET_GNTCNT_RELOAD_PG0_TGT2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.tgcnt_tgt_pg[0].max_tgt[2].tgcnt_rld.i_psf20_reg_targetgcnt, size=6, reset=0x02, address=4128, mask=0x3f
*/


#define PSF_2_TARGET_GNTCNT_RELOAD_PG0_TGT2_SB_PSF_HCB_REG 0x24224128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_TARGET_GNTCNT_RELOAD_PG0_TGT2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_TARGET_GNTCNT_RELOAD_PG0_TGT3_SB_PSF_HCB_REG supported on:             */
/*      SPRMCC (0x2000412c)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_TARGET_GNTCNT_RELOAD_PG0_TGT3 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.tgcnt_tgt_pg[0].max_tgt[3].tgcnt_rld.i_psf20_reg_targetgcnt, size=6, reset=0x02, address=412c, mask=0x3f
*/


#define PSF_2_TARGET_GNTCNT_RELOAD_PG0_TGT3_SB_PSF_HCB_REG 0x2422412C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_TARGET_GNTCNT_RELOAD_PG0_TGT3_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_TARGET_GNTCNT_RELOAD_PG1_TGT0_SB_PSF_HCB_REG supported on:             */
/*      SPRMCC (0x20004130)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_TARGET_GNTCNT_RELOAD_PG1_TGT0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.tgcnt_tgt_pg[1].max_tgt[0].tgcnt_rld.i_psf20_reg_targetgcnt, size=6, reset=0x02, address=4130, mask=0x3f
*/


#define PSF_2_TARGET_GNTCNT_RELOAD_PG1_TGT0_SB_PSF_HCB_REG 0x24224130

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_TARGET_GNTCNT_RELOAD_PG1_TGT0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_TARGET_GNTCNT_RELOAD_PG1_TGT4_SB_PSF_HCB_REG supported on:             */
/*      SPRMCC (0x20004134)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_TARGET_GNTCNT_RELOAD_PG1_TGT4 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.tgcnt_tgt_pg[1].max_tgt[4].tgcnt_rld.i_psf20_reg_targetgcnt, size=6, reset=0x02, address=4134, mask=0x3f
*/


#define PSF_2_TARGET_GNTCNT_RELOAD_PG1_TGT4_SB_PSF_HCB_REG 0x24224134

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_TARGET_GNTCNT_RELOAD_PG1_TGT4_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_TARGET_GNTCNT_RELOAD_PG1_TGT8_SB_PSF_HCB_REG supported on:             */
/*      SPRMCC (0x20004138)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_TARGET_GNTCNT_RELOAD_PG1_TGT8 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.tgcnt_tgt_pg[1].max_tgt[8].tgcnt_rld.i_psf20_reg_targetgcnt, size=6, reset=0x02, address=4138, mask=0x3f
*/


#define PSF_2_TARGET_GNTCNT_RELOAD_PG1_TGT8_SB_PSF_HCB_REG 0x24224138

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_TARGET_GNTCNT_RELOAD_PG1_TGT8_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_TARGET_GNTCNT_RELOAD_PG1_TGT12_SB_PSF_HCB_REG supported on:            */
/*      SPRMCC (0x2000413c)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_TARGET_GNTCNT_RELOAD_PG1_TGT12 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.tgcnt_tgt_pg[1].max_tgt[12].tgcnt_rld.i_psf20_reg_targetgcnt, size=6, reset=0x02, address=413c, mask=0x3f
*/


#define PSF_2_TARGET_GNTCNT_RELOAD_PG1_TGT12_SB_PSF_HCB_REG 0x2422413C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_TARGET_GNTCNT_RELOAD_PG1_TGT12_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_TARGET_GNTCNT_RELOAD_PG1_TGT16_SB_PSF_HCB_REG supported on:            */
/*      SPRMCC (0x20004140)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_TARGET_GNTCNT_RELOAD_PG1_TGT16 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.tgcnt_tgt_pg[1].max_tgt[16].tgcnt_rld.i_psf20_reg_targetgcnt, size=6, reset=0x02, address=4140, mask=0x3f
*/


#define PSF_2_TARGET_GNTCNT_RELOAD_PG1_TGT16_SB_PSF_HCB_REG 0x24224140

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_TARGET_GNTCNT_RELOAD_PG1_TGT16_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_TARGET_GNTCNT_RELOAD_PG1_TGT20_SB_PSF_HCB_REG supported on:            */
/*      SPRMCC (0x20004144)                                                     */
/* Register default value on SPRMCC: 0x00000002                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_TARGET_GNTCNT_RELOAD_PG1_TGT20 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.tgcnt_tgt_pg[1].max_tgt[20].tgcnt_rld.i_psf20_reg_targetgcnt, size=6, reset=0x02, address=4144, mask=0x3f
*/


#define PSF_2_TARGET_GNTCNT_RELOAD_PG1_TGT20_SB_PSF_HCB_REG 0x24224144

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000002*/

                            /*
                               Default value=0x2, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_TARGET_GNTCNT_RELOAD_PG1_TGT20_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_RLD0_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x20004148)                                                     */
/* Register default value on SPRMCC: 0x00000010                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_RLD0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.lcnt_rld[0].i_psf20_reg_linkgcnt, size=6, reset=0x10, address=4148, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_RLD0_SB_PSF_HCB_REG 0x24224148

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000010*/

                            /*
                               Default value=0x10, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_RLD0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_RLD1_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x2000414c)                                                     */
/* Register default value on SPRMCC: 0x00000010                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_RLD1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.lcnt_rld[1].i_psf20_reg_linkgcnt, size=6, reset=0x10, address=414c, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_RLD1_SB_PSF_HCB_REG 0x2422414C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000010*/

                            /*
                               Default value=0x10, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_RLD1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_RLD2_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x20004150)                                                     */
/* Register default value on SPRMCC: 0x00000010                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_RLD2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.lcnt_rld[2].i_psf20_reg_linkgcnt, size=6, reset=0x10, address=4150, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_RLD2_SB_PSF_HCB_REG 0x24224150

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000010*/

                            /*
                               Default value=0x10, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_RLD2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_RLD3_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x20004154)                                                     */
/* Register default value on SPRMCC: 0x00000010                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_RLD3 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.lcnt_rld[3].i_psf20_reg_linkgcnt, size=6, reset=0x10, address=4154, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_RLD3_SB_PSF_HCB_REG 0x24224154

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000010*/

                            /*
                               Default value=0x10, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_RLD3_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_RLD4_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x20004158)                                                     */
/* Register default value on SPRMCC: 0x00000010                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_RLD4 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.lcnt_rld[4].i_psf20_reg_linkgcnt, size=6, reset=0x10, address=4158, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_RLD4_SB_PSF_HCB_REG 0x24224158

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000010*/

                            /*
                               Default value=0x10, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_RLD4_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_RLD5_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x2000415c)                                                     */
/* Register default value on SPRMCC: 0x00000010                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_RLD5 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.lcnt_rld[5].i_psf20_reg_linkgcnt, size=6, reset=0x10, address=415c, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_RLD5_SB_PSF_HCB_REG 0x2422415C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000010*/

                            /*
                               Default value=0x10, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_RLD5_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T0_C0_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x20004160)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T0_C0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[0].max_port_t[0].max_chan_t[0].gress_p_np_c[0].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=4160, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T0_C0_SB_PSF_HCB_REG 0x24224160

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T0_C0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T0_C1_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x20004164)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T0_C1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[0].max_port_t[0].max_chan_t[0].gress_p_np_c[1].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=4164, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T0_C1_SB_PSF_HCB_REG 0x24224164

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T0_C1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T0_C2_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x20004168)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T0_C2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[0].max_port_t[0].max_chan_t[0].gress_p_np_c[2].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=4168, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T0_C2_SB_PSF_HCB_REG 0x24224168

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T0_C2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T1_C0_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x2000416c)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T1_C0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[0].max_port_t[0].max_chan_t[1].gress_p_np_c[0].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=416c, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T1_C0_SB_PSF_HCB_REG 0x2422416C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T1_C0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T1_C2_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x20004170)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T1_C2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[0].max_port_t[0].max_chan_t[1].gress_p_np_c[2].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=4170, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T1_C2_SB_PSF_HCB_REG 0x24224170

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T1_C2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T2_C0_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x20004174)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T2_C0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[0].max_port_t[0].max_chan_t[2].gress_p_np_c[0].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=4174, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T2_C0_SB_PSF_HCB_REG 0x24224174

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T2_C0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T2_C1_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x20004178)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T2_C1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[0].max_port_t[0].max_chan_t[2].gress_p_np_c[1].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=4178, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T2_C1_SB_PSF_HCB_REG 0x24224178

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T2_C1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T2_C2_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x2000417c)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T2_C2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[0].max_port_t[0].max_chan_t[2].gress_p_np_c[2].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=417c, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T2_C2_SB_PSF_HCB_REG 0x2422417C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T2_C2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T3_C0_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x20004180)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T3_C0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[0].max_port_t[0].max_chan_t[3].gress_p_np_c[0].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=4180, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T3_C0_SB_PSF_HCB_REG 0x24224180

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T3_C0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T3_C2_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x20004184)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T3_C2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[0].max_port_t[0].max_chan_t[3].gress_p_np_c[2].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=4184, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T3_C2_SB_PSF_HCB_REG 0x24224184

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG0_T0_T3_C2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T0_T0_C0_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x20004188)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T0_T0_C0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[1].max_port_t[0].max_chan_t[0].gress_p_np_c[0].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=4188, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T0_T0_C0_SB_PSF_HCB_REG 0x24224188

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T0_T0_C0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T0_T0_C1_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x2000418c)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T0_T0_C1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[1].max_port_t[0].max_chan_t[0].gress_p_np_c[1].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=418c, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T0_T0_C1_SB_PSF_HCB_REG 0x2422418C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T0_T0_C1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T0_T0_C2_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x20004190)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T0_T0_C2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[1].max_port_t[0].max_chan_t[0].gress_p_np_c[2].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=4190, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T0_T0_C2_SB_PSF_HCB_REG 0x24224190

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T0_T0_C2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T1_T0_C0_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x20004194)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T1_T0_C0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[1].max_port_t[1].max_chan_t[0].gress_p_np_c[0].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=4194, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T1_T0_C0_SB_PSF_HCB_REG 0x24224194

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T1_T0_C0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T1_T0_C1_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x20004198)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T1_T0_C1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[1].max_port_t[1].max_chan_t[0].gress_p_np_c[1].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=4198, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T1_T0_C1_SB_PSF_HCB_REG 0x24224198

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T1_T0_C1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T1_T0_C2_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x2000419c)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T1_T0_C2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[1].max_port_t[1].max_chan_t[0].gress_p_np_c[2].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=419c, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T1_T0_C2_SB_PSF_HCB_REG 0x2422419C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T1_T0_C2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T2_T0_C0_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041a0)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T2_T0_C0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[1].max_port_t[2].max_chan_t[0].gress_p_np_c[0].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=41a0, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T2_T0_C0_SB_PSF_HCB_REG 0x242241A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T2_T0_C0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T2_T0_C2_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041a4)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T2_T0_C2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[1].max_port_t[2].max_chan_t[0].gress_p_np_c[2].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=41a4, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T2_T0_C2_SB_PSF_HCB_REG 0x242241A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T2_T0_C2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T3_T0_C0_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041a8)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T3_T0_C0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[1].max_port_t[3].max_chan_t[0].gress_p_np_c[0].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=41a8, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T3_T0_C0_SB_PSF_HCB_REG 0x242241A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T3_T0_C0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T3_T0_C2_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041ac)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T3_T0_C2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[1].max_port_t[3].max_chan_t[0].gress_p_np_c[2].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=41ac, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T3_T0_C2_SB_PSF_HCB_REG 0x242241AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T3_T0_C2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T4_T0_C0_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041b0)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T4_T0_C0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[1].max_port_t[4].max_chan_t[0].gress_p_np_c[0].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=41b0, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T4_T0_C0_SB_PSF_HCB_REG 0x242241B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T4_T0_C0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T4_T0_C1_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041b4)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T4_T0_C1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[1].max_port_t[4].max_chan_t[0].gress_p_np_c[1].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=41b4, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T4_T0_C1_SB_PSF_HCB_REG 0x242241B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T4_T0_C1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T4_T0_C2_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041b8)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T4_T0_C2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[0].port_cgcnt_pg[1].max_port_t[4].max_chan_t[0].gress_p_np_c[2].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=41b8, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T4_T0_C2_SB_PSF_HCB_REG 0x242241B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS0_PG1_T4_T0_C2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T0_C0_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041bc)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T0_C0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[0].max_port_t[0].max_chan_t[0].gress_p_np_c[0].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=41bc, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T0_C0_SB_PSF_HCB_REG 0x242241BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T0_C0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T0_C1_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041c0)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T0_C1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[0].max_port_t[0].max_chan_t[0].gress_p_np_c[1].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=41c0, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T0_C1_SB_PSF_HCB_REG 0x242241C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T0_C1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T0_C2_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041c4)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T0_C2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[0].max_port_t[0].max_chan_t[0].gress_p_np_c[2].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=41c4, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T0_C2_SB_PSF_HCB_REG 0x242241C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T0_C2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T1_C0_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041c8)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T1_C0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[0].max_port_t[0].max_chan_t[1].gress_p_np_c[0].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=41c8, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T1_C0_SB_PSF_HCB_REG 0x242241C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T1_C0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T1_C1_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041cc)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T1_C1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[0].max_port_t[0].max_chan_t[1].gress_p_np_c[1].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=41cc, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T1_C1_SB_PSF_HCB_REG 0x242241CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T1_C1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T2_C0_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041d0)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T2_C0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[0].max_port_t[0].max_chan_t[2].gress_p_np_c[0].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=41d0, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T2_C0_SB_PSF_HCB_REG 0x242241D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T2_C0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T2_C1_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041d4)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T2_C1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[0].max_port_t[0].max_chan_t[2].gress_p_np_c[1].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=41d4, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T2_C1_SB_PSF_HCB_REG 0x242241D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T2_C1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T2_C2_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041d8)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T2_C2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[0].max_port_t[0].max_chan_t[2].gress_p_np_c[2].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=41d8, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T2_C2_SB_PSF_HCB_REG 0x242241D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T2_C2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T3_C0_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041dc)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T3_C0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[0].max_port_t[0].max_chan_t[3].gress_p_np_c[0].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=41dc, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T3_C0_SB_PSF_HCB_REG 0x242241DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T3_C0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T3_C1_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041e0)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T3_C1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[0].max_port_t[0].max_chan_t[3].gress_p_np_c[1].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x01, address=41e0, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T3_C1_SB_PSF_HCB_REG 0x242241E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG0_T0_T3_C1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T0_T0_C0_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041e4)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T0_T0_C0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[1].max_port_t[0].max_chan_t[0].gress_p_np_c[0].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=41e4, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T0_T0_C0_SB_PSF_HCB_REG 0x242241E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T0_T0_C0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T0_T0_C1_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041e8)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T0_T0_C1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[1].max_port_t[0].max_chan_t[0].gress_p_np_c[1].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=41e8, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T0_T0_C1_SB_PSF_HCB_REG 0x242241E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T0_T0_C1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T0_T0_C2_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041ec)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T0_T0_C2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[1].max_port_t[0].max_chan_t[0].gress_p_np_c[2].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=41ec, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T0_T0_C2_SB_PSF_HCB_REG 0x242241EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T0_T0_C2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T1_T0_C0_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041f0)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T1_T0_C0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[1].max_port_t[1].max_chan_t[0].gress_p_np_c[0].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=41f0, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T1_T0_C0_SB_PSF_HCB_REG 0x242241F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T1_T0_C0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T1_T0_C1_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041f4)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T1_T0_C1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[1].max_port_t[1].max_chan_t[0].gress_p_np_c[1].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=41f4, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T1_T0_C1_SB_PSF_HCB_REG 0x242241F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T1_T0_C1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T1_T0_C2_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041f8)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T1_T0_C2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[1].max_port_t[1].max_chan_t[0].gress_p_np_c[2].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=41f8, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T1_T0_C2_SB_PSF_HCB_REG 0x242241F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T1_T0_C2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T2_T0_C0_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x200041fc)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T2_T0_C0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[1].max_port_t[2].max_chan_t[0].gress_p_np_c[0].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=41fc, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T2_T0_C0_SB_PSF_HCB_REG 0x242241FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T2_T0_C0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T2_T0_C1_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x20004200)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T2_T0_C1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[1].max_port_t[2].max_chan_t[0].gress_p_np_c[1].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=4200, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T2_T0_C1_SB_PSF_HCB_REG 0x24224200

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T2_T0_C1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T3_T0_C0_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x20004204)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T3_T0_C0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[1].max_port_t[3].max_chan_t[0].gress_p_np_c[0].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=4204, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T3_T0_C0_SB_PSF_HCB_REG 0x24224204

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T3_T0_C0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T3_T0_C1_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x20004208)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T3_T0_C1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[1].max_port_t[3].max_chan_t[0].gress_p_np_c[1].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=4208, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T3_T0_C1_SB_PSF_HCB_REG 0x24224208

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T3_T0_C1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T4_T0_C0_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x2000420c)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T4_T0_C0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[1].max_port_t[4].max_chan_t[0].gress_p_np_c[0].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=420c, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T4_T0_C0_SB_PSF_HCB_REG 0x2422420C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T4_T0_C0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T4_T0_C1_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x20004210)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T4_T0_C1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[1].max_port_t[4].max_chan_t[0].gress_p_np_c[1].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=4210, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T4_T0_C1_SB_PSF_HCB_REG 0x24224210

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T4_T0_C1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T4_T0_C2_SB_PSF_HCB_REG supported on:    */
/*      SPRMCC (0x20004214)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T4_T0_C2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_cgcnt_gress[1].port_cgcnt_pg[1].max_port_t[4].max_chan_t[0].gress_p_np_c[2].port_cgcnt_gress_p_np_c.i_psf20_reg_port_cgcnt, size=6, reset=0x04, address=4214, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T4_T0_C2_SB_PSF_HCB_REG 0x24224214

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_GRESS1_PG1_T4_T0_C2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP0_PORT0_CHAN0_SB_PSF_HCB_REG supported on:   */
/*      SPRMCC (0x20004218)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_PG0_GRP0_PORT0_CHAN0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_tgcnt_pg[0].num_grp[0].max_port[0].max_chan[0].port_tgcnt_rld.i_psf20_reg_port_tgcnt_c_t, size=6, reset=0x01, address=4218, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP0_PORT0_CHAN0_SB_PSF_HCB_REG 0x24224218

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP0_PORT0_CHAN0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP0_PORT0_CHAN1_SB_PSF_HCB_REG supported on:   */
/*      SPRMCC (0x2000421c)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_PG0_GRP0_PORT0_CHAN1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_tgcnt_pg[0].num_grp[0].max_port[0].max_chan[1].port_tgcnt_rld.i_psf20_reg_port_tgcnt_c_t, size=6, reset=0x01, address=421c, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP0_PORT0_CHAN1_SB_PSF_HCB_REG 0x2422421C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP0_PORT0_CHAN1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP0_PORT0_CHAN2_SB_PSF_HCB_REG supported on:   */
/*      SPRMCC (0x20004220)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_PG0_GRP0_PORT0_CHAN2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_tgcnt_pg[0].num_grp[0].max_port[0].max_chan[2].port_tgcnt_rld.i_psf20_reg_port_tgcnt_c_t, size=6, reset=0x01, address=4220, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP0_PORT0_CHAN2_SB_PSF_HCB_REG 0x24224220

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP0_PORT0_CHAN2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP0_PORT0_CHAN3_SB_PSF_HCB_REG supported on:   */
/*      SPRMCC (0x20004224)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_PG0_GRP0_PORT0_CHAN3 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_tgcnt_pg[0].num_grp[0].max_port[0].max_chan[3].port_tgcnt_rld.i_psf20_reg_port_tgcnt_c_t, size=6, reset=0x01, address=4224, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP0_PORT0_CHAN3_SB_PSF_HCB_REG 0x24224224

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP0_PORT0_CHAN3_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT0_CHAN0_SB_PSF_HCB_REG supported on:   */
/*      SPRMCC (0x20004228)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT0_CHAN0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_tgcnt_pg[0].num_grp[1].max_port[0].max_chan[0].port_tgcnt_rld.i_psf20_reg_port_tgcnt_c_t, size=6, reset=0x04, address=4228, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT0_CHAN0_SB_PSF_HCB_REG 0x24224228

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT0_CHAN0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT1_CHAN0_SB_PSF_HCB_REG supported on:   */
/*      SPRMCC (0x2000422c)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT1_CHAN0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_tgcnt_pg[0].num_grp[1].max_port[1].max_chan[0].port_tgcnt_rld.i_psf20_reg_port_tgcnt_c_t, size=6, reset=0x04, address=422c, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT1_CHAN0_SB_PSF_HCB_REG 0x2422422C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT1_CHAN0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT2_CHAN0_SB_PSF_HCB_REG supported on:   */
/*      SPRMCC (0x20004230)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT2_CHAN0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_tgcnt_pg[0].num_grp[1].max_port[2].max_chan[0].port_tgcnt_rld.i_psf20_reg_port_tgcnt_c_t, size=6, reset=0x04, address=4230, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT2_CHAN0_SB_PSF_HCB_REG 0x24224230

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT2_CHAN0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT3_CHAN0_SB_PSF_HCB_REG supported on:   */
/*      SPRMCC (0x20004234)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT3_CHAN0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_tgcnt_pg[0].num_grp[1].max_port[3].max_chan[0].port_tgcnt_rld.i_psf20_reg_port_tgcnt_c_t, size=6, reset=0x04, address=4234, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT3_CHAN0_SB_PSF_HCB_REG 0x24224234

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT3_CHAN0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT4_CHAN0_SB_PSF_HCB_REG supported on:   */
/*      SPRMCC (0x20004238)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT4_CHAN0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_tgcnt_pg[0].num_grp[1].max_port[4].max_chan[0].port_tgcnt_rld.i_psf20_reg_port_tgcnt_c_t, size=6, reset=0x04, address=4238, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT4_CHAN0_SB_PSF_HCB_REG 0x24224238

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_PG0_GRP1_PORT4_CHAN0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN0_SB_PSF_HCB_REG supported on:   */
/*      SPRMCC (0x2000423c)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_tgcnt_pg[1].num_grp[0].max_port[0].max_chan[0].port_tgcnt_rld.i_psf20_reg_port_tgcnt_c_t, size=6, reset=0x01, address=423c, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN0_SB_PSF_HCB_REG 0x2422423C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN1_SB_PSF_HCB_REG supported on:   */
/*      SPRMCC (0x20004240)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_tgcnt_pg[1].num_grp[0].max_port[0].max_chan[1].port_tgcnt_rld.i_psf20_reg_port_tgcnt_c_t, size=6, reset=0x01, address=4240, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN1_SB_PSF_HCB_REG 0x24224240

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN2_SB_PSF_HCB_REG supported on:   */
/*      SPRMCC (0x20004244)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN2 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_tgcnt_pg[1].num_grp[0].max_port[0].max_chan[2].port_tgcnt_rld.i_psf20_reg_port_tgcnt_c_t, size=6, reset=0x01, address=4244, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN2_SB_PSF_HCB_REG 0x24224244

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN3_SB_PSF_HCB_REG supported on:   */
/*      SPRMCC (0x20004248)                                                     */
/* Register default value on SPRMCC: 0x00000001                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN3 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_tgcnt_pg[1].num_grp[0].max_port[0].max_chan[3].port_tgcnt_rld.i_psf20_reg_port_tgcnt_c_t, size=6, reset=0x01, address=4248, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN3_SB_PSF_HCB_REG 0x24224248

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000001*/

                            /*
                               Default value=0x1, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN3_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT0_CHAN0_SB_PSF_HCB_REG supported on:   */
/*      SPRMCC (0x2000424c)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT0_CHAN0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_tgcnt_pg[1].num_grp[1].max_port[0].max_chan[0].port_tgcnt_rld.i_psf20_reg_port_tgcnt_c_t, size=6, reset=0x04, address=424c, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT0_CHAN0_SB_PSF_HCB_REG 0x2422424C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT0_CHAN0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT1_CHAN0_SB_PSF_HCB_REG supported on:   */
/*      SPRMCC (0x20004250)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT1_CHAN0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_tgcnt_pg[1].num_grp[1].max_port[1].max_chan[0].port_tgcnt_rld.i_psf20_reg_port_tgcnt_c_t, size=6, reset=0x04, address=4250, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT1_CHAN0_SB_PSF_HCB_REG 0x24224250

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT1_CHAN0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT2_CHAN0_SB_PSF_HCB_REG supported on:   */
/*      SPRMCC (0x20004254)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT2_CHAN0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_tgcnt_pg[1].num_grp[1].max_port[2].max_chan[0].port_tgcnt_rld.i_psf20_reg_port_tgcnt_c_t, size=6, reset=0x04, address=4254, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT2_CHAN0_SB_PSF_HCB_REG 0x24224254

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT2_CHAN0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT3_CHAN0_SB_PSF_HCB_REG supported on:   */
/*      SPRMCC (0x20004258)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT3_CHAN0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_tgcnt_pg[1].num_grp[1].max_port[3].max_chan[0].port_tgcnt_rld.i_psf20_reg_port_tgcnt_c_t, size=6, reset=0x04, address=4258, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT3_CHAN0_SB_PSF_HCB_REG 0x24224258

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT3_CHAN0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT4_CHAN0_SB_PSF_HCB_REG supported on:   */
/*      SPRMCC (0x2000425c)                                                     */
/* Register default value on SPRMCC: 0x00000004                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Grant Count Reload values
        Details: InstName=psf_2, reg_name=psf_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT4_CHAN0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.port_tgcnt_pg[1].num_grp[1].max_port[4].max_chan[0].port_tgcnt_rld.i_psf20_reg_port_tgcnt_c_t, size=6, reset=0x04, address=425c, mask=0x3f
*/


#define PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT4_CHAN0_SB_PSF_HCB_REG 0x2422425C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntcntreload : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000004*/

                            /*
                               Default value=0x4, Value that is used to reload
                               an arbitration grant-counter for a transaction
                               or decode requester. The value 0 is reserved and
                               should not be used. These bits are not
                               synchronized, so they should only be written
                               when there are no IOSF Primary transactions
                               ongoing on PSF. This field width is variable,
                               and dependent on PSF configuration parameters.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_LINK_GNTCNT_RELOAD_PG1_GRP1_PORT4_CHAN0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGENT_MST_ROOTSPACE_PG1_PORT0_SB_PSF_HCB_REG supported on:             */
/*      SPRMCC (0x20004260)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Controls the Root Space ID of each root space
        Details: InstName=psf_2, reg_name=psf_2_AGENT_MST_ROOTSPACE_PG1_PORT0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.mrs_pg[1].port[0].gen.i_psf20_reg_mst_rs, size=2, reset=0x0, address=0x4260, mask=0x3
*/


#define PSF_2_AGENT_MST_ROOTSPACE_PG1_PORT0_SB_PSF_HCB_REG 0x24224260

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rs : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, This field indicates a unique
                               root space ID. This field width is variable, and
                               dependent on PSF configuration parameters. These
                               bits are not synchronized, so they should only
                               be written when there are no IOSF Primary
                               transactions ongoing on PSF.
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_AGENT_MST_ROOTSPACE_PG1_PORT0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGENT_MST_ROOTSPACE_PG1_PORT1_SB_PSF_HCB_REG supported on:             */
/*      SPRMCC (0x20004264)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Controls the Root Space ID of each root space
        Details: InstName=psf_2, reg_name=psf_2_AGENT_MST_ROOTSPACE_PG1_PORT1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.mrs_pg[1].port[1].gen.i_psf20_reg_mst_rs, size=2, reset=0x0, address=0x4264, mask=0x3
*/


#define PSF_2_AGENT_MST_ROOTSPACE_PG1_PORT1_SB_PSF_HCB_REG 0x24224264

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rs : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, This field indicates a unique
                               root space ID. This field width is variable, and
                               dependent on PSF configuration parameters. These
                               bits are not synchronized, so they should only
                               be written when there are no IOSF Primary
                               transactions ongoing on PSF.
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_AGENT_MST_ROOTSPACE_PG1_PORT1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGENT_TGT_ROOTSPACE_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x20004268)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Controls the Root Space ID of each root space
        Details: InstName=psf_2, reg_name=psf_2_AGENT_TGT_ROOTSPACE_hqm1_RS0_D9_F0_OFFSET67 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.t1agtrs[0].gen.i_psf20_reg_t1agtrs, size=2, reset=0x0, address=0x4268, mask=0x3
*/


#define PSF_2_AGENT_TGT_ROOTSPACE_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG 0x24224268

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rs : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, This field indicates a unique
                               root space ID. This field width is variable, and
                               dependent on PSF configuration parameters. These
                               bits are not synchronized, so they should only
                               be written when there are no IOSF Primary
                               transactions ongoing on PSF.
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_AGENT_TGT_ROOTSPACE_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGENT_TGT_ROOTSPACE_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x2000426c)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Controls the Root Space ID of each root space
        Details: InstName=psf_2, reg_name=psf_2_AGENT_TGT_ROOTSPACE_hqm0_RS0_D8_F0_OFFSET66 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.t1agtrs[1].gen.i_psf20_reg_t1agtrs, size=2, reset=0x0, address=0x426c, mask=0x3
*/


#define PSF_2_AGENT_TGT_ROOTSPACE_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG 0x2422426C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rs : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, This field indicates a unique
                               root space ID. This field width is variable, and
                               dependent on PSF configuration parameters. These
                               bits are not synchronized, so they should only
                               be written when there are no IOSF Primary
                               transactions ongoing on PSF.
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_AGENT_TGT_ROOTSPACE_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGENT_TGT_ROOTSPACE_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x20004270)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Controls the Root Space ID of each root space
        Details: InstName=psf_2, reg_name=psf_2_AGENT_TGT_ROOTSPACE_cpm1_RS0_D7_F0_OFFSET65 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.t1agtrs[2].gen.i_psf20_reg_t1agtrs, size=2, reset=0x0, address=0x4270, mask=0x3
*/


#define PSF_2_AGENT_TGT_ROOTSPACE_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG 0x24224270

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rs : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, This field indicates a unique
                               root space ID. This field width is variable, and
                               dependent on PSF configuration parameters. These
                               bits are not synchronized, so they should only
                               be written when there are no IOSF Primary
                               transactions ongoing on PSF.
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_AGENT_TGT_ROOTSPACE_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_AGENT_TGT_ROOTSPACE_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG supported on:*/
/*      SPRMCC (0x20004274)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Controls the Root Space ID of each root space
        Details: InstName=psf_2, reg_name=psf_2_AGENT_TGT_ROOTSPACE_cpm0_RS0_D6_F0_OFFSET64 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.t1agtrs[3].gen.i_psf20_reg_t1agtrs, size=2, reset=0x0, address=0x4274, mask=0x3
*/


#define PSF_2_AGENT_TGT_ROOTSPACE_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG 0x24224274

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rs : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, This field indicates a unique
                               root space ID. This field width is variable, and
                               dependent on PSF configuration parameters. These
                               bits are not synchronized, so they should only
                               be written when there are no IOSF Primary
                               transactions ongoing on PSF.
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_AGENT_TGT_ROOTSPACE_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CR_RANGE_POLICY_LO_SAI0_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x20004278)                                                     */
/* Register default value on SPRMCC: 0x0100021A                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register contains the lower 32-bits of the 64 bits identifying Security Attributes of the Initiator (SAI) that a have access to a privileged resource.
        Details: InstName=psf_2, reg_name=psf_2_CR_RANGE_POLICY_LO_SAI0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.gen.sb_sai[0].i_psf20_reg_sbsai_policy1, size=32, reset=0x0100021a, address=0x4278, mask=0xffffffff
*/


#define PSF_2_CR_RANGE_POLICY_LO_SAI0_SB_PSF_HCB_REG 0x24224278

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai : 32;

                            /* Bits[31:0], Access Type=RW, default=0x0100021A*/

                            /*
                               Default value=0x100021A, Multiple 6-bit SAIs can
                               be represented in a 64-bit vector inside a
                               policy register. . This register contains the
                               lower 32-bit of these 64-bits. If bit [n] is
                               set, SAIn has both read and write access to the
                               entity the policy register governs. If the bit
                               is not set, neither read nor write access is
                               allowed.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CR_RANGE_POLICY_LO_SAI0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CR_RANGE_POLICY_HI_SAI0_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x2000427c)                                                     */
/* Register default value on SPRMCC: 0x20000400                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register contains the higher 32-bits of the 64 bits identifying Security Attributes of the Initiator (SAI) that a have access to a privileged resource.
        Details: InstName=psf_2, reg_name=psf_2_CR_RANGE_POLICY_HI_SAI0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.gen.sb_sai[0].i_psf20_reg_sbsai_policy2, size=32, reset=0x20000400, address=0x427c, mask=0xffffffff
*/


#define PSF_2_CR_RANGE_POLICY_HI_SAI0_SB_PSF_HCB_REG 0x2422427C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai : 32;

                            /* Bits[31:0], Access Type=RW, default=0x20000400*/

                            /*
                               Default value=0x20000400, Multiple 6-bit SAIs
                               can be represented in a 64-bit vector inside a
                               policy register. . This register contains the
                               higher 32-bit of these 64-bits. If bit [n] is
                               set, SAIn has both read and write access to the
                               entity the policy register governs. If the bit
                               is not set, neither read nor write access is
                               allowed.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CR_RANGE_POLICY_HI_SAI0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_PROTECTED_CR_RANGE_SAI0_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x20004280)                                                     */
/* Register default value on SPRMCC: 0xFFFF0000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Defines a PSF CR range protected with SAI
        Details: InstName=psf_2, reg_name=psf_2_PROTECTED_CR_RANGE_SAI0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.gen.sb_sai[0].i_psf20_reg_sbsai_range, size=32, reset=0xffff0000, address=0x4280, mask=0xffffffff
*/


#define PSF_2_PROTECTED_CR_RANGE_SAI0_SB_PSF_HCB_REG 0x24224280

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addrbase : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* Default value=0x0, */
    UINT32 addrlimit : 16;

                            /* Bits[31:16], Access Type=RW, default=0x0000FFFF*/

                            /* Default value=0xFFFF, */

  } Bits;
  UINT32 Data;

} PSF_2_PROTECTED_CR_RANGE_SAI0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CR_RANGE_POLICY_LO_SAI1_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x20004284)                                                     */
/* Register default value on SPRMCC: 0x0100021E                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* This register contains the lower 32-bits of the 64 bits identifying Security Attributes of the Initiator (SAI) that a have access to a privileged resource.
        Details: InstName=psf_2, reg_name=psf_2_CR_RANGE_POLICY_LO_SAI1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.gen.sb_sai[1].i_psf20_reg_sbsai_policy1, size=32, reset=0x0100021e, address=0x4284, mask=0xffffffff
*/


#define PSF_2_CR_RANGE_POLICY_LO_SAI1_SB_PSF_HCB_REG 0x24224284

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai : 32;

                            /* Bits[31:0], Access Type=RW, default=0x0100021E*/

                            /*
                               Default value=0x100021E, Multiple 6-bit SAIs can
                               be represented in a 64-bit vector inside a
                               policy register. . This register contains the
                               lower 32-bit of these 64-bits. If bit [n] is
                               set, SAIn has both read and write access to the
                               entity the policy register governs. If the bit
                               is not set, neither read nor write access is
                               allowed.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CR_RANGE_POLICY_LO_SAI1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_CR_RANGE_POLICY_HI_SAI1_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x20004288)                                                     */
/* Register default value on SPRMCC: 0x20000400                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* This register contains the higher 32-bits of the 64 bits identifying Security Attributes of the Initiator (SAI) that a have access to a privileged resource.
        Details: InstName=psf_2, reg_name=psf_2_CR_RANGE_POLICY_HI_SAI1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.gen.sb_sai[1].i_psf20_reg_sbsai_policy2, size=32, reset=0x20000400, address=0x4288, mask=0xffffffff
*/


#define PSF_2_CR_RANGE_POLICY_HI_SAI1_SB_PSF_HCB_REG 0x24224288

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai : 32;

                            /* Bits[31:0], Access Type=RW, default=0x20000400*/

                            /*
                               Default value=0x20000400, Multiple 6-bit SAIs
                               can be represented in a 64-bit vector inside a
                               policy register. . This register contains the
                               higher 32-bit of these 64-bits. If bit [n] is
                               set, SAIn has both read and write access to the
                               entity the policy register governs. If the bit
                               is not set, neither read nor write access is
                               allowed.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_CR_RANGE_POLICY_HI_SAI1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_PROTECTED_CR_RANGE_SAI1_SB_PSF_HCB_REG supported on:                   */
/*      SPRMCC (0x2000428c)                                                     */
/* Register default value on SPRMCC: 0xFF00B000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Defines a PSF CR range protected with SAI
        Details: InstName=psf_2, reg_name=psf_2_PROTECTED_CR_RANGE_SAI1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.gen.sb_sai[1].i_psf20_reg_sbsai_range, size=32, reset=0xff00b000, address=0x428c, mask=0xffffffff
*/


#define PSF_2_PROTECTED_CR_RANGE_SAI1_SB_PSF_HCB_REG 0x2422428C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addrbase : 16;

                            /* Bits[15:0], Access Type=RW, default=0x0000B000*/

                            /* Default value=0xB000, */
    UINT32 addrlimit : 16;

                            /* Bits[31:16], Access Type=RW, default=0x0000FF00*/

                            /* Default value=0xFF00, */

  } Bits;
  UINT32 Data;

} PSF_2_PROTECTED_CR_RANGE_SAI1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_DFX_ACCUM_CMD_PG0_SB_PSF_HCB_REG supported on:                         */
/*      SPRMCC (0x20004290)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to read out an accum_o output value from the DfX Cmd Accumulator on port group 0, and to program control bits.
        Details: InstName=psf_2, reg_name=psf_2_DFX_ACCUM_CMD_PG0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.gen_dfx_accum.reg_dfx_accum_pg[0].i_psf20_reg_dfx_accum_cmd, size=32, reset=0x00000000, address=0x4290, mask=0x00060000
*/


#define PSF_2_DFX_ACCUM_CMD_PG0_SB_PSF_HCB_REG 0x24224290

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 accum_cmd : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, Output value from a DfX cmd
                               Accumulator for this port group
                            */
    UINT32 clr_cmd : 1;

                            /* Bits[16:16], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If written a 1, the cmd-
                               accumulator for this port group will be reset to
                               0. This bit always reads back a 0.
                            */
    UINT32 en_cmd : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set to 1, valid
                               transaction cmd phases will be accumulated from
                               any master port on this port group.
                            */
    UINT32 val_ovrd : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, f set to 1, cmd phases will
                               be accumulated every cycle, regardless of
                               whether there is a valid transaction.
                            */
    UINT32 rsvd : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_DFX_ACCUM_CMD_PG0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_DFX_ACCUM_DATA_PG0_SB_PSF_HCB_REG supported on:                        */
/*      SPRMCC (0x20004294)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to read out an accum_o output value from the DfX Data Accumulator on port group 0, and to program control bits.
        Details: InstName=psf_2, reg_name=psf_2_DFX_ACCUM_DATA_PG0 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.gen_dfx_accum.reg_dfx_accum_pg[0].i_psf20_reg_dfx_accum_data, size=32, reset=0x00000000, address=0x4294, mask=0x00060000
*/


#define PSF_2_DFX_ACCUM_DATA_PG0_SB_PSF_HCB_REG 0x24224294

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 accum_data : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, Output value from a DfX data
                               Accumulator for this port group
                            */
    UINT32 clr_data : 1;

                            /* Bits[16:16], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If written a 1, the data-
                               accumulator for this port group will be reset to
                               0. This bit always reads back a 0.
                            */
    UINT32 en_data : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set to 1, valid
                               transaction data phases will be accumulated from
                               any master port on this port group.
                            */
    UINT32 val_ovrd : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set to 1, data phases will
                               be accumulated every cycle, regardless of
                               whether there is a valid transaction.
                            */
    UINT32 rsvd : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_DFX_ACCUM_DATA_PG0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_DFX_ACCUM_CMD_PG1_SB_PSF_HCB_REG supported on:                         */
/*      SPRMCC (0x20004298)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to read out an accum_o output value from the DfX Cmd Accumulator on port group 0, and to program control bits.
        Details: InstName=psf_2, reg_name=psf_2_DFX_ACCUM_CMD_PG1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.gen_dfx_accum.reg_dfx_accum_pg[1].i_psf20_reg_dfx_accum_cmd, size=32, reset=0x00000000, address=0x4298, mask=0x00060000
*/


#define PSF_2_DFX_ACCUM_CMD_PG1_SB_PSF_HCB_REG 0x24224298

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 accum_cmd : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, Output value from a DfX cmd
                               Accumulator for this port group
                            */
    UINT32 clr_cmd : 1;

                            /* Bits[16:16], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If written a 1, the cmd-
                               accumulator for this port group will be reset to
                               0. This bit always reads back a 0.
                            */
    UINT32 en_cmd : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set to 1, valid
                               transaction cmd phases will be accumulated from
                               any master port on this port group.
                            */
    UINT32 val_ovrd : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, f set to 1, cmd phases will
                               be accumulated every cycle, regardless of
                               whether there is a valid transaction.
                            */
    UINT32 rsvd : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_DFX_ACCUM_CMD_PG1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_DFX_ACCUM_DATA_PG1_SB_PSF_HCB_REG supported on:                        */
/*      SPRMCC (0x2000429c)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to read out an accum_o output value from the DfX Data Accumulator on port group 0, and to program control bits.
        Details: InstName=psf_2, reg_name=psf_2_DFX_ACCUM_DATA_PG1 : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.gen_dfx_accum.reg_dfx_accum_pg[1].i_psf20_reg_dfx_accum_data, size=32, reset=0x00000000, address=0x429c, mask=0x00060000
*/


#define PSF_2_DFX_ACCUM_DATA_PG1_SB_PSF_HCB_REG 0x2422429C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 accum_data : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /*
                               Default value=0x0, Output value from a DfX data
                               Accumulator for this port group
                            */
    UINT32 clr_data : 1;

                            /* Bits[16:16], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If written a 1, the data-
                               accumulator for this port group will be reset to
                               0. This bit always reads back a 0.
                            */
    UINT32 en_data : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set to 1, valid
                               transaction data phases will be accumulated from
                               any master port on this port group.
                            */
    UINT32 val_ovrd : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set to 1, data phases will
                               be accumulated every cycle, regardless of
                               whether there is a valid transaction.
                            */
    UINT32 rsvd : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_DFX_ACCUM_DATA_PG1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EINJ_SB_PSF_HCB_REG supported on:                                      */
/*      SPRMCC (0x200042a0)                                                     */
/* Register default value on SPRMCC: 0x0000000A                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* This register can be used to program how to start and stop the EINJ function
        Details: InstName=psf_2, reg_name=psf_2_EINJ : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.gen_errinj.i_psf20_reg_errinj, size=32, reset=0x0000000a, address=0x42a0, mask=0x000000ff
*/


#define PSF_2_EINJ_SB_PSF_HCB_REG 0x242242A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 start : 4;

                            /* Bits[3:0], Access Type=RW, default=0x0000000A*/

                            /*
                               Default value=0xA, Selects the source which
                               causes EINJ activation. 0000: fth_trig_in
                               [0][br] 0001: fth_trig_in [1][br] 0010:
                               fth_trig_in [2][br] 0011: fth_trig_in [3][br]
                               0100: Reserved [br] 0101: Reserved [br] 0110:
                               Reserved [br] 0111: Reserved [br] 1000: Reserved
                               [br] 1001: Reserved [br] 1010: EINJ is never
                               activated [br] 1011: EINJ is always activated
                               [br] 1100: Reserved [br] 1101: Reserved [br]
                               1110: Reserved [br] 1111: Reserved
                            */
    UINT32 stop : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, Selects the source which
                               causes EINJ deactivation. 0000: fth_trig_in
                               [0][br] 0001: fth_trig_in [1][br] 0010:
                               fth_trig_in [2][br] 0011: fth_trig_in [3][br]
                               0100: Reserved [br] 0101: Reserved [br] 0110:
                               Reserved [br] 0111: Reserved [br] 1000: Reserved
                               [br] 1001: Reserved [br] 1010: Deactivate one
                               clock after activation [br] 1011: EINJ is never
                               deactivated [br] 1100: Deactivate EINJ when the
                               activating signal becomes false.[br] 1101:
                               Reserved [br] 1110: Reserved [br] 1111: Reserved
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EINJ_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EINJ_CONTROL_SB_PSF_HCB_REG supported on:                              */
/*      SPRMCC (0x200042a4)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI0                                  */
/* Error Injection Control Register
        Details: InstName=psf_2, reg_name=psf_2_EINJ_CONTROL : reg=psf20_wrpr_top_tb.psf20_wrpr_top.psf2.i_psf20_top_psf2.i_psf20_top_pgd.i_psf20_top_core.i_psf20_config_reg.gen_errinj.i_psf20_reg_errinj_control, size=32, reset=0x00000000, address=0x42a4, mask=0xc0000101
*/


#define PSF_2_EINJ_CONTROL_SB_PSF_HCB_REG 0x242242A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dpf : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set to 1, the data parity
                               bit of each IOSF data phase will get flipped
                               when parity errors are injected.
                            */
    UINT32 rsvd : 7;

                            /* Bits[7:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cpf : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, If set to 1, the command
                               parity bit of each IOSF command phase will get
                               flipped when parity errors are injected.
                            */
    UINT32 rsvd_9 : 20;

                            /* Bits[28:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 errinjsts : 1;

                            /* Bits[29:29], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, This field reflects when the
                               error has been injected. 0: Error has not been
                               injected 1: Error has been injected
                            */
    UINT32 errinjpg : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, Indicates the port group on
                               which errors will be injected. Errors will be
                               injected on all egress ports in the specified
                               port group. 1'b1
                            */
    UINT32 errinjen : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Default value=0x0, When this bit is a 0, error
                               injection is disabled, which is the default.
                               When this bit is a 1, error injection is
                               enabled.
                            */

  } Bits;
  UINT32 Data;

} PSF_2_EINJ_CONTROL_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS0_PG0_P0_CH0_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b000)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS0_PG0_P0_CH0_SB_PSF_HCB_REG 0x2422B000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS0_PG0_P0_CH0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS0_PG0_P0_CH1_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b004)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS0_PG0_P0_CH1_SB_PSF_HCB_REG 0x2422B004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS0_PG0_P0_CH1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS0_PG0_P0_CH2_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b008)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS0_PG0_P0_CH2_SB_PSF_HCB_REG 0x2422B008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS0_PG0_P0_CH2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS0_PG0_P0_CH3_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b00c)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS0_PG0_P0_CH3_SB_PSF_HCB_REG 0x2422B00C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS0_PG0_P0_CH3_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS0_PG1_P0_CH0_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b200)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS0_PG1_P0_CH0_SB_PSF_HCB_REG 0x2422B200

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS0_PG1_P0_CH0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS0_PG1_P1_CH0_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b220)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS0_PG1_P1_CH0_SB_PSF_HCB_REG 0x2422B220

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS0_PG1_P1_CH0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS0_PG1_P2_CH0_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b240)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS0_PG1_P2_CH0_SB_PSF_HCB_REG 0x2422B240

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS0_PG1_P2_CH0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS0_PG1_P3_CH0_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b260)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS0_PG1_P3_CH0_SB_PSF_HCB_REG 0x2422B260

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS0_PG1_P3_CH0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS0_PG1_P4_CH0_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b280)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS0_PG1_P4_CH0_SB_PSF_HCB_REG 0x2422B280

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS0_PG1_P4_CH0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS0_PG1_P5_CH0_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b2a0)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS0_PG1_P5_CH0_SB_PSF_HCB_REG 0x2422B2A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS0_PG1_P5_CH0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS3_PG0_P0_CH0_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b400)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS3_PG0_P0_CH0_SB_PSF_HCB_REG 0x2422B400

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS3_PG0_P0_CH0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS3_PG0_P0_CH1_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b404)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS3_PG0_P0_CH1_SB_PSF_HCB_REG 0x2422B404

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS3_PG0_P0_CH1_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS3_PG0_P0_CH2_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b408)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS3_PG0_P0_CH2_SB_PSF_HCB_REG 0x2422B408

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS3_PG0_P0_CH2_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS3_PG0_P0_CH3_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b40c)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS3_PG0_P0_CH3_SB_PSF_HCB_REG 0x2422B40C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS3_PG0_P0_CH3_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS3_PG1_P0_CH0_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b600)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS3_PG1_P0_CH0_SB_PSF_HCB_REG 0x2422B600

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS3_PG1_P0_CH0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS3_PG1_P1_CH0_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b620)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS3_PG1_P1_CH0_SB_PSF_HCB_REG 0x2422B620

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS3_PG1_P1_CH0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS3_PG1_P2_CH0_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b640)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS3_PG1_P2_CH0_SB_PSF_HCB_REG 0x2422B640

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS3_PG1_P2_CH0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS3_PG1_P3_CH0_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b660)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS3_PG1_P3_CH0_SB_PSF_HCB_REG 0x2422B660

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS3_PG1_P3_CH0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS3_PG1_P4_CH0_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b680)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS3_PG1_P4_CH0_SB_PSF_HCB_REG 0x2422B680

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS3_PG1_P4_CH0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */

#ifdef SPRMCC_HOST
/* PSF_2_EHCR_RS3_PG1_P5_CH0_SB_PSF_HCB_REG supported on:                       */
/*      SPRMCC (0x2000b6a0)                                                     */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRMCC Register File:    sprsp_top/hcx_psf/psf_2_spr                         */
/* Struct generated from SPRMCC BDF: 0_0_0                                      */
/* SPRMCC Security PolicyGroup: HCX_PSF_2_SAI1                                  */
/* These registers control logging of errors for transactions that have encountered an error.
*/


#define PSF_2_EHCR_RS3_PG1_P5_CH0_SB_PSF_HCB_REG 0x2422B6A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perr : 1;

                            /* Bits[0:0], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received and dropped an erroneous P transaction.
                               Cleared by writing a 1'b. This bit is sticky.
                            */
    UINT32 nperr : 1;

                            /* Bits[1:1], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received an erroneous NP transaction, and has
                               returned a UR CPL. Cleared by writing a 1'b1.
                               This bit is sticky.
                            */
    UINT32 cplerr : 1;

                            /* Bits[2:2], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Default value=0x0, If set, the Error Handler has
                               received ad dropped an erroneous CPL
                               transaction. Cleared by writing a 1'b1. This bit
                               is sticky.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSF_2_EHCR_RS3_PG1_P5_CH0_SB_PSF_HCB_STRUCT;
#endif /* (SPRMCC_HOST) */
#endif /* _SB_PSF_HCB_h */
