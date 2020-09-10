/** @file
  Platform Hooks file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#include <Base.h>
#include <Platform.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Include/SioRegs.h>
#include <RcRegs.h>
#include <Library/PreSiliconEnvDetectLib.h>

UINT8
IsSimPlatform(VOID)
{
  UINT8        i;
  UINT8        EmulationType = 0xff;

  //CSR_EMULATION_FLAG_1_16_5_CFG_REG
  for (i = 0; i < MAX_SOCKET; i++) {
    EmulationType = PciRead8 ( PCI_LIB_ADDRESS((i*0x40 + 0x3F), 16, 5, 0xb0));
    if (EmulationType != 0xff) {
      break;
    }
  }

  if (EmulationType != 0xff) {
    EmulationType &= 7;
  } else {
    EmulationType = 0;
  }

  return EmulationType;
}



STATIC  SIO_INDEX_DATA  mPilotIVTable [] = {
  { PILOTIV_LOGICAL_DEVICE,             PILOTIV_SIO_COM1 },
  { PILOTIV_BASE_ADDRESS_HIGH0,         (UINT8)(SIO_BASE_COM1 >> 8) },
  { PILOTIV_BASE_ADDRESS_LOW0,          (UINT8)(SIO_BASE_COM1 & 0xFF) },
  { PILOTIV_PRIMARY_INTERRUPT_SELECT,   0x04 },
  { PILOTIV_ACTIVATE,                   0x01}
};

STATIC  SIO_INDEX_DATA  mPc8374Table [] = {
  { PC8374_LOGICAL_DEVICE,        PC8374_SIO_COM1 },
  { PC8374_BASE_ADDRESS_HIGH0,    (UINT8)(SIO_BASE_COM1 >> 8) },
  { PC8374_BASE_ADDRESS_LOW0,     (UINT8)(SIO_BASE_COM1 & 0xFF) },
  { WAKEUP_ON_IRQ_EN,             0x04},
  { PC8374_ACTIVATE,              0x01},
  { PC8374_CLOCK_SELECT,          0},
  { PC8374_CLOCK_CONFIG,          0x80}
};


STATIC  SIO_INDEX_DATA  mNCT5104DTable [] = {
  { NCT5104D_LOGICAL_DEVICE,        NCT5104D_SIO_COM1 },
  { NCT5104D_ACTIVATE,              0x00},
  { NCT5104D_BASE_ADDRESS_HIGH0,    (UINT8)(SIO_BASE_COM1 >> 8) },
  { NCT5104D_BASE_ADDRESS_LOW0,     (UINT8)(SIO_BASE_COM1 & 0xFF) },
  { NCT5104D_WAKEUP_ON_IRQ_EN,      0x04},
  { NCT5104D_ACTIVATE,              0x01},
  { NCT5104D_LOGICAL_DEVICE,        NCT5104D_SIO_UARTA },
  { NCT5104D_ACTIVATE,              0x00},
};

/**

    Read Aspeed AHB register.

    @param RegIndex: register index of Aspeed.

    @retval value of register.

**/
UINT32
ReadAHBDword (
  UINT32 RegIndex
  )
{
  UINT8    bValue;
  UINT32   rdValue = 0;

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, REG_LOGICAL_DEVICE);
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (ASPEED2500_SIO_DATA_PORT, ASPEED2500_SIO_SMI);
  IoWrite8 (0xED, 0);//short delay.

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0x30);
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (ASPEED2500_SIO_DATA_PORT, 1);
  IoWrite8 (0xED, 0);//short delay.

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xf8);
  bValue = IoRead8(ASPEED2500_SIO_DATA_PORT);
  bValue &= 0xfc;
  bValue |= 2;  // 4 byte window.
  IoWrite8 (ASPEED2500_SIO_DATA_PORT, bValue);
  IoWrite8 (0xED, 0);//short delay.

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xf0);
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (ASPEED2500_SIO_DATA_PORT,  (UINT8)((RegIndex >> 24)& 0xff));

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xf1);
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (ASPEED2500_SIO_DATA_PORT,  (UINT8)((RegIndex >> 16)& 0xff));

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xf2);
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (ASPEED2500_SIO_DATA_PORT,  (UINT8)((RegIndex >> 8) & 0xff));

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xf3);
  IoWrite8 (0xED, 0);//short delay.
  IoWrite8 (ASPEED2500_SIO_DATA_PORT,  (UINT8)((RegIndex )& 0xff));

  // trigger read
  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xfe);
  IoRead8 (ASPEED2500_SIO_DATA_PORT);


  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xf4);
  rdValue += IoRead8 (ASPEED2500_SIO_DATA_PORT);
  rdValue <<= 8;

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xf5);
  rdValue += IoRead8 (ASPEED2500_SIO_DATA_PORT);
  rdValue <<= 8;

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xf6);
  rdValue += IoRead8 (ASPEED2500_SIO_DATA_PORT);
  rdValue <<= 8;

  IoWrite8 (ASPEED2500_SIO_INDEX_PORT, 0xf7);
  rdValue += IoRead8 (ASPEED2500_SIO_DATA_PORT);


  return rdValue;

}

/**
 * Checks for the presence of ASPEED SIO
 * @return TRUE if its present. FALSE if not.
 */
BOOLEAN
IsAspeedPresent (
  VOID
 )
{
 BOOLEAN PresenceStatus = FALSE;
 UINT32 DeviceID;
#ifdef ESPI_ENABLE
 UINT8  ChipID=0;
#endif
 //
 // IBMC will decode 0x4E/0x4F
 //
 //
 //ASPEED AST2500/AST2600
 //
 IoWrite8 (ASPEED2500_SIO_INDEX_PORT, ASPEED2500_SIO_UNLOCK);
 IoWrite8 (ASPEED2500_SIO_INDEX_PORT, ASPEED2500_SIO_UNLOCK);
 IoWrite8 (ASPEED2500_SIO_INDEX_PORT, REG_LOGICAL_DEVICE);
 IoWrite8 (ASPEED2500_SIO_DATA_PORT, ASPEED2500_SIO_UART1);
 if (IoRead8 (ASPEED2500_SIO_DATA_PORT) == ASPEED2500_SIO_UART1) {
  //
  // In ESPI mode, Aspeed do not support this SIO logic device, assume it always present.
  // Need define ESPI_ENABLE in platform side.
  //
#ifdef ESPI_ENABLE
  if (!IsSimicsEnvironment ()) {
    IoWrite8 (ASPEED2500_SIO_INDEX_PORT, AST2600_CHIP_ID_REG);
    IoWrite8 (ASPEED2500_SIO_DATA_PORT, 0xf0);
    ChipID = IoRead8 (ASPEED2500_SIO_DATA_PORT);
  }
  if ((ChipID == 0) || (ChipID == ASPEED2600_CHIP_ID)) { //Aspeed2500 or Aspeed2600
    DeviceID=0;
    PresenceStatus = TRUE;
  }
#else
  //
  //right now, maybe it is ASPEED. to detect the  device ID.
  //
  DeviceID = ReadAHBDword (SCU7C);
  //
  //There is a Aspeed card need to support as well. it's type is AST2500 A1 EVB.
  //
  //AST2300-A0 0x01000003
  //AST2300-A1 0x01010303
  //AST1300-A1 0x01010003
  //AST1050-A1 0x01010203
  //AST2400-A0 0x02000303
  //AST2400-A1 0x02010303
  //AST1400-A1 0x02010103
  //AST1250-A1 0x02010303
  //AST2500-A0 0x04000303
  //AST2510-A0 0x04000103
  //AST2520-A0 0x04000203
  //AST2530-A0 0x04000403
  //AST2500-A1 0x04010303
  //AST2510-A1 0x04010103
  //AST2520-A1 0x04010203
  //AST2530-A1 0x04010403
  //
  if ((DeviceID & 0xff0000ff) == 0x04000003) {
   PresenceStatus = TRUE;
  }
#endif
 }
 IoWrite8 (ASPEED2500_SIO_INDEX_PORT, ASPEED2500_SIO_LOCK);
 return PresenceStatus;
}

/**
 * Checks for the presence of PILOT 4 SIO
 * @return TRUE if its present. FALSE if not.
 */
BOOLEAN
IsPilot4Present (
  VOID
  )
{
  BOOLEAN PresenceStatus = FALSE;
  //
  //Pilot 4 will decode 0x2E/0x2F
  //
  IoWrite8 (PILOTIV_SIO_INDEX_PORT, PILOTIV_SIO_UNLOCK);
  IoWrite8 (PILOTIV_SIO_INDEX_PORT, PILOTIV_CHIP_ID_REG);
  if (IoRead8 (PILOTIV_SIO_DATA_PORT) == PILOTIV_CHIP_ID) {
    PresenceStatus = TRUE;
  }

  IoWrite8 (PILOTIV_SIO_INDEX_PORT, PILOTIV_SIO_LOCK);
  return PresenceStatus;
}

/**
 * Checks for the presence of PC8374 SIO
 * @return TRUE if its present. FALSE if not.
 */
BOOLEAN
IsPcSioPresent (
  VOID
  )
{
  BOOLEAN PresenceStatus = FALSE;
  //
  // PC SIO will decode 0x2E/0x2F
  //
  IoWrite8 (PC8374_SIO_INDEX_PORT, PC8374_LOGICAL_DEVICE);
  IoWrite8 (PC8374_SIO_DATA_PORT, PC8374_SIO_COM1);
  if (IoRead8 (PC8374_SIO_DATA_PORT) == PC8374_SIO_COM1) {
    IoWrite8 (PC8374_SIO_INDEX_PORT, PC8374_CHIP_ID_REG);
    if (IoRead8 (PC8374_SIO_DATA_PORT) == PC8374_CHIP_ID) {
      PresenceStatus = TRUE;
    }
  }
  return PresenceStatus;
}

/**
 * Checks for the presence of Nuvoton SIO
 * @return TRUE if its present. FALSE if not.
 */
BOOLEAN
IsNuvotonPresent (
  VOID
  )
{
  BOOLEAN PresenceStatus = FALSE;

  //
  // Nuvoton NCT5104D
  //
  IoWrite8 (NCT5104D_SIO_INDEX_PORT, NCT5104D_ENTER_THE_EXTENDED_FUNCTION_MODE);
  IoWrite8 (NCT5104D_SIO_INDEX_PORT, NCT5104D_ENTER_THE_EXTENDED_FUNCTION_MODE);
  IoWrite8 (NCT5104D_SIO_INDEX_PORT, NCT5104D_CHIP_ID_REG);
  if (IoRead8 (NCT5104D_SIO_DATA_PORT) == NCT5104D_CHIP_ID) {
    PresenceStatus = TRUE;
  }
  return PresenceStatus;
}

/**
 * Checks for the presence of the following SIO:
 *  -ASPEED AST2500
 *  -Pilot 4
 *  -PC SIO
 *  -Nuvoton NCT5104D
 *
 * @return An UINT32 with the corresponding bit set for each SIO.
 *         -ASPEED_EXIST     BIT4
 *         -NCT5104D_EXIST   BIT3
 *         -PC8374_EXIST     BIT1
 *         -PILOTIV_EXIST    BIT0
 */
UINT32
IsSioExist (
  VOID
  )
{
  UINT32   SioExit = 0;

  if(IsAspeedPresent ())
  {
    SioExit |= ASPEED_EXIST;
  }

  if(IsPilot4Present ())
  {
    SioExit |= PILOTIV_EXIST;
  }

  if(IsPcSioPresent ())
  {
    SioExit |= PC8374_EXIST;
  }

  if(IsNuvotonPresent ())
  {
    SioExit |= NCT5104D_EXIST;
  }

  DEBUG((DEBUG_INFO, "[SIO] Current system SIO exist bit:%x \n", SioExit));

  return SioExit;
}

