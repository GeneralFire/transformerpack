/** @file

  The SPD Access Lib Common header file provides common function definitions
  for the PEI, DXE, and SMM phases of the implmentation of SpdAccessLib.

  This common file provides defines and functions which are universal between
  PEI, DXE, and SMM.  The implementations for preparing the SMBus data and
  selecting the appropriate SPD page offset are common between phases so they
  are described here to mitigate replicated code.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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


#ifndef _SPD_ACCESS_LIB_COMMON_H_
#define _SPD_ACCESS_LIB_COMMON_H_

// Defines
#define SPD_PAGE_ZERO   0x0  // SPD Page 0
#define SPD_PAGE_ONE    0x1  // SPD Page 1

#define SPD_PAGE_INIT_DDR5  0xFFFFFFFF  // Indicates a DDR5 SPD Page has net yet been cached (each page is 4 bits)
#define DDR5_PAGE_BITWIDTH  0x4         // Number of bits each cached page occupies
#define DDR5_SPD_PAGE_MASK  0xF         // Used to keep only relevant page
#define SPD_PAGE_INIT_DDR4  0x3         // Indicates a DDR4 SPD Page has not yet been cached

#define SMB_PAGE_ADDR       0x00  // DDR4 SMBus address used to broadcast the page selection.

#define SPD_EE_PAGE_SELECT_0      6   // Page 0 - lower 256 EEPROM bytes for DDR4
#define SPD_EE_PAGE_SELECT_1      7   // Page 1 - upper 256 EEPROM bytes for DDR4
#define SPD_EE_PAGE_SIZE          256 // Size of 1 SPD page for DDR4
#define SPD_DDR5_PAGE_SIZE        128 // Size of 1 SPD page for DDR5

#define SPD_DDR4_EUP_RANGE  0x180  // End User Programmable offset begins at 384 (0x180)
#define SPD_DDR5_EUP_RANGE  0x280  // End User Programmable offset begins at 640 (0x280)

#define MEM_REG_SHIFT  7  // Bit7 is MemReg bit for DDR5 SPD Hub device

//
// Structure which describes how addresses are conveyed to the SPD5 Hub device (DDR5)
//
typedef union {
  struct {
    UINT8 Address : 7; // Bits[6:0] - Address bits
    UINT8 MemReg  : 1; // Bits[7]   - 1 for NVM location, 0 for internal register
  } Bits;
  UINT8 Data;
} SPD_DDR5_ADDRESS_FIRST_BYTE_STRUCT;

//
// The second byte only used when MR[11] = 1 in I2C mode or in I3C mode
//
typedef union {
  struct {
    UINT8 blk_addr_3_1 : 3; // Bits[2:0] - SpdPage
    UINT8 rnw : 1;          // Bits[3]   - this field only applicable when PEC is enabled in I3C native mode. 0x0: Write 0x1: Read
    UINT8 cmd  : 3;         // Bits[6:4] - this field only applicable when PEC is enabled in I3C native mode
    UINT8 rsvd_7  : 1;      // Bits[7]
  } Bits;
  UINT8 Data;
} SPD_DDR5_ADDRESS_SECOND_BYTE_STRUCT;

#define SMB_MR11_ADDR       0xB   // MR11 address

//
// Structure which describes how data is conveyed to MR11 for page selection (DDR5)
//
typedef union {
  struct {
    UINT8 Page          : 3; // Bits[2:0] - Page to be selected
    UINT8 I2cLegacyMode : 1; // Bits[3]   - I2C Legacy Mode Addressing
    UINT8 Reserved      : 4; // Bits[7:4] - Reserved
  } Bits;
  UINT8 Data;
} SPD_DDR5_MR11_STRUCT;

/**
  Fills the SMB_DEVICE_STRUC with the relevant SMBus address information in preparation
  for an SMBus transaction.

  @param[in] Socket      - Socket ID
  @param[in] Chan        - Channel on socket
  @param[in] Dimm        - DIMM on channel
  @param[out] SpdDev     - Pointer to a SMB_DEVICE structure

  @retval EFI_SUCCESS       SMBUS read was successful
  @retval EFI_DEVICE_ERROR  SMBUS read was unsuccessful
**/
EFI_STATUS
PrepSmbData (
  IN UINT8               Socket,
  IN UINT8               Chan,
  IN UINT8               Dimm,
  OUT SMB_DEVICE_STRUCT  *SpdDev
  );

/**
  Calculate page by dividing the offset by the page size

  @param[in] DramType      - DDR technology
  @param[in] ByteOffset    - SPD Byte Offset to be read

  @retval
**/
UINT8
CalculateSpdPageNum (
  IN UINT16   DramType,
  IN UINT16   ByteOffset
  );

#endif

