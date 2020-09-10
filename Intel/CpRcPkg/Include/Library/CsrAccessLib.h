/** @file
  Interface header file for the Csr Access library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _CSR_ACCESS_LIB_H_
#define _CSR_ACCESS_LIB_H_

#include <SystemInfoVar.h>
#include <Library/SystemInfoLib.h>

/**

    Count the CSR accesses.

    @param - None

**/

VOID
CpuCsrAccessCount (
  VOID
  );

/**

Get MMCFG base and limit

@retval 32-bit MMCFG base address

**/
UINT32
GetMmcfgAddress (
  VOID
  );

/**

  Get CPU socket id for the system boot strap processor
  This function only works on SBSP prior to KTIRC completion.

  @retval system boot strap processor socket ID

**/
UINT8
GetSbspSktId (
  VOID
  );

/**

  Enable the CSR address cache

**/
VOID
EnableCsrAddressCache (
  VOID
  );

/**

  Disable the CSR address cache

**/
VOID
DisableCsrAddressCache (
  VOID
  );

/**

    Update CpuCsrAccessVar structure.

    @param[out] CpuCsrAccessVar - pointer to CpuCsrAccessVar structure to be updated

**/
VOID
UpdateCpuCsrAccessVar_PEI (
  OUT CPU_CSR_ACCESS_VAR *CsrAccessVarPtr
  );

/**

  CsrAccess specific print to serial output if
  DEBUG_CODE_BLOCK is defined, then deadloop

  @param[in] Format    - string format
  @param[in] ...       - values matching the variables declared in
                         the Format parameter.  This is optional.

**/

#define CpuCsrAccessError(...) \
  { \
    RcDebugPrint (SDBG_DEFAULT, __VA_ARGS__); \
    ASSERT (FALSE); \
  }

//
// This macro must be used to access only ICH/PCH regs using
// PCI access routines in PciAccess.c. Note that bit[27:20] is encoded with 2 to indicate PCH/ICH bus
// to distinguish with IIO/Uncore buses.
//

#define  BUS_PCH  2

#define REG_ADDR( Bus, Dev, Func, Reg, Size ) ((Size << 28) + ((Bus + BUS_PCH) << 20) + (Dev << 15) + (Func << 12) + Reg)

/**

  Reads PCI configuration space using the MMIO mechanism

  @param[in] Socket  - CPU Socket Node number (Socket ID)
  @param[in] Reg     - input parameter "reg" uses the format in the Bus_Dev_Func_CFG.H files

  @retval Value in requested reg

**/
UINT32
ReadPciCfg (
  IN  UINT8       Socket,
  IN  UINT32      Reg
  );

/**

  Writes specified data to PCI configuration space using the MMIO mechanism

  @param[in] Socket  - CPU Socket Node number (Socket ID)
  @param[in] Reg     - input parameter "reg" uses the format in the Bus_Dev_Func_CFG.H files
  @param[in] Data    - Value to write

**/
VOID
WritePciCfg (
  IN  UINT8       Socket,
  IN  UINT32      Reg,
  IN  UINT32      Data
  );

/**

  Get PCI configuration space used the MMIO mechanism

  @param[in] Socket  - CPU Socket Node number (Socket ID)
  @param[in] Reg     - input parameter "reg" uses the format in the Bus_Dev_Func_CFG.H files

  @retval Address of requested reg

**/

UINT32
GetPciCfgAddress (
  IN  UINT8       Socket,
  IN  UINT32      Reg
  );

/**

  Returns regBase with true offset (after processing pseudo offset, if needed)

  @param[in] NodeId        - Memory controller index
  @param[in] ChIdOrBoxInst - DDR channel Id within a memory controller
                             or Box Instance (in case of non-MC boxes), 0 based, 0xFF if not used
  @param[in] RegBase       - Register offset; values come from the auto generated header file
                             may be a pseudo offset

  @retval Updated Register OFfset

**/

UINT32
MemPciCfgOffset (
  IN  UINT8    NodeId,
  IN  UINT8    ChIdOrBoxInst,
  IN  UINT32   RegBase
  );

/**

  Reads CPU Memory Controller configuration space using the MMIO mechanism

  @param[in] Socket        - Socket number
  @param[in] ChIdOrBoxInst - DDR channel Id within a memory controller
                             or Box Instance (in case of non-MC boxes), 0 based, 0xFF if not used
  @param[in] Offset        - Register offset; values come from the auto generated header file

  @retval Register value

**/
UINT32
MemReadPciCfgEp (
  IN  UINT8       Socket,
  IN  UINT8       ChIdOrBoxInst,
  IN  UINT32      Offset
  );

/**

  Reads CPU Memory Controller configuration space using the MMIO mechanism

  @param[in] Socket    - Socket number
  @param[in] Ch        - Channel number (0-based)
  @param[in] Offset    - Register offset; values come from the auto generated header file
  @param[in] ChOffset  - Channel offset

  @retval Register value

**/
UINT32
MemReadPciCfgMC_Ch (
  IN  UINT8    Socket,
  IN  UINT8    Ch,
  IN  UINT32   Offset,
  IN  UINT32   ChOffset
  );

/**

  Reads CPU Memory Controller configuration space using the MMIO mechanism

  @param[in] Socket        - Socket number
  @param[in] mcId          - Memory controller ID
  @param[in] Offset        - Register offset; values come from the auto generated header file

  @retval Register value

**/
UINT32
MemReadPciCfgMC (
  IN  UINT8       Socket,
  IN  UINT8       mcId,
  IN  UINT32      Offset
  );

/**

  Writes CPU Memory Controller configuration space using the MMIO mechanism

  @param[in] Socket        - Socket number
  @param[in] ChIdOrBoxInst - DDR channel Id within a memory controller
                             or Box Instance (in case of non-MC boxes), 0 based, 0xFF if not used
  @param[in] Offset        - Register offset; values come from the auto generated header file
  @param[in] Data          - Register data to be written

**/
VOID
MemWritePciCfgEp (
  IN  UINT8       Socket,
  IN  UINT8       ChIdOrBoxInst,
  IN  UINT32      Offset,
  IN  UINT32      Data
  );

/**

  Writes CPU Memory Controller configuration space using the MMIO mechanism

  @param[in] Socket   - Socket number
  @param[in] Ch       - Channel number (0-based)
  @param[in] Offset   - Register offset; values come from the auto generated header file
  @param[in] ChOffset - Channel offset
  @param[in] Data     - Register data to be written

**/
VOID
MemWritePciCfgMC_Ch (
  IN  UINT8 Socket,
  IN  UINT8 Ch,
  IN  UINT32 Offset,
  IN  UINT32 ChOffset,
  IN  UINT32 Data
  );

/**

  Writes CPU Memory Controller configuration space using the MMIO mechanism

  @param[in] Socket        - Socket number
  @param[in] mcId          - Memory controller ID
  @param[in] Offset        - Register offset; values come from the auto generated header file
  @param[in] Data          - Register data to be written

**/
VOID
MemWritePciCfgMC (
  IN  UINT8       Socket,
  IN  UINT8       mcId,
  IN  UINT32      Offset,
  IN  UINT32      Data
  );

/**

  Writes CPU Memory Controller configuration space using the MMIO mechanism

  @param[in] Socket        - Socket number
  @param[in] mcId          - Memory controller ID
  @param[in] Offset        - Register offset; values come from the auto generated header file
  @param[in] Data          - Register data to be written

**/
VOID
MemWritePciCfgMC_AllCh (
  IN  UINT8    Socket,
  IN  UINT8    mcId,
  IN  UINT32   Offset,
  IN  UINT32   Data
  );

/**

  Get the Memory Controller PCI config Address

  @param[in] Socket   - CPU Socket Node number (Socket ID)
  @param[in] mcId     - Memory controller ID
  @param[in] Offset   - Register offset; values come from the auto generated header file

  @retval Returns the return value from UsraGetCsrRegisterAddress

**/
UINTN
MemGetPciCfgMCAddr (
  IN  UINT8    Socket,
  IN  UINT8    mcId,
  IN  UINT32   Offset
  );

/**

  Get a Memory channel's EPMC Main value

  @param[in] Socket    - CPU Socket Node number (Socket ID)
  @param[in] RegBase   - MMIO Reg address of first base device

  @retval EPMC main value

**/
UINT32
MemReadPciCfgMain (
  IN  UINT8       Socket,
  IN  UINT32      RegBase
  );

/**

  Write a Memory channel's EPMC Main value

  @param[in] Socket    - CPU Socket Node number (Socket ID)
  @param[in] RegBase   - MMIO Reg address of first base device
  @param[in] Data      - Data to write

**/
VOID
MemWritePciCfgMain (
  IN  UINT8       Socket,
  IN  UINT32      RegBase,
  IN  UINT32      Data
  );

#endif  // #ifndef _CSR_ACCESS_LIB_H_
