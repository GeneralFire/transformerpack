/**
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c) 1999 - 2018, Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


  @file ApplicationProfile.c

  Platform Initialization Driver.

Revision History:

**/

#include "SetupPlatform.h"
#include <IioUniversalData.h>
#include <Library/CmosAccessLib.h>
#include "Platform.h"

//
// Application
//
#define AP_ENABLE            1
#define AP_DISABLE           0
#define AP_DEFAUTL           0xF

#define AP_HWPM_NATIVE         1
#define AP_HWPM_DISABLE        0

#define AP_EETurboDisable_D    1
#define AP_EETurboDisable_E    0

//
// Intel® Xeon® Scalable Family (Purley) Platform
// Performance and Power Optimization Guide
// Reversion: 1.1
// Notes: make sure the version align with PrleyRpPkg\Platform\Dxe\Setup\SetupStrings.uni STR_BENCHMARK_FORM_HELP
//
//APTIOV_SERVER_OVERRIDE_RC_START : To resolve build issue
#define ApplicationProfile_Revision  L"1.1"
//APTIOV_SERVER_OVERRIDE_RC_END : To resolve build issue
//
// which application we support, need to align with Configuration.h
//
// Notes: HPC and TPCx-BB are removed since can't cover lots of real cases
//
EFI_APPLICATION_PROFILE mApplicationProfile [] = {
    // CorePrefetch  HyperThreading  Turbo  SubNUMAClustering  UPIPrefetch
    { AP_ENABLE,    AP_ENABLE,    AP_ENABLE,    AP_ENABLE,    	AP_ENABLE}, // General Computing
    { AP_ENABLE,    AP_DISABLE,   AP_ENABLE,    AP_ENABLE,      AP_ENABLE}, // Memory BandWidth
    { AP_ENABLE,    AP_DISABLE,   AP_ENABLE,    AP_DISABLE,     AP_ENABLE}, // Matrix Calculation
    { AP_DISABLE,   AP_ENABLE,    AP_ENABLE,    AP_ENABLE,      AP_ENABLE}, // Eneryg Efficiency
    { AP_DISABLE,   AP_ENABLE,    AP_ENABLE,    AP_ENABLE,      AP_ENABLE}, // Server Side Java
    { AP_DISABLE,   AP_ENABLE,    AP_ENABLE,    AP_DISABLE,     AP_ENABLE}, // OLTP
    { AP_ENABLE,    AP_ENABLE,    AP_ENABLE,    AP_DISABLE,     AP_ENABLE}  // Virtualization
};

EFI_APPLICATION_PROFILE_EXT mApplicationProfileExt [] = {
    // EETurboDisable       PagePolicy          ProcessorHWPMEnable     DimmInterleaving    PatrolScrub     PerfPLimitEn    ProcessorC1eEnable  ProcessorVmxEnable  VTdSupport
    { AP_DEFAUTL,           AP_DEFAUTL,         AP_HWPM_NATIVE,         DIMM_INTER_DISABLE, AP_DEFAUTL,     AP_DEFAUTL,     AP_DISABLE,         AP_DEFAUTL,         AP_DEFAUTL}, // General Computing
    { AP_DEFAUTL,           AP_DEFAUTL,         AP_HWPM_NATIVE,         DIMM_INTER_DISABLE, AP_DEFAUTL,     AP_DEFAUTL,     AP_DISABLE,         AP_DEFAUTL,         AP_DEFAUTL}, // Memory BandWidth
    { AP_DEFAUTL,           AP_DEFAUTL,         AP_HWPM_NATIVE,         AP_DEFAUTL,         AP_DEFAUTL,     AP_DEFAUTL,     AP_DEFAUTL,         AP_DEFAUTL,         AP_DEFAUTL}, // Matrix Calculation
    { AP_DEFAUTL,           OPEN_PAGE_ADAPTIVE, AP_HWPM_NATIVE,         AP_DEFAUTL,         AP_DEFAUTL,     AP_DEFAUTL,     AP_DEFAUTL,         AP_DEFAUTL,         AP_DEFAUTL}, // Eneryg Efficiency
    { AP_DEFAUTL,           OPEN_PAGE_ADAPTIVE, AP_HWPM_DISABLE,        AP_DEFAUTL,         AP_ENABLE,      AP_DEFAUTL,     AP_DEFAUTL,         AP_DEFAUTL,         AP_DEFAUTL}, // Server Side Java
    { AP_EETurboDisable_E,  CLOSED_PAGE_DEFAULT,AP_DEFAUTL,             AP_DEFAUTL,         AP_DEFAUTL,     AP_DEFAUTL,     AP_DEFAUTL,         AP_DEFAUTL,         AP_DEFAUTL}, // OLTP
    { AP_DEFAUTL,           AP_DEFAUTL,         AP_DEFAUTL,             AP_DEFAUTL,         AP_DEFAUTL,     AP_DEFAUTL,     AP_DEFAUTL,         AP_ENABLE,          AP_ENABLE} // Virtualization
};


/**

    Setup Application profiles

    @param Default - Application: which Application that you selected

    @retval None

**/
VOID
SetupApplicationProfile (
   IN   UINT8                                      Application
   )
{
  EFI_STATUS                                 Status = EFI_SUCCESS;
  SOCKET_MP_LINK_CONFIGURATION               SocketMpLinkConfiguration;
  SOCKET_IIO_CONFIGURATION                   SocketIioConfiguration;
  SOCKET_PROCESSORCORE_CONFIGURATION         SocketProcessorCoreConfiguration;
  SOCKET_MEMORY_CONFIGURATION                SocketMemoryConfiguration;
  SOCKET_POWERMANAGEMENT_CONFIGURATION       SocketPowerManagementConfiguration;

  //
  // Do Nothing if it's Auto or invalid input
  //
  if ((Application == 0xff) || (Application >= ARRAY_SIZE (mApplicationProfileExt))) {
    return;
  }

  //
  // Prinf Pnp guide reversion:
  //
  //APTIOV_SERVER_OVERRIDE_RC_START : To resolve build issue
  DEBUG((EFI_D_INFO, "ApplicationProfile Revision : %a\n", ApplicationProfile_Revision ));
//APTIOV_SERVER_OVERRIDE_RC_END : To resolve build issue
  // Get gEfiSocketProcessorCoreVarGuid
  Status = GetSpecificConfigGuid(&gEfiSocketProcessorCoreVarGuid, &SocketProcessorCoreConfiguration);

  // Get SocketMemoryVariable
  Status = GetSpecificConfigGuid(&gEfiSocketMemoryVariableGuid, &SocketMemoryConfiguration);

  // Get SocketPowermanagementConfiguration
  Status = GetSpecificConfigGuid(&gEfiSocketPowermanagementVarGuid, &SocketPowerManagementConfiguration);

  // Get gEfiSocketMpLinkVariableGuid
  Status = GetSpecificConfigGuid(&gEfiSocketMpLinkVariableGuid, &SocketMpLinkConfiguration);

  // Get gEfiSocketIioVariableGuid
  Status = GetSpecificConfigGuid(&gEfiSocketIioVariableGuid, &SocketIioConfiguration);

  //
  // Program Application Profile - start
  //

  //
  // Core Prefetch: MLC Streamer, MLC Spatial Prefetcher, DCU Data Prefetcher, DCU Instruction Prefetcher
  //
  SocketProcessorCoreConfiguration.MlcStreamerPrefetcherEnable = mApplicationProfile[Application].CorePrefetch;
  SocketProcessorCoreConfiguration.MlcSpatialPrefetcherEnable = mApplicationProfile[Application].CorePrefetch;
  SocketProcessorCoreConfiguration.DCUStreamerPrefetcherEnable = mApplicationProfile[Application].CorePrefetch;
  SocketProcessorCoreConfiguration.DCUIPPrefetcherEnable = mApplicationProfile[Application].CorePrefetch;

  // HT
  SocketProcessorCoreConfiguration.ProcessorHyperThreadingDisable = mApplicationProfile[Application].HyperThreading;

  // Turbo
  SocketPowerManagementConfiguration.TurboMode= mApplicationProfile[Application].Turbo;

  // Sub-NUMA Clustering
  SocketMpLinkConfiguration.SncEn = mApplicationProfile[Application].SubNUMAClustering;

  // UPI Prefetch
  SocketMpLinkConfiguration.KtiPrefetchEn = mApplicationProfile[Application].UPIPrefetch;

  //
  // Program Application Profile EXT table.
  //
  // Notes: skip AP_DEFAUTL since as default
  //
  if (mApplicationProfileExt[Application].EETurboDisable != AP_DEFAUTL) {
    SocketPowerManagementConfiguration.EETurboDisable = mApplicationProfileExt[Application].EETurboDisable;
  }

  if (mApplicationProfileExt[Application].PagePolicy != AP_DEFAUTL) {
    SocketMemoryConfiguration.PagePolicy = mApplicationProfileExt[Application].PagePolicy;
  }

  if (mApplicationProfileExt[Application].ProcessorHWPMEnable != AP_DEFAUTL) {
    SocketPowerManagementConfiguration.ProcessorHWPMEnable = mApplicationProfileExt[Application].ProcessorHWPMEnable;
  }

  if (mApplicationProfileExt[Application].DimmInterleaving != AP_DEFAUTL) {
    SocketMemoryConfiguration.DfxDimmInterleaving = mApplicationProfileExt[Application].DimmInterleaving;
  }

  if (mApplicationProfileExt[Application].PatrolScrub != AP_DEFAUTL) {
    SocketMemoryConfiguration.PatrolScrub = mApplicationProfileExt[Application].PatrolScrub;
  }

  if (mApplicationProfileExt[Application].PerfPLimitEn != AP_DEFAUTL) {
    SocketPowerManagementConfiguration.PerfPLimitEn = mApplicationProfileExt[Application].PerfPLimitEn;
  }

  if (mApplicationProfileExt[Application].ProcessorC1eEnable != AP_DEFAUTL) {
    SocketPowerManagementConfiguration.ProcessorC1eEnable = mApplicationProfileExt[Application].ProcessorC1eEnable;
  }

  if (mApplicationProfileExt[Application].ProcessorVmxEnable != AP_DEFAUTL) {
    SocketProcessorCoreConfiguration.ProcessorVmxEnable = mApplicationProfileExt[Application].ProcessorVmxEnable;
  }

  if (mApplicationProfileExt[Application].VTdSupport != AP_DEFAUTL) {
    SocketIioConfiguration.VTdSupport = mApplicationProfileExt[Application].VTdSupport;
  }

  //
  // Program Application Profile -end
  //

  // Set gEfiSocketProcessorCoreVarGuid
  Status = SetSpecificConfigGuid(&gEfiSocketProcessorCoreVarGuid, &SocketProcessorCoreConfiguration);

  // Set SocketMemoryVariable
  Status = SetSpecificConfigGuid(&gEfiSocketMemoryVariableGuid, &SocketMemoryConfiguration);

  // Set SocketPowermanagementConfiguration
  Status = SetSpecificConfigGuid(&gEfiSocketPowermanagementVarGuid, &SocketPowerManagementConfiguration);

  // Set gEfiSocketMpLinkVariableGuid
  Status = SetSpecificConfigGuid(&gEfiSocketMpLinkVariableGuid, &SocketMpLinkConfiguration);

  // Get gEfiSocketIioVariableGuid
  Status = SetSpecificConfigGuid(&gEfiSocketIioVariableGuid, &SocketIioConfiguration);
}

