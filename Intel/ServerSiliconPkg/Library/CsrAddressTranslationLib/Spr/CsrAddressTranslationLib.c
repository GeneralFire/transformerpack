/** @file
  This library class translates Virtual CSR addresses to
  physical CSR addresses.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include "CsrAddressTranslationLibInternal.h"

CONST CHAR8 INVALID_BOX_INSTANCE_MESSAGE[] = "Invalid Box 0x%02x Instance 0x%02x\n";
CONST CHAR8 INVALID_FUN_INSTANCE_MESSAGE[] = "Invalid Fun 0x%02x\n";
CONST CHAR8 BAR_NOT_INITIALIZED[] = "[CSR PCI BAR Access] PCI BAR is not initialized !\n";
CONST CHAR8 BAR_64_NOT_INITIALIZED[] = "[CSR PCI BAR Access] PCI BAR 64 is not initialized !\n";
CONST CHAR8 BAR_64_NOT_IN_PEI[] = "[CSR PCI BAR Access] PCI BAR 64 bit can not access in PEI phase!\n";

GET_CSR_PHYSICAL_ADDRESS mPhyAddrFuncTable[MAX_BOX_TYPES][MAX_FUNCBLK] = {
  //  FuncBlok0       FuncBlok1         FuncBlok2          FuncBlok3          FuncBlok4         FuncBlok5           FuncBlok6           FuncBlok7
  { CNVT_CHA_MISC,    NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType00  CHA MISC
  { CNVT_CHA_PMA,     NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType01  CHA PMA
  { NULL,             NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType02  CHA CMS
  { CNVT_CHA_ABC,     CNVT_CHA_ABC,     NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType03  CHABC
  { CNVT_PCU,         CNVT_PCU,         CNVT_PCU,          CNVT_PCU,          CNVT_PCU,         CNVT_PCU,           CNVT_PCU,           CNVT_PCU,      },  //BoxType04  PCU
  { NULL,             NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType05  VCU
  { CNVT_M2MEM,       NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType06  M2MEM
  { CNVT_DDR_MC,      CNVT_DDR_MC,      CNVT_DDR_MC_GLOBAL,NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType07  MC
  { CNVT_MCIO,        CNVT_MCIO,        CNVT_MCIO,         NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType08  MCIO DDRIO
  { CNVT_KTI,         CNVT_KTI,         CNVT_KTI,          CNVT_KTI,          NULL,             NULL,               NULL,               NULL,          },  //BoxType09  KTI
  { CNVT_M3KTI,       CNVT_M3KTI,       CNVT_M3KTI,        NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType10  M3KTI
  { CNVT_DDR_MCDDC,   CNVT_DDR_MCDDC,   NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType11  MCDDC
  { NULL,             NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType12  M2UPCIE
  { CNVT_IIO_PCIEDMI, NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType13  IIO PCIEDMI
  { NULL,             CNVT_IIO_PCIE,    CNVT_IIO_EXPPTMBAR,NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType14  IIO PCIE
  { CNVT_IIO_PCIENTB, NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType15  IIO PCIENTB
  { NULL,             NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType16  IIOCB
  { CNVT_IIO_VTD,     NULL,             CNVT_IIO_VTD,      NULL,              CNVT_IIO_VTD,     NULL,               NULL,               NULL,          },  //BoxType17  IIO VTD
  { NULL,             NULL,             NULL,              NULL,              CNVT_IIO_DFX,     NULL,               NULL,               NULL,          },  //BoxType18  IIO DFX
  { CNVT_UBOX,        CNVT_UBOX,        CNVT_UBOX,         NULL,              NULL,             CNVT_UBOX,          NULL,               NULL           },  //BoxType19  UBOX
  { CNVT_MS2IDI_MAIN, NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType20  MS2IDI
  { CNVT_HBM_AWORD,   CNVT_HBM_DWORD,   CNVT_HBM_MIDSTACK, CNVT_HBM_MID_GBL,  NULL,             NULL,               NULL,               NULL,          },  //BoxType21  HBM
  { NULL,             NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType22  SMB
  { CNVT_MSM,         CNVT_MSM,         CNVT_MSM,          NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType23  MSM
  { NULL,             NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType24  RLINK
  { CNVT_SB,          CNVT_SB,          CNVT_SB,           CNVT_SB,           CNVT_SB,          CNVT_SB,            CNVT_SB,            NULL,          },  //BoxType25  SB
  { NULL,             NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType26
  { NULL,             NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType27
  { NULL,             CNVT_SMEE,        NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType28  SMEE
  { NULL,             NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType29  FXR
  { CNVT_NPK,         CNVT_NPK_MEM,     NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType30  NPK
  { CNVT_IEH_SATELLITE,CNVT_IEH_GLOBAL, NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType31  IEH SATELLITE
  { CNVT_SPK,         NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType32  SPK MAIN
  { CNVT_DSA_IAX,     CNVT_DSA_IAX,     CNVT_CPM,          NULL,              CNVT_HQM,         NULL,               NULL,               NULL,          },  //BoxType33  DINO
  { CNVT_IIO_M2IAL,   NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType34  IIO M2IAL
  { CNVT_CXL_RCRBBAR, CNVT_CXL_DP_MBAR, CNVT_IIO_PCIE,     CNVT_CXL_DP_MBAR,  NULL,             NULL,               NULL,               NULL,          },  //BoxType35  PI5
  { CNVT_SB,          CNVT_SB,          NULL,              NULL,              NULL,             CNVT_SB,            CNVT_SB,            NULL,          },  //BoxType36  SB
  { NULL,             NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType37
  { CNVT_DRA_MAIN,    NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType38  DRA
  { CNVT_MCCADB,      NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType39  MCCADB
  { CNVT_MCCPGC,      NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType40  MCCPGC
  { CNVT_MDFIS,       NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType41  MDFIS
  { CNVT_I3C,         CNVT_I3C,         CNVT_SB,           CNVT_SB,           NULL,             NULL,               NULL,               NULL,          },  //BoxType42  SB I3C
  { NULL,             NULL,             NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType43  SB HSPHY
  { CNVT_SB,          CNVT_SB,          CNVT_SB,           NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType44  SB FBLP
  { CNVT_S3M,         CNVT_S3M,         NULL,              NULL,              NULL,             NULL,               NULL,               NULL,          },  //BoxType45  S3M
  { CNVT_HBM_MC,      CNVT_HBM_MC,      CNVT_HBM_MC_GLOBAL,CNVT_HBM_MC,       CNVT_HBM_MC,      CNVT_HBM_CPGC,      NULL,               NULL,          },  //BoxType46  HBM2E MC
};

/*
  This function identifies the different access type and then goes to different address translation routines.

  @param[in] Address              A pointer of the address of the USRA Address Structure
  @param[out] PhysicalAddress     A pointer of aligned address converted from USRA address

  @retval RETURN_SUCCESS          The function completed successfully.
  @retval RETURN_UNSUPPORTED      The function failed, access type for 'Address' is not supported.
*/
UINTN
EFIAPI
CsrGetPhysicalAddress (
  IN USRA_ADDRESS             *Address,
  OUT UINTN                   *PhysicalAddress
  )
{
  CPU_CSR_ACCESS_VAR          *CpuCsrAccessVarPtr;
  CSR_OFFSET                  *CsrOffset;

  UINT8                       SocId;
  UINT8                       InstId;
  UINT8                       InstType;

  CsrOffset = (CSR_OFFSET *)&(Address->Csr.Offset);
  SocId     = (UINT8) Address->Csr.SocketId;
  InstId    = (UINT8) Address->Csr.InstId;
  InstType  = (UINT8) Address->Csr.InstType;
  //
  // Convert the pseudo offset to the real offset if the pseudo bit is set.
  //
  if (CsrOffset->Bits.pseudo) {
    CsrOffset->Data = GetPseudoRegisterOffset (*CsrOffset);
  }
  //
  // Get the access width and place is in the addresses AccessWidth member.
  //
  Address->Attribute.AccessWidth = (UINT8) CsrOffset->Bits.size;
  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar();
  ASSERT (CpuCsrAccessVarPtr->DataPopulated == TRUE);
  if (CsrOffset->Bits.boxtype >= MAX_BOX_TYPES || CsrOffset->Bits.funcblk >= MAX_FUNCBLK ||
      mPhyAddrFuncTable[CsrOffset->Bits.boxtype][CsrOffset->Bits.funcblk] == NULL) {

    DEBUG ((DEBUG_ERROR, "[USRA] ERROR: Null PhyAddreFunc entry at [%d][%d]\n", CsrOffset->Bits.boxtype, CsrOffset->Bits.funcblk));
    ASSERT (FALSE);
    *PhysicalAddress = 0;
    return RETURN_UNSUPPORTED;
  }
  *PhysicalAddress = mPhyAddrFuncTable[CsrOffset->Bits.boxtype][CsrOffset->Bits.funcblk](SocId, InstId, *CsrOffset, CpuCsrAccessVarPtr, InstType);

  return RETURN_SUCCESS;
}
