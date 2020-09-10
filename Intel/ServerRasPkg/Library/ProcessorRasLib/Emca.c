/** @file
  Implementation of EMCA for processor RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#include "ProcessorRasLibInternal.h"

#define EVENTS_CTRL_OFFSET                  0xff04

typedef struct _MCE_CONTEXT {
  VOID      **CpuSaveState;
  UINTN     *CpuSaveStateSize;
  BOOLEAN   Broadcast;
  BOOLEAN   IsSaveStateInMsr;
} MCE_CONTEXT;


/**
  Return TRUE if silicon Supports EMCA Gen 2 and FALSE otherwise.

  @retval TRUE  - Emca Gen 2 is supported
  @retval FALSE - Emca Gen 2 is not supported

**/
BOOLEAN
EFIAPI
IsEmcaGen2Cap (
  VOID
  )
{
  MSR_MCG_CAP_REGISTER McgCapReg;

  McgCapReg.Uint64 = AsmReadMsr64 (MSR_MCG_CAP);
  return (BOOLEAN) (McgCapReg.Bits.EnhancedMca);
}

/**
  Return TRUE if silicon support Emca logging and FALSE otherwise.

  @retval TRUE  - Emca logging is supported
  @retval FALSE - Emca logging is not supported

**/
BOOLEAN
EFIAPI
IsEmcaLogCap (
  VOID
  )
{
  MSR_MCG_CAP_REGISTER McgCapReg;

  McgCapReg.Uint64 = AsmReadMsr64 (MSR_MCG_CAP);
  return (BOOLEAN) (McgCapReg.Bits.ErrLog);
}



/**

    Enable MCE->SMI morphing for McBankIdx

    @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.

    @retval None

**/
VOID
EmcaEnableMsmi (
  IN UINT32 McBankIdx
  )
{
  UINT32   McBankSupp;
  UINT32   McBankAddr;
  UINT64   Value;

  McBankSupp = GetEmcaBankSupp ();

  if ((RShiftU64 (McBankSupp, McBankIdx) & 0x1) != 0) {
    McBankAddr = GetAddrMcBankCtl2 (McBankIdx);

    if (McBankAddr == MSR_IA32_INVALID) {
      RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "McBankAddr == MSR_IA32_INVALID\n"));
      RAS_ASSERT (FALSE);
      return;
    }

    Value = AsmMsrOr64 (McBankAddr, BIT34);
    RAS_DEBUG ((MP_PRINT | LEVEL_REG, "[CpuRas]EmcaEnableMsmi Bank 0x%x MSR (0x%x) = 0x%lx\n", McBankIdx, McBankAddr, Value));
  }
}

VOID
ApEnableMsmiBanks (
  IN MC_ENABLE_INFO  *McEnableInfo
  )
{
  UINT32   McBankNum;
  UINT32   Index;
  UINT16   McBankScope;

  McBankNum = GetNumMcBanksPerLp ();

  for (Index = 0; Index < McBankNum; Index++) {
    if ((McEnableInfo->McBankBitFieldEn & (1 << Index)) != 0) {
      McBankScope = GetMcBankScope (Index);
      if (McBankScope == McEnableInfo->McScope) {
        EmcaEnableMsmi (Index);
      }
    }
  }
}

/**
  This function enables the SMI generation of IERR in eMCA mode

  @param[in] Socket   The socket number

**/
VOID
EnableEmcaSmiForIerr (
  IN        UINT8                           Socket
  )
{
  VIRAL_CONTROL_PCU_FUN4_STRUCT     ViralControlPcuFun4;
  EMCA_EN_CORE_IERR_TO_MSMI_UBOX_CFG_STRUCT EmcaEnIERRMSMIUboxCfg;

  EmcaEnIERRMSMIUboxCfg.Data = ReadCpuCsr (Socket, 0, EMCA_EN_CORE_IERR_TO_MSMI_UBOX_CFG_REG);
  EmcaEnIERRMSMIUboxCfg.Bits.encoreierrtomsmi = 1;
  WriteCpuCsr (Socket, 0, EMCA_EN_CORE_IERR_TO_MSMI_UBOX_CFG_REG, EmcaEnIERRMSMIUboxCfg.Data);
  RAS_DEBUG ((LEVEL_REG, "[CpuRas]Enable EMCA SMI for IERR: CSR(S 0x%x I0 R 0x%x) = 0x%x\n",
              Socket, EMCA_EN_CORE_IERR_TO_MSMI_UBOX_CFG_REG, EmcaEnIERRMSMIUboxCfg.Data));

  ViralControlPcuFun4.Data = ReadCpuCsr (Socket, 0, VIRAL_CONTROL_PCU_FUN4_REG);
  ViralControlPcuFun4.Bits.emca_mode = 1;
  WriteCpuCsr (Socket, 0, VIRAL_CONTROL_PCU_FUN4_REG, ViralControlPcuFun4.Data);
  RAS_DEBUG ((LEVEL_REG, "[CpuRas]Enable EMCA SMI for IERR: CSR(S 0x%x I0 R 0x%x) = 0x%x\n",
              Socket, VIRAL_CONTROL_PCU_FUN4_REG, EmcaEnIERRMSMIUboxCfg.Data));
}


/**

  Enable SMI for uncorrected errors for EMCA2

  @param MsmiBankBitFieldEn   Bitmask for the banks to enable Msmi Gen2

**/
VOID
EFIAPI
EnableEmca2UncorrectableError (
  IN UINT32       MsmiBankBitFieldEn
  )
{
  UINT8           Socket;
  MC_ENABLE_INFO  McEnableInfo;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (IsSocketPresent (Socket) == FALSE) {
      continue;
    }
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "[CpuRas]EnableEmca2UncorrectableError: enable EMCA SMI for socket 0x%x\n", Socket));
    EnableEmcaSmiForIerr (Socket);
  }

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "[CpuRas]EnableEmca2UncorrectableError: enable MSI banks (bitmask 0x%x)\n", MsmiBankBitFieldEn));
  
  //
  // Enable MSMI according to MC bank scope
  //
  McEnableInfo.McBankBitFieldEn = MsmiBankBitFieldEn;
  McEnableInfo.McScope = MC_SCOPE_TD;
  MultiProcessorExecute (AllAps, ALL_PACKAGE, ALL_CORE, FALSE, ApEnableMsmiBanks, &McEnableInfo);

  McEnableInfo.McScope  = MC_SCOPE_CORE;
  MultiProcessorExecute (OneApPerCore, ALL_PACKAGE, ALL_CORE, FALSE, ApEnableMsmiBanks, &McEnableInfo);

  McEnableInfo.McScope  = MC_SCOPE_PKG;
  MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, FALSE, ApEnableMsmiBanks, &McEnableInfo);
}

/**

  Enable SMI for uncorrected errors for EMCA1 - DEPRECATED.

**/
VOID
EFIAPI
EnableEmca1UncorrectableError (
  VOID
  )
{
  ASSERT (FALSE);
}

/**
    Read Core SMI Source MSR which contains bitmap of McBanks containing errors for CORE units.

    @retval Value of THREAD_SMI_SRC || CORE_SMI_SRC

**/
UINT64
EFIAPI
ReadCoreSmiSrc (
  VOID
  )
{
  UINT16 McBankScope;

  ICX_MSR_CORE_SMI_ERR_SRC_REGISTER CoreSmiSrcReg;
  ICX_MSR_THREAD_SMI_ERR_SRC_REGISTER ThreadSmiSrcReg;

  //
  // Check IFU/DCU MC bank scope
  //
  McBankScope = GetMcBankScope (0);
  CoreSmiSrcReg.Uint64 = AsmReadMsr64 (ICX_MSR_CORE_SMI_ERR_SRC);

  ThreadSmiSrcReg.Uint64 = 0;
  if (McBankScope == MC_SCOPE_TD) {
    ThreadSmiSrcReg.Uint64 |= AsmReadMsr64 (ICX_MSR_THREAD_SMI_ERR_SRC);
  }

  return (CoreSmiSrcReg.Uint64 | ThreadSmiSrcReg.Uint64);
}


/**
    Write Value to SMI SRC to Clear it.

    @param McBankBitField  Value to Write to THREAD_SMI_SRC and CORE_SMI_SRC

**/
VOID
EFIAPI
ClearCoreSmiSrc (
  IN UINT64  McBankBitField
  )
{
  UINT16 McBankScope;
  UINT16 McBank;

  //
  // Check IFU/DCU/DTLB/MLC MC bank scope
  //
  McBank = (UINT16) HighBitSet64 (McBankBitField);
  McBankScope = GetMcBankScope (McBank);

  if (McBankScope == MC_SCOPE_TD) {
    AsmWriteMsr64 (ICX_MSR_THREAD_SMI_ERR_SRC, McBankBitField);
  } else {
    AsmWriteMsr64 (ICX_MSR_CORE_SMI_ERR_SRC, McBankBitField);
  }
}

/**

    Read Uncore SMI Source MSR which contains bitmap of McBanks containing errors for UNCORE units.

    @param None

    @retval Value of UNCORE_SMI_SRC

**/
UINT64
EFIAPI
ReadUncoreSmiSrc (
  VOID
  )
{
  return AsmReadMsr64 (ICX_MSR_UNCORE_SMI_ERR_SRC);
}

/**

    Write Value to UNCORE_SMI SRC to Clear it.

    @param McBankBitField   Value to Write to UNCORE_SMI SRC

**/
VOID
EFIAPI
ClearUncoreSmiSrc (
  IN UINT64  McBankBitField
  )
{
  AsmWriteMsr64 (ICX_MSR_UNCORE_SMI_ERR_SRC, McBankBitField);
}


VOID
EFIAPI
ApEmcaSmiSrc (
  IN OUT   UINT32           *ErrorNumber
  )
{
  if (ReadCoreSmiSrc () != 0 || ReadUncoreSmiSrc () != 0) {
    InterlockedIncrement (ErrorNumber);
    RAS_DEBUG ((MP_PRINT | LEVEL_REG, "[CpuRas]EmcaError Core SMI source 0x%lx, Uncore SMI source 0x%lx, Error number 0x%x\n",
                ReadCoreSmiSrc (), ReadUncoreSmiSrc (), *ErrorNumber));
  }
}

/**
  Use non-blocking mode to check EMCA SMI error

**/
BOOLEAN
EFIAPI
CheckEmcaSmiError (
  VOID
  )
{
  UINT32                ErrorNum;

  ErrorNum = 0;
  MultiProcessorExecute (AllAps, ALL_PACKAGE, ALL_CORE, FALSE, ApEmcaSmiSrc, &ErrorNum);

  return (BOOLEAN) (ErrorNum > 0);
}

BOOLEAN
EFIAPI
CheckUboxError (
  VOID
  )
{
  UINT8                           Socket;

#ifdef GNRSRF_HOST
  MCERRLOGGINGREG_N0_UBOX_CFG_STRUCT Mcerr;
  IERRLOGGINGREG_N0_UBOX_CFG_STRUCT  Ierr;
#else
  MCERRLOGGINGREG_UBOX_CFG_STRUCT Mcerr;
  IERRLOGGINGREG_UBOX_CFG_STRUCT  Ierr;
#endif // GNRSRF_HOST

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (IsSocketPresent (Socket) == FALSE) {
      continue;
    }

#ifdef GNRSRF_HOST
    Mcerr.Data = ReadCpuCsr (Socket, 0, MCERRLOGGINGREG_N0_UBOX_CFG_REG);
    Ierr.Data = ReadCpuCsr (Socket, 0, IERRLOGGINGREG_N0_UBOX_CFG_REG);
#else
    Mcerr.Data = ReadCpuCsr (Socket, 0, MCERRLOGGINGREG_UBOX_CFG_REG);
    Ierr.Data = ReadCpuCsr (Socket, 0, IERRLOGGINGREG_UBOX_CFG_REG);
#endif // GNRSRF_HOST
    if (Mcerr.Bits.firstmcerrsrcvalid == 1 || Ierr.Bits.firstierrsrcvalid == 1) {
      RAS_DEBUG ((LEVEL_REG, "[CpuRas]CheckUboxError MCERR 0x%x, IERR 0x%x\n", Mcerr.Data, Ierr.Data));
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Get Mcerr Logging data.
  @param [in]    Socket  - Socket ID
  @param [out]   FirstMcerrValid  - 1 if the FirstSrcID is valid.
  @param [out]   FirstMcerrId     - Logical Port ID of the end point with the first MCERR.
  @param [out]   SecondMcerrValid - 1 if the SecondSrcID is valid.
  @param [out]   SecondMcerrId    - Logical Port ID of the end point with the second MCERR.

  @retval NONE

**/
VOID
EFIAPI
GetMcerrLoggingData (
  UINT8    Socket,
  OUT UINT32  *FirstMcerrValid,   OPTIONAL
  OUT UINT32  *FirstMcerrId,      OPTIONAL
  OUT UINT32  *SecondMcerrValid,  OPTIONAL
  OUT UINT32  *SecondMcerrId      OPTIONAL
  )
{
#ifdef GNRSRF_HOST
  MCERRLOGGINGREG_N0_UBOX_CFG_STRUCT Mcerr;
  MCERRLOGGINGREG_N1_UBOX_CFG_STRUCT McerrN1;
#else
  MCERRLOGGINGREG_UBOX_CFG_STRUCT Mcerr;
#endif // GNRSRF_HOST

#ifdef GNRSRF_HOST
  Mcerr.Data = ReadCpuCsr (Socket, 0, MCERRLOGGINGREG_N0_UBOX_CFG_REG);
  McerrN1.Data = ReadCpuCsr (Socket, 0, MCERRLOGGINGREG_N1_UBOX_CFG_REG);
#else
  Mcerr.Data = ReadCpuCsr (Socket, 0, MCERRLOGGINGREG_UBOX_CFG_REG);
#endif // GNRSRF_HOST

  if (FirstMcerrValid != NULL) {
    *FirstMcerrValid = Mcerr.Bits.firstmcerrsrcvalid;
  }

  if (FirstMcerrId != NULL) {
    *FirstMcerrId = Mcerr.Bits.firstmcerrsrcid;
  }

  if (SecondMcerrValid != NULL) {
#ifdef GNRSRF_HOST
    *SecondMcerrValid = McerrN1.Bits.secondmcerrsrcvalid;
#else
    *SecondMcerrValid = Mcerr.Bits.secondmcerrsrcvalid;
#endif // GNRSRF_HOST
  }

  if (SecondMcerrId != NULL) {
#ifdef GNRSRF_HOST
    *SecondMcerrId = McerrN1.Bits.secondmcerrsrcid;
#else
    *SecondMcerrId = Mcerr.Bits.secondmcerrsrcid;
#endif // GNRSRF_HOST
  }

}

/**
  Get IERR logging data.
  @param [in]    Socket  - Socket ID
  @param [out]   FirstIerrvalid  - 1 if the FirstIerrSrcID is valid.
  @param [out]   FirstIerrId     - Logical Port ID of the end point with the first IERR.
  @param [out]   SecondIerrValid - 1 if the SecondSrcID is valid.
  @param [out]   SecondIerrId    - Logical Port ID of the end point with the second IERR.

  @retval NONE

**/
VOID
EFIAPI
GetIerrLoggingData (
  UINT8    Socket,
  OUT UINT32  *FirstIerrvalid,   OPTIONAL
  OUT UINT32  *FirstIerrId,      OPTIONAL
  OUT UINT32  *SecondIerrValid,  OPTIONAL
  OUT UINT32  *SecondIerrId      OPTIONAL
  )
{
#ifdef GNRSRF_HOST
  IERRLOGGINGREG_N0_UBOX_CFG_STRUCT  Ierr;
  IERRLOGGINGREG_N1_UBOX_CFG_STRUCT  IerrN1;
#else
  IERRLOGGINGREG_UBOX_CFG_STRUCT  Ierr;
#endif // GNRSRF_HOST

#ifdef GNRSRF_HOST
  Ierr.Data = ReadCpuCsr (Socket, 0, IERRLOGGINGREG_N0_UBOX_CFG_REG);
  IerrN1.Data = ReadCpuCsr (Socket, 0, IERRLOGGINGREG_N1_UBOX_CFG_REG);
#else
  Ierr.Data = ReadCpuCsr (Socket, 0, IERRLOGGINGREG_UBOX_CFG_REG);
#endif // GNRSRF_HOST

  if (FirstIerrvalid != NULL) {
    *FirstIerrvalid = Ierr.Bits.firstierrsrcvalid;
  }

  if (FirstIerrId != NULL) {
    *FirstIerrId = Ierr.Bits.firstierrsrcid;
  }

  if (SecondIerrValid != NULL) {
#ifdef GNRSRF_HOST
    *SecondIerrValid = IerrN1.Bits.secondierrsrcvalid;
#else
    *SecondIerrValid = Ierr.Bits.secondierrsrcvalid;
#endif // GNRSRF_HOST
  }

  if (SecondIerrId != NULL) {
#ifdef GNRSRF_HOST
    *SecondIerrId = IerrN1.Bits.secondierrsrcid;
#else
    *SecondIerrId = Ierr.Bits.secondierrsrcid;
#endif // GNRSRF_HOST
  }

}


BOOLEAN
EFIAPI
CheckUboxErrorStatus (
  VOID
  )
{
  UINT8                           Socket;

  //
  // Check the IMC error src in UBOX / SMISRCLOG
  //
   for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (IsSocketPresent (Socket) == FALSE) {
      continue;
    }

    if (IsUboxImcError (Socket)) {
      return TRUE;
    }

  } // For Socket
  return FALSE;
}


/**
  Determine MCE Mechanism

  This function returns the mechanism to use to generate the MCE

  @retval FALSE    MMIO
  @retval TRUE     MSR

**/
BOOLEAN
IsSaveStateInMsr (
  VOID
  )
{

  UINT8                                 Socket;
  SMM_FEATURE_CONTROL_UBOX_DFX_STRUCT   SmmFeatCtrlUboxDfxReg;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (IsSocketPresent (Socket) == FALSE) {
      continue;
    }
    SmmFeatCtrlUboxDfxReg.Data = ReadCpuCsr (Socket, 0, SMM_FEATURE_CONTROL_UBOX_DFX_REG);
    RAS_DEBUG ((MP_PRINT | LEVEL_REG, "[CpuRas]Save state enabled 0x%x CSR(S 0x%x I0 R 0x%x)=0x%x\n",
                SmmFeatCtrlUboxDfxReg.Bits.smm_cpu_save_en, Socket, SMM_FEATURE_CONTROL_UBOX_DFX_REG, SmmFeatCtrlUboxDfxReg.Data));
    if (SmmFeatCtrlUboxDfxReg.Bits.smm_cpu_save_en != 0) {
      return TRUE;
    }
  }

  return FALSE;
}

VOID
SignalMceMmio (
  IN OUT VOID  **CpuSaveState,
  IN UINTN     *CpuSaveStateSize
  )
{
  //
  //write 0x1 at the offset pointed by Events control field
  //
  /* Events control field is a 2-byte field in at offset 0xff04 of SMRAM
    if on chip save state is not enabled */
  UINTN     SmramSmbase;
  UINTN     EventsCtrl;
  UINT16    *EventsCtrlPtr;
  UINTN     LogicalCpu;

  LogicalCpu = GetProcessorInfo (NULL);
  SmramSmbase = ((UINTN *) CpuSaveState) [LogicalCpu] - SIZE_64KB + CpuSaveStateSize[LogicalCpu];
  EventsCtrl = (UINTN) SmramSmbase + EVENTS_CTRL_OFFSET;
  EventsCtrlPtr = (UINT16 *) EventsCtrl;
  *EventsCtrlPtr = 0x0001;

  RAS_DEBUG ((MP_PRINT | LEVEL_REG, "SignalMceMmio[%ld]: Save event control at %p\n", LogicalCpu, EventsCtrlPtr));
}

VOID
SignalMceMsr (
  VOID
  )
{
  MSR_SMRAM_EVENT_CTL_HLT_IO_REGISTER SmramEventCtlReg;

  SmramEventCtlReg.Uint64 = AsmReadMsr64 (MSR_SMRAM_EVENT_CTL_HLT_IO);
  
  SmramEventCtlReg.Bits.EventCtl = 1;

  AsmWriteMsr64 (MSR_SMRAM_EVENT_CTL_HLT_IO, SmramEventCtlReg.Uint64);

  RAS_DEBUG ((MP_PRINT | LEVEL_REG, "[CpuRas]SignalMceMsr: MSR(0x%x) = 0x%lx\n", MSR_SMRAM_EVENT_CTL_HLT_IO, SmramEventCtlReg.Uint64));
}

VOID
EFIAPI
ApSignalMceToOs (
  IN OUT MCE_CONTEXT          *MceContext
  )
{
  if (MceContext->Broadcast || (IsLmceSignaled () && !IsMcip ())) {
    RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "[CpuRas]ApSignalMceToOs: Broadcast 0x%x LMCE signaled 0x%x MCIP 0x%x\n", MceContext->Broadcast, IsLmceSignaled (), (IsMcip())));
    if (MceContext->IsSaveStateInMsr) {
      SignalMceMsr ();
    } else {
      SignalMceMmio (MceContext->CpuSaveState, MceContext->CpuSaveStateSize);
    }
  }
}



VOID
EFIAPI
SignalMceToOs (
  IN OUT VOID    **CpuSaveState,
  IN     UINTN   *CpuSaveStateSize,
  IN     BOOLEAN Broadcast
  )
{
  MCE_CONTEXT     MceContext;

  MceContext.CpuSaveState = CpuSaveState;
  MceContext.CpuSaveStateSize = CpuSaveStateSize;
  MceContext.Broadcast = Broadcast;
  MceContext.IsSaveStateInMsr = IsSaveStateInMsr ();

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "[CpuRas]SignalMceToOs: Broadcast 0x%x, SaveStateInMsr 0x%x, CpuSaveState %p, CpuSaveStateSize 0x%x\n",
              MceContext.Broadcast, MceContext.IsSaveStateInMsr, MceContext.CpuSaveState, (UINT32) *MceContext.CpuSaveStateSize));

  MultiProcessorExecute (AllAps, ALL_PACKAGE, ALL_CORE, FALSE, ApSignalMceToOs, &MceContext);
}

/**
  This function clears MCERRLOG and IERRLOG.

**/
VOID
EFIAPI
ClearErrLogReg (
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
#else
    WriteCpuCsr (Socket, 0, MCERRLOGGINGREG_UBOX_CFG_REG, 0);
    RAS_DEBUG ((LEVEL_REG, "[CpuRas]Clear MCERR logging status, CSR(S 0x%x I0 R 0x%x) = 0\n", Socket, MCERRLOGGINGREG_UBOX_CFG_REG));

    WriteCpuCsr (Socket, 0, IERRLOGGINGREG_UBOX_CFG_REG, 0);
    RAS_DEBUG ((LEVEL_REG, "[CpuRas]Clear IERR logging status, CSR(S 0x%x I0 R 0x%x) = 0\n", Socket, IERRLOGGINGREG_UBOX_CFG_REG));
#endif // GNRSRF_HOST

   ClearPcuMcaErrSrcWa5370783 (Socket);
  }
}

/**
  This function clears UMC status.

  Note: Nothing todo for 10nm since smisrcumc bit has been deprecated from UBOX

**/
VOID
EFIAPI
ClearUmcStatus (
  VOID
  )
{
  UINT8                       Socket;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (IsSocketPresent (Socket) == FALSE) {
      continue;
    }

    ClearUboxUmcError (Socket);
    RAS_DEBUG ((LEVEL_REG, "[CpuRas]Clear UMC status = 0x%x\n", Socket));
  }
}

/**
  This function clears shadow MSR register.

  The function clears shadow MSR status register before
  enabling RAS features to avoid last boot error impact current boot flow.

**/
VOID
EFIAPI
ClearShadowMsrReg (
  VOID
  )
{
  UINT8                                       Socket;
  UINT8                                       Mc;
  UINT8                                       ChOnMc;
  UINT8                                       ChOnSkt;

  for (Socket = 0; Socket < MAX_SOCKET; Socket ++) {
    if (IsSocketPresent (Socket) == FALSE) {
      continue;
    }

    for (Mc=0; Mc < MAX_IMC; Mc ++) {
      if (IsNodePresent (SKTMC_TO_NODE(Socket, Mc)) == FALSE) {
        continue;
      }
      //
      // Clear M2MEM and IMC shadow MSR status register
      //
      SetM2mMciStatusShadowN1 (Socket, Mc, 0);
      SetM2mMciStatusShadowN0 (Socket, Mc, 0);

      for (ChOnMc = 0; ChOnMc < MAX_MC_CH; ChOnMc ++) {
        ChOnSkt = NODECHA_TO_SKTCHA (Mc, ChOnMc);
        if (IsChannelPresentPerSocket (Socket, ChOnSkt) == FALSE) {
          continue;
        }
        SetImcMciStatusShadowN1 (Socket, ChOnSkt, 0);
        SetImcMciStatusShadowN0 (Socket, ChOnSkt, 0);
      }
    }
  }
}