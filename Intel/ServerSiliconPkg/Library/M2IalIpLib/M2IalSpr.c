/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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
#include <RcRegs.h>
#include <Library/DebugLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/M2IalIpLib.h>
#include <KtiSetupDefinitions.h>
#include <Memory/MemDefaults.h>

  UINT32 R2PrqInitCredRegTable[] = { R2PRQINITCRED0_IIO_M2IAL_REG,
                                     R2PRQINITCRED1_IIO_M2IAL_REG,
                                     R2PRQINITCRED2_IIO_M2IAL_REG,
                                     R2PRQINITCRED3_IIO_M2IAL_REG,
                                     R2PRQINITCRED4_IIO_M2IAL_REG,
                                     R2PRQINITCRED5_IIO_M2IAL_REG,
                                     R2PRQINITCRED6_IIO_M2IAL_REG,
                                     R2PRQINITCRED7_IIO_M2IAL_REG
                                   };

/**
  M2IAL:  Check if device exists

  @param SocId      - Socket ID
  @param CtrId      - Controller ID

  @retval                - TRUE if device exist and FALSE otherwise

**/

BOOLEAN
EFIAPI
M2IalDeviceExists (
  IN UINT8 SocId,
  IN UINT8 CtrId
  )
{
  BOOLEAN DevExists;

  UINT16 Data;

  Data = (UINT16)UsraCsrRead (SocId, CtrId, VID_IIO_M2IAL_REG);
  DevExists = ((Data == 0xFFFF)? FALSE : TRUE);

  return DevExists;
}

/**
  M2IAL:  Unmask Ingress Errors

  @param SocId      - Socket ID
  @param CtrId      - Controller ID

  @retval           - NONE

**/

VOID
EFIAPI
M2IalUnmaskR2PinError (
  IN UINT8 SocId,
  IN UINT8 CtrId
  )
{
  UsraCsrWrite (SocId, CtrId, R2PINGERRMSK0_IIO_M2IAL_REG, 0);
}

/**
  M2IAL:  Set UPI agent credits

  @param SocId      - Socket ID
  @param CtrId      - Controller ID
  @param Vn0Cr      - VN0 Credits

  @retval           - NONE

**/

VOID
EFIAPI
M2IalSetUpiCrd (
  IN UINT8  SocId,
  IN UINT8  CtrId,
  IN UINT32 Vn0Cr
  )
{
  UsraCsrWrite (SocId, CtrId, R2NDRTOKTIINITCRED_IIO_M2IAL_REG, Vn0Cr);
}

/**
  M2IAL:  Read and Write back the egress error log.

  @param SocId      - Socket ID
  @param CtrId      - Controller ID

  @retval           - R2 Egress error log

**/

UINT32
EFIAPI
M2IalR2WriteBackEgressErrLog (
  IN UINT8 SocId,
  IN UINT8 CtrId
  )
{
  UINT32 Data32;
  Data32 = UsraCsrRead (SocId, CtrId, R2EGRERRLOG_IIO_M2IAL_REG);
  UsraCsrWrite (SocId, CtrId, R2EGRERRLOG_IIO_M2IAL_REG, Data32);
  return Data32;
}

/**
  M2IAL:  Read and Write back the ingress error log.

  @param SocId      - Socket ID
  @param CtrId      - Controller ID

  @retval           - R2 Ingress error log

**/

UINT32
EFIAPI
M2IalR2WriteBackIngressErrLog (
  IN UINT8 SocId,
  IN UINT8 CtrId
  )
{
  UINT32 Data32;
  Data32 = UsraCsrRead (SocId, CtrId, R2PINGERRLOG0_IIO_M2IAL_REG);// RW1CS
  UsraCsrWrite (SocId, CtrId, R2PINGERRLOG0_IIO_M2IAL_REG, Data32);
  return Data32;
}

/**
  M2IAL:  Mask parity error in the ingress error mask.

  @param SocId      - Socket ID
  @param CtrId      - Controller ID

  @retval           - NONE

**/

VOID
EFIAPI
M2IalR2MaskParIngressErr (
  IN UINT8 SocId,
  IN UINT8 CtrId
  )
{
  R2PINGERRMSK0_IIO_M2IAL_STRUCT r2pIngErrReg;
  //
  // Si W/A 310710: Mask parity error on rsingrblpm with CSR bit
  //
  r2pIngErrReg.Data = UsraCsrRead (SocId, CtrId, R2PINGERRMSK0_IIO_M2IAL_REG);
  r2pIngErrReg.Bits.parerring0msk = 1;
  UsraCsrWrite (SocId, CtrId, R2PINGERRMSK0_IIO_M2IAL_REG, r2pIngErrReg.Data);
}

/**
  M2IAL:  Set the PRQ Credit Error Mask for each CHA

  @param SocId     - Socket ID
  @param CtrId     - Controller ID
  @param Mask      - mask when true, un-mask when false

  @retval          - NONE

**/

VOID
EFIAPI
M2IalSetR2CrdErrMask (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN BOOLEAN Mask
  )
{
  if (Mask) {
    UsraCsrWrite (SocId, CtrId, R2EGRPRQERRMSK0_IIO_M2IAL_REG, 0xFFFFFFFF);
  } else {
    UsraCsrWrite (SocId, CtrId, R2EGRPRQERRMSK0_IIO_M2IAL_REG, 0);
  }
}

/**
  M2IAL:  Program m2ial route table

  @param SocId       - Socket ID
  @param CtrId       - Controller ID
  @param KtiRtMode   - Kti Routing mode
  @param M2PcieKtiRt - Route Table

  @retval           - NONE

**/

VOID
EFIAPI
M2IalSetRouteTable (
  IN UINT8  SocId,
  IN UINT8  CtrId,
  IN UINT8  KtiRtMode,
  IN UINT32 M2PcieKtiRt
  )
{
  UPI_RT_IIO_M2IAL_STRUCT    UpiRtIio;

  UpiRtIio.Data = UsraCsrRead (SocId, CtrId, UPI_RT_IIO_M2IAL_REG);

  UpiRtIio.Bits.upiroutingtable = M2PcieKtiRt;
  UpiRtIio.Bits.upiinterleavemode = KtiRtMode;

  UsraCsrWrite (SocId, CtrId, UPI_RT_IIO_M2IAL_REG, UpiRtIio.Data);
}

/**
  M2IAL:  Clear DDRT Defeature.

  @param SocId       - Socket ID
  @param CtrId       - Controller ID
  @param DdrtQosMode - DDRT QoS Mode

  @retval           - NONE

**/

VOID
EFIAPI
M2IalClearDdrtDefeature (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 DdrtQosMode
  )
{
  R2LBWPRQCFG_IIO_M2IAL_STRUCT R2LbwPrgCfg;
  //
  // Clear MS2IOSF DDRT_Defeature[0:0] for DDRT
  //
  R2LbwPrgCfg.Data = UsraCsrRead (SocId, CtrId, R2LBWPRQCFG_IIO_M2IAL_REG);
  if (DdrtQosMode == DDRT_QOS_MODE_2) {
    R2LbwPrgCfg.Bits.ddrt_defeature = 0;
  } else {
    R2LbwPrgCfg.Bits.ddrt_defeature = 0x01;
  }
  UsraCsrWrite (SocId, CtrId, R2LBWPRQCFG_IIO_M2IAL_REG, R2LbwPrgCfg.Data);
}

/**
  M2IAL:  Set the NDR DRS routes

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param TempR2pGnCtrl - Temporary register value read previously.

  @retval           - NONE

**/

VOID
EFIAPI
M2IalSetRouteNdrDrs (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT32 TempR2pGnCtrl
  )
{
  //
  //Empty function for API consistency
  //
  return;
}

/**
  M2IAL:  Program m2pcie port into route table

  @param SocId           - Socket ID
  @param DSocId          - Socket ID destination
  @param LocalPort       - The local port to route
  @param PortFieldWidth  - The local port width

  @retval           - NONE

**/

VOID
EFIAPI
M2IalSetPortRouteTable (
  IN UINT8  SocId,
  IN UINT8  DSocId,
  IN UINT32 LocalPort,
  IN UINT32 PortFieldWidth
  )
{
  //
  //Empty function for API consistency
  //
  return;
}

/**
  M2IAL:  Set the PRQ initial credits  for each CHA

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param TotCha        - Total number of CHA
  @param PrqPerIoStack - PRQs per IO Stack

  @retval           - NONE

**/

VOID
EFIAPI
M2IalSetR2InitialCrd (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 TotCha,
  IN UINT8 PrqPerIoStack
  )
{
  UINT8  Cha;
  UINT32 Credit0 = 0;
  UINT32 Credit1 = 0;
  UINT32 Credit2 = 0;
  UINT32 Credit3 = 0;
  UINT32 Credit4 = 0;
  UINT32 Credit5 = 0;
  UINT32 Credit6 = 0;
  UINT32 Credit7 = 0;

  for (Cha = 0; Cha < TotCha; Cha++) {
    if (Cha < 8) {
      Credit0 = Credit0 | (PrqPerIoStack << (4 * (Cha % 8)));
    } else if (Cha < 16) {
      Credit1 = Credit1 | (PrqPerIoStack << (4 * (Cha % 8)));
    } else if (Cha < 24) {
      Credit2 = Credit2 | (PrqPerIoStack << (4 * (Cha % 8)));
    } else if (Cha < 32) {
      Credit3 = Credit3 | (PrqPerIoStack << (4 * (Cha % 8)));
    } else if (Cha < 40) {
      Credit4 = Credit4 | (PrqPerIoStack << (4 * (Cha % 8)));
    } else if (Cha < 48) {
      Credit5 = Credit5 | (PrqPerIoStack << (4 * (Cha % 8)));
    } else if (Cha < 56) {
      Credit6 = Credit6 | (PrqPerIoStack << (4 * (Cha % 8)));
    } else if (Cha < 64) {
      Credit7 = Credit7 | (PrqPerIoStack << (4 * (Cha % 8)));
    }
  }

  UsraCsrWrite (SocId, CtrId, R2PRQINITCRED0_IIO_M2IAL_REG, Credit0); //for CHA 0-7
  UsraCsrWrite (SocId, CtrId, R2PRQINITCRED1_IIO_M2IAL_REG, Credit1); //for CHA 8-15
  UsraCsrWrite (SocId, CtrId, R2PRQINITCRED2_IIO_M2IAL_REG, Credit2); //for CHA 16-23
  UsraCsrWrite (SocId, CtrId, R2PRQINITCRED3_IIO_M2IAL_REG, Credit3); //for CHA 24-31
  UsraCsrWrite (SocId, CtrId, R2PRQINITCRED4_IIO_M2IAL_REG, Credit4); //for CHA 32-39
  UsraCsrWrite (SocId, CtrId, R2PRQINITCRED5_IIO_M2IAL_REG, Credit5); //for CHA 40-47
  UsraCsrWrite (SocId, CtrId, R2PRQINITCRED6_IIO_M2IAL_REG, Credit6); //for CHA 48-55
  UsraCsrWrite (SocId, CtrId, R2PRQINITCRED7_IIO_M2IAL_REG, Credit7); //for CHA 56-63
}

/**
  M2IAL:  Get the Intial value of shared credits

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param Cred0         - Shared credit 0
  @param Cred1         - Shared credit 1
  @param Cred2         - Shared credit 2
  @param Cred3         - Shared credit 3

  @retval              - NONE

**/

VOID
EFIAPI
M2IalGetR2InitShrCrd (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  OUT UINT32 *Cred0,
  OUT UINT32 *Cred1,
  OUT UINT32 *Cred2,
  OUT UINT32 *Cred3
  )
{

  if (Cred0 == NULL || Cred1 == NULL || Cred2 == NULL || Cred3 == NULL) {
    DEBUG ((DEBUG_ERROR, "ERROR!! Null Pointer detected\n"));
    ASSERT (FALSE);
    return;
  }

  *Cred0 = UsraCsrRead (SocId, CtrId, R2P2PINITSHCRED0_IIO_M2IAL_REG);
  *Cred1 = UsraCsrRead (SocId, CtrId, R2P2PINITSHCRED1_IIO_M2IAL_REG);
  *Cred2 = UsraCsrRead (SocId, CtrId, R2P2PINITSHCRED2_IIO_M2IAL_REG);
  *Cred3 = UsraCsrRead (SocId, CtrId, R2P2PINITSHCRED3_IIO_M2IAL_REG);
}

/**
  M2IAL:  Set the Intial value of shared credits

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param Cred0         - Shared credit 0
  @param Cred1         - Shared credit 1
  @param Cred2         - Shared credit 2
  @param Cred3         - Shared credit 3

  @retval              - NONE

**/

VOID
EFIAPI
M2IalSetR2InitShrCrd (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT32 Cred0,
  IN UINT32 Cred1,
  IN UINT32 Cred2,
  IN UINT32 Cred3
  )
{
  UsraCsrWrite (SocId, CtrId, R2P2PINITSHCRED0_IIO_M2IAL_REG, Cred0);
  UsraCsrWrite (SocId, CtrId, R2P2PINITSHCRED1_IIO_M2IAL_REG, Cred1);
  UsraCsrWrite (SocId, CtrId, R2P2PINITSHCRED2_IIO_M2IAL_REG, Cred2);
  UsraCsrWrite (SocId, CtrId, R2P2PINITSHCRED3_IIO_M2IAL_REG, Cred3);
}

/**
  M2IAL:  Set the initial credits for iio2ubox

  @param SocId     - Socket ID
  @param CtrId     - Controller ID

  @retval          - NONE

**/

VOID
EFIAPI
M2IalSetIio2UboxInitCrd (
  IN UINT8 SocId,
  IN UINT8 CtrId
  )
{
  R2IIO2UBOXINITCRED_IIO_M2IAL_STRUCT R2Iii2UboxInitCred;
  //
  // Program MS2IOSF->UBOX credits
  //
  R2Iii2UboxInitCred.Data = UsraCsrRead (SocId, CtrId, R2IIO2UBOXINITCRED_IIO_M2IAL_REG);
  R2Iii2UboxInitCred.Bits.vn0ncbiio2ubxinitialcred = 1;
  UsraCsrWrite (SocId, CtrId, R2IIO2UBOXINITCRED_IIO_M2IAL_REG, R2Iii2UboxInitCred.Data); //M2IAL->UBOX
}

/**
  M2IAL:  Set the PRQ initial credits usiing and index

  @param SocId     - Socket ID
  @param CtrId     - Controller ID
  @param RegIdx    - Table index for the register
  @param Credit    - Credit value

  @retval          - NONE

**/

VOID
EFIAPI
M2IalSetPrqInitCrdIdx (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 RegIdx,
  IN UINT32 Credit
  )
{
  if (RegIdx >= (sizeof (R2PrqInitCredRegTable) / sizeof (R2PrqInitCredRegTable[0]))) {
    DEBUG ((DEBUG_ERROR, "ERROR!! Out of bounds access detected\n"));
    ASSERT (FALSE);
    return;
  }

  UsraCsrWrite (SocId, CtrId, R2PrqInitCredRegTable[RegIdx], Credit);
}

/**
  M2IAL:  Set the initial credits for NDR/NCS to KTI

  @param SocId    - Socket ID
  @param CtrId    - Controller ID
  @param Vn0Ad    - VN0 AD credit value
  @param BlNcs    - BL NCS credit value

  @retval          - NONE

**/

VOID
EFIAPI
M2IalSetNdrNcs2KtiInitCrd (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT32 Vn0Ad,
  IN UINT32 BlNcs
  )
{
  //
  //Empty function for API consistency
  //
  return;
}

/**
  M2IAL:  Set the initial credits for NBC/NCS to Iio

  @param SocId    - Socket ID
  @param CtrId    - Controller ID
  @param Credit   - Credit value

  @retval          - NONE

**/

VOID
EFIAPI
M2IalSetNcbNcs2IioInitCrd (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT32 Credit
  )
{
  //
  //Empty function for API consistency
  //
  return;
}

/**
  M2IAL:  Unmask Global Errors

  @param SocId      - Socket ID
  @param CtrId      - Controller ID

  @retval           - NONE

**/

VOID
EFIAPI
M2IalUnmaskGlError (
  IN UINT8 SocId,
  IN UINT8 CtrId
  )
{
  R2GLERRCFG_IIO_M2IAL_STRUCT    R2glerrcfgReg;

  R2glerrcfgReg.Data = UsraCsrRead (SocId, CtrId, R2GLERRCFG_IIO_M2IAL_REG);
  //
  // Unmask bit [0-17] in this register
  //
  R2glerrcfgReg.Bits.resetglobalfatalerror = 0;
  R2glerrcfgReg.Bits.resetglobalviral = 0 ;
  R2glerrcfgReg.Bits.maskexternviralinin = 0;
  R2glerrcfgReg.Bits.maskexternfatalerrorin = 0;
  R2glerrcfgReg.Bits.maskiiofatalerrorin = 0;
  R2glerrcfgReg.Bits.maskiioviralout = 0;
  R2glerrcfgReg.Bits.maskiioviralin = 0;
  R2glerrcfgReg.Bits.maskr2fatalerror = 0;

  UsraCsrWrite (SocId, CtrId, R2GLERRCFG_IIO_M2IAL_REG, R2glerrcfgReg.Data);
}

/**
  M2IAL:  Set the mesh credit update value

  @param SocId           - Socket ID
  @param CtrId           - Controller ID
  @param MeshCrdUpdate   - Mesh credit update value

  @retval EFI_STATUS     - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2IalSetMeshCrdUpdate (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 MeshCrdUpdate
  )
{
  R2PGNCTRL_SB_M2PCIE_STRUCT R2pGnCtrlStruct;
  R2pGnCtrlStruct.Data = UsraCsrRead (SocId, CtrId, R2PGNCTRL_SB_M2PCIE_REG);
  R2pGnCtrlStruct.Bits.meshcreditupdate = MeshCrdUpdate & 0x1;
  UsraCsrWrite (SocId, CtrId, R2PGNCTRL_SB_M2PCIE_REG, R2pGnCtrlStruct.Data);

  return EFI_SUCCESS;
}

/**
  M2IAL:  A routine for programming the QoS Performance settings.

  @param SocId        - Socket ID
  @param IioStack     - Stack Index
  @param DdrtQosMode  - DDRT QoS MODE

  @retval None

**/

VOID
EFIAPI
M2IalSetCrQosPerformance (
  IN UINT8 SocId,
  IN UINT8 IioStack,
  IN UINT8 DdrtQosMode
  )
{
  R22LMDDRTCFG_IIO_M2IAL_STRUCT             R22lmDdrtCfg;
  R22LMDDRTCOUNTERSCFG_IIO_M2IAL_STRUCT     R22lmDdrtCountersCfg;
  R2LBWPRQINITCRED0_IIO_M2IAL_STRUCT        R2LbwPrqInitCred0;
  //
  // Program the recommended values for PnP validation.
  //
  R22lmDdrtCountersCfg.Data = UsraCsrRead (SocId, IioStack, R22LMDDRTCOUNTERSCFG_IIO_M2IAL_REG);
  R22lmDdrtCountersCfg.Bits.r22lmtimewindow = 0x40;
  R22lmDdrtCountersCfg.Bits.r22lmddrtcountsatvalue = 0x10;
  UsraCsrWrite (SocId, IioStack, R22LMDDRTCOUNTERSCFG_IIO_M2IAL_REG, R22lmDdrtCountersCfg.Data);

  R22lmDdrtCfg.Data = UsraCsrRead (SocId, IioStack, R22LMDDRTCFG_IIO_M2IAL_REG);
  R22lmDdrtCfg.Bits.r22lmddrtth = 0x07;
  R22lmDdrtCfg.Bits.r22lmddrtloth = 0x00;
  R22lmDdrtCfg.Bits.r22lmddrtcleardistressabvth = 0x01;

  if (DdrtQosMode == DDRT_QOS_MODE_0) {
    R22lmDdrtCfg.Bits.r22lmthdisabled = 0x01;
  } else {
    R22lmDdrtCfg.Bits.r22lmthdisabled = 0;
  }
  UsraCsrWrite (SocId, IioStack, R22LMDDRTCFG_IIO_M2IAL_REG, R22lmDdrtCfg.Data);

  R2LbwPrqInitCred0.Data = UsraCsrRead (SocId, IioStack, R2LBWPRQINITCRED0_IIO_M2IAL_REG);
  R2LbwPrqInitCred0.Bits.lowbwcboprqinitialcred_07_00 = 0x11111111;
  UsraCsrWrite (SocId, IioStack, R2LBWPRQINITCRED0_IIO_M2IAL_REG, R2LbwPrqInitCred0.Data);

  return;
}

/**
  M2IAL:  Set the NCS/NCB/DRS defeature bit, to override credit.

  @param SocId           - Socket ID
  @param CtrId           - Controller ID
  @param Defeature       - NCS/NCB/DRS defeature bit value

  @retval EFI_STATUS     - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2IALSetNcsNcbDrsDefeature (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 Defeature
  )
{
  R2DEFEATURE_IIO_M2IAL_STRUCT R2Defeature;

  R2Defeature.Data = UsraCsrRead (SocId, CtrId, R2DEFEATURE_IIO_M2IAL_REG);
  R2Defeature.Bits.p2p_sharedcredring_ncs_def = 0x1 & Defeature;
  R2Defeature.Bits.p2p_sharedcredring_ncb_def = 0x1 & Defeature;
  R2Defeature.Bits.p2p_sharedcredring_drs_def = 0x1 & Defeature;
  UsraCsrWrite (SocId, CtrId, R2DEFEATURE_IIO_M2IAL_REG, R2Defeature.Data);

  return EFI_SUCCESS;
}