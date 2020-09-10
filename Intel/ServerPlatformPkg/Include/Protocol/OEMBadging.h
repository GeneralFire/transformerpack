/** @file
  The OEM Badging Protocol defines the interface to get the OEM badging
  image with the display attribute. This protocol can be produced based on OEM badging images.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2015 Intel Corporation. <BR>

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

#ifndef __EFI_OEM_BADGING_H__
#define __EFI_OEM_BADGING_H__

//
// GUID for EFI OEM Badging Protocol
//
#define EFI_OEM_BADGING_PROTOCOL_GUID \
  { 0x170e13c0, 0xbf1b, 0x4218, {0x87, 0x1d, 0x2a, 0xbd, 0xc6, 0xf8, 0x87, 0xbc } }


typedef struct _EFI_OEM_BADGING_PROTOCOL EFI_OEM_BADGING_PROTOCOL;

typedef enum {
  EfiBadgingFormatBMP,
  EfiBadgingFormatJPEG,
  EfiBadgingFormatTIFF,
  EfiBadgingFormatGIF,
  EfiBadgingFormatUnknown
} EFI_BADGING_FORMAT;

typedef enum {
  EfiBadgingDisplayAttributeLeftTop,
  EfiBadgingDisplayAttributeCenterTop,
  EfiBadgingDisplayAttributeRightTop,
  EfiBadgingDisplayAttributeCenterRight,
  EfiBadgingDisplayAttributeRightBottom,
  EfiBadgingDisplayAttributeCenterBottom,
  EfiBadgingDisplayAttributeLeftBottom,
  EfiBadgingDisplayAttributeCenterLeft,
  EfiBadgingDisplayAttributeCenter,
  EfiBadgingDisplayAttributeCustomized
} EFI_BADGING_DISPLAY_ATTRIBUTE;

/**

  Load an OEM badge image and return its data and attributes.

  @param This              The pointer to this protocol instance.
  @param Instance          The visible image instance is found.
  @param Format            The format of the image. Examples: BMP, JPEG.
  @param ImageData         The image data for the badge file. Currently only
                           supports the .bmp file format.
  @param ImageSize         The size of the image returned.
  @param Attribute         The display attributes of the image returned.
  @param CoordinateX       The X coordinate of the image.
  @param CoordinateY       The Y coordinate of the image.

  @retval EFI_SUCCESS      The image was fetched successfully.
  @retval EFI_NOT_FOUND    The specified image could not be found.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_BADGING_GET_IMAGE)(
  IN     EFI_OEM_BADGING_PROTOCOL          *This,
  IN OUT UINT32                            *Instance,
     OUT EFI_BADGING_FORMAT                *Format,
     OUT UINT8                             **ImageData,
     OUT UINTN                             *ImageSize,
     OUT EFI_BADGING_DISPLAY_ATTRIBUTE     *Attribute,
     OUT UINTN                             *CoordinateX,
     OUT UINTN                             *CoordinateY
);


struct _EFI_OEM_BADGING_PROTOCOL {
  EFI_BADGING_GET_IMAGE       GetImage;
};


extern EFI_GUID gEfiOEMBadgingProtocolGuid;

#endif
