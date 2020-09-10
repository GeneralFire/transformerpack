/** @file
  MemCpgcIpLib.h

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

#ifndef  _MEM_CPGC_IP_LIB_
#define  _MEM_CPGC_IP_LIB_

#include <ReferenceCodeDataTypes.h>
#include <Memory/CpgcDefinitions.h>
#include "SysHost.h"
#include <Ppi/SsaMemoryDdrtConfig.h>
#include <Ppi/SsaMemoryServerConfig.h>

typedef enum {
  CpgcFullyUnlocked,
  CpgcLockStateReserved, // Unused
  CpgcVrefUnlocked,
  CpgcLocked,
  CpgcLockStateMax
} CPGC_LOCK_STATE;

//
// CPGC_ERR_ADDR0_MCDDC_CTL_STRUCT row bits are defined as 20 bits but
// only uses bits 0 thru 17 for the row address.
// Bits 18 thru 20 are for c-bits.
//
#define CPGC_ERR_ADDR0_ROW_BITS_MASK 0x3FFFF

#define FAST_X             0
#define FAST_Y             1
#define BG_INTER_1         1
#define BG_INTER_2         2

#define NIBBLE_MASK        0xF
#define BYTE_MASK          0xFF
#define WORD_MASK          0xFFFF
#define DWORD_MASK         0xFFFFFFFF

#define NIBBLE_WIDTH       4
#define BYTE_WIDTH         8
#define WORD_WIDTH         16

#define FIRST_ITERATION_LOOP                0
#define SECOND_ITERATION_LOOP               1

#define CPGC_RANK_MAPPING_MAX_NUMBER 32 // MAX logic rank number supported by CPGC
typedef struct CpgcErrorStatus_old {
  UINT32 cpgcErrDat0S;      // Data lane [31:0] even UI error status
  UINT32 cpgcErrDat1S;      // Data lane [63:32] even UI error status
  UINT32 cpgcErrDat2S;      // Data lane [31:0] odd UI error status
  UINT32 cpgcErrDat3S;      // Data lane [63:32] odd UI error status
  UINT32 cpgcErrEccS;       // Ecc lane [7:0] error status
  UINT32 cpgcErrEccSEven;   // Ecc lane [7:0] even UI error status
  UINT32 cpgcErrEccSOdd;    // Ecc lane [7:0] odd UI error status
  UINT32 cpgcErrNibSEven;   // 18 bits of even UI nibble error status. Ecc nibbles are always at nibble [9:8].
  UINT32 cpgcErrNibSOdd;    // 18 bits of odd UI nibble error status. Ecc nibbles are always at nibble [9:8].
  UINT32 cpgcErrBytGrp;    // 9 bits of byte group error status. Ecc byte group is always at byte group [8].
} CPGC_ERROR_STATUS_OLD, *PCPGC_ERROR_STATUS_OLD;


/*

  Enable/Disable the MPR mode for DDR5 DRAMs

  @param[in]  Host      Pointer to SysHost structure
  @param[in]  Socket    Current socket under test (0-based)
  @param[in]  Ch        Current channel under test (0-based)
  @param[in]  Enable    1: Enable 0: Disable

*/
VOID
SetMprTrainModeDdr5 (
  IN        PSYSHOST    Host,
  IN        UINT8       Socket,
  IN        UINT8       Ch,
  IN        UINT8       Enable
  );

/**

  This routine sets the CSRs required by BiosSetEridValidationMask

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] EridMask                          - Early Read ID bitmask.  Bit value 1 = perform validation;
                                                 bit value 0 = do not perform validation.
                                                 Bit position 3: CKP Early Read ID (even).
                                                 Bit position 2: CKP Early Read ID (odd).
                                                 Bit position 1: CKN Early Read ID (even).
                                                 Bit position 0: CKN Early Read ID (odd).

  @retval N/A

**/
VOID
BiosSetEridValidationMaskChip (
  IN  PSYSHOST                              Host,
  IN  UINT8                                 Socket,
  IN  UINT8                                 ChannelInSocket,
  IN  UINT8                                 EridMask
  );

/**

  This routine gets the CSRs required by BiosGetEridErrorStatus

  @param[in]  Host                              - Pointer to sysHost
  @param[in]  Socket                            - Socket Id
  @param[in]  ChannelInSocket                   - Channel number (0-based)
  @param[out] Status                            - Pointer to where the Early Read ID bit error status will be stored.
                                                  Bit value 1 = at least one error occurred; bit value 0 = no errors occurred.
                                                  Bit position 3: CKP Early Read ID (even).
                                                  Bit position 2: CKP Early Read ID (odd).
                                                  Bit position 1: CKN Early Read ID (even).
                                                  Bit position 0: CKN Early Read ID (odd).
  @param[out] Count                             - Pointer to where the error counter value will be stored.

  @retval N/A

**/
VOID
BiosGetEridErrorStatusChip (
  IN  PSYSHOST                              Host,
  IN  UINT8                                 Socket,
  IN  UINT8                                 ChannelInSocket,
  OUT UINT8                                 *Status,
  OUT UINT16                                *Count
  );

/**

  This routine sets the CSRs to convert read to MRR command.

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] Ch                                - Channel number (0-based)
  @param[in] Enable                            - 1/0 for Enable or not

  @retval N/A

**/
VOID
ConvertRdToMrr (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT8       Ch,
  IN UINT8       Enable
  );

/*

  Function to write a temporal basic pattern for low stress test

  @param[in]  Host        Pointer to SysHost structure
  @param[in]  Socket      Current socket under test (0-based)
  @param[in]  TemporalPattern   Pattern to be written
  @param[in]  NumCachelines     Number of cachelines
  @param[in]  IncludeDeselects  TRUE: Include deselects, FALSE: does not include deselects

*/
VOID
WriteTemporalPattern (
  IN        PSYSHOST  Host,
  IN        UINT8     Socket,
  IN        UINT32    TemporalPattern,
  IN        UINT8     NumCachelines,
  IN        BOOLEAN   IncludeDeselects
  );

/**

  Set the Mpr training mode to filter ACT/PRE.

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] Ch                                - Channel number (0-based)
  @param[in] Enable                            - 1/0 for Enable or not

  @retval N/A

**/
VOID
SetMprModeFilterActPre (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT8       Ch,
  IN UINT8       Enable
  );

/**

  This routine sets the CSRs required by BiosSetEridPattern

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] Signal                            - Zero based ERID signal number.
  @param[in] PatternMode                       - LFSR or pattern buffer mode.
  @param[in] EnableReload                      - Enable or disable seed reload for the Early Read ID.
  @param[in] EnableReset                       - Enable or disable seed reset for the Early Read ID.
  @param[in] Seed                              - Seed value for Early Read ID.

  @retval N/A

**/
VOID
BiosSetEridPatternChip (
  IN  PSYSHOST                              Host,
  IN  UINT8                                 Socket,
  IN  UINT8                                 ChannelInSocket,
  IN  ERID_SIGNAL                           Signal,
  IN  ERID_PATTERN_MODE                     PatternMode,
  IN  BOOLEAN                               EnableReload,
  IN  BOOLEAN                               EnableReset,
  IN  UINT32                                Seed
  );
/**

  This routine sets the CSRs required by BiosSetEridTrainingConfig

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] EnableTraining                    - Specifies whether Early Read ID coarse training mode is enabled.

  @retval N/A

**/
VOID
BiosSetEridTrainingConfigChip (
  IN  PSYSHOST                              Host,
  IN  UINT8                                 Socket,
  IN  UINT8                                 ChannelInSocket,
  IN  BOOLEAN                               EnableTraining
  );

/**

  This routine sets MaxNumberErrorCounters required by BiosServerGetCpgcInfo

  @param[in]      Host                       - Pointer to sysHost
  @param[in, out] Socket                     - Pointer to MRC_SERVER_CPGC_INFO struct

  @retval N/A

**/
VOID
BiosServerGetCpgcInfoChip (
  IN     PSYSHOST                Host,
  IN OUT MRC_SERVER_CPGC_INFO    *CpgcInfoStruct
  );


/**

  This routine sets the CSRs required by BiosServerSetCreditWaitConfig

  @param[in] Host                       - Pointer to sysHost
  @param[in] Socket                     - Socket Id
  @param[in] Controller                 - Controller number (0-based)
  @param[in] Enable                     - Specifies whether CPGC credit-wait feature is enabled.

  @retval N/A

**/
VOID
BiosServerSetCreditWaitConfigChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 Controller,
  IN  BOOLEAN               Enable
  );

/**

  This routine sets the CSRs required by BiosServerSetWpqInOrderConfig

  @param[in] Host                       - Pointer to sysHost
  @param[in] Socket                     - Socket Id
  @param[in] Controller                 - Controller number (0-based)
  @param[in] ChannelInSocket            - Channel number (0-based)
  @param[in] Enable                     - Specifies whether the WPQ-in-order feature are enabled

  @retval N/A

**/
VOID
BiosServerSetWpqInOrderConfigChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 Controller,
  IN  UINT8                 ChannelInSocket,
  IN  BOOLEAN               Enable
  );

/**

  This routine sets the CSRs required by BiosServerSetMultipleWrCreditConfig

  @param[in] Host                       - Pointer to sysHost
  @param[in] Socket                     - Socket Id
  @param[in] Controller                 - Controller number (0-based)
  @param[in] ChannelInSocket            - Channel number (0-based)
  @param[in] Enable                     - Specifies whether multiple write credits are enabled.

  @retval N/A

**/
VOID
BiosServerSetMultipleWrCreditConfigChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 Controller,
  IN  UINT8                 ChannelInSocket,
  IN  BOOLEAN               Enable
  );

/**

  This routine sets the CSRs required by BiosServerSetDqdbIndexes

  @param[in] Host                       - Pointer to sysHost
  @param[in] Socket                     - Socket Id
  @param[in] Controller                 - Controller number (0-based)
  @param[in] ChannelInSocket            - Channel number (0-based)
  @param[in] StartIndex                 - Starting index in the DqDB.  The value must be less than or equal
                                          to the MaxNumberDqdbCachelines value from the GetCpgcInfo() function.
  @param[in] EndIndex                   - Ending index in the DqDB.  The value must be less than or equal to
                                          the StartIndex value and less than or equal to the MaxNumberDqdbCachelines
                                          value from the GetCpgcInfo() function.
  @param[in] IncRateMode                - Increment rate mode.
  @param[in] IncRate                    - Increment rate value.  If the IncRateMode is ExponentialDqdbIncRateMode
                                          then the IncRate value must be less than or equal to the
                                          MaxDqdbIncRateExponentVal from the GetCpgcInfo() function.  If the
                                          IncRateMode is LinearDqdbIncRateMode then the IncRate value must be less
                                          than or equal to the MaxDqdbIncRateLinearVal from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetDqdbIndexesChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 Controller,
  IN  UINT8                 ChannelInSocket,
  IN  UINT8                 StartIndex,
  IN  UINT8                 EndIndex,
  IN  DQDB_INC_RATE_MODE    IncRateMode,
  IN  UINT8                 IncRate
  );

/**

  This routine sets the CSRs required by BiosServerSetDqdbEccDataSource

  @param[in] Host                       - Pointer to sysHost
  @param[in] Socket                     - Socket Id
  @param[in] ChannelInSocket            - Channel number (0-based)
  @param[in] EccDataSource              - Specifies the ECC signal data source

  @retval N/A

**/
VOID
BiosServerSetDqdbEccDataSourceChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 ChannelInSocket,
  IN  ECC_DATA_SOURCE       EccDataSource
  );

/**

  This routine sets the CSRs required by BiosServerSetDqdbUniseqMode

  @param[in] Host                       - Pointer to sysHost
  @param[in] Socket                     - Socket Id
  @param[in] ChannelInSocket            - Channel number (0-based)
  @param[in] Uniseq                     - Zero based DqDB unisequencer number(0-based)
  @param[in] Mode                       - DqDB unisequencer mode.

  @retval N/A

**/
VOID
BiosServerSetDqdbUniseqModeChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 ChannelInSocket,
  IN  UINT8                 Uniseq,
  IN  DQDB_UNISEQ_MODE      Mode
  );

/**

  This routine sets the CSRs required by BiosServerSetDqdbUniseqWrSeed

  @param[in] Host                       - Pointer to sysHost
  @param[in] Socket                     - Socket Id
  @param[in] ChannelInSocket            - Channel number (0-based)
  @param[in] Uniseq                     - Zero based DqDB unisequencer number(0-based)
  @param[in] Seed                       - DqDB write unisequencer seed value.  The value must be less than
                                          or equal to the MaxDqdbUniseqSeedVal value from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetDqdbUniseqWrSeedChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 ChannelInSocket,
  IN  UINT8                 Uniseq,
  IN  UINT32                Seed
  );

/**

  This routine sets the CSRs required by BiosServerSetDqdbUniseqRdSeed

  @param[in] Host                       - Pointer to sysHost
  @param[in] Socket                     - Socket Id
  @param[in] ChannelInSocket            - Channel number (0-based)
  @param[in] Uniseq                     - Zero based DqDB unisequencer number(0-based)
  @param[in] Seed                       - DqDB write unisequencer seed value. The value must be less than or equal
                                          to the MaxDqdbUniseqSeedVal value from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetDqdbUniseqRdSeedChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 ChannelInSocket,
  IN  UINT8                 Uniseq,
  IN  UINT32                Seed
  );

/**

  This routine gets the CSRs required by BiosServerGetDqdbUniseqWrSeed

  @param[in]      Host                       - Pointer to sysHost
  @param[in]      Socket                     - Socket Id
  @param[in]      ChannelInSocket            - Channel number (0-based)
  @param[in]      Uniseq                     - Zero based DqDB unisequencer number(0-based)
  @param[in, out] Seed                       - Pointer to where the DqDB write unisequencer seed value will be stored.

  @retval N/A

**/
VOID
BiosServerGetDqdbUniseqWrSeedChip (
  IN      PSYSHOST              Host,
  IN      UINT8                 Socket,
  IN      UINT8                 ChannelInSocket,
  IN      UINT8                 Uniseq,
  IN OUT  UINT32                *Seed
  );

/**

  This routine gets the CSRs required by BiosServerGetDqdbUniseqRdSeed

  @param[in]      Host                       - Pointer to sysHost
  @param[in]      Socket                     - Socket Id
  @param[in]      ChannelInSocket            - Channel number (0-based)
  @param[in]      Uniseq                     - Zero based DqDB unisequencer number(0-based)
  @param[in, out] Seed                       - Pointer to where the DqDB write unisequencer seed value will be stored.

  @retval N/A

**/
VOID
BiosServerGetDqdbUniseqRdSeedChip (
  IN      PSYSHOST              Host,
  IN      UINT8                 Socket,
  IN      UINT8                 ChannelInSocket,
  IN      UINT8                 Uniseq,
  IN OUT  UINT32                *Seed
  );

/**

  This routine sets the CSRs required by BiosServerSetDqdbUniseqLmn

  @param[in] Host                       - Pointer to sysHost
  @param[in] Socket                     - Socket Id
  @param[in] ChannelInSocket            - Channel number (0-based)
  @param[in] Uniseq                     - Zero based DqDB unisequencer number(0-based)
  @param[in] L                          - DqDB unisequencer L counter value.
                                          The value must be less than or equal to the MaxDqdbUniseqLVal value from the GetCpgcInfo() function.
  @param[in] M                          - DqDB unisequencer M counter value.
                                          The value must be less than or equal to the MaxDqdbUniseqMVal value from the GetCpgcInfo() function.
  @param[in] N                          - DqDB unisequencer N counter value.
                                          The value must be less than or equal to the MaxDqdbUniseqNVal value from the GetCpgcInfo() function.
  @param[in] LDataSel                   - DqDB unisequencer L data select value.  The value must be 0 or 1.
  @param[in] EnableFreqSweep            - Enables the Frequency Sweep feature.

  @retval N/A

**/
VOID
BiosServerSetDqdbUniseqLmnChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 ChannelInSocket,
  IN  UINT8                 Uniseq,
  IN  UINT16                L,
  IN  UINT16                M,
  IN  UINT16                N,
  IN  UINT8                 LDataSel,
  IN  BOOLEAN               EnableFreqSweep
  );

/**

  This routine sets the CSRs required by BiosServerSetDqdbUniseqLfsrConfig

  @param[in] Host                       - Pointer to sysHost
  @param[in] Socket                     - Socket Id
  @param[in] ChannelInSocket            - Channel number (0-based)
  @param[in] ReloadRate                 - DqDB unisequencer LFSR seed reload rate.
                                          The value must be less than or equal to the MaxDqdbUniseqSeedReloadRateVal value from the GetCpgcInfo() function.
  @param[in] SaveRate                   - DqDB unisequencer LFSR seed save rate.
                                          The value must be less than or equal to the MaxDqdbUniseqSeedSaveRateVal value from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetDqdbUniseqLfsrConfigChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 ChannelInSocket,
  IN  UINT8                 ReloadRate,
  IN  UINT8                 SaveRate
  );

/**

  This routine sets the CSRs required by BiosServerSetDqdbInvertDcConfig

  @param[in] Host                       - Pointer to sysHost
  @param[in] Socket                     - Socket Id
  @param[in] ChannelInSocket            - Channel number (0-based)
  @param[in] Mode                       - Invert/DC mode.
  @param[in] DcPolarity                 - DC polarity (when operating in the DC mode). 0 = DC low. 1 = DC high.
  @param[in] ShiftRateExponent          - Exponent of the bitmask shift rate.  The value must be less than or equal
                                          to the MaxDqdbInvertDcShiftRateExponentVal value from the GetCpgcInfo() function.
  @param[in] DqMaskLow                  - Lower 32 bits of the DQ Mask (0-based)
  @param[in] DqMaskHigh                 - Upper 32 bits of the DQ Mask (0-based)
  @param[in] EccMask                    - Ecc Mask (0-based)

  @retval N/A

**/
VOID
BiosServerSetDqdbInvertDcConfigChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 ChannelInSocket,
  IN  INVERT_DC_MODE        Mode,
  IN  UINT8                 DcPolarity,
  IN  UINT8                 ShiftRateExponent,
  IN  UINT32                DqMaskLow,
  IN  UINT32                DqMaskHigh,
  IN  UINT8                 EccMask
  );


/**

  This routine sets the CSRs required by BiosServerSetCadbConfig

  @param[in] Host                       - Pointer to sysHost
  @param[in] Socket                     - Socket Id
  @param[in] ChannelInSocket            - Channel number (0-based)
  @param[in] EnableAlwaysOn             - Specifies whether CADB patterns will be driven out on all
                                          command and address pins every Dclk while a test is running.
  @param[in] EnableOnDeselect           - Specifies whether CADB patterns will be driven out during deselect packets.
  @param[in] EnableParityNTiming        - Specifies parity timing.  If this parameter is TRUE, CADB parity will be
                                          driven on same clock as CMD/Address (i.e., N timing for UDIMM DDR4 only).
                                          If this parameter is FALSE, CADB parity will be driven on the clock after
                                          CMD/Address (i.e,. N+1 timing for all other devices).
  @param[in] EnableOnePassAlwaysOn      - This parameter only applies if the EnableAlwaysOn parameter is TRUE.
                                          If this parameter is TRUE, the test stops after all 16 entries have been issued out of CADB.
  @param[in] EnablePdaDoubleLength      - If this parameter is TRUE, any PBA (Per Buffer Addressable) /PDA (Per DRAM addressable)
                                          writes issued from CADB will be double length (i.e. if lockstep is disabled, 16 UI
                                          instead of 8 UI or if lockstep is enabled, 8 UI instead of 4 UI).  This is to allow
                                          more setup up time for PBA accesses during training.  If this parameter is FALSE,
                                          PBA/PDA writes will look exactly the same as normal writes.

  @retval N/A

**/
VOID
BiosServerSetCadbConfigChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 ChannelInSocket,
  IN  BOOLEAN               EnableAlwaysOn,
  IN  BOOLEAN               EnableOnDeselect,
  IN  BOOLEAN               EnableParityNTiming,
  IN  BOOLEAN               EnableOnePassAlwaysOn,
  IN  BOOLEAN               EnablePdaDoubleLength
  );

/**

  This routine sets the CSRs required by BiosServerSetCadbUniseqMode

  @param[in] Host                       - Pointer to sysHost
  @param[in] Socket                     - Socket Id
  @param[in] ChannelInSocket            - Channel number (0-based)
  @param[in] Uniseq                     - Zero based CADB unisequencer number.
  @param[in] DeselectMode               - CADB deselect cycle unisequencer mode.
                                          The value must be less than or equal to the MaxCadbUniseqSeedVal value from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetCadbUniseqModeChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 ChannelInSocket,
  IN  UINT8                 Uniseq,
  IN  CADB_UNISEQ_MODE      DeselectMode
  );


/**

  This routine sets the CSRs required by BiosServerSetCadbUniseqSeed

  @param[in] Host                       - Pointer to sysHost
  @param[in] Socket                     - Socket Id
  @param[in] ChannelInSocket            - Channel number (0-based)
  @param[in] Uniseq                     - Zero based CADB unisequencer number.
  @param[in] DeselectMode               - CADB deselect cycle unisequencer mode.
                                          The value must be less than or equal to the MaxCadbUniseqSeedVal value from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetCadbUniseqSeedChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 ChannelInSocket,
  IN  UINT8                 Uniseq,
  IN  UINT32                DeselectSeed
  );



/**

  This routine sets the CSRs required by BiosServerSetCadbUniseqLmn

  @param[in] Host                       - Pointer to sysHost
  @param[in] Socket                     - Socket Id
  @param[in] ChannelInSocket            - Channel number (0-based)
  @param[in] Uniseq                     - Zero based DqDB unisequencer number(0-based)
  @param[in] L                          - CADB unisequencer L counter value.
                                          The value must be less than or equal to the MaxCadbUniseqLVal value from the GetCpgcInfo() function.
  @param[in] M                          - CADB unisequencer M counter value.
                                          The value must be less than or equal to the MaxCadbUniseqMVal value from the GetCpgcInfo() function.
  @param[in] N                          - CADB unisequencer N counter value.
                                          The value must be less than or equal to the MaxCadbUniseqNVal value from the GetCpgcInfo() function.
  @param[in] LDataSel                   - CADB unisequencer L data select value.  The value must be 0 or 1.
  @param[in] EnableFreqSweep            - Enables LMN frequency sweep feature.

  @retval N/A

**/
VOID
BiosServerSetCadbUniseqLmnChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 ChannelInSocket,
  IN  UINT8                 Uniseq,
  IN  UINT16                L,
  IN  UINT16                M,
  IN  UINT16                N,
  IN  UINT8                 LDataSel,
  IN  BOOLEAN               EnableFreqSweep
  );

/**

  This routine sets the CSRs required by BiosServerSetDummyReadLmn

  @param[in] Host                       - Pointer to sysHost
  @param[in] Socket                     - Socket Id
  @param[in] Controller                 - Controller (0-based)
  @param[in] ChannelInSocket            - Channel number (0-based)
  @param[in] L                          - Dummy Read unisequencer L counter value.
                                          The value must be less than or equal to the MaxDummyReadLVal value from the GetCpgcInfo() function.
  @param[in] M                          - Dummy Read unisequencer M counter value.
                                          The value must be less than or equal to the MaxDummyReadMVal value from the GetCpgcInfo() function.
  @param[in] N                          - Dummy Read unisequencer N counter value.
                                          The value must be less than or equal to the MaxDummyReadNVal value from the GetCpgcInfo() function.
  @param[in] LDataSel                   - Dummy Read unisequencer L data select value.  The value must be 0 or 1.
  @param[in] EnableFreqSweep            - Enables LMN frequency sweep feature.

  @retval NotAvailable if the system does not support the Dummy Read feature.  Else Success.
**/

SSA_STATUS
BiosServerSetDummyReadLmnChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 Controller,
  IN  UINT8                 ChannelInSocket,
  IN  UINT16                L,
  IN  UINT16                M,
  IN  UINT16                N,
  IN  UINT8                 LDataSel,
  IN  BOOLEAN               EnableFreqSweep
  );

/**

  This routine sets the CSRs required by BiosServerSetDummyReadBankMask

  @param[in] Host                       - Pointer to sysHost
  @param[in] Socket                     - Socket Id
  @param[in] Controller                 - Controller (0-based)
  @param[in] ChannelInSocket            - Channel number (0-based)
  @param[in] BankMask                   - Dummy Read bank mask.(0-based)

  @retval NotAvailable if the system does not support the Dummy Read feature.  Else UnsupportedValue if the given BankMask value is out of range.  Else Success.
**/

SSA_STATUS
BiosServerSetDummyReadBankMaskChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 Controller,
  IN  UINT8                 ChannelInSocket,
  IN  UINT16                BankMask
  );

/**

  This routine sets the CSRs required by BiosServerSetSubseqConfig

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] Controller                        - Controller (0-based)
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] Subseq                            - Zero based subsequence number.
                                                 The value must be less than the MaxNumberSubseqs value from the GetCpgcInfo() function.
  @param[in] BurstLengthMode,                  - Burst length mode.
  @param[in] BurstLength                       - Burst length in cachelines.
                                                 If BurstLengthMode is ExponentialBurstLengthMode then the Burstlength value
                                                 is an exponent and the value must be less than or equal to the MaxBurstLengthExponentVal
                                                 value from the in GetCpgcInfo() function.  If BurstLengthMode is LinearBurstLengthMode
                                                 then the Burstlength value is linear and the value must be less than or equal to the
                                                 MaxBurstLengthLinearVal value from the in GetCpgcInfo() function.
  @param[in] InterSubseqWait                   - Inter-subsequence wait in dclks.
                                                 The value must be less than or equal to the MaxInterSubseqWaitVal value from the GetCpgcinfo() function.
  @param[in] SubseqType                        - Subsequence type.
  @param[in] EnableSaveCurrentBaseAddrToStart  - Specifies whether the current base address is saved to the start address with this subsequence.
  @param[in] EnableResetCurrentBaseAddrToStart - Specifies whether the current base address is reset to the start address with this subsequence.
  @param[in] EnableAddrInversion               - Specifies whether address inversion enabled for this subsequence.
  @param[in] EnableDataInversion               - Specifies whether data inversion enabled for this subsequence.

  @retval N/A

**/
VOID
BiosServerSetSubseqConfigChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 Controller,
  IN  UINT8                 ChannelInSocket,
  IN  UINT8                 Subseq,
  IN  BURST_LENGTH_MODE     BurstLengthMode,
  IN  UINT32                BurstLength,
  IN  UINT32                InterSubseqWait,
  IN  SUBSEQ_TYPE           SubseqType,
  IN  BOOLEAN               EnableSaveCurrentBaseAddrToStart,
  IN  BOOLEAN               EnableResetCurrentBaseAddrToStart,
  IN  BOOLEAN               EnableAddrInversion,
  IN  BOOLEAN               EnableDataInversion
  );

/**

  This routine sets the CSRs required by BiosServerSetSubseqOffsetConfig

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] Controller                        - Controller (0-based)
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] Subseq                            - Zero based subsequence number.
                                                 The value must be less than the MaxNumberSubseqs value from the GetCpgcInfo() function.
  @param[in] EnableBaseInvertData              - Specifies whether base inversion of the data is enabled.
  @param[in] EnableRowIncrement                - Specifies whether row increment is enabled.
  @param[in] RowIncrementOrder                 - Specifies row increment order.  The value must be 0 or 1.
  @param[in] EnableColIncrement                - Specifies whether column increment is enabled.
  @param[in] ColIncrementOrder                 - Specifies column increment order.  The value must be 0 or 1.
  @param[in] BaseSubseqType                    - Every time the BaseSubseqRepeatRate is reached a Read or Write is performed for one
                                                 command based on the following fields: BaseSubseqType indicates whether a read
                                                 (BaseReadSubseqType) or write (BaseWriteSubseqType) operation will occur at the
                                                 current base address.  EnableBaseInvertData indicates whether the read or write
                                                 (BaseSubseqType) operation at the current base address is inverted
                                                 (EnableBaseInvertData = TRUE) or not (EnableBaseInvertData = FALSE).
  @param[in] BaseSubseqRepeatRate              - Specifies how often the BaseSubseqType operation occurs for 1 cacheline.
                                                 0 = No BaseSubseqType cacheline operations will occur during the Offset_Read or Offset_Write.
                                                 1 = Reserved value.  2 = Repeat the BaseSubseqType cacheline operation after every offset cacheline operation.
                                                 ...  31 = Repeat the BaseSubseqType cacheline operation after 30 offset cacheline operations.
  @param[in] OffsetAddrUpdateRate              - Specifies the rate that the Current Offset Address is updated.
                                                 The value must be less than or equal to the MaxOffsetAddrUpdateRateVal value from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetSubseqOffsetConfigChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 Controller,
  IN  UINT8                 ChannelInSocket,
  IN  UINT8                 Subseq,
  IN  BOOLEAN               EnableBaseInvertData,
  IN  BOOLEAN               EnableRowIncrement,
  IN  UINT8                 RowIncrementOrder,
  IN  BOOLEAN               EnableColIncrement,
  IN  UINT8                 ColIncrementOrder,
  IN  SUBSEQ_TYPE           BaseSubseqType,
  IN  UINT32                BaseSubseqRepeatRate,
  IN  UINT32                OffsetAddrUpdateRate
  );

/**

  This routine sets the CSRs required by BiosServerSetSeqConfig

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] Controller                        - Controller (0-based)
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] StartDelay                        - Specifies the delay period (in dclks) between the start of the test and the actual entry to
                                                 Loopback.Pattern where the first SubSequence is executed immediately.  The value must be
                                                 less than or equal to the MaxStartDelayVal value from the GetCpgcInfo() function.
  @param[in] SubseqStartIndex                  - Specifies the initial SubSequence pointer where a Sequence first enters Loopback Pattern (start of test).
                                                 The value must be less than the MaxNumberSubseqs value from the GetCpgcInfo() function.
  @param[in] SubseqEndIndex                    - When the subsequence pointed to by the SubseqEndIndex is completed the loop count is incremented
                                                 by 1 and the current subsequence index is reset to the SubseqStartIndex.  The SubseqEndIndex
                                                 must be greater than or equal to the SubseqStartIndex value.  The value must be less than
                                                 the MaxNumberSubseqs value from the GetCpgcInfo() function.
  @param[in] LoopCount                         - Specifies how many iterations of the complete sequence loop takes place before a test stops,
                                                 where a sequence loop is defined by the completion of all subsequences indicated by the
                                                 SubseqStartIndex to SubseqEndIndex. The value must be less than or equal to the MaxLoopCountVal
                                                 from the GetCpgcInfo() function.  If the IsLoopCountExponential value from the GetCpgcInfo()
                                                 function is TRUE then the number of loops is 2^(LoopCount-1).  If the IsLoopCountExponential value
                                                 from the GetCpgcInfo() function is FALSE then the number of loops is simply LoopCount.
                                                 In both the linear of exponential cases, 0 is a special value means that the loop count is infinite.
  @param[in] InitMode                          - Initialization mode.
  @param[in] EnableGlobalControl               - Specifies whether global control is enabled for the given channel.
  @param[in] EnableConstantWriteStrobe         - Specifies whether constant write strobe is enabled.
  @param[in] EnableDummyReads                  - Specifies whether dummy reads are enabled.
  @param[in] AddrUpdateRateMode                - Address update rate mode.

  @retval N/A

**/
VOID
BiosServerSetSeqConfigChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 Controller,
  IN  UINT8                 ChannelInSocket,
  IN  UINT32                StartDelay,
  IN  UINT8                 SubseqStartIndex,
  IN  UINT8                 SubseqEndIndex,
  IN  UINT32                LoopCount,
  IN  CPGC_INIT_MODE        InitMode,
  IN  BOOLEAN               EnableGlobalControl,
  IN  BOOLEAN               EnableConstantWriteStrobe,
  IN  BOOLEAN               EnableDummyReads,
  IN  ADDR_UPDATE_RATE_MODE AddrUpdateRateMode
  );

/**

  This routine sets the CSRs required by BiosServerSetLoopCount

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] Controller                        - Controller (0-based)
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] LoopCount                         - Specifies how many iterations of the complete sequence loop takes place before a test stops,
                                                 where a sequence loop is defined by the completion of all subsequences indicated by the
                                                 SubseqStartIndex to SubseqEndIndex. The value must be less than or equal to the MaxLoopCountVal
                                                 from the GetCpgcInfo() function.  If the IsLoopCountExponential value from the GetCpgcInfo()
                                                 function is TRUE then the number of loops is 2^(LoopCount-1).  If the IsLoopCountExponential
                                                 value from the GetCpgcInfo() function is FALSE then the number of loops is simply LoopCount.
                                                 In both the linear of exponential cases, 0 is a special value means that the loop count is infinite.

  @retval N/A

**/
VOID
BiosServerSetLoopCountChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 Controller,
  IN  UINT8                 ChannelInSocket,
  IN  UINT32                LoopCount
  );

/**

  This routine sets the CSRs required by BiosServerSetSeqAddrConfig

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] Controller                        - Controller (0-based)
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] EnableRankWrapCarry               - If a base rank address wrap occurs  and EnableRankWrapCarry is set to TRUE then a carry
                                                 increment or decrement from the rank address (order N) will propagate to the N+1 order address field(s).
  @param[in] EnableBankWrapCarry               - If a base bank address wrap occurs  and EnableBankWrapCarry is set to TRUE then a carry
                                                 increment or decrement from the bank address (order N) will propagate to the N+1 order address field(s).
  @param[in] EnableRowWrapCarry                - If a base row address wrap occurs and EnableRowWrapCarry is set to TRUE then a carry
                                                 increment or decrement from the row address (order N) will propagate to the N+1 order address field(s).
  @param[in] EnableColWrapCarry                - If a base column address wrap occurs  and EnableColWrapCarry is set to TRUE then a carry
                                                 increment or decrement from the column address (order N) will propagate to the N+1 order address field(s).
  @param[in] EnableRankAddrInvert              - Setting to TRUE will cause the current rank address to be inverted with the periodicity
                                                 described by the AddrInvertRate parameter.
  @param[in] EnableBankAddrInvert              - Setting to TRUE will cause the current bank address to be inverted with the periodicity
                                                 described by the AddrInvertRate parameter.
  @param[in] EnableRowAddrInvert               - Setting to TRUE will cause the current row address to be inverted with the periodicity
                                                 described by the AddrInvertRate parameter.
  @param[in] EnableColAddrInvert               - Setting to TRUE will cause the current column address to be inverted with the periodicity
                                                 described by the AddrInvertRate parameter.
  @param[in] AddrInvertRate                    - Base address inversion rate.  The SetSeqConfig() function's AddrUpdateRateMode parameter
                                                 specifies whether the base address inversion logic is based on number of cachelines written
                                                 and read or on the number of sequences performed.  If AddrUpdateRateMode from the SetSeqConfig()
                                                 function is CachelineAddrUpdateRate then the current base address is inverted based on
                                                 2^AddrInvertRate read and write cacheline transactions.  If AddrUpdateRateMode from the
                                                 SetSeqConfig() function is LoopCountAddrUpdateRate then the current base address is inverted
                                                 based on 2^AddrInvertRate loops through the sequence.  The value must be less than or equal
                                                 to the MaxAddrInvertRateVal value from the GetCpgcInfo() function.
  @param[in] RankAddrOrder                     - Specifies the relative ordering of the rank address update logic in the nested for loop in
                                                 relation to the bank, row, and column address update logic.  Any address fields set to the
                                                 same order that roll over will cause a distinct carry of +1 or -1 to the next higher order
                                                 address field (multiple simultaneous carries will cause only one increment/decrement).
                                                 All fields can be arbitrarily placed in any order as INT32 as all address order fields are
                                                 set in a continuous order starting from 0 and may not skip over an order number.
                                                 Example: setting 0,1,2,1 in the 4 fields (Col,Row,Bank,Rank) is legal;
                                                 setting 0,2,2,3 is illegal (not continuous).  The value must be less than or equal to 3.
  @param[in] BankAddrOrder                     - Specifies the relative ordering of the bank address update logic in the nested for loop
                                                 in relation to the rank, row, and column address update logic.  The value must be less than or equal to 3.
  @param[in] RowAddrOrder                      - Specifies the relative ordering of the row address update logic in the nested for loop
                                                 in relation to the rank, bank, and column address update logic.  The value must be less than or equal to 3.
  @param[in] ColAddrOrder                      - Specifies the relative ordering of the column address update logic in the nested for loop
                                                 in relation to the rank, bank, and row address update logic.  The value must be less than or equal to 3.

  @retval N/A

**/
VOID
BiosServerSetSeqAddrConfigChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 Controller,
  IN  UINT8                 ChannelInSocket,
  IN  BOOLEAN               EnableRankWrapCarry,
  IN  BOOLEAN               EnableBankWrapCarry,
  IN  BOOLEAN               EnableRowWrapCarry,
  IN  BOOLEAN               EnableColWrapCarry,
  IN  BOOLEAN               EnableRankAddrInvert,
  IN  BOOLEAN               EnableBankAddrInvert,
  IN  BOOLEAN               EnableRowAddrInvert,
  IN  BOOLEAN               EnableColAddrInvert,
  IN  UINT8                 AddrInvertRate,
  IN  UINT8                 RankAddrOrder,
  IN  UINT8                 BankAddrOrder,
  IN  UINT8                 RowAddrOrder,
  IN  UINT8                 ColAddrOrder
  );

/**

  This routine sets the CSRs required by BiosServerSetSeqStartAddr

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] Controller                        - Controller (0-based)
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] RankAddr                          - Rank start address.  This value is the index into the CPGC rank map (see the SetSeqRankMap() function).
                                                 The value must be less than the MaxNumberRankMapEntries value from the GetCpgcInfo() function.
  @param[in] BankAddr                          - Bank start address.  If the CPGC engine supports a bank map (i.e., the MaxNumberBankMapEntried
                                                 value from the GetCpgcInfo() function is greater than 0) then this value is is the index into
                                                 the CPGC bank map (see the SetSeqBankMap() function) and the value must be less than the
                                                 MaxNumberBankMapEntried value from the GetCpgcInfo() function.  If the CPGC engine does not
                                                 support a bank map then the value must be less than the BankCount value from the GetDimmInfo
                                                 function for the DIMMs that will be tested.
  @param[in] RowAddr                           - Row start address.  The value must be less than the RowSize value from the GetDimmInfo function
                                                 for the DIMMs that will be tested.
  @param[in] ColAddr                           - Column start address.  The value must be less than the ColumnSize value from the GetDimmInfo
                                                 function for the DIMMs that will be tested.

  @retval N/A

**/
VOID
BiosServerSetSeqStartAddrChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 Controller,
  IN  UINT8                 ChannelInSocket,
  IN  UINT8                 RankAddr,
  IN  UINT8                 BankAddr,
  IN  UINT32                RowAddr,
  IN  UINT32                ColAddr
  );

/**

  This routine sets the CSRs required by BiosServerSetSeqWrapAddr

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] Controller                        - Controller (0-based)
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] RankAddr                          - Rank start address.  This value is the index into the CPGC rank map (see the SetSeqRankMap() function).
                                                 The value must be less than the MaxNumberRankMapEntries value from the GetCpgcInfo() function.
  @param[in] BankAddr                          - Bank start address.  If the CPGC engine supports a bank map (i.e., the MaxNumberBankMapEntried value
                                                 from the GetCpgcInfo() function is greater than 0) then this value is is the index into the CPGC bank
                                                 map (see the SetSeqBankMap() function) and the value must be less than the MaxNumberBankMapEntried
                                                 value from the GetCpgcInfo() function.  If the CPGC engine does not support a bank map then the value
                                                 must be less than the BankCount value from the GetDimmInfo function for the DIMMs that will be tested.
  @param[in] RowAddr                           - Row start address.  The value must be less than the RowSize value from the GetDimmInfo function for the
                                                 DIMMs that will be tested.
  @param[in] ColAddr                           - Column start address.  The value must be less than the ColumnSize value from the GetDimmInfo function
                                                 for the DIMMs that will be tested.

  @retval N/A

**/
VOID
BiosServerSetSeqWrapAddrChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 Controller,
  IN  UINT8                 ChannelInSocket,
  IN  UINT8                 RankAddr,
  IN  UINT8                 BankAddr,
  IN  UINT32                RowAddr,
  IN  UINT32                ColAddr
  );

/**

  This routine sets the CSRs required by BiosServerSetSeqAddrInc

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] Controller                        - Controller (0-based)
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] RankAddrIncMode                   - Rank address increment rate mode.
  @param[in] RankAddrIncRate                   - Rank address increment rate.  In linear mode (see RankAddrIncMode), RankAddrIncRate+1 specifies
                                                 how many times a specific rank address is repeated before adding the RankAddrIncVal.
                                                 In exponential mode, 2^RankAddrIncRate defines how many times a specific rank address is
                                                 repeated before adding the RankAddrIncVal.  If RankAddrIncMode is ExponentialAddrIncMode
                                                 then the value must be less than or equal to the MaxRankAddrIncRateExponentVal from the
                                                 GetCpgcInfo() function.  If RankAddrIncMode is LinearAddrIncMode then the value must be
                                                 less than or equal to the MaxRankAddrIncRateLinearVal from the GetCpgcInfo() function.
  @param[in] RankAddrIncVal                    - Rank start address increment value.  When dictated by RankAddrIncRate the current rank address
                                                 will be modified by the RankAddrIncVal (which is a 2's compliment field).
                                                 A value of 0 means the rank address increment is effectively disabled since no offset will be applied.
                                                 Disabling the rank address increment by setting RankAddrIncVal to 0 does not affect a carry from a
                                                 lower order field rolling over to a higher order field which will also result in an increment or
                                                 decrement to the higher order field of +1 or -1.
                                                 Note that a positive value in this field will cause a +1 carry to propagate while a negative value
                                                 will cause a -1 carry to propagate from order N to order N+1.  The value must be greater than or equal
                                                 to the MinRankAddrIncVal from the GetCpgcInfo() function.  The value must be less than or equal to the
                                                 MaxRankAddrIncVal from the GetCpgcInfo() function.
  @param[in] BankAddrIncMode                   - Bank address increment rate mode.
  @param[in] BankAddrIncRate                   - Bank address increment rate.  In linear mode (see BankAddrIncMode), BankAddrIncRate+1 specifies how many
                                                 times a specific rank address is repeated before adding the BankAddrIncVal.  In exponential mode,
                                                 2^BankAddrIncRate defines how many times a specific rank address is repeated before adding the BankAddrIncVal.
                                                 If BankAddrIncMode is ExponentialAddrIncMode then the value must be less than or equal to the
                                                 MaxBankAddrIncRateExponentVal from the GetCpgcInfo() function.  If BankAddrIncMode is LinearAddrIncMode
                                                 then the value must be less than or equal to the MaxBankAddrIncRateLinearVal from the GetCpgcInfo() function.
  @param[in] BankAddrIncVal                    - Bank start address increment value.  When dictated by BankAddrIncRate the current bank address will be
                                                 modified by the BankAddrIncVal (which is a 2's compliment field).
                                                 A value of 0 means the bank address increment is effectively disabled since no offset will be applied.
                                                 Disabling the bank address increment by setting BankAddrIncVal to 0 does not affect a carry from a lower
                                                 order field rolling over to a higher order field which will also result in an increment or decrement to
                                                 the higher order field of +1 or -1.  Note that a positive value in this field will cause a +1 carry to
                                                 propagate while a negative value will cause a -1 carry to propagate from order N to order N+1.
                                                 The value must be greater than or equal to the MinBankAddrIncVal from the GetCpgcInfo() function.
                                                 The value must be less than or equal to the MaxBankAddrIncVal from the GetCpgcInfo() function.
  @param[in] RowAddrIncMode                    - Row address increment rate mode.
  @param[in] RowAddrIncRate                    - Row address increment rate.  In linear mode (see RowAddrIncMode), RowAddrIncRate+1 specifies how many
                                                 times a specific rank address is repeated before adding the RowAddrIncVal.
                                                 In exponential mode, 2^RowAddrIncRate defines how many times a specific rank address is repeated
                                                 before adding the RowAddrIncVal.  If RowAddrIncMode is ExponentialAddrIncMode then the value must
                                                 be less than or equal to the MaxRowAddrIncRateExponentVal from the GetCpgcInfo() function.
                                                 If RowAddrIncMode is LinearAddrIncMode then the value must be less than or equal to the
                                                 MaxRowAddrIncRateLinearVal from the GetCpgcInfo() function.
  @param[in] RowAddrIncVal                     - Row start address increment value.  When dictated by RowAddrIncRate the current row address will be
                                                 modified by the RowAddrIncVal (which is a 2's compliment field.  A value of 0 means the row address
                                                 increment is effectively disabled since no offset will be applied.  Disabling the row address
                                                 increment by setting RowAddrIncVal to 0 does not affect a carry from a lower order field rolling over
                                                 to a higher order field which will also result in an increment or decrement to the higher order field
                                                 of +1 or -1.  Note that a positive value in this field will cause a +1 carry to propagate while a
                                                 negative value will cause a -1 carry to propagate from order N to order N+1.  The value must be greater
                                                 than or equal to the MinRowAddrIncVal from the GetCpgcInfo() function.  The value must be less than or
                                                 equal to the MaxRowAddrIncVal from the GetCpgcInfo() function.
  @param[in] ColAddrIncMode                    - Column address increment rate mode.
  @param[in] ColAddrIncRate                    - Column address increment rate.  If ColAddrIncMode is ExponentialAddrIncMode then 2^ColAddrIncRate
                                                 defines how many times a specific rank address is repeated before adding the ColAddrIncVal.
                                                 If ColAddrIncMode is LinearAddrIncMode then ColAddrIncRate+1 specifies how many times a specific rank
                                                 address is repeated before adding the ColAddrIncVal.  If ColAddrIncMode is ExponentialAddrIncMode then
                                                 the value must be less than or equal to the MaxColAddrIncRateExponentVal from the GetCpgcInfo() function.
                                                 If ColAddrIncMode is LinearAddrIncMode then the value must be less than or equal to the
                                                 MaxColAddrIncRateLinearVal from the GetCpgcInfo() function.
  @param[in] ColAddrIncVal                     - Column start address increment value.  When dictated by ColAddrIncRate the current column address will
                                                 be modified by the ColAddrIncVal (which is a 2's compliment field).  A value of 0 means the column address
                                                 increment is effectively disabled since no offset will be applied.  Disabling the column address increment
                                                 by setting column address increment to 0 does not affect a carry from a lower order field rolling over to a
                                                 higher order field which will also result in an increment or decrement to the higher order field of +1 or -1.
                                                 Note that a positive value in this field will cause a +1 carry to propagate while a negative value will cause
                                                 a -1 carry to propagate from order N to order N+1.  The value must be greater than or equal to the
                                                 MinColAddrIncVal from the GetCpgcInfo() function.  The value must be less than or equal to the MaxColAddrIncVal
                                                 from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetSeqAddrIncChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 Controller,
  IN  UINT8                 ChannelInSocket,
  IN  ADDR_INC_MODE         RankAddrIncMode,
  IN  UINT32                RankAddrIncRate,
  IN  INT8                  RankAddrIncVal,
  IN  ADDR_INC_MODE         BankAddrIncMode,
  IN  UINT32                BankAddrIncRate,
  IN  INT8                  BankAddrIncVal,
  IN  ADDR_INC_MODE         RowAddrIncMode,
  IN  UINT32                RowAddrIncRate,
  IN  INT16                 RowAddrIncVal,
  IN  ADDR_INC_MODE         ColAddrIncMode,
  IN  UINT32                ColAddrIncRate,
  IN  INT16                 ColAddrIncVal
  );

/**

  This routine sets the CSRs required by BiosServerSetSeqRankMap

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] Controller                        - Controller (0-based)
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] PairCount                         - Number of entries in the DimmRankPairs array.
                                                 The minimum value is 1.  The value must be less than or
                                                 equal to the MaxNumberRankMapEntries value from the
                                                 GetCpgcInfo() function.
  @param[in] DimmRankPairs                     - Pointer to array of DIMM and physical rank pair values.
                                                 The valid DIMM/rank pairs can be determined via the
                                                 GetDimmBitMask() and GetRankInDimm() functions.  Note
                                                 that the same rank may appear more than once in the map.
                                                 Note also that all ranks do not need to appear in the map.

  @retval N/A

**/
VOID
BiosServerSetSeqRankMapChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 Controller,
  IN  UINT8                 ChannelInSocket,
  IN  UINT8                 PairCount,
  IN  CONST DIMM_RANK_PAIR  *DimmRankPairs
  );

/**

  This routine sets the CSRs required by BiosServerSetSeqRowAddrSwizzle

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] Controller                        - Controller (0-based)
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] SwizzleCount                      - Number of entries in the Swizzles array.  The value must be less than or equal to the
                                                 MaxNumberRowAddrSwizzleEntries value from the GetCpgcInfo() function.  If the
                                                 SwizzleCount value is 0 (and the Swizzles value is NULL) then the row address swizzling
                                                 will be configured so that all the lower row addresses are mapped to themselves; effectively
                                                 disabling the swizzling.
  @param[in] Swizzles                          - Pointer to array of lower row address swizzle values.  The values must be unique and less
                                                 than or equal to the MaxRowAddrSwizzleVal value from the GetCpgcInfo() function.  The
                                                 array position indicates the row address that the specified row address will be swapped
                                                 with, e.g., a value of 10 at the array index 1 means that row addresses 10 and 1 will be swapped.

  @retval N/A

**/
VOID
BiosServerSetSeqRowAddrSwizzleChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 Controller,
  IN  UINT8                 ChannelInSocket,
  IN  UINT8                 SwizzleCount,
  IN  CONST UINT8           *Swizzles
  );

/**

  This routine gets the CSRs required by BiosServerGetSeqState

  @param[in]  Host                              - Pointer to sysHost
  @param[in]  Socket                            - Socket Id
  @param[in]  Controller                        - Controller (0-based)
  @param[in]  ChannelInSocket                   - Channel number (0-based)
  @param[out] SeqState                          - Pointer to buffer where sequence state will be stored.

  @retval N/A

**/
VOID
BiosServerGetSeqStateChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 Controller,
  IN  UINT8                 ChannelInSocket,
  OUT SSA_SEQ_STATE         *SeqState
  );

/**

  This routine sets the CSRs required by BiosServerSetStopMode

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] StopMode                          - CPGC stop mode.
  @param[in] StopOnNthErrorCount               - Error count for stop-on-nth-any-lane error mode.  Minimum value is 1.
                                                 The value must be less than or equal to the MaxStopOnNthErrorCountVal
                                                 value from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetStopModeChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 ChannelInSocket,
  IN  CPGC_STOP_MODE        StopMode,
  IN  UINT32                StopOnNthErrorCount
  );

/**

  This routine sets the CSRs required by BiosServerSetErrorCounterMode

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] Counter                           - Zero based error counter number.  The value must be less than or
                                                 equal to the MaxNumberErrorCounters value from the GetCpgcInfo() function.
  @param[in] Mode                              - Error counter mode.
  @param[in] ModeIndex                         - Extra index used to provide additional information if needed by the
                                                 mode selected. This indicates which lane, nibble, byte, or chunk has
                                                 been selected.  If CounterMode value is LaneErrorCounterMode then the
                                                 ModeIndex value must be less than the BusWidth value from the
                                                 GetSystemInfo() function.  If CounterMode is NibbleErrorCounterMode
                                                 then the ModeIndex value must be less than the BusWidth/4 value from
                                                 the GetSystemInfo() function.  If CounterMode is ByteErrorCounterMode
                                                 then the ModeIndex value must be less than the BusWidth/8 value from the
                                                 GetSystemInfo() function.  If CounterMode is ChunkErrorCounterMode then
                                                 the ModeIndex value must be less than 8.

  @retval N/A

**/
VOID
BiosServerSetErrorCounterModeChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 ChannelInSocket,
  IN  UINT8                 Counter,
  IN  ERROR_COUNTER_MODE    Mode,
  IN  UINT32                ModeIndex
  );

/**

  This routine sets the CSRs required by BiosServerSetLaneValidationMask

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] DqMaskLow                         - Lower 32 bits of the DQ Mask (0-based)
  @param[in] DqMaskHigh                        - Upper 32 bits of the DQ Mask (0-based)
  @param[in] EccMask                           - Ecc Mask (0-based)


  @retval N/A

**/
VOID
BiosServerSetLaneValidationMaskChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 ChannelInSocket,
  IN  UINT32                DqMaskLow,
  IN  UINT32                DqMaskHigh,
  IN  UINT32                EccMask32
  );

/**

  This routine sets the CSRs required by BiosServerSetChunkValidationMask

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] ChunkPairsToCheck                 - One bit for each chunk of a cacheline. Used to identify each pair of chunks to be checked.

  @retval N/A

**/
VOID
BiosServerSetChunkValidationMaskChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 ChannelInSocket,
  IN  UINT8                 ChunkPairsToCheck
  );

/**

  This routine sets the CSRs required by BiosServerSetCachelineValidationMask

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] CachelineMask                     - Mask for cachelines to be enabled.
                                                 Bit value 1 = perform validation;
                                                 bit value 0 = do not perform validation.
                                                 Bit position 0 = cacheline 0;
                                                 bit position 1 = cacheline 1, etc.
                                                 Note that this may not be the same logic as the hardware.

  @retval N/A

**/
VOID
BiosServerSetCachelineValidationMaskChip (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  UINT8                 ChannelInSocket,
  IN  UINT8                 CachelineMask
  );
/**

  This routine gets the CSRs required by BiosServerGetChunkErrorStatus

  @param[in]  Host                              - Pointer to sysHost
  @param[in]  Socket                            - Socket Id
  @param[in]  ChannelInSocket                   - Channel number (0-based)
  @param[out] Status                            - Pointer to where the chunk error status will be stored.
                                                  Bit value 1 = at least one error occurred;
                                                  bit value 0 = no errors occurred.
                                                  Bit position 0 = chunk 0;
                                                  bit position 1 = chunk 1, etc.

  @retval N/A

**/
VOID
BiosServerGetChunkErrorStatusChip (
  IN  PSYSHOST                       Host,
  IN  UINT8                          Socket,
  IN  UINT8                          ChannelInSocket,
  OUT UINT8                          *Status
  );

/**

  This routine gets the CSRs required by BiosServerGetErrorAddress

  @param[in]  Host                              - Pointer to sysHost
  @param[in]  Socket                            - Socket Id
  @param[in]  ChannelInSocket                   - Channel number (0-based)
  @param[out] Address                           - Pointer to buffer where error address will be stored.

  @retval N/A

**/
VOID
BiosServerGetErrorAddressChip (
  IN  PSYSHOST                    Host,
  IN  UINT8                       Socket,
  IN  UINT8                       ChannelInSocket,
  OUT SSA_ADDRESS                 *Address
  );

/**

  This routine gets the CSRs required by BiosServerGetErrorCount

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] Counter                           - Zero based counter number.  The value must be less than or equal
                                                 to the MaxNumberErrorCounters value from the GetCpgcInfo() function.
  @param[out] Count                             - Pointer to where the error counter value will be stored.
  @param[out] Overflow                          - Pointer to where the error counter overflow status will be stored.

  @retval N/A

**/
VOID
BiosServerGetErrorCountChip (
  IN  PSYSHOST                 Host,
  IN  UINT8                    Socket,
  IN  UINT8                    ChannelInSocket,
  IN  UINT8                    Counter,
  OUT UINT32                   *Count,
  OUT BOOLEAN                  *Overflow
  );

/**

  This routine sets the CSRs required by BiosServerSetDqdbErrorCaptureConfig

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] Enable                            - If set to TRUE then cachelines for which errors are detected are written
                                                 to the DqDB starting at the StartCacheline and continuing to the
                                                 EndCacheline before wrapping around to the StartCacheline.  If set to
                                                 FALSE then the DqDB error capture feature is disabled.
  @param[in] StartCacheline                    - The starting cacheline in the DqDB where the error status will be captured.
                                                 The value must be less than the MaxNumberDqdbCachelines value from the GetCpgcInfo() function.
  @param[in] EndCacheline                      - The ending cacheline in the DqDB where the error status will be captured.
                                                 The value must be greater than or equal to the StartCacheline value.
                                                 The value must be less than the MaxNumberDqdbCachelines value from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetDqdbErrorCaptureConfigChip (
  IN  PSYSHOST                              Host,
  IN  UINT8                                 Socket,
  IN  UINT8                                 ChannelInSocket,
  IN  BOOLEAN                               Enable,
  IN  UINT8                                 StartCacheline,
  IN  UINT8                                 EndCacheline
  );

/**

  This routine gets the CSRs required by BiosServerGetDqdbErrorCaptureStatus

  @param[in]  Host                              - Pointer to sysHost
  @param[in]  Socket                            - Socket Id
  @param[in]  ChannelInSocket                   - Channel number (0-based)
  @param[out] CurrentCacheline                  - Pointer to where the current DqDB error capture cacheline value will be stored.

  @retval N/A

**/
VOID
BiosServerGetDqdbErrorCaptureStatusChip (
  IN  PSYSHOST                              Host,
  IN  UINT8                                 Socket,
  IN  UINT8                                 ChannelInSocket,
  OUT UINT8                                 *CurrentCacheline
  );

/**

  This routine sets the CSRs required by BiosServerSetLocalTestControl

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] Controller                        - Controller Id (0-based)
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] ClearErrors                       - Setting this parameter to TRUE causes all the channel's error counters and status to be cleared.
  @param[in] StartTest                         - Setting this parameter to TRUE starts the channel's CPGC engine.
  @param[in] StopTest                          - Setting this parameter to TRUE stops the channel's CPGC engine.

  @retval N/A

**/
VOID
BiosServerSetLocalTestControlChip (
  IN  PSYSHOST                              Host,
  IN  UINT8                                 Socket,
  IN  UINT8                                 Controller,
  IN  UINT8                                 ChannelInSocket,
  IN  BOOLEAN                               ClearErrors,
  IN  BOOLEAN                               StartTest,
  IN  BOOLEAN                               StopTest
  );

/**

  This routine sets the CSRs required by BiosServerSetGlobalTestControl

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] Controller                        - Controller Id (0-based)
  @param[in] ClearErrors                       - Setting this parameter to TRUE causes all the channel's error counters and status to be cleared.
  @param[in] StartTest                         - Setting this parameter to TRUE starts the channel's CPGC engine.
  @param[in] StopTest                          - Setting this parameter to TRUE stops the channel's CPGC engine.

  @retval N/A

**/
VOID
BiosServerSetGlobalTestControlChip (
  IN  PSYSHOST                              Host,
  IN  UINT8                                 Socket,
  IN  UINT8                                 Controller,
  IN  BOOLEAN                               ClearErrors,
  IN  BOOLEAN                               StartTest,
  IN  BOOLEAN                               StopTest
  );

/**

  This routine sets the CSRs required by BiosServerSetAddrConfig

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] Controller                        - Controller Id (0-based)
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] EnableDdr4                        - Setting this parameter to TRUE causes the CPGC engine to be in the DDR4 test mode,
                                                 otherwise it's in the NVMDIMM test mode.

  @retval N/A

**/
VOID
BiosServerSetAddrConfigChip (
  IN  PSYSHOST                              Host,
  IN  UINT8                                 Socket,
  IN  UINT8                                 Controller,
  IN  UINT8                                 ChannelInSocket,
  IN  BOOLEAN                               EnableDdr4
  );

/**

  This routine gets/sets the CSRs required by BiosServerSetScramblerConfig

  @param[in]  Host                              - Pointer to sysHost
  @param[in]  Socket                            - Socket Id
  @param[in]  Controller                        - Controller Id (0-based)
  @param[in]  Enable                            - Specifies whether the scrambler is enabled.
  @param[out] PrevEnable                        - Pointer to where the previous scrambler enable setting will be stored.  This value may be NULL.

  @retval N/A

**/
VOID
BiosServerSetScramblerConfigChip (
  IN  PSYSHOST                              Host,
  IN  UINT8                                 Socket,
  IN  UINT8                                 Controller,
  IN  BOOLEAN                               Enable,
  OUT BOOLEAN                               *PrevEnable
  );

/**

  This routine sets the CSRs required by BiosServerSetRefreshOverride

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] ChannelInSocket                   - ChannelInSocket No. (0-based)
  @param[in] EnablePanicRefreshOnly            - If set to TRUE then opportunistic and high priority auto refreshes will be
                                                 disabled for all ranks and refreshes will only occur when the panic refresh
                                                 watermark has been exceeded.  If set to FALSE then refreshes will occur
                                                 using the existing MC functional refresh logic.
  @param[in] PairCount                         - Number of entries in the DimmRankPairs array.  If you wish to have no rank's
                                                 refresh overridden then set PairCnt=0 and DimmRankPairs=NULL.  The value
                                                 must be less than the MaxNumberRankMapEntries value from the GetCpgcInfo() function.
  @param[in] DimmRankPairs                     - Pointer to array of DIMM and physical rank pair values.  If a DIMM/rank
                                                 appears in the list, then the CPGC engine will disable executing any
                                                 refresh to that rank while in REUT mode. If a DIMM/rank doesn't appear in the
                                                 list then the CPGC engine will enable executing a refresh (assuming the MC has
                                                 refreshes enabled) to that rank while in REUT mode based on the
                                                 EnablePanicRefreshOnly parameter value.  The valid DIMM/rank pairs can be
                                                 determined via the GetDimmBitMask() and GetRankInDimm() functions.

  @retval N/A

**/
VOID
BiosServerSetRefreshOverrideChip (
  IN  PSYSHOST                              Host,
  IN  UINT8                                 Socket,
  IN  UINT8                                 ChannelInSocket,
  IN  BOOLEAN                               EnablePanicRefreshOnly,
  IN  UINT8                                 PairCount,
  IN  CONST DIMM_RANK_PAIR                  *DimmRankPairs
  );

/**

  This routine sets the CSRs required by BiosServerSetZQCalOverride

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] ChannelInSocket                   - ChannelInSocket No. (0-based)
  @param[in] EnableZQCalAfterRefresh           - If set to TRUE then a ZQ operation will always take place after a refresh occurs
                                                 as INT32 as the DIMM/rank does not appear in the DimmRankPairs array.  If set
                                                 to FALSE then a ZQ operation will take place at the normal programmed period
                                                 after a refresh occurs as INT32 as the DIMM/rank does not appear in the
                                                 DimmRankPairs array.
  @param[in] PairCount                         - Number of entries in the DimmRankPairs array.  If you wish to have no rank's
                                                 ZQCal overridden then set PairCnt=0 and DimmRankPairs=NULL.  The value must be
                                                 less than the product of the MaxNumberDimms and MaxNumberRanks values from the
                                                 GetSystemInfo() function.
  @param[in] DimmRankPairs                     - Pointer to array of DIMM and physical rank pair values.  If a DIMM/rank appears
                                                 in the list, then the CPGC engine will disable executing any ZQCal to that rank
                                                 while in REUT mode.  The valid DIMM/rank pairs can be determined via the
                                                 GetDimmBitMask() and GetRankInDimm() functions.

  @retval N/A

**/
VOID
BiosServerSetZQCalOverrideChip (
  IN  PSYSHOST                              Host,
  IN  UINT8                                 Socket,
  IN  UINT8                                 ChannelInSocket,
  IN  BOOLEAN                               EnableZQCalAfterRefresh,
  IN  UINT8                                 PairCount,
  IN  CONST DIMM_RANK_PAIR                  *DimmRankPairs
  );

/**

  This routine sets the CSRs required by BiosServerSetOdtOverride

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] ChannelInSocket                   - ChannelInSocket No. (0-based)
  @param[in] EnableMprDdrTrain                 - This parameter is set to TRUE during DDR training.
  @param[in] ValCount                          - Number of entries in the DimmRankPairs and OverrideVals arrays.  If you wish to
                                                 have no rank's ODT overridden then set ValCnt=0, DimmRankPairs=NULL, and
                                                 OverrideVals=NULL.  The value must be less than twice the MaxNumberDimms value
                                                 from the GetSystemInfo() function.
  @param[in] DimmRankPairs                     - Pointer to array of DIMM and physical rank pair values.  If a DIMM/rank appears
                                                 in this array then its ODT value will be overridden with the value from the
                                                 corresponding index in the OverrideVals array.  If a DIMM/rank does not appear
                                                 in this array then its ODT will not be overridden.  The valid DIMM/rank pairs
                                                 can be determined via the GetDimmBitMask() and GetRankInDimm() functions.
  @param[in] OverrideVals                      - Pointer to array of override values.
                                                 Value 1 = enable ODT for that rank while in REUT mode;
                                                 value 0 = disable ODT for that rank while in REUT mode.

  @retval N/A

**/
VOID
BiosServerSetOdtOverrideChip (
  IN  PSYSHOST                              Host,
  IN  UINT8                                 Socket,
  IN  UINT8                                 ChannelInSocket,
  IN  BOOLEAN                               EnableMprDdrTrain,
  IN  UINT8                                 ValCount,
  IN  CONST DIMM_RANK_PAIR                  *DimmRankPairs,
  IN  CONST UINT8                           *OverrideVals
  );

/**

  This routine sets the CSRs required by BiosServerSetCkeOverride

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] ChannelInSocket                   - ChannelInSocket No. (0-based)
  @param[in] OverrideVals                      - Pointer to array of override values.
                                                 Value 1 = enable ODT for that rank while in REUT mode;
                                                 value 0 = disable ODT for that rank while in REUT mode.
  @param[in] ValCount                          - Number of entries in the DimmRankPairs and OverrideVals arrays.
                                                 If you wish to have no rank's CKE overridden then set ValCnt=0,
                                                 DimmRankPairs=NULL, and OverrideVals=NULL.  The value must be less
                                                 than the MaxNumberRankMapEntries value from the GetCpgcInfo() function.
  @param[in] DimmRankPairs                     - Pointer to array of DIMM and physical rank pair values.  If a DIMM/rank
                                                 appears in this array then its CKE value will be overridden with the
                                                 value from the corresponding index in the OverrideVals array.  If a
                                                 DIMM/rank does not appear in this array then its CKE will not be overridden.
                                                 The valid DIMM/rank pairs can be determined via the GetDimmBitMask()
                                                 and GetRankInDimm() functions.
  @param[in] OverrideVals                      - Pointer to array of override values.
                                                 Value 1 = enable CKE for that rank while in REUT mode;
                                                 value 0 = disable CKE for that rank while in REUT mode.

  @retval N/A

**/
VOID
BiosServerSetCkeOverrideChip (
  IN  PSYSHOST                              Host,
  IN  UINT8                                 Socket,
  IN  UINT8                                 ChannelInSocket,
  IN  UINT8                                 ValCount,
  IN  CONST DIMM_RANK_PAIR                  *DimmRankPairs,
  IN  CONST UINT8                           *OverrideVals
  );

/**

  Function used to configure the parity error event capture.

  @param[in out] Host                              - Pointer to sysHost
  @param[in]     Socket                            - Socket Id
  @param[in]     ChannelInSocket                   - Channel number (0-based)
  @param[in]     IgnoreAwordParityError            - Specifies whether to ignore Aword parity for the given channel.
  @param[in]     IgnoreDwordParityError            - Specifies whether to ignore Dword parity for the given channel.

  @retval NotAvailable if the system does not provide parity error event capture. Else Success.

**/
SSA_STATUS
BiosSetParityErrorConfigChip (
  IN OUT PSYSHOST                          host,
  IN     UINT8                             Socket,
  IN     UINT8                             ChannelInSocket,
  IN     BOOLEAN                           IgnoreAwordParityError,
  IN     BOOLEAN                           IgnoreDwordParityError
  );

/**

  Function used to get or set the IO level flag.

  @param[in out] Host                              - Pointer to sysHost
  @param[in]     Socket                            - Socket Id
  @param[in]     IsGet                             - Specifies whether the operation is a get or set.  TRUE = get; FALSE = set.
  @param[in out] MemSsType                         - Pointer to where the MemSsType value will be/is stored. Set the value pointed
                                                     to by MemSsType if IsGet is FALSE, while get the MemSsType if IsGet is TRUE

  @retval This function returns Success Status codes

**/
SSA_STATUS
BiosGetSetMemSsTypeChip (
  IN OUT PSYSHOST                          Host,
  IN     UINT8                             Socket,
  IN     BOOLEAN                           IsGet,
  IN OUT MRC_LT                            *MemSsType
  );

SSA_STATUS
BiosGetParityErrorStatusChip (
  IN OUT PSYSHOST                          host,
  IN     UINT8                             Socket,
  IN     UINT8                             ChannelInSocket,
  OUT    BOOLEAN                           *CurrentAwordParityError,
  OUT    BOOLEAN                           *ObservedAwordParityError,
  OUT    BOOLEAN                           *CurrentDwordParityError,
  OUT    BOOLEAN                           *ObservedDwordParityError
  );

/**
  Function used to get the per-strobe error Status.

  @param[in out] Host                              - Pointer to sysHost
  @param[in]     Socket                            - Socket Id
  @param[in]     ChannelInSocket                   - Channel number (0-based)
  @param[out]    Status                            - Pointer to where the strobe error Status will be stored.  Only the bits [3:0]
                                                     are valid because HBM has a 32-bit strobe Group.
                                                     Bit value 1 = at least one error occurred;
                                                     bit value 0 = no errors occurred.
                                                     Bit position 0 = strobe Group 0;
                                                     bit position 1 = strobe Group 1, etc.

  @retval NotAvailable if the system does not provide strobe error Status.  Else Success.

**/
SSA_STATUS
BiosGetStrobeErrorStatusChip (
  IN OUT PSYSHOST                          host,
  IN     UINT8                             Socket,
  IN     UINT8                             ChannelInSocket,
  OUT    UINT8                             *Status
  );

/**
  Function used to get or set the IO level flag.

  @param[in out] Host                              - Pointer to sysHost
  @param[in]     Socket                            - Socket Id
  @param[in]     IsGet                             - Specifies whether the operation is a get or set.  TRUE = get; FALSE = set.
  @param[in out] MemSsType                         - Pointer to where the MemSsType value will be/is stored. Set the value pointed
                                                     to by MemSsType if IsGet is FALSE, while get the MemSsType if IsGet is TRUE

  @retval This function returns Success Status codes
**/

SSA_STATUS
BiosGetSetMemSsTypeChip (
  IN OUT PSYSHOST                          Host,
  IN     UINT8                             Socket,
  IN     BOOLEAN                           IsGet,
  IN OUT MRC_LT                            *MemSsType
  );

/**

  Function used to set the DqDB Byte Group pattern select.

  @param[in out] host                          - Pointer to sysHost
  @param[in]     Socket                        - Socket Id
  @param[in]     ChannelInSocket               - Channel number (0-based)
  @param[in]     SelBitmask                    - Specifies the Byte Group pattern selection options.

  Bits 7:0 of the bitmask correspond to pseudo channel 0 "Byte groups" and bits
  15:8 correspond to pseudo channel 1 "Byte groups".  "Byte groups" is in
  quotes because, for HBM, it consists of 8 Dq signals and the corresponding
  ECC signal.  A bit value of 0 indicates Byte Group option 0 which maps the
  Byte Group bit lanes [ECC+DQ7:DQ0] to the WDB bit lanes [DQ8:DQ0].  A bit
  value of 1 indicates Byte Group option 1 which maps the Byte Group bit lanes
  [ECC+DQ7:DQ0] to the WDB bit lanes [DQ17:DQ9].

  @retval N/A

**/
VOID
BiosSetDqdbByteGroupPatSelChip (
  IN OUT PSYSHOST                          host,
  IN     UINT8                             Socket,
  IN     UINT8                             ChannelInSocket,
  IN     UINT16                            SelBitmask
  );

/**

  Function used to set the DqDB source for the DBI signals.

  @param[in out] Host                          - Pointer to sysHost
  @param[in]     Socket                        - Socket Id
  @param[in]     ChannelInSocket               - Channel number (0-based)
  @param[in]     SourceFromDqBit               - Specifies whether the DBI is sourced from DBI calculation in memory controller
                                                 (FALSE) or duplicated from one of the DQ bits (TRUE).
  @param[out]    DqBit                         - Specifies the zero based DQ bit (0 to 7) when the SourceFromDqLane is TRUE.

  @retval N/A

**/
VOID
BiosSetDqdbDbiDataSourceChip (
  IN OUT PSYSHOST                          host,
  IN     UINT8                             Socket,
  IN     UINT8                             ChannelInSocket,
  IN     BOOLEAN                           SourceFromDqBit,
  OUT    UINT8                             DqBit
  );

/**

  Function used to set the DqDB source for the data parity signals.

  @param[in]  Host                              - Pointer to sysHost
  @param[in]  Socket                            - Socket Id
  @param[in]  ChannelInSocket                   - Channel number (0-based)
  @param[in]  SourceFromDqBit                   - Specifies whether the data parity is sourced from data parity calculation
                                                  in memory controller (FALSE) or duplicated from one of the DQ bits (TRUE).
  @param[out] DqBit                             - Specifies the zero based DQ bit (0 to 7) when the SourceFromDqLane is TRUE

  @retval N/A

**/
VOID
BiosSetDqdbDataParityDataSourceChip (
  IN OUT PSYSHOST                          host,
  IN     UINT8                             Socket,
  IN     UINT8                             ChannelInSocket,
  IN     BOOLEAN                           SourceFromDqBit,
  OUT    UINT8                             DqBit
  );

/**

  Function used to configure whether duplicate the same cacheline transaction on both pseudo channels.

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] PseudoChannel                     - Pseudo channel configuration setting.

  @retval N/A

**/
VOID
BiosSetPseudoChannelConfigChip (
  IN OUT PSYSHOST                          host,
  IN     UINT8                             Socket,
  IN     UINT8                             ChannelInSocket,
  IN     PSEUDO_CH_SEL                     PseudoChannel
  );


/**

  This routine gets the CSRs required by BiosServerGetLaneErrorStatus

  @param[in]  Host                              - Pointer to sysHost
  @param[in]  Socket                            - Socket Id
  @param[in]  ChannelInSocket                   - Channel number (0-based)
  @param[in]  ChunkMask                         - Mask for the chunks. One bit for each chunk.  This mask should set
                                                  to be the same value as the mask being passed to the SetChunkValidationMask()
                                                  function.  Products don't have separate error status for even and odd UI
                                                  should ignore this parameter.
  @param[out] Status                            - Pointer to array where the lane error status values will be stored.  The number
                                                  of array elements is the BusWidth value from the GetSystemInfo() function
                                                  divided by 8.
                                                  Bit value 1 = at least one error occurred;
                                                  Bit value 0 = no errors occurred.

  @retval N/A

**/
VOID
BiosServerGetLaneErrorStatusChip (
  IN     PSYSHOST              Host,
  IN     UINT8                 Socket,
  IN     UINT8                 ChannelInSocket,
  IN     UINT8                 ChunkMask,
  OUT    UINT8                 *Status
  );

/**

  Perform RPQ drain operation

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Ch              - Channel number
  @param[in] Dimm            - DIMM number
  @param[in] Rank            - Rank number

  @retval N/A

**/
VOID
RPQDrainCadbMRSMode (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     Dimm,
  IN     UINT8     Rank
  );


/**

  Determine if this DIMM is DDRT or DDR4

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Ch              - Channel number

  @retval 1 - DIMM belongs to AEP Dimm; 0 - DIMM belongs to DDR4 Dimm

**/
UINT8
IsDdrtDimm (
  IN     PSYSHOST Host,
  IN     UINT8    Socket,
  IN     UINT8    Ch
  );

/**

  Setup Data Pattern Configuration and Data Pattern Sequencer registers

  @param[in] Host             - Pointer to sysHost, the system Host (root) structure
  @param[in] socket           - Memory controller number
  @param[in] ch               - channel
  @param[in] sequencerMode    - SequencerMode struct
  @param[in] sequencerPattern - Pointer to array of pattern sequences that are to be programmed

  @retval None

**/
VOID
SetupPatternSequencer (
  IN     PSYSHOST      Host,
  IN     UINT8         socket,
  IN     UINT8         ch,
  IN     SequencerMode sequencerMode,
  IN     UINT32        *sequencerPattern
  );

//
// Definition used with SetupIOTest SOE parameter
//

typedef enum {
  NSOE = 0,                       ///< Never Stop On Any Error
  NTHSOE,                         ///< Stop on the Nth Any Lane Error
  ABGSOE,                         ///< Stop on All Byte Groups Error
  ALSOE                           ///< Stop on All Lanes Error
} TStopOnError;

//
// Definition used with CmdPat SetupIOTest parameter
//

#define PatWrRd                             0
#define PatWr                               1
#define PatRd                               2
#define PatRdWrTA                           3
#define PatWrRdTA                           4
#define PatODTTA                            5
#define DimmTest                            6
#define PatRdWrRd                           7
#define PatWrRdFlyb                         8
#define PatWrRdAdvDq                        9
#define PatDummyWr                          10
#define PatPdaEnum                          11
#define PatWrUp                             12     // MemTest, MEMTEL ^(wD)
#define PatWrDown                           13     // MemTest, MEMTEL v(wD)
#define PatWrUpInvt                         14     // MemTest, MEMTEL ^(wI)
#define PatWrDownInvt                       15     // MemTest, MEMTEL v(wI)
#define PatRdUp                             16     // MemTest, MEMTEL ^(rD)
#define PatRdDown                           17     // MemTest, MEMTEL v(rD)
#define PatRdUpInvt                         18     // MemTest, MEMTEL ^(rI)
#define PatRdDownInvt                       19     // MemTest, MEMTEL v(rI)
#define PatRdWrUp                           20     // MemTest, MEMTEL ^(rD,wI)
#define PatRdWrDown                         21     // MemTest, MEMTEL v(rD,wI)
#define PatRdWrUpInvt                       22     // MemTest, MEMTEL ^(rI,wD)
#define PatRdWrDownInvt                     23     // MemTest, MEMTEL v(rI,wD)
#define PatRdWrUpAlt                        24     // MemTest, MEMTEL ^(rD,wD)
#define PatRdWrDownAlt                      25     // MemTest, MEMTEL v(rD,wD)
#define PatRdWrUpInvtAlt                    26     // MemTest, MEMTEL ^(rI,wI)
#define PatRdWrDownInvtAlt                  27     // MemTest, MEMTEL v(rI,wI)
#define PatTurnaroundSgSr                   28
#define PatTurnaroundDrDd                   29
#define PatBack2BackWrRd                    30     // BSSA WR...WR wait RD...RD wait
#define PatRdMrr                            31     // Mrr uses alternate data feature (fixed all zeros)
#define PatBack2BackWrWrRdRd                32     // WR...WR wait RD...RD wait
#define PatWrRdDdrt                         33

/**
     Programs all the key registers to define a CPCG test

      @param[in] Host           - Include all MRC global data.
      @param[in] socket         - Memory Controller
      @param[in] ChbitMask      - ch Bit mak for which test should be setup for.
      @param[in] CmdPat         - 0: PatWrRd (Standard Write/Read Loopback)
                                  1: PatWr (Write Only)
                                  2: PatRd (Read Only)
                                  3: PatRdWrTA (ReadWrite Turnarounds)
                                  4: PatWrRdTA (WriteRead Turnarounds)
                                  5: PatODTTA (ODT Turnaround)
      @param[in] NumCL          - Number of Cache lines
      @param[in] LC             - Loop Count
      @param[in] CPGCAddress    - Structure that stores start, stop and increment details for address
      @param[in] SOE            - [0: Never Stop, 1: Stop on Any Lane, 2: Stop on All Byte, 3: Stop on All Lane]
      @param[in] WDBPattern     - Structure that stores start, Stop, IncRate and Dqpat for pattern.
      @param[in] EnCADB         - Enable test to write random deselect packages on bus to create xtalk/isi
      @param[in] EnCKE          - Enable CKE power down by adding 64
      @param[in] SubSeqWait     - # of Dclks to stall at the end of a sub-sequence

    @retval N/A

**/
VOID
SetupIOTest (
  IN     PSYSHOST      Host,
  IN     UINT8         socket,
  IN     UINT32        ChbitMask,
  IN     UINT8         CmdPat,
  IN     UINT16        NumCL,
  IN     UINT32        LC,
  IN     TCPGCAddress  *CPGCAddress,
  IN     UINT8         SOE,
  IN     TWDBPattern   *WDBPattern,
  IN     UINT8         EnCADB,
  IN     UINT8         EnCKE,
  IN     UINT16        SubSeqWait
  );


/**

  Setup IO Test Basic VA

  @param[in] Host      - Pointer to sysHost
  @param[in] socket    - Socket from which we are going to read the registers
  @param[in] ChbitMask - ch Bit mask for which test should be setup for.
  @param[in] LC        - Loop Count
  @param[in] SOE       - [0: Never Stop, 1: Stop on Any Lane, 2:Stop on All Byte, 3: Stop on All Lane]
  @param[in] EnCADB    - Enable test to write random deselect packages on bus to create xtalk/isi
  @param[in] EnCKE     - Enable CKE power down by adding 64
  @param[in] Spread    - Pattern Spread

  @retval N/A

**/
VOID
SetupIOTestBasicVA (
  IN     PSYSHOST  Host,
  IN     UINT8     socket,
  IN     UINT32    ChbitMask,
  IN     UINT8     LC,
  IN     UINT8     SOE,
  IN     UINT8     EnCADB,
  IN     UINT8     EnCKE,
  IN     UINT32    Spread
  );

/**

  Setup IO Test MPR

  @param[in] Host      - Pointer to sysHost
  @param[in] socket    - Socket from which we are going to read the registers
  @param[in] ChbitMask - ch Bit mask for which test should be setup for.
  @param[in] LC        - Loop Count
  @param[in] SOE       - [0: Never Stop, 1: Stop on Any Lane, 2:Stop on All Byte, 3: Stop on All Lane]
  @param[in] EnCADB    - Enable test to write random deselect packages on bus to create xtalk/isi
  @param[in] EnCKE     - Enable CKE power down by adding 64

  @retval N/A

**/
VOID
SetupIOTestMPR (
  IN     PSYSHOST  Host,
  IN     UINT8     socket,
  IN     UINT32    ChbitMask,
  IN     UINT8     LC,
  IN     UINT8     SOE,
  IN     UINT8     EnCADB,
  IN     UINT8     EnCKE
  );

//
// Definition used with RunIOTest DQPat parameter
//

typedef enum {
  BasicVA = 0,                    ///<  Use 2 LFSR VicAggressor pattern with rotation of 10 bits
  SegmentWDB,                     ///<  Use 2 LFSR VA pattern 10 bit rotation + 3 LFSR VA pattern 6 bit rotation
  CADB,                           ///<  Do CADB on command/address bus and LMN VA on DQ (power supply noise)
  TurnAround,
  LMNVa,                          ///<  Use (LMN aggressor + LFSR Victim) with rotation of 10 bits
  TurnAroundWR,                   ///<  Run 8 tests, one for each subsequence with RankIncRate = 1/2/2/2/2/2/2/1
  TurnAroundODT,                  ///<  Run 4 tests, one for each subsequence with RankIncRate = 1/2/1/2
  DqAllSamePat,                   ///<  All dq lanes use same pattern
  DqAAAAAAAV,                     ///<  LFSR with 1 victim and 7 aggressors
  DqAAAVAAAV,                     ///<  LFSR with 1 victim and 3 aggressors
  DqAVAVAVAV                      ///<  LFSR with 1 victim and 1 aggressors
} TDqPat;

/**

  Runs one or more CPGC tests (based on TestType) and returns ch errors

  @param[in] Host        - Pointer to sysHost
  @param[in] socket      - Socket from which we are going to read the registers
  @param[in] ChBitMask   - ch Bit mask for which test should be setup for.
  @param[in] DQPat       - DQPat = [0: BasicVA, 1: SegmentWDB, 2: CADB, 3: TurnAround, 4: LMNVa]
  @param[in] SeqLCs      - SeqLCs is an array of one or more loopcounts.
  @param[in] ClearErrors - BOOLEAN to clear or not clear errors
  @param[in] Mode        - mode allows for different types of modes for margining:

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
RunIOTest (
  IN     PSYSHOST  Host,
  IN     UINT8     socket,
  IN     UINT32    ChBitMask,
  IN     UINT8     DQPat,
  IN     UINT8     *SeqLCs,
  IN     UINT8     ClearErrors,
  IN     UINT16    Mode
  );

/**
  Map physical ranks to CPGC logical ranks.

  @param[in]  Host                      - Pointer to sysHost
  @param[in]  Socket                    - Socket from which we are going to read the registers
  @param[in]  Ch                        - Channel
  @param[in]  Phy2LogicalRankMapping    - Physical to logical rank mapping
  @param[in]  LogicalRankCount          - Logical rank number
  @param[in]  RankFeatureEnable         - RankFeatureEnable is a bit mask that can enable Refresh or ZQ
  @param[in]  Phy2LogicalSubRankMapping - Sub-Rank number when 3DS dimm.

  @retval VOID

**/
VOID
MapCPGCRanks (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     (*Phy2LogicalRankMapping)[CPGC_RANK_MAPPING_MAX_NUMBER],
  IN UINT8     LogicalRankCount,
  IN UINT8     RankFeatureEnable,
  IN UINT8     (*Phy2LogicalSubRankMapping)[2]
  );

/**
  Programs CPGC to run on the selected logical ranks

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket from which we are going to read the registers
  @param[in] Ch                - Channel
  @param[in] RankBitMask       - Bit mask of logical ranks to work on
  @param[in] RankFeatureEnable - RankFeatureEnable is a bit mask that can enable CKE, Refresh or ZQ
  @param[in] SubRank           - Sub-Rank number when 3DS dimm. It applies to all the ranks specified by RankBitMask

  @retval 0
  @retval (UINT8) (1 << Ch)

**/
UINT8
SelectCPGCRanks (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     RankBitMask,
  IN     UINT8     RankFeatureEnable,
  IN     UINT8     SubRank
  );

//
// Definitions used with WriteCADBCmd Command parameter
//

#define MRS_CMD                             0
#define REF_CMD                             1
#define PRE_CMD                             2
#define ACT_CMD                             3
#define WR_CMD                              4
#define RD_CMD                              5
#define ZQ_CMD                              6
#define NOP_CMD                             7
#define RCW_CMD                             8
#define MRR_CMD                             9
#define MRW_CMD                             0xA
#define PREab_CMD                           0xB //Precharge all banks
#define PREsb_CMD                           0xC //Precharge same bank
#define PREpb_CMD                           0xD //Precharge per bank
#define MPC_CMD                             0x0F
#define RSVD_CMD                            0xff

/**

  this function writes to CADB

  @param[in] Host            - include all the MRC data
  @param[in] socket          - Memory Controller
  @param[in] ch              - ch to send command to
  @param[in] rank            - CS to send the command to
  @param[in] Command         - 0: MRS, 1: REF, 2: PRE, 3: ACT, 4: WR, 5: RD, 6: ZQ, 7: NOP
  @param[in] Bank            - MRS command to be sent
  @param[in] Data            - Value to be sent
  @param[in] Delay           - Delay in Dclocks

@retval UINT32 -  SUCCESS if the reset is succeded

**/
UINT32
WriteCADBCmd (
  IN     PSYSHOST  Host,
  IN     UINT8     socket,
  IN     UINT8     ch,
  IN     UINT8     dimm,
  IN     UINT8     rank,
  IN     UINT8     Command,
  IN     UINT8     Bank,
  IN     UINT32    Data,
  IN     UINT8     Delay
  );

typedef union {
  struct {
    UINT32  SubChABtyGrpDatS   :   4; // [3:0]   SubCh-A data lane [31:0] byte group error status.
    UINT32  SubChABtyGrpEccS   :   1; // [4]     SubCh-A ecc lane [7:0] byte group error status.
    UINT32  SubChANibEccEven   :   2; // [6:5]   SubCh-A ecc lane [7:0] even UI nibble error status.
    UINT32  SubChANibEccOdd    :   2; // [8:7]   SubCh-A ecc lane [7:0] odd UI nibble error status.
    UINT32  Rsvd0              :   7; // [15:9]  Reserved
    UINT32  SubChBBtyGrpDatS   :   4; // [19:16] SubCh-B data lane [31:0] byte group error status.
    UINT32  SubChBBtyGrpEccS   :   1; // [20]    SubCh-B ecc lane [7:0] byte group error status.
    UINT32  SubChBNibEccEven   :   2; // [22:21] SubCh-B ecc lane [7:0] even UI nibble error status.
    UINT32  SubChBNibEccOdd    :   2; // [24:23] SubCh-B ecc lane [7:0] odd UI nibble error status.
    UINT32  Rsvd1              :   7; // [31:25] Reserved
  } Bits;
  UINT32  Data;
} CPGC_ERR_STAT_MISC;

typedef union {
  struct {
    UINT32  SubChADataEven  :   8; // [7:0]   SubCh-A data lane [31:0] even UI nibble error status.
    UINT32  SubChADataOdd   :   8; // [15:8]  SubCh-A data lane [31:0] odd UI nibble error status.
    UINT32  SubChBDataEven  :   8; // [23:16] SubCh-B data lane [31:0] even UI nibble error status.
    UINT32  SubChBDataOdd   :   8; // [31:24] SubCh-B data lane [31:0] odd UI nibble error status.
  } Bits;
  UINT32  Data;
} CPGC_ERR_STAT_NIBBLE;

//
// Definition used with CpgcErrorStatus parameter
//
// DDR4 (Legacy):
// cpgcErrDat0S           - Data lane [31:0] even UI error status.
// cpgcErrDat1S           - Data lane [63:32] even UI error status.
// cpgcErrDat2S           - Data lane [31:0] odd UI error status.
// cpgcErrDat3S           - Data lane [63:32] odd UI error status.
// cpgcErrEccS            - [7:0]    - Ecc lane [7:0] even UI error status.
//                        - [15:8]   - Ecc lane [7:0] odd UI error status.
// cpgcErrNibS            - NA
// cpgcErrBytGrpS         - NA
//
// DDR5:
// cpgcErrDat0S           - Repo Inactive - SubCh-A data lane [31:0] even UI error status.
//                        - Repo Active   - SubCh-A data lane [31:0] UI error status.
// cpgcErrDat1S           - Repo Inactive - SubCh-B data lane [31:0] even UI error status.
//                        - Repo Active   - SubCh-B data lane [31:0] UI error status.
// cpgcErrDat2S           - Repo Inactive - SubCh-A data lane [31:0] odd UI error status.
//                        - Repo Active   - Same to cpgcErrDat0S
// cpgcErrDat3S           - Repo Inactive - SubCh-B data lane [31:0] odd UI error status.
//                        - Repo Active   - Same to cpgcErrDat1S
// cpgcErrEccS            - Repo Inactive
//                        - [7:0]    - SubCh-A ecc lane [7:0] even UI error status.
//                        - [15:8]   - SubCh-A ecc lane [7:0] odd UI error status.
//                        - [23:16]  - SubCh-B ecc lane [7:0] even UI error status.
//                        - [31:24]  - SubCh-B ecc lane [7:0] odd UI error status.
//                        - Repo Active
//                        - [7:0]    - SubCh-A ecc lane [7:0] UI error status.
//                        - [15:8]   - Same to [7:0]
//                        - [23:16]  - SubCh-B ecc lane [7:0] UI error status.
//                        - [31:24]  - Same to [23:16]
// cpgcErrNibS            - [7:0]    - SubCh-A data lane [31:0] even UI nibble error status.
//                        - [15:8]   - SubCh-A data lane [31:0] odd UI nibble error status.
//                        - [23:16]  - SubCh-B data lane [31:0] even UI nibble error status.
//                        - [31:24]  - SubCh-B data lane [31:0] odd UI nibble error status.
// cpgcErrMiscS           - [3:0]    - SubCh-A data lane [31:0] byte group error status.
//                        - [4]      - SubCh-A ecc lane [7:0] byte group error status.
//                        - [6:5]    - SubCh-A ecc lane [7:0] even UI nibble error status.
//                        - [8:6]    - SubCh-A ecc lane [7:0] odd UI nibble error status.
//                        - [19:16]  - SubCh-B data lane [31:0] byte group error status.
//                        - [20]     - SubCh-B ecc lane [7:0] byte group error status.
//                        - [22:21]  - SubCh-B ecc lane [7:0] even UI nibble error status.
//                        - [24:22]  - SubCh-B ecc lane [7:0] odd UI nibble error status.
//
typedef struct CpgcErrorStatus {
  UINT32                   cpgcErrDat0S;
  UINT32                   cpgcErrDat1S;
  UINT32                   cpgcErrDat2S;
  UINT32                   cpgcErrDat3S;
  UINT32                   cpgcErrEccS;
  CPGC_ERR_STAT_NIBBLE     cpgcErrNibS;
  CPGC_ERR_STAT_MISC       cpgcErrMiscS;
} CPGC_ERROR_STATUS, *PCPGC_ERROR_STATUS;

//
// Definition used with CpgcErrorCountType parameter
//
typedef enum {
  CpgcErrCntAllLanesOnAllUi = 0,  // Count errors on all lanes, 1 count per UI with an error, all UI
  CpgcErrCntAllLanesOnEvenUi,     // Count errors on all lanes, 1 count per UI with an error, even UI
  CpgcErrCntAllLanesOnOddUi,      // Count errors on all lanes, 1 count per UI with an error, odd UI
  CpgcErrCntUiOnAllLanes,         // Count errors on a particular UI, 1 count per UI with an error across all lanes
  CpgcErrCntLaneOnAllUi,          // Count errors on a particular lane, 1 count per UI with an error, all UI
  CpgcErrCntLaneOnEvenUi,         // Count errors on a particular lane, 1 count per UI with an error, even UI
  CpgcErrCntLaneOnOddUi,          // Count errors on a particular lane, 1 count per UI with an error, odd UI
  CpgcErrCntByteGroupOnAllUi,     // Count errors on a particular byte group, 1 count per UI with an error across the lanes in the byte group, all UI
  CpgcErrCntByteGroupOnEvenUi,    // Count errors on a particular byte group, 1 count per UI with an error across the lanes in the byte group, even UI
  CpgcErrCntByteGroupOnOddUi,     // Count errors on a particular byte group, 1 count per UI with an error across the lanes in the byte group, odd UI
  CpgcErrCntNibbleGroupOnAllUi,   // Count errors on a particular nibble group, 1 count per UI with an error across the lanes in the nibble group, all UI
  CpgcErrCntNibbleGroupOnEvenUi,  // Count errors on a particular nibble group, 1 count per UI with an error across the lanes in the nibble group, even UI
  CpgcErrCntNibbleGroupOnOddUi    // Count errors on a particular nibble group, 1 count per UI with an error across the lanes in the nibble group, all UI
} CPGC_ERROR_COUNT_TYPE;

//
// Definition used with CpgcErrorCountStatus parameter
//
typedef struct CpgcErrorCountStatus {
  UINT32 Count;
  BOOLEAN Overflow;
} CPGC_ERROR_COUNT_STATUS, *PCPGC_ERROR_COUNT_STATUS;

/**

  Collect DDR advanced command CPGC error status

  @param[in]  Host                - Pointer to sysHost
  @param[in]  Socket              - Socket to train
  @param[in]  DdrChEnMap          - 4-bit bit mask of ddr channels to test - (1 = active channel)
  @param[out] CpgcErrorStatus     - error status for each channel

  @retval n/a

**/
VOID
CpgcAdvTrainingErrorStatusDdr5 (
  IN     PSYSHOST          Host,
  IN     UINT8             Socket,
  IN     UINT32            DdrChEnMap,
  IN OUT CPGC_ERROR_STATUS CpgcErrorStatus[MAX_CH][SUB_CH]
  );

/**

  Collect DDR advanced command CPGC error status

  @param[in]  Host                - Pointer to sysHost
  @param[in]  Socket              - Socket to train
  @param[in]  DdrChEnMap          - 4-bit bit mask of ddr channels to test - (1 = active channel)
  @param[out] CpgcErrorStatus     - error status for each channel

  @retval n/a

**/
VOID
CpgcAdvTrainingErrorStatusDdr51 (
  IN     PSYSHOST          Host,
  IN     UINT8             Socket,
  IN     UINT32            DdrChEnMap,
  IN OUT CPGC_ERROR_STATUS_OLD CpgcErrorStatus[MAX_CH][SUB_CH]
  );



/**

 Get the CPGC errors in [SubCh][Ch] format

 @param[in]   Host        Pointer to syshost
 @param[in]   Socket      Current socket under test (0-based)
 @param[in]   DdrChEnMap  One hot variable to indicate if an specific channel has to be executed
 @param[out]  CpgcErrorStatus Pointer array to return the CPGC results

  @retval     MRC_STATUS_SUCCESS      When function completes execution without issues

**/
MRC_STATUS
CpgcAdvTrainingErrorStatusAdapter (
  IN        PSYSHOST          Host,
  IN        UINT8             Socket,
  IN        UINT32            DdrChEnMap,
      OUT   CPGC_ERROR_STATUS CpgcErrorStatus[SUB_CH][MAX_CH]
  );

/**

  This function writes MPC Command by CADB in BRS

  @param[in] Host            - include all the MRC data
  @param[in] Socket          - Memory Controller
  @param[in] Ch              - ch to send command to
  @param[in] Dimm            - dimm to send command to
  @param[in] SubChannelMask  - sub channel mask
  @param[in] Rank            - CS to send the command to
  @param[in] Opcode          - opcode to use
  @param[in] Timingmode      - timingmode to use. Support MPC_1N, MPC_2N, MPC_4N.
  @param[in] PassThroughMode - Filter to determine which register to copy.

  @retval UINT32 -  SUCCESS if the reset is succeded

**/
VOID
WriteCadbMpcCmd (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     Dimm,
  IN     UINT8     SubChannelMask,
  IN     UINT8     Rank,
  IN     UINT16    Opcode,
  IN     UINT8     Timingmode,
  IN     UINT8     PassThroughMode
  );

/**
 Save CPGC Rank Logical to physical mapping.

  @param[in]  Host                      - Pointer to sysHost
  @param[in]  Socket                    - Socket from which we are going to read the registers
  @param[in]  CpgcSeqRankL2PMapping     - Array to store the CPGC rank L2P mapping

  @retval VOID

**/
VOID
EFIAPI
SaveCpgcRankL2pMapping (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    CpgcSeqRankL2PMapping[MAX_CH][SUB_CH][2]
  );

/**
  Restore the CPGC rank logical to physical mapping.

  @param[in]  Host                      - Pointer to sysHost
  @param[in]  Socket                    - Socket from which we are going to read the registers
  @param[in]  CpgcSeqRankL2PMapping     - CPGC rank L2P mapping to be restored

  @retval VOID

**/
VOID
EFIAPI
RestoreCpgcRankL2pMapping (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    CpgcSeqRankL2PMapping[MAX_CH][SUB_CH][2]
  );

/**

This function writes Vref CS Command by CADB

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Ch              - Ch to send command to
  @param[in] Dimm            - Dimm to send command to
  @param[in] Subchannelmask  - Sub channel mask
  @param[in] Rank            - CS to send the command to
  @param[in] Opcode          - Opcode to use
  @param[in] Timingmode      - Supports TIMING_SINGLE_CS, TIMING_MULTI_CS
  @param[in] PassThroughMode - PassThrough mode or not

  @retval N/A

**/
VOID
WriteCadbVrefCsCmd (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Subchannelmask,
  IN UINT8     Rank,
  IN UINT16    Opcode,
  IN UINT8     Timingmode,
  IN BOOLEAN   PassThroughMode
);

/**

This function writes Vref CA Command by CADB

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Ch              - Ch to send command to
  @param[in] Dimm            - Dimm to send command to
  @param[in] Subchannelmask  - Sub channel mask
  @param[in] Rank            - CS to send the command to
  @param[in] Opcode          - Opcode to use
  @param[in] Timingmode      - Supports TIMING_SINGLE_CS, TIMING_MULTI_CS
  @param[in] PassThroughMode - PassThrough mode or not

  @retval N/A

**/
VOID
WriteCadbVrefCaCmd (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Subchannelmask,
  IN UINT8     Rank,
  IN UINT16    Opcode,
  IN UINT8     Timingmode,
  IN BOOLEAN   PassThroughMode
);

/**

  This function writes Vref CA Command by CADB in PDA mode

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Ch              - Ch to send command to
  @param[in] Dimm            - Dimm to send command to
  @param[in] Subchannelmask  - Sub channel mask
  @param[in] Rank            - CS to send the command to
  @param[in] Strobe          - Current Strobe (0-Based)
  @param[in] Opcode          - Opcode to use
  @param[in] Timingmode      - Supports TIMING_SINGLE_CS, TIMING_MULTI_CS
  @param[in] PassThroughMode - PassThrough mode or not

  @retval N/A

**/
VOID
WriteCadbVrefCaPdaCmd (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Subchannelmask,
  IN UINT8     Rank,
  IN UINT8     Strobe,
  IN UINT16    Opcode,
  IN UINT8     Timingmode,
  IN BOOLEAN   PassThroughMode
  );

/**

  This function sends Precharge command by CADB for DDR5.

  @param[in] Host            - Include all the MRC data
  @param[in] Socket          - Socket number
  @param[in] Ch              - Ch to send command to
  @param[in] Dimm            - Dimm to send command to
  @param[in] Subchannelmask  - Sub channel mask
  @param[in] Rank            - CS to send the command to
  @param[in] Bank            - Bank number
  @param[in] BankGroup       - Bank group number
  @param[in] Cmd             - Jedec command
                             - PREab_CMD: Precharge All, CA[3-0]=1011b, CA[4]=0b, CA[9:6]=vvvvb
                             - PREsb_CMD: Precharge Same Bank, CA[3-0]=1011b, CA[4]=0b, CA[9:6]=vvxxb, CA[10]=1b
                             - PREpb_CMD: Precharge, CA[3-0]=1011b, CA[4]=1b, CA[10:6]=xxxxxb

  @retval N/A

**/
VOID
WriteCadbPrechargeCmdDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Subchannelmask,
  IN UINT8     Rank,
  IN UINT8     Bank,
  IN UINT8     BankGroup,
  IN UINT8     Cmd
  );

/**

  This function sends mode register read or write command by CADB for DDR5.

  @param[in] Host            - Include all the MRC data
  @param[in] Socket          - Socket number
  @param[in] Ch              - Ch to send command to
  @param[in] Dimm            - Dimm to send command to
  @param[in] SubChMask       - Sub channel mask
  @param[in] Rank            - CS to send the command to
  @param[in] Address         - Mode register Address
  @param[in] Opcode          - Opcode to use
  @param[in] Cw              - CW to use
  @param[in] Cmd             - Which command: MRR_CMD or MRW_CMD

  @retval N/A

**/
VOID
WriteCadbMrCmd (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     SubChMask,
  IN UINT8     Rank,
  IN UINT16    Address,
  IN UINT16    Opcode,
  IN UINT8     Cw,
  IN UINT8     Cmd
  );

/**

  Read CPGC repo contents for particular mode register which is specified by Address parameter.
  For two sub-channels, the first half of data array for sub-channel A and the second half for
  sub-channel B.

  X8 dimm   SubChMSVx8 = 5
  LogicalStrobe     CPGC lanes
  0                 [7:0]
  1                 [15:8]
  2                 [23:16]
  3                 [31:24]
  4                 [39:32] ECC

  X4 dimm   SubChMSVx4 = 10
  LogicalStrobe     CPGC lanes
  0                 [3:0]
  5                 [7:4]
  1                 [11:8]
  6                 [15:12]
  2                 [19:16]
  7                 [23:20]
  3                 [27:24]
  8                 [31:28]
  4                 [35:32] ECC
  9                 [39:36] ECC

  @param[in]  Socket          - Socket Id
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  SubChMask       - Sub-channel bit mask
  @param[in]  Dimm            - DIMM number (0-based)
  @param[out] Data            - Data array to store the returned result for all dram devices

  @retval MRC_STATUS_SUCCESS if read is successful
  @retval MRC_STATUS_FAILURE otherwise

  **/
MRC_STATUS
EFIAPI
ReadCpgcMrrData (
  IN     UINT8  Socket,
  IN     UINT8  Ch,
  IN     UINT8  SubChMask,
  IN     UINT8  Dimm,
     OUT UINT8  (*Data)[SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

  Early CS CLK training pattern sets up. DDR5 only.
  It is used to setup CS training patterns for all channels defined by input parameter "ChannelMask"

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket to train
  @param[in] ChannelMask      - Ch Bit mask for which test should be setup for
  @param[in] Signal           - Signal

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcCsClkPattern (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChannelMask,
  IN GSM_CSN   Signal
  );

/**

  Early QCS CLK training pattern sets up. DDR5 only.
  It is used to setup QCS training patterns for all channels defined by input parameter "ChannelMask"

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket to train
  @param[in] ChannelMask      - Ch Bit mask for which test should be setup for
  @param[in] Dimm             - Dimm Id
  @param[in] Rank             - Rank Number

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcQcsClkPattern (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChannelMask,
  IN UINT8     Dimm,
  IN UINT8     Rank
  );

/**

  Early CA CLK training pattern sets up.
  It is used to setup CA training patterns for all channels defined by input parameter "ChannelMask"

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket to train
  @param[in] ChannelMask      - Ch Bit mask for which test should be setup for
  @param[in] Rank             - Rank number
  @param[in] Subrank          - Subrank number
  @param[in] Sig              - Signal
  @param[in] Mode             - Programming mode
  @param[in] PatternLoop      - Pattern Loop

  @retval SUCCESS / FAILURE

  **/
UINT32
CpgcCaClkPattern (
  IN PSYSHOST                  Host,
  IN UINT8                     Socket,
  IN UINT32                    ChannelMask,
  IN UINT8                     Rank,
  IN UINT8                     Subrank,
  IN GSM_CSN                   Sig,
  IN CPGC_ADV_CMD_PARITY_MODE  Mode,
  IN UINT8                     PatternLoop
  );

/**

  Early QCA CLK training pattern sets up.
  It is used to setup QCA training patterns for all channels defined by input parameter "ChannelMask"

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket to train
  @param[in] ChannelMask         - Ch Bit mask for which test should be setup for
  @param[in] Rank                - Rank number
  @param[in] Subrank             - Subrank number
  @param[in] Sig                 - Signal
  @param[in] Mode                - Programming mode.

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcQcaClkPattern (
    IN PSYSHOST  Host,
    IN UINT8     Socket,
    IN UINT32    ChannelMask,
    IN UINT8     Rank,
    IN UINT8     Subrank,
    IN GSM_CSN   Sig,
    IN CPGC_ADV_CMD_PARITY_MODE  Mode
);

/**

  Early BCOM CLK training pattern sets up.
  It is used to setup BCOM CLK training patterns for all channels defined by input parameter "ChannelMask"

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket to train
  @param[in] ChannelMask         - Ch Bit mask for which test should be setup for
  @param[in] Rank                - Rank number in channel
  @param[in] Signal              - Signal to test

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcBcomClkPattern (
    IN PSYSHOST Host,
    IN UINT8    Socket,
    IN UINT32   ChannelMask,
    IN UINT8    RankInCh,
    IN GSM_CSN  Signal
);

/**

  This function sends NOP command by CADB.

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - socket number
  @param[in] Channel         - ch to send command to
  @param[in] Dimm            - dimm to send command to
  @param[in] Dubchannelmask  - sub channel mask
  @param[in] Rank            - CS to send the command to
  @param[in] Cw              - CW to use
  @param[in] Cmd             - which command: MRR_CMD or MRW_CMD
  @param[in] PassThroughMode - Filter to determine which register to copy.

  @retval N/A

**/
VOID
WriteCadbNopCmd (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Channel,
  IN     UINT8     Dimm,
  IN     UINT8     Subchannelmask,
  IN     UINT8     Rank,
  IN     UINT8     Cw,
  IN     UINT8     Cmd,
  IN     BOOLEAN   PassThroughMode,
  IN     BOOLEAN   ExitCATM
  );

/**

Routine Description:

  This function allows stacking of MRS commands in cadb buffer in WriteCADBCmd function. Stacking commands in CADB
  is more efficient as the MRS are only ~32 clks apart instead of spread out by a bunch of CSR accesses

Arguments:
  Host            - Pointer to sysHost
  socket          - Memory Controller
  ch              - ch to send command to
  dimm            - dimm to send command to
  Command         - NORMAL_MRS  - How WriteCADBCmd usually works
                  - LOAD_MRS - WriteCADBCmd will load commands into cadb buffer but not execute
                  - EXECUTE_MRS - WriteCADBCmd will execute the stack of commands


Returns:
 UINT32 -  SUCCESS if the reset is succeded

**/
VOID
MRSCmdStacking (
  IN     PSYSHOST  Host,
  IN     UINT8     socket,
  IN     UINT8     ch,
  IN     UINT8     dimm,
  IN     UINT8     Command
  );

/**

  this function writes to MR SPD

  @param[in] Host            - Pointer to sysHost
  @param[in] socket          - Memory Controller
  @param[in] ch              - ch to send command to
  @param[in] dimm            - dimm to send command to
  @param[in] rank            - CS to send the command to
  @param[in] dram            - dram that is target of PPR sequence
  @param[in] data            - Value to be sent
  @param[in] bank            - Base bank address number to test

**/
VOID
WriteMRSPDA (
  IN     PSYSHOST  Host,
  IN     UINT8     socket,
  IN     UINT8     ch,
  IN     UINT8     dimm,
  IN     UINT8     rank,
  IN     UINT8     dram,
  IN     UINT16    data,
  IN     UINT8     bank
  );

/**

  This function writes to MR 6

  @param[in] Host            - Pointer to sysHost
  @param[in] socket          - Memory Controller
  @param[in] ch              - ch to send command to
  @param[in] dimm            - dimm to send command to
  @param[in] rank            - CS to send the command to
  @param[in] data            - Value to be sent
  @param[in] bank            - Base bank address number to test

**/
VOID
WriteMR6 (
  IN     PSYSHOST  Host,
  IN     UINT8     socket,
  IN     UINT8     ch,
  IN     UINT8     dimm,
  IN     UINT8     rank,
  IN     UINT16    data,
  IN     UINT8     bank
  );



/**

Routine Description:

  This function writes to CADB

  @param[in] Host                - Pointer to sysHost
  @param[in] socket              - Memory Controller
  @param[in] ch                  - ch to send command to
  @param[in] dimm                - dimm to send command to
  @param[in] rank                - CS to send the command to
  @param[in] controlWordDataPtr  - Pointer to a 9 byte array of control word data
  @param[in] controlWordFunc     - Function index in the LR buffer
  @param[in] controlWordAddr     - Control Word Address
  @param[in] bufferAddr          - Address of the individual buffer to target...or 'FF' for all buffers

Returns:
  N/A

**/
VOID
WritePBA (
  IN     PSYSHOST  Host,
  IN     UINT8     socket,
  IN     UINT8     ch,
  IN     UINT8     Dimm,
  IN     UINT8     rank,
  IN     UINT8     *controlWordDataPtr,
  IN     UINT8     controlWordFunc,
  IN     UINT8     controlWordAddr,
  IN     UINT8     bufferAddr
  );



/**

  This function issues a ZQ cycle

  @param[in] Host            - sysHost data structure
  @param[in] socket          - Processor socket
  @param[in] ch              - Channel to issue the ZQ to
  @param[in] zqType          - Type of ZQ Calibration: Long or Short

  @retval UINT32 - SUCCESS if the reset is succeded

**/
UINT32
CpgcIssueZQ (
  IN     PSYSHOST  Host,
  IN     UINT8     socket,
  IN     UINT8     ch,
  IN     UINT8     zqType
  );

/**

  DDR advanced training CPGC setup

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket to train
  @param[in] DdrChEnMap  - 4-bit bit mask of ddr channels to test - (1 = channel is selected to run tests)
  @param[in] Dimm        - Target dimm
  @param[in] Rank        - logical rank number (0-7)
  @param[in] SubRank     - subrank number
  @param[in] Pattern     - ddr pattern (0-3) ( 0 = dq, 1 = cmd group 1, 2 = cmd group 2, 3 = cmd group 3)
  @param[in] DqLoopCount - dq sequence loop count - number of times dq sequence is executed (2^(dqLoopCount - 1))

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcAdvTrainingSetup (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT32    DdrChEnMap,
  IN     UINT8     Dimm,
  IN     UINT8     Rank,
  IN     UINT8     SubRank,
  IN     UINT32    Pattern,
  IN     UINT8     DqLoopCount
  );

/**

  Select another rank for testing with the same CPGC setup

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket to train
  @param[in] DdrChEnMap  - 4-bit bit mask of ddr channels to test - (1 = channel is selected to run tests)
  @param[in] Dimm        - Target dimm
  @param[in] Rank        - logical rank number (0-7)
  @param[in] SubRank     - sub-rank number
  @param[in] Pattern     - ddr pattern (0-3) ( 0 = dq, 1 = cmd group 1, 2 = cmd group 2, 3 = cmd group 3)

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcAdvTrainingModifyRank (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    DdrChEnMap,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     SubRank,
  IN UINT32    Pattern
  );



/**

  Enable deselect patterns

  @param[in] Host        - Pointer to sysHost
  @param[in] socket      - Socket to train
  @param[in] ddrChEnMap  - DDR channels to setup
  @param[in] Dimm        - Dimm to train
  @param[in] mode        - MODE_DESELECT
                           MODE_VA_DESELECT

  @retval SUCCESS / FAILURE

**/
VOID
EnableCADBDeselect (
  IN     PSYSHOST  Host,
  IN     UINT8     socket,
  IN     UINT32    ddrChEnMap,
  IN     UINT8     Dimm,
  IN     UINT16    mode
  );

/**

  CPGC global setup for Advanced memory test

  @param[in] Host                         - Pointer to sysHost
  @param[in] socket                       - Memory controller to train
  @param[in] ch                           - ddr channel to setup
  @param[in] rank                         - Logical rank number
  @param[in] dqLoopCount                  - CPGC sequence loop count - number of times sequence is executed (2^(dqLoopCount - 1))
  @param[in] testType                     - Advanced MemTest type
  @param[in] mode                         - Type of sequence MT_CPGC_WRITE, MT_CPGC_READ, or MT_CPGC_READ_WRITE
  @param[in] seqDataInv[MT_MAX_SUBSEQ]    - Enables pattern inversion per subsequence
  @param[in] patternQW[MT_PATTERN_DEPTH]  - 64-bit wide data pattern to use per UI
  @param[in] IsUseInvtPat                 - Info to indicate whether or not patternQW is inverted by comparing original pattern
  @param[in] uiShl[MT_PATTERN_DEPTH]      - Enables pattern rotation per UI
  @param[in] numCacheLines                - Number of cachelines to use in WDB
  @param[in] patternDepth                 - Number of UI to use in patternQW
  @param[in] direction                    - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN

  @retval SUCCESS

**/
UINT32
CpgcMemTestGlobalSetupMATS (
  IN     PSYSHOST      Host,
  IN     UINT8         socket,
  IN     UINT8         ch,
  IN     UINT8         rank,
  IN     UINT8         dqLoopCount,
  IN     UINT8         testType,
  IN     UINT8         mode,
  IN     UINT8         seqDataInv[MT_MAX_SUBSEQ],
  IN     UINT64_STRUCT patternQW[MT_PATTERN_DEPTH],
  IN     BOOLEAN       IsUseInvtPat,
  IN     UINT8         uiShl[MT_PATTERN_DEPTH],
  IN     UINT8         numCacheLines,
  IN     UINT8         patternDepth,
  IN     UINT8         direction
  );

/**

  CPGC setup per rank for Advanced Memory test

  @param[in] Host              - Pointer to sysHost
  @param[in] socket            - Memory controller to train
  @param[in] ddrChEnMap        - ddr channels to setup
  @param[in] logRank           - logical rank to setup
  @param[in] logSubRank        - logical subrank number to test
  @param[in] colAddressBits    - number of DRAM column address bits to test
  @param[in] rowAddressBits    - number of DRAM row address bits to test
  @param[in] bankAddressBits   - number of DRAM bank address bits to test
  @param[in] mode              - Type of sequence MT_CPGC_WRITE, MT_CPGC_READ, or MT_CPGC_READ_WRITE
  @param[in] AddressMode       - FAST_Y for column first, FAST_X for row first
  @param[in] direction         - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN
  @param[in] bank              - Base bank address number to test
  @param[in] baseBits          - Number of bank address bits to skip on increment (for BG interleave)
  @param[in] rowAddr[MAX_CH]   - Base row address to start testing
  @param[in] rowSize[MAX_CH]   - Number of rows to test
  @param[in] deviceMask[MAX_CH][3] - Bitmask of DQ lanes to not test
  @param[in] LineMask[MAX_CH]      - Each bit corresponds to a cache line; 1 means test, 0 do not test
  @param[in] StopOnErr[MAX_CH]     - 1 = stop on 1st error; 0 = stop on all DQ lanes error
  @param[in] seqDataInv[MT_MAX_SUBSEQ] - Enables pattern inversion per subsequence

  @retval None

**/
VOID
CpgcMemTestRankSetupMATSRowRange (
  IN     PSYSHOST  Host,
  IN     UINT8     socket,
  IN     UINT32    ddrChEnMap,
  IN     UINT8     logRank,
  IN     UINT8     logSubRank,
  IN     UINT8     colAddressBits[MAX_CH],
  IN     UINT8     rowAddressBits[MAX_CH],
  IN     UINT8     bankAddressBits[MAX_CH],
  IN     UINT8     mode,
  IN     UINT8     AddressMode,
  IN     UINT8     direction,
  IN     UINT8     bank,
  IN     UINT8     baseBits,
  IN     UINT32    rowAddr[MAX_CH],
  IN     UINT32    rowSize[MAX_CH],
  IN     UINT32    deviceMask[MAX_CH][3],
  IN     UINT8     LineMask[MAX_CH],
  IN     UINT8     StopOnErr[MAX_CH],
  IN     UINT8     seqDataInv[MT_MAX_SUBSEQ]
  );

//
// Definition used with CpgcAdvTrainingErrorStatusMATS
// cpgcErrorStatus parameter
//
// DDR4 (Legacy):
// cpgcErrDat0S           - Data lane [31:0] even UI error status.
// cpgcErrDat1S           - Data lane [63:32] even UI error status.
// cpgcErrDat2S           - Data lane [31:0] odd UI error status.
// cpgcErrDat3S           - Data lane [63:32] odd UI error status.
// cpgcErrEccS            - [7:0]    - Ecc lane [7:0] even UI error status.
//                        - [15:8]   - Ecc lane [7:0] odd UI error status.
// cpgcErrChunk           - [7:0]    - Chunk error status
// cpgcErrColumn          - Column address that error happens on
// cpgcErrColumnExt       - NA
// cpgcErrRow             - Row address that error happens on
// cpgcErrRowExt          - NA
// cpgcErrBank            - Bank address that error happens on
// cpgcErrRank            - Rank address that error happens on
// cpgcSubrank            - Subrank address that error happens on
//
// DDR5:
// cpgcErrDat0S           - Repo Inactive - SubCh-A data lane [31:0] even UI error status.
//                        - Repo Active   - SubCh-A data lane [31:0] UI error status.
// cpgcErrDat1S           - Repo Inactive - SubCh-B data lane [31:0] even UI error status.
//                        - Repo Active   - SubCh-B data lane [31:0] UI error status.
// cpgcErrDat2S           - Repo Inactive - SubCh-A data lane [31:0] odd UI error status.
//                        - Repo Active   - Same to cpgcErrDat0S
// cpgcErrDat3S           - Repo Inactive - SubCh-B data lane [31:0] odd UI error status.
//                        - Repo Active   - Same to cpgcErrDat1S
// cpgcErrEccS            - Repo Inactive
//                        - [7:0]    - SubCh-A ecc lane [7:0] even UI error status.
//                        - [15:8]   - SubCh-A ecc lane [7:0] odd UI error status.
//                        - [23:16]  - SubCh-B ecc lane [7:0] even UI error status.
//                        - [31:24]  - SubCh-B ecc lane [7:0] odd UI error status.
//                        - Repo Active
//                        - [7:0]    - SubCh-A ecc lane [7:0] UI error status.
//                        - [15:8]   - Same to [7:0]
//                        - [23:16]  - SubCh-B ecc lane [7:0] UI error status.
//                        - [31:24]  - Same to [23:16]
// cpgcErrChunk           - [15:0]   - SubCh-A chunk error status
//                        - [31:16]  - SubCh-B chunk error status
// cpgcErrColumn          - SubCh-A Column address that error happens on
// cpgcErrColumnExt       - SubCh-B Column address that error happens on
// cpgcErrRow             - SubCh-A Row address that error happens on
// cpgcErrRowExt          - SubCh-B Row address that error happens on
// cpgcErrBank            - [4:0]   SubCh-A Bank address that error happens on
//                        - [20:16] SubCh-B Bank address that error happens on
// cpgcErrRank            - [2:0]   SubCh-A Rank address that error happens on
//                        - [18:16] SubCh-B Rank address that error happens on
// cpgcSubrank            - [2:0]   SubCh-A Subrank address that error happens on
//                        - [18:16] SubCh-B Subrank address that error happens on
//

typedef struct _CPGC_ERROR_STATUS_MATS {
  UINT32 cpgcErrDat0S;
  UINT32 cpgcErrDat1S;
  UINT32 cpgcErrDat2S;
  UINT32 cpgcErrDat3S;
  UINT32 cpgcErrEccS;
  UINT32 cpgcErrChunk;
  UINT32 cpgcErrColumn;
  UINT32 cpgcErrColumnExt;
  UINT32 cpgcErrRow;
  UINT32 cpgcErrRowExt;
  UINT32 cpgcErrBank;
  UINT32 cpgcErrRank;
  UINT32 cpgcSubrank;
} CPGC_ERROR_STATUS_MATS;

/**

  Collect DDR advanced command CPGC error status

  @param[in]  Host                    - Pointer to sysHost
  @param[in]  socket                  - Memory controller number
  @param[in]  ddrChEnMap              - channel mask
  @param[out] cpgcErrorStatus[MAX_CH] - array of error status

  @retval   SUCCESS/FAILURE

**/
UINT32
CpgcAdvTrainingErrorStatusMATS (
  IN     PSYSHOST               Host,
  IN     UINT8                  socket,
  IN     UINT32                 ddrChEnMap,
     OUT CPGC_ERROR_STATUS_MATS cpgcErrorStatus[MAX_CH]
  );

/**

  Collect DDR advanced command CPGC error status

  @param[in]  Host                - Pointer to sysHost
  @param[in]  Socket              - Socket to train
  @param[in]  DdrChEnMap          - 4-bit bit mask of ddr channels to test - (1 = active channel)
  @param[out] CpgcErrorStatus     - error status for each channel

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcAdvTrainingErrorStatus (
  IN     PSYSHOST          Host,
  IN     UINT8             Socket,
  IN     UINT32            DdrChEnMap,
     OUT CPGC_ERROR_STATUS CpgcErrorStatus[MAX_CH]
  );

/**

  Loop based on PatternLength

  @param[in] PatternLength - Number of loops

  @retval dqLoopCount

**/
UINT8
PatternLengthToCpgcLoopCnt (
  IN     UINT32 PatternLength
  );

/**

  DDR advanced command CPGC execution

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Memory controller to train
  @param[in] DdrChEnMap  - 4-bit bit mask of ddr channels to test - (1 = channel is selected to run tests)
  @param[in] Pattern     - ddr pattern (0-3) ( 0 = dq, 1 = cmd group 1, 2 = cmd group 2, 3 = cmd group 3)

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcAdvTrainingExecTest (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    DdrChEnMap,
  IN UINT32    Pattern
  );

/**

  CPGC Global Test Start

  @param[in] Host        - Pointer to sysHost
  @param[in] socket      - Memory controller to train
  @param[in] ddrChEnMap  - 4-bit bit mask of ddr channels to test - (1 = channel is selected to run tests)
  @param[in] pattern     - ddr pattern (0-3) ( 0 = dq, 1 = cmd group 1, 2 = cmd group 2, 3 = cmd group 3)

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcAdvTrainingStartTest (
  IN    PSYSHOST  Host,
  IN    UINT8     socket,
  IN    UINT32    ddrChEnMap,
  IN    UINT16    pattern,
  IN    UINT8     SubCh
  );

/**

  DDR advanced command CPGC execution

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Memory controller to train
  @param[in] DdrChEnMap  - 4-bit bit mask of ddr channels to test - (1 = channel is selected to run tests)
  @param[in] Pattern     - ddr pattern (0-3) ( 0 = dq, 1 = cmd group 1, 2 = cmd group 2, 3 = cmd group 3)

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
CpgcAdvTrainingExecTestGroup (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT32    DdrChEnMap,
  IN    UINT32    Pattern
  );

/**

  CPGC Global Poll function

  @param[in]  Host      Pointer to sysHost
  @param[in]  Socket    Socket number
  @param[in]  ChMask    Bit mask of channels to use
  @param[in]  SubCh     Sub channel number

  @retval     SUCCESS

**/
UINT32
CpgcPollGlobalTestDone (
  IN        PSYSHOST        Host,
  IN        UINT8           Socket,
  IN        UINT32          ChMask,
  IN        UINT8           SubCh
  );

/**

  CPGC Global Start Stop

  @param[in]  Host      Pointer to sysHost
  @param[in]  Socket    Socket number
  @param[in]  ChMask    Bit mask of channels to use
  @param[in]  SubCh     Sub channel number

  @retval     SUCCESS

**/
UINT32
CpgcGlobalTestStart (
  IN        PSYSHOST        Host,
  IN        UINT8           Socket,
  IN        UINT32          ChMask,
  IN        UINT8           SubCh
  );

/**

    CPGC Global Test Stop

    @param[in] Host       - Pointer to sysHost
    @param[in] socket     - socket number
    @param[in] ddrChEnMap - bit mask of channels to use
    @param[in] SubCh      - Sub channel number

    @retval SUCCESS

**/
UINT32
CpgcGlobalTestStop (
  IN PSYSHOST  Host,
  IN UINT8     socket,
  IN UINT32    ddrChEnMap,
  IN UINT8     SubCh
  );

/**

  DDR loopback control training

  @param[in] Host    - Pointer to sysHost
  @param[in] socket  - Socket to train
  @param[in] ch      - DDR channel to setup
  @param[in] signal  - Signal to test

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcCtlClkPattern (
  IN    PSYSHOST  Host,
  IN    UINT8     socket,
  IN    UINT8     ch,
  IN    GSM_CSN   signal
  );

/**

  DDR parity based command training

  @param[in] Host     Pointer to sysHost
  @param[in] Socket   Socket to train
  @param[in] Ch       DDR channel to setup
  @param[in] Rank     Logical rank number
  @param[in] SubRank  Sub-rank number
  @param[in] Signal   Signal to test
  @param[in] Mode     CADB Mode (NormalCadbSetup, BacksideCadbSetup, or RestoreCadbPattern)

  @retval SUCCESS   All operations successful
  @retval FAILURE   Failure(s) occurred

**/
UINT32
CpgcAdvCmdParity (
  IN  PSYSHOST                  Host,
  IN  UINT8                     Socket,
  IN  UINT8                     Ch,
  IN  UINT8                     Rank,
  IN  UINT8                     SubRank,
  IN  GSM_CSN                   Signal,
  IN  CPGC_ADV_CMD_PARITY_MODE  Mode
  );

/**

  Loads the CADB

  @param Host       - Pointer to sysHost
  @param socket     - Socket to train
  @param ch         - DDR channel to setup
  @param subCh      - Sub channel number
  @param startLine  - start line number
  @param numLines   - cache line numbers
  @param prog0      - pattern to be programmed
  @param prog1      - pattern to be programmed

  @retval SUCCESS / FAILURE

**/
VOID
ProgramCADB (
  IN  PSYSHOST      Host,
  IN  UINT8         socket,
  IN  UINT8         ch,
  IN UINT8          subCh,
  IN  UINT8         startLine,
  IN  UINT8         numLines,
  IN  PatCadbProg0  prog0[CADB_LINES],
  IN  PatCadbProg1  prog1[CADB_LINES]
  );

/**

  Setup CPGC sequence

  @param[in]  Host            - Pointer to sysHost
  @param[in]  socket          - Socket number
  @param[in]  ch              - Channel number
  @param[in]  dimm            - DIMM number
  @param[in]  dpa             - Address consumed during Program Sequence Address
  @param[in]  mode            - Choose between READ_ACCESS or WRITE_ACCESS
  @param[in]  uiBitMask       - Error checking mask used during READ_ACCESS
  @param[in]  data            - Pointer to data to write to the WDB

  @retval 0 - success
  @retval 1 - failure

**/
VOID
CpgcSetup64 (
  IN  PSYSHOST      Host,
  IN  UINT8         socket,
  IN  UINT8         ch,
  IN  UINT8         dimm,
  IN  UINT32        dpa,
  IN  UINT8         mode,
  IN  UINT8         uiBitMask,
  IN  UINT64_STRUCT *data
  );


/**

  Clears the bitwise error mask registers

  @param[in] Host        - Pointer to sysHost
  @param[in] socket      - Processor socket to clear

**/
VOID
ClearBwErrorMask (
  IN  PSYSHOST        Host,
  IN  UINT8           socket
  );

/**

  Sets the IOSAV_CH_X_BW_SERR registers to mask off data bits from reporting failures

  @param[in] Host         - Pointer to sysHost
  @param[in] socket       - Processor socket to mask
  @param[in] dimm         - DIMM to mask
  @param[in] rank         - Rank to mask
  @param[in] mask         - Bits to be masked

**/
VOID
SetBwErrorMask (
  IN  PSYSHOST        Host,
  IN  UINT8           socket,
  IN  UINT8           dimm,
  IN  UINT8           rank,
  IN  struct bitMask  *mask
  );



/**

  CPGC setup for PPR Sequence

  @param[in] Host        - Pointer to sysHost
  @param[in] socket      - Memory controller to train
  @param[in] ch          - ddr channel to setup
  @param[in] dimm        - DIMM id
  @param[in] rank        - rank id
  @param[in] subRank     - sub rank ID
  @param[in] dramMask    - bitmap of dram that is target of PPR sequence
  @param[in] bank        - bank id
  @param[in] row         - row id

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
CpgcSetupPprPattern (
  IN  PSYSHOST  Host,
  IN  UINT8     socket,
  IN  UINT8     ch,
  IN  UINT8     dimm,
  IN  UINT8     rank,
  IN  UINT8     subRank,
  IN  UINT32    dramMask,
  IN  UINT8     bank,
  IN  UINT32    row
  );


/**

  DDR Mem test / Mem Init CPGC setup.

  @param[in] Host        - Pointer to sysHost
  @param[in] socket      - Memory controller to train
  @param[in] ch          - ddr channel to setup
  @param[in] dimm        - DIMM id
  @param[in] rank        - rank id
  @param[in] subRank     - sub rank ID
  @param[in] dramMask    - dram that is target of PPR sequence
  @param[in] bank        - bank id
  @param[in] row         - row id
  @param[in] pattern     - flag to indicate mem init vs. mem test and read vs. write and data pattern

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
CpgcPprTestGlobalSetup (
  IN  PSYSHOST  Host,
  IN  UINT8     socket,
  IN  UINT8     ch,
  IN  UINT8     dimm,
  IN  UINT8     rank,
  IN  UINT8     subRank,
  IN  UINT32    dramMask,
  IN  UINT8     bank,
  IN  UINT32    row,
  IN  UINT32    pattern
  );

/**

  Clean-up after PPR sequence is done

  @param[in] Host        - Pointer to sysHost
  @param[in] socket      - Memory controller to train
  @param[in] chBitMask   - Bit map of channels to mask off

**/
UINT32
CpgcPprCleanup (
  IN  PSYSHOST  Host,
  IN  UINT8     socket,
  IN  UINT32    chBitMask
  );

/**

  Check if the DIMM is a specific DIMM manufactured by Hynix

  @param[in] Host        - Pointer to sysHost
  @param[in] socket      - Memory controller to train
  @param[in] ch          - ddr channel to setup

  @retval 1 = Match
  @retval 0 = No Match

**/
UINT8
IsHynix2013 (
  IN  PSYSHOST Host,
  IN  UINT8    socket,
  IN  UINT8    ch
  );

/**

  Disable CADB AlwaysOn Mode

  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number

  @retval VOID

**/
VOID
DisableCADBAlwaysOnMode (
  IN UINT8 Socket,
  IN UINT8 Ch
  );

/**

  Program CPGC to enable or disable IN-ORDER execution


  @param[in] Socket             - Current Socket (0-Based)
  @param[in] Ch                 - Current Channel (0-Based)
  @param[in] Enable             - TRUE: Enable IN-ORDER, FALSE: Disable IN-ORDER

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
CpgcInorderModeEnable (
  IN        UINT8           Socket,
  IN        UINT8           Ch,
  IN        BOOLEAN         Enable
  );

/**
  Configure Uniseq Mode Before Training.

  @param[in]  Socket              Socket number
  @param[in]  Ch                  Channel number

  @retval N/A
**/
VOID
EFIAPI
ConfigureUniseqModeBeforeTraining (
  IN UINT8         Socket,
  IN UINT8         Ch
  );

/**
  Initialize CADB pattern buffer

  @param[in]  Host      - Pointer to SysHost
  @param[in]  socket    - Socket instance

  @retval N/A
**/
VOID
CADBChickenBit (
  IN   PSYSHOST Host,
  IN   UINT8    socket
  );

/**

  this function writes to CADB

  @param[in] Host        - Pointer to sysHost
  @param[in] socket      - Memory controller to train
  @param[in] ch          - ddr channel to setup
  @param[in] dimm        - DIMM id
  @param[in] rank        - rank id
  @param[in] side        - Side of DIMM
  @param[in] mprPage     - MPR Page
  @param[in] mprLoc      - Value used to set the bank to access

**/
VOID
ReadMprCADBSeq (
  IN   PSYSHOST  Host,
  IN   UINT8     socket,
  IN   UINT8     ch,
  IN   UINT8     dimm,
  IN   UINT8     rank,
  IN   UINT8     side,
  IN   UINT8     mprPage,
  IN   UINT8     mprLoc
  );

/**

  Reads MPR page/location from given rank and updates associated MprData per DRAM device

  @param[in]  Host        - Pointer to sysHost
  @param[in]  socket      - Memory controller to train
  @param[in]  ch          - ddr channel to setup
  @param[in]  dimm        - DIMM id
  @param[in]  rank        - rank id
  @param[in]  mprPage     - MPR Page
  @param[in]  mprLoc      - Value used to set the bank to access
  @param[out] MprData     - each byte is updated with UI[0-7] data per DRAM device

**/
VOID
ReadMprRankDdr4 (
  IN   PSYSHOST  Host,
  IN   UINT8     socket,
  IN   UINT8     ch,
  IN   UINT8     dimm,
  IN   UINT8     rank,
  IN   UINT8     mprPage,
  IN   UINT8     mprLoc,
  OUT  UINT8     MprData[MAX_STROBE]
  );

/**

  Reads MPR page/location from given rank and updates associated MprData per DRAM device

  @param[in]  Host        - Pointer to sysHost
  @param[in]  socket      - Socket Id
  @param[in]  ch          - ddr channel to setup
  @param[in]  dimm        - DIMM id
  @param[in]  bcwFunc     - BIOS Control Word Function
  @param[in]  bcwByte     - BIOS Control Word Byte
  @param[in]  bcwNibble   - BIOS Control Word Nibble
  @param[out] MprData     - each byte is updated with UI[0-7] data per DRAM device

**/
VOID
ReadBcwDimmDdr4 (
  IN   PSYSHOST  Host,
  IN   UINT8     socket,
  IN   UINT8     ch,
  IN   UINT8     dimm,
  IN   UINT8     bcwFunc,
  IN   UINT8     bcwByte,
  IN   UINT8     bcwNibble,
  OUT  UINT8     MprData[MAX_STROBE]
  );

/**

  Get the RTL Offset

  @param[out] rtlOffset    - location to hold the RTL offset

**/
VOID
InitRtlOffsetChip (
  OUT  UINT8     rtlOffset[2]
  );

/**

  This routine to set CpgcDdrtMiscCtl register
  @param[in]  Host                  - Host pointer
  @param[in]  Socket                - Socket index
  @param[in]  Ch                    - Channel index
  @param[in]  ChangeMaxCredit       - If non-zero then change the CPGC Max Credit
  @param[in]  CpgcMaxCredit         - Value used to change CPGC Max Credit
  @param[in]  DisableMaxCreditCheck - boolean
  @param[in]  EnableEridReturn      - boolean
  @param[in]  MultiCreditOn         - boolean

  @retval N/A

**/
VOID
SetCpgcDdrtMiscCtl (
  IN   PSYSHOST Host,
  IN   UINT8    Socket,
  IN   UINT8    Ch,
  IN   BOOLEAN  ChangeMaxCredit,
  IN   UINT8    CpgcMaxCredit,
  IN   BOOLEAN  DisableMaxCreditCheck,
  IN   BOOLEAN  EnableEridReturn,
  IN   BOOLEAN  MultiCreditOn
  );

/**

  This routine to set  multi_credit_on field in register CpgcDdrtMiscCtl

  @param[in]  Host                  - Host pointer
  @param[in]  Socket                - Socket index
  @param[in]  Ch                    - Channel index
  @param[in]  MultiCreditOn         - boolean

  @retval N/A

**/
VOID
SetCpgcMultiCreditOn (
  IN   PSYSHOST Host,
  IN   UINT8    Socket,
  IN   UINT8    Ch,
  IN   BOOLEAN  MultiCreditOn
  );


/**

  This routine to set CpgcRpqForceDrain in CpgcDdrtMiscCtl register

  @param[in]  Host                  - Host pointer
  @param[in]  Socket                - Socket index
  @param[in]  Ch                    - Channel index
  @param[in]  CpgcRpqForceDrain     - boolean

  @retval N/A

**/
VOID
SetCpgcRpqForceDrain (
  IN   PSYSHOST Host,
  IN   UINT8    Socket,
  IN   UINT8    Ch,
  IN   BOOLEAN  CpgcRpqForceDrain
  );


/**

  This routine to sets or clears refresh enable based on RefreshEnable

  @param[in]  Host                  - Host pointer
  @param[in]  Socket                - Socket index
  @param[in]  Ch                    - Channel index
  @param[in]  RefreshEnable         - 1 = set; 0 = clear

  @retval N/A

**/
VOID
SetRefreshEnable (
  IN   PSYSHOST  Host,
  IN   UINT8     Socket,
  IN   UINT8     Ch,
  IN   UINT8     RefreshEnable
  );

/**

  This routine to sets or clears refresh enable based on RefreshEnable
  In 10nm version, it introduced stagger host refresh enable feature

  @param[in]  Host                    - Host pointer
  @param[in]  Socket                  - Socket index
  @param[in]  TrefiPerAviableChannel  - Enbable Stragger Trefi Per Available Channel
  @param[in]  NumberOfRank            - Select number of rank to keep stagger host refresh apart
  @param[in]  TrefiDelay              - Apply addition Trefi Delay when enable Host Refresh across different iMC Group
  @retval N/A

**/
VOID
SetRefreshEnable10nm (
  IN   PSYSHOST  Host,
  IN   UINT8     Socket,
  IN   UINT8     TrefiPerAviableChannel,
  IN   UINT8     NumberOfRank,
  IN   UINT16    TrefiDelay
  );

/**

  This function needs to immediately follow the
  SetRefreshEnable10nm() funtion.  This provides an end label that C
  can use to read the entire function (ExitSelfRefresh10nm) into
  cache before executing it.

**/
UINT8
SetRefreshEnable10nmEndMarker (
  VOID
  );

/**

  This routine gets/sets CSRs required by BiosSetZQCalConfig

  @param[in]      Host                       - Pointer to sysHost
  @param[in]      Socket                     - Socket Id
  @param[in]      Controller                 - Controller number (0-based)
  @param[in]      Enable                     - Specifies whether ZQCal is enabled.  TRUE enables ZQCal; FALSE disables it
  @param[in, out] PrevEnable                 - Pointer to where the previous ZQCal enable setting will be stored. This value may be NULL

  @retval N/A

**/
VOID
BiosSetZQCalConfigChip (
  IN      PSYSHOST              Host,
  IN      UINT8                 Socket,
  IN      UINT8                 Controller,
  IN      BOOLEAN               Enable,
  IN OUT  BOOLEAN               *PrevEnable
  );


//
// APIs from MemConfigMc.c
//

// for SKX: from InitMemChip.c
/**

  This function applies the given ZQ mask

  @param[in] Host      - Pointer to sysHost
  @param[in] socket    - Socket
  @param[in] ch        - Channel
  @param[in] ZQMask    - ZQ mask to apply

  @retval none

**/
VOID
SetZQMask (
  IN      PSYSHOST  Host,
  IN      UINT8     socket,
  IN      UINT8     ch,
  IN      UINT8     ZQMask
  );

/**

  This function applies the given Refresh mask

  @param[in] Host      - Pointer to sysHost
  @param[in] socket    - Socket
  @param[in] ch        - Channel
  @param[in] refMask   - Refresh mask to apply

  @retval none

**/
VOID
SetRefreshMask (
  IN      PSYSHOST  Host,
  IN      UINT8     socket,
  IN      UINT8     ch,
  IN      UINT8     refMask
  );

/**

  This function asserts CKE on each channel and unmasks periodic refresh

  @param[in] Host    - Pointer to sysHost
  @param[in] socket  - Socket

  @retval none

**/
VOID
ExitSelfRefreshCriticalSection (
  IN      PSYSHOST  Host,
  IN      UINT8     socket
  );

/**
  this is the end marker for ExitSelfRefreshCriticalSection to read to Cache from flash
  to fasten S3 path and comply with timing requirement to exit self refresh

  @retval SUCCESS

**/
UINT8
ExitSelfRefreshCriticalSectionEndMarker (
  VOID
  );

/**

  Return address of CPGC_ERR_DATA0_S_MCDDC_CTL_REG

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Ch             - Channel number

  @retval  Physical address of CPGC_ERR_DATA0_S_MCDDC_CTL_REG

**/
UINT8 *
GetCpgcErrData0sAddr (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Ch
  );

//
// Pattern Buffer Mux Data used with SetCpgcPatControl
// patbuf0, patbuf1, and patbuf2 parameters
//

#define DDR_CPGC_PATBUF_MUX0 0xAAAAAA
#define DDR_CPGC_PATBUF_MUX1 0xCCCCCC
#define DDR_CPGC_PATBUF_MUX2 0xF0F0F0
#define DDR_CPGC_PATBUF_MUX3 0xFFFFFF

/**

  Set the CPGC Pattern

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket
  @param[in] ChbitMask - Ch Bit mask for which test should be setup for.
  @param[in] Mux0      - MUX 0
  @param[in] Mux1      - MUX 1
  @param[in] Mux2      - MUX 2
  @param[in] Patbuf0   - Pattern Buffer 0
  @param[in] Patbuf1   - Pattern Buffer 1
  @param[in] Patbuf2   - Pattern Buffer 2

**/
VOID
SetCpgcPatControl (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT32    ChbitMask,
  IN    UINT8     Mux0,
  IN    UINT8     Mux1,
  IN    UINT8     Mux2,
  IN    UINT32    Patbuf0,
  IN    UINT32    Patbuf1,
  IN    UINT32    Patbuf2
  );

/**

  This function gets the contents in the CPGC error results register

  @param[in] Host      - Pointer to sysHost
  @param[in] socket    - Socket
  @param[in] ch        - Channel

  @retval UINT32 value which is the contents of the chip specific register containing the CPGC error results

**/
UINT32
GetErrorResults (
  IN    PSYSHOST   Host,
  IN    UINT8      socket,
  IN    UINT8      ch
  );


/**

  This function clears the count and status in the ERID LFSR error register

  @param[in] Host      - Pointer to sysHost
  @param[in] socket    - Socket
  @param[in] ch        - Channel

**/
VOID
ClearEridLfsrErr (
  IN    PSYSHOST Host,
  IN    UINT8    socket,
  IN    UINT8    ch
  );

/**

  This function gets the contents of the ERID LFSR error register

  @param[in]  Host                 - Pointer to sysHost
  @param[in]  socket               - Socket
  @param[in]  ch                   - Channel
  @param[out] eridCombinedResult   - Pointer to the location to store the date read

**/
VOID
GetEridCombineResult (
  IN    PSYSHOST Host,
  IN    UINT8    socket,
  IN    UINT8    ch,
  OUT   UINT8    *eridCombinedResult
  );

/**

  This function sets the status bits in the ERID LFSR error register

  @param[in]  Host                 - Pointer to sysHost
  @param[in]  socket               - Socket
  @param[in]  ch                   - Channel
  @param[in]  Value                - Value to set the status bits

**/
VOID
SetEridLfsrErrStatus (
  IN    PSYSHOST Host,
  IN    UINT8    socket,
  IN    UINT8    ch,
  IN    UINT32   Value
  );

/**

  Setup the course ERID training

  @param[in]  Host        - Pointer to sysHost
  @param[in]  socket      - Socket Id
  @param[in]  ch          - ddr channel to setup
  @param[in]  dimm        - DIMM id

**/
VOID
SetupCoarseEridTraining (
  IN    PSYSHOST Host,
  IN    UINT8    socket,
  IN    UINT8    ch,
  IN    UINT8    dimm
  );

/**

  Disable ERID LFSR mode

  @param[in]  Host        - Pointer to sysHost
  @param[in]  socket      - Socket Id
  @param[in]  ch          - ddr channel to setup

**/
VOID
DisableEridLfsrMode (
  IN    PSYSHOST Host,
  IN    UINT8    socket,
  IN    UINT8    ch
  );

/**

  Setup CPGC WDB Buffer for ERID

  @param[in]  Host        - Pointer to sysHost
  @param[in]  socket      - Socket Id
  @param[in]  ch          - ddr channel to setup

**/
VOID
SetupCpgcWdbBuffErid (
  IN    PSYSHOST Host,
  IN    UINT8    socket,
  IN    UINT8    ch
  );

/**

  Execute chip specific ODT MPR DIMM activation step

  @param[in]  Host                         - Pointer to sysHost
  @param[in]  socket                       - Socket Id
  @param[in]  ch                           - ddr channel to setup
  @param[in]  dimm                         - DIMM id
  @param[out] executeCtlCLKTestChipStruct  - Pointer to store the misc ODT information

**/
VOID
PerformChipODTMPRDimmActivationStep (
  IN    PSYSHOST Host,
  IN    UINT8 socket,
  IN    UINT8 ch,
  IN    UINT8 dimm,
  OUT   PEXECUTE_CTL_CLK_TEST_CHIP_STRUCT executeCtlCLKTestChipStruct
  );

/**

  Disable MPR training

  @param[in]  Host                         - Pointer to sysHost
  @param[in]  socket                       - Socket Id
  @param[in]  ch                           - ddr channel to setup
  @param[out] executeCtlCLKTestChipStruct  - Pointer to store the misc ODT information

**/
VOID
DisableChipChlRecEnOffsetTrainingMode (
  IN    PSYSHOST                          Host,
  IN    UINT8                             socket,
  IN    UINT8                             ch,
  OUT   PEXECUTE_CTL_CLK_TEST_CHIP_STRUCT executeCtlCLKTestChipStruct
  );

/**

  Setup the override for ODT activation

  @param[in]  Host                         - Pointer to sysHost
  @param[in]  socket                       - Socket Id
  @param[in]  ch                           - ddr channel to setup
  @param[in]  wrOdtTableData               - Bits set used to determine if ODT should be activated for each rank

**/
VOID
AssertChipODTOveride (
  IN    PSYSHOST Host,
  IN    UINT8    socket,
  IN    UINT8    ch,
  IN    UINT32   wrOdtTableData
  );

/**

  Disable the override for ODT activation

  @param[in]  Host                         - Pointer to sysHost
  @param[in]  socket                       - Socket Id
  @param[in]  ch                           - ddr channel to setup

**/
VOID
DeAssertChipODTOveride (
  IN    PSYSHOST Host,
  IN    UINT8    socket,
  IN    UINT8    ch
  );

/**

  Execute final chip specific write leveling cleanup.

  @param[in]  Host                         - Pointer to sysHost
  @param[in]  socket                       - Socket Id

**/
VOID
DoFinalChipSpecificWLCleanUp (
  PSYSHOST Host,
  UINT8    socket
  );

/**

  Set ERID return value to be ignored.

  @param[in]  Host                         - Pointer to sysHost
  @param[in]  socket                       - Socket Id

**/
VOID
DisableEridReturn (
  PSYSHOST Host,
  UINT8    socket
  );

//
// Structures used with SetupAddress
// ScaleRateSettings parameter.
//

typedef union {
  struct {
    UINT32  Bank            :   1;
    UINT32  Column          :   1;
    UINT32  Rank            :   1;
    UINT32  Row             :   1;
    UINT32  Reserved        :   28;
  } Bits;
  UINT32  Data;
} SCALE_LIST;

typedef union {
  struct {
    UINT32  Bank            :   8;
    UINT32  Column          :   8;
    UINT32  Rank            :   8;
    UINT32  Row             :   8;
  } Bits;
  UINT32  Data;
} RATE_LIST;

typedef union {
  struct {
    UINT32  Bank            :   16;
    UINT32  Rank            :   16;
  } Bits;
  UINT32  Data;
} INC_LIST;

typedef union {
  struct {
    UINT32  Column          :   16;
    UINT32  ColumnMsb       :   16;
  } Bits;
  UINT32  Data;
} INC_LIST2;

typedef union {
  struct {
    UINT32  Row             :   16;
    UINT32  Reserved        :   16;
  } Bits;
  UINT32  Data;
} INC_LIST3;

typedef union {
  struct {
    UINT32  Bank            :   8;
    UINT32  Column          :   8;
    UINT32  Rank            :   8;
    UINT32  Row             :   8;
  } Bits;
  UINT32  Data;
} ORDER_LIST;

typedef struct _SCALE_RATE_SETTINGS {
  SCALE_LIST    ScaleList;
  RATE_LIST     RateList;
  INC_LIST      IncList;
  INC_LIST2     IncList2;
  INC_LIST3     IncList3;
  ORDER_LIST    OrderList;
} SCALE_RATE_SETTINGS;

//
// Structures used with SetupAddress AddressSettings
// parameter.
//

typedef union {
  struct {
    UINT32  BankAddr        :   8;
    UINT32  ColumnAddr      :   24;
  } Bits;
  UINT32  Data;
} START_LIST;

typedef union {
  struct {
    UINT32  RowAddr         :   24;
    UINT32  RankAddr        :   8;
  } Bits;
  UINT32  Data;
} START_LIST2;

typedef union {
  struct {
    UINT32  BankAddr        :   8;
    UINT32  ColumnAddr      :   24;
  } Bits;
  UINT32  Data;
} WRAP_LIST;

typedef union {
  struct {
    UINT32  RowAddr         :   24;
    UINT32  RankAddr        :   8;
  } Bits;
  UINT32  Data;
} WRAP_LIST2;

typedef struct _ADDRESS_SETTINGS {
  START_LIST    StartList;
  START_LIST2   StartList2;
  WRAP_LIST     WrapList;
  WRAP_LIST2    WrapList2;
} ADDRESS_SETTINGS;

/**

  Configure the CPGC Start/Wrap addresses

  @param[in]  Socket              - Current Socket (0-Based)
  @param[in]  Channel             - Current Channel (0-Based)
  @param[in]  AddressSettings     - Pointer to address settings
  @param[in]  ScaleRateSettings   - Pointer to scale, rate and order settings

  @retval     EFI_STATUS
**/
EFI_STATUS
EFIAPI
SetupAddress (
  IN        UINT8               Socket,
  IN        UINT8               Channel,
  IN        ADDRESS_SETTINGS    *AddressSettings,
  IN        SCALE_RATE_SETTINGS *ScaleRateSettings
  );

/**
  Setups the CADB Write Pointer

  @param[in]   Socket             - Current Socket (0-Based)
  @param[in]   Channel            - Current Channel (0-Based)
  @param[in]   Value              - Value to setup the CADB write pointer

  @retval      EFI_STATUS

**/
EFI_STATUS
EFIAPI
SetupCadbWritePointer (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT32          Value
  );

//
// Structure used with AppendCadbLine CadbPattern
// parameter.
//

typedef union {
  struct {
    UINT32 ColAddrR         :   8;
    UINT32 ColAddrF         :   8;
    UINT32 RowAddrR         :   6;
    UINT32 RowAddrF         :   6;
    UINT32 Cke              :   1;
    UINT32 Reserved         :   3;
  } Bits;
  UINT32 Data;
} CADB_SIGNALS_LIST;

typedef struct _CADB_CMD_PATTERN {
  CADB_SIGNALS_LIST List;
} CADB_CMD_PATTERN;

/**
  Appends a CADB line

  @param[in] Socket             - Current Socket (0-Based)
  @param[in] Channel            - Current Channel (0-Based)
  @param[in] CadbPattern        - Value to setup the CADB write pointer

  @retval    EFI_STATUS

**/
EFI_STATUS
EFIAPI
AppendCadbLine (
  IN        UINT8             Socket,
  IN        UINT8             Channel,
  IN        CADB_CMD_PATTERN  CadbPattern
  );

/**
  Enables or disables CADB mode

  @param[in] Socket             - Current Socket (0-Based)
  @param[in] Channel            - Current Channel (0-Based)
  @param[in] Enable             - Enable(TRUE) / Disable(FALSE) CADB mode

  @retval    EFI_STATUS

**/
EFI_STATUS
EFIAPI
EnableCadb (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        BOOLEAN         Enable
  );

//
// Structure used with GetCpgcErrors CpgcErrorStatus parameter
//

typedef struct _CPGC_ERRORS {
  UINT32    CpgcEvenErrors;
  UINT32    CpgcOddErrors;
  UINT8     CpgcEccErrors;
} CPGC_ERRORS;

/**
  Clear errors from CPGC registers

  @param[in]  Socket             - Current Socket (0-Based)
  @param[in]  Channel            - Current Channel (0-Based)

  @retval     EFI_STATUS

**/
EFI_STATUS
EFIAPI
ClearCpgcErrors (
  IN        UINT8           Socket,
  IN        UINT8           Channel
  );

/**
  Setup the error mask in the CPGC registers

  @param[in]  Socket             - Current Socket (0-Based)
  @param[in]  Channel            - Current Channel (0-Based)
  @param[in]  Strobe             - Current Strobe (0-Based)
  @param[in]  Mask               - Bits to be masked

  @retval     EFI_SUCCESS         This function completed execution with no warnings/errors
  @retval     EFI_UNSUPPORTED     Strobe index is not supported

**/
EFI_STATUS
EFIAPI
SetupErrorMask (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           Strobe,
  IN        UINT32          Mask
  );

/**
  Mask ECC errors

  @param[in]  Socket             - Current Socket (0-Based)
  @param[in]  Channel            - Current Channel (0-Based)
  @param[in]  Enable             - Enable(TRUE) / Disable(FALSE) CADB mode

  @retval     EFI_STATUS

**/
EFI_STATUS
EFIAPI
MaskEccErrors (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        BOOLEAN         EnableMask
  );

//
// Structures used with GetSetCpgcBl4, GetSetCpgcTestStatus,
// and GetCpgcTestDone Global parameter.
//

typedef union {
  struct {
    UINT32  Bl4Support      :   1;
    UINT32  StartTest       :   1;
    UINT32  StopTest        :   1;
    UINT32  ClearErrors     :   1;
    UINT32  TestDone0       :   1;
    UINT32  TestDone1       :   1;
    UINT32  TestDone2       :   1;
    UINT32  TestDone3       :   1;
    UINT32  Reserved        :   24;
  } Bits;
  UINT32  Data;
} SETTINGS_LIST;

typedef struct _GLOBAL_FEATURE {
  SETTINGS_LIST SettingList;
} GLOBAL_FEATURE;

//
// Definitions for use with GetSetCpgcBl4
// GetSet parameter.
//

#define GET_CPGC_LIB        0
#define SET_CPGC_LIB        1

/**
  Get or set Cpgc BL4

  @param[in]      Socket             - Current Socket (0-Based)
  @param[in]      Channel            - Current Channel (0-Based)
  @param[in]      GetSet             - Specify (GET or SET)_CPGC_LIB command
  @param[in,out]  Global             - Pointer to Get/Set value for global features

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
GetSetCpgcBl4 (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           GetSet,
  IN  OUT   GLOBAL_FEATURE  *Global
  );

/**
  Get or set cpgc status (start test and clear errors)

  @param[in]      Socket             - Current Socket (0-Based)
  @param[in]      Channel            - Current Channel (0-Based)
  @param[in]      GetSet             - Specify (GET or SET)_CPGC_LIB command
  @param[in,out]  Global             - Pointer to Get/Set value for global features

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
GetSetCpgcTestStatus (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           GetSet,
  IN  OUT   GLOBAL_FEATURE  *Global
  );

/**
  Stops any cpgc test

  @param[in]  Socket             - Current Socket (0-Based)
  @param[in]  Channel            - Current Channel (0-Based)

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
StopCpgcTests (
  IN        UINT8           Socket,
  IN        UINT8           Channel
  );

/**
  Get all the test done available bits

  @param[in]      Socket             - Current Socket (0-Based)
  @param[in]      Channel            - Current Channel (0-Based)
  @param[in,out]  Global             - Pointer to Get/Set value for global features

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
GetCpgcTestDone (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN  OUT   GLOBAL_FEATURE  *Global
  );

//
// Structures used with SetupSubsequence0 and SetupSubsequence1
// SubsequenceFeatures parameter.
//

typedef union {
  struct {
    UINT32  ResetAddress    :   1;
    UINT32  SaveAddress     :   1;
    UINT32  UpdateRateMode  :   1;
    UINT32  StartSequence   :   8;
    UINT32  EndSequence     :   8;
    UINT32  Reserved        :   13;
  } Bits;
  UINT32  Data;
} ADDRESS_LIST;

typedef union {
  struct {
    UINT32  InvertDataEcc   :   1;
    UINT32  Cachelines      :   16;
    UINT32  Reserved        :   15;
  } Bits;
  UINT32  Data;
} DATA_LIST;

//
// Definitions for use with CONTROL_LIST Type member.
//

#define READ_SUBSEQ_TYPE_CPGC_LIB                 0
#define WRITE_SUBSEQ_TYPE_CPGC_LIB                1

typedef union {
  struct {
    UINT32  Type            :   8;
    UINT32  Wait            :   16;
    UINT32  Reserved        :   8;
  } Bits;
  UINT32  Data;
} CONTROL_LIST;

typedef struct _SUBSEQUENCE_FEATURE {
  ADDRESS_LIST  AddressList;
  DATA_LIST     DataList;
  CONTROL_LIST  ControlList;
} SUBSEQUENCE_FEATURE;

/**
  Setup Subsequence 0

  @param[in]  Socket                - Current Socket (0-Based)
  @param[in]  Channel               - Current Channel (0-Based)
  @param[in]  SubsequenceFeatures   - Pointer to Subsequence feature

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
SetupSubsequence0 (
  IN        UINT8               Socket,
  IN        UINT8               Channel,
  IN        SUBSEQUENCE_FEATURE *SubsequenceFeatures
  );

/**
  Setup Subsequence 1

  @param[in]  Socket                - Current Socket (0-Based)
  @param[in]  Channel               - Current Channel (0-Based)
  @param[in]  SubsequenceFeatures   - Pointer to Subsequence feature

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
SetupSubsequence1 (
  IN        UINT8               Socket,
  IN        UINT8               Channel,
  IN        SUBSEQUENCE_FEATURE *SubsequenceFeatures
  );

//
// Structures used with SetupLoopCount and SetupChannelSequence
// SequenceGlobalFeatures parameter.
//

//
// Definitions for use with TEST_LIST mode parameter, CPGC Init modes
//

#define IDLE_CPGC_LIB       0
#define TESTING_CPGC_LIB    1
#define MRS_CPGC_LIB        2
#define NOP_CPGC_LIB        3

typedef union {
  struct {
    UINT32  GlobalControl   :   1;
    UINT32  Loopcount       :   8;
    UINT32  Mode            :   4;
    UINT32  Reserved        :   19;
  } Bits;
  UINT32  Data;
} TEST_LIST;

typedef struct _SUBSEQUENCE_CHANNEL_FEATURE {
  TEST_LIST     TestList;
  ADDRESS_LIST  AddressList;
} SUBSEQUENCE_CHANNEL_FEATURE;

/**
  Setup Loopcount and Update Rate Mode

  @param[in]  Socket                   - Current Socket (0-Based)
  @param[in]  Channel                  - Current Channel (0-Based)
  @param[in]  SequenceChannelFeatures  - Pointer to Subsequence feature

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
SetupLoopCount (
  IN        UINT8                       Socket,
  IN        UINT8                       Channel,
  IN        SUBSEQUENCE_CHANNEL_FEATURE *SequenceChannelFeatures
  );

/**
  Enable / Disable CPGC in Native Mode

  @param[in]  Socket             - Current Socket (0-Based)
  @param[in]  Channel            - Current Channel (0-Based)
  @param[in]  MemSs              - MC ID
  @param[in]  Enable             - Enable(TRUE) / Disable(FALSE) to operate in native mode

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
EnableCpgcInNativeMode (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           MemSs,
  IN        BOOLEAN         Enable
  );

/**
  Enable / Disable Global Support

  @param[in]  Socket             - Current Socket (0-Based)
  @param[in]  Channel            - Current Channel (0-Based)
  @param[in]  Enable             - Enable(TRUE) / Disable(FALSE) support for global execution

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
EnableGlobalSupport (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        BOOLEAN         Enable
  );


/**
  Setup channel sequence configuration

  @param[in]  Socket                   - Current Socket (0-Based)
  @param[in]  Channel                  - Current Channel (0-Based)
  @param[in]  SequenceGlobalFeatures   - Pointer to Subsequence feature

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
SetupChannelSequence (
  IN        UINT8                       Socket,
  IN        UINT8                       Channel,
  IN        SUBSEQUENCE_CHANNEL_FEATURE *SequenceChannelFeatures
  );

//
// Structures used with SetupPatternWdbMux
// PatternBuffers parameter.
//

typedef union {
  UINT32  Data;
} RW_LIST;

//
// Definitions for use with MUX_LIST MuxSelect member, WDB control
//

#define PATBUF_CPGC_LIB     1
#define LFSR24_CPGC_LIB     2

typedef union {
  struct {
    UINT32  MuxSelect       :   4;
    UINT32  IncRate         :   8;
    UINT32  IncScale        :   1;
    UINT32  StartPointer    :   8;
    UINT32  EndPointer      :   8;
    UINT32  Reserved        :   3;
  } Bits;
  UINT32  Data;
} MUX_LIST;

typedef struct _PATTERN_BUFFERS {
  RW_LIST       ReadList0;
  RW_LIST       ReadList1;
  RW_LIST       ReadList2;
  RW_LIST       WriteList0;
  RW_LIST       WriteList1;
  RW_LIST       WriteList2;
  MUX_LIST      MuxList;
} PATTERN_BUFFERS;

/**
  Configure patterns, pattern type and others

  @param[in]  Socket             - Current Socket (0-Based)
  @param[in]  Channel            - Current Channel (0-Based)
  @param[in]  PatternBuffers     - Pointer to Pattern Buffer

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
SetupPatternWdbMux (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        PATTERN_BUFFERS *PatternBuffers
  );

//
// Structures used with FastProgramWdb and WdbRdWrPointer
// WdbRdWrPointer parameter.
//

typedef union {
  struct {
    UINT32 Pointer          :   16;
    UINT32 SubPointer       :   16;
  } Bits;
  UINT32 Data;
} POINTER_LIST;

typedef struct _RDWR_POINTER {
  POINTER_LIST  List;
} RDWR_POINTER;

/**

  Configures the WDB Rd/Wr pointer and Write pointers in fast Mode

  @param[in]  Channel            - Current Channel (0-Based)
  @param[in]  WdbRdWrPointer     - Pointer to pointer and subpointers
  @param[in]  Hi                 - Hi 32 bits
  @param[in]  Lo                 - Low 32 bits

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
FastProgramWdb (
  IN        UINT8           Channel,
  IN        RDWR_POINTER    *WdbRdWrPointer,
  IN        UINT32          Hi,
  IN        UINT32          Lo
  );

/**

  Configures the WDB Rd/Wr pointer

  @param[in]  Socket             - Current Socket (0-Based)
  @param[in]  Channel            - Current Channel (0-Based)
  @param[in]  WdbRdWrPointer     - Pointer to pointer and subpointers

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
WdbRdWrPointer (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        RDWR_POINTER    *WdbRdWrPointer
  );


/**

  Configures the WDB Write buffers

  @param[in]   Socket             - Current Socket (0-Based)
  @param[in]   Channel            - Current Channel (0-Based)
  @param[in]   Lo                 - Low 32 bits
  @param[in]   Hi                 - Hi 32 bits

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
WdbBuffer (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT32          Lo,
  IN        UINT32          Hi
  );

//
// Structures used with SetupRankLogicalMap
// RankLogicalMapping parameter.
//

typedef union {
  struct {
    UINT32 Rank0Map         :   4;
    UINT32 Reserved         :   28;
  } Bits;
  UINT32 Data;
} MAP_LIST;

typedef struct _RANK_LOGICAL_MAPPING {
  MAP_LIST      List;
} RANK_LOGICAL_MAPPING;

/**

  Configures the rank logical mapping

  @param[in]   Socket                - Current Socket (0-Based)
  @param[in]   Channel               - Current Channel (0-Based)
  @param[in]   RankLogicalMapping    - Pointer to Rank logical mapping

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
SetupRankLogicalMap (
  IN        UINT8                   Socket,
  IN        UINT8                   Channel,
  IN        RANK_LOGICAL_MAPPING    *RankLogicalMapping
  );

//
// Definitions for use with ERROR_LIST StopOnErrorCmd member.
//

#define ERROR_STOP_ON_ALL      0    ///< If all data lanes see an error, then test is stopped
#define ERROR_STOP_ON_SINGLE   1    ///< If single error is detected, then test is stopped

//
// Structures used with SetupErrorLogging ErrorLoggingFeature
// parameter.
//

typedef union {
  struct {
    UINT32  StopOnErrorCmd  :   4;
    UINT32  Reserved        :   28;
  } Bits;
  UINT32  Data;
} ERROR_LIST;

typedef struct _ERROR_LOGGING_FEATURE {
  ERROR_LIST    List;
} ERROR_LOGGING_FEATURE;

/**

  Configures the error logging registers

  @param[in]  Socket                 - Current Socket (0-Based)
  @param[in]  Channel                - Current Channel (0-Based)
  @param[in]  ErrorLoggingFeature    - Pointer to Error logging features

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
SetupErrorLogging (
  IN        UINT8                    Socket,
  IN        UINT8                    Channel,
  IN        ERROR_LOGGING_FEATURE    *ErrorLoggingFeature
  );

/**

  Configures Misc Cpgc options

  @param[in]  Socket             - Current Socket (0-Based)
  @param[in]  Channel            - Current Channel (0-Based)
  @param[in]  RefMask            - Refresh Mask
  @param[in]  ZqMask             - Zq Calibration Mask

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
SetupMiscCpgc (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           RefMask,
  IN        UINT8           ZqMask
  );

/**

  Configures HBM Misc Cpgc options

  @param[in]   Socket             - Current Socket (0-Based)
  @param[in]   Channel            - Current Channel (0-Based)

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
SetupHbmMiscCpgc (
  IN        UINT8           Socket,
  IN        UINT8           Channel
  );

/**

  Return whether a given channel has hit a DDRT Read Pending Queue timeout.

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Processor socket within the system (0-based)
  @param[in] Channel   - Number of the channel to check

  @retval FALSE The timeout has not been hit.
  @retval TRUE  The timeout has been hit.

**/
BOOLEAN
EFIAPI
DdrtReadPendingQueueTimedOut (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  );

/**

  Set the DDRT Read Pending Queue (RPQ) credit limit with the default value.

  If implemented with a non-null library, the credit limit is set for all
  channels with a DDRT DIMM.  However, this should only be implemented with a
  non-null library for projects that need to alternately set and clear the
  credit limit for specific training steps.

  @param[in] Host        - Pointer to Host structure
  @param[in] Socket      - Processor socket within the system (0-based)

  @retval N/A

**/
VOID
EFIAPI
SetDefaultCpgcRpqCreditLimitIfNeeded (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  );

/**

  Clear the DDRT Read Pending Queue (RPQ) credit limit.

  If implemented with a non-null library, the credit limit is cleared for all
  channels with a DDRT DIMM.  However, this should only be implemented with a
  non-null library for projects that need to alternately set and clear the
  credit limit for specific training steps.

  @param[in] Host     - Pointer to Host structure
  @param[in] Socket   - Processor socket within the system (0-based)

  @retval N/A

**/
VOID
EFIAPI
ClearCpgcRpqCreditLimitIfNeeded (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  );

/**

  Finds command margins for the given command group

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket number
  @param[in] ChBitmask   - Channel bit mask
  @param[in] SubCh       - Sub channel number.
  @retval N/A

**/
VOID
ExecuteCmdSignalTest (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     SubCh
  );

//
// Definitions used with TestControl
// StartTest parameter.
//

#define START_TEST          1
#define NOT_START_TEST      0

//
// Definitions used with TestControl
// StopTest parameter.
//

#define STOP_TEST           1
#define NOT_STOP_TEST       0

/**
This function is used to start a test, stop a test, and clear errors.
This function does not assume that all tests are CPGC tests (i.e. WL
fine test, which is executed in the IO only). This function will use
the last Level and TestType parameters given by SetupTest for the channels.
  @param[in] Host:           Pointer to sysHost
  @param[in] socket:         Processor socket within the system (0-based)
  @param[in] MemSs           MemSS number within the socket (0-based)
  @param[in] ChannelMask:    Channel mask within the MemSS
  @param[in] SubCh:          Sub channel number
  @param[in] TestType:       Indicates the test type to be configured
  @param[in] ClearErrors:    Clear the error status
  @param[in] StartTest:      Start the test: 1: start, 0: not start.
  @param[in] StopTest:       Stop the test: 1: stop, 0: not stop.

  @retval MRC_STATUS

**/
MRC_STATUS
TestControl (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT32        MemSs,
  IN UINT32        ChannelMask,
  IN UINT8         SubCh,
  IN MRC_TT        TestType,
  IN BOOLEAN       ClearErrors,
  IN BOOLEAN       StartTest,
  IN BOOLEAN       StopTest
  );

/**

  Set minimum good samples for receive enable feedback

  @param Host         - Pointer to sysHost, the system Host (root) structure

  @retval UINT8       - The number of minimum good samples

**/
UINT8
SetMinGoodInit (
   PSYSHOST Host
  );


/**
  Set up the memory training or memory test.

  @param[in] Host:           Pointer to sysHost
  @param[in] Socket:         Processor socket within the system (0-based)
  @param[in] MemSs:          MemSS number within the socket (0-based)
  @param[in] ChannelMask:    Channel mask within the MemSS
  @param[in] RankListSize:   Number of items in RankList array
  @param[in] *RankList:      List of ranks, subranks to operate on
  @param[in] Level:          Provides context for LRDIMM buffer back-side vs. Host-side test
  @param[in] TestType:       Indicates the test type to be configured
  @param[in] TargetSignal:   Required for some early command training pattern test types
  @param[in] SeqCnt:         Optional to override default loopcount for given test type
                        This setting impacts the number of times the sequence repeats.
                        This is a linear value. This does not impact number of cachelines
                        per subsequence.  Value of 0 means "infinite" setting.
  @param[in]  PatternLoop     Pattern Loop

  @retval MRC_STATUS

**/
MRC_STATUS
SetupTest (
  PSYSHOST  Host,               // Pointer to Host structure (outside scope of this spec)
  UINT8     Socket,             // Processor socket within the system (0-based)
  MRC_MST   MemSs,              // MemSS number within the socket (0-based)
  UINT32    ChannelMask,        // Channel mask within the MemSS
  UINT32    RankListSize,       // Number of items in RankList array
  MRC_RT    *RankList,          // List of ranks, subranks to operate on
  MRC_LT    Level,              // Provides context for LRDIMM buffer back-side vs. Host-side test
  MRC_TT    TestType,           // Indicates the test type to be configured
  MRC_CSN   TargetSignal,       // Required for some early command training pattern test types
  UINT32    SeqCnt,              // Optional to override default loopcount for given test type
                                // This setting impacts the number of times the sequence repeats.
                                // This is a linear value. This does not impact number of cachelines
                               //  per subsequence.  Value of 0 means "infinite" setting.

  UINT8     Patternloop
  );

/**

  Executes a memory test for advance trainings

  @param Host          - Pointer to sysHost, the system Host (root) structure
  @param Socket        - Socket Id
  @param DdrChEnMap    - Bit-mask of enabled channels to test:\n 1 = Enabled\n 0 = Skipped
  @param Dimm          - Target DIMM to test
  @param Rank          - Target Rank to test
  @param Group         - Parameter to be margined: \n
                           RxDqsDelay    : Margin Rx DqDqs \n
                           TxDqDelay     : Margin Tx DqDqs \n
                           RxVref         : Margin Rx Vref \n
                           TxVref         : Margin Tx Vref \n
                           RecEnDelay    : Margin Receive Enable \n
                           WrLvlDelay    : Margin Write Level \n
                           CmdGrp0        : Margin CMD group 0 \n
                           CmdGrp1        : Margin CMD group 1 \n
                           CmdGrp2        : Margin CMD group 2 \n
                           CmdAll         : Margin all CMD groups
  @param Mode          - Margin mode to use: \n
                           MODE_XTALK            BIT0: Enable crosstalk (placeholder) \n
                           MODE_VIC_AGG          BIT1: Enable victim/aggressor \n
                           MODE_START_ZERO       BIT2: Start at margin 0 \n
                           MODE_POS_ONLY         BIT3: Margin only the positive side \n
                           MODE_NEG_ONLY         BIT4: Margin only the negative side \n
                           MODE_DATA_MPR         BIT5: Enable MPR Data pattern \n
                           MODE_DATA_FIXED       BIT6: Enable Fixed Data pattern \n
                           MODE_DATA_LFSR        BIT7: Enable Data LFSR \n
                           MODE_ADDR_LFSR        BIT8: Enable Address LFSR \n
                           MODE_CHECK_PARITY     BIT10: Enable parity checking \n
                           MODE_DESELECT         BIT11: Enable deselect patterns \n
                           MODE_VA_DESELECT BIT12: Enable Victim - Aggressor deselect
  @param PatternLength - Number of sequences to test (units of burstLength cache lines)
  @param BurstLength   - Number of cachelines to test for each write/read operation.\n
                           It is the repeat count for the WR/RD subsequence

  @retval None

**/
UINT32
ExecuteTest (
  PSYSHOST  Host,             // Pointer to the system Host (root) structure
  UINT8     Socket,           // Socket Id
  UINT32    DdrChEnMap,       // Bit-mask of enabled channels to test. 1 = Enabled, 0 = Skipped
  UINT8     Dimm,             // Target DIMM to test
  UINT8     Rank,             // Target Rank to test
  MRC_GT    Group,            // Parameter to be margined
                              //   RxDqsDelay    : Margin Rx DqDqs
                              //   TxDqDelay     : Margin Tx DqDqs
                              //   RxVref         : Margin Rx Vref
                              //   TxVref         : Margin Tx Vref
                              //   RecEnDelay    : Margin Receive Enable
                              //   WrLvlDelay    : Margin Write Level
                              //   CmdGrp0        : Margin CMD group 0
                              //   CmdGrp1        : Margin CMD group 1
                              //   CmdGrp2        : Margin CMD group 2
                              //   CmdAll         : Margin all CMD groups
  UINT16    Mode,             // Margin mode to use
                              //   MODE_XTALK            BIT0: Enable crosstalk (placeholder)
                              //   MODE_VIC_AGG          BIT1: Enable victim/aggressor
                              //   MODE_START_ZERO       BIT2: Start at margin 0
                              //   MODE_POS_ONLY         BIT3: Margin only the positive side
                              //   MODE_NEG_ONLY         BIT4: Margin only the negative side
                              //   MODE_DATA_MPR         BIT5: Enable MPR Data pattern
                              //   MODE_DATA_FIXED       BIT6: Enable Fixed Data pattern
                              //   MODE_DATA_LFSR        BIT7: Enable Data LFSR
                              //   MODE_ADDR_LFSR        BIT8: Enable Address LFSR
                              //   MODE_ADDR_CMD0_SETUP  BIT9
                              //   MODE_CHECK_PARITY     BIT10: Enable parity checking
                              //   MODE_DESELECT         BIT11: Enable deselect patterns
                              //   MODE_VA_DESELECT BIT12: Enable Victim - Aggressor deselect
                              // patterns
  UINT32   PatternLength,     // Number of sequences to test (units of burstLength cache lines)
  UINT8    BurstLength        // Number of cachelines to test for each write/read operation.
                              //   It is the repeat count for the WR/RD subsequence
  );

/**

  Modify memory training or memory test set up by the setuptest function.

  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param MemSs           MemSS number within the socket (0-based)
  @param ChannelMask:    Channel mask within the MemSS
  @param RankListSize:   Number of items in RankList array
  @param *RankList:      List of ranks, subranks to operate on
  @param TestType:       Indicates the test type to be configured

@retval MRC_STATUS

**/
MRC_STATUS
ModifyTest (
  PSYSHOST  Host,             // Pointer to Host structure (outside scope of this spec)
  UINT8     Socket,           // Processor socket within the system (0-based)
  UINT32    MemSs,            // MemSS number within the socket (0-based)
  UINT32    ChannelMask,      // Channel mask within the MemSS
  UINT32    RankListSize,     // Number of items in RankList array
  MRC_RT    *RankList,        // List of ranks, subranks to operate on
  MRC_TT    TestType
  );

/**
  This function Writes DQ lane-based pattern to the WDB with/without deselect patterns

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket Id
  @param[in] PatternDQ         - Pattern for each DQ lane
  @param[in] NumCachelines     - Number of cacheline to be written
  @param[in] IncludeDeselects  - Whether includes deselect pattern or not.

  @retval None

**/
VOID
WriteWDBDqLanePattern(
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    PatternDQ,
  IN UINT8     NumCachelines,
  IN UINT8     IncludeDeselects
  );

/**
  This function Writes chunk-based pattern (clock pattern) to the WDB

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket Id
  @param[in] PatternChunk    - Initial pattern for the first chunk
  @param[in] NumCachelines   - Number of cacheline to be written

  @retval None

**/
VOID
WriteWDBChunkPattern(
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    PatternChunk,
  IN UINT8     NumCachelines
  );

/**

  This function Writes multiple chunk-based patterns to the specific channel's WDB

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket Id
  @param[in] Channel             - Channel Id
  @param[in] Pattern             - Pointer to buffer containing chunk-based patterns.
  @param[in] CachelineCount      - Size of the buffer pattern in number of cachelines.  Each cacheline has 8 UINT64 elements.  For example, if CachelineCount is 1 then the Pattern buffer size is 64 bytes.
  @param[in] StartCachelineIndex - Start offset on the cacheline.  The sum of the CachelineCount and StartCachelineIndex values must be less than or equal to MRC_WDB_LINES.

  @retval None

**/
VOID
WriteWDBMultiChunkPattern(
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN CONST UINT64 *Pattern,
  IN UINT8        CachelineCount,
  IN UINT8        StartCachelineIndex
  );

/*++

  Sets the patter to be used during test

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Mode    - 0 = MemTest; 1 = MemInit
  @param[in] LoopNum - Loop number for MemTest

  @retval N/A

--*/
VOID
SetPatternBuffers(
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT16    Mode,
  IN UINT16    LoopNum
  );

/*++

  Sets the pattern to be used during test

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Socket Id
  @param[in] FlipPattern    - 0 = Normal Pattern; 1 = Flip Pattern

  @retval N/A

--*/
VOID
SetPatternBuffersX16Dqs(
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  BOOLEAN   FlipPattern
  );

/**
  This function Writes same fixed chunk-based patterns to the specific channel's WDB
  Before "NumCachelines" of cachelines, zero patterns are loaded
  After "NumCachelines" of cachelines, "NumSpread" of cachelines of PatternChunk patterns are loaded

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket Id
  @param[in] Ch              - Channel Id
  @param[in] PatternChunk    - Pattern for chunks
  @param[in] NumSpread       - Number of cacheline for PatternChunk pattern
  @param[in] NumCachelines   - Number of cacheline for zero pattern

  @retval None

**/
VOID
WriteWDBFixedPattern(
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT32    PatternChunk,
  IN UINT8     NumSpread,
  IN UINT8     NumCachelines
  );

/**
  WDB setup for Advanced Memory test

  @param[in] Host                         - Pointer to sysHost
  @param[in] Socket                       - Memory controller to train
  @param[in] Ch                           - Ddr channel to setup
  @param[in] Mode                         - Type of sequence MT_CPGC_WRITE, MT_CPGC_READ, or MT_CPGC_READ_WRITE
  @param[in] SeqDataInv[MT_MAX_SUBSEQ]    - Enables pattern inversion per subsequence
  @param[in] PatternQW[MT_PATTERN_DEPTH]  - 64-bit wide data pattern to use per UI
  @param[in] IsUseInvtPat                 - Info to indicate whether or not patternQW is inverted by comparing original pattern
  @param[in] UiShl[MT_PATTERN_DEPTH]      - Enables pattern rotation per UI
  @param[in] NumCacheLines                - Number of cachelines to use in WDB
  @param[in] PatternDepth                 - Number of UI to use in patternQW
  @param[in] AddrOffset                   - Number of cachelines to advance pattern in given direction
  @param[in] Direction                    - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN
  @param[in] ErrInjMask                   - Bitmask of DQ lanes to inject error

  @retval None

**/
VOID
WriteWDBFixedPatternMATS(
  IN PSYSHOST          Host,
  IN UINT8             Socket,
  IN UINT8             Ch,
  IN UINT8             Mode,
  IN UINT8             SeqDataInv[MT_MAX_SUBSEQ],
  IN UINT64_STRUCT     PatternQW[MT_PATTERN_DEPTH],
  IN BOOLEAN           IsUseInvtPat,
  IN UINT8             UiShl[MT_PATTERN_DEPTH],
  IN UINT8             NumCacheLines,
  IN UINT8             PatternDepth,
  IN UINT8             AddrOffset,
  IN UINT8             Direction,
  IN UINT64_STRUCT     ErrInjMask
  );

/**
  Clear indicators for all channels signifying that cached values in the WDB match the programmed values.

  @param[in] Socket   Target socket

  @retval N/A
**/
VOID
EFIAPI
ClearWdbCacheValidAllChannels (
  IN  UINT8 Socket
  );

/**

  Disable the DDRT Read Pending Queue (RPQ) credit limit for all channels with a DDRT DIMM.

  @param[in] Host             - Pointer to Host structure
  @param[in] Socket           - Processor socket within the system (0-based)

  @retval N/A

**/
VOID
EFIAPI
DisableCpgcRpqCreditLimit (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  );

/**
  Set the CPGC lock state.

  Refer to the enum CPGC_LOCK_STATE for available options. Note that this is not available on all steppings of all
  processors.

  @param[in]  Socket            The target socket
  @param[in]  CpgcLockState     The desired lock state

  @retval   EFI_STATUS  Whether or not setting the lock state was successful.
**/
EFI_STATUS
EFIAPI
SetCpgcLock (
  IN  UINT8           Socket,
  IN  CPGC_LOCK_STATE CpgcLockState
  );


/**
Function used to enable/disable timer for active channels with the credit limit set required by BiosSetAepTimerConfig.

@param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
@param[in]      Socket           - Zero based CPU socket number.
@param[in]      Controller       - Zero based controller number.
@param[in]      ChannelMask      - Channel mask.  Each bit represents a channel to be checked.  Bit value 1 = perform check; bit value 0 = do not perform check.  Bit position 0 = channel 0; bit position 1 = channel 1, etc.
@param[in]      Enable           - TRUE: enable the timer for active channels with credit limit set;
                                   FALSE: disable the timer and clear DDRT scheduler error bits

@retval None.
**/
VOID
BiosSetAepTimerConfigChip (
  IN PSYSHOST                  Host,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     ChannelMask,
  IN BOOLEAN                   Enable
  );

/**
Function used to get the DDRT scheduler time out error status for active channels with the credit limit set required by BiosGetAepTimerErrorStatus.

@param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
@param[in]      Socket           - Zero based CPU socket number.
@param[in]      Controller       - Zero based controller number.
@param[in]      ChannelMask      - Channel mask.  Each bit represents a channel to be checked.  Bit value 1 = perform check; bit value 0 = do not perform check.  Bit position 0 = channel 0; bit position 1 = channel 1, etc.
@param[out]     Status           - Pointer to where the scheduler timer error will be stored. Bit value 1 indicate there is an error
                                   The status is in channel bit mask format. Bit nth corresponds to channel nth inside the Controller

@retval None.
**/
VOID
BiosGetAepTimerErrorStatusChip (
  IN PSYSHOST                  Host,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     ChannelMask,
  OUT UINT8 *                  Status
  );

/**

  This function sets CPGC register used to enable/disable refresh per rank

  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param RefMaskValue:   Bit-field per physical chip-select rank. 0 = refresh commands masked; 0xFF = refresh commands not masked

  @retval n/a

**/
VOID
SetRefresh (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     RefMaskValue
);

/**

  Get Write Leveling Fly by Loop Count.

  @param[out]  LoopCount     Write Leveling Fly by Loop Count.

  @retval      EFI_SUCCESS   This function completed execution with no warnings/errors.
**/
EFI_STATUS
EFIAPI
GetWlFlyByLoopCount (
  OUT  UINT32  *LoopCount
  );

/**
  Early config on Cpgc CMI Credit to enable CPGC for memory training

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If CPGC doesn't require early config of CMI credit
          EFI_SUCCESS            If CPGC requires early config of CMI credit
**/
EFI_STATUS
EFIAPI
EarlyConfigCpgcCmi (
  IN UINT8 Socket,
  IN UINT8 Ch
  );

/**
  Polling whether or not CPGC credit ISM is in the ISM_IDLE state

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If CPGC doesn't support this
          EFI_TIMEOUT            Polling timeout
          EFI_SUCCESS            Otherwise
**/
EFI_STATUS
EFIAPI
PollCpgcIsmIdle (
  IN UINT8 Socket,
  IN UINT8 Ch
  );


/**
  Polling whether or not CPGC credit negotiation process is complete.

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If CPGC doesn't support this
          EFI_TIMEOUT            Polling timeout
          EFI_SUCCESS            Otherwise
**/
EFI_STATUS
EFIAPI
PollCpgcCreditsInitialized (
  IN UINT8 Socket,
  IN UINT8 Ch
  );

/**
  Initialize CPGC Credit Config Done bit to force re-negotiation for credits

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If CPGC doesn't support this
          EFI_SUCCESS            Otherwise
**/
EFI_STATUS
EFIAPI
InitCpgcCreditConfigDone (
  IN UINT8 Socket,
  IN UINT8 Ch
  );

/**
  Set Credit Config Done bit once the desired number of credits have been placed in the CREDIT_CFG register

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If CPGC doesn't support this
          EFI_SUCCESS            Otherwise
**/
EFI_STATUS
EFIAPI
SetCpgcCreditConfigDone (
  IN UINT8 Socket,
  IN UINT8 Ch
  );

/**

  Specify CPGC error counter and error type for error counting

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket number
  @param[in] Ch                  - Channel number
  @param[in] SubCh               - Subchannel number
  @param[in] CounterIndex        - Error counter index: 0~8
  @param[in] ErrorType           - Error type
  @param[in] CounterPointer      - Pointer used to indicate which lane, bytegroup, nibble or UI the counter will count errors on

  @retval SUCCESS                - Setup error counter successfully
          FAILURE                - Failed to Setup error counter

**/
UINT32
CpgcErrorCounterSelect (
  IN PSYSHOST               Host,
  IN UINT8                  Socket,
  IN UINT8                  Ch,
  IN UINT8                  SubCh,
  IN UINT8                  CounterIndex,
  IN CPGC_ERROR_COUNT_TYPE  ErrorType,
  IN UINT8                  CounterPointer
  );

/**

  Read specific CPGC error counter

  @param[in]  Host                - Pointer to sysHost
  @param[in]  Socket              - Socket number
  @param[in]  Ch                  - Channel number
  @param[in]  SubCh               - Subchannel number
  @param[in]  CounterIndex        - Error counter index: 0~8
  @param[out] *CounterNumber      - Pointer to counter value
  @param[out] *CounterOverFlow    - Pointer to indicate whether or not the counter is overflow

  @retval SUCCESS                 - Read error counter successfully
          FAILURE                 - Failed to Read error counter

**/
UINT32
CpgcReadErrorCounter (
  IN  PSYSHOST               Host,
  IN  UINT8                  Socket,
  IN  UINT8                  Ch,
  IN  UINT8                  SubCh,
  IN  UINT8                  CounterIndex,
  OUT UINT32                *CounterNumber,
  OUT BOOLEAN               *CounterOverFlow
  );

/**

  Routine Description:

    Entry point for DRAM Maintenance Test (DMT)

  Arguments:

    @param[in] Host          - Pointer to sysHost
    @param[in] socket        - Socket on which we are going to run the DRAM Maintenance test

  Returns:

    @retval SUCCESS/FAILURE

**/

INT32
EFIAPI
DramMaintenanceTool (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  );

/**

  Get CADB Test Done Status

  @param[in]  Host          Pointer to sysHost
  @param[in]  Socket        Socket number
  @param[in]  Ch            Channel number
  @param[in]  SubCh         Sub Channel number

  @retval TRUE:  Done
          FALSE: Not Done

**/
BOOLEAN
EFIAPI
GetCadbTestDone (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubCh
  );

/**

  CADB Poll function

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket number
  @param[in] DdrChEnMap - Bit mask of channels to use
  @param[in] SubChEnMap - Bit mask of sub-channels to use

  @retval SUCCESS / FAILURE

**/
UINT32
CadbPollTestDone (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT32    DdrChEnMap,
  IN  UINT8     SubChEnMap
);


//
// CPGC configuration header
//
typedef struct {
  UINT8 MajorVersion;
  UINT8 MinorVersion;
} CPGC_CONFIGURE_HEADER;


/**
  Set up the CPGC engine for BSSA test.

  @param Host:                     Pointer to sysHost
  @param socket:                   Processor socket within the system (0-based)
  @param MemSs                     MemSS type within the socket
  @param ChannelMask:              Channel mask within the MemSS
  @param RankListSize:             Number of items in RankList array
  @param *RankList:                List of ranks, subranks to operate on
  @param CpgcConfig:               Pointer to a CPGC configuration structure.
  @param UpdatePatternAndChecker:  Flag to update the CPGC pattern and error checker settings.
  @param UpdateSequenceAndAddress: Flag to update the CPGC sequencers and address generation settings.

  @retval MRC_STATUS_SUCCESS, MRC_STATUS_FAILURE, MRC_FUNCTION_NOT_IMPLEMENTED

  **/
MRC_STATUS
EFIAPI
SetupBssaCpgcTest (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN MRC_MST   MemSs,
  IN UINT32    ChannelMask,
  IN UINT32    RankListSize,
  IN MRC_RT    *RankList,
  IN CPGC_CONFIGURE_HEADER *CpgcConfig,
  IN BOOLEAN   UpdatePatternAndChecker,
  IN BOOLEAN   UpdateSequenceAndAddress
  );

/**
  Program CPGC loopcount.

  @param[in] Host:           Pointer to sysHost
  @param[in] Socket:         Processor socket within the system (0-based)
  @param[in] MemSs           MemSS type within the socket
  @param[in] ChannelMask:    Channel mask within the MemSS
  @param[in] Loopcount:      CPGC loopcount.

  @retval MRC_STATUS_SUCCESS, MRC_STATUS_FAILURE, MRC_FUNCTION_NOT_IMPLEMENTED

**/
MRC_STATUS
EFIAPI
SetupTestLoopCount (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN MRC_MST   MemSs,
  IN UINT32    ChannelMask,
  IN UINT32    LoopCount
  );

/**

  This function is used to get the lane/bit test status for BSSA test.

  If there are multiple SubChannels inside a Channel, then the LaneErroStatus
  of SubChannels are concatenated.

  For DDR5:
  SUB_CH0 32bits lane error status are stored to the LaneErrorStatus[3:0]
  SUB_CH0 8bits ECC lane error status are stored to the LaneErorStatus[4]
  SUB_CH1 32bits lane error status are stored to the LaneErrorStatus[8:5]
  SUB_CH1 8bits ECC lane error status are storeed to the LaneErrorStatus[9]

  @param[in] Host:            Pointer to sysHost
  @param[in] Socket:          Processor socket within the system (0-based)
  @param[in] MemSs            MemSS type within the socket
  @param[in] Channel          Channel number within socket (0-based)
  @param[in] TestType         Indicates the test type
  @param[in] ChunkErrSel      Select the chunck/UIs of the cache line to be checked
  @param[in] Size             The size of the Status array
  @param[out] LaneErrorStatus Pointer to array where the lane error status values will be stored.
                              The number of array elements is (MAX_BITS / 8).
                              Bit value 1 = at least one error occurred; bit value 0 = no errors occurred.

  @retval MRC_STATUS_SUCCESS, MRC_STATUS_FAILURE, MRC_FUNCTION_NOT_IMPLEMENTED
**/
MRC_STATUS
EFIAPI
GetCpgcLaneErrorStatus (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      MRC_MST   MemSs,
  IN      UINT8     Channel,
  IN      MRC_TT    TestType,
  IN      MRC_CT    ChunkErrSel,
  IN      UINT8     Size,
  OUT     UINT8     *LaneErrorStatus
  );

/**

  This function is used to get the CPGC error counter values for BSSA test.

  If there are multiple SubChannels inside a Channel, then the error count
  of SubChannels are added together.

  @param[in] Host:            Pointer to sysHost
  @param[in] Socket:          Processor socket within the system (0-based)
  @param[in] MemSs            MemSS type within the socket
  @param[in] Channel          Channel number within socket (0-based)
  @param[in] TestType         Indicates the test type
  @param[in] Counter,         Counter ID number
  @param[in,out] CountStatus  Pointer to the erorr count status

  @retval MRC_STATUS_SUCCESS, MRC_STATUS_FAILURE, MRC_FUNCTION_NOT_IMPLEMENTED

**/
MRC_STATUS
EFIAPI
GetErrorCount (
  IN      PSYSHOST                 Host,
  IN      UINT8                    Socket,
  IN      MRC_MST                  MemSs,
  IN      UINT8                    Channel,
  IN      MRC_TT                   TestType,
  IN      UINT8                    Counter,
  OUT     CPGC_ERROR_COUNT_STATUS *CountStatus
  );

/**

  Check whether or not the hardware can support the current MAST with defined test size, and
  fixup the cacheline number if it is supported.

  @param[in]     TestSize                      - Test size for the current MAST
  @param[in out] NumOfCacheLine                - Pointer to the number of cacheline

  @retval TRUE                                 - The current MAST can be supported by hardware
          FALSE                                - The current MAST can not be supported by hardware

**/
BOOLEAN
EFIAPI
CheckHwSupportedForMATSAndFixupCacheLine (
  IN     UINT8 TestSize,
  IN OUT UINT8 *NumOfCacheLine
  );

/**

  Initialize CPGC controller rank mask registers

  @param[in] Socket  - Socket number

  @retval n/a

**/
VOID
ConfigureCpgcRankMask (
  IN UINT8 Socket
  );

#define EVEN_CHUNKS    0x5555
#define ODD_CHUNKS     0xAAAA
#define ALL_CHUNKS     0xFFFF

/**

  This function is used to configure error check mask for chunks

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Processor socket within the system (0-based)
  @param[in] Ch               - Channel number
  @param[in] SubCh            - Sub-channel number
  @param[in] MaskChunk        - Bit mask for chunks

  @retval N/A

**/
VOID
EFIAPI
SetCpgcErrChkMaskChunks (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubCh,
  IN UINT32   MaskChunk
  );

#ifdef LRDIMM_SUPPORT
/**

  Returns the number of writes the CPGC will do during the Data Buffer Write
  DFE test.

  @param N/A

  @retval NumWrites - The number of writes the CPGC will issue every test.

**/
UINT32
EFIAPI
GetDbDfeNumCpgcWrites (
  VOID
  );
#endif // #ifdef LRDIMM_SUPPORT

#if FixedPcdGetBool(PcdMcrSupport)
/**

This function writes MPC Command by CADB in DNT

@param[in] Socket          - Memory Controller
@param[in] Ch              - Ch to send command to
@param[in] Dimm            - Dimm to send command to
@param[in] Subchannelmask  - sub channel mask
@param[in] PsChMask        - Pseudo channel mask
@param[in] Rank            - CS to send the command to
@param[in] Opcode          - Opcode to use
@param[in] Timingmode      - Timingmode to use. Support MPC_1N, MPC_2N, MPC_4N.
@param[in] PassThroughMode - pass through mode

@retval N/A

**/
VOID
EFIAPI
WriteCadbMcrMpcCmd (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Subchannelmask,
  IN UINT8     PsChMask,
  IN UINT8     Rank,
  IN UINT16    Opcode,
  IN UINT8     Timingmode,
  IN BOOLEAN   PassThroughMode
  );

/**

  This function sends the NOP command by CADB for DDR5/MCR.

  @param[in] Socket          - socket number
  @param[in] Channel         - ch to send command to
  @param[in] Dimm            - Dimm to send command to
  @param[in] Subchannelmask  - sub channel mask
  @param[in] PsChMask        - Pseudo channel Mask
  @param[in] Rank            - CS to send the command to
  @param[in] Cw              - CW to use
  @param[in] Cmd             - which command: NOP_CMD
  @param[in] PassThroughMode - Pass Through Mode
  @param[in] ExitCATM        - Exit CATM
  @param[in] NoOfCsClks      - Number of clock cycles that CS will be asserted
                               Valid values for MCR - 1 and 2
                               Valid values for DDR5 - 1,3,4

  @retval N/A

**/
VOID
EFIAPI
WriteCadbMcrNopCmd (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     Subchannelmask,
  IN UINT8     PsChMask,
  IN UINT8     Rank,
  IN UINT8     Cw,
  IN UINT8     Cmd,
  IN BOOLEAN   PassThroughMode,
  IN BOOLEAN   ExitCATM,
  IN UINT8     NoOfCsClks
  );

/**

This function sends mode register read or write command by CADB for MCR/DDR5

@param[in] Socket          - socket number
@param[in] Ch              - ch to send command to
@param[in] Dimm            - Dimm to send command to
@param[in] Subchannelmask  - sub channel mask
@param[in] PsChMask        - Pseudo channel mask
@param[in] Rank            - CS to send the command to
@param[in] Address         - mode register Address
@param[in] Opcode          - Opcode to use
@param[in] Cw              - CW to use
@param[in] Cmd             - which command: MRR_CMD or MRW_CMD
@retval N/A

**/
VOID
EFIAPI
WriteCadbMcrMrCmd (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Subchannelmask,
  IN UINT8     PsChMask,
  IN UINT8     Rank,
  IN UINT16    Address,
  IN UINT16    Opcode,
  IN UINT8     Cw,
  IN UINT8     Cmd
  );

/**

  This function sends the NOP command by CADB for MCR to establish phase for PS0 and PS1.

  @param[in] Socket          - socket number
  @param[in] Ch              - ch to send command to
  @param[in] Dimm            - Dimm to send command to
  @param[in] Subchannelmask  - sub channel mask
  @param[in] NoOfClks        - No of CLKs to assert CS - valid values 2, 4 or 64
  @retval N/A

  **/
VOID
EFIAPI
McrSetPhaseCmd (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Subchannelmask,
  IN UINT8    NoOfClks
  );
#endif // FixedPcdGetBool(PcdMcrSupport)
#endif  //_MEM_CPGC_IP_LIB_
