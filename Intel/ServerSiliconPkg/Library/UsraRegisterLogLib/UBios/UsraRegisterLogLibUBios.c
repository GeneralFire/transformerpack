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

#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/UsraRegisterLogLib.h>

/**
  Perform register read log.
  Additional code could be added before or after RegAlignedRead() per specific needs.

  @param[in]  Address              A pointer of the address of the USRA Address Structure to be read out.
  @param[in]  AlignedAddress       An address to be read out.
  @param[out] Buffer               A pointer of buffer contains the data to be read out.

  @retval RETURN_UNSUPPORTED       Register log is not supported.
**/
RETURN_STATUS
EFIAPI
UsraRegisterLogRead (
  IN  USRA_ADDRESS            *Address,
  IN  UINTN                   AlignedAddress,
  OUT VOID                    *Buffer
  )
{
  return RETURN_UNSUPPORTED;
}

/**
  Perform register write log.
  Additional code could be added before or after RegAlignedWrite() per specific needs.

  @param[in]  Address              A pointer of the address of the USRA Address Structure to be written.
  @param[in]  AlignedAddress       An address to be written.
  @param[in]  Buffer               A pointer of buffer contains the data to be written.

  @retval RETURN_SUCCESS           Register access is issued.
**/
RETURN_STATUS
EFIAPI
UsraRegisterLogWrite (
  IN  USRA_ADDRESS             *Address,
  IN  UINTN                    AlignedAddress,
  IN  VOID                     *Buffer
  )
{
  CSR_OFFSET       RegOffset;
  UINT8            Size;

  RegOffset.Data = Address->dwRawData[0];

  if (UbiosGenerationEnabled () == TRUE) {
    DEBUG ((EFI_D_ERROR, "\nuBIOSTag AddrType CSR"));
    if (Address->Attribute.AddrType == AddrTypeCSRMEM) {
      DEBUG ((EFI_D_ERROR, "MEM Address: [0%08xh]", AlignedAddress));
      Size = (UINT8) Address->Attribute.AccessWidth;
    } else if (Address->Attribute.AddrType == AddrTypeCSRCFG) {
      DEBUG ((EFI_D_ERROR, "CFG Address: [0%08xh]", AlignedAddress));
      Size = (UINT8) Address->Attribute.AccessWidth;
    } else if (Address->Attribute.AddrType == AddrTypePCIE || Address->Attribute.AddrType == AddrTypePCIEBLK) {
      DEBUG ((EFI_D_ERROR, "PCIE Address: [0%08xh]", AlignedAddress));
      Size = (UINT8) Address->Attribute.AccessWidth;
    } else {
      DEBUG ((EFI_D_ERROR, " Boxtype:0x%02x;Funcblk:0x%x;Inst:0x%x;Address: [0%08xh]", (UINT16)RegOffset.Bits.boxtype, (UINT8)RegOffset.Bits.funcblk, (UINT8)Address->Csr.InstId, AlignedAddress));
      Size = (UINT8) RegOffset.Bits.size;
    }

    switch (Size) {
    case  UsraWidth64:
      DEBUG ((EFI_D_ERROR, ", Value: 0%016xh\n\nWARNING: Not support 64bit MMIO access! Please use two 32bit access instead!\n", *(UINT64 *) Buffer));
      DEBUG ((EFI_D_ERROR, "  hlt\n"));  //Enter halt state
      CpuDeadLoop ();
      break;
    case  UsraWidth32:
      DEBUG ((EFI_D_ERROR, ", Value: 0%08xh\n  mov DWORD PTR ds:[0%08xh], 0%08xh\n", *(UINT32 *) Buffer, AlignedAddress, *(UINT32*) Buffer));
      break;
    case  UsraWidth16:
      DEBUG ((EFI_D_ERROR, ", Value: 0%08xh\n  mov  WORD PTR ds:[0%08xh], 0%04xh\n", *(UINT16 *) Buffer, AlignedAddress, *(UINT16*) Buffer));
      break;
    case  UsraWidth8:
      DEBUG ((EFI_D_ERROR, ", Value: 0%08xh\n  mov  BYTE PTR ds:[0%08xh], 0%02xh\n", *(UINT8 *) Buffer, AlignedAddress, *(UINT8*) Buffer));
      break;
    default:
      DEBUG ((EFI_D_ERROR, "\nInvalid Register Size in RegOffset = 0x%X\n", RegOffset.Data));
      break;
    }
  }

  return RETURN_SUCCESS;
}
