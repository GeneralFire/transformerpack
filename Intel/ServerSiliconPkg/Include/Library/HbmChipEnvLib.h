/** @file
  Interface header file for the HBM library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2017 Intel Corporation. <BR>

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

#ifndef _HBM_CHIP_ENV_LIB_H_
#define _HBM_CHIP_ENV_LIB_H_


/**

  Send delay to MRC server

  @param[in]                Clocks              Delay time

**/
VOID
EFIAPI
DelayDClk (
  IN        INT32           Clocks
  );

/**

  Send delay to MRC server

  @param[in]                Clocks              Delay time in QCLKS

**/
VOID
EFIAPI
DelayQClk (
  IN        INT32           Clocks
  );

/**

  Send delay to MRC server if CTE,
  if any other environment is detected system will wait a 'DelayInUs' microseconds


  @param[in]                Clocks              Delay time in DCLKS (Only for CTE)
  @param[in]                DelayInUs           Delay in microseconds (for any other environment)

**/
VOID
EFIAPI
HbmDelay (
  IN        INT32           Clocks,
  IN        UINT32          DelayInUs
  );

/**

  Return TRUE when the elapsed time is greater or equal than 'DelayInUs' microsecond.
  To calculate elapsed time it is required to have an initial reference ('StartTime'),
  and then apply the next equation:

  TimeElapsed = CurrentTime - 'StartTime', this is done by GetDelayHL(..) function


  @param[in]                StartTime           Start time used as reference to know completion
  @param[in]                DelayInUs           Delay in microseconds (for any other environment)

  @retval                   TRUE                When TimeElapsed is greater or equal that 'DelayInUs' microsecond
  @retval                   FALSE               Otherwise

**/
BOOLEAN
EFIAPI
CheckTimerExpiration (
  IN        UINT32          StartTime,
  IN        UINT32          DelayInUs
  );

/**

  HBM Error Handler

  @param[in]                HbmError            HBM error code
  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            Index of the HBM IO stack.

**/
VOID
EFIAPI
HbmHandleError (
  IN        HBM_ERROR_HANDLER HbmError,
  IN        UINT8           Socket,
  IN        UINT8           IoModule
  );

/**

  Initializes the CSR register

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Group               MRC_GT parameter to program
  @param[in]                Mode                Indicates the mode used to program the register:
                                                HBM_CSR_NONE_POINT:   Csr is programmed with 'Value'
                                                HBM_CSR_MIDDLE_POINT: Csr is programmed with MIDDLE point + 'Value' (is signed)
                                                HBM_CSR_MIN_POINT:    Csr is programmed with MIN point + 'Value' (is signed)
                                                HBM_CSR_MAX_POINT:    Csr is programmed with MAX point + 'Value' (is signed)
  @param[in]                Value               Value to be written

**/
VOID
EFIAPI
CsrSetup (
  IN        UINT8           Socket,
  IN        MRC_GT          Group,
  IN        UINT8           Mode,
  IN        INT16           Value
  );


/**
  Overrides the panhandle register information

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                IoModule            Index of the HBM IO stack.
  @param[in]                OffsetReg           Offset register
  @param[in]                ForceOverride       Force register override when applicable to environment

**/
VOID
EFIAPI
OverridePanHandleRegister (
    IN        UINT8           Socket,
    IN        UINT8           IoModule,
    IN        UINT32          OffsetReg,
    IN        BOOLEAN         ForceOverride
  );

/**

  Override CPGC errors

  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Strobe              Current Strobe (0-Based)
  @param[out]               HbmCpgcErrorStatus  Cpgc Error Status

**/
VOID
EFIAPI
OverrideCpgcErrors (
  IN        UINT8           Channel,
  IN        UINT8           Strobe,
  IN        CPGC_ERRORS     HbmCpgcErrorStatus[MAX_HBM_CH][MAX_HBM_STROBE]
  );

/**

  Override results from CPGC test for Clear Errors and Start Test bit

  @param[out]               Global              Pointer to Get/Set value for global features

**/
VOID
EFIAPI
OverrideCpcgTestStatus (
  IN        GLOBAL_FEATURE  *GlobalFeature
  );

/**

  Override results from CPGC test for test done

  @param[out]               LoopTestDoneStatus  Pointer to specify the Test done value

**/
VOID
EFIAPI
OverrideTestDoneStatus (
  IN        UINT8           *LoopTestDoneStatus
  );

/**

  Override results read data from 1500 port

  @param[out]               Hbm1500RdData       Pointer to read data for 1500 port

**/
VOID
EFIAPI
OverrideDiscoveryData (
  IN        UINT32          *Hbm1500RdData
  );

/**

  Override timing settings for Jedec & MC

  @param[out]               JedecVendorTimings  Pointer address to Jedec settings
  @param[out]               McGuardbands        Pointer address to MC settings

**/
VOID
EFIAPI
OverrideTimingSettings (
  IN        UINT8          **JedecVendorTimings,
  IN        UINT8          **McGuardbands
  );

/**

  Override timing settings for Jedec vendor settings

  @param[out]               JedecVendor         Pointer address to Jedec vendor settings

**/
VOID
EFIAPI
OverrideJedecVendorData (
  IN        UINT8          **JedecVendor
  );

/**

  Override loopcount depending on the environment

  @param[out]               SequenceChannelFeatures     Pointer to sequence channel features

**/
VOID
EFIAPI
OverrideLoopcount (
  IN        SUBSEQUENCE_CHANNEL_FEATURE   *SequenceChannelFeatures
  );

/**

  Get DWORD iteration count

  @return     Iteration count

**/
UINT8
EFIAPI
GetDwordIterationCount (
  VOID
  );

/**

  Get basic train step

  @return     Basic train step size

**/
UINT8
EFIAPI
GetBasicTrainStep (
  VOID
  );

/**

  Get chip IO latencies depending on environment and Read/Write latencies

  @param[in]                ReadLatency         Read latency value in NCLK
  @param[in]                WriteLatency        Write latency value in NCLK
  @param[out]               SaOnDuration        Pointer to SaOnDuration
  @param[out]               RxSaEnDelay         Pointer to RxSaEnDelay
  @param[out]               TxFifoWr            Pointer to TxFifoWr

**/
VOID
EFIAPI
GetChipIoLatency (
  IN        UINT8           ReadLatency,
  IN        UINT8           WriteLatency,
      OUT   UINT8           *SaOnDuration,
      OUT   UINT8           *RxSaEnDelay,
      OUT   UINT8           *TxFifoWr
  );

/**

  Get Fast WDB option from the environment

  @param[out]               FastWdb             Pointer to FastWdb option

**/
VOID
EFIAPI
GetFastWdbOption (
      OUT   UINT32          *FastWdb
  );
#endif // _HBM_CHIP_ENV_LIB_H_
