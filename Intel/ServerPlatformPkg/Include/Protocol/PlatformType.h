/** @file
  This file defines platform policies for Platform Type.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2005 Intel Corporation. <BR>

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

#ifndef _PLATFORM_TYPE_H_
#define _PLATFORM_TYPE_H_

#include <Guid/PlatformInfo.h>

typedef struct _EFI_PLATFORM_TYPE_PROTOCOL {
  UINT8                       SystemUuid[16];     // 16 bytes
  UINT32                      Signature;          // "$PIT" 0x54495024
  UINT32                      Size;               // Size of the table
  UINT16                      Revision;           // Revision of the table
  UINT16                      Type;               // Platform Type
  UINT32                      CpuType;            // Cpu Type
  UINT8                       CpuStepping;        // Cpu Stepping
  UINT32                      TypeRevisionId;     // Board Revision ID
  UINT16                      IioSku;
  UINT16                      IioRevision;
  UINT16                      PchSku;
  UINT16                      PchRevision;
  UINT16                      PchType;            // Retrive PCH SKU type installed
  BOOLEAN                     ExtendedInfoValid;  // If TRUE then below fields are Valid
  UINT8                       Checksum;           // Checksum minus SystemUuid is valid in DXE only.
  UINT64                      TypeStringPtr;
  UINT64                      IioStringPtr;
  UINT64                      PchStringPtr;
  EFI_PLATFORM_PCI_DATA       PciData;
  EFI_PLATFORM_CPU_DATA       CpuData;
  EFI_PLATFORM_MEM_DATA       MemData;
  EFI_PLATFORM_SYS_DATA       SysData;
  EFI_PLATFORM_PCH_DATA       PchData;
  UINT8                       IioRiserId;
  UINT8                       BoardId;
  UINT8                       PcieRiser1Type;
  UINT8                       PcieRiser2Type;
  UINT8                       Emulation;         // 100b = Simics
} EFI_PLATFORM_TYPE_PROTOCOL;

extern EFI_GUID gEfiPlatformTypeProtocolGuid;

#endif
