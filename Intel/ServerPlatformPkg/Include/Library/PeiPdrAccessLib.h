/** @file
  PeiPdrAccess Liarary Definition Header File.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2017 Intel Corporation. <BR>

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

#ifndef _PDR_ACCESS_PEI_LIB_H_
#define _PDR_ACCESS_PEI_LIB_H_

#include <Library/PeiServicesLib.h>
#include <Guid/Pdr.h>


typedef enum {
 RTC_SECONDS = 0,   // R/W  Range 0..59
 RTC_MINUTES = 2,   // R/W  Range 0..59
 RTC_HOURS   = 4,   // R/W  Range 1..12 or 0..23 Bit 7 is AM/PM
 RTC_DAY     = 7,   // R/W  Range 1..31
 RTC_MONTH   = 8,   // R/W  Range 1..12
 RTC_YEAR    = 9,   // R/W  Range 0..99
} RTC_ADDRESS;

/**
  Update PlatformDataRegion information, not only for UUID. This routine reads the PDR information - 64KB size to memory buffer,
  and backup the PDR data, update memory buffer and then write back to PDR.

  @param  PeiServices      Describes the list of possible PEI Services.
  @param  Buffer           Data to update.
  @param  DestOffset       Region to update, the offset of PDR.
  @param  BufferSize       The size of data to update.

  @return EFI_STATUS

**/
EFI_STATUS
UpdatePlatDataArea (
  IN        CONST EFI_PEI_SERVICES          **PeiServices,
  IN        UINT8                           *Buffer,
  IN        UINT32                          DestOffset,
  IN        UINT32                          BufferSize
);

/**
  Read PDR Information.

  @param  PeiServices      Describes the list of possible PEI Services.
  @param  Pdr              A pointer to PLATFORM_PDR_TABLE, which contains PDR information.

  @return EFI_STATUS

**/
EFI_STATUS
PlatDataRegRead (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT PLATFORM_PDR_TABLE        *Pdr
  );

/**
  Read UUID from PDR and copy the value into the address provided.

  @param[in]  PeiServices   Describes the list of possible PEI Services.
  @param[out] UuidPtr       A pointer to UUID info.

  @retval EFI_SUCCESS   Function completed successfully.
  @retval others        An error occured.

**/
EFI_STATUS
UuidGetByPdr (
  IN  CONST EFI_PEI_SERVICES     **PeiServices,
  OUT EFI_GUID                   *UuidPtr
  );

/**
  The RP cannot have a UUID value of all 0's or F's for PXE PDK tests.

  @param[in]  PeiServices   Describes the list of possible PEI Services.
  @param[out] UuidPtr       A pointer to UUID.

  @retval EFI_SUCCESS   Function completed successfully.
  @retval others        An error occured.

**/
EFI_STATUS
RpUuidRefresh (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN OUT EFI_GUID               *UuidPtr
  );

/**
  Get variable MfgSystemUuid, which was programmed by tool in MFG Mode.

  @param[in]  PeiServices   Describes the list of possible PEI Services.
  @param[out] UuidPtr       A pointer to variable MfgSystemUuid.

  @retval EFI_SUCCESS   Function completed successfully.
  @retval others        An error occured.

**/
EFI_STATUS
GetMfgSystemUuidVar (
  IN EFI_PEI_SERVICES           **PeiServices,
  OUT EFI_GUID                  *UuidPtr
  );

/**
  Check for variable containing the Mfg UUID, if exists, update PDR with new UUID,
  if not found, get UUID from PDR, and then sync to PlatformInfoHob.

  @param[in]  PeiServices   Describes the list of possible PEI Services.
  @param[out] UuidPtr       A pointer to UUID value.

  @retval EFI_SUCCESS   Function completed successfully.
  @retval others        An error occured.

**/
EFI_STATUS
MfgUuidGet (
  IN EFI_PEI_SERVICES           **PeiServices,
  OUT EFI_GUID                  *UuidPtr
  );

/**
  Read RTC data register and return contents after converting it to decimal.
  Needed here because it looks like other methods of getting time (ex. gRT->GetTime) tend to fail.

  @param Address RTC Register address to read
  @retval Value of data register contents converted to Decimal

**/
UINT8
RtcReadDec (
  IN RTC_ADDRESS Address
  );

#endif //_PDR_ACCESS_PEI_LIB_H_
