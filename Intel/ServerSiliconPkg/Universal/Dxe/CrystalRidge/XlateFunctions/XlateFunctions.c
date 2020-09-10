/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#include <Library/UefiLib.h>
#include <Library/DebugLib.h>

#include "CrystalRidge.h"
#include "XlateFunctions/XlateFunctions.h"


extern CONST UINT32 Imc1Way4k_2ChWay4k_LineMultipliers[2][2];
extern CONST UINT32 Imc1Way4k_2ChWay256_LineMultipliers[2][8];
extern CONST UINT32 Imc2Way4k_2ChWay4k_LineMultipliers[4][2];
extern CONST UINT32 Imc2Way4k_2ChWay256_LineMultipliers[4][8];
extern CONST UINT32 Imc4Way4k_2ChWay4k_LineMultipliers[8][2];
extern CONST UINT32 Imc4Way4k_2ChWay256_LineMultipliers[8][8];
extern CONST UINT32 Imc8Way4k_2ChWay256_LineMultipliers[16][8];
extern CONST UINT32 Imc1Way4k_3ChWay4k_LineMultipliers[3][2];
extern CONST UINT32 Imc1Way4k_3ChWay256_LineMultipliers[3][16];
extern CONST UINT32 Imc2Way4k_3ChWay4k_LineMultipliers[6][2];
extern CONST UINT32 Imc2Way4k_3ChWay256_LineMultipliers[6][16];
extern CONST UINT32 Imc4Way4k_3ChWay4k_LineMultipliers[12][2];
extern CONST UINT32 Imc4Way4k_3ChWay256_LineMultipliers[12][16];
extern CONST UINT32 Imc8Way4k_3ChWay4k_LineMultipliers[24][2];
extern CONST UINT32 Imc8Way4k_3ChWay256_LineMultipliers[24][16];

#if XDEBUG_ENABLED
#define XDEBUG(Expr) CRDEBUG(Expr)
#else
#define XDEBUG(Expr)
#endif

/**
  @brief This function returns number of lines in interleaving.

  @param[in] ChWays      - channel interleaving ways
  @param[in] ImcGran     - memory controller granularity
  @param[in] ChGran      - channel granularity

  @return UINT8      - number of lines in given interleaving configuration
**/
UINT8
GetNumLinesInRotation (
  UINT8  ChWays,
  UINT32 ImcGran,
  UINT32 ChGran
  )
{
  if (ImcGran == ChGran) {
    if (ChWays == CH_1WAY) {
      return 1;
    } else {
      return 2;
    }
  } else {
    switch (ChWays) {
    case CH_1WAY:
      return 16;
      break;
    case CH_2WAY:
      return 8;
      break;
    case CH_3WAY:
      return 16;
      break;
    default:
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "GetNumLinesInRotation: channel ways %d not supported\n", ChWays));
      return 0;
    }
  }
} // GetNumLinesInRotation()


/**
  @brief This function returns line multiplier for given dimm and
         line numbers taking part in interleaving.

  @param[in] ImcSize     - memory controller granularity
  @param[in] LineSize    - channel granularity
  @param[in] ImcWays     - Imc interleaving ways
  @param[in] ChWays      - channel interleaving ways
  @param[in] DimmNum     - dimm number in interleaving set (starting from 0)
  @param[in] LineNum     - line number

  @return UINT32     - line multiplier for given arguments
**/
UINT32
GetLineMultiplier (
  UINT32 ImcSize,
  UINT32 LineSize,
  UINT8  ImcWays,
  UINT8  ChWays,
  UINT8  DimmNum,
  UINT8  LineNum)
{
  UINT32 LineMultiplier = 0;
  UINT32 NumLinesInRotation;

  NumLinesInRotation = GetNumLinesInRotation (ChWays, ImcSize, LineSize);
  //
  //if GetNumLinesInRotation returns 0 it defaulted from a bad value so return;
  //
  if (NumLinesInRotation == 0) {
    return 0;
  }

  DimmNum %= ImcWays * ChWays;
  LineNum %= NumLinesInRotation;

  switch (ChWays) {
  case CH_1WAY:
    LineMultiplier = LineNum + (DimmNum * NumLinesInRotation);
    break;
  case CH_2WAY:
    switch (ImcWays) {
    case ONE_IMC_WAY:
      if (ImcSize == LineSize) {
        LineMultiplier = Imc1Way4k_2ChWay4k_LineMultipliers[DimmNum][LineNum];
      } else {
        LineMultiplier = Imc1Way4k_2ChWay256_LineMultipliers[DimmNum][LineNum];
      }
      break;
    case TWO_IMC_WAY:
      if (ImcSize == LineSize) {
        LineMultiplier = Imc2Way4k_2ChWay4k_LineMultipliers[DimmNum][LineNum];
      } else {
        LineMultiplier = Imc2Way4k_2ChWay256_LineMultipliers[DimmNum][LineNum];
      }
      break;
    case FOUR_IMC_WAY:
      if (ImcSize == LineSize) {
        LineMultiplier = Imc4Way4k_2ChWay4k_LineMultipliers[DimmNum][LineNum];
      } else {
        LineMultiplier = Imc4Way4k_2ChWay256_LineMultipliers[DimmNum][LineNum];
      }
      break;
    case EIGHT_IMC_WAY:
      // 8 Imc Way not supported for 4k/4k
      LineMultiplier = Imc8Way4k_2ChWay256_LineMultipliers[DimmNum][LineNum];
      break;
    default:
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "GetLineMultiplier: iMC ways %d not supported\n", ImcWays));
      LineMultiplier = 0;
      break;
    }
    break;
  case CH_3WAY:
    switch (ImcWays) {
    case ONE_IMC_WAY:
      if (ImcSize == LineSize) {
        LineMultiplier = Imc1Way4k_3ChWay4k_LineMultipliers[DimmNum][LineNum];
      } else {
        LineMultiplier = Imc1Way4k_3ChWay256_LineMultipliers[DimmNum][LineNum];
      }
      break;
    case TWO_IMC_WAY:
      if (ImcSize == LineSize) {
        LineMultiplier = Imc2Way4k_3ChWay4k_LineMultipliers[DimmNum][LineNum];
      } else {
        LineMultiplier = Imc2Way4k_3ChWay256_LineMultipliers[DimmNum][LineNum];
      }
      break;
    case FOUR_IMC_WAY:
      if (ImcSize == LineSize) {
        LineMultiplier = Imc4Way4k_3ChWay4k_LineMultipliers[DimmNum][LineNum];
      } else {
        LineMultiplier = Imc4Way4k_3ChWay256_LineMultipliers[DimmNum][LineNum];
      }
      break;
    case EIGHT_IMC_WAY:
      if (ImcSize == LineSize) {
        LineMultiplier = Imc8Way4k_3ChWay4k_LineMultipliers[DimmNum][LineNum];
      } else {
        LineMultiplier = Imc8Way4k_3ChWay256_LineMultipliers[DimmNum][LineNum];
      }
      break;
    default:
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "GetLineMultiplier: iMC ways %d not supported\n", ImcWays));
      LineMultiplier = 0;
      break;
    }
    break;
  default:
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "GetLineMultiplier: channel ways %d not supported\n", ChWays));
    LineMultiplier = 0;
    break;
  }
  return LineMultiplier;
} // GetLineMultiplier()


/**
Routine Description: This function converts given SPA to DPA

  @param[in] SpaOffset   - Spa for which DPA is computed
  @param[in] ImcWays     - Imc interleaving ways
  @param[in] ChWays      - channel interleaving ways
  @param[in] LineSize    - channel granularity
  @param[in] ImcSize     - memory controller granularity

  @return EFI status is returned.
**/
EFI_STATUS
GetDpaAddressFromSpaOffset (
  IN  UINT64       SpaOffset,
  IN  UINT8        ImcWays,
  IN  UINT8        ChWays,
  IN  UINT32       LineSize,
  IN  UINT32       ImcSize,
  OUT DPA_ADDRESS *DpaPtr
  )
{
  UINT64           SpaIndex;
  UINT64           RotationNum;
  UINT8            NumLinesInRotation;
  UINT8            IntWays;
  UINT32           RotationSize;
  UINT8            LineNum = 0;
  UINT8            IntDimmNum = 0;
  UINT8            MaxImc;
  BOOLEAN          IndexFound = FALSE;

  NumLinesInRotation = GetNumLinesInRotation (ChWays, ImcSize, LineSize);
  if (NumLinesInRotation == 0) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "GetDpaAddressFromSpaOffset: Cannot translate SPA %p (iMC ways %d, channel ways %d, line size %d, iMC size %d)\n",
            SpaOffset, ImcWays, ChWays, LineSize, ImcSize));
    return EFI_INVALID_PARAMETER;
  }
  IntWays      = ImcWays * ChWays;
  RotationSize = LineSize * NumLinesInRotation;
  RotationNum  = SpaOffset / (RotationSize * IntWays);
  SpaIndex     = SpaOffset % (RotationSize * IntWays) / LineSize;

  for (IntDimmNum = 0; IntDimmNum < IntWays; IntDimmNum++) {
    for (LineNum = 0; LineNum < NumLinesInRotation; LineNum++) {
      if (SpaIndex == GetLineMultiplier (ImcSize, LineSize, ImcWays, ChWays, IntDimmNum, LineNum)) {
        IndexFound = TRUE;
        break;
      }
    }
    if (IndexFound) {
      break;
    }
  }

  MaxImc = GetMaxImc ();
  //
  // Now we have everything to compute the DPA.
  //
  DpaPtr->Offset     = (RotationNum * RotationSize) + (LineNum * LineSize) + (SpaOffset % LineSize);
  DpaPtr->IntDimmNum = IntDimmNum;
  DpaPtr->Ch         = IntDimmNum % ChWays;
  DpaPtr->Imc        = (IntDimmNum / ChWays) % MaxImc;
  DpaPtr->Skt        = (IntDimmNum / ChWays) / MaxImc;

  return EFI_SUCCESS;
}


/**
  @brief This function converts given DPA offset to SPA offset

  @param[in] Dpa         - DPA offset for which SPA is computed
  @param[in] DimmNum     - DIMM number in interleave set
  @param[in] ImcWays     - Imc interleaving ways
  @param[in] ChWays      - channel interleaving ways
  @param[in] LineSize    - channel granularity
  @param[in] ImcSize     - memory controller granularity

  @return UINT64     - computed SPA address offset
**/
UINT64
GetSpaOffsetFromDpaOffset (
  UINT64 Dpa,
  UINT8  DimmNum,
  UINT8  ImcWays,
  UINT8  ChWays,
  UINT32 LineSize,
  UINT32 ImcSize
  )
{
  UINT8  LineNum;
  UINT8  NumLinesInRotation;
  UINT64 RotationNum;
  UINT64 RotationSize;
  INT32  LineMultiplier;
  UINT64 Spa;

  XDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "GetSpaOffsetFromDpaOffset: DPA %llX DimmNo %d ImcWays %d ChWays %d LineSize %d ImcSize %d\n",
           Dpa, DimmNum, ImcWays, ChWays, LineSize, ImcSize));

  NumLinesInRotation = GetNumLinesInRotation (ChWays, ImcSize, LineSize);
  if (NumLinesInRotation == 0) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "GetSpaOffsetFromDpaOffset: Cannot translate DPA %p (DIMM %d, iMC ways %d, channel ways %d, line size %d, iMC size %d)\n",
            Dpa, DimmNum, ImcWays, ChWays, LineSize, ImcSize));
    return 0;
  }
  XDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "GetSpaOffsetFromDpaOffset: NumLinesInRotation %d\n", NumLinesInRotation));

  RotationSize  = LineSize * NumLinesInRotation;
  RotationNum   = Dpa / RotationSize;
  LineNum       = (UINT8)((Dpa % RotationSize) / LineSize);

  LineMultiplier = GetLineMultiplier (ImcSize, LineSize, ImcWays, ChWays, DimmNum, LineNum);

  XDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "GetSpaOffsetFromDpaOffset: LineMultiplier %d\n", LineMultiplier));

  Spa = (RotationNum * RotationSize * ImcWays * ChWays) + (LineMultiplier * LineSize) + (Dpa % LineSize);

  XDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "GetSpaOffsetFromDpaOffset: SPA 0x%llX\n", Spa));

  return Spa;
}


/**

Routine Description: This function converts given SPA to DPA Address

  @param[in]  SpaBaseAddr - SAD Base Address
  @param[in]  Spa         - Spa for which we need the DPA
  @param[in]  ImcWays     - Imc interleaving ways
  @param[in]  ChWays      - channel interleaving ways
  @param[in]  LineSize    - channel granularity
  @param[in]  ImcSize     - memory controller granularity
  @param[out] *DpaAddress - Full DPA address

  @return EFI_STATUS - status of the conversion effort

**/
EFI_STATUS
XlateSpaToDpaAddress (
  IN  UINT64      SpaBaseAddr,
  IN  UINT64      Spa,
  IN  UINT8       ImcWays,
  IN  UINT8       ChWays,
  IN  UINT32      LineSize,
  IN  UINT32      ImcSize,
  OUT DPA_ADDRESS *DpaAddress
  )
{

  if (Spa < SpaBaseAddr || ImcWays == 0 || ChWays == 0 || LineSize == 0 || ImcSize == 0 || DpaAddress == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  return GetDpaAddressFromSpaOffset ((Spa - SpaBaseAddr), ImcWays, ChWays, LineSize, ImcSize, DpaAddress);
}


/**
Routine Description: XlateSpaToDpa - This function converts
given Spa to Dpa in the case of the Dimm involved in all Channel
ways and up to 8 Imc Ways. Converts only PMEM and CTRL regions.

  @param[in]  DimmPtr     - Pointer to DIMM structure
  @param[in]  SpaBaseAddr - SAD Base Address
  @param[in]  Spa         - SPA address for which we need the DPA
  @param[out] DpaBufPtr   - Pointer to a buffer for DPA address

  @return EFI_STATUS - status of the conversion effort
**/
EFI_STATUS
XlateSpaToDpaOffset (
  IN  NVDIMM   *DimmPtr,
  IN  UINT64    SpaBaseAddr,
  IN  UINT64    Spa,
  OUT UINT64   *DpaBufPtr
  )
{
  UINT8       ImcWays;
  UINT8       ChWays;
  UINT32      LineSize;
  UINT32      ImcSize;
  INT32       Index;
  DPA_ADDRESS DpaAddr;

  if (Spa < SpaBaseAddr || DimmPtr == NULL || DpaBufPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Spa >= mCrInfo.CrbdAddrBase && Spa < mCrInfo.CrbdAddrLimit) {
    //
    // For control region block decoder interleaving DIMM index bits (CR_CRBD_DIMM_INDEX) are inserted
    // on physycal address bits {18..12}, and the DPA address bits starting from 12 are shifted left by 7.
    // So to translate SPA to DPA just remove SPA base from the SPA address and shift right by 7 the address
    // bits starting from 19.
    //
    Spa -= mCrInfo.CrbdAddrBase;
    *DpaBufPtr = ((Spa >> 7) & ~0xFFF) | (Spa & 0xFFF);
    return EFI_SUCCESS;
  }
  if (IsPmemRgn (&mCrInfo.NvdimmInfo, SpaBaseAddr)) {
    Index = GetPmemIndexBasedOnSocChDimm (&mCrInfo.NvdimmInfo, SpaBaseAddr, (UINT8) DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm);
    if (Index == -1) {
      return EFI_INVALID_PARAMETER;
    }

    ImcWays  = mCrInfo.NvdimmInfo.PMEMInfo[Index].iMCWays;
    ChWays   = mCrInfo.NvdimmInfo.PMEMInfo[Index].ChWays;
    LineSize = GetChGranularityFromSadBaseAddr (SpaBaseAddr);
    ImcSize  = GetSktGranularityFromSadBaseAddr (SpaBaseAddr);
  } else {
    // Control Region only !
    SpaBaseAddr = DimmPtr->SADSpaBase;
    ImcWays     = DimmPtr->iMCWays;
    ChWays      = DimmPtr->ChWays;
    LineSize    = mCrInfo.NvdimmInfo.ChGran;
    ImcSize     = mCrInfo.NvdimmInfo.SktGran;
  }

  if (LineSize == 0) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }


  if (EFI_ERROR (GetDpaAddressFromSpaOffset ((Spa - SpaBaseAddr), ImcWays, ChWays, LineSize, ImcSize, &DpaAddr))) {
    return EFI_INVALID_PARAMETER;
  }
  *DpaBufPtr = DpaAddr.Offset;
  return EFI_SUCCESS;
}


/**
Routine Description: XlateDpaToSpa - This function converts given Dpa
to Spa in the case of the Dimm involved in all Channel ways and
up to 8 Imc Ways. Converts only PMEM and CTRL regions.

  @param[in]  DimmPtr     - Pointer to NVM DIMM structure
  @param[in]  SpaBaseAddr - Start Address of the SPA for DPA Translation
  @param[in]  Dpa         - DPA offset for which we need the SPA address
  @param[out] SpaBufPtr   - Pointer to a buffer for SPA address

  @return EFI_STATUS - status of the conversion effort
**/
EFI_STATUS
XlateDpaOffsetToSpa (
  IN  NVDIMM   *DimmPtr,
  IN  UINT64    SpaBaseAddr,
  IN  UINT64    Dpa,
  OUT UINT64   *SpaBufPtr
  )
{
  UINT8       ChWays;
  UINT8       ImcWays;
  UINT8       DimmNum;
  UINT32      LineSize;
  UINT32      ImcSize;
  INT32       Index;

  if (DimmPtr == NULL || SpaBufPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  XDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "S%d.C%d.D%d: Translate DPA %llX (Base 0x%1lX)\n",
           DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm, Dpa, SpaBaseAddr));

  if (SpaBaseAddr >= mCrInfo.CrbdAddrBase && SpaBaseAddr < mCrInfo.CrbdAddrLimit) {
    //
    // For control region block decoder interleaving DIMM index bits (CR_CRBD_DIMM_INDEX) are inserted
    // on physycal address bits {18..12}, and the DPA address bits starting from 12 are shifted left by 7.
    // So to translate DPA to SPA just shift left by 7 the address bits starting from 12 and add the
    // DIMM control base address that is the block decoder SPA base + DIMM offset (CR_CRBD_DIMM_OFFSET).
    //
    *SpaBufPtr = DimmPtr->CntrlBase + ((Dpa & ~0xFFF) << 7) + (Dpa & 0xFFF);
    return EFI_SUCCESS;
  }
  if (IsPmemRgn (&mCrInfo.NvdimmInfo, SpaBaseAddr)) {

    Index = GetPmemIndexBasedOnSocChDimm (&mCrInfo.NvdimmInfo, SpaBaseAddr, (UINT8) DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm);
    if (Index == -1) {
      return EFI_INVALID_PARAMETER;
    }
    DimmNum  = mCrInfo.NvdimmInfo.PMEMInfo[Index].IntDimmNum;
    ImcWays  = mCrInfo.NvdimmInfo.PMEMInfo[Index].iMCWays;
    ChWays   = mCrInfo.NvdimmInfo.PMEMInfo[Index].ChWays;
    LineSize = GetChGranularityFromSadBaseAddr (SpaBaseAddr);
    ImcSize  = GetSktGranularityFromSadBaseAddr (SpaBaseAddr);
  } else {
    //
    // SKX Control Region only !
    //
    SpaBaseAddr = DimmPtr->SADSpaBase;
    DimmNum     = DimmPtr->IntDimmNum;
    ImcWays     = DimmPtr->iMCWays;
    ChWays      = DimmPtr->ChWays;
    LineSize    = mCrInfo.NvdimmInfo.ChGran;
    ImcSize     = mCrInfo.NvdimmInfo.SktGran;
  }
  if (LineSize == 0) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }


  *SpaBufPtr = GetSpaOffsetFromDpaOffset (Dpa, DimmNum, ImcWays, ChWays, LineSize, ImcSize);
  *SpaBufPtr += SpaBaseAddr;

  return EFI_SUCCESS;
}

