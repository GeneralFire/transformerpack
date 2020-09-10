/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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
#include <Library/KtiApi.h>
#include <Library/EmulationConfigurationLib.h>
#include <RcRegs.h>
#include <Upi/KtiSi.h>
#include <Library/UsraCsrLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/KtiSimLib.h>
#include <Library/RcrbAccessLib.h>

/**
  MmioWrite32 wrapper to print assembly code for emulation

  @param Address  - The MMIO register to write.
  @param Value    - The value to write to the MMIO register.

  @return Value.
**/
UINT32
EFIAPI
UbiosMmioWrite32 (
  IN UINTN    Address,
  IN UINT32   Value
  )
{
  if (UbiosGenerationEnabled ()) {
    DEBUG ((DEBUG_ERROR, "\n  mov DWORD PTR ds:[0%08xh], 0%08xh\n", Address, Value));
  }

  return MmioWrite32 (Address, Value);
}

/**
  Get RCRBBAR base address from the CXL RCRBBAR register.

  @param[in]  SocId     - Socket ID
  @param[in]  CtrId     - Controller ID
  @param[in]  CxlFlag   - Indicate what type of the control, TRUE: Pcie G5 - CXL; FALSE: Pcie G4 - DMI

  @retval               - The 64-bit RCRBBAR base address or 0 if failed.
**/
UINT64
EFIAPI
GetRcrbBar (
  IN  UINT8        SocId,
  IN  UINT8        CtrId,
  IN  BOOLEAN      CxlFlag
  )
{
  UINT64    RcrbBarBase;
  UINT32    Data32;
  UINT32    RcrbBarAddrLow, RcrbBarAddrHigh;

  if (CxlFlag) {
    RcrbBarAddrLow = RCRBBAR_N0_SB_PRVT_PCIE_G5_REG;
    RcrbBarAddrHigh = RCRBBAR_N1_SB_PRVT_PCIE_G5_REG;
  } else {
    if ((SocId != GetSysSbspSocketIdNv ()) || (CtrId != 0)) {
      DEBUG ((DEBUG_ERROR, "\nInput parameter is invalid!\n"));
      ASSERT (FALSE);
      return 0;
    }
    RcrbBarAddrLow = RCRBBAR_N0_SB_PRVT_PCIE_G4_REG;
    RcrbBarAddrHigh = RCRBBAR_N1_SB_PRVT_PCIE_G4_REG;
  }

  Data32 = UsraCsrRead (SocId, CtrId, RcrbBarAddrHigh);
  RcrbBarBase = Data32;

  Data32 = UsraCsrRead (SocId, CtrId, RcrbBarAddrLow);
  RcrbBarBase = (LShiftU64 (RcrbBarBase, 32) | (Data32  & 0xFFFFFFFE));

  if (RcrbBarBase == 0) {
    DEBUG ((DEBUG_ERROR, "\nWARNING: CXL DP RCRBBAR is 0!!\n"));
  }

  return RcrbBarBase;
}

/**
  Set RCRBBar for CXL or PCIe Port

  @param SocId                       Socket ID
  @param CtrId                       Controller ID
  @param CxlFlag                     Indicate what type of the control, TRUE: Pcie G5 - CXL; FALSE: Pcie G4 - DMI
  @param BarAddr                     RCRB Bar Address

  @retval                            - EFI_UNSUPPORTED: the function not implemented
                                     - EFI_SUCCESS: the function is excuted successfully
**/
EFI_STATUS
EFIAPI
SetRcrbBar (
  IN  UINT8                       SocId,
  IN  UINT8                       CtrId,
  IN  BOOLEAN                     CxlFlag,
  IN  UINT32                      BarAddr
  )
{
  RCRBBAR_N0_SB_PRVT_PCIE_G5_STRUCT     RcrbBarCxlDpN0;
  RCRBBAR_N0_SB_PRVT_PCIE_G4_STRUCT     RcrbBarDmiN0;

  if (CxlFlag) {
    //
    // RCRB should be allocated below 4GB, so just program the N1 register to 0
    //
    UsraCsrWrite (SocId, CtrId, RCRBBAR_N1_SB_PRVT_PCIE_G5_REG, 0);

    //
    // Note:
    // By spec, the RcrbBar must be 8KB aligned.
    // - In this routine, the input BarAddr is likely to be the MmiolBase of the stack to which the CXL belongs, and
    //   it is directly programmed to RcrbBar register, which assumes the stack's MmiolBase is at least 8KB aligned.
    //
    RcrbBarCxlDpN0.Data = 0;
    RcrbBarCxlDpN0.Bits.baddr = BarAddr >> 13;
    RcrbBarCxlDpN0.Bits.en = 1;
    UsraCsrWrite (SocId, CtrId, RCRBBAR_N0_SB_PRVT_PCIE_G5_REG, RcrbBarCxlDpN0.Data);
  } else {
    if (CtrId != 0) {
      DEBUG ((DEBUG_ERROR, "\nInput parameter is invalid!\n"));
      ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }

    UsraCsrWrite (SocId, CtrId, RCRBBAR_N1_SB_PRVT_PCIE_G4_REG, 0);

    RcrbBarDmiN0.Data = 0;
    RcrbBarDmiN0.Bits.baddr = BarAddr >> 13;
    RcrbBarDmiN0.Bits.en = 1;
    UsraCsrWrite (SocId, CtrId, RCRBBAR_N0_SB_PRVT_PCIE_G4_REG, RcrbBarDmiN0.Data);
  }

  DEBUG ((DEBUG_ERROR, "  %a (): RcrbBase = 0x%08X\n", __FUNCTION__, BarAddr));

  return EFI_SUCCESS;
}

/**
  Clear RCRBBar for CXL or PCIe Port.

  @param[in]  SocId                      - Socket ID
  @param[in]  CtrId                      - Controller ID
  @param[in]  CxlFlag                    - Indicate what type of the control, TRUE: Pcie G5 - CXL; FALSE: Pcie G4 - DMI

  @retval     EFI_SUCCESS                - Function completed successfully
  @retval     EFI_INVALID_PARAMETER      - Fuction failed due to input parameter wrong
**/
EFI_STATUS
EFIAPI
ClearRcrbBar (
  IN  UINT8                       SocId,
  IN  UINT8                       CtrId,
  IN  BOOLEAN                     CxlFlag
  )
{
  if (CxlFlag) {
    UsraCsrWrite (SocId, CtrId, RCRBBAR_N1_SB_PRVT_PCIE_G5_REG, 0);
    UsraCsrWrite (SocId, CtrId, RCRBBAR_N0_SB_PRVT_PCIE_G5_REG, 0);
  } else {
    if ((SocId != GetSysSbspSocketIdNv ()) || (CtrId != 0)) {
      DEBUG ((DEBUG_ERROR, "\nInput parameter is invalid!\n"));
      ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }

    UsraCsrWrite (SocId, CtrId, RCRBBAR_N1_SB_PRVT_PCIE_G4_REG, 0);
    UsraCsrWrite (SocId, CtrId, RCRBBAR_N0_SB_PRVT_PCIE_G4_REG, 0);
  }

  return EFI_SUCCESS;
}

/**
  Routine to probe and program the 64-bit MEMBAR0 register for CXL port.

  @param[in]  RcrbBaseAddr           RCRB base address
  @param[in]  BarRegOff              The offset of the Bar register
  @param[in]  MmioStartAddr          The start address of available MMIO resource
  @param[out] MemBar0Base            The pointer to the base address of MMIO resource allocated for MEMBAR0
  @param[out] MemBar0Size            The pointer to the size of MMIO resource requested by MEMBAR0

  @retval     EFI_SUCCESS            - Function return successfully.
              EFI_INVALID_PARAMETER  - Input parameter is invalid.
**/
EFI_STATUS
EFIAPI
ProgramMemBar0Register (
  IN  UINTN      RcrbBaseAddr,
  IN  UINT32     BarRegOff,
  IN  UINT32     MmioStartAddr,
  OUT UINT64     *MemBar0Base,
  OUT UINT64     *MemBar0Size
  )
{
  UINT32     DataBuffer, Data32;
  UINT64     Data64;

  if (MemBar0Base == NULL || MemBar0Size == NULL) {
    DEBUG ((DEBUG_ERROR, "\nInvalid input parameters!!\n"));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Save the original low 32bit value of MEMBAR0
  //
  DataBuffer = MmioRead32 (RcrbBaseAddr + BarRegOff);

  //
  // Probe the MEMBAR0 size
  //
  MmioWrite32 (RcrbBaseAddr + BarRegOff, 0xFFFFFFFF);  //Low 32 bits
  MmioWrite32 (RcrbBaseAddr + BarRegOff + 4, 0xFFFFFFFF);  //High 32 bits
  Data32 = MmioRead32 (RcrbBaseAddr + BarRegOff + 4);  //Read back the high 32 bits
  Data64 = Data32;
  Data32 = MmioRead32 (RcrbBaseAddr + BarRegOff);  //Read back the low 32 bits
  Data32 &= 0xFFFFFFF0;  //Clearing the encoding information bits 0-3 for memory
  Data64 = LShiftU64 (Data64, 32) | Data32;
  *MemBar0Size = ~Data64 + 1;

  //
  // Adjust the MMIO base address to meet the alignment
  //
  *MemBar0Base = (MmioStartAddr + *MemBar0Size - 1) & (~(*MemBar0Size - 1));
  if ((UINT64) MmioStartAddr != *MemBar0Base) {
    DEBUG ((DEBUG_ERROR, "  Warning: Created a Memory Hole: 0x%x ~ 0x%x\n", MmioStartAddr, *MemBar0Base - 1));
  }
  DEBUG ((DEBUG_ERROR, "  MEMBAR0: Base = 0x%lx, Size = 0x%lx\n", *MemBar0Base, *MemBar0Size));

  //
  // Program the MEMBAR0 register
  //
  Data32 = ((UINT32) (*MemBar0Base & 0xFFFFFFF0)) | (DataBuffer & 0xF);
  UbiosMmioWrite32 (RcrbBaseAddr + BarRegOff, Data32);
  Data32 = (UINT32) RShiftU64 (*MemBar0Base, 32);
  UbiosMmioWrite32 (RcrbBaseAddr + BarRegOff + 4, Data32);

  return EFI_SUCCESS;
}

/**
  Routine to clear the 64-bit MEMBAR0 register for CXL port.

  @param[in]  RcrbBaseAddr           RCRB base address
  @param[in]  BarRegOff              The offset of the Bar register

  @retval     VOID

**/
VOID
EFIAPI
ClearMmioForCxlPort (
  IN  UINTN      RcrbBaseAddr,
  IN  UINT32     BarRegOff
  )
{
  UbiosMmioWrite32 (RcrbBaseAddr + BarRegOff, 0);      // Low 32-bit
  UbiosMmioWrite32 (RcrbBaseAddr + BarRegOff + 4, 0);  // High 32-bit
}

/**
  Get MEMBAR0 base address using the CXL RCRBBAR register.

  @param[in]  SocId     - Socket ID
  @param[in]  CtrId     - Controller ID
  @param[in]  BarRegOff - The offset of the Bar register

  @retval               - The 64-bit RCRBBAR base address or 0 if failed.
**/
UINT64
EFIAPI
GetMemBar0 (
  IN  UINT8        SocId,
  IN  UINT8        CtrId,
  IN  UINT32       BarRegOff
  )
{
  UINT32 RcrbBar;
  UINT64 MemBar0;
  UINT32 MemBar0High, MemBar0Low;

  RcrbBar = (UINT32)GetRcrbBar (SocId, CtrId, TYPE_CXL_RCRB);
  if (RcrbBar == 0) {
    return 0;
  }

  MemBar0High = MmioRead32 (RcrbBar + BarRegOff + 4);
  MemBar0     = MemBar0High;
  MemBar0Low  = MmioRead32 (RcrbBar + BarRegOff);
  MemBar0Low &= 0xFFFFFFF0;  //Clearing the encoding information bits 0-3 for memory

  MemBar0 = LShiftU64 (MemBar0, 32) | MemBar0Low;

  return MemBar0;
}