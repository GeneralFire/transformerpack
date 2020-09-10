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

#include <Library/PcuMailBoxLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <SiliconSetting.h>
#include <Upi/KtiSi.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/RasRcMirrorLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/BaseLib.h>
#include <Protocol/IioUds.h>
#include <Protocol/NvdimmSmbusSmmProtocol.h>
#include <Library/MemTypeLib.h>
#ifdef SKX_HOST
#include <Mem/Library/MemMcIpLib14nm/Include/MemMcRegs.h>
#else
#include <Mem/Library/MemMcIpLib/Include/MemMcRegs.h>
#endif
#include "JedecNvdimm.h"
#include "Nfit.h"
#include "NvdimmSmbusAccess.h"
#include <Library/NvdimmCommonLib.h>
#include "AcpiTableHandle.h"
#include "NfitTableUpdate.h"
#include "NvdimmAcpiSmmInterface.h"
#include <Guid/MemoryMapData.h>
#include <Memory/NvdimmnDefinitions.h>
#include <Library/DebugPrintErrorLevelLib.h>

#ifndef __GNUC__
#pragma warning(disable : 4100)
#pragma optimize("", off)
#endif  //__GNUC__

//
// Globals
BOOLEAN                         mInSmm;
struct SystemMemoryMapHob       *mSystemMemoryMap;
NVDIMM_COMMON_INFO              NvdimmInfo;
EFI_SMM_SYSTEM_TABLE2           *mSmst = NULL;

SMBIOS_HANDLE_INFO              SMBiosHandleInfo[MAX_SOCKET * MAX_CH * MAX_DIMM];
EFI_CPU_CSR_ACCESS_PROTOCOL     *mCpuCsrAccess;
NVDIMM_ACPI_SMM_INTERFACE       *mNvdimmAcpiSmmInterface = NULL;
NVDIMM_SMBUS_SMM_INTERFACE              *mNvdimmSmbusSmmInterface = NULL;
EFI_NVDIMM_SMBUS_SMM_INTERFACE_PROTOCOL *mNvdimmSmbusSmmInterfaceProtocol;
EFI_SMM_BASE2_PROTOCOL          *mSmmBase = NULL;
UINT8                           Skts[MAX_SOCKET];
UINT8                           Chs[MAX_MC_CH * MAX_IMC];

// Used for ARS Implementation
BOOLEAN                         mIsArs = FALSE;
BOOLEAN                         mHasArsPerformed = FALSE;
UINT64                          ArsStartSpa;
UINT32                          mPmemIndex;
ARS_DSM_HEADER                  mArsDsmHeader;
ARS_SCRUB_DATA                  mArsScrubData;
MCMAIN_CHKN_BITS_MC_MAIN_STRUCT mMcMainChknRegSave[MC_MAX_NODE];
UINT64                          PMEMSADBaseAddresses[MAX_PMEM_INTERLEAVE_SET];
BOOLEAN                         IsDcpmmPresent;
BOOLEAN                         IsJedecNvdimmPresent = FALSE;
PMEM_REGION                     *mPmemRegions;
UINT32                          *mScrubIntervalSave;
UINT32                          mClearUcErrLenUnit = 1; // TODO: in bytes, this is prob the minimum error record length (1 byte or rank?)
UINT32                          mArsTimeEstimate = 100; // TODO: in seconds, how do we determine estimated time?

UINT8                           mMaxErrorsPerRank;
UINT8                           mNumErrorsCurrRank;

typedef struct {
  AMAP_MC_MAIN_STRUCT                 AMAPReg;
} CH_AMAP_STRUCT2;

typedef enum {
  SCRUBBING_NON_SPARERANK,
  SCRUBBING_SPARERANK
} SSRSTATETYPE;

typedef struct {
  SCRUBCTL_MC_MAIN_STRUCT              ScrubCtlReg;
  SCRUBADDRESSLO_MC_MAIN_STRUCT        ScrubAddrLoReg;
  SCRUBADDRESSHI_MC_MAIN_STRUCT        ScrubAddrHiReg;
  SCRUBADDRESS2LO_MC_MAIN_STRUCT       ScrubAddrLo2Reg;
  SCRUBADDRESS2HI_MC_MAIN_STRUCT       ScrubAddrHi2Reg;
  UINT32                               *ScrubInterval;
  CH_AMAP_STRUCT2                      AMAPReg[MAX_MC_CH];

  SSRSTATETYPE                         SSRState;
  UINT8                                ScrubSpareCh;
  UINT8                                ScrubSpareRank;
} PATROL_SCRUB_STATUS_STRUC;

static
PATROL_SCRUB_STATUS_STRUC mPatrolScrubStatus[MC_MAX_NODE];


// For 4k/256 interleave granularity, the SAD interleave allows for 1, 2, 3, 4, 6, 8, 12, 16, and 24 way channel interleave
// For 3ChWay, this allows Imc1Way, Imc2Way, Imc4Way, and Imc8Way
// For 2ChWay, this allows Imc1Way, Imc2Way, Imc4Way, and Imc8Way

// For 4k/4k interleave granularity, the SAD interleave allows for 1, 2, 3, 4, 6, 8, 12, and 24 way channel interleave
// For 3ChWay, this allows Imc1Way, Imc2Way, Imc4Way, and Imc8Way
// For 2ChWay, this allows Imc1Way, Imc2Way, and Imc4Way

//
// The following table is used for the translation of Dpa to Spa based for the given ImcWays and 3 channel ways that the dimms participate in the
// interleave set. These numbers for this table is derived from the TestNfit Tool which is available separately.
//
UINT32  Imc1Way4k_3ChWay256_LineMultipliers[3][16] = {{ 0, 3, 6, 9,  12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45 },                  // Imc0/Ch0
                                                      { 1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31, 34, 37, 40, 43, 46 },                 // Imc0/Ch1
                                                      { 2, 5, 8, 11, 14, 17, 20, 23, 26, 29, 32, 35, 38, 41, 44, 47 }};                // Imc0/Ch2

UINT32  Imc2Way4k_3ChWay256_LineMultipliers[6][16] = {{ 0,  3,  6,  9,  12, 15, 66, 69, 72, 75, 78, 33, 36, 39, 42, 45 },                  // Imc0/Ch0
                                                      { 1,  4,  7,  10, 13, 64, 67, 70, 73, 76, 79, 34, 37, 40, 43, 46 },                 // Imc0/Ch1
                                                      { 2,  5,  8,  11, 14, 65, 68, 71, 74, 77, 32, 35, 38, 41, 44, 47 },                 // Imc0/Ch2
                                                      { 48, 51, 54, 57, 60, 63, 18, 21, 24, 27, 30, 81, 84, 87, 90, 93 },              // Imc1/Ch0
                                                      { 49, 52, 55, 58, 61, 16, 19, 22, 25, 28, 31, 82, 85, 88, 91, 94 },              // Imc1/Ch1
                                                      { 50, 53, 56, 59, 62, 17, 20, 23, 26, 29, 80, 83, 86, 89, 92, 95 }};             // Imc1/Ch2

UINT32  Imc4Way4k_3ChWay256_LineMultipliers[12][16] = {{ 0,   3,   6,   9,   12,  15,  66,  69,  72,  75,  78,  129, 132, 135, 138, 141 },           // Imc0/Ch0
                                                       { 1,   4,   7,   10,  13,  64,  67,  70,  73,  76,  79,  130, 133, 136, 139, 142 },          // Imc0/Ch1
                                                       { 2,   5,   8,   11,  14,  65,  68,  71,  74,  77,  128, 131, 134, 137, 140, 143 },         // Imc0/Ch2
                                                       { 144, 147, 150, 153, 156, 159, 18,  21,  24,  27,  30,  81,  84,  87,  90,  93 },      // Imc1/Ch0
                                                       { 145, 148, 151, 154, 157, 16,  19,  22,  25,  28,  31,  82,  85,  88,  91,  94 },       // Imc1/Ch1
                                                       { 146, 149, 152, 155, 158, 17,  20,  23,  26,  29,  80,  83,  86,  89,  92,  95 },       // Imc1/Ch2
                                                       { 96,  99,  102, 105, 108, 111, 162, 165, 168, 171, 174, 33,  36,  39,  42,  45 },   // Imc2/Ch0
                                                       { 97,  100, 103, 106, 109, 160, 163, 166, 169, 172, 175, 34,  37,  40,  43,  46 },  // Imc2/Ch1
                                                       { 98,  101, 104, 107, 110, 161, 164, 167, 170, 173, 32,  35,  38,  41,  44,  47 },   // Imc2/Ch2
                                                       { 48,  51,  54,  57,  60,  63,  114, 117, 120, 123, 126, 177, 180, 183, 186, 189 },  // Imc3/Ch0
                                                       { 49,  52,  55,  58,  61,  112, 115, 118, 121, 124, 127, 178, 181, 184, 187, 190 }, // Imc3/Ch1
                                                       { 50,  53,  56,  59,  62,  113, 116, 119, 122, 125, 176, 179, 182, 185, 188, 191 }};// Imc3/Ch2

UINT32  Imc8Way4k_3ChWay256_LineMultipliers[24][16]  = {{ 0,   3,   6,   9,   12,  15,  258, 261, 264, 267, 270, 129, 132, 135, 138, 141 },           // Imc0/Ch0
                                                        { 1,   4,   7,   10,  13,  256, 259, 262, 265, 268, 271, 130, 133, 136, 139, 142 },         // Imc0/Ch1
                                                        { 2,   5,   8,   11,  14,  257, 260, 263, 266, 269, 128, 131, 134, 137, 140, 143 },         // Imc0/Ch2
                                                        { 144, 147, 150, 153, 156, 159, 18,  21,  24,  27,  30,  273, 276, 279, 282, 285 },      // Imc1/Ch0
                                                        { 145, 148, 151, 154, 157, 16,  19,  22,  25,  28,  31,  274, 277, 280, 283, 286 },       // Imc1/Ch1
                                                        { 146, 149, 152, 155, 158, 17,  20,  23,  26,  29,  272, 275, 278, 281, 284, 287 },      // Imc1/Ch2
                                                        { 288, 291, 294, 297, 300, 303, 162, 165, 168, 171, 174, 33,  36,  39,  42,  45 },      // Imc2/Ch0
                                                        { 289, 292, 295, 298, 301, 160, 163, 166, 169, 172, 175, 34,  37,  40,  43,  46 },      // Imc2/Ch1
                                                        { 290, 293, 296, 299, 302, 161, 164, 167, 170, 173, 32,  35,  38,  41,  44,  47 },       // Imc2/Ch2
                                                        { 48,  51,  54,  57,  60,  63,  306, 309, 312, 315, 318, 177, 180, 183, 186, 189 },       // Imc3/Ch0
                                                        { 49,  52,  55,  58,  61,  304, 307, 310, 313, 316, 319, 178, 181, 184, 187, 190 },      // Imc3/Ch1
                                                        { 50,  53,  56,  59,  62,  305, 308, 311, 314, 317, 176, 179, 182, 185, 188, 191 },      // Imc3/Ch2
                                                        { 192, 195, 198, 201, 204, 207, 66,  69,  72,  75,  78,  321, 324, 327, 330, 333 },      // Imc4/Ch0
                                                        { 193, 196, 199, 202, 205, 64,  67,  70,  73,  76,  79,  322, 325, 328, 331, 334 },       // Imc4/Ch1
                                                        { 194, 197, 200, 203, 206, 65,  68,  71,  74,  77,  320, 323, 326, 329, 332, 335 },      // Imc4/Ch2
                                                        { 336, 339, 342, 345, 348, 351, 210, 213, 216, 219, 222, 81,  84,  87,  90,  93 },      // Imc5/Ch0
                                                        { 337, 340, 343, 346, 349, 208, 211, 214, 217, 220, 223, 82,  85,  88,  91,  94 },      // Imc5/Ch1
                                                        { 338, 341, 344, 347, 350, 209, 212, 215, 218, 221, 80,  83,  86,  89,  92,  95 },       // Imc5/Ch2
                                                        { 96,  99,  102, 105, 108, 111, 354, 357, 360, 363, 366, 225, 228, 231, 234, 237 },   // Imc6/Ch0
                                                        { 97,  100, 103, 106, 109, 352, 355, 358, 361, 364, 367, 226, 229, 232, 235, 238 },  // Imc6/Ch1
                                                        { 98,  101, 104, 107, 110, 353, 356, 359, 362, 365, 224, 227, 230, 233, 236, 239 },  // Imc6/Ch2
                                                        { 240, 243, 246, 249, 252, 255, 114, 117, 120, 123, 126, 369, 372, 375, 378, 381 }, // Imc7/Ch0
                                                        { 241, 244, 247, 250, 253, 112, 115, 118, 121, 124, 127, 370, 373, 376, 379, 382 }, // Imc7/Ch1
                                                        { 242, 245, 248, 251, 254, 113, 116, 119, 122, 125, 368, 371, 374, 377, 380, 383 }};// Imc7/Ch2

// 4k Channel granularity multipliers
UINT32  Imc1Way4k_3ChWay4k_LineMultipliers[3][2] = {{ 0, 3 }, { 1, 4 }, { 2, 5 }};  // Imc0

UINT32  Imc2Way4k_3ChWay4k_LineMultipliers[6][2] = {{ 0, 6 }, { 4, 10 }, { 2, 8 },    // Imc0
                                                    { 3, 9 }, { 1, 7 },  { 5, 11 }};  // Imc1

UINT32   Imc4Way4k_3ChWay4k_LineMultipliers[12][2] = {{ 0, 12 }, { 4, 16 },  { 8, 20 }, // Imc0
                                                      { 9, 21 }, { 1, 13 },  { 5, 17 }, // Imc1
                                                      { 6, 18 }, { 10, 22 }, { 2, 14 },
                                                      { 3, 15 }, { 7, 19 },  { 11, 23 }};

UINT32  Imc8Way4k_3ChWay4k_LineMultipliers[24][2]  = {{ 0, 24 },  { 16, 40 }, { 8, 32 },  // Imc0
                                                      { 9, 33 },  { 1, 25 },  { 17, 41 }, // Imc1
                                                      { 18, 42 }, { 10, 34 }, { 2, 26 },
                                                      { 3, 27 },  { 19, 43 }, { 11, 35 },
                                                      { 12, 36 }, { 4, 28 },  { 20, 44 },
                                                      { 21, 45 }, { 13, 37 }, { 5, 29 },
                                                      { 6, 30 },  { 22, 46 }, { 14, 38 },
                                                      { 15, 39 }, { 7, 31 },  { 23, 47 }};

//
// The following table is used for the translation of Dpa to Spa based for the given ImcWays and 2 channel ways that the dimms participate in the
// interleave set.
//
UINT32  Imc1Way4k_2ChWay256_LineMultipliers[2][8] = {{ 0, 2, 4, 6, 8, 10, 12, 14 }, // Imc0/Ch0
                                                     { 1, 3, 5, 7, 9, 11, 13, 15 }};

UINT32  Imc2Way4k_2ChWay256_LineMultipliers[4][8] = {{ 0,  2,  4,  6,  8,  10, 12, 14 },  // Imc0/Ch0
                                                     { 1,  3,  5,  7,  9,  11, 13, 15 },
                                                     { 16, 18, 20, 22, 24, 26, 28, 30 },
                                                     { 17, 19, 21, 23, 25, 27, 29, 31 }};

UINT32  Imc4Way4k_2ChWay256_LineMultipliers[8][8] = {{ 0,  2,  4,  6,  8,  10, 12, 14 },  // Imc0/Ch0
                                                     { 1,  3,  5,  7,  9,  11, 13, 15 },
                                                     { 16, 18, 20, 22, 24, 26, 28, 30 },
                                                     { 17, 19, 21, 23, 25, 27, 29, 31 },
                                                     { 32, 34, 36, 38, 40, 42, 44, 46 },
                                                     { 33, 35, 37, 39, 41, 43, 45, 47 },
                                                     { 48, 50, 52, 54, 56, 58, 60, 62 },
                                                     { 49, 51, 53, 55, 57, 59, 61, 63 }};

UINT32  Imc8Way4k_2ChWay256_LineMultipliers[16][8] = {{ 0,   2,   4,   6,   8,   10,  12,  14 },  // Imc0/Ch0
                                                      { 1,   3,   5,   7,   9,   11,  13,  15 },
                                                      { 16,  18,  20,  22,  24,  26,  28,  30 },
                                                      { 17,  19,  21,  23,  25,  27,  29,  31 },
                                                      { 32,  34,  36,  38,  40,  42,  44,  46 },
                                                      { 33,  35,  37,  39,  41,  43,  45,  47 },
                                                      { 48,  50,  52,  54,  56,  58,  60,  62 },
                                                      { 49,  51,  53,  55,  57,  59,  61,  63 },
                                                      { 64,  66,  68,  70,  72,  74,  76,  78 },
                                                      { 65,  67,  69,  71,  73,  75,  77,  79 },
                                                      { 80,  82,  84,  86,  88,  90,  92,  94 },
                                                      { 81,  83,  85,  87,  89,  91,  93,  95 },
                                                      { 96,  98,  100, 102, 104, 106, 108, 110 },
                                                      { 97,  99,  101, 103, 105, 107, 109, 111 },
                                                      { 112, 114, 116, 118, 120, 122, 124, 126 },
                                                      { 113, 115, 117, 119, 121, 123, 125, 127 }};

// 4k Channel granularity multipliers
UINT32  Imc1Way4k_2ChWay4k_LineMultipliers[2][2] = {{ 0, 2 }, { 1, 3 }};  // Imc0

UINT32  Imc2Way4k_2ChWay4k_LineMultipliers[4][2] = {{ 0, 4 }, { 1, 5 },   // Imc0
                                                    { 2, 6 }, { 3, 7 }};  // Imc1

UINT32  Imc4Way4k_2ChWay4k_LineMultipliers[8][2] = {{ 0, 8 },  { 1, 9 },  // Imc0
                                                    { 2, 10 }, { 3, 11 }, // Imc1
                                                    { 4, 12 }, { 5, 13 },
                                                    { 6, 14 }, { 7, 15 }};


/**

  Save Patrol scrub engine registers

  @param NodeId - Memory controller ID


  @retval EFI_SUCCESS - Always

  **/
EFI_STATUS
SavePatrolScrubEngine (
  IN UINT8 Node
  )
{
  UINT8                        Socket = NODE_TO_SKT(Node);
  UINT8                        Mc     = NODE_TO_MC(Node);
  UINT8                        ChOnMc;
  SCRUBCTL_MC_MAIN_STRUCT      ScrubCtlReg;

  //
  // Save registers and disable scrubbing
  //
  PcuGetScrubInterval(Socket, Mc, mPatrolScrubStatus[Node].ScrubInterval);

  mPatrolScrubStatus[Node].ScrubAddrLoReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBADDRESSLO_MC_MAIN_REG);
  mPatrolScrubStatus[Node].ScrubAddrHiReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBADDRESSHI_MC_MAIN_REG);
  mPatrolScrubStatus[Node].ScrubAddrLo2Reg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBADDRESS2LO_MC_MAIN_REG);
  mPatrolScrubStatus[Node].ScrubAddrHi2Reg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBADDRESS2HI_MC_MAIN_REG);
  ScrubCtlReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBCTL_MC_MAIN_REG);
  mPatrolScrubStatus[Node].ScrubCtlReg.Data    =  ScrubCtlReg.Data;


  for (ChOnMc = 0; ChOnMc < MAX_MC_CH; ChOnMc ++) {
      mPatrolScrubStatus[Node].AMAPReg[ChOnMc].AMAPReg.Data =  mCpuCsrAccess->ReadCpuCsr(Socket,NODECH_TO_SKTCH(Mc,ChOnMc), AMAP_MC_MAIN_REG);
  }

  return EFI_SUCCESS;
}

/**

  Disable Patrol scrub engine and save patrol registers

  @param NodeId - Memory controller ID


  @retval EFI_SUCCESS - Always

  **/
EFI_STATUS
EFIAPI
DisablePatrolScrubEngine (
  IN UINT8 Node
  )
{
  UINT8                        Socket = NODE_TO_SKT(Node);
  UINT8                        Mc     = NODE_TO_MC(Node);
  SCRUBCTL_MC_MAIN_STRUCT      ScrubCtlReg;

  ScrubCtlReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBCTL_MC_MAIN_REG);
  if (ScrubCtlReg.Bits.scrub_en == 1) {

    //
    //  disable scrubbing
    //
    ScrubCtlReg.Bits.scrub_en = 0;
    mCpuCsrAccess->WriteMcCpuCsr(Socket, Mc, SCRUBCTL_MC_MAIN_REG, ScrubCtlReg.Data);
  }

  return EFI_SUCCESS;
}

/**

  Enable Patrol scrub engine to start from the last Address.

  @param NodeId - Memory controller ID

  @retval EFI_SUCCESS - always

  **/
EFI_STATUS
EFIAPI
ReEnablePatrolScrubEngine (
  IN UINT8 Node
  )
{
  UINT8 Socket = NODE_TO_SKT(Node);
  UINT8 Mc = NODE_TO_MC(Node);
  SCRUBCTL_MC_MAIN_STRUCT      ScrubCtlReg;

  //
  // Disable scrubbing bgefore enbaling the scrub address registers.
  //
  ScrubCtlReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBCTL_MC_MAIN_REG);
  if (ScrubCtlReg.Bits.scrub_en == 1) {
    ScrubCtlReg.Bits.scrub_en = 0;
    mCpuCsrAccess->WriteMcCpuCsr (Socket, Mc, SCRUBCTL_MC_MAIN_REG, ScrubCtlReg.Data);
  }
  //
  // Patrol scrub was enabled before. Re-enable it now and restore old register values
  //
  mCpuCsrAccess->WriteMcCpuCsr (Socket, Mc, SCRUBADDRESSLO_MC_MAIN_REG, mPatrolScrubStatus[Node].ScrubAddrLoReg.Data);
  mCpuCsrAccess->WriteMcCpuCsr (Socket, Mc, SCRUBADDRESSHI_MC_MAIN_REG, mPatrolScrubStatus[Node].ScrubAddrHiReg.Data);
  mCpuCsrAccess->WriteMcCpuCsr (Socket, Mc, SCRUBADDRESS2LO_MC_MAIN_REG, (UINT32)mPatrolScrubStatus[Node].ScrubAddrLo2Reg.Data);
  mCpuCsrAccess->WriteMcCpuCsr (Socket, Mc, SCRUBADDRESS2HI_MC_MAIN_REG, (UINT32)mPatrolScrubStatus[Node].ScrubAddrHi2Reg.Data);

  mCpuCsrAccess->WriteMcCpuCsr (Socket, Mc, SCRUBCTL_MC_MAIN_REG, mPatrolScrubStatus[Node].ScrubCtlReg.Data);

  return EFI_SUCCESS;
}

/**
  @brief Returns channel interleave ways for given socket and SAD index.

  @param[in] Socket   - socket number
  @param[in] SadInx   - SAD index

  @return Ways of channel interleave for socket/sad
**/
UINT8
GetChInterleave (
  IN UINT8 Socket,
  IN UINT8 SadInx
  )
{
  UINT8       ChInterleave;
  UINT8       Bitmap;

  if (Socket >= MAX_SOCKET || SadInx >= MAX_SAD_RULES) {
    DEBUG((DEBUG_ERROR, "GetChInterleave() returns 0. Socket is %d, SadInx is %d. \n", Socket, SadInx));
    return 0;
  }
  Bitmap = GetChInterBitmap(&mSystemMemoryMap->Socket[Socket].SAD[SadInx]);

  for (ChInterleave = 0; Bitmap != 0; Bitmap >>= 1) {

    if (Bitmap & 1) {

      ChInterleave++;
    }
  }
  return ChInterleave;
}


/**
  @brief Get the channel ways number for given SAD index and socket.

  @param[in] Socket   - socket number (0 based)
  @param[in] SadIndex - SAD index

  @return Channel ways
*/
UINT8
GetChWays (
  IN UINT8  Socket,
  IN UINT8  SadIndex
  )
{
  UINT8     ChWays;
  UINT8     McIndex;
  UINT8     Bitmap;

  //
  // Determine which IMC channel bitmap to use
  //
  McIndex = GetMcIndexFromBitmap (mSystemMemoryMap->Socket[Socket].SAD[SadIndex].imcInterBitmap);
  ChWays = 0;
  for (Bitmap = mSystemMemoryMap->Socket[Socket].SAD[SadIndex].channelInterBitmap[McIndex]; Bitmap != 0; Bitmap >>= 1) {

    if (Bitmap & 1) {
      ChWays++;
    }
  }
  return ChWays;
}

/**

Routine Description:AddNvDimmToPMem adds a found NVDIMM
to the global mPmemRegions structure

  @param PmemIndex   - Index to PMEM region
  @param NvDimmIndex - Index to NVDIMM
  @param XlatedAddr  - NVDIMM to add
  @retval Status     - Status

**/
EFI_STATUS
AddNvDimmToPMem (
   UINTN                PmemIndex,
   UINTN                NvDimmIndex,
   TRANSLATED_ADDRESS   XlatedAddr
   )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  JEDEC_NVDIMM      *pNvDimm;

  mPmemRegions[PmemIndex].NumNvDimms++;
  mPmemRegions[PmemIndex].NvDimms[NvDimmIndex].ImcId = XlatedAddr.MemoryControllerId;
  mPmemRegions[PmemIndex].NvDimms[NvDimmIndex].SocketId = XlatedAddr.SocketId;
  mPmemRegions[PmemIndex].NvDimms[NvDimmIndex].Dimm = XlatedAddr.DimmSlot;
  mPmemRegions[PmemIndex].NvDimms[NvDimmIndex].Ch = XlatedAddr.ChannelId;
  mPmemRegions[PmemIndex].NvDimms[NvDimmIndex].SpaBase = XlatedAddr.SystemAddress;
  mPmemRegions[PmemIndex].NvDimms[NvDimmIndex].EnergyType = mSystemMemoryMap->Socket[XlatedAddr.SocketId].ChannelInfo[MEM_IMCCH_TO_SKTCH(XlatedAddr.MemoryControllerId, XlatedAddr.ChannelId)].DimmInfo[XlatedAddr.DimmSlot].EnergyType;
  mPmemRegions[PmemIndex].NvDimms[NvDimmIndex].Size = (UINT64)mSystemMemoryMap->Socket[XlatedAddr.SocketId].ChannelInfo[MEM_IMCCH_TO_SKTCH(XlatedAddr.MemoryControllerId, XlatedAddr.ChannelId)].DimmInfo[XlatedAddr.DimmSlot].NVmemSize  * (64 * 1024 * 1024);
  pNvDimm = &mPmemRegions[PmemIndex].NvDimms[NvDimmIndex];
  DEBUG ((EFI_D_ERROR, "NVDIMM PmemIndex = %d\n",PmemIndex));
  DEBUG ((EFI_D_ERROR, "NVDIMM NvDimmIndex = %d\n", NvDimmIndex));
  DEBUG ((EFI_D_ERROR, "NVDIMM Socket = %d\n", mPmemRegions[PmemIndex].NvDimms[NvDimmIndex].SocketId));
  DEBUG ((EFI_D_ERROR, "NVDIMM ImcId = %d\n", mPmemRegions[PmemIndex].NvDimms[NvDimmIndex].ImcId));
  DEBUG ((EFI_D_ERROR, "NVDIMM Channel = %d\n", mPmemRegions[PmemIndex].NvDimms[NvDimmIndex].Ch));
  DEBUG ((EFI_D_ERROR, "NVDIMM DIMM # = %d\n", mPmemRegions[PmemIndex].NvDimms[NvDimmIndex].Dimm));
  DEBUG ((EFI_D_ERROR, "NVDIMM Base Address = 0x%lX\n", mPmemRegions[PmemIndex].NvDimms[NvDimmIndex].SpaBase));
  DEBUG ((EFI_D_ERROR, "NVDIMM EnergyType = 0x%lX\n", mPmemRegions[PmemIndex].NvDimms[NvDimmIndex].EnergyType));
  DEBUG ((EFI_D_ERROR, "NVDIMM size = 0x%lX\n", mPmemRegions[PmemIndex].NvDimms[NvDimmIndex].Size));

  // Init DSM attributes
  DEBUG ((EFI_D_ERROR, "NVDIMM Get Identification\n"));
  GetDimmId(pNvDimm);
  DEBUG ((EFI_D_ERROR, "NVDIMM Get Power Info\n"));
  GetDimmPwrInfo(pNvDimm);
  DEBUG ((EFI_D_ERROR, "NVDIMM Get Energy Storage Info\n"));
  GetDimmESInfo(pNvDimm);
  DEBUG ((EFI_D_ERROR, "NVDIMM Get Operation Statistics Info\n"));
  GetDimmOpStatistics(pNvDimm);
  DEBUG ((EFI_D_ERROR, "NVDIMM Get Module Info\n"));
  GetDimmIModuleInfo(pNvDimm);
  IsJedecNvdimmPresent = TRUE;

  return Status;
}

/**

Routine Description: FillPmemRegions searches the SAD table to find entries
that are contained with persistent memory regions in the memory map.  The global
variable mPmemRegions contains all PMEM regions and JEDEC NVDIMMs found.

  @param  VOID         - None
  @retval EFI_STATUS   - EFI_SUCCESS if found at least one NVDIMM

**/
EFI_STATUS
FillPmemRegions (
   VOID
   )
{
  EFI_STATUS            Status;
  struct SADTable       *pSAD;
  UINT8                 TmpMemoryMap[1];
  UINTN                 MapKey;
  UINTN                 DescriptorSize;
  UINT32                DescriptorVersion;
  UINTN                 MemoryMapSize;
  EFI_MEMORY_DESCRIPTOR *MemoryMap;
  EFI_MEMORY_DESCRIPTOR *MemoryMapPtr;
  UINTN                 Index;
  UINTN                 PmemIndex = 0;
  UINTN                 NvDimmIndex;
  UINT64                SadBaseAddress;
  UINT64                SadLimit;
  UINT64                NvDimmAddress;
  UINT8                 Socket;
  UINT8                 SadNum;
  UINT64                CurrentBase;
  UINT64                PmemLimit;
  UINT8                 ChInterBitmap;
  UINT8                 i;
  TRANSLATED_ADDRESS    XlatedAddr;
  UINT64                NvDimmSize;
  UINT64                NvDimmOffset;
  UINT8                 NvDimmsInSad;

  DEBUG((EFI_D_ERROR, "FillPmemRegions start\n"));
  //
  // Get System MemoryMapSize
  //
  MemoryMapSize = 1;
  Status = gBS->GetMemoryMap (
                  &MemoryMapSize,
                  (EFI_MEMORY_DESCRIPTOR *)TmpMemoryMap,
                  &MapKey,
                  &DescriptorSize,
                  &DescriptorVersion
                  );
  ASSERT (Status == EFI_BUFFER_TOO_SMALL);
  //
  // Enlarge space here, because we will allocate pool now.
  //
  MemoryMapSize += EFI_PAGE_SIZE;
  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  MemoryMapSize,
                  (VOID**)&MemoryMap
                  );
  ASSERT_EFI_ERROR (Status);
  //
  // Get System MemoryMap
  //
  Status = gBS->GetMemoryMap (
                  &MemoryMapSize,
                  MemoryMap,
                  &MapKey,
                  &DescriptorSize,
                  &DescriptorVersion
                  );
  ASSERT_EFI_ERROR (Status);

  MemoryMapPtr = MemoryMap;

  Status = EFI_NOT_FOUND;

  //
  // Search for the PMEM regions
  //
  for (Index = 0; Index < (MemoryMapSize / DescriptorSize); Index++) {
    switch (MemoryMap->Type){
      case EfiPersistentMemory:

        DEBUG((EFI_D_ERROR, "EfiPersistentMemory region found at %lx\n", MemoryMap->PhysicalStart));
        // Scan SAD Table for corresponding memory region
        SadBaseAddress = 0;
        CurrentBase = MemoryMap->PhysicalStart;
        PmemLimit = MemoryMap->PhysicalStart + (MemoryMap->NumberOfPages <<  EFI_PAGE_SHIFT);
        for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
          for (SadNum = 0; SadNum < MAX_SAD_RULES; SadNum++) {
            if (mSystemMemoryMap->Socket[Socket].SAD[SadNum].Enable == 0) {
              continue;
            }
            if (mSystemMemoryMap->Socket[Socket].SAD[SadNum].local == 0) {
              continue;
            }
            SadLimit = (UINT64)mSystemMemoryMap->Socket[Socket].SAD[SadNum].Limit << 26;
            ChInterBitmap = GetChInterBitmap(&mSystemMemoryMap->Socket[Socket].SAD[SadNum]);

            if ((SadBaseAddress == CurrentBase) && (SadBaseAddress != SadLimit)) {
              if (PmemIndex >= MAX_PMEM_REGIONS) {
                DEBUG ((EFI_D_ERROR, "WARNING: PMEM region number exceeds max (%d)...skipping.\n", MAX_PMEM_REGIONS));
                break;
              }
              pSAD = &mSystemMemoryMap->Socket[Socket].SAD[SadNum];
              mPmemRegions[PmemIndex].BaseAddress = CurrentBase;
              mPmemRegions[PmemIndex].Size = SadLimit - SadBaseAddress;
              mPmemRegions[PmemIndex].SadIndex = SadNum;
              mPmemRegions[PmemIndex].SocketNum = Socket;
              mPmemRegions[PmemIndex].NumNvDimms = 0;
              NvDimmIndex = 0;

              DEBUG((EFI_D_ERROR, "SAD[%x] matches\n", SadNum));
              DEBUG((EFI_D_ERROR, "SAD[%x] base = %lx\n", SadNum, SadBaseAddress));
              DEBUG((EFI_D_ERROR, "SAD[%x] limit = %lx\n", SadNum, SadLimit));
              DEBUG((EFI_D_ERROR, "SAD[%x] type = %x\n", SadNum, pSAD->type));
              DEBUG((EFI_D_ERROR, "SAD[%x] ChInterBitmap = %x\n", SadNum, ChInterBitmap));
              DEBUG((EFI_D_ERROR, "SAD[%x] imcInterBitmap = %x\n", SadNum, pSAD->imcInterBitmap));

              NvDimmAddress = SadBaseAddress;

              // Determine number of NVDIMMs in SAD by getting size of first one
              ZeroMem (&XlatedAddr, sizeof(TRANSLATED_ADDRESS));
              Status = SystemAddressToDimmAddress (NvDimmAddress, &XlatedAddr);
              if (EFI_ERROR(Status)) {
                ASSERT_EFI_ERROR (Status);
                return Status;
              }

              NvDimmSize = (UINT64)mSystemMemoryMap->Socket[XlatedAddr.SocketId].ChannelInfo[MEM_IMCCH_TO_SKTCH(XlatedAddr.MemoryControllerId, XlatedAddr.ChannelId)].DimmInfo[XlatedAddr.DimmSlot].NVmemSize  * (64 * 1024 * 1024);
              if (NvDimmSize == 0) {
                DEBUG((EFI_D_ERROR, "NvDimmSize is 0 not a real Nvdimm go to next DIMM\n"));
                continue;
              }
              NvDimmsInSad = (UINT8)((SadLimit - SadBaseAddress) / NvDimmSize);
              if (((SadLimit - SadBaseAddress) % NvDimmSize) != 0) {
                NvDimmsInSad++;
              }
              DEBUG((EFI_D_ERROR, "Total NVDIMMS in SAD  = %d\n", NvDimmsInSad));

              // If interleaving is disabled then the next NVDIMM is located after NvDimmSize,
              // otherwise the next NVDIMM in interleave set is at offset of SAD granularity
              NvDimmOffset = NvDimmSize;
              if (GetImcWays(Socket, SadNum) != 1) NvDimmOffset = GetSocketGranularity(pSAD);
              if (GetChWays(Socket, SadNum) != 1) NvDimmOffset = GetChannelGranularity(pSAD);

              // Find all NVDIMMs in SAD
              for (i = 0; i < NvDimmsInSad; i++) {
                ZeroMem (&XlatedAddr, sizeof(TRANSLATED_ADDRESS));
                Status = SystemAddressToDimmAddress (NvDimmAddress, &XlatedAddr);
                if (EFI_ERROR(Status)) {
                  ASSERT_EFI_ERROR (Status);
                  return Status;
                }

                Status = AddNvDimmToPMem (PmemIndex, NvDimmIndex, XlatedAddr);
                NvDimmAddress += NvDimmOffset;

                // This handles the case when interleaving is disabled and 2 NVDIMMs are
                // contained within 1 SAD entry.  If interleaving is disabled and 2 NVDIMMs
                // on one channel then create 2 PMEM regions each with 1 NVDIMM.
                if ((NvDimmsInSad > 1) && (NvDimmOffset == NvDimmSize)) {
                  // Update last PMEM region size to match size of NVDIMM
                  mPmemRegions[PmemIndex].Size = NvDimmSize;
                  DEBUG((EFI_D_ERROR, "PMEM region contains %d NVDIMMs\n", mPmemRegions[PmemIndex].NumNvDimms));

                  // Add new PMEM region
                  PmemIndex++;
                  mPmemRegions[PmemIndex].BaseAddress = NvDimmAddress;
                  mPmemRegions[PmemIndex].Size = NvDimmSize;
                  mPmemRegions[PmemIndex].SadIndex = SadNum;
                  mPmemRegions[PmemIndex].SocketNum = Socket;
                  mPmemRegions[PmemIndex].NumNvDimms = 0;
                  NvDimmIndex = 0;  // Reset to 0 for new PMEM region
                  i++;              // Increment index for this NVDIMM

                  // Add NVDIMM to new PMEM region
                  ZeroMem (&XlatedAddr, sizeof(TRANSLATED_ADDRESS));
                  Status = SystemAddressToDimmAddress (NvDimmAddress, &XlatedAddr);
                  if (EFI_ERROR(Status)) {
                    ASSERT_EFI_ERROR (Status);
                    return Status;
                  }
                  Status = AddNvDimmToPMem(PmemIndex, NvDimmIndex, XlatedAddr);
                  NvDimmAddress += NvDimmOffset;
                }
                else {
                  NvDimmIndex++;
                }
              }

              // PMEM region may contain multiple SAD regions
              if (PmemLimit > SadLimit) {
                CurrentBase = SadLimit;
              }
              DEBUG((EFI_D_ERROR, "PMEM region contains %d NVDIMMs\n", mPmemRegions[PmemIndex].NumNvDimms));
              PmemIndex++;
            }
            SadBaseAddress = SadLimit;
          }
        }
        break;
      default:
        break;
    }
    MemoryMap = (EFI_MEMORY_DESCRIPTOR *)((UINTN)MemoryMap + DescriptorSize);
  }

  gBS->FreePool (MemoryMapPtr);

  return Status;
}


/**

Routine Description: GetChInterBitmap - This function gets
channelInterBitmap or FMchannelInterBitmap depeding on the type.

  @param pSAD       - Pointer to the SAD
  @retval ChInter   - Return channel interleave bitmap

**/
UINT8
GetChInterBitmap(
   struct SADTable  *pSAD
   )
{
  UINT8 ChInterBitmap = 0;
  UINT8 McIndex;

  McIndex = GetMcIndexFromBitmap (pSAD->imcInterBitmap);

  if (IsMemType1lm(pSAD->type)) {
    ChInterBitmap = pSAD->channelInterBitmap[McIndex];
  } else if (IsMemType2lm(pSAD->type) ||
             IsMemTypeAppDirect(pSAD->type) ||
             pSAD->type == MemType2lmDdrWbCacheAppDirect ||
             pSAD->type == MemType1lmCtrl) {
    ChInterBitmap = pSAD->FMchannelInterBitmap[McIndex];
  } else {
    DEBUG ((EFI_D_INFO, "GetChInterBitmap: Unknown case.\n"));
  }

  return ChInterBitmap;
}


/**

Routine Description: CreateDimmPresentBitmaps - This function
basically creates 2 64-bit fields that are part of the
AcpiSMMInterface. The first 64-bit field is for socket 0-3 and
the second 64-bit field is for socket 3-7. This field contains
bit that are set in the corresponding position when an
NVMDIMM is present in that position. Least significant 16 bits
(of the 1st field) represents socket0 and the most signifFicant
16-bit is for socket3 and the other sockets in between. For
example, if the system has 2 sockets and there is an NVMDIMM in
Dimm0 of all channels, then the bitmap would look like below:
0000000000000000000000000000000000000101010101010000010101010101b

  @param VOID         - None
  @retval VOID        - None

**/
VOID
CreateDimmPresentBitmaps(
   VOID
   )
{
  UINT64       DimmBitmap1;
  UINT64       DimmBitmap2;
  UINT8        Socket, Ch, Dimm;

  DimmBitmap1 = DimmBitmap2 = 0;
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Ch = 0; Ch < MAX_CH; Ch++) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((IsNvDimm(Socket, Ch, Dimm)) && (mSystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].Enabled)) {
          if (Socket < 4) {
            DimmBitmap1 |= ((UINT64)0x1 << ((Socket * 16) + (Ch * 2) + Dimm));
            mNvdimmAcpiSmmInterface->EnergySourcePolicy0 = (mSystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].EnergyType == 1) ? 0 : 1;
          }
          else {
            DimmBitmap2 |= ((UINT64)0x1 << ((Socket * 16) + (Ch * 2) + Dimm - 64));
            mNvdimmAcpiSmmInterface->EnergySourcePolicy1 = (mSystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].EnergyType == 1) ? 0 : 1;
          }
        }
      }
    }
  }
  mNvdimmAcpiSmmInterface->NvDimmConfigBitmap0 = DimmBitmap1;
  mNvdimmAcpiSmmInterface->NvDimmConfigBitmap1 = DimmBitmap2;
}

/**

Routine Description: GetDimmInfoFromDevHandle - This function
deconstructs Socket, Channel and Dimm from the Device Handle
that is passed in and returns them in their respective pointers
that are passed in.

  @param *Socket      - pointer to Socket that this function
         will return
  @param *Mc          - pointer to channel that this function
         will return
  @param *Ch          - pointer to channel that this function
         will return
  @param *Dimm        - Pointer to Dimm that this function will
         return
  @param DevHandle    - 32-bit device handle.
  @retval None - VOID

**/
VOID
GetDimmInfoFromDevHandle (
   UINT8       *Socket,
   UINT8       *Mc,
   UINT8       *Ch,
   UINT8       *Dimm,
   UINT32      DevHandle
   )
{
  // Bit 15:12 of DevHandle contains the socket
  *Socket = (UINT8)((DevHandle >> 12) & 0x0000000F);
  // Bit 11:8 contains the memory controller ID in Nfit DeviceHandle
  *Mc = (UINT8)((DevHandle >> 8) & 0x0000000F);
  // Bit 7:4 contains the channel number
  *Ch = (UINT8)((DevHandle >> 4) & 0x0000000F);
  // Bit 3:0 contains the dimm number
  *Dimm = (UINT8)(DevHandle & 0x0000000F);
}

/**

Routine Description: GetPMEMIntDimmNum - This helper function
returns Dimm Number in the Interleave set of the SAD Rule for a
PMEM Region.

  @param SADBaseAddr - SAD Rule Base Address
  @param Socket    - Socket
  @param Ch        - Channel number
  @retval Interleave Dimm Number

**/
UINT8
GetPMEMIntDimmNum(
   UINT64      SADBaseAddr,
   UINT8       Socket,
   UINT8       Ch
   )
{
  UINT8            i, j;
  PMEM_INFO        *pPMEM = NULL;

  for (i = 0; (i < NvdimmInfo.NumPMEMRgns) && (i < ARRAY_SIZE (NvdimmInfo.PMEMInfo)); i++) {
    pPMEM =  &NvdimmInfo.PMEMInfo[i];
    j = i;
    if (pPMEM->SADPMemBase == SADBaseAddr) {
      while (pPMEM->SADPMemBase == SADBaseAddr) {
        if ((pPMEM->Socket == Socket) && (pPMEM->Channel == Ch)) {
          return (pPMEM->IntDimmNum);
        }
        j++;
        pPMEM = &NvdimmInfo.PMEMInfo[j];
      }
    }
  }
  return (0);
}

/**

Routine Description: GetSADLimit - This helper function returns
SAD Rule Limit based on the SAD Rule base address passed in.

  @param SADBaseAddr  - Spa Start Address of the SAD Rule
  @param *Limit       - Limit will contain the SAD Rule Limit
  @retval VOID

**/
VOID
GetSADLimit(
   UINT64       SADBaseAddr,
   UINT64       *Limit
   )
{
  UINT8            Socket;
  UINT8            PmemIndex;
  UINT8            i;
  struct SADTable  *pSAD;
  UINT64           SpaStart, SpaEnd;

  //
  // Traverse thru all SAD entries to check for the Spa being in the range of one
  for (PmemIndex = 0; PmemIndex < MAX_PMEM_REGIONS; PmemIndex++) {
    if (mPmemRegions[PmemIndex].Size == 0) {
      continue;
    }
    Socket = mPmemRegions[PmemIndex].SocketNum;
    i = mPmemRegions[PmemIndex].SadIndex;
    pSAD = &mSystemMemoryMap->Socket[Socket].SAD[i];
    if (pSAD->Enable == 0) {
      continue;
    }
    if (pSAD->local == 0) {
      continue;
    }
    if (i == 0) {
      SpaStart = 0;
    } else {
      SpaStart = ((UINT64)mSystemMemoryMap->Socket[Socket].SAD[i - 1].Limit << BITS_64MB_TO_BYTES);
    }
    SpaEnd = ((UINT64)mSystemMemoryMap->Socket[Socket].SAD[i].Limit << BITS_64MB_TO_BYTES);
    if (SADBaseAddr == SpaStart) {
      *Limit = SpaEnd;
    }
  }
}

/**

Routine Description: GetDpaFor1WayChInt - This function converts
given Spa to Dpa in the case of the Dimm(s) involved in 1-way
Channel interleaved for up to 8 Imc Ways.

  @param pDimm       - Pointer to Dimm structure
  @param Spa         - Spa for which we need the DPA
  @param SADSpaStart - Start Address for SAD Rule interleave set
  @param *pDpa       - Pointer to the Dpa that this function
                       returns.
  @retval EFI_STATUS - status of the conversion effort

**/
EFI_STATUS
GetDpaFor1WayChInt(
   NVDIMM         *pDimm,
   UINT64          Spa,
   UINT64          SADSpaStart,
   UINT64          *pDpa
   )
{
  EFI_STATUS       Status = EFI_SUCCESS;
  UINT64           RotationSize, NumRotation, SpaStart;
  UINT32           SpaIndex, LineNum;
  UINT32           NumLinesDesc;
  UINT32           LineIndexSize = 0;
  UINT32           LineSize, ImcSize;
  INT32            Index;
  UINT8            i, ImcWays;

  if (SADSpaStart == pDimm->SADSpaBase) {
    // It is Control Region for translation
    ImcWays = pDimm->iMCWays;
    SpaStart = pDimm->SADSpaBase;
    i = pDimm->IntDimmNum;
  }
  else {
    Index = GetPmemIndex(&NvdimmInfo, SADSpaStart);
    if (Index == -1) {
      return (EFI_INVALID_PARAMETER);
    }
    else {
      // 1st PMEM Region of this dimm
      ImcWays = NvdimmInfo.PMEMInfo[Index].iMCWays;
      SpaStart = SADSpaStart;
      i = NvdimmInfo.PMEMInfo[Index].IntDimmNum;
    }
  }

  // Check if start of the NVDIMM
  if ((Spa - SpaStart) == 0) {
    *pDpa = 0;
    return Status;
  }

  ImcSize = NvdimmInfo.SktGran;
  LineSize = GetChGranularityFromSadBaseAddr(SADSpaStart);
  if (LineSize == 0) {
    return (EFI_INVALID_PARAMETER);
  }

  if(ImcSize == LineSize) {
    NumLinesDesc = 2;
  } else {
    NumLinesDesc = ImcSize / LineSize;
  }

  RotationSize = LineSize * NumLinesDesc;
  NumRotation = (UINT32)((Spa - SpaStart) / (RotationSize * ImcWays));
  SpaIndex = (UINT32)((Spa - SpaStart) % (Spa - SpaStart));
  //
  // Need the Line Number and for that need to locate SpaIndex in the table
  LineIndexSize = ImcSize * i;
  for (LineNum = 0; LineNum <  NumLinesDesc; LineNum++) {
    if (SpaIndex == LineIndexSize) {
      break;
    }
    LineIndexSize += LineSize;
  }

  // Now we have everything to compute the Dpa
  *pDpa = (LineNum * LineSize) + (NumRotation * RotationSize) + (Spa % LineSize);

  return (Status);
}

/**

Routine Description: GetDpaFor2WayChInt - This function converts
given Spa to Dpa in the case of the Dimm(s) involved in 2-way
Channel interleaved for up to 8 Imc Ways.

  @param pDimm       - Pointer to Dimm structure
  @param Spa         - Spa for which we need the DPA
  @param SADSpaStart - Start Address for SAD Rule interleave set
  @param *pDpa       - Pointer to the Dpa that this function
                       returns.
  @retval EFI_STATUS - status of the conversion effort

**/
EFI_STATUS
GetDpaFor2WayChInt(
   NVDIMM          *pDimm,
   UINT64          Spa,
   UINT64          SADSpaStart,
   UINT64          *pDpa
   )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT64      RotationSize;
  UINT64      NumRotation;
  UINT64      SpaStart;
  UINT32      SpaIndex;
  UINT32      LineNum = 0;
  UINT32      NumLinesDesc;
  UINT32      LineIndexSize = 0;
  UINT32      LineSize;
  UINT32      ImcSize;
  UINT32      *LineMultiplier;
  INT32       Index;
  UINT8       i;
  UINT8       ImcWays;

  if (SADSpaStart == pDimm->SADSpaBase) {
    // It is Control Region for translation
    ImcWays = pDimm->iMCWays;
    SpaStart = pDimm->SADSpaBase;
    i = pDimm->IntDimmNum;
  }
  else {
    Index = GetPmemIndex(&NvdimmInfo, SADSpaStart);
    if (Index == -1) {
      return (EFI_INVALID_PARAMETER);
    }
    else {
      // 1st PMEM Region of this dimm
      ImcWays = NvdimmInfo.PMEMInfo[Index].iMCWays;
      SpaStart = SADSpaStart;
      i = NvdimmInfo.PMEMInfo[Index].IntDimmNum;
    }
  }

  // Check if start of the NVDIMM
  if ((Spa - SpaStart) == 0) {
    *pDpa = 0;
    return Status;
  }

  ImcSize = NvdimmInfo.SktGran;
  LineSize = GetChGranularityFromSadBaseAddr(SADSpaStart);
  if (LineSize == 0) {
    return (EFI_INVALID_PARAMETER);
  }

  if(ImcSize == LineSize) {
    NumLinesDesc = 2;
  } else {
    NumLinesDesc = ImcSize / (LineSize * CH_2WAY);
  }

  RotationSize = LineSize * NumLinesDesc;
  NumRotation = (UINT32)((Spa - SpaStart) / (RotationSize * ImcWays * CH_2WAY));
  SpaIndex = (UINT32)((Spa - SpaStart) % (RotationSize * ImcWays * CH_2WAY));

  switch (ImcWays) {
  case ONE_IMC_WAY:
    if(ImcSize == LineSize) {
      LineMultiplier = &Imc1Way4k_2ChWay4k_LineMultipliers[i][0];
    } else {
      LineMultiplier = &Imc1Way4k_2ChWay256_LineMultipliers[i][0];
    }
    break;
  case TWO_IMC_WAY:
    if(ImcSize == LineSize) {
      LineMultiplier = &Imc2Way4k_2ChWay4k_LineMultipliers[i][0];
    } else {
      LineMultiplier = &Imc2Way4k_2ChWay256_LineMultipliers[i][0];
    }
    break;
  case FOUR_IMC_WAY:
    if(ImcSize == LineSize) {
      LineMultiplier = &Imc4Way4k_2ChWay4k_LineMultipliers[i][0];
    } else {
      LineMultiplier = &Imc4Way4k_2ChWay256_LineMultipliers[i][0];
    }
    break;
  case EIGHT_IMC_WAY:
    // 8 Imc Way not supported for 4k/4k
    LineMultiplier = &Imc8Way4k_2ChWay256_LineMultipliers[i][0];
    break;
  default:
    DEBUG ((EFI_D_ERROR, "\n*** ERROR. Invalid 'ImcSize' value, current value is 0x%X ***\n", ImcSize));
    return EFI_UNSUPPORTED;
  }

  LineIndexSize = *LineMultiplier * LineSize;
  for (LineNum = 0; LineNum <  NumLinesDesc; LineNum++) {
    if (SpaIndex == LineIndexSize) {
      break;
    }
    LineIndexSize += (LineMultiplier[LineNum] * LineSize);
  }

    // Now we have everything to compute the Dpa
  *pDpa = (LineNum * LineSize) + (NumRotation * RotationSize) + (Spa % LineSize);

  return (Status);
}

/**

Routine Description: GetDpaFor3WayChInt - This function converts
given Spa to Dpa in the cases of the Dimms involved in 3-way
Channel interleaved for up to 8 Imc Ways.

  @param pDimm       - Pointer to Dimm structure
  @param Spa         - Spa for which we need the DPA
  @param SADSpaStart - Start Address for SAD Rule interleave set
  @param *pDpa       - Pointer to the Dpa that this function
                       returns.
  @retval EFI_STATUS - status of the conversion effort

**/
EFI_STATUS
GetDpaFor3WayChInt(
   NVDIMM          *pDimm,
   UINT64          Spa,
   UINT64          SADSpaStart,
   UINT64          *pDpa
   )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT64      RotationSize;
  UINT64      NumRotation;
  UINT64      SpaStart;
  UINT32      SpaIndex;
  UINT32      LineNum = 0;
  UINT32      NumLinesDesc;
  UINT32      LineIndexSize = 0;
  UINT32      LineSize;
  UINT32      ImcSize;
  UINT32      *LineMultiplier;
  INT32       Index;
  UINT8       i;
  UINT8       ImcWays;

  if (SADSpaStart == pDimm->SADSpaBase) {
    // It is Control Region for translation
    ImcWays = pDimm->iMCWays;
    SpaStart = pDimm->SADSpaBase;
    i = pDimm->IntDimmNum;
  }
  else {
    Index = GetPmemIndex(&NvdimmInfo, SADSpaStart);
    if (Index == -1) {
      return (EFI_INVALID_PARAMETER);
    }
    else {
      // 1st PMEM Region of this dimm
      ImcWays = NvdimmInfo.PMEMInfo[Index].iMCWays;
      SpaStart = SADSpaStart;
      i = NvdimmInfo.PMEMInfo[Index].IntDimmNum;
    }
  }

  // Check if start of the NVDIMM
  if ((Spa - SpaStart) == 0) {
    *pDpa = 0;
    return Status;
  }

  ImcSize = NvdimmInfo.SktGran;
  LineSize = GetChGranularityFromSadBaseAddr(SADSpaStart);
  if (LineSize == 0) {
    return (EFI_INVALID_PARAMETER);
  }

  if(ImcSize == LineSize) {
    NumLinesDesc = 2;
  } else {
    NumLinesDesc = ImcSize / LineSize;
  }

  RotationSize = LineSize * NumLinesDesc;
  NumRotation = (UINT32)((Spa - SpaStart) / (RotationSize * ImcWays * CH_3WAY));
  SpaIndex = (UINT32)((Spa - SpaStart) % (RotationSize * ImcWays * CH_3WAY));

  switch (ImcWays) {
  case ONE_IMC_WAY:
    if(ImcSize == LineSize) {
      LineMultiplier = &Imc1Way4k_3ChWay4k_LineMultipliers[i][0];
    } else {
      LineMultiplier = &Imc1Way4k_3ChWay256_LineMultipliers[i][0];
    }
    break;
  case TWO_IMC_WAY:
    if(ImcSize == LineSize) {
      LineMultiplier = &Imc2Way4k_3ChWay4k_LineMultipliers[i][0];
    } else {
      LineMultiplier = &Imc2Way4k_3ChWay256_LineMultipliers[i][0];
    }
    break;
  case FOUR_IMC_WAY:
    if(ImcSize == LineSize) {
      LineMultiplier = &Imc4Way4k_3ChWay4k_LineMultipliers[i][0];
    } else {
      LineMultiplier = &Imc4Way4k_3ChWay256_LineMultipliers[i][0];
    }
    break;
  case EIGHT_IMC_WAY:
    if(ImcSize == LineSize) {
      LineMultiplier = &Imc8Way4k_3ChWay4k_LineMultipliers[i][0];
    } else {
      LineMultiplier = &Imc8Way4k_3ChWay256_LineMultipliers[i][0];
    }
    break;
  default:
    DEBUG ((EFI_D_ERROR, "\n*** ERROR. Invalid 'ImcSize' value, current value is 0x%X ***\n", ImcSize));
    return EFI_UNSUPPORTED;
  }

  LineIndexSize = *LineMultiplier * LineSize;
  for (LineNum = 0; LineNum <  NumLinesDesc; LineNum++) {
    if (SpaIndex == LineIndexSize) {
      break;
    }
    LineIndexSize += (LineMultiplier[LineNum] * LineSize);
  }

  // Now we have everything to compute the Dpa
  *pDpa = (LineNum * LineSize) + (NumRotation * RotationSize) + (Spa % LineSize);

  return (Status);
}

/**

Routine Description: XlateSpaToDpa - This function converts
given Spa to Dpa in the case of the Dimm involved in all Channel
ways and up to 8 Imc Ways.

  @param pDimm       - Pointer to Dimm structure
  @param Spa         - Spa for which we need the DPA
  @param SADSpaStart - SAD Base Address
  @param *pDpa       - Pointer to the Dpa that this function
                       returns.
  @retval EFI_STATUS - status of the conversion effort

**/
EFI_STATUS
XlateSpaToDpa(
   NVDIMM          *pDimm,
   UINT64          Spa,
   UINT64          SADSpaStart,
   UINT64          *pDpa
   )
{
  EFI_STATUS    Status = EFI_NOT_FOUND;
  TRANSLATED_ADDRESS XlatedAddr;

  ZeroMem (&XlatedAddr, sizeof(TRANSLATED_ADDRESS));
  Status = SystemAddressToDimmAddress (Spa, &XlatedAddr);
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  *pDpa = XlatedAddr.DPA;

  return Status;
}

/**
 * Routine Description: SpaToDpa - This function converts given Dpa
to Spa and also populates the socket channel and dimm information with
only system phy address as the input

  @param Spa        - System Physical address SPA to be translated
  @param Skt        - socket number of the given SPA
  @param ch         - channel number of the given SPA
  @param Dimm       - dimm corresponding to the given SPA
  @param pDpa       - Device physical address
  @retval EFI_STATUS - status of the conversion effort
 */
EFI_STATUS
JedecSpaToDpa(
   UINT64       Spa,
   UINT8        *Skt,
   UINT8        *Ch,
   UINT8        *Dimm,
   UINT64       *pDpa
   )
{
  EFI_STATUS    Status = EFI_NOT_FOUND;
  TRANSLATED_ADDRESS XlatedAddr;

  ZeroMem (&XlatedAddr, sizeof(TRANSLATED_ADDRESS));
  Status = SystemAddressToDimmAddress (Spa, &XlatedAddr);
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  *Skt = XlatedAddr.SocketId;
  *Dimm = XlatedAddr.DimmSlot;
  *Ch = MEM_IMCCH_TO_SKTCH (XlatedAddr.MemoryControllerId, XlatedAddr.ChannelId);
  *pDpa = XlatedAddr.DPA;

  return Status;
}

/**

Routine Description: GetSpaFor3WayChInt - This function converts
given Dpa to Spa in the case of 3 way Channel Interleaved and up
to 16 Imc Ways.

  @param pDimm       - Pointer to Dimm structure
  @param SADBaseAddr - SAD Rule Base Address
  @param Dpa         - Dpa for which we need the SPA
  @param *pSpa       - Pointer to the Spa.
  @retval EFI_STATUS - status of the conversion effort

**/
EFI_STATUS
GetSpaFor3WayChInt(
   NVDIMM      *pDimm,
   UINT64      SADBaseAddr,
   UINT64      Dpa,
   UINT64      *pSpa
   )
{
  UINT32           NumLines = 0;
  EFI_STATUS       Status = EFI_SUCCESS;
  UINT64           RotationSize, NumRotation;
  UINT32           NumLinesDesc = 0;
  UINT32           LineIndexSize = 0;
  UINT32           LineSize, ImcSize;
  UINT8            i, ImcWays;
  BOOLEAN          PMEMRgn =  FALSE;

  if (IsPmemRgn(&NvdimmInfo, SADBaseAddr)) {
    LineSize = GetChGranularityFromSadBaseAddr(SADBaseAddr);
    // If PMEM Region, need to get the Interleave Dimm Number from the PMEMInfo struct
    i = NvdimmInfo.PMEMInfo[mPmemIndex].IntDimmNum;
    PMEMRgn = TRUE;
  }
  else {
    i = pDimm->IntDimmNum;
    LineSize = NvdimmInfo.ChGran;
  }
  if (LineSize == 0) {
    return EFI_INVALID_PARAMETER;
  }
  ImcSize = NvdimmInfo.SktGran;
  ImcWays = pDimm->iMCWays;
  if (ImcSize == LineSize) {
    NumLinesDesc = 2;
  } else {
    NumLinesDesc = ImcSize / LineSize;
  }

#if VERBOSE_DEBUG
  DEBUG((EFI_D_ERROR, "JNP: LineSize = %d, ImcSize = %d, ImcWays = %d\n", LineSize, ImcSize, ImcWays));
  DEBUG((EFI_D_ERROR, "JNP: DPA = 0x%llx\n", Dpa));
#endif

  RotationSize = LineSize * NumLinesDesc;
  NumRotation = Dpa / RotationSize;
  NumLines = (UINT32)((Dpa % RotationSize) / LineSize);

  switch (ImcWays) {
  case ONE_IMC_WAY:
    // For 1 ImcWay Interleave Interleave Dimm Number should be < 3.
    ASSERT(i < (ONE_IMC_WAY * MAX_MC_CH));
    if (ImcSize == LineSize) {
      LineIndexSize = (Imc1Way4k_3ChWay4k_LineMultipliers[i][NumLines] * LineSize);
    } else {
      LineIndexSize = (Imc1Way4k_3ChWay256_LineMultipliers[i][NumLines] * LineSize);
    }
    break;
  case TWO_IMC_WAY:
    // For 2 ImcWay Interleave Interleave Dimm Number should be < 6.
    ASSERT(i < (TWO_IMC_WAY * MAX_MC_CH));
    if (ImcSize == LineSize) {
      LineIndexSize = (Imc2Way4k_3ChWay4k_LineMultipliers[i][NumLines] * LineSize);
    } else {
      LineIndexSize = (Imc2Way4k_3ChWay256_LineMultipliers[i][NumLines] * LineSize);
    }
    break;
  case FOUR_IMC_WAY:
    // For 4 ImcWay Interleave Interleave Dimm Number should be < 12.
    ASSERT(i < (FOUR_IMC_WAY * MAX_MC_CH));
    if (ImcSize == LineSize) {
      LineIndexSize = (Imc4Way4k_3ChWay4k_LineMultipliers[i][NumLines] * LineSize);
    } else {
      LineIndexSize = (Imc4Way4k_3ChWay256_LineMultipliers[i][NumLines] * LineSize);
    }
    break;
  case EIGHT_IMC_WAY:
    // For  ImcWay Interleave Interleave Dimm Number should be < 24.
    ASSERT(i < (EIGHT_IMC_WAY * MAX_MC_CH));
    if (ImcSize == LineSize) {
      LineIndexSize = (Imc8Way4k_3ChWay4k_LineMultipliers[i][NumLines] * LineSize);
    } else {
      LineIndexSize = (Imc8Way4k_3ChWay256_LineMultipliers[i][NumLines] * LineSize);
    }
    break;
    default:
      DEBUG ((EFI_D_INFO, "default case.\n"));  //Auto added. Please review.
      break;
  }
  if (PMEMRgn) {
    *pSpa = (UINT64)(SADBaseAddr + (NumRotation * RotationSize * ImcWays * CH_3WAY) + LineIndexSize + (Dpa % LineSize));
  }
  else {
    *pSpa = (UINT64)(pDimm->SADSpaBase + (NumRotation * RotationSize * ImcWays * CH_3WAY) + LineIndexSize + (Dpa % LineSize));
  }

  return Status;
}

/**

Routine Description: GetSpaFor1ChWay - This function converts
given Dpa to Spa in the case of 1 way Channel Interleaved ways
and up to 16 Imc Ways.

  @param pDimm       - Pointer to Dimm structure
  @param SADBaseAddr - SAD Rule Base Address
  @param Dpa         - Dpa for which we need the SPA
  @param *pSpa       - Pointer to the Spa.
  @retval EFI_STATUS - status of the conversion effort

**/
EFI_STATUS
GetSpaFor1ChWay(
   NVDIMM      *pDimm,
   UINT64      SADBaseAddr,
   UINT64      Dpa,
   UINT64      *pSpa
   )
{
  UINT32           NumLines = 0; // one based
  EFI_STATUS       Status = EFI_SUCCESS;
  UINT64           RotationSize, NumRotation;
  UINT32           NumLinesDesc = 0;
  UINT32           LineIndexSize = 0;
  UINT32           LineSize, ImcSize;
  UINT8            i, ImcWays;
  BOOLEAN          PMEMRgn =  FALSE;

  if (IsPmemRgn(&NvdimmInfo, SADBaseAddr)) {
    LineSize = GetChGranularityFromSadBaseAddr(SADBaseAddr);
    // If PMEM Region, need to get the Interleave Dimm Number from the PMEMInfo struct
    i = NvdimmInfo.PMEMInfo[mPmemIndex].IntDimmNum;
    PMEMRgn = TRUE;
  }
  else {
    i = pDimm->IntDimmNum;
    LineSize = NvdimmInfo.ChGran;
  }
  if (LineSize == 0) {
    return EFI_INVALID_PARAMETER;
  }
  ImcSize = NvdimmInfo.SktGran;
  ImcWays =  pDimm->iMCWays;
  if (ImcSize == LineSize) {
    NumLinesDesc = 2;
  } else {
    NumLinesDesc = ImcSize / LineSize;
  }

#if VERBOSE_DEBUG
  DEBUG((EFI_D_ERROR, "JNP: LineSize = %d, ImcSize = %d, ImcWays = %d\n", LineSize, ImcSize, ImcWays));
  DEBUG((EFI_D_ERROR, "JNP: DPA = 0x%llx\n", Dpa));
#endif

  switch (ImcWays) {
  case ONE_IMC_WAY:
    // For 1 ImcWay and 1ChWay Interleave Spa is the same as Dpa
    if (PMEMRgn) {
      *pSpa = SADBaseAddr + Dpa;
    }
    else {
      *pSpa = pDimm->SADSpaBase + Dpa;
    }
    break;
  // Intended fall thrus
  case TWO_IMC_WAY:
    ASSERT(i < TWO_IMC_WAY);
  case FOUR_IMC_WAY:
    ASSERT(i < FOUR_IMC_WAY);
  case EIGHT_IMC_WAY:
    ASSERT(i < EIGHT_IMC_WAY);
  case SIXTEEN_IMC_WAY:
    ASSERT(i < SIXTEEN_IMC_WAY);
    RotationSize = LineSize * NumLinesDesc;
    NumRotation = Dpa / RotationSize;
    NumLines = (UINT32)((Dpa % RotationSize) / LineSize);
    LineIndexSize = (i * ImcSize) + (LineSize * NumLines);
    if (PMEMRgn) {
      *pSpa = (UINT64)(SADBaseAddr + (NumRotation * RotationSize * ImcWays) + LineIndexSize + (Dpa % LineSize));
    }
    else {
      *pSpa = (UINT64)(pDimm->SADSpaBase + (NumRotation * RotationSize * ImcWays) + LineIndexSize + (Dpa % LineSize));
    }
    break;
    default:
      DEBUG ((EFI_D_INFO, "default case.\n"));  //Auto added. Please review.
      break;
  }

  return Status;
}

/**

Routine Description: GetSpaFor2ChWays - This function converts
given Dpa to Spa in the case of 2 way Channel Interleaved ways
and up to 16 Imc Ways.

  @param pDimm       - Pointer to Dimm structure
  @param SADBaseAddr - SAD Rule Base Address
  @param Dpa         - Dpa for which we need the SPA
  @param *pSpa       - Pointer to the Spa.
  @retval EFI_STATUS - status of the conversion effort

**/
EFI_STATUS
GetSpaFor2ChWays(
   NVDIMM      *pDimm,
   UINT64      SADBaseAddr,
   UINT64      Dpa,
   UINT64      *pSpa
   )
{
  UINT32           NumLines = 0; // one based
  EFI_STATUS       Status = EFI_SUCCESS;
  UINT64           RotationSize, NumRotation;
  UINT32           NumLinesDesc;
  UINT32           LineIndexSize = 0;
  UINT32           LineSize, ImcSize;
  UINT8            i, ImcWays;
  BOOLEAN          PMEMRgn =  FALSE;

  if (IsPmemRgn(&NvdimmInfo, SADBaseAddr)) {
    LineSize = GetChGranularityFromSadBaseAddr(SADBaseAddr);
    // If PMEM Region, need to get the Interleave Dimm Number from the PMEMInfo struct
    i = NvdimmInfo.PMEMInfo[mPmemIndex].IntDimmNum;
    PMEMRgn = TRUE;
  }
  else {
    i = pDimm->IntDimmNum;
    LineSize = NvdimmInfo.ChGran;
  }
  if (LineSize == 0) {
    return EFI_INVALID_PARAMETER;
  }
  ASSERT(LineSize != 0);
  ImcSize = NvdimmInfo.SktGran;
  ImcWays =  pDimm->iMCWays;

  if (ImcSize == LineSize) {
    NumLinesDesc = 2;
  } else {
    NumLinesDesc = ImcSize / (LineSize * CH_2WAY);
  }

#if VERBOSE_DEBUG
  DEBUG((EFI_D_ERROR, "JNP: LineSize = %d, ImcSize = %d, ImcWays = %d\n", LineSize, ImcSize, ImcWays));
  DEBUG((EFI_D_ERROR, "JNP: DPA = 0x%llx\n", Dpa));
#endif

  RotationSize = LineSize * NumLinesDesc;
  NumRotation = Dpa / RotationSize;
  NumLines = (UINT32)((Dpa % RotationSize) / LineSize);

  switch (ImcWays) {
  case ONE_IMC_WAY:
    // For 1 ImcWay Interleave Interleave Dimm Number should be < 2.
    ASSERT(i < (ONE_IMC_WAY * CH_2WAY));
    if (ImcSize == LineSize) {
      LineIndexSize = (Imc1Way4k_2ChWay4k_LineMultipliers[i][NumLines] * LineSize);
    } else {
      LineIndexSize = (Imc1Way4k_2ChWay256_LineMultipliers[i][NumLines] * LineSize);
    }
    break;
  case TWO_IMC_WAY:
    // For 2 ImcWay Interleave Interleave Dimm Number should be < 4.
    ASSERT(i < (TWO_IMC_WAY * CH_2WAY));
    if (ImcSize == LineSize) {
      LineIndexSize = (Imc2Way4k_2ChWay4k_LineMultipliers[i][NumLines] * LineSize);
    } else {
      LineIndexSize = (Imc2Way4k_2ChWay256_LineMultipliers[i][NumLines] * LineSize);
    }
    break;
  case FOUR_IMC_WAY:
    // For 4 ImcWay Interleave Interleave Dimm Number should be < 8.
    ASSERT(i < (FOUR_IMC_WAY * CH_2WAY));
    if (ImcSize == LineSize) {
      LineIndexSize = (Imc4Way4k_2ChWay4k_LineMultipliers[i][NumLines] * LineSize);
    } else {
      LineIndexSize = (Imc4Way4k_2ChWay256_LineMultipliers[i][NumLines] * LineSize);
    }
    break;
  case EIGHT_IMC_WAY:
    // For 8 ImcWay Interleave Interleave Dimm Number should be < 16.
    ASSERT(i < (EIGHT_IMC_WAY * CH_2WAY));
    // 8 Imc Way not supported for 4k/4k
    LineIndexSize = (Imc8Way4k_2ChWay256_LineMultipliers[i][NumLines] * LineSize);
    break;
    default:
      DEBUG ((EFI_D_INFO, "default case.\n"));  //Auto added. Please review.
      break;
  }

  if (PMEMRgn) {
    *pSpa = (UINT64)(SADBaseAddr + (NumRotation * RotationSize * ImcWays * CH_2WAY) + LineIndexSize + (Dpa % LineSize));
  }
  else {
    *pSpa = (UINT64)(pDimm->SADSpaBase + (NumRotation * RotationSize * ImcWays * CH_2WAY) + LineIndexSize + (Dpa % LineSize));
  }

  return Status;
}

/**

Routine Description: XlateDpaToSpa - This function converts given Dpa
to Spa in the case of the Dimm involved in all Channel ways and
up to 8 Imc Ways.

  @param pDimm       - Pointer to Dimm structure
  @param SADBaseAddr - Start Address of the Spa for
                               Dpa Translation
  @param Dpa         - Dpa for which we need the SPA
  @param *pSpa       - Pointer to the Spa.
  @retval EFI_STATUS - status of the conversion effort

**/
EFI_STATUS
XlateDpaToSpa(
   NVDIMM          *pDimm,
   UINT64          SADBaseAddr,
   UINT64          Dpa,
   UINT64          *pSpa
   )
{
  EFI_STATUS       Status = EFI_SUCCESS;
  UINT8            ChWays;

  ChWays =  pDimm->ChWays;

  if (ChWays == CH_3WAY) {
    Status = GetSpaFor3WayChInt(pDimm, SADBaseAddr, Dpa, pSpa);
  }
  else if (ChWays == CH_2WAY) {
    Status = GetSpaFor2ChWays(pDimm, SADBaseAddr, Dpa, pSpa);
  }
  else if (ChWays == CH_1WAY) {
    Status = GetSpaFor1ChWay(pDimm, SADBaseAddr, Dpa, pSpa);
  }
  return (Status);
}

/**

Routine Description: Is this Dimm NVMDIMM

  @retval BOOLEAN     - TRUE if NVMDIMM otherwise false

**/
BOOLEAN
IsNvDimm (
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm
   )
{
  if (mSystemMemoryMap->Socket[socket].ChannelInfo[ch].DimmInfo[dimm].NVmemSize != 0) {
    return TRUE;
  }
  else
    return FALSE;
}

/**

Routine Description:PreInitDimmStruct pre initializes NVDIMM
struct that is being created with the values that are being
passed in of its members.

  @param SADSpaBase  - Spa Base Address of SAD Rule
  @param socket      - socket
  @param Ch          - Channel
  @param *dimm       - This pointer will hold dimm number
  @param ImcWays     - ImcWays this dimm is interleaved across
                     Imcs
  @param ChWays      - Channel Ways this dimm is interleaved.
  @param IntDimmNum  - Dimm number in the interleave set
  @param FMchannelInterBitmap - pSAD->FMchannelInterBitmap of
                              the sad rule

**/
VOID
PreInitDimmStruct(
   UINT64    SADSpaBase,
   UINT8     socket,
   UINT8     ch,
   UINT8     dimm,
   UINT8     ImcWays,
   UINT8     ChWays,
   UINT8     IntDimmNum,
   UINT8     FMchannelInterBitmap
   )
{
  NVDIMM       *pDimm;

  pDimm = &NvdimmInfo.Nvdimms[NvdimmInfo.NumNvdimms];
  pDimm->SADSpaBase = SADSpaBase;
  pDimm->Ch = ch;
  pDimm->Dimm = dimm;
  pDimm->SocketId = (UINT16)socket;
  pDimm->ImcId = MEM_SKTCH_TO_IMC (ch);
  pDimm->NonceSet = 0;
  pDimm->iMCWays = ImcWays;
  pDimm->ChWays = ChWays;
  pDimm->IntDimmNum = IntDimmNum;
  pDimm->FMchannelInterBitmap = FMchannelInterBitmap;
}

/**

Routine Description:InitPMEMInfoStruct initializes the PMEMInfo
struct that is in the NVDIMM structure with the values
provided as arguments into this function. Basically these values
are used for the translation of Spa-To-Dpa implementation
routines.

  @param Socket      - Socket Number
  @param Ch          - Channel number
  @param *Dimm       - This pointer will hold dimm number
  @param SADSpaBase  - Base Address of the SAD Rule found
  @param ImcWays     - Imc Ways interleave
  @param ChWays      - Channel Ways it is interleaved.
  @param IntDimmNum  - Dimm Number in the SAD Interleave set
  @retval None (VOID)

**/
VOID
InitPMEMInfoStruct(
   UINT8           Socket,
   UINT8           Ch,
   UINT8           Dimm,
   UINT64          SADSpaBase,
   UINT8           ImcWays,
   UINT8           ChWays,
   UINT8           IntDimmNum
   )
{
  PMEM_INFO        *pPMemInfo;

  if (NvdimmInfo.NumPMEMRgns >= ARRAY_SIZE (NvdimmInfo.PMEMInfo)) {
    return;
  }

  pPMemInfo = &NvdimmInfo.PMEMInfo[NvdimmInfo.NumPMEMRgns];

  pPMemInfo->SADPMemBase = SADSpaBase;
  pPMemInfo->Socket = Socket;
  pPMemInfo->Channel = Ch;
  pPMemInfo->Dimm = Dimm;
  pPMemInfo->ChWays = ChWays;
  pPMemInfo->iMCWays = ImcWays;
  pPMemInfo->IntDimmNum = IntDimmNum;
  NvdimmInfo.NumPMEMRgns += 1;
}


/**

Routine Description:AddPMemInfo adds pertinent
information regarding PMEM Region that has been found

  @param ImcWays     - Imc Ways interleave
  @param ChWays      - Channel Ways involved in the SAD Rule
  @param SADSpaBase  - Base Address of the SAD Rule found
  @param PmemRegion  - Each PMEM region already found
  @retval Status     - Status

**/
EFI_STATUS
AddPMemInfo(
   UINT8          ImcWays,
   UINT8          ChWays,
   UINT64         SADSpaBase,
   PMEM_REGION    *PmemRegion
   )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINT8           Soc, Ch, Dimm;
  UINT8           IntDimmNum = 0;
  UINT8           NvDimmIndex = 0;

  for (NvDimmIndex = 0; NvDimmIndex < PmemRegion->NumNvDimms; NvDimmIndex++) {
    Soc = PmemRegion->NvDimms[NvDimmIndex].SocketId;
    Ch = MEM_IMCCH_TO_SKTCH(PmemRegion->NvDimms[NvDimmIndex].ImcId , PmemRegion->NvDimms[NvDimmIndex].Ch);
    Dimm = PmemRegion->NvDimms[NvDimmIndex].Dimm;
    InitPMEMInfoStruct(Soc, Ch, Dimm, SADSpaBase, ImcWays, ChWays, IntDimmNum);
#if VERBOSE_DEBUG
    DEBUG((EFI_D_ERROR, "JNP: Found PMEM on, Socket = %d, Channel = %d, Dimm = %d\n", Soc, Ch, Dimm));
    DEBUG((EFI_D_ERROR, "JNP: ImcWays = %d, ChWays = %d\n", ImcWays, ChWays));
#endif
    IntDimmNum++;
  }

  return Status;
}

/**

Routine Description:BuildDimms builds Dimm info data
structs for each NVMDIMM's (found)

  @param ImcWays     - Imc Ways involved in the SAD Rule
  @param ChWays      - Channel Ways involved in the SAD Rule
  @param SADSpaBase  - Base Address associated with the SAD Rule
  @param PmemRegion  - Each PMEM region already found
  @retval Status     - Status

**/
EFI_STATUS
BuildDimmInfo(
   UINT8          ImcWays,
   UINT8          ChWays,
   UINT64         SADSpaBase,
   PMEM_REGION    *PmemRegion
   )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINT8           NvDimmIndex;
  UINT8           Soc, Ch, Dimm;
  UINT8           IntDimmNum = 0;
  struct          SADTable *pSAD;
  UINT8           ChInterBitmap;

  pSAD = &mSystemMemoryMap->Socket[PmemRegion->SocketNum].SAD[PmemRegion->SadIndex];
  ChInterBitmap = GetChInterBitmap(pSAD);

  for (NvDimmIndex = 0; NvDimmIndex < PmemRegion->NumNvDimms; NvDimmIndex++) {
    Soc = PmemRegion->NvDimms[NvDimmIndex].SocketId;
    Ch = MEM_IMCCH_TO_SKTCH(PmemRegion->NvDimms[NvDimmIndex].ImcId, PmemRegion->NvDimms[NvDimmIndex].Ch);
    Dimm = PmemRegion->NvDimms[NvDimmIndex].Dimm;
    PreInitDimmStruct(SADSpaBase, Soc, Ch, Dimm, ImcWays, ChWays, IntDimmNum, ChInterBitmap);
#if VERBOSE_DEBUG
    DEBUG((EFI_D_ERROR, "JNP: Found An Nvdimm on, Socket = %d, Channel = %d, Dimm = %d\n", Soc, Ch, Dimm));
    DEBUG((EFI_D_ERROR, "JNP: ImcWays = %d, ChWays = %d\n", ImcWays, ChWays));
    DEBUG((EFI_D_ERROR, "JNP: Dimm Num in the Interleave set = %d\n", IntDimmNum));
#endif
    NvdimmInfo.NumNvdimms++;
    IntDimmNum++;
  }

  ASSERT(NvdimmInfo.NumNvdimms <= MAX_SYS_DIMM);
#if VERBOSE_DEBUG
  DEBUG((EFI_D_ERROR, "JNP: Number of NvDimms found so far = %d\n", NvdimmInfo.NumNvdimms));
#endif
  return Status;
}

/**

Routine Description:UpdateDimmInfo updates Dimm info data struct
of the NVMDIMM's (found) Control Region and other info about
the Dimm.If there is more than one dimm in this sad rule, then
it is interleaved and need to account for each one, so creates a
separate dimminfo struct for each one.

  @param MemType       - Memory Type
  @retval VOID         - None

**/
VOID
UpdateDimmInfo()
{
  struct SADTable *pSAD;
  UINT8           ChInterBitmap;
  UINT64          SpaBase;
  UINT64          Size;
  UINT8           i, ChWays, ImcWays;
  UINT8           Socket;
  UINT8           PmemIndex;

  for (PmemIndex = 0; PmemIndex < MAX_PMEM_REGIONS; PmemIndex++) {
    if (mPmemRegions[PmemIndex].Size == 0) {
      continue;
    }
    Socket = mPmemRegions[PmemIndex].SocketNum;
    i = mPmemRegions[PmemIndex].SadIndex;
    pSAD = &mSystemMemoryMap->Socket[Socket].SAD[i];
    ChInterBitmap = GetChInterBitmap(pSAD);
    // Get Imc Interleave ways for this Cntrl Region SAD
    ImcWays = GetImcWays(Socket, i);
    // Get the Channel ways for this SAD
    ChWays = GetChInterleave(Socket, i);

    if(ImcWays == 0 || ChWays == 0){
      continue;
    }
    NvdimmInfo.SktGran = GetSocketGranularity(pSAD);
    NvdimmInfo.ChGran = GetChannelGranularity(pSAD);

    if (i == 0) {
      SpaBase = 0;
      Size = ((UINT64)mSystemMemoryMap->Socket[Socket].SAD[i].Limit << 26);
    }
    else {
      SpaBase = ((UINT64)mSystemMemoryMap->Socket[Socket].SAD[i - 1].Limit << 26);
      Size = ((UINT64)(mSystemMemoryMap->Socket[Socket].SAD[i].Limit - mSystemMemoryMap->Socket[Socket].SAD[i - 1].Limit) << 26);
    }
#if VERBOSE_DEBUG
    DEBUG((EFI_D_ERROR, "JNP: UpdateDimmInfo - SAD Range Base = 0x%1lx   Size = 0x%1lx\n", SpaBase, Size));
#endif

    BuildDimmInfo(ImcWays, ChWays, SpaBase, &mPmemRegions[PmemIndex]);
    AddPMemInfo(ImcWays, ChWays, SpaBase, &mPmemRegions[PmemIndex]);
  }
}

/**

  This function returns the total number of Ngn Dimms installed
  in the system.

  @param VOID - none

  @return Number of Ngn Dimms installed in the system.

**/
UINT32
GetNumNgnDimms(
  VOID
  )
{
  UINT8       socket, ch, dimm;
  UINT32      NumNgnDimms = 0;

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    for (ch = 0; ch < MAX_CH; ch++) {
      if (mSystemMemoryMap->Socket[socket].ChannelInfo[ch].Enabled == 0) {
        continue;
      }
      // Get the dimmNvList to see if any of them are DCPMMs
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if (mSystemMemoryMap->Socket[socket].ChannelInfo[ch].DimmInfo[dimm].Present == 0) {
          continue;
        }
        if (IsNvDimm(socket, ch, dimm)) {
          NumNgnDimms += 1;
        }
      }
    }
  }
  return (NumNgnDimms);
}

VOID
SwitchAddressModeToLegacy(
   UINT8 Node
   )
{
  SCRUBADDRESSHI_MC_MAIN_STRUCT   ScrubAddrHi;
  UINT8                           Socket = NODE_TO_SKT(Node);
  UINT8                           Mc = NODE_TO_MC(Node);

  ScrubAddrHi.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBADDRESSHI_MC_MAIN_REG);

  //
  // If the patrol scrub mode is SPA mode, need to move to legacy mode to setup the registers
  // and then move back to SPA mode.
  //

  if (ScrubAddrHi.Bits.ptl_sa_mode == 1) {
    ScrubAddrHi.Bits.ptl_sa_mode = 0;
    mCpuCsrAccess->WriteMcCpuCsr(Socket, Mc, SCRUBADDRESSHI_MC_MAIN_REG, ScrubAddrHi.Data);
  }
}

VOID
SwitchAddressModeToSystem(
   UINT8 Node
   )
{
  SCRUBADDRESSHI_MC_MAIN_STRUCT   ScrubAddrHi;
  UINT8                           Socket = NODE_TO_SKT(Node);
  UINT8                           Mc = NODE_TO_MC(Node);
  if (mSystemMemoryMap->MemSetup.patrolScrubAddrMode & PATROL_SCRUB_SPA)
  {
    ScrubAddrHi.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBADDRESSHI_MC_MAIN_REG);

    if (ScrubAddrHi.Bits.ptl_sa_mode == 0) {
      ScrubAddrHi.Bits.ptl_sa_mode = 1;
      mCpuCsrAccess->WriteMcCpuCsr(Socket, Mc, SCRUBADDRESSHI_MC_MAIN_REG, ScrubAddrHi.Data);
    }
  }
}

/**

  This routine an Address Range Scrub (ARS) using the global ARS data.

  @return Status

**/
EFI_STATUS
DoScrub()
{
  EFI_STATUS    Status;
  UINT8         Socket;
  UINT8         Mc;
  UINT8         Node;
  UINT64        SizeToScrub;
  UINT64        RankSize;
  UINT64        DimmSize;
  UINT64        NextDpaStart;
  UINT8         NumRanks;
  NVDIMM        *pDimm = NULL;
  SCRUBCTL_MC_MAIN_STRUCT         ScrubCtlReg;
  SCRUBADDRESSLO_MC_MAIN_STRUCT   ScrubAddrLoReg;
  SCRUBADDRESSHI_MC_MAIN_STRUCT   ScrubAddrHiReg;
  MCMAIN_CHKN_BITS_MC_MAIN_STRUCT McMainChknReg;
  SMISPARECTL_MC_MAIN_STRUCT      ImcSmiSpareCtlReg;

  Status = SystemAddressToDimmAddress (mArsScrubData.NextBaseAddress, &mArsScrubData.Ta);
  if(EFI_ERROR(Status)) {
    return ARS_START_FAILED;
  }

  Socket = mArsScrubData.Ta.SocketId;
  Mc = mArsScrubData.Ta.MemoryControllerId;
  Node = SKTMC_TO_NODE(Socket, Mc);

  // Switch to legacy mode
  SwitchAddressModeToLegacy (Node);

  // Set patrol inteval to PCU
  Status = PcuSetScrubInterval(Socket, Mc, ARS_SCRUB_INTERVAL);
  ASSERT_EFI_ERROR(Status);

  //
  // Enable patrol scrub SMI, stop on complete, and error logging
  //
  ImcSmiSpareCtlReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SMISPARECTL_MC_MAIN_REG);
  ImcSmiSpareCtlReg.Bits.intrpt_sel_smi  = 1;   // SMI Enable
  mCpuCsrAccess->WriteMcCpuCsr(Socket, Mc, SMISPARECTL_MC_MAIN_REG, ImcSmiSpareCtlReg.Data);

  McMainChknReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, MCMAIN_CHKN_BITS_MC_MAIN_REG);
  ScrubCtlReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBCTL_MC_MAIN_REG);

  // Enable patrol to stop after rank is scrubbed
#ifdef SKX_HOST
    McMainChknReg.Bits.defeature_12 = 1;
#elif defined(ICX_HOST)
    ScrubCtlReg.Bits.stop_on_rank = 1;
#endif
  mCpuCsrAccess->WriteMcCpuCsr(Socket, Mc, MCMAIN_CHKN_BITS_MC_MAIN_REG, McMainChknReg.Data);

  DimmSize = mSystemMemoryMap->Socket[Socket].ChannelInfo[MEM_IMCCH_TO_SKTCH(Mc, mArsScrubData.Ta.ChannelId)].DimmInfo[mArsScrubData.Ta.DimmSlot].NVmemSize;
  NumRanks = mSystemMemoryMap->Socket[Socket].ChannelInfo[MEM_IMCCH_TO_SKTCH(Mc, mArsScrubData.Ta.ChannelId)].DimmInfo[mArsScrubData.Ta.DimmSlot].NumRanks;
  RankSize = (DimmSize / NumRanks) << 26;
  SizeToScrub = RankSize - mArsScrubData.Ta.RankAddress;
#if VERBOSE_DEBUG
  DEBUG ((DEBUG_ERROR, " DimmSize = 0x%lx, NumRanks = %lx, RankSize = 0x%lx\n", DimmSize, NumRanks, RankSize));
  DEBUG ((DEBUG_ERROR, " SizeToScrub = 0x%lx\n", SizeToScrub));
#endif

  mMaxErrorsPerRank = MAX_ERRORS_PER_DIMM / NumRanks;
  mNumErrorsCurrRank = 0;

  DEBUG ((DEBUG_ERROR, "ARS: DoScrub  - SocketId = 0x%x, McId = 0x%x\n", mArsScrubData.Ta.SocketId, mArsScrubData.Ta.MemoryControllerId));
  DEBUG ((DEBUG_ERROR, "                Ch = 0x%x, DimmSlot = 0x%x, ChipSelect = 0x%x PhysicalRank = 0x%x,\n", mArsScrubData.Ta.ChannelId, mArsScrubData.Ta.DimmSlot,mArsScrubData.Ta.ChipSelect, mArsScrubData.Ta.PhysicalRankId));
  DEBUG ((DEBUG_ERROR, "                Row = 0x%x, Col = 0x%x, Bank = 0x%x\n", mArsScrubData.Ta.Row, mArsScrubData.Ta.Col, mArsScrubData.Ta.Bank));
  DEBUG ((DEBUG_ERROR, "                Node = %x, PhyRank = %x, RankAddr = %lx\n", Node, mArsScrubData.Ta.PhysicalRankId, mArsScrubData.Ta.RankAddress));
  DEBUG ((DEBUG_ERROR, "                ScrubAddr = %lx, SizeToScrub = %lx, RankSize = %lx\n", mArsScrubData.NextBaseAddress, SizeToScrub, RankSize));

#ifdef SKX_HOST
  *mScrubIntervalSave = ScrubCtlReg.Bits.scrubinterval;
  ScrubCtlReg.Bits.scrubinterval = ARS_SCRUB_INTERVAL;
#elif defined(ICX_HOST)
  Status = PcuGetScrubInterval(Socket, Mc, mScrubIntervalSave);
  ASSERT_EFI_ERROR(Status);
  Status = PcuSetScrubInterval(Socket, Mc, ARS_SCRUB_INTERVAL);
  ASSERT_EFI_ERROR(Status);
#endif

  ScrubCtlReg.Bits.scrub_en = 1;
  ScrubCtlReg.Bits.startscrub = 1;
  ScrubCtlReg.Bits.stop_on_cmpl = 1;
  ScrubCtlReg.Bits.stop_on_err = 1;
  ScrubCtlReg.Bits.ptl_cmpl = 0;

  ScrubAddrHiReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBADDRESSHI_MC_MAIN_REG);
  ScrubAddrHiReg.Bits.rank = mArsScrubData.Ta.ChipSelect;
  ScrubAddrHiReg.Bits.chnl = mArsScrubData.Ta.ChannelId;
  ScrubAddrHiReg.Bits.mirr_pri = 0;
  mCpuCsrAccess->WriteMcCpuCsr(Socket, Mc, SCRUBADDRESSHI_MC_MAIN_REG, ScrubAddrHiReg.Data);

  ScrubAddrLoReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBADDRESSLO_MC_MAIN_REG);
  ScrubAddrLoReg.Bits.rankadd = (UINT32)(mArsScrubData.Ta.RankAddress >> 6);
  mCpuCsrAccess->WriteMcCpuCsr(Socket, Mc, SCRUBADDRESSLO_MC_MAIN_REG, ScrubAddrLoReg.Data);

  mCpuCsrAccess->WriteMcCpuCsr(Socket, Mc, SCRUBADDRESSLO_MC_MAIN_REG, ScrubAddrLoReg.Data);
  mCpuCsrAccess->WriteMcCpuCsr(Socket, Mc, SCRUBADDRESSHI_MC_MAIN_REG, ScrubAddrHiReg.Data);
  mCpuCsrAccess->WriteMcCpuCsr(Socket, Mc, SCRUBCTL_MC_MAIN_REG, ScrubCtlReg.Data);

  // Check if more area to scrub on this NVDIMM
  if (mArsScrubData.SizeLeftToScrub > SizeToScrub) {
    mArsScrubData.SizeLeftToScrub -= SizeToScrub;
    mPmemIndex = mArsScrubData.PmemIndex[mArsScrubData.CurrentPmemIndex];
    NextDpaStart = NvdimmInfo.PMEMInfo[mPmemIndex].ARSDpaEnd - mArsScrubData.SizeLeftToScrub;
    pDimm = GetDimm (&NvdimmInfo, NvdimmInfo.PMEMInfo[mPmemIndex].Socket, NvdimmInfo.PMEMInfo[mPmemIndex].Channel, NvdimmInfo.PMEMInfo[mPmemIndex].Dimm, NVDIMM_FLAG_ALL);
    ASSERT(pDimm != NULL);
    if (pDimm != NULL) {
      Status = XlateDpaToSpa(pDimm, NvdimmInfo.PMEMInfo[mPmemIndex].SADPMemBase, NextDpaStart, &mArsScrubData.NextBaseAddress);
    }
  }
  // Check if another NVDIMM to scrub
  else if (mArsScrubData.NumDimmsLeft > 0) {
    mArsScrubData.NumDimmsLeft--;
    mArsScrubData.CurrentPmemIndex++;
    mPmemIndex = mArsScrubData.PmemIndex[mArsScrubData.CurrentPmemIndex];
    NextDpaStart = NvdimmInfo.PMEMInfo[mPmemIndex].ARSDpaStart;
    mArsScrubData.SizeLeftToScrub = (NvdimmInfo.PMEMInfo[mPmemIndex].ARSDpaEnd - NvdimmInfo.PMEMInfo[mPmemIndex].ARSDpaStart);
    pDimm = GetDimm (&NvdimmInfo, NvdimmInfo.PMEMInfo[mPmemIndex].Socket, NvdimmInfo.PMEMInfo[mPmemIndex].Channel, NvdimmInfo.PMEMInfo[mPmemIndex].Dimm, NVDIMM_FLAG_ALL);
    ASSERT(pDimm != NULL);
    if (pDimm != NULL) {
      Status = XlateDpaToSpa(pDimm, NvdimmInfo.PMEMInfo[mPmemIndex].SADPMemBase, NextDpaStart, &mArsScrubData.NextBaseAddress);
    }
  }
  else {
    mArsScrubData.SizeLeftToScrub = 0;
  }

  return ARS_START_SUCCESS;
}

/**

  This routine is sent to start Address Range Scrub for a
  particular Address range of a Ngn Dimm from the NVMCTLR command
  and passes the information back to the caller thru Acpi and
  the SMM Handler.

  @param Spa       - System Physical Address
  @param ArsLength - ARS Length
  @return Status

**/
EFI_STATUS
StartAddressRangeScrub(
   IN UINT64 Spa,
   IN UINT64 ArsLength
   )
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  UINT8                       Socket;
  UINT8                       Mc;
  UINT8                       Node;
  SCRUBCTL_MC_MAIN_STRUCT     ScrubCtlReg;
  INT32                       Index;

  // Make sure ARS is not already in progress
  if (mIsArs) {
    return ARS_IN_PROGRESS;
  }

  // Initialize ARS DSM Header
  mArsDsmHeader.NumErrorRecords = 0;
  mArsDsmHeader.StartSpa = Spa;
  mArsDsmHeader.Length = ArsLength;
  mArsDsmHeader.Type = PMEM_SCRUB_SUPPORTED;
  mArsDsmHeader.Flags = 0;
  // Init the Output length with that of header size & as and when we error records,
  // we will increment this appropriately.
  mArsDsmHeader.OutputLength = ARS_DSM_HEADER_SIZE;

  // Init global ARS data to start with first NVDIMM in SPA region
  mArsScrubData.CurrentPmemIndex = 0;
  Index = mArsScrubData.PmemIndex[mArsScrubData.CurrentPmemIndex];
  mArsScrubData.NextBaseAddress = Spa;
  mArsScrubData.SizeLeftToScrub = (NvdimmInfo.PMEMInfo[Index].ARSDpaEnd - NvdimmInfo.PMEMInfo[Index].ARSDpaStart);
  mArsScrubData.NumDimmsLeft--; // Decrement for first NVDIMM

  for (Socket = 0; Socket < MAX_SOCKET; Socket++){
    for (Mc = 0; Mc < MAX_IMC; Mc++) {
      if (!mSystemMemoryMap->Socket[Socket].imcEnabled[Mc]){
        continue;
      }
      Node = SKTMC_TO_NODE(Socket, Mc);
      ScrubCtlReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBCTL_MC_MAIN_REG);
      if (ScrubCtlReg.Bits.scrub_en == 1) {
        // Save MCMAIN_CHKN_BITS_MC_MAIN_REG, save patrol scrub regs, and disable scrub for each node
        mMcMainChknRegSave[Node].Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, MCMAIN_CHKN_BITS_MC_MAIN_REG);
        SavePatrolScrubEngine(Node);
        DisablePatrolScrubEngine(Node);
        mArsScrubData.CurrentNodeScrub[Node] = 1;
      }
      else {
        mArsScrubData.CurrentNodeScrub[Node] = 0;
      }
    }
  }

  Status = DoScrub();

  if (Status == ARS_START_SUCCESS) {
    // Set to indicate ARS has performed at least once on current boot (for query sts)
    if (!mHasArsPerformed) {
      mHasArsPerformed = TRUE;
    }
    mIsArs = TRUE;
  }

  return Status;
}

/**

  Given a PMEM Base Address, Socket and Channel number of the
  dimm in this interleave set, CheckIfTwoPMEMInDimm scans thru
  the PMemInfo struct to see if there is another PMEM Region
  defined for this socket and channel (with a different SAD Base
  Address of course) and if it is then it is the second PMEM
  Region for the dimm. If so, it returns TRUE, else FALSE.

  @param SADBaseAddr - SAD Rule base address
  @param Socket      - Socket
  @param Ch          - Channel
  @param Index       - will return the Index of the PMemInfo
                     struct that has the information about 2nd
                     PMEM Region
  @return Boolena    -  TRUE if a second PMem is found, else
          FALSE

**/
BOOLEAN
CheckIfTwoPMEMInDimm(
   UINT64         SADBaseAddr,
   UINT8          Socket,
   UINT8          Ch,
   INT32          *Index
   )
{
  UINT8           i;

  for (i = 0; (i < NvdimmInfo.NumPMEMRgns) && (i < ARRAY_SIZE (NvdimmInfo.PMEMInfo)); i++) {
    if ((NvdimmInfo.PMEMInfo[i].SADPMemBase != SADBaseAddr) && (NvdimmInfo.PMEMInfo[i].Socket == Socket) && \
        (NvdimmInfo.PMEMInfo[i].Channel == Ch)) {
      *Index = (INT32)i;
      return TRUE;
    }
  }
  return FALSE;
}


/**

  Given a PMEM Base Address, for all the dimms in this
  interleave set, SetupDpasForPMEMBase checks to see if the
  second PMEM is mapped to any of the dimms in this set, and if
  so, adjust the ArsEnd address to that of the second PMEM
  region so that when Start ARS command is sent for this dimm,
  it will cover both PMEM regions.

  @param SADBaseAddr - SAD Rule base address
  @return VOID

**/
VOID
SetupDpasForPMEMBase(
   UINT64         SADBaseAddr
   )
{
  UINT8           Skt, Ch;
  INT32           Index, PMem2Index;

  Index = GetPmemIndex(&NvdimmInfo, SADBaseAddr);
  if (Index == -1) {
    return;
  }

  Skt = NvdimmInfo.PMEMInfo[Index].Socket;
  Ch = NvdimmInfo.PMEMInfo[Index].Channel;
  Index++;
  // Make sure we don't index out of bounds of PMEMInfo in while loop
  if(Index >= (MAX_SYS_DIMM * MAX_PMEM_RGN_PER_DIMM)) {
    return;
  }

  while ((SADBaseAddr == NvdimmInfo.PMEMInfo[Index].SADPMemBase) && (Index < (MAX_SYS_DIMM * MAX_PMEM_RGN_PER_DIMM))) {
    if (CheckIfTwoPMEMInDimm(SADBaseAddr, Skt, Ch, &PMem2Index)) {
      if (!NvdimmInfo.PMEMInfo[PMem2Index].SecondPMemRgn) {
        if (NvdimmInfo.PMEMInfo[PMem2Index].ARSDpaEnd > NvdimmInfo.PMEMInfo[Index].ARSDpaEnd) {
          // Set the 1st PMem Region to that of actual DpaEnd which is in the second PMem Region
          NvdimmInfo.PMEMInfo[Index].ARSDpaEnd = NvdimmInfo.PMEMInfo[PMem2Index].ARSDpaEnd;
          // And set the flag to tell that it is a second PMem Region
          NvdimmInfo.PMEMInfo[PMem2Index].SecondPMemRgn = TRUE;
        }
        if(NvdimmInfo.PMEMInfo[PMem2Index].ARSDpaStart > NvdimmInfo.PMEMInfo[Index].ARSDpaStart) {
          NvdimmInfo.PMEMInfo[PMem2Index].ARSDpaStart = NvdimmInfo.PMEMInfo[Index].ARSDpaStart;
          // And set the flag to tell that it is a second PMem Region
          NvdimmInfo.PMEMInfo[PMem2Index].SecondPMemRgn = TRUE;
        }
      }
    }
    if(Index < ((MAX_SYS_DIMM * MAX_PMEM_RGN_PER_DIMM) - 1)) {
      Index++;
    }
    Skt = NvdimmInfo.PMEMInfo[Index].Socket;
    Ch = NvdimmInfo.PMEMInfo[Index].Channel;
  }
}

/**

  This routine SetupDpasForMultiplePMEMRgns is called when a
  ARSStart request comes in and the ARS Range is found to spawn
  over multiple SAD Rules. When this request comes in DpaStart
  and DpaEnd addresses are computed for all dimms in all the
  interleave sets. However, there could be dimms with 2 PMEM
  Regions that could be participating in the ARS Range that
  could be part of 2 interleave sets. This routine basically
  checks for these cases and computes the DpaEnd Addresses of
  these dimms so that only one NVMCTLR ARS Command is sent per dimm
  and their status is tracked.

  @param VOID
  @return VOID

**/
VOID
SetupDpasForMultiplePMEMRgns(
   VOID
   )
{
  UINT8           i;

  for (i = 0; i < MAX_PMEM_INTERLEAVE_SET; i++) {
    if (PMEMSADBaseAddresses[i] == 0) {
      break;
    }
    SetupDpasForPMEMBase(PMEMSADBaseAddresses[i]);
  }
}

/**

  This routine InitPMEMDpas is sent to reset the Dpa values
  stored in PMEMInfo Data struct before the start of the ARS
  processing.

  @param VOID

  @return VOID

**/
VOID
InitPMEMDpas(
   VOID
   )
{
  UINT8           i;

  for (i = 0; (i < NvdimmInfo.NumPMEMRgns) && (i < ARRAY_SIZE (NvdimmInfo.PMEMInfo)); i++) {
    NvdimmInfo.PMEMInfo[i].ARSDpaStart = 0;
    NvdimmInfo.PMEMInfo[i].ARSDpaEnd = 0;
    NvdimmInfo.PMEMInfo[i].SecondPMemRgn = 0;
  }
  //
  // Also reset the global PMEMSADBaseAddresses array
  for (i = 0; i < MAX_PMEM_INTERLEAVE_SET; i++) {
    PMEMSADBaseAddresses[i] = 0;
  }
}

/**

  This routine InitSktsAndChs is sent to initialize the global
  module variable arrays Skts and Chs (with 0xff).

  @param VOID

  @return VOID

**/
VOID
InitSktsAndChs(
   VOID
   )
{
  UINT8           i;

  for (i = 0; i < MAX_SOCKET; i++) {
    Skts[i] = 0xFF;
  }

  for (i = 0; i < MAX_MC_CH * MAX_IMC; i++) {
    Chs[i] = 0xFF;
  }
}

/**

  This routine IsUniqueSktOrCh returns a boolean when either of
  the input (Skt or Ch) is not found in the Skts and Chs global
  arrays.

  @param Skt       - Socket
  @param Ch        - Channel

  @return Boolean when either of the input is not founde in the
          array

**/
BOOLEAN
IsUniqueSktOrCh(
   UINT8           Skt,
   UINT8           Ch
   )
{
  UINT8           i;
  BOOLEAN         UniqueSkt = TRUE, UniqueCh = TRUE;

  for (i = 0; i < MAX_SOCKET; i++) {
    if (Skts[i] == Skt) {
      UniqueSkt = FALSE;
      break;
    }
    if (Skts[i] == 0xFF) {
      Skts[i] = Skt;
      break;
    }
  }

  for (i =  0; i < MAX_MC_CH * MAX_IMC; i++) {
    if (Chs[i] == Ch) {
      UniqueCh = FALSE;
      break;
    }
    if (Chs[i] == 0xFF) {
      Chs[i] = Ch;
      break;
    }
  }

  if ((UniqueSkt) || (UniqueCh)) {
    return (TRUE);
  }
  else
    return (FALSE);

}

/**

  This routine ComputeDPAsForARS Computes the Dpa start and end
  addresses of the Dimms involved in the interleave set and the
  ARS Range. And saves these values in the data structs for soon
  to send ARS Commands. If there are multiple PMEM Regions in
  any dimm, the end address will be offset to the second PMEM
  region address.

  @param Spa       - System Physical Address that needs to for
              which it requires ARS processing
  @param SADBaseAddr - SAD Rule base address
  @param ArsLength - ARS Length
  @return Status

**/
EFI_STATUS
ComputeDPAsForARS(
   UINT64         Spa,
   UINT64         SADBaseAddr,
   UINT64         ArsLength
   )
{
  UINT8           Skt, Ch, Dimm;
  UINT8           i, TotalDimms;
  INT32           Index;
  UINT32          ChGran;
  UINT8           StartDimmNum;
  UINT64          SpaStart;
  NVDIMM          *pDimm = NULL;
  EFI_STATUS      Status;

  //
  // Given the Spa, lets get the Socket, Ch & Dimm that this Spa is mapped into.
  Status = GetDimmInfoFromSpa(Spa, &SADBaseAddr, &Skt, &Ch, &Dimm);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  ChGran = GetChGranularityFromSadBaseAddr(SADBaseAddr);
  ASSERT (ChGran != 0);

  StartDimmNum = GetPMEMIntDimmNum(SADBaseAddr, Skt, Ch);
#if VERBOSE_DEBUG
  DEBUG((EFI_D_ERROR, "ARS: CompDPAs - Starting Dimm Number is = %d\n", StartDimmNum));
#endif

  // Total Number of Dimms in this interleave set
  TotalDimms = GetNumDimmsForPmemRgn(&NvdimmInfo, SADBaseAddr);

  Index =  GetPmemIndexBasedOnSocChDimm(&NvdimmInfo, SADBaseAddr, Skt, Ch, Dimm);

  // GetPmemIndexBasedOnSocChDimm returns -1 if it fails.
  if (Index < 0) {
    return EFI_NOT_FOUND;
  } // Return an error if Index is bigger than array PMEMInfo
  else if (Index >= (MAX_SYS_DIMM * MAX_PMEM_RGN_PER_DIMM)) {
    return EFI_BUFFER_TOO_SMALL;
  }

  mArsScrubData.PmemIndex[mArsScrubData.NumDimmsLeft] = Index;
  pDimm = GetDimm (&NvdimmInfo, Skt, Ch, Dimm, NVDIMM_FLAG_ALL);
  if (pDimm == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Get the starting dimm Spa translated to Dpa and save it in the PMEMInfo struct
  Status = XlateSpaToDpa(pDimm, Spa, SADBaseAddr, &NvdimmInfo.PMEMInfo[Index].ARSDpaStart);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "ARS: CompDPAs - Xlation from Spa to Dpa failed! returning\n"));
    return Status;
  }

  // Get the Dpa End based on ImcWays and Channel Ways
  NvdimmInfo.PMEMInfo[Index].ARSDpaEnd = NvdimmInfo.PMEMInfo[Index].ARSDpaStart + (ArsLength / (NvdimmInfo.PMEMInfo[Index].iMCWays * NvdimmInfo.PMEMInfo[Index].ChWays));

  //
  // Now get the Dpa starting address of all the dimms that cross into this given ARS Range
  // Done with starting 1st dimm, get the rest
  if (ChGran && (Spa % ChGran)) {
    SpaStart = (Spa / ChGran) * ChGran;
  }
  else {
    SpaStart = Spa;
  }
  //
  // Now get the Dpa starting address of all the dimms that cross into this given ARS Range
  // Done with starting 1st dimm, get the rest
  for (i = 0; i < (TotalDimms - 1); i++) {
    SpaStart += ChGran;
    // Ensure that SpaStart is with in the ARS Range provided
    if (SpaStart > (Spa + ArsLength)) {
      goto AllFound;
    }
    Status = GetDimmInfoFromSpa(SpaStart, &SADBaseAddr, &Skt, &Ch, &Dimm);

    // Now, we should have the next Dimm that is part of the ARS Set.
    Index =  GetPmemIndexBasedOnSocChDimm(&NvdimmInfo, SADBaseAddr, Skt, Ch, Dimm);
    if (Index == -1) {
      return EFI_INVALID_PARAMETER;
    }
    mArsScrubData.PmemIndex[mArsScrubData.NumDimmsLeft+i+1] = Index;
    pDimm = GetDimm (&NvdimmInfo, Skt, Ch, Dimm, NVDIMM_FLAG_ALL);
    if (pDimm == NULL) {
      return EFI_INVALID_PARAMETER;
    }

    // Get the starting dimm Spa translated to Dpa and save it in the PMEMInfo struct
    if (Index < (sizeof(NvdimmInfo.PMEMInfo) / sizeof(NvdimmInfo.PMEMInfo[0]))) {
      Status = XlateSpaToDpa(pDimm, SpaStart, SADBaseAddr, &NvdimmInfo.PMEMInfo[Index].ARSDpaStart);
      // Get the Dpa End based on ImcWays and Channel Ways
      NvdimmInfo.PMEMInfo[Index].ARSDpaEnd = NvdimmInfo.PMEMInfo[Index].ARSDpaStart + (ArsLength / (NvdimmInfo.PMEMInfo[Index].iMCWays * NvdimmInfo.PMEMInfo[Index].ChWays));
    } else {
      return EFI_INVALID_PARAMETER;
    }
  }
  mArsScrubData.NumDimmsLeft += TotalDimms;
AllFound:
  return Status;
}

VOID
GetDimmId(
   JEDEC_NVDIMM *Nvdimm
)
{
  UINT8 Data;
  NVDIMM_IDENTIFICATION *DimmId = (NVDIMM_IDENTIFICATION *)&Nvdimm->DimmId;

  JedecSwitchPage(Nvdimm, 3);
  ReadSmb(Nvdimm, (NVM_CURRENT_FW_SLOT & 0xFF), &Data);
  DimmId->CurrentFirmwareSlot = Data >> 4;

  JedecSwitchPage(Nvdimm, 0);
  if (DimmId->CurrentFirmwareSlot == 0) {
    ReadSmb(Nvdimm, (NVM_FW_REV_SLOT0 & 0xFF), &Data);
    DimmId->FirmwareRevision = Data;
    ReadSmb(Nvdimm, ((NVM_FW_REV_SLOT0 + 1) & 0xFF), &Data);
    DimmId->FirmwareRevision |= Data << 8;
  } else if (DimmId->CurrentFirmwareSlot == 1) {
    ReadSmb(Nvdimm, (NVM_FW_REV_SLOT1 & 0xFF), &Data);
    DimmId->FirmwareRevision = Data;
    ReadSmb(Nvdimm, ((NVM_FW_REV_SLOT1 + 1) & 0xFF), &Data);
    DimmId->FirmwareRevision |= Data << 8;
  }

  ReadSmb(Nvdimm, (NVM_SPECIFICATION_REV & 0xFF), &Data);
  DimmId->SpecificationRevision = Data;
  ReadSmb(Nvdimm, (NVM_NUM_STANDARD_PAGES & 0xFF), &Data);
  DimmId->StandardPageNum = Data;
  ReadSmb(Nvdimm, (NVM_FIRST_VENDOR_PAGE & 0xFF), &Data);
  DimmId->FirstVendorPage = Data;
  ReadSmb(Nvdimm, (NVM_NUM_VENDOR_PAGES & 0xFF), &Data);
  DimmId->VendorPageNum = Data;
  ReadSmb(Nvdimm, (NVM_HARDWARE_REV & 0xFF), &Data);
  DimmId->HardwareRevision = Data;
  DimmId->FirmwareSlotCount = 2;    // Must be 2 for slot 0 and slot 1
  ReadSmb(Nvdimm, (NVM_CAPABILITIES & 0xFF), &Data);
  DimmId->Capabilities = Data;
  ReadSmb(Nvdimm, (NVM_SUPPORTED_BACKUP_TRIGGER & 0xFF), &Data);
  DimmId->SupportedBackupTriggers = Data;
  ReadSmb(Nvdimm, (NVM_MAX_OP_RETRY_COUNT & 0xFF), &Data);
  DimmId->MaxOpRetryCount = Data;
  ReadSmb(Nvdimm, (NVM_SUPPORTED_NOTIFY_EVENTS & 0xFF), &Data);
  DimmId->SupportedNotificationEvents = Data;
  ReadSmb(Nvdimm, (NVM_SAVE_OP_TIMEOUT & 0xFF), &Data);
  DimmId->SaveOpTimeout = Data;
  ReadSmb(Nvdimm, ((NVM_SAVE_OP_TIMEOUT + 1) & 0xFF), &Data);
  DimmId->SaveOpTimeout |= Data << 8;
  ReadSmb(Nvdimm, (NVM_RESTORED_OP_TIMEOUT & 0xFF), &Data);
  DimmId->RestoreOpTimeout = Data;
  ReadSmb(Nvdimm, ((NVM_RESTORED_OP_TIMEOUT + 1) & 0xFF), &Data);
  DimmId->RestoreOpTimeout |= Data << 8;
  ReadSmb(Nvdimm, (NVM_ERASED_OP_TIMEOUT & 0xFF), &Data);
  DimmId->EraseOpTimeout = Data;
  ReadSmb(Nvdimm, ((NVM_ERASED_OP_TIMEOUT + 1) & 0xFF), &Data);
  DimmId->EraseOpTimeout |= Data << 8;
  ReadSmb(Nvdimm, (NVM_ARMED_OP_TIMEOUT & 0xFF), &Data);
  DimmId->ArmOpTimeout = Data;
  ReadSmb(Nvdimm, ((NVM_ARMED_OP_TIMEOUT + 1) & 0xFF), &Data);
  DimmId->ArmOpTimeout |= Data << 8;
  ReadSmb(Nvdimm, (NVM_FW_OP_TIMEOUT & 0xFF), &Data);
  DimmId->FwOpTimeout = Data;
  ReadSmb(Nvdimm, ((NVM_FW_OP_TIMEOUT + 1) & 0xFF), &Data);
  DimmId->FwOpTimeout |= Data << 8;
  ReadSmb(Nvdimm, (NVM_ABORT_OP_TIMEOUT & 0xFF), &Data);
  DimmId->AbortOpTimeout = Data;
  ReadSmb(Nvdimm, (NVM_MIN_OP_TEMP & 0xFF), &Data);
  DimmId->MinOpTemp = Data;
  ReadSmb(Nvdimm, (NVM_MAX_OP_TEMP & 0xFF), &Data);
  DimmId->MaxOpTemp = Data;
  ReadSmb(Nvdimm, (NVM_REGION_BLOCK_SIZE & 0xFF), &Data);
  DimmId->RegionBlockSize = Data;

#if VERBOSE_DEBUG
  DEBUG((EFI_D_INFO, "NVDIMM NVM_SPECIFICATION_REV = 0x%lX\n", DimmId->SpecificationRevision));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_NUM_STANDARD_PAGES = 0x%lX\n", DimmId->StandardPageNum));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_FIRST_VENDOR_PAGE = 0x%lX\n", DimmId->FirstVendorPage));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_NUM_VENDOR_PAGES = 0x%lX\n", DimmId->VendorPageNum));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_HARDWARE_REV = 0x%lX\n", DimmId->HardwareRevision));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_FW_REV_SLOT0 = 0x%lX\n", DimmId->FirmwareRevision));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_FW_REV_SLOT0 = 0x%lX\n", DimmId->FirmwareRevision));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_FW_REV_SLOT1 = 0x%lX\n", DimmId->FirmwareRevision));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_FW_REV_SLOT1 = 0x%lX\n", DimmId->FirmwareRevision));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_CAPABILITIES = 0x%lX\n", DimmId->Capabilities));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_SUPPORTED_BACKUP_TRIGGER = 0x%lX\n", DimmId->SupportedBackupTriggers));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_MAX_OP_RETRY_COUNT = 0x%lX\n", DimmId->MaxOpRetryCount));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_SUPPORTED_NOTIFY_EVENTS = 0x%lX\n", DimmId->SupportedNotificationEvents));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_SAVE_OP_TIMEOUT = 0x%lX\n", DimmId->SaveOpTimeout));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_RESTORED_OP_TIMEOUT = 0x%lX\n", DimmId->RestoreOpTimeout));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_ERASED_OP_TIMEOUT = 0x%lX\n", DimmId->EraseOpTimeout));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_ARMED_OP_TIMEOUT = 0x%lX\n", DimmId->ArmOpTimeout));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_FW_OP_TIMEOUT = 0x%lX\n", DimmId->FwOpTimeout));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_ABORT_OP_TIMEOUT = 0x%lX\n", DimmId->AbortOpTimeout));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_MIN_OP_TEMP = 0x%lX\n", DimmId->MinOpTemp));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_MAX_OP_TEMP = 0x%lX\n", DimmId->MaxOpTemp));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_REGION_BLOCK_SIZE = 0x%lX\n", DimmId->RegionBlockSize ));
  DEBUG((EFI_D_INFO, "NVDIMM NVM_CURRENT_FW_SLOT = 0x%lX\n", DimmId->CurrentFirmwareSlot));
#endif
}

VOID
GetDimmPwrInfo(
JEDEC_NVDIMM *Nvdimm
)
{
  UINT8 Data;
  SAVE_OPERATION_REQ *SaveOpReg = (SAVE_OPERATION_REQ *)&Nvdimm->SaveOpReg;

  JedecSwitchPage(Nvdimm, 0);
  ReadSmb(Nvdimm, (NVM_AVG_PWR_REQ & 0xFF), &Data);
  SaveOpReg->AveragePwrReq = Data;
  ReadSmb(Nvdimm, ((NVM_AVG_PWR_REQ + 1) & 0xFF), &Data);
  SaveOpReg->AveragePwrReq |= Data << 8;
  ReadSmb(Nvdimm, (NVM_IDLE_PWR_REQ & 0xFF), &Data);
  SaveOpReg->IdlePwrReg = Data;
  ReadSmb(Nvdimm, ((NVM_IDLE_PWR_REQ + 1) & 0xFF), &Data);
  SaveOpReg->IdlePwrReg |= Data << 8;
  ReadSmb(Nvdimm, (NVM_MIN_VOLT_REQ & 0xFF), &Data);
  SaveOpReg->MinVoltReq = Data;
  ReadSmb(Nvdimm, ((NVM_MIN_VOLT_REQ + 1) & 0xFF), &Data);
  SaveOpReg->MinVoltReq |= Data << 8;
  ReadSmb(Nvdimm, (NVM_MAX_VOLT_REQ & 0xFF), &Data);
  SaveOpReg->MaxVoltReg = Data;
  ReadSmb(Nvdimm, ((NVM_MAX_VOLT_REQ + 1)& 0xFF), &Data);
  SaveOpReg->MaxVoltReg |= Data << 8;

#if VERBOSE_DEBUG
  DEBUG((EFI_D_INFO, "AveragePwrReq = %02x\n", SaveOpReg->AveragePwrReq));
  DEBUG((EFI_D_INFO, "IdlePwrReg    = %02x\n", SaveOpReg->IdlePwrReg));
  DEBUG((EFI_D_INFO, "MinVoltReq    = %02x\n", SaveOpReg->MinVoltReq));
  DEBUG((EFI_D_INFO, "MaxVoltReg    = %02x\n", SaveOpReg->MaxVoltReg));
#endif
}

VOID
GetDimmESInfo(
JEDEC_NVDIMM *Nvdimm
)
{
  UINT8 Data;
  UINT8 ESPolicy;
  ENERGY_SOURCE_IDENTIFICATION *ESInfo = (ENERGY_SOURCE_IDENTIFICATION *)&Nvdimm->ESInfo;
  ES_LIFETIME_INFO             *ESLifeInfo = (ES_LIFETIME_INFO *)&Nvdimm->ESLifeInfo;

  ZeroMem(ESInfo, sizeof(ENERGY_SOURCE_IDENTIFICATION));
  JedecSwitchPage(Nvdimm, 0);
  ReadSmb(Nvdimm, (NVM_ES_POLICY & 0xFF), &Data);
  DEBUG((EFI_D_ERROR, "ES Policy Supported  = %02x\n", Data));

  ReadSmb(Nvdimm, (NVM_SET_ES_POLICY_STATUS & 0xFF), &ESPolicy);
  DEBUG((EFI_D_ERROR, "ES Policy Status  = %02x\n", ESPolicy));

  if (ESPolicy & DEVICE_MANAGED_POLICY_ENABLED) {
    ESInfo->ESPolicy = ESINFO_DEVICE_MANAGED_POLICY;
    DEBUG((EFI_D_ERROR, "Selected device managed ES policy\n"));
  } else if (ESPolicy & HOST_MANAGED_POLICY_ENABLED) {
    ESInfo->ESPolicy = ESINFO_HOST_MANAGED_POLICY;
    DEBUG((EFI_D_ERROR, "Selected Host managed ES policy. Cannot read Energy Source Info registers.\n"));
    return;
  } else {
    DEBUG((EFI_D_ERROR, "ERROR: ES Policy is not set!\n"));
  }

  if (ESInfo->ESPolicy == ESINFO_DEVICE_MANAGED_POLICY) {
    ReadSmb(Nvdimm, (NVM_ES_HEALTH_CHECK_FREQ & 0xFF), &Data);
    ESInfo->DevESId.ESHealthChkFreq = Data;
    JedecSwitchPage(Nvdimm, 1);
    ReadSmb(Nvdimm, (NVM_ES_ATTRIBUTES & 0xFF), &Data);
    ESInfo->DevESId.ESAttribute = Data;
    ReadSmb(Nvdimm, (NVM_ES_TECHNOLOGY & 0xFF), &Data);
    ESInfo->DevESId.ESTechnology = Data;
    ReadSmb(Nvdimm, (NVM_ES_HARDWARE_REV & 0xFF), &Data);
    ESInfo->DevESId.ESHardwareRev = Data;
    ReadSmb(Nvdimm, (NVM_ES_FW_REV & 0xFF), &Data);
    ESInfo->DevESId.ESFirmwareRev = Data;
    ReadSmb(Nvdimm, ((NVM_ES_FW_REV + 1) & 0xFF), &Data);
    ESInfo->DevESId.ESFirmwareRev |= Data << 8;
    ReadSmb(Nvdimm, (NVM_ES_CHARGE_TIMEOUT & 0xFF), &Data);
    ESInfo->DevESId.ESChargeTimeout0 = Data;
    ReadSmb(Nvdimm, ((NVM_ES_CHARGE_TIMEOUT + 1) & 0xFF), &Data);
    ESInfo->DevESId.ESChargeTimeout1 = Data;
    ReadSmb(Nvdimm, (NVM_ES_MIN_OP_TEMP & 0xFF), &Data);
    ESInfo->DevESId.ESMinOpTemp = Data;
    ReadSmb(Nvdimm, (NVM_ES_MAX_OP_TEMP & 0xFF), &Data);
    ESInfo->DevESId.ESMaxOpTemp = Data;
#if VERBOSE_DEBUG
    DEBUG((EFI_D_INFO, "ESHealthChkFreq  = %02x\n", ESInfo->DevESId.ESHealthChkFreq));
    DEBUG((EFI_D_INFO, "ESAttribute      = %02x\n", ESInfo->DevESId.ESAttribute));
    DEBUG((EFI_D_INFO, "ESTechnology     = %02x\n", ESInfo->DevESId.ESTechnology));
    DEBUG((EFI_D_INFO, "ESHardwareRev    = %02x\n", ESInfo->DevESId.ESHardwareRev));
    DEBUG((EFI_D_INFO, "ESFirmwareRev    = %02x\n", ESInfo->DevESId.ESFirmwareRev));
    DEBUG((EFI_D_INFO, "ESChargeTimeout0 = %02x\n", ESInfo->DevESId.ESChargeTimeout0));
    DEBUG((EFI_D_INFO, "ESChargeTimeout1 = %02x\n", ESInfo->DevESId.ESChargeTimeout1));
    DEBUG((EFI_D_INFO, "ESMinOpTemp      = %02x\n", ESInfo->DevESId.ESMinOpTemp));
    DEBUG((EFI_D_INFO, "ESMaxOpTemp      = %02x\n", ESInfo->DevESId.ESMaxOpTemp));
#endif
  } else {
    ReadSmb(Nvdimm, (NVM_ES_HEALTH_CHECK_FREQ & 0xFF), &Data);
    ESInfo->HostESId.ESHealthChkFreq = Data;
    JedecSwitchPage(Nvdimm, 2);
    ReadSmb(Nvdimm, (NVM_HOST_MANAGED_ES_ATTRIBUTES & 0xFF), &Data);
    ESInfo->HostESId.ESAttribute = Data;
    ESInfo->HostESId.ESTechnology = 1; // ESTechnology is [0] : Undefined
#if VERBOSE_DEBUG
    DEBUG((EFI_D_INFO, "ESHealthChkFreq  = %02x\n", ESInfo->DevESId.ESHealthChkFreq));
    DEBUG((EFI_D_INFO, "ESAttribute      = %02x\n", ESInfo->DevESId.ESAttribute));
    DEBUG((EFI_D_INFO, "ESTechnology     = %02x\n", ESInfo->DevESId.ESTechnology));
#endif
  }

  JedecSwitchPage(Nvdimm, 1);
  ReadSmb(Nvdimm, (NVM_ES_LIFETIME_PERCENTAGE & 0xFF), &Data);
  ESLifeInfo->ESLifeTime = Data;
  ReadSmb(Nvdimm, (NVM_ES_CURRENT_TEMP & 0xFF), &Data);
  ESLifeInfo->ESTemp0 = Data;
  ReadSmb(Nvdimm, ((NVM_ES_CURRENT_TEMP + 1) & 0xFF), &Data);
  ESLifeInfo->ESTemp1 = Data;
  ReadSmb(Nvdimm, (NVM_ES_TOTAL_RUNTIME & 0xFF), &Data);
  ESLifeInfo->ESRuntime0 = Data;
  ReadSmb(Nvdimm, ((NVM_ES_TOTAL_RUNTIME + 1) & 0xFF), &Data);
  ESLifeInfo->ESRuntime1 = Data;
#if VERBOSE_DEBUG
  DEBUG((EFI_D_INFO, "NVM_ES_LIFETIME_PERCENTAGE   = %02x\n", ESLifeInfo->ESLifeTime));
  DEBUG((EFI_D_INFO, "NVM_ES_CURRENT_TEMP0         = %02x\n", ESLifeInfo->ESTemp0));
  DEBUG((EFI_D_INFO, "NVM_ES_CURRENT_TEMP1         = %02x\n", ESLifeInfo->ESTemp1));
  DEBUG((EFI_D_INFO, "NVM_ES_TOTAL_RUNTIME         = %02x\n", ESLifeInfo->ESRuntime0));
  DEBUG((EFI_D_INFO, "NVM_ES_TOTAL_RUNTIME1        = %02x\n", ESLifeInfo->ESRuntime1));
#endif
}

VOID
GetDimmOpStatistics(
JEDEC_NVDIMM *Nvdimm
)
{
  UINT8 Data;
  OPERATION_STATISTIC_INFO *OpInfo = (OPERATION_STATISTIC_INFO *)&Nvdimm->OpInfo;

  JedecSwitchPage(Nvdimm, 2);
  ReadSmb(Nvdimm, (NVM_LAST_SAVE_OP_DURATION & 0xFF), &Data);
  OpInfo->LastSaveDuration0 = Data;
  ReadSmb(Nvdimm, ((NVM_LAST_SAVE_OP_DURATION + 1) & 0xFF), &Data);
  OpInfo->LastSaveDuration1 = Data;

  ReadSmb(Nvdimm, (NVM_LAST_RESTORE_OP_DURATION & 0xFF), &Data);
  OpInfo->LastRestoreDuration0 = Data;
  ReadSmb(Nvdimm, ((NVM_LAST_RESTORE_OP_DURATION + 1) & 0xFF), &Data);
  OpInfo->LastRestoreDuration1 = Data;

  ReadSmb(Nvdimm, (NVM_LAST_ERASE_OP_DURATION & 0xFF), &Data);
  OpInfo->LastEraseDuration0 = Data;
  ReadSmb(Nvdimm, ((NVM_LAST_ERASE_OP_DURATION + 1) & 0xFF), &Data);
  OpInfo->LastEraseDuration1 = Data;

  ReadSmb(Nvdimm, (NVM_COMPLETED_SAVE_OP_NUMBER & 0xFF), &Data);
  OpInfo->NumSaveOpsCount0 = Data;
  ReadSmb(Nvdimm, ((NVM_COMPLETED_SAVE_OP_NUMBER + 1) & 0xFF), &Data);
  OpInfo->NumSaveOpsCount1 = Data;

  ReadSmb(Nvdimm, (NVM_COMPLETED_RESTORE_OP_NUMBER & 0xFF), &Data);
  OpInfo->NumRestoreOpsCount0 = Data;
  ReadSmb(Nvdimm, ((NVM_COMPLETED_RESTORE_OP_NUMBER + 1) & 0xFF), &Data);
  OpInfo->NumRestoreOpsCount1 = Data;

  ReadSmb(Nvdimm, (NVM_COMPLETED_ERASE_OP_NUMBER & 0xFF), &Data);
  OpInfo->NumEraseOpsCount0 = Data;
  ReadSmb(Nvdimm, ((NVM_COMPLETED_ERASE_OP_NUMBER + 1) & 0xFF), &Data);
  OpInfo->NumEraseOpsCount1 = Data;

  ReadSmb(Nvdimm, (NVM_MODULE_PWR_CYCLES & 0xFF), &Data);
  OpInfo->NumModulePwrCycle0 = Data;
  ReadSmb(Nvdimm, ((NVM_MODULE_PWR_CYCLES + 1) & 0xFF), &Data);
  OpInfo->NumModulePwrCycle1 = Data;

#if VERBOSE_DEBUG
  DEBUG((EFI_D_INFO, "LastSaveDuration0    = %02x\n", OpInfo->LastSaveDuration0));
  DEBUG((EFI_D_INFO, "LastSaveDuration1    = %02x\n", OpInfo->LastSaveDuration1));
  DEBUG((EFI_D_INFO, "LastRestoreDuration0 = %02x\n", OpInfo->LastRestoreDuration0));
  DEBUG((EFI_D_INFO, "LastRestoreDuration1 = %02x\n", OpInfo->LastRestoreDuration1));
  DEBUG((EFI_D_INFO, "LastEraseDuration0   = %02x\n", OpInfo->LastEraseDuration0));
  DEBUG((EFI_D_INFO, "LastEraseDuration1   = %02x\n", OpInfo->LastEraseDuration1));
  DEBUG((EFI_D_INFO, "NumSaveOpsCount0     = %02x\n", OpInfo->NumSaveOpsCount0));
  DEBUG((EFI_D_INFO, "NumSaveOpsCount1     = %02x\n", OpInfo->NumSaveOpsCount1));
  DEBUG((EFI_D_INFO, "NumRestoreOpsCount0  = %02x\n", OpInfo->NumRestoreOpsCount0));
  DEBUG((EFI_D_INFO, "NumRestoreOpsCount1  = %02x\n", OpInfo->NumRestoreOpsCount1));
  DEBUG((EFI_D_INFO, "NumEraseOpsCount0    = %02x\n", OpInfo->NumEraseOpsCount0));
  DEBUG((EFI_D_INFO, "NumEraseOpsCount1    = %02x\n", OpInfo->NumEraseOpsCount1));
  DEBUG((EFI_D_INFO, "NumModulePwrCycle0   = %02x\n", OpInfo->NumModulePwrCycle0));
  DEBUG((EFI_D_INFO, "NumModulePwrCycle1   = %02x\n", OpInfo->NumModulePwrCycle1));
#endif
}

VOID
GetDimmIModuleInfo(
JEDEC_NVDIMM *Nvdimm
)
{
  UINT8 Data;
  MODULE_INFO *ModuleInfo = (MODULE_INFO *)&Nvdimm->ModuleInfo;

  JedecSwitchPage(Nvdimm, 0);
  ReadSmb(Nvdimm, (NVM_MODULE_HEALTH & 0xFF), &Data);
  ModuleInfo->Health.ModuleHealthStatus = Data;
  ReadSmb(Nvdimm, ((NVM_MODULE_HEALTH + 1) & 0xFF), &Data);
  ModuleInfo->Health.ModuleHealthStatus |= Data << 8;
  ModuleInfo->Health.ModuleTemp = 28;         // N/A I2C
  ReadSmb(Nvdimm, (NVM_ERR_THRESHOLD_STATUS & 0xFF), &Data);
  ModuleInfo->Health.ErrorThresStatus = Data;
  ReadSmb(Nvdimm, (NVM_WARN_THRESHOLD_STATUS & 0xFF), &Data);
  ModuleInfo->Health.ErrorThresWarn = Data;
  ReadSmb(Nvdimm, (NVM_LIFETIME & 0xFF), &Data);
  ModuleInfo->Health.NVMLifetime = Data;
  JedecSwitchPage(Nvdimm, 2);
  ReadSmb(Nvdimm, (NVM_UNCORR_ECC_COUNTS & 0xFF), &Data);
  ModuleInfo->Health.UECount = Data;
  ReadSmb(Nvdimm, (NVM_CORR_ECC_COUNTS & 0xFF), &Data);
  ModuleInfo->Health.CECount = Data;

#if VERBOSE_DEBUG
  DEBUG((EFI_D_INFO, "ModuleHealthStatus   = %02x\n", ModuleInfo->Health.ModuleHealthStatus));
  DEBUG((EFI_D_INFO, "ModuleTemp           = %02x\n", ModuleInfo->Health.ModuleTemp));
  DEBUG((EFI_D_INFO, "ErrorThresStatus     = %02x\n", ModuleInfo->Health.ErrorThresStatus));
  DEBUG((EFI_D_INFO, "ErrorThresWarn       = %02x\n", ModuleInfo->Health.ErrorThresWarn));
  DEBUG((EFI_D_INFO, "NVMLifetime          = %02x\n", ModuleInfo->Health.NVMLifetime));
  DEBUG((EFI_D_INFO, "UECount              = %02x\n", ModuleInfo->Health.UECount));
  DEBUG((EFI_D_INFO, "CECount              = %02x\n", ModuleInfo->Health.CECount));
#endif

  JedecSwitchPage(Nvdimm, 0);
  ReadSmb(Nvdimm, (NVM_CRITICAL_HEALTH_INFO & 0xFF), &Data);
  ModuleInfo->CriticalHealthInfo = Data;
  ReadSmb(Nvdimm, (NVM_LIFETIME_PERCENT_WARN_THRESHOLD & 0xFF), &Data);
  ModuleInfo->LifetimePercentWarnTheshold = Data;
  ReadSmb(Nvdimm, (NVM_LIFETIME_PERCENT_ERR_THRESHOLD & 0xFF), &Data);
  ModuleInfo->LifetimePercentErrorTheshold = Data;

  ReadSmb(Nvdimm, (NVM_TRIGGER_INFO & 0xFF), &Data);
  ModuleInfo->TriggerInformation = Data;

  ReadSmb(Nvdimm, (NVM_SAVE_FAILURE_INFO & 0xFF), &Data);
  ModuleInfo->SaveFailInformation = Data;
  ReadSmb(Nvdimm, ((NVM_SAVE_FAILURE_INFO + 1) & 0xFF), &Data);
  ModuleInfo->SaveFailInformation |= Data << 8;

  ReadSmb(Nvdimm, (NVM_VENDOR_PAGE_LOG_SIZE & 0xFF), &Data);
  ModuleInfo->VendorLogPageSize = Data;
  ModuleInfo->VendorLogPage = Data; //  - NOTE: Vendor Log Page not supported, return Page Size instead.
  ReadSmb(Nvdimm, (NVM_ERR_INJ_ENABLE & 0xFF), &Data);
  ModuleInfo->ErrorInjEnable = ((Data & BIT2) == 0) ? 0 : 1;

#if VERBOSE_DEBUG
  DEBUG((EFI_D_INFO, "CriticalHealthInfo           = %02x\n", ModuleInfo->CriticalHealthInfo));
  DEBUG((EFI_D_INFO, "LifetimePercentWarnTheshold  = %02x\n", ModuleInfo->LifetimePercentWarnTheshold));
  DEBUG((EFI_D_INFO, "LifetimePercentErrorTheshold = %02x\n", ModuleInfo->LifetimePercentErrorTheshold));
  DEBUG((EFI_D_INFO, "TriggerInformation           = %02x\n", ModuleInfo->TriggerInformation));
  DEBUG((EFI_D_INFO, "SaveFailInformation          = %02x\n", ModuleInfo->SaveFailInformation));
  DEBUG((EFI_D_INFO, "VendorLogPageSize            = %02x\n", ModuleInfo->VendorLogPageSize));
  DEBUG((EFI_D_INFO, "VendorLogPage                = %02x\n", ModuleInfo->VendorLogPage));
  DEBUG((EFI_D_INFO, "ErrorInjEnable               = %02x\n", ModuleInfo->ErrorInjEnable));
#endif
}

/**

  HandleARSStart handles Start Address Range Scrub command from
  the OS. Given the Spa and ARS Length, this function figurs out
  all the Nvm Dimms in the interleave set and sends Start ARS
  commands to all these Dimms and also handles the results of
  these commands to these dimms.

  @param Spa       - System Physical Address that needs to for
              which it requires ARS processing
  @param ArsLength - ARS Length
  @return Status

**/
EFI_STATUS
HandleARSStart(
   UINT64         Spa,
   UINT64         ArsLength
   )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINT64          SADBaseAddr, SADNextBase, Limit, NextLimit;
  UINT64          SpaStart;
  UINT64          SADSize;
  UINT64          ArsSize;
  UINT8           Skt, Ch, Dimm;
  UINT8           i;
  BOOLEAN         MultipleSADs = FALSE;

  //
  // Given the Spa, lets get the Socket, Ch & Dimm that this Spa is mapped into.
  DEBUG((EFI_D_ERROR, "ARS: ARSStart - SpaStart = 0x%1lx, Length = 0x%1lx\n", Spa, ArsLength));
  Status = GetDimmInfoFromSpa(Spa, &SADBaseAddr, &Skt, &Ch, &Dimm);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "ARS: GetDimmInfoFromSpa failed, returning!!!\n"));
    return (Status);
  }

  DEBUG((EFI_D_ERROR, "ARS: ARSStart - SADBaseAddr = 0x%1lx\n", SADBaseAddr));
  DEBUG((EFI_D_ERROR, "ARS: ARSStart - ARSStart is in Soc = %d, Ch = %d, Dimm = %d\n", Skt, Ch, Dimm));

  // Check and if the range is in the PMEM Region, if it isn't return error status
  if (!IsPmemRgn(&NvdimmInfo, SADBaseAddr)) {
    DEBUG((EFI_D_ERROR, "ARS: ARSStart - Not a PMEM region, returning Invalid Param\n"));
    return (EFI_INVALID_PARAMETER);
  }

  GetSADLimit(SADBaseAddr, &Limit);

  mArsScrubData.NumDimmsLeft = 0;

  //
  // Check and see if this entire ARS Length falls with in one interleave set
  Status = GetDimmInfoFromSpa((Spa + ArsLength)-1, &SADNextBase, &Skt, &Ch, &Dimm);
  DEBUG((EFI_D_ERROR, "ARS: ARSStart - SADNextBase = 0x%1lx\n", SADNextBase));

  // if ((SADBaseAddr + Limit) < (Spa + ArsLength)) {
  if (SADBaseAddr != SADNextBase) {
    //
    // The given ARS range spawns over one SAD Rule.
    DEBUG((EFI_D_ERROR, "ARS: ARS Spawns over multiple SAD Rules\n"));
    MultipleSADs = TRUE;
  }

  if (MultipleSADs) {
    //
    // The first check is to make sure the entire ARS Range provided is in PMEM Region(s).
    SADSize = Limit - SADBaseAddr;
    SADNextBase = SADBaseAddr + SADSize;
    while (SADNextBase < (Spa + ArsLength)) {
      // Check and if the range is in the PMEM Region, if it isn't return error status
      if (!IsPmemRgn(&NvdimmInfo, SADNextBase)) {
        DEBUG((EFI_D_ERROR, "ARS: ARS Spawns over multiple SAD Rules and not all SAD Rules are PMEM Regions\n"));
        return (EFI_INVALID_PARAMETER);
      }
      // Get the limit of this SAD base.
      GetSADLimit(SADNextBase, &NextLimit);
      SADSize = NextLimit - SADNextBase;
      // Get the next SAD Base
      SADNextBase += SADSize;
    }
    // Reset all DPAs in the PMEM Data Structs.
    InitPMEMDpas();

    //
    // Now that we've ensured all PMEM Regions in this ARS Range,
    // Start processing for ARS Start Commands starting with the 1st SAD Rule
    // First is to get all the DpaStart and DpaEnd for all the dimms participating in the
    // ARS Range.
    i = 0;
    PMEMSADBaseAddresses[i] = SADBaseAddr;
    GetSADLimit(SADBaseAddr, &NextLimit);
    SADSize = NextLimit - SADBaseAddr;
    SpaStart = Spa;
    ArsSize = SADSize;
    ArsSize -= (SADSize - (NextLimit - SpaStart));  // Adjust ArsSize to based on SPA
    while ((SpaStart + ArsSize) <= (Spa + ArsLength)) {
      if (ArsSize == 0) {
        break;
      }  // Exit if finished processing SPA range
      Status = ComputeDPAsForARS(SpaStart, PMEMSADBaseAddresses[i], ArsSize);
      if (Status != EFI_SUCCESS) {
        return Status;
      }
      //
      // Get the next one, current one becomes the start of the next one
      i++;
      PMEMSADBaseAddresses[i] = SpaStart + ArsSize;
      SpaStart = PMEMSADBaseAddresses[i];
      // Get the next limit
      GetSADLimit(PMEMSADBaseAddresses[i], &NextLimit);
      ArsSize = NextLimit - PMEMSADBaseAddresses[i];
      if ((PMEMSADBaseAddresses[i] + ArsSize) > (Spa + ArsLength)) {
        Limit = (PMEMSADBaseAddresses[i] + ArsSize) - (Spa + ArsLength);
        NextLimit -= Limit;
        ArsSize = NextLimit - PMEMSADBaseAddresses[i];
      }
    }
  }
  else {
    // ARS Range is covered in one single SAD Rule.
    Status = GetDimmInfoFromSpa(Spa, &SADBaseAddr, &Skt, &Ch, &Dimm);
    Status = ComputeDPAsForARS(Spa, SADBaseAddr, ArsLength);
    if (Status != EFI_SUCCESS) {
      return Status;
    }
    i = 0;
    PMEMSADBaseAddresses[i] = SADBaseAddr;
  }

  Status = StartAddressRangeScrub(Spa, ArsLength);

  return Status;
}

/**

  HandleARSQueryStatus gathers Status from the previous Start
  ARS commands to dimms that were participating in the Start
  Address Range Scrub command from the OS and returns Status as
  specified in the DSM Spec.

  @return Status

**/
EFI_STATUS
HandleARSQueryStatus(
   VOID
   )
{
  EFI_STATUS      Status = QUERY_ARS_STS_SUCCESS;

  // Check if ARS is in progress
  if (mIsArs) {
    Status = EXT_STS_ARS_IN_PROGRESS;
  }

  // Check if ARS has been performed during this boot
  if (!mHasArsPerformed) {
    Status = EXT_STS_NO_ARS_PERFORMED;
  }

  if (mArsDsmHeader.Flags == 1) {
    Status = EXT_STS_ARS_STOPPED;
  }

  if ((Status == EXT_STS_NO_ARS_PERFORMED) || (Status == EXT_STS_ARS_IN_PROGRESS)) {
    mNvdimmAcpiSmmInterface->OutputLength = 0;
  }
  else {
    CopyMem(&mNvdimmAcpiSmmInterface->OutputBuffer[0], &mArsDsmHeader, mArsDsmHeader.OutputLength);
    mNvdimmAcpiSmmInterface->OutputLength = mArsDsmHeader.OutputLength;
  }

  return Status;
}

/**

  Clear uncorrectable error from the global list of error records

  @param ClearUcErrSpa - Starting location from which to clear the uncorrectable error
  @param ClearUcErrLen - Length of the region to clear the uncorrectable error from

  @return Status
  @return ActualLenCleared - The range of errors actually cleared by the platform, starting from the requested Clear Error SPA Base

**/
EFI_STATUS
ClearUncorrErr(
  IN  UINT64 ClearUcErrSpa,
  IN  UINT64 ClearUcErrLen,
  OUT UINT64 *ActualLenCleared
 )
{
  UINTN i = 0;
  UINTN j;

  // Make sure valid error records exist
  if (!mHasArsPerformed || (mArsDsmHeader.NumErrorRecords == 0)) {
    DEBUG ((DEBUG_ERROR, "[Clear Error] No error records exist!\n"));
    return CLR_UNCORR_ERR_STS_SUCCESS;
  }

  // Loop through error records and delete any that fall within the input range
  while(i < mArsDsmHeader.NumErrorRecords) {
    if ((mArsDsmHeader.ErrRecords[i].SpaOfErrLoc >= ClearUcErrSpa) &&
        (mArsDsmHeader.ErrRecords[i].SpaOfErrLoc + mArsDsmHeader.ErrRecords[i].Length) <= (ClearUcErrSpa + ClearUcErrLen)) {
      DEBUG ((DEBUG_ERROR, "[Clear Error] Clearing error record: SpaOfErrLoc = 0x%lX, Length = 0x%lX\n", mArsDsmHeader.ErrRecords[i].SpaOfErrLoc, mArsDsmHeader.ErrRecords[i].Length));
      *ActualLenCleared += mArsDsmHeader.ErrRecords[i].Length;
      for (j = i; j < (mArsDsmHeader.NumErrorRecords - 1); j++) mArsDsmHeader.ErrRecords[j] = mArsDsmHeader.ErrRecords[j + 1];
      mArsDsmHeader.NumErrorRecords--;
    }
    else {
      i++;
    }
  }

  return CLR_UNCORR_ERR_STS_SUCCESS;
}

/**

  HandleClearUncorrErr allows system software to clear uncorrectable errors
  from the NVDIMM based on System Physical Address (SPA). Uncorrectable
  errors reported by the Query ARS Status function can be cleared utilizing
  this mechanism.

  @return Status

**/
EFI_STATUS
HandleClearUncorrErr(
   IN UINT64    ClearUcErrSpa,
   IN UINT64    ClearUcErrLen
   )
{
  EFI_STATUS    Status;
  UINT64        ActualLenCleared = 0;   // This is the sum of all error record lengths cleared
  UINT64        PmemRegionStart = 0;
  UINT64        TotalPmemSize = 0;
  BOOLEAN       SpaOutOfRange = TRUE;
  UINT8         PmemIndex;
  UINT8         PmemIndex2;

  // Init return length to 0 since always valid regardless of status
  CopyMem(&mNvdimmAcpiSmmInterface->OutputBuffer[4], &ActualLenCleared, sizeof(UINT64));

  // Validate SPA range base is aligned to the Clear Uncorrectable Error Range Length Unit Size and
  // the length is an integer multiple of the Clear Uncorrectable Error Range Length Unit Size
  if (((ClearUcErrSpa % mClearUcErrLenUnit) != 0) || ((ClearUcErrLen % mClearUcErrLenUnit) != 0)) {
    return CLR_UNCORR_ERR_INVALID_PARAM;
  }

  // Validate SPA range falls within the PMEM regions
  for (PmemIndex = 0; PmemIndex < MAX_SOCKET; PmemIndex++) {
    if ((ClearUcErrSpa >= mPmemRegions[PmemIndex].BaseAddress) && (ClearUcErrSpa < (mPmemRegions[PmemIndex].BaseAddress + mPmemRegions[PmemIndex].Size))) {
      PmemRegionStart = mPmemRegions[PmemIndex].BaseAddress;
      TotalPmemSize = mPmemRegions[PmemIndex].Size;
      if (PmemIndex < (MAX_SOCKET - 1)) {
        for (PmemIndex2 = PmemIndex + 1; PmemIndex2 < MAX_SOCKET; PmemIndex2++) {
          if ((mPmemRegions[PmemIndex].BaseAddress + TotalPmemSize) == mPmemRegions[PmemIndex2].BaseAddress) {
            TotalPmemSize += mPmemRegions[PmemIndex2].Size;
          }
        }
      }
      if ((ClearUcErrSpa >= PmemRegionStart) && (ClearUcErrSpa + ClearUcErrLen) <= (PmemRegionStart + TotalPmemSize)) {
        SpaOutOfRange = FALSE;
      }
      break;
    }
  }
  if (SpaOutOfRange) {
    return CLR_UNCORR_ERR_INVALID_PARAM;
  }

  Status = ClearUncorrErr(ClearUcErrSpa, ClearUcErrLen, &ActualLenCleared);

  CopyMem(&mNvdimmAcpiSmmInterface->OutputBuffer[4], &ActualLenCleared, sizeof(UINT64));
  mNvdimmAcpiSmmInterface->OutputLength = sizeof(UINT64);

  return EFI_SUCCESS;
}

/**

  Handles ARS complete event

  @param NodeId  - Memory controller ID

  @return Status

**/
EFI_STATUS
HandleArsCompleteEvent(
   IN  UINT8 NodeId
)
{
  EFI_STATUS                      Status = EFI_SUCCESS;
  SCRUBCTL_MC_MAIN_STRUCT         ScrubCtlReg;
  SCRUBADDRESSLO_MC_MAIN_STRUCT   ScrubAddrLoReg;
  SCRUBADDRESSHI_MC_MAIN_STRUCT   ScrubAddrHiReg;
  UINT8                           Socket = NODE_TO_SKT(NodeId);
  UINT8                           Mc = NODE_TO_MC(NodeId);
  UINT8                           Node;

  ScrubCtlReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBCTL_MC_MAIN_REG);
  DEBUG((DEBUG_ERROR, "[ARS Event] ScrubCtlReg on node %d, socket %d, Imc %d = %X\n", NodeId, Socket, Mc, ScrubCtlReg.Data));
  ScrubAddrLoReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBADDRESSLO_MC_MAIN_REG);
  DEBUG((DEBUG_ERROR, "[ARS Event] ScrubAddrLoReg on node, socket %d, Imc %d %d = %X\n", NodeId, Socket, Mc, ScrubAddrLoReg.Data));
  ScrubAddrHiReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBADDRESSHI_MC_MAIN_REG);
  DEBUG((DEBUG_ERROR, "[ARS Event] ScrubAddrHiReg on node, socket %d, Imc %d %d = %X\n", NodeId, Socket, Mc, ScrubAddrHiReg.Data));

  // Disable scrub
  ScrubCtlReg.Bits.ptl_cmpl = 0;
  ScrubCtlReg.Bits.stop_on_cmpl  = 0;
  ScrubCtlReg.Bits.stop_on_err = 0;
  ScrubCtlReg.Bits.scrub_en  = 0;
  mCpuCsrAccess->WriteMcCpuCsr(Socket, Mc, SCRUBCTL_MC_MAIN_REG, ScrubCtlReg.Data);

  if (mArsScrubData.SizeLeftToScrub) {
    Status = DoScrub();
  }
  else {
    // ARS complete, re-enable any previous scrubs
    DEBUG((DEBUG_ERROR, "[ARS Event] ARS has completed\n"));
    mIsArs = FALSE;

    // Restore PCU setting for patrol scrub
    SwitchAddressModeToSystem (NodeId);

    Status = PcuSetScrubInterval(Socket, Mc, *mScrubIntervalSave);
    ASSERT_EFI_ERROR(Status);

    for (Socket = 0; Socket < MAX_SOCKET; Socket++){
      for (Mc = 0; Mc < MAX_IMC; Mc++) {
        if (!mSystemMemoryMap->Socket[Socket].imcEnabled[Mc]){
          continue;
        }
        Node = SKTMC_TO_NODE(Socket, Mc);
        ScrubCtlReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBCTL_MC_MAIN_REG);
        if (mArsScrubData.CurrentNodeScrub[Node]) {
          // Restore MCMAIN_CHKN_BITS_MC_MAIN_REG and re-enable scrub for each node it was running
          mCpuCsrAccess->WriteMcCpuCsr(Socket, Mc, MCMAIN_CHKN_BITS_MC_MAIN_REG, mMcMainChknRegSave[Node].Data);
          ReEnablePatrolScrubEngine(Node);
        }
      }
    }
  }

  return Status;
}

/**

  Handles ARS error event

  @param NodeId  - Memory controller ID

  @return Status

**/
EFI_STATUS
HandleArsErrorEvent(
    IN  UINT8 NodeId
)
{
  EFI_STATUS                                Status = EFI_SUCCESS;
  SCRUBCTL_MC_MAIN_STRUCT                   ScrubCtlReg;
  SCRUBADDRESSLO_MC_MAIN_STRUCT             ScrubAddrLoReg;
  SCRUBADDRESSHI_MC_MAIN_STRUCT             ScrubAddrHiReg;
  ERROR_RECORD                              *ErrorRecord;
  NVDIMM                                    *pDimm = NULL;
  UINT64                                    SystemAddress;
  UINT64                                    DpaStart;
  //UINT32                                    PcodeMailboxData;
  UINT8                                     Node;
  UINTN                                     CurrRecord = 0;
  UINT8                                     Socket = NODE_TO_SKT(NodeId);
  UINT8                                     Mc = NODE_TO_MC(NodeId);
  UINT8                                     Channel;
  UINT8                                     Dimm;

  // Get current ARS address that triggered the error
  ScrubAddrLoReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBADDRESSLO_MC_MAIN_REG);
  ScrubAddrHiReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBADDRESSHI_MC_MAIN_REG);

  // TODO: figure out how to get Ch, Dimm, and DpaStart correctly without MemRas conversion funcs
  Channel = (UINT8)ScrubAddrHiReg.Bits.chnl;
  DpaStart = ScrubAddrLoReg.Bits.rankadd << 6;
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
#if VERBOSE_DEBUG
    DEBUG((DEBUG_ERROR, "[ARS Error] GetDimm socket=%d, ch %d dimm %d: \n", Socket, Channel, Dimm));
#endif
    pDimm = GetDimm (&NvdimmInfo, Socket, NODECH_TO_SKTCH(NodeId, Channel), Dimm, NVDIMM_FLAG_ALL);
    if (pDimm == NULL) {
      continue;
    } else {
      break;
    }
  }
  if (pDimm != NULL) {
#if VERBOSE_DEBUG
   DEBUG((DEBUG_ERROR, "[ARS Error] pDimm->SADSpaBase = 0x%lx, DpaStart = 0x%x\n", pDimm->SADSpaBase, DpaStart));
#endif
   Status = XlateDpaToSpa(pDimm, pDimm->SADSpaBase, DpaStart, &SystemAddress);
  } else {
    return EFI_INVALID_PARAMETER;
  }
  DEBUG((DEBUG_ERROR, "[ARS Error] Error at address: 0x%lX\n", SystemAddress));
  DEBUG((DEBUG_ERROR, "[ARS Error] Error at DIMM: Socket = %X, McId = %X, Ch = %X, Dimm = %X\n", Socket, Mc, Channel, Dimm));

  // Check for overflow condition
  if (mArsDsmHeader.NumErrorRecords >= (MAX_NVDIMMS * MAX_ERRORS_PER_DIMM)) {
    DEBUG((DEBUG_ERROR, "[ARS Error] Too many error records! Stopping scrub...\n"));
    mArsDsmHeader.Flags = 1;

    // Set restart SPA and length
    mArsDsmHeader.RestartArsStartSpa = SystemAddress;
    mArsDsmHeader.RestartArsLength = (mArsDsmHeader.StartSpa + mArsDsmHeader.Length) - SystemAddress;

    // Disable scrub
    ScrubCtlReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBCTL_MC_MAIN_REG);
    ScrubCtlReg.Bits.ptl_cmpl = 0;
    ScrubCtlReg.Bits.stop_on_cmpl  = 0;
    ScrubCtlReg.Bits.stop_on_err = 0;
    ScrubCtlReg.Bits.scrub_en  = 0;
    mCpuCsrAccess->WriteMcCpuCsr(Socket, Mc, SCRUBCTL_MC_MAIN_REG, ScrubCtlReg.Data);

    // Stop ARS, re-enable any previous scrubs
    mIsArs = FALSE;

    // Restore PCU setting for patrol scrub
    SwitchAddressModeToSystem (NodeId);

    Status = PcuSetScrubInterval(Socket, Mc, *mScrubIntervalSave);
    ASSERT_EFI_ERROR(Status);

    for (Socket = 0; Socket < MAX_SOCKET; Socket++){
      for (Mc = 0; Mc < MAX_IMC; Mc++) {
        if (!mSystemMemoryMap->Socket[Socket].imcEnabled[Mc]){
          continue;
        }
        Node = SKTMC_TO_NODE(Socket, Mc);
        ScrubCtlReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBCTL_MC_MAIN_REG);
        if (mArsScrubData.CurrentNodeScrub[Node]) {
          // Restore MCMAIN_CHKN_BITS_MC_MAIN_REG and re-enable scrub for each node it was running
          mCpuCsrAccess->WriteMcCpuCsr(Socket, Mc, MCMAIN_CHKN_BITS_MC_MAIN_REG, mMcMainChknRegSave[Node].Data);
          ReEnablePatrolScrubEngine(Node);
        }
      }
    }
    return EFI_INVALID_PARAMETER;
  }

  // Check for adjacent error
  while (CurrRecord < mArsDsmHeader.NumErrorRecords) {
    ErrorRecord = &mArsDsmHeader.ErrRecords[CurrRecord];
    if (SystemAddress == (ErrorRecord->SpaOfErrLoc + ErrorRecord->Length)) {
      break;
    }
    CurrRecord++;
  }

  // If no match found then create new error record
  if (CurrRecord == mArsDsmHeader.NumErrorRecords) {
    mNumErrorsCurrRank++;
    if (mNumErrorsCurrRank > mMaxErrorsPerRank) {
      // Reached max errors per rank, move to next rank
      return HandleArsCompleteEvent(NodeId);
    }

    ErrorRecord = &mArsDsmHeader.ErrRecords[mArsDsmHeader.NumErrorRecords];
    ErrorRecord->NfitHandle = (Socket << 12) | (Mc << 8) | (Channel << 4) | Dimm;
    ErrorRecord->Flags = 0;
    ErrorRecord->SpaOfErrLoc = SystemAddress;
    ErrorRecord->Length = 1; // TODO: is length 1 byte or whole rank?

    mArsDsmHeader.NumErrorRecords++;                      // Increment total error record count
    mArsDsmHeader.OutputLength += sizeof (ERROR_RECORD);  // Update the Ars Dsm struct length to account for the Error Record just added
  }
  // Adjacent error found, update previous error record
  else {
    ErrorRecord = &mArsDsmHeader.ErrRecords[CurrRecord];
    ErrorRecord->Length += 1; // TODO: is length 1 byte or whole rank?
  }

  // Restart scrub
  mCpuCsrAccess->WriteMcCpuCsr (Socket, Mc, SCRUBADDRESSLO_MC_MAIN_REG,(ScrubAddrLoReg.Data + 1));
  ScrubCtlReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBCTL_MC_MAIN_REG);
  ScrubCtlReg.Bits.ptl_stopped = 0;
  ScrubCtlReg.Bits.startscrub = 1;
  mCpuCsrAccess->WriteMcCpuCsr(Socket, Mc, SCRUBCTL_MC_MAIN_REG, ScrubCtlReg.Data);

  return Status;
}

/**

  Checks and handles when ARS has completed or encountered and error

  @param DispatchHandle - Dispatch handle (unused for now!)
  @param Dispatch Context - Context information (unused for now!)
  @param CommBuffer     - Buffer used for communication between caller/callback function (unused for now!)
  @param CommBufferSize - size of communication buffer (unused for now!)

  @retval EFI_SUCCESS / Error code

**/
EFI_STATUS
EFIAPI
CheckAndHandleArsEvent(
   IN EFI_HANDLE        DispatchHandle,
   IN CONST VOID        *DispatchContext,
   IN OUT   VOID        *CommBuffer,
   IN OUT   UINTN       *CommBufferSize
   )
{
  UINT8                       Socket;
  UINT8                       Mc;
  UINT8                       Node;
  EFI_STATUS                  Status = EFI_SUCCESS;
  SCRUBCTL_MC_MAIN_STRUCT     ScrubCtlReg;

  if (!mIsArs) {
    return Status;
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++){
    for (Mc = 0; Mc < MAX_IMC; Mc++) {
      if (!mSystemMemoryMap->Socket[Socket].imcEnabled[Mc]){
        continue;
      }

      Node = SKTMC_TO_NODE(Socket, Mc);
      ScrubCtlReg.Data = mCpuCsrAccess->ReadMcCpuCsr(Socket, Mc, SCRUBCTL_MC_MAIN_REG);
      if(ScrubCtlReg.Bits.scrub_en) {
        if(ScrubCtlReg.Bits.ptl_cmpl){
          Status = HandleArsCompleteEvent(Node);
        }
        if(ScrubCtlReg.Bits.ptl_stopped){
          DEBUG((DEBUG_ERROR, "[ARS Event] ARS encountered an error\n"));
          Status = HandleArsErrorEvent(Node);
        }
      }
    }
  }
  return Status;
}

/**

  This routine handles Address Range Scrub related commands from
  OS.

  @param VOID - No direct input

  @return Status

**/
EFI_STATUS
HandleARSCommandsFromOS(
   VOID
   )
{
  UINT32          *pBuf;
  UINT64          ArsLength;
  EFI_STATUS      Status = ARS_INVALID_PARAM;

  switch (mNvdimmAcpiSmmInterface->FunctionIndex) {
  case 1:
    // Query ARS Capabilities - This should return success status and the extended status bytes
    // should just have ARS Enabled for PMEM bit set and set the Output size and Bios SMM Buffer size in it.
    Status = PMEM_SCRUB_SUPPORTED;
    mNvdimmAcpiSmmInterface->Status = (UINT32)(Status << 16) | (EFI_SUCCESS);
    mNvdimmAcpiSmmInterface->OutputLength = sizeof(UINT64);
    pBuf = (UINT32 *)&mNvdimmAcpiSmmInterface->OutputBuffer[0];
    *pBuf = OUTPUT_BUF_SIZE;
    *(pBuf + 1) = mClearUcErrLenUnit;     // Clear Uncorrectable Error Range Length Unit Size
    break;

  case 2:
    // Start Address Range Scrubbing
    // We only PMEM Scrub, not volatile scrub
    if (mNvdimmAcpiSmmInterface->ArsType & VOLATILE_SCRUB_SUPPORTED) {
      Status = ARS_INVALID_PARAM;
    }
    else {
      ArsStartSpa = mNvdimmAcpiSmmInterface->ArsStartAddress;
      // Align the Spa to 256 byte boundary
      ArsStartSpa = (ArsStartSpa / ARS_ALIGNMENT_SIZE) * ARS_ALIGNMENT_SIZE;
      ArsLength = mNvdimmAcpiSmmInterface->ArsLength;
      Status = HandleARSStart(ArsStartSpa, mNvdimmAcpiSmmInterface->ArsLength);
    }
    mNvdimmAcpiSmmInterface->Status = (UINT32)Status;
    mNvdimmAcpiSmmInterface->OutputLength = sizeof(UINT32);
    pBuf = (UINT32 *)&mNvdimmAcpiSmmInterface->OutputBuffer[0];
    *pBuf = mArsTimeEstimate;
    break;

  case 3:
    // Query Address Range Scrubbing Status
    Status = HandleARSQueryStatus();
    mNvdimmAcpiSmmInterface->Status = (UINT32)Status;
    // Update Specific Area in ACPI with data, is done in the above handler
    // Give the status back what was returned.
    break;

  case 4:
    // Clear Uncorrectable Error
    // TODO: use ArsStartAddress & ArsLength?? Might need to update _DSM to include this new function
    Status = HandleClearUncorrErr(mNvdimmAcpiSmmInterface->ArsStartAddress, mNvdimmAcpiSmmInterface->ArsLength);
    mNvdimmAcpiSmmInterface->Status = (UINT32)Status;
    break;

  default:
    Status = ARS_INVALID_PARAM;
    mNvdimmAcpiSmmInterface->Status = (UINT32)Status;
    break;
  }

  return Status;
}

VOID
DSM_Map_NCH (
  JEDEC_NVDIMM *Nvdimm
  )
{
  DSM_FNOUT_NCH   *FnOutPtr;
  UINT8           ModuleHealthReg = 0;
  UINT8           ModuleHealthStatus0Reg = 0;
  UINT8           ModuleHealthStatus1Reg = 0;
  UINT8           ErrorThresholdStatusReg = 0;
  UINT8           WarningThresholdStatusReg = 0;
  UINT8           CsaveInfoReg = 0;
  EFI_STATUS      Status = EFI_SUCCESS;

  mNvdimmAcpiSmmInterface->OutputLength = sizeof (*FnOutPtr);
  FnOutPtr = (DSM_FNOUT_NCH *)mNvdimmAcpiSmmInterface->OutputBuffer;
  ZeroMem (FnOutPtr, sizeof (*FnOutPtr));

  if (mNvdimmAcpiSmmInterface->InputLength != 0) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NCH: Invalid parameter. InputLength is: %d.\n", mNvdimmAcpiSmmInterface->InputLength));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_INVALID_INPUT_PARAMETERS;
    return;
  }

  // Used registers are compliant with 2.1 and above
  if (Nvdimm->DimmId.SpecificationRevision < SPEC_REVISION_2_1) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NCH: Specification revision is %x. Should be 2.1 or above.\n", Nvdimm->DimmId.SpecificationRevision));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_NOT_IMPLEMENTED;
    return;
  }

  Status = JedecSwitchPage(Nvdimm, 0);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NCH: JedecSwitchPage() return: %d\n", Status));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_RETRY_SUGGESTED;
    return;
  }

  Status = ReadSmb(Nvdimm, MODULE_HEALTH, &ModuleHealthReg);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NCH: ReadSmb() for MODULE_HEALTH return: %d.\n", Status));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_RETRY_SUGGESTED;
    return;
  }
  Status = ReadSmb(Nvdimm, MODULE_HEALTH_STATUS0, &ModuleHealthStatus0Reg);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NCH: ReadSmb() for MODULE_HEALTH_STATUS0 return: %d.\n", Status));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_RETRY_SUGGESTED;
    return;
  }
  Status = ReadSmb(Nvdimm, MODULE_HEALTH_STATUS1, &ModuleHealthStatus1Reg);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NCH: ReadSmb() for MODULE_HEALTH_STATUS1 return: %d.\n", Status));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_RETRY_SUGGESTED;
    return;
  }
  Status = ReadSmb(Nvdimm, ERROR_THRESHOLD_STATUS, &ErrorThresholdStatusReg);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NCH: ReadSmb() for ERROR_THRESHOLD_STATUS return: %d.\n", Status));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_RETRY_SUGGESTED;
    return;
  }
  Status = ReadSmb(Nvdimm, WARNING_THRESHOLD_STATUS, &WarningThresholdStatusReg);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NCH: ReadSmb() for WARNING_THRESHOLD_STATUS return: %d.\n", Status));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_RETRY_SUGGESTED;
    return;
  }
  Status = ReadSmb(Nvdimm, CSAVE_INFO, &CsaveInfoReg);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NCH: ReadSmb() for CSAVE_INFO return: %d.\n", Status));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_RETRY_SUGGESTED;
    return;
  }

  // Persistent condition
  if (ModuleHealthStatus0Reg & VOLTAGE_REGULATOR_FAILED ||
  ModuleHealthStatus0Reg & VDD_LOST ||
  ModuleHealthStatus0Reg & VPP_LOST ||
  ModuleHealthStatus0Reg & VTT_LOST) {
    FnOutPtr->HealthStatus.WritePersistencyPowerLoss = 1;
    FnOutPtr->HealthStatus.WritePersistencyLossOffline = 1;
    FnOutPtr->HealthStatus.WritePersistencyLossImminent = 1;
    FnOutPtr->HealthStatus.PermanentHealthCondition = 1;
    if ((CsaveInfoReg & NVM_DATA_VALID) == 0) {
      FnOutPtr->HealthStatus.AllDataLossOffline = 1;
      FnOutPtr->HealthStatus.AllDataLossPowerLoss = 1;
      FnOutPtr->HealthStatus.AllDataLossOffline = 1;
      FnOutPtr->HealthStatus.AllDataLossImminent = 1;
    }
  }

  if (ModuleHealthStatus0Reg & NVM_CONTROLLER_ERROR ||
  ModuleHealthStatus0Reg & NVM_LIFETIME_ERROR ||
  ModuleHealthReg & PERMANENT_HARDWARE_FAILURE) {
    FnOutPtr->HealthStatus.PermanentHealthCondition = 1;
    FnOutPtr->HealthStatus.WritePersistencyPowerLoss = 1;
    FnOutPtr->HealthStatus.WritePersistencyLossOffline = 1;
    FnOutPtr->HealthStatus.AllDataLossPowerLoss = 1;
    FnOutPtr->HealthStatus.AllDataLossOffline = 1;
  }

  if (ModuleHealthStatus0Reg & CONTROLLER_HARDWARE_ERROR ||
  ModuleHealthStatus1Reg & ES_HARDWARE_FAILURE ||
  ModuleHealthStatus1Reg & ES_HEALTH_ASSESSMENT_ERROR ||
  ErrorThresholdStatusReg & NVM_LIFETIME_ERROR ||
  ErrorThresholdStatusReg & ES_LIFETIME_ERROR ||
  ModuleHealthReg & PERSISTENCY_LOST_ERROR) {
    FnOutPtr->HealthStatus.PermanentHealthCondition = 1;
    FnOutPtr->HealthStatus.WritePersistencyPowerLoss = 1;
    FnOutPtr->HealthStatus.WritePersistencyLossOffline = 1;
    if ((CsaveInfoReg & NVM_DATA_VALID) == 0) {
      FnOutPtr->HealthStatus.AllDataLossPowerLoss = 1;
      FnOutPtr->HealthStatus.AllDataLossOffline = 1;
    }
  }

  if (ModuleHealthStatus1Reg & CONFIG_DATA_ERROR) {
    FnOutPtr->HealthStatus.PermanentHealthCondition = 1;
    FnOutPtr->HealthStatus.AllDataLossPowerLoss = 1;
    FnOutPtr->HealthStatus.AllDataLossOffline = 1;
    FnOutPtr->HealthStatus.AllDataLossImminent = 1;
  }

  if (WarningThresholdStatusReg & NVM_LIFETIME_WARNING ||
  WarningThresholdStatusReg & ES_LIFETIME_WARNING) {
    FnOutPtr->HealthStatus.PermanentHealthCondition = 1;
    FnOutPtr->HealthStatus.MaintenanceNeeded = 1;
  }

  if (ModuleHealthReg & WARNING_THRESHOLD_EXCEEDED) {
    FnOutPtr->HealthStatus.PermanentHealthCondition = 1;
    FnOutPtr->HealthStatus.MaintenanceNeeded = 1;
  }

  //Not persistent condition
  if (ModuleHealthStatus1Reg & NOT_ENOUGH_ENERGY_FOR_CSAVE ||
  ModuleHealthStatus1Reg & NO_ES_PRESENT ||
  ModuleHealthStatus1Reg & ES_POLICY_NOT_SET ||
  ErrorThresholdStatusReg & ES_TEMP_ERROR) {
    FnOutPtr->HealthStatus.WritePersistencyPowerLoss = 1;
    FnOutPtr->HealthStatus.WritePersistencyLossOffline = 1;
    if ((CsaveInfoReg & NVM_DATA_VALID) == 0) {
      FnOutPtr->HealthStatus.AllDataLossPowerLoss = 1;
      FnOutPtr->HealthStatus.AllDataLossOffline = 1;
    }
  }

  if (ModuleHealthStatus1Reg & INVALID_FIRMWARE_ERROR) {
    FnOutPtr->HealthStatus.WritePersistencyPowerLoss = 1;
    FnOutPtr->HealthStatus.WritePersistencyLossOffline = 1;
    FnOutPtr->HealthStatus.WritePersistencyLossImminent = 1;
  }

  if (WarningThresholdStatusReg & ES_TEMP_WARNING) {
    FnOutPtr->HealthStatus.MaintenanceNeeded = 1;
  }

  FnOutPtr->OverallHealthStatusAttrValid = 1;
  FnOutPtr->OverallHealthStatusFlagValid = 1;
  mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_SUCCESS;
}

VOID
DSM_Map_NBS (
  JEDEC_NVDIMM *Nvdimm
  )
{
  DSM_FNOUT_NBS   *FnOutPtr;
  UINT8           CsaveFailureCount0 = 0;
  UINT8           CsaveFailureCount1 = 0;
  EFI_STATUS      Status = EFI_SUCCESS;

  mNvdimmAcpiSmmInterface->OutputLength = sizeof (*FnOutPtr);
  FnOutPtr = (DSM_FNOUT_NBS *)mNvdimmAcpiSmmInterface->OutputBuffer;
  ZeroMem (FnOutPtr, sizeof (*FnOutPtr));

  if (mNvdimmAcpiSmmInterface->InputLength != 0) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NBS: Invalid parameter. InputLength is: %d.\n", mNvdimmAcpiSmmInterface->InputLength));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_INVALID_INPUT_PARAMETERS;
    return;
  }

  // Used registers are compliant with 2.1 and above
  if (Nvdimm->DimmId.SpecificationRevision < SPEC_REVISION_2_1) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NBS: Specification revision is %x. Should be 2.1 or above.\n", Nvdimm->DimmId.SpecificationRevision));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_NOT_IMPLEMENTED;
    return;
  }

  Status = JedecSwitchPage(Nvdimm, 2);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NBS: JedecSwitchPage() return: %d\n", Status));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_RETRY_SUGGESTED;
    return;
  }
  Status = ReadSmb(Nvdimm, CSAVE_FAILURE_COUNT0, &CsaveFailureCount0);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NBS: ReadSmb() for CSAVE_FAILURE_COUNT0 return: %d\n", Status));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_RETRY_SUGGESTED;
    return;
  }
  Status = ReadSmb(Nvdimm, CSAVE_FAILURE_COUNT1, &CsaveFailureCount1);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NBS: ReadSmb() for CSAVE_FAILURE_COUNT1 return: %d\n", Status));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_RETRY_SUGGESTED;
    return;
  }

  // CsaveFailureCount1 provides the most significant byte and CsaveFailureCount0 provides the least significant byte
  FnOutPtr->DataLossCount = ((UINT16) CsaveFailureCount1 << 8) | CsaveFailureCount0;
  FnOutPtr->DataLossCountValid = 1;
  mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_SUCCESS;

}

VOID
DSM_Map_NIC (
  JEDEC_NVDIMM *Nvdimm
  )
{
  DSM_FNOUT_NIC   *FnOutPtr;

  mNvdimmAcpiSmmInterface->OutputLength = sizeof (*FnOutPtr);
  FnOutPtr = (DSM_FNOUT_NIC *)mNvdimmAcpiSmmInterface->OutputBuffer;
  ZeroMem (FnOutPtr, sizeof (*FnOutPtr));

  if (mNvdimmAcpiSmmInterface->InputLength != 0) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NIC: Invalid parameter. InputLength is: %d.\n", mNvdimmAcpiSmmInterface->InputLength));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_INVALID_INPUT_PARAMETERS;
    return;
  }

  // Used registers are compliant with 2.1 and above
  if (Nvdimm->DimmId.SpecificationRevision < SPEC_REVISION_2_1) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NIC: Specification revision is %x. Should be 2.1 or above.\n", Nvdimm->DimmId.SpecificationRevision));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_NOT_IMPLEMENTED;
    return;
  }

  // _NIH/_NIC/_NIG are supported only if error injection is supported
  if ((Nvdimm->DimmId.Capabilities & ERROR_INJECTION_SUPPORTED) == 0) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NIC: Error Injection is not supported.\n"));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_ERROR_UNKNOWN_REASON;
  }

  FnOutPtr->HealthStatus.PermanentHealthCondition = 1;
  FnOutPtr->HealthStatus.WritePersistencyPowerLoss = 1;
  FnOutPtr->HealthStatus.WritePersistencyLossOffline = 1;
  FnOutPtr->HealthStatus.MaintenanceNeeded = 1;
  FnOutPtr->HealthStatus.AllDataLossPowerLoss = 1;
  FnOutPtr->HealthStatus.AllDataLossOffline = 1;

  mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_SUCCESS;
}

VOID
DSM_Map_NIG (
  JEDEC_NVDIMM *Nvdimm
  )
{
  DSM_FNOUT_NIG   *FnOutPtr;
  UINT8   InjectOpsFailures0Reg = 0;
  UINT8   InjectEsFailuresReg = 0;
  EFI_STATUS Status = EFI_SUCCESS;

  mNvdimmAcpiSmmInterface->OutputLength = sizeof (*FnOutPtr);
  FnOutPtr = (DSM_FNOUT_NIG *)mNvdimmAcpiSmmInterface->OutputBuffer;
  ZeroMem (FnOutPtr, sizeof (*FnOutPtr));

  if (mNvdimmAcpiSmmInterface->InputLength != 0) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NIG: Invalid parameter. InputLength is: %d.\n", mNvdimmAcpiSmmInterface->InputLength));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_INVALID_INPUT_PARAMETERS;
    return;
  }

  // Used registers are compliant with 2.1 and above
  if (Nvdimm->DimmId.SpecificationRevision < SPEC_REVISION_2_1) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NIG: Specification revision is %x. Should be 2.1 or above.\n", Nvdimm->DimmId.SpecificationRevision));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_NOT_IMPLEMENTED;
    return;
  }

  // _NIH/_NIC/_NIG are supported only if error injection is supported
  if ((Nvdimm->DimmId.Capabilities & ERROR_INJECTION_SUPPORTED) == 0) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NIG: Error Injection is not supported.\n"));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_ERROR_UNKNOWN_REASON;
  }

  Status = JedecSwitchPage(Nvdimm, 2);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NIG: JedecSwitchPage() return: %d\n", Status));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_RETRY_SUGGESTED;
    return;
  }

  Status = ReadSmb(Nvdimm, INJECT_ES_FAILURES, &InjectEsFailuresReg);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NIG: ReadSmb() for INJECT_ES_FAILURES return: %d.\n", Status));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_RETRY_SUGGESTED;
    return;
  }
  Status = ReadSmb(Nvdimm, INJECT_OPS_FAILURES0, &InjectOpsFailures0Reg);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NIG: ReadSmb() for INJECT_OPS_FAILURES0 return: %d.\n", Status));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_RETRY_SUGGESTED;
    return;
  }

  if (InjectEsFailuresReg & INJECT_ES_FAILURE ||
  InjectEsFailuresReg & INJECT_ES_ASSESEMENT_FAILURE ||
  InjectEsFailuresReg & INJECT_ES_LIFETIME_ERROR) {
    FnOutPtr->HealthStatus.PermanentHealthCondition = 1;
    FnOutPtr->HealthStatus.WritePersistencyPowerLoss = 1;
    FnOutPtr->HealthStatus.WritePersistencyLossOffline = 1;
  }

  if (InjectEsFailuresReg & INJECT_ES_LIFETIME_WARNING ||
  InjectOpsFailures0Reg & INJECT_NVM_LIFETIME_WARNING) {
    FnOutPtr->HealthStatus.PermanentHealthCondition = 1;
    FnOutPtr->HealthStatus.MaintenanceNeeded = 1;
  }

  if (InjectOpsFailures0Reg & INJECT_INTERNAL_CONTROLLER_FAILURE ||
  InjectOpsFailures0Reg & INJECT_NVM_LIFETIME_ERROR) {
    FnOutPtr->HealthStatus.PermanentHealthCondition = 1;
    FnOutPtr->HealthStatus.WritePersistencyPowerLoss = 1;
    FnOutPtr->HealthStatus.WritePersistencyLossOffline = 1;
    FnOutPtr->HealthStatus.AllDataLossPowerLoss = 1;
    FnOutPtr->HealthStatus.AllDataLossOffline = 1;
  }

  if (InjectEsFailuresReg & INJECT_ES_TEMP_ERROR) {
    FnOutPtr->HealthStatus.WritePersistencyPowerLoss = 1;
    FnOutPtr->HealthStatus.WritePersistencyLossOffline = 1;
  }

  if (InjectEsFailuresReg & INJECT_ES_TEMP_WARNING) {
    FnOutPtr->HealthStatus.MaintenanceNeeded = 1;
  }

  mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_SUCCESS;
  FnOutPtr->OverallHealthStatusAttrValid = 1;
  FnOutPtr->OverallHealthStatusFlagValid = 1;
}

VOID
DSM_Map_NIH (
  JEDEC_NVDIMM *Nvdimm
  )
{
  DSM_FNINP_NIH   *FnInpPtr;
  UINT8           InjectOpsFailures0Reg = 0;
  UINT8           InjectEsFailuresReg = 0;
  UINT8           InjectErrorTypeReg = 0;
  EFI_STATUS      Status = EFI_SUCCESS;

  mNvdimmAcpiSmmInterface->OutputLength = 0;
  ZeroMem (mNvdimmAcpiSmmInterface->OutputBuffer, sizeof (*mNvdimmAcpiSmmInterface->OutputBuffer));
  if (mNvdimmAcpiSmmInterface->InputLength != sizeof (*FnInpPtr)) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NIH: Invalid size of input parameter: %d\n", mNvdimmAcpiSmmInterface->InputLength));
    mNvdimmAcpiSmmInterface->Status = (UINT32)NVDIMM_STS_INVALID_INPUT_PARAMETERS;
    return;
  }

  // Used registers are compliant with 2.1 and above
  if (Nvdimm->DimmId.SpecificationRevision < SPEC_REVISION_2_1) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NIH: Specification revision is %x. Should be 2.1 or above.\n", Nvdimm->DimmId.SpecificationRevision));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_NOT_IMPLEMENTED;
    return;
  }

  // _NIH/_NIC/_NIG are supported only if error injection is supported
  if ((Nvdimm->DimmId.Capabilities & ERROR_INJECTION_SUPPORTED) == 0) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NIH: Error Injection is not supported.\n"));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_ERROR_UNKNOWN_REASON;
  }

  Status = JedecSwitchPage(Nvdimm, 2);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NIH: JedecSwitchPage() return: %d\n", Status));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_RETRY_SUGGESTED;
    return;
  }

  if (Nvdimm->ESInfo.ESPolicy == ESINFO_DEVICE_MANAGED_POLICY) {
    Status = ReadSmb(Nvdimm, INJECT_ES_FAILURES, &InjectEsFailuresReg);
    if (Status != EFI_SUCCESS) {
      DEBUG((EFI_D_ERROR, "DSM_Map_NIH: ReadSmb() for INJECT_ES_FAILURES return: %d\n", Status));
      mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_RETRY_SUGGESTED;
      return;
    }
  }
  Status = ReadSmb(Nvdimm, INJECT_OPS_FAILURES0, &InjectOpsFailures0Reg);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NIH: ReadSmb() for INJECT_OPS_FAILURES0 return: %d\n", Status));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_RETRY_SUGGESTED;
    return;
  }

  FnInpPtr = (DSM_FNINP_NIH *)mNvdimmAcpiSmmInterface->InputBuffer;

  if (FnInpPtr->HealthStatus.PermanentHealthCondition &&
  FnInpPtr->HealthStatus.WritePersistencyPowerLoss &&
  FnInpPtr->HealthStatus.WritePersistencyLossOffline) {
    if (FnInpPtr->Mode == NIH_INPUT_MODE_INJECT_ERRORS) {
      InjectEsFailuresReg |= INJECT_ES_FAILURE;
    } else if (FnInpPtr->Mode == NIH_INPUT_MODE_CLEAR_ERRORS) {
      InjectEsFailuresReg &= ~INJECT_ES_FAILURE;
    } else { // 0 and 3-255 Reserved
      DEBUG((EFI_D_ERROR, "DSM_Map_NIH: Invalid Mode input parameter: %d\n", FnInpPtr->Mode));
      mNvdimmAcpiSmmInterface->Status = (UINT32)NVDIMM_STS_INVALID_INPUT_PARAMETERS;
      return;
    }
  }

  if (FnInpPtr->HealthStatus.PermanentHealthCondition && FnInpPtr->HealthStatus.MaintenanceNeeded) {
    if (FnInpPtr->Mode == NIH_INPUT_MODE_INJECT_ERRORS) {
      InjectEsFailuresReg |= INJECT_ES_LIFETIME_WARNING;
    } else if (FnInpPtr->Mode == NIH_INPUT_MODE_CLEAR_ERRORS) {
      InjectEsFailuresReg &= ~INJECT_ES_LIFETIME_WARNING;
    } else { // 0 and 3-255 Reserved
      DEBUG((EFI_D_ERROR, "DSM_Map_NIH: Invalid Mode input parameter: %d\n", FnInpPtr->Mode));
      mNvdimmAcpiSmmInterface->Status = (UINT32)NVDIMM_STS_INVALID_INPUT_PARAMETERS;
      return;
    }
  }

  if (FnInpPtr->HealthStatus.PermanentHealthCondition &&
  FnInpPtr->HealthStatus.WritePersistencyPowerLoss &&
  FnInpPtr->HealthStatus.WritePersistencyLossOffline &&
  FnInpPtr->HealthStatus.AllDataLossPowerLoss &&
  FnInpPtr->HealthStatus.AllDataLossOffline) {
    if (FnInpPtr->Mode == NIH_INPUT_MODE_INJECT_ERRORS) {
      InjectOpsFailures0Reg |= INJECT_INTERNAL_CONTROLLER_FAILURE;
    } else if (FnInpPtr->Mode == NIH_INPUT_MODE_CLEAR_ERRORS) {
      InjectOpsFailures0Reg &= ~INJECT_INTERNAL_CONTROLLER_FAILURE;
    } else { // 0 and 3-255 Reserved
      DEBUG((EFI_D_ERROR, "DSM_Map_NIH: Invalid Mode input parameter: %d\n", FnInpPtr->Mode));
      mNvdimmAcpiSmmInterface->Status = (UINT32)NVDIMM_STS_INVALID_INPUT_PARAMETERS;
      return;
    }
  }

  if (FnInpPtr->HealthStatus.WritePersistencyPowerLoss && FnInpPtr->HealthStatus.WritePersistencyLossOffline) {
    if (FnInpPtr->Mode == NIH_INPUT_MODE_INJECT_ERRORS) {
      InjectEsFailuresReg |= INJECT_ES_TEMP_ERROR;
    } else if (FnInpPtr->Mode == NIH_INPUT_MODE_CLEAR_ERRORS) {
      InjectEsFailuresReg &= ~INJECT_ES_TEMP_ERROR;
    } else { // 0 and 3-255 Reserved
      DEBUG((EFI_D_ERROR, "DSM_Map_NIH: Invalid Mode input parameter: %d\n", FnInpPtr->Mode));
      mNvdimmAcpiSmmInterface->Status = (UINT32)NVDIMM_STS_INVALID_INPUT_PARAMETERS;
      return;
    }
  }

  if (FnInpPtr->HealthStatus.MaintenanceNeeded) {
    if (FnInpPtr->Mode == NIH_INPUT_MODE_INJECT_ERRORS) {
      InjectEsFailuresReg |= INJECT_ES_TEMP_WARNING;
    } else if (FnInpPtr->Mode == NIH_INPUT_MODE_CLEAR_ERRORS) {
      InjectEsFailuresReg &= ~INJECT_ES_TEMP_WARNING;
    } else { // 0 and 3-255 Reserved
      DEBUG((EFI_D_ERROR, "DSM_Map_NIH: Invalid Mode input parameter: %d\n", FnInpPtr->Mode));
      mNvdimmAcpiSmmInterface->Status = (UINT32)NVDIMM_STS_INVALID_INPUT_PARAMETERS;
      return;
    }
  }

  Status = ReadSmb(Nvdimm, INJECT_ERROR_TYPE, &InjectErrorTypeReg);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NIH: ReadSmb() for INJECT_ERROR_TYPE return: %d\n", Status));
    mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_RETRY_SUGGESTED;
    return;
  }

  // if ONE_TIME_USE is not 0, we zeroed it
  if (InjectErrorTypeReg & ONE_TIME_USE) {
    InjectErrorTypeReg &= ~ONE_TIME_USE;
    Status = WriteSmb(Nvdimm, INJECT_ERROR_TYPE, &InjectErrorTypeReg);
    if (Status != EFI_SUCCESS) {
      DEBUG((EFI_D_ERROR, "DSM_Map_NIH: WriteSmb() for INJECT_ERROR_TYPE return: %d\n", Status));
      mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_RETRY_SUGGESTED;
      return;
    }
  }

  if (Nvdimm->ESInfo.ESPolicy == ESINFO_DEVICE_MANAGED_POLICY) {
    Status = WriteSmb(Nvdimm, INJECT_ES_FAILURES, &InjectEsFailuresReg);
    if (Status != EFI_SUCCESS) {
      DEBUG((EFI_D_ERROR, "DSM_Map_NIH: WriteSmb() for INJECT_ES_FAILURES return: %d\n", Status));
      mNvdimmAcpiSmmInterface->Status = CREATE_DSM_EXT_STATUS (NVDIMM_STS_METHOD_SPECIFIC_ERROR_CODE, NIH_EXT_STS_SUBSET_OF_ERRORS);
      return;
    }
  }
  else {
    DEBUG((EFI_D_ERROR, "DSM_Map_NIH: DEVICE_MANAGED_POLICY_ENABLED is not set. Injections to INJECT_ES_FAILURES won't be successfull. \n"));
    mNvdimmAcpiSmmInterface->Status = CREATE_DSM_EXT_STATUS (NVDIMM_STS_METHOD_SPECIFIC_ERROR_CODE, NIH_EXT_STS_SUBSET_OF_ERRORS);
    return;
  }
  Status = WriteSmb(Nvdimm, INJECT_OPS_FAILURES0, &InjectOpsFailures0Reg);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "DSM_Map_NIH: WriteSmb() for INJECT_OPS_FAILURES0 return: %d\n", Status));
    mNvdimmAcpiSmmInterface->Status = CREATE_DSM_EXT_STATUS (NVDIMM_STS_METHOD_SPECIFIC_ERROR_CODE, NIH_EXT_STS_SUBSET_OF_ERRORS);
    return;
  }
  mNvdimmAcpiSmmInterface->Status = NVDIMM_STS_SUCCESS;

}

EFI_STATUS
EFIAPI
ServeNgnCommandsFromOS(
   IN  EFI_HANDLE            DispatchHandle,
   IN CONST EFI_SMM_SW_REGISTER_CONTEXT  *DispatchContext,
   IN OUT   VOID             *CommBuffer,     OPTIONAL
   IN OUT   UINTN            *CommBufferSize  OPTIONAL
   )
{
  EFI_STATUS      Status = ARS_INVALID_PARAM;
  UINT8           PmemIndex = 0;
  UINT8           Socket = 0;
  UINT8           ImcId = 0;
  UINT8           Ch = 0;
  UINT8           Dimm = 0, DimmIndex = 0;
  UINT8           Data = 0;
  UINT8           Index;
  JEDEC_NVDIMM    Nvdimm;

  DEBUG((EFI_D_ERROR, "ServePMemCommandsFromOS: ENTER\n"));

  //
  // The new encoding is that the device handle of _DSM commands for the root (ARS Commands) will be
  // 0xFFFFFFFF. Current spec has 4 ARS related functions
  if ((mNvdimmAcpiSmmInterface->FunctionIndex > 0) && (mNvdimmAcpiSmmInterface->FunctionIndex < MAX_ROOT_DEVICE_FCNS) && \
         (mNvdimmAcpiSmmInterface->DeviceHandle == 0xFFFFFFFF)) {
    // ARS commands to use BIOS MB only.
    DEBUG((EFI_D_ERROR, "ServePMemCommandsFromOS: before HandleARSCommandsFromOS() functionIndex: %d\n", mNvdimmAcpiSmmInterface->FunctionIndex));
    Status = HandleARSCommandsFromOS();
    DEBUG((EFI_D_ERROR, "ServePMemCommandsFromOS: after HandleARSCommandsFromOS(), Status = 0x%x\n", Status));
    return Status;
  }
  else {
    // _DSM Interface for Byte Addressable Energy Backed Function Class, Function Interface 1

    // Get socket ch and dimm from NgnCommandDeviceHandle in ACPI shared space
    GetDimmInfoFromDevHandle(&Socket, &ImcId, &Ch, &Dimm, mNvdimmAcpiSmmInterface->DeviceHandle);

    DEBUG((EFI_D_ERROR, "DSM Passing: Socket %d, ImcId %d, Ch%d, Dimm %d\n", Socket, ImcId, Ch, Dimm));;
    DEBUG((EFI_D_ERROR, "ENTER PERBYTE DSM\n"));;

    Status = ARS_INVALID_PARAM;
    mNvdimmAcpiSmmInterface->OutputLength = 0;
    mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_INVALID_PARAMETERS;

    for (PmemIndex = 0; PmemIndex < MAX_PMEM_REGIONS; PmemIndex++) {
      DEBUG((EFI_D_ERROR, "Size:0x%lX\n",(mPmemRegions[PmemIndex].Size)));
      if (mPmemRegions[PmemIndex].Size) {
        for (DimmIndex = 0; DimmIndex < mPmemRegions[PmemIndex].NumNvDimms; DimmIndex++) {
          DEBUG((EFI_D_ERROR, "Socket:%d ,IMCID: %d,Ch: %d,DIMM: %d\n",mPmemRegions[PmemIndex].NvDimms[DimmIndex].SocketId,mPmemRegions[PmemIndex].NvDimms[DimmIndex].ImcId,mPmemRegions[PmemIndex].NvDimms[DimmIndex].Ch,mPmemRegions[PmemIndex].NvDimms[DimmIndex].Dimm));
          if (mPmemRegions[PmemIndex].NvDimms[DimmIndex].SocketId == Socket &&
                  mPmemRegions[PmemIndex].NvDimms[DimmIndex].ImcId == ImcId &&
                  mPmemRegions[PmemIndex].NvDimms[DimmIndex].Ch == Ch &&
                  mPmemRegions[PmemIndex].NvDimms[DimmIndex].Dimm == Dimm) {
            Nvdimm = mPmemRegions[PmemIndex].NvDimms[DimmIndex];
            Status = EFI_SUCCESS;
            break;
          }
        }
      }
      if(Status == EFI_SUCCESS) {
        break;
      }
    }
    DEBUG((EFI_D_ERROR, "DimmIndex, PmemIndex:%d ,%d\n", DimmIndex, PmemIndex));

    DEBUG((EFI_D_ERROR, "mNvdimmAcpiSmmInterface->FunctionIndex: 0x%x\n", mNvdimmAcpiSmmInterface->FunctionIndex));
    if (Status == EFI_SUCCESS) {
      DEBUG((EFI_D_ERROR, "mNvdimmAcpiSmmInterface->FunctionIndex:2: 0x%x\n", mNvdimmAcpiSmmInterface->FunctionIndex));
      // Determine Function Index
      switch (mNvdimmAcpiSmmInterface->FunctionIndex) {
        case 1:   // Get NVDIMM-N Identification
          CopyMem(mNvdimmAcpiSmmInterface->OutputBuffer, &mPmemRegions[PmemIndex].NvDimms[DimmIndex].DimmId, sizeof(NVDIMM_IDENTIFICATION));
          mNvdimmAcpiSmmInterface->OutputLength = sizeof(NVDIMM_IDENTIFICATION);
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          break;

        case 2:   // Get Save Operation Requirement
          CopyMem(mNvdimmAcpiSmmInterface->OutputBuffer, &mPmemRegions[PmemIndex].NvDimms[DimmIndex].SaveOpReg, sizeof(SAVE_OPERATION_REQ));
          mNvdimmAcpiSmmInterface->OutputLength = sizeof(SAVE_OPERATION_REQ);
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          break;

        case 3:   // Get Energy Source Identification
          CopyMem(mNvdimmAcpiSmmInterface->OutputBuffer, &mPmemRegions[PmemIndex].NvDimms[DimmIndex].ESInfo, sizeof(ENERGY_SOURCE_IDENTIFICATION));
          mNvdimmAcpiSmmInterface->OutputLength = sizeof(ENERGY_SOURCE_IDENTIFICATION);
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          break;

        case 4:   // Get Last Backup Information
          ((NVDIMM_BACKUP_INFO *)mNvdimmAcpiSmmInterface->OutputBuffer)->TriggerInformation = mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.TriggerInformation;
          ((NVDIMM_BACKUP_INFO *)mNvdimmAcpiSmmInterface->OutputBuffer)->SaveFailInformation = mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.SaveFailInformation;
          mNvdimmAcpiSmmInterface->OutputLength = sizeof(NVDIMM_BACKUP_INFO);
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          break;

        case 5:   // Get NVDIMM Thresholds
          ((NVDIMM_THRESHOLD *)mNvdimmAcpiSmmInterface->OutputBuffer)->NVDIMMLifetimePercentWarnTheshold = mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.LifetimePercentWarnTheshold;
          ((NVDIMM_THRESHOLD *)mNvdimmAcpiSmmInterface->OutputBuffer)->NVDIMMLifetimePercentErrorTheshold = mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.LifetimePercentErrorTheshold;
          mNvdimmAcpiSmmInterface->OutputLength = sizeof(NVDIMM_THRESHOLD);
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          break;

        case 6:   // Set NVDIMM Lifetime Percentage Warning Threshold
          if (mNvdimmAcpiSmmInterface->InputLength != sizeof(NVDIMM_LIFETIME_WARN_THRESHOLD)) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_INVALID_PARAMETERS;
          }
          else {
            mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.LifetimePercentWarnTheshold = mNvdimmAcpiSmmInterface->InputBuffer[0];
            if (JedecSwitchPage(&Nvdimm, 0) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            if (WriteSmb(&Nvdimm, (NVM_LIFETIME_PERCENT_WARN_THRESHOLD & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          }
          break;

        case 7:   // Get Energy Source Threshold
          ((ENERGY_SOURCE_THRESHOLD *)mNvdimmAcpiSmmInterface->OutputBuffer)->ESLifetimePercentWarnTheshold = mPmemRegions[PmemIndex].NvDimms[DimmIndex].ESLifeInfo.ESLifetimePercentWarnTheshold;
          ((ENERGY_SOURCE_THRESHOLD *)mNvdimmAcpiSmmInterface->OutputBuffer)->ESLifetimePercentErrorThreshold = mPmemRegions[PmemIndex].NvDimms[DimmIndex].ESLifeInfo.ESLifetimePercentErrorThreshold;
          ((ENERGY_SOURCE_THRESHOLD *)mNvdimmAcpiSmmInterface->OutputBuffer)->ESTempWarnTheshold = mPmemRegions[PmemIndex].NvDimms[DimmIndex].ESLifeInfo.ESLifetimeTempWarnTheshold;
          ((ENERGY_SOURCE_THRESHOLD *)mNvdimmAcpiSmmInterface->OutputBuffer)->ESTempErrorTheshold = mPmemRegions[PmemIndex].NvDimms[DimmIndex].ESLifeInfo.ESLifetimeTempErrorTheshold;
          mNvdimmAcpiSmmInterface->OutputLength = sizeof(ENERGY_SOURCE_THRESHOLD);
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          break;

        case 8:   // Set Energy Source Lifetime Warning Threshold
          if (mNvdimmAcpiSmmInterface->InputLength != sizeof(ENERGY_SOURCE_LIFETIME_WARN_THRESHOLD)) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_INVALID_PARAMETERS;
          }
          else {
            mPmemRegions[PmemIndex].NvDimms[DimmIndex].ESLifeInfo.ESLifetimePercentWarnTheshold = mNvdimmAcpiSmmInterface->InputBuffer[0];
            if (JedecSwitchPage(&Nvdimm, 0) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            if (WriteSmb(&Nvdimm, (NVM_ES_LIFETIME_PERCENT_WARN_THRESHOLD & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          }
          break;

        case 9:   // Set Energy Source Temperature Warning Threshold
          if (mNvdimmAcpiSmmInterface->InputLength != sizeof(ENERGY_SOURCE_TEMPERATURE_WARN_THRESHOLD)) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_INVALID_PARAMETERS;
          }
          else {
            mPmemRegions[PmemIndex].NvDimms[DimmIndex].ESLifeInfo.ESLifetimeTempWarnTheshold = mNvdimmAcpiSmmInterface->InputBuffer[0];
            if (JedecSwitchPage(&Nvdimm, 0) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            if (WriteSmb(&Nvdimm, (NVM_ES_LIFETIME_TEMP_WARN_THRESHOLD & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          }
          break;

        case 10:  // Get Critical Health Info
          ((CRITICAL_HEALTH_INFO *)&mNvdimmAcpiSmmInterface->OutputBuffer)->CriticalHealthInfo = mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.CriticalHealthInfo;
          mNvdimmAcpiSmmInterface->OutputLength = sizeof(CRITICAL_HEALTH_INFO);
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          break;

        case 11:  // Get NVDIMM Health Info
          Status = JedecSwitchPage(&Nvdimm, 0);
          if (EFI_ERROR (Status)){
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
          } else {
            if (ReadSmb (&Nvdimm, MODULE_HEALTH_STATUS0 & 0xFF, &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.Health.ModuleHealthStatus = Data;

            if (ReadSmb (&Nvdimm, MODULE_HEALTH_STATUS1 & 0xFF, &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.Health.ModuleHealthStatus |= ((UINT16)Data << 8);
            DEBUG ((EFI_D_ERROR, "ModuleHealthStatus = 0x%x\n", mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.Health.ModuleHealthStatus));

            mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.Health.ModuleTemp = 0x1C1C;

            if (ReadSmb (&Nvdimm, ERROR_THRESHOLD_STATUS & 0xFF, &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.Health.ErrorThresStatus = Data;
            DEBUG ((EFI_D_ERROR, "ErrorThresStatus = 0x%x\n", Data));

            if (ReadSmb (&Nvdimm, WARNING_THRESHOLD_STATUS & 0xFF, &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.Health.ErrorThresWarn = Data;
            DEBUG ((EFI_D_ERROR, "ErrorThresWarn = 0x%x\n", Data));

            if (ReadSmb (&Nvdimm, NVM_LIFETIME & 0xFF, &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.Health.NVMLifetime = Data;
            DEBUG ((EFI_D_ERROR, "NVMLifetime = 0x%x\n", Data));

            Status = JedecSwitchPage (&Nvdimm, 2);
            if (EFI_ERROR (Status)) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            if (ReadSmb (&Nvdimm, DRAM_ECC_ERROR_COUNT & 0xFF, &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.Health.UECount = Data;
            DEBUG ((EFI_D_ERROR, "UECount = 0x%x\n", Data));

            if (ReadSmb (&Nvdimm, DRAM_THRESHOLD_ECC_COUNT & 0xFF, &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.Health.CECount = Data;
            DEBUG ((EFI_D_ERROR, "CECount = 0x%x\n", Data));

            CopyMem(mNvdimmAcpiSmmInterface->OutputBuffer, &mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.Health, sizeof(NVDIMM_HEALTH_INFO));
            mNvdimmAcpiSmmInterface->OutputLength = sizeof(NVDIMM_HEALTH_INFO);
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          }
          break;

        case 12:  // Get Energy Source Health Info
          Status = JedecSwitchPage(&Nvdimm, 1);
          if (EFI_ERROR (Status)) {
            mNvdimmAcpiSmmInterface->Status = CREATE_DSM_STATUS_FUNCTION_SPECIFIC (DSM_12_FUNCTION_SPECIFIC_PLATFORM_DOES_NOT_SUPPORT_ES_INFO);
          } else {
            if (ReadSmb(&Nvdimm, (NVM_ES_LIFETIME_PERCENTAGE & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            DEBUG((EFI_D_ERROR, "1.NVM_ES_LIFETIME_PERCENTAGE = 0x%1lx\n",Data));
            mPmemRegions[PmemIndex].NvDimms[DimmIndex].ESLifeInfo.ESLifeTime = Data;

            if (ReadSmb(&Nvdimm, (NVM_ES_CURRENT_TEMP & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            DEBUG((EFI_D_ERROR, "1.NVM_ES_CURRENT_TEMP = 0x%1lx\n",Data));
            mPmemRegions[PmemIndex].NvDimms[DimmIndex].ESLifeInfo.ESTemp0 = Data;

            if (ReadSmb(&Nvdimm, ((NVM_ES_CURRENT_TEMP + 1) & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            DEBUG((EFI_D_ERROR, "1.NVM_ES_CURRENT_TEMP = 0x%1lx\n",Data));
            mPmemRegions[PmemIndex].NvDimms[DimmIndex].ESLifeInfo.ESTemp1 = Data;

            if (ReadSmb(&Nvdimm, (NVM_ES_TOTAL_RUNTIME & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            DEBUG((EFI_D_ERROR, "1.NVM_ES_TOTAL_RUNTIME = 0x%1lx\n",Data));
            mPmemRegions[PmemIndex].NvDimms[DimmIndex].ESLifeInfo.ESRuntime0 = Data;

            if (ReadSmb(&Nvdimm, ((NVM_ES_TOTAL_RUNTIME + 1) & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            DEBUG((EFI_D_ERROR, "1.NVM_ES_TOTAL_RUNTIME = 0x%1lx\n",Data));

            ((ENERGY_STORAGE_HEALTH_INFO *)mNvdimmAcpiSmmInterface->OutputBuffer)->ESLifeTime = mPmemRegions[PmemIndex].NvDimms[DimmIndex].ESLifeInfo.ESLifeTime;
            ((ENERGY_STORAGE_HEALTH_INFO *)mNvdimmAcpiSmmInterface->OutputBuffer)->ESTemp0 = mPmemRegions[PmemIndex].NvDimms[DimmIndex].ESLifeInfo.ESTemp0;
            ((ENERGY_STORAGE_HEALTH_INFO *)mNvdimmAcpiSmmInterface->OutputBuffer)->ESTemp1 = mPmemRegions[PmemIndex].NvDimms[DimmIndex].ESLifeInfo.ESTemp1;
            ((ENERGY_STORAGE_HEALTH_INFO *)mNvdimmAcpiSmmInterface->OutputBuffer)->ESRuntime0 = mPmemRegions[PmemIndex].NvDimms[DimmIndex].ESLifeInfo.ESRuntime0;
            ((ENERGY_STORAGE_HEALTH_INFO *)mNvdimmAcpiSmmInterface->OutputBuffer)->ESRuntime1 = mPmemRegions[PmemIndex].NvDimms[DimmIndex].ESLifeInfo.ESRuntime1;
            ((ENERGY_STORAGE_HEALTH_INFO *)mNvdimmAcpiSmmInterface->OutputBuffer)->Rsvd[0] = 0;
            ((ENERGY_STORAGE_HEALTH_INFO *)mNvdimmAcpiSmmInterface->OutputBuffer)->Rsvd[1] = 0;

            mNvdimmAcpiSmmInterface->OutputLength = sizeof(ENERGY_STORAGE_HEALTH_INFO);
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          }
          break;

        case 13:  // Get Operational Statistics
          CopyMem(mNvdimmAcpiSmmInterface->OutputBuffer, &mPmemRegions[PmemIndex].NvDimms[DimmIndex].OpInfo, sizeof(OPERATION_STATISTIC_INFO));
          mNvdimmAcpiSmmInterface->OutputLength = sizeof(OPERATION_STATISTIC_INFO);
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          break;

        case 14:  // VENDOR_LOG_PAGE_SIZE
          ((VENDOR_LOG_PAGE_SIZE *)mNvdimmAcpiSmmInterface->OutputBuffer)->VendorLogPageSize = mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.VendorLogPageSize;
          mNvdimmAcpiSmmInterface->OutputLength = sizeof(VENDOR_LOG_PAGE_SIZE);
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          break;

        case 15:  // VENDOR_LOG_PAGE_SIZE - NOTE: Vendor Log Page not supported, return Page Size instead.
          ((VENDOR_LOG_PAGE *)mNvdimmAcpiSmmInterface->OutputBuffer)->VendorLogPage = mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.VendorLogPage;
          mNvdimmAcpiSmmInterface->OutputLength = sizeof(VENDOR_LOG_PAGE);
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          break;

        case 16:  // QUERY ERROR INJ STATUS: error injection is disabled
          ((QUERY_ERROR_INJ_STATUS *)mNvdimmAcpiSmmInterface->OutputBuffer)->ErrorInjEnable = mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.ErrorInjEnable;
          mNvdimmAcpiSmmInterface->OutputLength = sizeof(QUERY_ERROR_INJ_STATUS);
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          break;

        case 17:  // Inject Error
          if (mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.ErrorInjEnable == 0) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_INVALID_PARAMETERS;
          }
          else {
            if (JedecSwitchPage(&Nvdimm, 2) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            // Fill up what to inject!
            Data = ((INJECT_ERROR *)mNvdimmAcpiSmmInterface->InputBuffer)->InjectOpsFailures;  // INJECT_OPS_FAILURES
            if (WriteSmb(&Nvdimm, (NVM_INJECT_OP_FAILURES & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            if (Data & BIT7) {
              // Byte 1 - If INJECT_BAD_BLOCKS is 1 (bit 7 of Byte 0), this is INJECT_BAD_BLOCK_CAP (2, 0x67). Otherwise, it shall be 0.
              Data = ((INJECT_ERROR *)mNvdimmAcpiSmmInterface->InputBuffer)->InjectBadBlocks;
              if (WriteSmb(&Nvdimm, (NVM_INJECT_BAD_BLOCK_CAP & 0xFF), &Data) != EFI_SUCCESS) {
                mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
                break;
              }
            }

            Data = ((INJECT_ERROR *)mNvdimmAcpiSmmInterface->InputBuffer)->InjectESFailures;
            if (WriteSmb(&Nvdimm, (NVM_INJECT_ES_FAILURES & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            Data = ((INJECT_ERROR *)mNvdimmAcpiSmmInterface->InputBuffer)->InjectFwFailures;
            if (WriteSmb(&Nvdimm, (NVM_FW_UDPATE_FAILURES & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          }
          mNvdimmAcpiSmmInterface->OutputLength = 0;
          break;

        case 18:  // Get Injected Errors
          if (JedecSwitchPage(&Nvdimm, 2) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          if (ReadSmb(&Nvdimm, (NVM_INJECT_OP_FAILURES & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          ((INJECT_ERROR_FAILURE_STATUS *)mNvdimmAcpiSmmInterface->OutputBuffer)->InjectOpsFailures = Data;
          if (ReadSmb(&Nvdimm, (NVM_INJECT_BAD_BLOCK_CAP & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          ((INJECT_ERROR_FAILURE_STATUS *)mNvdimmAcpiSmmInterface->OutputBuffer)->InjectBadBlocks = Data;
          if (ReadSmb(&Nvdimm, (NVM_INJECT_ES_FAILURES & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          ((INJECT_ERROR_FAILURE_STATUS *)mNvdimmAcpiSmmInterface->OutputBuffer)->InjectESFailures = Data;
          if (ReadSmb(&Nvdimm, (NVM_FW_UDPATE_FAILURES & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          ((INJECT_ERROR_FAILURE_STATUS *)mNvdimmAcpiSmmInterface->OutputBuffer)->InjectFwFailures = Data;
          mNvdimmAcpiSmmInterface->OutputLength = sizeof(INJECT_ERROR_FAILURE_STATUS);
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          break;

        case 19:  // Erase NVDIMM STATUS (Bit[3]-Start Erase)
          if (JedecSwitchPage(&Nvdimm, 0) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          if (ReadSmb(&Nvdimm, (NVM_ERASE_IMAGE & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          Data |= (1 << 3);
          if (WriteSmb(&Nvdimm, (NVM_ERASE_IMAGE & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          mNvdimmAcpiSmmInterface->OutputLength = 0;
          break;

        case 20:  // Arm NVDIMM STATUS (Bit0 set)
          if (JedecSwitchPage(&Nvdimm, 0) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          if (ReadSmb(&Nvdimm, (NVM_ARM & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          Data |= 1;
          if (WriteSmb(&Nvdimm, (NVM_ARM & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          mNvdimmAcpiSmmInterface->OutputLength = 0;
          break;

        case 21:  // Reset Factory Default STATUS (Bit0 set)
          if (JedecSwitchPage(&Nvdimm, 0) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          if (ReadSmb(&Nvdimm, (NVM_RESET_FACTORY_DEFAULT & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          Data |= 1;
          if (WriteSmb(&Nvdimm, (NVM_RESET_FACTORY_DEFAULT & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          mNvdimmAcpiSmmInterface->OutputLength = 0;
          break;

        case 22:  // Start Firmware Update (Bit[3:0]-Select FW SLOT)
          // Get firmware slot
          if (JedecSwitchPage(&Nvdimm, 3) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          if (ReadSmb(&Nvdimm, (NVM_FW_SLOT & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          Data &= 0xF0;
          Data |= mNvdimmAcpiSmmInterface->InputBuffer[0] & 0x0F;
          if (WriteSmb(&Nvdimm, (NVM_FW_SLOT & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          if (JedecSwitchPage(&Nvdimm, 0) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          if (ReadSmb(&Nvdimm, (NVM_START_FW_UPDATE & 0xFF), &Data)!= EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          Data |= 1; // (Bit[0] = 1 enable fw update mode)
          if (WriteSmb(&Nvdimm, (NVM_START_FW_UPDATE & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          mNvdimmAcpiSmmInterface->OutputLength = 0;
          break;

        case 23:  // Send Firmware Update Data
          if (((SEND_FW_UPDATE_DATA *)mNvdimmAcpiSmmInterface->InputBuffer)->RegionLength > READ_TYPE_DATA_SIZE) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_INVALID_PARAMETERS;
          }
          else {
            if (JedecSwitchPage(&Nvdimm, 3) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            Data = ((SEND_FW_UPDATE_DATA *)mNvdimmAcpiSmmInterface->InputBuffer)->RegionId & 0xFF;
            if (WriteSmb(&Nvdimm, (NVM_REGION_ID & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            Data = (((SEND_FW_UPDATE_DATA *)mNvdimmAcpiSmmInterface->InputBuffer)->RegionId >> 8) & 0xFF;
            if (WriteSmb(&Nvdimm, ((NVM_REGION_ID + 1) & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            Data = ((SEND_FW_UPDATE_DATA *)mNvdimmAcpiSmmInterface->InputBuffer)->BlockId;
            if (WriteSmb(&Nvdimm, (NVM_BLOCK_ID & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            for (Index = 0; Index < ((SEND_FW_UPDATE_DATA *)mNvdimmAcpiSmmInterface->InputBuffer)->RegionLength; Index++) {
              Data = ((SEND_FW_UPDATE_DATA *)mNvdimmAcpiSmmInterface->InputBuffer)->FwData[Index];
              if (WriteSmb(&Nvdimm, ((NVM_DATA_RD_WR + Index) & 0xFF), &Data) != EFI_SUCCESS) {
                mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
                break;
              }
            }
            if (Index == ((SEND_FW_UPDATE_DATA *)mNvdimmAcpiSmmInterface->InputBuffer)->RegionLength) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
              mNvdimmAcpiSmmInterface->OutputLength = 0;
            }
          }
          break;

        case 24:  // Finish Firmware Update STATUS
          if (JedecSwitchPage(&Nvdimm, 0) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          if (ReadSmb(&Nvdimm, (NVM_START_FW_UPDATE & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          Data &= 0xFE; // (Bit[0] = 0 - disable fw update mode)
          if (WriteSmb(&Nvdimm, (NVM_START_FW_UPDATE & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          mNvdimmAcpiSmmInterface->OutputLength = 0;
          break;

        case 25:  // Select Firmware Image Slot
          Data = ((START_FW_UDPATE *)mNvdimmAcpiSmmInterface->InputBuffer)->FwSlot;
          if (JedecSwitchPage(&Nvdimm, 3) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          if (WriteSmb(&Nvdimm, (NVM_FW_SLOT & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          mNvdimmAcpiSmmInterface->OutputLength = 0;
          break;

        case 26:  // Get Firmware Info
          Data = ((START_FW_UDPATE *)mNvdimmAcpiSmmInterface->InputBuffer)->FwSlot;
          if (JedecSwitchPage(&Nvdimm, 3) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          if (WriteSmb(&Nvdimm, (NVM_FW_SLOT & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          if (JedecSwitchPage(&Nvdimm, 0) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          if (Data == 0) { // FW SLOT 0
            if (ReadSmb(&Nvdimm, (NVM_FW_SLOT0_REV & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            ((FW_INFO *)mNvdimmAcpiSmmInterface->OutputBuffer)->SlotFwVer0 = Data;
            if (ReadSmb(&Nvdimm, ((NVM_FW_SLOT0_REV + 1) & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            ((FW_INFO *)mNvdimmAcpiSmmInterface->OutputBuffer)->SlotFwVer1 = Data;
          }else {      // FW SLOT 1
            if (ReadSmb(&Nvdimm, (NVM_FW_SLOT1_REV & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            ((FW_INFO *)mNvdimmAcpiSmmInterface->OutputBuffer)->SlotFwVer0 = Data;
            if (ReadSmb(&Nvdimm, ((NVM_FW_SLOT1_REV + 1) & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            ((FW_INFO *)mNvdimmAcpiSmmInterface->OutputBuffer)->SlotFwVer1 = Data;
          }
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          mNvdimmAcpiSmmInterface->OutputLength = sizeof(FW_INFO);
          break;

        case 27:  // I2C Read
          Data = ((I2C_READ *)mNvdimmAcpiSmmInterface->InputBuffer)->Page;
          if (JedecSwitchPage(&Nvdimm, Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          if (ReadSmb(&Nvdimm, ((I2C_READ *)mNvdimmAcpiSmmInterface->InputBuffer)->Offset, &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          mNvdimmAcpiSmmInterface->OutputBuffer[0] = Data;
          mNvdimmAcpiSmmInterface->OutputLength = 1;
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          break;

        case 28:  // I2C Write
          Data = ((I2C_WRITE *)mNvdimmAcpiSmmInterface->InputBuffer)->Page;
          if (JedecSwitchPage(&Nvdimm, Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          Data = ((I2C_WRITE *)mNvdimmAcpiSmmInterface->InputBuffer)->Data;
          if (WriteSmb(&Nvdimm, ((I2C_WRITE *)mNvdimmAcpiSmmInterface->InputBuffer)->Offset, &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          mNvdimmAcpiSmmInterface->OutputLength = 0;
          break;

        case 29:  // Read Typed Data
          if (JedecSwitchPage(&Nvdimm, 3) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          Data = ((READ_TYPE_DATA *)mNvdimmAcpiSmmInterface->InputBuffer)->Type;
          if (WriteSmb(&Nvdimm, (NVM_DATA_TYPE & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          Data = ((READ_TYPE_DATA *)mNvdimmAcpiSmmInterface->InputBuffer)->RegionId[0];
          if (WriteSmb(&Nvdimm, (NVM_REGION_ID & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          Data |= ((READ_TYPE_DATA *)mNvdimmAcpiSmmInterface->InputBuffer)->RegionId[1];
          if (WriteSmb(&Nvdimm, ((NVM_REGION_ID + 1) & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          Data = ((READ_TYPE_DATA *)mNvdimmAcpiSmmInterface->InputBuffer)->BlockId;
          if (WriteSmb(&Nvdimm, (NVM_BLOCK_ID & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          for (Index = 0; Index < READ_TYPE_DATA_SIZE; Index++) {
            if (ReadSmb(&Nvdimm, ((NVM_DATA_RD_WR + Index) & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
            ((READ_TYPE_DATA *)mNvdimmAcpiSmmInterface->InputBuffer)->Data[Index] = Data;
          }
          if (Index == READ_TYPE_DATA_SIZE) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
            mNvdimmAcpiSmmInterface->OutputLength = READ_TYPE_DATA_SIZE;
          }
          break;

        case 30:  // Write Typed Data
          if (JedecSwitchPage(&Nvdimm, 3) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          Data = ((WRITE_TYPE_DATA *)mNvdimmAcpiSmmInterface->InputBuffer)->Type;
          if (WriteSmb(&Nvdimm, (NVM_DATA_TYPE & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          Data = ((WRITE_TYPE_DATA *)mNvdimmAcpiSmmInterface->InputBuffer)->RegionId[0];
          if (WriteSmb(&Nvdimm, (NVM_REGION_ID & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          Data |= ((WRITE_TYPE_DATA *)mNvdimmAcpiSmmInterface->InputBuffer)->RegionId[1];
          if (WriteSmb(&Nvdimm, ((NVM_REGION_ID + 1) & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          Data = ((WRITE_TYPE_DATA *)mNvdimmAcpiSmmInterface->InputBuffer)->BlockId;
          if (WriteSmb(&Nvdimm, (NVM_BLOCK_ID & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          for (Index = 0; Index < READ_TYPE_DATA_SIZE; Index++) {
            Data = ((READ_TYPE_DATA *)mNvdimmAcpiSmmInterface->InputBuffer)->Data[Index];
            if (WriteSmb(&Nvdimm, ((NVM_DATA_RD_WR + Index) & 0xFF), &Data) != EFI_SUCCESS) {
              mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
              break;
            }
          }
          if (Index == READ_TYPE_DATA_SIZE) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
            mNvdimmAcpiSmmInterface->OutputLength = 0;
          }
          break;

        case 31:  // Set Memory Error Counters
          if (JedecSwitchPage(&Nvdimm, 2) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          Data = ((SET_MEMORY_ERROR_COUNTER *)mNvdimmAcpiSmmInterface->InputBuffer)->UECounter;
          mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.Health.UECount = Data;
          if (WriteSmb(&Nvdimm, (NVM_UCECC_COUNT_REG & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          Data = ((SET_MEMORY_ERROR_COUNTER *)mNvdimmAcpiSmmInterface->InputBuffer)->CEThresholdCounter;
          //  mPmemRegions[PmemIndex].NvDimms[DimmIndex].ModuleInfo.Health.CEThresholdCounter = Data; TODO
          if (WriteSmb(&Nvdimm, (NVM_CECC_COUNT_REG & 0xFF), &Data) != EFI_SUCCESS) {
            mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR;
            break;
          }
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_SUCCESS;
          mNvdimmAcpiSmmInterface->OutputLength = 0;
          break;

        case NCH_FUNCTION_INDEX:
          DSM_Map_NCH(&Nvdimm);
          break;

        case NBS_FUNCTION_INDEX:
          DSM_Map_NBS(&Nvdimm);
          break;

        case NIC_FUNCTION_INDEX:
          DSM_Map_NIC(&Nvdimm);
          break;

        case NIH_FUNCTION_INDEX:
          DSM_Map_NIH(&Nvdimm);
          break;

        case NIG_FUNCTION_INDEX:
          DSM_Map_NIG(&Nvdimm);
          break;

        default:
          mNvdimmAcpiSmmInterface->Status = DSM_GENERAL_STATUS_INVALID_PARAMETERS;
      } // case
    } // if success
  }

  DEBUG((EFI_D_ERROR, " mNvdimmAcpiSmmInterface outputlength::%d ", mNvdimmAcpiSmmInterface->OutputLength));
  DEBUG((EFI_D_ERROR, "mNvdimmAcpiSmmInterface->STATUS :%d\n", mNvdimmAcpiSmmInterface->Status));
  for(Index = 0; Index < mNvdimmAcpiSmmInterface->OutputLength; Index++)
    DEBUG((EFI_D_ERROR, " %x ", mNvdimmAcpiSmmInterface->OutputBuffer[Index]));
  DEBUG((EFI_D_ERROR, " \n"));
  DEBUG((EFI_D_ERROR, "ServeNgnCommandsFromOS: EXIT\n"));

  return Status;
}

/**

Routine Description: GetDimmInfoFromSpa - This function is
responsible for getting Socket, Channel and Dimm information
based on the given System Physical Address. That is, to figure
out the Nvm Dimm that is mapped to the passed in Spa.

  @param Spa          - Spa
  @param ArsLength    -
  @param *Soc         - pointer to Socket that this function
         will return
  @param *Channel     - pointer to channel that this function
         will return
  @param *Dimm        - Pointer to Dimm that this function will
         return
  @retval Status      - Return Status

**/
EFI_STATUS
GetDimmInfoFromSpa(
   UINT64      Spa,
   UINT64      *SADSpaStart,
   UINT8       *Soc,
   UINT8       *Channel,
   UINT8       *Dimm
   )
{
  EFI_STATUS       Status = EFI_INVALID_PARAMETER;
  UINT8            Socket;
  UINT8            PmemIndex;
  UINT8            i;
  UINT64           SpaStart, SpaEnd;
  struct SADTable  *pSAD;
  UINT8            ChInterBitmap;


  //
  // Traverse thru all SAD entries to check for the Spa being in the range of one
  for (PmemIndex = 0; PmemIndex < MAX_PMEM_REGIONS; PmemIndex++) {
    if (mPmemRegions[PmemIndex].Size == 0) {
      continue;
    }
    Socket = mPmemRegions[PmemIndex].SocketNum;
    i = mPmemRegions[PmemIndex].SadIndex;
    pSAD = &mSystemMemoryMap->Socket[Socket].SAD[i];
    ChInterBitmap = GetChInterBitmap(pSAD);
    if (pSAD->Enable == 0) {
      continue;
    }
    if (pSAD->local == 0) {
      continue;
    }
    if (i == 0) {
      SpaStart = 0;
    }
    else {
      SpaStart = ((UINT64)mSystemMemoryMap->Socket[Socket].SAD[i - 1].Limit << BITS_64MB_TO_BYTES);
    }
    SpaEnd = ((UINT64)mSystemMemoryMap->Socket[Socket].SAD[i].Limit << BITS_64MB_TO_BYTES);

    //
    // If the passed in SPA falls with in this SAD SPA Range
    if ((Spa >= SpaStart) && (Spa <= SpaEnd)) {
      //
      // Found the Sad rule with in which the passed in Spa has mapping, now next to figure out to which dimm
      // this Spa is mapped into.
      //
      // One of the return values is SADSpaStart address
      *SADSpaStart = SpaStart;
    }
      Status = JedecSpaToDpa(Spa, Soc, Channel, Dimm, NULL);
  }

  if (Status ==  EFI_SUCCESS) {
#if VERBOSE_DEBUG
    DEBUG((EFI_D_ERROR, "JNP: GetDimmInfoFromSpa - Found Dimm Info From Spa (0x%llx), Soc = %d, Channel = %d, Dimm = %d\n", Spa, *Soc, *Channel, *Dimm));
#endif
  }
  else {
    DEBUG((EFI_D_ERROR, "JNP: GetDimmInfoFromSpa - Failed\n"));

  }
  return (Status);
}

/**

Routine Description:JedecNvdimmStart gathers Control Region
Data from the SADs and initializes the internal data structs for
later enablement of the read/write of the NVDIMM registers thru the
SAD interface.

  @retval UINT32 - status

**/
EFI_STATUS
EFIAPI
JedecNvdimmStart (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS           Status = EFI_SUCCESS;
  EFI_HANDLE           Handle = NULL;
  BOOLEAN              InSmm = FALSE;
  BOOLEAN              NvDimmPresent;
  UINT8                Socket;
  UINT8                Ch;
  UINT8                Dimm;

  Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, (VOID**)&mSmmBase);
  if (mSmmBase == NULL) {
    InSmm = FALSE;
  } else {
    mSmmBase->InSmm (mSmmBase, &InSmm);
    mSmmBase->GetSmstLocation (mSmmBase, &mSmst);
  }
  mInSmm = InSmm;

  mSystemMemoryMap = GetSystemMemoryMapHob ();

  // If CR DCPMMs present then exit
  IsDcpmmPresent = FALSE;
  if (mSystemMemoryMap->DcpmmPresent) {
    IsDcpmmPresent = TRUE;
    DEBUG ((EFI_D_ERROR, "[JEDEC] (InSmm: %d) PMem present, exiting JEDEC NVDIMM driver.\n", InSmm));
    return EFI_SUCCESS;
  }
  // DCPMMs not present, check if NVDIMM-N exists
  NvDimmPresent = FALSE;
  for (Socket = 0; (Socket < MAX_SOCKET) && !NvDimmPresent; Socket++) {
    for (Ch = 0; (Ch < MAX_CH) && !NvDimmPresent; Ch++) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (mSystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].NVmemSize != 0) {
          NvDimmPresent = TRUE;
          break;
        }
      }
    }
  }
  if (NvDimmPresent == FALSE) {
    DEBUG ((EFI_D_ERROR, "[JEDEC] (InSmm: %d) JEDEC NVDIMM not present - exiting\n", InSmm));
    return EFI_SUCCESS;
  }

  if (!InSmm) {
    Status = gBS->LocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &mCpuCsrAccess);
    ASSERT_EFI_ERROR (Status);

    Status = gBS->AllocatePool (EfiRuntimeServicesData, sizeof (PMEM_REGION)*MAX_PMEM_REGIONS, &mPmemRegions);
    ASSERT_EFI_ERROR (Status);
    ZeroMem (mPmemRegions, sizeof (PMEM_REGION)*MAX_PMEM_REGIONS);
  }
  else {
    Status = mSmst->SmmLocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &mCpuCsrAccess);
    ASSERT_EFI_ERROR (Status);

    Status = mSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (PMEM_REGION)*MAX_PMEM_REGIONS, &mPmemRegions);
    ASSERT_EFI_ERROR (Status);
    ZeroMem (mPmemRegions, sizeof (PMEM_REGION)*MAX_PMEM_REGIONS);
  }

  Status = gBS->LocateProtocol (&gEfiNvdimmSmbusSmmInterfaceProtocolGuid, NULL, (VOID**)&mNvdimmSmbusSmmInterfaceProtocol);
  ASSERT_EFI_ERROR (Status);
  mNvdimmSmbusSmmInterface = mNvdimmSmbusSmmInterfaceProtocol->Area;

  Status = InitializeNvdimmAcpiSmmInterface ();
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_ERROR, "[JEDEC] (InSmm: %d) ACPI<->SMM interface address: 0x%llx\n", InSmm, mNvdimmAcpiSmmInterface));

  // Find JEDEC NVDIMMs
  Status = FillPmemRegions ();

  if (InSmm) {
    mNvdimmAcpiSmmInterface->DebugLevel = GetDebugPrintErrorLevel();
    mNvdimmAcpiSmmInterface->DebugInterface = mSystemMemoryMap->MemSetup.dfxMemSetup.DfxCrAcpiDebugInterface;
    mNvdimmAcpiSmmInterface->MaxMcChannel = MAX_MC_CH;
  }
  else {
    Status = InstallNfitTableUpdateProtocol (ImageHandle);
    ASSERT_EFI_ERROR (Status);

    Status = LoadAcpiSsdtNvdrTable ();
    ASSERT_EFI_ERROR (Status);
  }

  ZeroMem (&NvdimmInfo, sizeof (NVDIMM_COMMON_INFO));

  //
  // Get and update the NVDIMM and PMEM info structs
  UpdateDimmInfo ();

  //
  // Create NVMDIMM Present Bitmaps based on ALL NVDIMMs installed in all sockets
  // for use by AcpiSmmInterface.
  CreateDimmPresentBitmaps ();

  //
  // Get some details of these dimms using HOB.
  InitAllDimmsFromHOB (&NvdimmInfo);

  if (InSmm) {
    EFI_SMM_SW_REGISTER_CONTEXT       SwContext;
    EFI_SMM_SW_DISPATCH2_PROTOCOL     *SwDispatch  = 0;
    EFI_HANDLE                        SwHandle;
    Status = mSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, &SwDispatch);
    ASSERT_EFI_ERROR (Status);
    SwContext.SwSmiInputValue = 0xFD;
    Status = SwDispatch->Register (SwDispatch, ServeNgnCommandsFromOS, &SwContext, &SwHandle);
    ASSERT_EFI_ERROR (Status);

    NvdimmSmbusAccessSmmInit ();

    //
    // Install handler for ARS
    //
    Handle = NULL;
    Status = mSmst->SmiHandlerRegister (
                        CheckAndHandleArsEvent,
                        NULL,
                        &Handle
                        );
  }


/**
  ARS testing code


{
  UINT64 Spa = 0x180000000;
  UINT64 ArsLength = 0xC00000000;

  if (InSmm) {
    //
    // NVDIMM configuration: 1way = (S0:C1:D0) 2way = (S0:C4:D1, S0:C5:D1) 3way = (S0:C3:D0, S0:C4:D0, S0:C5:D0)
    //
    DEBUG((EFI_D_ERROR, "\n\nARS Test...test all persistent memory\n\n\n"));
    HandleARSStart(Spa, ArsLength);
    DEBUG((EFI_D_ERROR, "\n\nARS Test...test 1way only\n\n\n"));
    Spa = 0x180000000;
    ArsLength = 0x200000000;
    HandleARSStart(Spa, ArsLength);
    DEBUG((EFI_D_ERROR, "\n\nARS Test...test 3way only\n\n\n"));
    Spa = 0x380000000;
    ArsLength = 0x600000000;
    HandleARSStart(Spa, ArsLength);
    DEBUG((EFI_D_ERROR, "\n\nARS Test...test 2way only\n\n\n"));
    Spa = 0x980000000;
    ArsLength = 0x400000000;
    HandleARSStart(Spa, ArsLength);
    DEBUG((EFI_D_ERROR, "\n\nARS Test...test middle of 3way\n\n\n"));
    Spa = 0x580000000;
    ArsLength = 0x200000000;
    HandleARSStart(Spa, ArsLength);
    DEBUG((EFI_D_ERROR, "\n\nARS Test...test start in 1way and end in 3way\n\n\n"));
    Spa = 0x200000000;
    ArsLength = 0x400000000;
    HandleARSStart(Spa, ArsLength);
    DEBUG((EFI_D_ERROR, "\n\nARS Boundary Test...test before PMEM start\n\n\n"));
    Spa = 0x100000000;
    ArsLength = 0xC00000000;
    HandleARSStart(Spa, ArsLength);
    DEBUG((EFI_D_ERROR, "\n\nARS Boundary Test...test after PMEM end\n\n\n"));
    Spa = 0x200000000;
    ArsLength = 0xC00000000;
    HandleARSStart(Spa, ArsLength);
    DEBUG((EFI_D_ERROR, "\n\nARS Boundary Test...test larger than PMEM region\n\n\n"));
    Spa = 0x100000000;
    ArsLength = 0xD00000000;
    HandleARSStart(Spa, ArsLength);
  }
}

**/


  return EFI_SUCCESS;
}


