/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _DDRT2_SPD_REGS_H_
#define _DDRT2_SPD_REGS_H_

#define SPD_CPS_MOD_REV                         0x1

#define SPD_CPS_VENDORID_BYTE0                      512 // Leverage DDR5
#define SPD_CPS_VENDORID_BYTE1                      513 // Leverage DDR5

//
// DDRT2 SPD Spec 0.7 Register Definitions by SpdSpecParser.py tool version 0.88
//

/*  Byte 551 (0x227) Module Revision Code  */

#define SPD_MODULE_REVISION_CODE_REG       0x0227

typedef union {
  struct {
    UINT8 module_type : 4;
                            /* Bits[0:3]
                               Module Type
                               0000 = Reserved
                               0001 = Crow Pass (CPS)
                               0010 = Fleet Peak (FLP) 0100 Reserved
                               1001 = Reserved
                               1010 = Reserved
                               1100 = Reserved
                               1101 = Reserved
                               1110 = Reserved) All others = Reserved
                            */
    UINT8 module_revision : 4;
                            /* Bits[4:7]
                               Module Revision
                               0000 = Revision 0
                               0001 = Revision 1
                               0010 = Revision 2
                               1111 = Revision 15
                            */
  } Bits;
  UINT8 Data;
} MODULE_REVISION_CODE_STRUCT;


/*  Byte 584 (0x0248) CPS Module Product Identifier, LSB  */

#define SPD_CPS_MODULE_PRODUCT_IDENTIFIER_LSB_REG       0x0248

typedef union {
  struct {
    UINT8 cps_module_product_identifier_lsb : 8;
                            /* Bits[0:7]
                               CPS Module Product Identifier LSB
                               0x0000 - 0x5FFF Reserved
                                Crow Pass
                               0x6000-0x6551 SKU1 (128GB)
                               0x6552-0x6AA6 SKU2 (256GB)
                               0x6AA7-0x6FFB SKU3 (512GB)
                               0x6FFC-0x7550 SKU4 (1TB)
                               0x7551-0x7AA5 SKU5 (Spare)
                               0x7AA6-0x7FFF SKU6 (Spare)
                               0x8000-0xFFFF Reserved
                            */
  } Bits;
  UINT8 Data;
} CPS_MODULE_PRODUCT_IDENTIFIER_LSB_STRUCT;

/*  Byte 585 (0x249) CPS Module Product Identifier, MSB  */

#define SPD_CPS_MODULE_PRODUCT_IDENTIFIER_MSB_REG       0x0249

typedef union {
  struct {
    UINT8 cps_module_product_identifier_msb : 7;
                            /* Bits[0:6]
                               CPS Module Product Identifier MSB
                               0x0000 - 0x5FFF Reserved
                                Crow Pass
                               0x6000-0x6551 SKU1 (128GB)
                               0x6552-0x6AA6 SKU2 (256GB)
                               0x6AA7-0x6FFB SKU3 (512GB)
                               0x6FFC-0x7550 SKU4 (1TB)
                               0x7551-0x7AA5 SKU5 (Spare)
                               0x7AA6-0x7FFF SKU6 (Spare)
                               0x8000-0xFFFF Reserved
                            */
    UINT8 reserved_7 : 1;
                            /* Bits[7]
                               reserved
                               0 - no continuation (typical)
                               1- Continuation bit (future growth)
                            */
  } Bits;
  UINT8 Data;
} CPS_MODULE_PRODUCT_IDENTIFIER_MSB_STRUCT;

/*  Byte 586 (0x24A) Non-Volatile Memory Subsystem Controller Revision  */

#define SPD_NON_VOLATILE_MEMORY_SUBSYSTEM_CONTROLLER_REVISION_REG       0x024A

typedef union {
  struct {
    UINT8 cps_subsystem_controller_revision : 8;
                            /* Bits[0:7]
                               CPS Subsystem Controller Revision
                               00 A0 FF - Reserved
                            */
  } Bits;
  UINT8 Data;
} NON_VOLATILE_MEMORY_SUBSYSTEM_CONTROLLER_REVISION_STRUCT;

/*  Byte 587 (0x24B) System Data Channel Byte Connection Ordering  */

#define SPD_SYSTEM_DATA_CHANNEL_BYTE_CONNECTION_ORDERING_REG       0x024B

typedef union {
  struct {
    UINT8 byte_b3 : 1;
                            /* Bits[0]
                               byte_b3
                               System Data Channel Byte Connection Ordering
                            */
    UINT8 byte_cb_b : 1;
                            /* Bits[1]
                               byte_cb_b
                               System Data Channel Byte Connection Ordering
                            */
    UINT8 reserved_2_7 : 6;
                            /* Bits[2:7]
                               reserved
                               System Data Channel Byte Connection Ordering
                            */
  } Bits;
  UINT8 Data;
} SYSTEM_DATA_CHANNEL_BYTE_CONNECTION_ORDERING_STRUCT;

/*  Byte 588 (0x24C) System Data Channel Byte Connection Ordering  */

#define SPD_SYSTEM_DATA_CHANNEL_BYTE_CONNECTION_ORDERING_1_REG       0x024C

typedef union {
  struct {
    UINT8 byte_a0 : 1;
                            /* Bits[0]
                               Byte A0
                               System Data Channel Byte Connection Ordering
                            */
    UINT8 byte_a1 : 1;
                            /* Bits[1]
                               Byte A1
                               System Data Channel Byte Connection Ordering
                            */
    UINT8 byte_a2 : 1;
                            /* Bits[2]
                               Byte A2
                               System Data Channel Byte Connection Ordering
                            */
    UINT8 byte_a3 : 1;
                            /* Bits[3]
                               Byte A3
                               System Data Channel Byte Connection Ordering
                            */
    UINT8 byte_cb_a : 1;
                            /* Bits[4]
                               Byte CB_A
                               System Data Channel Byte Connection Ordering
                            */
    UINT8 byte_b0 : 1;
                            /* Bits[5]
                               Byte B0
                               System Data Channel Byte Connection Ordering
                            */
    UINT8 byte_b1 : 1;
                            /* Bits[6]
                               Byte B1
                               System Data Channel Byte Connection Ordering
                            */
    UINT8 byte_b2 : 1;
                            /* Bits[7]
                               Byte B2
                               System Data Channel Byte Connection Ordering
                            */
  } Bits;
  UINT8 Data;
} SYSTEM_DATA_CHANNEL_BYTE_CONNECTION_ORDERING_1_STRUCT;

/*  Byte 589 (0x024D) NVM Manufacturer ID Code, First Byte  */

#define SPD_NVM_MANUFACTURER_ID_CODE_0_REG       0x024D

typedef union {
  struct {
    UINT8 number_of_continuation_codes : 7;
                            /* Bits[0:6]
                               Number of Continuation Codes
                               0000000
                            */
    UINT8 parity : 1;
                            /* Bits[7]
                               Parity
                               1= Odd Parity for bits 6-0
                            */
  } Bits;
  UINT8 Data;
} NVM_MANUFACTURER_ID_CODE_0_STRUCT;

/*  Byte 591 (0x24F) NVM Device and Stepping  */

#define SPD_NVM_DEVICE_AND_STEPPING_REG       0x024F

typedef union {
  struct {
    UINT8 minor_revision_code : 3;
                            /* Bits[0:2]
                               Minor Revision Code
                               000 = Major Code.0
                               001 = MajorCode.1
                               010 = MajorCode.2
                               011 = MajorCode.3
                               100 = MajorCode.4
                               101 = MajorCode.5
                               110 = MajorCode.6
                               111 = MajorCode.7
                            */
    UINT8 major_revision_code : 3;
                            /* Bits[3:5]
                               Major Revision Code
                               000 = A step
                               001 = B step
                               010 = C step
                               011 = D step
                               100 = E step
                               101 = F step
                               110 = G step
                               111 = H step
                            */
    UINT8 generation : 2;
                            /* Bits[6:7]
                               Generation
                               00 = S25D, NVM Gen 2
                               01 = S26, NVM Gen 2
                               10 = ALF, NVM Gen 2
                               11 = ATF, NVM Gen 2
                            */
  } Bits;
  UINT8 Data;
} NVM_DEVICE_AND_STEPPING_STRUCT;

/*  Byte 592 (0x250) Memory Device Density  */

#define SPD_MEMORY_DEVICE_DENSITY_REG       0x0250

typedef union {
  struct {
    UINT8 individual_nvm_die_density : 4;
                            /* Bits[0:3]
                               Individual NVM Die Density
                               0000 = 256 Mbit (32 MB)
                               0001 = 512 Mbit(64 MB)
                               0010 = 1 Gbit (128 MB)
                               0011 = 2 Gbit (256 MB)
                               0100 = 4 Gbit (512 MB)
                               0101 = 8 Gbit (1 GB)
                               0110 = 16 Gbit (2 GB)
                               0111 = 32 Gbit (4 GB)
                               1000 = 64 Gbit (8 GB)
                               1001 = 128 Gbit (16GB)
                               1010 = 256 Gbit (32GB)
                               1011 = 512 Gbit (64GB)
                               1100 = 1 Tbit (128GB)
                               1101 = 2 Tbit (256 GB)
                               1110 = 4 Tbit (512 GB)
                               1111 = 8 Tbit (1TB)
                            */
    UINT8 reserved_4_5 : 2;
                            /* Bits[4:5]
                               RESERVED
                               00 = RESERVED
                            */
    UINT8 reserved_6_7 : 2;
                            /* Bits[6:7]
                               RESERVED
                               00 = RESERVED
                            */
  } Bits;
  UINT8 Data;
} MEMORY_DEVICE_DENSITY_STRUCT;

/*  Byte 594 (0x252) NVM Package Type and Loading  */

#define SPD_NVM_PACKAGE_TYPE_AND_LOADING_REG       0x0252

typedef union {
  struct {
    UINT8 dq_loading_per_nvm_port : 2;
                            /* Bits[0:1]
                               DQ Loading Per NVM Port
                               00 = RESERVED
                               01 = Multi Load Stack
                               10 = Single Load Stack
                               11 = RESERVED
                            */
    UINT8 independent_ports_per_nvm_package : 2;
                            /* Bits[2:3]
                               Independent Ports Per NVM Package
                               00 = 1
                               01 = 2 1x = RESERVED
                            */
    UINT8 package_die_count : 3;
                            /* Bits[4:6]
                               Package Die Count
                               000 = 1
                               001 = 2
                               110 = 7
                               111 = 8
                            */
    UINT8 package_type : 1;
                            /* Bits[7]
                               Package Type
                               0 = Mono Die
                               1 = Multi Die
                            */
  } Bits;
  UINT8 Data;
} NVM_PACKAGE_TYPE_AND_LOADING_STRUCT;

/*  Byte 596 (0x254) CPS Module Organization  */

#define SPD_CPS_MODULE_ORGANIZATION_REG       0x0254

typedef union {
  struct {
    UINT8 nvm_die_width : 3;
                            /* Bits[0:2]
                               NVM Die Width
                               000 = 4 bits
                               001 = 8 bits
                               010 = 16 bits
                            */
    UINT8 nvm_active_packages_rank : 3;
                            /* Bits[3:5]
                               NVM Active Packages/Rank
                               000 = 4
                               001 = 5
                            */
    UINT8 spare_nvm_rank : 1;
                            /* Bits[6]
                               Spare NVM/Rank
                               0 = No
                               1 = Yes
                            */
    UINT8 nvm_channels_per_dimm : 1;
                            /* Bits[7]
                               NVM Channels per DIMM
                               0 = 1 channel
                               1 = 2 channels
                            */
  } Bits;
  UINT8 Data;
} CPS_MODULE_ORGANIZATION_STRUCT;

/*  Byte 597 (0x255) CPS Module NVM Memory Bus Width  */

#define SPD_CPS_MODULE_NVM_MEMORY_BUS_WIDTH_REG       0x0255

typedef union {
  struct {
    UINT8 effective_nvm_channel_data_width : 3;
                            /* Bits[0:2]
                               Effective NVM Channel Data Width
                               000 = 8 bits
                               001 = 16 bits
                               010 = 32 bits
                               011 = 64 bits 100
                               111 = RESERVED
                            */
    UINT8 effective_nvm_channel_extension_width : 3;
                            /* Bits[3:5]
                               Effective NVM Channel Extension Width
                               000 = 0 bits (no extension)
                               001 = 8 bits
                               010 = 16 bits New Code
                               011 = 24 bits New Code
                               100 = 32 bits New Code 101
                               111 = RESERVED
                            */
    UINT8 nvm_channel_spare_width : 2;
                            /* Bits[6:7]
                               NVM Channel SPARE Width
                               00 = 0 bits (no SPARE)
                               01 = 8 bits
                               10 = 16 bits
                               11 = RESERVED
                            */
  } Bits;
  UINT8 Data;
} CPS_MODULE_NVM_MEMORY_BUS_WIDTH_STRUCT;

/*  Byte 598 (0x256) Module Thermal Sensor Accuracy  */

#define SPD_MODULE_THERMAL_SENSOR_ACCURACY_REG       0x0256

typedef union {
  struct {
    UINT8 reserved_0_3 : 4;
                            /* Bits[0:3]
                               RESERVED
                               0000
                            */
    UINT8 ts_accuracy : 3;
                            /* Bits[4:6]
                               Ts Accuracy
                               000 = C
                               001 = 1C
                               010 = 2C
                               011 = 3C
                               100 = 4C
                               101 = 5C 110
                               111 = RESERVED
                            */
    UINT8 thermal_sensor : 1;
                            /* Bits[7]
                               Thermal Sensor
                               0 = No Ts Accessible via SPD Read
                               1 = Ts Accessible via SPD Read
                            */
  } Bits;
  UINT8 Data;
} MODULE_THERMAL_SENSOR_ACCURACY_STRUCT;

/*  Byte 599 (0x257) Module Thermal Solution  */

#define SPD_MODULE_THERMAL_SOLUTION_REG       0x0257

typedef union {
  struct {
    UINT8 thermal_solution : 4;
                            /* Bits[0:3]
                               Thermal Solution
                               0000 = No thermal solution
                               0001 = Type 1
                               0010 = Type 2
                               0011 = Type 3
                               0100 = Type 4
                            */
    UINT8 reserved_0_7 : 8;
                            /* Bits[0:7]
                               RESERVED
                               0000 = RESERVED
                            */
  } Bits;
  UINT8 Data;
} MODULE_THERMAL_SOLUTION_STRUCT;

/*  Byte 602 (0x25A) Module Subsystem Controller Product Identifier, LSB  */

#define SPD_MODULE_SUBSYSTEM_CONTROLLER_PRODUCT_IDENTIFIER_LSB_REG       0x025A

typedef union {
  struct {
    UINT8 module_subsystem_controller_product_identifier_lsb : 8;
                            /* Bits[0:7]
                               Byte 602 Module Subsystem Controller Product Identifier, LSB
                               FNV = 0x79
                               EKV = 0x7A
                               BWV = 0x7B
                               CWV = 0x7C
                            */
  } Bits;
  UINT8 Data;
} MODULE_SUBSYSTEM_CONTROLLER_PRODUCT_IDENTIFIER_LSB_STRUCT;

/*  Byte 603 (0x25B) Module Subsystem Controller Product Identifier, MSB  */

#define SPD_MODULE_SUBSYSTEM_CONTROLLER_PRODUCT_IDENTIFIER_MSB_REG       0x025B

typedef union {
  struct {
    UINT8 module_subsystem_controller_product_identifier_msb : 8;
                            /* Bits[0:7]
                               Byte 603 Module Subsystem Controller Product Identifier, MSB
                               0x09 - All DIMM Persistent Memory Modules
                            */
  } Bits;
  UINT8 Data;
} MODULE_SUBSYSTEM_CONTROLLER_PRODUCT_IDENTIFIER_MSB_STRUCT;

/*  Byte 604 (0x25C) Number of Write Credits CWV Memory Controller Can Handle  */

#define SPD_NUMBER_OF_WRITE_CREDITS_CWV_MEMORY_CONTROLLER_CAN_HANDLE_REG       0x025C

typedef union {
  struct {
    UINT8 number_of_write_credits_cwv_can_handle : 7;
                            /* Bits[0:6]
                               Number of Write Credits CWV can Handle
                               0000000 (0x00) = Reserved
                               0000001 (0x01) = 1 credit
                               0000010 (0x02) = 2 credits
                               1111110 (0x7E) = 126 credits
                               1111111 (0x7F) = 127 credits
                            */
    UINT8 reserved_7 : 1;
                            /* Bits[7]
                               reserved
                               0 = RESERVED
                            */
  } Bits;
  UINT8 Data;
} NUMBER_OF_WRITE_CREDITS_CWV_MEMORY_CONTROLLER_CAN_HANDLE_STRUCT;

/*  Byte 605 (0x25D) Number of Read Credits CWV Memory Controller Can Handle  */

#define SPD_NUMBER_OF_READ_CREDITS_CWV_MEMORY_CONTROLLER_CAN_HANDLE_REG       0x025D

typedef union {
  struct {
    UINT8 number_of_write_credits_cwv_can_handle : 7;
                            /* Bits[0:6]
                               Number of Write Credits CWV can Handle
                               0000000 (0x00) = Reserved
                               0000001 (0x01) = 1 credit
                               0000010 (0x02) = 2 credits
                               1111110 (0x7E) = 126 credits
                               1111111 (0x7F) = 127 credits
                            */
    UINT8 reserved_7 : 1;
                            /* Bits[7]
                               reserved
                               0 = RESERVED
                            */
  } Bits;
  UINT8 Data;
} NUMBER_OF_READ_CREDITS_CWV_MEMORY_CONTROLLER_CAN_HANDLE_STRUCT;

/*  Byte 606 (0x25E) Memory Subsystem Controller Manufacturers ID Code, First Byte  */

#define SPD_MEMORY_SUBSYSTEM_CONTROLLER_MANUFACTURERS_ID_CODE_0_REG       0x025E

typedef union {
  struct {
    UINT8 number_of_continuation_codes : 7;
                            /* Bits[0:6]
                               Number of Continuation Codes
                               0000000
                            */
    UINT8 parity : 1;
                            /* Bits[7]
                               Parity
                               1= Odd Parity for bits 6-0
                            */
  } Bits;
  UINT8 Data;
} MEMORY_SUBSYSTEM_CONTROLLER_MANUFACTURERS_ID_CODE_0_STRUCT;

/*  Byte 607 (0x25F) Memory Subsystem Controller Manufacturers ID Code, Second Byte  */

#define SPD_MEMORY_SUBSYSTEM_CONTROLLER_MANUFACTURERS_ID_CODE_1_REG       0x025F

typedef union {
  struct {
    UINT8 last_non_zero_byte_nvm_device_manufacturer_id : 8;
                            /* Bits[0:7]
                               Last Non-Zero Byte NVM Device Manufacturer ID
                               1000 1001
                            */
  } Bits;
  UINT8 Data;
} MEMORY_SUBSYSTEM_CONTROLLER_MANUFACTURERS_ID_CODE_1_STRUCT;


#endif  // #ifndef _DDRT2_SPD_REGS_H_
