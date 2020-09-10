/** @file
  Interface header file for the IoAccess library class.

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

#ifndef _KTI_SIM_LIB_H_
#define _KTI_SIM_LIB_H_

#include <Library/MemoryCoreLib.h>
#include <Upi/KtiHost.h>

#define  MAX_LINE_SIZE        256
#define  MAX_TOKEN_SIZE       16
#define  DATA_FILE_COMMENT    '#'
#define  DATA_FILE_DELIMITOR  ":"

#define  READ_ACCESS              0
#define  WRITE_ACCESS             1

#define  MAX_CPU_KTI_REGISTERS    5000
#define  REG_NAME_STRSIZE         50

#define  FIRST_REG_OFFSET           0

#define  REG_ATTR_STICKY          1
#define  REG_ATTR_SHADOW          2
#define  REG_DO_NOT_VERIFY        4

#define SIM_ASSERT(assertion) \
  if (!(assertion)) { \
    printf ("\n %s: %u   %s \n", __FILE__, __LINE__, #assertion); \
    { __asm int 0x3 } \
  }

#define IncRegIndex \
  {                 \
    RegIndex++;     \
    SIM_ASSERT(RegIndex < MAX_CPU_KTI_REGISTERS); \
  }

#define IncRegIndexPtr \
  {                    \
    (*RegIndex)++;     \
    SIM_ASSERT(*RegIndex < MAX_CPU_KTI_REGISTERS); \
  }

typedef enum  {
  TYPE_NONE_HANDLER = 0,
  TYPE_KTI_MAILBOX_DATA_HANDLER,
  TYPE_KTI_MAILBOX_INTERFACE_HANDLER,
  TYPE_KTI_UPDATE_PHCTR_CURR_HANDLER,
  TYPE_KTI_IAR_BOX_HANDLER,
  TYPE_KTI_PI_CAL_CODE_HANDLER,
  TYPE_KTI_MMCFG_DATA_HANDLER,
  TYPE_KTI_LBC_PER_IOU_HANDLER,
  TYPE_KTI_LBC_HANDER,
  TYPE_KTI_PH_CTR1_HANDER,
  TYPE_KTI_PH_PIS_HANDLER,
  TYPE_KTI_LCL_HANDLER,
  TYPE_KTI_RCRB_BAR_REG_HANDLER,
  TYPE_KTI_UNALIGNED_ACCESS_HANDLER,
  TYPE_KTI_CXL_SUBSYS_W1C_REG_ACCESS_HANDLER,
  TYPE_KTI_CXL_SUBSYS_GENERAL_REG_ACCESS_HANDLER,
  TYPE_KTI_BIOS_RESET_CPL_HANDLER,
  TYPE_KTI_PH_CIS_HANDLER,
  TYPE_KTI_PMON_BAR_HANDLER,
  TYPE_KTI_SPK_BAR_HANDLER,
  TYPE_KTI_PH_PIPE_CLK_CTRL_HANDLER
} TYPE_REG_HANDLER;

typedef struct _KTI_SIM_REGISTER {
  UINT8       RegName[REG_NAME_STRSIZE];  // Register name
  UINT32      RegAddr;                    // Register Offset. 0 indicates register is not created yet.
  UINT32      RegDefault;                 // Holds the 32 bit default value for the register to be applied during every reset
  UINT32      RegData;                    // Holds the 32 bit value for the register
  UINT32      ShadowReg;                  // Address of the register being shadowed by this register; applicable only when Attr is set to Shadow.
  UINT8       Attr;
  TYPE_REG_HANDLER RegHandlerType;        // Register HandlerType
  VOID        *RegHandler;                // Handler if this register needs special handling while R/W
} KTI_SIM_REGISTER;


#if defined(GNRSRF_HOST)
// For GNRSRF, because SCF BAR is over 1M space
#define UBOX_MMIO_SCF_BAR      0x00c0000  // this is used for KTI RCSIM MS2IDI (0.24.0.)only. Don't copy to actual syshost.h
#define UBOX_MMIO_SBREG_BAR    0x00d0000  // this is used for KTI RCSIM SBBAR (0.26.0.)only. Don't copy to actual syshost.h
#define SIM_MC_MMIO_BAR        0x00e0000  // this is used for KTI RCSIM MCBAR (0.28.0.)only. Don't copy to actual syshost.h
#define UBOX_MMIO_MDFIS_BAR    0x00f0000  // this is used for KTI RCSIM MDFIS (0.30.0.)only. Don't copy to actual syshost.h

#else
//
// FOR 10NM & SPR
//
#define UBOX_MMIO_SCF_BAR      0x00c9000  // this is used for KTI RCSIM MS2IDI (0.25.1.)only. Don't copy to actual syshost.h
#define UBOX_MMIO_SBREG_BAR    0x00d1000  // this is used for KTI RCSIM SBBAR (0.26.1.)only. Don't copy to actual syshost.h
#define SIM_MC_MMIO_BAR        0x00d9000  // this is used for KTI RCSIM MCBAR (0.27.1.)only. Don't copy to actual syshost.h
#define UBOX_MMIO_MDFIS_BAR    0x00e1000  // this is used for KTI RCSIM MDFIS (only. Don't copy to actual syshost.h
#endif

//
// This is used for KTI RCSIM SBBAR (PCIe Gen5)only. Don't copy to actual syshost.h
// Note: It need to reserve 192KB(MAX_SOCKET * MAX_CXL_PER_SOCKET * 4KB) buffer, and each stack will occupy 4KB.
//
#define UBOX_MMIO_SBREG_G5_BAR 0x00e9000
#define UBOX_MMIO_SBREG_G5_SIZE_PER_STACK  0x1000
#define UBOX_MMIO_SBREG_G5_SIZE            (UBOX_MMIO_SBREG_G5_SIZE_PER_STACK * MAX_SOCKET * MAX_CXL_PER_SOCKET)  //0x30000

#define PMON_MEM_SIZE          0x1000
#define SPK_MEM_SIZE           0x80000

//
// DMI/CXL RCRB Mem Bar must be 8KB aligned, and each port(DMI or CXL) will occupy 8KB.
// Note: It need to reserve (n * MAX_CXL_PER_SOCKET * 8KB) buffer, in which n is Socket number.
//       Currently, it is restricted to Socket[0] only(n=1).
//
#define SIM_RCRB_MEM_BAR       ((UBOX_MMIO_SBREG_G5_BAR + UBOX_MMIO_SBREG_G5_SIZE + CXL_RCRB_BAR_SIZE - 1) & (~(CXL_RCRB_BAR_SIZE - 1)))  //0x11A000
#define SIM_RCRB_MEM_SIZE      (CXL_RCRB_BAR_SIZE * MAX_CXL_PER_SOCKET)  //0xC000

//
// DMI/CXL MEMBAR0 base should be 128KB aligned, and each port(DMI port or CXL DP/UP port) will occupy 128KB.
// Note: It need to reserve (n * MAX_CXL_PER_SOCKET * 128KB * 2) buffer, in which n is Socket number.
//       Currently, it is restricted to Socket[0] only(n=1).
//
#define SIM_MEMBAR0_RES_BASE   ((SIM_RCRB_MEM_BAR + SIM_RCRB_MEM_SIZE + CXL_MEMBAR0_SIZE_PER_PORT - 1) & (~(CXL_MEMBAR0_SIZE_PER_PORT - 1)))  //0x140000
#define SIM_MEMBAR0_RES_SIZE   (CXL_MEMBAR0_SIZE_PER_PORT * 2 * MAX_CXL_PER_SOCKET)  //0x180000

//
// CXL Port bitmap
// Each bit indicates if that stack is worked as CXL port(1b = CXL Port, 0b = Not CXL Port).
// Bit0: Socket[0]Stack[0] - reserved for DMI use, so Bit[0] will be ignored
// Bit1: Socket[0]Stack[1]
// ...,
// Bit5: Socket[0]Stack[5]
// Bit6: Socket[1]Stack[0]
// ...,
// Bit11:Socket[1]Stack[5]
// ...
// Bit47:Socket[7]Stack[5]
// Bit[63:48]: Reserved.
// Note:
// (1) To reduce the virtual memory consumption, it is restricted to simulate CXL port only in the stacks of Socket[0].
//     That means Bit[63:6] will be ignored as well.
// (2) Currently, Socket[0]Stack[1] is enabled as CXL port by default.
//     If want to enable more stacks in Socket[0] as CXL ports, please remember to edit the table
//     CxlEpDevInfoTbl[MAX_CXL_PER_SOCKET] to provide valid VendorID, DVSEC.Cap, SerialNum.
//     Otherwise, the CXL port might be disabled in the end.
//
#define CXL_PORT_BITMAP_SIM    0x0000000000000002

/**

  Simulate memory detection

  @param       None

  @retval      None

**/
VOID
SimulateDetectMemoryConfig (
  VOID
  );

/**

  Interface for register read/write for simulation environment

  @param[in]   Cpu                - CPU Socket Node number (Socket ID)
  @param[in]   Reg                - Register address
  @param[in]   RegSize            - Register size (1,2,4)
  @param[in]   Data32             - Data to be written for WRITE
  @param[in]   Accesstype         - READ_ACCESS or WRITE_ACCESS

  @retval      Data read for READ_ACCESS

**/
UINT32
KtiSimRegAccess (
  IN UINT8        Cpu,
  IN UINT32       Reg,
  IN UINT8        RegSize,
  IN UINT32       Data32,
  IN UINT8        Accesstype
  );

/**

  Search the sim for a register with particular register offset.

  @param[in] Reg - Register offset

  @retval    Pointer to register - if found
  @retval    NULL - if register not found

**/
KTI_SIM_REGISTER *
SearchRegOffset (
  IN UINT32       Reg
  );

/**

  Update registers to simulate warm reset.

  @param    Host - Pointer to the system Host (root) structure

  @retval   KTI_SUCCESS - on successful completion

**/
KTI_STATUS
PrimeRegsForReset (
  IN PSYSHOST Host
  );

/**

  Sim handler for pCode mailbox accesses, data portion.

  @param[in] Host       - Pointer to the system Host root structure
  @param[in] Cpu        - SocketID of the CPU being accessed
  @param[in] Reg        - Register offset
  @param[in] SimReg     - Register offset in KTI_SIM_REGISTER format
  @param[in] Data32     - Data to write in case of a WRITE_ACCESS
  @param[in] Accesstype - READ_ACCESS or WRITE_ACCESS

  @retval    Data read from register

**/
UINT32
KtiMailBoxDataHandler (
  IN PSYSHOST             Host,
  IN UINT8                Cpu,
  IN UINT32               Reg,
  IN KTI_SIM_REGISTER     *SimReg,
  IN UINT32               Data32,
  IN UINT8                Accesstype
  );

/**

  Sim interface for pCode mailbox commands interface.

  @param[in] Host       - Pointer to the system Host root structure
  @param[in] Cpu        - SocketID of the CPU being accessed
  @param[in] Reg        - Register offset
  @param[in] SimReg     - Register offset in KTI_SIM_REGISTER format
  @param[in] Data32     - Data to write in case of a WRITE_ACCESS
  @param[in] Accesstype - READ_ACCESS or WRITE_ACCESS

  @retval    Data read

**/
UINT32
KtiMailBoxInterfaceHandler (
  IN PSYSHOST             Host,
  IN UINT8                Cpu,
  IN UINT32               Reg,
  IN KTI_SIM_REGISTER     *SimReg,
  IN UINT32               Data32,
  IN UINT8                Accesstype
  );

/**

  Update the KTIPHCTR_CURR register

  @param[in] Host       - Pointer to the system Host root structure
  @param[in] Cpu        - SocketID of the CPU being accessed
  @param[in] Reg        - Register offset
  @param[in] SimReg     - Register offset in KTI_SIM_REGISTER format
  @param[in] Data32     - Data to write in case of a WRITE_ACCESS
  @param[in] Accesstype - READ_ACCESS or WRITE_ACCESS

  @retval    SimReg->RegData - RegData part of the SimReg

**/
UINT32
KtiUpdatePhctrCurrHandler (
  IN PSYSHOST             Host,
  IN UINT8                Cpu,
  IN UINT32               Reg,
  IN KTI_SIM_REGISTER     *SimReg,
  IN UINT32               Data32,
  IN UINT8                Accesstype
  );

/**

  Sim handler for IAR accesses.

  @param[in] Host       - Pointer to the system Host root structure
  @param[in] Cpu        - SocketID of the CPU being accessed
  @param[in] Reg        - Register offset
  @param[in] SimReg     - Register offset in KTI_SIM_REGISTER format
  @param[in] Data32     - Data to write in case of a WRITE_ACCESS
  @param[in] Accesstype - READ_ACCESS or WRITE_ACCESS

  @retval    Currently always returns 0

**/
UINT32
KtiIarBoxHandler (
  IN PSYSHOST             Host,
  IN UINT8                Cpu,
  IN UINT32               Reg,
  IN KTI_SIM_REGISTER     *SimReg,
  IN UINT32               Data32,
  IN UINT8                Accesstype
  );

/**

  Mmcfg Data handler

  @param[in] Host       - Pointer to the system Host root structure
  @param[in] Cpu        - SocketID of the CPU being accessed
  @param[in] Reg        - Register offset
  @param[in] SimReg     - Register offset in KTI_SIM_REGISTER format
  @param[in] Data32     - Data to write in case of a WRITE_ACCESS
  @param[in] Accesstype - READ_ACCESS or WRITE_ACCESS

  @retval    BUSCFGSYNC_PBSP_DONE if WRITE_ACCESS with Data32 being BUSCFGSYNC_PBSP_GO

**/
UINT32
KtiMmcfgDataHandler (
  IN PSYSHOST             Host,
  IN UINT8                Cpu,
  IN UINT32               Reg,
  IN KTI_SIM_REGISTER     *SimReg,
  IN UINT32               Data32,
  IN UINT8                Accesstype
  );

/**

  KtPiCal code handler; currently not used.

  @param[in] Host       - Pointer to the system Host root structure
  @param[in] Cpu        - SocketID of the CPU being accessed
  @param[in] Reg        - Register offset
  @param[in] SimReg     - Register offset in KTI_SIM_REGISTER format
  @param[in] Data32     - Data to write in case of a WRITE_ACCESS
  @param[in] Accesstype - READ_ACCESS or WRITE_ACCESS

  @retval    Returns unused RegData portion of SimReg

**/
UINT32
KtPiCalCodeHandler (
  IN PSYSHOST             Host,
  IN UINT8                Cpu,
  IN UINT32               Reg,
  IN KTI_SIM_REGISTER     *SimReg,
  IN UINT32               Data32,
  IN UINT8                Accesstype
  );

/**

  Update the KTIREUT_PH_PIS register

  @param[in] host       - Pointer to the system host root structure
  @param[in] Cpu        - SocketID of the CPU being accessed
  @param[in] Reg        - Register offset
  @param[in] SimReg     - Register offset in KTI_SIM_REGISTER format
  @param[in] Data32     - Data to write in case of a WRITE_ACCESS
  @param[in] Accesstype - READ_ACCESS or WRITE_ACCESS

  @retval    SimReg->RegData - RegData part of the SimReg

**/
UINT32
KtiPhPisHandler (
  IN PSYSHOST             Host,
  IN UINT8                Cpu,
  IN UINT32               Reg,
  IN KTI_SIM_REGISTER     *SimReg,
  IN UINT32               Data32,
  IN UINT8                Accesstype
  );

/**

  Update the KTIREUT_PH_CTR1 register

  @param[in] Host       - Pointer to the system Host root structure
  @param[in] Cpu        - SocketID of the CPU being accessed
  @param[in] Reg        - Register offset
  @param[in] SimReg     - Register offset in KTI_SIM_REGISTER format
  @param[in] Data32     - Data to write in case of a WRITE_ACCESS
  @param[in] Accesstype - READ_ACCESS or WRITE_ACCESS

  @retval    SimReg->RegData - RegData part of the SimReg

**/
UINT32
KtiPhCtr1Handler (
  IN PSYSHOST             Host,
  IN UINT8                Cpu,
  IN UINT32               Reg,
  IN KTI_SIM_REGISTER     *SimReg,
  IN UINT32               Data32,
  IN UINT8                Accesstype
  );

/**

    Update the PIPE_CLK_RATE_CTRL_KTI_REUT_REG register

    @param Host       - Pointer to the system Host root structure
    @param Cpu        - SocketID of the CPU being accessed
    @param Reg        - Register offset
    @param SimReg     - Register offset in KTI_SIM_REGISTER format
    @param Data32     - Data to write in case of a WRITE_ACCESS
    @param Accesstype - READ_ACCESS or WRITE_ACCESS

    @retval SimReg->RegData - RegData part of the SimReg

**/
UINT32
KtiPhPipeClkCtrlHandler (
  SYSHOST              *Host,
  UINT8                Cpu,
  UINT32               Reg,
  KTI_SIM_REGISTER     *SimReg,
  UINT32               Data32,
  UINT8                Accesstype
  );

/**

  Update the KTIREUT_PH_CIS register

  @param[in] host       - pointer to the system host root structure
  @param[in] Cpu        - SocketID of the CPU being accessed
  @param[in] Reg        - register offset
  @param[in] SimReg     - register offset in KTI_SIM_REGISTER format
  @param[in] Data32     - data to write in case of a WRITE_ACCESS
  @param[in] Accesstype - READ_ACCESS or WRITE_ACCESS

  @retval SimReg->RegData - RegData part of the SimReg

**/
UINT32
KtiPhCisHandler (
  IN PSYSHOST             Host,
  IN UINT8                Cpu,
  IN UINT32               Reg,
  IN KTI_SIM_REGISTER     *SimReg,
  IN UINT32               Data32,
  IN UINT8                Accesstype
  );

/**

  Update the KTILCL_KTI_LLPMON_REG register

  @param[in] Host       - Pointer to the system Host root structure
  @param[in] Cpu        - SocketID of the CPU being accessed
  @param[in] Reg        - Register offset
  @param[in] SimReg     - Register offset in KTI_SIM_REGISTER format
  @param[in] Data32     - Data to write in case of a WRITE_ACCESS
  @param[in] Accesstype - READ_ACCESS or WRITE_ACCESS

  @retval    SimReg->RegData - RegData part of the SimReg

**/
UINT32
KtiLclHandler (
  IN PSYSHOST             Host,
  IN UINT8                Cpu,
  IN UINT32               Reg,
  IN KTI_SIM_REGISTER     *SimReg,
  IN UINT32               Data32,
  IN UINT8                Accesstype
  );

/**

  Sim handler for Lbc accesses.

  @param[in] Host       - Pointer to the system Host root structure
  @param[in] Cpu        - SocketID of the CPU being accessed
  @param[in] Reg        - Register offset
  @param[in] SimReg     - Register offset in KTI_SIM_REGISTER format
  @param[in] Data32     - Data to write in case of a WRITE_ACCESS
  @param[in] Accesstype - READ_ACCESS or WRITE_ACCESS

  @retval    Currently always returns 0

**/
UINT32
KtiLbcHandler (
  IN PSYSHOST             Host,
  IN UINT8                Cpu,
  IN UINT32               Reg,
  IN KTI_SIM_REGISTER     *SimReg,
  IN UINT32               Data32,
  IN UINT8                Accesstype
  );

/**

  Sim handler for LbcPerIou accesses.

  @param[in] Host       - Pointer to the system Host root structure
  @param[in] Cpu        - SocketID of the CPU being accessed
  @param[in] Reg        - Register offset
  @param[in] SimReg     - Register offset in KTI_SIM_REGISTER format
  @param[in] Data32     - Data to write in case of a WRITE_ACCESS
  @param[in] Accesstype - READ_ACCESS or WRITE_ACCESS

  @retval    Currently always returns 0

**/
UINT32
KtiLbcPerIouHandler (
  IN PSYSHOST             Host,
  IN UINT8                Cpu,
  IN UINT32               Reg,
  IN KTI_SIM_REGISTER     *SimReg,
  IN UINT32               Data32,
  IN UINT8                Accesstype
  );

/**

  Get BDFO format register offset from the box/type format offset.

  @param[in] Host       - Pointer to the system Host root structure
  @param[in] Socket     - SocketID of the CPU being accessed
  @param[in] Ch         - Channel
  @param[in] Dimm       - Dimm


  @retval    None

**/
VOID
ConfigDimm (
  IN PSYSHOST            Host,
  IN UINT8               Socket,
  IN UINT8               Ch,
  IN UINT8               Dimm
  );

/**

  Get BDFO format register offset from the box/type format offset.

  @param[in]  Host    - Pointer to the system Host root structure
  @param[out] Offset  - Box/type format offset

  @retval     BDFO format offset

**/
UINT32
GetRegisterOffsetSim (
  IN PSYSHOST Host,
  OUT UINT32 Offset
  );

/**

  Handle the side effect of c_init_begin and update the Link layer state accordingly
  so that we can simulate the topology reduction scenarios. This will also help to simulate
  setup based KTI link disabling scenarios.

  @param     Cpu        - SocketID of the CPU being accessed
  @param     Port       - Port id of the CPU being accessed

  @retval    None

**/
VOID
KtiHandleCinitUpdateLinkLayer (
  UINT8 Cpu,
  UINT8 Port
  );

/**

  KtiSimRegAccess wrapper for register write for simulation environment.

  @param[in]   SocId              - CPU Socket Node number (Socket ID)
  @param[in]   RegAddr            - Register address
  @param[in]   RegSize            - Register size (1,2,4)
  @param[in]   Data32             - Data to be written

  @retval      VOID

**/
VOID
EFIAPI
UbiosKtiSimRegWrite (
  IN UINT8        SocId,
  IN UINT32       RegAddr,
  IN UINT8        RegSize,
  IN UINT32       Data32
  );

/**

  Support access to MEMBAR0 register in CXL DP/UP RCRB.
  For example: It can support sizing the Bar register per PCI spec.

  @param[in] Cpu        - SocketID of the CPU being accessed
  @param[in] Reg        - Register offset
  @param[in] RegSize    - Register size (1,2,4)
  @param[in] SimReg     - Register offset in KTI_SIM_REGISTER format
  @param[in] Data32     - Data to write in case of a WRITE_ACCESS
  @param[in] Accesstype - READ_ACCESS or WRITE_ACCESS

  @retval    SimReg->RegData - RegData part of the SimReg.

**/
UINT32
KtiRcrbBarRegAccessHandler (
  UINT8                Cpu,
  UINT32               Reg,
  UINT8                RegSize,
  KTI_SIM_REGISTER     *SimReg,
  UINT32               Data32,
  UINT8                Accesstype
  );

/**

  Support unaligned access to registers in PCIe configuration space. Details as below:
  - 1 Byte access at any offset;
  - 2 Byte access at any even offset;
  - 4 Byte access at 4-byte aligned offset.

  @param[in] Cpu        - SocketID of the CPU being accessed
  @param[in] Reg        - Register offset
  @param[in] RegSize    - Register size (1,2,4)
  @param[in] SimReg     - Register offset in KTI_SIM_REGISTER format
  @param[in] Data32     - Data to write in case of a WRITE_ACCESS
  @param[in] Accesstype - READ_ACCESS or WRITE_ACCESS

  @retval    - If WRITE_ACCESS, return SimReg->RegData.
             - If READ_ACCESS, return the desired part of SimReg->RegData.

**/
UINT32
KtiUnalignedRegAccessHandler (
  UINT8                Cpu,
  UINT32               Reg,
  UINT8                RegSize,
  KTI_SIM_REGISTER     *SimReg,
  UINT32               Data32,
  UINT8                Accesstype
  );

/**

  Support access to W1C(Write 1 to clear) registers of the CXL DP/UP Port Subsystem Component.

  @param[in] Cpu        - SocketID of the CPU being accessed
  @param[in] Reg        - Register offset
  @param[in] RegSize    - Register size (1,2,4)
  @param[in] SimReg     - Register offset in KTI_SIM_REGISTER format
  @param[in] Data32     - Data to write in case of a WRITE_ACCESS
  @param[in] Accesstype - READ_ACCESS or WRITE_ACCESS

  @retval    SimReg->RegData - RegData part of the SimReg.

**/
UINT32
KtiCxlSubsysW1cRegAccessHandler (
  UINT8                Cpu,
  UINT32               Reg,
  UINT8                RegSize,
  KTI_SIM_REGISTER     *SimReg,
  UINT32               Data32,
  UINT8                Accesstype
  );

/**

  Support access to general registers of the CXL DP/UP Port Subsystem Component.

  @param[in] Cpu        - SocketID of the CPU being accessed
  @param[in] Reg        - Register offset
  @param[in] RegSize    - Register size (1,2,4)
  @param[in] SimReg     - Register offset in KTI_SIM_REGISTER format
  @param[in] Data32     - Data to write in case of a WRITE_ACCESS
  @param[in] Accesstype - READ_ACCESS or WRITE_ACCESS

  @retval    SimReg->RegData - RegData part of the SimReg.

**/
UINT32
KtiCxlSubsysGeneralRegAccessHandler (
  UINT8                Cpu,
  UINT32               Reg,
  UINT8                RegSize,
  KTI_SIM_REGISTER     *SimReg,
  UINT32               Data32,
  UINT8                Accesstype
  );

  /**

  Support access to general registers of the BIOS_RESET_CPL register.

  @param[in] Cpu        - SocketID of the CPU being accessed
  @param[in] Reg        - Register offset
  @param[in] RegSize    - Register size (1,2,4)
  @param[in] SimReg     - Register offset in KTI_SIM_REGISTER format
  @param[in] Data32     - Data to write in case of a WRITE_ACCESS
  @param[in] Accesstype - READ_ACCESS or WRITE_ACCESS

  @retval    SimReg->RegData - RegData part of the SimReg.

**/
UINT32
KtiBiosResetCplHandler (
  IN PSYSHOST             Host,
  IN UINT8                Cpu,
  IN UINT32               Reg,
  IN KTI_SIM_REGISTER     *SimReg,
  IN UINT32               Data32,
  IN UINT8                Accesstype
  );


/**
  KtiSimHandleTxeqAdaptationFlow: Simulate the TXEQ Adaptation flow
    @param Cpu        - SocketID
    @param Link       - Link Id

  @retval:
**/
VOID
EFIAPI
KtiSimHandleTxeqAdaptationFlow (
  IN UINT8               Cpu,
  IN UINT8               Link
  );

/**

    Sim handler for Pmon Bar accesses

    @param Host       - Pointer to the system Host root structure
    @param Cpu        - SocketID of the CPU being accessed
    @param Reg        - Register offset
    @param SimReg     - Register offset in KTI_SIM_REGISTER format
    @param Data32     - Data to write in case of a WRITE_ACCESS
    @param Accesstype - READ_ACCESS or WRITE_ACCESS

    @retval Data read from register

**/
UINT32
KtiPmonBarHandler (
  SYSHOST              *Host,
  UINT8                Cpu,
  UINT32               Reg,
  KTI_SIM_REGISTER     *SimReg,
  UINT32               Data32,
  UINT8                Accesstype
  );

/**

    Sim handler for Spk Bar accesses

    @param Host       - Pointer to the system Host root structure
    @param Cpu        - SocketID of the CPU being accessed
    @param Reg        - Register offset
    @param SimReg     - Register offset in KTI_SIM_REGISTER format
    @param Data32     - Data to write in case of a WRITE_ACCESS
    @param Accesstype - READ_ACCESS or WRITE_ACCESS

    @retval Data read from register

**/
UINT32
KtiSpkBarHandler (
  SYSHOST              *Host,
  UINT8                Cpu,
  UINT32               Reg,
  KTI_SIM_REGISTER     *SimReg,
  UINT32               Data32,
  UINT8                Accesstype
  );


#endif // #ifndef _KTI_SIM_LIB_H_
