/** @file
  HSIO routines

@copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation.

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

@par Specification
**/
#ifndef _PEI_HSIO_LIB_H_
#define _PEI_HSIO_LIB_H_

#include <Ppi/SiPolicy.h>
#include <Library/PchPcrLib.h>
#include <Register/PchPcrRegs.h>

/**
PCH PCIE PLL SSC Data
**/
#define MAX_PCIE_PLL_SSC_PERCENT  20
#define PCIE_PLL_SSC_AUTO         0xFF

/**
  Represents HSIO lane
**/
typedef struct {
  UINT8        Index; ///< Lane index
  UINT8        Pid;   ///< Sideband ID
  UINT16       Base;  ///< Sideband base address
} HSIO_LANE;

/**
  Read HSIO lane DWORD

  @param[in] HsioLane   HSIO Lane
  @param[in] Offset     PCR offset
  @return DWORD value
**/
UINT32
HsioLaneRead32 (
  CONST HSIO_LANE *HsioLane,
  UINT32 Offset
  );

/**
  Write HSIO lane DWORD

  @param[in] HsioLane   HSIO Lane
  @param[in] Offset     PCR offset
  @param[in] Data32     DWORD Value
**/
VOID
HsioLaneWrite32 (
  CONST HSIO_LANE *HsioLane,
  UINT32          Offset,
  UINT32          Data32
  );

/**
  Modify HSIO lane DWORD.

  @param[in] HsioLane   HSIO Lane
  @param[in] Offset     PCR offset
  @param[in] AndMask    Mask to be ANDed with original value.
  @param[in] OrMask     Mask to be ORed with original value.
**/
VOID
HsioLaneAndThenOr32 (
  CONST HSIO_LANE *HsioLane,
  UINT32          Offset,
  UINT32          AndMask,
  UINT32          OrMask
  );

/**
  Modify HSIO lane DWORD and update ChipsetInit SUS Table.

  @param[in] HsioLane   HSIO Lane
  @param[in] Offset     PCR offset
  @param[in] AndMask    Mask to be ANDed with original value.
  @param[in] OrMask     Mask to be ORed with original value.
**/
VOID
HsioLaneAndThenOr32WithSusWrite (
  CONST HSIO_LANE *HsioLane,
  UINT32          Offset,
  UINT32          AndMask,
  UINT32          OrMask
  );

/**
  Write PCR register and update ChipsetInit SUS Table.
  It programs PCR register and size in 4bytes then update ChipsetInit SUS Table..
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  AndData  AND Data. Must be the same size as Size parameter.
  @param[in]  OrData   OR Data. Must be the same size as Size parameter.

**/
VOID
PchPcrAndThenOr32WithSusWrite (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset,
  IN  UINT32                            AndData,
  IN  UINT32                            OrData
  );

/**
  Initialize HSIO lane for non-common clock PCIe port.

  @param[in] HsiLane   HSIO Lane
**/
VOID
HsioPcieNccLaneInit (
  CONST HSIO_LANE     *HsioLane
);

/**
  Tune the USB 3.0 signals quality.

  @param[in]  Usb3HsioConfig      The USB3_HSIO_CONFIG policy instance

**/
VOID
XhciUsb3HsioTuning (
  IN  USB3_HSIO_CONFIG      *Usb3HsioConfig
  );

/**
  Enables SATA test mode in Modular PHY

  @param[in]  SataCtrlIndex     SATA controlller index
  @param[in]  CtrlPortNum       SATA controlller number of ports

**/
VOID
HsioSataTestModeEnable (
  IN  UINT32    SataCtrlIndex
  );

/**
  The function program HSIO registers.

  @param[in] SiPreMemPolicyPpi          The SI PREMEM Policy PPI instance

  @retval EFI_SUCCESS                   THe HSIO settings have been programmed correctly
**/
VOID
PchHsioBiosProgPreMem (
  IN  SI_PREMEM_POLICY_PPI             *SiPreMemPolicyPpi
  );

/**
  The function program HSIO registers.

  @param[in] SiPolicyPpi               The PCH Policy PPI instance

**/

VOID
PchHsioConfigure (
  IN  SI_POLICY_PPI    *SiPolicyPpi
  );

#endif // _PEI_HSIO_LIB_H_
