/** @file
  Header file for Graphics Private Info Lib implementation.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#ifndef _GRAPHICS_INFO_LIB_H_
#define _GRAPHICS_INFO_LIB_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci22.h>
#include <Register/SaRegsHostBridge.h>

#define GT_WAIT_TIMEOUT          3000000  ///< ~3 seconds

/**
  This function will check if Bus Master and Memory access on 0:2:0 is enabled or not

  @retval TRUE          IGD Bus Master Access and Memory Space access is Enabled
  @retval FALSE         IGD Bus Master Access and Memory Space access is Disable
**/
BOOLEAN
IgfxCmdRegEnabled (
  VOID
  );


/**
  GetIgdBusNumber: Get IGD Bus Number

  @retval PCI bus number for IGD
**/
UINT8
GetIgdBusNumber (
  VOID
  );

/**
  GetIgdDevNumber: Get IGD Dev Number

  @retval PCI dev number for IGD
**/
UINT8
GetIgdDevNumber (
  VOID
  );

/**
  GetIgdFunNumber: Get IGD Fun Number

  @retval PCI fun number for IGD
**/
UINT8
GetIgdFuncNumber (
  VOID
  );


/**
  GetGttMmAdrOffset: Get GTTMMADR Offset value

  @retval PCI Offset for GTTMMADR
**/
UINT8
GetGttMmAdrOffset (
  VOID
  );

/**
  GetGmAdrOffset: Get GMADR Offset value

  @retval PCI Offset for GMADR
**/
UINT8
GetGmAdrOffset (
  VOID
  );

/**
  GetIgdMsacOffset: Get IGD MSAC Offset value

  @retval PCI Offset for IGD Aperture size.
**/
UINT8
GetIgdMsacOffset (
  VOID
  );

/**
  GetIgdDssmRefClockFreqValue: Read DSSM (Display Strap State) register to get display reference clock frequence value

  @param[in] GttMmAdr         IGD MMIO Base address value

  @retval IGD Display Reference clock frequency value
**/
UINT32
GetIgdDssmRefClockFreqValue (
  IN  UINTN                GttMmAdr
  );

/**
  "Poll Status" for GT Readiness

  @param[in] Base            - Base address of MMIO
  @param[in] Offset          - MMIO Offset
  @param[in] Mask            - Mask
  @param[in] Result          - Value to wait for

  @retval EFI_SUCCESS        - Wait Bit Successfully set
  @retval EFI_TIMEOUT        - Timed out
**/
EFI_STATUS
PollGtReady (
  IN       UINT64                  Base,
  IN       UINT32                  Offset,
  IN       UINT32                  Mask,
  IN       UINT32                  Result
  );

/**
  This function will check if Intel Graphics is Enabled or Supported

  @retval TRUE          Graphics Enabled or Supported
  @retval FALSE         Graphics not Enabled or not Supported
**/
BOOLEAN
IsIGfxSupported (
  VOID
  );

/**
  This function will Return the Graphics Memory Stolen Size in MB

  @retval GsmSize          Graphics Memory Stolen Size
**/
UINT32
EFIAPI
GetIgdGmsSizeInMb (
  VOID
  );

/**
  This function will check if RC6 Context Base is locked or not on IGD

  @param[in] GttMmAdr         IGD MMIO Base address value

  @retval TRUE          RC6 is Locked
  @retval FALSE         RC6 is not locked
**/
BOOLEAN
IsRc6Locked (
  IN  UINTN     GttMmAdr
  );

/**
  This function will check if Doorbell is in Range or not on IGD

  @param[in] GttMmAdr         IGD MMIO Base address value

  @retval TRUE          Doorbell is in Range
  @retval FALSE         Doorbell is not in Range
**/
BOOLEAN
IsDoorbellRangeValid (
  IN  UINTN     GttMmAdr
  );

/**
  This function will check if RC6 is in Range or not on IGD.

  @param[in] GttMmAdr         IGD MMIO Base address value

  @retval TRUE          RC6 is in Range
  @retval FALSE         RC6 is not in Range
**/
BOOLEAN
IsRc6RangeValid (
  IN  UINTN     GttMmAdr
  );

/**
  This function will check if PAVP is in Range or not on IGD.

  @param[in] GttMmAdr         IGD MMIO Base address value

  @retval TRUE          RC6 is in Range
  @retval FALSE         RC6 is not in Range
**/
BOOLEAN
IsWoPcmRangeValid (
  IN  UINTN     GttMmAdr
  );

  /**
  This function will check if WOPCM base is consistent in different memory scopes.

  @param[in] GttMmAdr         IGD MMIO Base address value

  @retval TRUE          WOPCM Base is consistent
  @retval FALSE         WOPCM Base is not consistent
**/
BOOLEAN
IsWoPcmBaseRegistersConsistent (
  IN  UINTN     GttMmAdr
  );

/**
  This function will check if PAVP is locked in different memory scopes.

  @param[in] GttMmAdr         IGD MMIO Base address value

  @retval TRUE          PAVP is locked
  @retval FALSE         PAVP is not locked
**/
BOOLEAN
IsPavpBitLocked (
  IN  UINTN     GttMmAdr
  );
#endif
