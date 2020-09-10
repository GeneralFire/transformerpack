/** @file
  PCU generation specific silicon access interface

  This library abstracts silicon register differences between different versions
  of PCU IP blocks.
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
#ifndef __PCU_IP_INTERFACE_H__
#define __PCU_IP_INTERFACE_H__

#include <Uefi.h>
#include <Base.h>
#include <Library/UsraCsrLib.h>
#include <Upi/KtiSi.h>

#define SET_CPL_1     1
#define SET_CPL_2     2
#define SET_CPL_3     3
#define SET_CPL_4     4

#define EXSEGMENT_TYPE_BIT_FIELD_OFFSET     0x09
#define EXSEGMENT_TYPE_BIT_FIELD_MASK_10NM  0x0F

/**
  Get CAPID3 register value

  @param SocId        - Socket ID

  @retval             - The value of CAPID3 register

**/
UINT32
EFIAPI
PcuGetCapid3RegisterValue (
  UINT8               SocId
  );

/**
  PCU: Get the Physical Chop

  @param SocId        - Socket ID

  @retval             - The Physical chop

**/
UINT8
EFIAPI
PcuGetPhysicalChop (
  UINT8 SocId
  );

/**
  PCU: Get the Wayness

  @param SocId        - Socket ID

  @retval             - The wayness

**/
UINT8
EFIAPI
PcuGetWayness (
  UINT8 SocId
  );

/**
  PCU: Get the Segment type

  @param SocId        - Socket ID

  @retval             - The segment type

**/
UINT8
EFIAPI
PcuGetSegType (
  UINT8 SocId
  );

/**
  PCU: Check if the Socket supports S2S or S4S

  @param SocId        - Socket ID

  @retval             - TRUE if Socket supports S2S or S4S
                        FALSE otherwise

**/
BOOLEAN
EFIAPI
PcuIsScalable (
  UINT8 SocId
  );

/**
  PCU: Check if the CPU hot add is enabled

  @param SocId        - Socket ID

  @retval             - TRUE if CPU hot add is enabled
                        FALSE otherwise

**/
BOOLEAN
EFIAPI
PcuIsCpuHotAddEn (
  UINT8 SocId
  );

/**
  PCU: Set the Global NID MAP reg (0-3)

  @param SocId      - Socket ID
  @param GblNidMap  - Global Node ID

  @retval           - NONE
**/
VOID
EFIAPI
PcuSetGblNid0to3Map (
  UINT8 SocId,
  UINT32 GblNidMap
  );

/**
  PCU: Set the Global NID MAP reg (4-7)

  @param SocId      - Socket ID
  @param GblNidMap  - Global Node ID Map

  @retval           - NONE
**/
VOID
EFIAPI
PcuSetGblNid4to7Map (
  UINT8 SocId,
  UINT32 GblNidMap
  );

/**
  PCU:  Get UNC TSC value
  @param SocId         - Socket ID
  @param TscSnapShotLo - UNC TSC Snapshot low part
  @param TscSnapShotHi - UNC TSC Snapshot high part

  @retval              - None

**/
VOID
EFIAPI
PcuGetUncTsc (
  UINT8  SocId,
  UINT32 *TscSnapShotLo,
  UINT32 *TscSnapShotHi
  );

/**
  PCU:  Set TSC offset value
  @param SocId         - Socket ID
  @param TscDeltaLo -  TSC Delta low part
  @param TscDeltaLo -  TSC Delta high part

  @retval              - None

**/
VOID
EFIAPI
PcuSetTscOffset (
  UINT8  SocId,
  UINT32 TscDeltaLo,
  UINT32 TscDeltaHi
  );

/**
  PCU: Get disabled freqs.

  @param SocId        - Socket ID

  @retval             - OR of disabled freqs.

**/
UINT32
EFIAPI
PcuGetUpiFreqDisable (
  UINT8 SocId
  );

/**
  PCU:  Get the LLC Slice enabled field
  @param SocId     - Socket ID
  @param SliceList - The array with the list of slices

  @retval          - None

**/
VOID
EFIAPI
PcuGetLlcSlice (
  UINT8  SocId,
  UINT32 SliceMap[MAX_CHA_MAP]
  );

/**
  PCU: Get RAS Type from CAP Registers

  @param  SocId             - Socket ID

  @retval                   - The type of Ras

**/
UINT32
EFIAPI
PcuGetRasType (
  UINT8 SocId
  );

/**
  PCU: Get DEV Hide data

  @param[in]  SocId        Socket ID
  @param[in]  BusNum       Stack ID (0-based)
  @param[in]  FunctionNum  PCI function number
  @param[out] Data         Pointer to data type UINT32 for storing data returned
                           from CSR read

  @retval EFI_UNSUPPORTED - Either the function is not implemented, or stack ID
                            provided by the caller exceeds maximum bus number
                            supported by this function.
          EFI_SUCCESS     - The requested Devhide register data was returned
                            successfully.

**/
EFI_STATUS
EFIAPI
PcuGetDevHideData (
  IN  UINT8  SocId,
  IN  UINT8  BusNum,
  IN  UINT8  FunctionNum,
  OUT UINT32 *Data
  );

/**
  PCU: Set DEV Hide data using index and stack ID (0-based).

  @param[in]  SocId        Socket ID
  @param[in]  BusNum       Stack ID (0-based)
  @param[in]  TabIdx       Index into PcuDevhideRegs table. It is PCI function number.
  @param[in]  DevHideData  Data to be programmed into the requested devhide register

  @retval              - None

**/
VOID
EFIAPI
PcuSetDevHideDataIdx (
  IN UINT8  SocId,
  IN UINT8  BusNum,
  IN UINT8  TabIdx,
  IN UINT32 DevHideData
  );

/**
  PCU: Get the Ms2Iosf Stacks enabled value

  @param  SocId      - Socket ID

  @retval            - The Ms2Iosf Stacks enabled value

**/
UINT32
EFIAPI
PcuGetMs2IosfStacksEn (
  UINT8 SocId
  );

/**
  PCU:  Get the CPU Bus Number data
  @param SocId    - Socket ID
  @param CpuBus0  - CPU bus 0 data
  @param CpuBus1  - CPU bus 1 data

  @retval         - None

**/
VOID
EFIAPI
PcuSetCpuBusNo (
  UINT8 SocId,
  UINT32 CpuBus0,
  UINT32 CpuBus1
  );

/**
  PCU:  Get the status of pcode_init_done* bit in BIOS_RESET_CPL PCU register.

  This routine doesn't read BIOS_RESET_CPL register if the requested "Phase" is
  disabled in PcdDfxRstCplBitsEn, in this case, PcodeInitDone function parameter
  is returned as 0.

  @param[in] SocId               Socket ID
  @param[in] Phase               Accepted values are: SET_CPL_1, SET_CPL_2, SET_CPL_3, SET_CPL_4
  @param[out]PcodeInitDone       Returns the value of pcode_init_done* bit for the requested Phase.

  @retval EFI_ABORTED            The requested rst_cpl bit is disabled in PcdDfxRstCplBitsEn.
                                 Therefore programming is skipped.
  @retval EFI_INVALID_PARAMETER  Invalid value is passed into the function parameter "Phase".
  @retval EFI_SUCCESS            The value of the requested pcode_init_done bit was returned.

**/
EFI_STATUS
EFIAPI
PcuGetPcodeInitDone (
  IN  UINT8    SocId,
  IN  UINT8    Phase,
  OUT UINT8    *PcodeInitDone
  );

/**
  PCU:  Set rst_cpl* bit in BIOS_RESET_CPL PCU register.

  Programming of rst_cpl* bit is skipped if the corresponding bit is
  disabled in PcdDfxRstCplBitsEn.

  @param[in] SocId               Socket ID
  @param[in] Phase               Accepted values are: SET_CPL_1, SET_CPL_2, SET_CPL_3, SET_CPL_4

  @retval EFI_ABORTED            The requested rst_cpl bit is disabled in PcdDfxRstCplBitsEn.
                                 Therefore programming is skipped.
  @retval EFI_INVALID_PARAMETER  Invalid value is passed into the function parameter "Phase".
  @retval EFI_SUCCESS            The requested rst_cpl bit is programmed.

**/
EFI_STATUS
EFIAPI
PcuSetRstCpl (
  IN UINT8    SocId,
  IN UINT8    Phase
  );

/**
  PCU:  Set comp_disable bit in M_COMP_PCU_FUN1_STRUCT PCU register.

  @param[in] SocId               Socket ID
  @param[in] EnableMemoryComp    Value for comp_disable bit.
                                 TRUE - comp_disable will be set as zero.
                                 FALSE- comp_disable will be set as one.

  @retval EFI_SUCCESS            The comp_disable bit is programmed.

**/
EFI_STATUS
EFIAPI
PcuEnablePeriodicRcomp (
  IN UINT8    SocId,
  IN BOOLEAN  EnableMemoryComp
  );

/**
  PCU:  Set comp_interval bits in M_COMP_PCU_FUN1_STRUCT PCU register.
  The time is indicated by power(2,COMP_INTERVAL) * 10.24 usec.
  The default value of Dh corresponds to ~84 ms.

  @param[in] SocId       Socket ID.
  @param[in] Interval    Periodic Rcomp interval.

  @retval EFI_SUCCESS    The comp_interval bits is programmed.

**/
EFI_STATUS
EFIAPI
PcuSetPeriodicRcompInterval (
  IN UINT8    SocId,
  IN UINT8    Interval
  );

/**
  PCU:  Program NID and AM_I_MASTER fields in GLOBAL_PKG_C_S_CONTROL register.
  AM_I_MASTER field is only programmed if the Socket is SBSP.

  @param[in] SocId               Socket ID
  @param[in] SbspSocId           Node ID of SBSP Socket.

  @retval UINT32            The updated value of GLOBAL_PKG_C_S_CONTROL register.

**/
UINT32
EFIAPI
PcuSetNidAndAmIMaster (
  IN UINT8    SocId,
  IN UINT8    SbsbSocId
  );

/**
  PCU: Determine the Package based on the PCU Package capability bit

  @param SocId        - Socket ID

  @retval             - 1-based Package ID, 0 indicates not supported

**/
UINT8
EFIAPI
PcuGetPackageId (
  UINT8 SocId
  );

/**
  PCU: Get the number of die in the socket

  @param SocId        - Socket ID

  @retval             - The number of die in the socket

**/
UINT8
EFIAPI
PcuGetDieCount (
  UINT8 SocId
  );

/**

  Get bitmap of available max cores for the given CPU package

  @param Socket:  CPU socket ID

  @retval AvailCoresMask - UINT32  bitmap of available max cores for the CPU package

**/
UINT64
PcuGetAvailableCores (
  UINT8              SocketId
  );

/**

  PCU: Get bitmap of resolved cores for the given CPU package

  @param socket:  CPU socket ID

  @retval ResolvedCoreMask  UINT32     - bitmap of resolved cores

**/
UINT64
EFIAPI
PcuGetResolvedCores (
  UINT8 SocketId
  );

/**
  PCU: Get the Csr Desired Cores Data

  @param  Socket      - Socket ID

  @retval             - The desired cores data value

**/
UINT32
EFIAPI
PcuGetDesiredCoreSmtDis (
  UINT8 Socket
  );

/**
  PCU: Get Exsegment Type from CAP Registers

  @param  SocId             - Socket ID

  @retval                   - The type of Exsegment

**/
UINT8
EFIAPI
PcuGetExsegType (
  UINT8 SocId
  );

/**
  PCU: Set fADR related configurations.

  This function programs the PCU_BIOS_SPARE2 register to enable CPU & IO cache flush and set the ADR
  type and CPU & IO cache flush type for enabling the fADR support on 10nm wave3.

  @param[in] ScktId      Socket index.

  @retval N/A
**/
VOID
EFIAPI
PcuSetFadrConfig (
  IN UINT8    ScktId
  );

/**

  PCU: Get Hbm Enabled Io Number, 0 means not HBM SKU.

  This function checks CAPID to get HBM Enabled Io Number.

  @param[in] ScktId      Socket index.

  @retval 0      - Hbm Enabled Io Number, It is not HBM SKU
          others - Hbm Enabled Io Number, it is HBM SKU

**/
UINT8
EFIAPI
PcuGetHbmEnabledIoNum (
  IN UINT8    ScktId
  );

#endif // __PCU_IP_INTERFACE_H__
