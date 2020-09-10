/** @file
  System Infor Var Hearder File

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

#ifndef __SYSTEM_INFO_VAR_INCLUDES__
#define __SYSTEM_INFO_VAR_INCLUDES__

#include <MemCommon.h>
#include <Upi/KtiSi.h>

#define SYS_INFO_NVRAM_VAR_NAME L"InfoVarNvramData"

#define POSTED_METHOD_DISABLED  0
#define POSTED_METHOD_ENABLED   1

#pragma pack (push,1)

typedef enum BootMode {
  NormalBoot  = 0,  // Normal path through RC with full init, mem detection, init, training, etc.
                    //    Some of these MRC specific init routines can be skipped based on MRC input params
                    //    in addition to the sub-boot type (WarmBoot, WarmBootFast, etc).
  S3Resume    = 1   // S3 flow through RC. Should do the bare minimum required for S3
                    //    init and be optimized for speed.
} BootMode;

//
// This is used to determine what type of die is connected to a UPI link
//
typedef enum {
  UpiConnectionTypeCpu,
  UpiConnectionTypePcieGen4,
  UpiConnectionTypeFpga,
  UpiConnectionTypeMax
} UPI_CONNECTION_TYPE;

typedef struct {
  UINT16 stackPresentBitmap[MAX_SOCKET];           ///< bitmap of present stacks per socket
  UINT8  StackBus[MAX_SOCKET][MAX_LOGIC_IIO_STACK];///< Bus of each stack
  UINT32 StackMmiol[MAX_SOCKET][MAX_IIO_STACK];    ///< mmiol of each IIO stack, if it works as CXL, the mmiol base is RCRBBAR
  UINT8  SocketFirstBus[MAX_SOCKET];
  UINT8  Socket10nmUboxBus0[MAX_SOCKET];           //10nm CPU use only
  UINT8  SocketLastBus[MAX_SOCKET];
  UINT8  segmentSocket[MAX_SOCKET];
  UINT8  KtiPortCnt;
  UINT32 socketPresentBitMap;
  UINT32 SlaveDieBitMap;
  UINT32 FpgaPresentBitMap;
  UINT32 mmCfgBase;
  UINT32 mmCfgBaseH[MAX_SOCKET];
  UINT32 mmCfgBaseL[MAX_SOCKET];
  UINT8  DdrMaxCh;
  UINT8  DdrMaxImc;
  UINT8  DdrNumChPerMc;
  UINT8  imcEnabled[MAX_SOCKET][MAX_IMC];
  UINT8  mcId[MAX_SOCKET][MAX_CH];
  MRC_MST MemSsType[MAX_SOCKET];                    ///< MemSsType global identifier for DDR vs. HBM
  UINT32  MmioBar[MAX_SOCKET][TYPE_MAX_MMIO_BAR];
  UINT8   HbmMaxCh;
  UINT8   HbmMaxIoInst;
  UINT8   HbmNumChPerMc;
  UINT8   HbmNumChPerIo;
  UINT32  LastCsrAddress[2];
  UINT32  LastCsrMmioAddr;
  UINT8   CsrCachingEnable;
  UINT32  LastCsrMcAddress[2];
  UINT32  LastCsrMcMmioPhyAddr;
  UINT8   CsrPciBarCachingEnable;
  UINT32  LastCsrPciBarAddr[2];
  UINT64  LastCsrPciBarPhyAddr;
  UINT32  LastSBPortId[MAX_SOCKET];
  UPI_CONNECTION_TYPE         UpiConnectionType[MAX_SOCKET];
  BOOLEAN PostedCsrAccessAllowed;                   // SW is allowed to use posted CSR writes method when TRUE
  BOOLEAN PostedWritesEnabled;                      // All CSR writes use posted method when TRUE, non-posted when FALSE
  BOOLEAN DataPopulated;                            // CPU_CSR_ACCESS_VAR is unavailable when FALSE
  BOOLEAN HbmSku;
  UINT8   SocketConfig;
  UINT8   HcxType[MAX_SOCKET];
} CPU_CSR_ACCESS_VAR;

typedef struct {
  UINT32 MeRequestedSizeNv;
  UINT32 MeRequestedAlignmentNv;
  UINT32 IeRequestedSizeNv;
  UINT32 IeRequestedAlignmentNv;
  UINT8  SbspSocketIdNv;
} SYS_INFO_VAR_NVRAM;

typedef struct {
  UINT32 MeRequestedSize;
  UINT32 MeRequestedAlignment;
  UINT32 IeRequestedSize;
  UINT32 IeRequestedAlignment;
  UINT32 CheckPoint;
  UINT8  ResetRequired;
  UINT8  Emulation;
  BootMode SysBootMode;
  CPU_CSR_ACCESS_VAR        CpuCsrAccessVarHost;    // Common resource for CsrAccessRoutines
  UINT64   CpuFreq;
  UINT8    SocketId;
  SYS_INFO_VAR_NVRAM SysInfoVarNvram;
  BOOLEAN  UsraTraceControl;
  UINT16   UsraTraceConfiguration;                 // Bit               7       6        5           4                     3:0
                                                   //               Write, Modify, GetAddr, Long/Short, Trace 0x5 as signature
                                                   // Bit               15                              14:11   10     9     8
                                                   // DumpCpuCsrAccessVar,                                TBD, CSR, PCIE, Read
  BOOLEAN  CpuCsrAccessVarInfoDumped;
  UINTN    UsraPpiPtr;
} SYSTEM_INFO_VAR;

#pragma pack (pop)

#endif //#ifndef __SYSTEM_INFO_VAR_INCLUDES__
