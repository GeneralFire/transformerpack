/** @file
  CmosAccessLib implementation.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/CmosAccessLib.h>
#include <Library/PlatformCmosAccessLib.h>


// CMOS access Port address

#define PORT_70            0x70
#define PORT_71            0x71
#define PORT_72            0x72
#define PORT_73            0x73

#define CMOS_BANK0_LIMIT   0x7F

/**
  Read a byte value from a CMOS address.

  @param [in] Address   Location to read from CMOS

  @return The byte value read from the CMOS address.
**/
UINT8
EFIAPI
CmosRead8 (
  IN  UINT8 Address
  )
{
  if (Address <= CMOS_BANK0_LIMIT) {
    if (PlatformCmosGetNmiState ()) {
      Address |= BIT7;
    }
    IoWrite8 (PORT_70, Address);
    return IoRead8 (PORT_71);
  } else {
    IoWrite8 (PORT_72, Address);
    return IoRead8 (PORT_73);
  }
}

/**
  Write a byte value to a CMOS address.

  @param [in] Address Location to write to CMOS.
  @param [in] Data    The byte value write to the CMOS address.
**/
VOID
EFIAPI
CmosWrite8 (
  IN UINT8 Address,
  IN UINT8 Data
  )
{
    // CMOS 0-9, A, B, C, D are for RTC.
    if (Address <= 0xD) {
    return;
    }
    
    if (Address <= CMOS_BANK0_LIMIT) {
      if (PlatformCmosGetNmiState ()) {
        Address |= BIT7;
      }
      IoWrite8 (PORT_70, Address);
      IoWrite8 (PORT_71, Data);
    } else {
      IoWrite8 (PORT_72, Address);
      IoWrite8 (PORT_73, Data);
    }
}

/**
  Read a word value from a CMOS address.

  @param [in] Address   Location to read from CMOS

  @return The word value read from the CMOS address.
**/
UINT16
EFIAPI
CmosRead16 (
  IN  UINT8  Address
  )
{
  return CmosRead8 (Address) + (CmosRead8 (Address + 1) << 8);
}

/**
  Write a word value to a CMOS address.

  @param [in] Address Location to write to CMOS.
  @param [in] Data    The word value write to the CMOS address.
**/
VOID
EFIAPI
CmosWrite16 (
  IN UINT8  Address,
  IN UINT16 Data
  )
{
  CmosWrite8 (Address, (UINT8) Data);
  CmosWrite8 (Address + 1, (UINT8) (Data >> 8));
}

/**
  Read a dword value from a CMOS address.

  @param [in] Address   Location to read from CMOS

  @return The dword value read from the CMOS address.
**/
UINT32
EFIAPI
CmosRead32 (
  IN  UINT8  Address
  )
{
  return CmosRead16 (Address) + (CmosRead16 (Address + 2) << 16);
}

/**
  Write a dword value to a CMOS address.

  @param [in] Address Location to write to CMOS.
  @param [in] Data    The dword value write to the CMOS address.
**/
VOID
EFIAPI
CmosWrite32 (
  IN UINT8  Address,
  IN UINT32 Data
  )
{
  CmosWrite16 (Address, (UINT16) Data);
  CmosWrite16 (Address + 2, (UINT16) (Data >> 16));
}
