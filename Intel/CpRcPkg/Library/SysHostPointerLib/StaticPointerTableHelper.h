/** @file
  StaticPointerTableHelper.h provides common definitions for SysHostPointerLib implementation.

  This library allows for setting and getting pointers in a way that provides for
  higher performance than normal services allowed.  The use of normal services is
  preferred.

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

#ifndef _STATIC_POINTER_TABLE_HELPER_H_
#define _STATIC_POINTER_TABLE_HELPER_H_

#include <Pi/PiPeiCis.h>

//
// This is an internal use only GUID, it is intentionally not published widely and not included in DEC
// This GUID is used for the HOB containing the table in some implementations.
//
#define STATIC_POINTER_TABLE_GUID { 0xa6a8d25b, 0xfab2, 0x411a, { 0x98, 0x77, 0xfe, 0x9f, 0xe4, 0x7, 0xc7, 0xba } }

///
/// This is not a general purpose capability.  It only knows a defined set of pointers and keeps one of each type.
///
typedef enum {
  SptSysHost,
  SptHbmHost,
  SptSysInfoVar,
  SptSysSetup,
  SptSemaphoreData,
  SptSpdData,
  SptDebugData,
  SptMemMapHost,
  SptEmulationSetting,
  SptCpuAndRevision,
  SptMrcPerformanceData,
  SptDdr5LrdimmTrainingData,
  SptCpgcHost,
  SptMax
} STATIC_POINTER_TYPE;

///
/// PPI GUID for each static point type. They are private and will not be exposed in DEC file
///
#define SPT_SYS_HOST_PPI_GUID          { 0x4874dfb1, 0x1dd6, 0x4b8a, { 0x80, 0xc0, 0xeb, 0x64, 0xc1, 0x13, 0x88, 0xa0}}
#define SPT_HBM_HOST_PPI_GUID          { 0x216f5289, 0xc0e7, 0x4cdb, { 0x90, 0xd4, 0xe1, 0x39, 0x1d, 0xc1, 0xa2, 0xb9}}
#define SPT_SYS_INFO_VAR_PPI_GUID      { 0x6052067b, 0x8dba, 0x4b35, { 0xbf, 0x6a, 0x48, 0x61, 0xf5, 0xbf, 0x1a, 0xde}}
#define SPT_SYS_SETUP_PPI_GUID         { 0x58f6472a, 0x76e3, 0x4b59, { 0xbb, 0x25, 0x67, 0xbb, 0xe1, 0xd9, 0xce, 0xfe}}
#define SPT_SEMAPHORE_DATA_PPI_GUID    { 0xb805014f, 0x2a1c, 0x4c03, { 0x9d, 0x8e, 0x70, 0x4f, 0xfd, 0x25, 0x43, 0x47}}
#define SPT_SPD_DATA_PPI_GUID          { 0x6411f543, 0x11a7, 0x4011, { 0xa4, 0x89, 0xe7, 0x74, 0xa1, 0x71, 0x1e, 0x0c}}
#define SPT_DEBUG_DATA_PPI_GUID        { 0x4a2981e0, 0xe7dc, 0x44a1, { 0x8c, 0x1d, 0x3f, 0x2c, 0xb0, 0x06, 0x80, 0xe5}}
#define SPT_MEM_MAP_HOST_PPI_GUID      { 0xa3906b6f, 0xa8cb, 0x4d52, { 0xa2, 0x28, 0x73, 0x99, 0x88, 0x0d, 0x00, 0xc4}}
#define SPT_EMULATION_SETTING_PPI_GUID { 0x889e90f5, 0x6d1a, 0x464f, { 0xa0, 0xd1, 0xec, 0xa9, 0x10, 0x8f, 0x9f, 0x50}}
#define SPT_CPU_AND_REVISION_PPI_GUID  { 0xc0e7a6d2, 0xcf42, 0x4b6f, { 0xb5, 0xf7, 0xda, 0xe2, 0x1d, 0x29, 0xed, 0x90}}
#define SPT_CPGC_HOST_PPI_GUID         { 0x90668208, 0xb482, 0x44b5, { 0x84, 0xcb, 0xb6, 0xa4, 0x62, 0x08, 0x61, 0x4d}}


/**
  Fix up static pointers once PEI core has migrated NEM data to permanent memory.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
FixupStaticPointers (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**
  Syncs MM7 register from BSP across all APs for use in multithreaded functions.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
SyncMm7AllThreads (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDesc,
  IN VOID                       *Ppi
);

///
/// The static pointer table structure is optimized for quick pointer location
/// The pointers are designed for use in mixed 32 and 64 bit environments (PEI and SMM for example)
///
typedef struct {
  EFI_PHYSICAL_ADDRESS          SptEntries[SptMax];
  EFI_PEI_PPI_DESCRIPTOR        Ppis[SptMax];
} STATIC_POINTER_TABLE;

#endif // _STATIC_POINTER_TABLE_HELPER_H_
