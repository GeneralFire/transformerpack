/** @file
  Pch information library.

  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation.

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
#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include "PchInfoLibPrivate.h"
#include <Register/PchRegs.h>
#include <Register/PchRegsLpc.h>
#include <PchPcieRpInfo.h>
#include <IndustryStandard/Pci30.h>
#include <Library/PchPciBdfLib.h>

/**
  Return LPC Device Id

  @retval PCH_LPC_DEVICE_ID         PCH Lpc Device ID
**/
UINT16
PchGetLpcDid (
  VOID
  )
{
  UINT64  LpcBaseAddress;

  LpcBaseAddress = LpcPciCfgBase ();

  return PciSegmentRead16 (LpcBaseAddress + PCI_DEVICE_ID_OFFSET);
}

/**
  Return Pch Series

  @retval PCH_SERIES            Pch Series
**/
PCH_SERIES
PchSeries (
  VOID
  )
{
  PCH_SERIES        PchSer;
  static PCH_SERIES PchSeries = PCH_UNKNOWN_SERIES;

  if (PchSeries != PCH_UNKNOWN_SERIES) {
    return PchSeries;
  }

  PchSer = PchSeriesFromLpcDid (PchGetLpcDid ());

  PchSeries = PchSer;

  return PchSer;
}

/**
  Return Pch stepping type

  @retval PCH_STEPPING            Pch stepping type
**/
PCH_STEPPING
PchStepping (
  VOID
  )
{
  UINT8                RevId;
  UINT64               LpcBaseAddress;
  static PCH_STEPPING  PchStepping = PCH_STEPPING_MAX;

  if (PchStepping != PCH_STEPPING_MAX) {
    return PchStepping;
  }

  LpcBaseAddress = LpcPciCfgBase ();
  RevId = PciSegmentRead8 (LpcBaseAddress + PCI_REVISION_ID_OFFSET);

  RevId = PchSteppingFromRevId (RevId);

  PchStepping = RevId;

  return RevId;
}

/**
  Determine if PCH is supported

  @retval TRUE                    PCH is supported
  @retval FALSE                   PCH is not supported
**/
BOOLEAN
IsPchSupported (
  VOID
  )
{
  UINT16         LpcDeviceId;
  UINT16         LpcVendorId;
  UINT64         LpcBaseAddress;

  LpcBaseAddress = LpcPciCfgBase ();

  LpcDeviceId = PciSegmentRead16 (LpcBaseAddress + PCI_DEVICE_ID_OFFSET);
  LpcVendorId = PciSegmentRead16 (LpcBaseAddress + PCI_VENDOR_ID_OFFSET);

  ///
  /// Verify that this is a supported chipset
  ///
  if ((LpcVendorId == V_PCH_INTEL_VENDOR_ID) && (PchSeries () != PCH_UNKNOWN_SERIES)) {
    return TRUE;
  } else {
    DEBUG ((DEBUG_ERROR, "PCH code doesn't support the LpcDeviceId: 0x%04x!\n", LpcDeviceId));
    return FALSE;
  }
}

/**
  Check if this is PCH LP series

  @retval TRUE                It's PCH LP series
  @retval FALSE               It's not PCH LP series
**/
BOOLEAN
IsPchLp (
  VOID
  )
{
  return (PchSeries () == PCH_LP);
}

/**
  Check if this is PCH P series

  @retval TRUE                It's PCH P series
  @retval FALSE               It's not PCH P series
**/
BOOLEAN
IsPchP (
  VOID
  )
{
  return (PchSeries () == PCH_P);
}

/**
  Check if this is PCH M series

  @retval TRUE                It's PCH M series
  @retval FALSE               It's not PCH M series
**/
BOOLEAN
IsPchM (
  VOID
  )
{
  return (PchSeries () == PCH_M);
}

/**
  Check if this is PCH S series

  @retval TRUE                It's PCH S series
  @retval FALSE               It's not PCH S series
**/
BOOLEAN
IsPchS (
  VOID
  )
{
  return (PchSeries () == PCH_S);
}

/**
  Check if this is CDF PCH generation

  @retval TRUE                It's CDF PCH
  @retval FALSE               It's not CDF PCH
**/
BOOLEAN
IsCdfPch (
  VOID
  )
{
  return (PchGeneration () == CDF_PCH);
}


/**
  Check if this is EBG PCH generation

  @retval TRUE                It's EBG PCH
  @retval FALSE               It's not EBG PCH
**/
BOOLEAN
IsEbgPch (
  VOID
  )
{
  return (PchGeneration () == EBG_PCH);
}

/**
  Check if this is CNL PCH generation

  @retval TRUE                It's CNL PCH
  @retval FALSE               It's not CNL PCH
**/
BOOLEAN
IsCnlPch (
  VOID
  )
{
  return (PchGeneration () == CNL_PCH);
}

/**
  Check if this is ICL PCH generation

  @retval TRUE                It's ICL PCH
  @retval FALSE               It's not ICL PCH
**/
BOOLEAN
IsIclPch (
  VOID
  )
{
  return (PchGeneration () == ICL_PCH);
}

/**
  Check if this is TGL PCH generation

  @retval TRUE                It's TGL PCH
  @retval FALSE               It's not TGL PCH
**/
BOOLEAN
IsTglPch (
  VOID
  )
{
  return (PchGeneration () == TGL_PCH);
}

/**
  Check if this is JSL PCH generation

  @retval TRUE                It's JSL PCH
  @retval FALSE               It's not JSL PCH
**/
BOOLEAN
IsJslPch (
  VOID
  )
{
  return (PchGeneration () == JSL_PCH);
}

/**
  Check if this is ADL PCH generation

  @retval TRUE                It's ADL PCH
  @retval FALSE               It's not ADL PCH
**/
BOOLEAN
IsAdlPch (
  VOID
  )
{
  return (PchGeneration () == ADL_PCH);
}

/**
  Check if this is MTL SOC generation

  @retval TRUE                It's MTL SOC
  @retval FALSE               It's not MTL SOC
**/
BOOLEAN
IsMtlSoc (
  VOID
  )
{
  return (PchGeneration () == MTL_SOC);
}

/**
  Get PCH stepping ASCII string.
  Function determines major and minor stepping versions and writes them into a buffer.
  The return string is zero terminated

  @param [out]     Buffer               Output buffer of string
  @param [in]      BufferSize           Buffer size.
                                        Must not be less then PCH_STEPPING_STR_LENGTH_MAX

  @retval EFI_SUCCESS                   String copied successfully
  @retval EFI_INVALID_PARAMETER         The stepping is not supported, or parameters are NULL
  @retval EFI_BUFFER_TOO_SMALL          Input buffer size is too small
**/
EFI_STATUS
PchGetSteppingStr (
  OUT    CHAR8                          *Buffer,
  IN     UINT32                         BufferSize
  )
{
  PCH_STEPPING PchStep;

  PchStep = PchStepping ();

  if ((Buffer == NULL) || (BufferSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }
  if (BufferSize < PCH_STEPPING_STR_LENGTH_MAX) {
    return EFI_BUFFER_TOO_SMALL;
  }

  PchPrintSteppingStr (Buffer, BufferSize, PchStep);

  return EFI_SUCCESS;
}

/**
  Get PCH Sku ASCII string
  The return string is zero terminated.

  @retval Static ASCII string of PCH Sku
**/
CHAR8*
PchGetSkuStr (
  VOID
  )
{
  UINTN          Index;
  UINT16         LpcDid;

  LpcDid = PchGetLpcDid ();

  for (Index = 0; mSkuStrs[Index].Id != 0xFFFF; Index++) {
    if (LpcDid == mSkuStrs[Index].Id) {
      return mSkuStrs[Index].String;
    }
  }

  return "Undefined SKU";
}

/**
  Get Pch Maximum Pcie Controller Number

  @retval Pch Maximum Pcie Root Port Number
**/
UINT8
GetPchMaxPcieControllerNum (
  VOID
  )
{
  return GetPchMaxPciePortNum () / PCH_PCIE_CONTROLLER_PORTS;
}

/**
  return support status for P2SB PCR 20-bit addressing

  @retval    TRUE
  @retval    FALSE
**/
BOOLEAN
IsP2sb20bPcrSupported (
  VOID
  )
{
  if (IsPchS ()) {
    return TRUE;
  } else {
    return FALSE;
  }
}