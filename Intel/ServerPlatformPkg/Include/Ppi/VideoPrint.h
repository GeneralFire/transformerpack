/** @file

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

#ifndef _EFI_VIDEOPRINT_PEI_H
#define _EFI_VIDEOPRINT_PEI_H

/**
  Print string on the cosole.

  @param Format                  A pointer to the string that will print on the console

  @retval EFI_SUCCESS         The string prints on the console successfully

**/
typedef
EFI_STATUS
(EFIAPI *EFI_VIDEOPRINT_PPI_VIDEO_PRINT ) (IN CONST CHAR8 *Format);

/**
  On the specified line of the screen, print string with ForegroundColor and BackgroundColor.

  @param ForegroundColor    The print string's fore ground color
  @param BackgroundColor    The print string's back ground color
  @param Format             A pointer to the string that will print on the console

  @retval EFI_SUCCESS         The string prints on the console successfully

**/
typedef
EFI_STATUS
(EFIAPI *EFI_VIDEOPRINT_PPI_VIDEO_PRINT_COLOR ) (
  IN UINT8 ForegroundColor,
  IN UINT8 BackgroundColor,
  IN CONST CHAR8 *Format
  );

/**
  On the specified position of the screen, print string with ForegroundColor and BackgroundColor.

  @param Row                The specified line on the console
  @param Column             The specified Column on the console
  @param ForegroundColor    The print string's fore ground color
  @param BackgroundColor    The print string's back ground color
  @param Format             A pointer to the string that will print on the console

  @retval EFI_SUCCESS     The string prints on the console successfully

**/
typedef 
UINT32
(EFIAPI *EFI_VIDEOPRINT_PPI_VIDEO_PRINT_XY_COLOR ) (
  IN UINT8 Row, 
  IN UINT8 Column,
  IN UINT8 ForegroundColor,
  IN UINT8 BackgroundColor,
  IN CONST CHAR8 *Format,
  ...
  );

/**
  Clear the console.

  @param 
  
  @retval EFI_SUCCESS         Console clear successfully

**/
typedef
EFI_STATUS
(EFIAPI *EFI_VIDEOPRINT_PPI_CONSOLE_CLEAR ) ();

typedef struct _EFI_VIDEOPRINT_PPI {
  EFI_VIDEOPRINT_PPI_VIDEO_PRINT            ConsolePrint;
  EFI_VIDEOPRINT_PPI_VIDEO_PRINT_COLOR      ConsolePrintColor;
  EFI_VIDEOPRINT_PPI_VIDEO_PRINT_XY_COLOR   ConsolePrintXYColor;
  EFI_VIDEOPRINT_PPI_CONSOLE_CLEAR          ConsoleClear;
} EFI_VIDEOPRINT_PPI;

#define VIDEO_COLOR_BLACK               0
#define VIDEO_COLOR_BLUE                1
#define VIDEO_COLOR_GREEN               2
#define VIDEO_COLOR_CYAN                3
#define VIDEO_COLOR_RED                 4
#define VIDEO_COLOR_MAGENTA             5
#define VIDEO_COLOR_ORANGE              6
#define VIDEO_COLOR_WHITE               7

extern EFI_GUID   gEfiVideoPrintPpiGuid;

#endif
