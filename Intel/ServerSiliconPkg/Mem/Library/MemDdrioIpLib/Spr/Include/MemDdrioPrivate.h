
/** @file
  MemDdrioPrivate.h

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

#ifndef _DDRIO_PRIVATE_H_
#define _DDRIO_PRIVATE_H_

#include <ReferenceCodeDataTypes.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Memory/MemCmdCtlClkCommon.h>
#include <Library/MemDdrioIpLib.h>
#include "Include/MemCmdControl.h"

#define MAX_CLK_VALID_REG     4  // SPR only has 4 valid clk registers, see details in function GetClkRegOffset

#define DDR_CLK0_ENABLE       1  // DIMM 0 RDIMM, LRDIMM, DDRT2
#define DDR_CLK0_CLK2_ENABLE  5  // DIMM 0 UDIMM

#define MAX_DDR_CC_BITS_PER_SUB_CH   22 // DDR Cmd/Ctrl Bits per sub-channel

//SPR specific TCO range description
#define MAX_TCO_RANGE2     95
#define MIN_TCO_RANGE2     65
#define MAX_TCO_RANGE1     31
#define MIN_TCO_RANGE1     0

#pragma pack(push, 1)

//
// The NVDATA structure for the DDRIO IP Lib
// The data stored here is used to cache registers and settings and to save
// them to NVRAM for use in the fast boot and resume paths.
//

typedef struct {
  UINT32                    DdrintfCtl0Ch;            // DDRINTF_CTL0_CH_0_MCIO_DDRIO_STRUCT
  UINT32                    DdrCcCompLa1;
  UINT32                    DdrCcCompOffset;
  UINT32                    DdrccPirefOffset00;                   // DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG
  UINT32                    DdrccPirefOffset01;                   // DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG
  UINT32                    DdrccPirefOffset02;                   // DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_REG
  UINT32                    DdrccPirefOffset03;                   // DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_REG
  UINT32                    RespASaOffset;                        // DDRCC_CTL1_BIT_[0-21]_MCIO_DDRIOEXT_REG
  UINT32                    RespBSaOffset;                        // DDRCC_CTL1_BIT_[0-21]_MCIO_DDRIOEXT_REG
} DDRIO_SUBCHANNEL_NVDATA;

typedef struct {
  DDRIO_SUBCHANNEL_NVDATA   SubChannel[SUB_CH];
  UINT32                    ClkCsr[MAX_CLK];
  UINT32                    CmdCsr[MAX_DDR_CC_BITS_PER_SUB_CH][SUB_CH];
  UINT32                    DdrdN0PirefOffset0[MAX_STROBE];       // DDRD_N0_PIREF_OFFSET0_MCIO_DDRIO_REG
  UINT32                    DdrdN0PirefOffset1[MAX_STROBE];       // DDRD_N0_PIREF_OFFSET1_MCIO_DDRIO_REG
  UINT32                    DdrclkPirefOffset00;                  // DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG
  UINT32                    DdrclkPirefOffset01;                  // DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG
  UINT8                     RcompCode[MAX_STROBE];
  UINT32                    ddrCRClkControls;
  UINT32                    ddrCRClkTraining;
  UINT32                    rxGroup0[MAX_RANK_CH][MAX_STROBE];
  UINT32                    rxGroup1n[MAX_RANK_CH][MAX_STROBE];
  UINT32                    rxGroup1p[MAX_RANK_CH][MAX_STROBE];
  UINT32                    RxDfeCoeff[MAX_STROBE];
  UINT32                    RxDfeControl[MAX_STROBE];
  UINT32                    rxOffset[MAX_RANK_CH][MAX_STROBE];
  UINT32                    rxVrefCtrl[MAX_STROBE];
  UINT32                    RxSamplerEven[MAX_STROBE];
  UINT32                    RxSamplerOdd[MAX_STROBE];
  UINT32                    txGroup0[MAX_RANK_CH][MAX_STROBE];
  UINT32                    txGroup1[MAX_RANK_CH][MAX_STROBE];
  UINT32                    txGroup2[MAX_RANK_CH][MAX_STROBE];
  UINT32                    TxEqCoefTap2[MAX_RANK_CH][MAX_STROBE];
  UINT16                    XtalkDeltaN0[MAX_RANK_CH][MAX_STROBE];
  UINT16                    XtalkDeltaN1[MAX_RANK_CH][MAX_STROBE];
  UINT32                    txTco[MAX_STROBE / 2];
  UINT8                     txVrefCache[MAX_RANK_CH][MAX_STROBE]; ///< Cached value of txVref (this might not be the programmed value)
  UINT8                     txVref[MAX_RANK_CH][MAX_STROBE];      ///< Actual current value of txVref
  UINT32                    DdrDataTxCtl0[MAX_STROBE];
  UINT32                    DdrDataRxCtl0[MAX_STROBE];
  UINT32                    DdrdTrainOffset0;
  UINT32                    DdrCcTxCtl0[SUB_CH];
  UINT32                    DdrClkTxCtl0;
  UINT32                    DcaTco[SUB_CH];
  UINT32                    DdrdN0TrainOffset0[MAX_STROBE];
  UINT32                    DdrdN0TrainCtl0[MAX_STROBE];          // DDRD_N0_TRAIN_CTL0_MCIO_DDRIO_REG
  UINT32                    DdrdN0TrainCtl1[MAX_STROBE];          // DDRD_N0_TRAIN_CTL1_MC_DDRIOMC_REG
  UINT32                    DdrdN0TrainCtl2[MAX_STROBE];          // DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG
  UINT32                    DdrdN0TrainCtl3[MAX_STROBE];          // DDRD_N0_TRAIN_CTL3_MCIO_DDRIO_REG
  UINT32                    DdrdN0PxcCtl0[MAX_STROBE];            // DDRD_N0_PXC_CTL0_MCIO_DDRIO_REG
  UINT32                    DdrDataCompOffset[MAX_STROBE];
  UINT32                    DdrDataCompLa1[MAX_STROBE];
  UINT32                    DfeCoeffOffset0[MAX_RANK_CH][MAX_STROBE];
  UINT32                    DfeCoeffOffset1[MAX_RANK_CH][MAX_STROBE];
  UINT32                    DfeCoeff[MAX_RANK_CH][MAX_STROBE];
#if 0 // SPR_TODO BIOS HSD-ES 1504716216
  UINT32                    DdrClkRxVrefCtl;
#endif // SPR_TODO BIOS HSD-ES 1504716216
} DDRIO_CHANNEL_NVDATA;

typedef struct {
  UINT32                    ddrCRCompCtl0;
  UINT32                    dimmVrefControl1;

} DDRIO_IMC_NVDATA;

typedef struct {
  DDRIO_CHANNEL_NVDATA      ChannelList[MAX_CH];
  DDRIO_IMC_NVDATA          Imc[MAX_IMC];
} DDRIO_SOCKET_NVDATA;

//
// Structure specifically for DDRIO data that will be saved to NVRAM in DXE phase
// and restored from NVRAM at the start of MRC, if available.
// This data is used to cache registers for faster MRC execution and also to be
// used during fast boot and S3 resume paths to skip training.
//
typedef struct {
  DDRIO_SOCKET_NVDATA   Socket[MAX_SOCKET];
} DDRIO_NVDATA;

//
// Sense Amp Calibration Training - temporary settings storage
//
typedef struct {
  UINT32                                        DataOdtStaticLegCfgStore [MAX_CH][MAX_STROBE_DDR5];
  UINT32                                        CcOdtStaticLegCfgStore [MAX_CH][SUB_CH];
  DDRCOMP_CMD_COMP0_MCIO_DDRIOEXT_STRUCT        DdrcompCmdComp0Store[MAX_CH][SUB_CH];
  DDRCOMP_DATA_COMP0_MCIO_DDRIOEXT_STRUCT       DdrcompDataComp0Store[MAX_CH][MAX_STROBE_DDR5];
  UINT32                                        DdrccTrainCtl1Store[MAX_CH][SUB_CH];
} SENSE_AMP_SETTING_STORE;

#define CC_SENSE_AMP_SIGNAL_LIST        {RSPA_SA, RSPB_SA, RSPA_SB, RSPB_SB}
#define CC_SENSE_AMP_SIGNAL_LIST_SIZE   4
//
// Sense Amp Calibration Training Results
//
typedef struct {
  UINT8   OddSampler[MAX_CH][MAX_STROBE_DDR5][BITS_PER_NIBBLE];
  UINT8   EvenSampler[MAX_CH][MAX_STROBE_DDR5][BITS_PER_NIBBLE];
  UINT8   Summer[MAX_CH][MAX_STROBE_DDR5][BITS_PER_NIBBLE];
  UINT8   CcSaOffset[MAX_CH][CC_SENSE_AMP_SIGNAL_LIST_SIZE];
} SENSE_AMP_HWFSM_RESULTS;

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
  );

/**

  Issue a DDRIO COMP cycle

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket number

  @retval MRC_STATUS_SUCCESS   COMP cycle successfully completed
  @retval !MRC_STATUS_SUCCESS  COMP cycle was not successful

**/
MRC_STATUS
EFIAPI
DoCompDdrio (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  );

/**
  Obtain pointer to DDRIO NVRAM data channel list

  @param[in]  Host    - Pointer to SysHost
  @param[in]  Socket  - Socket number

  @retval Pointer to DdrioNvData.Socket[Socket].ChannelList
**/
DDRIO_CHANNEL_NVDATA
(*GetDdrioChannelNvList (
  PSYSHOST  Host,
  UINT8     Socket
  ))[MAX_CH];

/**
  Obtain pointer to DDRIO NVRAM data memory controller list

  @param[in]  Host    - Pointer to SysHost
  @param[in]  Socket  - Socket number

  @retval Pointer to DdrioNvData.Socket[Socket].Imc
**/
DDRIO_IMC_NVDATA
(*GetDdrioImcNvList (
  PSYSHOST  Host,
  UINT8     Socket
  ))[MAX_IMC];

/**

  Poll the HW FSM Complete.

  @param[in]  Host                  - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket                - Current CPU socket
  @param[in]  ChannelMask           - Mask of channels to poll for completion
  @param[in]  MaxEstimatedFsmTime   - Polling time out value

  @retval EFI_SUCCESS   Successfully detected completion
  @retval EFI_TIMEOUT   Timed out while polling for completion

**/
EFI_STATUS
PollHwFsmComplete (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT32   ChannelMask,
  IN  UINT64   MaxEstimatedFsmTime
  );

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
  );

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
  );

/**
  Program the DDRINTF_CTL0 register for the channel/subchannel
  Update the cache.

  @param[in]  Host          - Pointer to SYSHOST
  @param[in]  Socket        - Socket number
  @param[in]  Ch            - Channel number - socket based
  @param[in]  SubCh         - Sub channel number
  @param[out] DdrintfCtl0   - Register value

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
  );

/**

  Get Set Tx DQ Scomp.

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
  @param[in][out] Value   - Data to program

  @retval IO delay

**/
MRC_STATUS
GetSetTxDqScomp (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Ch,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN UINT8        Strobe,
  IN UINT8        Bit,
  IN MRC_LT       Level,
  IN MRC_GT       Group,
  IN UINT8        Mode,
  IN OUT INT16    *Value
  );

/**
  Gets the CSR address for a DDRCC_CTL1 index

  @param[in] Host       - Pointer to SYSHOST
  @param[in] DdrccBit   - DDRCC_CTRL0_BIT register index

  @retval CSR address for given register index if the index is valid
  @retval 0xFFFFFFFF if register not found

**/
UINT32
GetDdrCcCtl1RegisterAddress (
  IN  PSYSHOST Host,
  IN  UINT8    DdrccBit
  );

#pragma pack(pop)

#endif //_DDRIO_PRIVATE_H_
