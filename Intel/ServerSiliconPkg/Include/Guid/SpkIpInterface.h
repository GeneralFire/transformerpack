/** @file
  SPK generation specific silicon access interface

  This library abstracts silicon register differences between different versions
  of SPK IP blocks.
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
#ifndef __SPK_IP_INTERFACE_H__
#define __SPK_IP_INTERFACE_H__

#include <Uefi.h>
#include <Base.h>
#include <Library/UsraCsrLib.h>

/**
  SPK:  Set the starting address using and index

  @param MmioBar      - MMIO BAR
  @param PciBarOffset - PCI BAR offset
  @param ClusterIdx   - The cluster index
  @param ClMemBase    - The cluster memory base address

  @retval             - EFI_UNSUPPORTED: the function not implemented
                        EFI_INVALID_PARAMETER   A required parameter was NULL or is out of range.
                        EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
SpkSetStartingAddr (
  UINT32 MmioBar,
  UINT32 PciBarOffset,
  UINT8  ClusterIdx,
  UINT32 ClMemBase
  );
//
// Function pointer definiton for SpkSetStartingAddr()
//
typedef
EFI_STATUS
(EFIAPI *SPK_SET_STARTING_ADDR) (
  UINT32 MmioBar,
  UINT32 PciBarOffset,
  UINT8  ClusterIdx,
  UINT32 ClMemBase
  );

/**
  SPK:  Set SNC upper base configuration

  @param MmioBar       - MMIO BAR
  @param PciBarOffset  - PCI BAR offset
  @param SncUpperBase  - SNC upper base configuration

  @retval              - EFI_UNSUPPORTED: the function not implemented
                         EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
SpkSetSncUpperBase (
  UINT32 MmioBar,
  UINT32 PciBarOffset,
  UINT32 SncUpperBase
  );
//
// Function pointer definiton for SpkSetSncUpperBase()
//
typedef
EFI_STATUS
(EFIAPI *SPK_SET_SNC_UPPER_BASE) (
  UINT32 MmioBar,
  UINT32 PciBarOffset,
  UINT32 SncUpperBase
  );

/**
  SPK:  Set Snc

  @param MmioBar      - MMIO BAR
  @param PciBarOffset - PCI BAR offset
  @param SncConfig    - SNC configuration value

  @retval             - EFI_UNSUPPORTED: the function not implemented
                        EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
SpkSetSncCfg (
  UINT32 MmioBar,
  UINT32 PciBarOffset,
  UINT32 SncConfig
  );
//
// Function pointer definiton for SpkSetSncCfg()
//
typedef
EFI_STATUS
(EFIAPI *SPK_SET_SNC_CFG) (
  UINT32 MmioBar,
  UINT32 PciBarOffset,
  UINT32 SncConfig
  );

/**
  SPK:  Set UMA cluster configuration

  @param MmioBar        - MMIO BAR
  @param PciBarOffset   - PCI BAR offset
  @param UmaClusterEn   - UMA clustering enable
  @param XorDefeature   - Enable for XOR defeature mode

  @retval               - EFI_UNSUPPORTED: the function not implemented
                          EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
SpkSetUmaClusterCfg (
  UINT32 MmioBar,
  UINT32 PciBarOffset,
  UINT8  UmaClusterEn,
  UINT8  XorDefeatureg
  );
//
// Function pointer definiton for SpkSetUmaClusterCfg()
//
typedef
EFI_STATUS
(EFIAPI *SPK_SET_UMA_CLUSTER_CFG) (
  UINT32 MmioBar,
  UINT32 PciBarOffset,
  UINT8  UmaClusterEn,
  UINT8  XorDefeatureg
  );

/**
  SPK:  Set Snc uncore configuration

  @param MmioBar          - MMIO BAR
  @param PciBarOffset     - PCI BAR offset
  @param NumChaPerCluster - Number of CHAs per SNC Cluster
  @param BaseChaCluster1  - Base of CHA Cluster 1
  @param BaseChaCluster2  - Base of CHA Cluster 2
  @param BaseChaCluster3  - Base of CHA Cluster 3

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
SpkSetSncUncoreCfg (
  UINT32 MmioBar,
  UINT32 PciBarOffset,
  UINT8  NumChaPerCluster,
  UINT8  BaseChaCluster1,
  UINT8  BaseChaCluster2,
  UINT8  BaseChaCluster3
  );
//
// Function pointer definiton for SpkSetSncUncoreCfg()
//
typedef
EFI_STATUS
(EFIAPI *SPK_SET_SNC_UNCORE_CFG) (
  UINT32 MmioBar,
  UINT32 PciBarOffset,
  UINT8  NumChaPerCluster,
  UINT8  BaseChaCluster1,
  UINT8  BaseChaCluster2,
  UINT8  BaseChaCluster3
  );

typedef struct {
  //
  // Function pointers
  //
  SPK_SET_STARTING_ADDR   SpkSetStartingAddr;
  SPK_SET_SNC_UPPER_BASE  SpkSetSncUpperBase;
  SPK_SET_SNC_CFG         SpkSetSncCfg;
  SPK_SET_UMA_CLUSTER_CFG SpkSetUmaClusterCfg;
  SPK_SET_SNC_UNCORE_CFG  SpkSetSncUncoreCfg;
} SPK_IP_INTERFACE;

//
// This API supports multiple instances with the GUID identifying the instance
//
#define SPK_IP_INTERFACE_CPU_GUID \
  { 0x3edf7f6d, 0x2061, 0x4941, { 0xb6, 0x31, 0xdd, 0x40, 0xa0, 0xba, 0xcb, 0xad } }  // {3EDF7F6D-2061-4941-B631-DD40A0BACBAD}

extern EFI_GUID gSpkIpInterfaceCpuGuid;

#endif // __SPK_IP_INTERFACE_H__
