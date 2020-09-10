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

#include <Library/RcDebugLib.h>
#include <Library/UsraRegisterLogSkipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Chip/Include/CpuPciAccess.h>

#ifdef MRC_TRACE
#ifdef DEBUG_CODE_BLOCK

CHAR8 *mTraceCapString = "*%d, 1, 0x%x, %x, %d, ";
CHAR8 *mInvalidRegSizeString = "\nInvalid Register Size in RegOffset = 0x%X\n";
CHAR8 *mMovQwordString = "\n  mov QWORD PTR ds:[0%08xh], 0%016xh\n";
CHAR8 *mMovDwordString = "\n  mov DWORD PTR ds:[0%08xh], 0%08xh\n";
CHAR8 *mMovWordString = "\n  mov  WORD PTR ds:[0%08xh], 0%04xh\n";
CHAR8 *mMovByteString = "\n  mov  BYTE PTR ds:[0%08xh], 0%02xh\n";
CHAR8 *mFormatHex64String = "0x%016x\n";
CHAR8 *mFormatHex32String = "0x%08x\n";
CHAR8 *mFormatHex16String = "0x%04x\n";
CHAR8 *mFormatHex8String = "0x%02x\n";

/**
    Capture the CSR access in format:

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

    @param SocId     - socket ID of the processor executing CSR access
    @param RegAddr   - CSR address in BDFO format
    @param RegOffset - CSR offset in Box/Type format
    @param Offset    - CSR offset in Box/Type format
    @param Data      - Data being read/written
    @param Size      - Size of the CSR being accessed
    @param RwFlag    - 0 for read, 1 for write

    @retval None

**/
VOID
CsrLongTraceCap (
  UINT8 SocId,
  UINT8 *RegAddr,
  CSR_OFFSET RegOffset,
  UINT32 Offset,
  UINT32 Data,
  UINT8  Size,
  UINT8 RwFlag
)
{

  if (GetDebugLevel () & SDBG_TRACE) {
    if (RwFlag == READ_ACCESS) {
      if (!(SkipCsrReg (RegOffset.Data))) {
        AcquirePrintControl ();
        RcDebugPrint (SDBG_DEFAULT, mTraceCapString, SocId, RegAddr, 0, Size);

        //
        // Determine register size
        //
        switch (Size) {
        case  UsraWidth32:
          RcDebugPrint (SDBG_DEFAULT, mFormatHex32String, (UINT32) Data);
          break;

        case  UsraWidth16:
          RcDebugPrint (SDBG_DEFAULT, mFormatHex16String, (UINT16) Data);
          break;


        case  UsraWidth8:
          RcDebugPrint (SDBG_DEFAULT, mFormatHex8String, (UINT8) Data);
          break;

        default:
          RcDebugPrint (SDBG_DEFAULT, mInvalidRegSizeString, RegOffset.Data);
          RC_FATAL_ERROR (FALSE, ERR_INVALID_REG_ACCESS, RC_FATAL_ERROR_MINOR_CODE_5);
        }
        ReleasePrintControl ();
      }
    } else {
      if (!(SkipCsrReg (RegOffset.Data))) {
        AcquirePrintControl ();
        RcDebugPrint (SDBG_DEFAULT, mTraceCapString, SocId, RegAddr, 1, Size);

        //
        // Determine register size
        //
        switch (Size) {
        case  UsraWidth32:
          RcDebugPrint (SDBG_DEFAULT, mFormatHex32String, (UINT32) Data);
          break;

        case  UsraWidth16:
          RcDebugPrint (SDBG_DEFAULT, mFormatHex16String, (UINT16) Data);
          break;


        case  UsraWidth8:
          RcDebugPrint (SDBG_DEFAULT, mFormatHex8String, (UINT8) Data);
          break;

        default:
          RcDebugPrint (SDBG_DEFAULT, mInvalidRegSizeString, RegOffset.Data);
          RC_FATAL_ERROR (FALSE, ERR_INVALID_REG_ACCESS, RC_FATAL_ERROR_MINOR_CODE_5);
        }
        ReleasePrintControl ();
      }
    }
  }
  //
  // if SDBG_TRACE
  //
}
#endif //DEBUG_CODE_BLOCK
#endif //MRC_TRACE


/**

    Capture CSR write as a mov instruction for EX platform

    @param RegAddr   - CSR address in BDFO format
    @param RegOffset - CSR offset in Box/Type format
    @param Data      - Data being written
    @param Offset    - CSR offset in Box/Type format
    @param Size      - Size of the CSR being accessed
    @param InstId    - The Box Instance Id.

    @retval None

**/
VOID
CsrUBiosWriteLog (
  UINT8 *RegAddr,
  CSR_OFFSET RegOffset,
  UINT32     Data,
  UINT32     Offset,
  UINT8      Size,
  UINT8      InstId
  )
{
  BOOLEAN NeedPrint;
  NeedPrint = FALSE;

#ifdef IA32
#ifdef DEBUG_CODE_BLOCK
  //
  // Only print stuff uBIOS cares about
  // Only care about CPU Bus 0 and 1 writes (skip PCH writes)
  //
  //if  (((((UINT32 )RegAddr & 0x0ff00000) >> 20) == 0) || ((((UINT32 )RegAddr & 0x0ff00000) >> 20) == 1)) {
  //
  // skip printing Scratch pad writes and SMBUS
  //
  if (!SkipAsmReg (Offset, Data)) {
    if (IsRegFilterPassSmb (Offset)) {
      NeedPrint = TRUE;
    }
  }

  if (NeedPrint == TRUE) {

    AcquirePrintControl ();
    //
    // Determine register size
    //
    RcDebugPrint (
      SDBG_DEFAULT,
      "\nuBIOSTag AddrTypeCSR Boxtype:0x%02x;Funcblk:0x%x;Inst:0x%x;Address: [0%08xh], ",
      (UINT16)RegOffset.Bits.boxtype,
      (UINT8)RegOffset.Bits.funcblk,
      InstId,
      RegAddr
      );
    switch (Size) {

    case  UsraWidth32:
      RcDebugPrint (SDBG_DEFAULT, "Value: 0%08xh", (UINT32) Data);
      RcDebugPrint (SDBG_DEFAULT, mMovDwordString, RegAddr, (UINT32) Data);
      break;

    case  UsraWidth16:
      RcDebugPrint (SDBG_DEFAULT, "Value: 0%04xh", (UINT16) Data);
      RcDebugPrint (SDBG_DEFAULT, mMovWordString, RegAddr, (UINT16) Data);
      break;

    case  UsraWidth8:
      RcDebugPrint (SDBG_DEFAULT, "Value: 0%02xh", (UINT8) Data);
      RcDebugPrint (SDBG_DEFAULT, mMovByteString, RegAddr, (UINT8) Data);
      break;

    default:
      RcDebugPrint (SDBG_DEFAULT, mInvalidRegSizeString, RegOffset.Data);
      RC_FATAL_ERROR (FALSE, ERR_INVALID_REG_ACCESS, RC_FATAL_ERROR_MINOR_CODE_7);
    }
    ReleasePrintControl ();
  }
#endif //DEBUG_CODE_BLOCK
#endif //IA32
}

/**

    Capture PCIE write as a mov instruction for EX platform

    @param[in] RegAddr      - PCIE address
    @param[in] Data         - Data being written
    @param[in] Bus          - Bus number
    @param[in] Dev          - Device number
    @param[in] Func         - Function number
    @param[in] Size         - Size of the register being accessed
**/
VOID
PcieUBiosWriteLog (
  UINT32     RegAddr,
  UINT32     Data,
  UINT8      Bus,
  UINT8      Dev,
  UINT8      Func,
  UINT8      Size
  )
{
#ifdef DEBUG_CODE_BLOCK
  AcquirePrintControl ();
  //
  // Determine register size
  //
  RcDebugPrint (
    SDBG_DEFAULT,
    "\nuBIOSTag AddrTypePCIE Bus:0x%02x;Dev:0x%02x;Fun:0x%x; Address:[0%08xh], ",
    Bus,
    Dev,
    Func,
    RegAddr
    );

  switch (Size) {

  case  UsraWidth32:
    RcDebugPrint (SDBG_DEFAULT, "Value: 0%08xh", (UINT32) Data);
    RcDebugPrint (SDBG_DEFAULT, mMovDwordString, RegAddr, (UINT32) Data);
    break;

  case  UsraWidth16:
    RcDebugPrint (SDBG_DEFAULT, "Value: 0%04xh", (UINT16) Data);
    RcDebugPrint (SDBG_DEFAULT, mMovWordString, RegAddr, (UINT16) Data);
    break;

  case  UsraWidth8:
    RcDebugPrint (SDBG_DEFAULT, "Value: 0%02xh", (UINT8) Data);
    RcDebugPrint (SDBG_DEFAULT, mMovByteString, RegAddr, (UINT8) Data);
    break;

  default:
    RcDebugPrint (SDBG_DEFAULT, "\nInvalid Register Size");
    RC_FATAL_ERROR (FALSE, ERR_INVALID_REG_ACCESS, RC_FATAL_ERROR_MINOR_CODE_7);
  }
  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
}
/**

    Capture PCIIO write as a mov instruction

    @param ioAddress    - IO Address to use
    @param ioDataAddr - IO Address to use
    @param AddrData   - Data to write to IO Address
    @param data      - Data to write to IO Address

    @retval None

**/
VOID
PciIoAccessWriteToLog (
  UINT16   ioAddress,
  UINT16   ioDataAddr,
  UINT32   AddrData,
  UINT32   data
  )
{
  DEBUG ((EFI_D_ERROR, "\n  mov dx,  0%04xh\n", ioAddress));
  DEBUG ((EFI_D_ERROR, "  mov eax, 0%08xh\n", AddrData));
  DEBUG ((EFI_D_ERROR, "  out dx,  eax\n"));
  DEBUG ((EFI_D_ERROR, "  mov dx,  0%04xh\n", ioDataAddr));
  DEBUG ((EFI_D_ERROR, "  mov eax, 0%08xh\n", data));
  DEBUG ((EFI_D_ERROR, "  out dx,  eax\n"));
}

/**
  Write out CSR traces/logs for Mem/Cfg types

  @param[in] Socket          The socket for the access being traced
  @param[in] RegAddr         An address which was read or written to
  @param[in] Data            Pointer to the data that was read or written
  @param[in] RwFlag          Whether a read or write occurred
  @param[in] AccessWidth     The Access width for 8, 16, 32, 64 -bit access. See USRA_ACCESS_WIDTH enum.

**/
VOID
CsrShortTraceCap (
  IN UINT32 Socket,
  IN UINTN  RegAddr,
  IN VOID   *Data,
  IN UINT8  RwFlag,
  IN UINT8  AccessWidth
  )
{
#ifdef DEBUG_CODE_BLOCK
  CpuCsrAccessCount ();

#ifdef MRC_TRACE
  if (GetDebugLevel () & SDBG_TRACE) {
    AcquirePrintControl ();
    RcDebugPrint (SDBG_DEFAULT,
                  mTraceCapString,
                  Socket,
                  RegAddr,
                  (RwFlag == READ_ACCESS) ? 0 : 1, // Ternary to print read/write access
                  AccessWidth);

    //
    // Determine register size
    //
    switch (AccessWidth) {
    case UsraWidth64:
      RcDebugPrint (SDBG_DEFAULT, mFormatHex64String, *(UINT64 *) Data);
      break;

    case UsraWidth32:
      RcDebugPrint (SDBG_DEFAULT, mFormatHex32String, *(UINT32 *) Data);
      break;

    case UsraWidth16:
      RcDebugPrint (SDBG_DEFAULT, mFormatHex16String, *(UINT16 *) Data);
      break;

    case UsraWidth8:
      RcDebugPrint (SDBG_DEFAULT, mFormatHex8String, *(UINT8 *) Data);
      break;

    default:
      RcDebugPrint (SDBG_DEFAULT, mInvalidRegSizeString, Data);
      RC_FATAL_ERROR (FALSE, ERR_INVALID_REG_ACCESS, RC_FATAL_ERROR_MINOR_CODE_5);
    }
    ReleasePrintControl ();
  }
#endif //MRC_TRACE

#ifdef IA32
  if ((RwFlag == WRITE_ACCESS) && UbiosGenerationEnabled ()) {
    AcquirePrintControl ();
    switch (AccessWidth) {
    case UsraWidth64:
      RcDebugPrint (SDBG_DEFAULT, mMovQwordString, RegAddr, *(UINT64 *) Data);
      break;

    case UsraWidth32:
      RcDebugPrint (SDBG_DEFAULT, mMovDwordString, RegAddr, *(UINT32 *) Data);
      break;

    case UsraWidth16:
      RcDebugPrint (SDBG_DEFAULT, mMovWordString, RegAddr, *(UINT16 *) Data);
      break;

    case UsraWidth8:
      RcDebugPrint (SDBG_DEFAULT, mMovByteString, RegAddr, *(UINT8 *) Data);
      break;

    default:
      RcDebugPrint (SDBG_DEFAULT, mInvalidRegSizeString, Data);
      RC_FATAL_ERROR (FALSE, ERR_INVALID_REG_ACCESS, RC_FATAL_ERROR_MINOR_CODE_7);
    }
    ReleasePrintControl ();
  }
#endif // IA32

#endif // DEBUG_CODE_BLOCK
}
