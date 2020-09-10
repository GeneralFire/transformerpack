/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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

#include <PiDxe.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ProcMemInitChipLib.h>

#include <Protocol/CpuCsrAccess.h>
#include <Protocol/SmmBase2.h>

#include <Library/MemoryCoreLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <RcRegs.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/VcuMailBoxLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/CsrAccessLib.h>
#include <ScratchpadList.h>
#include <Library/UsraCsrLib.h>

EFI_CPU_CSR_ACCESS_PROTOCOL    mCpuCsrAccessProtocol;
EFI_SMM_BASE2_PROTOCOL         *mSmmBase = NULL;
EFI_SMM_SYSTEM_TABLE2          *mSmst = NULL;
UINT32                          mCurrentCheckPoint;

/**

  Computes address of CPU Uncore & IIO PCI configuration space using the MMIO mechanism

  @param[in] SocId      - CPU Socket Node number
  @param[in] BoxInst    - Box Instance, 0 based
  @param[in] Offset     - Register offset; values come from the auto generated header file
  @param[in, out] Size  - Ptr to register size in bytes (may be updated if pseudo-offset)

  @retval Address

**/

UINT64
EFIAPI
GetCpuCsrAddress (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset,
  UINT8    *Size
  )
{
  return (UINT64) UsraGetCsrRegisterAddress (SocId, BoxInst, Offset);

} // GetCpuCsrAddress

/**

  Reads CPU Uncore & IIO PCI configuration space using the MMIO mechanism

  @param[in] SocId    - CPU Socket Node number
  @param[in] BoxInst  - Box Instance, 0 based
  @param[in] Offset   - Register offset; values come from the auto generated header file

  @retval Register value

**/

UINT32
EFIAPI
ReadCpuCsr (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset
  )
{
  CSR_OFFSET  RegOffset;
  UINT8       BoxType;

  RegOffset.Data = Offset;
  BoxType = (UINT8)RegOffset.Bits.boxtype;

  if( BoxType == BOX_MC || BoxType == BOX_MCDDC || BoxType == BOX_MCIO ) {
    return MemReadPciCfgEp (SocId, BoxInst, Offset);

  }

  return UsraCsrRead (SocId, BoxInst, Offset);

} // ReadCpuCsr

/**

  Writes CPU Uncore & IIO PCI configuration space using the MMIO mechanism

  @param[in] SocId     - CPU Socket Node number
  @param[in] BoxInst   - Box Instance, 0 based
  @param[in] Offset    - Register offset; values come from the auto generated header file
  @param[in] Data      - Register data to be written

  @retval None

**/

VOID
EFIAPI
WriteCpuCsr (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset,
  UINT32   Data
  )
{
  CSR_OFFSET  RegOffset;
  UINT8       BoxType;
  UINT8*      RegAddr = 0;

  RegOffset.Data = Offset;
  BoxType = (UINT8)RegOffset.Bits.boxtype;

  if (BoxType == BOX_MC || BoxType == BOX_MCDDC || BoxType == BOX_MCIO ) {
    MemWritePciCfgEp (SocId, BoxInst, Offset, Data);

  } else {
    UsraCsrWrite (SocId, BoxInst, Offset, Data);
  }

  if (UbiosGenerationEnabled ()) {
    RegAddr = (UINT8 *)(UINTN) UsraGetCsrRegisterAddress (SocId, BoxInst, Offset);
    DEBUG ((EFI_D_ERROR, "\nuBIOSTag AddrTypeCSR Boxtype:0x%02x;Funcblk:0x%x;Inst:0x%x; Address: [0%08xh], Value: 0%08xh\n", (UINT16)RegOffset.Bits.boxtype, (UINT8)RegOffset.Bits.funcblk, BoxInst, RegAddr, Data));
    DEBUG ((EFI_D_ERROR, "  mov DWORD PTR ds:[0%08xh], 0%08xh\n", RegAddr, Data));
  }

  return;

} // WriteCpuCsr

/**

   Writes the given command to BIOS to PCU Mailbox Interface CSR register

  @param[in] Socket   - CPU Socket number
  @param[in] Command  - Pcu mailbox command to write
  @param[in] Data     - Pcu mailbox data

  @retval error code from the Pcu mailbox (0 = NO ERROR)

**/

UINT64
EFIAPI
Bios2VcodeMailBoxWrite (
  UINT8  Socket,
  UINT32 Command,
  UINT32 Data
  )
{
  UINT64_STRUCT VcodeData;

  VcodeData = WriteBios2VcuMailboxCommand (Socket, Command, Data);

  return (((UINT64)VcodeData.hi << 32) | VcodeData.lo);

} // Bios2VcodeMailBoxWrite

/**

  Reads CPU Memory Controller configuration space using the MMIO mechanism

  @param[in] SocId        - Socket ID
  @param[in] McId         - Memory controller ID
  @param[in] Offset       - Register offset; values come from the auto generated header file

  @retval Register value

**/

UINT32
EFIAPI
ReadMcCpuCsr (
  UINT8    SocId,
  UINT8    McId,
  UINT32   Offset
  )
{
  return MemReadPciCfgMC (SocId, McId, Offset);

} // ReadMcCpuCsr

/**

  Writes CPU Memory Controller configuration space using the MMIO mechanism

  @param[in] SocId        - Socket ID
  @param[in] McId         - Memory controller ID
  @param[in] RegOffset    - Register offset; values come from the auto generated header file
  @param[in] Data         - Register data to be written

  @retval None

**/

VOID
EFIAPI
WriteMcCpuCsr (
  UINT8    SocId,
  UINT8    McId,
  UINT32   Offset,
  UINT32   Data
  )
{
  MemWritePciCfgMC (SocId, McId, Offset, Data);

} // WriteMcCpuCsr

/**

  Get CPU Memory Controller configuration space address used by MMIO mechanism

  @param[in] SocId        - Socket ID
  @param[in] McId         - Memory controller ID
  @param[in] Offset       - Register offset; values come from the auto generated header file

  @retval MC Register MMCFG address

**/

UINTN
EFIAPI
GetMcCpuCsrAddress (
  UINT8    SocId,
  UINT8    McId,
  UINT32   Offset
  )
{
  return MemGetPciCfgMCAddr (SocId, McId, Offset);

} // GetMcCpuCsrAddress

/**

  Reads PCI configuration space using the MMIO mechanism

  @param[in] Socket - Socket
  @param[in] Reg    - "Reg" uses the format in the Bus_Dev_Func_CFG.H files

  @retval Value in requested reg

**/

UINT32
EFIAPI
ReadPciCsr (
  IN UINT8    Socket,
  IN UINT32   Reg
  )
{
  return ReadPciCfg (Socket, Reg);

} // ReadPciCsr

/**

  Writes specified data to PCI configuration space using the MMIO mechanism

  @param[in] Socket - Socket
  @param[in] Reg    - "Reg" uses the format in the Bus_Dev_Func_CFG.H files
  @param[in] Data   - Value to write

  @retval VOID

**/

VOID
EFIAPI
WritePciCsr (
  IN UINT8    Socket,
  IN UINT32   Reg,
  IN UINT32   Data
  )
{
  WritePciCfg (Socket, Reg, Data);

} // WritePciCsr

/**

  Get PCI configuration space address used MMIO mechanism

  @param[in] Socket - Socket
  @param[in] Reg    - "Reg" uses the format in the Bus_Dev_Func_CFG.H files

  @retval Address of requested reg

**/

UINT32
EFIAPI
GetPciCsrAddress (
  IN UINT8    Socket,
  IN UINT32   Reg
  )
{
  return GetPciCfgAddress (Socket, Reg);

} // GetPciCsrAddress

/**

  Writes the checkpoint code to the checkpoint CSR and breaks if match with debug breakpoint
  @param[in] Socket    - Socket to write
  @param[in] majorCode - Major Checkpoint code to write
  @param[in] minorCode - Minor Checkpoint code to write
  @param[in] data      - Data specific to the minor checkpoint is written to
                         low word of the checkpoint CSR

  @retval VOID

**/

VOID
EFIAPI
BreakAtCheckpoint (
  IN UINT8    Socket,
  IN UINT8    MajorCode,
  IN UINT8    MinorCode,
  IN UINT16   Data
  )
{
  UINT16 Checkpoint;
  BIOS_NON_STICKY_SCRATCHPAD7_STRUCT CsrRegCheckpoint;
  BIOS_SCRATCHPAD6_STRUCT            Sp6;

  Checkpoint = (UINT32) ((MajorCode << 8) | MinorCode);
  CsrRegCheckpoint.Bits.MajorPostCode = MajorCode;
  CsrRegCheckpoint.Bits.MinorPostCode = MinorCode;
  CsrRegCheckpoint.Bits.ImpSpecData = Data;

  //
  // Write checkpoint to local scratchpad
  //

  UsraCsrWrite (Socket, 0, BIOSNONSTICKYSCRATCHPAD7_UBOX_MISC_REG, CsrRegCheckpoint.Data);
  DEBUG ((EFI_D_INFO, "Scratchpad_Debug BreakAtCheckpoint: Verify Non Sticky Scratchpad7 0x%x\n", CsrRegCheckpoint.Data));

  //
  // Update global checkpoint
  //

  mCurrentCheckPoint = CsrRegCheckpoint.Data;

  IoWrite8 (0x80, (UINT8) (mCurrentCheckPoint >> 24));

  //
  // Output the progress code
  //
  DEBUG ((EFI_D_INFO, "Checkpoint Code: Socket %d, 0x%02X, 0x%02X, 0x%04X\n", Socket, MajorCode, MinorCode, Data));

  //
  // Compare the breakpoint CSR
  //
  Sp6.Data = UsraCsrRead (Socket, 0, BIOSSCRATCHPAD6_UBOX_MISC_REG);
  if (Sp6.Bits.EvToolBreakPoint == Checkpoint) {

    DEBUG ((EFI_D_INFO, "Breakpoint match found. S:%x waiting...\n", Socket));

    //
    // Stall the boot until breakpoint changes
    //
    do {
      Sp6.Data = UsraCsrRead (Socket, 0, BIOSSCRATCHPAD6_UBOX_MISC_REG);
    }while (Sp6.Bits.EvToolBreakPoint == Checkpoint);
  }

  return;

} // BreakAtCheckpoint

//
// Driver entry point
//

/**
  Initialize the CpuCsrAccess driver.

  @param[in] ImageHandle  - The firmware allocated handle for the EFI image.
  @param[in] SystemTable  - A pointer to the EFI System Table.

  @retval EFI_SUCCESS     - The driver initialized correctly.
  @retval !EFI_SUCCESS    - Error
**/

EFI_STATUS
EFIAPI
CpuCsrAccessStart (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS                     Status;
  EFI_HANDLE                     Handle = NULL;
  BOOLEAN                        InSmm;

  //
  // Retrieve SMM Base Protocol
  //
  Status = gBS->LocateProtocol (
                  &gEfiSmmBase2ProtocolGuid,
                  NULL,
                  &mSmmBase
                  );
  if (mSmmBase == NULL) {
    InSmm = FALSE;
  } else {
    mSmmBase->InSmm (mSmmBase, &InSmm);
    mSmmBase->GetSmstLocation (mSmmBase, &mSmst);
  }

  ZeroMem (&mCpuCsrAccessProtocol, sizeof (EFI_CPU_CSR_ACCESS_PROTOCOL));

  mCpuCsrAccessProtocol.GetCpuCsrAddress       = GetCpuCsrAddress;
  mCpuCsrAccessProtocol.ReadCpuCsr             = ReadCpuCsr;
  mCpuCsrAccessProtocol.WriteCpuCsr            = WriteCpuCsr;
  mCpuCsrAccessProtocol.Bios2VcodeMailBoxWrite = Bios2VcodeMailBoxWrite;
  mCpuCsrAccessProtocol.ReadMcCpuCsr           = ReadMcCpuCsr;
  mCpuCsrAccessProtocol.WriteMcCpuCsr          = WriteMcCpuCsr;
  mCpuCsrAccessProtocol.GetMcCpuCsrAddress     = GetMcCpuCsrAddress;
  mCpuCsrAccessProtocol.ReadPciCsr             = ReadPciCsr;
  mCpuCsrAccessProtocol.WritePciCsr            = WritePciCsr;
  mCpuCsrAccessProtocol.GetPciCsrAddress       = GetPciCsrAddress;
  mCpuCsrAccessProtocol.BreakAtCheckpoint      = BreakAtCheckpoint;

  if (InSmm) {
    Status = mSmst->SmmInstallProtocolInterface (
            &Handle,
            &gEfiCpuCsrAccessGuid,
            EFI_NATIVE_INTERFACE,
            &mCpuCsrAccessProtocol
            );
  } else {
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEfiCpuCsrAccessGuid,
                  EFI_NATIVE_INTERFACE,
                  &mCpuCsrAccessProtocol
                  );
  }

  ASSERT_EFI_ERROR (Status);

  return Status;

} // CpuCsrAccessStart
