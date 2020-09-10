/** @file
  Mesh 2 Mem CMI Credit information
  This file is for SPR

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
#include <Uefi.h>
#include <CmiPerformance.h>
#include <Library/MemMap2SgxInterface.h>
#include <Library/MemSmeeIpLib.h>
#include <Library/RcDebugLib.h>

#if defined(IA32)
#include <Library/CpuAndRevisionLib.h>
#include "Mesh2MemCmiPerformanceData.h"
#include <Library/MemTypeLib.h>
#endif //#if defined (IA32) || defined (SIM_BUILD)

#if defined(IA32)
/**
Mesh2Mem Get CMI Tables

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
Mesh2MemGetCmiTableChannel(
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

#define MESH2MEM_GET_CMI_TABLE_CHANNEL(RegisterName)                                                              \
  Mesh2MemGetCmiTableChannel (                                                                                    \
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

#endif //#if defined (IA32) || defined (SIM_BUILD)

/**
  Get Read Completion Credit Channel Config Value per Channel
  Returns value for SPR

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval None
**/
VOID
GetCmiRdCplCreditChnlConfig1ValueChannelSpr (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
#if defined(IA32)
  MESH2MEM_GET_CMI_TABLE_CHANNEL (CmiRdCplCreditChnlConfig1Value)
#endif //#if defined (IA32) || defined (SIM_BUILD)
}

/**
  Get Read Completion Credit Channel Config Value per Channel
  Returns value for SPR

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval None
**/
VOID
GetCmiRdCplCreditChnlConfig2ValueChannelSpr (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
#if defined(IA32)
  MESH2MEM_GET_CMI_TABLE_CHANNEL (CmiRdCplCreditChnlConfig2Value)
#endif //#if defined (IA32) || defined (SIM_BUILD)
}

/**
  Get CMI Rsp Credit Channel Config Value per Channel
  Returns value of SPR

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval None
**/
VOID
GetCmiRspCreditChnlConfigValueChannelSpr (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
#if defined(IA32)
  MESH2MEM_GET_CMI_TABLE_CHANNEL (CmiRspCreditChnlConfigValue)
#endif //#if defined (IA32) || defined (SIM_BUILD)
}

/**

  Print Mesh2Mem CMI SpreadSheet Data Version

  @param[in] None

  @retVal    None
**/
VOID
EFIAPI
PrintMesh2MemCmiDataVersion (
  VOID
  )
{
#if defined(IA32)
  UINT8 CmiDataVersion = 0xFF;

  if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_C0)) {
    CmiDataVersion = MESH2MEM_CMI_PERFORMANCE_DATA_VERSION_C0;
  } else if (IsCpuAndRevision (CPU_SPRSP, REV_B0)) {
    CmiDataVersion = MESH2MEM_CMI_PERFORMANCE_DATA_VERSION_B0;
  } else {
    CmiDataVersion = MESH2MEM_CMI_PERFORMANCE_DATA_VERSION_A0;
  }
  RcDebugPrint (SDBG_MINMAX, "Mesh2Mem CMI Data Version: %d\n", CmiDataVersion);
#endif //#if defined (IA32) || defined (SIM_BUILD)
}
