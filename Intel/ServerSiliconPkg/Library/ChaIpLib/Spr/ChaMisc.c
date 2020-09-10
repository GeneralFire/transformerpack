/** @file
  Configure CHA_MISC* registers and others for 10nm.

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

//
// Include files
//
#include "Include/ChaRegs.h"
#include <Library/UsraCsrLib.h>
#include <Library/ChaLib.h>
#include <Library/KtiApi.h>
#include <Upi/KtiSi.h>
#include <Library/MemoryCoreLib.h>
#include <Upi/KtiHost.h>
#include <Library/UpiHostDataLib.h>
#include <KtiSetupDefinitions.h>

//
// Legacy Decode
//
/**
  CHA: Program All PAM region access to DRAM

  @param SocId     - CPU Socket Node number (Socket ID)

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ProgramAllPamAccessToDram (
  IN UINT8           SocId
  )
{
  PAM0123_CHABC_SAD_STRUCT                Pam0123Reg;
  PAM456_CHABC_SAD_STRUCT                 Pam456Reg;

  Pam0123Reg.Data = UsraCsrRead (SocId, 0, PAM0123_CHABC_SAD_REG);
  Pam456Reg.Data = UsraCsrRead (SocId, 0, PAM456_CHABC_SAD_REG);

  Pam0123Reg.Bits.pam0_hienable = 3;
  Pam0123Reg.Bits.pam1_loenable = 3;
  Pam0123Reg.Bits.pam1_hienable = 3;
  Pam0123Reg.Bits.pam2_loenable = 3;
  Pam0123Reg.Bits.pam2_hienable = 3;
  Pam0123Reg.Bits.pam3_loenable = 3;
  Pam0123Reg.Bits.pam3_hienable = 3;

  Pam456Reg.Bits.pam4_loenable = 3;
  Pam456Reg.Bits.pam4_hienable = 3;
  Pam456Reg.Bits.pam5_loenable = 3;
  Pam456Reg.Bits.pam5_hienable = 3;
  Pam456Reg.Bits.pam6_loenable = 3;
  Pam456Reg.Bits.pam6_hienable = 3;

  UsraCsrWrite (SocId, 0, PAM0123_CHABC_SAD_REG, Pam0123Reg.Data);
  UsraCsrWrite (SocId, 0, PAM456_CHABC_SAD_REG, Pam456Reg.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Enable Modulo3 based KTI Link

  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaSadTargetEnableModulo3BasedKtiLink (
  UINT8    SocId,
  UINT8    BoxInst
  )
{
  return EFI_UNSUPPORTED;
}

/**
  CHA: Configure TOR Thresholds: Loc2rem_thresh_norm and Loc2rem_thresh_empty

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param Loc2rem_thresh_norm  - number of local to remote requests allowed in
                                pipeline when remote requests are also present
                                in pipeline
  @param Loc2rem_thresh_empty - number of local to remote requests allowed in
                                pipeline when pipeline is empty of remote
                                requests
  @param Tor_threshold        - remote requests threshold

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureTorThresholds (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Loc2rem_thresh_norm,
  UINT32   Loc2rem_thresh_empty,
  UINT32   Tor_threshold
  )
{
  TOR_THRESHOLDS_CHABC_SAD1_STRUCT           TorThresholds;

  TorThresholds.Data = UsraCsrRead(SocId, BoxInst, TOR_THRESHOLDS_CHABC_SAD1_REG);
  TorThresholds.Bits.loc2rem_thresh_norm = Loc2rem_thresh_norm;
  TorThresholds.Bits.loc2rem_thresh_empty = Loc2rem_thresh_empty;
  UsraCsrWrite(SocId, BoxInst, TOR_THRESHOLDS_CHABC_SAD1_REG, TorThresholds.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Configure DDRT QoS TOR Thresholds.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param DdrtQosMode          - DDRT QoS Mode

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureDDRTQoSTorThresholds (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT8    DdrtQosMode
  )
{
  LOWBWMEM_LOC_TOR_THRESHOLDS_CHABC_SAD1_STRUCT         LowBwMemLocTorThresholds;
  LOWBWMEM_LOCTOREM_TOR_THRESHOLDS_CHABC_SAD1_STRUCT    LowBwMemLocToRemTorThresholds;
  TOR_THRESHOLDS_CHABC_SAD1_STRUCT                      TorThresholdsChabcSad1;
  HA_COH_CFG_TOR_CHABC_SAD1_STRUCT                      HaCohCfgTor;

  LowBwMemLocTorThresholds.Data = UsraCsrRead (SocId, 0, LOWBWMEM_LOC_TOR_THRESHOLDS_CHABC_SAD1_REG);
  if (DdrtQosMode == DDRT_QOS_MODE_2) {
    LowBwMemLocTorThresholds.Bits.loc_enable = 0x01;
    LowBwMemLocTorThresholds.Bits.loc_rdwr_hi_th = 0x1F;
    LowBwMemLocTorThresholds.Bits.loc_wr_hi_th = 0x0A;
    LowBwMemLocTorThresholds.Bits.loc_wr_maxbw_th = 0x08;
    LowBwMemLocTorThresholds.Bits.loc_wr_vic_hi_th = 0x0A;
    LowBwMemLocTorThresholds.Bits.ddrt_tor_req_fifo_enable = 0x01;
  } else {
    LowBwMemLocTorThresholds.Bits.loc_enable = 0;
  }
  UsraCsrWrite (SocId, 0, LOWBWMEM_LOC_TOR_THRESHOLDS_CHABC_SAD1_REG, LowBwMemLocTorThresholds.Data);

  LowBwMemLocToRemTorThresholds.Data = UsraCsrRead (SocId, 0, LOWBWMEM_LOCTOREM_TOR_THRESHOLDS_CHABC_SAD1_REG);
  if (DdrtQosMode == DDRT_QOS_MODE_2) {
    LowBwMemLocToRemTorThresholds.Bits.loctorem_enable = 1;
    LowBwMemLocToRemTorThresholds.Bits.loctorem_hi_th = 0x08;
    LowBwMemLocToRemTorThresholds.Bits.loctorem_maxbw_th = 0x09;
    LowBwMemLocToRemTorThresholds.Bits.loctorem_rdwr_hi_th = 0x1F;
    LowBwMemLocToRemTorThresholds.Bits.loctorem_vic_hi_th = 0x08;
  } else {
    LowBwMemLocToRemTorThresholds.Bits.loctorem_enable = 0;
  }
  UsraCsrWrite (SocId, 0, LOWBWMEM_LOCTOREM_TOR_THRESHOLDS_CHABC_SAD1_REG, LowBwMemLocToRemTorThresholds.Data);

  if (DdrtQosMode == DDRT_QOS_MODE_2) {
    TorThresholdsChabcSad1.Data = UsraCsrRead (SocId, 0, TOR_THRESHOLDS_CHABC_SAD1_REG);
    TorThresholdsChabcSad1.Bits.loc2rem_thresh_empty = 0x04;
    UsraCsrWrite (SocId, 0, TOR_THRESHOLDS_CHABC_SAD1_REG, TorThresholdsChabcSad1.Data);

    HaCohCfgTor.Data = UsraCsrRead (SocId, 0, HA_COH_CFG_TOR_CHABC_SAD1_REG);
    HaCohCfgTor.Bits.haddrtcreditcnt = 0x04;
    UsraCsrWrite (SocId, 0, HA_COH_CFG_TOR_CHABC_SAD1_REG, HaCohCfgTor.Data);
  }

  return EFI_SUCCESS;
}

/**
  CHA: Enable KTI Broadcast Route Table

  @param SocId                - CPU Socket Node number (Socket ID)
  @param Enable               - TURE: enable the broadcast, FALSE: disable the broadcast

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaKtiBroadcastRouteTableEnable (
  UINT8    SocId,
  BOOLEAN  Enable
  )
{
  return EFI_UNSUPPORTED;
}

/**
  CHA: Configure KTI Fanout

  @param[in] SocId                CPU Socket Node number (Socket ID)
  @param[in] BoxInst              Box Instance, 0 based
  @param[in] ChaKtiFanOut         CHA KTI Fanout value to be written into CHA_RTR_SNP_CTL0 register
  @param[in] ChaKtiFanOut1        CHA KTI Fanout value to be written into CHA_RTR_SNP_CTL1 register.
                                  This field is only applicable to SOC that supports 6 UPI links.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureSnoopFanout (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   ChaKtiFanOut,
  IN UINT32   ChaKtiFanOut1
  )
{
  CHA_RTR_SNP_CTL1_CHABC_SAD_STRUCT ChaRtrSnpCtr1;
  UINT32 Data32;

  Data32 = UsraCsrRead (SocId, BoxInst, CHA_RTR_SNP_CTL0_CHABC_SAD_REG);
  Data32 = Data32 | ChaKtiFanOut;           // Set send2qpiports_nx
  UsraCsrWrite (SocId, BoxInst, CHA_RTR_SNP_CTL0_CHABC_SAD_REG, Data32);

  ChaRtrSnpCtr1.Data = UsraCsrRead (SocId, BoxInst, CHA_RTR_SNP_CTL1_CHABC_SAD_REG);
  ChaRtrSnpCtr1.Bits.enable_fanout = 0xff;
  UsraCsrWrite (SocId, BoxInst, CHA_RTR_SNP_CTL1_CHABC_SAD_REG, ChaRtrSnpCtr1.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Configure QPI XNC Mask

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param QpiXncmask           - QPI XNC Mask to be set

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureQpiXncmask (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   QpiXncmask
  )
{
  UsraCsrWrite (SocId, BoxInst, QPI_XNC_MASK_CHABC_SAD1_REG, QpiXncmask);

  return EFI_SUCCESS;
}

/**
  CHA: Configure HA - Disable Egress Isoc Credit Reservation

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param DisEgressIsocRes     - Disable Egress Isoc credit reservation

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaDisableEgressIsocRes (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT8    DisEgressIsocRes
  )
{
  HA_THRESHOLDS_CHABC_SAD1_STRUCT   HaThresholds;

  HaThresholds.Data = UsraCsrRead (SocId, BoxInst, HA_THRESHOLDS_CHABC_SAD1_REG);

  HaThresholds.Bits.disegressisocres = DisEgressIsocRes;

  UsraCsrWrite (SocId, BoxInst, HA_THRESHOLDS_CHABC_SAD1_REG, HaThresholds.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Configure HA Coherency TOR

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param HitMEEnable          - Enable HitME cache overloading. HitME$
                                should be enabled only when directory is
                                enabled.
  @param HitMERfoDirs         - Enable HitME DIR=S RFO optimization. This
                                optimization allows snoops to be broadcast in
                                parallel with memory lookup for RFO HitS in the
                                local CA.
  @param HitMEShar            - HitME cache allows caching of SHARed lines
  @param Is2SConfig           - 2S config flag

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureHaCohTor (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   HitMEEnable,
  UINT32   HitMERfoDirs,
  UINT32   HitMEShar,
  UINT8    Is2SConfig
  )
{
  HA_COH_CFG_TOR_CHABC_SAD1_STRUCT  HaCohCfgTor;

  HaCohCfgTor.Data = UsraCsrRead (SocId, BoxInst, HA_COH_CFG_TOR_CHABC_SAD1_REG);

  HaCohCfgTor.Bits.hitme_enable = HitMEEnable;
  HaCohCfgTor.Bits.hitme_rfo_dirs = HitMERfoDirs;
  HaCohCfgTor.Bits.hitme_shar = HitMEShar;

  if (Is2SConfig) {
    HaCohCfgTor.Bits.hitme_alloconlocalme = 0x1;
    HaCohCfgTor.Bits.hitme_ways           = 0x3;
  }

  UsraCsrWrite (SocId, BoxInst, HA_COH_CFG_TOR_CHABC_SAD1_REG, HaCohCfgTor.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Configure HA Coherency1

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param HaMiniBypassDis      - HA Mini Bypass mode. When a Read request gets
                                read out of the TORReqQ and there is not
                                anything ahead of it in the HApipe, the read
                                will be issued right away saving 2 cycles.
  @param StaleAtoSOptEn       - A to S directory optimization. When RdData*
                                finds DIR=A and all snoop responses received are
                                RspI, then directory is moved to S and data is
                                returned in S-state. This optimization will not
                                be effective in xNC configuration where BuriedM
                                is possible.
  @param GateOsbIodcAlloc     - When OSB indicates that there arent enough snoop
                                credits dont allocate IODC entry
  @param Is2SConfig           - 2S config flag

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureHaCoh1 (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   HaMiniBypassDis,
  UINT32   StaleAtoSOptEn,
  UINT32   GateOsbIodcAlloc,
  UINT8    Is2SConfig
  )
{
  HA_COH_CFG_1_CHABC_SAD1_STRUCT    HaCohCfg1;

  HaCohCfg1.Data = UsraCsrRead (SocId, BoxInst, HA_COH_CFG_1_CHABC_SAD1_REG);

  HaCohCfg1.Bits.ha_mini_bypass_dis = HaMiniBypassDis;
  HaCohCfg1.Bits.staleatosopten = StaleAtoSOptEn;
  HaCohCfg1.Bits.gateosbiodcalloc = GateOsbIodcAlloc;

  if (Is2SConfig) {
    HaCohCfg1.Bits.iodc_alloc_osb_bl_vna_thr  = 0x1;
    HaCohCfg1.Bits.iodc_alloc_osb_snp_thr     = 0x1;
    HaCohCfg1.Bits.osb_bl_vna_thr             = 0x1;
  }

  UsraCsrWrite (SocId, BoxInst, HA_COH_CFG_1_CHABC_SAD1_REG, HaCohCfg1.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Configure StaleAtoSOptEn

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param StaleAtoSOptEn       - A to S directory optimization. When RdData*
                                finds DIR=A and all snoop responses received are
                                RspI, then directory is moved to S and data is
                                returned in S-state. This optimization will not
                                be effective in xNC configuration where BuriedM
                                is possible.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureStaleAtoSOptEn (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   StaleAtoSOptEn
  )
{
  HA_COH_CFG_1_CHABC_SAD1_STRUCT    HaCohCfg1;
  HA_XNC_NID_MASK_CHABC_SAD1_STRUCT NidMask;

  HaCohCfg1.Data = UsraCsrRead (SocId, BoxInst, HA_COH_CFG_1_CHABC_SAD1_REG);
  HaCohCfg1.Bits.staleatosopten = StaleAtoSOptEn;
  UsraCsrWrite (SocId, BoxInst, HA_COH_CFG_1_CHABC_SAD1_REG, HaCohCfg1.Data);

  NidMask.Data = UsraCsrRead (SocId, BoxInst, HA_XNC_NID_MASK_CHABC_SAD1_REG);

  if (StaleAtoSOptEn) {
    NidMask.Bits.atos_opt_enable_nid_mask = GetSocketPresentBitMap ();
  } else {
    NidMask.Bits.atos_opt_enable_nid_mask = 0;
  }

  UsraCsrWrite (SocId, BoxInst, HA_XNC_NID_MASK_CHABC_SAD1_REG, NidMask.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Configure HA OSB

  @param SbspSocId            - SBSP Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param EnableOsbLocRd       - Threshold based enablement of local read OSB
  @param EnableOsbLocRdCur    - Threshold based&nbsp;enablement of local RdCur OSB
  @param EnableOsbRmtRd       - Force enablement of remote read OSB
  @param Is2SConfig           - 2S config flag

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureHaOsbSad (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   EnableOsbLocRd,
  UINT32   EnableOsbLocRdCur,
  UINT32   EnableOsbRmtRd,
  UINT8    Is2SConfig
  )
{
  HA_OSB_CHABC_SAD1_STRUCT   HaOsbSad;

  HaOsbSad.Data = UsraCsrRead (SocId, BoxInst, HA_OSB_CHABC_SAD1_REG);

  HaOsbSad.Bits.en_osb_loc_rd = EnableOsbLocRd;
  HaOsbSad.Bits.en_osb_loc_rdcur = EnableOsbLocRdCur;
  HaOsbSad.Bits.en_osb_rmt_rd = EnableOsbRmtRd;

  if (Is2SConfig) {
    HaOsbSad.Bits.osb_bl_vna_locrd_thr  = 0x1;
    HaOsbSad.Bits.osb_bl_vna_rmtrd_thr  = 0x1;
    HaOsbSad.Bits.osb_snp_locrd_thr     = 0x1;
    HaOsbSad.Bits.osb_snp_rmtrd_thr     = 0x1;
  }

  UsraCsrWrite (SocId, BoxInst, HA_OSB_CHABC_SAD1_REG, HaOsbSad.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Configure OSB Settings

  @param SbspSocId            - SBSP Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param IodcEnable           - Enabled IODC. IODC can be enabled only in
                                DIRECTORY enabled configurations.
  @param DisDirectory         - Disable Directory. Directory should be
                                disabled for single socket configurations.
                                Directory should be enabled for all multi-socket
                                configurations.
  @param SysOsbEn             - System OSB enable or not
  @param SysConfig            - System Configure
  @param Is4SRing             - Is 4S Ring or not
  @param Is4SFC               - Is 4S full connection

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureOsbSetting (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   IodcEnable,
  UINT32   DisDirectory,
  UINT8    SysOsbEn,
  UINT32   SysConfig,
  UINT32   Is4SRing,
  UINT32   Is4SFC
  )
{
  HA_COH_CHABC_SAD1_STRUCT           HaCoh;
  HA_XNC_NID_MASK_CHABC_SAD1_STRUCT  HaXncNidMask;

  HaCoh.Data = UsraCsrRead (SocId, BoxInst, HA_COH_CHABC_SAD1_REG);
  HaCoh.Bits.iodc_enable = IodcEnable;
  HaCoh.Bits.dis_directory = DisDirectory;

  if (SysOsbEn == FALSE) {
    HaCoh.Bits.en_osb_loc_invitoe = 0;
    HaCoh.Bits.osb_ad_vna_thr = 0;
    HaCoh.Bits.osb_snp_thr = 0;
  } else {
    HaCoh.Bits.en_osb_loc_invitoe = 1;
    //
    // 4929489: CHA tile performance recommended CSR values for BIOS programming
    //          This will change in future based on the part core count
    //
    HaCoh.Bits.osb_ad_vna_thr= 0;
    HaCoh.Bits.osb_snp_thr = 1;

    //
    // s5370161: OSB Setting for 4S Ring, 4S FC and 8S
    //
    if (SysConfig == SYS_CONFIG_4S){
      if (Is4SRing){
        HaCoh.Bits.osb_ad_vna_thr = 2;
      } else if (Is4SFC){
        HaCoh.Bits.osb_ad_vna_thr = 1;
      }
    } else if (SysConfig == SYS_CONFIG_8S){
      HaCoh.Bits.osb_ad_vna_thr = 1;
    }

    //
    // Configure nodes that will be allowed to OSB for local read and remote read/InvItoE.
    //
    HaXncNidMask.Data = UsraCsrRead (SocId, BoxInst, HA_XNC_NID_MASK_CHABC_SAD1_REG);
    HaXncNidMask.Bits.rd_osb_enable_nid_mask = GetSocketPresentBitMap ();
    UsraCsrWrite (SocId, BoxInst, HA_XNC_NID_MASK_CHABC_SAD1_REG, HaXncNidMask.Data);
  }

  UsraCsrWrite (SocId, BoxInst, HA_COH_CHABC_SAD1_REG, HaCoh.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Configure HA Coherency to Enable KTI, ActiveNode and DataM

  @param[in] SbspSocId            - SBSP Node number (Socket ID)
  @param[in] BoxInst              - Box Instance, 0 based
  @param[in] ActiveNodeMask       - Active Nodes In System. One bit per Node ID of
                                    caching agents in the system. It is used to
                                    control which node snoops are spawned to and
                                    responses are expected from. Default value is
                                    NodeID_0 and NodeID_1 active.
  @param[in] KtiEnabledMask       - Bitmask indicating which UPI link(s) is enabled.
                                    This is used by OSB logic to determine if a spcific
                                    UPI link is enabled. Bit-0 indicates UPI Link 0,
                                    Bit-1 indicates UPI link 1, ..., Bit-5 indicates UPI
                                    link 5.
  @param[in] RdInvOwnAcceptSm     - Allows Data_M in the local CA without WB to
                                    memory for local RdInvOwn receiving Rsp*IWb.
                                    Expect to be set to 1 in multi-socket systems.
  @param[in] RdDataMigAcceptSm    - Allows Data_M in the local CA without WB to
                                    memory for local RdDataMig receiving Rsp*IWb.
                                    Expect to be set to 1 in multi-socket systems.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureHaCohKtiEnableActiveNodeAndDataM (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   ActiveNodeMask,
  IN UINT32   KtiEnabledMask,
  IN UINT32   RdInvOwnAcceptSm,
  IN UINT32   RdDataMigAcceptSm
  )
{
  HA_COH_CHABC_SAD1_STRUCT                   HaCoh;

  HaCoh.Data = UsraCsrRead (SocId, BoxInst, HA_COH_CHABC_SAD1_REG);

  HaCoh.Bits.active_node_mask = ActiveNodeMask;
  HaCoh.Bits.kti0enabled = KtiEnabledMask & 0x1;
  HaCoh.Bits.kti1enabled = (KtiEnabledMask >> 1) & 0x1;
  HaCoh.Bits.kti2enabled = (KtiEnabledMask >> 2) & 0x1;
  HaCoh.Bits.rdinvownacceptsm = RdInvOwnAcceptSm;
  HaCoh.Bits.rddatamigacceptsm = RdDataMigAcceptSm;

  UsraCsrWrite (SocId, BoxInst, HA_COH_CHABC_SAD1_REG, HaCoh.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Configure CHA to M2M credit without cluster

  @param SocId                    - CPU Socket Node number (Socket ID)
  @param NumOfCha                 - CHA count for socket "SocId"
  @param MaxIMC                   - Max iMC number
  @param AdCreditsToM2mPerCha     - AD credit to M2M per CHA
  @param BlCreditsToM2mPerCha     - BL credit to M2M per CHA
  @param AdCreditsLeftOver        - AD credit leftover
  @param BlCreditsLeftOver        - BL credit leftover
  @param AdCreditMask             - AD Credit Mask
  @param BlCreditMask             - BL Credit Mask

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureChaToM2MCreditWithoutCluster (
  UINT8    SocId,
  UINT8    NumOfCha,
  UINT8    MaxIMC,
  UINT32   AdCreditsToM2mPerCha,
  UINT32   BlCreditsToM2mPerCha,
  UINT8    AdCreditsLeftOver,
  UINT8    BlCreditsLeftOver,
  UINT32   AdCreditMask,
  UINT32   BlCreditMask
  )
{
  HA_AD_CREDITS_CHA_MISC_STRUCT   HaAdCredit;
  HA_BL_CREDITS_CHA_MISC_STRUCT   HaBlCredit;
  UINT32  DataAd32, DataBl32;
  UINT8   Port, Cha;
  KTI_HOST_OUT                    *KtiHostOutPtr;
  KTI_HOST_IN                     *KtiHostInPtr;
  UINT8   ChaNumPerMc = 0;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;
  KtiHostInPtr  = KTI_HOST_IN_PTR;
  DataAd32 = DataBl32 = 0;
  ChaNumPerMc = NumOfCha / MaxIMC;

  for (Port = 0; Port < MaxIMC; Port++) {
    DataAd32 |=  (AdCreditsToM2mPerCha << ( Port * 4));
    DataBl32 |=  (BlCreditsToM2mPerCha << ( Port * 4));
  }

  for (Cha=0; Cha < NumOfCha; Cha++) {
    HaAdCredit.Data = UsraCsrRead (SocId, Cha, HA_AD_CREDITS_CHA_MISC_REG);
    HaAdCredit.Data &= AdCreditMask;
    HaAdCredit.Data |= DataAd32;
    HaBlCredit.Data = UsraCsrRead (SocId, Cha, HA_BL_CREDITS_CHA_MISC_REG);
    HaBlCredit.Data &= BlCreditMask;
    HaBlCredit.Data |= DataBl32;

    //
    // assign credits leftover to logically distant CHA
    //
    if (AdCreditsLeftOver) {
      if (Cha < AdCreditsLeftOver && HaAdCredit.Bits.ad_vna_port2 < MAX_CREDIT ) {
        HaAdCredit.Bits.ad_vna_port2++;
        HaAdCredit.Bits.ad_vna_port3++;
      }
      if (Cha >= (NumOfCha - AdCreditsLeftOver) &&
        HaAdCredit.Bits.ad_vna_port0 < MAX_CREDIT ) {
        HaAdCredit.Bits.ad_vna_port0++;
        HaAdCredit.Bits.ad_vna_port1++;
      }
    }
    if (BlCreditsLeftOver) {
      if (Cha < BlCreditsLeftOver && HaBlCredit.Bits.bl_vna_port2 < MAX_CREDIT) {
        HaBlCredit.Bits.bl_vna_port2++;
        HaBlCredit.Bits.bl_vna_port3++;
      }
      if (Cha >= (NumOfCha - BlCreditsLeftOver) &&
        HaBlCredit.Bits.bl_vna_port0 < MAX_CREDIT ) {
        HaBlCredit.Bits.bl_vna_port0++;
        HaBlCredit.Bits.bl_vna_port1++;
      }
    }

    //
    // Set disadvnaisocres and disblvnaisocres fields to 1 to disable reserving credits for isoch
    // If isoch is enabled, this will be reversed after WR
    //
    // In non-SNC configuration, No need to set disadvnaisocres/disblvnaisocres
    // because no VNA_PORT0/1/2/3 is equal to 1
    //
    if (KtiHostOutPtr->OutIsocEn == TRUE) {
      HaAdCredit.Bits.disadvnaisocres = 0;
      HaBlCredit.Bits.disblvnaisocres = 0;
    } else {
      HaAdCredit.Bits.disadvnaisocres = 1;
      HaBlCredit.Bits.disblvnaisocres = 1;
    }
    UsraCsrWrite (SocId, Cha, HA_AD_CREDITS_CHA_MISC_REG, HaAdCredit.Data);
    UsraCsrWrite (SocId, Cha, HA_BL_CREDITS_CHA_MISC_REG, HaBlCredit.Data);
  }

  //
  // The credit programming wasn't taking effect without the 1->0 transition on the hacreditchange signal
  //
  ChaSetHaCreditChange (SocId, 0x1);
  ChaSetHaCreditChange (SocId, 0x0);

  return EFI_SUCCESS;
}

/**
  CHA: Configure CHA to M2M credit with cluster

  @param SocId                    - CPU Socket Node number (Socket ID)
  @param NumOfCha                 - CHA count for socket "SocId"
  @param NumOfCluster             - Cluster Number
  @param NumOfChaPerCluster       - CHA count per cluster
  @param MaxIMC                   - Max iMC number
  @param AdCreditsToM2mPerCha     - AD credit to M2M per CHA
  @param BlCreditsToM2mPerCha     - BL credit to M2M per CHA
  @param NonClusterChaCredit      - Non Cluster CHA Credit
  @param AdCreditsLeftOver        - AD credit leftover
  @param BlCreditsLeftOver        - BL credit leftover
  @param AdCreditMask             - AD Credit Mask
  @param BlCreditMask             - BL Credit Mask

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureChaToM2MCreditWithCluster (
  UINT8    SocId,
  UINT8    NumOfCha,
  UINT8    NumOfCluster,
  UINT8    NumOfChaPerCluster,
  UINT8    MaxIMC,
  UINT32   AdCreditsToM2mPerCha,
  UINT32   BlCreditsToM2mPerCha,
  UINT32   NonClusterChaCredit,
  UINT8    AdCreditsLeftOver,
  UINT8    BlCreditsLeftOver,
  UINT32   AdCreditMask,
  UINT32   BlCreditMask
  )
{
  UINT32  DataAd32, DataBl32;
  UINT8   Port, NumOfCloseChaAd, NumOfCloseChaBl, Cha, Cluster;
  UINT8   AbsoluteChaNum = 0;
  HA_AD_CREDITS_CHA_MISC_STRUCT   HaAdCredit;
  HA_BL_CREDITS_CHA_MISC_STRUCT   HaBlCredit;
  KTI_HOST_OUT                    *KtiHostOutPtr;
  KTI_HOST_IN                     *KtiHostInPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;
  KtiHostInPtr  = KTI_HOST_IN_PTR;

  HaAdCredit.Data = 0;
  HaBlCredit.Data = 0;
  DataAd32 = DataBl32 = 0;

  NumOfCloseChaAd = NumOfChaPerCluster - AdCreditsLeftOver;
  NumOfCloseChaBl = NumOfChaPerCluster - BlCreditsLeftOver;

  for (Port = 0; Port < MaxIMC; Port++) {       //for mc0, mc1, mc2, mc3
    DataAd32 |= (AdCreditsToM2mPerCha << ( Port * 4));
    DataBl32 |= (BlCreditsToM2mPerCha << ( Port * 4));
  }

  for (Cluster = 0; Cluster < NumOfCluster; Cluster ++) {
    for (Cha = 0; Cha < NumOfChaPerCluster; Cha++) {
      AbsoluteChaNum = Cha + NumOfChaPerCluster * Cluster;
      HaAdCredit.Data = UsraCsrRead (SocId, AbsoluteChaNum, HA_AD_CREDITS_CHA_MISC_REG);
      HaAdCredit.Data &= AdCreditMask; //for mc0, mc1, mc2, mc3
      HaAdCredit.Data |= DataAd32;
      HaBlCredit.Data = UsraCsrRead (SocId, AbsoluteChaNum, HA_BL_CREDITS_CHA_MISC_REG);
      HaBlCredit.Data &= BlCreditMask; //for mc0, mc1, mc2, mc3
      HaBlCredit.Data |= DataBl32;
      //
      // program NonClusterChaCredit
      //
      if (Cluster == 0) {       //for mc0, mc1, mc2, mc3
        HaAdCredit.Bits.ad_vna_port2 = NonClusterChaCredit;
        HaBlCredit.Bits.bl_vna_port2 = NonClusterChaCredit;
        HaAdCredit.Bits.ad_vna_port3 = NonClusterChaCredit;
        HaBlCredit.Bits.bl_vna_port3 = NonClusterChaCredit;
        if (NumOfCluster == 4) {
          HaAdCredit.Bits.ad_vna_port1 = NonClusterChaCredit;
          HaBlCredit.Bits.bl_vna_port1 = NonClusterChaCredit;
        }
      } else if (Cluster == 1) {
        if (NumOfCluster == 4) {
          HaAdCredit.Bits.ad_vna_port0 = NonClusterChaCredit;
          HaBlCredit.Bits.bl_vna_port0 = NonClusterChaCredit;
          HaAdCredit.Bits.ad_vna_port2 = NonClusterChaCredit;
          HaBlCredit.Bits.bl_vna_port2 = NonClusterChaCredit;
          HaAdCredit.Bits.ad_vna_port3 = NonClusterChaCredit;
          HaBlCredit.Bits.bl_vna_port3 = NonClusterChaCredit;
        } else {
          HaAdCredit.Bits.ad_vna_port0 = NonClusterChaCredit;
          HaBlCredit.Bits.bl_vna_port0 = NonClusterChaCredit;
          HaAdCredit.Bits.ad_vna_port1 = NonClusterChaCredit;
          HaBlCredit.Bits.bl_vna_port1 = NonClusterChaCredit;
        }
      } else if (Cluster == 2) {
        HaAdCredit.Bits.ad_vna_port0 = NonClusterChaCredit;
        HaBlCredit.Bits.bl_vna_port0 = NonClusterChaCredit;
        HaAdCredit.Bits.ad_vna_port1 = NonClusterChaCredit;
        HaBlCredit.Bits.bl_vna_port1 = NonClusterChaCredit;
        HaAdCredit.Bits.ad_vna_port3 = NonClusterChaCredit;
        HaBlCredit.Bits.bl_vna_port3 = NonClusterChaCredit;
      } else if (Cluster == 3) {
        HaAdCredit.Bits.ad_vna_port0 = NonClusterChaCredit;
        HaBlCredit.Bits.bl_vna_port0 = NonClusterChaCredit;
        HaAdCredit.Bits.ad_vna_port1 = NonClusterChaCredit;
        HaBlCredit.Bits.bl_vna_port1 = NonClusterChaCredit;
        HaAdCredit.Bits.ad_vna_port2 = NonClusterChaCredit;
        HaBlCredit.Bits.bl_vna_port2 = NonClusterChaCredit;
      }
      //
      // assign credits leftover to logically distant CHA
      //
      if (NumOfCluster == 4) {
        //
        // SNC 4 Mode
        //
        if (AdCreditsLeftOver &&
            ((HaAdCredit.Data & (0x0000000F << (4 * Cluster))) < (UINT32)(0x0000000F << (4 * Cluster)))) {
          //
          // 1st half of clusters, bigger than NumOfCloseCha are distant
          // 2nd half of clusters, smaller than NumOfCloseCha are distant
          //
          if ((Cluster < NumOfCluster / 2 && Cha >= NumOfCloseChaAd) ||
              (Cluster >= NumOfCluster / 2 && Cha < AdCreditsLeftOver)) {
            HaAdCredit.Data += (1 << (4 * Cluster));
          }
        }//AdCreditsLeftOver

        if (BlCreditsLeftOver &&
            ((HaBlCredit.Data & (0x0000000F << (4 * Cluster))) < (UINT32)(0x0000000F << (4 * Cluster)))) {
          //
          // 1st half of clusters, bigger than NumOfCloseCha are distant
          // 2nd half of clusters, smaller than NumOfCloseCha are distant
          //
          if ((Cluster < NumOfCluster / 2 && Cha >= NumOfCloseChaBl) ||
              (Cluster >= NumOfCluster / 2 && Cha < BlCreditsLeftOver)) {
            HaBlCredit.Data += (1 << (4 * Cluster));
          }
        }//BlCreditsLeftOver
      } else {
        //
        // SNC 2 Mode, Cluster 0: MC0 & MC1; Cluster 1: MC2 & MC3
        //
        if (AdCreditsLeftOver &&
            ((HaAdCredit.Data & (0x0000000F << (4 * Cluster * 2))) < (UINT32)(0x0000000F << (4 * Cluster * 2)))) {
          //
          // 1st half of clusters, bigger than NumOfCloseCha are distant
          // 2nd half of clusters, smaller than NumOfCloseCha are distant
          //
          if ((Cluster < NumOfCluster / 2 && Cha >= NumOfCloseChaAd) ||
              (Cluster >= NumOfCluster / 2 && Cha < AdCreditsLeftOver)) {
            //
            // In SNC2 Mode, update the MC2 & MC3 / MC0 & MC1
            //
            HaAdCredit.Data += (1 << (4 * Cluster * 2));
            HaAdCredit.Data += (1 << (4 * (Cluster * 2 + 1)));
          }
        }//AdCreditsLeftOver

        if (BlCreditsLeftOver &&
            ((HaBlCredit.Data & (0x0000000F << (4 * Cluster * 2))) < (UINT32)(0x0000000F << (4 * Cluster * 2)))) {
          //
          // 1st half of clusters, bigger than NumOfCloseCha are distant
          // 2nd half of clusters, smaller than NumOfCloseCha are distant
          //
          if ((Cluster < NumOfCluster / 2 && Cha >= NumOfCloseChaBl) ||
              (Cluster >= NumOfCluster / 2 && Cha < BlCreditsLeftOver)) {
            //
            // In SNC2 Mode, update the MC2 & MC3 / MC0 & MC1
            //
            HaBlCredit.Data += (1 << (4 * Cluster * 2));
            HaBlCredit.Data += (1 << (4 * (Cluster * 2 + 1)));
          }
        }//BlCreditsLeftOver
      }

      //
      // In SNC configuration, set disadvnaisocres/disblvnaisocres
      // when one of VNA_PORT0/1/2/3 is 1
      //
      if (KtiHostOutPtr->OutIsocEn == TRUE) {
        if ((HaAdCredit.Bits.ad_vna_port0 == 1) || (HaAdCredit.Bits.ad_vna_port1 == 1)
            || (HaAdCredit.Bits.ad_vna_port2 == 1) || (HaAdCredit.Bits.ad_vna_port3 == 1)
            ) {
          HaAdCredit.Bits.disadvnaisocres = 1;
        } else {
          HaAdCredit.Bits.disadvnaisocres = 0;
        }
        if ((HaBlCredit.Bits.bl_vna_port0 == 1) || (HaBlCredit.Bits.bl_vna_port1 == 1)
            || (HaBlCredit.Bits.bl_vna_port2 == 1) || (HaBlCredit.Bits.bl_vna_port3 == 1)
           ) {
          HaBlCredit.Bits.disblvnaisocres = 1;
        } else {
          HaBlCredit.Bits.disblvnaisocres = 0;
        }
      } else {
        HaAdCredit.Bits.disadvnaisocres = 1;
        HaBlCredit.Bits.disblvnaisocres = 1;
      }
      UsraCsrWrite (SocId, AbsoluteChaNum, HA_AD_CREDITS_CHA_MISC_REG, HaAdCredit.Data);
      UsraCsrWrite (SocId, AbsoluteChaNum, HA_BL_CREDITS_CHA_MISC_REG, HaBlCredit.Data);
    }//Cha loop
  }//Cluster loop
  //
  // in case of TotCha is not exactly divisible by NumOfCluster
  // program the remainder as the last cluster
  //
  while (++AbsoluteChaNum < NumOfCha) {
    //
    // In SNC configuration, set disadvnaisocres/disblvnaisocres
    // when one of VNA_PORT0/1/2/3 is 1
    //
    if (KtiHostOutPtr->OutIsocEn == TRUE) {
      if ((HaAdCredit.Bits.ad_vna_port0 == 1) || (HaAdCredit.Bits.ad_vna_port1 == 1)
          || (HaAdCredit.Bits.ad_vna_port2 == 1) || (HaAdCredit.Bits.ad_vna_port3 == 1)
          ) {
        HaAdCredit.Bits.disadvnaisocres = 1;
      } else {
        HaAdCredit.Bits.disadvnaisocres = 0;
      }
      if ((HaBlCredit.Bits.bl_vna_port0 == 1) || (HaBlCredit.Bits.bl_vna_port1 == 1)
          || (HaBlCredit.Bits.bl_vna_port2 == 1) || (HaBlCredit.Bits.bl_vna_port3 == 1)
         ) {
        HaBlCredit.Bits.disblvnaisocres = 1;
      } else {
        HaBlCredit.Bits.disblvnaisocres = 0;
      }
    } else {
      HaAdCredit.Bits.disadvnaisocres = 1;
      HaBlCredit.Bits.disblvnaisocres = 1;
    }

    UsraCsrWrite (SocId, AbsoluteChaNum, HA_AD_CREDITS_CHA_MISC_REG, HaAdCredit.Data);
    UsraCsrWrite (SocId, AbsoluteChaNum, HA_BL_CREDITS_CHA_MISC_REG, HaBlCredit.Data);
  }

  //
  // The credit programming wasn't taking effect without the 1->0 transition on the hacreditchange signal
  //
  ChaSetHaCreditChange (SocId, 0x1);
  ChaSetHaCreditChange (SocId, 0x0);

  return EFI_SUCCESS;
}
/**
  CHA: Configure CHA to HBM M2M credit

  @param SocId                    - CPU Socket Node number (Socket ID)
  @param NumOfCha                 - Total Cha number of the Socket
  @param HbmAdClusterCHAsClose    - HBM AD credit to close M2M per CHA
  @param HbmAdClusterCHAsDistant  - HBM AD credit to distant M2M per CHA
  @param HbmBlClusterCHAsClose    - HBM BL credit to close M2M per CHA
  @param HbmBlClusterCHAsDistant  - HBM BL credit to close M2M per CHA

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureChaToHbmM2mCredit (
  UINT8    SocId,
  UINT8    NumOfCha,
  UINT8    HbmAdClusterCHAsClose,
  UINT8    HbmAdClusterCHAsDistant,
  UINT8    HbmBlClusterCHAsClose,
  UINT8    HbmBlClusterCHAsDistant
  )
{
  UINT8                            HbmDataAd;
  UINT8                            HbmDataBl;
  UINT8                            Cha, Cluster, NumOfChaPerCluster;
  UINT8                            AbsoluteChaNum;
  HA_AD_CREDITS_CHA_MISC_STRUCT    HaAdCredit;
  HA_AD_CREDITS1_CHA_MISC_STRUCT   HaAdCredit1;
  HA_BL_CREDITS_CHA_MISC_STRUCT    HaBlCredit;
  HA_BL_CREDITS1_CHA_MISC_STRUCT   HaBlCredit1;

  //HBM is always SNC4, hardcode the cluster num to 4. Reminder: may support 1 die?
  NumOfChaPerCluster = NumOfCha / 4;

  for (Cluster = 0; Cluster < 4; Cluster ++) {
    for (Cha = 0; Cha < NumOfChaPerCluster; Cha++) {
      if (Cha <= (NumOfChaPerCluster / 2)) {
        HbmDataAd = HbmAdClusterCHAsClose;
        HbmDataBl = HbmBlClusterCHAsClose;
      } else {
        HbmDataAd = HbmAdClusterCHAsDistant;
        HbmDataBl = HbmBlClusterCHAsDistant;
      }

      AbsoluteChaNum = Cha + NumOfChaPerCluster * Cluster;

      HaAdCredit.Data  = UsraCsrRead (SocId, AbsoluteChaNum, HA_AD_CREDITS_CHA_MISC_REG);
      HaAdCredit1.Data = UsraCsrRead (SocId, AbsoluteChaNum, HA_AD_CREDITS1_CHA_MISC_REG);
      HaBlCredit.Data  = UsraCsrRead (SocId, AbsoluteChaNum, HA_BL_CREDITS_CHA_MISC_REG);
      HaBlCredit1.Data = UsraCsrRead (SocId, AbsoluteChaNum, HA_BL_CREDITS1_CHA_MISC_REG);

      HaAdCredit.Bits.ad_vna_port4  = HbmDataAd;
      HaAdCredit.Bits.ad_vna_port5  = HbmDataAd;
      HaAdCredit1.Bits.ad_vna_port6 = HbmDataAd;
      HaAdCredit1.Bits.ad_vna_port7 = HbmDataAd;

      HaBlCredit.Bits.bl_vna_port4  = HbmDataBl;
      HaBlCredit.Bits.bl_vna_port5  = HbmDataBl;
      HaBlCredit1.Bits.bl_vna_port6 = HbmDataBl;
      HaBlCredit1.Bits.bl_vna_port7 = HbmDataBl;

      UsraCsrWrite (SocId, AbsoluteChaNum, HA_AD_CREDITS_CHA_MISC_REG, HaAdCredit.Data);
      UsraCsrWrite (SocId, AbsoluteChaNum, HA_AD_CREDITS1_CHA_MISC_REG, HaAdCredit1.Data);
      UsraCsrWrite (SocId, AbsoluteChaNum, HA_BL_CREDITS_CHA_MISC_REG, HaBlCredit.Data);
      UsraCsrWrite (SocId, AbsoluteChaNum, HA_BL_CREDITS1_CHA_MISC_REG, HaBlCredit1.Data);
    }//Cha loop
  }//Cluster loop
   //
  // The credit programming wasn't taking effect without the 1->0 transition on the hacreditchange signal
  //
  ChaSetHaCreditChange (SocId, 0x1);
  ChaSetHaCreditChange (SocId, 0x0);
  return EFI_SUCCESS;
}

/**
  CHA: Read H0 CH_ROUTE_TABLE

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param NumOfRt              - Number of CH_ROUTE_TABLE
  @param H0ChRouteTable       - H0ChRouteTable

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaH0ChRouteTableRead (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    NumOfRt,
  OUT UINT32* H0ChRouteTable
  )
{

  UINT32   ChRouteTable[3] = {H0_CH_ROUTE_TABLE_0_CHA_MISC_REG,
                              H0_CH_ROUTE_TABLE_1_CHA_MISC_REG,
                              H0_CH_ROUTE_TABLE_2_CHA_MISC_REG};

  *H0ChRouteTable = UsraCsrRead (SocId, BoxInst, ChRouteTable[NumOfRt]);

  return EFI_SUCCESS;
}


/**
  CHA: Configure OSB Power Heuristics

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param EnableVal            - Enable OSB Power Heuristics
  @param ProgShiftVal         - OSB Programmable Local readcount shift to determine NUMA mode
  @param TimerFractionVal     - OSB Time period fraction size
  @param TimerMaxVal          - OSB Timer Maximum Value

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigurePowerHeuristics (
  UINT8    SocId,
  UINT8    BoxInst
  )
{
  HA_OSB_POWERHEUR_CHABC_SAD1_STRUCT   HaOsbPwrHeur;

  HaOsbPwrHeur.Data = UsraCsrRead (SocId, BoxInst, HA_OSB_POWERHEUR_CHABC_SAD1_REG);

  HaOsbPwrHeur.Bits.osbpoweren        = 0x1;
  HaOsbPwrHeur.Bits.osbprogshift      = 0x5;
  HaOsbPwrHeur.Bits.osbtimerfraction  = 0x10;
  HaOsbPwrHeur.Bits.osbtimermax       = 0x12;

  UsraCsrWrite (SocId, BoxInst, HA_OSB_POWERHEUR_CHABC_SAD1_REG, HaOsbPwrHeur.Data);

  return EFI_SUCCESS;
}

/**

  ChaGetSetSaiRegisters.

  @param SocId         - CPU Socket Node number (Socket ID)
  @param BoxInst       - Box Instance, 0 based
  @param SaiRegIndex   - SaiRegIndex
  @param Get           - TRUE: read register into *Value, FALSE: write register using data in *Value
  @param *Value        - *Value: pointer of UINT32, can be input & output

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaGetSetSaiRegisters (
  UINT8                      SocId,
  UINT8                      BoxInst,
  IN CHA_SAI_REG_ENUM        SaiRegIndex,
  IN BOOLEAN                 Get,
  IN OUT UINT32              *Value
  )
{
  UINT32 RegAddress;

  switch (SaiRegIndex) {
    case ChaLbWMcheckRacN0RegIndex:
      RegAddress = LB_W_MCHECK_CHA_RAC_N0_CHABC_SAD1_REG;
      break;

    case ChaLbWMcheckRac0x3FFN0RegIndex:
      RegAddress = LB_W_MCHECK_CHA_RAC_0X3FF_N0_CHABC_SAD1_REG;
      break;

    case ChaLbWMcheckWac0x3FFN0RegIndex:
      RegAddress = LB_W_MCHECK_CHA_WAC_0X3FF_N0_CHABC_SAD1_REG;
      break;

    case ChaLbWMcheckWac0x3FFN1RegIndex:
      RegAddress = LB_W_MCHECK_CHA_WAC_0X3FF_N1_CHABC_SAD1_REG;
      break;
    case ChaLbWMcheckWacN0RegIndex:
      RegAddress = LB_W_MCHECK_CHA_WAC_N0_CHABC_SAD1_REG;
      break;

    case ChaLbWMcheckWacN1RegIndex:
      RegAddress = LB_W_MCHECK_CHA_WAC_N1_CHABC_SAD1_REG;
      break;

    case ChaLbWMcheckCp0x3FFN0RegIndex:
      RegAddress = LB_W_MCHECK_CHA_CP_0X3FF_N0_CHABC_SAD1_REG;
      break;

    case ChaLbWMcheckCp0x3FFN1RegIndex:
      RegAddress = LB_W_MCHECK_CHA_CP_0X3FF_N1_CHABC_SAD1_REG;
      break;

    case ChaLbWMcheckCpN0RegIndex:
      RegAddress = LB_W_MCHECK_CHA_CP_N0_CHABC_SAD1_REG;
      break;

    case ChaLbWMcheckCpN1RegIndex:
      RegAddress = LB_W_MCHECK_CHA_CP_N1_CHABC_SAD1_REG;
      break;

    default:
      RegAddress = 0;
      break;
  }

  if (!RegAddress) {
    return EFI_INVALID_PARAMETER;
  }

  if (Get) {
    *Value = UsraCsrRead (SocId, BoxInst, RegAddress);
  } else {
    UsraCsrWrite (SocId, BoxInst, RegAddress, *Value);
  }

  return EFI_SUCCESS;
}

/**
  Programs CHA to set the HA credit change field.

  @param SocId            The socket number ID
  @param HaCreditChange   The HA credit change value

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                          - EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
ChaSetHaCreditChange (
  IN UINT8    SocId,
  IN UINT8    HaCreditChange
  )
{
  HA_THRESHOLDS_CHABC_SAD1_STRUCT   HaThresholds;

  HaThresholds.Data = UsraCsrRead (SocId, 0, HA_THRESHOLDS_CHABC_SAD1_REG);
  HaThresholds.Bits.hacreditchange = (UINT32)HaCreditChange & 0x1;
  UsraCsrWrite (SocId, 0, HA_THRESHOLDS_CHABC_SAD1_REG, HaThresholds.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Configure CHA for EliminateDirectoryInFarMemory

  @param SocId                   CPU Socket Node number (Socket ID)

  @retval EFI_SUCCESS            The function is executed successfully without any error.

**/
EFI_STATUS
EFIAPI
ChaEliminateDirectoryInFarMemory (
  IN UINT8  SocId
  )
{
  HA_COH_CFG_2_CHABC_SAD1_STRUCT            HaCohCfg2;
  CBPIPES_UNICAST_CFG_SPARE_CHA_PMA_STRUCT  UnicastCfgSpare;
  UINT8                                     ChaNumber;
  UINT32                                    TotalChaNumber;

  TotalChaNumber = GetTotChaCount (SocId);

  // CHABC Registers are only valid on CHA 0
  HaCohCfg2.Data = UsraCsrRead (SocId, 0, HA_COH_CFG_2_CHABC_SAD1_REG);
  HaCohCfg2.Bits.sparehacsrunnnh |= BIT0 + BIT1;
  UsraCsrWrite (SocId, 0, HA_COH_CFG_2_CHABC_SAD1_REG, HaCohCfg2.Data);

  for (ChaNumber = 0; ChaNumber < TotalChaNumber; ChaNumber++) {
    UnicastCfgSpare.Data = UsraCsrRead (SocId, ChaNumber, CBPIPES_UNICAST_CFG_SPARE_CHA_PMA_REG);
    UnicastCfgSpare.Data |= BIT0;
    UsraCsrWrite (SocId, ChaNumber, CBPIPES_UNICAST_CFG_SPARE_CHA_PMA_REG, UnicastCfgSpare.Data);
  }

  return EFI_SUCCESS;
}

/**
  CHA: Configure CHA For DisableDirForAppDirect

  @param SocId                   CPU Socket Node number (Socket ID)

  @retval EFI_SUCCESS            The function is executed successfully without any error.

**/
EFI_STATUS
EFIAPI
ChaDisableDirForAppDirect (
  IN UINT8  SocId
  )
{
  HA_COH_CFG_2_CHABC_SAD1_STRUCT  HaCohCfg2;

  // CHABC Registers are only valid on CHA 0
  HaCohCfg2.Data = UsraCsrRead (SocId, 0, HA_COH_CFG_2_CHABC_SAD1_REG);
  HaCohCfg2.Bits.sparehacsrunnnh |= BIT2;
  UsraCsrWrite (SocId, 0, HA_COH_CFG_2_CHABC_SAD1_REG, HaCohCfg2.Data);

  return EFI_SUCCESS;
}

/**

  CHA: Configure Clock Modulation: disable_clockmod in CLOCK_MODULATION_RATE_CHABC_SAD_REG

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param ClockModulationEn    - TRUE: enable, FALSE: disable


  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureClockModulation (
  UINT8    SocId,
  UINT8    BoxInst,
  BOOLEAN  ClockModulationEn
  )
{
  CLOCK_MODULATION_RATE_CHABC_SAD_STRUCT           ClockModulationRate;

  ClockModulationRate.Data = UsraCsrRead (SocId, BoxInst, CLOCK_MODULATION_RATE_CHABC_SAD_REG);
  if (ClockModulationEn) {
    ClockModulationRate.Bits.disable_clockmod = 0;
  }
  UsraCsrWrite (SocId, BoxInst, CLOCK_MODULATION_RATE_CHABC_SAD_REG, ClockModulationRate.Data);

  return EFI_SUCCESS;
}

/**

  Programs CHA to set snoop throttle threshold.

  @param SocId              The socket number ID
  @param BoxInst            Box Instance, 0 based
  @param SnoopThrottleLevel The Snoop throttle threshold

  @retval - EFI_UNSUPPORTED:       The function not implemented
          - EFI_INVALID_PARAMETER: The function is not executed successfully
                                   due to some illegal input.
          - EFI_SUCCESS:           The function is excuted successfully

**/
EFI_STATUS
EFIAPI
ChaSetSnoopThrottleLevel (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    SnoopThrottleLevel
  )
{
  return EFI_UNSUPPORTED;
}

/**
  CHA: Override default snoop_credits setting
  This function may be called for CLX+FPGA, and for ICX-SP-LCC FPGA platform configurations.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
ChaOverrideSnoopCredits  (
  UINT8    SocId,
  UINT8    BoxInst
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Return size of per entry in CHA snoop fanout router table, that is, the
  length of send2ktiport_* bitfield. The function also provide information such as:
  - if send2ktiport_* field is extended to CHA_RTR_SNP_CTL1 register.
  - the first requester node ID field (send2ktiport_i) in CHA_RTR_SNP_CTL1 register.

  @param[out] SupportExtSnpFanoutTable  TRUE if send2ktiport_* field is extended to
                                        CHA_RTR_SNP_CTL1 register. FALSE otherwise.
  @param[out] FirstNodeIdInExtTable     The first requester Node ID in CHA_RTR_SNP_CTL1 register.

  @retval  Size of per entry in CHA snoop fanout router table, i.e. length of send2ktiport_* bitfield.

**/
UINT8
EFIAPI
ChaSnpFanoutTablePerEntrySize (
  OUT BOOLEAN  *SupportExtSnpFanoutTable, OPTIONAL
  OUT UINT8    *FirstNodeIdInExtTable     OPTIONAL
  )
{
  if (SupportExtSnpFanoutTable != NULL) {
    //
    // return FALSE as CHA_RTR_SNP_CTL1 register does not have send2ktiport_* field
    //
    *SupportExtSnpFanoutTable = FALSE;
  }

  if (FirstNodeIdInExtTable != NULL) {
    //
    // return 0 as CHA_RTR_SNP_CTL1 register does not have send2ktiport_* field
    //
    *FirstNodeIdInExtTable = 0;
  }

  //
  // return length of send2ktiport_* bitfield
  //
  return 4;
}

