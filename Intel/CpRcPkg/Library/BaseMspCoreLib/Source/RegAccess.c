/** @file
  RegAccess.h
  This file handles register accesses.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2018 Intel Corporation. <BR>

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

#include "RegAccess.h"
#include <Library/MemoryCoreLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/KtiApi.h>
#include <Library/SimRegsLookupLib.h>
#include <Library/RcDebugLib.h>
#include "MspLibraries.h"
#include <Library/MspCoreLib.h>

extern VOID debugPrintMsp(UINT32,CHAR8*, ...);
//
// Memory Mapped IO
//
UINT32
Mmio32Read (
  IN        UINTN      RegisterAddress
)
{
  VOLATILE UINT32 *addr;
  addr = (UINT32 *) (RegisterAddress);

  return *addr;
}

VOID
Mmio32Write (
  IN        UINTN      RegisterAddress,
  IN        UINT32      Value
)
{
  PSYSHOST Host;

  VOLATILE UINT32 *addr;
  addr = (UINT32 *) (RegisterAddress);
  *addr = Value;

  if (UbiosGenerationEnabled ()) {
    Host = GetSysHostPointer ();
    RcDebugPrint (SDBG_DEFAULT, "\nuBIOSTag AddrTypeMMIO: [0%08xh], Value: 0%08xh\n", RegisterAddress, (UINT32) Value);
    RcDebugPrint (SDBG_DEFAULT, "  mov  DWORD PTR ds:[0%08xh], 0%08xh\n", RegisterAddress, (UINT32) Value);
  }
}

UINT32
GetRegisterAccessInfo (
  IN        MSP_DATA    *MspData,
  IN        UINT8       BoxType,
  IN        UINT8       Channel,
  IN        UINT8       Instance,
  IN  OUT   UINT32      *Command,
  IN  OUT   UINT8       *PortId,
  IN  OUT   UINT32      *Offset,
  IN  OUT   UINT8       *Bus,
  IN  OUT   UINT8       *Device,
  IN  OUT   UINT8       *Func,
  IN        UINT32      Mode
)
{
  UINT32      Error;
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  Error   = NoError;
  CpuCsrAccessVar = GetSysCpuCsrAccessVar();

  //
  // Provided into this function is the instance number within the channel, getRegAccess requires the total instance index.
  // This translates the channel to the instance number in the INSTANCE_PORT_OFFSET structure.
  //
  if (CHANNEL_TO_INSTANCE(BoxType,Channel) == 0xFF) {
    return InvalidInstance;
  } else {
    Instance += CHANNEL_TO_INSTANCE(BoxType,Channel);
  }
  //
  // If the Type is a PCI access, get the B/D/F.
  //
  if (INSTANCE_PORT_MAP(BoxType).AccessMethod == ePCI) {
    //
    // PCI Accesses
    //
    *Bus = CpuCsrAccessVar->StackBus[((PSYSHOST) GetSysHostPointer ())->var.mem.currentSocket][(INSTANCE_PORT_OFFSET(INSTANCE_PORT_MAP(BoxType).InstanceStartIndex + Instance).Port >> 8) & 0xff];
    *Device = (INSTANCE_PORT_OFFSET(INSTANCE_PORT_MAP(BoxType).InstanceStartIndex + Instance).Port >> 3) & 0x1f;
    *Func = INSTANCE_PORT_OFFSET(INSTANCE_PORT_MAP(BoxType).InstanceStartIndex + Instance).Port & 0x07;
    *Offset = 0;

  } else if (INSTANCE_PORT_MAP(BoxType).AccessMethod == eSB) {
    //
    // Sideband Accesses
    //
  } else if (INSTANCE_PORT_MAP(BoxType).AccessMethod == eBAR) {
    //
    // For BAR accesses, compile a full 32 bit offset from Port (upper 16 bits) and Offset (lower 16 bits).
    // We reuse the definition of Port for the upper 16 bits to save on code space in InstancePortOffset.
    //
    *Offset = (INSTANCE_PORT_OFFSET(INSTANCE_PORT_MAP(BoxType).InstanceStartIndex + Instance).Port << 16) + INSTANCE_PORT_OFFSET(INSTANCE_PORT_MAP(BoxType).InstanceStartIndex + Instance).Offset;
  }
  //
  // Get the sideband command to read/write this particular box type.
  // If the command is not read or write, do not attempt to change it.
  //
  if (Mode == ModeRead) {
    *Command = INSTANCE_PORT_MAP(BoxType).ReadOperation  << 24;
  } else if (Mode == ModeWrite) {
    *Command = INSTANCE_PORT_MAP(BoxType).WriteOperation << 24;
  } else {
    *Command = *Command << 24;
  }
  //
  // Handle all errors.
  //
  if (Error != NoError) {
    if (Mode == ModeRead) {
      RcDebugPrint (SDBG_MINMAX, "RegRead ERROR: ");
    } else if (Mode == ModeWrite) {
      RcDebugPrint (SDBG_MINMAX, "RegWrite ERROR: ");
    }
  }
  switch (Error) {
  case NoError:
    break;
  case InvalidInstance:
    RcDebugPrint (SDBG_MINMAX, "Invalid Instance 0x%x for Box 0x%d (offset = 0x%x)\n", Instance, BoxType, *Offset);
    break;
  default:
    break;
  }
  return Error;
}

UINT32
MemRegRead (
  IN        MSP_DATA        *MspData,
  IN        UINT8           BoxType,
  IN        UINT8           Channel,
  IN        UINT8           Instance,
  IN        REGISTER_ACCESS Register
)
{
  UINT32          Command;
  UINT8           PortId;
  UINT32          Data;
  UINT32          BaseOffset;
  UINT8           Bus;
  UINT8           Device;
  UINT8           Func;
  UINT32          *MemOffset;
  UINT8           OffsetShift;
  UINT8           ShiftLeftAmount;
  UINT8           ShiftRightAmount;
  UINT8           MemSsType;
  UINT32          TotalValue;
  REGISTER_ACCESS TempRegister;
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  Data              = 0;
  OffsetShift       = 0;
  ShiftLeftAmount   = 0;
  ShiftRightAmount  = 0;
  TotalValue        = 0;
  CpuCsrAccessVar   = GetSysCpuCsrAccessVar ();

  if (GetRegisterAccessInfo (MspData, BoxType, Channel, Instance, &Command, &PortId, &BaseOffset, &Bus, &Device, &Func, ModeRead) == NoError) {
    //
    // If we are dealing with a PCI or MEM type register, the offset could
    // not be DWORD aligned. In this case, we may need to do multiple reads to
    // piece together the entire register since reads to PCI or MEM should be
    // DWORD aligned.
    //
    if (INSTANCE_PORT_MAP(BoxType).AccessMethod == ePCI || INSTANCE_PORT_MAP(BoxType).AccessMethod == eBAR) {
      OffsetShift       = (Register.Offset % 4);
      ShiftLeftAmount   = OffsetShift * 8;
      ShiftRightAmount  = 32 - ShiftLeftAmount;
      //
      // Align the offset to the next DWORD down (i.e. offset 0x33 will be offset 0x30 after this).
      //
      Register.Offset  -= OffsetShift;
      //
      // If the offset needed to be shifted (ie. it's not DWORD aligned) or the
      // mask is more than 32 bits, we need to recurse to read another DWORD.
      //
      if (OffsetShift != 0 || Register.Mask > 0xFFFFFFFF) {
        //
        // Create a temporary register mask in case we need to recurse with a new mask.
        //
        TempRegister.Mask     = Register.Mask >> ShiftRightAmount;
        TempRegister.Offset   = Register.Offset + 4;
        if (TempRegister.Mask != 0) {
          TotalValue = MemRegRead (MspData, BoxType, Channel, Instance, TempRegister);
        }
      }
      //
      // We've completed recursion, building up all upper DWORDs of the register. Now we
      // need to read the final DWORD. Since we've subtracted OffsetShift from the original
      // offset, we need to align the mask with the new DWORD aligned offset.
      //
      Register.Mask <<= ShiftLeftAmount;
    }
    //
    // Define the MemSsType
    //
    MemSsType   = CpuCsrAccessVar->MemSsType[((PSYSHOST) GetSysHostPointer ())->var.mem.currentSocket];
    if (INSTANCE_PORT_MAP(BoxType).ForceHbmLevel) {
      CpuCsrAccessVar->MemSsType[((PSYSHOST) GetSysHostPointer ())->var.mem.currentSocket] = Hbm2MemSs;
    }
    //
    // If the action is to be taking place and the transaction is a PCI, then just do the PCI Access.
    //
    if (INSTANCE_PORT_MAP(BoxType).AccessMethod == ePCI) {
      //
      // PCI access
      //
      Data = PCI_CFG_32B_READ(GetmmCfgBaseL (((PSYSHOST) GetSysHostPointer ())->var.mem.currentSocket), Bus, Device, Func, Register.Offset + BaseOffset);
      RcDebugPrint (SDBG_REG_ACCESS, "PCI read %02X/%02X/%01X, Offset=0x%X, Mask=0x%08X, Value=0x%08X\n",
        Bus, Device, Func, Register.Offset + BaseOffset, (UINT32)Register.Mask, (UINT32)Data);
    } else if (INSTANCE_PORT_MAP(BoxType).AccessMethod == eBAR) {
      //
      // Memory BAR access
      //
      MemOffset = (UINT32 *)(UINTN)(DYNAMIC_VARS(Channel,INSTANCE_PORT_MAP(BoxType).AccessBase) + ((Register.Offset + BaseOffset) & 0xFFFFFFFF));
      Data = (UINT32)*MemOffset;
      RcDebugPrint (SDBG_REG_ACCESS, "MEM read to offset=0x%08X; data=0x%08X\n", MemOffset, (UINT32) Data);
    } else {
      //
      // Sideband access.
      //
    }
    TotalValue = (TotalValue << ShiftRightAmount) | ((Data & (UINT32) Register.Mask) >> ShiftLeftAmount);
    CpuCsrAccessVar->MemSsType[((PSYSHOST) GetSysHostPointer ())->var.mem.currentSocket] = MemSsType;
    return TotalValue;
  }
  return 0xFFFFFFFF;
}

VOID
MemRegWrite (
  IN        MSP_DATA        *MspData,
  IN        UINT8           BoxType,
  IN        UINT8           Channel,
  IN        UINT8           Instance,
  IN        REGISTER_ACCESS Register,
  IN        UINT32          Data,
  IN        UINT8           Be
)
{
  UINT32          Command;
  UINT8           PortId;
  UINT32          BaseOffset;
  UINT8           Bus;
  UINT8           Device;
  UINT8           Func;
  UINT32          *MemOffset;
  UINT8           OffsetShift;
  UINT8           ShiftLeftAmount;
  UINT8           ShiftRightAmount;
  UINT8           MemSsType;
  UINT32          TotalValue;
  REGISTER_ACCESS TempRegister;
  UINT32          TempData;
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  OffsetShift       = 0;
  ShiftLeftAmount   = 0;
  ShiftRightAmount  = 0;
  TotalValue        = 0;
  CpuCsrAccessVar   = GetSysCpuCsrAccessVar ();

  if (GetRegisterAccessInfo (MspData, BoxType, Channel, Instance, &Command, &PortId, &BaseOffset, &Bus, &Device, &Func, ModeWrite) == NoError) {
    //
    // If we are dealing with a PCI or MEM type register, the offset could
    // not be DWORD aligned. In this case, we may need to do multiple writes to
    // piece together the entire register since reads to PCI or MEM should be
    // DWORD aligned.
    //
    if (INSTANCE_PORT_MAP(BoxType).AccessMethod == ePCI || INSTANCE_PORT_MAP(BoxType).AccessMethod == eBAR) {
      OffsetShift       = (Register.Offset % 4);
      ShiftLeftAmount   = OffsetShift * 8;
      ShiftRightAmount  = 32 - ShiftLeftAmount;
      //
      // Align the offset to the next DWORD down (i.e. offset 0x33 will be offset 0x30 after this).
      //
      Register.Offset  -= OffsetShift;
      //
      // If the offset needed to be shifted (ie. it's not DWORD aligned) or the
      // mask is more than 32 bits, we need to recurse to read another DWORD.
      //
      if (OffsetShift != 0 || Register.Mask > 0xFFFFFFFF) {
        //
        // Write the next 32 bits (0xFFFFFFFF), but leave any bits above that
        // left over so if we need to recurse again, we can.
        //
        TempRegister.Mask     = Register.Mask >> ShiftRightAmount;
        TempRegister.Offset   = Register.Offset + 4;
        TempRegister.ShiftBit = Register.ShiftBit;
        TempData              = Data >> ShiftRightAmount;
        if (TempRegister.Mask != 0) {
          MemRegWrite (MspData, BoxType, Channel, Instance, TempRegister, TempData, 0xF);
        }
      }
      //
      // We've completed recursion, writing all upper DWORDs of the register. Now we
      // need to write the final DWORD. Since we've subtracted OffsetShift from the original
      // offset, we need to align the mask with the new DWORD aligned offset.
      //
      Register.Mask <<= ShiftLeftAmount;
      //
      // If the register is DWORD aligned and we're writing all 32 bits, skip the read.
      // Else, need to read/modify/write register.
      //
      if (OffsetShift != 0 || (UINT32) Register.Mask != 0xFFFFFFFF) {
        //
        // Since we're doing a 32 bit read/modify/write, only use the lower 32 bits of
        // the mask.
        //
        TempRegister.Mask = (UINT32) ~Register.Mask;
        TempRegister.Offset = Register.Offset;
        TotalValue = MemRegRead (MspData, BoxType, Channel, Instance, TempRegister);
      }
      //
      // Now that we have the fields read from the register and masked, OR in the new data.
      //
      TotalValue |= (Data << Register.ShiftBit);
      Data        = TotalValue << ShiftLeftAmount;
    }
    //
    // Define the MemSsType
    //
    MemSsType   = CpuCsrAccessVar->MemSsType[((PSYSHOST) GetSysHostPointer ())->var.mem.currentSocket];
    if (INSTANCE_PORT_MAP(BoxType).ForceHbmLevel) {
      CpuCsrAccessVar->MemSsType[((PSYSHOST) GetSysHostPointer ())->var.mem.currentSocket] = Hbm2MemSs;
    }
    //
    // If the action is to be taking place and the transaction is a PCI, then just do the PCI Access.
    //
    if (INSTANCE_PORT_MAP(BoxType).AccessMethod == ePCI) {
      //
      // PCI access
      //
      RcDebugPrint (SDBG_REG_ACCESS, "PCI write to %02X/%02X/%01X, Offset=0x%X, Mask=0x%08X, Data=0x%08X\n",
        Bus, Device, Func, Register.Offset + BaseOffset, (UINT32)Register.Mask, (UINT32)Data);
      PCI_CFG_32B_WRITE(GetmmCfgBaseL (((PSYSHOST) GetSysHostPointer ())->var.mem.currentSocket), Bus, Device, Func, Register.Offset + BaseOffset, (UINT32)Data);
    } else if (INSTANCE_PORT_MAP(BoxType).AccessMethod == eBAR) {
      //
      // Memory BAR access
      //
      MemOffset = (UINT32 *)(UINTN)(DYNAMIC_VARS(Channel,INSTANCE_PORT_MAP(BoxType).AccessBase) + ((Register.Offset + BaseOffset) & 0xFFFFFFFF));
      RcDebugPrint (SDBG_REG_ACCESS, "MEM write to offset=0x%08X; data=0x%08X\n", MemOffset, (UINT32) Data);
      *MemOffset = (UINT32)Data;

      if (UbiosGenerationEnabled ()) {
        RcDebugPrint (SDBG_DEFAULT, "\nuBIOSTag AddrTypeCSR Boxtype:0x%x;Channel:0x%x;Inst:0x%x; Offset:[0%08xh], Value:0%08xh\n", BoxType, Channel, Instance, (UINT32)(UINTN) MemOffset, (UINT32) Data);
        RcDebugPrint (SDBG_DEFAULT, "  mov  DWORD PTR ds:[0%08xh], 0%08xh\n", (UINT32)(UINTN) MemOffset, (UINT32) Data);
      }
    } else {
      //
      // Sideband Access
      //
    }
    CpuCsrAccessVar->MemSsType[((PSYSHOST) GetSysHostPointer ())->var.mem.currentSocket] = MemSsType;
  }
}


UINT32
MemFieldRead (
  IN        MSP_DATA        *MspData,
  IN        UINT8           BoxType,
  IN        UINT8           Channel,
  IN        UINT8           Instance,
  IN        REGISTER_ACCESS Register
  )
{
  UINT32 Value;
  //
  // Read the entire register specified.
  //
  Value = MemRegRead (MspData, BoxType, Channel, Instance, Register);
  //
  // Shift it over so the field starting at ShiftBit is returned.
  //
  Value >>= Register.ShiftBit;
  //
  // Return the Value.
  //
  return Value;
}

VOID
MemFieldWrite (
  IN        MSP_DATA        *MspData,
  IN        UINT8           BoxType,
  IN        UINT8           Channel,
  IN        UINT8           Instance,
  IN        REGISTER_ACCESS Register,
  IN        UINT32          Value,
  IN        UINT8           Be
  )
{
  UINT32  TempValue;

  if (Register.Offset == 0xffff) {
    return;
  }
  //
  // Read the 32-bit register.
  //
  TempValue = MemRegRead (MspData, BoxType, Channel, Instance, Register);
  //
  // Shift the inputted Value to the correct start location
  //
  TempValue |= (TempValue << Register.ShiftBit);
  //
  // Write the register back.
  //
  MemRegWrite (MspData, BoxType, Channel, Instance, Register, TempValue, Be);

  return;
}


