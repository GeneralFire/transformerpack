/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2017 Intel Corporation. <BR>

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

#ifndef  _CPU_CSR_ACCESS_H_
#define  _CPU_CSR_ACCESS_H_

#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/CpuCsrAccess.h>
#include <Protocol/IioUds.h>
#include <Protocol/SmmBase2.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MemoryCoreLib.h>
#include <Chip/Include/CpuPciAccess.h>

UINT64 EFIAPI GetCpuCsrAddress (
  UINT8 SocId,
  UINT8 BoxInst,
  UINT32 Offset,
  UINT8* Size
  );

UINT32 EFIAPI ReadCpuCsr (
  UINT8 SocId,
  UINT8 BoxInst,
  UINT32 Offset
  );

VOID EFIAPI WriteCpuCsr (
  UINT8 SocId,
  UINT8 BoxInst,
  UINT32 Offset,
  UINT32 Data
  );

UINT32 EFIAPI ReadMcCpuCsr (
  UINT8 SocId,
  UINT8 McId,
  UINT32 Offset
  );

VOID EFIAPI WriteMcCpuCsr (
  UINT8 SocId,
  UINT8 McId,
  UINT32 Offset,
  UINT32 Data
  );

UINTN
EFIAPI
GetMcCpuCsrAddress (
  UINT8    SocId,
  UINT8    McId,
  UINT32   Offset
  );

UINT32 EFIAPI ReadPciCsr (
  UINT8 socket,
  UINT32 reg
  );

VOID EFIAPI WritePciCsr (
  UINT8 socket,
  UINT32 reg,
  UINT32 data
  );

UINT32 EFIAPI GetPciCsrAddress (
  UINT8 socket,
  UINT32 reg
  );

VOID EFIAPI BreakAtCheckpoint (
  UINT8    Socket,
  UINT8    majorCode,
  UINT8    minorCode,
  UINT16   data
  );

UINT32 EFIAPI Bios2PcodeMailBoxWrite (
  UINT8  socket,
  UINT32 command,
  UINT32 data
  );

UINT64 EFIAPI Bios2VcodeMailBoxWrite (
  UINT8  socket,
  UINT32 command,
  UINT32 data
  );

#endif   // _CPU_CSR_ACCESS_H_
