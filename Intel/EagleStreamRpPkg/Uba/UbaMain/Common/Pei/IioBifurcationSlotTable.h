/** @file
  Contains several definitions for table structures used for Bifurcation and slot configuration
  on the different platforms.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#ifndef _IIOBIFURCATIONSLOTTABLE_H_
#define _IIOBIFURCATIONSLOTTABLE_H_

#include "IioPlatformData.h"

#define QAT_ENABLED                0  // QAT is active-low
#define RISER_PRESENT              0
#define RISER_NOT_PRESENT          1
#define RISER_HP_EN                1
#define RISER_WINGED_IN            0
#define RISER_SLOT9_DISABLE        1

typedef struct {
  UINT8 Socket;
  UINT8 IouNumber;
  UINT8 BroadwayAddress; // 0xff, no override bifurcation settings.
                           // 0-2 BW5 card can be present
} IIO_BROADWAY_ADDRESS_ENTRY;

typedef enum {
  Bw5_Addr_0 = 0,
  Bw5_Addr_1,
  Bw5_Addr_2,
  Bw5_Addr_3,
  Bw5_Addr_Max
} BW5_ADDRESS;

typedef union {
  struct {
    UINT8 PresentSignal0:1;
    UINT8 PresentSignal1:1;
  } Bits;
  UINT8 Data;
} PCIE_RISER_ID;

enum {
  Iio_PortA = 0,
  Iio_PortB = 1,
  Iio_PortC = 2,
  Iio_PortD = 3,
  Iio_PortE = 4,
  Iio_PortF = 5,
  Iio_PortG = 6,
  Iio_PortH = 7
};
typedef enum {
 Iio_Iou0 =0,
 Iio_Iou1,
 Iio_Iou2,
 Iio_Iou3,
 Iio_Iou4,
 Iio_IouMax
} IIO_IOUS;

typedef enum {
  Iio_Socket0 = 0,
  Iio_Socket1,
  Iio_Socket2,
  Iio_Socket3,
  Iio_Socket4,
  Iio_Socket5,
  Iio_Socket6,
  Iio_Socket7
} IIO_SOCKETS;

typedef enum {
  VPP_PORT_0 = 0,
  VPP_PORT_1,
  VPP_PORT_2,
  VPP_PORT_3
} VPP_PORTS;

///
/// Platform Port/Socket assignments.
///

#define ENABLE            1
#define DISABLE           0
#define NO_SLT_IMP        0xFF
#define SLT_IMP           1
#define HIDE              1
#define NOT_HIDE          0
#define VPP_PORT_0        0
#define VPP_PORT_1        1
#define VPP_PORT_MAX      0xFF
#define VPP_ADDR_MAX      0xFF
#define PWR_VAL_MAX       0xFF
#define PWR_SCL_MAX       0xFF

//
// BW5 SMbus slave address
//
#define BW5_SMBUS_ADDRESS                 0x4C
#define PCA9555_COMMAND_CONFIG_PORT0_REG  0x06
#define PCA9555_COMMAND_INPUT_PORT0_REG   0x00
#define NUM_OF_RETRIES                    0x03
#define BW5_0_ID_MASK                     0x0007
#define BW5_1_ID_MASK                     0x0070
#define BW5_2_ID_MASK                     0x0700
#define BW5_3_ID_MASK                     0x7000
#define BW5_CONFIG_REG_MASK               0xFF
#define BW5_CARDS_PRESENT                 0x04
#define BW5_CARD_NOT_PRESENT              0x07


// Bifurcation read from MCU
#define BW5_BIFURCATE_x4x4x4x4  8
#define BW5_BIFURCATE_xxx8x4x4  9
#define BW5_BIFURCATE_x4x4xxx8  10
#define BW5_BIFURCATE_xxx8xxx8  11

#define BW4_DATA_OFFSET         0
#define BW4_CONFIG_OFFSET       3

typedef union {
  struct {
    UINT8 BifBits:3;
    UINT8 ExtPresent:1;
    UINT8 HotPlugEna:1;
    UINT8 Rsvd:2;
    UINT8 ExtId:1;
  } Bits;
  UINT8 Data;
} BW5_BIFURCATION_DATA_STRUCT;

#endif
