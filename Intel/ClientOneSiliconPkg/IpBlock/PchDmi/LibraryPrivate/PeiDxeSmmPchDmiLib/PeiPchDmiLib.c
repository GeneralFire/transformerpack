/** @file
  This files contains PCH DMI services for RCs usage

@copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation.

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
#include <IndustryStandard/PciExpress21.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PeiPchDmiLib.h>
#include <Register/PchDmiRegs.h>
#include <Register/PchPcrRegs.h>
#include <Register/PchDmi17Regs.h>

#include "PchDmi14.h"
#include "PchDmi15.h"

#define PCH_DMI_MPSS PchPcieMaxPayload256  // According to EDS 256B supported


/**
  Get PCH default TC VC Mapping settings. This funciton returns the default PCH setting
  System Agent can update the settings according to polices.

  @param[in, out] PchDmiTcVcMap         Buffer for PCH_DMI_TC_VC_MAP instance.

**/
VOID
PchDmiTcVcMapInit (
  IN OUT  PCH_DMI_TC_VC_MAP             *PchDmiTcVcMap
  )
{
  if (PchDmiTcVcMap == NULL) {
    return;
  }

  PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc0].Vc   = PchDmiVcTypeVc0;
  PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc0].TcId = PchDmiTcTypeTc0;
  PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc1].Vc   = PchDmiVcTypeVc1;
  PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc1].TcId = PchDmiTcTypeTc1;
  if (IsAdlPch () && IsPchS ()) {
    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc2].Vc   = PchDmiVcTypeVc0;
    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc2].TcId = PchDmiTcTypeTc2;
    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc3].Vc   = PchDmiVcTypeVc0;
    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc3].TcId = PchDmiTcTypeTc3;
    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc4].Vc   = PchDmiVcTypeVc0;
    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc4].TcId = PchDmiTcTypeTc4;
    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc5].Vc   = PchDmiVcTypeVc0;
    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc5].TcId = PchDmiTcTypeTc5;
    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc6].Vc   = PchDmiVcTypeVc0;
    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc6].TcId = PchDmiTcTypeTc6;
  }
  PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc7].Vc   = PchDmiVcTypeVcm;
  PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc7].TcId = PchDmiTcTypeTc7;

  PchDmiTcVcMap->DmiVc[PchDmiVcTypeVc0].Enable          = TRUE;
  PchDmiTcVcMap->DmiVc[PchDmiVcTypeVc0].VcId            = 0;
  PchDmiTcVcMap->DmiVc[PchDmiVcTypeVc1].Enable          = TRUE;
  PchDmiTcVcMap->DmiVc[PchDmiVcTypeVc1].VcId            = 1;
  PchDmiTcVcMap->DmiVc[PchDmiVcTypeVcm].Enable          = TRUE;
  PchDmiTcVcMap->DmiVc[PchDmiVcTypeVcm].VcId            = 7;
}

/**
  Programing transaction classes of the corresponding virtual channel and Enable it

  @param[in] Vc                   The virtual channel number for programming
  @param[in] VcId                 The Identifier to be used for this virtual channel
  @param[in] VcMap                The transaction classes are mapped to this virtual channel.
                                  When a bit is set, this transaction class is mapped to the virtual channel

  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_SUCCESS             The function completed successfully
**/
STATIC
VOID
SetPchDmiTcVcMapping (
  IN   PCH_DMI_VC_TYPE  Vc,
  IN   UINT8            VcId,
  IN   UINT8            VcMap
  )
{
  UINT32         VxCtlAnd = 0;
  UINT32         VxCtlOr = 0;
  UINT16         VcCtlAddress;
  UINT16         VcStsAddress;

  if (IsAdlPch () && IsPchS ()) {
    PchDmi17VcRegs (Vc, &VcCtlAddress, &VcStsAddress);
    switch (Vc) {
      case PchDmiVcTypeVc0:
      VxCtlAnd  = (UINT32) (~(B_PCH_DMI_PCR_V0CTL_ID | V_PCH_DMI_PCR_V0CTL_TVM_MASK));
      VxCtlOr   = ((UINT32) VcId << N_PCH_DMI_PCR_V0CTL_ID) & B_PCH_DMI_PCR_V0CTL_ID;
      VxCtlOr |= (UINT32) VcMap;
      VxCtlOr |= B_PCH_DMI_PCR_V0CTL_EN;
      break;

    case PchDmiVcTypeVc1:
      VxCtlAnd  = (UINT32) (~(B_PCH_DMI_PCR_V1CTL_ID | V_PCH_DMI_PCR_V1CTL_TVM_MASK));
      VxCtlOr   = ((UINT32) VcId << N_PCH_DMI_PCR_V1CTL_ID) & B_PCH_DMI_PCR_V1CTL_ID;
      VxCtlOr |= (UINT32) VcMap;
      VxCtlOr |= B_PCH_DMI_PCR_V1CTL_EN;
      break;

    case PchDmiVcTypeVcm:
      VxCtlAnd  = (UINT32) (~(B_PCH_DMI_PCR_V1CTL_ID | V_PCH_DMI_PCR_V1CTL_TVM_MASK));
      VxCtlOr   = ((UINT32) VcId << N_PCH_DMI_PCR_V1CTL_ID) & B_PCH_DMI_PCR_V1CTL_ID;
      VxCtlOr |= (UINT32) VcMap;
      VxCtlOr |= B_PCH_DMI_PCR_V1CTL_EN;
      break;

    case PchDmiVcTypeMax:
      ASSERT (FALSE);
    }
    ConfigurePchDmi17MultiVC (Vc);
  } else {
    if (IsPchWithPdmi ()) {
      PchDmi15VcRegs (Vc, &VcCtlAddress, &VcStsAddress);
    } else {
      PchDmi14VcRegs (Vc, &VcCtlAddress, &VcStsAddress);
    }
    VxCtlAnd  = (UINT32) (~(B_PCH_DMI_PCR_V1CTL_ID | V_PCH_DMI_PCR_V1CTL_TVM_MASK));
    VxCtlOr   = ((UINT32) VcId << N_PCH_DMI_PCR_V1CTL_ID) & B_PCH_DMI_PCR_V1CTL_ID;
    VxCtlOr |= (UINT32) VcMap;
    VxCtlOr |= B_PCH_DMI_PCR_V1CTL_EN;
  }

  PchPcrAndThenOr32 (PID_DMI, VcCtlAddress, VxCtlAnd, VxCtlOr);
  //
  // Reads back for posted write to take effect
  // Read back is done in PchPcr lib
  //
}

/**
  Polling negotiation status of the corresponding virtual channel

  @param[in] Vc                   The virtual channel number for programming

  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_SUCCESS             The function completed successfully
**/
STATIC
VOID
PollPchDmiVcStatus (
  IN PCH_DMI_VC_TYPE   Vc
  )
{
  UINT16         VcCtlAddress;
  UINT16         VcStsAddress;

  if (IsPchWithPdmi ()) {
    PchDmi15VcRegs (Vc, &VcCtlAddress, &VcStsAddress);
  } else {
    PchDmi14VcRegs (Vc, &VcCtlAddress, &VcStsAddress);
  }

  //
  // Wait for negotiation to complete
  //
  while ((PchPcrRead16 (PID_DMI, VcStsAddress) & B_PCH_DMI_PCR_V1STS_NP) != 0) {
  }
}


/**
  The function performing TC/VC mapping program, and poll all PCH Virtual Channel
  until negotiation completion.

  @param[in] PchDmiTcVcMap              Buffer for PCH_DMI_TC_VC_MAP instance.

  @retval EFI_SUCCESS                   The function completed successfully
  @retval Others                        All other error conditions encountered result in an ASSERT.
**/
EFI_STATUS
PchDmiTcVcProgPoll (
  IN  PCH_DMI_TC_VC_MAP                 *PchDmiTcVcMap
  )
{
  UINT8                   Index;
  UINT8                   VcMap[PchDmiVcTypeMax];

  ZeroMem (VcMap, PchDmiVcTypeMax);

  ///
  /// Set the TC/VC mappings
  ///
  for (Index = 0; Index < PchDmiTcTypeMax; Index++) {
    DEBUG ((DEBUG_INFO, "TC:%0x VC:%0x!\n", PchDmiTcVcMap->DmiTc[Index].TcId, PchDmiTcVcMap->DmiTc[Index].Vc));
    VcMap[PchDmiTcVcMap->DmiTc[Index].Vc] |= (BIT0 << PchDmiTcVcMap->DmiTc[Index].TcId);
  }

  for (Index = 0; Index < PchDmiVcTypeMax; Index++) {
    DEBUG ((DEBUG_INFO, "VC:%0x VCID:%0x Enable:%0x!\n",Index, PchDmiTcVcMap->DmiVc[Index].VcId, PchDmiTcVcMap->DmiVc[Index].Enable));
    if (PchDmiTcVcMap->DmiVc[Index].Enable == TRUE) {
      SetPchDmiTcVcMapping (
        Index,
        PchDmiTcVcMap->DmiVc[Index].VcId,
        VcMap[Index]
        );
    }
  }

  ///
  /// After both above and System Agent DMI TC/VC mapping are programmed,
  /// poll VC negotiation pending status until is zero:
  ///
  for (Index = 0; Index < PchDmiVcTypeMax; Index++) {
    if (PchDmiTcVcMap->DmiVc[Index].Enable == TRUE) {
      PollPchDmiVcStatus (Index);
    }
  }

  return EFI_SUCCESS;
}


/**
  This function provides Max Payload Size Supported by PCH in DMI.

  @return A value encoded in PCIe Capabilities is returned, i.e. 1 - 128B, 2 - 256B, 3 - 512B, etc.
**/
UINT8
PchDmiGetMaxPayloadSizeSupported (
  VOID
  )
{
  return PCH_DMI_MPSS;
}


/**
  This function reads Supported Link Speeds Vector from Link Capabilities 2 register.

  @return A bitmap of supported link speeds (gens) is returned.
**/
UINT8
PchDmiGetSupportedLinkSpeedsVector (
  VOID
  )
{
  PCI_REG_PCIE_LINK_CAPABILITY2 LinkCap2;

  LinkCap2.Uint32 = PchPcrRead32 (PID_DMI, R_PCH_DMI17_PCR_LCAP2);
  return (UINT8)LinkCap2.Bits.LinkSpeedsVector;
}


/**
  The function set the Target Link Speed in PCH to DMI GEN 3.

  @param[in] TargetLinkSpeed        Target Link Speed
                                    2: GEN2
                                    3: GEN3
**/
VOID
PchDmiSetTargetLinkSpeed (
  IN  UINT8                 TargetLinkSpeed
  )
{
  DEBUG ((DEBUG_INFO, "PchDmiSetTargetLinkSpeed(%d) Start\n", TargetLinkSpeed));
  if (IsPchWithPdmi () || (IsAdlPch () && IsPchS ())) {
    PchDmi15SetTargetLinkSpeed (TargetLinkSpeed);
  }
}

#define MAX_RANGE_INDEX 2 // Only 3 GPMR registers are available

/**
  Makes memory cycles that fall within the given range inclusive to be
  forwarded using source decode to the specified Destination ID

  @param[in]    RangeIndex      Index of which General Purpose Memory
                                Range shall be configured
  @param[in]    RangeBase       Base of Source Decode Range, bits[15:0]
                                of range are assumed to be 0000h
  @param[in]    RangeLimit      Limit of Source Decode range, bits[15:0]
                                of range are assumed to be FFFFh
  @param[in]    DestinationId   The destination ID to be used to forward
                                the cycle decode

  @retval   EFI_SUCCESS               Range was programmed and enabled
  @retval   EFI_INVALID_PARAMETER     One of arguments is incorrect
**/
EFI_STATUS
PchDmiSetGeneralPurposeMemoryRange (
  UINT8     RangeIndex,
  UINT32    RangeBase,
  UINT32    RangeLimit,
  UINT32    DestinationId
  )
{
  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  if (RangeIndex > MAX_RANGE_INDEX) {
    DEBUG ((
      DEBUG_ERROR,
      "RangeIndex is invalid. Max value is %d but requested to configure range %d.\n",
      MAX_RANGE_INDEX,
      RangeIndex
      ));
    return EFI_INVALID_PARAMETER;
  }

  // Check for valid range
  if (RangeBase >= RangeLimit) {
    DEBUG ((
      DEBUG_ERROR,
      "Invalid range for configuration. Range base must me smaller than range limit.\n"
    ));
    return EFI_INVALID_PARAMETER;
  }

  // Program range into General Purpose Memory Range X (where X is desired index)
  PchPcrWrite32 (
    PID_DMI,
    R_PCH_DMI_PCR_GPMR1 + (RangeIndex * 8),
    (UINT32) ((RangeLimit & 0xFFFF0000) | ((RangeBase >> 16) & 0xFFFF))
    );

  // Program source decode enable bit along with the Destination ID
  PchPcrWrite32 (
    PID_DMI,
    R_PCH_DMI_PCR_GPMR1DID + (RangeIndex * 8),
    (UINT32) ((DestinationId & 0x7FFFFFFF) | B_PCH_DMI_PCR_GPMRXDID_GPMR1DE)
    );
  return EFI_SUCCESS;
}
