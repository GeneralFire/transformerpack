/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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
#ifndef _CRASH_LOG_CPU_LIB_H_
#define _CRASH_LOG_CPU_LIB_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UsraCsrLib.h>
#include <Guid/CrashLog.h>

/// The Physical Layer PCI Express Extended Capability definitions.
///
/// Based on section 7.39 of PCI Express Base Specification 4.0.
///@{
#define PCI_EXPRESS_EXTENDED_CAPABILITY_DVSEC_ID    0x0023
#define PCI_EXPRESS_EXTENDED_CAPABILITY_DVSEC_VER1  0x1

// Register offsets from Physical Layer PCI-E Ext Cap Header
#define PCI_EXPRESS_REG_DVSEC_CAPABILITIES_OFFSET                         0x04
#define INTEL_REG_DVSEC_DISCOVERY_OFFSET                                  0x08

#define V_CRASHLOG_DVSEC_ID                           0x04          // CrashLog DEVSC ID
#define V_CRASHLOG_DVSEC_TBIR_BAR0                    0
#define V_CRASHLOG_DVSEC_TBIR_BAR1                    1

#pragma pack(1)

typedef union {
  struct {
    UINT32 CapabilityId:16;
    UINT32 CapabilityVersion:4;
    UINT32 NextCapabilityOffset:12;
  } Bits;
  UINT32   Data32;
} PCI_EXPRESS_EXTENDED_CAPABILITIES_HDR;

typedef union {
  struct {
    UINT32 DvsecVid                  : 16; // DVSEC Vendor ID
    UINT32 DvsecRev                  : 4;  // DVSEC Version
    UINT32 DvsecLen                  : 12; // DVSEC Length
  } Bits;
  UINT32   Data32;
} PCI_EXPRESS_REG_DVSEC_CAPABILITIES;

typedef union {
  struct {
    UINT32  DevscId               : 16;   // DVSEC_ID
    UINT32  NumEntries            : 8;    // Number of entries, describes the number of telemetry aggregators that would exist in this capability lookup table.
    UINT32  EntrySize             : 8;    // Entry Size in DWORDS
    UINT32  TBir                  : 3;    // tBIR, The BAR to be used
    UINT32  DiscoveryTableOffset  : 29;   // Discovery Table Offset
  } Bits;
  UINT32 Data32[2];
} INTEL_REG_DVSEC_DISCOVERY;

typedef struct {
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HDR                         Header;
  PCI_EXPRESS_REG_DVSEC_CAPABILITIES                            Capablities;
  INTEL_REG_DVSEC_DISCOVERY                                     DiscoveryData;
} PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC;
///@}

#define CRASH_LOG_HEADER_SIZE_IN_DWORDS     4
#define CRASH_LOG_ACCESS_TYPE_BDF_BARID     1
#define CRASH_LOG_ACCESS_TYPE_LCL_BARID     2
#define CRASH_LOG_ACCESS_TYPE_LOCAL         3
//
// Structure of Cpu CrashLog Header
//
typedef union {
  struct {
    UINT32  AccessType            : 4;             ///< Access Type
    UINT32  CrashType             : 4;             ///< Crash Type
    UINT32  Count                 : 7;
    UINT32  CrashLogDisabled      : 1;
    UINT32  CrashlogCapability    : 1;
    UINT32  DisableCommand        : 1;
    UINT32  DiscoveryMech         : 2;
    UINT32  ManualTriggerSupp     : 1;
    UINT32  ClearCommand          : 1;
    UINT32  TriggerOnAllReset     : 1;
    UINT32  ReARMSupported        : 1;
    UINT32  CrashlogInProgress    : 1;
    UINT32  TriggerARMStatus      : 1;
    UINT32  TriggerOnAllResets    : 1;
    UINT32  DisableCrashDump      : 1;
    UINT32  ClearCrashDump        : 1;
    UINT32  ExecuteCrashDump      : 1;
    UINT32  Reserved              : 1;
    UINT32  CrashDump_Complete    : 1;
    UINT32  Guid              : 32;            ///< GLOBALLY_UNIQUE_ID
    union {
      struct {
        UINT32  Offset;                            ///< Offset in this space where the Telemetry Region Starts
        UINT32  SizeInDwords;                      ///< Size in DWORDS
      } Local;
      struct {
        UINT32  tBIR                : 3;
        UINT32  offset              : 29;
        UINT32 SizeInDwords         : 32;
      } BarId;
      struct {
        UINT32  Segment             : 8;
        UINT32  Bus                 : 8;
        UINT32  Device              : 5;
        UINT32  Function            : 3;
        UINT32  Reserved            : 8;
        UINT32  tBIR                : 3;
        UINT32  offset              : 29;
      } BdfBarId;
    } Address;

  } Fields;
  UINT64 Data[CRASH_LOG_HEADER_SIZE_IN_DWORDS/2];
} CPU_CRASHLOG_DISCOVERY_HEADER;

#pragma pack()

/**
  Find the address of Cpu CrashLog Capability

  @param[IN]  SocId                Socket ID of the processor
  @param[OUT] CrashLogDevscCap     Pointer to structure of CrashLog DEVSC Data

  @retval EFI_UNSUPPORTED          Cpu CrashLog ID not found
  @retval EFI_SUCCESS              Cpu CrashLog ID found
**/
EFI_STATUS
GetCpuCrashLogCapability (
  IN  UINT8                                    SocId,
  OUT PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC  *CrashLogDevscCap
  );

/**
  This function discovers CPU crashLog region and gets the size and base address

  @param[in]  SocketId                - Socket ID
  @param[out] CpuCrashLogRecRegs      - Pointer to CrashLog Discovery Data

  @retval EFI_UNSUPPORTED         Cpu CrashLog is not supported
  @retval EFI_SUCCESS             Cpu CrashLog is supported
**/
EFI_STATUS
CpuCrashLogDiscovery (
  IN  UINT8                                 SocId,
  OUT CPU_CRASHLOG_RECORD_REGIONS_STRUCT    *CpuCrashLogRecRegs
  );

/**
  This function clears cpu crash log

  @param[in] SocketId                  - Socket ID

  @retval EFI_SUCCESS

**/
EFI_STATUS
CpuCrashLogClear (
  UINT8       SocId
  );

/**
  This function ReArm cpu crash log

  @param[in] SocketId                  - Socket ID

  @retval EFI_SUCCESS

**/
EFI_STATUS
CpuCrashLogReArm (
  UINT8       SocId
  );

/**
  This function enable/disables cpu crash log
  @param[in] SocketId                  - Socket ID
  @param[in] Enable                    - TRUE = Enable, FALSE = Disable

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED

**/
EFI_STATUS
CpuCrashLogEnableDisable (
  UINT8       SocId,
  BOOLEAN     Enable
  );

/**
  This function enable/disables cpu crash log collect on all resets

  @param[in] SocketId                  - Socket ID
  @param[in] Enable                    - TRUE = Enable, FALSE = Disable

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED
**/
EFI_STATUS
CpuCrashLogOnAllReset (
  UINT8       SocId,
  BOOLEAN     Enable
  );

/**
  Init PMon crash log Bar

  @param[in] SocketId                  - Socket ID
  @param[in] CrashLogDevscCap          - Structure of CrashLog DEVSC Data
  @param[in] SavedStsCmd               - the saved value of PMon PCICFG status and command register

  @retval FALSE - Crash Log Bar already asigned
  @retval TRUE -  Crash Log Bar asigned by this routine
**/
BOOLEAN
InitOobMsmPmonCrashlogBar (
  IN  UINT8                                    SocketId,
  IN  PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC  *CrashLogDevscCap,
  OUT UINT32                                   *SavedStsCmd
  );

/**
  Clear PMon crash log Bar and restore the PMon PCICFG status and command register value

  @param[in] SocketId                  - Socket ID
  @param[in] CrashLogDevscCap          - Structure of CrashLog DEVSC Data
  @param[in] SavedStsCmd               - the restore value of PMon PCICFG status and command register
  @param[in] ClearBar                  - if TRUE need to clear the bar, else don't

  return none
**/
VOID
ClearOobMsmPmonCrashlogBar (
  IN UINT8                                    SocketId,
  IN PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC  *CrashLogDevscCap,
  IN UINT32                                   SavedStsCmd,
  IN BOOLEAN                                  ClearBar
  );

#endif //_CRASH_LOG_CPU_LIB_H_

