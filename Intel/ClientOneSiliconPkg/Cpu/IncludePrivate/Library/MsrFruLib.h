/** @file
  This file contains MSR access for specific generation.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/
#ifndef _MSR_FRU_LIB_H_
#define _MSR_FRU_LIB_H_

#include <Uefi/UefiBaseType.h>

/**
  This function get MSR_FIT_BIOS_ERROR.

  @param[out]  FitEntryType
  @param[out]  FitErrorCode
**/
VOID
MsrGetFitBiosError (
  OUT UINT8 *FitEntryType,
  OUT UINT8 *FitErrorCode
  );

/**
  This function returns if programable TJ offset enable

  @retval TRUE if programable TJ offset is supported
          FALSE if programable TJ offset is not supported
**/
BOOLEAN
MsrIsPrgTjOffsetEn (
  VOID
  );

/**
  This function return max and min bus ratio.

  @param[out]  MaxBusRatio  Maximum Non-Turbo Ratio
  @param[out]  MinBusRatio  Max efficiency ratio
**/
VOID
MsrGetBusRatio (
  OUT UINT8 *MaxBusRatio, OPTIONAL
  OUT UINT8 *MinBusRatio  OPTIONAL
  );

/**
  This function return max Efficiency Ratio.

  @retval Max efficiency ratio
**/
UINT8
MsrGetMaxEfficiencyRatio (
  VOID
  );

/**
  This function return max Non-Turbo Ratio.

  @retval Max Non-Turbo Ratio
**/
UINT8
MsrGetMaxNonTurboRatio (
  VOID
  );

/**
  This function return the supported Config TDP Levels.

  @retval number of config TDP levels
**/
UINT8
MsrGetConfigTdpLevels (
  VOID
  );

/**
  This function returns if PPIN feature present

  @retval TRUE if PPIN feature present
          FALSE if PPIN feature not present
**/
BOOLEAN
MsrIsPpinCap (
  VOID
  );

/**
  This function returns if MCU 2nd patch load success

  @retval TRUE if MCU 2nd patch success
          FALSE if MCU 2nd patch not success
**/
BOOLEAN
MsrIsMcu2ndPatchSuccess (
  VOID
  );

/**
  This function returns if Turbo RatioLimit is Programmable or not

  @retval TRUE if Turbo RatioLimit is Programmable
          FALSE if Turbo RatioLimit is Not Programmable
**/
BOOLEAN
MsrIsTurboRatioLimitProgrammable (
  VOID
  );

/**
  This function returns if TDP Limit is Programmable or not

  @retval TRUE if TDP Limit is Programmable
          FALSE if TDP Limit is Not Programmable
**/
BOOLEAN
MsrIsTdpLimitProgrammable (
  VOID
  );

/**
  This function returns if Timed Mwait is Supported or Not

  @retval TRUE if Timed Mwait is Supported
          FALSE if Timed Mwait is Not Supported
**/
BOOLEAN
MsrIsTimedMwaitSupported (
  VOID
  );

/**
  Return if CPU supports PFAT

  @retval TRUE             If CPU Supports
  @retval FALSE            If CPU doesn't Supports
**/
BOOLEAN
MsrIsPfatEnabled (
  VOID
  );

/**
  Return if CPU Supports LPM

  @retval TRUE             If CPU Supports
  @retval FALSE            If CPU Doesn't supports
**/
BOOLEAN
MsrIsCpuSupportsLpm (
  VOID
  );

/**
  This function is to get platform PowerLimit.

  @param[out]  PowerLimit1
  @param[out]  PowerLimit1En
  @param[out]  PowerLimit1Time
  @param[out]  PowerLimit2
  @param[out]  PowerLimit2En
  @param[out]  Lock
**/
VOID
MsrGetPlatformPowerLimit (
  OUT UINT32   *PowerLimit1,
  OUT BOOLEAN  *PowerLimit1En,
  OUT UINT32   *PowerLimit1Time,
  OUT UINT32   *PowerLimit2,
  OUT BOOLEAN  *PowerLimit2En,
  OUT BOOLEAN  *Lock
  );

/**
  This function is to set platform PowerLimit. All input parameters
  must be provided to override the existing platform PowerLimit value.

  @param[in]  PowerLimit1
  @param[in]  PowerLimit1En
  @param[in]  PowerLimit1Time
  @param[in]  PowerLimit2
  @param[in]  PowerLimit2En
  @param[in]  Lock
**/
VOID
MsrSetPlatformPowerLimit (
  IN UINT32   PowerLimit1,
  IN BOOLEAN  PowerLimit1En,
  IN UINT32   PowerLimit1Time,
  IN UINT32   PowerLimit2,
  IN BOOLEAN  PowerLimit2En,
  IN BOOLEAN  Lock
  );

/**
  Return if CPU Supports SMM_SUPOVR_STATE_LOCK

  @retval TRUE             If CPU supports
  @retval FALSE            If CPU doesn't supports
**/
BOOLEAN
MsrIsSmmSupovrStateLockSupported (
  VOID
  );

/**

  This function returns bit per logical processor indicating whether
  the logical processor is in the middle of long flow and hence will
  delay servicing of SMI.

  @retval Bit per logical processor

**/
UINT64
MsrGetSmmDelayed (
  VOID
  );

/**
  This function returns bit per logical processor indicating whether
  the logical processor is in state where SMIs are blocked.

  @retval Bit per logical processor

**/
UINT64
MsrGetSmmBlocked (
  VOID
  );

/**
  Return if Edram Enable

  @retval TRUE             If Edram Enable
  @retval FALSE            If Edram Disable
**/
BOOLEAN
MsrIsEdramEnable (
  VOID
  );

/**
  Sets WRC COS of specific policy

  @param[in] UINT8   Wrc COS Policy to set ways
  @param[in] UINT32  COS Ways Mask

  @retval EFI_SUCCESS           set WRC COS successful
  @retval EFI_INVALID_PARAMETER invalid COS policy
**/
EFI_STATUS
MsrSetWrcCos (
  IN UINT8     CosPolicy,
  IN UINT32    CosWaysMask
  );

/**
  Return if AesXts256 is supported

  @retval TRUE             If AesXts256 is supported
  @retval FALSE            If AesXts256 is not supported
**/
BOOLEAN
MsrIsAesXts256Supported (
  VOID
  );

/**
  This function return MkTmeMaxKeyidBits value

  @retval MkTmeMaxKeyidBits value
**/
UINT32
MsrGetMkTmeMaxKeyidBits (
  VOID
  );

/**
  This function return MkTmeMaxKey value

  @retval MkTmeMaxKey value
**/
UINT32
MsrGetMkTmeMaxKey (
  VOID
  );

/**
  This function is to configure TME Activate MSR.

  @param[in]  TmeEnable
  @param[in]  KeySelect
  @param[in]  SaveKeyForStandby
  @param[in]  TmePolicy
  @param[in]  MkTmeKeyIdBits
  @param[in]  MkTmeCryptoAlgs
**/
VOID
MsrConfigureTmeActivate (
  IN UINT8   TmeEnable,
  IN UINT8   KeySelect,
  IN UINT8   SaveKeyForStandby,
  IN UINT8   TmePolicy,
  IN UINT8   MkTmeKeyIdBits,
  IN UINT16  MkTmeCryptoAlgs
  );

/**
  This function write-lock the TME Activate MSR.

  @param[in]  TmeEnable
**/

VOID
MsrLockTmeActivate (
  IN UINT32  TmeEnable
  );

/**
  Return if Lock bit in TME Activate MSR is set

  @retval TRUE             If lock bit is set
  @retval FALSE            If lock bit is not set
**/
BOOLEAN
MsrIsTmeActivateLockSet (
  VOID
  );

/**
  Return if TME is enabled

  @retval TRUE             If TME enable & lock bit is set
  @retval FALSE            If TME enable & lock bit is not set
**/
BOOLEAN
MsrIsTmeEnabled (
  VOID
  );

/**
  Return if the processor is a preproduction sample

  @retval TRUE          If the processor is a preproduction sample
  @retval FALSE         If the part is intended for production
**/
BOOLEAN
MsrIsSamplePart (
  VOID
  );

/**
  Return if FME_ACTIVATE MSR can be setup and
  the FZM functionality can be enabled

  @retval TRUE          If FME_ACTIVATE MSR can be setup and
                        the FZM functionality can be enabled
  @retval FALSE         If FME_ACTIVATE MSR can not be setup and
                        the FZM functionality can not be enabled
**/
BOOLEAN
MsrIsSxp2lmEnabled (
  VOID
  );

/**
  This function is to Enable RAR Timer
**/
VOID
MsrEnableRarTimer (
  VOID
  );

/**
  This function returns Max Turbo Ratio.

  @return Max Turbo Ratio
**/
UINT8
MsrGetMaxTurboRatio (
  VOID
  );

/**
  Disable or enable CPU Crashlog dump feature by MSR.

  @param[in] BOOLEAN      Policy for CrashLog
**/
VOID
MsrEnableCpuCrashLog (
  IN  BOOLEAN                          CrashLogEnable
  );

/**
  This function is to enable/disbale AVX Instructions

  @param[in]  AvxDisable
**/
VOID
MsrSetAvx (
  IN BOOLEAN AvxDisable
  );

/**
  This function is to enable/disable AVX3 Instructions

  @param[in]  Avx3Disable
**/
VOID
MsrSetAvx3 (
  IN BOOLEAN Avx3Disable
  );

/**
  This function set GrashLog GPRs (General Purpose Registers) Mask
  Which allows GPRs to not be included in the Crash Log CPU register data.

  @param[in] BOOLEAN     Policy for CrashLog Enable
  @param[in] UINT8       Policy for CrashLog GPRs dump
**/
VOID
MsrSetCrashLogGprsMask (
  IN  BOOLEAN                 CrashLogEnable,
  IN  UINT8                   CrashLogGprsEnable
  );
#endif // _MSR_FRU_LIB_H_
