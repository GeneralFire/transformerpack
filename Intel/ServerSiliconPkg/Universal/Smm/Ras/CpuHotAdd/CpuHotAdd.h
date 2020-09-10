/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2020 Intel Corporation. <BR>

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

#ifndef _CPU_HOT_ADD_H_
#define _CPU_HOT_ADD_H_

#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/LocalApicLib.h>
#include <Library/TimerLib.h>
#include <Library/MtrrLib.h>
#include <Library/CpuPpmLib.h>
#include <Library/SynchronizationLib.h>
#include <Protocol/MpService.h>
#include <Protocol/CpuPpmProtocol.h>
#include <Library/CpuConfigLib.h>
#include <Protocol/CpuHotAddData.h>
#include <Protocol/PpmPolicyProtocol.h>
#include <Guid/AcpiVariable.h>
#include <Register/ArchitecturalMsr.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Register/Cpuid.h>
#include "UncoreCommonIncludes.h"
#include <Register/Microcode.h>

#ifndef EFI_DEADLOOP
 #define EFI_DEADLOOP()    { volatile int __iii; __iii = 1; while (__iii); }
#endif

#define INVALID_APIC_ID 0xFFFFFFFFFFFFFFFF



#define IA32_PG_P                   1u
#define IA32_PG_RW                  (1u << 1)
#define IA32_PG_USR                 (1u << 2)
#define IA32_PG_WT                  (1u << 3)
#define IA32_PG_CD                  (1u << 4)
#define IA32_PG_A                   (1u << 5)
#define IA32_PG_D                   (1u << 6)
#define IA32_PG_PS                  (1u << 7)
#define IA32_PG_G                   (1u << 8)
#define IA32_PG_PAT_2M              (1u << 12)
#define IA32_PG_PAT_4K              IA32_PG_PS


//
// SMM defines
//

#define SMM_DEFAULT_SMBASE          0x30000
#define SMM_HANDLER_OFFSET          0x8000
#define SMM_PSD_OFFSET              0xfb00
#define SMM_CPU_STATE_OFFSET        0xfc00

#define TRIGGER_MODE_EDGE                     0x00
#define TRIGGER_MODE_LEVEL                    0x01

#define BROADCAST_MODE_SPECIFY_CPU            0x00
#define BROADCAST_MODE_ALL_INCLUDING_SELF     0x01
#define BROADCAST_MODE_ALL_EXCLUDING_SELF     0x02

// CPU save-state strcuture for IA32.
typedef struct {
  UINT8   Reserved[0x200];  // fc00h
  UINT8   Reserved1[0xf8];  // fe00h
  UINT32  SMBASE;           // fef8h
  UINT32  SMMRevId;         // fefch
  UINT16  IORestart;        // ff00h
  UINT16  AutoHALTRestart;  // ff02h
  UINT32  IEDBASE;          // ff04h
  UINT8   Reserved2[0x98];  // ff08h
  UINT32  IOMemAddr;        // ffa0h
  UINT32  IOMisc;           // ffa4h
  UINT32  _ES;
  UINT32  _CS;
  UINT32  _SS;
  UINT32  _DS;
  UINT32  _FS;
  UINT32  _GS;
  UINT32  Reserved3;
  UINT32  _TR;
  UINT32  _DR7;
  UINT32  _DR6;
  UINT32  _EAX;
  UINT32  _ECX;
  UINT32  _EDX;
  UINT32  _EBX;
  UINT32  _ESP;
  UINT32  _EBP;
  UINT32  _ESI;
  UINT32  _EDI;
  UINT32  _EIP;
  UINT32  _EFLAGS;
  UINT32  _CR3;
  UINT32  _CR0;
} SOCKET_LGA_775_SMM_CPU_STATE32;


// CPU save-state strcuture for X64.
typedef struct {
  UINT8   Reserved1[0x1d0];  // fc00h
  UINT32  GdtBaseHiDword;    // fdd0h
  UINT32  LdtBaseHiDword;    // fdd4h
  UINT32  IdtBaseHiDword;    // fdd8h
  UINT8   Reserved2[0xc];    // fddch
  UINT64  IO_EIP;            // fde8h
  UINT8   Reserved3[0x50];   // fdf0h
  UINT32  _CR4;              // fe40h
  UINT8   Reserved4[0x48];   // fe44h
  UINT32  GdtBaseLoDword;    // fe8ch
  UINT32  GdtLimit;          // fe90h
  UINT32  IdtBaseLoDword;    // fe94h
  UINT32  IdtLimit;          // fe98h
  UINT32  LdtBaseLoDword;    // fe9ch
  UINT32  LdtLimit;          // fea0h
  UINT32  LdtInfo;           // fea4h
  UINT8   Reserved5[0x50];   // fea8h
  UINT32  SMBASE;            // fef8h
  UINT32  SMMRevId;          // fefch
  UINT16  IORestart;         // ff00h
  UINT16  AutoHALTRestart;   // ff02h
  UINT32  IEDBASE;           // ff04h
  UINT8   Reserved6[0x14];   // ff08h
  UINT64  _R15;              // ff1ch
  UINT64  _R14;
  UINT64  _R13;
  UINT64  _R12;
  UINT64  _R11;
  UINT64  _R10;
  UINT64  _R9;
  UINT64  _R8;
  UINT64  _RAX;              // ff5ch
  UINT64  _RCX;
  UINT64  _RDX;
  UINT64  _RBX;
  UINT64  _RSP;
  UINT64  _RBP;
  UINT64  _RSI;
  UINT64  _RDI;
  UINT64  IOMemAddr;         // ff9ch
  UINT32  IOMisc;            // ffa4h
  UINT32  _ES;               // ffa8h
  UINT32  _CS;
  UINT32  _SS;
  UINT32  _DS;
  UINT32  _FS;
  UINT32  _GS;
  UINT32  _LDTR;             // ffc0h
  UINT32  _TR;
  UINT64  _DR7;              // ffc8h
  UINT64  _DR6;
  UINT64  _RIP;              // ffd8h
  UINT64  IA32_EFER;         // ffe0h
  UINT64  _RFLAGS;           // ffe8h
  UINT64  _CR3;              // fff0h
  UINT64  _CR0;              // fff8h
} SOCKET_LGA_775_SMM_CPU_STATE64;


// Union of CPU save-state strcutures for IA32 and X64.
typedef union  {
  SOCKET_LGA_775_SMM_CPU_STATE32  x86;
  SOCKET_LGA_775_SMM_CPU_STATE64  x64;
} SOCKET_LGA_775_SMM_CPU_STATE;

//
//  CPU HotAdd Driver defines
//

typedef struct {
  UINT32            BufferStart;
  UINT32            Cr3;
  IA32_DESCRIPTOR   GdtrProfile;
  IA32_DESCRIPTOR   IdtrProfile;
  HOT_ADD_CPU_EXCHANGE_INFO  *MpExchangeInfoPtr;
} CPU_INIT_INFO;

VOID
InitialProcessorInit(
);

VOID
LoadMicrocodeEx (
  );

VOID
SmmInitializeSmrrEmrr (
  );

VOID
AsmRsm (
  VOID
  );

VOID
AsmAcquireMPLock (
  IN   UINT8            *Lock
  );


VOID
AsmReleaseMPLock (
  IN   UINT8            *Lock
  );

UINT32
Gen4GPageTable (
  VOID
  );

VOID PpmCsrProgramming (
  UINT8   SktId
  );

VOID PpmMsrProgramming (
  EFI_CPU_PM_STRUCT *PpmStruct,
  PPM_POLICY_CONFIGURATION *PpmPolicy
  );

VOID CpuMiscMsrProgramming (
  );

#ifdef LT_FLAG


#define  LT_SPAD_HIGH_VALUE        *(volatile UINT32 *)(UINTN)(0xFED300A4)
#define  LT_SPAD_SET_HIGH_VALUE    *(volatile UINT32 *)(UINTN)(0xFED206A4)
#define  LT_EXIST_VALUE            *(volatile UINT32 *)(UINTN)(0xFED30010)
#define  LT_JOIN_VALUE             *(volatile UINT32 *)(UINTN)(0xFED30020)
#define  LT_SPAD_TXT_CPU_HOT_PLUG  BIT29
#define  LT_LOCK_CONFIG            0x06


#define  IIO_DFX_LCK_GLOBAL        0x8006F504

VOID
LTSXHotPlug (
  VOID
  );

VOID
ProgramVirtualWireMode_RAS (
  BOOLEAN       BSP,
  UINT32        VirtualWireMode
  );

EFI_STATUS
SetApicBSPBit (
  IN  BOOLEAN   Enable
  );



EFI_STATUS
SendInterrupt (
  IN  UINT32                               BroadcastMode,
  IN  UINT32                               ApicID,
  IN  UINT32                               VectorNumber,
  IN  UINT32                               DeliveryMode,
  IN  UINT32                               TriggerMode,
  IN  BOOLEAN                              Assert
  );

VOID
LaunchBiosAcm (
  IN      UINTN                  AcmBase,
  IN      UINTN                  EsiValue
  );

#endif  //LT_FLAG

#endif
