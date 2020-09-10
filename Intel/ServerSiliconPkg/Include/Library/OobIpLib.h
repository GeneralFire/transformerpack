/** @file

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
#ifndef _OOB_LIB_H_
#define _OOB_LIB_H_

#include <Uefi.h>
#include <Base.h>
#include <ReferenceCodeDataTypes.h>
#include <Library/UsraCsrLib.h>

#define BUS_NUM_OOBMSMBUS     0xFB

//
// PMON Block ID definition
//

//
// The first PMON always starts at this base offset
//
#define PMON_BASE_OFFSET           0x20

//
// The stride in bytes used to reach the next pmon
//
#define PMON_BLOCK_STRIDE_OFFSET   0x20

//
// The stride in number of 32 bit registers used to reach the next pmon
//
#define PMON_BLOCK_STRIDE          4

#if defined(SNR_HOST) || defined(TNR_HOST) || defined(ICXDE_HOST)

//
//SNR PMON definitions
//

#define MAX_STATE_CHA_XCC     6
#define MAX_STATE_CHA_HCC     6
#define MAX_STATE_CHA_LCC     6
#define MAX_STATE_STACK       5
#define MAX_STATE_STACK_LCC   MAX_STATE_STACK
#define MAX_STATE_TC          MAX_STATE_STACK//stack + ubox
#define MAX_STATE_IRP         MAX_STATE_STACK//stack + ubox
#define MAX_STATE_M2PCIE      MAX_STATE_STACK//stack + ubox
#define MAX_STATE_PCU         1
#define MAX_STATE_UBOX        1
#define MAX_STATE_MC          3
#define MAX_STATE_M2M         1
#define MAX_STATE_UPI         0
#define MAX_STATE_UPI_LL      MAX_STATE_UPI
#define MAX_STATE_M3UPI       MAX_STATE_UPI
#define MAX_STATE_PCIE        1

#define PMON_CHA_STRIDE             0x10
#define PMON_TC_STRIDE              0x10
#define PMON_IRP_STRIDE             0x10
#define PMON_M2PCIE_STRIDE          0x10
#define PMON_TC_STRIDE_UPPER(x)      0x0
#define PMON_IRP_STRIDE_UPPER(x)     0x0
#define PMON_M2PCIE_STRIDE_UPPER(x)  0x0

#define PMON_MAX_BLOCKS            48
#define PMON_NUM_BLOCK_STS_BITS    30
#define PMON_GLBL_CTR_ADDR         0x1F80
#define PMON_GLBL_STAT_CTR_ADDR    0xE

#endif

#if defined(ICX_HOST)

//
//ICX-SP PMON definitions
//

#define MAX_STATE_CHA_XCC      40
#define MAX_STATE_CHA_HCC      28
#define MAX_STATE_CHA_LCC      16
#define MAX_STATE_STACK        6
#define MAX_STATE_STACK_LCC   (MAX_STATE_STACK - 1)
#define MAX_STATE_TC           MAX_STATE_STACK//stack + ubox
#define MAX_STATE_IRP          MAX_STATE_STACK//stack + ubox
#define MAX_STATE_M2PCIE       MAX_STATE_STACK//stack + ubox
#define MAX_STATE_PCU          1
#define MAX_STATE_UBOX         1
#define MAX_STATE_MC           12
#define MAX_STATE_M2M          4
#define MAX_STATE_UPI          3
#define MAX_STATE_UPI_LL       MAX_STATE_UPI
#define MAX_STATE_M3UPI        MAX_STATE_UPI
#define MAX_STATE_PCIE         2

#define PMON_CHA_STRIDE             0x0E
#define PMON_TC_STRIDE              0x20
#define PMON_IRP_STRIDE             0x20
#define PMON_M2PCIE_STRIDE          0x20
#define PMON_TC_STRIDE_UPPER(x)     ((x < 3)? 0x0 : 0x30)
#define PMON_IRP_STRIDE_UPPER(x)    ((x < 3)? 0x0 : 0x30)
#define PMON_M2PCIE_STRIDE_UPPER(x) ((x < 3)? 0x0 : 0x30)

#define PMON_MAX_BLOCKS            96
#define PMON_NUM_BLOCK_STS_BITS    92
#define PMON_GLBL_CTR_ADDR         0x700
#define PMON_GLBL_STAT_CTR_ADDR    0xE

#endif

#if defined(SPR_HOST)

//
//SPR-SP PMON definitions
//

#define MAX_STATE_CHA_XCC     60
#define MAX_STATE_CHA_HCC     60
#define MAX_STATE_CHA_MCC     60
#define MAX_STATE_CHA_LCC     60
#define MAX_STATE_STACK       12
#define MAX_STATE_STACK_LCC   MAX_STATE_STACK
#define MAX_STATE_TC          MAX_STATE_STACK//stack + ubox
#define MAX_STATE_IRP         MAX_STATE_STACK//stack + ubox
#define MAX_STATE_M2PCIE      MAX_STATE_STACK//stack + ubox
#define MAX_STATE_PCU         1
#define MAX_STATE_UBOX        1
#define MAX_STATE_MC          8
#define MAX_STATE_M2M         4
#define MAX_STATE_UPI         4
#define MAX_STATE_UPI_LL      MAX_STATE_UPI
#define MAX_STATE_M3UPI       MAX_STATE_UPI
#define MAX_STATE_PCIE        0
#define MAX_STATE_MDF_H       24
#define MAX_STATE_MDF_V       16
#define MAX_STATE_IAL         6
#define MAX_STATE_IAL_0       MAX_STATE_IAL
#define MAX_STATE_IAL_1       MAX_STATE_IAL

#define PMON_CHA_STRIDE             0x10
#define PMON_TC_STRIDE              0x10
#define PMON_IRP_STRIDE             0x10
#define PMON_M2PCIE_STRIDE          0x10
#define PMON_TC_STRIDE_UPPER(x)     ((x < 3)? 0x0 : 0x30)
#define PMON_IRP_STRIDE_UPPER(x)    ((x < 3)? 0x0 : 0x30)
#define PMON_M2PCIE_STRIDE_UPPER(x) ((x < 3)? 0x0 : 0x30)

#define PMON_MAX_BLOCKS            177
#define PMON_NUM_BLOCK_STS_BITS    92
#define PMON_GLBL_CTR_ADDR         0x2FF0
#define PMON_GLBL_STAT_CTR_ADDR    0xE

#endif

#define PMON_BLOCK_ID_INV        0xFF
#define PMON_SINGLE_UNIT_STRIDE  0x1
#define PMON_DOUBLE_UNIT_STRIDE  0x2
#define PMON_PER_UPI_LINK_STRIDE 0x2
#define PMON_PER_STACK_STRIDE    0x3

#define UNIT_DISC_0_LOW(CounterOffset, CtrlWidth, Ctrl0Offset, CtrlReg) \
  (UINT32)( \
    ((UINTN) CounterOffset) << 24 | \
    ((UINTN) CtrlWidth) << 16 | \
    ((UINTN) Ctrl0Offset) << 8 | \
    (CtrlReg))

#define UNIT_DISC_0_HIGH(AccessType, UnitStatus) \
    (UINT32)( \
    ((UINTN) AccessType) << 30 | \
    (UnitStatus))

#define PMON_MAX_UNIT_ID_CHA 16
#define PMON_MAX_UNIT_ID_CHA_SPR 18

//
// BIOS decides the order and unit type values
//
typedef enum {
  UNIT_TYPE_CHA = 0,
  UNIT_TYPE_TC,
  UNIT_TYPE_IRP,
  UNIT_TYPE_M2PCIE,
  UNIT_TYPE_PCU,
  UNIT_TYPE_UBOX,
  UNIT_TYPE_MC,
  UNIT_TYPE_M2M,
  UNIT_TYPE_UPI_LL,
  UNIT_TYPE_M3UPI,
  UNIT_TYPE_PCIE,
#if defined(SPR_HOST)
  UNIT_TYPE_MDF_H,
  UNIT_TYPE_MDF_V,
  UNIT_TYPE_IAL_0,
  UNIT_TYPE_IAL_1,
#endif
  UNIT_TYPE_MAX // Invalid
} UNIT_TYPE;

typedef enum {
  PMON_ACCESS_MSR = 0,
  PMON_ACCESS_MMIO,
  PMON_ACCESS_CFG,
  PMON_ACCESS_MAX
} PMON_ACCESS_TYPE;

typedef enum {
  PMON_DISC_REG_0 = 0,
  PMON_DISC_REG_1,
  PMON_DISC_REG_2,
  PMON_BLOCK_ID_REG,
  PMON_REG_MAX
} PMON_DISC_REGS;

typedef struct {
  UINT32    BaseOffset        : 16; // [0:15]  Offset of the first pmon discovery unit
  UINT32    Stride            : 8;  // [16:23] The stride used to move to the next pmon discovery unit
  UINT32    MaxSupportedUnits : 8;  // [24:31] Total number of units supported
} PMON_DISC_INFO;

typedef struct {
  UINT32    UnitType;
  UINT32    UnitDiscovery0Low;
  UINT32    UnitDiscovery0High;
  UINT32    BaseOffset;
  UINT32    GlobalStatPos;
  UINT32    BlockId;
} UNIT_DISCOVERY_STRUCT;

typedef struct {
  UINT64    OobAddress;       ///< Base address of NvDimm OOB mailbox.
  UINT16    InterleaveWays;   ///< Number of interleave ways.
  UINT8     InterleaveSize;   ///< Number of contiguous bytes assigned to each CR Dimm.
  UINT32    DimmPresence;     ///< 32 bit Bitmap indicating which Dimms in the interleave set are present.
} OOB_NVDIMM_MAILBOX;

#pragma pack(1)
//
// PMon related definition
//
typedef union {
  struct {
    UINT32 Type : 8;                // [7:0] Type
    UINT32 BlockStride : 8;         // [15:8] Block Stride
    UINT32 MaxBlocks : 10;          // [25:16] Max Blocks
    UINT32 Reserved0 : 6;           // [31:26] Reserved
    UINT32 Reserved1 : 30;          // [61:32] Reserved
    UINT32 AccessType : 2;          // [63:62] Access Type

    UINT64 GlobalControlAddr;       // Global Control Address, serves as Base Addr

    UINT32 GblCtrStatAddr : 8;      // [7:0]
    UINT32 NumBlockStatusBits : 16; // [23:8]
    UINT32 Reserved2 : 8;           // [31:24] Reserved
    UINT32 Reserved3;               // Reserved

    UINT64 Reserved4;               // Reserved
  } Fields;
  UINT64_STRUCT  RawData64[4];
} GLOBAL_DISCOVERY_STATE;

//
// Refer to M2MEM_PMON_UNIT_DISCOVERY0_N0_M2MEM_MAIN_STRUCT
//
typedef union {
  struct {
    UINT32 ControlRegs : 8;             // [7:0] # Control Regs
    UINT32 CtrlAddr : 8;                // [15:8] Ctrl Ctrl 0 Addr (Offset)
    UINT32 CtrlWidth : 8;               // [23:16] Ctrl Width
    UINT32 Counter0Addr : 8;            // [31:24] Counter 0 Addr (Offset)
    UINT32 UnitStatusAddr : 8;          // [39:32] Unit Status Addr (Offset)
    UINT32 Reserved1 : 22;              // [61:40] Reserved
    UINT32 AccessType : 2;              // [63:62] Access Type

    UINT64 UnitCounterControlAddr;      // Unit Counter Control Address, serves as Base Addr

    UINT32 UnitType : 16;               // [15:0] Unit Type
    UINT32 UnitID : 16;                 // [31:16] Unit ID
    UINT32 GlobalStatusPosition : 16;   // [47:32]
    UINT32 Reserved2 : 16;              // [63:48]

    UINT64 BlockId;                   // BlockId
  } Fields;
  UINT64_STRUCT  RawData64[4];
} UNIT_DISCOVERY_STATE;

#pragma pack()

/**
  OOB:  Get OOB bus number

  @param BusData0      - Store Bus Data 0
  @param BusData1      - Store Bus Data 1

  @retval                - None

**/
VOID
EFIAPI
OobGetRootBusNo (
  UINT8  SocId,
  UINT32 *BusData0,
  UINT32 *BusData1
  );

/**
  OOB: Enable PECI

  @param[in] IioIndex - IIO instance (typically socket)
**/
VOID
EFIAPI
OobSetPeciTrusted (
  UINT8 IioIndex
  );

/**
  OOB: Inform OOB-MSM that PCI has been configured.

  @param[in] IioIndex - IIO instance (typically socket)
**/
VOID
EFIAPI
OobSetPciCfgCmpl (
  UINT8 IioIndex
  );

/**
  OOB: Enable PECI Downstream

  This function sets PECI Downstream access enabled for WrPCIConfig, on PECI
  and other agents.

  @param IioIndex      - IIO Index (Socket)

  @retval              - None

**/
VOID
EFIAPI
OobSetPeciDownstreamEnabled (
  UINT8 IioIndex,
  BOOLEAN enabled
  );

/**
  OOB: Set EID value

  @param SocId        - Socket ID
  @param MctpEid      - MctpEid value

  @retval             - None

**/
VOID
EFIAPI
OobSetEid (
  UINT8 SocId,
  UINT8 MctpEid
  );

/**
  OOB: Configure Bus Number root

  @param SocId        - Socket ID
  @param RootBus[5]   - Bus Base values
  @param BusBase      - Bus Base value
  @param BusLimit     - Bus Limit value

  @retval             - None

**/
VOID
EFIAPI
OobSetBusNoRoot (
  UINT8 SocId,
  UINT8 BusBase,
  UINT8 BusLimit,
  UINT8 RootBus[6]
  );

/**
  This function configures Bus Number root for Virtual RootPort (RootbusSn+1)
  and sets this bus as configured

  @param IioIndex     - IIO index (socket number)
  @param Bus          - Bus Number
  @param Device       - Device Number

**/
VOID
OobSetBusNoRootOfVrp (
  IN UINT8                 IioIndex,
  IN UINT8                 Bus,
  IN UINT8                 Device
  );

/**
  OOB: Enable Cpu Bus

  @param SocId             - Socket ID
  @param CpuBusEnMask      - CPU bus enable mask

  @retval                  - None

**/
VOID
EFIAPI
OobSetCpuBusEnable (
  UINT8 SocId,
  UINT32 CpuBusEnMask
  );

/**
  OOB: Set Socket range

  @param SocId        - Socket ID
  @param BusBase      - Bus Base value
  @param BusLimit     - Bus Limit value
  @param PciSegment   - Pci Segment value

  @retval             - None

**/
VOID
EFIAPI
OobSetSocketBusRange (
  UINT8 SocId,
  UINT8 BusBase,
  UINT8 BusLimit,
  UINT8 PciSegment
  );

/**
  OOB: Get the pmon offset table

  @param   UnitTable  - Table to fill in with discovery unit offsets
  @param   ChopType   - The CPU chop type to be used
  @param   PmonConfig - Indicates what PMONs will be configured

  @retval            - True if pmon table was supported

**/
BOOLEAN
EFIAPI
OobMsmGetPmonOffsetTable (
  PMON_DISC_INFO  UnitTable[UNIT_TYPE_MAX],
  UINT8           ChopType,
  UINT8           PmonConfig
  );

/**
  OOB: Get the available pmon count

  @param   ChopType   - The CPU chop type to be used

  @retval             - The number of PMONs available or zero if not supported

**/
UINT32
EFIAPI
OobMsmGetPmonAvailableCount (
  UINT8           ChopType
  );

/**
  OOB: Get the unit ID from the block ID and Unit type

  @param  ChopType    - The CPU chop type to be used
  @param  UnitType    - Type of Unit being accessed
  @param  PmonIndex   - Index of the unit being accessed within the pmon block type
  @param  NewUnit     - Place to store the unit default values

  @retval             - The Pmon index within the unit or PMON_BLOCK_ID_INV

**/
UINT8
EFIAPI
OobMsmNewUnitDefaultValue (
  UINT8     ChopType,
  UNIT_TYPE UnitType,
  UINT8     PmonIndex,
  UNIT_DISCOVERY_STRUCT *NewUnit
  );

/**
  OOB: Get the pmon global default register value based on CPU type

  @param   ChopType             - The CPU chop type to be used
  @param   GlobalDiscoveryState - Variable used to store the default value

  @retval            - True if pmon table was supported

**/
BOOLEAN
EFIAPI
OobMsmGetPmonGlobalDiscDefault (
  UINT8           ChopType,
  GLOBAL_DISCOVERY_STATE *GlobalDiscoveryState
  );

/**
  OOB: Get the pmon unit offset

  @param  ChopType    - The CPU chop type to be used
  @param  PmonIndex   - Index of the unit being accessed within the pmon block type
  @param  UnitType    - The unit type being accessed
  @param  Unit        - Index of the unit being accessed within the pmon block type

  @retval             - The Pmon offset

**/
UINT32
EFIAPI
OobMsmGetPmonOffset (
  UINT8          ChopType,
  UINT8          PmonIndex,
  UNIT_TYPE      UnitType,
  PMON_DISC_INFO Unit
  );

/**
  OOB: Set NvDimm mailbox registers for OOBMSM.

  This function programs the address offset and dimm configuration into the OOB NvDimm mailbox registers
  for OOBMSM MCTP proxy to support MCTP over DDRT-2 feature.

  @param[in] ScktId               Socket index.
  @param[in] OobNvdimmMailbox     Data information of NvDimm mailbox address.

  @retval EFI_SUCCESS             OOB sets the NvDimm mailbox registers successfully.
  @retval EFI_INVALID_PARAMETER   The input of this function is invalid.

**/
EFI_STATUS
EFIAPI
OobSetNvDimmMailBox (
  IN UINT8                ScktId,
  IN OOB_NVDIMM_MAILBOX   *OobNvdimmMailbox
  );
#endif //_OOB_LIB_H_

