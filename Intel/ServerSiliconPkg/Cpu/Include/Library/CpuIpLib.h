/** @file

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

#ifndef __CPU_IP_LIB_H__
#define __CPU_IP_LIB_H__

/**
  Set abort inbound request.
  @param[in] Socket            Socket ID.
  @param[in] Value             The value to abort all Inbound Requests on the DMI port.

  @retval VOID

**/
VOID
EFIAPI
SetAbortInboundRequests (
  IN UINT8     Socket,
  IN UINT8     Value
  );

/**
  Return DYNAMIC_PERF_POWER_CTL_PCU_FUN2_STRUCT register data.
  @param[in]      Socket                         Socket ID.
  @param[in]      UncrPerfPlmtOvrdEn             The value of uncore_perf_plimit_override_enable.
  @param[in]      EetOverrideEn                  The value of eet_override_enable.
  @param[in]      IoBwPlmtOvrdEn                 The value of io_bw_plimit_override_enable.
  @param[in]      IomApmOvrdEn                   The value of iom_apm_override_enable.
  @param[in]      KtiApmOvrdEn                   The value of kti_apm_override_enable.

  @retval VOID

**/
VOID
EFIAPI
SetDynamicPerPowerCtlData (
  IN UINT8         Socket,
  IN UINT32        UncrPerfPlmtOvrdEn,
  IN UINT32        EetOverrideEn,
  IN UINT32        IoBwPlmtOvrdEn,
  IN UINT32        IomApmOvrdEn,
  IN UINT32        KtiApmOvrdEn
  );

/**
  Set PCU_BIOS_SPARE2_PCU_FUN3_REG register data.
  @param[in]      Socket                         Socket ID.
  @param[in]      OutOfBandAlternateEpb          The set data of bios_spare.

  @retval VOID

**/
VOID
EFIAPI
SetPcuBiosSpare2Csr (
  IN UINT8         Socket,
  IN UINT32        OutOfBandAlternateEpb
  );


/**

  Get B2P MAILBOX_BIOS_CMD_READ_PM_CONFIG info

  @param socket:           CPU socket ID

  @retval Value get from B2P

**/
UINT32
EFIAPI
ReadPmFeatureB2p (
  IN     UINT8     Socket
  );

/**

  This routine is called to program SST-CP (RAPL Prioritization)
  It will be call multiple time passing any the SocketNumber to be programmed.

  @param      SocketNumber             Socket index that is executing.
  @param[in]  SstCpSystemStatus        SST-CP capable status in system
  @param[in]  SstCpSetting             SST-CP setting

  @retval     None

**/
VOID
EFIAPI
InitializeSstCpConfig (
    IN  UINT8         SocketNumber,
    IN  UINT32        SstCpSystemStatus,
    IN  UINT8         SstCpSetting
  );

#endif
