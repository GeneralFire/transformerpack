/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2012 Intel Corporation. <BR>

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

#ifndef _EFI_PLATFORM_TXT_H_
#define _EFI_PLATFORM_TXT_H_

#include <Uefi.h>


extern EFI_GUID gEfiPlatformTxtDeviceMemoryGuid;

extern EFI_GUID gEfiPlatformTxtPolicyDataGuid;

//
// TXT Device Memory
//
// Platform MUST expose this Hob and Size field should be page-aligned non-zero.
// If Address field is not zero, TXT driver will use it,
// or TXT driver will allocate 1M aligned memory for it.
// NOTE: DMA protection memory region address top is just below TSEG. Zero-Address
// means no DPR support in platform.
//
typedef struct {
  EFI_PHYSICAL_ADDRESS     NoDMATableAddress;
  UINT64                   NoDMATableSize;
  EFI_PHYSICAL_ADDRESS     SINITMemoryAddress;
  UINT64                   SINITMemorySize;
  EFI_PHYSICAL_ADDRESS     TXTHeapMemoryAddress;
  UINT64                   TXTHeapMemorySize;
  EFI_PHYSICAL_ADDRESS     DMAProtectionMemoryRegionAddress;
  UINT64                   DMAProtectionMemoryRegionSize;
} EFI_PLATFORM_TXT_DEVICE_MEMORY;

//
// Policy
//
// Platform MUST expose this Hob. BIOSAcm field should not be zero.
//
typedef struct {
  //
  // ACM information
  //
  EFI_PHYSICAL_ADDRESS     BiosAcmAddress;
  EFI_PHYSICAL_ADDRESS     StartupAcmAddressInFit;
  EFI_PHYSICAL_ADDRESS     StartupAcmAddress;
  //
  // Misc Policy
  //
  UINT32                   MiscPolicy;
  //
  // BiosOs region. It should be at least 2 for LCP PD.
  //
  UINT32                   BiosOsDataRegionRevision;
  EFI_PHYSICAL_ADDRESS     LcpPolicyDataBase;
  UINT64                   LcpPolicyDataSize;
  //
  // 64-bits TXT Scratch Register
  //
  EFI_PHYSICAL_ADDRESS     TxtScratchAddress;
  //
  // Bios ACM Policy
  //
  UINT32                   BiosAcmPolicy;
  //
  // BiosOsData (SINIT consumption, chipset specific)
  //
  UINT64                   FlagVariable;
  //
  // DPR Lock
  //
//  UINT32                   DprLock;
  //
  // Clear TPM Aux Index
  //
  UINT8                    TxtClearAuxIndex;
} EFI_PLATFORM_TXT_POLICY_DATA;

//
// BIOS ACM policy
//

//
// Misc policy
//

//
// SCHECK should be performed in TXT BIOS spec 1.0.
// However, some platform does not need this.
//
#define EFI_PLATFORM_TXT_POLICY_MISC_NO_SCHECK_IN_RESUME 0x1
//
// Some platform need not programming NODMA table, it uses
// chipset-specific DMA protection memory region.
// If this flag is set, no NODMA register will not be touched.
//
#define EFI_PLATFORM_TXT_POLICY_MISC_NO_NODMA_TABLE      0x2

#pragma pack (push, 1)
typedef struct {
  UINT32                   Revision;
  UINT32                   BiosSinitSize;
} EFI_TXT_BIOS_OS_DATA_REGION_V1;

typedef struct {
  UINT32                   Revision;
  UINT32                   BiosSinitSize;
  UINT64                   LcpPolicyDataBase;
  UINT64                   LcpPolicyDataSize;
  UINT32                   NumberOfLogicalProcessors;
  UINT64                   FlagVariable;
} EFI_TXT_BIOS_OS_DATA_REGION_V3;

typedef struct {
  UINT32                   Revision;
  UINT32                   BiosSinitSize;
  UINT64                   LcpPolicyDataBase;
  UINT64                   LcpPolicyDataSize;

  UINT32                   NumberOfLogicalProcessors;
} EFI_TXT_BIOS_OS_DATA_REGION_V2;
#pragma pack (pop)
#endif
