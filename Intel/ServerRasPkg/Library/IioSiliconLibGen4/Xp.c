/** @file
  Implementation of XP IIO RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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
//#include <RcRegs.h>
#include <RcRegs.h>
#include <Library/RasDebugLib.h>
#include <Library/RasMailBoxLib.h>
#include <Library/MpExtensionLib.h>
#include <Library/MmPciExtLib.h>
#include <Library/IoLib.h>
#include <Register/IioRegDef.h>
#include <Library/PcieRasLib.h>
#include <Library/PcieStdLib.h>
#include <Register/PcieRegDef.h>
#include <Library/SiliconWorkaroundLib.h>

/**
Program Iio XP  errors(Correctable, Uncorrectable, Fatal) .

@param Bus       - Bus
@param Device    - Device
@param Function  - Function
@param CorrErrMsk  - Correctable error mask

@retval  none
**/
VOID
EFIAPI
IioXPProgramCorrErrors (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  XPCORERRMSK_IIO_PCIEDMI_STRUCT      XpCorErrMsk;
  XPCOREDMASK_IIO_PCIEDMI_STRUCT      XpCorEdMask;
  RAS_MAIL_BOX                        *pMailBox = NULL;

  //
  // Gen4 IIO doesn't support XP errors, but ICX CPU DMI Port is GEN3
  //
  if (PcieIsDmiPort (Socket, Bus, Device, Function, NULL)) {
    pMailBox = GetRasMailBox ();

    // XP Correctable Error Mask Register
    XpCorErrMsk.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPCORERRMSK_IIO_PCIEDMI_REG), NULL));
    RAS_DEBUG((LEVEL_REG, "IioXPProgramCorrErrors XPCORERRMSK_IIO_PCIEDMI_REG = 0x%x!\n",XpCorErrMsk.Data));
    XpCorEdMask.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPCOREDMASK_IIO_PCIEDMI_REG), NULL));
    RAS_DEBUG((LEVEL_REG, "IioXPProgramCorrErrors XPCOREDMASK_IIO_PCIEDMI_REG = 0x%x!\n",XpCorEdMask.Data));
    // Adaptec RAID (model) encounters SMI storm when pci link bandwidth error is unmasked.
    // Mask error to prevent system hang.
    if (pMailBox->IioInitPar.IioPcieAddCorrErrorEn == 1) {
      XpCorErrMsk.Bits.pci_link_bandwidth_changed_mask = 1;
      XpCorEdMask.Bits.pci_link_bandwidth_changed_detect_mask = 0;
    } else {
      XpCorErrMsk.Bits.pci_link_bandwidth_changed_mask = 1;
      XpCorEdMask.Bits.pci_link_bandwidth_changed_detect_mask = 1;
    }
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPCORERRMSK_IIO_PCIEDMI_REG), NULL), XpCorErrMsk.Data);
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPCOREDMASK_IIO_PCIEDMI_REG), NULL), XpCorEdMask.Data);
  }
}

/**
  Program Iio XP  errors(Correctable, Uncorrectable, Fatal) .

  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param CorrErrMsk  - Correctable error mask

**/
VOID
EFIAPI
IioXPProgramUnCorrErrors (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  UINT32        XpUncEdMask;
  UINT32        XpUncErrMsk;
  RAS_MAIL_BOX  *pMailBox = NULL;

  //
  // Gen4 IIO doesn't support XP errors, but ICX CPU DMI Port is GEN3
  //
  if (PcieIsDmiPort (Socket, Bus, Device, Function, NULL)) {

    RAS_DEBUG((LEVEL_REG, "IioXPProgramUnCorrErrors Bus = 0x%x,Device = 0x%x, Function = 0x%x!\n",Bus,Device,Function));
    pMailBox = GetRasMailBox ();
    // XP Un-Correctable Error Mask Register - Mask bit6 (received_pcie_completion_with_ur_detect_mask)
    XpUncEdMask = IIO_PCIEADD_UNCORR_MSK;
    XpUncErrMsk = IIO_PCIEADD_UNCORR_MSK;
    RAS_DEBUG((LEVEL_REG, "IioXPProgramUnCorrErrors pMailBox->IioInitPar.IioPcieAddUnCorrBitMap = 0x%x!\n",pMailBox->IioInitPar.IioPcieAddUnCorrBitMap));

    if (pMailBox->IioInitPar.IioPcieAddUnCorrEn == TRUE) {
      XpUncEdMask &= ~ pMailBox->IioInitPar.IioPcieAddUnCorrBitMap;
      XpUncErrMsk &= ~pMailBox->IioInitPar.IioPcieAddUnCorrBitMap;
    }

    RAS_DEBUG((LEVEL_REG, "IioXPProgramUnCorrErrors XpUncEdMask = 0x%x!\n",XpUncEdMask));
    RAS_DEBUG((LEVEL_REG, "IioXPProgramUnCorrErrors XpUncErrMsk = 0x%x!\n",XpUncErrMsk));

    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPUNCEDMASK_IIO_PCIEDMI_REG), NULL), XpUncEdMask);
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPUNCERRMSK_IIO_PCIEDMI_REG), NULL), XpUncErrMsk);
  }
}

/**
  Program Iio XP  Sev .

  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param CorrErrMsk  - Correctable error mask

**/
VOID
EFIAPI
IioXPProgramErrSev (
  IN UINT8   Socket,
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
)
{
  UINT32 XpUncErrSev = 0;
  RAS_MAIL_BOX  *pMailBox = NULL;

  //
  // Gen4 IIO doesn't support XP errors, but ICX CPU DMI Port is GEN3
  //
  if (PcieIsDmiPort (Socket, Bus, Device, Function, NULL)) {
    RAS_DEBUG((LEVEL_REG, "IioXPProgramErrSev Socket = 0x%x,Bus = 0x%x,Device = 0x%x, Function = 0x%x!\n",Socket,Bus,Device,Function));

    pMailBox = GetRasMailBox ();
    RAS_DEBUG((LEVEL_REG, "IioXPProgramErrSev pMailBox->IioInitPar.IioPcieAddUnCorrBitMap = 0x%x!\n",pMailBox->IioInitPar.IioPcieAddUnCorrBitMap));

    if (pMailBox->IioInitPar.IioPcieAddUnCorrEn == TRUE) {
      XpUncErrSev = pMailBox->IioInitPar.IioPcieAddUnCorrSevBitMap & IIO_PCIEADD_UNCORR_MSK;
    }

    RAS_DEBUG((LEVEL_REG, "IioXPProgramErrSev XpUncErrSev = 0x%x!\n",XpUncErrSev));
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPUNCERRSEV_IIO_PCIEDMI_REG), NULL), XpUncErrSev);
  }
}

/**
    Program Iio Correctable error count .

    @param Bus       - Bus
    @param Device    - Device
    @param Function  - Function
    @param CorrErrMsk  - Correctable error mask

**/
VOID
EFIAPI
IioProgramCorrErrCnt (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  CORERRCNTMSK_IIO_PCIEGen4_STRUCT    CorErrCntrMsk;
  CORERRTH_IIO_PCIEGen4_STRUCT        CorErrThres;
  XPCORERRCOUNTER_IIO_PCIE_STRUCT     XpCorErrCntr;
  XPCORERRTHRESHOLD_IIO_PCIE_STRUCT   XpCorErrThres;
  CORERRMSK_IIO_PCIE_STRUCT           CorErrMskBitMap;

  RAS_MAIL_BOX  *pMailBox = NULL;

  pMailBox = GetRasMailBox ();

  // Enable PCIe corrected Error counter threshold and set the mask if counter is enabled
  RAS_DEBUG((LEVEL_REG, "IioProgramCorrErrCnt Bus = 0x%x,Device = 0x%x, Function = 0x%x,PcieCorErrCntr = 0x%x,PcieCorErrThres = 0x%x,PcieAerCorrErrBitMap = 0x%x!\n",Bus,Device,Function,pMailBox->PcieInitPar.PcieCorErrCntr,pMailBox->PcieInitPar.PcieCorErrThres,pMailBox->PcieInitPar.PcieAerCorrErrBitMap));

  if (PcieIsDmiPort (Socket, Bus, Device, Function, NULL)) {
    XpCorErrThres.Data =  MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPCORERRTHRESHOLD_IIO_PCIE_REG), NULL));
    XpCorErrCntr.Data =  MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPCORERRCOUNTER_IIO_PCIE_REG), NULL));
    RAS_DEBUG((LEVEL_REG, "XpCorErrThres.XPCORERRTHRESHOLD_IIO_PCIE_REG: 0x%x\n", XpCorErrThres.Data));
    RAS_DEBUG((LEVEL_REG, "XpCorErrCntr.XPCORERRCOUNTER_IIO_PCIE_REG: 0x%x\n", XpCorErrCntr.Data));

    if(pMailBox->PcieInitPar.PcieCorErrCntr == 1) {

      CorErrMskBitMap.Data = ~pMailBox->PcieInitPar.PcieAerCorrErrBitMap;

      XpCorErrCntr.Bits.receiver_error_mask = CorErrMskBitMap.Bits.receiver_error_mask;
      XpCorErrCntr.Bits.bad_tlp_mask = CorErrMskBitMap.Bits.bad_tlp_mask;
      XpCorErrCntr.Bits.bad_dllp_mask = CorErrMskBitMap.Bits.bad_dllp_mask;
      XpCorErrCntr.Bits.replay_num_rollover_mask = CorErrMskBitMap.Bits.replay_num_rollover_mask;
      XpCorErrCntr.Bits.replay_timer_timeout_mask = CorErrMskBitMap.Bits.replay_timer_time_out_mask;
      XpCorErrCntr.Bits.advisory_nonfatal_mask = CorErrMskBitMap.Bits.advisory_non_fatal_error_mask;

      XpCorErrThres.Bits.error_threshold = pMailBox->PcieInitPar.PcieCorErrThres;
      if (IsSiliconWorkaroundEnabled ("S22010729796")) {
        XpCorErrThres.Bits.error_threshold |= BIT15;
        XpCorErrCntr.Bits.error_count = BIT15;
      }
    } else {
      XpCorErrThres.Bits.error_threshold = 0;
      XpCorErrCntr.Bits.error_count = 0;
    }

    MmioWrite16 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPCORERRTHRESHOLD_IIO_PCIE_REG), NULL), XpCorErrThres.Data);
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPCORERRCOUNTER_IIO_PCIE_REG), NULL), XpCorErrCntr.Data);

    RAS_DEBUG((LEVEL_REG, "XpCorErrThres.Data: 0x%x\n", XpCorErrThres.Data));
    RAS_DEBUG((LEVEL_REG, "XpCorErrCntr.Data: 0x%x\n", XpCorErrCntr.Data));
  } else { //if (PcieIsDmiPort (Socket, Bus, Device, Function, NULL))
    CorErrThres.Data =  MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (CORERRTH_IIO_PCIEGen4_REG), NULL));
    CorErrCntrMsk.Data =  MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (CORERRCNTMSK_IIO_PCIEGen4_REG), NULL));
    RAS_DEBUG((LEVEL_REG, "CorErr.CORERRTH_IIO_PCIEGen4_REG: 0x%x\n", CorErrThres.Data));
    RAS_DEBUG((LEVEL_REG, "CorErr.CORERRCNTMSK_IIO_PCIEGen4_REG: 0x%x\n", CorErrCntrMsk.Data));

    if(pMailBox->PcieInitPar.PcieCorErrCntr == 1) {
      CorErrCntrMsk.Data  &= ~pMailBox->PcieInitPar.PcieAerCorrErrBitMap;
      CorErrThres.Bits.errth = pMailBox->PcieInitPar.PcieCorErrThres;
      //
      // PCIE GEN4 EIP generate ERR_COR both AER and error counter,
      // Using both AER and the error counter is a duplicate notification mechanism so
      // the best approach is to choose only one notification mechanism
      // mask AER once error counter is enabled to avoid system event generated on every
      // correctable error, DMI port still same as legacy error
      //
        PcieAerProgramCorrErrMsk (Socket, Bus, Device, Function, NULL, PCIEAER_CORRERR_MSK);
    } else {
      CorErrThres.Bits.errth = 0;
    }
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (CORERRTH_IIO_PCIEGen4_REG), NULL), CorErrThres.Data);
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (CORERRCNTMSK_IIO_PCIEGen4_REG), NULL), CorErrCntrMsk.Data);

    RAS_DEBUG((LEVEL_REG, "CorErrThres.Data: 0x%x\n", CorErrThres.Data));
    RAS_DEBUG((LEVEL_REG, "CorErrCntr.Data: 0x%x\n", CorErrCntrMsk.Data));
  } //if (PcieIsDmiPort (Socket, Bus, Device, Function, NULL))
}

/**
  This function is to get correctable error counter mask value

  @param[in] Socket         Device's Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    correctable error counter mask value
**/
UINT32
EFIAPI
IioGetCorrCntErrMsk (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function,
  IN      VOID    *MmInfo
  )
{
  UINT32  Data32;

  Data32 = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (CORERRCNTMSK_IIO_PCIEGen4_REG), MmInfo));
  RAS_DEBUG ((LEVEL_REG, "IioGetCorrCntErrMsk CorrErrCntMsk = 0x%x\n", Data32));
  return Data32;
}

/**
    Clear XP errors.

    @param Bus       - Bus
    @param Device    - Device
    @param Function  - Function
    @param CorrErrMsk  - Correctable error mask

**/
VOID
EFIAPI
IioXpClearErr (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  UINT32                               Data32;
  UINT16                               Data16;
  XPCORERRCOUNTER_IIO_PCIEDMI_STRUCT   XpCorErrCntr;
  CORERRCNT_IIO_PCIEGen4_STRUCT        CorErrCnt;
  //
  // Gen4 IIO doesn't support XP errors, but ICX CPU DMI Port is GEN3
  //
  if (PcieIsDmiPort (Socket, Bus, Device, Function, NULL)) {
    Data32 = MmioRead32 ( MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPCORERRSTS_IIO_PCIEDMI_REG), NULL));
    if (Data32 != 0) {
      RAS_DEBUG((LEVEL_FUNC_FLOW, "IioXpClearErr XPCORERRSTS_IIO_PCIEDMI_REG = 0x%x\n",Data32));
      MmioWrite32 ( MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPCORERRSTS_IIO_PCIEDMI_REG), NULL), Data32);
    }

    Data32 = MmioRead32 ( MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPUNCERRSTS_IIO_PCIEDMI_REG), NULL));
    if (Data32 != 0) {
      RAS_DEBUG((LEVEL_FUNC_FLOW, "IioXpClearErr XPUNCERRSTS_IIO_PCIEDMI_REG = 0x%x\n",Data32));
      MmioWrite32 ( MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPUNCERRSTS_IIO_PCIEDMI_REG), NULL), Data32);
      Data32 = MmioRead32 ( MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPUNCERRSTS_IIO_PCIEDMI_REG), NULL));
      RAS_DEBUG((LEVEL_FUNC_FLOW, "IioXpClearErr read again XPUNCERRSTS_IIO_PCIEDMI_REG = 0x%x\n",Data32));
    }

    XpCorErrCntr.Data = MmioRead32 ( MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPCORERRCOUNTER_IIO_PCIEDMI_REG), NULL));
    if (XpCorErrCntr.Bits.error_count != 0) {
      RAS_DEBUG((LEVEL_FUNC_FLOW, "IioXpClearErr XPCORERRCOUNTER_IIO_PCIEDMI_REG = 0x%x\n",XpCorErrCntr.Data));
      if (IsSiliconWorkaroundEnabled ("S22010729796")) {
        XpCorErrCntr.Bits.error_count = BIT15;
      } else {
        XpCorErrCntr.Bits.error_count = 0;
      }
      MmioWrite32 ( MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPCORERRCOUNTER_IIO_PCIEDMI_REG), NULL), XpCorErrCntr.Data);
    }

    Data16 = MmioRead16 ( MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPGLBERRSTS_IIO_PCIEDMI_REG), NULL));
    if (Data16 != 0) {
      RAS_DEBUG((LEVEL_FUNC_FLOW, "IioXpClearErr XPGLBERRSTS_IIO_PCIEDMI_REG = 0x%x\n",Data16));
      MmioWrite16 ( MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPGLBERRSTS_IIO_PCIEDMI_REG), NULL), Data16);
    }
  } else {
    CorErrCnt.Data = MmioRead32 (
      MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (CORERRCNT_IIO_PCIEGen4_REG), NULL)
      );
    if (CorErrCnt.Data != 0) {
      RAS_DEBUG((LEVEL_FUNC_FLOW, "IioXpClearErr CORERRCNT_IIO_PCIEGen4_REG = 0x%x\n", CorErrCnt.Data));
      CorErrCnt.Data = 0;
      MmioWrite32 ( MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (CORERRCNT_IIO_PCIEGen4_REG), NULL), CorErrCnt.Data);
    }

  }
}

/**
    set XP mask to disable.

    @param Socket    - Socket
    @param Bus       - Bus
    @param Device    - Device
    @param Function  - Function

**/
VOID
EFIAPI
SetXPMaskError (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  UINT32     Data32;
  //
  // Gen4 IIO doesn't support XP errors, but ICX CPU DMI Port is GEN3
  //
  if (PcieIsDmiPort (Socket, Bus, Device, Function, NULL)) {
    Data32 = IIO_XPCORERR_MASK_DATA;
    MmioWrite32 ( MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPCORERRMSK_IIO_PCIEDMI_REG), NULL), Data32);

    Data32 = IIO_XPUNCERRD_MASK_DATA;
    MmioWrite32 ( MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPUNCEDMASK_IIO_PCIEDMI_REG), NULL), Data32);
  }
}

/**
  This function is to get the XP Global Error Status registers and to see if one of the enabled errors
  has occurred.

  @param[in] Socket         PCIe root port device's socket number.
  @param[in] Bus            PCIe root port device's bus number.
  @param[in] Device         PCIe root port device's device number.
  @param[in] Function       PCIe root port device's function number.
  @param[out] Xpcorsts      Xp correctable error status.
  @param[out] Xpuncsts      Xp uncorrectable error status.
  @param[out] xpglbsts      xp global error status.
  @retval    BOOLEAN        TRUE   - An error was detected.
                            FALSE  - An error was not detected, or the device is not a root port.
--*/
BOOLEAN
IioGetXpErrorStatus (
  IN  UINT8    Socket,
  IN  UINT8    Bus,
  IN  UINT8    Device,
  IN  UINT8    Function,
  OUT UINT32   *Xpcorsts,
  OUT UINT32   *Xpuncsts,
  OUT UINT32   *Xpglbsts
  )
 {

  UINT32     XpCorErrSts = 0;
  UINT32     XpUncErrSts = 0;
  UINT32     XpGlbErrSts = 0;
  BOOLEAN    IsError;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioGetXpErrorStatus  Socket = 0x%x,Bus = %x, Dev = %x, Func = %x, \n", Socket,Bus,Device, Function));

  IsError = FALSE;

  //
  // Gen4 IIO doesn't support XP errors, but ICX CPU DMI Port is GEN3
  //
  if (PcieIsDmiPort (Socket, Bus, Device, Function, NULL)) {
    //
    // Determine which errors are detected
    //
    XpGlbErrSts = MmioRead32 (MmPciAddressExt(Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPGLBERRSTS_IIO_PCIE_REG), NULL));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioGetXpErrorStatus XpGlbErrSts = %x, \n", XpGlbErrSts));

    XpCorErrSts = MmioRead32 (MmPciAddressExt(Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPCORERRSTS_IIO_PCIE_REG), NULL));
    RAS_DEBUG((LEVEL_FUNC_FLOW,"IioGetXpErrorStatus XPCORERRSTS = %x ", XpCorErrSts));

    XpUncErrSts = MmioRead32 (MmPciAddressExt(Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (XPUNCERRSTS_IIO_PCIE_REG), NULL));
    RAS_DEBUG((LEVEL_FUNC_FLOW,"IioGetXpErrorStatus XpUncErrSts = %x ", XpUncErrSts));

    if (XpGlbErrSts !=0 || XpCorErrSts !=0 || XpUncErrSts !=0) {
      IsError = TRUE;
    }

  }

  *Xpcorsts = XpCorErrSts;
  *Xpuncsts = XpUncErrSts;
  *Xpglbsts = XpGlbErrSts;

  return IsError;
 }