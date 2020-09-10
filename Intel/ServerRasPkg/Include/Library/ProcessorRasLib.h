/** @file
  Interface of processor RAS library.

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

#ifndef __PROCESSOR_RAS_LIB_H__
#define __PROCESSOR_RAS_LIB_H__

#include <Emca.h>

/*
  Get core and thread shift bits.

  @param[out] ThreadBits   Output for thread shift bits
  @param[out] CoreBits     Output for core shift bits

*/
VOID
EFIAPI
GetCoreAndSmtBits (
  OUT UINTN *ThreadBits,
  OUT UINTN *CoreBits
  );

/**
  Enable the feature to clear pending SMI on SIPI on all APs.

  An AP responds with a spurious SMI when it is woken from Wait-For-SIPI (WFS)
  by a Startup-IPI (SIPI) if an SMI has occurred while it was in WFS. This
  feature clears any pending SMI for an AP when a SIPI is issued to it.
**/
VOID
EFIAPI
EnableClearPendingSmiOnSipi (
  VOID
  );

/**

  Enable Processor cloaking feature.

**/
VOID
EFIAPI
EnableCloaking (
  VOID
  );

/**
  Is Lmce supported by the processor

  @return True if Lmce is supported by the processor

**/
BOOLEAN
EFIAPI
IsLmceProcessorSupported (
  VOID
  );

/**

  Enable Processor LMCE feature.

**/
VOID
EFIAPI
EnableLmce (
  VOID
  );

/**

  Check whether it is BMCE.

  @retval TRUE    It is MBCE.
  @retval FALSE   It is not BMCE.

**/
BOOLEAN
EFIAPI
IsBmce (
  VOID
  );

/**

  Check whether it is LMCE.

  @retval TRUE    It is LMCE.
  @retval FALSE   It is not LMCE.

**/
BOOLEAN
EFIAPI
IsLmce (
  VOID
  );

/**

  Clears LMCE status.

**/
VOID
EFIAPI
ClearLmceStatus (
  VOID
  );


/**
  Return TRUE if silicon Supports EMCA Gen 2 and FALSE otherwise.

  @retval TRUE  - Emca Gen 2 is supported
  @retval FALSE - Emca Gen 2 is not supported

**/
BOOLEAN
EFIAPI
IsEmcaGen2Cap (
  VOID
  );


/**
  Return TRUE if silicon support Emca logging and FALSE otherwise.

  @retval TRUE  - Emca logging is supported
  @retval FALSE - Emca logging is not supported

**/
BOOLEAN
EFIAPI
IsEmcaLogCap (
  VOID
  );


/**

  Enable SMI for uncorrected errors for EMCA2

  @param MsmiBankBitFieldEn   Bitmask for the banks to enable Msmi Gen2

**/
VOID
EFIAPI
EnableEmca2UncorrectableError (
  IN UINT32       MsmiBankBitFieldEn
  );

/**

  Enable SMI for uncorrected errors for EMCA1 - DEPRECATED.

**/
VOID
EFIAPI
EnableEmca1UncorrectableError (
  VOID
  );

/**

  Checks Emca SMI error.

  @retval TRUE    This is EMCA SMI error.
  @retval FALSE   This is not EMCA SMI error.

**/
BOOLEAN
EFIAPI
CheckEmcaSmiError (
  VOID
  );


/**

  Checks Ubox error.

  @retval TRUE    This is Ubox error.
  @retval FALSE   This is not Ubox error.

**/
BOOLEAN
EFIAPI
CheckUboxError (
  VOID
  );

/**

  Checks Ubox error status.

  @retval TRUE    This is Ubox error.
  @retval FALSE   This is not Ubox error.

**/
BOOLEAN
EFIAPI
CheckUboxErrorStatus (
  VOID
  );

/**
  This function clears MCERRLOG and IERRLOG.

**/
VOID
EFIAPI
ClearErrLogReg (
  VOID
  );

/**
  This function clears UMC status.

**/
VOID
EFIAPI
ClearUmcStatus (
  VOID
  );

/**
  Signal machine check exception to OS

  @param CpuSaveState      Points to an array, where each element is a pointer to a CPU save state. The
                           corresponding element in CpuSaveStateSize specifies the number of bytes in
                           the save state area.
  @param CpuSaveStateSize  Points to an array, where each element describes the number of bytes
                           in the corresponding save state specified by CpuSaveState
  @param Broadcast         Indicate whether it is a broadcast machine check exception

**/
VOID
EFIAPI
SignalMceToOs (
  IN OUT VOID    **CpuSaveState,
  IN     UINTN   *CpuSaveStateSize,
  IN     BOOLEAN Broadcast
  );

/**
  Enables Csmi Gen1 - DEPRECATED.

**/
VOID
EFIAPI
EnableCsmiGen1 (
  VOID
  );


/**
  Enables Csmi Gen2 for all APs

  @param[in]  CsmiBankBitFieldEn   Bitmask for the banks to enable Csmi Gen2
  @param[in]  CsmiThreshold        Threshold for correctable until CSMI is signaled
  @param[in]  CloakingEn           Indicates whether cloaking is enabled or not

**/
VOID
EFIAPI
EnableCsmiGen2 (
  IN UINT32    CsmiBankBitFieldEn,
  IN UINT16    CsmiThreshold,
  IN BOOLEAN   CloakingEn
  );


/**
  Signal a CMCI to the OS

  This function will signal an CMCI to the OS

  @param[in] BankIndex  Bank Index of the CPU.
  @param[in] Package    The package number of the CPU.
  @param[in] Core       The Core number of the CPU.

**/
VOID
EFIAPI
SignalCmciToOs (
  IN  UINT32  BankIndex,
  IN  UINT32  Package,
  IN  UINT32  Core,
  IN  UINTN   *CpuIndex OPTIONAL
  );

/**
    Read Core SMI Source MSR which contains bitmap of McBanks containing errors for CORE units.

    @retval Value of CORE_SMI_SRC

**/
UINT64
EFIAPI
ReadCoreSmiSrc (
  VOID
  );

/**
    Write Value to SMI SRC to Clear it.

    @param McBankBitField  Value to Write to CORE_SMI_SRC

**/
VOID
EFIAPI
ClearCoreSmiSrc (
  IN UINT64  McBankBitField
  );


/**

    Read Uncore SMI Source MSR which contains bitmap of McBanks containing errors for UNCORE units.

    @param None

    @retval Value of UNCORE_SMI_SRC

**/
UINT64
EFIAPI
ReadUncoreSmiSrc (
  VOID
  );

/**

    Write Value to UNCORE_SMI SRC to Clear it.

    @param McBankBitField   Value to Write to UNCORE_SMI SRC

**/
VOID
EFIAPI
ClearUncoreSmiSrc (
  IN UINT64  McBankBitField
  );



/**
    Get Scope for the Bank. The scope can be thread level, core level and package level.

    @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.

    @retval  MC_SCOPE_TD    Thread Scope
    @retval  MC_SCOPE_CORE  Core Scope
    @retval  MC_SCOPE_PKG   Package Scope

**/
UINT16
EFIAPI
GetMcBankScope (
  IN UINT32 McBankIdx
  );

/**
    Checks whether the machine check bank support log.

    @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.

    @retval TRUE         The machine check bank support Emca log.
    @retval FALSE        The machine check bank does not support Emca log.

**/
BOOLEAN
EFIAPI
IsMcBankSupportEmcaLog (
  IN UINT32 McBankIdx
  );

/**

    Get Section Type for McBank

    @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.

    @retval SecType - Return the section type related to the McBank Unit

**/
UINT16
EFIAPI
GetMcBankSecType (
  IN UINT32 McBankIdx
  );


/**

    Return whether the bank unit is Memory

    @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.

    @retval UNIT TYPE  - Silicon Specific Bank Unit Type

**/
UINT32
EFIAPI
GetMcBankUnitType (
  IN UINT32 McBankIdx
  );

/**
  Get the number of MCBANKS_PER_LP.

  MCBANKS_PER_LP = MCBANK_CORE + MCBANKS_UNCORE = MCG_CAP.Count
  Assumption of implementation is that all sockets are symmetric, they all have the same number of MCBanks Per LP

  @return MCBANKS_PER_LP

**/
UINT32
EFIAPI
GetNumMcBanksPerLp (
  VOID
  );


/**

    Get Bank Index for Mci_status for McBank

    @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.

    @retval Mci_status Index

**/
UINT32
EFIAPI
GetAddrMcBankSts (
  IN UINT32 McBankIdx
  );


/**
  Disables CSMI at MC bank

  @param McBank - McBank number

**/
VOID
EFIAPI
DisableCsmiAtMcBank (
  IN UINT8 McBank
  );


/**
  Disables memory correctable SMI

**/
VOID
EFIAPI
DisableCorrectedMemErrorSmi (
  VOID
  );

/**
  Access MC Bank MSRs to fill MC signature as defined in the EMCA SAS.

  If MC Bank information is invalid use the Shadow CSRs.

  @param[in]  McBankIdx   The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.
  @param[in]  Socket      Socket ID.
  @param[in]  Instance    Unit instance number.
  @param[out] McSig       A pointer to Machine Check Signature Error Record. See Related definitions and EMCA SAS.
**/
VOID
EFIAPI
ReadM2MemMcBankSignature (
  IN  UINT32                        McBankIdx,
  IN  UINT8                         Socket,
  IN  UINT8                         Instance,
  OUT EMCA_MC_SIGNATURE_ERR_RECORD  *McSig
  );

/**

    Call Bank specific Signature hook.

    @param McBankIdx  The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.
    @param Socket     Socket Id
    @param McSig      A pointer to Machine Check Signature Error Record. See Related definitions and EMCA SAS.

**/
VOID
EFIAPI
ReadMcBankSigHook (
  IN UINT32                         McBankIdx,
  IN UINT8                          Socket,
  OUT EMCA_MC_SIGNATURE_ERR_RECORD  *McSig
  );

/**

    Write updated values for corrected error in the McBank. The Firmware updated bit must be set.

    @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.
    @param McSig     - A pointer to Machine Check Signature Error Record. See Related definitions and EMCA SAS.
                       The values in the signature will be written to the banks.

**/
VOID
EFIAPI
WriteMcBank (
  IN UINT32                       McBankIdx,
  IN EMCA_MC_SIGNATURE_ERR_RECORD *McSig
  );

/**

  Sets firmware updated bit values for corrected error in the McBank.

  @param McBankInx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.

**/
VOID
EFIAPI
SetMcBankFirmwareUpdatedBit (
  IN UINT32                 McBankIdx
  );

typedef enum _MCE_CLASS {
  FATAL,
  DUE,
  UCNA,
  SRAO,
  SRAR,
  CORRECTED,
  INVALID
} MCE_CLASS;

/**
  Get MC class

  @param   McErrSig     Signature structure

  @return MC exception class

**/
MCE_CLASS
EFIAPI
GetMceClass (
  IN EMCA_MC_SIGNATURE_ERR_RECORD  *McErrSig
  );

/**
  Get MC bank physical address

  @param   McErrSig     Signature structure

  @return  physical address of machine check bank
  @retval  MAX_ADDRESS  The physical address in machine check bank is not valid

**/
PHYSICAL_ADDRESS
EFIAPI
GetMcAddress (
  IN EMCA_MC_SIGNATURE_ERR_RECORD  *McErrSig
  );

/**
  Get machine check bank instance

  @param   BankIndex     Machine check bank index

  @return  machine check bank instance
  @retval  NA  The instance is not valid
**/
UINT8
EFIAPI
GetMcBankInstance (
  IN UINT32 McBankIdx
  );

/**
  Get machine M2Mem McBank Index

  @param   MC_Index     memory controller index

  @return  McBank Index
  @retval  McBank Index
**/
UINT32
EFIAPI
GetM2MemMcBankIndex (
  IN        UINT8   MC_Index
) ;

typedef enum {
  DisablePoisonViral = 0,
  EnablePoisonOnly = 1,
  EnablePoisonViral = 3
  } POISON_VIRAL_FLAG;

/**
  Sets poison and viral feature for the whole system.

  This function supports three option specified by the input flag parameter:
  1. DisablePoisonViral - Disable viral and poison
  2. EnablePoisonOnly   - Enable poison only
  3. EnablePoisonViral  - Enable both poison and viral
  The function will ASSERT() if the flag is not one of the above options.

  @param[in] Flag  Enumerate type to specify poison and viral setting.

**/
VOID
EFIAPI
SetViralPoison (
  IN POISON_VIRAL_FLAG           Flag
  );


/**
  Returns whether machine check is in progress.

  @retval BOOLEAN - True/False

**/
BOOLEAN
EFIAPI
IsMcip (
  VOID
  );

/**
  Returns whether IP is related to error.

  @retval BOOLEAN - True/False

**/
BOOLEAN
EFIAPI
IsEipv (
  VOID
  );


/**
  Returns whether restart IP is valid.

  @retval BOOLEAN - True/False

**/
BOOLEAN
EFIAPI
IsRipv (
  VOID
  );

/**
  Clear Machine Check in progress bit.

**/
VOID
EFIAPI
ClearMcip (
  VOID
  );

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
  UINT8       Socket,
  OUT UINT32  *FirstMcerrValid,    OPTIONAL
  OUT UINT32  *FirstMcerrId,       OPTIONAL
  OUT UINT32  *SecondMcerrValid,   OPTIONAL
  OUT UINT32  *SecondMcerrId       OPTIONAL
  );

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
  UINT8       Socket,
  OUT UINT32  *FirstIerrvalid,   OPTIONAL
  OUT UINT32  *FirstIerrId,      OPTIONAL
  OUT UINT32  *SecondIerrValid,  OPTIONAL
  OUT UINT32  *SecondIerrId      OPTIONAL
  );

/**
  This function clears shadow MSR register.

  The function clears shadow MSR status register before
  enabling RAS features to avoid last boot error impact current boot flow.

**/
VOID
EFIAPI
ClearShadowMsrReg (
  VOID
  );

/**

  Clear the value in a given McBank.

  @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.

  @retval None
**/
VOID
EFIAPI
ClearMcBank (
  IN UINT32 McBankIdx
  );

/**

  Set the value in SKX_MSR_DCU_MODE_CONTROL MSR per core.

  @param None.

  @retval None
**/
VOID
EFIAPI
SetDCUPoisonDetectionMode (
  IN VOID
  );
#endif
