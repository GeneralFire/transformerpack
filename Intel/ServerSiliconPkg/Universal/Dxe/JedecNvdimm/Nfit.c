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

#include "Nfit.h"
#include <Library/ProcMemInitChipLib.h>
#include <Include/NvdimmCommon.h>

#ifndef __GNUC__
#pragma optimize("", off)
#endif  //__GNUC__

extern struct SystemMemoryMapHob *mSystemMemoryMap;
extern NVDIMM_COMMON_INFO               NvdimmInfo;
extern SMBIOS_HANDLE_INFO        SMBiosHandleInfo[MAX_SOCKET * MAX_CH * MAX_DIMM];

extern UINT8 GetChInterBitmap(struct SADTable *pSAD);

extern UINT32 Imc1Way4k_3ChWay256_LineMultipliers[3][16];
extern UINT32 Imc2Way4k_3ChWay256_LineMultipliers[6][16];
extern UINT32 Imc4Way4k_3ChWay256_LineMultipliers[12][16];
extern UINT32 Imc8Way4k_3ChWay256_LineMultipliers[24][16];
extern UINT32 Imc1Way4k_3ChWay4k_LineMultipliers[3][2];
extern UINT32 Imc2Way4k_3ChWay4k_LineMultipliers[6][2];
extern UINT32 Imc4Way4k_3ChWay4k_LineMultipliers[12][2];
extern UINT32 Imc8Way4k_3ChWay4k_LineMultipliers[24][2];

UINT32  RgnOffsetMultiplier[48] = { // MAX_SOCKET * MAX_MC_CH * MAX_IMC (for 8 sockets)
                                   0, 1, 2, 18, 16, 17,
                                   33, 34, 32, 48, 49, 50,
                                   66, 64, 65, 81, 82, 80,
                                   96, 97, 98, 114, 112, 113,
                                   129, 130, 128, 144, 145, 146,
                                   162, 160, 161, 177, 178, 176,
                                   192, 193, 194, 210, 208, 209,
                                   225, 226, 224, 240, 241, 242
                                  };

UINT32                          mNfitSize;
UINT16                          mSPARangeDescTblIndex;
UINT16                          mMemDevRegionalID;
UINT16                          mNumInterleaveDescTbls;
UINT16                          mNumSMBiosHandles;
UINT8                           mCtrlRegionIndex = 0;

//
// This struct is used by processing code to prevent redundant SADs
// getting entered into Nfit.
struct _SAD_INFO_ {
  UINT8       NumOfSADs;
  UINT32      Limits[MAX_SOCKET * MAX_SADS];
} mSAD_Info;

//
// Struct is used to store the list of control regions for all SADs.
struct _CTRL_INFO_ {
  UINT8       DimmIndex;
  UINT16      Ifc;
  BOOLEAN     TableAdded;
} mCtrlRegions[MAX_SOCKET * MAX_CH * MAX_CTRL_RGNS_PER_CHANNEL];

UINT16 mCtrlRegionsLen;

VOID
DisplayInterleaveDescriptionTable (
  INTERLEAVE_DESC_TABLE *pIntDescTable,
  UINT32    *LineOffset
  )
{
   UINT8 i;

   DEBUG((EFI_D_ERROR, "Interleave Structure\n"));
   DEBUG((EFI_D_ERROR, "\tType..............0x%02X\n", pIntDescTable->Type ));
   DEBUG((EFI_D_ERROR, "\tLength............0x%02X\n", pIntDescTable->Length ));
   DEBUG((EFI_D_ERROR, "\tIndex.............0x%02X\n", pIntDescTable->InterleaveStructureIndex));
   DEBUG((EFI_D_ERROR, "\tNumber Lines......0x%04X\n", pIntDescTable->NumberOfLines));
   DEBUG((EFI_D_ERROR, "\tLine Size.........0x%04X\n", pIntDescTable->LineSize ));
   for(i = 0; i < pIntDescTable->NumberOfLines; i++) {
     DEBUG((EFI_D_ERROR, "\tLine Offset[%d]...0x%04X\n", i, LineOffset[i]));
   }
}

VOID
DisplayMemDevicetoSPATable(
  NVDIMM_REGION_MAPPING_TABLE *pMDTSPARMT
)
{
   DEBUG((EFI_D_ERROR, "Memory Device to SPA Table\n"));
   DEBUG((EFI_D_ERROR, "\tType.......................0x%02X\n", pMDTSPARMT->Type ));
   DEBUG((EFI_D_ERROR, "\tLength.....................0x%02X\n", pMDTSPARMT->Length ));
   DEBUG((EFI_D_ERROR, "\tDevice Handle..............0x%04X\n", pMDTSPARMT->NFITDeviceHandle ));
   DEBUG((EFI_D_ERROR, "\tPhysical ID................0x%02X\n", pMDTSPARMT->NVDIMMPhysicalID ));
   DEBUG((EFI_D_ERROR, "\tRegion ID..................0x%02X\n", pMDTSPARMT->NVDIMMRegionID ));
   DEBUG((EFI_D_ERROR, "\tSPA Index..................0x%02X\n", pMDTSPARMT->SPARangeStructureIndex ));
   DEBUG((EFI_D_ERROR, "\tNVDIMM Ctrl Region Index...0x%02X\n", pMDTSPARMT->NVDIMMControlRegionStructureIndex ));
   DEBUG((EFI_D_ERROR, "\tRegion Size................0x%08llX\n", pMDTSPARMT->NVDIMMRegionSize ));
   DEBUG((EFI_D_ERROR, "\tRegion Offset..............0x%08llX\n", pMDTSPARMT->RegionOffset ));
   DEBUG((EFI_D_ERROR, "\tPhysical Address Base......0x%08llX\n", pMDTSPARMT->NVDIMMPhysicalAddressRegionBase ));
   DEBUG((EFI_D_ERROR, "\tInterleave Structure Index.0x%02X\n", pMDTSPARMT->InterleaveStructureIndex ));
   DEBUG((EFI_D_ERROR, "\tInterleave Ways............0x%02X\n", pMDTSPARMT->InterleaveWays ));
   DEBUG((EFI_D_ERROR, "\tFlags......................0x%02X\n", pMDTSPARMT->NVDIMMStateFlags ));
}

/**

This routine computes the checksum of a table of given size.

@param *TblStart    - Start of the table for which checksum needs to be computed
@param BytesCount   - Total size of the table for which checksum needs to be computed

@return UINT8       - Checksum computed

**/
UINT8
ChsumTbl (
  UINT8  *TblStart,
  UINT32 BytesCount
  )
{
  UINTN i;
  UINT8 res;

  if (TblStart == NULL || BytesCount == 0)
    return 0;

  res = *TblStart;
  for (i = 1; i < BytesCount; i++)
    res = res + TblStart[i];
  res = 0 - res;

  return (res);
} // ChsumTbl

/**

Routine Description:
This routine builds Interleave Description Tables for a 1 Way Channel Interleave.

  @param pNfit     -  pointer to Nvdimm f/w interface table
  @param SktGran   -  socket granularity
  @param ChGran    -  channel granularity
  @param ImcWays   -  Imc Ways

  @return VOID

**/
VOID
Build1ChWayIntDescTbls (
  NVDIMM_FW_INTERFACE_TABLE       *pNfit,
  UINT32                          SktGran,
  UINT32                          ChGran,
  UINT8                           ImcWays
  )
{
  INTERLEAVE_DESC_TABLE           *pIntDescTbl;
  UINT32                          LinesDesc, LineSize;
  UINT8                           i, j;
  UINT32                          *LineOffset;

  if (pNfit == NULL) {
    return;
  }

  //
  // No interleaving in this case
  if (ImcWays == ONE_IMC_WAY) {
    return;
  }

  ASSERT(SktGran >= ChGran);
  LinesDesc = SktGran / ChGran;
  LineSize = ChGran;

  for (i = 0; i < ImcWays; i++) {
    pIntDescTbl = (INTERLEAVE_DESC_TABLE *)(&pNfit->NfitTables[0] + pNfit->Length);
    pIntDescTbl->Type = TYPE_INTERLEAVE_DESCRIPTOR;
    pIntDescTbl->Length = (UINT16)(sizeof(*pIntDescTbl) + LinesDesc * sizeof(*LineOffset));
    // Index must be non-zero, i.e., 1 based.
    pIntDescTbl->InterleaveStructureIndex = mNumInterleaveDescTbls + 1;
    pIntDescTbl->NumberOfLines = LinesDesc;
    pIntDescTbl->LineSize = LineSize;

    LineOffset = (UINT32 *)((UINT8 *)pIntDescTbl + sizeof(*pIntDescTbl));
    for (j = 0; j < LinesDesc; j++) {
      LineOffset[j] = j;
    }
    DisplayInterleaveDescriptionTable(pIntDescTbl, LineOffset);

    mNumInterleaveDescTbls++;

    //
    // Update the Nfit lenth as appropriate
    ASSERT((pNfit->Length + pIntDescTbl->Length) <= mNfitSize);
    pNfit->Length += pIntDescTbl->Length;
  }
}

/**

Routine Description:
This routine builds Interleave Description Tables for a 2 Way Channel Interleave.

  @param pNfit     -  pointer to Nvdimm f/w interface table
  @param SktGran   -  socket granularity
  @param ChGran    -  channel granularity
  @param ImcWays   -  Imc Ways

  @return VOID

**/
VOID
Build2ChWayIntDescTbls (
  NVDIMM_FW_INTERFACE_TABLE       *pNfit,
  UINT32                          SktGran,
  UINT32                          ChGran,
  UINT8                           ImcWays
  )
{
  INTERLEAVE_DESC_TABLE           *pIntDescTbl;
  UINT32                          LinesDesc, LineSize;
  UINT8                           i, j;
  UINT32                          *LineOffset;

  if (pNfit == NULL) {
    return;
  }

  ASSERT(SktGran >= ChGran);
  LineSize = ChGran;

  // Compute the number of lines required for each channel and address range associated with each MC before the pattern repeats
  if((ImcWays == 1) || (SktGran == ChGran)) {
    LinesDesc = 2;
  } else {
    LinesDesc = SktGran / (ChGran * 2);
  }

  // Create one table for each MC and CH involved in this interleave
  for (i = 0; i < (ImcWays * 2); i++) {
    pIntDescTbl = (INTERLEAVE_DESC_TABLE *)(&pNfit->NfitTables[0] + pNfit->Length);
    pIntDescTbl->Type = TYPE_INTERLEAVE_DESCRIPTOR;
    pIntDescTbl->Length = (UINT16)(sizeof(*pIntDescTbl) + LinesDesc * sizeof(*LineOffset));
    // Index must be non-zero, i.e., 1 based.
    pIntDescTbl->InterleaveStructureIndex = mNumInterleaveDescTbls + 1;
    pIntDescTbl->NumberOfLines = LinesDesc;
    pIntDescTbl->LineSize = LineSize;

    LineOffset = (UINT32 *)((UINT8 *)pIntDescTbl + sizeof(*pIntDescTbl));
    for( j = 0 ; j < LinesDesc ; j++ )  {
      if(SktGran != ChGran) {
        // When SktGran is not equal to ChGran, the computed LineOffsets do not cross Imcs
        LineOffset[j] = (j * 2);
      } else {
        // When SktGran is equal to ChGran, the computed LineOffsets cross Imcs
        LineOffset[j] = (j * 2 * ImcWays);
      }
    }
    DisplayInterleaveDescriptionTable(pIntDescTbl, LineOffset);

    mNumInterleaveDescTbls++;

    //
    // Update the Nfit lenth as appropriate
    ASSERT((pNfit->Length + pIntDescTbl->Length) <= mNfitSize);
    pNfit->Length += pIntDescTbl->Length;
  }
}

/**

Routine Description:
This routine builds Interleave Description Tables for a 3 Way Channel Interleave.

  @param pNfit     -  pointer to Nvdimm f/w interface table
  @param SktGran   -  socket granularity
  @param ChGran    -  channel granularity
  @param ImcWays   -  Imc Ways

  @return VOID

**/
VOID
Build3ChWayIntDescTbls (
  NVDIMM_FW_INTERFACE_TABLE       *pNfit,
  UINT32                          SktGran,
  UINT32                          ChGran,
  UINT8                           ImcWays
  )
{
  INTERLEAVE_DESC_TABLE   *pIntDescTbl;
  UINT32                  LineSize;
  UINT32                  LinesDesc;
  UINT8                   imc, ch, line;
  UINT32                  IntDescTblMinValue;
  UINT32                  *InterleaveMultipliers = NULL;
  UINT32                  *LineOffset;
  UINT16                  InterleaveArrayIndex;

  if (pNfit == NULL) {
    return;
  }

  ASSERT(SktGran >= ChGran);

  //
  // Set Line Size and Lines Described
  LineSize = ChGran;

  if (SktGran == ChGran) {
    LinesDesc = 2;
  } else {
    LinesDesc = SktGran / ChGran;
  }

  for (imc = 0; imc < ImcWays; imc++) {
    for (ch = 0; ch < MAX_MC_CH; ch++) {
      //
      // Initialize the table headers
      pIntDescTbl = (INTERLEAVE_DESC_TABLE *)(&pNfit->NfitTables[0] + pNfit->Length);
      pIntDescTbl->Type = TYPE_INTERLEAVE_DESCRIPTOR;
      // Index must be non-zero, i.e., 1 based.
      pIntDescTbl->InterleaveStructureIndex = mNumInterleaveDescTbls + 1;
      pIntDescTbl->Length = (UINT16)(sizeof(*pIntDescTbl) + LinesDesc * sizeof(*LineOffset));
      pIntDescTbl->NumberOfLines = LinesDesc;
      pIntDescTbl->LineSize = LineSize;

      //
      // Point to the correct multiplier arrays
      InterleaveArrayIndex = ((imc * MAX_MC_CH) + ch) % (ImcWays * CH_3WAY);
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
          ASSERT(ImcWays < EIGHT_IMC_WAY);
          return;
          break;
      }

      LineOffset = (UINT32 *)((UINT8 *)pIntDescTbl + sizeof(*pIntDescTbl));

      IntDescTblMinValue = 0xffffffff;
      //
      // Copy the known values from the table
      for (line = 0;  line < LinesDesc; line++) {
        LineOffset[line] = InterleaveMultipliers[line];
        if (LineOffset[line] < IntDescTblMinValue) {
          IntDescTblMinValue = LineOffset[line];
        }
      }

      //
      // Convert the table values to multiplier indices
      for (line = 0;  line < LinesDesc; line++) {
        LineOffset[line] -= IntDescTblMinValue;
      }

      //
      // Display Interleave tables
      DisplayInterleaveDescriptionTable(pIntDescTbl, LineOffset);

      mNumInterleaveDescTbls++;

      //
      // Update the Nfit lenth as appropriate
      ASSERT((pNfit->Length + pIntDescTbl->Length) <= mNfitSize);
      pNfit->Length += pIntDescTbl->Length;
    }
  }
}

/**

Routine Description:GetSMBiosHandle finds from the previously
stored SMBIOS Type 17 Handles for all DCPMM Dimms, the SMBios Type
17 Handle for this particular NVMDIMM in this socket and
Channel.

  @param socket         - Socket Number
  @param ch             - DDR Channel ID
  @param dimm           - DDR Dimm ID
  @retval SMBiosHandle  - SMBios Type17 Handle for a given
                          socket and channel

**/
UINT16
GetSMBiosHandle (
  UINT8                          Socket,
  UINT8                          Channel,
  UINT8                          Dimm
  )
{
  UINT8                           i;

  for (i = 0; i < mNumSMBiosHandles; i++) {
    if ((SMBiosHandleInfo[i].Socket == Socket) &&
        (SMBiosHandleInfo[i].Channel == Channel) &&
        (SMBiosHandleInfo[i].Dimm == Dimm)) {
      return (SMBiosHandleInfo[i].SMBiosType17Handle);
    }
  }
  return 0;
}

/*++

Routine Description:

This routine basically builds Memory Device To SPA Range Map Table as it adds it to
the Nfit

Arguments:

pNfit         - Pointer to NFIT
socket        - socket
Imc           - Imc Index (0 for Imc0 & 1 for Imc1)
Ch            - Channel
pSPARDT       - Pointer to SPA Range Desc Table
IntIndex      - Interleave table Index associated with this Mem Dev to SPA Range Mapping table
RegionOffset  - Region offset
Ways          - Interleave ways
SadInx        - Sad Index for this SPA region
Nvdimm        - Pointer to Nvdimm structure

Returns:

VOID

--*/
VOID
BuildMemDevToSPARangeMapTbls (
  NVDIMM_FW_INTERFACE_TABLE       *pNfit,
  UINT8                           Socket,
  UINT8                           Imc,
  UINT8                           Ch,
  SPA_RANGE_DESC_TABLE            *pSPARDT,
  UINT16                          IntIndex,
  UINT64                          RegionOffset,
  UINT16                          Ways,
  UINT8                           SadInx,
  JEDEC_NVDIMM                          *Nvdimm
  )
{
  NVDIMM_REGION_MAPPING_TABLE     *pMDTSPARMT;
  struct SADTable                 *pSAD;
  UINT8                           Dimm;
  UINT64                          PMemDpa;
  UINT8                           Buffer8;
  EFI_STATUS                      Status = EFI_SUCCESS;

  DEBUG((EFI_D_ERROR, "Entering BldMemDevToSPARngMap!\n"));

  Dimm = Nvdimm->Dimm;

  if (Socket >= MAX_SOCKET ||
      Imc >= MAX_IMC ||
      Ch >= MAX_CH ||
      SadInx >= MAX_SAD_RULES ||
      Ways == 0)
  {
     return;
  }
  if (pSPARDT == NULL || pNfit == NULL) {
    return;
  }

  pSAD = &mSystemMemoryMap->Socket[Socket].SAD[SadInx];
  ASSERT(pSAD != NULL);
  if (pSAD == NULL) {
    return;
  }

  // Get the pointer to the next Nfit table, which in this case is MDTSPARMT.
  pMDTSPARMT = (NVDIMM_REGION_MAPPING_TABLE*)(UINTN)(&pNfit->NfitTables[0] + pNfit->Length);
  // Set the Type for MDTSPARMT, which is 1.
  pMDTSPARMT->Type = TYPE_NVDIMM_REGION_MAPPING_DESCRIPTOR;
  pMDTSPARMT->Length = sizeof(NVDIMM_REGION_MAPPING_TABLE);
  *(UINT32*)&pMDTSPARMT->NFITDeviceHandle = NFIT_HANDLE_BUILD (Socket, MEM_SKTCH_TO_IMC (Ch), MEM_SKTCH_TO_IMCCH (Ch), Dimm);
  DEBUG((EFI_D_ERROR, "Nfit: NfitDevHandle = 0x%X\n", *((UINT32*)&pMDTSPARMT->NFITDeviceHandle)));
  // SMBIOS Type 17 handle
  pMDTSPARMT->NVDIMMPhysicalID = GetSMBiosHandle(Socket, Ch, Dimm);
  DEBUG((EFI_D_ERROR, "Nfit: SMBIOSHandle = 0x%X\n", pMDTSPARMT->NVDIMMPhysicalID));
  pMDTSPARMT->NVDIMMRegionID = mMemDevRegionalID;
  mMemDevRegionalID++;
  pMDTSPARMT->SPARangeStructureIndex = pSPARDT->SPARangeStructureIndex;
  pMDTSPARMT->NVDIMMControlRegionStructureIndex = Nvdimm->CntrlRegionIndex;

  DEBUG((EFI_D_ERROR, "Nfit: CntrlRgnInx = 0x%X\n", pMDTSPARMT->NVDIMMControlRegionStructureIndex));
  DEBUG((EFI_D_ERROR, "Nfit: RegionOffset = 0x%llx\n", RegionOffset));

  pMDTSPARMT->NVDIMMRegionSize = (pSPARDT->SystemPhysicalAddressRangeLength / Ways);

  // Since this is in a Hob, needs to copied to a variable before it is shifted to convert it to bytes
  PMemDpa = (UINT64) mSystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].perRegionDPA;
  pMDTSPARMT->NVDIMMPhysicalAddressRegionBase = (PMemDpa << BITS_64MB_TO_BYTES);
  pMDTSPARMT->RegionOffset = RegionOffset;
  //pMDTSPARMT->InterleaveStructureIndex = IntIndex;
  pMDTSPARMT->InterleaveStructureIndex = 0;   // Set to zero since only block region requires interleave structures
  pMDTSPARMT->InterleaveWays = Ways;
  // This flag has the Bitmaps to indicate certain failure and SMART events.
  pMDTSPARMT->NVDIMMStateFlags = BIT5;

  // Switch for page 0 register map
  Status = JedecSwitchPage(Nvdimm, 0);
  ASSERT_EFI_ERROR (Status);

  Status = ReadSmb(Nvdimm, SAVE_STATUS, &Buffer8);
  ASSERT_EFI_ERROR (Status);
  DEBUG((EFI_D_ERROR, "SAVE_STATUS = 0x%x\n", Buffer8));
  if (Buffer8 & BIT1) {
    pMDTSPARMT->NVDIMMStateFlags |= BIT0;
  }

  Status = ReadSmb(Nvdimm, RESTORE_STATUS, &Buffer8);
  ASSERT_EFI_ERROR (Status);
  DEBUG((EFI_D_ERROR, "RESTORE_STATUS = 0x%x\n", Buffer8));
  if (Buffer8 & BIT1) {
    pMDTSPARMT->NVDIMMStateFlags |= BIT1;
  }

  Status = ReadSmb(Nvdimm, ARM_STATUS, &Buffer8);
  ASSERT_EFI_ERROR (Status);
  DEBUG((EFI_D_ERROR, "ARM_STATUS = 0x%x\n", Buffer8));
  if (Buffer8 & BIT1) {
    pMDTSPARMT->NVDIMMStateFlags |= BIT3;
  }

  DisplayMemDevicetoSPATable(pMDTSPARMT);

  // Update the Nfit length as appropriate
  // DEBUG((EFI_D_ERROR, "Nfit Length = 0x%X\n", pNfit->Length));
  ASSERT((pNfit->Length + pMDTSPARMT->Length) <= mNfitSize);
  pNfit->Length += pMDTSPARMT->Length;
}

/**

Routine Description: AddACntrlRgnToNfit creates One Control
Region Descriptor Table and adds it to Nfit.

  @param pNfit       - Pointer to Nfit Struct
  @param Socket      - Socket Number
  @param Ch          - Channel Mumber
  @retval VOID       - None

**/
VOID
AddACtrlRgnToNfit (
  NVDIMM_FW_INTERFACE_TABLE     *pNfit,
  JEDEC_NVDIMM                        *Nvdimm
  )
{
  CTRL_REGION_TABLE             *pCntrlRgnTbl;
  NVDIMM                        *pDimm = NULL;

  DEBUG((EFI_D_ERROR, "Entering AddCtrlRgnToNfit!\n"));

  if (pNfit == NULL) {
    DEBUG((EFI_D_ERROR, "Nfit: Parameters error! AddCtrlRgnToNfit interrupted!\n"));
    ASSERT(pNfit != NULL);
    return;
  }

  pDimm = GetDimm (&NvdimmInfo, Nvdimm->SocketId, MEM_IMCCH_TO_SKTCH(Nvdimm->ImcId, Nvdimm->Ch), Nvdimm->Dimm, NVDIMM_FLAG_ALL);

  if (pDimm == NULL) {
    DEBUG((EFI_D_ERROR, "Nfit: dimm information structure not found! AddCtrlRgnToNfit interrupted!\n"));
    ASSERT (pDimm != NULL);
    return;
  }

  DEBUG((EFI_D_ERROR, "Nfit: Adding CntrlRgn for a NvmDimm on Socket = %d, Channel = %d, Dimm = %d\n", Nvdimm->SocketId, MEM_IMCCH_TO_SKTCH(Nvdimm->ImcId, Nvdimm->Ch), Nvdimm->Dimm));

  // Get the pointer of Control Region Table to point to the right area of the Nfit.
  pCntrlRgnTbl = (CTRL_REGION_TABLE *)(UINTN)(&pNfit->NfitTables[0] + pNfit->Length);

  pCntrlRgnTbl->Type                              = TYPE_CNTRL_REGION_DESCRIPTOR;
  pCntrlRgnTbl->Length                            = sizeof(CTRL_REGION_TABLE);
  pCntrlRgnTbl->NVDIMMControlRegionStructureIndex = Nvdimm->CntrlRegionIndex;
  pCntrlRgnTbl->VendorID                          = pDimm->VendorID;
  pCntrlRgnTbl->DeviceID                          = pDimm->DeviceID;
  pCntrlRgnTbl->RevisionID                        = pDimm->RevisionID;
  pCntrlRgnTbl->SubsystemVendorID                 = pDimm->SubsystemVendorID;
  pCntrlRgnTbl->SubsystemDeviceID                 = pDimm->SubsystemDeviceID;
  pCntrlRgnTbl->SubsystemRevisionID               = pDimm->SubsystemRevisionID;
  pCntrlRgnTbl->ValidFields                       = MANUFACTURING_VALID_FIELDS;
  pCntrlRgnTbl->ManufacturingLocation             = pDimm->ManufLocation;
  pCntrlRgnTbl->ManufacturingDate                 = pDimm->ManufDate;
  if (pDimm->SerialNum == 0) {
    pCntrlRgnTbl->SerialNumber                    = Nvdimm->CntrlRegionIndex;  // Make sure unique serial number (NVDR yellow bang if not unique)
  }
  else {
    pCntrlRgnTbl->SerialNumber                    = pDimm->SerialNum;
  }
  pCntrlRgnTbl->RegionFormatInterfaceCode         = 0x0101; // TODO: change to pDimm->FormatIntefaceCode when it stores correct value

  DEBUG((EFI_D_ERROR, "Nfit: CntrlRgnIndex = 0x%X\n", pCntrlRgnTbl->NVDIMMControlRegionStructureIndex));
  DEBUG((EFI_D_ERROR, "Nfit: CntrlRgn VendorID = 0x%X\n", pDimm->VendorID));
  DEBUG((EFI_D_ERROR, "Nfit: CntrlRgn DeviceID = 0x%X\n", pDimm->DeviceID));
  DEBUG((EFI_D_ERROR, "Nfit: CntrlRgn RevisionID = 0x%X\n", pDimm->RevisionID));
  DEBUG((EFI_D_ERROR, "Nfit: CntrlRgn SubsystemVendorID = 0x%X\n", pDimm->SubsystemVendorID));
  DEBUG((EFI_D_ERROR, "Nfit: CntrlRgn SubsystemDeviceID = 0x%X\n", pDimm->SubsystemDeviceID));
  DEBUG((EFI_D_ERROR, "Nfit: CntrlRgn SubsystemRevisionID = 0x%X\n", pDimm->SubsystemRevisionID));
  DEBUG((EFI_D_ERROR, "Nfit: CntrlRgn ManufLocation = 0x%X\n", pDimm->ManufLocation));
  DEBUG((EFI_D_ERROR, "Nfit: CntrlRgn ManufDate = 0x%X\n", pDimm->ManufDate));
  DEBUG((EFI_D_ERROR, "Nfit: CntrlRgn SerialNumber = 0x%X\n", pCntrlRgnTbl->SerialNumber));
  DEBUG((EFI_D_ERROR, "Nfit: CntrlRgn RegionFormatInterfaceCode = 0x%X\n", pCntrlRgnTbl->RegionFormatInterfaceCode));

  pCntrlRgnTbl->NumberOfBlockControlWindows = 0x0;
  pCntrlRgnTbl->SizeOfBlockControlWindow = 0x0;
  pCntrlRgnTbl->CommandRegisterOffsetInBlockControlWindow = 0x0;
  pCntrlRgnTbl->SizeOfCommandRegisterInBlockControlWindows = 0;
  pCntrlRgnTbl->StatusRegisterOffsetInBlockControlWindow = 0;
  pCntrlRgnTbl->SizeOfStatusRegisterInBlockControlWindows = 0;
  pCntrlRgnTbl->NVDIMMControlRegionFlag = 0;

  ASSERT((pNfit->Length + pCntrlRgnTbl->Length) <= mNfitSize);
  pNfit->Length += pCntrlRgnTbl->Length;
}

/**
Routine Description:

This routine basically builds NvDIMM F/W Interface Table for 1 Way Channel Interleave, appending the sub-tables to Nfit
from the SAD struct passed in.

Arguments:

   @param pNfit           -  pointer to Nvdimm f/w interface table
   @param socket          - socket number
   @param Ch              - Interleave Channel number
   @param ImcWays         - Number of Imc ways this interleave should
   @param happen. SadInx  - SAD Index that needs to be added to
   @param Nfit pSPARDT    - Pointer to Range Desc Table
   @param SADSpaBase      - SPA Base Address
   @param PmemRegion      - PMEM region


Returns:

   @retval VOID
**/
VOID
BuildMemDevFor1WayChInt (
  NVDIMM_FW_INTERFACE_TABLE     *pNfit,
  UINT8                         ImcWays,
  SPA_RANGE_DESC_TABLE          *pSPARDT,
  UINT64                        SADSpaBase,
  PMEM_REGION                   *PmemRegion
  )
{
  struct SADTable                 *pSAD;
  UINT16                          NumIntTbls;
  UINT8                           Ch = 0;
  UINT64                          RegionOffset;
  UINT32                          SktGran;
  UINT8                           ChInterBitmap;
  UINT8                           ImcIndex = 0;
  UINT8                           NvDimmIndex = 0;

  if (pNfit == NULL || pSPARDT == NULL || ImcWays == 0) {
    return;
  }

  NumIntTbls = ImcWays == ONE_IMC_WAY ? 0 : mNumInterleaveDescTbls + 1;
  // Region Offset starts as 0 for the 1st Dimm, then increments by Target Granularity.
  RegionOffset = 0;
  pSAD = &mSystemMemoryMap->Socket[PmemRegion->SocketNum].SAD[PmemRegion->SadIndex];
  ChInterBitmap = GetChInterBitmap(pSAD);
  SktGran = GetSocketGranularity(pSAD);

  // Imc0 is participating in 1 Channel Way Interleave set
  // Knowing it is 1 way interleaved, get the channel
  if ((ChInterBitmap & 0x01) == 0x01) {
    Ch = 0;
  } else if ((ChInterBitmap & 0x02) == 0x02) {
    Ch = 1;
  } else if ((ChInterBitmap & 0x04) == 0x04) {
    Ch = 2;
  }

  if ((PmemRegion->SocketNum * MAX_IMC) >= MC_MAX_NODE) {
    DEBUG((EFI_D_ERROR, "Nfit: BuildMemDevFor1WayChInt (PmemRegion->SocketNum * MAX_IMC) >= MC_MAX_NODE \n"));
    return ;
  }

  for (ImcIndex = 0; ImcIndex < MAX_IMC; ImcIndex++) {
    if (mSystemMemoryMap->Socket[PmemRegion->SocketNum].SadIntList[PmemRegion->SadIndex][(PmemRegion->SocketNum * MAX_IMC) + ImcIndex]) {
      ASSERT (NvDimmIndex < PmemRegion->NumNvDimms);
      BuildMemDevToSPARangeMapTbls(pNfit, PmemRegion->SocketNum, ImcIndex, Ch, pSPARDT, NumIntTbls, RegionOffset, ImcWays, PmemRegion->SadIndex, &PmemRegion->NvDimms[NvDimmIndex]);
      NvDimmIndex++;
      NumIntTbls = ImcWays == ONE_IMC_WAY ? 0 : NumIntTbls + 1;
      RegionOffset += SktGran;
    }
    Ch += MAX_MC_CH;
  }
}

/**

Routine Description:

This routine basically builds NvDIMM F/W Interface Tables for 2 way Channel Interleaved, appending the sub-tables to Nfit
from the SAD struct passed in.

Arguments:

  @param pNfit   -  pointer to Nvdimm f/w interface table
  @param socket  - socket number
  @param ImcWays - Number of Imc ways this interleave should
  @param happen. SadInx  - SAD Index that needs to be added to
  @param Nfit pSPARDT - Pointer to Range Desc Table
  @param SADSpaBase - SPA Base Address
  @param PmemRegion - PMEM region


Returns:

  @retval VOID

**/
VOID
BuildMemDevFor2WayChInt (
  NVDIMM_FW_INTERFACE_TABLE     *pNfit,
  UINT8                         ImcWays,
  SPA_RANGE_DESC_TABLE          *pSPARDT,
  UINT64                        SADSpaBase,
  PMEM_REGION                   *PmemRegion
  )
{
  struct SADTable                 *pSAD;
  UINT64                          SpaBase, RegionOffset;
  UINT8                           Soc, i;
  UINT8                           Ch1 = 0;
  UINT8                           Ch2 = 1;
  UINT16                          NumIntTbls;
  UINT32                          SktGran, ChGran;
  UINT8                           ChInterBitmap;
  UINT8                           ImcIndex = 0;
  UINT8                           NvDimmIndex = 0;

  if (pNfit == NULL || pSPARDT == NULL || ImcWays == 0) {
    return;
  }

  // Region Offset starts with 0 for the 1st Dimm and increments by Channel Granularity for dimms in the same
  // Imc but increments by Target Granularity for the Dimms in other Imcs.
  RegionOffset = 0;
  for (Soc = 0; Soc < MAX_SOCKET; Soc++) {
    for (i = 0; i < MAX_SAD_RULES; i++) {
      pSAD = &mSystemMemoryMap->Socket[Soc].SAD[i];
      ASSERT(pSAD != NULL);
      if (pSAD == NULL) {
        break;
      }
      if (pSAD->Enable == 0) {
        continue;
      }

      ChInterBitmap = GetChInterBitmap(pSAD);
      SktGran = GetSocketGranularity(pSAD);
      ChGran = GetChannelGranularity(pSAD);
      if (i == 0) {
        SpaBase = 0;
      } else {
        SpaBase = ((UINT64)mSystemMemoryMap->Socket[Soc].SAD[i - 1].Limit << 26);
      }
      if ((SpaBase == SADSpaBase) && (mSystemMemoryMap->Socket[Soc].SAD[i].local)) {
        if ((ChInterBitmap & 0x03) == 0x03) {
          Ch1 = 0;
          Ch2 = 1;
        } else if ((ChInterBitmap & 0x05) == 0x05) {
          Ch1 = 0;
          Ch2 = 2;
        } else if ((ChInterBitmap & 0x06) == 0x06) {
          Ch1 = 1;
          Ch2 = 2;
        }
        NumIntTbls = mNumInterleaveDescTbls + 1;

        for (ImcIndex = 0; ImcIndex < MAX_IMC; ImcIndex++) {
          if (mSystemMemoryMap->Socket[Soc].SadIntList[i][(Soc * MAX_IMC) + ImcIndex]) {
            ASSERT ((NvDimmIndex + 1) < PmemRegion->NumNvDimms);
            BuildMemDevToSPARangeMapTbls(pNfit, Soc, ImcIndex, Ch1, pSPARDT, NumIntTbls, RegionOffset, (2 * ImcWays), i, &PmemRegion->NvDimms[NvDimmIndex]);
            NumIntTbls++;
            NvDimmIndex++;

            BuildMemDevToSPARangeMapTbls(pNfit, Soc, ImcIndex, Ch2, pSPARDT, NumIntTbls, (RegionOffset + ChGran), (2 * ImcWays), i, &PmemRegion->NvDimms[NvDimmIndex]);
            NumIntTbls++;
            NvDimmIndex++;

            if( SktGran != ChGran )  {
              // RegionOffset for next pair of channels is at the previous region offset plus memory controller interleave granularity
              RegionOffset += SktGran ;
            }
            else  {
              // RegionOffset for next pair of channels is at the previous region offset plus the channel granularity for the two channels already processed
              RegionOffset = RegionOffset + ( ChGran * 2 ) ;
            }
          }
          Ch1 += MAX_MC_CH;
          Ch2 += MAX_MC_CH;
        }
      }
    }
  }
}

/**
Routine Description:

This routine basically builds NvDIMM F/W Interface Tables for 3 way Channel Interleaved, appending the sub-tables to Nfit
from the SAD struct passed in.

Arguments:

  @param pNfit   -  pointer to Nvdimm f/w interface table
  @param socket  - socket number
  @param ImcWays - Number of Imc ways this interleave should
  @param happen. SadInx  - SAD Index that needs to be added to
  @param Nfit pSPARDT - Pointer to Range Desc Table
  @param SADSpaBase - SPA Base Address
  @param PmemRegion - PMEM region


Returns:

  @retval VOID

**/
VOID
BuildMemDevFor3WayChInt (
  NVDIMM_FW_INTERFACE_TABLE     *pNfit,
  UINT8                         ImcWays,
  SPA_RANGE_DESC_TABLE          *pSPARDT,
  UINT64                        SADSpaBase,
  PMEM_REGION                   *PmemRegion
  )
{
  UINT16                          NumIntTbls;
  UINT8                           Soc, i, j;
  struct                          SADTable *pSAD;
  UINT64                          SpaBase;
  UINT32                          ChGran;

  if (pNfit == NULL || pSPARDT == NULL || ImcWays == 0) {
    return;
  }

  // Region Offset starts with 0 for the 1st Dimm and increments by Channel Granularity for dimms in the same
  // Imc but increments by Target Granularity for the Dimms in other Imcs.
  j = 0;
  NumIntTbls = mNumInterleaveDescTbls + 1;
  for (Soc = 0; Soc < MAX_SOCKET; Soc++) {
    for (i = 0; i < MAX_SAD_RULES; i++) {
      pSAD = &mSystemMemoryMap->Socket[Soc].SAD[i];
      ASSERT(pSAD != NULL);
      if (pSAD == NULL) {
        break;
      }
      if (pSAD->Enable == 0) {
        continue;
      }

      ChGran = GetChannelGranularity(pSAD);
      if (i == 0) {
        SpaBase = 0;
      } else {
        SpaBase = ((UINT64)mSystemMemoryMap->Socket[Soc].SAD[i - 1].Limit << 26);
      }
      if ((SpaBase == SADSpaBase) && (mSystemMemoryMap->Socket[Soc].SAD[i].local)) {
        if (mSystemMemoryMap->Socket[Soc].SadIntList[i][Soc * MAX_IMC]) {
          // Imc0 is participating in the 3 channel way interleave
          BuildMemDevToSPARangeMapTbls(pNfit, Soc, 0, 0, pSPARDT, NumIntTbls, (RgnOffsetMultiplier[j] * ChGran), (3 * ImcWays), i, &PmemRegion->NvDimms[0]);
          NumIntTbls++;
          j++;
          BuildMemDevToSPARangeMapTbls(pNfit, Soc, 0, 1, pSPARDT, NumIntTbls, (RgnOffsetMultiplier[j] * ChGran), (3 * ImcWays), i, &PmemRegion->NvDimms[1]);
          NumIntTbls++;
          j++;
          BuildMemDevToSPARangeMapTbls(pNfit, Soc, 0, 2, pSPARDT, NumIntTbls, (RgnOffsetMultiplier[j] * ChGran), (3 * ImcWays), i, &PmemRegion->NvDimms[2]);
          NumIntTbls++;
          j++;
        }
        if (mSystemMemoryMap->Socket[Soc].SadIntList[i][(Soc * MAX_IMC) + 1]) {
          // Imc1 is participating in the 3 channel way interleave for this socket
          BuildMemDevToSPARangeMapTbls(pNfit, Soc, 1, 3, pSPARDT, NumIntTbls, (RgnOffsetMultiplier[j] * ChGran), (3 * ImcWays), i, &PmemRegion->NvDimms[0]);
          NumIntTbls++;
          j++;
          BuildMemDevToSPARangeMapTbls(pNfit, Soc, 1, 4, pSPARDT, NumIntTbls, (RgnOffsetMultiplier[j] * ChGran), (3 * ImcWays), i, &PmemRegion->NvDimms[1]);
          NumIntTbls++;
          j++;
          BuildMemDevToSPARangeMapTbls(pNfit, Soc, 1, 5, pSPARDT, NumIntTbls, (RgnOffsetMultiplier[j] * ChGran), (3 * ImcWays), i, &PmemRegion->NvDimms[2]);
          NumIntTbls++;
          j++;
        }
      }
    }
  }
}

/**

This routine adds SMBIOS Management Information Descriptor Table
to Nfit. This Table essentiall has SMBios Type17 handles of all
NVMDimms installed in the system.

  @param pNfit        - Pointer to Nfit Table

  @retval VOID        - Nothing

**/
VOID
AddSMBiosMgmtInfoTblToNfit (
  NVDIMM_FW_INTERFACE_TABLE    *pNfit
  )
{
  SMBIOS_MGMT_INFO_TABLE     *pSMBiosTbl;
  UINT16                     *Handles;
  UINT8                      i;

  if (pNfit == NULL) {
    return;
  }

  pSMBiosTbl = (SMBIOS_MGMT_INFO_TABLE *)(UINTN)(&pNfit->NfitTables[0] + pNfit->Length);
  pSMBiosTbl->Type = TYPE_SMBIOS_MGMT_INFO_DESCRIPTOR;
  pSMBiosTbl->Length = (sizeof(*pSMBiosTbl) + (mNumSMBiosHandles * 2));

  Handles = (UINT16 *)((UINT8 *)pSMBiosTbl + sizeof(*pSMBiosTbl));
  for (i = 0; i < mNumSMBiosHandles; i++) {
    Handles[i] = SMBiosHandleInfo[i].SMBiosType17Handle;
  }
  //
  // Update the Nfit length as appropriate

  ASSERT((pNfit->Length + pSMBiosTbl->Length) <= mNfitSize);
  pNfit->Length += pSMBiosTbl->Length;
}


/*++

Routine Description:

This routine checks to see if the Limit field of the current SAD Table has been previously
accounted for thru another SAD. The idea is to eliminate redundancy while adding SADs to the Nfit.
This is accomplished by adding the current SAD's limit to the Sad_Info struct when the current Limit is not found
in the Limits of Sad_Info and increment NumOfSads of Sad_Info struct.

Arguments:

pSAD      - Pointer to the current SAD

Returns:

TRUE if this SAD's limit has been found in another SAD that was added previously. Else FALSE

--*/
BOOLEAN
CheckIfRedundantSAD (
  struct SADTable                 *pSAD
  )
{
  UINT8 i;

  ASSERT(pSAD != NULL);
  if (pSAD == NULL) {
    return FALSE;
  }

  if (mSAD_Info.NumOfSADs == 0) {
    mSAD_Info.Limits[0] = pSAD->Limit;
    mSAD_Info.NumOfSADs++;
  } else {
    for (i = 0; i < mSAD_Info.NumOfSADs; i++) {
      if (mSAD_Info.Limits[i] == pSAD->Limit) {
        return TRUE;
      }
    }
    if (i == mSAD_Info.NumOfSADs) {
      mSAD_Info.Limits[i] = pSAD->Limit;
      mSAD_Info.NumOfSADs++;
    }
  }
  return FALSE;
}

/**

Routine Description:

This routine basically builds NvDIMM F/W Interface Table, appending the sub-tables to Nfit
using the information from the SAD struct passed in.

Arguments:

  @param pNfit -  pointer to Nvdimm f/w interface table
  @param socket - socket number
  @param SadInx - SAD Index that needs to be added to Nfit

Returns:

  @retval VOID

**/
VOID
AppendToNFIT (
  NVDIMM_FW_INTERFACE_TABLE     *pNfit,
  UINT8                         socket,
  UINT8                         SadInx,
  PMEM_REGION                   *PmemRegion
  )
{
  struct SADTable       *pSAD;
  SPA_RANGE_DESC_TABLE  *pSPARDT = NULL;
  UINT64                SpaBase = 0;
  UINT8                 ChInterleave;
  UINT8                 ImcWays;
  UINT32                InterBitmap;
  UINT8                 Imc = 0;
  UINT32                SktGran, ChGran;
  UINT8                 NvDimmIndex;

  EFI_GUID PMEMRegionGuid = PMEM_REGION_GUID;

  // If we would go outside bounds for SadIntList[Sadinx], return.
  if (pNfit == NULL || socket >= MAX_SOCKET || SadInx >= MAX_SAD_RULES) {
    return;
  }

  pSAD = &mSystemMemoryMap->Socket[socket].SAD[SadInx];

  ASSERT(pSAD != NULL);
  if (pSAD == NULL) {
    return;
  }
    DEBUG((EFI_D_ERROR, "Nfit: NfitTables = 0x%X\n", &pNfit->NfitTables[0]));

    // Get the pointer to the next Nfit table, which in this case is SPARDT.
    pSPARDT = (SPA_RANGE_DESC_TABLE*)(UINTN)(&pNfit->NfitTables[0] + pNfit->Length);
    DEBUG((EFI_D_ERROR, "Nfit: pSPARDT = 0x%X\n", pSPARDT));

    pSPARDT->Type = TYPE_SPA_RANGE_DESCRIPTOR;

    // Right attributes will be set further down below, start with 0.
    pSPARDT->AddressRangeMemoryMappingAttribute = 0;

    CopyGuid(&pSPARDT->AddressRangeTypeGUID,  &PMEMRegionGuid);
    pSPARDT->AddressRangeMemoryMappingAttribute = EFI_MEMORY_NV | EFI_MEMORY_WB;

    if (pSAD->mirrored != 0){
      pSPARDT->AddressRangeMemoryMappingAttribute |=  EFI_MEMORY_MORE_RELIABLE;
    }

    pSPARDT->SPARangeStructureIndex = mSPARangeDescTblIndex;
    mSPARangeDescTblIndex++;

    //
    // SPARDT Proximity Domain is the socket # (at least for now),
    // When SNC (Sub-Numa Cluster is enabled, this could change), we need to check when it is enabled.
    //
    if (mSystemMemoryMap->sncEnabled && (mSystemMemoryMap->NumOfCluster == 2)) {
      if (mSystemMemoryMap->Socket[socket].SadIntList[SadInx][socket * MAX_IMC]) {
        // Imc0 of this socket is involved in this interleave set,
        Imc = 0;
      } else {
        Imc = 1;
      }
      pSPARDT->ProximityDomain = (socket * 2) + Imc;
    } else {
      pSPARDT->ProximityDomain = socket;
    }

    //
    // Update the Flag of the SPARDT
    pSPARDT->Flags = PROX_DOMAIN_VALID;


    SpaBase = pSPARDT->SystemPhysicalAddressRangeBase = PmemRegion->BaseAddress;
    pSPARDT->SystemPhysicalAddressRangeLength = PmemRegion->Size;

    DEBUG((EFI_D_ERROR, "Nfit: StartAddr = 0x%lx\n", pSPARDT->SystemPhysicalAddressRangeBase));
    DEBUG((EFI_D_ERROR, "Nfit: Size = 0x%lx\n", pSPARDT->SystemPhysicalAddressRangeLength));

    pSPARDT->Length = sizeof(SPA_RANGE_DESC_TABLE);
    //
    // At this point we've updated the SPARDT table, include its length into Nfit Length
    //
    ASSERT((pNfit->Length + pSPARDT->Length) <= mNfitSize);
    pNfit->Length += pSPARDT->Length;

  for (NvDimmIndex = 0; NvDimmIndex < PmemRegion->NumNvDimms; NvDimmIndex++) {
    mCtrlRegionIndex++;
    PmemRegion->NvDimms[NvDimmIndex].CntrlRegionIndex = mCtrlRegionIndex;
    AddACtrlRgnToNfit(pNfit, &PmemRegion->NvDimms[NvDimmIndex]);
  }

  ChInterleave =  GetChInterleave(socket, SadInx);
  ImcWays =       GetImcWays(socket, SadInx);
  InterBitmap =   GetChInterBitmap(pSAD);
  SktGran =       GetSocketGranularity(pSAD);
  ChGran =        GetChannelGranularity(pSAD);

  switch (ChInterleave) {
  case CH_1WAY:
    // 1st Case - if channel is interleaved across both IMCs
    // 1 or 2 Imc, 1 Way Channel Interleaved, but which channel is Interleaved?
    if (ImcWays == 0) {
      DEBUG((EFI_D_ERROR, "Nfit: ImcWays = 0\n"));
      return;
    }

    BuildMemDevFor1WayChInt (pNfit, ImcWays, pSPARDT, SpaBase, PmemRegion);
    //Build1ChWayIntDescTbls(pNfit, SktGran, ChGran, ImcWays);
    break;
  case CH_2WAY:
    // Build Nvdimm F/W Interface Table 2 way Channel Interleaved
    BuildMemDevFor2WayChInt (pNfit, ImcWays, pSPARDT, SpaBase, PmemRegion);
    //Build2ChWayIntDescTbls(pNfit, SktGran, ChGran, ImcWays);
    break;
  case CH_3WAY:
    // Build Nvdimm F/W Interface Table 3 way Channel Interleaved
    BuildMemDevFor3WayChInt (pNfit, ImcWays, pSPARDT, SpaBase, PmemRegion);
    //Build3ChWayIntDescTbls(pNfit, SktGran, ChGran, ImcWays);
    break;
  default:
    DEBUG ((EFI_D_INFO, "default case.\n"));  //Auto added. Please review.
    break;
  }
}

/**

Routine Description:

This routine updates the checksum field of already initialized NFIT table.

Arguments:

  @param JedecNvdimmTablePtr - pointer to NFIT table address allocated
                                in platform ACPI code

Returns:

  @retval EFI_SUCCESS - checksum updated successfully
  @retval EFI_INVALID_PARAMETER - JedecNvdimmTablePtr is NULL

**/
EFI_STATUS
UpdateNfitTableChecksum (
  UINT64 *JedecNvdimmTablePtr
  )
{
  NVDIMM_FW_INTERFACE_TABLE *pNfit;

  if (JedecNvdimmTablePtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  pNfit = (NVDIMM_FW_INTERFACE_TABLE *)JedecNvdimmTablePtr;
  pNfit->Checksum = ChsumTbl((UINT8 *)pNfit, pNfit->Length);
  return EFI_SUCCESS;
}

/**

Routine Description:

This routine basically builds NvDIMM F/W Interface Table,
appending the needed sub-tables.

Arguments:

  @param JedecNvdimmTablePtr - pointer to NFIT table address allocated
                                in platform ACPI code

Returns:

  @retval EFI_SUCCESS - NFIT table created successfully.
  @retval EFI_INVALID_PARAMETER - JedecNvdimmTablePtr is NULL

**/

/**

Routine Description:GetDcpmmSMBiosType17Handles traces thru
all installed DCPMMs and gets SMBios Type17 handles and saves
these handles in the global struct for later use by Nfit
creation code. updates Dimm info data struct of the NVMDIMM's
(found) Control Region and other info about the Dimm.

  @param socket      - Socket Number
  @param ch          - DDR Channel ID
  @param dimm        - DIMM number
  @retval VOID       - None

**/
UINT16
GetDcpmmSMBiosType17Handles (
   VOID
   )
{
  EFI_STATUS               Status;
  UINT8                    Socket, Ch, Dimm;
  UINT16                   i;
  EFI_SMBIOS_PROTOCOL      *SmbiosProtocol = NULL;
  EFI_SMBIOS_TABLE_HEADER  *SmbiosRecord = NULL;
  EFI_SMBIOS_HANDLE        SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **)&SmbiosProtocol);
  ASSERT_EFI_ERROR (Status);
  i = 0;
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Ch = 0; Ch < MAX_CH; Ch++) {
      if (mSystemMemoryMap->Socket[Socket].ChannelInfo[Ch].Enabled == 0) {
        continue;
      }
      // Get the dimmNvList to see if any of them are NVDIMMs
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (mSystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].Present == 0) {
          continue;
        }
        Status = GetNextValidSmbiosHandle (SmbiosProtocol, &SmbiosHandle, &SmbiosRecord);
        if ((!(EFI_ERROR(Status))) && (IsNvDimm (Socket, Ch, Dimm)) && SmbiosRecord != NULL) {
          // Found an NVMDIMM, now get it's SMBios Type17 Handle and save it.
          // Save the handle in the SMBiosHandleInfo & other info
          SMBiosHandleInfo[i].Socket = Socket;
          SMBiosHandleInfo[i].Channel = Ch;
          SMBiosHandleInfo[i].Dimm = Dimm;
          SMBiosHandleInfo[i].SMBiosType17Handle = SmbiosRecord->Handle;
          i++;
        }
      }
    }
  }
  return (i);
}

/**

Routine Description:GetNextValidSmbiosHandle iterates through
the list of SmbiosRecords and returns the valid SMBIOS17 entry.

  @param SmbiosProtocol      - Handler to SMBIOS_PROTOCOL
  @param SmbiosHandle          pointer to returned valid SMBIOS handle
  @param SmbiosRecord        - pointer to SMBBIOSRECORD
  @retval Status             - Returned Status

**/

EFI_STATUS
GetNextValidSmbiosHandle (
  EFI_SMBIOS_PROTOCOL *SmbiosProtocol,
  EFI_SMBIOS_HANDLE *SmbiosHandle,
  EFI_SMBIOS_TABLE_HEADER  **SmbiosRecord
  )
{
  EFI_STATUS               Status = EFI_NOT_FOUND;
  EFI_SMBIOS_TYPE          SmbiosType;
  SMBIOS_TABLE_TYPE17      *SmbiosType17Record;
  UINT8 Recordfound = FALSE;
  SmbiosType = EFI_SMBIOS_TYPE_MEMORY_DEVICE;
  do {
    Status = SmbiosProtocol->GetNext (SmbiosProtocol, SmbiosHandle, &SmbiosType, SmbiosRecord, NULL);
    SmbiosType17Record = (SMBIOS_TABLE_TYPE17 *)*SmbiosRecord;
    if (SmbiosType17Record->Size != 0 &&
      SmbiosType17Record->Speed != 0 &&
      SmbiosType17Record->TotalWidth != 0 &&
      SmbiosType17Record->DataWidth != 0 &&
      SmbiosType17Record->MemoryType != MemoryTypeUnknown) {
      Recordfound = TRUE;
    }
  } while (!EFI_ERROR(Status) && !Recordfound);

  return Status;
}

EFI_STATUS
BuildNfitTable (
  UINT64        *JedecNvdimmTablePtr,
  UINTN         JedecNvdimmTableSize,
  PMEM_REGION   *PmemRegions
  )
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  NVDIMM_FW_INTERFACE_TABLE   *pNfit;
  UINT8                       socket;
  UINT8                       i;
  UINT8                       PmemIndex;
  //UINT64                      TempOemTableId;

  if (JedecNvdimmTablePtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get the SMBios Type17 Handles for all the Nvdimms
  //
  mNumSMBiosHandles = GetDcpmmSMBiosType17Handles();

  //
  // Start building the Nfit based on the current memory configuration,
  // before it can be published as part of the Dsdt. Nfit is required ONLY
  // when DCPMM Dimms are installed
  //

  // Initialize Sad Info, particularly Number of SADs participating in Nfit
  mSAD_Info.NumOfSADs = 0;

  // Also initialize others that are for Unique ID numbers.
  // This Index is 1 based.
  mSPARangeDescTblIndex = 1;
  mMemDevRegionalID = mNumInterleaveDescTbls = 0;

  //
  // Adding support for Nfit, Adding Nvdimm F/W Interface Table as part of the DsDt
  //
  pNfit = (NVDIMM_FW_INTERFACE_TABLE *)JedecNvdimmTablePtr;
  DEBUG((EFI_D_ERROR, "Nfit: main table address = 0x%p\n", pNfit));

  mNfitSize = (UINT32)JedecNvdimmTableSize;
  ZeroMem(pNfit, mNfitSize);

//  TempOemTableId  = PcdGet64(PcdAcpiDefaultOemTableId);
  //
  // Create NFIT header
  //
  pNfit->Signature      = NVDIMM_FW_INTERFACE_TABLE_SIGNATURE;
  pNfit->Length         = 0;
  pNfit->Revision       = NVDIMM_FW_INTERFACE_TABLE_REVISION;
  CopyMem(pNfit->OemID,    PcdGetPtr (PcdAcpiDefaultOemId), sizeof(pNfit->OemID));
  pNfit->OemTblID       = PcdGet64(PcdAcpiDefaultOemTableId);
  //CopyMem(pNfit->OemTblID,  &TempOemTableId, sizeof(pNfit->OemTblID));
  pNfit->OemRevision    = EFI_ACPI_OEM_REVISION;
  pNfit->CreatorID      = EFI_ACPI_CREATOR_ID;
  pNfit->CreatorRev     = EFI_ACPI_CREATOR_REVISION;

  //
  // Create NFIT sub-tables
  //
  for (PmemIndex = 0; PmemIndex < MAX_PMEM_REGIONS; PmemIndex++) {
    if (PmemRegions[PmemIndex].Size == 0) {
      continue;
    }
    socket = PmemRegions[PmemIndex].SocketNum;
    i = PmemRegions[PmemIndex].SadIndex;
    if (mSystemMemoryMap->Socket[socket].SAD[i].local) {
      AppendToNFIT(pNfit, socket, i, &PmemRegions[PmemIndex]);
    }
  }

  //
  // Add SMBIOS Management Information Table to Nfit
  //
  AddSMBiosMgmtInfoTblToNfit(pNfit);

  pNfit->Length += sizeof(NFIT_HEADER);
  DEBUG((EFI_D_ERROR, "Nfit: After adding all tables, Nfit Length = 0x%X\n", pNfit->Length));

  Status = UpdateNfitTableChecksum(JedecNvdimmTablePtr);

  return Status;
}
