/** @file
  Mem SMEE CMI Credit information
  This file is for SPR

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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
#include <Uefi.h>
#include <CmiPerformance.h>
#include <Library/MemMap2SgxInterface.h>
#include <Library/MemSmeeIpLib.h>
#include "MemSmeeCmiCredits.h"
#include "MemSmeeCmiPerformanceData.h"
#include <Library/CpuAndRevisionLib.h>
#include <Library/RcDebugLib.h>
#include <Library/MemTypeLib.h>

BOOLEAN McTmeCmiDataVersionPrintedDone = FALSE;

/**
  Mem Smee Get CMI Tables

@param[in]   Socket                           -   Socket number
@param[in]   Channel                          -   Channel number
@param[in]   *Config_Table_TME_XTILE_A0       -   Pointer to A0 Stepping TME XTILE Config Table.
@param[in]   *Config_Table_XTILE_A0           -   Pointer to A0 Stepping XTILE Config Table.
@param[in]   *Config_Table_TME_2LM_A0         -   Pointer to A0 Stepping TME 2LM Config Table.
@param[in]   *Config_Table_2LM_A0             -   Pointer to A0 Stepping 2LM Config Table.
@param[in]   *Config_Table_TME_A0             -   Pointer to A0 Stepping TME Config Table.
@param[in]   *Config_Table_A0                 -   Pointer to A0 Stepping NO_TME NO_XTILE NO_2LM Config Table.
@param[in]   *Config_Table_TME_XTILE_B0       -   Pointer to B0 Stepping TME XTILE Config Table.
@param[in]   *Config_Table_XTILE_B0           -   Pointer to B0 Stepping XTILE Config Table.
@param[in]   *Config_Table_TME_2LM_B0         -   Pointer to B0 Stepping TME 2LM Config Table.
@param[in]   *Config_Table_2LM_B0             -   Pointer to B0 Stepping 2LM Config Table.
@param[in]   *Config_Table_TME_B0             -   Pointer to B0 Stepping TME Config Table.
@param[in]   *Config_Table_B0                 -   Pointer to B0 Stepping NO_TME NO_XTILE NO_2LM Config Table.
@param[in]   *Config_Table_TME_XTILE_C0       -   Pointer to C0 Stepping TME XTILE Config Table.
@param[in]   *Config_Table_XTILE_C0           -   Pointer to C0 Stepping XTILE Config Table.
@param[in]   *Config_Table_TME_2LM_C0         -   Pointer to C0 Stepping TME 2LM Config Table.
@param[in]   *Config_Table_2LM_C0             -   Pointer to C0 Stepping 2LM Config Table.
@param[in]   *Config_Table_TME_C0             -   Pointer to C0 Stepping TME Config Table.
@param[in]   *Config_Table_C0                 -   Pointer to C0 Stepping NO_TME NO_XTILE NO_2LM Config Table.
@param[in]   Size_Config_Table_TME_XTILE_A0   -   Size of A0 Stepping TME XTILE Config Table.
@param[in]   Size_Config_Table_XTILE_A0       -   Size of A0 Stepping XTILE Config Table.
@param[in]   Size_Config_Table_TME_2LM_A0     -   Size of A0 Stepping TME 2LM Config Table.
@param[in]   Size_Config_Table_2LM_A0         -   Size of A0 Stepping 2LM Config Table.
@param[in]   Size_Config_Table_TME_A0         -   Size of A0 Stepping TME Config Table.
@param[in]   Size_Config_Table_A0             -   Size of A0 Stepping NO_TME NO_XTILE NO_2LM Config Table.
@param[in]   Size_Config_Table_TME_XTILE_B0   -   Size of B0 Stepping TME XTILE Config Table.
@param[in]   Size_Config_Table_XTILE_B0       -   Size of B0 Stepping XTILE Config Table.
@param[in]   Size_Config_Table_TME_2LM_B0     -   Size of B0 Stepping TME 2LM Config Table.
@param[in]   Size_Config_Table_2LM_B0         -   Size of B0 Stepping 2LM Config Table.
@param[in]   Size_Config_Table_TME_B0         -   Size of B0 Stepping TME Config Table.
@param[in]   Size_Config_Table_B0             -   Size of B0 Stepping NO_TME NO_XTILE NO_2LM Config Table.
@param[in]   Size_Config_Table_TME_XTILE_C0   -   Size of C0 Stepping TME XTILE Config Table.
@param[in]   Size_Config_Table_XTILE_C0       -   Size of C0 Stepping XTILE Config Table.
@param[in]   Size_Config_Table_TME_2LM_C0     -   Size of C0 Stepping TME 2LM Config Table.
@param[in]   Size_Config_Table_2LM_C0         -   Size of C0 Stepping 2LM Config Table.
@param[in]   Size_Config_Table_TME_C0         -   Size of C0 Stepping TME Config Table.
@param[in]   Size_Config_Table_C0             -   Size of C0 Stepping NO_TME NO_XTILE NO_2LM Config Table.
@param[out]  *ConfigValue                     -   Pointer to Register Configuration value.
@param[out]  *Size                            -   Pointer to Structure Size

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
MemSmeeGetCmiTableChannel(
    IN  UINT8                 Socket,
    IN  UINT8                 Channel,
    IN  CMI_DEFAULT_REG_VALUE *Config_Table_TME_XTILE_A0,
    IN  CMI_DEFAULT_REG_VALUE *Config_Table_XTILE_A0,
    IN  CMI_DEFAULT_REG_VALUE *Config_Table_TME_2LM_A0,
    IN  CMI_DEFAULT_REG_VALUE *Config_Table_2LM_A0,
    IN  CMI_DEFAULT_REG_VALUE *Config_Table_TME_A0,
    IN  CMI_DEFAULT_REG_VALUE *Config_Table_A0,
    IN  CMI_DEFAULT_REG_VALUE *Config_Table_TME_XTILE_B0,
    IN  CMI_DEFAULT_REG_VALUE *Config_Table_XTILE_B0,
    IN  CMI_DEFAULT_REG_VALUE *Config_Table_TME_2LM_B0,
    IN  CMI_DEFAULT_REG_VALUE *Config_Table_2LM_B0,
    IN  CMI_DEFAULT_REG_VALUE *Config_Table_TME_B0,
    IN  CMI_DEFAULT_REG_VALUE *Config_Table_B0,
    IN  CMI_DEFAULT_REG_VALUE *Config_Table_TME_XTILE_C0,
    IN  CMI_DEFAULT_REG_VALUE *Config_Table_XTILE_C0,
    IN  CMI_DEFAULT_REG_VALUE *Config_Table_TME_2LM_C0,
    IN  CMI_DEFAULT_REG_VALUE *Config_Table_2LM_C0,
    IN  CMI_DEFAULT_REG_VALUE *Config_Table_TME_C0,
    IN  CMI_DEFAULT_REG_VALUE *Config_Table_C0,
    IN  UINT8                 Size_Config_Table_TME_XTILE_A0,
    IN  UINT8                 Size_Config_Table_XTILE_A0,
    IN  UINT8                 Size_Config_Table_TME_2LM_A0,
    IN  UINT8                 Size_Config_Table_2LM_A0,
    IN  UINT8                 Size_Config_Table_TME_A0,
    IN  UINT8                 Size_Config_Table_A0,
    IN  UINT8                 Size_Config_Table_TME_XTILE_B0,
    IN  UINT8                 Size_Config_Table_XTILE_B0,
    IN  UINT8                 Size_Config_Table_TME_2LM_B0,
    IN  UINT8                 Size_Config_Table_2LM_B0,
    IN  UINT8                 Size_Config_Table_TME_B0,
    IN  UINT8                 Size_Config_Table_B0,
    IN  UINT8                 Size_Config_Table_TME_XTILE_C0,
    IN  UINT8                 Size_Config_Table_XTILE_C0,
    IN  UINT8                 Size_Config_Table_TME_2LM_C0,
    IN  UINT8                 Size_Config_Table_2LM_C0,
    IN  UINT8                 Size_Config_Table_TME_C0,
    IN  UINT8                 Size_Config_Table_C0,
    OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
    OUT UINT8                 *Size
)
{
    MEMORY_MODE_ATTRIBUTE MemMode;
    BOOLEAN TmeEnabled;
    BOOLEAN TwoLmEnabled;

    GetMemMode(Socket, &MemMode);
    TmeEnabled = IsTmeEnabled(Socket);
    TwoLmEnabled = IsMemTypePresentChFunction(Socket, Channel, IsMemType2lmDdr);
    RcDebugPrint(SDBG_MINMAX, "TmeEnabled: %d\n", TmeEnabled);
    RcDebugPrint(SDBG_MINMAX, "TwoLmEnabled: %d\n", TwoLmEnabled);
    RcDebugPrint(SDBG_MINMAX, "2LM_XTILE: %d\n", (MemMode & MEMORY_MODE_ATTRIBUTE_2LM_XTILE));
    if (IsCpuAndRevisionOrLater(CPU_SPRSP, REV_C0)) {
        RcDebugPrint(SDBG_MINMAX, "CMI Credit Data for CPU Stepping: C0 and Later\n");
        if ((MemMode & MEMORY_MODE_ATTRIBUTE_2LM_XTILE) != 0) {
            if (TmeEnabled) {
                *ConfigValue = Config_Table_TME_XTILE_C0;
                *Size = Size_Config_Table_TME_XTILE_C0 / sizeof(CMI_DEFAULT_REG_VALUE);
            }
            else {
                *ConfigValue = Config_Table_XTILE_C0;
                *Size = Size_Config_Table_XTILE_C0 / sizeof(CMI_DEFAULT_REG_VALUE);
            }
        }
        else {
            if (TwoLmEnabled) {
                if (TmeEnabled) {
                    *ConfigValue = Config_Table_TME_2LM_C0;
                    *Size = Size_Config_Table_TME_2LM_C0 / sizeof(CMI_DEFAULT_REG_VALUE);
                }
                else {
                    *ConfigValue = Config_Table_2LM_C0;
                    *Size = Size_Config_Table_2LM_C0 / sizeof(CMI_DEFAULT_REG_VALUE);
                }
            }
            else {
                if (TmeEnabled) {
                    *ConfigValue = Config_Table_TME_C0;
                    *Size = Size_Config_Table_TME_C0 / sizeof(CMI_DEFAULT_REG_VALUE);
                }
                else {
                    *ConfigValue = Config_Table_C0;
                    *Size = Size_Config_Table_C0 / sizeof(CMI_DEFAULT_REG_VALUE);
                }
            }
        }
    }
    else if (IsCpuAndRevision(CPU_SPRSP, REV_B0)) {
        RcDebugPrint(SDBG_MINMAX, "CMI Credit Data for CPU Stepping: B0\n");
        if ((MemMode & MEMORY_MODE_ATTRIBUTE_2LM_XTILE) != 0) {
            if (TmeEnabled) {
                *ConfigValue = Config_Table_TME_XTILE_B0;
                *Size = Size_Config_Table_TME_XTILE_B0 / sizeof(CMI_DEFAULT_REG_VALUE);
            }
            else {
                *ConfigValue = Config_Table_XTILE_B0;
                *Size = Size_Config_Table_XTILE_B0 / sizeof(CMI_DEFAULT_REG_VALUE);
            }
        }
        else {
            if (TwoLmEnabled) {
                if (TmeEnabled) {
                    *ConfigValue = Config_Table_TME_2LM_B0;
                    *Size = Size_Config_Table_TME_2LM_B0 / sizeof(CMI_DEFAULT_REG_VALUE);
                }
                else {
                    *ConfigValue = Config_Table_2LM_B0;
                    *Size = Size_Config_Table_2LM_B0 / sizeof(CMI_DEFAULT_REG_VALUE);
                }
            }
            else {
                if (TmeEnabled) {
                    *ConfigValue = Config_Table_TME_B0;
                    *Size = Size_Config_Table_TME_B0 / sizeof(CMI_DEFAULT_REG_VALUE);
                }
                else {
                    *ConfigValue = Config_Table_B0;
                    *Size = Size_Config_Table_B0 / sizeof(CMI_DEFAULT_REG_VALUE);
                }
            }
        }
    }
    else {
        RcDebugPrint(SDBG_MINMAX, "CMI Credit Data for CPU Stepping: A0\n");
        if ((MemMode & MEMORY_MODE_ATTRIBUTE_2LM_XTILE) != 0) {
            if (TmeEnabled) {
                *ConfigValue = Config_Table_TME_XTILE_A0;
                *Size = Size_Config_Table_TME_XTILE_A0 / sizeof(CMI_DEFAULT_REG_VALUE);
            }
            else {
                *ConfigValue = Config_Table_XTILE_A0;
                *Size = Size_Config_Table_XTILE_A0 / sizeof(CMI_DEFAULT_REG_VALUE);
            }
        }
        else {
            if (TwoLmEnabled) {
                if (TmeEnabled) {
                    *ConfigValue = Config_Table_TME_2LM_A0;
                    *Size = Size_Config_Table_TME_2LM_A0 / sizeof(CMI_DEFAULT_REG_VALUE);
                }
                else {
                    *ConfigValue = Config_Table_2LM_A0;
                    *Size = Size_Config_Table_2LM_A0 / sizeof(CMI_DEFAULT_REG_VALUE);
                }
            }
            else {
                if (TmeEnabled) {
                    *ConfigValue = Config_Table_TME_A0;
                    *Size = Size_Config_Table_TME_A0 / sizeof(CMI_DEFAULT_REG_VALUE);
                }
                else {
                    *ConfigValue = Config_Table_A0;
                    *Size = Size_Config_Table_A0 / sizeof(CMI_DEFAULT_REG_VALUE);
                }
            }
        }
    }
    return EFI_SUCCESS;
}

#define MEMSMEE_GET_CMI_TABLE_CHANNEL(RegisterName)                                                              \
  MemSmeeGetCmiTableChannel (                                                                                    \
   Socket,                                                                                                      \
   Channel, \
   RegisterName##_Tme_XtileA0, \
   RegisterName##_XtileA0, \
   RegisterName##_Tme_2LMA0, \
   RegisterName##_2LMA0, \
   RegisterName##_TmeA0, \
   RegisterName##A0, \
   RegisterName##_Tme_XtileB0, \
   RegisterName##_XtileB0, \
   RegisterName##_Tme_2LMB0, \
   RegisterName##_2LMB0, \
   RegisterName##_TmeB0, \
   RegisterName##B0, \
   RegisterName##_Tme_XtileC0, \
   RegisterName##_XtileC0, \
   RegisterName##_Tme_2LMC0, \
   RegisterName##_2LMC0, \
   RegisterName##_TmeC0, \
   RegisterName##C0, \
   sizeof (RegisterName##_Tme_XtileA0), \
   sizeof (RegisterName##_XtileA0), \
   sizeof (RegisterName##_Tme_2LMA0), \
   sizeof (RegisterName##_2LMA0), \
   sizeof (RegisterName##_TmeA0), \
   sizeof (RegisterName##A0), \
   sizeof (RegisterName##_Tme_XtileB0), \
   sizeof (RegisterName##_XtileB0), \
   sizeof (RegisterName##_Tme_2LMB0), \
   sizeof (RegisterName##_2LMB0), \
   sizeof (RegisterName##_TmeB0), \
   sizeof (RegisterName##B0), \
   sizeof (RegisterName##_Tme_XtileC0), \
   sizeof (RegisterName##_XtileC0), \
   sizeof (RegisterName##_Tme_2LMC0), \
   sizeof (RegisterName##_2LMC0), \
   sizeof (RegisterName##_TmeC0), \
   sizeof (RegisterName##C0), \
   ConfigValue, \
   Size);

/**
  Get MC TME Cmi Configuration Value
  Returns value for SPR

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval None
**/
VOID
GetMcTmeCmiCfgValueChannelSpr (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
  MEMSMEE_GET_CMI_TABLE_CHANNEL (McTmeCmiCfgValue)
}

/**
  Get MC TME CMI Qdepth Value0
  Returns value for SPR

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval None
**/
VOID
GetMcTmeCmiQdepthValue0ChannelSpr (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
  MEMSMEE_GET_CMI_TABLE_CHANNEL (McTmeCmiQdepthValue0)
}

/**
  Get MC TME CMI Qdepth Value1
  Returns value for SPR

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval None
**/
VOID
GetMcTmeCmiQdepthValue1ChannelSpr (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
  MEMSMEE_GET_CMI_TABLE_CHANNEL (McTmeCmiQdepthValue1)
}

/**
  Get MC TME Configuration Value
  Returns value for SPR

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval None
**/
VOID
GetMcTmeCfgValueChannelSpr (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
  MEMSMEE_GET_CMI_TABLE_CHANNEL (McTmeCfgValue)
}

/**
  Get MC TME Rdb Configuration Value (lower 32 bit)
  Returns value for SPR

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval None
**/
VOID
GetMcTmeRdbCfgValueChannelSpr (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
  MEMSMEE_GET_CMI_TABLE_CHANNEL (McTmeRdbCfgValue)
}

/**
  Get MC TME Configuration structures

  @param[in    ] Socket              - Socket number
  @param[in    ] Channel             - Channel number
  @param[   out] **McTmeCmiCfgValue  - Pointer to Register Configuration value
  @param[   out] *McTmeCmiCfgLen     - Pointer to Structure Size
  @param[   out] **McTmeRdbCfgValue  - Pointer to Register Configuration value
  @param[   out] *McTmeRdbCfgLen     - Pointer to Structure Size
  @param[   out] **McTmeRdbCfgValue1 - Pointer to Register Configuration value
  @param[   out] *McTmeRdbCfgValue1  - Pointer to Structure Size
  @param[   out] **McTmeCfgValue     - Pointer to Register Configuration value
  @param[   out] *McTmeCfgLen        - Pointer to Structure Size

  @retval N/A
**/
VOID
EFIAPI
GetMcTmeStructures (
  IN     UINT8                 Socket,
  IN     UINT8                 Channel,
     OUT CMI_DEFAULT_REG_VALUE **McTmeCmiCfgValue,
     OUT UINT8                 *McTmeCmiCfgLen,
     OUT CMI_DEFAULT_REG_VALUE **McTmeRdbCfgValue,
     OUT UINT8                 *McTmeRdbCfgLen,
     OUT CMI_DEFAULT_REG_VALUE **McTmeRdbCfgValue1,
     OUT UINT8                 *McTmeRdbCfgValue1Len,
     OUT CMI_DEFAULT_REG_VALUE **McTmeCfgValue,
     OUT UINT8                 *McTmeCfgLen
  )
{
  GetMcTmeCmiCfgValueChannelSpr (Socket, Channel, McTmeCmiCfgValue, McTmeCmiCfgLen);
  GetMcTmeRdbCfgValueChannelSpr (Socket, Channel, McTmeRdbCfgValue, McTmeRdbCfgLen);
  GetMcTmeCfgValueChannelSpr (Socket, Channel, McTmeCfgValue, McTmeCfgLen);
}

/**
  Get TME structure values based on programable parameter

  @param[in] ProgramableValue     - Programmable CMI credit value
  @param[in] *McTmeStructureValue - Structure pointer containing the fields for the programable value
  @param[in] *RegData             - Register data to be returned

  @retval N/A
**/
VOID
EFIAPI
GetMcTmeStructureField (
  IN     UINT8                 ProgramableValue,
  IN     CMI_DEFAULT_REG_VALUE McTmeStructureValue,
     OUT UINT32                *RegData
  )
{
  switch (ProgramableValue) {
    case CMI_DDRT_VAL:
      *RegData = McTmeStructureValue.DdrtVal;
      break;
    case CMI_1LM_VAL:
      *RegData = McTmeStructureValue.OneLMVal;
      break;
    case CMI_CR_VAL:
      *RegData = McTmeStructureValue.CRVal;
      break;
    case CMI_1LM_ISOCH_VAL:
      *RegData = McTmeStructureValue.OneLMIsoChVal;
      break;
    case CMI_1LM_2WAY_VAL:
      *RegData = McTmeStructureValue.OneLM2WAYVal;
      break;
    case CMI_CR_2WAY_VAL:
      *RegData = McTmeStructureValue.CR2WAYVal;
      break;
    default:
      break;
  }
}

/**

  Print McTme CMI SpreadSheet Data Version

  @param[in] None

  @retVal    None
**/
VOID
EFIAPI
PrintMcTmeCmiDataVersion (
  VOID
  )
{
  UINT8 CmiDataVersion = 0xFF;

  if (McTmeCmiDataVersionPrintedDone) {
    return;
  }
  if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_C0)) {
    CmiDataVersion = MEMSMEE_CMI_PERFORMANCE_DATA_VERSION_C0;
  } else if (IsCpuAndRevision (CPU_SPRSP, REV_B0)) {
    CmiDataVersion = MEMSMEE_CMI_PERFORMANCE_DATA_VERSION_B0;
  } else {
    CmiDataVersion = MEMSMEE_CMI_PERFORMANCE_DATA_VERSION_A0;
  }
  RcDebugPrint (SDBG_MINMAX, "McTme CMI Data Version: %d\n", CmiDataVersion);
  McTmeCmiDataVersionPrintedDone = TRUE;
}