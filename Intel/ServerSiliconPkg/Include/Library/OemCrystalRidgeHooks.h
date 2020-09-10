/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation. <BR>

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
  Intel's suppliers or licensor's in any way.
**/

// OemCrystalRidgeHooks
#ifndef _OEMCRYSTALRIDGEHOOKS_H_
#define _OEMCRYSTALRIDGEHOOKS_H_
#define FLUSH_NEAR_MEMORY_CACHE_GPP_GROUP      GPIO_SKL_H_GROUP_GPP_D
#define FLUSH_NEAR_MEMORY_CACHE_GPP_PAD        GPIO_SKL_H_GPP_D0
#define FM_ADR_TRIGGER_N_GPIO_PAD              GPIO_SKL_H_GPP_E3
#ifdef EBG_SC_FLAG
#define FLUSH_NEAR_MEMORY_CACHE_GPP_SMI_STS    R_EBG_PCH_GPIO_PCR_GPP_D_SMI_STS
#elif CDF_SC_FLAG
#define FLUSH_NEAR_MEMORY_CACHE_GPP_SMI_STS    R_CDF_PCH_GPIO_PCR_GPP_D_SMI_STS
#else
#define FLUSH_NEAR_MEMORY_CACHE_GPP_SMI_STS    R_PCH_PCR_GPIO_GPP_D_SMI_STS
#endif

VOID OemGetMemTopologyInfo(UINT8 *skt, UINT8 *ch, UINT8 *dimm);
VOID OemBeforeUpdatePCDBIOSPartition(UINT8 skt, UINT8 ch, UINT8 dimm, UINT8 **cfgCurRecBuffer, UINT32* cfgCurRecLength);
VOID OemBeforeUpdatePCDOSPartition(  UINT8 skt,UINT8 ch, UINT8 dimm, UINT8 **cfgOutRecBuffer, UINT32* cfgOutRecLength);
EFI_STATUS OemCreateNfitTable(UINT64  *Table, UINT32 TableBufferSize);
EFI_STATUS OemCreatePcatTable(UINT64 *Table, UINT64 TableBufferSize);
VOID OemUpdateNfitTable (UINT64  *Table);
VOID OemUpdatePcatTable (UINT64  *Table);
BOOLEAN OemInitFlushNearMemoryCacheGPI(VOID);

/**
  @brief OEM hook for retrieving of maximum amount of time allowed to use the SMBus

  @param[out] TimeMs            Maximum amount of time allowed to use the SMBus

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_INVALID_PARAMETER TimeMs is NULL.
**/
EFI_STATUS
OemSmbusGetMaxAccessTime (
  OUT UINT32 *TimeMs
  );

/**
  @brief OEM hook for SMBus access acquiring

  @param[in]  Socket            Socket index at system level
  @param[in]  Imc               iMC index at socket level
  @param[out] TsodPollingState  TSOD polling state

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_NOT_READY         TSOD control acquisition or TSOD state change failed.
  @retval EFI_INVALID_PARAMETER Invalid Socket or Imc value.
**/
EFI_STATUS
OemAcquireSmbusCtrl (
  IN  UINT8 Socket,
  IN  UINT8 Imc,
  OUT UINT8 *TsodPollingState
  );

/**
  @brief OEM hook for SMBus access releasing

  @param[in]  Socket            Socket index at system level
  @param[in]  Imc               iMC index at socket level
  @param[in] TsodPollingState   TSOD polling state

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_NOT_READY         TSOD control acquisition or TSOD state change failed.
  @retval EFI_INVALID_PARAMETER Invalid Socket or Imc value.
**/
EFI_STATUS
OemReleaseSmbusCtrl (
  IN UINT8 Socket,
  IN UINT8 Imc,
  IN UINT8 TsodPollingState
  );

/**

  Return Date and Time from RTC in Unix format which fits in 32 bit format.

  @param[out] NumOfSeconds Pointer to return calculated time.

  @retval EFI_SUCCESS .
  @retval Other EFI status if error occurred.

**/
EFI_STATUS
OemGetTimeStamp (
  OUT UINT32 *NumOfSeconds
  );

/**
  Routine Description: OEM hook for preventing ADR trigger during power failure.

  This OEM hook disables the FM_ADR_TRIGGER_N receiver input to prevent ADR trigger by setting GPIORXDIS to 1.
  The eADR handler will re-enable and assert FM_ADR_TRIGGER_N to trigger the legacy ADR after eADR flush is complete.

**/
VOID
OemPreventAdrTrigger (
  VOID
  );

#endif // _OEMCRYSTALRIDGEHOOKS_H_
