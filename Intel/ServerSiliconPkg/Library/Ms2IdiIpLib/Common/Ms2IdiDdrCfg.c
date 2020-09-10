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
#include <Library/UsraCsrLib.h>
#include <Library/Ms2IdiLib.h>
#include <KtiSetupDefinitions.h>
#include "Include/Ms2IdiLibInternal.h"

/**
  MS2IDI:  Get LB_MCHECK_W_RAC_N0

  @param SocId           - Socket ID

  @retval                - LB_MCHECK_W_RAC_N0

**/
UINT32
EFIAPI
Ms2IdiGetSaiLowValueRac (
  UINT8 SocId
  )
{
  UINT32 SaiLowValue;

  SaiLowValue = UsraCsrRead (SocId, MS2IDI_MULTICAST, LB_MCHECK_W_RAC_N0_MS2IDI_MAIN_REG);

  return SaiLowValue;
}

/**
  MS2IDI:  Get LB_MCHECK_W_WAC_N0

  @param SocId           - Socket ID

  @retval                - LB_MCHECK_W_WAC_N0

**/
UINT32
EFIAPI
Ms2IdiGetSaiLowValueWac (
  UINT8 SocId
  )
{
  UINT32 SaiLowValue;

  SaiLowValue = UsraCsrRead (SocId, MS2IDI_MULTICAST, LB_MCHECK_W_WAC_N0_MS2IDI_MAIN_REG);

  return SaiLowValue;
}

/**
  MS2IDI:  Set LB_MCHECK_W_WAC_N0

  @param SocId           - Socket ID
  @param SaiLowValue     - Sai Low Value to set

  @retval                - None

**/
VOID
EFIAPI
Ms2IdiSetSaiLowValueWac (
  UINT8 SocId,
  UINT32 SaiLowValue
  )
{
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, LB_MCHECK_W_WAC_N0_MS2IDI_MAIN_REG, SaiLowValue);
}

/**
  MS2IDI:  Get LB_MCHECK_W_WAC_N1

  @param SocId           - Socket ID

  @retval                - LB_MCHECK_W_WAC_N1

**/
UINT32
EFIAPI
Ms2IdiGetSaiHighValueWac (
  UINT8 SocId
  )
{
  UINT32 SaiHighValue;

  SaiHighValue = UsraCsrRead (SocId, MS2IDI_MULTICAST, LB_MCHECK_W_WAC_N1_MS2IDI_MAIN_REG);

  return SaiHighValue;
}

/**
  MS2IDI:  Set LB_MCHECK_W_WAC_N1

  @param SocId           - Socket ID
  @param SaiHighValue    - Sai Low Value to set

  @retval                - None

**/
VOID
EFIAPI
Ms2IdiSetSaiHighValueWac (
  UINT8 SocId,
  UINT32 SaiHighValue
  )
{
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, LB_MCHECK_W_WAC_N1_MS2IDI_MAIN_REG, SaiHighValue);
}

/**
  MS2IDI:  Get LB_MCHECK_W_CP_N0

  @param SocId           - Socket ID

  @retval                - LB_MCHECK_W_CP_N0

**/
UINT32
EFIAPI
Ms2IdiGetSaiLowValueCp (
  UINT8 SocId
  )
{
  UINT32 SaiLowValue;

  SaiLowValue = UsraCsrRead (SocId, MS2IDI_MULTICAST, LB_MCHECK_W_CP_N0_MS2IDI_MAIN_REG);

  return SaiLowValue;
}

/**
  MS2IDI:  Set LB_MCHECK_W_CP_N0

  @param SocId           - Socket ID
  @param SaiLowValue     - Sai Low Value to set

  @retval                - None

**/
VOID
EFIAPI
Ms2IdiSetSaiLowValueCp (
  UINT8 SocId,
  UINT32 SaiLowValue
  )
{
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, LB_MCHECK_W_CP_N1_MS2IDI_MAIN_REG, SaiLowValue);
}

/**
  MS2IDI:  Get LB_MCHECK_W_CP_N1

  @param SocId           - Socket ID

  @retval                - LB_MCHECK_W_CP_N1

**/
UINT32
EFIAPI
Ms2IdiGetSaiHighValueCp (
  UINT8 SocId
  )
{
  UINT32 SaiHighValue;

  SaiHighValue = UsraCsrRead (SocId, MS2IDI_MULTICAST, LB_MCHECK_W_CP_N1_MS2IDI_MAIN_REG);

  return SaiHighValue;
}

/**
  MS2IDI:  Set LB_MCHECK_W_CP_N1

  @param SocId           - Socket ID
  @param SaiHighValue    - Sai Low Value to set

  @retval                - None

**/
VOID
EFIAPI
Ms2IdiSetSaiHighValueCp (
  UINT8 SocId,
  UINT32 SaiHighValue
  )
{
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, LB_MCHECK_W_CP_N1_MS2IDI_MAIN_REG, SaiHighValue);
}

/**
  MS2IDI:  Enable/Disable the DDRT QoS Feature and program the required registers

  @param SocId            - Socket ID
  @param DdrtQosMode      - DDRT QoS Mode

  @retval         - None

**/
VOID
EFIAPI
Ms2IdiSetEnDdrtQoS (
  UINT8  SocId,
  UINT8  DdrtQosMode
  )
{
  DDRT_QOS_CFG1_MS2IDI_MAIN_STRUCT  DdrtQosCfg1;
  DDRT_QOS_CFG2_MS2IDI_MAIN_STRUCT  DdrtQosCfg2;
  DDRT_QOS_CFG3_MS2IDI_MAIN_STRUCT  DdrtQosCfg3;

  TWOLM_QOS_CONFIG1_MS2IDI_MAIN_STRUCT TwolmQosConfig1;
  TWOLM_QOS_CONFIG2_MS2IDI_MAIN_STRUCT TwolmQosConfig2;
  TWOLM_QOS_CONFIG5_MS2IDI_MAIN_STRUCT TwolmQosConfig5;
  TWOLM_QOS_CONFIG6_MS2IDI_MAIN_STRUCT TwolmQosConfig6;

  DdrtQosCfg2.Data = UsraCsrRead (SocId, MS2IDI_MULTICAST, DDRT_QOS_CFG2_MS2IDI_MAIN_REG);
  if (DdrtQosMode == DDRT_QOS_MODE_2) {
    DdrtQosCfg2.Bits.ddrtthdisable = 0;
    DdrtQosCfg2.Bits.leakybktwindowcntr = 0x800;
    DdrtQosCfg2.Bits.ddrtwroralltranscount = 0x01;
  } else {
    DdrtQosCfg2.Bits.ddrtthdisable = 0x01;
  }
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, DDRT_QOS_CFG2_MS2IDI_MAIN_REG, DdrtQosCfg2.Data);

  if (DdrtQosMode == DDRT_QOS_MODE_2) {
    DdrtQosCfg1.Data = UsraCsrRead (SocId, MS2IDI_MULTICAST, DDRT_QOS_CFG1_MS2IDI_MAIN_REG);
    DdrtQosCfg1.Bits.ddrtwrthreshold = 0x07;
    DdrtQosCfg1.Bits.ddrtthreshold = 0x60;
    UsraCsrWrite (SocId, MS2IDI_MULTICAST, DDRT_QOS_CFG1_MS2IDI_MAIN_REG, DdrtQosCfg1.Data);

    DdrtQosCfg3.Data = UsraCsrRead (SocId, MS2IDI_MULTICAST, DDRT_QOS_CFG3_MS2IDI_MAIN_REG);
    DdrtQosCfg3.Bits.leakybktsatvalue = 0x10;
    UsraCsrWrite (SocId, MS2IDI_MULTICAST, DDRT_QOS_CFG3_MS2IDI_MAIN_REG, DdrtQosCfg3.Data);
  }
  TwolmQosConfig1.Data = UsraCsrRead (SocId, MS2IDI_MULTICAST, TWOLM_QOS_CONFIG1_MS2IDI_MAIN_REG);
  TwolmQosConfig1.Bits.leakybktwindowcntr = 0x800;
  TwolmQosConfig1.Bits.leakybktsatvalue = 0x10;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, TWOLM_QOS_CONFIG1_MS2IDI_MAIN_REG, TwolmQosConfig1.Data);

  TwolmQosConfig2.Data = UsraCsrRead (SocId, MS2IDI_MULTICAST, TWOLM_QOS_CONFIG2_MS2IDI_MAIN_REG);
  TwolmQosConfig2.Bits.threshold = 0x07;
  TwolmQosConfig2.Bits.low_threshold = 0x00;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, TWOLM_QOS_CONFIG2_MS2IDI_MAIN_REG, TwolmQosConfig2.Data);

  TwolmQosConfig5.Data = UsraCsrRead (SocId, MS2IDI_MULTICAST, TWOLM_QOS_CONFIG5_MS2IDI_MAIN_REG);
  TwolmQosConfig5.Bits.leakybktwindowcntr = 0x1500;
  TwolmQosConfig5.Bits.leakybktsatvalue = 0x10;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, TWOLM_QOS_CONFIG5_MS2IDI_MAIN_REG, TwolmQosConfig5.Data);

  TwolmQosConfig6.Data = UsraCsrRead (SocId, MS2IDI_MULTICAST, TWOLM_QOS_CONFIG6_MS2IDI_MAIN_REG);
  TwolmQosConfig6.Bits.threshold = 0x07;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, TWOLM_QOS_CONFIG6_MS2IDI_MAIN_REG, TwolmQosConfig6.Data);

  Ms2IdiSetEnDdrtQoSChip (SocId, DdrtQosMode);
}

/**
  MS2IDI:  Enable/Disable the DDRT QoS Feature

  @param SocId    - Socket ID
  @param EnVal    - Enable/Disable the DDRT QoS Feature

  @retval         - None

**/
VOID
EFIAPI
Ms2IdiSetDdrtRange (
  UINT8  SocId,
  UINT32 DdrtBaseAddress,
  UINT32 DdrtLimitAddress
  )
{
  DDRT_RANGE_BASE_MS2IDI_MAIN_STRUCT  DdrtRangeBase;
  DDRT_RANGE_LIMIT_MS2IDI_MAIN_STRUCT DdrtRangeLimit;

  DdrtRangeBase.Data  = 0;
  DdrtRangeLimit.Data = 0;

  DdrtRangeBase.Bits.ddrt_base_address    = DdrtBaseAddress;
  DdrtRangeLimit.Bits.ddrt_limit_address  = DdrtLimitAddress;

  UsraCsrWrite (SocId, MS2IDI_MULTICAST, DDRT_RANGE_BASE_MS2IDI_MAIN_REG, DdrtRangeBase.Data);
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, DDRT_RANGE_LIMIT_MS2IDI_MAIN_REG, DdrtRangeLimit.Data);
}