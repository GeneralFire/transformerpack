//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file AmiSmbiosLib.c
    SMBIOS Library implementation

*/

#include <Library/AmiSmbiosLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>

#if SMBIOS_SUPPORT && SB_WAKEUP_TYPE_FN
/**

  Detect and return SMBIOS wake-up type

  @param[in] void
  
  @retval       0x01 = Other
                0x02 = Unknown
                0x03 = APM Timer
                0x04 = Modem Ring
                0x05 = LAN Remote
                0x06 = Power Switch
                0x07 = PCI PME#
                0x08 = AC Power Restored

**/

UINT8
getWakeupTypeForSmbios (VOID)
{
    return (PcdGet8(PcdWakeupType));
}
#endif
