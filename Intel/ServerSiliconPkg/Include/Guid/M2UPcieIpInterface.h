/** @file
  Header file for M2UPcie Ip library

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
#ifndef __M2UPCIE_IP_INTERFACE_H__
#define __M2UPCIE_IP_INTERFACE_H__

#include <Uefi.h>
#include <Base.h>
#include <Library/UsraCsrLib.h>

/**
  M2UPCIE:  Check if device exists

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param DevExists     - Device exist

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieDeviceExists (
  IN  UINT8    SocId,
  IN  UINT8    CtrId,
  OUT BOOLEAN* DevExists
  );
//
// Function pointer definiton for M2UPcieDeviceExists()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_DEVICE_EXISTS) (
  IN  UINT8    SocId,
  IN  UINT8    CtrId,
  OUT BOOLEAN* DevExists

  );

/**
  M2UPCIE:  Unmask Global Errors

  @param SocId      - Socket ID
  @param CtrId      - Controller ID

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieUnmaskGlError (
  IN UINT8 SocId,
  IN UINT8 CtrId
  );
//
// Function pointer definiton for M2UPcieUnmaskGlError()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_UNMASK_GL_ERROR) (
  IN UINT8 SocId,
  IN UINT8 CtrId
  );

/**
  M2UPCIE:  Unmask Ingress Errors

  @param SocId         - Socket ID
  @param CtrId         - Controller ID

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieUnmaskR2PinError (
  IN UINT8 SocId,
  IN UINT8 CtrId
  );

/**
  M2UPCIE:  Unmask Ingress Errors

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param Parerring0msk - Parerring0msk bit field value
  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieMaskParerring0msk  (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 Parerring0msk
  );

//
// Function pointer definiton for M2UPcieUnmaskR2PinError()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_UNMASK_R2PIN_ERROR) (
  IN UINT8 SocId,
  IN UINT8 CtrId
  );

/**
  M2UPCIE:  Set UPI agent credits

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param VnaCr         - VNA Credits

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieSetUpiCrd (
  IN UINT8  SocId,
  IN UINT8  CtrId,
  IN UINT32 VnaCr
  );
//
// Function pointer definiton for M2UPcieSetUpiCrd()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_SET_UPI_CRD) (
  IN UINT8  SocId,
  IN UINT8  CtrId,
  IN UINT32 VnaCr
  );

/**
  M2UPCIE:  Set P2P Shared credits per upi agent

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param ShrCrdUpi0    - Shared P2P credits for UPI 0
  @param ShrCrdUpi1    - Shared P2P credits for UPI 1
  @param ShrCrdUpi2    - Shared P2P credits for UPI 2

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieSetP2PShrCrd (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 ShrCrdUpi0,
  IN UINT8 ShrCrdUpi1,
  IN UINT8 ShrCrdUpi2
  );
//
// Function pointer definiton for M2UPcieSetP2PShrCrd()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_SET_P2P_SHR_CRD) (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 ShrCrdUpi0,
  IN UINT8 ShrCrdUpi1,
  IN UINT8 ShrCrdUpi2
  );

/**
  M2UPCIE:  Program m2pcie route table

  @param[in] SocId           Socket ID
  @param[in] StackId         Stack ID
  @param[in] M2PcieKtiRt     Route Table

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieSetRouteTable (
  IN UINT8  SocId,
  IN UINT8  StackId,
  IN UINT32 M2PcieKtiRt
  );
//
// Function pointer definition for M2UPcieSetRouteTable()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_SET_ROUTE_TABLE) (
  IN UINT8  SocId,
  IN UINT8  StackId,
  IN UINT32 M2PcieKtiRt
  );

/**
  M2UPCIE:  Read and Write back the egress error log.

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param EgressErrLog  - Egress Error log

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieR2WriteBackEgressErrLog (
  IN  UINT8   SocId,
  IN  UINT8   CtrId,
  OUT UINT32* EgressErrLog
  );
//
// Function pointer definiton for M2UPcieR2WriteBackEgressErrLog()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_R2_WRITEBACKE_EGRESS_ERR_LOG) (
  IN  UINT8   SocId,
  IN  UINT8   CtrId,
  OUT UINT32* EgressErrLog
  );

/**
  M2UPCIE:  Read and Write back the ingress error log.

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param IngressErrLog - Ingress Error log

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieR2WriteBackIngressErrLog (
  IN  UINT8   SocId,
  IN  UINT8   CtrId,
  OUT UINT32* IngressErrLog
  );
//
// Function pointer definiton for M2UPcieR2WriteBackIngressErrLog()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_R2_WRITEBACK_INGRESS_ERR_LOG) (
  IN  UINT8   SocId,
  IN  UINT8   CtrId,
  OUT UINT32* IngressErrLog
  );

/**
  M2UPCIE:  Mask parity error in the ingress error mask.

  @param SocId         - Socket ID
  @param CtrId         - Controller ID

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieR2MaskParIngressErr (
  IN UINT8 SocId,
  IN UINT8 CtrId
  );
//
// Function pointer definiton for M2UPcieR2MaskParIngressErr)
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_R2_MASK_PAR_INGRESS_ERR) (
  IN UINT8 SocId,
  IN UINT8 CtrId
  );

/**
  M2UPCIE:  Clear DDRT Defeature.

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param DdrtQosMode   - DDRT QoS Mode

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieClearDdrtDefeature (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 DdrtQosMode
  );
//
// Function pointer definiton for M2UPcieClearDdrtDefeature()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_CLEAR_DDRT_DEFEATURE) (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 DdrtQosMode
  );

/**
  M2UPCIE:  Get R2 General Control Reg value.

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param R2pGnCtrl     - R2 General Control

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieGetR2GenCtrl (
  IN  UINT8   SocId,
  IN  UINT8   CtrId,
  OUT UINT32* R2pGnCtrl
  );
//
// Function pointer definiton for M2UPcieGetR2GenCtrl()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_GET_R2_GEN_CTRL) (
  IN  UINT8   SocId,
  IN  UINT8   CtrId,
  OUT UINT32* R2pGnCtrl
  );

/**
  M2UPCIE:  Set The R2 General Control Register.

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param R2GenCtrl     - The value R2GenCtrl

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieSetR2GenCtrl (
  IN UINT8  SocId,
  IN UINT8  CtrId,
  IN UINT32 R2GenCtrl
  );
//
// Function pointer definiton for M2UPcieSetR2GenCtrl()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_SET_R2_GEN_CTRL) (
  IN UINT8  SocId,
  IN UINT8  CtrId,
  IN UINT32 R2GenCtrl
  );

/**
  M2UPCIE:  Program Ubox route table (SKX Ubox is behind M2PCIe1)

  @param SocId           - Socket ID
  @param DSocId          - Socket ID destination
  @param LocalPort       - The local port to route
  @param PortFieldWidth  - The local port width

  @retval EFI_STATUS     - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieSetUboxRouteTable (
  IN UINT8  SocId,
  IN UINT8  DSocId,
  IN UINT32 LocalPort,
  IN UINT32 PortFieldWidth
  );
//
// Function pointer definiton for M2UPcieSetUboxRouteTable()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_SET_UBOX_ROUTE_TABLE) (
  IN UINT8  SocId,
  IN UINT8  DSocId,
  IN UINT32 LocalPort,
  IN UINT32 PortFieldWidth
  );

/**
  M2UPCIE:  Set the PRQ Credit Error Mask for each CHA

  @param SocId           - Socket ID
  @param CtrId           - Controller ID
  @param Mask            - mask when true, un-mask when false

  @retval EFI_STATUS     - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieSetR2CrdErrMask (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN BOOLEAN Mask
  );
//
// Function pointer definiton for M2UPcieSetR2CrdErrMask()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_SET_R2_CRD_ERR_MASK) (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN BOOLEAN Mask
  );

/**
  M2UPCIE:  Set the PRQ initial credits  for each CHA

  @param SocId           - Socket ID
  @param CtrId           - Controller ID
  @param TotCha          - Total number of CHA
  @param PrqPerIoStack   - PRQs per IO Stack

  @retval EFI_STATUS     - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieSetR2InitialCrd (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 TotCha,
  IN UINT8 PrqPerIoStack
  );
//
// Function pointer definiton for M2UPcieSetR2InitialCrd()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_SET_R2_INITIAL_CRD) (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 TotCha,
  IN UINT8 PrqPerIoStack
  );

/**
  M2UPCIE:  Get the Intial value of shared credits

  @param SocId           - Socket ID
  @param CtrId           - Controller ID
  @param Cred0           - Shared credit 0
  @param Cred1           - Shared credit 1
  @param Cred2           - Shared credit 2

  @retval EFI_STATUS     - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieGetR2InitShrCrd (
  IN  UINT8 SocId,
  IN  UINT8 CtrId,
  OUT UINT32 *Cred0,
  OUT UINT32 *Cred1,
  OUT UINT32 *Cred2
  );
//
// Function pointer definiton for M2UPcieGetR2InitShrCrd()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_GET_R2_INIT_SHR_CRD) (
  IN  UINT8 SocId,
  IN  UINT8 CtrId,
  OUT UINT32 *Cred0,
  OUT UINT32 *Cred1,
  OUT UINT32 *Cred2
  );

/**
  M2UPCIE:  Set the Intial value of shared credits

  @param SocId           - Socket ID
  @param CtrId           - Controller ID
  @param Cred0           - Shared credit 0
  @param Cred1           - Shared credit 1
  @param Cred2           - Shared credit 2

  @retval EFI_STATUS     - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieSetR2InitShrCrd (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT32 Cred0,
  IN UINT32 Cred1,
  IN UINT32 Cred2
  );
//
// Function pointer definiton for M2UPcieSetR2InitShrCrd()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_SET_R2_INIT_SHR_CRD) (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT32 Cred0,
  IN UINT32 Cred1,
  IN UINT32 Cred2
  );

/**
M2UPCIE:  Program the M2IOSF to M2IOSF P2P credit ring register bitfields

@param Soc             - Socket ID
@param M2iosfBitMap    - bit map that lists all M2IOSF mesh agents present on this Socket

@retval EFI_STATUS     - EFI_SUCCESS: success

**/
EFI_STATUS
EFIAPI
M2UPcieProgramM2iosfToM2iosfP2pCredits (
  IN UINT8  Soc,
  IN UINT16 M2iosfBitMap
  );
//
// Function pointer definiton for M2UPcieProgramM2iosfToM2iosfP2pCredits()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_PROGRAM_M2IOSF_P2P_CREDITS) (
  IN UINT8  Soc,
  IN UINT16 M2iosfBitMap
  );

/**
  M2UPCIE:  Set the initial credits for iio2ubox

  @param SocId           - Socket ID
  @param CtrId           - Controller ID


  @retval EFI_STATUS     - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieSetIio2UboxInitCrd (
  IN UINT8 SocId,
  IN UINT8 CtrId
  );
//
// Function pointer definiton for M2UPcieSetIio2UboxInitCrd()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_SET_IIO2UBOX_INIT_CRD) (
  IN UINT8 SocId,
  IN UINT8 CtrId
  );

/**
  M2UPCIE:  Set the PRQ initial credits usiing and index

  @param SocId           - Socket ID
  @param CtrId           - Controller ID
  @param RegIdx          - Table index for the register
  @param Credit          - Credit value

  @retval EFI_STATUS     - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieSetPrqInitCrdIdx (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 RegIdx,
  IN UINT32 Credit
  );
//
// Function pointer definiton for M2UPcieSetPrqInitCrdIdx()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_SET_PRQ_INIT_CRD_IDX) (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 RegIdx,
  IN UINT32 Credit
  );

/**
  M2UPCIE:  Set the initial credits for NDR/NCS to KTI

  @param SocId           - Socket ID
  @param CtrId           - Controller ID
  @param Vn0Ad           - VN0 AD credit value
  @param BlNcs           - BL NCS credit value

  @retval EFI_STATUS     - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieSetNdrNcs2KtiInitCrd (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT32 Vn0Ad,
  IN UINT32 BlNcs
  );
//
// Function pointer definiton for M2UPcieSetNdrNcs2KtiInitCrd()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_SET_NDR_NCS2KTI_INIT_CRD) (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT32 Vn0Ad,
  IN UINT32 BlNcs
  );

/**
  M2UPCIE:  Set the initial credits for NBC/NCS to Iio

  @param SocId           - Socket ID
  @param CtrId           - Controller ID
  @param Credit          - Credit value

  @retval EFI_STATUS     - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieSetNcbNcs2IioInitCrd (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT32 Credit
  );
//
// Function pointer definiton for M2UPcieSetNcbNcs2IioInitCrd()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_SET_NCB_NCS2IIO_INIT_CRD) (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT32 Credit
  );

/**
  A routine for programming the QoS Performance settings.

  @param SocId           - Socket ID
  @param IioStack        - Stack Index
  @param DdrtQosMode     - DDRT QoS Mode

  @retval EFI_STATUS     - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieSetCrQosPerformance (
  IN UINT8 SocId,
  IN UINT8 IioStack,
  IN UINT8 DdrtQosMode
  );
//
// Function pointer definiton for M2UPcieSetCrQosPerformance()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_SET_CR_QOS_PERFORMANCE) (
  IN UINT8 SocId,
  IN UINT8 IioStack,
  IN UINT8 DdrtQosMode
  );

/**
  M2UPCIE:  Set the mesh credit update value

  @param SocId           - Socket ID
  @param CtrId           - Controller ID
  @param MeshCrdUpdate   - Mesh credit update value

  @retval EFI_STATUS     - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieSetMeshCrdUpdate (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 MeshCrdUpdate
  );
//
// Function pointer definiton for M2UPcieSetMeshCrdUpdate()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_SET_MESH_CRD_UPDATE) (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 MeshCrdUpdate
  );

/**
  M2UPCIE:  Set the QoS values

  @param SocId           - Socket ID
  @param CtrId           - Controller ID
  @param AepPresent      - Aep DIMM present flag

  @retval EFI_STATUS     - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieSetQos (
  IN UINT8   SocId,
  IN UINT8   CtrId,
  IN BOOLEAN AepPresent
  );
//
// Function pointer definiton for M2UPcieSetQos()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_SET_QOS) (
  IN UINT8   SocId,
  IN UINT8   CtrId,
  IN BOOLEAN AepPresent
  );

/**
  This is a wrapper function for calling the auto-generated routine that
  configures spare_defeature_bit field in M2UPCIe R2EGRSPARECSR register.

  @param[in] Socket                Socket ID
  @param[in] AepDimmPresent        TRUE indicates AEP DIMM presents in the system, FALSE otherwise.

  @retval EFI_SUCCESS       CBO Coherency register was setup based on the requested knob settings
  @retval EFI_NOT_READY     NULL KTI_HOST_OUT pointer

**/
EFI_STATUS
EFIAPI
M2UPcieSetQosPerf (
  IN  UINT8              Socket,
  IN  BOOLEAN            AepDimmPresent
  );
//
// Function pointer definiton for M2UPcieSetQosPerf()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_SET_QOS_PERF) (
  IN UINT8   SocId,
  IN UINT8   CtrId,
  IN BOOLEAN AepPresent
  );

/**
  M2UPCIE:  Set the NCS defeature bit, to override credit.

  @param SocId           - Socket ID
  @param CtrId           - Controller ID
  @param Defeature       - NCS defeature bit value

  @retval EFI_STATUS     - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieSetNcsDefeature (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 Defeature
  );
//
// Function pointer definiton for M2UPcieSetNcbNcs2IioInitCrd()
//
typedef
EFI_STATUS
(EFIAPI *M2UPCIE_SET_NCS_DEFEATURE) (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 Defeature
  );

typedef struct {
  //
  // Function pointers
  //
  M2UPCIE_DEVICE_EXISTS                  M2UPcieDeviceExists;
  M2UPCIE_UNMASK_GL_ERROR                M2UPcieUnmaskGlError;
  M2UPCIE_UNMASK_R2PIN_ERROR             M2UPcieUnmaskR2PinError;
  M2UPCIE_SET_UPI_CRD                    M2UPcieSetUpiCrd;
  M2UPCIE_SET_P2P_SHR_CRD                M2UPcieSetP2PShrCrd;
  M2UPCIE_SET_ROUTE_TABLE                M2UPcieSetRouteTable;
  M2UPCIE_R2_WRITEBACKE_EGRESS_ERR_LOG   M2UPcieR2WriteBackEgressErrLog;
  M2UPCIE_R2_WRITEBACK_INGRESS_ERR_LOG   M2UPcieR2WriteBackIngressErrLog;
  M2UPCIE_R2_MASK_PAR_INGRESS_ERR        M2UPcieR2MaskParIngressErr;
  M2UPCIE_CLEAR_DDRT_DEFEATURE           M2UPcieClearDdrtDefeature;
  M2UPCIE_GET_R2_GEN_CTRL                M2UPcieGetR2GenCtrl;
  M2UPCIE_SET_R2_GEN_CTRL                M2UPcieSetR2GenCtrl;
  M2UPCIE_SET_UBOX_ROUTE_TABLE           M2UPcieSetUboxRouteTable;
  M2UPCIE_SET_R2_CRD_ERR_MASK            M2UPcieSetR2CrdErrMask;
  M2UPCIE_SET_R2_INITIAL_CRD             M2UPcieSetR2InitialCrd;
  M2UPCIE_GET_R2_INIT_SHR_CRD            M2UPcieGetR2InitShrCrd;
  M2UPCIE_SET_R2_INIT_SHR_CRD            M2UPcieSetR2InitShrCrd;
  M2UPCIE_PROGRAM_M2IOSF_P2P_CREDITS     M2UPcieProgramM2iosfToM2iosfP2pCredits;
  M2UPCIE_SET_IIO2UBOX_INIT_CRD          M2UPcieSetIio2UboxInitCrd;
  M2UPCIE_SET_PRQ_INIT_CRD_IDX           M2UPcieSetPrqInitCrdIdx;
  M2UPCIE_SET_NDR_NCS2KTI_INIT_CRD       M2UPcieSetNdrNcs2KtiInitCrd;
  M2UPCIE_SET_NCB_NCS2IIO_INIT_CRD       M2UPcieSetNcbNcs2IioInitCrd;
  M2UPCIE_SET_CR_QOS_PERFORMANCE         M2UPcieSetCrQosPerformance;
  M2UPCIE_SET_MESH_CRD_UPDATE            M2UPcieSetMeshCrdUpdate;
  M2UPCIE_SET_QOS                        M2UPcieSetQos;
  M2UPCIE_SET_QOS_PERF                   M2UPcieSetQosPerf;
  M2UPCIE_SET_NCS_DEFEATURE              M2UPcieSetNcsDefeature;
} M2UPCIE_IP_INTERFACE;

//
// This API supports multiple instances with the GUID identifying the instance
//
#define M2UPCIE_IP_INTERFACE_CPU_GUID \
  { 0x793a789f, 0x9764, 0x48fd, { 0xb8, 0xc9, 0xac, 0xdd, 0xfa, 0x5b, 0x4e, 0x26 } } // {793A789F-9764-48FD-B8C9-ACDDFA5B4E26}
extern EFI_GUID gM2UPcieIpInterfaceCpuGuid;

#define M2UPCIE_IP_INTERFACE_PCIE_GEN4_GUID \
  { 0x5032faf9, 0x6c56, 0x4941, { 0x94, 0x4d, 0x1f, 0xe7, 0x63, 0xe6, 0xab, 0x5f } } // {5032FAF9-6C56-4941-944D-1FE763E6AB5F}
extern EFI_GUID gM2UPcieIpInterfacePcieGen4Guid;
#endif //__M2UPCIE_IP_INTERFACE_H__
