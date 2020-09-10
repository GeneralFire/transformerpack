/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

//
// The package level header files this module uses
//
#include <Uefi.h>

//
// Driver specifc include files
//
#include <Library/MemoryCoreLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <RcRegs.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/SimRegsLookupLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SysHostPointerLib.h>

/**

    CSR trace in serial log file that can later be used

    @param socket:  CPU socket ID
    @param regAddr - CSR register address
    @param reg     - CSR register name as defined in register header files
    @param data    - CSR data
    @param RwFlag  -  0 for Rd, or 1 for Wr

    @retval None

**/
VOID
PciCfgAccessTraceCap (
  UINT8 socket,
  UINT8 *regAddr,
  UINT32 reg,
  UINT32 data,
  UINT8 RwFlag
)
{

#ifdef IA32
#ifdef MRC_TRACE
#ifdef DEBUG_CODE_BLOCK
  if (RwFlag == READ_ACCESS) {
    /*
     *Socket, Type, Address, Direction, Size, Data

     Where:
     - Socket = processor socket # that executes the instruction
     - Type = 0 for IO, or 1 for Memory
     - Address = 16-bit IO or 32-bit Memory address (the MMIO address will encode bus, device, function, offset according to MMCFG format)
     - Direction = 0 for Rd, or 1 for Wr
     - Size = 1, 2, or 4 bytes
     - Data = hex data corresponding to the size

     For example:

     *0, 0, 0x80, 1, 1, 0xA0
     *0, 1, 0x100800dc, 1, 4, 0x00000055
     *0, 1, 0x10000000, 0, 4, 0x36008086
     *0, 1, 0x10000008, 0, 1, 0x00
    */
    if (GetDebugLevel () & SDBG_TRACE) {

      PSYSHOST Host;

      Host = GetSysHostPointer ();

      if (!((reg  == SR_POST_CODE_CSR_ADDR(Host))||(reg  == SR_BIOS_SERIAL_DEBUG_CSR_ADDR(Host)))) {
        AcquirePrintControl ();
        RcDebugPrint (SDBG_DEFAULT, "*%d, 1, 0x%x, 0, %d, ", socket, regAddr, reg >> 28);

        //
        // Determine register size
        //
        switch (reg >> 28) {
          case sizeof (UINT32):
            RcDebugPrint (SDBG_DEFAULT, "0x%08x\n", (UINT32) data);
            break;

          case sizeof (UINT16):
            RcDebugPrint (SDBG_DEFAULT, "0x%04x\n", (UINT16) data);
            break;

          case sizeof (UINT8):
            RcDebugPrint (SDBG_DEFAULT, "0x%02x\n", (UINT8) data);
            break;

          default:
            RcDebugPrint (SDBG_DEFAULT, "Invalid register size in reg = 0x%X.\n", reg);
            ASSERT (FALSE);
            break;
        }
        ReleasePrintControl ();
      }
    }
  } else {
    if (GetDebugLevel () & SDBG_TRACE) {

      PSYSHOST Host;

      Host = GetSysHostPointer ();

      if (!((reg  == SR_POST_CODE_CSR_ADDR(Host))||(reg  == SR_BIOS_SERIAL_DEBUG_CSR_ADDR(Host)))) {
        AcquirePrintControl ();
        RcDebugPrint (SDBG_DEFAULT, "*%d, 1, 0x%x, 1, %d, ", socket, regAddr, reg >> 28);

        //
        // Determine register size
        //
        switch (reg >> 28) {
          case sizeof (UINT32):
            RcDebugPrint (SDBG_DEFAULT, "0x%08x\n", (UINT32) data);
            break;

          case sizeof (UINT16):
            RcDebugPrint (SDBG_DEFAULT, "0x%04x\n", (UINT16) data);
            break;

          case sizeof (UINT8):
            RcDebugPrint (SDBG_DEFAULT, "0x%02x\n", (UINT8) data);
            break;

          default:
            RcDebugPrint (SDBG_DEFAULT, "Invalid register size in reg = 0x%X.\n", reg);
            ASSERT (FALSE);
            break;
        }
        ReleasePrintControl ();
      }
    }
  }
#endif // DEBUG_CODE_BLOCK
#endif // MRC_TRACE
#endif // IA32
}

/**

    For a CSR write, generate "mov ...."  instruction in serial log file that can later be used by emulation BIOS

    @param regAddr - CSR register address
    @param reg        - CSR register name as defined in register header files
    @param data      - CSR data

    @retval None

**/
VOID
PciCfgAccessWriteToLog (
  UINT8 *regAddr,
  UINT32 reg,
  UINT32 data
)
{

#ifdef IA32
#ifdef DEBUG_CODE_BLOCK
  //
  // Only print stuff uBIOS cares about
  // Only care about CPU Bus 0 and 1 writes (skip PCH writes)
  //
  if (!(UbiosGenerationEnabled ())) {
    return;
  }
  if ((((reg & 0x0ff00000) >> 20) == 0) || (((reg & 0x0ff00000) >> 20) == 1)) {

    PSYSHOST Host;

    Host = GetSysHostPointer ();

    //
    // skip printing Scratch pad writes and SMBUS
    //
    if (!(reg  == SR_BIOS_SERIAL_DEBUG_CSR_ADDR(Host))) {
      if (GetUbiosOutputMode () == ASM_OUTPUT_ENABLE) {
        //
        // Determine register size
        //
        switch (reg >> 28) {
          case sizeof (UINT32):
            RcDebugPrint (SDBG_DEFAULT, "  mov DWORD PTR ds:[0%08xh], 0%08xh\n", regAddr, (UINT32) data);
            break;
          case sizeof (UINT16):
            RcDebugPrint (SDBG_DEFAULT, "  mov  WORD PTR ds:[0%08xh], 0%04xh\n", regAddr, (UINT16) data);
            break;
          case sizeof (UINT8):
            RcDebugPrint (SDBG_DEFAULT, "  mov  BYTE PTR ds:[0%08xh], 0%02xh\n", regAddr, (UINT8) data);
            break;
          default:
            RcDebugPrint (SDBG_DEFAULT, "Invalid register size in reg = 0x%X.\n", reg);
            ASSERT (FALSE);
            break;
        }
      }
    }
  }
#endif //DEBUG_CODE_BLOCK
#endif //IA32
}

/**

  Reads PCI configuration space using the MMIO mechanism

  @param[in] socket  - CPU Socket Node number (Socket ID)
  @param[in] reg     - input parameter "reg" uses the format in the Bus_Dev_Func_CFG.H files

  @retval Value in requested reg

**/
UINT32
ReadPciCfg (
  IN  UINT8       socket,
  IN  UINT32      reg
  )
{
  UINT8   *regAddr;
  UINT32  data = 0;
  UINT32  bus = 0;
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;
  EFI_STATUS         Status = EFI_SUCCESS;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  CpuCsrAccessCount ();

  if (socket > 0) {
    CpuCsrAccessError ("Invalid socket number %u.\n", socket);
    ASSERT (FALSE);
  }
  //
  // determine if targeting IIO bus or Uncore Bus (see if bus# filed of the register address = 0)
  //
  switch ((reg & 0x0ff00000) >> 20) {
    case 0:
      //
      // IIO Bus
      //
      bus = CpuCsrAccessVar->SocketFirstBus[socket];
      break;

    case 1:
      CpuCsrAccessError ("Invalid bus number 0x%2X.\n", (reg & 0x0ff00000) >> 20);
      ASSERT (FALSE);

      //
      // Uncore Bus
      //
      break;

    case 2:
      //
      // PCH on Bus 0
      //
      bus = 0;
      break;

    default:
      //
      // non-defined value
      //
      bus = 0;
      CpuCsrAccessError ("Attempt to access undefined bus number.\n");
      break;
  }

  //
  // Form address. Cast address to a UINTN before casting to a pointer
  // because pointers are 32bit in Pei, 64bit in Dxe. UINTN handles both cases.
  //

  regAddr = (UINT8 *) (UINTN) (CpuCsrAccessVar->mmCfgBase + (bus << 20) + (reg & 0x000FFFFF));


  //
  // bus + Dev:Func:Offset
  // Check register size and read data
  //
  switch (reg >> 28) {
    case sizeof (UINT32):
      data = *(volatile UINT32 *) regAddr;
      break;

    case sizeof (UINT16):
      data = *(volatile UINT16 *) regAddr;
      break;

    case sizeof (UINT8):
      data = *(volatile UINT8 *) regAddr;
      break;

    default:
      CpuCsrAccessError ("Invalid register size in reg = 0x%X.\n", reg);
      break;
  }

  PciCfgAccessTraceCap (socket, regAddr, reg, data, READ_ACCESS);

  if (EFI_ERROR (Status)) {
    RcDebugPrint (SDBG_DEFAULT, "ReadPciCfg: data may not be valid.\n");
  }
  //
  // Return data
  //
  return data;
}

/**

  Writes specified data to PCI configuration space using the MMIO mechanism

  @param[in] socket  - CPU Socket Node number (Socket ID)
  @param[in] reg     - input parameter "reg" uses the format in the Bus_Dev_Func_CFG.H files
  @param[in] data    - Value to write

**/
VOID
WritePciCfg (
  IN  UINT8       socket,
  IN  UINT32      reg,
  IN  UINT32      data
  )
{
  UINT8   *regAddr;
  UINT32  bus = 0;
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;


  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  CpuCsrAccessCount ();

  //
  // determine if targeting IIO bus or Uncore Bus (see if bus# filed of the register address = 0)
  //
  if (socket > 0) {
    CpuCsrAccessError ("Invalid socket number %u.\n", socket);
    ASSERT (FALSE);
  }

  switch ((reg & 0x0ff00000) >> 20) {
    case 0:
      //
      // IIO Bus
      //
      bus = CpuCsrAccessVar->SocketFirstBus[socket];
      break;

    case 1:
      CpuCsrAccessError ("Invalid bus number 0x%2X.\n", (reg & 0x0ff00000) >> 20);
      ASSERT (FALSE);

      //
      // Uncore Bus
      //
//      bus = CpuCsrAccessVar.busUncore[socket];
      break;

    case 2:
      //
      // PCH on Bus 0
      //
      bus = 0;
      break;

    default:
      //
      // non-defined value
      //
      bus = 0;
      CpuCsrAccessError ("Attempt to access undefined bus number.\n");
      break;
  }

  //
  // Form address. Cast address to a UINTN before casting to a pointer
  // because pointers are 32bit in Pei, 64bit in Dxe. UINTN handles both cases.
  //

  regAddr = (UINT8 *) (UINTN) (CpuCsrAccessVar->mmCfgBase + (bus << 20) + (reg & 0x000FFFFF));

  //
  // bus + Dev:Func:Offset
  // Check register size and write data
  //
  switch (reg >> 28) {
    case sizeof (UINT32):
      *(volatile UINT32 *) regAddr = data;
      break;

    case sizeof (UINT16):
      *(volatile UINT16 *) regAddr = (UINT16) data;
      break;

    case sizeof (UINT8):
      *(volatile UINT8 *) regAddr = (UINT8) data;
      break;

    default:
      CpuCsrAccessError ("Invalid register size in reg = 0x%X.\n", reg);
      break;
  }

  PciCfgAccessTraceCap (socket, regAddr, reg, data, WRITE_ACCESS);
  PciCfgAccessWriteToLog (regAddr, reg, data);
}

/**

  Get PCI configuration space used the MMIO mechanism

  @param[in] socket  - CPU Socket Node number (Socket ID)
  @param[in] reg     - input parameter "reg" uses the format in the Bus_Dev_Func_CFG.H files

  @retval Address of requested reg

**/
UINT32
GetPciCfgAddress (
  IN  UINT8       socket,
  IN  UINT32      reg
  )
{
  UINT32  bus = 0;
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  CpuCsrAccessCount ();

  if (socket > 0) {
    CpuCsrAccessError ("Invalid socket number %u.\n", socket);
    ASSERT (FALSE);
  }
  //
  // determine if targeting IIO bus or Uncore Bus (see if bus# filed of the register address = 0)
  //
  switch ((reg & 0x0ff00000) >> 20) {
    case 0:
      //
      // IIO Bus
      //
      bus = CpuCsrAccessVar->SocketFirstBus[socket];
      break;

    case 1:
      CpuCsrAccessError ("Invalid bus number 0x%2X.\n", (reg & 0x0ff00000) >> 20);
      ASSERT (FALSE);

      //
      // Uncore Bus
      //
//      bus = CpuCsrAccessVar.busUncore[socket];
      break;

    case 2:
      //
      // PCH on Bus 0
      //
      bus = 0;
      break;

    default:
      //
      // non-defined value
      //
      bus = 0;
      CpuCsrAccessError ("Attempt to access undefined bus number.\n");
      break;
  }
  //
  // Form address
  //
  return (CpuCsrAccessVar->mmCfgBase + (bus << 20) + (reg & 0x000FFFFF));
}
