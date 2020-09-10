/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation. <BR>

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
**/

#ifndef _ME_FIA_MUX_LIB_H_
#define _ME_FIA_MUX_LIB_H_

#include <FiaMuxConfig.h>

//
// Define a gap of non used lines
//
// SNR uses 0-7 & 16-23
#define HSIO_GAP_SNR_LOW                 7
#define HSIO_GAP_SNR_HI                  16
// TNR uses 0-15 & 20-23
#define HSIO_GAP_TNR_LOW                 15
#define HSIO_GAP_TNR_HI                  20
// ICX-D uses 0 - 23 (no gap)

typedef enum {
  FIA_MUX_LANE_DISABLED,
  FIA_MUX_LANE_PCIE_OWNER,
  FIA_MUX_LANE_SATA_OWNER,
  FIA_MUX_LANE_USB_OWNER
} FIA_MUX_SELECTION;

typedef enum {
  SATA_CONTROLLER_ASSIGNED,
  SATA_CONTROLLER_NOT_ASSIGNED
} SATA_SELECTION;

typedef enum {
  PCIE_ROOT_PORT_IS_DISABLED,
  PCIE_ROOT_PORT_IS_ENABLED
} PCIE_ROOT_PORT_ENABLED;

typedef enum {
  PCIE_ROOT_PORT_LINK_WIDTH_SET_BY_BICTRL = 0,
  PCIE_ROOT_PORT_LINK_WIDTH_X1 = 0xF
} PCIE_ROOT_PORT_LINK_WIDTH;


/**
  Check if FIA MUX is supported by ME.

  @retval TRUE         FiaMux messages are allowed
  @retval FALSE        FiaMux messages are not allowed
**/
BOOLEAN
MeFiaMuxAllowed (
  VOID
  );

/**
  Read FIA MUX configuration from ME.

  @param [out] pConfig              Pointer to a buffer for ME FIA MUX configuration
  @param [out] pLanesAllowed        Pointer to a buffer for the number of lanes allowed
  @param [out] FiaMuxConfigFromMe   Pointer to a buffer for ME FIA MUX configuration (without overwriting)
  @param [out] OverridesActive      Pointer to a OverridesActive indicator.
                                    If TRUE, set command should be executed

  @retval EFI_NOT_READY         Returned if this API is called before ME signals init
                                complete, or after END_OF_POST was sent.
  @retval EFI_INVALID_PARAMETER Returned if any passed NULL configuration pointer
  @retval EFI_DEVICE_ERROR      Returned if HECI transport is not available.
  @retval EFI_PROTOCOL_ERROR    Unexpected protocol error returned by ME.
  @retval EFI_SUCCESS           FiaMux configuration has been read successfully
**/
EFI_STATUS
EFIAPI
MeFiaMuxConfigGet (
  OUT ME_FIA_CONFIG *pConfig,
  OUT ME_FIA_CONFIG *FiaMuxConfigFromMe,
  OUT UINT32        *pLanesAllowed,
  OUT BOOLEAN       *OverridesActive
  );

/**
  Sends FIA MUX configuration to ME.

  @param [in] pConfig           Pointer to FIA MUX configuration to set in ME,
                                when NULL, Invalidate field is set to 1 (causing
                                invalidate of configuration stored in flash -
                                configuration from Soft Straps will be used)

  @retval EFI_NOT_READY         Returned if this API is called before ME signals init
                                complete, or after END_OF_POST was sent.
  @retval EFI_DEVICE_ERROR      Returned if HECI transport is not available.
  @retval EFI_PROTOCOL_ERROR    Unexpected protocol error returned by ME.
  @retval EFI_OUT_OF_RESOURCES  Requested configuration refused by ME, because
                                it does not match the SoC SKU.
  @retval EFI_ABORTED           Requested configuration refused by ME, because it is
                                invalid, or nothing to update
  @retval EFI_INVALID_PARAMETER Input parameter is invalid
  @retval EFI_SUCCESS           FiaMux configuration has been sent successfully
**/
EFI_STATUS
EFIAPI
MeFiaMuxConfigSet (
  IN ME_FIA_CONFIG *pConfig
  );

/**
  Get number of HSIO lanes for given CPU

  @param [out]  LaneGapLow  pointer to return low limit of unused lines
  @param [out]  LaneGapHi   pointer to return hi limit of unused lines

  @retval UINT8             Number of Hsio lanes in the system
**/
UINT8
GetHsioLanes (
  OUT UINT8 *LaneGapLow,
  OUT UINT8 *LaneGapHi
  );

#endif // _ME_FIA_MUX_LIB_H_
