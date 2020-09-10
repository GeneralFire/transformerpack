/** @file
  Register Access includes

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef __REGISTER_ACCESS_INCLUDES__
#define __REGISTER_ACCESS_INCLUDES__

#include <PiPei.h>

#define MAX_CHUNK_SIZE 16
#define MAX_CHUNKS 4
#define RAT_TOPOLOGY_VERSION_MAJOR 0x1
#define RAT_TOPOLOGY_VERSION_MINOR 0x3
#define RAT_TOPOLOGY_VERSION ((RAT_TOPOLOGY_VERSION_MAJOR << 4) | RAT_TOPOLOGY_VERSION_MINOR)

#define SWAPPED_BYTE_ORDER // Byte orders for UINTs in map file are reversed
#define SMALL_INSTANCE_COUNT // Instance counts in map file are UINT8

#ifdef SWAPPED_BYTE_ORDER
  #define Swap16(x) SwapBytes16(x)
  #define Swap32(x) SwapBytes32(x)
  #define Swap64(x) SwapBytes64(x)
#else
  #define Swap16(x) x
  #define Swap32(x) x
  #define Swap64(x) x
#endif

#pragma pack(1)

typedef enum {
  PciCfg,
  FixedMemory,
  MemBar,
  MAX_ACCESSOR
} ACCESSOR_TYPES;

typedef struct _BLANK_ACCESSOR {
  UINT32 Reserved1;
  UINT32 Reserved2;
  UINT32 Reserved3;
  UINT32 Reserved4;
  UINT32 Reserved5;
  UINT32 Reserved6;
  UINT32 Reserved7;
  UINT32 Reserved8;
} BLANK_ACCESSOR;

typedef struct _PCI_BDFO {
  UINT32 Reserved:4;
  UINT32 Bus:8;
  UINT32 Device:5;
  UINT32 Function:3;
  UINT32 Offset:12;
} PCI_BDFO;

typedef union _PCI_CFG_INFO {
  PCI_BDFO Bdfo;
  UINT32 Raw;
} PCI_CFG_INFO;

typedef struct _PCI_CFG_ACCESSOR {
  UINT32 Reserved1;
  UINT32 Reserved2;
  UINT32 Reserved3;
  UINT32 Reserved4;
  UINT32 Reserved5;
  UINT32 Reserved6;
  UINT32 Reserved7;
  PCI_CFG_INFO Info;
} PCI_CFG_ACCESSOR;

typedef struct _FIXED_MEMORY_ACCESSOR {
  UINT32 Reserved1;
  UINT32 Reserved2;
  UINT32 Reserved3;
  UINT32 Reserved4;
  UINT32 Reserved5;
  UINT32 Reserved6;
  UINT32 Reserved7;
  UINT32 Address;
} FIXED_MEMORY_ACCESSOR;

typedef union _MEM_BAR_BITS {
  UINT32 Reserved:21;
  UINT32 Shift2:5;
  UINT32 Combine:1;
  UINT32 Shift1:5;
} MEM_BAR_BITS;

typedef union _MEM_BAR_COMBINATION {
  MEM_BAR_BITS Bits;
  UINT32 Raw;
} MEM_BAR_COMBINATION;

typedef struct _MEM_BAR_ACCESSOR {
  UINT32 Reserved1;
  UINT32 Reserved2;
  UINT32 Offset;
  UINT32 Mask2;
  PCI_CFG_INFO Reg2;
  UINT32 Mask1;
  PCI_CFG_INFO Reg1;
  MEM_BAR_COMBINATION Combine;
} MEM_BAR_ACCESSOR;

typedef union _ACCESSOR_DESCRIPTOR {
  PCI_CFG_ACCESSOR PciCfg;
  FIXED_MEMORY_ACCESSOR FixedMemory;
  MEM_BAR_ACCESSOR MemBar;
  BLANK_ACCESSOR Raw;
} ACCESSOR_DESCRIPTOR;

typedef struct _IP_DESCRIPTOR {
  UINT64               Id;
#ifdef SMALL_INSTANCE_COUNT
  UINT8                InstanceCount;
#else
  UINT16               InstanceCount;
#endif
  UINT8                AccessorBitmap;
} IP_DESCRIPTOR;

typedef struct _SOC_DESCRIPTOR {
  UINT8                *Chunks;
  UINT8                 ChunkLevelCount;
  UINT16                ChunkLevelStarts[MAX_CHUNKS];
  UINT8                 AccessorSize;
  UINT16                IpCount;
  IP_DESCRIPTOR       **Ips;
} SOC_DESCRIPTOR;

typedef struct _SYSTEM_TOPOLOGY {
  UINT8            StructureVersion;
  UINT8            SocketCount;
  UINT8            BusShiftPerSocket;
  SOC_DESCRIPTOR  *Soc;
} SYSTEM_TOPOLOGY;

#pragma pack()

#endif // __REGISTER_ACCESS_INCLUDES__

