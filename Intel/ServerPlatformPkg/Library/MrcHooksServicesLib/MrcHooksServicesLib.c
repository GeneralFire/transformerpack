/** @file
  This library class provides MRC Hooks Services.

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

#include <Uefi.h>
#include <PiPei.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiDecompressLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/ProcMemInitChipLib.h>

#include "SysHost.h"
#include "SysRegs.h"
#include <RcRegs.h>

#include <Ppi/MrcHooksServicesPpi.h>
#include <Ppi/MrcHooksChipServicesPpi.h>
#include <Library/SysHostPointerLib.h>
#include <Library/CsrAccessLib.h>

/**

  MRC wrapper code.
  Check if Mrc Hooks Services PPI are initialized

  @param[in] Host  Pointer to sysHost structure

**/
BOOLEAN
IsMrcHooksServicesPpiReady (
  IN PSYSHOST Host
  )
{
  BOOLEAN Result = FALSE;

  if ((Host != NULL) && (Host->MrcHooksServicesPpi != 0) && (Host->MrcHooksChipServicesPpi != 0)) {
    Result = TRUE;
  } else {
    DEBUG ((DEBUG_ERROR, "MrcHooksServicesPpiWrapper used before available\n"));
  }
  return Result;
}

/**
  LogWarning() will always log warning details to the legacy warning log

  @param majorCode = Major warning code
  @param minorCode = Minor warning code
  @param logData   = (Socket << 24) + (Channel << 16) + (Dimm << 8) + (Rank)
**/
VOID
EFIAPI
LogWarning (
  UINT8    majorCode,
  UINT8    minorCode,
  UINT32   logData
  )
{
  PSYSHOST Host;
  MRC_HOOKS_SERVICES_PPI *MrcHooksServicesPpi;

  Host = GetSysHostPointer ();

  MrcHooksServicesPpi = (MRC_HOOKS_SERVICES_PPI *)Host->MrcHooksServicesPpi;

  if (IsMrcHooksServicesPpiReady (Host)) {
    MrcHooksServicesPpi->LogWarning (majorCode, minorCode, logData);
  }
}


/**
  OutputWarning() will always log warning to Enhanced Warning Log if possible.
  If EWL is not defined, it will log warning details to legacy warning log.

  @param majorCode = Major warning code
  @param minorCode = Minor warning code
  @param logData   = (Socket << 24) + (Channel << 16) + (Dimm << 8) + (Rank)
**/

VOID
EFIAPI
OutputWarning (
  UINT8    majorCode,
  UINT8    minorCode,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank
  )
{
  PSYSHOST Host;
  MRC_HOOKS_SERVICES_PPI *MrcHooksServicesPpi;

  Host = GetSysHostPointer ();

  MrcHooksServicesPpi = (MRC_HOOKS_SERVICES_PPI *)Host->MrcHooksServicesPpi;

  if (IsMrcHooksServicesPpiReady (Host)) {
    MrcHooksServicesPpi->OutputWarning (majorCode, minorCode, socket, ch, dimm, rank);
  }
}

UINT32
EFIAPI
SetPromoteWarningException (
  UINT8    majorCode,
  UINT8    minorCode
  )
{
  PSYSHOST Host;
  MRC_HOOKS_SERVICES_PPI *MrcHooksServicesPpi;

  Host = GetSysHostPointer ();

  MrcHooksServicesPpi = (MRC_HOOKS_SERVICES_PPI *)Host->MrcHooksServicesPpi;

  UINT32 ReturnValue = 0;

  if (IsMrcHooksServicesPpiReady (Host)) {
    ReturnValue = MrcHooksServicesPpi->SetPromoteWarningException (majorCode, minorCode);
  }

  // ASSERT if number of exceptions is over the limit
  ASSERT (ReturnValue == 0);

  return ReturnValue;
}

EFI_STATUS
EFIAPI
ReadSmb (
  PSYSHOST          Host,
  SMB_DEVICE_STRUCT Dev,
  UINT8             ByteOffset,
  volatile UINT8    *Data
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  MRC_HOOKS_SERVICES_PPI *MrcHooksServicesPpi;

  MrcHooksServicesPpi = (MRC_HOOKS_SERVICES_PPI *)Host->MrcHooksServicesPpi;

  if (IsMrcHooksServicesPpiReady (Host)) {
    Status = MrcHooksServicesPpi->ReadSmb (Dev, ByteOffset, Data);
  }

  return Status;
}

EFI_STATUS
EFIAPI
WriteSmb (
  PSYSHOST          Host,
  SMB_DEVICE_STRUCT Dev,
  UINT8             ByteOffset,
  UINT8             *Data
  )
{
  EFI_STATUS  Status = 0;
  MRC_HOOKS_SERVICES_PPI *MrcHooksServicesPpi;

  MrcHooksServicesPpi = (MRC_HOOKS_SERVICES_PPI *)Host->MrcHooksServicesPpi;

  if (IsMrcHooksServicesPpiReady (Host)) {
    Status = MrcHooksServicesPpi->WriteSmb (Dev, ByteOffset, Data);
  }

  return Status;
}

UINT32
EFIAPI
DiscoverUsb2DebugPort (
  PSYSHOST Host
  )
{
  UINT32 ReturnValue = 0;
  MRC_HOOKS_SERVICES_PPI *MrcHooksServicesPpi;

  MrcHooksServicesPpi = (MRC_HOOKS_SERVICES_PPI *)Host->MrcHooksServicesPpi;

  if (IsMrcHooksServicesPpiReady (Host)) {
    ReturnValue = MrcHooksServicesPpi->DiscoverUsb2DebugPort (Host);
  }

  return ReturnValue;
}

UINT32
EFIAPI
InitializeUsb2DebugPort (
  PSYSHOST Host
  )
{
  UINT32 ReturnValue = 0;
  MRC_HOOKS_SERVICES_PPI *MrcHooksServicesPpi;

  MrcHooksServicesPpi = (MRC_HOOKS_SERVICES_PPI *)Host->MrcHooksServicesPpi;

  if (IsMrcHooksServicesPpiReady (Host)) {
    ReturnValue = MrcHooksServicesPpi->InitializeUsb2DebugPort (Host);
  }

  return ReturnValue;

}

VOID
EFIAPI
FatalError (
  UINT8    majorCode,
  UINT8    minorCode
  )
{
  PSYSHOST Host;
  MRC_HOOKS_CHIP_SERVICES_PPI *MrcHooksChipServicesPpi;

  Host = GetSysHostPointer ();

  MrcHooksChipServicesPpi = (MRC_HOOKS_CHIP_SERVICES_PPI *)Host->MrcHooksChipServicesPpi;

  if (IsMrcHooksServicesPpiReady (Host)) {
    MrcHooksChipServicesPpi->FatalError (majorCode, minorCode);
  }
}

VOID
WriteMsrPipeHL (
  UINT8         socket,
  UINT32        msr,
  UINT64_STRUCT msrReg
  )
{

  PSYSHOST Host;
  MRC_HOOKS_SERVICES_PPI *MrcHooksServicesPpi;

  Host = GetSysHostPointer ();

  MrcHooksServicesPpi = (MRC_HOOKS_SERVICES_PPI *)Host->MrcHooksServicesPpi;

  if (IsMrcHooksServicesPpiReady (Host)) {
    MrcHooksServicesPpi->WriteMsrPipe (socket, msr, msrReg);
  }
}

VOID
EFIAPI
OutputCheckpoint (
  UINT8    majorCode,
  UINT8    minorCode,
  UINT16   data
  )
{

  PSYSHOST Host;
  MRC_HOOKS_SERVICES_PPI *MrcHooksServicesPpi;

  Host = GetSysHostPointer ();

  MrcHooksServicesPpi = (MRC_HOOKS_SERVICES_PPI *)Host->MrcHooksServicesPpi;

  if (IsMrcHooksServicesPpiReady (Host)) {
    MrcHooksServicesPpi->OutputCheckpoint (majorCode, minorCode, data);
  }
}

KTI_STATUS
EFIAPI
SendMailBoxCmdToPcode (
  IN PSYSHOST  Host,
  IN UINT8     Cpu,
  IN UINT32    Command,
  IN UINT32    Data
  )
{
  KTI_STATUS ReturnValue = 0;
  MRC_HOOKS_CHIP_SERVICES_PPI *MrcHooksChipServicesPpi;

  MrcHooksChipServicesPpi = (MRC_HOOKS_CHIP_SERVICES_PPI *)Host->MrcHooksChipServicesPpi;

  if (IsMrcHooksServicesPpiReady (Host)) {
    ReturnValue = MrcHooksChipServicesPpi->SendMailBoxCmdToPcode (Cpu, Command, Data);
  }

  return ReturnValue;
}

/**

  Read SPD byte from Serial EEPROM

  @param Host       - Pointer to sysHost
  @param socket     - Socket ID
  @param ch         - Channel on socket
  @param dimm       - DIMM on channel
  @param byteOffset - Byte Offset to read
  @param data       - Pointer to data

  @retval status

**/
EFI_STATUS
EFIAPI
ReadSpd (
  PSYSHOST         Host,
  UINT8            Socket,
  UINT8            Ch,
  UINT8            Dimm,
  UINT16           ByteOffset,
  UINT8            *Data
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  MRC_HOOKS_SERVICES_PPI *MrcHooksServicesPpi;

  MrcHooksServicesPpi = (MRC_HOOKS_SERVICES_PPI *)Host->MrcHooksServicesPpi;

  if (IsMrcHooksServicesPpiReady (Host)) {
    Status = MrcHooksServicesPpi->ReadSpd (Socket, Ch, Dimm, ByteOffset, Data);
  }

  return Status;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Below functions are not part of MrcHooksServicesPpi
// Only wrap other UEFI libraries to avoid change OemProcMemInitLib to save effort for OEMs
//

UINT8
InPort8 (
  UINT16   ioAddress
  )
{
  return IoRead8 (ioAddress);
}

UINT16
InPort16 (
  UINT16   ioAddress
  )
{
  return IoRead16 (ioAddress);
}

UINT32
InPort32 (
  UINT16   ioAddress
  )
{
  return IoRead32 (ioAddress);
}

VOID
OutPort8 (
  UINT16   ioAddress,
  UINT8    data
  )
{
  IoWrite8 (ioAddress, data);
}

VOID
OutPort16 (
  UINT16   ioAddress,
  UINT16   data
  )
{
  IoWrite16 (ioAddress, data);
}
VOID
OutPort32 (
  UINT16   ioAddress,
  UINT32   data
  )
{
  IoWrite32 (ioAddress, data);
}

UINT32
Decompress (
  VOID    *Source,
  UINT32  SrcSize,
  VOID    *Destination,
  UINT32  DstSize,
  VOID    *Scratch,
  UINT32  ScratchSize,
  UINT8   Version
  )
{
  EFI_STATUS Status;
  Status =  UefiDecompress (Source, Destination,  Scratch);
  return EFI_ERROR (Status);
}

VOID
FixedDelayMicroSecond (
  UINT32   usDelay
  )
{
  NanoSecondDelay (usDelay * 1000);
}
