/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <Library/MemTypeLib.h>
#include "MemTypeLibInternal.h"
#include <Library/MemRcLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/SystemInfoLib.h>
#include <Include/MemMapHost.h>

/**

  Check if the input MemType is classified as "Reserved"

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType is classified as "Reserved"
  @retval FALSE - The input MemType is not classified as "Reserved"

**/
BOOLEAN
IsMemTypeReserved (
  MEM_TYPE MemType
  )
{
  switch (MemType) {
  case MemType1lmAppDirectReserved:
    return TRUE;
  default:
    return FALSE;
  }
}

/**

  Check if the input MemType is classified as "AppDirect"

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType is classified as "AppDirect"
  @retval FALSE - The input MemType is not classified as "AppDirect"

**/
BOOLEAN
IsMemTypeAppDirect (
  MEM_TYPE MemType
  )
{
  switch (MemType) {
  case MemType1lmAppDirect:
  case MemType1lmAppDirectReserved:
  case MemType2lmDdrWbCacheAppDirect:
    return TRUE;
  default:
    return FALSE;
  }
}

/**

  Check if the input MemType is classified as "2lm DDR"

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType is classified as "2lm DDR"
  @retval FALSE - The input MemType is not classified as "2lm DDR"

**/
BOOLEAN
IsMemType2lmDdr     (
  MEM_TYPE MemType
  )
{
  switch (MemType) {
  case MemType2lmDdrCacheMemoryMode:
  case MemType2lmDdrWbCacheAppDirect:
    return TRUE;
  default:
    return FALSE;
  }
}

/**

  Check if the input MemType is classified as "2lm"

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType is classified as "2lm"
  @retval FALSE - The input MemType is not classified as "2lm"

**/
BOOLEAN
IsMemType2lm (
  MEM_TYPE MemType
  )
{
  switch (MemType) {
  case MemType2lmDdrCacheMemoryMode:
  case MemType2lmDdrWbCacheAppDirect:
  case MemType2lmHbmCacheDdr:
  case MemType2lmHbmCacheMemoryMode:
    return TRUE;
  default:
    return FALSE;
  }
}

/**

  Check if the input MemType is classified as "Hbm"

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType is classified as "Hbm"
  @retval FALSE - The input MemType is not classified as "Hbm"

**/
BOOLEAN
IsMemTypeHbm (
  MEM_TYPE MemType
  )
{
  switch (MemType) {
  case MemType1lmHbm:
  case MemType2lmHbmCacheDdr:
  case MemType2lmHbmCacheMemoryMode:
    return TRUE;
  default:
    return FALSE;
  }
}

/**

  Check if the input MemType is classified as "2lm HBM"

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType is classified as "2lm HBM"
  @retval FALSE - The input MemType is not classified as "2lm HBM"

**/
BOOLEAN
IsMemType2lmHbm (
  MEM_TYPE MemType
  )
{
  switch (MemType) {
  case MemType2lmHbmCacheDdr:
  case MemType2lmHbmCacheMemoryMode:
    return TRUE;
  default:
    return FALSE;
  }
}

/**

  Check if the input MemType is classified as "1lm"

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType is classified as "1lm"
  @retval FALSE - The input MemType is not classified as "1lm"

**/
BOOLEAN
IsMemType1lm (
  MEM_TYPE MemType
  )
{
  switch (MemType) {
  case MemType1lmDdr:
  case MemType1lmHbm:
    return TRUE;
  default:
    return FALSE;
  }
}

/**

  Check if the input MemType involves DDR Volatile Memory or DDRT Crystal Ridge media
  configured for volatile MemoryMode access.

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType involves DDRT "MemoryMode" or DDR Volatile memory
  @retval FALSE - The input MemType does not involve DDRT "MemoryMode" or DDR Volatile memory

**/
BOOLEAN
IsMemTypeVolatile (
  MEM_TYPE MemType
  )
{
  return (IsMemType1lm(MemType) || IsMemType2lm(MemType));
}

/**

  Check if the input MemType involves DDR volatile memory.

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType involves DDR memory type.
  @retval FALSE - The input MemType does not involve DDR memory type.

**/
BOOLEAN
IsMemTypeDdr (
  MEM_TYPE MemType
  )
{
  switch (MemType) {
  case MemType1lmDdr:
  case MemType2lmDdrCacheMemoryMode:
  case MemType2lmHbmCacheDdr:
  case MemType2lmDdrWbCacheAppDirect:
    return TRUE;
  default:
    return FALSE;
  }
}

/**

  Check if the input MemType involves DDRT Crystal Ridge media.

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType involves DDRT memory type.
  @retval FALSE - The input MemType does not involve DDRT memory type.

**/
BOOLEAN
IsMemTypeDdrt (
  MEM_TYPE MemType
  )
{
  return (IsMemTypeAppDirect(MemType) || IsMemType2lm(MemType));
}

/**

  Check if the input MemType involves CXL memory device.

  @param[in]  MemType - The MemType enum to check

  @retval TRUE  - The input MemType involves CXL memory device.
  @retval FALSE - The input MemType does not involve CXL memory device.

**/
BOOLEAN
IsMemTypeCxl (
  MEM_TYPE MemType
  )
{
  switch (MemType) {
  case MemTypeCxlAccVolatileMem:
  case MemTypeCxlAccPersistentMem:
  case MemTypeCxlExpVolatileMem:
  case MemTypeCxlExpPersistentMem:
    return TRUE;
  default:
    return FALSE;
  }
}

/**

  Get a list of the memory types present in the input channel.

  @param[in]  SocketId           Socket index.
  @param[in]  ChId               Channel index within socket.
  @param[out] MemTypesPresent    Pointer to an array that will be populated with the list of memory types present in the channel.

**/
VOID
GetChMemTypeList (
  IN  UINT8       SocketId,
  IN  UINT8       ChId,
  OUT BOOLEAN     (*MemTypesPresent)[MemTypeMax]
  )
{
  MEMORY_MAP_HOST *MemMapHost;
  UINT8           McId;
  UINT8           MaxChPerMc;
  UINT8           ChIdPerMc;
  UINT8           ClusterId;
  UINT8           SadId;
  UINT8           BeginSadId;
  UINT8           EndSadId;
  SAD_TABLE       *SadEntry;

  if (MemTypesPresent == NULL) {
    return;
  }

  ZeroMem (MemTypesPresent, sizeof (*MemTypesPresent));

  MemMapHost = GetMemMapHostPointer ();
  if (MemMapHost == NULL) {
    return;
  }

  MaxChPerMc = GetNumChannelPerMc ();
  McId = ChId / MaxChPerMc;
  ChIdPerMc = ChId % MaxChPerMc;

  for (ClusterId = 0; ClusterId < MemMapHost->NumberofChaDramClusters; ClusterId++) {
    BeginSadId = ClusterId * MemMapHost->MaxSadRules;
    EndSadId = BeginSadId + MemMapHost->MaxSadRules;

    SadEntry = GetSadRuleEntry (SocketId, BeginSadId);
    if (SadEntry->Enable == 0) {
      break;  // Break if there is no more available SAD entry.
    }

    for (SadId = BeginSadId; SadId < EndSadId; SadId++) {
      SadEntry = GetSadRuleEntry (SocketId, SadId);
      if (SadEntry->Enable == 0) {
        break;  // Break if there is no more available SAD entry.
      }

      if (SadEntry->local == 0) {
        continue;  // Skip if this is not a local SAD entry.
      }

      //
      // Check off the memory types present in the SAD entry the channel participates in
      //
      if (((SadEntry->channelInterBitmap[McId] & (BIT0 << ChIdPerMc)) != 0) ||
          ((SadEntry->FMchannelInterBitmap[McId] & (BIT0 << ChIdPerMc)) != 0)) {
        if (SadEntry->type < MemTypeMax) {
          (*MemTypesPresent)[SadEntry->type] = TRUE;
        }
      }
    }
  }

  if (MemMapHost->BlockDecoderData.BlockMcChEn[SocketId][McId][ChIdPerMc] != 0) {
    (*MemTypesPresent)[MemType1lmCtrl] = TRUE;
  }

  return;
}

/**

  Get a list of the memory modes present in the input IMC.
  @param[in]      Sckt            - Socket Id.
  @param[in]      Mc              - MC Index
  @param[in,out]  MemTypesPresent - A pointer to an array that will be populated with the list of memory modes present in the MC

**/
VOID
GetMcMemTypeList (
  IN     UINT8     Sckt,
  IN     UINT8     Mc,
  IN OUT BOOLEAN   (*MemTypesPresent)[MemTypeMax]
  )
{
  MEMORY_MAP_HOST               *MemMapHost;
  UINT8                         SadIndex;
  UINT8                         BeginSadIndex;
  UINT8                         EndSadIndex;
  UINT8                         ClusterId;
  UINT8                         MaxImc;
  SAD_TABLE                     *SADEntry;
  UINT8                         Ch;
  UINT8                         NumChPerMc;

  MemMapHost = GetMemMapHostPointer ();
  ASSERT(MemMapHost != NULL);
  if (MemMapHost == NULL) {
    return;
  }

  if (MemTypesPresent == NULL) {
    // The input buffer should never be NULL
    ASSERT (MemTypesPresent != NULL);
    return;
  }

  ZeroMem(MemTypesPresent, sizeof(*MemTypesPresent));

  if (CheckSocketEnabled (Sckt) == 0) {
    return;
  }

  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();

  for (ClusterId = 0; ClusterId < MemMapHost->NumberofChaDramClusters; ClusterId++) {
    BeginSadIndex = (ClusterId * MAX_SAD_RULES);
    EndSadIndex = (BeginSadIndex + MAX_SAD_RULES);
    SADEntry = GetSadRuleEntry (Sckt, BeginSadIndex);
    if (SADEntry == NULL) {
      break;
    }
    if (SADEntry->Enable == 0) {
      break;
    }
    for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
      SADEntry = GetSadRuleEntry (Sckt, SadIndex);
      if(SADEntry == NULL) {
        break;
      }
      //Break if we have reached the end of the SAD table
      if (SADEntry->Enable == 0) {
        break;
      }

      // Continue if entry is a remote entry
      if (SADEntry->local == 0) {
        continue;
      }

      // Check off the memory modes present in the SAD rule the MC participates in.
      // For 2LM we also need to cover the cases where the MC is acting as near memory for
      // X-tile configuration.
      if ((MemMapHost->SocketInfo[Sckt].SadIntList[SadIndex][(Sckt * MaxImc) + Mc] != 0) ||
        (IsMemType2lm (SADEntry->type) && (SADEntry->NmImcInterBitmap & (1 << Mc)))) {
        if (SADEntry->type < MemTypeMax) {
          (*MemTypesPresent)[SADEntry->type] = TRUE;
        }
      }
    } // SadIndex
  } // ClusterId

  for (Ch = 0; Ch < NumChPerMc; Ch++) {
    if (MemMapHost->BlockDecoderData.BlockMcChEn[Sckt][Mc][Ch] != 0) {
      (*MemTypesPresent)[MemType1lmCtrl] = TRUE;
      break;
    }
  }

  return;
}

/**

  Get a list of the memory modes present in the input Socket.

  @param[in]      Sckt            - Socket Id.
  @param[in,out]  MemTypesPresent - A pointer to an array that will be populated with the list of memory modes present
                                    on the Socket

**/
VOID
GetSocketMemTypeList (
  IN     UINT8     Sckt,
  IN OUT BOOLEAN   (*MemTypesPresent)[MemTypeMax]
  )
{
  UINT8          MaxImc;
  UINT8          McIndex;
  BOOLEAN        MemTypesPresentTemp[MemTypeMax];
  MEM_TYPE       Index;

  if (MemTypesPresent == NULL) {
    // The input buffer should never be NULL
    ASSERT (MemTypesPresent != NULL);
    return;
  }

  ZeroMem (MemTypesPresent, sizeof (*MemTypesPresent));

  MaxImc = GetMaxImc ();
  for (McIndex = 0; McIndex < MaxImc; McIndex++) {
    GetMcMemTypeList (Sckt, McIndex, &MemTypesPresentTemp);
    //
    // Merge the list across MCs
    //
    for (Index = 0; Index < MemTypeMax; Index++) {
      if (MemTypesPresentTemp[Index]) {
        (*MemTypesPresent)[Index] = TRUE;
      }
    }
  }

  return;
}

/**

  Get a list of the memory modes present in all sockets on the system.

  @param[in,out]  MemTypesPresent - A pointer to an array that will be populated with the list of memory modes present
                                    on the system

**/
VOID
GetSystemMemTypeList (
  IN OUT BOOLEAN   (*MemTypesPresent)[MemTypeMax]
  )
{
  UINT8          Socket;
  BOOLEAN        MemTypesPresentTemp[MemTypeMax];
  MEM_TYPE       Index;

  if (MemTypesPresent == NULL) {
    // The input buffer should never be NULL
    ASSERT (MemTypesPresent != NULL);
    return;
  }

  ZeroMem (MemTypesPresent, sizeof (*MemTypesPresent));

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    GetSocketMemTypeList (Socket, &MemTypesPresentTemp);
    //
    // Merge the list across Sockets
    //
    for (Index = 0; Index < MemTypeMax; Index++) {
      if (MemTypesPresentTemp[Index]) {
        (*MemTypesPresent)[Index] = TRUE;
      }
    }
  }

  return;
}

/**

  Check if the input list of MemTypesPresent has no memory modes present.

  @param[in]  MemTypesPresent - A pointer to an array that will be checked for memory modes present

  @retval TRUE  - The MemTypesPresent array has no entries set to "TRUE"
  @retval FALSE - The MemTypesPresent array has one or more entries set to "TRUE"

**/
BOOLEAN
CheckMemTypeListEmpty (
  IN BOOLEAN   (*MemTypesPresent)[MemTypeMax]
  )
{
  MEM_TYPE MemType;

  if (MemTypesPresent == NULL) {
    ASSERT (MemTypesPresent != NULL);
    return FALSE;
  }

  for (MemType = 0; MemType < MemTypeMax; MemType++) {
    if (MemType == MemTypeNone) {
      // Don't check for MemTypeNone
      continue;
    }
    if ((*MemTypesPresent)[MemType]) {
      return FALSE;
    }
  }
  return TRUE;
}


/**

  Check if the input list of MemTypesPresent has one or more memory modes matching the IsMemType condition.

  @param[in] MemTypesPresent - A pointer to an array that will be checked
  @param[in] IsMemType       - A pointer to the function that will be used to check the list

  @retval TRUE  - The MemTypesPresent array has one or more memory modes present and matching the IsMemType condition.
  @retval FALSE - The MemTypesPresent array has no entries matching the IsMemType condition.

**/
BOOLEAN
CheckMemTypeList (
  IN BOOLEAN        (*MemTypesPresent)[MemTypeMax],
  IN MEM_TYPE_CHECK IsMemType
  )
{
  MEM_TYPE MemType;

  if (MemTypesPresent == NULL || IsMemType == NULL) {
    ASSERT (!(MemTypesPresent == NULL || IsMemType == NULL));
    return FALSE;
  }

  for (MemType = 0; MemType < MemTypeMax; MemType++) {
    if ((*MemTypesPresent)[MemType]) {
      if ((*IsMemType) (MemType)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**

  Check if the input channel contains a SAD table entry with a type that matches the IsMemType condition.

  @param[in] SocketId        Index of the socket for the input channel to check.
  @param[in] ChId            Index of the channel to check within socket.
  @param[in] IsMemType       Pointer to the function that will be used to check the SAD table entry types.

  @retval TRUE   The channel has one or more SAD table entries with the type matching the IsMemType condition.
  @retval FALSE  The channel does not have any SAD table entries with the type matching the IsMemType condition.

**/
BOOLEAN
EFIAPI
IsMemTypePresentChFunction (
  IN UINT8          SocketId,
  IN UINT8          ChId,
  IN MEM_TYPE_CHECK IsMemType
  )
{
  BOOLEAN MemTypesPresent[MemTypeMax];

  GetChMemTypeList (SocketId, ChId, &MemTypesPresent);
  return CheckMemTypeList (&MemTypesPresent, IsMemType);
}

/**

  Check if the input channel contains a SAD table entry that matches the MemType element.

  @param[in] SocketId        Index of the Socket for the input channel to check.
  @param[in] ChId            Index of the channel to check within socket.
  @param[in] MemType         MemType element that will be used to check the SAD table entry types

  @retval TRUE   The channel has one or more SAD table entries with the type matching the MemType element.
  @retval FALSE  The channel does not have any SAD table entries with the type matching the MemType element.

**/
BOOLEAN
EFIAPI
IsMemTypePresentChEnum (
  IN UINT8    SocketId,
  IN UINT8    ChId,
  IN MEM_TYPE MemType
  )
{
  BOOLEAN MemTypesPresent[MemTypeMax];

  GetChMemTypeList (SocketId, ChId, &MemTypesPresent);
  return MemTypesPresent[MemType];
}

/**

  Check if the input Mc contains a SAD table entry with a type that matches the IsMemType condition

  @param[in] Sckt      - Index of the Socket for the input MC to check
  @param[in] Mc        - Index of the Mc to check
  @param[in] IsMemType - A pointer to the function that will be used to check the SAD table entry types

  @retval TRUE  - The MC has one or more SAD table entries present with a type that matches the IsMemType condition.
  @retval FALSE - The MC does not have any SAD table entries with the type matching the IsMemType condition.

**/
BOOLEAN
IsMemTypePresentMcFunction (
  IN UINT8          Sckt,
  IN UINT8          Mc,
  IN MEM_TYPE_CHECK IsMemType
  )
{
  BOOLEAN MemTypesPresent[MemTypeMax];

  GetMcMemTypeList (Sckt, Mc, &MemTypesPresent);
  return CheckMemTypeList (&MemTypesPresent, IsMemType);
}

/**

  Check if the input Mc contains a SAD table entry that matches the MemType element

  @param[in] Sckt      - Index of the Socket for the input MC to check
  @param[in] Mc        - Index of the Mc to check
  @param[in] MemType   - MemType element that will be used to check the SAD table entry types

  @retval TRUE  - The MC has one or more memory modes present that match the MemType element
  @retval FALSE - The MC does not have memory modes matching the MemType element

**/
BOOLEAN
IsMemTypePresentMcEnum (
  IN UINT8    Sckt,
  IN UINT8    Mc,
  IN MEM_TYPE MemType
  )
{
  BOOLEAN MemTypesPresent[MemTypeMax];

  GetMcMemTypeList (Sckt, Mc, &MemTypesPresent);
  return MemTypesPresent[MemType];
}

/**

  Check if the input Mc contains no memory types.

  @param[in] Sckt      - Index of the Socket for the input MC to check
  @param[in] Mc        - Index of the Mc to check

  @retval TRUE  - The MC has no memory types present
  @retval FALSE - The MC has one or more memory types present

**/
BOOLEAN
IsMemTypeEmptyMc (
  IN UINT8    Sckt,
  IN UINT8    Mc
  )
{
  BOOLEAN MemTypesPresent[MemTypeMax];

  GetMcMemTypeList (Sckt, Mc, &MemTypesPresent);
  return CheckMemTypeListEmpty (&MemTypesPresent);
}

/**

  Check if the input Socket contains a SAD table entry with a type that matches the IsMemType condition

  @param[in] Sckt      - Index of the Socket to check
  @param[in] IsMemType - A pointer to the function that will be used to check the SAD table entry types

  @retval TRUE  - The Socket has one or more SAD table entries present with a type that matches the IsMemType condition.
  @retval FALSE - The Socket does not have any SAD table entries with the type matching the IsMemType condition.

**/
BOOLEAN
IsMemTypePresentSocketFunction (
  IN UINT8          Sckt,
  IN MEM_TYPE_CHECK IsMemType
  )
{
  BOOLEAN MemTypesPresent[MemTypeMax];

  GetSocketMemTypeList (Sckt, &MemTypesPresent);
  return CheckMemTypeList (&MemTypesPresent, IsMemType);
}

/**

  Check if the input Socket contains a SAD table entry that matches the MemType element

  @param[in] Sckt      - Index of the Socket to check
  @param[in] MemType   - MemType element that will be used to check the SAD table entry types

  @retval TRUE  - The Socket has one or more memory modes present that match the MemType element
  @retval FALSE - The Socket does not have memory modes matching the MemType element

**/
BOOLEAN
IsMemTypePresentSocketEnum (
  IN UINT8    Sckt,
  IN MEM_TYPE MemType
  )
{
  BOOLEAN MemTypesPresent[MemTypeMax];

  GetSocketMemTypeList (Sckt, &MemTypesPresent);
  return MemTypesPresent[MemType];
}

/**

  Check if the System contains a SAD table entry with a type that matches the IsMemType condition

  @param[in] IsMemType - A pointer to the function that will be used to check the SAD table entry types

  @retval TRUE  - The System has one or more SAD table entries present with a type that matches the IsMemType condition.
  @retval FALSE - The System does not have any SAD table entries with the type matching the IsMemType condition.

**/
BOOLEAN
IsMemTypePresentSystemFunction (
  IN MEM_TYPE_CHECK IsMemType
  )
{
  BOOLEAN MemTypesPresent[MemTypeMax];

  GetSystemMemTypeList (&MemTypesPresent);
  return CheckMemTypeList (&MemTypesPresent, IsMemType);
}

/**

  Check if the input System contains a SAD table entry that matches the MemType element

  @param[in] MemType   - MemType element that will be used to check the SAD table entry types

  @retval TRUE  - The System has one or more memory modes present that match the MemType element
  @retval FALSE - The System does not have memory modes matching the MemType element

**/
BOOLEAN
IsMemTypePresentSystemEnum (
  IN MEM_TYPE MemType
  )
{
  BOOLEAN MemTypesPresent[MemTypeMax];

  GetSystemMemTypeList (&MemTypesPresent);
  return MemTypesPresent[MemType];
}

/**

  Gets the printable memory type string for the given memory type.

  @param[in] MemType  Memory type element

  @retval Memory type string

**/
CONST
CHAR8*
EFIAPI
GetMemTypeString (
  IN MEM_TYPE    MemType
  )
{
  switch (MemType) {
  case MemType1lmDdr:
    return "1LM";
  case MemType2lmDdrCacheMemoryMode:
    return "2LM";
  case MemType1lmAppDirect:
    return "PMEM";
  case MemType1lmAppDirectReserved :
    return "PMEM RSVD";
  case MemType2lmDdrWbCacheAppDirect :
    return "WB PMEM";
  case MemType1lmCtrl:
    return "NVDIM_CTRL";
  case MemType1lmHbm:
    return "HBM";
  case MemType2lmHbmCacheDdr:
    return "HBM 2LM DDR";
  case MemType2lmHbmCacheMemoryMode:
    return "HBM 2LM DDRT";
  case MemTypeNxm:
    return "NXM";
  case MemTypeCxlAccVolatileMem:
    return "CXL ACC VOL";
  case MemTypeCxlAccPersistentMem:
    return "CXL ACC PER";
  case MemTypeFpga:
    return "FPGA";
  default:
    return "Unknown";
  }
}
