/** @file

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

#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/IioAccessLib.h>
#include <Library/EmulationConfigurationLib.h>

/**
  This routine checks if address is in MMIO memory space.
  This test works only for 32-bit addresses (PEI)

  @param Address     - Address to check

  */
VOID
CheckMmioAddress(
  IN  UINTN Address
  )
{
#ifdef IA32
  if ((Address & 0xF0000000) == 0) {
    // MMIO address has to be high
    DEBUG ((DEBUG_ERROR, "\nIio Mmio Reading/Writing: %x ", Address));
    ASSERT(FALSE);
    CpuDeadLoop();
  }
#endif //IA32
}

/**
  Writes 32-bits Memory address memory

  @param Address            - Address that needs to be updated
  @param Data               - Data value
  */
VOID
IioMmioWrite32 (
  IN UINTN  Address,
  IN UINT32 Data
  )
{
  CheckMmioAddress (Address);
  MmioWrite32 (Address, Data);
  if (UbiosGenerationEnabled ()) {
    DEBUG ((DEBUG_ERROR, "\n  mov  DWORD PTR ds:[0%08xh], 0%08xh\n", Address, Data));
  }

#ifndef IA32
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, Address, 1, &Data);
#endif // IA32
}

/**
  Writes 16-bits Memory address memory

  @param Address            - Address that needs to be updated
  @param Data               - Data value
  */
VOID
IioMmioWrite16 (
  IN  UINTN Address,
  IN  UINT16 Data
  )
{
  CheckMmioAddress (Address);
  MmioWrite16 (Address, Data);
  if (UbiosGenerationEnabled ()) {
    DEBUG ((DEBUG_ERROR, "\n  mov  WORD PTR ds:[0%08xh], 0%04xh\n", Address, Data));
  }

#ifndef IA32
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint16, Address, 1, &Data);
#endif // IA32
}

/**
  Writes 8-bits Memory address memory

  @param Address            - Address that needs to be updated
  @param Data               - Data value
  */
VOID
IioMmioWrite8 (
  IN  UINTN Address,
  IN  UINT8 Data
  )
{
  CheckMmioAddress (Address);
  MmioWrite8 (Address, Data);
  if (UbiosGenerationEnabled ()) {
    DEBUG ((DEBUG_ERROR, "\n  mov  BYTE PTR ds:[0%08xh], 0%02xh\n", Address, Data));
  }

#ifndef IA32
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint8, Address, 1, &Data);
#endif // IA32
}

/**
  Reads 32-bits Memory address memory

  @param Address            - Address of requested data

  @return Data              - Requested data
  */
UINT32
IioMmioRead32 (
  IN  UINTN Address
  )
{
  CheckMmioAddress (Address);
  return MmioRead32 (Address);
}

/**
  Reads 16-bits Memory address memory

  @param Address            - Address of requested data

  @return Data              - Requested data
  */
UINT16
IioMmioRead16 (
  IN  UINTN Address
  )
{
  CheckMmioAddress (Address);
  return MmioRead16 (Address);
}

/**
  Reads 8-bits Memory address memory

  @param Address            - Address of requested data

  @return Data              - Requested data
  */
UINT8
IioMmioRead8 (
  IN  UINTN Address
  )
{
  CheckMmioAddress (Address);
  return MmioRead8 (Address);
}
