/** @file
  Interface of Error Control Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

/**
  Enable or disable Patrol Error Downgrade

  @param[in]  PatrolErrorDowngradeEn     Patrol Error Downgrade setting.
**/
VOID
EFIAPI
PatrolErrorDowngradeSetting (
  IN UINT8 PatrolErrorDowngradeEn
  );

/**
  Enable or disable 2LM Correctable Error Logging in m2mem

  @param[in]  Ce2LmLoggingEn      2LM Correctable Error Logging setting.
**/
VOID
EFIAPI
Ce2LmLoggingSetting (
  IN UINT8 Ce2LmLoggingEn
  );

/**
  Enable or disable latch first corrected error in KTI

  @param[in]  KtiFirstCeLatchEn   Latch first corrected error setting.
**/
VOID
EFIAPI
KtiFirstCeLatchSetting (
  IN UINT8 KtiFirstCeLatchEn
  );

/**
  Enable or disable LMCE downgrade

  @param[in]  LmceDowngradeEn     LMCE downgrade downgrade setting.
**/
VOID
EFIAPI
LmceDowngradeSetting (
  IN UINT8 LmceDowngradeEn
  );

/**
  Control the signaling of EWB errors as UCNA or SRAO

  @param[in]  LlcEwbErrorControl   LLC EWB Error Control setting.
**/
VOID
EFIAPI
LlcEwbErrorControlSetting (
  IN UINT8 LlcEwbErrorControl
  );

/**
  Clear Fnv viral Received flag

**/
VOID
EFIAPI
ClearFnvViral (
  VOID
  );
