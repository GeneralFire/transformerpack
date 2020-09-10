/** @file
  Reference Code Module.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#include <MemDecode.h>
#include <Gen2/MemDecodeNode.h>
#include "Chip10nm/Include/RcRegs10nm.h"
#include <Library/UpiHostDataLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/UsraCsrLib.h>
#include <Guid/FpgaIpInterface.h>
#include <Include/MemMapHost.h>
#include <Library/SysHostPointerLib.h>

/**
  Create SAD entries for FPGA's present in the platform

  @param [in] Host         Pointer to sysHost
  @param [in] MemMapData   MemMapData  Pointer to the memory map private data structure
  @param [in] MemType      Type of memory region to be mapped

  @retval EFI_SUCCESS           - MemorySize pointer initialized to indicate FPGA memory size
  @retval EFI_INVALID_PARAMETER - can't indicate FPGA memory in MemorySize pointer
**/
UINT32
CreateNewRegionSadFPGA (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT16           MemType
  )
{
  UINT8           SocketNum;
  UINT32          Status;
  UINT32          FpgaMemSize = 0;
  RSAD_TABLE      *SADEntry;
  UINT8           RemoteSadIndex;
  MEMORY_MAP_HOST *MemMapHost;

  MemMapHost = GetMemMapHostPointer ();
  MemMapHost->FpgaMemCnt = 0;
  //
  // check for FPGA, get FPGA memory size and program as remote DRAM rules on all other Xeon CPU Sockets
  //
  for (SocketNum = 0; SocketNum < MAX_SOCKET; SocketNum++) {

    //
    // Get FPGA socket memory size in 64MB
    //
    Status = (UINT32)FpgaIpGetMemorySize(SocketNum, &FpgaMemSize);
    if (Status != SUCCESS) {
      RcDebugPrintWithDevice (SDBG_MAX, SocketNum, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Not an FPGA Socket\n");
      continue;
    }
    RcDebugPrintWithDevice (SDBG_MAX, SocketNum, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Fpga Socket MemSize = 0x%x\n", FpgaMemSize);

    //
    // Adding FPGA memory in system address space (As remote memory for XEON sockets)
    //
    Status = AddRemoteSadEntryForFPGAMemory (Host, MemMapData, SocketNum, (MEM_TYPE)MemType, 0, FpgaMemSize);
    if (Status != SUCCESS) {
      RcDebugPrintWithDevice (SDBG_MINMAX, SocketNum, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "ERROR-Adding FPGA memory in Xeon Sockets Failed\n");
      return Status;
    }

    //
    // Set FPGA Base address
    //
    RemoteSadIndex = FindRemoteSadIndex(MemMapData, 0);
    if (RemoteSadIndex > 0 && RemoteSadIndex < MemMapData->MaxRemoteSadRules) {
      SADEntry = &MemMapData->MemMapSocket[0].RSad[RemoteSadIndex - 1];
      Status = (UINT32)FpgaIpSetMemBaseAddress(SocketNum, SADEntry->Base);
      if (Status != SUCCESS) {
        RcDebugPrintWithDevice (SDBG_MINMAX, SocketNum, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"ERROR!-Updating FPGA Base address\n");
        return Status;
      }
      if (MemMapHost->FpgaMemCnt >= MAX_FPGA_REMOTE_SAD_RULES) {
        RcDebugPrintWithDevice (SDBG_MINMAX, SocketNum, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"ERROR!-Exceeds No. of FPGA's support Limit: %d\n", MemMapHost->FpgaMemCnt);
		return Status;
      }
	  MemMapHost->FpgaRsadRules[MemMapHost->FpgaMemCnt].Attr = SAD_FPGA_ATTR;
      MemMapHost->FpgaRsadRules[MemMapHost->FpgaMemCnt].Base = SADEntry->Base;
      MemMapHost->FpgaRsadRules[MemMapHost->FpgaMemCnt].Enable = SADEntry->Enable;
      MemMapHost->FpgaRsadRules[MemMapHost->FpgaMemCnt].Limit = SADEntry->Limit;
      MemMapHost->FpgaRsadRules[MemMapHost->FpgaMemCnt].NodeId = SADEntry->NodeId;
      MemMapHost->FpgaMemCnt++;
    } else {
      RcDebugPrintWithDevice (SDBG_MINMAX, SocketNum, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"ERROR-Invalid FPGA Memory \n");
    }
  }
  return SUCCESS;
}

