/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation. <BR>

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
#include <Library/M2IalIpLib.h>
#include <Library/KtiApi.h>
#include <Guid/KtiIpInterface.h>

extern UINT8 Spr_SharedP2pCreditBlVnaM2ial2M3kti[SPR_TOPOLOGY_MAX][4];

//
// SPR M2UPI destination credit arrays
//
static UINT8 Spr_M2UpiDestinationCreditAd[SPR_TOPOLOGY_MAX][5] = {
//UBOX VN0, M2UPI VNA, M2UPI VN0 ,M2UPI VN1, Core VN0
  {0,       0,         0,          0,         0       },  // 1S
  {1,       0,         0,          0,         8       },  // 2S1L
  {1,       0,         0,          0,         8       },  // 2S2L
  {1,       0,         0,          0,         8       },  // 2S3L
  {1,       0,         0,          0,         8       },  // 2S4L
  {1,       0,         0,          0,         8       },  // 4S3L
  {1,       15,        1*3,        0,         8       },  // 4S2L
  {1,       15,        1*3,        1*3,       8       }   // 8S
};


static UINT8 Spr_M2UpiDestinationCreditAd_Cha2M2UPIVN0[4] = { 3, 3, 6, 12 };  // LCC, MCC, HCC, XCC

static UINT8 Spr_M2UpiDestinationCreditAd_Ms2IOSF2M2UPIVN0[SPR_TOPOLOGY_MAX][4] = {
//LCC       MCC      HCC     XCC
  {0,       0,       0,       0       },  // 1S
  {14,      15,      13,      11      },  //2S1L
  {14,      15,      13,      11      },  //2S2L
  {14,      15,      13,      11      },  //2S3L
  {0,       0,       13,      11      },  //2S4L
  {14,      15,      13,      11      },  //4S3L
  {11,      12,      11,      9       },  //4S2L
  {9,       11,      10,      8       }   //8S3L
};

static UINT8 Ms2iosf2M2UpiCreditTableTopologyMapping[] = {
  0,  // SPR-2S-1Link
  0,  // SPR-2S-2Link
  0,  // SPR-2S-3Link
  0,  // SPR-2S-4Link
  0,  // SPR-4S-3Link
  1,  // SPR-4S-2Link
  2   // SPR-8S-3Link
};

 static UINT8 M2upiDestinationCreditAd_Ms2iosf_Xcc[3][4][12] = {
  {
    {
       8,  9,  3, 16, 15, 26,  0,  0,  6,  1,  1, 30
    }, // UPI0
    {
      17, 16,  5,  8, 10, 21,  0,  0, 18,  1,  1, 18
    }, //UPI1
    {
      15, 16,  2, 21, 21, 12,  0,  0, 13,  1,  1, 13
    }, //UPI2
    {
      21, 21,  2, 16, 17,  7,  0,  0, 24,  1,  1,  5
    }, //UPI3
  }, // SPR-2S-1Link /SPR-2S-2Link /SPR-2S-3Link /SPR-2S-4Link /SPR-4S-3Link

  {
    {
       7,  8,  3, 13, 12, 21,  0,  0,  5,  1,  1, 24
    }, // UPI0
    {
      14, 13,  4,  7,  8, 17,  0,  0, 15,  1,  1, 15
    }, //UPI1
    {
      13, 13,  1, 18, 17,  9,  0,  0, 11,  1,  1, 11
    }, //UPI2
    {
      18, 17,  2, 13, 14,  5,  0,  0, 20,  1,  1,  4
    }, //UPI3
  }, // SPR-4S-2Link

  {
    {
       6,  7,  3, 12, 11, 20,  0,  0,  4,  1,  1, 23
    }, // UPI0
    {
      13, 12,  4,  6,  7, 16,  0,  0, 14,  1,  1, 14
    }, //UPI1
    {
      12, 12,  1, 16, 16,  9,  0,  0, 10,  1,  1, 10
    }, //UPI2
    {
      17, 16,  1, 12, 13,  5,  0,  0, 18,  1,  1,  4
    } //UPI3
  } // SPR-8S-3Link
};

//MS2IOSF VNA in below table is different between XCC/MCC/UCC ...
static UINT8 Spr_M2UpiDestinationCreditBl[SPR_TOPOLOGY_MAX][7] = {
//M2M VNA, UBOX VN0, MS2IOSF VNA, MS2IOSF VN0, M2UPI VNA, M2UPI VN0 ,M2UPI VN1
  {0,       0,        0,           0,           0,         0,          0        },  // 1S
  {7,       1*3,      69,          1*2,         0,         0,          0        },  // 2S1L
  {7,       1*3,      69,          1*2,         0,         0,          0        },  // 2S2L
  {7,       1*3,      69,          1*2,         0,         0,          0        },  // 2S3L
  {7,       1*3,      69,          1*2,         0,         0,          0        },  // 2S4L
  {7,       1*3,      69,          1*2,         0,         0,          0        },  // 4S3L
  {6,       1*3,      59,          1*2,         10,        1*4,        0        },  // 4S2L
  {5,       1*3,      46,          1*2,         15,        2*4,        2*4      }   // 8S
};


static UINT8 Spr_M2UpiDestinationCreditBl_CHA2M2UPIVN0[SPR_TOPOLOGY_MAX][4] = {
//LCC       MCC      HCC       XCC
  {0,       0,       0,        0,        },  // 1S
  {1*4,     1*4,     2*4,      4*4       },  // 2S1L
  {1*4,     1*4,     2*4,      4*4       },  // 2S2L
  {1*4,     1*4,     2*4,      4*4       },  // 2S3L
  {0,       0,       2*4,      4*4       },  // 2S4L
  {1*4,     1*4,     2*4,      4*4       },  // 4S3L
  {1*4,     1*4,     2*4,      4*4       },  // 4S2L
  {1*4,     1*4,     2*4,      4*4       }   // 8S
};

//
// SPR CHA destination credit arrays
//
static UINT8 Spr_Upi2ChaVnaCredit[SPR_TOPOLOGY_MAX] = {
//UPI RPQ VNA (Per UPI active link)
  0,            // 1S
  1*3,          // 2S1L
  1*3,          // 2S2L
  1*3,          // 2S3L
  1*3,          // 2S4L
  1*3,          // 4S3L
  1*3,          // 4S2L
  1*3           // 8S
};

/**
  Routine to get topology for credit programming.

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval TOPOLOGY4CREDIT  - Topology defined as TOPOLOGY4CREDIT enum.

**/
TOPOLOGY4CREDIT
GetTopology4Credit (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  )
{
  TOPOLOGY4CREDIT Topology;

  switch (KTIVAR->SysConfig) {
    case SYS_CONFIG_2S1L:
      Topology = SPR_2S1L;
      break;
    case SYS_CONFIG_2S2L:
      Topology = SPR_2S2L;
      break;
    case SYS_CONFIG_2S3L:
      Topology = SPR_2S3L;
      break;
    case SYS_CONFIG_2S4L:
      Topology = SPR_2S4L;
      break;
    case SYS_CONFIG_4S:
      if (KtiInternalGlobal->Is4SRing) {
        Topology = SPR_4S2L;
      } else {
        Topology = SPR_4S3L;
      }
      break;
    case SYS_CONFIG_8S:
      Topology = SPR_8S;
      break;
    default:
      Topology = SPR_1S;
  }
  return Topology;
}

/**
  Program all the credits for destination M2UPI.

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
ProgramCreditForDestM2Upi (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  )
{
  UINT8           Cha;
  UINT8           Stack;
  UINT8           Port;
  UINT8           NumOfCha;
  UINT8           NumofActiveKtiLink;
  SYS_CONFIG      SysConfig;
  TOPOLOGY4CREDIT Topology;
  UINT32          AdCha2M2upiVnaCredit;
  UINT8           AdCha2M2upiVn0Credit;
  UINT8           AdUbox2M2upiVn0Credit;
  UINT8           AdMs2iosf2M2upiVn0Credit;
  UINT8           AdM2upi2M2upiVnaCredit;
  UINT8           AdM2upi2M2upiVn0Credit;
  UINT8           AdM2upi2M2upiVn1Credit;
  UINT8           AdCore2M2upiVn0Credit;
  UINT32          BlCha2M2upiVnaCredit;
  UINT8           BlCha2M2upiVn0Credit;
  UINT8           BlM2m2M2upiVnaCredit;
  UINT8           BlUbox2M2upiVn0Credit;
  UINT8           BlMs2iosf2M2upiVnaCredit;
  UINT8           BlMs2iosf2M2upiVn0Credit;
  UINT8           BlM2upi2M2upiVnaCredit;
  UINT8           BlM2upi2M2upiVn0Credit;
  UINT8           BlM2upi2M2upiVn1Credit;
  UINT32          VN0_AD_Data32;
  UINT8           AdMs2iosf2M2upiVn0CreditTotal;
  UINT8           AdMs2iosf2M2UpiVn0CreditPerPort[4];
  UINT8           Ms2iosfCreditTopologyIndex;
  UINT8           TempCredit;

  SysConfig = KTIVAR->SysConfig;
  //
  // Skip all credit programming for single socket configuration.
  //
  if (SysConfig == SYS_CONFIG_1S) {
    return KTI_SUCCESS;
  }

  NumofActiveKtiLink = CountActiveKtiLink(KtiInternalGlobal, Soc);
  if (NumofActiveKtiLink < 1) {
    return KTI_SUCCESS;
  }
  NumOfCha = KtiInternalGlobal->TotCha[Soc];
  // Force to min of 2CHA if detect 1CHA
  if (NumOfCha < 2) {
    NumOfCha = 2;
  }

  Topology = GetTopology4Credit (KtiInternalGlobal, Soc);

  AdCha2M2upiVn0Credit     = Spr_M2UpiDestinationCreditAd_Cha2M2UPIVN0[KtiInternalGlobal->ChopType[Soc]];
  AdUbox2M2upiVn0Credit    = Spr_M2UpiDestinationCreditAd[Topology][0];
  AdMs2iosf2M2upiVn0Credit = Spr_M2UpiDestinationCreditAd_Ms2IOSF2M2UPIVN0[Topology][KtiInternalGlobal->ChopType[Soc]];
  AdM2upi2M2upiVnaCredit   = Spr_M2UpiDestinationCreditAd[Topology][1];
  AdM2upi2M2upiVn0Credit   = Spr_M2UpiDestinationCreditAd[Topology][2];
  AdM2upi2M2upiVn1Credit   = Spr_M2UpiDestinationCreditAd[Topology][3];
  AdCore2M2upiVn0Credit    = Spr_M2UpiDestinationCreditAd[Topology][4];

  BlCha2M2upiVn0Credit     = Spr_M2UpiDestinationCreditBl_CHA2M2UPIVN0[Topology][KtiInternalGlobal->ChopType[Soc]];
  BlM2m2M2upiVnaCredit     = Spr_M2UpiDestinationCreditBl[Topology][0];
  BlUbox2M2upiVn0Credit    = Spr_M2UpiDestinationCreditBl[Topology][1];
  BlMs2iosf2M2upiVnaCredit = Spr_SharedP2pCreditBlVnaM2ial2M3kti[Topology][KtiInternalGlobal->ChopType[Soc]];
  BlMs2iosf2M2upiVn0Credit = Spr_M2UpiDestinationCreditBl[Topology][3];
  BlM2upi2M2upiVnaCredit   = Spr_M2UpiDestinationCreditBl[Topology][4];
  BlM2upi2M2upiVn0Credit   = Spr_M2UpiDestinationCreditBl[Topology][5];
  BlM2upi2M2upiVn1Credit   = Spr_M2UpiDestinationCreditBl[Topology][6];

  //
  // Program MS2IOSF->M2UPI VNA credit registers
  //
  AdMs2iosf2M2upiVn0CreditTotal = 0;
  Ms2iosfCreditTopologyIndex = Ms2iosf2M2UpiCreditTableTopologyMapping[Topology - 1];
  AdMs2iosf2M2UpiVn0CreditPerPort[0] = AdMs2iosf2M2UpiVn0CreditPerPort[1] = AdMs2iosf2M2UpiVn0CreditPerPort[2] = AdMs2iosf2M2UpiVn0CreditPerPort[3] = 0;

  if (KtiInternalGlobal->ChopType[Soc] == TypeXcc) {
    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      if (KtiInternalGlobal->M2PcieActiveBitmap[Soc] & (1 << Stack)) {
        VN0_AD_Data32 = 0;
        for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
          if (KTIVAR->CpuInfo[Soc].LepInfo[Port].Valid == TRUE) {
            TempCredit = M2upiDestinationCreditAd_Ms2iosf_Xcc[Ms2iosfCreditTopologyIndex][Port][Stack];
            AdMs2iosf2M2UpiVn0CreditPerPort[Port] += TempCredit;
            VN0_AD_Data32 |= (TempCredit << (8 * Port));
          }
        }
        M2IalSetUpiCrd (Soc, Stack, VN0_AD_Data32);
      }
    }

    AdMs2iosf2M2upiVn0CreditTotal = MAX (MAX (MAX (AdMs2iosf2M2UpiVn0CreditPerPort[0], AdMs2iosf2M2UpiVn0CreditPerPort[1]), AdMs2iosf2M2UpiVn0CreditPerPort[2]), AdMs2iosf2M2UpiVn0CreditPerPort[3]);

  } else {

    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      if (KtiInternalGlobal->M2PcieActiveBitmap[Soc] & (1 << Stack)) {
        VN0_AD_Data32 = 0;
        for (Port = 0; Port < (KtiInternalGlobal->KtiPortCnt); Port++) {
          if (KTIVAR->CpuInfo[Soc].LepInfo[Port].Valid == TRUE) {
            VN0_AD_Data32 |= (AdMs2iosf2M2upiVn0Credit << (8 * Port));
          }
        }
        M2IalSetUpiCrd (Soc, Stack, VN0_AD_Data32);
      }
    }
    AdMs2iosf2M2upiVn0CreditTotal = AdMs2iosf2M2upiVn0Credit * KtiInternalGlobal->TotActiveM2Pcie[Soc];
  }

  if (AdM2upi2M2upiVnaCredit > 15) {
    AdM2upi2M2upiVnaCredit = 15;
  }

  if (!(KTIVAR->OutVn1En)) {
    AdM2upi2M2upiVn1Credit = 0;
    BlM2upi2M2upiVn1Credit = 0;
  }

  if (!(KtiInternalGlobal->IsRouteThrough)) {
    AdM2upi2M2upiVnaCredit = 0;
    AdM2upi2M2upiVn0Credit = 0;

    BlM2upi2M2upiVnaCredit = 0;
    BlM2upi2M2upiVn0Credit = 0;
  }

  AdCha2M2upiVnaCredit = (TOTAL_UPI_AD_CREDIT_SPR -
                           (AdCha2M2upiVn0Credit +
                            AdUbox2M2upiVn0Credit +
                            AdMs2iosf2M2upiVn0CreditTotal +
                            AdM2upi2M2upiVnaCredit +
                            (AdM2upi2M2upiVn0Credit + AdM2upi2M2upiVn1Credit) * (NumofActiveKtiLink - 1) +
                            AdCore2M2upiVn0Credit
                           )
                         ) / NumOfCha;

  if (AdCha2M2upiVnaCredit > 15) {
    AdCha2M2upiVnaCredit = 15;
  }

  BlCha2M2upiVnaCredit = (TOTAL_UPI_BL_CREDIT_SPR -
                           (BlCha2M2upiVn0Credit +
                            BlM2m2M2upiVnaCredit * KTIVAR->CpuInfo[Soc].TotM2m +
                            BlUbox2M2upiVn0Credit +
                            BlMs2iosf2M2upiVnaCredit +
                            BlMs2iosf2M2upiVn0Credit * KtiInternalGlobal->TotActiveM2Pcie[Soc] +
                            BlM2upi2M2upiVnaCredit +
                            (BlM2upi2M2upiVn0Credit + BlM2upi2M2upiVn1Credit) * (NumofActiveKtiLink - 1)
                           )
                         ) / NumOfCha;

  if (BlCha2M2upiVnaCredit > 15) {
    BlCha2M2upiVnaCredit = 15;
  }

  if ((KTIVAR->CpuInfo[Soc].DieCount == ONE_DIE_IN_SOCKET) && (KtiInternalGlobal->ChopType[Soc] == TypeXcc)) {
    AdCha2M2upiVnaCredit = 2;
    BlCha2M2upiVnaCredit = 2;
  }
  //
  // Program CHA->M2UPI VNA credit registers
  //
  for (Cha = 0; Cha < KtiInternalGlobal->TotCha[Soc]; Cha++) {
    ChaConfigureChaToM2UpiCredit (Soc, Cha, AdCha2M2upiVnaCredit, BlCha2M2upiVnaCredit);
  }

  //
  // Program MS2UPI->M2UPI VNA credit registers
  //
  for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
    if (KTIVAR->CpuInfo[Soc].LepInfo[Port].Valid == TRUE) {
      ProgramM3KtiRouteThruCredits (Soc,
                                    Port,
                                    AdM2upi2M2upiVnaCredit,
                                    AdM2upi2M2upiVn0Credit,
                                    BlM2upi2M2upiVnaCredit,
                                    BlM2upi2M2upiVn0Credit);
    }
  }

  return KTI_SUCCESS;
}

/**
  Program all the credits for destination CHA.

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
ProgramCreditForDestCha (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  )
{
  UINT8           Port;
  UINT8           NumofActiveKtiLink;
  TOPOLOGY4CREDIT Topology;
  UINT32          Upi2ChaRpqVnaCredit;
  UINT32          Upi2ChaGeneralRrqCredit;

  NumofActiveKtiLink = CountActiveKtiLink (KtiInternalGlobal, Soc);
  Topology = GetTopology4Credit (KtiInternalGlobal, Soc);

  Upi2ChaRpqVnaCredit = Spr_Upi2ChaVnaCredit[Topology];

  if (NumofActiveKtiLink > 0) {
    //
    // Calculate UPI->CHA RRQ credit
    //
    Upi2ChaGeneralRrqCredit = TOTAL_UPI2CHA_CREDIT_SPR / NumofActiveKtiLink - Upi2ChaRpqVnaCredit;

    //
    // Program UPI->CHA RPQ VNA credit registers
    //
    for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
      if (KTIVAR->CpuInfo[Soc].LepInfo[Port].Valid == TRUE) {
        ProgramM3KtiRpqVnaCredits(Soc, Port, Upi2ChaGeneralRrqCredit);
      }
    }
  }

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
  UINT8           Port;
  UINT8           M2m;
  UINT8           BlM2m2M2upiVnaCredit;
  UINT8           NumofActiveKtiLink;
  SYS_CONFIG      SysConfig;
  TOPOLOGY4CREDIT Topology;

  SysConfig = KTIVAR->SysConfig;
  //
  // Skip all credit programming for single socket configuration.
  //
  if (SysConfig == SYS_CONFIG_1S) {
    return KTI_SUCCESS;
  }

  NumofActiveKtiLink = CountActiveKtiLink(KtiInternalGlobal, Soc);
  if (NumofActiveKtiLink < 1) {
    return KTI_SUCCESS;
  }

  Topology = GetTopology4Credit (KtiInternalGlobal, Soc);


  BlM2m2M2upiVnaCredit     = Spr_M2UpiDestinationCreditBl[Topology][0];

  //
  // Program M2M->M2UPI VNA credit registers
  //
  for (M2m = 0; M2m < KTIVAR->MaxSupportedM2m; M2m++) {
    if ((KTIVAR->CpuInfo[Soc].M2mBitmap & (1 << M2m)) == 0) {
      continue;
    }
    for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
      if (KTIVAR->CpuInfo[Soc].LepInfo[Port].Valid == TRUE) {
        SetM2mCreditReg (Soc, M2m, Port, 0xf, BlM2m2M2upiVnaCredit);
      }
    }
  }
  return KTI_SUCCESS;
}