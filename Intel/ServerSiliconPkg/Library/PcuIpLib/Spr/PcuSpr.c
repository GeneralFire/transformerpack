/** @file

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

#include <ReferenceCodeDataTypes.h>
#include <RcRegs.h>
#include <Library/DebugLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/PcuIpLib.h>
#include <Library/CpuAndRevisionLib.h>

//
// PCU_BIOS_SPARE2 Register Configurations for fADR Support
//
#define ADR_TYPE_PCODE_BASED                  BIT18
#define ADR_CPU_CACHE_FLUSH_ENABLED           BIT22
#define ADR_IO_CACHE_FLUSH_ENABLED            BIT25
#define ADR_CPU_CACHE_FLUSH_TYPE_FADR_BASED   (~(BIT23 | BIT24))

/**
  PCU: Get disabled freqs.

  @param SocId        - Socket ID

  @retval             - OR of disabled freqs.

**/
UINT32
EFIAPI
PcuGetUpiFreqDisable (
  UINT8 SocId
  )
{
  CAPID2_PCU_FUN3_STRUCT Capid2;
  Capid2.Data = UsraCsrRead (SocId, 0, CAPID2_PCU_FUN3_REG);
  // bit30: 14.4GT/s; bit31: 16GT/s
  return (Capid2.Data >> 30);
}

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
  )
{
  CAPID6_PCU_FUN3_STRUCT  Capid6;
  CAPID7_PCU_FUN3_STRUCT  Capid7;

  Capid6.Data = UsraCsrRead (SocId, 0, CAPID6_PCU_FUN3_REG);
  Capid7.Data = UsraCsrRead (SocId, 0, CAPID7_PCU_FUN3_REG);

  SliceMap[0] = Capid6.Bits.llc_slice_en_low;
  SliceMap[1] = Capid7.Bits.llc_slice_en_high;
}

/**
  PCU: Get RAS Type from CAP Registers

  @param  SocId             - Socket ID

  @retval                   - The type of Ras

**/
UINT32
EFIAPI
PcuGetRasType (
  UINT8 SocId
  )
{
  UINT32 RasType = 0;
  CAPID2_PCU_FUN3_STRUCT  Capid2;

  Capid2.Data = UsraCsrRead (SocId, 0, CAPID2_PCU_FUN3_REG);

  /**
  CAPID2_[4:0] indicate RAS levels: 5'b00000,
                               HEDT; 5'b00001, Std; 5'b00010, HPC; 5'b00011,
                               Adv; 5'b00101, 1SWS; 5'b00111, FPGA; 5'b01000,
                               Cirrus 5'b01001, Core;
  **/

  RasType = Capid2.Bits.ras_levels;

  return RasType;
}

/**
  PCU: Get the Ms2Iosf Stacks enabled value

  @param  SocId      - Socket ID

  @retval            - The Ms2Iosf Stacks enabled value

**/
UINT32
EFIAPI
PcuGetMs2IosfStacksEn (
  UINT8 SocId
  )
{
  CAPID10_PCU_FUN3_STRUCT Capid10;

  Capid10.Data = UsraCsrRead (SocId, 0, CAPID10_PCU_FUN3_REG);
  return Capid10.Data;
}

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
  )
{
  //
  // Empty function to keep API consistency
  //
  return;
}

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
  )
{
  //
  // Empty function to keep API consistency
  //
  return 0;
}

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
  )
{
  //
  // Empty function to keep API consistency
  //
  return;
}

/**
  PCU: Check if the Socket has RCD or not

  @param SocId        - Socket ID

  @retval             - FALSE because it doesn't support
                      - FALSE otherwise

**/
BOOLEAN
EFIAPI
PcuIsRCDPresent (
  UINT8 SocId
  )
{
  return FALSE;
}

/**

  Get bitmap of available max cores for the given CPU package

  @param Socket:  CPU socket ID

  @retval AvailCoresMask - UINT32  bitmap of available max cores for the CPU package

**/
UINT64
PcuGetAvailableCores (
  UINT8              SocketId
  )
{
  UINT64_DATA                          AvailCoresMask;

  FUSED_CORES_LOW_N0_PCU_FUN3_STRUCT   SprFusedCoresMaskLow;
  FUSED_CORES_LOW_N1_PCU_FUN3_STRUCT   SprFusedCoresMaskHi;

  AvailCoresMask.Data = 0;

  SprFusedCoresMaskLow.Data = UsraCsrRead (SocketId, 0, FUSED_CORES_LOW_N0_PCU_FUN3_REG);
  SprFusedCoresMaskHi.Data =  UsraCsrRead (SocketId, 0, FUSED_CORES_LOW_N1_PCU_FUN3_REG);

  AvailCoresMask.Data32.High = (UINT32) SprFusedCoresMaskHi.Bits.fused_core_mask_1;
  AvailCoresMask.Data32.Low = (UINT32) SprFusedCoresMaskLow.Bits.fused_core_mask_0;

  return AvailCoresMask.Data;
}

/**

  Get bitmap of resolved cores for the given CPU package

  @param SocketId:  CPU socket ID

  @retval ResolvedCoreMask  UINT32     - bitmap of resolved cores

**/
UINT64
PcuGetResolvedCores (
  UINT8               SocketId
  )
{
  UINT64_DATA                        ResolvedCoreMask;
  RESOLVED_CORES_N0_PCU_FUN6_STRUCT  CsrResovedCoresMaskLow;
  RESOLVED_CORES_N1_PCU_FUN6_STRUCT  CsrResovedCoresMaskHi;

  ResolvedCoreMask.Data = 0;

  CsrResovedCoresMaskLow.Data = UsraCsrRead (SocketId, 0, RESOLVED_CORES_N0_PCU_FUN6_REG);
  CsrResovedCoresMaskHi.Data = UsraCsrRead (SocketId, 0, RESOLVED_CORES_N1_PCU_FUN6_REG);
  ResolvedCoreMask.Data32.High = (UINT32)CsrResovedCoresMaskHi.Bits.core_mask_1;
  ResolvedCoreMask.Data32.Low = (UINT32)CsrResovedCoresMaskLow.Bits.core_mask_0;
  return ResolvedCoreMask.Data;
}


/**
  PCU: Get the Csr Desired Cores Data

  @param  Socket      - Socket ID

  @retval             - The desired cores data value

**/
UINT32
EFIAPI
PcuGetDesiredCoreSmtDis (
  UINT8 Socket
  )
{
  CSR_DESIRED_CORES_PCU_FUN1_STRUCT  DesiredCores;

  DesiredCores.Data = UsraCsrRead (Socket, 0, CSR_DESIRED_CORES_PCU_FUN1_REG);
  return DesiredCores.Bits.smt_disable;
}

/**
  PCU: Determine the Package based on the PCU Package capability bit

  @param SocId        - Socket ID

  @retval             - 1-based Package ID, 0 indicates not supported

**/
UINT8
EFIAPI
PcuGetPackageId (
  UINT8 SocId
  )
{
  return 0;
}

/**
  PCU: Get the number of die in the socket

  @param SocId        - Socket ID

  @retval             - The number of die in the socket

**/
UINT8
EFIAPI
PcuGetDieCount (
  UINT8 SocId
  )
{
  CAPID3_PCU_FUN3_STRUCT  Capid3;
  UINT8  DieMode;
  UINT8  DieCountInSoc;

  DieCountInSoc = 0;
  Capid3.Data = UsraCsrRead (SocId, 0, CAPID3_PCU_FUN3_REG);
  //
  //  Definition in SPR CAPID3.Bit[7:6]
  //  00: All dies (in UCC, 2c-4d); 01: 1 Die Mode; 10: 2 Die mode (in UCC, 2c-2d)
  //
  DieMode = (UINT8)((Capid3.Bits.capid3_7 << 1) + Capid3.Bits.capid3_6);
  if (DieMode == 0) {
    //
    // Currently only XCC is defined as 4Die configuration, and there is no 2Die configuration defined
    // Return 4 for All Dies mode now
    // In SPR UCC, all dies mode means 2c-4d (2 clusters - 4 dies)
    //
    if (IsCpuAndRevision (CPU_SPRSP, REV_UX)) {
      //
      // UCC case, 2 Dies per socket in 2cl-4d
      //
      DieCountInSoc = 2;
    } else {
      DieCountInSoc = 4;
    }
  } else if (DieMode == 1) {
    //
    // return as 1 Die
    //
    DieCountInSoc = 1;
  } else if (DieMode == 2) {
    //
    // in UCC, it indicates 2cl-2d (2 clusters - 2 dies)
    //
    if (IsCpuAndRevision (CPU_SPRSP, REV_UX)) {
      //
      // UCC case, 1 Dies per socket in 2cl-2d
      //
      DieCountInSoc = 1;
    }
  }
  //
  // Return the real Die Count.
  //
  return DieCountInSoc;
}

/**
  PCU: Get Exsegment Type from CAP Registers

  @param  SocId             - Socket ID

  @retval                   - The type of Exsegment

**/
UINT8
EFIAPI
PcuGetExsegType (
  UINT8 SocId
  )
{
  UINT8 ExsegmentType = 0;
  CAPID5_PCU_FUN3_STRUCT      Capid5;

  Capid5.Data = UsraCsrRead (SocId, 0, CAPID5_PCU_FUN3_REG);

  /*
  When CAPID0_5:3 is 3'b111, CAPID5_12:9==4'b0010 indicate a Cicada 4S.
                             CAPID5_12:9==4'b0001 indicate a Server D.
                             CAPID5_12:9==4'b0100 indicate a DLC.
                             CAPID5_12:9==4'b1100 indicate an AP.
  When CAPID0_5:3 is 3'b110, CAPID5_12:9==4'b0000 indicate a SNR-BTS.
                             CAPID5_12:9==4'b1000 indicate a SNR-NSX.
                             CAPID5_12:9==4'b1001 indicate a SNR-NIC.
  */
  ExsegmentType = (UINT8)(Capid5.Bits.capid5_9 >> EXSEGMENT_TYPE_BIT_FIELD_OFFSET) & EXSEGMENT_TYPE_BIT_FIELD_MASK_10NM;

  return ExsegmentType;
}

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
  )
{
  PCU_BIOS_SPARE2_PCU_FUN5_STRUCT  PcuBiosSpare2Reg;

  PcuBiosSpare2Reg.Data = UsraCsrRead (ScktId, 0, PCU_BIOS_SPARE2_PCU_FUN5_REG);

  PcuBiosSpare2Reg.Data |= ADR_TYPE_PCODE_BASED;
  PcuBiosSpare2Reg.Data |= ADR_IO_CACHE_FLUSH_ENABLED;
  PcuBiosSpare2Reg.Data |= ADR_CPU_CACHE_FLUSH_ENABLED;
  PcuBiosSpare2Reg.Data &= ADR_CPU_CACHE_FLUSH_TYPE_FADR_BASED;

  UsraCsrWrite (ScktId, 0, PCU_BIOS_SPARE2_PCU_FUN5_REG, PcuBiosSpare2Reg.Data);

  return;
}

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
  )
{
  CAPID3_PCU_FUN3_STRUCT  CapId3;
  UINT8                   HbmMaxIoSocket;

  CapId3.Data = UsraCsrRead (ScktId, 0, CAPID3_PCU_FUN3_REG);

  //
  // Find the highest order enabled IO to determine HbmMaxIoInst.
  //
  if (CapId3.Bits.capid3_30 == 1) {
    HbmMaxIoSocket = 4;
  } else if (CapId3.Bits.capid3_29 == 1) {
    HbmMaxIoSocket = 3;
  } else if (CapId3.Bits.capid3_28 == 1) {
    HbmMaxIoSocket = 2;
  } else if (CapId3.Bits.capid3_27 == 1) {
    HbmMaxIoSocket = 1;
  } else {
    HbmMaxIoSocket = 0;
  }

  return HbmMaxIoSocket;
}
