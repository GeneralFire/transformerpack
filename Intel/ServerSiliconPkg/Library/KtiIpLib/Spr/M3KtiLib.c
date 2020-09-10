/** @file
  KTI *_M3KTI_MAIN_REG related registers access special for 10nm.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <Base.h>
#include <Guid/KtiIpInterface.h>
#include "Include/KtiRegs.h"
#include <Library/UsraCsrLib.h>
#include <Library/DebugLib.h>
#include <Library/UpimdfIpAccessLib.h>

#define M3ING_REM_CREDIT_VN0_VN1_CNT_ACTIVE    0x99999900   //Set credit value for VN0 & VN1 in M3INGDBGREMCRE1KTI0_M3KTI_MAIN_REG and M3INGDBGREMCRE0KTI0_M3KTI_MAIN_REG register.
#define M3ING_REM_CREDIT_VN0_VN1_CNT           0x11111100   //Clear active bit for VN0 & VN1 in M3INGDBGREMCRE1KTI0_M3KTI_MAIN_REG and M3INGDBGREMCRE0KTI0_M3KTI_MAIN_REG register.
#define M3ING_REM_CREDIT_VNA_ACTIVE_CLEAR_MASK 0x0000007F   //clear VNA active bit in M3INGDBGREMCRE1KTI0_M3KTI_MAIN_REG register.
#define M3ING_REM_CREDIT2_VN0_VN1_CNT_ACTIVE   0x99000000   //Set VN0&VN1 ad rsp count and active bit in M3INGDBGREMCRE2KTI0_M3KTI_MAIN_REG register.

/**
  Program M3KTI to M2IAL NCB and NCS Credits

  @param SocId             - Socket Id
  @param M3Kti             - M3Kti number
  @param NcbCredit         - Ncb Credit value
  @param NcsCredit         - Ncs Credit value

  @retval None
**/
VOID
EFIAPI
M3KtiConfigureM3KtiToM2IalCredit (
  IN UINT8                       SocId,
  IN UINT8                       M3Kti,
  IN UINT8                       NcbCredit,
  IN UINT8                       NcsCredit
  )
{
  M3KPCIR0CR_M3KTI_MAIN_STRUCT  M3KPciRing0CrdtCtrl;
  M3KPCIR1CR_M3KTI_MAIN_STRUCT  M3KPciRing1CrdtCtrl;
  M3KPCIR2CR_M3KTI_MAIN_STRUCT  M3KPciRing2CrdtCtrl;
  M3KPCIR3CR_M3KTI_MAIN_STRUCT  M3KPciRing3CrdtCtrl;
  //
  //1 port per M3KTI to program
  //
  M3KPciRing0CrdtCtrl.Data = UpimdfIpReadAccess32 (SocId, M3Kti, 0, M3KPCIR0CR_M3KTI_MAIN_REG);
  M3KPciRing1CrdtCtrl.Data = UpimdfIpReadAccess32 (SocId, M3Kti, 0, M3KPCIR1CR_M3KTI_MAIN_REG);
  M3KPciRing2CrdtCtrl.Data = UpimdfIpReadAccess32 (SocId, M3Kti, 0, M3KPCIR2CR_M3KTI_MAIN_REG);
  M3KPciRing3CrdtCtrl.Data = UpimdfIpReadAccess32 (SocId, M3Kti, 0, M3KPCIR3CR_M3KTI_MAIN_REG);
  //
  // Program NCB credits for all IIO stacks
  //
  M3KPciRing0CrdtCtrl.Bits.iio0_ncb = NcbCredit;
  M3KPciRing0CrdtCtrl.Bits.iio1_ncb = NcbCredit;
  M3KPciRing0CrdtCtrl.Bits.iio2_ncb = NcbCredit;
  M3KPciRing0CrdtCtrl.Bits.iio3_ncb = NcbCredit;
  M3KPciRing0CrdtCtrl.Bits.iio4_ncb = NcbCredit;
  M3KPciRing0CrdtCtrl.Bits.iio5_ncb = NcbCredit;
  M3KPciRing0CrdtCtrl.Bits.iio6_ncb = NcbCredit;


  M3KPciRing2CrdtCtrl.Bits.iio7_ncb = NcbCredit;
  M3KPciRing2CrdtCtrl.Bits.iio8_ncb = NcbCredit;
  M3KPciRing2CrdtCtrl.Bits.iio9_ncb = NcbCredit;
  M3KPciRing2CrdtCtrl.Bits.iio10_ncb = NcbCredit;
  M3KPciRing2CrdtCtrl.Bits.iio11_ncb = NcbCredit;

  //
  // Program NCS credits for all IIO stacks
  //
  M3KPciRing1CrdtCtrl.Bits.iio0_ncs = NcsCredit;
  M3KPciRing1CrdtCtrl.Bits.iio1_ncs = NcsCredit;
  M3KPciRing1CrdtCtrl.Bits.iio2_ncs = NcsCredit;
  M3KPciRing1CrdtCtrl.Bits.iio3_ncs = NcsCredit;
  M3KPciRing1CrdtCtrl.Bits.iio4_ncs = NcsCredit;
  M3KPciRing1CrdtCtrl.Bits.iio5_ncs = NcsCredit;
  M3KPciRing1CrdtCtrl.Bits.iio6_ncs = NcsCredit;

  M3KPciRing3CrdtCtrl.Bits.iio7_ncs  = NcsCredit;
  M3KPciRing3CrdtCtrl.Bits.iio8_ncs  = NcsCredit;
  M3KPciRing3CrdtCtrl.Bits.iio9_ncs  = NcsCredit;
  M3KPciRing3CrdtCtrl.Bits.iio10_ncs = NcsCredit;
  M3KPciRing3CrdtCtrl.Bits.iio11_ncs = NcsCredit;

  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3KPCIR0CR_M3KTI_MAIN_REG, M3KPciRing0CrdtCtrl.Data); // M3KTI N Port 0 (KTI portN) Upi->M2Pcie ncb
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3KPCIR1CR_M3KTI_MAIN_REG, M3KPciRing1CrdtCtrl.Data); // M3KTI N Port 0 (KTI portN) Upi->M2Pcie ncs
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3KPCIR2CR_M3KTI_MAIN_REG, M3KPciRing2CrdtCtrl.Data); // M3KTI N Port 0 (KTI portN) Upi->M2Pcie ncb
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3KPCIR3CR_M3KTI_MAIN_REG, M3KPciRing3CrdtCtrl.Data); // M3KTI N Port 0 (KTI portN) Upi->M2Pcie ncs
}

/**
  Program M3KTI IIO NCB and NCS Credits

  @param SocId             - Socket Id
  @param M3Kti             - M3Kti number

  @retval None
**/
VOID
EFIAPI
ProgramM3KtiNcbNcsCredits (
  UINT8                    SocId,
  UINT8                    M3Kti
  )
{
  M3KPCIR0CR_M3KTI_MAIN_STRUCT  M3KPciRing0CrdtCtrl;
  M3KPCIR1CR_M3KTI_MAIN_STRUCT  M3KPciRing1CrdtCtrl;

  //
  //1 port per M3KTI to program
  //
  M3KPciRing0CrdtCtrl.Data = UpimdfIpReadAccess32 (SocId, M3Kti, 0, M3KPCIR0CR_M3KTI_MAIN_REG);
  M3KPciRing1CrdtCtrl.Data = UpimdfIpReadAccess32 (SocId, M3Kti, 0, M3KPCIR1CR_M3KTI_MAIN_REG);
  //
  // Program NCB credits for all IIO and UBOX stacks
  //
  M3KPciRing0CrdtCtrl.Bits.iio0_ncb = 1;
  M3KPciRing0CrdtCtrl.Bits.iio1_ncb = 1;
  M3KPciRing0CrdtCtrl.Bits.iio2_ncb = 1;
  M3KPciRing0CrdtCtrl.Bits.iio3_ncb = 1;
  M3KPciRing0CrdtCtrl.Bits.iio4_ncb = 1;
  M3KPciRing0CrdtCtrl.Bits.iio5_ncb = 1;
  M3KPciRing0CrdtCtrl.Bits.ubox_ncb = 1;
  //
  // Program NCS credits for all IIO and UBOX stacks
  //
  M3KPciRing1CrdtCtrl.Bits.iio0_ncs = 1;
  M3KPciRing1CrdtCtrl.Bits.iio1_ncs = 1;
  M3KPciRing1CrdtCtrl.Bits.iio2_ncs = 1;
  M3KPciRing1CrdtCtrl.Bits.iio3_ncs = 1;
  M3KPciRing1CrdtCtrl.Bits.iio4_ncs = 1;
  M3KPciRing1CrdtCtrl.Bits.iio5_ncs = 1;
  M3KPciRing1CrdtCtrl.Bits.ubox_ncs = 1;

  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3KPCIR0CR_M3KTI_MAIN_REG, M3KPciRing0CrdtCtrl.Data);     // M3KTI N Port 0 (KTI portN) Upi->M2Pcie ncb
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3KPCIR1CR_M3KTI_MAIN_REG, M3KPciRing1CrdtCtrl.Data);     // M3KTI N Port 0 (KTI portN) Upi->M2Pcie ncs
}

/**
  Program  M3INGDBGREMCRE_0/1/2_KTI0_M3KTI_MAIN_REG, the value need to match with LCCL setting on peer port of peer socket
  If VnaCredit1 is not NULL, it means the Ad Vna credit.
  If VnaCredit2 is not NULL, it means the Ak Vna credit

  @param SocId             - Socket ID
  @param M3Kti             - M3Kti number
  @param VnaCredit0        - Vna Credit data 0, BL VNA Credit
  @param VnaCredit1        - Vna Credit data 1, AD VNA Credit, OPTIONAL
  @param VnaCredit2        - Vna Credit data 2, AK VNA Credit, OPTIONAL

  @retval KTI_SUCCESS - on successful completion

**/
VOID
EFIAPI
ProgramLinkLayerOnM3Kti (
  UINT8                    SocId,
  UINT8                    M3Kti,
  UINT32                   *VnaCredit0,
  UINT32                   *VnaCredit1  OPTIONAL,
  UINT32                   *VnaCredit2  OPTIONAL
  )
{
  //
  // SPR UCC has the same register bit definitions, but the bit fields' name are not same as other steppings and skus.
  // 1508192893 --- to trace the follow up, crif will be changed same as other steppings on M3INGDBGREMCRE0KTI0 and M3INGDBGREMCRE1KTI0
  //                M3INGDBGREMCRE2KTI0 -- needs to get the related credit number for ad&ak vna programming
  //
  M3INGDBGREMCRE0KTI0_M3KTI_MAIN_SPRA0_SPRB0_SPRHBM_SPRC0_SPRMCC_STRUCT RemCre0Kti0M3KtiReg;
  M3INGDBGREMCRE1KTI0_M3KTI_MAIN_SPRA0_SPRB0_SPRHBM_SPRC0_SPRMCC_STRUCT RemCre1Kti0M3KtiReg;
  M3INGDBGREMCRE2KTI0_M3KTI_MAIN_STRUCT                                 RemCre2Kti0M3KtiReg;

  if (VnaCredit0 == NULL) {
    return;
  }
  RemCre2Kti0M3KtiReg.Data = 0;

  RemCre0Kti0M3KtiReg.Data = M3ING_REM_CREDIT_VN0_VN1_CNT_ACTIVE | (*VnaCredit0);
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3INGDBGREMCRE0KTI0_M3KTI_MAIN_REG, RemCre0Kti0M3KtiReg.Data);  //program KTI0 VN0 & VNA value and set Load bit

  RemCre1Kti0M3KtiReg.Data = UpimdfIpReadAccess32 (SocId, M3Kti, 0, M3INGDBGREMCRE1KTI0_M3KTI_MAIN_REG);
  RemCre1Kti0M3KtiReg.Data &= 0x000000FF;   //Keep bit0:bit7,clear other bit.
  RemCre1Kti0M3KtiReg.Data = RemCre1Kti0M3KtiReg.Data | M3ING_REM_CREDIT_VN0_VN1_CNT_ACTIVE;
  RemCre1Kti0M3KtiReg.Bits.kti0loadremotevnacreditreference = 1;
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3INGDBGREMCRE1KTI0_M3KTI_MAIN_REG, RemCre1Kti0M3KtiReg.Data);  //program KTI0 VN1 value & VNA active value and set Load bit

  if ((VnaCredit1 != NULL) && (VnaCredit2 != NULL)) {
    RemCre2Kti0M3KtiReg.Data = UpimdfIpReadAccess32 (SocId, M3Kti, 0, M3INGDBGREMCRE2KTI0_M3KTI_MAIN_REG);
    RemCre2Kti0M3KtiReg.Bits.kti0remoteadvnacreditreferencecount = (*VnaCredit1);
    RemCre2Kti0M3KtiReg.Bits.kti0loadremoteadvnacreditreference = 1;
    RemCre2Kti0M3KtiReg.Bits.kti0remoteakvnacreditreferencecount = (*VnaCredit2);
    RemCre2Kti0M3KtiReg.Bits.kti0loadremoteakvnacreditreference = 1;
    RemCre2Kti0M3KtiReg.Data |= M3ING_REM_CREDIT2_VN0_VN1_CNT_ACTIVE;
    UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3INGDBGREMCRE2KTI0_M3KTI_MAIN_REG, RemCre2Kti0M3KtiReg.Data);
  }

  RemCre1Kti0M3KtiReg.Data &= M3ING_REM_CREDIT_VNA_ACTIVE_CLEAR_MASK;
  RemCre1Kti0M3KtiReg.Data |= M3ING_REM_CREDIT_VN0_VN1_CNT;
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3INGDBGREMCRE1KTI0_M3KTI_MAIN_REG, RemCre1Kti0M3KtiReg.Data);  //clear Load bit

  RemCre0Kti0M3KtiReg.Data = M3ING_REM_CREDIT_VN0_VN1_CNT | (*VnaCredit0);
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3INGDBGREMCRE0KTI0_M3KTI_MAIN_REG, RemCre0Kti0M3KtiReg.Data);  //clear Load bit

  if (RemCre2Kti0M3KtiReg.Data != 0) {
    RemCre2Kti0M3KtiReg.Bits.kti0loadremoteadvnacreditreference = 0;
    RemCre2Kti0M3KtiReg.Bits.kti0loadremoteakvnacreditreference = 0;
    RemCre2Kti0M3KtiReg.Bits.kti0loadremotevn1adrspcreditreference = 0;
    RemCre2Kti0M3KtiReg.Bits.kti0loadremotevn0adrspcreditreference = 0;
    UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3INGDBGREMCRE2KTI0_M3KTI_MAIN_REG, RemCre2Kti0M3KtiReg.Data);
  }

}

/**
  @param[in] SocId        - Socket to program
  @param[in] InPort       - input KTI port for route through
  @param[in] OutPort      - Output KTI port for route through
  @param[in] DestSocId    - endpoint processor socket ID
  @param[in,out]Data32    - current m3kti routing table value
  @param[in] ShareUpiP0P1 - UPI0 and UPI1 shared one Instance

**/
VOID
EFIAPI
ConvertM3KTIRoutingTable (
  IN     UINT8                  SocId,
  IN     UINT8                  InPort,
  IN     UINT8                  OutPort,
  IN     UINT8                  DestSocId,
  IN OUT UINT32                 *Data32,
  IN     BOOLEAN                ShareUpiP0P1
  )
{
  UINT32       StartBit;

  if (ShareUpiP0P1) {
    StartBit = 16 * (InPort & 0x1) + (2 * DestSocId);   // even port uses bit 0-15, odd port uses bit 16-31
  } else {
    StartBit = (2 * DestSocId);   // port uses bit 0-15
  }
  *Data32 &= (~(3 << StartBit));
  *Data32 |= (OutPort << StartBit);   // routing through for dest socket
}

/**
  Program the pass-through route in M3KTI routing table.

  @param[in] InPort       - input KTI port for route through
  @param[in] InterSocket  - intermediate socket ID
  @param[in] DestSocId    - endpoint processor socket ID
  @param[in] OutPort      - Output KTI port for route through
  @param[in] ShareUpiP0P1 - TRUE to indicate UPI Port 0 and Port 1 shared one mesh stop,
                            FALSE otherwise.

**/
VOID
EFIAPI
ProgramM3KtiRoute (
  IN UINT8                     InPort,
  IN UINT8                     InterSocket,
  IN UINT8                     DestSocId,
  IN UINT8                     OutPort,
  IN BOOLEAN                   ShareUpiP0P1
  )
{
  UINT32 Data32;

  Data32 = UpimdfIpReadAccess32 (InterSocket, InPort, 0, M3KKRT_M3KTI_MAIN_REG);
  ConvertM3KTIRoutingTable (InterSocket, InPort, OutPort, DestSocId, &Data32, ShareUpiP0P1);
  UpimdfIpWriteAccess32 (InterSocket, InPort, UPIMDF_INSTANCE_BROADCAST, M3KKRT_M3KTI_MAIN_REG, Data32);
}

/**
  Configure M3KTI mesh stop. This routine programs the route table, virtual
  network, direct to core enabling, pe_mode and other miscellaneous setting on
  M3KTI.

  @param[in] SocId                 - Socket Id
  @param[in] M3Kti                 - M3KTI mesh stop ID
  @param[in] M3KtiRt               - M3KTI Route table setting
  @param[in] M3KtiRt1              - M3KTI Route table setting for Port 1 on this mesh stop; this field
                                     is only applicable to CPX
  @param[in] IgrVnSel              - Virtual network selection setting
  @param[in] D2cEn                 - Direc-to-core enabling. True/False
  @param[in] ProgramPeMode         - PE bit setting. True/False
  @param[in] DisableMeshCreditPush - Set to TRUE to disable mesh credit push on M3KTI tile; FALSE otherwise.

  @retval None
**/
VOID
EFIAPI
ProgramM3KtiMeshStop (
  IN UINT8                    SocId,
  IN UINT8                    M3Kti,
  IN UINT32                   M3KtiRt,
  IN UINT32                   M3KtiRt1,
  IN UINT32                   IgrVnSel,
  IN BOOLEAN                  D2cEn,
  IN BOOLEAN                  ProgramPeMode,
  IN BOOLEAN                  DisableMeshCreditPush
  )
{
  M3KCTRL_M3KTI_MAIN_STRUCT    M3KCtrl;
  M3EFQCTRL_M3KTI_MAIN_STRUCT  M3efqctrl;

  //
  // Per HSD-ES 2205994811 request, need to replicate lower 8 bits in upper 8 bits for M3KKRT.Port0RouteSkt,
  // when PE mode is enable.
  //
  if ( ProgramPeMode == TRUE) {
    M3KtiRt |= (M3KtiRt << 8);
  }

  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3KKRT_M3KTI_MAIN_REG, M3KtiRt); //Route Table
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3KIGRVNSEL0_M3KTI_MAIN_REG, IgrVnSel);

  M3KCtrl.Data = UpimdfIpReadAccess32 (SocId, M3Kti, 0, M3KCTRL_M3KTI_MAIN_REG);
  M3KCtrl.Bits.socketid = SocId;   // SocketId

  if (D2cEn) {
    M3KCtrl.Bits.d2cthreshold = D2C_THLD_DFLT_SETTING;
  } else {
    M3KCtrl.Bits.d2cthreshold = 0;
  }
  M3KCtrl.Bits.meshcreditpush = (DisableMeshCreditPush) ? 0 : 1;
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3KCTRL_M3KTI_MAIN_REG, M3KCtrl.Data);

  //
  // Program pe_mode based on isocen
  //
  M3efqctrl.Data = UpimdfIpReadAccess32 (SocId, M3Kti, 0, M3EFQCTRL_M3KTI_MAIN_REG);
  //
  // clear pe_mode bits first
  //
  M3efqctrl.Bits.pe_mode = 0;
  if (ProgramPeMode) {
    M3efqctrl.Bits.pe_mode = 1;
  }

  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3EFQCTRL_M3KTI_MAIN_REG, M3efqctrl.Data);
}

/**
  Routine to program M3KTI Snoop Fanout

  @param[in] SocId             Socket to program
  @param[in] M3Kti             M3Kti number
  @param[in] M3KtiSnpFanOut    Pointer to an array of Snoop Fanout route setting (Element 0 is
                               for Port 0, element 1 is for Port 1. Port 1 is only applicable to
                               SOC that has shared KTI agent.
  @param[in] M3KtiSnpFanOut1   Pointer to another array of Snoop Fanout route setting for SOC.
                               Currently this parameter is only used by CPX for destination node
                               IDs 4 to 7.
  @param ProgramPeMode         PE bit setting. True/False

  @retval None
**/
VOID
ProgramM3KtiSnoopFanout (
  IN UINT8                    SocId,
  IN UINT8                    M3Kti,
  IN UINT32                   *M3KtiSnpFanOut,
  IN UINT32                   *M3KtiSnpFanOut1,
  IN BOOLEAN                  ProgramPeMode
  )
{

  //
  // Per HSD-ES 2205994811 request, need to replicate lower 12 bits in upper 1 bits for M3SNPFANOUT_P0.FanoutTable,
  // when PE mode is enable.
  //
  if ( ProgramPeMode == TRUE) {
    M3KtiSnpFanOut[0] |= (M3KtiSnpFanOut[0] << 12);
  }

  //
  // Program M3KTI Snoop Fanout
  //
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3SNPFANOUT_P0_M3KTI_MAIN_REG, M3KtiSnpFanOut[0]);
}

/**
  Program M3KTI RPQ VNA Credits

  @param SocId             - Socket Id
  @param M3Kti             - M3Kti number
  @param RpqVna            - rpq_vna credits to program in M3KEGRCTRL registers

  @retval None
**/
VOID
EFIAPI
ProgramM3KtiRpqVnaCredits (
  UINT8                    SocId,
  UINT8                    M3Kti,
  UINT32                   RpqVna
  )
{
  M3KEGRCTRL0_M3KTI_MAIN_STRUCT M3KErgCtrl;
  //
  // M3KEGRCTRL0/M3KEGRCTRL1 PRQ_VNA (UPI->CHA)
  //
  M3KErgCtrl.Data = UpimdfIpReadAccess32 (SocId, M3Kti, 0, M3KEGRCTRL0_M3KTI_MAIN_REG);
  M3KErgCtrl.Bits.rpq_vna = RpqVna;
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3KEGRCTRL0_M3KTI_MAIN_REG, M3KErgCtrl.Data);//UPI->CHA
}

/**
  Program M3KTI VNA/VN0 Credits

  @param SocId               - Socket Id
  @param M3Kti               - M3Kti number
  @param M2upiRouteThruAdVna - AD Vna credits to program in M3KRTE registers
  @param M2upiRouteThruAdVn0 - AD Vn0 credits to program in M3KRTE registers
  @param M2upiRouteThruBlVna - BL Vna credits to program in M3KRTE registers
  @param M2upiRouteThruBlVn0 - BL Vn0 credits to program in M3KRTE registers

  @retval None
**/
VOID
EFIAPI
ProgramM3KtiRouteThruCredits (
  IN UINT8                    SocId,
  IN UINT8                    M3Kti,
  IN UINT32                   M2upiRouteThruAdVna,
  IN UINT32                   M2upiRouteThruAdVn0,
  IN UINT32                   M2upiRouteThruBlVna,
  IN UINT32                   M2upiRouteThruBlVn0
  )
{
  M3KRTE0CR_M3KTI_MAIN_STRUCT   M3KRtEgressCrdtCtrlTemp;
  UINT32                        AdVn0ForEachField;
  UINT32                        BlVn0ForEachField;

  AdVn0ForEachField = M2upiRouteThruAdVn0/3;
  BlVn0ForEachField = M2upiRouteThruBlVn0/4;

  M3KRtEgressCrdtCtrlTemp.Data               = UpimdfIpReadAccess32 (SocId, M3Kti, 0, M3KRTE0CR_M3KTI_MAIN_REG);
  M3KRtEgressCrdtCtrlTemp.Bits.vna_rt_ad     = M2upiRouteThruAdVna;
  M3KRtEgressCrdtCtrlTemp.Bits.vn0_rt_snp    = AdVn0ForEachField;
  M3KRtEgressCrdtCtrlTemp.Bits.vn0_rt_req    = AdVn0ForEachField;
  M3KRtEgressCrdtCtrlTemp.Bits.vn0_rt_rsp_ad = AdVn0ForEachField;

  M3KRtEgressCrdtCtrlTemp.Bits.vna_rt_bl     = M2upiRouteThruBlVna;
  M3KRtEgressCrdtCtrlTemp.Bits.vn0_rt_rsp_bl = BlVn0ForEachField;
  M3KRtEgressCrdtCtrlTemp.Bits.vn0_rt_ncs    = BlVn0ForEachField;
  M3KRtEgressCrdtCtrlTemp.Bits.vn0_rt_ncb    = BlVn0ForEachField;
  M3KRtEgressCrdtCtrlTemp.Bits.vn0_rt_wb     = BlVn0ForEachField;
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3KRTE0CR_M3KTI_MAIN_REG, M3KRtEgressCrdtCtrlTemp.Data);
}

/**
  ICX R0 WA: enable M3KINGPRIVC1.AllowRateMatchingStallWhenUclkSlower

  @param SocId             - Socket ID
  @param M3Kti             - M3Kti number

**/
VOID
EFIAPI
ProgramExtraStallOnM3Kti (
  UINT8                    SocId,
  UINT8                    M3Kti
  )
{
 return;
}

/**
  ICXSP R0 WA: set M3KINGPRIVC1.DisableAdBypassBusy1

  @param SocId             - Socket ID
  @param M3Kti             - M3Kti number

**/
VOID
EFIAPI
ProgramDisableAdByPassOnM3Kti (
  UINT8                    SocId,
  UINT8                    M3Kti
  )
{
  //
  //Empty function to keep API consistency
  //
  return ;
}

/**
  Mask the remotevnacreditoverflow
  SKX WA s5372538

  @param SocId             - Socket ID
  @param M3Kti             - M3KTI number

  @retval None
**/
VOID
EFIAPI
MaskRemoteVnaCreditOverflow (
  UINT8                      SocId,
  UINT8                      M3Kti
  )
{
  //
  // No special action on 10nm code base, this function only be called on 14nm.
  //
  return;
}

/**
  Routine to setup M3KTI misc
  Called at end of SetupKtiMisc()


  SKX WA HSD 4929504: KTI tile performance critical CSRs for BIOS programming

  @param SocId             - Socket to program
  @param M3Kti             - M3Kti number

  @retval None
**/
VOID
EFIAPI
SetupM3KtiMisc (
  IN UINT8                    Soc,
  UINT8                       M3Kti
  )
{
  M3KINGCTL_M3KTI_MAIN_SPRA0_STRUCT  M3kIngCtl;

  //
  // M3KTI
  // 4929504: KTI tile performance critical CSRs for BIOS programming
  //
  M3kIngCtl.Data = UpimdfIpReadAccess32 (Soc, M3Kti, 0, M3KINGCTL_M3KTI_MAIN_REG);
  M3kIngCtl.Bits.validtxcyclemode = 2;
  M3kIngCtl.Bits.flitsinbgfifothresholdunnnh = 6;
  UpimdfIpWriteAccess32 (Soc, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3KINGCTL_M3KTI_MAIN_REG, M3kIngCtl.Data);
}

/**
  Routine to collect and clear M3KTI errors

  @param SocId             - Socket to program
  @param M3Kti             - M3KTI number

  @retval None

**/
VOID
EFIAPI
ClearM3KtiErrors (
  UINT8                      SocId,
  UINT8                      M3Kti,
  OUT KTI_CPU_M3KTI_ERRLOG   *M3KtiErrLog
  )
{
  M3INGERRMASK0_M3KTI_MAIN_SPRA0_SPRB0_SPRHBM_SPRC0_SPRMCC_STRUCT M3IngerrMask0;

  M3KtiErrLog->M3Egrerrlog0 = UpimdfIpReadAccess32 (SocId, M3Kti, 0, M3EGRERRLOG0_M3KTI_MAIN_REG); // RW1C
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3EGRERRLOG0_M3KTI_MAIN_REG, M3KtiErrLog->M3Egrerrlog0);

  M3KtiErrLog->M3Egrerrlog1 = UpimdfIpReadAccess32 (SocId, M3Kti, 0, M3EGRERRLOG1_M3KTI_MAIN_REG); // RW1C
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3EGRERRLOG1_M3KTI_MAIN_REG, M3KtiErrLog->M3Egrerrlog1);

  M3KtiErrLog->M3Ingerrlog0 = UpimdfIpReadAccess32 (SocId, M3Kti, 0, M3INGERRLOG0_M3KTI_MAIN_REG); // RW1C
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3INGERRLOG0_M3KTI_MAIN_REG, M3KtiErrLog->M3Ingerrlog0);

  M3KtiErrLog->M3Ingerrlog1 = UpimdfIpReadAccess32 (SocId, M3Kti, 0, M3INGERRLOG1_M3KTI_MAIN_REG); // RW1C
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3INGERRLOG1_M3KTI_MAIN_REG, M3KtiErrLog->M3Ingerrlog1);

  M3KtiErrLog->M3Ingerrlog1 = UpimdfIpReadAccess32 (SocId, M3Kti, 0, M3INGERRLOG2_M3KTI_MAIN_REG); // RW1C
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3INGERRLOG2_M3KTI_MAIN_REG, M3KtiErrLog->M3Ingerrlog1); //NOTE: re-use of M3INGERRLOG1

  //
  // No need to clear remotevnacreditoverflowmask on SPR, as its default value is the desired value
  // Skip the M3INGERRMASK1 programming (in UCC, it contrains different bitfields which do not need to be programmed now;
  // in other steppings of SPR, this register is for PORT1 in M3KTI, no need to program.)
  //
  M3IngerrMask0.Data = UpimdfIpReadAccess32 (SocId, M3Kti, 0, M3INGERRMASK0_M3KTI_MAIN_REG);
  M3IngerrMask0.Bits.blpumperrorfirstnot0mask = 1;
  M3IngerrMask0.Bits.blpumperrorsecondnot1mask = 1;
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3INGERRMASK0_M3KTI_MAIN_REG, M3IngerrMask0.Data);
}