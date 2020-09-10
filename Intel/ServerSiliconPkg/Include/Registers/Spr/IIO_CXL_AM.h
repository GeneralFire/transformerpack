
/** @file
  IIO_CXL_AM.h

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
/* SIPAM_G0_RLINK_AM                                                            */
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
/*    HOSTIA_BOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI |                 */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_INTEL_MANUFACTURING_SAI | PM_PCS_SAI |     */
/*    HW_CPU_SAI | HW_PCH_SAI | OOB_MSM_SAI                                     */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRB0 Security Policy Groups:                                                */

/* SPRHBM Security Policy Groups:                                               */

/* SPRC0 Security Policy Groups:                                                */

/* SPRMCC Security Policy Groups:                                               */

/* SPRUCC Security Policy Groups:                                               */


#ifndef _IIO_CXL_AM_h
#define _IIO_CXL_AM_h
#include <Base.h>

#ifdef SPRA0_HOST
/* ARBMUXCTL0_IIO_CXL_AM_REG supported on:                                      */
/*      SPRA0 (0x2000e140)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/SPR_am_0[0]/am_core_upc/am_core_mem        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SIPAM_G0_RLINK_AM                                */
/* ARB/MUX Control0
*/


#define ARBMUXCTL0_IIO_CXL_AM_REG 0x2332E140

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hard : 1;

                            /* Bits[0:0], Access Type=RW/V/L, default=None*/

                            /*
                               HW Autonomous RECENTER Disable (HARD): [br]When
                               this bit is set, the ARB/MUX will not
                               autonomously trigger RECENTER upon receiving an
                               error - instead it communicates the error to the
                               Link Layer and waits for the Link Layer to
                               trigger RECENTER through the LPIF+ STATE_REQ
                               protocol. [br]The default value of this field is
                               set by strap. [br]This bit is Read-Only if
                               ARBMUXCTL.SRL is set.
                            */
    UINT32 abe : 1;

                            /* Bits[1:1], Access Type=RW, default=None*/

                            /*
                               ARB/MUX Bypass Enable (ABE): [br]When set, the
                               arbitration logic and PLP generation logic are
                               disabled. This enables interoperability with a
                               companion die that does not have an ARB/MUX.
                               Only a single LPIF+ interface can be configured
                               for operation when this bit is set.
                            */
    UINT32 lctge : 1;

                            /* Bits[2:2], Access Type=RW, default=None*/

                            /*
                               Link Clock Trunk Gate Enable (LCTGE): [br]When
                               set, allows the link clock trunk to be gated
                               when the idle conditions are met. When clear,
                               the link clock is never trunk gated.
                            */
    UINT32 dlcge : 1;

                            /* Bits[3:3], Access Type=RW, default=None*/

                            /*
                               Dynamic Link Clock Gate Enable (DLCGE): [br]When
                               set, the link clock may be gated dynamically
                               when idle conditions are met. When clear, the
                               link clock is never dynamically gated.
                            */
    UINT32 l12entryhys : 3;

                            /* Bits[6:4], Access Type=RW, default=None*/

                            /*
                               L1/L2 Entry Hysteresis (L12ENTRYHYS): This
                               specifies the minimum number of clocks (lclk)
                               the ARB-MUX Consolidator will wait from prior
                               L1/L2 exit before issuing the next earliest
                               L1/L2 entry request on LPIF[0]. The following
                               defines the encodings used:- [br]'000' - 0 clock
                               [br]'001' - 32 clocks [br]'010' - 64 clocks
                               [br]'011' - 128 clocks [br]'100' - 256 clocks
                               [br]Others Reserved [br]Note: If this field is
                               required to be programmed, it should be done
                               before initiating any L1/L2 STATE_REQ from the
                               Link Layer(s) [br]Note: This field is only
                               applicable when ARB-MUX Consolidator
                               functionality is not bypassed
                            */
    UINT32 sdcge : 1;

                            /* Bits[7:7], Access Type=RW, default=None*/

                            /*
                               Side Clock Dynamic Clock Gating Enable (SDCGE):
                               [br]When set, the Side clock used within the
                               Sideband Endpoint will be dynamically gated when
                               the condition permits. When cleared, the dynamic
                               Side clock gating mechanism is disabled.
                               [br]Register Attribute: Static.
                            */
    UINT32 stcge : 1;

                            /* Bits[8:8], Access Type=RW, default=None*/

                            /*
                               Side Clock Trunk Clock Gating Enable (STCGE):
                               [br]When set, the Side Clock will be clock gated
                               at the trunk level when the conditions to clock
                               gate are met. When clear, the Side Clock will
                               never be clock gated at the trunk level.
                               [br]Register Attribute: Static.
                            */
    UINT32 rsvd_m : 21;

                            /* Bits[29:9], Access Type=RO, default=None*/

                            /* Reserved (RSVD). */
    UINT32 ammode : 1;

                            /* Bits[30:30], Access Type=RW/L, default=None*/

                            /*
                               ARB-MUX Mode (AMMODE): Indicates either
                               Upstream/Downstream Component operation [br]0 -
                               Downstream Component [br]1 - Upstream Component
                               [br]Note: If required to be changed from its
                               default value, this bit is expected to be
                               programmed prior to LPIF's reset is de-asserted.
                               [br]This bit is Read-Only if ARBMUXCTL0.SRL is
                               set. [br]Register Attribute: Static.
                            */
    UINT32 srl : 1;

                            /* Bits[31:31], Access Type=RW/L, default=None*/

                            /*
                               Secure Register Lock (SRL): [br]When set, locks
                               the following register bits: [br]-
                               ARBMUXCTL0.HARD [br]- ARBMUXCTL.SRL [br]-
                               ARBMUXSRV.* [br]- LPIF#CTL.EN (Excluding LPIF#0)
                               [br]Once this bit is written to '1', it cannot
                               be changed until a side_rst_b assertion occurs.
                               [br]This bit is Read-Only if set.
                            */

  } Bits;
  UINT32 Data;

} ARBMUXCTL0_IIO_CXL_AM_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* ARBMUXSTS0_IIO_CXL_AM_REG supported on:                                      */
/*      SPRA0 (0x2000e144)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/SPR_am_0[0]/am_core_upc/am_core_mem        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SIPAM_G0_RLINK_AM                                */
/* ARB/MUX Status0
*/


#define ARBMUXSTS0_IIO_CXL_AM_REG 0x2332E144

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_1 : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved (RSVD). */
    UINT32 arbmuxerr : 15;

                            /* Bits[15:1], Access Type=RW/1C/V/P, default=None*/

                            /*
                               ARB/MUX Detected Error (ARBMUXERR): [br]These
                               bits are set when the ARB/MUX error, e.g.
                               invalid PLP, is detected. These bits are cleared
                               by software by writing a '1'. [br]Bit [1] -
                               Error Detected on LPIF+ Instance #1 [br]Bit [2]
                               - Error Detected on LPIF+ Instance #2 [br]. . .
                               [br]Bit [13] - Error Detected on LPIF+ Instance
                               #13 [br]Bit [14] - Error Detected on LPIF+
                               Instance #14 [br]Bit [15] - Error Detected on
                               unsupported LPIF+ instance.
                            */
    UINT32 rsvd_m : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved (RSVD). */

  } Bits;
  UINT32 Data;

} ARBMUXSTS0_IIO_CXL_AM_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* ARBMUXCPL_IIO_CXL_AM_REG supported on:                                       */
/*      SPRA0 (0x2000e148)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/SPR_am_0[0]/am_core_upc/am_core_mem        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SIPAM_G0_RLINK_AM                                */
/* ARB-MUX Control Policy 0
*/

#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* ARBMUXCPH_IIO_CXL_AM_REG supported on:                                       */
/*      SPRA0 (0x2000e14c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/SPR_am_0[0]/am_core_upc/am_core_mem        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SIPAM_G0_RLINK_AM                                */
/* ARB-MUX Control Policy 1
*/

#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* ARBMUXRACPL_IIO_CXL_AM_REG supported on:                                     */
/*      SPRA0 (0x2000e150)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/SPR_am_0[0]/am_core_upc/am_core_mem        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SIPAM_G0_RLINK_AM                                */
/* ARB-MUX Read Access Control 0
*/

#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* ARBMUXRACPH_IIO_CXL_AM_REG supported on:                                     */
/*      SPRA0 (0x2000e154)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/SPR_am_0[0]/am_core_upc/am_core_mem        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SIPAM_G0_RLINK_AM                                */
/* ARB-MUX Read Access Control 1
*/

#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* ARBMUXWACPL_IIO_CXL_AM_REG supported on:                                     */
/*      SPRA0 (0x2000e158)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/SPR_am_0[0]/am_core_upc/am_core_mem        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SIPAM_G0_RLINK_AM                                */
/* ARB-MUX Write Access Control 0
*/

#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* ARBMUXWACPH_IIO_CXL_AM_REG supported on:                                     */
/*      SPRA0 (0x2000e15c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/SPR_am_0[0]/am_core_upc/am_core_mem        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SIPAM_G0_RLINK_AM                                */
/* ARB-MUX Write Access Control 1
*/

#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* ARBMUXSRV_IIO_CXL_AM_REG supported on:                                       */
/*      SPRA0 (0x2000e160)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/SPR_am_0[0]/am_core_upc/am_core_mem        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SIPAM_G0_RLINK_AM                                */
/* ARB-MUX Survivability
*/


#define ARBMUXSRV_IIO_CXL_AM_REG 0x2332E160

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 l1crd : 1;

                            /* Bits[0:0], Access Type=RW/L, default=None*/

                            /*
                               L1 Consolidator Resolution Disable (L1CRD):
                               [br]When this bit is set, the ARB-MUX will not
                               resolve to request for STATE_REQ==L1 on its LPIF
                               interface with the Logical PHY. [br]Note: If
                               this field is required to be programmed, it
                               should be done before initiating any L1/L2
                               STATE_REQ from the Link Layer(s)
                            */
    UINT32 l2crd : 1;

                            /* Bits[1:1], Access Type=RW/L, default=None*/

                            /*
                               L2 Consolidator Resolution Disable (L2CRD):
                               [br]When this bit is set, the ARB-MUX will not
                               resolve to request for STATE_REQ==L2 on its LPIF
                               interface with the Logical PHY. [br]Note: If
                               this field is required to be programmed, it
                               should be done before initiating any L1/L2
                               STATE_REQ from the Link Layer(s)
                            */
    UINT32 dcrd : 1;

                            /* Bits[2:2], Access Type=RW/L, default=None*/

                            /*
                               DISABLE Consolidator Resolution Disable (DCRD):
                               [br]When this bit is set, the ARB-MUX will not
                               resolve to request for STATE_REQ==DISABLE on its
                               LPIF interface with the Logical PHY. [br]Note:
                               If this field is required to be programmed, it
                               should be done before initiating any DISABLE
                               STATE_REQ from the Link Layer(s)
                            */
    UINT32 lrcrd : 1;

                            /* Bits[3:3], Access Type=RW/L, default=None*/

                            /*
                               LINKRESET Consolidator Resolution Disable
                               (LRCRD): [br]When this bit is set, the ARB-MUX
                               will not resolve to request for
                               STATE_REQ==LINKRESET on its LPIF interface with
                               the Logical PHY. [br]Note: If this field is
                               required to be programmed, it should be done
                               before initiating any LINKRESET STATE_REQ from
                               the Link Layer(s) [br]Note: This field is not
                               applicable when LPIF LINKRESET Virtualization is
                               disabled
                            */
    UINT32 flsfixdis : 1;

                            /* Bits[4:4], Access Type=RW/L, default=None*/

                            /*
                               FW LINKINIT/SPEED Fix Disable (FLSFIXDIS): When
                               this bit is set, the ARB-MUX fix for handling
                               Physical LSM transitioning when LPIF STATE_STS
                               remains at ACTIVE state (i.e. HSD Issue
                               #1504209701) will be disabled.
                            */
    UINT32 lrcrh : 1;

                            /* Bits[5:5], Access Type=RW/L, default=None*/

                            /*
                               LINKRESET Consolidator Resolution Hold (LRCRH):
                               [br]When this bit is set, once the ARB-MUX
                               Consolidators STATE_REQ==LINKRESET, it will be
                               held there until this bit is cleared or until
                               side_rst_b assertion. [br]Note: If this field is
                               required to be programmed, it should be done
                               before initiating any LINKRESET STATE_REQ from
                               the Link Layer(s)
                            */
    UINT32 rsvd_rw : 26;

                            /* Bits[31:6], Access Type=RW/L, default=None*/

                            /* Reserved (RSVD) */

  } Bits;
  UINT32 Data;

} ARBMUXSRV_IIO_CXL_AM_STRUCT;
#endif /* (SPRA0_HOST) */
#endif /* _IIO_CXL_AM_h */
