/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2019 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>
#include <RcRegs.h>
#include <KtiSetupDefinitions.h>
#include <Upi/KtiSi.h>
#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>
#include <KtiMisc.h>
#include <UncoreCommonIncludes.h>
#include <SiliconSetting.h>
#include <Library/MemMcIpLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/ChaLib.h>
#include <Guid/M2UPcieIpInterface.h>
#include <Guid/KtiIpInterface.h>
#include <Library/KtiApi.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/UpiHostDataLib.h>

//
//10nm M2IOSF->M2UPI VN0 AD Credit arrays
//

//
// ICX 1 scoket M2IOSF->M2UPI VN0 AD credit table
//
static UINT8 Icx_M2iosfToM2UpiVn0Ad_1S[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0}
};
//
// ICX XCC M2IOSF->M2UPI VN0 AD credit table
//
static UINT8 Icx_Xcc_M2iosfToM2UpiVn0Ad_2S2link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {12, 16, 12},
  {14, 12, 14},
  {14, 12, 12},
  {12, 12, 12},
  {12, 12, 16},
  {12, 12, 18}
};
static UINT8 Icx_Xcc_M2iosfToM2UpiVn0Ad_2S3link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {18, 16, 12},
  {14, 12, 12},
  {14, 12, 12},
  {12, 12, 12},
  {12, 12, 12},
  {12, 12, 12}
};
static UINT8 Icx_Xcc_M2iosfToM2UpiVn0Ad_4S3link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {18, 16, 12},
  {14, 12, 12},
  {14, 12, 12},
  {12, 12, 12},
  {12, 12, 12},
  {12, 12, 12}
};
static UINT8 Icx_Xcc_M2iosfToM2UpiVn0Ad_4S2link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {14, 16, 12},
  {12, 12, 14},
  {12, 12, 12},
  {12, 14, 12},
  {12, 12, 12},
  {12, 12, 12}
};
static UINT8 Icx_Xcc_M2iosfToM2UpiVn0Ad_8S3link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {18, 16, 12},
  {12, 14, 12},
  {14, 12, 12},
  {12, 12, 12},
  {12, 12, 12},
  {12, 12, 12}
};
//
// ICX HCC M2IOSF->M2UPI VN0 AD credit table
//
static UINT8 Icx_Hcc_M2iosfToM2UpiVn0Ad_2S2link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {12, 12, 12},
  {14, 12, 12},
  {14, 12, 12},
  {12, 12, 12},
  {12, 12, 12},
  {12, 12, 12}
};
static UINT8 Icx_Hcc_M2iosfToM2UpiVn0Ad_2S3link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {12, 12, 12},
  {14, 12, 12},
  {14, 12, 12},
  {12, 12, 12},
  {12, 12, 12},
  {12, 12, 12}
};
static UINT8 Icx_Hcc_M2iosfToM2UpiVn0Ad_4S3link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {12, 12, 12},
  {14, 12, 12},
  {14, 12, 12},
  {12, 12, 12},
  {12, 12, 12},
  {12, 12, 12}
};
static UINT8 Icx_Hcc_M2iosfToM2UpiVn0Ad_4S2link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {12, 12, 12},
  {12, 12, 12},
  {12, 12, 12},
  {12, 14, 12},
  {12, 12, 14},
  {12, 12, 12}
};
static UINT8 Icx_Hcc_M2iosfToM2UpiVn0Ad_8S3link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {12, 12, 12},
  {12, 14, 12},
  {14, 12, 12},
  {12, 12, 12},
  {12, 12, 12},
  {12, 12, 12}
};

//
// ICX LCC M2IOSF->M2UPI VN0 AD credit table
//
static UINT8 Icx_Lcc_M2iosfToM2UpiVn0Ad_2S2link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {12, 12, 00},
  {12, 12, 00},
  {12, 12, 00},
  {00, 00, 00},
  {12, 12, 00},
  {12, 12, 00}
};

static UINT8 Icx_Lcc_M2iosfToM2UpiVn0Ad_4S2link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {12, 12, 00},
  {12, 12, 00},
  {12, 12, 00},
  {00, 00, 00},
  {12, 12, 00},
  {12, 12, 00}
};

//
//10nm 1 socket M2IOSF->M2UPI VNA BL Credit arrays
//
static UINT8 Icx_M2iosfToM2UpiVnaBl_1S[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0}
};

//
// ICX XCC & HCC M2IOSF->M2UPI VNA BL credit table
//
static UINT8 Icx_Xcc_Hcc_M2iosfToM2UpiVnaBl_2S2link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {25, 20, 25},
  {20, 25, 20},
  {20, 25, 20},
  {20, 25, 20},
  {20, 25, 20},
  {25, 20, 25}
};
static UINT8 Icx_Xcc_Hcc_M2iosfToM2UpiVnaBl_2S3link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {25, 20, 25},
  {20, 25, 20},
  {20, 25, 20},
  {20, 25, 20},
  {20, 25, 20},
  {25, 20, 25}
};
static UINT8 Icx_Xcc_Hcc_M2iosfToM2UpiVnaBl_4S3link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {25, 20, 25},
  {20, 25, 20},
  {20, 25, 20},
  {20, 25, 20},
  {20, 25, 20},
  {25, 20, 25}
};
static UINT8 Icx_Xcc_Hcc_M2iosfToM2UpiVnaBl_4S2link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {25, 20, 25},
  {20, 25, 20},
  {20, 25, 20},
  {20, 25, 20},
  {20, 25, 20},
  {25, 20, 25}
};
//
// ICX XCC M2IOSF->M2UPI VNA BL credit table for 8S3Link
//
static UINT8 Icx_Xcc_M2iosfToM2UpiVnaBl_8S3link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {20, 18, 20},
  {18, 20, 18},
  {18, 20, 18},
  {18, 20, 18},
  {18, 20, 18},
  {20, 18, 20}
};

//
// ICX HCC M2IOSF->M2UPI VNA BL credit table for 8S3Link
//
static UINT8 Icx_Hcc_M2iosfToM2UpiVnaBl_8S3link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {25, 20, 25},
  {20, 25, 20},
  {20, 25, 20},
  {20, 25, 20},
  {20, 25, 20},
  {25, 20, 25}
};
//
// ICX LCC M2IOSF->M2UPI VNA BL credit table
//
static UINT8 Icx_Lcc_M2iosfToM2UpiVnaBl_2S2link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {30, 25, 00},
  {25, 30, 00},
  {25, 30, 00},
  {00, 00, 00},
  {25, 30, 00},
  {30, 25, 00}
};
static UINT8 Icx_Lcc_M2iosfToM2UpiVnaBl_4S2link[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX] = {
  {30, 25, 00},
  {25, 30, 00},
  {25, 30, 00},
  {00, 00, 00},
  {25, 30, 00},
  {30, 25, 00}
};
/**
  This is a purpose-built version of Memcpy for VN0 AD 2d credit array initialization

  @param GlobalArray - one of the statically defined arrays at the top of this file
  @param LocalArray  - Array to be initialized
**/
VOID
InitializeM2iosfToM2upiCreditArray (
  UINT8 GlobalArray[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX],
  UINT8 LocalArray[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX]
  )
{
  UINT8 Port;
  UINT8 M2iosf;
  for (Port = 0; Port < GetKtiPortCnt(); Port++) {
    for (M2iosf = 0; M2iosf < MAX_IIO_STACK; M2iosf++) {
      LocalArray[M2iosf][Port] = GlobalArray[M2iosf][Port];
    }
  }
}

/**
  This function initializes the credit array parameters based on current topology

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID
  @param Vn0AdCredit       - Array to be initialized
  @param VnaBlCredit       - Array to be initialized
**/
VOID
GetM2iosfToM2upiCreditArrays (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc,
  UINT8                      Vn0AdCredit[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX],
  UINT8                      VnaBlCredit[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX]
  )
{
  UINT8 NumofActiveKtiLink;
  UINT8 ChopTypeInfo;

  NumofActiveKtiLink = CountActiveKtiLink (KtiInternalGlobal, Soc);
  ChopTypeInfo = KtiInternalGlobal->ChopType[KtiInternalGlobal->SbspSoc];
  if (KTIVAR->SysConfig == SYS_CONFIG_8S) {
    switch (ChopTypeInfo) {
      case TypeXcc:
        InitializeM2iosfToM2upiCreditArray (Icx_Xcc_M2iosfToM2UpiVn0Ad_8S3link, Vn0AdCredit);
        InitializeM2iosfToM2upiCreditArray (Icx_Xcc_M2iosfToM2UpiVnaBl_8S3link, VnaBlCredit);
        break;
      case TypeHcc:
        InitializeM2iosfToM2upiCreditArray (Icx_Hcc_M2iosfToM2UpiVn0Ad_8S3link, Vn0AdCredit);
        InitializeM2iosfToM2upiCreditArray (Icx_Hcc_M2iosfToM2UpiVnaBl_8S3link, VnaBlCredit);
        break;
      default:
        KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\nChopType - %d is not supported in M2iosfToM2Upi credit programming!\n", ChopTypeInfo));
        ASSERT (0);
    }
  } else if (KTIVAR->SysConfig == SYS_CONFIG_4S) {
    if (NumofActiveKtiLink == 3) {
      switch (ChopTypeInfo) {
        case TypeXcc:
          InitializeM2iosfToM2upiCreditArray (Icx_Xcc_M2iosfToM2UpiVn0Ad_4S3link, Vn0AdCredit);
          InitializeM2iosfToM2upiCreditArray (Icx_Xcc_Hcc_M2iosfToM2UpiVnaBl_4S3link, VnaBlCredit);
          break;
        case TypeHcc:
          InitializeM2iosfToM2upiCreditArray (Icx_Hcc_M2iosfToM2UpiVn0Ad_4S3link, Vn0AdCredit);
          InitializeM2iosfToM2upiCreditArray (Icx_Xcc_Hcc_M2iosfToM2UpiVnaBl_4S3link, VnaBlCredit);
          break;
        default:
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\nChopType - %d is not supported in M2iosfToM2Upi credit programming!\n", ChopTypeInfo));
          ASSERT (0);
      }
    } else { // (NumofActiveKtiLink == 3)
      switch (ChopTypeInfo) {
        case TypeXcc:
          InitializeM2iosfToM2upiCreditArray (Icx_Xcc_M2iosfToM2UpiVn0Ad_4S2link, Vn0AdCredit);
          InitializeM2iosfToM2upiCreditArray (Icx_Xcc_Hcc_M2iosfToM2UpiVnaBl_4S2link, VnaBlCredit);
          break;
        case TypeHcc:
          InitializeM2iosfToM2upiCreditArray (Icx_Hcc_M2iosfToM2UpiVn0Ad_4S2link, Vn0AdCredit);
          InitializeM2iosfToM2upiCreditArray (Icx_Xcc_Hcc_M2iosfToM2UpiVnaBl_4S2link, VnaBlCredit);
          break;
        case TypeLcc:
          InitializeM2iosfToM2upiCreditArray (Icx_Lcc_M2iosfToM2UpiVn0Ad_4S2link, Vn0AdCredit);
          InitializeM2iosfToM2upiCreditArray (Icx_Lcc_M2iosfToM2UpiVnaBl_4S2link, VnaBlCredit);
          break;
        default:
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\nChopType - %d is not supported in M2iosfToM2Upi credit programming!\n", ChopTypeInfo));
          ASSERT (0);
      }
    }
  } else if (KTIVAR->SysConfig == SYS_CONFIG_2S3L) {
      switch (ChopTypeInfo) {
        case TypeXcc:
          InitializeM2iosfToM2upiCreditArray (Icx_Xcc_M2iosfToM2UpiVn0Ad_2S3link, Vn0AdCredit);
          InitializeM2iosfToM2upiCreditArray (Icx_Xcc_Hcc_M2iosfToM2UpiVnaBl_2S3link, VnaBlCredit);
          break;
        case TypeHcc:
          InitializeM2iosfToM2upiCreditArray (Icx_Hcc_M2iosfToM2UpiVn0Ad_2S3link, Vn0AdCredit);
          InitializeM2iosfToM2upiCreditArray (Icx_Xcc_Hcc_M2iosfToM2UpiVnaBl_2S3link, VnaBlCredit);
          break;
        default:
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\nChopType - %d is not supported in M2iosfToM2Upi credit programming!\n", ChopTypeInfo));
          ASSERT (0);
      }
  } else if (KTIVAR->SysConfig == SYS_CONFIG_2S2L || KTIVAR->SysConfig == SYS_CONFIG_2S1L) {
    switch (ChopTypeInfo) {
      case TypeXcc:
        InitializeM2iosfToM2upiCreditArray (Icx_Xcc_M2iosfToM2UpiVn0Ad_2S2link, Vn0AdCredit);
        InitializeM2iosfToM2upiCreditArray (Icx_Xcc_Hcc_M2iosfToM2UpiVnaBl_2S2link, VnaBlCredit);
        break;
      case TypeHcc:
        InitializeM2iosfToM2upiCreditArray (Icx_Hcc_M2iosfToM2UpiVn0Ad_2S2link, Vn0AdCredit);
        InitializeM2iosfToM2upiCreditArray (Icx_Xcc_Hcc_M2iosfToM2UpiVnaBl_2S2link, VnaBlCredit);
        break;
      case TypeLcc:
        InitializeM2iosfToM2upiCreditArray (Icx_Lcc_M2iosfToM2UpiVn0Ad_2S2link, Vn0AdCredit);
        InitializeM2iosfToM2upiCreditArray (Icx_Lcc_M2iosfToM2UpiVnaBl_2S2link, VnaBlCredit);
        break;
      default:
        KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\nChopType - %d is not supported in M2iosfToM2Upi credit programming!\n", ChopTypeInfo));
        ASSERT (0);
    }
  } else {
    InitializeM2iosfToM2upiCreditArray (Icx_M2iosfToM2UpiVn0Ad_1S, Vn0AdCredit);
    InitializeM2iosfToM2upiCreditArray (Icx_M2iosfToM2UpiVnaBl_1S, VnaBlCredit);
  }
}

/**
  Program M2IOSF to M2UPI AD credits
  Note: This routine does NOT program the M2IOSF BL P2P Credits for M2UPI destination

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramM2iosfToM2upiCreditSetting (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  )
{
  UINT8  Vn0AdCredit[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX];
  UINT8  VnaBlCredit[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX];
  UINT8  Credits;
  UINT32 VN0_AD_Data32;
  UINT8  Port;
  UINT8  Idx1;
  UINT8  NumofActiveKtiLink;

  NumofActiveKtiLink = CountActiveKtiLink (KtiInternalGlobal, Soc);

  GetM2iosfToM2upiCreditArrays (KtiInternalGlobal, Soc, Vn0AdCredit, VnaBlCredit);

  for (Idx1 = 0; Idx1 < MAX_IIO_STACK; Idx1++) {
    if (KtiInternalGlobal->M2PcieActiveBitmap[Soc] & (1 << Idx1)) {
      //
      // M2IOSF->M2UPI AD credits
      //
      VN0_AD_Data32 = 0;
      for (Port = 0; Port < GetKtiPortCnt(); Port++) {
        if (KTIVAR->CpuInfo[Soc].LepInfo[Port].Valid == TRUE) {
          Credits = Vn0AdCredit[Idx1][Port];
          //This credit accumulator is sized to 4 bits instead of 5 on 10nm Wave 1, so only a max of 15 can be returned
          if (Credits > MS2IOSF_TO_UPI_AD_VN0_INGRESS_MAX_10nm) {
            Credits = MS2IOSF_TO_UPI_AD_VN0_INGRESS_MAX_10nm;
          }
          VN0_AD_Data32 |= (Credits << (8 * Port));
        }
      }
      M2UPcieSetUpiCrd (Soc, Idx1, VN0_AD_Data32);
    }
  }
  return KTI_SUCCESS;
}

/**
Program M2IOSF to M2UPI BL P2P credit ring bitfields

@param KtiInternalGlobal - pointer to the KTI RC internal global structure
@param Soc               - Socket ID

@retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramM2iosfToM2upiP2pCredits (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  )
{
  UINT8   Vn0AdCredit[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX];
  UINT8   VnaBlCredit[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX];
  UINT8   NorthCapMask;
  UINT8   SouthCapMask;
  BOOLEAN NorthCapNotProgrammed;
  BOOLEAN SouthCapNotProgrammed;
  UINT8   Idx1;

  GetM2iosfToM2upiCreditArrays (KtiInternalGlobal, Soc, Vn0AdCredit, VnaBlCredit);

  //
  // Use ICX-SP topology M2IOSF ID north/south bit masks, only 10nm CPU type that executes this function
  //
  NorthCapMask = NORTH_CAP_M2IOSF_ID_MASK;
  SouthCapMask = SOUTH_CAP_M2IOSF_ID_MASK;

  NorthCapNotProgrammed = TRUE;
  SouthCapNotProgrammed = TRUE;
  for (Idx1 = 0; Idx1 < MAX_IIO_STACK; Idx1++) {
    if (KTI_HOST_OUT_PTR->M2PciePresentBitmap[Soc] & (1 << Idx1)) {
      //
      // M2IOSF->M2UPI BL credits
      //
      if ((((1 << Idx1) & NorthCapMask) && NorthCapNotProgrammed) ||
          (((1 << Idx1) & SouthCapMask) && SouthCapNotProgrammed)) {

        M2UPcieSetP2PShrCrd (Soc, Idx1, VnaBlCredit[Idx1][0],
                                        VnaBlCredit[Idx1][1],
                                        VnaBlCredit[Idx1][2]);

        if ((1 << Idx1) & NorthCapMask) {
          NorthCapNotProgrammed = FALSE;
        } else if ((1 << Idx1) & SouthCapMask) {
          SouthCapNotProgrammed = FALSE;
        }
      }
    }
  }
  return KTI_SUCCESS;
}

/**
Program M2IOSF to M2UPI credits

@param KtiInternalGlobal - pointer to the KTI RC internal global structure
@param Soc               - Socket ID

@retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramChaToM2upiCreditSetting (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  )
{
  UINT32 RequiredCreditAd;
  UINT32 RequiredCreditBl;
  UINT32 AdVnaCrditPerCha;
  UINT32 BlVnaCrditPerCha;
  UINT8  NumofActiveKtiLink;
  UINT8  Vn0AdCredit[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX];
  UINT8  VnaBlCredit[MAX_IIO_STACK][SI_MAX_KTI_PORTS_ICX];
  UINT8  Port;
  UINT8  Src;
  UINT8  Cha;
  UINT8  NumOfCha;
  UINT8  M2iosfToM2upiVn0AdCredit;
  UINT8  TotalIioStackToM3KtiAdVn0;
  UINT8  M2iosfToM2upiVnaBlCredit;
  UINT8  NorthCapMask;
  UINT8  SouthCapMask;
  BOOLEAN NorthCapNotProgrammed;
  BOOLEAN SouthCapNotProgrammed;
  UINT8  TotalIioStackToM3KtiBlVna;
  UINT8  ChopType;

  //Local variable initialization
  AdVnaCrditPerCha = 0; //CHA->M2UPI (AD)
  BlVnaCrditPerCha = 0; //CHA->M2UPI (BL)
  RequiredCreditAd = 0; //Number of M2UPI Destination credits reserved for other (non CHA) sources
  RequiredCreditBl = 0;
  TotalIioStackToM3KtiAdVn0 = 0; //MS2IOSF->M2UPI (AD) VN0
  TotalIioStackToM3KtiBlVna = 0;
  ChopType = KtiInternalGlobal->ChopType[Soc];
  NumofActiveKtiLink = CountActiveKtiLink (KtiInternalGlobal, Soc);
  NumOfCha = KtiInternalGlobal->TotCha[Soc];
  // Force to min of 2CHA if detect 1CHA
  if (NumOfCha < 2) {
    NumOfCha = 2;
  }

  if (KtiInternalGlobal->CpuType == CPU_ICXD) {
    NorthCapMask = NORTH_CAP_M2IOSF_ID_MASK_ICXD;
    SouthCapMask = SOUTH_CAP_M2IOSF_ID_MASK_ICXD;
  } else {
    NorthCapMask = NORTH_CAP_M2IOSF_ID_MASK;
    SouthCapMask = SOUTH_CAP_M2IOSF_ID_MASK;
  }

  //
  //Get number of credits required for M2UPI route-through
  //
  if (KtiInternalGlobal->IsRouteThrough == TRUE) {
    RequiredCreditAd = KTI_TO_KTI_VN0_AD_INGRESS;                       // VN0
    RequiredCreditBl = KTI_TO_KTI_VN0_BL_INGRESS;
    if (KTIVAR->OutVn1En == TRUE) {
      RequiredCreditAd = RequiredCreditAd + KTI_TO_KTI_VN1_AD_INGRESS;     // VN1
      RequiredCreditBl = RequiredCreditBl + KTI_TO_KTI_VN1_BL_INGRESS;     // VN1
    }
    if (KTIVAR->SysConfig == SYS_CONFIG_8S) {                   //8S
      if (ChopType == TypeXcc) {
        RequiredCreditAd = RequiredCreditAd + XCC_8S_K2K_AD_VNA_10nm;
        RequiredCreditBl = RequiredCreditBl + TOTAL_K2K_BL_VNA_10nm;
      } else {
        RequiredCreditAd = RequiredCreditAd + MIN_K2K_AD_VNA_10nm;
        RequiredCreditBl = RequiredCreditBl + MIN_K2K_BL_VNA_10nm;
      }
    } else {                                                         //4S, route-through (like 4s ring)
      RequiredCreditAd = RequiredCreditAd + TOTAL_K2K_AD_VNA_10nm;
      RequiredCreditBl = RequiredCreditBl + MIN_K2K_BL_VNA_10nm;
    }
    RequiredCreditAd = RequiredCreditAd * (NumofActiveKtiLink - 1);
    RequiredCreditBl = RequiredCreditBl * (NumofActiveKtiLink - 1);
  }

  //
  // Get the maximum number of M2IOSF->M2UPI VN0-AD and VNA-BL credits for this topology
  // Calculate value of TotalIioStackToM3KtiAdVn0 and TotalIioStackToM3KtiBlVna
  //
  NorthCapNotProgrammed = TRUE;
  SouthCapNotProgrammed = TRUE;
  GetM2iosfToM2upiCreditArrays (KtiInternalGlobal, Soc, Vn0AdCredit, VnaBlCredit);
  for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
    M2iosfToM2upiVn0AdCredit = 0;
    M2iosfToM2upiVnaBlCredit = 0;
    if (KTIVAR->CpuInfo[Soc].LepInfo[Port].Valid == TRUE) {
      for (Src = 0; Src < MAX_IIO_STACK; Src++) {
        if (KtiInternalGlobal->M2PcieActiveBitmap[Soc] & (1 << Src)) {
          M2iosfToM2upiVn0AdCredit = M2iosfToM2upiVn0AdCredit + Vn0AdCredit[Src][Port];
          if ((((1 << Src) & NorthCapMask) && NorthCapNotProgrammed)) {
            M2iosfToM2upiVnaBlCredit = M2iosfToM2upiVnaBlCredit + VnaBlCredit[Src][Port];
            NorthCapNotProgrammed = FALSE;
          } else if (((1 << Src) & SouthCapMask) && SouthCapNotProgrammed) {
            M2iosfToM2upiVnaBlCredit = M2iosfToM2upiVnaBlCredit + VnaBlCredit[Src][Port];
            SouthCapNotProgrammed = FALSE;
          }
        }
      }
    }
    if (M2iosfToM2upiVn0AdCredit > TotalIioStackToM3KtiAdVn0) {
      TotalIioStackToM3KtiAdVn0 = M2iosfToM2upiVn0AdCredit;
    }
    if (M2iosfToM2upiVnaBlCredit > TotalIioStackToM3KtiBlVna) {
      TotalIioStackToM3KtiBlVna = M2iosfToM2upiVnaBlCredit;
    }
    NorthCapNotProgrammed = TRUE;
    SouthCapNotProgrammed = TRUE;
  }

  //RequiredCreditAd += (CHA->M2UPI (AD) shared credits) + (UBOX->M2UPI (AD) credits) + (sum of MS2IOSF->M2UPI (AD) credits)
  RequiredCreditAd = RequiredCreditAd + CHA_TO_KTI_VN0_AD_INGRESS + UBOX_TO_KTI_VN0_AD_INGRESS_10nm + TotalIioStackToM3KtiAdVn0;        // (CHA->KTI VN0) + (IIO stack -> M3KTI AD VN0)
  RequiredCreditBl = RequiredCreditBl + CHA_TO_KTI_VN0_BL_INGRESS + UBOX_TO_KTI_VN0_BL_INGRESS_10nm + (MS2IOSF_TO_UPI_BL_VN0_INGRESS_10nm * KtiInternalGlobal->TotActiveM2Pcie[Soc]) + TotalIioStackToM3KtiBlVna;

  if (KTIVAR->SysConfig == SYS_CONFIG_8S) {
    RequiredCreditBl += M2M_TO_KTI_VNA_BL_INGRESS_10nm_8S * M2M_PER_SOCKET_10nm;
  } else if ((KTIVAR->SysConfig == SYS_CONFIG_4S) && (KtiInternalGlobal->IsRouteThrough == TRUE)) {
    if ((ChopType == TypeXcc) || (ChopType == TypeLcc)) {
      RequiredCreditBl += M2M_TO_KTI_VNA_BL_INGRESS_10nm_4S2L_XCC_LCC * M2M_PER_SOCKET_10nm;
    } else {
      RequiredCreditBl += M2M_TO_KTI_VNA_BL_INGRESS_10nm_4S2L_HCC * M2M_PER_SOCKET_10nm;
    }
  } else {
    RequiredCreditBl += M2M_TO_KTI_VNA_BL_INGRESS_10nm * M2M_PER_SOCKET_10nm;
  }

  //Assert that we have not run out of credits yet
  KTI_ASSERT (TOTAL_UPI_AD_CREDIT_10nm > RequiredCreditAd, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_2);
  KTI_ASSERT (TOTAL_UPI_BL_CREDIT_10nm > RequiredCreditBl, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_2);//Give this a unique minor error code?

  AdVnaCrditPerCha = (TOTAL_UPI_AD_CREDIT_10nm - RequiredCreditAd) / NumOfCha;

  BlVnaCrditPerCha = (TOTAL_UPI_BL_CREDIT_10nm - RequiredCreditBl) / NumOfCha;

  //ICX-SP restriction from cms on akc credit ring can only accommodate maximum of 3 CHA AD VNA credits (MAX_CHA_TO_M2UPI_AD_CREDIT)
  if (AdVnaCrditPerCha > MAX_CHA_TO_M2UPI_AD_CREDIT) {
    AdVnaCrditPerCha = MAX_CHA_TO_M2UPI_AD_CREDIT;
  }
  //Only 4 bits only in this bitfield, avoid overflow
  if (BlVnaCrditPerCha > MAX_CREDIT) {
    BlVnaCrditPerCha = MAX_CREDIT;
  }

  //
  //Program CHA registers
  //
  for (Cha = 0; Cha < KtiInternalGlobal->TotCha[Soc]; Cha++) {
    ChaConfigureChaToM2UpiCredit (Soc, Cha, AdVnaCrditPerCha, BlVnaCrditPerCha);
  }
  return KTI_SUCCESS;
}

/**
  Program M3KTI credits.

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramM3KtiCreditSetting (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      Soc
  )
{
  UINT8 NumofActiveKtiLink;
  UINT8 Ctr;
  //
  //UPI->CHA
  //
  UINT32 GeneralRrq = 0;
  //
  //M2UPI->M2UPI (AD and BL)
  //
  UINT32 M2upiRouteThruAdVna;
  UINT32 M2upiRouteThruAdVn0;
  UINT32 M2upiRouteThruBlVna;
  UINT32 M2upiRouteThruBlVn0;
  UINT8  ChopType;

  ChopType = KtiInternalGlobal->ChopType[Soc];
  NumofActiveKtiLink = CountActiveKtiLink (KtiInternalGlobal, Soc);
  //
  //M2UPI->M2UPI credits
  //
  M2upiRouteThruAdVna = 0;
  M2upiRouteThruAdVn0 = 0;
  M2upiRouteThruBlVna = 0;
  M2upiRouteThruBlVn0 = 0;

  if (KtiInternalGlobal->IsRouteThrough == TRUE) {
    if (KTIVAR->SysConfig == SYS_CONFIG_8S) {
      //
      //8S topology credit settings
      //

      if (ChopType == TypeXcc) {
        M2upiRouteThruAdVna = XCC_8S_K2K_AD_VNA_10nm;
        M2upiRouteThruBlVna = TOTAL_K2K_BL_VNA_10nm;
      } else {
        M2upiRouteThruAdVna = MIN_K2K_AD_VNA_10nm;
        M2upiRouteThruBlVna = MIN_K2K_BL_VNA_10nm;
      }
    } else {
      //
      //4S 2L topology credit settings
      //
      M2upiRouteThruAdVna = TOTAL_K2K_AD_VNA_10nm;
      M2upiRouteThruBlVna = MIN_K2K_BL_VNA_10nm;
    }
    M2upiRouteThruAdVn0 = KTI_TO_KTI_VN0_AD_INGRESS;
    M2upiRouteThruBlVn0 = KTI_TO_KTI_VN0_BL_INGRESS;
  }

  //
  //UPI->CHA General RRQ credits
  //
  KTI_ASSERT((NumofActiveKtiLink != 0), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_7E);
  if (NumofActiveKtiLink > 0) {
    GeneralRrq = (TOTAL_PRQ_10nm / NumofActiveKtiLink) - RPQ_VNA_PER_UPI_10nm;
  }

  //
  //UPI->M2PCIE
  //
  for (Ctr = 0; Ctr < KtiInternalGlobal->KtiPortCnt; Ctr++) {
    if ((KTIVAR->CpuInfo[Soc].LepInfo[Ctr].Valid == TRUE) ||
        (KTIVAR->OutKtiCpuSktHotPlugEn == TRUE && (KtiInternalGlobal->KtiPortPresentBitmap[Soc] & (1 << Ctr)) != 0)) {
      ProgramM3KtiNcbNcsCredits (Soc, Ctr);
      //
      // M3KEGRCTRL0/M3KEGRCTRL1 PRQ_VNA (UPI->CHA)
      // M3KTI is 1:1 mapped to KtiPort
      //
      ProgramM3KtiRpqVnaCredits (Soc, Ctr, GeneralRrq);
      ProgramM3KtiRouteThruCredits (Soc, Ctr, M2upiRouteThruAdVna, M2upiRouteThruAdVn0, M2upiRouteThruBlVna, M2upiRouteThruBlVn0);
    } //If This UPI port is valid
  } //For each UPI port
  return KTI_SUCCESS;
}
/**
Program M2MEM to M2UPI credits

@param KtiInternalGlobal - pointer to the KTI RC internal global structure
@param Soc               - Socket ID

@retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramM2memToM2upiCreditSetting (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  )
{
  UINT8 BlCreditPerM2m; //Number of credits to program per M2M
  UINT8 RequiredCredit; //Number of credits already assigned to M2UPI BL credit buffer
  UINT8 NumOfCha;
  UINT8 NumofActiveKtiLink;
  UINT8 Ctr;
  UINT8 Port;
  UINT8 ChopType;
  ChopType = KtiInternalGlobal->ChopType[Soc];
  RequiredCredit = 0;
  BlCreditPerM2m = 0;


  //CHA->M2UPI VNA (BL)
  NumOfCha = KtiInternalGlobal->TotCha[Soc];
  RequiredCredit += NumOfCha;

  //CHA->M2UPI BL VN0
  RequiredCredit += CHA_TO_KTI_VN0_BL_INGRESS; //from CHA->M2UPI VNA (BL)

  //UBOX->M2UPI
  RequiredCredit += UBOX_TO_KTI_VN0_BL_INGRESS_10nm;

  //MS2IOSF->M2UPI
  RequiredCredit += (MS2IOSF_TO_UPI_BL_VN0_INGRESS_10nm * KtiInternalGlobal->TotActiveM2Pcie[Soc]); // Make this use 38 credits in 8S-3L
  if (KTIVAR->SysConfig == SYS_CONFIG_8S) {
    RequiredCredit += MS2IOSF_TO_UPI_BL_VNA_INGRESS_8S_10nm;
  } else {
    RequiredCredit += MS2IOSF_TO_UPI_BL_VNA_INGRESS_10nm;
  }

  if (KtiInternalGlobal->IsRouteThrough == TRUE) {
    //From M2UPI->M2UPI BL VNA/VN0
    //These are calculated in M3Kti function in a similar way.
    NumofActiveKtiLink = CountActiveKtiLink (KtiInternalGlobal, Soc);
    if (KTIVAR->SysConfig == SYS_CONFIG_8S) {
      if (ChopType == TypeXcc) {
        RequiredCredit += ((KTI_TO_KTI_VN0_BL_INGRESS * 2) + TOTAL_K2K_BL_VNA_10nm) * (NumofActiveKtiLink - 1);    // VN0 + VN1 + VNA
      } else {
        RequiredCredit += ((KTI_TO_KTI_VN0_BL_INGRESS * 2) + MIN_K2K_BL_VNA_10nm) * (NumofActiveKtiLink - 1);    // VN0 + VN1 + VNA
      }
    } else {
      RequiredCredit += (KTI_TO_KTI_VN0_BL_INGRESS + MIN_K2K_BL_VNA_10nm) * (NumofActiveKtiLink - 1);    // VN0 + VNA
    }
  }

  //Calculate number of BL credits per M2M
  BlCreditPerM2m = (TOTAL_UPI_BL_CREDIT_10nm - RequiredCredit) / (UINT8) (KTIVAR->CpuInfo[Soc].TotM2m);
  if (BlCreditPerM2m > MAX_M2M_TO_M2UPI_BL_VNA) {
    BlCreditPerM2m = MAX_M2M_TO_M2UPI_BL_VNA;
  }
  //Assign credits Per UPI port, Per M2M
  for (Ctr = 0; Ctr < KTIVAR->CpuInfo[Soc].TotM2m; Ctr++) {
    for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
      if (KTIVAR->CpuInfo[Soc].LepInfo[Port].Valid == TRUE) {
        SetM2mCreditReg (Soc, Ctr, Port, 0xf, BlCreditPerM2m);
      }
    }
  }
  return KTI_SUCCESS;
}
/**
  Program Kti Agent credits.

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramKtiCreditSetting (
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN UINT8                     Soc
  )
{
  return KTI_SUCCESS;
}

/**
  Program Vna Bl credits for destination M2UPI (when cleard on reset)

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramM2MVnaBlCreditForDestM2Upi (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      Soc
  )
{
  //
  // Empty function for API consistency
  //
  return KTI_SUCCESS;
}