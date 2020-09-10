/** @file
  Implementation of Near memory cache flush.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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
**/

#ifndef _CRYSTALRIDGE_FLUSH_NEAR_MEMORY_LIB_H_
#define _CRYSTALRIDGE_FLUSH_NEAR_MEMORY_LIB_H_

#define CR_FLUSH_NM_EXT     0
#define CR_FLUSH_NM_ETR     1
#define SIG_NM_SUCCESS      0
#define SIG_NM_ERROR        1
#define SIG_NM_UNKNOWN      0xFF // For DXE null library usage only
#define PCH_SMI_STS_DISABLE 0xFFFFFFFF

typedef struct _CORE_FLUSH_TASK {
  UINT8   Socket;
  UINT64  StartingAddress;
  UINT64  EndingAddress;
  UINT16  Interleave;
  UINT8   DdrtCount;        // Number of DDRT DIMMS interleaved for this address range spans
  UINT8   DimmPos;          // The assigned DIMM in the set of DIMM that make up the range
  BOOLEAN Started;
} CORE_FLUSH_TASK;

// Callback to provide NM flush Err information
typedef
VOID
(EFIAPI *CRYSTALRIDGE_NMFLUSH_ERR_INFO) (
  VOID
  );

typedef struct _EFI_CRYSTALRIDGE_FLUSH_NEAR_MEMORY_PROTOCOL {
  struct SystemMemoryMapHob     *SystemMemoryMap;
  CORE_FLUSH_TASK               *CoreTasks;
  UINT8                         *CoreTasksCount;
  CRYSTALRIDGE_NMFLUSH_ERR_INFO CrNmFlushErrInfo;                 // OEM Call back routine to save Error records
  UINT8                         *InCrystalridgeFlushNMProtocol;   // To indicate whether in NM flush protocol or not
  UINT32                        *ErrCnt;                          // TO count number of errors durinf flush

} EFI_CRYSTALRIDGE_FLUSH_NEAR_MEMORY_PROTOCOL;

#define MAX_RANGES_PER_SOCKET (6)
#define MAX_DDRT_PER_SAD      (8)
#define DdrtTransactionSize   (64)
#define MAX_CORE_TASKS        (MAX_SOCKET * MAX_RANGES_PER_SOCKET * MAX_DDRT_PER_SAD)

#define ACQUIRE_SPIN_LOCK(Semaphore)   while (!AcquireSpinLockOrFail ((Semaphore))) { CpuPause (); }

#define WARM_RESET 0x06

// Ratio in Performance Control MSR (MSR_IA32_PERF_CTL 0xCE)
#define B_IA32_PERF_CTRLP_STATE_TARGET  (0x7F << 8)

//
// Bit in CMOS Diagnostic Status register used to keep information that NM flush is in progress.
//
#define CMOS_DIAG_STAT_REG_NMFLUSH BIT0

#define MSR_PLATFORM_INFO         0x000000CE

#define NM_FLUSH_DISABLE 0x0
#define NM_FLUSH_ENABLE  0x1

//
//  Max ARS errors supported in the platform
//
#define MAX_ERROR_LOGS_NMFLUSH    0xA00

/**

  Routine description: Flush all near memory cache lines containing AppDirect data

  @return EFI status is returned

**/
EFI_STATUS
FlushNearMemoryCache (
  VOID
  );

/**

  Routine description: This routine determines if NM Flush is needed upon S0>S5 transition or Warm reset

  @param[out] AdwbFlushTaskCount  Count the number of NM flush task needed for AppDirect-WB.

  @return EFI status is returned

**/
EFI_STATUS
DetermineNearMemoryFlushCapability (
  OUT UINTN *AdwbFlushTaskCount
  );

/**

  Routine description: This routine sets bit0 in CMOS Diagnostic Status register

  @param [IN] Value - NM flush status bit value

  @return EFI status is returned

**/
EFI_STATUS
SignalNmFlushStatusToCmosReg (
  IN UINT8 Value
  );

/**

  Routine description: This routine retrieves the bit0 value from CMOS Diagnostic Status register

  @param None

  @return NM flush status bit value

**/
UINT8
GetNmFlushStatusFromCmosReg (
VOID
  );

#endif
