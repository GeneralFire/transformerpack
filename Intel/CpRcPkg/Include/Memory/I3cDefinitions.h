/** @file
  I3cDefinitions.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _I3C_DEFINITIONS_H_
#define _I3C_DEFINITIONS_H_

//
// Jedec TS Spec indicates TSOD conversion time is 125ms
// Set slot_interval_count = 8, to get tsod to next_tsod interval of 128ms.
// 8ms for one tsod per sub channel, total 16 tsod for one I3C
//
#define  TSOD_SLOT_INTERVAL_COUNT    8

//
// For SPR SPD polling, both PPCT & PPST are 16 entries deep. The first 8 entries are for TS0 & the next 8
// are for TS1 as shown in the below table. The Slave IDs in the PPCT commands must be programmed by SW appropriately.
//
#define  PPCT_PPST_INDEX_DIMM0_TS0   0
#define  PPCT_PPST_INDEX_DIMM1_TS0   1
#define  PPCT_PPST_INDEX_DIMM2_TS0   2
#define  PPCT_PPST_INDEX_DIMM3_TS0   3
#define  PPCT_PPST_INDEX_DIMM4_TS0   4
#define  PPCT_PPST_INDEX_DIMM5_TS0   5
#define  PPCT_PPST_INDEX_DIMM6_TS0   6
#define  PPCT_PPST_INDEX_DIMM7_TS0   7
#define  PPCT_PPST_INDEX_DIMM0_TS1   8
#define  PPCT_PPST_INDEX_DIMM1_TS1   9
#define  PPCT_PPST_INDEX_DIMM2_TS1   10
#define  PPCT_PPST_INDEX_DIMM3_TS1   11
#define  PPCT_PPST_INDEX_DIMM4_TS1   12
#define  PPCT_PPST_INDEX_DIMM5_TS1   13
#define  PPCT_PPST_INDEX_DIMM6_TS1   14
#define  PPCT_PPST_INDEX_DIMM7_TS1   15

// The convert function should follow the above DIMM*_TS* Index table
#define  CH_DIMM_TS_TO_TSOD(Ch, Dimm, TS) (UINT16) (0x1 << ((TS * I3C_CH_NUMBER * MAX_DIMM) + ((Ch % I3C_CH_NUMBER) * MAX_DIMM) + Dimm))

// For Immediate Transfer, the max data length is 4; For Regular Transfer the max data length is 65535
#define SETHID_DATA_LENGTH          1       // The SPD-hub would replace the 3 bits of the SETHID CCC's payload with its detected HID, known to it through mechanisms like precision resistor.
#define BROADCAST_DATA_LENGTH       0
#define DATA_LEN_I3C_PORT           4    // 4 Bytes per one Data Port
#define BROADCAST_I3C_ADDRESS       0x7E    // The Address in the Address Header is 7'h7E (the I3C Broadcast Address). All I3C Slaves shall match
// Address value 7'h7E. No I2C Slave will match Address 7'h7E, because that value is reserved and unused in I2C

#define MAX_CCC_CODE_BROADCAST      0x80
#define MAX_CCC_CODE_DIRECT         0xFF
#define NO_CCC                      MAX_CCC_CODE_DIRECT   // Send CCC flag

//
// Broadcast CCC(Common Command Code) code
//
#define CCC_BROADCAST_ENEC          0x00  // Enable Events Command    Enable Slave event driven interrupts
#define CCC_BROADCAST_DISEC         0x01  // Disable Events Command   Disable Slave event driven interrupts
#define CCC_BROADCAST_ENTAS0        0x02  // Enter Activity State 0   Set Activity Mode to State 0 (normal operation). Required If: Any other ENTASn CCC is supported.
#define CCC_BROADCAST_ENTAS1        0x03  // Enter Activity State 1   Set Activity State 1
#define CCC_BROADCAST_ENTAS2        0x04  // Enter Activity State 2   Set Activity State 2
#define CCC_BROADCAST_ENTAS3        0x05  // Enter Activity State 3   Set Activity State 3
#define CCC_BROADCAST_RSTDAA        0x06  // Reset Dynamic Address Assignment   Forget current Dynamic Address and wait for new assignment
#define CCC_BROADCAST_ENTDAA        0x07  // Enter Dynamic Address Assignment   Entering Master initiation of Slave Dynamic Address Assignment. Don't participate if the Slave already has an Address assigned.
#define CCC_BROADCAST_DEFSLVS       0x08  // Define List of Slaves    Master defines Dynamic Address, DCR Type, and Static Address (or 0) per Slave
#define CCC_BROADCAST_SETMWL        0x09  // Set Max Write Length     Maximum write length in a single command
#define CCC_BROADCAST_SETMRL        0x0A  // Set Max Read Length      Maximum read length in a single command
#define CCC_BROADCAST_ENTTM         0x0B  // Enter Test Mode          Master has entered Test Mode

#define CCC_BROADCAST_ENTHDR0       0x20  // Enter HDR Mode 0    Master has entered HDR - DDR Mode
#define CCC_BROADCAST_ENTHDR1       0x21  // Enter HDR Mode 1    Master has entered HDR - TSP Mode
#define CCC_BROADCAST_ENTHDR2       0x22  // Enter HDR Mode 2    Master has entered HDR - TSL Mode
#define CCC_BROADCAST_ENTHDR3       0x23  // Enter HDR Mode 3    Master has entered HDR - Future
#define CCC_BROADCAST_ENTHDR4       0x24  // Enter HDR Mode 4    Master has entered HDR - Future
#define CCC_BROADCAST_ENTHDR5       0x25  // Enter HDR Mode 5    Master has entered HDR - Future
#define CCC_BROADCAST_ENTHDR6       0x26  // Enter HDR Mode 6    Master has entered HDR - Future
#define CCC_BROADCAST_ENTHDR7       0x27  // Enter HDR Mode 7    Master has entered HDR - Future
#define CCC_BROADCAST_SETXTIME      0x28  // Exchange Timing Information   Framework for exchanging event timing information
#define CCC_BROADCAST_SETAASA       0x29  // Set Static Address as Dynamic Address    All slaves use their known Static Addresses as their Dynamic Addresses

// JEDEC specific CCC
#define CCC_BROADCAST_SETHID        0x61  // Hub updates 3-bit HID field, updates "T" bit with updated parity calculation for all devices behind hub and stops 3-bit HID translation
#define CCC_BROADCAST_DEVCTRL       0x62  // Configure SPD Hub and all devices behind Hub

//
// Direct CCC code
//
#define CCC_DIRECT_ENEC             0x80  // Enable Events Command    Enable Slave event driven interrupts
#define CCC_DIRECT_DISEC            0x81  // Disable Events Command   Disable Slave event driven interrupts
#define CCC_DIRECT_ENTAS0           0x82  // Enter Activity State 0   Set Activity Mode to State 0 (normal operation).
#define CCC_DIRECT_ENTAS1           0x83  // Enter Activity State 1   Set Activity State 1
#define CCC_DIRECT_ENTAS2           0x84  // Enter Activity State 2   Set Activity State 2
#define CCC_DIRECT_ENTAS3           0x85  // Enter Activity State 3   Set Activity State 3
#define CCC_DIRECT_RSTDAA           0x86  // Reset Dynamic Address Assignment   Forget current Dynamic Address and wait for new assignment
#define CCC_DIRECT_SETDASA          0x87  // Set Dynamic Address from Static Address   Master assigns a Dynamic Address to a Slave with a known Static Address.
#define CCC_DIRECT_SETNEWDA         0x88  // Set New Dynamic Address  Master assigns a new Dynamic Address to any I3C Slave
#define CCC_DIRECT_SETMWL           0x89  // Set Max Write Length     Maximum write length in a single command
#define CCC_DIRECT_SETMRL           0x8A  // Set Max Read Length      Maximum read length in a single command
#define CCC_DIRECT_GETMWL           0x8B  // Get Max Write Length     Get Slave's maximum possible write length
#define CCC_DIRECT_GETMRL           0x8C  // Get Max Read Length      Get Slave's maximum possible read length
#define CCC_DIRECT_GETPID           0x8D  // Get Provisional ID       Get a Slave's Provisional ID
#define CCC_DIRECT_GETBCR           0x8E  // Get Bus Characteristics Register     Get a Device's Bus Characteristic Register (BCR)
#define CCC_DIRECT_GETDCR           0x8F  // Get Device Characteristics Register  Get a Device's Device Characteristics Register (DCR)
#define CCC_DIRECT_GETSTATUS        0x90  // Get Device Status        Get a Device's operating status
#define CCC_DIRECT_GETACCMST        0x91  // Get Accept Mastership    Current Master is requesting and confirming a Bus Mastership from a Secondary Master
#define CCC_DIRECT_MIPI_RESV1       0x92  // MIPI reserved direct CCC code
#define CCC_DIRECT_SETBRGTGT        0x93  // Set Bridge Targets   Master tells Bridge (to/from I2C, SPI, UART, etc.) what endpoints it is talking to (by Dynamic Address and type/ID).
#define CCC_DIRECT_GETMXDS          0x94  // Get Max Data Speed   Master asks Slave for its SDR Mode max. Read and Write data speeds (& optionally max. Read Turnaround time)
#define CCC_DIRECT_GETHDRCAP        0x95  // Get HDR Capability   This CCC is not included in the I3C Basic Specification. To gain access to this capability please contact MIPI Alliance.
#define CCC_DIRECT_MIPI_RESV2       0x96  // MIPI reserved direct CCC code
#define CCC_DIRECT_MIPI_RESV3       0x97  // MIPI reserved direct CCC code
#define CCC_DIRECT_SETXTIME         0x98  // Set Exchange Timing Information  This CCC is not included in the I3C Basic Specification. To gain access to this capability please contact MIPI Alliance.
#define CCC_DIRECT_GETXTIME         0x99  // Get Exchange Timing Information  This CCC is not included in the I3C Basic Specification. To gain access to this capability please contact MIPI Alliance.

// JEDEC specific CCC
#define CCC_DIRECT_DEVCAP           0xE0  // Get Device Capability

#define  I3C_INTR_STATUS_ENABLE_ALL    (UINT32)0x63F
#define  I3C_INTR_SIGNAL_ENABLE_ALL    (UINT32)0x63F

#define  I3C_PERIODIC_POLL_ENABLE_ALL  (UINT32)0xFFFF
#define  I3C_PERIODIC_POLL_DISABLE_ALL  (UINT32)0x0

// SAI bit definitions
#define  HOSTIA_POSTBOOT_SAI    BIT0
#define  HOSTIA_BOOT_SAI        BIT4
#define  MMIO_CONTROL_POLICY_LOW 0x0100021E
#define  MMIO_READ_POLICY_LOW   0x0300021F
#define  MMIO_WRITE_POLICY_LOW  0x0100021E

//
// It takes time for simics I3C module to get the status bit ready. So we cannot use
// "if (GetEmulation () & SIMICS_FLAG) {break;}" to skip status polling in simics
// But after we removed these code, it will TIMEOUT, which is 100000(100ms)
// simics will running slow when detecting dimm configuration
// And simics does not need 100000(100ms) to TIMEOUT, So we use 1000(0.1ms) for simics.
//
#define  I3C_TIMEOUT    ((GetEmulation () & SIMICS_FLAG) ? 1000:100000)     // 0.1 ms for simics; others 100 ms

#define  I3C_MC_NUMBER  2                           // I3C is per 2 MC
#define  I3C_CH_NUMBER  (I3C_MC_NUMBER * MAX_MC_CH) // I3C is per 8 DIMMs, 4 CHs
#define  MAX_TS_PER_DIMM  2                         // 2 TS per DDR5 DIMM
#define  I3C_TS0          0
#define  I3C_TS1          1

#define TWO_BYTE_ADDRESSING  2 // Data length for 2 Byte addressing
#define ONE_BYTE_ADDRESSING  1 // Data length for 1 Byte addressing

// 7-bit slave address is combined with Device Type ID (4 bits) and strapAddress (3 bits).
#define  I3C_STRAP_ADDRESS_OFFSET 0x3

// MR18[5]: INF_SEL Interface Selection
#define I2C_MODE 0 // 0 = I2C Protocol (Max speed of 1 MHz)
#define I3C_MODE 1 // 1 = I3C Basic Protocol

//
// Enable disable
//
#define  I3C_DISABLE  0x0  // disable
#define  I3C_ENABLE   0x1  // enable

//
// Device NACK Retry Count
//
#define  MAX_DEV_NACK_RETRY_CNT 3

//
// Command Attribute field defines Command Type and Bit-Field Format.
//
#define  I3C_COM_ATTR_XFER             0x0  // Regular Transfer
#define  I3C_COM_ATTR_IMMED_DATA_XFER  0x1  // Immediate Data Transfer
#define  I3C_COM_ATTR_ADDR_ASSGN_CMD   0x2  // Address Assignment Command
#define  I3C_COM_ATTR_WWR_COMBO_XFER   0x3  // Write+Write/Read Combo Transfer
#define  I3C_COM_ATTR_INTERNAL_CONTROL 0x7  // Internal Control command

//
// CP Command Present. Indicates whether CMD field is valid for CCC or HDR Transfer
//
#define  CP_TRANFSER  0x0  // 0x0: TRANFSER: Describes SDR transfer. CMD field is not valid.
#define  CP_CCC_HDR   0x1  // 0x1: CCC_HDR: Describes CCC or HDR transfer. CMD field is valid.

//
// rnw: Read no Write field identifies direction of the transfer
//
#define  RNW_WRITE  0x0  // Write transfer
#define  RNW_READ   0x1  // Read transfer

//
// Indicates 8-or 16-bit sub-offset
//
#define  SUBOFFSET_8_BIT   0x0  // 8 bit suboffset
#define  SUBOFFSET_16_BIT  0x1  // 16 bit suboffset

//
// Response on Completion controls whether Response Status is required after successful completion of the
// transfer command. The successful completion shall be read from RESPONSE_QUEUE_PORT register.
// Upon unsuccessful transfer the Response Status shall always be sent.
//
#define  ROC_NOT_REQUIRED  0x0  // Response Status is not required
#define  ROC_REQUIRED      0x1  // Response Status is required

//
//  Terminate on Completion controls whether STOP is issued after completion of the transfer.
//  Note: TOC needs always to be set to 0x1 for ENTDAA, and this bit is meaningful only for SETDASA transfers.
//
#define  TOC_RESTART  0x0  // Repeated Start (Sr) is issued at the end of transfer
#define  TOC_STOP     0x1  // Stop (P) is issued at end of the transfer

//
// Transaction ID field is used as identification tag for the command
// if there are multi-read operations, will still get correct data from queue.
// just that you would not be able to distinguish the responses to each read, so we must use unique TID values.
// it just needs to be unique - incremental or randomn, is up to SW.
//
typedef enum {
  TidPeriodicPoll = 0x0,
  TidRead,
  TidWrite,
  TidCcc,
  IidMax
} TRANSCATION_ID;

//
// Available I3C mode/speed of Combo Transfer Command to be programmed.
//
typedef enum {
  I3cSdr0I2cFm = 0x0,
  //0x0: I3C SDR0/ I2C Fm:
  // Note: I3C SDR0 represents standard SDR Speed (0 to fSCL Max)
  // Max to 12.5MHz; Max to 10MHz for SPR

  I3cSdr1I2cFmPlus,
  // 0x1: I3C SDR1 / I2C Fm+
  // Note: I3C SDR1 represents Sustainable Data Rate of 8 MHz

  I3cSdr2I2cUd1,
  // 0x2: I3C SDR2 / I2C UD (I2C SS)
  // Note: I3C SDR2 represents Sustainable Data Rate of 6 MHz.
  //       I2C UD represents the User Defined I2C Speed (Used for Standard Speed).

  I3cSdr3I2cUd2,
  // 0x3: I3C SDR3/ I2C UD(Reserved)
  // Note: I3C SDR3 represents Sustainable Data Rate of 4 MHz.
  //       I2C UD represents the User Defined I2C Speed (currently It is Reserved for I2C Mode).

  I3cSdr4I2cUd3,
  // 0x4: I3C SDR4/ I2C UD(Reserved)
  // Note: I3C SDR4 represents Sustainable Data Rate of 2 MHz.
  //       I2C UD represents the User Defined I2C Speed (currently It is Reserved for I2C Mode).

  I3cHdrTsI2cRsvd1,
  // 0x5: Reserved (I3C HDR-TS : HDR-Ternary Mode) / I2C Reserved
  // Note: Combo Transfers are supported in only SDR Mode and not in HDR Mode.

  I3cHdrDdrI2cRsvd2,
  // 0x6: Reserved (I3C HDR-DDR : HDR-Double Data Rate Mode) / I2C Reserved
  // Note: Combo Transfers are supported in only SDR Mode and not in HDR Mode.

  I3cClkMax,
} I3C_BUS_MODE;

//
// The command structure of Address Assignment command
//
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT64 com_attr : 3;
    /* Bits[2:0], Access Type=WO, default=0x0*/
    /*
      Command Attribute field defines Command Type and Bit-Field Format.
      Values:
        0x0: XFER: Regular Transfer
        0x1: IMMED_DATA_XFER: Immediate Data Transfer
        0x2: ADDR_ASSGN_CMD: Address Assignment Command
        0x3: WWR_COMBO_XFER: Write+Write/Read Combo Transfer
        0x7: INTERNAL_CONTROL: Internal Control command
      Note: Values 0x4-0x6 are reserved.
    */

    UINT64 tid : 4;
    /* Bits[6:3], Access Type=WO, default=0x0*/
    /*
      Transaction ID field is used as tag for the command
    */

    UINT64 cmd : 8;
    /* Bits[14:7], Access Type=WO, default=0x0*/
    /*
      Transfer Command field specifies CCC code indicating
      whether Address Assignment uses ENTDAA or SETDASA commands.
      The field comprises entire command code (ENTDAA or SETDASA)
    */

    UINT64 rsvd_15 : 1;
    /* Bits[15:15], Access Type=WO, default=0x0*/
    /*

    */

    UINT64 dev_index : 4;
    /* Bits[19:16], Access Type=WO, default=0x0*/
    /*
      Device Index indicates DAT table index where
      information related to Static and Device addressing is
      stored. The controller will start with the information
      from this Index and sequentially increment to the
      next location for assigning the address to subsequent
      devices until the DEV_COUNT number of times.
    */

    UINT64 rsvd_25_20 : 6;
    /* Bits[25:20], Access Type=WO, default=0x0*/
    /*

    */

    UINT64 dev_count : 4;
    /* Bits[29:26], Access Type=WO, default=0x0*/
    /*
      Device Count indicates number of Devices that
      address shall be assigned to.
    */

    UINT64 roc : 1;
    /* Bits[30:30], Access Type=WO, default=0x0*/
    /*
      Response on Completion controls whether Response
      Status is required after successful completion of the
      transfer command. The successful completion shall
      be read from RESPONSE_QUEUE_PORT register.
      Upon unsuccessful transfer the Response Status shall
      always be sent.
      Note: ROC value of 0 is only valid for requests in PIO
      mode. In DMA mode the user has to write always '1'
      to populate Response Descriptor.
      Values:
        0x0: NOT_REQUIRED: Response Status is not required
        0x1: REQUIRED: Response Status is required
    */

    UINT64 toc : 1;
    /* Bits[31:31], Access Type=WO, default=0x0*/
    /*
      Terminate on Completion controls whether STOP is
      issued after completion of the transfer.
      Values:
        0x0: RESTART: Repeated Start (Sr) is issued at the end of transfer
        0x1: STOP: Stop (P) is issued at end of the transfer
      Note: TOC needs always to be set to 0x1 for ENTDAA,
      and this bit is meaningful only for SETDASA transfers.
    */

    UINT64 rsvd_63_32 : 32;
    /* Bits[63:32], Access Type=WO, default=0x0*/
    /*
    */

  } Bits;
  UINT64 Data;

} ADDRESS_ASIGNMENT_COMMAND_SB_I3C_STRUCT;

//
// struct to describe i3c command on bus.
//
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT64 data_transfer_command_low : 32;
    UINT64 data_transfer_command_high : 32;
  } Bits;
  UINT64 Data;

} DATA_TRANSFER_COMMAND;

//
// The high 32 bits structure of Immediate Data Transfer Command with IC_HAS_DAT=0
//
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_15_0 : 16;
    UINT32 data_length : 16;
  } Bits;
  UINT32 Data;

} IMMEDIATE_DATA_TRANSFER_COMMAND_HIGH_WITHOUT_DAT;

//
// The low 32 bits structure of Immediate Data Transfer Command with IC_HAS_DAT=0
//
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 com_attr : 3;
    UINT32 tid : 3;
    UINT32 i2cni3c : 1;
    UINT32 cmd : 8;
    UINT32 cp : 1;
    UINT32 slave_address : 7;
    UINT32 rsvd_25_23 : 3;
    UINT32 mode_speed : 3;
    UINT32 rnw : 1;
    UINT32 roc : 1;
    UINT32 toc : 1;
  } Bits;
  UINT32 Data;

} IMMEDIATE_DATA_TRANSFER_COMMAND_LOW_WITHOUT_DAT;

//
// The high 32 bits structure of Regular Data Transfer Command with IC_HAS_DAT=0
//
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_15_0 : 16;
    UINT32 data_length : 16;
  } Bits;
  UINT32 Data;

} REGULAR_DATA_TRANSFER_COMMAND_HIGH_WITHOUT_DAT;

//
// The low 32 bits structure of Regular Data Transfer Command with IC_HAS_DAT=0
//
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 com_attr : 3;
    UINT32 tid : 3;
    UINT32 i2cni3c : 1;
    UINT32 cmd : 8;
    UINT32 cp : 1;
    UINT32 slave_address : 7;
    UINT32 rsvd_25_23 : 3;
    UINT32 mode_speed : 3;
    UINT32 rnw : 1;
    UINT32 roc : 1;
    UINT32 toc : 1;
  } Bits;
  UINT32 Data;

} REGULAR_DATA_TRANSFER_COMMAND_LOW_WITHOUT_DAT;

//
// The high 32 bits structure of Combo Transfer Command with IC_HAS_DAT=0
//
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 offset_suboffset : 16; // Offset/sub-offset to select offset of target operation
    UINT32 data_length : 16; // Data Length field is used to indicate the length of the transfer in bytes.
  } Bits;
  UINT32 Data;

} COMBO_TRANSFER_COMMAND_HIGH_WITHOUT_DAT;

//
// The low 32 bits structure of Combo Transfer Command with IC_HAS_DAT=0
//
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 com_attr : 3;
    /*
      Command Attribute field defines Command Type and Bit-Field Format.
      Values:
      0x0: XFER: Regular Transfer
      0x1: IMMED_DATA_XFER: Immediate Data Transfer
      0x2: Reserved
      0x3: WWR_COMBO_XFER: Write+Write/Read Combo Transfer
      0x7: INTERNAL_CONTROL: Internal Control command
        Note: Values 0x4-0x6 are reserved
    */
    UINT32 tid : 3;
    /*
      Transaction ID field is used as identification tag for the command.
    */
    UINT32 i2cni3c : 1;
    /*
      This field indicates the addressed slave is a I2C device or I3C device.
      Values:
      0x0: I3C device
      0x1: I2C device
    */
    UINT32 rsvd_15_7 : 9;
    /*
    */
    UINT32 slave_address : 7;
    /*
      This field indicates Slave Device of the addressed
      device. The address could be static (I2C) or dynamic (I3C)
    */
    UINT32 rsvd_24_23 : 2;
    /*
    */
    UINT32 suboffset_16_bit : 1;
    /*
      Indicates 8-or 16-bit sub-offset
      Values:
      0x0: 8_BIT_SUBOFFSET: 8 bit suboffset
      0x1: 16_BIT_SUBOFFSET:16 bit suboffset
    */
    UINT32 mode_speed : 3;
    /*
      Speed and Mode of the Transfer. This field is
      used to program the speed/mode in which I3C or
      I2C are to be initiated by the controller.
      The value of this field is decoded based on
      whether the transfer is targeted to I3C device or
      I2C device as determined by the 'DEVICE' field in
      the Device address table pointed by the
      'DEV_INDEX' field of the command.
      Values:
      0x0: I3C SDR0/ I2C Fm:
         Note: I3C SDR0 represents standard SDR Speed (0 to fSCL Max)
      0x1: I3C SDR1 / I2C Fm+
         Note: I3C SDR1 represents Sustainable Data Rate of 8 MHz
      0x2: I3C SDR2 / I2C UD (I2C SS)
         Note: I3C SDR2 represents Sustainable Data Rate
          of 6 MHz. I2C UD represents the User Defined I2C Speed (Used for Standard Speed).
      0x3: I3C SDR3/ I2C UD(Reserved)
         Note: I3C SDR3 represents Sustainable Data Rate of 4 MHz. I2C UD represents the User Defined I2C
        Speed (currently It is Reserved for I2C Mode).
      0x4: I3C SDR4/ I2C UD(Reserved)
         Note: I3C SDR4 represents Sustainable Data Rate of 2 MHz. I2C UD represents the User Defined I2C
         Speed (currently It is Reserved for I2C Mode).
      0x5: Reserved (I3C HDR-TS : HDR-Ternary Mode) / I2C Reserved
         Note: Combo Transfers are supported in only SDR Mode and not in HDR Mode.
      0x6: Reserved (I3C HDR-DDR : HDR-Double Data Rate Mode) / I2C Reserved
         Note: Combo Transfers are supported in only SDR Mode and not in HDR Mode.
      0x7: RESERVED
    */
    UINT32 rnw : 1;
    /*
      Read no Write field identifies direction of the transfer
      Values:
      0x0: WRITE: Write transfer
      0x1: READ: Read transfer
    */
    UINT32 roc : 1;
    /*
      Response on Completion controls whether
      Response Status is required after successful
      completion of the transfer command. The
      successful completion shall be read from
      RESPONSE_QUEUE_PORT register. Upon
      unsuccessful transfer the Response Status shall
      always be sent.

      Note: ROC value of 0 is only valid for requests in
        PIO mode. In DMA mode the user has to write
        always '1' to populate Response Descriptor.
      Values:
      0x0: NOT_REQUIRED: Response Status is not required
      0x1: REQUIRED: Response Status is required
    */
    UINT32 toc : 1;
    /*
      Terminate on Completion controls whether STOP
      is issued after completion of the transfer.
      Values:
        0x0: RESTART: Repeated Start (Sr) is issued at the end of transfer
      0x1: STOP: Stop (P) is issued at end of the transfer
    */
  } Bits;
  UINT32 Data;

} COMBO_TRANSFER_COMMAND_LOW_WITHOUT_DAT;

#endif /* _I3C_DEFINITIONS_H_ */
