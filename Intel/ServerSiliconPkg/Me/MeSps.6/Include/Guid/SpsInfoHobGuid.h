/** @file
  SPS info HOB definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#ifndef _SPS_INFO_HOB_GUID_H_
#define _SPS_INFO_HOB_GUID_H_

//
// Definition of ME FW SPS Features Set - according to the
// Intel ME-BIOS Interface Specification (revision 1.0.3) - chapter 4.5.1
//
typedef union
{
  struct
  {
    UINT32 Set1;
    UINT32 Set2;
  } Data;
  struct
  {
    UINT32 NodeManager                 :1,    // 0
           PeciProxy                   :1,    // 1
           ICC                         :1,    // 2
           MeStorageServices           :1,    // 3
           BootGuard                   :1,    // 4
           PTT                         :1,    // 5
           OemDefinedCpuDebugPolicy    :1,    // 6
           ResetSuppression            :1,    // 7
           PmBusProxy                  :1,    // 8
           CpuHotPlug                  :1,    // 9
           MicProxy                    :1,    //10
           MctpProxy                   :1,    //11
           ThermalReportVolumeAirflow  :1,    //12
           SocThermalReporting         :1,    //13
           DualBIOSSupport             :1,    //14
           MPHYSurvivabilityProgramming:1,    //15
           PECIOverDMI                 :1,    //16
           PCHDebug                    :1,    //17
           PowerThermalUtilitySupport  :1,    //18
           FiaMuxConfiguration         :1,    //19
           PchThermalSensorInit        :1,    //20
           DeepSx                      :1,    //21
           DualMeFwImage               :1,    //22
           DirectFwUpdate              :1,    //23
           MctpInfrastructure          :1,    //24
           CUPS                        :1,    //25
           FlashDescRegionVerification :1,    //26
           Hotham                      :1,    //27
           TurboStateLimiting          :1,    //28
           TelemetryHub                :1,    //29
           MeShutdown                  :1,    //30
           ASA                         :1;    //31
    UINT32 WarmResetNotificationSubFlow:1;    //0
  } Bits;
} SPS_FEATURE_SET;

typedef struct _SPS_INFO_HOB_
{
  SPS_FEATURE_SET   FeatureSet;
  UINT8             WorkFlow;
  BOOLEAN           PwrOptBoot;    // NM may request power reduction at boot
  UINT8             Cores2Disable; // NM may request disabling processor cores
  BOOLEAN           NmPowerMsmtSupport;
  BOOLEAN           NmHwChangeStatus;
  BOOLEAN           DualBIOSWatchdogEnabled;
} SPS_INFO_HOB;


extern EFI_GUID gSpsInfoHobGuid;

#endif // _SPS_INFO_HOB_GUID_H_

