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

#include <Library/MemoryCoreLib.h>
#include <Library/IoLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/SysHostPointerLib.h>

/*++

  Reads 8 bit IO port

  @param[in] IoAddress - IO Address to use

  @retval Value read from IoAddress

--*/

UINT8
InPort8_ (
  IN UINT16 IoAddress
  )
{
  UINT8 AlReg = 0;

  AlReg = IoRead8 (IoAddress);

  return AlReg;
}

/**

  Reads 8 bit IO port with serial debug output

  @param[in] IoAddress  - IO Address to use

  @retval Value read from IoAddress

**/

UINT8
InPort8 (
  IN UINT16   IoAddress
  )
{
  UINT8 AlReg;

  AlReg = InPort8_ (IoAddress);

#ifdef MRC_TRACE
  /*
   *Socket, Type, Address, Direction, Size, Data

   Where:
   - Socket = processor socket # that executes the instruction
   - Type = 0 for IO, or 1 for Memory
   - Address = 16-bit IO or 32-bit Memory address (the MMIO address will encode bus, device, function, offset according to MMCFG format)
   - Direction = 0 for Rd, or 1 for Wr
   - Size = 1, 2, or 4 bytes
   - Data = hex Data corresponding to the size

   For example:
   *0, 0, 0x80, 1, 1, 0xA0
   */
  {
    RcDebugPrint (SDBG_TRACE, "*%d, 0, 0x%x, 0, %d, 0x%02x\n", GetSysSocketId (), IoAddress, sizeof (UINT8), AlReg);
  }
#endif
  return AlReg;
}

/**

  Reads 16 bit IO port with serial debug output

  @param[in] IoAddress - IO Address to use

  @retval Value read from IoAddress

**/
UINT16
InPort16 (
  IN UINT16   IoAddress
  )
{
  UINT16  AxReg=0;

  AxReg = IoRead16 (IoAddress);

#ifdef MRC_TRACE
  {
    RcDebugPrint (SDBG_TRACE, "*%d, 0, 0x%x, 0, %d, 0x%04x\n", GetSysSocketId (), IoAddress, sizeof (UINT16), AxReg);
  }
#endif // MRC_TRACE
  return AxReg;
}

/**

  Reads 32 bit IO port with serial debug output

  @param[in] IoAddress - IO Address to use

  @retval Value read from ioAddress

**/
UINT32
InPort32 (
  IN UINT16   IoAddress
  )
{
  UINT32  EaxReg = 0;

  EaxReg = IoRead32 (IoAddress);

#ifdef MRC_TRACE
  {
    RcDebugPrint (SDBG_TRACE, "*%d, 0, 0x%x, 0, %d, 0x%08x\n", GetSysSocketId (), IoAddress, sizeof (UINT32), EaxReg);
  }
#endif // MRC_TRACE
  return EaxReg;
}

/*++

  Writes 8 bit IO port with designated value

  @param IoAddress - IO Address to use
  @param Data      - Data to write to IO Address

  @retval N/A

--*/

VOID
OutPort8_ (
  UINT16 IoAddress,
  UINT8  Data
  )
{
  IoWrite8 (IoAddress, Data);
}

/**

  Writes 8 bit IO port with serial debug output

  @param[in] IoAddress - IO Address to use
  @param[in] Data      - Data to write to IO Address

  @retval None

**/

VOID
OutPort8 (
  IN UINT16   IoAddress,
  IN UINT8    Data
  )
{
  OutPort8_ (IoAddress, Data);

#ifdef MRC_TRACE
  {
    RcDebugPrint (SDBG_TRACE, "*%d, 0, 0x%x, 1, %d, 0x%02x\n", GetSysSocketId (), IoAddress, sizeof (UINT8), Data);
  }
#endif
}

/**

  Writes 16 bit IO port with serial debug output

  @param[in] IoAddress - IO Address to use
  @param[in] Data      - Data to write to IO Address

  @retval None

**/

VOID
OutPort16 (
  IN UINT16   IoAddress,
  IN UINT16   Data
  )
{
  IoWrite16 (IoAddress, Data);

#ifdef MRC_TRACE
  {
    RcDebugPrint (SDBG_DEFAULT, "*%d, 0, 0x%x, 1, %d, 0x%04x\n", GetSysSocketId (), IoAddress, sizeof (UINT16), Data);
  }
#endif
}

/**

  Writes 32 bit IO port with serial debug output

  @param[in] IoAddress - IO Address to use
  @param[in] Data      - Data to write to IO Address

  @retval None

**/

VOID
OutPort32 (
  IN UINT16   IoAddress,
  IN UINT32   Data
  )
{
  IoWrite32 (IoAddress, Data);

#ifdef MRC_TRACE
  {
    RcDebugPrint (SDBG_TRACE, "*%d, 0, 0x%x, 1, %d, 0x%08x\n", GetSysSocketId (), IoAddress, sizeof (UINT32), Data);
  }
#endif
}
