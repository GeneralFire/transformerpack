/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>
#include <SysHostChip.h>
#include <Chip/Include/CpuPciAccess.h>
#include "Chip10nm/Include/MemHostChip10nm.h"
#include <Chip/Include/FnvAccessCommon.h>
#include <UncoreCommonIncludes.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/CheckpointLib.h>
#include <Library/BaseLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/SysHostPointerLib.h>
#include "MemCmdControl.h"
#include "Include/MemIoControl.h"
#include "Include/MemPxc.h"
#include "Include/MemXoverCalib.h"
#include "Include/MemDdrioRegs.h"
#include "Include/MemDdrioPrivate.h"
#include <Include/MemDdrioSpecific.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CteNetLib.h>
#include <Library/MspChipLib.h>
#include <Library/MemDdrIoIpTargetLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/OdtTableLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemRoutingLib.h>
#include <Library/SwizzleLib.h>
#include <Memory/JedecDefinitions.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/MemRcLib.h>
#include "Common/Include/MemDdrioIpLibDefinitions.h"
#include <MemProjectSpecific.h>
#include <Library/Ddr5CoreLib.h>
#include <Library/PerformanceTrackerLib.h>
#include <Memory/Ddr5MrRegs.h>
#include <Memory/Ddr5Cmds.h>
#include <Memory/Ddr5DbRegs.h>

#define MAX_TX_DQ_DELAY                      2048
#define UINT16_MINUS_COMPENSATION_5_BITS     0xFFC0   // Minus compensation for UINT16 value where bit5 indicates a negative sign
//
// Internal data types
//


//
// Per-DIMM round-trip register offsets. Multiple per-DIMM arrays are used rather than a two-dimensional array or a
// single, flattened array in order to make bounds checking more deliberate and less error-prone.
//

CONST UINT32 mDimm0RoundTripRegOffsets[] = {
  DDRINTF_RT_LAT0_CH_0_MCIO_DDRIO_REG, // Ch0A
  DDRINTF_RT_LAT0_CH_1_MCIO_DDRIO_REG, // Ch0B
  DDRINTF_RT_LAT0_CH_2_MCIO_DDRIO_REG, // Ch1A
  DDRINTF_RT_LAT0_CH_3_MCIO_DDRIO_REG  // Ch1B
};

#ifdef DDR5_SUPPORT

//
// SPR DDR5 DIMM1 Rank mapping:
//   DIMM1 Rank 0   -   DDRIO Rank 2
//   DIMM1 Rank 1   -   DDRIO Rank 3
// So SPR DDR5 DIMM1 uses rt_lat2 and rt_lat3 of DDRINTF_RT_LAT0_xx.
//
CONST UINT32 mDimm1RoundTripRegOffsets[] = {
  DDRINTF_RT_LAT0_CH_0_MCIO_DDRIO_REG, // Ch0A
  DDRINTF_RT_LAT0_CH_1_MCIO_DDRIO_REG, // Ch0B
  DDRINTF_RT_LAT0_CH_2_MCIO_DDRIO_REG, // Ch1A
  DDRINTF_RT_LAT0_CH_3_MCIO_DDRIO_REG  // Ch1B
};
#define DIMM_ROUND_TRIP_REGS          1
#else

//
// DDR4 DIMM1 Rank mapping:
//   DIMM1 Rank 0   -   DDRIO Rank 4
//   DIMM1 Rank 1   -   DDRIO Rank 5
//   DIMM1 Rank 2   -   DDRIO Rank 6
//   DIMM1 Rank 3   -   DDRIO Rank 7
// DDR4 DIMM1 uses rt_lat4, rt_lat5, rt_lat6 and rt_lat7 of DDRINTF_RT_LAT1_xx.
//
CONST UINT32 mDimm1RoundTripRegOffsets[] = {
  DDRINTF_RT_LAT1_CH_0_MCIO_DDRIO_REG, // Ch0A
  DDRINTF_RT_LAT1_CH_1_MCIO_DDRIO_REG, // Ch0B
  DDRINTF_RT_LAT1_CH_2_MCIO_DDRIO_REG, // Ch1A
  DDRINTF_RT_LAT1_CH_3_MCIO_DDRIO_REG  // Ch1B
};
#define DIMM_ROUND_TRIP_REGS          2
#endif

#define DIMM_ROUND_TRIP_RANKS_PER_REG 4
#define DIMM_ROUND_TRIP_RANKS         (DIMM_ROUND_TRIP_RANKS_PER_REG * DIMM_ROUND_TRIP_REGS)


//
// Rcomp static leg training defines
//

// Initial legs value - all 15 static legs eanbled
#define STATIC_LEG_CTL_INIT   31
// 11 static legs enabled when 15 doesn't work
#define STATIC_LEG_CTL_ADJUST 23

//
// upper and lower desired range resistor value limits for rcomp static leg training
//
#define LOWER_RCOMP_CODE_VALUE 4
#define MAX_CC_PIN_NUM_SUBCH   22

extern UINT32 ClkAddressConvertTable[MAX_CH];
extern VOID
EnableForceSaOn (
  IN PSYSHOST Host
  );

//
// Local Prototypes
//
STATIC UINT32 ProgramIOCompValues(PSYSHOST Host, UINT8 socket);
STATIC MRC_STATUS GetSetTxDelayBit(PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe, UINT8 bit, MRC_LT level,
             MRC_GT group, UINT8 mode, INT16 *value);
STATIC MRC_STATUS GetSetTxVref (PSYSHOST Host, UINT8 Socket, UINT8 Ch, UINT8 SubCh, UINT8 Dimm, UINT8 Rank, UINT8 Strobe, UINT8 Bit, MRC_LT Level,
             MRC_GT Group, UINT8 Mode, INT16 *Value);
STATIC MRC_STATUS GetSetImode (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 strobe, UINT8 mode, INT16 *value);
STATIC MRC_STATUS GetSetCTLE (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 strobe, UINT8 bit, MRC_LT level,
             MRC_GT group, UINT8 mode, INT16 *value);
STATIC MRC_STATUS GetSetCPUODT (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe, UINT8 bit, MRC_LT level,
             MRC_GT group, UINT8 mode, INT16 *value);
STATIC MRC_STATUS GetSetDIMMODT (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe, UINT8 bit, MRC_LT level,
             MRC_GT group, UINT8 mode, INT16 *value);
STATIC UINT32 MemWriteDimmVrefDdr4 (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe, MRC_LT level, UINT8 vref);
STATIC MRC_STATUS GetSetPxcDdjcDelta (IN PSYSHOST Host, IN UINT8 Socket, IN UINT8 Ch, IN UINT8 Dimm, IN UINT8 Rank,
                               IN UINT8 Strobe, IN UINT8 Bit, IN MRC_GT Group, IN UINT8 Mode, IN OUT INT16 *Value);
STATIC UINT32 RcompStaticLegTraining (PSYSHOST Host);
VOID RxDfeCsrInit (IN PSYSHOST Host, IN UINT8 Socket, IN UINT8 Ch);
MRC_STATUS
GetSetRxSamplerOffsetAdj (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  );

MRC_STATUS
GetSetRxStrobeInvert (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  );

/**

  Get RxDelay

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval IO delay

**/
MRC_STATUS
GetSetRxDelayUnicast (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  );

/**

  Get/Set Rx Vref thru unicast registers

  @param[in]      Host        Pointer to SysHost structure
  @param[in]      Socket      Current socket under test (0-based)
  @param[in]      Ch          Current channel under test (0-based)
  @param[in]      Dimm        Current dimm under test (0-based)
  @param[in]      Rank        Current rank under test (0-based)
  @param[in]      Strobe      Current strobe under test (0-based || ALL_STROBES)
  @param[in]      Bit         Current bit under test (0-based || ALL_BITS)
  @param[in]      Level       Current level (not applicable)
  @param[in]      Group       Current group under test (RxVref)
  @param[in]      Mode        Register/Cache access mode (GSM_READ_CSR, GSM_READ_ONLY, GSM_UPDATE_CACHE, GSM_FORCE_WRITE)
  @param[in,out]  Value       Pointer to data that will be used to retrieve or store

  @retval         MRC_STATUS_STROBE_NOT_PRESENT     Invalid strobe parameter
  @retval         MRC_STATUS_FAILURE                Invalid bit parameter
  @retval         MRC_STATUS_SUCCESS                Otherwise

**/
MRC_STATUS
GetSetRxVrefUnicast (
  IN        PSYSHOST  Host,
  IN        UINT8     Socket,
  IN        UINT8     Ch,
  IN        UINT8     Dimm,
  IN        UINT8     Rank,
  IN        UINT8     Strobe,
  IN        UINT8     Bit,
  IN        MRC_LT    Level,
  IN        MRC_GT    Group,
  IN        UINT8     Mode,
  IN        INT16     *Value
  );

/**

  Get/Set function to modify the RxSampler at a bit or strobe level

  @param[in]      Host        Pointer to SysHost structure
  @param[in]      Socket      Current socket under test (0-based)
  @param[in]      Ch          Current channel under test (0-based)
  @param[in]      Dimm        Current dimm under test (0-based)
  @param[in]      Rank        Current rank under test (0-based)
  @param[in]      Strobe      Current strobe under test (0-based || ALL_STROBES)
  @param[in]      Bit         Current bit under test (0-based || ALL_BITS)
  @param[in]      Level       Current level (not applicable)
  @param[in]      Group       Current group under test (RxSamplerEvenOdd, RxSamplerEven or RxSamplerOdd)
  @param[in]      Mode        Register/Cache access mode (GSM_READ_CSR, GSM_READ_ONLY, GSM_UPDATE_CACHE, GSM_FORCE_WRITE)
  @param[in,out]  Value       Pointer to data that will be used to retrieve or store

  @retval         MRC_STATUS_STROBE_NOT_PRESENT     Invalid strobe parameter
  @retval         MRC_STATUS_INVALID_PARAMETER      Invalid bit parameter
  @retval         MRC_STATUS_INVALID_PARAMETER      Invalid Group parameter
  @retval         MRC_STATUS_SUCCESS                Otherwise

**/
MRC_STATUS
GetSetRxSamplerEvenOddUnicast (
  PSYSHOST    Host,
  UINT8       Socket,
  UINT8       Ch,
  UINT8       Dimm,
  UINT8       Rank,
  UINT8       Strobe,
  UINT8       Bit,
  MRC_LT      Level,
  MRC_GT      Group,
  UINT8       Mode,
  INT16*      Value
  );

/**

  Procedure to write the Lrdimm Buffer Dram Interface Internal VrefDq control word
  PG[2]RW[F1:F0] - DRAM Interface Internal VrefDQ Control Word

  @param[in] Host       - Pointer to sysHost struct
  @param[in] Socket     - Processor socket number
  @param[in] Ch         - Channel number within the socket
  @param[in] SubChMask  - Mask of SubChannels within the channel
  @param[in] Dimm       - DIMM number within the channel
  @param[in] Rank       - Rank number within the DIMM
  @param[in] Strobe     - Strobe number for the rank
  @param[in] Vref       - Vref values to write

  @retval MRC_STATUS_SUCCESS if the Vref is written
  @retval MRC_STATUS_FAILURE if the Vref is not written

**/
MRC_STATUS
MemWriteLrdimmBufferDramVrefDq (
  IN  PSYSHOST            Host,
  IN  UINT8               Socket,
  IN  UINT8               Ch,
  IN  UINT8               SubChMask,
  IN  UINT8               Dimm,
  IN  UINT8               Rank,
  IN  UINT8               Strobe,
  IN  UINT8               Vref
  );

/**

  Get or Set Rx Vref for LRDIMM buffers using Paged Vref Control Words
  PG[2]RW[F1:F0] - DRAM Interface Internal VrefDQ Control Word

  @param[in]     Host    - Pointer to sysHost
  @param[in]     Socket  - Socket number
  @param[in]     Ch      - Channel number within the socket
  @param[in]     Dimm    - DIMM number within the channel
  @param[in]     Rank    - Rank number within the DIMM
  @param[in]     Strobe  - Strobe number within the 80-bit channel
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param[in,out] Value   - Data to program or return buffer

  @retval MRC_STATUS_SUCCESS if data is read or programmed successfully
  @retval !MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS
GetSetLrdimmBufferRxVref (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     Dimm,
  IN     UINT8     Rank,
  IN     UINT8     Strobe,
  IN     MRC_LT    Level,
  IN     MRC_GT    Group,
  IN     UINT8     Mode,
  IN OUT INT16     *Value
  );

/**

  This function will program the multicast register to zero and
  unicast registers will be OFFSET by the amount specified in Value

  @param[in]      Host              Pointer to syshost structure
  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      Dimm              Current dimm under test (0-based)
  @param[in]      Rank              Current rank under test (0-based within dimm)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be modified/read
  @param[in]      MulticastGroup    Specifies the MRC_GT multicast group that will be modified/read
  @param[in,out]  Value             Pointer to store/retrieve a value

  @retval         MRC_STATUS_SUCCESS  When the function was able to flush the multicast value into unicast registers
  @retval         Any other           When the function was not able to flush the multicast value into unicast registers

**/
MRC_STATUS
EFIAPI
FlushMulticastIntoUnicast (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      MRC_GT    MulticastGroup,
  IN OUT  INT16     *Value
  );

/**

Get Set Rx Dfe Coefficient Tap

@param Host    - Pointer to sysHost
@param socket  - Socket number
@param ch      - Channel number (0-based)
@param dimm    - DIMM number (0-based)
@param rank    - Rank number (0-based)
@param strobe  - Strobe number (0-based)
@param bit     - Bit number
@param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
@param group   - CSM_GT - Parameter to program
@param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
@param value   - Data to program

@retval IO delay

**/
MRC_STATUS
GetSetRxDfeCoefficientTap (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  );


/**

Get Set Rx Dfe Coefficient Tap Pe rBit

@param Host    - Pointer to sysHost
@param socket  - Socket number
@param ch      - Channel number (0-based)
@param dimm    - DIMM number (0-based)
@param rank    - Rank number (0-based)
@param strobe  - Strobe number (0-based)
@param bit     - Bit number. When mode is GSM_FORCE_WRITE, only supports ALL_BITS.
@param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
@param group   - CSM_GT - Parameter to program
@param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
@param value   - Data to program. When mode is GSM_FORCE_WRITE, value must be an array of size BITS_PER_NIBBLE, which contains
different values for different bits in the nibble.

@retval IO delay

**/
MRC_STATUS
GetSetRxDfeCoefficientTapPerBit (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  );

/**

  Get/Set function to modify the RxSampler at a bit or strobe level

  @param[in]      Host        Pointer to SysHost structure
  @param[in]      Socket      Current socket under test (0-based)
  @param[in]      Ch          Current channel under test (0-based)
  @param[in]      Dimm        Current dimm under test (0-based)
  @param[in]      Rank        Current rank under test (0-based)
  @param[in]      Strobe      Current strobe under test (0-based || ALL_STROBES)
  @param[in]      Bit         Current bit under test (0-based || ALL_BITS)
  @param[in]      Level       Current level (not applicable)
  @param[in]      Group       Current group under test (RxSamplerEvenOdd, RxSamplerEven or RxSamplerOdd)
  @param[in]      Mode        Register/Cache access mode (GSM_READ_CSR, GSM_READ_ONLY, GSM_UPDATE_CACHE, GSM_FORCE_WRITE)
  @param[in,out]  Value       Pointer to data that will be used to retrieve or store

  @retval         MRC_STATUS_STROBE_NOT_PRESENT     Invalid strobe parameter
  @retval         MRC_STATUS_INVALID_PARAMETER      Invalid bit parameter
  @retval         MRC_STATUS_INVALID_PARAMETER      Invalid Group parameter
  @retval         MRC_STATUS_SUCCESS                Otherwise

**/
MRC_STATUS
GetSetRxSamplerEvenOdd(
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     Rank,
  UINT8     Strobe,
  UINT8     Bit,
  MRC_LT    Level,
  MRC_GT    Group,
  UINT8     Mode,
  INT16     *Value
  );

/**

  Get/Set function to modify the RxSummer at a bit or strobe level

  @param[in]      Host        Pointer to SysHost structure
  @param[in]      Socket      Current socket under test (0-based)
  @param[in]      Ch          Current channel under test (0-based)
  @param[in]      Dimm        Current dimm under test (0-based)
  @param[in]      Rank        Current rank under test (0-based)
  @param[in]      Strobe      Current strobe under test (0-based || ALL_STROBES)
  @param[in]      Bit         Current bit under test (0-based || ALL_BITS)
  @param[in]      Level       Current level (not applicable)
  @param[in]      Group       Current group under test (RxSummer)
  @param[in]      Mode        Register/Cache access mode (GSM_READ_CSR, GSM_READ_ONLY, GSM_UPDATE_CACHE, GSM_FORCE_WRITE)
  @param[in,out]  Value       Pointer to data that will be used to retrieve or store

  @retval         MRC_STATUS_STROBE_NOT_PRESENT     Invalid strobe parameter
  @retval         MRC_STATUS_INVALID_PARAMETER      Invalid bit parameter
  @retval         MRC_STATUS_INVALID_PARAMETER      Invalid Group parameter
  @retval         MRC_STATUS_SUCCESS                Otherwise

**/
MRC_STATUS
GetSetRxSummer (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     Rank,
  UINT8     Strobe,
  UINT8     Bit,
  MRC_LT    Level,
  MRC_GT    Group,
  UINT8     Mode,
  INT16     *Value
  );

MRC_STATUS
GetSetRxSamplerOffsetAdjUnicast (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Strobe,
  IN MRC_LT    Level,
  IN MRC_GT    Group,
  IN UINT8     Mode,
  IN OUT INT16 *Value
);
MRC_STATUS
GetSetDelayMulticastOffset (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      UINT8     Bit,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  );
/**
  Adjust Tco By Offset
    TCO settings to monotonically increase duty cycle adjustments are [95 - 65][0 - 31]
    This function takes the current value and desired offset and returns the adjusted value

  @param[in]  CurrentValue - Starting value
  @param[in]  Offset       - value of desired adjustment

  @retval Modified value

**/
INT16
AdjustTcoByOffset (
  IN  UINT32 CurrentValue,
  IN  INT16  Offset
);


/*
  Returns true if GetSet (..,ALL_STROBES,..) modifies
  all the strobes across all subchannels

  @param[in]      Host              Pointer to SysHost structure
  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test

  @retval   TRUE    GetSet modifies all strobes across subchannels when ALL_STROBES is used
  @retval   FALSE   GetSet modifies all strobes within each subchannel when ALL_STROBES is used

*/
BOOLEAN
IsAllStrobesAcrossSubCh (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      MRC_LT      Level,
  IN      MRC_GT      Group
  )
{
  switch (Group) {
    case DcaDelay:
    case RcdDcaVref:
    case DcaDfeVref:
    case DcsDelay:
    case QcaDelay:
    case QcsDelay:
      return FALSE;
    default:
      return TRUE;
  }
}

/*
  Returns true if a margin group acts as per strobe control but underline is per be bit control which has
  this behavior: When it is read, only the maximum value of the strobe is returned.
  The MemSweepLib need to adjust the individual bit sweep results by read back the per bit value
  with a per bit version group.

  @param[in]      Host              Pointer to SysHost structure
  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test

  @retval   TRUE    Group acts as per strobe control but underline is per be bit control
  @retval   FALSE   Group does not act as per strobe control but underline is per be bit control

*/
BOOLEAN
IsPerBitControlWhileActPerStrobe (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      MRC_LT      Level,
  IN      MRC_GT      Group
  )
{
  BOOLEAN   IsPerBitDelay = FALSE;

  if (Level == DdrLevel) {
    switch (Group) {
    case TxDqDelay:
    case RxVref:
    case RxSamplerEvenOdd:
    case RxSamplerEven:
    case RxSamplerOdd:
      IsPerBitDelay = TRUE;
      break;
    }
  }

  return IsPerBitDelay;
}


/*
  Returns a per bit control margin group that corresponds to the input group.

  NOTE: it should be called after IsPerBitControlWhileActPerStrobe() to get the corresponding group.

  @param[in]      Host              Pointer to SysHost structure
  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test

  @retval   Corresponding per be bit control group

*/
MRC_GT
GetPerBitGroup (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      MRC_LT      Level,
  IN      MRC_GT      Group
  )
{
  if ((Level == DdrLevel) && (Group == TxDqDelay)) {
    return TxDqBitDelay;
  } else {
    return Group;
  }

}

/**

Get Set TxDq Pie Offset Per Bit

@param[in] Host    - Pointer to sysHost
@param[in] socket  - Socket number
@param[in] ch      - Channel number (0-based)
@param[in] dimm    - DIMM number (0-based)
@param[in] rank    - Rank number (0-based)
@param[in] strobe  - Strobe number (0-based)
@param[in] bit     - Bit number
@param[in] level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
@param[in] group   - CSM_GT - Parameter to program
@param[in] mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
@param[in out] value  - Pointer data to read or write. For writes, must point to an array of values for every bit in the nibble

@retval MRC_STATUS

**/
STATIC
MRC_STATUS
GetSetTxDqPieOffsetBit (
  IN PSYSHOST  Host,
  IN UINT8     socket,
  IN UINT8     ch,
  IN UINT8     dimm,
  IN UINT8     rank,
  IN UINT8     strobe,
  IN UINT8     bit,
  IN MRC_LT    level,
  IN MRC_GT    group,
  IN UINT8     mode,
  IN OUT INT16 *value
  );

/**

Set Vsshi before MMRC

@param[in] Socket    - Current Socket

@retval N/A

**/
VOID
EFIAPI
SetVsshi (
  IN UINT8     Socket
  );

/**

Disable Tx Retrain init.

@param[in] socket             - Current Socket

@retval N/A

**/
VOID
DisTxRtInit (
  IN UINT8     Socket
  );

/**

  Configure Pin Io direction.

  @param[in] Socket - Socket Id

  @retval  None

**/
VOID
ConfigurePinIodirection (
  IN UINT8                     Socket
  );

MRC_STATUS
GetSetTxTco (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Strobe,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  );

/**

  GetSet GetSetDcaTco

  @param[in]     Host    - Pointer to sysHost
  @param[in]     Socket  - Socket number
  @param[in]     Ch      - Channel number (0-based)
  @param[in]     SubCh   - Sub-Channel number (0-based)
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - Bit-field of different modes
  @param[in,out] Value   - Pointer to delay Value or offset based on mode

  @retval MRC_STATUS_SUCCESS if no problem
**/
MRC_STATUS
GetSetDcaTco (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     SubCh,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  );

/**

  Procedure to write the Lrdimm Buffer Host Interface Internal VrefDq control word

  @param[in] Host       - Pointer to sysHost struct
  @param[in] Socket     - Processor socket number
  @param[in] Ch         - Channel number within the socket
  @param[in] SubCh      - Subchannel number
  @param[in] Dimm       - DIMM number within the channel
  @param[in] Rank       - Rank number within the DIMM
  @param[in] Strobe     - Strobe number for the rank
  @param[in] Bit        - Bit number within the strobe
  @param[in] LrbuffVref - Pointer to LRBUFF_HOST_VREF containing Vref values to write

  @retval MRC_STATUS_SUCCESS if the Vref is written
  @retval MRC_STATUS_FAILURE if the Vref is not written

**/
MRC_STATUS
MemWriteLrdimmBufferHostVrefDq (
  IN  PSYSHOST            Host,
  IN  UINT8               Socket,
  IN  UINT8               Ch,
  IN  UINT8               SubCh,
  IN  UINT8               Dimm,
  IN  UINT8               Rank,
  IN  UINT8               Strobe,
  IN  UINT8               Bit,
  IN  LRBUFF_HOST_VREF    *LrbuffVref
  );

/**

  Procedure to write the DRAM Vref setting

  @param[in] Host       - Pointer to sysHost struct
  @param[in] Socket     - Processor socket number
  @param[in] Ch         - Channel number within the socket
  @param[in] SubChMask  - Mask of SubChannels within the channel
  @param[in] Dimm       - DIMM number within the channel
  @param[in] Rank       - Rank number within the DIMM
  @param[in] Strobe     - Strobe number for the subchannel
  @param[in] Vref       - Vref value to write

  @retval MRC_STATUS_SUCCESS if the Vref circuit is present and can be written
  @retval MRC_STATUS_FAILURE if the Vref circuit is not present or cannot be written

**/
MRC_STATUS
MemWriteDramVref (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubChMask,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  IN  UINT8     Strobe,
  IN  UINT8     Vref
  );

/**

Initialize the DDRIO interface Wrapper

@param Host  - Pointer to sysHost

@retval SUCCESS

**/
UINT32
InitInterface (
  PSYSHOST  Host                              // Pointer to sysHost, the system Host (root) structure
  )
{
  UINT32  Status = SUCCESS;
  //
  // SetVsshi right before MMRC
  //
  SetVsshi (Host->var.mem.currentSocket);

  if (IsMemFlowEnabled (MmrcInitExecution)) {
    Status = RunMmrc (Host);
  }

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  CteDelayDclk (350); // Delay MMRC after vsshi is set for it to settle

  return Status;
}

/** Get the DDRCRINTFROUNDTRIP*_CH* register offset for a particular channel and DIMM.

  This additionally provides bounds checking for the channel and DIMM values.

  @param[in]  Channel             The target channel number on the memory controller
  @param[in]  SubCh               The target subchannel number on the memory controller
  @param[in]  Dimm                The target DIMM number
  @param[out] RoundTripRegOffset  The DDRCRINTFROUNDTRIP*_CH* register offset for the target channel and DIMM

  @retval EFI_SUCCESS   Register offset was successfully determined
  @retval !EFI_SUCCESS  Register offset was not successfully determined

**/
EFI_STATUS
GetRoundTripRegOffset (
  IN      UINT8   Socket,
  IN      UINT8   GlobalChannel,
  IN      UINT8   McLocalChannel,
  IN      UINT8   SubCh,
  IN      UINT8   Dimm,
      OUT UINT32  *RoundTripRegOffset
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT8       Index;
  PSYSHOST    Host;
  UINT8       SwappedSubCh;

  Host = GetSysHostPointer ();
  SwappedSubCh = UpdateDdrIoCcRegisterSubCh (Host, Socket, GlobalChannel, SubCh);

  Index = McLocalChannel * SUB_CH + SwappedSubCh;

  *RoundTripRegOffset = 0;

  switch (Dimm) {
    case 0:
      if (Index >= ((sizeof mDimm0RoundTripRegOffsets) / (sizeof mDimm0RoundTripRegOffsets[0]))) {
        RcDebugPrint (SDBG_ERROR, "\nGetRoundTripRegOffset: Ch(%d) or SubCh(%d) out of bounds\n", GlobalChannel, SubCh);
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_165);
        Status = EFI_UNSUPPORTED;
      } else {
        *RoundTripRegOffset = mDimm0RoundTripRegOffsets[Index];
      }
      break;
    case 1:
      if (Index >= ((sizeof mDimm1RoundTripRegOffsets) / (sizeof mDimm1RoundTripRegOffsets[0]))) {
        RcDebugPrint (SDBG_ERROR, "\nGetRoundTripRegOffset: Ch(%d) or SubCh(%d) out of bounds\n", GlobalChannel, SubCh);
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_166);
        Status = EFI_UNSUPPORTED;
      } else {
        *RoundTripRegOffset = mDimm1RoundTripRegOffsets[Index];
      }
      break;
    default:
      RcDebugPrint (SDBG_ERROR, "\nDimm out of bounds: %d\n", Dimm);
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_167);
      Status = EFI_UNSUPPORTED;
      break;
  }

  return Status;
}

STATIC
VOID
RestoreDDRIO (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  UINT8                 Ch;
  UINT8                 SubCh;
  UINT8                 Imc;
  UINT8                 Strobe;
  DDRIO_IMC_NVDATA      (*DdrioImcNvList)[MAX_IMC];
  DDRIO_CHANNEL_NVDATA  (*DdrioChannelNvList)[MAX_CH];
  UINT8                 MSVx4;
  UINT8                 MaxChDdr;
  UINT8                 MaxImc;

  MaxImc   = GetMaxImc ();
  MaxChDdr = GetMaxChDdr ();
  MSVx4 = GetMaxStrobeValid(Host);
  DdrioChannelNvList = GetDdrioChannelNvList(Host, Socket);
  DdrioImcNvList     = GetDdrioImcNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
#if 0 // SPR_TODO BIOS HSD-ES 1504716216
    MemWritePciCfgEp (Socket, Ch, DDRCLK_RXVREF_CTL_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[Ch].DdrClkRxVrefCtl);
#else // SPR_TODO BIOS HSD-ES 1504716216
    //
    // Temporarily use ddrCRClkControls but switch to DdrClkRxVrefCtl
    // when we can do that without growing the size of SysHost.
    //
    MemWritePciCfgEp (Socket, Ch, DDRCLK_RXVREF_CTL_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[Ch].ddrCRClkControls);
#endif // SPR_TODO BIOS HSD-ES 1504716216

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL0_MCIO_DDRIO_REG), (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl0[Strobe]);
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG), (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe]);
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG), (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl2[Strobe]);
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL3_MCIO_DDRIO_REG), (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl3[Strobe]);
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_PXC_CTL0_MCIO_DDRIO_REG), (*DdrioChannelNvList)[Ch].DdrdN0PxcCtl0[Strobe]);
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TX_CTL0_MCIO_DDRIO_REG), (*DdrioChannelNvList)[Ch].DdrDataTxCtl0[Strobe]);
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_RX_CTL0_MCIO_DDRIO_REG), (*DdrioChannelNvList)[Ch].DdrDataRxCtl0[Strobe]);
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_PIREF_OFFSET0_MCIO_DDRIO_REG), (*DdrioChannelNvList)[Ch].DdrdN0PirefOffset0[Strobe]);
    }

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_TX_CTL0_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[Ch].DdrCcTxCtl0[SubCh]);
      SetDdrintfCtl0 (Host, Socket, Ch, SubCh, (*DdrioChannelNvList)[Ch].SubChannel[SubCh].DdrintfCtl0Ch);
    }

    (*DdrioChannelNvList)[Ch].DdrClkTxCtl0 = MemReadPciCfgEp (Socket, Ch, DDRCLK_TX_CTL0_MCIO_DDRIOEXT_REG);

    // Restore Sense Amp settings
    RestoreSenseAmpCalib (Socket, Ch);

    //
    // Restore roundtrip, IO latencies, etc, for fast boot.
    //
    RestoreTimings (Host, Socket, Ch);

    //
    // Restore CA slew rate comp.
    //
    RestoreCaSlewRate (Host, Socket, Ch);

    //
    // Restore DQ slew rate comp.
    //
    RestoreDqSlewRate (Host, Socket, Ch);
  } // Ch

  for (Imc = 0; Imc < MaxImc; Imc++) {
    if (Host->var.mem.socket[Socket].imcEnabled[Imc] == 0) {
      continue;
    }
    MemWritePciCfgMC (Socket, Imc, DDRCOMP_DIMM_VREF_CTL0_MCIO_DDRIOEXT_REG, (*DdrioImcNvList)[Imc].dimmVrefControl1);
    MemWritePciCfgMC (Socket, Imc, DDRCOMP_DQ_VREF_CTL_MCIO_DDRIOEXT_REG, (*DdrioImcNvList)[Imc].ddrCRCompCtl0);
  }

}

/**
  Override Timing Parameters based on input data

  @param Host  - Pointer to sysHost

**/
STATIC
UINT32
DfxTimingOverride10nm (
  PSYSHOST  Host
  )
{
  UINT16                                      MinLimit;
  UINT16                                      MaxLimit;
  UINT16                                      UsDelay;
  UINT8                                       Socket;
  UINT8                                       Bit;
  UINT8                                       Ch;
  UINT8                                       Dimm;
  UINT8                                       Rank;
  UINT8                                       SubCh;
  UINT8                                       LogRank;
  UINT8                                       RoundTripValue;
  UINT8                                       IoLatencyValue;
  INT16                                       InputValue;
  INT16                                       InputTxDqsDelay;
  INT16                                       InputTxDqBitDelay;
  INT16                                       NewDelays[BITS_PER_NIBBLE];
  struct dimmNvram                            (*DimmNvList)[MAX_DIMM];
  struct channelNvram                         (*channelNvList)[MAX_CH];
  SYS_SETUP                                   *Setup;
  INT16                                       CmdDelay;
  UINT16                                      CmdDelayminVal = 0xFF;
  UINT16                                      CmdDelaymaxVal = 0x0;
  UINT8                                       MaxChDdr;
  UINT16                                      RcvEnDelayValue;

  Setup = GetSysSetupPointer ();

  Socket = Host->var.mem.currentSocket;

  RcDebugPrint (SDBG_DEFAULT, "\n DfxTimingOverride10nm Starts \n");

  if (!(UbiosGenerationEnabled ())) {
    if (Setup->mem.dfxMemSetup.DfxTimingOverrideCtrl == 0) {
      return SUCCESS;
    }
  }
  //
  // Return if this socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  channelNvList = GetChannelNvList (Host, Socket);
  MaxChDdr      = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*channelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList  = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        //
        // Get the logical rank #
        //
        LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

        //
        //Program Round Trip via DFx knobs for uBIOS
        //

        //
        // Get the round trip value for DDRT from setup and ensure it is an even value.
        // The choice to do it in this code section is that this is SOC specific code and
        // setup is generic code.  This is an SOC specific requirement.
        //
        if ((*DimmNvList)[Dimm].DcpmmPresent == 1) {
          RoundTripValue = (UINT8) Setup->mem.dfxMemSetup.DfxDdrtRt;
        } else {
          RoundTripValue = (UINT8) Setup->mem.dfxMemSetup.DfxInputRt;
        }
        //
        // DfxDdrtRt writes data across 7 bit fields; so an I/P > MRC_ROUND_TRIP_MAX_VALUE means skip programming
        // Check the original setup value to determine if this value should be programmed.
        //
        if (RoundTripValue <= MRC_ROUND_TRIP_MAX_VALUE) {
          if (RoundTripValue % 2 == 1) {
            // This is an ODD value so use -1 to get to a safe even value
            RoundTripValue -= 1;
          }
#ifdef DDR5_SUPPORT
          if (IsDdr5Present (Host, Socket)) {
            for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
              SetRoundTripDdr5 (Socket, Ch, SubCh, LogRank, RoundTripValue);
            }
          } else
#endif
          {
            SetRoundTrip (Socket, Ch, LogRank, RoundTripValue);
          }
        }

        //
        // Get the IO Latency value from setup and ensure it is an odd value.
        // The choice to do it in this code section is that this is SOC specific code and
        // setup is generic code.  This is an SOC specific requirement.
        //
        IoLatencyValue = Setup->mem.dfxMemSetup.DfxInputIoLatency;
        if (IoLatencyValue != 0 && IoLatencyValue % 2 == 0) {
          // This is an even value so use +1 to get to a safe odd value
          IoLatencyValue += 1;
        }
        //
        // DfxInputIoLatency is a UINT8, > 0xF means skip programming
        // Check the original setup value to determine if this value should be programmed.
        //
        if (Setup->mem.dfxMemSetup.DfxInputIoLatency <= 0xF) {
          for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
            SetIoLatencyDdr5 (Socket, Ch, SubCh, LogRank, IoLatencyValue);
          }
        }

        //
        // Per soc emulation request in 1507120898
        // bios shall directly consume the value from Bios knob DfxInputTxDqDelay to program
        //   txdq_dly0, txdq_dly1, txdq_dly2, txdq_dly3, which belongs to TxDqBitDelay
        //

        //
        // DfxInputTxDqDelay is a UINT16, outside of silicon limits means skip programming
        //
        GetDataGroupLimits (Host, DdrLevel, TxDqBitDelay, &MinLimit, &MaxLimit, &UsDelay);
        if ((Setup->mem.dfxMemSetup.DfxInputTxDqDelay > MinLimit) && (Setup->mem.dfxMemSetup.DfxInputTxDqDelay < MaxLimit)) {
          for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
            NewDelays[Bit] = (INT16)Setup->mem.dfxMemSetup.DfxInputTxDqDelay;
          }
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, ALL_STROBES, ALL_BITS, DdrLevel, TxDqBitDelay, GSM_READ_CSR | GSM_FORCE_WRITE | GSM_UPDATE_CACHE, NewDelays);
        }
        //
        // DfxInputTxDqsDelay is a UINT16, outside of silicon limits means skip programming (Silicon max 703)
        //
        GetDataGroupLimits (Host, DdrLevel, TxDqsDelay, &MinLimit, &MaxLimit, &UsDelay);
        if ((*DimmNvList)[Dimm].DcpmmPresent == 1) {
          InputTxDqsDelay = (INT16) Setup->mem.dfxMemSetup.DfxInputDdrtTxDqsDelay;
          InputTxDqBitDelay = (INT16) Setup->mem.dfxMemSetup.DfxInputDdrtTxDqBitDelay;
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                                  "DfxInputDdrtTxDqsDelay: %d DfxInputDdrtTxDqBitDelay %d\n", InputTxDqsDelay, InputTxDqBitDelay);
        } else {
          InputTxDqsDelay = (INT16) Setup->mem.dfxMemSetup.DfxInputTxDqsDelay;
          InputTxDqBitDelay = (INT16) Setup->mem.dfxMemSetup.DfxInputTxDqBitDelay;
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                                  "DfxInputTxDqsDelay: %d DfxInputTxDqBitDelay %d\n", InputTxDqsDelay, InputTxDqBitDelay);
        }
        if ((InputTxDqsDelay > MinLimit) && (InputTxDqsDelay < MaxLimit)) {
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, ALL_STROBES, 0, DdrLevel, TxDqsDelay, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &InputTxDqsDelay);
        }

        //
        // DfxInputTxDqBitDelay is a UINT16, outside of silicon limits means skip programming (Silicon max 703)
        //
        GetDataGroupLimits (Host, DdrLevel, TxDqBitDelay, &MinLimit, &MaxLimit, &UsDelay);
        if ((InputTxDqBitDelay > MinLimit) && (InputTxDqBitDelay < MaxLimit)) {
          for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
            NewDelays[Bit] = InputTxDqBitDelay;
          }
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, ALL_STROBES, ALL_BITS, DdrLevel, TxDqBitDelay, GSM_READ_CSR | GSM_FORCE_WRITE | GSM_UPDATE_CACHE, NewDelays);
        }

        //
        // DfxInputRxDqDelay is a UINT8, outside of silicon limits means skip programming (Silcon max 0x7)
        //
        GetDataGroupLimits (Host, DdrLevel, RxDqDelay, &MinLimit, &MaxLimit, &UsDelay);
        if ((Setup->mem.dfxMemSetup.DfxInputRxDqDelay > MinLimit) && (Setup->mem.dfxMemSetup.DfxInputRxDqDelay < MaxLimit)) {
          InputValue = (INT16) Setup->mem.dfxMemSetup.DfxInputRxDqDelay;
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, ALL_STROBES, 0, DdrLevel, RxDqDelay, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &InputValue);
        }

        //
        // DfxInputRxDqsDelay is a UINT16, outside of silicon limits means skip programming (silicon max 127)
        //
        GetDataGroupLimits (Host, DdrLevel, RxDqsDelay, &MinLimit, &MaxLimit, &UsDelay);
        if ((Setup->mem.dfxMemSetup.DfxInputRxDqsDelay > MinLimit) && (Setup->mem.dfxMemSetup.DfxInputRxDqsDelay < MaxLimit)) {
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, ALL_STROBES, 0, DdrLevel, RxDqsDelay, GSM_UPDATE_CACHE, (INT16 *)&Setup->mem.dfxMemSetup.DfxInputRxDqsDelay);
        }

        //
        // DfxInputRxDqsBitDelay is a UINT8, outside of silicon limits means skip programming
        //
        GetDataGroupLimits (Host, DdrLevel, RxDqsBitDelay, &MinLimit, &MaxLimit, &UsDelay);
        if ((Setup->mem.dfxMemSetup.DfxInputRxDqBitDelay > MinLimit) && (Setup->mem.dfxMemSetup.DfxInputRxDqBitDelay < MaxLimit)) {
          for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
            NewDelays[Bit] = (INT16)Setup->mem.dfxMemSetup.DfxInputRxDqBitDelay;
          }
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, ALL_STROBES, ALL_BITS, DdrLevel, RxDqsBitDelay, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, NewDelays);
        }

        //
        // DfxRecEnDelay is a UINT16, outside of silicon limits means skip programming (silicon max 639)
        //
        GetDataGroupLimits (Host, DdrLevel, RecEnDelay, &MinLimit, &MaxLimit, &UsDelay);
        if ((*DimmNvList)[Dimm].DcpmmPresent == 1) {
          RcvEnDelayValue = (INT16) Setup->mem.dfxMemSetup.DfxDdrtRcvEnDelay;
        } else {
          RcvEnDelayValue = (INT16) Setup->mem.dfxMemSetup.DfxRcvEnDelay;
        }
        if ((RcvEnDelayValue > MinLimit) && (RcvEnDelayValue < MaxLimit)) {
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, ALL_STROBES, 0, DdrLevel, RecEnDelay, GSM_UPDATE_CACHE, (INT16 *)&RcvEnDelayValue);
        }
      } //rank loop

      //
      // DfxInputGnt2Erid is an UINT16, > DDRT_GNT2ERID_MAX means skip programming
      //
      if ((Setup->mem.dfxMemSetup.DfxInputGnt2Erid <= DDRT_GNT2ERID_MAX)  && ((*DimmNvList)[Dimm].DcpmmPresent)) {
        GetSetSxpGnt2Erid (Host, Socket, Ch, Dimm, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, (INT16 *)&Setup->mem.dfxMemSetup.DfxInputGnt2Erid);
      }

      //
      // DfxInputIoLatencyIoComp is a UINT8, > 0x3F means skip programming
      //
      if (Setup->mem.dfxMemSetup.DfxInputIoLatencyIoComp <= 0x3F) {
        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          SetIoLatencyIoComp (Host, Socket, Ch, SubCh, Setup->mem.dfxMemSetup.DfxInputIoLatencyIoComp);
        }
      }

      //
      // DfxCwlAdjPos is a UINT8, > MAX_CWL_ADJ skip programming
      //
      if (Setup->mem.dfxMemSetup.DfxCwlAdjPos <= MAX_CWL_ADJ) {
        if ((*DimmNvList)[Dimm].DcpmmPresent) {
          SetCwlAdj (Host, Socket, Ch, CWLADJ_DDRT, (INT16) Setup->mem.dfxMemSetup.DfxCwlAdjPos);
        } else {
          SetCwlAdj (Host, Socket, Ch, CWLADJ_DDR4, (INT16) Setup->mem.dfxMemSetup.DfxCwlAdjPos);
        }
      }

      //
      // DfxCwlAdjNeg is a UINT8, > ABS (MIN_CWL_ADJ) skip programming
      //
      if (Setup->mem.dfxMemSetup.DfxCwlAdjNeg <= ABS (MIN_CWL_ADJ)) {
        if ((*DimmNvList)[Dimm].DcpmmPresent) {
          SetCwlAdj (Host, Socket, Ch, CWLADJ_DDRT, (INT16) Setup->mem.dfxMemSetup.DfxCwlAdjNeg);
        } else {
          SetCwlAdj (Host, Socket, Ch, CWLADJ_DDR4, (INT16) Setup->mem.dfxMemSetup.DfxCwlAdjNeg);
        }
      }
    } //dimm loop

    if (Setup->mem.dfxMemSetup.DfxInputCmdDelay != INPUT_CMD_DELAY_DEFAULT) {
      CmdDelay = Setup->mem.dfxMemSetup.DfxInputCmdDelay;
      GetSetCmdGroupDelay (Host, Socket, Ch, CmdCtlAll, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &CmdDelay, &CmdDelayminVal, &CmdDelaymaxVal);
    }
  } // ch loop

  RcDebugPrint (SDBG_DEFAULT, " \n DfxTimingOverride10nm Ends \n");
  return SUCCESS;
}

/**

  Initialize DDRIO read and write preamble state

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number

  @retval n/a

**/
VOID
PreambleInitDdrio (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch
  )
{
  //
  //longpreambleenable was removed in SPR.
  //
  return;
}

/**

  Initialize the Rx DFE DDRIO interface

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number

  @retval n/a

**/
VOID
RxDfeCsrInit (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{

}

/**

  Detect if DDRIO lane reversal is enabled.

  @param[in]  Host    - Pointer to sysHost.
  @param[in]  Socket  - Socket number.
  @param[in]  Channel - Channel number.

  @retval TRUE   DDRIO lane reversal is enabled.
  @retval FALSE  DDRIO lane reversal is disabled.

**/
BOOLEAN
EFIAPI
IsLaneReversalEnabled (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  UINT8                           McId;
  IMC_NVRAM_STRUCT                (*ImcNvList)[MAX_IMC];
  MEM_NVRAM_STRUCT                *NvramMem;
  BOOLEAN                         LaneReservalEn;

  McId = GetMCID (Host, Socket, Channel);
  NvramMem = &Host->nvram.mem;
  ImcNvList = &NvramMem->socket[Socket].imc;
  LaneReservalEn = (*ImcNvList)[McId].LaneReversalEn;

  if (LaneReservalEn == FALSE) {
    return FALSE;
  } else {
    return TRUE;
  }
}

/**
  Initizlize DDRIO lane reversal.

  North dies (Die 0 and Die 2) do not need to enable lane reversal.
  South dies (Die 1 and Die 3) have to enable lane reversal.

  @param[in]  Host           - Pointer to sysHost.
  @param[in]  Socket         - Socket number.

  @retval N/A

**/
VOID
EFIAPI
InitializeLaneReversal (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{

  DDRINTF_CTL0_MCIO_DDRIO_STRUCT  DdrintfCtl0;
  UINT8                           McId; // MC ID is the same as Die ID on 10nm wave3.
  UINT8                           MaxImc;
  BOOLEAN                         EnableReversal = FALSE;
  IMC_NVRAM_STRUCT                (*ImcNvList)[MAX_IMC];
  MEM_NVRAM_STRUCT                *NvramMem;

  MaxImc      = GetMaxImc();
  NvramMem = &Host->nvram.mem;
  ImcNvList = &NvramMem->socket[Socket].imc;

  for (McId = 0; McId < MaxImc; McId++) {

    if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
      continue;
    }

    //
    // Check if lane reversal need to be enabled.
    // North dies (Die 0 and Die 2) do not need to enable lane reversal.
    // South dies (Die 1 and Die 3) have to enable lane reversal.
    //

    if ((McId == 0) || (McId == 2)) {
      //
      // Die 0 and Die 2 are north dies. Disable lane reversal.
      //
      EnableReversal = FALSE;
    } else if ((McId == 1) || (McId == 3)) {

      //
      // Die 1 and Die 3 are south dies. Enable lane reversal.
      //
      EnableReversal = TRUE;
    }

    DdrintfCtl0.Data = MemReadPciCfgMC (Socket, McId, DDRINTF_CTL0_MCIO_DDRIO_REG);

    //
    // Cache lane reversal setting.
    //
    (*ImcNvList)[McId].LaneReversalEn = EnableReversal;

    if (EnableReversal == TRUE) {
      DdrintfCtl0.Bits.lane_reversal_en = 1;
    } else {
      DdrintfCtl0.Bits.lane_reversal_en = 0;
    }

    MemWritePciCfgMC (Socket, McId, DDRINTF_CTL0_MCIO_DDRIO_REG, DdrintfCtl0.Data);

  }

  return;
}



/**

  Disabled unused nibbles. Disable ECC nibbles for non-ECC DIMM and any other unused nibbles.

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number

  @retval     EFI_SUCCESS

**/
EFI_STATUS
DisableUnusedNibbles (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch
  )
{
  DDRD_N0_TRAIN_CTL1_MC_DDRIOMC_STRUCT        DdrdN0TrainCtl1;
  DDRIO_CHANNEL_NVDATA                        (*DdrioChannelNvList)[MAX_CH];
  UINT8                                       Strobe;
  UINT8                                       MSVx4;
  UINT8                                       Index;
  UINT8                                       *NotApplicableNibbles;
  UINT8                                       Length;
  EFI_STATUS                                  Status;

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  MSVx4 = GetMaxStrobeValid (Host);

  if (IsDdr5Present (Host, Socket)) {
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)){
        DdrdN0TrainCtl1.Data = (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe];
        DdrdN0TrainCtl1.Bits.nibble_dis = 1;
        (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe] = DdrdN0TrainCtl1.Data;
        MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG), DdrdN0TrainCtl1.Data);
      }
    }
  } else {
    //
    // SPR DDR4.
    //

    //
    // Disable ECC nibbles for non-ECC DIMM.
    //
    for (Strobe = 0; Strobe < MSVx4; Strobe ++) {
      if (IsStrobeNotValid (Host, Strobe )){
        DdrdN0TrainCtl1.Data = (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe];
        DdrdN0TrainCtl1.Bits.nibble_dis = 1;
        (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe] = DdrdN0TrainCtl1.Data;
        MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG), DdrdN0TrainCtl1.Data);
      }
    }

    //
    // Disable not applicable nibbles which is not reported by GetMaxStrobeValid:
    //
    Status = GetNotApplicableNibblesDdr4 (Host, Ch, &NotApplicableNibbles, &Length);
    if (Status == EFI_SUCCESS) {
      for (Index = 0; Index < Length; Index ++) {
        Strobe  = NotApplicableNibbles[Index];
        DdrdN0TrainCtl1.Data = (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe];
        DdrdN0TrainCtl1.Bits.nibble_dis = 1;
        (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe] = DdrdN0TrainCtl1.Data;
        MemWritePciCfgEp (Socket, Ch, GetDdrIoAddr (Host, Strobe, DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG), DdrdN0TrainCtl1.Data);
      }
    }
  }

  return EFI_SUCCESS;
}

/**

  Configure timer test mode to shorten VSSHI and LDO timers. It is simulation-only
  control and used to improve polling efficiency when polling DLL lock

  @param[in] Host   - Pointer to sysHost, the system Host (root) structure struct
  @param[in] Socket - Socket number
  @param[in] Data   - Data to set. 0 - clear this mode; 1 - enable this mode

  @retval None

  **/
VOID
ConfigureTmrTestMode (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Data
  )
{
  UINT8                                      Ch = 0;
  UINT8                                      Strobe;
  UINT8                                      SubCh = 0;
  DDRD_N0_DFX_CTL0_MCIO_DDRIO_STRUCT         DdrdDfxCtl0;
  DDRCC_DFX_CTL0_MCIO_DDRIOEXT_STRUCT        DdrccDfxCtl0;
  DDRCLK_DFX_CTL0_MCIO_DDRIOEXT_STRUCT       DdrclkDfxCtl0;
  UINT8                                      MaxChDdr;

  UINT8                                      MSVx4 = GetMaxStrobeValid (Host);
  struct channelNvram (*ChannelNvList) [MAX_CH];

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }
    //
    // Configure timer test mode for Data.
    //
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      DdrdDfxCtl0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG));
      DdrdDfxCtl0.Bits.tmr_test_mode = Data;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG), DdrdDfxCtl0.Data);
    }
    //
    // Configure timer test mode for CMD/CTL.
    //
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      DdrccDfxCtl0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_DFX_CTL0_MCIO_DDRIOEXT_REG);
      DdrccDfxCtl0.Bits.tmr_test_mode = Data;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_DFX_CTL0_MCIO_DDRIOEXT_REG, DdrccDfxCtl0.Data);
    } //Subch loop
    //
    // Configure timer test mode for CLK.
    //
    DdrclkDfxCtl0.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_DFX_CTL0_MCIO_DDRIOEXT_REG);
    DdrclkDfxCtl0.Bits.tmr_test_mode = Data;
    MemWritePciCfgEp (Socket, Ch, DDRCLK_DFX_CTL0_MCIO_DDRIOEXT_REG, DdrclkDfxCtl0.Data);
  } // Ch loop
} // ConfigureTmrTestMode

/**

  Initialize the DDRIO interface

  @param Host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
InitDdrioInterfaceLate (
  PSYSHOST  Host
  )
{
  UINT8                                       socket;
  UINT8                                       ch;
  UINT8                                       dimm;
  UINT8                                       rank;
  UINT8                                       strobe;
  UINT8                                       Clk;
  UINT8                                       txVref = 0;
  UINT8                                       TxVrefSafe;
  UINT8                                       LrbufBacksideTxVref;
  UINT8                                       imc;
  INT16                                       tempVal;
  UINT16                                      minVal = 0xFF;
  UINT16                                      maxVal = 0x0;
  UINT8                                       CkEnabled[MAX_CH][MAX_CLK];
  INT16                                       Linear;
  UINT16                                      VrefValue;
#ifdef DDR5_SUPPORT
  UINT16                                      DcaVrefValue;
  INT8                                        DcaVrefValueReg;
  UINT16                                      DcsVrefValue;
  INT8                                        DcsVrefValueReg;
  UINT8                                       IbtCaValue;
  UINT8                                       IbtCaValueReg;
  UINT8                                       IbtCsValue;
  UINT8                                       IbtCsValueReg;
  UINT8                                       IbtClkValue;
  UINT8                                       IbtClkValueReg;
  UINT8                                       DramOrDbHostRon;
#endif // DDR5_SUPPORT
  struct channelNvram                         (*channelNvList)[MAX_CH];
  DDRIO_CHANNEL_NVDATA                        (*DdrioChannelNvList)[MAX_CH];
  struct dimmNvram                            (*dimmNvList)[MAX_DIMM];
  struct ddrRank                              (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                           (*rankStruct)[MAX_RANK_DIMM];
  UINT8                                       RankEn;
  //DDRCRCLKCONTROLS_MCIO_DDRIOEXT_STRUCT     ddrCRClkControls;
  UINT8                                       MSVx4 = GetMaxStrobeValid (Host);
  BootMode                                    SysBootMode;
  UINT8                                       MaxChDdr;
  UINT8                                       MaxImc;

  SysBootMode = GetSysBootMode ();
  MaxImc      = GetMaxImc ();
  Linear = 0;
  ZeroMem (CkEnabled, sizeof (CkEnabled));

  socket = Host->var.mem.currentSocket;

  //
  // Initialize DDRIO lane reversal.
  //
  InitializeLaneReversal (Host, socket);

  SetMcDdrtCfg (Host);
  //
  // Return if this socket is disabled
  //

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return SUCCESS;
  }
  channelNvList = GetChannelNvList (Host, socket);
  MaxChDdr      = GetMaxChDdr ();
  SetFeedbackCmdVrefTarget (Host, socket, SysBootMode);

  if (Host->DdrioUltSupport) {
    if (IsDdr5Present (Host, socket)) {
      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList) [ch].enabled == 0) {
          continue;
        }
        if (((*channelNvList) [ch].timingMode == TIMING_1N)) {
          CteSetOptions ("cmd_stretch", 0x3, TWO_SUBCH_MASK, 0, 1);
          CteSetOptions ("cmd_stretch", 0x3, TWO_SUBCH_MASK, 4, 1);
        } else {
          CteSetOptions ("cmd_stretch", 0x3, TWO_SUBCH_MASK, 0, 2);
          CteSetOptions ("cmd_stretch", 0x3, TWO_SUBCH_MASK, 4, 2);
        }
      }
    } else {
      CteSetOptions ("cmd_stretch", 0x3, 0x3, 0, 3);
      CteSetOptions ("cmd_stretch", 0x3, 0x3, 4, 3);
    }
  }

  DdrioChannelNvList = GetDdrioChannelNvList (Host, socket);
  if ((SysBootMode == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot)) {

    //
    // Initialize DDRIO cached values
    //
    CacheDDRIO (Host, socket);

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      if (!IsDdr5Present (Host, socket)) {
        //
        // The initial cmd value is set assuming 1n cmd timing, in the case where the configuration is not 1n then push cmd 1Qclk
        //
        if ((*channelNvList)[ch].timingMode != TIMING_1N) {
          tempVal = 64;
          GetSetCmdGroupDelay (Host, socket, ch, CmdAll, GSM_WRITE_OFFSET | GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &tempVal, &minVal, &maxVal);
        }
       }

      //
      // Initialize read/write preamble DDRIO registers
      //
      PreambleInitDdrio (Host, socket, ch);

      //
      // Initialize RX DFE related CSRs
      //
      RxDfeCsrInit (Host, socket, ch);

      //
      // Disable ECC nibbles for non-ECC DIMM and any other unused nibbles.
      //
      DisableUnusedNibbles (Host, socket, ch);
     //
     // Set the Txrt_init to 0.
     //
      DisTxRtInit (socket);
    }

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      //
      // Construct CKE mask
      //
      dimmNvList  = GetDimmNvList (Host, socket, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        //
        // Continue if the Vref value is not found
        //
        if (!GetDramVrefValueOdt (socket, ch, dimm, &VrefValue)) {
          continue;
        }
        RcDebugPrintWithDevice (
          SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "DRAM Vref   from ODT table: %d.%d%%\n",
          VrefValue / ODT_TABLE_LIB_VREF_PERCENT_FACTOR,
          VrefValue % ODT_TABLE_LIB_VREF_PERCENT_FACTOR
          );

#ifdef DDR5_SUPPORT
        GetDcaVrefValueOdt (socket, ch, dimm, &DcaVrefValue);
        RcDebugPrintWithDevice (
          SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "CA Vref     from ODT table: %d.%d%%\n",
          DcaVrefValue / ODT_TABLE_LIB_VREF_PERCENT_FACTOR,
          DcaVrefValue % ODT_TABLE_LIB_VREF_PERCENT_FACTOR
          );

        GetDcsVrefValueOdt (socket, ch, dimm, &DcsVrefValue);
        RcDebugPrintWithDevice (
          SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "CS Vref     from ODT table: %d.%d%%\n",
          DcsVrefValue / ODT_TABLE_LIB_VREF_PERCENT_FACTOR,
          DcsVrefValue % ODT_TABLE_LIB_VREF_PERCENT_FACTOR
          );

        GetIbtCaValueOdt (socket, ch, dimm, &IbtCaValue);
        RcDebugPrintWithDevice (
          SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "IbtCaValue  from ODT table: %d ohms\n",
          IbtCaValue
          );

        GetIbtCsValueOdt (socket, ch, dimm, &IbtCsValue);
        RcDebugPrintWithDevice (
          SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "IbtCsValue  from ODT table: %d ohms\n",
          IbtCsValue
          );

        GetIbtClkValueOdt (socket, ch, dimm, &IbtClkValue);
        RcDebugPrintWithDevice (
          SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "IbtClkValue from ODT table: %d ohms\n",
          IbtClkValue
          );

        GetDramOrDbHostRon (socket, ch, dimm, &DramOrDbHostRon);
#ifdef LRDIMM_SUPPORT
        if (IsLrdimmPresent (socket, ch, dimm)) {
          RcDebugPrintWithDevice (
            SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "LRDIMM Host Ron from ODT table: %d ohms\n",
            DramOrDbHostRon
            );
        } else
#endif //LRDIMM_SUPPORT
        {
          RcDebugPrintWithDevice (
            SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "RDIMM DRAM Ron from ODT table: %d ohms\n",
            DramOrDbHostRon
            );
        }
#endif // New DDR5 parameters

        rankList = GetRankNvList (Host, socket, ch, dimm);
        rankStruct  = GetRankStruct (Host, socket, ch, dimm);

        // Calculate Vref % based on Jedec spec
#ifdef DDR5_SUPPORT
        if (IsDdr5Present (Host, socket)) {

          Linear = ConvertPercentageToLinear (VrefValue);
          TxVrefSafe = (UINT8) ConvertLinearToPhysical (Linear);

        } else
#endif // #ifdef DDR5_SUPPORT
        {
          // Assume DRAM Vref range 1 (60% - 92.5%)
          VrefValue = ODT_TABLE_LIB_ROUND_VREF_PERCENT (VrefValue);
          txVref = ((((VrefValue - 60) * 100) / 65) & 0x3F);
          TxVrefSafe = txVref + DDR4_VREF_RANGE1_OFFSET;
        }

        for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          //
          // Store ODT values into channelNvList
          //
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Storing [DRAM Vref %%:    0x%x] (txVrefSafe)\n", TxVrefSafe);
          (*channelNvList)[ch].txVrefSafe[(*rankList)[rank].rankIndex] = TxVrefSafe;

#ifdef DDR5_SUPPORT
          Linear = ConvertPercentageToLinear (DcaVrefValue);
          DcaVrefValueReg = (INT8) ConvertLinearToPhysical (Linear);
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Storing [DRAM CA Vref %%: 0x%x]\n", DcaVrefValueReg);
          (*channelNvList)[ch].DcaVrefRegValue[(*rankList)[rank].rankIndex] = DcaVrefValueReg; // Value from ODT table, converted to MR value (per JEDEC spec)

          Linear = ConvertPercentageToLinear (DcsVrefValue);
          DcsVrefValueReg = (INT8) ConvertLinearToPhysical (Linear);
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Storing [DRAM CS Vref %%: 0x%x]\n", DcsVrefValueReg);
          (*channelNvList)[ch].DcsVrefRegValue[(*rankList)[rank].rankIndex] = DcsVrefValueReg; // Value from ODT table, converted to MR value (per JEDEC spec)

          if (EncodeIbtValueToRegister (IbtCaValue, &IbtCaValueReg) == MRC_STATUS_SUCCESS) {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Storing [IbtCaValueReg:  0x%x]\n", IbtCaValueReg);
          } else {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Storing *DEFAULT* value [IbtCaValueReg:  0x%x]\n", IbtCaValueReg);
          }
          (*channelNvList)[ch].IbtCaRegValue[(*rankList)[rank].rankIndex] = IbtCaValueReg; // CA value from ODT table (or a default), converted to MR value (per JEDEC spec)

          if (EncodeIbtValueToRegister (IbtCsValue, &IbtCsValueReg) == MRC_STATUS_SUCCESS) {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Storing [IbtCsValueReg:  0x%x]\n", IbtCsValueReg);
          } else {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Storing *DEFAULT* value [IbtCsValueReg:  0x%x]\n", IbtCsValueReg);
          }
          (*channelNvList)[ch].IbtCsRegValue[(*rankList)[rank].rankIndex] = IbtCsValueReg; // CS value from ODT table (or a default), converted to MR value (per JEDEC spec)

          if (EncodeIbtValueToRegister (IbtClkValue, &IbtClkValueReg) == MRC_STATUS_SUCCESS) {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Storing [IbtClkValueReg: 0x%x]\n", IbtClkValueReg);
          } else {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Storing *DEFAULT* value [IbtClkValueReg:  0x%x]\n", IbtClkValueReg);
          }
          (*channelNvList)[ch].IbtClkRegValue[(*rankList)[rank].rankIndex] = IbtClkValueReg; // CLK value from ODT table (or a default), converted to MR value (per JEDEC spec)
#endif // DDR5_SUPPORT

          //
          // Save Tx Vref
          //
          for (strobe = 0; strobe < MSVx4; strobe++) {
            (*DdrioChannelNvList)[ch].txVref[(*rankList)[rank].rankIndex][strobe] = (*channelNvList)[ch].txVrefSafe[(*rankList)[rank].rankIndex];
            (*DdrioChannelNvList)[ch].txVrefCache[(*rankList)[rank].rankIndex][strobe] = (*channelNvList)[ch].txVrefSafe[(*rankList)[rank].rankIndex];
          } // strobe loop

#ifdef LRDIMM_SUPPORT
          //
          // Save backside DRAM TxVref safe value
          //
          if (IsLrdimmPresent (socket, ch, dimm)) {
            if ((*dimmNvList)[dimm].numRanks == 1) {
              LrbufBacksideTxVref = BACKSIDE_DRAM_DQ_VREF_INIT_VALUE_SR;
            } else {
              LrbufBacksideTxVref = BACKSIDE_DRAM_DQ_VREF_INIT_VALUE_DR;
            }

            for (strobe = 0; strobe < MSVx4; strobe++) {
              (*rankList)[rank].lrbufTxVref[strobe] = LrbufBacksideTxVref;
              (*rankStruct)[rank].lrbufTxVrefCache[strobe] = LrbufBacksideTxVref;
            } // strobe
          }
#endif
        } // rank loop
      } // dimm loop
    } // ch loop

  PretrainingDdrioInit (Host, socket);

#ifdef DDR5_SUPPORT
    SetCSMask(Host, socket);
#endif

  } //boot mode

  //
  // Restore DDRIO values
  //
  if ((SysBootMode == S3Resume) || (Host->var.mem.subBootMode == WarmBootFast) || (Host->var.mem.subBootMode == ColdBootFast)) {
    RestoreDDRIO (Host, socket);
  }

  // Intialize PXC and DDJC related registers if PXC or DDJC training is enabled
  PxcDdjcInitialize (Host, socket);

  // HSD 5331856 disable closed page mode during training
  for (imc = 0; imc < MaxImc; imc++) {
    if (Host->var.mem.socket[socket].imcEnabled[imc] == 0) {
      continue;
    }

    SetPageModeMc (Host, socket, imc, CLOSED_PAGE_DIS);
  } // imc loop

  //
  // Hard coded COMP settings
  //
  ProgramIOCompValues (Host, socket);

  RcompStaticLegTraining (Host);

  //
  // One RCOMP cycle only on PHY Init (MMRC Init) if RcompStaticLeg step is not enabled
  // As it has RCOMP cycle in this step.
  //
  if (!IsMemFlowEnabled (RcompStaticLeg)) {
    DoCompDdrioPerSetupOption (socket, TRAINING_COMP_MMRC_INIT);
  }

  if ((SysBootMode == NormalBoot) && ((Host->var.mem.subBootMode == ColdBoot) || (Host->var.mem.subBootMode == ColdBootFast))) {

    // HSD 5331110  Disable Periodic Rcomp during memory training
    for (imc = 0; imc < MaxImc; imc++) {
      if (Host->var.mem.socket[socket].imcEnabled[imc] == 0) {
        continue;
      }
      SetRcompDisableMc (Host, socket, imc);
    }

    FnvDdrtIoInitPllTarget (Host, socket, SysBootMode);
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      rankList = GetRankNvList(Host, socket, ch, dimm);
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        Clk = (*rankList)[rank].ckIndex;
        CkEnabled[ch][Clk] = 1;
      }
    }
  }


  if (Host->var.mem.subBootMode == WarmBootFast) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      //SPR_TODO_DDRIO. Don't find the below commented out registers in SPR.
      /*MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[ch].ddrCRCmdTrainingCmdN);
      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[ch].ddrCRCmdTrainingCmdS);
      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CTL_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[ch].ddrCRCtlTraining);
      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CKE_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[ch].ddrCRCkeTraining);
      MemWritePciCfgEp (socket, ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[ch].ddrCRClkTraining);

      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[ch].ddrCRCmdControls3CmdN);
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDS_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[ch].ddrCRCmdControls3CmdS);
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CTL_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[ch].ddrCRCmdControls3Ctl);
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CKE_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[ch].ddrCRCmdControls3Cke);*/
      //SPR_TODO_DDRIO. Have two rxfifo_ptr_ctl in two registers. Which one to use?
      for (Clk = 0; Clk < MAX_CLK; Clk++) {
        if (CkEnabled[ch][Clk] == 0) {
          continue;
        }

        MemWritePciCfgEp (socket, ch, GetClkRegOffset (Clk), (*DdrioChannelNvList)[ch].ClkCsr[Clk]);
      }
    }
  }

  OutputCheckpoint (CHECKPOINT_MAJOR_DDRIO_INIT, CHECKPOINT_MINOR_CLOCK_EN, 0);
  if ((SysBootMode != S3Resume)) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      SetRankEnDdrio (Host, socket, ch, 0xF);
    }
    IO_Reset (Host, socket);
  }

  if ((SysBootMode == S3Resume)) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      //SPR_TODO_DDRIO. Don't find the below commented out registers in SPR.
      /*MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[ch].ddrCRCmdTrainingCmdN);
      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[ch].ddrCRCmdTrainingCmdS);
      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CTL_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[ch].ddrCRCtlTraining);
      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CKE_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[ch].ddrCRCkeTraining);
      MemWritePciCfgEp (socket, ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[ch].ddrCRClkTraining);

      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[ch].ddrCRCmdControls3CmdN);
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDS_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[ch].ddrCRCmdControls3CmdS);
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CTL_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[ch].ddrCRCmdControls3Ctl);
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CKE_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList)[ch].ddrCRCmdControls3Cke);*/
      //SPR_TODO_DDRIO. Have two rxfifo_ptr_ctl in two registers. Which one to use?
      for (Clk = 0; Clk < MAX_CLK; Clk++) {
        if (CkEnabled[ch][Clk] == 0) {
          continue;
        }

        MemWritePciCfgEp (socket, ch, GetClkRegOffset (Clk), (*DdrioChannelNvList)[ch].ClkCsr[Clk]);
      }

      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }

        OutputCheckpoint (CHECKPOINT_MAJOR_DDRIO_INIT, CHECKPOINT_MINOR_CLOCK_EN_S3, 0);

        //
        // Enable CK for all ranks
        //
        SetRankEnDdrio (Host, socket, ch, 0xF);
      }
      IO_Reset (Host, socket);

      SetDclkEnableMc (MemTechDdr, socket);
      FnvPollingBootStatusRegister (Host, socket, FNV_DT_DONE);
    }
  }

  CrossoverCalib10nm (Host);

  if (Host->var.mem.subBootMode == ColdBoot) {
#if 0 //SPR_TODO. Will enable once getting the SPR CCC table, or algorithm (1707074146).
    StartingCCCTarget (Host, socket);
#endif //if 0
  }

  //
  // Scad target shall execute before ResetAllDdrChannels(DRAM Reset)
  //
  if (((SysBootMode == NormalBoot) && ((Host->var.mem.subBootMode == ColdBoot) || (Host->var.mem.subBootMode == ColdBootFast)))){
    ScadExitTarget (Host, socket);
  }

  //
  // Execute NVMDIMM IO init in NVMCTLR
  //
  OutputCheckpoint (CHECKPOINT_MAJOR_DDRIO_INIT, CHECKPOINT_MINOR_JEDEC_RESET_INIT, 0);
  ResetAllDdrChannels (Host, socket, CH_BITMASK);

  OutputCheckpoint (CHECKPOINT_MAJOR_DDRIO_INIT, CHECKPOINT_MINOR_DDRT_IO_INIT, 0);

  if (Ddr5RambusD0Errata (Host, socket)) {
    //
    // Reset DRAM if Errata is applied
    //
    ResetAllDdrChannels (Host, socket, CH_BITMASK);
  }

  if ((SysBootMode == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot)) {
    //
    // Configure the Pin IO Direction
    //
#if !defined(CWV_A0_REFACTOR_POST_PO)
    //
    // This is only to unblock 2S boot issue which is reported by 1508051976
    // While this change would break 1507966874. Need to revisit it to meed
    // the requirement of 1507966874.
    //
    SenseAmpOffset (Host);
#endif
    ConfigurePinIodirection (socket);
  }

  OutputCheckpoint (CHECKPOINT_MAJOR_DDRIO_INIT, CHECKPOINT_MINOR_TXVREF_CACHE, 0);

  //
  // Disable unused output clocks for ddr4 and NVMDIMM dimms
  //
  //
  //        DIMM1              DIMM0             Ranken -- binary(slot0 - bit 0 and bit 1; slot1 - bit 2 and bit 3)
  //  (L)RDIMM (SR/DR)   (L)RDIMM (SR/DR)          0101
  //       UDIMM (SR)        UDIMM (SR)            0101
  //       UDIMM (DR)        UDIMM (DR)            1111
  //       UDIMM (DR)        UDIMM (SR)            1101
  //       UDIMM (SR)        UDIMM (DR)            0111
  //       DDRT          (L)RDIMM (SR/DR)          1101

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    if (!(UbiosGenerationOrHsleEnabled ())) {
      dimmNvList = GetDimmNvList (Host, socket, ch);

      RankEn = 0;
      for (dimm = 0; dimm < Host->var.mem.socket[socket].channelList[ch].numDimmSlots; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        //
        // CLK0 goes to DIMM0, CLK1 goes to DIMM1.
        //
        if (((*dimmNvList)[dimm].numRanks == 2) && ((Host->nvram.mem.dimmTypePresent == UDIMM) || (Host->nvram.mem.dimmTypePresent == SODIMM))) {
          RankEn |= (DDR_CLK0_CLK2_ENABLE << dimm);
        } else {
          RankEn |= (DDR_CLK0_ENABLE << dimm);
        }
      }
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "Clocks enabled(ranken): 0x%x\n", RankEn);
      SetRankEnDdrio (Host, socket, ch, RankEn);
    }

    SetIdleCmdToCkeLowMc (Host, socket, ch);
  } // ch loop

  //
  // DFX hook to set timing values for FC Emulation at the end of InitDdrioInterfaceLate
  //
  DfxTimingOverride10nm (Host);
  if ((UbiosGenerationOrHsleEnabled ())) {
    EnableForceSaOn (Host);
  }
  return SUCCESS;
} // InitDdrioInterfaceLate

/**

  Initialize/Train the DDRIO Static Leg values

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS

**/
STATIC
UINT32
RcompStaticLegTraining (
  IN      PSYSHOST Host
  )
{
  UINT8                                          Socket;
  UINT8                                          McId;
  UINT8                                          Ch;
  UINT8                                          SubCh;
  UINT8                                          Strobe;
  PT_ZONE                                        MemPhaseSave;
  DDRIO_CHANNEL_NVDATA                           (*DdrioChannelNvList)[MAX_CH];
  DDRCOMP_STATIC_LEG_CTL0_MCIO_DDRIOEXT_STRUCT   DdrCrCompStatLegTrainCtrl;
  DDRCOMP_DATA_COMP0_MCIO_DDRIOEXT_STRUCT        DdrCrDataComp;
  DDRCOMP_CMD_COMP0_MCIO_DDRIOEXT_STRUCT         DdrCrCmdComp;
  DDRCOMP_CLK_COMP0_MCIO_DDRIOEXT_STRUCT         DdrCrClkComp;
  BOOLEAN                                        ReRunComp;
  BOOLEAN                                        UpdateNeededForThisImc;
  DDRD_N0_RX_CTL0_MCIO_DDRIO_STRUCT              DdrDataRxCtl0;
  DDRD_N0_TX_CTL0_MCIO_DDRIO_STRUCT              DdrDataTxCtl0;
  DDRCC_TX_CTL0_MCIO_DDRIOEXT_STRUCT             DdrCcTxCtl0;
  DDRCLK_TX_CTL0_MCIO_DDRIOEXT_STRUCT            DdrClkTxCtl0;
  UINT8                                          MaxChDdr;
  UINT8                                          MaxImc;


  MaxImc      = GetMaxImc ();
  MaxChDdr    = GetMaxChDdr ();
  UINT8 MSVx4 = GetMaxStrobeValid(Host);

  Socket = Host->var.mem.currentSocket;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "RcompStaticLegTraining Starts\n");
  OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_RCOMP_STAT_LEG, Socket);

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  //
  // Return if this Socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  if (!IsMemFlowEnabled (RcompStaticLeg)) {
    return SUCCESS;
  }

  GetMemPhase (&MemPhaseSave);
  SetMemPhaseCP (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_RCOMP_STAT_LEG);

  //
  // Initialize the static legs to the max value
  //
  for (McId = 0; McId < MaxImc; McId++) {
    if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
      continue;
    }
    DdrCrCompStatLegTrainCtrl.Data = MemReadPciCfgMC (Socket, McId, DDRCOMP_STATIC_LEG_CTL0_MCIO_DDRIOEXT_REG);
    DdrCrCompStatLegTrainCtrl.Bits.odt_static_leg_ctl = STATIC_LEG_CTL_INIT;
    DdrCrCompStatLegTrainCtrl.Bits.txdq_static_leg_ctl = STATIC_LEG_CTL_INIT;
    DdrCrCompStatLegTrainCtrl.Bits.cmd_static_leg_ctl = STATIC_LEG_CTL_INIT;
    DdrCrCompStatLegTrainCtrl.Bits.clk_static_leg_ctl = STATIC_LEG_CTL_INIT;
    MemWritePciCfgMC (Socket, McId, DDRCOMP_STATIC_LEG_CTL0_MCIO_DDRIOEXT_REG, DdrCrCompStatLegTrainCtrl.Data);
  } //McId loop
  //
  // Run the comp engine and poll until it completes
  //
  DoCompDdrio (Host, Socket);
  //
  // Read the Rcomp up/down codes
  // If either Rcomp code < LOWER_RCOMP_CODE_VALUE, set the number of static legs to STATIC_LEG_CTL_ADJUST and rerun comp
  //
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "Comparing static leg results with resistor value: 0x%x\n", LOWER_RCOMP_CODE_VALUE);
  ReRunComp = FALSE;
  for (McId = 0; McId < MaxImc; McId++) {
    if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
      continue;
    }
    UpdateNeededForThisImc = FALSE;
    DdrCrCompStatLegTrainCtrl.Data = MemReadPciCfgMC (Socket, McId, DDRCOMP_STATIC_LEG_CTL0_MCIO_DDRIOEXT_REG);
    DdrCrDataComp.Data = MemReadPciCfgMC (Socket, McId, DDRCOMP_DATA_COMP0_MCIO_DDRIOEXT_REG);
    DdrCrCmdComp.Data = MemReadPciCfgMC (Socket, McId, DDRCOMP_CMD_COMP0_MCIO_DDRIOEXT_REG);
    DdrCrClkComp.Data = MemReadPciCfgMC (Socket, McId, DDRCOMP_CLK_COMP0_MCIO_DDRIOEXT_REG);

    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "MC%d DataRcompDrvUp = 0x%x  DataRcompDrvDown = 0x%x\n", McId, DdrCrDataComp.Bits.rcomp_drv_up, DdrCrDataComp.Bits.rcomp_drv_dn);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "MC%d RcompOdtUp =     0x%x  RcompOdtDown =     0x%x\n", McId, DdrCrDataComp.Bits.rcomp_odt_up, DdrCrDataComp.Bits.rcomp_odt_dn);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "MC%d CmdRcompDrvUp =  0x%x  CmdRcompDrvDown =  0x%x\n", McId, DdrCrCmdComp.Bits.rcomp_drv_up, DdrCrCmdComp.Bits.rcomp_drv_dn);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "MC%d ClkRcompDrvUp =  0x%x  ClkRcompDrvDown =  0x%x\n", McId, DdrCrClkComp.Bits.rcomp_drv_up, DdrCrClkComp.Bits.rcomp_drv_dn);
    //
    // Check comp results for TxDq
    //
    if ((DdrCrDataComp.Bits.rcomp_drv_up < LOWER_RCOMP_CODE_VALUE) || (DdrCrDataComp.Bits.rcomp_drv_dn < LOWER_RCOMP_CODE_VALUE)) {
      DdrCrCompStatLegTrainCtrl.Bits.txdq_static_leg_ctl = STATIC_LEG_CTL_ADJUST;
      UpdateNeededForThisImc = TRUE;
      ReRunComp = TRUE;
    }
    //
    // Check comp results for ODT
    //
    if ((DdrCrDataComp.Bits.rcomp_odt_up < LOWER_RCOMP_CODE_VALUE) || (DdrCrDataComp.Bits.rcomp_odt_dn < LOWER_RCOMP_CODE_VALUE)) {
      DdrCrCompStatLegTrainCtrl.Bits.odt_static_leg_ctl = STATIC_LEG_CTL_ADJUST;
      UpdateNeededForThisImc = TRUE;
      ReRunComp = TRUE;
    }
    //
    // Check comp results for CMD
    //
    if ((DdrCrCmdComp.Bits.rcomp_drv_up < LOWER_RCOMP_CODE_VALUE) || (DdrCrCmdComp.Bits.rcomp_drv_dn < LOWER_RCOMP_CODE_VALUE)) {
      DdrCrCompStatLegTrainCtrl.Bits.cmd_static_leg_ctl = STATIC_LEG_CTL_ADJUST;
      UpdateNeededForThisImc = TRUE;
      ReRunComp = TRUE;
    }
    //
    // Check comp results for CLK
    //
    if ((DdrCrClkComp.Bits.rcomp_drv_up < LOWER_RCOMP_CODE_VALUE) || (DdrCrClkComp.Bits.rcomp_drv_dn < LOWER_RCOMP_CODE_VALUE)) {
      DdrCrCompStatLegTrainCtrl.Bits.clk_static_leg_ctl = STATIC_LEG_CTL_ADJUST;
      UpdateNeededForThisImc = TRUE;
      ReRunComp = TRUE;
    }
    if (UpdateNeededForThisImc) {
      MemWritePciCfgMC (Socket, McId, DDRCOMP_STATIC_LEG_CTL0_MCIO_DDRIOEXT_REG, DdrCrCompStatLegTrainCtrl.Data);
    }
  } //McId loop
  if (ReRunComp) {
    DoCompDdrio (Host, Socket);
  }
  //
  // Program static training results to shadow/endpoint registers
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    McId = GetMCID (Host, Socket, Ch);
    if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
      continue;
    }
    DdrCrCompStatLegTrainCtrl.Data = MemReadPciCfgMC (Socket, McId, DDRCOMP_STATIC_LEG_CTL0_MCIO_DDRIOEXT_REG);

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValid(Host, Strobe)) {
        continue;
      }
      //
      // DATA
      // Assumes CacheDDRIO has been run, so it makes use of the cached values for reads
      //
      DdrDataTxCtl0.Data = (*DdrioChannelNvList)[Ch].DdrDataTxCtl0[Strobe];
      DdrDataRxCtl0.Data = (*DdrioChannelNvList)[Ch].DdrDataRxCtl0[Strobe];
      DdrDataTxCtl0.Bits.drv_static_leg_cfg = DdrCrCompStatLegTrainCtrl.Bits.txdq_static_leg_ctl;
      DdrDataRxCtl0.Bits.odt_static_leg_cfg = DdrCrCompStatLegTrainCtrl.Bits.odt_static_leg_ctl;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TX_CTL0_MCIO_DDRIO_REG), DdrDataTxCtl0.Data);
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_RX_CTL0_MCIO_DDRIO_REG), DdrDataRxCtl0.Data);
      //
      //update cache
      //
      (*DdrioChannelNvList)[Ch].DdrDataTxCtl0[Strobe] = DdrDataTxCtl0.Data;
      (*DdrioChannelNvList)[Ch].DdrDataRxCtl0[Strobe] = DdrDataRxCtl0.Data;
    } //Strobe loop

    //
    // CMD/CTL
    //
    for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
      DdrCcTxCtl0.Data = (*DdrioChannelNvList)[Ch].DdrCcTxCtl0[SubCh];
      DdrCcTxCtl0.Bits.drv_static_leg_cfg = DdrCrCompStatLegTrainCtrl.Bits.cmd_static_leg_ctl;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_TX_CTL0_MCIO_DDRIOEXT_REG, DdrCcTxCtl0.Data);
      //
      //update cache
      //
      (*DdrioChannelNvList)[Ch].DdrCcTxCtl0[SubCh] = DdrCcTxCtl0.Data;
    }

    //
    // CLK
    //
    DdrClkTxCtl0.Data = (*DdrioChannelNvList)[Ch].DdrClkTxCtl0;
    DdrClkTxCtl0.Bits.drv_static_leg_cfg = DdrCrCompStatLegTrainCtrl.Bits.clk_static_leg_ctl;
    MemWritePciCfgEp (Socket, Ch, DDRCLK_TX_CTL0_MCIO_DDRIOEXT_REG, DdrClkTxCtl0.Data);
    //
    //update cache
    //
    (*DdrioChannelNvList)[Ch].DdrClkTxCtl0 = DdrClkTxCtl0.Data;

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "Final static leg settings:\n    DdrCrCompStatLegTrainCtrl.txdq_static_leg_ctl: 0x%x\n", DdrCrCompStatLegTrainCtrl.Bits.txdq_static_leg_ctl);
    RcDebugPrint (SDBG_MAX, "    DdrCrCompStatLegTrainCtrl.odt_static_leg_ctl:  0x%x\n", DdrCrCompStatLegTrainCtrl.Bits.odt_static_leg_ctl);
    RcDebugPrint (SDBG_MAX, "    DdrCrCompStatLegTrainCtrl.cmd_static_leg_ctl:  0x%x\n", DdrCrCompStatLegTrainCtrl.Bits.cmd_static_leg_ctl);
    RcDebugPrint (SDBG_MAX, "    DdrCrCompStatLegTrainCtrl.clk_static_leg_ctl:  0x%x\n", DdrCrCompStatLegTrainCtrl.Bits.clk_static_leg_ctl);
  } //Ch loop

  SetMemPhase (MemPhaseSave);
  return SUCCESS;
} //RcompStaticLegTraining


/**

  Single API to access, modify, increase or set –one or more registers– pertaining to a single MC, DDRIO or DRAM "knob"

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer, HBM)
  @param group   - CSM_GT - Parameter to program
  @param mode    - Bit-field of different modes
  @param value   - Pointer to delay value or offset based on mode

  @retval MRC_STATUS

**/
MRC_STATUS
GetSetDataGroup (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     subch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  MRC_STATUS  status = MRC_STATUS_SUCCESS;
  struct dimmNvram (*dimmNvList) [MAX_DIMM];
  UINT8       ModeOriginal;

  //
  // WA until SPR DDRIO CSR cache function check pass. HSD 1507315818
  //
  ModeOriginal = mode;
  if ((GetCurrentTestType (socket) != ReadPreDfe2DCenteringTest) &&
      (GetCurrentTestType (socket) != ReadPostDfe2DCenteringTest) &&
      (GetCurrentTestType (socket) != WritePreDfe2DCenteringTest) &&
      (GetCurrentTestType (socket) != WritePostDfe2DCenteringTest) &&
      (GetCurrentTestType (socket) != ReadDfeTest) &&
      (GetCurrentTestType (socket) != WriteDfeTest)) {
    //
    // Need to do enough CSR comparing tests and then remove the below code.
    //
    mode = mode | GSM_READ_CSR;
  }
  if ((mode & GSM_UPDATE_CACHE) && (group != TxVref)) {
    mode = mode | GSM_FORCE_WRITE;
  }

  //
  // The functions below expect channel-based "logical" strobe number
  // Logical channel strobe numbers are (0-19) for x4 devices and (0-4,10-14) for x8 devices
  // If caller specifies a subchannel, convert the strobe number to logical channel strobe
  //
  if ((subch != NO_SUBCH) && (strobe < (MAX_STROBE_DDR5 / 2))) {
    strobe = STROBE_INDEX_SUBCH (strobe, subch);
  }

  dimmNvList = GetDimmNvList (Host, socket, ch);
  switch (group) {
    case TxDqDelay:
    case TxDqsDelay:
    case TxDqsDelayCycle:
    case TxEq:
    case TxEqTap2:
    case WrLvlDelay:
    case TxDqsPieOffset:
      //
      // Caching is enabled
      //
      mode = ModeOriginal;
      status = GetSetTxDelay (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;

    case TxDqBitDelay:
      //
      // Caching is enabled
      //
      mode = ModeOriginal;
      status = GetSetTxDelayBit (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;

    case RxDqsBitDelay:
    case RxDqsPBitDelay:
    case RxDqsNBitDelay:
      //
      // Caching is enabled
      //
      mode = ModeOriginal;
      status = GetSetRxDelayBit (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;

    case RxDfeCoefficientTap1PerBit:
    case RxDfeCoefficientTap2PerBit:
    case RxDfeCoefficientTap3PerBit:
    case RxDfeCoefficientTap4PerBit:
      //
      // Caching is enabled
      //
      mode = ModeOriginal;
      status = GetSetRxDfeCoefficientTapPerBit (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;

    case RecEnDelay:
    case RecEnDelayCycle:
    case RxDqsPDelay:
    case RxDqsNDelay:
    case RxDqsDelay:
    case RxDqDelay:
    case RxDfeCoefficient:
      //
      // Caching is enabled
      //
      mode = ModeOriginal;
      status = GetSetRxDelay (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;

    case RxDfeCoefficientTap1:
    case RxDfeCoefficientTap2:
    case RxDfeCoefficientTap3:
    case RxDfeCoefficientTap4:
      //
      // Caching is enabled
      //
      mode = ModeOriginal;
      status = GetSetRxDfeCoefficientTap (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;

    case RxStrobeInvert:
      //
      // Caching is enabled
      //
      mode = ModeOriginal;
      status = GetSetRxStrobeInvert (Host, socket, ch, dimm, rank, strobe, level, group, mode, value);
      break;
    case RxVref:
      status = GetSetRxVref (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;
    case RxSummer:
      status = GetSetRxSummer (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;
    case PxcDelta:
    case DdjcDelta:
      status = GetSetPxcDdjcDelta (Host, socket, ch, dimm, rank, strobe, bit, group, mode, value);
      break;

    case TxVref:
      //
      // Caching is enabled
      //
      mode = ModeOriginal;
      status = GetSetTxVref (Host, socket, ch, subch, dimm, rank, strobe, bit, level, group, mode, value);
      break;

    case DbDfeVrefDq0:
    case DbDfeVrefDq1:
    case DbDfeVrefDq2:
    case DbDfeVrefDq3:
    case DbDfeVrefDq4:
    case DbDfeVrefDq5:
    case DbDfeVrefDq6:
    case DbDfeVrefDq7:

      mode = ModeOriginal;
      status =GetSetDbDfeVrefDqn (Host, socket, ch, dimm, strobe, group, mode, value);
      break;

    case TxImode:
      status = GetSetImode (Host, socket, ch, strobe, mode, value);
      break;

    case RxEq:
      //
      // Caching is enabled
      //
      mode = ModeOriginal;
      status = GetSetRxDelay (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;
    case RxCtleC:
    case RxCtleR:
      status = GetSetCTLE (Host, socket, ch, dimm, strobe, bit, level, group, mode, value);
      break;

    case RxOdt:
    case TxRon:
    case TxDsPd:
    case TxDsPu:
      status = GetSetCPUODT (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;
#if defined (DDR5_SUPPORT)
    case RttNomWr:
    case RttNomRd:
    case RttWr:
    case RttPark:
    case DqsRttPark:
    case DramRonPullUp:
    case DramRonPullDown:
    case RttCkGroupA:
    case RttCkGroupB:
    case RttCsGroupA:
    case RttCsGroupB:
    case RttCaGroupA:
    case RttCaGroupB:
      status = GetSetDimmRttRonDdr5 (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;
#endif
    case TxRiseFallSlewRate:
    case TxFallSlewRate:
    case TxDqRiseSlewRate:
    case TxDqFallSlewRate:
      status = GetSetTxRiseFallSlewRate (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;
    case TcoDqDqs:
    case LinearCntlTcoDqDqs:
      status = GetSetTxTco (Host, socket, ch, strobe, level, group, mode, value);
      break;
    case TxDqScomp:
      status = GetSetTxDqScomp (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);      break;
    case DcaTco:
    case LinearCntlDcaTco:
      status = GetSetDcaTco (Host, socket, ch, subch, level, group, mode, value);
      break;
    case CmdTxEq:
      status = GetSetCmdTxEq (Host, socket, ch, mode, value);
      break;
    case PxcControl:
      status = GetSetPxcControl (Host, socket, ch, mode, value);
      break;
    case RxFlyBy:
      status = GetRxFlyByCore (Host, socket, ch, subch, dimm, rank, strobe, level, mode, value);
      break;
    case TxFlyBy:
      status = GetTxFlyByCore (Host, socket, ch, dimm, rank, strobe, level, mode, value);
      break;
    case RxSampler:
      status = GetSetRxSamplerOffsetAdj (Host, socket, ch, dimm, rank, strobe, level, group, mode, value);
      break;
    case RxSamplerEvenOdd:
    case RxSamplerEven:
    case RxSamplerOdd:
      status = GetSetRxSamplerEvenOdd (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;
    case RxDqsPerChDelayOffset:
    case TxDqPerChDelayOffset:
    case RxVrefPerChDelayOffset:
    case RxSamplerPerChDelayOffset:
      status = GetSetDelayMulticastOffset (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;
    case TxDqPieOffset:
      status = GetSetTxDqPieOffsetBit (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;
    default:
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
        "GetSetDataGroup called unknown group!\n");
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_53);
      break;

  }

  return status;
} // GetSetDataGroup


/**
  Read/write PXC DQ attackers for a strobe
  Each DQ pin has 0, 1 or 2 attacker DQs within the same byte.

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       Strobe      - Strobe number
  @param [in]       Mode        - Bit-field of different access modes
  @param [in,out]   Attacker    - Pointer to array of PXC_ATTACKER_SEL structures

  @retval SUCCESS
**/
UINT32
GetSetPxcAttackSel (
  IN     PSYSHOST          Host,
  IN     UINT8             Socket,
  IN     UINT8             Ch,
  IN     UINT8             Strobe,
  IN     UINT8             Mode,
  IN OUT PXC_ATTACKER_SEL  Attacker[MaxPxcAttacker]
  )
{
  //
  //SPR_TODO_DDRIO. AttackerBit0/attackr0d0 was removed in SPR.
  //
  return SUCCESS;
}

/**
  Get and Set PXC Delta

  @param[in]      Host    - Pointer to sysHost
  @param[in]      Socket  - Socket number
  @param[in]      Ch      - Channel number (0-based)
  @param[in]      Dimm    - DIMM number (0-based)
  @param[in]      Rank    - Rank number (0-based)
  @param[in]      Strobe  - Strobe number (0-based)
  @param[in]      Bit     - Bit number (byte-scope)
  @param[in]      Group   - CSM_GT - Parameter to program
  @param[in]      Mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param[in,out]  Value   - Data to program

  @retval IO delay

**/
STATIC
MRC_STATUS
GetSetPxcDdjcDelta (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     Dimm,
  IN     UINT8     Rank,
  IN     UINT8     Strobe,
  IN     UINT8     Bit,
  IN     MRC_GT    Group,
  IN     UINT8     Mode,
  IN OUT INT16     *Value
  )
{
  MRC_STATUS                            Status = MRC_STATUS_SUCCESS;
  UINT8                                 MaxStrobe;
  UINT8                                 TargetStrobe;
  UINT8                                 LogicalBit = 0;
  UINT8                                 IndexBit = 0;
  UINT8                                 BitStart = 0;
  UINT8                                 BitStop  = 0;
  UINT8                                 SwizzleBit = 0;
  UINT8                                 LogRank;
  UINT16                                CurValPxc[MAX_BITS_IN_BYTE];
  UINT16                                CurValDdjc[MAX_BITS_IN_BYTE];
  UINT16                                PiDelay[MAX_BITS_IN_BYTE];
  INT16                                 Tmp[MAX_BITS_IN_BYTE];
  UINT16                                MaxLimit = 127;
  UINT16                                MinLimit = 0;
  UINT16                                UsDelay = 1;
  BOOLEAN                               ValueOutOfRange;
  UINT8                                 MSVx8 = GetMaxStrobeValid (Host) / 2;
  DDRIO_CHANNEL_NVDATA                  (*DdrioChannelNvList)[MAX_CH];
  DDRD_N0_TX_CTL1_RANK_0_MCIO_DDRIO_STRUCT     XtalkDeltaN0;
  DDRD_N0_TX_CTL1_RANK_0_MCIO_DDRIO_STRUCT     XtalkDeltaN1;
  DDRD_N0_TX_CTL2_RANK_0_MCIO_DDRIO_STRUCT     XtalkDdjcDeltaN0;
  DDRD_N0_TX_CTL2_RANK_0_MCIO_DDRIO_STRUCT     XtalkDdjcDeltaN1;
  struct rankDevice                     (*RankStruct)[MAX_RANK_DIMM];
  struct ddrRank                        (*RankList)[MAX_RANK_DIMM];
  struct dimmNvram                      (*DimmNvList)[MAX_DIMM];

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
  RankList = GetRankNvList (Host, Socket, Ch, Dimm);
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  //
  // Get the logical rank #
  //
  LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits (Host, DdrLevel, Group, &MinLimit, &MaxLimit, &UsDelay);

  if (Bit == ALL_BITS) {
    BitStart = 0;
    BitStop  = MAX_BITS_IN_BYTE;
  } else {
    BitStart = 0;
    BitStop  = 1;
  }

  //
  // Clear all variables
  //
  for (IndexBit = 0; IndexBit < MAX_BITS_IN_BYTE; IndexBit++) {
    CurValPxc[IndexBit] = 0;
    CurValDdjc[IndexBit] = 0;
    PiDelay[IndexBit] = 0;
    Tmp[IndexBit] = 0;
  }

  if (Strobe == ALL_STROBES) {
    Strobe = 0;
    MaxStrobe = MAX_STROBE / 2;
  } else {
    Strobe = Strobe % (MAX_STROBE / 2);
    MemCheckIndex (Strobe, MAX_STROBE / 2);
    MaxStrobe = Strobe + 1;
  }

  for (; Strobe < MaxStrobe; Strobe++) {

    TargetStrobe = Strobe;
    if (Bit != ALL_BITS) {
      //
      // Determine swizzled bit position within the nibble
      //
      LogicalBit = Bit % BITS_PER_NIBBLE;
      //
      // If the bit is >= 4, then it's a bit in the upper nibble
      //
      if (Bit >= BITS_PER_NIBBLE) {
        TargetStrobe += 9;
      }
      //
      // Get DQ swizzle bit
      //
      SwizzleBit = GetDqSwizzle (Ch, TargetStrobe, LogicalBit, DdrLevel, Group);
      //
      // bit != ALL_BIT
      // Loop should only target the SwizzleBit
      //
      BitStart = SwizzleBit;
      BitStop  = SwizzleBit + 1;
    }

    if (Mode & GSM_READ_CSR) {
      //
      // Read from PCI config space
      //
      XtalkDeltaN0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, LogRank, (Strobe % MSVx8), DDRD_N0_TX_CTL1_RANK_0_MCIO_DDRIO_REG));
      XtalkDeltaN1.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, LogRank, (Strobe % MSVx8) + MSVx8, DDRD_N0_TX_CTL1_RANK_0_MCIO_DDRIO_REG));
      XtalkDdjcDeltaN0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, LogRank, (Strobe % MSVx8), DDRD_N0_TX_CTL2_RANK_0_MCIO_DDRIO_REG));
      XtalkDdjcDeltaN1.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, LogRank, (Strobe % MSVx8) + MSVx8, DDRD_N0_TX_CTL2_RANK_0_MCIO_DDRIO_REG));
    } else {
      //
      // Read from cache
      //
      XtalkDeltaN0.Data = (UINT32)(*DdrioChannelNvList)[Ch].XtalkDeltaN0[LogRank][Strobe % MSVx8];
      XtalkDeltaN1.Data = (UINT32)(*DdrioChannelNvList)[Ch].XtalkDeltaN1[LogRank][(Strobe % MSVx8) + MSVx8];
      XtalkDdjcDeltaN0.Data = 0;  // Not used, no cache or csr write in this function
      XtalkDdjcDeltaN1.Data = 0;  // Not used
    }

    CurValPxc[0] = (UINT16)XtalkDeltaN0.Bits.pxc_delta0; // Bit 0
    CurValPxc[1] = (UINT16)XtalkDeltaN0.Bits.pxc_delta1; // Bit 1
    CurValPxc[2] = (UINT16)XtalkDeltaN0.Bits.pxc_delta2; // Bit 2
    CurValPxc[3] = (UINT16)XtalkDeltaN0.Bits.pxc_delta3; // Bit 3
    CurValPxc[4] = (UINT16)XtalkDeltaN1.Bits.pxc_delta0; // Bit 4
    CurValPxc[5] = (UINT16)XtalkDeltaN1.Bits.pxc_delta1; // Bit 5
    CurValPxc[6] = (UINT16)XtalkDeltaN1.Bits.pxc_delta2; // Bit 6
    CurValPxc[7] = (UINT16)XtalkDeltaN1.Bits.pxc_delta3; // Bit 7

    CurValDdjc[0] = (UINT16)XtalkDdjcDeltaN0.Bits.ddjc_delta0; // Bit 0
    CurValDdjc[1] = (UINT16)XtalkDdjcDeltaN0.Bits.ddjc_delta1; // Bit 1
    CurValDdjc[2] = (UINT16)XtalkDdjcDeltaN0.Bits.ddjc_delta2; // Bit 2
    CurValDdjc[3] = (UINT16)XtalkDdjcDeltaN0.Bits.ddjc_delta3; // Bit 3
    CurValDdjc[4] = (UINT16)XtalkDdjcDeltaN1.Bits.ddjc_delta0; // Bit 4
    CurValDdjc[5] = (UINT16)XtalkDdjcDeltaN1.Bits.ddjc_delta1; // Bit 5
    CurValDdjc[6] = (UINT16)XtalkDdjcDeltaN1.Bits.ddjc_delta2; // Bit 6
    CurValDdjc[7] = (UINT16)XtalkDdjcDeltaN1.Bits.ddjc_delta3; // Bit 7

    if (Group == DdjcDelta) {
      PiDelay[0] = CurValDdjc[0];
      PiDelay[1] = CurValDdjc[1];
      PiDelay[2] = CurValDdjc[2];
      PiDelay[3] = CurValDdjc[3];
      PiDelay[4] = CurValDdjc[4];
      PiDelay[5] = CurValDdjc[5];
      PiDelay[6] = CurValDdjc[6];
      PiDelay[7] = CurValDdjc[7];
    } else {
      PiDelay[0] = CurValPxc[0];
      PiDelay[1] = CurValPxc[1];
      PiDelay[2] = CurValPxc[2];
      PiDelay[3] = CurValPxc[3];
      PiDelay[4] = CurValPxc[4];
      PiDelay[5] = CurValPxc[5];
      PiDelay[6] = CurValPxc[6];
      PiDelay[7] = CurValPxc[7];
    }

    //
    // Check if read only
    //
    if (Mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      if (Group == DdjcDelta) {
        *Value = CurValDdjc[SwizzleBit];
      } else {
        *Value = CurValPxc[SwizzleBit];
      }
    } else {
      //
      // Write
      // Assume failure by setting ValutOutOfRange to TRUE. If ANY bit is in range, set ValutOutOfRange to FALSE.
      //
      ValueOutOfRange = TRUE;
      for (IndexBit = BitStart; IndexBit < BitStop; IndexBit++) {
        // Adjust the current value by offset
        if (Mode & GSM_WRITE_OFFSET) {
          //
          // Add the offset to the current value
          //
          if (Group == DdjcDelta) {
            Tmp[IndexBit] = CurValDdjc[IndexBit] + *Value;
          } else {
            Tmp[IndexBit] = CurValPxc[IndexBit] + *Value;
          }
        } else {
          Tmp[IndexBit] = *Value;
        }
        //
        // Make sure we do not exeed the limits
        //
        if (Tmp[IndexBit] >= MinLimit) {
          PiDelay[IndexBit] = Tmp[IndexBit];
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, Bit,
                          "%a Out of range!! tmp = 0x%x, minLimit = 0x%x\n", GetGroupStr (Group), Tmp[IndexBit], MinLimit);
          PiDelay[IndexBit] = MinLimit;
        }

        //
        // Ensure we do not exceed maximums
        //
        if (PiDelay[IndexBit] > MaxLimit) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, Bit,
                          "%a Out of range!! piDelay = 0x%x, maxLimit = 0x%x\n", GetGroupStr (Group), PiDelay[IndexBit], MaxLimit);
          PiDelay[IndexBit] = MaxLimit;
        }

        //
        // When setting DdjcDelta, validate that DdjcDelta + XtalkDelta does not exceed MAX_DDJC_PLUS_PXC.
        //
        if (Group != DdjcDelta) {
          //
          // "ValueOutOfRange==TRUE" only valid for DdjcDelta. Always set it to FALSE for PxcDelta.
          //
          ValueOutOfRange = FALSE;
        } else {
          if ((PiDelay[IndexBit] + CurValPxc[IndexBit]) > MAX_DDJC_PLUS_PXC) {
            //
            // This debug message is disabled because it is expected to appear frequently under typical circumstances. It is not an error.
            //
            RcDebugPrintWithDevice (SDBG_NONE, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, Bit == ALL_BITS ? IndexBit : Bit,
                            "DdjcDelta(0x%x) + PxcDelta(0x%x) out of range!! Capping DdjcDelta at 0x%x\n", PiDelay[IndexBit], CurValPxc[IndexBit], MAX_DDJC_PLUS_PXC - CurValPxc[IndexBit]);
            PiDelay[IndexBit] = MAX_DDJC_PLUS_PXC - CurValPxc[IndexBit];
          } else {
            //
            // If any of the bits are in range, even if some are not, indicate success
            //
            ValueOutOfRange = FALSE;
          }
        }
      } // IndexBit
      if (Bit != ALL_BITS) {
        //
        // Update the return value in case it is different from the value passed in
        // This is useful if the caller prints out the return value after calling with a value that is
        // found to be out of range.
        //
        *Value = PiDelay[SwizzleBit];
      }
      if (ValueOutOfRange) {
        Status = MRC_STATUS_FAILURE;
      }

      if (Group == DdjcDelta) {
        XtalkDdjcDeltaN0.Bits.ddjc_delta0 = PiDelay[0]; // Bit 0
        XtalkDdjcDeltaN0.Bits.ddjc_delta1 = PiDelay[1]; // Bit 1
        XtalkDdjcDeltaN0.Bits.ddjc_delta2 = PiDelay[2]; // Bit 2
        XtalkDdjcDeltaN0.Bits.ddjc_delta3 = PiDelay[3]; // Bit 3
        XtalkDdjcDeltaN1.Bits.ddjc_delta0 = PiDelay[4]; // Bit 4
        XtalkDdjcDeltaN1.Bits.ddjc_delta1 = PiDelay[5]; // Bit 5
        XtalkDdjcDeltaN1.Bits.ddjc_delta2 = PiDelay[6]; // Bit 6
        XtalkDdjcDeltaN1.Bits.ddjc_delta3 = PiDelay[7]; // Bit 7
      } else {
        XtalkDeltaN0.Bits.pxc_delta0 = PiDelay[0]; // Bit 0
        XtalkDeltaN0.Bits.pxc_delta1 = PiDelay[1]; // Bit 1
        XtalkDeltaN0.Bits.pxc_delta2 = PiDelay[2]; // Bit 2
        XtalkDeltaN0.Bits.pxc_delta3 = PiDelay[3]; // Bit 3
        XtalkDeltaN1.Bits.pxc_delta0 = PiDelay[4]; // Bit 4
        XtalkDeltaN1.Bits.pxc_delta1 = PiDelay[5]; // Bit 5
        XtalkDeltaN1.Bits.pxc_delta2 = PiDelay[6]; // Bit 6
        XtalkDeltaN1.Bits.pxc_delta3 = PiDelay[7]; // Bit 7
      }

      //
      // Write it back if the current data does not equal the cache value or if in force write mode
      //
      if ((XtalkDeltaN0.Data != (UINT32)(*DdrioChannelNvList)[Ch].XtalkDeltaN0[LogRank][Strobe % MSVx8]) ||
        (XtalkDeltaN1.Data != (UINT32)(*DdrioChannelNvList)[Ch].XtalkDeltaN1[LogRank][(Strobe % MSVx8) + MSVx8]) || (Mode & GSM_FORCE_WRITE)) {

        MemWritePciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, LogRank, (Strobe % MSVx8), DDRD_N0_TX_CTL1_RANK_0_MCIO_DDRIO_REG), XtalkDeltaN0.Data);
        MemWritePciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, LogRank, (Strobe % MSVx8) + MSVx8, DDRD_N0_TX_CTL1_RANK_0_MCIO_DDRIO_REG), XtalkDeltaN1.Data);//SPR_TODO_DDRIO. To update the parameter +9.
      }
    } // if read only

    //
    // Save to cache
    //
    if (Mode & GSM_UPDATE_CACHE) {
      (*DdrioChannelNvList)[Ch].XtalkDeltaN0[LogRank][Strobe % MSVx8]        = (UINT16)(XtalkDeltaN0.Data & 0xFFFF);
      (*DdrioChannelNvList)[Ch].XtalkDeltaN1[LogRank][(Strobe % MSVx8) + MSVx8]  = (UINT16)(XtalkDeltaN1.Data & 0xFFFF);
    }
  } // Strobe loop

  return Status;
} // GetSetPxcDdjcDelta

/**

  Translate the register value to TxEQCode acording to: EqCode1=
  (TxEQ * (( DrvStaticLegCfg + (rcompcode/3))*
  DataSegmentEnable)) / 128
  Translate the TxEQCode to register value acording to: TxEQ   =
  (128*EqCode1)/(( DrvStaticLegCfg + (rcompcode/3))*
  DataSegmentEnable)

  @param Host      - Pointer to sysHost
  @param Socket    - Socket
  @param Ch        - Channel
  @param Strobe    - Strobe
  @param RegVal    - RegVal to translate to EqCode
  @param Direction - Direction 1: RegVal->EqCode 0:
                   EqCode->RegVal

  @retval UINT8

**/
STATIC
UINT8
RegVal2EqCode (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Strobe,
  IN UINT8     Value,
  IN BOOLEAN   Direction
)
{
  UINT8                               EqCode;
  UINT8                               RegVal;
  UINT8                               DrvStaticLegCfg;
  UINT8                               DataSegmentEnable;
  UINT8                               RcompCode;
  DDRD_N0_TX_CTL0_MCIO_DDRIO_STRUCT   DdrDataTxCtl0;
  DDRIO_CHANNEL_NVDATA                (*DdrioChannelNvList)[MAX_CH];

  DdrioChannelNvList = GetDdrioChannelNvList(Host, Socket);
  DdrDataTxCtl0.Data = (*DdrioChannelNvList)[Ch].DdrDataTxCtl0[Strobe];
  DataSegmentEnable  = (UINT8)DdrDataTxCtl0.Bits.data_seg_en;
  RcompCode          = (*DdrioChannelNvList)[Ch].RcompCode[Strobe];
  DdrDataTxCtl0.Data = (*DdrioChannelNvList)[Ch].DdrDataTxCtl0[Strobe];
  DrvStaticLegCfg    = (UINT8)DdrDataTxCtl0.Bits.drv_static_leg_cfg;

  if (Direction == REG_TO_TXEQ) {
    RegVal = Value;
    EqCode = (UINT8)(((RegVal * ((DrvStaticLegCfg + (RcompCode / TXEQ_FACTOR2)) * DataSegmentEnable))) / TXEQ_FACTOR1);
    Value  = EqCode;
  } else {
    EqCode = Value;
    //
    // Apply the formula and avoid division by 0
    //
    if (((DrvStaticLegCfg + (RcompCode / TXEQ_FACTOR2)) * DataSegmentEnable) == 0) {
      RegVal = 0;
    } else {
      RegVal = (UINT8)((TXEQ_FACTOR1 * EqCode) / ((DrvStaticLegCfg + (RcompCode / TXEQ_FACTOR2)) * DataSegmentEnable));
    }
    Value  = RegVal;
  }

  return Value;
}

/*

  Dumps each value across each bit in the byte (2 Nibbles)

  @param[in]         Socket      Current socket under test (0-based)
  @param[in]         Channel     Current channel under test (0-based)
  @param[in]         Dimm        Current dimm under test (0-based)
  @param[in]         Rank        Current rank under test (0-based)
  @param[in]         Strobe      Current strobe under test (0-based)
  @param[in]         Bit         Current bit under test (0-based) or ALL_BITS
  @param[in]         Group       Parameter to be margined
  @param[in]         Caller      String that identifies the purpose to be calling this function

*/
VOID
DumpRxDelayBitRegs (
  IN        UINT8     Socket,
  IN        UINT8     Channel,
  IN        UINT8     Dimm,
  IN        UINT8     Rank,
  IN        UINT8     Strobe,
  IN        UINT8     Bit,
  IN        MRC_GT    Group,
  IN        CHAR8     *Caller
  )
{
}


/*

  Dumps each value across each bit in the byte (2 Nibbles)

  @param[in]         Socket      Current socket under test (0-based)
  @param[in]         Channel     Current channel under test (0-based)
  @param[in]         Dimm        Current dimm under test (0-based)
  @param[in]         Rank        Current rank under test (0-based)
  @param[in]         Strobe      Current strobe under test (0-based)
  @param[in]         Bit         Current bit under test (0-based) or ALL_BITS
  @param[in]         Group       Parameter to be margined
  @param[in]         Caller      String that identifies the purpose to be calling this function

*/
VOID
DumpTxDelayBitRegs (
  IN        UINT8     Socket,
  IN        UINT8     Channel,
  IN        UINT8     Dimm,
  IN        UINT8     Rank,
  IN        UINT8     Strobe,
  IN        UINT8     Bit,
  IN        MRC_GT    Group,
  IN        CHAR8     *Caller
  )
{
}

/*

  Dumps each value across each bit in the byte (2 Nibbles)

  @param[in]         Socket      Current socket under test (0-based)
  @param[in]         Channel     Current channel under test (0-based)
  @param[in]         Dimm        Current dimm under test (0-based)
  @param[in]         Rank        Current rank under test (0-based)
  @param[in]         Strobe      Current strobe under test (0-based)
  @param[in]         Bit         Current bit under test (0-based) or ALL_BITS
  @param[in]         Group       Parameter to be margined
  @param[in]         Caller      String that identifies the purpose to be calling this function

*/
VOID
DumpRxVrefBitRegs (
  IN        UINT8     Socket,
  IN        UINT8     Channel,
  IN        UINT8     Dimm,
  IN        UINT8     Rank,
  IN        UINT8     Strobe,
  IN        UINT8     Bit,
  IN        MRC_GT    Group,
  IN        CHAR8     *Caller
  )
{
}

/**

  Determine if it is appropriate to use the multicast offset register

  @param[in]     Strobe  - Strobe number (0-based)
  @param[in]     Bit     - Bit number
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - Bit-field of different modes

  @retval TRUE  - OK to use multicast registers
          FALSE - Cannot use multicast register
**/
BOOLEAN
UseDelayMulticastOffset (
  IN      UINT8     Strobe,
  IN      UINT8     Bit,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode
  )
{
  //
  // Don't use multicast registers if the feature is disabled in the build
  //
  if (!FeaturePcdGet (PcdUseRxTxMultiCastRegisters)) {
    return FALSE;
  }

  //
  // Can use multicast registers if these are all true:
  // - Group is TxDqDelay or RxDqsDelay or RxVref or RxSamplerEvenOdd
  // - All strobes and all bits are being set to the same value
  // - Targeting front-side (host) registers
  // - Writing an offset and is not a read only, nor a read from CSR
  //
  if (((Group != TxDqDelay) && (Group != RxDqsDelay) &&
      (Group != RxVref) && (Group != RxSamplerEvenOdd))   ||
      (Strobe != ALL_STROBES) || (Bit != ALL_BITS)   ||
      (Level  != DdrLevel)                              ||
      ((Mode & GSM_WRITE_OFFSET) == 0) ||
      ((Mode & GSM_READ_ONLY) != 0) ||
      ((Mode & GSM_READ_CSR) != 0)) {
    //
    // Can't use multicast register
    //
    return FALSE;
  }
  return TRUE;
}

/**

  Get/Set TxDq and RxDqs Delay using multicast offset registers.
  The value written to the multicast registers used in this routine is added to
  the unicast register values within the channel. The value does not replace the
  value programmed in the unicast registers.

  @param[in]     Host    - Pointer to sysHost
  @param[in]     Socket  - Socket number
  @param[in]     Ch      - Channel number within the socket
  @param[in]     Dimm    - DIMM number within the channel
  @param[in]     Rank    - Rank number within the channel
  @param[in]     Strobe  - Strobe number
  @param[in]     Bit     - Bit number
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - Bit-field of different modes
  @param[in,out] Value   - Pointer to delay value or offset based on mode

  @retval SUCCESS

**/
MRC_STATUS
GetSetDelayMulticastOffset (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      UINT8     Bit,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  UINT16                                    RawValue;
  INT16                                     MaxLimit = 0;
  INT16                                     MinLimit = 0;
  UINT16                                    UsDelay = 0;
  INT16                                     PiDelay;
  INT16                                     CurrentValue = 0;
  INT16                                     NewValue;
  DDRIO_CHANNEL_NVDATA                      (*DdrioChannelNvList)[MAX_CH];
  DDRD_N0_TRAIN_OFFSET0_MC_DDRIOMC_STRUCT   DdrdTrainOffset0;
  MRC_STATUS  Status;

  Status = MRC_STATUS_SUCCESS;

  //
  // Don't use multicast registers if:
  // - The feature is disabled in the build
  // - Signal group is not supported
  // - Not targeting all strobes and bits
  // - Level is not DdrLevel (frontside)
  //
  if (!FeaturePcdGet (PcdUseRxTxMultiCastRegisters)) {
    if ((Mode & GSM_READ_ONLY) != 0) {
      //
      // If the feature is disabled, reads must return 0
      //
      *Value = 0;
    }
    return MRC_STATUS_FAILURE;
  }

  if ((Group != RxDqsPerChDelayOffset) && (Group != TxDqPerChDelayOffset) &&
      (Group != RxVrefPerChDelayOffset) && (Group != RxSamplerPerChDelayOffset)) {
    return MRC_STATUS_FAILURE;
  }

  if (!((Strobe == ALL_STROBES) || (Bit == ALL_BITS))) {
    return MRC_STATUS_FAILURE;
  }

  if (Level  != DdrLevel) {
    return MRC_STATUS_FAILURE;
  }

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits (Host, DdrLevel, Group, (UINT16 *)&MinLimit, (UINT16 *)&MaxLimit, &UsDelay);

  if (Mode & GSM_READ_CSR) {
    //
    // Read nibble 0 register from MMIO space
    //
    DdrdTrainOffset0.Data = MemReadPciCfgEp (Socket, Ch, DDRD_N0_TRAIN_OFFSET0_MC_DDRIOMC_REG);
  } else {
    //
    // Read from cache
    //
    DdrdTrainOffset0.Data = (*DdrioChannelNvList)[Ch].DdrdTrainOffset0;
  }

  switch (Group) {
    case TxDqPerChDelayOffset:
      RawValue = (UINT16)DdrdTrainOffset0.Bits.txdq_dly_adj;
      CurrentValue = (INT16)ConvertTwosComplementToInt (RawValue, 7);
      break;
    case RxDqsPerChDelayOffset:
      RawValue = (UINT16)DdrdTrainOffset0.Bits.rxdqs_dly_adj;
      CurrentValue = (INT16)ConvertTwosComplementToInt (RawValue, 7);
      break;
    case RxVrefPerChDelayOffset:
      RawValue = (UINT16)DdrdTrainOffset0.Bits.vref_adj;
      CurrentValue = (INT16)ConvertTwosComplementToInt (RawValue, 6);
      break;
    case RxSamplerPerChDelayOffset:
      RawValue = (UINT16)DdrdTrainOffset0.Bits.sampler_offset_adj;
      CurrentValue = (INT16)ConvertTwosComplementToInt (RawValue, 5);
      break;
    default:
      break;
  } // switch

  //
  // Check if read only
  //
  if (Mode & GSM_READ_ONLY) {
    *Value = CurrentValue;
  } else {
    //
    // Write
    //
    if (Mode & GSM_WRITE_OFFSET) {
      NewValue = CurrentValue + *Value;
    } else {
      NewValue = *Value;
    }

    if (NewValue >= MinLimit) {
      PiDelay = NewValue;
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "%a Multicast Out of range!! NewValue = 0x%x, MinLimit = 0x%x\n", GetGroupStr (Group), NewValue, MinLimit);
      PiDelay = MinLimit;

      Status = MRC_STATUS_OUT_OF_RESOURCES;
    }

    // Ensure we do not exceed maximums
    if (PiDelay > MaxLimit) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "%a Multicast Out of range!! piDelay = 0x%x, MaxLimit = 0x%x\n", GetGroupStr (Group), PiDelay, MaxLimit);
      PiDelay = MaxLimit;
      Status = MRC_STATUS_OUT_OF_RESOURCES;
    }

    switch (Group) {
      case TxDqPerChDelayOffset:
        DdrdTrainOffset0.Bits.txdq_dly_adj = PiDelay;
        break;
      case RxDqsPerChDelayOffset:
        DdrdTrainOffset0.Bits.rxdqs_dly_adj = PiDelay;
        break;
      case RxVrefPerChDelayOffset:
        DdrdTrainOffset0.Bits.vref_adj = PiDelay;
        break;
      case RxSamplerPerChDelayOffset:
        DdrdTrainOffset0.Bits.sampler_offset_adj = PiDelay;
        break;
      default:
        break;
    } // switch

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    // No need to write to nibble 1 - writes to nibble 0 are applied to nibble 1
    //
    if ((DdrdTrainOffset0.Data != (*DdrioChannelNvList)[Ch].DdrdTrainOffset0) || (Mode & GSM_FORCE_WRITE)) {
      MemWritePciCfgEp (Socket, Ch, DDRD_N0_TRAIN_OFFSET0_MC_DDRIOMC_REG, DdrdTrainOffset0.Data);
    }

    //
    // Wait for the new value to settle
    //
    FixedDelayMicroSecond (UsDelay);
  } // if read only

  //
  // Save to cache
  //
  if (Mode & GSM_UPDATE_CACHE) {
    (*DdrioChannelNvList)[Ch].DdrdTrainOffset0 = DdrdTrainOffset0.Data;
  }

  return Status;
} // GetSetDelayMulticastOffset


/**

  Get TxDelay

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - Bit-field of different modes
  @param value   - Pointer to delay value or offset based on mode

  @retval IO delay

**/
MRC_STATUS
GetSetTxDelayUnicast (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
{

  MRC_STATUS                                Status;
  UINT8                                     logRank;
  UINT8                                     maxStrobe;
  UINT8                                     SubChannel;
  UINT16                                    curVal = 0;
  UINT16                                    piDelay;
  INT16                                     ValueArray[BITS_PER_NIBBLE];
  INT16                                     tmp;
  UINT8                                     Index;
  UINT16                                    maxLimit = 0;
  UINT16                                    minLimit = 0;
  UINT16                                    usDelay = 0;
  DDRIO_CHANNEL_NVDATA                      (*DdrioChannelNvList)[MAX_CH];
  DDRD_N0_TX_CTL0_RANK_0_MCIO_DDRIO_STRUCT  DdrdN0TxCtl0;
#ifdef LRDIMM_SUPPORT
  UINT8                                     updateNeeded;
  BOOLEAN                                   CompNeeded;
  BOOLEAN                                   (*NibbleSwap) [MAX_CH][MAX_DIMM][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT8                                     tmpStrobe;
  struct dimmNvram                          (*dimmNvList)[MAX_DIMM];
  struct rankDevice                         (*rankStruct)[MAX_RANK_DIMM];
  DDR5_LRDIMM_TRAINING_DATA                 *TrainingData;
  UINT8                                     CachedPage;
#endif //LRDIMM_SUPPORT
  UINT8                                     MSVx4 = GetMaxStrobeValid(Host);
  UINT8                                     MSVx8 = GetMaxStrobeValid(Host) / 2;
  UINT8                                     MaxStrobePairs = GetSubChMaxStrobeValid (Host) / 2;

  DdrioChannelNvList = GetDdrioChannelNvList(Host, socket);
  CompNeeded = FALSE;

#ifdef LRDIMM_SUPPORT
  rankStruct  = GetRankStruct(Host, socket, ch, dimm);
  dimmNvList = GetDimmNvList(Host, socket, ch);
#endif //LRDIMM_SUPPORT

  DdrdN0TxCtl0.Data = 0;

  //
  // SPR only has per-bit TxDqDelay registers
  // Redirect TxDqDelay to GetSetTxDelayBit with bit set to ALL_BITS and pass an array of 4 bit values
  // For writes, set all bits in the nibble to *value
  // For reads, return the value from array index 0
  //
  if (level == DdrLevel && group == TxDqDelay) {
    for (Index = 0; Index < sizeof (ValueArray) / sizeof (ValueArray[0]); Index++) {
      ValueArray[Index] = *value;
    }
    Status = GetSetTxDelayBit (Host, socket, ch, dimm, rank, strobe, ALL_BITS, DdrLevel, TxDqBitDelay, mode, ValueArray);
    *value = ValueArray[0];
    return Status;
  }

  //
  // Get the logical rank #
  //
  logRank = GetLogicalRank(Host, socket, ch, dimm, rank);

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits(Host, level, group, &minLimit, &maxLimit, &usDelay);

  if ((strobe >= MAX_STROBE) && (strobe != ALL_STROBES)) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                   "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((strobe < MAX_STROBE || strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

#ifdef LRDIMM_SUPPORT
  //
  // Handle Nibble Swapped Nibble, 0/5, 1/2 and so on...
  //
  if (level == LrbufLevel) {
    Status = GetDdr5LrdimmTrainingData (&TrainingData);
    NibbleSwap = &(TrainingData->NibbleSwap);
    updateNeeded = 0;
    if (strobe != ALL_STROBES) {
      SubChannel = (strobe > 9) ? 1 : 0;
      tmpStrobe = strobe % MaxStrobePairs;
      if ((*NibbleSwap) [ch][dimm][SubChannel][tmpStrobe] == TRUE) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
          "Nibble swap is detected\n");
        if (strobe < MaxStrobePairs) {
          strobe += MaxStrobePairs;
        } else if (strobe < MaxStrobePairs * 2) {
          strobe -= MaxStrobePairs;
        } else if (strobe < MaxStrobePairs * 3) {
          strobe += MaxStrobePairs;
        } else if (strobe < MaxStrobePairs * 4) {
          strobe -= MaxStrobePairs;
        }
      }
    }
  }
#endif //LRDIMM_SUPPORT

  if (strobe == ALL_STROBES) {
    strobe      = 0;
    maxStrobe   = MSVx4;
  } else {
    maxStrobe = strobe + 1;
  }

  for ( ; strobe < maxStrobe; strobe++) {
    if (strobe >= MAX_STROBE) {
      return MRC_STATUS_STROBE_NOT_PRESENT;
    }

    SubChannel = (strobe > 9) ? 1 : 0;
#ifdef LRDIMM_SUPPORT
    if (level == LrbufLevel) {
      if (mode & GSM_READ_CSR) {
        //
        // Read from "actual setting" cache
        //
        switch (group) {
        case TxDqDelay:
          curVal = (*dimmNvList)[dimm].rankList[rank].LrBuf_PG01RWE6E7[strobe];
          break;
        case TxDqsDelay:
          curVal = (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCAxBx[strobe];
          break;
        case TxDqsDelayCycle:
          curVal = (((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[strobe % MSVx8] >> (4 * (strobe / MSVx8))) & 0xF);
          break;
        default:
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                         "GetSetTxDelay called unsupported group!\n");
          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_54);
          break;
        } //switch group
      } else {
        //
        // Read from setting cache
        //
        switch (group) {
        case TxDqDelay:
            curVal = (*rankStruct) [rank].cachedLrBuf_PG01RWE6E7[strobe];
          break;
        case TxDqsDelay:
          curVal = (*rankStruct)[rank].cachedLrBuf_FxBCAxBx[strobe];
          break;
        case TxDqsDelayCycle:
          curVal = (((*rankStruct)[rank].cachedLrBuf_FxBCDxFx[strobe % MSVx8] >> (4 * (strobe / MSVx8))) & 0xF);
          break;
        default:
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                         "GetSetTxDelay called unsupported group!\n");
          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL,  RC_FATAL_ERROR_MINOR_CODE_55);
          break;
        } //switch group
      }
    } else
#endif //LRDIMM_SUPPORT
    {

      if (mode & GSM_READ_CSR) {
        //
        // Read from PCI config space
        //
        if ((group == TxDqsDelay) || (group == WrLvlDelay) || (group == TxEq) || (group == TxDqsPieOffset)) {
          DdrdN0TxCtl0.Data = MemReadPciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_TX_CTL0_RANK_0_MCIO_DDRIO_REG));
        }
      } else {
        //
        // Read from cache
        //
        if ((group == TxDqsDelay) || (group == WrLvlDelay) || (group == TxEq) || (group == TxDqsPieOffset)) {
          DdrdN0TxCtl0.Data = (*DdrioChannelNvList)[ch].txGroup0[logRank][strobe];
        }
      }

      switch (group) {
        case WrLvlDelay:
        case TxDqsDelay:
          curVal = (UINT16)DdrdN0TxCtl0.Bits.txdqs_dly;
          break;
        case TxDqsPieOffset:
          curVal = (UINT16)DdrdN0TxCtl0.Bits.txdqs_pie_offset;
          break;
#if 0
        case TxEq:
          curVal = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) txGroup0.Bits.txeq, REG_TO_TXEQ);
          break;

        case TxEqTap2:
          switch (logRank) {
          case 0:
            curVal = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) TxEqCoefTap2.Bits.rank0code, REG_TO_TXEQ);
            break;
          case 1:
            curVal = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) TxEqCoefTap2.Bits.rank1code, REG_TO_TXEQ);
            break;
          case 2:
            curVal = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) TxEqCoefTap2.Bits.rank2code, REG_TO_TXEQ);
            break;
          case 3:
            curVal = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) TxEqCoefTap2.Bits.rank3code, REG_TO_TXEQ);
            break;
          case 4:
            curVal = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) TxEqCoefTap2.Bits.rank4code, REG_TO_TXEQ);
            break;
          case 5:
            curVal = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) TxEqCoefTap2.Bits.rank5code, REG_TO_TXEQ);
            break;
          case 6:
            curVal = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) TxEqCoefTap2.Bits.rank6code, REG_TO_TXEQ);
            break;
          case 7:
            curVal = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) TxEqCoefTap2.Bits.rank7code, REG_TO_TXEQ);
            break;
          default:
            // return error on parameter
            break;
          }
          break;

#endif //if 0
        default:
          break;
      } // switch
    }

    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      *value = curVal;
    } else {
      //
      // Write
      //

      // Adjust the current CMD delay value by offset
      if (mode & GSM_WRITE_OFFSET) {
        tmp = curVal + *value;
      } else {
        tmp = *value;
      }

      if (tmp >= minLimit) {
        piDelay = tmp;
      } else {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                        "%a Out of range!! tmp = 0x%x, minLimit = 0x%x\n", GetGroupStr (group), tmp, minLimit);
        piDelay = minLimit;
      }

      // Ensure we do not exceed maximums
      if (piDelay > maxLimit) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                        "%a Out of range!! piDelay = 0x%x, maxLimit = 0x%x\n", GetGroupStr (group), piDelay, maxLimit);
        piDelay = maxLimit;
      }

#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        curVal = piDelay;
        switch (group) {
          case TxDqDelay:
            if ((*dimmNvList)[dimm].rankList[rank].LrBuf_PG01RWE6E7[strobe] != (UINT8) (curVal)) {
              updateNeeded = 1;
              (*dimmNvList)[dimm].rankList[rank].LrBuf_PG01RWE6E7[strobe] = (UINT8) (curVal);
            }
            break;
          case TxDqsDelay:
            if ((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCAxBx[strobe] != (UINT8) curVal) {
              updateNeeded = 1;
              (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCAxBx[strobe] = (UINT8) curVal;
            }
            break;
          case TxDqsDelayCycle:
            if ((((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[strobe % MSVx8]>>(4 * (strobe / MSVx8))) & 0xF) != (UINT8) curVal) {
              updateNeeded = 1;
              (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[strobe % MSVx8] &= 0xF0 >> (4 * (strobe / MSVx8));
              (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[strobe % MSVx8] |= (UINT8) curVal<<(4 * (strobe / MSVx8));
            }
            break;
          default:
            break;
        }
      } else
#endif  //LRDIMM
      {
        switch (group) {
          case WrLvlDelay:
          case TxDqsDelay:
            DdrdN0TxCtl0.Bits.txdqs_dly = piDelay;
            break;
          case TxDqsPieOffset:
            DdrdN0TxCtl0.Bits.txdqs_pie_offset = piDelay;
            break;
#if 0 //SPR_TODO.
          case TxEq:
            txGroup0.Bits.txeq = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) piDelay, TXEQ_TO_REG);
            break;

          case TxEqTap2:
            piDelay = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) piDelay, TXEQ_TO_REG);
            switch (logRank) {
            case 0:
              TxEqCoefTap2.Bits.rank0code = piDelay;
              break;
            case 1:
              TxEqCoefTap2.Bits.rank1code = piDelay;
              break;
            case 2:
              TxEqCoefTap2.Bits.rank2code = piDelay;
              break;
            case 3:
              TxEqCoefTap2.Bits.rank3code = piDelay;
              break;
            case 4:
              TxEqCoefTap2.Bits.rank4code = piDelay;
              break;
            case 5:
              TxEqCoefTap2.Bits.rank5code = piDelay;
              break;
            case 6:
              TxEqCoefTap2.Bits.rank6code = piDelay;
              break;
            case 7:
              TxEqCoefTap2.Bits.rank7code = piDelay;
              break;
            default:
              //return error invalid rank parameter
              break;
            }
           break;

#endif //if 0
          default:
            break;
        } // switch

        //
        // Write it back if the current data does not equal the cache value or if in force write mode
        //
        if ((DdrdN0TxCtl0.Data != (*DdrioChannelNvList)[ch].txGroup0[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
          if ((group == TxDqsDelay) || (group == WrLvlDelay) || (group == TxEq) || (group == TxDqsPieOffset)) {
            MemWritePciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_TX_CTL0_RANK_0_MCIO_DDRIO_REG), DdrdN0TxCtl0.Data);
          }
        }

#if 0 //SPR_TODO.
        if ((TxEqCoefTap2.Data != (*DdrioChannelNvList)[ch].TxEqCoefTap2[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
          if (group == TxEqTap2) {
            MemWritePciCfgEp (socket, ch, UpdateIoRegisterCh10nm (Host, strobe, DATATXEQCOEF_MCIO_DDRIO_REG), TxEqCoefTap2.Data);
          }
        }

#endif //if 0
        //
        // Wait for the new value to settle
        //
        FixedDelayMicroSecond (usDelay);
      }
    } // if read only

#ifdef LRDIMM_SUPPORT
    if ((level == LrbufLevel) && !(mode & GSM_READ_ONLY) && ((updateNeeded) || (mode & GSM_FORCE_WRITE))) {
      switch (group) {
        case TxDqDelay:
          //
          // Select appropriate buffer device, PBA ID: 0~4 for both subchannel
          //
          DbControlWordWrite (Host, socket, ch, (1 << SubChannel), dimm, rank, DDR5_DATA_BUFFER_RW93_REG, strobe % MaxStrobePairs);

          //
          // Select corresponding page for DB rank setting
          //
          ReadDbCwCache (socket, ch, SubChannel, dimm, strobe % MaxStrobePairs, LRDIMM_DB_PAGE0, RDIMM_RWDF, &CachedPage);
          if (CachedPage != rank) {
            DbControlWordWrite (Host, socket, ch, (1 << SubChannel), dimm, rank, RDIMM_RWDF, rank);
          }

          if ((strobe % 10) < MaxStrobePairs) {
            DbControlWordWrite (Host, socket, ch, (1 << SubChannel), dimm, rank, DDR5_DATA_BUFFER_PG0_RWE6_REG, (*dimmNvList)[dimm].rankList[rank].LrBuf_PG01RWE6E7[strobe]);
          } else {
            DbControlWordWrite (Host, socket, ch, (1 << SubChannel), dimm, rank, DDR5_DATA_BUFFER_PG0_RWE7_REG, (*dimmNvList)[dimm].rankList[rank].LrBuf_PG01RWE6E7[strobe]);
          }
          DbControlWordWrite (Host, socket, ch, (1 << SubChannel), dimm, rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);
          break;
        default:
          break;
      } // switch
    }
#endif  //LRDIMM

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        switch (group) {
          case TxDqDelay:
              (*rankStruct)[rank].cachedLrBuf_PG01RWE6E7[strobe] = (*dimmNvList)[dimm].rankList[rank].LrBuf_PG01RWE6E7[strobe];
            break;
          default:
            break;
        } // switch
      } else
#endif  //LRDIMM
      {
        if ((group == TxDqsDelay) || (group == WrLvlDelay) || (group == TxEq) || (group == TxDqsPieOffset)) {
          (*DdrioChannelNvList)[ch].txGroup0[logRank][strobe] = DdrdN0TxCtl0.Data;
        }
      } //level
    }
  } // strobe loop
  if (CompNeeded) {
    DoComp (Host, socket);
  }

  return MRC_STATUS_SUCCESS;
} // GetSetTxDelayUnicast

/**

  Get/Set unicast registers for an specific group delay

  @param[in]      Host        Pointer to SysHost structure
  @param[in]      Socket      Current socket under test (0-based)
  @param[in]      Ch          Current channel under test (0-based)
  @param[in]      Dimm        Current dimm under test (0-based)
  @param[in]      Rank        Current rank under test (0-based)
  @param[in]      Strobe      Current strobe under test (0-based || ALL_STROBES)
  @param[in]      Bit         Current bit under test (0-based || ALL_BITS)
  @param[in]      Level       Current level (not applicable)
  @param[in]      Group       Current group under test
  @param[in]      Mode        Register/Cache access mode (GSM_READ_CSR, GSM_READ_ONLY, GSM_UPDATE_CACHE, GSM_FORCE_WRITE)
  @param[in,out]  Value       Pointer to data that will be used to retrieve or store

  @retval         MRC_STATUS_SUCCESS  Executed properly thru multicast or unicast registers
  @retval         Any other           Failure to write/read the register(s)

**/
MRC_STATUS
GetSetDelayUnicast (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      UINT8     Bit,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  MRC_STATUS Status = MRC_STATUS_SUCCESS;

  if (Group == TxDqDelay) {

    Status = GetSetTxDelayUnicast (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, Group, Mode, Value);

  } else if (Group == RxDqsDelay) {

    Status = GetSetRxDelayUnicast (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, Group, Mode, Value);

  } else if (Group == RxVref) {

    Status = GetSetRxVrefUnicast (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, Group, Mode, Value);

  } else if (Group == RxSamplerEvenOdd) {

    Status = GetSetRxSamplerEvenOddUnicast (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, Group, Mode, Value);

  } else {

    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_244);

  }

  if (Status != MRC_STATUS_SUCCESS) {

    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_246);

  }

  return Status;
}

/**

  This function will program the multicast register to zero and
  unicast registers will be OFFSET by the amount specified in Value

  @param[in]      Host              Pointer to syshost structure
  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      Dimm              Current dimm under test (0-based)
  @param[in]      Rank              Current rank under test (0-based within dimm)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be modified/read
  @param[in]      MulticastGroup    Specifies the MRC_GT multicast group that will be modified/read
  @param[in,out]  Value             Pointer to store/retrieve a value

  @retval         MRC_STATUS_SUCCESS  When the function was able to flush the multicast value into unicast registers
  @retval         Any other           When the function was not able to flush the multicast value into unicast registers

**/
MRC_STATUS
EFIAPI
FlushMulticastIntoUnicast (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      MRC_GT    MulticastGroup,
  IN OUT  INT16     *Value
  )
{

  INT16   ZeroValue;
  MRC_STATUS Status;

  ZeroValue = 0;

  RcDebugPrintWithDevice (
    SDBG_NONE, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "%a FlushMulticastIntoUnicast  *Value = %d LocalValue16 = %d\n",
    GetGroupStr (MulticastGroup),
    *Value,
    ZeroValue
    );


  GetSetDelayMulticastOffset (Host, Socket, Ch, Dimm, Rank, ALL_STROBES, ALL_BITS, Level, MulticastGroup, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &ZeroValue);

  Status = GetSetDelayUnicast (Host, Socket, Ch, Dimm, Rank, ALL_STROBES, ALL_BITS, Level, Group, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, Value);
  

  return Status;
}

/**

  This function will program the multicast register to zero and
  unicast registers will OFFSSET them by the amount specified in Value

  @param[in]      Host              Pointer to syshost structure
  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      Dimm              Current dimm under test (0-based)
  @param[in]      Rank              Current rank under test (0-based within dimm)
  @param[in]      Strobe            Current strobe under test (0-based, or ALL_STROBES)
  @param[in]      Bit               Current bit under test (0-based, or ALL_BITS)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be modified/read
  @param[in]      MulticastGroup    Specifies the MRC_GT multicast group that will be modified/read
  @param[in]      Mode              Mode to access the register
                                    GSM_READ_CSR: Read register directly instead of cache value
                                    GSM_READ_ONLY: Only read, skip write portion
                                    GSM_WRITE_OFFSET: Use the value as an offset to original
                                    GSM_UPDATE_CACHE: Update cache

  @param[in,out]  Value             Pointer to store/retrieve a value

  @retval         MRC_STATUS_SUCCESS  When the function was able to execute multicast/unicast API
  @retval         Any other           When the function

**/
MRC_STATUS
EFIAPI
GetSetMulticastUnicast (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      UINT8     Bit,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      MRC_GT    MulticastGroup,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  MRC_STATUS  Status;
  BOOLEAN     UseMulticast;
  INT16       OriginalOffset;

  Status = MRC_STATUS_SUCCESS;

  if (!FeaturePcdGet (PcdUseRxTxMultiCastRegisters)) {

    Status = GetSetDelayUnicast (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, Group, Mode, Value);

  } else {

    UseMulticast  = UseDelayMulticastOffset (Strobe, Bit, Level, Group, Mode);

    Status  = GetSetDelayMulticastOffset (Host, Socket, Ch, Dimm, Rank, ALL_STROBES, ALL_BITS, Level, MulticastGroup, GSM_READ_ONLY, &OriginalOffset);

    if (Status != MRC_STATUS_SUCCESS) {

      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_245);

    }

    if ((Mode & GSM_FLUSH_OFFSET) != 0 && OriginalOffset != 0) {
      //
      // Force flush command from multicast to unicast registers
      //
      FlushMulticastIntoUnicast (Host, Socket, Ch, Dimm, Rank, Level, Group, MulticastGroup, &OriginalOffset);
      OriginalOffset = 0;
    }

    if (!UseMulticast) {
      if (OriginalOffset != 0) {
        RcDebugPrintWithDevice (
          SDBG_NONE, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "%a !UseMulticast  Mode = 0x%02x OriginalOffset !=0\n",
          GetGroupStr (MulticastGroup),
          Mode
          );
        //
        // When multicast offset will not be used and the current value is different than zero,
        // we need to 'flush' the current multicast value into each unicast register
        //
        FlushMulticastIntoUnicast (Host, Socket, Ch, Dimm, Rank, Level, Group, MulticastGroup, &OriginalOffset);

      }

      Status = GetSetDelayUnicast (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, Group, Mode, Value);

    } else {

      Status  = GetSetDelayMulticastOffset (Host, Socket, Ch, Dimm, Rank, ALL_STROBES, ALL_BITS, Level, MulticastGroup, Mode, Value);

      if (Status == MRC_STATUS_OUT_OF_RESOURCES) {
        //
        // Multicast register hit the limit (MRC_STATUS_OUT_OF_RESOURCES) so we need to flush the multicast register into
        // unicast register, although, while doing the flush, add the Value that was intended for the multicast register
        //
        OriginalOffset = OriginalOffset + *Value;

        FlushMulticastIntoUnicast (Host, Socket, Ch, Dimm, Rank, Level, Group, MulticastGroup, &OriginalOffset);

      } else if (Status != MRC_STATUS_SUCCESS) {

        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_247);

      }

    }

  }
  return Status;
}

/**

  Get TxDelay
  Wrapper for multicast and unicast versions of GetSetTxDelay

  @param[in]     Host    - Pointer to sysHost
  @param[in]     Socket  - Socket number
  @param[in]     Ch      - Channel number (0-based)
  @param[in]     Dimm    - DIMM number (0-based)
  @param[in]     Rank    - Rank number (0-based)
  @param[in]     Strobe  - Strobe number (0-based)
  @param[in]     Bit     - Bit number
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - Bit-field of different modes
  @param[in,out] Value   - Pointer to delay value or offset based on mode

  @retval IO delay

**/
MRC_STATUS
GetSetTxDelay (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      UINT8     Bit,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  MRC_STATUS Status;

  if (Group == TxDqDelay && Level == DdrLevel) {

    Status = GetSetMulticastUnicast (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, Group, TxDqPerChDelayOffset, Mode, Value);

  } else {

    Status = GetSetTxDelayUnicast (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, Group, Mode, Value);

  }

  return Status;
}

/**

  Get TxDelayBit

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Pointer data to read or write. For writes, must point to an array of values for every bit in the nibble

  @retval IO delay

**/
STATIC
MRC_STATUS
GetSetTxDelayBit (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT8                               logRank;
  UINT8                               maxStrobe;
  UINT8                               SwizzleBit;
  UINT16                              piValBit[BITS_PER_NIBBLE];
  UINT16                              MinValBit;
  INT16                               tmp[BITS_PER_NIBBLE];
  UINT16                              maxLimit = 0;
  UINT16                              minLimit = 0;
  UINT16                              usDelay = 0;
  UINT8                               MaxBits;
  UINT8                               MinBits;
  DDRIO_CHANNEL_NVDATA                (*DdrioChannelNvList)[MAX_CH];
  DDRD_N0_TX_CTL1_RANK_0_MCIO_DDRIO_STRUCT DdrdN0TxCtl1;
  DDRD_N0_TX_CTL2_RANK_0_MCIO_DDRIO_STRUCT DdrdN0TxCtl2;
  UINT8 MSVx4 = GetMaxStrobeValid(Host);

  DdrioChannelNvList = GetDdrioChannelNvList(Host, socket);

  DdrdN0TxCtl1.Data = 0;
  DdrdN0TxCtl2.Data = 0;
  //
  // Get the logical rank #
  //
  logRank = GetLogicalRank(Host, socket, ch, dimm, rank);
  ZeroMem (tmp, sizeof (tmp));
  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits(Host, level, group, &minLimit, &maxLimit, &usDelay);

  if (strobe >= MAX_STROBE && strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                   "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((strobe < MAX_STROBE || strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (strobe == ALL_STROBES) {
    strobe    = 0;
    maxStrobe = MSVx4;
  } else {
    maxStrobe = strobe + 1;
  }
  //
  // bit must be either 0-3 or ALL_BITS, nothing else is supported
  //
  RC_FATAL_ERROR (bit < BITS_PER_NIBBLE || bit == ALL_BITS, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_56);
  if (bit == ALL_BITS) {
    MinBits = 0;;
    MaxBits = BITS_PER_NIBBLE;
  } else {
    MinBits = bit;
    MaxBits = bit + 1;
  }
  //
  //Assert on error if the MinBits is greater than or equal to 4.
  //
  RC_FATAL_ERROR (MinBits < 4, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_56);

  for ( ; strobe < maxStrobe; strobe++) {

    if (mode & GSM_READ_CSR) {
      //
      // Read from PCI config space
      //
      DdrdN0TxCtl1.Data = MemReadPciCfgEp (socket, ch, UpdateIoRegister10nm(Host, ch, logRank, strobe, DDRD_N0_TX_CTL1_RANK_0_MCIO_DDRIO_REG));
      DdrdN0TxCtl2.Data = MemReadPciCfgEp (socket, ch, UpdateIoRegister10nm(Host, ch, logRank, strobe, DDRD_N0_TX_CTL2_RANK_0_MCIO_DDRIO_REG));
    } else {
      //
      // Read from cache.
      //
      DdrdN0TxCtl1.Data = (*DdrioChannelNvList)[ch].txGroup1[logRank][strobe];
      DdrdN0TxCtl2.Data = (*DdrioChannelNvList)[ch].txGroup2[logRank][strobe];
    }


    piValBit[0] = (UINT16)DdrdN0TxCtl1.Bits.txdq_dly0;
    piValBit[1] = (UINT16)DdrdN0TxCtl1.Bits.txdq_dly1;
    piValBit[2] = (UINT16)DdrdN0TxCtl2.Bits.txdq_dly2;
    piValBit[3] = (UINT16)DdrdN0TxCtl2.Bits.txdq_dly3;

    MinValBit = MAX_INT16;
    for (bit = MinBits; bit < MaxBits; bit++) {
      //
      // Get Swizzle Bit
      //
      SwizzleBit = GetDqSwizzle (ch, strobe, bit, level, group);
      //
      // Check if read only
      //
      if (mode & GSM_READ_ONLY) {
        //
        // Save the value read into the output parameter
        // When reading the entire nibble, find the minimum value in the nibble
        //
        if (piValBit[SwizzleBit] < MinValBit) {
          MinValBit = piValBit[SwizzleBit];
        }
        *value = MinValBit;

      } else {
        //
        // Write
        //
        if (mode & GSM_WRITE_OFFSET) {
          //
          // Add the offset to the current value
          //
          tmp[SwizzleBit] = piValBit[SwizzleBit] + value[bit];

        } else {
          //
          // Write absolute value
          //
          tmp[SwizzleBit] = value[bit];
        }

        //
        // Make sure we do not exeed the limits
        //
        if (tmp[SwizzleBit] < minLimit) {
          RcDebugPrintWithDevice(SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, bit,
            "%a Out of range!! tmp[%d] = 0x%x, minLimit = 0x%x\n", GetGroupStr(group), bit, tmp[SwizzleBit], minLimit);
          tmp[SwizzleBit] = minLimit;
        }
        // Ensure we do not exceed maximums
        if (tmp[SwizzleBit] > maxLimit) {
          RcDebugPrintWithDevice(SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, bit,
            "%a Out of range!! tmp[%d] = 0x%x, maxLimit = 0x%x\n", GetGroupStr(group), bit, tmp[SwizzleBit], maxLimit);
          tmp[SwizzleBit] = maxLimit;
        }
      }
    }//bit

    //
    // Update CSR settings if not read only
    //
    if ((mode & GSM_READ_ONLY) == 0) {
      DdrdN0TxCtl1.Bits.txdq_dly0 = tmp[0];
      DdrdN0TxCtl1.Bits.txdq_dly1 = tmp[1];
      DdrdN0TxCtl2.Bits.txdq_dly2 = tmp[2];
      DdrdN0TxCtl2.Bits.txdq_dly3 = tmp[3];

      //
      // Write it back if the current data does not equal the cache value or if in force write mode
      //
      if ((DdrdN0TxCtl1.Data != (*DdrioChannelNvList)[ch].txGroup1[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
        MemWritePciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_TX_CTL1_RANK_0_MCIO_DDRIO_REG), DdrdN0TxCtl1.Data);
      }
      if ((DdrdN0TxCtl2.Data != (*DdrioChannelNvList)[ch].txGroup2[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
        MemWritePciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_TX_CTL2_RANK_0_MCIO_DDRIO_REG), DdrdN0TxCtl2.Data);
      }
      //
      // Wait for the new value to settle
      //
      FixedDelayMicroSecond (usDelay);
    } // if not read only

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
      (*DdrioChannelNvList)[ch].txGroup1[logRank][strobe] = DdrdN0TxCtl1.Data;
      (*DdrioChannelNvList)[ch].txGroup2[logRank][strobe] = DdrdN0TxCtl2.Data;
    }
  } // strobe loop
  return MRC_STATUS_SUCCESS;
} // GetSetTxDelayBit

// Strobe to byte index map
// The index of the array represents the strobe and the value represents the controlling byte
UINT8 DdrioStrobeToByteIndex[MAX_STROBE_DDR5] =
{
  0, 1, 2, 3, 4, 0, 1, 2, 3, 4,
  8, 7, 6, 5, 9, 8, 7, 6, 5, 9
};

/**
  Convert linear strobe number to a byte index
  This is a helper function to return a mapping for strobes [0 - 19]
  into a mapping value [0 - 9] based on the following:
  Input strobes are linear strobe numbers where strobes [0 - 9] are subchannel A
  and [10 - 19] are subchannel B.

  Strobes 0 - 4 map to byte index 0 - 4 respectively
  Strobes 5 - 9 map to byte index 0 - 4 respectively

  Strobe 10, 15 map to byte 8
  Strobe 11, 16 map to byte 7
  Strobe 12, 17 map to byte 6
  Strobe 13, 18 map to byte 5
  Strobe 14, 19 map to byte 9

  @param[in]  Strobe   - Strobe value to map

  @retval ByteIndex - Value that maps to input strobe
**/
UINT8
GetByteIndexFromStrobe (
  IN UINT8  Strobe
  )
{
  return DdrioStrobeToByteIndex[Strobe];
}

// Byte index to Strobe map
// The index of the array represents the byte index and the value represents the controlling strobe
UINT8 DdrioByteIndexToStrobe[MAX_STROBE_DDR5 / 2] =
{
  0, 1, 2, 3, 4, 18, 17, 16, 15, 14
};

/**
  Convert a byte index to a controlling strobe
  This is a helper function to map a controlling byte index [0 - 9]
  and returns a strobe that corresponds to that byte as discussed below:
  Byte index 0 - 4 maps to strobe 0 - 4
  Byte index 5, maps to strobe 18
  Byte index 6, maps to strobe 17
  Byte index 7, maps to strobe 16
  Byte index 8, maps to strobe 15
  Byte index 9, maps to strobe 14

  @param[in]  ByteIndex - linear value that maps to controlling strobes

  @retval Strobe   - Strobe value that controls the byte
**/
UINT8
GetStrobeFromByteIndex (
  IN UINT8  ByteIndex
  )
{
  return DdrioByteIndexToStrobe[ByteIndex];
}

/**

  Get TxTco

  @param[in]     Host    - Pointer to sysHost
  @param[in]     Socket  - Socket number
  @param[in]     Ch      - Channel number (0-based)
  @param[in]     Strobe  - Strobe number (0-based)
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - Bit-field of different modes
  @param[in,out] Value   - Pointer to delay Value or offset based on mode

  @retval IO delay

**/
MRC_STATUS
GetSetTxTco (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Strobe,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  UINT32                                    CurCsrVal = 0;
  UINT32                                    NewCsrVal = 0;
  UINT16                                    PiDelay;
  INT16                                     Tmp;
  UINT16                                    MaxLimit = 0;
  UINT16                                    MinLimit = 0;
  UINT16                                    UsDelay = 0;
  DDRIO_CHANNEL_NVDATA                      (*DdrioChannelNvList)[MAX_CH];
  DDRD_N0_COMP_LA1_MCIO_DDRIO_STRUCT        DdrdN0CompLa1;
  BOOLEAN                                   CompUpdateNeeded;
  UINT8                                     MSVx4 = GetMaxStrobeValid(Host);
  UINT8                                     ByteRegisterIndex;
  UINT8                                     MaxByteRegisterIndex;
  UINT8                                     ByteControlStrobe;

  if (Level != DdrLevel) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                    "GetSetTxTcoDq - Level not supported: %d\n", Level);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_159);
    return MRC_STATUS_LEVEL_NOT_SUPPORTED;
  }

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  CompUpdateNeeded = FALSE;

  DdrdN0CompLa1.Data    = 0;

  //
  // Get minimum and maximum Value
  //
  GetDataGroupLimits (Host, Level, Group, &MinLimit, &MaxLimit, &UsDelay);

  if ((MSVx4 > (MAX_STROBE)) || ((Strobe >= MSVx4) && (Strobe != ALL_STROBES))) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, Strobe, NO_BIT,
                                    "The strobe Value is out of range!\n");
    RC_FATAL_ERROR (FALSE, ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (Strobe == ALL_STROBES) {
    Strobe = 0;
    ByteRegisterIndex = 0;
    MaxByteRegisterIndex = MSVx4 / 2;
  } else {
    ByteRegisterIndex = GetByteIndexFromStrobe (Strobe);
    MaxByteRegisterIndex = ByteRegisterIndex + 1;
  }

  //
  // Array bounds check
  //
  MemCheckIndex (Strobe, MSVx4);

  for ( ; ByteRegisterIndex < MaxByteRegisterIndex; ByteRegisterIndex++) {

    // The controlling strobe for a byte is the lower nibble for subch A
    // and the upper nibble for subch B, so set the ByteControlStrobe
    // to [0..4][15..19]
    ByteControlStrobe = GetStrobeFromByteIndex (ByteRegisterIndex);

    if (Mode & GSM_READ_CSR) {
      //
      // Read from PCI config space
      //
      DdrdN0CompLa1.Data = ReadDdrioDataRegCh (Host, Socket, Ch, ByteControlStrobe, DDRD_N0_COMP_LA1_MCIO_DDRIO_REG);

    } else {
      //
      // Read from cache
      //
      DdrdN0CompLa1.Data = (*DdrioChannelNvList)[Ch].txTco[ByteRegisterIndex];

    }

    CurCsrVal = (UINT16)DdrdN0CompLa1.Bits.tco_comp;

    //
    // Check if read only
    //
    if (Mode & GSM_READ_ONLY) {
      if (Group == LinearCntlTcoDqDqs) {
        *Value = GetLinearCntlTcoValue (CurCsrVal);
      } else {
        *Value = (INT16) CurCsrVal;
      }
    } else {
      //
      // Write
      //

      // Adjust the current CMD delay Value by offset
      if (Mode & GSM_WRITE_OFFSET) {
        if (Group == LinearCntlTcoDqDqs) {
          Tmp = *Value + GetLinearCntlTcoValue (CurCsrVal);
        } else {
          Tmp = AdjustTcoByOffset (CurCsrVal, *Value);
        }
      } else {
        Tmp = *Value;
      }

      if (Tmp >= MinLimit) {
        PiDelay = Tmp;
      } else {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, ByteControlStrobe, NO_BIT,
                                "%a Out of range!! Tmp = 0x%x, MinLimit = 0x%x\n", GetGroupStr (Group), Tmp, MinLimit);
        PiDelay = MinLimit;
      }

      // Ensure we do not exceed maximums
      if (PiDelay > MaxLimit) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, ByteControlStrobe, NO_BIT,
                                "%a Out of range!! PiDelay = 0x%x, MaxLimit = 0x%x\n", GetGroupStr (Group), PiDelay, MaxLimit);
        PiDelay = MaxLimit;
      }

      if (Group == LinearCntlTcoDqDqs) {
        NewCsrVal = GetIpTcoSetting (PiDelay);
      } else {
        NewCsrVal = (UINT32) PiDelay;
      }
      DdrdN0CompLa1.Bits.tco_comp = NewCsrVal;

      //
      // Write it back if the current data does not equal the cache Value or if in force write mode
      //

      if ((DdrdN0CompLa1.Data != (*DdrioChannelNvList)[Ch].txTco[ByteRegisterIndex]) || (Mode & GSM_FORCE_WRITE)) {
        WriteDdrioDataRegCh (Host, Socket, Ch, ByteControlStrobe, DDRD_N0_COMP_LA1_MCIO_DDRIO_REG, DdrdN0CompLa1.Data);
        CompUpdateNeeded = TRUE;
      }

      //
      // Wait for the new Value to settle
      //
      FixedDelayMicroSecond (UsDelay);
    } // if read only

    //
    // Save to cache
    //
    if (Mode & GSM_UPDATE_CACHE) {
      (*DdrioChannelNvList)[Ch].txTco[ByteRegisterIndex] = DdrdN0CompLa1.Data;
    }
  } // Strobe loop

  if (CompUpdateNeeded) {
    DoCompUpdate (Host, Socket, GetMCID (Host, Socket, Ch));
  }

  return MRC_STATUS_SUCCESS;
} // GetSetTxTco

/**
  Adjust Tco By Offset
    TCO settings to monotonically increase duty cycle adjustments are [95 - 65][0 - 31]
    This function takes the current value and desired offset and returns the adjusted value

  @param[in]  CurrentValue - Starting value
  @param[in]  Offset       - value of desired adjustment

  @retval Modified value

**/
INT16
AdjustTcoByOffset (
  IN  UINT32 CurrentValue,
  IN  INT16  Offset
)
{
  INT16  NewValue = 0;

  // If the current value is less than or equal to 31, we start in range 1
  if (CurrentValue <= MAX_TCO_RANGE1) {
    //Start by doing the normal addition
    NewValue = ((INT16) CurrentValue) + Offset;

    // Now check if this change switched to range2 by going below range1 min value
    if (NewValue < MIN_TCO_RANGE1) {

      //For range 2, we want to reverse direction however far we are into range 2
      NewValue = MIN_TCO_RANGE2 - NewValue;
    }
  }

  // If the current value is greater than or equal to 65, we start in range 2
  if (CurrentValue >= MIN_TCO_RANGE2) {

    //Start by reversing direction (i.e. subtracting the offset)
    // For example, if the current value is 75 and you want to go 3 ticks back, the result is 78
    NewValue = ((INT16) CurrentValue) - Offset;

    // Now check if this change switched to range1 by going below range2 min value
    if (NewValue < MIN_TCO_RANGE2) {
      // If we switch to range1 then we need to reverse direction again to normally increase
      NewValue = (MIN_TCO_RANGE2 - NewValue) + MIN_TCO_RANGE1;
    }
  }

  return NewValue;
}

/**
  GetLinearCntlTcoValue
    TCO settings to monotonically increase duty cycle adjustments are [95 - 65][0 - 31]
    This function takes the current CSR value and returns a value that is normalized
    to 0 and monotonically increasing.

    For example:
    input values  95, 94, 93,..., 65,  0,  1,  2,..., 31
    correspond to
    output values  0,  1,  2,..., 30, 31, 32, 33,..., 63

  @param[in]  CsrSetting - Setting based on the CSR value

  @retval Correponding Linear Control value

**/
INT16
EFIAPI
GetLinearCntlTcoValue (
  IN UINT32  CsrSetting
)
{
  INT16  LinearValue = 0;

  // If the CSR value is greater than or equal to 65, we are in range "2" (the earlier range)
  if (CsrSetting >= MIN_TCO_RANGE2) {

    //Range 2 goes backwards from 95
    LinearValue = (INT16) (MAX_TCO_RANGE2 - CsrSetting);
  } else {

    //Range 1 goes forward from 0...31, so add the size of Range 2
    LinearValue = (INT16) (CsrSetting + (MAX_TCO_RANGE2 - MIN_TCO_RANGE2 + 1));
  }

  return LinearValue;
} //GetLinearCntlTcoValue

/**
  GetIpTcoSetting
    TCO settings to monotonically increase duty cycle adjustments are [95 - 65][0 - 31]
    This function undoes what GetLinearCntlTcoValue does.  It takes a normalized
    monotonically increasing value and encodes it based on the IP specific value.

    For example:
    input values    0,  1,  2,..., 30, 31, 32, 33,..., 63
    correspond to
    output values  95, 94, 93,..., 65,  0,  1,  2,..., 31

  @param[in]  LinearValue - Normalized Linear Control value

  @retval Correponding Setting based on the CSR definition

**/
UINT32
EFIAPI
GetIpTcoSetting (
  IN INT16  LinearValue
)
{
  UINT32  CsrSetting = 0;

  // If the linear value is greater than the size of range 2, we are in range "1"
  if (LinearValue > (MAX_TCO_RANGE2 - MIN_TCO_RANGE2)) {

    // Range 1 settings just correspond directly to the linear setting minus the
    // size of range 2.  The "+ 1" is used because the range is inclusive of the ends
    CsrSetting = (UINT32) (LinearValue - (MAX_TCO_RANGE2 - MIN_TCO_RANGE2 + 1));

  } else {

    //Range 2 goes backwards from 95
    CsrSetting = (UINT32) (MAX_TCO_RANGE2 - LinearValue);
  }

  return CsrSetting;
} //GetIpTcoSetting

/**

  GetSet GetSetDcaTco

  @param[in]     Host    - Pointer to sysHost
  @param[in]     Socket  - Socket number
  @param[in]     Ch      - Channel number (0-based)
  @param[in]     SubCh   - Sub-Channel number (0-based)
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - Bit-field of different modes
  @param[in,out] Value   - Pointer to delay Value or offset based on mode

  @retval MRC_STATUS_SUCCESS if no problem

**/
MRC_STATUS
GetSetDcaTco (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     SubCh,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  UINT32                                    CurCsrVal = 0;
  UINT32                                    NewCsrVal = 0;
  INT16                                     PiDelay;
  INT16                                     Tmp;
  UINT16                                    MaxLimit = 0;
  UINT16                                    MinLimit = 0;
  UINT16                                    UsDelay = 0;
  DDRIO_CHANNEL_NVDATA                      (*DdrioChannelNvList)[MAX_CH];
  DDRCC_COMP_LA1_MCIO_DDRIOEXT_STRUCT       DdrCcCompLa1;
  BOOLEAN                                   CompUpdateNeeded;

  if (SubCh >= SUB_CH) {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                    "GetSetDcaTco - SubCh not supported: %d\n", SubCh);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_243);
    return MRC_STATUS_INVALID_PARAMETER;
  }

  if (Level != DdrLevel) {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
                                    "GetSetDcaTco - Level not supported: %d\n", Level);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_159);
    return MRC_STATUS_LEVEL_NOT_SUPPORTED;
  }




  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  CompUpdateNeeded = FALSE;

  DdrCcCompLa1.Data    = 0;

  //
  // Get minimum and maximum Value
  //
  GetDataGroupLimits (Host, Level, Group, &MinLimit, &MaxLimit, &UsDelay);

  if (Mode & GSM_READ_CSR) {
    //
    // Read from PCI config space
    //
    DdrCcCompLa1.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_COMP_LA1_MCIO_DDRIOEXT_REG);
  } else {
    //
    // Read from cache
    //
    DdrCcCompLa1.Data = (*DdrioChannelNvList)[Ch].DcaTco[SubCh];
  }

  CurCsrVal = (UINT16)DdrCcCompLa1.Bits.tco_comp;

  //
  // Check if read only
  //
  if (Mode & GSM_READ_ONLY) {
    if (Group == LinearCntlDcaTco) {
      *Value = GetLinearCntlTcoValue (CurCsrVal);
    } else {
      *Value = (INT16) CurCsrVal;
    }
  } else {
    //
    // Write
    //

    // Adjust the current CMD delay Value by offset
    if (Mode & GSM_WRITE_OFFSET) {
      if (Group == LinearCntlDcaTco) {
        Tmp = *Value + GetLinearCntlTcoValue (CurCsrVal);
      } else {
        Tmp = AdjustTcoByOffset (CurCsrVal, *Value);
      }
    } else {
      Tmp = *Value;
    }

    if (Tmp >= MinLimit) {
      PiDelay = Tmp;
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
                              "%a Out of range!! Tmp = 0x%x, MinLimit = 0x%x\n", GetGroupStr (Group), Tmp, MinLimit);
      PiDelay = MinLimit;
    }

    // Ensure we do not exceed maximums
    if (PiDelay > MaxLimit) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
                              "%a Out of range!! PiDelay = 0x%x, MaxLimit = 0x%x\n", GetGroupStr (Group), PiDelay, MaxLimit);
      PiDelay = MaxLimit;
    }

    if (Group == LinearCntlDcaTco) {
      NewCsrVal = GetIpTcoSetting (PiDelay);
    } else {
      NewCsrVal = (UINT32) PiDelay;
    }

    DdrCcCompLa1.Bits.tco_comp = NewCsrVal;

    //
    // Write it back if the current data does not equal the cache Value or if in force write mode
    //

    if ((DdrCcCompLa1.Data != (*DdrioChannelNvList)[Ch].DcaTco[SubCh]) || (Mode & GSM_FORCE_WRITE)) {
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_COMP_LA1_MCIO_DDRIOEXT_REG, DdrCcCompLa1.Data);
      CompUpdateNeeded = TRUE;
    }

    //
    // Wait for the new Value to settle
    //
    FixedDelayMicroSecond (UsDelay);
  } // if read only

  //
  // Save to cache
  //
  if (Mode & GSM_UPDATE_CACHE) {
    (*DdrioChannelNvList)[Ch].DcaTco[SubCh]           = DdrCcCompLa1.Data;
  }

  if (CompUpdateNeeded) {
    DoCompUpdate (Host, Socket, GetMCID (Host, Socket, Ch));
  }
  return MRC_STATUS_SUCCESS;
} // GetSetDcaTco

/**

Get Set Rx Sampler Offset Adjust
Wrapper for multicast and unicast versions of GetSetRxSamplerOffsetAdj

@param[in]      Host    - Pointer to sysHost
@param[in]      Socket  - Socket number
@param[in]      Ch      - Channel number (0-based)
@param[in]      Dimm    - DIMM number (0-based)
@param[in]      Rank    - Rank number (0-based)
@param[in]      Strobe  - Strobe number (0-based)
@param[in]      Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
@param[in]      Group   - CSM_GT - Parameter to program
@param[in]      Mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
@param[in,out]  Value   - Data to program

@retval IO delay

**/
MRC_STATUS
GetSetRxSamplerOffsetAdj (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  MRC_STATUS      Status = MRC_STATUS_FAILURE;
#if 0 //HSD 1507681886 SPR_todo.
  //
  // Use multicast version if possible
  // Multicast register value is added, in HW, to all strobe values in all channels on the MC
  //
  if ((*Value > 0) && UseDelayMulticastOffset (Strobe, Bit, Level, Group, Mode)) {
    Status = GetSetRxSamplerOffsetAdjMulticast (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, RxDqsPerChDelayOffset, Mode, Value);
  }
  if (Status != MRC_STATUS_SUCCESS) {
    //
    // Multicast not available
    // Force multicast register to 0 (in case it isn't already 0) then call the unicast routine
    //
    if ((*Value == 0) && UseDelayMulticastOffset (Strobe, Bit, Level, Group, Mode)) {
      //
      // This should not result in a register access if the Multicast register is already 0.
      // GetSetxxx will return without accessing HW if the cached value already matches the value passed in.
      //
      GetSetRxSamplerOffsetAdjMulticast (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, RxDqsPerChDelayOffset, Mode, Value);
    }
    Status = GetSetRxSamplerOffsetAdjUnicast (Host, Socket, Ch, Dimm, Rank, Strobe, Level, Group, Mode, Value);
  }
#endif //if 0
  Status = GetSetRxSamplerOffsetAdjUnicast (Host, Socket, Ch, Dimm, Rank, Strobe, Level, Group, Mode, Value);
  return Status;
}

/**

  Get RxDelay

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval IO delay

**/
MRC_STATUS
GetSetRxDelayUnicast (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  MRC_STATUS                                   Status;
  UINT8                                        logRank;
  UINT8                                        maxStrobe;
  UINT8                                        SubChannel;
  UINT16                                       curVal = 0;
  UINT16                                       curVal2 = 0;
  UINT16                                       piDelay;
  UINT16                                       piDelay2 = 0;
  INT16                                        tmp;
  INT16                                        tmp2 = 0;
  UINT16                                       maxLimit = 0;
  UINT16                                       minLimit = 0;
  UINT16                                       usDelay = 0;
  DDRIO_CHANNEL_NVDATA                         (*DdrioChannelNvList)[MAX_CH];
  struct ddrRank                               (*RankList)[MAX_RANK_DIMM];
  DDRD_N0_RX_CTL0_RANK_0_MCIO_DDRIO_STRUCT     rxGroup0;
#ifdef LRDIMM_SUPPORT
  UINT8                                        tmpStrobe;
  UINT8                                        updateNeeded = 0;
  struct rankDevice                            (*rankStruct)[MAX_RANK_DIMM];
  struct dimmNvram                             (*dimmNvList)[MAX_DIMM];
  BOOLEAN                                      (*NibbleSwap)[MAX_CH][MAX_DIMM][SUB_CH][MAX_STROBE_DDR5 / 2];
  DDR5_LRDIMM_TRAINING_DATA                    *TrainingData;
  UINT8                                        CachedPage;
#endif  //LRDIMM
  UINT8                                        MSVx4 = GetMaxStrobeValid (Host);
  UINT8                                        MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8                                        MaxStrobePairs = GetSubChMaxStrobeValid (Host) / 2;

  DdrioChannelNvList = GetDdrioChannelNvList (Host, socket);
  RankList = GetRankNvList (Host, socket, ch, dimm);
#ifdef LRDIMM_SUPPORT
  rankStruct = GetRankStruct (Host, socket, ch, dimm);
  dimmNvList = GetDimmNvList (Host, socket, ch);

  //
  // Handle Nibble Swapped Nibble, 0/5, 1/2 and so on...
  //
  if (level == LrbufLevel) {
    Status = GetDdr5LrdimmTrainingData (&TrainingData);
    NibbleSwap = &(TrainingData->NibbleSwap);
    updateNeeded = 0;
    if (strobe != ALL_STROBES) {
      SubChannel = (strobe > 9) ? 1 : 0;
      tmpStrobe = strobe % MaxStrobePairs;
      if ((*NibbleSwap) [ch][dimm][SubChannel][tmpStrobe] == TRUE) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
          "Nibble swap is detected\n");
        if (strobe < MaxStrobePairs) {
          strobe += MaxStrobePairs;
        } else if (strobe < MaxStrobePairs * 2) {
          strobe -= MaxStrobePairs;
        } else if (strobe < MaxStrobePairs * 3) {
          strobe += MaxStrobePairs;
        } else if (strobe < MaxStrobePairs * 4) {
          strobe -= MaxStrobePairs;
        }
      }
    }
  }
#endif  //LRDIMM

  rxGroup0.Data = 0;

  //
  // Get the logical rank #
  //
  logRank = GetLogicalRank (Host, socket, ch, dimm, rank);
  ASSERT (logRank < MAX_RANK_CH);

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits (Host, level, group, &minLimit, &maxLimit, &usDelay);

  if (strobe == ALL_STROBES) {
    strobe = 0;
    maxStrobe = MSVx4;
  } else {
    maxStrobe = strobe + 1;

    if (maxStrobe > MAX_STROBE) {
      return MRC_STATUS_STROBE_NOT_PRESENT;
    }
  }

  for (; strobe < maxStrobe; strobe++) {
    if (strobe >= MAX_STROBE) {
      return MRC_STATUS_STROBE_NOT_PRESENT;
    }

    SubChannel = (strobe > 9) ? 1 : 0;
#ifdef LRDIMM_SUPPORT
    if (level == LrbufLevel) {

      if (mode & GSM_READ_CSR) {
        //
        // Read from actual setting cache
        //
        switch (group) {
          case RxDqsDelay:
            if ((*RankList) [rank].LrBuf_PG01RWE4E5[strobe] < 16) {
              curVal = (*RankList) [rank].LrBuf_PG01RWE4E5[strobe] + 15;
            } else {
              curVal = (*RankList) [rank].LrBuf_PG01RWE4E5[strobe] ^ 31;
            }
            break;
          case RecEnDelay:
            curVal = (*RankList) [rank].lrBuf_FxBC2x3x[strobe];
            break;
          case RecEnDelayCycle:
            curVal = (((*RankList) [rank].lrBuf_FxBCCxEx[strobe % MSVx8] >> (4 * (strobe / MSVx8))) & 0xF);
            break;
          default:
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
              "GetSetRxDelay called unsupported group!\n");
            RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_58);
            break;
        } //switch group
      } else {
        //
        // Read from setting cache
        //
        switch (group) {
          case RxDqsDelay:
            if ((*rankStruct) [rank].cachedLrBuf_PG01RWE4E5[strobe] < 16) {
              curVal = (*rankStruct) [rank].cachedLrBuf_PG01RWE4E5[strobe] + 15;
            } else {
              curVal = (*rankStruct) [rank].cachedLrBuf_PG01RWE4E5[strobe] ^ 31;
            }
            break;
          case RecEnDelay:
            curVal = (*rankStruct) [rank].cachedLrBuf_FxBC2x3x[strobe];
            break;
          case RecEnDelayCycle:
            curVal = (((*rankStruct) [rank].cachedLrBuf_FxBCCxEx[strobe % MSVx8]>> (4 * (strobe / MSVx8))) & 0xF);
            break;
          default:
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
              "GetSetRxDelay called unsupported group!\n");
            RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_59);
            break;
        } //switch group
      }
    } else
#endif //LRDIMM
    {
      if (mode & GSM_READ_CSR) {
        //
        // Read from PCI config space
        //
        rxGroup0.Data = MemReadPciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_RX_CTL0_RANK_0_MCIO_DDRIO_REG));

      } else {
        //
        // Read from cache
        //
        rxGroup0.Data = (*DdrioChannelNvList) [ch].rxGroup0[logRank][strobe];

      }

      switch (group) {
        case RecEnDelay:
          curVal = (UINT16) rxGroup0.Bits.rcven_dly;
          break;

        case RxDqsPDelay:
          curVal = (UINT16) rxGroup0.Bits.rxdqsp_dly;
          break;

        case RxDqsNDelay:
          curVal = (UINT16) rxGroup0.Bits.rxdqsn_dly;
          break;

        case RxDqsDelay:
          curVal = (UINT16) rxGroup0.Bits.rxdqsp_dly;
          curVal2 = (UINT16) rxGroup0.Bits.rxdqsn_dly;
          break;

        case RxDqDelay:
          curVal = (UINT16) rxGroup0.Bits.rxdq_dly;
          break;

        default:
          break;
      } // switch
    }

    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *value = curVal;
    } else {
      //
      // Write
      //
      // Adjust the current CMD delay value by offset
      if (mode & GSM_WRITE_OFFSET) {
        //
        // Add the offset to the current value
        //
        tmp = curVal + *value;
        if (group == RxDqsDelay) {
          tmp2 = curVal2 + *value;
        }

      } else {
        //
        // Write absolute value
        //
        tmp = *value;
        if (group == RxDqsDelay) {
          tmp2 = *value;
        }
      }

      //
      // Make sure we do not exeed the limits
      //
      if (tmp >= minLimit) {
        piDelay = tmp;
      } else {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
          "%a Out of range!! tmp = 0x%x, minLimit = 0x%x\n", GetGroupStr (group), tmp, minLimit);
        piDelay = minLimit;
      }
      // Ensure we do not exceed maximums
      if (piDelay > maxLimit) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
          "%a Out of range!! piDelay = 0x%x, maxLimit = 0x%x\n", GetGroupStr (group), piDelay, maxLimit);
        piDelay = maxLimit;
      }

      if ((group == RxDqsDelay) && (level == DdrLevel)) {
        if (tmp2 >= minLimit) {
          piDelay2 = tmp2;
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
            "%a Out of range!! tmp2 = 0x%x, minLimit = 0x%x\n", GetGroupStr (group), tmp2, minLimit);
          piDelay2 = minLimit;
        }
        // Ensure we do not exceed maximums
        if (piDelay2 > maxLimit) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
            "%a Out of range!! piDelay2 = 0x%x, maxLimit = 0x%x\n", GetGroupStr (group), piDelay2, maxLimit);
          piDelay2 = maxLimit;
        }
      }

      if (level == DdrLevel) {
        switch (group) {
          case RecEnDelay:
            rxGroup0.Bits.rcven_dly = piDelay;
            break;

          case RxDqsPDelay:
            rxGroup0.Bits.rxdqsp_dly = piDelay;
            break;

          case RxDqsNDelay:
            rxGroup0.Bits.rxdqsn_dly = piDelay;
            break;

          case RxDqsDelay:
            rxGroup0.Bits.rxdqsp_dly = piDelay;
            rxGroup0.Bits.rxdqsn_dly = piDelay2;
            break;

          case RxDqDelay:
            rxGroup0.Bits.rxdq_dly = piDelay;
            break;

          default:
            break;
        } // switch
      } //DdrLevel

#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        //
        // first translate encoded value to register value
        //
        //curVal = piDelay >> 1;
        curVal = piDelay;

        switch (group) {
          case RxDqsDelay:
            if (curVal < 16) {
              if ((*RankList) [rank].LrBuf_PG01RWE4E5[strobe] != (UINT8) (curVal ^ 31)) {
                updateNeeded = 1;
                (*RankList) [rank].LrBuf_PG01RWE4E5[strobe] = (UINT8) (curVal ^ 31);
              }
            } else {
              if ((*RankList) [rank].LrBuf_PG01RWE4E5[strobe] != (UINT8) (curVal - 15)) {
                updateNeeded = 1;
                (*RankList) [rank].LrBuf_PG01RWE4E5[strobe] = (UINT8) (curVal - 15);
              }
            }
            break;
          case RecEnDelay:
            if ((*RankList)[rank].lrBuf_FxBC2x3x[strobe] != (UINT8) curVal) {
              updateNeeded = 1;
              (*RankList)[rank].lrBuf_FxBC2x3x[strobe] = (UINT8) curVal;
            }
            break;
          case RecEnDelayCycle:
            if ((((*RankList)[rank].lrBuf_FxBCCxEx[strobe % MSVx8]>>(4 * (strobe / MSVx8))) & 0xF) != (UINT8) curVal) {
              updateNeeded = 1;
              (*RankList)[rank].lrBuf_FxBCCxEx[strobe % MSVx8] &= 0xF0 >> (4 * (strobe / MSVx8));
              (*RankList)[rank].lrBuf_FxBCCxEx[strobe % MSVx8] |= (UINT8) curVal<<(4 * (strobe / MSVx8));
            }
            break;
          default:
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                           "GetSetRxDelay called unsupported group!\n");
            RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_60);
            break;
        } //switch group
      } else
#endif  //LRDIMM
      {
        //
        // Write it back if the current data does not equal the cache value or if in force write mode
        //
        if ((rxGroup0.Data != (*DdrioChannelNvList) [ch].rxGroup0[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
          MemWritePciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_RX_CTL0_RANK_0_MCIO_DDRIO_REG), rxGroup0.Data);
          //
          // Wait for the new value to settle
          //
          FixedDelayMicroSecond (usDelay);
        }
      }
    } // if read only

#ifdef LRDIMM_SUPPORT
    if ((level == LrbufLevel) && !(mode & GSM_READ_ONLY) && ((updateNeeded) || (mode & GSM_FORCE_WRITE))) {
      switch (group) {
        case RxDqsDelay:
          //
          // Select appropriate buffer device, PBA ID: 0~4 for both subchannel
          //
          DbControlWordWrite (Host, socket, ch, (1 << SubChannel), dimm, rank, DDR5_DATA_BUFFER_RW93_REG, strobe % MaxStrobePairs);

          //
          // Select corresponding page for DB rank setting
          //
          ReadDbCwCache (socket, ch, SubChannel, dimm, strobe % MaxStrobePairs, LRDIMM_DB_PAGE0, RDIMM_RWDF, &CachedPage);
          if (CachedPage != rank) {
            DbControlWordWrite (Host, socket, ch, (1 << SubChannel), dimm, rank, RDIMM_RWDF, rank);
          }

          if ((strobe % 10) < MaxStrobePairs) {
            DbControlWordWrite (Host, socket, ch, (1 << SubChannel), dimm, rank, DDR5_DATA_BUFFER_PG0_RWE4_REG, (*RankList) [rank].LrBuf_PG01RWE4E5[strobe]);
          } else {
            DbControlWordWrite (Host, socket, ch, (1 << SubChannel), dimm, rank, DDR5_DATA_BUFFER_PG0_RWE5_REG, (*RankList) [rank].LrBuf_PG01RWE4E5[strobe]);
          }
          DbControlWordWrite (Host, socket, ch, (1 << SubChannel), dimm, rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);
          break;
        default:
          break;
      } // switch
    }
#endif  //LRDIMM

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        switch (group) {
          case RxDqsDelay:
              (*rankStruct)[rank].cachedLrBuf_PG01RWE4E5[strobe] =  (*RankList) [rank].LrBuf_PG01RWE4E5[strobe];
            break;
          default:
            break;
        } // switch
      } else
#endif  //LRDIMM
      {
        (*DdrioChannelNvList)[ch].rxGroup0[logRank][strobe] = rxGroup0.Data;
      }
    }

  } // strobe loop

  return MRC_STATUS_SUCCESS;
} // GetSetRxDelayUnicast

/**

  Get Set Rx Strobe Invert value Unicast function.

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval MRC_STATUS

**/
MRC_STATUS
GetSetRxStrobeInvertUnicast (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT8                               logRank;
  UINT8                               maxStrobe;
  UINT16                              curVal = 0;
  UINT16                              StrobeInvertValue;
  INT16                               tmp;
  UINT16                              maxLimit = 0;
  UINT16                              minLimit = 0;
  UINT16                              usDelay = 0;
  DDRIO_CHANNEL_NVDATA                (*DdrioChannelNvList) [MAX_CH];
  struct ddrRank                      (*RankList) [MAX_RANK_DIMM];
  DDRD_N0_RX_CTL0_RANK_0_MCIO_DDRIO_STRUCT rxGroup0;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  DdrioChannelNvList = GetDdrioChannelNvList (Host, socket);
  RankList = GetRankNvList (Host, socket, ch, dimm);

  rxGroup0.Data = 0;

  //
  // Get the logical rank #
  //
  logRank = GetLogicalRank (Host, socket, ch, dimm, rank);
  ASSERT (logRank < MAX_RANK_CH);

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits (Host, level, group, &minLimit, &maxLimit, &usDelay);

  if (strobe == ALL_STROBES) {
    strobe = 0;
    maxStrobe = MSVx4;
  } else {
    maxStrobe = strobe + 1;

    if (maxStrobe > MAX_STROBE) {
      return MRC_STATUS_STROBE_NOT_PRESENT;
    }
  }

  for (; strobe < maxStrobe; strobe++) {
    {
      if (mode & GSM_READ_CSR) {
        //
        // Read from PCI config space
        //
        rxGroup0.Data = MemReadPciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_RX_CTL0_RANK_0_MCIO_DDRIO_REG));

      } else {
        //
        // Read from cache
        //
        rxGroup0.Data = (*DdrioChannelNvList) [ch].rxGroup0[logRank][strobe];

      }

      switch (group) {
        case RxStrobeInvert:
          curVal = (UINT16) rxGroup0.Bits.rx_strobe_inv;
          break;
        default:
          break;
      } // switch
    }

    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *value = curVal;
    } else {
      //
      // Write
      //
      // Adjust the current CMD delay value by offset
      if (mode & GSM_WRITE_OFFSET) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
          "GetSetRxStrobeInvert called unsupported Mode!\n");
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_181);
        return MRC_STATUS_FAILURE;
      } else {
        //
        // Write absolute value
        //
        tmp = *value;
      }

      //
      // Make sure we do not exeed the limits
      //
      if (tmp >= minLimit) {
        StrobeInvertValue = tmp;
      } else {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
          "%a Out of range!! tmp = 0x%x, minLimit = 0x%x\n", GetGroupStr (group), tmp, minLimit);
        StrobeInvertValue = minLimit;
      }
      // Ensure we do not exceed maximums
      if (StrobeInvertValue > maxLimit) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
          "%a Out of range!! StrobeInvertValue = 0x%x, maxLimit = 0x%x\n", GetGroupStr (group), StrobeInvertValue, maxLimit);
        StrobeInvertValue = maxLimit;
      }


      if (level == DdrLevel) {
        switch (group) {
          case RxStrobeInvert:
            rxGroup0.Bits.rx_strobe_inv = StrobeInvertValue;
            break;
          default:
            break;
        } // switch
      } //DdrLevel

      {
        //
        // Write it back if the current data does not equal the cache value or if in force write mode
        //
        if ((rxGroup0.Data != (*DdrioChannelNvList) [ch].rxGroup0[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
          MemWritePciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_RX_CTL0_RANK_0_MCIO_DDRIO_REG), rxGroup0.Data);
          //
          // Wait for the new value to settle
          //
          FixedDelayMicroSecond (usDelay);
          (*DdrioChannelNvList) [ch].rxGroup0[logRank][strobe] = rxGroup0.Data;
        }
      }
    } // if read only

  } // strobe loop

  return MRC_STATUS_SUCCESS;
} // GetSetRxStrobeInvertUnicast

/**

  Get RxDelay
  Wrapper for multicast and unicast versions of GetSetRxDelay

  @param[in]      Host    - Pointer to sysHost
  @param[in]      Socket  - Socket number
  @param[in]      Ch      - Channel number (0-based)
  @param[in]      Dimm    - DIMM number (0-based)
  @param[in]      Rank    - Rank number (0-based)
  @param[in]      Strobe  - Strobe number (0-based)
  @param[in]      Bit     - Bit number
  @param[in]      Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]      Group   - CSM_GT - Parameter to program
  @param[in]      Mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param[in,out]  Value   - Data to program

  @retval IO delay

**/
MRC_STATUS
GetSetRxDelay (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      UINT8     Bit,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  MRC_STATUS      Status;
  INT16           OriginalOffset;

  if ((Group == RxDqsPDelay || Group == RxDqsNDelay) && Level == DdrLevel) {

    if (FeaturePcdGet (PcdUseRxTxMultiCastRegisters)) {
      //
      // Before any operation happen for Even or Odd knobs, it is required
      // to make sure the multicast register is flushed into the unicast registers, so
      // any operation (READ/WRITE_OFFSET) is accurate
      //
      Status  = GetSetDelayMulticastOffset (Host, Socket, Ch, Dimm, Rank, ALL_STROBES, ALL_BITS, Level, RxDqsPerChDelayOffset, GSM_READ_ONLY, &OriginalOffset);

      if (Status != MRC_STATUS_SUCCESS) {

        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_248);

      }
      if (OriginalOffset != 0) {

        FlushMulticastIntoUnicast (Host, Socket, Ch, Dimm, Rank, Level, RxDqsDelay, RxDqsPerChDelayOffset, &OriginalOffset);

      }

    }

    Status = GetSetRxDelayUnicast (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, Group, Mode, Value);

  } else if (Group == RxDqsDelay && Level == DdrLevel) {

    Status = GetSetMulticastUnicast (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, Group, RxDqsPerChDelayOffset, Mode, Value);

  } else {

    Status = GetSetRxDelayUnicast (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, Group, Mode, Value);

  }

  return Status;
} // GetSetRxDelay


/**

  Get Set Rx Strobe Invert Vallue

  @param[in]      Host    - Pointer to sysHost
  @param[in]      Socket  - Socket number
  @param[in]      Ch      - Channel number (0-based)
  @param[in]      Dimm    - DIMM number (0-based)
  @param[in]      Rank    - Rank number (0-based)
  @param[in]      Strobe  - Strobe number (0-based)
  @param[in]      Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]      Group   - CSM_GT - Parameter to program
  @param[in]      Mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param[in,out]  Value   - Data to program

  @retval IO delay

**/
MRC_STATUS
GetSetRxStrobeInvert (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  MRC_STATUS      Status;
  Status = MRC_STATUS_SUCCESS;
  Status = GetSetRxStrobeInvertUnicast (Host, Socket, Ch, Dimm, Rank, Strobe, Level, Group, Mode, Value);
  return Status;
} // GetSetRxDelay

/**

  Get RxDelayBit

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number. When mode is GSM_FORCE_WRITE, only supports ALL_BITS.
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program. When mode is GSM_FORCE_WRITE, value must be an array of size BITS_PER_NIBBLE, which contains
                   different values for different bits in the nibble.

  @retval IO delay

**/
MRC_STATUS
GetSetRxDelayBit (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT8                               logRank;
  UINT8                               maxStrobe;
  UINT8                               i;
  UINT8                               clkSel;
  INT16                               tmp;
  UINT16                              curVal[BITS_PER_NIBBLE];
  UINT16                              piDelayNibble = 0;
  UINT16                              piDelay;
  UINT16                              piDelayMin;
  UINT16                              piDelayMax;
  UINT16                              delta;
  UINT16                              piDelayBit[BITS_PER_NIBBLE];
  UINT16                              maxLimit = 0;
  UINT16                              minLimit = 0;
  UINT16                              maxPerBitLimit = 0;
  UINT16                              minPerBitLimit = 0;
  UINT16                              usDelay = 0;
  DDRIO_CHANNEL_NVDATA                (*DdrioChannelNvList)[MAX_CH];
  struct ddrRank                      (*RankList)[MAX_RANK_DIMM];
  DDRD_N0_RX_CTL0_RANK_0_MCIO_DDRIO_STRUCT rxGroup0;
  DDRD_N0_RX_CTL1_RANK_0_MCIO_DDRIO_STRUCT rxGroup1n;
  DDRD_N0_RX_CTL2_RANK_0_MCIO_DDRIO_STRUCT rxGroup1p;
  DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_STRUCT rxOffset;
  UINT8                               MSVx4 = GetMaxStrobeValid (Host);
  UINT8                               SwizzleBit;

  DdrioChannelNvList = GetDdrioChannelNvList (Host, socket);
  RankList = GetRankNvList (Host, socket, ch, dimm);
  rxGroup1n.Data = 0;
  rxGroup1p.Data = 0;
  //
  // Get the logical rank #
  //
  logRank = GetLogicalRank (Host, socket, ch, dimm, rank);

  //
  // Get minimum and maximum value for the overall RxDqDqsDelay, and the per bit Delays
  //
  GetDataGroupLimits (Host, level, RxDqsDelay, &minLimit, &maxLimit, &usDelay);
  GetDataGroupLimits (Host, level, group, &minPerBitLimit, &maxPerBitLimit, &usDelay);

  if (strobe >= MAX_STROBE && strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
      "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((strobe < MAX_STROBE || strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (strobe == ALL_STROBES) {
    strobe = 0;
    maxStrobe = MSVx4;
  } else {
    maxStrobe = strobe + 1;
  }

  if ((mode & GSM_READ_ONLY) == GSM_READ_ONLY) {
    //
    // For reads, specific bit must be specified
    //
    if (bit >= BITS_PER_NIBBLE) {
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_61);
      return MRC_STATUS_FAILURE;
    }
  } else {
    //
    // For writes, must specify the entire nibble (ALL_BITS)
    //
    RC_FATAL_ERROR (bit == ALL_BITS, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_163);
  }

  for (; strobe < maxStrobe; strobe++) {

    if (mode & GSM_READ_CSR) {
      //
      // Read from PCI config space
      //
      rxGroup0.Data  = MemReadPciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_RX_CTL0_RANK_0_MCIO_DDRIO_REG));
      rxGroup1n.Data = MemReadPciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_RX_CTL1_RANK_0_MCIO_DDRIO_REG));
      rxGroup1p.Data = MemReadPciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_RX_CTL2_RANK_0_MCIO_DDRIO_REG));
      rxOffset.Data  = MemReadPciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_REG));
    } else {
      //
      // Read from cache
      //
      rxGroup0.Data = (*DdrioChannelNvList) [ch].rxGroup0[logRank][strobe];
      rxGroup1n.Data = (*DdrioChannelNvList) [ch].rxGroup1n[logRank][strobe];
      rxGroup1p.Data = (*DdrioChannelNvList) [ch].rxGroup1p[logRank][strobe];
      rxOffset.Data = (*DdrioChannelNvList) [ch].rxOffset[logRank][strobe];
    }

    switch (group) {
      case RxDqsBitDelay:
      case RxDqsPBitDelay:
        piDelayNibble = (UINT16) rxGroup0.Bits.rxdqsp_dly;
        piDelayBit[0] = (UINT16) rxGroup1p.Bits.rxdqsp_dly0;
        piDelayBit[1] = (UINT16) rxGroup1p.Bits.rxdqsp_dly1;
        piDelayBit[2] = (UINT16) rxGroup1p.Bits.rxdqsp_dly2;
        piDelayBit[3] = (UINT16) rxGroup1p.Bits.rxdqsp_dly3;
        break;
      case RxDqsNBitDelay:
        piDelayNibble = (UINT16) rxGroup0.Bits.rxdqsn_dly;
        piDelayBit[0] = (UINT16) rxGroup1n.Bits.rxdqsn_dly0;
        piDelayBit[1] = (UINT16) rxGroup1n.Bits.rxdqsn_dly1;
        piDelayBit[2] = (UINT16) rxGroup1n.Bits.rxdqsn_dly2;
        piDelayBit[3] = (UINT16) rxGroup1n.Bits.rxdqsn_dly3;
        break;
      default:
        piDelayNibble = 0;
        piDelayBit[0] = 0;
        piDelayBit[1] = 0;
        piDelayBit[2] = 0;
        piDelayBit[3] = 0;
        break;
    }
    for (i = 0; i < BITS_PER_NIBBLE; i++) {
      piDelayBit[i] += piDelayNibble;
    }
    for (i = 0; i < BITS_PER_NIBBLE; i++) {
      curVal[i] = piDelayBit[i];
    }

    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      //
      // Convert logical bit to DDRIO physical register bit.
      //
      SwizzleBit = GetDqSwizzle (ch, strobe, bit, level, group);
      //
      // Save the value read into the output parameter
      //
      *value = curVal[SwizzleBit];
    } else {
      //
      // Write.
      // When mode is GSM_FORCE_WRITE, only supports ALL_BITS. Input parameter "value" must
      // be an array of size BITS_PER_NIBBLE, which contains different values for different
      // bits in the nibble.
      //
      for (i = 0; i < BITS_PER_NIBBLE; i++) {

        //
        // "SwizzleBit" is the physical DDRIO register bit. "i" is logical bit.
        //
        SwizzleBit = GetDqSwizzle (ch, strobe, i, level, group);

        // Adjust the current CMD delay value by offset
        if (mode & GSM_WRITE_OFFSET) {
          //
          // Add the offset to the current value.
          //
          tmp = curVal[SwizzleBit] + value[i];

        } else {
          //
          // Write absolute value
          //
          tmp = value[i];
        }
        //
        // Make sure we do not exeed the limits
        //
        if (tmp >= minLimit) {
          piDelay = tmp;
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, i,
            "%a Out of range!! tmp = 0x%x, minLimit = 0x%x\n", GetGroupStr (group), tmp, minLimit);
          piDelay = minLimit;
        }
        // Ensure we do not exceed maximums
        if (piDelay > maxLimit) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, i,
            "%a Out of range!! piDelay = 0x%x, maxLimit = 0x%x\n", GetGroupStr (group), piDelay, maxLimit);
          piDelay = maxLimit;
        }
        //
        // Adjust target bit delay
        //
        piDelayBit[SwizzleBit] = piDelay;

      }//i loop

      //
      // Normalize smallest per bit delay to 0.
      // Below code adjusts per-nibble and per-bit register value within a nibble.
      // Do not need to consider DQ swizzle.
      //

      piDelayMax = 0;
      piDelayMin = 0xFF;
      clkSel = 0;

      for (i = 0; i < BITS_PER_NIBBLE; i++) {
        if (piDelayBit[i] > piDelayMax) {
          piDelayMax = piDelayBit[i];
          clkSel = i;
        }
        if (piDelayBit[i] < piDelayMin) {
          piDelayMin = piDelayBit[i];
        }
      }

      delta = piDelayMin - piDelayNibble;
      piDelayNibble += delta;

      //
      // Make sure we do not exeed the limits
      //
      if (piDelayNibble < minLimit) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, bit,
          "%a Out of range!! piDelayNibble = 0x%x, minLimit = 0x%x\n", GetGroupStr (RxDqsDelay), piDelayNibble, minLimit);
        piDelayNibble = minLimit;
      }

      if (piDelayNibble > maxLimit) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, bit,
          "%a Out of range!! piDelayNibble = 0x%x, maxLimit = 0x%x\n", GetGroupStr (RxDqsDelay), piDelayNibble, maxLimit);
        piDelayNibble = maxLimit;
      }

      for (i = 0; i < BITS_PER_NIBBLE; i++) {
        piDelayBit[i] = piDelayBit[i] - piDelayNibble;
        //Check per bit limits
        if (piDelayBit[i] > maxPerBitLimit) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, bit,
            "%a Out of range!! piDelayBit[%d] = 0x%x, maxPerBitLimit = 0x%x\n", GetGroupStr (group), i, piDelayBit[i], maxPerBitLimit);
          piDelayBit[i] = maxPerBitLimit;
        }
        if (piDelayBit[i] < minPerBitLimit) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, bit,
            "%a Out of range!! piDelayBit[%d] = 0x%x, minPerBitLimit = 0x%x\n", GetGroupStr (group), i, piDelayBit[i], minPerBitLimit);
          piDelayBit[i] = minPerBitLimit;
        }
      }

      //
      // Program per-bit register value with the adjusted delay value.
      //
      for (i = 0; i < BITS_PER_NIBBLE; i++) {
        piDelay = piDelayBit[i];
        switch (group) {
          case RxDqsBitDelay:
            rxGroup0.Bits.rxdqsp_dly = piDelayNibble;
            rxGroup0.Bits.rxdqsn_dly = piDelayNibble;
            switch (i) {
              case 0:
                rxGroup1p.Bits.rxdqsp_dly0 = piDelay;
                rxGroup1n.Bits.rxdqsn_dly0 = piDelay;
                break;
              case 1:
                rxGroup1p.Bits.rxdqsp_dly1 = piDelay;
                rxGroup1n.Bits.rxdqsn_dly1 = piDelay;
                break;
              case 2:
                rxGroup1p.Bits.rxdqsp_dly2 = piDelay;
                rxGroup1n.Bits.rxdqsn_dly2 = piDelay;
                break;
              case 3:
                rxGroup1p.Bits.rxdqsp_dly3 = piDelay;
                rxGroup1n.Bits.rxdqsn_dly3 = piDelay;
                break;
              default:
                //Automated add of default case. Please review.
                break;
            } // switch i
            break;
          case RxDqsPBitDelay:
            rxGroup0.Bits.rxdqsp_dly = piDelayNibble;
            switch (i) {
              case 0:
                rxGroup1p.Bits.rxdqsp_dly0 = piDelay;
                break;
              case 1:
                rxGroup1p.Bits.rxdqsp_dly1 = piDelay;
                break;
              case 2:
                rxGroup1p.Bits.rxdqsp_dly2 = piDelay;
                break;
              case 3:
                rxGroup1p.Bits.rxdqsp_dly3 = piDelay;
                break;
              default:
                //Automated add of default case. Please review.
                break;
            } // switch i
            break;
          case RxDqsNBitDelay:
            rxGroup0.Bits.rxdqsn_dly = piDelayNibble;
            switch (i) {
              case 0:
                rxGroup1n.Bits.rxdqsn_dly0 = piDelay;
                break;
              case 1:
                rxGroup1n.Bits.rxdqsn_dly1 = piDelay;
                break;
              case 2:
                rxGroup1n.Bits.rxdqsn_dly2 = piDelay;
                break;
              case 3:
                rxGroup1n.Bits.rxdqsn_dly3 = piDelay;
                break;
              default:
                //Automated add of default case. Please review.
                break;
            } // switch i
            break;
          default:
            break;
        } //switch group
      } //for i

      //
      // Update mux select to reflect largest per bit offset
      //
      switch (group) {
        case RxDqsBitDelay:
          rxOffset.Bits.rxdqsp_sel = clkSel;
          rxOffset.Bits.rxdqsn_sel = clkSel;
          break;
        case RxDqsPBitDelay:
          rxOffset.Bits.rxdqsp_sel = clkSel;
          break;
        case RxDqsNBitDelay:
          rxOffset.Bits.rxdqsn_sel = clkSel;
          break;
        default:
          break;
      }

      //
      // Write it back if the current data does not equal the cache value or if in force write mode
      //
      if ((rxGroup0.Data != (*DdrioChannelNvList) [ch].rxGroup0[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
        MemWritePciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_RX_CTL0_RANK_0_MCIO_DDRIO_REG), rxGroup0.Data);
      }
      if ((rxGroup1n.Data != (*DdrioChannelNvList) [ch].rxGroup1n[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
        MemWritePciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_RX_CTL1_RANK_0_MCIO_DDRIO_REG), rxGroup1n.Data);
      }
      if ((rxGroup1p.Data != (*DdrioChannelNvList) [ch].rxGroup1p[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
        MemWritePciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_RX_CTL2_RANK_0_MCIO_DDRIO_REG), rxGroup1p.Data);
      }
      if ((rxOffset.Data != (*DdrioChannelNvList) [ch].rxOffset[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
        MemWritePciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_REG), rxOffset.Data);
      }

      //
      // Wait for the new value to settle
      //
      FixedDelayMicroSecond (usDelay);

    } // if read only

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
      (*DdrioChannelNvList)[ch].rxGroup0[logRank][strobe] = rxGroup0.Data;
      (*DdrioChannelNvList)[ch].rxGroup1n[logRank][strobe] = rxGroup1n.Data;
      (*DdrioChannelNvList)[ch].rxGroup1p[logRank][strobe] = rxGroup1p.Data;
      (*DdrioChannelNvList)[ch].rxOffset[logRank][strobe] = rxOffset.Data;
    }
  } // strobe loop
  return MRC_STATUS_SUCCESS;
} // GetSetRxDelayBit

/**

  Get/Set Rx Vref thru unicast registers

  @param[in]      Host        Pointer to SysHost structure
  @param[in]      Socket      Current socket under test (0-based)
  @param[in]      Ch          Current channel under test (0-based)
  @param[in]      Dimm        Current dimm under test (0-based)
  @param[in]      Rank        Current rank under test (0-based)
  @param[in]      Strobe      Current strobe under test (0-based || ALL_STROBES)
  @param[in]      Bit         Current bit under test (0-based || ALL_BITS)
  @param[in]      Level       Current level (not applicable)
  @param[in]      Group       Current group under test (RxVref)
  @param[in]      Mode        Register/Cache access mode (GSM_READ_CSR, GSM_READ_ONLY, GSM_UPDATE_CACHE, GSM_FORCE_WRITE)
  @param[in,out]  Value       Pointer to data that will be used to retrieve or store

  @retval         MRC_STATUS_STROBE_NOT_PRESENT     Invalid strobe parameter
  @retval         MRC_STATUS_FAILURE                Invalid bit parameter
  @retval         MRC_STATUS_SUCCESS                Otherwise

**/
MRC_STATUS
GetSetRxVrefUnicast (
  IN        PSYSHOST  Host,
  IN        UINT8     Socket,
  IN        UINT8     Ch,
  IN        UINT8     Dimm,
  IN        UINT8     Rank,
  IN        UINT8     Strobe,
  IN        UINT8     Bit,
  IN        MRC_LT    Level,
  IN        MRC_GT    Group,
  IN        UINT8     Mode,
  IN        INT16     *Value
  )
{
  UINT8                             MaxStrobe;
  UINT8                             BitIndex;
  UINT8                             BitStart;
  UINT8                             BitStop;
  UINT8                             SwizzleBit;
  UINT16                            BitValue[BITS_PER_NIBBLE];
  UINT16                            MinBitValue;
  UINT16                            MaxLimit = 127;
  UINT16                            MinLimit = 0;
  UINT16                            UsDelay = 1;
  DDRIO_CHANNEL_NVDATA              (*DdrioChannelNvList) [MAX_CH];
  DDRD_N0_RXVREF_CTL_MCIO_DDRIO_STRUCT RxVrefCtrl;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  CountTrackingData (PTC_VREF_MOVE, 1);

  //
  // LRDIMM buffer backside RxVref
  //
  if (IsLrdimmPresent (Socket, Ch, Dimm) && (Level == LrbufLevel)) {
    return GetSetLrdimmBufferRxVref (Host, Socket, Ch, Dimm, Rank, Strobe, Level, Group, Mode, Value);
  }

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  RxVrefCtrl.Data = 0;

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits (Host, Level, Group, &MinLimit, &MaxLimit, &UsDelay);

  if (Bit == ALL_BITS) {
    BitStart    = 0;
    BitStop     = BITS_PER_NIBBLE;
  } else {
    BitStart    = Bit;
    BitStop     = Bit + 1;
  }

  if (Strobe >= MAX_STROBE && Strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (
      SDBG_MAX,
      Socket,
      Ch,
      Dimm,
      NO_SUBCH,
      Rank,
      Strobe,
      NO_BIT,
      "The strobe value is out of range!\n"
      );
    RC_FATAL_ERROR ((Strobe < MAX_STROBE || Strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_227);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (Strobe == ALL_STROBES) {
    Strobe = 0;
    MaxStrobe = MSVx4;
  } else {
    MaxStrobe = Strobe + 1;
  }

  for (; Strobe < MaxStrobe; Strobe++) {

    Strobe = Strobe % MSVx4;

    if (Mode & GSM_READ_CSR) {

      //
      // Read from PCI config space
      //
      RxVrefCtrl.Data = ReadDdrioDataRegCh (Host, Socket, Ch, Strobe, DDRD_N0_RXVREF_CTL_MCIO_DDRIO_REG);
    } else {
      //
      // Read from cache
      //
      RxVrefCtrl.Data = (*DdrioChannelNvList) [Ch].rxVrefCtrl[Strobe];
    }

    BitValue[0] = (UINT16) RxVrefCtrl.Bits.vref_ctl0;
    BitValue[1] = (UINT16) RxVrefCtrl.Bits.vref_ctl1;
    BitValue[2] = (UINT16) RxVrefCtrl.Bits.vref_ctl2;
    BitValue[3] = (UINT16) RxVrefCtrl.Bits.vref_ctl3;
    //
    // MinBitValue is only used for GSM_READ_ONLY, so we can return the minimum value across the nibble
    //
    MinBitValue = MaxLimit;

    for (BitIndex = BitStart; BitIndex < BitStop; BitIndex++) {

      if (BitIndex >= BITS_PER_NIBBLE) {
        RC_FATAL_ERROR (FALSE, ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_228);
        return MRC_STATUS_FAILURE;
      }

      SwizzleBit = GetDqSwizzle (Ch, Strobe, BitIndex, Level, Group);

      //
      // Check if read only
      //
      if (Mode & GSM_READ_ONLY) {

        if (BitValue[SwizzleBit] < MinBitValue) {
          MinBitValue = BitValue[SwizzleBit];
        }

        *Value = MinBitValue;

      } else {
        //
        // Write
        //
        if (Mode & GSM_WRITE_OFFSET) {
          //
          // Add the offset to the current value
          //
          BitValue[SwizzleBit] = BitValue[SwizzleBit] + *Value;
          //
          // Make sure we do not exceed the limits
          //
          if (BitValue[SwizzleBit] > MaxLimit) {
            RcDebugPrintWithDevice (
              SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, BitIndex,
              "RxVref is Out of Range!! BitValue[%d] = 0x%x, MaxLimit = 0x%x\n",
              SwizzleBit,
              BitValue[SwizzleBit],
              MaxLimit
              );
            BitValue[SwizzleBit] = MaxLimit;
          }
          if (BitValue[SwizzleBit] < MinLimit) {
            RcDebugPrintWithDevice (
              SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, BitIndex,
              "RxVref is Out of Range!! BitValue[%d] = 0x%x, MinLimit = 0x%x\n",
              SwizzleBit,
              BitValue[SwizzleBit],
              MinLimit
              );
            BitValue[SwizzleBit] = MinLimit;
          }

        } else {
          BitValue[SwizzleBit] = *Value;
        }
      }
    } // BitIndex loop

    RxVrefCtrl.Bits.vref_ctl0 = BitValue[0];
    RxVrefCtrl.Bits.vref_ctl1 = BitValue[1];
    RxVrefCtrl.Bits.vref_ctl2 = BitValue[2];
    RxVrefCtrl.Bits.vref_ctl3 = BitValue[3];
    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if ((RxVrefCtrl.Data != (*DdrioChannelNvList) [Ch].rxVrefCtrl[Strobe]) || (Mode & GSM_FORCE_WRITE)) {
      WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DDRD_N0_RXVREF_CTL_MCIO_DDRIO_REG, RxVrefCtrl.Data);
    }
    //
    // Save to cache
    //
    if (Mode & GSM_UPDATE_CACHE) {
      (*DdrioChannelNvList) [Ch].rxVrefCtrl[Strobe] = RxVrefCtrl.Data;
    }

  } // Strobe loop

  return MRC_STATUS_SUCCESS;
} // GetSetRxVrefUnicast

/**

  Get/Set Rx Vref thru multicast/unicast registers

  @param[in]      Host        Pointer to SysHost structure
  @param[in]      Socket      Current socket under test (0-based)
  @param[in]      Ch          Current channel under test (0-based)
  @param[in]      Dimm        Current dimm under test (0-based)
  @param[in]      Rank        Current rank under test (0-based)
  @param[in]      Strobe      Current strobe under test (0-based || ALL_STROBES)
  @param[in]      Bit         Current bit under test (0-based || ALL_BITS)
  @param[in]      Level       Current level (not applicable)
  @param[in]      Group       Current group under test (RxVref)
  @param[in]      Mode        Register/Cache access mode (GSM_READ_CSR, GSM_READ_ONLY, GSM_UPDATE_CACHE, GSM_FORCE_WRITE)
  @param[in,out]  Value       Pointer to data that will be used to retrieve or store

  @retval         MRC_STATUS_SUCCESS  Executed properly thru multicast or unicast registers
  @retval         Any other           Failure to write/read the register(s)

**/
MRC_STATUS
GetSetRxVref (
  IN        PSYSHOST  Host,
  IN        UINT8     Socket,
  IN        UINT8     Ch,
  IN        UINT8     Dimm,
  IN        UINT8     Rank,
  IN        UINT8     Strobe,
  IN        UINT8     Bit,
  IN        MRC_LT    Level,
  IN        MRC_GT    Group,
  IN        UINT8     Mode,
  IN        INT16     *Value
  )
{
  MRC_STATUS Status;

  if (Group == RxVref && Level == DdrLevel) {

    Status = GetSetMulticastUnicast(Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, Group, RxVrefPerChDelayOffset, Mode, Value);

  } else {

    Status = GetSetRxVrefUnicast (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, Group, Mode, Value);

  }

  return Status;

}
/**

  Get/Set function to modify the RxSampler at a bit or strobe level

  @param[in]      Host        Pointer to SysHost structure
  @param[in]      Socket      Current socket under test (0-based)
  @param[in]      Ch          Current channel under test (0-based)
  @param[in]      Dimm        Current dimm under test (0-based)
  @param[in]      Rank        Current rank under test (0-based)
  @param[in]      Strobe      Current strobe under test (0-based || ALL_STROBES)
  @param[in]      Bit         Current bit under test (0-based || ALL_BITS)
  @param[in]      Level       Current level (not applicable)
  @param[in]      Group       Current group under test (RxSamplerEvenOdd, RxSamplerEven or RxSamplerOdd)
  @param[in]      Mode        Register/Cache access mode (GSM_READ_CSR, GSM_READ_ONLY, GSM_UPDATE_CACHE, GSM_FORCE_WRITE)
  @param[in,out]  Value       Pointer to data that will be used to retrieve or store

  @retval         MRC_STATUS_STROBE_NOT_PRESENT     Invalid strobe parameter
  @retval         MRC_STATUS_INVALID_PARAMETER      Invalid bit parameter
  @retval         MRC_STATUS_INVALID_PARAMETER      Invalid Group parameter
  @retval         MRC_STATUS_SUCCESS                Otherwise

**/
MRC_STATUS
GetSetRxSamplerEvenOddUnicast (
  PSYSHOST    Host,
  UINT8       Socket,
  UINT8       Ch,
  UINT8       Dimm,
  UINT8       Rank,
  UINT8       Strobe,
  UINT8       Bit,
  MRC_LT      Level,
  MRC_GT      Group,
  UINT8       Mode,
  INT16*      Value
  )
{
  UINT8   MaxStrobe;
  UINT8   BitIndex;
  UINT8   BitStart;
  UINT8   BitStop;
  UINT8   SamplingIndex;
  UINT8   SamplingStart;
  UINT8   SamplingEnd;
  UINT8   SwizzleBit;
  UINT16  BitValue[MAX_SAMPLING_LOOP][BITS_PER_NIBBLE];
  UINT16  MinBitValue;
  UINT16  MaxLimit = 0;
  UINT16  MinLimit = 0;
  UINT16  UsDelay = 1;
  DDRIO_CHANNEL_NVDATA (*DdrioChannelNvList)[MAX_CH];
  DDRD_N0_SAMPLER_EVEN_OFFSET_MCIO_DDRIO_STRUCT SamplerEven;
  DDRD_N0_SAMPLER_ODD_OFFSET_MCIO_DDRIO_STRUCT  SamplerOdd;
  UINT8 MSVx4 = GetMaxStrobeValid(Host);
  DdrioChannelNvList = GetDdrioChannelNvList(Host, Socket);
  SamplerOdd.Data = 0;
  SamplerEven.Data = 0;
  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits (Host, Level, Group, &MinLimit, &MaxLimit, &UsDelay);
  if (Bit == ALL_BITS) {
    BitStart = 0;
    BitStop = BITS_PER_NIBBLE;
  } else {
    BitStart = Bit;
    BitStop = Bit + 1;
  }
  if (Strobe >= MAX_STROBE && Strobe != ALL_STROBES) {
    RcDebugPrintWithDevice(
      SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
      "The strobe value is out of range!\n"
      );
    RC_FATAL_ERROR(FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_200);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (Bit >= BITS_PER_NIBBLE && Bit != ALL_BITS) {
    RcDebugPrintWithDevice(
      SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, Bit,
      "The bit value is out of range!\n"
      );
    RC_FATAL_ERROR(FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_220);
    return MRC_STATUS_INVALID_PARAMETER;
  }


  if (Strobe == ALL_STROBES) {
    Strobe = 0;
    MaxStrobe = MSVx4;
  } else {
    MaxStrobe = Strobe + 1;
  }
  if (Group == RxSamplerEvenOdd) {
    SamplingStart = EVEN_SAMPLING;
    SamplingEnd   = MAX_SAMPLING_LOOP;
  } else if (Group == RxSamplerEven) {
    SamplingStart = EVEN_SAMPLING;
    SamplingEnd   = EVEN_SAMPLING + 1;
  } else if (Group == RxSamplerOdd) {
    SamplingStart = ODD_SAMPLING;
    SamplingEnd   = ODD_SAMPLING + 1;
  } else {
    RC_FATAL_ERROR(FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_199);
    return MRC_STATUS_INVALID_PARAMETER;
  }

  for (; Strobe < MaxStrobe; Strobe++) {

    Strobe = Strobe % MSVx4;

    if (Mode & GSM_READ_CSR) {
      //
      // Read from PCI config space
      //
      SamplerEven.Data  = ReadDdrioDataRegCh(Host, Socket, Ch, Strobe, DDRD_N0_SAMPLER_EVEN_OFFSET_MCIO_DDRIO_REG);
      SamplerOdd.Data   = ReadDdrioDataRegCh(Host, Socket, Ch, Strobe, DDRD_N0_SAMPLER_ODD_OFFSET_MCIO_DDRIO_REG);
    } else {
      //
      // Read from cache
      //
      SamplerEven.Data  = (*DdrioChannelNvList)[Ch].RxSamplerEven[Strobe];
      SamplerOdd.Data   = (*DdrioChannelNvList)[Ch].RxSamplerOdd[Strobe];
    }

    BitValue[EVEN_SAMPLING][0] = (UINT16) SamplerEven.Bits.sampler_even_offset0;
    BitValue[EVEN_SAMPLING][1] = (UINT16) SamplerEven.Bits.sampler_even_offset1;
    BitValue[EVEN_SAMPLING][2] = (UINT16) SamplerEven.Bits.sampler_even_offset2;
    BitValue[EVEN_SAMPLING][3] = (UINT16) SamplerEven.Bits.sampler_even_offset3;

    BitValue[ODD_SAMPLING][0] = (UINT16) SamplerOdd.Bits.sampler_odd_offset0;
    BitValue[ODD_SAMPLING][1] = (UINT16) SamplerOdd.Bits.sampler_odd_offset1;
    BitValue[ODD_SAMPLING][2] = (UINT16) SamplerOdd.Bits.sampler_odd_offset2;
    BitValue[ODD_SAMPLING][3] = (UINT16) SamplerOdd.Bits.sampler_odd_offset3;

    //
    // MinBitValue is only used for GSM_READ_ONLY, so we can return the minimum value across the nibble
    //
    MinBitValue = MaxLimit;
    for (BitIndex = BitStart; BitIndex < BitStop; BitIndex++) {

      SwizzleBit = GetDqSwizzle (Ch, Strobe, BitIndex, Level, Group);

      for (SamplingIndex = SamplingStart; SamplingIndex < SamplingEnd; SamplingIndex++) {
        //
        // Check if read only
        //
        if (Mode & GSM_READ_ONLY) {

          if (BitValue[SamplingIndex][SwizzleBit] < MinBitValue) {
            MinBitValue = BitValue[SamplingIndex][SwizzleBit];
          }

          *Value = MinBitValue;

        } else {
          //
          // Write
          //
          if (Mode & GSM_WRITE_OFFSET) {
            //
            // Add the offset to the current value
            //
            BitValue[SamplingIndex][SwizzleBit] = BitValue[SamplingIndex][SwizzleBit] + *Value;
            //
            // Make sure we do not exceed the limits
            //
            if (BitValue[SamplingIndex][SwizzleBit] > MaxLimit) {
              RcDebugPrintWithDevice (
                SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, BitIndex,
                "RxSampler[%d] is Out of Range!! BitValue[%d] = 0x%x, MaxLimit = 0x%x\n",
                SamplingIndex,
                SwizzleBit,
                BitValue[SamplingIndex][SwizzleBit],
                MaxLimit
                );
              BitValue[SamplingIndex][SwizzleBit] = MaxLimit;
              return MRC_STATUS_FAILURE;
            }
            if (BitValue[SamplingIndex][SwizzleBit] < MinLimit) {
              RcDebugPrintWithDevice (
                SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, BitIndex,
                "RxSampler[%d] is Out of Range!! BitValue[%d] = 0x%x, MinLimit = 0x%x\n",
                SamplingIndex,
                SwizzleBit,
                BitValue[SamplingIndex][SwizzleBit],
                MinLimit
                );
              BitValue[SamplingIndex][SwizzleBit] = MinLimit;
              return MRC_STATUS_FAILURE;
            }

          } else {
            BitValue[SamplingIndex][SwizzleBit] = *Value;
          }
        }
      } // SamplingIndex loop ...
    } // BitIndex loop

    SamplerEven.Bits.sampler_even_offset0 = BitValue[EVEN_SAMPLING][0];
    SamplerEven.Bits.sampler_even_offset1 = BitValue[EVEN_SAMPLING][1];
    SamplerEven.Bits.sampler_even_offset2 = BitValue[EVEN_SAMPLING][2];
    SamplerEven.Bits.sampler_even_offset3 = BitValue[EVEN_SAMPLING][3];

    SamplerOdd.Bits.sampler_odd_offset0 = BitValue[ODD_SAMPLING][0];
    SamplerOdd.Bits.sampler_odd_offset1 = BitValue[ODD_SAMPLING][1];
    SamplerOdd.Bits.sampler_odd_offset2 = BitValue[ODD_SAMPLING][2];
    SamplerOdd.Bits.sampler_odd_offset3 = BitValue[ODD_SAMPLING][3];
    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if ((SamplerEven.Data != (*DdrioChannelNvList)[Ch].RxSamplerEven[Strobe]) || (Mode & GSM_FORCE_WRITE)) {
      WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DDRD_N0_SAMPLER_EVEN_OFFSET_MCIO_DDRIO_REG, SamplerEven.Data);
    }
    if ((SamplerOdd.Data != (*DdrioChannelNvList)[Ch].RxSamplerOdd[Strobe]) || (Mode & GSM_FORCE_WRITE)) {
      WriteDdrioDataRegCh(Host, Socket, Ch, Strobe, DDRD_N0_SAMPLER_ODD_OFFSET_MCIO_DDRIO_REG, SamplerOdd.Data);
    }
    //
    // Save to cache
    //
    if (Mode & GSM_UPDATE_CACHE) {
      (*DdrioChannelNvList)[Ch].RxSamplerEven[Strobe] = SamplerEven.Data;
      (*DdrioChannelNvList)[Ch].RxSamplerOdd[Strobe]  = SamplerOdd.Data;
    }

  }
  return MRC_STATUS_SUCCESS;
}

/**

  Get/Set function to modify the RxSampler at a bit or strobe level

  @param[in]      Host        Pointer to SysHost structure
  @param[in]      Socket      Current socket under test (0-based)
  @param[in]      Ch          Current channel under test (0-based)
  @param[in]      Dimm        Current dimm under test (0-based)
  @param[in]      Rank        Current rank under test (0-based)
  @param[in]      Strobe      Current strobe under test (0-based || ALL_STROBES)
  @param[in]      Bit         Current bit under test (0-based || ALL_BITS)
  @param[in]      Level       Current level (not applicable)
  @param[in]      Group       Current group under test (RxSamplerEvenOdd, RxSamplerEven or RxSamplerOdd)
  @param[in]      Mode        Register/Cache access mode (GSM_READ_CSR, GSM_READ_ONLY, GSM_UPDATE_CACHE, GSM_FORCE_WRITE)
  @param[in,out]  Value       Pointer to data that will be used to retrieve or store

  @retval         MRC_STATUS_STROBE_NOT_PRESENT     Invalid strobe parameter
  @retval         MRC_STATUS_INVALID_PARAMETER      Invalid bit parameter
  @retval         MRC_STATUS_INVALID_PARAMETER      Invalid Group parameter
  @retval         MRC_STATUS_SUCCESS                Otherwise

**/
MRC_STATUS
GetSetRxSamplerEvenOdd (
  IN        PSYSHOST  Host,
  IN        UINT8     Socket,
  IN        UINT8     Ch,
  IN        UINT8     Dimm,
  IN        UINT8     Rank,
  IN        UINT8     Strobe,
  IN        UINT8     Bit,
  IN        MRC_LT    Level,
  IN        MRC_GT    Group,
  IN        UINT8     Mode,
  IN        INT16     *Value
  )
{
  MRC_STATUS Status;
  INT16      OriginalOffset;

  if ((Group == RxSamplerEven || Group == RxSamplerOdd) && Level == DdrLevel) {

    if (FeaturePcdGet (PcdUseRxTxMultiCastRegisters)) {
      //
      // Before any operation happen for Even or Odd knobs, it is required
      // to make sure the multicast register is flushed into the unicast registers, so
      // any operation (READ/WRITE_OFFSET) is accurate
      //
      Status  = GetSetDelayMulticastOffset (Host, Socket, Ch, Dimm, Rank, ALL_STROBES, ALL_BITS, Level, RxSamplerPerChDelayOffset, GSM_READ_ONLY, &OriginalOffset);

      if (Status != MRC_STATUS_SUCCESS) {

        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_24);

      }

      if (OriginalOffset != 0) {

        FlushMulticastIntoUnicast (Host, Socket, Ch, Dimm, Rank, Level, RxSamplerEvenOdd, RxSamplerPerChDelayOffset, &OriginalOffset);

      }

    }

    Status = GetSetRxSamplerEvenOddUnicast (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, Group, Mode, Value);

  } else if (Group == RxSamplerEvenOdd && Level == DdrLevel) {

    Status = GetSetMulticastUnicast(Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, Group, RxSamplerPerChDelayOffset, Mode, Value);

  } else {

    Status = GetSetRxSamplerEvenOddUnicast (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, Group, Mode, Value);

  }

  return Status;

}

/**

  Get Set RxSampler Offset Adjust Unicast function.

  @param[in]     Host        - Pointer to sysHost
  @param[in]     Socket      - Socket number
  @param[in]     Ch          - Channel number (0-based)
  @param[in]     Dimm        - DIMM number (0-based)
  @param[in]     Rank        - Rank number (0-based)
  @param[in]     Strobe      - Strobe number (0-based)
  @param[in]     Level       - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group       - CSM_GT - Parameter to program
  @param[in]     Mode        - Mode parameter
                               0: do not read from cache
                               1: read from cache
                               2: use cache to program register (used for fast boot or S3)
  @param[in,out] Value       - Data to program

  @retval        MRC_STATUS_STROBE_NOT_PRESENT    Invalid Strobe parameter
  @retval        MRC_STATUS_LEVEL_NOT_SUPPORTED   Invalid Level parameter
  @retval        MRC_STATUS_SUCCESS               Otherwise

**/
MRC_STATUS
GetSetRxSamplerOffsetAdjUnicast (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Strobe,
  IN MRC_LT    Level,
  IN MRC_GT    Group,
  IN UINT8     Mode,
  IN OUT INT16 *Value
  )
{
  UINT8                             MaxStrobe;
  UINT16                            CurVal;
  INT16                             FinalValue = 0;
  INT16                             Tmp;
  UINT16                            MaxLimit = 31;
  INT16                             MinLimit = -32;
  UINT16                            UsDelay = 1;
  DDRIO_CHANNEL_NVDATA  (*DdrioChannelNvList) [MAX_CH];
  BOOLEAN                           RegisterAccessed;
  DDRD_N0_TRAIN_OFFSET0_MCIO_DDRIO_STRUCT DdrdN0TrainOffset0;

  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  CountTrackingData (PTC_RX_SAMPLER_OFFSET_MOVE, 1);

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  DdrdN0TrainOffset0.Data = 0;

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits (Host, Level, Group, &MinLimit, &MaxLimit, &UsDelay);

  if (Strobe >= MAX_STROBE && Strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
      "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((Strobe < MAX_STROBE || Strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (Level != DdrLevel) {
    return MRC_STATUS_LEVEL_NOT_SUPPORTED;
  }

  if (Strobe == ALL_STROBES) {
    Strobe = 0;
    MaxStrobe = MSVx4;
  } else {
    MaxStrobe = Strobe + 1;
    MemCheckIndex (Strobe, MSVx4);
  }

  for (; Strobe < MaxStrobe; Strobe++) {
    if (Mode & GSM_READ_CSR) {
      //
      // Read from PCI config space
      //
      DdrdN0TrainOffset0.Data = ReadDdrioDataRegCh (Host, Socket, Ch, Strobe, DDRD_N0_TRAIN_OFFSET0_MCIO_DDRIO_REG);
    } else {
      //
      // Read from cache
      //
      DdrdN0TrainOffset0.Data = (*DdrioChannelNvList) [Ch].DdrdN0TrainOffset0[Strobe];

      RcDebugPrintWithDevice (SDBG_REG_ACCESS, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, Strobe, NO_BIT,
        "DdrdN0TrainOffset0 = %2d \n", DdrdN0TrainOffset0.Data);
    }

    CurVal = (UINT16) DdrdN0TrainOffset0.Bits.sampler_offset_adj;

    //
    // Conversion to INT16, need to extend the sign bit.
    //
    if (CurVal & BIT5) {
      CurVal = UINT16_MINUS_COMPENSATION_5_BITS | CurVal;
    }
    FinalValue = CurVal;
    //
    // Check if read only
    //
    if (Mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *Value = CurVal;
    } else {
      //
      // Write
      // Adjust the current value by offset
      //
      if (Mode & GSM_WRITE_OFFSET) {
        Tmp = CurVal + *Value;
        //
        // Ensure we do not exceed minimums
        //
        if (Tmp > MinLimit) {
          FinalValue = Tmp;
        } else {
          FinalValue = MinLimit;
        }
        //
        // Ensure we do not exceed maximums
        //
        if (FinalValue > MaxLimit) {
          FinalValue = MaxLimit;
        }
      } else {
        //
        // Write absolute value
        //
        FinalValue = *Value;
      }
      DdrdN0TrainOffset0.Bits.sampler_offset_adj = FinalValue;
      RegisterAccessed = FALSE;
      //
      // Write it back if the current data does not equal the cache value or if in force write mode
      //
      if ((DdrdN0TrainOffset0.Data != (*DdrioChannelNvList) [Ch].DdrdN0TrainOffset0[Strobe]) || (Mode & GSM_FORCE_WRITE)) {
        RegisterAccessed = TRUE;
        WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DDRD_N0_TRAIN_OFFSET0_MCIO_DDRIO_REG, DdrdN0TrainOffset0.Data);
      }

      if (RegisterAccessed) {
        //
        // Wait for the new value to settle
        //
        FixedDelayMicroSecond (UsDelay);
      }
    } // if read only
    //
    // Save to cache
    //
    if (Mode & GSM_UPDATE_CACHE) {
      (*DdrioChannelNvList) [Ch].DdrdN0TrainOffset0[Strobe] = DdrdN0TrainOffset0.Data;
    }
  } // Strobe loop
  return MRC_STATUS_SUCCESS;
} // GetSetRxSamplerOffsetAdjUnicast

/**

  Get Strobe number after considering nibble swap inside the data buffer.

  @param[in]     Channel      - Channel number within the socket
  @param[in]     Dimm         - Dimm number within the Channel
  @param[in]     Strobe       - Strobe number within Channel

  @retval Strobe number

**/
UINT8
GetNibbleSwappedStrobe (
  IN     UINT8     Channel,
  IN     UINT8     Dimm,
  IN     UINT8     Strobe
  )
{
  UINT8                         NewStrobe;
  BOOLEAN                       (*NibbleSwap)[MAX_CH][MAX_DIMM][SUB_CH][MAX_STROBE_DDR5 / 2];
  DDR5_LRDIMM_TRAINING_DATA     *TrainingData;
  PSYSHOST                      Host;
  UINT8                         MSVx4;
  UINT8                         MaxStrobePairs;
  MRC_STATUS                    Status = MRC_STATUS_SUCCESS;

  Host = GetSysHostPointer ();
  MSVx4 = GetMaxStrobeValid(Host);
  MaxStrobePairs = GetSubChMaxStrobeValid (Host) / 2;

  NewStrobe = Strobe;

  if (!VALID_LOGICAL_STROBE (Strobe, MSVx4, MSVx4 / 2)) {
    return NewStrobe;
  }

  //
  // Retrieve DB to DRAM DQ mapping info
  //
  Status = GetDdr5LrdimmTrainingData (&TrainingData);
  if (Status != MRC_STATUS_SUCCESS) {
    return NewStrobe;
  }

  NibbleSwap = &(TrainingData->NibbleSwap);

  if ((*NibbleSwap)[Channel][Dimm][Strobe / (MSVx4 / 2)][Strobe % (MSVx4 / 2)]) {
    //
    // When swap happens, the strobes corresponding to the upper and lower nibble are swapped.
    // Here are the mapping:
    // strobe of lower nibble <-> strobe of upper nibble
    // 0  <-> 5     1 <-> 6    2 <-> 7     3 <-> 8     4 <-> 9
    // 10 <-> 15   11 <-> 16  12 <-> 17   13 <-> 18   14 <-> 19
    //
    if (Strobe < MaxStrobePairs) {
      NewStrobe = Strobe + MaxStrobePairs;
    } else if (Strobe < MaxStrobePairs * 2) {
      NewStrobe = Strobe - MaxStrobePairs;
    } else if (Strobe < MaxStrobePairs * 3) {
      NewStrobe = Strobe + MaxStrobePairs;
    } else if (Strobe < MaxStrobePairs * 4) {
      NewStrobe = Strobe - MaxStrobePairs;
    }
  } // Swapped happen

  return NewStrobe;
}

/**

  Get or Set Rx Vref for LRDIMM buffers using Paged Vref Control Words
  PG[2]RW[F1:F0] - DRAM Interface Internal VrefDQ Control Word

  @param[in]     Host    - Pointer to sysHost
  @param[in]     Socket  - Socket number
  @param[in]     Ch      - Channel number within the socket
  @param[in]     Dimm    - DIMM number within the channel
  @param[in]     Rank    - Rank number within the DIMM
  @param[in]     Strobe  - Strobe number within the 80-bit channel
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param[in,out] Value   - Data to program or return buffer

  @retval MRC_STATUS_SUCCESS if data is read or programmed successfully
  @retval !MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS
GetSetLrdimmBufferRxVref (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     Dimm,
  IN     UINT8     Rank,
  IN     UINT8     Strobe,
  IN     MRC_LT    Level,
  IN     MRC_GT    Group,
  IN     UINT8     Mode,
  IN OUT INT16     *Value
  )
{
#ifdef LRDIMM_SUPPORT
  UINT8                 MaxStrobe;
  UINT8                 NewStrobe = 0; // After consider nibble swap in DB
  INT16                 CurVal = 0;
  INT16                 NewVref;
  INT16                 Tmp;
  UINT16                MaxLimit = 0;
  UINT16                MinLimit = 0;
  UINT16                UsDelay = 0;
  struct ddrRank        (*RankList) [MAX_RANK_DIMM];
  struct rankDevice     (*RankStruct)[MAX_RANK_DIMM];
  UINT8                 MSVx4;
  UINT8                 MaxStrobeCh;
  UINT8                 MaxStrobeSubCh;
  UINT8                 SubChMask;
  SYS_SETUP             *Setup;

  MSVx4          = GetMaxStrobeValid (Host);
  MaxStrobeCh    = MSVx4;
  MaxStrobeSubCh = GetSubChMaxStrobeValid (Host);
  Setup       = GetSysSetupPointer ();

  RankList = GetRankNvList (Host, Socket, Ch, Dimm);
  RankStruct  = GetRankStruct (Host, Socket, Ch, Dimm);

  CountTrackingData (PTC_VREF_MOVE, 1);

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits (Host, Level, Group, &MinLimit, &MaxLimit, &UsDelay);

  if (Strobe >= MaxStrobeCh && Strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
      "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((Strobe < MaxStrobeCh || Strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (Strobe == ALL_STROBES) {
    Strobe = 0;
    MaxStrobe = MaxStrobeCh;
  } else {
    MaxStrobe = Strobe + 1;
    if (MaxStrobe > MaxStrobeCh) {
      return MRC_STATUS_STROBE_NOT_PRESENT;
    }
  }

  for (; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
      continue;
    }
    if (!VALID_LOGICAL_STROBE (Strobe, MaxStrobeCh, MaxStrobeSubCh)) {
      continue;
    }

    NewStrobe = GetNibbleSwappedStrobe (Ch, Dimm, Strobe);

    if (Mode & GSM_READ_CSR) {
      // RxVref value is shared by the ranks, just use the rank 0 value
      CurVal = (*RankList)[0].lrbufRxVref[NewStrobe];
    } else {
      CurVal = (*RankStruct)[0].lrbufRxVrefCache[NewStrobe];
    } // if cached
    CurVal = ConvertPhysicalToLinear (CurVal);

    //
    // Check if read only
    //
    if (Mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *Value = CurVal;
    } else {
      //
      // Write
      //
      if (Mode & GSM_WRITE_OFFSET) {
        //
        // Add the offset to the current value
        //
        Tmp = CurVal + *Value;

      } else {
        //
        // Write absolute value
        //
        Tmp = *Value;
      }

      //
      // Make sure we do not exeed the limits
      //
      if (Tmp > MinLimit) {
        NewVref = Tmp;
      } else {
        NewVref = MinLimit;
      }
      // Ensure we do not exceed maximums
      if (NewVref > MaxLimit) {
        NewVref = MaxLimit;
      }

      NewVref = ConvertLinearToPhysical (NewVref);
      //
      // Write it back if the current data does not equal the current value or if in force write Mode
      //

      //
      // RxVref value is shared by the ranks, just use the rank 0 value
      //
      if ((NewVref != (*RankList)[0].lrbufRxVref[NewStrobe]) || (Mode & GSM_FORCE_WRITE)) {

        (*RankList)[0].lrbufRxVref[NewStrobe] = (UINT8) NewVref;

        //
        // Strobe is "logical" per-channel, convert it to per-subchannel and set the subchannel mask
        // Logical channel strobe numbers for x8 devices are 0-4 and 10-14
        //
        if (NewStrobe < (MaxStrobeCh / 2)) {
          SubChMask = SUB_CH_A_MASK;
        } else {
          SubChMask = SUB_CH_B_MASK;
        }
        //
        // Write the new vref
        //
        MemWriteLrdimmBufferDramVrefDq (Host, Socket, Ch, SubChMask, Dimm, Rank, NewStrobe % (MaxStrobeCh / 2), (UINT8)NewVref);
      }
    } // if read only

    //
    // Save to cache
    //
    if (Mode & GSM_UPDATE_CACHE) {
      //
      // RxVref value is shared by the ranks, just use the rank 0 value
      //
      (*RankStruct)[0].lrbufRxVrefCache[NewStrobe] = (*RankList)[0].lrbufRxVref[NewStrobe];
    }

  } // Strobe loop

  return MRC_STATUS_SUCCESS;
#else //LRDIMM
  return MRC_STATUS_FAILURE;
#endif //LRDIMM
} // GetSetLrdimmBufferRxVref

/**

  Get or Set Tx Vref for LRDIMM buffers using Paged Vref Control Words
  PG[2]RW[E7:E0] - Host Interface Internal VrefDQ Control Word

  @param[in]     Host    - Pointer to sysHost
  @param[in]     Socket  - Socket number
  @param[in]     Ch      - Channel number within the socket
  @param[in]     SubCh   - SubChannel number within the channel
  @param[in]     Dimm    - DIMM number within the channel
  @param[in]     Rank    - Rank number within the DIMM
  @param[in]     Strobe  - Strobe number within the 80-bit channel
  @param[in]     Bit     - Bit number withtin the 80-bit channel
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param[in,out] Value   - Data to program or return buffer

  @retval MRC_STATUS_SUCCESS if data is read or programmed successfully
  @retval !MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS
GetSetLrdimmBufferTxVref (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     SubCh,
  IN     UINT8     Dimm,
  IN     UINT8     Rank,
  IN     UINT8     Strobe,
  IN     UINT8     Bit,
  IN     MRC_LT    Level,
  IN     MRC_GT    Group,
  IN     UINT8     Mode,
  IN OUT INT16     *Value
  )
{
#ifdef LRDIMM_SUPPORT
  UINT8                 MaxStrobe;
  UINT8                 Dq;
  UINT16                CurVal = 0;
  INT16                 NewVref;
  INT16                 Tmp;
  UINT16                MaxLimit = 0;
  UINT16                MinLimit = 0;
  UINT16                UsDelay = 0;
  DDRIO_CHANNEL_NVDATA  (*DdrioChannelNvList) [MAX_CH];
  struct ddrRank        (*RankList) [MAX_RANK_DIMM];
  UINT8                 TmpStrobe;
  UINT8                 MSVx4;
  UINT8                 MSVx8;
  UINT8                 MSVx8SubCh;
  UINT8                 MaxStrobeCh;
  UINT8                 MaxStrobeSubCh;
  UINT8                 SubChMask;
  SYS_SETUP             *Setup;
  LRBUFF_HOST_VREF      LrbuffVref;

  MSVx4          = GetMaxStrobeValid (Host);
  MSVx8          = MSVx4 / 2;
  MSVx8SubCh     = GetSubChMaxStrobeValid (Host) / 2;
  MaxStrobeCh    = MSVx4;
  MaxStrobeSubCh = MSVx8SubCh;
  Setup       = GetSysSetupPointer ();
  ZeroMem (&LrbuffVref, sizeof (LrbuffVref));

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  RankList = GetRankNvList (Host, Socket, Ch, Dimm);

  CountTrackingData (PTC_VREF_MOVE, 1);

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits (Host, Level, Group, &MinLimit, &MaxLimit, &UsDelay);

  if (Strobe >= MaxStrobeCh && Strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
      "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((Strobe < MaxStrobeCh || Strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if ((Setup->mem.optionsExt & PDA_EN) &&
      ((Host->nvram.mem.socket[Socket].TrainingStepDone.TxVref == 1))) {

    if (Strobe == ALL_STROBES) {
      Strobe = 0;
      MaxStrobe = MaxStrobeCh;
    } else {
      MaxStrobe = Strobe + 1;
      if (MaxStrobe > MaxStrobeCh) {
        return MRC_STATUS_STROBE_NOT_PRESENT;
      }
    }

    for (; Strobe < MaxStrobe; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      if (!VALID_LOGICAL_STROBE (Strobe, MaxStrobeCh, MaxStrobeSubCh)) {
        continue;
      }

      if (Mode & GSM_READ_CSR) {
        CurVal = (*DdrioChannelNvList) [Ch].txVref[ (*RankList) [Rank].rankIndex][Strobe];
      } else {
        CurVal = (*DdrioChannelNvList) [Ch].txVrefCache[ (*RankList) [Rank].rankIndex][Strobe];
      } // if cached
      CurVal = ConvertPhysicalToLinear (CurVal);

      //
      // Check if read only
      //
      if (Mode & GSM_READ_ONLY) {
        //
        // Save the value read into the output parameter
        //
        *Value = CurVal;
      } else {
        //
        // Write
        //
        if (Mode & GSM_WRITE_OFFSET) {
          //
          // Add the offset to the current value
          //
          Tmp = CurVal + *Value;

        } else {
          //
          // Write absolute value
          //
          Tmp = *Value;
        }

        //
        // Make sure we do not exeed the limits
        //
        if (Tmp > MinLimit) {
          NewVref = Tmp;
        } else {
          NewVref = MinLimit;
        }
        // Ensure we do not exceed maximums
        if (NewVref > MaxLimit) {
          NewVref = MaxLimit;
        }

        NewVref = ConvertLinearToPhysical (NewVref);
        //
        // Write it back if the current data does not equal the current value or if in force write Mode
        //
        if ((NewVref != (*DdrioChannelNvList) [Ch].txVref[ (*RankList) [Rank].rankIndex][Strobe]) || (Mode & GSM_FORCE_WRITE)) {

          (*DdrioChannelNvList) [Ch].txVref[ (*RankList) [Rank].rankIndex][Strobe] = (UINT8) NewVref;

          //
          // MemWriteLrdimmBufferHostVrefDq supports per-Dq Vref values as input
          //
          for (Dq = 0; Dq < MAX_BITS_IN_BYTE; Dq++) {
            LrbuffVref.VrefDq[Dq] = (UINT8)NewVref;
          }
          //
          // Strobe is "logical" per-channel, convert it to per-subchannel and set the subchannel mask
          // Logical channel strobe numbers for x8 devices are 0-4 and 10-14
          //
          if (Strobe < (MaxStrobeCh / 2)) {
            SubChMask = SUB_CH_A_MASK;
          } else {
            SubChMask = SUB_CH_B_MASK;
          }
          //
          // Write the new vref
          //
          MemWriteLrdimmBufferHostVrefDq (Host, Socket, Ch, SubChMask, Dimm, Rank, Strobe % (MaxStrobeCh / 2), ALL_BITS, &LrbuffVref);
        }
      } // if read only

      //
      // Save to cache
      //
      if (Mode & GSM_UPDATE_CACHE) {
        (*DdrioChannelNvList) [Ch].txVrefCache[ (*RankList) [Rank].rankIndex][Strobe] = (*DdrioChannelNvList) [Ch].txVref[ (*RankList) [Rank].rankIndex][Strobe];
      }

      // Wait for Wr Vref circuit to settle
      FixedDelayMicroSecond (UsDelay);
    } // Strobe loop

    //
    // Make sure Vref settles
    //
    FixedDelayMicroSecond (1);

  } else {
    if (Mode & GSM_READ_CSR) {
      CurVal = (*DdrioChannelNvList) [Ch].txVref[ (*RankList) [Rank].rankIndex][0];
    } else {
      CurVal = (*DdrioChannelNvList) [Ch].txVrefCache[ (*RankList) [Rank].rankIndex][0];
    } // if cached

    CurVal = ConvertPhysicalToLinear (CurVal);

    //
    // Check if read only
    //
    if (Mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *Value = CurVal;
    } else {
      //
      // Write
      //
      if (Mode & GSM_WRITE_OFFSET) {
        //
        // Add the offset to the current value
        //
        Tmp = CurVal + *Value;

      } else {
        //
        // Write absolute value
        //
        Tmp = *Value;
      }

      //
      // Make sure we do not exeed the limits
      //
      if (Tmp > MinLimit) {
        NewVref = Tmp;
      } else {
        NewVref = MinLimit;
      }
      // Ensure we do not exceed maximums
      if (NewVref > MaxLimit) {
        NewVref = MaxLimit;
      }

      NewVref = ConvertLinearToPhysical (NewVref);
      //
      // Write it back if the current data does not equal the current Value or if in force write Mode
      //
      if ((NewVref != (*DdrioChannelNvList) [Ch].txVref[ (*RankList) [Rank].rankIndex][0]) || (Mode & GSM_FORCE_WRITE)) {

        for (TmpStrobe = 0; TmpStrobe < MaxStrobeCh; TmpStrobe++) {
          if (!VALID_LOGICAL_STROBE (Strobe, MaxStrobeCh, MaxStrobeSubCh)) {
            continue;
          }
          (*DdrioChannelNvList) [Ch].txVref[ (*RankList) [Rank].rankIndex][Strobe] = (UINT8) NewVref;
        } // Strobe loop
        //
        // MemWriteLrdimmBufferHostVrefDq supports per-Dq Vref values as input
        //
        for (Dq = 0; Dq < MAX_BITS_IN_BYTE; Dq++) {
          LrbuffVref.VrefDq[Dq] = (UINT8)NewVref;
        }
        //
        // Write the new vref
        //
        MemWriteLrdimmBufferHostVrefDq (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, ALL_STROBES, ALL_BITS, &LrbuffVref);
      }
    } // if read only

    //
    // Save to cache
    //
    if (Mode & GSM_UPDATE_CACHE) {
      for (Strobe = 0; Strobe < MaxStrobeCh; Strobe++) {
        if (!VALID_LOGICAL_STROBE (Strobe, MaxStrobeCh, MaxStrobeSubCh)) {
          continue;
        }
        (*DdrioChannelNvList) [Ch].txVrefCache[ (*RankList) [Rank].rankIndex][Strobe] = (*DdrioChannelNvList) [Ch].txVref[ (*RankList) [Rank].rankIndex][Strobe];
      } // Strobe loop
    }

    // Wait for Wr Vref circuit to settle
    FixedDelayMicroSecond (UsDelay);
  }

  return MRC_STATUS_SUCCESS;
#else //LRDIMM
  return MRC_STATUS_FAILURE;
#endif //LRDIMM
} // GetSetLrdimmBufferTxVref

/**

  Get or Set Tx Vref for DRAM

  @param[in]     Host    - Pointer to sysHost
  @param[in]     Socket  - Socket number
  @param[in]     Ch      - Channel number within the socket
  @param[in]     SubCh   - SubChannel number within the channel
  @param[in]     Dimm    - DIMM number within the channel
  @param[in]     Rank    - Rank number within the DIMM
  @param[in]     Strobe  - Strobe number within the 80-bit channel
  @param[in]     Bit     - Bit number withtin the 80-bit channel
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param[in,out] Value   - Data to program or return buffer

  @retval MRC_STATUS_SUCCESS if data is read or programmed successfully
  @retval !MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS
GetSetDramTxVref (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     SubCh,
  IN     UINT8     Dimm,
  IN     UINT8     Rank,
  IN     UINT8     Strobe,
  IN     UINT8     Bit,
  IN     MRC_LT    Level,
  IN     MRC_GT    Group,
  IN     UINT8     Mode,
  IN OUT INT16     *Value
  )
{
  UINT16                curVal = 0;
  INT16                 newVref;
  INT16                 tmp;
  UINT16                maxLimit = 0;
  UINT16                minLimit = 0;
  UINT16                usDelay = 0;
  DDRIO_CHANNEL_NVDATA  (*DdrioChannelNvList) [MAX_CH];
  struct ddrRank        (*rankList) [MAX_RANK_DIMM];
  struct rankDevice     (*rankStruct) [MAX_RANK_DIMM];
  UINT8                 RegisterValue;
  UINT8                 CachedValue;
  UINT8                 MSVx4;
  UINT8                 MSVx4SubCh;
  UINT8                 MSVx8SubCh;
  UINT8                 MaxStrobeCh;
  UINT8                 MaxStrobeSubCh;
  UINT8                 MaxStrobe;
  UINT8                 SubChMask;
  SYS_SETUP             *Setup;

  Setup = GetSysSetupPointer ();

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  rankList = GetRankNvList (Host, Socket, Ch, Dimm);
  rankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
  MSVx4 = GetMaxStrobeValid (Host);
  MSVx4SubCh = GetSubChMaxStrobeValid (Host);
  MSVx8SubCh = MSVx4SubCh / 2;
  CountTrackingData (PTC_VREF_MOVE, 1);

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits (Host, Level, Group, &minLimit, &maxLimit, &usDelay);

  MaxStrobeSubCh = MSVx8SubCh;
  if (IsX4Dimm (Socket, Ch, Dimm)) {
    MaxStrobeSubCh = MSVx4SubCh;
  }
  MaxStrobeCh = MSVx4;

  if (Strobe >= MaxStrobeCh && Strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, Strobe, NO_BIT,
      "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((Strobe < MaxStrobeCh || Strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if ((Setup->mem.optionsExt & PDA_EN) &&
      ((Host->nvram.mem.socket[Socket].TrainingStepDone.TxVref == 1))) {

    if (Strobe == ALL_STROBES) {
      Strobe = 0;
      MaxStrobe = MaxStrobeCh;
    } else {
      MaxStrobe = Strobe + 1;
      if (MaxStrobe > MaxStrobeCh) {
        return MRC_STATUS_STROBE_NOT_PRESENT;
      }
    }

    for (; Strobe < MaxStrobe; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      if (!VALID_LOGICAL_STROBE (Strobe, MaxStrobeCh, MaxStrobeSubCh)) {
        //
        // For x8 devices, valid logical strobe numbers in the channel are 0-4 and 10-14
        // Skip the other strobes
        //
        continue;
      }

      if (Level == DdrLevel) {
        RegisterValue = (*DdrioChannelNvList) [Ch].txVref[ (*rankList) [Rank].rankIndex][Strobe];
        CachedValue   = (*DdrioChannelNvList) [Ch].txVrefCache[ (*rankList) [Rank].rankIndex][Strobe];
      } else {
        RegisterValue = (UINT8) (*rankList) [Rank].lrbufTxVref[Strobe];
        CachedValue   = (UINT8) (*rankStruct) [Rank].lrbufTxVrefCache[Strobe];
      }

      if (Mode & GSM_READ_CSR) {
        curVal = RegisterValue;
      } else {
        curVal = CachedValue;
      } // if cached
      curVal = ConvertPhysicalToLinear (curVal);

      //
      // Check if read only
      //
      if (Mode & GSM_READ_ONLY) {
        //
        // Save the value read into the output parameter
        //
        *Value = curVal;
      } else {
        //
        // Write
        //
        if (Mode & GSM_WRITE_OFFSET) {
          //
          // Add the offset to the current value
          //
          tmp = curVal + *Value;

        } else {
          //
          // Write absolute value
          //
          tmp = *Value;
        }

        //
        // Make sure we do not exeed the limits
        //
        if (tmp > minLimit) {
          newVref = tmp;
        } else {
          newVref = minLimit;
        }
        // Ensure we do not exceed maximums
        if (newVref > maxLimit) {
          newVref = maxLimit;
        }

        newVref = ConvertLinearToPhysical (newVref);

        //
        // Write it back if the current data does not equal the current value or if in force write Mode
        //
        if ((newVref != RegisterValue) || (Mode & GSM_FORCE_WRITE)) {

          if (Level == DdrLevel) {
            (*DdrioChannelNvList) [Ch].txVref[ (*rankList) [Rank].rankIndex][Strobe] = (UINT8) newVref;
          } else {
            (*rankList) [Rank].lrbufTxVref[Strobe] = (UINT8) newVref;
          }

          //
          // Write the new vref
          // Strobe is per-channel, convert it to per-subchannel and set the subchannel mask
          //
          if (Strobe < (MaxStrobeCh / 2)) {
            SubChMask = SUB_CH_A_MASK;
          } else {
            SubChMask = SUB_CH_B_MASK;
          }
          MemWriteDramVref (Host, Socket, Ch, SubChMask, Dimm, Rank, Strobe % (MaxStrobeCh / 2), (UINT8) newVref);
        }

      } // if read only

      //
      // Save to cache
      //
      if (Mode & GSM_UPDATE_CACHE) {
        if (Level == DdrLevel) {
          (*DdrioChannelNvList) [Ch].txVrefCache[ (*rankList) [Rank].rankIndex][Strobe] = (*DdrioChannelNvList) [Ch].txVref[ (*rankList) [Rank].rankIndex][Strobe];
        } else {
          (*rankStruct) [Rank].lrbufTxVrefCache[Strobe] = (*rankList) [Rank].lrbufTxVref[Strobe];
        }
      }

      // Wait for Wr Vref circuit to settle
      FixedDelayMicroSecond (usDelay);
    } // Strobe loop

    //
    // Make sure Vref settles
    //
    FixedDelayMicroSecond (1);

  } else {
    if (Level == DdrLevel) {
      RegisterValue = (*DdrioChannelNvList) [Ch].txVref[ (*rankList) [Rank].rankIndex][0];
      CachedValue   = (*DdrioChannelNvList) [Ch].txVrefCache[ (*rankList) [Rank].rankIndex][0];
    } else {
      RegisterValue = (UINT8) (*rankList) [Rank].lrbufTxVref[0];
      CachedValue   = (UINT8) (*rankStruct) [Rank].lrbufTxVrefCache[0];
    }

    if (Mode & GSM_READ_CSR) {
      curVal = RegisterValue;
    } else {
      curVal = CachedValue;
    } // if cached

    curVal = ConvertPhysicalToLinear (curVal);

    //
    // Check if read only
    //
    if (Mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *Value = curVal;
    } else {
      //
      // Write
      //
      if (Mode & GSM_WRITE_OFFSET) {
        //
        // Add the offset to the current value
        //
        tmp = curVal + *Value;

      } else {
        //
        // Write absolute value
        //
        tmp = *Value;
      }

      //
      // Make sure we do not exeed the limits
      //
      if (tmp > minLimit) {
        newVref = tmp;
      } else {
        newVref = minLimit;
      }
      // Ensure we do not exceed maximums
      if (newVref > maxLimit) {
        newVref = maxLimit;
      }

      newVref = ConvertLinearToPhysical (newVref);

      //
      // Write it back if the current data does not equal the current value or if in force write Mode
      //
      if ((newVref != RegisterValue) || (Mode & GSM_FORCE_WRITE)) {

        for (Strobe = 0; Strobe < MaxStrobeCh; Strobe++) {
          if (!VALID_LOGICAL_STROBE (Strobe, MaxStrobeCh, MaxStrobeSubCh)) {
            continue;
          }
          if (Level == DdrLevel) {
            (*DdrioChannelNvList) [Ch].txVref[ (*rankList) [Rank].rankIndex][Strobe] = (UINT8) newVref;
          } else {
            (*rankList) [Rank].lrbufTxVref[Strobe] = (UINT8) newVref;
          }
        } // Strobe loop

        //
        // Write the new vref
        //
        MemWriteDramVref (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, ALL_STROBES, (UINT8) newVref);

      }
    } // if read only

    //
    // Save to cache
    //
    if (Mode & GSM_UPDATE_CACHE) {
      for (Strobe = 0; Strobe < MaxStrobeCh; Strobe++) {
        if (!VALID_LOGICAL_STROBE (Strobe, MaxStrobeCh, MaxStrobeSubCh)) {
          continue;
        }
        if (Level == DdrLevel) {
          (*DdrioChannelNvList) [Ch].txVrefCache[ (*rankList) [Rank].rankIndex][Strobe] = (*DdrioChannelNvList) [Ch].txVref[ (*rankList) [Rank].rankIndex][Strobe];
        } else {
          (*rankStruct) [Rank].lrbufTxVrefCache[Strobe] = (*rankList) [Rank].lrbufTxVref[Strobe];
        }
      } // Strobe loop
    }

    // Wait for Wr Vref circuit to settle
    FixedDelayMicroSecond (usDelay);
  }

  return MRC_STATUS_SUCCESS;
} // GetSetDramTxVref

/**

  Get or Set Tx Vref

  @param[in]     Host    - Pointer to sysHost
  @param[in]     Socket  - Socket number
  @param[in]     Ch      - Channel number within the socket
  @param[in]     SubCh   - SubChannel number wihtin the channel
  @param[in]     Dimm    - DIMM number within the channel
  @param[in]     Rank    - Rank number within the DIMM
  @param[in]     Strobe  - Strobe number within the 80-bit channel
  @param[in]     Bit     - Bit number withtin the 80-bit channel
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param[in,out] Value   - Data to program or return buffer

  @retval MRC_STATUS_SUCCESS if data is read or programmed successfully
  @retval !MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS
GetSetTxVref (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     SubCh,
  IN     UINT8     Dimm,
  IN     UINT8     Rank,
  IN     UINT8     Strobe,
  IN     UINT8     Bit,
  IN     MRC_LT    Level,
  IN     MRC_GT    Group,
  IN     UINT8     Mode,
  IN OUT INT16     *Value
  )
{
  //
  // Check for NVMDIMM Present and LrbufLevel, then call GetSetTxVrefFnv
  //
  if (IsDcpmmPresentDimm (Socket, Ch, Dimm) && (Level == LrbufLevel)) {
    return GetSetTxVrefFnv (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, Group, Mode, Value);
  }

  //
  // LRDIMM buffer front side
  //
  if (IsLrdimmPresent (Socket, Ch, Dimm) && (Level == DdrLevel)) {
    return GetSetLrdimmBufferTxVref (Host, Socket, Ch, SubCh, Dimm, Rank, Strobe, Bit, Level, Group, Mode, Value);
  }

  //
  // DRAM, front side or back side
  //
  return GetSetDramTxVref (Host, Socket, Ch, SubCh, Dimm, Rank, Strobe, Bit, Level, Group, Mode, Value);

}

/**

  Get Imode

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param strobe  - Strobe number (0-based)
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval IO delay

**/
STATIC
MRC_STATUS
GetSetImode (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     strobe,
  UINT8     mode,
  INT16     *value
  )
{
#if 0 //SPR_TODO_DDRIO. To enable.
  UINT8                               maxStrobe;
  UINT16                              curVal = 0;
  UINT16                              piDelay;
  INT16                               tmp;
  UINT16                              maxLimit = 15;
  UINT16                              minLimit = 0;
  DDRIO_CHANNEL_NVDATA                (*DdrioChannelNvList)[MAX_CH];
  DATACONTROL2N0_0_MCIO_DDRIO_STRUCT  DataControl2N0;
  DATACONTROL2N0_0_MCIO_DDRIO_STRUCT  DataControl2N1;
  UINT8                               MSVx8 = GetMaxStrobeValid(Host) / 2;

  DdrioChannelNvList = GetDdrioChannelNvList(Host, socket);

  if (strobe >= MAX_STROBE && strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, strobe, NO_BIT,
                   "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((strobe < MAX_STROBE || strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (strobe == ALL_STROBES) {
    strobe    = 0;
    maxStrobe = MSVx8;
  } else {
    maxStrobe = strobe + 1;
  }

  MemCheckIndex (strobe, MSVx8);

  for (; strobe < maxStrobe; strobe++) {
    if (IsStrobeNotValid(Host, strobe)) {
      continue;
    }

    if(mode & GSM_READ_CSR) {
      //
      // Read from PCI config space
      //
      DataControl2N0.Data = ReadDdrioDataReg (Host, socket, ch, 0, strobe,         DATACONTROL2N0_0_MCIO_DDRIO_REG);
      DataControl2N1.Data = ReadDdrioDataReg (Host, socket, ch, 0, strobe + MSVx8, DATACONTROL2N0_0_MCIO_DDRIO_REG);
    } else {
      //
      // Read from cache
      //
      DataControl2N0.Data = (*DdrioChannelNvList)[ch].dataControl2[strobe];
      DataControl2N1.Data = (*DdrioChannelNvList)[ch].dataControl2[strobe + MSVx8];
    }

    curVal = (UINT16)DataControl2N0.Bits.imodeeqcode;
    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *value = curVal;
    } else {
      //
      // Write
      //
      // Adjust the current CMD delay value by offset
      if (mode & GSM_WRITE_OFFSET) {
        //
        // Add the offset to the current value
        //
        tmp = curVal + *value;

        //
        // Make sure we do not exeed the limits
        //
        if (tmp > minLimit) {
          piDelay = tmp;
        } else {
          piDelay = minLimit;
        }
        // Ensure we do not exceed maximums
        if (piDelay > maxLimit) {
          piDelay = maxLimit;
        }
      } else {
        //
        // Write absolute value
        //
        piDelay = *value;
      }

      DataControl2N0.Bits.imodeeqcode = piDelay;
      DataControl2N1.Bits.imodeeqcode = piDelay;

      if (piDelay != 0) {
        //
        // Enable Imode registers per byte
        //
        DataControl2N0.Bits.imodeenable = 1;
        DataControl2N1.Bits.imodeenable = 1;
      } else {
        DataControl2N0.Bits.imodeenable = 0;
        DataControl2N1.Bits.imodeenable = 0;
      }

      //
      // Write it back if the current data does not equal the cache value or if in force write mode
      //
      if ((DataControl2N0.Data != (*DdrioChannelNvList)[ch].dataControl2[strobe]) || (mode & GSM_FORCE_WRITE)) {
        WriteDdrioDataReg (Host, socket, ch, 0, strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG, DataControl2N0.Data);
      }
      if ((DataControl2N1.Data != (*DdrioChannelNvList)[ch].dataControl2[strobe + MSVx8]) || (mode & GSM_FORCE_WRITE)) {
        WriteDdrioDataReg (Host, socket, ch, 0, strobe + MSVx8, DATACONTROL2N0_0_MCIO_DDRIO_REG, DataControl2N1.Data);
      }
    } // if read only

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
      (*DdrioChannelNvList)[ch].dataControl2[strobe]         = DataControl2N0.Data;
      (*DdrioChannelNvList)[ch].dataControl2[strobe + MSVx8] = DataControl2N1.Data;
    }
  } // strobe loop
#endif
  return MRC_STATUS_SUCCESS;
} // GetSetImode

/**

  Get/Set Tx Rise Fall Slew Rate

  @param[in]     Host    - Pointer to sysHost
  @param[in]     Socket  - Socket number
  @param[in]     Ch      - Channel number (0-based)
  @param[in]     Dimm    - DIMM number (0-based)
  @param[in]     Rank    - Rank number (0-based)
  @param[in]     Strobe  - Strobe number (0-based)
  @param[in]     Bit     - Bit number
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param[in,out] Value   - Data to program

  @retval MRC_STATUS_STROBE_NOT_PRESENT if Strobe is out of range
  @retval MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS

GetSetTxDqScomp (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      UINT8     Bit,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  DDRD_N0_COMP_LA1_MCIO_DDRIO_STRUCT    DdrDataCompLa1;
  DDRIO_CHANNEL_NVDATA                  (*DdrioChannelNvList) [MAX_CH];

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  if ((Mode & GSM_READ_CSR) != 0) {
     DdrDataCompLa1.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_COMP_LA1_MCIO_DDRIO_REG));
  } else {
     //
     // Read from cache.
     //
     DdrDataCompLa1.Data = ((*DdrioChannelNvList)[Ch]).DdrDataCompLa1[Strobe];
  }

  if ((Mode & GSM_READ_ONLY) == GSM_READ_ONLY) {

    *Value = (INT16)DdrDataCompLa1.Bits.scomp;
    return MRC_STATUS_SUCCESS;
  }

  if ((Mode & GSM_FORCE_WRITE) == GSM_FORCE_WRITE) {

    DdrDataCompLa1.Bits.scomp = *Value;

    MemWritePciCfgEp(Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_COMP_LA1_MCIO_DDRIO_REG), DdrDataCompLa1.Data);

    DoCompUpdate (Host, Socket, GetMCID (Host, Socket, Ch));

    //
    // Update cache.
    //
    ((*DdrioChannelNvList) [Ch]).DdrDataCompLa1[Strobe] = DdrDataCompLa1.Data;
  }

  return MRC_STATUS_SUCCESS;


}

/**

  Get/Set Tx Rise Fall Slew Rate

  @param[in]     Host    - Pointer to sysHost
  @param[in]     Socket  - Socket number
  @param[in]     Ch      - Channel number (0-based)
  @param[in]     Dimm    - DIMM number (0-based)
  @param[in]     Rank    - Rank number (0-based)
  @param[in]     Strobe  - Strobe number (0-based)
  @param[in]     Bit     - Bit number
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param[in,out] Value   - Data to program

  @retval MRC_STATUS_STROBE_NOT_PRESENT if Strobe is out of range
  @retval MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS
GetSetTxRiseFallSlewRate (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      UINT8     Bit,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  DDRD_N0_COMP_OFFSET_MCIO_DDRIO_STRUCT DdrDataCompOffset;
  DDRIO_CHANNEL_NVDATA                  (*DdrioChannelNvList) [MAX_CH];
  UINT8                                 MaxStrobe;
  UINT8                                 MSVx4 = GetMaxStrobeValid (Host);

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  if (Strobe >= MAX_STROBE && Strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
                   "The Strobe value is out of range!\n");
    RC_FATAL_ERROR (FALSE, ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (Strobe == ALL_STROBES) {
    if ((Mode & GSM_READ_ONLY) == GSM_READ_ONLY) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
                     "Parameter ALL_STROBES can not be suppported for GSM_READ_ONLY mode!\n");
      RC_FATAL_ERROR (FALSE, ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
      return MRC_STATUS_STROBE_NOT_PRESENT;
    }
    Strobe    = 0;
    MaxStrobe = MSVx4;
  } else {
    MaxStrobe = Strobe + 1;
  }

  //
  // Array bounds check
  //
  MemCheckIndex (Strobe, MSVx4);

  for (; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }

    if ((Mode & GSM_READ_CSR) != 0) {
      DdrDataCompOffset.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_COMP_OFFSET_MCIO_DDRIO_REG));
    } else {
      //
      // Read from cache.
      //
      DdrDataCompOffset.Data = ((*DdrioChannelNvList)[Ch]).DdrDataCompOffset[Strobe];
    }

    if ((Mode & GSM_READ_ONLY) == GSM_READ_ONLY) {
      if (Group == TxDqRiseSlewRate) {
        *Value = (INT16)DdrDataCompOffset.Bits.scomp_up_adj;
      } else {
        //
        // TxDqFallSlewRate
        //
        *Value = (INT16)DdrDataCompOffset.Bits.scomp_dn_adj;
      }
    }

    if ((Mode & GSM_FORCE_WRITE) == GSM_FORCE_WRITE) {
      if (Group == TxDqRiseSlewRate) {
        DdrDataCompOffset.Bits.scomp_up_adj = *Value;
      } else {
        //
        // TxDqFallSlewRate
        //
        DdrDataCompOffset.Bits.scomp_dn_adj = *Value;
      }

      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_COMP_OFFSET_MCIO_DDRIO_REG), DdrDataCompOffset.Data);

      //
      // Update cache.
      //
      ((*DdrioChannelNvList) [Ch]).DdrDataCompOffset[Strobe] = DdrDataCompOffset.Data;
    }
  } // Strobe

  if ((Mode & GSM_FORCE_WRITE) == GSM_FORCE_WRITE) {
    DoCompUpdate (Host, Socket, GetMCID (Host, Socket, Ch));
  }

  return MRC_STATUS_SUCCESS;
} // GetSetTxRiseFallSlewRate


STATIC
MRC_STATUS
GetSetCTLE (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
/*++

Routine Description:

  Get CTLE

Arguments:

  Host    - Pointer to sysHost
  socket  - Socket number
  ch      - Channel number (0-based)
  dimm    - DIMM number (0-based)
  strobe  - Strobe number (0-based)
  bit     - Bit number
  level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  group   - CSM_GT - Parameter to program
  mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  value   - Data to program

Returns:

  IO delay

--*/
{
#if 0 //SPR_TODO_DDRIO. To enable.
  UINT8                             maxStrobe;
  UINT16                            curVal = 0;
  UINT16                            piDelay;
  INT16                             tmp;
  UINT16                            maxLimit = 3;
  UINT16                            minLimit = 0;
  DDRIO_CHANNEL_NVDATA              (*DdrioChannelNvList)[MAX_CH];
  DATACONTROL3N0_0_MCIO_DDRIO_STRUCT dataControl3;
  DATACONTROL3N0_0_MCIO_DDRIO_STRUCT dataControl3n1;
  UINT8 MSVx8 = GetMaxStrobeValid(Host) / 2;

  DdrioChannelNvList = GetDdrioChannelNvList(Host, socket);

  if (strobe == ALL_STROBES) {
    strobe    = 0;
    maxStrobe = MSVx8;
  } else {
    maxStrobe = strobe + 1;
    if (maxStrobe > MSVx8) {
      return MRC_STATUS_STROBE_NOT_PRESENT;
    }
  }

  //
  // Array bounds check
  //
  MemCheckIndex (strobe, MSVx8);

  for (; strobe < maxStrobe; strobe++) {
    if (IsStrobeNotValid(Host, strobe)) {
      continue;
    }

    if(mode & GSM_READ_CSR) {
      //
      // Read from PCI config space
      //
      dataControl3.Data = MemReadPciCfgEp (socket, ch, UpdateIoRegisterCh10nm (Host, strobe, DATACONTROL3N0_0_MCIO_DDRIO_REG));
      dataControl3n1.Data = MemReadPciCfgEp (socket, ch, UpdateIoRegisterCh10nm (Host, (strobe + MSVx8), DATACONTROL3N0_0_MCIO_DDRIO_REG));
    } else {
      //
      // Read from cache
      //
      dataControl3.Data = (*DdrioChannelNvList)[ch].dataControl3[strobe];
      dataControl3n1.Data = (*DdrioChannelNvList)[ch].dataControl3[(strobe + MSVx8)];
    } // DDR_LEVEL

    switch (group) {

    case RxCtleC:
      curVal = (UINT16)dataControl3.Bits.ddrcrctlecapen;
      break;

    case RxCtleR:
      curVal = (UINT16)dataControl3.Bits.ddrcrctleresen;
      break;

    default:
      break;
    };

    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *value = curVal;
    } else {
      //
      // Write
      //
      // Adjust the current CMD delay value by offset
      if (mode & GSM_WRITE_OFFSET) {
        //
        // Add the offset to the current value
        //
        tmp = curVal + *value;

        //
        // Make sure we do not exeed the limits
        //
        if (tmp > minLimit) {
          piDelay = tmp;
        } else {
          piDelay = minLimit;
        }
        // Ensure we do not exceed maximums
        if (piDelay > maxLimit) {
          piDelay = maxLimit;
        }
      } else {
        //
        // Write absolute value
        //
        piDelay = *value;
      }

      switch (group) {

      case RxCtleC:
        dataControl3.Bits.ddrcrctlecapen = piDelay;
        dataControl3n1.Bits.ddrcrctlecapen = piDelay;
        break;

      case RxCtleR:
        dataControl3.Bits.ddrcrctleresen = piDelay;
        dataControl3n1.Bits.ddrcrctleresen = piDelay;
        break;

      default:
        break;
      };

      {
        //
        // Write it back if the current data does not equal the cache value or if in force write mode
        //
        if ((dataControl3.Data != (*DdrioChannelNvList)[ch].dataControl3[strobe]) || (mode & GSM_FORCE_WRITE)) {
          MemWritePciCfgEp (socket, ch, UpdateIoRegisterCh10nm (Host, strobe, DATACONTROL3N0_0_MCIO_DDRIO_REG), dataControl3.Data);
          MemWritePciCfgEp (socket, ch, UpdateIoRegisterCh10nm (Host, (strobe + MSVx8), DATACONTROL3N0_0_MCIO_DDRIO_REG), dataControl3n1.Data);
        }
      }
    } // if read only

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
      (*DdrioChannelNvList)[ch].dataControl3[strobe] = dataControl3.Data;
      (*DdrioChannelNvList)[ch].dataControl3[(strobe + MSVx8)] = dataControl3n1.Data;
    }
  } // strobe loop
#endif
  return MRC_STATUS_SUCCESS;
} // GetSetCTLE

/**
  Converts DDRCRCOMPCTL0.DqOdtPupVref value to ODT in ohms (or vice versa)

  @param[in] Value            Value to convert.
  @param[in] Code             Type of operation CODE_ODT or DECODE_ODT.

  @return                     Encoded or decoded ODT value.
**/
UINT16
CodeDeCodeRxOdt (
  IN  UINT16 Value,
  IN  UINT8  Code
  )
{
  UINT16 ConvertOdt         = 0;
  UINT16 TargetDq           = 0;
  UINT16 TotalSegments      = 2;
  UINT16 ExternalResistor   = 100;
  UINT16 VrefSteps          = 256;
  UINT16 ScalingFactor      = 10000; // Because GroundOffset is 0.014, ScalingFactor must be >= 1000  (also, mind the magnitude of ScalingFactor to avoid integer overflow)
  UINT16 ScaledGroundOffset = 140;   // 140 is (0.014 * ScalingFactor) and assumes ScalingFactor == 10,000  (if ScalingFactor changes then ScaledGroundOffset must change accordingly)
  UINT16 ScaledVcc          = 12000; // 12,000 is (1.2 * ScalingFactor) and assumes ScalingFactor == 10,000  (if ScalingFactor changes then ScaledVcc must change accordingly)
  UINT16 RoundUp            = 0;
  UINT32 Numerator          = 0;
  UINT32 Denominator        = 0;
  UINT32 ScaledVrefCode     = 0;
  UINT32 PartialResult      = 0;

  if (CODE_ODT == Code) {
    // get DqOdtPupVref value and convert it to ODT value
    ScaledVrefCode = ScalingFactor * Value;
    PartialResult = (UINT32)(ScaledVrefCode / VrefSteps);
    Numerator = (UINT32)(((PartialResult * (ScaledVcc - (2 * ScaledGroundOffset))) / ScalingFactor) + ScaledGroundOffset);
    Denominator = (UINT32)(2 * ((ScalingFactor * Numerator) / ScaledVcc));
    PartialResult = (UINT32)((ScalingFactor * ExternalResistor) / Denominator);
    TargetDq = (UINT16)((PartialResult - ExternalResistor));
    RoundUp = (UINT16)((TargetDq % TotalSegments) / (TotalSegments / 2)); // RoundUp compensates for integer division truncation  (we don't use ScalingFactor here because 2/2 == 1)
    ConvertOdt = (UINT16)((TargetDq / TotalSegments) + RoundUp);
  } else {
    // get ODT value and convert it to DqOdtPupVref value
    TargetDq = TotalSegments * Value; // Target ohm per segment is (3 * X)
    Numerator = (UINT32)(((ScaledVcc * ExternalResistor) / (2 * (TargetDq + ExternalResistor))) - ScaledGroundOffset);
    Denominator = (UINT32)(ScaledVcc - (2 * ScaledGroundOffset));
    ScaledVrefCode = (UINT32)(VrefSteps * ((ScalingFactor * Numerator) / Denominator));
    RoundUp = (UINT16)((ScaledVrefCode % ScalingFactor ) / (ScalingFactor / 2)); // Modulo idiom, to compensate for integer division "round down" (truncation)
    ConvertOdt  = (UINT16)((ScaledVrefCode / ScalingFactor) + RoundUp);
  } // if Code

  return ConvertOdt;
} // CodeDeCodeRxOdt

/**
  Converts DDRCRCOMPCTL0.DqDrvPupVref value to MC RON in ohms (or vice versa)

  @param[in] Value            Value to convert.
  @param[in] Code             Type of operation CODE_ODT or DECODE_ODT.

  @return                     Encoded or decoded MC RON value.
**/
UINT16
CodeDeCodeTxRon (
  IN  UINT16 Value,
  IN  UINT8  Code
  )
{
  UINT16 ConvertRon         = 0;
  UINT16 TargetDq           = 0;
  UINT16 TotalSegments      = 3;
  UINT16 ExternalResistor   = 90;
  UINT16 VrefSteps          = 256;
  UINT16 ScalingFactor      = 10000; // Because GroundOffset is 0.014, ScalingFactor must be >= 1000  (also, mind the magnitude of ScalingFactor to avoid integer overflow)
  UINT16 ScaledGroundOffset = 140;   // 140 is (0.014 * ScalingFactor) and assumes ScalingFactor == 10,000  (if ScalingFactor changes then ScaledGroundOffset must change accordingly)
  UINT16 ScaledVcc          = 12000; // 12,000 is (1.2 * ScalingFactor) and assumes ScalingFactor == 10,000  (if ScalingFactor changes then ScaledVcc must change accordingly)
  UINT16 RoundUp            = 0;
  UINT32 Numerator          = 0;
  UINT32 Denominator        = 0;
  UINT32 ScaledVrefCode     = 0;
  UINT32 PartialResult      = 0;

  if (CODE_ODT == Code) {
    // get DqDrvPupVref value and convert it to TxRon value
    ScaledVrefCode = ScalingFactor * Value;
    PartialResult = (UINT32)(ScaledVrefCode / VrefSteps);
    Numerator = (UINT32)(((PartialResult * (ScaledVcc - (2 * ScaledGroundOffset))) / ScalingFactor) + ScaledGroundOffset);
    Denominator = (UINT32)(2 * ((ScalingFactor * Numerator) / ScaledVcc));
    PartialResult = (UINT32)((ScalingFactor * ExternalResistor) / Denominator);
    TargetDq = (UINT16)((PartialResult - ExternalResistor));
    RoundUp = (UINT16)((ScalingFactor * (TargetDq % TotalSegments)) / ((ScalingFactor * TotalSegments) / 2)); // RoundUp compensates for integer division truncation  (further, we use ScalingFactor here because 3/2 truncates to 1)
    ConvertRon = (UINT16)((TargetDq / TotalSegments) + RoundUp);
  } else {
    // get TxRon value and convert it to DqDrvPupVref value
    TargetDq = TotalSegments * Value; // Target ohm per segment is (3 * X)
    Numerator = (UINT32)(((ScaledVcc * ExternalResistor) / (2 * (TargetDq + ExternalResistor))) - ScaledGroundOffset);
    Denominator = (UINT32)(ScaledVcc - (2 * ScaledGroundOffset));
    ScaledVrefCode = (UINT32)(VrefSteps * ((ScalingFactor * Numerator) / Denominator));
    RoundUp = (UINT16)((ScaledVrefCode % ScalingFactor ) / (ScalingFactor / 2)); // Modulo idiom, to compensate for integer division "round down" (truncation)
    ConvertRon  = (UINT16)((ScaledVrefCode / ScalingFactor) + RoundUp);
  } // if Code

  return ConvertRon;
} // CodeDeCodeTxRon

STATIC
MRC_STATUS
GetSetCPUODT (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
/*++

Routine Description:

  Get ODT

Arguments:

  Host    - Pointer to sysHost
  socket  - Socket number
  ch      - Channel number (0-based)
  dimm    - DIMM number (0-based)
  rank    - Rank number (0-based)
  strobe  - Strobe number (0-based)
  bit     - Bit number
  level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  group   - CSM_GT - Parameter to program
  mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  value   - Data to program

Returns:

  IO delay

--*/
{
#if 0 //SPR_TODO_DDRIO. To enable.
  UINT16                              curVal = 0;
  UINT16                              tempVal = 0;
  UINT16                              piDelay;
  INT16                               tmp;
  UINT16                              maxLimit = 3;
  UINT16                              minLimit = 0;
  UINT8                               imc;
  DDRCRCOMPCTL0_MCIO_DDRIOEXT_STRUCT  ddrCRCompCtl0;
  DDRIO_IMC_NVDATA      (*DdrioImcNvList)[MAX_IMC];

  DdrioImcNvList     = GetDdrioImcNvList (Host, socket);
  imc = GetMCID(Host, socket, ch);

  //if (ch & 1) return SUCCESS;

  //
  // Get minimum and maximum value
  //
  // GetDataGroupLimits(Host, level, group, &minLimit, &maxLimit, &usDelay);

  if(mode & GSM_READ_CSR) {

    //
    // Read from PCI config space
    //
    ddrCRCompCtl0.Data = MemReadPciCfgMC (socket, imc, DDRCRCOMPCTL0_MCIO_DDRIOEXT_REG);

  } else {
    // Read from cache
    //
    ddrCRCompCtl0.Data = (*DdrioImcNvList)[imc].ddrCRCompCtl0;

  } // DDR_LEVEL

  switch (group) {
    case RxOdt:
      tempVal = (UINT16)ddrCRCompCtl0.Bits.dqodtpupvref;
      curVal  = ConvertOdtValue (Host, group, tempVal, CODE_ODT, Host->nvram.mem.dimmTypePresent);
      break;
    case TxRon:
      tempVal = (UINT16)ddrCRCompCtl0.Bits.dqdrvpupvref;
      curVal  = ConvertOdtValue (Host, group, tempVal, CODE_ODT, Host->nvram.mem.dimmTypePresent);
      break;
    default:
      break;
  }

  //
  // Check if read only
  //
  if (mode & GSM_READ_ONLY) {
    //
    // Save the value read into the output parameter
    //
    *value = curVal;
  } else {
    //
    // Write
    //
    // Adjust the current CMD delay value by offset
    if (mode & GSM_WRITE_OFFSET) {
      //
      // Add the offset to the current value
      //
      tmp = tempVal + *value;

      //
      // Make sure we do not exeed the limits
      //
      if (tmp > minLimit) {
        piDelay = tmp;
      } else {
        piDelay = minLimit;
      }
      // Ensure we do not exceed maximums
      if (piDelay > maxLimit) {
        piDelay = maxLimit;
      }
    } else {
      //
      // Write absolute value
      //
      tempVal = *value;
      piDelay = ConvertOdtValue (Host, group, tempVal, DECODE_ODT, Host->nvram.mem.dimmTypePresent);
    }

    switch (group) {
      case RxOdt:
        ddrCRCompCtl0.Bits.dqodtpupvref = piDelay;
        break;
      case TxRon:
        ddrCRCompCtl0.Bits.dqdrvpupvref = piDelay;
        break;
      default:
        break;
      } {

      //
      // Write it back if the current data does not equal the cache value or if in force write mode
      //
      // no cache
      if ((ddrCRCompCtl0.Data != (*DdrioImcNvList)[imc].ddrCRCompCtl0) || (mode & GSM_FORCE_WRITE)) {
        MemWritePciCfgMC (socket, imc, DDRCRCOMPCTL0_MCIO_DDRIOEXT_REG, ddrCRCompCtl0.Data);
        DoComp (Host,socket);
      }
    }
  } // if read only

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
    {
      // no cache
      (*DdrioImcNvList)[imc].ddrCRCompCtl0 = ddrCRCompCtl0.Data;
    }
  }
#endif // if 0
  return MRC_STATUS_SUCCESS;
} // GetSetCPUODT

/**

  Procedure to write the DDR4 DIMM Vref setting to the given target

  @param[in] Host   - Pointer to sysHost struct
  @param[in] Socket - Processor socket number
  @param[in] Ch     - Channel number within the socket
  @param[in] Dimm   - DIMM number within the channel
  @param[in] Rank   - Rank number within the DIMM
  @param[in] Strobe - Strobe number for the rank
  @param[in] Level  - The MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in] Vref   - Vref value to write

  @retval MRC_STATUS_SUCCESS if the Vref circuit is present and can be written
  @retval MRC_STATUS_FAILURE if the Vref circuit is not present or cannot be written

**/
MRC_STATUS
MemWriteDimmVrefDdr4 (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  IN  UINT8     Strobe,
  IN  MRC_LT    Level,
  IN  UINT8     Vref
  )
{
#ifndef DDR5_SUPPORT
  UINT8                 vrefRange;
  UINT8                 dram;
  struct channelNvram   (*channelNvList) [MAX_CH];
  DDRIO_CHANNEL_NVDATA  (*DdrioChannelNvList) [MAX_CH];
  struct rankDevice     (*rankStruct) [MAX_RANK_DIMM];
  struct dimmNvram      (*dimmNvList) [MAX_DIMM];
  UINT8                 SubCh = 0;
#ifdef LRDIMM_SUPPORT
  struct ddrRank        (*rankList) [MAX_RANK_DIMM];
  UINT8                 HostSideBufferTxVref[MAX_STROBE / 2];
  UINT8                 BufferIndex;
#endif
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  SYS_SETUP             *Setup;

  Setup = GetSysSetupPointer ();

  channelNvList = GetChannelNvList (Host, Socket);
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  rankStruct = GetRankStruct (Host, Socket, Ch, Dimm);

  dimmNvList = GetDimmNvList (Host, Socket, Ch);

  //check to make sure that strobe is within the range of strobes (used as array index in function)
  if (Strobe < 0 || Strobe >= MSVx4) {
    return MRC_STATUS_FAILURE;
  }

  if ((*dimmNvList) [Dimm].x4Present) {
    if (Strobe >= MSVx8) {
      SubCh = SUB_CH_B;
    } else {
      SubCh = SUB_CH_A;
    }
  } else {

    if (Strobe >= (MSVx8 /2)) {
      SubCh = SUB_CH_B;
    } else {
      SubCh = SUB_CH_A;
    }
  }
#ifdef LRDIMM_SUPPORT
  if (Level == LrbufLevel) {
    // for LR buffer side, only support RANGE1 (TODO: see if RANGE 2 is needed)
    vrefRange = Vref;
    // Enforce upper limit per JEDEC spec
    if (vrefRange > 50) {
      vrefRange = 50;
    }
  } else
#endif //LRDIMM
  {
    //
    // Select the Vref range
    //
    vrefRange = GetVrefRange (Host, Vref);
  }
#ifdef LRDIMM_SUPPORT
  if (IsLrdimmPresent (Socket, Ch, Dimm)) {

    if (Level == DdrLevel) {

      rankList = GetRankNvList (Host, Socket, Ch, Dimm);
      //
      // Get per strobe range settings into an array
      //
      // (take Vref range select bit 6 and put it in the LRDIMM buffer register bit 1)
      //NOTE: the backside DRAM range is written to range 1...ok for now, but may need to change later
      for (BufferIndex = 0; BufferIndex < MSVx8; BufferIndex++) {
        HostSideBufferTxVref[BufferIndex] = ((((GetVrefRange (Host, (*DdrioChannelNvList) [Ch].txVref[ (*rankList) [Rank].rankIndex][BufferIndex])) & BIT6) >> 5) | BIT0);
      }
      HostSideBufferTxVref[Strobe % MSVx8] = ((vrefRange & BIT6) >> 5) | BIT0;
      if ((Setup->mem.optionsExt & PDA_EN) &&
          (Host->nvram.mem.socket[Socket].TrainingStepDone.TxVref == 1)) {
        //NOTE: the backside DRAM range is written to range 1...ok for now, but may need to change later
        WriteLrbufPBA (Host, Socket, Ch, Dimm, Rank, HostSideBufferTxVref, LRDIMM_F6, LRDIMM_BC4x, (Strobe % MSVx8));
      } else {
        WriteLrbuf (Host, Socket, Ch, Dimm, 0, HostSideBufferTxVref[0], LRDIMM_F6, LRDIMM_BC4x);
      }
      //
      // Get per strobe txVref settings into an array
      //
      for (BufferIndex = 0; BufferIndex < MSVx8; BufferIndex++) {
        HostSideBufferTxVref[BufferIndex] = ((GetVrefRange (Host, (*DdrioChannelNvList) [Ch].txVref[ (*rankList) [Rank].rankIndex][BufferIndex])) & 0x3F);
      }
      HostSideBufferTxVref[Strobe % MSVx8] = vrefRange & 0x3F;
      //
      // program LRDIMM Host Interface VREF Control Word
      //
      if ((Setup->mem.optionsExt & PDA_EN) &&
          (Host->nvram.mem.socket[Socket].TrainingStepDone.TxVref == 1)) {
        WriteLrbufPBA (Host, Socket, Ch, Dimm, Rank, HostSideBufferTxVref, LRDIMM_F5, LRDIMM_BC5x, (Strobe % MSVx8));
      } else {
        WriteLrbuf (Host, Socket, Ch, Dimm, 0, HostSideBufferTxVref[0], LRDIMM_F5, LRDIMM_BC5x);
      }
    } else {
      //LEVEL = LrbufLevel

      // Update MR6 value with new vref setting mode value
      (*rankStruct) [Rank].MR6[Strobe] &= ~0x7F;
      (*rankStruct) [Rank].MR6[Strobe] |= (vrefRange & 0x7F);

      MRSCmdStacking (Host, Socket, Ch, Dimm, LOAD_MRS);
      // Write the new DRAM Vref value and enable VrefDQ training mode
      WriteMRS (Host, Socket, Ch, Dimm, Rank, (*rankStruct) [Rank].MR6[Strobe] | BIT7, BANK6);
      WriteMRS (Host, Socket, Ch, Dimm, Rank, (*rankStruct) [Rank].MR6[Strobe] | BIT7, BANK6);
      WriteMRS (Host, Socket, Ch, Dimm, Rank, (*rankStruct) [Rank].MR6[Strobe], BANK6);
      MRSCmdStacking (Host, Socket, Ch, Dimm, EXECUTE_MRS);

    }
    WriteBC (Host, Socket, Ch, Dimm, 0, 0, LRDIMM_F0, LRDIMM_BC7x);
    (*channelNvList) [Ch].dimmList[Dimm].rcLrFunc = 0;

  } else
#endif
  {
    // Update MR6 value with new vref setting mode value
    (*rankStruct) [Rank].MR6[Strobe] &= ~0x7F;
    (*rankStruct) [Rank].MR6[Strobe] |= (vrefRange & 0x7F);

    // Write the new DRAM Vref value and enable VrefDQ training mode
    if ((Setup->mem.optionsExt & PDA_EN) &&
        (Host->nvram.mem.socket[Socket].TrainingStepDone.TxVref == 1)) {
      if ((*dimmNvList) [Dimm].x4Present) {
        if (Strobe >= MSVx8) {
          dram = ((Strobe - MSVx8) * 2) + 1;
        } else {
          dram = Strobe * 2;
        }
      } else {
        dram = Strobe % MSVx8;
      }
      WriteMRSPDA (Host, Socket, Ch, Dimm, Rank, dram, (*rankStruct) [Rank].MR6[Strobe], BANK6);
    } else {
      WriteMR6 (Host, Socket, Ch, Dimm, Rank, (*rankStruct) [Rank].MR6[Strobe], BANK6);
    }
  }
  return MRC_STATUS_SUCCESS;
#else // !DDR5_SUPPORT
  return MRC_STATUS_FAILURE;
#endif // !DDR5_SUPPORT
} // MemWriteDimmVrefDdr4

/**

  Procedure to write the DDR5 DRAM Vref setting

  @param[in] Host       - Pointer to sysHost struct
  @param[in] Socket     - Processor socket number
  @param[in] Ch         - Channel number within the socket
  @param[in] SubChMask  - Mask of SubChannels within the channel
  @param[in] Dimm       - DIMM number within the channel
  @param[in] Rank       - Rank number within the DIMM
  @param[in] Strobe     - Strobe number for the subchannel
  @param[in] Vref       - Vref value to write

  @retval MRC_STATUS_SUCCESS if the Vref circuit is present and can be written
  @retval MRC_STATUS_FAILURE if the Vref circuit is not present or cannot be written

**/
MRC_STATUS
MemWriteDramVrefDdr5 (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubChMask,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  IN  UINT8     Strobe,
  IN  UINT8     Vref
  )
{
  UINT8                         MaxChDdr;
  UINT8                         NumRanks;
  UINT8                         Dram;
  SYS_SETUP                     *Setup;
  UINT8                         MSVx4;
  UINT8                         MaxStrobeSubCh;
  UINT8                         MSVx4SubCh;
  UINT8                         MSVx8SubCh;
  DDR5_MODE_REGISTER_10_STRUCT  Mr10;

  MaxChDdr    = GetMaxChDdr ();
  NumRanks    = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);
  MSVx4       = GetMaxStrobeValid (Host);
  MSVx4SubCh  = GetSubChMaxStrobeValid (Host);
  MSVx8SubCh  = MSVx4SubCh / 2;

  if (IsX4Dimm (Socket, Ch, Dimm)) {
    MaxStrobeSubCh = MSVx4SubCh;
  } else {
    MaxStrobeSubCh = MSVx8SubCh;
  }

  if ((Socket >= MAX_SOCKET)  ||
      (Ch >= MaxChDdr)        ||
      (Dimm >= MAX_DIMM)      ||
      (Rank >= NumRanks)      ||
      ((Strobe >= MaxStrobeSubCh) && (Strobe != ALL_STROBES)))
  {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_120);
    return MRC_STATUS_FAILURE;
  }

  Setup = GetSysSetupPointer ();

  //
  // Enforce lower limit per JEDEC spec
  //
  if (Vref > DDR5_MAX_VREF_RANGE) {
    Vref = DDR5_MAX_VREF_RANGE;
  }
  Mr10.Data = 0;
  Mr10.Bits.vrefdq_cal_value = Vref;

  //
  // Send MRW for either PDA or broadcast to all devices
  //
  if ((Setup->mem.optionsExt & PDA_EN) &&
      (Host->nvram.mem.socket[Socket].TrainingStepDone.TxVref == 1) &&
      (Strobe != ALL_STROBES))
  {
    Dram = Strobe % MaxStrobeSubCh;
    DramModeRegWritePda (Socket, Ch, SubChMask, Dimm, Rank, Dram, DDR5_MODE_REGISTER_10_REG, Mr10.Data);
  } else {
    DramModeRegWrite (Socket, Ch, SubChMask, Dimm, Rank, DDR5_MODE_REGISTER_10_REG, Mr10.Data);
  }

  return MRC_STATUS_SUCCESS;
} // MemWriteDramVrefDdr5

/**

  Procedure to write the DRAM Vref setting

  @param[in] Host       - Pointer to sysHost struct
  @param[in] Socket     - Processor socket number
  @param[in] Ch         - Channel number within the socket
  @param[in] SubChMask  - Mask of SubChannels within the channel
  @param[in] Dimm       - DIMM number within the channel
  @param[in] Rank       - Rank number within the DIMM
  @param[in] Strobe     - Strobe number for the subchannel
  @param[in] Vref       - Vref value to write

  @retval MRC_STATUS_SUCCESS if the Vref circuit is present and can be written
  @retval MRC_STATUS_FAILURE if the Vref circuit is not present or cannot be written

**/
MRC_STATUS
MemWriteDramVref (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubChMask,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  IN  UINT8     Strobe,
  IN  UINT8     Vref
  )
{
  if (IsDdr5Present (Host, Socket)) {
    return MemWriteDramVrefDdr5 (Host, Socket, Ch, SubChMask, Dimm, Rank, Strobe, Vref);
  } else {
    return MemWriteDimmVrefDdr4 (Host, Socket, Ch, Dimm, Rank, Strobe, DdrLevel, Vref);
  }
} // MemWriteDramVref

/**

  Procedure to write the Lrdimm Buffer Host Interface Internal VrefDq control word

  @param[in] Host       - Pointer to sysHost struct
  @param[in] Socket     - Processor socket number
  @param[in] Ch         - Channel number within the socket
  @param[in] SubChMask  - Mask of SubChannels within the channel
  @param[in] Dimm       - DIMM number within the channel
  @param[in] Rank       - Rank number within the DIMM
  @param[in] Strobe     - Strobe number for the rank
  @param[in] Bit        - Bit number within the strobe
  @param[in] LrbuffVref - Pointer to LRBUFF_HOST_VREF containing Vref values to write

  @retval MRC_STATUS_SUCCESS if the Vref is written
  @retval MRC_STATUS_FAILURE if the Vref is not written

**/
MRC_STATUS
MemWriteLrdimmBufferHostVrefDq (
  IN  PSYSHOST            Host,
  IN  UINT8               Socket,
  IN  UINT8               Ch,
  IN  UINT8               SubChMask,
  IN  UINT8               Dimm,
  IN  UINT8               Rank,
  IN  UINT8               Strobe,
  IN  UINT8               Bit,
  IN  LRBUFF_HOST_VREF    *LrbuffVref
  )
{
  UINT8                               Dq;
  UINT8                               DqStart;
  UINT8                               DqEnd;
  BOOLEAN                             AllDqVrefSame;
  UINT8                               CommonVref;
  UINT8                               VrefDq;
  UINT8                               Address;
  UINT8                               BufferSelect;
  UINT8                               MSVx8SubCh;
  DDR5_DATA_BUFFER_PG2_RWE0_STRUCT    DataBufferPg2RwE0;
  DDR5_DATA_BUFFER_RW81_STRUCT        DataBufferRw81;

  MSVx8SubCh  = GetSubChMaxStrobeValid (Host) / 2;

  if (((Strobe >= MSVx8SubCh) && (Strobe != ALL_STROBES)) ||
      ((Bit >= MAX_BITS_IN_BYTE) && (Bit != ALL_BITS)))
  {
    //
    // MemWriteLrdimmBufferHostVrefDq input parameter out of range
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_119);
    return MRC_STATUS_FAILURE;
  }

  AllDqVrefSame = TRUE;
  CommonVref = LrbuffVref->VrefDq[0];
  if (Bit == ALL_BITS) {
    //
    // If all of the VrefDq values are the same, use the VrefDq broadcast bit so
    // that only 1 VrefDq needs to be programmed.
    // Otherwise, each individual VrefDq will need to be programmed.
    //
    DqStart = 0;
    DqEnd   = 1;
    for (Dq = 1; Dq < MAX_BITS_IN_BYTE; Dq++) {
      if (LrbuffVref->VrefDq[Dq] != CommonVref) {
        AllDqVrefSame = FALSE;
        DqEnd         = MAX_BITS_IN_BYTE;
        break;
      }
    }
  } else {
    DqStart = Bit;
    DqEnd   = Bit + 1;
  }

  //
  // Select all buffers for ALL_STROBES or just a single buffer for PBA
  //
  BufferSelect = Strobe;
  if (Strobe == ALL_STROBES) {
    BufferSelect = ALL_DATABUFFERS;
  }
  DbControlWordWrite (Host, Socket, Ch, SubChMask, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, BufferSelect);
  //
  // Set VrefDq broadcast bit if all VrefDq values are the same
  //
  DataBufferRw81.Data = 0;
  if (AllDqVrefSame) {
    DataBufferRw81.Bits.vrefdq_broadcast = 1;
  }
  DbControlWordWrite (Host, Socket, Ch, SubChMask, Dimm, Rank, DDR5_DATA_BUFFER_RW81_REG, DataBufferRw81.Data);
  //
  // Select DB page to program
  //
  DbControlWordWrite (Host, Socket, Ch, SubChMask, Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGE2);
  for (Dq = DqStart; Dq < DqEnd; Dq++) {
    //
    // Enforce lower limit per JEDEC spec (min Vref setting corresponds to the largest value the register supports)
    //
    VrefDq = LrbuffVref->VrefDq[Dq];
    if (VrefDq > DDR5_MAX_VREF_RANGE) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, Dq,
                      "LRDIMM VrefDq Out of range!! Requested = 0x%x, Max Allowed = 0x%x\n", VrefDq, DDR5_MAX_VREF_RANGE);
      VrefDq = DDR5_MAX_VREF_RANGE;
    }
    //
    // Set VrefDq for each DQ
    //
    DataBufferPg2RwE0.Data = 0;
    DataBufferPg2RwE0.Bits.vref_dq = VrefDq;
    Address = DDR5_DATA_BUFFER_PG2_RWE0_REG + Dq;
    DbControlWordWrite (Host, Socket, Ch, SubChMask, Dimm, Rank, Address, DataBufferPg2RwE0.Data);
  } // Dq
  //
  // Return DB page to 0
  //
  DbControlWordWrite (Host, Socket, Ch, SubChMask, Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGE0);

  return MRC_STATUS_SUCCESS;
} // MemWriteLrdimmBufferHostVrefDq

/**

  Procedure to write the Lrdimm Buffer Dram Interface Internal VrefDq control word
  It is the DB PG2RWF1:F0 registers.

  @param[in] Host       - Pointer to sysHost struct
  @param[in] Socket     - Processor socket number
  @param[in] Ch         - Channel number within the socket
  @param[in] SubChMask  - Mask of SubChannels within the channel
  @param[in] Dimm       - DIMM number within the channel
  @param[in] Rank       - Rank number within the DIMM
  @param[in] Strobe     - Strobe number for the rank inside the Subchannel
  @param[in] Vref       - Vref value to write

  @retval MRC_STATUS_SUCCESS if the Vref is written
  @retval MRC_STATUS_FAILURE if the Vref is not written

**/
MRC_STATUS
MemWriteLrdimmBufferDramVrefDq (
  IN  PSYSHOST            Host,
  IN  UINT8               Socket,
  IN  UINT8               Ch,
  IN  UINT8               SubChMask,
  IN  UINT8               Dimm,
  IN  UINT8               Rank,
  IN  UINT8               Strobe,
  IN  UINT8               Vref
  )
{
  UINT8                               Address;
  UINT8                               BufferSelect;
  UINT8                               MSVx4SubCh;
  DDR5_DATA_BUFFER_PG2_RWF0_STRUCT    DataBufferPg2RwF0;

  MSVx4SubCh  = GetSubChMaxStrobeValid (Host);

  if ((Strobe >= MSVx4SubCh) && (Strobe != ALL_STROBES)) {
    //
    // MemWriteLrdimmBufferDramVrefDq input parameter out of range
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_119);
    return MRC_STATUS_FAILURE;
  }

  //
  // Select all buffers for ALL_STROBES or just a single buffer for PBA
  //

  //
  // DB0: Strobe 0 and 5.  DB1: Strobe 1 and 6 ...... DB4: Strobe 4 and 9
  //
  BufferSelect = Strobe % (MSVx4SubCh / 2);
  if (Strobe == ALL_STROBES) {
    BufferSelect = ALL_DATABUFFERS;
  }
  DbControlWordWrite (Host, Socket, Ch, SubChMask, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, BufferSelect);

  //
  // Select DB page to program
  //
  DbControlWordWrite (Host, Socket, Ch, SubChMask, Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGE2);

  //
  // Set VrefDq for each DQ
  //
  DataBufferPg2RwF0.Data = 0;
  DataBufferPg2RwF0.Bits.Vref_MDQ = Vref;

  if (Strobe != ALL_STROBES) {
    //
    // Strobe 0 - 4 lower nibble; Strobe 5 - 9 upper nibble 
    //
    if (Strobe < MSVx4SubCh / 2) {
      Address = DDR5_DATA_BUFFER_PG2_RWF0_REG;
    } else {
      Address = DDR5_DATA_BUFFER_PG2_RWF1_REG;
    }
    DbControlWordWrite (Host, Socket, Ch, SubChMask, Dimm, Rank, Address, DataBufferPg2RwF0.Data);
  } else {
    DbControlWordWrite (Host, Socket, Ch, SubChMask, Dimm, Rank, DDR5_DATA_BUFFER_PG2_RWF0_REG, DataBufferPg2RwF0.Data);
    DbControlWordWrite (Host, Socket, Ch, SubChMask, Dimm, Rank, DDR5_DATA_BUFFER_PG2_RWF1_REG, DataBufferPg2RwF0.Data);
  }

  return MRC_STATUS_SUCCESS;
} // MemWriteLrdimmBufferDramVrefDq

/**

  Converts the DDR4 Vref setting to be written into MR6
  This function does not support DDR5

  @param Host - Pointer to sysHost struct
  @param vref - vref value to write

**/
UINT8
GetVrefRange (
  PSYSHOST  Host,
  UINT8     vref
  )
{
  UINT8 vrefRange;
  //
  // Select the Vref range
  //
  if (vref < DDR4_VREF_RANGE1_OFFSET) {

    // Use range 2 and enforce lower limit
    if (vref > DDR4_VREF_RANGE2_OFFSET) {
      vrefRange = (vref - DDR4_VREF_RANGE2_OFFSET) | BIT6;
    } else {
      vrefRange = BIT6;
    }

  } else {

    // Use range 1
    vrefRange = vref - DDR4_VREF_RANGE1_OFFSET;

    // Enforce upper limit per JEDEC spec
    if (vrefRange > 50) {
      vrefRange = 50;
    }
  }
  return vrefRange;
} // GetVrefRange

/**

  Converts the Vref setting to be written into MR6

  @param Host    - Pointer to sysHost struct
  @param socket  - processor number

**/
VOID
UpdateSafeTxVref (
  PSYSHOST  Host,
  UINT8     socket,
  MRC_LT    level
  )
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  UINT8               StrobeIndex;
  UINT8               maxStrobe;
  UINT8               hiVref;
  UINT8               loVref;
  UINT8               safelevel;
  INT16               vref;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8               MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList(Host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      // skip NVMDIMM backside
      if (((*dimmNvList)[dimm].DcpmmPresent) && (level == LrbufLevel)) {
        continue;
      }
      // skip if lrdimm level but no LRDIMM found
      if ((level == LrbufLevel) && (IsLrdimmPresent (socket, ch, dimm) == 0)) {
        continue;
      }
      maxStrobe = GetMaxStrobe(Host, socket, ch, dimm, TxVref, level);

      rankList = GetRankNvList(Host, socket, ch, dimm);

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (IsLrdimmPresent (socket, ch, dimm) && (rank > 0)) {
          continue;
        }
        loVref = 0xFF;
        hiVref = 0;

        for (StrobeIndex = 0; StrobeIndex < maxStrobe; StrobeIndex++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if ((!Host->nvram.mem.eccEn) && IsEccStrobe (Host, StrobeIndex)){
            continue;
          }

          //
          // Convert from linear strobe index to "logical"/"IO" strobe
          // in the x8 case, the linear strobes 5-9 are mapped to the 10-14
          //
          strobe = StrobeIndex % (maxStrobe / 2) + (StrobeIndex / (maxStrobe / 2)) * (MAX_STROBE_DDR5 / 2);

          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, ALL_BITS, level, TxVref, GSM_READ_ONLY, &vref);

          if (loVref > vref) {
            loVref = (UINT8)vref;
          }
          if (hiVref < vref) {
            hiVref = (UINT8)vref;
          }
        } // StrobeIndex loop

        safelevel = (UINT8)((loVref + hiVref) / 2);

        if (level ==  DdrLevel) {
          (*channelNvList)[ch].txVrefSafe[(*rankList)[rank].rankIndex] = safelevel;
        }

        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
              "txVrefSafe = 0x%x\n", safelevel);
      } // rank loop
    } // dimm loop
  } // ch loop
} // UpdateSafeTxVref

/**

  Get the limits for the group or level requested

  @param Host      - Pointer to sysHost
  @param level     - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group     - CSM_GT - Parameter to program
  @param minLimit  - Minimum delay value allowed
  @param maxLimit  - Maximum absolute value allowed
  @param usdelay   - Time in usec required for new setting to take effect

  @retval MRC_STATUS

**/
MRC_STATUS
GetDataGroupLimits (
  PSYSHOST  Host,
  MRC_LT    level,
  MRC_GT    group,
  UINT16    *minLimit,
  UINT16    *maxLimit,
  UINT16    *usDelay
  )
{
  MRC_STATUS  status = MRC_STATUS_SUCCESS;
  UINT8       Socket;

  Socket = Host->var.mem.currentSocket;

  if (level == DdrLevel) {
    switch (group) {
    case RxEq:
      *minLimit = 0;
      *maxLimit = 7;
      *usDelay  = 0;
      break;
    case TxEq:
      *minLimit = 0;
      *maxLimit = 9;
      *usDelay  = 0;
      break;
    case TxEqTap2:
      *minLimit = 0;
      *maxLimit = 6;
      *usDelay  = 0;
      break;
    case RecEnDelay:
      *minLimit = 0;
      *maxLimit = 1023; // 16UIs
      *usDelay  = 0;
      break;
    case WrLvlDelay:
    case TxDqsDelay:
    case TxDqDelay:
    case TxDqBitDelay:
      *minLimit = 0;
      *maxLimit = 1023; // 16UIs
      *usDelay  = 0;
      break;
    case RxDqsDelay:
    case RxDqsPDelay:
    case RxDqsNDelay:
      *minLimit = 0;
      *maxLimit = 175;
      *usDelay  = 0;
      break;
    case RxSummer:
      *minLimit = 0;
      *maxLimit = 60;
      *usDelay  = 0;
      break;
    case RxStrobeInvert:
      *minLimit = 0;
      *maxLimit = 1;
      *usDelay  = 0;
      break;
    case DcaTco:
    case TcoDqDqs:
      *minLimit = MIN_TCO_RANGE1;
      *maxLimit = MAX_TCO_RANGE2;
      *usDelay  = 0;
      break;
    case LinearCntlTcoDqDqs:
    case LinearCntlDcaTco:
      *minLimit = 0;
      *maxLimit = 63;
      *usDelay  = 0;
      break;
    case DcaDelay:
      *minLimit = 0;
      *maxLimit = 511;
      break;
    case DcsDelay:
      *minLimit = 0;
      *maxLimit = 511;
      break;
    case RxDqsBitDelay:
    case RxDqsPBitDelay:
    case RxDqsNBitDelay:
      *minLimit = 0;
      *maxLimit = 22;
      *usDelay  = 0;
      break;
      //case RxDqDelay:
    case RxVref:
      *minLimit = 0;
      *maxLimit = 127;
      *usDelay  = 1;
      break;
    case TxVref:
      if (IsDdr5Present (Host, Socket)) {
        *maxLimit = DDR5_MAX_VREF_RANGE;
        *minLimit = 0;
      } else {
        *maxLimit = DDR4_VREF_RANGE1_OFFSET + 50;
        *minLimit = DDR4_VREF_RANGE2_OFFSET;
      }
      *usDelay  = 1;
      break;
    case RxDqDelay:
      *minLimit = 0;
      *maxLimit = 7;
      *usDelay  = 0;
      break;
    case TxRiseFallSlewRate:
    case TxFallSlewRate:
      *minLimit = 0;
      *maxLimit = 31;
      *usDelay  = 0;
      break;
    case PxcDelta:
    case DdjcDelta:
      *minLimit = 0;
      *maxLimit = 3;
      *usDelay = 0;
      break;
    case RxDfeCoefficient:
      *minLimit = 0;
      *maxLimit = 14;
      *usDelay = 0;
      break;
    case RxDfeCoefficientTap1:
      *minLimit = 0;
      *maxLimit = 31;
      *usDelay = 0;
      break;
    case RxDfeCoefficientTap2:
      *minLimit = 0;
      *maxLimit = 15;
      *usDelay = 0;
      break;
    case RxDfeCoefficientTap3:
      *minLimit = 0;
      *maxLimit = 15;
      *usDelay = 0;
      break;
    case RxDfeCoefficientTap4:
      *minLimit = 0;
      *maxLimit = 15;
      *usDelay = 0;
      break;


    case RxDfeCoefficientTap1PerBit:
      *minLimit = 0;
      *maxLimit = 7;
      *usDelay = 0;
      break;
    case RxDfeCoefficientTap2PerBit:
      *minLimit = 0;
      *maxLimit = 7;
      *usDelay = 0;
      break;
    case RxDfeCoefficientTap3PerBit:
      *minLimit = 0;
      *maxLimit = 7;
      *usDelay = 0;
      break;
    case RxDfeCoefficientTap4PerBit:
      *minLimit = 0;
      *maxLimit = 7;
      *usDelay = 0;
      break;

    case TxDqPerChDelayOffset:
    case RxDqsPerChDelayOffset:
      *minLimit = (UINT16)(-128);
      *maxLimit = 127;
      *usDelay = 0;
      break;
    case RxVrefPerChDelayOffset:
      *minLimit = (UINT16)(-64);
      *maxLimit = 63;
      *usDelay = 0;
      break;
    case TxDqPieOffset:
    case TxDqsPieOffset:
      *minLimit = 0;
      *maxLimit = 127;
      *usDelay  = 0;
      break;
    case RxSamplerPerChDelayOffset:
    case RxSampler:
      *minLimit = (UINT16) (-32);
      *maxLimit = 31;
      *usDelay = 0;
      break;
    case RxSamplerEvenOdd:
    case RxSamplerEven:
    case RxSamplerOdd:
      *minLimit = 0;
      *maxLimit = 62;
      *usDelay = 0;
      break;
    default:
      status = MRC_STATUS_GROUP_NOT_SUPPORTED;
      break;
    } // switch group
#ifdef LRDIMM_SUPPORT
  } else if (level == LrbufLevel) {
    switch (group) {
    case RxDqsPDelay:
    case RxDqsNDelay:
    case RxDqsDelay:
      *minLimit = 0;
      *maxLimit = 30;
      *usDelay  = 0;
      break;
    case TxDqDelay:
      // PG[1:0]RWE6/E7[7:0]
      *minLimit = 0;
      *maxLimit = 255;
      *usDelay  = 0;
      break;
    case RxVref:
      *maxLimit = DDR5_MAX_VREF_RANGE; // PG2RWF[1:0]
      *minLimit = 0;
      *usDelay  = 1;
      break;

    case TxVref:
      *maxLimit = DDR5_MAX_VREF_RANGE;
      *minLimit = 0;
      *usDelay  = 1;
      break;
    default:
      status = MRC_STATUS_GROUP_NOT_SUPPORTED;
      break;
    } // switch group
#endif
  } else {
    status = MRC_STATUS_LEVEL_NOT_SUPPORTED;
  }

  return status;
} // GetDataGroupLimits

/**

  Get/Set function to modify the RxSummer at a bit or strobe level

  @param[in]      Host        Pointer to SysHost structure
  @param[in]      Socket      Current socket under test (0-based)
  @param[in]      Ch          Current channel under test (0-based)
  @param[in]      Dimm        Current dimm under test (0-based)
  @param[in]      Rank        Current rank under test (0-based)
  @param[in]      Strobe      Current strobe under test (0-based || ALL_STROBES)
  @param[in]      Bit         Current bit under test (0-based || ALL_BITS)
  @param[in]      Level       Current level (not applicable)
  @param[in]      Group       Current group under test (RxSummer)
  @param[in]      Mode        Register/Cache access mode (GSM_READ_CSR, GSM_READ_ONLY, GSM_UPDATE_CACHE, GSM_FORCE_WRITE)
  @param[in,out]  Value       Pointer to data that will be used to retrieve or store

  @retval         MRC_STATUS_STROBE_NOT_PRESENT     Invalid strobe parameter
  @retval         MRC_STATUS_INVALID_PARAMETER      Invalid bit parameter
  @retval         MRC_STATUS_INVALID_PARAMETER      Invalid Group parameter
  @retval         MRC_STATUS_SUCCESS                Otherwise

**/
MRC_STATUS
GetSetRxSummer (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     Rank,
  UINT8     Strobe,
  UINT8     Bit,
  MRC_LT    Level,
  MRC_GT    Group,
  UINT8     Mode,
  INT16     *Value
  )
{
  UINT8                             MaxStrobe;
  UINT8                             BitIndex;
  UINT8                             BitStart;
  UINT8                             BitStop;
  UINT8                             SwizzleBit;
  UINT16                            BitValue[BITS_PER_NIBBLE];
  UINT16                            MinBitValue;
  UINT16                            MaxLimit = 127;
  UINT16                            MinLimit = 0;
  UINT16                            UsDelay = 1;
  DDRIO_CHANNEL_NVDATA              (*DdrioChannelNvList) [MAX_CH];
  DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_STRUCT    DdrdN0SaOffsetRank0;

  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  DdrdN0SaOffsetRank0.Data = 0;

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits (Host, Level, Group, &MinLimit, &MaxLimit, &UsDelay);

  if (Bit == ALL_BITS) {
    BitStart    = 0;
    BitStop     = BITS_PER_NIBBLE;
  } else {
    BitStart    = Bit;
    BitStop     = Bit + 1;
  }

  if (Strobe >= MAX_STROBE && Strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (
      SDBG_MAX,
      Socket,
      Ch,
      Dimm,
      NO_SUBCH,
      Rank,
      Strobe,
      NO_BIT,
      "The strobe value is out of range!\n"
      );
    RC_FATAL_ERROR ((Strobe < MAX_STROBE || Strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (Strobe == ALL_STROBES) {
    Strobe = 0;
    MaxStrobe = MSVx4;
  } else {
    MaxStrobe = Strobe + 1;
  }

  for (; Strobe < MaxStrobe; Strobe++) {

    Strobe = Strobe % MSVx4;

    if (Mode & GSM_READ_CSR) {

      //
      // Read from PCI config space
      //
      DdrdN0SaOffsetRank0.Data = ReadDdrioDataRegCh (Host, Socket, Ch, Strobe, DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_REG);
    } else {
      //
      // Read from cache
      //

      DdrdN0SaOffsetRank0.Data = (*DdrioChannelNvList) [Ch].rxOffset[Rank][Strobe];
    }

    BitValue[0] = (UINT16) DdrdN0SaOffsetRank0.Bits.sa_offset0;
    BitValue[1] = (UINT16) DdrdN0SaOffsetRank0.Bits.sa_offset1;
    BitValue[2] = (UINT16) DdrdN0SaOffsetRank0.Bits.sa_offset2;
    BitValue[3] = (UINT16) DdrdN0SaOffsetRank0.Bits.sa_offset3;
    //
    // MinBitValue is only used for GSM_READ_ONLY, so we can return the minimum value across the nibble
    //
    MinBitValue = MaxLimit;

    for (BitIndex = BitStart; BitIndex < BitStop; BitIndex++) {

      SwizzleBit = GetDqSwizzle (Ch, Strobe, BitIndex, Level, Group);

      //
      // Check if read only
      //
      if (Mode & GSM_READ_ONLY) {

        if (BitValue[SwizzleBit] < MinBitValue) {
          MinBitValue = BitValue[SwizzleBit];
        }

        *Value = MinBitValue;

      } else {
        //
        // Write
        //
        if (Mode & GSM_WRITE_OFFSET) {
          //
          // Add the offset to the current value
          //
          BitValue[SwizzleBit] = BitValue[SwizzleBit] + *Value;
          //
          // Make sure we do not exceed the limits
          //
          if (BitValue[SwizzleBit] > MaxLimit) {
            RcDebugPrintWithDevice (
              SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, BitIndex,
              "RxSummer is Out of Range!! BitValue[%d] = 0x%x, MaxLimit = 0x%x\n",
              SwizzleBit,
              BitValue[SwizzleBit],
              MaxLimit
              );
            BitValue[SwizzleBit] = MaxLimit;
          }
          if (BitValue[SwizzleBit] < MinLimit) {
            RcDebugPrintWithDevice (
              SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, BitIndex,
              "RxSummer is Out of Range!! BitValue[%d] = 0x%x, MinLimit = 0x%x\n",
              SwizzleBit,
              BitValue[SwizzleBit],
              MinLimit
              );
            BitValue[SwizzleBit] = MinLimit;
          }

        } else {
          BitValue[SwizzleBit] = *Value;
        }
      }
    } // BitIndex loop

    DdrdN0SaOffsetRank0.Bits.sa_offset0 = BitValue[0];
    DdrdN0SaOffsetRank0.Bits.sa_offset1 = BitValue[1];
    DdrdN0SaOffsetRank0.Bits.sa_offset2 = BitValue[2];
    DdrdN0SaOffsetRank0.Bits.sa_offset3 = BitValue[3];
    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if ((DdrdN0SaOffsetRank0.Data != (*DdrioChannelNvList) [Ch].rxOffset[Rank][Strobe]) || (Mode & GSM_FORCE_WRITE)) {
      WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_REG, DdrdN0SaOffsetRank0.Data);
    }
    //
    // Save to cache
    //
    if (Mode & GSM_UPDATE_CACHE) {
      (*DdrioChannelNvList) [Ch].rxOffset[Rank][Strobe] = DdrdN0SaOffsetRank0.Data;
    }

  } // Strobe loop

  return MRC_STATUS_SUCCESS;
} // GetSetRxSummer

/**
  Cache all RoundTrip registers for a channel

  @param  Host      - Pointer to sysHost structure
  @param  socket    - Socket number
  @param  channel   - Channel number

  @retval EFI_SUCCESS   Round trip registers were successfully cached
  @retval !EFI_SUCCESS  Round trip registers were not successfully cached

**/
EFI_STATUS
CacheRoundTrip (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch
  )
{
  EFI_STATUS                                Status;
  UINT8                                     SubCh;
  UINT8                                     MemoryController;
  UINT8                                     MemoryControllerChannel;
  UINT8                                     NumChPerMc;
  DDRINTF_RT_LAT0_CH_0_MCIO_DDRIO_STRUCT    RoundTrip0;
  UINT32                                    RoundTripRegOffset;
  struct channelNvram                       (*channelNvList)[MAX_CH];
  struct RankCh                             (*RankPerCh)[MAX_RANK_CH];
  UINT8                                     MaxChDdr;
#ifndef DDR5_SUPPORT
  EFI_STATUS                                TemporaryStatus;
  DDRINTF_RT_LAT1_CH_0_MCIO_DDRIO_STRUCT    RoundTrip1;
#endif


  MaxChDdr = GetMaxChDdr ();
  NumChPerMc = GetNumChannelPerMc ();

  if ((Ch >= MaxChDdr) ||
      ((MAX_RANK_CH * SUB_CH) < DIMM_ROUND_TRIP_RANKS))
  {
    // An invalid preconditions for this function was observed:
    // 1. The channel index is too high.
    // 2. The number of channel ranks is too low.
    // Either would lead to out-of-bounds accesses.
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_87);
    return FAILURE;
  }

  channelNvList = GetChannelNvList (Host, Socket);
  RankPerCh = &(*channelNvList)[Ch].rankPerCh;
  MemoryController = Ch / NumChPerMc;
  MemoryControllerChannel = Ch % NumChPerMc;
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    Status = GetRoundTripRegOffset (Socket, Ch, MemoryControllerChannel, SubCh, DIMM0, &RoundTripRegOffset);
    RoundTrip0.Data = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
    (*RankPerCh)[0].Roundtrip = (UINT8)RoundTrip0.Bits.rt_lat0;
    (*RankPerCh)[1].Roundtrip = (UINT8)RoundTrip0.Bits.rt_lat1;
    (*RankPerCh)[2].Roundtrip = (UINT8)RoundTrip0.Bits.rt_lat2;
    (*RankPerCh)[3].Roundtrip = (UINT8)RoundTrip0.Bits.rt_lat3;

#ifndef DDR5_SUPPORT
    //
    // DDR4 DIMM1: Need to cache DDRINTF_RT_LAT1_XXX registers for ranks on DDR4 DIMM1.
    // DDR5 DIMM1: DDR5 DIMM1 uses the same DDRINTF_RT_LAT0_XXX registers as DIMM0, which has
    //             been cached in above code. No need to cache DDRINTF_RT_LAT1_XXX registers for DDR5.
    //
    TemporaryStatus = GetRoundTripRegOffset (Socket, Ch, MemoryControllerChannel, SubCh, DIMM1, &RoundTripRegOffset);
    if (!EFI_ERROR (Status)) {
      Status = TemporaryStatus;
    }
    RoundTrip1.Data = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
    (*RankPerCh)[4].Roundtrip = (UINT8)RoundTrip1.Bits.rt_lat4;
    (*RankPerCh)[5].Roundtrip = (UINT8)RoundTrip1.Bits.rt_lat5;
    (*RankPerCh)[6].Roundtrip = (UINT8)RoundTrip1.Bits.rt_lat6;
    (*RankPerCh)[7].Roundtrip = (UINT8)RoundTrip1.Bits.rt_lat7;
#endif
  }

  return Status;
}

/**
  Cache IoLatency registers for a channel

  @param  Host      - Pointer to sysHost structure
  @param  socket    - Socket number
  @param  channel   - Channel number

  @retval SUCCESS if IoLatency cached
          FAILURE otherwise
**/
UINT32
CacheIoLatency (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch
  )
{
  UINT8                                     SubCh;
  UINT8                                     RankIndex;
  DDRINTF_IO_LAT0_CH_0_MCIO_DDRIO_STRUCT    IoLatency0;
  UINT32                                    IoLatency0Ch[] = { DDRINTF_IO_LAT0_CH_0_MCIO_DDRIO_REG,
                                                               DDRINTF_IO_LAT0_CH_1_MCIO_DDRIO_REG,
                                                               DDRINTF_IO_LAT0_CH_2_MCIO_DDRIO_REG,
                                                               DDRINTF_IO_LAT0_CH_3_MCIO_DDRIO_REG
                                                             };
  UINT32                                    IoLatency1Ch[] = { DDRINTF_IO_LAT1_CH_0_MCIO_DDRIO_REG,
                                                               DDRINTF_IO_LAT1_CH_1_MCIO_DDRIO_REG,
                                                               DDRINTF_IO_LAT1_CH_2_MCIO_DDRIO_REG,
                                                               DDRINTF_IO_LAT1_CH_3_MCIO_DDRIO_REG
                                                             };
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  struct RankCh                             (*RankPerCh)[MAX_RANK_CH];
  UINT8                                     Index;
  UINT8                                     MaxChDdr;
  UINT8                                     SwappedSubCh;

  MaxChDdr = GetMaxChDdr ();
  if (Ch >= MaxChDdr) {
    // invalid ch specified
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_86);
    return FAILURE;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);
  RankPerCh = &(*ChannelNvList)[Ch].rankPerCh;

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

    //
    // Update Sub Ch just in case lane reversal is enabled on current Ch.
    //
    SwappedSubCh = UpdateDdrIoCcRegisterSubCh (Host, Socket, Ch, SubCh);

    Index = (Ch * SUB_CH + SwappedSubCh) % MAX_MC_SCH;

    //
    // IoLatency0 is stored in NVRAM in per-rank structures
    //
    IoLatency0.Data = MemReadPciCfgEp (Socket, Ch, IoLatency0Ch[Index]);
    for (RankIndex = 0; RankIndex < MAX_RANK_CH; RankIndex++) {
#ifdef DDR5_SUPPORT
      (*RankPerCh)[RankIndex].IOLatency[SubCh] = (UINT8) ((IoLatency0.Data >> (RankIndex * 4)) & 0xF);
#else
      (*RankPerCh)[RankIndex].IOLatency = (UINT8) ((IoLatency0.Data >> (RankIndex * 4)) & 0xF);
#endif // #ifdef DDR5_SUPPORT
    }

    //
    // IoLatency1 is stored in NVRAM in per-channel structures
    //
#ifdef DDR5_SUPPORT
    (*ChannelNvList)[Ch].ioLatency1[SubCh] = MemReadPciCfgEp (Socket, Ch, IoLatency1Ch[Index]);
#else
    (*ChannelNvList)[Ch].ioLatency1 = MemReadPciCfgEp (Socket, Ch, IoLatency1Ch[Index]);
#endif // #ifdef DDR5_SUPPORT
  }

  return SUCCESS;
}

/**

  Cache all DDRIO registers

  @param Host    - Pointer to sysHost
  @param Socket  - Socket number

**/
UINT32
CacheDDRIO (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  UINT8                 Bit;
  UINT8                 Strobe;
  UINT8                 Ch;
  UINT8                 SubCh;
  UINT8                 Imc;
  UINT8                 Dimm;
  UINT8                 Rank;
  UINT16                MinVal;
  UINT16                MaxVal;
  INT16                 ZeroValue;
  DDRIO_IMC_NVDATA      (*DdrioImcNvList)[MAX_IMC];
  struct channelNvram   (*ChannelNvList)[MAX_CH];
  DDRIO_CHANNEL_NVDATA  (*DdrioChannelNvList)[MAX_CH];
  struct dimmNvram      (*DimmNvList)[MAX_DIMM];
  struct ddrRank        (*RankList)[MAX_RANK_DIMM];
  UINT8                 MaxImc;
  UINT8                 MaxChDdr;
  UINT8                 MSVx4;
  UINT32                DdrintfCtl0ChAddress;
  INT16                 BitSkew[BITS_PER_NIBBLE];

  MSVx4 = GetMaxStrobeValid (Host);
  MaxImc = GetMaxImc ();
  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  DdrioImcNvList = GetDdrioImcNvList (Host, Socket);
  ZeroValue = 0;
  MinVal = 0xFF;
  MaxVal = 0x0;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }
    //
    // Any read from a broadcast register is not valid, so initialized the multicast registers to
    // zero and update the cache value, so subsequent reads are done from cache
    //
    GetSetDelayMulticastOffset (Host, Socket, Ch, NO_DIMM, NO_RANK, ALL_STROBES, ALL_BITS, DdrLevel, TxDqPerChDelayOffset, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &ZeroValue);
    GetSetDelayMulticastOffset (Host, Socket, Ch, NO_DIMM, NO_RANK, ALL_STROBES, ALL_BITS, DdrLevel, RxDqsPerChDelayOffset, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &ZeroValue);
    GetSetDelayMulticastOffset (Host, Socket, Ch, NO_DIMM, NO_RANK, ALL_STROBES, ALL_BITS, DdrLevel, RxVrefPerChDelayOffset, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &ZeroValue);
    GetSetDelayMulticastOffset (Host, Socket, Ch, NO_DIMM, NO_RANK, ALL_STROBES, ALL_BITS, DdrLevel, RxSamplerPerChDelayOffset, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &ZeroValue);
    //
    // Loop for each dimm and each rank
    //
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      RankList = GetRankNvList (Host, Socket, Ch, Dimm);
      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        for (Strobe = 0; Strobe < MSVx4; Strobe++) {

          //
          // Read TxDqsDelay into the cache a nibble at a time
          // Read TxDqBitDelay into the cache a nibble at a time using TxDqDelay
          // Read RecEnDelay, RxDqsNDelay, RxDqsPDelay, RxDqsDelay, RxDqDelay, rx_strobe_inv into the cache a nibble at a time using RxDqDelay
          // Read RxDqsBitDelay, RxDqsPBitDelay, RxDqsNBitDelay, rxdqsp_sel, rxdqsn_sel, sa_offset[3:0], dfe_en into the cache a bit at a time using RxDqsBitDelay
          //
          GetSetTxDelay (Host, Socket, Ch, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, &ZeroValue);
          GetSetTxDelay (Host, Socket, Ch, Dimm, Rank, Strobe, 0, DdrLevel, TxDqDelay, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, &ZeroValue);
          GetSetRxDelay (Host, Socket, Ch, Dimm, Rank, Strobe, 0, DdrLevel, RxDqDelay, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, &ZeroValue);
          for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
            GetSetRxDelayBit (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, DdrLevel, RxDqsBitDelay, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, &ZeroValue);

            //
            // Init the per bit cached value
            //
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, Bit, DdrLevel, RxDfeCoefficientTap1PerBit, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, BitSkew);
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, Bit, DdrLevel, RxDfeCoefficientTap2PerBit, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, BitSkew);
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, Bit, DdrLevel, RxDfeCoefficientTap3PerBit, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, BitSkew);
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, Bit, DdrLevel, RxDfeCoefficientTap4PerBit, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, BitSkew);

          }


          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RxDfeCoefficientTap1, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, &ZeroValue);
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RxDfeCoefficientTap2, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, &ZeroValue);
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RxDfeCoefficientTap3, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, &ZeroValue);
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RxDfeCoefficientTap4, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, &ZeroValue);
        } // Strobe loop
      } // Rank loop
    } // Dimm loop

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      //
      // Per-channel, per-strobe register that don't have their own GetSet function
      //
      (*DdrioChannelNvList)[Ch].DdrDataTxCtl0[Strobe]       = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TX_CTL0_MCIO_DDRIO_REG));
      (*DdrioChannelNvList)[Ch].DdrDataRxCtl0[Strobe]       = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_RX_CTL0_MCIO_DDRIO_REG));
      (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl0[Strobe]     = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL0_MCIO_DDRIO_REG));
      (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe]     = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG));
      (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl2[Strobe]     = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG));
      (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl3[Strobe]     = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL3_MCIO_DDRIO_REG));
      (*DdrioChannelNvList)[Ch].DdrdN0PxcCtl0[Strobe]       = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_PXC_CTL0_MCIO_DDRIO_REG));
      (*DdrioChannelNvList)[Ch].DdrdN0PirefOffset0[Strobe]  = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_PIREF_OFFSET0_MCIO_DDRIO_REG));

    } // Strobe loop

    //
    // Read all CMD/CTL registers into cache
    //
    GetSetCmdGroupDelay (Host, Socket, Ch, CmdCtlAll, GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &ZeroValue, &MinVal, &MaxVal);

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      //
      // Per-channel, per-subchannel registers
      //
      (*DdrioChannelNvList)[Ch].DdrCcTxCtl0[SubCh] = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_TX_CTL0_MCIO_DDRIOEXT_REG);
      if (GetDdrintfCtl0RegAddress (Socket, Ch, SubCh, &DdrintfCtl0ChAddress) == MRC_STATUS_SUCCESS) {
        (*DdrioChannelNvList)[Ch].SubChannel[SubCh].DdrintfCtl0Ch = MemReadPciCfgEp (Socket, Ch, DdrintfCtl0ChAddress);
      }
    } // SubCh

    //
    // Per-channel registers
    //
    (*DdrioChannelNvList)[Ch].DdrClkTxCtl0 = MemReadPciCfgEp (Socket, Ch, DDRCLK_TX_CTL0_MCIO_DDRIOEXT_REG);
  } // Ch

  CacheSenseAmpSettings (Host, Socket);

  if (FeaturePcdGet(PcdCteBuild) == TRUE) {
    return SUCCESS;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
#if 0 // SPR_TODO BIOS HSD-ES 1504716216
    (*DdrioChannelNvList)[Ch].DdrClkRxVrefCtl  = MemReadPciCfgEp (Socket, Ch, DDRCLK_RXVREF_CTL_MCIO_DDRIOEXT_REG);
#else // SPR_TODO BIOS HSD-ES 1504716216
    //
    // Temporarily use ddrCRClkControls but switch to DdrClkRxVrefCtl
    // when we can do that without growing the size of SysHost.
    //
    (*DdrioChannelNvList)[Ch].ddrCRClkControls  = MemReadPciCfgEp (Socket, Ch, DDRCLK_RXVREF_CTL_MCIO_DDRIOEXT_REG);
#endif // SPR_TODO BIOS HSD-ES 1504716216


#if 0 // SPR_TODO BIOS HSD-ES 1504716216
    (*DdrioChannelNvList)[Ch].ddrCRClkTraining       = MemReadPciCfgEp (Socket, Ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG);
    (*DdrioChannelNvList)[Ch].ddrCRCmdTrainingCmdN   = MemReadPciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG);
    (*DdrioChannelNvList)[Ch].ddrCRCmdTrainingCmdS   = MemReadPciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG);
    (*DdrioChannelNvList)[Ch].ddrCRCtlTraining       = MemReadPciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CTL_MCIO_DDRIOEXT_REG);
    (*DdrioChannelNvList)[Ch].ddrCRClkTraining       = MemReadPciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CKE_MCIO_DDRIOEXT_REG);
    (*DdrioChannelNvList)[Ch].ddrCRCmdControls3CmdN  = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_REG);
    (*DdrioChannelNvList)[Ch].ddrCRCmdControls3Cke   = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CKE_MCIO_DDRIOEXT_REG);
    (*DdrioChannelNvList)[Ch].ddrCRCmdControls3CmdS  = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CMDS_MCIO_DDRIOEXT_REG);
    (*DdrioChannelNvList)[Ch].ddrCRCmdControls3Ctl   = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CTL_MCIO_DDRIOEXT_REG);

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      (*DdrioChannelNvList)[Ch].dataControl3[Strobe]     = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Strobe, DATACONTROL3N0_0_MCIO_DDRIO_REG));
      (*DdrioChannelNvList)[Ch].RxDfeControl[Strobe]     = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Strobe, DDRCRDATARXDFECONTROL_MCIO_DDRIO_REG));
      (*DdrioChannelNvList)[Ch].RxDfeCoeff[Strobe]       = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Strobe, DDRCRDATARXDFECOEFF_MCIO_DDRIO_REG));
      (*DdrioChannelNvList)[Ch].dataOffsetTrain[Strobe]  = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Strobe, DATAOFFSETTRAINN0_0_MCIO_DDRIO_REG));
    } // strobe loop
#endif // SPR_TODO BIOS HSD-ES 1504716216

#if 0 // SPR_TODO BIOS HSD-ES 1504716216
    //
    // Cache registers that are not common between support chips
    //
    CacheRoundTrip (Host, Socket, Ch);
    CacheIoLatency (Host, Socket, Ch);

    //
    // Loop for each dimm and each rank
    //
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      RankList = GetRankNvList (Host, Socket, Ch, Dimm);
      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        GetSetCtlGroupDelay (Host, Socket, Ch, (*RankList)[Rank].ctlIndex + CtlGrp0, GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &TempVal, &MinVal, &MaxVal);
        GetSetClkDelay (Host, Socket, Ch, (*RankList)[Rank].ckIndex, GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &TempVal);

        for (Strobe = 0; Strobe < MSVx4; Strobe++) {
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }

          (*DdrioChannelNvList)[Ch].rxOffset[(*RankList)[Rank].rankIndex][Strobe] =
            MemReadPciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, (*RankList)[Rank].logicalRank, Strobe, RXOFFSETN0RANK0_0_MCIO_DDRIO_REG));

          GetSetRxVref (Host, Socket, Ch, Dimm, Rank, Strobe, 0, DdrLevel, RxVref, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, (INT16 *)&ZeroValue);

        } // strobe loop
      } // rank loop
    } // dimm loop
#endif // SPR_TODO BIOS HSD-ES 1504716216
  } // ch loop

  for (Imc = 0; Imc < MaxImc; Imc++) {
    if (Host->var.mem.socket[Socket].imcEnabled[Imc] == 0) {
      continue;
    }

    (*DdrioImcNvList)[Imc].dimmVrefControl1 = MemReadPciCfgMC (Socket, Imc, DDRCOMP_DIMM_VREF_CTL0_MCIO_DDRIOEXT_REG);
    (*DdrioImcNvList)[Imc].ddrCRCompCtl0 = MemReadPciCfgMC (Socket, Imc, DDRCOMP_DQ_VREF_CTL_MCIO_DDRIOEXT_REG);
  }

  return SUCCESS;
} // CacheDDRIO


/**
  Find register address for DDRIO strobe-based register.
  Take into account logical-to-physical strobe swizzling.

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Ch          - Channel number
  @param [in]       Strobe      - Strobe number
  @param [in]       AddressBase - Base address for ch0, rank0, strobe0

  @retval Register address for the swizzled strobe
**/
STATIC
UINT32
GetDdrIoSwizzledAddr (
  IN     PSYSHOST  Host,
  IN     UINT8     Ch,
  IN     UINT8     Strobe,
  IN     UINT32    AddressBase
  )
{
  UINT8   StrobeSwizzled;
  UINT32  SwizzledAddress;
  UINT8   SubChMSVx4;
  UINT8   Socket;

  Socket = Host->var.mem.currentSocket;

  //
  // Step 1: Swizzle logical strobe to physical strobe
  //
  StrobeSwizzled = GetStrobeSwizzle (Host, Ch, Strobe);

  //
  // Step 2: Adjust for register addresses not increasing monotonically with strobe#
  //         AddressBase is assumed to be the address for strobe 0.
  //         Address bits [15:8] increase with the strobe number from 0 to 9 but
  //         reset at strobe 10 to a value that is 20 less than that of strobe 0.
  //

  if (IsDdr5Present (Host, Socket)) {

    SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  } else {

    //
    // Calcuate the strobe number of each sub channel for SPR DDR4.
    // For SPR DDR4, GetMaxStrobeValid actually reports the valid x4 nibbles
    // of the whole channel, which is 18. The other two extra unused x4 nibbles
    // are not reported by GetSubChMaxStrobeValid. The total number of x4 nibbles
    // per channel are 20. The number of x4 nibbles per sub-channel are 10.
    //
    SubChMSVx4 = (GetMaxStrobeValid (Host) + (MAX_STROBE_DDR5 - MAX_STROBE_DDR4)) / SUB_CH_NUMBER;
  }

  if (StrobeSwizzled < SubChMSVx4) {
    //
    // DDRIO sub channel A.
    // Address bits[15:8] increase with strobe # from strobe 0 through 9.
    //
    SwizzledAddress = SUBCH_STROBE_OFFSET (SUB_CH_A, StrobeSwizzled, AddressBase);
  } else {
    //
    // DDRIO sub channel B.
    // Address bits[15:8] increase with strobe # from strobe 10 through 19.
    //
    SwizzledAddress = SUBCH_STROBE_OFFSET (SUB_CH_B, (StrobeSwizzled - SubChMSVx4), AddressBase);
  }
  return SwizzledAddress;
}


/**
  Find register address for DDRIO strobe-based register.
  This function does not take into account logical-to-physical
  strobe swizzling.

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Strobe      - Strobe number
  @param [in]       AddressBase - Base address for ch0, rank0, strobe0

  @retval Register address for the swizzled strobe
**/
UINT32
GetDdrIoAddr (
  IN     PSYSHOST  Host,
  IN     UINT8     Strobe,
  IN     UINT32    AddressBase
  )
{
  UINT32  RegAddress;
  UINT32  SubChMSVx4;
  UINT8   Socket;

  Socket = Host->var.mem.currentSocket;

  if (IsDdr5Present (Host, Socket)) {

    SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  } else {
    //
    // Calcuate the strobe number of each sub channel.
    // For SPR DDR4, GetMaxStrobeValid actually reports the valid x4 nibbles
    // of the whole channel, which is 18. The other two extra unused x4 nibbles
    // are not reported by GetSubChMaxStrobeValid. The total number of x4 nibbles
    // per channel are 20. The number of x4 nibbles per sub-channel are 10.
    //
    SubChMSVx4 = (GetMaxStrobeValid (Host) + (MAX_STROBE_DDR5 - MAX_STROBE_DDR4)) / SUB_CH_NUMBER;
  }

  if (Strobe < SubChMSVx4) {
    //
    // DDRIO sub channel A.
    // Address bits[15:8] increase with strobe # from strobe 0 through 9.
    //
    RegAddress = SUBCH_STROBE_OFFSET (SUB_CH_A, Strobe, AddressBase);
  } else {
    //
    // DDRIO sub channel B.
    // Address bits[15:8] increase with strobe # from strobe 10 through 19.
    //
    RegAddress = SUBCH_STROBE_OFFSET (SUB_CH_B, (Strobe - SubChMSVx4), AddressBase);
  }

  return RegAddress;
}

/**

  This function updates the register function block number and offset

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Ch          - Channel number
  @param [in]       LogRank     - Logical rank number in the DDRIO
  @param [in]       Strobe      - Strobe number
  @param [in]       RegOffset   - Register define

  @retval IO Address

**/
UINT32
UpdateIoRegister10nm (
  IN      PSYSHOST    Host,
  IN      UINT8       Ch,
  IN      UINT8       LogRank,
  IN      UINT8       Strobe,
  IN      UINT32      RegOffset
  )
{
  CSR_OFFSET  CsrReg;
  UINT32      RealOffset;

  //
  // Convert pseudo offset to real offset
  //
  RealOffset = MemPciCfgOffset (0, 0, RegOffset);

  CsrReg.Data = RealOffset;

  //
  // Add in offset for data group. Swizzle
  //
  CsrReg.Bits.offset = GetDdrIoSwizzledAddr (Host, Ch, Strobe, CsrReg.Data);

  //
  // Add in offset for rank
  //
  CsrReg.Bits.offset = CsrReg.Bits.offset + (LogRank * 4);

  return CsrReg.Data;
} // UpdateIoRegister10nm

/**

  This function updates the register function block number and offset

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Ch          - Channel number
  @param [in]       Strobe      - Strobe number
  @param [in]       RegOffset   - Register define

  @retval IO Address

**/
UINT32
UpdateIoRegisterCh10nm (
  IN      PSYSHOST    Host,
  IN      UINT8       Ch,
  IN      UINT8       Strobe,
  IN      UINT32      RegOffset
  )
{
  CSR_OFFSET      CsrReg;
  UINT32          RealOffset;

  //
  // Convert pseudo offset to real offset
  //
  RealOffset = MemPciCfgOffset (0, 0, RegOffset);

  CsrReg.Data = RealOffset;

  //
  // Add in offset for data group
  //
  CsrReg.Bits.offset = GetDdrIoSwizzledAddr (Host, Ch, Strobe, CsrReg.Data);

  return CsrReg.Data;
} // UpdateIoRegisterCh10nm


/**

  Reads per-rank register from DDRIO data FUB instance selected by strobe number

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       LogRank     - Logical rank number in the DDRIO
  @param [in]       Strobe      - Strobe number
  @param [in]       Offset      - Register define

  @retval Register value

**/
UINT32
ReadDdrioDataReg (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    LogRank,
  IN      UINT8    Strobe,
  IN      UINT32   Offset
  )
{
  return MemReadPciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, LogRank, Strobe, Offset));
}

/**

  Reads per-channel register from DDRIO data FUB instance selected by strobe number

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       Strobe      - Strobe number
  @param [in]       Offset      - Register define

  @retval Register value

**/
UINT32
ReadDdrioDataRegCh (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    Strobe,
  IN      UINT32   Offset
  )
{
  return MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, Offset));
}

/**

  Writes per-rank register to DDRIO data FUB instance selected by strobe number

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       LogRank     - Logical rank number in the DDRIO
  @param [in]       Strobe      - Strobe number
  @param [in]       Offset      - Register define
  @param [in]       Data        - Register value

  @retval N/A

**/
VOID
WriteDdrioDataReg (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    LogRank,
  IN      UINT8    Strobe,
  IN      UINT32   Offset,
  IN      UINT32   Data
  )
{
  MemWritePciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, LogRank, Strobe, Offset), Data);
}


/**

  Writes per-channel register to DDRIO data FUB instance selected by strobe number

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       Strobe      - Strobe number
  @param [in]       Offset      - Register define
  @param [in]       Data        - Register value

  @retval N/A

**/
VOID
WriteDdrioDataRegCh (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    Strobe,
  IN      UINT32   Offset,
  IN      UINT32   Data
  )
{
  MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, Offset), Data);
}

/**

  Reads DDRIO CC registers. It takes DDRIO lane reversal into consideration.

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       SubCh       - Sub channel number
  @param [in]       RegOffset   - Register define

  @retval Register value

**/
UINT32
ReadDdrioCcRegister (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    SubCh,
  IN      UINT32   RegOffset
  )
{
  UINT32 Data32;
  UINT8  SubChannel;

  SubChannel = UpdateDdrIoCcRegisterSubCh (Host, Socket, Ch, SubCh);
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET (SubChannel, RegOffset));

  return Data32;
}


/**

  Writes DDRIO CC register. It takes DDRIO lane reversal into consideration.

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       SubCh       - Sub channel number
  @param [in]       Offset      - Register define
  @param [in]       Data        - Register value

  @retval N/A

**/
VOID
WriteDdrioCcRegister (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    SubCh,
  IN      UINT32   RegOffset,
  IN      UINT32   Data
  )
{
  UINT8 SubChannel;

  SubChannel = UpdateDdrIoCcRegisterSubCh (Host, Socket, Ch, SubCh);

  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET (SubChannel, RegOffset), Data);
}

/**

/**

  Set the DDRIO training mode

  @param[in]  Host            - SysHost pointer
  @param[in]  Socket          - Socket number
  @param[in]  Dimm            - DIMM number (Channel-based)
  @param[in]  Rank            - Rank number (DIMM-based)
  @param[in]  Mode            - Training mode to set

  @retval N/A

**/
VOID
SetTrainingMode (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Mode
  )
{
  //
  // Unused for DDR5
  //
} // SetTrainingMode

/**

  Initialize IO Comp settings and issue a comp cycle

  @param Host    - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
STATIC
UINT32
ProgramIOCompValues (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8                             ch;
  UINT8                             SubCh;
  UINT8                             dimm;
  UINT8                             rank;
  UINT8                             logRank;
  UINT8                             rtDelay;
  UINT8                             IoLatency;
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  UINT8                             MaxChDdr;

  if (!(UbiosGenerationOrHsleEnabled ())) {
    return SUCCESS;
  }

  channelNvList = GetChannelNvList(Host, socket);
  MaxChDdr = GetMaxChDdr ();
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList(Host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      rankList = GetRankNvList(Host, socket, ch, dimm);

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        //
        // Get the logical rank #
        //
        logRank = GetLogicalRank (Host, socket, ch, dimm, rank);

        rtDelay = DEFAULT_EMULATION_ROUND_TRIP;
        SetRoundTrip (socket, ch, logRank, rtDelay);

        IoLatency = 0;
        SetIOLatency (socket, ch, logRank, IoLatency);

      } // rank loop
    } // dimm loop

    //
    // per Celeste Brown, for RDIMM +1 to t_cwl_adj.
    //
    if (Host->nvram.mem.dimmTypePresent == RDIMM) {
      SetCwlAdj (Host, socket, ch, 0, 1);
    }

    for(SubCh = 0; SubCh < SUB_CH; SubCh++) {
      SetIoLatencyIoComp (Host, socket, ch, SubCh, MRC_ROUND_TRIP_IO_COMPENSATION_EMU);
    }
  } // ch loop

  return SUCCESS;
} // ProgramIOCompValues

/**

  Issue a comp update

  @param[in] Host   - Pointer to sysHost
  @param[in] Socket - Socket number
  @param[in] McId   - iMC number

  @retval N/A

**/
VOID
DoCompUpdate (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    McId
  )
{
  DDRINTF_CTL0_MCIO_DDRIO_STRUCT PmConfig0;
  //
  // Read DDRCRINTFPMCONFIG0
  //
  PmConfig0.Data = MemReadPciCfgMC (Socket, McId, DDRINTF_CTL0_MCIO_DDRIO_REG);
  //
  // Set comp update bit
  //
  PmConfig0.Bits.force_comp_update = 1;
  //
  // Write DDRCRINTFPMCONFIG0
  //
  MemWritePciCfgMC (Socket, McId, DDRINTF_CTL0_MCIO_DDRIO_REG, PmConfig0.Data);
} // DoCompUpdate

/**

  Issue a ZQ cycle

  @param Host            - sysHost data structure
  @param socket          - Processor socket
  @param ch              - Channel to issue the ZQ to
  @param zqType          - Type of ZQ Calibration: Long or Short

  @retval N/A

**/
VOID
DoZQ (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     zqType
  )
{
  CpgcIssueZQ (Host, socket, ch, zqType);
} // DoZQ

VOID
GetOriginalRtlChip (
  PSYSHOST    Host,                             // Pointer to the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Rank number (0-based)
  UINT8       RtlInitializationMode,            // RTL Initialization Mode: RTL_INITIALIZE_ONCE or RTL_RE_INITIALIZE
  UINT8       *roundTripLatency                 // Pointer to the round trip latency
  )
{
  //
  // Get original RTL
  //
  if (RtlInitializationMode == RTL_INITIALIZE_ONCE) {
    GetRoundTrip (socket, ch, GetLogicalRank (Host, socket, ch, dimm, rank), GSM_READ_CSR, roundTripLatency);
  }
} // GetOriginalRtlChip

/**

  Set the IO latency register value in the register and cache

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0 >= Ch < MaxChDdr)
  @param[in]  Rank            - Rank number (0 >= Rank < MAX_RANK_CH)
  @param[in]  IoLatencyValue  - IO Latency value to set

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
SetIOLatency (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    Rank,
  IN    UINT8    IOLatencyValue
  )
{
  PSYSHOST                                  Host;
  DDRINTF_IO_LAT0_CH_0_MCIO_DDRIO_STRUCT    IoLatency;
  UINT32                                    IoLatency0Ch[] = { DDRINTF_IO_LAT0_CH_0_MCIO_DDRIO_REG,
                                                               DDRINTF_IO_LAT0_CH_1_MCIO_DDRIO_REG,
                                                               DDRINTF_IO_LAT0_CH_2_MCIO_DDRIO_REG,
                                                               DDRINTF_IO_LAT0_CH_3_MCIO_DDRIO_REG
                                                             };
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  struct RankCh                             (*RankPerCh)[MAX_RANK_CH];
  UINT8                                     Index;
  UINT8                                     NumChPerMc;
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  NumChPerMc = GetNumChannelPerMc ();

  if ((Ch >= MaxChDdr) || (Rank >= MAX_RANK_CH)) {
    //
    // Invalid Ch or Rank specified
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_88);
    return EFI_INVALID_PARAMETER;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);
  RankPerCh = &(*ChannelNvList)[Ch].rankPerCh;
  Index = Ch % NumChPerMc;

  IoLatency.Data = MemReadPciCfgEp (Socket, Ch, IoLatency0Ch[Index]);
  IoLatency.Data &= ~(0xF << (Rank * 4));
  IoLatency.Data |= IOLatencyValue << (Rank * 4);
  MemWritePciCfgEp (Socket, Ch, IoLatency0Ch[Index], IoLatency.Data);

#ifdef DDR5_SUPPORT
  (*RankPerCh)[Rank].IOLatency[SUB_CH_A] = IOLatencyValue;
#else
  (*RankPerCh)[Rank].IOLatency = IOLatencyValue;
#endif // #ifdef DDR5_SUPPORT

  return EFI_SUCCESS;
} // SetIOLatency

/**
  Get the IO latency value for a single rank

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0 >= Ch < MaxChDdr)
  @param[in]  Rank            - Rank number (0 >= Rank < MAX_RANK_CH)
  @param[in]  Mode            - Bit field of access modes
  @param[out] IoLatencyValue  - Pointer to caller's IO Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
GetIoLatency (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    Rank,
  IN    UINT8    Mode,
  OUT   UINT8    *IoLatencyValue
  )
{
  PSYSHOST                                  Host;
  UINT8                                     Index;
  UINT8                                     NumChPerMc;
  DDRINTF_IO_LAT0_CH_0_MCIO_DDRIO_STRUCT    IoLatency0;
  UINT32                                    IoLatency0Ch[] = { DDRINTF_IO_LAT0_CH_0_MCIO_DDRIO_REG,
                                                               DDRINTF_IO_LAT0_CH_1_MCIO_DDRIO_REG,
                                                               DDRINTF_IO_LAT0_CH_2_MCIO_DDRIO_REG,
                                                               DDRINTF_IO_LAT0_CH_3_MCIO_DDRIO_REG
                                                             };
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  struct RankCh                             (*RankPerCh)[MAX_RANK_CH];
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  NumChPerMc = GetNumChannelPerMc ();

  if ((Ch >= MaxChDdr) || (Rank >= MAX_RANK_CH)) {
    //
    // Invalid Ch or Rank specified
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_89);
    return EFI_INVALID_PARAMETER;
  }

  if ((Mode & GSM_READ_CSR) != 0) {
    //
    // Read from the register
    //
    Index = Ch % NumChPerMc;

    IoLatency0.Data  = MemReadPciCfgEp (Socket, Ch, IoLatency0Ch[Index]);
    *IoLatencyValue = (UINT8)((IoLatency0.Data >> (Rank * 4)) & 0xF);

  } else {
    //
    // Read from the cache
    //
    ChannelNvList = GetChannelNvList (Host, Socket);
    RankPerCh = &(*ChannelNvList)[Ch].rankPerCh;

#ifdef DDR5_SUPPORT
    *IoLatencyValue = (*RankPerCh)[Rank].IOLatency[SUB_CH_A];
#else {
    *IoLatencyValue = (*RankPerCh)[Rank].IOLatency;
#endif // #ifdef DDR5_SUPPORT

  }

  return EFI_SUCCESS;
} // GetIoLatency

/**

  Set the round trip register value in the register and cache

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  Rank            - Rank number (0-based)
  @param[in]  RoundTripValue  - New Round Trip Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER if channel or rank is out of range
          EFI_OUT_OF_RESOURCES - indicates to the caller that the input RoundTripValue exceeds the max Round Trip Latency.
          !EFI_SUCCESS if other errors were encountered

**/
EFI_STATUS
SetRoundTrip (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    Rank,
  IN    UINT8    RoundTripValue
  )
{
  EFI_STATUS                                TemporaryStatus;
  EFI_STATUS                                Status = EFI_SUCCESS;
  PSYSHOST                                  Host;
  UINT8                                     MemoryController;
  UINT8                                     MemoryControllerChannel;
  UINT8                                     SubCh;
  UINT8                                     NumChPerMc;
  DDRINTF_RT_LAT0_CH_0_MCIO_DDRIO_STRUCT    RoundTrip0;
  DDRINTF_RT_LAT1_CH_0_MCIO_DDRIO_STRUCT    RoundTrip1;
  UINT8                                     RoundTripLocal[DIMM_ROUND_TRIP_RANKS_PER_REG];
  UINT32                                    RoundTripRegOffset;
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  struct RankCh                             (*RankPerCh)[MAX_RANK_CH];
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  NumChPerMc = GetNumChannelPerMc ();

  if ((Ch >= MaxChDdr) || (Rank >= MAX_RANK_CH)) {
    // Invalid Ch or Rank specified
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_90);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check input: RoundTripValue must be within range and must be an even value
  //
  if (RoundTripValue > MRC_ROUND_TRIP_MAX_VALUE) {
    // Max value exceeded so set RoundTripValue to its maximum value and set status to inform caller.
    // This is not necessarily a failure so the caller must decide what action to take.
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "Round Trip delay %d exceeds limit %d\n", RoundTripValue, MRC_ROUND_TRIP_MAX_VALUE);
    RoundTripValue = MRC_ROUND_TRIP_MAX_VALUE;
    Status = EFI_OUT_OF_RESOURCES;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);
  RankPerCh = &(*ChannelNvList)[Ch].rankPerCh;
  MemoryController = Ch / NumChPerMc;
  MemoryControllerChannel = Ch % NumChPerMc;

  for (SubCh = 0; SubCh < SUB_CH; SubCh ++) {
    TemporaryStatus = GetRoundTripRegOffset (Socket, Ch, MemoryControllerChannel, SubCh, Rank / MAX_RANK_DIMM, &RoundTripRegOffset);
    if (!EFI_ERROR (Status)) {
      Status = TemporaryStatus;
    }
    if (EFI_ERROR (TemporaryStatus)) {
      continue;
    }

    if (Rank < DIMM_ROUND_TRIP_RANKS_PER_REG) {
      RoundTrip0.Data  = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
      RoundTripLocal[0] = (UINT8)RoundTrip0.Bits.rt_lat0;
      RoundTripLocal[1] = (UINT8)RoundTrip0.Bits.rt_lat1;
      RoundTripLocal[2] = (UINT8)RoundTrip0.Bits.rt_lat2;
      RoundTripLocal[3] = (UINT8)RoundTrip0.Bits.rt_lat3;
    } else {
      RoundTrip1.Data  = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
      RoundTripLocal[0] = (UINT8)RoundTrip1.Bits.rt_lat4;
      RoundTripLocal[1] = (UINT8)RoundTrip1.Bits.rt_lat5;
      RoundTripLocal[2] = (UINT8)RoundTrip1.Bits.rt_lat6;
      RoundTripLocal[3] = (UINT8)RoundTrip1.Bits.rt_lat7;
    }

    //
    // Set the new value for the target rank and update the cache
    //
    RoundTripLocal[Rank % DIMM_ROUND_TRIP_RANKS_PER_REG] = RoundTripValue;
    (*RankPerCh)[Rank].Roundtrip = RoundTripValue;

    if (Rank < DIMM_ROUND_TRIP_RANKS_PER_REG) {
      RoundTrip0.Bits.rt_lat0 = RoundTripLocal[0];
      RoundTrip0.Bits.rt_lat1 = RoundTripLocal[1];
      RoundTrip0.Bits.rt_lat2 = RoundTripLocal[2];
      RoundTrip0.Bits.rt_lat3 = RoundTripLocal[3];
      MemWritePciCfgMC (Socket, MemoryController, RoundTripRegOffset, RoundTrip0.Data);
    } else {
      RoundTrip1.Bits.rt_lat4 = RoundTripLocal[0];
      RoundTrip1.Bits.rt_lat5 = RoundTripLocal[1];
      RoundTrip1.Bits.rt_lat6 = RoundTripLocal[2];
      RoundTrip1.Bits.rt_lat7 = RoundTripLocal[3];
      MemWritePciCfgMC (Socket, MemoryController, RoundTripRegOffset, RoundTrip1.Data);
    }
    //
    // Set any additional round trip settings in the MC
    //
    SetRoundTripMc (Socket, Ch, Rank, RoundTripValue);
  } // SubCh

  return Status;
} // SetRoundTrip

/**
  Get the round trip value

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  Rank            - Rank number (0-based)
  @param[in]  Mode            - Bit field of access modes
  @param[out] RoundTripValue  - Pointer to caller's Round Trip Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER if channel or rank is out of range
          !EFI_SUCCESS if other errors were encountered

**/
EFI_STATUS
GetRoundTrip (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    Rank,
  IN    UINT8    Mode,
  OUT   UINT8    *RoundTripValue
  )
{
  PSYSHOST                                  Host;
  UINT8                                     MemoryController;
  UINT8                                     MemoryControllerChannel;
  UINT8                                     NumChPerMc;
  DDRINTF_RT_LAT0_CH_0_MCIO_DDRIO_STRUCT    RoundTrip0;
  DDRINTF_RT_LAT1_CH_0_MCIO_DDRIO_STRUCT    RoundTrip1;
  UINT8                                     RoundTripLocal[DIMM_ROUND_TRIP_RANKS_PER_REG];
  UINT32                                    RoundTripRegOffset;
  EFI_STATUS                                Status = EFI_SUCCESS;
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  struct RankCh                             (*RankPerCh)[MAX_RANK_CH];
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  NumChPerMc = GetNumChannelPerMc ();

  if ((Ch >= MaxChDdr) || (Rank >= MAX_RANK_CH)) {
    //
    // Invalid Ch or Rank specified
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_91);
    return EFI_INVALID_PARAMETER;
  }

  if ((Mode & GSM_READ_CSR) != 0) {
    //
    // Read from the register
    //
    MemoryController = Ch / NumChPerMc;
    MemoryControllerChannel = Ch % NumChPerMc;

    Status = GetRoundTripRegOffset (Socket, Ch, MemoryControllerChannel, SUB_CH_A, Rank / MAX_RANK_DIMM, &RoundTripRegOffset);

    if (Rank < DIMM_ROUND_TRIP_RANKS_PER_REG) {
      RoundTrip0.Data  = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
      RoundTripLocal[0] = (UINT8)RoundTrip0.Bits.rt_lat0;
      RoundTripLocal[1] = (UINT8)RoundTrip0.Bits.rt_lat1;
      RoundTripLocal[2] = (UINT8)RoundTrip0.Bits.rt_lat2;
      RoundTripLocal[3] = (UINT8)RoundTrip0.Bits.rt_lat3;
    } else {
      RoundTrip1.Data  = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
      RoundTripLocal[0] = (UINT8)RoundTrip1.Bits.rt_lat4;
      RoundTripLocal[1] = (UINT8)RoundTrip1.Bits.rt_lat5;
      RoundTripLocal[2] = (UINT8)RoundTrip1.Bits.rt_lat6;
      RoundTripLocal[3] = (UINT8)RoundTrip1.Bits.rt_lat7;
    }

    *RoundTripValue = RoundTripLocal[Rank % DIMM_ROUND_TRIP_RANKS_PER_REG];

  } else {
    //
    // Read from the cache
    //
    ChannelNvList = GetChannelNvList (Host, Socket);
    RankPerCh = &(*ChannelNvList)[Ch].rankPerCh;
    *RoundTripValue = (*RankPerCh)[Rank].Roundtrip;
  }

  return Status;
} // GetRoundTrip

/**

  Restore roundtrip timings for fast boot or resume paths

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Processor to initialize
  @param[in]  Ch      - Channel number

  @retval EFI_SUCCESS   Round trip value was successfully restored
  @retval !EFI_SUCCESS  Round trip value was not successfully restored

**/
EFI_STATUS
RestoreRoundTrip (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch
  )
{
  UINT8                                     MemoryController;
  UINT8                                     MemoryControllerChannel;
  UINT8                                     SubCh;
  UINT8                                     RankIndex;
  UINT8                                     RoundTrip[DIMM_ROUND_TRIP_RANKS];
  UINT8                                     NumChPerMc;
  DDRINTF_RT_LAT0_CH_0_MCIO_DDRIO_STRUCT    RoundTrip0;
  UINT32                                    RoundTripRegOffset;
  EFI_STATUS                                Status;
  UINT8                                     MaxChDdr;
#ifndef DDR5_SUPPORT
  EFI_STATUS                                TemporaryStatus;
  DDRINTF_RT_LAT1_CH_0_MCIO_DDRIO_STRUCT    RoundTrip1;
#endif


  MaxChDdr = GetMaxChDdr ();
  NumChPerMc = GetNumChannelPerMc ();

  if (Ch >= MaxChDdr) {
    // invalid ch specified
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_124);
    return EFI_INVALID_PARAMETER;
  }

  MemoryController = Ch / NumChPerMc;
  MemoryControllerChannel = Ch % NumChPerMc;

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    for (RankIndex = 0; RankIndex < DIMM_ROUND_TRIP_RANKS; RankIndex++) {
      if (IsDdr5Present (Host, Socket)) {
        GetRoundTripDdr5 (Socket, Ch, SubCh, RankIndex, GSM_READ_ONLY, &RoundTrip[RankIndex]);
      } else {
        GetRoundTrip (Socket, Ch, RankIndex, GSM_READ_ONLY, &RoundTrip[RankIndex]);
      }
    }

    Status = GetRoundTripRegOffset (Socket, Ch, MemoryControllerChannel, SubCh, DIMM0, &RoundTripRegOffset);
    RoundTrip0.Data = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
    RoundTrip0.Bits.rt_lat0 = RoundTrip[0];
    RoundTrip0.Bits.rt_lat1 = RoundTrip[1];
    RoundTrip0.Bits.rt_lat2 = RoundTrip[2];
    RoundTrip0.Bits.rt_lat3 = RoundTrip[3];
    MemWritePciCfgMC (Socket, MemoryController, RoundTripRegOffset, RoundTrip0.Data);

#ifndef DDR5_SUPPORT
    //
    // DDR4 DIMM1: Need to restore DDRINTF_RT_LAT1_XXX registers for ranks on DDR4 DIMM1.
    // DDR5 DIMM1: DDR5 DIMM1 uses the same DDRINTF_RT_LAT0_XXX registers as DIMM0, which has
    //             been restored in above code. No need to restore DDRINTF_RT_LAT1_XXX registers for DDR5.
    //
    TemporaryStatus = GetRoundTripRegOffset (Socket, Ch, MemoryControllerChannel, SubCh, DIMM1, &RoundTripRegOffset);
    if (!EFI_ERROR (Status)) {
      Status = TemporaryStatus;
    }
    RoundTrip1.Data = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
    RoundTrip1.Bits.rt_lat4 = RoundTrip[4];
    RoundTrip1.Bits.rt_lat5 = RoundTrip[5];
    RoundTrip1.Bits.rt_lat6 = RoundTrip[6];
    RoundTrip1.Bits.rt_lat7 = RoundTrip[7];
    MemWritePciCfgMC (Socket, MemoryController, RoundTripRegOffset, RoundTrip1.Data);
 #endif
  }

  return Status;
}

/**

  Get the round trip register value for DDR5

  @param[in]  socket          - Socket number
  @param[in]  ch              - Channel number (0-based)
  @param[in]  subch           - Sub Channel number (0-based)
  @param[in]  rank            - Rank number (0-based)
  @param[in]  Mode            - Bit field specifying access mode
  @param[out] RoundTripValue  - Pointer to caller's Round Trip Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER if channel or rank is out of range
          !EFI_SUCCESS if other errors were encountered

**/
EFI_STATUS
GetRoundTripDdr5 (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    SubCh,
  IN    UINT8    Rank,
  IN    UINT8    Mode,
  OUT   UINT8    *RoundTripValue
  )
{
  PSYSHOST                                  Host;
  UINT8                                     MemoryController;
  UINT8                                     MemoryControllerChannel;
  UINT8                                     NumChPerMc;
  DDRINTF_RT_LAT0_CH_0_MCIO_DDRIO_STRUCT    RoundTrip0;
  DDRINTF_RT_LAT1_CH_0_MCIO_DDRIO_STRUCT    RoundTrip1;
  UINT8                                     RoundTripLocal[DIMM_ROUND_TRIP_RANKS_PER_REG];
  UINT32                                    RoundTripRegOffset;
  EFI_STATUS                                Status = EFI_SUCCESS;
  UINT8                                     ChannelLocal = 0;
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  struct RankCh                             (*RankPerCh)[MAX_RANK_CH];
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  NumChPerMc = GetNumChannelPerMc ();

  ChannelLocal = Ch;
  Mode = Mode | GSM_READ_CSR; //SPR_TODO BIOS HSD-ES 1507419027
  GetChannelMapping (Ch, SubCh, &ChannelLocal);

  if ((ChannelLocal >= MaxChDdr) || (Rank >= MAX_RANK_CH)) {
    // Invalid Ch or Rank specified
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_91);
    return EFI_INVALID_PARAMETER;
  }

  if ((Mode & GSM_READ_CSR) != 0) {
    //
    // Read from the register
    //
    MemoryController = ChannelLocal / NumChPerMc;
    MemoryControllerChannel = ChannelLocal % NumChPerMc;

    Status = GetRoundTripRegOffset (Socket, Ch, MemoryControllerChannel, SubCh, Rank / MAX_RANK_DIMM, &RoundTripRegOffset);

    if (Rank < DIMM_ROUND_TRIP_RANKS_PER_REG) {
      RoundTrip0.Data  = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
      RoundTripLocal[0] = (UINT8)RoundTrip0.Bits.rt_lat0;
      RoundTripLocal[1] = (UINT8)RoundTrip0.Bits.rt_lat1;
      RoundTripLocal[2] = (UINT8)RoundTrip0.Bits.rt_lat2;
      RoundTripLocal[3] = (UINT8)RoundTrip0.Bits.rt_lat3;
    } else {
      RoundTrip1.Data  = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
      RoundTripLocal[0] = (UINT8)RoundTrip1.Bits.rt_lat4;
      RoundTripLocal[1] = (UINT8)RoundTrip1.Bits.rt_lat5;
      RoundTripLocal[2] = (UINT8)RoundTrip1.Bits.rt_lat6;
      RoundTripLocal[3] = (UINT8)RoundTrip1.Bits.rt_lat7;
    }

    *RoundTripValue = RoundTripLocal[Rank % DIMM_ROUND_TRIP_RANKS_PER_REG];

  } else {
    //
    // Read from the cache
    //
    ChannelNvList = GetChannelNvList (Host, Socket);
    RankPerCh = &(*ChannelNvList)[ChannelLocal].rankPerCh;
    // SPR_TODO BIOS HSD-ES 1504716216 - We need to double the number of roundtrip registers that are cached
    *RoundTripValue = (*RankPerCh)[Rank].Roundtrip;
  }

  return Status;
}

/**

  Set the round trip register value for DDR5

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  Subch           - Sub Channel number (0-based)
  @param[in]  Rank            - Rank number (0-based)
  @param[in]  RoundTripValue  - New Round Trip Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER if channel or rank is out of range
          !EFI_SUCCESS if other errors were encountered

**/
EFI_STATUS
SetRoundTripDdr5 (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    SubCh,
  IN    UINT8    Rank,
  IN    UINT8    RoundTripValue
  )
{
  EFI_STATUS                                Status;
  PSYSHOST                                  Host;
  UINT8                                     MemoryController;
  UINT8                                     MemoryControllerChannel;
  UINT8                                     NumChPerMc;
  DDRINTF_RT_LAT0_CH_0_MCIO_DDRIO_STRUCT    RoundTrip0;
  DDRINTF_RT_LAT1_CH_0_MCIO_DDRIO_STRUCT    RoundTrip1;
  UINT8                                     RoundTripLocal[DIMM_ROUND_TRIP_RANKS_PER_REG];
  UINT32                                    RoundTripRegOffset;
  UINT8                                     ChannelLocal = 0;
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  struct RankCh                             (*RankPerCh)[MAX_RANK_CH];
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  NumChPerMc = GetNumChannelPerMc ();

  ChannelNvList = GetChannelNvList (Host, Socket);
  ChannelLocal = Ch;

  GetChannelMapping (Ch, SubCh, &ChannelLocal);

  if ((ChannelLocal >= MaxChDdr) || (Rank >= MAX_RANK_CH)) {
    // Invalid Ch or Rank specified
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_90);
    return EFI_INVALID_PARAMETER;
  }
  if (GetCurrentTestType (Socket) == RcvenFineTest) {
    //
    // Add the delay before changing the round trip latency as there are outstanding rdcmds in the INTF FIFO.
    // DCLK Delay = RoundTripValue + 9
    //
      TrainingDelay (Host, (RoundTripValue + 9)*2, 1);
  }
  RankPerCh = &(*ChannelNvList)[ChannelLocal].rankPerCh;
  MemoryController = ChannelLocal / NumChPerMc;
  MemoryControllerChannel = ChannelLocal % NumChPerMc;
  Status = GetRoundTripRegOffset (Socket, Ch, MemoryControllerChannel, SubCh, Rank / MAX_RANK_DIMM, &RoundTripRegOffset);

  if (Rank < DIMM_ROUND_TRIP_RANKS_PER_REG) {
    RoundTrip0.Data  = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
    RoundTripLocal[0] = (UINT8)RoundTrip0.Bits.rt_lat0;
    RoundTripLocal[1] = (UINT8)RoundTrip0.Bits.rt_lat1;
    RoundTripLocal[2] = (UINT8)RoundTrip0.Bits.rt_lat2;
    RoundTripLocal[3] = (UINT8)RoundTrip0.Bits.rt_lat3;
  } else {
    RoundTrip1.Data  = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
    RoundTripLocal[0] = (UINT8)RoundTrip1.Bits.rt_lat4;
    RoundTripLocal[1] = (UINT8)RoundTrip1.Bits.rt_lat5;
    RoundTripLocal[2] = (UINT8)RoundTrip1.Bits.rt_lat6;
    RoundTripLocal[3] = (UINT8)RoundTrip1.Bits.rt_lat7;
  }

  //
  // Set the new value for the target rank and update the cache
  //
  RoundTripLocal[Rank % DIMM_ROUND_TRIP_RANKS_PER_REG] = RoundTripValue;
  // SPR_TODO BIOS HSD-ES 1504716216 - We need to double the number of roundtrip registers that are cached
  (*RankPerCh)[Rank].Roundtrip = RoundTripValue;

  if (Rank < DIMM_ROUND_TRIP_RANKS_PER_REG) {
    RoundTrip0.Bits.rt_lat0 = RoundTripLocal[0];
    RoundTrip0.Bits.rt_lat1 = RoundTripLocal[1];
    RoundTrip0.Bits.rt_lat2 = RoundTripLocal[2];
    RoundTrip0.Bits.rt_lat3 = RoundTripLocal[3];
    MemWritePciCfgMC (Socket, MemoryController, RoundTripRegOffset, RoundTrip0.Data);
  } else {
    RoundTrip1.Bits.rt_lat4 = RoundTripLocal[0];
    RoundTrip1.Bits.rt_lat5 = RoundTripLocal[1];
    RoundTrip1.Bits.rt_lat6 = RoundTripLocal[2];
    RoundTrip1.Bits.rt_lat7 = RoundTripLocal[3];
    MemWritePciCfgMC (Socket, MemoryController, RoundTripRegOffset, RoundTrip1.Data);
  }

  return Status;
} // SetRoundTripDdr5


/**

  Set the round trip register value for DDR5 for all ranks

  @param[in]  Socket          Socket number
  @param[in]  Ch              Channel number (0-based)
  @param[in]  Subch           Sub Channel number (0-based)
  @param[in]  RoundTripValue  New Round Trip Latency value

  @retval MRC_STATUS_SUCCESS  When this function completes execution

**/
MRC_STATUS
SetRoundTripAll (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    SubCh,
  IN    UINT8    RoundTripValue
  )
{
  UINT8   Rank;

  for (Rank = 0; Rank < MAX_RANK_CH; Rank++) {
    SetRoundTripDdr5 (Socket, Ch, SubCh, Rank, RoundTripValue);
  }


  return MRC_STATUS_SUCCESS;
} // SetRoundTripAll

/**

  Set the IO latency register value in the register and cache

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0 >= Ch < MaxChDdr)
  @param[in]  Subch           - Sub Channel number (0-based)
  @param[in]  Rank            - Rank number (0 >= Rank < MAX_RANK_CH)
  @param[in]  IoLatencyValue  - IO Latency value to set

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
SetIoLatencyDdr5 (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    SubCh,
  IN    UINT8    Rank,
  IN    UINT8    IOLatencyValue
  )
{
  PSYSHOST                                Host;
  DDRINTF_IO_LAT0_CH_0_MCIO_DDRIO_STRUCT  IoLatency;
  UINT8                                   Index;
  UINT32                                  IoLatency0Ch[] = { DDRINTF_IO_LAT0_CH_0_MCIO_DDRIO_REG,
                                                             DDRINTF_IO_LAT0_CH_1_MCIO_DDRIO_REG,
                                                             DDRINTF_IO_LAT0_CH_2_MCIO_DDRIO_REG,
                                                             DDRINTF_IO_LAT0_CH_3_MCIO_DDRIO_REG};
  struct channelNvram                     (*ChannelNvList)[MAX_CH];
  struct RankCh                           (*RankPerCh)[MAX_RANK_CH];
  UINT8                                   MaxChDdr;
  UINT8                                   SwappedSubCh;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();


  if ((Ch >= MaxChDdr) || (Rank >= MAX_RANK_CH)) {
    //
    // Invalid Ch or Rank specified
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_88);
    return EFI_INVALID_PARAMETER;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);
  RankPerCh     = &(*ChannelNvList)[Ch].rankPerCh;

  //
  // Update Sub Ch just in case lane reversal is enabled on current Ch.
  //
  SwappedSubCh = UpdateDdrIoCcRegisterSubCh (Host, Socket, Ch, SubCh);

  Index         = (Ch*SUB_CH + SwappedSubCh) % MAX_MC_SCH;

  IoLatency.Data = MemReadPciCfgEp (Socket, Ch, IoLatency0Ch[Index]);
  IoLatency.Data &= ~(0xF << (Rank * 4));
  IoLatency.Data |= IOLatencyValue << (Rank * 4);
  MemWritePciCfgEp (Socket, Ch, IoLatency0Ch[Index], IoLatency.Data);

#ifdef DDR5_SUPPORT
  (*RankPerCh)[Rank].IOLatency[SubCh] = IOLatencyValue;
#else
  (*RankPerCh)[Rank].IOLatency = IOLatencyValue;
#endif // #ifdef DDR5_SUPPORT

  return EFI_SUCCESS;
} // SetIoLatencyDdr5

/**
  Get the IO latency value for a single rank

  @param[in]  Socket          - Socket numer
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  Subch           - Sub Channel number (0-based)
  @param[in]  Rank            - Rank number (0-based)
  @param[in]  Mode            - Bit field of access modes
  @param[out] IoLatencyValue  - Pointer to caller's IO Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
GetIoLatencyDdr5 (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    SubCh,
  IN    UINT8    Rank,
  IN    UINT8    Mode,
  OUT   UINT8    *IoLatencyValue
  )
{
  PSYSHOST                                Host;
  UINT8                                   Index;
  DDRINTF_IO_LAT0_CH_0_MCIO_DDRIO_STRUCT  IoLatency0;
  UINT32                                  IoLatency0Ch[] = { DDRINTF_IO_LAT0_CH_0_MCIO_DDRIO_REG,
                                                             DDRINTF_IO_LAT0_CH_1_MCIO_DDRIO_REG,
                                                             DDRINTF_IO_LAT0_CH_2_MCIO_DDRIO_REG,
                                                             DDRINTF_IO_LAT0_CH_3_MCIO_DDRIO_REG};
  struct channelNvram                     (*ChannelNvList)[MAX_CH];
  struct RankCh                           (*RankPerCh)[MAX_RANK_CH];
  UINT8                                   MaxChDdr;
  UINT8                                   SwappedSubCh;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();

  //
  // Update Sub Ch just in case lane reversal is enabled on current Ch.
  //
  SwappedSubCh = UpdateDdrIoCcRegisterSubCh (Host, Socket, Ch, SubCh);

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    Mode = Mode | GSM_READ_CSR;
  }
  if ((Ch >= MaxChDdr) || (Rank >= MAX_RANK_CH)) {
    //
    // Invalid Ch or Rank specified
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_89);
    return EFI_INVALID_PARAMETER;
  }

  if ((Mode & GSM_READ_CSR) != 0) {
    //
    // Read from the register
    //
    Index = (Ch*SUB_CH + SwappedSubCh) % MAX_MC_SCH;

    IoLatency0.Data  = MemReadPciCfgEp (Socket, Ch, IoLatency0Ch[Index]);
    *IoLatencyValue = (UINT8)((IoLatency0.Data >> (Rank * 4)) & 0xF);

  } else {
    //
    // Read from the cache
    //
    ChannelNvList = GetChannelNvList (Host, Socket);
    RankPerCh = &(*ChannelNvList)[Ch].rankPerCh;

#ifdef DDR5_SUPPORT
    *IoLatencyValue = (*RankPerCh)[Rank].IOLatency[SubCh];
#else {
    *IoLatencyValue = (*RankPerCh)[Rank].IOLatency;
#endif // #ifdef DDR5_SUPPORT

  }

  return EFI_SUCCESS;
} // GetIoLatencyDdr5


/**
  Find the maximal round trip latency of all ranks in a channel, adjust each rank's
  round trip latency to this common channel level value.

  @param[in    ]  Socket            Socket number
  @param[in out]  RoundTripLatency  Input value is per-rank RTL, output value is the adjusted per-channel RTL value.
  @param[in out]  IoLatency         Input value is per-rank IOL, output value is the adjusted per-channel IOL value.

  @retval None
*/
VOID
EFIAPI
AdjustChannelRoundTrip (
 IN     UINT8   Socket,
 IN OUT UINT8   (*RoundTripLatency)[MAX_CH][SUB_CH][MAX_RANK_CH],
 IN OUT UINT8   (*IoLatency)[MAX_CH][SUB_CH][MAX_RANK_CH]
 )
{
  PSYSHOST        Host;
  UINT8           ChLevelOptimalRT[MAX_CH];
  UINT8           Ch;
  UINT8           SubCh;
  UINT8           Dimm;
  UINT8           Rank;
  UINT8           LogicalRank;
  UINT8           MaxChDdr;
  UINT8           RtlDelta[MAX_CH][SUB_CH][MAX_RANK_CH];

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();

  ZeroMem ((UINT8 *)RtlDelta,sizeof(RtlDelta));

  RcDebugPrint(SDBG_MAX, "\nRTL Adjusment Summary:\n");
  RcDebugPrint(SDBG_MAX, "                Rank0  Rank1  Rank2  Rank3\n");
  for (Ch = 0; Ch < MaxChDdr; Ch++) {

    ChLevelOptimalRT[Ch] = (*RoundTripLatency)[Ch][SUB_CH_A][RANK0];

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    //
    // Find the maximal round trip latency of all ranks in a channel.
    //
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      for (Rank = 0; Rank < GetNumberOfRanksOnDimm (Socket, Ch, Dimm); Rank++) {

        if (IsRankValid (Socket, Ch, Dimm, Rank, FALSE) == FALSE) {
          continue;
        }

        LogicalRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          if (ChLevelOptimalRT[Ch] < (*RoundTripLatency)[Ch][SubCh][LogicalRank]) {
            ChLevelOptimalRT[Ch] = (*RoundTripLatency)[Ch][SubCh][LogicalRank];
          }
        }
      }
    }

    //
    // Adjust each rank's round trip latency to this common channel level value.
    //
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      for (Rank = 0; Rank < GetNumberOfRanksOnDimm (Socket, Ch, Dimm); Rank++) {

        if (IsRankValid (Socket, Ch, Dimm, Rank, FALSE) == FALSE) {
          continue;
        }

        LogicalRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          RtlDelta [Ch][SubCh][LogicalRank] = ChLevelOptimalRT[Ch] - (*RoundTripLatency)[Ch][SubCh][LogicalRank];
          (*RoundTripLatency)[Ch][SubCh][LogicalRank] = ChLevelOptimalRT[Ch];
          (*IoLatency)[Ch][SubCh][LogicalRank] = (*IoLatency)[Ch][SubCh][LogicalRank] + RtlDelta[Ch][SubCh][LogicalRank];
        }
      }
    }

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,"");

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        for (Rank = 0; Rank < GetNumberOfRanksOnDimm (Socket, Ch, Dimm); Rank++) {
          LogicalRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
          RcDebugPrint(SDBG_MAX,"%7d", RtlDelta [Ch][SubCh][LogicalRank]);
        }
      }
      RcDebugPrint(SDBG_MAX,"\n");
    }
  }

  return;
}

/**

  Set/Restore the DDRIO training mode

  @param[in]     Socket           - Socket number
  @param[in]     Mode             - Training mode to set

  @retval N/A

**/
VOID
EFIAPI
SetTrainingModeDdr5 (
  IN      UINT8           Socket,
  IN      TRAINING_MODE   Mode
  )
{
  PSYSHOST                                        Host;
  DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_STRUCT            DdrdN0TrainCtl1;
  DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_STRUCT            DdrdN0TrainCtl2;
  DDRIO_CHANNEL_NVDATA                            (*DdrioChannelNvList) [MAX_CH];
  UINT8                                           Ch;
  UINT8                                           Strobe;
  UINT8                                           MSVx4;
  UINT8                                           MaxChDdr;

  Host               = GetSysHostPointer ();
  MSVx4              = GetMaxStrobeValid (Host);
  MaxChDdr           = GetMaxChDdr ();
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      //
      // Skip if this is an ECC Strobe when ECC is disabled
      //
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      //
      // Get the current state
      //
      DdrdN0TrainCtl1.Data     = (*DdrioChannelNvList) [Ch].DdrdN0TrainCtl1[Strobe];
      DdrdN0TrainCtl2.Data     = (*DdrioChannelNvList) [Ch].DdrdN0TrainCtl2[Strobe];

      switch (Mode) {
        case TmSenseAmp:
          DdrdN0TrainCtl1.Bits.dq_vref_en           = 1;
          DdrdN0TrainCtl1.Bits.ddr4                 = 0;
          DdrdN0TrainCtl1.Bits.force_odt_on         = 1;
          DdrdN0TrainCtl1.Bits.force_sa_on          = 1;
          DdrdN0TrainCtl1.Bits.force_rxbias_on      = 1;
          DdrdN0TrainCtl1.Bits.vref_highz           = 1;
          DdrdN0TrainCtl1.Bits.force_strong_odt_on  = 1;
          break;

        case TmSenseAmpExit:
          DdrdN0TrainCtl1.Bits.vref_highz           = 0;
          DdrdN0TrainCtl1.Bits.force_strong_odt_on  = 0;
          break;

        case TmReceiveEnable:
          DdrdN0TrainCtl1.Bits.dq_vref_en           = 1;
          DdrdN0TrainCtl1.Bits.force_odt_on         = 1;
          DdrdN0TrainCtl1.Bits.force_sa_on          = 1;
          DdrdN0TrainCtl1.Bits.force_rxbias_on      = 1;
          DdrdN0TrainCtl1.Bits.rd_level_train_en    = 1;
          break;

        case TmReceiveEnableExit:
          DdrdN0TrainCtl1.Bits.rd_level_train_en    = 0;
          break;

        case TmQcsQcaClk:
        case TmRxDqDqs:
        case TmBacksideTraining:
          DdrdN0TrainCtl1.Bits.dq_vref_en           = 1;
          DdrdN0TrainCtl1.Bits.force_odt_on         = 1;
          DdrdN0TrainCtl1.Bits.force_sa_on          = 1;
          DdrdN0TrainCtl1.Bits.force_rxbias_on      = 1;
          DdrdN0TrainCtl1.Bits.rd_level_train_en    = 0;
          break;

        case TmRxDqDqsCoarseExit:
          DdrdN0TrainCtl1.Bits.force_odt_on         = 1;
          DdrdN0TrainCtl1.Bits.force_sa_on          = 0;
          break;

        case TmRxDqDqsExit:
        case TmBacksideTrainingExit:
          DdrdN0TrainCtl1.Bits.force_odt_on         = 1;
          DdrdN0TrainCtl1.Bits.force_sa_on          = 0;
          DdrdN0TrainCtl1.Bits.force_rxbias_on      = 0;
          break;

        case TmWriteLeveing:
          DdrdN0TrainCtl1.Bits.dq_vref_en           = 1;
          DdrdN0TrainCtl1.Bits.force_odt_on         = 1;
          DdrdN0TrainCtl1.Bits.force_sa_on          = 1;
          DdrdN0TrainCtl1.Bits.force_rxbias_on      = 1;
          DdrdN0TrainCtl1.Bits.wr_level_train_en    = 1;
          DdrdN0TrainCtl1.Bits.cmd_train_ctr_en     = 0;
          DdrdN0TrainCtl1.Bits.rd_level_train_en    = 0;
          break;

        case TmWriteLeveingExit:
          DdrdN0TrainCtl1.Bits.force_odt_on         = 1;
          DdrdN0TrainCtl1.Bits.force_sa_on          = 0;
          DdrdN0TrainCtl1.Bits.wr_level_train_en    = 0;
          break;

        case TmTxDqDqs:
          DdrdN0TrainCtl1.Bits.dq_vref_en           = 1;
          DdrdN0TrainCtl1.Bits.force_odt_on         = 1;
          DdrdN0TrainCtl1.Bits.force_sa_on          = 1;
          DdrdN0TrainCtl1.Bits.force_rxbias_on      = 1;
          break;

        case TmTxDqDqsExit:
          DdrdN0TrainCtl1.Bits.force_odt_on         = 1;
          break;

        case TmPostTraining:
          if (UbiosGenerationOrHsleEnabled ()) {
            DdrdN0TrainCtl1.Bits.dq_vref_en         = 1;
            DdrdN0TrainCtl1.Bits.force_sa_on        = 1;
            DdrdN0TrainCtl1.Bits.force_odt_on       = 0;
            DdrdN0TrainCtl1.Bits.force_rxbias_on    = 0;
          } else {
            DdrdN0TrainCtl1.Bits.dq_vref_en         = 1;
            DdrdN0TrainCtl1.Bits.force_sa_on        = 0;
            DdrdN0TrainCtl1.Bits.force_odt_on       = 1;
            DdrdN0TrainCtl1.Bits.force_rxbias_on    = 1;
          }
          break;

        case TmBcomTraining:
        case TmMrd:
          DdrdN0TrainCtl1.Bits.dq_vref_en           = 1;
          DdrdN0TrainCtl1.Bits.force_sa_on          = 1;
          DdrdN0TrainCtl1.Bits.force_rxbias_on      = 1;
          DdrdN0TrainCtl1.Bits.force_odt_on         = 1;

          DdrdN0TrainCtl2.Bits.sample_sel           = 1;
          DdrdN0TrainCtl2.Bits.dq_sample_mask       = 0x0;
          DdrdN0TrainCtl2.Bits.sampler_feedback_en  = 0;
          DdrdN0TrainCtl2.Bits.unsampled_path_en    = 1;
          break;

        case TmBcomTrainingExit:
        case TmMrdExit:
          DdrdN0TrainCtl1.Bits.dq_vref_en           = 1;
          DdrdN0TrainCtl1.Bits.force_sa_on          = 0;
          DdrdN0TrainCtl1.Bits.force_odt_on         = 1;
          DdrdN0TrainCtl1.Bits.force_rxbias_on      = 1;
          DdrdN0TrainCtl2.Bits.unsampled_path_en    = 0;
          DdrdN0TrainCtl2.Bits.dq_sample_mask       = 0x0;
          break;

        case TmMre:
          DdrdN0TrainCtl1.Bits.dq_vref_en           = 1;
          DdrdN0TrainCtl1.Bits.force_sa_on          = 1;
          DdrdN0TrainCtl1.Bits.force_rxbias_on      = 1;
          DdrdN0TrainCtl1.Bits.force_odt_on         = 1;
          DdrdN0TrainCtl1.Bits.rd_level_train_type  = 1;
          DdrdN0TrainCtl1.Bits.rd_level_train_en    = 1;

          DdrdN0TrainCtl2.Bits.sampler_feedback_en  = 0;
          DdrdN0TrainCtl2.Bits.unsampled_path_en    = 1;
          DdrdN0TrainCtl2.Bits.sample_sel           = 1;
          DdrdN0TrainCtl2.Bits.dq_sample_mask       = 0x0;
          break;

        case TmMreExit:
          DdrdN0TrainCtl1.Bits.dq_vref_en           = 1;
          DdrdN0TrainCtl1.Bits.force_sa_on          = 0;
          DdrdN0TrainCtl1.Bits.force_odt_on         = 1;
          DdrdN0TrainCtl1.Bits.force_rxbias_on      = 1;
          DdrdN0TrainCtl1.Bits.rd_level_train_type  = 0;
          DdrdN0TrainCtl1.Bits.rd_level_train_en    = 0;
          DdrdN0TrainCtl2.Bits.unsampled_path_en    = 0;
          break;

        case TmDwl:
          DdrdN0TrainCtl1.Bits.dq_vref_en           = 1;
          DdrdN0TrainCtl1.Bits.force_sa_on          = 1;
          DdrdN0TrainCtl1.Bits.force_rxbias_on      = 1;
          DdrdN0TrainCtl1.Bits.force_odt_on         = 1;
          DdrdN0TrainCtl1.Bits.wr_level_train_en    = 1;

          DdrdN0TrainCtl2.Bits.sampler_feedback_en  = 0;
          DdrdN0TrainCtl2.Bits.unsampled_path_en    = 1;
          DdrdN0TrainCtl2.Bits.sample_sel           = 1;
          DdrdN0TrainCtl2.Bits.dq_sample_mask       = 0x0;
          break;

        case TmDwlExit:
          DdrdN0TrainCtl1.Bits.dq_vref_en           = 1;
          DdrdN0TrainCtl1.Bits.force_sa_on          = 0;
          DdrdN0TrainCtl1.Bits.force_rxbias_on      = 1;
          DdrdN0TrainCtl1.Bits.force_odt_on         = 1;
          DdrdN0TrainCtl1.Bits.wr_level_train_en    = 0;

          DdrdN0TrainCtl2.Bits.unsampled_path_en    = 0;
          break;

        case TmMwd:
          DdrdN0TrainCtl1.Bits.dq_vref_en           = 1;
          DdrdN0TrainCtl1.Bits.force_sa_on          = 1;
          DdrdN0TrainCtl1.Bits.force_rxbias_on      = 1;
          DdrdN0TrainCtl1.Bits.force_odt_on         = 1;
          DdrdN0TrainCtl1.Bits.wr_level_train_en    = 1;

          DdrdN0TrainCtl2.Bits.sampler_feedback_en  = 0;
          DdrdN0TrainCtl2.Bits.unsampled_path_en    = 1;
          DdrdN0TrainCtl2.Bits.dq_sample_mask       = 0;
          break;

        case TmMwdExit:
          DdrdN0TrainCtl1.Bits.dq_vref_en           = 1;
          DdrdN0TrainCtl1.Bits.force_sa_on          = 0;
          DdrdN0TrainCtl1.Bits.force_odt_on         = 1;
          DdrdN0TrainCtl1.Bits.force_rxbias_on      = 1;
          DdrdN0TrainCtl1.Bits.wr_level_train_en    = 0;
          DdrdN0TrainCtl2.Bits.unsampled_path_en    = 0;
          DdrdN0TrainCtl2.Bits.dq_sample_mask       = 0;
          break;

        default:
          break;
      } // switch Mode
      //
      // Set new state in HW and maintain cache coherency
      //
      (*DdrioChannelNvList) [Ch].DdrdN0TrainCtl1[Strobe]   = DdrdN0TrainCtl1.Data;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG), DdrdN0TrainCtl1.Data);

      // Only write to TrainCtl2 if necessary
      if (DdrdN0TrainCtl2.Data != (*DdrioChannelNvList) [Ch].DdrdN0TrainCtl2[Strobe]) {
        (*DdrioChannelNvList) [Ch].DdrdN0TrainCtl2[Strobe] = DdrdN0TrainCtl2.Data;
        MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG), DdrdN0TrainCtl2.Data);
      }
    } // Strobe loop
  } // Ch loop

} // SetTrainingModeDdr5

/**

  Enable/Disable Write Level training mode in the DDRIO.

  @param[in] Socket  - Socket number
  @param[in] Enable  - TRUE to enable, FALSE to disable

  retval N/A

**/
VOID
EFIAPI
SetWlTrainingMode (
  IN UINT8    Socket,
  IN BOOLEAN  Enable
  )
{
  PSYSHOST                                        Host;
  DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_STRUCT            DdrdN0TrainCtl1;
  DDRIO_CHANNEL_NVDATA                            (*DdrioChannelNvList) [MAX_CH];
  UINT8                                           Ch;
  UINT8                                           Strobe;
  UINT8                                           MSVx4;
  UINT8                                           MaxChDdr;
  UINT8                                           WlEnable;

  Host               = GetSysHostPointer ();
  MSVx4              = GetMaxStrobeValid (Host);
  MaxChDdr           = GetMaxChDdr ();
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  if (Enable) {
    WlEnable = 1;
  } else {
    WlEnable = 0;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      //
      // Skip if this is an ECC Strobe when ECC is disabled
      //
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      //
      // Get the current state
      //
      DdrdN0TrainCtl1.Data = (*DdrioChannelNvList) [Ch].DdrdN0TrainCtl1[Strobe];

      DdrdN0TrainCtl1.Bits.wr_level_train_en = WlEnable;

      //
      // Set new state in HW and maintain cache coherency
      //
      (*DdrioChannelNvList) [Ch].DdrdN0TrainCtl1[Strobe]   = DdrdN0TrainCtl1.Data;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG), DdrdN0TrainCtl1.Data);
    }
  }
}

/**

  Clears Bit-wise error status registers per channel

  @param Host  - Pointer to sysHost
  @param socket  - Socket number
  @param ch    - Channel number (0-based)

  @retval N/A

**/
VOID
ClearBWErrorStatus (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
} // ClearBWErrorStatus

/**

  Toggle EqPost2Sign while TxEq Tunning for tap2

  @param Host    - Pointer to sysHost
  @param Socket  - Socket number
  @param Enable  - Enable / Disable

  @retval N/A

**/
MRC_STATUS
ToggleEqPost2Sign (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Strobe,
  IN UINT8    InvertData
) {
  UINT8                               Ch;
  UINT8                               MaxStrobe;
  DDRD_N0_TX_CTL0_MCIO_DDRIO_STRUCT   DdrDataTxCtl0;
  DDRIO_CHANNEL_NVDATA                (*DdrioChannelNvList)[MAX_CH];
  UINT8                               MaxChDdr;

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  MaxChDdr = GetMaxChDdr ();
  //
  // Define the limits
  //
  if (Strobe == ALL_DRAMS) {
    Strobe    = 0;
    MaxStrobe = MAX_STROBE;
  } else {
    MaxStrobe = Strobe + 1;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (Strobe; Strobe < MaxStrobe; Strobe++) {
      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }
      DdrDataTxCtl0.Data = (*DdrioChannelNvList)[Ch].DdrDataTxCtl0[Strobe];
      if (InvertData != INVERT_TRUE) {
        InvertData = INVERT_FALSE;
      }
      DdrDataTxCtl0.Bits.eq_post2_sign = InvertData;
      (*DdrioChannelNvList)[Ch].DdrDataTxCtl0[Strobe] = DdrDataTxCtl0.Data;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TX_CTL0_MCIO_DDRIO_REG), DdrDataTxCtl0.Data);
    }
  }

  return MRC_STATUS_SUCCESS;
}

/**

  Control flow between 10nm and other legacy products

  @param Host    - Pointer to sysHost
  @param Param   - Parameter power training

  @retval N/A

**/
MRC_STATUS
ExecuteTheNextOptPhaseOnThisProduct (
  IN PSYSHOST          Host,
  IN POWER_TRAIN_PARAM Param
)
{
  return MRC_STATUS_SUCCESS;
}
/**

  Set the initial configuration for the TxEq training

  @param Host     - Pointer to sysHost
  @param Socket   - Socket

  @retval MRC_STATUS

**/
MRC_STATUS
InitTxEqTunning (
  IN PSYSHOST          Host,
  IN UINT8             Socket
  )
{
  ToggleEqPost2Sign (Host, Socket, ALL_DRAMS, INVERT_FALSE);

  return MRC_STATUS_SUCCESS;
}

/**

  Initialize VSSHIORVREFCONTROL

  @param[in] Host  - Pointer to sysHost
  @param[in] socket  - Processor to initialize

  @retval N/A

**/
VOID
InitVssHiControlDdrio (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket
  )
{
  //
  // Stub function to satisfy API.
  //

  RC_FATAL_ERROR (FALSE, 0, 0);

  return;

} // InitVssHiControlDdrio

/**

  DDRIO setup for C/A Parity error flow

  @param[in] Host     - Pointer to sysHost
  @param[in] socket   - Processor to initialize
  @param[in] Ch       - Channel number

  @retval N/A

**/
VOID
SetupCapWrCrcErrorFlowDdrio (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      UINT8       Ch
  )
{
  //
  // Stub function to satisfy API.
  //

  RC_FATAL_ERROR (FALSE, 0, 0);

  return;

} // SetupCapWrCrcErrorFlowDdrio

/**
  Set/Restore Long Read Preamble in DDRIO

  @param[in] Host         - Point to sysHost
  @param[in] Socket       - Socket number
  @param[in] Ch           - Channel number
  @param[in] Set          - if TRUE: Set Long Read Preamble
                            else:    Restore original Read Preamble state

  @retval N/A
**/
VOID
SetLongReadPreambleDdrio (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      BOOLEAN  Set
  )
{
  //
  // Stub function to satisfy API.
  //

  RC_FATAL_ERROR (FALSE, 0, 0);

  return;

} // SetupCapWrCrcErrorFlowDdrio

/**
  Disables channels in DDRIO hardware that are unsupported by the platform

  @param[in]  Socket    Socket number to disable unused channels for

  @retval EFI_SUCCESS   Unsupported channels successfully disabled
  @retval !EFI_SUCCESS  An error occurred during the disabling of unsupported channels
**/
EFI_STATUS
EFIAPI
DisableUnsupportedChannelsDdrio (
  IN  UINT8   Socket
  )
{

  //
  // SPR does not yet support the DDRCRINTFPMCONFIG0 register, so there is not support for disabling
  // DDRIO channels. This stub is here to fulfill API requirements and will be populated
  // the register is supported.
  //

  return EFI_SUCCESS;
} // DisableUnsupportedChannelsDdrio

/**
  Return the address for the DDRINTF_CTL0 register for socket/channel/subchannel

  @param[in]  Socket        - Socket number
  @param[in]  Ch            - Channel number - socket based
  @param[in]  SubCh         - Sub channel number
  @param[out] Address       - Pointer to callers' register address buffer

  @retval   MRC_STATUS_SUCCESS if register address is returned
  @retval   MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
GetDdrintfCtl0RegAddress (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  IN  UINT32    *Address
  )
{
  UINT8                                 RegIndex;
  UINT8                                 NumChPerMc;
  UINT32                                DdrintfCtl0ChReg[] = { DDRINTF_CTL0_CH_0_MCIO_DDRIO_REG,   //sub ch 0a
                                                               DDRINTF_CTL0_CH_1_MCIO_DDRIO_REG,   //sub ch 0b
                                                               DDRINTF_CTL0_CH_2_MCIO_DDRIO_REG,   //sub ch 1a
                                                               DDRINTF_CTL0_CH_3_MCIO_DDRIO_REG    //sub ch 1b
                                                             };

  NumChPerMc = GetNumChannelPerMc ();

  RegIndex = (Ch % NumChPerMc) * SUB_CH + SubCh;
  if ((ARRAY_SIZE (DdrintfCtl0ChReg) <= RegIndex) || (Address == NULL)){
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_114);
    return MRC_STATUS_FAILURE;
  }
  *Address = DdrintfCtl0ChReg[RegIndex];
  return MRC_STATUS_SUCCESS;
}

/**
  Return the cached value of the DDRINTF_CTL0 register for the channel/subchannel

  @param[in]  Host          - Pointer to SYSHOST
  @param[in]  Socket        - Socket number
  @param[in]  Ch            - Channel number - socket based
  @param[in]  SubCh         - Sub channel number
  @param[out] DdrintfCtl0   - Pointer to register value

  @retval   MRC_STATUS_SUCCESS if register value is returned
  @retval   MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
GetDdrintfCtl0 (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  OUT UINT32    *DdrintfCtl0
  )
{
  DDRIO_CHANNEL_NVDATA                  (*DdrioChannelNvList) [MAX_CH];

  if ((Ch >= MAX_CH) || (SubCh >= SUB_CH) || (DdrintfCtl0 == NULL)) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_113);
    return MRC_STATUS_FAILURE;
  }

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  *DdrintfCtl0 = (*DdrioChannelNvList)[Ch].SubChannel[SubCh].DdrintfCtl0Ch;
  return MRC_STATUS_SUCCESS;
}

/**
  Program the DDRINTF_CTL0 register for the channel/subchannel
  Update the cache.

  @param[in]  Host          - Pointer to SYSHOST
  @param[in]  Socket        - Socket number
  @param[in]  Ch            - Channel number - socket based
  @param[in]  SubCh         - Sub channel number
  @param[in]  DdrintfCtl0   - Register value

  @retval   MRC_STATUS_SUCCESS if register value is returned
  @retval   MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
SetDdrintfCtl0 (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  IN  UINT32    DdrintfCtl0
  )
{
  UINT32                                Address;
  DDRIO_CHANNEL_NVDATA                  (*DdrioChannelNvList) [MAX_CH];

  if ((Ch >= MAX_CH) || (SubCh >= SUB_CH)) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_112);
    return MRC_STATUS_FAILURE;
  }

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  GetDdrintfCtl0RegAddress (Socket, Ch, SubCh, &Address);

  (*DdrioChannelNvList)[Ch].SubChannel[SubCh].DdrintfCtl0Ch = DdrintfCtl0;
  MemWritePciCfgEp (Socket, Ch, Address, DdrintfCtl0);
  return MRC_STATUS_SUCCESS;
}

/**
  Disables the specified channels in the DDRIO

  @param[in]  Socket        Socket number
  @param[in]  McId          Memory controller ID
  @param[in]  ChDisableMask Bit mask indicating which channels on memory controller McId to disable
                            Bit 0 = McId CH0, Bit 1 = McId CH1, etc

  @retval   EFI_SUCCESS   Operation was successful
  @retval   EFI_INVALID_PARAMETER if input is invalid

**/
EFI_STATUS
EFIAPI
DisableChannelsDdrio (
  IN  UINT8     Socket,
  IN  UINT8     McId,
  IN  UINT32    ChDisableMask
  )
{
  PSYSHOST                              Host;
  UINT8                                 ChGlobal;
  UINT8                                 ChLocal;
  UINT8                                 SubCh;
  UINT8                                 NumChPerMc;
  UINT8                                 MaxChDdr;
  DDRINTF_CTL0_CH_0_MCIO_DDRIO_STRUCT   DdrintfCtl0;

  Host = GetSysHostPointer ();
  MaxChDdr   = GetMaxChDdr ();
  NumChPerMc = GetNumChannelPerMc ();

  for (ChLocal = 0; ChLocal < NumChPerMc; ChLocal++) {
    ChGlobal = ChLocal + NumChPerMc * McId;
    if ((ChDisableMask & (1 << ChLocal)) != 0) {
      for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {

        if (GetDdrintfCtl0 (Host, Socket, ChGlobal, SubCh, &(DdrintfCtl0.Data)) != MRC_STATUS_SUCCESS) {
          return EFI_INVALID_PARAMETER;
        }
        DdrintfCtl0.Bits.ch_dis = 1;
        SetDdrintfCtl0 (Host, Socket, ChGlobal, SubCh, DdrintfCtl0.Data);

      } // SubCh
    } // ChDisableMask
  } // ChLocal

  return EFI_SUCCESS;
}

/**

  Get/Set FIFO Point Control bit

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       Mode        - GSM_READ_ONLY - Do not write
                                - GSM_FORCE_WRITE - Force the write
  @param [in,out]   Value       - IN this value is used to set/clear fifoptctl bit
                                - OUT contains the current value of the fifoptctl bit

  @retval EFI_SUCESS

**/
EFI_STATUS
GetSetFifoPointControl (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    Mode,
  IN OUT  UINT8    *Value
)
{
  return EFI_SUCCESS;
}

/**
  Program Cmd Ctl Output Enable

  @param[in]              Host    Pointer to sysHost
  @param[in]              Socket  Current socket
  @param[in]              Channel Channel to program registers for
  @param[in]              Command Indicates whether to get, enable, or disable output and on which signals
  @param[in, out] Buffer  Buffer to store current enable settings to restore later

  @retval N/A
**/
VOID
CmdCtlOutputEnable (
  IN      PSYSHOST        Host,
  IN      UINT8           Socket,
  IN      UINT8           Channel,
  IN      CMD_CTL_OUT_EN  Command,
  IN OUT  UINT64          *Buffer
  )
{
  return;
}

/**

Training Mode Selection. It supports CS/CA CLK training.

@param[in]  MEMFLOWS       - Training Step
@param[out] TrainingMode   - Training Mode. 1: In parallel. 0: In serial.

@retval MRC_STATUS_SUCCESS  - Select training mode Successfully
        MRC_STATUS_FAILURE  - Fail to set training mode

**/
MRC_STATUS
EFIAPI
TrainingModeSelect (
  IN     MEMFLOWS   Trainingstep,
     OUT UINT8      *TrainingMode
  )
{
  if ((Trainingstep == EarlyCsClkTraining) ||
      (Trainingstep == CaClkTraining) ||
      (Trainingstep == CaClkTrainingBackSide) ||
      (Trainingstep == CaSlewRateTraining) ||
      (Trainingstep == CmdVrefCenteringTraining) ||
      (Trainingstep == DcaDckDutyCycleTraining) ||
      (Trainingstep == RcdDcaDfeDdr5)) {
#if defined(SPR_PO_FLAG) || defined(GNRSRF_HOST)
    *TrainingMode = RUN_IN_SERIAL;
#else
    *TrainingMode = RUN_IN_PARALLEL;
#endif
    return EFI_SUCCESS;
  } else {
    *TrainingMode = TRAINING_MODE_NOT_SUPPORT;
    return MRC_STATUS_FAILURE;
  }
}

/**

  Gets the maximum latency of DDRIO write plus read per channel. This value is used
  by the MC as a delay between write and read commands that hit a PCLS line.

  @param[in]  Socket         - Socket index at system level
  @param[in]  Ch             - Channel index at socket level
  @param[out] *Delay         - DDRIO write plus read latency in DCLK units

  @retval EFI_SUCCESS        - Delay value was updated
  @reval  EFI_UNSUPPORTED    - Delay value not updated

**/
EFI_STATUS
EFIAPI
GetPclsCaptureDelayDddrio (
  UINT8   Socket,
  UINT8   Ch,
  UINT32  *Delay
)
{
  return EFI_UNSUPPORTED;

} // GetPclsCaptureDelayDddrio

  /**

  Set the training mode to latch the training result.

  @param[in] Host  - Pointer to sysHost
  @param[in] socket  - Socket number
  @param[in] mode  - Training mode

  @retval N/A

  **/
VOID
SetTrainingModeLatchResult (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Mode
  )
{
  return;
} // SetTrainingModeLatchResult

/*

  Enforces the maximum gap between bits within the BYTE, and if enabled also enforces the nibbles to be in the same TxDq UI
  Warning: This function must be called during 80b mode or Basic training execution

  @param[in]  Socket    Current socket under test (0-based)
  @param[in]  Ch        Current channel under test (0-based)
  @param[in]  SubCh     Current subchannel under test (0-based)
  @param[in]  Dimm      Current dimm under test (0-based)
  @param[in]  Rank      Current rank under test (0-based)
  @param[in]  PiPosition  Pointer with the information of each center position across ALL bits
  @param[in]  ResultsBit  Pointer with the margining information of each of bit
  @param[in]  UpdateTxDqsOriginalValue  TRUE: After executing the WA (*ChannelNvList)[ChannelLocal].TxDqsDelay is updated with the new value
                                        FALSE: (*ChannelNvList)[ChannelLocal].TxDqsDelay is not modified

*/
VOID
TxDqWA (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     SubCh,
  UINT8     Dimm,
  UINT8     Rank,
  PerBitPi  *PiPosition,
  IN struct bitMarginCh  *ResultsBit,   OPTIONAL
  BOOLEAN   UpdateTxDqsOriginalValue
  )
{
} // TxDqWA

/*

  Enforces the maximum gap between bits within the BYTE, and if enabled also enforces the nibbles to be in the same TxDq UI
  Warning: This function must be called during 40b mode or Adv training execution

  @param[in]  Socket    Current socket under test (0-based)
  @param[in]  Ch        Current channel under test (0-based)
  @param[in]  SubCh     Current subchannel under test (0-based)
  @param[in]  Dimm      Current dimm under test (0-based)
  @param[in]  Rank      Current rank under test (0-based)
  @param[in]  UpdateTxDqsOriginalValue  TRUE: After executing the WA (*ChannelNvList)[ChannelLocal].TxDqsDelay is updated with the new value
                                        FALSE: (*ChannelNvList)[ChannelLocal].TxDqsDelay is not modified

*/
VOID
TxDqWaAdv (
  IN        UINT8     Socket,
  IN        UINT8     Ch,
  IN        UINT8     SubCh,
  IN        UINT8     Dimm,
  IN        UINT8     Rank,
  IN        BOOLEAN   UpdateTxDqsOriginalValue
  )
{
}

/**
  Perform any silicon specific initialization required before Recieve Enable Training

  @param[in, out] ** ChipDataPtr - A pointer to a pointer can be used to pass data

  @retval EFI_SUCCESS if init done
          !EFI_SUCCESS if other errors were encountered
**/

EFI_STATUS
EFIAPI
SaveAndInitChipRxEnableParams (
  IN OUT VOID                               **ChipDataPtr
  )
{
  *ChipDataPtr = NULL;
  return EFI_SUCCESS;
}

/**
  Perform any silicon specific initialization required after Recieve Enable Training

  @param[in, out] ** ChipDataPtr - A pointer to a pointer can be used to pass data

  @retval EFI_SUCCESS if init done
          !EFI_SUCCESS if other errors were encountered
**/
EFI_STATUS
EFIAPI
RestoreChipRxEnableParams (
  IN OUT VOID                               **ChipDataPtr
  )
{
  return EFI_SUCCESS;
}

/**
  Initialize Tx Echo Cancellation feature.

  @param[in] Socket   - Socket number

  @retval  N/A
**/
VOID
EFIAPI
InitTxEchoCancellation (
  IN UINT8 Socket
 )
{
  //NULL Implementation
}

/**
  Initialize Legacy RxDfe feature.

  @param[in] Socket   - Socket number

  @retval  N/A
**/
VOID
EFIAPI
InitLegacyRxDfe (
  IN UINT8 Socket
 )
{
  //NULL Implementation
}

/**
  Ensure TX DQS delays in a byte are within MAX_DQ_GAP ticks of each other

  @param[in] Socket     - Socket number

  @reval    N/A
**/
VOID
TxDqsWA (
  UINT8     Socket
  )
{
  return;
} // TxDqsWA

/**
  Initializes all instances of VssHiOrVrefControl.vsshiorvrefctl

  Not required for this platform.

  @param[in]  Host    Pointer to sysHost
  @param[in]  Socket  Current socket
**/
VOID
InitVrefControls (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  return;
}


/**

Set Vsshi before MMRC

@param[in] Socket    - Current Socket

@retval N/A

**/
VOID
SetVsshi (
  IN UINT8     Socket
  )
{
  UINT8                                            Ch;
  UINT8                                            SubCh;
  UINT8                                            Strobe;
  DDRIO_CHANNEL_NVDATA                             (*DdrioChannelNvList) [MAX_CH];
  DDRCC_VSSHI_CTL_MCIO_DDRIOEXT_STRUCT             DdrccVsshiCtl;
  DDRD_N0_VSSHI_CTL_MCIO_DDRIO_STRUCT              DdrdN0VsshiCtl;
  DDRCC_DLL_CSR_0_MCIO_DDRIOEXT_STRUCT             DdrccDllCsr0;
  DDRD_N0_DLL_CSR_MCIO_DDRIO_STRUCT                DdrdN0DllCsr;
  UINT8                                            MSVx4;
  UINT8                                            MaxChDdr;
  UINT8                                            Value = 0;
  PSYSHOST                                         Host;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  MSVx4 = GetMaxStrobeValid (Host);
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Firstly, read the analog Tuning value
    //
    DdrdN0DllCsr.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, 0, DDRD_N0_DLL_CSR_MCIO_DDRIO_REG));
    if (DdrdN0DllCsr.Bits.analog_tuning == 0) {
      Value = 3;
    } else if ((DdrdN0DllCsr.Bits.analog_tuning == 1) || (DdrdN0DllCsr.Bits.analog_tuning == 2)) {
      Value = 1;
    } else if (DdrdN0DllCsr.Bits.analog_tuning == 3) {
      Value = 0;
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Error: analog_tuning value %d is not valid \n",DdrdN0DllCsr.Bits.analog_tuning);
    }
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      //
      // To be safe, program all the nibbles.
      //

      DdrdN0VsshiCtl.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_VSSHI_CTL_MCIO_DDRIO_REG));
      DdrdN0VsshiCtl.Bits.vsshi_bias_ctl = Value;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_VSSHI_CTL_MCIO_DDRIO_REG), DdrdN0VsshiCtl.Data);
    } // Strobe loop

    //
    // Program vsshi_bias_ctl in CC fub.
    //
    DdrccDllCsr0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SUB_CH_A, DDRCC_DLL_CSR_0_MCIO_DDRIOEXT_REG);

    if (DdrccDllCsr0.Bits.analog_tuning == 0) {
      Value = 3;
    } else if ((DdrccDllCsr0.Bits.analog_tuning == 1) || (DdrccDllCsr0.Bits.analog_tuning == 2)) {
      Value = 1;
    } else if (DdrccDllCsr0.Bits.analog_tuning == 3) {
      Value = 0;
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Error: analog_tuning value %d is not valid \n",DdrccDllCsr0.Bits.analog_tuning);
    }

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

      DdrccVsshiCtl.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_VSSHI_CTL_MCIO_DDRIOEXT_REG);
      DdrccVsshiCtl.Bits.vsshi_bias_ctl = Value;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_VSSHI_CTL_MCIO_DDRIOEXT_REG, DdrccVsshiCtl.Data);
    } //Subch loop
  } // Ch loop

  //
  // Program vsshi_target_code.
  //

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      //
      // To be safe, program all the nibbles.
      //
      DdrdN0VsshiCtl.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_VSSHI_CTL_MCIO_DDRIO_REG));
      DdrdN0VsshiCtl.Bits.vsshi_target_code = 0x28;
      DdrdN0VsshiCtl.Bits.ldo_bw_ctl = 1;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_VSSHI_CTL_MCIO_DDRIO_REG), DdrdN0VsshiCtl.Data);
    }//strobe loop


    //
    // Program vsshi_bias_ctl in CC fub.
    //

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

      DdrccVsshiCtl.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_VSSHI_CTL_MCIO_DDRIOEXT_REG);
      DdrccVsshiCtl.Bits.vsshi_target_code = 0x28;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_VSSHI_CTL_MCIO_DDRIOEXT_REG, DdrccVsshiCtl.Data);
    } //subch loop

  }//Ch loop

  //
  // Program vsshi_close_loop
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Program for data fubs.
    //
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      DdrdN0VsshiCtl.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_VSSHI_CTL_MCIO_DDRIO_REG));
      DdrdN0VsshiCtl.Bits.vsshi_close_loop = 1;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_VSSHI_CTL_MCIO_DDRIO_REG), DdrdN0VsshiCtl.Data);
    } //strobe loop

    //
    // Program for CC fubs.
    //
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      DdrccVsshiCtl.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_VSSHI_CTL_MCIO_DDRIOEXT_REG);
      DdrccVsshiCtl.Bits.vsshi_close_loop = 1;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_VSSHI_CTL_MCIO_DDRIOEXT_REG, DdrccVsshiCtl.Data);
    } //subch loop


  } //Ch loop
  //
  // If this is a CTE build, delay 50 QCLK
  //
  CteDelayQclk (50);

  return;
} // SetVsshi


/**

  Configure Pin Io direction.

  @param[in] Socket - Socket Id

  @retval  None

**/
VOID
ConfigurePinIodirection (
  IN UINT8                     Socket
  )
{
  PSYSHOST                                     Host;
  UINT8                                        Ch;
  DDRCC_CTL0_BIT_0_MCIO_DDRIOEXT_STRUCT        DdrccCtl0Bit0;
  UINT8                                        SubCh;
  UINT8                                        PinIndex = 0;
  UINT8                                        Value = 0;
  DDRCC_TX_CTL1_MCIO_DDRIOEXT_STRUCT           DdrccTxCtl1;
  DDRCLK_TX_CTL1_MCIO_DDRIOEXT_STRUCT          DdrclkTxCtl1;
  struct dimmNvram                             (*DimmNvList)[MAX_DIMM];
  UINT32                                       CaPinPiRegs[] = { DDRCC_CTL0_BIT_0_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_1_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_2_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_3_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_4_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_5_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_6_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_7_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_8_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_9_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_10_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_11_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_12_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_13_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_14_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_15_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_16_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_17_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_18_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_19_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_20_MCIO_DDRIOEXT_REG,
                                                                 DDRCC_CTL0_BIT_21_MCIO_DDRIOEXT_REG
                                                               };
  UINT8                                        MaxChDdr;
  DDRIO_CHANNEL_NVDATA                         (*DdrioChannelNvList) [MAX_CH];

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    //
    // Set the IO direction as input.
    //
    for (PinIndex = 0; PinIndex < MAX_CC_PIN_NUM_SUBCH; PinIndex ++) {
      if ((PinIndex == 0) || (PinIndex == 15) || (PinIndex == 16)) {
        Value = 0;
      } else {
        Value = 1;
      }

      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        if ((PinIndex == 0) && (SubCh == 1)) {
          if (((*DimmNvList) [0].DcpmmPresent == 1) || ((*DimmNvList) [1].DcpmmPresent == 1)) {
            Value = 0;
          } else {
            Value = 1;
          }
        }

        DdrccCtl0Bit0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, CaPinPiRegs[PinIndex]);
        DdrccCtl0Bit0.Bits.io_dir = Value;
        WriteDdrioCcRegister (Host, Socket, Ch, SubCh, CaPinPiRegs[PinIndex], DdrccCtl0Bit0.Data);
        //
        // Update the cache
        //
        (*DdrioChannelNvList)[Ch].CmdCsr[PinIndex][SubCh] = DdrccCtl0Bit0.Data;
      }
    }
  }
  //
  // Program the io_dir_valid
  //

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

      DdrccTxCtl1.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_TX_CTL1_MCIO_DDRIOEXT_REG);
      DdrccTxCtl1.Bits.io_dir_valid = 1;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_TX_CTL1_MCIO_DDRIOEXT_REG, DdrccTxCtl1.Data);
    }

    DdrclkTxCtl1.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_TX_CTL1_MCIO_DDRIOEXT_REG);
    DdrclkTxCtl1.Bits.io_dir_valid = 1;
    MemWritePciCfgEp (Socket, Ch, DDRCLK_TX_CTL1_MCIO_DDRIOEXT_REG, DdrclkTxCtl1.Data);
  }

} //ConfigurePinIodirection

/**

Disable Tx Retrain init.

@param[in] socket             - Current Socket

@retval N/A

**/
VOID
DisTxRtInit (
  IN UINT8     Socket
  )
{
  UINT8                                       Ch;
  UINT8                                       Strobe;
  UINT8                                       MSVx4;
  UINT8                                       LogRank;
  DDRD_N0_TX_RETRAIN_RANK_0_MCIO_DDRIO_STRUCT DddrdN0TxReTrainRank0;
  struct channelNvram                         (*ChannelNvList) [MAX_CH];
  struct dimmNvram                            (*DimmNvList) [MAX_DIMM];
  struct ddrRank                              (*RankList) [MAX_RANK_DIMM];
  UINT8                                       MaxChDdr;
  DDRD_N0_TX_RETRAIN0_MCIO_DDRIO_STRUCT       DdrdN0TxRetrain0;
  UINT8                                       Dimm;
  UINT8                                       Rank;
  PSYSHOST                                    Host;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  MSVx4 = GetMaxStrobeValid (Host);
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      DdrdN0TxRetrain0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TX_RETRAIN0_MCIO_DDRIO_REG));
      DdrdN0TxRetrain0.Bits.txrt_init = 0;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TX_RETRAIN0_MCIO_DDRIO_REG), DdrdN0TxRetrain0.Data);
    } // strobe loop

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList) [Dimm].dimmPresent == 0) {
        continue;
      }

      RankList = GetRankNvList (Host, Socket, Ch, Dimm);

      for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        //
        // Get the logical rank #
        //
        LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

        for (Strobe = 0; Strobe < MSVx4; Strobe++) {
          if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
            continue;
          }
          DddrdN0TxReTrainRank0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, LogRank, Strobe, DDRD_N0_TX_RETRAIN_RANK_0_MCIO_DDRIO_REG));
          if (DddrdN0TxReTrainRank0.Bits.txrt_init_picode != 0) {

            DddrdN0TxReTrainRank0.Bits.txrt_init_picode = 0;
            MemWritePciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, LogRank, Strobe, DDRD_N0_TX_RETRAIN_RANK_0_MCIO_DDRIO_REG), DddrdN0TxReTrainRank0.Data);
          }
        } // strobe loop
      }
    }

  } //Ch loop
} // DisTxRtInit

/**

  Save the dataOffsetTrain value for subsequent boots.

  @param[in]  Host        - Pointer to sysHost

  @retval None

**/
VOID
EFIAPI
DataOffsetTrainSave (
  IN PSYSHOST          Host
  )
{

}


/**

  Get Set TxDq Pie Offset Per Bit

  @param[in] Host    - Pointer to sysHost
  @param[in] socket  - Socket number
  @param[in] ch      - Channel number (0-based)
  @param[in] dimm    - DIMM number (0-based)
  @param[in] rank    - Rank number (0-based)
  @param[in] strobe  - Strobe number (0-based)
  @param[in] bit     - Bit number
  @param[in] level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in] group   - CSM_GT - Parameter to program
  @param[in] mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param[in out] value  - Pointer data to read or write. For writes, must point to an array of values for every bit in the nibble

  @retval MRC_STATUS

**/
STATIC
MRC_STATUS
GetSetTxDqPieOffsetBit (
  IN PSYSHOST  Host,
  IN UINT8     socket,
  IN UINT8     ch,
  IN UINT8     dimm,
  IN UINT8     rank,
  IN UINT8     strobe,
  IN UINT8     bit,
  IN MRC_LT    level,
  IN MRC_GT    group,
  IN UINT8     mode,
  IN OUT INT16 *value
  )
{
  UINT8                               logRank;
  UINT8                               maxStrobe;
  UINT8                               SwizzleBit;
  UINT8                               piValBit[BITS_PER_NIBBLE];
  UINT16                              MinValBit;
  INT16                               tmp[BITS_PER_NIBBLE];
  UINT16                              maxLimit = 0;
  UINT16                              minLimit = 0;
  UINT16                              usDelay = 0;
  UINT8                               MaxBits;
  UINT8                               MinBits;
  DDRIO_CHANNEL_NVDATA                (*DdrioChannelNvList) [MAX_CH];
  DDRD_N0_TXDQ_PIE_OFFSET_RANK_0_MCIO_DDRIO_STRUCT DdrdN0TxDqPieOffset;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  DdrioChannelNvList = GetDdrioChannelNvList (Host, socket);

  DdrdN0TxDqPieOffset.Data = 0;
  //
  // Get the logical rank #
  //
  logRank = GetLogicalRank (Host, socket, ch, dimm, rank);

  ZeroMem (tmp, sizeof (tmp));

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits (Host, level, group, &minLimit, &maxLimit, &usDelay);

  if (strobe >= MAX_STROBE && strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
      "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((strobe < MAX_STROBE || strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (strobe == ALL_STROBES) {
    strobe = 0;
    maxStrobe = MSVx4;
  } else {
    maxStrobe = strobe + 1;
  }
  //
  // bit must be either 0-3 or ALL_BITS, nothing else is supported
  //
  RC_FATAL_ERROR (bit < BITS_PER_NIBBLE || bit == ALL_BITS, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_56);
  if (bit == ALL_BITS) {
    MinBits = 0;;
    MaxBits = BITS_PER_NIBBLE;
  } else {
    MinBits = bit;
    MaxBits = bit + 1;
  }
  //
  // If the MinBits >= 4, fata error here.
  //
  RC_FATAL_ERROR (MinBits < BITS_PER_NIBBLE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_56);

  for ( ; strobe < maxStrobe; strobe++) {
    //
    // Read from PCI config space
    //
    DdrdN0TxDqPieOffset.Data = MemReadPciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_TXDQ_PIE_OFFSET_RANK_0_MCIO_DDRIO_REG));

    piValBit[0] = (UINT8) DdrdN0TxDqPieOffset.Bits.txdq_pie_offset0;
    piValBit[1] = (UINT8) DdrdN0TxDqPieOffset.Bits.txdq_pie_offset1;
    piValBit[2] = (UINT8) DdrdN0TxDqPieOffset.Bits.txdq_pie_offset2;
    piValBit[3] = (UINT8) DdrdN0TxDqPieOffset.Bits.txdq_pie_offset3;

    MinValBit = MAX_INT16;
    for (bit = MinBits; bit < MaxBits; bit++) {
      //
      // Get Swizzle Bit
      //
      SwizzleBit = GetDqSwizzle (ch, strobe, bit, level, group);
      //
      // Check if read only
      //
      if (mode & GSM_READ_ONLY) {
        //
        // Save the value read into the output parameter
        // When reading the entire nibble, find the minimum value in the nibble
        //
        if (piValBit[SwizzleBit] < MinValBit) {
          MinValBit = piValBit[SwizzleBit];
        }
        *value = MinValBit;

      } else {
        //
        // Write
        //
        // Adjust the current delay value by offset
        if (mode & GSM_WRITE_OFFSET) {
          //
          // Add the offset to the current value
          //
          tmp[SwizzleBit] = piValBit[SwizzleBit] + value[bit];

        } else {
          //
          // Write absolute value
          //
          tmp[SwizzleBit] = value[bit];
        }

        //
        // Make sure we do not exeed the limits
        //
        if (tmp[SwizzleBit] < minLimit) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, bit,
            "%a Out of range!! tmp[%d] = 0x%x, minLimit = 0x%x\n", GetGroupStr (group), bit, tmp[SwizzleBit], minLimit);
          tmp[SwizzleBit] = minLimit;
        }
        // Ensure we do not exceed maximums
        if (tmp[SwizzleBit] > maxLimit) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, bit,
            "%a Out of range!! tmp[%d] = 0x%x, maxLimit = 0x%x\n", GetGroupStr (group), bit, tmp[SwizzleBit], maxLimit);
          tmp[SwizzleBit] = maxLimit;
        }
      }
    }//bit

    //
    // Update CSR settings if not read only
    //
    if ((mode & GSM_READ_ONLY) == 0) {
      DdrdN0TxDqPieOffset.Bits.txdq_pie_offset0 = tmp[0];
      DdrdN0TxDqPieOffset.Bits.txdq_pie_offset1 = tmp[1];
      DdrdN0TxDqPieOffset.Bits.txdq_pie_offset2 = tmp[2];
      DdrdN0TxDqPieOffset.Bits.txdq_pie_offset3 = tmp[3];

      //
      // Write it back
      //

      MemWritePciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_TXDQ_PIE_OFFSET_RANK_0_MCIO_DDRIO_REG), DdrdN0TxDqPieOffset.Data);
      //
      // Wait for the new value to settle
      //
      FixedDelayMicroSecond (usDelay);
    } // if not read only

    //
    // As the txdq_pie_offset is only programmed one time during the duty cycle correction,
    // no need to waste NEM space to do the unbenefited CSR cache.
    //
    if (mode & GSM_UPDATE_CACHE) {
      RcDebugPrintWithDevice (SDBG_ERROR, socket, ch, dimm, NO_SUBCH, rank, strobe, bit,
        "The GSM_UPDATE_CACHE mode is not supported in GetSetTxDqPieOffsetBit\n");
    }
  } // strobe loop
  return MRC_STATUS_SUCCESS;
} // GetSetTxDqPieOffsetBit

/**
  Initialize Chip Specific Tx DQ Scomp.

  @param[in] Host     - Pointer to sysHost, the system Host (root) structure
  @param[in] socket   - Socket number
  @param[in] ch       - Channel number
  @param[in] SubCh    - Sub channel number
  @param[in] Strobe   - Strobe number

  @retval         - EFI_SUCCESS

**/
MRC_STATUS
EFIAPI
InitializeDataScompChip (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  IN  UINT8     Strobe
  )
{
  DDRD_N0_COMP_OFFSET_MCIO_DDRIO_STRUCT DdrDataCompOffset;
  DDRD_N0_COMP_LA1_MCIO_DDRIO_STRUCT    DdrDataCompLa1;
  DDRIO_CHANNEL_NVDATA                  (*DdrioChannelNvList) [MAX_CH];

  Strobe = STROBE_INDEX_SUBCH(Strobe,SubCh);

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  DdrDataCompOffset.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_COMP_OFFSET_MCIO_DDRIO_REG));
  DdrDataCompOffset.Bits.scomp_up_adj = 0;
  DdrDataCompOffset.Bits.scomp_dn_adj = 0;
  MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_COMP_OFFSET_MCIO_DDRIO_REG), DdrDataCompOffset.Data);

  DdrDataCompLa1.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_COMP_LA1_MCIO_DDRIO_REG));
  DdrDataCompLa1.Bits.scomp = 0;
  MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_COMP_LA1_MCIO_DDRIO_REG), DdrDataCompLa1.Data);

  DoCompUpdate (Host, Socket, GetMCID (Host, Socket, Ch));

  //
  // Initialize cache.
  //
  ((*DdrioChannelNvList) [Ch]).DdrDataCompOffset[Strobe] = DdrDataCompOffset.Data;
  ((*DdrioChannelNvList) [Ch]).DdrDataCompLa1[Strobe] = DdrDataCompLa1.Data;

  return MRC_STATUS_SUCCESS;
}

/**
  Restore Tx DQ Slew Rate comp of channel.

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param Channel  - Channel number

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
EFIAPI
RestoreDqSlewRate (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  DDRD_N0_COMP_OFFSET_MCIO_DDRIO_STRUCT DdrDataCompOffset;
  DDRD_N0_COMP_LA1_MCIO_DDRIO_STRUCT    DdrDataCompLa1;
  DDRIO_CHANNEL_NVDATA                  (*DdrioChannelNvList) [MAX_CH];
  UINT8                                  Strobe;
  UINT8                                 MSVx4 = GetMaxStrobeValid(Host);

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  for (Strobe = 0; Strobe < MSVx4; Strobe++) {

    DdrDataCompOffset.Data = (*DdrioChannelNvList) [Channel].DdrDataCompOffset[Strobe];
    MemWritePciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_COMP_OFFSET_MCIO_DDRIO_REG), DdrDataCompOffset.Data);

    DdrDataCompLa1.Data = (*DdrioChannelNvList) [Channel].DdrDataCompLa1[Strobe];
    MemWritePciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_COMP_LA1_MCIO_DDRIO_REG), DdrDataCompLa1.Data);
  }

  DoCompUpdate (Host, Socket, GetMCID (Host, Socket, Channel));

  return MRC_STATUS_SUCCESS;
}

/**

 Get Set Rx Dfe Coefficient Tap Pe rBit

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number. When mode is GSM_FORCE_WRITE, only supports ALL_BITS.
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program. When mode is GSM_FORCE_WRITE, value must be an array of size BITS_PER_NIBBLE, which contains
                   different values for different bits in the nibble.

  @retval IO delay

**/
MRC_STATUS
GetSetRxDfeCoefficientTapPerBit (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT8                               logRank;
  UINT8                               maxStrobe;
  UINT8                               i;
  INT16                               tmp;
  UINT16                              curVal[BITS_PER_NIBBLE];
  UINT16                              piDelayNibble = 0;
  UINT16                              piDelay;
  UINT16                              piDelayMin;
  UINT16                              piDelayMax;
  UINT16                              delta;
  UINT16                              piDelayBit[BITS_PER_NIBBLE];
  UINT16                              maxLimit = 0;
  UINT16                              minLimit = 0;
  UINT16                              maxPerBitLimit = 0;
  UINT16                              minPerBitLimit = 0;
  UINT16                              usDelay = 0;
  DDRIO_CHANNEL_NVDATA                (*DdrioChannelNvList) [MAX_CH];
  struct ddrRank                      (*RankList) [MAX_RANK_DIMM];
  DDRD_N0_DFE_COEFF_OFFSET0_RANK_0_MCIO_DDRIO_STRUCT DdrdDfeCoeffOffset0;
  DDRD_N0_DFE_COEFF_OFFSET1_RANK_0_MCIO_DDRIO_STRUCT DdrdDfeCoeffOffset1;
  DDRD_N0_DFE_COEFF_RANK_0_MCIO_DDRIO_STRUCT         DdrdDfeCoeff;
  UINT8                               MSVx4 = GetMaxStrobeValid (Host);
  UINT8                               SwizzleBit;

  DdrioChannelNvList = GetDdrioChannelNvList (Host, socket);
  RankList = GetRankNvList (Host, socket, ch, dimm);
  DdrdDfeCoeffOffset0.Data = 0;
  DdrdDfeCoeffOffset1.Data = 0;
  DdrdDfeCoeff.Data = 0;
  //
  // Get the logical rank #
  //
  logRank = GetLogicalRank (Host, socket, ch, dimm, rank);

  //
  // Get minimum and maximum value for the overall RxDqDqsDelay, and the per bit Delays
  //
  GetDataGroupLimits (Host, level, RxDqsDelay, &minLimit, &maxLimit, &usDelay);
  GetDataGroupLimits (Host, level, group, &minPerBitLimit, &maxPerBitLimit, &usDelay);

  if (strobe >= MAX_STROBE && strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
      "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((strobe < MAX_STROBE || strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (strobe == ALL_STROBES) {
    strobe = 0;
    maxStrobe = MSVx4;
  } else {
    maxStrobe = strobe + 1;
  }

  if ((mode & GSM_READ_ONLY) == GSM_READ_ONLY) {
    //
    // For reads, specific bit must be specified
    //
    if (bit >= BITS_PER_NIBBLE) {
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_61);
      return MRC_STATUS_FAILURE;
    }
  } else {
    //
    // For writes, must specify the entire nibble (ALL_BITS)
    //
    RC_FATAL_ERROR (bit == ALL_BITS, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_163);
  }

  for (; strobe < maxStrobe; strobe++) {

    if (mode & GSM_READ_CSR) {
      //
      // Read from PCI config space
      //
      DdrdDfeCoeffOffset0.Data = MemReadPciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_DFE_COEFF_OFFSET0_RANK_0_MCIO_DDRIO_REG));
      DdrdDfeCoeffOffset1.Data = MemReadPciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_DFE_COEFF_OFFSET1_RANK_0_MCIO_DDRIO_REG));
      DdrdDfeCoeff.Data = MemReadPciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_DFE_COEFF_RANK_0_MCIO_DDRIO_REG));
    } else {
      //
      // Read from cache
      //
      DdrdDfeCoeffOffset0.Data = (*DdrioChannelNvList) [ch].DfeCoeffOffset0[logRank][strobe];
      DdrdDfeCoeffOffset1.Data = (*DdrioChannelNvList) [ch].DfeCoeffOffset1[logRank][strobe];
      DdrdDfeCoeff.Data = (*DdrioChannelNvList) [ch].DfeCoeff[logRank][strobe];

    }

    switch (group) {
      case RxDfeCoefficientTap1PerBit:
        piDelayNibble = (UINT16) DdrdDfeCoeff.Bits.dfe_coeff1;
        piDelayBit[0] = (UINT16) DdrdDfeCoeffOffset0.Bits.dfe_c1_offset0;
        piDelayBit[1] = (UINT16) DdrdDfeCoeffOffset0.Bits.dfe_c1_offset1;
        piDelayBit[2] = (UINT16) DdrdDfeCoeffOffset0.Bits.dfe_c1_offset2;
        piDelayBit[3] = (UINT16) DdrdDfeCoeffOffset0.Bits.dfe_c1_offset3;
        break;
      case RxDfeCoefficientTap2PerBit:
        piDelayNibble = (UINT16) DdrdDfeCoeff.Bits.dfe_coeff2;
        piDelayBit[0] = (UINT16) DdrdDfeCoeffOffset0.Bits.dfe_c2_offset0;
        piDelayBit[1] = (UINT16) DdrdDfeCoeffOffset0.Bits.dfe_c2_offset1;
        piDelayBit[2] = (UINT16) DdrdDfeCoeffOffset0.Bits.dfe_c2_offset2;
        piDelayBit[3] = (UINT16) DdrdDfeCoeffOffset0.Bits.dfe_c2_offset3;
        break;
      case RxDfeCoefficientTap3PerBit:
        piDelayNibble = (UINT16) DdrdDfeCoeff.Bits.dfe_coeff3;
        piDelayBit[0] = (UINT16) DdrdDfeCoeffOffset1.Bits.dfe_c3_offset0;
        piDelayBit[1] = (UINT16) DdrdDfeCoeffOffset1.Bits.dfe_c3_offset1;
        piDelayBit[2] = (UINT16) DdrdDfeCoeffOffset1.Bits.dfe_c3_offset2;
        piDelayBit[3] = (UINT16) DdrdDfeCoeffOffset1.Bits.dfe_c3_offset3;
        break;
      case RxDfeCoefficientTap4PerBit:
        piDelayNibble = (UINT16) DdrdDfeCoeff.Bits.dfe_coeff4;
        piDelayBit[0] = (UINT16) DdrdDfeCoeffOffset1.Bits.dfe_c4_offset0;
        piDelayBit[1] = (UINT16) DdrdDfeCoeffOffset1.Bits.dfe_c4_offset1;
        piDelayBit[2] = (UINT16) DdrdDfeCoeffOffset1.Bits.dfe_c4_offset2;
        piDelayBit[3] = (UINT16) DdrdDfeCoeffOffset1.Bits.dfe_c4_offset3;
        break;
      default:
        piDelayNibble = 0;
        piDelayBit[0] = 0;
        piDelayBit[1] = 0;
        piDelayBit[2] = 0;
        piDelayBit[3] = 0;
        break;
    }
    for (i = 0; i < BITS_PER_NIBBLE; i++) {
      piDelayBit[i] += piDelayNibble;
    }
    for (i = 0; i < BITS_PER_NIBBLE; i++) {
      curVal[i] = piDelayBit[i];
    }

    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      //
      // Convert logical bit to DDRIO physical register bit.
      //
      SwizzleBit = GetDqSwizzle (ch, strobe, bit, level, group);
      //
      // Save the value read into the output parameter
      //
      *value = curVal[SwizzleBit];
    } else {
      //
      // Write.
      // When mode is GSM_FORCE_WRITE, only supports ALL_BITS. Input parameter "value" must
      // be an array of size BITS_PER_NIBBLE, which contains different values for different
      // bits in the nibble.
      //
      for (i = 0; i < BITS_PER_NIBBLE; i++) {

        //
        // "SwizzleBit" is the physical DDRIO register bit. "i" is logical bit.
        //
        SwizzleBit = GetDqSwizzle (ch, strobe, i, level, group);

        // Adjust the current CMD delay value by offset
        if (mode & GSM_WRITE_OFFSET) {
          //
          // Add the offset to the current value.
          //
          tmp = curVal[SwizzleBit] + value[i];

        } else {
          //
          // Write absolute value
          //
          tmp = value[i];
        }
        //
        // Make sure we do not exeed the limits
        //
        if (tmp >= minLimit) {
          piDelay = tmp;
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, i,
            "%a Out of range!! tmp = 0x%x, minLimit = 0x%x\n", GetGroupStr (group), tmp, minLimit);
          piDelay = minLimit;
        }
        // Ensure we do not exceed maximums
        if (piDelay > maxLimit) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, i,
            "%a Out of range!! piDelay = 0x%x, maxLimit = 0x%x\n", GetGroupStr (group), piDelay, maxLimit);
          piDelay = maxLimit;
        }
        //
        // Adjust target bit delay
        //
        piDelayBit[SwizzleBit] = piDelay;

      }//i loop

      //
      // Normalize smallest per bit delay to 0.
      // Below code adjusts per-nibble and per-bit register value within a nibble.
      // Do not need to consider DQ swizzle.
      //

      piDelayMax = 0;
      piDelayMin = 0xFF;

      for (i = 0; i < BITS_PER_NIBBLE; i++) {
        if (piDelayBit[i] > piDelayMax) {
          piDelayMax = piDelayBit[i];
        }
        if (piDelayBit[i] < piDelayMin) {
          piDelayMin = piDelayBit[i];
        }
      }

      delta = piDelayMin - piDelayNibble;
      piDelayNibble += delta;

      //
      // Make sure we do not exeed the limits
      //
      if (piDelayNibble < minLimit) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, bit,
          "%a Out of range!! piDelayNibble = 0x%x, minLimit = 0x%x\n", GetGroupStr (RxDqsDelay), piDelayNibble, minLimit);
        piDelayNibble = minLimit;
      }

      if (piDelayNibble > maxLimit) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, bit,
          "%a Out of range!! piDelayNibble = 0x%x, maxLimit = 0x%x\n", GetGroupStr (RxDqsDelay), piDelayNibble, maxLimit);
        piDelayNibble = maxLimit;
      }

      for (i = 0; i < BITS_PER_NIBBLE; i++) {
        piDelayBit[i] = piDelayBit[i] - piDelayNibble;
        //Check per bit limits
        if (piDelayBit[i] > maxPerBitLimit) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, bit,
            "%a Out of range!! piDelayBit[%d] = 0x%x, maxPerBitLimit = 0x%x\n", GetGroupStr (group), i, piDelayBit[i], maxPerBitLimit);
          piDelayBit[i] = maxPerBitLimit;
        }
        if (piDelayBit[i] < minPerBitLimit) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, bit,
            "%a Out of range!! piDelayBit[%d] = 0x%x, minPerBitLimit = 0x%x\n", GetGroupStr (group), i, piDelayBit[i], minPerBitLimit);
          piDelayBit[i] = minPerBitLimit;
        }
      }

      //
      // Program per-bit register value with the adjusted delay value.
      //
      for (i = 0; i < BITS_PER_NIBBLE; i++) {
        piDelay = piDelayBit[i];
        switch (group) {
          case RxDfeCoefficientTap1PerBit:
            DdrdDfeCoeff.Bits.dfe_coeff1 = piDelayNibble;
            switch (i) {
              case 0:
                DdrdDfeCoeffOffset0.Bits.dfe_c1_offset0 = piDelay;
                break;
              case 1:
                DdrdDfeCoeffOffset0.Bits.dfe_c1_offset1 = piDelay;
                break;
              case 2:
                DdrdDfeCoeffOffset0.Bits.dfe_c1_offset2 = piDelay;
                break;
              case 3:
                DdrdDfeCoeffOffset0.Bits.dfe_c1_offset3 = piDelay;
                break;
              default:
                break;
            } // switch i
            break;


          case RxDfeCoefficientTap2PerBit:
            DdrdDfeCoeff.Bits.dfe_coeff2 = piDelayNibble;
            switch (i) {
              case 0:
                DdrdDfeCoeffOffset0.Bits.dfe_c2_offset0 = piDelay;
                break;
              case 1:
                DdrdDfeCoeffOffset0.Bits.dfe_c2_offset1 = piDelay;
                break;
              case 2:
                DdrdDfeCoeffOffset0.Bits.dfe_c2_offset2 = piDelay;
                break;
              case 3:
                DdrdDfeCoeffOffset0.Bits.dfe_c2_offset3 = piDelay;
                break;
              default:
                break;
            } // switch i
            break;


          case RxDfeCoefficientTap3PerBit:
            DdrdDfeCoeff.Bits.dfe_coeff3 = piDelayNibble;
            switch (i) {
              case 0:
                DdrdDfeCoeffOffset1.Bits.dfe_c3_offset0 = piDelay;
                break;
              case 1:
                DdrdDfeCoeffOffset1.Bits.dfe_c3_offset1 = piDelay;
                break;
              case 2:
                DdrdDfeCoeffOffset1.Bits.dfe_c3_offset2 = piDelay;
                break;
              case 3:
                DdrdDfeCoeffOffset1.Bits.dfe_c3_offset3 = piDelay;
                break;
              default:
                break;
            } // switch i
            break;


          case RxDfeCoefficientTap4PerBit:
            DdrdDfeCoeff.Bits.dfe_coeff4 = piDelayNibble;
            switch (i) {
              case 0:
                DdrdDfeCoeffOffset1.Bits.dfe_c4_offset0 = piDelay;
                break;
              case 1:
                DdrdDfeCoeffOffset1.Bits.dfe_c4_offset1 = piDelay;
                break;
              case 2:
                DdrdDfeCoeffOffset1.Bits.dfe_c4_offset2 = piDelay;
                break;
              case 3:
                DdrdDfeCoeffOffset1.Bits.dfe_c4_offset3 = piDelay;
                break;
              default:
                break;
            } // switch i
            break;

          default:
            break;
        } //switch group
      } //for i


      //
      // Write it back if the current data does not equal the cache value or if in force write mode
      //
      if ((DdrdDfeCoeffOffset0.Data != (*DdrioChannelNvList) [ch].DfeCoeffOffset0[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
        MemWritePciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_DFE_COEFF_OFFSET0_RANK_0_MCIO_DDRIO_REG), DdrdDfeCoeffOffset0.Data);
      }

      if ((DdrdDfeCoeffOffset1.Data != (*DdrioChannelNvList) [ch].DfeCoeffOffset1[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
        MemWritePciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_DFE_COEFF_OFFSET1_RANK_0_MCIO_DDRIO_REG), DdrdDfeCoeffOffset1.Data);
      }

      if ((DdrdDfeCoeff.Data != (*DdrioChannelNvList) [ch].DfeCoeff[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
        MemWritePciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_DFE_COEFF_RANK_0_MCIO_DDRIO_REG), DdrdDfeCoeff.Data);
      }
      //
      // Wait for the new value to settle
      //
      FixedDelayMicroSecond (usDelay);

    } // if read only

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
      (*DdrioChannelNvList) [ch].DfeCoeffOffset0[logRank][strobe] = DdrdDfeCoeffOffset0.Data;
      (*DdrioChannelNvList) [ch].DfeCoeffOffset1[logRank][strobe] = DdrdDfeCoeffOffset1.Data;
      (*DdrioChannelNvList) [ch].DfeCoeff[logRank][strobe] = DdrdDfeCoeff.Data;
    }
  } // strobe loop
  return MRC_STATUS_SUCCESS;
} // GetSetRxDfeCoefficientTapPerBit




/**

  Get Set Rx Dfe Coefficient Tap

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval IO delay

**/
MRC_STATUS
GetSetRxDfeCoefficientTap (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
{

  UINT8                               logRank;
  UINT8                               maxStrobe;
  UINT16                              curVal = 0;
  UINT16                              piDelay;
  INT16                               tmp;
  UINT16                              maxLimit = 0;
  UINT16                              minLimit = 0;
  UINT16                              usDelay = 0;
  DDRIO_CHANNEL_NVDATA                (*DdrioChannelNvList) [MAX_CH];
  struct ddrRank                      (*RankList) [MAX_RANK_DIMM];
  DDRD_N0_DFE_COEFF_RANK_0_MCIO_DDRIO_STRUCT         DdrdDfeCoeff;

  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  DdrioChannelNvList = GetDdrioChannelNvList (Host, socket);
  RankList = GetRankNvList (Host, socket, ch, dimm);
  DdrdDfeCoeff.Data = 0;

  //
  // Get the logical rank #
  //
  logRank = GetLogicalRank (Host, socket, ch, dimm, rank);
  ASSERT (logRank < MAX_RANK_CH);

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits (Host, level, group, &minLimit, &maxLimit, &usDelay);

  if (strobe == ALL_STROBES) {
    strobe = 0;
    maxStrobe = MSVx4;
  } else {
    maxStrobe = strobe + 1;

    if (maxStrobe > MAX_STROBE) {
      return MRC_STATUS_STROBE_NOT_PRESENT;
    }
  }

  for (; strobe < maxStrobe; strobe++) {
    {
      if (mode & GSM_READ_CSR) {
        //
        // Read from PCI config space
        //
        DdrdDfeCoeff.Data = MemReadPciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_DFE_COEFF_RANK_0_MCIO_DDRIO_REG));
      } else {
        //
        // Read from cache
        //
        DdrdDfeCoeff.Data = (*DdrioChannelNvList) [ch].DfeCoeff[logRank][strobe];
      }

      switch (group) {
        case RxDfeCoefficientTap1:
          curVal = (UINT16) DdrdDfeCoeff.Bits.dfe_coeff1;
          break;

        case RxDfeCoefficientTap2:
          curVal = (UINT16) DdrdDfeCoeff.Bits.dfe_coeff2;
          break;
        case RxDfeCoefficientTap3:
          curVal = (UINT16) DdrdDfeCoeff.Bits.dfe_coeff3;
          break;
        case RxDfeCoefficientTap4:
          curVal = (UINT16) DdrdDfeCoeff.Bits.dfe_coeff4;
          break;

        default:
          break;
      } // switch
    }

    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *value = curVal;
    } else {
      //
      // Write
      //
      // Adjust the current CMD delay value by offset
      if (mode & GSM_WRITE_OFFSET) {
        //
        // Add the offset to the current value
        //
        tmp = curVal + *value;
      } else {
        //
        // Write absolute value
        //
        tmp = *value;
      }

      //
      // Make sure we do not exeed the limits
      //
      if (tmp >= minLimit) {
        piDelay = tmp;
      } else {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
          "%a Out of range!! tmp = 0x%x, minLimit = 0x%x\n", GetGroupStr (group), tmp, minLimit);
        piDelay = minLimit;
      }
      // Ensure we do not exceed maximums
      if (piDelay > maxLimit) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
          "%a Out of range!! piDelay = 0x%x, maxLimit = 0x%x\n", GetGroupStr (group), piDelay, maxLimit);
        piDelay = maxLimit;
      }

      switch (group) {
        case RxDfeCoefficientTap1:
          DdrdDfeCoeff.Bits.dfe_coeff1 = piDelay;
          break;

        case RxDfeCoefficientTap2:
          DdrdDfeCoeff.Bits.dfe_coeff2 = piDelay;
          break;
        case RxDfeCoefficientTap3:
          DdrdDfeCoeff.Bits.dfe_coeff3 = piDelay;
          break;
        case RxDfeCoefficientTap4:
          DdrdDfeCoeff.Bits.dfe_coeff4 = piDelay;
          break;

        default:
          break;
      } // switch
      {
        //
        // Write it back if the current data does not equal the cache value or if in force write mode
        //
        if ((DdrdDfeCoeff.Data != (*DdrioChannelNvList) [ch].DfeCoeff[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
          MemWritePciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, logRank, strobe, DDRD_N0_DFE_COEFF_RANK_0_MCIO_DDRIO_REG), DdrdDfeCoeff.Data);
          //
          // Wait for the new value to settle
          //
          FixedDelayMicroSecond (usDelay);
        }
      }
    } // if read only

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
      (*DdrioChannelNvList) [ch].DfeCoeff[logRank][strobe] = DdrdDfeCoeff.Data;
    }

  } // strobe loop

  return MRC_STATUS_SUCCESS;
} // GetSetRxDfeCoefficientTap
