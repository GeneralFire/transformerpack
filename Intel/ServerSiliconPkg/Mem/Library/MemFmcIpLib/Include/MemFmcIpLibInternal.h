/** @file
  Header file for the Mem FMC IP library implementation.

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

#ifndef _MEM_FMC_IP_LIB_INTERNAL_H_
#define _MEM_FMC_IP_LIB_INTERNAL_H_

#include <ReferenceCodeDataTypes.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/MemFmcIpLib.h>

#define FMC_STEPPING_MASK   0x33  // Used to mask reserved bits from revision field

#define ASSERT_BRST_A     0x1  // assert BRST_A
#define DE_ASSERT_BRST_A  0x2  // de-assert BRST_A
#define ASSERT_BRST_B     0x3  // assert BRST_B
#define DE_ASSERT_BRST_B  0x4  // de-assert BRST_B

#define MMRC_DEFAULT_VALUES 256
//
// Prototypes for function used ony within this implementation,
// this is not an API header.
//

EFI_STATUS
ReadFnvCfgSmbWorkerFunction (
  PSYSHOST          Host,
  UINT8             socket,
  UINT32            smbReg,
  SMB_DEVICE_STRUCT dev,
  volatile UINT32   *localData,
  UINT8             regSize
  );

/**

  Write NVMCTLR SMBUS dev/offset with specified data

  @param Host        - Pointer to the system Host (root) structure
  @param socket        - Processor socket ID
  @param ch          - DDR Channel ID
  @param reg         - byte offset for NVMCTLR
  @param data        - pointer to data to be read

  @retval 0 - success
  @retval 1 - failure

**/

EFI_STATUS
WriteFnvCfgSmb (
  PSYSHOST Host,
  UINT8  socket,
  UINT8  ch,
  UINT8  dimm,
  UINT32 reg,
  UINT32 data
  );

UINT32
WriteFnvCfgEmrs (
  PSYSHOST Host,
  UINT8  socket,
  UINT8  ch,
  UINT8  dimm,
  UINT32 reg,
  UINT32 data
  );

VOID
DdrtCosimDelay (
  IN UINT8 Delay
  );

EFI_STATUS
ReadFnvCfgSmb (
  PSYSHOST Host,
  UINT8  socket,
  UINT8  ch,
  UINT8  dimm,
  UINT32 reg,
  UINT32 *data
  );

EFI_STATUS
WriteFnvCfgSmbWorkerFunction (
  PSYSHOST          Host,
  UINT8             socket,
  UINT32            smbReg,
  SMB_DEVICE_STRUCT dev,
  UINT32            *localData,
  UINT8             regSize,
  UINT8             cmdCode
  );

VOID
FnvBcwRead (
  PSYSHOST Host,
  UINT8  socket,
  UINT8  ch,
  UINT8  dimm,
  UINT32 dpa,
  UINT8 data[MAX_STROBE / 2]
  );

VOID EmulationReadFnvCfgSmbus (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT32   reg,
  UINT32   mask,
  UINT8    logic
  );

VOID EmulationReadFnvCfg (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT32   reg,
  UINT32   mask,
  UINT8    logic
  );

/**

Routine Description: This is a generic Write to IO

  1. Write Data into Window Register MC_WDATA for Write only
  2. Write Bus, Device, and Function into Account Info
     Bits   [11]   - IA
     Bits   [10:8] - Bus - (Contrary to what the documentation may say, this is 2 for both NGN and NVMDIMM IO)
     Bits   [7:3]  - Device - Found in CSR description document (CSPEC/XML).
     Bits   [2:0]  - Function - Found in CSR description document (CSPEC/XML).
  3. Configure control Register
  4. Wait for completion for reads and nonposted writes
  @param Host        - Pointer to the system Host (root) structure
  @retval Data or Status depending on Read or Write

**/
UINT32
GetSetFnvIO (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT32   reg,
  UINT32   *data,
  UINT8    IA,
  UINT8    mode,
  UINT8    byteMask
  );

/**

  Routine Description: This function reads the Major and Minor Checkpoint of NGN training
  Bits  7 - 0 represent the Major Checkpoint
  Bits 15 - 8 represent the Minor Checkpoint
  @param Host        - Pointer to the system Host (root) structure
  @retval Data or Status depending on Read or Write
**/
UINT32
GetFnvTrainingStatus (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm
  );

UINT32
CheckMailboxReady (
  UINT8  Socket,
  UINT8  Ch,
  UINT8  Dimm
  );

/**

Routine Description: CheckLongOperationStatus Gets the long operation status

  @param Host             - Pointer to the system host (root) structure
  @param Socket           - Socket Number
  @param Ch               - DDR Channel ID
  @param Dimm             - DIMM Number
  @param Command          - Opcode and SubOpcode of the long operation
  @param PercentComplete  - Percentage of the command that has been completed
  @param Etc              - Estimated time to completion
  @param StatusCode       - Mailbox Status code of the long operation
  @param MbStatus         - Pointer to 8 bit status code from status register
  @retval 0 SUCCESS
  @retval 1 FAILURE

**/

UINT32
CheckLongOperationStatus (
  SYSHOST   *Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT16    *Command,
  UINT16    *PercentComplete,
  UINT32    *Etc,
  UINT8     *StatusCode,
  UINT8     *MbStatus
  );

/**

  Peform crossover calibration

  @param Host - Pointer to sysHost, the system Host (root) structure struct

  @retval SUCCESS

**/
UINT32
CrossoverCalibFnv (
  PSYSHOST  Host
  );

/**
*Updated 8/19/2014*
*NVMCTLR CIS 0.70*

Routine Description: FnvMailboxInterface is a generic read/write to the BIOS/SMM mailbox. BIOS mailbox will always use CPGC interface
Program Flow:

  @param Host          - Pointer to the system host (root) structure
  @param Socket        - Socket Number
  @param Ch            - DDR Channel ID
  @param Dimm          - DIMM number
  @param InputPayload  - input Payload Register
  @param OutputPayload - Output Payload Register
  @param Opcode        - NVMCTLR command (subopcode | opcode)
@retval 0 SUCCESS
@retval 1 FAILURE
**/
UINT32
FnvMailboxInterface (
  SYSHOST   *Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT32    InputPayload[NUM_PAYLOAD_REG],
  UINT32    OutputPayload[NUM_PAYLOAD_REG],
  UINT32    Opcode,
  UINT8     *MbStatus
  );

/**

Routine Description: Status Decoder will print status message after issuing a NVMCTLR Command


  @param[in] Host           - Pointer to the system Host (root) structure
  @param[in] Socket         - Socket Number
  @param[in] Ch             - DDR Channel ID
  @param[in] Dimm           - DIMM number
  @param[in] MailboxStatus  - 8 bit status code from status register

  @retval 0                 - Success
  @retval 1                 - Failure

**/
UINT32
StatusDecoder (
  IN SYSHOST *Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT32   MailboxStatus,
  IN UINT32   CmdOpcode
  );


/**

  This function returns the register address with propper swizzle.
  DDRT IO swizzle will not change from SKX. Hence DDRT IO
  registers have to use this function

  @param strobe            - Strobe number (0-based)
  @param RegisterGroupBase - Address of the the register group

  @retval IO Address

**/
UINT32
UpdateIoRegisterChDdrt (
  PSYSHOST Host,
  UINT8 strobe,
  UINT32 regOffset
  );

UINT32
GetPowerManagementPolicy (
  IN  SYSHOST                 *Host,
  IN  UINT8                    Socket,
  IN  UINT8                    Ch,
  IN  UINT8                    Dimm,
  IN  UINT16                   FisVersion,
  OUT POWER_MANAGEMENT_POLICY *PowerManagementPolicy,
  OUT UINT8                   *MailboxStatus
  );

UINT32
SetPowerManagementPolicy (
  IN  SYSHOST                 *Host,
  IN  UINT8                    Socket,
  IN  UINT8                    Ch,
  IN  UINT8                    Dimm,
  IN  UINT16                   FisVersion,
  IN  POWER_MANAGEMENT_POLICY *PowerManagementPolicy,
  OUT UINT8                   *MailboxStatus
  );

 /**
   Determines if the DDRIO CMD buffers need to be tristated during DDRIO init
   based on DIMM population and stepping

   @param[in]   Host    Pointer to sysHost
   @param[in]   Socket  Socket ID
   @param[in]   Channel Channel ID

   @retval      True    The given channel requires CMD pin tristate during DDRIO init
   @retval      False   The given channel does not require CMD pin tristate during DDRIO init
 **/
BOOLEAN
EFIAPI
ChannelRequiresCmdTristate (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  );

/**

Routine Description: Converged Polling function of the Boot Status Register

  @param[in] Host        - Pointer to the system Host (root) structure
  @param[in] Socket      - Socket Number
  @param[in] Ch          - Channel number
  @param[in] Bitmask     - Bit mask that identifies the Bit field in the Boot Status Register

  @retval 0 - success

**/
UINT32
FnvPollingBootStatusRegisterCh (
  IN  PSYSHOST Host,
  IN  UINT8    socket,
  IN  UINT8    ch,
  IN  UINT32   bitmask
  );

//
// Bit masks for setting/clearing bits in protected registers
//
#define PROT_REG_MASK_GNT_32_CONT_EN  BIT14   // DA_DDRT_FUNC_DEFEATURE1
#define PROT_REG_MASK_WCRD_INIT       BIT21   // D_FUNC_DEFEATURE1
#define PROT_REG_MASK_DAUNIT_PMRESET  BIT12   // MSC_PLL_CTRL
#define PROT_REG_MASK_DDRTIO_PMRESET  BIT13   // MSC_PLL_CTRL
#define PROT_REG_MASK_DDRTIO_CKIDLE   BIT31   // MSC_PLL_CTRL

/**
  Used to read/write registers that Crystal Ridge Firwmare has restricted access to

  @param[in]      Host              Pointer to syshost
  @param[in]      Socket            Current socket index
  @param[in]      Channel           Current channel index
  @param[in]      Dimm              Current DIMM index
  @param[in]      Mode              Indicates a read or a write - GSM_FORCE_WRITE and GSM_READ_ONLY are the only allowed values
  @param[in]      Register          DCPMM DIMM register address to access
  @param[in]      DesiredWriteMask  For write accesses, indicates which bits CRFW should write
                                      A "1" indicates the corresponding register bit will be set or cleared by CRFW according
                                        to the state of the corresponding bit in Value
                                      A "0" indicates the corresponding register bit will not be modified by CRFW
  @param[in, out] Value             Pointer to buffer containing either the value to write or to place read data in

  @retval MRC_STATUS_SUCCESS    All operations completed successfully
  @retval MRC_STATUS_FAILURE    One or more operations could not be completed successfully
**/
MRC_STATUS
EFIAPI
GetSetProtectedDcpmmRegister (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      UINT8       Channel,
  IN      UINT8       Dimm,
  IN      UINT8       Mode,
  IN      UINT32      Register,
  IN      UINT32      DesiredWriteMask,
  IN OUT  UINT32      *Value
  );

#endif  // #ifndef _MEM_FMC_IP_LIB_INTERNAL_H_
