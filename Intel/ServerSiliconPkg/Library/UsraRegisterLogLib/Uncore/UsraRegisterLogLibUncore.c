/** @file
  Implementation of UsraRegisterFilterLib class library.

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

#include "UsraRegisterLogLibInternal.h"
#include <Library/UsraAccessApi.h>
#include <Library/IoLib.h>
#include <Library/MemoryCoreLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <RcRegs.h>
#include <UncoreCommonIncludes.h>
#include <SysRegs.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/CsrAccessLib.h>
#include <MemHostChip.h>
#include <Library/MemoryServicesLib.h>
#include <Library/UsraRegisterLogSkipLib.h>
#include <Library/UsraRegisterLogLib.h>
#include <Library/PerformanceTrackerLib.h>

/**
  Perform register read.

  @param[in]  Address              A pointer of the address of the USRA Address Structure to be read out.
  @param[in]  AlignedAddress       An address to be read out.
  @param[out] Buffer               A pointer of buffer contains the data to be read out.

  @retval RETURN_SUCCESS           Register access is issued.
**/
RETURN_STATUS
EFIAPI
UsraRegisterLogRead (
  IN USRA_ADDRESS *Address,
  IN UINTN AlignedAddress,
  OUT VOID *Buffer
  )
{
  CSR_OFFSET RegOffset;

#ifdef DEBUG_CODE_BLOCK

  PT_ZONE OldPhase = PTZ_MAX_ZONE;
  PT_ZONE CurrentMemPhase = PTZ_MAX_ZONE;

#endif //DEBUG_CODE_BLOCK

  if (Address->Attribute.AddrType == AddrTypeCSRMEM) {
    CsrShortTraceCap(Address->CsrMem.SocketId, AlignedAddress, Buffer, READ_ACCESS, (UINT8)Address->Attribute.AccessWidth);
  } else if (Address->Attribute.AddrType == AddrTypeCSRCFG) {
    CsrShortTraceCap(Address->CsrCfg.SocketId, AlignedAddress, Buffer, READ_ACCESS, (UINT8)Address->Attribute.AccessWidth);
  } else if (Address->Attribute.AddrType == AddrTypeCSR) {

    RegOffset.Data = Address->dwRawData[0];

    if (PcdGetBool(PcdMrcDebugPerformanceStats) == TRUE) {
#ifdef DEBUG_CODE_BLOCK
      //
      // Note that the below will block not just those two CSR accesses from being
      // added to the performance log, but also any other CSR accesses until the zone
      // is changed from PTZ_PIPE_POLL.
      //
      if (Address->Csr.Offset == SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR ||
          Address->Csr.Offset == SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR) {
        GetMemPhase (&OldPhase);
        SetMemPhase (PTZ_PIPE_POLL);
      }

      CpuCsrAccessCount ();

#endif //DEBUG_CODE_BLOCK
    }

#ifdef MRC_TRACE
#ifdef DEBUG_CODE_BLOCK

    CsrLongTraceCap ((UINT8)Address->Csr.SocketId, (UINT8 *)AlignedAddress, RegOffset, (UINT32)RegOffset.Bits.offset, *(UINT32 *)Buffer, (UINT8)RegOffset.Bits.size, READ_ACCESS);

#endif //DEBUG_CODE_BLOCK
#endif //MRC_TRACE

    if (PcdGetBool(PcdMrcDebugPerformanceStats) == TRUE) {
#ifdef DEBUG_CODE_BLOCK
      GetMemPhase (&CurrentMemPhase);
      if ((CurrentMemPhase == PTZ_PIPE_POLL) && (OldPhase != PTZ_MAX_ZONE)) {
        SetMemPhase (OldPhase);
      }
#endif //DEBUG_CODE_BLOCK
    }
  } else {
    return RETURN_UNSUPPORTED;
  }

  return RETURN_SUCCESS;
}

/**
  Perform register write

  @param[in]  Address              A pointer of the address of the USRA Address Structure to be written.
  @param[in]  AlignedAddress       An address to be written.
  @param[in]  Buffer               A pointer of buffer contains the data to be written.

  @retval RETURN_SUCCESS           Register access is issued.
**/
RETURN_STATUS
EFIAPI
UsraRegisterLogWrite (
  IN USRA_ADDRESS *Address,
  IN UINTN AlignedAddress,
  IN VOID *Buffer
  )
{
  CSR_OFFSET RegOffset;

  if (Address->Attribute.AddrType == AddrTypePCIIO) {
    if (UbiosGenerationEnabled()) {
      PciIoAccessWriteToLog(PCI_CONFIGURATION_ADDRESS_PORT, PCI_CONFIGURATION_DATA_PORT, (UINT32)AlignedAddress, *((UINT32 *)Buffer));
    }
  } else if (Address->Attribute.AddrType == AddrTypeCSRMEM) {
    if (UbiosGenerationEnabled () == TRUE) {
      RegOffset.Data = Address->dwRawData[0];
      DEBUG ((EFI_D_ERROR, "\nuBIOSTag AddrTypeCSR"));
      DEBUG ((EFI_D_ERROR, " Boxtype:0x%02x;Funcblk:0x%x;Inst:0x%x;Address: [0%08xh], Value: 0%08xh\n", (UINT16)RegOffset.Bits.boxtype, (UINT8)RegOffset.Bits.funcblk, (UINT8)Address->Csr.InstId, AlignedAddress, *(UINT32*) Buffer));
    }
    CsrShortTraceCap(Address->CsrMem.SocketId, AlignedAddress, Buffer, WRITE_ACCESS, (UINT8)Address->Attribute.AccessWidth);
  } else if (Address->Attribute.AddrType == AddrTypeCSRCFG) {
    if (UbiosGenerationEnabled () == TRUE) {
      RegOffset.Data = Address->dwRawData[0];
      DEBUG ((EFI_D_ERROR, "\nuBIOSTag AddrTypeCSR"));
      DEBUG ((EFI_D_ERROR, " Boxtype:0x%02x;Funcblk:0x%x;Inst:0x%x;Address: [0%08xh], Value: 0%08xh\n", (UINT16)RegOffset.Bits.boxtype, (UINT8)RegOffset.Bits.funcblk, (UINT8)Address->Csr.InstId, AlignedAddress, *(UINT32*) Buffer));
    }
    CsrShortTraceCap(Address->CsrCfg.SocketId, AlignedAddress, Buffer, WRITE_ACCESS, (UINT8)Address->Attribute.AccessWidth);
  } else if (Address->Attribute.AddrType == AddrTypeCSR) {
    RegOffset.Data = Address->dwRawData[0];

    CpuCsrAccessCount ();

#ifdef MRC_TRACE
#ifdef DEBUG_CODE_BLOCK

    CsrLongTraceCap ((UINT8)Address->Csr.SocketId, (UINT8 *)AlignedAddress, RegOffset, (UINT32)RegOffset.Bits.offset, *(UINT32 *)Buffer, (UINT8)RegOffset.Bits.size, WRITE_ACCESS);

#endif //DEBUG_CODE_BLOCK
#endif // #ifdef MRC_TRACE

    if (UbiosGenerationEnabled()) {
      CsrUBiosWriteLog ((UINT8 *)AlignedAddress, RegOffset, *(UINT32 *)Buffer, RegOffset.Data, (UINT8)RegOffset.Bits.size, (UINT8)Address->Csr.InstId);
    }
  } else if (Address->Attribute.AddrType == AddrTypePCIE) {
    if (UbiosGenerationEnabled()) {
      PcieUBiosWriteLog (AlignedAddress,
        *(UINT32 *)Buffer,
        (UINT8)Address->Pcie.Bus,
        (UINT8)Address->Pcie.Dev,
        (UINT8)Address->Pcie.Func,
        (UINT8)Address->Attribute.AccessWidth
      );
    }
  } else {
    return RETURN_UNSUPPORTED;
  }

  return RETURN_SUCCESS;
}
