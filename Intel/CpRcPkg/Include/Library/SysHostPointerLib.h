/** @file
  Interface header file for accessing pointers in a highly optimized manner

  This library provides a way of accessing pointers that is highly optimized for
  performance.  This is necessary because some silicon code has significant
  performance challenges using PPI, protocol, and other standard services.

  Please note that this is not intended for general purpose use.  This should only
  be used when data flow design and standard services are shown to be insufficient.
  And shown with measured performance data.

  The API is based on specific Get/Set pointer functions.  This allows for flatter
  implementations.  This is consistent with EDK II library service API like
  PeiServicesLib.  Reducing parameters and other such instruction count reduction
  steps are necessary for maximum performance.

  To extend this API, copy and modify the GetSysHostPointer and SetSysHostPointer
  functions for your pointer.
  Avoid extending this API for other purposes besides fast pointer lookup.


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

#ifndef _SYS_HOST_POINTER_LIB_H_
#define _SYS_HOST_POINTER_LIB_H_

#include <Uefi/UefiBaseType.h>

//
// If this struct is the first member of a SysHostPointerLib pool and the size is non-zero,
// this StaticPointerTableLib Pool will be copied into SMM for use by SMM drivers.  If this
// struct does not exist in a given SPT Pool, then the PEI/DXE pointer is used.  Keep in
// mind that the PEI/DXE memory cannot be accessed from SMM after SMM Lock.
//
// Note: The Guid member must be equal to gStaticPointerSmmSizeGuid to enable SMM copy
//
typedef struct _STATIC_POINTER_SMM_SIZE_STRUCT {
  EFI_GUID        Guid;
  UINT16          Size;
} STATIC_POINTER_SMM_SIZE_STRUCT;

/**
  Gets SysHost pointer.

  This function abstracts the retrieval for sysHost pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance for very high
  frequency operations.

  @param    None

  @return   Pointer to SysHost

**/
VOID*
EFIAPI
GetSysHostPointer (
  VOID
  );

/**
  Sets SysHost pointer.

  This function abstracts the setting for sysHost pointer.
  We may have implementation specific way to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of SysHost pointer to store.

  @return     None

**/
VOID
EFIAPI
SetSysHostPointer (
  IN VOID *SysHost
  );


/**
  Gets HBM Host pointer.

  This function abstracts the retrieval for HbmHost pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance for very high
  frequency operations.

  @param    None

  @return   Pointer to HbmHost

**/
VOID*
EFIAPI
GetHbmHostPointer (
  VOID
  );

/**
  Sets HbmHost pointer.

  This function abstracts the setting for HbmHost pointer.
  We may have implementation specific way to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of HbmHost pointer to store.

  @return     None

**/
VOID
EFIAPI
SetHbmHostPointer (
  IN VOID *HbmHost
  );

/**
  Gets CpgcHost pointer.

  This function abstracts the retrieval for CpgcHost pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance for very high
  frequency operations.

  @param    None

  @return   Pointer to CpgcHost

**/
VOID*
EFIAPI
GetCpgcHostPointer (
  VOID
  );

/**
  Sets CpgcHost pointer.

  This function abstracts the setting for CpgcHost pointer.
  We may have implementation specific way to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of CpgcHost pointer to store.

  @return     None

**/
VOID
EFIAPI
SetCpgcHostPointer (
  IN VOID *CpgcHost
  );

/**
  Gets CpuAndRevision pointer.

  This function abstracts the retrieval for CpuAndRevision pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance for very high
  frequency operations.

  @param    None

  @return   Pointer to CpuAndRevision

**/
VOID*
EFIAPI
GetCpuAndRevisionPointer (
  VOID
  );

/**
  Sets CpuAndRevision pointer.

  This function abstracts the setting for CpuAndRevision pointer.
  We may have implementation specific way to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of CpuAndRevision pointer to store.

  @return     None

**/
VOID
EFIAPI
SetCpuAndRevisionPointer (
  IN VOID *CpuAndRevision
  );

/**
  Gets SysInforvar type pointer.

  This function abstracts the retrieval for SysInfoVar pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance for very high
  frequency operations.

  @param    None

  @return   Pointer to SysInfoVar

**/
VOID*
EFIAPI
GetSysInfoVarPointer (
  VOID
  );

/**
  Sets SysInforvar type pointer.

  This function abstracts the setting for SysInfoVar pointer.
  We may have implementation specific way to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of SysInfoVar pointer to store.

  @return     None

**/
VOID
EFIAPI
SetSysInfoVarPointer (
  IN VOID *SysInforVar
  );

/**
  Gets SysSetup pointer.

  This function abstracts the retrieval for SysSetup pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance for very high
  frequency operations.

  @param    None

  @return   Pointer to SysSetup

**/
VOID*
EFIAPI
GetSysSetupPointer (
  VOID
  );

/**
  Sets SysSetup pointer.

  This function abstracts the setting for SysSetup pointer.
  We may have implementation specific way to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of SysSetup pointer to store.

  @return     None

**/
VOID
EFIAPI
SetSysSetupPointer (
  IN VOID *SysSetup
  );

/**
  Gets SemaphoreData pointer.

  This function abstracts the retrieval for SemaphoreData pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance for very high
  frequency operations.

  @param    None

  @return   Pointer to SemaphoreData

**/
VOID*
EFIAPI
GetSemaphoreDataPointer (
  VOID
  );

/**
  Sets the Semaphore Data pointer.

  This function abstracts the setting of the SemaphoreData pointer.
  We may have implementation specific ways to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of SemaphoreData pointer to store.

  @return     None

**/
VOID
EFIAPI
SetSemaphoreDataPointer (
  IN VOID *SemaphoreData
  );

/**
  Gets SpdData pointer.

  This function abstracts the retrieval for SpdData pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance for very high
  frequency operations.

  @param    None

  @return   Pointer to SpdData

**/
VOID*
EFIAPI
GetSpdDataPointer (
  VOID
  );

/**
  Sets the SPD Data pointer.

  This function abstracts the setting of the SpdData pointer.
  We may have implementation specific ways to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of SpdData pointer to store.

  @return     None

**/
VOID
EFIAPI
SetSpdDataPointer (
  IN VOID *SemaphoreData
  );

/**
  Gets DebugData pointer.

  This function abstracts the retrieval for DebugData pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance for very high
  frequency operations.

  @param    None

  @return   Pointer to DebugData

**/
VOID*
EFIAPI
GetDebugDataPointer (
  VOID
  );

/**
  Sets the DebugData pointer.

  This function abstracts the setting of the DebugData pointer.
  We may have implementation specific ways to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of DebugData pointer to store.

  @return     None

**/
VOID
EFIAPI
SetDebugDataPointer (
  IN VOID *DebugData
  );

/**
  Gets MemMapHost pointer.

  This function abstracts the retrieval for the MemMapHost pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance.

  @param    None

  @return   Pointer to MemMapHost

**/
VOID*
EFIAPI
GetMemMapHostPointer (
  VOID
  );

/**
  Sets the MemMapHost pointer.

  This function abstracts the setting of the MemMapHost pointer.
  We may have implementation specific ways to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of MemMapHost pointer to store.

  @return     None

**/
VOID
EFIAPI
SetMemMapHostPointer (
  IN VOID *MemMapHost
  );

/**
  Gets Emulation Setting.

  This function abstracts the retrieval for the EmulationSetting pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance.

  @param    None

  @return   Pointer to EmulationSetting

**/
VOID*
EFIAPI
GetEmulationSettingPointer (
  VOID
  );

/**
  Sets the Emulation Setting.

  This function abstracts the setting of the EmulationSetting pointer.
  We may have implementation specific ways to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of EmulationSetting pointer to store.

  @return     None

**/
VOID
EFIAPI
SetEmulationSettingPointer (
  IN VOID *EmulationSetting
  );

/**
  Gets MrcPerformanceData pointer.

  This function abstracts the retrieval for the MrcPerformanceData pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance.

  @param    None

  @return   Pointer to MrcPerformanceData

**/
VOID*
EFIAPI
GetMrcPerformanceDataPointer (
  VOID
  );

/**
  Sets the MrcPerformanceData pointer.

  This function abstracts the setting of the MrcPerformanceData pointer.
  We may have implementation specific ways to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The MrcPerformanceData pointer to store.

  @return     None

**/
VOID
EFIAPI
SetMrcPerformanceDataPointer (
  IN VOID *MrcPerformanceData
  );

/**
  Gets Ddr5LrdimmTrainingData pointer.

  This function abstracts the retrieval for the Ddr5LrdimmTrainingData pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance.

  @param    None

  @return   Pointer to Ddr5LRdimmTrainingData

**/
VOID*
EFIAPI
GetDdr5LrdimmTrainingDataPointer (
  VOID
  );

/**
  Sets the Ddr5LrdimmTrainingData pointer.

  This function abstracts the setting of the Ddr5LrdimmTrainingData pointer.
  We may have implementation specific ways to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The DdrTrainingData pointer to store.

  @return     None

**/
VOID
EFIAPI
SetDdr5LrdimmTrainingDataPointer (
  IN VOID *Ddr5LrdimmTrainingData
  );

#endif  // _SYS_HOST_POINTER_LIB_H_
