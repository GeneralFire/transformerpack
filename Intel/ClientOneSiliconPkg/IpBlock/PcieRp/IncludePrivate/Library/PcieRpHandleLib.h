/** @file
  Header file for PCH PCIE handle library

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
#ifndef _PCIE_RP_HANDLE_LIB_
#define _PCIE_RP_HANDLE_LIB_

#include <PchPcieRpConfig.h>
#include <EnterprisePcieConfig.h>
#include <Library/PchPcieRpDev.h>
#include <Library/PsfLib.h>
#include <Library/PchSbiAccessLib.h>

/**
  Internal Root Port policy options
**/
typedef struct {
  UINT32 dummy;
} PCH_RP_PRIVATE_CONFIG;

/**
  Execute PCH SBI message
  Take care of that there is no lock protection when using SBI programming in both POST time and SMI.
  It will clash with POST time SBI programming when SMI happen.
  Programmer MUST do the save and restore opration while using the PchSbiExecution inside SMI
  to prevent from racing condition.
  This function will reveal P2SB and hide P2SB if it's originally hidden. If more than one SBI access
  needed, it's better to unhide the P2SB before calling and hide it back after done.

  When the return value is "EFI_SUCCESS", the "Response" do not need to be checked as it would have been
  SBI_SUCCESS. If the return value is "EFI_DEVICE_ERROR", then this would provide additional information
  when needed.

  @param[in] RpDevice                   Pointer to PCIE RP device
  @param[in] Pid                        Port ID of the SBI message
  @param[in] Offset                     Offset of the SBI message
  @param[in] Opcode                     Opcode
  @param[in] Posted                     Posted message
  @param[in, out] Data32                Read/Write data
  @param[out] Response                  Response

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_DEVICE_ERROR              Transaction fail
  @retval EFI_INVALID_PARAMETER         Invalid parameter
  @retval EFI_TIMEOUT                   Timeout while waiting for response
**/
typedef
EFI_STATUS
(*PCH_RP_SBI_EXECUTION) (
  IN     PCH_RP_DEVICE      *RpDevice,
  IN     PCH_SBI_PID        Pid,
  IN     UINT64             Offset,
  IN     PCH_SBI_OPCODE     Opcode,
  IN     BOOLEAN            Posted,
  IN OUT UINT32             *Data32,
  OUT    UINT8              *Response
  );

/**
  Disable PCIe Root Port at PSF level

  @param[in] RpDevice            Pointer to PCIE RP device
  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - RpDevice is NULL
**/
typedef
EFI_STATUS
(*PCH_RP_PSF_DISABLE_ROOT_PORT) (
  IN  PCH_RP_DEVICE         *RpDevice
  );

/**
  Full function for executing PCH SBI message
  Take care of that there is no lock protection when using SBI programming in both POST time and SMI.
  It will clash with POST time SBI programming when SMI happen.
  Programmer MUST do the save and restore opration while using the PchSbiExecution inside SMI
  to prevent from racing condition.
  This function will reveal P2SB and hide P2SB if it's originally hidden. If more than one SBI access
  needed, it's better to unhide the P2SB before calling and hide it back after done.

  When the return value is "EFI_SUCCESS", the "Response" do not need to be checked as it would have been
  SBI_SUCCESS. If the return value is "EFI_DEVICE_ERROR", then this would provide additional information
  when needed.

  @param[in] RpDevice                   Pointer to PCIE RP device
  @param[in] Pid                        Port ID of the SBI message
  @param[in] Offset                     Offset of the SBI message
  @param[in] Opcode                     Opcode
  @param[in] Posted                     Posted message
  @param[in] Fbe                        First byte enable
  @param[in] Bar                        Bar
  @param[in] Fid                        Function ID
  @param[in, out] Data32                Read/Write data
  @param[out] Response                  Response

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_DEVICE_ERROR              Transaction fail
  @retval EFI_INVALID_PARAMETER         Invalid parameter
  @retval EFI_TIMEOUT                   Timeout while waiting for response
**/
typedef
EFI_STATUS
(*PCH_RP_SBI_EXECUTION_EX) (
  IN     PCH_RP_DEVICE      *RpDevice,
  IN     PCH_SBI_PID        Pid,
  IN     UINT64             Offset,
  IN     PCH_SBI_OPCODE     Opcode,
  IN     BOOLEAN            Posted,
  IN     UINT16             Fbe,
  IN     UINT16             Bar,
  IN     UINT16             Fid,
  IN OUT UINT32             *Data32,
  OUT    UINT8              *Response
  );

/**
  Checks if a given PCIe lane is assigned any FIA lane

  @param[in]  RpDevice          Pointer to PCIE RP device
  @param[in]  PciePhysicalLane  Physical lane number

  @return  TRUE if given PCIe lane is assigned a FIA lane, FALSE otherwise
**/
typedef
BOOLEAN
(*PCH_RP_FIA_LANE_CONNECTED) (
  IN  PCH_RP_DEVICE         *RpDevice,
  IN  UINT32                PciePhysicalLane
  );

/**
  Read PCR register.
  It returns PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  RpDevice Pointer to PCIE RP device
  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of this Port ID
**/
typedef
UINT32
(*PCH_RP_PCR_READ32) (
  IN  PCH_RP_DEVICE         *RpDevice,
  IN  PCH_SBI_PID           Pid,
  IN  UINT32                Offset
  );

/**
  This function disables (non-static power gating) PCIe Root Port and enables ModPHY SPD gating (PCH-LP only).

  @param[in] RpDevice            Pointer to PCIE RP device
  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - RpDevice is NULL
**/
typedef
EFI_STATUS
(*PCH_RP_PMC_DISABLE_ROOT_PORT) (
  IN  PCH_RP_DEVICE         *RpDevice
  );

/**
  Write PCR register.
  It programs PCR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  RpDevice Pointer to PCIE RP device
  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  AndData  AND Data. Must be the same size as Size parameter.
  @param[in]  OrData   OR Data. Must be the same size as Size parameter.

  @retval UINT16       Value written to register
**/
typedef
UINT16
(*PCH_RP_PCR_AND_OR_16) (
  IN  PCH_RP_DEVICE         *RpDevice,
  IN  PCH_SBI_PID           Pid,
  IN  UINT32                Offset,
  IN  UINT16                AndData,
  IN  UINT16                OrData
  );

/**
  Set PCH IO port 80h cycle decoding to PCIE root port.
  System BIOS is likely to do this very soon after reset before PCI bus enumeration.
  This cycle decoding is allowed to set when DMI is unlocked

  @param[in] RpDevice            Pointer to PCIE RP device
  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - RpDevice is NULL
**/
typedef
EFI_STATUS
(*PCH_RP_LPC_PORT80_DECODE_SET) (
  IN  PCH_RP_DEVICE         *RpDevice
  );

/**
  Pcie Root Port callbacks
  List of function pointers can passed to IP Block driver
**/
typedef struct {
  PCH_RP_SBI_EXECUTION               PchRpSbiExecution;
  PCH_RP_PSF_DISABLE_ROOT_PORT       PchRpPsfDisablePcieRootPort;
  PCH_RP_SBI_EXECUTION_EX            PchRpSbiExecutionEx;
  PCH_RP_FIA_LANE_CONNECTED          PchRpIsPcieRootPortLaneConnected;
  PCH_RP_PCR_READ32                  PchRpPcrRead32;
  PCH_RP_PMC_DISABLE_ROOT_PORT       PchRpPmcDisablePcieRootPort;
  PCH_RP_PCR_AND_OR_16               PchRpPcrAndThenOr16;
  PCH_RP_LPC_PORT80_DECODE_SET       PchRpIoPort80DecodeSet;
} PCH_RP_CALLBACK;

/**
  PCIe Root Port structure
  Stores all data necessary to initialize PCIe IP block
**/
typedef struct {
  PCH_RP_DEVICE                       *Device;
  PCH_PCIE_CONFIG                     *PciExpressConfig;
  PCH_PCIE_RP_PREMEM_CONFIG           *PcieRpPreMemConfig;
  ENTERPRISE_PCIE_LINK_PREMEM_CONFIG  *PcieRpLinkEqPreMemConfig;
  PCH_RP_PRIVATE_CONFIG               *PrivateConfig;
  PCH_RP_CALLBACK                     *Callback;
} PCH_RP_HANDLE;

/**
  Assign callback functions

  @param[in] RpCallback          Pointer to PCIE RP callback structure
 **/
VOID
PchPcieRpAssignCallbacks (
  IN  PCH_RP_CALLBACK       *RpCallback
  );

#endif
