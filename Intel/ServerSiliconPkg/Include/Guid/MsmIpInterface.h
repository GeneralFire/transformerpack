/** @file
  MSM generation specific silicon access interface

  This library abstracts silicon register differences between different versions
  of MSM IP blocks.
  This library is designed to be directly linked into the Uncore consumer of these
  services
  OR
  To be used to expose a dynamic interface (PPI, DXE Protocol, or SMM Protocol) as
  appropriate for the phase being executed.

  Furthermore, this subsystem is designed to support multiple different versions
  dynamically linked at runtime.

  The expected valid uses for this library are thus:
  (1) One Uncore PEIM supporting a homogenous system and the single instance is statically linked
  (2) One Uncore PEIM supporting a heterogeneous system where a router instance is statically linked
    and multiple PEIM produce silicon specific instances of the library and the router selects the
    correct instance per socket.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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
#ifndef __MSM_IP_INTERFACE_H__
#define __MSM_IP_INTERFACE_H__

#include <Uefi.h>
#include <Base.h>
#include <Library/UsraCsrLib.h>

/**
  Init Bar 0 for SPK:
  If the BAR0 has already been initialized, re-use the BAR0, so no need to reassign temp bus.
  If BAR0 not enabled, assign one bar to it, then after configure SNC register,

  @param SocketId                     - KtiInternalGlobal
  @param SocketId                     - Socket ID
  @param StatusCommandDefaultValuePtr - pointer to the power on default value of PMon PCICFG status and command register
  @param SpkBar0                      - value of SPK BAR0
  @param MmioBar0Initialized          - Indicate whether MMIOB BAR0 has been initialized or not

  @retval                             - EFI_UNSUPPORTED: the function not implemented
                                        EFI_INVALID_PARAMETER   A required parameter was NULL or is out of range.
                                        EFI_SUCCESS: the function is excuted successfully
**/
EFI_STATUS
EFIAPI
SpkBarInit (
  IN UINT32                    TempBar,
  IN UINT8                     SocketId,
  OUT BOOLEAN                  *MmioBar0Initialized,
  OUT UINT32                   *StatusCommandDefaultValuePtr,
  OUT UINT32                   *SpkBar0
  );
//
// Function pointer definiton for SpkBarInit()
//
typedef
EFI_STATUS
(EFIAPI *SPK_BAR_INIT) (
  IN UINT32                    TempBar,
  IN UINT8                     SocketId,
  OUT BOOLEAN                  *MmioBar0Initialized,
  OUT UINT32                   *StatusCommandDefaultValuePtr,
  OUT UINT32                   *SpkBar0
  );

/**
  Clear PMon temp Bar 0 and restore the PMon PCICFG status and command register value

  @param SocketId                  - Socket ID
  @param StatusCommandDefaultValue - the power on default value of PMon PCICFG status and command register

  @retval                          - EFI_UNSUPPORTED: the function not implemented
                                     EFI_INVALID_PARAMETER   A required parameter was NULL or is out of range.
                                     EFI_SUCCESS: the function is excuted successfully
**/
EFI_STATUS
EFIAPI
ClearSpkBar (
  IN UINT8    SocketId,
  IN UINT32   StatusCommandDefaultValue
  );
//
// Function pointer definiton for ClearSpkBar()
//
typedef
EFI_STATUS
(EFIAPI *CLEAR_SPK_BAR) (
  IN UINT8    SocketId,
  IN UINT32   StatusCommandDefaultValue
  );

typedef struct {
  //
  // Function pointers
  //
  SPK_BAR_INIT   SpkBarInit;
  CLEAR_SPK_BAR  ClearSpkBar;
} MSM_IP_INTERFACE;

//
// This API supports multiple instances with the GUID identifying the instance
//
#define MSM_IP_INTERFACE_CPU_GUID \
  { 0x6ebabfae, 0xcfd2, 0x4fac, { 0x96, 0xa6, 0x8b, 0x2c, 0xf, 0xc3, 0x8, 0xbd } } // {6EBABFAE-CFD2-4FAC-96A6-8B2C0FC308BD}

extern EFI_GUID gMsmIpInterfaceCpuGuid;

#endif // __MSM_IP_INTERFACE_H__
