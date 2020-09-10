/** @file
  The PEI implementation of the ProcSmbIpLib for Spr.

  This implementation provides processor I3C services for use during PEI.
  Functions are implemented for use with Spr.  Reset, Initialize, Read, and
  Write functions are implemented as well as a variety of other misc
  functions which provide data from and/or interact with the processor
  I3C controller.  I3C initialization should be called before attempting
  to interact with the controller.

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

#include <Include/ProcSmbRegs.h>
#include <Include/ProcSmbEmulation.h>
#include <UncoreCommonIncludes.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemRcLib.h>
#include <Library/IoAccessLib.h>
#include <Library/TimerLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SemaphoreLib.h>
#include <ProcSmbChipCommonSpr.h>
#include <Library/CsrAccessLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/ProcSmbIpTargetLib.h>
#include <Library/HardwareSemaphoreTargetLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/ProcMemInitTargetLib.h>
#include <Library/MemoryCoreLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <CpuPciAccess.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/BaseMemoryLib.h>
#include <Memory/JedecDefinitions.h>
#include <Library/PerformanceTrackerLib.h>

/**
  Set output mode to show I3C CSR traces if enabled by user

  @param[in] PrintFlag                - Flag to print the trace log

  @retval Value of the current output mode
**/
UINT8
EnableI3cAsmOutput (
  IN  BOOLEAN  PrintFlag
  )
{
  UINT8       OriginalUbiosOutputMode;
  SYS_SETUP   *Setup;

  Setup = GetSysSetupPointer ();

  OriginalUbiosOutputMode = GetUbiosOutputMode ();

  if (Setup->mem.dfxMemSetup.DfxUbiosSmbusTraceEnable) {
    SetUbiosOutputMode (ASM_OUTPUT_ENABLE_ON_SMBUS);

    if (PrintFlag) {
      RcDebugPrint (SDBG_DEFAULT, "\n******* I3C CSR TRACE - START *******\n");
    }
  }

  return OriginalUbiosOutputMode;
}

/**
  Restore uBIOS output mode

  @param[in] OriginalUbiosOutputMode  - Output mode value to restore
  @param[in] PrintFlag                - Flag to print the trace log

  @retval N/A
**/
VOID
RestoreI3cAsmOutput (
  IN  UINT8    OriginalUbiosOutputMode,
  IN  BOOLEAN  PrintFlag
  )
{
  SYS_SETUP   *Setup;

  Setup = GetSysSetupPointer ();

  if (Setup->mem.dfxMemSetup.DfxUbiosSmbusTraceEnable) {
    SetUbiosOutputMode (OriginalUbiosOutputMode);

    if (PrintFlag) {
      RcDebugPrint (SDBG_DEFAULT, "\n******* I3C CSR TRACE - END *******\n");
    }
  }
}

/**
  Set I3C bus operation mode

  @param[in] Socket     - Socket number
  @param[in] Instance   - I3C Instance number
  @param[in] BusMode    - I3C bus operation mode

  @rval EFI_SUCCESS     - Successfully set I3C bus operation mode

**/
EFI_STATUS
EFIAPI
SetI3cBusMode (
  IN  UINT8  Socket,
  IN  UINT8  Instance,
  IN  UINT8  BusMode
  )
{
  PSYSHOST               Host;

  Host = GetSysHostPointer ();

  Host->var.mem.SmbMode[Socket][Instance] = BusMode;

  return EFI_SUCCESS;
}

/**
  Get slave device type - i2cni3c

  @param[in] Socket     - Socket number
  @param[in] Instance   - I3C Instance number

  @rval SlaveType   - slave device type

**/
UINT8
GetSlaveType (
  IN UINT8  Socket,
  IN UINT8  Instance
  )
{
  PSYSHOST               Host;

  Host = GetSysHostPointer ();

  //
  // Treat the slave as I3C device only when the dramType is DDR5 and switched to I3C mode
  //
  if ((Host->var.mem.SmbMode[Socket][Instance] == I3C_MODE) && (Host->nvram.mem.dramType == SPD_TYPE_DDR5)) {
    return I3C_DEVICE;
  } else {
    return I2C_DEVICE;
  }
}

/**
  Get I3C mode and speed - mode_speed

  @param[in] Socket     - Socket number
  @param[in] Instance   - I3C Instance number

  @rval ModeSpeed   - I3C mode and speed

**/
I3C_BUS_MODE
GetModeSpeed (
  IN UINT8  Socket,
  IN UINT8  Instance
  )
{
  SYS_SETUP         *Setup;
  I3C_BUS_MODE      ModeSpeed;

  Setup = GetSysSetupPointer ();

  //
  // I3C SDR3 and SDR4 are not supported as there is no corresponding I2C Speed.
  // I2C UDR  and UDR3 are reserved in I2C mode
  //
  switch (Setup->mem.SpdSmbSpeed) {
  case SmbClk1M:   // 1 Mhz in I2C mode; 10Mhz in I3C mode
    ModeSpeed = I3cSdr0I2cFm;
    break;
  case SmbClk700K: // 700 Khz in I2C mode; 8Mhz in I3C mode
    ModeSpeed = I3cSdr1I2cFmPlus;
  case SmbClk400K: // 400 Khz in I2C mode; 6Mhz in I3C mode
  case SmbClk100K: // 100 Khz in I2C mode; 4Mhz in I3C mode
    ModeSpeed = I3cSdr2I2cUd1;
    break;
  default:
    ModeSpeed = I3cSdr0I2cFm; // Default
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "Invalid SMBUS ClkFrequency Parameter: %x\n", Setup->mem.SpdSmbSpeed);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_85);
    break;
  }

  return ModeSpeed;
}

/**

  This routine sets the i2c_slave_present

  @param[in] Socket    - Socket Id
  @param[in] Instance  - SMBus Instance number
  @param[in] IsI2c     - Is I2c present or not

  @rval EFI_SUCCESS     - Successfully set I3C bus operation mode

**/
EFI_STATUS
EFIAPI
SetI2cSlavePresent (
  IN UINT8   Socket,
  IN UINT8   Instance,
  IN BOOLEAN IsI2c
  )
{
  UINT32                               I3cDevCtrlReg;
  DEVICE_CONTROL_SB_I3C0_STRUCT        I3cDevCtrl;

  if (Instance == 0) {
    I3cDevCtrlReg = DEVICE_CONTROL_SB_I3C0_REG;
  } else {
    I3cDevCtrlReg = DEVICE_CONTROL_SB_I3C1_REG;
  }

  I3cDevCtrl.Data = UsraCsrRead (Socket, Instance, I3cDevCtrlReg);
  I3cDevCtrl.Bits.i2c_slave_present = ((IsI2c == TRUE) ? 1 : 0);
  UsraCsrWrite (Socket, Instance, I3cDevCtrlReg, I3cDevCtrl.Data);

  return EFI_SUCCESS;
}

/**
  Program the SMBUS frequency to the specified value - program i3c timing registers with i2c/i3c timing values.

  @param[in] Socket       - Processor socket ID
  @param[in] Instance     - SMBUS Instance number
  @param[in] BusMode      - Bus mode

  @retval EFI_SUCCESS           - Clock Period programmed successfully
  @retval EFI_INVALID_PARAMETER - Invalid Socket
  @retval EFI_UNSUPPORTED       - Function not supported
**/
EFI_STATUS
EFIAPI
ProgramProcSmbFreq (
  IN UINT8                Socket,
  IN UINT8                Instance,
  IN UINT8                BusMode
  )
{
  UINT32                             I3cOdReg;
  UINT32                             I3cPpReg;
  UINT32                             I3cExtReg;
  UINT32                             I3cFmReg;
  UINT32                             I3cFmPlusReg;
  UINT32                             I3cSsReg;
  SCL_I3C_OD_TIMING_SB_I3C0_STRUCT   I3cOd;
  SCL_I3C_PP_TIMING_SB_I3C0_STRUCT   I3cPp;
  SCL_EXT_LCNT_TIMING_SB_I3C0_STRUCT I3cExt;
  SCL_I2C_FM_TIMING_SB_I3C0_STRUCT   I3cFm;
  SCL_I2C_FMP_TIMING_SB_I3C0_STRUCT  I3cFmPlus;
  SCL_I2C_SS_TIMING_SB_I3C0_STRUCT   I3cSs;
  SYS_SETUP                          *Setup;

  // Validate incoming parameters
  if (Socket >= MAX_SOCKET) {
    return EFI_INVALID_PARAMETER;
  }

  // Check for invalid Instance
  if (Instance >= MAX_SMB_INSTANCE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "Invalid SMBUS SmbInstance Parameter\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_84);
    return EFI_INVALID_PARAMETER;
  }

  Setup = GetSysSetupPointer ();

  //
  // Check the I3C mode and speed from setup
  //
  GetModeSpeed (Socket, Instance);

  if (Instance == 0) {
    I3cOdReg     = SCL_I3C_OD_TIMING_SB_I3C0_REG;
    I3cPpReg     = SCL_I3C_PP_TIMING_SB_I3C0_REG;
    I3cExtReg    = SCL_EXT_LCNT_TIMING_SB_I3C0_REG;
    I3cFmReg     = SCL_I2C_FM_TIMING_SB_I3C0_REG;
    I3cFmPlusReg = SCL_I2C_FMP_TIMING_SB_I3C0_REG;
    I3cSsReg     = SCL_I2C_SS_TIMING_SB_I3C0_REG;
  } else {
    I3cOdReg     = SCL_I3C_OD_TIMING_SB_I3C1_REG;
    I3cPpReg     = SCL_I3C_PP_TIMING_SB_I3C1_REG;
    I3cExtReg    = SCL_EXT_LCNT_TIMING_SB_I3C1_REG;
    I3cFmReg     = SCL_I2C_FM_TIMING_SB_I3C1_REG;
    I3cFmPlusReg = SCL_I2C_FMP_TIMING_SB_I3C1_REG;
    I3cSsReg     = SCL_I2C_SS_TIMING_SB_I3C1_REG;
  }

  //
  // SCL I3C Open Drain Timing - 1Mhz
  //
  I3cOd.Data = 0;
  I3cOd.Bits.i3c_od_lcnt = SMB_1M_CLK_PERIOD / 2;
  I3cOd.Bits.i3c_od_hcnt = SMB_1M_CLK_PERIOD / 2;
  UsraCsrWrite (Socket, Instance, I3cOdReg, I3cOd.Data);

  //
  // SCL I3C Push Pull Timing - 10Mhz
  // SCL I2C Fast Mode Timing - 1Mhz
  //
  I3cPp.Data = 0;
  if (BusMode == I3C_MODE) {
    I3cPp.Bits.i3c_pp_lcnt = SMB_10M_CLK_PERIOD / 2;
    I3cPp.Bits.i3c_pp_hcnt = SMB_10M_CLK_PERIOD / 2;
  } else {
    I3cPp.Bits.i3c_pp_lcnt = SMB_1M_CLK_PERIOD / 2;
    I3cPp.Bits.i3c_pp_hcnt = SMB_1M_CLK_PERIOD / 2;
  }
  UsraCsrWrite (Socket, Instance, I3cPpReg, I3cPp.Data);

  //
  // I3C SCL Extended Low Count Timing
  //
  I3cExt.Data = 0;
  if (BusMode == I3C_MODE) {
    I3cExt.Bits.i3c_ext_lcnt_1 = SMB_8M_CLK_LOW_COUNT;
    if (Setup->mem.SpdSmbSpeed == SmbClk400K) {
      I3cExt.Bits.i3c_ext_lcnt_2 = SMB_6M_CLK_LOW_COUNT;
    } else {
      I3cExt.Bits.i3c_ext_lcnt_2 = SMB_4M_CLK_LOW_COUNT;
    }
    I3cExt.Bits.i3c_ext_lcnt_3 = SMB_4M_CLK_LOW_COUNT;
    I3cExt.Bits.i3c_ext_lcnt_4 = SMB_2M_CLK_LOW_COUNT;
  } else {
    I3cExt.Bits.i3c_ext_lcnt_1 = SMB_700K_CLK_LOW_COUNT;  // SCL I2C Fast Mode Plus Timing - 700Khz
    if (Setup->mem.SpdSmbSpeed == SmbClk400K) {
      I3cExt.Bits.i3c_ext_lcnt_2 = SMB_400K_CLK_LOW_COUNT;  // SCL I2C Standard Speed Timing - 400Khz
    } else {
      I3cExt.Bits.i3c_ext_lcnt_2 = SMB_100K_CLK_LOW_COUNT;  // SCL I2C Standard Speed Timing - 100Khz
    }
    I3cExt.Bits.i3c_ext_lcnt_3 = SMB_100K_CLK_LOW_COUNT;
    I3cExt.Bits.i3c_ext_lcnt_4 = SMB_100K_CLK_LOW_COUNT;
  }
  UsraCsrWrite (Socket, Instance, I3cExtReg, I3cExt.Data);

  SetI2cSlavePresent (Socket, Instance, (BusMode == I2C_MODE));

  //
  // SCL I2C Fast Mode Timing - 1Mhz
  //
  I3cFm.Data = 0;
  I3cFm.Bits.i2c_fm_lcnt = SMB_1M_CLK_PERIOD / 2;
  I3cFm.Bits.i2c_fm_hcnt = SMB_1M_CLK_PERIOD / 2;
  UsraCsrWrite (Socket, Instance, I3cFmReg, I3cFm.Data);

  //
  // SCL I2C Fast Mode Plus Timing - 700Khz
  //
  I3cFmPlus.Data = 0;
  I3cFmPlus.Bits.i2c_fmp_lcnt = SMB_700K_CLK_PERIOD / 2;
  I3cFmPlus.Bits.i2c_fmp_hcnt = SMB_700K_CLK_PERIOD / 2;
  UsraCsrWrite (Socket, Instance, I3cFmPlusReg, I3cFmPlus.Data);

  //
  // SCL I2C Standard Speed Timing - 100Khz/400Khz
  //
  I3cSs.Data = 0;
  if (Setup->mem.SpdSmbSpeed == SmbClk400K) {
    I3cSs.Bits.i2c_ss_lcnt = SMB_400K_CLK_PERIOD / 2;
    I3cSs.Bits.i2c_ss_hcnt = SMB_400K_CLK_PERIOD / 2;
  } else {
    I3cSs.Bits.i2c_ss_lcnt = SMB_100K_CLK_PERIOD / 2;
    I3cSs.Bits.i2c_ss_hcnt = SMB_100K_CLK_PERIOD / 2;
  }
  UsraCsrWrite (Socket, Instance, I3cSsReg, I3cSs.Data);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "Bus Instance = %d SMB ClkFrequency = %d Bus mode = %d \n", Instance, Setup->mem.SpdSmbSpeed, BusMode);

  return EFI_SUCCESS;
}

/**
  Enable Read Address Pointer Mode for TSOD

  @param[in]  Socket      - Processor socket ID
  @param[in]  Ch          - Channel number
  @param[in]  Dimm        - Dimm number
  @param[in]  Dev         - Device address
  @param[in]  TempSensor  - Temperature sensor number

  @retval EFI_SUCCESS       Enable was successful
  @retval EFI_DEVICE_ERROR  Enable failed
  @retval EFI_UNSUPPORTED   Function not supported
**/
EFI_STATUS
EFIAPI
EnableTsodReadAddressPointerMode (
  IN  UINT8              Socket,
  IN  UINT8              Ch,
  IN  UINT8              Dimm,
  IN  SMB_DEVICE_STRUCT  Dev,
  IN  UINT8              TempSensor
  )
{
  EFI_STATUS                            Status;
  TS5_MR18_DEVICE_CONFIGURATION_STRUCT  TsodMr18;

  Status = ReadProcSmb (Dev, TS5_MR18_DEVICE_CONFIGURATION, &TsodMr18.Data);

  if (EFI_ERROR (Status)) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "TSOD %d failed to read MR18 0x%x\n", TempSensor, TsodMr18.Data);
  } else {

    TsodMr18.Bits.DEF_RD_ADDR_POINT_EN = 1;
    Status = WriteProcSmb (Dev, TS5_MR18_DEVICE_CONFIGURATION, &TsodMr18.Data);

    if (EFI_ERROR (Status)) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "TSOD %d failed to write MR18 0x%x to enable Read Address Pointer Mode\n", TempSensor, TsodMr18.Data);
    }
  }

  return Status;
}

/**
  Initalize Tsod Configuration.

  BIOS would
  1. Program the number of core clock periods to count for computing 1 millisecond through 'TICK_TIME_COUNT' register.
  2. Program the number of valid slots trough 'SLOT_RANGE' field on PERIODIC_POLL_CONTROL register.
  3. Program the number of ticks to count before scheduling the commands from the subsequent slot through 'SLOT_INTERVAL_COUNT' register.
  4. Setup the PPCT (Periodic Poll Command Table) which contains the polling commands.
  5. Enable TSOD polling for the desired temperature sensors (since I3C has the capability to disable/enable every temperature sensor)
  6. Setup the PPST (Periodic Poll Slot Table) to indicate the slot(s) in which the corresponding PPCT entry must be launched on the bus.
  7. Enable the Periodic Poll Scheduler by writing 1 to 'PERIODIC_POLL_EN' field on register PERIODIC_POLL_CONTROL

  @param[in] Socket         - Socket Id
  @param[in] Instance       - I3C Instance number

  @rval EFI_SUCCESS  Successfully initalized the TSOD
**/
EFI_STATUS
EFIAPI
InitalizeTsodConfig (
  IN UINT8   Socket,
  IN UINT8   Instance
  )
{
  EFI_STATUS                                     Status;
  SMB_DEVICE_STRUCT                              Dev;
  UINT8                                          Ch;
  UINT8                                          Dimm;
  UINT8                                          TempSensor;
  UINT8                                          TransactionID;
  UINT8                                          MaxChDdr;
  REGULAR_DATA_TRANSFER_COMMAND_LOW_WITHOUT_DAT  RegularCommandLow;
  REGULAR_DATA_TRANSFER_COMMAND_HIGH_WITHOUT_DAT RegularCommandHigh;
  UINT32                                         TsodCmdLowerReg;
  UINT32                                         TsodCmdUpperReg;
  UINT32                                         TsodSlotReg;
  PERIODIC_POLL_SLOT0_SB_I3C0_STRUCT             TsodSlot;
  UINT32                                         TsodCtrlReg;
  PERIODIC_POLL_CONTROL_SB_I3C0_STRUCT           TsodCtrl;
  UINT8  TsodDeviceTypeId[MAX_TS_PER_DIMM] = {DDR5_TS0, DDR5_TS1};
  UINT8  TsodIndexTable[I3C_CH_NUMBER][MAX_DIMM][MAX_TS_PER_DIMM] = {
    // MC 0 Channel 0
    {
      {PPCT_PPST_INDEX_DIMM0_TS0, PPCT_PPST_INDEX_DIMM0_TS1}, // DIMM 0
      {PPCT_PPST_INDEX_DIMM1_TS0, PPCT_PPST_INDEX_DIMM1_TS1}  // DIMM 1
    },
    // MC 0 Channel 1
    {
      {PPCT_PPST_INDEX_DIMM2_TS0, PPCT_PPST_INDEX_DIMM2_TS1}, // DIMM 0
      {PPCT_PPST_INDEX_DIMM3_TS0, PPCT_PPST_INDEX_DIMM3_TS1}  // DIMM 1
    },
  #if MAX_MC_CH > 1
    // MC 1 Channel 0
    {
      {PPCT_PPST_INDEX_DIMM4_TS0, PPCT_PPST_INDEX_DIMM4_TS1}, // DIMM 0
      {PPCT_PPST_INDEX_DIMM5_TS0, PPCT_PPST_INDEX_DIMM5_TS1}  // DIMM 1
    },
    // MC 1 Channel 1
    {
      {PPCT_PPST_INDEX_DIMM6_TS0, PPCT_PPST_INDEX_DIMM6_TS1}, // DIMM 0
      {PPCT_PPST_INDEX_DIMM7_TS0, PPCT_PPST_INDEX_DIMM7_TS1}  // DIMM 1
    }
  #endif
  };

  UINT32  I3c0TsodCmdTableLowerReg[I3C_MC_NUMBER * MAX_MC_CH * MAX_DIMM * MAX_TS_PER_DIMM] = {
    PERIODIC_POLL_CMD0_LOC1_SB_I3C0_REG,
    PERIODIC_POLL_CMD1_LOC1_SB_I3C0_REG,
    PERIODIC_POLL_CMD2_LOC1_SB_I3C0_REG,
    PERIODIC_POLL_CMD3_LOC1_SB_I3C0_REG,
    PERIODIC_POLL_CMD4_LOC1_SB_I3C0_REG,
    PERIODIC_POLL_CMD5_LOC1_SB_I3C0_REG,
    PERIODIC_POLL_CMD6_LOC1_SB_I3C0_REG,
    PERIODIC_POLL_CMD7_LOC1_SB_I3C0_REG,
  #if MAX_MC_CH > 1
    PERIODIC_POLL_CMD8_LOC1_SB_I3C0_REG,
    PERIODIC_POLL_CMD9_LOC1_SB_I3C0_REG,
    PERIODIC_POLL_CMD10_LOC1_SB_I3C0_REG,
    PERIODIC_POLL_CMD11_LOC1_SB_I3C0_REG,
    PERIODIC_POLL_CMD12_LOC1_SB_I3C0_REG,
    PERIODIC_POLL_CMD13_LOC1_SB_I3C0_REG,
    PERIODIC_POLL_CMD14_LOC1_SB_I3C0_REG,
    PERIODIC_POLL_CMD15_LOC1_SB_I3C0_REG
  #endif
  };

  UINT32  I3c0TsodCmdTableUpperReg[I3C_MC_NUMBER * MAX_MC_CH * MAX_DIMM * MAX_TS_PER_DIMM] = {
    PERIODIC_POLL_CMD0_LOC2_SB_I3C0_REG,
    PERIODIC_POLL_CMD1_LOC2_SB_I3C0_REG,
    PERIODIC_POLL_CMD2_LOC2_SB_I3C0_REG,
    PERIODIC_POLL_CMD3_LOC2_SB_I3C0_REG,
    PERIODIC_POLL_CMD4_LOC2_SB_I3C0_REG,
    PERIODIC_POLL_CMD5_LOC2_SB_I3C0_REG,
    PERIODIC_POLL_CMD6_LOC2_SB_I3C0_REG,
    PERIODIC_POLL_CMD7_LOC2_SB_I3C0_REG,
  #if MAX_MC_CH > 1
    PERIODIC_POLL_CMD8_LOC2_SB_I3C0_REG,
    PERIODIC_POLL_CMD9_LOC2_SB_I3C0_REG,
    PERIODIC_POLL_CMD10_LOC2_SB_I3C0_REG,
    PERIODIC_POLL_CMD11_LOC2_SB_I3C0_REG,
    PERIODIC_POLL_CMD12_LOC2_SB_I3C0_REG,
    PERIODIC_POLL_CMD13_LOC2_SB_I3C0_REG,
    PERIODIC_POLL_CMD14_LOC2_SB_I3C0_REG,
    PERIODIC_POLL_CMD15_LOC2_SB_I3C0_REG
  #endif
  };

  UINT32  I3c0TsodSlotTableReg[I3C_MC_NUMBER * MAX_MC_CH * MAX_DIMM * MAX_TS_PER_DIMM] = {
    PERIODIC_POLL_SLOT0_SB_I3C0_REG,
    PERIODIC_POLL_SLOT1_SB_I3C0_REG,
    PERIODIC_POLL_SLOT2_SB_I3C0_REG,
    PERIODIC_POLL_SLOT3_SB_I3C0_REG,
    PERIODIC_POLL_SLOT4_SB_I3C0_REG,
    PERIODIC_POLL_SLOT5_SB_I3C0_REG,
    PERIODIC_POLL_SLOT6_SB_I3C0_REG,
    PERIODIC_POLL_SLOT7_SB_I3C0_REG,
  #if MAX_MC_CH > 1
    PERIODIC_POLL_SLOT8_SB_I3C0_REG,
    PERIODIC_POLL_SLOT9_SB_I3C0_REG,
    PERIODIC_POLL_SLOT10_SB_I3C0_REG,
    PERIODIC_POLL_SLOT11_SB_I3C0_REG,
    PERIODIC_POLL_SLOT12_SB_I3C0_REG,
    PERIODIC_POLL_SLOT13_SB_I3C0_REG,
    PERIODIC_POLL_SLOT14_SB_I3C0_REG,
    PERIODIC_POLL_SLOT15_SB_I3C0_REG
  #endif
  };

  UINT32  I3c1TsodCmdTableLowerReg[I3C_MC_NUMBER * MAX_MC_CH * MAX_DIMM * MAX_TS_PER_DIMM] = {
    PERIODIC_POLL_CMD0_LOC1_SB_I3C1_REG,
    PERIODIC_POLL_CMD1_LOC1_SB_I3C1_REG,
    PERIODIC_POLL_CMD2_LOC1_SB_I3C1_REG,
    PERIODIC_POLL_CMD3_LOC1_SB_I3C1_REG,
    PERIODIC_POLL_CMD4_LOC1_SB_I3C1_REG,
    PERIODIC_POLL_CMD5_LOC1_SB_I3C1_REG,
    PERIODIC_POLL_CMD6_LOC1_SB_I3C1_REG,
    PERIODIC_POLL_CMD7_LOC1_SB_I3C1_REG,
  #if MAX_MC_CH > 1
    PERIODIC_POLL_CMD8_LOC1_SB_I3C1_REG,
    PERIODIC_POLL_CMD9_LOC1_SB_I3C1_REG,
    PERIODIC_POLL_CMD10_LOC1_SB_I3C1_REG,
    PERIODIC_POLL_CMD11_LOC1_SB_I3C1_REG,
    PERIODIC_POLL_CMD12_LOC1_SB_I3C1_REG,
    PERIODIC_POLL_CMD13_LOC1_SB_I3C1_REG,
    PERIODIC_POLL_CMD14_LOC1_SB_I3C1_REG,
    PERIODIC_POLL_CMD15_LOC1_SB_I3C1_REG
  #endif
  };

  UINT32  I3c1TsodCmdTableUpperReg[I3C_MC_NUMBER * MAX_MC_CH * MAX_DIMM * MAX_TS_PER_DIMM] = {
    PERIODIC_POLL_CMD0_LOC2_SB_I3C1_REG,
    PERIODIC_POLL_CMD1_LOC2_SB_I3C1_REG,
    PERIODIC_POLL_CMD2_LOC2_SB_I3C1_REG,
    PERIODIC_POLL_CMD3_LOC2_SB_I3C1_REG,
    PERIODIC_POLL_CMD4_LOC2_SB_I3C1_REG,
    PERIODIC_POLL_CMD5_LOC2_SB_I3C1_REG,
    PERIODIC_POLL_CMD6_LOC2_SB_I3C1_REG,
    PERIODIC_POLL_CMD7_LOC2_SB_I3C1_REG,
  #if MAX_MC_CH > 1
    PERIODIC_POLL_CMD8_LOC2_SB_I3C1_REG,
    PERIODIC_POLL_CMD9_LOC2_SB_I3C1_REG,
    PERIODIC_POLL_CMD10_LOC2_SB_I3C1_REG,
    PERIODIC_POLL_CMD11_LOC2_SB_I3C1_REG,
    PERIODIC_POLL_CMD12_LOC2_SB_I3C1_REG,
    PERIODIC_POLL_CMD13_LOC2_SB_I3C1_REG,
    PERIODIC_POLL_CMD14_LOC2_SB_I3C1_REG,
    PERIODIC_POLL_CMD15_LOC2_SB_I3C1_REG
  #endif
  };

  UINT32  I3c1TsodSlotTableReg[I3C_MC_NUMBER * MAX_MC_CH * MAX_DIMM * MAX_TS_PER_DIMM] = {
    PERIODIC_POLL_SLOT0_SB_I3C1_REG,
    PERIODIC_POLL_SLOT1_SB_I3C1_REG,
    PERIODIC_POLL_SLOT2_SB_I3C1_REG,
    PERIODIC_POLL_SLOT3_SB_I3C1_REG,
    PERIODIC_POLL_SLOT4_SB_I3C1_REG,
    PERIODIC_POLL_SLOT5_SB_I3C1_REG,
    PERIODIC_POLL_SLOT6_SB_I3C1_REG,
    PERIODIC_POLL_SLOT7_SB_I3C1_REG,
  #if MAX_MC_CH > 1
    PERIODIC_POLL_SLOT8_SB_I3C1_REG,
    PERIODIC_POLL_SLOT9_SB_I3C1_REG,
    PERIODIC_POLL_SLOT10_SB_I3C1_REG,
    PERIODIC_POLL_SLOT11_SB_I3C1_REG,
    PERIODIC_POLL_SLOT12_SB_I3C1_REG,
    PERIODIC_POLL_SLOT13_SB_I3C1_REG,
    PERIODIC_POLL_SLOT14_SB_I3C1_REG,
    PERIODIC_POLL_SLOT15_SB_I3C1_REG
  #endif
  };

  MaxChDdr = GetMaxChDdr ();
  Status = EFI_SUCCESS;
  TransactionID = TidPeriodicPoll;


  if (Instance == 0) {
    TsodCtrlReg = PERIODIC_POLL_CONTROL_SB_I3C0_REG;
  } else {
    TsodCtrlReg = PERIODIC_POLL_CONTROL_SB_I3C1_REG;
  }

  //
  // Below steps to InitalizeTsodConfig, the I3C module should load the default value in the CRIF if BIOS do not program it.
  //

  // Step 1. Program the number of core clock periods to count for computing 1 millisecond through 'TICK_TIME_COUNT' register.
  //         The clock used for timer is the side_clk of 100MHz for SPR.
  //         The default Interval is 1 millisecond between polling every temperature sensor.

  //
  // Step 2. Program the number of valid slots trough 'SLOT_RANGE' field on PERIODIC_POLL_CONTROL register.
  //         Slot Range defines the number of valid slots.
  //         Value After Reset: 0xf
  //

  //
  // Step 3. Program the number of ticks to count before scheduling the commands from the subsequent slot through 'SLOT_INTERVAL_COUNT' register.
  //         This count value decides the number of timer base ticks to count before scheduling the commands from the subsequent slot.
  //         The count value of 0 and 1 are treated as 1 by the controller.
  //         Attention: Register SLOT_TICK_TIME_CNT.TICK_TIME_COUNT shall be configured such that the internal timer base reference is 1 millisecond.
  //         Value After Reset: 0x1
  //  Set to 8 for 128ms TSOD interval time per Jedec TS Spec
  //
  TsodCtrl.Data = UsraCsrRead (Socket, Instance, TsodCtrlReg);
  TsodCtrl.Bits.slot_interval_count = TSOD_SLOT_INTERVAL_COUNT;
  UsraCsrWrite (Socket, Instance, TsodCtrlReg, TsodCtrl.Data);

  //
  // Form read command, Same command apply to all the temperature sensors
  //
  RegularCommandLow.Data = 0x0;
  RegularCommandLow.Bits.com_attr = I3C_COM_ATTR_XFER;  // 0x0: XFER: Regular Transfer
  RegularCommandLow.Bits.tid = TransactionID; // Transaction ID field is used as identification tag for the command.
  // RegularCommandLow.Bits.cmd = 0;  // do not care when cp == CP_TRANFSER
  RegularCommandLow.Bits.cp = CP_TRANFSER;
  // RegularCommandLow.Bits.slave_address = (UINT32) ((Dev.address.deviceType << I3C_STRAP_ADDRESS_OFFSET) | Dev.address.strapAddress); // set this field in step 4
  // 0x0: I3C SDR0/ I2C Fm: Note: I3C SDR0 represents standard SDR Speed (0 to fSCL Max)
  RegularCommandLow.Bits.mode_speed = GetModeSpeed (Socket, Instance);
  RegularCommandLow.Bits.rnw = RNW_READ; // 0x1: READ: Read transfer
  RegularCommandLow.Bits.roc = ROC_REQUIRED; // 0x1: REQUIRED: Response Status is required
  RegularCommandLow.Bits.toc = TOC_STOP; // 0x1: STOP: Stop (P) is issued at end of the transfer

  RegularCommandHigh.Data = 0x0;
  RegularCommandHigh.Bits.data_length = 2;  // 2 byte

  Dev.compId = SPD;
  Dev.address.controller = PLATFORM_SMBUS_CONTROLLER_PROCESSOR;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    //
    // Skip if the channel number per socket is not belong to this I3C Instance
    //
    if (Instance != (Ch / I3C_CH_NUMBER)) {
      continue;
    }

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      // Update the strapAddress
      Status = GetSmbAddressCommon (Socket, Ch, Dimm, &Dev);

      //
      // Step 4. Setup the PPCT (Periodic Poll Command Table) which contains the polling commands.
      //         Populate the commands on 'PERIODIC_POLL_CMD*_LOC*' register.
      //
      for (TempSensor = 0; TempSensor < MAX_TS_PER_DIMM; TempSensor++) {

        Dev.address.deviceType = TsodDeviceTypeId[TempSensor];
        EnableTsodReadAddressPointerMode (Socket, Ch, Dimm, Dev, TempSensor);

        if (Instance == 0) {
          TsodCmdLowerReg = I3c0TsodCmdTableLowerReg[TsodIndexTable[Ch % I3C_CH_NUMBER][Dimm][TempSensor]];
          TsodCmdUpperReg = I3c0TsodCmdTableUpperReg[TsodIndexTable[Ch % I3C_CH_NUMBER][Dimm][TempSensor]];
        } else {
          TsodCmdLowerReg = I3c1TsodCmdTableLowerReg[TsodIndexTable[Ch % I3C_CH_NUMBER][Dimm][TempSensor]];
          TsodCmdUpperReg = I3c1TsodCmdTableUpperReg[TsodIndexTable[Ch % I3C_CH_NUMBER][Dimm][TempSensor]];
        }

        RegularCommandLow.Bits.i2cni3c = GetSlaveType (Socket, Instance); // 0x0: I3C device 0x1: I2C device
        RegularCommandLow.Bits.slave_address = (UINT32) ((TsodDeviceTypeId[TempSensor] << I3C_STRAP_ADDRESS_OFFSET) | Dev.address.strapAddress);
        UsraCsrWrite (Socket, Instance, TsodCmdLowerReg, RegularCommandLow.Data);
        UsraCsrWrite (Socket, Instance, TsodCmdUpperReg, RegularCommandHigh.Data);
      }

      //
      // Step 5. Enable TSOD polling for the desired temperature sensors (since I3C has the capability to disable/enable every temperature sensor)
      //         Select the desired commands to be schedule through the 'PERIODIC_POLL_CMD_EN' register.
      // Enable TSOD polling in EnableTsod ()

      //
      // Step 6. Setup the PPST (Periodic Poll Slot Table) to indicate the slot(s) in which the corresponding PPCT entry must be launched on the bus.
      //         Program the desired slot bit map to each command on 'PERIODIC_POLL_SLOT*' register. do diagonal slot enabling per DIMM.
      //
      for (TempSensor = 0; TempSensor < MAX_TS_PER_DIMM; TempSensor++) {
        if (Instance == 0) {
          TsodSlotReg = I3c0TsodSlotTableReg[TsodIndexTable[Ch % I3C_CH_NUMBER][Dimm][TempSensor]];
        } else {
          TsodSlotReg = I3c1TsodSlotTableReg[TsodIndexTable[Ch % I3C_CH_NUMBER][Dimm][TempSensor]];
        }

        TsodSlot.Data = UsraCsrRead (Socket, Instance, TsodSlotReg);
        UsraCsrWrite (Socket, Instance, TsodSlotReg, TsodSlot.Bits.periodic_poll_slot0 | CH_DIMM_TS_TO_TSOD (Ch, Dimm, TempSensor));
      }

      //
      // Step 7. Enable the Periodic Poll Scheduler by writing 1 to 'PERIODIC_POLL_EN' field on register PERIODIC_POLL_CONTROL
      //         There is no 'PERIODIC_POLL_EN' field on register PERIODIC_POLL_CONTROL, by default is enabled.
      //

    } // Dimm
  } // Ch

  return Status;
}

/**
  Initialize I3C controller for given socket

  @param[in] Socket  - processor socket ID

  @retval EFI_SUCCESS      I3C initalized successfully
  @retval EFI_UNSUPPORTED  Function not supported
**/
EFI_STATUS
EFIAPI
InitProcSmb (
  IN UINT8  Socket
  )
{
  EFI_STATUS                                     Status;
  UINT8                                          MaxImc;
  UINT8                                          Instance;
  BOOLEAN                                        RcSimShouldBreak = FALSE;
  UINT32                                         DisablePollingReg;
  SB_I3C_DISABLE_POLLING_SB_I3C0_STRUCT          DisablePolling;
  UINT32                                         DevCapabilitesReg;
  DEVICE_CAPABILITIES_SB_I3C0_STRUCT             DevCapabilites;
  MMIO_CONTROL_POLICY_LOW_SB_I3C0_PVT_STRUCT     I3cControlPolicyLow;
  MMIO_READ_POLICY_LOW_SB_I3C0_PVT_STRUCT        I3cReadPolicyLow;
  MMIO_WRITE_POLICY_LOW_SB_I3C0_PVT_STRUCT       I3cWritePolicyLow;
  BOOLEAN                                        SiWaPostBootSai = FALSE;
  BOOLEAN                                        SiWaBootSai = FALSE;
  UINT32                                         I3cControlPolicyLowReg;
  UINT32                                         I3cReadPolicyLowReg;
  UINT32                                         I3cWritePolicyLowReg;
  UINT8                                          OriginalUbiosOutputMode;

  OriginalUbiosOutputMode = EnableI3cAsmOutput (TRUE);
  MaxImc = GetMaxImc ();
  Status = EFI_SUCCESS;

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_DIMM_DETECT, CHECKPOINT_MINOR_INIT_SMB, Socket));

  for (Instance = 0; Instance < MAX_SMB_INSTANCE; Instance++) {

    // Skip programming I3C1 in 1 die config
    if (I3C_MC_NUMBER * Instance >= MaxImc) {
      continue;
    }

    if (Instance == 0) {
      DevCapabilitesReg    = DEVICE_CAPABILITIES_SB_I3C0_REG;
      DisablePollingReg    = SB_I3C_DISABLE_POLLING_SB_I3C0_REG;
      I3cControlPolicyLowReg = MMIO_CONTROL_POLICY_LOW_SB_I3C0_PVT_REG;
      I3cReadPolicyLowReg  = MMIO_READ_POLICY_LOW_SB_I3C0_PVT_REG;
      I3cWritePolicyLowReg = MMIO_WRITE_POLICY_LOW_SB_I3C0_PVT_REG;
    } else {
      DevCapabilitesReg    = DEVICE_CAPABILITIES_SB_I3C1_REG;
      DisablePollingReg    = SB_I3C_DISABLE_POLLING_SB_I3C1_REG;
      I3cControlPolicyLowReg = MMIO_CONTROL_POLICY_LOW_SB_I3C1_PVT_REG;
      I3cReadPolicyLowReg  = MMIO_READ_POLICY_LOW_SB_I3C1_PVT_REG;
      I3cWritePolicyLowReg = MMIO_WRITE_POLICY_LOW_SB_I3C1_PVT_REG;
    }

    BreakForSmbDataTarget (&RcSimShouldBreak);
    //
    // Check if COMBO_COMMAND is supported in DEVICE_CAPABILITIES
    //
    DevCapabilites.Data = UsraCsrRead (Socket, Instance, DevCapabilitesReg);

    if (!RcSimShouldBreak && (DevCapabilites.Bits.combo_command == 0)) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "I3C Instance %d: COMBO_COMMAND is not supported!\n", Instance);
      RestoreI3cAsmOutput (OriginalUbiosOutputMode, TRUE);
      return EFI_UNSUPPORTED;
    }

    //
    // When we support emulation team for I3C registers programming
    // MRC shall enable a silicon workaround before the SAI STRAP issue got fixed.
    // Set bit 0(HOSTIA_POSTBOOT_SAI) and bit 4(HOSTIA_BOOT_SAI) of the policy registers for I3C registers access.
    //

    //
    // Headless MRC is not able to access the related registers in sideband.
    //
    if (IsSiliconWorkaroundEnabled ("S2209312485")) {
      SiWaBootSai = TRUE;
    }
    if (IsSiliconWorkaroundEnabled ("S22010953912")) {
      SiWaPostBootSai = TRUE;
    }

    if (SiWaPostBootSai || SiWaBootSai) {
      if (UbiosGenerationEnabled ()) {
        I3cControlPolicyLow.Data = MMIO_CONTROL_POLICY_LOW;
        I3cReadPolicyLow.Data = MMIO_READ_POLICY_LOW;
        I3cWritePolicyLow.Data = MMIO_WRITE_POLICY_LOW;
      } else {
        I3cControlPolicyLow.Data = UsraCsrRead (Socket, Instance, I3cControlPolicyLowReg);
        I3cReadPolicyLow.Data = UsraCsrRead (Socket, Instance, I3cReadPolicyLowReg);
        I3cWritePolicyLow.Data = UsraCsrRead (Socket, Instance, I3cWritePolicyLowReg);
      }

      if (SiWaPostBootSai) {
        I3cControlPolicyLow.Data |= HOSTIA_POSTBOOT_SAI;
        I3cReadPolicyLow.Data |= HOSTIA_POSTBOOT_SAI;
        I3cWritePolicyLow.Data |= HOSTIA_POSTBOOT_SAI;
      }

      if (SiWaBootSai) {
        I3cReadPolicyLow.Data |= HOSTIA_BOOT_SAI;
        I3cWritePolicyLow.Data |= HOSTIA_BOOT_SAI;
      }

      UsraCsrWrite (Socket, Instance, I3cControlPolicyLowReg, I3cControlPolicyLow.Data);
      UsraCsrWrite (Socket, Instance, I3cReadPolicyLowReg, I3cReadPolicyLow.Data);
      UsraCsrWrite (Socket, Instance, I3cWritePolicyLowReg, I3cWritePolicyLow.Data);
    }

    Status = ResetProcSmb (Socket, Instance);

    //
    // Make Sure TSOD Polling is all disabled
    //
    DisablePolling.Data = UsraCsrRead (Socket, Instance, DisablePollingReg);
    DisablePolling.Bits.disable_polling = 1;  // disable all Tsod polling
    UsraCsrWrite (Socket, Instance, DisablePollingReg, DisablePolling.Data);
  }

  RestoreI3cAsmOutput (OriginalUbiosOutputMode, TRUE);
  return Status;
} //InitProcSmb

/**
  Reset I3C controller with given Socket and Instance

  @param[in] Socket      - processor socket ID
  @param[in] SmbInstance - I3C Instance number

  @retval EFI_SUCCESS      I3C reset successfully
  @retval EFI_UNSUPPORTED  Function not supported
**/
EFI_STATUS
EFIAPI
ResetProcSmb (
  IN UINT8   Socket,
  IN UINT16  SmbInstance
  )
{
  EFI_STATUS                           Status;
  UINT8                                Instance;
  UINT32                               Timeout;
  BOOLEAN                              ShouldBreak = FALSE;
  UINT32                               I3cComDevCtrlReg;
  UINT32                               I3cDevCtrlReg;
  UINT32                               I3cResetReg;
  UINT32                               I3cQueueCtrlReg;
  UINT32                               I3cStatusEnReg;
  UINT32                               I3cDataCtrlReg;
  COMMON_DEVICE_CTRL_SB_I3C0_STRUCT    I3cComDevCtrl;
  DEVICE_CONTROL_SB_I3C0_STRUCT        I3cDevCtrl;
  DATA_BUFFER_THLD_CTRL_SB_I3C0_STRUCT I3cDataThld;
  QUEUE_THLD_CTRL_SB_I3C0_STRUCT       I3cQueueThld;
  RESET_CTRL_SB_I3C0_STRUCT            ResetCtrl;
  SYS_SETUP                            *Setup;

  Status = EFI_SUCCESS;
  Instance = (UINT8) SmbInstance;
  Setup = GetSysSetupPointer ();

  if (Instance == 0) {
    I3cComDevCtrlReg  = COMMON_DEVICE_CTRL_SB_I3C0_REG;
    I3cDevCtrlReg     = DEVICE_CONTROL_SB_I3C0_REG;
    I3cResetReg       = RESET_CTRL_SB_I3C0_REG;
    I3cQueueCtrlReg   = QUEUE_THLD_CTRL_SB_I3C0_REG;
    I3cStatusEnReg    = INTR_STATUS_ENABLE_SB_I3C0_REG;
    I3cDataCtrlReg    = DATA_BUFFER_THLD_CTRL_SB_I3C0_REG;
  } else {
    I3cComDevCtrlReg  = COMMON_DEVICE_CTRL_SB_I3C1_REG;
    I3cDevCtrlReg     = DEVICE_CONTROL_SB_I3C1_REG;
    I3cResetReg       = RESET_CTRL_SB_I3C1_REG;
    I3cQueueCtrlReg   = QUEUE_THLD_CTRL_SB_I3C1_REG;
    I3cStatusEnReg    = INTR_STATUS_ENABLE_SB_I3C1_REG;
    I3cDataCtrlReg    = DATA_BUFFER_THLD_CTRL_SB_I3C1_REG;
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "Reset I3C controller Instance %d\n", Instance);

  //
  //Disable I3C
  //
  I3cDevCtrl.Data = UsraCsrRead (Socket, Instance, I3cDevCtrlReg);
  I3cDevCtrl.Bits.enable = I3C_DISABLE;
  UsraCsrWrite (Socket, Instance, I3cDevCtrlReg, I3cDevCtrl.Data);

  //reset the sts and fifo
  ResetCtrl.Data = UsraCsrRead (Socket, Instance, I3cResetReg);
  ResetCtrl.Bits.soft_rst = I3C_ENABLE;
  ResetCtrl.Bits.cmd_queue_rst = I3C_ENABLE;
  ResetCtrl.Bits.resp_queue_rst = I3C_ENABLE;
  ResetCtrl.Bits.tx_fifo_rst = I3C_ENABLE;
  ResetCtrl.Bits.rx_fifo_rst = I3C_ENABLE;
  ResetCtrl.Bits.ibi_queue_rst = I3C_ENABLE;
  UsraCsrWrite (Socket, Instance, I3cResetReg, ResetCtrl.Data);

  //
  // Wait for reset completion.
  //
  Timeout = I3C_TIMEOUT;

  do {
    ResetCtrl.Data = UsraCsrRead (Socket, Instance, I3cResetReg);

    if (GetEmulation () & SIMICS_FLAG) {
      break;
    }

    BreakForSmbDataTarget (&ShouldBreak);

    if (ShouldBreak) {
      break;
    }

    MicroSecondDelay (1);
    Timeout--;

    if (Timeout <= 0) {
      Status = EFI_TIMEOUT;
      break;
    }

  } while (ResetCtrl.Bits.soft_rst == 1 || ResetCtrl.Bits.cmd_queue_rst == 1);

  I3cQueueThld.Data = UsraCsrRead (Socket, Instance, I3cQueueCtrlReg);
  //set resp buf thld to 0  [15:8]
  I3cQueueThld.Bits.resp_buf_thld = 0;
  //set cmd empty thld buf to 1  [7:0]
  I3cQueueThld.Bits.cmd_empty_buf_thld = 1;
  UsraCsrWrite (Socket, Instance, I3cQueueCtrlReg, I3cQueueThld.Data);

  I3cDataThld.Data = UsraCsrRead (Socket, Instance, I3cDataCtrlReg);
  //
  // set resp_buf_thld as 0 to identify every transaction is ready for reading/writing
  // set RX/TX Start/buf thld as n to support n+1 byte Read/Write
  // The supported values for RX_START_THLD/TX_START_THLD/RX_BUF_THLD/TX_BUF_THLD are:
  //   000 - 1;  001 - 4;  010 - 8;   011 - 16
  //   100 - 32; 101 - 64; 110 - 128; 111 - 256
  //
  //set RX Start thld to 0  [26:24]
  I3cDataThld.Bits.rx_start_thld = 0;
  //set RX buf thld to 0  [10:8]
  I3cDataThld.Bits.rx_buf_thld = 0;
  //set TX Start THLD to 0  [18:16]
  I3cDataThld.Bits.tx_start_thld = 0;
  //set tX buf thld to 0  [2:0]
  I3cDataThld.Bits.tx_buf_thld = 0;
  UsraCsrWrite (Socket, Instance, I3cDataCtrlReg, I3cDataThld.Data);

  //Enable I3C
  I3cDevCtrl.Bits.enable = I3C_ENABLE;
  UsraCsrWrite (Socket, Instance, I3cDevCtrlReg, I3cDevCtrl.Data);

  //
  // Enable all the states for I3cStatusEnReg
  //
  UsraCsrWrite (Socket, Instance, I3cStatusEnReg, I3C_INTR_STATUS_ENABLE_ALL);

  //
  // Set Device NACK Retry Count
  //
  I3cComDevCtrl.Data = UsraCsrRead (Socket, Instance, I3cComDevCtrlReg);
  I3cComDevCtrl.Bits.dev_nack_retry_cnt = MAX_DEV_NACK_RETRY_CNT;
  UsraCsrWrite (Socket, Instance, I3cComDevCtrlReg, I3cComDevCtrl.Data);

  //
  // Program timing registers
  //
  if (GetSlaveType (Socket, Instance) == I3C_DEVICE) {
    ProgramProcSmbFreq (Socket, Instance, I3C_MODE);
  } else {
    ProgramProcSmbFreq (Socket, Instance, I2C_MODE);
  }

  //
  // Set i2c_slave_present when the Bus is in I2C mode
  //
  if (Setup->mem.dfxMemSetup.DfxSmbusI3cModeEnable == FALSE) {
    SetI2cSlavePresent (Socket, Instance, TRUE);
  }

  return Status;
} //ResetProcSmb

/**
  Adjust the I3C frequency to the specified value.

  @param[in] Socket       - Processor socket ID
  @param[in] SmbInstance  - SMBUS Instance number
  @param[in] ClkFrequency - Clock Frequency to be programmed

  @retval EFI_SUCCESS           - Clock Period programmed successfully
  @retval EFI_INVALID_PARAMETER - Invalid Socket
  @retval EFI_UNSUPPORTED       - Function not supported
**/
EFI_STATUS
EFIAPI
AdjustProcSmbFreq (
  IN UINT8                Socket,
  IN UINT8                SmbInstance,
  IN SMB_CLOCK_FREQUENCY  ClkFrequency
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Determines if Semaphore is needed to access I3C.

  If SBSP is trying to access local I3C, Semaphore is NOT needed.
  If SBSP is trying to access AP I3C, Semaphore IS needed
  If I3C target socket, which is not SBSP, is trying to access AP I3C, Semaphore IS needed.

  @param[in] Socket  - Processor socket ID

  @retval TRUE   Semaphore is needed to access I3C
  @retval FALSE  Semaphore is not needed to access I3C
**/
BOOLEAN
IsSmbSemaphoreNeeded (
  IN UINT8  Socket
  )
{
  UINT8    SbspSocketIdNv;
  BOOLEAN  SemaphoreNeeded;

  SemaphoreNeeded = FALSE;

  SbspSocketIdNv = GetSysSbspSocketIdNv ();

  // Current thread is SBSP
  if (GetSysSocketId () == SbspSocketIdNv) {
    if (Socket != SbspSocketIdNv) {
      //
      // I3C target socket wants to access AP I3C, semaphore needed
      //
      SemaphoreNeeded = TRUE;

    } else {
      //
      // I3C target socket is SBSP, semaphore not needed
      //
      SemaphoreNeeded = FALSE;
    }
  } else {  // Current thread is NOT SBSP
    if (Socket == GetSysSocketId ()) {
      //
      // I3C target socket wants to access AP I3C, semaphore needed
      //
      SemaphoreNeeded = TRUE;
    } else {
      //
      // I3C target thread does not equal current thread
      //
      SemaphoreNeeded = FALSE;
      RC_FATAL_ERROR (Socket == GetSysSocketId (), ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_29);
    }
  }

  return SemaphoreNeeded;

}

/**
  Read I3C register at a given device and offset

  @param[in]  Socket      - Processor socket ID
  @param[in]  Dev         - Device address on I3C
  @param[in]  ByteOffset  - Offset in Device
  @param[out] Data        - Pointer to store value

  @retval EFI_SUCCESS       Read was successful
  @retval EFI_DEVICE_ERROR  Read failed
  @retval EFI_UNSUPPORTED   Function not supported
**/
EFI_STATUS
EFIAPI
ReadProcSmbWorker (
  IN  UINT8              Socket,
  IN  SMB_DEVICE_STRUCT  Dev,
  IN  UINT8              ByteOffset,
  OUT volatile UINT8     *Data
  )
{
  EFI_STATUS                              Status;
  UINT32                                  I3cStatusReg;
  UINT32                                  I3cCmdReg;
  UINT32                                  I3cResetReg;
  UINT32                                  I3cQueueCtrlReg;
  UINT32                                  I3cDataReg;
  UINT32                                  I3cRespReg;
  UINT32                                  Timeout;
  RESPONSE_QUEUE_PORT_SB_I3C0_STRUCT      I3cResp;
  BOOLEAN                                 ShouldBreak = FALSE;
  UINT8                                   TransactionID;
  UINT8                                   Instance;
  UINT8                                   I2cOrI3c;
  UINT8                                   SubOffsetLen;
  UINT32                                  SubOffset;
  COMMAND_QUEUE_PORT_SB_I3C0_STRUCT       CmdPort;
  COMBO_TRANSFER_COMMAND_LOW_WITHOUT_DAT  ComboCommandLow;
  COMBO_TRANSFER_COMMAND_HIGH_WITHOUT_DAT ComboCommandHigh;
  // Same address with RX_DATA_PORT and TX_DATA_PORT, single bi-direction design
  volatile DATA_PORT_SB_I3C0_STRUCT       DataPort;
  INTR_STATUS_SB_I3C0_STRUCT              I3cStatus;

  Status = EFI_SUCCESS;
  TransactionID = TidRead;
  Instance = (UINT8) Dev.address.SmbInstance;

  CountTrackingData (PTC_SMBUS_READ, 1);

  if (Instance == 0) {
    I3cCmdReg = COMMAND_QUEUE_PORT_SB_I3C0_REG;
    I3cResetReg = RESET_CTRL_SB_I3C0_REG;
    I3cQueueCtrlReg = QUEUE_THLD_CTRL_SB_I3C0_REG;
    I3cStatusReg = INTR_STATUS_SB_I3C0_REG;
    I3cDataReg = DATA_PORT_SB_I3C0_REG;  // Use DATA_PORT for reading and writing
    I3cRespReg = RESPONSE_QUEUE_PORT_SB_I3C0_REG;
  } else {
    I3cCmdReg = COMMAND_QUEUE_PORT_SB_I3C1_REG;
    I3cResetReg = RESET_CTRL_SB_I3C1_REG;
    I3cQueueCtrlReg = QUEUE_THLD_CTRL_SB_I3C1_REG;
    I3cStatusReg = INTR_STATUS_SB_I3C1_REG;
    I3cDataReg = DATA_PORT_SB_I3C1_REG;  // Use DATA_PORT for reading and writing
    I3cRespReg = RESPONSE_QUEUE_PORT_SB_I3C1_REG;
  }

  if (GetSlaveType (Socket, Instance) == I3C_DEVICE) {
    I2cOrI3c = I3C_DEVICE;
  } else {
    I2cOrI3c = I2C_DEVICE;
  }

  // Use 2 Byte addressing only when accessing the SPR DDR5 SPD data in I3C mode
  if ((I2cOrI3c == I3C_DEVICE) && (Dev.address.deviceType == DTI_EEPROM)) {
    SubOffsetLen = SUBOFFSET_16_BIT;
    SubOffset = (Dev.SpdPage | (ByteOffset << 8));  // The low byte should follow the definition of SPD_DDR5_ADDRESS_SECOND_BYTE_STRUCT
  } else {
    SubOffsetLen = SUBOFFSET_8_BIT;
    SubOffset = ByteOffset;
  }

  //
  // Form read command
  //
  ComboCommandLow.Data = 0x0;
  ComboCommandLow.Bits.com_attr = I3C_COM_ATTR_WWR_COMBO_XFER;  // 0x3: WWR_COMBO_XFER: Write+Write/Read Combo Transfer;
  ComboCommandLow.Bits.tid = TransactionID; // Transaction ID field is used as identification tag for the command.
  ComboCommandLow.Bits.i2cni3c = I2cOrI3c; // 0x0: I3C device 0x1: I2C device
  ComboCommandLow.Bits.slave_address = (UINT32) ((Dev.address.deviceType << I3C_STRAP_ADDRESS_OFFSET) | Dev.address.strapAddress);
  //Indicates 8-or 16-bit sub-offset Values:0x0: 8_BIT_SUBOFFSET: 8 bit suboffset; 0x1: suboffset_16_bit:16 bit suboffset
  ComboCommandLow.Bits.suboffset_16_bit = SubOffsetLen;
  // 0x0: I3C SDR0/ I2C Fm: Note: I3C SDR0 represents standard SDR Speed (0 to fSCL Max)
  ComboCommandLow.Bits.mode_speed = GetModeSpeed (Socket, Instance);
  ComboCommandLow.Bits.rnw = RNW_READ; // 0x1: READ: Read transfer
  ComboCommandLow.Bits.roc = ROC_REQUIRED; // 0x1: REQUIRED: Response Status is required
  ComboCommandLow.Bits.toc = TOC_STOP; // 0x1: STOP: Stop (P) is issued at end of the transfer

  ComboCommandHigh.Data = 0x0;
  ComboCommandHigh.Bits.offset_suboffset = SubOffset;

  if (Dev.compId == MTS) {
    ComboCommandHigh.Bits.data_length = 2;  // 2 byte
  } else {
    ComboCommandHigh.Bits.data_length = 1;  // 1 byte
  }

  CmdPort.Data = 0x0;
  CmdPort.Bits.command = ComboCommandLow.Data;

  //
  // Send command
  //
  // Low 32 bits
  Status = WaitForHostNotBusyTarget (Socket, Instance, I3cStatusReg);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  PollSmbusStatusRegister (Socket, Instance, I3cStatusReg, I3C_CMD_QUEUE_READY_BIT_MASK, I3C_CMD_QUEUE_READY_POLLING_LOGIC);
  UsraCsrWrite (Socket, Instance, I3cCmdReg, CmdPort.Bits.command);

  // High 32 bits
  // wait for there is space in the command queue
  CmdPort.Bits.command = ComboCommandHigh.Data;
  Status = WaitForHostNotBusyTarget (Socket, Instance, I3cStatusReg);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  PollSmbusStatusRegister (Socket, Instance, I3cStatusReg, I3C_CMD_QUEUE_READY_BIT_MASK, I3C_CMD_QUEUE_READY_POLLING_LOGIC);
  UsraCsrWrite (Socket, Instance, I3cCmdReg, CmdPort.Bits.command);

  //
  // Check Transfer Complete
  //
  Timeout = I3C_TIMEOUT;

  do {
    I3cStatus.Data = UsraCsrRead (Socket, Instance, I3cStatusReg);

    CountTrackingData (PTC_POLLING_COUNT, 1);

    if (GetEmulation () & SIMICS_FLAG) {
      break;
    }

    BreakForSmbDataTarget (&ShouldBreak);

    if (ShouldBreak) {
      break;
    }

    MicroSecondDelay (1);
    Timeout--;

    if (Timeout <= 0) {
      Status = EFI_TIMEOUT;
      break;
    }

  } while ((I3cStatus.Bits.resp_ready_stat == 0) && (I3cStatus.Bits.transfer_err_stat == 0));

  PollSmbusStatusRegister (Socket, Instance, I3cStatusReg, I3C_RESPONSE_READY_BIT_MASK, I3C_RESPONSE_READY_POLLING_LOGIC);

  //
  // Wait for the read data
  //
  Timeout = I3C_TIMEOUT;

  do {
    I3cStatus.Data = UsraCsrRead (Socket, Instance, I3cStatusReg);

    CountTrackingData (PTC_POLLING_COUNT, 1);

    BreakForSmbDataTarget (&ShouldBreak);

    if (ShouldBreak) {
      break;
    }

    MicroSecondDelay (1);
    Timeout--;

    if (Timeout <= 0) {
      Status = EFI_TIMEOUT;
      break;
    }

  } while ((I3cStatus.Bits.rx_thld_stat == 0) && (I3cStatus.Bits.transfer_err_stat == 0));

  PollSmbusStatusRegister (Socket, Instance, I3cStatusReg, I3C_READ_DONE_BIT_MASK, I3C_READ_DONE_POLLING_LOGIC);
  PollSmbusStatusRegister (Socket, Instance, I3cStatusReg, I3C_TRANSFER_ERROR_BIT_MASK, I3C_TRANSFER_ERROR_POLLING_LOGIC);

  //
  // If read data is valid
  //
  if ((I3cStatus.Bits.rx_thld_stat == 1) && (I3cStatus.Bits.transfer_err_stat == 0)) {
    DataPort.Data = UsraCsrRead (Socket, Instance, I3cDataReg);

    if (Dev.compId == MTS) {
      *Data = (UINT8) DataPort.Bits.data_port;  //lsb
      *(Data + 1) = (UINT8) (DataPort.Bits.data_port >> 8); //msb
    } else {
      *Data = (UINT8) DataPort.Bits.data_port;
    }

  } else {
    Status = EFI_DEVICE_ERROR;
  }

  //
  // Check response status
  //
  I3cResp.Data = UsraCsrRead (Socket, Instance, I3cRespReg);


  if ((I3cResp.Bits.tid != TransactionID) || I3cResp.Bits.err_status == 1) {
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
} //ReadProcSmbWorker

/**
  Read the processor I3C device at a given device and offset

  @param[in] Dev         - I3C Device address
  @param[in] ByteOffset  - byte offset within Device
  @param[out] Data       - pointer to store value

  @retval EFI_SUCCESS       Read was successful
  @retval EFI_DEVICE_ERROR  Read failed
**/
EFI_STATUS
EFIAPI
ReadProcSmb (
  IN  SMB_DEVICE_STRUCT  Dev,
  IN  UINT8              ByteOffset,
  OUT volatile UINT8     *Data
  )
{
  EFI_STATUS  Status;
  UINT8       SmbSocket;

  Status = EFI_SUCCESS;

  SmbSocket = (UINT8) Dev.address.SmbSocket;
  // Acquire SMBUS Semaphore
  AcquireSmbusSemaphoreControl (SmbSocket);

  Status = ReadProcSmbWorker (SmbSocket, Dev, ByteOffset, Data);
  if (EFI_ERROR (Status) && IsSmbSimulated ()) {
    Status = EFI_SUCCESS; // If in a simulated environment, do not report I3C failures
    *Data = 0;            // make sure data is determinant
  }
  // Retry if error detected
  if (EFI_ERROR (Status)) {
    ResetProcSmb (SmbSocket, Dev.address.SmbInstance);
    Status = ReadProcSmbWorker (SmbSocket, Dev, ByteOffset, Data);
  }

  // Release Semaphore
  ReleaseSmbusSemaphoreControl (SmbSocket);

  return Status;
}

/**
  Write I3C dev/offset with specified data

  @param[in] Socket      - Processor socket ID
  @param[in] Dev         - I3C device address
  @param[in] ByteOffset  - byte offset within device
  @param[in] Data        - pointer to data to write

  @retval EFI_SUCCESS       Write was successful
  @retval EFI_DEVICE_ERROR  Write failed
  @retval EFI_UNSUPPORTED   Function not supported
**/
EFI_STATUS
EFIAPI
WriteProcSmbWorker (
  IN UINT8              Socket,
  IN SMB_DEVICE_STRUCT  Dev,
  IN UINT8              ByteOffset,
  IN UINT8              *Data
  )
{
  EFI_STATUS                              Status = EFI_SUCCESS;
  UINT32                                  I3cStatusReg;
  UINT32                                  I3cCmdReg;
  UINT32                                  I3cResetReg;
  UINT32                                  I3cQueueCtrlReg;
  UINT32                                  I3cDataReg;
  UINT32                                  I3cRespReg;
  UINT32                                  Timeout;
  RESPONSE_QUEUE_PORT_SB_I3C0_STRUCT      I3cResp;
  BOOLEAN                                 ShouldBreak = FALSE;
  UINT8                                   TransactionID;
  UINT8                                   Instance;
  UINT8                                   I2cOrI3c;
  COMMAND_QUEUE_PORT_SB_I3C0_STRUCT       CmdPort;
  REGULAR_DATA_TRANSFER_COMMAND_LOW_WITHOUT_DAT  RegularCommandLow;
  REGULAR_DATA_TRANSFER_COMMAND_HIGH_WITHOUT_DAT RegularCommandHigh;
  DATA_PORT_SB_I3C0_STRUCT                DataPort;
  INTR_STATUS_SB_I3C0_STRUCT              I3cStatus;

  TransactionID = TidWrite;
  Instance = (UINT8) Dev.address.SmbInstance;

  if (IsHvmModeEnable ()) {
    return Status;
  }

  CountTrackingData (PTC_SMBUS_WRITE, 1);

  if (Instance == 0) {
    I3cCmdReg = COMMAND_QUEUE_PORT_SB_I3C0_REG;
    I3cResetReg = RESET_CTRL_SB_I3C0_REG;
    I3cQueueCtrlReg = QUEUE_THLD_CTRL_SB_I3C0_REG;
    I3cStatusReg = INTR_STATUS_SB_I3C0_REG;
    I3cDataReg = DATA_PORT_SB_I3C0_REG;  // Use DATA_PORT for reading and writing
    I3cRespReg = RESPONSE_QUEUE_PORT_SB_I3C0_REG;
  } else {
    I3cCmdReg = COMMAND_QUEUE_PORT_SB_I3C1_REG;
    I3cResetReg = RESET_CTRL_SB_I3C1_REG;
    I3cQueueCtrlReg = QUEUE_THLD_CTRL_SB_I3C1_REG;
    I3cStatusReg = INTR_STATUS_SB_I3C1_REG;
    I3cDataReg = DATA_PORT_SB_I3C1_REG;  // Use DATA_PORT for reading and writing
    I3cRespReg = RESPONSE_QUEUE_PORT_SB_I3C1_REG;
  }

  if (GetSlaveType (Socket, Instance) == I3C_DEVICE) {
    I2cOrI3c = I3C_DEVICE;
  } else {
    I2cOrI3c = I2C_DEVICE;
  }

  //
  // Form write command
  //
  RegularCommandLow.Data = 0x0;
  RegularCommandLow.Bits.com_attr = I3C_COM_ATTR_XFER;  // I3C_COM_ATTR_XFER             0x0  // Regular Transfer
  RegularCommandLow.Bits.tid = TransactionID; // Transaction ID field is used as identification tag for the command.
  RegularCommandLow.Bits.i2cni3c = I2cOrI3c; // 0x0: I3C device 0x1: I2C device
  //RegularCommandLow.Bits.cmd // CMD field is not valid
  RegularCommandLow.Bits.cp = CP_TRANFSER; // 0x0: TRANFSER: Describes SDR transfer. CMD field is not valid.
  RegularCommandLow.Bits.slave_address = (UINT32) ((Dev.address.deviceType << I3C_STRAP_ADDRESS_OFFSET) | Dev.address.strapAddress);
  // 0x0: I3C SDR0/ I2C Fm: Note: I3C SDR0 represents standard SDR Speed (0 to fSCL Max)
  RegularCommandLow.Bits.mode_speed = GetModeSpeed (Socket, Instance);
  RegularCommandLow.Bits.rnw = RNW_WRITE; // 0x0: WRITE: Write transfer
  RegularCommandLow.Bits.roc = ROC_REQUIRED; // 0x1: REQUIRED: Response Status is required
  RegularCommandLow.Bits.toc = TOC_STOP; // 0x1: STOP: Stop (P) is issued at end of the transfer

  RegularCommandHigh.Data = 0x0;

  CmdPort.Data = 0x0;
  CmdPort.Bits.command = RegularCommandLow.Data;
  //
  // data to write
  //
  DataPort.Data = 0;

  if (Dev.compId == MTS) {
    RegularCommandHigh.Bits.data_length = ONE_BYTE_ADDRESSING + 2;  // 1 Byte addressing + 2 bytes data
    DataPort.Bits.data_port = ((UINT32) ByteOffset) | ((UINT32) *Data << 8) | ((UINT32) *(Data + 1) << 16);
  } else {
    // Use 2 Byte addressing only when accessing the SPR DDR5 SPD data in I3C mode
    if ((I2cOrI3c == I3C_DEVICE) && (Dev.address.deviceType == DTI_EEPROM)) {
      RegularCommandHigh.Bits.data_length = TWO_BYTE_ADDRESSING + 1;  // 2 Byte addressing + 1 byte data
      DataPort.Bits.data_port = ((UINT32) ByteOffset) | ((UINT32) Dev.SpdPage << 8) | ((UINT32) *Data << 16);
    } else {
      RegularCommandHigh.Bits.data_length = ONE_BYTE_ADDRESSING + 1;  // 1 Byte addressing + 1 byte data
      DataPort.Bits.data_port = ((UINT32) ByteOffset) | ((UINT32) *Data << 8);
    }
  }

  //
  // check if there is space in the tx_fifo
  //
  Timeout = I3C_TIMEOUT;

  do {
    I3cStatus.Data = UsraCsrRead (Socket, Instance, I3cStatusReg);

    if (GetEmulation () & SIMICS_FLAG) {
      break;
    }

    BreakForSmbDataTarget (&ShouldBreak);

    if (ShouldBreak) {
      break;
    }

    MicroSecondDelay (1);
    Timeout--;

    if (Timeout <= 0) {
      Status = EFI_TIMEOUT;
      break;
    }

  } while ((I3cStatus.Bits.tx_thld_stat == 0) && (I3cStatus.Bits.transfer_err_stat == 0));

  PollSmbusStatusRegister (Socket, Instance, I3cStatusReg, I3C_WRITE_DONE_BIT_MASK, I3C_WRITE_DONE_POLLING_LOGIC);
  PollSmbusStatusRegister (Socket, Instance, I3cStatusReg, I3C_TRANSFER_ERROR_BIT_MASK, I3C_TRANSFER_ERROR_POLLING_LOGIC);

  //
  // Write the Data
  //
  UsraCsrWrite (Socket, Instance, I3cDataReg, DataPort.Data);

  //
  // Send command
  //
  // Low 32 bits
  Status = WaitForHostNotBusyTarget (Socket, Instance, I3cStatusReg);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  PollSmbusStatusRegister (Socket, Instance, I3cStatusReg, I3C_CMD_QUEUE_READY_BIT_MASK, I3C_CMD_QUEUE_READY_POLLING_LOGIC);
  UsraCsrWrite (Socket, Instance, I3cCmdReg, CmdPort.Bits.command);

  // High 32 bits
  CmdPort.Bits.command = RegularCommandHigh.Data;
  Status = WaitForHostNotBusyTarget (Socket, Instance, I3cStatusReg);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  PollSmbusStatusRegister (Socket, Instance, I3cStatusReg, I3C_CMD_QUEUE_READY_BIT_MASK, I3C_CMD_QUEUE_READY_POLLING_LOGIC);
  UsraCsrWrite (Socket, Instance, I3cCmdReg, CmdPort.Bits.command);

  //
  // Wait for the write to complete
  //
  if (!EFI_ERROR (Status)) {
    Status = WaitForWriteToCompleteTarget (Socket, Instance, I3cStatusReg);
    PollSmbusStatusRegister (Socket, Instance, I3cStatusReg, I3C_RESPONSE_READY_BIT_MASK, I3C_RESPONSE_READY_POLLING_LOGIC);
  }

  //
  // Check response status
  //
  I3cResp.Data = UsraCsrRead (Socket, Instance, I3cRespReg);


  if ((I3cResp.Bits.tid != TransactionID) || I3cResp.Bits.err_status == 1) {
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
} //WriteProcSmbWorker

/**
  Write the processor I3C dev/offset with specified data

  @param[in] Dev         - I3C device address
  @param[in] ByteOffset  - byte offset within device
  @param[in] Data        - pointer to data to write

  @retval EFI_SUCCESS      - Write was successful
  @retval EFI_DEVICE_ERROR - Write failed
**/
EFI_STATUS
EFIAPI
WriteProcSmb (
  IN SMB_DEVICE_STRUCT  Dev,
  IN UINT8              ByteOffset,
  IN UINT8              *Data
  )
{
  EFI_STATUS  Status;
  UINT8       SmbSocket;

  Status = EFI_SUCCESS;

  SmbSocket = (UINT8) Dev.address.SmbSocket;
  // Acquire SMBUS Semaphore
  AcquireSmbusSemaphoreControl (SmbSocket);

  Status = WriteProcSmbWorker (SmbSocket, Dev, ByteOffset, Data);

  if (IsSmbSimulated ()) {
    Status = EFI_SUCCESS;
  }

  // Retry if error detected
  if (EFI_ERROR (Status)) {
    ResetProcSmb (SmbSocket, Dev.address.SmbInstance);
    Status = WriteProcSmbWorker (SmbSocket, Dev, ByteOffset, Data);
  }

  // Release Semaphore
  ReleaseSmbusSemaphoreControl (SmbSocket);

  return Status;
}

/**
  Returns the I3C strap address of the DIMM slot.

  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number
  @param[in]  Dimm    - Dimm number
  @param[out] Dev     - Pointer to smbDevice

  @retval EFI_SUCCESS  Successfully retrieved I3C address data
**/
EFI_STATUS
EFIAPI
GetSmbAddress (
  IN UINT8               Socket,
  IN UINT8               Ch,
  IN UINT8               Dimm,
  OUT SMB_DEVICE_STRUCT  *Dev
  )
{
  EFI_STATUS  Status;

  Status = GetSmbAddressCommon (Socket, Ch, Dimm, Dev);

  return Status;
}

/**
  Calculates the Channel and Dimm number based on the I3C address information.

  @param[in]  SpdDev - ptr to smbDevice which contains Instance and strap address data
  @param[out] Chan   - Channel number based on I3C address
  @param[out] Dimm   - Dimm number based on I3C address

  @rval EFI_SUCCESS  Successfully calculated channel and dimm number
**/
EFI_STATUS
EFIAPI
GetChDimmFromSmbAddress (
  IN SMB_DEVICE_STRUCT  *Dev,
  OUT UINT8             *Chan,
  OUT UINT8             *Dimm
  )
{
  UINT8                 NumChPerMc;
  UINT8                 MaxImc;

  NumChPerMc       =    GetNumChannelPerMc ();
  MaxImc           =    GetMaxImc ();

  *Chan = (UINT8) ((Dev->address.SmbInstance * (NumChPerMc * MaxImc / 2)) + (Dev->address.strapAddress / MAX_DIMM));
  *Dimm = Dev->address.strapAddress % MAX_DIMM;

  return EFI_SUCCESS;
}

/**
  This routine gets the Tsod Polling enable bit value

  @param[in] Socket   - Socket Id
  @param[in] SmbInstance  - SMBus Instance number

  @retval TsodPollEn  value of Tsod poll enable bit
**/
UINT32
GetTsodPollEnSmb (
  IN UINT8   Socket,
  IN UINT16  SmbInstance
  )
{
  UINT32                                      TsodCmdReg;
  PERIODIC_POLL_COMMAND_ENABLE_SB_I3C0_STRUCT TsodCmd;
  UINT8                                       Instance;

  Instance = (UINT8) SmbInstance;

  if (Instance > 0) {
    TsodCmdReg = PERIODIC_POLL_COMMAND_ENABLE_SB_I3C1_REG;
  } else {
    TsodCmdReg = PERIODIC_POLL_COMMAND_ENABLE_SB_I3C0_REG;
  }

  TsodCmd.Data = UsraCsrRead (Socket, Instance, TsodCmdReg);

  return ((TsodCmd.Bits.periodic_poll_cmd_en == 0) ? 0x0 : 0x1);
}

/**
  This routine sets periodic_poll_cmd_en of valid channel and dimm to the given value

  @param[in] Socket        - Socket Id
  @param[in] SmbInstance   - SMBus Instance number
  @param[in] TsodPollEn    - TsodPollEn value to set

  @retval  VOID
**/
VOID
SetTsodPollEnSmb (
  IN UINT8   Socket,
  IN UINT8   SmbInstance,
  IN UINT32  TsodPollEn
  )
{
  UINT8                                       Ch;
  UINT8                                       Dimm;
  UINT8                                       MaxChDdr;
  UINT8                                       TempSensor;
  UINT32                                      TsodCmdDataMask;
  UINT32                                      TsodCmdReg;
  PERIODIC_POLL_COMMAND_ENABLE_SB_I3C0_STRUCT TsodCmd;
  UINT32                                      DisablePollingReg;
  SB_I3C_DISABLE_POLLING_SB_I3C0_STRUCT       DisablePolling;

  MaxChDdr = GetMaxChDdr ();
  TsodCmdDataMask = 0;

  if (SmbInstance > 0) {
    TsodCmdReg = PERIODIC_POLL_COMMAND_ENABLE_SB_I3C1_REG;
    DisablePollingReg = SB_I3C_DISABLE_POLLING_SB_I3C1_REG;
  } else {
    TsodCmdReg = PERIODIC_POLL_COMMAND_ENABLE_SB_I3C0_REG;
    DisablePollingReg = SB_I3C_DISABLE_POLLING_SB_I3C0_REG;
  }

  DisablePolling.Data = UsraCsrRead (Socket, SmbInstance, DisablePollingReg);

  if (TsodPollEn == 0) {

    DisablePolling.Bits.disable_polling = 1;
    UsraCsrWrite (Socket, SmbInstance, DisablePollingReg, DisablePolling.Data);

  } else {

    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      //
      // Skip if the channel number per socket is not belong to this I3C Instance
      //
      if (SmbInstance != (Ch / I3C_CH_NUMBER)) {
        continue;
      }

      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
          continue;
        }

        for (TempSensor = 0; TempSensor < MAX_TS_PER_DIMM; TempSensor++) {
          TsodCmdDataMask |= CH_DIMM_TS_TO_TSOD (Ch, Dimm, TempSensor);
        }
      } // Dimm
    } // Ch

    InitalizeTsodConfig (Socket, SmbInstance);

    DisablePolling.Bits.disable_polling = 0;
    UsraCsrWrite (Socket, SmbInstance, DisablePollingReg, DisablePolling.Data);

    TsodCmd.Data = 0;
    TsodCmd.Bits.periodic_poll_cmd_en = TsodCmdDataMask;
    UsraCsrWrite (Socket, SmbInstance, TsodCmdReg, TsodCmd.Data);
  }
}

/**
  This routine sets smb_tsod_poll_rate to the given value

  @param[in] Socket        - Socket Id
  @param[in] SmbInstance   - SMBus Instance number
  @param[in] TsodPollRate  - TsodPollRate value to set

  @retval VOID
**/
VOID
SetTsodPollRateSmb (
  IN UINT8   Socket,
  IN UINT8   SmbInstance,
  IN UINT32  TsodPollRate
  )
{
  return;
}

/**
  This routine updates the Tsod population based on DIMM types

  @param[in] Socket   Socket ID
  @param[in] McId     Memory controller ID

  @retval VOID
**/
VOID
UpdateTsodPopulation (
  IN UINT8  Socket,
  IN UINT8  McId
  )
{
  return;
}

/**
  This routine compares RegOffset to I3C command registers

  @param[in] RegOffset  - Register address

  @retval 1  for match
  @retval 0  for no match
**/
UINT32
IsRegCmdSmb (
  IN UINT32  RegOffset
  )
{
  return 0;
}

/**
  This routine checks if the offset belongs to SMB

  @param[in] RegOffset  - Register address

  @retval 1  for pass
  @retval 0  for no pass
**/
UINT32
IsRegFilterPassSmb (
  IN UINT32  RegOffset
  )
{
  if ((GetUbiosOutputMode () == ASM_OUTPUT_ENABLE_ON_SMBUS) ||
      ((GetUbiosOutputMode () == ASM_OUTPUT_ENABLE)
       && !(((CSR_OFFSET *)&RegOffset)->Bits.boxtype == BOX_SB_I3C)
      )) {
    return 1;
  } else {
    return 0;
  }
}

/**
  This routine gets processor smbus data register

  @param[in] Socket       - Socket Id
  @param[in] SmbInstance  - Processor Smbus Instance
  @param[in] RegSize      - Pointer to size of register

  @retval  register address
**/
UINT32
GetProcSmbDataReg (
  IN UINT8   Socket,
  IN UINT16  SmbInstance,
  IN UINT8   *RegSize
  )
{
  UINT8     *RegAddr = 0;
  UINT32    I3cDataReg;

  if (SmbInstance == 0) {
    I3cDataReg = DATA_PORT_SB_I3C0_REG;
  } else {
    I3cDataReg = DATA_PORT_SB_I3C1_REG;
  }

  RegAddr = (UINT8 *) UsraGetCsrRegisterAddress (Socket, (UINT8) SmbInstance, I3cDataReg);

  return (UINT32) (UINTN) RegAddr;
}

/**
  This routine sets tsod smbus slave address for reading

  @param[in] Socket       - Socket Id
  @param[in] SmbInstance  - Processor Smbus Instance
  @param[in] TsodSa       - Tsod Slave address

  @retval  VOID
**/
VOID
DoProcSmbTsodRead (
  IN UINT8   SktId,
  IN UINT8   SmbInstance,
  IN UINT32  TsodSa
  )
{
  // Unused in PEI
  return;
}

/**
  This routine gets smbus status register

  @param[in] SmbInstance  - Processor Smbus Instance

  @retval  register address
**/
UINT32
GetProcSmbStatReg (
  IN UINT8  SmbInstance
  )
{
  // Unused in PEI
  return 0;
}

/**
  This routine gets smbus command register

  @param[in] SmbInstance  - Processor Smbus Instance

  @retval register address
**/
UINT32
GetProcSmbCmdReg (
  IN UINT8  SmbInstance
  )
{
  // Unused in PEI
  return 0;
}

/**
  This routine gets smbus error bit

  @param[in] SktId    - Socket number
  @param[in] SmbInstance  - Processor Smbus Instance

  @retval  Smbus error bit value
**/
UINT32
GetProcSmbSbe (
  IN UINT8  SktId,
  IN UINT8  SmbInstance
  )
{
  // Unused in PEI
  return 0;
}

/**
  This routine gets smbus clock override bit

  @param[in] SktId        - Socket number
  @param[in] SmbInstance  - Processor Smbus Instance

  @retval Smbus clock override bit value
**/
UINT32
GetProcSmbCkovrd (
  IN UINT8  SktId,
  IN UINT8  SmbInstance
  )
{
  // Unused in PEI
  return 0;
}

/**
  This routine sets smbus soft reset bit

  @param[in] SktId        - Socket number
  @param[in] SmbInstance  - Processor Smbus Instance
  @param[in] Softrst      - Smbus soft reset bit value

  @retval  VOID
**/
VOID
SetProcSmbSoftrst (
  IN UINT8   SktId,
  IN UINT8   SmbInstance,
  IN UINT32  Softrst
  )
{
  // Unused in PEI
  return;
}

/**
  This routine gets information on TSOD slave address, Tsod polling bit
  and Smbus recover enable bit info

  @param[in] SktId        - Socket number
  @param[in] SmbInstance  - Processor Smbus Instance
  @param[out] TsodSa      - Tsod Slave Address
  @param[out] TsodPollEn  - Tsode Polling enable bit info
  @param[out] SbeEn       - SMBus error recovery enable bit

  @retval  VOID
**/
VOID
GetProcSmbTsodSaTsodPollEnSbeEn (
  IN UINT8    SktId,
  IN UINT8    SmbInstance,
  OUT UINT32  *TsodSa,
  OUT UINT32  *TsodPollEn,
  OUT UINT32  *SbeEn
  )
{
  // Unused in PEI
  return;
}

/**
  This routine sets information on Tsod polling bit
  and Smbus recover enable bit info

  @param[in] SktId       - Socket number
  @param[in] SmbInstance - Processor Smbus Instance
  @param[in] TsodPollEn  - Tsode Polling enable bit info
  @param[in] SbeEn       - SMBus error recovery enable bit

  @retval  VOID
**/
VOID
SetProcSmbTsodPollEnSbeEn (
  IN UINT8   SktId,
  IN UINT8   SmbInstance,
  IN UINT32  TsodPollEn,
  IN UINT32  SbeEn
  )
{
  // Unused in PEI
  return;
}

/**
  This routine get smi enable on SBE bit.

  @param[in] SktId       - Socket number
  @param[in] SmbInstance - Processor Smbus Instance

  @retval Smbus SmiEn bit value
**/
UINT32
GetProcSmbSmiEn (
  IN UINT8  SktId,
  IN UINT8  SmbInstance
  )
{
  return 0;
}

/**
  This routine sets smi enable on SBE bit. Also sets\resets SBE (smbus error recovery)
  enable bit.

  @param[in] SktId       - Socket number
  @param[in] SmbInstance - Processor Smbus Instance
  @param[in] SmiEn       - Smi enable on SBE error
  @param[in] SbeEn       - SBE Enable

  @retval  VOID
**/
VOID
SetProcSmbSmiEnSbeEn (
  IN UINT8   SktId,
  IN UINT8   SmbInstance,
  IN UINT32  SmiEn,
  IN UINT32  SbeEn
  )
{
  // Unused in PEI
  return;
}

/**
  Prepares the Smbus Recovery Operation by disabling TSOD polling
  and Smbus recovery enable.

  @param[in] SktId       - Socket Id
  @param[in] SmbInstance - Instance

  @retval  VOID
**/
VOID
DisableTsodAndSmbSbeEn (
  IN UINT8  SktId,
  IN UINT8  SmbInstance
  )
{
  // Unused in PEI
  return;
}

/**
  Triggers the Smbus Recovery operation.

  @param[in] SktId       - Socket number
  @param[in] SmbInstance - Instance

  @retval VOID
**/
VOID
TriggerSmbRecovery (
  IN UINT8   SktId,
  IN UINT8   SmbInstance
  )
{
  // Unused in PEI
  return;
}

/**
  Performs the Smbus Recovery operation.

  @param[in] SktId       - Socket number
  @param[in] SmbInstance - Instance

  @retval VOID
**/
VOID
DoProcSmbRecovery (
  IN UINT8   SktId,
  IN UINT8   SmbInstance
  )
{
  // Unused in PEI
  return;
}

/**
  Returns Smbus Instance for given McId, by mapping Instance to
  McId

  @param[in] McId  - Memory Controller Id, 0 based

  @retval UINT8  Instance Number
**/
UINT8
EFIAPI
GetProcSmbInstanceFromMcId (
  IN UINT8  McId
  )
{
  return GetProcSmbInstanceFromMcIdCommon (McId);
}

/**
  I3C Send CCC Commands.

  Directed CCC Write/Read Transfer
  Data Flow Representation of Directed CCC Transfer with IC_HAS_DAT=0
  1. Enqueue the Immediate/Regular Transfer command with the required field settings.
  2. The DWC_mipi_i3c controller will generate the Directed CCC transfer on the line to the device with the
   address of SLV_ADDR field and maintains the timing based on the MODE field in the command and issues
   data payload (if present) retrieved from the command.
  3. The DWC_mipi_i3c controller terminates the Directed CCC transfer either when BYTE_CNT/DATA_LENGTH
   expires or upon receiving NACK for Broadcast address or for dynamic address.
  4. The DWC_mipi_i3c Controller generates the response for the enqueued Directed CCC transfer command in the
   Response Queue with the Error status (if any) and the Remaining Data Length of the transfer if the write transfer
   terminated before the BYTE_CNT/DATA_LENGTH expires or the received data length in case of read transfer.

  Broadcast CCC Write Transfer
  Data Flow Representation of Broadcast CCC Transfer with IC_HAS_DAT=0
  1. Enqueue the Immediate Transfer/Regular Transfer command with the required field.
  2. The DWC_mipi_i3c controller will generate the Broadcast CCC transfer on the line and issues data payload (if
     present) retrieved from the command.
  3. The DWC_mipi_i3c controller terminates the Broadcast CCC transfer either on BYTE_CNT/DATA_LENGTH
     expires or upon receiving NACK for Broadcast address.
  4. The DWC_mipi_i3c Controller generates the response of the enqueued Broadcast CCC transfer command in the Response
     Queue with the Error status (if any) and the Remaining Data Length of the transfer if the transfer terminated before the
     BYTE_CNT/DATA_LENGTH expires.

  @param[in]  Socket      - Socket Id
  @param[in]  Dev         - Device address on I3C
  @param[in]  CccCommand  - CCC Command
  @param[in]  Attribute   - Attribute for Read/Write CCC/None-CCC
  @param[out] Data        - Pointer to Data to store the value
  @param[out] DataLength  - Pointer to the length, in bytes, of Data

  @retval EFI_SUCCESS           - Successfully Send CCC commands
  @retval EFI_DEVICE_ERROR      - Send CCC failed
  @retval EFI_UNSUPPORTED       - Function not supported
  @retval EFI_INVALID_PARAMETER - Invalid Parameter
**/
EFI_STATUS
EFIAPI
SmbSendCccCommandWorker (
  IN     UINT8             Socket,
  IN     SMB_DEVICE_STRUCT Dev,
  IN     UINT8             CccCommand,
  IN     SMB_ATTR_FLAG     Attribute,
  IN OUT UINT8             *Data,
  IN OUT UINT8             *DataLength
  )
{
  EFI_STATUS                              Status;
  UINT32                                  I3cStatusReg;
  UINT32                                  I3cCmdReg;
  UINT32                                  I3cResetReg;
  UINT32                                  I3cQueueCtrlReg;
  UINT32                                  I3cDataReg;
  UINT32                                  I3cRespReg;
  UINT32                                  Timeout;
  RESPONSE_QUEUE_PORT_SB_I3C0_STRUCT      I3cResp;
  BOOLEAN                                 ShouldBreak = FALSE;
  UINT8                                   Div4Quotient;
  UINT8                                   Div4Remainder;
  UINT8                                   QuoIndex;
  UINT8                                   RemIndex;
  UINT8                                   RemLen;
  UINT8                                   TransactionID;
  UINT8                                   Instance;
  UINT8                                   ReadOrWrite;
  UINT8                                   CmdPresent;
  UINT8                                   Toc;
  UINT8                                   Roc;
  COMMAND_QUEUE_PORT_SB_I3C0_STRUCT       CmdPort;
  REGULAR_DATA_TRANSFER_COMMAND_LOW_WITHOUT_DAT  RegularCommandLow;
  REGULAR_DATA_TRANSFER_COMMAND_HIGH_WITHOUT_DAT RegularCommandHigh;
  // Same address with RX_DATA_PORT and TX_DATA_PORT, single bi-direction design
  volatile DATA_PORT_SB_I3C0_STRUCT       DataPort;
  INTR_STATUS_SB_I3C0_STRUCT              I3cStatus;
  UINT32                                  I3cDataCtrlReg;
  DATA_BUFFER_THLD_CTRL_SB_I3C0_STRUCT    I3cDataThld;
  UINT32                                  I3cDevCtrlReg;
  DEVICE_CONTROL_SB_I3C0_STRUCT           I3cDevCtrl;

  Status = EFI_SUCCESS;
  TransactionID = TidCcc;
  Instance = (UINT8) Dev.address.SmbInstance;

  CountTrackingData (PTC_SMBUS_READ, 1);

  if (Instance == 0) {
    I3cCmdReg = COMMAND_QUEUE_PORT_SB_I3C0_REG;
    I3cResetReg = RESET_CTRL_SB_I3C0_REG;
    I3cQueueCtrlReg = QUEUE_THLD_CTRL_SB_I3C0_REG;
    I3cStatusReg = INTR_STATUS_SB_I3C0_REG;
    I3cDataReg = DATA_PORT_SB_I3C0_REG;  // Use DATA_PORT for reading and writing
    I3cRespReg = RESPONSE_QUEUE_PORT_SB_I3C0_REG;
    I3cDataCtrlReg = DATA_BUFFER_THLD_CTRL_SB_I3C0_REG;
    I3cDevCtrlReg = DEVICE_CONTROL_SB_I3C0_REG;
  } else {
    I3cCmdReg = COMMAND_QUEUE_PORT_SB_I3C1_REG;
    I3cResetReg = RESET_CTRL_SB_I3C1_REG;
    I3cQueueCtrlReg = QUEUE_THLD_CTRL_SB_I3C1_REG;
    I3cStatusReg = INTR_STATUS_SB_I3C1_REG;
    I3cDataReg = DATA_PORT_SB_I3C1_REG;  // Use DATA_PORT for reading and writing
    I3cRespReg = RESPONSE_QUEUE_PORT_SB_I3C1_REG;
    I3cDataCtrlReg = DATA_BUFFER_THLD_CTRL_SB_I3C1_REG;
    I3cDevCtrlReg = DEVICE_CONTROL_SB_I3C1_REG;
  }

  //
  // determine the Read no Write field identifies direction of the transfer
  //
  switch (Attribute) {
  case AttrCccRead:
    ReadOrWrite = RNW_READ;
    CmdPresent = CP_CCC_HDR;
    Roc = ROC_REQUIRED;
    Toc = TOC_STOP;
    break;

  case AttrCccWrite:
    ReadOrWrite = RNW_WRITE;
    CmdPresent = CP_CCC_HDR;
    Roc = ROC_REQUIRED;
    Toc = TOC_STOP;
    break;

  case AttrNormalRead:
    ReadOrWrite = RNW_READ;
    CmdPresent = CP_TRANFSER;
    Roc = ROC_REQUIRED;
    Toc = TOC_STOP;
    break;

  case AttrNormalWrite:
    ReadOrWrite = RNW_WRITE;
    CmdPresent = CP_TRANFSER;
    Roc = ROC_REQUIRED;
    Toc = TOC_STOP;
    break;

  case AttrNormalWriteReStartRead:
    ReadOrWrite = RNW_WRITE;
    CmdPresent = CP_TRANFSER;
    Roc = ROC_NOT_REQUIRED;
    Toc = TOC_RESTART;
    break;

  default:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "ERROR: I3C unsupported Attribute %d\n", Attribute);
    return EFI_INVALID_PARAMETER;
    break;
  }

  //
  // Set tx_start_thld = 2 and iba_include = 0 before RCD read
  // do not send write command until whole write payload is written to tx_data_port
  //
  if ((Attribute == AttrNormalWriteReStartRead) || (Attribute == AttrNormalWrite)) {
    I3cDataThld.Data = UsraCsrRead (Socket, Instance, I3cDataCtrlReg);
    //set TX Start THLD to 2  [18:16] 010 - 8
    I3cDataThld.Bits.tx_start_thld = 2;
    UsraCsrWrite (Socket, Instance, I3cDataCtrlReg, I3cDataThld.Data);

    I3cDevCtrl.Data = UsraCsrRead (Socket, Instance, I3cDevCtrlReg);
    I3cDevCtrl.Bits.iba_include = 0;
    UsraCsrWrite (Socket, Instance, I3cDevCtrlReg, I3cDevCtrl.Data);
  }

  //
  // Form read/write command
  //
  RegularCommandLow.Data = 0x0;
  RegularCommandLow.Bits.com_attr = I3C_COM_ATTR_XFER;  // I3C_COM_ATTR_XFER  0x0 Regular Transfer
  RegularCommandLow.Bits.tid = TransactionID; // Transaction ID field is used as identification tag for the command.
  RegularCommandLow.Bits.i2cni3c = GetSlaveType (Socket, Instance); // 0x0: I3C device 0x1: I2C device
  if (CmdPresent == CP_CCC_HDR) {
    RegularCommandLow.Bits.cmd = CccCommand;
  }
  RegularCommandLow.Bits.cp = CmdPresent;

  // Broadcast CCC do not care slave_address field
  if (CccCommand >= MAX_CCC_CODE_BROADCAST) {  // Dirct CCC
    RegularCommandLow.Bits.slave_address = (UINT32) ((Dev.address.deviceType << 3) | Dev.address.strapAddress);
  }

  // 0x0: I3C SDR0/ I2C Fm: Note: I3C SDR0 represents standard SDR Speed (0 to fSCL Max)
  RegularCommandLow.Bits.mode_speed = GetModeSpeed (Socket, Instance);  // All CCC transfers will be initiated with SDR0 Speed
  RegularCommandLow.Bits.rnw = ReadOrWrite; // 0x1: READ: Read transfer
  RegularCommandLow.Bits.roc = Roc; // Response Status is required/not required
  RegularCommandLow.Bits.toc = Toc; // Repeated Start (Sr) / STOP

  RegularCommandHigh.Data = 0x0;
  RegularCommandHigh.Bits.data_length = *DataLength;

  CmdPort.Data = 0x0;
  CmdPort.Bits.command = RegularCommandLow.Data;

  //
  // Send command
  //
  // Low 32 bits
  Status = WaitForHostNotBusyTarget (Socket, Instance, I3cStatusReg);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  PollSmbusStatusRegister (Socket, Instance, I3cStatusReg, I3C_CMD_QUEUE_READY_BIT_MASK, I3C_CMD_QUEUE_READY_POLLING_LOGIC);

  UsraCsrWrite (Socket, Instance, I3cCmdReg, CmdPort.Bits.command);

  // High 32 bits
  // wait for there is space in the command queue
  CmdPort.Bits.command = RegularCommandHigh.Data;
  Status = WaitForHostNotBusyTarget (Socket, Instance, I3cStatusReg);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  PollSmbusStatusRegister (Socket, Instance, I3cStatusReg, I3C_CMD_QUEUE_READY_BIT_MASK, I3C_CMD_QUEUE_READY_POLLING_LOGIC);

  UsraCsrWrite (Socket, Instance, I3cCmdReg, CmdPort.Bits.command);

  if (Attribute == AttrNormalWriteReStartRead) {

    RegularCommandLow.Bits.roc = ROC_REQUIRED;
    RegularCommandLow.Bits.toc = TOC_STOP;
    RegularCommandLow.Bits.rnw = RNW_READ;

    CmdPort.Data = 0x0;
    CmdPort.Bits.command = RegularCommandLow.Data;

    //
    // Send command
    //
    // Low 32 bits
    Status = WaitForHostNotBusyTarget (Socket, Instance, I3cStatusReg);

    if (EFI_ERROR (Status)) {
      return Status;
    }

    PollSmbusStatusRegister (Socket, Instance, I3cStatusReg, I3C_CMD_QUEUE_READY_BIT_MASK, I3C_CMD_QUEUE_READY_POLLING_LOGIC);

    UsraCsrWrite (Socket, Instance, I3cCmdReg, CmdPort.Bits.command);

    // High 32 bits
    // wait for there is space in the command queue
    CmdPort.Bits.command = RegularCommandHigh.Data;
    Status = WaitForHostNotBusyTarget (Socket, Instance, I3cStatusReg);

    if (EFI_ERROR (Status)) {
      return Status;
    }

    PollSmbusStatusRegister (Socket, Instance, I3cStatusReg, I3C_CMD_QUEUE_READY_BIT_MASK, I3C_CMD_QUEUE_READY_POLLING_LOGIC);

    UsraCsrWrite (Socket, Instance, I3cCmdReg, CmdPort.Bits.command);
  }

  //
  // data to write.
  // if com_attr is Immediate transfer command, populate data in Immediate command; else push data to transmit fifo.
  // program the data port when Data Length is not 0 and (BroadCast CCC or Direct CCC with Read attribute)
  //
  if ((*DataLength > 0) && (ReadOrWrite == RNW_WRITE)) {

    // There are 4 bytes per one Data Port
    Div4Quotient = (*DataLength / DATA_LEN_I3C_PORT);
    Div4Remainder = (*DataLength % DATA_LEN_I3C_PORT);
    if (Div4Remainder != 0) {
      Div4Quotient += 1;
    }

    for (QuoIndex = 0; QuoIndex < Div4Quotient; QuoIndex++) {

      // All 4 bytes are valid if it is not the last Data Port to read
      if ((Div4Remainder != 0) && (QuoIndex == (Div4Quotient - 1))) {
        RemLen = Div4Remainder;
      } else {
        RemLen = DATA_LEN_I3C_PORT;
      }

      DataPort.Data = 0;
      for (RemIndex = 0; RemIndex < RemLen; RemIndex++) {
        DataPort.Bits.data_port |= ((UINT32) *(Data + (DATA_LEN_I3C_PORT * QuoIndex + RemIndex)) << (8 * RemIndex));
      }

      //
      // check if there is space in the tx_fifo
      //
      Timeout = I3C_TIMEOUT;

      do {
        I3cStatus.Data = UsraCsrRead (Socket, Instance, I3cStatusReg);

        if (GetEmulation () & SIMICS_FLAG) {
          break;
        }

        BreakForSmbDataTarget (&ShouldBreak);

        if (ShouldBreak) {
          break;
        }

        MicroSecondDelay (1);
        Timeout--;

        if (Timeout <= 0) {
          Status = EFI_TIMEOUT;
          break;
        }

      } while ((I3cStatus.Bits.tx_thld_stat == 0) && (I3cStatus.Bits.transfer_err_stat == 0));

      PollSmbusStatusRegister (Socket, Instance, I3cStatusReg, I3C_WRITE_DONE_BIT_MASK, I3C_WRITE_DONE_POLLING_LOGIC);
      PollSmbusStatusRegister (Socket, Instance, I3cStatusReg, I3C_TRANSFER_ERROR_BIT_MASK, I3C_TRANSFER_ERROR_POLLING_LOGIC);

      //
      // Write the Data
      //
      UsraCsrWrite (Socket, Instance, I3cDataReg, DataPort.Data);
    }
  }

  if (Attribute == AttrNormalWriteReStartRead) {
    // Write then Read
    ReadOrWrite = RNW_READ;
  }

  //
  // Transfer Complete or not INTR_STATUS[4]
  //
  if (!EFI_ERROR (Status)) {
    Status = WaitForWriteToCompleteTarget (Socket, Instance, I3cStatusReg);
    PollSmbusStatusRegister (Socket, Instance, I3cStatusReg, I3C_RESPONSE_READY_BIT_MASK, I3C_RESPONSE_READY_POLLING_LOGIC);
  }

  // No Read BroadCast CCC
  if ((*DataLength > 0) && (ReadOrWrite == RNW_READ)) {

    //
    // For Read, Wait for the read data
    //
    Timeout = I3C_TIMEOUT;

    do {
      I3cStatus.Data = UsraCsrRead (Socket, Instance, I3cStatusReg);

      CountTrackingData (PTC_POLLING_COUNT, 1);

      BreakForSmbDataTarget (&ShouldBreak);

      if (ShouldBreak) {
        break;
      }

      MicroSecondDelay (1);
      Timeout--;

      if (Timeout <= 0) {
        Status = EFI_TIMEOUT;
        break;
      }

    } while ((I3cStatus.Bits.rx_thld_stat == 0) && (I3cStatus.Bits.transfer_err_stat == 0));

    PollSmbusStatusRegister (Socket, Instance, I3cStatusReg, I3C_READ_DONE_BIT_MASK, I3C_READ_DONE_POLLING_LOGIC);
    PollSmbusStatusRegister (Socket, Instance, I3cStatusReg, I3C_TRANSFER_ERROR_BIT_MASK, I3C_TRANSFER_ERROR_POLLING_LOGIC);

    //
    // If read data is valid
    //
    if ((I3cStatus.Bits.rx_thld_stat == 1) && (I3cStatus.Bits.transfer_err_stat == 0)) {

      Div4Quotient = (*DataLength / DATA_LEN_I3C_PORT);
      Div4Remainder = (*DataLength % DATA_LEN_I3C_PORT);
      if (Div4Remainder != 0) {
        Div4Quotient += 1;
      }

      for (QuoIndex = 0; QuoIndex < Div4Quotient; QuoIndex++) {
        if ((Div4Remainder != 0) && (QuoIndex == (Div4Quotient - 1))) {
          RemLen = Div4Remainder;
        } else {
          RemLen = DATA_LEN_I3C_PORT;
        }

        DataPort.Data = UsraCsrRead (Socket, Instance, I3cDataReg);

        for (RemIndex = 0; RemIndex < RemLen; RemIndex++) {
          *(Data + (DATA_LEN_I3C_PORT * QuoIndex + RemIndex)) = (UINT8) (DataPort.Bits.data_port >> (8 * RemIndex));
        }
      }

    } else {
      Status = EFI_DEVICE_ERROR;
    }
  }

  //
  // Check response status
  //
  Timeout = I3C_TIMEOUT;

  do {
    I3cResp.Data = UsraCsrRead (Socket, Instance, I3cRespReg);

    if (GetEmulation () & SIMICS_FLAG) {
      break;
    }

    BreakForSmbDataTarget (&ShouldBreak);

    if (ShouldBreak) {
      break;
    }

    MicroSecondDelay (1);
    Timeout--;

    if (Timeout <= 0) {
      Status = EFI_TIMEOUT;
      break;
    }
  } while ((I3cResp.Bits.tid != TransactionID) || I3cResp.Bits.err_status);

  //
  // Restore tx_start_thld = 0 and iba_include = 1 after RCD read
  //
  if ((Attribute == AttrNormalWriteReStartRead) || (Attribute == AttrNormalWrite)) {
    //set TX Start THLD to 0 [18:16] 000 - 1
    I3cDataThld.Bits.tx_start_thld = 0;
    UsraCsrWrite (Socket, Instance, I3cDataCtrlReg, I3cDataThld.Data);

    I3cDevCtrl.Bits.iba_include = 1;
    UsraCsrWrite (Socket, Instance, I3cDevCtrlReg, I3cDevCtrl.Data);
  }

  return Status;
}

/**
  I3C Send CCC Commands.

  @param[in]  Socket      - Socket Id
  @param[in]  Dev         - Device address on I3C
  @param[in]  CccCommand  - CCC Command
  @param[in]  Attribute   - Attribute for Read/Write CCC/None-CCC
  @param[out] Data        - Pointer to Data to store the value
  @param[out] DataLength  - Pointer to the length, in bytes, of Data

  @retval EFI_SUCCESS           - Successfully Send CCC commands
  @retval EFI_DEVICE_ERROR      - Send CCC failed
  @retval EFI_UNSUPPORTED       - Function not supported
  @retval EFI_INVALID_PARAMETER - Invalid Parameter
**/
EFI_STATUS
EFIAPI
SmbSendCccCommand (
  IN     UINT8             Socket,
  IN     SMB_DEVICE_STRUCT Dev,
  IN     UINT8             CccCommand,
  IN     SMB_ATTR_FLAG     Attribute,
  IN OUT UINT8             *Data,
  IN OUT UINT8             *DataLength
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  Status = SmbSendCccCommandWorker (Socket, Dev, CccCommand, Attribute, Data, DataLength);

  // Retry if error detected
  if (EFI_ERROR (Status)) {
    ResetProcSmb (Socket, Dev.address.SmbInstance);
    Status = SmbSendCccCommandWorker (Socket, Dev, CccCommand, Attribute, Data, DataLength);
  }

  return Status;
}

/**
  SwitchToI3cModeWorker.

  The SETAASA CCC is only supported when device is in I2C mode. When SETAASA CCC is registered by the SPD5
  Hub, it updates Table 117, "MR18" [5] = '1'. Table 71 shows an example of a single SETAASA CCC.
  SETAASA CCC does not support PEC function as device is in I2C mode and there is no PEC function in I2C mode.

  @param[in] Socket      - Socket Id
  @param[in] Dev         - Device address on I3C

  @rval EFI_SUCCESS  Successfully Switch To I3c Mode
**/
EFI_STATUS
EFIAPI
SwitchToI3cModeWorker (
  IN  UINT8              Socket,
  IN  SMB_DEVICE_STRUCT  Dev
  )
{
  EFI_STATUS             Status = EFI_SUCCESS;
  UINT8                  Data;
  UINT8                  DataLength;

  SetI3cBusMode (Socket, (UINT8) Dev.address.SmbInstance, I2C_MODE);

  //
  // Issue SETHID CCC to update the 3-bit HID code in I3C mode
  //
  Data = 0;
  DataLength = SETHID_DATA_LENGTH;
  Status = SmbSendCccCommand (Socket, Dev, CCC_BROADCAST_SETHID, AttrCccWrite, &Data, &DataLength);

  if (EFI_ERROR (Status)) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "I3C Instance %d: Issue SETHID CCC failed!\n", Dev.address.SmbInstance);
  } else {

    //
    // Broadcast commands donot need the slave address.
    //
    Data = 0;
    //
    // Apply simics WA when UbiosGeneration is not Enabled, as simics will crash in WB
    //
    if (IsSiliconWorkaroundEnabled ("S1507820821") && !UbiosGenerationEnabled ()) {
      DataLength = 1;
    } else {
      DataLength = BROADCAST_DATA_LENGTH;
    }
    Status = SmbSendCccCommand (Socket, Dev, CCC_BROADCAST_SETAASA, AttrCccWrite, &Data, &DataLength);

    if (EFI_ERROR (Status)) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "I3C Instance %d: Issue SETAASA CCC failed!\n", Dev.address.SmbInstance);
    } else {
      Status = SetI3cBusMode (Socket, (UINT8) Dev.address.SmbInstance, I3C_MODE);
    }
  }

  return Status;
}

/**
  Is Rambus version D0 or D1 Present per Socket

  @param[in]      Socket      - Socket number

  @retval TRUE  - Rambus D0 or D1 Present
          FALSE - Rambus D0 or D1 not Present
*/
BOOLEAN
EFIAPI
IsRambusD0D1Present (
  IN  UINT8  Socket
  )
{
  PSYSHOST            Host;
  UINT8               MaxChDdr;
  UINT8               Ch;
  UINT8               Dimm;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8               RcdRevision;
  UINT16              RcdVendor;
  BOOLEAN             PresentFlag = FALSE;

  Host = (PSYSHOST) GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      RcdVendor = (*DimmNvList)[Dimm].SPDRegVen;
      RcdRevision = (*DimmNvList)[Dimm].SPDRegRev;

      // Check if Rambus version D0 or D1 present
      if ((RcdVendor == MFGID_RAMBUS) && ((RcdRevision == RAMBUS_SPD_REGREV_D0) || (RcdRevision == RAMBUS_SPD_REGREV_D1))) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "Donot enable I3C mode if Rambus version D0 or D1 present\n");
        PresentFlag = TRUE;
        break;
      }
    }
  }

  return PresentFlag;
}

/**
  I3cSwitchToI3cMode.

  Switch To I3c Mode for every I3C instance

  @param[in] Socket        - Socket Id

  @rval EFI_SUCCESS       Successfully Switch To I3c Mode
  @rval EFI_DEVICE_ERROR  Cannot Switch To I3c Mode
  @rval EFI_UNSUPPORTED   Do not support Switch To I3c Mode
**/
EFI_STATUS
EFIAPI
I3cSwitchToI3cMode (
  IN  UINT8    Socket
  )
{
  EFI_STATUS          Status = EFI_DEVICE_ERROR;
  SYS_SETUP           *Setup;
  SMB_DEVICE_STRUCT   Spd;
  UINT8               SmbInstance;
  BOOLEAN             SkipI3cMode;
  UINT8               MaxChDdr;
  UINT8               OriginalUbiosOutputMode;

  Setup = GetSysSetupPointer ();
  SkipI3cMode = IsRambusD0D1Present (Socket);

  //
  // Switch To I3C native Mode for SPR
  // Do not enable I3C mode if system has Rambus version D0 or D1 present
  //
  if (Setup->mem.dfxMemSetup.DfxSmbusI3cModeEnable) {

    ZeroMem (&Spd, sizeof (Spd));
    MaxChDdr = GetMaxChDdr ();
    OriginalUbiosOutputMode = EnableI3cAsmOutput (TRUE);

    for (SmbInstance = 0; SmbInstance < MAX_SMB_INSTANCE; SmbInstance++) {

      // Skip programming I3C1 in 1 die config
      if (I3C_CH_NUMBER * SmbInstance >= MaxChDdr) {
        continue;
      }

      // Broadcast CCC only care about the Instance number and CCC code
      Spd.address.SmbInstance = SmbInstance;

      if (SkipI3cMode) {
        Status = EFI_UNSUPPORTED;
      } else {
        // Override to I2C timing before send SETHID and SETAASA
        ProgramProcSmbFreq (Socket, SmbInstance, I2C_MODE);
        Status = SwitchToI3cModeWorker (Socket, Spd);
        ProgramProcSmbFreq (Socket, SmbInstance, I3C_MODE);
      }

      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "I3C Instance %d: Switch to I3C mode - Status = %r\n", SmbInstance, Status);

      //
      // When the Bus is failed to switch to I3C mode, set i2c_slave_present
      //
      if (EFI_ERROR (Status)) {
        SetI2cSlavePresent (Socket, SmbInstance, TRUE);
      }
    }

    RestoreI3cAsmOutput (OriginalUbiosOutputMode, TRUE);
  }

  return Status;
}
