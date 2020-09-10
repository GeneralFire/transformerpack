/** @file
  Dell designed file (Proposal for Intel to add this for PlatformPkg forward) *****
  Provides the services required to access Fpga board information.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2017 Intel Corporation. <BR>

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

  @par Revision Reference
  This PPI is defined in UEFI Platform Initialization Specification 1.2 Volume 1:
  Pre-EFI Initalization Core Interface.
**/

#ifndef _PEI_FPGA_BOARD_H_
#define _PEI_FPGA_BOARD_H_

#include <Library/GpioLib.h>

///
/// FPGA Hssi Card slot inforamtion on board
///
typedef struct {
  UINT8           BoardId;                  // Board ID
  UINT8           FpgaSktIndex;             // Socket index for the FPGA which have the HSSI card slot
  GPIO_PAD        GpioHssiCardID0;          // Gpio Number of RC_CARD_ID[0]
  GPIO_PAD        GpioHssiCardID1;          // Gpio Number of RC_CARD_ID[1]
  GPIO_PAD        GpioHssiCardID2;          // Gpio Number of RC_CARD_ID[2]
} FPGA_HSSI_DATA;

typedef union {
  struct {
    UINT8 CardID0:1;
    UINT8 CardID1:1;
    UINT8 CardID2:1;
  } Bits;
  UINT8 Data;
} FPGA_HSSI_CARD_ID;

///
/// FPGA Hssi Card slot inforamtion on board
///
typedef struct {
  UINT8           BoardId;                  // Board ID
  UINT8           HssiCardID;             // HSSI card ID
  UINT8           BBStype;                // Blue Bit Stream to be loaded
} FPGA_HSSI_BBS_Map;

///
/// FPGA Blue Bit Stream Type
///
typedef enum {
  BBS_Gbe = 0,
  BBS_Pcie,
  BBS_None    = 0xff
} FPGA_BLUE_BIT_STREAM;


///
/// GPIO table for FPGA socket power off
///
typedef struct {
  UINT8           BoardId;                  // Board ID
  UINT8           FpgaSktIndex;             // Socket index for the FPGA which have the HSSI card slot
  GPIO_PAD        FpgaPowerOffGpio;         // Blue Bit Stream to be loaded
} FPGA_SOCKET_POWEROFF_MAP;


///
/// FPGA Blue Bit Stream Platfrom default map
///
typedef struct {
  UINT8           BoardId;                  // Board ID
  UINT8           FpgaSktIndex;             // Socket index for the FPGA which have the HSSI card slot
  UINT8           Bbstype;                  // Blue Bit Stream to be loaded
} FPGA_PLATFORM_BBS_MAP;

//
// The forward declaration for FPGA_BOARD_PPI.
//
typedef struct _FPGA_BOARD_PPI  FPGA_BOARD_PPI;

/**

    GC_TODO:  Gets FPGA BBS index according to the HSSI card

    @param   Socket Number

    @retval  Bbs Index

**/
typedef
UINT8
(EFIAPI *PEI_GET_FPGA_HSSICARD_BBS_INDEX) (
  UINT8 SocketNum
 );

/**
Routine Description:
  Gets Fpga Power Control GPIO number

Arguments:
  Socket Number

Returns:
  GPIO_PAD number
**/
typedef
EFI_STATUS
(EFIAPI *PEI_SET_FPGA_POWER_GPIO) (
  UINT8 SocketNum
 );

/**

    GC_TODO:  Gets FPGA default BBS index

    @param   Socket Number

    @retval  Bbs Index

**/
typedef
UINT8
(EFIAPI *PEI_GET_DEFAULT_BBS_INDEX) (
   UINT8 SocketNum
   );

struct _FPGA_BOARD_PPI {
  PEI_GET_FPGA_HSSICARD_BBS_INDEX   GetFpgaHssiCardBbsIndex;
  PEI_SET_FPGA_POWER_GPIO           SetFpgaGpiooff;
  PEI_GET_DEFAULT_BBS_INDEX         GetDefaultFpgaBbsIndex;
};

extern EFI_GUID gEfiPeiFpgaBoardPpiGuid;

#endif
