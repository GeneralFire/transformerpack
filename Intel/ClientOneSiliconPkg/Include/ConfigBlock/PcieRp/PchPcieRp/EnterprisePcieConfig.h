/** @file
  PCH Enterprice Pcie root port policy

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

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

#ifndef _ENTERPRISE_PCIE_CONFIG_H_
#define _ENTERPRISE_PCIE_CONFIG_H_

#include <PchLimits.h>
#include <PcieConfig.h>
#include <ConfigBlock.h>


#define PCIE_LINK_PREMEM_CONFIG_REVISION 1
extern EFI_GUID gPcieLinkPrememConfigGuid;

#pragma pack (push,1)


typedef struct {
  /**
    Specifies the value of the downstream port transmitter preset to be used during phase 1 of the equalization process.
    Will be applied to all connected lanes of the root port.
  **/
  UINT32                         DownstreamPortTxPreset;
  /**
    Specifies the value of the upstream port transmitter preset to be used during phase 1 of the equalization process.
    Will be applied to all connected lanes of the root port.
  **/
  UINT32                         UpstreamPortTxPreset;
} ENTERPRISE_PCIE_LINK_TRANSMITTER_PRESETS;


typedef struct {
  /**
    TRUE:  transmitter coefficient override enabled
    FALSE: transmitter coefficient override disabled
  **/
  UINT8       LocalTxOverrideEnable;
  /**
    Specifies the value of the transmitter coefficient pre-cursor preset override
  **/
  UINT8       TxCoeffPreCursor;
  /**
    Specifies the value of the transmitter coefficient main cursor preset override
  **/
  UINT8       TxCoeffCursor;
  /**
    Specifies the value of the transmitter coefficient post-cursor preset override
  **/
  UINT8       TxCoeffPostCursor;
} ENTERPRISE_PCIE_LINK_TX_COEFF;

#define ENTERPRISE_PCIE_LINK_EQ_MAX_LIST_ENTRIES 12

typedef enum {
  EnterprisePcieEqFormatPreset = 0,
  EnterprisePcieEqFormatCoefficient,
  EnterprisePcieEqFormatMaxBoostLine,
  EnterprisePcieEqFormatNextStage,
  EnterprisePcieEqFormatDisabled = 0xff
} ENTERPRISE_PCIE_LINK_EQ_ENTRY_FORMAT;

typedef struct {
  /**
    Specifies the format of the FOM hint table entry type.
  **/
  UINT8  EntryFormat;  // Entry format
  UINT8  D1;           // See FOM Hint table description
  UINT8  D2;           // in EIP PCIe HAS Chapter 28
  UINT8  Reserved;
} ENTERPRISE_PCIE_LINK_EQ_FOMHINTTBL_ENTRY;

typedef struct {
  /**
    List of the equalization entries that provide points to search through during equalization process.
  **/
  ENTERPRISE_PCIE_LINK_EQ_FOMHINTTBL_ENTRY  Ph3Entries[ENTERPRISE_PCIE_LINK_EQ_MAX_LIST_ENTRIES];
} ENTERPRISE_PCIE_LINK_EQ_FOMHINTTBL;


typedef struct {
  ///
  /// This member states whether the PCIe Equalization settings should be overridden by the values from BIOS setup
  /// or Reference Code default values should be used. <b>0: Don't override RC default values</b>; 1: Override RC default values
  ///
  UINT32                                    OverrideDefaultEqSettings:  1;
  UINT32                                    Reserved                 : 31;

  ENTERPRISE_PCIE_LINK_TRANSMITTER_PRESETS  Ph1Settings;
  ENTERPRISE_PCIE_LINK_TX_COEFF             Ph2Settings;
  ENTERPRISE_PCIE_LINK_EQ_FOMHINTTBL        Ph3Settings;
} ENTERPRISE_PCIE_LINK_EQ_CONFIG;

/**
  The ENTERPRISE_PCIE_LINK_EQ_PREMEM_CONFIG block describes the expected configuration of the PCH PCI Express link equalization

  <b>Revision 1</b>:
  - Init version
**/
typedef struct {
  CONFIG_BLOCK_HEADER      Header;                     ///< Config Block Header

  ENTERPRISE_PCIE_LINK_EQ_CONFIG EqConfig[PCH_MAX_PCIE_ROOT_PORTS];
} ENTERPRISE_PCIE_LINK_PREMEM_CONFIG;


#pragma pack (pop)


#endif //_ENTERPRISE_PCIE_CONFIG_H_
