/** @file
  Base RAS RC mirror library class implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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
**/

#include <Base.h>
#include <Library/RasRcMirrorLib.h>
#include <Library/MemTypeLib.h>
#include <Library/SysHostPointerLib.h>
#include <SysHost.h>

//
// Internal function prototypes
//

BOOLEAN
IsMirrorByRatioEnabledInternal (
  IN CONST  SYSHOST   *Host,
  IN CONST  SYS_SETUP *Setup,
  IN        UINTN     MemoryType,
  OUT       BOOLEAN   *IsFull     OPTIONAL
  );

BOOLEAN
IsMirrorBySizeEnabledInternal (
  IN CONST  SYSHOST   *Host,
  IN CONST  SYS_SETUP *Setup,
  IN        UINTN     MemoryType
  );

//
// Library API function implementations
//

/**
  Check if low memory (below 4GB) mirror is enabled for a given memory type.

  @param[in]      MemoryType        Type of memory to check enable for.

  @retval TRUE                      Enabled for the given memory type.
  @retval FALSE                     Disabled for the given memory type.
**/
BOOLEAN
EFIAPI
IsLowMemoryMirrorEnabled (
  IN        UINTN     MemoryType
  )
{
  CONST SYSHOST   *Host;
  CONST SYS_SETUP *Setup;
  BOOLEAN         Enabled;

  Host = GetSysHostPointer ();
  Setup = GetSysSetupPointer ();

  ASSERT (Host != NULL);
  ASSERT (Setup != NULL);
  if (Host == NULL || Setup == NULL) {
    return FALSE;
  }

  Enabled = FALSE;

  if (!IsMemTypeAppDirect (MemoryType)) {
    if (IsMemType1lm (MemoryType)) {
      if ((Host->nvram.mem.RASmode & FULL_MIRROR_1LM) != 0) {
        Enabled = TRUE;
      } else if ((Host->nvram.mem.RASmode & PARTIAL_MIRROR_1LM) != 0) {
        if (Setup->mem.partialmirrorsad0 == 1) {
          Enabled = TRUE;
        }
      }
    } else if (IsMemType2lm (MemoryType)) {
      if ((Host->nvram.mem.RASmode & FULL_MIRROR_2LM) != 0) {
        Enabled = TRUE;
      } else if ((Host->nvram.mem.RASmode & PARTIAL_MIRROR_2LM) != 0) {
        if (Setup->mem.partialmirrorsad0 == 1) {
          Enabled = TRUE;
        }
      }
    }
  }

  return Enabled;
}

/**
  Check if mirror by ratio is enabled for a given memory type.

  If this function returns TRUE, GetMirrorRatio should be called to determine
  what ratio to use.

  @param[in]      MemoryType        Type of memory to check enable for.

  @retval TRUE                      Enabled for the given memory type.
  @retval FALSE                     Disabled for the given memory type.
**/
BOOLEAN
EFIAPI
IsMirrorByRatioEnabled (
  IN        UINTN     MemoryType
  )
{
  return IsMirrorByRatioEnabledInternal (
    GetSysHostPointer (),
    GetSysSetupPointer (),
    MemoryType,
    NULL
    );
}

/**
  Check if mirror by size is enabled for a given memory type.

  If this function returns TRUE, GetMirrorSizes should be called to determine
  what block sizes to use.

  @param[in]      MemoryType        Type of memory to check enable for.

  @retval TRUE                      Enabled for the given memory type.
  @retval FALSE                     Disabled for the given memory type.
**/
BOOLEAN
EFIAPI
IsMirrorBySizeEnabled (
  IN        UINTN     MemoryType
  )
{
  return IsMirrorBySizeEnabledInternal (
    GetSysHostPointer (),
    GetSysSetupPointer (),
    MemoryType
    );
}

/**
  Get the mirror ratio for a given memory type.

  This function should not be called and will fail if IsMirrorByRatioEnabled
  returns FALSE.

  Given the Numerator (N) and Denominator (D) returned by this function, and
  the total memory size (T), the mirror size (M) should be computed as follows:

  M = (T * N) / D

  @param[in]      MemoryType        Type of memory to get ratio for.
  @param[out]     Numerator         Numerator of the ratio.
  @param[out]     Denominator       Demoninator of the ratio.

  @retval EFI_SUCCESS               Data successfully returned.
  @retval EFI_UNSUPPORTED           Unsupported for the given memory type.
  @retval EFI_UNSUPPORTED           Ratio greater than 1/2 was requested.
  @retval EFI_INVALID_PARAMETER     A required pointer parameter was NULL.
**/
EFI_STATUS
EFIAPI
GetMirrorRatio (
  IN        UINTN     MemoryType,
  OUT       UINT32    *Numerator,
  OUT       UINT32    *Denominator
  )
{
  CONST SYSHOST   *Host;
  CONST SYS_SETUP *Setup;
  BOOLEAN         IsFull;

  ASSERT (Numerator != NULL);
  ASSERT (Denominator != NULL);
  if (Numerator == NULL || Denominator == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Host = GetSysHostPointer ();
  Setup = GetSysSetupPointer ();

  if (!IsMirrorByRatioEnabledInternal (Host, Setup, MemoryType, &IsFull)) {
    return EFI_UNSUPPORTED;
  }

  if (IsFull) {
    //
    // Full mirroring means half the memory is mirrored by the other half.
    //
    *Numerator = 1;
    *Denominator = 2;
  } else {
    if (Setup->mem.partialmirrorpercent > 5000) {
      return EFI_UNSUPPORTED;
    } else {
      *Numerator = Setup->mem.partialmirrorpercent;
      *Denominator = 10000;
    }
  }

  return EFI_SUCCESS;
}

/**
  Get the mirror sizes for a given memory type.

  This function should not be called and will fail if IsMirrorBySizeEnabled
  returns FALSE.

  To get the sizes, the caller should provide an array pointed to by Sizes,
  with *Count set to the length of the array. If the number of sizes supported
  is less than or equal to *Count, this function will update *Count to the
  number supported and return EFI_SUCCESS.

  To query the total number of sizes supported for the given memory type, the
  caller should set *Count to 0. Sizes may be NULL in this case only. This
  function returns EFI_BUFFER_TOO_SMALL with *Count updated to the number of
  supported sizes. The caller may then allocate an array of appropriate size
  before calling this function again.

  @param[in]      MemoryType        Type of memory to get ratio for.
  @param[in, out] Count             Number of sizes provided.
  @param[out]     Sizes             Array of *Count mirror sizes.

  @retval EFI_SUCCESS               Data successfully returned.
  @retval EFI_UNSUPPORTED           Unsupported for the given memory type.
  @retval EFI_INVALID_PARAMETER     A required pointer parameter was NULL.
  @retval EFI_BUFFER_TOO_SMALL      Array too small on input, *Count updated.
**/
EFI_STATUS
EFIAPI
GetMirrorSizes (
  IN        UINTN     MemoryType,
  IN OUT    UINT32    *Count,
  OUT       UINT32    *Sizes      OPTIONAL
  )
{
  CONST SYSHOST   *Host;
  CONST SYS_SETUP *Setup;
  UINT8           Index;

  ASSERT (Count != NULL);
  if (Count == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (*Count != 0) {
    ASSERT (Sizes != NULL);
    if (Sizes == NULL) {
      return EFI_INVALID_PARAMETER;
    }
  }

  Host = GetSysHostPointer ();
  Setup = GetSysSetupPointer ();

  if (!IsMirrorBySizeEnabledInternal (Host, Setup, MemoryType)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Loop to count the valid entries.
  //
  ASSERT (MAX_PARTIAL_MIRROR <= MAX_UINT8);
  for (Index = 0; Index < MAX_PARTIAL_MIRROR; Index++) {
    if (Setup->mem.partialmirrorsize[Index] == 0) {
      break;
    }
  }

  if (Index > *Count) {
    *Count = Index;
    return EFI_BUFFER_TOO_SMALL;
  }

  //
  // Second loop, to copy the entries.
  //
  *Count = Index;
  for (Index = 0; Index < *Count; Index++) {
    ASSERT (Setup->mem.partialmirrorsize[Index] != 0);
    Sizes[Index] = (UINT32) Setup->mem.partialmirrorsize[Index];
  }

  return EFI_SUCCESS;
}

//
// Internal function implementations
//

/**
  Internal helper for IsMirrorByRatioEnabled.

  This function be called internally by functions that already have the Host
  and Setup pointers.

  @param[in]      Host              Pointer to the host structure.
  @param[in]      Setup             Pointer to the setup structure.
  @param[in]      MemoryType        Type of memory to check enable for.
  @param[out]     IsFull            TRUE if full mirroring, FALSE if partial.

  @retval TRUE                      Enabled for the given memory type.
  @retval FALSE                     Disabled for the given memory type.
**/
BOOLEAN
IsMirrorByRatioEnabledInternal (
  IN CONST  SYSHOST   *Host,
  IN CONST  SYS_SETUP *Setup,
  IN        UINTN     MemoryType,
  OUT       BOOLEAN   *IsFull     OPTIONAL
  )
{
  BOOLEAN         Enabled;

  ASSERT (Host != NULL);
  ASSERT (Setup != NULL);
  if (Host == NULL || Setup == NULL) {
    return FALSE;
  }

  Enabled = FALSE;
  if (IsFull != NULL) {
    *IsFull = FALSE;
  }

  if (!IsMemTypeAppDirect (MemoryType)) {
    if (IsMemType1lm (MemoryType)) {
      if ((Host->nvram.mem.RASmode & FULL_MIRROR_1LM) != 0) {
        Enabled = TRUE;
        if (IsFull != NULL) {
          *IsFull = TRUE;
        }
      } else if ((Host->nvram.mem.RASmode & PARTIAL_MIRROR_1LM) != 0) {
        if (Setup->mem.partialMirrorUEFI == 1) {
          Enabled = TRUE;
        }
      }
    } else if (IsMemType2lm (MemoryType)) {
      if ((Host->nvram.mem.RASmode & FULL_MIRROR_2LM) != 0) {
        Enabled = TRUE;
        if (IsFull != NULL) {
          *IsFull = TRUE;
        }
      } else if ((Host->nvram.mem.RASmode & PARTIAL_MIRROR_2LM) != 0) {
        if (Setup->mem.partialMirrorUEFI == 1) {
          Enabled = TRUE;
        }
      }
    }
  }

  return Enabled;
}

/**
  Internal helper for IsMirrorBySizeEnabled.

  This function be called internally by functions that already have the Host
  and Setup pointers.

  @param[in]      Host              Pointer to the host structure.
  @param[in]      Setup             Pointer to the setup structure.
  @param[in]      MemoryType        Type of memory to check enable for.

  @retval TRUE                      Enabled for the given memory type.
  @retval FALSE                     Disabled for the given memory type.
**/
BOOLEAN
IsMirrorBySizeEnabledInternal (
  IN CONST  SYSHOST   *Host,
  IN CONST  SYS_SETUP *Setup,
  IN        UINTN     MemoryType
  )
{
  BOOLEAN         Enabled;

  ASSERT (Host != NULL);
  ASSERT (Setup != NULL);
  if (Host == NULL || Setup == NULL) {
    return FALSE;
  }

  Enabled = FALSE;

  if (!IsMemTypeAppDirect (MemoryType)) {
    if (IsMemType1lm (MemoryType)) {
      if ((Host->nvram.mem.RASmode & PARTIAL_MIRROR_1LM) != 0) {
        if (Setup->mem.partialMirrorUEFI == 0) {
          Enabled = TRUE;
        }
      }
    } else if (IsMemType2lm (MemoryType)) {
      if ((Host->nvram.mem.RASmode & PARTIAL_MIRROR_2LM) != 0) {
        if (Setup->mem.partialMirrorUEFI == 0) {
          Enabled = TRUE;
        }
      }
    }
  }

  return Enabled;
}
