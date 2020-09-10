/** @file
  Internal header file used for SncPrefetch library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#ifndef _SNC_PREFETCH_LIB_INTERNAL_H
#define _SNC_PREFETCH_LIB_INTERNAL_H

#include <Upi/KtiHost.h>
#include <Library/RcDebugLib.h>
#include <Library/UncoreLib.h>

typedef struct {
  BOOLEAN          Enabled;
  UINT8            IntlvGranularity;
  UINT32           Base;
  UINT32           Limit;
} PREFETCH_RANGE_INFO_INTERNAL;

typedef enum {
  PREFETCH_1LM = 0,
  PREFETCH_2LM,
  MAX_PREFETCH_RNG_TYPE
} PREFETCH_RNG_TYPE;

/**
  API callable from outside UncoreRC to calculate  Clustering (SNC) base

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param  MemInfo          - Memory Information structure.

  @retval UNCORE_SUCCESS   - SNC mode is programmed successfully; or system is not configured for SNC mode operation.
  @retval UNCORE_FAILURE   - Memory profile is not suitable for SNC mode operation.

**/
KTI_STATUS
CalculateClusterBase (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo
  );

/**
  API callable from outside UncoreRC to program Sub Numa Clustering (SNC) mode.

  @param  KtiInternalGlobal - KTIRC Internal Global data
  @param  MemInfo           - Memory Information structure.
  @param  Status            - SNC mode status

  @retval UNCORE_SUCCESS    - SNC mode is programmed successfully; or system is not configured for SNC mode operation.
  @retval UNCORE_FAILURE    - Memory profile is not suitable for SNC mode operation.

**/
KTI_STATUS
ConfigureSncMode (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo,
  KTI_STATUS                 Status
  );

/**
  Program SNC config register in Kti Agents

  @param  KtiInternalGlobal    -  KTIRC Internal Global data
  @param  Soc                  -  Socket Id
  @param  SncConfigData        -  SNC Config Data
  @param  SncConfigMs2IdiData  -  SNC Config Data in Mesh2IDI, if not exist, set it to be 0

  @retval None

**/
VOID
ProgramSncConfigInKtiAgent (
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN UINT8                    Soc,
  IN UINT32                   SncConfigData,
  IN UINT32                   SncConfigMs2IdiData
  );

/**
  Enable XPT/KTI/Remote prefetch features.

  This routine is to enable all prefetch features

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param  Socket           - Socket Id
  @param  MemInfo          - Memory Information structure.

  @retval None

**/
VOID
ProgramPrefetchEntry (
  IN OUT KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                          Socket,
  IN MEM_INFO                       *MemInfo
  );

/**
  Read Channel Dimm Information

  This routine read nvram dimm info for this channel

  @param  Socket                  Socket
  @param  Mc                      Memory controller
  @param  Ch                      Channel
  @param  ChannelMemSize          Pointer to ChannelMemSize
  @param  AepDimmPresent          Pointer to AepDimmPresent
  @param  NvDimmPresent           Pointer to NvDimmPresent

  @retval None

**/
VOID
ReadChannelDimmInfo (
  UINT8  Socket,
  UINT8  Mc,
  UINT8  Ch,
  UINT16 *ChannelMemSize,
  UINT8  *AepDimmPresent,
  UINT8  *NvDimmPresent
  );

VOID
GetMemoryChannelInfo (
  MEM_INFO       *MemInfo,
  UINT8          Socket
  );

/**
  Determine SNC setting

  This routine determiness SNC CHA slices for each cluster and number of clusters within this socket.
  This setting is based on mc0/1 population and NUMA non-interleave cross mc or interleave cross mc

  @param  KtiInternalGlobal - KTIRC Internal Global data
  @param  MemInfo           - Memory Information structure.

  @retval KTI_SUCCESS - on successful

**/
KTI_STATUS
DetermineSncSetting (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo
  );

/**
  Check whether XptKti Prefetch need to be disable or not.

  This routine is to check whether XPT/KTI prefetch need to be disable or not

  @param  KtiInternalGlobal - KTIRC Internal Global data
  @param  Socket            - Socket Id
  @param  MemInfo           - Memory Information structure.

  @retval TRUE/FALSE

**/
BOOLEAN
CheckXptKtiPrefetchNeedToDisable (
  IN OUT KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                          Socket,
  IN MEM_INFO                       *MemInfo
  );

/**
  This routine is to disable xpt remote prefetch feature

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Socket            - Socket Id

  @retval None

**/
VOID
DisableRemoteXpt (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      Socket
  );

/**
  This routine is to enable xpt remote prefetch feature

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param  Socket           - Socket Id
  @param  MemInfo          - Memory Information structure.

  @retval None

**/
VOID
ProgramRemoteXpt (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                  Socket,
  IN MEM_INFO               *MemInfo
  );

/**
  Setup UPI MiniSad

  This routine is to set UPI MiniSad for all ports in the specific socket

  @param  KtiInternalGlobal    -  KTIRC Internal Global data
  @param  Socket               -  Socket Id
  @param  Minisadmodeselect    -  Minisad mode
  @param  MiniSadTable         -  Minisad table value

  @retval None

**/
VOID
SetupUpiMiniSad (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      Socket,
  IN UINT32                     Minisadmodeselect,
  IN UINT32                     MiniSadTable
  );

/**
  Enable Preftch in KTI Agent
  For KTI prefetch, need to configure: ktiagctrl.enableprefetch
  For remote XPT prefetch, need to configure: ktiagctrl.enableprefetch

  This routine is to enable prefetch in KTI agent.

  @param  KtiInternalGlobal            -  KTIRC Internal Global data
  @param  Socket                       -  Socket Id
  @param  KtiOrRemoteXptPrefetchEnable:
           0: KtiXptPrefetch Enabled
           1: RemoteXptPrefetch Enabled
           2: KtiXptPrefetch Disabled
           3: RemoteXptPrefetch Disabled

  @retval None

**/
VOID
EnablePrefetchInKtiAgent (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Socket,
  UINT8                      KtiOrRemoteXptPrefetchEnable
  );

/**
  W/A for kti prefetch

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param MemInfo           - Memory Information structure.

**/
VOID
KtiPrefetchWa (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo
  );
#endif // _SNC_PREFETCH_LIB_INTERNAL_H
