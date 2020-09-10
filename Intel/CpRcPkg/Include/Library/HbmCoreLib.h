/** @file
  Interface header file for the HBM library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#ifndef _HBM_CORE_LIB_H_
#define _HBM_CORE_LIB_H_

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <ReferenceCodeDataTypes.h>
#include <MrcCommonTypes.h>
#include "Memory/HbmJedec.h"
#include <MemCommon.h>

#define MAX_HBM_IO                                4

#define MAX_HBM_STROBE                            4
#define MAX_HBM_IO_MC                             4
#define MAX_HBM_MC_CH                             2
#define MAX_HBM_IO_CH                             (MAX_HBM_IO_MC * MAX_HBM_MC_CH)
#define MAX_HBM_ECC_BITS_PER_STROBE               4
#define MAX_HBM_DQ_BITS_PER_STROBE                32

#define MAX_HBM_BITS_PER_STROBE                   (MAX_HBM_DQ_BITS_PER_STROBE + MAX_HBM_ECC_BITS_PER_STROBE)
#define MAX_HBM_CH                                (MAX_HBM_IO * MAX_HBM_IO_CH)
#define MAX_HBM_BITS_PER_CH                       (MAX_HBM_STROBE * MAX_HBM_BITS_PER_STROBE)

#define HBM_MT_ADDR_DIR_UP                        0
#define HBM_MT_ADDR_DIR_DN                        1

//
// HBM policy Options
//
#define HBM_PAGE_POLICY                           BIT0
#define HBM_ADAPTIVE_PAGE_EN                      BIT1
#define HBM_DBI                                   BIT2
#define HBM_RELAXED_TA                            BIT3
#define HBM_PER_BANK_REFRESH_EN                   BIT4
#define HBM_TETHER_MODE_EN                        BIT5
#define HBM_ECC_TRACK_EN                          BIT6

//
// Debug levels
//
#define HBM_SDBG_NONE                             0
#define HBM_SDBG_MIN                              SDBG_HBM + SDBG_MIN
#define HBM_SDBG_MAX                              SDBG_HBM + SDBG_MAX
#define HBM_SDBG_TRACE                            SDBG_HBM + SDBG_TRACE
#define HBM_SDBG_MEM_TRAIN                        HBM_SDBG_MAX
#define HBM_SDBG_TST                              SDBG_HBM + SDBG_TST
#define HBM_SDBG_CPGC                             SDBG_HBM + SDBG_CPGC
#define HBM_SDBG_RCWRITETAG                       SDBG_HBM + SDBG_RCWRITETAG
#define HBM_SDBG_REG_ACCESS                       SDBG_HBM + SDBG_REG_ACCESS
#define HBM_SDBG_MINMAX                           SDBG_HBM + SDBG_MINMAX

//
// Debug print macros
//
#define HbmDebugPrint                             RcDebugPrint
#define HbmDebugPrintDevice                       RcDebugPrintWithDevice
#define HbmDebugPrintLine(DbgString)              RcDebugPrint (HBM_SDBG_MINMAX, DbgString)

//
// Advanced MemTest definitions
//
#define HBM_ADV_MT_TYPE_XMATS8                    0
#define HBM_ADV_MT_TYPE_XMATS16                   1
#define HBM_ADV_MT_TYPE_XMATS32                   2
#define HBM_ADV_MT_TYPE_XMATS64                   3
#define HBM_ADV_MT_TYPE_WCMATS8                   4
#define HBM_ADV_MT_TYPE_WCMCH8                    5
#define HBM_ADV_MT_TYPE_GNDB64                    6
#define HBM_ADV_MT_TYPE_MARCHCM64                 7
#define HBM_ADV_MT_TYPE_LTEST_SCRAM               8
#define HBM_ADV_MT_TYPE_LINIT_SCRAM               9

#define HBM_ADV_MT_LTEST_SCRAM                    (1 << HBM_ADV_MT_TYPE_LTEST_SCRAM)

//
// HBM Flows
//
#define HF_INIT                                   BIT0
#define HF_DISCOVERY                              BIT1
#define HF_DISPLAY                                BIT2
#define HF_CHECK_POR                              BIT3
#define HF_LANE_REPAIR                            BIT5
#define HF_BASIC_ALL_TRAINING                     BIT6
#define HF_THROTTLING                             BIT19
#define HF_MEM_TEST                               BIT20
#define HF_MEM_INIT                               BIT21
#define HF_RAS                                    BIT22
#define HF_RMT_SSA                                BIT23
#define HF_BDAT                                   BIT24
#define HF_LATE_CONFIG                            BIT26
#define HF_BUS_TURNAROUND                         BIT27
#define HF_EARLY_CONFIG                           BIT28
#define HF_CMI_CREDIT_PROGRAMMING                 BIT29

//
// HBM Policy related
//
typedef enum {
  HbmOptionCacheEnable = 0,
  HbmOptionCacheRatio,
  HbmOptionCacheFmType,
  HbmOptionCacheDdrtGran,
  HbmOptionSrefProgramming,
  HbmOptionSrefIdleTimer,
  HbmOptionOppSrefEn,
  HbmOptionPkgcSrefEn,
  HbmOptionMiscOptions,
  HbmOptionMax
} HBM_POLICY_OPTIONS;

typedef struct _HBM_POLICY {
  UINT8   HbmCacheDdrtGran;
  UINT8   HbmSrefProgramming;
  UINT8   HbmSrefIdleTimer;
  UINT8   HbmOppSrefEn;
  UINT8   HbmPkgcSrefEn;
  UINT8   HbmCacheEnable;
  UINT8   HbmCacheRatio;
  UINT8   HbmCacheFmType;
  UINT32  HbmMiscOptions;
} HBM_POLICY;

typedef enum {
  HbmCacheMr00 = 0,
  HbmCacheMr01,
  HbmCacheMr02,
  HbmCacheMr03,
  HbmCacheMr04,
  HbmCacheMr05,
  HbmCacheMr06,
  HbmCacheMr07,
  HbmCacheMr08,
  HbmCacheMr09,
  HbmCacheMr10,
  HbmCacheMr11,
  HbmCacheMr12,
  HbmCacheMr13,
  HbmCacheMr14,
  HbmCacheMr15,
  HbmMaxCacheMr
} HBM_CACHE_MR;

typedef enum {
  HbmErrorLaneRepair = 0,
  HbmErrorDllLock,
  HbmErrorIllegalGroup,
  HbmErrorCsrTrainingIdle,
  HbmErrorStartBusyNotClear,
  MaxHbmErrorHandler
} HBM_ERROR_HANDLER;

typedef struct _HBM_BIT_MASK {
  UINT32 Bits[MAX_HBM_CH][MAX_HBM_STROBE];
} HBM_BIT_MASK;

// ********************************************************************************************************************
//                                                                                                                    *
// Public HBM Core interfaces                                                                                         *
//                                                                                                                    *
// ********************************************************************************************************************

/**

  Convert HBM IO stack index and MC index to HBM Mesh2Mem instance index.

  @param[in] HbmIoId         HBM IO stack index.
  @param[in] HbmMcId         HBM MC index within HBM IO stack.

  @retval HBM Mesh2Mem instance index.

**/
#define HBM_M2M_INSTANCE_ID(HbmIoId, HbmMcId)  (MAX_IMC + ((HbmIoId) * MAX_HBM_IO_MC) + (HbmMcId))

/**

  Gets architectural variables for HBM

  @param[out]               HbmMaxIoInst        Pointer to specify the max number of instances
  @param[out]               HbmMaxChannel       Pointer to specify the max number of channels
  @param[out]               HbmNumChPerIo       Pointer to specify the max number of channels per IO

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
HbmPopulateLegacyVariables (
  OUT   UINT8           *HbmMaxIoInst,
  OUT   UINT8           *HbmMaxChannel,
  OUT   UINT8           *HbmNumChPerIo
  );

/**

  Populate HBM global variables based on the MSM table

  @param[in]                Socket              Current Socket (0-Based)

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
EFIAPI
HbmPopulateArchVariables (
  IN        UINT8           Socket
  );

/**

  Verify HBM arch variables

  @param[in]                MsmListPtr          Pointer to MSM List

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
EFIAPI
VerifyHbmMsmList (
  IN      MRC_MSM           *MsmListPtr
  );

/**

  Set the flow bits

  @param[in]                Flows               Value that represents all steps for HBM

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
SetHbmFlows (
  IN        UINT32          Flows
  );

/**

  Returns the flow bits

  @return                   Flow bits

**/
UINT32
EFIAPI
GetHbmFlows (
  VOID
  );

/**
  This function enables all HBM flows.

  @param VOID

  @retval EFI_SUCCESS   All HBM flows are enabled successfully.
**/
EFI_STATUS
EFIAPI
EnableAllHbmFlows (
  VOID
  );

/**

  Allocates the required resources for HBM Host

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors
  @retval                   EFI_OUT_OF_RESOURCES  Pei services not able to allocate resources for HBM Host

**/
EFI_STATUS
EFIAPI
HbmHostAllocation (
  VOID
  );

/**
  Gets the value for the policy option specified

  @param[in]                HBM_POLICY_OPTIONS  Specific HBM Setup Option

  @return                   Value for specified setup option

**/
UINT32
EFIAPI
HbmGetPolicyOption (
  IN        HBM_POLICY_OPTIONS  HbmPolicyOption
  );

/**
  Sets the value for the setup option specified

  @param[in]                HBM_POLICY_OPTIONS  Specific HBM Setup Option
  @param[in]                ConfigValue          Value

**/
VOID
EFIAPI
HbmSetPolicyOption (
  IN        HBM_POLICY_OPTIONS  HbmPolicyOption,
  IN        UINT32              ConfigValue
  );

/**

  Determines if HBM memory subsystem is enabled or disabled.

  @param[in] Socket         Current Socket (0-Based)
  @param[in] MemSs          Current HBM memory subsystem

  @retval TRUE              HBM memory subsystem is enabled
  @retval FALSE             HBM memory subsystem is disabled

**/
BOOLEAN
EFIAPI
IsHbmMemSsEnabled (
  IN UINT8    Socket,
  IN UINT8    MemSs
  );

/**

  Sets if MemSs is enabled or not

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSs               Current Memory Sub System
  @param[in]                Enable              Value to set

**/
VOID
EFIAPI
SetHbmMemSsEnable (
  IN        UINT8               Socket,
  IN        UINT8               MemSs,
  IN        UINT8               Enable
  );

/**
  This function gets the HBM frequency from HBM Host.

  @param[in] SocketId         Socket index.

  @retval  The value of HBM frequency.
**/
HBM_FREQUENCY
EFIAPI
HbmGetFrequency (
  IN UINT8  SocketId
  );

/**
  This function gets the HBM frequency in MHz from HBM Host for the specified socket.

  @param[in] SocketId         Socket index.

  @retval    The value of HBM frequency in Mhz.
**/
UINT16
EFIAPI
HbmGetFrequencyInMhz (
  IN UINT8  SocketId
  );

/**
  This function sets the HBM frequency into HBM Host.

  @param[in] SocketId         Socket index.
  @param[in] HbmFrequency     HBM frequency to set.

  @retval N/A.
**/
VOID
EFIAPI
HbmSetFrequency (
  IN UINT8          SocketId,
  IN HBM_FREQUENCY  HbmFrequency
  );

/**

  Sets (Enables/Disables) an HBM channel

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Enable              Value to set

**/
VOID
EFIAPI
SetHbmChEnable (
  IN        UINT8               Socket,
  IN        UINT8               Channel,
  IN        UINT8               Enable
  );

/**

  Gets the start/end channel of a particular MemSs

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSs               Current Memory Sub System
  @param[out]               StartCh             Start channel of current MemSs (Pointer)
  @param[out]               EndCh               End channel of current MemSs (Pointer)

**/
VOID
EFIAPI
GetHbmStartEndCh (
  IN    UINT8            Socket,
  IN    UINT8            MemSs,
  OUT   UINT8           *StartCh,
  OUT   UINT8           *EndCh
  );
/**
  Gets the cache value for Mem Size

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSs               Current Memory Sub System

  @return                   Cache value for Mem Size

**/
UINT16
EFIAPI
GetHbmCacheMemSize (
  IN        UINT8               Socket,
  IN        UINT8               MemSs
  );

/**
  Sets the cache value for Mem Size

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSs               Current Memory Sub System
  @param[in]                Value               Value to set

**/
VOID
EFIAPI
SetHbmCacheMemSize (
  IN        UINT8               Socket,
  IN        UINT8               MemSs,
  IN        UINT16              Value
  );

/**

  HBM training step

  @param[in]                Socket              Current Socket (0-Based)

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
EFIAPI
HbmAdvAwordTraining (
  IN        UINT8           Socket
  );

/**

  Initialize  HBM IO via FSM mechanism.

  @param[in]                Socket              Current Socket (0-Based)

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
EFIAPI
HbmBasicTraining (
  IN        UINT8           Socket
  );

/**
  HBM Discovery

  This function reads the Device ID data from HBM FW and detects the HBM device attributes
  according to the Device ID data. The HBM device attributes will be cached into HBM Host.

  @param[in] SocketId             Socket index.

  @retval  EFI_SUCCESS            This function completed the HBM discovery successfully.
  @retval  !EFI_SUCCESS           This function failed to discover HBM device.
**/
EFI_STATUS
EFIAPI
HbmDiscovery (
  IN UINT8       SocketId
  );

/**

  Display HBM Memory information

  @param[in]                Socket              Current Socket (0-Based)

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/

UINT32
EFIAPI
HbmMemDisplay (
  IN        UINT8           Socket
  );

/**

  The advanced RD Strobe centering determines an optimal timing centering for Rx
  using information about the setup and hold margins of the data eye.

  @param[in]                Socket              Current Socket (0-Based)

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
EFIAPI
HbmAdvRxDwordTraining (
  IN        UINT8           Socket
  );

/**

  The advanced WR Strobe centering determines an optimal timing centering for Tx
  using information about the setup and hold margins of the data eye.

  @param[in]                Socket              Current Socket (0-Based)

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
EFIAPI
HbmAdvTxDwordTraining (
  IN        UINT8           Socket
  );

/**

  The advanced RX VREF centering determines an optimal timing centering for RX VREF
  using information about the low and high margins of the data eye.

  @param[in]                Socket              Current Socket (0-Based)

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
EFIAPI
HbmRxRefCentering (
  IN        UINT8           Socket
  );

/**

  Initialize  HBM data structures.

  @param[in]                Socket              Current Socket (0-Based)

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
HbmInit (
  IN        UINT8           Socket
  );

/**

  Memory Test - Modified Scan algorithm

  @param[in]                Socket              Current Socket (0-Based)

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
EFIAPI
HbmMemoryTest (
  IN        UINT8           Socket
  );

/**

  Memory Init - initialize all the HBM Memory to '0'

  @param[in]                Socket              Current Socket (0-Based)

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
EFIAPI
HbmMemoryInit (
  IN        UINT8           Socket
  );

/**

  JEDEC initialization for HBM.

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            Index of the HBM IO stack.

**/
VOID
HbmJedecInit (
  IN        UINT8           Socket,
  IN        UINT8           IoModule
  );

/**

  Get max strobes based on current memory subsystem

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSsIndex          Current Memory Sub System

  @return                   Max strobes

**/
UINT8
EFIAPI
HbmGetMaxStrobesMemSs (
  IN        UINT8           Socket,
  IN        UINT8           MemSsIndex
  );

/**

  Get max strobes based on channel

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)

  @return                   Max strobes

**/
UINT8
EFIAPI
HbmGetMaxStrobesChannel (
  IN        UINT8           Socket,
  IN        UINT8           Channel
  );

/**

  Get max bits based on current memory subsystem

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSsIndex          Current Memory Sub System

  @return                   Max Bits

**/
UINT8
EFIAPI
HbmGetMaxBitsMemSs (
  IN        UINT8           Socket,
  IN      UINT8             MemSsIndex
  );

/**

  Get max bits based on channel

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)

  @return                   Max bits

**/
UINT8
EFIAPI
HbmGetMaxBitsChannel (
  IN        UINT8           Socket,
  IN      UINT8             Channel
  );

/**

  Get max channels per MemSs based on current MemSs (0-Based)

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSsIndex          Current Memory Sub System

  @return                   Max channels for current HBM-MemSs

**/
UINT8
EFIAPI
HbmGetMaxChannelMemSs (
  IN      UINT8             Socket,
  IN      UINT8             MemSsIndex
  );

/**

  Get max HBM Memory subsystems

  @param[in]                Socket              Current Socket (0-Based)

  @return                   Max HBM-MemSs

**/
UINT8
EFIAPI
HbmGetMaxMemSs (
  IN        UINT8           Socket
  );

/**
  Get ManufactureId

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                HbmIoIndex          The HBMIO instance index (0~ max HBMIO in socket -1)

**/
UINT32
EFIAPI
GetHbmManufacturerId (
  IN UINT8     Socket,
  IN UINT8     HbmIoIndex
  );

/**
  Get the pointer to HBM device ID data.

  @param[in] Socket              Current Socket (0-Based)
  @param[in] HbmIoIndex          The HBMIO instance index (0~ max HBMIO in socket -1)

  @retval  The pointer to HBM device ID data.
**/
DEVICE_ID_WDR *
EFIAPI
GetHbmDeviceId (
  IN UINT8     Socket,
  IN UINT8     HbmIoIndex
  );

/**
  Get gen2_test from DEVICE_ID_WDR

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                HbmIoIndex          The HBMIO instance index (0~ max HBMIO in socket -1)

**/
BOOLEAN
EFIAPI
GetHbmGen2Test (
  IN UINT8     Socket,
  IN UINT8     HbmIoIndex
  );

/**
  Get the HBM Pannic Handle Info

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            The HBMIO instance index (0~ max HBMIO in socket -1)

**/
UINT32
EFIAPI
GetHbmPanHandleInfo (
  IN UINT8     Socket,
  IN UINT8     HbmIoIndex
  );

/**
  Converts the input channel to proper MemSs Index

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)

  @return                   Hbm Memory Sub system Index based on input Channel

**/
UINT8
EFIAPI
ConvertChannelToMemSsIndex (
  IN        UINT8           Socket,
  IN        UINT8           Channel
  );

/**

  Determine if an specific step needs to be executed

  @param[in]                FlowBit             An step represented by one or more bits

  @retval                   TRUE                All bits are set
  @retval                   FALSE               At least 1 bit is not set for current flow

**/
BOOLEAN
EFIAPI
IsHbmFlowEnabled (
  IN        UINT32          FlowBit
  );

/**

  Programs HBM DWORD Registers

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)

  @retval                   TRUE                Channel is enabled
  @retval                   FALSE               Channels is not enabled

**/
BOOLEAN
EFIAPI
IsHbmChannelEnabled (
  IN        UINT8               Socket,
  IN        UINT8               Channel
  );

/**

  Gets the cached value for the specified MRS

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSs               Current Memory Sub System
  @param[in]                HbmCacheMr          HBM MRS

  @return                   Gets the cached value for the specified MRS

**/
UINT8
EFIAPI
GetHbmCacheMr (
  IN        UINT8               Socket,
  IN        UINT8               MemSs,
  IN        HBM_CACHE_MR        HbmCacheMr
  );

/**

  Sets the cached value for the specified MRS

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSs               Current Memory Sub System
  @param[in]                HbmCacheEye         HBM MRS
  @param[in]                CacheValue          Value to set

**/
VOID
EFIAPI
SetHbmCacheMr (
  IN        UINT8               Socket,
  IN        UINT8               MemSs,
  IN        HBM_CACHE_MR        HbmCacheMr,
  IN        UINT8               CacheValue
  );

/**

  Apply settings after Memory Test ends

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                ScrambleConfigOrg   Original value

**/
VOID
EFIAPI
HbmPostAdvancedMemTest (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT32          ScrambleConfigOrg
  );

/**
  Sets the registers to Mask off data bits from reporting failures

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Mask                Bits to be masked

**/
VOID
EFIAPI
HbmSetBwErrorMask (
  IN        UINT8           Socket,
  IN        VOID            *Mask
  );

/**

  Runs Advanced Memory Tests based on the legacy Scan algorithm with HBM scrambler enabled

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                ChEnMap             Each bit indicates if channel is enabled/disabled
  @param[in]                TestType            Test type to execute
  @param[in]                Mode                CPGC Mode of operation
  @param[in]                SeqDataInv          Enables Pattern inversion per subsequence
  @param[in]                PatternQW           Pattern for DQ
  @param[in]                UiShl               UI shifted
  @param[in]                NumCacheLines       Number of cachelines
  @param[in]                PatternDepth        Pattern Depth
  @param[in]                Direction           Sequential address Direction

  @return                   Error Status across any Strobe

**/
UINT32
EFIAPI
HbmCpgcMemTestMats (
  IN        UINT8           Socket,
  IN        UINT32          ChEnMap,
  IN        UINT8           TestType,
  IN        UINT8           Mode,
  IN        UINT8           *SeqDataInv,
  IN        UINT64_STRUCT   *PatternQW,
  IN        UINT8           *UiShl,
  IN        UINT8           NumCacheLines,
  IN        UINT8           PatternDepth,
  IN        UINT8           Direction
  );

/**

  Execute test

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                ChEnMap             Each bit indicates if channel is enabled/disabled

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
EFIAPI
HbmCpgcAdvTrainingExecTest (
  IN        UINT8           Socket,
  IN        UINT32          ChEnMap
  );

/**
  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                ChEnMap             Each bit indicates if channel is enabled/disabled
  @param[in]                Rank                Logical rank (in HBM represents the station ID)
  @param[in]                SubRank             Logical sub-rank (in HBM is always '0')
  @param[in]                Group               MRC_GT parameter to program
  @param[in]                Pattern             Pattern Depth
  @param[in]                DqLoopCount         Loopcount

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
EFIAPI
HbmCpgcAdvTrainingSetup (
  IN        UINT8           Socket,
  IN        UINT32          ChEnMap,
  IN        UINT8           Rank,
  IN        UINT8           SubRank,
  IN        MRC_GT          Group,
  IN        UINT32          Pattern,
  IN        UINT8           DqLoopCount
  );

/**

  Runs Advanced HBM Memory Tests based on the legacy Scan algorithm with scrambler enabled

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                ChEnabled           List of channels to be tested
  @param[in]                TestType            Memory test type
  @param[in]                TestSize            Memory test size
  @param[in]                ShiftRate           Rate to shift the WDB content

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
EFIAPI
HbmMemTestScram (
  IN        UINT8           Socket,
  IN        UINT32          ChEnabled,
  IN        UINT8           TestType,
  IN        UINT8           TestSize,
  IN        UINT8           ShiftRate,
  IN        UINT32          RepeatTest
  );

/**
  Gets the MemSs architecture based on current Socket and channel

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[out]               MaxMemSs            Max Number of HBM MemSs
  @param[out]               MaxChannelsAll      Max Number of channels across All MemSs
  @param[out]               MaxChannels         Max Number of channels for current MemSs
  @param[out]               MaxStrobes          Max Number of data IO (Strobes) for current Channel
  @param[out]               MaxStrobesSub       Max Number of data IO (Strobes) for current SubChannel
  @param[out]               MaxBits             Max Number of bits IO for current Channel

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors
  @retval                   EFI_UNSUPPORTED     If number of max channel/strobe/bits does not match arch defines
**/
EFI_STATUS
EFIAPI
GetHbmMemSsParameters (
  IN    UINT8           Socket,
  IN    UINT8           Channel,
  OUT   UINT32          *MaxMemSs,
  OUT   UINT32          *MaxChannelsAll,
  OUT   UINT32          *MaxChannels,
  OUT   UINT32          *MaxStrobes,
  OUT   UINT32          *MaxStrobesSub,
  OUT   UINT32          *MaxBits
  );

UINT32
EFIAPI
HbmCpgcMemTestWithScramble (
  IN        UINT8           Socket,
  IN        UINT32          ChEnMap,
  IN        UINT8           NumCacheLines,
  IN        UINT8           PatternDepth,
  IN        UINT8           Direction
  );

/**

  Get the enabled HBM Memory Subsystems.
  1 bit per memory subsystem (one hot)

  @param[in]                Socket        Current Socket index
  @param[out]               MemSsEnabled  Pointer to specify the HBM MemSs enabled

  @retval                   EFI_SUCCESS   Function executed

**/
EFI_STATUS
EFIAPI
GetMemSsEnabled (
  IN        UINT8         Socket,
  OUT   UINT8         *MemSsEnabled
  );

/**

  Set/Clear the reset_io pin

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            Index of the HBM IO stack
  @param[in]                Value               Value to program for reset
**/
VOID
TrainResetAction (
  IN        UINT8           Socket,
  IN        UINT8           IoModule,
  IN        UINT8           Value
  );

/**
  Toggles the reset_io pin

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            Index of the HBM IO stack.

**/
VOID
TrainResetToggle (
  IN        UINT8           Socket,
  IN        UINT8           IoModule
  );

/**

  Writes HBM Mode registers using IEEE1500

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            Index of the HBM IO stack.
  @param[in]                Mrs                 MRS register number (0-based)
  @param[in]                Data                Data for the MRS write

**/
VOID
WriteMrs1500 (
  IN        UINT8           Socket,
  IN        UINT8           IoModule,
  IN        UINT8           Mrs,
  IN        UINT8           Data
  );

/**

  Generate HBM commands IEEE1500

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            Index of the HBM IO stack.
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Command             IEEE1500 command
  @param[in]                Size                Size of data to Rd/Wr to/from IEEE1500
  @param[in]                Data                Data return path

**/
VOID
Hbm1500PortCmd (
  IN        UINT8           Socket,
  IN        UINT8           IoModule,
  IN        UINT8           Channel,
  IN        UINT8           Command,
  IN        UINT8           Size,
  IN        UINT8           *Data
  );

/**

  Get HBM MTR per Density

  @param[in]      HbmDensity     - HBM Density
  @param[in]      AddressingMode - Addressing Mode Support
  @param[in, out] DimmMtr        - Pointer to DIMM MTR

  @retval EFI_SUCCESS     if got
          EFI_UNSUPPORTED otherwise

**/
EFI_STATUS
EFIAPI
HbmGetDimmMtr (
  IN     HBM_DENSITY             HbmDensity,
  IN     UINT8                   AddressingMode,
  IN OUT DIMMMTR_DATA_STRUCT     *DimmMtr
  );

/**

  This function returns the value of tCK with unit pico seconds

  @param[in] Socket  - Socket Id

  @retval UINT16 Value of tCK

**/
UINT16
HbmGettCK (
  IN UINT8 Socket
  );

/**

  This function returns the value of tRCD_RD

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRCD_RD

**/
UINT32
HbmGettRCD_RD (
  IN UINT8 Socket
  );

/**

  This function returns the value of tRCD_WR

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRCD_WR

**/
UINT32
HbmGettRCD_WR (
  IN UINT8 Socket
  );

UINT32
HbmGettRCD_ImpRD (
  IN UINT8 Socket
  );

UINT32
HbmGettRCD_ImpWR (
  IN UINT8 Socket
  );

/**

  This function returns the value of tRRD_L

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRRD_L

**/
UINT32
HbmGettRRD_L (
  IN UINT8 Socket
  );

/**

  This function returns the value of tRRD

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRRD

**/
UINT32
HbmGettRRD (
  IN UINT8 Socket
  );

/**

  This function returns the value of tFAW

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tFAW

**/
UINT32
HbmGettFAW (
  IN UINT8 Socket
  );

/**

  This function returns the value of tCL

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tCL

**/
UINT32
HbmGettCL (
  IN UINT8 Socket
  );

/**

  This function returns the value of tCWL

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tCWL

**/
UINT32
HbmGettCWL (
  IN UINT8 Socket
  );

/**

  This function returns the value of tRTP

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRTP

**/
UINT32
HbmGettRTP (
  IN UINT8 Socket
  );

/**

  This function returns the value of tRP

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRP

**/
UINT32
HbmGettRP (
  IN UINT8 Socket
  );

/**

  This function returns the value of tRC

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRC

**/
UINT32
HbmGettRC (
  IN UINT8 Socket
  );

/**

  This function returns the value of tRAS

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRAS

**/
UINT32
HbmGettRAS (
  IN UINT8 Socket
  );

/**

  This function returns the value of tPRPDEN

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tPRPDEN

**/
UINT32
HbmGettPRPDEN (
  IN UINT8 Socket
  );

/**

  This function returns the value of tCKE

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tCKE

**/
UINT32
HbmGettCKE (
  IN UINT8 Socket
  );

/**

  This function returns the value of tXP

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tXP

**/
UINT32
HbmGettXP (
  IN UINT8 Socket
  );

/**

  This function returns the value of tRDPDEN

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRDPDEN

**/
UINT32
HbmGettRDPDEN (
  IN UINT8 Socket
  );

/**

  This function returns the value of tWRPDEN

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tWRPDEN

**/
UINT32
HbmGettWRPDEN (
  IN UINT8 Socket
  );

/**

  This function returns the value of tRDA

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tRDA

**/
UINT32
HbmGettRDA (
  IN UINT8 Socket
  );

/**

  This function returns the value of tWR

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tWRA

**/
UINT32
HbmGettWR (
  IN UINT8 Socket
  );

/**

  This function returns the value of tWRA

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tWRA

**/
UINT32
HbmGettWRA (
  IN UINT8 Socket
  );

/**

  This function returns the value of tWRPRE

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tWRPRE

**/
UINT32
HbmGettWRPRE (
  IN UINT8 Socket
  );

/**

  This function returns the value of tWRRDA

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tWRRDA

**/
UINT32
HbmGettWRRDA (
  IN UINT8 Socket
  );

/**

  Set Pseudo Channel Mask for FSM HBM

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            Index of the HBM IO stack.
  @param[in]                Pseudo              Even or odd pseudo Ch number (0-based)

**/
VOID
HbmFsmPseudoChMask (
  IN        UINT8           Socket,
  IN        UINT8           IoModule,
  IN        UINT8           Pseudo
  );

/**

  Get HBM stack routing per channel

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            Index of the HBM IO stack.

**/
VOID
HbmModRoutingPoll (
  IN        UINT8           Socket,
  IN        UINT8           IoModule
  );

/**

  HBM Lane Repair Auto Load.

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            Index of the HBM IO stack.

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
EFI_STATUS
HbmLaneRepairAutoLoad (
  IN        UINT8           Socket,
  IN        UINT8           IoModule
  );

/**
  Setups the initial settings prior to start FSM training

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            Index of the HBM IO stack.

**/
VOID
TrainBaseSequence (
  IN        UINT8           Socket,
  IN        UINT8           IoModule
  );

/**

  This deasserts resets all HBM MC IO modules

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            Index of the HBM IO stack.
  @param[in]                Set                 Assert/deasert (Boolean)

**/
VOID
SetClearHbmReset (
  IN        UINT8           Socket,
  IN        UINT8           IoModule,
  IN        BOOLEAN         Set
  );

/**

  This disables HBM memory subSistem and all HBM channels within IO module

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            Index of the HBM IO stack.

**/
VOID
DisableHbmMod (
  IN        UINT8           Socket,
  IN        UINT8           IoModule
  );

/**

  Applies settings before memory test execution

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)

  @return                   Scrambler register value

**/
UINT32
HbmPreAdvancedMemTest (
  IN        UINT8           Socket,
  IN        UINT8           Channel
  );

/**

  Apply settings after Memory Test ends

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                ScrambleConfigOrg   Original value

**/
VOID
HbmPostAdvancedMemTestRestoreScrambler (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT32          ScrambleConfigOrg
  );

/**
  Sets the number of Max Sockets in the HBM Host structure

  @param[in]                MaxSockets          Specifies the number for Max Sockets

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
SetHbmMaxSockets (
  IN        UINT8           MaxSockets
  );

/**
  This function detects which HBM memory subsystems are enabled.

  @param[in] SocketId       Current Socket index (0-Based).

  @retval HbmMemSsBitMap    The bitmap of HBM memory subsystems. Enabled = 1; Disabled = 0.
                            One bit corresponds to one HBM memory subsystem.
**/
UINT8
EFIAPI
HbmDetected (
  IN UINT8    SocketId
  );

/**
  Gets the value for the setup option specified

  @param[in]                HBM_POLICY_OPTIONS  Specific HBM Setup Option

  @return                   Value for specified setup option

**/
UINT32
EFIAPI
GetHbmSetupOption (
  IN        HBM_POLICY_OPTIONS  HbmSetupOption
  );

#endif // _HBM_CORE_LIB_H_
