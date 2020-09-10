/** @file
  Implementation of UBOX related interface

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

#include <RcRegs.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/RasDebugLib.h>

//
// IMC GPSB Ports
//
#define SIDE_BAND_ID_IMC0  966
#define SIDE_BAND_ID_IMC1  967
#define SIDE_BAND_ID_IMC2  968
#define SIDE_BAND_ID_IMC3  969

//
// M2MEM Ports
//
#define SIDE_BAND_ID_MS2MEM0  958
#define SIDE_BAND_ID_MS2MEM1  959
#define SIDE_BAND_ID_MS2MEM2  960
#define SIDE_BAND_ID_MS2MEM3  961


//
// Smbus GPSB Port
//
#define SIDE_BAND_ID_SMBUS0  466
#define SIDE_BAND_ID_SMBUS1  467

#define CSMI_THRESHOLD_REG_NUMBER 28
#define MCA_UBOX_ERROR_MASK       0x3fffff
//
// Register offsets created as arrays for CSMI threshold
//
UINT32 mCsmiThresholdCfg[CSMI_THRESHOLD_REG_NUMBER] = {
  UBOXCSMITHRES4_UBOX_CFG_REG, UBOXCSMITHRES5_UBOX_CFG_REG, UBOXCSMITHRES6_UBOX_CFG_REG, UBOXCSMITHRES7_UBOX_CFG_REG,
  UBOXCSMITHRES8_UBOX_CFG_REG, UBOXCSMITHRES9_UBOX_CFG_REG, UBOXCSMITHRES10_UBOX_CFG_REG, UBOXCSMITHRES11_UBOX_CFG_REG,
  UBOXCSMITHRES12_UBOX_CFG_REG, UBOXCSMITHRES13_UBOX_CFG_REG, UBOXCSMITHRES14_UBOX_CFG_REG, UBOXCSMITHRES15_UBOX_CFG_REG,
  UBOXCSMITHRES16_UBOX_CFG_REG, UBOXCSMITHRES17_UBOX_CFG_REG, UBOXCSMITHRES18_UBOX_CFG_REG, UBOXCSMITHRES19_UBOX_CFG_REG,
  UBOXCSMITHRES20_UBOX_CFG_REG, UBOXCSMITHRES21_UBOX_CFG_REG, UBOXCSMITHRES22_UBOX_CFG_REG, UBOXCSMITHRES23_UBOX_CFG_REG,
  UBOXCSMITHRES24_UBOX_CFG_REG, UBOXCSMITHRES25_UBOX_CFG_REG, UBOXCSMITHRES26_UBOX_CFG_REG, UBOXCSMITHRES27_UBOX_CFG_REG,
  UBOXCSMITHRES28_UBOX_CFG_REG, UBOXCSMITHRES29_UBOX_CFG_REG, UBOXCSMITHRES30_UBOX_CFG_REG, UBOXCSMITHRES31_UBOX_CFG_REG
};

UINT8     mSempOwned[MAX_SOCKET];
UINT32    mSempCurrTail[MAX_SOCKET];

/**
  check if iMC has SMI error interrupt

  @param  Skt -- socket index

  @return  TRUE -- error happens

**/
BOOLEAN
EFIAPI
IsUboxImcError (
  IN  UINT8   Skt
  )
{
  NCEVENTS_CR_SMISRCLOG_UBOX_CFG_STRUCT UboxSmiSrcLogReg;
  UboxSmiSrcLogReg.Data = ReadCpuCsr (Skt, 0, NCEVENTS_CR_SMISRCLOG_UBOX_CFG_REG);

  //
  // Check Imc Src is valid using GPSB
  //
  if (UboxSmiSrcLogReg.Bits.sidebandvalid == 1) {
    if (UboxSmiSrcLogReg.Bits.sidebandportid >= SIDE_BAND_ID_IMC0 &&
        UboxSmiSrcLogReg.Bits.sidebandportid <= SIDE_BAND_ID_IMC3) {
        return TRUE;
      }
  }

  return FALSE;
}

/**
  Check if M2MEM has SMI error interrupt

  @param  Skt -- socket index

  @return  TRUE -- error happens

**/
BOOLEAN
EFIAPI
IsUboxM2MemError (
  IN  UINT8   Skt
  )
{
  NCEVENTS_CR_SMISRCLOG_UBOX_CFG_STRUCT UboxSmiSrcLogReg;
  UboxSmiSrcLogReg.Data = ReadCpuCsr (Skt, 0, NCEVENTS_CR_SMISRCLOG_UBOX_CFG_REG);

  //
  // Check source is from mesh 2 mem for mirror failover event
  //
  if (UboxSmiSrcLogReg.Bits.sidebandvalid == 1) {
    if (UboxSmiSrcLogReg.Bits.sidebandportid >= SIDE_BAND_ID_MS2MEM0 &&
      UboxSmiSrcLogReg.Bits.sidebandportid <= SIDE_BAND_ID_MS2MEM3) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Check if Smbus has SMI error interrupt

  @param  Skt -- socket index

  @return  FALSE -- SPR I3C BUS reports error through IEH, The I3C Error handling is in IEH Error handling path.

**/
BOOLEAN
EFIAPI
IsUboxSmbusError (
  IN  UINT8   Skt
  )
{
  //
  // SPR I3C BUS reports error through IEH, The I3C Error handling is in IEH Error handling path.
  //
  return FALSE;
}

/**
  clear umc in UBOX

  @param  Socket -- socket index

  @return none

**/
VOID
EFIAPI
ClearUboxUmcError (
  IN   UINT8   Socket
  )
{

}


/**
  Enable SMI for memory domain in UBOX

  @param  socket -- socket index

  @return  none.
**/
VOID
EFIAPI
EnableUboxSmiSrc (
  IN   UINT8   Socket
  )
{
  UBOX_SMICTRL_UBOX_MISC_STRUCT                UboxSmiCtrlReg;
  UboxSmiCtrlReg.Data = ReadCpuCsr (Socket, 0, UBOX_SMICTRL_UBOX_MISC_REG);

  //
  //Disable any SMI for Ubox errors such as LockTimeout, Cfg Rd/WrMis-aligned Errors
  //
  UboxSmiCtrlReg.Bits.smidis = 1;
  UboxSmiCtrlReg.Bits.smidis2 = 1;

  //
  // Enable the Generation of SMI from message channel
  //
  UboxSmiCtrlReg.Bits.smidis3 = 0;
  UboxSmiCtrlReg.Bits.smidis4 = 0;

   WriteCpuCsr (Socket, 0, UBOX_SMICTRL_UBOX_MISC_REG, UboxSmiCtrlReg.Data);
}


/**
  Enable Ubox Error ctrol register

  @param  Socket  -- socket index

  @return none
**/
VOID
EFIAPI
EnableUboxErrCtrl (
  IN   UINT8   Socket
  )
{
  UBOXERRCTL_UBOX_CFG_STRUCT        UboxErrCtrl;

  //
  //Enable MCA Signalling for Ubox errors
  //
  UboxErrCtrl.Data = ReadCpuCsr (Socket, 0, UBOXERRCTL_UBOX_CFG_REG);
  UboxErrCtrl.Bits.uboxerrortopcumask = 0;
  WriteCpuCsr(Socket, 0, UBOXERRCTL_UBOX_CFG_REG, UboxErrCtrl.Data);
}


/**
  clear imc controller interrupt status in UBOX

  @param  Socket  -- socket index

  @return  none
**/
VOID
EFIAPI
ClearImcInterruptInUbox (
  IN   UINT8     Socket
  )
{

  NCEVENTS_CR_UBOXERRSTS_UBOX_CFG_STRUCT  UboxErrStsReg;

  UBOXERRMISC_UBOX_CFG_STRUCT             UboxErrMisc;
  UBOXERRMISC2_UBOX_CFG_STRUCT            UboxErrMisc2;

  UboxErrStsReg.Data      = ReadCpuCsr (Socket, 0, NCEVENTS_CR_UBOXERRSTS_UBOX_CFG_REG);
  UboxErrStsReg.Bits.poisonrsvd        = 0;
  UboxErrStsReg.Bits.unsupportedopcode = 0;
  WriteCpuCsr (Socket, 0, NCEVENTS_CR_UBOXERRSTS_UBOX_CFG_REG, UboxErrStsReg.Data);

  UboxErrMisc.Data = 0;
  WriteCpuCsr (Socket, 0, UBOXERRMISC_UBOX_CFG_REG, UboxErrMisc.Data);

  UboxErrMisc2.Data = 0;
  WriteCpuCsr (Socket, 0, UBOXERRMISC2_UBOX_CFG_REG, UboxErrMisc2.Data);
}


/**
  Enable Viral in  UBOX.

  @param[in] Skt      - The socket to enable

  @retval None

**/
VOID
EnableUBoxViralPerSkt (
  IN        UINT8   Skt
  )
{
  UBOX_GL_ERR_CFG_UBOX_CFG_STRUCT   UboxglerrcfgReg;

  //
  // Enable Viral in ubox
  //
  UboxglerrcfgReg.Data = ReadCpuCsr (Skt, 0, UBOX_GL_ERR_CFG_UBOX_CFG_REG);
  UboxglerrcfgReg.Bits.resetuboxfatalerror = 0;
  UboxglerrcfgReg.Bits.maskuboxumc  = 0;
  UboxglerrcfgReg.Bits.maskuboxierr = 0;
  UboxglerrcfgReg.Bits.maskpcuproxyierr = 3;
  UboxglerrcfgReg.Bits.maskpcuproxymsmi = 3;
  UboxglerrcfgReg.Bits.maskuboxmsmi = 0;
  UboxglerrcfgReg.Bits.maskieherr2 = 0;
  WriteCpuCsr (Skt, 0, UBOX_GL_ERR_CFG_UBOX_CFG_REG, UboxglerrcfgReg.Data);
}


/**
  check viral status in UBOX

  @param  Socket -- socket index

  @return none.
**/
BOOLEAN
EFIAPI
IsUboxViral (
  IN  UINT8   Socket
  )
{
  NCEVENTS_CR_UBOXERRSTS_UBOX_CFG_STRUCT    UboxErrStsReg;
  UboxErrStsReg.Data = ReadCpuCsr (Socket, 0, NCEVENTS_CR_UBOXERRSTS_UBOX_CFG_REG);

  if (UboxErrStsReg.Bits.unsupportedopcode == 1) {
    return TRUE;
  } else {
    return FALSE;
  }
}


/**

  This function will release the semaphore after the recovery process is done.

  @param [in]  SktId     SktId to release semaphore.

  @retval  None

**/
VOID
EFIAPI
ReleaseSemaphoreSmbus (
  IN  UINT8    Skt
)
{


  //
  // Release Semaphore -  Write Acquired_Tail to Sema_Release
  //
  WriteCpuCsr (Skt, 0, SYSTEMRELEASESEMP_0_UBOX_MISC_REG, mSempCurrTail[Skt]);
}

/**
  aquire SMBUS controller ownership

  @param  skt -- socket index

  @return none

**/
VOID
EFIAPI
AquireSemaphore (
  IN  UINT8   Skt
  )
{
  SYSTEMIMPAQUSEMP_0_UBOX_MISC_STRUCT       RegAddrImpAqu;

  RegAddrImpAqu.Data = ReadCpuCsr (Skt, 0, SYSTEMIMPAQUSEMP_0_UBOX_MISC_REG);

  mSempOwned[Skt] = (UINT8)RegAddrImpAqu.Bits.sempowned;
  mSempCurrTail[Skt] = RegAddrImpAqu.Bits.sempcurrtail;


}

/**
  Override SMBUS controller ownership

  @param  skt -- socket index

  @return none

**/
VOID
EFIAPI
OverrideSemaphore (
  IN  UINT8   Skt
  )
{

  //
  // In 10nm processors, there is no BIOS defined way to override the semaphore.
  // So, call the AcqSemaphore.
  //
  AquireSemaphore (Skt);


}

/**
  check if aquired SMBUS controller ownership.

  @param  skt -- socket index

  @return TRUE -- ownership aquired;

**/
BOOLEAN
EFIAPI
IsAquireSemaphored (
  IN  UINT8   Skt
  )
{

  SYSTEMHEADSEMP_0_UBOX_MISC_STRUCT         RegAddrHead;
  SYSTEMIMPAQUSEMP_0_UBOX_MISC_STRUCT       RegAddrImpAqu;

  RegAddrHead.Data   = ReadCpuCsr (Skt, 0, SYSTEMHEADSEMP_0_UBOX_MISC_REG);

  //
  // If !SEMx_ACQUIRE.Owned, poll SEMx_HEAD until SEMx_HEAD.Head == SEMx_ACQUIRE.Tail
  //
  if (mSempOwned[Skt] != 1) {
    RegAddrImpAqu.Data = ReadCpuCsr(Skt, 0, SYSTEMIMPAQUSEMP_0_UBOX_MISC_REG);
    mSempOwned[Skt] = (UINT8)RegAddrImpAqu.Bits.sempowned;
    mSempCurrTail[Skt] = (UINT32)RegAddrImpAqu.Bits.sempcurrtail;
  }

  if (mSempCurrTail[Skt] == RegAddrHead.Bits.sempcurrhead) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "Semaphore is acquired on SktId: 0x%x\n", Skt));
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  This function clears sticky error log register.

**/
VOID
EFIAPI
ClearStickyErrLogReg (
  VOID
  )
{
  UINT8                           Socket;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (IsSocketPresent (Socket) == FALSE) {
      continue;
    }

#ifdef GNRSRF_HOST
    WriteCpuCsr (Socket, 0, MCERRLOGGINGREG_N0_UBOX_CFG_REG, 0);
    RAS_DEBUG ((LEVEL_REG, "[CpuRas]Clear MCERR logging status, CSR(S 0x%x I0 R 0x%x) = 0\n", Socket, MCERRLOGGINGREG_N0_UBOX_CFG_REG));

    WriteCpuCsr (Socket, 0, MCERRLOGGINGREG_N1_UBOX_CFG_REG, 0);
    RAS_DEBUG ((LEVEL_REG, "[CpuRas]Clear MCERR logging status, CSR(S 0x%x I0 R 0x%x) = 0\n", Socket, MCERRLOGGINGREG_N1_UBOX_CFG_REG));

    WriteCpuCsr (Socket, 0, IERRLOGGINGREG_N0_UBOX_CFG_REG, 0);
    RAS_DEBUG ((LEVEL_REG, "[CpuRas]Clear IERR logging status, CSR(S 0x%x I0 R 0x%x) = 0\n", Socket, IERRLOGGINGREG_N0_UBOX_CFG_REG));

    WriteCpuCsr (Socket, 0, IERRLOGGINGREG_N1_UBOX_CFG_REG, 0);
    RAS_DEBUG ((LEVEL_REG, "[CpuRas]Clear IERR logging status, CSR(S 0x%x I0 R 0x%x) = 0\n", Socket, IERRLOGGINGREG_N1_UBOX_CFG_REG));
#else // GNRSRF_HOST
    WriteCpuCsr (Socket, 0, MCERRLOGGINGREG_UBOX_CFG_REG, 0);
    RAS_DEBUG ((LEVEL_REG, "[CpuRas]Clear MCERR logging status, CSR(S 0x%x I0 R 0x%x) = 0\n", Socket, MCERRLOGGINGREG_UBOX_CFG_REG));

    WriteCpuCsr (Socket, 0, IERRLOGGINGREG_UBOX_CFG_REG, 0);
    RAS_DEBUG ((LEVEL_REG, "[CpuRas]Clear IERR logging status, CSR(S 0x%x I0 R 0x%x) = 0\n", Socket, IERRLOGGINGREG_UBOX_CFG_REG));
#endif // GNRSRF_HOST

    WriteCpuCsr (Socket, 0, NCEVENTS_CR_SMISRCLOG_UBOX_CFG_REG, 0);
    RAS_DEBUG ((LEVEL_REG, "[CpuRas]Clear SMI logging status, CSR(S 0x%x I0 R 0x%x) = 0\n", Socket, NCEVENTS_CR_SMISRCLOG_UBOX_CFG_REG));

    WriteCpuCsr (Socket, 0, NCEVENTS_CR_UBOXERRSTS_UBOX_CFG_REG, 0);
    RAS_DEBUG ((LEVEL_REG, "[CpuRas]Clear UBOX error status register, CSR(S 0x%x I0 R 0x%x) = 0\n", Socket, NCEVENTS_CR_UBOXERRSTS_UBOX_CFG_REG));

    WriteCpuCsr (Socket, 0, NCEVENTS_CR_UBOXERRSTS2_UBOX_CFG_REG, 0);
    RAS_DEBUG ((LEVEL_REG, "[CpuRas]Clear UBOX error status 2 register, CSR(S 0x%x I0 R 0x%x) = 0\n", Socket, NCEVENTS_CR_UBOXERRSTS2_UBOX_CFG_REG));
  }
}

/**
  Set UBOX CSMI threshold

  @param[in]  CsmiThreshold    UBOX CSMI threshold setting
  @param[in]  CloakingEn       Indicates whether cloaking is enabled or not

**/
VOID
EFIAPI
SetUboxCsmiThreshold (
  IN UINT16    CsmiThreshold,
  IN BOOLEAN   CloakingEn
  )
{
  UINT8                          Socket;
  UINT8                          Index;
  UBOXCSMITHRES4_UBOX_CFG_STRUCT UboxCsmiThresCfg;

  UboxCsmiThresCfg.Data                   = 0;
  UboxCsmiThresCfg.Bits.bios_ce_threshold = CsmiThreshold;
  if (CloakingEn) {
    UboxCsmiThresCfg.Bits.match_en          = 1;
  } else {
    UboxCsmiThresCfg.Bits.match_en          = CsmiThreshold == 0 ? 0 : 1;
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (IsSocketPresent (Socket) == FALSE) {
      continue;
    }
    for (Index = 0; Index < CSMI_THRESHOLD_REG_NUMBER; Index++) {
      WriteCpuCsr (Socket, 0, mCsmiThresholdCfg[Index], UboxCsmiThresCfg.Data);
    }
  }
}

/**
  This function configurate Ubox Error Mask.

  @param[in] UboxErrorMask  Ubox Error Mask in Ubox.

**/
VOID
ConfigUboxErrorMask (
  IN UINT8 UboxErrorMask
  )
{
  UINT8    Skt;
  NCEVENTS_CR_UBOXERRCTL2_UBOX_CFG_STRUCT  UboxErrCtl2;

  for (Skt = 0; Skt < MAX_SOCKET; Skt ++) {
    if (!IsSocketPresent (Skt)) {
      return;
    }
    UboxErrCtl2.Data = ReadCpuCsr (Skt, 0, NCEVENTS_CR_UBOXERRCTL2_UBOX_CFG_REG);
    if (UboxErrorMask == 0) {
      UboxErrCtl2.Data &= ~ (UINT32) MCA_UBOX_ERROR_MASK;
    } else {
      UboxErrCtl2.Data |= MCA_UBOX_ERROR_MASK;
    }
    WriteCpuCsr (Skt, 0, NCEVENTS_CR_UBOXERRCTL2_UBOX_CFG_REG, UboxErrCtl2.Data);
  }
}