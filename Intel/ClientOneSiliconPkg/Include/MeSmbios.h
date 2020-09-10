/**@file
  Header file for the MeSmbios Driver.

@copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification Reference:
**/

#ifndef _ME_SMBIOS_H_
#define _ME_SMBIOS_H_

#include <Library/BaseLib.h>
#include <IndustryStandard/SmBios.h>
#include <Guid/SmBios.h>
#include <Protocol/Smbios.h>
#include <Protocol/MebxProtocol.h>
#include <Library/DxeMeLib.h>

#define MESMBIOS_TYPE_130           0x82
#define MESMBIOS_TYPE_131           0x83

///
/// OEMCapabilities1
///
#define BIOS_REFLASH_BIT_OFFSET      2
#define BIOS_SETUP_BIT_OFFSET        3
#define BIOS_PAUSE_BIT_OFFSET        4

///
/// OEMCapabilities2
///
#define TERMINAL_EMULATION_VT100PLUS 0x02

///
/// OEMCapabilities3
///
#define SECURE_ERASE_BIT_OFFSET      6
#define BIOS_SECURE_BOOT_BIT_OFFSET  7

///
/// OEMCapabilities4
///
#define VPRO_TBT_DOCK_BIT_OFFSET     0

///
/// This is definition for SMBIOS Oem data type 0x82
///
typedef struct {
  SMBIOS_STRUCTURE              Header;
  UINT8                         AmtSignature[4];
  UINT8                         AmtSupported;
  UINT8                         AmtEnabled;
  UINT8                         SrouEnabled;
  UINT8                         SolEnabled;
  UINT8                         NetworkEnabled;
  UINT8                         ExtendedData;
  UINT8                         OemCapabilities1;
  UINT8                         OemCapabilities2;
  UINT8                         OemCapabilities3;
  UINT8                         OemCapabilities4;
  UINT8                         KvmEnabled;
  UINT8                         Reserved;
} SMBIOS_TABLE_TYPE130;

typedef struct {
    UINT32  VMXState     : 1;    ///< [0]     VMX enabled state (On/Off) (This bit is a duplicate of Bit 5 and maintained for backward compatibility)
    UINT32  SMXState     : 1;    ///< [1]     SMX enabled state (On/Off) (This bit is a duplicate of Bit 3 and maintained for backward compatibility)
    UINT32  LtTxtCap     : 1;    ///< [2]     LT/TXT capability
    UINT32  LtTxtEnabled : 1;    ///< [3]     LT/TXT Enabled state (Optional for vPro verification)
    UINT32  VTxCap       : 1;    ///< [4]     VT-x capability
    UINT32  VTxEnabled   : 1;    ///< [5]     VT-x Enabled state (Optional for vPro verification)
    UINT32  Reserved     : 26;   ///< [31:6]  Reserved, set to 0
} CPU_CAP;

typedef struct {
    UINT32  FunctionNumber : 3;  ///< [2:0]     PCI Device Function Number
    UINT32  DeviceNumber   : 5;  ///< [7:3]     PCI Device Device Number
    UINT32  BusNumber      : 8;  ///< [15:8]    PCI Device Bus Number
    UINT32  DeviceID       : 16; ///< [31:16]   Device Identification Number (DID): This field is the identifier of PCH PCI device. DID will be set to 0xFF if not found.
    UINT32  Reserved       : 32; ///< [63:32]   Reserved, must set to 0
} PCH_CAP;

typedef struct {
    UINT32  FunctionNumber : 3;  ///< [2:0]   PCI Device Function Number of Wired LAN
    UINT32  DeviceNumber   : 5;  ///< [7:3]   PCI Device Device Number of Wired LAN
    UINT32  BusNumber      : 8;  ///< [15:8]  PCI Device Bus Number of Wired LAN
    UINT32  DeviceID       : 16; ///< [31:16] PCI Device Identifier (DID) of wired NIC. DID will be set to 0xFF if not found.
    UINT32  Reserved1      : 16; ///< [47:32] Reserved for Wired NIC.
    UINT32  Reserved2      : 16; ///< [63:48] Reserved, must set to 0 (Wireless)
    UINT32  Reserved3      : 32; ///< [95:64] Reserved, must set to 0 (Wireless)
} NETWORK_DEV;

typedef struct {
    UINT32  Reserved1    : 1;    ///< [0]    Reserved, must be set to 0
    UINT32  VTdSupport   : 1;    ///< [1]    BIOS supports VT-d in BIOS setup screen (can be editable)
    UINT32  TxtSupport   : 1;    ///< [2]    BIOS supports TXT in BIOS setup screen (can be editable)
    UINT32  Reserved2    : 1;    ///< [3]    Reserved, must be set to 0
    UINT32  Reserved3    : 1;    ///< [4]    Reserved, must be set to 0
    UINT32  VTxSupport   : 1;    ///< [5]    BIOS supports VT-x in BIOS setup screen (can be editable)
    UINT32  TdsMeasuredBoot     : 1;    ///< [6]    BIOS supports Intel Trusted Device Setup Measured Boot
    UINT32  TdsBiosLock         : 1;    ///< [7]    BIOS supports Intel Trusted Device Setup Boot and Setup Lock
    UINT32  TdsChassisIntrusion : 1;    ///< [8]    BIOS supports Intel Trusted Device Setup Chassis Intrusion
    UINT32  Reserved4           : 23;   ///< [31:9] Reserved, must be set to 0
} BIOS_CAP;

///
/// This is definition for SMBIOS Oem data type 0x83
///
typedef struct {
  SMBIOS_STRUCTURE               Header;
  CPU_CAP                        CpuCapabilities;
  MEBX_VER                       MebxVersion;
  PCH_CAP                        PchCapabilities;
  ME_CAP                         MeCapabilities;
  MEFWCAPS_SKU                   MeFwConfigured;
  NETWORK_DEV                    NetworkDevice;
  BIOS_CAP                       BiosCapabilities;
  UINT8                          StructureIdentifier[4];
  UINT32                         Reserved;
} SMBIOS_TABLE_TYPE131;

#endif
