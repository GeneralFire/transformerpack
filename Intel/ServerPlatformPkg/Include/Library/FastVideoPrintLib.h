/** @file
  FastVideoPrint implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2018 Intel Corporation. <BR>

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

#ifndef _FAST_VIDEO_PRINT_LIB_H_
#define _FAST_VIDEO_PRINT_LIB_H_

#include <Base.h>
#include <Uefi.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Protocol/VideoPrint.h>

#define VIDEO_PRINT_MAX_STRING             80
#define POST_CODE_PRINT_SCREEN_ROWS        24
#define POST_CODE_PRINT_SCREEN_COLS        73

#define VIDEO_PRINT_MEMORY_BASE               0xB8000
#define VIDEO_PRINT_MEMORY_ROW                25
#define VIDEO_PRINT_MEMORY_COLUMN             80
#define VIDEO_PRINT_MEMORY_COLUMN_BYTE        (VIDEO_PRINT_MEMORY_COLUMN * 2)
#define VIDEO_PRINT_MEMORY_SIZE_PER_SCREEN    (VIDEO_PRINT_MEMORY_ROW * VIDEO_PRINT_MEMORY_COLUMN_BYTE)
#define VIDEO_PATH_ENTRY                      3

#define FRAMEBUFFER                     0xB8000
#define COLS                            80
#define ROWS                            25
#define BYTES_PER_CHAR                  2   //ASCII value and attribute byte
//
// Use memory outside the screen area to hold cursor location and
// current color.
//
#define CURSOR_STORAGE                  FRAMEBUFFER + ROWS*COLS*BYTES_PER_CHAR

#define VIDEO_COLOR_BLACK               0
#define VIDEO_COLOR_BLUE                1
#define VIDEO_COLOR_GREEN               2
#define VIDEO_COLOR_CYAN                3
#define VIDEO_COLOR_RED                 4
#define VIDEO_COLOR_MAGENTA             5
#define VIDEO_COLOR_ORANGE              6
#define VIDEO_COLOR_WHITE               7

/**
  Print string on the cosole.

  @param Str                       A pointer to the string that will print on the console

  @retval EFI_SUCCESS         The string prints on the console successfully

**/
EFI_STATUS
ConsolePrint (
  IN CONST CHAR8 *Str
  );

/**
  On the specified line of the screen, print string with ForegroundColor and BackgroundColor.

  @param ForegroundColor    The print string's fore ground color
  @param BackgroundColor    The print string's back ground color
  @param Str                A pointer to the string that will print on the console

  @retval EFI_SUCCESS         The string prints on the console successfully

**/
EFI_STATUS
ConsolePrintColor (
  IN UINT8 ForegroundColor,
  IN UINT8 BackgroundColor,
  IN CONST CHAR8 *Str
  );

/**
  On the specified position of the screen, print string with ForegroundColor and BackgroundColor.

  @param Row                The specified line on the console
  @param Column             The specified Column on the console
  @param ForegroundColor    The print string's fore ground color
  @param BackgroundColor    The print string's back ground color
  @param Str                A pointer to the string that will print on the console

  @retval EFI_SUCCESS     The string prints on the console successfully

**/
UINT32
ConsolePrintXYColor(
  IN UINT8 Row, 
  IN UINT8 Column,
  IN UINT8 ForegroundColor,
  IN UINT8 BackgroundColor,
  IN CONST CHAR8* Format,
  ...
  );

/**
  Clear the console.

  @param 
  
  @retval EFI_SUCCESS         Console clear successfully

**/
EFI_STATUS
ConsoleClear (
  VOID
  );

/*++
  Convert BCD code to Ascii code.

  @param Value            input BCD code

  @retval CHAR8            Ascii code of the input value.

--*/
CHAR8
ConvertBcdToAscii (
  IN UINT8 Value
  );

#endif
