/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2020 Intel Corporation. <BR>

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

#include "CrystalRidge.h"
#include "Nfit.h"
#include "XlateFunctions/XlateFunctions.h"
#include <Protocol/AcpiPlatformProtocol.h>
#include <Protocol/SmbiosMemInfo.h>
#include "Fis.h"
#include <Library/MemTypeLib.h>
#include <Chip/Include/FnvAccessCommon.h>
#include <Library/MemMcIpLib.h>
#include <Include/MemMapHost.h>
#include <Protocol/Dcpmm.h>
#include "NfitDebug.h"
#include <Library/NvdimmCommonLib.h>
#include <Library/MemRcLib.h>


/*****************************************************************************
 * Definitions
 *****************************************************************************/

#define NFIT_DEFAULT_DIMM_SKU (DIMM_SKU_MEMORY_MODE | DIMM_SKU_APP_DIRECT | DIMM_SKU_ENCRYPTION)

extern CONST UINT32               Imc1Way4k_3ChWay256_LineMultipliers[3][16];
extern CONST UINT32               Imc2Way4k_3ChWay256_LineMultipliers[6][16];
extern CONST UINT32               Imc4Way4k_3ChWay256_LineMultipliers[12][16];
extern CONST UINT32               Imc8Way4k_3ChWay256_LineMultipliers[24][16];
extern CONST UINT32               Imc1Way4k_3ChWay4k_LineMultipliers[3][2];
extern CONST UINT32               Imc2Way4k_3ChWay4k_LineMultipliers[6][2];
extern CONST UINT32               Imc4Way4k_3ChWay4k_LineMultipliers[12][2];
extern CONST UINT32               Imc8Way4k_3ChWay4k_LineMultipliers[24][2];
extern MEMORY_MAP_HOST            *mMemMapHost;


/*****************************************************************************
 * Function prototypes
 *****************************************************************************/
EFI_STATUS
NfitBuildNvdimmRegionMappingStructure (
  OUT NVDIMM_FW_INTERFACE_TABLE  *NfitPtr,
  IN  UINT8                       Skt,
  IN  UINT8                       Ch,
  IN  UINT8                       Dimm,
  IN  NFIT_SPA_RANGE_STRUCTURE   *SpaRangeStrcPtr,
  IN  UINT16                      IntIndex,
  IN  UINT64                      RegionOffset,
  IN  UINT16                      Ways,
  IN  UINT16                      MemType
  );
UINT16
NfitBuildNvdimmCtrlRegionStructure (
  IN OUT NVDIMM_FW_INTERFACE_TABLE *NfitPtr,
  IN     UINT8                      Skt,
  IN     UINT8                      Ch,
  IN     UINT8                      Dimm,
  IN     UINT16                     Rfic
  );


/*****************************************************************************
 * Variables,
 *****************************************************************************/
CONST EFI_GUID AcpiBlkRegionGuid  = BLOCK_DATA_WINDOW_REGION_GUID;
CONST EFI_GUID AcpiCtrlRegionGuid = CONTROL_REGION_GUID;
CONST EFI_GUID AcpiPmemRegionGuid = PMEM_REGION_GUID;
CONST EFI_GUID IntelCtrlRegionGuid = CONTROL_REGION_DFX_GUID;

UINT32                            mNfitMaxSize;               // Space left in NFIT buffer when building structures
SMBIOS_MEM_INFO_PROTOCOL         *mSmbiosInfoProtocol = NULL;
//
// NFIT structure indexes/IDs
//
UINT16                            mSpaRangeIndex;         // Identifies SPA Range Structure
UINT16                            mNvdimmRegionMappingId; // Identifies NVDIMM Region Mapping Structure
UINT16                            mNvdimmCtrlRegionIndex; // Identifies NVDIMM Ctrl Region Structure
UINT16                            mInterleaveDescIndex;   // Identifies Interleave Structure

//
// This structure is used to set proximity domains of NFIT SPA regions that
// will match corresponding entries of the SRAT Memory Affinity table.
//
ACPI_MEMORY_AFFINITY_DATA        *mAcpiMemAffinityTbl;
UINTN                             mAcpiMemAffinityTblLen;

//
// This struct is used by processing code to prevent redundant SADs
// getting entered into NFIT.
// TODO: SADs are not redundant, this table actually merges UMA interleaving SADs (e.g. NVM_CTRL), rename it.
//
struct _SAD_INFO_ {
  UINT8       NumOfSADs;
  UINT32      Limits[MAX_SOCKET * MAX_SADS];
} mSAD_Info;

struct {
  UINT16  StorageCtrlStrcIndex;   // Index of NVDIMM Ctrl Region Structure for storage on this NVDIMM
  UINT16  StorageMapStrcIndex;    // Index of NVDIMM Region Mapping Structure for storage on this NVDIMM
  UINT16  AppDirectCtrlStrcIndex; // Index of NVDIMM Ctrl Region Structure for PMEM on this NVDIMM
  UINT16  AppDirectMapStrcIndex;  // Index of NVDIMM Region Mapping Structure for PMEM on this NVDIMM
  UINT16  MailboxCtrlStrcIndex;   // Index of NVDIMM Ctrl Region Structure for mailbox on this DIMM
  UINT16  MailboxMapStrcIndex;    // Index of NVDIMM Region Mapping Structure for mailbox on this NVDIMM
} mNfitNvdimmInfo[MAX_SOCKET][MAX_CH][MAX_DIMM];


/******************************************************************************
 * Functions
 ******************************************************************************/
/**
  Checks to see if the Limit field of the current SAD Table has been previously
  accounted for thru another SAD. The idea is to eliminate redundancy while adding SADs to the Nfit.
  This is accomplished by adding the current SAD's limit to the Sad_Info struct when the current Limit is not found
  in the Limits of Sad_Info and increment NumOfSads of Sad_Info struct.


  @param pSad         Pointer to the current SAD

  @retval TRUE  SAD's limit has been found in another SAD that was added previously
  @retval FALSE SAD's limit has not been found in another SAD that was added previously
**/
BOOLEAN
NfitCheckIfRedundantSad (
  IN SAD_TABLE *SadPtr
  )
{
  UINT8 i;

  if (SadPtr == NULL) {
    ASSERT (SadPtr != NULL);
    return FALSE;
  }
  for (i = 0; i < mSAD_Info.NumOfSADs; i++) {
    if (mSAD_Info.Limits[i] == SadPtr->Limit) {
      return TRUE;
    }
  }
  //
  // SAD is new, store it in mSAD_Info cache and return false.
  //
  mSAD_Info.Limits[i] = SadPtr->Limit;
  mSAD_Info.NumOfSADs++;

  return FALSE;
}


/**
  Retrieves the DIMM Sku bitmap, if DIMM is disabled the default SKU bitmap will be returned

  @param Socket                 Socket number of the memory device
  @param Channel                Channel number of the memory device
  @param Dimm                   DIMM number

  @retval SKU bitmap
**/
UINT8
NfitGetDimmSku (
  IN UINT8  Skt,
  IN UINT8  Ch,
  IN UINT8  Dimm
  )
{
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  if (Skt >= MAX_SOCKET || Ch >= MaxChDdr || Dimm >= MAX_DIMM) {
    ASSERT (FALSE);
    return 0;
  }
  if (!mSystemMemoryMap->Socket[Skt].ChannelInfo[Ch].DimmInfo[Dimm].Enabled) {
    //
    // We need to use the default NVDIMM SKU mask, since it might not be available when NVDIMM failed to train
    //
    return NFIT_DEFAULT_DIMM_SKU;
  }
  return mSystemMemoryMap->Socket[Skt].ChannelInfo[Ch].DimmInfo[Dimm].DimmSku;
}


/**
  This routine stores given ACPI SRAT Memory Affinity structures address and length

  @param *MemAffDataPtr     - ACPI SRAT Memory Affinity structures
  @param MemAffDataLength   - ACPI SRAT Memory Affinity structures length

  @retval EFI_SUCCESS           - NVDIMM SPA ranges affinity table created successfully.
  @retval EFI_INVALID_PARAMETER - MemAffDataValid is TRUE and MemAffDataPtr is NULL
**/
EFI_STATUS
NfitSaveAcpiMemoryAffinityData (
  IN ACPI_MEMORY_AFFINITY_DATA *MemAffDataPtr,
  IN UINTN                      MemAffDataLength
  )
{
  if (MemAffDataPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  mAcpiMemAffinityTbl = MemAffDataPtr;
  mAcpiMemAffinityTblLen = MemAffDataLength;

  NfitDumpMemoryAffinityData (mAcpiMemAffinityTbl, mAcpiMemAffinityTblLen);
  return EFI_SUCCESS;
}


/**
  @brief This routine returns proximity domain for given SPA range

  @param[in]  SpaBase           System Physical Address Range Base
  @param[in]  SpaLen            System Physical Address Range Length
  @param[in]  Type              Memory Region Region type
  @param[out] DomainPtr         Pointer to Proximity Domain Id

  @retval EFI_SUCCESS           Proximity domain found
          EFI_INVALID_PARAMETER ProximityDomain is null
          EFI_NOT_FOUND         Proximity domain not available
**/
EFI_STATUS
GetProximityDomainOf (
  IN  UINT64  SpaBase,
  IN  UINT64  SpaLen,
  IN  UINT16  Type,
  OUT UINT32 *DomainPtr
  )
{
  UINTN   Idx;
  UINT64  SpaEnd;
  UINT64  DomainBase;
  UINT64  DomainEnd;

  if (DomainPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if ((mAcpiMemAffinityTbl != NULL) && (mAcpiMemAffinityTblLen != 0) && IsMemTypeAppDirect (Type)) {

    SpaEnd = SpaBase + SpaLen;
    for (Idx = 0; Idx < mAcpiMemAffinityTblLen; Idx++) {

      DomainBase = ((UINT64) mAcpiMemAffinityTbl[Idx].AddressBaseHigh << 32) + mAcpiMemAffinityTbl[Idx].AddressBaseLow;
      DomainEnd = DomainBase + ((UINT64) mAcpiMemAffinityTbl[Idx].LengthHigh << 32) + mAcpiMemAffinityTbl[Idx].LengthLow;
      if ((mAcpiMemAffinityTbl[Idx].Flags & ACPI_MEMORY_NONVOLATILE) && SpaBase >= DomainBase && SpaEnd <= DomainEnd) {

        *DomainPtr = mAcpiMemAffinityTbl[Idx].ProximityDomain;
        CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "SpaBase 0x%llX len 0x%llX - found proximity domain 0x%X\n",
                SpaBase, SpaLen, *DomainPtr));
        return EFI_SUCCESS;
      }
    }
  }
  CRDEBUG ((DEBUG_WARN, CR_WARN_STR ("NFIT") "SpaBase 0x%llX len 0x%llX type 0x%X - proximity domain not found\n",
          SpaBase, SpaLen, Type));
  return EFI_NOT_FOUND;
}


/**
This routine computes the checksum of a table of given size.

@param *TblStart    - Start of the table for which checksum needs to be computed
@param BytesCount   - Total size of the table for which checksum needs to be computed

@return UINT8       - Checksum computed
**/
UINT8
NfitComputeTblChsum (
  IN UINT8  *TblStart,
  IN UINT32 BytesCount
  )
{
  UINTN i;
  UINT8 res;

  if (TblStart == NULL || BytesCount == 0) {
    return 0;
  }

  res = *TblStart;
  for (i = 1; i < BytesCount; i++) {
    res = res + TblStart[i];
  }
  res = 0 - res;

  return (res);
} // NfitComputeTblChsum()


/**
 @brief Create NVDIMM Control Region Structure for each RFIC according to DIMM SKU, for each NVDIMM in the system.

 @param[in,out] NfitPtr - Pointer to buffer with NFIT structures.

 @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
NfitGenerateCtrlRegionsListSkuBased (
  IN OUT NVDIMM_FW_INTERFACE_TABLE *NfitPtr
  )
{
  UINT8       Skt;
  UINT8       Ch;
  UINT8       Dimm;
  UINT8       DimmSku;
  UINT8       MaxChDdr;

  ZeroMem (&mNfitNvdimmInfo[0], sizeof(mNfitNvdimmInfo));
  MaxChDdr = GetMaxChDdr ();

  for (Skt = 0; Skt < MAX_SOCKET; Skt++) {

    if (!mSystemMemoryMap->Socket[Skt].SocketEnabled) {
      continue; // Skip disabled socket
    }
    for (Ch = 0; Ch < MaxChDdr; Ch++) {

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

        if (!(mSystemMemoryMap->Socket[Skt].ChannelInfo[Ch].DimmInfo[Dimm].DcpmmPresent)) {
          continue; // Skip non DCPMM NVDIMM
        }
        DimmSku = NfitGetDimmSku (Skt, Ch, Dimm);

        if (DimmSku & DIMM_SKU_APP_DIRECT) {
          //
          // Persistent memory feature enabled, create NVDIMM Control Region Structure for NFIT_RFIC_APPDIRECT.
          //
          mNfitNvdimmInfo[Skt][Ch][Dimm].AppDirectCtrlStrcIndex = NfitBuildNvdimmCtrlRegionStructure (NfitPtr, Skt, Ch, Dimm, NFIT_RFIC_APPDIRECT);
          if (mNfitNvdimmInfo[Skt][Ch][Dimm].AppDirectCtrlStrcIndex == 0) {

            CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("NFIT") "S%d.C%d.%d: Failed to build NVDIMM Ctrl Region Structure for RFIC 0x%04X\n",
                    Skt, Ch, Dimm, NFIT_RFIC_APPDIRECT));
          }
        }
        if (DimmSku & DIMM_SKU_STORAGE_MODE) {
          //
          // Block storage feature enabled, create NVDIMM Control Region Structure for NFIT_RFIC_STORAGE. If this feature
          // is present its ctrl structure will be refered also by mailbox region related structures in NFIT.
          // Otherwise we have to create separate ctrl structure of NULL RFIC for mailbox region.
          //
          mNfitNvdimmInfo[Skt][Ch][Dimm].MailboxCtrlStrcIndex =
          mNfitNvdimmInfo[Skt][Ch][Dimm].StorageCtrlStrcIndex = NfitBuildNvdimmCtrlRegionStructure (NfitPtr, Skt, Ch, Dimm, NFIT_RFIC_STORAGE);
          if (mNfitNvdimmInfo[Skt][Ch][Dimm].StorageCtrlStrcIndex == 0) {

            CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("NFIT") "S%d.C%d.%d: Failed to build NVDIMM Ctrl Region Structure for RFIC 0x%04X\n",
                    Skt, Ch, Dimm, NFIT_RFIC_STORAGE));
          }
        } else if (!mSystemMemoryMap->MemSetup.NfitPublishMailboxStructsDisable) {
          //
          // No block storage feature, we have to create NVDIMM Control Region Structure with NFIT_RFIC_NULL
          // to refer in mailbox region related structures in NFIT.
          //
          mNfitNvdimmInfo[Skt][Ch][Dimm].MailboxCtrlStrcIndex = NfitBuildNvdimmCtrlRegionStructure (NfitPtr, Skt, Ch, Dimm, NFIT_RFIC_NULL);
          if (mNfitNvdimmInfo[Skt][Ch][Dimm].MailboxCtrlStrcIndex == 0) {

            CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("NFIT") "S%d.C%d.%d: Failed to build NVDIMM Ctrl Region Structure for RFIC 0x%04X\n",
                    Skt, Ch, Dimm, NFIT_RFIC_NULL));
          }
        }
      } // for (Dimm...)
    } // for (Ch...)
  } // for (Skt...)
  return EFI_SUCCESS;
} // NfitGenerateCtrlRegionsListSkuBased()


/**
 @brief Create NVDIMM Control Region Structure for each RFIC according to DIMM paticipation in SADs.

 @param[in,out] NfitPtr - Pointer to buffer with NFIT structures.

 @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
NfitGenerateCtrlRegionsListSadBased (
  IN OUT NVDIMM_FW_INTERFACE_TABLE *NfitPtr
  )
{
  EFI_STATUS  Status;
  UINT32      ChannelMap;
  UINT16      Rfic;
  UINT8       Skt;
  UINT8       Ch;
  UINT8       Dimm;
  UINT8       Sad;
  UINT8       Idx;
  UINT8       MaxChDdr;
  UINT16     *MapIndexToUpdatePtr;

  ZeroMem (&mNfitNvdimmInfo[0], sizeof(mNfitNvdimmInfo));
  MaxChDdr = GetMaxChDdr ();

  for (Skt = 0; Skt < MAX_SOCKET; Skt++) {

    if (!mSystemMemoryMap->Socket[Skt].SocketEnabled) {
      continue; // Skip disabled socket
    }
    for (Sad = 0; Sad < NELEMENTS (mSystemMemoryMap->Socket[Skt].SAD); Sad++) {

      if (!mSystemMemoryMap->Socket[Skt].SAD[Sad].local ||
           IsMemTypeReserved (mSystemMemoryMap->Socket[Skt].SAD[Sad].type)) {

        continue;  // Skip remote and rserved SADs
      }
      if (IsMemTypeAppDirect (mSystemMemoryMap->Socket[Skt].SAD[Sad].type)) {

        Rfic = NFIT_RFIC_APPDIRECT;
      } else {
        //
        // Skip other SAD types. The MemType1lmCtrl SAD will be handled in the second loop after this one.
        //
        continue;
      }
      for (Ch = 0, ChannelMap = GetChInterleaveBitmap (Skt, Sad);
           Ch < MaxChDdr && ChannelMap != 0; Ch++, ChannelMap >>= 1) {

        if (!(ChannelMap & BIT0)) {
          continue;
        }
        Status = GetChannelDimm (Skt, Ch, &Dimm);
        if (EFI_ERROR (Status)) {

          CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("NFIT") "S%d.C%d: No NVDIMM found for SAD[%d] of type %d participating in interleave 0x%08X\n",
                  Skt, Ch, Sad, mSystemMemoryMap->Socket[Skt].SAD[Sad].type, GetChInterleaveBitmap (Skt, Sad)));
          continue;
        }
        MapIndexToUpdatePtr = NULL;
        switch (Rfic) {
        case NFIT_RFIC_APPDIRECT:
          if (mNfitNvdimmInfo[Skt][Ch][Dimm].AppDirectCtrlStrcIndex != 0) {

            CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("NFIT") "S%d.C%d.D%d: NVDIMM in SAD[%d] of type %d (interleave 0x%08X) already participating in other PMEM?\n",
                    Skt, Ch, Dimm, Sad, mSystemMemoryMap->Socket[Skt].SAD[Sad].type, GetChInterleaveBitmap (Skt, Sad)));
            break;
          }
          MapIndexToUpdatePtr = &mNfitNvdimmInfo[Skt][Ch][Dimm].AppDirectCtrlStrcIndex;
          break;
        default:
          ASSERT (FALSE);
        }
        if (MapIndexToUpdatePtr != NULL) {
          *MapIndexToUpdatePtr = NfitBuildNvdimmCtrlRegionStructure (NfitPtr, Skt, Ch, Dimm, Rfic);
          if (*MapIndexToUpdatePtr == 0) {

            CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("NFIT") "S%d.C%d.%d: Failed to build NVDIMM Ctrl Region Structure for RFIC 0x%04X\n",
                    Skt, Ch, Dimm, Rfic));
          }
        }
      } // for (Ch...)
    } // for (Sad...)
  } // for (Skt...)
  //
  // Now browse NVDIMM list and:
  // - create NVDIMM Control Region Structure for DIMM configured for PMEM but not mapped (media disabled or incomplete interleave set);
  // - select or create NVDIMM Control Region Structure for NVDIMM mailbox region.
  //
  for (Idx = 0; Idx < mCrInfo.NvdimmInfo.NumNvdimms; Idx++) {

    Skt = (UINT8)mCrInfo.NvdimmInfo.Nvdimms[Idx].SocketId;
    Ch = mCrInfo.NvdimmInfo.Nvdimms[Idx].Ch;
    Dimm = mCrInfo.NvdimmInfo.Nvdimms[Idx].Dimm;

    if (mNfitNvdimmInfo[Skt][Ch][Dimm].AppDirectCtrlStrcIndex == 0 &&
        mSystemMemoryMap->Socket[Skt].ChannelInfo[Ch].DimmInfo[Dimm].nonVolCap > 0) {

      mNfitNvdimmInfo[Skt][Ch][Dimm].AppDirectCtrlStrcIndex = NfitBuildNvdimmCtrlRegionStructure (NfitPtr, Skt, Ch, Dimm, NFIT_RFIC_APPDIRECT);
    }
    if (mNfitNvdimmInfo[Skt][Ch][Dimm].StorageCtrlStrcIndex != 0) {
      //
      // If we got storage use it also for mailbox region.
      //
      mNfitNvdimmInfo[Skt][Ch][Dimm].MailboxCtrlStrcIndex = mNfitNvdimmInfo[Skt][Ch][Dimm].StorageCtrlStrcIndex;

    } else if (mNfitNvdimmInfo[Skt][Ch][Dimm].AppDirectCtrlStrcIndex != 0) {
      //
      // If we got PMEM use it also for mailbox region.
      //
      mNfitNvdimmInfo[Skt][Ch][Dimm].MailboxCtrlStrcIndex = mNfitNvdimmInfo[Skt][Ch][Dimm].AppDirectCtrlStrcIndex;

    } else {
      //
      // Don't have any structure for this NVDIMM yet, it is 2LM only or disabled, create structure of NFIT_RFIC_APPDIRECT type.
      //
      mNfitNvdimmInfo[Skt][Ch][Dimm].MailboxCtrlStrcIndex = NfitBuildNvdimmCtrlRegionStructure (NfitPtr, Skt, Ch, Dimm, NFIT_RFIC_APPDIRECT);
      if (mNfitNvdimmInfo[Skt][Ch][Dimm].MailboxCtrlStrcIndex == 0) {

        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("NFIT") "S%d.C%d.%d: Failed to build NVDIMM Ctrl Region Structure for RFIC 0x%04X\n",
                Skt, Ch, Dimm, NFIT_RFIC_APPDIRECT));
      }
    }
  } // if for (Idx...)
  return EFI_SUCCESS;
} // NfitGenerateCtrlRegionsListSadBased()


/**
  @brief This routine builds Interleave Description Tables for a 1 Way Channel Interleave.

  @param NfitPtr     -  pointer to Nvdimm f/w interface table
  @param SktGran   -  socket granularity
  @param ChGran    -  channel granularity
  @param ImcWays   -  iMC Ways

  @retval EFI_SUCCESS           Memory Device To SPA Range Map Table created successfully (or no interleaving)
  @retval EFI_INVALID_PARAMETER Invalid Socket/ SktGran/ ChGran or NfitPtr is NULL
**/
EFI_STATUS
NfitBuild1ChWayIntDescTbls (
  IN OUT NVDIMM_FW_INTERFACE_TABLE *NfitPtr,
  IN     UINT32                     SktGran,
  IN     UINT32                     ChGran,
  IN     UINT8                      ImcWays
  )
{
  NFIT_INTERLEAVE_STRUCTURE        *pIntDescTbl;
  UINT32                            LinesDesc, LineSize;
  UINT8                             i;
  UINT8                             j;
  UINT32                           *LineOffset;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "NfitBuild1ChWayIntDescTbls: SktGran %d, ChGran %d, ImcWays %d\n",
              SktGran, ChGran, ImcWays));
  if (NfitPtr == NULL || SktGran < ChGran) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  if (ImcWays == ONE_IMC_WAY) {
    //
    // No interleaving in this case
    //
    return EFI_SUCCESS;
  }
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "Building structure of type %d (%a)\n",
          TYPE_INTERLEAVE_DESCRIPTOR, NfitDebugStrcType2Name (TYPE_INTERLEAVE_DESCRIPTOR)));

  ASSERT (SktGran >= ChGran);
  LinesDesc = SktGran / ChGran;
  LineSize = ChGran;

  for (i = 0; i < ImcWays; i++) {

    pIntDescTbl = (NFIT_INTERLEAVE_STRUCTURE*)&NfitPtr->NfitTables[NfitPtr->Length];
    pIntDescTbl->Type = TYPE_INTERLEAVE_DESCRIPTOR;
    pIntDescTbl->Length = (UINT16) (sizeof (*pIntDescTbl) + LinesDesc * sizeof (*LineOffset));
    // Index must be non-zero, i.e., 1 based.
    pIntDescTbl->InterleaveStructureIndex = mInterleaveDescIndex++;
    pIntDescTbl->NumberOfLines = LinesDesc;
    pIntDescTbl->LineSize = LineSize;

    LineOffset = (UINT32 *) ((UINT8 *) pIntDescTbl + sizeof (*pIntDescTbl));
    for (j = 0; j < LinesDesc; j++) {

      LineOffset[j] = j;
    }
    //
    // Update the NFIT length
    //
    ASSERT ((NfitPtr->Length + pIntDescTbl->Length) <= mNfitMaxSize);
    NfitPtr->Length += pIntDescTbl->Length;

    NfitDumpInterleaveStruct (pIntDescTbl);
  }
  return EFI_SUCCESS;
}


/**
Routine Description:
This routine builds Interleave Description Tables for a 2 Way Channel Interleave.

  @param NfitPtr     -  pointer to Nvdimm f/w interface table
  @param SktGran   -  socket granularity
  @param ChGran    -  channel granularity
  @param ImcWays   -  iMC Ways

  @retval EFI_SUCCESS           Memory Device To SPA Range Map Table created successfully (or no interleaving)
  @retval EFI_INVALID_PARAMETER Invalid Socket/ SktGran/ ChGran or NfitPtr is NULL
**/
EFI_STATUS
NfitBuild2ChWayIntDescTbls (
  IN OUT NVDIMM_FW_INTERFACE_TABLE *NfitPtr,
  IN     UINT32                     SktGran,
  IN     UINT32                     ChGran,
  IN     UINT8                      ImcWays
  )
{
  NFIT_INTERLEAVE_STRUCTURE        *pIntDescTbl;
  UINT32                            LinesDesc, LineSize;
  UINT8                             i;
  UINT8                             j;
  UINT32                           *LineOffset;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "Build2ChWayIntDescTbls: SktGran %d, ChGran %d, ImcWays %d\n",
              SktGran, ChGran, ImcWays));
  if (NfitPtr == NULL || SktGran < ChGran) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "Building structure of type %d (%a)\n",
          TYPE_INTERLEAVE_DESCRIPTOR, NfitDebugStrcType2Name (TYPE_INTERLEAVE_DESCRIPTOR)));

  ASSERT (SktGran >= ChGran);
  LineSize = ChGran;
  //
  // Compute the number of lines required for each channel and address range associated with each MC before the pattern repeats
  //
  if ((ImcWays == 1) || (SktGran == ChGran)) {
    LinesDesc = 2;
  } else {
    LinesDesc = SktGran / (ChGran * 2);
  }
  //
  // Create one table for each MC and CH involved in this interleave
  //
  for (i = 0; i < (ImcWays * 2); i++) {

    pIntDescTbl = (NFIT_INTERLEAVE_STRUCTURE*)&NfitPtr->NfitTables[NfitPtr->Length];
    pIntDescTbl->Type = TYPE_INTERLEAVE_DESCRIPTOR;
    pIntDescTbl->Length = (UINT16) (sizeof (*pIntDescTbl) + LinesDesc * sizeof (*LineOffset));
    //
    // Index must be non-zero, i.e., 1 based.
    //
    pIntDescTbl->InterleaveStructureIndex = mInterleaveDescIndex++;
    pIntDescTbl->NumberOfLines = LinesDesc;
    pIntDescTbl->LineSize = LineSize;

    LineOffset = (UINT32 *) ((UINT8 *) pIntDescTbl + sizeof (*pIntDescTbl));
    for (j = 0; j < LinesDesc; j++)  {
      if (SktGran != ChGran) {
        // When SktGran is not equal to ChGran, the computed LineOffsets do not cross iMCs
        LineOffset[j] = (j * 2);
      } else {
        // When SktGran is equal to ChGran, the computed LineOffsets cross iMCs
        LineOffset[j] = (j * 2 * ImcWays);
      }
    }
    //
    // Update the NFIT length
    //
    ASSERT ((NfitPtr->Length + pIntDescTbl->Length) <= mNfitMaxSize);
    NfitPtr->Length += pIntDescTbl->Length;

    NfitDumpInterleaveStruct (pIntDescTbl);
  }
  return EFI_SUCCESS;
}


/**
Routine Description:
This routine builds Interleave Description Tables for a 3 Way Channel Interleave.

  @param NfitPtr     -  pointer to Nvdimm f/w interface table
  @param SktGran   -  socket granularity
  @param ChGran    -  channel granularity
  @param ImcWays   -  iMC Ways

  @retval EFI_SUCCESS           Memory Device To SPA Range Map Table created successfully (or no interleaving)
  @retval EFI_INVALID_PARAMETER Invalid Socket/ SktGran/ ChGran / ImcWays or NfitPtr is NULL
**/
EFI_STATUS
NfitBuild3ChWayIntDescTbls (
  IN OUT NVDIMM_FW_INTERFACE_TABLE *NfitPtr,
  IN     UINT32                     SktGran,
  IN     UINT32                     ChGran,
  IN     UINT8                      ImcWays
  )
{
  NFIT_INTERLEAVE_STRUCTURE        *pIntDescTbl;
  UINT32                            LineSize;
  UINT32                            LinesDesc;
  UINT8                             Imc;
  UINT8                             Ch;
  UINT8                             Line;
  UINT8                             NumChannelPerMc;
  UINT32                            IntDescTblMinValue;
  CONST UINT32                     *InterleaveMultipliers = NULL;
  UINT32                           *LineOffset;
  UINT16                            InterleaveArrayIndex;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "Build3ChWayIntDescTbls: SktGran %d, ChGran %d, ImcWays %d\n",
              SktGran, ChGran, ImcWays));
  if (NfitPtr == NULL || SktGran < ChGran) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "Building structure of type %d (%a)\n",
          TYPE_INTERLEAVE_DESCRIPTOR, NfitDebugStrcType2Name (TYPE_INTERLEAVE_DESCRIPTOR)));

  if (SktGran == ChGran) {
    LinesDesc = 2;
  } else {
    LinesDesc = SktGran / ChGran;
  }

  NumChannelPerMc = GetNumChannelPerMc ();
  LineSize = ChGran;

  for (Imc = 0; Imc < ImcWays; Imc++) {
    for (Ch = 0; Ch < NumChannelPerMc; Ch++) {
      //
      // Initialize the table headers
      //
      pIntDescTbl = (NFIT_INTERLEAVE_STRUCTURE*)&NfitPtr->NfitTables[NfitPtr->Length];
      pIntDescTbl->Type = TYPE_INTERLEAVE_DESCRIPTOR;
      // Index must be non-zero, i.e., 1 based.
      pIntDescTbl->InterleaveStructureIndex = mInterleaveDescIndex++;
      pIntDescTbl->Length = (UINT16) (sizeof (*pIntDescTbl) + LinesDesc * sizeof (*LineOffset));
      pIntDescTbl->NumberOfLines = LinesDesc;
      pIntDescTbl->LineSize = LineSize;
      //
      // Point to the correct multiplier arrays
      //
      InterleaveArrayIndex = ((Imc * NumChannelPerMc) + Ch) % (ImcWays * CH_3WAY);
      switch (ImcWays) {
      case ONE_IMC_WAY:
        if (SktGran == ChGran) {
          InterleaveMultipliers = &Imc1Way4k_3ChWay4k_LineMultipliers[InterleaveArrayIndex][0];
        } else {
          InterleaveMultipliers = &Imc1Way4k_3ChWay256_LineMultipliers[InterleaveArrayIndex][0];
        }
        break;
      case TWO_IMC_WAY:
        if (SktGran == ChGran) {
          InterleaveMultipliers = &Imc2Way4k_3ChWay4k_LineMultipliers[InterleaveArrayIndex][0];
        } else {
          InterleaveMultipliers = &Imc2Way4k_3ChWay256_LineMultipliers[InterleaveArrayIndex][0];
        }
        break;
      case FOUR_IMC_WAY:
        if (SktGran == ChGran) {
          InterleaveMultipliers = &Imc4Way4k_3ChWay4k_LineMultipliers[InterleaveArrayIndex][0];
        } else {
          InterleaveMultipliers = &Imc4Way4k_3ChWay256_LineMultipliers[InterleaveArrayIndex][0];
        }
        break;
      case EIGHT_IMC_WAY:
        if (SktGran == ChGran) {
          InterleaveMultipliers = &Imc8Way4k_3ChWay4k_LineMultipliers[InterleaveArrayIndex][0];
        } else {
          InterleaveMultipliers = &Imc8Way4k_3ChWay256_LineMultipliers[InterleaveArrayIndex][0];
        }
        break;
      default:
          ASSERT (FALSE);
        return EFI_INVALID_PARAMETER;
      }

      LineOffset = (UINT32 *) ((UINT8 *) pIntDescTbl + sizeof (*pIntDescTbl));

      IntDescTblMinValue = 0xffffffff;
      //
      // Copy the known values from the table
      //
      for (Line = 0;  Line < LinesDesc; Line++) {
        LineOffset[Line] = InterleaveMultipliers[Line];
        if (LineOffset[Line] < IntDescTblMinValue) {
          IntDescTblMinValue = LineOffset[Line];
        }
      }
      //
      // Convert the table values to multiplier indices
      //
      for (Line = 0;  Line < LinesDesc; Line++) {

        LineOffset[Line] -= IntDescTblMinValue;
      }
      //
      // Update the Nfit lenth as appropriate
      //
      ASSERT ((NfitPtr->Length + pIntDescTbl->Length) <= mNfitMaxSize);
      NfitPtr->Length += pIntDescTbl->Length;

      NfitDumpInterleaveStruct (pIntDescTbl);
    }
  }
  return EFI_SUCCESS;
}


/**
 @breif Find SMBIOS Type 17 handle for given NVDIMM.

  Note that this function returns handle for DIMM location,
  it does not determine if DIMM actually exists or the slot is empty.

  @param[in] Skt  - Socket number
  @param[in] Ch   - Channel number in a socket
  @param[in] Dimm - DIMM slot number in a channel

  @return SMBIOS Type17 handle for a given DIMM location is returned, or
          SMBIOS_INVALID_HANDLE if invalid DIMM location was provided.
**/
UINT16
NfitGetSmbiosHandle (
  UINT8            Skt,
  UINT8            Ch,
  UINT8            Dimm
  )
{
  SMBIOS_DIMM_INFO DimmInfo;

  DimmInfo.Socket = Skt;
  DimmInfo.Imc = ConvertSktChToImc (Ch);
  DimmInfo.Channel = ConvertSktChToImcCh (Ch);
  DimmInfo.Dimm = Dimm;
  DimmInfo.Type17Handle = SMBIOS_INVALID_HANDLE;
  mSmbiosInfoProtocol->SmbiosGetDimmByLocation (mSmbiosInfoProtocol, &DimmInfo);
  return DimmInfo.Type17Handle;
}


/**
 @brief This routine basically builds NVDIMM Region Mapping Structure.

 @param[out] NfitPtr         - Pointer to NFIT tables buffer
 @param[in]  Socket          - Socket number
 @param[in]  Ch              - Channel index in socket (not iMC channel)
 @param[in]  Dimm            - DIMM index in channel
 @param[in]  SpaRangeStrcPtr - Pointer to SPA Range Desc Table, NULL if disabled DIMM
 @param[in]  IntIndex        - Interleave Structure Index associated with this region
 @param[in]  RegionOffset    - Region offset
 @param[in]  Ways            - Interleave ways
 @param[in]  MemType         - Type of memory, see MEM_TYPE_<*> definitons

 @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
NfitBuildNvdimmRegionMappingStructure (
  IN OUT NVDIMM_FW_INTERFACE_TABLE *NfitPtr,
  IN     UINT8                      Skt,
  IN     UINT8                      Ch,
  IN     UINT8                      Dimm,
  IN     NFIT_SPA_RANGE_STRUCTURE  *SpaRangeStrcPtr,
  IN     UINT16                     InterleaveIndex,
  IN     UINT64                     RegionOffset,
  IN     UINT16                     Ways,
  IN     UINT16                     MemType
  )
{
  EFI_STATUS               Status;
  UINT8                    MaxChDdr;
  UINT8                    Imc;
  UINT8                    ImcCh;
  UINT16                   CtrlStrcIndex;
  UINT16                   SpaRangeStructureIndex = 0;
  UINT64                   MemoryDeviceRegionSize = 0;
  FIS_GET_SMART_HEALTH_RSP FisRsp;

  NVDIMM                  *DimmPtr;
  UINT16                  *MapIndexToUpdatePtr;

  NFIT_NVDIMM_REGION_MAPPING_STRUCTURE *NvdimmRgnMapPtr;

  MaxChDdr = GetMaxChDdr ();
  Imc = ConvertSktChToImc (Ch);
  ImcCh = ConvertSktChToImcCh (Ch);
  if (NfitPtr == NULL || Skt >= MAX_SOCKET || Ch >= MaxChDdr ||
      ((SpaRangeStrcPtr != NULL) && (MemType == 0 || Ways == 0))) {

    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Skt, Ch, Dimm, NVDIMM_FLAG_ALL);
  if (DimmPtr == NULL) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("NFIT") "S%d.C%d.D%d: No NVDIMM structure found\n", Skt, Ch, Dimm));
    return EFI_NOT_FOUND;
  }
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "Building structure of type %d (%a) for S%d.C%d.D%d MemType %d\n",
          TYPE_MEMDEV_TO_SPA_RANGE_MAP_DESCRIPTOR, NfitDebugStrcType2Name (TYPE_MEMDEV_TO_SPA_RANGE_MAP_DESCRIPTOR),
          Skt, Ch, Dimm, MemType));
  if (SpaRangeStrcPtr != NULL) {

    SpaRangeStructureIndex = SpaRangeStrcPtr->SPARangeStructureIndex;
    ASSERT (Ways != 0);
    MemoryDeviceRegionSize = (SpaRangeStrcPtr->SystemPhysicalAddressRangeLength / Ways);
  }
  if (IsMemTypeAppDirect (MemType)) {

    CtrlStrcIndex = mNfitNvdimmInfo[Skt][Ch][Dimm].AppDirectCtrlStrcIndex;
    MapIndexToUpdatePtr = &mNfitNvdimmInfo[Skt][Ch][Dimm].AppDirectMapStrcIndex;

  } else if (MemType == MemType1lmCtrl) {

    if (mSystemMemoryMap->MemSetup.NfitPublishMailboxStructsDisable) {
      //
      // Mailbox structures in NFIT disabled, ignore this call.
      //
      return EFI_SUCCESS;
    }
    CtrlStrcIndex = mNfitNvdimmInfo[Skt][Ch][Dimm].MailboxCtrlStrcIndex;
    MapIndexToUpdatePtr = &mNfitNvdimmInfo[Skt][Ch][Dimm].MailboxMapStrcIndex;

  } else {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("NFIT") "Will not build %a for unexpected memory type %d passed for S%d.C%d.D%d\n",
            NfitDebugStrcType2Name (TYPE_MEMDEV_TO_SPA_RANGE_MAP_DESCRIPTOR), MemType, Skt, Ch, Dimm));
    return EFI_UNSUPPORTED;
  }
  //
  // At this point NVDIMM Ctrl Region should be already created in the NFIT.
  //
  ASSERT (CtrlStrcIndex != 0);

  // Get the pointer to the next Nfit table, which in this case is MDTSPARMT.
  NvdimmRgnMapPtr = (NFIT_NVDIMM_REGION_MAPPING_STRUCTURE*) &NfitPtr->NfitTables[NfitPtr->Length];
  NvdimmRgnMapPtr->Type = TYPE_MEMDEV_TO_SPA_RANGE_MAP_DESCRIPTOR;
  NvdimmRgnMapPtr->Length = sizeof (*NvdimmRgnMapPtr);
  *(UINT32*)&NvdimmRgnMapPtr->NFITDeviceHandle = NFIT_HANDLE_BUILD (Skt, Imc, ImcCh, Dimm);
  NvdimmRgnMapPtr->NVDIMMPhysicalID = NfitGetSmbiosHandle (Skt, Ch, Dimm); // Find SMBIOS Type 17 handle for this DIMM
  //
  // Contact CLV before changing the trace below, test depends on it.
  //
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "SMBIOSHandle = 0x%X\n", NvdimmRgnMapPtr->NVDIMMPhysicalID));
  NvdimmRgnMapPtr->NVDIMMRegionID = mNvdimmRegionMappingId++;
  NvdimmRgnMapPtr->SPARangeStructureIndex = SpaRangeStructureIndex;
  NvdimmRgnMapPtr->NVDIMMControlRegionStructureIndex = CtrlStrcIndex;
  NvdimmRgnMapPtr->NVDIMMRegionSize = MemoryDeviceRegionSize;
  NvdimmRgnMapPtr->RegionOffset = RegionOffset;
  NvdimmRgnMapPtr->NVDIMMPhysicalAddressRegionBase = 0;
  if (MemType == MemType1lmCtrl) {
    //
    // If this represents mailbox region, DPA of this region in NVDIMM is 0.
    //
    NvdimmRgnMapPtr->NVDIMMPhysicalAddressRegionBase = CTRL_REG_DPA_OFFSET;

  } else if (IsMemTypeAppDirect (MemType) && SpaRangeStrcPtr != NULL) {

    NvdimmRgnMapPtr->NVDIMMPhysicalAddressRegionBase =
                                       GetPmemDpaBase (SpaRangeStrcPtr->SystemPhysicalAddressRangeBase, Skt, Ch, Dimm);
  }
  NvdimmRgnMapPtr->InterleaveStructureIndex = InterleaveIndex;
  NvdimmRgnMapPtr->InterleaveWays = Ways;
  //
  // DIMM device flag has a bitmap to indicate certain failure and SMART events.
  // Update it using SMART and Health Info.
  //
  NvdimmRgnMapPtr->NVDIMMStateFlags = NFIT_DFLAG_HEALTH_NOTIFY_ENABLED;
  if (DimmPtr->SADSpaBase == SPA_NOT_MAPPED) {
    //
    // This NVDIMM is disabled for any mapping, no matter what MemType it is.
    //
    CRDEBUG ((DEBUG_ERROR, CR_WARN_STR ("NFIT") "S%d.C%d.D%d: NVDIMM disabled, report not mapped (MemType %d)\n",
            Skt, Ch, Dimm, MemType));
    NvdimmRgnMapPtr->NVDIMMStateFlags |= NFIT_DFLAG_SMART_EVENTS_OBSERVED | NFIT_DFLAG_NVDIMM_NOT_MAPPED;

  } else {

    if (mSystemMemoryMap->HostDdrtDimmInfo[Skt][Ch].NvdimmUnmapped) {
      CRDEBUG ((DEBUG_ERROR, CR_WARN_STR ("NFIT") "S%d.C%d.D%d: NvdimmUnmapped set, report NVDIMM not mapped (MemType %d)\n",
                Skt, Ch, Dimm, MemType));
      NvdimmRgnMapPtr->NVDIMMStateFlags |= NFIT_DFLAG_NVDIMM_NOT_MAPPED;
    }

    if (SpaRangeStrcPtr == NULL && (GetMediaStatus (DimmPtr) != MEDIA_READY || mSystemMemoryMap->Socket[Skt].ChannelInfo[Ch].DimmInfo[Dimm].nonVolCap != 0)) {
      CRDEBUG ((DEBUG_ERROR, CR_WARN_STR ("NFIT") "S%d.C%d.D%d: No SPA range, report NVDIMM not mapped (MemType %d)\n",
                Skt, Ch, Dimm, MemType));
      NvdimmRgnMapPtr->NVDIMMStateFlags |= NFIT_DFLAG_NVDIMM_NOT_MAPPED;
    }
    if (GetMediaStatus (DimmPtr) != MEDIA_READY) {

      CRDEBUG ((DEBUG_ERROR, CR_WARN_STR ("NFIT") "S%d.C%d.D%d: Media not ready, report NVDIMM SMART observed (MemType %d)\n",
              Skt, Ch, Dimm, MemType));
      NvdimmRgnMapPtr->NVDIMMStateFlags |= NFIT_DFLAG_SMART_EVENTS_OBSERVED;
    }
    //
    // NVDIMM mailbox is is mapped, so we read SMART health state.
    //
    Status = FisRequest (Skt, Ch, Dimm, NULL, 0, &FisRsp.DWord[0], GET_SMART_HEALTH, FIS_DEFTIMEOUT, NULL);
    if (!EFI_ERROR (Status)) {

      if (FisRsp.Bits.HealthStatusVal && FisRsp.Bits.HealthStatus != 0) {

        CRDEBUG ((DEBUG_ERROR, CR_WARN_STR ("NFIT") "S%d.C%d.D%d: Reporting unhealthy state (health status: 0x%02X)\n",
                Skt, Ch, Dimm, FisRsp.Bits.HealthStatus));
        NvdimmRgnMapPtr->NVDIMMStateFlags |= NFIT_DFLAG_SMART_EVENTS_OBSERVED;
      }
      if (FisRsp.Bits.LLastShutdownStatusVal && FisRsp.Bits.LLastShutdownStatus != FIS_LSS_PLATFORM_CLEAN_SHUTDOWN) {

        CRDEBUG ((DEBUG_ERROR, CR_WARN_STR ("NFIT") "S%d.C%d.D%d: Reporting unclear shutdown (LSS: 0x%X, details: 0x%02X%02X)\n",
                Skt, Ch, Dimm, FisRsp.Bits.LLastShutdownStatus,
                FisRsp.Bits.VendorData.LLssExtDetails.Byte[0], FisRsp.Bits.VendorData.LLssDetails.Byte));
        NvdimmRgnMapPtr->NVDIMMStateFlags |= NFIT_DFLAG_PLATFORM_FLUSH_FAIL | NFIT_DFLAG_SMART_EVENTS_OBSERVED;
      }
      if (!FisRsp.Bits.HealthStatusVal || !FisRsp.Bits.LLastShutdownStatusVal) {

        CRDEBUG ((DEBUG_ERROR, CR_WARN_STR ("NFIT") "S%d.C%d.D%d: Health or Latched LSS not valid in FIS response (flags: 0x%X)\n",
                Skt, Ch, Dimm, FisRsp.DWord[0]));
      }
    }
  }
  *MapIndexToUpdatePtr = NvdimmRgnMapPtr->NVDIMMRegionID;

  ASSERT ((NfitPtr->Length + NvdimmRgnMapPtr->Length) <= mNfitMaxSize);
  NfitPtr->Length += NvdimmRgnMapPtr->Length;

  NfitDumpNvdimmRgnMappingStruct (NvdimmRgnMapPtr);
  return EFI_SUCCESS;
} // NfitBuildNvdimmRegionMappingStructure()


/**
  @brief Build NVDIMM Control Region Structure in NFIT table.

  @param[in,out] NfitPtr - Pointer to Nfit Struct
  @param[in]     Skt     - Socket number
  @param[in]     Ch      - Channel number in socket
  @param[in]     Dimm    - DIMM number in channel
  @param[in]     Rfic    - Region format interface code

  @retrun Index of the structure is returned, or zero in case of any error.
**/
UINT16
NfitBuildNvdimmCtrlRegionStructure (
  IN OUT NVDIMM_FW_INTERFACE_TABLE *NfitPtr,
  IN     UINT8                      Skt,
  IN     UINT8                      Ch,
  IN     UINT8                      Dimm,
  IN     UINT16                     Rfic
  )
{
  NVDIMM                               *DimmPtr;
  NFIT_NVDIMM_CONTROL_REGION_STRUCTURE *NvdimmCtrlPtr;

  if (NfitPtr == NULL) {

    ASSERT (FALSE);
    return 0;
  }
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "Building structure of type %d (%a) for S%d.C%d.D%d RFIC %03X\n",
          TYPE_CNTRL_REGION_DESCRIPTOR, NfitDebugStrcType2Name (TYPE_CNTRL_REGION_DESCRIPTOR), Skt, Ch, Dimm, Rfic));
  DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Skt, Ch, Dimm, NVDIMM_FLAG_ALL);
  if (DimmPtr == NULL) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("NFIT") "S%d.C%d.D%d: No NVDIMM information structure found!\n",
            Skt, Ch, Dimm));
    ASSERT (DimmPtr != NULL);
    return 0;
  }
  //
  // Get the pointer of Control Region Table to point to the right area of the Nfit.
  //
  NvdimmCtrlPtr = (NFIT_NVDIMM_CONTROL_REGION_STRUCTURE*)&NfitPtr->NfitTables[NfitPtr->Length];

  NvdimmCtrlPtr->Type                              = TYPE_CNTRL_REGION_DESCRIPTOR;
  NvdimmCtrlPtr->Length                            = sizeof(*NvdimmCtrlPtr);
  NvdimmCtrlPtr->NVDIMMControlRegionStructureIndex = mNvdimmCtrlRegionIndex++;
  NvdimmCtrlPtr->VendorID                          = DimmPtr->VendorID;
  NvdimmCtrlPtr->DeviceID                          = DimmPtr->DeviceID;
  NvdimmCtrlPtr->SubsystemVendorID                 = DimmPtr->SubsystemVendorID;
  NvdimmCtrlPtr->SubsystemDeviceID                 = DimmPtr->SubsystemDeviceID;
  NvdimmCtrlPtr->SubsystemRevisionID               = DimmPtr->SubsystemRevisionID;
  NvdimmCtrlPtr->ValidFields                       = MANUFACTURING_VALID_FIELDS;
  NvdimmCtrlPtr->ManufacturingLocation             = DimmPtr->ManufLocation;
  NvdimmCtrlPtr->ManufacturingDate                 = DimmPtr->ManufDate;
  NvdimmCtrlPtr->SerialNumber                      = DimmPtr->SerialNum;
  NvdimmCtrlPtr->RegionFormatInterfaceCode         = Rfic;
  //
  // Contact CLV before changing the trace below, test depends on it.
  //
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "CntrlRgn VendorID = 0x%04X\n", DimmPtr->VendorID));
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "CntrlRgn DeviceID = 0x%04X\n", DimmPtr->DeviceID));

  if (NvdimmCtrlPtr->RegionFormatInterfaceCode == NFIT_RFIC_STORAGE) {

    NvdimmCtrlPtr->NumberOfBlockControlWindows                = NUM_BLOCK_CTRL_WINDOWS;
    NvdimmCtrlPtr->SizeOfBlockControlWindow                   = BLOCK_CTRL_WIN_SIZE;
    // Blk Control offset is known
    NvdimmCtrlPtr->CommandRegisterOffsetInBlockControlWindow  = BLK_CONTROL_OFFSET;
    NvdimmCtrlPtr->SizeOfCommandRegisterInBlockControlWindows = CR_REG_SIZE;
    NvdimmCtrlPtr->StatusRegisterOffsetInBlockControlWindow   = BLK_CONTROL_STATUS_REG_OFFSET;
    NvdimmCtrlPtr->SizeOfStatusRegisterInBlockControlWindows  = STATUS_REG_SIZE;

  } else {

    NvdimmCtrlPtr->NumberOfBlockControlWindows = 0;
    NvdimmCtrlPtr->Length -= CONTROL_REGION_SIZE_DIFF_NO_BLOCK_WINDOWS;
  }
  ASSERT ((NfitPtr->Length + NvdimmCtrlPtr->Length) <= mNfitMaxSize);
  NfitPtr->Length += NvdimmCtrlPtr->Length;

  NfitDumpCtrlRgnStruct (NvdimmCtrlPtr);
  return NvdimmCtrlPtr->NVDIMMControlRegionStructureIndex;
} // NfitBuildNvdimmCtrlRegionStructure()

/**
  Builds NvDIMM F/W Interface Table for 1 Way Channel Interleave, appending the sub-tables to
  NFIT from the SAD struct passed in.

  @param[in,out] NfitPtr         Pointer to NFIT buffer
  @param[in]     ImcWays         Interleave ways
  @param[in]     SpaRangeStrcPtr SPA Range Structure pointer
  @param[in]     SpaRangeBase    SPA range base address

  @retval EFI_SUCCESS           NvDIMM F/W Interface Table for 1 Way Channel Interleave created successfully
  @retval EFI_INVALID_PARAMETER Invalid ImcWays or pNfit is NULL
**/
EFI_STATUS
NfitBuildMemDevFor1WayChInt (
  IN OUT NVDIMM_FW_INTERFACE_TABLE *NfitPtr,
  IN     UINT8                      ImcWays,
  IN     NFIT_SPA_RANGE_STRUCTURE  *SpaRangeStrcPtr,
  IN     UINT64                     SpaRangeBase
  )
{
  EFI_STATUS  Status;
  UINT16      NumIntTbls;
  UINT8       Skt;
  UINT8       Imc;
  UINT8       ImcCh;
  UINT8       SktCh;
  UINT8       Dimm;
  UINT8       Sad;
  UINT8       MaxImc;
  UINT8       NumChannelPerMc;
  UINT64      SadSpaBase;
  UINT64      RegionOffset;
  UINT32      SktGran;

  SAD_TABLE  *SadPtr;

  if (NfitPtr == NULL || ImcWays == 0) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  NumIntTbls = (ImcWays == 1) ? 0 : mInterleaveDescIndex;
  MaxImc = GetMaxImc ();
  NumChannelPerMc = GetNumChannelPerMc ();
  RegionOffset = 0;  // Region Offset starts as 0 for the 1st Dimm, then increments by Target Granularity.

  for (Skt = 0; Skt < MAX_SOCKET; Skt++) {

    if (!mSystemMemoryMap->Socket[Skt].SocketEnabled) {
      continue;
    }
    for (Sad = 0; Sad < MAX_SAD_RULES; Sad++) {

      SadPtr = &mSystemMemoryMap->Socket[Skt].SAD[Sad];
      if (!SadPtr->local) {
        continue;
      }
      SadSpaBase = NvdimmGetSadBase (Skt, Sad);
      //
      // Check if given SPA Range base belongs to the currently processed SAD
      //
      if (SadSpaBase == SpaRangeBase) {

        SktGran = GetSocketGranularity (SadPtr);
        for (Imc = 0; Imc < MaxImc; Imc++) {
          //
          // Check if currently processed iMC of this socket is involved in this interleave set
          //
          if (mSystemMemoryMap->Socket[Skt].SadIntList[Sad][Skt * MaxImc + Imc]) {
            //
            // Get channel for 1 Way Channel Interleave
            //
            ImcCh = BitmapGetBit (SadPtr->FMchannelInterBitmap[Imc], 0);
            if (ImcCh >= NumChannelPerMc) {

              CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("NFIT") "S%d.MC%d: No channel found for SAD[%d] (iMC mask 0x%X, CH mask 0x%X)\n",
                      Skt, Imc, Sad, SadPtr->imcInterBitmap, SadPtr->FMchannelInterBitmap[Imc]));
              continue;
            }
            SktCh = ConvertImcChToSktCh (Imc, ImcCh);
            //
            // Get NVDIMM location for current socket and channel
            //
            Status = GetChannelDimm (Skt, SktCh, &Dimm);
            if (EFI_ERROR (Status)) {
              ASSERT_EFI_ERROR (Status);
              continue;
            }
            NfitBuildNvdimmRegionMappingStructure (NfitPtr, Skt, SktCh, Dimm, SpaRangeStrcPtr, NumIntTbls, RegionOffset, ImcWays, SadPtr->type);
            if (ImcWays != 1) {
              NumIntTbls++;
              RegionOffset += SktGran;
            }
          }
        } // for (Imc...)
      }
    } // for (Sad...)
  }
  return EFI_SUCCESS;
} // NfitBuildMemDevFor1WayChInt()


/**
  Builds NvDIMM F/W Interface Table for 2 Way Channel Interleave, appending the sub-tables to
  NFIT from the SAD struct passed in.

  @param[in,out] NfitPtr         Pointer to NFIT buffer
  @param[in]     ImcWays         Interleave ways
  @param[in]     SpaRangeStrcPtr SPA Range Structure pointer
  @param[in]     SpaRangeBase    SPA range base address

  @retval EFI_SUCCESS           NvDIMM F/W Interface Table for 2 Way Channel Interleave created successfully
  @retval EFI_INVALID_PARAMETER Invalid ImcWays or pNfit is NULL
**/
EFI_STATUS
NfitBuildMemDevFor2WayChInt (
  IN OUT NVDIMM_FW_INTERFACE_TABLE *NfitPtr,
  IN     UINT8                      ImcWays,
  IN     NFIT_SPA_RANGE_STRUCTURE  *SpaRangeStrcPtr,
  IN     UINT64                     SpaRangeBase
  )
{
  EFI_STATUS  Status;
  UINT64      SadSpaBase;
  UINT64      DimmSpaBase;
  UINT8       Sad;
  UINT8       Skt;
  UINT8       Imc;
  UINT8       ImcCh;
  UINT8       SktCh;
  UINT8       Dimm;
  UINT8       MaxImc;
  UINT8       NumChannelPerMc;
  UINT32      SktGran;
  UINT32      ChGran;
  UINT16      NumIntTbls;

  SAD_TABLE  *SadPtr;
  NVDIMM     *DimmPtr;

  if (NfitPtr == NULL || ImcWays == 0) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  MaxImc = GetMaxImc ();
  NumChannelPerMc = GetNumChannelPerMc ();

  for (Skt = 0; Skt < MAX_SOCKET; Skt++) {

    if (!mSystemMemoryMap->Socket[Skt].SocketEnabled) {
      continue;  // Skip disabled sockets
    }
    for (Sad = 0; Sad < MAX_SAD_RULES; Sad++) {

      SadPtr = &mSystemMemoryMap->Socket[Skt].SAD[Sad];
      if (!SadPtr->local) {
        continue;
      }
      SadSpaBase = NvdimmGetSadBase (Skt, Sad);
      //
      // Check if given SPA Range base belongs to the currently processed SAD
      //
      if (SadSpaBase == SpaRangeBase) {
        //
        // Get socket and channel granularity
        //
        SktGran = GetSocketGranularity (SadPtr);
        ChGran = GetChannelGranularity (SadPtr);
        NumIntTbls = mInterleaveDescIndex;
        for (Imc = 0; Imc < MaxImc; Imc++) {
          //
          // Check if currently processed iMC of this socket is involved in this interleave set
          //
          if (mSystemMemoryMap->Socket[Skt].SadIntList[Sad][(Skt * MaxImc) + Imc]) {

            for (ImcCh = BitmapGetBit (SadPtr->FMchannelInterBitmap[Imc], 0);
                 ImcCh < NumChannelPerMc;
                 ImcCh = BitmapGetBit (SadPtr->FMchannelInterBitmap[Imc], ImcCh + 1)) {

              SktCh = ConvertImcChToSktCh (Imc, ImcCh);
              Status = GetChannelDimm (Skt, SktCh, &Dimm);
              if (EFI_ERROR (Status) || (DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Skt, SktCh, Dimm, NVDIMM_FLAG_ENABLED)) == NULL) {

                CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("NFIT") "S%d.C%d: No NVDIMM found for SAD[%d] (iMC mask 0x%X, CH mask 0x%X)\n",
                        Skt, SktCh, Sad, SadPtr->imcInterBitmap, SadPtr->FMchannelInterBitmap[Imc]));
                ASSERT_EFI_ERROR (Status);
                continue;
              }
              Status = XlateDpaOffsetToSpa (DimmPtr, SadSpaBase, 0, &DimmSpaBase);
              if (EFI_ERROR (Status)) {

                ASSERT_EFI_ERROR (Status);
                continue;
              }
              NfitBuildNvdimmRegionMappingStructure (NfitPtr, Skt, SktCh, Dimm, SpaRangeStrcPtr, NumIntTbls, DimmSpaBase - SadSpaBase, (2 * ImcWays), SadPtr->type);
              NumIntTbls++;
            } // for (ImcCh...)
          }
        } // for (Imc...)
      }
    } // for (Sad...)
  }
  return EFI_SUCCESS;
} // BuildMemDevFor2WayChInt()


/**
  Builds NvDIMM F/W Interface Table for 3 Way Channel Interleave, appending the sub-tables to
  NFIT from the SAD struct passed in.

  @param[in,out] NfitPtr         Pointer to NFIT buffer
  @param[in]     ImcWays         Interleave ways
  @param[in]     SpaRangeStrcPtr SPA Range Structure pointer
  @param[in]     SpaRangeBase    SPA range base address

  @retval EFI_SUCCESS           NvDIMM F/W Interface Table for 3 Way Channel Interleave created successfully
  @retval EFI_INVALID_PARAMETER Invalid ImcWays or NfitPtr is NULL
**/
EFI_STATUS
NfitBuildMemDevFor3WayChInt (
  IN OUT NVDIMM_FW_INTERFACE_TABLE *NfitPtr,
  IN     UINT8                      ImcWays,
  IN     NFIT_SPA_RANGE_STRUCTURE  *SpaRangeStrcPtr,
  IN     UINT64                     SpaRangeBase
  )
{
  EFI_STATUS  Status;
  UINT64      SadSpaBase;
  UINT16      NumIntTbls = mInterleaveDescIndex;
  UINT8       Imc;
  UINT8       Socket;
  UINT8       i;
  UINT8       Channel;
  UINT8       Dimm;
  UINT8       Sad;
  UINT8       DimmIntSetNum = 0;
  UINT8       MaxImc;
  UINT8       NumChannelPerMc;
  UINT32      ChGran;
  UINT32      SktGran;
  UINT32      OffsetMultiplier;

  static UINT32 RgnOffsetMultiplier[48] = { // MAX_SOCKET * MAX_MC_CH * MAX_IMC (for 8 sockets)
                                           0, 1, 2, 18, 16, 17,
                                           33, 34, 32, 48, 49, 50,
                                           66, 64, 65, 81, 82, 80,
                                           96, 97, 98, 114, 112, 113,
                                           129, 130, 128, 144, 145, 146,
                                           162, 160, 161, 177, 178, 176,
                                           192, 193, 194, 210, 208, 209,
                                           225, 226, 224, 240, 241, 242
                                          };
  SAD_TABLE  *SadPtr;

  if (NfitPtr == NULL || ImcWays == 0) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  MaxImc = GetMaxImc ();
  NumChannelPerMc = GetNumChannelPerMc ();

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

    if (!mSystemMemoryMap->Socket[Socket].SocketEnabled) {
      continue;  // Skip disabled sockets
    }
    for (Sad = 0; Sad < MAX_SAD_RULES; Sad++) {

      SadPtr = &mSystemMemoryMap->Socket[Socket].SAD[Sad];
      if (!SadPtr->local) {
        continue;
      }
      SadSpaBase = NvdimmGetSadBase (Socket, Sad);
      //
      // Check if given SPA Range base belongs to the currently processed SAD
      //
      if (SadSpaBase == SpaRangeBase) {
        //
        // Get socket and channel granularity
        //
        SktGran = GetSocketGranularity (SadPtr);
        ChGran = GetChannelGranularity (SadPtr);

        for (Imc = 0; Imc < MaxImc; Imc++) {
          //
          // Check if currently processed iMC of this socket is involved in this interleave set
          //
          if ((mSystemMemoryMap->Socket[Socket].SadIntList[Sad][(Socket * MaxImc) + Imc])) {

            for (i = 0; i < CH_3WAY; i++) {
              //
              // Calculate offset multiplier
              //
              OffsetMultiplier = (SktGran == ChGran) ? GetLineMultiplier (SktGran, ChGran, ImcWays, CH_3WAY, DimmIntSetNum++, 0) : RgnOffsetMultiplier[DimmIntSetNum++];

              //
              // Calculate channel number
              //
              if (i == 0) {
                Channel = ((SadSpaBase / ChGran) % CH_3WAY) + (NumChannelPerMc * Imc);
              } else {
                Channel = (Channel == (CH_3WAY - 1 + (NumChannelPerMc * Imc))) ? (NumChannelPerMc * Imc) : (Channel + 1);
              }

              //
              // Get NVDIMM location for current socket and channel
              //
              Status = GetChannelDimm (Socket, Channel, &Dimm);
              if (EFI_ERROR (Status)) {
                CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("NFIT") "S%d.C%d: No NVDIMM found for SAD[%d] (iMC mask 0x%X, CH mask 0x%X)\n",
                        Socket, Channel, Sad, SadPtr->imcInterBitmap, SadPtr->FMchannelInterBitmap[Imc]));
                return Status;
              }
              //
              // Create Memory Device To SPA Range Map Table
              //
              NfitBuildNvdimmRegionMappingStructure (NfitPtr, Socket, Channel, Dimm, SpaRangeStrcPtr, NumIntTbls, OffsetMultiplier * ChGran, (3 * ImcWays), SadPtr->type);
              NumIntTbls++;
            }
          }
        }
      }
    }
  }
  return EFI_SUCCESS;
} // NfitBuildMemDevFor3WayChInt()


/**
  This routine adds Flush Hint Address Descriptor Tables to Nfit.

  @param NfitPtr - Pointer to NFIT buffer

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
NfitBuildFlushHintAddrStructure (
  IN OUT NVDIMM_FW_INTERFACE_TABLE *NfitPtr
  )
{
  UINT8                   Socket;
  UINT8                   Imc;
  UINT8                   Dimm;
  UINT8                   SktCh;
  UINT8                   ImcCh;
  UINT8                   MaxImc;
  UINT8                   NumChannelPerMc;
  NFIT_FLUSH_HINT_ADDRESS_STRUCTURE *FlushHintStrcPtr;
  UINT64                            *FlushHintAddressPtr;

  if (NfitPtr == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  MaxImc = GetMaxImc ();
  NumChannelPerMc = GetNumChannelPerMc ();

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Imc = 0; Imc < MaxImc; Imc++) {
      for (ImcCh = 0; ImcCh < NumChannelPerMc; ImcCh++) {
        //
        // Convert the channel# per MC into channel# per socket
        //
        SktCh = ConvertImcChToSktCh (Imc, ImcCh);

        if (mMemMapHost->SocketInfo[Socket].ChannelInfo[SktCh].WpqFlushHintAddress == 0) {
          continue;
        }

        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

          if (!IsNvmDimm (Socket, SktCh, Dimm)) {
            continue;
          }
          CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "Building structure of type %d (%a)\n",
                  TYPE_FLUSH_HINT_DESCRIPTOR, NfitDebugStrcType2Name (TYPE_FLUSH_HINT_DESCRIPTOR)));
          FlushHintStrcPtr = (NFIT_FLUSH_HINT_ADDRESS_STRUCTURE*)&NfitPtr->NfitTables[NfitPtr->Length];
          FlushHintStrcPtr->Type = TYPE_FLUSH_HINT_DESCRIPTOR;
          *(UINT32*)&FlushHintStrcPtr->NFITDeviceHandle = NFIT_HANDLE_BUILD (Socket, Imc, ImcCh, Dimm);
          FlushHintStrcPtr->NumberOfFlushHintAddresses = 1;
          FlushHintStrcPtr->Length = sizeof (*FlushHintStrcPtr) + FlushHintStrcPtr->NumberOfFlushHintAddresses * sizeof (*FlushHintAddressPtr);
          FlushHintAddressPtr = (UINT64*)&FlushHintStrcPtr[1];
          *FlushHintAddressPtr = mMemMapHost->SocketInfo[Socket].ChannelInfo[SktCh].WpqFlushHintAddress;

          ASSERT ((NfitPtr->Length + FlushHintStrcPtr->Length) <= mNfitMaxSize);
          NfitPtr->Length += FlushHintStrcPtr->Length;

          NfitDumpFlushHintAddrStruct (FlushHintStrcPtr);
        }
      }
    }
  }
  return EFI_SUCCESS;
} // NfitBuildFlushHintAddrStructure()


/**

  Returns NVIDMM ADR global status for NFIT Platform Capabilities Structure

  @param VOID             Nothing

  @retval TRUE            NVIDMM ADR global status is enabled
  @retval FALSE           NVIDMM ADR global status is disabled

**/
BOOLEAN
NfitGetPlatformAdrStateforNvdimms (
  VOID
  )
{
  UINT8       Nvdimm;
  BOOLEAN     State;

  for (Nvdimm = 0; Nvdimm < mCrInfo.NvdimmInfo.NumNvdimms; Nvdimm++) {
    //
    // Skip not mapped and unhealthy NVDIMMs
    //
    if (!((mCrInfo.NvdimmInfo.Nvdimms[Nvdimm].SADSpaBase != SPA_NOT_MAPPED) &&
        (GetMediaStatus (&mCrInfo.NvdimmInfo.Nvdimms[Nvdimm]) == MEDIA_READY))) {
      continue;
    }

    //
    // Get NVDIMM ADR state
    //
    State = mSystemMemoryMap->AdrStateForPmemModule[mCrInfo.NvdimmInfo.Nvdimms[Nvdimm].SocketId][mCrInfo.NvdimmInfo.Nvdimms[Nvdimm].Ch];

    if (!State) {
      //
      // ADR status is retrieved as Platform Capabilities,
      // so if disabled for one DIMM then assuming gloabal
      // status as disabled also
      //
      return FALSE;
    }
  }

  return TRUE;
}

/**

  Returns Extended ADR global status for NFIT Platform Capabilities Structure

  @param VOID             Nothing

  @retval TRUE            eADR global status is enabled
  @retval FALSE           eADR global status is disabled

**/
BOOLEAN
NfitGetPlatformExtendedAdrState (
  VOID
  )
{
  EFI_STATUS        Status;
  UINTN             DataSize;
  UINT32            Attributes = EFI_VARIABLE_BOOTSERVICE_ACCESS;
  EXT_ADR_STATE     ExtAdrState;
  EFI_GUID          ExtAdrStateUefiVariableGuid = CR_EXT_ADR_STATE_UEFI_VARIABLE_GUID;
  EXT_ADR_FLOWS     ExtAdrFlows;
  EFI_GUID          ExtAdrFlowsUefiVariableGuid = CR_EXT_ADR_FLOWS_UEFI_VARIABLE_GUID;

  DataSize = sizeof (EXT_ADR_STATE);

  Status = gRT->GetVariable (CR_EXT_ADR_STATE_UEFI_VARIABLE_NAME, &ExtAdrStateUefiVariableGuid, &Attributes, &DataSize, &ExtAdrState);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("NFIT") "Getting Extended ADR state UEFI variable failed (%r)\n", Status));
    return FALSE;
  }

  DataSize = sizeof (EXT_ADR_FLOWS);

  Status = gRT->GetVariable (CR_EXT_ADR_FLOWS_UEFI_VARIABLE_NAME, &ExtAdrFlowsUefiVariableGuid, &Attributes, &DataSize, &ExtAdrFlows);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("NFIT") "Getting Extended ADR flows UEFI variable failed (%r)\n", Status));
    return FALSE;
  }

  return ((ExtAdrState == EXT_ADR_ENABLED) && (ExtAdrFlows.FlowsBits.AcLossIioAndCpuCacheFlush));
}


/**
  Adds Platform Capabilities Structure to NFIT.

  @param[in]  NfitPtr  Pointer to NFIT

**/
EFI_STATUS
NfitBuildPlatformCapabilitiesStructure (
  IN NVDIMM_FW_INTERFACE_TABLE  *NfitPtr
  )
{
  UINT16                                Length;
  NFIT_PLATFORM_CAPABILITIES_STRUCTURE *PlatCapPtr;

  if (NfitPtr == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "Building structure of type %d (%a)\n",
          PLATFORM_CAPABILITIES_STRUCTURE_TYPE, NfitDebugStrcType2Name (PLATFORM_CAPABILITIES_STRUCTURE_TYPE)));

  Length = sizeof (*PlatCapPtr);
  ASSERT (NfitPtr->Length + Length <= mNfitMaxSize);

  PlatCapPtr = (NFIT_PLATFORM_CAPABILITIES_STRUCTURE*)&NfitPtr->NfitTables[NfitPtr->Length];
  PlatCapPtr->Type = PLATFORM_CAPABILITIES_STRUCTURE_TYPE;
  PlatCapPtr->Length = Length;
  PlatCapPtr->HighestValidCapability = 2;
  if (NfitGetPlatformAdrStateforNvdimms ()) {
    PlatCapPtr->Capabilities = PLATFORM_CAPABILITY_MEMORY_CONTROLLER_FLUSH;
  }
  if ((PlatformPmemConfiguration () == WBCACHED_PMEM_PRESENT) ||
      NfitGetPlatformExtendedAdrState() ||
      IsFadrEnabled ()) {
    PlatCapPtr->Capabilities |= PLATFORM_CAPABILITY_CPU_CACHE_FLUSH;
  }
  NfitPtr->Length += Length;

  NfitDumpPlatCapStruct (PlatCapPtr);

  return EFI_SUCCESS;
} // NfitBuildPlatformCapabilitiesStructure()


/**
  @brief This function builds NFIT structures for control region mapped via control region block decoder.

  The control region in block decoder is interleaved different than typical SAD region and thus it is
  handled with separate function.

  @param[out] NfitPtr -  pointer to Nvdimm f/w interface table

  @return Void.
**/
VOID
NfitCreateSubTblsForCrbd (
  OUT NVDIMM_FW_INTERFACE_TABLE *NfitPtr
  )
{
  UINT8                  Skt;
  UINT8                  Ch;
  UINT8                  Dimm;
  UINT8                  Idx;
  UINT8                  Imc;
  UINT8                  ImcCh;
  UINT32                *LineOffsetTable;
  NFIT_SPA_RANGE_STRUCTURE  *SpaRangeStrPtr;
  NFIT_INTERLEAVE_STRUCTURE *IntStrPtr;

  if (NfitPtr == NULL || mCrInfo.CrbdAddrLimit <= mCrInfo.CrbdAddrBase) {

    ASSERT (NfitPtr != NULL);
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "No control region block decoder in this system\n")); // SKX case
    return;
  }
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "Building structure of type %d (%a)\n",
          TYPE_SPA_RANGE_DESCRIPTOR, NfitDebugStrcType2Name (TYPE_SPA_RANGE_DESCRIPTOR)));
  //
  // Calculate pointer for the SPA Range Structure in NFIT table as the first byte after current NFIT content,
  // and setup SPA Range Structure for the control region block decoder.
  //
  SpaRangeStrPtr = (NFIT_SPA_RANGE_STRUCTURE*)&NfitPtr->NfitTables[NfitPtr->Length];
  SpaRangeStrPtr->Type = TYPE_SPA_RANGE_DESCRIPTOR;
  SpaRangeStrPtr->SPARangeStructureIndex = mSpaRangeIndex++;
  SpaRangeStrPtr->Flags = 0;           // Proximity domain not provided for block decoder.
  SpaRangeStrPtr->ProximityDomain = 0; // It is UMA interleaving over all sockets.
  if (mSystemMemoryMap->MemSetup.dfxMemSetup.DfxNfitNvdimmSkuBasedDisable &&
      !mSystemMemoryMap->MemSetup.dfxMemSetup.DfxNfitMailboxWithPropGuid) {

    CopyGuid (&SpaRangeStrPtr->AddressRangeTypeGUID, &AcpiCtrlRegionGuid);

  } else {

    CopyGuid (&SpaRangeStrPtr->AddressRangeTypeGUID, &IntelCtrlRegionGuid);
  }
  SpaRangeStrPtr->SystemPhysicalAddressRangeBase = mCrInfo.CrbdAddrBase;
  SpaRangeStrPtr->SystemPhysicalAddressRangeLength = mCrInfo.CrbdAddrLimit - mCrInfo.CrbdAddrBase;
  SpaRangeStrPtr->AddressRangeMemoryMappingAttribute = EFI_MEMORY_UC;
  //
  // Shrink control region size per each NVDIMM so MC Flush Hint Address Offset lies outside the region.
  //
  ASSERT (CONTROL_REGION_TRIM_SIZE * CR_CRBD_INTWAYS < SpaRangeStrPtr->SystemPhysicalAddressRangeLength);
  SpaRangeStrPtr->SystemPhysicalAddressRangeLength -= CONTROL_REGION_TRIM_SIZE * CR_CRBD_INTWAYS;

  SpaRangeStrPtr->Length = sizeof (*SpaRangeStrPtr);
  ASSERT ((NfitPtr->Length + SpaRangeStrPtr->Length) <= mNfitMaxSize);
  NfitPtr->Length += SpaRangeStrPtr->Length;

  NfitDumpSpaRangeStruct (SpaRangeStrPtr);

  //
  // Create one Interleave Structure for control region block decoder.
  // Interleaving is fixed at MaxSkt * MaxCh * MaxMc = 128 ways.
  //
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "Building structure of type %d (%a)\n",
          TYPE_INTERLEAVE_DESCRIPTOR, NfitDebugStrcType2Name (TYPE_INTERLEAVE_DESCRIPTOR)));
  IntStrPtr = (NFIT_INTERLEAVE_STRUCTURE*)&NfitPtr->NfitTables[NfitPtr->Length];
  IntStrPtr->Type = TYPE_INTERLEAVE_DESCRIPTOR;
  IntStrPtr->Length = sizeof (*IntStrPtr) + CR_CRBD_INTWAYS * sizeof (LineOffsetTable[0]);
  IntStrPtr->InterleaveStructureIndex = mInterleaveDescIndex++;
  IntStrPtr->NumberOfLines = CR_CRBD_INTWAYS;
  IntStrPtr->LineSize = CR_CRBD_LINESIZE;
  LineOffsetTable = (UINT32*)&IntStrPtr[1];

  for (Idx = 0; Idx < CR_CRBD_INTWAYS; Idx++) {

    LineOffsetTable[Idx] = Idx * CR_CRBD_INTWAYS;
  }
  ASSERT ((NfitPtr->Length + IntStrPtr->Length) <= mNfitMaxSize);
  NfitPtr->Length += IntStrPtr->Length;

  NfitDumpInterleaveStruct (IntStrPtr);
  //
  // Create NVDIMM Region Mapping Structures (Type 1) for DIMMs in CRBD.
  //
  for (Idx = 0; Idx < mCrInfo.NvdimmInfo.NumNvdimms; Idx++) {

    if (mCrInfo.NvdimmInfo.Nvdimms[Idx].SADSpaBase == SPA_NOT_MAPPED) {

      continue; // Skip unmapped DIMM, it will be added in NfitCreateSubTblsForNotMappedDimms()
    }
    Skt = (UINT8)mCrInfo.NvdimmInfo.Nvdimms[Idx].SocketId;
    Ch  = mCrInfo.NvdimmInfo.Nvdimms[Idx].Ch;
    Dimm = mCrInfo.NvdimmInfo.Nvdimms[Idx].Dimm;
    Imc = ConvertSktChToImc (Ch);
    ImcCh = ConvertSktChToImcCh (Ch);
    NfitBuildNvdimmRegionMappingStructure (NfitPtr, Skt, Ch, Dimm, SpaRangeStrPtr, IntStrPtr->InterleaveStructureIndex,
                                           CR_CRBD_DIMM_OFFSET (Skt, Imc, ImcCh),
                                           CR_CRBD_INTWAYS, MemType1lmCtrl);
  }
  return;
} // NfitCreateSubTblsForCrbd()


/**
  Builds NvDIMM F/W Interface Table, appending the sub-tables to NFIT
  using the information from the SAD structure of given index.
  Control strucures are handled separately.

  @param pNfit                  Pointer to NFIT Struct
  @param Socket                 Socket number
  @param SadIdx                 SAD Index

  @retval EFI_SUCCESS           NvDIMM F/W Interface Table created successfully
  @retval EFI_INVALID_PARAMETER Invalid Socket/ SadInx or pNfit is NULL
**/
EFI_STATUS
NfitCreateSubTbls (
   NVDIMM_FW_INTERFACE_TABLE  *NfitPtr,
   UINT8                       Skt,
   UINT8                       SadIdx
   )
{
  SAD_TABLE            *pSAD;
  SPA_RANGE_DESC_TABLE *pSPARDT = NULL;
  UINT64                SpaBase = 0;
  UINT8                 ChInterleave;
  UINT8                 ImcWays;
  UINT32                InterBitmap;
  UINT8                 Imc;
  BOOLEAN               IsRedundantSAD;
  UINT32                SktGran, ChGran;
  UINT64                TrimSize;
  EFI_STATUS            Status;

  // If we would go outside bounds for SadIntList[SadIdx], return.
  if (NfitPtr == NULL || Skt >= MAX_SOCKET || SadIdx >= MAX_SAD_RULES) {
    return EFI_INVALID_PARAMETER;
  }
  pSAD = &mSystemMemoryMap->Socket[Skt].SAD[SadIdx];
  //
  // Determine which IMC channel bitmap to use
  //
  Imc = GetMcIndexFromBitmap (pSAD->imcInterBitmap);

  ChInterleave = GetChInterleave (Skt, Imc, SadIdx);
  ImcWays = GetImcWays (Skt, SadIdx);
  InterBitmap = GetChInterleaveBitmap (Skt, SadIdx);
  SktGran = GetSocketGranularity (pSAD);
  ChGran = GetChannelGranularity (pSAD);
  //
  // This SAD is to be added to the Nfit only when this memory range hasn't been added before thru another SAD previously
  //
  IsRedundantSAD = NfitCheckIfRedundantSad (pSAD);
  if (!IsRedundantSAD) {

    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "Building structure of type %d (%a) for SAD[%d] of type %d\n",
            TYPE_SPA_RANGE_DESCRIPTOR, NfitDebugStrcType2Name (TYPE_SPA_RANGE_DESCRIPTOR), SadIdx, pSAD->type));

    // Get the pointer to the next Nfit table, which in this case is SPARDT.
    pSPARDT = (SPA_RANGE_DESC_TABLE*) (UINTN) (&NfitPtr->NfitTables[0] + NfitPtr->Length);
    pSPARDT->Type = TYPE_SPA_RANGE_DESCRIPTOR;

    // Right attributes will be set further down below, start with 0.
    pSPARDT->AddressRangeMemoryMappingAttribute = 0;

    if (IsMemTypeAppDirect (pSAD->type)) {

      CRDEBUG ((DEBUG_INFO, CR_INFO_STR("NFIT") "SAD[%d] is either PMem or PMem_Cache\n", SadIdx));
      CopyGuid(&pSPARDT->AddressRangeTypeGUID,  &AcpiPmemRegionGuid);
      pSPARDT->AddressRangeMemoryMappingAttribute = EFI_MEMORY_NV | EFI_MEMORY_WB;

    } else if (pSAD->type == MemType1lmCtrl) {

      CRDEBUG ((DEBUG_INFO, CR_INFO_STR("NFIT") "SAD[%d] is Mailbox Region\n", SadIdx));
      if (mSystemMemoryMap->MemSetup.dfxMemSetup.DfxNfitNvdimmSkuBasedDisable &&
          !mSystemMemoryMap->MemSetup.dfxMemSetup.DfxNfitMailboxWithPropGuid) {

        CopyGuid (&pSPARDT->AddressRangeTypeGUID, &AcpiCtrlRegionGuid);

      } else {

        CopyGuid (&pSPARDT->AddressRangeTypeGUID, &IntelCtrlRegionGuid);
      }
      //
      // Mem type uncacheable as its attribute for control region, per spec
      //
      pSPARDT->AddressRangeMemoryMappingAttribute = EFI_MEMORY_UC;

    }

    if (pSAD->mirrored != 0) {

      pSPARDT->AddressRangeMemoryMappingAttribute |= EFI_MEMORY_MORE_RELIABLE;
    }

    pSPARDT->SPARangeStructureIndex = mSpaRangeIndex++;

    SpaBase = NvdimmGetSadBase (Skt, SadIdx);
    pSPARDT->SystemPhysicalAddressRangeBase = SpaBase;
    pSPARDT->SystemPhysicalAddressRangeLength = ((UINT64)pSAD->Limit << BITS_64MB_TO_BYTES) - SpaBase;
    //
    // Shrink control region size per each DIMM so MC Flush Hint Address Offset lies outside the region
    //
    if (pSAD->type == MemType1lmCtrl) {

      TrimSize = CONTROL_REGION_TRIM_SIZE * ImcWays * ChInterleave;
      ASSERT (TrimSize < pSPARDT->SystemPhysicalAddressRangeLength);
      pSPARDT->SystemPhysicalAddressRangeLength -= TrimSize;
    }
    //
    // Update the Proximity Domain and Flag of the SPARDT
    //
    Status = GetProximityDomainOf (pSPARDT->SystemPhysicalAddressRangeBase,
                                   pSPARDT->SystemPhysicalAddressRangeLength,
                                   pSAD->type, &pSPARDT->ProximityDomain);
    if (!EFI_ERROR (Status)) {

      pSPARDT->Flags = PROX_DOMAIN_VALID;
    }

    pSPARDT->Length = sizeof (SPA_RANGE_DESC_TABLE);
    //
    // At this point we've updated the SPARDT table, include its length into Nfit Length
    //
    ASSERT ((NfitPtr->Length + pSPARDT->Length) <= mNfitMaxSize);
    NfitPtr->Length += pSPARDT->Length;

    NfitDumpSpaRangeStruct ((NFIT_SPA_RANGE_STRUCTURE*)pSPARDT);
  }

  switch (ChInterleave) {
    case CH_1WAY:
      // 1st Case - if channel is interleaved across both IMCs
      // 1 or 2 iMC, 1 Way Channel Interleaved, but which channel is Interleaved?
      if (!IsRedundantSAD) {
        if (ImcWays != 0) {
          NfitBuildMemDevFor1WayChInt (NfitPtr, ImcWays, pSPARDT, SpaBase);
        }
        NfitBuild1ChWayIntDescTbls (NfitPtr, SktGran, ChGran, ImcWays);
      }
      break;
    case CH_2WAY:
      // Build Nvdimm F/W Interface Table 2 way Channel Interleaved
      if (!IsRedundantSAD) {
        NfitBuildMemDevFor2WayChInt (NfitPtr, ImcWays, pSPARDT, SpaBase);
        NfitBuild2ChWayIntDescTbls (NfitPtr, SktGran, ChGran, ImcWays);
      }
      break;
    case CH_3WAY:
      // Build Nvdimm F/W Interface Table 3 way Channel Interleaved
      if (!IsRedundantSAD) {
        NfitBuildMemDevFor3WayChInt (NfitPtr, ImcWays, pSPARDT, SpaBase);
        NfitBuild3ChWayIntDescTbls (NfitPtr, SktGran, ChGran, ImcWays);
      }
    break;
  default:
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("NFIT") "Unexpected channel wayness %d for SAD[%d][%d] of type %d, on iMC %d",
            ChInterleave, Skt, SadIdx, pSAD->type, Imc));
    CRDEBUG ((DEBUG_ERROR, " (McMap %02X, ChMap %02X)\n", pSAD->imcInterBitmap, pSAD->FMchannelInterBitmap[Imc]));
    ASSERT (FALSE);
  }
  return EFI_SUCCESS;
} // NfitCreateSubTbls()


/**
  Builds NVDIMM Region Mapping Structures for disabled DIMMs and adds them to Nfit

  @param NfitPtr   - Pointer to Nfit Struct

  @retval VOID
**/
EFI_STATUS
NfitCreateSubTblsForNotMappedDimms (
  NVDIMM_FW_INTERFACE_TABLE *NfitPtr
  )
{
  UINT8  Skt;
  UINT8  Ch;
  UINT8  Dimm;
  UINT8  Idx;

  for (Idx = 0; Idx < mCrInfo.NvdimmInfo.NumNvdimms; Idx++) {

    Skt = (UINT8)mCrInfo.NvdimmInfo.Nvdimms[Idx].SocketId;
    Ch = mCrInfo.NvdimmInfo.Nvdimms[Idx].Ch;
    Dimm = mCrInfo.NvdimmInfo.Nvdimms[Idx].Dimm;

    if (mNfitNvdimmInfo[Skt][Ch][Dimm].MailboxMapStrcIndex == 0 &&
        mNfitNvdimmInfo[Skt][Ch][Dimm].MailboxCtrlStrcIndex != 0) {
      //
      // Mailbox region not mapped, NVDIMM is disabled, create Mapping Structure without SPA to indicate that.
      // If PMEM shares the same Ctrl Strc use the same Mapping for PMEM to avoid creating two unmapped structures.
      //
      NfitBuildNvdimmRegionMappingStructure (NfitPtr, Skt, Ch, Dimm, NULL, 0, 0, 0, MemType1lmCtrl);

      if (mNfitNvdimmInfo[Skt][Ch][Dimm].AppDirectCtrlStrcIndex == mNfitNvdimmInfo[Skt][Ch][Dimm].MailboxCtrlStrcIndex) {
        mNfitNvdimmInfo[Skt][Ch][Dimm].AppDirectMapStrcIndex = mNfitNvdimmInfo[Skt][Ch][Dimm].MailboxMapStrcIndex;
      }
    }
    if (mNfitNvdimmInfo[Skt][Ch][Dimm].AppDirectMapStrcIndex == 0 &&
        mNfitNvdimmInfo[Skt][Ch][Dimm].AppDirectCtrlStrcIndex != 0) {
      //
      // There was no PMEM SAD for this NVDIMM so media is not mapped, could be incomplete interleave set.
      // Create mapping structure without SPA to indicate that.
      //
      NfitBuildNvdimmRegionMappingStructure (NfitPtr, Skt, Ch, Dimm, NULL, 0, 0, 0, MemType1lmAppDirect);
    }
  }
  return EFI_SUCCESS;
} // NfitCreateSubTblsForNotMappedDimms()


/**
  Verifies if information from the SAD should be included in the NvDIMM F/W Interface Table

  @param Socket   Socket number
  @param SadInx   SAD Index that needs to be added to NFIT

  @retval TRUE    NFIT should be appended
  @retval FALSE   NFIT should not be appended
**/
BOOLEAN
NfitCheckIfCreateSubTbls (
  UINT8 Skt,
  UINT8 Sad
  )
{
  if (Skt >= NELEMENTS (mSystemMemoryMap->Socket) || Sad >= NELEMENTS (mSystemMemoryMap->Socket[Skt].SAD)) {
    ASSERT (FALSE);
    return FALSE;
  }
  if (mSystemMemoryMap->Socket[Skt].SAD[Sad].local) {
    //
    // Only local SADs
    //
    if (!IsMemTypeReserved (mSystemMemoryMap->Socket[Skt].SAD[Sad].type)) {
      //
      // Only type of PMEM, cached PMEM, CTRL should be taken into consideration
      //
      if (IsMemTypeAppDirect (mSystemMemoryMap->Socket[Skt].SAD[Sad].type) ||
           (mSystemMemoryMap->Socket[Skt].SAD[Sad].type == MemType1lmCtrl &&
            !mSystemMemoryMap->MemSetup.NfitPublishMailboxStructsDisable)) {

        return TRUE;
      }
    }
  }
  return FALSE;
}


/**
  Updates the checksum field of already initialized NFIT table.

  @param TablePtr   Pointer to NFIT table in platform ACPI

  @retval EFI_SUCCESS           Checksum updated successfully
  @retval EFI_INVALID_PARAMETER TablePtr is NULL
**/
EFI_STATUS
NfitUpdateTableChecksum (
  IN OUT NVDIMM_FW_INTERFACE_TABLE *NfitPtr
  )
{
  if (NfitPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  NfitPtr->Checksum = NfitComputeTblChsum ((UINT8*)NfitPtr, NfitPtr->Length);
  return EFI_SUCCESS;
}


/**
  Builds NVDIMM FW Interface Table (NFIT) appending the needed structures.

  @param[out] TablePtr  - Pointer to a buffer in platform ACPI for NFIT table.
  @param[in]  TableSize - Size of the buffer for NFIT table.

  @return Standard EFI status is returned.
**/
EFI_STATUS
NfitBuildTable (
  IN OUT NVDIMM_FW_INTERFACE_TABLE *NfitPtr,
  IN     UINTN                      NfitMaxSize
  )
{
  EFI_STATUS                  Status;
  ACPI_PLATFORM_PROTOCOL     *AcpiPlatformPtr = NULL;
  ACPI_MEMORY_AFFINITY_DATA  *AcpiMemAffDataPtr = NULL;
  UINTN                       AcpiMemAffSize = 0;
  UINT8                       Skt;
  UINT8                       Sad;
  UINT64                      TempOemTableId;

  if (NfitPtr == NULL || NfitMaxSize < MAX_NFIT_SIZE) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Locate ACPI Platform Protocol to get memory affinity data. If it cannot be located or
  // memory affinity data is not available we can ignore it, the proximity domain in
  // SPA Range Strucuture will be not set and appropriate flag will signal it.
  //
  Status = gBS->LocateProtocol (&gAcpiPlatformProtocolGuid, NULL, (VOID**)&AcpiPlatformPtr);
  if (EFI_ERROR (Status)) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("NFIT") "Cannot locate ACPI Platform protocol (%r)\n", Status));

  } else {

    Status = AcpiPlatformPtr->GetAcpiMemoryAffinityData (&AcpiMemAffDataPtr, &AcpiMemAffSize);
    if (EFI_ERROR (Status)) {

      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("NFIT") "Cannot get memory affinity data from ACPI Platform protocol (%r)\n", Status));

    } else {

      NfitSaveAcpiMemoryAffinityData (AcpiMemAffDataPtr, AcpiMemAffSize);
    }
  }
  if (EFI_ERROR (Status)) {

    CRDEBUG ((DEBUG_ERROR, CR_WARN_STR ("NFIT") "Memory affinity data not available, cannot assign proximity domain to SPA regions\n"));
  }
  //
  // Locate SMBIOS protocol to retrieve SMBIOS handles for NVDIMMs.
  //
  Status = gBS->LocateProtocol (&gSmbiosMemInfoProtocolGuid, NULL, (VOID**)&mSmbiosInfoProtocol);
  if (EFI_ERROR (Status)) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("NFIT") "Cannot locate SmbiosMemInfoProtocol! (%r)\n", Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  //
  // Start building the NFIT structures based on the current memory configuration, and
  // publish it as part of the ACPI tables. NFIT is required ONLY when NVDIMMs are installed.
  //
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "Building NFIT table at 0x%p (buffer len: %d)\n", NfitPtr, NfitMaxSize));
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "NfitPublishMailboxStructsDisable: %d\n",    mSystemMemoryMap->MemSetup.NfitPublishMailboxStructsDisable));
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "DfxNfitDebugLogsEnable:              %d\n", mSystemMemoryMap->MemSetup.dfxMemSetup.DfxNfitDebugLogsEnable));
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "DfxNfitNvdimmSkuBasedDisable:        %d\n", mSystemMemoryMap->MemSetup.dfxMemSetup.DfxNfitNvdimmSkuBasedDisable));
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "DfxNfitMailboxWithPropGuid:          %d\n", mSystemMemoryMap->MemSetup.dfxMemSetup.DfxNfitMailboxWithPropGuid));
  //
  // Initialize SAD info, particularly number of SADs participating in NFIT.
  //
  mSAD_Info.NumOfSADs = 0;
  ZeroMem (&mSAD_Info, sizeof(mSAD_Info));
  //
  // Initialize globals used for Unique ID numbers.
  //
  mSpaRangeIndex = 1;         // Identifies SPA Range Structure, zero is reserved, starts at 1
  mNvdimmRegionMappingId = 1; // Identifies NVDIMM Region Mapping Structure, zero allowed but we start at 1
  mNvdimmCtrlRegionIndex = 1; // Identifies NVDIMM Ctrl Region Structure, zero allowed but we start at 1
  mInterleaveDescIndex = 1;   // Identifies Interleave Structure, zero is reserved, starts at 1

  ZeroMem (NfitPtr, NfitMaxSize);
  //
  // Create NFIT header
  //
  TempOemTableId = PcdGet64 (PcdAcpiDefaultOemTableId);
  NfitPtr->Signature = NVDIMM_FW_INTERFACE_TABLE_SIGNATURE;
  NfitPtr->Length = 0;
  NfitPtr->Revision = NVDIMM_FW_INTERFACE_TABLE_REVISION;
  CopyMem (NfitPtr->OemID, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (NfitPtr->OemID));
  CopyMem (NfitPtr->OemTblID, &TempOemTableId, sizeof (NfitPtr->OemTblID));
  NfitPtr->OemRevision = EFI_ACPI_OEM_REVISION;
  NfitPtr->CreatorID = EFI_ACPI_CREATOR_ID;
  NfitPtr->CreatorRev = EFI_ACPI_CREATOR_REVISION;
  //
  // Although header was created in NFIT buffer the NfitPtr->Length remains zero because it is used
  // to index NfitPtr->NfitTables[] in functions that build structures. But let's adjust max size
  // so that we can control if we do not override the buffer. The header size will be added to
  // NfitPtr->Length after all tables are built.
  //
  mNfitMaxSize = (UINT32)NfitMaxSize - sizeof (NFIT_HEADER);

  if (mSystemMemoryMap->MemSetup.dfxMemSetup.DfxNfitNvdimmSkuBasedDisable) {

    NfitGenerateCtrlRegionsListSadBased (NfitPtr);

  } else {

    NfitGenerateCtrlRegionsListSkuBased (NfitPtr);
  }
  if (!mSystemMemoryMap->MemSetup.NfitPublishMailboxStructsDisable) {
    //
    // Create NFIT structures for Control Region Block Decoder if it exists.
    //
    NfitCreateSubTblsForCrbd (NfitPtr);
  }

  //
  // Create NFIT sub-tables
  //
  for (Skt = 0; Skt < MAX_SOCKET; Skt++) {
    //
    // Skip disabled sockets
    //
    if (!mSystemMemoryMap->Socket[Skt].SocketEnabled) {
      continue;
    }
    for (Sad = 0; Sad < MAX_SAD_RULES; Sad++) {

      if  (NfitCheckIfCreateSubTbls (Skt, Sad)) {

        NfitCreateSubTbls (NfitPtr, Skt, Sad);
      }
    }
  }
  //
  // Append to NFIT structures for Disabled NVDIMMs
  //
  NfitCreateSubTblsForNotMappedDimms (NfitPtr);
  //
  // Add Flush Hint Address to Nfit
  //
  NfitBuildFlushHintAddrStructure (NfitPtr);
  //
  // Do not build SMBIOS Management Information Structure because we don't support hot add.
  // Build Platform Capabilities Structure.
  //
  NfitBuildPlatformCapabilitiesStructure (NfitPtr);

  NfitPtr->Length += sizeof (NFIT_HEADER);
  //
  // Update the NFIT checksum.
  //
  NfitUpdateTableChecksum (NfitPtr);
  //
  // Contact CLV before changing the trace below, test depends on it.
  //
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("NFIT") "After adding all tables, NFIT Length = 0x%X\n", NfitPtr->Length));

  return Status;
} // NfitBuildTable()

