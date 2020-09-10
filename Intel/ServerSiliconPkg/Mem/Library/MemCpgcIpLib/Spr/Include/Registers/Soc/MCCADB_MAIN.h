
/** @file
  MCCADB_MAIN.h

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
/* CADB_PROTECTED                                                               */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRB0 Security Policy Groups:                                                */
/* CADB_PROTECTED                                                               */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRHBM Security Policy Groups:                                               */
/* CADB_PROTECTED                                                               */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRC0 Security Policy Groups:                                                */
/* CADB_PROTECTED                                                               */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRMCC Security Policy Groups:                                               */
/* CADB_PROTECTED                                                               */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRUCC Security Policy Groups:                                               */
/* CADB_PROTECTED                                                               */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */


#ifndef _MCCADB_MAIN_h
#define _MCCADB_MAIN_h
#include <Base.h>







/* CADB_CTL_MCCADB_MAIN_REG supported on:                                       */
/*      SPRA0 (0x20007018)                                                      */
/*      SPRB0 (0x20007018)                                                      */
/*      SPRHBM (0x20007018)                                                     */
/*      SPRC0 (0x20007018)                                                      */
/*      SPRMCC (0x20007018)                                                     */
/*      SPRUCC (0x20007018)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer Control Register.
*/


#define CADB_CTL_MCCADB_MAIN_REG 0x27027018

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 start_test : 1;

                            /* Bits[0:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Writing a 1 to this field will issue a start
                               request to the CADB engine. After 1-2 SPID
                               clocks, the CADB will transition into the mode
                               specified by the CADB_MODE field. This field
                               self clears once the CADB has started.
                            */
    UINT32 stop_test : 1;

                            /* Bits[1:1], Access Type=RW/V, default=0x00000000*/

                            /*
                               Writing a 1 to this field will issue a stop
                               request to the CADB engine. The CADB will stop
                               driving all traffic at the next command boundary
                               (i.e. an in-flight MRS or select override will
                               complete before the hardware stops). This field
                               self-clears once the CADB has returned to an
                               idle state.
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_CTL_MCCADB_MAIN_STRUCT;

/* CADB_CFG_MCCADB_MAIN_REG supported on:                                       */
/*      SPRA0 (0x2000701c)                                                      */
/*      SPRB0 (0x2000701c)                                                      */
/*      SPRHBM (0x2000701c)                                                     */
/*      SPRC0 (0x2000701c)                                                      */
/*      SPRMCC (0x2000701c)                                                     */
/*      SPRUCC (0x2000701c)                                                     */
/* Register default value on SPRA0: 0x00001800                                  */
/* Register default value on SPRB0: 0x00001800                                  */
/* Register default value on SPRHBM: 0x00001800                                 */
/* Register default value on SPRC0: 0x00001800                                  */
/* Register default value on SPRMCC: 0x00001800                                 */
/* Register default value on SPRUCC: 0x00001800                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer Configuration Register.
*/


#define CADB_CFG_MCCADB_MAIN_REG 0x2702701C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_deselect_start : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               Specifies which commands can start the CADB to
                               drive deselect cycles. If deselects were on
                               before the command that is supposed to start it,
                               they will remain on. If a command type is
                               enabled in both this field and the
                               CMD_DESELECT_STOP field, that command type will
                               cause the deselect state (on/off) to toggle.[br]
                               Setting Bit 3 - Start driving deselects on the
                               following DRAM command clock after a PRE
                               command.[br] Setting Bit 2 - Start driving
                               deselects on the following DRAM command clock
                               after a ACT command.[br] Setting Bit 1 - Start
                               driving deselects on the following DRAM command
                               clock after a RD command.[br] Setting Bit 0 -
                               Start driving deselects on the following DRAM
                               command clock after a WR command.
                            */
    UINT32 cmd_deselect_stop : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /*
                               Specifies which commands can stop the CADB from
                               driving deselect cycles. If deselects were off
                               before the command that is supposed to start it,
                               they will remain off. If a command type is
                               enabled in both this field and the
                               CMD_DESELECT_START field, that command type will
                               cause the deselect state (on/off) to toggle.[br]
                               Setting Bit 7 - Stop driving deselects on the
                               following DRAM command clock after a PRE
                               command.[br] Setting Bit 6 - Stop driving
                               deselects on the following DRAM command clock
                               after a ACT command.[br] Setting Bit 5 - Stop
                               driving deselects on the following DRAM command
                               clock after a RD command.[br] Setting Bit 4 -
                               Stop driving deselects on the following DRAM
                               command clock after a WR command.
                            */
    UINT32 lane_deselect_en : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Specifies which lanes are driven during deselect
                               cycles.[br] Setting Bit 10 - Drive CADB pattern
                               on CA pins.[br] Setting Bit 9 - Drive CADB
                               pattern on Parity pin.[br] Setting Bit 8 - Drive
                               CADB pattern on CS pin.
                            */
    UINT32 initial_dsel_en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Specifies whether deselects are on (1) or off
                               (0) at the start of CADB operation. Note that in
                               global deselect operation modes, this initial
                               state will persist throughout the duration of
                               the test as command-based deselect triggers are
                               ignored.
                            */
    UINT32 initial_dsel_sseq_en : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Specifies the initial deselect subsequence
                               enable information that the CADB will use before
                               subsequence information is received from the
                               CPGC. Note that in normal operation modes, this
                               initial state will persist throughout the
                               duration of the test as subsequence enable
                               information passed by the memory controller is
                               ignored.
                            */
    UINT32 cadb_dsel_throttle_mode : 3;

                            /* Bits[15:13], Access Type=RW, default=0x00000000*/

                            /*
                               The Deselect/Always On throttling mode of the
                               CA/Parity/CS pattern generator is encoded as
                               follows:[br] 00, 01 - 1N Mode[br] 10 - 2N
                               Mode[br] 11 - 3N Mode
                            */
    UINT32 cadb_sel_throttle_mode : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               The Select throttling mode of the CA/Parity/CS
                               pattern generator is encoded as follows:[br] 00,
                               01 - 1N Mode[br] 10 - 2N Mode[br] 11 - 3N
                               Mode[br] This setting is only used when using a
                               folded CA bus for timing of subsequent phases of
                               the same select cycle.
                            */
    UINT32 rsvd : 5;

                            /* Bits[23:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cadb_to_cpgc_bind : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit will bind this CADB engine to
                               its associated CPGC request engine. A test start
                               for the currently assigned CPGC request will
                               case this CADB engine to also start as if
                               CADB_CTL.START_TEST was written to a 1. A test
                               stop (both forced and due to a stop condition)
                               for the currently assigned CPGC request engine
                               will cause the this CADB engine to also stop as
                               if CADB_CTL.STOP_TEST was written to a 1. This
                               feature is usually used when synchronization
                               between CADB deselect activity and CPGC
                               transactions are desired. Note: this field is
                               only available if multiple channels/engines
                               synchronization is privisioned for the current
                               implementation, otherwise it is reserved.
                            */
    UINT32 global_start_bind : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit will bind this CADB engine to
                               all other globally enabled CPGC and CADB
                               engines. A test start for any CPGC or CADB
                               engine with GLOBAL_START_BIND set will cause the
                               same action to occur on all engines with
                               GLOBAL_START_BIND set. This feature is usually
                               used when synchronization between multiple
                               engines/channels necessitates a global control
                               of all supported engines. Note: this field is
                               only available if multiple channels/engines
                               synchronization is privisioned for the current
                               implementation, otherwise it is reserved.
                            */
    UINT32 global_stop_bind : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit will bind this CADB engine to
                               all other globally enabled CPGC and CADB
                               engines. A test stop (both forced and due to a
                               stop condition) for any CPGC or CADB engine with
                               GLOBAL_STOP_BIND set will cause the same action
                               to occur on all engines with GLOBAL_STOP_BIND
                               set. This feature is usually used when
                               synchronization between multiple
                               engines/channels necessitates a global control
                               of all supported engines. Note: this field is
                               only available if multiple channels/engine
                               synchronization is provisioned for the current
                               implementation, otherwise it is reserved.
                            */
    UINT32 rsvd_27 : 2;

                            /* Bits[28:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cadb_mode : 3;

                            /* Bits[31:29], Access Type=RW, default=0x00000000*/

                            /*
                               The operation mode of the command/address
                               pattern generator is encoded as follows:[br] 011
                               - Normal mode Select and trigger-driven-Deselect
                               : CADB will drive patterns during Select and
                               Deselect cycles. This mode allows start/stop of
                               Deselect cycles to synchronize with various CMD
                               events. This mode ignores SubSequence Enables
                               from CPGC.[br] 111 - Active mode Select and
                               trigger-driven-Deselect : CADB will drive
                               patterns during Select and Deselect cycles. This
                               mode allows start/stop of Deselect cycles to
                               synchronize with various CMD events. This mode
                               respects SubSequence Enables from CPGC.[br] 010
                               - Normal mode Global Deselect : CADB will drive
                               patterns during Select cycles and all Deselect
                               cycles. This mode ignores the programming of
                               CMD_DESELECT_START / CMD_DESELECT_STOP. This
                               mode ignores SubSequence Enables from CPGC.[br]
                               110 - Active mode Global Deselect : CADB will
                               drive patterns during Select cycles and all
                               Deselect cycles. This mode ignores the
                               programming of CMD_DESELECT_START /
                               CMD_DESELECT_STOP. This mode respects
                               SubSequence Enables from CPGC.[br] 001 - Always
                               On : CADB will drive Deselect patterns on every
                               DRAM command clock. All lanes will be driven
                               including CKE and ODT. This is not a functional
                               mode and should only be used for BDV purposes.
                               [br] 101 - MRS Mode : CADB will drive patterns
                               under the control of the CADB MRS state machine.
                               All lanes will be driven including CKE and ODT.
                               This is the way to send MRS commands to DRAM
                               devices using CADB. This mode requires the
                               CADB_MRSCFG register to be programmed to specify
                               the timing between commands.[br] 100 - Reserved:
                               Starting the CADB engine in this mode will
                               ignore the start.[br] 000 - Off : Starting the
                               CADB engine in this mode will ignore the start.
                            */

  } Bits;
  UINT32 Data;

} CADB_CFG_MCCADB_MAIN_STRUCT;

/* CADB_DLY_MCCADB_MAIN_REG supported on:                                       */
/*      SPRA0 (0x20007020)                                                      */
/*      SPRB0 (0x20007020)                                                      */
/*      SPRHBM (0x20007020)                                                     */
/*      SPRC0 (0x20007020)                                                      */
/*      SPRMCC (0x20007020)                                                     */
/*      SPRUCC (0x20007020)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* CADB Start Delay
*/


#define CADB_DLY_MCCADB_MAIN_REG 0x27027020

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 start_delay : 10;

                            /* Bits[9:0], Access Type=RW, default=0x00000000*/

                            /*
                               Number of clock cycles (in DRAM clock domain)
                               the start of the test is delayed following the
                               user setting START_TEST, or a start from the
                               Global signaling, or a start from
                               CPGC_TO_CADB_BIND. This is usually used to
                               synchronize multiple SoC CPGC and CADB engines
                               on multiple channels according to a defined
                               phase relationship.
                            */
    UINT32 rsvd : 6;

                            /* Bits[15:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 stop_delay : 10;

                            /* Bits[25:16], Access Type=RW, default=0x00000000*/

                            /*
                               Number of clock cycles (in DRAM clock domain)
                               the stop of the test is delayed by following a
                               stop from the CPGC_TO_CADB_BIND. This is usually
                               used to extend the CADB deselects after the CPGC
                               has finished if some transactions are still
                               pending and it might otherwise stop too soon.
                            */
    UINT32 rsvd_26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_DLY_MCCADB_MAIN_STRUCT;

/* CADB_STATUS_MCCADB_MAIN_REG supported on:                                    */
/*      SPRA0 (0x20007024)                                                      */
/*      SPRB0 (0x20007024)                                                      */
/*      SPRHBM (0x20007024)                                                     */
/*      SPRC0 (0x20007024)                                                      */
/*      SPRMCC (0x20007024)                                                     */
/*      SPRUCC (0x20007024)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Sequence Test Status Register
*/


#define CADB_STATUS_MCCADB_MAIN_REG 0x27027024

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 21;

                            /* Bits[20:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mrs_curr_ptr : 3;

                            /* Bits[23:21], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Current MRS/Select command entry pointer from
                               the CADB buffer. Not very useful.
                            */
    UINT32 test_aborted : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This bit will be set when, following a Start
                               Test, while in Start Test Delay period, the CADB
                               engine encounters a stop condition. In this
                               case, Test Done will not be set, however this
                               bit will be set instead. Possible stop
                               conditions include Stop Test or an external Stop
                               Test from CPGC/CADB instance with Bind Stop Test
                               set.
                            */
    UINT32 rsvd_25 : 3;

                            /* Bits[27:25], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 algo_done : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This bit will be set when all the MRS/AO
                               commands are complete. It is cleared on a Start
                               Test.
                            */
    UINT32 start_test_delay_busy : 1;

                            /* Bits[29:29], Access Type=RO/V, default=0x00000000*/

                            /*
                               This bit will be set when a test has started.
                               Bit is cleared on a reset or after waiting for
                               START_TEST_DELAY clocks. It will not ever set if
                               the delay is 0. It will also assert during the
                               delay between a stop test during any
                               STOP_TEST_DELAY clocks.
                            */
    UINT32 test_busy : 1;

                            /* Bits[30:30], Access Type=RO/V, default=0x00000000*/

                            /*
                               This bit will be set by the RTL while the CADB
                               is able to drive cycles. It is cleared on a
                               reset or once the test is done.
                            */
    UINT32 test_done : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This bit will be set by the RTL when the test is
                               complete. It is cleared on a reset, when the
                               user starts another test, or by writing a 1.
                            */

  } Bits;
  UINT32 Data;

} CADB_STATUS_MCCADB_MAIN_STRUCT;


/* CADB_OVERRIDE_N1_MCCADB_MAIN_REG supported on:                               */
/*      SPRA0 (0x2000702c)                                                      */
/*      SPRB0 (0x2000702c)                                                      */
/*      SPRHBM (0x2000702c)                                                     */
/*      SPRC0 (0x2000702c)                                                      */
/*      SPRMCC (0x2000702c)                                                     */
/*      SPRUCC (0x2000702c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* CADB Chicken Register
*/


#define CADB_OVERRIDE_N1_MCCADB_MAIN_REG 0x2702702C

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

} CADB_OVERRIDE_N1_MCCADB_MAIN_STRUCT;

/* CADB_DSEL_UNISEQ_STAGR_POLY_N0_MCCADB_MAIN_REG supported on:                 */
/*      SPRA0 (0x20007030)                                                      */
/*      SPRB0 (0x20007030)                                                      */
/*      SPRHBM (0x20007030)                                                     */
/*      SPRC0 (0x20007030)                                                      */
/*      SPRMCC (0x20007030)                                                     */
/*      SPRUCC (0x20007030)                                                     */
/* Register default value on SPRA0: 0x00411421                                  */
/* Register default value on SPRB0: 0x00411421                                  */
/* Register default value on SPRHBM: 0x00411421                                 */
/* Register default value on SPRC0: 0x00411421                                  */
/* Register default value on SPRMCC: 0x00411421                                 */
/* Register default value on SPRUCC: 0x00411421                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Unisequencer Stagger Polynomials
*/


#define CADB_DSEL_UNISEQ_STAGR_POLY_N0_MCCADB_MAIN_REG 0x27027030

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 stagr1_poly : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000021*/

                            /* Unisequencer Stagger Polynomial 1 */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 stagr2_poly : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000014*/

                            /* Unisequencer Stagger Polynomial 2 */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 stagr3_poly : 7;

                            /* Bits[22:16], Access Type=RW, default=0x00000041*/

                            /* Unisequencer Stagger Polynomial 3 */
    UINT32 rsvd_23 : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_DSEL_UNISEQ_STAGR_POLY_N0_MCCADB_MAIN_STRUCT;

/* CADB_DSEL_UNISEQ_STAGR_POLY_N1_MCCADB_MAIN_REG supported on:                 */
/*      SPRA0 (0x20007034)                                                      */
/*      SPRB0 (0x20007034)                                                      */
/*      SPRHBM (0x20007034)                                                     */
/*      SPRC0 (0x20007034)                                                      */
/*      SPRMCC (0x20007034)                                                     */
/*      SPRUCC (0x20007034)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Unisequencer Stagger Polynomials
*/


#define CADB_DSEL_UNISEQ_STAGR_POLY_N1_MCCADB_MAIN_REG 0x27027034

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

} CADB_DSEL_UNISEQ_STAGR_POLY_N1_MCCADB_MAIN_STRUCT;

/* CADB_DSEL_UNISEQ_CFG_0_MCCADB_MAIN_REG supported on:                         */
/*      SPRA0 (0x7038)                                                          */
/*      SPRB0 (0x7038)                                                          */
/*      SPRHBM (0x7038)                                                         */
/*      SPRC0 (0x7038)                                                          */
/*      SPRMCC (0x7038)                                                         */
/*      SPRUCC (0x7038)                                                         */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Unisequencer # Configuration.
*/


#define CADB_DSEL_UNISEQ_CFG_0_MCCADB_MAIN_REG 0x27007038

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 uniseq_mode : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000001*/

                            /*
                               Defines the operational mode for Deselect
                               Unisequencer as follows:[br] 00 - LMN Clock
                               Pattern Generator[br] 01 - Pattern Buffer Shift
                               Pattern Generator[br] 10 - Fibonacci LFSR based
                               PRBS Pattern Generator (fixed polynomials)[br]
                               11 - Galois LFSR based PRBS Pattern Generator
                               (programmable polynomials)[br] A single LMN is
                               available to be selected for any Unisequencer.
                            */
    UINT8 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT8 uniseq_lfsr_stagger : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               0=No Stagger, 1=Stagger.[br] This bit must not
                               be set unless Uniseq_Mode is programmed to
                               Select LFSR (Fibonacci or Galois) or the
                               behavior is undefined.[br] Stagger is disallowed
                               for DDR5R and for CADB_DSEL_THROTTLE modes > 1N.
                            */
    UINT8 uniseq_lfsr_polynomial_size : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Possible polynomial sizes:[br] 0x0=Use 8 bit
                               LFSR (x8 + x6 + x5 + x4)[br] 0x1=Use 15 bit LFSR
                               (x15 + x14)[br] 0x2=Use 31 bit LFSR (x31 +
                               x28)[br] 0x3=Use 23 bit LFSR (x23 + x18)[br]
                               0x4=Use 7 bit LFSR (x7 + x6)[br] 0x5=Use 16 bit
                               LFSR (x16+x5+x4+x3)[br] 0x6=Use 23 bit LFSR
                               (x23+x21+x18+x15+x7+x2)[br] 0x7=Use 32 bit LFSR
                               (x32+x31+x30+x10)[br] When set to 0x7, the
                               Galois LFSR based PRBS Pattern Generator will
                               modify its behavior slightly:[br] It will no
                               longer mask the seed as provided, but use it as-
                               is. This can cause a non-PRBS sequence to be
                               generated, or for the LFSR to hit a stead state
                               so should only be used when appropriate for such
                               behavior, such as to acheive an effective shift
                               length longer than 32-bits.
                            */
    UINT8 rsvd_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT8 Data;

} CADB_DSEL_UNISEQ_CFG_0_MCCADB_MAIN_STRUCT;

/* CADB_DSEL_UNISEQ_CFG_1_MCCADB_MAIN_REG supported on:                         */
/*      SPRA0 (0x7039)                                                          */
/*      SPRB0 (0x7039)                                                          */
/*      SPRHBM (0x7039)                                                         */
/*      SPRC0 (0x7039)                                                          */
/*      SPRMCC (0x7039)                                                         */
/*      SPRUCC (0x7039)                                                         */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Unisequencer # Configuration.
*/


#define CADB_DSEL_UNISEQ_CFG_1_MCCADB_MAIN_REG 0x27007039

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 uniseq_mode : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000001*/

                            /*
                               Defines the operational mode for Deselect
                               Unisequencer as follows:[br] 00 - LMN Clock
                               Pattern Generator[br] 01 - Pattern Buffer Shift
                               Pattern Generator[br] 10 - Fibonacci LFSR based
                               PRBS Pattern Generator (fixed polynomials)[br]
                               11 - Galois LFSR based PRBS Pattern Generator
                               (programmable polynomials)[br] A single LMN is
                               available to be selected for any Unisequencer.
                            */
    UINT8 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT8 uniseq_lfsr_stagger : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               0=No Stagger, 1=Stagger.[br] This bit must not
                               be set unless Uniseq_Mode is programmed to
                               Select LFSR (Fibonacci or Galois) or the
                               behavior is undefined.[br] Stagger is disallowed
                               for DDR5R and for CADB_DSEL_THROTTLE modes > 1N.
                            */
    UINT8 uniseq_lfsr_polynomial_size : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Possible polynomial sizes:[br] 0x0=Use 8 bit
                               LFSR (x8 + x6 + x5 + x4)[br] 0x1=Use 15 bit LFSR
                               (x15 + x14)[br] 0x2=Use 31 bit LFSR (x31 +
                               x28)[br] 0x3=Use 23 bit LFSR (x23 + x18)[br]
                               0x4=Use 7 bit LFSR (x7 + x6)[br] 0x5=Use 16 bit
                               LFSR (x16+x5+x4+x3)[br] 0x6=Use 23 bit LFSR
                               (x23+x21+x18+x15+x7+x2)[br] 0x7=Use 32 bit LFSR
                               (x32+x31+x30+x10)[br] When set to 0x7, the
                               Galois LFSR based PRBS Pattern Generator will
                               modify its behavior slightly:[br] It will no
                               longer mask the seed as provided, but use it as-
                               is. This can cause a non-PRBS sequence to be
                               generated, or for the LFSR to hit a stead state
                               so should only be used when appropriate for such
                               behavior, such as to acheive an effective shift
                               length longer than 32-bits.
                            */
    UINT8 rsvd_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT8 Data;

} CADB_DSEL_UNISEQ_CFG_1_MCCADB_MAIN_STRUCT;

/* CADB_DSEL_UNISEQ_CFG_2_MCCADB_MAIN_REG supported on:                         */
/*      SPRA0 (0x703a)                                                          */
/*      SPRB0 (0x703a)                                                          */
/*      SPRHBM (0x703a)                                                         */
/*      SPRC0 (0x703a)                                                          */
/*      SPRMCC (0x703a)                                                         */
/*      SPRUCC (0x703a)                                                         */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Unisequencer # Configuration.
*/


#define CADB_DSEL_UNISEQ_CFG_2_MCCADB_MAIN_REG 0x2700703A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 uniseq_mode : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000001*/

                            /*
                               Defines the operational mode for Deselect
                               Unisequencer as follows:[br] 00 - LMN Clock
                               Pattern Generator[br] 01 - Pattern Buffer Shift
                               Pattern Generator[br] 10 - Fibonacci LFSR based
                               PRBS Pattern Generator (fixed polynomials)[br]
                               11 - Galois LFSR based PRBS Pattern Generator
                               (programmable polynomials)[br] A single LMN is
                               available to be selected for any Unisequencer.
                            */
    UINT8 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT8 uniseq_lfsr_stagger : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               0=No Stagger, 1=Stagger.[br] This bit must not
                               be set unless Uniseq_Mode is programmed to
                               Select LFSR (Fibonacci or Galois) or the
                               behavior is undefined.[br] Stagger is disallowed
                               for DDR5R and for CADB_DSEL_THROTTLE modes > 1N.
                            */
    UINT8 uniseq_lfsr_polynomial_size : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Possible polynomial sizes:[br] 0x0=Use 8 bit
                               LFSR (x8 + x6 + x5 + x4)[br] 0x1=Use 15 bit LFSR
                               (x15 + x14)[br] 0x2=Use 31 bit LFSR (x31 +
                               x28)[br] 0x3=Use 23 bit LFSR (x23 + x18)[br]
                               0x4=Use 7 bit LFSR (x7 + x6)[br] 0x5=Use 16 bit
                               LFSR (x16+x5+x4+x3)[br] 0x6=Use 23 bit LFSR
                               (x23+x21+x18+x15+x7+x2)[br] 0x7=Use 32 bit LFSR
                               (x32+x31+x30+x10)[br] When set to 0x7, the
                               Galois LFSR based PRBS Pattern Generator will
                               modify its behavior slightly:[br] It will no
                               longer mask the seed as provided, but use it as-
                               is. This can cause a non-PRBS sequence to be
                               generated, or for the LFSR to hit a stead state
                               so should only be used when appropriate for such
                               behavior, such as to acheive an effective shift
                               length longer than 32-bits.
                            */
    UINT8 rsvd_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT8 Data;

} CADB_DSEL_UNISEQ_CFG_2_MCCADB_MAIN_STRUCT;

/* CADB_DSEL_UNISEQ_CFG_3_MCCADB_MAIN_REG supported on:                         */
/*      SPRA0 (0x703b)                                                          */
/*      SPRB0 (0x703b)                                                          */
/*      SPRHBM (0x703b)                                                         */
/*      SPRC0 (0x703b)                                                          */
/*      SPRMCC (0x703b)                                                         */
/*      SPRUCC (0x703b)                                                         */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Unisequencer # Configuration.
*/


#define CADB_DSEL_UNISEQ_CFG_3_MCCADB_MAIN_REG 0x2700703B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 uniseq_mode : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000001*/

                            /*
                               Defines the operational mode for Deselect
                               Unisequencer as follows:[br] 00 - LMN Clock
                               Pattern Generator[br] 01 - Pattern Buffer Shift
                               Pattern Generator[br] 10 - Fibonacci LFSR based
                               PRBS Pattern Generator (fixed polynomials)[br]
                               11 - Galois LFSR based PRBS Pattern Generator
                               (programmable polynomials)[br] A single LMN is
                               available to be selected for any Unisequencer.
                            */
    UINT8 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT8 uniseq_lfsr_stagger : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               0=No Stagger, 1=Stagger.[br] This bit must not
                               be set unless Uniseq_Mode is programmed to
                               Select LFSR (Fibonacci or Galois) or the
                               behavior is undefined.[br] Stagger is disallowed
                               for DDR5R and for CADB_DSEL_THROTTLE modes > 1N.
                            */
    UINT8 uniseq_lfsr_polynomial_size : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Possible polynomial sizes:[br] 0x0=Use 8 bit
                               LFSR (x8 + x6 + x5 + x4)[br] 0x1=Use 15 bit LFSR
                               (x15 + x14)[br] 0x2=Use 31 bit LFSR (x31 +
                               x28)[br] 0x3=Use 23 bit LFSR (x23 + x18)[br]
                               0x4=Use 7 bit LFSR (x7 + x6)[br] 0x5=Use 16 bit
                               LFSR (x16+x5+x4+x3)[br] 0x6=Use 23 bit LFSR
                               (x23+x21+x18+x15+x7+x2)[br] 0x7=Use 32 bit LFSR
                               (x32+x31+x30+x10)[br] When set to 0x7, the
                               Galois LFSR based PRBS Pattern Generator will
                               modify its behavior slightly:[br] It will no
                               longer mask the seed as provided, but use it as-
                               is. This can cause a non-PRBS sequence to be
                               generated, or for the LFSR to hit a stead state
                               so should only be used when appropriate for such
                               behavior, such as to acheive an effective shift
                               length longer than 32-bits.
                            */
    UINT8 rsvd_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT8 Data;

} CADB_DSEL_UNISEQ_CFG_3_MCCADB_MAIN_STRUCT;

/* CADB_DSEL_UNISEQ_SAVE_RELOAD_MCCADB_MAIN_REG supported on:                   */
/*      SPRA0 (0x1000703c)                                                      */
/*      SPRB0 (0x1000703c)                                                      */
/*      SPRHBM (0x1000703c)                                                     */
/*      SPRC0 (0x1000703c)                                                      */
/*      SPRMCC (0x1000703c)                                                     */
/*      SPRUCC (0x1000703c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Unisequencer PRBS Save/Reload Configuration.
*/


#define CADB_DSEL_UNISEQ_SAVE_RELOAD_MCCADB_MAIN_REG 0x2701703C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 save_lfsr_seed_rate : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               Sets the number of Reloads counted before the
                               Unisequencer state is saved into an internal
                               save register. (These are currently hidden
                               registers)[br] The internal save register is
                               initially loaded at the start of a test with the
                               Seed from CADB_DSEL_UNISEQ_PBUF.CMD. [br] The
                               current LFSR state is saved into the internal
                               save register every 2^(Reload_LFSR_Seed_Rate-1)
                               * (Save_LFSR_Seed_Rate) clocks.[br] When the
                               Save_LFSR_Seed_Rate and Reload_LFSR_Seed_Rate is
                               reached following the same Clock, only the
                               saving of the LFSR seed takes place.[br]
                               Examples:[br] 0 = Disable - The current LFSR
                               seed is saved only at the start of the test into
                               the internal save register.[br] 1 = Save every
                               Reload (Effectively disables Reload).[br] 2 =
                               The current LFSR seed is saved every other
                               Reload (2^(Reload_LFSR_Seed_Rate) Cache
                               Lines).[br] 3 = The current LFSR seed is saved
                               every third Reload.
                            */
    UINT16 reload_lfsr_seed_rate : 5;

                            /* Bits[12:8], Access Type=RW, default=0x00000000*/

                            /*
                               The current LFSR state is reloaded from and
                               internal Save buffer every[br]
                               2^(Reload_LFSR_Seed_Rate-1) CADB Clocks worth of
                               Deselect activity.[br] When the
                               Save_LFSR_Seed_Rate and Reload_LFSR_Seed_Rate is
                               reached in the same transaction, only the saving
                               of the LFSR seed takes place. A Save Rate of 1
                               will override this and effectively disable
                               reloads.[br] Examples:[br] 0=Disable - The
                               current LFSR seed is never reloaded. Also
                               disables Saves.[br] 1= The current LFSR seed is
                               reloaded every Clock.[br] 2=The current LFSR
                               seed is reloaded every other Clock.[br] 3=The
                               current LFSR seed is reloaded every fourth
                               Clock.
                            */
    UINT16 rsvd : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} CADB_DSEL_UNISEQ_SAVE_RELOAD_MCCADB_MAIN_STRUCT;

/* CADB_DSEL_UNISEQ_PBUF_0_MCCADB_MAIN_REG supported on:                        */
/*      SPRA0 (0x20007040)                                                      */
/*      SPRB0 (0x20007040)                                                      */
/*      SPRHBM (0x20007040)                                                     */
/*      SPRC0 (0x20007040)                                                      */
/*      SPRMCC (0x20007040)                                                     */
/*      SPRUCC (0x20007040)                                                     */
/* Register default value on SPRA0: 0x0000AA55                                  */
/* Register default value on SPRB0: 0x0000AA55                                  */
/* Register default value on SPRHBM: 0x0000AA55                                 */
/* Register default value on SPRC0: 0x0000AA55                                  */
/* Register default value on SPRMCC: 0x0000AA55                                 */
/* Register default value on SPRUCC: 0x0000AA55                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Unisequencer # PRBS/PBUF Seed
*/


#define CADB_DSEL_UNISEQ_PBUF_0_MCCADB_MAIN_REG 0x27027040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd : 32;

                            /* Bits[31:0], Access Type=RW, default=0x0000AA55*/

                            /*
                               The initial content of the rotating pattern
                               buffer for the deselect unified sequencer. Used
                               in both LFSR and pattern buffer modes.
                            */

  } Bits;
  UINT32 Data;

} CADB_DSEL_UNISEQ_PBUF_0_MCCADB_MAIN_STRUCT;

/* CADB_DSEL_UNISEQ_PBUF_1_MCCADB_MAIN_REG supported on:                        */
/*      SPRA0 (0x20007044)                                                      */
/*      SPRB0 (0x20007044)                                                      */
/*      SPRHBM (0x20007044)                                                     */
/*      SPRC0 (0x20007044)                                                      */
/*      SPRMCC (0x20007044)                                                     */
/*      SPRUCC (0x20007044)                                                     */
/* Register default value on SPRA0: 0x0000AA55                                  */
/* Register default value on SPRB0: 0x0000AA55                                  */
/* Register default value on SPRHBM: 0x0000AA55                                 */
/* Register default value on SPRC0: 0x0000AA55                                  */
/* Register default value on SPRMCC: 0x0000AA55                                 */
/* Register default value on SPRUCC: 0x0000AA55                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Unisequencer # PRBS/PBUF Seed
*/


#define CADB_DSEL_UNISEQ_PBUF_1_MCCADB_MAIN_REG 0x27027044

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd : 32;

                            /* Bits[31:0], Access Type=RW, default=0x0000AA55*/

                            /*
                               The initial content of the rotating pattern
                               buffer for the deselect unified sequencer. Used
                               in both LFSR and pattern buffer modes.
                            */

  } Bits;
  UINT32 Data;

} CADB_DSEL_UNISEQ_PBUF_1_MCCADB_MAIN_STRUCT;

/* CADB_DSEL_UNISEQ_PBUF_2_MCCADB_MAIN_REG supported on:                        */
/*      SPRA0 (0x20007048)                                                      */
/*      SPRB0 (0x20007048)                                                      */
/*      SPRHBM (0x20007048)                                                     */
/*      SPRC0 (0x20007048)                                                      */
/*      SPRMCC (0x20007048)                                                     */
/*      SPRUCC (0x20007048)                                                     */
/* Register default value on SPRA0: 0x0000AA55                                  */
/* Register default value on SPRB0: 0x0000AA55                                  */
/* Register default value on SPRHBM: 0x0000AA55                                 */
/* Register default value on SPRC0: 0x0000AA55                                  */
/* Register default value on SPRMCC: 0x0000AA55                                 */
/* Register default value on SPRUCC: 0x0000AA55                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Unisequencer # PRBS/PBUF Seed
*/


#define CADB_DSEL_UNISEQ_PBUF_2_MCCADB_MAIN_REG 0x27027048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd : 32;

                            /* Bits[31:0], Access Type=RW, default=0x0000AA55*/

                            /*
                               The initial content of the rotating pattern
                               buffer for the deselect unified sequencer. Used
                               in both LFSR and pattern buffer modes.
                            */

  } Bits;
  UINT32 Data;

} CADB_DSEL_UNISEQ_PBUF_2_MCCADB_MAIN_STRUCT;

/* CADB_DSEL_UNISEQ_PBUF_3_MCCADB_MAIN_REG supported on:                        */
/*      SPRA0 (0x2000704c)                                                      */
/*      SPRB0 (0x2000704c)                                                      */
/*      SPRHBM (0x2000704c)                                                     */
/*      SPRC0 (0x2000704c)                                                      */
/*      SPRMCC (0x2000704c)                                                     */
/*      SPRUCC (0x2000704c)                                                     */
/* Register default value on SPRA0: 0x0000AA55                                  */
/* Register default value on SPRB0: 0x0000AA55                                  */
/* Register default value on SPRHBM: 0x0000AA55                                 */
/* Register default value on SPRC0: 0x0000AA55                                  */
/* Register default value on SPRMCC: 0x0000AA55                                 */
/* Register default value on SPRUCC: 0x0000AA55                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Unisequencer # PRBS/PBUF Seed
*/


#define CADB_DSEL_UNISEQ_PBUF_3_MCCADB_MAIN_REG 0x2702704C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd : 32;

                            /* Bits[31:0], Access Type=RW, default=0x0000AA55*/

                            /*
                               The initial content of the rotating pattern
                               buffer for the deselect unified sequencer. Used
                               in both LFSR and pattern buffer modes.
                            */

  } Bits;
  UINT32 Data;

} CADB_DSEL_UNISEQ_PBUF_3_MCCADB_MAIN_STRUCT;

/* CADB_DSEL_UNISEQ_LMN_MCCADB_MAIN_REG supported on:                           */
/*      SPRA0 (0x20007050)                                                      */
/*      SPRB0 (0x20007050)                                                      */
/*      SPRHBM (0x20007050)                                                     */
/*      SPRC0 (0x20007050)                                                      */
/*      SPRMCC (0x20007050)                                                     */
/*      SPRUCC (0x20007050)                                                     */
/* Register default value on SPRA0: 0x01010100                                  */
/* Register default value on SPRB0: 0x01010100                                  */
/* Register default value on SPRHBM: 0x01010100                                 */
/* Register default value on SPRC0: 0x01010100                                  */
/* Register default value on SPRMCC: 0x01010100                                 */
/* Register default value on SPRUCC: 0x01010100                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Unisequencer # Data Pattern LMN control.
*/


#define CADB_DSEL_UNISEQ_LMN_MCCADB_MAIN_REG 0x27027050

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 l_sel : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               The initial logic output of the sequencer is
                               defined as follows:[br] 0 - Initial Output Logic
                               '0'[br] 1 - Initial Output Logic '1'
                            */
    UINT32 freq_sweep_en : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               If L-Count mode, then the following pattern is
                               continuously repeated:[br] 1) Drive L_Polarity
                               for L_Count Bits[br] 2) Drive inverse of
                               L_Polarity for L_Count Bits[br] 3) Go to step 1
                               [br] [br] If Normal LMN mode, then the following
                               pattern is continuously repeated:[br] 1) Drive
                               L_Count bits with the polarity set by
                               L_Polarity[br] 2) Drive M_Count bits with the
                               inverse of L_Polarity[br] 3) Drive N_Count bits
                               with the value of L_Polarity[br] 4) Go to step
                               2)[br] If any L_Count, M_Count, or N_Count=0
                               then the state will freeze at the polarity
                               driven for that stage.[br] [br] If Frequency
                               Sweep mode, then the following pattern is
                               continuously repeated:[br] 1) Drive L_Count bits
                               with the polarity set by L_Polarity[br] 2)
                               X_Count=M_Count [br] 3) Drive X_Count bits with
                               the inverse of L_Polarity[br] 4) Drive X_Count
                               bits with the value of L_Polarity[br] 5) Drive
                               X_Count bits with the inverse of L_Polarity[br]
                               6) If X_Count==N_Count then go to step 9[br] 7)
                               X_Count=X_Count-1[br] 8) Go to step 10[br] 9)
                               X_Count=M_Count[br] 10) Drive X_Count bits with
                               the value of L_Polarity[br] 11) Go to step 3[br]
                               [br] In Dwell Frequency Sweep mode, behavior is
                               similar to normal Frequency Sweep mode[br] 1)
                               X_Count=M_Count [br] 2) Drive X_Count bits with
                               the value of L_Polarity[br] 3) Drive X_Count
                               bits with the inverse of L_Polarity[br] 4)
                               Repeat 2 through 3 for L_Count times[br] 5) If
                               X_Count==N_Count then go to step 1[br] 6)
                               X_Count=X_Count-1[br] 7) Go to step 2[br] [br]
                               For {L_COUNT_EN,FREQ_SWEEP_EN}[br] if {x,1},
                               L_Count, M_Count, and N_Count must never be
                               programmed to 0 and is considered undefined.[br]
                               if {x,1}, M_Count and N_Count must be greater
                               than or equal to 8 or behavior may be
                               implementation specific.[br] if {x,1}, N_Count
                               must be programmed less than M_Count.[br] if
                               {1,0}, L_Count must never be programmed to 0 and
                               is considered undefined.[br] if {1,1}, L_Count
                               is not recommended be programmed to 1.
                            */
    UINT32 l_count_en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Used in concatenation with FREQ_SWEEP_EN to
                               enable 4 possible modes of LMN behavior.[br]
                               {L_COUNT_EN,FREQ_SWEEP_EN}[br] {0,0} - Normal
                               LMN - clock pattern using an initial L clock
                               phase followed by M and N repeating.[br] {0,1} -
                               Frequency Sweep mode - clock pattern from 2*M
                               period down to 2*N_Count period and repeat. Four
                               phases per frequency.[br] {1,0} - L-Counter -
                               clock pattern of 2*L period.[br] {1,1} - Dwell
                               Frequency Sweep mode - clock pattern from 2*M
                               period down to 2*N_Count period and repeat.
                               2*L_Count phases per frequency.
                            */
    UINT32 rsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 l_counter_hi : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /*
                               Additional 4 bits to increase the size of L
                               Count up to 12. Considered as 4'b0000 in Dwell
                               Frequency Sweep mode (ignored).[br] For all the
                               following descriptions, L_Count is considered to
                               be this concatenation of {L_COUNTER_HI,L_CNT}.
                            */
    UINT32 l_cnt : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000001*/

                            /*
                               Lower portion of L_Count, used for generating a
                               clock pattern from the LMN patgen.[br] See
                               L_Count_En and Frequency_Sweep_En for exact
                               behavior.
                            */
    UINT32 m_cnt : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000001*/

                            /*
                               M_Count, used for generating a clock pattern
                               from the LMN patgen.[br] See L_Count_En and
                               Frequency_Sweep_En for exact behavior.
                            */
    UINT32 n_cnt : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000001*/

                            /*
                               N_Count, used for generating a clock pattern
                               from the LMN patgen.[br] [br] In L-Counter mode
                               (L Counter Mode Enable =1, Frequency Sweep Mode
                               Enable = 0), the values of the M and N count
                               fields are ignored, and a periodic square wave
                               is generated where each phase is L_Count bits
                               long.[br] In Normal LMN Counter mode (L Counter
                               Mode Enable =0, Frequency Sweep Mode Enable =
                               0), a steady state frequency is driven using an
                               initial phase of L_Count bits, followed by a
                               repeating clock pattern using M and N as the two
                               phases.[br] In Normal Frequency Sweep mode (L
                               Counter Mode Enable =0, Frequency Sweep Mode
                               Enable = 1), a frequency sweep is continuously
                               generated ranging from 2*(M) to 2*(N) in period
                               with 4 phases per frequency step before
                               increasing the frequency.[br] The initial phase
                               of the 2*M period clock is held for L Count
                               bits.[br] In Dwell Frequency Sweep mode (L
                               Counter Mode Enable =1, Frequency Sweep Mode
                               Enable = 1), a frequency sweep is continuously
                               generated ranging from 2*(M) to 2*(N) in period
                               with 2*L_Count phases per frequency step before
                               increasing the frequency.[br] [br] See
                               L_Counter_Mode_En and Frequency_Sweep_Mode_En
                               for exact behavior.
                            */

  } Bits;
  UINT32 Data;

} CADB_DSEL_UNISEQ_LMN_MCCADB_MAIN_STRUCT;

/* CADB_DSEL_UNISEQ_POLY_0_MCCADB_MAIN_REG supported on:                        */
/*      SPRA0 (0x20007058)                                                      */
/*      SPRB0 (0x20007058)                                                      */
/*      SPRHBM (0x20007058)                                                     */
/*      SPRC0 (0x20007058)                                                      */
/*      SPRMCC (0x20007058)                                                     */
/*      SPRUCC (0x20007058)                                                     */
/* Register default value on SPRA0: 0x80000000                                  */
/* Register default value on SPRB0: 0x80000000                                  */
/* Register default value on SPRHBM: 0x80000000                                 */
/* Register default value on SPRC0: 0x80000000                                  */
/* Register default value on SPRMCC: 0x80000000                                 */
/* Register default value on SPRUCC: 0x80000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Unisequencer # Galois PRBS Polynomial.
*/


#define CADB_DSEL_UNISEQ_POLY_0_MCCADB_MAIN_REG 0x27027058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 galois_poly : 32;

                            /* Bits[31:0], Access Type=RW, default=0x80000000*/

                            /*
                               When programmed to a value that has a single bit
                               set, this will create a shift register of the
                               CADB_DPAT_UNISEQ.PATTERN_BUFFER of the length
                               from bit 0 including that set bit. The default
                               value is for a 32-bit shift register. Normal
                               maximal length polynomial values will have an
                               odd number of bits set. Some validated values
                               for different PRBS lengths:[br] 32'h0000_00B8 //
                               x8 + x6 + x5 + x4 +1[br] 32'h0000_6000 // x15 +
                               x14 + 1[br] 32'h4800_0000 // x31 + x28 + 1[br]
                               32'h0042_0000 // x23 + x18 + 1[br] 32'h0000_0060
                               // x7 + x6 + 1[br] 32'h0000_801C // x16 + x5 +
                               x4 + x3 + 1[br] 32'h0052_4042 // x23 + x21 + x18
                               + x15 + x7 + x2 + 1[br] 32'h8020_0003 // x32 +
                               x22 + x2 + x1 + 1[br] 32'h8000_0000 // 32-bit
                               shift register[br] 32'h0000_8000 // 16-bit shift
                               register
                            */

  } Bits;
  UINT32 Data;

} CADB_DSEL_UNISEQ_POLY_0_MCCADB_MAIN_STRUCT;

/* CADB_DSEL_UNISEQ_POLY_1_MCCADB_MAIN_REG supported on:                        */
/*      SPRA0 (0x2000705c)                                                      */
/*      SPRB0 (0x2000705c)                                                      */
/*      SPRHBM (0x2000705c)                                                     */
/*      SPRC0 (0x2000705c)                                                      */
/*      SPRMCC (0x2000705c)                                                     */
/*      SPRUCC (0x2000705c)                                                     */
/* Register default value on SPRA0: 0x80000000                                  */
/* Register default value on SPRB0: 0x80000000                                  */
/* Register default value on SPRHBM: 0x80000000                                 */
/* Register default value on SPRC0: 0x80000000                                  */
/* Register default value on SPRMCC: 0x80000000                                 */
/* Register default value on SPRUCC: 0x80000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Unisequencer # Galois PRBS Polynomial.
*/


#define CADB_DSEL_UNISEQ_POLY_1_MCCADB_MAIN_REG 0x2702705C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 galois_poly : 32;

                            /* Bits[31:0], Access Type=RW, default=0x80000000*/

                            /*
                               When programmed to a value that has a single bit
                               set, this will create a shift register of the
                               CADB_DPAT_UNISEQ.PATTERN_BUFFER of the length
                               from bit 0 including that set bit. The default
                               value is for a 32-bit shift register. Normal
                               maximal length polynomial values will have an
                               odd number of bits set. Some validated values
                               for different PRBS lengths:[br] 32'h0000_00B8 //
                               x8 + x6 + x5 + x4 +1[br] 32'h0000_6000 // x15 +
                               x14 + 1[br] 32'h4800_0000 // x31 + x28 + 1[br]
                               32'h0042_0000 // x23 + x18 + 1[br] 32'h0000_0060
                               // x7 + x6 + 1[br] 32'h0000_801C // x16 + x5 +
                               x4 + x3 + 1[br] 32'h0052_4042 // x23 + x21 + x18
                               + x15 + x7 + x2 + 1[br] 32'h8020_0003 // x32 +
                               x22 + x2 + x1 + 1[br] 32'h8000_0000 // 32-bit
                               shift register[br] 32'h0000_8000 // 16-bit shift
                               register
                            */

  } Bits;
  UINT32 Data;

} CADB_DSEL_UNISEQ_POLY_1_MCCADB_MAIN_STRUCT;

/* CADB_DSEL_UNISEQ_POLY_2_MCCADB_MAIN_REG supported on:                        */
/*      SPRA0 (0x20007060)                                                      */
/*      SPRB0 (0x20007060)                                                      */
/*      SPRHBM (0x20007060)                                                     */
/*      SPRC0 (0x20007060)                                                      */
/*      SPRMCC (0x20007060)                                                     */
/*      SPRUCC (0x20007060)                                                     */
/* Register default value on SPRA0: 0x80000000                                  */
/* Register default value on SPRB0: 0x80000000                                  */
/* Register default value on SPRHBM: 0x80000000                                 */
/* Register default value on SPRC0: 0x80000000                                  */
/* Register default value on SPRMCC: 0x80000000                                 */
/* Register default value on SPRUCC: 0x80000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Unisequencer # Galois PRBS Polynomial.
*/


#define CADB_DSEL_UNISEQ_POLY_2_MCCADB_MAIN_REG 0x27027060

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 galois_poly : 32;

                            /* Bits[31:0], Access Type=RW, default=0x80000000*/

                            /*
                               When programmed to a value that has a single bit
                               set, this will create a shift register of the
                               CADB_DPAT_UNISEQ.PATTERN_BUFFER of the length
                               from bit 0 including that set bit. The default
                               value is for a 32-bit shift register. Normal
                               maximal length polynomial values will have an
                               odd number of bits set. Some validated values
                               for different PRBS lengths:[br] 32'h0000_00B8 //
                               x8 + x6 + x5 + x4 +1[br] 32'h0000_6000 // x15 +
                               x14 + 1[br] 32'h4800_0000 // x31 + x28 + 1[br]
                               32'h0042_0000 // x23 + x18 + 1[br] 32'h0000_0060
                               // x7 + x6 + 1[br] 32'h0000_801C // x16 + x5 +
                               x4 + x3 + 1[br] 32'h0052_4042 // x23 + x21 + x18
                               + x15 + x7 + x2 + 1[br] 32'h8020_0003 // x32 +
                               x22 + x2 + x1 + 1[br] 32'h8000_0000 // 32-bit
                               shift register[br] 32'h0000_8000 // 16-bit shift
                               register
                            */

  } Bits;
  UINT32 Data;

} CADB_DSEL_UNISEQ_POLY_2_MCCADB_MAIN_STRUCT;

/* CADB_DSEL_UNISEQ_POLY_3_MCCADB_MAIN_REG supported on:                        */
/*      SPRA0 (0x20007064)                                                      */
/*      SPRB0 (0x20007064)                                                      */
/*      SPRHBM (0x20007064)                                                     */
/*      SPRC0 (0x20007064)                                                      */
/*      SPRMCC (0x20007064)                                                     */
/*      SPRUCC (0x20007064)                                                     */
/* Register default value on SPRA0: 0x80000000                                  */
/* Register default value on SPRB0: 0x80000000                                  */
/* Register default value on SPRHBM: 0x80000000                                 */
/* Register default value on SPRC0: 0x80000000                                  */
/* Register default value on SPRMCC: 0x80000000                                 */
/* Register default value on SPRUCC: 0x80000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Unisequencer # Galois PRBS Polynomial.
*/


#define CADB_DSEL_UNISEQ_POLY_3_MCCADB_MAIN_REG 0x27027064

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 galois_poly : 32;

                            /* Bits[31:0], Access Type=RW, default=0x80000000*/

                            /*
                               When programmed to a value that has a single bit
                               set, this will create a shift register of the
                               CADB_DPAT_UNISEQ.PATTERN_BUFFER of the length
                               from bit 0 including that set bit. The default
                               value is for a 32-bit shift register. Normal
                               maximal length polynomial values will have an
                               odd number of bits set. Some validated values
                               for different PRBS lengths:[br] 32'h0000_00B8 //
                               x8 + x6 + x5 + x4 +1[br] 32'h0000_6000 // x15 +
                               x14 + 1[br] 32'h4800_0000 // x31 + x28 + 1[br]
                               32'h0042_0000 // x23 + x18 + 1[br] 32'h0000_0060
                               // x7 + x6 + 1[br] 32'h0000_801C // x16 + x5 +
                               x4 + x3 + 1[br] 32'h0052_4042 // x23 + x21 + x18
                               + x15 + x7 + x2 + 1[br] 32'h8020_0003 // x32 +
                               x22 + x2 + x1 + 1[br] 32'h8000_0000 // 32-bit
                               shift register[br] 32'h0000_8000 // 16-bit shift
                               register
                            */

  } Bits;
  UINT32 Data;

} CADB_DSEL_UNISEQ_POLY_3_MCCADB_MAIN_STRUCT;

/* CADB_BUF_0_N0_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x20007068)                                                      */
/*      SPRB0 (0x20007068)                                                      */
/*      SPRHBM (0x20007068)                                                     */
/*      SPRC0 (0x20007068)                                                      */
/*      SPRMCC (0x20007068)                                                     */
/*      SPRUCC (0x20007068)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_0_N0_MCCADB_MAIN_REG 0x27027068

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cs : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip select signals.[br] In MRS Mode,
                               the command will be driven to the rank which is
                               programmed with the CS_ACTIVE_POLARITY
                            */
    UINT32 cid : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip ID signal. Only used in memory
                               technologies with stacked dies.
                            */
    UINT32 odt : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM on-die termination signals. Only used
                               in MRS and Always On modes.
                            */
    UINT32 cke : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM clock enable signals. Only used in MRS
                               and Always On modes.
                            */
    UINT32 use_half_ca : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, only the lower half of this
                               buffer's CMD_ADDR entry will be used. Only
                               applies in memory technologies with folded
                               command buses (e.g. LPDDR4).
                            */
    UINT32 val : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               When this field is set to a 1, the CADB will
                               assert a SPID signal (ca_valid) to indicate the
                               PHY that it should drive its output pins during
                               this DRAM command clock.[br] When this field is
                               set to a 0, the CADB will de-assert the ca_valid
                               signal, to indicate to the PHY that it should
                               tristate its output pins during this DRAM
                               command clock.
                            */
    UINT32 par : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to write the parity invert CADB data. When
                               working with DDR4 if this bit is set and the
                               CADB entry is selected, the parity value is
                               inverted to reflect a CA parity error.
                            */
    UINT32 ca : 14;

                            /* Bits[31:18], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */

  } Bits;
  UINT32 Data;

} CADB_BUF_0_N0_MCCADB_MAIN_STRUCT;

/* CADB_BUF_0_N1_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x2000706c)                                                      */
/*      SPRB0 (0x2000706c)                                                      */
/*      SPRHBM (0x2000706c)                                                     */
/*      SPRC0 (0x2000706c)                                                      */
/*      SPRMCC (0x2000706c)                                                     */
/*      SPRUCC (0x2000706c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_0_N1_MCCADB_MAIN_REG 0x2702706C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca : 18;

                            /* Bits[17:0], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_BUF_0_N1_MCCADB_MAIN_STRUCT;

/* CADB_BUF_1_N0_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x20007070)                                                      */
/*      SPRB0 (0x20007070)                                                      */
/*      SPRHBM (0x20007070)                                                     */
/*      SPRC0 (0x20007070)                                                      */
/*      SPRMCC (0x20007070)                                                     */
/*      SPRUCC (0x20007070)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_1_N0_MCCADB_MAIN_REG 0x27027070

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cs : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip select signals.[br] In MRS Mode,
                               the command will be driven to the rank which is
                               programmed with the CS_ACTIVE_POLARITY
                            */
    UINT32 cid : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip ID signal. Only used in memory
                               technologies with stacked dies.
                            */
    UINT32 odt : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM on-die termination signals. Only used
                               in MRS and Always On modes.
                            */
    UINT32 cke : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM clock enable signals. Only used in MRS
                               and Always On modes.
                            */
    UINT32 use_half_ca : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, only the lower half of this
                               buffer's CMD_ADDR entry will be used. Only
                               applies in memory technologies with folded
                               command buses (e.g. LPDDR4).
                            */
    UINT32 val : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               When this field is set to a 1, the CADB will
                               assert a SPID signal (ca_valid) to indicate the
                               PHY that it should drive its output pins during
                               this DRAM command clock.[br] When this field is
                               set to a 0, the CADB will de-assert the ca_valid
                               signal, to indicate to the PHY that it should
                               tristate its output pins during this DRAM
                               command clock.
                            */
    UINT32 par : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to write the parity invert CADB data. When
                               working with DDR4 if this bit is set and the
                               CADB entry is selected, the parity value is
                               inverted to reflect a CA parity error.
                            */
    UINT32 ca : 14;

                            /* Bits[31:18], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */

  } Bits;
  UINT32 Data;

} CADB_BUF_1_N0_MCCADB_MAIN_STRUCT;

/* CADB_BUF_1_N1_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x20007074)                                                      */
/*      SPRB0 (0x20007074)                                                      */
/*      SPRHBM (0x20007074)                                                     */
/*      SPRC0 (0x20007074)                                                      */
/*      SPRMCC (0x20007074)                                                     */
/*      SPRUCC (0x20007074)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_1_N1_MCCADB_MAIN_REG 0x27027074

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca : 18;

                            /* Bits[17:0], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_BUF_1_N1_MCCADB_MAIN_STRUCT;

/* CADB_BUF_2_N0_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x20007078)                                                      */
/*      SPRB0 (0x20007078)                                                      */
/*      SPRHBM (0x20007078)                                                     */
/*      SPRC0 (0x20007078)                                                      */
/*      SPRMCC (0x20007078)                                                     */
/*      SPRUCC (0x20007078)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_2_N0_MCCADB_MAIN_REG 0x27027078

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cs : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip select signals.[br] In MRS Mode,
                               the command will be driven to the rank which is
                               programmed with the CS_ACTIVE_POLARITY
                            */
    UINT32 cid : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip ID signal. Only used in memory
                               technologies with stacked dies.
                            */
    UINT32 odt : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM on-die termination signals. Only used
                               in MRS and Always On modes.
                            */
    UINT32 cke : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM clock enable signals. Only used in MRS
                               and Always On modes.
                            */
    UINT32 use_half_ca : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, only the lower half of this
                               buffer's CMD_ADDR entry will be used. Only
                               applies in memory technologies with folded
                               command buses (e.g. LPDDR4).
                            */
    UINT32 val : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               When this field is set to a 1, the CADB will
                               assert a SPID signal (ca_valid) to indicate the
                               PHY that it should drive its output pins during
                               this DRAM command clock.[br] When this field is
                               set to a 0, the CADB will de-assert the ca_valid
                               signal, to indicate to the PHY that it should
                               tristate its output pins during this DRAM
                               command clock.
                            */
    UINT32 par : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to write the parity invert CADB data. When
                               working with DDR4 if this bit is set and the
                               CADB entry is selected, the parity value is
                               inverted to reflect a CA parity error.
                            */
    UINT32 ca : 14;

                            /* Bits[31:18], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */

  } Bits;
  UINT32 Data;

} CADB_BUF_2_N0_MCCADB_MAIN_STRUCT;

/* CADB_BUF_2_N1_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x2000707c)                                                      */
/*      SPRB0 (0x2000707c)                                                      */
/*      SPRHBM (0x2000707c)                                                     */
/*      SPRC0 (0x2000707c)                                                      */
/*      SPRMCC (0x2000707c)                                                     */
/*      SPRUCC (0x2000707c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_2_N1_MCCADB_MAIN_REG 0x2702707C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca : 18;

                            /* Bits[17:0], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_BUF_2_N1_MCCADB_MAIN_STRUCT;

/* CADB_BUF_3_N0_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x20007080)                                                      */
/*      SPRB0 (0x20007080)                                                      */
/*      SPRHBM (0x20007080)                                                     */
/*      SPRC0 (0x20007080)                                                      */
/*      SPRMCC (0x20007080)                                                     */
/*      SPRUCC (0x20007080)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_3_N0_MCCADB_MAIN_REG 0x27027080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cs : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip select signals.[br] In MRS Mode,
                               the command will be driven to the rank which is
                               programmed with the CS_ACTIVE_POLARITY
                            */
    UINT32 cid : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip ID signal. Only used in memory
                               technologies with stacked dies.
                            */
    UINT32 odt : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM on-die termination signals. Only used
                               in MRS and Always On modes.
                            */
    UINT32 cke : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM clock enable signals. Only used in MRS
                               and Always On modes.
                            */
    UINT32 use_half_ca : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, only the lower half of this
                               buffer's CMD_ADDR entry will be used. Only
                               applies in memory technologies with folded
                               command buses (e.g. LPDDR4).
                            */
    UINT32 val : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               When this field is set to a 1, the CADB will
                               assert a SPID signal (ca_valid) to indicate the
                               PHY that it should drive its output pins during
                               this DRAM command clock.[br] When this field is
                               set to a 0, the CADB will de-assert the ca_valid
                               signal, to indicate to the PHY that it should
                               tristate its output pins during this DRAM
                               command clock.
                            */
    UINT32 par : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to write the parity invert CADB data. When
                               working with DDR4 if this bit is set and the
                               CADB entry is selected, the parity value is
                               inverted to reflect a CA parity error.
                            */
    UINT32 ca : 14;

                            /* Bits[31:18], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */

  } Bits;
  UINT32 Data;

} CADB_BUF_3_N0_MCCADB_MAIN_STRUCT;

/* CADB_BUF_3_N1_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x20007084)                                                      */
/*      SPRB0 (0x20007084)                                                      */
/*      SPRHBM (0x20007084)                                                     */
/*      SPRC0 (0x20007084)                                                      */
/*      SPRMCC (0x20007084)                                                     */
/*      SPRUCC (0x20007084)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_3_N1_MCCADB_MAIN_REG 0x27027084

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca : 18;

                            /* Bits[17:0], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_BUF_3_N1_MCCADB_MAIN_STRUCT;

/* CADB_BUF_4_N0_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x20007088)                                                      */
/*      SPRB0 (0x20007088)                                                      */
/*      SPRHBM (0x20007088)                                                     */
/*      SPRC0 (0x20007088)                                                      */
/*      SPRMCC (0x20007088)                                                     */
/*      SPRUCC (0x20007088)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_4_N0_MCCADB_MAIN_REG 0x27027088

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cs : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip select signals.[br] In MRS Mode,
                               the command will be driven to the rank which is
                               programmed with the CS_ACTIVE_POLARITY
                            */
    UINT32 cid : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip ID signal. Only used in memory
                               technologies with stacked dies.
                            */
    UINT32 odt : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM on-die termination signals. Only used
                               in MRS and Always On modes.
                            */
    UINT32 cke : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM clock enable signals. Only used in MRS
                               and Always On modes.
                            */
    UINT32 use_half_ca : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, only the lower half of this
                               buffer's CMD_ADDR entry will be used. Only
                               applies in memory technologies with folded
                               command buses (e.g. LPDDR4).
                            */
    UINT32 val : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               When this field is set to a 1, the CADB will
                               assert a SPID signal (ca_valid) to indicate the
                               PHY that it should drive its output pins during
                               this DRAM command clock.[br] When this field is
                               set to a 0, the CADB will de-assert the ca_valid
                               signal, to indicate to the PHY that it should
                               tristate its output pins during this DRAM
                               command clock.
                            */
    UINT32 par : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to write the parity invert CADB data. When
                               working with DDR4 if this bit is set and the
                               CADB entry is selected, the parity value is
                               inverted to reflect a CA parity error.
                            */
    UINT32 ca : 14;

                            /* Bits[31:18], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */

  } Bits;
  UINT32 Data;

} CADB_BUF_4_N0_MCCADB_MAIN_STRUCT;

/* CADB_BUF_4_N1_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x2000708c)                                                      */
/*      SPRB0 (0x2000708c)                                                      */
/*      SPRHBM (0x2000708c)                                                     */
/*      SPRC0 (0x2000708c)                                                      */
/*      SPRMCC (0x2000708c)                                                     */
/*      SPRUCC (0x2000708c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_4_N1_MCCADB_MAIN_REG 0x2702708C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca : 18;

                            /* Bits[17:0], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_BUF_4_N1_MCCADB_MAIN_STRUCT;

/* CADB_BUF_5_N0_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x20007090)                                                      */
/*      SPRB0 (0x20007090)                                                      */
/*      SPRHBM (0x20007090)                                                     */
/*      SPRC0 (0x20007090)                                                      */
/*      SPRMCC (0x20007090)                                                     */
/*      SPRUCC (0x20007090)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_5_N0_MCCADB_MAIN_REG 0x27027090

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cs : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip select signals.[br] In MRS Mode,
                               the command will be driven to the rank which is
                               programmed with the CS_ACTIVE_POLARITY
                            */
    UINT32 cid : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip ID signal. Only used in memory
                               technologies with stacked dies.
                            */
    UINT32 odt : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM on-die termination signals. Only used
                               in MRS and Always On modes.
                            */
    UINT32 cke : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM clock enable signals. Only used in MRS
                               and Always On modes.
                            */
    UINT32 use_half_ca : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, only the lower half of this
                               buffer's CMD_ADDR entry will be used. Only
                               applies in memory technologies with folded
                               command buses (e.g. LPDDR4).
                            */
    UINT32 val : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               When this field is set to a 1, the CADB will
                               assert a SPID signal (ca_valid) to indicate the
                               PHY that it should drive its output pins during
                               this DRAM command clock.[br] When this field is
                               set to a 0, the CADB will de-assert the ca_valid
                               signal, to indicate to the PHY that it should
                               tristate its output pins during this DRAM
                               command clock.
                            */
    UINT32 par : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to write the parity invert CADB data. When
                               working with DDR4 if this bit is set and the
                               CADB entry is selected, the parity value is
                               inverted to reflect a CA parity error.
                            */
    UINT32 ca : 14;

                            /* Bits[31:18], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */

  } Bits;
  UINT32 Data;

} CADB_BUF_5_N0_MCCADB_MAIN_STRUCT;

/* CADB_BUF_5_N1_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x20007094)                                                      */
/*      SPRB0 (0x20007094)                                                      */
/*      SPRHBM (0x20007094)                                                     */
/*      SPRC0 (0x20007094)                                                      */
/*      SPRMCC (0x20007094)                                                     */
/*      SPRUCC (0x20007094)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_5_N1_MCCADB_MAIN_REG 0x27027094

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca : 18;

                            /* Bits[17:0], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_BUF_5_N1_MCCADB_MAIN_STRUCT;

/* CADB_BUF_6_N0_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x20007098)                                                      */
/*      SPRB0 (0x20007098)                                                      */
/*      SPRHBM (0x20007098)                                                     */
/*      SPRC0 (0x20007098)                                                      */
/*      SPRMCC (0x20007098)                                                     */
/*      SPRUCC (0x20007098)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_6_N0_MCCADB_MAIN_REG 0x27027098

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cs : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip select signals.[br] In MRS Mode,
                               the command will be driven to the rank which is
                               programmed with the CS_ACTIVE_POLARITY
                            */
    UINT32 cid : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip ID signal. Only used in memory
                               technologies with stacked dies.
                            */
    UINT32 odt : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM on-die termination signals. Only used
                               in MRS and Always On modes.
                            */
    UINT32 cke : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM clock enable signals. Only used in MRS
                               and Always On modes.
                            */
    UINT32 use_half_ca : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, only the lower half of this
                               buffer's CMD_ADDR entry will be used. Only
                               applies in memory technologies with folded
                               command buses (e.g. LPDDR4).
                            */
    UINT32 val : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               When this field is set to a 1, the CADB will
                               assert a SPID signal (ca_valid) to indicate the
                               PHY that it should drive its output pins during
                               this DRAM command clock.[br] When this field is
                               set to a 0, the CADB will de-assert the ca_valid
                               signal, to indicate to the PHY that it should
                               tristate its output pins during this DRAM
                               command clock.
                            */
    UINT32 par : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to write the parity invert CADB data. When
                               working with DDR4 if this bit is set and the
                               CADB entry is selected, the parity value is
                               inverted to reflect a CA parity error.
                            */
    UINT32 ca : 14;

                            /* Bits[31:18], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */

  } Bits;
  UINT32 Data;

} CADB_BUF_6_N0_MCCADB_MAIN_STRUCT;

/* CADB_BUF_6_N1_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x2000709c)                                                      */
/*      SPRB0 (0x2000709c)                                                      */
/*      SPRHBM (0x2000709c)                                                     */
/*      SPRC0 (0x2000709c)                                                      */
/*      SPRMCC (0x2000709c)                                                     */
/*      SPRUCC (0x2000709c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_6_N1_MCCADB_MAIN_REG 0x2702709C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca : 18;

                            /* Bits[17:0], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_BUF_6_N1_MCCADB_MAIN_STRUCT;

/* CADB_BUF_7_N0_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x200070a0)                                                      */
/*      SPRB0 (0x200070a0)                                                      */
/*      SPRHBM (0x200070a0)                                                     */
/*      SPRC0 (0x200070a0)                                                      */
/*      SPRMCC (0x200070a0)                                                     */
/*      SPRUCC (0x200070a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_7_N0_MCCADB_MAIN_REG 0x270270A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cs : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip select signals.[br] In MRS Mode,
                               the command will be driven to the rank which is
                               programmed with the CS_ACTIVE_POLARITY
                            */
    UINT32 cid : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip ID signal. Only used in memory
                               technologies with stacked dies.
                            */
    UINT32 odt : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM on-die termination signals. Only used
                               in MRS and Always On modes.
                            */
    UINT32 cke : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM clock enable signals. Only used in MRS
                               and Always On modes.
                            */
    UINT32 use_half_ca : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, only the lower half of this
                               buffer's CMD_ADDR entry will be used. Only
                               applies in memory technologies with folded
                               command buses (e.g. LPDDR4).
                            */
    UINT32 val : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               When this field is set to a 1, the CADB will
                               assert a SPID signal (ca_valid) to indicate the
                               PHY that it should drive its output pins during
                               this DRAM command clock.[br] When this field is
                               set to a 0, the CADB will de-assert the ca_valid
                               signal, to indicate to the PHY that it should
                               tristate its output pins during this DRAM
                               command clock.
                            */
    UINT32 par : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to write the parity invert CADB data. When
                               working with DDR4 if this bit is set and the
                               CADB entry is selected, the parity value is
                               inverted to reflect a CA parity error.
                            */
    UINT32 ca : 14;

                            /* Bits[31:18], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */

  } Bits;
  UINT32 Data;

} CADB_BUF_7_N0_MCCADB_MAIN_STRUCT;

/* CADB_BUF_7_N1_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x200070a4)                                                      */
/*      SPRB0 (0x200070a4)                                                      */
/*      SPRHBM (0x200070a4)                                                     */
/*      SPRC0 (0x200070a4)                                                      */
/*      SPRMCC (0x200070a4)                                                     */
/*      SPRUCC (0x200070a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_7_N1_MCCADB_MAIN_REG 0x270270A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca : 18;

                            /* Bits[17:0], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_BUF_7_N1_MCCADB_MAIN_STRUCT;

/* CADB_BUF_8_N0_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x200070a8)                                                      */
/*      SPRB0 (0x200070a8)                                                      */
/*      SPRHBM (0x200070a8)                                                     */
/*      SPRC0 (0x200070a8)                                                      */
/*      SPRMCC (0x200070a8)                                                     */
/*      SPRUCC (0x200070a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_8_N0_MCCADB_MAIN_REG 0x270270A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cs : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip select signals.[br] In MRS Mode,
                               the command will be driven to the rank which is
                               programmed with the CS_ACTIVE_POLARITY
                            */
    UINT32 cid : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip ID signal. Only used in memory
                               technologies with stacked dies.
                            */
    UINT32 odt : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM on-die termination signals. Only used
                               in MRS and Always On modes.
                            */
    UINT32 cke : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM clock enable signals. Only used in MRS
                               and Always On modes.
                            */
    UINT32 use_half_ca : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, only the lower half of this
                               buffer's CMD_ADDR entry will be used. Only
                               applies in memory technologies with folded
                               command buses (e.g. LPDDR4).
                            */
    UINT32 val : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               When this field is set to a 1, the CADB will
                               assert a SPID signal (ca_valid) to indicate the
                               PHY that it should drive its output pins during
                               this DRAM command clock.[br] When this field is
                               set to a 0, the CADB will de-assert the ca_valid
                               signal, to indicate to the PHY that it should
                               tristate its output pins during this DRAM
                               command clock.
                            */
    UINT32 par : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to write the parity invert CADB data. When
                               working with DDR4 if this bit is set and the
                               CADB entry is selected, the parity value is
                               inverted to reflect a CA parity error.
                            */
    UINT32 ca : 14;

                            /* Bits[31:18], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */

  } Bits;
  UINT32 Data;

} CADB_BUF_8_N0_MCCADB_MAIN_STRUCT;

/* CADB_BUF_8_N1_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x200070ac)                                                      */
/*      SPRB0 (0x200070ac)                                                      */
/*      SPRHBM (0x200070ac)                                                     */
/*      SPRC0 (0x200070ac)                                                      */
/*      SPRMCC (0x200070ac)                                                     */
/*      SPRUCC (0x200070ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_8_N1_MCCADB_MAIN_REG 0x270270AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca : 18;

                            /* Bits[17:0], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_BUF_8_N1_MCCADB_MAIN_STRUCT;

/* CADB_BUF_9_N0_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x200070b0)                                                      */
/*      SPRB0 (0x200070b0)                                                      */
/*      SPRHBM (0x200070b0)                                                     */
/*      SPRC0 (0x200070b0)                                                      */
/*      SPRMCC (0x200070b0)                                                     */
/*      SPRUCC (0x200070b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_9_N0_MCCADB_MAIN_REG 0x270270B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cs : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip select signals.[br] In MRS Mode,
                               the command will be driven to the rank which is
                               programmed with the CS_ACTIVE_POLARITY
                            */
    UINT32 cid : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip ID signal. Only used in memory
                               technologies with stacked dies.
                            */
    UINT32 odt : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM on-die termination signals. Only used
                               in MRS and Always On modes.
                            */
    UINT32 cke : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM clock enable signals. Only used in MRS
                               and Always On modes.
                            */
    UINT32 use_half_ca : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, only the lower half of this
                               buffer's CMD_ADDR entry will be used. Only
                               applies in memory technologies with folded
                               command buses (e.g. LPDDR4).
                            */
    UINT32 val : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               When this field is set to a 1, the CADB will
                               assert a SPID signal (ca_valid) to indicate the
                               PHY that it should drive its output pins during
                               this DRAM command clock.[br] When this field is
                               set to a 0, the CADB will de-assert the ca_valid
                               signal, to indicate to the PHY that it should
                               tristate its output pins during this DRAM
                               command clock.
                            */
    UINT32 par : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to write the parity invert CADB data. When
                               working with DDR4 if this bit is set and the
                               CADB entry is selected, the parity value is
                               inverted to reflect a CA parity error.
                            */
    UINT32 ca : 14;

                            /* Bits[31:18], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */

  } Bits;
  UINT32 Data;

} CADB_BUF_9_N0_MCCADB_MAIN_STRUCT;

/* CADB_BUF_9_N1_MCCADB_MAIN_REG supported on:                                  */
/*      SPRA0 (0x200070b4)                                                      */
/*      SPRB0 (0x200070b4)                                                      */
/*      SPRHBM (0x200070b4)                                                     */
/*      SPRC0 (0x200070b4)                                                      */
/*      SPRMCC (0x200070b4)                                                     */
/*      SPRUCC (0x200070b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_9_N1_MCCADB_MAIN_REG 0x270270B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca : 18;

                            /* Bits[17:0], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_BUF_9_N1_MCCADB_MAIN_STRUCT;

/* CADB_BUF_10_N0_MCCADB_MAIN_REG supported on:                                 */
/*      SPRA0 (0x200070b8)                                                      */
/*      SPRB0 (0x200070b8)                                                      */
/*      SPRHBM (0x200070b8)                                                     */
/*      SPRC0 (0x200070b8)                                                      */
/*      SPRMCC (0x200070b8)                                                     */
/*      SPRUCC (0x200070b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_10_N0_MCCADB_MAIN_REG 0x270270B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cs : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip select signals.[br] In MRS Mode,
                               the command will be driven to the rank which is
                               programmed with the CS_ACTIVE_POLARITY
                            */
    UINT32 cid : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip ID signal. Only used in memory
                               technologies with stacked dies.
                            */
    UINT32 odt : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM on-die termination signals. Only used
                               in MRS and Always On modes.
                            */
    UINT32 cke : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM clock enable signals. Only used in MRS
                               and Always On modes.
                            */
    UINT32 use_half_ca : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, only the lower half of this
                               buffer's CMD_ADDR entry will be used. Only
                               applies in memory technologies with folded
                               command buses (e.g. LPDDR4).
                            */
    UINT32 val : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               When this field is set to a 1, the CADB will
                               assert a SPID signal (ca_valid) to indicate the
                               PHY that it should drive its output pins during
                               this DRAM command clock.[br] When this field is
                               set to a 0, the CADB will de-assert the ca_valid
                               signal, to indicate to the PHY that it should
                               tristate its output pins during this DRAM
                               command clock.
                            */
    UINT32 par : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to write the parity invert CADB data. When
                               working with DDR4 if this bit is set and the
                               CADB entry is selected, the parity value is
                               inverted to reflect a CA parity error.
                            */
    UINT32 ca : 14;

                            /* Bits[31:18], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */

  } Bits;
  UINT32 Data;

} CADB_BUF_10_N0_MCCADB_MAIN_STRUCT;

/* CADB_BUF_10_N1_MCCADB_MAIN_REG supported on:                                 */
/*      SPRA0 (0x200070bc)                                                      */
/*      SPRB0 (0x200070bc)                                                      */
/*      SPRHBM (0x200070bc)                                                     */
/*      SPRC0 (0x200070bc)                                                      */
/*      SPRMCC (0x200070bc)                                                     */
/*      SPRUCC (0x200070bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_10_N1_MCCADB_MAIN_REG 0x270270BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca : 18;

                            /* Bits[17:0], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_BUF_10_N1_MCCADB_MAIN_STRUCT;

/* CADB_BUF_11_N0_MCCADB_MAIN_REG supported on:                                 */
/*      SPRA0 (0x200070c0)                                                      */
/*      SPRB0 (0x200070c0)                                                      */
/*      SPRHBM (0x200070c0)                                                     */
/*      SPRC0 (0x200070c0)                                                      */
/*      SPRMCC (0x200070c0)                                                     */
/*      SPRUCC (0x200070c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_11_N0_MCCADB_MAIN_REG 0x270270C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cs : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip select signals.[br] In MRS Mode,
                               the command will be driven to the rank which is
                               programmed with the CS_ACTIVE_POLARITY
                            */
    UINT32 cid : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip ID signal. Only used in memory
                               technologies with stacked dies.
                            */
    UINT32 odt : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM on-die termination signals. Only used
                               in MRS and Always On modes.
                            */
    UINT32 cke : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM clock enable signals. Only used in MRS
                               and Always On modes.
                            */
    UINT32 use_half_ca : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, only the lower half of this
                               buffer's CMD_ADDR entry will be used. Only
                               applies in memory technologies with folded
                               command buses (e.g. LPDDR4).
                            */
    UINT32 val : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               When this field is set to a 1, the CADB will
                               assert a SPID signal (ca_valid) to indicate the
                               PHY that it should drive its output pins during
                               this DRAM command clock.[br] When this field is
                               set to a 0, the CADB will de-assert the ca_valid
                               signal, to indicate to the PHY that it should
                               tristate its output pins during this DRAM
                               command clock.
                            */
    UINT32 par : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to write the parity invert CADB data. When
                               working with DDR4 if this bit is set and the
                               CADB entry is selected, the parity value is
                               inverted to reflect a CA parity error.
                            */
    UINT32 ca : 14;

                            /* Bits[31:18], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */

  } Bits;
  UINT32 Data;

} CADB_BUF_11_N0_MCCADB_MAIN_STRUCT;

/* CADB_BUF_11_N1_MCCADB_MAIN_REG supported on:                                 */
/*      SPRA0 (0x200070c4)                                                      */
/*      SPRB0 (0x200070c4)                                                      */
/*      SPRHBM (0x200070c4)                                                     */
/*      SPRC0 (0x200070c4)                                                      */
/*      SPRMCC (0x200070c4)                                                     */
/*      SPRUCC (0x200070c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_11_N1_MCCADB_MAIN_REG 0x270270C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca : 18;

                            /* Bits[17:0], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_BUF_11_N1_MCCADB_MAIN_STRUCT;

/* CADB_BUF_12_N0_MCCADB_MAIN_REG supported on:                                 */
/*      SPRA0 (0x200070c8)                                                      */
/*      SPRB0 (0x200070c8)                                                      */
/*      SPRHBM (0x200070c8)                                                     */
/*      SPRC0 (0x200070c8)                                                      */
/*      SPRMCC (0x200070c8)                                                     */
/*      SPRUCC (0x200070c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_12_N0_MCCADB_MAIN_REG 0x270270C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cs : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip select signals.[br] In MRS Mode,
                               the command will be driven to the rank which is
                               programmed with the CS_ACTIVE_POLARITY
                            */
    UINT32 cid : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip ID signal. Only used in memory
                               technologies with stacked dies.
                            */
    UINT32 odt : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM on-die termination signals. Only used
                               in MRS and Always On modes.
                            */
    UINT32 cke : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM clock enable signals. Only used in MRS
                               and Always On modes.
                            */
    UINT32 use_half_ca : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, only the lower half of this
                               buffer's CMD_ADDR entry will be used. Only
                               applies in memory technologies with folded
                               command buses (e.g. LPDDR4).
                            */
    UINT32 val : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               When this field is set to a 1, the CADB will
                               assert a SPID signal (ca_valid) to indicate the
                               PHY that it should drive its output pins during
                               this DRAM command clock.[br] When this field is
                               set to a 0, the CADB will de-assert the ca_valid
                               signal, to indicate to the PHY that it should
                               tristate its output pins during this DRAM
                               command clock.
                            */
    UINT32 par : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to write the parity invert CADB data. When
                               working with DDR4 if this bit is set and the
                               CADB entry is selected, the parity value is
                               inverted to reflect a CA parity error.
                            */
    UINT32 ca : 14;

                            /* Bits[31:18], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */

  } Bits;
  UINT32 Data;

} CADB_BUF_12_N0_MCCADB_MAIN_STRUCT;

/* CADB_BUF_12_N1_MCCADB_MAIN_REG supported on:                                 */
/*      SPRA0 (0x200070cc)                                                      */
/*      SPRB0 (0x200070cc)                                                      */
/*      SPRHBM (0x200070cc)                                                     */
/*      SPRC0 (0x200070cc)                                                      */
/*      SPRMCC (0x200070cc)                                                     */
/*      SPRUCC (0x200070cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_12_N1_MCCADB_MAIN_REG 0x270270CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca : 18;

                            /* Bits[17:0], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_BUF_12_N1_MCCADB_MAIN_STRUCT;

/* CADB_BUF_13_N0_MCCADB_MAIN_REG supported on:                                 */
/*      SPRA0 (0x200070d0)                                                      */
/*      SPRB0 (0x200070d0)                                                      */
/*      SPRHBM (0x200070d0)                                                     */
/*      SPRC0 (0x200070d0)                                                      */
/*      SPRMCC (0x200070d0)                                                     */
/*      SPRUCC (0x200070d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_13_N0_MCCADB_MAIN_REG 0x270270D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cs : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip select signals.[br] In MRS Mode,
                               the command will be driven to the rank which is
                               programmed with the CS_ACTIVE_POLARITY
                            */
    UINT32 cid : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip ID signal. Only used in memory
                               technologies with stacked dies.
                            */
    UINT32 odt : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM on-die termination signals. Only used
                               in MRS and Always On modes.
                            */
    UINT32 cke : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM clock enable signals. Only used in MRS
                               and Always On modes.
                            */
    UINT32 use_half_ca : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, only the lower half of this
                               buffer's CMD_ADDR entry will be used. Only
                               applies in memory technologies with folded
                               command buses (e.g. LPDDR4).
                            */
    UINT32 val : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               When this field is set to a 1, the CADB will
                               assert a SPID signal (ca_valid) to indicate the
                               PHY that it should drive its output pins during
                               this DRAM command clock.[br] When this field is
                               set to a 0, the CADB will de-assert the ca_valid
                               signal, to indicate to the PHY that it should
                               tristate its output pins during this DRAM
                               command clock.
                            */
    UINT32 par : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to write the parity invert CADB data. When
                               working with DDR4 if this bit is set and the
                               CADB entry is selected, the parity value is
                               inverted to reflect a CA parity error.
                            */
    UINT32 ca : 14;

                            /* Bits[31:18], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */

  } Bits;
  UINT32 Data;

} CADB_BUF_13_N0_MCCADB_MAIN_STRUCT;

/* CADB_BUF_13_N1_MCCADB_MAIN_REG supported on:                                 */
/*      SPRA0 (0x200070d4)                                                      */
/*      SPRB0 (0x200070d4)                                                      */
/*      SPRHBM (0x200070d4)                                                     */
/*      SPRC0 (0x200070d4)                                                      */
/*      SPRMCC (0x200070d4)                                                     */
/*      SPRUCC (0x200070d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_13_N1_MCCADB_MAIN_REG 0x270270D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca : 18;

                            /* Bits[17:0], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_BUF_13_N1_MCCADB_MAIN_STRUCT;

/* CADB_BUF_14_N0_MCCADB_MAIN_REG supported on:                                 */
/*      SPRA0 (0x200070d8)                                                      */
/*      SPRB0 (0x200070d8)                                                      */
/*      SPRHBM (0x200070d8)                                                     */
/*      SPRC0 (0x200070d8)                                                      */
/*      SPRMCC (0x200070d8)                                                     */
/*      SPRUCC (0x200070d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_14_N0_MCCADB_MAIN_REG 0x270270D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cs : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip select signals.[br] In MRS Mode,
                               the command will be driven to the rank which is
                               programmed with the CS_ACTIVE_POLARITY
                            */
    UINT32 cid : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip ID signal. Only used in memory
                               technologies with stacked dies.
                            */
    UINT32 odt : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM on-die termination signals. Only used
                               in MRS and Always On modes.
                            */
    UINT32 cke : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM clock enable signals. Only used in MRS
                               and Always On modes.
                            */
    UINT32 use_half_ca : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, only the lower half of this
                               buffer's CMD_ADDR entry will be used. Only
                               applies in memory technologies with folded
                               command buses (e.g. LPDDR4).
                            */
    UINT32 val : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               When this field is set to a 1, the CADB will
                               assert a SPID signal (ca_valid) to indicate the
                               PHY that it should drive its output pins during
                               this DRAM command clock.[br] When this field is
                               set to a 0, the CADB will de-assert the ca_valid
                               signal, to indicate to the PHY that it should
                               tristate its output pins during this DRAM
                               command clock.
                            */
    UINT32 par : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to write the parity invert CADB data. When
                               working with DDR4 if this bit is set and the
                               CADB entry is selected, the parity value is
                               inverted to reflect a CA parity error.
                            */
    UINT32 ca : 14;

                            /* Bits[31:18], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */

  } Bits;
  UINT32 Data;

} CADB_BUF_14_N0_MCCADB_MAIN_STRUCT;

/* CADB_BUF_14_N1_MCCADB_MAIN_REG supported on:                                 */
/*      SPRA0 (0x200070dc)                                                      */
/*      SPRB0 (0x200070dc)                                                      */
/*      SPRHBM (0x200070dc)                                                     */
/*      SPRC0 (0x200070dc)                                                      */
/*      SPRMCC (0x200070dc)                                                     */
/*      SPRUCC (0x200070dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_14_N1_MCCADB_MAIN_REG 0x270270DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca : 18;

                            /* Bits[17:0], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_BUF_14_N1_MCCADB_MAIN_STRUCT;

/* CADB_BUF_15_N0_MCCADB_MAIN_REG supported on:                                 */
/*      SPRA0 (0x200070e0)                                                      */
/*      SPRB0 (0x200070e0)                                                      */
/*      SPRHBM (0x200070e0)                                                     */
/*      SPRC0 (0x200070e0)                                                      */
/*      SPRMCC (0x200070e0)                                                     */
/*      SPRUCC (0x200070e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_15_N0_MCCADB_MAIN_REG 0x270270E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cs : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip select signals.[br] In MRS Mode,
                               the command will be driven to the rank which is
                               programmed with the CS_ACTIVE_POLARITY
                            */
    UINT32 cid : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM chip ID signal. Only used in memory
                               technologies with stacked dies.
                            */
    UINT32 odt : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM on-die termination signals. Only used
                               in MRS and Always On modes.
                            */
    UINT32 cke : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /*
                               The DRAM clock enable signals. Only used in MRS
                               and Always On modes.
                            */
    UINT32 use_half_ca : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, only the lower half of this
                               buffer's CMD_ADDR entry will be used. Only
                               applies in memory technologies with folded
                               command buses (e.g. LPDDR4).
                            */
    UINT32 val : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               When this field is set to a 1, the CADB will
                               assert a SPID signal (ca_valid) to indicate the
                               PHY that it should drive its output pins during
                               this DRAM command clock.[br] When this field is
                               set to a 0, the CADB will de-assert the ca_valid
                               signal, to indicate to the PHY that it should
                               tristate its output pins during this DRAM
                               command clock.
                            */
    UINT32 par : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to write the parity invert CADB data. When
                               working with DDR4 if this bit is set and the
                               CADB entry is selected, the parity value is
                               inverted to reflect a CA parity error.
                            */
    UINT32 ca : 14;

                            /* Bits[31:18], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */

  } Bits;
  UINT32 Data;

} CADB_BUF_15_N0_MCCADB_MAIN_STRUCT;

/* CADB_BUF_15_N1_MCCADB_MAIN_REG supported on:                                 */
/*      SPRA0 (0x200070e4)                                                      */
/*      SPRB0 (0x200070e4)                                                      */
/*      SPRHBM (0x200070e4)                                                     */
/*      SPRC0 (0x200070e4)                                                      */
/*      SPRMCC (0x200070e4)                                                     */
/*      SPRUCC (0x200070e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer.
*/


#define CADB_BUF_15_N1_MCCADB_MAIN_REG 0x270270E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca : 18;

                            /* Bits[17:0], Access Type=RW, default=0x00000000*/

                            /* This field is used to drive the CA bus. */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_BUF_15_N1_MCCADB_MAIN_STRUCT;

/* CADB_AO_MRSCFG_MCCADB_MAIN_REG supported on:                                 */
/*      SPRA0 (0x200070e8)                                                      */
/*      SPRB0 (0x200070e8)                                                      */
/*      SPRHBM (0x200070e8)                                                     */
/*      SPRC0 (0x200070e8)                                                      */
/*      SPRMCC (0x200070e8)                                                     */
/*      SPRUCC (0x200070e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_c/cadb_c_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Pattern Buffer MRS Register.
*/


#define CADB_AO_MRSCFG_MCCADB_MAIN_REG 0x270270E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mrs_gap_scale : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Linear Scale for MRS_GAP. MRS_INTERVAL will
                               be (MRS_GAP+1)[br] 1 - Exponential Scale for
                               MRS_GAP. MRS_INTERVAL will be 2^(MRS_GAP)
                            */
    UINT32 mrs_gap : 4;

                            /* Bits[4:1], Access Type=RW, default=0x00000000*/

                            /*
                               In MRS Mode, this specifies the number of DRAM
                               clocks between MRS commands. This has to be a
                               multiple of the gear ratio (cadb_clk_ratio).[br]
                               In Always On mode, this specifies the number of
                               DRAM clocks between Always On (AO) patterns.[br]
                               Legal values for MRS_GAP are : [br] 0-15 : when
                               MRS_GAP_SCALE is Linear (0).[br] 0-7 : when
                               MRS_GAP_SCALE is Exponential (1).
                            */
    UINT32 rsvd : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mrs_ao_repeats : 16;

                            /* Bits[23:8], Access Type=RW, default=0x00000000*/

                            /*
                               In MRS Mode, the CADB_BUF entries will be driven
                               for MRS_AO_REPEATS times in a linear order using
                               MRS_START_PTR, MRS_GOTO_PTR and MRS_END_PTR.
                               [br] If MRS_AO_REPEATS = 0, then the CADB_BUF
                               entries will be driven once (ie from
                               MRS_START_PTR - MRS_END_PTR). This is equivalent
                               to MRS_AO_REPEATS = 1.[br] In Always On Mode,
                               the CADB_BUF entries will be driven for
                               MRS_AO_REPEATS times in the oder determined by
                               the unisequencers. [br] If MRS_AO_REPEATS = 0,
                               then the CADB_BUF entries will be driven until
                               the STOP_TEST is assrted by the user.
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_AO_MRSCFG_MCCADB_MAIN_STRUCT;

/* CADB_SELCFG_MCCADB_MAIN_REG supported on:                                    */
/*      SPRA0 (0x200070f0)                                                      */
/*      SPRB0 (0x200070f0)                                                      */
/*      SPRHBM (0x200070f0)                                                     */
/*      SPRC0 (0x200070f0)                                                      */
/*      SPRMCC (0x200070f0)                                                     */
/*      SPRUCC (0x200070f0)                                                     */
/* Register default value on SPRA0: 0x10000000                                  */
/* Register default value on SPRB0: 0x10000000                                  */
/* Register default value on SPRHBM: 0x10000000                                 */
/* Register default value on SPRC0: 0x10000000                                  */
/* Register default value on SPRMCC: 0x10000000                                 */
/* Register default value on SPRUCC: 0x10000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_s/cadb_s_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_s/cadb_s_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_s/cadb_s_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_s/cadb_s_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_s/cadb_s_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_s/cadb_s_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Data Buffer Select Features Configuration Register.
*/


#define CADB_SELCFG_MCCADB_MAIN_REG 0x270270F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wr_select_enable : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Drive CADB generated patterns during all WR
                               select cycles[br] Setting Bit 2 - Drive CADB
                               pattern on CA pins.[br] Setting Bit 1 - Drive
                               CADB pattern on Parity pin.[br] Setting Bit 0 -
                               Drive CADB pattern on CS pin.
                            */
    UINT32 rsvd : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rd_select_enable : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Drive CADB generated patterns during all RD
                               select cycles[br] Setting Bit 6 - Drive CADB
                               pattern on CA pins.[br] Setting Bit 5 - Drive
                               CADB pattern on Parity pin.[br] Setting Bit 4 -
                               Drive CADB pattern on CS pin.
                            */
    UINT32 rsvd_11 : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 act_select_enable : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Drive CADB generated patterns during all ACT
                               select cycles[br] Setting Bit 10 - Drive CADB
                               pattern on CA pins.[br] Setting Bit 9 - Drive
                               CADB pattern on Parity pin.[br] Setting Bit 8 -
                               Drive CADB pattern on CS pin.
                            */
    UINT32 rsvd_19 : 5;

                            /* Bits[23:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pre_select_enable : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Drive CADB generated patterns during all PRE
                               select cycles[br] Setting Bit 14 - Drive CADB
                               pattern on CA pins.[br] Setting Bit 13 - Drive
                               CADB pattern on Parity pin.[br] Setting Bit 12 -
                               Drive CADB pattern on CS pin.
                            */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 initial_sel_sseq_en : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Specifies the initial select subsequence enable
                               information that the CADB will use before
                               subsequence information is received from the
                               CPGC. Note that in normal operation modes, this
                               initial state will persist throughout the
                               duration of the test as subsequence enable
                               information passed by the memory controller is
                               ignored.
                            */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_SELCFG_MCCADB_MAIN_STRUCT;

/* CADB_MRSCFG_N0_MCCADB_MAIN_REG supported on:                                 */
/*      SPRA0 (0x200070f8)                                                      */
/*      SPRB0 (0x200070f8)                                                      */
/*      SPRHBM (0x200070f8)                                                     */
/*      SPRC0 (0x200070f8)                                                      */
/*      SPRMCC (0x200070f8)                                                     */
/*      SPRUCC (0x200070f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_s/cadb_s_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_s/cadb_s_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_s/cadb_s_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_s/cadb_s_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_s/cadb_s_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_s/cadb_s_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Pattern Buffer MRS Register.
*/


#define CADB_MRSCFG_N0_MCCADB_MAIN_REG 0x270270F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cs_clocks : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Number of DRAM command clocks the buffer
                               programmed CS value is driven in programmable CS
                               mode. CS_CLOCKS cannot be greater than
                               SETUP_CLOCKS.
                            */
    UINT32 dsel_clocks : 4;

                            /* Bits[6:3], Access Type=RW, default=0x00000000*/

                            /*
                               Number of DRAM command clocks deselect cycles
                               are driven between commands in programmable CS
                               mode. This time is split before and after the
                               MRS commands with ceiling(DSEL_CLOCKS/2) before
                               the command and floor(DSEL_CLOCKS/2) after it.
                            */
    UINT32 setup_clocks : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               Number of DRAM command clocks the command is
                               driven before the capture edge of CS. Default
                               value of "0" will place CS at the middle of the
                               MRS_INTERVAL. SETUP_CLOCKS cannot be greater
                               than MRS_INTERVAL.
                            */
    UINT32 cs_active_polarity : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Inverse of this field's value is driven on chip-
                               select during MRS_INTERVAL when CS is not
                               asserted.
                            */
    UINT32 mrs_start_ptr : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /*
                               Index of the first CADB Buffer entry that gets
                               driven in MRS mode.
                            */
    UINT32 mrs_end_ptr : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /*
                               Index of the last CADB Buffer entry (CADB_BUF)
                               that gets driven in MRS mode. If this is less
                               than MRS_START_PTR, then the CADB engine wraps
                               around the pointer to 0 after the highest index
                               of the CADB_BUF's, then counts back up to the
                               MRS_END_PTR.
                            */
    UINT32 mrs_goto_ptr : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /*
                               Index of the CADB buffer entry (CADB_BUF) that
                               gets driven in MRS after MRS_END_PTR is reached.
                            */
    UINT32 mrs_cs_mode : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /*
                               This field sets the MRS mode of operation
                               encoded as follows:[br] 00 - No CS Override -
                               Drive the value programmed in CADB_BUF as-is for
                               MRS_INTERVAL DRAM command clocks. This includes
                               holding the CS signals at the value specified by
                               the buffer for the entire MRS_INTERVAL.[br] 01 -
                               Programmable CS - Program the SETUP_CLOCKS,
                               CS_CLOCKS, MRS_GAP, DSEL_CLOCKS fields to drive
                               the MRS Command. CS signals are driven to their
                               programmed buffer values for CS_CLOCKS DRAM
                               command clocks. This mode allows more flexible
                               placement and duration of CS assertion during
                               MRS_INTERVAL clocks and also allows for deselect
                               cycles in between consecutive MRS commands.[br]
                               10 - One Cycle CS - Drive the CS signals to
                               their buffer values for 1 DRAM command clock
                               placing them in the middle of the MRS_INTERVAL
                               (rounding up). The inverse of CS_ACTIVE_POLARITY
                               is driven during the remaining clocks.[br] 11 -
                               Two Cycles CS - Drive the CS signals to their
                               buffer values for 2 DRAM command clock placing
                               them in the middle of the MRS_INTERVAL (rounding
                               up). The inverse of CS_ACTIVE_POLARITY is driven
                               during the remaining clocks.
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_MRSCFG_N0_MCCADB_MAIN_STRUCT;

/* CADB_MRSCFG_N1_MCCADB_MAIN_REG supported on:                                 */
/*      SPRA0 (0x200070fc)                                                      */
/*      SPRB0 (0x200070fc)                                                      */
/*      SPRHBM (0x200070fc)                                                     */
/*      SPRC0 (0x200070fc)                                                      */
/*      SPRMCC (0x200070fc)                                                     */
/*      SPRUCC (0x200070fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_s/cadb_s_sub_MEM       */
/* SPRB0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_s/cadb_s_sub_MEM       */
/* SPRHBM Register File:    sprsp_top/mc_cadb_reg[0]/cadb_s/cadb_s_sub_MEM      */
/* SPRC0 Register File:    sprsp_top/mc_cadb_reg[0]/cadb_s/cadb_s_sub_MEM       */
/* SPRMCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_s/cadb_s_sub_MEM      */
/* SPRUCC Register File:    sprsp_top/mc_cadb_reg[0]/cadb_s/cadb_s_sub_MEM      */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRB0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRHBM Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRC0 Security PolicyGroup: CADB_PROTECTED                                   */
/* SPRMCC Security PolicyGroup: CADB_PROTECTED                                  */
/* SPRUCC Security PolicyGroup: CADB_PROTECTED                                  */
/* Cmd/Address Pattern Buffer MRS Register.
*/


#define CADB_MRSCFG_N1_MCCADB_MAIN_REG 0x270270FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mrs_delay_cnt : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               The number of SPID clocks between each loop of
                               the list of MRS commands.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CADB_MRSCFG_N1_MCCADB_MAIN_STRUCT;
#endif /* _MCCADB_MAIN_h */
