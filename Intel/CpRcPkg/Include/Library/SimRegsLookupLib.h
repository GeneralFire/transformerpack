/** @file
  SimRegsLookupLib.h

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

#ifndef  __SIM_REGS_LOOKUP_LIB_H__
#define  __SIM_REGS_LOOKUP_LIB_H__

//
// Register types used to divide up SimRegs definitions into small groups related by address type
//
typedef enum {
  PcieBdfReg,
  HbmIoReg,
  HbmMcReg,
  HbmMcCpgcReg,
  HbmDfxMiscReg,
  DdrIoReg,
  DdrMcReg,
  DdrDfxMiscReg,
  CadbMcReg,
  CpgcMcReg,
  FmcReg,
  SmeeReg,
  I3cScfReg,
  I3cSidebandReg,
  MaxSimRegsType
} SIM_REGS_TYPE;

/**

  Get socket address with logical bus number from a PCIE physical address

  @param[in]  PhysicalAddress  - Register address (physical bus)
  @param[out] Socket           - Socket for address
  @param[out] LogicalAddress   - Register address (logical bus)

  @retval EFI_SUCCESS if socket found
          EFI_NOT_FOUND otherwise

**/
EFI_STATUS
GetSocketAddr (
  IN    UINT32  PhysicalAddress,
  OUT   UINT8   *Socket,
  OUT   UINT32  *LogicalAddress
  );

/**

  Given a physical register address, return a pointer to a storage location for the
  simulated register and a string of the register name.

  @param[in]  Host             - Pointer to the system Host (root) structure
  @param[in]  RegisterAddress  - Register address in MMCFG space

  @retval Pointer to similation storage for the register

**/
UINT32 *
SimAddrCr (
  IN    PSYSHOST  Host,
  IN    UINT32    PhyscialAddress
  );

/**

    Read a simulated register

    @param[in]  Host              - Pointer to the system Host (root) structure
    @param[in]  RegisterAddress   - Physical register address
    @param[out] Data              - Pointer to caller's data storage

    @retval EFI_SUCCESS

**/
EFI_STATUS
SimRegRead (
  IN    PSYSHOST  Host,
  IN    UINT32    RegisterAddress,
  OUT   UINT32    *Data
  );

/**

    Write a simulated register

    @param[in]  Host              - Pointer to the system Host (root) structure
    @param[in]  RegisterAddress   - Physical register address
    @param[in]  Data              - Pointer to caller's data

    @retval EFI_SUCCESS

**/
EFI_STATUS
SimRegWrite (
  IN    PSYSHOST  Host,
  IN    UINT32    PhysicalAddr,
  IN    UINT32    *ptr
  );

/**

  Interface for register read/write for simulation environment

  @param[in]   Host               - Pointer to the system Host (root) structure
  @param[in]   Scoket             - CPU Socket Node number (Socket ID)
  @param[in]   RegisterAddress    - Register address
  @param[in]   Size               - Register size (1,2,4)
  @param[in]   Data               - Data to be written for WRITE
  @param[in]   Mode               - READ_ACCESS or WRITE_ACCESS

  @retval data read for READ_ACCESS

**/
UINT32
CpuSimRegAccess (
  IN    UINT8    Socket,
  IN    UINT32   RegisterAddress,
  IN    UINT8    Size,
  IN    UINT32   Data,
  IN    UINT8    Mode
  );
#endif  //__SIM_REGS_LOOKUP_LIB_H__
