/** @file
  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _EV_CONTENTCHIP_H_
#define _EV_CONTENTCHIP_H_

#include <ReferenceCodeDataTypes.h>
#include "ssabios.h"
#include <Library/MemoryEvContentLib.h>
#include "CpgcPointTest.h"
#include "ChkRetStat.h"
#include "MemPointTest.h"
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/SsaUtilLib.h>
#include <Library/BitMaskLib.h>
#include <Library/Ddr5CoreLib.h>
#include <Memory/MemoryCheckpointCodes.h>


/**
  @brief
  This function udpates the CPGC point test configuration based on the Margin1D test configuration

  @param[in ]  pMargin1DTestCfg   Pointer to Margin1D test configuration structure.
  @param[out]  pCpgcPointTestCfg  Pointer to CPGC point test configuration structure.

  @retval status - SUCCESS/FAILURE
**/
UINT32
UpdateCpgcPointTestCfgByMargin1DCfg (
  IN CONST MARGIN_1D_CONFIG  *pTestCfg,
  OUT CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg
  );

/**
  @brief
  This function udpates the CPGC point test configuration based on the Margin2D test configuration

  @param[in ]  pMargin2DTestCfg   Pointer to Margin2D test configuration structure.
  @param[out]  pCpgcPointTestCfg  Pointer to CPGC point test configuration structure.

  @retval status - SUCCESS/FAILURE
  **/
UINT32
UpdateCpgcPointTestCfgByMargin2DCfg (
  IN CONST MARGIN_2D_CONFIG  *pTestCfg,
  OUT CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg
  );

/**
  @brief
  This function updates the CPGC point test configuration based on the RunCpgcPointTest test configuration

  @param[in ]  pRunCpgcPointTestCfg   Pointer to RunCpgcPointTest test configuration structure.
  @param[out]  pCpgcPointTestCfg  Pointer to CPGC point test configuration structure.

  @retval status - SUCCESS/FAILURE
**/
UINT32
UpdateCpgcPointTestCfgByRunCpgcPoinTestCfg (
  IN CONST RUN_CPGC_POINT_TEST_CONFIG *pTestCfg,
  OUT CPGC_POINT_TEST_CONFIG          *pCpgcPointTestCfg
  );

/**
  @brief
  This function logs the Margin1D test configuration.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to core test data structure.
**/
VOID
LogMargin1DTestCfg (
  IN SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN MARGIN_1D_CONFIG *pTestCfg
  );

/**
  @brief
  This function logs the Margin2D test configuration.

  @param[in]  SsaServicesHandle  Pointer to SSA services.
  @param[in]  pTestCfg           Pointer to Margin2D test configuration structure.
**/
VOID
LogMargin2DTestCfg (
  IN SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN MARGIN_2D_CONFIG      *pTestCfg
  );

/**
  @brief
  This function is used to log the CPGC point test configuration.

  @param[in]  SsaServicesHandle  Pointer to SSA services.
  @param[in]  pTestCfg           Pointer to test configuration structure.
**/
VOID
LogRunCpgcTestCfg (
  IN SSA_BIOS_SERVICES_PPI            *SsaServicesHandle,
  IN CONST RUN_CPGC_POINT_TEST_CONFIG *pTestCfg
);

/**
  @brief
  This function udpates the CPGC point test configuration header field with CPGC configuration version

  @param[in, out]  pCpgcPointTestCfg  Pointer to CPGC point test configuration structure.

**/
VOID
UpdateCpgcPointTestCfgHeader (
  IN OUT CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg
);

/**
  @brief
  This function returns the number of SubChannel and the width of a SubChannel.
  It is used to help display test results. BSSA test treats DDR5 channel as 80bits bus internally.
  The 80bits bus is broken into SubChannel when displaying test results.

        NumSubCh  SubChMaxBit
  DDR4      1          64/72
  DDR5      2          40

  @param[in    ] BusWidth    - DDR Bus width
  @param[   out] NumSubCh    - Number of SubChannel
  @param[   out] SubChMaxBit - Number of bits in a SubChannel

**/
VOID
GetSubChInfo (
  IN     UINT8 BusWidth,
     OUT UINT8 *NumSubCh,
     OUT UINT8 *SubChMaxBit
  );

/**
  This function collects the BCOM margin results for EV tests.
  The margin tests are conducted in BCOM training modes.

  @param[in]      CpgcConfig        Pointer to a CPGC configuration structure. It can be used to customize the EV margin test setup
  @param[in]      Socket            Processor socket within the system (0-based)
  @param[in]      ChannelMask       One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      DimmRank          Structure to specify current dimm and rank under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test
  @param[in]      Direction         Specifies the sweep direction: low side only, high side only, or both side
  @param[in]      StepSize          Specifies the StepSize to do the margining
  @param[in, out] Margins           Pointer to margin results data structure which is indexed as [MAX_CH][SUB_CH]

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
BcomMarginsWorker (
  IN     CPGC_POINT_TEST_CONFIG       *CpgcConfig,
  IN     UINT8                        Socket,
  IN     UINT32                       ChannelBitMask,
  IN     MRC_RT                       DimmRank[MAX_CH],
  IN     MRC_LT                       Level,
  IN     MRC_GT                       Group,
  IN     UINT8                        Direction,
  IN     UINT8                        StepSize,
  IN OUT struct baseMargin            Margins[MAX_CH][SUB_CH]
  );

/**
  This function collects the Cmd/Ctl margin results for EV tests.
  The margin tests are conducted in CA or CS training modes.

  @param[in]      TestType          Indicates the test type to be configured. It can use to customize the EV margin test setup
  @param[in]      Socket            Processor socket within the system (0-based)
  @param[in]      ChannelMask       One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      DimmRank          Structure to specify current dimm and rank under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test
  @param[in]      Direction         Specifies the sweep direction: low side only, high side only, or both side
  @param[in]      StepSize          Specifies the StepSize to do the margining
  @param[in, out] DcaResults        Pointer to margin results data structure which is indexed as [MAX_CH][SUB_CH][Signal]

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
CaCsMarginsWorker (
  IN     CPGC_POINT_TEST_CONFIG       *CpgcConfig,
  IN     UINT8                        Socket,
  IN     UINT32                       ChannelBitMask,
  IN     MRC_RT                       DimmRank[MAX_CH],
  IN     MRC_LT                       Level,
  IN     MRC_GT                       Group,
  IN     UINT8                        Direction,
  IN     UINT8                        StepSize,
  IN OUT struct baseMargin            Margins[MAX_CH][SUB_CH]
  );

/**
  This function collects the CA 1D margin results for EV tests.
  The margin tests are conducted in CA training modes.

  @param[in]      CpgcConfig        Pointer to a CPGC configuration structure. It can be used to customize the EV margin test setup
  @param[in]      Socket            Processor socket within the system (0-based)
  @param[in]      ChannelMask       One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      DimmRank          Structure to specify current dimm and rank under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test
  @param[in]      Direction         Specifies the sweep direction: low side only, high side only, or both side
  @param[in]      StepSize          Specifies the StepSize to do the margining
  @param[in]      CountWindow       Count Window Value
  @param[in, out] SweepResults      Pointer to margin results data structure which is indexed as [MAX_CH][SUB_CH][Signal]

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
CaMargins1DWorker (
  IN     CPGC_POINT_TEST_CONFIG       *CpgcConfig,
  IN     UINT8                        Socket,
  IN     UINT32                       ChannelBitMask,
  IN     MRC_RT                       DimmRank[MAX_CH],
  IN     MRC_LT                       Level,
  IN     MRC_GT                       Group,
  IN     UINT8                        Direction,
  IN     UINT8                        StepSize,
  IN     UINT32                       CountWindow,
  IN OUT struct baseMargin            (*SweepResults)[MAX_CH][SUB_CH][RCD_CA_SIGNALS_TO_SWEEP]
  );

/**
  This function collects the CS 1D margin results for EV tests.
  The margin tests are conducted in CS training modes.

  @param[in]      CpgcConfig        Pointer to a CPGC configuration structure. It can be used to customize the EV margin test setup
  @param[in]      Socket            Processor socket within the system (0-based)
  @param[in]      ChannelMask       One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      DimmRank          Structure to specify current dimm and rank under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test
  @param[in]      Direction         Specifies the sweep direction: low side only, high side only, or both side
  @param[in]      StepSize          Specifies the StepSize to do the margining
  @param[in]      CountWindow       Count Window Value
  @param[in, out] SweepResults      Pointer to margin results data structure which is indexed as [MAX_CH][SUB_CH]

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
CsMargins1DWorker (
  IN     CPGC_POINT_TEST_CONFIG       *CpgcConfig,
  IN     UINT8                        Socket,
  IN     UINT32                       ChannelBitMask,
  IN     MRC_RT                       DimmRank[MAX_CH],
  IN     MRC_LT                       Level,
  IN     MRC_GT                       Group,
  IN     UINT8                        Direction,
  IN     UINT8                        StepSize,
  IN     UINT32                       CountWindow,
  IN OUT struct baseMargin            (*SweepResults)[MAX_CH][SUB_CH]
  );


/**
  This function collects the QCA 1D margin results for EV tests.
  The margin tests are conducted in CA training modes.

  @param[in]      TestType          Indicates the test type to be configured. It can use to customize the EV margin test setup
  @param[in]      Socket            Processor socket within the system (0-based)
  @param[in]      ChannelMask       One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      DimmRank          Structure to specify current dimm and rank under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test
  @param[in]      Direction         Specifies the sweep direction: low side only, high side only, or both side
  @param[in]      StepSize          Specifies the StepSize to do the margining
  @param[in]      CountWindow       Count Window Value
  @param[in, out] SweepResults      Pointer to margin results data structure which is indexed as [MAX_CH][SUB_CH][Signal]

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
QcaMargins1DWorker (
  IN     CPGC_POINT_TEST_CONFIG       *CpgcConfig,
  IN     UINT8                        Socket,
  IN     UINT32                       ChannelBitMask,
  IN     MRC_RT                       DimmRank[MAX_CH],
  IN     MRC_LT                       Level,
  IN     MRC_GT                       Group,
  IN     UINT8                        Direction,
  IN     UINT8                        StepSize,
  IN     UINT32                       CountWindow,
  IN OUT struct baseMargin            (*SweepResults)[MAX_CH][SUB_CH][BACKSIDE_CA_SIGNALS_TO_SWEEP]
  );

/**
  This function collects the QCS 1D margin results for EV tests.
  The margin tests are conducted in DRAM CS training modes.

  @param[in]      TestType          Indicates the test type to be configured. It can use to customize the EV margin test setup
  @param[in]      Socket            Processor socket within the system (0-based)
  @param[in]      ChannelMask       One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      DimmRank          Structure to specify current dimm and rank under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test
  @param[in]      Direction         Specifies the sweep direction: low side only, high side only, or both side
  @param[in]      StepSize          Specifies the StepSize to do the margining
  @param[in]      CountWindow       Count Window Value
  @param[in, out] SweepResults      Pointer to margin results data structure which is indexed as [MAX_CH][SUB_CH]

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
QcsMargins1DWorker (
  IN     CPGC_POINT_TEST_CONFIG       *CpgcConfig,
  IN     UINT8                        Socket,
  IN     UINT32                       ChannelBitMask,
  IN     MRC_RT                       DimmRank[MAX_CH],
  IN     MRC_LT                       Level,
  IN     MRC_GT                       Group,
  IN     UINT8                        Direction,
  IN     UINT8                        StepSize,
  IN     UINT32                       CountWindow,
  IN OUT struct baseMargin            (*SweepResults)[MAX_CH][SUB_CH]
  );

/**
  This function returns if a margin group should be margined in in CA or CS training modes.

  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test

  @retval TRUE or FALSE
**/
BOOLEAN
EFIAPI
UseCaCsTrainingMode (
  IN     MRC_LT                       Level,
  IN     MRC_GT                       Group
  );

#endif //_EV_CONTENTCHIP_H_
