/** @file
  Implementation of iio root port silicon interface.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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
#include <RcRegs.h>
#include <Library/RasDebugLib.h>
#include <Library/RasMailBoxLib.h>
#include <Library/PlatPolicyLib.h>
#include <Library/MpExtensionLib.h>
#include <Library/MmPciExtLib.h>
#include <Library/IoLib.h>
#include <Register/IioRegDef.h>
#include <Library/PcieStdLib.h>
#include <Library/PcieRasLib.h>

/**
  Disable IIO MSI error.

  @param[in]  Socket    Socket index.
  @param[in]  Bus       PCI bus number.
  @param[in]  Device    PCI device number.
  @param[in]  Function  PCI function number.
**/
VOID
EFIAPI
IioDisMsiError (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
}


/**
  Program IIO errors (Correctable, Uncorrectable, Fatal).

  @param[in]  Socket    Socket index.
  @param[in]  Bus       PCI bus number.
  @param[in]  Device    PCI device number.
  @param[in]  Function  PCI function number.
**/
VOID
EFIAPI
IioProgramErrors (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  IIOMISCCTRL_N1_IIO_PCIE_STRUCT IioMiscCtrlN1;
  RAS_MAIL_BOX                   *pMailBox = NULL;

  RAS_DEBUG((LEVEL_FUNC_FLOW,"IioProgramErrors."));
  pMailBox = GetRasMailBox ();

  if (pMailBox->IioInitPar.IioPcieAerSpecCompEn) {
    //
    // enable poismsg spec behavior
    //
    IioMiscCtrlN1.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (IIOMISCCTRL_N1_IIO_VTD_REG), NULL));
    IioMiscCtrlN1.Bits.en_poismsg_spec_behavior = 1;
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (IIOMISCCTRL_N1_IIO_PCIE_REG), NULL), IioMiscCtrlN1.Data);
  }
}

/**
  This function will enable all the RP PIO errors.

  @param[in]  Socket    Socket index.
  @param[in]  Bus       PCI bus number.
  @param[in]  Device    PCI device number.
  @param[in]  Function  PCI function number.
**/
VOID
EFIAPI
RppIoErrEnable (
  IN       UINT8                  Socket,
  IN       UINT8                  Bus,
  IN       UINT8                  Device,
  IN       UINT8                  Function
  )
{
}

/**
  Program Leaky Bucket registers.

  The BDF provided is assumed to be an IIO PCIe root port. This function
  should not be called for any other device.

  @param[in]  Socket    Socket index.
  @param[in]  Bus       PCI bus number.
  @param[in]  Device    PCI device number.
  @param[in]  Function  PCI function number.
**/
VOID
EFIAPI
IioProgramLeakyBucket (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  RAS_MAIL_BOX                  *MailBox;
  UINTN                         LekBErrAddress;
  UINTN                         LekBErr0Address;
  UINTN                         LekBErr1Address;
  UINTN                         LekBArAddress;
  LEKBERR_IIO_PCIEGen4_STRUCT   LeakyBucketError;
  LEKBERR0_IIO_PCIEGen4_STRUCT  LeakyBucketTimeWindow0;
  LEKBERR1_IIO_PCIEGen4_STRUCT  LeakyBucketTimeWindow1;
  LEKBAR_IIO_PCIEGen4_STRUCT    LeakyBucketActions;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioProgramLeakyBucket: Socket %d BDF 0x%x:0x%x:0x%x\n", Socket, Bus, Device, Function));

  MailBox = GetRasMailBox ();
  ASSERT (MailBox != NULL);
  if (MailBox == NULL) {
    return;
  }

  LekBErrAddress  = MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (LEKBERR_IIO_PCIEGen4_REG), NULL);
  LekBErr0Address = MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (LEKBERR0_IIO_PCIEGen4_REG), NULL);
  LekBErr1Address = MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (LEKBERR1_IIO_PCIEGen4_REG), NULL);
  LekBArAddress   = MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (LEKBAR_IIO_PCIEGen4_REG), NULL);

  LeakyBucketError.Data       = MmioRead32 (LekBErrAddress);
  LeakyBucketTimeWindow0.Data = MmioRead32 (LekBErr0Address);
  LeakyBucketTimeWindow1.Data = MmioRead32 (LekBErr1Address);
  LeakyBucketActions.Data     = MmioRead32 (LekBArAddress);

  LeakyBucketTimeWindow0.Bits.exp_ber    = (UINT32) MailBox->PcieInitPar.ExpectedBer;
  LeakyBucketTimeWindow1.Bits.exp_ber    = (UINT32) RShiftU64 (MailBox->PcieInitPar.ExpectedBer, 32);
  LeakyBucketError.Bits.aggrerr          = MailBox->PcieInitPar.Gen12TimeWindow;
  LeakyBucketTimeWindow1.Bits.g3aggrerr  = MailBox->PcieInitPar.Gen34TimeWindow;
  LeakyBucketError.Bits.errthresh        = MailBox->PcieInitPar.Gen12ErrorThreshold;
  LeakyBucketError.Bits.g3errthresh      = MailBox->PcieInitPar.Gen34ErrorThreshold;
  LeakyBucketError.Bits.g3lbeen          = MailBox->PcieInitPar.Gen34ReEqualization;
  LeakyBucketActions.Bits.lbeg2degradeen = MailBox->PcieInitPar.Gen2LinkDegradation;
  LeakyBucketActions.Bits.lbeg3degradeen = MailBox->PcieInitPar.Gen3LinkDegradation;
  LeakyBucketActions.Bits.lbeg4degradeen = MailBox->PcieInitPar.Gen4LinkDegradation;

  RAS_DEBUG ((LEVEL_REG, "Setting BDF 0x%x:0x%x:0x%x leaky bucket registers:\n", Bus, Device, Function));
  RAS_DEBUG ((LEVEL_REG, "  LEKBERR  = 0x%08x\n", LeakyBucketError.Data));
  RAS_DEBUG ((LEVEL_REG, "  LEKBERR0 = 0x%08x\n", LeakyBucketTimeWindow0.Data));
  RAS_DEBUG ((LEVEL_REG, "  LEKBERR1 = 0x%08x\n", LeakyBucketTimeWindow1.Data));
  RAS_DEBUG ((LEVEL_REG, "  LEKBAR   = 0x%08x\n", LeakyBucketActions.Data));

  MmioWrite32 (LekBErrAddress,  LeakyBucketError.Data);
  MmioWrite32 (LekBErr0Address, LeakyBucketTimeWindow0.Data);
  MmioWrite32 (LekBErr1Address, LeakyBucketTimeWindow1.Data);
  MmioWrite32 (LekBArAddress,   LeakyBucketActions.Data);
}

/**
  Get leacky bucket lane error mask.

  Return the error status mask of one lane per bit. If at least one lane has
  exceeded the threshold (status bit is set), get the status for
  re-equalization and degradation, and the error counts for each lane in the
  output parameters.

  Note that if the lane error mask is 0, the output parameters are not updated
  and should be ignored.

  @param[in]  Socket            Socket index.
  @param[in]  Bus               PCI bus number.
  @param[in]  Device            PCI device number.
  @param[in]  Function          PCI function number.
  @param[out] LaneErrorCount    Array of error counts for each lane.
  @param[out] Gen34ReEqualize   TRUE if Gen3/4 re-equalization was requested.
  @param[out] Gen2LinkDegrade   TRUE if Gen2 link degradation occurred.
  @param[out] Gen3LinkDegrade   TRUE if Gen3 link degradation occurred.
  @param[out] Gen4LinkDegrade   TRUE if Gen4 link degradation occurred.

  @return     The bitmask of lanes that were triggered.
**/
UINT16
EFIAPI
GetLeakyBucketLaneErrorMask (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function,
  OUT     UINT8   LaneErrorCount[16],
  OUT     BOOLEAN *Gen34ReEqualize,
  OUT     BOOLEAN *Gen2LinkDegrade,
  OUT     BOOLEAN *Gen3LinkDegrade,
  OUT     BOOLEAN *Gen4LinkDegrade
  )
{
  UINT16                            LaneErrorMask;
  UINTN                             LekBProErrAddress;
  UINTN                             LekBLnErrCnt0Address;
  UINTN                             LekBLnErrCnt1Address;
  UINTN                             LekBLnErrCnt2Address;
  UINTN                             LekBArAddress;
  UINTN                             LinkSts2Address;
  UINTN                             G4StsAddress;
  LEKBPROERR_IIO_PCIEGen4_STRUCT    ProlongedError;
  LEKBLNERRCNT0_IIO_PCIEGen4_STRUCT LaneErrorCount0;
  LEKBLNERRCNT1_IIO_PCIEGen4_STRUCT LaneErrorCount1;
  LEKBLNERRCNT2_IIO_PCIEGen4_STRUCT LaneErrorCount2;
  LEKBAR_IIO_PCIEGen4_STRUCT        LeakyBucketActions;
  LINKSTS2_IIO_PCIEGen4_STRUCT      LinkStatus2;
  G4STS_IIO_PCIEGen4_STRUCT         Gen4Status;

  LekBProErrAddress    = MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (LEKBPROERR_IIO_PCIEGen4_REG), NULL);
  LekBLnErrCnt0Address = MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (LEKBLNERRCNT0_IIO_PCIEGen4_REG), NULL);
  LekBLnErrCnt1Address = MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (LEKBLNERRCNT1_IIO_PCIEGen4_REG), NULL);
  LekBLnErrCnt2Address = MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (LEKBLNERRCNT2_IIO_PCIEGen4_REG), NULL);
  LekBArAddress        = MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (LEKBAR_IIO_PCIEGen4_REG), NULL);
  LinkSts2Address      = MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (LINKSTS2_IIO_PCIEGen4_REG), NULL);
  G4StsAddress         = MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (G4STS_IIO_PCIEGen4_REG), NULL);

  ProlongedError.Data = MmioRead32 (LekBProErrAddress);
  LaneErrorMask = (UINT16) (ProlongedError.Bits.errlnsts | ProlongedError.Bits.g3errlnsts);

  if (LaneErrorMask != 0) {
    LaneErrorCount0.Data    = MmioRead32 (LekBLnErrCnt0Address);
    LaneErrorCount1.Data    = MmioRead32 (LekBLnErrCnt1Address);
    LaneErrorCount2.Data    = MmioRead32 (LekBLnErrCnt2Address);
    LaneErrorCount2.Data    = MmioRead32 (LekBLnErrCnt2Address);
    LeakyBucketActions.Data = MmioRead32 (LekBArAddress);
    LinkStatus2.Data        = MmioRead16 (LinkSts2Address);
    Gen4Status.Data         = MmioRead32 (G4StsAddress);

    RAS_DEBUG ((LEVEL_REG, "Got BDF 0x%x:0x%x:0x%x leaky bucket registers:\n", Bus, Device, Function));
    RAS_DEBUG ((LEVEL_REG, "  LEKBPROERR    = 0x%08x\n", ProlongedError.Data));
    RAS_DEBUG ((LEVEL_REG, "  LEKBLNERRCNT0 = 0x%08x\n", LaneErrorCount0.Data));
    RAS_DEBUG ((LEVEL_REG, "  LEKBLNERRCNT1 = 0x%08x\n", LaneErrorCount1.Data));
    RAS_DEBUG ((LEVEL_REG, "  LEKBLNERRCNT2 = 0x%08x\n", LaneErrorCount2.Data));
    RAS_DEBUG ((LEVEL_REG, "  LEKBAR        = 0x%08x\n", LeakyBucketActions.Data));
    RAS_DEBUG ((LEVEL_REG, "  LINKSTS2      = 0x%08x\n", LinkStatus2.Data));
    RAS_DEBUG ((LEVEL_REG, "  G4STS         = 0x%08x\n", Gen4Status.Data));

    *Gen34ReEqualize = (LinkStatus2.Bits.lnkeqreq == 1 || Gen4Status.Bits.linkeqreq16 == 1);
    *Gen2LinkDegrade = (LeakyBucketActions.Bits.lbeg2dis == 1);
    *Gen3LinkDegrade = (LeakyBucketActions.Bits.lbeg3dis == 1);
    *Gen4LinkDegrade = (LeakyBucketActions.Bits.lbeg4dis == 1);

    LaneErrorCount[0]  = (UINT8) LaneErrorCount0.Bits.errcnt0;
    LaneErrorCount[1]  = (UINT8) LaneErrorCount0.Bits.errcnt1;
    LaneErrorCount[2]  = (UINT8) LaneErrorCount0.Bits.errcnt2;
    LaneErrorCount[3]  = (UINT8) LaneErrorCount0.Bits.errcnt3;
    LaneErrorCount[4]  = (UINT8) LaneErrorCount0.Bits.errcnt4;
    LaneErrorCount[5]  = (UINT8) LaneErrorCount0.Bits.errcnt5;
    LaneErrorCount[6]  = (UINT8) LaneErrorCount1.Bits.errcnt6;
    LaneErrorCount[7]  = (UINT8) LaneErrorCount1.Bits.errcnt7;
    LaneErrorCount[8]  = (UINT8) LaneErrorCount1.Bits.errcnt8;
    LaneErrorCount[9]  = (UINT8) LaneErrorCount1.Bits.errcnt9;
    LaneErrorCount[10] = (UINT8) LaneErrorCount1.Bits.errcnt10;
    LaneErrorCount[11] = (UINT8) LaneErrorCount1.Bits.errcnt11;
    LaneErrorCount[12] = (UINT8) LaneErrorCount2.Bits.errcnt12;
    LaneErrorCount[13] = (UINT8) LaneErrorCount2.Bits.errcnt13;
    LaneErrorCount[14] = (UINT8) LaneErrorCount2.Bits.errcnt14;
    LaneErrorCount[15] = (UINT8) LaneErrorCount2.Bits.errcnt15;
  }

  return LaneErrorMask;
}

/**
  Clear leacky bucket status.

  Clear the leaky bucket error status for each lane.

  @param[in]  Socket    Socket index.
  @param[in]  Bus       PCI bus number.
  @param[in]  Device    PCI device number.
  @param[in]  Function  PCI function number.
**/
VOID
EFIAPI
ClearLeakyBucketStatus (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  UINTN                             LekBProErrAddress;
  LEKBPROERR_IIO_PCIEGen4_STRUCT    ProlongedError;

  LekBProErrAddress = MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (LEKBPROERR_IIO_PCIEGen4_REG), NULL);
  ProlongedError.Data = MmioRead32 (LekBProErrAddress);
  MmioWrite32 (LekBProErrAddress, ProlongedError.Data); // write 1 to clear
}

/**
  Dump IOH PCIe registers.

  Dump the IOH PCIe registers for a given BDF on a given socket.

  @param[in]  Socket    Socket index.
  @param[in]  Bus       PCI bus number.
  @param[in]  Device    PCI device number.
  @param[in]  Function  PCI function number.
**/
VOID
EFIAPI
DumpIohPcieRegisters (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  ERRCORMSK_IIO_PCIEGen4_STRUCT         CorErrMsk;
  ERRUNCMSK_IIO_PCIEGen4_STRUCT         UncErrMsk;
  CORERRCNT_IIO_PCIEGen4_STRUCT         CorErrCnt;

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "DumpIohPcieRegisters for Socket=0x%x, Bus=0x%x, Device=0x%x, Function=0x%x:\n",
    Socket,
    Bus,
    Device,
    Function
    ));

  if (!PcieIsDevicePresent (Socket, Bus, Device, Function, NULL)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "  NOT PRESENT!\n"));
  } else {
    CorErrMsk.Data = MmioRead32 (
      MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (ERRCORMSK_IIO_PCIEGen4_REG), NULL)
      );
    UncErrMsk.Data = MmioRead32 (
      MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (ERRUNCMSK_IIO_PCIEGen4_REG), NULL)
      );
    CorErrCnt.Data = MmioRead32 (
      MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (CORERRCNT_IIO_PCIEGen4_REG), NULL)
      );

    RAS_DEBUG ((LEVEL_FUNC_FLOW, "  ERRCORMSK_IIO_PCIEGen4_REG:      0x%08x\n", CorErrMsk.Data));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "  ERRUNCMSK_IIO_PCIEGen4_REG:      0x%08x\n", UncErrMsk.Data));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "  CORERRCNT_IIO_PCIEGen4_REG:      0x%08x\n", CorErrCnt.Data));
  }
}

/**
  This function checks if a Silicon specific PCI or PCIe device exist on a specificed address.

  @param[in] Socket         Device's Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    BOOLEAN        True   Device is a Silicon specific device and it is exist.
                            False  Device is not a silicon specific device or it is not exist.
**/
BOOLEAN
EFIAPI
IsSiSpecPcieDeviceExist (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function,
  IN      VOID    *MmInfo
  )
{
  return FALSE;
}