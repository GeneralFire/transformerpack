/**@file

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/
#include <Register/GpioRegs.h>
#include <Register/GpioRegsVer3.h>

//
// PCH GPIO Community address
//
#define PCH_GPIO_COM0 0x006e0000 // PID_GPIOCOM0 = 0x6E
#define PCH_GPIO_COM1 0x006d0000 // PID_GPIOCOM1 = 0x6D
#define PCH_GPIO_COM2 0x006c0000 // PID_GPIOCOM2 = 0x6C
#define PCH_GPIO_COM3 0x006b0000 // PID_GPIOCOM3 = 0x6B
#define PCH_GPIO_COM4 0x006a0000 // PID_GPIOCOM4 = 0x6A
#define PCH_GPIO_COM5 0x00690000 // PID_GPIOCOM5 = 0x69

//
// If in GPIO_GROUP_INFO structure certain register doesn't exist
// it will have value equal to NO_REGISTER_FOR_PROPERTY
//
#define NO_REGISTER_FOR_PROPERTY 0xFFFF

//
// If in GPIO_GROUP_INFO structure certain group should not be used
// by GPIO OS driver then "Gpio base number" field should be set to below value
//
#define GPIO_OS_DRV_NOT_SUPPORTED 0xFFFF

//
// GPIO Library objects
//
Scope(\_SB)
{
  //
  // GPIO information data structure
  //
  // GPCx[GroupIndex][Field]
  //   Field0 - Community
  //   Field1 - Pad number
  //   Field2 - PADCFG register offset
  //   Field3 - HOSTSW_OWN register offset
  //   Field4 - PAD_OWN register offset
  //   Field5 - GPI_GPE_STS register offset
  //   Field6 - Gpio base number for GPIO OS Driver usage
  //            GpioIo and GpioInt number is created in following way:
  //              GpioAcpiPinNumber = GpioHwBankIndex * 32 + GpioPadNumber
  //            where GPIO OS Driver banks are:
  //              GpioHwBank[] ={"GPP_A","GPP_B","SPI","GPP_C","GPP_D","GPP_E","JTAG","GPP_H","GPP_J","GPP_I","GPP_L","GPP_M", "GPP_N", "GPP_O"}
  //
  Name(GPCS, Package(){
    Package(){
      PCH_GPIO_COM0,
      GPIO_VER3_GPIO_GPP_A_PAD_MAX,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_PADCFG_OFFSET,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_HOSTSW_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_PAD_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_GPI_GPE_STS,
      0
    },
    Package(){
      PCH_GPIO_COM0,
      GPIO_VER3_GPIO_GPP_B_PAD_MAX,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_PADCFG_OFFSET,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_HOSTSW_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_PAD_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_GPI_GPE_STS,
      32
    },
    Package(){
      PCH_GPIO_COM0,
      GPIO_VER3_GPIO_SPI_PAD_MAX,
      R_GPIO_VER3_PCH_GPIO_PCR_SPI_PADCFG_OFFSET,
      R_GPIO_VER3_PCH_GPIO_PCR_SPI_HOSTSW_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_SPI_PAD_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_SPI_GPI_GPE_STS,
      64
    },
    Package(){
      PCH_GPIO_COM1,
      GPIO_VER3_GPIO_GPP_C_PAD_MAX,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_PADCFG_OFFSET,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_HOSTSW_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_PAD_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_GPI_GPE_STS,
      96
    },
    Package(){
      PCH_GPIO_COM1,
      GPIO_VER3_GPIO_GPP_D_PAD_MAX,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_PADCFG_OFFSET,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_HOSTSW_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_PAD_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_GPI_GPE_STS,
      128
    },
    Package(){
      PCH_GPIO_COM3,
      GPIO_VER3_GPIO_GPP_E_PAD_MAX,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_PADCFG_OFFSET,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_HOSTSW_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_PAD_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_GPI_GPE_STS,
      192
    },
    Package(){
      PCH_GPIO_COM3,
      GPIO_VER3_GPIO_JTAG_PAD_MAX,
      R_GPIO_VER3_PCH_GPIO_PCR_JTAG_PADCFG_OFFSET,
      R_GPIO_VER3_PCH_GPIO_PCR_JTAG_HOSTSW_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_JTAG_PAD_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_JTAG_GPI_GPE_STS,
      GPIO_OS_DRV_NOT_SUPPORTED
    },
    Package(){
      PCH_GPIO_COM4,
      GPIO_VER3_GPIO_GPP_H_PAD_MAX,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_PADCFG_OFFSET,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_HOSTSW_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_PAD_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_GPI_GPE_STS,
      256
    },
    Package(){
      PCH_GPIO_COM4,
      GPIO_VER3_GPIO_GPP_J_PAD_MAX,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_PADCFG_OFFSET,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_HOSTSW_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_PAD_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_GPI_GPE_STS,
      288
    },
    Package(){
      PCH_GPIO_COM5,
      GPIO_VER3_GPIO_GPP_I_PAD_MAX,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_PADCFG_OFFSET,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_HOSTSW_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_PAD_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_GPI_GPE_STS,
      320
    },
    Package(){
      PCH_GPIO_COM5,
      GPIO_VER3_GPIO_GPP_L_PAD_MAX,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_PADCFG_OFFSET,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_HOSTSW_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_PAD_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_GPI_GPE_STS,
      352
    },
    Package(){
      PCH_GPIO_COM5,
      GPIO_VER3_GPIO_GPP_M_PAD_MAX,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_PADCFG_OFFSET,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_HOSTSW_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_PAD_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_GPI_GPE_STS,
      384
    },
    Package(){
      PCH_GPIO_COM5,
      GPIO_VER3_GPIO_GPP_N_PAD_MAX,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_PADCFG_OFFSET,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_HOSTSW_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_PAD_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_GPI_GPE_STS,
      416
    },
    Package(){
      PCH_GPIO_COM2,
      GPIO_VER3_GPIO_GPP_O_PAD_MAX,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_PADCFG_OFFSET,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_HOSTSW_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_PAD_OWN,
      R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_GPI_GPE_STS,
      160
    },
  })

  //
  // Object for storing RX Level/Edge Configuration for all pads.
  // One fields contains data for one pad.
  //   00b = Level
  //   01b = Edge (RxInv=0 for rising edge; 1 for falling edge)
  //   10b = Disabled
  //   11b = Either rising edge or falling edge
  //
  // Each row has data for one group. Buffer size needs
  // to equal to number of pads
  Name(RXEV, Package(){
    Buffer(GPIO_VER3_GPIO_GPP_A_PAD_MAX){},
    Buffer(GPIO_VER3_GPIO_GPP_B_PAD_MAX){},
    Buffer(GPIO_VER3_GPIO_SPI_PAD_MAX){},
    Buffer(GPIO_VER3_GPIO_GPP_C_PAD_MAX){},
    Buffer(GPIO_VER3_GPIO_GPP_D_PAD_MAX){},
    Buffer(GPIO_VER3_GPIO_GPP_E_PAD_MAX){},
    Buffer(GPIO_VER3_GPIO_JTAG_PAD_MAX){},
    Buffer(GPIO_VER3_GPIO_GPP_H_PAD_MAX){},
    Buffer(GPIO_VER3_GPIO_GPP_J_PAD_MAX){},
    Buffer(GPIO_VER3_GPIO_GPP_I_PAD_MAX){},
    Buffer(GPIO_VER3_GPIO_GPP_L_PAD_MAX){},
    Buffer(GPIO_VER3_GPIO_GPP_M_PAD_MAX){},
    Buffer(GPIO_VER3_GPIO_GPP_N_PAD_MAX){},
    Buffer(GPIO_VER3_GPIO_GPP_O_PAD_MAX){}
  })
}
