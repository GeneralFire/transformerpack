/** @file
  PCH FIA MUX config block containing user configuration options.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/
#ifndef _FIA_MUX_CONFIG_H_
#define _FIA_MUX_CONFIG_H_

#include <ConfigBlock.h>
#include <PchLimits.h>

#define PCH_FIA_MUX_PREMEM_CONFIG_REVISION   1

extern EFI_GUID gFiaMuxPreMemConfigGuid;

#pragma pack (push,1)

//
//  The structure of FIA MUX configuration that can be set in ME.
//
typedef struct {
    UINT8 FiaMuxSelection    : 2;
    UINT8 FiaMuxHardwareIndex: 6;
} FIA_MUX_RECORD;

typedef struct {
    UINT8 SataSelection    : 2;
    UINT8 SataHardwareIndex: 6;
} SATA_RECORD;

typedef struct {
    UINT8 PcieRootPortEnable    : 1;
    UINT8 PcieRootPortLinkWidth : 4;
    UINT8 Reserved              : 3;
    UINT8 PcieRootPortHardwareIndex;
} PCIE_ROOT_PORT_RECORD;

#define FIA_FILER_SIZE  (4 - (PCH_MAX_HSIO_LANES & 0x3))

typedef struct {
  UINT8                        FiaMuxCfgInvalidate;                      // FiaMuxInvalidate setting passed in Set message
  UINT8                        FiaMuxRecordsCount;                       // Number of FiaMux lanes in configuration
  UINT8                        SataRecordsCount;                         // Number of SATA lanes described in configuration
  UINT8                        PcieRootPortRecordsCount;                 // Number of RootPort lanes described in configuration
  FIA_MUX_RECORD               FiaMux[PCH_MAX_HSIO_LANES];               // Table pointer to FiaMux description table
  SATA_RECORD                  Sata[PCH_MAX_HSIO_LANES];                 // Table pointer to SATA description table
  PCIE_ROOT_PORT_RECORD        PcieRootPort[PCH_MAX_HSIO_LANES];         // Table pointer to RootPorts description table
  UINT8                        FiaMuxLaneOverride[PCH_MAX_HSIO_LANES];   // Table pointer to override per lane switches
  UINT8                        Reserved[FIA_FILER_SIZE];                 // filler to have ConfigBlock size multiple of 4 bytes
                                                                         // PCH_MAX_HSIO_LANES == 38
} ME_FIA_CONFIG;

/**
  Describes the FIA MUX premem policy.
**/
typedef struct {
  CONFIG_BLOCK_HEADER           Header;                   // Config Block Header
  UINT8                         FiaMuxOverrideRequired;   // Enable/Disable ME_FIA_CONFIG override
  UINT8                         Rsvd[3];
  ME_FIA_CONFIG                 FiaMuxConfig;             // FIA MUX override configuration
} PCH_FIA_MUX_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _FIA_MUX_CONFIG_H_
