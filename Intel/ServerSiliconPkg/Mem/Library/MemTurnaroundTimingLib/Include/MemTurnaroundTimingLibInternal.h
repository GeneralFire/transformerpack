/** @file
  MemPor.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019-2020 Intel Corporation. <BR>

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

#ifndef  __MEM_TURNAROUND_TIMING_LIB_INTERNAL_H__
#define  __MEM_TURNAROUND_TIMING_LIB_INTERNAL_H__

// enforce due to PXC (HSD 5370733)
#define ENFORCE_WWDR_PXC_EN 3
#define ENFORCE_WWDR_PXC_DIS 2

#define ENFORCE_WWDD_PXC_EN 3
#define ENFORCE_WWDD_PXC_DIS 2

#define ADJUSTED_BURST_LENGTH 6 // (Burst length / QCLK_PER_DCLK) + preamble + postamble (without WR_CRC)

#define DDRT_T_WRWR_S_B0_SI_WA_VALUE  6
#define DDRT_T_WRWR_S_DEFAULT_VALUE   4

//
// ODT Stretch selectors
//
typedef enum {
  RD_RD = 0,
  RD_WR = 1,
  WR_RD = 2,
  WR_WR = 3,
  } MRC_ODTSTRETCH_SELECTION;

/**

  Initializes a turn around table by setting all values to 0 and setting the timing
  type to the appropriate value.

  @param[in] TurnaroundTable  - Table to be initialized
  @param[in] ArraySize        - Size of array to initialize

  @retval none

**/
VOID
InitTurnaroundTable (
  IN UINT32  TurnaroundTable[],
  IN UINT32  ArraySize
  );

/**

  Display turnaround times

  @param[in] MemTechType   - Memory Technology Type
  @param[in] Socket        - Socket number
  @param[in] Ch            - Channel number

  @retval None

**/
VOID
DisplayTurnaroundTimes (
  IN     MEM_TECH_TYPE  MemTechType,
  IN     UINT8          Socket,
  IN     UINT8          Ch
  );

/**

  @param[in] MemTechType   - Memory Technology Type
  @param[in] Socket        - Socket number
  @param[in] Channel       - Channel number

  @retval None

**/
VOID
DisplayTurnaroundTimesDcpmm (
  IN     MEM_TECH_TYPE  MemTechType,
  IN     UINT8          Socket,
  IN     UINT8          Channel
  );

/**

  Returns a pointer to a table of relaxed turnaorund constants

  @param[in]  Socket             - Socket number
  @param[out] TurnaroundTable    - Pointer to caller's table pointer to be
                                   populated by this function.

  @retval  EFI_SUCCESS

**/
EFI_STATUS
GetRelaxedTurnaroundConstants (
  IN UINT8                        Socket,
  OUT CONST TURNAROUND_CONSTANTS  **TurnaroundConstants
  );

/**

  Returns a pointer to a table of relaxed DCPMM turnaorund constants

  @param[out] TurnaroundTable    - Pointer to caller's table pointer to be
                                   populated by this function.

  @retval  EFI_SUCCESS

**/
EFI_STATUS
GetDcpmmRelaxedTurnaroundConstants (
  OUT CONST DCPMM_TURNAROUND_CONSTANTS  **DcpmmTurnaroundConstants
  );

/**

  Find and return a pointer to the DCPMM turnaround contants structure.

  @param[out] DcpmmTurnaroundConstants  - On return, points to a pointer to the turnaround constants

  @retval EFI_SUCCESS if table is found
          !EFI_SUCCESS otherwise

**/
EFI_STATUS
GetDcpmmTurnaroundConstants (
  OUT CONST DCPMM_TURNAROUND_CONSTANTS  **DcpmmTurnaroundConstants
  );

/**

  Displays the read/write to grant turnaround times for this SOC.

  @param[in]  DcpmmTurnaroundSettings  - Table which contains DCPMM turnaround settings

  @retval none

**/
VOID
DisplayRWtoGNT (
  IN UINT32  DcpmmTurnaroundTimes[]
  );

/**

  Program early turnaround times in accordance with Turnaround doc for this SOC.
  Display at the end of training for final results.

  @param[in] Socket              - Socket number
  @param[in] Channel             - Channel number
  @param[in] TurnaroundTable     -  Pointer to table containing constants provided by EV
  @param[in] TurnaroundSettings  - Pointer to table containing settings to be programmed

  @retval none

**/
VOID
RelaxTurnaroundTimesSoc (
  IN UINT8                           Socket,
  IN UINT8                           Channel,
  IN CONST TURNAROUND_CONSTANTS      *TurnaroundTable,
  IN UINT32                          TurnaroundSettings[]
  );

/**

  Calculate and program optimal TA times based on formulas and training results

  @param[in] Socket              - Socket number
  @param[in] Ch                  - Channel number
  @param[in] TurnaroundSettings  - Pointer to table containing settings to be programmed

  @retval none

**/
VOID
OptimizeTurnaroundTimingsSoc (
  IN UINT8                           Socket,
  IN UINT8                           Ch,
  IN UINT32                          TurnaroundSettings[]
  );

/**

  This function calculates the read to read turnaround time for same rank on the same DIMM
  for this SOC

  @param[in] Host            - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number

  @retval Trrdr channel value

**/
UINT8
CalculateRRSRSoc (
  IN  PSYSHOST         Host,                     // Pointer to sysHost, the system Host (root) structure
  IN  UINT8            Socket,                   // CPU Socket Node number (Socket ID)
  IN  UINT8            Channel                   // Channel number (0-based)
  );

/**

  This function calculates the read to read turnaround time for same group
  for this SOC

  @param[in] Host            - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number

  @retval Trrsg channel value

**/
UINT8
CalculateRRSGSoc (
  IN  PSYSHOST         Host,                     // Pointer to sysHost, the system Host (root) structure
  IN  UINT8            Socket,                   // CPU Socket Node number (Socket ID)
  IN  UINT8            Channel                   // Channel number (0-based)
  );

/**

  This function calculates the write to write turnaround time for same rank on the same DIMM
  for this SOC

  @param[in] Host            - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number

  @retval Twwsr channel value

**/
UINT8
CalculateWWSRSoc (
  IN  PSYSHOST         Host,                     // Pointer to sysHost, the system Host (root) structure
  IN  UINT8            Socket,                   // CPU Socket Node number (Socket ID)
  IN  UINT8            Channel                   // Channel number (0-based)
  );

/**

  This function calculates the write to write turnaround time for same group
  for this SOC

  @param[in] Host            - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number

  @retval Twwsg channel value

**/
UINT8
CalculateWWSGSoc (
  IN  PSYSHOST         Host,                     // Pointer to sysHost, the system Host (root) structure
  IN  UINT8            Socket,                   // CPU Socket Node number (Socket ID)
  IN  UINT8            Channel                   // Channel number (0-based)
  );

/**

  This function calculates the write to read turnaround time for same group
  for this SOC

  @param[in] Host            - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number

  @retval Twrsg channel value

**/
UINT8
CalculateWRSGSoc (
  IN  PSYSHOST         Host,                     // Pointer to sysHost, the system Host (root) structure
  IN  UINT8            Socket,                   // CPU Socket Node number (Socket ID)
  IN  UINT8            Channel                   // Channel number (0-based)
  );

/**

  This function calculates the read to read turnaround time for different ranks on the same DIMM
  for this SOC

  @param[in] Host            - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number
  @param[in] BusTimingData   - Structure that holds turnaround data variables

  @retval Trrdr channel value

**/
UINT8
CalculateRRDRSoc (
  IN  PSYSHOST         Host,                     // Pointer to sysHost, the system Host (root) structure
  IN  UINT8            Socket,                   // CPU Socket Node number (Socket ID)
  IN  UINT8            Channel,                  // Channel number (0-based)
  IN  BUS_TIMING_DATA  *BusTimingData            // Pointer to the structure for timing variables
  );

/**

  This function calculates the read to read turnaround time for different ranks on the same DIMM

  @param[in] Host            - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number
  @param[in] BusTimingData   - Structure that holds turnaround data variables

  @retval Trrdr channel value

**/
UINT8
CalculateRRDR (
  IN  PSYSHOST         Host,                     // Pointer to sysHost, the system Host (root) structure
  IN  UINT8            Socket,                   // CPU Socket Node number (Socket ID)
  IN  UINT8            Channel,                  // Channel number (0-based)
  IN  BUS_TIMING_DATA  *BusTimingData            // Pointer to the structure for timing variables
  );

/**

  This function calculates the read to read turnaround time for different ranks on the same DIMM (10nm version)

  @param[in] Host            - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number
  @param[in] BusTimingData   - Structure that holds turnaround data variables

  @retval Trrdr channel value

**/
UINT8
CalculateRRDR10nm (
  IN  PSYSHOST         Host,                     // Pointer to sysHost, the system Host (root) structure
  IN  UINT8            Socket,                   // CPU Socket Node number (Socket ID)
  IN  UINT8            Channel,                  // Channel number (0-based)
  IN  BUS_TIMING_DATA  *BusTimingData            // Pointer to the structure for timing variables
  );

/**

  This function calculates the read to read turnaround time for different DIMMs for this SOC

  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number
  @param[in] BusTimingData   - Structure that holds turnaround data variables

  @retval Trrdd channel value

**/
UINT8
CalculateRRDDSoc (
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the read to read turnaround time for different DIMMs

  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number
  @param[in] BusTimingData   - Structure that holds turnaround data variables

  @retval Trrdd channel value

**/
UINT8
CalculateRRDD (
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the read to read turnaround time for different DIMMs (10nm version)

  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number
  @param[in] BusTimingData   - Structure that holds turnaround data variables

  @retval Trrdd channel value

**/
UINT8
CalculateRRDD10nm (
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the write to write turnaround time for different ranks on the same DIMM
  for this SOC

  @param[in]  Host            - Point to sysHost
  @param[in]  Socket          - Socket number
  @param[in]  Channel         - Channel number
  @param[in]  BusTimingData   - Structure that holds turnaround data variables

  @retval tWWDR

**/
UINT8
CalculateWWDRSoc (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the write to write turnaround time for different ranks on the same DIMM

  @param[in]  Host,           - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket          - CPU Socket Node number (Socket ID)
  @param[in]  Channel         - Channel number (0-based)
  @param[in]  BusTimingData   - Pointer to the structure for timing variables
  @param[out] Twwdr           - Pointer to the return value for Twwdr

  @retval None

**/
VOID
CalculateWWDR (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT32           *Twwdr
  );

/**

  This function calculates the write to write turnaround time for different ranks on the same DIMM (10nm version)

  @param[in]  Host,           - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket          - CPU Socket Node number (Socket ID)
  @param[in]  Channel         - Channel number (0-based)
  @param[in]  BusTimingData   - Pointer to the structure for timing variables
  @param[out] Twwdr           - Pointer to the return value for Twwdr

  @retval None

**/
VOID
CalculateWWDR10nm (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT32           *Twwdr
  );

/**

  This function calculates the write to write turnaround time for different DIMMs
  for this SOC

  @param[in]  Host            - Point to sysHost
  @param[in]  Socket          - Socket number
  @param[in]  Channel         - Channel number
  @param[in]  BusTimingData   - Structure that holds turnaround data variables

  @retval tWWDD

**/
UINT8
CalculateWWDDSoc (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the write to write turnaround time for different DIMMs

  @param[in]  Host            - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket          - CPU Socket Node number (Socket ID)
  @param[in]  Channel         - Channel number (0-based)
  @param[in]  BusTimingData   - Pointer to the structure for timing variables
  @param[out] Twwdd           - Pointer to the return value for Twwdd

  @retval None

**/
VOID
CalculateWWDD (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT32           *Twwdd
  );

/**

  This function calculates the write to write turnaround time for different DIMMs (10nm version)

  @param[in]  Host            - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket          - CPU Socket Node number (Socket ID)
  @param[in]  Channel         - Channel number (0-based)
  @param[in]  BusTimingData   - Pointer to the structure for timing variables
  @param[out] Twwdd           - Pointer to the return value for Twwdd

  @retval None

**/
VOID
CalculateWWDD10nm (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT32           *Twwdd
  );

/**

  This function calculates the read to write turnaround time for the same rank for this SOC

  @param[in]  Socket          - Socket number
  @param[in]  Channel         - Channel number
  @param[in]  BusTimingData   - Structure that holds turnaround data variables

  @retval rRWSR

**/
UINT8
CalculateRWSRSoc (
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the read to write turnaround time for same rank

  @param[in]  Host            - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket          - CPU Socket Node number (Socket ID)
  @param[in]  Channel         - Channel number (0-based)
  @param[in]  *BusTimingData  - Pointer to the structure for timing variables
  @param[out] *Trwsr          - Pointer to the return value for Trwsr

  @retval None

**/
VOID
CalculateRWSR (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT32           *Trwsr
  );

/**

  This function calculates the read to write turnaround time for same rank (10nm version)

  @param[in]  Host            - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket          - CPU Socket Node number (Socket ID)
  @param[in]  Channel         - Channel number (0-based)
  @param[in]  *BusTimingData  - Pointer to the structure for timing variables
  @param[out] *Trwsr          - Pointer to the return value for Trwsr

  @retval None

**/
VOID
CalculateRWSR10nm (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT32           *Trwsr
  );

/**

  This function calculates the read to write turnaround time for different ranks on the same DIMM
  for this SOC

  @param[in]  Host            - Point to sysHost
  @param[in]  Socket          - Socket number
  @param[in]  Channel         - Channel number
  @param[in]  BusTimingData   - Structure that holds turnaround data variables

  @retval tRWDR

**/
UINT8
CalculateRWDRSoc (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the read to write turnaround time for different ranks on the same DIMM

  @param[in]  Host            - Pointer to sysHost, the system Host(root)structure
  @param[in]  Socket          - CPU Socket Node number (Socket ID)
  @param[in]  Channel         - Channel number (0-based)
  @param[in]  *BusTimingData  - Pointer to the structure for timing variables
  @param[out] *Trwdr          - Pointer to the return value for Trwdr

  @retval None

**/
VOID
CalculateRWDR (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT32           *Trwdr
  );

/**

  This function calculates the read to write turnaround time for different ranks on the same DIMM (10nm version)

  @param[in]  Host            - Pointer to sysHost, the system Host(root)structure
  @param[in]  Socket          - CPU Socket Node number (Socket ID)
  @param[in]  Channel         - Channel number (0-based)
  @param[in]  *BusTimingData  - Pointer to the structure for timing variables
  @param[out] *Trwdr          - Pointer to the return value for Trwdr

  @retval None

**/
VOID
CalculateRWDR10nm (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT32           *Trwdr
  );

/**

  This function calculates the write to read turnaround time for the same ranks on the same DIMM
  for this SOC.

  @param[in]  Host     - Point to sysHost
  @param[in]  Socket   - Socket number
  @param[in]  Channel  - Channel number

  @retval tWRSR

**/
UINT8
CalculateWRSRSoc (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel
  );

/**

  This function calculates the read to write turnaround time for different DIMMs
  for this SOC

  @param[in]  Socket          - Socket number
  @param[in]  Channel         - Channel number
  @param[in]  BusTimingData   - Structure that holds turnaround data variables

  @retval tRWDD

**/
UINT8
CalculateRWDDSoc (
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the read to write turnaround time for different ranks on the different DIMM

  @param[in]  Host            - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket          - CPU Socket Node number (Socket ID)
  @param[in]  Channel         - Channel number (0-based)
  @param[in]  *BusTimingData  - Pointer to the structure for timing variables
  @param[out] *Trwdd          - Pointer to the return value for tRWDD

  @retval None

**/
VOID
CalculateRWDD (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT32           *tRWDD
  );

/**

  This function calculates the read to write turnaround time for different ranks on the different DIMM (10nm)

  @param[in]  Host            - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket          - CPU Socket Node number (Socket ID)
  @param[in]  Channel         - Channel number (0-based)
  @param[in]  *BusTimingData  - Pointer to the structure for timing variables
  @param[out] *Trwdd          - Pointer to the return value for tRWDD

  @retval None

**/
VOID
CalculateRWDD10nm (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT32           *tRWDD
  );

/**

  This function calculates the write to read turnaround time for different ranks on the same DIMM
  for this SOC

  @param[in]  Host            - Point to sysHost
  @param[in]  Socket          - Socket number
  @param[in]  Channel         - Channel number
  @param[in]  BusTimingData   - Structure that holds turnaround data variables

  @retval tWRDR

**/
UINT8
CalculateWRDRSoc (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the write to read turnaround time for different ranks on the same DIMM

  @param[in]  Host            - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket          - CPU Socket Node number (Socket ID)
  @param[in]  Channel         - Channel number (0-based)
  @param[in]  BusTimingData   - Pointer to the structure for timing variables
  @param[out] Twrdr           - Pointer to the return value for Twrdr

  @retval None
**/
VOID
CalculateWRDR (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT32           *Twrdr
  );

/**

  This function calculates the write to read turnaround time for different ranks on the same DIMM (10nm version)

  @param[in]  Host            - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket          - CPU Socket Node number (Socket ID)
  @param[in]  Channel         - Channel number (0-based)
  @param[in]  BusTimingData   - Pointer to the structure for timing variables
  @param[out] Twrdr           - Pointer to the return value for Twrdr

  @retval None
**/
VOID
CalculateWRDR10nm (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT32           *Twrdr
  );

/**

  This function calculates the write to read turnaround time for different DIMMs for this SOC

  @param[in]  Socket          - Socket number
  @param[in]  Channel         - Channel number
  @param[in]  BusTimingData   - Structure that holds turnaround data variables

  @retval tWRDD

**/
UINT8
CalculateWRDDSoc (
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the write to read turnaround time for different DIMMs

  @param[in]  Host             - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket           - CPU Socket Node number (Socket ID)
  @param[in]  Channel          - Channel number (0-based)
  @param[in]  *BusTimingData   - Pointer to the structure for timing variables
  @param[out] *Twrdd           - Pointer to the return value for Twrdd

  @retval None

**/
VOID
CalculateWRDD (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT32           *Twrdd
  );

/**

  This function calculates the write to read turnaround time for different DIMMs (10nm version)

  @param[in]  Host             - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket           - CPU Socket Node number (Socket ID)
  @param[in]  Channel          - Channel number (0-based)
  @param[in]  *BusTimingData   - Pointer to the structure for timing variables
  @param[out] *Twrdd           - Pointer to the return value for Twrdd

  @retval None

**/
VOID
CalculateWRDD10nm (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT8            *Twrdd
  );

/**

  This function calculates the DDR4 read to DDRT write turnaround time for different DIMMs

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] DDRTCWL        - DDRT CAS Write Latency
  @param[in] BusTimingData  - Structure that holds the variables

  @retval DDRTRDWR channel value

**/
UINT8
CalctDdr4toDdrtRWDD (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN UINT8            DDRTCWL,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the DDR4 read to DDRT write turnaround time for different DIMMs (10nm version)

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] DDRTCWL        - DDRT CAS Write Latency
  @param[in] BusTimingData  - Structure that holds the variables

  @retval DDRTRDWR channel value

**/
UINT8
CalctDdr4toDdrtRWDD10nm (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN UINT8            DDRTCWL,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the DDR4 write to DDRT read turnaround time for different DIMMs

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] DDRTCL         - DDRT CAS Latency
  @param[in] BusTimingData  - Structure that holds the variables

  @retval DDRTWRRD channel value

**/
UINT8
CalctDdr4toDdrtWRDD (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN UINT8            DDRTCL,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the DDR4 write to DDRT read turnaround time for different DIMMs (10nm version)

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] DDRTCL         - DDRT CAS Latency
  @param[in] BusTimingData  - Structure that holds the variables

  @retval DDRTWRRD channel value

**/
UINT8
CalctDdr4toDdrtWRDD10nm (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN UINT8            DDRTCL,
  IN BUS_TIMING_DATA  *BusTimingData
  );


/**

  DDR4-TO-DDRT
  This function calculates the read to read turnaround time for different DIMMs

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Ch             - Channel number
  @param[in] BusTimingData  - Structure that holds the variables

  @retval t_DDRT_GNTGNT_DD channel value

**/
UINT16
CalctDdr4toDdrtRRDD (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Ch,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  DDR4-TO-DDRT
  This function calculates the read to read turnaround time for different DIMMs (10nm version)

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Ch             - Channel number
  @param[in] BusTimingData  - Structure that holds the variables

  @retval t_DDRT_GNTGNT_DD channel value

**/
UINT16
CalctDdr4toDdrtRRDD10nm (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Ch,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  DDRT-TO-DDR4
  This function calculates the read to read turnaround time for different DIMMs

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Ch             - Channel number
  @param[in] BusTimingData  - Structure that holds the variables

  @retval DDRTRD_to_DDR4RD channel value

**/
UINT8
CalctDdrttoDdr4RRDD (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Ch,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  DDRT-TO-DDR4
  This function calculates the read to read turnaround time for different DIMMs (10nm version)

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Ch             - Channel number
  @param[in] BusTimingData  - Structure that holds the variables

  @retval DDRTRD_to_DDR4RD channel value

**/
UINT8
CalctDdrttoDdr4RRDD10nm (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Ch,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the DDRT GNT to DDR4 write turnaround time for different DIMMs

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] DDRTCL         - DDRT CAS Latency
  @param[in] BusTimingData  - Structure that holds the variables

  @retval DDRTRDWR channel value

**/
UINT8
CalctDdrttoDdr4RWDD (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN UINT8            DDRTCL,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the DDRT GNT to DDR4 write turnaround time for different DIMMs (10nm version)

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] DDRTCL         - DDRT CAS Latency
  @param[in] BusTimingData  - Structure that holds the variables

  @retval DDRTRDWR channel value

**/
UINT8
CalctDdrttoDdr4RWDD10nm (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN UINT8            DDRTCL,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the DDRT write to DDR4 read turnaround time for different DIMMs

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] DDRTCWL        - DDRT CAS Write Latency
  @param[in] BusTimingData  - Structure that holds the variables

  @retval DDRTWRRD channel value

**/
UINT8
CalctDdrttoDdr4WRDD (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN UINT8            DDRTCWL,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the DDRT write to DDR4 read turnaround time for different DIMMs (10nm version)

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] DDRTCWL        - DDRT CAS Write Latency
  @param[in] BusTimingData  - Structure that holds the variables

  @retval DDRTWRRD channel value

**/
UINT8
CalctDdrttoDdr4WRDD10nm (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN UINT8            DDRTCWL,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the DDRT read to DDRT write turnaround time for same DIMM

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] DDRTCL         - DDRT CAS Latency
  @param[in] DDRTCWL        - DDRT CAS Write Latency
  @param[in] BusTimingData  - Structure that holds the variables

  @retval DdrtRWSR channel value

**/
UINT8
CalctDdrttoDdrtRWSR (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN UINT8            DDRTCL,
  IN UINT8            DDRTCWL,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the DDRT read to DDRT write turnaround time for same DIMM (10nm version)

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] DDRTCL         - DDRT CAS Latency
  @param[in] DDRTCWL        - DDRT CAS Write Latency
  @param[in] BusTimingData  - Structure that holds the variables

  @retval DdrtRWSR channel value

**/
UINT8
CalctDdrttoDdrtRWSR10nm (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN UINT8            DDRTCL,
  IN UINT8            DDRTCWL,
  IN BUS_TIMING_DATA  *BusTimingData
  );

/**

  This function calculates the DDRT write to DDRT read turnaround time for same rank (10nm version)

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] DdrtCl         - DDRT CAS Latency
  @param[in] DdrtCwl        - DDRT CAS Write Latency

  @retval DdrtWRSR channel value

**/
UINT8
CalctDdrttoDdrtWRSR10nm (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN UINT8            DdrtCl,
  IN UINT8            DdrtCwl
  );

/**

  If a 3DS DIMM is present, this functions accounts for it by adjusting the necessary turnaround times

  @param[in]  Host                - Pointer to syshost
  @param[in]  Socket              - Socket number
  @param[in]  Channel             - Channel number
  @param[out] TurnaroundSettings  - Pointer to table containing settings to be programmed

  @retval none

**/
VOID
AdjustTimingsFor3DS (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT32    TurnaroundSettings[]
  );

/**

  Enforces WWDD settings

  @param[in]  Host     - Pointer to sysHost, the system Host (root) structure
  @param[in]  Channel  - Channel number (0-based)
  @param[out] tWWDD    - Pointer to the resulting enforcement decision

  @retval None

**/
VOID
EnforceWWDD (
  IN  PSYSHOST  Host,
  IN  UINT8     Channel,
  OUT UINT32    *tWWDD
  );

/**

  Enforces WWDR settings

  @param[in]  Host  - Pointer to sysHost, the system Host (root) structure
  @param[in]  Ch    - Channel number (0-based)
  @param[out] tWWDR - Pointer to the resulting enforcement decision

  @retval None

**/
VOID
EnforceWWDR (
  IN  PSYSHOST  Host,
  IN  UINT8     Ch,
  OUT UINT32    *tWWDR
  );

/**

  Calculate the grant-to-completion delay

  @param[in] Host      - Pointer to the SysHost structure
  @param[in] Socket    - Socket to calculate the delay for
  @param[in] Channel   - Channel to calculate the delay for
  @param[in] Gnt2Erid  - Grant-to-early read ID delay (in Dclks)

  @retval The grant-to-completion delay (in Dclks)

**/
UINT8
CalculateGnt2Cmpl (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Gnt2Erid
  );

/**

  Calculate the maximum round trip latency (in Dclks).

  @param[in] Host           - Pointer to the SysHost structure
  @param[in] Socket         - Socket to calculate the delay for
  @param[in] Channel        - Channel to calculate the delay for
  @param[in] BusTimingData  - Structure that holds the turnaround timing variables

  @retval The maximum round trip latency (in Dclks).

**/
UINT8
CalculateMaxRoundTrip (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData
  );

/**

  Get turnaround times from Perf table and properly caculated out

  @param Socket         - Socket number
  @param Channel        - Channel number
  @param PerfTableRegs  - Perf table register matrix, will be updated after this function.

  @retval EFI_SUCCESS if turnaround times are available
          !EFI_SUCCESS otherwise

**/
EFI_STATUS
GetPerfTableTaValuesPerCh (
  IN UINT8       Socket,
  IN UINT8       Channel,
  IN OUT UINT32  *PerfTableRegs
  );

/**

  Returns a Minimum turnaround setting for desired DDR4 turnaround timing

  @param Socket                    - Socket number
  @param Channel                   - Channel number
  @param[in]  Selection            - Desired turnaround timing
  @param[out] MinTurnaroundSetting - Minimum turnaround setting for desired turnaround timing

  @retval  EFI_SUCCESS   - Found it
           EFI_NOT_FOUND - Not found

**/
EFI_STATUS
GetDdr4TurnaroundTimingMinLimits (
  IN  UINT8                             Socket,
  IN  UINT8                             Channel,
  IN  TURNAROUND_TYPE                   Selection,
  OUT  UINT32                           *MinTurnaroundSetting
  );

/**

  Returns a Minimum turnaround setting for desired DDRT turnaround timing

  @param Socket                    - Socket number
  @param Channel                   - Channel number
  @param[in]  Selection            - Desired turnaround timing
  @param[out] MinTurnaroundSetting - Minimum turnaround setting for desired turnaround timing

  @retval  EFI_SUCCESS   - Found it
           EFI_NOT_FOUND - Not found

**/
EFI_STATUS
GetDdrtTurnaroundTimingMinLimits (
  IN  UINT8                             Socket,
  IN  UINT8                             Channel,
  IN  DCPMM_TURNAROUND_TYPE             Selection,
  OUT  UINT32                           *MinTurnaroundSetting
  );

/**

  Returns a ODT stretch

  @param[in]  Socket           - Socket Number
  @param[in]  Channel          - Channel Number
  @param[in]  *BusTimingData   - Structure that holds turnaround data variables
  @param[out] Direction        - Direction for "rr", "rw", "wr", and "ww"

  @retval  OdtStretch

**/
UINT8
CalculateOdtStretch (
  IN  UINT8                     Socket,
  IN  UINT8                     Channel,
  IN  BUS_TIMING_DATA           *BusTimingData,
  IN  MRC_ODTSTRETCH_SELECTION  Direction
  );

#endif // _MEM_TURNAROUND_TIMING_LIB_INTERNAL_H_
