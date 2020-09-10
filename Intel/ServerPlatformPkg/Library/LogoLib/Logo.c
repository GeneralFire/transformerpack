/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2015 Intel Corporation. <BR>

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

#include <PiDxe.h>
#include <Protocol/SimpleTextOut.h>
#include <Protocol/OEMBadging.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/UgaDraw.h>
#include <Protocol/BootLogo.h>
//#
//#
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BmpSupportLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>

#include "Bmp.h"

/**
  Use SystemTable Conout to stop video based Simple Text Out consoles from going
  to the video device. Put up LogoFile on every video device that is a console.

  @param[in]  LogoFile   File name of logo to display on the center of the screen.

  @retval EFI_SUCCESS     ConsoleControl has been flipped to graphics and logo displayed.
  @retval EFI_UNSUPPORTED Logo not found

**/
EFI_STATUS
EFIAPI
EnableQuietBoot (
  IN  EFI_GUID  *LogoFile
  )
{
  EFI_STATUS                    Status;
  EFI_OEM_BADGING_PROTOCOL      *Badging;
  UINT32                        SizeOfX;
  UINT32                        SizeOfY;
  INTN                          DestX;
  INTN                          DestY;
  UINT8                         *ImageData;
  UINTN                         ImageSize;
  UINTN                         BltSize;
  UINT32                        Instance;
  EFI_BADGING_FORMAT            Format;
  EFI_BADGING_DISPLAY_ATTRIBUTE Attribute;
  UINTN                         CoordinateX;
  UINTN                         CoordinateY;
  UINTN                         Height;
  UINTN                         Width;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Blt;
  EFI_UGA_DRAW_PROTOCOL         *UgaDraw;
  UINT32                        ColorDepth;
  UINT32                        RefreshRate;
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput;

  EFI_BOOT_LOGO_PROTOCOL        *BootLogo = NULL;
  UINTN                         NumberOfLogos = 0;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *LogoBlt = NULL;

  UINTN                         LogoDestX = 0;
  UINTN                         LogoDestY = 0;
  UINTN                         LogoHeight = 0;
  UINTN                         LogoWidth = 0;

  UINTN                         NewDestX = 0;
  UINTN                         NewDestY = 0;
  UINTN                         NewHeight = 0;
  UINTN                         NewWidth = 0;

  UINTN                         BufferSize = 0;

//#
//#

//#
//#

  UgaDraw = NULL;
  //
  // Try to open GOP first
  //
  Status = gBS->HandleProtocol (gST->ConsoleOutHandle, &gEfiGraphicsOutputProtocolGuid, (VOID **) &GraphicsOutput);
  if (EFI_ERROR (Status) && FeaturePcdGet (PcdUgaConsumeSupport)) {
    GraphicsOutput = NULL;
    //
    // Open GOP failed, try to open UGA
    //
    Status = gBS->HandleProtocol (gST->ConsoleOutHandle, &gEfiUgaDrawProtocolGuid, (VOID **) &UgaDraw);
  }
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Erase Cursor from screen
  //
  gST->ConOut->EnableCursor (gST->ConOut, FALSE);

  Badging = NULL;
  Status  = gBS->LocateProtocol (&gEfiOEMBadgingProtocolGuid, NULL, (VOID **) &Badging);

  if (GraphicsOutput != NULL) {
    SizeOfX = GraphicsOutput->Mode->Info->HorizontalResolution;
    SizeOfY = GraphicsOutput->Mode->Info->VerticalResolution;

  } else if (UgaDraw != NULL && FeaturePcdGet (PcdUgaConsumeSupport)) {
    Status = UgaDraw->GetMode (UgaDraw, &SizeOfX, &SizeOfY, &ColorDepth, &RefreshRate);
    if (EFI_ERROR (Status)) {
      return EFI_UNSUPPORTED;
    }
  } else {
    return EFI_UNSUPPORTED;
  }

  Instance = 0;
  while (1) {
    ImageData = NULL;
    ImageSize = 0;

    if (Badging != NULL) {
      //
      // Get image from OEMBadging protocol.
      //
      Status = Badging->GetImage (
                          Badging,
                          &Instance,
                          &Format,
                          &ImageData,
                          &ImageSize,
                          &Attribute,
                          &CoordinateX,
                          &CoordinateY
                          );
      if (EFI_ERROR (Status)) {
        break;
      }
    } else {
      //
      // Get the specified image from FV.
      //
      Status = GetSectionFromAnyFv (LogoFile, EFI_SECTION_RAW, 0, (VOID **) &ImageData, &ImageSize);
      if (EFI_ERROR (Status)) {
        return EFI_UNSUPPORTED;
      }

      CoordinateX = 0;
      CoordinateY = 0;
      Attribute   = EfiBadgingDisplayAttributeCenter;
    }

    Blt = NULL;
    Status = TranslateBmpToGopBlt  (
              ImageData,
              ImageSize,
              &Blt,
              &BltSize,
              &Height,
              &Width
              );

//#
//#

    if (EFI_ERROR (Status)) {
      FreePool (ImageData);

      if (Badging == NULL) {
        return Status;
      } else {
        continue;
      }
    }

    //
    // Calculate the display position according to Attribute.
    //
    switch (Attribute) {
    case EfiBadgingDisplayAttributeLeftTop:
      DestX = CoordinateX;
      DestY = CoordinateY;
      break;

    case EfiBadgingDisplayAttributeCenterTop:
      DestX = (SizeOfX - Width) / 2;
      DestY = CoordinateY;
      break;

    case EfiBadgingDisplayAttributeRightTop:
      DestX = (SizeOfX - Width - CoordinateX);
      DestY = CoordinateY;;
      break;

    case EfiBadgingDisplayAttributeCenterRight:
      DestX = (SizeOfX - Width - CoordinateX);
      DestY = (SizeOfY - Height) / 2;
      break;

    case EfiBadgingDisplayAttributeRightBottom:
      DestX = (SizeOfX - Width - CoordinateX);
      DestY = (SizeOfY - Height - CoordinateY);
      break;

    case EfiBadgingDisplayAttributeCenterBottom:
      DestX = (SizeOfX - Width) / 2;
      DestY = (SizeOfY - Height - CoordinateY);
      break;

    case EfiBadgingDisplayAttributeLeftBottom:
      DestX = CoordinateX;
      DestY = (SizeOfY - Height - CoordinateY);
      break;

    case EfiBadgingDisplayAttributeCenterLeft:
      DestX = CoordinateX;
      DestY = (SizeOfY - Height) / 2;
      break;

    case EfiBadgingDisplayAttributeCenter:
      DestX = (SizeOfX - Width) / 2;
      DestY = (SizeOfY - Height) / 2;
      break;

    default:
      DestX = CoordinateX;
      DestY = CoordinateY;
      break;
    }

    if ((DestX >= 0) && (DestY >= 0)) {
      if (GraphicsOutput != NULL) {
        Status = GraphicsOutput->Blt (
                            GraphicsOutput,
                            Blt,
                            EfiBltBufferToVideo,
                            0,
                            0,
                            (UINTN) DestX,
                            (UINTN) DestY,
                            Width,
                            Height,
                            Width * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                            );
      } else if (UgaDraw != NULL && FeaturePcdGet (PcdUgaConsumeSupport)) {
        Status = UgaDraw->Blt (
                            UgaDraw,
                            (EFI_UGA_PIXEL *) Blt,
                            EfiUgaBltBufferToVideo,
                            0,
                            0,
                            (UINTN) DestX,
                            (UINTN) DestY,
                            Width,
                            Height,
                            Width * sizeof (EFI_UGA_PIXEL)
                            );
      } else {
        Status = EFI_UNSUPPORTED;
      }

      //
      // Report displayed Logo information.
      //
      if (!EFI_ERROR (Status)) {
        NumberOfLogos++;

        if (NumberOfLogos == 1) {
          //
          // The first Logo.
          //
          LogoDestX   = (UINTN) DestX;
          LogoDestY   = (UINTN) DestY;
          LogoWidth   = Width;
          LogoHeight  = Height;
        } else {
          //
          // Merge new logo with old one.
          //
          NewDestX  = MIN ((UINTN) DestX, LogoDestX);
          NewDestY  = MIN ((UINTN) DestY, LogoDestY);
          NewWidth  = MAX ((UINTN) DestX + Width, LogoDestX + LogoWidth) - NewDestX;
          NewHeight = MAX ((UINTN) DestY + Height, LogoDestY + LogoHeight) - NewDestY;

          LogoDestX   = NewDestX;
          LogoDestY   = NewDestY;
          LogoWidth   = NewWidth;
          LogoHeight  = NewHeight;
        }
      }
    }

    FreePool (ImageData);

    if (Blt != NULL) {
      FreePool (Blt);
    }

    if (Badging == NULL) {
      break;
    }
  }

  //
  // Set current screen as BGRT logo.
  //
  Status = gBS->LocateProtocol (&gEfiBootLogoProtocolGuid, NULL, (VOID **) &BootLogo);
  if (!EFI_ERROR(Status)) {
    //
    // Ensure the LogoHeight * LogoWidth doesn't overflow
    //
    if (LogoHeight > DivU64x64Remainder ((UINT64) ~0, LogoWidth, NULL)) {
      return EFI_UNSUPPORTED;
    }
    BufferSize = LogoWidth * LogoHeight;

    //
    // Ensure the BufferSize * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL) doesn't overflow
    //
    if (BufferSize > ((UINTN) ~0) / sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)) {
      return EFI_UNSUPPORTED;
    }

    LogoBlt = AllocateZeroPool (BufferSize * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
    if (LogoBlt == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    if (GraphicsOutput != NULL) {
      Status = GraphicsOutput->Blt (
                          GraphicsOutput,
                          LogoBlt,
                          EfiBltVideoToBltBuffer,
                          LogoDestX,
                          LogoDestY,
                          0,
                          0,
                          LogoWidth,
                          LogoHeight,
                          LogoWidth * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                          );
    } else if (UgaDraw != NULL && FeaturePcdGet (PcdUgaConsumeSupport)) {
      Status = UgaDraw->Blt (
                          UgaDraw,
                          (EFI_UGA_PIXEL *) LogoBlt,
                          EfiUgaVideoToBltBuffer,
                          LogoDestX,
                          LogoDestY,
                          0,
                          0,
                          LogoWidth,
                          LogoHeight,
                          LogoWidth * sizeof (EFI_UGA_PIXEL)
                          );
    } else {
      Status = EFI_UNSUPPORTED;
    }

    if (!EFI_ERROR (Status)) {
      Status = BootLogo->SetBootLogo (BootLogo, LogoBlt, LogoDestX, LogoDestY, LogoWidth, LogoHeight);
    }
  }

  return Status;
}

/**
  Use SystemTable Conout to turn on video based Simple Text Out consoles. The
  Simple Text Out screens will now be synced up with all non video output devices

  @retval EFI_SUCCESS     UGA devices are back in text mode and synced up.

**/
EFI_STATUS
EFIAPI
DisableQuietBoot (
  VOID
  )
{

  //
  // Enable Cursor on Screen
  //
  gST->ConOut->EnableCursor (gST->ConOut, TRUE);
  return EFI_SUCCESS;
}


/**

  Show progress bar with title above it. It only works in Graphics mode.


  @param TitleForeground Foreground color for Title.
  @param TitleBackground Background color for Title.
  @param Title           Title above progress bar.
  @param ProgressColor   Progress bar color.
  @param Progress        Progress (0-100)
  @param PreviousValue   The previous value of the progress.

  @retval  EFI_STATUS       Success update the progress bar

**/
EFI_STATUS
EFIAPI
ShowProgress (
  IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL TitleForeground,
  IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL TitleBackground,
  IN CHAR16                        *Title,
  IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL ProgressColor,
  IN UINTN                         Progress,
  IN UINTN                         PreviousValue
  )
{
  EFI_STATUS                     Status;
  EFI_GRAPHICS_OUTPUT_PROTOCOL   *GraphicsOutput;
  EFI_UGA_DRAW_PROTOCOL          *UgaDraw;
  UINT32                         SizeOfX;
  UINT32                         SizeOfY;
  UINT32                         ColorDepth;
  UINT32                         RefreshRate;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL  Color;
  UINTN                          BlockHeight;
  UINTN                          BlockWidth;
  UINTN                          BlockNum;
  UINTN                          PosX;
  UINTN                          PosY;
  UINTN                          Index;

  if (Progress > 100) {
    return EFI_INVALID_PARAMETER;
  }

  UgaDraw = NULL;
  Status = gBS->HandleProtocol (
                  gST->ConsoleOutHandle,
                  &gEfiGraphicsOutputProtocolGuid,
                  (VOID **) &GraphicsOutput
                  );
  if (EFI_ERROR (Status) && FeaturePcdGet (PcdUgaConsumeSupport)) {
    GraphicsOutput = NULL;

    Status = gBS->HandleProtocol (
                    gST->ConsoleOutHandle,
                    &gEfiUgaDrawProtocolGuid,
                    (VOID **) &UgaDraw
                    );
  }
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  SizeOfX = 0;
  SizeOfY = 0;
  if (GraphicsOutput != NULL) {
    SizeOfX = GraphicsOutput->Mode->Info->HorizontalResolution;
    SizeOfY = GraphicsOutput->Mode->Info->VerticalResolution;
  } else if (UgaDraw != NULL) {
    Status = UgaDraw->GetMode (
                        UgaDraw,
                        &SizeOfX,
                        &SizeOfY,
                        &ColorDepth,
                        &RefreshRate
                        );
    if (EFI_ERROR (Status)) {
      return EFI_UNSUPPORTED;
    }
  } else {
    return EFI_UNSUPPORTED;
  }

  BlockWidth  = SizeOfX / 100;
  BlockHeight = SizeOfY / 50;

  BlockNum    = Progress;

  PosX        = 0;
  PosY        = SizeOfY * 48 / 50;

  if (BlockNum == 0) {
    //
    // Clear progress area
    //
    SetMem (&Color, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0x0);

    if (GraphicsOutput != NULL) {
      Status = GraphicsOutput->Blt (
                          GraphicsOutput,
                          &Color,
                          EfiBltVideoFill,
                          0,
                          0,
                          0,
                          PosY - EFI_GLYPH_HEIGHT - 1,
                          SizeOfX,
                          SizeOfY - (PosY - EFI_GLYPH_HEIGHT - 1),
                          SizeOfX * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                          );
    } else if (FeaturePcdGet (PcdUgaConsumeSupport)) {
      Status = UgaDraw->Blt (
                          UgaDraw,
                          (EFI_UGA_PIXEL *) &Color,
                          EfiUgaVideoFill,
                          0,
                          0,
                          0,
                          PosY - EFI_GLYPH_HEIGHT - 1,
                          SizeOfX,
                          SizeOfY - (PosY - EFI_GLYPH_HEIGHT - 1),
                          SizeOfX * sizeof (EFI_UGA_PIXEL)
                          );
    } else {
      return EFI_UNSUPPORTED;
    }
  }
  //
  // Show progress by drawing blocks
  //
  for (Index = PreviousValue; Index < BlockNum; Index++) {
    PosX = Index * BlockWidth;
    if (GraphicsOutput != NULL) {
      Status = GraphicsOutput->Blt (
                          GraphicsOutput,
                          &ProgressColor,
                          EfiBltVideoFill,
                          0,
                          0,
                          PosX,
                          PosY,
                          BlockWidth - 1,
                          BlockHeight,
                          (BlockWidth) * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                          );
    } else if (FeaturePcdGet (PcdUgaConsumeSupport)) {
      Status = UgaDraw->Blt (
                          UgaDraw,
                          (EFI_UGA_PIXEL *) &ProgressColor,
                          EfiUgaVideoFill,
                          0,
                          0,
                          PosX,
                          PosY,
                          BlockWidth - 1,
                          BlockHeight,
                          (BlockWidth) * sizeof (EFI_UGA_PIXEL)
                          );
    } else {
      return EFI_UNSUPPORTED;
    }
  }

  PrintXY (
    (SizeOfX - StrLen (Title) * EFI_GLYPH_WIDTH) / 2,
    PosY - EFI_GLYPH_HEIGHT - 1,
    &TitleForeground,
    &TitleBackground,
    Title
    );

  return EFI_SUCCESS;
}
