/** @file
  Implementation of WheaSciLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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

#include <Library/BaseLib.h>
#include <Library/WheaSciLib.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
//#include <Library/PmcLib.h>
#include <Library/TimerLib.h>
#include <Library/RasDebugLib.h>

#include <Register/PchRegsLpc.h>
#include <Register/PchRegsPmc.h>
#include <Uefi/UefiBaseType.h>
#include <Library/GpioLib.h>
#include <Library/PmcLib.h>
#include <Library/PchCycleDecodingLib.h>



/**

 Trigger a System Common Interrupt(SCI). This hook is invoked at runtime (in SMM) at the end of creating Error data for WHEA OS.
 WheaPlatformSupport driver will call this when correctable or recoverable errors are being reported via WHEA to the OS.

 @param VOID

 @retval EFI_SUCCESS - If programming HW to trigger SCI is success.
 @retval EFI_DEVICE_ERROR - If failed to program the HW.
 @retval EFI_UNSUPPORTED - If OS is not Whea Enabled

 **/
RETURN_STATUS
EFIAPI
SignalWheaSci(
  VOID
  )
{
  PmcSetSwGpeSts ();

  return RETURN_SUCCESS;

}

/**
Generate an NMI

This function generates an NMI  to force an entry to the NMI handler..
Please refer to Platform Controller Hub BIOS Specification: Routing NMI to SMI

@retval None.

**/
RETURN_STATUS
EFIAPI
SignalWheaNmi (
  VOID
  )
{
  UINT16     TcoBaseAddr;
  UINT8      Data;
  UINT8      SaveNmi2SmiEn;
  UINT8      SavePort70;
  UINT16     Data16;

  RAS_DEBUG((LEVEL_FUNC_FLOW,"SignalWheaNmi \n"));
  TcoBaseAddr = 0;

  //
  // Get the TCO Base Address
  // TcoBase was moved to SMBUS device in PCH
  //
  if(EFI_ERROR (PchTcoBaseGet(&TcoBaseAddr))) {
    return RETURN_UNSUPPORTED;
  }
  //
  // Read the NMI2SMI_EN bit, save it for future restore
  //
  SaveNmi2SmiEn = IoRead8 (TcoBaseAddr + R_TCO_IO_TCO1_CNT + 1);
  RAS_DEBUG((LEVEL_FUNC_FLOW,"SignalWheaNmi R_TCO_IO_TCO1_CNT + 1 = 0x%x\n",SaveNmi2SmiEn));

  //
  // Set the NMI2SMI_EN bit to 0
  //
  Data = SaveNmi2SmiEn & 0xfd;
  IoWrite8 (TcoBaseAddr + R_TCO_IO_TCO1_CNT + 1, Data);

  //
  // Enable NMI_EN
  //
  SavePort70 = IoRead8 (0x74);
  Data = SavePort70 & 0x7f;
  IoWrite8 (R_PCH_IO_NMI_EN, Data);

  //
  // Set NMI_NOW = 1
  //
  Data = IoRead8 (TcoBaseAddr + R_TCO_IO_TCO1_CNT + 1);

  Data |= 0x01;
  IoWrite8 (TcoBaseAddr + R_TCO_IO_TCO1_CNT + 1, Data);

  //
  // Clear NMI_NOW = 0 by writing 1 to NMI_NOW bit
  //
  Data = IoRead8 (TcoBaseAddr + R_TCO_IO_TCO1_CNT + 1);
  RAS_DEBUG ((LEVEL_FUNC_FLOW,"SignalWheaNmi R_TCO_IO_TCO1_CNT + 1 = 0x%x\n",Data));

  Data |= 0x01;
  IoWrite8 (TcoBaseAddr + R_TCO_IO_TCO1_CNT + 1, Data);

  //
  // Restore NMI2SMI_EN
  //
  IoWrite8 (TcoBaseAddr + R_TCO_IO_TCO1_CNT + 1, SaveNmi2SmiEn);

  //
  // Clear the CPUSERR_STS/DMISERR_STS bit, bit 12
  // This bit is set to 1 if the CPU complex sends a
  //DMI special cycle message indicating that it wants to cause an SERR#.
  Data16 = IoRead16 (TcoBaseAddr + R_TCO_IO_TCO1_STS);
  RAS_DEBUG ((LEVEL_FUNC_FLOW,"SignalWheaNmi R_TCO_IO_TCO1_STS = 0x%x\n",Data16));
  Data16 |= B_TCO_IO_TCO1_STS_DMISERR;
  IoWrite16(TcoBaseAddr + R_TCO_IO_TCO1_STS, Data16);

  //
  // Clear the NMI2SMI_STS bit if set
  //
  Data16 = IoRead16(TcoBaseAddr + R_TCO_IO_TCO1_STS);
  RAS_DEBUG ((LEVEL_FUNC_FLOW,"SignalWheaNmi R_TCO_IO_TCO1_STS = 0x%x\n",Data16));

  if ((Data16 & 0x0001) != 0) {
    //
    // check port 0x61
    //
    Data = IoRead8 (0x61); //NMI_STS_CNT
    RAS_DEBUG ((LEVEL_FUNC_FLOW,"SignalWheaNmi NMI_STS_CNT 0x61  = 0x%x\n",Data));
    if ((Data & 0x80) != 0) {
      Data = IoRead8 (0x61);
      RAS_DEBUG ((LEVEL_FUNC_FLOW,"SignalWheaNmi NMI_STS_CNT 0x61  = 0x%x\n",Data));
      Data |= 0x04;
      Data &= 0x0f;
      IoWrite8 (0x61, Data);
      Data &= 0x0b;
      IoWrite8 (0x61, Data);
    }
  }
  //
  // Restore NMI_EN
  //
  IoWrite8 (R_PCH_IO_NMI_EN, SavePort70);

  return RETURN_SUCCESS;
}

