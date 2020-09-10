/** @file
  Defines file-path manipulation functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2016 Intel Corporation. <BR>

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

#include  <Library/BaseMemoryLib.h>
#include  <Library/BaseLib.h>

/**
  Removes the last directory or file entry in a path. For a path which is
  like L"fs0:startup.nsh", it's converted to L"fs0:".

  @param[in,out] Path     A pointer to the path to modify.

  @retval FALSE     Nothing was found to remove.
  @retval TRUE      A directory or file was removed.
**/
BOOLEAN
EFIAPI
PathRemoveLastItem(
  IN OUT CHAR16 *Path
  )
{
  CHAR16        *Walker;
  CHAR16        *LastSlash;
  //
  // get directory name from path... ('chop' off extra)
  //
  for ( Walker = Path, LastSlash = NULL
      ; Walker != NULL && *Walker != CHAR_NULL
      ; Walker++
     ){
    if (*Walker == L'\\' && *(Walker + 1) != CHAR_NULL) {
      LastSlash = Walker+1;
    } else if (*Walker == L':' && *(Walker + 1) != L'\\' && *(Walker + 1) != CHAR_NULL) {
      LastSlash = Walker+1;
    }
  }
  if (LastSlash != NULL) {
    *LastSlash = CHAR_NULL;
    return (TRUE);
  }
  return (FALSE);
}

/**
  Function to clean up paths.

  - Single periods in the path are removed.
  - Double periods in the path are removed along with a single parent directory.
  - Forward slashes L'/' are converted to backward slashes L'\'.

  This will be done inline and the existing buffer may be larger than required
  upon completion.

  @param[in] Path       The pointer to the string containing the path.

  @return       Returns Path, otherwise returns NULL to indicate that an error has occured.
**/
CHAR16*
EFIAPI
PathCleanUpDirectories(
  IN CHAR16 *Path
)
{
  CHAR16  *TempString;

  if (Path == NULL) {
    return NULL;
  }

  //
  // Replace the '/' with '\'
  //
  for (TempString = Path; *TempString != CHAR_NULL; TempString++) {
    if (*TempString == L'/') {
      *TempString = L'\\';
    }
  }

  //
  // Remove all the "\.". E.g.: fs0:\abc\.\def\.
  //
  while ((TempString = StrStr (Path, L"\\.\\")) != NULL) {
    CopyMem (TempString, TempString + 2, StrSize (TempString + 2));
  }
  if (StrCmp (Path + StrLen (Path) - 2, L"\\.") == 0) {
    Path[StrLen (Path) - 1] = CHAR_NULL;
  }

  //
  // Remove all the "\..". E.g.: fs0:\abc\..\def\..
  //
  while (((TempString = StrStr(Path, L"\\..")) != NULL) &&
         ((*(TempString + 3) == L'\\') || (*(TempString + 3) == CHAR_NULL))
        ) {
    *(TempString + 1) = CHAR_NULL;
    PathRemoveLastItem(Path);
    CopyMem (Path + StrLen (Path), TempString + 3, StrSize (TempString + 3));
  }

  //
  // Replace the "\\" with "\"
  //
  while ((TempString = StrStr (Path, L"\\\\")) != NULL) {
    CopyMem (TempString, TempString + 1, StrSize (TempString + 1));
  }

  return Path;
}

