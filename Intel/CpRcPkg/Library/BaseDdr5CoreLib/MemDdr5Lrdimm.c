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

#include <Library/MemoryCoreLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CteNetLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SysHostPointerLib.h>
#include <Memory/Ddr5MrRegs.h>
#include <Memory/Ddr5DbRegs.h>
#include <Library/MemoryCoreLib.h>
#include "Ddr5Core.h"
#include <Memory/JedecDefinitions.h>
#include <Library/MemRcLib.h>
#include <Library/SiliconWorkaroundPMemLib.h>

#define  KEEP_FAILURE(_test, _current) (_test == FAILURE ? FAILURE : _current)

#ifdef  LRDIMM_SUPPORT
#define MDQS_PHASE_CYCLE_START 2
#define MDQS_PHASE_CYCLE_END 3
#define MAX_REC_EN_DELAY     64
#define MIN_MDQS_CYCLE_DELAY -4
#define MAX_MDQS_CYCLE_DELAY 4
#define MDQS_CYCLE_TRAINING_STEP_SIZE -1
#define STROBE_MASK_X8 0x1F
#define MR26_SWIZZLE_PATTERN 0xFF
#define MR27_SWIZZLE_PATTERN 0xFF
#define MRE_LOOP_COUNT    1
#define MRE_PHASE_NUM_CL  4
#define MRE_CYCLE_NUM_CL  1
#define MRE_BURST_WAIT    0
#define MRD_COARSE_LOOP_COUNT  8
#define MRD_COARSE_NUM_CL      32
#define MRD_COARSE_BURST_WAIT  4
#define MRD_SWIZZLE_LOOP_COUNT  1
#define MRD_SWIZZLE_NUM_CL      32
#define MRD_SWIZZLE_BURST_WAIT  4
#define MWD_LOOP_COUNT          8
#define MWD_NUM_CL              32
#define MWD_BURST_WAIT          16
#define POSITIVE_ENCODE         15
#define NEGATIVE_ENCODE         31
#define DQ_BIT_MASK             0xF
#define MIN_MDQ_OFFSET          -3
#define MAX_MDQ_OFFSET          3
#define SWIZZLE_BIT_MASK        0x7  // Each entry saved in DQ Mapping structure is 3 bits wide
#define MRE_PHASE_OFFSET        16
#define MRE_COUNTING_WINDOW     8
#define LFSR_ASSIGNMENT         0x55 // LFSR1 on even bits, LFSR0 on odd bits
#define MWD_1S_PATTERN          0xFFFFFFFF // host will drive an all '1' pattern on all DQ bits
#define DWL_WRITE_CYCLE                     32
#define DWL_COUNTING_WINDOW                 32
#define MRD_COUNTING_WINDOW                255
#define MWD_COUNTING_WINDOW                  2
#define SWZ_COUNTING_WINDOW                 31
#define PHASE_SPOT_CHECK_NUM                 3
#define MDQ_MIN_EYE                          6 // Min Eye Pulse Width check for MDQ

#define MDQS_External_WRITE_LEVEL_CYCLE_START -2
#define MDQS_External_WRITE_LEVEL_CYCLE_END    4

#define MDQS_External_WRITE_LEVEL_PHASE_START 0
#define MDQS_External_WRITE_LEVEL_PHASE_END   63

#define MDQS_Internal_WRITE_LEVEL_CYCLE_START 0
#define MDQS_Internal_WRITE_LEVEL_CYCLE_END   15

#define MDQS_Internal_WRITE_LEVEL_PHASE_START 0
#define MDQS_Internal_WRITE_LEVEL_PHASE_END   63

#define MDQS_INTERNAL_WL_25_TCK               16
#define MDQS_INTERNAL_WL_75_TCK               48
#define MDQS_INTERNAL_WL_TCK_PER_CYCLE        64

// Specifies whether to encode or decode the read delay value
typedef enum {
  Encode,
  Decode,
  EncodeDecodeMax
} CODED_READDELAY;

//
// Local Function prototypes
//

/**

  Perform LRDIMM MDQ Receive Enable Phase Training (MREP)

  @param[in] Socket - Socket number

  @retval Status

**/
STATIC
UINT32
EFIAPI
MDQSReceiveEnablePhaseCycleTraining (
  IN UINT8     Socket
  );

/**

  Perform LRDIMM MDQS Read Delay Training.

  @param[in] Socket  - Socket number

  @retval Status

**/
STATIC
MRC_STATUS
CoarseReadDelayTraining (
  IN UINT8  Socket
  );

/**

  Perform LRDIMM DQ backside (Data Buffer to DRAM) Swizzle Discovery.

  @param[in] Socket  - Socket number

  @retval Status

**/
STATIC
MRC_STATUS
BacksideSwizzleDiscovery (
  IN UINT8  Socket
  );

/**

  Perform LRDIMM DQ frontside (Host to Data Buffer) Swizzle Discovery.

  @param[in] Socket  - Socket number

  @retval Status

**/
STATIC
MRC_STATUS
FrontsideSwizzleDiscovery (
  IN UINT8  Socket
  );

/**

  Perform LRDIMM Per-Bit MDQS Read Delay Deskew training.

  @param[in] Socket  - Socket number

  @retval Status

**/
STATIC
MRC_STATUS
PerBitReadDelayTraining (
  IN UINT8  Socket
  );

/**

  Perform LRDIMM Write DQ/DQS Delay training.

  @param[in] Socket  - Socket number

  @retval Status - Non-zero if any failure detected

**/
STATIC
MRC_STATUS
WriteDelayTraining (
  IN UINT8  Socket
  );

/**
 For each data module, a 128 bit vector of zeros and ones holding the tests results is
 evaluated which is a concatenation of the two reads from the feedback CRs.
 BIOS locates the middle of the longest consecutive stream of ones (when the vector is evaluated as cyclic,
 this location in the vector indicates the PI setting and logic delay for that data module).

  @param[in]  Socket        - Current Socket
  @param[in]  Ch            - Channel number
  @param[in]  Subch         - Sub channel number
  @param[in]  Dimm          - Dimm number on Ch
  @param[in]  Rank          - Rank on Dimm
  @param[in]  Strobe        - Strobe number
  @param[in]  RisingEdge    - Pointer to save the rising edge
  @param[in]  CenterPoint   - Pointer to save the center point
  @param[in]  FallingEdge   - Pointer to save the falling edge
  @param[in]  PulseWidth    - Pointer to save the pulse width
  @param[in]  Step          - The PI Step to work on:
                                FPT_PI_READ_TYPE FPT_PI_WRITE_TYPE
                                FPT_PI_LRDIMM_READ_TYPE
                                FPT_PI_LRDIMM_WRITE_TYPE
                                FPT_PI_LRDIMM_RD_MRD_TYPE
                                FPT_PI_LRDIMM_WR_MRD_TYPE
  @param[out] TrainResInput - Training result

  @retval N/A

**/
extern VOID
EvaluatePiSettingResultsDdr5 (
  IN UINT8                       Socket,
  IN UINT8                       Ch,
  IN UINT8                       Subch,
  IN UINT8                       Dimm,
  IN UINT8                       Rank,
  IN UINT8                       Strobe,
  IN UINT16                      RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT16                      CenterPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT16                      FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT16                      PulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT8                       Step,
  OUT struct TrainingResultsDdr5 (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

  Initialize CPGC for LRDIMM MRD Training

  @param[in] Socket    - Socket number
  @param[in] ChBitmask - mask of channels to be used in this CPGC test
  @param[in] Dimm      - DIMM number
  @param[in] Rank      - Rank number
  @param[in] TestType  - Current MRC test

  @retval N/A

**/
VOID
SetupLrdimmReadTestDdr5 (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN MRC_TT    TestType
  );

/**

  Restore Data Buffer Rank Presence Settings:

  Restore the Rank presence settings for data buffers associated with a
  particular DIMM.

  @param[in] Socket - Socket number
  @param[in] Dimm   - DIMM number

  @retval Status

**/
UINT32
EFIAPI
RestoreDimmRankPresence (
  IN UINT8     Socket,
  IN UINT8     Dimm
  );

#ifdef DEBUG_CODE_BLOCK
/**

  This function print the sample array for external coarse

  @param[in] Socket            - Socket number
  @param[in] Channel           - Channel number
  @param[in] Dimm              - DIMM number
  @param[in][out] TrainResult  - Trained result

  @retval N/A

**/
VOID
DisplayDwlExtCoarseResult (
  IN UINT8                         Socket,
  IN UINT8                         Channel,
  IN UINT8                         Dimm,
  IN OUT struct DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );


/**

  This function print the sample array for external fine

  @param[in] Socket            - Socket number
  @param[in] Channel           - Channel number
  @param[in] Dimm              - DIMM number
  @param[in][out] TrainResult  - Trained result

  @retval N/A

**/
VOID
DisplayDwlExtFineResult (
  IN UINT8                         Socket,
  IN UINT8                         Channel,
  IN UINT8                         Dimm,
  IN OUT struct DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

  This function print the sample array

  @param[in] Socket            - Socket number
  @param[in] Channel           - Channel number
  @param[in] Dimm              - DIMM number
  @param[in][out] TrainResult  - Trained result

  @retval N/A

**/
VOID
DisplayDwlIntCoarseResult (
  IN UINT8                         Socket,
  IN UINT8                         Channel,
  IN UINT8                         Dimm,
  IN OUT struct DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

  This function print the sample array

  @param[in] Socket            - Socket number
  @param[in] Channel           - Channel number
  @param[in] Dimm              - DIMM number
  @param[in][out] TrainResult  - Trained result

  @retval N/A

**/
VOID
DisplayDwlIntFineResult (
  IN UINT8                         Socket,
  IN UINT8                         Channel,
  IN UINT8                         Dimm,
  IN OUT struct DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );
#endif

/**

  Perform LRDIMM DRAM Interface DWL External Coarse

  This training step leverages the comparison capability of the buffer MWD
  Training Mode to determine the best Tx strobe (MDQS) cycle alignment for
  the associated CWL setting and DIMM routing.

  @param socket - socket number

  @retval status

**/
STATIC
UINT32
EFIAPI
ExternalWriteLevelingCoarse (
  IN UINT8     Socket
  );

/**

  Perform LRDIMM DRAM Interface WL (DWL):

  This training step leverages the comparison capability of the buffer MWD
  Training Mode to determine the best Tx strobe (MDQS) cycle alignment for
  the associated CWL setting and DIMM routing.

  @param[in] Socket - Socket number

  @retval status

**/
STATIC
UINT32
EFIAPI
ExternalWriteLevelingFine (
  IN UINT8     Socket
  );

/**

  Initial External Write Leveling Training

  @param[in] Socket  - Socket Number
  @param[in] Channel - Channel Number
  @param[in] Dimm    - Dimm Number
  @param[in] Rank    - Rank Number

  @retval N/A

**/
VOID
InitialExternalWriteLevelingTraining (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**

  Initialize the DB MDQS Write Level Phase Delay to 0

  @param[in] Socket  - Socket Number
  @param[in] Channel - Channel Number
  @param[in] Dimm    - Dimm Number
  @param[in] Rank    - Rank Number

  @retval N/A

**/
STATIC
VOID
ConfigureDataBufferWriteLevelPhase (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**

  Get's the results from DWL - external Coarse

  @param[in] Socket           - Socket Number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number of the DIMM
  @param[in] CycleDelay       - Cycle delay
  @param[in][out] StrobePass  - Strobe Pass information
  @param[in][out] TrainResult - Store trained result

  @retval N/A

**/
VOID
GetDwlExtCoarseTrainResults (
  IN UINT8                          Socket,
  IN UINT8                          Dimm,
  IN UINT8                          Rank,
  IN INT8                           CycleDelay,
  IN OUT UINT32                     StrobePass [MAX_CH][SUB_CH][MAX_RANK_DIMM],
  IN OUT struct DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

  Sets up and executes the MRE CPGC test which sends MRR commands to the DRAM.

  @param[in]  Socket    - Socket number
  @param[in]  ChBitmask - Mask of channels on which test should be executed
  @param[in]  Dimm      - Dimm number
  @param[in]  Rank      - Rank number of the DIMM

**/
VOID
ExecuteBacksideExtCoarseTest (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank
  );

/**

  Initialize CPGC for LRDIMM Write Training

  @param[in]    Socket    - socket number
  @param[in]    ChBitmask - mask of channels to be used in this CPGC test
  @param[in]    NumCL     - Number of cacheline
  @param[in]    LoopCount - Number of LoopCount
  @param[in]    Subseqwait - Waiting time in between subsequents

  @retval N/A
**/
VOID
SetupLrdimmWriteTest (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     NumCL,
  IN UINT8     LoopCount,
  IN UINT8     Subseqwait
  );

/**

  Evaluates and prints out the results from the DWL External Coarse and programs
  the cycle result

  @param[in] Socket      - Socket number
  @param[in] ChBitmask   - Channel bit mask
  @param[in] Dimm        - Dimm number
  @param[in] LrTrainRes  - Training result

  @retval N/A

**/
MRC_STATUS
EvaluateAndProgramDwlExtCoarseResults (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN struct    DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

  Clean up Write Leveling Mode

  @param[in] Socket  - Socket Number
  @param[in] Channel - Channel Number
  @param[in] Dimm    - Dimm Number
  @param[in] Rank    - Rank Number

  @retval N/A

**/
VOID
CleanupWriteLevelingMode (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**

  Get's the results from DWL - external fine training

  @param[in] Socket           - Socket Number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number of the DIMM
  @param[in] PhaseDelay       - Phase delay
  @param[in][out] StrobePass  - Strobe Pass information
  @param[in][out] TrainResult - Store trained result

  @retval N/A

**/
VOID
GetDwlExtFineTrainResults (
  IN UINT8                          Socket,
  IN UINT8                          Dimm,
  IN UINT8                          Rank,
  IN INT8                           PhaseDelay,
  IN OUT UINT32                     StrobePass [MAX_CH][SUB_CH][MAX_RANK_DIMM],
  IN OUT struct DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

  Sets up and executes the MRE CPGC test which sends MRR commands to the DRAM.

  @param[in]  Socket    - Socket number
  @param[in]  ChBitmask - Mask of channels on which test should be executed
  @param[in]  Dimm      - Dimm number
  @param[in]  Rank      - Rank number of the DIMM

**/
VOID
ExecuteBacksideExtFineTest (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank
  );

/**

  Perform LRDIMM DB to DRAM Internal Write Leveling.

  Performs the DRAM internal WL cycle adjustment, and places the DB MDQS so that the DRAM interna
  WL pulse rising edge (or DRAM receive enable rising edge) is centered in the MDQS write preamble.
  Internal WL is not applicable for DDRT2 DIMM

  @param[in] Socket - Socket number

  @retval status

**/
UINT32
InternalWriteLeveling (
  IN UINT8     Socket
  );

/**

  Initial External Write Leveling Training

  @param[in] Socket  - Socket Number
  @param[in] Channel - Channel Number
  @param[in] Dimm    - Dimm Number
  @param[in] Rank    - Rank Number

  @retval N/A

**/
STATIC
VOID
EFIAPI
InitialInternalWriteLevelingTraining (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**

  Calculate DB MDQS WL Phase Delay

  @param[in] Socket  - Socket Number
  @param[in] Channel - Channel Number
  @param[in] Dimm    - Dimm Number
  @param[in] Rank    - Rank Number

  @retval N/A

**/
STATIC
VOID
EFIAPI
CalculateAndProgramDbMdqsWlPhaseDelay (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**

  Get's the results from DWL - internal Coarse

  @param[in] Socket           - Socket Number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number of the DIMM
  @param[in] CycleDelay       - Cycle delay
  @param[in][out] StrobePass  - Strobe Pass information
  @param[in][out] TrainResult - Store trained result

  @retval N/A

**/
VOID
GetDwlIntCoarseTrainResults (
  IN UINT8                          Socket,
  IN UINT8                          Dimm,
  IN UINT8                          Rank,
  IN INT8                           CycleDelay,
  IN OUT UINT32                     StrobePass [MAX_CH][SUB_CH][MAX_RANK_DIMM],
  IN OUT struct DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

  Evaluates and prints out the results from the DWL Internal Coarse and programs
  the cycle result

  @param[in] Socket      - Socket number
  @param[in] ChBitmask   - Channel bit mask
  @param[in] Dimm        - Dimm number
  @param[in] LrTrainRes  - Training result

  @retval N/A

**/
VOID
EvaluateAndProgramDwlIntCoarseResults (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN struct    DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

  Initial DB MDQS WL Cycle Delay for DWL Internal Fine training

  @param[in] Socket  - Socket Number
  @param[in] Channel - Channel Number
  @param[in] Dimm    - Dimm Number
  @param[in] Rank    - Rank Number

  @retval N/A

**/
STATIC
VOID
EFIAPI
InitialDbMdqsWlCycleDelay (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**

  Get's the results from DWL - internal Fine

  @param[in] Socket           - Socket Number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number of the DIMM
  @param[in] PhaseDelay       - Phase delay
  @param[in][out] StrobePass  - Strobe Pass information
  @param[in][out] TrainResult - Store trained result

  @retval N/A

**/
VOID
GetDwlIntFineTrainResults (
  IN UINT8                          Socket,
  IN UINT8                          Dimm,
  IN UINT8                          Rank,
  IN INT8                           PhaseDelay,
  IN OUT UINT32                     StrobePass [MAX_CH][SUB_CH][MAX_RANK_DIMM],
  IN OUT struct DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

  Evaluates and prints out the results from the DWL Internal Fine and programs
  the Phase/Cycle result

  @param[in] Socket      - Socket number
  @param[in] ChBitmask   - Channel bit mask
  @param[in] Dimm        - Dimm number
  @param[in] LrTrainRes  - Training result

  @retval N/A

**/
VOID
EvaluateAndProgramDwlIntFineResults (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN struct    DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

  Evaluates and prints out the results from the DWL External Fine and programs
  the phase result

  @param[in] Socket      - Socket number
  @param[in] ChBitmask   - Channel bit mask
  @param[in] Dimm        - Dimm number
  @param[in] LrTrainRes  - Training result

  @retval N/A

**/
MRC_STATUS
EvaluateAndProgramDwlExtFineResults (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN struct    DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

  Decode DB Page[1:0] RWE1

  @param[in] Rwe1setting  - RWE1 current setting

  @retval Actual write leveling timing latency adder

**/
INT8
DecodePg01Rwe1 (
  IN UINT8 Rwe1setting
  );

/**

  Sets up and executes the MRE CPGC test which sends MRR commands to the DRAM.

  @param[in]  Socket    - Socket number
  @param[in]  ChBitmask - Mask of channels on which test should be executed
  @param[in]  Dimm      - Dimm number
  @param[in]  Rank      - Rank number of the DIMM

**/
VOID
ExecuteBacksideInternalTest (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank
  );

/**

  Phase Spot Pre-Check is to ensure External Fine sweep is able to catch 0->1 transition

  @param[in]  Socket    - Socket number
  @param[in]  ChBitmask - Mask of channels on which test should be executed
  @param[in]  Dimm      - Dimm number
  @param[in]  Rank      - Rank number of the DIMM

**/
VOID
PhaseSpotsPreCheck (
  IN  UINT8                  Socket,
  IN  UINT32                 ChBitmask,
  IN  UINT8                  Dimm,
  IN  UINT8                  Rank
  );


/**

  Change the bitmask of channels to skip DDRT channel

  @param[in] Socket     - Socket to check
  @param[in] Dimm       - DIMM to check
  @param[in] ChBitMask  - Bitmask of channels present for the current rank and Dimm

  @retval ChBitMask - Bitmask of no-ddrt channels present for the current rank and Dimm

**/
UINT32
EFIAPI
SkipDdrtCh (
  IN UINT8     Socket,
  IN UINT8     Dimm,
  IN UINT32    ChBitMask
  )
{
  PSYSHOST     Host;
  UINT8        Ch;
  UINT8        MaxChDdr;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();

  //
  // Skip MRD for DDRT2
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (!((1 << Ch) & ChBitMask)) {
      continue;
    }

    if (IsPmemSiliconWorkaroundEnabled (Host, Socket, Ch, Dimm, "S14012067920")) {
      ChBitMask &= ~(1 << Ch);
    }
  }

  return ChBitMask;
}

/**

  Retrieves the Ddr5LrdimmTrainingData structure pointer from the static pointer table.  If
  it has not yet been initialized, resources are allocated and the pointer is set.

  @param[in,out] Ddr5LrdimmTrainingData  - Pointer to the DDR5_LRDIMM_TRAINING_DATA structure

  @retval MRC_STATUS_SUCCESS  DDR5 Training Data structure pointer retrieved
  @retval MRC_STATUS_FAILURE  Cannot retrieve DDR5 Training Data

**/
MRC_STATUS
EFIAPI
GetDdr5LrdimmTrainingData (
  IN OUT DDR5_LRDIMM_TRAINING_DATA  **Ddr5LrdimmTrainingData
  )
{
  MRC_STATUS                 Status;
  DDR5_LRDIMM_TRAINING_DATA  *InitDataPtr;

  Status = MRC_STATUS_SUCCESS;
  InitDataPtr = NULL;

  *Ddr5LrdimmTrainingData = (DDR5_LRDIMM_TRAINING_DATA *) GetDdr5LrdimmTrainingDataPointer ();

  // If first time calling into this library, initialize LRDIMM Training Data
  if (*Ddr5LrdimmTrainingData == NULL) {
    InitDataPtr = RcAllocatePool (sizeof (DDR5_LRDIMM_TRAINING_DATA));
    if (InitDataPtr == NULL) {
      return MRC_STATUS_FAILURE;
    }

    // Store the pointer to the DDR5 Training Data
    SetDdr5LrdimmTrainingDataPointer (InitDataPtr);

    // If pointer is still NULL, return error
    *Ddr5LrdimmTrainingData = (DDR5_LRDIMM_TRAINING_DATA *) GetDdr5LrdimmTrainingDataPointer ();
    if (*Ddr5LrdimmTrainingData == NULL) {
      return MRC_STATUS_FAILURE;
    }
    // Initialize data
    ZeroMem (*Ddr5LrdimmTrainingData, sizeof (DDR5_LRDIMM_TRAINING_DATA));
  }

  return Status;
}

/**

This Function print the sample array for DDR5 WL coarse sweep.

@param[in] Socket            - Socket number
@param[in] Ch                - Channel number
@param[in] Dimm              - DIMM number
@param[in] ReadMode          - 5 = LRDIMM Write MWD Delay
                           4 = LRDIMM Read DQ DQS
                           3 = LRDIMM Read Recieve Enable Phase
                           2 = LRDIMM Write DWL
                           1 = Read DqDqs
                           0 = Write DqDqs
@param[in] SubChannel        - Sub Channel number
@param[in] Length            -
@param[in] CycleDelayResult  - Cycle Delay Result
@param[in] TrainRes          - Training result
@param[in] CycleDelayValue   - Array containing cycle settings detected during cycle training

@retval N/A

**/
VOID
EFIAPI
PrintMdqsReceiveEnableCycleDdr5 (
  IN UINT8                      Socket,
  IN UINT8                      Ch,
  IN UINT8                      Dimm,
  IN UINT8                      ReadMode,
  IN UINT8                      SubChannel,
  IN UINT16                     Length,
  IN INT8                       CycleDelayResult,
  IN struct TrainingResultsDdr5 (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN INT8                       CycleDelayValue[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8            Strobe;
  UINT8            MaxStrobe;
  INT8             PiSetting;
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  UINT32           StrobePass = 0;
  UINT32           StrobeMask = 0;
  INT8             PiPrint = 0;
  PSYSHOST         Host;
  UINT16           LengthInDwords;
  UINT16           ArrayResultsSize;

  LengthInDwords   = Length / MAX_BITS_IN_DWORD;
  ArrayResultsSize = ARRAY_SIZE ((*TrainRes) [0][0][0].results);
  if ( LengthInDwords > ArrayResultsSize) {
    RcDebugPrint (SDBG_DEFAULT,
                  "Input parameter Length is out of range, LengthInDwords = 0x%0x, ArrayResultsSize = 0x%0x\n",
                  LengthInDwords, ArrayResultsSize);
    return;
  }

  Host = GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  //
  // Set Strobe Bit masks
  //
  StrobeMask = GetSubChStrobeMaskDdr5 (Host);
  if (!(*DimmNvList) [Dimm].x4Present) {
    StrobeMask &= STROBE_MASK_X8;
  }
  if ((ReadMode == 0) && !(*DimmNvList) [Dimm].x4Present) {
    RcDebugPrintLine (SDBG_DEFAULT, 23, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    if (Host->nvram.mem.eccEn) {
      RcDebugPrint (SDBG_DEFAULT, "          0   1   2   3\n");
    } else {
      RcDebugPrint (SDBG_DEFAULT, "          0   1   2   3   4\n");
    }
    MaxStrobe = MAX_STROBE_DDR5 / 4;
  } else {
    RcDebugPrintLine (SDBG_DEFAULT, 48, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    if (Host->nvram.mem.eccEn) {
      RcDebugPrint (SDBG_DEFAULT, "        0   1   2   3   4   5   6   7   8   9\n");
    } else {
      RcDebugPrint (SDBG_DEFAULT, "        0   1   2   3   5   6   7   8\n");
    }
    MaxStrobe = MAX_STROBE_DDR5 / 2;
  }

  for (PiSetting = 0; PiSetting < Length; PiSetting++) {
    PiPrint = CycleDelayResult + PiSetting * MDQS_CYCLE_TRAINING_STEP_SIZE;
    if (PiPrint < MIN_MDQS_CYCLE_DELAY) {
      break;
    }
    if ((ReadMode > 1) && (PiSetting % 2)) {
      continue;
    }
    if (StrobePass == StrobeMask) {
      break;
    }
    if (((ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_RD_DELAY) || (ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_WR_DELAY)) && (PiSetting % 4)) continue;
    if ((ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_RD_DELAY) || (ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_WR_DELAY)) {
      RcDebugPrint (SDBG_DEFAULT, "%d ", (((INT32) PiSetting / 4) - 15));

      if ((((INT32) PiSetting / 4 - 15) >= 0) && (((INT32) PiSetting / 4 - 15) < 10)) {
        RcDebugPrint (SDBG_DEFAULT, "  ");
      }

      if ((((INT32) PiSetting / 4 - 15) < 100) && (((INT32) PiSetting / 4 - 15) > 9)) {
        RcDebugPrint (SDBG_DEFAULT, " ");
      }

      if ((((INT32) PiSetting / 4 - 15) < 0) && (((INT32) PiSetting / 4 - 15) > -10)) {
        RcDebugPrint (SDBG_DEFAULT, " ");
      }

    } else {
      RcDebugPrint (SDBG_DEFAULT, "%d ", PiPrint);

      if ((PiPrint < 10) && (PiPrint >= 0)) {
        RcDebugPrint (SDBG_DEFAULT, "   ");
      }
      if (PiPrint < 0) {
        RcDebugPrint (SDBG_DEFAULT, "  ");
      }
      if ((PiPrint < 100) && (PiPrint > 9)) {
        RcDebugPrint (SDBG_DEFAULT, "   ");
      }
      if (PiPrint > 100) {
        RcDebugPrint (SDBG_DEFAULT, "  ");
      }
    }

    for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
      if (IsDimmStrobeNotValidDdr5 (&(*DimmNvList) [Dimm], Strobe)) {
        continue;
      }

      if (StrobePass & (1 << Strobe)) {
        RcDebugPrint (SDBG_DEFAULT, "   *");
        continue;
      }
      if (((1 << PiSetting) & (*TrainRes) [Ch][SubChannel][Strobe].results[PiSetting / 32]) == 0) {
        RcDebugPrint (SDBG_DEFAULT, "   0");
        StrobePass = StrobePass | (1 << Strobe);
      } else {
        RcDebugPrint (SDBG_DEFAULT, "   1");
      }
    } // Strobe loop

    RcDebugPrint (SDBG_DEFAULT, "\n");
  } // PiSetting loop
  if ((ReadMode == 0) && !(*DimmNvList) [Dimm].x4Present) {
    RcDebugPrintLine (SDBG_DEFAULT, 23, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  } else {
    RcDebugPrintLine (SDBG_DEFAULT, 48, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  }
  RcDebugPrint (SDBG_DEFAULT, "RE:");
  RcDebugPrint (SDBG_DEFAULT, "  ");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", CycleDelayValue[Ch][SubChannel][Strobe]);
  } // strobe loop
  RcDebugPrint (SDBG_DEFAULT, "\n");
} // PrintMdqsReceiveEnableCycleDdr5

/**

  Perform DDR5 LRDIMM Backside Read Training (RX)

  @param Host  - Pointer to sysHost

  @retval Status

**/
UINT32
EFIAPI
Ddr5LrdimmMrePhaseCycleTraining (
  PSYSHOST  Host
  )
{
  UINT8                             Socket;
  UINT32                            Status = SUCCESS;
  struct channelNvram               (*ChannelNvList) [MAX_CH];
  UINT8                             Ch;
  UINT8                             Dimm;

  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);
  //
  // Reasons to just return with SUCCESS...
  //

  if ((!IsMemFlowEnabled (LrdimmBacksideRxTraining))      ||  // Training Step is disabled by SIMICS
      (Host->nvram.mem.socket[Socket].enabled == 0)       ||  // Socket is disabled
      (Host->nvram.mem.socket[Socket].maxDimmPop == 0)    ||  // No DIMM present
      (Host->nvram.mem.socket[Socket].lrDimmPresent == 0)) {  // No LRDIMM present

    return SUCCESS;
  }

  SetDisable2cycBypass (Host, Socket, 0);

  //
  //MDQ Receive Enable Phase Training (MREP)
  //
  Status = KEEP_FAILURE (MDQSReceiveEnablePhaseCycleTraining (Socket), Status);

  SetDisable2cycBypass (Host, Socket, 1);

  // Set Function control nibble back to 0
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Ch = 0; Ch < MAX_CH; Ch++) {
      if ((*ChannelNvList) [Ch].enabled == 0) {
        continue;
      }
      if ((*ChannelNvList) [Ch].dimmList[Dimm].rcLrFunc != 0) {
        WriteBC (Host, Socket, Ch, Dimm, 0, 0, LRDIMM_F0, LRDIMM_BC7x);
        (*ChannelNvList) [Ch].dimmList[Dimm].rcLrFunc = 0;
      }
    } // Ch
  } //Dimm

  SetAepTrainingMode (Host,Socket, DISABLE_TRAINING_STEP);

  return Status;
} //Ddr5LrdimmMrePhaseCycleTraining

/**

  Perform DDR5 LRDIMM Backside Read Delay Training (RX).

  @param[in] Host  - Pointer to sysHost

  @retval Status - Non-zero if any failure detected

**/
UINT32
EFIAPI
Ddr5LrdimmMrdReadDelayTraining (
  IN PSYSHOST  Host
  )
{
  UINT8       Socket;
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;

  Socket = Host->var.mem.currentSocket;

  //
  // Reasons to just return with SUCCESS...
  //
  if ((IsMemFlowEnabled (LrdimmBacksideRxTraining) == FALSE) || // Training Step is disabled
      (Host->nvram.mem.socket[Socket].enabled == 0) ||          // Socket is disabled
      (Host->nvram.mem.socket[Socket].maxDimmPop == 0) ||       // No DIMM present
      (Host->nvram.mem.socket[Socket].lrDimmPresent == 0))      // No LRDIMM present
  {
    return (UINT32) Status;
  }

  // DDRIO initialization for MRD
  SetAepTrainingMode (Host, Socket, CHECKPOINT_MINOR_RX_BACKSIDE_PHASE_TRAINING);
  MrdDdrioInit (Socket);

  //
  // MDQ Read Delay Training (MRD)
  //
  Status = CoarseReadDelayTraining (Socket);
  if (Status != MRC_STATUS_SUCCESS) {
    PostMrdDdrioProgramming (Socket);
    SetAepTrainingMode(Host, Socket, DISABLE_TRAINING_STEP);
    return (UINT32) Status;
  }

  Status = BacksideSwizzleDiscovery (Socket);
  if (Status != MRC_STATUS_SUCCESS) {
    PostMrdDdrioProgramming (Socket);
    SetAepTrainingMode(Host, Socket, DISABLE_TRAINING_STEP);
    return (UINT32) Status;
  }

  Status = FrontsideSwizzleDiscovery (Socket);
  if (Status != MRC_STATUS_SUCCESS) {
    PostMrdDdrioProgramming (Socket);
    SetAepTrainingMode(Host, Socket, DISABLE_TRAINING_STEP);
    return (UINT32) Status;
  }

  Status = PerBitReadDelayTraining (Socket);

  PostMrdDdrioProgramming (Socket);
  SetAepTrainingMode(Host, Socket, DISABLE_TRAINING_STEP);

  return (UINT32) Status;
} // Ddr5LrdimmMrdReadDelayTraining

/**

  Perform DDR5 LRDIMM Backside Write Delay Training (TX).

  @param[in] Host  - Pointer to sysHost

  @retval Status - Non-zero if any failure detected

**/
UINT32
EFIAPI
LrdimmBacksideWriteDelay (
  IN PSYSHOST  Host
  )
{
  UINT8       Socket;
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;

  Socket = Host->var.mem.currentSocket;

  //
  // Reasons to just return with SUCCESS...
  //
  if ((IsMemFlowEnabled (LrdimmBacksideTxTraining) == FALSE) || // Training Step is disabled
      (Host->nvram.mem.socket[Socket].enabled == 0) ||          // Socket is disabled
      (Host->nvram.mem.socket[Socket].maxDimmPop == 0) ||       // No DIMM present
      (Host->nvram.mem.socket[Socket].lrDimmPresent == 0))      // No LRDIMM present
  {
    return (UINT32) Status;
  }

  Status = WriteDelayTraining (Socket);

  return (UINT32) Status;
}

/**

  Initialize CPGC for LRDIMM Write Training


  @param[in] Socket    - Socket number
  @param[in] ChBitmask - mask of channels to be used in this CPGC test
  @param[in] NumCL     - CL number
  @param[in] LoopCount - Loop Count

  @retval N/A

**/
VOID
EFIAPI
SetupLrdimmReadWriteTestDdr5 (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     NumCL,
  IN UINT8     LoopCount
  )
{
  PSYSHOST            Host;

  Host = GetSysHostPointer ();
  //                                Rank, Bank, Row, Col
  TCPGCAddress        CpgcAddress = { { 0, 0, 0, 31 },     // Start
    { 0, 4, 0, 31 },  // Stop
    { 0, 0, 0, 0 },     // Order
    { 0, 0, 0, 0 },     // IncRate
    { 0, 0, 0, 0 } };   // IncValue
  //                            IncRate, Start, Stop, DQPat
  TWDBPattern         WdbPattern = { 0, 0, 1, BasicVA }; // Should not matter what this is. We do not compare data.
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "SetupReadDqDqsTestDdr5 start...\n");
  //
  // CmdPat = PatRd, NumCL = input, LC = input, CPGCAddress, SOE = 0,
  // WDBPattern, EnCADB = 0, EnCKE = 0, SubSeqWait = 0/100
  // NumCl = 32;
  // LoopCount = 3;
  //
  SetupIOTest (Host, Socket, ChBitmask, PatWrRd, 32, 3, &CpgcAddress, NSOE, &WdbPattern, 0, 0, 24);

} //SetupLrdimmReadWriteTestDdr5


/**

  Get the loop count, number of cachelines, and burst wait parameters based on the current
  test type.

  @param[in] Socket      - Socket number
  @param[out] LoopCount  - Pointer to Number of cycles
  @param[out] NumCl      - Pointer to Number of cache lines
  @param[out] BurstWait  - Pointer to Number of DCLK spacing between bursts

  @retval None

**/
VOID
GetLrdimmTestParameters (
  IN UINT8   Socket,
  OUT UINT8  *LoopCount,
  OUT UINT8  *NumCl,
  OUT UINT8  *BurstWait
  )
{
  MRC_TT    CurrentTestType;

  CurrentTestType = GetCurrentTestType (Socket);

  switch (CurrentTestType) {
    case MdqsReceiveEnablePhaseTraining:
      *LoopCount = MRE_LOOP_COUNT;
      *NumCl     = MRE_PHASE_NUM_CL;
      *BurstWait = MRE_BURST_WAIT;
      break;

    case MdqsReceiveEnableCycleTraining:
      *LoopCount = MRE_LOOP_COUNT;
      *NumCl     = MRE_CYCLE_NUM_CL;
      *BurstWait = MRE_BURST_WAIT;
      break;

    case MdqsCoarseReadDelayTraining:
    case MdqsReadDelayDeskew:
      *LoopCount = MRD_COARSE_LOOP_COUNT;
      *NumCl     = MRD_COARSE_NUM_CL;
      *BurstWait = MRD_COARSE_BURST_WAIT;
      break;

    case BacksideDqSwizzleDiscovery:
    case FrontsideDqSwizzleDiscovery:
      *LoopCount = MRD_SWIZZLE_LOOP_COUNT;
      *NumCl     = MRD_SWIZZLE_NUM_CL;
      *BurstWait = MRD_SWIZZLE_BURST_WAIT;
      break;

    case MdqsWriteDelay:
      *LoopCount = MWD_LOOP_COUNT;
      *NumCl     = MWD_NUM_CL;
      *BurstWait = MWD_BURST_WAIT;
      break;

    default:
      *LoopCount = MRE_LOOP_COUNT;
      *NumCl     = MRE_PHASE_NUM_CL;
      *BurstWait = MRE_BURST_WAIT;
      break;
  }
}

/**

  Initialize CPGC for LRDIMM MWD Training

  @param[in] Socket    - Socket number
  @param[in] ChBitmask - mask of channels to be used in this CPGC test
  @param[in] Dimm      - DIMM number
  @param[in] Rank      - Rank number
  @param[in] TestType  - Current MRC test

  @retval N/A

**/
VOID
SetupLrdimmWriteReadTestDdr5 (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN MRC_TT    TestType
  )
{

  UINT8               LoopCnt;
  UINT8               NumCl;
  UINT8               BurstWait;
  PSYSHOST            Host;
  MRC_RT              MrcRankList;
  MRC_MST             MemSsType;
  //                                 Rank, Bank, Row, Col
  TCPGCAddress        CpgcAddress = { {0, 0, 0, 0},      // Start
                                      {0, 0xF, 0, 0x1F}, // Stop
                                      {0, 0, 0, 0},      // Order
                                      {0, 0, 0, 0},      // IncRate
                                      {0, 4, 0, 4} };    // IncValue
  //                            IncRate, Start, Stop, DQPat
  TWDBPattern         WdbPattern = { 0, 0, 1, DqAllSamePat };

  Host = GetSysHostPointer ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  MrcRankList.Dimm = Dimm;
  MrcRankList.Rank = Rank;

  if (Host->DdrioUltSupport) {

    LoopCnt = 2;
    NumCl = 32;

    CteSetupPattern ("rcven", (LoopCnt) | (NumCl << 4), ChBitmask, 1, Rank);  //set the subchannelmask as 1 for ddr4.
    CteSetOptions ("cmd_stretch", ChBitmask, 0x3, 0, 3);
    CteSetOptions ("cmd_stretch", ChBitmask, 0x3, 4, 3);

  } else {
    if (IsBrsPresent (Host, Socket) == TRUE) {
      GetLrdimmTestParameters (Socket, &LoopCnt, &NumCl, &BurstWait);
      SetupIOTest (Host, Socket, ChBitmask, PatWrRd, NumCl, LoopCnt, &CpgcAddress, NSOE, &WdbPattern, 0, 0, BurstWait);
      SetCpgcPatControl (Host, Socket, ChBitmask, BTBUFFER, BTBUFFER, BTBUFFER, MWD_1S_PATTERN, MWD_1S_PATTERN, MWD_1S_PATTERN);
    } else {
      SetupTest (Host, Socket, MemSsType, ChBitmask, 1, &MrcRankList, DdrLevel, TestType, gsmCsnDelim, 0, 0);
    }
  }
} //SetupLrdimmReadTestDdr5

/**

  Sets up and executes the CPGC test which sends MRR commands to the DRAM for MRE.

  @param[in]  Host      - Pointer to syshost structure
  @param[in]  Socket    - Socket number
  @param[in]  ChBitmask - Mask of channels on which test should be executed
  @param[in]  Dimm      - Dimm number
  @param[in]  Rank      - Rank number of the DIMM

**/
VOID
ExecuteBacksideRecEnReadTest (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  MRC_MST  MemSsType;

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  // Setup CPGC test here for now so SendMrw does not override the setup.
  // NOTE: The setup here should be evaluated - it may need to be moved
  //       earlier to prevent unnecessary CSR accesses.
  SetupLrdimmReadTestDdr5 (Socket, ChBitmask, Dimm, Rank, MdqsReceiveEnablePhaseTraining);

  //
  // Clear train_cnt before executing test
  //
  IO_Reset (Host, Socket);

  // Execute the CPGC MPR RD sequence
  TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, MdqsReceiveEnablePhaseTraining, 0, START_TEST, NOT_STOP_TEST);

  EnableDdrioTrainingCounter (Host, Socket, Dimm, Rank);

  if (FeaturePcdGet (PcdCosimBuild) == TRUE) {

    //
    // Delay if this is a CTE build, otherwise do nothing.
    //

    CteDelayDclk (100);
  } else {

    FixedDelayMicroSecond (1);
  }
}

/**

  Sets up and executes the CPGC test which sends MRR commands to the DRAM.

  @param[in]  Host      - Pointer to syshost structure
  @param[in]  Socket    - Socket number
  @param[in]  ChBitmask - Mask of channels on which test should be executed
  @param[in]  Dimm      - Dimm number
  @param[in]  Rank      - Rank number of the DIMM
  @param[in]  TestType  - Current MRC test

  @retval N/A

**/
VOID
ExecuteBacksideReadTest (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN MRC_TT    TestType
  )
{
  MRC_MST  MemSsType;

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  // Setup CPGC test here for now so SendMrw does not override the setup.
  // NOTE: The setup here should be evaluated - it may need to be moved
  //       earlier to prevent unnecessary CSR accesses.
  SetupLrdimmReadTestDdr5 (Socket, ChBitmask, Dimm, Rank, TestType);

  // Execute the CPGC MPR RD sequence
  TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, TestType, 0, START_TEST, NOT_STOP_TEST);

  EnableDdrioTrainingCounter (Host, Socket, Dimm, Rank);

  if (IsBrsPresent (Host, Socket) == TRUE) {
    // IO Reset is necessary for consistent feedback results.
    // NOTE: See HSD 2209987785 - IO_Reset may only need to be invoked for specific silicon
    IO_Reset (Host, Socket);
  }

  if (FeaturePcdGet (PcdCosimBuild) == TRUE) {

    //
    // Delay if this is a CTE build, otherwise do nothing.
    //
    CteDelayDclk (100);
  } else {

    FixedDelayMicroSecond (1);
  }
}

/**

  Sets up and executes the CPGC test to send write and read commands.

  @param[in]  Host      - Pointer to syshost structure
  @param[in]  Socket    - Socket number
  @param[in]  ChBitmask - Mask of channels on which test should be executed
  @param[in]  Dimm      - Dimm number
  @param[in]  Rank      - Rank number of the DIMM
  @param[in]  TestType  - Current MRC test

  @retval N/A

**/
VOID
ExecuteBacksideWriteReadTest (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN MRC_TT    TestType
  )
{
  MRC_MST  MemSsType;

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  // Setup CPGC test here for now so SendMrw does not override the setup.
  // NOTE: The setup here should be evaluated - it may need to be moved
  //       earlier to prevent unnecessary CSR accesses.
  SetupLrdimmWriteReadTestDdr5 (Socket, ChBitmask, Dimm, Rank, TestType);

  // Execute the CPGC MPR RD sequence
  TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, TestType, 0, START_TEST, NOT_STOP_TEST);

  if (FeaturePcdGet (PcdCosimBuild) == TRUE) {

    //
    // Delay if this is a CTE build, otherwise do nothing.
    //
    CteDelayDclk (100);
  } else {
    FixedDelayMicroSecond (1);
  }
}

/**

  Get's the results from the current test

  @param[in]      Socket              - Socket number
  @param[in]      Dimm                - DIMM number
  @param[in]      Rank                - Rank number of the DIMM
  @param[in]      PiDelay             - CLK adjustment setting for this test
  @param[in]      Step                - which training Step is this called from, using same value as training mode for convenience
  @param[in]      CycleDelay          - Cycle Delay
  @param[in][out] LrTrainRes          - Training result

  @retval N/A

**/
VOID
EFIAPI
GetLrdimmPhaseTrainResultsDdr5 (
  IN UINT8                          Socket,
  IN UINT8                          Dimm,
  IN UINT8                          Rank,
  IN UINT16                         PiDelay,
  IN UINT8                          Step,
  IN UINT8                          CycleDelay,
  IN OUT struct TrainingResultsDdr5 (*LrTrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8               Ch;
  UINT8               Strobe;
  UINT8               SubCh = 0;
  UINT8               SwizzleStrobe;
  UINT8               DWord;
  UINT32              LrdimmFeedBack;
  UINT32              ChBitmask;
  UINT32              Databit;
  UINT32              StepSizeBits = (1 << LRDIMM_BACKSIDE_STEP_SIZE) - 1;
  PSYSHOST            Host= GetSysHostPointer ();
  UINT8               SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, Rank);
  DWord = (UINT8) ((PiDelay - LRDIMM_BACKSIDE_PI_START) / 32);

  ExecuteBacksideRecEnReadTest (Host, Socket, ChBitmask, Dimm, Rank);

  for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
    if (!((1 << Ch) & ChBitmask)) {
      continue;
    }
    for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        SwizzleStrobe = CheckBacksideSwizzle (Host, Socket, Ch, Dimm, Strobe);
        LrdimmFeedBack = ReadDataTrainCounterFeedbackDdr5 (Host, Socket, Ch, SubCh, SwizzleStrobe);

        //
        // Add full results to a larger results structure (populate odd ticks same as the even, so I can reuse same results routines
        // as Host receive enable)
        //

        if (LrdimmFeedBack) { // mapping a clock cycle so any Bit of 4 passing will be logged as a 1
          Databit = (StepSizeBits << (PiDelay % 32)); // 3 is used to fill 2 locations in the results array
          (*LrTrainRes) [Ch][SubCh][Strobe].results[DWord] |= Databit;
        }
      } // Strobe loop
    }//Subch loop
  } //Ch loop
} // GetLrdimmPhaseTrainResultsDdr5

/**

  Get's the results from the current test

  @param[in]      Socket              - Socket number
  @param[in]      Dimm                - DIMM number
  @param[in]      Rank                - Rank number of the DIMM
  @param[in]      PiDelay             - CLK adjustment setting for this test
  @param[in]      Step                - which training Step is this called from, using same value as training mode for convenience
  @param[in]      CycleDelay          - Cycle Delay
  @param[in]      NibbleSwap          - Indicates if nibble swap is present for given strobe
  @param[in][out] StrobePass          - Strobe Pass
  @param[out]     CycleDelayPass      - Cycle result
  @param[in][out] LrTrainRes          - Training result

  @retval N/A

**/
VOID
EFIAPI
GetLrdimmCycleTrainResultsDdr5 (
  IN     UINT8                      Socket,
  IN     UINT8                      Dimm,
  IN     UINT8                      Rank,
  IN     UINT16                     PiDelay,
  IN     UINT8                      Step,
  IN     INT8                       CycleDelay,
  IN OUT UINT32                     StrobePass [MAX_CH][SUB_CH],
  OUT    INT8                       CycleDelayPass [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN OUT struct TrainingResultsDdr5 (*LrTrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8               Ch;
  UINT8               Strobe;
  UINT8               SubCh = 0;
  UINT8               SwizzleStrobe;
  UINT8               DWord;
  UINT32              LrdimmFeedBack;
  UINT32              ChBitmask;
  UINT32              Databit = 0;
  UINT32              StepSizeBits = (1 << LRDIMM_BACKSIDE_CYCLE_STEP_SIZE) - 1;
  PSYSHOST            Host = GetSysHostPointer ();
  UINT8               SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, Rank);
  DWord = (UINT8) ((PiDelay - LRDIMM_BACKSIDE_PI_START) / 32);

  ExecuteBacksideRecEnReadTest (Host, Socket, ChBitmask, Dimm, Rank);

  for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
    if (!((1 << Ch) & ChBitmask)) {
      continue;
    }
    for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        if ((StrobePass [Ch][SubCh]) & (1 << Strobe)) {
          continue;
        }
        SwizzleStrobe = CheckBacksideSwizzle (Host, Socket, Ch, Dimm, Strobe);
        LrdimmFeedBack = ReadDataTrainCounterFeedbackDdr5 (Host, Socket, Ch,  SubCh, SwizzleStrobe);

        //
        // Add full results to a larger results structure (populate odd ticks same as the even, so I can reuse same results routines
        // as Host receive enable)
        //

        if (LrdimmFeedBack) { // mapping a clock cycle so any Bit of 4 passing will be logged as a 1
          Databit = (StepSizeBits << (PiDelay % 32)); // 3 is used to fill 2 locations in the results array
          (*LrTrainRes) [Ch][SubCh][Strobe].results[DWord] |= Databit;
        }
        if (LrdimmFeedBack == 0) {
          StrobePass [Ch][SubCh] |= (1 << Strobe);
          // Record cycle delay for this strobe
          CycleDelayPass[Ch][SubCh][Strobe] = CycleDelay;
        }
      } // Strobe loop
    }//Subch loop
  } //Ch loop
} // GetLrdimmCycleTrainResultsDdr5




/**

  Initialize CPGC for LRDIMM MRD Training

  @param[in] Socket    - Socket number
  @param[in] ChBitmask - mask of channels to be used in this CPGC test
  @param[in] Dimm      - DIMM number
  @param[in] Rank      - Rank number
  @param[in] TestType  - Current MRC test

  @retval N/A

**/
VOID
SetupLrdimmReadTestDdr5 (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN MRC_TT    TestType
  )
{

  UINT8               LoopCnt;
  UINT8               NumCl;
  UINT8               BurstWait;
  PSYSHOST            Host;
  MRC_RT              MrcRankList;
  MRC_MST             MemSsType;
  //                                Rank, Bank, Row, Col
  TCPGCAddress        CpgcAddress = { { 0, 0, 0, 31 },    // Start
                                      { 0, 4, 0, 31 },    // Stop
                                      { 0, 0, 0, 0 },     // Order
                                      { 0, 0, 0, 0 },     // IncRate
                                      { 0, 0, 0, 0 } };   // IncValue
  //                            IncRate, Start, Stop, DQPat
  TWDBPattern         WdbPattern = { 0, 0, 1, BasicVA }; // Should not matter what this is. We do not compare data.

  Host = GetSysHostPointer ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  MrcRankList.Dimm = Dimm;
  MrcRankList.Rank = Rank;

  if (Host->DdrioUltSupport) {

    LoopCnt = 2;
    NumCl = 32;

    CteSetupPattern ("rcven", (LoopCnt) | (NumCl << 4), ChBitmask, 1, Rank);  //set the subchannelmask as 1 for ddr4.
    CteSetOptions ("cmd_stretch", ChBitmask, 0x3, 0, 3);
    CteSetOptions ("cmd_stretch", ChBitmask, 0x3, 4, 3);

  } else {
    GetLrdimmTestParameters (Socket, &LoopCnt, &NumCl, &BurstWait);
    if (IsBrsPresent (Host, Socket)) {
      SetupIOTest (Host, Socket, ChBitmask, PatRd, NumCl, LoopCnt, &CpgcAddress, NSOE, &WdbPattern, 0, 0, BurstWait);
    } else {
      SetupTest (Host, Socket, MemSsType, ChBitmask, 1, &MrcRankList, DdrLevel, TestType, gsmCsnDelim, 0, 0);
    }
  }

  return;
} //SetupLrdimmReadTestDdr5

/**

  Cleanup LRDIMM MDQS Receive Enable Training Parameters

  @param[in] Socket     - Socket number
  @param[in] ChBitmask  - Channel bit mask
  @param[in] Dimm       - Dimm number
  @param[in] Rank       - Rank number

  @retval N/A

**/
VOID
LrdimmMdqsRecCleanup (
  IN UINT8    Socket,
  IN UINT32   ChBitmask,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  UINT8                             Ch;
  struct channelNvram               (*ChannelNvList) [MAX_CH];
  struct rankDevice                 (*RankStruct) [MAX_RANK_DIMM];
  struct dimmNvram                  (*DimmNvList) [MAX_DIMM];
  PSYSHOST                          Host;
  DDR5_DATA_BUFFER_RW97_STRUCT      BufferTrainingConfig;

  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
    if (!((1 << Ch) & ChBitmask)) {
      continue;
    }
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);

    SetMprModeFilterActPre (Host, Socket, Ch, 0);
    ConvertRdToMrr (Host, Socket, Ch, 0);

    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    //
    // Select all buffer devices
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);

    // Exit MRE Training Mode
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW83_REG, LRDIMM_EXIT_TRAINING);

    //
    // Exit the read preamble training mode
    //
    if (IsDcpmmPresentDimm(Socket, Ch, Dimm)) {
        SetFMCReceiveEnableModeDdrt2(Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, FALSE);
    } else {
        DramModeRegWrite(Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_2_REG, 0);
    }
    // Clear training feedback
    BufferTrainingConfig.Data = 0;
    BufferTrainingConfig.Bits.clear_feedback_status_bit = 1;
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW97_REG, BufferTrainingConfig.Data);
  } //Ch
}

/**

  Initialize LRDIMM MDQS Receive Enable Training Parameters

  @param[in] Socket     - Socket number
  @param[in] ChBitmask  - Channel bit mask
  @param[in] Dimm       - Dimm number
  @param[in] Rank       - Rank to sysHostr

  @retval N/A

**/
STATIC
VOID
EFIAPI
LrdimmMdqsRecInit (
  IN UINT8    Socket,
  IN UINT32   ChBitmask,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  UINT8                             Ch;
  struct channelNvram               (*ChannelNvList) [MAX_CH];
  struct rankDevice                 (*RankStruct) [MAX_RANK_DIMM];
  struct dimmNvram                  (*DimmNvList) [MAX_DIMM];
  DDR5_MODE_REGISTER_2_STRUCT       ModeRegister2;
  PSYSHOST                          Host;

  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
    if (!((1 << Ch) & ChBitmask)) {
      continue;
    }
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);

    SetMprModeFilterActPre (Host, Socket, Ch, 1);
    ConvertRdToMrr (Host, Socket, Ch, 1);

    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    //
    // 6. Program RW83 to enable MDQS REC training mode.
    //    Write 1 to RW83
    //
    // Select all buffer devices
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);

    // Enter MRE Training Mode
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW83_REG, LRDIMM_RECEN_TRAINING);

    (*RankStruct) [Rank].CurrentLrdimmTrainingMode = LRDIMM_MREP_TRAINING_MODE;
    //
    // Expectation is that the DB Pattern configuration gets programmed automatically through the snooping feature of DRAM MRW. If needed, this can be programmed specifically in the DB (RW99 to RW9D).
    //
    //
    //2.  Set the DRAM with expected Pattern.
    //
    DramModeRegWrite (Socket, Ch, 3, Dimm, Rank, DDR5_MODE_REGISTER_25_REG, 0x0);
    //
    // Set the LFSR0 as the default value 0x5A
    //
    DramModeRegWrite (Socket, Ch, 3, Dimm, Rank, DDR5_MODE_REGISTER_26_REG, MR26_DEFAULT_RD_TRAIN);
    //
    // Set the LFSR1 as the default value 0x3C
    //
    DramModeRegWrite (Socket, Ch, 3, Dimm, Rank, DDR5_MODE_REGISTER_27_REG, MR27_DEFAULT_RD_TRAIN);

    //
    // Enable the read preamble training mode
    //
    if (IsDcpmmPresentDimm(Socket, Ch, Dimm)) {
        SetFMCReceiveEnableModeDdrt2(Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, TRUE);
    } else {
        ModeRegister2.Data = 0;
#ifdef USE_LATEST_DRAM_SPEC
        ModeRegister2.Bits.read_preamble_training = 1;
#else // USE_LATEST_DRAM_SPEC
        ModeRegister2.Bits.read_preamble_enable = 1;
#endif // USE_LATEST_DRAM_SPEC
        DramModeRegWrite(Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_2_REG, ModeRegister2.Data);
    }
    //
    // Select appropriate DB page
    //
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, Rank);

    (*RankStruct) [Rank].CurrentDramMode = MR3_MPR;
    (*RankStruct) [Rank].CurrentMpr0Pattern = 0x55;
  } //Ch
} // LrdimmMdqsRecInit

/**

  If small/no eye was detected, log an error message and halt if necessary.

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket number
  @param[in] Channel     - Channel number
  @param[in] Dimm        - DIMM number
  @param[in] Rank        - Rank number on DIMM
  @param[in] Strobe      - Strobe number
  @param[in] PulseWidth  - The eyesize detected

  @retval NA

**/
VOID
HandleFaultyPart (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Strobe,
  IN UINT16    PulseWidth
  )
{
  UINT8  FaultyPartsStatus;
  //
  // Add this error to the warning log for both correctable and uncorrectable errors.
  //
  FaultyPartsStatus = EvaluateFaultyParts (Host, Socket, Channel);

  switch (FaultyPartsStatus) {
    case FPT_NO_ERROR:
      break;
    case FPT_CORRECTABLE_ERROR:
      // if correctable  log the warning -> OutputWarning
      // careful for cases when warning gets promoted to error !!!
      EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_READ_DELAY_TRAINING, Socket, Channel, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, RxDqsDelay, LrbufLevel, (UINT8) PulseWidth);
      break;
    default:
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
          "LRDIMM training failure!!!\n");
      //
      // Add this error to the warning log for both correctable and uncorrectable errors.
      //
      Host->var.mem.piSettingStopFlag[Channel] |= (1 << Strobe);
      DisableChannelSw (Host, Socket, Channel);
      EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_READ_DELAY_TRAINING, Socket, Channel, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, RxDqsDelay, LrbufLevel, (UINT8) PulseWidth);
      break;
  }
}

/**

  Display final MRE phase and cycle training results.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Current socket number
  @param[in] FinalPhaseValue  - Array containing final MRE Phase training results
  @param[in] FinalCycleValue  - Array containing final MRE Cycle training results

  @retval N/A

**/
VOID
DisplayFinalMreResults (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     FinalPhaseValue[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2],
  IN INT8      FinalCycleValue[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2]
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8   Channel;
  UINT8   Dimm;
  UINT8   Rank;
  UINT8   SubChannel;
  UINT8   Strobe;
  UINT8   MaxStrobes;
  UINT8   MaxChDdr;

  if (!(GetDebugLevel () & SDBG_MAX)) {
    return;
  }

  MaxStrobes = GetSubChMaxStrobeValid (Host);
  MaxChDdr = GetMaxChDdr ();

  AcquirePrintControl ();

  RcDebugPrint (SDBG_MAX, "\nSTART_DATA_DB_RX_RCVEN_PHASE\n");

  RcDebugPrint (SDBG_MAX, "NIBBLE:           ");
  for (Strobe = 0; Strobe < MaxStrobes; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_MAX, "%4d", Strobe);
  }

  RcDebugPrint (SDBG_MAX, "\n");
  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

        for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
          if (IsRankValid (Socket, Channel, Dimm, Rank, FALSE) == FALSE) {
            continue;
          }
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT, "");

          for (Strobe = 0; Strobe < MaxStrobes; Strobe++) {
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }

            RcDebugPrint (SDBG_MAX, "%4d", FinalPhaseValue[Channel][Dimm][SubChannel][Rank][Strobe]);
          } // Strobe loop
          RcDebugPrint (SDBG_MAX, "\n");
        } // Rank loop
      } // SubChannel loop
    } // Dimm loop
  } // Channel loop

  RcDebugPrint (SDBG_MAX, "STOP_DATA_DB_RX_RCVEN_PHASE\n");

  RcDebugPrint (SDBG_MAX, "\nSTART_DATA_DB_RX_RCVEN_CYCLE\n");

  RcDebugPrint (SDBG_MAX, "NIBBLE:           ");
  for (Strobe = 0; Strobe < MaxStrobes; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_MAX, "%4d", Strobe);
  }

  RcDebugPrint (SDBG_MAX, "\n");
  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

        for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
          if (IsRankValid (Socket, Channel, Dimm, Rank, FALSE) == FALSE) {
            continue;
          }
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT, "");

          for (Strobe = 0; Strobe < MaxStrobes; Strobe++) {
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }

            RcDebugPrint (SDBG_MAX, "%4d", FinalCycleValue[Channel][Dimm][SubChannel][Rank][Strobe]);
          } // Strobe loop
          RcDebugPrint (SDBG_MAX, "\n");
        } // Rank loop
      } // SubChannel loop
    } // Dimm loop
  } // Channel loop

  RcDebugPrint (SDBG_MAX, "STOP_DATA_DB_RX_RCVEN_CYCLE\n\n");

  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
}

/**

  Performs nibble swap detection for each strobe pair (lower and upper nibbles).  This is necessary so
  the proper values are programmed to the appropriate data buffer control words (0xE2 and 0xE3).

  @param[in] Host             - Pointer to the SysHost structure
  @param[in] Socket           - Socket number
  @param[in] Dimm             - Dimm number
  @param[in] Rank             - Rank number
  @param[in] ChBitmask        - Channel bit mask
  @param[in] CyclePhaseResult - Array containing cycle settings
  @param[in] RisingEdge       - Array containing the rising edge values detected during phase training
  @param[out] NibbleSwap      - Array storing nibble swap information

  @retval  NA

**/
VOID
NibbleSwapDetection (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT32    ChBitmask,
  IN INT8      CyclePhaseResult[MAX_CH][SUB_CH],
  IN UINT16    RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  OUT BOOLEAN  (*NibbleSwap)[MAX_CH][MAX_DIMM][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8   Channel;
  UINT8   SubChannel;
  UINT8   Strobe;
  UINT8   MaxStrobePairs;
  UINT8   SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8   NibbleConstant = 5;
  UINT16  LoNibbleRisingEdge;
  UINT16  HiNibbleRisingEdge;
  UINT8   RegisterCycleValue;
  UINT8   LoSwizzleStrobe;
  UINT8   HiSwizzleStrobe;
  UINT32  LoFeedBackTest1;
  UINT32  HiFeedBackTest1;
  UINT32  LoFeedBackTest2;
  UINT32  HiFeedBackTest2;
  UINT8   LoCalculatedDelay;
  UINT8   HiCalculatedDelay;
  UINT16  TempValue;
  struct dimmNvram                  (*DimmNvList) [MAX_DIMM];

  RcDebugPrint (SDBG_MAX, "MRE Nibble Swap Detection Start\n");

  MaxStrobePairs = SubChMSVx4 / 2;
  for (Channel = 0; Channel < MAX_CH; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
          continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Channel);

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (Strobe = 0; Strobe < MaxStrobePairs; Strobe++) {
        //
        // DDRT2 use SPD data to update NibbleSwap results
        //
        if (IsDcpmmPresentDimm(Socket, Channel, Dimm) == TRUE) {
          if ((*DimmNvList)[Dimm].lrbufswizzle & (1 << ((Strobe % MaxStrobePairs) + (SubChannel * MaxStrobePairs)))) {
            // Save off Nibble Swap status
            (*NibbleSwap)[Channel][Dimm][SubChannel][Strobe] = TRUE;
            (*NibbleSwap)[Channel][Dimm][SubChannel][Strobe + MaxStrobePairs] = TRUE;
          } else {
            // No Nibble swap
            (*NibbleSwap)[Channel][Dimm][SubChannel][Strobe] = FALSE;
            (*NibbleSwap)[Channel][Dimm][SubChannel][Strobe + MaxStrobePairs] = FALSE;
          }
        } else {
          LoNibbleRisingEdge = RisingEdge[Channel][SubChannel][Strobe] / 2;
          HiNibbleRisingEdge = RisingEdge[Channel][SubChannel][Strobe + MaxStrobePairs] / 2;

          // 1) Program the low and high nibble to a "0" region
          //    - Low Nibble test offset  = Low Nibble rising edge - Constant
          //    - High Nibble test offset = High Nibble rising edge - Constant

          if (LoNibbleRisingEdge < NibbleConstant) {
            LoCalculatedDelay = (UINT8) (LoNibbleRisingEdge + MAX_REC_EN_DELAY - NibbleConstant);
          } else {
            LoCalculatedDelay = (UINT8) (LoNibbleRisingEdge - NibbleConstant);
          }

          if (HiNibbleRisingEdge < NibbleConstant) {
            HiCalculatedDelay = (UINT8) (HiNibbleRisingEdge + MAX_REC_EN_DELAY - NibbleConstant);
          } else {
            HiCalculatedDelay = (UINT8) (HiNibbleRisingEdge - NibbleConstant);
          }

          // 2) For each byte, use PBA to program the DB Receive Enable cycle delay and PI code fine (phase)
          // Select appropriate buffer device
          DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, Strobe);
          // Program Cycle delay
          RegisterCycleValue = ABS (CyclePhaseResult[Channel][SubChannel]);
          if (CyclePhaseResult[Channel][SubChannel] < 0) {
            RegisterCycleValue |= BIT3;
          }
          RegisterCycleValue |= (RegisterCycleValue << 4);
          DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE0_REG, RegisterCycleValue);

          // Program PI code fine
          DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE2_REG, LoCalculatedDelay);
          DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE3_REG, HiCalculatedDelay);

          //
          // 3) Start the pattern and capture the feedback from Sense Amp
          //
          ExecuteBacksideRecEnReadTest (Host, Socket, ChBitmask, Dimm, Rank);

          LoSwizzleStrobe = CheckBacksideSwizzle (Host, Socket, Channel, Dimm, Strobe);
          HiSwizzleStrobe = CheckBacksideSwizzle (Host, Socket, Channel, Dimm, Strobe + MaxStrobePairs);
          LoFeedBackTest1 = ReadDataTrainCounterFeedbackDdr5 (Host, Socket, Channel, SubChannel, LoSwizzleStrobe);
          HiFeedBackTest1 = ReadDataTrainCounterFeedbackDdr5 (Host, Socket, Channel, SubChannel, HiSwizzleStrobe);

          // 4) Program low nibble to a "0" region and high nibble to a "1" region
          //    - Low Nibble test offset  = Low Nibble rising edge - Constant (Same value as previous)
          //    - High Nibble test offset = High Nibble rising edge + Constant
          HiCalculatedDelay = (HiNibbleRisingEdge + NibbleConstant) % 64;

          DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE3_REG, HiCalculatedDelay);

          // 5) Start the pattern and capture feedback
          ExecuteBacksideRecEnReadTest (Host, Socket, ChBitmask, Dimm, Rank);

          LoSwizzleStrobe = CheckBacksideSwizzle (Host, Socket, Channel, Dimm, Strobe);
          HiSwizzleStrobe = CheckBacksideSwizzle (Host, Socket, Channel, Dimm, Strobe + MaxStrobePairs);
          LoFeedBackTest2 = ReadDataTrainCounterFeedbackDdr5 (Host, Socket, Channel, SubChannel, LoSwizzleStrobe);
          HiFeedBackTest2 = ReadDataTrainCounterFeedbackDdr5 (Host, Socket, Channel, SubChannel, HiSwizzleStrobe);

          // If low nibble test results are 0 for test1 and 0 for test2 AND
          //    high nibble test results are 0 for test1 and 1 for test 2 THEN there is no nibble swap
          if ((LoFeedBackTest1 == 0) && (LoFeedBackTest2 == 0) && (HiFeedBackTest1 == 0) && (HiFeedBackTest2 > 0)) {
            // No Nibble swap
            (*NibbleSwap)[Channel][Dimm][SubChannel][Strobe] = FALSE;
            (*NibbleSwap)[Channel][Dimm][SubChannel][Strobe + MaxStrobePairs] = FALSE;
          } else {
            //
            // Any other combination of results means there IS nibble swap
            // Save off Nibble Swap status
            //
            (*NibbleSwap)[Channel][Dimm][SubChannel][Strobe] = TRUE;
            (*NibbleSwap)[Channel][Dimm][SubChannel][Strobe + MaxStrobePairs] = TRUE;
          }
        } // if (IsDcpmmPresentDimm(Socket, Channel, Dimm) == TRUE)

        // If there is nibble swap, swap lower and upper nibbles rising edge values here.  When the register programming occurs,
        //  the 'upper nibble' will get programmed to CW 0xE2 (lower nibble delay) and 'lower nibble' will get programmed to
        //  CW 0xE3 (upper nibble delay)
        if ((*NibbleSwap)[Channel][Dimm][SubChannel][Strobe] == TRUE) {
          RcDebugPrint (SDBG_MAX, "Nibble Swap Detected: SubChannel: %d Lo Strobe: %d Hi Strobe: %d\n",
                        SubChannel, Strobe, Strobe + MaxStrobePairs);
          TempValue = RisingEdge[Channel][SubChannel][Strobe];  // Save off lower nibble
          RisingEdge[Channel][SubChannel][Strobe] = RisingEdge[Channel][SubChannel][Strobe + MaxStrobePairs];
          RisingEdge[Channel][SubChannel][Strobe + MaxStrobePairs] = TempValue;
        } else {
          RcDebugPrint (SDBG_MAX, "NO Nibble Swap Detected: SubChannel: %d Lo Strobe: %d Hi Strobe: %d\n",
                        SubChannel, Strobe, Strobe + MaxStrobePairs);
        }
      }
    }
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);
  }
  RcDebugPrint (SDBG_MAX, "MRE Nibble Swap Detection End\n");
}

/**

  Program the final delays for Receive Enable Phase Training (RWE0 - lower/upper nibble receive enable cycle delay,
  RWE2/E3 - lower/upper nibble receive enable phase timing).

  @param[in] Host             - Pointer to the SysHost structure
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] Dimm             - Dimm number
  @param[in] Rank             - Rank number
  @param[in] SubChannel       - SubChannel number
  @param[in] MaxStrobe        - Maximum number of strobes for this device
  @param[in] RisingEdge       - Array containing the rising edge values detected during phase training
  @param[in] CycleDelayValue  - Array containing cycle settings detected during cycle training
  @param[in] NibbleSwap       - Array storing nibble swap information
  @param[out] FinalCycleValue - Array storing the final cycle delay

  @retval  NA

**/
VOID
ProgramFinalMreDelays (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    SubChannel,
  IN UINT8    MaxStrobe,
  IN UINT16   RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN INT8     CycleDelayValue[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN BOOLEAN  (*NibbleSwap)[MAX_CH][MAX_DIMM][SUB_CH][MAX_STROBE_DDR5 / 2],
  OUT INT8    FinalCycleValue[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2]
)
{
  UINT8  Strobe;
  INT8   CycleDelayTemp;
  UINT8  UpperNibbleStrobe;
  UINT8  UpperNibbleCycleDelay;
  UINT8  RegisterCycleValue;
  UINT8  ProgrammedDelay;
  UINT8  CenterTestPoint;

  for (Strobe = 0; Strobe < (MaxStrobe / 2); Strobe++) {
    //
    // Take into account nibble_swap to determine which nibble found the pre-amble:
    //  If NibbleSwap  == FALSE (no nibble swap),
    //    If low nibble sampled 0 ' record the cycle delay for low nibble
    //    If high nibble sampled 0 ' record the cycle delay for high nibble
    //  else  (NibbleSwap == TRUE)
    //    If low nibble sampled 0 ' record the cycle delay for high nibble
    //    If high nibble sampled 0 ' record the cycle delay for low nibble
    //

    // Select appropriate buffer device
    DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, Strobe);

    UpperNibbleStrobe = Strobe + (MaxStrobe / 2);

    // if there is nibble swap, swap the cycle delay for the low and high nibble
    if ((*NibbleSwap)[Channel][Dimm][SubChannel][Strobe] == TRUE) {
      CycleDelayTemp = CycleDelayValue[Channel][SubChannel][Strobe];
      CycleDelayValue[Channel][SubChannel][Strobe] = CycleDelayValue[Channel][SubChannel][UpperNibbleStrobe];
      CycleDelayValue[Channel][SubChannel][UpperNibbleStrobe] = CycleDelayTemp;
    }

    // Use test point that was used to walk back cycle delay to find the preamble
    CenterTestPoint = ((RisingEdge[Channel][SubChannel][Strobe] / 2) + MRE_PHASE_OFFSET) % 64;
    // If Center Point > Rising Edge increase cycle delay by 2
    if (CenterTestPoint > RisingEdge[Channel][SubChannel][Strobe] / 2) {
      CycleDelayValue[Channel][SubChannel][Strobe] += 2;
    } else {
      CycleDelayValue[Channel][SubChannel][Strobe] += 1;
    }

    CenterTestPoint = ((RisingEdge[Channel][SubChannel][UpperNibbleStrobe] / 2) + MRE_PHASE_OFFSET) % 64;
    if (CenterTestPoint > RisingEdge[Channel][SubChannel][UpperNibbleStrobe] / 2) {
      CycleDelayValue[Channel][SubChannel][UpperNibbleStrobe] += 2;
    } else {
      CycleDelayValue[Channel][SubChannel][UpperNibbleStrobe] += 1;
    }

    // Save final programmed cycle value (save results without nibble swap applied)
    if ((*NibbleSwap)[Channel][Dimm][SubChannel][Strobe] == TRUE) {
      FinalCycleValue[Channel][Dimm][SubChannel][Rank][Strobe] = CycleDelayValue[Channel][SubChannel][UpperNibbleStrobe];
      FinalCycleValue[Channel][Dimm][SubChannel][Rank][UpperNibbleStrobe] = CycleDelayValue[Channel][SubChannel][Strobe];
    } else {
      FinalCycleValue[Channel][Dimm][SubChannel][Rank][Strobe] = CycleDelayValue[Channel][SubChannel][Strobe];
      FinalCycleValue[Channel][Dimm][SubChannel][Rank][UpperNibbleStrobe] = CycleDelayValue[Channel][SubChannel][UpperNibbleStrobe];
    }

    //
    // Program the cycle Delay
    //
    RegisterCycleValue = ABS (CycleDelayValue[Channel][SubChannel][Strobe]);
    if (CycleDelayValue[Channel][SubChannel][Strobe] < 0) {
      RegisterCycleValue |= BIT3;
    }
    UpperNibbleCycleDelay = ABS (CycleDelayValue[Channel][SubChannel][UpperNibbleStrobe]);
    if (CycleDelayValue[Channel][SubChannel][UpperNibbleStrobe] < 0) {
      UpperNibbleCycleDelay |= BIT3;
    }
    RegisterCycleValue |= (UpperNibbleCycleDelay << 4);
    DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE0_REG, RegisterCycleValue);

    // h.  Final calculations for each nibble:
    //     - Program the rising edge (nibble swap already taken into account for phase delay)
    ProgrammedDelay = (UINT8) RisingEdge[Channel][SubChannel][Strobe]/2;
    DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE2_REG, ProgrammedDelay);
    ProgrammedDelay = (UINT8) RisingEdge[Channel][SubChannel][UpperNibbleStrobe]/2;
    DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE3_REG, ProgrammedDelay);
  }
}

/**

  Perform LRDIMM MDQ Receive Enable Phase Training (MREP)

  @param[in] Socket - Socket number

  @retval Status

**/
UINT32
EFIAPI
MDQSReceiveEnablePhaseCycleTraining (
  IN UINT8     Socket
  )
{
  UINT8                             Ch;
  UINT8                             Dimm;
  UINT8                             Rank;
  UINT32                            ChBitmask;
  UINT8                             LogRank = 0;
  UINT8                             LogSubRank = 0;
  UINT32                            Status = MRC_STATUS_SUCCESS;
  UINT8                             Strobe;
  UINT8                             DWord;
  struct channelNvram               (*ChannelNvList) [MAX_CH];
  struct dimmNvram                  (*DimmNvList) [MAX_DIMM];
  struct ddrRank                    (*RankList) [MAX_RANK_DIMM];
  struct rankDevice                 (*RankStruct) [MAX_RANK_DIMM];
  struct TrainingResults            (*LrTrainRes) [MAX_CH][MAX_STROBE];
  UINT8                             NibblePhaseSetting;
  UINT16                            RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT16                            CenterPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT16                            FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT16                            PulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  BOOLEAN                           (*NibbleSwap)[MAX_CH][MAX_DIMM][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT8                             CalculatedDelay;
  UINT8                             FaultyPartsStatus;
  struct TrainingResultsDdr5        (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT8                              CycleDelay = 0;
  UINT8                             RegisterCycleValue;
  UINT8                             ResultPass[MAX_CH][SUB_CH][MAX_STROBE/2];
  UINT8                             ResultFailChSch[MAX_CH][SUB_CH];
  UINT8                             ResultPassChSch[MAX_CH][SUB_CH];
  UINT8                             SubCh = 0;
  BOOLEAN                           NextCycle = FALSE;
  UINT32                            StrobePass[MAX_CH][SUB_CH];
  INT8                              CyclePhaseResult[MAX_CH][SUB_CH];
  INT8                              CycleDelayValue[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT8                              CycleDelayTemp[MAX_CH][SUB_CH];
  UINT32                            StrobeMask[MAX_CH];
  UINT8                             Done = 0;
  UINT8                             Fail = 0;
  UINT8                             Loop = 0;
  UINT8                             DoneSum = 0;
  PSYSHOST                          Host = GetSysHostPointer ();
  UINT8                             MSVx4 = GetMaxStrobeValid (Host);
  UINT8                             SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8                             MaxChDdr;
  ACT_TO_ACT_SAVE_STRUCT            ActToActSave[MAX_CH];
  UINT8                             ChannelLocal;
  UINT8                             DataBuffer;
  DDR5_LRDIMM_TRAINING_DATA         *TrainingData;
  UINT8                             FinalPhaseValue[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2];
  INT8                              FinalCycleValue[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2];

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  LrTrainRes = &Host->var.mem.lrTrainRes;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "DDR5 LRDIMM MDQS Receive Enable Training\n");
  // Nibble Swap information is needed in MRD (MDQS Read Delay Training). Save off a pointer here
  //  so it can be retrieved in MRD.
  Status = (UINT32) GetDdr5LrdimmTrainingData (&TrainingData);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }
  NibbleSwap = &(TrainingData->NibbleSwap);

  TrainRes = RcAllocatePool (sizeof (*TrainRes));

  //
  // Clear the training result.
  //
  ZeroMem ((UINT8 *) ResultFailChSch, sizeof (ResultFailChSch));
  ZeroMem ((UINT8 *) StrobePass,                    sizeof (StrobePass));
  ZeroMem ((UINT8 *) CyclePhaseResult,              sizeof (CyclePhaseResult));
  ZeroMem ((UINT8 *) CycleDelayValue,               sizeof (CycleDelayValue));
  ZeroMem ((UINT8 *) StrobeMask, sizeof (StrobeMask));
  ReceiveEnableInitAllRanks (Host, Socket); //lrdimm rcven doesn't have roundtrip latency set.

  // DDRIO initialization for MRE
  MreDdrioInit (Socket);

  SetAepTrainingMode (Host, Socket, DISABLE_TRAINING_STEP);
  SetMem(ResultPassChSch, sizeof(ResultPassChSch), 1);
  //
  // Train each DIMM
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Train each Rank
    //
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_RX_BACKSIDE_PHASE_TRAINING, (UINT16) ((Socket << 8) | (Dimm << 4) |Rank)));
      //
      // Create a Bit mask of channels that have at least 1 Rank present for this Dimm and Rank
      //
      ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, Rank);
      //
      // Go to the next Rank if this Rank is not present on any channels
      //
      if (ChBitmask == 0) {
        continue;
      }

      // Clear training results
      ZeroMem ((UINT8 *) TrainRes, sizeof (*TrainRes));

      SetCurrentTestType(Socket, MdqsReceiveEnablePhaseTraining);

      for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
        if (!((1 << Ch) & ChBitmask)) {
          continue;
        }

        ProgramCasLDdr5 (Host, Socket, Ch);
        //
        // Init DRAM interface only for CTE.
        //
        if (FeaturePcdGet (PcdCteBuild) == TRUE) {
          //
          // Init DB and DRAM interface only for CTE. To add #ifdef YAM_ENV
          //
          InitDbDramInterface (Socket, Ch, Dimm, 3, Rank);
        }

        ActToActSave[Ch].TRrdSOrg = 0;
        ActToActSave[Ch].TRrdLOrg = 0;
        ActToActSave[Ch].TRcdRdOrg = 0;
        ActToActSave[Ch].TRrsrOrg = 0;
        ActToActSave[Ch].TRrSgOrg = 0;

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          ChannelLocal = Ch;
          GetChannelMapping (Ch, SubCh, &ChannelLocal);
          //
          // Get the logical Rank #
          //
          LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

          SetActToActPeriod (Host, Socket, ChannelLocal, ActToActSave);
          //
          // Indicate this Rank is present on at least one channel
          //
          if ((*ChannelNvList) [Ch].encodedCSMode) {
            LogSubRank = (Rank >> 1);
          } else {
            LogSubRank = 0;
          }

          SelectCPGCRanks (Host, Socket, ChannelLocal, 1 << LogRank, 0, LogSubRank);
        } //Subch loop

        //
        // Clear training results variable for this channel
        //
        for (Strobe = 0; Strobe < MSVx4; Strobe++) {
          for (DWord = 0; DWord < 4; DWord++) {
            (*LrTrainRes) [Ch][Strobe].results[DWord] = 0;
          } // DWord loop
        } // Strobe loop
      } // Ch loop

      //
      // Program counting window settings and enable receivers/ODT.
      //
      SetDataTrainCountWindow (Host, Socket, Dimm, Rank, MRE_COUNTING_WINDOW);
      SetDqCountingWindow (Host, Socket, Dimm, Rank);
      //
      // Training Init
      //
      LrdimmMdqsRecInit (Socket, ChBitmask, Dimm, Rank);
      //
      //5.  For each back-side Rank [x]:
      //
      // Find the DQ rising edge
      //
      //
      // b) For nibble phase settings of 0 to 64, do the following:
      //
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "DDR5 LRDIMM MDQS RecEn Phase Training - Pi Scanning:\n");
      for (CycleDelay = MDQS_PHASE_CYCLE_START ; CycleDelay <= MDQS_PHASE_CYCLE_END ; CycleDelay++) {
        for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
          if (!((1 << Ch) & ChBitmask)) {
            continue;
          }
          //
          // Program the cycle Delay
          //
          RegisterCycleValue = ABS (CycleDelay);
          if (CycleDelay < 0) {
            RegisterCycleValue |= BIT3;
          }
          RegisterCycleValue |= (RegisterCycleValue << 4);
          if (CycleDelay == MDQS_PHASE_CYCLE_START) {
            DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE0_REG, RegisterCycleValue);
          } else {
            for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
              if (ResultFailChSch[Ch][SubCh] == 1) {
                DbControlWordWrite (Host, Socket, Ch, 1 << SubCh, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE0_REG, RegisterCycleValue);
              } //if
            } //Subch
          } //if

          for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
            if (ResultFailChSch[Ch][SubCh] == 1) {
              for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
                for (DWord = 0; DWord < DDR5_TRAIN_RESULTS_LEN; DWord++) {
                  (*TrainRes) [Ch][SubCh][Strobe].results[DWord] = 0;
                }
              }
            }
          }
        } //Ch loop
        ZeroMem ((UINT8 *) ResultPass, sizeof (ResultPass));
        for (NibblePhaseSetting = LRDIMM_BACKSIDE_PI_START; NibblePhaseSetting < LRDIMM_BACKSIDE_PI_RANGE; NibblePhaseSetting += LRDIMM_BACKSIDE_STEP_SIZE) {
          for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
            if (!((1 << Ch) & ChBitmask)) {
              continue;
            }

            //
            // Set CW E2/E3 to phase setting for all buffers

            if (CycleDelay == MDQS_PHASE_CYCLE_START) {
              DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE2_REG, (NibblePhaseSetting >> 1));
              DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE3_REG, (NibblePhaseSetting >> 1));
            } else {
              for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
                if (ResultFailChSch[Ch][SubCh] == 1) {
                  DbControlWordWrite (Host, Socket, Ch, 1 << SubCh, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE2_REG, (NibblePhaseSetting >> 1));
                  DbControlWordWrite (Host, Socket, Ch, 1 << SubCh, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE3_REG, (NibblePhaseSetting >> 1));
                } //if
              }//Subch
            }//if
          } // Ch loop

          //
          // read results from training results registers
          //
          GetLrdimmPhaseTrainResultsDdr5 (Socket, Dimm, Rank, NibblePhaseSetting, LRDIMM_MREP_TRAINING_MODE, CycleDelay, TrainRes);
        } //NibblePhaseSetting loop

        for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
          if (!((1 << Ch) & ChBitmask)) {
            continue;
          }
          DimmNvList = GetDimmNvList (Host, Socket, Ch);

          if ((*DimmNvList) [Dimm].dimmPresent == 0) {
            Host->var.mem.piSettingStopFlag[Ch] = 0;
            continue;
          }

          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            Host->var.mem.piSettingStopFlag[Ch] = 0;
            continue;
          }

          Host->var.mem.piSettingStopFlag[Ch] = GetStrobeMask (Host);

        } // Ch loop

        for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
          if (!((1 << Ch) & ChBitmask)) {
            continue;
          }
          RankList = GetRankNvList (Host, Socket, Ch, Dimm);
          DimmNvList = GetDimmNvList (Host, Socket, Ch);

          RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
          for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
            if ((ResultFailChSch[Ch][SubCh] == 1) || (CycleDelay == MDQS_PHASE_CYCLE_START)) {
              ResultPassChSch[Ch][SubCh] = 1;
#ifdef DEBUG_CODE_BLOCK
              if (GetDebugLevel () & SDBG_MAX) {
                RcDebugPrint (SDBG_MINMAX, "\nSummary: LRDIMM MDQS RcvEn Phase Training\nS%d, Ch%d, SC%d, DIMM%d, Rank%d, Cycle Delay = %d\n",
                              Socket, Ch, SubCh, Dimm, Rank, CycleDelay);
                PrintSampleArrayDdr5 (Host, Socket, Ch, SubCh, Dimm, PRINT_ARRAY_TYPE_LRDIMM_RCV_EN_PHASE_DDR5, TrainRes);
              }
#endif // DEBUG_CODE_BLOCK
              for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
                //
                // Skip if this is an ECC Strobe when ECC is disabled
                //
                if (IsStrobeNotValidDdr5 (Host, Strobe)) {
                  continue;
                }

                EvaluatePiSettingResultsDdr5 (Socket, Ch, SubCh, Dimm, Rank, Strobe, RisingEdge, CenterPoint, FallingEdge, PulseWidth,
                  FPT_PI_LRDIMM_READ_TYPE, TrainRes);

                if (Host->nvram.mem.socket[Socket].FaultyPartsFlag[Ch] & (1 << Strobe)) {
                  //
                  // Add this error to the warning log for both correctable and uncorrectable errors.
                  //
                  FaultyPartsStatus = EvaluateFaultyParts (Host, Socket, Ch);

                  if (FaultyPartsStatus == FPT_NO_ERROR) {
                    // do we need to do anything here ?!?
                  } else if (FaultyPartsStatus == FPT_CORRECTABLE_ERROR) {
                    // if correctable  log the warning -> OutputWarning
                    // careful for cases when warning gets promoted to error !!!
                    EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_RCVEN_PHASE_TRAINING, Socket, Ch, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, RecEnDelay, LrbufLevel, (UINT8) PulseWidth[Ch][SubCh][Strobe]);
                  } else {
                    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
                      "LRDIMM training failure!!!\n");
                    //
                    // Add this error to the warning log for both correctable and uncorrectable errors.
                    //
                    Host->var.mem.piSettingStopFlag[Ch] |= (1 << Strobe);
                    DisableChannelSw (Host, Socket, Ch);
                    EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_RCVEN_PHASE_TRAINING, Socket, Ch, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, RecEnDelay, LrbufLevel, (UINT8) PulseWidth[Ch][SubCh][Strobe]);
                  }
                }
                // If no edges found, increase cycle delay and retry
                if ((RisingEdge[Ch][SubCh][Strobe] == 0) && (FallingEdge[Ch][SubCh][Strobe] == 0)) {
                  NextCycle = TRUE;
                  ResultFailChSch[Ch][SubCh] = 1;
                  ResultPass[Ch][SubCh][Strobe] = 0;
                } else {
                  // Edges found, indicate we do not need to go to next cycle value
                  ResultPass[Ch][SubCh][Strobe] = 1;
                  // Save off rising edge values (before considering nibble swap)
                  FinalPhaseValue[Ch][Dimm][SubCh][Rank][Strobe] = (UINT8) (RisingEdge[Ch][SubCh][Strobe] / 2);
                }
              } // Strobe loop

              for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
                //
                // Skip if this is an ECC Strobe when ECC is disabled
                //
                if (IsStrobeNotValidDdr5 (Host, Strobe)) {
                  continue;
                }
                ResultPassChSch[Ch][SubCh] = ResultPassChSch[Ch][SubCh] & ResultPass[Ch][SubCh][Strobe];
              }
              DisplayEdgesDDR5 (Host, Socket, Ch, PulseWidth[Ch], RisingEdge[Ch], FallingEdge[Ch], SubCh, SubChMSVx4);
            }//if
          } //Subch loop
          for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
            if (ResultPassChSch[Ch][SubCh] == 1) {
              CyclePhaseResult[Ch][SubCh] = CycleDelay;
              ResultPassChSch[Ch][SubCh] = 0;
            }
          }
        }//Ch loop
        if (NextCycle != TRUE) {
          break;
        }
      }//Cycle Delay loop

      NibbleSwapDetection (Host, Socket, Dimm, Rank, ChBitmask, CyclePhaseResult, RisingEdge, NibbleSwap);
      //
      // Program the result
      //
      for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
        if (!((1 << Ch) & ChBitmask)) {
          continue;
        }
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "Rx Skew Adjust\n");
          for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
            //
            // store the data to write for later use
            //
            //save off the "true" value in the cached variable
            CalculatedDelay = (UINT8) (((RisingEdge[Ch][SubCh][Strobe]/2) + MRE_PHASE_OFFSET) % 64);
            (*RankStruct) [Rank].cachedLrBuf_RWE2E3[Strobe + (SubCh * SubChMSVx4)] = CalculatedDelay;
            (*RankList) [Rank].lrBuf_RWE2E3[Strobe + (SubCh * SubChMSVx4)] = (*RankStruct) [Rank].cachedLrBuf_RWE2E3[Strobe + (SubCh * SubChMSVx4)];

            DataBuffer = (Strobe % (SubChMSVx4 / 2));
            // Select appropriate buffer device
            DbControlWordWrite (Host, Socket, Ch, (1 << SubCh), Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, DataBuffer);

            // Already swapped RE values, so no need to swap here - just program E2 and E3 Control words
            if (Strobe < SubChMSVx4 / 2) {
              DbControlWordWrite (Host, Socket, Ch, (1 << SubCh), Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE2_REG, CalculatedDelay);
            } else {
              DbControlWordWrite (Host, Socket, Ch, (1 << SubCh), Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE3_REG, CalculatedDelay);
            }
          }//Strobe
        } //Subch loop
        DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);
      } // Ch loop

      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "DDR5 LRDIMM MDQS RecEn Cycle Training\n");

      SetCurrentTestType (Socket, MdqsReceiveEnableCycleTraining);
      ZeroMem ((UINT8 *) CycleDelayValue,               sizeof (CycleDelayValue));
      ZeroMem ((UINT8 *) StrobePass,                    sizeof (StrobePass));
      ZeroMem ((UINT8 *) ResultPassChSch, sizeof (ResultPassChSch));
      //
      // Start the MDQS Receive Enable Cycle Training
      //
      ZeroMem ((UINT8 *) TrainRes, sizeof (*TrainRes));
      for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          SetMem (CycleDelayValue[Ch][SubCh], sizeof (INT8) * SubChMSVx4, CyclePhaseResult[Ch][SubCh]);
        }
      }

      for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          CycleDelayTemp[Ch][SubCh] = CyclePhaseResult[Ch][SubCh];
        }
      }

      //
      // Initialzie the DoneSum as 0.
      //
      DoneSum = 0;
      for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
        if (!((1 << Ch) & ChBitmask)) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (Rank >= (*DimmNvList) [Dimm].numRanks) {
          continue;
        }
        //
        // Set Strobe Bit masks
        //
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        StrobeMask[Ch] = GetSubChStrobeMaskDdr5 (Host);
        if (!(*DimmNvList) [Dimm].x4Present) {
          StrobeMask[Ch] &= STROBE_MASK_X8;
        }
        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          DoneSum = DoneSum + 1;
        }
      } //Ch loop

      Done = 0;
      Fail = 0;
      CycleDelay = 0;
      for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
        if (!((1 << Ch) & ChBitmask)) {
          continue;
        }
        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          if (CycleDelay < CyclePhaseResult[Ch][SubCh]) {
            CycleDelay = CyclePhaseResult[Ch][SubCh];
          }
        }
      }
      for (Loop = 0; CycleDelay >= MIN_MDQS_CYCLE_DELAY; CycleDelay += MDQS_CYCLE_TRAINING_STEP_SIZE, Loop++) {
        //Loop = Loop + 1;
        for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
          if (!((1 << Ch) & ChBitmask)) {
            continue;
          }

          //
          //i.  Set F[x]BC2x and F[x]BC3x to phase setting in all buffers
          //    x = backside Rank 0:3
          //

          RegisterCycleValue = ABS (CycleDelay);
          if (CycleDelay < 0) {
            RegisterCycleValue |= BIT3;
          }
          RegisterCycleValue |= (RegisterCycleValue << 4);

          for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
            if (ResultPassChSch[Ch][SubCh] == 0) {
              DbControlWordWrite (Host, Socket, Ch, 1 << SubCh, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE0_REG, RegisterCycleValue);
            } //if
          } //Subch
        } // Ch loop

        //
        // read results from training results registers
        //
        GetLrdimmCycleTrainResultsDdr5 (Socket, Dimm, Rank, Loop, LRDIMM_MREP_TRAINING_MODE, CycleDelay, StrobePass, CycleDelayValue, TrainRes);
      } //CycleDelay loop

      for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
        if (!((1 << Ch) & ChBitmask)) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          Host->var.mem.piSettingStopFlag[Ch] = 0;
          continue;
        }

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          Host->var.mem.piSettingStopFlag[Ch] = 0;
          continue;
        }

        Host->var.mem.piSettingStopFlag[Ch] = GetStrobeMask (Host);

      } // Ch loop

      for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
        if (!((1 << Ch) & ChBitmask)) {
          continue;
        }
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          if (GetDebugLevel () & SDBG_MAX) {
            RcDebugPrint (SDBG_MINMAX, "\nSummary: LRDIMM MDQS RcvEn Cycle Training\nS%d, Ch%d, SC%d, DIMM%d, Rank%d, Cycle Delay = %d\n",
                          Socket, Ch, SubCh, Dimm, Rank, CyclePhaseResult[Ch][SubCh]);
            PrintMdqsReceiveEnableCycleDdr5  (Socket, Ch, Dimm, 0, SubCh, 8, CyclePhaseResult[Ch][SubCh], TrainRes, CycleDelayValue);
          }

          for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
            //
            // Skip if this is an ECC Strobe when ECC is disabled
            //
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }

            if (Host->nvram.mem.socket[Socket].FaultyPartsFlag[Ch] & (1 << Strobe)) {
              //
              // Add this error to the warning log for both correctable and uncorrectable errors.
              //
              FaultyPartsStatus = EvaluateFaultyParts (Host, Socket, Ch);

              if (FaultyPartsStatus == FPT_NO_ERROR) {
                // do we need to do anything here ?!?
              } else if (FaultyPartsStatus == FPT_CORRECTABLE_ERROR) {
                // if correctable  log the warning -> OutputWarning
                // careful for cases when warning gets promoted to error !!!
                EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_RCVEN_PHASE_TRAINING, Socket, Ch, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, RecEnDelay, LrbufLevel, (UINT8) PulseWidth[Ch][SubCh][Strobe]);
              } else {
                RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
                  "LRDIMM training failure!!!\n");
                //
                // Add this error to the warning log for both correctable and uncorrectable errors.
                //
                Host->var.mem.piSettingStopFlag[Ch] |= (1 << Strobe);
                DisableChannelSw (Host, Socket, Ch);
                EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_RCVEN_PHASE_TRAINING, Socket, Ch, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, RecEnDelay, LrbufLevel, (UINT8) PulseWidth[Ch][SubCh][Strobe]);
              }
            }
            // If no edges found, increase cycle delay and retry
            if ((RisingEdge[Ch][SubCh][Strobe] == 0) && (FallingEdge[Ch][SubCh][Strobe] == 0)) {
              NextCycle = TRUE;
              ResultFailChSch[Ch][SubCh] = 1;
            } else {
              ResultPass[Ch][SubCh][Strobe] = 1;
            }
          } // Strobe loop
          //
          // Program the final result. + 2 (cycle), -16 (phase) to the cycle Delay
          //
          ProgramFinalMreDelays (Host, Socket, Ch, Dimm, Rank, SubCh, SubChMSVx4, RisingEdge, CycleDelayValue, NibbleSwap, FinalCycleValue);
        } //Subch loop
      }//Ch loop

      LrdimmMdqsRecCleanup (Socket, ChBitmask, Dimm, Rank);

      // Restore timings to saved values
      for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
        if (!((1 << Ch) & ChBitmask)) {
          continue;
        }

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          ChannelLocal = Ch;
          GetChannelMapping (Ch, SubCh, &ChannelLocal);
          RestoreActToActPeriod (Socket, ChannelLocal, ActToActSave);
        }
      }
    } //Rank
  } //Dimm

  DisplayFinalMreResults (Host, Socket, FinalPhaseValue, FinalCycleValue);

  // DDRIO cleanup
  PostMreDdrioProgramming (Socket);

  SetAepTrainingMode (Host,Socket, DISABLE_TRAINING_STEP);
  FifoTrainReset (Host, Socket);
  RcFreePool (TrainRes);
  return Status;
} //MDQSReceiveEnablePhaseCycleTraining

/**

  Performs the CPGC setup necessary for coarse read delay training.

  @param[in] Host              - Pointer to the SysHost structure
  @param[in] Socket            - Socket number
  @param[in] ChBitmask         - Channel bit mask
  @param[in] Dimm              - Dimm number
  @param[in] Rank              - Rank number

  @retval N/A

**/
VOID
SetupCpgcForReadDelay (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  UINT8                Channel;
  UINT8                SubChannel;
  UINT8                ChannelLocal;
  UINT8                LogRank;
  UINT8                LogSubRank = 0;
  struct channelNvram  (*ChannelNvList)[MAX_CH];
  struct dimmNvram     (*DimmNvList)[MAX_DIMM];

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Channel = 0; Channel < MAX_CH; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Channel);

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      ChannelLocal = Channel;
      GetChannelMapping (Channel, SubChannel, &ChannelLocal);
      //
      // Get the logical Rank #
      //
      LogRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);

      //
      // Indicate this Rank is present on at least one channel
      //
      if ((*ChannelNvList) [Channel].encodedCSMode) {
        LogSubRank = (Rank >> 1);
      } else {
        LogSubRank = 0;
      }

      SelectCPGCRanks (Host, Socket, ChannelLocal, 1 << LogRank, 0, LogSubRank);
    } //Subch loop

    SetMprModeFilterActPre (Host, Socket, Channel, 1);
    ConvertRdToMrr (Host, Socket, Channel, 1);
  } // ch loop
}

/**

  Performs the CPGC setup necessary for write delay training.

  @param[in] Host              - Pointer to the SysHost structure
  @param[in] Socket            - Socket number
  @param[in] ChBitmask         - Channel bit mask
  @param[in] Dimm              - Dimm number
  @param[in] Rank              - Rank number

  @retval N/A

**/
VOID
SetupCpgcForWriteDelay (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  UINT8                Channel;
  UINT8                SubChannel;
  UINT8                ChannelLocal;
  UINT8                LogRank;
  UINT8                LogSubRank = 0;
  struct channelNvram  (*ChannelNvList)[MAX_CH];
  struct dimmNvram     (*DimmNvList)[MAX_DIMM];

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Channel = 0; Channel < MAX_CH; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Channel);

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      ChannelLocal = Channel;
      GetChannelMapping (Channel, SubChannel, &ChannelLocal);
      //
      // Get the logical Rank #
      //
      LogRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);

      //
      // Indicate this Rank is present on at least one channel
      //
      if ((*ChannelNvList) [Channel].encodedCSMode) {
        LogSubRank = (Rank >> 1);
      } else {
        LogSubRank = 0;
      }

      SelectCPGCRanks (Host, Socket, ChannelLocal, 1 << LogRank, 0, LogSubRank);
    } //Subch loop
  } // ch loop
}

/**

  Initialize LRDIMM Coarse MDQS Read Delay training parameters.

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket number
  @param[in] ChBitmask  - Channel bit mask
  @param[in] Dimm       - Dimm number
  @param[in] Rank       - Rank number

  @retval N/A

**/
VOID
LrdimmCoarseMrdInit (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  UINT8                         Channel;
  DDR5_MODE_REGISTER_25_STRUCT  Mr25;
  DDR5_DATA_BUFFER_RW97_STRUCT  BufferTrainingConfig;

  SetDataTrainCountWindow (Host, Socket, Dimm, Rank, MRD_COUNTING_WINDOW);

  for (Channel = 0; Channel < MAX_CH; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
      continue;
    }
    if (IsRankValid (Socket, Channel, Dimm, Rank, FALSE) == FALSE) {
      continue;
    }

    if (FeaturePcdGet (PcdCteBuild) == TRUE) {
      //
      // Init DB and DRAM interface only for CTE. To add #ifdef YAM_ENV
      //
      InitDbDramInterface (Socket, Channel, Dimm, TWO_SUBCH_MASK, Rank);
    }

    // c. Configure the pattern generated by the DRAM (MR25 to MR30)
    // d. Configure the pattern on the DB (snooped through DRAM programming)
    Mr25.Data = 0;
    // Use LFSR mode
    Mr25.Bits.read_training_pattern_format = 1;
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_25_REG, Mr25.Data);

    // Set the LFSR0 as the default value 0x5A
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_26_REG, MR26_DEFAULT_RD_TRAIN);

    // Set the LFSR1 as the default value 0x3C
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_27_REG, MR27_DEFAULT_RD_TRAIN);

    // LFSR0 on all bits, no pattern inversion for any DQ bit
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_28_REG, 0);
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_29_REG, 0);
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_30_REG, 0);

    // e. Set the DB training config (RW97) to per transaction.  Clear feedback.
    BufferTrainingConfig.Data = 0;
    BufferTrainingConfig.Bits.clear_feedback_status_bit = 1;
    BufferTrainingConfig.Bits.per_bit_vs_per_transaction = 1;
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW97_REG, BufferTrainingConfig.Data);

    // Select appropriate DB page to program DB pattern
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGE8);
    Mr25.Data = 0;
    // Use LFSR mode
    Mr25.Bits.read_training_pattern_format = 1;
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE2_REG, Mr25.Data);
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE3_REG, MR26_DEFAULT_RD_TRAIN);
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE4_REG, MR27_DEFAULT_RD_TRAIN);
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE5_REG, 0);
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE6_REG, 0);

    // f. Enable MRD training mode on DB
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW83_REG, LRDIMM_RDDL_TRAINING);

    // Select appropriate DB page
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, Rank);
  }
}

/**

  Encodes the register Read Delay value such that values 0 to 15 maps to settings -15 to 0
  and values 16 to 31 maps to values 1 to 16. The decode option undoes this mapping.

  @param[in] Code      - Specifies whether to encode or decode the delay
  @param[in] ReadDelay - The read delay value to be encoded/decoded

  @retval UINT8 - the encoded/decoded delay value

**/
UINT8
EncodeDecodeReadDelay (
  CODED_READDELAY  Code,
  UINT8            ReadDelay
  )
{
  UINT8  CodedDelayResult;

  if (Code == Encode) {
    //
    // The encoded read delay is such that values 0 to 15d map to settings 0 to + 15,
    // and values 16 to 31 map to values 0 to -15 respectively.  However, to find
    // the widest passing region, it is more convient to map the results such that
    // values 0 to 15 maps to settings -15 to 0 and values 16 to 31 maps to values
    // 1 to 16.
    if (ReadDelay <= POSITIVE_ENCODE) {
      CodedDelayResult = ReadDelay + POSITIVE_ENCODE;
    } else {
      CodedDelayResult = ReadDelay ^ NEGATIVE_ENCODE;
    }
  } else {
    // Decode read delay
    if (ReadDelay < POSITIVE_ENCODE) {
      CodedDelayResult = ReadDelay ^ NEGATIVE_ENCODE;
    } else {
      CodedDelayResult = ReadDelay - POSITIVE_ENCODE;
    }
  }

  return CodedDelayResult;
}

/**

  Initialize LRDIMM Backside Swizzle Discovery training parameters.

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket number
  @param[in] ChBitmask  - Channel bit mask
  @param[in] Dimm       - Dimm number
  @param[in] Rank       - Rank number

  @retval N/A

**/
VOID
LrdimmBacksideSwizzleInit (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  UINT8                         Channel;
  struct channelNvram           (*ChannelNvList)[MAX_CH];
  DDR5_DATA_BUFFER_RW97_STRUCT  BufferTrainingConfig;

  ChannelNvList = GetChannelNvList (Host, Socket);

  SetDataTrainCountWindow (Host, Socket, Dimm, Rank, SWZ_COUNTING_WINDOW);

  for (Channel = 0; Channel < MAX_CH; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
      continue;
    }
    if (IsRankValid (Socket, Channel, Dimm, Rank, FALSE) == FALSE) {
      continue;
    }

    if (FeaturePcdGet (PcdCteBuild) == TRUE) {
      //
      // Init DB and DRAM interface only for CTE. To add #ifdef YAM_ENV
      //
      InitDbDramInterface (Socket, Channel, Dimm, TWO_SUBCH_MASK, Rank);
    }

    // Configure the pattern generated by the DRAM (MR25 to MR30).
    // Set Read Training Mode as default (Serial)
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_25_REG, 0);

    // Configure pattern - High/Low 0xFFFF
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_26_REG, MR26_SWIZZLE_PATTERN);
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_27_REG, MR27_SWIZZLE_PATTERN);

    // No pattern inversion (default values)
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_28_REG, 0);
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_29_REG, 0);
    // Set Read Assignments default (BUFF0)
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_30_REG, 0);

    // Set the DB training config (RW97) to per transaction.  Clear feedback.
    BufferTrainingConfig.Data = 0;
    BufferTrainingConfig.Bits.clear_feedback_status_bit = 1;
    BufferTrainingConfig.Bits.per_bit_vs_per_transaction = 1;
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW97_REG, BufferTrainingConfig.Data);

    // Select appropriate DB page to program DB pattern - initial pattern is snooped from DRAM programming. Page 8
    // selection is needed for programming the invert bits during the backside sweep.
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGE8);

    // Configure the DB Buffer Training Configuration and enable MRD mode.
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW83_REG, LRDIMM_RDDL_TRAINING);

  }
}

/**

  Initialize LRDIMM Frontside Swizzle Discovery training parameters.

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket number
  @param[in] ChBitmask  - Channel bit mask
  @param[in] Dimm       - Dimm number
  @param[in] Rank       - Rank number

  @retval N/A

**/
VOID
LrdimmFrontsideSwizzleInit (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  UINT8                         Channel;
  struct channelNvram           (*ChannelNvList)[MAX_CH];
  DDR5_DATA_BUFFER_RW97_STRUCT  BufferTrainingConfig;

  ChannelNvList = GetChannelNvList (Host, Socket);

  SetDataTrainCountWindow (Host, Socket, Dimm, Rank, SWZ_COUNTING_WINDOW);

  for (Channel = 0; Channel < MAX_CH; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
      continue;
    }
    if (IsRankValid (Socket, Channel, Dimm, Rank, FALSE) == FALSE) {
      continue;
    }

    if (FeaturePcdGet (PcdCteBuild) == TRUE) {
      //
      // Init DB and DRAM interface only for CTE. To add #ifdef YAM_ENV
      //
      InitDbDramInterface (Socket, Channel, Dimm, TWO_SUBCH_MASK, Rank);
    }

    // c. Set the DB Buffer training configuration (RW97) with 'per-bit' results
    BufferTrainingConfig.Data = 0;
    BufferTrainingConfig.Bits.clear_feedback_status_bit = 1;
    BufferTrainingConfig.Bits.per_bit_vs_per_transaction = 0;
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW97_REG, BufferTrainingConfig.Data);

    // d. Enable MRD training Mode on the DB
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW83_REG, LRDIMM_RDDL_TRAINING);

    // e. Set all DRAMs to drive the same pattern (BUFF0) on all DQs with no pattern inversion
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_25_REG, 0);

    // Configure pattern - High/Low 0xFFFF
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_26_REG, MR26_SWIZZLE_PATTERN);
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_27_REG, MR27_SWIZZLE_PATTERN);

    // No pattern inversion
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_28_REG, 0);
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_29_REG, 0);

    // Select appropriate DB page to program DB pattern
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGE8);
  } // Channel loop
}

/**

  Programs the Data Buffer read pattern for Per-Bit Read Delay Deskew training.  This takes into
  account the Data Buffer to DRAM DQ mapping found during backside swizzle discovery.

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket number
  @param[in] Channel  - Channel number
  @param[in] Dimm     - Dimm number
  @param[in] Rank     - Rank number

  @retval Status  - Non-zero if any failure detected

**/
MRC_STATUS
ProgramDbDeskewPattern (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  UINT8                         SubChannel;
  UINT8                         DataBuffer;
  UINT8                         DqNumber;
  UINT8                         Dword;
  UINT32                        SwizzleValue;
  UINT8                         DqMask = 0x7;  // Each entry in DbToDramDqMap is 3 bits
  MRC_STATUS                    Status = MRC_STATUS_SUCCESS;
  DDR5_LRDIMM_TRAINING_DATA     *TrainingData;
  UINT32                        (*DbToDramDqMap)[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][SUB_CH][SWIZZLE_RESULTS_LEN];
  DDR5_MODE_REGISTER_30_STRUCT  Mr30;

  // Retrieve DB to DRAM DQ mapping info from swizzle discovery
  Status = GetDdr5LrdimmTrainingData (&TrainingData);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }
  DbToDramDqMap = &(TrainingData->DbToDramDqMap);

  // Select appropriate DB page to program DB pattern
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGE8);

  // DRAM sets LFSR1 on even bits, and LFSR0 on odd bits.  For the DB, we need to retrieve the mapping for
  // each DQ lane and ensure that each DQ is programmed to the same starting seed.  For example, if DB DQ2
  // maps to DRAM DQ1 then DB DQ2 needs to be programmed to LFSR0.
  for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    for (DataBuffer = 0; DataBuffer < DB_PER_SUBCH; DataBuffer++) {

      // Select appropriate data buffer device
      DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, DataBuffer);

      Mr30.Data = 0;
      for (DqNumber = 0; DqNumber < DB_DQ_LANE; DqNumber++) {
        Dword = (DqNumber + (DataBuffer * DB_DQ_LANE)) / 10;
        SwizzleValue = ((*DbToDramDqMap)[Channel][Dimm][Rank][SubChannel][Dword] >> (((DqNumber + (DataBuffer * DB_DQ_LANE)) % 10) * 3));
        SwizzleValue &= DqMask;

        // If the swizzle value for this DB DQ lane is even, then this lane needs to be programmed to LFSR1.  Otherwise
        // leave it as 0 (LFSR0)
        if (SwizzleValue % 2 == 0) {
          Mr30.Data |= (1 << DqNumber);
        }
      } // DqNumber loop

      // Set Read LFSR assignment for this data buffer
      DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE6_REG, Mr30.Data);
    } // DataBuffer loop
  }

  // Select all buffer devices
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);

  // Select appropriate DB page for given rank
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, Rank);

  return Status;
}

/**

  Initialize LRDIMM Per-Bit Read Delay Deskew training parameters.

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket number
  @param[in] ChBitmask  - Channel bit mask
  @param[in] Dimm       - Dimm number
  @param[in] Rank       - Rank number

  @retval N/A

**/
MRC_STATUS
LrdimmPerBitReadDelayInit (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  MRC_STATUS                    Status = MRC_STATUS_SUCCESS;
  UINT8                         MaxChDdr;
  UINT8                         Channel;
  DDR5_MODE_REGISTER_25_STRUCT  Mr25;
  DDR5_MODE_REGISTER_30_STRUCT  Mr30;
  DDR5_DATA_BUFFER_RW97_STRUCT  BufferTrainingConfig;

  MaxChDdr = GetMaxChDdr ();

  SetDataTrainCountWindow (Host, Socket, Dimm, Rank, MRD_COUNTING_WINDOW);

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
      continue;
    }
    if (IsRankValid (Socket, Channel, Dimm, Rank, FALSE) == FALSE) {
      continue;
    }

    if (FeaturePcdGet (PcdCteBuild) == TRUE) {
      //
      // Init DB and DRAM interface only for CTE. To add #ifdef YAM_ENV
      //
      InitDbDramInterface (Socket, Channel, Dimm, TWO_SUBCH_MASK, Rank);
    }

    // c. Configure the pattern generated by the DRAM (MR25 to MR30).
    // Set to LFSR mode
    Mr25.Data = 0;
    Mr25.Bits.read_training_pattern_format = 1;  // LFSR pattern format
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_25_REG, Mr25.Data);

    // Configure pattern to default read pattern
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_26_REG, MR26_DEFAULT_RD_TRAIN);
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_27_REG, MR27_DEFAULT_RD_TRAIN);

    // No pattern inversion
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_28_REG, 0);
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_29_REG, 0);

    // Set LFSR1 on even bits, and LFSR0 on odd bits
    Mr30.Data = 0;
    Mr30.Bits.lfsr_assignment_dql0_dqu0 = 1;  // LFSR1 on even bits
    Mr30.Bits.lfsr_assignment_dql1_dqu1 = 0;  // LFSR0 on odd bits
    Mr30.Bits.lfsr_assignment_dql2_dqu2 = 1;  // LFSR1 on even bits
    Mr30.Bits.lfsr_assignment_dql3_dqu3 = 0;  // LFSR0 on odd bits
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_30_REG, Mr30.Data);

    // d. Configure the pattern on DB
    Status = ProgramDbDeskewPattern (Host, Socket, Channel, Dimm, Rank);
    if (Status != MRC_STATUS_SUCCESS) {
      return Status;
    }

    // e. Set the DB Buffer training configuration (RW97) with per-DQ feedback enabled.
    BufferTrainingConfig.Data = 0;
    BufferTrainingConfig.Bits.clear_feedback_status_bit = 1;
    BufferTrainingConfig.Bits.per_bit_vs_per_transaction = 0; // Per-bit

    // f. Enable the MRD Training Mode on the DB.
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW83_REG, LRDIMM_RDDL_TRAINING);
  }
  return Status;
}

/**

  Initialize LRDIMM Per-Bit Read Delay Deskew training parameters.

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket number
  @param[in] ChBitmask  - Channel bit mask
  @param[in] Dimm       - Dimm number
  @param[in] Rank       - Rank number

  @retval Status - Non-zero if any failure detected

**/
VOID
LrdimmWriteDelayInit (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  UINT8                             MaxChDdr;
  UINT8                             Channel;
  DDR5_MODE_REGISTER_10_STRUCT      Mr10;
  DDR5_MODE_REGISTER_25_STRUCT      Mr25;
  DDR5_DATA_BUFFER_RW97_STRUCT      BufferTrainingConfig;
  DDR5_DATA_BUFFER_PGB_RWE0_STRUCT  DbPgbE0;
  DDR5_DATA_BUFFER_PGB_RWE1_STRUCT  DbPgbE1;
  DDR5_DATA_BUFFER_PGB_RWE2_STRUCT  DbPgbE2;
  DDR5_DATA_BUFFER_PGB_RWE3_STRUCT  DbPgbE3;
  struct channelNvram               (*ChannelNvList) [MAX_CH];

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  SetDataTrainCountWindow (Host, Socket, Dimm, Rank, MWD_COUNTING_WINDOW);

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
      continue;
    }
    if (IsRankValid (Socket, Channel, Dimm, Rank, FALSE) == FALSE) {
      continue;
    }

    if (FeaturePcdGet (PcdCteBuild) == TRUE) {
      //
      // Init DB and DRAM interface only for CTE
      //
      InitDbDramInterface (Socket, Channel, Dimm, TWO_SUBCH_MASK, Rank);
    }

    // a. Initialize DRAM
    // Program DB ODTLon/off to a conservative setting - DRAM programming is done in MemJedecDdr5.c called by EarlyCaClkBackside
    // Select appropriate DB page to program dynamic ODT on the DB
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGEB);
    DbPgbE0.Data = 0;
    DbPgbE0.Bits.odtlon_wr_n0_r1_offset_the_offset = INITIAL_ODTLON_WR_OFFSET;
    DbPgbE0.Bits.odtlon_wr_n1_r1_offset_the_offset = INITIAL_ODTLON_WR_OFFSET;
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PGB_RWE0_REG, DbPgbE0.Data);

    DbPgbE1.Data = 0;
    DbPgbE1.Bits.odtloff_wr_n0_r0_offset_the_offset = INITIAL_ODTLON_WR_OFFSET;
    DbPgbE1.Bits.odtloff_wr_n1_r0_offset_the_offset = INITIAL_ODTLON_WR_OFFSET;
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PGB_RWE1_REG, DbPgbE1.Data);

    DbPgbE2.Data = 0;
    DbPgbE2.Bits.odtloff_wr_n0_r1_offset_the_offset = INITIAL_ODTLOFF_WR_OFFSET;
    DbPgbE2.Bits.odtloff_wr_n1_r1_offset_the_offset = INITIAL_ODTLOFF_WR_OFFSET;
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PGB_RWE2_REG, DbPgbE2.Data);

    DbPgbE3.Data = 0;
    DbPgbE3.Bits.odtloff_wr_n0_r1_offset_the_offset = INITIAL_ODTLOFF_WR_OFFSET;
    DbPgbE3.Bits.odtloff_wr_n1_r1_offset_the_offset = INITIAL_ODTLOFF_WR_OFFSET;
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PGB_RWE3_REG, DbPgbE3.Data);

    // d. Configure the pattern generated by the DB (program snooped registers)
    // Select appropriate DB page to program DB pattern
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGE8);

    // MR25 is the DRAM mode register that the DB snoops. Use the structure to program RWE2 on the data buffer (LFSR mode)
    Mr25.Data = 0;
    Mr25.Bits.read_training_pattern_format = 1;
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE2_REG, Mr25.Data);

    // Program read pattern data0/data1
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE3_REG, MR26_DEFAULT_RD_TRAIN);
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE4_REG, MR27_DEFAULT_RD_TRAIN);

    // No pattern inverstion
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE5_REG, 0);
    // Set LFSR assignments
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE6_REG, LFSR_ASSIGNMENT);

    // e. Set the DB Buffer training configuration
    BufferTrainingConfig.Data = 0;
    BufferTrainingConfig.Bits.clear_feedback_status_bit = 1;
    BufferTrainingConfig.Bits.per_bit_vs_per_transaction = 0; // Per-bit
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW97_REG, BufferTrainingConfig.Data);

    // f. Enable MWD Training Mode
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW83_REG, LRDIMM_WRDL_TRAINING);

    // Select appropriate DB page based on rank being trained
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, Rank);

    // g. Set DRAM VREF via MR10
    Mr10.Data = 0;
    Mr10.Bits.vrefdq_cal_value = (UINT8) (*ChannelNvList)[Channel].txVrefSafe[Rank];
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_10_REG, Mr10.Data);
  }
}

/**

  Gets the results from the current test.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] ChBitmask        - Channel bit mask
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - rank number of the DIMM
  @param[in] DelaySetting     - Encoded Delay MDQS setting (per RWE4/E5 register definition in JEDEC Spec)
  @param[in][out] LrTrainRes  - Training result

  @retval N/A

**/
VOID
GetLrdimmMrdCoarseResults (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT32      ChBitmask,
  IN UINT8       Dimm,
  IN UINT8       Rank,
  IN UINT8       DelaySetting,
  IN OUT struct  TrainingResultsDdr5 (*LrTrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8   Channel;
  UINT8   SubChannel;
  UINT8   Strobe;
  UINT8   SwizzleStrobe;
  UINT8   DWord;
  UINT32  LrdimmFeedBack;
  UINT8   CodedDelayResult;
  UINT8   MaxChDdr;
  UINT8   SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  MaxChDdr = GetMaxChDdr ();

  CodedDelayResult = EncodeDecodeReadDelay (Encode, DelaySetting);

  //
  // Sweep has a range of 32 values. Each result is stored in 1 bit. So results for entire sweep
  // can be stored in 32 bits (or a single dword).
  //
  DWord = 0;

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    // Continue to the next rank if this one is disabled
    if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    for (SubChannel = 0 ; SubChannel < SUB_CH; SubChannel++) {
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        SwizzleStrobe = CheckBacksideSwizzle (Host, Socket, Channel, Dimm, Strobe);
        LrdimmFeedBack = ReadDataTrainCounterFeedbackDdr5 (Host, Socket, Channel, SubChannel, SwizzleStrobe);

        // If greater than half the total samples, consider the result a '1'
        if (LrdimmFeedBack > MRD_COUNTING_WINDOW / 2) {
          (*LrTrainRes)[Channel][SubChannel][Strobe].results[DWord] =
              (*LrTrainRes)[Channel][SubChannel][Strobe].results[DWord] | (0x1 << CodedDelayResult);
        }
      } // Strobe loop
    } // SubChannel loop
  } // Channel loop
} // GetLrdimmMrdCoarseResults

/**

  Returns the value to be programmed in the MDQS Read Delay Control Word (0xE4/E5) based on
  the desired delay (-15 to 15).

  @param[in] MdqsReadDelay - read delay value to be programmed

  @retval UINT8 - register value based on desired read delay

**/
UINT8
GetReadDelayRegValue (
  INT8  MdqsReadDelay
  )
{
  UINT8  ReadDelayRegValue;

  // RWE4/E5 is programmed as the delay value for positive numbers.
  // RWE4/E5 is programmed as the delay value with BIT4 set for negative numbers.
  if (MdqsReadDelay < 0) {
    ReadDelayRegValue = (UINT8) (ABS (MdqsReadDelay) | BIT4);
  } else {
    ReadDelayRegValue = (UINT8) MdqsReadDelay;
  }

  return ReadDelayRegValue;
}

/**

  Returns the value to be programmed in the MDQ Read Delay Control Word (0xEA - 0xED) or the
  MDQ Write Delay Control Word (0xEE - 0xF1) based on the desired delay (-3 to 3).

  @param[in] MdqDelay - delay value to be programmed

  @retval UINT8 - register value based on desired read delay

**/
UINT8
GetPerBitDelayRegValue (
  INT8  MdqDelay
  )
{
  UINT8  PerBitDelayRegValue;

  // RWEA - RWED is programmed as the read delay value for positive numbers
  // RWEE - RWF1 is programmed as the write delay value for positive numbers
  // RWEA - RWED is programmed as the read delay value with BIT2 set for negative numbers
  // RWEE - RWF1 is programmed as the write delay value with BIT2 set for negative numbers
  if (MdqDelay < 0) {
    PerBitDelayRegValue = (UINT8) (ABS (MdqDelay) | BIT2);
  } else {
    PerBitDelayRegValue = (UINT8) MdqDelay;
  }

  return PerBitDelayRegValue;
}

/**

  Gets the results from the backside swizzle test.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - DIMM number
  @param[in] ChBitmask        - Channel bit mask
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number of the DIMM
  @param[in] Strobe           - Strobe number
  @param[in] DramDqBitIndex   - Current DQ lane of the DRAM
  @param[in] DbDqBitIndex     - Current DQ lane of the DB
  @param[in] MappingFound     - Indicates if mapping has been found for current DRAM DQ lane
  @param[in][out] DbToDramDqMap  - Array to store DQ mapping

  @retval N/A

**/
VOID
GetBacksideSwizzleSweepResults (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Strobe,
  IN UINT8     DramDqBitIndex,
  IN UINT8     DbDqBitIndex,
  IN BOOLEAN   MappingFound[MAX_CH][SUB_CH],
  OUT UINT32   (*DbToDramDqMap)[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][SUB_CH][SWIZZLE_RESULTS_LEN]
  )
{
  UINT8                Channel;
  UINT8                SubChannel;
  UINT8                MaxChDdr;
  UINT8                SwizzleStrobe;
  UINT32               LrdimmFeedBack;
  UINT8                DataBuffer;
  UINT8                DbDqLane;
  struct channelNvram  (*ChannelNvList) [MAX_CH];
  struct ddrRank       (*RankList) [MAX_RANK_DIMM];
  UINT8                Dword;
  UINT8                MaxStrobePairs = GetSubChMaxStrobeValid (Host) / 2;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  // Determine appropriate data buffer number and data buffer DQ lane based on inputs
  DataBuffer = Strobe;
  if (Strobe >= MaxStrobePairs) {
    DbDqLane = DbDqBitIndex + BITS_PER_NIBBLE;
    DataBuffer = Strobe - MaxStrobePairs;
  } else {
    DbDqLane = DbDqBitIndex;
    DataBuffer = Strobe;
  }
  // Each Dword can fit 10 entries (3 bits per entry) - find appropriate Dword to store data
  Dword = (DbDqLane + (DataBuffer * DB_DQ_LANE)) / 10;

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }

    // Continue to the next rank if this one is disabled
    if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    for (SubChannel = 0 ; SubChannel < SUB_CH; SubChannel++) {
      SwizzleStrobe = CheckBacksideSwizzle (Host, Socket, Channel, Dimm, Strobe);
      LrdimmFeedBack = ReadDataTrainCounterFeedbackDdr5 (Host, Socket, Channel, SubChannel, SwizzleStrobe);


      // If greater than half the total samples, consider the result a '1'
      if (LrdimmFeedBack > SWZ_COUNTING_WINDOW / 2) {
        // If mapping already found for the DRAM DQ lane, throw error
        if (MappingFound[Channel][SubChannel]) {

          // Flag error if more than one DQ Lane mapping found
          Host->nvram.mem.socket[Socket].FaultyPartsFlag[Channel] |= ~0;
          RankList = GetRankNvList (Host, Socket, Channel, Dimm);
          (*RankList) [Rank].faultyParts[0] |= FPT_TRAINING_FAILED;
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
              "More than one DQ Lane = 1: SubCh%d DB:%d DbDqLane:%d DramDqLane%d!\n", SubChannel, DataBuffer, DbDqLane, DramDqBitIndex);
          EwlOutputType10 (WARN_DQ_SWIZZLE_DISC, WARN_DQ_SWIZZLE_DISC_UNCORR, Socket, Channel, NO_DIMM, NO_RANK, EwlSeverityWarning, DbDqBitIndex, DramDqBitIndex, 8, Strobe);
          DisableChannelSw (Host, Socket, Channel);
        } else {
          MappingFound[Channel][SubChannel] = TRUE;
          (*DbToDramDqMap)[Channel][Dimm][Rank][SubChannel][Dword] |= DramDqBitIndex << (((DbDqLane + (DataBuffer * DB_DQ_LANE)) % 10) * 3);
        }
      }
    } // SubChannel loop
  } // Channel loop
}

/**

  Gets the results from the frontside swizzle test.

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - DIMM number
  @param[in] ChBitmask      - Channel bit mask
  @param[in] Dimm           - DIMM number
  @param[in] Rank           - Rank number of the DIMM
  @param[in] DqLane         - Current DQ lane of the DB (to Host)
  @param[in] MaskLane       - Current DQ lane to get results for
  @param[out] SweepResults  - Array to store results from sweep

  @retval N/A

**/
VOID
GetFrontsideSwizzleSweepResults (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     DqLane,
  IN UINT8     MaskLane,
  OUT UINT8    SweepResults [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8                Channel;
  UINT8                MaxChDdr;
  UINT8                SubChannel;
  UINT8                Strobe;
  UINT8                SwizzleStrobe;
  UINT32               LrdimmFeedBack;
  UINT8                MaxSubChStrobes = GetSubChMaxStrobeValid (Host);

  MaxChDdr = GetMaxChDdr ();

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }

    // Continue to the next rank if this one is disabled
    if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    for (SubChannel = 0 ; SubChannel < SUB_CH; SubChannel++) {
      for (Strobe = 0; Strobe < MaxSubChStrobes; Strobe++) {

        // Retrieve feedback for lower and upper nibble
        SwizzleStrobe = CheckBacksideSwizzle (Host, Socket, Channel, Dimm, Strobe);
        LrdimmFeedBack = ReadDataTrainCounterFeedbackDdr5 (Host, Socket, Channel, SubChannel, SwizzleStrobe);


        // If counter value is greater than half of the total samples, consider the result a '1'
        if (LrdimmFeedBack >= SWZ_COUNTING_WINDOW / 2) {
          SweepResults [Channel][SubChannel][Strobe] |= (1 << MaskLane);
        }

      } // Strobe loop
    } // SubChannel loop
  } // Channel loop
}

/**

  Analyzes the results from the frontside swizzle test and saves swizzle results
  to HostToDbDqMap.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - DIMM number
  @param[in] ChBitmask        - Channel bit mask
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number of the DIMM
  @param[in] DqLane           - Current DQ lane of the DB (to Host)
  @param[in] SweepResults     - Array which holds sweep results
  @param[out] HostToDbDqMap   - Array to store DQ mapping

  @retval N/A

**/
VOID
AnalyzeFrontsideSwizzleResults (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     DbDqLane,
  IN UINT8     SweepResults [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  OUT UINT32   (*HostToDbDqMap) [MAX_CH][MAX_DIMM][SUB_CH][SWIZZLE_RESULTS_LEN]
  )
{
  UINT8                Channel;
  UINT8                MaxChDdr;
  UINT8                SubChannel;
  UINT8                Strobe;
  struct ddrRank       (*RankList) [MAX_RANK_DIMM];
  UINT8                Dword;
  UINT8                DataBuffer;
  UINT8                HostDq;
  BOOLEAN              MappingFound;
  UINT8                MaxStrobePairs = GetSubChMaxStrobeValid (Host) / 2;

  MaxChDdr = GetMaxChDdr ();

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }

    // Continue to the next rank if this one is disabled
    if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    for (SubChannel = 0 ; SubChannel < SUB_CH; SubChannel++) {
      for (Strobe = 0; Strobe < MaxStrobePairs; Strobe++) {
        // Indicate mapping has not been found for this data buffer
        MappingFound = FALSE;
        DataBuffer = Strobe;

        // Combine Sweep results for lower nibble and upper nibble into a single array (will use the
        // lower nibble results for storage)
        SweepResults[Channel][SubChannel][Strobe] |= (SweepResults[Channel][SubChannel][Strobe + MaxStrobePairs] << BITS_PER_NIBBLE);

        // Each Dword can fit 10 entries (3 bits per entry) - find appropriate Dword to store data
        Dword = (DbDqLane + (DataBuffer * DB_DQ_LANE)) / 10;


        for (HostDq = 0; HostDq < DB_DQ_LANE; HostDq++) {
          // A '0' sample represents a comparison failure and it means the mapping has been found
          if ((SweepResults[Channel][SubChannel][Strobe] & (1 << HostDq)) == 0) {
            if (MappingFound == FALSE) {
              // Indicate we've found a mapping for this DQ lane
              MappingFound = TRUE;
              (*HostToDbDqMap)[Channel][Dimm][SubChannel][Dword] |= HostDq << (((DbDqLane + (DataBuffer * DB_DQ_LANE)) % 10) * 3);

            } else {
              // Mapping for this DQ lane has already been recorded - error
              Host->nvram.mem.socket[Socket].FaultyPartsFlag[Channel] |= ~0;
              RankList = GetRankNvList (Host, Socket, Channel, Dimm);
              (*RankList) [Rank].faultyParts[0] |= FPT_TRAINING_FAILED;
              RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "More than one DQ Lane = 1: SubCh%d DB:%d DbDqLane:%d HostDqLane%d!\n", SubChannel, DataBuffer, DbDqLane, HostDq);
              EwlOutputType10 (WARN_DQ_SWIZZLE_DISC, WARN_DQ_SWIZZLE_DISC_UNCORR, Socket, Channel, Dimm, Rank,
                  EwlSeverityWarning, MR26_SWIZZLE_PATTERN, HostDq, BITS_PER_NIBBLE, Strobe);
              DisableChannelSw (Host, Socket, Channel);
            }
          }
        }

        // Make sure we've found a mapping for this lane.  If not, error.
        if (MappingFound == FALSE) {
          Host->nvram.mem.socket[Socket].FaultyPartsFlag[Channel] |= ~0;
          RankList = GetRankNvList (Host, Socket, Channel, Dimm);
          (*RankList) [Rank].faultyParts[0] |= FPT_TRAINING_FAILED;
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "No DQ lanes = 1! SubCh%d Strobe:%d DbDqLane%d!\n", SubChannel, Strobe, DbDqLane);
          EwlOutputType10 (WARN_DQ_SWIZZLE_DISC, WARN_DQ_SWIZZLE_DISC_UNCORR, Socket, Channel, Dimm, Rank,
              EwlSeverityWarning, MR26_SWIZZLE_PATTERN, HostDq, BITS_PER_NIBBLE, Strobe);
          DisableChannelSw (Host, Socket, Channel);
        }
      } // Strobe loop
    } // SubChannel loop
  } // Channel loop
}

/**

  Gets the results from Per-Bit MDQS Read Delay Deskew test.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] ChBitmask        - Channel bit mask
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - rank number of the DIMM
  @param[in] MaskLane         - The DQ lane for which the training counter will have results
  @param[in] DelaySetting     - Encoded Delay MDQS setting (per RWE4/E5 register definition in JEDEC Spec)
  @param[in][out] LrTrainRes  - Training result

  @retval N/A

**/
VOID
GetLrdimmMrdPerBitResults (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT32      ChBitmask,
  IN UINT8       Dimm,
  IN UINT8       Rank,
  IN UINT8       MaskLane,
  IN UINT8       DelaySetting,
  IN OUT struct  MdqTrainingResults (*LrTrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8   Channel;
  UINT8   SubChannel;
  UINT8   Strobe;
  UINT8   SwizzleStrobe;
  UINT8   DWord;
  UINT32  LrdimmFeedBack;
  UINT8   CodedDelayResult;
  UINT8   MaxChDdr;
  UINT8   SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  MaxChDdr = GetMaxChDdr ();

  CodedDelayResult = EncodeDecodeReadDelay (Encode, DelaySetting);

  //
  // Putting the results per nibble in a 128bit structure, so every 4 bits = 1 entry
  // and 32 / 4 = 8, thus divide by 8 to get the proper dword for this entry
  //
  DWord = (UINT8) (CodedDelayResult / 8);

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }

    // Continue to the next rank if this one is disabled
    if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    for (SubChannel = 0 ; SubChannel < SUB_CH; SubChannel++) {
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        SwizzleStrobe = CheckBacksideSwizzle (Host, Socket, Channel, Dimm, Strobe);
        LrdimmFeedBack = ReadDataTrainCounterFeedbackDdr5 (Host, Socket, Channel, SubChannel, SwizzleStrobe);


        // If counter value is greater than half of the total samples, consider the result a '1'
        if (LrdimmFeedBack >= MRD_COUNTING_WINDOW / 2) {
          (*LrTrainRes)[Channel][SubChannel][Strobe].Results[DWord] |= ((1 << MaskLane) << ((CodedDelayResult % 8) * 4));
        }
      } // Strobe loop
    } // SubChannel loop
  } // Channel loop
} // GetLrdimmMrdCoarseResults

/**

  Gets the results from the write delay test.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] ChBitmask        - Channel bit mask
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - rank number of the DIMM
  @param[in] MaskLane         - The DQ lane for which the training counter will have results
  @param[in] DelaySetting     - Encoded Delay MDQS setting (per RWE4/E5 register definition in JEDEC Spec)
  @param[in][out] LrTrainRes  - Training result

  @retval N/A

**/
VOID
GetLrdimmMwdResults (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT32      ChBitmask,
  IN UINT8       Dimm,
  IN UINT8       Rank,
  IN UINT8       MaskLane,
  IN UINT16      DelaySetting,
  IN OUT struct  MdqTrainingResults (*LrTrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8   Channel;
  UINT8   SubChannel;
  UINT8   Strobe;
  UINT8   SwizzleStrobe;
  UINT8   DWord;
  UINT32  LrdimmFeedBack;
  UINT8   MaxChDdr;
  UINT8   SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  MaxChDdr = GetMaxChDdr ();

  //
  // Putting the results per nibble in a 1024bit structure. Every 4 bits = 1 entry
  // and 32 / 4 = 8, thus divide by 8 to get the proper dword for this entry
  //
  DWord = (UINT8) (DelaySetting / 8);

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }

    // Continue to the next rank if this one is disabled
    if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    for (SubChannel = 0 ; SubChannel < SUB_CH; SubChannel++) {

      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        // Skip if this is an ECC Strobe when ECC is disabled
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        SwizzleStrobe = CheckBacksideSwizzle (Host, Socket, Channel, Dimm, Strobe);
        LrdimmFeedBack = ReadDataTrainCounterFeedbackDdr5 (Host, Socket, Channel, SubChannel, SwizzleStrobe);


        // If counter value is greater than half of the total samples, consider the result a '1'
        if (LrdimmFeedBack >= MWD_COUNTING_WINDOW / 2) {
          (*LrTrainRes)[Channel][SubChannel][Strobe].Results[DWord] |= ((1 << MaskLane) << ((DelaySetting % 8) * 4));
        }
      } // Strobe loop
    } // SubChannel loop
  } // Channel loop
}

/**

  Sweeps the upper and lower nibble read delay settings from -15 to +15 for DDR5 LRDIMM.

  @param[in] Host             - Pointer to sysHost structure
  @param[in] Socket           - Socket number
  @param[in] ChBitmask        - Channel bit mask
  @param[in] Dimm             - Dimm number
  @param[in] Rank             - Rank number
  @param[in][out] LrTrainRes  - Training result

  @retval N/A

**/
VOID
SweepReadDelay (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT32      ChBitmask,
  IN UINT8       Dimm,
  IN UINT8       Rank,
  IN OUT struct  TrainingResultsDdr5 (*LrTrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8                             Channel;
  UINT8                             MaxChDdr;
  INT8                              MdqsReadDelay = 0;
  UINT8                             ReadDelayRegData;
  DDR5_DATA_BUFFER_PG0_RWE4_STRUCT  MdqsDelayControl;
  DDR5_DATA_BUFFER_RW97_STRUCT      BufferTrainingConfig;

  MaxChDdr = GetMaxChDdr ();
  MdqsDelayControl.Data = 0;

  for (MdqsReadDelay = LRDIMM_BACKSIDE_READ_DELAY_START; MdqsReadDelay < LRDIMM_BACKSIDE_READ_DELAY_END; MdqsReadDelay += LRDIMM_BACKSIDE_READ_DELAY_STEP_SIZE) {

    ReadDelayRegData = GetReadDelayRegValue (MdqsReadDelay);
    MdqsDelayControl.Bits.lower_nibble_mdqs_delay_control_during_read = ReadDelayRegData;

    for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
      if (!((1 << Channel) & ChBitmask)) {
        continue;
      }
      // i. Set the Clear feedback status bit in RW97 to clear errors from previous run
      BufferTrainingConfig.Data = 0;
      BufferTrainingConfig.Bits.clear_feedback_status_bit = 1;
      BufferTrainingConfig.Bits.per_bit_vs_per_transaction = 1;
      DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW97_REG, BufferTrainingConfig.Data);

      // Program the lower and upper nibble MDQS Read Delay to same value for sweep
      DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE4_REG, MdqsDelayControl.Data);
      DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE5_REG, MdqsDelayControl.Data);
    } // Channel loop

    // ii. Send the read pattern
    ExecuteBacksideReadTest (Host, Socket, ChBitmask, Dimm, Rank, MdqsCoarseReadDelayTraining);

    // iii. Read the result from the sense amp sampler
    GetLrdimmMrdCoarseResults (Host, Socket, ChBitmask, Dimm, Rank, ReadDelayRegData, LrTrainRes);
  } // end read delay sweep
}

/**

  Sweeps the upper and lower nibble write delay settings from 0 to 255 for DDR5 LRDIMM.

  @param[in] Host             - Pointer to sysHost structure
  @param[in] Socket           - Socket number
  @param[in] ChBitmask        - Channel bit mask
  @param[in] Dimm             - Dimm number
  @param[in] Rank             - Rank number
  @param[in][out] LrTrainRes  - Training resultss

  @retval N/A

**/
VOID
SweepWriteDelay (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT32      ChBitmask,
  IN UINT8       Dimm,
  IN UINT8       Rank,
  IN OUT struct  MdqTrainingResults (*LrTrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8                             Channel;
  UINT8                             MaxChDdr;
  UINT16                            WriteDelay = 0;
  UINT8                             MaskLane;
  DDR5_DATA_BUFFER_PG0_RWE6_STRUCT  WriteDelayControl;
  DDR5_DATA_BUFFER_RW97_STRUCT      BufferTrainingConfig;

  MaxChDdr = GetMaxChDdr ();
  WriteDelayControl.Data = 0;

  // h. Sweep MDQ phase with respect to MDQS delay from 0 to 255
  for (WriteDelay = DDR5_LRDIMM_BACKSIDE_WRITE_DELAY_START; WriteDelay < DDR5_LRDIMM_BACKSIDE_WRITE_DELAY_END; WriteDelay += LRDIMM_BACKSIDE_WRITE_DELAY_STEP_SIZE) {

    WriteDelayControl.Bits.phase_control_for_lower_nibble_mdq_with = (UINT8) WriteDelay;

    for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
      if (!((1 << Channel) & ChBitmask)) {
        continue;
      }
      // i. Set the Clear feedback status bit in RW97 to clear errors from previous run
      BufferTrainingConfig.Data = 0;
      BufferTrainingConfig.Bits.clear_feedback_status_bit = 1;
      BufferTrainingConfig.Bits.per_bit_vs_per_transaction = 0;
      DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW97_REG, BufferTrainingConfig.Data);

      // Program the lower and upper nibble MDQS Read Delay to same value for sweep
      DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE6_REG, WriteDelayControl.Data);
      DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE7_REG, WriteDelayControl.Data);
    } // Channel loop

    // i. Send the write-read pattern
    ExecuteBacksideWriteReadTest (Host, Socket, ChBitmask, Dimm, Rank, MdqsWriteDelay);

    // Disable write leveling mode so cmd_train_ctr_en can be used to collect feedback
    SetWlTrainingMode (Socket, FALSE);

    // Using DDRIO counter to retrieve feedback - in order to get per-bit results, we must set the dq_sample_mask
    // to get the results for a single DQ lane.
    for (MaskLane = 0; MaskLane < BITS_PER_NIBBLE; MaskLane++) {

      SetCounterDqSampleMask (Socket, ChBitmask, MaskLane);

      // Set cmd_train_ctr_en to capture feedback
      EnableDdrioTrainingCounter (Host, Socket, Dimm, Rank);

      // ii. Read the result from DDRIO counter
      GetLrdimmMwdResults (Host, Socket, ChBitmask, Dimm, Rank, MaskLane, WriteDelay, LrTrainRes);
    }

    // Re-enable write leveling mode before next test is executed
    SetWlTrainingMode (Socket, TRUE);
  }
}

/**

  Conducts a pattern sweep for frontside swizzle discovery for DDR5 LRDIMM.

  @param[in]  Host           - Pointer to sysHost structure
  @param[in]  Socket         - Socket number
  @param[in]  ChBitmask      - Channel bit mask
  @param[in]  Dimm           - Dimm number
  @param[in]  Rank           - Rank number
  @param[out] HostToDbDqMap  - Array containing Host to Data Buffer DQ map

  @retval N/A

**/
VOID
FrontsideSwizzleSweep (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT32      ChBitmask,
  IN UINT8       Dimm,
  IN UINT8       Rank,
  OUT UINT32     (*HostToDbDqMap)[MAX_CH][MAX_DIMM][SUB_CH][SWIZZLE_RESULTS_LEN]
  )
{
  UINT8                         Channel;
  UINT8                         MaxChDdr;
  UINT8                         DqLane;
  UINT8                         NumDqLane = DB_DQ_LANE;
  UINT8                         MaskLane;
  DDR5_MODE_REGISTER_28_STRUCT  DbMr28;
  DDR5_DATA_BUFFER_RW97_STRUCT  BufferTrainingConfig;
  UINT8                         SweepResults [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];

  MaxChDdr = GetMaxChDdr ();
  DbMr28.Data = 0;

  for (DqLane = 0; DqLane < NumDqLane; DqLane++) {

    for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
      if (!((1 << Channel) & ChBitmask)) {
        continue;
      }
      // 1. Configure the DB Pattern inversion for the DQ indicated by index
      DbMr28.Bits.dq_invert_lower_dq_bits = (1 << DqLane);
      DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE5_REG, DbMr28.Data);

    } // Channel loop

    ZeroMem ((UINT8 *) SweepResults, sizeof (SweepResults));
    // Using DDRIO counter to retrieve feedback - in order to get per-bit results, we must set the dq_sample_mask
    // to get the results for a single DQ lane.
    for (MaskLane = 0; MaskLane < BITS_PER_NIBBLE; MaskLane++) {
      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }
        // 2. Set the "Clear feedback status bit" (RW97[0]=1 to clear errors from previous run)
        BufferTrainingConfig.Data = 0;
        BufferTrainingConfig.Bits.clear_feedback_status_bit = 1;
        BufferTrainingConfig.Bits.per_bit_vs_per_transaction = 0;
        DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW97_REG, BufferTrainingConfig.Data);
      }

      SetCounterDqSampleMask (Socket, ChBitmask, MaskLane);

      // 3. Send the reads pattern (execute CPGC sequence)
      ExecuteBacksideReadTest (Host, Socket, ChBitmask, Dimm, Rank, FrontsideDqSwizzleDiscovery);

      // 4. Read per-bit result from sense amp sampler
      GetFrontsideSwizzleSweepResults (Host, Socket, ChBitmask, Dimm, Rank, DqLane, MaskLane, SweepResults);

    }

    AnalyzeFrontsideSwizzleResults (Host, Socket, ChBitmask, Dimm, Rank, DqLane, SweepResults, HostToDbDqMap);
  }
}

/**

  Conducts a pattern sweep for Per-Bit Read Delay Deskew.

  @param[in] Host             - Pointer to sysHost structure
  @param[in] Socket           - Socket number
  @param[in] ChBitmask        - Channel bit mask
  @param[in] Dimm             - Dimm number
  @param[in] Rank             - Rank number
  @param[in][out] LrTrainRes  - Training results

  @retval N/A

**/
VOID
PerBitDeskewSweep (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT32      ChBitmask,
  IN UINT8       Dimm,
  IN UINT8       Rank,
  IN OUT struct  MdqTrainingResults (*LrTrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8                             Channel;
  UINT8                             MaxChDdr;
  INT8                              MdqsReadDelay = 0;
  UINT8                             ReadDelayRegData;
  UINT8                             MaskLane;
  DDR5_DATA_BUFFER_PG0_RWE4_STRUCT  MdqsDelayControl;
  DDR5_DATA_BUFFER_RW97_STRUCT      BufferTrainingConfig;

  MaxChDdr = GetMaxChDdr ();
  MdqsDelayControl.Data = 0;

  for (MdqsReadDelay = LRDIMM_BACKSIDE_READ_DELAY_START; MdqsReadDelay < LRDIMM_BACKSIDE_READ_DELAY_END; MdqsReadDelay += LRDIMM_BACKSIDE_READ_DELAY_STEP_SIZE) {

    ReadDelayRegData = GetReadDelayRegValue (MdqsReadDelay);
    MdqsDelayControl.Bits.lower_nibble_mdqs_delay_control_during_read = ReadDelayRegData;

    for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
      if (!((1 << Channel) & ChBitmask)) {
        continue;
      }

      // Program the lower and upper nibble MDQS Read Delay to same value for sweep
      DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE4_REG, MdqsDelayControl.Data);
      DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE5_REG, MdqsDelayControl.Data);
    } // Channel loop

    // Using DDRIO counter to retrieve feedback - in order to get per-bit results, we must set the dq_sample_mask
    // to get the results for a single DQ lane.
    for (MaskLane = 0; MaskLane < BITS_PER_NIBBLE; MaskLane++) {
      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }

        // i. Set the Clear feedback status bit in RW97 to clear errors from previous run
        BufferTrainingConfig.Data = 0;
        BufferTrainingConfig.Bits.clear_feedback_status_bit = 1;
        BufferTrainingConfig.Bits.per_bit_vs_per_transaction = 0;
        DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW97_REG, BufferTrainingConfig.Data);
      }

      SetCounterDqSampleMask (Socket, ChBitmask, MaskLane);

      // ii. Send the read pattern
      ExecuteBacksideReadTest (Host, Socket, ChBitmask, Dimm, Rank, MdqsReadDelayDeskew);

      // iii. Read the result from the DDRIO counter
      GetLrdimmMrdPerBitResults (Host, Socket, ChBitmask, Dimm, Rank, MaskLane, ReadDelayRegData, LrTrainRes);
    }
  }
}

/**

  Make sure a mapping was found to the Data Buffer DQ for a given DRAM DQ lane.

  @param[in] Host            - Pointer to sysHost structure
  @param[in] Socket          - Socket
  @param[in] ChBitmask       - Channel bit mask
  @param[in] Dimm            - Dimm number
  @param[in] Rank            - Rank number
  @param[in] Strobe          - Strobe number
  @param[in] DramDqBitIndex  - DQ lane of the DRAM device
  @param[in] MappingFound    - Array indicating whether the mapping has been found

**/
VOID
CheckForDqMapping (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Strobe,
  IN UINT8     DramDqBitIndex,
  IN BOOLEAN   MappingFound[MAX_CH][SUB_CH]
  )
{
  UINT8           Channel;
  UINT8           SubChannel;
  UINT8           MaxChDdr;
  struct ddrRank  (*RankList) [MAX_RANK_DIMM];

  MaxChDdr = GetMaxChDdr ();

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      if (MappingFound[Channel][SubChannel] == FALSE) {
        //
        // Flag error if no DQ Lane = 1 per byte
        //
        Host->nvram.mem.socket[Socket].FaultyPartsFlag[Channel] |= ~0;
        RankList = GetRankNvList (Host, Socket, Channel, Dimm);
        (*RankList) [Rank].faultyParts[0] |= FPT_TRAINING_FAILED;
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "No DQ lanes = 1! SubCh%d Strobe:%d DramDqLane%d!\n", SubChannel, Strobe, DramDqBitIndex);
        EwlOutputType10 (WARN_DQ_SWIZZLE_DISC, WARN_DQ_SWIZZLE_DISC_UNCORR, Socket, Channel, NO_DIMM, NO_RANK, EwlSeverityWarning, 0, DramDqBitIndex, 8, Strobe);
        DisableChannelSw (Host, Socket, Channel);
      }
    } // SubChannel loop
  } // Channel loop
}

/**

  Conducts a pattern sweep for backside swizzle discovery for DDR5 LRDIMM.

  @param[in] Host           - Pointer to sysHost structure
  @param[in] Socket         - Socket number
  @param[in] ChBitmask      - Channel bit mask
  @param[in] Dimm           - Dimm number
  @param[in] Rank           - Rank number
  @param[in] DbToDramDqMap  - Array containing Data Buffer to DRAM DQ map
  @param[in] NibbleSwap     - Array containing Nibble Swap information discovered during MRE

  @retval NA

**/
VOID
BacksideSwizzleSweep (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT32      ChBitmask,
  IN UINT8       Dimm,
  IN UINT8       Rank,
  IN UINT32      (*DbToDramDqMap)[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][SUB_CH][SWIZZLE_RESULTS_LEN],
  IN BOOLEAN     (*NibbleSwap)[MAX_CH][MAX_DIMM][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8                         Channel;
  UINT8                         SubChannel;
  UINT8                         MaxChDdr;
  UINT8                         Strobe;
  UINT8                         DramDqBitIndex;
  UINT8                         DbDqBitIndex;
  UINT8                         SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8                         MaxStrobePairs = GetSubChMaxStrobeValid (Host) / 2;
  BOOLEAN                       MappingFound[MAX_CH][SUB_CH];
  DDR5_MODE_REGISTER_28_STRUCT  DramMr28;
  DDR5_MODE_REGISTER_28_STRUCT  DbMr28;
  DDR5_DATA_BUFFER_RW97_STRUCT  BufferTrainingConfig;

  MaxChDdr = GetMaxChDdr ();

  // For x4 device, strobe 0-4 represent lower nibble, 5-9 upper nibble
  for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
    for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
      if (!((1 << Channel) & ChBitmask)) {
        continue;
      }
      // Skip PDA sequence for DDRT 
      if (IsDcpmmPresentDimm(Socket, Channel, Dimm) == TRUE) {
        continue;
      }
      // Use PDA to target low nibble or high nibble per Strobe index
      EnterRcdCaPassThroughMode (Host, Socket, Channel, Dimm, Rank);
      WriteMPC (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, Rank, PDA_SELECT_ID | Strobe, TIMING_4N, TRUE);
      ExitRcdCaPassThroughMode (Host, Socket, Channel, Dimm, Rank);
    } // Channel loop

    // a. For DramDqBitIndex = 0 to 3 in steps of 1, Generate DRAM pattern by setting appropriate invert bit on DRAM
    // i. Configure DRAM Pattern inversion:
    for (DramDqBitIndex = 0; DramDqBitIndex < BITS_PER_NIBBLE; DramDqBitIndex++) {
      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }
        // Init mapping found indicator to false for every new DRAM DQ bit index
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          MappingFound[Channel][SubChannel] = FALSE;
        }
        DramMr28.Data = 0;
        DramMr28.Bits.dq_invert_lower_dq_bits = (1 << DramDqBitIndex);

        DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_28_REG, DramMr28.Data);
      } // Channel loop

      // ii. For index2 = 0 to 3 in steps of 1, Generate DB pattern by setting appropriate invert bit on DB
      //  1.  Configure DB Pattern inversion for the DQ indicated by index2
      for (DbDqBitIndex = 0; DbDqBitIndex < BITS_PER_NIBBLE; DbDqBitIndex++) {
        for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
          if (!((1 << Channel) & ChBitmask)) {
            continue;
          }

          // Program invert bits on DB taking into account Nibble Swap
          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
            DbMr28.Data = 0;

            // If no Nibble Swap, do not invert DbDq Invert programming
            if ((*NibbleSwap)[Channel][Dimm][SubChannel][Strobe] == FALSE) {

              // If low nibble -> inversion target bit: DQ(DbDqBitIndex)
              if (Strobe < MaxStrobePairs) {
                DbMr28.Bits.dq_invert_lower_dq_bits = (1 << DbDqBitIndex);
              } else {
                // else high nibble -> inversion target bit: DQ(DbDqBitIndex + 4)
                DbMr28.Bits.dq_invert_lower_dq_bits = (1 << (DbDqBitIndex + BITS_PER_NIBBLE));
              }

            } else {  // Nibble swap - invert low and high programming

              if (Strobe < MaxStrobePairs) {
                DbMr28.Bits.dq_invert_lower_dq_bits = (1 << (DbDqBitIndex + BITS_PER_NIBBLE));
              } else {
                DbMr28.Bits.dq_invert_lower_dq_bits = (1 << DbDqBitIndex);
              }
            }
            DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE5_REG, DbMr28.Data);
          }

          // 2. Set the "Clear feedback status bit" (RW97[0]=1 to clear errors from previous run)
          BufferTrainingConfig.Data = 0;
          BufferTrainingConfig.Bits.clear_feedback_status_bit = 1;
          BufferTrainingConfig.Bits.per_bit_vs_per_transaction = 1;
          DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW97_REG, BufferTrainingConfig.Data);
        } // Channel loop

        // 3. Send the reads pattern (execute CPGC sequence).
        ExecuteBacksideReadTest (Host, Socket, ChBitmask, Dimm, Rank, BacksideDqSwizzleDiscovery);

        // 4. Read the result from the DDRIO counter
        GetBacksideSwizzleSweepResults (Host, Socket, ChBitmask, Dimm, Rank, Strobe, DramDqBitIndex, DbDqBitIndex, MappingFound, DbToDramDqMap);

      } // DbDqBit loop

      // We've swept every DQ lane for the data buffer -- make sure we've found a mapping for this DRAM DQ lane
      CheckForDqMapping (Host, Socket, ChBitmask, Dimm, Rank, Strobe, DramDqBitIndex, MappingFound);
    } // DramDqBit loop

    // Clear invert from previous sweep before moving to next DRAM device
    for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }
        DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_28_REG, 0);
    }
  } // Strobe loop
  // Select all DRAM devices
  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }
    // Skip PDA sequence for DDRT 
    if (IsDcpmmPresentDimm(Socket, Channel, Dimm) == TRUE) {
      continue;
    }
    EnterRcdCaPassThroughMode (Host, Socket, Channel, Dimm, Rank);
    WriteMPC (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, Rank, PDA_SELECT_ID | PDA_SELECT_ALL, TIMING_4N, TRUE);
    ExitRcdCaPassThroughMode (Host, Socket, Channel, Dimm, Rank);
  }
}

/**

  This function prints the Coarse Delay sample array

  @param[in] Host          - Pointer to sysHost
  @param[in] Socket        - Socket number
  @param[in] Channel       - Channel number
  @param[in] Dimm          - DIMM number
  @param[in] SubChannel    - SubChannel number
  @param[in] Dimm          - DIMM number
  @param[in] Rank          - Rank number on DIMM
  @param[in] TrainResults  - Pointer to struct containing DDR5 training results

  @retval N/A

**/
VOID
DisplayCoarseReadDelaySweepResults (
  IN PSYSHOST                    Host,
  IN UINT8                       Socket,
  IN UINT8                       Channel,
  IN UINT8                       SubChannel,
  IN UINT8                       Dimm,
  IN UINT8                       Rank,
  IN struct TrainingResultsDdr5  (*TrainResults) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8             Strobe;
  UINT32            PiSetting;
  UINT32            NibbleResult;
  UINT8             Dword;
  struct dimmNvram  (*DimmNvList) [MAX_DIMM];
  UINT8             MaxStrobe = GetSubChMaxStrobeValid (Host);

  AcquirePrintControl ();

  DimmNvList = GetDimmNvList (Host, Socket, Channel);
  // All results stored in first 32 bits
  Dword = 0;

  RcDebugPrint (SDBG_MAX, "\nSummary: LRDIMM Rd MDQS Delay\nS%d, Ch%d, SubCh%d, DIMM%d, Rank%d\n", Socket, Channel, SubChannel, Dimm, Rank);
  RcDebugPrintLine (SDBG_DEFAULT, (MaxStrobe * 4) + 13, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

  RcDebugPrint (SDBG_DEFAULT, "    ");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (!IsStrobeNotValidDdr5 (Host, Strobe)) {
      RcDebugPrint (SDBG_DEFAULT, "%4d", Strobe);
    }
  }

  RcDebugPrint (SDBG_DEFAULT, "\n");

  for (PiSetting = 0; PiSetting < LRDIMM_MRD_BACKSIDE_PI_RANGE; PiSetting++) {

    RcDebugPrint (SDBG_DEFAULT, "%d ", (((INT32) PiSetting) - 15));

    if (((INT32) PiSetting - 15) <= -10) {
      RcDebugPrint (SDBG_DEFAULT, " ");
    }

    if ((((INT32) PiSetting - 15) >= 0) && (((INT32) PiSetting - 15) < 10)) {
      RcDebugPrint (SDBG_DEFAULT, "   ");
    }

    if ((((INT32) PiSetting - 15) < 100) && (((INT32) PiSetting - 15) > 9)) {
      RcDebugPrint (SDBG_DEFAULT, "  ");
    }

    if ((((INT32) PiSetting - 15) < 0) && (((INT32) PiSetting - 15) > -10)) {
      RcDebugPrint (SDBG_DEFAULT, "  ");
    }

    for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
      if (IsStrobeNotValidDdr5 (Host, Strobe)) {
        continue;
      }
      NibbleResult = (*TrainResults) [Channel][SubChannel][Strobe].results[Dword] >> PiSetting;

      if ((NibbleResult & BIT0) != 0) {
        RcDebugPrint (SDBG_DEFAULT, "  1");
      } else {
        RcDebugPrint (SDBG_DEFAULT, "  0");
      }

      RcDebugPrint (SDBG_DEFAULT, " ");
    } // Strobe loop

    RcDebugPrint (SDBG_DEFAULT, "\n");
  } // PiSetting loop

  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
}

/**

  Prints the edges found for DDR5 LRDIMMs during coarse read delay training.

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Current Socket
  @param[in] Channel      - Channel number
  @param[in] PulseWidth   - Pointer to save the pulse width
  @param[in] FallingEdge  - Pointer to save the falling edge
  @param[in] RisingEdge   - Pointer to save the rising edge
  @param[in] CenterPoint  - Pointer to save the center point
  @param[in] MaxStrobe    - Maximum number of strobes

  @retval N/A

**/
VOID
DisplayDdr5LrdimmMrdEdges (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     SubChannel,
  IN UINT16    PulseWidth[SUB_CH][MAX_STROBE_DDR5/2],
  IN UINT16    FallingEdge[SUB_CH][MAX_STROBE_DDR5/2],
  IN UINT16    RisingEdge[SUB_CH][MAX_STROBE_DDR5/2],
  IN UINT16    CenterPoint[SUB_CH][MAX_STROBE_DDR5/2],
  IN UINT8     MaxStrobe
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8 Strobe;

  if (!(GetDebugLevel () & SDBG_MAX)) {
    return;
  }

  AcquirePrintControl ();

  RcDebugPrintLine (SDBG_MAX, 76, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

  // Print rising edges
  RcDebugPrint (SDBG_DEFAULT, "RE: ");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", ((INT32) RisingEdge[SubChannel][Strobe] - 15));
  } // Strobe loop

  // Print center points
  RcDebugPrint (SDBG_DEFAULT, "\nCP: ");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", ((INT32) CenterPoint[SubChannel][Strobe] - 15));
  } // Strobe loop

  // Print falling edges
  RcDebugPrint (SDBG_DEFAULT, "\nFE: ");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", ((INT32) FallingEdge[SubChannel][Strobe] - 15));
  } // Strobe loop

  // Print pulse width
  RcDebugPrint (SDBG_DEFAULT, "\nPW: ");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", ((INT32) PulseWidth[SubChannel][Strobe]));
  } // Strobe loop
  RcDebugPrint (SDBG_DEFAULT, "\n\n");

  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
} // DisplayDdr5LrdimmMrdEdges

/**

  Prints out the backside swizzle detection results for DDR5 LRDIMM.

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Socket number
  @param[in] Dimm           - DIMM number
  @param[in] Rank           - Rank number
  @param[in] DbToDramDqMap  - Array which contains mapping info

  @retval NA

**/
VOID
DisplayBacksideSwizzleResults (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT32   (*DbToDramDqMap)[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][SUB_CH][SWIZZLE_RESULTS_LEN]
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8             MaxChDdr;
  UINT8             Channel;
  UINT8             SubChannel;
  UINT8             DataBuffer;
  UINT8             DqNumber;
  UINT8             DramDq;
  UINT8             Dword;
  UINT32            SwizzleValue;

  if (!(GetDebugLevel () & SDBG_MAX)) {
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  AcquirePrintControl ();

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
      continue;
    }

    RcDebugPrint (SDBG_DEFAULT, "\nSummary: LRDIMM Backside Swizzle Discovery\nS%d, Ch%d, DIMM%d Rank%d\n", Socket, Channel, Dimm, Rank);

    RcDebugPrint (SDBG_DEFAULT, "DRAM DQ            ");
    for (DqNumber = 0; DqNumber < DB_DQ_LANE; DqNumber++) {
      DramDq = DqNumber;
      if (DqNumber >= BITS_PER_NIBBLE) {
        DramDq = DqNumber - BITS_PER_NIBBLE;
      }
      RcDebugPrint (SDBG_DEFAULT, "%4d", DramDq);
    }
    RcDebugPrint (SDBG_DEFAULT, "\n");
    RcDebugPrint (SDBG_DEFAULT, "DB DQ:                0   1   2   3   4   5   6   7\n");
    RcDebugPrint (SDBG_DEFAULT, "\n");

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (DataBuffer = 0; DataBuffer < DB_PER_SUBCH; DataBuffer++) {

        RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, Channel, Dimm, SubChannel, NO_RANK, DataBuffer, NO_BIT, "");
        for (DqNumber = 0; DqNumber < DB_DQ_LANE; DqNumber++) {
          Dword = (DqNumber + (DataBuffer * DB_DQ_LANE)) / 10;
          SwizzleValue = ((*DbToDramDqMap)[Channel][Dimm][Rank][SubChannel][Dword] >> (((DqNumber + (DataBuffer * DB_DQ_LANE)) % 10) * 3));
          SwizzleValue &= SWIZZLE_BIT_MASK;
          RcDebugPrint (SDBG_DEFAULT, "%4d", SwizzleValue);
        }
        RcDebugPrint (SDBG_DEFAULT, "\n");

      } // DataBuffer loop
      RcDebugPrint (SDBG_DEFAULT, "\n");
    } // SubChannel loop
  } // Channel loop
  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
}

/**

  Prints out the frontside swizzle detection results for DDR5 LRDIMM.

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Socket number
  @param[in] Dimm           - DIMM number
  @param[in] HostToDbDqMap  - Array which contains mapping info

  @retval NA

**/
VOID
DisplayFrontsideSwizzleResults (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Dimm,
  IN UINT32   (*HostToDbDqMap)[MAX_CH][MAX_DIMM][SUB_CH][SWIZZLE_RESULTS_LEN]
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8             MaxChDdr;
  UINT8             Channel;
  UINT8             SubChannel;
  UINT8             DataBuffer;
  UINT8             DataBuffersPerSubCh;
  UINT8             NumDqLanes;
  UINT8             DqNumber;
  UINT8             Dword;
  UINT32            SwizzleValue;

  if (!(GetDebugLevel () & SDBG_MAX)) {
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  DataBuffersPerSubCh = DB_PER_SUBCH;
  NumDqLanes = DB_DQ_LANE;
  AcquirePrintControl ();

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
      continue;
    }

    RcDebugPrint (SDBG_DEFAULT, "\nSummary: LRDIMM Frontside Swizzle Discovery\nS%d, Ch%d, DIMM%d\n", Socket, Channel, Dimm);

    RcDebugPrint (SDBG_DEFAULT, "Host DQ            ");
    for (DqNumber = 0; DqNumber < NumDqLanes; DqNumber++) {
      RcDebugPrint (SDBG_DEFAULT, "%4d", DqNumber);
    }
    RcDebugPrint (SDBG_DEFAULT, "\n\n");
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (DataBuffer = 0; DataBuffer < DataBuffersPerSubCh; DataBuffer++) {

        RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, Channel, Dimm, SubChannel, NO_RANK, DataBuffer, NO_BIT, "");
        for (DqNumber = 0; DqNumber < NumDqLanes; DqNumber++) {
          Dword = (DqNumber + (DataBuffer * DB_DQ_LANE)) / 10;
          SwizzleValue = ((*HostToDbDqMap)[Channel][Dimm][SubChannel][Dword] >> (((DqNumber + (DataBuffer * DB_DQ_LANE)) % 10) * 3));
          SwizzleValue &= SWIZZLE_BIT_MASK;
          RcDebugPrint (SDBG_DEFAULT, "%4d", SwizzleValue);
        }
        RcDebugPrint (SDBG_DEFAULT, "\n");

      } // DataBuffer loop
      RcDebugPrint (SDBG_DEFAULT, "\n");
    } // SubChannel loop
  } // Channel loop
  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
}

/**

  This function prints the Per-Bit Read Delay sample array

  @param[in] Host          - Pointer to sysHost
  @param[in] Socket        - Socket number
  @param[in] Channel       - Channel number
  @param[in] Dimm          - DIMM number
  @param[in] SubChannel    - SubChannel number
  @param[in] Dimm          - DIMM number
  @param[in] Rank          - Rank number on DIMM
  @param[in] TrainResults  - Pointer to struct containing DDR5 training results

  @retval N/A

**/
VOID
DisplayPerBitSweepResults (
  IN PSYSHOST                    Host,
  IN UINT8                       Socket,
  IN UINT8                       Channel,
  IN UINT8                       SubChannel,
  IN UINT8                       Dimm,
  IN UINT8                       Rank,
  IN struct MdqTrainingResults   (*TrainResults) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8             Strobe;
  UINT32            PiSetting;
  UINT8             DqLane;
  UINT32            NibbleResult;
  struct dimmNvram  (*DimmNvList) [MAX_DIMM];
  UINT8             MaxStrobe = GetSubChMaxStrobeValid (Host);

  AcquirePrintControl ();

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  RcDebugPrint (SDBG_MAX, "\nSummary: LRDIMM Per-Bit Rd Delay\nS%d, Ch%d, SubCh%d, DIMM%d, Rank%d\n", Socket, Channel, SubChannel, Dimm, Rank);
  RcDebugPrintLine (SDBG_DEFAULT, (MaxStrobe * 4) + 60, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (!IsStrobeNotValidDdr5 (Host, Strobe)) {
      RcDebugPrint (SDBG_DEFAULT, "%9d", Strobe);
    }
  }
  RcDebugPrint (SDBG_DEFAULT, "\n");
  RcDebugPrint (SDBG_DEFAULT, "DQ    ");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (!IsStrobeNotValidDdr5 (Host, Strobe)) {
      RcDebugPrint (SDBG_DEFAULT, " ");
      for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
        RcDebugPrint (SDBG_DEFAULT, "%2d", DqLane);
      }
    }
  }
  RcDebugPrint (SDBG_DEFAULT, "\n\n");

  for (PiSetting = 0; PiSetting < (MAX_PHASE_IN_FINE_ADJUSTMENT * 2); PiSetting++) {

    if (PiSetting % 4) {
      continue;
    }

    RcDebugPrint (SDBG_DEFAULT, "%d ", (((INT32) PiSetting / 4) - 15));

    if (((INT32) PiSetting / 4 - 15) <= -10) {
      RcDebugPrint (SDBG_DEFAULT, "   ");
    }

    if ((((INT32) PiSetting / 4 - 15) >= 0) && (((INT32) PiSetting / 4 - 15) < 10)) {
      RcDebugPrint (SDBG_DEFAULT, "     ");
    }

    if ((((INT32) PiSetting / 4 - 15) < 100) && (((INT32) PiSetting / 4 - 15) > 9)) {
      RcDebugPrint (SDBG_DEFAULT, "    ");
    }

    if ((((INT32) PiSetting / 4 - 15) < 0) && (((INT32) PiSetting / 4 - 15) > -10)) {
      RcDebugPrint (SDBG_DEFAULT, "    ");
    }

    for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
      if (IsStrobeNotValidDdr5 (Host, Strobe)) {
        continue;
      }
      NibbleResult = (*TrainResults) [Channel][SubChannel][Strobe].Results[PiSetting / 32] >> PiSetting;
      for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
        if (((NibbleResult) & (1 << DqLane)) != 0) {
          RcDebugPrint (SDBG_DEFAULT, " 1");
        } else {
          RcDebugPrint (SDBG_DEFAULT, " 0");
        }
      }
      RcDebugPrint (SDBG_DEFAULT, " ");
    } // Strobe loop

    RcDebugPrint (SDBG_DEFAULT, "\n");
  } // PiSetting loop

  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
}

/**

  This function prints the Per-Bit Write Delay sample array

  @param[in] Host          - Pointer to sysHost
  @param[in] Socket        - Socket number
  @param[in] Channel       - Channel number
  @param[in] Dimm          - DIMM number
  @param[in] SubChannel    - SubChannel number
  @param[in] Dimm          - DIMM number
  @param[in] Rank          - Rank number on DIMM
  @param[in] TrainResults  - Pointer to struct containing DDR5 training results

  @retval N/A

**/
VOID
DisplayWriteDelaySweepResults (
  IN PSYSHOST                   Host,
  IN UINT8                      Socket,
  IN UINT8                      Channel,
  IN UINT8                      SubChannel,
  IN UINT8                      Dimm,
  IN UINT8                      Rank,
  IN struct MdqTrainingResults  (*LrTrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8             Strobe;
  UINT16            PiSetting;
  UINT8             DWord;
  UINT8             DqLane;
  UINT32            NibbleResult;
  struct dimmNvram  (*DimmNvList) [MAX_DIMM];
  UINT8             MaxStrobe = GetSubChMaxStrobeValid (Host);

  AcquirePrintControl ();

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  RcDebugPrint (SDBG_MAX, "\nSummary: LRDIMM Write Delay\nS%d, Ch%d, SubCh%d, DIMM%d, Rank%d\n", Socket, Channel, SubChannel, Dimm, Rank);
  RcDebugPrintLine (SDBG_MAX, (MaxStrobe * 4) + 60, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (!IsStrobeNotValidDdr5 (Host, Strobe)) {
      RcDebugPrint (SDBG_MAX, "%9d", Strobe);
    }
  }
  RcDebugPrint (SDBG_MAX, "\n");
  RcDebugPrint (SDBG_MAX, "DQ    ");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (!IsStrobeNotValidDdr5 (Host, Strobe)) {
      RcDebugPrint (SDBG_MAX, " ");
      for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
        RcDebugPrint (SDBG_MAX, "%2d", DqLane);
      }
    }
  }
  RcDebugPrint (SDBG_MAX, "\n\n");

  for (PiSetting = 0; PiSetting < DDR5_LRDIMM_BACKSIDE_WRITE_DELAY_END; PiSetting++) {

    RcDebugPrint (SDBG_MAX, "%d ", PiSetting);

    if (PiSetting < 10) {
      RcDebugPrint (SDBG_MAX, "     ");
    }

    if ((PiSetting < 100) && (PiSetting > 9)) {
      RcDebugPrint (SDBG_MAX, "    ");
    }

    if (PiSetting >= 100) {
      RcDebugPrint (SDBG_MAX, "   ");
    }

    DWord = (UINT8) (PiSetting / 8);
    for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
      if (IsStrobeNotValidDdr5 (Host, Strobe)) {
        continue;
      }
      NibbleResult = (*LrTrainRes) [Channel][SubChannel][Strobe].Results[DWord] >> ((PiSetting % 8) * 4);
      for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
        if (((NibbleResult) & (1 << DqLane)) != 0) {
          RcDebugPrint (SDBG_MAX, " 1");
        } else {
          RcDebugPrint (SDBG_MAX, " 0");
        }
      }
      RcDebugPrint (SDBG_MAX, " ");
    } // Strobe loop

    RcDebugPrint (SDBG_MAX, "\n");
  } // PiSetting loop

  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
}

/**

  Displays the calculated per-MDQ rising edge, center point, falling edge, and
  pulse width results.

  @param[in] Host               - Pointer to sysHost
  @param[in] Socket             - Socket number
  @param[in] Channel            - Channel number
  @param[in] SubChannel         - SubChannel number
  @param[in] PerBitPulseWidth   - Array containing per-MDQ pulse widths
  @param[in] PerBitFallingEdge  - Array containing per-MDQ falling edges
  @param[in] PerBitRisingEdge   - Array containing per-MDQ rising edges
  @param[in] PerBitCenterPoint  - Array containing per-MDQ center points

  @retval N/A

**/
VOID
DisplayPerBitEdges (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     SubChannel,
  IN UINT16    PerBitPulseWidth[SUB_CH][MAX_STROBE_DDR5/2][BITS_PER_NIBBLE],
  IN UINT16    PerBitFallingEdge[SUB_CH][MAX_STROBE_DDR5/2][BITS_PER_NIBBLE],
  IN UINT16    PerBitRisingEdge[SUB_CH][MAX_STROBE_DDR5/2][BITS_PER_NIBBLE],
  IN UINT16    PerBitCenterPoint[SUB_CH][MAX_STROBE_DDR5/2][BITS_PER_NIBBLE]
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8   Strobe;
  UINT8   DqLane;
  UINT8   MaxStrobe = GetSubChMaxStrobeValid (Host);
  MRC_TT  TestType;
  UINT8   Encode;

  if (!(GetDebugLevel () & SDBG_MAX)) {
    return;
  }

  TestType = GetCurrentTestType (Socket);
  // Read Delay applies encode to stored values to make finding edges easier. De-apply this
  // encode for Read Delay values.
  if (TestType == MdqsReadDelayDeskew) {
    Encode = POSITIVE_ENCODE;
  } else {
    Encode = 0;
  }
  AcquirePrintControl ();

  RcDebugPrintLine (SDBG_DEFAULT, (MaxStrobe * 4) + 60, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

  RcDebugPrint (SDBG_DEFAULT, "Nibble    ");
  for (Strobe = 0; Strobe < MaxStrobe / 2; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    if (Strobe == 0) {
      RcDebugPrint (SDBG_DEFAULT, "%d", Strobe);
    } else {
      RcDebugPrint (SDBG_DEFAULT, "%16d", Strobe);
    }
  }
  RcDebugPrint (SDBG_DEFAULT, "\n");
  RcDebugPrint (SDBG_DEFAULT, "       ");
  for (Strobe = 0; Strobe < MaxStrobe / 2; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
      RcDebugPrint (SDBG_DEFAULT, "%4d", DqLane);
    }
  }
  RcDebugPrint (SDBG_DEFAULT, "\n");
  // Print rising edges
  RcDebugPrint (SDBG_DEFAULT, "RE:     ");
  for (Strobe = 0; Strobe < MaxStrobe / 2; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
      RcDebugPrint (SDBG_DEFAULT, " %3d", ((INT32) PerBitRisingEdge[SubChannel][Strobe][DqLane] - Encode));
    }
  } // Strobe loop

  // Print center points
  RcDebugPrint (SDBG_DEFAULT, "\nCP:     ");
  for (Strobe = 0; Strobe < MaxStrobe / 2; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
      RcDebugPrint (SDBG_DEFAULT, " %3d", ((INT32) PerBitCenterPoint[SubChannel][Strobe][DqLane] - Encode));
    }
  } // Strobe loop

  // Print falling edges
  RcDebugPrint (SDBG_DEFAULT, "\nFE:     ");
  for (Strobe = 0; Strobe < MaxStrobe / 2; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
      RcDebugPrint (SDBG_DEFAULT, " %3d", ((INT32) PerBitFallingEdge[SubChannel][Strobe][DqLane] - Encode));
    }
  } // Strobe loop

  // Print pulse width
  RcDebugPrint (SDBG_DEFAULT, "\nPW:     ");
  for (Strobe = 0; Strobe < MaxStrobe / 2; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
      RcDebugPrint (SDBG_DEFAULT, " %3d", ((INT32) PerBitPulseWidth[SubChannel][Strobe][DqLane]));
    }
  } // Strobe loop
  RcDebugPrint (SDBG_DEFAULT, "\n\n");

  RcDebugPrint (SDBG_DEFAULT, "Nibble    ");
  for (Strobe = MaxStrobe / 2; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    if (Strobe == 5) {
      RcDebugPrint (SDBG_DEFAULT, "%d", Strobe);
    } else {
      RcDebugPrint (SDBG_DEFAULT, "%16d", Strobe);
    }
  }
  RcDebugPrint (SDBG_DEFAULT, "\n");
  RcDebugPrint (SDBG_DEFAULT, "       ");
  for (Strobe = MaxStrobe / 2; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
      RcDebugPrint (SDBG_DEFAULT, "%4d", DqLane);
    }
  }
  RcDebugPrint (SDBG_DEFAULT, "\n");
  // Print rising edges
  RcDebugPrint (SDBG_DEFAULT, "RE:     ");
  for (Strobe = MaxStrobe / 2; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
      RcDebugPrint (SDBG_DEFAULT, " %3d", ((INT32) PerBitRisingEdge[SubChannel][Strobe][DqLane] - Encode));
    }
  } // Strobe loop

  // Print center points
  RcDebugPrint (SDBG_DEFAULT, "\nCP:     ");
  for (Strobe = MaxStrobe / 2; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
      RcDebugPrint (SDBG_DEFAULT, " %3d", ((INT32) PerBitCenterPoint[SubChannel][Strobe][DqLane] - Encode));
    }
  } // Strobe loop

  // Print falling edges
  RcDebugPrint (SDBG_DEFAULT, "\nFE:     ");
  for (Strobe = MaxStrobe / 2; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
      RcDebugPrint (SDBG_DEFAULT, " %3d", ((INT32) PerBitFallingEdge[SubChannel][Strobe][DqLane] - Encode));
    }
  } // Strobe loop

  // Print pulse width
  RcDebugPrint (SDBG_DEFAULT, "\nPW:     ");
  for (Strobe = MaxStrobe / 2; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
      RcDebugPrint (SDBG_DEFAULT, " %3d", ((INT32) PerBitPulseWidth[SubChannel][Strobe][DqLane]));
    }
  } // Strobe loop
  RcDebugPrint (SDBG_DEFAULT, "\n\n");

  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
}

/**

  Displays the calculated per-MDQ rising edge, center point, falling edge, and
  pulse width results.

  @param[in] Host                  - Pointer to sysHost
  @param[in] Socket                - Socket number
  @param[in] Channel               - Channel number
  @param[in] Dimm                  - DIMM number
  @param[in] Rank                  - Rank number on DIMM
  @param[in] SubChannel            - SubChannel number
  @param[in] PerNibbleCenterPoint  - Array containing per-nibble center points
  @param[in] DqOffset              - Array containing per-MDQ offsets to be programmed

  @retval N/A

**/
VOID
DisplayMdqOffsets (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     SubChannel,
  IN UINT8     PerNibbleCenter[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2],
  IN INT8      DqOffset[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE]
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8   Strobe;
  UINT8   DqLane;
  UINT8   MaxStrobe = GetSubChMaxStrobeValid (Host);
  MRC_TT  TestType;
  UINT8   Encode;

  if (!(GetDebugLevel () & SDBG_MAX)) {
    return;
  }

  TestType = GetCurrentTestType (Socket);
  // Read Delay applies encode to stored values to make finding edges easier. De-apply this
  // encode for Read Delay values.
  if (TestType == MdqsReadDelayDeskew) {
    Encode = POSITIVE_ENCODE;
  } else {
    Encode = 0;
  }
  AcquirePrintControl ();

  RcDebugPrint (SDBG_DEFAULT, "\nCalculated Nibble Center Points\n");
  RcDebugPrint (SDBG_DEFAULT, "    ");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (!IsStrobeNotValidDdr5 (Host, Strobe)) {
      RcDebugPrint (SDBG_DEFAULT, "%4d", Strobe);
    }
  }

  // Print center points
  RcDebugPrint (SDBG_DEFAULT, "\nCP: ");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", ((INT32) PerNibbleCenter[Channel][Dimm][SubChannel][Rank][Strobe] - Encode));
  } // Strobe loop

  RcDebugPrint (SDBG_DEFAULT, "\n\nPer-DQ Offset\n");

  RcDebugPrint (SDBG_DEFAULT, "Nibble    ");
  for (Strobe = 0; Strobe < MaxStrobe / 2; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    if (Strobe == 0) {
      RcDebugPrint (SDBG_DEFAULT, "%d", Strobe);
    } else {
      RcDebugPrint (SDBG_DEFAULT, "%16d", Strobe);
    }
  }
  RcDebugPrint (SDBG_DEFAULT, "\n");
  RcDebugPrint (SDBG_DEFAULT, "       ");
  for (Strobe = 0; Strobe < MaxStrobe / 2; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
      RcDebugPrint (SDBG_DEFAULT, "%4d", DqLane);
    }
  }

  RcDebugPrint (SDBG_DEFAULT, "\n");
  RcDebugPrintLine (SDBG_DEFAULT, (MaxStrobe * 4) + 60, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  // Print DQ Offsets
  RcDebugPrint (SDBG_DEFAULT, "OFFSET ");
  for (Strobe = 0; Strobe < MaxStrobe / 2; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
      RcDebugPrint (SDBG_DEFAULT, " %3d", ((INT32) DqOffset[Channel][SubChannel][Strobe][DqLane]));
    }
  } // Strobe loop

  RcDebugPrint (SDBG_DEFAULT, "\n\n");

  RcDebugPrint (SDBG_DEFAULT, "Nibble    ");
  for (Strobe = MaxStrobe / 2; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    if (Strobe == 5) {
      RcDebugPrint (SDBG_DEFAULT, "%d", Strobe);
    } else {
      RcDebugPrint (SDBG_DEFAULT, "%16d", Strobe);
    }
  }
  RcDebugPrint (SDBG_DEFAULT, "\n");
  RcDebugPrint (SDBG_DEFAULT, "       ");
  for (Strobe = MaxStrobe / 2; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
      RcDebugPrint (SDBG_DEFAULT, "%4d", DqLane);
    }
  }
  RcDebugPrint (SDBG_DEFAULT, "\n");
  RcDebugPrintLine (SDBG_DEFAULT, (MaxStrobe * 4) + 60, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  // Print rising edges
  RcDebugPrint (SDBG_DEFAULT, "OFFSET ");
  for (Strobe = MaxStrobe / 2; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
      RcDebugPrint (SDBG_DEFAULT, " %3d", ((INT32) DqOffset[Channel][SubChannel][Strobe][DqLane]));
    }
  } // Strobe loop

  RcDebugPrint (SDBG_DEFAULT, "\n\n");

  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
}

/**

  Display the Per-bit offset results of backside MDQ Delay across all bits of a subchannel.

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Current socket number
  @param[in] PerBitResults  - Stores final per-bit results

  @retval N/A

**/
VOID
DisplayFinalMdqPerBitResults (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    PerBitResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][SWIZZLE_RESULTS_LEN]
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8   Channel;
  UINT8   Dimm;
  UINT8   Rank;
  UINT8   SubChannel;
  UINT8   Strobe;
  UINT8   MaxStrobePairs;
  UINT8   MaxChDdr;
  UINT8   MaxBits;
  UINT8   DqLane;
  INT32   DqOffset;
  UINT8   Dword;
  MRC_TT  TestType;

  // Maximum bits per subchannel
  if (IsBrsPresent (Host, Socket)) {
    MaxBits = GetMaxBitsValid (Host);
  } else {
    MaxBits = GetMaxBitsValid (Host) / 2;
  }
  TestType = GetCurrentTestType (Socket);
  if (Host->nvram.mem.eccEn == 0) {
    MaxBits = MaxBits - DB_DQ_LANE;
  }
  MaxStrobePairs = GetSubChMaxStrobeValid (Host) / 2;
  if (!(GetDebugLevel () & SDBG_MAX)) {
    return;
  }

  AcquirePrintControl ();

  MaxChDdr = GetMaxChDdr ();
  if (TestType == MdqsReadDelayDeskew) {
    RcDebugPrint (SDBG_DEFAULT, "\nSTART_DATA_DB_RX_MDQ_PER_BIT\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "\nSTART_DATA_DB_TX_MDQ_PER_BIT\n");
  }

  RcDebugPrint (SDBG_DEFAULT, "BIT:              ");
  for (DqLane = 0; DqLane < MaxBits; DqLane++) {
    if (IsBitNotValid (Host, DqLane)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, "%4d", DqLane);
  }

  RcDebugPrint (SDBG_DEFAULT, "\n");
  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

        for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
          if (IsRankValid (Socket, Channel, Dimm, Rank, FALSE) == FALSE) {
            continue;
          }
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT, "");

          for (Strobe = 0; Strobe < MaxStrobePairs; Strobe++) {
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }
            for (DqLane = 0; DqLane < DB_DQ_LANE; DqLane++) {
              Dword = (DqLane + (Strobe * DB_DQ_LANE)) / 10;
              DqOffset = (PerBitResults[Channel][Dimm][SubChannel][Rank][Dword] >> (((DqLane + (Strobe * DB_DQ_LANE)) % 10) * 3));
              DqOffset &= SWIZZLE_BIT_MASK;
              // BIT2 means offset is negative - mask off bit and make offset negative
              if (DqOffset & BIT2) {
                DqOffset &= ~(BIT2);
                DqOffset *= -1;
              }
              RcDebugPrint (SDBG_DEFAULT, "%4d", DqOffset);
            } // DqLane loop
          } // Strobe loop
          RcDebugPrint (SDBG_DEFAULT, "\n");
        } // Rank loop
      } // SubChannel loop
    } // Dimm loop
  } // Channel loop

  if (TestType == MdqsReadDelayDeskew) {
    RcDebugPrint (SDBG_DEFAULT, "STOP_DATA_DB_RX_MDQ_PER_BIT\n\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "STOP_DATA_DB_TX_MDQ_PER_BIT\n\n");
  }
  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
}

/**

  Display the nibble offset results of backside MDQ Delay across all bits of a subchannel.

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Current socket number
  @param[in] PerBitResults  - Stores final per-bit results

  @retval N/A

**/
VOID
DisplayFinalMdqPerNibbleResults (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     PerNibbleCenter[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2]
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8   Channel;
  UINT8   Dimm;
  UINT8   Rank;
  UINT8   SubChannel;
  UINT8   Strobe;
  UINT8   MaxStrobes;
  UINT8   MaxChDdr;
  MRC_TT  TestType;

  TestType = GetCurrentTestType (Socket);

  MaxStrobes = GetSubChMaxStrobeValid (Host);
  if (!(GetDebugLevel () & SDBG_MAX)) {
    return;
  }

  AcquirePrintControl ();

  MaxChDdr = GetMaxChDdr ();
  if (TestType == MdqsReadDelayDeskew) {
    RcDebugPrint (SDBG_DEFAULT, "\nSTART_DATA_DB_RX_MDQ\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "\nSTART_DATA_DB_TX_MDQ\n");
  }

  RcDebugPrint (SDBG_DEFAULT, "NIBBLE:           ");
  for (Strobe = 0; Strobe < MaxStrobes; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, "%4d", Strobe);
  }

  RcDebugPrint (SDBG_DEFAULT, "\n");
  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

        for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
          if (IsRankValid (Socket, Channel, Dimm, Rank, FALSE) == FALSE) {
            continue;
          }
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT, "");

          for (Strobe = 0; Strobe < MaxStrobes; Strobe++) {
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }

            RcDebugPrint (SDBG_DEFAULT, "%4d", PerNibbleCenter[Channel][Dimm][SubChannel][Rank][Strobe]);
          } // Strobe loop
          RcDebugPrint (SDBG_DEFAULT, "\n");
        } // Rank loop
      } // SubChannel loop
    } // Dimm loop
  } // Channel loop

  if (TestType == MdqsReadDelayDeskew) {
    RcDebugPrint (SDBG_DEFAULT, "STOP_DATA_DB_RX_MDQ\n\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "STOP_DATA_DB_TX_MDQ\n\n");
  }
  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
}

/**

  Display the ODTLON/ODTLOFF values of backside MDQ Write Delay.

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Current socket number
  @param[in] OdtlOnResults   - Stores final ODTLON results
  @param[in] OdtlOffResults  - Stores final ODTLOFF results

  @retval N/A

**/
VOID
DisplayFinalOdtlResults (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN INT8      OdtlOnResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2],
  IN INT8      OdtlOffResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2]
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8   Channel;
  UINT8   Dimm;
  UINT8   Rank;
  UINT8   SubChannel;
  UINT8   Strobe;
  UINT8   MaxStrobes;
  UINT8   MaxChDdr;

  MaxStrobes = GetSubChMaxStrobeValid (Host);
  if (!(GetDebugLevel () & SDBG_MAX)) {
    return;
  }

  AcquirePrintControl ();

  MaxChDdr = GetMaxChDdr ();

  RcDebugPrint (SDBG_DEFAULT, "\nSTART_DATA_DRAM_ODTLON_WR\n");
  RcDebugPrint (SDBG_DEFAULT, "DRAM:             ");
  for (Strobe = 0; Strobe < MaxStrobes; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, "%4d", Strobe);
  }

  RcDebugPrint (SDBG_DEFAULT, "\n");
  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

        for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
          if (IsRankValid (Socket, Channel, Dimm, Rank, FALSE) == FALSE) {
            continue;
          }
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT, "");

          for (Strobe = 0; Strobe < MaxStrobes; Strobe++) {
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }

            RcDebugPrint (SDBG_DEFAULT, "%4d", OdtlOnResults[Channel][Dimm][SubChannel][Rank][Strobe]);
          } // Strobe loop
          RcDebugPrint (SDBG_DEFAULT, "\n");
        } // Rank loop
      } // SubChannel loop
    } // Dimm loop
  } // Channel loop

  RcDebugPrint (SDBG_DEFAULT, "STOP_DATA_DRAM_ODTLON_WR\n\n");
  RcDebugPrint (SDBG_DEFAULT, "\nSTART_DATA_DRAM_ODTLOFF_WR\n");
  RcDebugPrint (SDBG_DEFAULT, "DRAM:             ");

  for (Strobe = 0; Strobe < MaxStrobes; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, "%4d", Strobe);
  }

  RcDebugPrint (SDBG_DEFAULT, "\n");
  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

        for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
          if (IsRankValid (Socket, Channel, Dimm, Rank, FALSE) == FALSE) {
            continue;
          }
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT, "");

          for (Strobe = 0; Strobe < MaxStrobes; Strobe++) {
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }

            RcDebugPrint (SDBG_DEFAULT, "%4d", OdtlOffResults[Channel][Dimm][SubChannel][Rank][Strobe]);
          } // Strobe loop
          RcDebugPrint (SDBG_DEFAULT, "\n");
        } // Rank loop
      } // SubChannel loop
    } // Dimm loop
  } // Channel loop

  RcDebugPrint (SDBG_DEFAULT, "STOP_DATA_DRAM_ODTLOFF_WR\n\n");
  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
}

/**

  Program the final Read Delay for Coarse Read Delay training.

  @param[in]      Host         - Pointer to sysHost structure
  @param[in]      Socket       - Socket number
  @param[in]      Channel      - Channel number
  @param[in]      SubChannel   - SubChannel number
  @param[in]      Dimm         - DIMM number
  @param[in]      Rank         - Rank number
  @param[in][out] CenterPoint  - Pointer to array which holds centered delay values

  @retval Status  - Non-zero if any failure detected

**/
MRC_STATUS
ProgramFinalCoarseReadDelay (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Channel,
  IN UINT8      SubChannel,
  IN UINT8      Dimm,
  IN UINT8      Rank,
  IN OUT UINT8  CenterPoint[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2]
  )
{
  MRC_STATUS                        Status;
  UINT8                             Strobe;
  DDR5_DATA_BUFFER_PG0_RWE4_STRUCT  LowerMdqsDelayControl;
  DDR5_DATA_BUFFER_PG0_RWE5_STRUCT  UpperMdqsDelayControl;
  UINT8                             ReadDelayTemp;
  UINT8                             DataBuffer;
  UINT8                             MaxStrobePairs;
  UINT8                             SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  DDR5_LRDIMM_TRAINING_DATA         *TrainingData;
  BOOLEAN                           (*NibbleSwap)[MAX_CH][MAX_DIMM][SUB_CH][MAX_STROBE_DDR5 / 2];
  struct dimmNvram                  (*DimmNvList) [MAX_DIMM];
  struct rankDevice                 (*RankStruct) [MAX_RANK_DIMM];

  RankStruct = GetRankStruct (Host, Socket, Channel, Dimm);
  DimmNvList = GetDimmNvList (Host, Socket, Channel);
  // Retrieve nibble swap info from MRE
  Status = GetDdr5LrdimmTrainingData (&TrainingData);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }
  NibbleSwap = &(TrainingData->NibbleSwap);
  LowerMdqsDelayControl.Data = 0;
  UpperMdqsDelayControl.Data = 0;

  MaxStrobePairs = SubChMSVx4 / 2;
  for (Strobe = 0; Strobe < MaxStrobePairs; Strobe++) {
    LowerMdqsDelayControl.Bits.lower_nibble_mdqs_delay_control_during_read = EncodeDecodeReadDelay (Decode, ((UINT8) (CenterPoint[Channel][Dimm][SubChannel][Rank][Strobe])));
    UpperMdqsDelayControl.Bits.upper_nibble_mdqs_delay_control_during_read = EncodeDecodeReadDelay (Decode, ((UINT8) (CenterPoint[Channel][Dimm][SubChannel][Rank][Strobe + MaxStrobePairs])));

    // If there is nibble swap, low nibble MDQS center = high nibble MDQS center and vice versa
    if ((*NibbleSwap)[Channel][Dimm][SubChannel][Strobe] == TRUE) {
      ReadDelayTemp = LowerMdqsDelayControl.Data;
      LowerMdqsDelayControl.Data = UpperMdqsDelayControl.Data;
      CenterPoint[Channel][Dimm][SubChannel][Rank][Strobe] = CenterPoint[Channel][Dimm][SubChannel][Rank][Strobe + MaxStrobePairs];
      UpperMdqsDelayControl.Data = ReadDelayTemp;
      CenterPoint[Channel][Dimm][SubChannel][Rank][Strobe + MaxStrobePairs] = EncodeDecodeReadDelay (Encode, ReadDelayTemp);
      RcDebugPrint (SDBG_MAX, "Nibble Swap Detected:  SubCh%d  Strobe%d\n", SubChannel, Strobe);
    }
    DataBuffer = Strobe;
    // Select appropriate buffer device
    DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, DataBuffer);

    DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE4_REG, LowerMdqsDelayControl.Data);
    DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE5_REG, UpperMdqsDelayControl.Data);
    //
    // Save trained setting to Cache and NVRAM
    //
    (*DimmNvList) [Dimm].rankList[Rank].LrBuf_PG01RWE4E5[Strobe + (SubChannel * SubChMSVx4)] = LowerMdqsDelayControl.Data;
    (*DimmNvList) [Dimm].rankList[Rank].LrBuf_PG01RWE4E5[Strobe + MaxStrobePairs + (SubChannel * SubChMSVx4)] = UpperMdqsDelayControl.Data;
    (*RankStruct) [Rank].cachedLrBuf_PG01RWE4E5[Strobe + (SubChannel * SubChMSVx4)] = LowerMdqsDelayControl.Data;
    (*RankStruct) [Rank].cachedLrBuf_PG01RWE4E5[Strobe + MaxStrobePairs + (SubChannel * SubChMSVx4)] = UpperMdqsDelayControl.Data;
  }
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);
  return MRC_STATUS_SUCCESS;
} // ProgramFinalCoarseReadDelay

/**

  Evaluates and prints out the results from the Coarse Read Delay sweep and programs
  the Read Delay values.

  @param[in] Host        - Pointer to sysHost structure
  @param[in] Socket      - Socket number
  @param[in] ChBitmask   - Channel bit mask
  @param[in] Dimm        - Dimm number
  @param[in] LrTrainRes  - Training result

  @retval Status  - Non-zero if any failure detected

**/
MRC_STATUS
EvaluateAndProgramReadDelayResults (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN struct    TrainingResultsDdr5 (*LrTrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;
  UINT8       Channel;
  UINT8       MaxChDdr;
  UINT8       SubChannel;
  UINT8       Strobe;
  UINT8       SubChMSVx4;
  UINT16      RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT16      CenterPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT16      FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT16      PulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT8       PerNibbleCenter[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2];

  MaxChDdr = GetMaxChDdr ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }
    if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
      Host->var.mem.piSettingStopFlag[Channel] = 0;
      continue;
    }

    Host->var.mem.piSettingStopFlag[Channel] = GetStrobeMask (Host);

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

      DisplayCoarseReadDelaySweepResults (Host, Socket, Channel, SubChannel, Dimm, Rank, LrTrainRes);

      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        //
        // Skip if this is an ECC strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }

        EvaluatePiSettingResultsDdr5 (Socket, Channel, SubChannel, Dimm, Rank, Strobe, RisingEdge, CenterPoint, FallingEdge, PulseWidth,
             FPT_PI_LRDIMM_RD_MRD_TYPE, LrTrainRes);
        if (Host->nvram.mem.socket[Socket].FaultyPartsFlag[Channel] & (1 << Strobe)) {
          HandleFaultyPart (Host, Socket, Channel, Dimm, Rank, Strobe, PulseWidth[Channel][SubChannel][Strobe]);
        }

        PerNibbleCenter[Channel][Dimm][SubChannel][Rank][Strobe] = (UINT8) CenterPoint[Channel][SubChannel][Strobe];
      } // strobe loop

      DisplayDdr5LrdimmMrdEdges (Host, Socket, Channel, SubChannel, PulseWidth[Channel], FallingEdge[Channel], RisingEdge[Channel], CenterPoint[Channel], SubChMSVx4);

      Status = ProgramFinalCoarseReadDelay (Host, Socket, Channel, SubChannel, Dimm, Rank, PerNibbleCenter);
    }
  } // ch loop
  return Status;
}

/**

  For each data module, a 128 bit vector of zeros and ones holding the tests results is
  evaluated which is a concatenation of the two reads from the feedback CRs.
  BIOS locates the middle of the longest consecutive stream of ones.

  @param[in]  Socket       - Current Socket
  @param[in]  Channel      - Channel number
  @param[in]  SubChannel   - Sub channel number
  @param[in]  Dimm         - Dimm number on Channel
  @param[in]  Rank         - Rank on Dimm
  @param[in]  Strobe       - Strobe number
  @param[out] RisingEdge   - Pointer to save the rising edge
  @param[out] CenterPoint  - Pointer to save the center point
  @param[out] FallingEdge  - Pointer to save the falling edge
  @param[out] PulseWidth   - Pointer to save the pulse width
  @param[in]  TrainRes     - Training result

  @retval N/A

**/
VOID
EFIAPI
EvaluatePerBitResults (
  IN UINT8                       Socket,
  IN UINT8                       Channel,
  IN UINT8                       SubChannel,
  IN UINT8                       Dimm,
  IN UINT8                       Rank,
  IN UINT8                       Strobe,
  IN UINT8                       DqLane,
  OUT UINT16                     PerBitRisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE],
  OUT UINT16                     PerBitCenterPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE],
  OUT UINT16                     PerBitFallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE],
  OUT UINT16                     PerBitPulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE],
  OUT struct MdqTrainingResults  (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT16                     PiSetting = 0;
  UINT16                     PiStart = LRDIMM_BACKSIDE_PI_START;
  UINT32                     NibbleResult;
  UINT16                     OneCount;
  UINT16                     OneStart;
  UINT16                     NumberOfCycle;
  UINT16                     MaxOneCount;
  UINT8                      CyclicCase;
  struct ddrRank             (*RankList) [MAX_RANK_DIMM];
  PSYSHOST                   Host;
  MRC_TT                     TestType;

  Host = GetSysHostPointer ();
  NibbleResult = 0;
  OneStart = 0;

  OneCount = 0;
  MaxOneCount = 0;
  CyclicCase = 0;

  TestType = GetCurrentTestType (Socket);
  if (TestType == MdqsReadDelayDeskew) {
    NumberOfCycle = LRDIMM_MRD_BACKSIDE_PI_RANGE;
  } else {
    NumberOfCycle = DDR5_LRDIMM_BACKSIDE_WRITE_DELAY_END;
  }

  PerBitRisingEdge[Channel][SubChannel][Strobe][DqLane] = 0;
  PerBitFallingEdge[Channel][SubChannel][Strobe][DqLane] = 0;
  PerBitPulseWidth[Channel][SubChannel][Strobe][DqLane] = 0;

  RankList = GetRankNvList (Host, Socket, Channel, Dimm);

  //
  // Start at bit 0
  //
  Host->var.mem.piSettingStopFlag[Channel] = GetStrobeMask (Host); //to make sure all the Strobe will be handled.
  while (Host->var.mem.piSettingStopFlag[Channel] & (1 << Strobe)) {

    //
    // Get the proper training results
    //
    // Each nibble contains the individual DQ results
    NibbleResult = (*TrainRes) [Channel][SubChannel][Strobe].Results[PiSetting / 8];
    // 8 Nibbles stored in each Dword
    NibbleResult = NibbleResult >> ((PiSetting % 8) * BITS_PER_NIBBLE);

    //
    // algo start
    //
    if (NibbleResult & (1 << DqLane)) {
      //
      // this index is 1
      //
      if (OneCount == 0) {
        //
        // This first one so save the starting point
        //
        OneStart = PiSetting;
      }

      OneCount++;
    } else {
      //
      // this index is 0
      //
      if (OneCount > 0) {
        //
        // We just moved from a one to a zero so check if that was the largest stream of one's
        //
        if (MaxOneCount < OneCount) {
          //
          // Record rising/falling edge
          //
          MaxOneCount = OneCount;
          PerBitRisingEdge[Channel][SubChannel][Strobe][DqLane] = OneStart;
          PerBitFallingEdge[Channel][SubChannel][Strobe][DqLane] = PiSetting;
        }
        //
        // Start counting again at 0
        //
        OneCount = 0;
      }
    }

    //
    // Go to the Pi Setting
    //
    PiSetting++;

    //
    // Check if we're at the end of the cycle
    //
    if (PiSetting == NumberOfCycle) {
      if (OneCount > 0) {
        //
        // Cyclic case continue to scan
        // Start back over with bit 0 if the last bit is a 1
        //
        PiSetting = LRDIMM_BACKSIDE_PI_START;
        CyclicCase = 1;
      } else {
        //
        // stop the scan
        // We've checked all bits so save the results
        //
        Host->var.mem.piSettingStopFlag[Channel] &= ~(1 << Strobe);
        PerBitPulseWidth[Channel][SubChannel][Strobe][DqLane] =
          PerBitFallingEdge[Channel][SubChannel][Strobe][DqLane] - PerBitRisingEdge[Channel][SubChannel][Strobe][DqLane];
        PerBitCenterPoint[Channel][SubChannel][Strobe][DqLane] =
          ((PerBitPulseWidth[Channel][SubChannel][Strobe][DqLane] / 2) + PerBitRisingEdge[Channel][SubChannel][Strobe][DqLane]) % (NumberOfCycle - PiStart);
      }
    }
    //
    // Check if this is a cyclic case and if we are at the end of the one's
    //
    if ((CyclicCase == 1) && (OneCount == 0)) {
      Host->var.mem.piSettingStopFlag[Channel] &= ~(1 << Strobe);
      if (PerBitRisingEdge[Channel][SubChannel][Strobe][DqLane] > PerBitFallingEdge[Channel][SubChannel][Strobe][DqLane]) {
        PerBitPulseWidth[Channel][SubChannel][Strobe][DqLane] =
            ((NumberOfCycle - PerBitRisingEdge[Channel][SubChannel][Strobe][DqLane]) + PerBitFallingEdge[Channel][SubChannel][Strobe][DqLane]) - PiStart;
        PerBitCenterPoint[Channel][SubChannel][Strobe][DqLane] =
            (((PerBitPulseWidth[Channel][SubChannel][Strobe][DqLane] / 2) + PerBitRisingEdge[Channel][SubChannel][Strobe][DqLane])) % (NumberOfCycle - PiStart);
      } else {
        //
        // last bit is 1 and first bit after cycle is 0
        //
        PerBitPulseWidth[Channel][SubChannel][Strobe][DqLane] =
          PerBitFallingEdge[Channel][SubChannel][Strobe][DqLane] - PerBitRisingEdge[Channel][SubChannel][Strobe][DqLane];
        PerBitCenterPoint[Channel][SubChannel][Strobe][DqLane] =
          ((PerBitPulseWidth[Channel][SubChannel][Strobe][DqLane] / 2) + PerBitRisingEdge[Channel][SubChannel][Strobe][DqLane]) % (NumberOfCycle - PiStart);
      }
    }
  } // while loop

  if (FAULTY_PARTS_TRACKING_INJECT_ERROR == 1) {
    if ((Channel == 0) && (Dimm == 0) && (Rank == 1) && (Strobe == 5)) {
      PerBitPulseWidth[Channel][SubChannel][Strobe][DqLane] = 1;
      PerBitCenterPoint[Channel][SubChannel][Strobe][DqLane] = ((PerBitPulseWidth[Channel][SubChannel][Strobe][DqLane] / 2) + PerBitRisingEdge[Channel][SubChannel][Strobe][DqLane]) % (NumberOfCycle - PiStart);
    }
  } // FAULTY_PARTS_TRACKING_INJECT_ERROR

  //
  // If the PulseWidth (ones stream) is smaller, indicate error
  //
  if (PerBitPulseWidth[Channel][SubChannel][Strobe][DqLane] <= MDQ_MIN_EYE) {
    Host->nvram.mem.socket[Socket].FaultyPartsFlag[Channel] |= (1 << Strobe);
    (*RankList) [Rank].faultyParts[Strobe] |= FPT_LRDIMM_TRAINING_FAILED;
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Channel, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
      "Failed LRDIMM Pi\n");
  }
}

/**

  Saves off the final offsets for each DQ so the final results can be displayed when training is
  complete.

  @param[in]  Channel              - Channel number
  @param[in]  SubChannel           - SubChannel number
  @param[in]  Dimm                 - DIMM number
  @param[in]  Rank                 - Rank number
  @param[in]  Strobe               - Current strobe number
  @param[in]  LowerDword           - Lower nibble Dword location within structure
  @param[in]  UpperDword           - Upper nibble Dword location within structure
  @param[in]  DqLane               - DQ lane to be saved
  @param[in]  LowerNibbleDqOffset  - DQ offset from nibble center
  @param[in]  UpperNibbleDqOffset  - DQ offset from nibble center
  @param[out] PerBitResults        - Structure in which to save results

**/
VOID
SavePerBitResults (
  IN UINT8    Channel,
  IN UINT8    SubChannel,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Strobe,
  IN UINT8    LowerDword,
  IN UINT8    UpperDword,
  IN UINT8    DqLane,
  IN INT8     LowerNibbleDqOffset,
  IN INT8     UpperNibbleDqOffset,
  OUT UINT32  PerBitResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][SWIZZLE_RESULTS_LEN]
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8  DqOffsetValue;

  // If value is negative, set BIT2 to indicate negative number
  if (LowerNibbleDqOffset < 0) {
    DqOffsetValue = (UINT8) (ABS (LowerNibbleDqOffset) | BIT2);
  } else {
    DqOffsetValue = (UINT8) LowerNibbleDqOffset;
  }

  // Save result to display later
  PerBitResults[Channel][Dimm][SubChannel][Rank][LowerDword] |= DqOffsetValue <<
      (((DqLane + (Strobe * DB_DQ_LANE)) % 10) * 3);

  if (UpperNibbleDqOffset < 0) {
    DqOffsetValue = (UINT8) (ABS (UpperNibbleDqOffset) | BIT2);
  } else {
    DqOffsetValue = (UINT8) UpperNibbleDqOffset;
  }

  PerBitResults[Channel][Dimm][SubChannel][Rank][UpperDword] |= DqOffsetValue <<
      ((((DqLane + BITS_PER_NIBBLE) + (Strobe * DB_DQ_LANE)) % 10) * 3);
#endif // DEBUG_CODE_BLOCK
}

/**

  Programs the Per-DQ values to the Data Buffer.

  @param[in]  Host           - Pointer sysHost
  @param[in]  Socket         - Socket number
  @param[in]  Channel        - Channel number
  @param[in]  SubChannel     - SubChannel number
  @param[in]  Dimm           - DIMM number
  @param[in]  Rank           - Rank number on DIMM
  @param[in]  DqOffset       - Array containing DQ offsets to be programmed
  @param[out] PerBitResults  - Stores final per-bit results

  @retval Status  - Non-zero if any failure detected

**/
MRC_STATUS
ProgramPerBitReadDelay (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     SubChannel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN INT8      DqOffset[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE],
  OUT UINT32   PerBitResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][SWIZZLE_RESULTS_LEN]
  )
{
  MRC_STATUS                 Status = MRC_STATUS_SUCCESS;
  UINT8                      LowerDword;
  UINT8                      UpperDword;
  UINT8                      Strobe;
  UINT8                      DqLane;
  UINT8                      LowerHostDqLane;
  UINT8                      UpperHostDqLane;
  INT8                       LowerNibbleDqOffset;
  INT8                       UpperNibbleDqOffset;
  UINT8                      MaxStrobePairs;
  UINT8                      MdqRegValue;
  UINT8                      DbRegisterAddress;
  DDR5_LRDIMM_TRAINING_DATA  *TrainingData;
  UINT32                      (*HostToDbDqMap)[MAX_CH][MAX_DIMM][SUB_CH][SWIZZLE_RESULTS_LEN];

  // Retrieve nibble swap info
  Status = GetDdr5LrdimmTrainingData (&TrainingData);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }
  HostToDbDqMap = &(TrainingData->HostToDbDqMap);

  MaxStrobePairs = GetSubChMaxStrobeValid (Host) / 2;

  for (Strobe = 0; Strobe < MaxStrobePairs; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    // Select appropriate buffer device
    DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, Strobe);

    for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
      // Determine mapping of DB DQ Lane to Host DQ Lane.  If, for example, DB DQ 0 maps to Host DQ 3, then the DQ
      // results we calculated for DQ 3, should be programmed to the DB DQ 0 register.
      LowerDword = (DqLane + (Strobe * DB_DQ_LANE)) / 10;
      UpperDword = ((DqLane + BITS_PER_NIBBLE) + (Strobe * DB_DQ_LANE)) / 10;
      LowerHostDqLane = ((*HostToDbDqMap)[Channel][Dimm][SubChannel][LowerDword] >>
          ((DqLane + (Strobe * DB_DQ_LANE)) % 10) * 3) & SWIZZLE_BIT_MASK;
      UpperHostDqLane = ((*HostToDbDqMap)[Channel][Dimm][SubChannel][UpperDword] >>
          (((DqLane + BITS_PER_NIBBLE) + (Strobe * DB_DQ_LANE)) % 10) * 3) & SWIZZLE_BIT_MASK;

      // If lower nibble DQ lanes on Host map to lower nibble DQ lanes on DB, then no nibble swapping necessary
      if (LowerHostDqLane < BITS_PER_NIBBLE) {
        LowerNibbleDqOffset = DqOffset[Channel][SubChannel][Strobe][LowerHostDqLane];
        UpperNibbleDqOffset = DqOffset[Channel][SubChannel][Strobe + MaxStrobePairs][UpperHostDqLane - BITS_PER_NIBBLE];
      } else {
        // Lower nibble DQ lanes on Host map to upper nibble DQ lanes on DB
        LowerNibbleDqOffset = DqOffset[Channel][SubChannel][Strobe + MaxStrobePairs][LowerHostDqLane - BITS_PER_NIBBLE];
        UpperNibbleDqOffset = DqOffset[Channel][SubChannel][Strobe][UpperHostDqLane];
      }

      SavePerBitResults (Channel, SubChannel, Dimm, Rank, Strobe, LowerDword, UpperDword, DqLane,
          LowerNibbleDqOffset, UpperNibbleDqOffset, PerBitResults);

      // Lower MDQ bits (0-3) are programmed in the lower 4 bits of control word
      MdqRegValue = GetPerBitDelayRegValue (LowerNibbleDqOffset);
      // Upper MDQ bits (4-7) are programmed in the upper 4 bits of control word
      MdqRegValue |= (GetPerBitDelayRegValue (UpperNibbleDqOffset) << 4);

      // Select appropriate register based on DQ Lane
      switch (DqLane) {
        case 0:
          DbRegisterAddress = DDR5_DATA_BUFFER_PG0_RWEA_REG;
          break;

        case 1:
          DbRegisterAddress = DDR5_DATA_BUFFER_PG0_RWEB_REG;
          break;

        case 2:
          DbRegisterAddress = DDR5_DATA_BUFFER_PG0_RWEC_REG;
          break;

        case 3:
          DbRegisterAddress = DDR5_DATA_BUFFER_PG0_RWED_REG;
          break;

        default:
          // We should never hit this
          FatalError (ERR_INVALID_REG_ACCESS, ERR_UNKNOWN_REG_TYPE);
          break;
      }

      DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DbRegisterAddress, MdqRegValue);
    } // DqLane loop
  } // Strobe loop

  // Select all data buffers now that programming is done
  DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);

  return Status;
}

/**

  Verifies the maximum/minimum value for the calculated MDQ offset falls within the programmable
  range.

  @param[in]     Socket    - Socket number
  @param[in]     Channel   - Channel number
  @param[in]     Strobe    - Strobe number
  @param[in]     DqLane    - MDQ Lane number
  @param[in out] DqOffset  - Pointer to value to be checked

  @retval NA

**/
VOID
CheckDqOffsetRange (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Strobe,
  IN UINT8     DqLane,
  IN OUT INT8  *DqOffset
  )
{
  // If value is less than minimum programmable value, print warning and adjust offset
  if (*DqOffset < MIN_MDQ_OFFSET) {
    // Print warning and adjust value
    RcDebugPrint (SDBG_MINMAX, "MDQ Read Delay Control -- Register Overflow! Strobe: %d DQLane: %d MinValue = %d, Value = %d, adjusted to MinVal.\n", Strobe, DqLane, MIN_MDQ_OFFSET, *DqOffset);
    OutputWarning (WARN_REGISTER_OVERFLOW, WARN_MINOR_REGISTER_OVERFLOW, Socket, Channel, NO_DIMM, NO_RANK);
    *DqOffset = MIN_MDQ_OFFSET;
  } else if (*DqOffset > MAX_MDQ_OFFSET) {
    // Print warning and adjust value
    RcDebugPrint (SDBG_MINMAX, "MDQ Read Delay Control -- Register Overflow! Strobe: %d DQLane: %d MaxValue = %d, Value = %d, adjusted to MaxVal.\n", Strobe, DqLane, MAX_MDQ_OFFSET, *DqOffset);
    OutputWarning (WARN_REGISTER_OVERFLOW, WARN_MINOR_REGISTER_OVERFLOW, Socket, Channel, NO_DIMM, NO_RANK);
    *DqOffset = MAX_MDQ_OFFSET;
  }
}

/**

  Handles the calculations for finding the Per-Bit Read Delay offsets and programming
  these values.

  @param[in]      Host               - Pointer to sysHost
  @param[in]      Socket             - Socket number
  @param[in]      Channel            - Channel number
  @param[in]      Dimm               - DIMM number
  @param[in]      Rank               - Rank number on DIMM
  @param[in]      SubChannel         - SubChannel number
  @param[in][out] PerNibbleCenter    - Array containing Nibble center points for Read Delay
  @param[in]      PerBitCenterPoint  - Array containing per-DQ center points for Read Delay
  @param[out]     PerBitResults      - Stores final per-bit results

  @retval Status - Non-zero if any failure detected

**/
MRC_STATUS
CalculateAndProgramPerBitDelays (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Channel,
  IN UINT8      Dimm,
  IN UINT8      Rank,
  IN UINT8      SubChannel,
  IN OUT UINT8  PerNibbleCenter[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2],
  IN UINT16     PerBitCenterPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE],
  OUT UINT32    PerBitResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][SWIZZLE_RESULTS_LEN]
  )
{
  MRC_STATUS  Status;
  UINT8       Strobe;
  UINT8       DqLane;
  UINT8       MaxStrobe = GetSubChMaxStrobeValid (Host);
  INT8        DqOffset[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE];

  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
      // Store the offset for this DQ lane based on calculated center point for the nibble it resides in
      DqOffset[Channel][SubChannel][Strobe][DqLane] =
          (INT8) (PerNibbleCenter[Channel][Dimm][SubChannel][Rank][Strobe] - PerBitCenterPoint[Channel][SubChannel][Strobe][DqLane]);

      CheckDqOffsetRange (Socket, Channel, Strobe, DqLane, &(DqOffset[Channel][SubChannel][Strobe][DqLane]));
    } // DqLane loop
  } // Strobe loop

  DisplayMdqOffsets (Host, Socket, Channel, Dimm, Rank, SubChannel, PerNibbleCenter, DqOffset);

  // Program the read delay per nibble
  Status = ProgramFinalCoarseReadDelay (Host, Socket, Channel, SubChannel, Dimm, Rank, PerNibbleCenter);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }

  // Program the read delay per MDQ
  Status = ProgramPerBitReadDelay (Host, Socket, Channel, SubChannel, Dimm, Rank, DqOffset, PerBitResults);

  return Status;
}

/**

  Calculates the nibble center based on the center points of the DQ lanes.

  @param[in]  DqCenter0     - Center point for DQ Lane 0
  @param[in]  DqCenter1     - Center point for DQ Lane 1
  @param[in]  DqCenter2     - Center point for DQ Lane 2
  @param[in]  DqCenter3     - Center point for DQ Lane 3
  @param[out] NibbleCenter  - Pointer to storage for nibble center

  @retval N/A

**/
VOID
CalculateNibbleCenter (
  IN  UINT16  DqCenter0,
  IN  UINT16  DqCenter1,
  IN  UINT16  DqCenter2,
  IN  UINT16  DqCenter3,
  OUT UINT8   *NibbleCenter
  )
{
  INT16 NibCenter;

  // Multiply Centers by 10 to better account for rounding errors
  NibCenter = (DqCenter0 * 10) + (DqCenter1 * 10) + (DqCenter2 * 10) + (DqCenter3 * 10);
  // Find average
  NibCenter = NibCenter / 4;

  // Apply rounding by adding/subtracting 5
  if (NibCenter > 0) {
    NibCenter += 5;
  } else {
    NibCenter -=5;
  }

  // Divide by 10 for final result
  NibCenter = NibCenter / 10;
  *NibbleCenter = (UINT8) NibCenter;
}

/**

  Evaluates and prints out the results from the Coarse Read Delay sweep and programs
  the Read Delay values.

  @param[in]  Host             - Pointer to sysHost structure
  @param[in]  Socket           - Socket number
  @param[in]  ChBitmask        - Channel bit mask
  @param[in]  Dimm             - Dimm number
  @param[in]  LrTrainRes       - Training results
  @param[out] PerNibbleCenter  - Stores final per-nibble results
  @param[out] PerBitResults    - Stores final per-bit results

  @retval Status - Non-zero if any failure detected

**/
MRC_STATUS
EvaluateAndProgramPerBitReadResults (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN struct    MdqTrainingResults (*LrTrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  OUT UINT8    PerNibbleCenter[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2],
  OUT UINT32   PerBitResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][SWIZZLE_RESULTS_LEN]
  )
{
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;
  UINT8       Channel;
  UINT8       MaxChDdr;
  UINT8       SubChannel;
  UINT8       Strobe;
  UINT8       DqLane;
  UINT8       SubChMSVx4;
  UINT16      PerBitRisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE];
  UINT16      PerBitCenterPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE];
  UINT16      PerBitFallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE];
  UINT16      PerBitPulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE];

  MaxChDdr = GetMaxChDdr ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }
    if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
      Host->var.mem.piSettingStopFlag[Channel] = 0;
      continue;
    }

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      DisplayPerBitSweepResults (Host, Socket, Channel, SubChannel, Dimm, Rank, LrTrainRes);
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
          EvaluatePerBitResults (Socket, Channel, SubChannel, Dimm, Rank, Strobe, DqLane, PerBitRisingEdge, PerBitCenterPoint,
            PerBitFallingEdge, PerBitPulseWidth, LrTrainRes);

          // Error handling for bad eye
          if (Host->nvram.mem.socket[Socket].FaultyPartsFlag[Channel] & (1 << Strobe)) {
            HandleFaultyPart (Host, Socket, Channel, Dimm, Rank, Strobe, PerBitPulseWidth[Channel][SubChannel][Strobe][DqLane]);
          }
        } // DqLane loop

        // All edges of DQ lanes for this strobe have been calculated.  Calculate the center point for the nibble
        CalculateNibbleCenter (PerBitCenterPoint[Channel][SubChannel][Strobe][0] - POSITIVE_ENCODE,
                               PerBitCenterPoint[Channel][SubChannel][Strobe][1] - POSITIVE_ENCODE,
                               PerBitCenterPoint[Channel][SubChannel][Strobe][2] - POSITIVE_ENCODE,
                               PerBitCenterPoint[Channel][SubChannel][Strobe][3] - POSITIVE_ENCODE,
                               &PerNibbleCenter[Channel][Dimm][SubChannel][Rank][Strobe]);

        // Apply same encode to nibble center point that is applied to DQ center points
        PerNibbleCenter[Channel][Dimm][SubChannel][Rank][Strobe] += POSITIVE_ENCODE;
      } // Strobe loop

      DisplayPerBitEdges (Host, Socket, Channel, SubChannel, PerBitPulseWidth[Channel], PerBitFallingEdge[Channel],
          PerBitRisingEdge[Channel], PerBitCenterPoint[Channel]);

      Status = CalculateAndProgramPerBitDelays (Host, Socket, Channel, Dimm, Rank, SubChannel, PerNibbleCenter,
          PerBitCenterPoint, PerBitResults);
      if (Status != MRC_STATUS_SUCCESS) {
        return Status;
      }
    } // SubChannel loop
  } // Channel loop

  return Status;
}

/**

  Program the final Write Delays per nibble.

  @param[in]      Host         - Pointer to sysHost structure
  @param[in]      Socket       - Socket number
  @param[in]      Channel      - Channel number
  @param[in]      SubChannel   - SubChannel number
  @param[in]      Dimm         - DIMM number
  @param[in]      Rank         - Rank number
  @param[in][out] CenterPoint  - Pointer to array which holds centered delay values

  @retval Status  - Non-zero if any failure detected

**/
MRC_STATUS
ProgramFinalWriteDelay (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Channel,
  IN UINT8      SubChannel,
  IN UINT8      Dimm,
  IN UINT8      Rank,
  IN OUT UINT8  PerNibbleCenter[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2]
  )
{
  MRC_STATUS                        Status;
  UINT8                             Strobe;
  DDR5_DATA_BUFFER_PG0_RWE6_STRUCT  LowerMdqDelayControl;
  DDR5_DATA_BUFFER_PG0_RWE7_STRUCT  UpperMdqDelayControl;
  UINT8                             WriteDelayTemp;
  UINT8                             MaxStrobePairs;
  UINT8                             SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  DDR5_LRDIMM_TRAINING_DATA         *TrainingData;
  BOOLEAN                           (*NibbleSwap)[MAX_CH][MAX_DIMM][SUB_CH][MAX_STROBE_DDR5 / 2];
  struct dimmNvram                  (*DimmNvList) [MAX_DIMM];
  struct rankDevice                 (*RankStruct) [MAX_RANK_DIMM];

  RankStruct = GetRankStruct (Host, Socket, Channel, Dimm);
  // Retrieve nibble swap info from MRE
  Status = GetDdr5LrdimmTrainingData (&TrainingData);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }
  NibbleSwap = &(TrainingData->NibbleSwap);
  LowerMdqDelayControl.Data = 0;
  UpperMdqDelayControl.Data = 0;
  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  MaxStrobePairs = SubChMSVx4 / 2;
  for (Strobe = 0; Strobe < MaxStrobePairs; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    LowerMdqDelayControl.Bits.phase_control_for_lower_nibble_mdq_with = (UINT8) PerNibbleCenter[Channel][Dimm][SubChannel][Rank][Strobe];
    UpperMdqDelayControl.Bits.phase_control_for_upper_nibble_mdq_with = (UINT8) PerNibbleCenter[Channel][Dimm][SubChannel][Rank][Strobe + MaxStrobePairs];

    // If there is nibble swap, low nibble MDQ center = high nibble MDQ center and vice versa
    if ((*NibbleSwap)[Channel][Dimm][SubChannel][Strobe] == TRUE) {
      WriteDelayTemp = LowerMdqDelayControl.Data;
      LowerMdqDelayControl.Data = UpperMdqDelayControl.Data;
      PerNibbleCenter[Channel][Dimm][SubChannel][Rank][Strobe] = PerNibbleCenter[Channel][Dimm][SubChannel][Rank][Strobe + MaxStrobePairs];
      UpperMdqDelayControl.Data = WriteDelayTemp;
      PerNibbleCenter[Channel][Dimm][SubChannel][Rank][Strobe + MaxStrobePairs] = WriteDelayTemp;
      RcDebugPrint (SDBG_MAX, "Nibble Swap Detected:  SubCh%d  Strobe%d\n", SubChannel, Strobe);
    }

    // Select appropriate buffer device
    DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, Strobe);

    DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE6_REG, LowerMdqDelayControl.Data);
    DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE7_REG, UpperMdqDelayControl.Data);
    //
    // Save trained setting to Cache and NVRAM, format will be as below, and stored setting is after Nibble Swapped
    //                          Subch 0            |      Subch 1
    // LrBuf_PG01RWE6E7:  Strobe 0-4: Lower Nibble | Strobe 10-14: Lower Nibble
    // LrBuf_PG01RWE6E7:  Strobe 5-9: Upper Nibble | Strobe 15-19: Upper Nibble
    //
    (*DimmNvList )[Dimm].rankList[Rank].LrBuf_PG01RWE6E7[Strobe + (SubChannel * SubChMSVx4)] = LowerMdqDelayControl.Data;
    (*DimmNvList) [Dimm].rankList[Rank].LrBuf_PG01RWE6E7[Strobe + MaxStrobePairs + (SubChannel * SubChMSVx4)] = UpperMdqDelayControl.Data;
    (*RankStruct) [Rank].cachedLrBuf_PG01RWE6E7[Strobe + (SubChannel * SubChMSVx4)] = LowerMdqDelayControl.Data;
    (*RankStruct) [Rank].cachedLrBuf_PG01RWE6E7[Strobe + MaxStrobePairs + (SubChannel * SubChMSVx4)] = UpperMdqDelayControl.Data;
  }
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);
  return MRC_STATUS_SUCCESS;
} // ProgramFinalWriteDelay

/**

  Programs the Per-DQ write values to the Data Buffer.

  @param[in]  Host           - Pointer sysHost
  @param[in]  Socket         - Socket number
  @param[in]  Channel        - Channel number
  @param[in]  SubChannel     - SubChannel number
  @param[in]  Dimm           - DIMM number
  @param[in]  Rank           - Rank number on DIMM
  @param[in]  DqOffset       - Array containing DQ offsets to be programmed
  @param[out] PerBitResults  - Stores final per-bit results

  @retval Status  - Non-zero if any failure detected

**/
MRC_STATUS
ProgramPerBitWriteDelay (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     SubChannel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN INT8      DqOffset[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE],
  OUT UINT32   PerBitResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][SWIZZLE_RESULTS_LEN]
  )
{
  MRC_STATUS                 Status = MRC_STATUS_SUCCESS;
  UINT8                      LowerDword;
  UINT8                      UpperDword;
  UINT8                      Strobe;
  UINT8                      DqLane;
  UINT8                      LowerHostDqLane;
  UINT8                      UpperHostDqLane;
  INT8                       LowerNibbleDqOffset;
  INT8                       UpperNibbleDqOffset;
  UINT8                      MaxStrobePairs;
  UINT8                      MdqRegValue;
  UINT8                      DbRegisterAddress;
  DDR5_LRDIMM_TRAINING_DATA  *TrainingData;
  UINT32                      (*HostToDbDqMap)[MAX_CH][MAX_DIMM][SUB_CH][SWIZZLE_RESULTS_LEN];

  // Retrieve nibble swap info
  Status = GetDdr5LrdimmTrainingData (&TrainingData);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }
  HostToDbDqMap = &(TrainingData->HostToDbDqMap);

  MaxStrobePairs = GetSubChMaxStrobeValid (Host) / 2;

  for (Strobe = 0; Strobe < MaxStrobePairs; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    // Select appropriate buffer device
    DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, Strobe);

    for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
      // Determine mapping of DB DQ Lane to Host DQ Lane.  If, for example, DB DQ 0 maps to Host DQ 3, then the DQ
      // results we calculated for DQ 3, should be programmed to the DB DQ 0 register.
      LowerDword = (DqLane + (Strobe * DB_DQ_LANE)) / 10;                                       // Each Dword can fit 10 entries (3 bits per entry)
      UpperDword = ((DqLane + BITS_PER_NIBBLE) + (Strobe * DB_DQ_LANE)) / 10;                   // Find appropriate Dword based on current strobe and DQ bit
      LowerHostDqLane = ((*HostToDbDqMap)[Channel][Dimm][SubChannel][LowerDword] >>
          ((DqLane + (Strobe * DB_DQ_LANE)) % 10) * 3) & SWIZZLE_BIT_MASK;                     // Determine which of the 10 entries of the Dword is being examined,
      UpperHostDqLane = ((*HostToDbDqMap)[Channel][Dimm][SubChannel][UpperDword] >>            // mulitply by 3 (3 bits per entry) to find correct entry, mask off
          (((DqLane + BITS_PER_NIBBLE) + (Strobe * DB_DQ_LANE)) % 10) * 3) & SWIZZLE_BIT_MASK; // all other bits to find the record we care about

      // If lower nibble DQ lanes on Host map to lower nibble DQ lanes on DB, then no nibble swapping necessary
      if (LowerHostDqLane < BITS_PER_NIBBLE) {
        LowerNibbleDqOffset = DqOffset[Channel][SubChannel][Strobe][LowerHostDqLane];
        UpperNibbleDqOffset = DqOffset[Channel][SubChannel][Strobe + MaxStrobePairs][UpperHostDqLane - BITS_PER_NIBBLE];
      } else {
        // Lower nibble DQ lanes on Host map to upper nibble DQ lanes on DB
        LowerNibbleDqOffset = DqOffset[Channel][SubChannel][Strobe + MaxStrobePairs][LowerHostDqLane - BITS_PER_NIBBLE];
        UpperNibbleDqOffset = DqOffset[Channel][SubChannel][Strobe][UpperHostDqLane];
      }

      SavePerBitResults (Channel, SubChannel, Dimm, Rank, Strobe, LowerDword, UpperDword, DqLane,
          LowerNibbleDqOffset, UpperNibbleDqOffset, PerBitResults);

      // Lower MDQ bits (0-3) are programmed in the lower 4 bits of control word
      MdqRegValue = GetPerBitDelayRegValue (LowerNibbleDqOffset);
      // Upper MDQ bits (4-7) are programmed in the upper 4 bits of control word
      MdqRegValue |= (GetPerBitDelayRegValue (UpperNibbleDqOffset) << 4);

      // Select appropriate register based on DQ Lane
      switch (DqLane) {
        case 0:
          DbRegisterAddress = DDR5_DATA_BUFFER_PG0_RWEE_REG;
          break;

        case 1:
          DbRegisterAddress = DDR5_DATA_BUFFER_PG0_RWEF_REG;
          break;

        case 2:
          DbRegisterAddress = DDR5_DATA_BUFFER_PG0_RWF0_REG;
          break;

        case 3:
          DbRegisterAddress = DDR5_DATA_BUFFER_PG0_RWF1_REG;
          break;

        default:
          // We should never hit this
          FatalError (ERR_INVALID_REG_ACCESS, ERR_UNKNOWN_REG_TYPE);
          break;
      }

      DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DbRegisterAddress, MdqRegValue);
    } // DqLane loop
  } // Strobe loop

  // Select all data buffers now that programming is done
  DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);

  return Status;
}
/**

  Calculates and programs ODTLon_WR_Offset and ODTLoff_WR_Offset values.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] SubChannel       - SubChannel number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number on DIMM
  @param[in] PerNibbleCenter  - Array containing centered write delay values
  @param[in] DqOffset         - Array containing per DQ lane offsets from nibble center
  @param[out] OdtlOnResults   - Stores final DRAM_ODTLON results
  @param[out] OdtlOffResults  - Stores final DRAM_ODTLOFF results

  @retval N/A

**/
VOID
CalculateOdtlValues (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     SubChannel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     PerNibbleCenter[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2],
  IN INT8      DqOffset[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE],
  OUT INT8     OdtlOnResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2],
  OUT INT8     OdtlOffResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8                         Strobe;
  UINT16                        Dq0Delay;
  UINT16                        Dq1Delay;
  UINT16                        Dq2Delay;
  UINT16                        Dq3Delay;
  UINT16                        MinRxDqsDq;
  UINT16                        MaxRxDqsDq;
  INT16                         OdtlOnWrOffset;
  INT16                         OdtlOffWrOffset;
  DDR5_MODE_REGISTER_37_STRUCT  OdtlMr37;
  UINT8                         MaxStrobe = GetSubChMaxStrobeValid (Host);

  OdtlMr37.Data = 0;

  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    OdtlOnWrOffset = 0;
    OdtlOffWrOffset = 0;
    // a. tRx_DQS2DQ_DQ0: MDQ baseline write delay + MDQ0 write delay
    // b. tRx_DQS2DQ_DQ1: MDQ baseline write delay + MDQ0 write delay
    // c. tRx_DQS2DQ_DQ2: MDQ baseline write delay + MDQ0 write delay
    // d. tRx_DQS2DQ_DQ3: MDQ baseline write delay + MDQ0 write delay
    // Note: step (tick) size is in 1/64 CLK increments. Convert value to 1/128 scale.

    Dq0Delay = (PerNibbleCenter[Channel][Dimm][SubChannel][Rank][Strobe] + DqOffset[Channel][SubChannel][Strobe][0]) % MWD_SCALE;
    Dq1Delay = (PerNibbleCenter[Channel][Dimm][SubChannel][Rank][Strobe] + DqOffset[Channel][SubChannel][Strobe][1]) % MWD_SCALE;
    Dq2Delay = (PerNibbleCenter[Channel][Dimm][SubChannel][Rank][Strobe] + DqOffset[Channel][SubChannel][Strobe][2]) % MWD_SCALE;
    Dq3Delay = (PerNibbleCenter[Channel][Dimm][SubChannel][Rank][Strobe] + DqOffset[Channel][SubChannel][Strobe][3]) % MWD_SCALE;

    // Min_tRx_DQS2DQ = MIN (tRx_DQS2DQ_DQ0, tRx_DQS2DQ_DQ1, tRx_DQS2DQ_DQ2, tRx_DQS2DQ_DQ3)
    // Max_tRx_DQS2DQ = MAX (tRx_DQS2DQ_DQ0, tRx_DQS2DQ_DQ1, tRx_DQS2DQ_DQ2, tRx_DQS2DQ_DQ3)

    MinRxDqsDq = MIN (Dq0Delay, Dq1Delay);
    MinRxDqsDq = MIN (MinRxDqsDq, Dq2Delay);
    MinRxDqsDq = MIN (MinRxDqsDq, Dq3Delay);
    MaxRxDqsDq = MAX (Dq0Delay, Dq1Delay);
    MaxRxDqsDq = MAX (MaxRxDqsDq, Dq2Delay);
    MaxRxDqsDq = MAX (MaxRxDqsDq, Dq3Delay);

    // 2. Perform the following calculations to obtain the ODTLon_WR_offset:
    //  ODTLon_WR_offset _ticks = tDQSoffset_max * 128 +
    //                            tADC_max * 128 +
    //                            Min_tRx_DQS2DQ * 2 +
    //                            tRx_DQS2DQ_Drift_Guardband
    // Where:
    //  tDQSoffset_max = -0.5 (or the max delta across all strobes between external WL and internal WL)
    //  tADC_max = -0.8
    //  tRx_DQS2DQ_Drift_Guardband = -10 ticks Note: Guardband can be tuned based on Post-Silicon Characterization.
    // ODTLon_WR_offset = RoundUP(ODTLon_WR_offset / 128)

    // Factors are multiplied by 10 to better account for rounding
    OdtlOnWrOffset =  ((128 * ODTON_TDQSOFFSET_MAX) + (128 * TADC_MAX) + ((MinRxDqsDq * 10) * 2) + ODTON_DRIFT_GUARD);
    OdtlOnWrOffset = OdtlOnWrOffset / 128;
    // Round up if value is not a factor of 10 and number is positive. Negative numbers will be rounded up by the divide by 10
    if ((OdtlOnWrOffset % 10 != 0) && (OdtlOnWrOffset > 0)) {
      OdtlOnWrOffset += 10;
    }
    // Final rounded value
    OdtlOnWrOffset = OdtlOnWrOffset / 10;
    OdtlOnResults[Channel][Dimm][SubChannel][Rank][Strobe] = (INT8) OdtlOnWrOffset;
    GetOdtlOnRegValue (Socket, Channel, Strobe, OdtlOnWrOffset, &OdtlMr37);

    // 3. Perform the following calculations to obtain the ODTLoff_WR_offset:
    //    ODTLoff_WR_offset _ticks = tDQSoffset_max * 128 +
    //                               Max_tRx_DQS2DQ * 2 +
    //                               tRx_DQS2DQ_Drift_Guardband
    // Where:
    //  tDQSoffset_max = +0.5 (or the max delta across all strobes between external WL and internal WL)
    //  tRx_DQS2DQ_Drift_Guardband = +20 ticks Note: Guardband can be tuned based on Post-Silicon Characterization.
    // ODTLoff_WR_offset = RoundUP(ODTLoff_WR_offset / 128)
    OdtlOffWrOffset = ((128 * ODTOFF_TDQSOFFSET_MAX) + ((MaxRxDqsDq * 10) * 2) + ODTOFF_DRIFT_GUARD);
    OdtlOffWrOffset = OdtlOffWrOffset / 128;
    // Round up if value is not a factor of 10 and number is positive. Negative numbers will be rounded up by the divide by 10
    if ((OdtlOffWrOffset % 10 != 0) && (OdtlOffWrOffset > 0)) {
      OdtlOffWrOffset += 10;
    }
    // Final rounded value
    OdtlOffWrOffset = OdtlOffWrOffset / 10;
    OdtlOffResults[Channel][Dimm][SubChannel][Rank][Strobe] = (INT8) OdtlOffWrOffset;
    GetOdtlOffRegValue (Socket, Channel, Strobe, OdtlOffWrOffset, &OdtlMr37);

    // Use PDA to target appropriate DRAM device
    DramModeRegWritePda (Socket, Channel, (1 << SubChannel), Dimm, Rank, Strobe, DDR5_MODE_REGISTER_37_REG, OdtlMr37.Data);
  }

}

/**

  Handles the calculations for finding the per-bit Write Delay offsets and programming
  these values.

  @param[in]      Host               - Pointer to sysHost
  @param[in]      Socket             - Socket number
  @param[in]      Channel            - Channel number
  @param[in]      Dimm               - DIMM number
  @param[in]      Rank               - Rank number on DIMM
  @param[in]      SubChannel         - SubChannel number
  @param[in][out] PerNibbleCenter    - Array containing Nibble center points for Write Delay
  @param[in]      PerBitCenterPoint  - Array containing per-DQ center points for Write Delay
  @param[out]     PerBitResults      - Stores final per-bit results
  @param[out]     OdtlOnResults     - Stores final DRAM_ODTLON results
  @param[out]     OdtlOffResults    - Stores final DRAM_ODTLOFF results

  @retval Status - Non-zero if any failure detected

**/
MRC_STATUS
CalculateAndProgramWriteBitDelays (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Channel,
  IN UINT8      Dimm,
  IN UINT8      Rank,
  IN UINT8      SubChannel,
  IN OUT UINT8  PerNibbleCenter[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2],
  IN UINT16     PerBitCenterPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE],
  OUT UINT32    PerBitResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][SWIZZLE_RESULTS_LEN],
  OUT INT8      OdtlOnResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2],
  OUT INT8      OdtlOffResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2]
  )
{
  MRC_STATUS  Status;
  UINT8       Strobe;
  UINT8       DqLane;
  UINT8       MaxStrobe = GetSubChMaxStrobeValid (Host);
  INT8        DqOffset[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE];

  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
      // Store the offset for this DQ lane based on calculated center point for the nibble it resides in
      DqOffset[Channel][SubChannel][Strobe][DqLane] =
          (INT8) (PerBitCenterPoint[Channel][SubChannel][Strobe][DqLane] - PerNibbleCenter[Channel][Dimm][SubChannel][Rank][Strobe]);

      CheckDqOffsetRange (Socket, Channel, Strobe, DqLane, &(DqOffset[Channel][SubChannel][Strobe][DqLane]));
    } // DqLane loop
  } // Strobe loop

  DisplayMdqOffsets (Host, Socket, Channel, Dimm, Rank, SubChannel, PerNibbleCenter, DqOffset);

  // Program the read delay per nibble
  Status = ProgramFinalWriteDelay (Host, Socket, Channel, SubChannel, Dimm, Rank, PerNibbleCenter);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }

  // Program the read delay per MDQ
  Status = ProgramPerBitWriteDelay (Host, Socket, Channel, SubChannel, Dimm, Rank, DqOffset, PerBitResults);

  CalculateOdtlValues (Host, Socket, Channel, SubChannel, Dimm, Rank, PerNibbleCenter, DqOffset, OdtlOnResults, OdtlOffResults);

  return Status;
}

/**

  Evaluates and prints out the results from the Write Delay sweep and programs
  the Write Delay values.

  @param[in]  Host             - Pointer to sysHost structure
  @param[in]  Socket           - Socket number
  @param[in]  ChBitmask        - Channel bit mask
  @param[in]  Dimm             - DIMM number
  @param[in]  Rank             - Rank number on DIMM
  @param[in]  LrTrainRes       - Training results
  @param[out] PerNibbleCenter  - Stores final per-nibble results
  @param[out] PerBitResults    - Stores final per-bit results
  @param[out] OdtlOnResults    - Stores final DRAM_ODTLON results
  @param[out] OdtlOffResults   - Stores final DRAM_ODTLOFF results

  @retval Status - Non-zero if any failure detected

**/
MRC_STATUS
EvaluateAndProgramWriteDelayResults (
  IN PSYSHOST                   Host,
  IN UINT8                      Socket,
  IN UINT32                     ChBitmask,
  IN UINT8                      Dimm,
  IN UINT8                      Rank,
  IN struct MdqTrainingResults  (*LrTrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  OUT UINT8                     PerNibbleCenter[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2],
  OUT UINT32                    PerBitResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][SWIZZLE_RESULTS_LEN],
  OUT INT8                      OdtlOnResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2],
  OUT INT8                      OdtlOffResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2]
  )
{
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;
  UINT8       Channel;
  UINT8       MaxChDdr;
  UINT8       SubChannel;
  UINT8       Strobe;
  UINT8       DqLane;
  UINT8       SubChMSVx4;
  UINT16      PerBitRisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE];
  UINT16      PerBitCenterPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE];
  UINT16      PerBitFallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE];
  UINT16      PerBitPulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][BITS_PER_NIBBLE];

  MaxChDdr = GetMaxChDdr ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }
    if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
      Host->var.mem.piSettingStopFlag[Channel] = 0;
      continue;
    }

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      DisplayWriteDelaySweepResults (Host, Socket, Channel, SubChannel, Dimm, Rank, LrTrainRes);
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        for (DqLane = 0; DqLane < BITS_PER_NIBBLE; DqLane++) {
          EvaluatePerBitResults (Socket, Channel, SubChannel, Dimm, Rank, Strobe, DqLane, PerBitRisingEdge, PerBitCenterPoint,
            PerBitFallingEdge, PerBitPulseWidth, LrTrainRes);

          // Error handling for bad eye
          if (Host->nvram.mem.socket[Socket].FaultyPartsFlag[Channel] & (1 << Strobe)) {
            HandleFaultyPart (Host, Socket, Channel, Dimm, Rank, Strobe, PerBitPulseWidth[Channel][SubChannel][Strobe][DqLane]);
          }
        } // DqLane loop

        // All edges of DQ lanes for this strobe have been calculated.  Calculate the center point for the nibble
        CalculateNibbleCenter (PerBitCenterPoint[Channel][SubChannel][Strobe][0],
                               PerBitCenterPoint[Channel][SubChannel][Strobe][1],
                               PerBitCenterPoint[Channel][SubChannel][Strobe][2],
                               PerBitCenterPoint[Channel][SubChannel][Strobe][3],
                               &PerNibbleCenter[Channel][Dimm][SubChannel][Rank][Strobe]);
      } // Strobe loop

      DisplayPerBitEdges (Host, Socket, Channel, SubChannel, PerBitPulseWidth[Channel], PerBitFallingEdge[Channel],
          PerBitRisingEdge[Channel], PerBitCenterPoint[Channel]);

      Status = CalculateAndProgramWriteBitDelays (Host, Socket, Channel, Dimm, Rank, SubChannel, PerNibbleCenter,
          PerBitCenterPoint, PerBitResults, OdtlOnResults, OdtlOffResults);
      if (Status != MRC_STATUS_SUCCESS) {
        return Status;
      }
    } // SubChannel loop
  } // Channel loop
  return Status;
}

/**

  Cleanup LRDIMM Training Parameters.

  @param[in] Host       - Pointer to SysHost structure
  @param[in] Socket     - Socket number
  @param[in] ChBitmask  - Channel bit mask
  @param[in] Dimm       - Dimm number
  @param[in] Rank       - Rank number

  @retval N/A

**/
VOID
LrdimmTrainingCleanup (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  UINT8                         Channel;
  UINT8                         MaxChDdr;
  DDR5_DATA_BUFFER_RW97_STRUCT  BufferTrainingConfig;

  MaxChDdr = GetMaxChDdr ();

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    SetMprModeFilterActPre (Host, Socket, Channel, 0);
    ConvertRdToMrr (Host, Socket, Channel, 0);

    if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    //
    // Select all buffer devices
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);

    // Exit Training Mode
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW83_REG, LRDIMM_EXIT_TRAINING);

    // Clear training feedback, disable Long Read pattern sticky status feedback mode
    BufferTrainingConfig.Data = 0;
    BufferTrainingConfig.Bits.clear_feedback_status_bit = 1;
    BufferTrainingConfig.Bits.long_read_pattern_sticky_status_feedback_mode = 1;
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW97_REG, BufferTrainingConfig.Data);

    // Restore Data Buffer page to 0
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, 0);
  } //Ch
}

/**

  Perform LRDIMM MDQS Read Delay Training.

  @param[in] Socket  - Socket number

  @retval Status  - Non-zero if any failure detected

**/
STATIC
MRC_STATUS
CoarseReadDelayTraining (
  IN UINT8  Socket
  )
{
  UINT8                       Channel;
  UINT8                       SubChannel;
  UINT8                       Dimm;
  UINT8                       Rank;
  UINT32                      ChBitmask;
  MRC_STATUS                  Status = MRC_STATUS_SUCCESS;
  struct TrainingResultsDdr5  (*TrainRes)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  ACT_TO_ACT_SAVE_STRUCT      ActToActSave[MAX_CH];
  UINT8                       MaxChDdr;
  UINT8                       ChannelLocal;
  PSYSHOST                    Host = GetSysHostPointer ();

  MaxChDdr = GetMaxChDdr ();

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "DDR5 LRDIMM MDQS Coarse Read Delay Training\n");

  SetCurrentTestType (Socket, MdqsCoarseReadDelayTraining);
  SetAepTrainingMode (Host, Socket, CHECKPOINT_MINOR_RX_BACKSIDE_PHASE_TRAINING);
  TrainRes = RcAllocatePool (sizeof (*TrainRes));

  //
  // Train each DIMM
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Train each rank
    //
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_RX_BACKSIDE_DELAY_TRAINING, (UINT16) ((Socket << 8) | (Dimm << 4) |Rank)));
      //
      // Create a bit mask of channels that have at least 1 rank present for this dimm and rank
      //
      ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, Rank);
      if (ChBitmask == 0) {
        continue;
      }

      ZeroMem ((UINT8 *) TrainRes, sizeof (*TrainRes));

      // Program timings guaranteed to generate back-to-back reads
      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }

        ActToActSave[Channel].TRrdSOrg = 0;
        ActToActSave[Channel].TRrdLOrg = 0;
        ActToActSave[Channel].TRcdRdOrg = 0;
        ActToActSave[Channel].TRrsrOrg = 0;
        ActToActSave[Channel].TRrSgOrg = 0;

        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          ChannelLocal = Channel;
          GetChannelMapping (Channel, SubChannel, &ChannelLocal);
          SetActToActPeriod (Host, Socket, ChannelLocal, ActToActSave);
        }
      }

      ChBitmask = SkipDdrtCh (Socket, Dimm, ChBitmask);

      // a. Configure the DDRIO for sense amp.  This step is performed while collecting data

      // b. Configure the CPGC pattern for the target rank.  Here we will perform the CPGC setup necessary
      //    for this step, but we will wait to program the pattern so the configuration doesn't get over-written
      //    by SendMrw commands.
      SetupCpgcForReadDelay (Host, Socket, ChBitmask, Dimm, Rank);

      // c. Configure the pattern generated by the DRAM (MR25 to MR30)
      // d. Configure the pattern on the DB (snooped through DRAM programming)
      // e. Set the DB training config (RW97)
      // f. Enable MRD training mode on DB
      LrdimmCoarseMrdInit (Host, Socket, ChBitmask, Dimm, Rank);

      // g. Sweep MDQS delay on both low and high nibbles from -15/64 to +15/64 in 1/64 increments.
      SweepReadDelay (Host, Socket, ChBitmask, Dimm, Rank, TrainRes);

      // h. For each byte calculate the center of low and high nibbles taking into account nibble swap
      // i. Print out the final results
      // l. (steps j and k are performed below) Program the results for each byte
      Status = EvaluateAndProgramReadDelayResults (Host, Socket, ChBitmask, Dimm, Rank, TrainRes);

      // j. Disable MRD Training Mode
      // k. Disable the Long Read Pattern Sticky Status feedback mode
      LrdimmTrainingCleanup (Host, Socket, ChBitmask, Dimm, Rank);

      // Restore timing values
      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }

        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          ChannelLocal = Channel;
          GetChannelMapping (Channel, SubChannel, &ChannelLocal);
          RestoreActToActPeriod (Socket, ChannelLocal, ActToActSave);
        }
      }
    } //rank
  } //dimm

  RcFreePool (TrainRes);
  SetAepTrainingMode (Host,Socket, DISABLE_TRAINING_STEP);
  FifoTrainReset (Host, Socket);
  return Status;
} // CoarseReadDelayTraining

/**

  Perform LRDIMM DQ Swizzle Discovery.

  @param[in] Socket  - Socket number

  @retval Status  - Non-zero if any failure detected

**/
STATIC
MRC_STATUS
BacksideSwizzleDiscovery (
  IN UINT8  Socket
  )
{
  UINT8                      Channel;
  UINT8                      SubChannel;
  UINT8                      Dimm;
  UINT8                      Rank;
  UINT32                     ChBitmask;
  MRC_STATUS                 Status = MRC_STATUS_SUCCESS;
  ACT_TO_ACT_SAVE_STRUCT     ActToActSave[MAX_CH];
  PSYSHOST                   Host = GetSysHostPointer ();
  UINT8                      MaxChDdr;
  UINT8                      ChannelLocal;
  DDR5_LRDIMM_TRAINING_DATA  *TrainingData;
  UINT32                     (*DbToDramDqMap)[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][SUB_CH][SWIZZLE_RESULTS_LEN];
  BOOLEAN                    (*NibbleSwap)[MAX_CH][MAX_DIMM][SUB_CH][MAX_STROBE_DDR5 / 2];

  MaxChDdr = GetMaxChDdr ();

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "DDR5 LRDIMM Backside Swizzle Discovery\n");

  SetCurrentTestType (Socket, BacksideDqSwizzleDiscovery);

  Status = GetDdr5LrdimmTrainingData (&TrainingData);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }
  DbToDramDqMap = &(TrainingData->DbToDramDqMap);
  NibbleSwap = &(TrainingData->NibbleSwap);

  // Train each DIMM
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    // Train each rank
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_BACKSIDE_SWIZZLE, (UINT16) ((Socket << 8) | (Dimm << 4) |Rank)));

      //
      // Create a bit mask of channels that have at least 1 rank present for this dimm and rank
      //
      ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, Rank);
      if (ChBitmask == 0) {
        continue;
      }

      // Program timings guaranteed to generate back-to-back reads
      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }

        ActToActSave[Channel].TRrdSOrg = 0;
        ActToActSave[Channel].TRrdLOrg = 0;
        ActToActSave[Channel].TRcdRdOrg = 0;
        ActToActSave[Channel].TRrsrOrg = 0;
        ActToActSave[Channel].TRrSgOrg = 0;

        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          ChannelLocal = Channel;
          GetChannelMapping (Channel, SubChannel, &ChannelLocal);
          SetActToActPeriod (Host, Socket, ChannelLocal, ActToActSave);
        }
      }

      ChBitmask = SkipDdrtCh (Socket, Dimm, ChBitmask);

      // a. Configure the DDRIO for sense amp. This step is performed while collecting data

      // b. Configure the CPGC pattern for the target rank.  Here we will perform the CPGC setup necessary
      //    for this step, but we will wait to program the pattern so the configuration doesn't get over-written
      //    by SendMrw commands. CPGC setup is the same as it was for Coarse Read Delay.
      SetupCpgcForReadDelay (Host, Socket, ChBitmask, Dimm, Rank);

      // c. Configure the DB Buffer Training Configuration and enable MRD mode.
      // d. Configure the pattern generated by the DRAM (MR25 to MR30).
      // e. Configure the pattern generated by the DB (RW99 to RW9D).
      LrdimmBacksideSwizzleInit (Host, Socket, ChBitmask, Dimm, Rank);

      // f. Backside Swizzle Discovery
      BacksideSwizzleSweep (Host, Socket, ChBitmask, Dimm, Rank, DbToDramDqMap, NibbleSwap);

      DisplayBacksideSwizzleResults (Host, Socket, Dimm, Rank, DbToDramDqMap);

      // g. Disable MRD Training Mode
      // h. Disable the Long Read Pattern Sticky Status feedback mode
      LrdimmTrainingCleanup (Host, Socket, ChBitmask, Dimm, Rank);

      // Restore timing values
      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }

        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          ChannelLocal = Channel;
          GetChannelMapping (Channel, SubChannel, &ChannelLocal);
          RestoreActToActPeriod (Socket, ChannelLocal, ActToActSave);
        }
      }
    } // Rank loop
  } // DIMM loop

  return Status;
}

/**

  Perform LRDIMM DQ frontside (Host to Data Buffer) Swizzle Discovery.

  @param[in] Socket  - Socket number

  @retval Status  - Non-zero if any failure detected

**/
STATIC
MRC_STATUS
FrontsideSwizzleDiscovery (
  IN UINT8  Socket
  )
{
  UINT8                       Channel;
  UINT8                       SubChannel;
  UINT8                       Dimm;
  UINT8                       Rank;
  UINT32                      ChBitmask;
  MRC_STATUS                  Status = MRC_STATUS_SUCCESS;
  ACT_TO_ACT_SAVE_STRUCT      ActToActSave[MAX_CH];
  PSYSHOST                    Host = GetSysHostPointer ();
  UINT8                       MaxChDdr;
  UINT8                       ChannelLocal;
  DDR5_LRDIMM_TRAINING_DATA   *TrainingData;
  UINT32                      (*HostToDbDqMap)[MAX_CH][MAX_DIMM][SUB_CH][SWIZZLE_RESULTS_LEN];

  MaxChDdr = GetMaxChDdr ();

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "DDR5 LRDIMM Frontside Swizzle Discovery\n");

  SetCurrentTestType (Socket, FrontsideDqSwizzleDiscovery);

  Status = GetDdr5LrdimmTrainingData (&TrainingData);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }
  HostToDbDqMap = &(TrainingData->HostToDbDqMap);

  //
  // Discovery for each DIMM
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    // Frontside Swizzle does not need to be discovered on a per-rank basis.  Setting rank to 0 so a valid rank is passed
    // into sub-functions.
    Rank = 0;
    OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_FRONTSIDE_SWIZZLE, (UINT16) ((Socket << 8) | (Dimm << 4) |Rank)));

    //
    // Create a bit mask of channels that have at least 1 rank present for this dimm and rank
    //
    ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, Rank);
    if (ChBitmask == 0) {
      continue;
    }

    // Program timings guaranteed to generate back-to-back reads
    for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
      if (!((1 << Channel) & ChBitmask)) {
        continue;
      }

      ActToActSave[Channel].TRrdSOrg = 0;
      ActToActSave[Channel].TRrdLOrg = 0;
      ActToActSave[Channel].TRcdRdOrg = 0;
      ActToActSave[Channel].TRrsrOrg = 0;
      ActToActSave[Channel].TRrSgOrg = 0;

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        ChannelLocal = Channel;
        GetChannelMapping (Channel, SubChannel, &ChannelLocal);
        SetActToActPeriod (Host, Socket, ChannelLocal, ActToActSave);
      }
    }

    ChBitmask = SkipDdrtCh (Socket, Dimm, ChBitmask);

    // a. Configure the DDRIO for sense amp. This step is performed while collecting data

    // b. Configure the CPGC pattern for the target rank.  Here we will perform the CPGC setup necessary
    //    for this step, but we will wait to program the pattern so the configuration doesn't get over-written
    //    by SendMrw commands. CPGC setup is the same as it was for Coarse Read Delay.
    SetupCpgcForReadDelay (Host, Socket, ChBitmask, Dimm, Rank);

    // c. Set the DB Buffer training configuration with 'per-bit' results
    // d. Enable MRD Training Mode on the DB
    // e. Set all DRAMs to drive the same pattern on all DQs with no pattern inversion
    LrdimmFrontsideSwizzleInit (Host, Socket, ChBitmask, Dimm, Rank);

    // f. Sweep for results
    FrontsideSwizzleSweep (Host, Socket, ChBitmask, Dimm, Rank, HostToDbDqMap);

    DisplayFrontsideSwizzleResults (Host, Socket, Dimm, HostToDbDqMap);

    // g. Disable MRD Training Mode
    // h. Disable the Long Read Pattern Sticky Status feedback mode
    LrdimmTrainingCleanup (Host, Socket, ChBitmask, Dimm, Rank);

    // Restore timing values
    for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
      if (!((1 << Channel) & ChBitmask)) {
        continue;
      }

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        ChannelLocal = Channel;
        GetChannelMapping (Channel, SubChannel, &ChannelLocal);
        RestoreActToActPeriod (Socket, ChannelLocal, ActToActSave);
      }
    }
  } // Dimm loop

  return Status;
}

/**

  Perform LRDIMM Per-Bit MDQS Read Delay Deskew training.

  @param[in] Socket  - Socket number

  @retval Status  - Non-zero if any failure detected

**/
STATIC
MRC_STATUS
PerBitReadDelayTraining (
  IN UINT8  Socket
  )
{
  MRC_STATUS                  Status = MRC_STATUS_SUCCESS;
  UINT8                       MaxChDdr;
  UINT8                       Channel;
  UINT32                      ChBitmask;
  UINT8                       SubChannel;
  UINT8                       ChannelLocal;
  UINT8                       Dimm;
  UINT8                       Rank;
  ACT_TO_ACT_SAVE_STRUCT      ActToActSave[MAX_CH];
  PSYSHOST                    Host = GetSysHostPointer ();
  struct MdqTrainingResults   (*TrainRes)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT8                       PerNibbleCenter[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2];
  UINT32                      PerBitResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][SWIZZLE_RESULTS_LEN];

  MaxChDdr = GetMaxChDdr ();

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "DDR5 LRDIMM Per-Bit Read Delay Training\n");

  SetCurrentTestType (Socket, MdqsReadDelayDeskew);
  TrainRes = RcAllocatePool (sizeof (*TrainRes));
  ZeroMem ((UINT8 *) PerBitResults, sizeof (PerBitResults));

  //
  // Train each DIMM
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Train each rank
    //
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_BACKSIDE_RX_PER_BIT_DESKEW,
          (UINT16) ((Socket << 8) | (Dimm << 4) |Rank)));

      //
      // Create a bit mask of channels that have at least 1 rank present for this dimm and rank
      //
      ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, Rank);
      if (ChBitmask == 0) {
        continue;
      }

      ZeroMem ((UINT8 *) TrainRes, sizeof (*TrainRes));

      // Program timings guaranteed to generate back-to-back reads
      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }

        ActToActSave[Channel].TRrdSOrg = 0;
        ActToActSave[Channel].TRrdLOrg = 0;
        ActToActSave[Channel].TRcdRdOrg = 0;
        ActToActSave[Channel].TRrsrOrg = 0;
        ActToActSave[Channel].TRrSgOrg = 0;

        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          ChannelLocal = Channel;
          GetChannelMapping (Channel, SubChannel, &ChannelLocal);
          SetActToActPeriod (Host, Socket, ChannelLocal, ActToActSave);
        }
      }

      ChBitmask = SkipDdrtCh (Socket, Dimm, ChBitmask);

      // a. Configure the DDRIO for sense amp. This step is performed while collecting data

      // b. Configure the CPGC pattern for the target rank.  Here we will perform the CPGC setup necessary
      //    for this step, but we will wait to program the pattern so the configuration doesn't get over-written
      //    by SendMrw commands. CPGC setup is the same as it was for Coarse Read Delay.
      SetupCpgcForReadDelay (Host, Socket, ChBitmask, Dimm, Rank);

      // c. Configure the pattern generated by the DRAM (MR25 to MR30).
      // d. Configure the pattern on DB.
      // e. Set the DB Buffer training configuration (RW97) with per-DQ feedback enabled.
      // f. Enable the MRD Training Mode on the DB.
      Status = LrdimmPerBitReadDelayInit (Host, Socket, ChBitmask, Dimm, Rank);
      if (Status != MRC_STATUS_SUCCESS) {
        return Status;
      }

      // g. Sweep MDQS delay
      PerBitDeskewSweep (Host, Socket, ChBitmask, Dimm, Rank, TrainRes);

      // h. Calculate the center for each nibble
      // i. Calculate the per-MDQ delay adjustments
      Status = EvaluateAndProgramPerBitReadResults (Host, Socket, ChBitmask, Dimm, Rank, TrainRes, PerNibbleCenter, PerBitResults);
      if (Status != MRC_STATUS_SUCCESS) {
        return Status;
      }

      // g. Disable MRD Training Mode
      // h. Disable the Long Read Pattern Sticky Status feedback mode
      LrdimmTrainingCleanup (Host, Socket, ChBitmask, Dimm, Rank);

      // Restore timing values
      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }

        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          ChannelLocal = Channel;
          GetChannelMapping (Channel, SubChannel, &ChannelLocal);
          RestoreActToActPeriod (Socket, ChannelLocal, ActToActSave);
        }
      }
    } // Rank loop
  } // Dimm loop

  DisplayFinalMdqPerBitResults (Host, Socket, PerBitResults);

  RcFreePool (TrainRes);
  return Status;
}

/**

  Perform LRDIMM Write DQ/DQS Delay training.

  @param[in] Socket  - Socket number

  @retval Status - Non-zero if any failure detected

**/
STATIC
MRC_STATUS
WriteDelayTraining (
  IN UINT8  Socket
  )
{
  MRC_STATUS                  Status = MRC_STATUS_SUCCESS;
  UINT8                       MaxChDdr;
  UINT8                       Channel;
  UINT32                      ChBitmask;
  UINT8                       SubChannel;
  UINT8                       ChannelLocal;
  UINT8                       Dimm;
  UINT8                       Rank;
  ACT_TO_ACT_SAVE_STRUCT      ActToActSave[MAX_CH];
  PSYSHOST                    Host = GetSysHostPointer ();
  struct MdqTrainingResults   (*TrainRes)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT8                       PerNibbleCenter[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2];
  UINT32                      PerBitResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][SWIZZLE_RESULTS_LEN];
  INT8                        OdtlOnResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2];
  INT8                        OdtlOffResults[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2];

  MaxChDdr = GetMaxChDdr ();

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "DDR5 LRDIMM Write Delay Training\n");

  SetCurrentTestType (Socket, MdqsWriteDelay);
  TrainRes = RcAllocatePool (sizeof (*TrainRes));
  ZeroMem ((UINT8 *) PerBitResults, sizeof (PerBitResults));
  ZeroMem ((UINT8 *) OdtlOnResults, sizeof (OdtlOnResults));
  ZeroMem ((UINT8 *) OdtlOffResults, sizeof (OdtlOffResults));

  // b. Configure the DDRIO for sense amp.
  MwdDdrioInit (Socket);

  //
  // Train each DIMM
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Train each rank
    //
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_BACKSIDE_RX_PER_BIT_DESKEW,
          (UINT16) ((Socket << 8) | (Dimm << 4) |Rank)));

      //
      // Create a bit mask of channels that have at least 1 rank present for this dimm and rank
      //
      ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, Rank);
      if (ChBitmask == 0) {
        continue;
      }

      // Set timings to ensure back-to-back transactions
      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }

        ActToActSave[Channel].TRrdSOrg = 0;
        ActToActSave[Channel].TRrdLOrg = 0;
        ActToActSave[Channel].TRcdRdOrg = 0;
        ActToActSave[Channel].TRrsrOrg = 0;
        ActToActSave[Channel].TRrSgOrg = 0;

        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          ChannelLocal = Channel;
          GetChannelMapping (Channel, SubChannel, &ChannelLocal);
          SetActToActPeriod (Host, Socket, ChannelLocal, ActToActSave);
        }
      }

      ZeroMem ((UINT8 *) TrainRes, sizeof (*TrainRes));

      // a. Initialze DRAM (performed in LrdimmWriteDelayInit)

      // c. Configure the CPGC pattern for the target rank.  Here we will perform the CPGC setup necessary
      //    for this step, but we will wait to program the pattern so the configuration doesn't get over-written
      //    by SendMrw commands.
      SetupCpgcForWriteDelay (Host, Socket, ChBitmask, Dimm, Rank);

      // d. Configure the pattern generated by the DB
      // e. Set the DB Buffer training configuration
      // f. Enable MWD training mode
      // g. Set DRAM VREF
      LrdimmWriteDelayInit (Host, Socket, ChBitmask, Dimm, Rank);

      // h. Sweep MDQ phase with respect to MDQS delay
      SweepWriteDelay (Host, Socket, ChBitmask, Dimm, Rank, TrainRes);

      // i. Calculate the center for each nibble
      // j. Calculate the per-DQ adjustment
      // k. print out the final results
      // n. Program the results for each nibble
      Status = EvaluateAndProgramWriteDelayResults (Host, Socket, ChBitmask, Dimm, Rank, TrainRes,
                   PerNibbleCenter, PerBitResults, OdtlOnResults, OdtlOffResults);
      if (Status != MRC_STATUS_SUCCESS) {
        return Status;
      }

      // l. Disable MWD Training mode on the DB
      // m. Disable the long read pattern sticky status
      LrdimmTrainingCleanup (Host, Socket, ChBitmask, Dimm, Rank);

      // Restore timings to previous value
      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }

        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          ChannelLocal = Channel;
          GetChannelMapping (Channel, SubChannel, &ChannelLocal);
          RestoreActToActPeriod (Socket, ChannelLocal, ActToActSave);
        }
      }
    }
  }

  DisplayFinalMdqPerNibbleResults (Host, Socket, PerNibbleCenter);
  DisplayFinalMdqPerBitResults (Host, Socket, PerBitResults);
  DisplayFinalOdtlResults (Host, Socket, OdtlOnResults, OdtlOffResults);

  PostMwdDdrioProgramming (Socket);
  RcFreePool (TrainRes);
  return Status;
}

/**

  Restore Data Buffer Rank Presence Settings:

  Restore the Rank presence settings for data buffers associated with a
  particular DIMM.

  @param[in] Socket - Socket number
  @param[in] Dimm   - DIMM number

  @retval Status

**/
UINT32
EFIAPI
RestoreDimmRankPresence (
  IN UINT8     Socket,
  IN UINT8     Dimm
  )
{
  UINT8                             Ch;
  UINT32                            ChBitmask;
  UINT32                            ChStatus;
  struct dimmNvram                  (*DimmNvList) [MAX_DIMM];
  UINT8                             NumDramRanks;
  UINT8                             RankBitMask;
  UINT32                            Status = SUCCESS;
  PSYSHOST                          Host;

  Host = GetSysHostPointer ();
  for (Ch = 0; Ch < MAX_CH; Ch++) {
    ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, 0);
    if (!((1 << Ch) & ChBitmask)) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if ((*DimmNvList) [Dimm].dimmPresent == 0) {
      continue;
    }

    ChStatus = SUCCESS;
    NumDramRanks = (*DimmNvList) [Dimm].numDramRanks;
    RankBitMask = 0;
    switch (NumDramRanks) {
      case 1:
        RankBitMask = 0xE;
        break;
      case 2:
        RankBitMask = 0xC;
        break;
      case 4:
        RankBitMask = 0x0;
        break;
      default:
        ChStatus = FAILURE;
        Status = FAILURE;
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "ERROR: Invalid number of NV DIMM ranks: %u. Keeping current settings.\n", NumDramRanks);
        break;
    }
    if (ChStatus == SUCCESS) {
      WriteLrbuf (Host, Socket, Ch, Dimm, 0, RankBitMask, LRDIMM_F0, LRDIMM_BC07);
    }
  } //Ch

  return Status;
} //RestoreDimmRankPresence

/**

  Perform Data Buffer to Memory Write Leveling Training (DWL)

  @param[in] Host  - Pointer to sysHost

  @retval status

**/
UINT32
EFIAPI
LrdimmBacksideWriteLeveling (
  PSYSHOST  Host
  )
{
  UINT8                             Socket;
  UINT32                            Status = SUCCESS;

  Socket = Host->var.mem.currentSocket;

  //
  // Reasons to just return with SUCCESS...
  //
  if ((!IsMemFlowEnabled (LrdimmBacksideTxTraining))      ||   // Training step is disabled
      (Host->nvram.mem.socket[Socket].enabled == 0)       ||   // No Socket present
      (Host->nvram.mem.socket[Socket].maxDimmPop == 0)    ||   // No DIMM present
      (Host->nvram.mem.socket[Socket].lrDimmPresent == 0))   // No LRDIMM present
  {
    return SUCCESS;
  }

  Status = KEEP_FAILURE (ExternalWriteLevelingCoarse (Socket), Status);

  Status = KEEP_FAILURE (ExternalWriteLevelingFine (Socket), Status);

  Status = KEEP_FAILURE (InternalWriteLeveling (Socket), Status);

  return Status;
} //LrdimmBacksideWriteLeveling

/**

  Perform LRDIMM DRAM Interface WL (DWL):

  This training step leverages the comparison capability of the buffer MWD
  Training Mode to determine the best Tx strobe (MDQS) cycle alignment for
  the associated CWL setting and DIMM routing.

  @param Host   - Pointer to sysHost
  @param socket - socket number

  @retval status

**/
UINT32
ExternalWriteLevelingCoarse (
  IN UINT8     Socket
  )
{
  PSYSHOST                          Host;
  INT8                              CycleDelay;
  UINT8                             Channel;
  INT8                              Dimm;
  UINT8                             Rank;
  UINT8                             MaxChDdr;
  UINT8                             ChannelLocal;
  UINT8                             SubChannel = 0;
  UINT8                             ControlWordData;
  UINT8                             LogRank = 0;
  UINT8                             LogSubRank = 0;
  UINT8                             FaultyPartsStatus;
  UINT8                             SubChMSVx4;
  UINT32                            ChBitmask;
  UINT32                            Status = SUCCESS;
  UINT32                            StrobePass[MAX_CH][SUB_CH][MAX_RANK_DIMM];
  ACT_TO_ACT_SAVE_STRUCT            ActToActSave[MAX_CH];
  struct channelNvram               (*ChannelNvList)[MAX_CH];
  struct DwlTrainingResults         (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];

  Host = GetSysHostPointer ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  SetCurrentTestType (Socket, MdqsExternalWriteLevelingCoarseTraining);
  TrainResult = RcAllocatePool (sizeof (*TrainResult));
  ZeroMem ((UINT8 *) TrainResult,      sizeof (*TrainResult));

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "LRDIMM External Write Leveling Coarse Training\n");

  DwlDdrioInit (Socket);
  ProgramDdrt2WrWrS (Host, Socket, 0x22);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    ZeroMem ((UINT8 *) StrobePass, sizeof (StrobePass));
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_LRDIMM_EXTERNAL_WRITE_LEVEL_COARSE, (UINT16) ((Socket << 8) | (Dimm << 4) | Rank)));

      FaultyPartsStatus = FPT_NO_ERROR;

      ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, Rank);
      if (ChBitmask == 0) {
        continue;
      }

      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }

        ActToActSave[Channel].TRrdSOrg = 0;
        ActToActSave[Channel].TRrdLOrg = 0;
        ActToActSave[Channel].TRcdRdOrg = 0;
        ActToActSave[Channel].TRrsrOrg = 0;
        ActToActSave[Channel].TRrSgOrg = 0;

        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          ChannelLocal = Channel;
          GetChannelMapping (Channel, SubChannel, &ChannelLocal);
          //
          // Get the logical Rank #
          //
          LogRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);

          SetActToActPeriod (Host, Socket, ChannelLocal, ActToActSave);
          //
          // Indicate this Rank is present on at least one channel
          //
          if ((*ChannelNvList) [Channel].encodedCSMode) {
            LogSubRank = (Rank >> 1);
          } else {
            LogSubRank = 0;
          }

          SelectCPGCRanks (Host, Socket, ChannelLocal, 1 << LogRank, 0, LogSubRank);

          //
          // Initial training result to 0xF0 to filter out cycle not in valid range [-4, 4]
          //
          SetMem ((UINT8 *) TrainResult, sizeof (*TrainResult), 0xF0);
        } // SubChannel loop

        //
        // 0. Initialize External Write Leveling Mode
        //
        InitialExternalWriteLevelingTraining (Socket, Channel, Dimm, Rank);

        //
        // 1. Initialize the DB MDQS Write Level Phase Delay to 0
        //
        ConfigureDataBufferWriteLevelPhase (Socket, Channel, Dimm, Rank);
      } // Channel loop

      //
      // Program counting window settings and enable receivers/ODT
      //
      if (IsBrsPresent (Host, Socket)) {
        SetDqCountingWindow (Host, Socket, Dimm, Rank);
      } else {
        SetDataTrainCountWindow (Host, Socket, Dimm, Rank, DWL_COUNTING_WINDOW);
      }

      //
      // 2. Sweep DB WL Cycle Delay = -1 to 4, step size 1
      //
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "LRDIMM MDQS External Coarse Write Leveling - Cycle Scanning:\n");

      //
      // Select appropriate DB page for RWE1 according to rank
      //
      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }
        DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, Rank);
      }

      for (CycleDelay = MDQS_External_WRITE_LEVEL_CYCLE_START; CycleDelay <= MDQS_External_WRITE_LEVEL_CYCLE_END; CycleDelay ++) {
        ControlWordData = ABS (CycleDelay);
        if (CycleDelay < 0) {
          ControlWordData |= BIT3;
        }

        //
        // Copy ControlWordData to upper nibble
        //
        ControlWordData |= (ControlWordData << 4);
        for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
          if (!((1 << Channel) & ChBitmask)) {
            continue;
          }

          //
          // Program RWE1 for DB Write Level Cycle Delay
          //
          DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE1_REG, ControlWordData);
        } // Channel loop

        GetDwlExtCoarseTrainResults (Socket, Dimm, Rank, CycleDelay, StrobePass, TrainResult);
      } // CycleDelay loop

      Status = (UINT32) EvaluateAndProgramDwlExtCoarseResults (Socket, ChBitmask, Dimm, Rank, TrainResult);

      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }

        CleanupWriteLevelingMode (Socket, Channel, Dimm, Rank);

        // Restore timing values
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          ChannelLocal = Channel;
          GetChannelMapping (Channel, SubChannel, &ChannelLocal);
          RestoreActToActPeriod (Socket, ChannelLocal, ActToActSave);
        }
      }
    } //Rank loop

    Status = KEEP_FAILURE (RestoreDimmRankPresence (Socket, Dimm), Status);

  } //Dimm loop

  // DDRIO cleanup
  PostDwlDdrioProgramming (Socket);
  ProgramDdrt2WrWrS (Host, Socket, 0x8);
  FifoTrainReset (Host, Socket);
  RcFreePool (TrainResult);
  return Status;
} // ExternalWriteLevelingCoarse


/**

  Initial External Write Leveling Training

  @param[in] Socket  - Socket Number
  @param[in] Channel - Channel Number
  @param[in] Dimm    - Dimm Number
  @param[in] Rank    - Rank Number

  @retval N/A

**/
VOID
InitialExternalWriteLevelingTraining (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  struct rankDevice                 (*RankStruct) [MAX_RANK_DIMM];
  struct dimmNvram                  (*DimmNvList) [MAX_DIMM];
  DDR5_MODE_REGISTER_2_STRUCT       ModeRegister2;
  PSYSHOST                          Host;

  Host = GetSysHostPointer ();

  DimmNvList = GetDimmNvList (Host, Socket, Channel);
  RankStruct = GetRankStruct (Host, Socket, Channel, Dimm);
  if ((*DimmNvList)[Dimm].DcpmmPresent) {
    ProgramWlMr2Ddrt2 (Host, Socket, Channel, Dimm, Rank, EXTERNAL_WRITE_LEVELING_MODE);
  } else {
    //
    // Select all DRAM devices
    //
    WriteMPC (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, Rank, PDA_SELECT_ALL, TIMING_4N, TRUE);

    //
    // Enable DRAM Write Leveling training mode
    //
    ModeRegister2.Data = 0;
    ModeRegister2.Bits.write_leveling = 1;
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_2_REG, ModeRegister2.Data);
  }

  //
  // Enable DB Write Leveling
  //
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW83_REG, LRDIMM_WRLVL_TRAINING);

  (*RankStruct) [Rank].CurrentLrdimmTrainingMode = LRDIMM_DWL_EXT_TRAINING_MODE;
} // InitialExternalWriteLevelingTraining

/**

  Initialize the DB MDQS Write Level Phase Delay to 0

  @param[in] Socket  - Socket Number
  @param[in] Channel - Channel Number
  @param[in] Dimm    - Dimm Number
  @param[in] Rank    - Rank Number

  @retval N/A

**/
STATIC
VOID
ConfigureDataBufferWriteLevelPhase (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  PSYSHOST                          Host;

  Host = GetSysHostPointer ();
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, Rank);
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE8_REG, 0);
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE9_REG, 0);
} // ConfigureDataBufferWriteLevelPhase

/**

  Get's the results from DWL - external Coarse

  @param[in] Socket           - Socket Number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number of the DIMM
  @param[in] CycleDelay       - Cycle delay
  @param[in][out] StrobePass  - Strobe Pass information
  @param[in][out] TrainResult - Store trained result

  @retval N/A

**/
VOID
GetDwlExtCoarseTrainResults (
  IN UINT8                          Socket,
  IN UINT8                          Dimm,
  IN UINT8                          Rank,
  IN INT8                           CycleDelay,
  IN OUT UINT32                     StrobePass [MAX_CH][SUB_CH][MAX_RANK_DIMM],
  IN OUT struct DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8                         Channel;
  UINT8                         Strobe;
  UINT8                         SwizzleStrobe;
  UINT8                         MaxChDdr;
  UINT8                         SubCh = 0;
  UINT8                         SubChMSVx4;
  UINT32                        LrdimmFeedBack;
  UINT32                        ChBitmask;
  struct channelNvram           (*ChannelNvList)[MAX_CH];
  PSYSHOST                      Host;

  Host = GetSysHostPointer ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, Rank);

  //
  // Execute test
  //
  ExecuteBacksideExtCoarseTest (Socket, ChBitmask, Dimm, Rank);

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }

    for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        if ((StrobePass [Channel][SubCh][Rank]) & (1 << Strobe)) {
          continue;
        }

        SwizzleStrobe = CheckBacksideSwizzle (Host, Socket, Channel, Dimm, Strobe);
        LrdimmFeedBack = ReadDataTrainCounterFeedbackDdr5 (Host, Socket, Channel,  SubCh, SwizzleStrobe);

        if (LrdimmFeedBack > DWL_WRITE_CYCLE / 2) {
          //
          // 3.The final results per nibble are obtained by substracting 1 from the recorded value.
          // This is to start the fine phase sweep right before the 0 to 1 transition
          //
          (*TrainResult) [Channel][SubCh][Strobe].Results = CycleDelay - 1;

          //
          // Record the strobe already passed
          //
          StrobePass [Channel][SubCh][Rank] |= (1 << Strobe);
        }
      } // strobe loop
    } // SubCh loop
  } // Channel loop
} // GetDwlExtCoarseTrainResults

/**

  Sets up and executes the MRE CPGC test which sends MRR commands to the DRAM.

  @param[in]  Socket    - Socket number
  @param[in]  ChBitmask - Mask of channels on which test should be executed
  @param[in]  Dimm      - Dimm number
  @param[in]  Rank      - Rank number of the DIMM

**/
VOID
ExecuteBacksideExtCoarseTest (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  PSYSHOST                        Host;

  Host = GetSysHostPointer ();
  UINT8   DumArr[7] = {0, 0, 0, 0, 0, 0, 0};

  //
  // Setup CPGC to execute single cacheline write loopback test
  //
  SetupLrdimmWriteTest (Socket, ChBitmask, DWL_WRITE_CYCLE, 1, 0);

  //
  // Execute the CPGC MPR WR sequence
  //
  RunIOTest (Host, Socket, ChBitmask, BasicVA, DumArr, 0, 0);

  if (IsBrsPresent (Host, Socket)) {
    // IO Reset is necessary for BRS for consistent feedback results.
    // NOTE: This IO Reset should be evaluated for SPR
    IO_Reset (Host, Socket);
    EnableDdrioTrainingCounter (Host, Socket, Dimm, Rank);
  }

  if (FeaturePcdGet (PcdCosimBuild) == TRUE) {
    //
    // Delay if this is a CTE build, otherwise do nothing.
    //
    CteDelayDclk (100);
  } else {
    FixedDelayMicroSecond (1);
  }

} // ExecuteBacksideExtCoarseTest

/**

  Initialize CPGC for LRDIMM Write Training

  @param[in]    Socket    - socket number
  @param[in]    ChBitmask - mask of channels to be used in this CPGC test
  @param[in]    NumCL     - Number of cacheline
  @param[in]    LoopCount - Number of LoopCount
  @param[in]    Subseqwait - Waiting time in between subsequents

  @retval N/A
**/
VOID
SetupLrdimmWriteTest (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     NumCL,
  IN UINT8     LoopCount,
  IN UINT8     Subseqwait
  )
{
  UINT8                           Channel;
  UINT8                           MaxChDdr;
  struct    channelNvram          (*ChannelNvList)[MAX_CH];
  PSYSHOST                        Host;
  //
  // Setup CPGC Pattern
  //
  TCPGCAddress CPGCAddress = {{0, 0, 0, 0},    // Start
                              {0, 7, 0, 0x1F}, // Stop
                              {0, 0, 0, 0},    // Order
                              {0, 0, 0, 0},    // IncRate
                              {0, 0, 0, 1}};   // IncValue
  //                       IncRate Start, Stop, DQPat
  TWDBPattern WDBPattern = {0,  0,  1,  BasicVA};

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }
    if (IsDcpmmPresentChannel (Socket, Channel) == TRUE) {
      Subseqwait = 100;
      break;
    }
  }
  SetupIOTest (Host, Socket, ChBitmask, PatWr, NumCL, LoopCount, &CPGCAddress, NSOE, &WDBPattern, 0, 0, Subseqwait);
} //SetupLrdimmReadWriteTest


/**

  Evaluates and prints out the results from the DWL External Coarse and programs
  the cycle result

  @param[in] Socket      - Socket number
  @param[in] ChBitmask   - Channel bit mask
  @param[in] Dimm        - Dimm number
  @param[in] LrTrainRes  - Training result

  @retval N/A

**/
MRC_STATUS
EvaluateAndProgramDwlExtCoarseResults (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN struct    DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8                       Channel;
  UINT8                       MaxChDdr;
  UINT8                       SubChannel;
  UINT8                       Strobe;
  UINT8                       SubChMSVx4;
  UINT8                       MaxStrobePairs;
  UINT8                       FaultyPartsStatus;
  UINT8                       ControlWordData;
  UINT8                       ControlWordDataLowerNibble;
  UINT8                       ControlWordDataUpperNibble;
  MRC_STATUS                  Status = MRC_STATUS_SUCCESS;
  struct ddrRank              (*RankList)[MAX_RANK_DIMM];
  struct rankDevice           (*RankStruct) [MAX_RANK_DIMM];
  struct dimmNvram            (*DimmNvList)[MAX_DIMM];
  BOOLEAN                     (*NibbleSwap) [MAX_CH][MAX_DIMM][SUB_CH][MAX_STROBE_DDR5 / 2];
  PSYSHOST                    Host;
  DDR5_LRDIMM_TRAINING_DATA   *TrainingData;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  MaxStrobePairs = SubChMSVx4 / 2;

  //
  // Retrieve nibble swap info from MRE
  //
  Status = GetDdr5LrdimmTrainingData (&TrainingData);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }
  NibbleSwap = &(TrainingData->NibbleSwap);


  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }

    RankList = GetRankNvList (Host, Socket, Channel, Dimm);
    RankStruct = GetRankStruct (Host, Socket, Channel, Dimm);
    DimmNvList = GetDimmNvList (Host, Socket, Channel);

    for (SubChannel = 0 ; SubChannel < SUB_CH; SubChannel++) {
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }

        if ((UINT8) (*TrainResult) [Channel][SubChannel][Strobe].Results == 0xF0) {
          (*TrainResult) [Channel][SubChannel][Strobe].Results = 0;
#ifdef DDRT_SUPPORT
          if ((*DimmNvList)[Dimm].DcpmmPresent) {
            //
            // De-swizzle NVMDIMM strobe for FPT tracking
            //
            UINT8 SwizzleStrobe = CheckBacksideSwizzle (Host, Socket, Channel, Dimm, Strobe);
            //
            // Failure, mark this part as faulty
            //
            (*RankList)[Rank].faultyParts[SwizzleStrobe] |= FPT_LRDIMM_TRAINING_FAILED;
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, SwizzleStrobe, NO_BIT,
              "PMem DWL External Coarse - FAILED WRITE CYCLE\n");
          }
#endif // DDRT_SUPPORT
          if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
            //
            // Failure, mark this part as faulty
            //
            (*RankList)[Rank].faultyParts[Strobe] |= FPT_LRDIMM_TRAINING_FAILED;
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, Strobe, NO_BIT,
              "LRDIMM DWL External Coarse - FAILED WRITE CYCLE\n");
          }
        }
      } //Strobe

      //
      // Program final training result
      //
      for (Strobe = 0; Strobe < MaxStrobePairs; Strobe++) {
        ControlWordDataLowerNibble = ABS ((*TrainResult) [Channel][SubChannel][Strobe].Results);
        if ((*TrainResult) [Channel][SubChannel][Strobe].Results < 0) {
          ControlWordDataLowerNibble |= BIT3;
        }

        ControlWordDataUpperNibble = ABS ((*TrainResult) [Channel][SubChannel][Strobe + MaxStrobePairs].Results);
        if ((*TrainResult) [Channel][SubChannel][Strobe + MaxStrobePairs].Results < 0) {
          ControlWordDataUpperNibble |= BIT3;
        }

        //
        // ControlWordData need to swap if there is Nibble Swap behind of Data Buffer
        //
        if ((*NibbleSwap) [Channel][Dimm][SubChannel][Strobe] == TRUE) {
          ControlWordData = (ControlWordDataLowerNibble << 4) | ControlWordDataUpperNibble;
        } else {
          ControlWordData = ControlWordDataLowerNibble | (ControlWordDataUpperNibble << 4);
        }

        //
        // Select appropriate buffer device and progarm final value to RWE1
        //
        DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, Strobe);
        DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE1_REG, ControlWordData);

        //
        // Save RWE1 settings for later use
        //
        (*RankStruct) [Rank].cachedLrBuf_PG01RWE1[Strobe + (SubChannel * MaxStrobePairs)] = ControlWordData;
      } // Strobe loop

      //
      // Set all buffer device back to RW93
      //
      DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);

      FaultyPartsStatus = EvaluateFaultyParts (Host, Socket, Channel);
      if (FaultyPartsStatus == FPT_CORRECTABLE_ERROR) {
        EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_DWL_EXT_COARSE_TRAINING, Socket, Channel, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, MrcGtDelim, LrbufLevel, 0xFF);
      } else if (FaultyPartsStatus == FPT_NO_CORRECTABLE_ERROR) {
        EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_DWL_EXT_COARSE_TRAINING, Socket, Channel, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, MrcGtDelim, LrbufLevel, 0xFF);
        DisableRank (Host, Socket, Channel, Dimm, Rank);
      }
    } // SubCh loop

#ifdef DEBUG_CODE_BLOCK
    if (GetDebugLevel () & SDBG_MAX) {
      AcquirePrintControl ();
      RcDebugPrint (SDBG_DEFAULT, "\nSummary: LRDIMM External Write Leveling Coarse\nS%d, Ch%d, DIMM%d, Rank%d\n", Socket, Channel, Dimm, Rank);
      DisplayDwlExtCoarseResult (Socket, Channel, Dimm, TrainResult);
      ReleasePrintControl ();
    }
#endif // DEBUG_CODE_BLOCK
  } // Channel loop
  return Status;
} // EvaluateAndProgramDwlExtCoarseResults

/**

  Clean up Write Leveling Mode

  @param[in] Socket  - Socket Number
  @param[in] Channel - Channel Number
  @param[in] Dimm    - Dimm Number
  @param[in] Rank    - Rank Number

  @retval N/A

**/
VOID
CleanupWriteLevelingMode (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  struct rankDevice                 (*RankStruct) [MAX_RANK_DIMM];
  struct dimmNvram                  (*DimmNvList) [MAX_DIMM];
  DDR5_MODE_REGISTER_2_STRUCT       ModeRegister2;
  DDR5_DATA_BUFFER_RW97_STRUCT      BufferTrainingConfig;
  PSYSHOST                          Host;

  Host = GetSysHostPointer ();

  DimmNvList = GetDimmNvList (Host, Socket, Channel);
  RankStruct = GetRankStruct (Host, Socket, Channel, Dimm);

  //
  // Disable DRAM Write Leveling training mode
  //
  if ((*DimmNvList)[Dimm].DcpmmPresent) {
    ProgramCwvWriteLevel (Host, Socket, Channel, Dimm, 0);
  } else {
    //
    // Select all DRAM devices and set internal_leveling to enabl
    //
    WriteMPC (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, Rank, PDA_SELECT_ALL, TIMING_4N, TRUE);

    ModeRegister2.Data = 0;
    ModeRegister2.Bits.write_leveling = 0;

    //
    // Only leave internal_write_timing to 1 when exit from internal write leveling training
    //
    if ((*RankStruct) [Rank].CurrentLrdimmTrainingMode == LRDIMM_DWL_INT_TRAINING_MODE) {
      ModeRegister2.Bits.internal_write_timing = 1;
    }
    DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_2_REG, ModeRegister2.Data);
  }

  //
  // Select all DB devices
  //
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);

  //
  // Disable DB Write Leveling
  //
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW83_REG, LRDIMM_EXIT_TRAINING);

  //
  // Clear DB training feedback
  //
  BufferTrainingConfig.Data = 0;
  BufferTrainingConfig.Bits.clear_feedback_status_bit = 1;
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW97_REG,  BufferTrainingConfig.Data);

  //
  // Cleanup DB page
  //
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, 0);

  (*RankStruct) [Rank].CurrentLrdimmTrainingMode = LRDIMM_NORMAL_MODE;
} // CleanupWriteLevelingMode

/**

  Perform LRDIMM DRAM Interface WL (DWL):

  This training step leverages the comparison capability of the buffer MWD
  Training Mode to determine the best Tx strobe (MDQS) cycle alignment for
  the associated CWL setting and DIMM routing.

  @param[in] Socket - Socket number

  @retval status

**/
UINT32
ExternalWriteLevelingFine (
  IN UINT8     Socket
  )
{
  PSYSHOST                          Host;
  UINT8                             PhaseDelay;
  UINT8                             Channel;
  UINT8                             Dimm;
  UINT8                             Rank;
  UINT8                             MaxChDdr;
  UINT8                             ChannelLocal;
  UINT8                             SubChannel = 0;
  UINT8                             ControlWordData;
  UINT8                             LogRank = 0;
  UINT8                             LogSubRank = 0;
  UINT8                             FaultyPartsStatus;
  UINT8                             SubChMSVx4;
  UINT32                            ChBitmask;
  UINT32                            Status = SUCCESS;
  UINT32                            StrobePass[MAX_CH][SUB_CH][MAX_RANK_DIMM];
  ACT_TO_ACT_SAVE_STRUCT            ActToActSave[MAX_CH];
  struct channelNvram               (*ChannelNvList)[MAX_CH];
  struct DwlTrainingResults         (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);


  SetCurrentTestType (Socket, MdqsExternalWriteLevelingFineTraining);
  TrainResult = RcAllocatePool (sizeof (*TrainResult));
  ZeroMem ((UINT8 *) TrainResult, sizeof (*TrainResult));

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "LRDIMM External Write Leveling Fine Training\n");

  DwlDdrioInit (Socket);
  ProgramDdrt2WrWrS (Host, Socket, 0x22);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    ZeroMem ((UINT8 *) StrobePass, sizeof (StrobePass));
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_LRDIMM_EXTERNAL_WRITE_LEVEL_COARSE, (UINT16) ((Socket << 8) | (Dimm << 4) | Rank)));

      FaultyPartsStatus = FPT_NO_ERROR;

      ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, Rank);
      if (ChBitmask == 0) {
        continue;
      }

      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }

        ActToActSave[Channel].TRrdSOrg = 0;
        ActToActSave[Channel].TRrdLOrg = 0;
        ActToActSave[Channel].TRcdRdOrg = 0;
        ActToActSave[Channel].TRrsrOrg = 0;
        ActToActSave[Channel].TRrSgOrg = 0;

        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          ChannelLocal = Channel;
          GetChannelMapping (Channel, SubChannel, &ChannelLocal);
          //
          // Get the logical Rank #
          //
          LogRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);

          SetActToActPeriod (Host, Socket, ChannelLocal, ActToActSave);
          //
          // Indicate this Rank is present on at least one channel
          //
          if ((*ChannelNvList) [Channel].encodedCSMode) {
            LogSubRank = (Rank >> 1);
          } else {
            LogSubRank = 0;
          }

          SelectCPGCRanks (Host, Socket, ChannelLocal, 1 << LogRank, 0, LogSubRank);

          //
          // Initial training result to 0xFF
          //
          SetMem ((UINT8 *) TrainResult, sizeof (*TrainResult), 0xFF);
        } //Subch loop

        //
        // 1. Initialize External Write Leveling Mode
        //
        InitialExternalWriteLevelingTraining (Socket, Channel, Dimm, Rank);
      } // Channel loop

      //
      // Program counting window settings and enable receivers/ODT
      //
      if (IsBrsPresent (Host, Socket)) {
        SetDqCountingWindow (Host, Socket, Dimm, Rank);
      } else {
        SetDataTrainCountWindow (Host, Socket, Dimm, Rank, DWL_COUNTING_WINDOW);
      }

      //
      // 4.a Sweep DB WL Phase Delay = 0 to 63, step size 1
      //
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "LRDIMM MDQS External Fine Write Leveling - Phase Scanning:\n");

      //
      // Select appropriate DB page according to rank
      //
      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }
        DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, Rank);
      }

      PhaseSpotsPreCheck (Socket, ChBitmask, Dimm, Rank);

      for (PhaseDelay = MDQS_External_WRITE_LEVEL_PHASE_START; PhaseDelay <= MDQS_External_WRITE_LEVEL_PHASE_END; PhaseDelay ++) {
        ControlWordData = PhaseDelay;

        for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
          if (!((1 << Channel) & ChBitmask)) {
            continue;
          }
          //
          // Program RWE8/9 for DB Write Level Phase Delay
          //
          DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE8_REG, ControlWordData);
          DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE9_REG, ControlWordData);
        } // Channel loop

        GetDwlExtFineTrainResults (Socket, Dimm, Rank, PhaseDelay, StrobePass, TrainResult);
      } // PhaseDelay loop

      Status = (UINT32) EvaluateAndProgramDwlExtFineResults (Socket, ChBitmask, Dimm, Rank, TrainResult);

      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }

        CleanupWriteLevelingMode (Socket, Channel, Dimm, Rank);

        // Restore timing values
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          ChannelLocal = Channel;
          GetChannelMapping (Channel, SubChannel, &ChannelLocal);
          RestoreActToActPeriod (Socket, ChannelLocal, ActToActSave);
        }
      }
    } //Rank loop

    Status = KEEP_FAILURE (RestoreDimmRankPresence (Socket, Dimm), Status);

  } //Dimm loop

  // DDRIO cleanup
  PostDwlDdrioProgramming (Socket);
  ProgramDdrt2WrWrS (Host, Socket, 0x8);
  FifoTrainReset (Host, Socket);
  RcFreePool (TrainResult);
  return Status;
} // ExternalWriteLevelingFine

/**

  Get's the results from DWL - external fine training

  @param[in] Socket           - Socket Number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number of the DIMM
  @param[in] PhaseDelay       - Phase delay
  @param[in][out] StrobePass  - Strobe Pass information
  @param[in][out] TrainResult - Store trained result

  @retval N/A

**/
VOID
GetDwlExtFineTrainResults (
  IN UINT8                          Socket,
  IN UINT8                          Dimm,
  IN UINT8                          Rank,
  IN INT8                           PhaseDelay,
  IN OUT UINT32                     StrobePass [MAX_CH][SUB_CH][MAX_RANK_DIMM],
  IN OUT struct DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8                         Channel;
  UINT8                         Strobe;
  UINT8                         SwizzleStrobe;
  UINT8                         SubCh = 0;
  UINT8                         SubChMSVx4;
  UINT32                        LrdimmFeedBack;
  UINT32                        ChBitmask;
  struct channelNvram           (*ChannelNvList)[MAX_CH];
  PSYSHOST                      Host;
  UINT8                         MaxChDdr;

  Host = GetSysHostPointer ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  ChannelNvList = GetChannelNvList (Host, Socket);
  ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, Rank);
  MaxChDdr = GetMaxChDdr ();

  //
  // 3. Setup the CPGC test pattern to 32 writes with 16 clocks in between writes.
  //    This will result in 32 samples for edge detection
  //
  ExecuteBacksideExtFineTest (Socket, ChBitmask, Dimm, Rank);

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }

    for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        SwizzleStrobe = CheckBacksideSwizzle (Host, Socket, Channel, Dimm, Strobe);
        LrdimmFeedBack = ReadDataTrainCounterFeedbackDdr5 (Host, Socket, Channel,  SubCh, SwizzleStrobe);

        if (LrdimmFeedBack > DWL_WRITE_CYCLE / 2) {
          //
          // 4.b If this is greater than 16 (number of writes)/2, then DQ sample is 1
          //
          if (((StrobePass [Channel][SubCh][Rank]) & (1 << Strobe)) == 0) {
            (*TrainResult) [Channel][SubCh][Strobe].Results = PhaseDelay;
          }

          (*TrainResult) [Channel][SubCh][Strobe].ShowResults[PhaseDelay] = PhaseDelay;
          //
          // Record the strobe already passed
          //
          StrobePass [Channel][SubCh][Rank] |= (1 << Strobe);
        }
      } // strobe loop
    } // SubCh loop
  } // Channel loop
} // GetDwlExtFineTrainResults

/**

  Sets up and executes the MRE CPGC test which sends MRR commands to the DRAM.

  @param[in]  Socket    - Socket number
  @param[in]  ChBitmask - Mask of channels on which test should be executed
  @param[in]  Dimm      - Dimm number
  @param[in]  Rank      - Rank number of the DIMM

**/
VOID
ExecuteBacksideExtFineTest (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  PSYSHOST                        Host;

  Host = GetSysHostPointer ();
  UINT8   DumArr[7] = {0, 0, 0, 0, 0, 0, 0};

  //
  // Clear train_cnt before issuing next test
  //
  IO_Reset (Host, Socket);

  //
  // Setup CPGC to execute 32 writes with 16 clocks in between writes
  //
  SetupLrdimmWriteTest (Socket, ChBitmask, DWL_WRITE_CYCLE, 1, 16);

  //
  // Execute the CPGC MPR WR sequence
  //
  RunIOTest (Host, Socket, ChBitmask, BasicVA, DumArr, 0, 0);

  if (IsBrsPresent (Host, Socket)) {
    EnableDdrioTrainingCounter (Host, Socket, Dimm, Rank);
  }

  if (FeaturePcdGet (PcdCosimBuild) == TRUE) {
    //
    // Delay if this is a CTE build, otherwise do nothing.
    //
    CteDelayDclk (100);
  } else {
    FixedDelayMicroSecond (1);
  }

} // ExecuteBacksideExtFineTest

/**

  Evaluates and prints out the results from the DWL External Fine and programs
  the phase result

  @param[in] Socket      - Socket number
  @param[in] ChBitmask   - Channel bit mask
  @param[in] Dimm        - Dimm number
  @param[in] LrTrainRes  - Training result

  @retval N/A

**/
MRC_STATUS
EvaluateAndProgramDwlExtFineResults (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN struct    DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8                       Channel;
  UINT8                       MaxChDdr;
  UINT8                       SubChannel;
  UINT8                       Strobe;
  UINT8                       SubChMSVx4;
  UINT8                       MaxStrobePairs;
  UINT8                       FaultyPartsStatus;
  UINT8                       ControlWordDataLowerNibble;
  UINT8                       ControlWordDataUpperNibble;
  MRC_STATUS                  Status = MRC_STATUS_SUCCESS;
  struct ddrRank              (*RankList)[MAX_RANK_DIMM];
  struct rankDevice           (*RankStruct) [MAX_RANK_DIMM];
  struct dimmNvram            (*DimmNvList)[MAX_DIMM];
  BOOLEAN                     (*NibbleSwap) [MAX_CH][MAX_DIMM][SUB_CH][MAX_STROBE_DDR5 / 2];
  PSYSHOST                    Host;
  DDR5_LRDIMM_TRAINING_DATA   *TrainingData;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  MaxStrobePairs = SubChMSVx4 / 2;

  //
  // Retrieve nibble swap info from MRE
  //
  Status = GetDdr5LrdimmTrainingData (&TrainingData);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }
  NibbleSwap = &(TrainingData->NibbleSwap);

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }

    RankList = GetRankNvList (Host, Socket, Channel, Dimm);
    RankStruct = GetRankStruct (Host, Socket, Channel, Dimm);
    DimmNvList = GetDimmNvList (Host, Socket, Channel);

    for (SubChannel = 0 ; SubChannel < SUB_CH; SubChannel++) {
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }

        if ((UINT8) (*TrainResult) [Channel][SubChannel][Strobe].Results == 0xFF) {
          (*TrainResult) [Channel][SubChannel][Strobe].Results = MDQS_External_WRITE_LEVEL_PHASE_END;
#ifdef DDRT_SUPPORT
          if ((*DimmNvList)[Dimm].DcpmmPresent) {
            //
            // De-swizzle NVMDIMM strobe for FPT tracking
            //
            UINT8 SwizzleStrobe = CheckBacksideSwizzle (Host, Socket, Channel, Dimm, Strobe);
            //
            // Failure, mark this part as faulty
            //
            (*RankList)[Rank].faultyParts[SwizzleStrobe] |= FPT_LRDIMM_TRAINING_FAILED;
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, SwizzleStrobe, NO_BIT,
              "PMem DWL External FINE - FAILED WRITE PHASE\n");
          }
#endif // DDRT_SUPPORT
          if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
            //
            // Failure, mark this part as faulty
            //
            RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, SubChannel, Rank, Strobe, NO_BIT,
              "LRDIMM DWL External Fine - FAILED WRITE PHASE\n");
          }
        }
      } //Strobe

      //
      // Program final training result
      //
      for (Strobe = 0; Strobe < MaxStrobePairs; Strobe++) {
        ControlWordDataLowerNibble = (*TrainResult) [Channel][SubChannel][Strobe].Results;
        ControlWordDataUpperNibble = (*TrainResult) [Channel][SubChannel][Strobe + MaxStrobePairs].Results;

        //
        // Select appropriate buffer device and progarm final value to RWE8/9
        //
        DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, Strobe);
        if ((*NibbleSwap) [Channel][Dimm][SubChannel][Strobe] == TRUE) {
          //
          // ControlWordData need to swap if there is Nibble Swap behind of Data Buffer
          //
          DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE8_REG, ControlWordDataUpperNibble);
          DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE9_REG, ControlWordDataLowerNibble);

          //
          // Save RWE8/9 settings for later use
          //
          (*RankStruct) [Rank].cachedLrBuf_PG01RWE8[Strobe + (SubChannel * MaxStrobePairs)] = ControlWordDataUpperNibble;
          (*RankStruct) [Rank].cachedLrBuf_PG01RWE9[Strobe + (SubChannel * MaxStrobePairs)] = ControlWordDataLowerNibble;

        } else {
          DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE8_REG, ControlWordDataLowerNibble);
          DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE9_REG, ControlWordDataUpperNibble);

          //
          // Save RWE8/9 settings for later use
          //
          (*RankStruct) [Rank].cachedLrBuf_PG01RWE8[Strobe + (SubChannel * MaxStrobePairs)] = ControlWordDataLowerNibble;
          (*RankStruct) [Rank].cachedLrBuf_PG01RWE9[Strobe + (SubChannel * MaxStrobePairs)] = ControlWordDataUpperNibble;
        }
      } // Strobe loop

      //
      // Set all buffer device back to RW93
      //
      DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);

      FaultyPartsStatus = EvaluateFaultyParts (Host, Socket, Channel);
      if (FaultyPartsStatus == FPT_CORRECTABLE_ERROR) {
        EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_DWL_EXT_FINE_TRAINING, Socket, Channel, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, MrcGtDelim, LrbufLevel, 0xFF);
      } else if (FaultyPartsStatus == FPT_NO_CORRECTABLE_ERROR) {
        EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_DWL_EXT_FINE_TRAINING, Socket, Channel, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, MrcGtDelim, LrbufLevel, 0xFF);
        DisableRank (Host, Socket, Channel, Dimm, Rank);
      }
    } // SubCh loop

#ifdef DEBUG_CODE_BLOCK
    if (GetDebugLevel () & SDBG_MAX) {
      AcquirePrintControl ();
      RcDebugPrint (SDBG_DEFAULT, "\nSummary: LRDIMM External Write Leveling Fine\nS%d, Ch%d, DIMM%d, Rank%d\n", Socket, Channel, Dimm, Rank);
      DisplayDwlExtFineResult (Socket, Channel, Dimm, TrainResult);
      ReleasePrintControl ();
    }
#endif // DEBUG_CODE_BLOCK
  } // Channel loop
  return Status;
} // EvaluateAndProgramDwlExtFineResults

#ifdef DEBUG_CODE_BLOCK
/**

  This function print the sample array for external coarse

  @param[in] Socket            - Socket number
  @param[in] Channel           - Channel number
  @param[in] Dimm              - DIMM number
  @param[in][out] TrainResult  - Trained result

  @retval N/A

**/
VOID
DisplayDwlExtCoarseResult (
  IN UINT8                         Socket,
  IN UINT8                         Channel,
  IN UINT8                         Dimm,
  IN OUT struct DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8             SubCh;
  UINT8             Strobe;
  UINT8             SubChMSVx4;
  INT8              CycleSetting;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];
  PSYSHOST          Host;

  Host = GetSysHostPointer ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  RcDebugPrintLine (SDBG_DEFAULT, 85, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  RcDebugPrint (SDBG_DEFAULT, "                   SubCh A                   |             SubCh B                  \n");
  RcDebugPrintLine (SDBG_DEFAULT, 85, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3   4   5   6   7   8   9 | 0   1   2   3   4   5   6   7   8   9\n");

  for (CycleSetting = MDQS_External_WRITE_LEVEL_CYCLE_START; CycleSetting < MDQS_External_WRITE_LEVEL_CYCLE_END; CycleSetting++) {
    RcDebugPrint (SDBG_DEFAULT, "%2d  ", CycleSetting);
    for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        RcDebugPrint (SDBG_DEFAULT, "   ");
        if ((CycleSetting - 1) == (*TrainResult) [Channel][SubCh][Strobe].Results ) {
          RcDebugPrint (SDBG_DEFAULT, "1");
        } else {
          RcDebugPrint (SDBG_DEFAULT, "0");
        }
      }
    } // Strobe loop

    RcDebugPrint (SDBG_DEFAULT, "\n");
  } // CycleSetting loop

  //
  // Print rising edges
  //
  RcDebugPrintLine (SDBG_MAX, 85, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  RcDebugPrint (SDBG_DEFAULT, "RE: ");
  for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
    for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
      if (IsStrobeNotValidDdr5 (Host, Strobe)) {
        continue;
      }

      RcDebugPrint (SDBG_DEFAULT, " %3d", (*TrainResult) [Channel][SubCh][Strobe].Results);
    } // Strobe loop
  } // SubCh loop
  RcDebugPrint (SDBG_DEFAULT, "\n");
} // DisplayDwlExtCoarseResult

/**

  This function print the sample array for external fine

  @param[in] Socket            - Socket number
  @param[in] Channel           - Channel number
  @param[in] Dimm              - DIMM number
  @param[in][out] TrainResult  - Trained result

  @retval N/A

**/
VOID
DisplayDwlExtFineResult (
  IN UINT8                         Socket,
  IN UINT8                         Channel,
  IN UINT8                         Dimm,
  IN OUT struct DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8             SubCh;
  UINT8             Strobe;
  UINT8             SubChMSVx4;
  INT8              PhaseSetting;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];
  PSYSHOST          Host;

  Host = GetSysHostPointer ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  RcDebugPrintLine (SDBG_DEFAULT, 85, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  RcDebugPrint (SDBG_DEFAULT, "                   SubCh A                   |             SubCh B                  \n");
  RcDebugPrintLine (SDBG_DEFAULT, 85, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3   4   5   6   7   8   9 | 0   1   2   3   4   5   6   7   8   9\n");

  for (PhaseSetting = MDQS_External_WRITE_LEVEL_PHASE_START; PhaseSetting <= MDQS_External_WRITE_LEVEL_PHASE_END; PhaseSetting++) {
    RcDebugPrint (SDBG_DEFAULT, "%2d  ", PhaseSetting);
    for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }

        RcDebugPrint (SDBG_DEFAULT, "   ");
        if (PhaseSetting  == (*TrainResult) [Channel][SubCh][Strobe].ShowResults[PhaseSetting] ) {
          RcDebugPrint (SDBG_DEFAULT, "1");
        } else {
          RcDebugPrint (SDBG_DEFAULT, "0");
        }
      }
    } // Strobe loop

    RcDebugPrint (SDBG_DEFAULT, "\n");
  } // PhaseSetting loop

  //
  // Print rising edges
  //
  RcDebugPrintLine (SDBG_MAX, 85, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  RcDebugPrint (SDBG_DEFAULT, "RE: ");
  for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
    for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
      if (IsStrobeNotValidDdr5 (Host, Strobe)) {
        continue;
      }

      RcDebugPrint (SDBG_DEFAULT, " %3d", (*TrainResult) [Channel][SubCh][Strobe].Results);
    } // Strobe loop
  } // SubCh loop
  RcDebugPrint (SDBG_DEFAULT, "\n");
} // DisplayDwlExtFineResult

/**

  This function print the sample array

  @param[in] Socket            - Socket number
  @param[in] Channel           - Channel number
  @param[in] Dimm              - DIMM number
  @param[in][out] TrainResult  - Trained result

  @retval N/A

**/
VOID
DisplayDwlIntCoarseResult (
  IN UINT8                         Socket,
  IN UINT8                         Channel,
  IN UINT8                         Dimm,
  IN OUT struct DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8             SubCh;
  UINT8             Strobe;
  UINT8             SubChMSVx4;
  INT8              CycleSetting;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];
  PSYSHOST          Host;

  Host = GetSysHostPointer ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  RcDebugPrintLine (SDBG_DEFAULT, 85, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3   4   5   6   7   8   9 | 0   1   2   3   4   5   6   7   8   9\n");

  for (CycleSetting = MDQS_Internal_WRITE_LEVEL_CYCLE_START; CycleSetting <= MDQS_Internal_WRITE_LEVEL_CYCLE_END; CycleSetting++) {
    RcDebugPrint (SDBG_DEFAULT, "%2d  ", CycleSetting);
    for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }

        RcDebugPrint (SDBG_DEFAULT, "   ");
        if (CycleSetting == (*TrainResult) [Channel][SubCh][Strobe].Results ) {
          RcDebugPrint (SDBG_DEFAULT, "1");
        } else {
          RcDebugPrint (SDBG_DEFAULT, "0");
        }
      }
    } // Strobe loop

    RcDebugPrint (SDBG_DEFAULT, "\n");
  } // CycleSetting loop

  //
  // Print rising edges
  //
  RcDebugPrintLine (SDBG_MAX, 85, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  RcDebugPrint (SDBG_DEFAULT, "RE: ");
  for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
    for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
      if (IsStrobeNotValidDdr5 (Host, Strobe)) {
        continue;
      }

      RcDebugPrint (SDBG_DEFAULT, " %3d", (*TrainResult) [Channel][SubCh][Strobe].Results);
    } // Strobe loop
  } // SubCh loop
  RcDebugPrint (SDBG_DEFAULT, "\n");
} // DisplayDwlExtCoarseResult

/**

  This function print the sample array

  @param[in] Socket            - Socket number
  @param[in] Channel           - Channel number
  @param[in] Dimm              - DIMM number
  @param[in][out] TrainResult  - Trained result

  @retval N/A

**/
VOID
DisplayDwlIntFineResult (
  IN UINT8                         Socket,
  IN UINT8                         Channel,
  IN UINT8                         Dimm,
  IN OUT struct DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8             SubCh;
  UINT8             Strobe;
  UINT8             SubChMSVx4;
  INT8              PhaseSetting;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];
  PSYSHOST          Host;

  Host = GetSysHostPointer ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  RcDebugPrintLine (SDBG_DEFAULT, 85, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3   4   5   6   7   8   9 | 0   1   2   3   4   5   6   7   8   9\n");

  for (PhaseSetting = MDQS_Internal_WRITE_LEVEL_PHASE_START; PhaseSetting < MDQS_Internal_WRITE_LEVEL_PHASE_END; PhaseSetting++) {
    RcDebugPrint (SDBG_DEFAULT, "%2d  ", PhaseSetting);
    for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }

        RcDebugPrint (SDBG_DEFAULT, "   ");
        if (PhaseSetting  == (*TrainResult) [Channel][SubCh][Strobe].ShowResults[PhaseSetting] ) {
          RcDebugPrint (SDBG_DEFAULT, "1");
        } else {
          RcDebugPrint (SDBG_DEFAULT, "0");
        }
      }
    } // Strobe loop

    RcDebugPrint (SDBG_DEFAULT, "\n");
  } // CycleSetting loop

  //
  // Print rising edges
  //
  RcDebugPrintLine (SDBG_MAX, 85, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  RcDebugPrint (SDBG_DEFAULT, "RE: ");
  for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
    for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
      if (IsStrobeNotValidDdr5 (Host, Strobe)) {
        continue;
      }

      RcDebugPrint (SDBG_DEFAULT, " %3d", (*TrainResult) [Channel][SubCh][Strobe].Results);
    } // Strobe loop
  } // SubCh loop
  RcDebugPrint (SDBG_DEFAULT, "\n");
} // DisplayDwlExtCoarseResult

#endif // DEBUG_CODE_BLOCK

/**

  Perform LRDIMM DB to DRAM Internal Write Leveling.

  Performs the DRAM internal WL cycle adjustment, and places the DB MDQS so that the DRAM interna
  WL pulse rising edge (or DRAM receive enable rising edge) is centered in the MDQS write preamble.
  Internal WL is not applicable for DDRT2 DIMM

  @param Host   - Pointer to sysHost
  @param socket - socket number

  @retval status

**/
UINT32
InternalWriteLeveling (
  IN UINT8     Socket
  )
{
  PSYSHOST                          Host;
  INT8                              CycleDelay;
  INT8                              PhaseDelay;
  INT8                              ControlWordDataLowerNibbleDecode;
  INT8                              ControlWordDataUpperNibbleDecode;
  UINT8                             Channel;
  UINT8                             Dimm;
  UINT8                             Rank;
  UINT8                             Strobe;
  UINT8                             MaxChDdr;
  UINT8                             ChannelLocal;
  UINT8                             SubChannel = 0;
  UINT8                             LogRank = 0;
  UINT8                             LogSubRank = 0;
  UINT8                             FaultyPartsStatus;
  UINT8                             SubChMSVx4;
  UINT8                             MaxStrobePairs;
  UINT8                             ControlWordDataLowerNibble;
  UINT8                             ControlWordDataUpperNibble;
  UINT8                             ControlWordDataRWE1;
  UINT8                             ControlWordDataRWE8;
  UINT8                             ControlWordDataRWE9;
  UINT32                            ChBitmask;
  UINT32                            Status = SUCCESS;
  UINT32                            StrobePass[MAX_CH][SUB_CH][MAX_RANK_DIMM];
  DDR5_MODE_REGISTER_3_STRUCT       ModeRegister3;
  ACT_TO_ACT_SAVE_STRUCT            ActToActSave[MAX_CH];
  struct channelNvram               (*ChannelNvList)[MAX_CH];
  struct DwlTrainingResults         (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  struct rankDevice                 (*RankStruct) [MAX_RANK_DIMM];

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  MaxStrobePairs = SubChMSVx4 / 2;

  SetCurrentTestType (Socket, MdqsExternalWriteLevelingCoarseTraining);
  TrainResult = RcAllocatePool (sizeof (*TrainResult));
  ZeroMem ((UINT8 *) TrainResult,      sizeof (*TrainResult));

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "LRDIMM Internal Write Leveling Coarse Training\n");

  DwlDdrioInit (Socket);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    ZeroMem ((UINT8 *) StrobePass, sizeof (StrobePass));
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_LRDIMM_INTERNAL_WRITE_LEVEL_COARSE, (UINT16) ((Socket << 8) | (Dimm << 4) | Rank)));

      FaultyPartsStatus = FPT_NO_ERROR;

      ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, Rank);
      if (ChBitmask == 0) {
        continue;
      }

      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }

        ActToActSave[Channel].TRrdSOrg = 0;
        ActToActSave[Channel].TRrdLOrg = 0;
        ActToActSave[Channel].TRcdRdOrg = 0;
        ActToActSave[Channel].TRrsrOrg = 0;
        ActToActSave[Channel].TRrSgOrg = 0;

        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          ChannelLocal = Channel;
          GetChannelMapping (Channel, SubChannel, &ChannelLocal);
          //
          // Get the logical Rank #
          //
          LogRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);

          SetActToActPeriod (Host, Socket, ChannelLocal, ActToActSave);
          //
          // Indicate this Rank is present on at least one channel
          //
          if ((*ChannelNvList) [Channel].encodedCSMode) {
            LogSubRank = (Rank >> 1);
          } else {
            LogSubRank = 0;
          }

          SelectCPGCRanks (Host, Socket, ChannelLocal, 1 << LogRank, 0, LogSubRank);

          //
          // Initial training result to 0xFF
          //
          SetMem ((UINT8 *) TrainResult, sizeof (*TrainResult), 0xFF);
        } // Subch loop

        //
        // Bypass DDRT2 since it didn't support internal write leveling training
        //
        if (IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
          continue;
        }

        //
        // 1. Initialize Internal Write Leveling Mode
        //
        InitialInternalWriteLevelingTraining (Socket, Channel, Dimm, Rank);

        //
        // 2. Calculate and Program DB MDQS WL Phase Delay
        //
        CalculateAndProgramDbMdqsWlPhaseDelay (Socket, Channel, Dimm, Rank);
      } // Channel loop

      //
      // Program counting window settings and enable receivers/ODT
      //
      if (IsBrsPresent (Host, Socket)) {
        SetDqCountingWindow (Host, Socket, Dimm, Rank);
      } else {
        SetDataTrainCountWindow (Host, Socket, Dimm, Rank, DWL_COUNTING_WINDOW);
      }

      //
      // 3. Sweep the DRAM WL Internal Cycle Alignment from 0 to 15. Note: Higher setting will pull internal pulse earlier.
      //
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "LRDIMM MDQS Internal Coarse Write Leveling - Cycle Scanning:\n");
      for (CycleDelay = MDQS_Internal_WRITE_LEVEL_CYCLE_START; CycleDelay <= MDQS_Internal_WRITE_LEVEL_CYCLE_END; CycleDelay ++) {
        for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
          if (!((1 << Channel) & ChBitmask)) {
            continue;
          }

          //
          // Bypass DDRT2 since it didn't support internal write leveling training
          //
          if (IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
            continue;
          }

          RankStruct = GetRankStruct (Host, Socket, Channel, Dimm);

          ModeRegister3.Data = 0;
#ifdef USE_LATEST_DRAM_SPEC
          ModeRegister3.Bits.write_leveling_internal_cycle_alignment_lower_byte = CycleDelay;
          DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_3_REG, ModeRegister3.Data);
#endif // USE_LATEST_DRAM_SPEC
        }
        GetDwlIntCoarseTrainResults (Socket, Dimm, Rank, CycleDelay, StrobePass, TrainResult);
      } // CycleDelay loop

      EvaluateAndProgramDwlIntCoarseResults (Socket, ChBitmask, Dimm, Rank, TrainResult);

      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "LRDIMM Internal Write Leveling Fine Training\n");
      ZeroMem ((UINT8 *) StrobePass, sizeof (StrobePass));
      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }

        //
        // Bypass DDRT2 since it didn't support internal write leveling training
        //
        if (IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
          continue;
        }

        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          ChannelLocal = Channel;
          GetChannelMapping (Channel, SubChannel, &ChannelLocal);
          //
          // Get the logical Rank #
          //
          LogRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);

          SetActToActPeriod (Host, Socket, ChannelLocal, ActToActSave);
          //
          // Indicate this Rank is present on at least one channel
          //
          if ((*ChannelNvList) [Channel].encodedCSMode) {
            LogSubRank = (Rank >> 1);
          } else {
            LogSubRank = 0;
          }

          SelectCPGCRanks (Host, Socket, ChannelLocal, 1 << LogRank, 0, LogSubRank);

          //
          // Initial training result to 0xFF
          //
          SetMem ((UINT8 *) TrainResult, sizeof (*TrainResult), 0xFF);
        } //Subch loop

        //
        // 1. DB MDQS strobe groups to the following setting: DB WL Cycle Delay - 1
        //
        InitialDbMdqsWlCycleDelay (Socket, Channel, Dimm, Rank);
      } // Channel loop

      //
      // 2. Sweep the DRAM WL Internal Cycle Alignment from 0 to 15. Note: Higher setting will pull internal pulse earlier.
      //
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "LRDIMM MDQS Internal Fine Write Leveling - Phase Scanning:\n");
      for (PhaseDelay = MDQS_Internal_WRITE_LEVEL_PHASE_START; PhaseDelay <= MDQS_Internal_WRITE_LEVEL_PHASE_END; PhaseDelay ++) {
        for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
          if (!((1 << Channel) & ChBitmask)) {
            continue;
          }

          //
          // Bypass DDRT2 since it didn't support internal write leveling training
          //
          if (IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
            continue;
          }

          RankStruct = GetRankStruct (Host, Socket, Channel, Dimm);
          for (SubChannel = 0 ; SubChannel < SUB_CH; SubChannel++) {
            for (Strobe = 0; Strobe < MaxStrobePairs; Strobe++) {
              if (IsStrobeNotValidDdr5 (Host, Strobe)) {
                continue;
              }

              ControlWordDataLowerNibble = (*RankStruct) [Rank].cachedLrBuf_PG01RWE1_IntFine[Strobe + (SubChannel * MaxStrobePairs)] & 0x0F;
              ControlWordDataLowerNibbleDecode = DecodePg01Rwe1 (ControlWordDataLowerNibble);
              ControlWordDataUpperNibble = (*RankStruct) [Rank].cachedLrBuf_PG01RWE1_IntFine[Strobe + (SubChannel * MaxStrobePairs)] & 0xF0;
              ControlWordDataUpperNibbleDecode = DecodePg01Rwe1 (ControlWordDataUpperNibble >> 4);
              ControlWordDataRWE8 = (*RankStruct) [Rank].cachedLrBuf_PG01RWE8[Strobe + (SubChannel * MaxStrobePairs)];
              ControlWordDataRWE9 = (*RankStruct) [Rank].cachedLrBuf_PG01RWE9[Strobe + (SubChannel * MaxStrobePairs)];

              //
              // Sweep range 1: In N-1 cycle delay: from trained phase to 63
              // Sweep range 2: In N cycle delay:   from 0 to trained phase
              //
              if ((ControlWordDataRWE8 + PhaseDelay) <= 63) {
                ControlWordDataRWE8 += PhaseDelay;
              } else {
                ControlWordDataLowerNibbleDecode += 1;
                ControlWordDataRWE8 = ControlWordDataRWE8 + PhaseDelay - 64;
              }
              if ((ControlWordDataRWE9 + PhaseDelay) <= 63) {
                ControlWordDataRWE9 += PhaseDelay;
              } else {
                ControlWordDataUpperNibbleDecode += 1;
                ControlWordDataRWE9 = ControlWordDataRWE9 + PhaseDelay - 64;
              }

              ControlWordDataLowerNibble = ABS (ControlWordDataLowerNibbleDecode);
              if ((ControlWordDataLowerNibbleDecode) < 0) {
                ControlWordDataLowerNibble |= BIT3;
              }
              ControlWordDataUpperNibble = ABS (ControlWordDataUpperNibbleDecode);
              if ((ControlWordDataUpperNibbleDecode) < 0) {
                ControlWordDataUpperNibble |= BIT3;
              }
              ControlWordDataRWE1 = (ControlWordDataUpperNibble << 4) | ControlWordDataLowerNibble;

              //
              // Select DB page for RWE1/RWE8/RWE9
              //
              DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, Rank);

              //
              // Select buffer device and progarm RWE1/8/9 via PBA write
              //
              DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, Strobe);
              DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE1_REG, ControlWordDataRWE1);
              DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE8_REG, ControlWordDataRWE8);
              DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE9_REG, ControlWordDataRWE9);
            } // Strobe loop
          } // SubChannel loop
        } // Channel loop

        GetDwlIntFineTrainResults (Socket, Dimm, Rank, PhaseDelay, StrobePass, TrainResult);
      } // PhaseDelay loop

      EvaluateAndProgramDwlIntFineResults (Socket, ChBitmask, Dimm, Rank, TrainResult);

      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        if (!((1 << Channel) & ChBitmask)) {
          continue;
        }

        CleanupWriteLevelingMode (Socket, Channel, Dimm, Rank);
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          ChannelLocal = Channel;
          GetChannelMapping (Channel, SubChannel, &ChannelLocal);
          RestoreActToActPeriod (Socket, ChannelLocal, ActToActSave);
        } // SubChannel loop
      } // Channel loop
    } //Rank loop

    Status = KEEP_FAILURE (RestoreDimmRankPresence (Socket, Dimm), Status);

  } //Dimm loop

  // DDRIO cleanup
  PostDwlDdrioProgramming (Socket);

  FifoTrainReset (Host, Socket);
  RcFreePool (TrainResult);
  return Status;
} // InternalWriteLeveling

/**

  Initial External Write Leveling Training

  @param[in] Socket  - Socket Number
  @param[in] Channel - Channel Number
  @param[in] Dimm    - Dimm Number
  @param[in] Rank    - Rank Number

  @retval N/A

**/
STATIC
VOID
EFIAPI
InitialInternalWriteLevelingTraining (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  struct rankDevice                 (*RankStruct) [MAX_RANK_DIMM];
  DDR5_MODE_REGISTER_2_STRUCT       ModeRegister2;
  PSYSHOST                          Host;

  Host = GetSysHostPointer ();
  RankStruct = GetRankStruct (Host, Socket, Channel, Dimm);

  //
  // Select all DRAM devices
  //
  WriteMPC (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, Rank, PDA_SELECT_ALL, TIMING_4N, TRUE);

  //
  // Enable DRAM Write Leveling training mode
  //
  ModeRegister2.Data = 0;
  ModeRegister2.Bits.write_leveling = 1;
  ModeRegister2.Bits.internal_write_timing = 1;
  DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_2_REG, ModeRegister2.Data);

  //
  // Select All DBs and Enable DB Write Leveling
  //
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW83_REG, LRDIMM_WRLVL_TRAINING);

  (*RankStruct) [Rank].CurrentLrdimmTrainingMode = LRDIMM_DWL_INT_TRAINING_MODE;
} // InitialInternalWriteLevelingTraining

/**

  Calculate DB MDQS WL Phase Delay

  @param[in] Socket  - Socket Number
  @param[in] Channel - Channel Number
  @param[in] Dimm    - Dimm Number
  @param[in] Rank    - Rank Number

  @retval N/A

**/
STATIC
VOID
EFIAPI
CalculateAndProgramDbMdqsWlPhaseDelay (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  UINT8                             Strobe;
  UINT8                             SubChannel;
  UINT8                             MaxStrobePairs;
  UINT8                             WritePreamble;
  UINT8                             ControlWordDataRWE1;
  UINT8                             ControlWordDataRWE8;
  UINT8                             ControlWordDataRWE9;
  UINT8                             ControlWordDataLowerNibble;
  UINT8                             ControlWordDataUpperNibble;
  INT8                              ControlWordDataLowerNibbleDecode;
  INT8                              ControlWordDataUpperNibbleDecode;
  struct rankDevice                 (*RankStruct) [MAX_RANK_DIMM];
  PSYSHOST                          Host;

  Host = GetSysHostPointer ();
  RankStruct = GetRankStruct (Host, Socket, Channel, Dimm);
  MaxStrobePairs = GetSubChMaxStrobeValid (Host) / 2;

  WritePreamble = GetPreambleState (Socket, WRITE_PREAMBLE);

  //
  // Select appropriate DB page and program RWE8/9 for DB Write Level Phase Delay
  //
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, Rank);

  for (SubChannel = 0 ; SubChannel < SUB_CH; SubChannel++) {
    for (Strobe = 0; Strobe < MaxStrobePairs; Strobe++) {
      if (IsStrobeNotValidDdr5 (Host, Strobe)) {
        continue;
      }

      //
      // Retrieve RWE1/RWE8/RWE9 from cache
      //
      ControlWordDataLowerNibble = (*RankStruct) [Rank].cachedLrBuf_PG01RWE1[Strobe + (SubChannel * MaxStrobePairs)] & 0x0F;
      ControlWordDataLowerNibbleDecode = DecodePg01Rwe1 (ControlWordDataLowerNibble);
      ControlWordDataUpperNibble = (*RankStruct) [Rank].cachedLrBuf_PG01RWE1[Strobe + (SubChannel * MaxStrobePairs)] & 0xF0;
      ControlWordDataUpperNibbleDecode = DecodePg01Rwe1 (ControlWordDataUpperNibble >> 4);
      ControlWordDataRWE8 = (*RankStruct) [Rank].cachedLrBuf_PG01RWE8[Strobe + (SubChannel * MaxStrobePairs)];
      ControlWordDataRWE9 = (*RankStruct) [Rank].cachedLrBuf_PG01RWE9[Strobe + (SubChannel * MaxStrobePairs)];

      //
      // Adjust for Write preamble
      // 2a. Example for 2 tCK preamble. 0.75 tCK = 48 ticks.
      //    If MDQS >= 48, then (MDQS = MDQS + -48)
      //    If MDQS < 48, then (MDQS Cycle = MDQS Cycle - 1) and (MDQS = MDQS + -48 + 64)
      // 2b. Example for 3 tCK preamble. 1.25 tCK = 1 cycle + 0.25 tCK; 0.25 tCK = 16 ticks
      //    If MDQS >= 16, then (MDQS Cycle = MDQS Cycle - 1) and (MDQS = MDQS + -16)
      //    If MDQS < 16, then (MDQS Cycle = MDQS Cycle - 2) and (MDQS = MDQS + -16 + 64)
      //
      switch (WritePreamble) {
        case PREAMBLE_2TCLK:
          if (ControlWordDataRWE8 >= 48) {
            ControlWordDataRWE8 -= 48;
          } else {
            ControlWordDataLowerNibbleDecode -= 1;
            ControlWordDataRWE8 = ControlWordDataRWE8 - 48 + 64;
          }

          if (ControlWordDataRWE9 >= 48) {
            ControlWordDataRWE9 -= 48;
          } else {
            ControlWordDataUpperNibbleDecode -= 1;
            ControlWordDataRWE9 = ControlWordDataRWE9 - 48 + 64;
          }
          break;
        case PREAMBLE_3TCLK:
          if (ControlWordDataRWE8 >= 16) {
            ControlWordDataLowerNibbleDecode -= 1;
            ControlWordDataRWE8 -= 16;
          } else {
            ControlWordDataLowerNibbleDecode -= 2;
            ControlWordDataRWE8 = ControlWordDataRWE8 - 16 + 64;
          }

          if (ControlWordDataRWE9 >= 16) {
            ControlWordDataUpperNibbleDecode -= 1;
            ControlWordDataRWE9 -= 16;
          } else {
            ControlWordDataUpperNibbleDecode -= 2;
            ControlWordDataRWE9 = ControlWordDataRWE9 - 16 + 64;
          }
          break;
        case PREAMBLE_4TCLK:
        default:
          break;
      }

      ControlWordDataLowerNibble = ABS (ControlWordDataLowerNibbleDecode);
      if (ControlWordDataLowerNibbleDecode < 0) {
        ControlWordDataLowerNibble |= BIT3;
      }
      ControlWordDataUpperNibble = ABS (ControlWordDataUpperNibbleDecode);
      if (ControlWordDataUpperNibbleDecode < 0) {
        ControlWordDataUpperNibble |= BIT3;
      }

      ControlWordDataRWE1 = (ControlWordDataUpperNibble << 4) | ControlWordDataLowerNibble;

      //
      // Select appropriate buffer device and progarm final value to RWE1/8/9 via PBA write
      //
      DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, Strobe);

      DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE1_REG, ControlWordDataRWE1);
      DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE8_REG, ControlWordDataRWE8);
      DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE9_REG, ControlWordDataRWE9);

      //
      // Record the DB WL Cycle/Phase delay as it will be consumed later
      //
      (*RankStruct) [Rank].cachedLrBuf_PG01RWE1[Strobe + (SubChannel * MaxStrobePairs)] = ControlWordDataRWE1;
      (*RankStruct) [Rank].cachedLrBuf_PG01RWE8[Strobe + (SubChannel * MaxStrobePairs)] = ControlWordDataRWE8;
      (*RankStruct) [Rank].cachedLrBuf_PG01RWE9[Strobe + (SubChannel * MaxStrobePairs)] = ControlWordDataRWE9;
    }

    //
    // Set all DB devices back to RW93
    //
    DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);
  }
} // CalculateAndProgramDbMdqsWlPhaseDelay

/**

  Decode DB Page[1:0] RWE1

  @param[in] Rwe1setting  - RWE1 current setting

  @retval Actual write leveling timing latency adder

**/
INT8
DecodePg01Rwe1 (
  IN UINT8 Rwe1setting
)
{
  INT8 Rwe1decode;

  if ((Rwe1setting & BIT3) == BIT3) {
    Rwe1decode = 0 - (INT8) (Rwe1setting & ~BIT3);
  } else {
    Rwe1decode = (INT8) Rwe1setting;
  }

  return Rwe1decode;
} // DecodePg01Rwe1

/**

  Get's the results from DWL - internal Coarse

  @param[in] Socket           - Socket Number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number of the DIMM
  @param[in] CycleDelay       - Cycle delay
  @param[in][out] StrobePass  - Strobe Pass information
  @param[in][out] TrainResult - Store trained result

  @retval N/A

**/
VOID
GetDwlIntCoarseTrainResults (
  IN UINT8                          Socket,
  IN UINT8                          Dimm,
  IN UINT8                          Rank,
  IN INT8                           CycleDelay,
  IN OUT UINT32                     StrobePass [MAX_CH][SUB_CH][MAX_RANK_DIMM],
  IN OUT struct DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8                         Channel;
  UINT8                         Strobe;
  UINT8                         SwizzleStrobe;
  UINT8                         MaxChDdr;
  UINT8                         SubCh = 0;
  UINT8                         SubChMSVx4;
  UINT32                        LrdimmFeedBack;
  UINT32                        ChBitmask;
  struct channelNvram           (*ChannelNvList)[MAX_CH];
  PSYSHOST                      Host;

  Host = GetSysHostPointer ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, Rank);

  //
  // Execute test
  //
  ExecuteBacksideInternalTest (Socket, ChBitmask, Dimm, Rank);

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }

    if (IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
      continue;
    }

    for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        if ((StrobePass [Channel][SubCh][Rank]) & (1 << Strobe)) {
          continue;
        }

        SwizzleStrobe = CheckBacksideSwizzle (Host, Socket, Channel, Dimm, Strobe);
        LrdimmFeedBack = ReadDataTrainCounterFeedbackDdr5 (Host, Socket, Channel,  SubCh, SwizzleStrobe);

        if (LrdimmFeedBack > DWL_WRITE_CYCLE / 2) {
          //
          // 4.b If this is greater than 16 (number of writes)/2, then DQ sample is 1
          //
          if (((StrobePass [Channel][SubCh][Rank]) & (1 << Strobe)) == 0) {
            (*TrainResult) [Channel][SubCh][Strobe].Results = CycleDelay;
          }

          (*TrainResult) [Channel][SubCh][Strobe].ShowResults[CycleDelay] = CycleDelay;
          //
          // Record the strobe already passed
          //
          StrobePass [Channel][SubCh][Rank] |= (1 << Strobe);
        }
      } // strobe loop
    } // SubCh loop
  } // Channel loop
} // GetDwlIntCoarseTrainResults

/**

  Evaluates and prints out the results from the DWL Internal Coarse and programs
  the cycle result

  @param[in] Socket      - Socket number
  @param[in] ChBitmask   - Channel bit mask
  @param[in] Dimm        - Dimm number
  @param[in] LrTrainRes  - Training result

  @retval N/A

**/
VOID
EvaluateAndProgramDwlIntCoarseResults (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN struct    DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8              Channel;
  UINT8              MaxChDdr;
  UINT8                             SubChannel;
  UINT8                             Strobe;
  UINT8                             SubChMSVx4;
  UINT8                             FaultyPartsStatus;
  struct ddrRank                    (*RankList)[MAX_RANK_DIMM];
  struct rankDevice                 (*RankStruct) [MAX_RANK_DIMM];
  struct dimmNvram                  (*DimmNvList)[MAX_DIMM];
  PSYSHOST                          Host;
  DDR5_MODE_REGISTER_3_STRUCT       ModeRegister3;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }

    if (IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
      continue;
    }

    RankList = GetRankNvList (Host, Socket, Channel, Dimm);
    RankStruct = GetRankStruct (Host, Socket, Channel, Dimm);
    DimmNvList = GetDimmNvList (Host, Socket, Channel);

    for (SubChannel = 0 ; SubChannel < SUB_CH; SubChannel++) {
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }

        if ((UINT8) (*TrainResult) [Channel][SubChannel][Strobe].Results == 0xFF) {
          (*TrainResult) [Channel][SubChannel][Strobe].Results = 0;
          //
          // Failure, mark this part as faulty
          //
          (*RankList)[Rank].faultyParts[Strobe] |= FPT_LRDIMM_TRAINING_FAILED;
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, Strobe, NO_BIT,
            "LRDIMM DWL Internal Coarse - FAILED WRITE CYCLE\n");
        }
      } //Strobe

      //
      // Program final training result via PDA ca passthru mode
      //
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        WriteMPC (Host, Socket, Channel, Dimm, 1 << SubChannel, Rank, PDA_SELECT_ID | Strobe, TIMING_4N, TRUE);

        ModeRegister3.Data = 0;
#ifdef USE_LATEST_DRAM_SPEC
        ModeRegister3.Bits.write_leveling_internal_cycle_alignment_lower_byte = (*TrainResult) [Channel][SubChannel][Strobe].Results;
        DramModeRegWrite (Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_MODE_REGISTER_3_REG, ModeRegister3.Data);
#endif // USE_LATEST_DRAM_SPEC
      } // Strobe loop

      //
      // Restore PDA to select_all
      //
      WriteMPC (Host, Socket, Channel, Dimm, 1 << SubChannel, Rank, PDA_SELECT_ALL, TIMING_4N, TRUE);

      FaultyPartsStatus = EvaluateFaultyParts (Host, Socket, Channel);
      if (FaultyPartsStatus == FPT_CORRECTABLE_ERROR) {
        EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_DWL_INT_COARSE_TRAINING, Socket, Channel, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, MrcGtDelim, LrbufLevel, 0xFF);
      } else if (FaultyPartsStatus == FPT_NO_CORRECTABLE_ERROR) {
        EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_DWL_INT_COARSE_TRAINING, Socket, Channel, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, MrcGtDelim, LrbufLevel, 0xFF);
        DisableRank (Host, Socket, Channel, Dimm, Rank);
      }
    } // SubCh loop

#ifdef DEBUG_CODE_BLOCK
    if (GetDebugLevel () & SDBG_MAX) {
      AcquirePrintControl ();
      RcDebugPrint (SDBG_DEFAULT, "\nSummary: LRDIMM Internal Write Leveling Coarse\nS%d, Ch%d, DIMM%d, Rank%d\n", Socket, Channel, Dimm, Rank);
      DisplayDwlIntCoarseResult (Socket, Channel, Dimm, TrainResult);
      ReleasePrintControl ();
    }
#endif // DEBUG_CODE_BLOCK
  } // Channel loop
} // EvaluateAndProgramDwlIntCoarseResults

/**

  Initial DB MDQS WL Cycle Delay for DWL Internal Fine training

  @param[in] Socket  - Socket Number
  @param[in] Channel - Channel Number
  @param[in] Dimm    - Dimm Number
  @param[in] Rank    - Rank Number

  @retval N/A

**/
STATIC
VOID
EFIAPI
InitialDbMdqsWlCycleDelay (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  UINT8                             Strobe;
  UINT8                             SubChannel;
  UINT8                             MaxStrobePairs;
  UINT8                             WritePreamble;
  UINT8                             ControlWordDataRWE1;
  UINT8                             ControlWordDataLowerNibble;
  UINT8                             ControlWordDataUpperNibble;
  INT8                              ControlWordDataLowerNibbleDecode;
  INT8                              ControlWordDataUpperNibbleDecode;
  struct rankDevice                 (*RankStruct) [MAX_RANK_DIMM];
  struct dimmNvram                  (*DimmNvList) [MAX_DIMM];
  PSYSHOST                          Host;

  Host = GetSysHostPointer ();

  DimmNvList = GetDimmNvList (Host, Socket, Channel);
  RankStruct = GetRankStruct (Host, Socket, Channel, Dimm);
  MaxStrobePairs = GetSubChMaxStrobeValid (Host) / 2;
  WritePreamble = GetPreambleState (Socket, WRITE_PREAMBLE);

  //
  // Select appropriate DB page and program RWE8/9 for DB Write Level Phase Delay
  //
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, Rank);

  for (SubChannel = 0 ; SubChannel < SUB_CH; SubChannel++) {
    for (Strobe = 0; Strobe < MaxStrobePairs; Strobe++) {
      if (IsStrobeNotValidDdr5 (Host, Strobe)) {
        continue;
      }

      ControlWordDataLowerNibble = (*RankStruct) [Rank].cachedLrBuf_PG01RWE1[Strobe + (SubChannel * MaxStrobePairs)] & 0x0F;
      ControlWordDataLowerNibbleDecode = DecodePg01Rwe1 (ControlWordDataLowerNibble);
      ControlWordDataUpperNibble = (*RankStruct) [Rank].cachedLrBuf_PG01RWE1[Strobe + (SubChannel * MaxStrobePairs)] & 0xF0;
      ControlWordDataUpperNibbleDecode = DecodePg01Rwe1 (ControlWordDataUpperNibble >> 4);

      //
      // 1. Set all DB MDQS strobe groups to the following setting: DB WL Cycle Delay - 1
      //    This is to provide range for the fine phase sweep as we search the 0 to 1 transition
      //
      ControlWordDataLowerNibble = ABS (ControlWordDataLowerNibbleDecode - 1);
      if ((ControlWordDataLowerNibbleDecode - 1) < 0) {
        ControlWordDataLowerNibble |= BIT3;
      }
      ControlWordDataUpperNibble = ABS (ControlWordDataUpperNibbleDecode - 1);
      if ((ControlWordDataUpperNibbleDecode - 1) < 0) {
        ControlWordDataUpperNibble |= BIT3;
      }
      ControlWordDataRWE1 = (ControlWordDataUpperNibble << 4) | ControlWordDataLowerNibble;

      //
      // Select appropriate buffer device and progarm final value to RWE1/8/9 via PBA write
      //
      DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, Strobe);
      DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE1_REG, ControlWordDataRWE1);

      //
      // Record the DB WL cycle delay as it will be consumed later
      //
      (*RankStruct) [Rank].cachedLrBuf_PG01RWE1_IntFine[Strobe + (SubChannel * MaxStrobePairs)] = ControlWordDataRWE1;
    }

    //
    // Set all buffer device back to RW93
    //
    DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);
  }
} // InitialDbMdqsWlCycleDelay

/**

  Get's the results from DWL - internal Fine

  @param[in] Socket           - Socket Number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number of the DIMM
  @param[in] PhaseDelay       - Phase delay
  @param[in][out] StrobePass  - Strobe Pass information
  @param[in][out] TrainResult - Store trained result

  @retval N/A

**/
VOID
GetDwlIntFineTrainResults (
  IN UINT8                          Socket,
  IN UINT8                          Dimm,
  IN UINT8                          Rank,
  IN INT8                           PhaseDelay,
  IN OUT UINT32                     StrobePass [MAX_CH][SUB_CH][MAX_RANK_DIMM],
  IN OUT struct DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8                         Channel;
  UINT8                         Strobe;
  UINT8                         SwizzleStrobe;
  UINT8                         MaxChDdr;
  UINT8                         SubCh = 0;
  UINT8                         SubChMSVx4;
  UINT32                        LrdimmFeedBack;
  UINT32                        ChBitmask;
  struct channelNvram           (*ChannelNvList)[MAX_CH];
  PSYSHOST                      Host;

  Host = GetSysHostPointer ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, Rank);

  //
  // Execute test
  //
  ExecuteBacksideInternalTest (Socket, ChBitmask, Dimm, Rank);

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }

    //
    // Bypass DDRT2 since it didn't support internal write leveling training
    //
    if (IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
      continue;
    }

    for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        SwizzleStrobe = CheckBacksideSwizzle (Host, Socket, Channel, Dimm, Strobe);
        LrdimmFeedBack = ReadDataTrainCounterFeedbackDdr5 (Host, Socket, Channel,  SubCh, SwizzleStrobe);

        if (LrdimmFeedBack > DWL_WRITE_CYCLE / 2) {
          //
          // 4.b If this is greater than 16 (number of writes)/2, then DQ sample is 1
          //
          if (((StrobePass [Channel][SubCh][Rank]) & (1 << Strobe)) == 0) {
            (*TrainResult) [Channel][SubCh][Strobe].Results = PhaseDelay;
          }

          (*TrainResult) [Channel][SubCh][Strobe].ShowResults[PhaseDelay] = PhaseDelay;
          //
          // Record the strobe already passed
          //
          StrobePass [Channel][SubCh][Rank] |= (1 << Strobe);
        }
      } // strobe loop
    } // SubCh loop
  } // Channel loop
} // GetDwlIntFineTrainResults

/**

  Evaluates and prints out the results from the DWL Internal Fine and programs
  the Phase/Cycle result

  @param[in] Socket      - Socket number
  @param[in] ChBitmask   - Channel bit mask
  @param[in] Dimm        - Dimm number
  @param[in] LrTrainRes  - Training result

  @retval N/A

**/
VOID
EvaluateAndProgramDwlIntFineResults (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN struct    DwlTrainingResults (*TrainResult) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8                       Channel;
  UINT8                       MaxChDdr;
  UINT8                       SubChannel;
  UINT8                       Strobe;
  UINT8                       SubChMSVx4;
  UINT8                       MaxStrobePairs;
  UINT8                       FaultyPartsStatus;
  UINT8                       WritePreamble;
  UINT8                       ControlWordDataRWE1;
  UINT8                       ControlWordDataRWE8;
  UINT8                       ControlWordDataRWE9;
  UINT8                       ControlWordDataLowerNibble;
  UINT8                       ControlWordDataUpperNibble;
  UINT8                       ControlWordDataLowerNibbleRWE8;
  UINT8                       ControlWordDataUpperNibbleRWE9;
  INT8                        ControlWordDataLowerNibbleDecode;
  INT8                        ControlWordDataUpperNibbleDecode;
  struct ddrRank              (*RankList)[MAX_RANK_DIMM];
  struct rankDevice           (*RankStruct) [MAX_RANK_DIMM];
  struct dimmNvram            (*DimmNvList)[MAX_DIMM];
  BOOLEAN                     (*NibbleSwap) [MAX_CH][MAX_DIMM][SUB_CH][MAX_STROBE_DDR5 / 2];
  PSYSHOST                    Host;
  DDR5_LRDIMM_TRAINING_DATA   *TrainingData;
  UINT8                       CyclesToAdd;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  MaxStrobePairs = SubChMSVx4 / 2;

  WritePreamble = GetPreambleState (Socket, WRITE_PREAMBLE);

  //
  // Retrieve nibble swap info from MRE
  //
  GetDdr5LrdimmTrainingData (&TrainingData);
  NibbleSwap = &(TrainingData->NibbleSwap);

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }

    //
    // Bypass DDRT2 since it didn't support internal write leveling training
    //
    if (IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
      continue;
    }

    RankList = GetRankNvList (Host, Socket, Channel, Dimm);
    RankStruct = GetRankStruct (Host, Socket, Channel, Dimm);
    DimmNvList = GetDimmNvList (Host, Socket, Channel);

    for (SubChannel = 0 ; SubChannel < SUB_CH; SubChannel++) {
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }

        if ((UINT8) (*TrainResult) [Channel][SubChannel][Strobe].Results == 0xFF) {
          (*TrainResult) [Channel][SubChannel][Strobe].Results = 0;
          //
          // Failure, mark this part as faulty
          //
          (*RankList)[Rank].faultyParts[Strobe] |= FPT_LRDIMM_TRAINING_FAILED;
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, Strobe, NO_BIT,
            "LRDIMM DWL Internal Fine - FAILED WRITE PHASE\n");
        }
      } // Strobe loop

      //
      // Program final training result
      //
      for (Strobe = 0; Strobe < MaxStrobePairs; Strobe++) {
        //
        // Read RWE1/8/9 from cache and de-swap it when Nibble Swap behind of DB
        //
        if ((*NibbleSwap) [Channel][Dimm][SubChannel][Strobe] == TRUE) {
          ControlWordDataLowerNibble = (*RankStruct) [Rank].cachedLrBuf_PG01RWE1_IntFine[Strobe + (SubChannel * MaxStrobePairs)] & 0xF0;
          ControlWordDataLowerNibbleDecode = DecodePg01Rwe1 (ControlWordDataLowerNibble >> 4);
          ControlWordDataUpperNibble = (*RankStruct) [Rank].cachedLrBuf_PG01RWE1_IntFine[Strobe + (SubChannel * MaxStrobePairs)] & 0x0F;
          ControlWordDataUpperNibbleDecode = DecodePg01Rwe1 (ControlWordDataUpperNibble);
          ControlWordDataLowerNibbleRWE8 = (*RankStruct) [Rank].cachedLrBuf_PG01RWE9[Strobe + (SubChannel * MaxStrobePairs)];
          ControlWordDataUpperNibbleRWE9 = (*RankStruct) [Rank].cachedLrBuf_PG01RWE8[Strobe + (SubChannel * MaxStrobePairs)];
        } else {
          ControlWordDataLowerNibble = (*RankStruct) [Rank].cachedLrBuf_PG01RWE1_IntFine[Strobe + (SubChannel * MaxStrobePairs)] & 0x0F;
          ControlWordDataLowerNibbleDecode = DecodePg01Rwe1 (ControlWordDataLowerNibble);
          ControlWordDataUpperNibble = (*RankStruct) [Rank].cachedLrBuf_PG01RWE1_IntFine[Strobe + (SubChannel * MaxStrobePairs)] & 0xF0;
          ControlWordDataUpperNibbleDecode = DecodePg01Rwe1 (ControlWordDataUpperNibble >> 4);
          ControlWordDataLowerNibbleRWE8 = (*RankStruct) [Rank].cachedLrBuf_PG01RWE8[Strobe + (SubChannel * MaxStrobePairs)];
          ControlWordDataUpperNibbleRWE9 = (*RankStruct) [Rank].cachedLrBuf_PG01RWE9[Strobe + (SubChannel * MaxStrobePairs)];
        }

        //
        // Final trained phase should be phase cached setting + the edge code in sweep
        //
        ControlWordDataRWE8 = (*TrainResult) [Channel][SubChannel][Strobe].Results + ControlWordDataLowerNibbleRWE8;
        ControlWordDataRWE9 = (*TrainResult) [Channel][SubChannel][Strobe + MaxStrobePairs].Results + ControlWordDataUpperNibbleRWE9;

        //
        // Select appropriate buffer device and progarm final value to RWE8/9
        //
        DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, Strobe);

        //
        // 3.a  Example for 2 tCK preamble. 1.25 tCK = 1 cycle + 0.25 tCK; 0.25 tCK = 16 ticks
        //   If MDQS < 48, then (MDQS Cycle = MDQS Cycle +1) and (MDQS = MDQS + 16)
        //   If MDQS >= 48, then (MDQS Cycle = MDQS Cycle + 2) and (MDQS = MDQS + 16 - 64)
        // 3.b  Example for 3 tCK preamble. 1.75 tCK = 1 cycle + 0.75 tCK; 0.75 tCK = 48 ticks
        //   If MDQS < 16, then (MDQS Cycle = MDQS Cycle + 1) and (MDQS = MDQS + 48)
        //   f MDQS >= 16, then (MDQS Cycle = MDQS Cycle + 2) and (MDQS = MDQS +48 - 64)
        //
        switch (WritePreamble) {
          case PREAMBLE_2TCLK:
            // Lower nibble calculations
            CyclesToAdd = (ControlWordDataRWE8 + MDQS_INTERNAL_WL_25_TCK) / MDQS_INTERNAL_WL_TCK_PER_CYCLE;
            // Always increment Cycle delay by 1 - see above calculations
            ControlWordDataLowerNibbleDecode = ControlWordDataLowerNibbleDecode + CyclesToAdd + 1;

            ControlWordDataRWE8 = ControlWordDataRWE8 - (CyclesToAdd * MDQS_INTERNAL_WL_TCK_PER_CYCLE) + MDQS_INTERNAL_WL_25_TCK;

            // Upper nibble calculations
            CyclesToAdd = (ControlWordDataRWE9 + MDQS_INTERNAL_WL_25_TCK) / MDQS_INTERNAL_WL_TCK_PER_CYCLE;
            ControlWordDataUpperNibbleDecode = ControlWordDataUpperNibbleDecode + CyclesToAdd + 1;

            ControlWordDataRWE9 = ControlWordDataRWE9 - (CyclesToAdd * MDQS_INTERNAL_WL_TCK_PER_CYCLE) + MDQS_INTERNAL_WL_25_TCK;
            break;
          case PREAMBLE_3TCLK:
            // Lower nibble calculations
            CyclesToAdd = (ControlWordDataRWE8 + MDQS_INTERNAL_WL_75_TCK) / MDQS_INTERNAL_WL_TCK_PER_CYCLE;
            ControlWordDataLowerNibbleDecode = ControlWordDataLowerNibbleDecode + CyclesToAdd + 1;

            ControlWordDataRWE8 = ControlWordDataRWE8 - (CyclesToAdd * MDQS_INTERNAL_WL_TCK_PER_CYCLE) + MDQS_INTERNAL_WL_75_TCK;

            // Upper nibble calculations
            CyclesToAdd = (ControlWordDataRWE9 + MDQS_INTERNAL_WL_75_TCK) / MDQS_INTERNAL_WL_TCK_PER_CYCLE;
            ControlWordDataUpperNibbleDecode = ControlWordDataUpperNibbleDecode + CyclesToAdd + 1;

            ControlWordDataRWE9 = ControlWordDataRWE9 - (CyclesToAdd * MDQS_INTERNAL_WL_TCK_PER_CYCLE) + MDQS_INTERNAL_WL_75_TCK;
            break;
          case PREAMBLE_4TCLK:
          default:
            break;
        }

        ControlWordDataLowerNibble = ABS (ControlWordDataLowerNibbleDecode);
        if (ControlWordDataLowerNibbleDecode < 0) {
          ControlWordDataLowerNibble |= BIT3;
        }
        ControlWordDataUpperNibble = ABS (ControlWordDataUpperNibbleDecode);
        if (ControlWordDataUpperNibbleDecode < 0) {
          ControlWordDataUpperNibble |= BIT3;
        }

        if ((*NibbleSwap) [Channel][Dimm][SubChannel][Strobe] == TRUE) {
          ControlWordDataRWE1 = (ControlWordDataLowerNibble << 4) | ControlWordDataUpperNibble;
          //
          // ControlWordData need to swap if there is Nibble Swap behind of Data Buffer
          //
          DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE8_REG, ControlWordDataRWE9);
          DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE9_REG, ControlWordDataRWE8);
          //
          // Save RWE8/9
          //
          (*RankStruct) [Rank].cachedLrBuf_PG01RWE8[Strobe + (SubChannel * MaxStrobePairs)] = ControlWordDataRWE9;
          (*RankStruct) [Rank].cachedLrBuf_PG01RWE9[Strobe + (SubChannel * MaxStrobePairs)] = ControlWordDataRWE8;
        } else {
          ControlWordDataRWE1 = (ControlWordDataUpperNibble << 4) | ControlWordDataLowerNibble;
          //
          // ControlWordData need to swap if there is Nibble Swap behind of Data Buffer
          //
          DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE8_REG, ControlWordDataRWE8);
          DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE9_REG, ControlWordDataRWE9);
          //
          // Save RWE8/9
          //
          (*RankStruct) [Rank].cachedLrBuf_PG01RWE8[Strobe + (SubChannel * MaxStrobePairs)] = ControlWordDataRWE8;
          (*RankStruct) [Rank].cachedLrBuf_PG01RWE9[Strobe + (SubChannel * MaxStrobePairs)] = ControlWordDataRWE9;
        }
        DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE1_REG, ControlWordDataRWE1);

        //
        // Save RWE1
        //
        (*RankStruct) [Rank].cachedLrBuf_PG01RWE1[Strobe + (SubChannel * MaxStrobePairs)] = ControlWordDataRWE1;
      } // Strobe loop

      //
      // Set all buffer device back to RW93
      //
      DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);

      FaultyPartsStatus = EvaluateFaultyParts (Host, Socket, Channel);
      if (FaultyPartsStatus == FPT_CORRECTABLE_ERROR) {
        EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_DWL_INT_FINE_TRAINING, Socket, Channel, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, MrcGtDelim, LrbufLevel, 0xFF);
      } else if (FaultyPartsStatus == FPT_NO_CORRECTABLE_ERROR) {
        EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_DWL_INT_FINE_TRAINING, Socket, Channel, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, MrcGtDelim, LrbufLevel, 0xFF);
        DisableRank (Host, Socket, Channel, Dimm, Rank);
      }
    } // SubCh loop

#ifdef DEBUG_CODE_BLOCK
    if (GetDebugLevel () & SDBG_MAX) {
      AcquirePrintControl ();
      RcDebugPrint (SDBG_DEFAULT, "\nSummary: LRDIMM Internal Write Leveling Fine\nS%d, Ch%d, DIMM%d, Rank%d\n", Socket, Channel, Dimm, Rank);
      DisplayDwlIntFineResult (Socket, Channel, Dimm, TrainResult);
      ReleasePrintControl ();
    }
#endif // DEBUG_CODE_BLOCK
  } // Channel loop
} // EvaluateAndProgramDwlIntFineResults

/**

  Sets up and executes the MRE CPGC test which sends MRR commands to the DRAM.

  @param[in]  Socket    - Socket number
  @param[in]  ChBitmask - Mask of channels on which test should be executed
  @param[in]  Dimm      - Dimm number
  @param[in]  Rank      - Rank number of the DIMM

**/
VOID
ExecuteBacksideInternalTest (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  PSYSHOST                        Host;

  Host = GetSysHostPointer ();
  UINT8   DumArr[7] = {0, 0, 0, 0, 0, 0, 0};

  //
  // Clear train_cnt before issuing next test
  //
  IO_Reset (Host, Socket);

  //
  // Setup CPGC to execute single cacheline write loopback test
  //
  SetupLrdimmWriteTest (Socket, ChBitmask, DWL_WRITE_CYCLE, 1, 16);

  //
  // Execute the CPGC MPR WR sequence
  //
  RunIOTest (Host, Socket, ChBitmask, BasicVA, DumArr, 0, 0);

  if (IsBrsPresent (Host, Socket)) {
    EnableDdrioTrainingCounter (Host, Socket, Dimm, Rank);
  }

  if (FeaturePcdGet (PcdCosimBuild) == TRUE) {
    //
    // Delay if this is a CTE build, otherwise do nothing.
    //
    CteDelayDclk (100);
  } else {
    FixedDelayMicroSecond (1);
  }
} //ExecuteBacksideInternalTest

/**

  Phase Spot Pre-Check is to ensure External Fine sweep is able to catch 0->1 transition

  @param[in]  Socket    - Socket number
  @param[in]  ChBitmask - Mask of channels on which test should be executed
  @param[in]  Dimm      - Dimm number
  @param[in]  Rank      - Rank number of the DIMM

**/
VOID
PhaseSpotsPreCheck (
  IN  UINT8                  Socket,
  IN  UINT32                 ChBitmask,
  IN  UINT8                  Dimm,
  IN  UINT8                  Rank
  )
{
  PSYSHOST                   Host;
  INT8                       Index = 0;
  INT8                       ControlWordDataLowerNibbleDecode;
  INT8                       ControlWordDataUpperNibbleDecode;
  UINT8                      Channel;
  UINT8                      SubChannel;
  UINT8                      Strobe;
  UINT8                      StrobePair;
  UINT8                      MaxChDdr;
  UINT8                      SubChMSVx4;
  UINT8                      SwizzleStrobe;
  UINT8                      MaxStrobePairs;
  UINT8                      ControlWordDataRWE1;
  UINT8                      ControlWordDataLowerNibble;
  UINT8                      ControlWordDataUpperNibble;
  UINT8                      PhaseSpot[PHASE_SPOT_CHECK_NUM] = {0, 31, 63};
  UINT32                     LrdimmFeedBack[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2][PHASE_SPOT_CHECK_NUM];
  BOOLEAN                    UpdateRwE1 = FALSE;
  BOOLEAN                    (*NibbleSwap) [MAX_CH][MAX_DIMM][SUB_CH][MAX_STROBE_DDR5 / 2];
  struct rankDevice          (*RankStruct) [MAX_RANK_DIMM];
  DDR5_LRDIMM_TRAINING_DATA  *TrainingData;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  MaxStrobePairs = GetSubChMaxStrobeValid (Host) / 2;
  ZeroMem ((UINT8 *) LrdimmFeedBack, sizeof (LrdimmFeedBack));

  //
  // Retrieve nibble swap info from MRE
  //
  GetDdr5LrdimmTrainingData (&TrainingData);
  NibbleSwap = &(TrainingData->NibbleSwap);

  for (Index = 0; Index < PHASE_SPOT_CHECK_NUM; Index ++) {
    for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
      if (!((1 << Channel) & ChBitmask)) {
        continue;
      }

      //
      // Program RWE8/9 for Phase Spot check
      //
      DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE8_REG, PhaseSpot[Index]);
      DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE9_REG, PhaseSpot[Index]);
    } // Channel loop

    //
    // Execute test for phase spot check
    //
    ExecuteBacksideExtFineTest (Socket, ChBitmask, Dimm, Rank);

    for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
      if (!((1 << Channel) & ChBitmask)) {
        continue;
      }

      for (SubChannel = 0 ; SubChannel < SUB_CH; SubChannel++) {
        for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
          SwizzleStrobe = CheckBacksideSwizzle (Host, Socket, Channel, Dimm, Strobe);
          LrdimmFeedBack[Channel][SubChannel][Strobe][Index] = ReadDataTrainCounterFeedbackDdr5 (Host, Socket, Channel, SubChannel, SwizzleStrobe);
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, Strobe, NO_BIT,
            "Phase Spot LrdimmFeedBack = %d, Index=%d\n", LrdimmFeedBack[Channel][SubChannel][Strobe][Index], Index);
        } // Strobe loop
      } // SubCh loop
    } // Channel loop
  } // Index loop

  //
  // Adjust External Cycle base upon Phase Spot result
  //
  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }

    RankStruct = GetRankStruct (Host, Socket, Channel, Dimm);
    for (SubChannel = 0 ; SubChannel < SUB_CH; SubChannel++) {
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        StrobePair = Strobe % 5;
        //
        // Retrieve RWE1 from cache
        //
        ControlWordDataLowerNibble = (*RankStruct) [Rank].cachedLrBuf_PG01RWE1[StrobePair + (SubChannel * MaxStrobePairs)] & 0x0F;
        ControlWordDataLowerNibbleDecode = DecodePg01Rwe1 (ControlWordDataLowerNibble);
        ControlWordDataUpperNibble = (*RankStruct) [Rank].cachedLrBuf_PG01RWE1[StrobePair + (SubChannel * MaxStrobePairs)] & 0xF0;
        ControlWordDataUpperNibbleDecode = DecodePg01Rwe1 (ControlWordDataUpperNibble >> 4);

        //
        // External cycle subtract by 1 if Phase 0 has feedback
        //
        if (LrdimmFeedBack[Channel][SubChannel][Strobe][0] >= DWL_WRITE_CYCLE / 2) {
          if ((*NibbleSwap) [Channel][Dimm][SubChannel][Strobe] == TRUE) {
            if (Strobe < 5) {
              ControlWordDataUpperNibbleDecode -= 1;
            } else {
              ControlWordDataLowerNibbleDecode -= 1;
            }
          } else {
            if (Strobe < 5) {
              ControlWordDataLowerNibbleDecode -= 1;
            } else {
              ControlWordDataUpperNibbleDecode -= 1;
            }
          }
          UpdateRwE1 = TRUE;
        }

        //
        // External cycle add 1 if no feedback in each Phase check spots
        //
        if ((LrdimmFeedBack[Channel][SubChannel][Strobe][0] < DWL_WRITE_CYCLE / 2) && (LrdimmFeedBack[Channel][SubChannel][Strobe][1] < DWL_WRITE_CYCLE / 2) &&
          (LrdimmFeedBack[Channel][SubChannel][Strobe][2] < DWL_WRITE_CYCLE / 2)) {
          if ((*NibbleSwap) [Channel][Dimm][SubChannel][Strobe] == TRUE) {
            if (Strobe < 5) {
              ControlWordDataUpperNibbleDecode += 1;
            } else {
              ControlWordDataLowerNibbleDecode += 1;
            }
          } else {
            if (Strobe < 5) {
              ControlWordDataLowerNibbleDecode += 1;
            } else {
              ControlWordDataUpperNibbleDecode += 1;
            }
          }
          UpdateRwE1 = TRUE;
        }

        //
        // Update RWE1 if needed
        //
        if (UpdateRwE1 == TRUE) {
          ControlWordDataLowerNibble = ABS (ControlWordDataLowerNibbleDecode);
          if (ControlWordDataLowerNibbleDecode < 0) {
            ControlWordDataLowerNibble |= BIT3;
          }
          ControlWordDataUpperNibble = ABS (ControlWordDataUpperNibbleDecode);
          if (ControlWordDataUpperNibbleDecode < 0) {
            ControlWordDataUpperNibble |= BIT3;
          }

          ControlWordDataRWE1 = (ControlWordDataUpperNibble << 4) | ControlWordDataLowerNibble;

          DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, StrobePair);
          DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_PG0_RWE1_REG, ControlWordDataRWE1);

          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, Strobe, NO_BIT,
            "Update RWE1 = 0x%x:\n", ControlWordDataRWE1);

          //
          // Update RWE1 cache
          //
          (*RankStruct) [Rank].cachedLrBuf_PG01RWE1[StrobePair + (SubChannel * MaxStrobePairs)] = ControlWordDataRWE1;

          //
          // Clear update flag for next strobe iteration
          UpdateRwE1 = FALSE;

          //
          // Set all buffer device back to RW93
          //
          DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);
        }
      } // Strobe loop
    } // SubChannel loop
  } // Channel loop
} // PhaseSpotsPreCheck

#endif //LRDIMM_SUPPPORT
