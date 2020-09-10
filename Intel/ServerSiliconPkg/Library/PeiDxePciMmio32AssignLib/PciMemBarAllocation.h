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

#ifndef __PEI_PCI_MEM_ALLOCATION_HEAD__
#define __PEI_PCI_MEM_ALLOCATION_HEAD__

#include <Base.h>
#include <IndustryStandard/Pci22.h>
#include <Uefi/UefiBaseType.h>
#include <UsraAccessType.h>
#include <Pi/PiBootMode.h>
#include <Uefi/UefiMultiPhase.h>
#include <Pi/PiHob.h>
#include <Pi/PiPeiCis.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/UsraAccessApi.h>
#include <Library/PciMmio32AssignLib.h>

#include <Library/SiliconWorkaroundLib.h>

#define PCI_BAR_MASK               0xF
#define PCI_MEM_BAR_BASE_ADDR_MASK 0xFFFFFFF0

#define BAR_WRITE_ALL_ONE          0xFFFFFFFF
#define BAR_WRITE_ALL_ONE_WA       0XFFFFFFF0

#define MAX_PCI_INFO_NODE_COUNT    (MAX_SOCKET * 0x50)
#define MEMORY_MAP_TABLE_SIZE      (MAX_PCI_INFO_NODE_COUNT * 0x10)

typedef enum {
  PciBarUnknown = 0,
  PciBarMem32,
  PciBarMem64,
  PciBarMaxType
} PCI_BAR_NODE_TYPE;

typedef struct {
  UINT16             NodeIndex;
  UINT16             Seg;
  UINT8              Bus;
  UINT8              Dev;
  UINT8              Func;
  UINT16             Offset;
  PCI_BAR_NODE_TYPE  BarType;
  UINT32             MmioBase;
  UINT32             MmioSize;
} PCI_BAR_INFO;

typedef struct {
  EFI_HOB_GUID_TYPE  EfiHobGuidType;
  UINT32             Count;
  BOOLEAN            BarIsCleaned;
  UINT64             MemoryMapRecordTablePtr;
  PCI_BAR_INFO       PciBarInfo[MAX_PCI_INFO_NODE_COUNT];
} MMIO_ALLOCATION_INFO_HOB;

typedef struct {
  UINT8              CpuType;
  UINT8              CpuRevision;
  UINT16             Vid;
  UINT16             Did;
  UINT8              BarOffset;
  UINT32             BarAlignment;
} INCOMPATIBLE_PCIE_BAR;

//
// Constants used only by WA for OOB-MSM bar issue
//
#ifdef SPR_HOST
#define OOB_MSM_DEV       3
#define OOB_MSM_FUNC      0
#else
#define OOB_MSM_DEV       2
#define OOB_MSM_FUNC      0
#endif

#define OOB_MSM_FUNC_DFX  2
#define OOB_MSM_FUNC_PMON 1

#define OOB_MSM_VID         0x8086
#define OOB_MSM_DID         0x09A7
#define OOB_MSM_DID_NEW     0x09A8

#endif  //__PEI_PCI_MEM_ALLOCATION_HEAD__

