/** @file
  Produce Stall Ppi.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation. <BR>

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

#include <Platform.h>
#include <Ppi/Stall.h>
#include <Library/IoLib.h>
#include <Library/PreSiliconEnvDetectLib.h>


/**

  Waits for at least the given number of microseconds.

  @param PeiServices     General purpose services available to every PEIM.
  @param This            PPI instance structure.
  @param Microseconds    Desired length of time to wait.

  @retval EFI_SUCCESS     If the desired amount of time was passed.

**/
EFI_STATUS
EFIAPI
Stall (
  IN EFI_PEI_SERVICES   **PeiServices,
  IN EFI_PEI_STALL_PPI  *This,
  IN UINTN              Microseconds
  )
{
  UINTN                 Ticks;
  UINTN                 Counts;
  UINT32                CurrentTick;
  UINT32                OriginalTick;
  UINT32                RemainingTick;


  if (Microseconds == 0) {
    return EFI_SUCCESS;
  }

  OriginalTick = IoRead32 (PCH_ACPI_TIMER_ADDRESS);
  OriginalTick &= (PCH_ACPI_TIMER_MAX_VALUE - 1);
  CurrentTick = OriginalTick;

  //
  // The timer frequency is 3.579545MHz, so 1 ms corresponds to 3.58 clocks
  //
  Ticks = Microseconds * 358 / 100 + OriginalTick + 1;

  //
  // The loops needed for timer overflow
  //
  Counts = Ticks / PCH_ACPI_TIMER_MAX_VALUE;

  //
  // Remaining clocks within one loop
  //
  RemainingTick = (UINT32)Ticks % PCH_ACPI_TIMER_MAX_VALUE;

  //
  // Do not intend to use TMROF_STS bit of register PM1_STS, because this add extra
  // one I/O operation, and may generate SMI
  //

  while (Counts != 0) {
    CurrentTick = IoRead32 (PCH_ACPI_TIMER_ADDRESS);
    CurrentTick &= (PCH_ACPI_TIMER_MAX_VALUE - 1);
    if (CurrentTick <= OriginalTick) {
      Counts--;
    }
    OriginalTick = CurrentTick;
  }

  while ((RemainingTick > CurrentTick) && (OriginalTick <= CurrentTick) ) {
    OriginalTick = CurrentTick;
    CurrentTick = IoRead32 (PCH_ACPI_TIMER_ADDRESS);
    CurrentTick &= (PCH_ACPI_TIMER_MAX_VALUE - 1);
  }

  return EFI_SUCCESS;
}
