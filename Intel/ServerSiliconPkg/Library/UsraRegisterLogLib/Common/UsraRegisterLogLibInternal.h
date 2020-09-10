/** @file
  Implementation of UsraRegisterFilterLib class library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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

#include <Library/UsraAccessApi.h>
#include <Chip/Include/CpuPciAccess.h>

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
  );

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
  );

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
  );

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
  );

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
  );
