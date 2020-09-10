/** @file
  provide address decode mirror definition.

@copyright
INTEL CONFIDENTIAL
Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef  __ADDRESS_DECODE_MIRROR_H__
#define  __ADDRESS_DECODE_MIRROR_H__

#include <Uefi/UefiBaseType.h>
#include <Library/AddressDecodeLib.h>

#define Pri2SecCa(PriCa)  (PriCa |= BIT19)
#define GetMirrorBitFromCa(Ca)  (BOOLEAN) BitFieldRead64(Ca, 19, 19)
#define MirrorSubCa2SubCa(SubCa) BitFieldRemove64(SubCa, 18, 18)
#define MirrorCa2Ca(Ca) BitFieldRemove64(Ca, 19, 19)

 /**

 Check if mirror failover happend.

 @param[in]    Socket    --   socket index
 @param[in]    Mc        --   MC index
 @param[in]    Ch        --   Channel index

 @retval TRUE/FALSE      -- Return TRUE if failover. Otherwise FALSE.


**/
BOOLEAN
IsMirrorFailover (
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Ch
  );

/**
 This is routine to get primary channel address if mirror

 @param[in]    Sa               --   System address
 @param[in]    Socket           --   socket index
 @param[in]    Mc               --   MC index
 @param[in]    ChannelAddress   --   Channel address

 @ret  return primary channel address if mirror,  return channel address if not mirror, or -1 in error
**/
UINT64
EFIAPI
MirrorPriCa (
  IN      UINT64    Sa,
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT64    ChannelAddress
  );

/**
 This function is called by TranslateMc when ddr act as FM or 1LM
 This function is to get secondary channel and dimm info if mirror

 @param  [in/out] TA  -- pointer to the structure containing DIMM Address

 @retval EFI_SUCCESS / Error code
**/
EFI_STATUS
EFIAPI
TranslateMirror (
  IN OUT PTRANSLATED_ADDRESS    TA
  );

/**
 This function is to move secondary info to primary.

 @param  [in/out] TA  -- pointer to the structure containing DIMM Address

 @retval EFI_SUCCESS / Error code
**/
VOID
EFIAPI
MoveSec2Pri (
  IN OUT PTRANSLATED_ADDRESS    TA
  );
#endif
