/** @file
  Implementation of ITP/TC/OTC

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

#include <Library/CpuCsrAccessLib.h>
#include <Library/RasMailBoxLib.h>
#include <Library/MmPciExtLib.h>
#include <Library/IoLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <RcRegs.h>

/**
  Enable IRP errors.

  @param[in]  Socket    -- socket index
  @param[in]  Bus       -- bus index
  @param[in]  Device    -- device index
  @param[in]  Function  -- Function index
**/
VOID
EFIAPI
IioEnableIrpError (
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function
  )
{
  IRPRINGERRCTL_IIO_RAS_STRUCT  IrpRingErrCtl;
  IRPERRCTL_IIO_RAS_STRUCT      IrpErrCtl;

  RAS_MAIL_BOX  *MailBox;
  MailBox = GetRasMailBox ();

  if (MailBox->IioInitPar.IioIrpErrorEn == 1) {
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (IRPPERRSV_N0_IIO_RAS_REG), NULL), MailBox->IioInitPar.IioCohSevBitMap);

    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (IRPRINGERRCNT_IIO_RAS_REG), NULL), 0xff);

    IrpRingErrCtl.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (IRPRINGERRCTL_IIO_RAS_REG), NULL));
    IrpRingErrCtl.Bits.unexpected_go_err_rcvd = 1;
    IrpRingErrCtl.Bits.snpq_rf_addr_parity_error = 1;
    IrpRingErrCtl.Bits.txq1_addr_par_error = 1;
    IrpRingErrCtl.Bits.txq0_addr_par_error = 1;
    IrpRingErrCtl.Bits.ak1_cqid_parity_error = 1;
    IrpRingErrCtl.Bits.ak0_cqid_parity_error = 1;
    IrpRingErrCtl.Bits.bl_cqid_parity_error = 1;
    IrpRingErrCtl.Bits.bl_ecc_uncorrectable_error = 1;
    IrpRingErrCtl.Bits.bl_ecc_corrected_error = 1;
    IrpRingErrCtl.Bits.blq_parity_error = 1;
    IrpRingErrCtl.Bits.rxdata0_parity_error = 1;
    IrpRingErrCtl.Bits.rxdata1_parity_error = 1;
    IrpRingErrCtl.Bits.snp_addr_parity_error = 1;
    IrpRingErrCtl.Bits.bl_parity_error = 1;
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (IRPRINGERRCTL_IIO_RAS_REG), NULL), IrpRingErrCtl.Data);

    IrpErrCtl.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (IRPERRCTL_IIO_RAS_REG), NULL));
    if (IsSiliconWorkaroundEnabled ("S14011491308")) {
      IrpErrCtl.Bits.iommu_faf_rf_addr_par_error = 0;
    } else {
      IrpErrCtl.Bits.iommu_faf_rf_addr_par_error = 1;
    }

    IrpErrCtl.Bits.pf_timeout_err_cs3 = 1;
    IrpErrCtl.Bits.pf_timeout_err_cs2 = 1;
    IrpErrCtl.Bits.pf_timeout_err_cs1 = 1;
    IrpErrCtl.Bits.pf_timeout_err_cs0 = 1;
    IrpErrCtl.Bits.addrcam1_addr_par_error = 1;
    IrpErrCtl.Bits.addrcam0_addr_par_error = 1;
    IrpErrCtl.Bits.faf_rf_addr_par_error = 1;
    IrpErrCtl.Bits.p2p_hdr_rf_addr_par_error = 1;
    IrpErrCtl.Bits.protocol_parity_error = 1;
    if (IsSiliconWorkaroundEnabled ("S14011415986")) {
      IrpErrCtl.Bits.protocol_qt_overflow_underflow = 0;
    } else {
      IrpErrCtl.Bits.protocol_qt_overflow_underflow = 1;
    }
    IrpErrCtl.Bits.protocol_rcvd_unexprsp = 1;
    IrpErrCtl.Bits.csr_acc_32b_unaligned = 1;
    IrpErrCtl.Bits.wrcache_uncecc_error = 1;
    IrpErrCtl.Bits.protocol_rcvd_poison = 1;
    IrpErrCtl.Bits.wrcache_correcc_error = 1;
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (IRPERRCTL_IIO_RAS_REG), NULL), IrpErrCtl.Data);
  }
}

/**

  Enable Iio ITC


  @param  [in]  Socket  -- socket index
  @param  [in]  Bus     -- bus index
  @param  [in]  Device  -- device index
  @param  [in]  Function-- Function index

  @retval none

**/
VOID
EFIAPI
IioEnableItc (
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function
  )
{
/*
  ITCERRSEV_IIO_RAS_STRUCT        ItcErrSev;
  RAS_MAIL_BOX    *pMailBox = NULL;

  pMailBox = GetRasMailBox ();

  ItcErrSev.Data = pMailBox->IioInitPar.IioItcErrSevBitMap;
  ItcErrSev.Bits.itc_vtmisc_hdr_rf = 0x2;

  ItcErrSev.Bits.itc_hw_assert = 2;
  ItcErrSev.Bits.itc_misc_prh_overflow = 2;
  ItcErrSev.Bits.itc_enq_overflow = 2;
  ItcErrSev.Bits.itc_mtc_tgt_err = 2;
  ItcErrSev.Bits.itc_mabort = 0;
  ItcErrSev.Bits.itc_cabort = 0;
  ItcErrSev.Bits.itc_ecc_uncor_rf = 2;
  ItcErrSev.Bits.itc_ecc_cor_rf = 0;
  ItcErrSev.Bits.itc_par_addr_rf = 2;
  ItcErrSev.Bits.itc_vtmisc_hdr_rf = 2;
  ItcErrSev.Bits.itc_par_hdr_rf = 2;
  ItcErrSev.Bits.itc_par_iosf_dat = 2;
  ItcErrSev.Bits.itc_irp_cred_of = 2;
  ItcErrSev.Bits.itc_irp_cred_uf = 2;

  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (ITCERRSEV_IIO_RAS_REG), NULL), ItcErrSev.Data);
*/
}

/**
  Enable IIO TC.

  @param[in]  Socket    -- socket index
  @param[in]  Bus       -- bus index
  @param[in]  Device    -- device index
  @param[in]  Function  -- Function index
**/
 VOID
 EFIAPI
 IioEnableTc(
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function
  )
{
  ITCERRCTL_IIO_RAS_STRUCT      ITcErrCtl;
  OTCERRCTL_IIO_RAS_STRUCT      OTcErrCtl;

  RAS_MAIL_BOX  *MailBox;
  MailBox = GetRasMailBox ();

  ITcErrCtl.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (ITCERRCTL_IIO_RAS_REG), NULL));
  ITcErrCtl.Bits.itc_msgd_illegal_size = 1;
  ITcErrCtl.Bits.itc_par_iosf_cmd = 1;
  ITcErrCtl.Bits.itc_hw_assert = 1;
  ITcErrCtl.Bits.itc_misc_prh_overflow = 1;
  ITcErrCtl.Bits.itc_sb_of = 1;
  ITcErrCtl.Bits.itc_sb_uf = 1;
  ITcErrCtl.Bits.itc_iosf_cred_uf = 1;
  ITcErrCtl.Bits.itc_iosf_cred_of = 1;
  if (IsSiliconWorkaroundEnabled ("S14011009740")) {
    ITcErrCtl.Bits.itc_enq_overflow = 0;
  } else {
    ITcErrCtl.Bits.itc_enq_overflow = 1;
  }
  ITcErrCtl.Bits.inb_unsuccessful_cmpl = 0;
  if (MailBox->IioInitPar.ItcOtcCaMaEnable == 0) {
    ITcErrCtl.Bits.itc_mabort = 0;
    ITcErrCtl.Bits.itc_cabort = 0;
  } else {
    ITcErrCtl.Bits.itc_mabort = 1;
    ITcErrCtl.Bits.itc_cabort = 1;
  }
  ITcErrCtl.Bits.itc_ecc_uncor_rf = 1;
  ITcErrCtl.Bits.itc_ecc_cor_rf = 1;
  ITcErrCtl.Bits.itc_par_addr_rf = 1;
  ITcErrCtl.Bits.itc_vtmisc_hdr_rf = 1;
  ITcErrCtl.Bits.itc_par_hdr_rf = 1;
  ITcErrCtl.Bits.itc_par_iosf_dat = 1;
  if (IsSiliconWorkaroundEnabled ("S14011367913")) {
    ITcErrCtl.Bits.itc_irp_cred_of = 0;
    ITcErrCtl.Bits.itc_irp_cred_uf = 0;
  } else {
    ITcErrCtl.Bits.itc_irp_cred_of = 1;
    ITcErrCtl.Bits.itc_irp_cred_uf = 1;
  }
  ITcErrCtl.Bits.sw2iosf_req_buf_ov = 1;
  ITcErrCtl.Bits.sw2iosf_req_buf_uf = 1;
  ITcErrCtl.Bits.sw2iosf_mps_err = 1;
  if (IsSiliconWorkaroundEnabled ("S14011009740")) {
    ITcErrCtl.Bits.itc_enq_data_overflow = 0;
  } else {
    ITcErrCtl.Bits.itc_enq_data_overflow = 1;
  }
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (ITCERRCTL_IIO_RAS_REG), NULL), ITcErrCtl.Data);

  OTcErrCtl.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (OTCERRCTL_IIO_RAS_REG), NULL));
  OTcErrCtl.Bits.otc_poison_mabort = 1;
  OTcErrCtl.Bits.vmd_cfgbar_overflow = 1;
  OTcErrCtl.Bits.otc_misc_oobmsm_mult_pend = 1;
  OTcErrCtl.Bits.otc_misc_snarf_fifo_uf = 1;
  OTcErrCtl.Bits.otc_misc_snarf_fifo_of = 1;
  OTcErrCtl.Bits.otc_gpsb_par = 1;
  OTcErrCtl.Bits.otc_txn_dur_lock = 1;
  OTcErrCtl.Bits.otc_hw_assert = 1;
  OTcErrCtl.Bits.otc_irp_addr_par = 1;
  OTcErrCtl.Bits.otc_irp_dat_par = 1;
  OTcErrCtl.Bits.otc_par_rte = 1;
  OTcErrCtl.Bits.otc_mctp_bcast_to_dmi = 1;
  OTcErrCtl.Bits.otc_ecc_cor_rf = 1;
  OTcErrCtl.Bits.otc_mtc_tgt_err = 1;
  if (MailBox->IioInitPar.ItcOtcCaMaEnable == 0) {
    OTcErrCtl.Bits.otc_mabort = 0;
    OTcErrCtl.Bits.otc_cabort = 0;
  } else {
    OTcErrCtl.Bits.otc_mabort = 1;
    OTcErrCtl.Bits.otc_cabort = 1;
  }
  OTcErrCtl.Bits.otc_ecc_uncor_rf = 1;
  OTcErrCtl.Bits.otc_par_addr_rf = 1;
  OTcErrCtl.Bits.otc_par_hdr_rf = 1;
  OTcErrCtl.Bits.otc_crdt_of = 1;
  OTcErrCtl.Bits.otc_crdt_uf = 1;
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (OTCERRCTL_IIO_RAS_REG), NULL), OTcErrCtl.Data);
}

 /**

   Enable Iio OTC


  @param  [in]  Socket  -- socket index
  @param  [in]  Bus     -- bus index
  @param  [in]  Device  -- device index
  @param  [in]  Function-- Function index

  @retval none

 **/
 VOID
 EFIAPI
 IioEnableOtc (
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function
  )
 {
/*
   OTCERRSEV_N0_IIO_RAS_STRUCT OtcErrSev;
   RAS_MAIL_BOX    *pMailBox = NULL;
   pMailBox = GetRasMailBox ();

   OtcErrSev.Data = (pMailBox->IioInitPar.IioOtcErrSevBitMap) ;

   OtcErrSev.Bits.otc_hw_assert = 2;
   OtcErrSev.Bits.otc_irp_addr_par = 2;
   OtcErrSev.Bits.otc_irp_dat_par = 2;
   OtcErrSev.Bits.otc_par_rte = 2;
   OtcErrSev.Bits.otc_mctp_bcast_to_dmi = 2;
   OtcErrSev.Bits.otc_ecc_cor_rf = 0;
   OtcErrSev.Bits.otc_mtc_tgt_err = 2;
   OtcErrSev.Bits.otc_mabort = 0;
   OtcErrSev.Bits.otc_cabort = 0;
   OtcErrSev.Bits.otc_ecc_uncor_rf = 2;
   OtcErrSev.Bits.otc_par_addr_rf = 2;
   OtcErrSev.Bits.otc_par_hdr_rf = 2;

// for ICX
   OtcErrSev.Bits.otc_crdt_of = 2;
   OtcErrSev.Bits.otc_crdt_uf = 2;
//else
   OtcErrSev.Bits.otc_sb_of = 2;
   OtcErrSev.Bits.otc_sb_uf = 2;

   MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (OTCERRSEV_N0_IIO_RAS_REG), NULL), OtcErrSev.Data);
*/
 }

/**

Enable Iio Enable IioSetIioMisc


  @param  [in]  Socket  -- socket index
  @param  [in]  Bus     -- bus index
  @param  [in]  Device  -- device index
  @param  [in]  Function-- Function index
  @param  [in]  IioMiscErrorEn      - Policy Data
  @param  [in]  IioMiscErrorSevMap  - MIERRSV
  @param  [in]  IioMiscErrorBitMap  - MIERRCTL
  @retval none


**/
VOID
EFIAPI
IioSetIioMisc (
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function,
  IN   UINT8    IioMiscErrorEn,
  IN   UINT32   IioMiscErrorSevMap,
  IN   UINT32   IioMiscErrorBitMap
  )
{

}

/**
  This function is to check the IRP Error, if there is error on IRP on the
  ErrSeverity return TRUE, else return FALSE

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param ErrSeverity The error's severity
  @retval None.

**/
BOOLEAN
EFIAPI
IsIioIrpError (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Function,
  IN  UINT8  ErrSeverity
  )
{
  UINT32       IrpErrSt;

  IrpErrSt = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (IRPERRST_IIO_RAS_REG), NULL));
  if (IrpErrSt != 0) {
    return TRUE;
  }

  return FALSE;
}

/**
  This function is to check the IRP RING Error, if there is error on IRP RING on the
  ErrSeverity return TRUE, else return FALSE

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param ErrSeverity The error's severity
  @retval None.

**/
BOOLEAN
EFIAPI
IsIioIrpRingError (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Function,
  IN  UINT8  ErrSeverity
  )
{
  UINT32       IrpRingErrSt;

  IrpRingErrSt = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (IRPRINGERRST_IIO_RAS_REG), NULL));
  if (IrpRingErrSt != 0) {
    return TRUE;
  }

  return FALSE;
}

/**
  This function is to check the ITC Error, if there is error on ITC on the
  ErrSeverity return TRUE, else return FALSE

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param ErrSeverity The error's severity
  @retval None.

**/
BOOLEAN
EFIAPI
IsIioItcError (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Function,
  IN  UINT8  ErrSeverity
  )
{
  UINT32       ITCErrSt;

  ITCErrSt = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (ITCERRST_IIO_RAS_REG), NULL));

  if (ITCErrSt != 0) {
    return TRUE;
  }
  return FALSE;
}

/**
  This function is to check the OTC Error, if there is error on OTC on the
  ErrSeverity return TRUE, else return FALSE

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param ErrSeverity The error's severity
  @retval None.

**/
BOOLEAN
EFIAPI
IsIioOtcError (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Function,
  IN  UINT8  ErrSeverity
  )
{
  UINT32       OTCErrSt;

  OTCErrSt = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (OTCERRST_IIO_RAS_REG), NULL));

  if (OTCErrSt != 0) {
    return TRUE;
  }
  return FALSE;
}

/**
  This function is clear TC Error Status

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @retval None.

**/
VOID
EFIAPI
ClearTcErrst (
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function
  )
{
  UINT32       ITCErrSt;
  UINT32       OTCErrSt;

  ITCErrSt = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (ITCERRST_IIO_RAS_REG), NULL));
  OTCErrSt = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (OTCERRST_IIO_RAS_REG), NULL));
  if (ITCErrSt != 0) {
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (ITCERRST_IIO_RAS_REG), NULL), ITCErrSt);
  }
  if (OTCErrSt != 0) {
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (OTCERRST_IIO_RAS_REG), NULL), OTCErrSt);
  }
}

/**
  clear RAS pci device status, each iio stack have one RAS device.

  @param  [in]  Socket  -- socket index
  @param  [in]  Bus     -- bus index
  @param  [in]  Device  -- device index
  @param  [in]  Function-- Function index
  @param  [in]  ErrSeverity The error's severity

  @retval none
**/
VOID
EFIAPI
ClearRasDevice (
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function,
  IN   UINT32   ErrSeverity
  )
{
  UINT32       IrpRingErrSt;
  UINT32       IrpErrSt;

  ClearTcErrst(Socket, Bus, Device, Function);

  IrpErrSt = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (IRPERRST_IIO_RAS_REG), NULL));
  if (IrpErrSt != 0) {
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (IRPERRST_IIO_RAS_REG), NULL), IrpErrSt);
  }

  IrpRingErrSt = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (IRPRINGERRST_IIO_RAS_REG), NULL));
  if (IrpRingErrSt != 0) {
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (IRPRINGERRST_IIO_RAS_REG), NULL), IrpRingErrSt);
  }
}

/**
  This function enable forwarding of Viral indication

   @param Bus       - Bus
   @param Device    - Device
   @param Function  - Function
   @param ClearFlg  - Viral status clear flag
   @param IoMcaEn   - IoMca Enable

   @retval    none.
**/
VOID
EFIAPI
IioEnableViral (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function,
  IN      UINT8   ClearFlg,
  IN      UINT8   IoMcaEn
  )
{
  VIRAL_IIO_RAS_STRUCT      IioViral; //VIRAL_IIO_RAS_REG
  UINT32                    IioViralMask;

  //
  // Set Viral Enable on the IIO
  //
  IioViral.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (VIRAL_IIO_RAS_REG), NULL));
  if (ClearFlg) {
    IioViral.Bits.iio_viral_status = 1;
    IioViralMask = 0xc0000000;
  } else {
    IioViral.Bits.iio_viral_status = 0;
    IioViralMask = 0x80000000;
  }
  IioViral.Bits.iio_viral_state = 1;
  do {
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (VIRAL_IIO_RAS_REG), NULL), IioViral.Data);
    IioViral.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (VIRAL_IIO_RAS_REG), NULL));
  } while (IioViral.Bits.iio_viral_state == 1 || (ClearFlg && IioViral.Bits.iio_viral_status == 1));

  IioViral.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (VIRAL_IIO_RAS_REG), NULL));

  if (IioViral.Bits.iio_viral_status == 1){
    IioViral.Bits.iio_viral_status = 0;
  }

  IioViral.Bits.iio_signal_global_fatal = 1;

  IioViral.Bits.iio_global_viral_mask = 0;
  IioViral.Bits.iio_fatal_viral_alert_enable = 1;
  IioViral.Bits.generate_viral_alert = 0;
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (VIRAL_IIO_RAS_REG), NULL), IioViral.Data);
}