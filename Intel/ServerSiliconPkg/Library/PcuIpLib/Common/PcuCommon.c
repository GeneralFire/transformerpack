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
#include <RcRegs.h>
#include <Library/DebugLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/PcuIpLib.h>

/**
  PCU: Get the Physical Chop

  @param SocId        - Socket ID

  @retval             - The Physical chop

**/
UINT8
EFIAPI
PcuGetPhysicalChop (
  UINT8 SocId
  )
{
  CAPID4_PCU_FUN3_STRUCT Capid4;
  Capid4.Data = UsraCsrRead (SocId, 0, CAPID4_PCU_FUN3_REG);
  return (UINT8) Capid4.Bits.physical_chop;
}

/**
  PCU: Get the Wayness

  @param SocId        - Socket ID

  @retval             - The wayness

**/
UINT8
EFIAPI
PcuGetWayness (
  UINT8 SocId
  )
{
  CAPID0_PCU_FUN3_STRUCT  Capid0;
  Capid0.Data = UsraCsrRead (SocId, 0, CAPID0_PCU_FUN3_REG);
  return (UINT8)Capid0.Bits.wayness;
}

/**
  PCU: Get the Segment type

  @param SocId        - Socket ID

  @retval             - The segment type

**/
UINT8
EFIAPI
PcuGetSegType (
  UINT8 SocId
  )
{
  CAPID0_PCU_FUN3_STRUCT  Capid0;
  Capid0.Data = UsraCsrRead (SocId, 0, CAPID0_PCU_FUN3_REG);
  return (UINT8)Capid0.Bits.segment;
}

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
  )
{
  CAPID0_PCU_FUN3_STRUCT  Capid0;
  Capid0.Data = UsraCsrRead (SocId, 0, CAPID0_PCU_FUN3_REG);
  return (Capid0.Bits.scalable == 1);
}

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
  )
{
  CAPID1_PCU_FUN3_STRUCT  Capid1;
  Capid1.Data = UsraCsrRead (SocId, 0, CAPID1_PCU_FUN3_REG);
  return (Capid1.Bits.cpu_hot_add_en == 1);
}

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
  )
{
  UsraCsrWrite (SocId, 0, GLOBAL_NID_SOCKET_0_TO_3_MAP_PCU_FUN2_REG, GblNidMap);
}

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
  )
{
  UsraCsrWrite (SocId, 0, GLOBAL_NID_SOCKET_4_TO_7_MAP_PCU_FUN4_REG, GblNidMap);
}

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
  )
{
  if (TscSnapShotLo == NULL || TscSnapShotHi == NULL) {
    DEBUG ((DEBUG_ERROR, "ERROR!! Null Pointer detected\n"));
    ASSERT (FALSE);
    return;
  }

  *TscSnapShotLo = UsraCsrRead (SocId, 0, UNC_TSC_SNAPSHOT_N0_PCU_FUN4_REG);
  *TscSnapShotHi = UsraCsrRead (SocId, 0, UNC_TSC_SNAPSHOT_N1_PCU_FUN4_REG);
}

/**
  PCU:  Get TSC offset value
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
  )
{
  TSC_HP_OFFSET_N0_PCU_FUN4_STRUCT TscHpOffset0;
  TSC_HP_OFFSET_N1_PCU_FUN4_STRUCT TscHpOffset1;

  TscHpOffset0.Data = TscDeltaLo & 0xFFFFFFFF;
  UsraCsrWrite (SocId, 0, TSC_HP_OFFSET_N0_PCU_FUN4_REG, TscHpOffset0.Data);
  TscHpOffset1.Bits.tsc_offset = TscDeltaHi & 0x7FFFFFFF;
  TscHpOffset1.Bits.tsc_update = 1;
  UsraCsrWrite (SocId, 0, TSC_HP_OFFSET_N1_PCU_FUN4_REG, TscHpOffset1.Data);
}

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
  )
{
  BIOS_RESET_CPL_PCU_FUN1_STRUCT  BiosResetCpl;

  if ((PcdGet8 (PcdDfxRstCplBitsEn) & (1 << (Phase - 1))) == 0) {
    return EFI_ABORTED;
  }

  BiosResetCpl.Data = UsraCsrRead (SocId, 0, BIOS_RESET_CPL_PCU_FUN1_REG);

  switch (Phase) {

    case SET_CPL_1:
      //
      // Set CPL1: NID MAP, package C-state and S-state BIOS configurations are completed.
      //
      BiosResetCpl.Bits.rst_cpl1 = 1;
      break;

    case SET_CPL_2:
      //
      // Set CPL2: Ready for Periodic RCOMP Starts
      //
      BiosResetCpl.Bits.rst_cpl2 = 1;
      break;

    case SET_CPL_3:
      //
      // Set CPL3: BIOS Power and Thermal Management Configure Completed
      //
      BiosResetCpl.Bits.rst_cpl3 = 1;
      break;

    case SET_CPL_4:
      //
      // Set CPL4: Memory Calibration Completed
      //
      BiosResetCpl.Bits.rst_cpl4 = 1;
      break;

    default:
      return EFI_INVALID_PARAMETER;
  }

  UsraCsrWrite (SocId, 0, BIOS_RESET_CPL_PCU_FUN1_REG, BiosResetCpl.Data);

  return EFI_SUCCESS;
}

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
  )
{
  M_COMP_PCU_FUN1_STRUCT MemComp;

  MemComp.Data = UsraCsrRead (SocId, 0, M_COMP_PCU_FUN1_REG);

  if (EnableMemoryComp == FALSE) {
    MemComp.Bits.comp_disable = 1; // Disable periodic Rcomp.
  } else {
    MemComp.Bits.comp_disable = 0; // Enable periodic Rcomp.
  }
  UsraCsrWrite (SocId, 0, M_COMP_PCU_FUN1_REG, MemComp.Data);

  return EFI_SUCCESS;
}

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
  )
{

  M_COMP_PCU_FUN1_STRUCT MemComp;

  MemComp.Data = UsraCsrRead (SocId, 0, M_COMP_PCU_FUN1_REG);

  MemComp.Bits.comp_interval = Interval;

  UsraCsrWrite (SocId, 0, M_COMP_PCU_FUN1_REG, MemComp.Data);

  return EFI_SUCCESS;
}

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
  )
{
  BIOS_RESET_CPL_PCU_FUN1_STRUCT  BiosResetCpl;

  *PcodeInitDone = 0;

  if ((PcdGet8 (PcdDfxRstCplBitsEn) & (1 << (Phase - 1))) == 0) {
    return EFI_ABORTED;
  }

  BiosResetCpl.Data = UsraCsrRead (SocId, 0, BIOS_RESET_CPL_PCU_FUN1_REG);
  switch (Phase) {

  case SET_CPL_1:
    *PcodeInitDone = (UINT8) BiosResetCpl.Bits.pcode_init_done1;
    break;

  case SET_CPL_2:
    *PcodeInitDone = (UINT8) BiosResetCpl.Bits.pcode_init_done2;
    break;

  case SET_CPL_3:
    *PcodeInitDone = (UINT8) BiosResetCpl.Bits.pcode_init_done3;
    break;

  case SET_CPL_4:
    *PcodeInitDone = (UINT8) BiosResetCpl.Bits.pcode_init_done4;
    break;

  default:
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

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
  IN UINT8    SbspSocId
  )
{
  GLOBAL_PKG_C_S_CONTROL_REGISTER_PCU_FUN2_STRUCT    CsrReg;

  CsrReg.Data = UsraCsrRead (SocId, 0, GLOBAL_PKG_C_S_CONTROL_REGISTER_PCU_FUN2_REG);

  CsrReg.Bits.my_nid = SocId;
  CsrReg.Bits.master_nid = SbspSocId;

  if (SocId == SbspSocId) {
    CsrReg.Bits.am_i_master = 1;
  }

  UsraCsrWrite (SocId, 0, GLOBAL_PKG_C_S_CONTROL_REGISTER_PCU_FUN2_REG, CsrReg.Data);
  return CsrReg.Data;
}

/**
  Get CAPID3 register value

  @param SocId        - Socket ID

  @retval             - The value of CAPID3 register

**/
UINT32
EFIAPI
PcuGetCapid3RegisterValue (
  UINT8               SocId
  )
{
  UINT32              Value;

  Value = UsraCsrRead (SocId, 0, CAPID3_PCU_FUN3_REG);
  return Value;
}
