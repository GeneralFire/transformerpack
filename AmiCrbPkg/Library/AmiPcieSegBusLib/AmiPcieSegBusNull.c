//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file AmiPcieSegBusNull.c
    Contains the Base library functions to get the segment and bus.
*/

#include <PiDxe.h>

/**

  Return Socket presence state.

  @param[in] SocketId               - Socket number of a multi socket system

  @retval BOOLEAN

**/
BOOLEAN
AmiIsSocketValid (
  IN UINT8          SocketId
  )
{
  return FALSE;
}

/**

  Return Stack presence state.

  @param[in] SocketId             - Socket number of a multi socket system
  @param[in] StackId              - Stack number in a Socket

  @retval BOOLEAN

**/
BOOLEAN
AmiIsStackValid (
  IN UINT8          SocketId,
  IN UINT8          StackId
  )
{
  return FALSE; 
}

/**

  Return the PCI segment and bus number of specified Socket and stack.

  @param[in] SocketId               - Socket number of a multi socket system
  @param[in] StackId                - Stack number in a Socket
  @param[in,out] *Seg               - PCI Segment number
  @param[in,out] *Bus               - PCI Bus number

  @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
AmiGetPcieSegmentBus (
  IN UINT8          SocketId,
  IN UINT8          StackId,
  IN OUT UINT8      *Seg,
  IN OUT UINT8      *Bus
  )
{
  return EFI_UNSUPPORTED;
}

/**

  Return the PCI segment number of specified Socket and stack.

  @param[in] SocketId               - Socket number of a multi socket system
  @param[in,out] *Seg               - PCI Segment number

  @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
AmiGetPcieSegment (
  IN UINT8          SocketId,
  IN OUT UINT8      *Seg
  )
{
  return EFI_UNSUPPORTED;
}
