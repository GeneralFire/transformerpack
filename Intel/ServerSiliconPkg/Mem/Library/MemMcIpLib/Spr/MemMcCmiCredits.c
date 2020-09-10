/** @file
  Cmi credit values.
  This file has been generated Automatically using CmiCreditExtractor.py, please avoid direct edit.

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
#include <SysHostChip.h>
#include <MemCommon.h>
#include <CmiPerformance.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemRcLib.h>
#include <Library/IoAccessLib.h>
#include "Include/MemMcRegs.h"
#include <Library/MemMap2SgxInterface.h>
#include <Library/MemSmeeIpLib.h>
#include "Include/MemMcIpLibInternal.h"
#include "MemMcCmiPerformanceData.h"
#include <Library/MemoryServicesLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/MemTypeLib.h>

/**
  Save the register default value of the read and write CmiCreditConfig registers

  @param[in]   Host      -   Pointer to SysHost
  @param[in]   Socket    -   Socket number

  @retval  N/A
**/
VOID
SaveCmiCreditConfigDefault (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket
  )
{
  UINT8   Ch;

  //
  // Save only the default value of channle 0 - assume all channels have the same default
  //
  Ch = 0;
  Host->var.mem.CmiCreditConfigDefault.CmiRdCreditConfigN0 = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N0_MC_2LM_REG);
  Host->var.mem.CmiCreditConfigDefault.CmiRdCreditConfigN1 = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N1_MC_2LM_REG);
  Host->var.mem.CmiCreditConfigDefault.CmiWrCreditConfigN0 = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N0_MC_2LM_REG);
  Host->var.mem.CmiCreditConfigDefault.CmiWrCreditConfigN1 = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N1_MC_2LM_REG);
  //
  // Indicate the save buffer is valid
  //
  Host->var.mem.CmiCreditConfigDefault.Valid = TRUE;
}

/**

  Save the default value of CmiCreditConfig registers

  @param[in]    Socket                    -  Socket number
  @param[in, out]   CmiCreditConfigDefault    -  Pointer to a CMI_CREDIT_CONFIG_DEFAULT buffer

  @retval  N/A

**/
VOID
EFIAPI
SaveCmiCreditConfig (
  IN     UINT8                     Socket,
  IN OUT CMI_CREDIT_CONFIG_DEFAULT *CmiCreditConfigDefault
  )
{
  UINT8      Ch;

  if (CmiCreditConfigDefault == NULL) {
    RcDebugPrint (SDBG_MINMAX, "Warning: CmiCreditConfigDefault pointer is NULL\n");
    return;
  }

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    CmiCreditConfigDefault->CmiRdCreditConfigN0 = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N0_MC_2LM_REG);
    CmiCreditConfigDefault->CmiRdCreditConfigN1 = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N1_MC_2LM_REG);
    CmiCreditConfigDefault->CmiWrCreditConfigN0 = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N0_MC_2LM_REG);
    CmiCreditConfigDefault->CmiWrCreditConfigN1 = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N1_MC_2LM_REG);

    //
    // Indicate the save buffer is valid
    //
    CmiCreditConfigDefault->Valid = TRUE;
  }
}

/**

  Restore the read and write CmiCreditConfig registers to default values

  @param[in]   Host      -   Pointer to SysHost
  @param[in]   Socket    -   Socket number
  @param[in]   Ch        -   Channel number within the socket

  @retval  N/A

**/
VOID
RestoreCmiCreditConfigDefault (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket,
  IN  UINT8       Ch
  )
{
  //
  // Only restore if the save buffer is valid
  //
  if (Host->var.mem.CmiCreditConfigDefault.Valid) {
    //
    // Assume all channels have the same default value
    //
    MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N0_MC_2LM_REG, Host->var.mem.CmiCreditConfigDefault.CmiRdCreditConfigN0);
    MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N1_MC_2LM_REG, Host->var.mem.CmiCreditConfigDefault.CmiRdCreditConfigN1);
    MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N0_MC_2LM_REG, Host->var.mem.CmiCreditConfigDefault.CmiWrCreditConfigN0);
    MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N1_MC_2LM_REG, Host->var.mem.CmiCreditConfigDefault.CmiWrCreditConfigN1);
  }
}


/**

  Restore the values of CmiCreditConfig registers

  @param[in]   Socket                    -  Socket number
  @param[in]   CmiCreditConfigDefault    -  Pointer to a CMI_CREDIT_CONFIG_DEFAULT buffer

  @retval  N/A

**/
VOID
EFIAPI
RestoreCmiCreditConfig (
  IN UINT8                     Socket,
  IN CMI_CREDIT_CONFIG_DEFAULT *CmiCreditConfigDefault
  )
{
  UINT8      Ch;

  if (CmiCreditConfigDefault == NULL) {
    return;
  }

  if (CmiCreditConfigDefault->Valid == FALSE) {
    return;
  }

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Assume all channels have the same default value
    //
    MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N0_MC_2LM_REG, CmiCreditConfigDefault->CmiRdCreditConfigN0);
    MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N1_MC_2LM_REG, CmiCreditConfigDefault->CmiRdCreditConfigN1);
    MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N0_MC_2LM_REG, CmiCreditConfigDefault->CmiWrCreditConfigN0);
    MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N1_MC_2LM_REG, CmiCreditConfigDefault->CmiWrCreditConfigN1);
  }
}

/**
Mem Mc Get CMI Tables

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
MemMcGetCmiTableChannel(
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

#define MEMMC_GET_CMI_TABLE_CHANNEL(RegisterName)                                                              \
  MemMcGetCmiTableChannel (                                                                                    \
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
  Get MC CMI Credit Config N0 Value per Channel

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
EFIAPI
GetMcCmiCreditCfgN0ValueChannel (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
  MEMMC_GET_CMI_TABLE_CHANNEL (McCmiCreditCfgN0Value)
  return EFI_SUCCESS;
}

/**
  Get MC CMI Credit Config N1 Value per Channel

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcCmiCreditCfgN1ValueChannel (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
  MEMMC_GET_CMI_TABLE_CHANNEL (McCmiCreditCfgN1Value)
  return EFI_SUCCESS;
}

/**
  Get MC CMI Request Completion Config Value0 per Channel

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcCmiReqCpcfgConfigValue0Channel (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
  MEMMC_GET_CMI_TABLE_CHANNEL (McCmiReqCpcfgConfigValue0)
  return EFI_SUCCESS;
}

/**
  Get MC CMI Request Completion Config Value1 per Channel

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcCmiReqCpcfgConfigValue1Channel (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
  MEMMC_GET_CMI_TABLE_CHANNEL (McCmiReqCpcfgConfigValue1)
  return EFI_SUCCESS;
}

/**
  Get MC MISC CMI credit config Value per Channel

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcMiscCmiCreditConfigValueChannel (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
  MEMMC_GET_CMI_TABLE_CHANNEL (McMiscCmiCreditConfigValue)
  return EFI_SUCCESS;
}

/**
  Get McDp CMI Qdepth credit config Value per Channel
  Returns value of SPR

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcDpCmiQdepthCreditConfigValueChannel (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
  MEMMC_GET_CMI_TABLE_CHANNEL (McDpCmiQdepthCreditConfigValue)
  return EFI_SUCCESS;
}

/**
  Get Mc Tme Cfg Value  per Channel
  Returns value of SPR

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcTmeCfgValueChannel (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
  MEMMC_GET_CMI_TABLE_CHANNEL (Mc_tme_cfg)
  return EFI_SUCCESS;
}

/**
  Get Mc CmiStructWrCreditCfg N0 Value per Channel
  Returns value of SPR

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcCmiStructWrCreditCfgN0ValueChannel (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
  MEMMC_GET_CMI_TABLE_CHANNEL (Mccmistructwrcreditcfg_n0)
  return EFI_SUCCESS;
}

/**
  Get Mc CmiStructWrCreditCfg N1 Value per Channel
  Returns value of SPR

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcCmiStructWrCreditCfgN1ValueChannel (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
  MEMMC_GET_CMI_TABLE_CHANNEL (Mccmistructwrcreditcfg_n1)
  return EFI_SUCCESS;
}

/**
  Modify Mc CMI credits registers, chip specific

  @param[   out] McCmiCreditCfgN0Value        - McCmiCreditCfgN0 values pointer
  @param[   out] McCmiCreditCfgN1Value        - McCmiCreditCfgN1 values pointer
  @param[   out] McCmiReqCpcfgConfigValue0    - McCmiReqCpcfgConfig0 values pointer
  @param[   out] McCmiReqCpcfgConfigValue1    - McCmiReqCpcfgConfig1 values pointer
  @param[   out] McMiscCmiCreditConfigValue   - McMiscCmiCreditConfig values pointer
  @param[   out] McCmiCreditCfgN0Len          - McCmiCreditCfgN0 length pointer
  @param[   out] McCmiCreditCfgN1Len          - McCmiCreditCfgN1 length pointer
  @param[   out] McCmiReqCpcfgConfigValue0Len - McCmiReqCpcfgConfig0 length pointer
  @param[   out] McCmiReqCpcfgConfigValue1Len - McCmiReqCpcfgConfig1 length pointer
  @param[   out] McMiscCmiCreditConfigLen     - McMiscCmiCreditConfig length pointer

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
EFIAPI
ModifyMcCmiCreditProgrammingChip (
     OUT CMI_DEFAULT_REG_VALUE **McCmiCreditCfgN0Value,
     OUT CMI_DEFAULT_REG_VALUE **McCmiCreditCfgN1Value,
     OUT CMI_DEFAULT_REG_VALUE **McCmiReqCpcfgConfigValue0,
     OUT CMI_DEFAULT_REG_VALUE **McCmiReqCpcfgConfigValue1,
     OUT CMI_DEFAULT_REG_VALUE **McMiscCmiCreditConfigValue,
     OUT UINT8                 *McCmiCreditCfgN0Len,
     OUT UINT8                 *McCmiCreditCfgN1Len,
     OUT UINT8                 *McCmiReqCpcfgConfigValue0Len,
     OUT UINT8                 *McCmiReqCpcfgConfigValue1Len,
     OUT UINT8                 *McMiscCmiCreditConfigLen
  )
{
  return EFI_UNSUPPORTED;
} // ModifyMcCmiCreditProgrammingChip

/**
  Program McDp CMI Credit Registers

  @param[in] ProgramableValue  - Programmable CMI credit value
  @param[in] Socket            - Socket number
  @param[in] McId              - Mc Id number

  @retval N/A
**/
VOID
EFIAPI
ProgramMcDpCmiRegisters (
  IN UINT8 ProgramableValue[MAX_CH],
  IN UINT8 Socket,
  IN UINT8 McId
  )
{
  UINT8                                         Ch;
  UINT8                                         ChId;
  UINT8                                         IndexVal;
  UINT32                                        RegData;
  UINT8                                         NumChPerMc;

  CMI_DEFAULT_REG_VALUE                         *McDpCmiQdepthCreditConfigValue = NULL;
  UINT8                                         McDpCmiQdepthCreditConfigLen = 0;
  MCDP_CMI_QDEPTH_CFG_MCDDC_DP_STRUCT           McDpCmiQdepthCreditConfig[MAX_MC_CH];
  PSYSHOST                                      Host;

  Host = GetSysHostPointer ();
  NumChPerMc = GetNumChannelPerMc ();

  for (ChId = 0; ChId < NumChPerMc; ChId++) {
    Ch = (McId * NumChPerMc) + ChId;
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }
    GetMcDpCmiQdepthCreditConfigValueChannel (Socket, Ch, &McDpCmiQdepthCreditConfigValue ,&McDpCmiQdepthCreditConfigLen);
    if (McDpCmiQdepthCreditConfigValue != NULL) {
      McDpCmiQdepthCreditConfig[ChId].Data = MemReadPciCfgEp (Socket, Ch, MCDP_CMI_QDEPTH_CFG_MCDDC_DP_REG);
      for (IndexVal = 0; IndexVal < McDpCmiQdepthCreditConfigLen; IndexVal++) {
        RegData = (McDpCmiQdepthCreditConfig[ChId].Data >> McDpCmiQdepthCreditConfigValue[IndexVal].BitStart) & McDpCmiQdepthCreditConfigValue[IndexVal].AndVal;
        McDpCmiQdepthCreditConfig[ChId].Data &= ~(McDpCmiQdepthCreditConfigValue[IndexVal].AndVal << McDpCmiQdepthCreditConfigValue[IndexVal].BitStart);
        switch (ProgramableValue[Ch]) {
          case CMI_DDRT_VAL:
            RegData = McDpCmiQdepthCreditConfigValue[IndexVal].DdrtVal;
            break;
          case CMI_1LM_VAL:
            RegData = McDpCmiQdepthCreditConfigValue[IndexVal].OneLMVal;
            break;
          case CMI_CR_VAL:
            RegData = McDpCmiQdepthCreditConfigValue[IndexVal].CRVal;
            break;
          case CMI_1LM_ISOCH_VAL:
            RegData = McDpCmiQdepthCreditConfigValue[IndexVal].OneLMIsoChVal;
            break;
          case CMI_1LM_2WAY_VAL:
            RegData = McDpCmiQdepthCreditConfigValue[IndexVal].OneLM2WAYVal;
            break;
          case CMI_CR_2WAY_VAL:
            RegData = McDpCmiQdepthCreditConfigValue[IndexVal].CR2WAYVal;
            break;
          default:
            break;
        }
        McDpCmiQdepthCreditConfig[ChId].Data |= (RegData << McDpCmiQdepthCreditConfigValue[IndexVal].BitStart);
      }
      MemWritePciCfgEp (Socket, Ch, MCDP_CMI_QDEPTH_CFG_MCDDC_DP_REG, McDpCmiQdepthCreditConfig[ChId].Data);
    }
  }
}

/**
  Program McTmeCfg Registers

  @param[in] ProgramableValue  - Programmable CMI credit value
  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval N/A
**/
VOID
EFIAPI
ProgramMcTmeCfgRegisters (
  IN UINT8 ProgramableValue[MAX_CH],
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  UINT8                                         IndexVal;
  UINT32                                        RegData;

  CMI_DEFAULT_REG_VALUE                         *McTmeCfgValue = NULL;
  UINT8                                         McTmeCfgLen = 0;
  MC_TME_CFG_MC_2LM_STRUCT                      McTmeCfg;
  PSYSHOST                                      Host;

  Host = GetSysHostPointer ();

  GetMcTmeCfgValueChannel (Socket, Ch, &McTmeCfgValue ,&McTmeCfgLen);

  McTmeCfg.Data = MemReadPciCfgEp (Socket, Ch, MC_TME_CFG_MC_2LM_REG);
  for (IndexVal = 0; IndexVal < McTmeCfgLen; IndexVal++) {
    RegData = (McTmeCfg.Data >> McTmeCfgValue[IndexVal].BitStart) & McTmeCfgValue[IndexVal].AndVal;
    McTmeCfg.Data &= ~(McTmeCfgValue[IndexVal].AndVal << McTmeCfgValue[IndexVal].BitStart);
    switch (ProgramableValue[Ch]) {
      case CMI_DDRT_VAL:
        RegData = McTmeCfgValue[IndexVal].DdrtVal;
        break;
      case CMI_1LM_VAL:
        RegData = McTmeCfgValue[IndexVal].OneLMVal;
        break;
      case CMI_CR_VAL:
        RegData = McTmeCfgValue[IndexVal].CRVal;
        break;
      case CMI_1LM_ISOCH_VAL:
        RegData = McTmeCfgValue[IndexVal].OneLMIsoChVal;
        break;
      case CMI_1LM_2WAY_VAL:
        RegData = McTmeCfgValue[IndexVal].OneLM2WAYVal;
        break;
      case CMI_CR_2WAY_VAL:
        RegData = McTmeCfgValue[IndexVal].CR2WAYVal;
        break;
      default:
        break;
    }
    McTmeCfg.Data |= (RegData << McTmeCfgValue[IndexVal].BitStart);
  }
  MemWritePciCfgEp (Socket, Ch, MC_TME_CFG_MC_2LM_REG, McTmeCfg.Data);
}

/**
  Program McCmiStructWrCreditCfg Registers

  @param[in] ProgramableValue  - Programmable CMI credit value
  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval N/A
**/
VOID
EFIAPI
ProgramMcCmiStructWrCreditCfgRegisters (
  IN UINT8 ProgramableValue[MAX_CH],
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  UINT8                                         IndexVal;
  UINT32                                        RegData;

  CMI_DEFAULT_REG_VALUE                         *McCmiStructWrCreditCfgN0Value = NULL;
  CMI_DEFAULT_REG_VALUE                         *McCmiStructWrCreditCfgN1Value = NULL;
  UINT8                                         McCmiStructWrCreditCfgN0Len = 0;
  UINT8                                         McCmiStructWrCreditCfgN1Len = 0;
  MCCMISTRUCTWRCREDITCFG_N0_MC_2LM_STRUCT       McCmiStructWrCreditCfgN0;
  MCCMISTRUCTWRCREDITCFG_N1_MC_2LM_STRUCT       McCmiStructWrCreditCfgN1;
  PSYSHOST                                      Host;

  Host = GetSysHostPointer ();

  GetMcCmiStructWrCreditCfgN0ValueChannel (Socket, Ch, &McCmiStructWrCreditCfgN0Value ,&McCmiStructWrCreditCfgN0Len);
  GetMcCmiStructWrCreditCfgN1ValueChannel (Socket, Ch, &McCmiStructWrCreditCfgN1Value ,&McCmiStructWrCreditCfgN1Len);

  McCmiStructWrCreditCfgN0.Data = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N0_MC_2LM_REG);
  for (IndexVal = 0; IndexVal < McCmiStructWrCreditCfgN0Len; IndexVal++) {
    RegData = (McCmiStructWrCreditCfgN0.Data >> McCmiStructWrCreditCfgN0Value[IndexVal].BitStart) & McCmiStructWrCreditCfgN0Value[IndexVal].AndVal;
    McCmiStructWrCreditCfgN0.Data &= ~(McCmiStructWrCreditCfgN0Value[IndexVal].AndVal << McCmiStructWrCreditCfgN0Value[IndexVal].BitStart);
    switch (ProgramableValue[Ch]) {
      case CMI_DDRT_VAL:
        RegData = McCmiStructWrCreditCfgN0Value[IndexVal].DdrtVal;
        break;
      case CMI_1LM_VAL:
        RegData = McCmiStructWrCreditCfgN0Value[IndexVal].OneLMVal;
        break;
      case CMI_CR_VAL:
        RegData = McCmiStructWrCreditCfgN0Value[IndexVal].CRVal;
        break;
      case CMI_1LM_ISOCH_VAL:
        RegData = McCmiStructWrCreditCfgN0Value[IndexVal].OneLMIsoChVal;
        break;
      case CMI_1LM_2WAY_VAL:
        RegData = McCmiStructWrCreditCfgN0Value[IndexVal].OneLM2WAYVal;
        break;
      case CMI_CR_2WAY_VAL:
        RegData = McCmiStructWrCreditCfgN0Value[IndexVal].CR2WAYVal;
        break;
      default:
        break;
    }
    McCmiStructWrCreditCfgN0.Data |= (RegData << McCmiStructWrCreditCfgN0Value[IndexVal].BitStart);
  }
  MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N0_MC_2LM_REG, McCmiStructWrCreditCfgN0.Data);

  McCmiStructWrCreditCfgN1.Data = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N1_MC_2LM_REG);
  for (IndexVal = 0; IndexVal < McCmiStructWrCreditCfgN1Len; IndexVal++) {
    RegData = (McCmiStructWrCreditCfgN1.Data >> McCmiStructWrCreditCfgN1Value[IndexVal].BitStart) & McCmiStructWrCreditCfgN1Value[IndexVal].AndVal;
    McCmiStructWrCreditCfgN1.Data &= ~(McCmiStructWrCreditCfgN1Value[IndexVal].AndVal << McCmiStructWrCreditCfgN1Value[IndexVal].BitStart);
    switch (ProgramableValue[Ch]) {
      case CMI_DDRT_VAL:
        RegData = McCmiStructWrCreditCfgN1Value[IndexVal].DdrtVal;
        break;
      case CMI_1LM_VAL:
        RegData = McCmiStructWrCreditCfgN1Value[IndexVal].OneLMVal;
        break;
      case CMI_CR_VAL:
        RegData = McCmiStructWrCreditCfgN1Value[IndexVal].CRVal;
        break;
      case CMI_1LM_ISOCH_VAL:
        RegData = McCmiStructWrCreditCfgN1Value[IndexVal].OneLMIsoChVal;
        break;
      case CMI_1LM_2WAY_VAL:
        RegData = McCmiStructWrCreditCfgN1Value[IndexVal].OneLM2WAYVal;
        break;
      case CMI_CR_2WAY_VAL:
        RegData = McCmiStructWrCreditCfgN1Value[IndexVal].CR2WAYVal;
        break;
      default:
        break;
    }
    McCmiStructWrCreditCfgN1.Data |= (RegData << McCmiStructWrCreditCfgN1Value[IndexVal].BitStart);
  }
  MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N1_MC_2LM_REG, McCmiStructWrCreditCfgN1.Data);
}

/**

  Print Mem Mc CMI SpreadSheet Data Version

  @param[in] None

  @retVal    None
**/
VOID
EFIAPI
PrintMemMcCmiDataVersion (
  VOID
  )
{
  UINT8 CmiDataVersion = 0xFF;

  if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_C0)) {
    CmiDataVersion = MEMMC_CMI_PERFORMANCE_DATA_VERSION_C0;
  } else if (IsCpuAndRevision (CPU_SPRSP, REV_B0)) {
    CmiDataVersion = MEMMC_CMI_PERFORMANCE_DATA_VERSION_B0;
  } else {
    CmiDataVersion = MEMMC_CMI_PERFORMANCE_DATA_VERSION_A0;
  }
  RcDebugPrint (SDBG_MINMAX, "MemMc Cmi Data Version: %d\n", CmiDataVersion);
}

/**

  Program Mem Mc CMI registers

  @param[in] ProgramableValue[MAX_CH]    - Array containing type of value to program per channel (From 0 to Max Channel in Socket)
                                           1 = DDRT,2 = 1LM,3 = CR, 4 = 1LM ISOCH..CMI value
  @param[in] Socket                      - Socket number

  @retVal   1 CMI Registers initialized
            0 CMI Registers not initialized
**/
UINT32
EFIAPI
ProgramMemMcCmiRegisters (
  IN UINT8 ProgramableValue[MAX_CH],
  IN UINT8 Socket
  )
{
  UINT32                   Status                       = 0;
  CMI_DEFAULT_REG_VALUE    *McCmiCreditCfgN0Value       = NULL;
  CMI_DEFAULT_REG_VALUE    *McCmiCreditCfgN1Value       = NULL;
  CMI_DEFAULT_REG_VALUE    *McCmiReqCpcfgConfigValue0   = NULL;
  CMI_DEFAULT_REG_VALUE    *McCmiReqCpcfgConfigValue1   = NULL;
  CMI_DEFAULT_REG_VALUE    *McMiscCmiCreditConfigValue  = NULL;
  UINT8                    McCmiCreditCfgN0Len          = 0;
  UINT8                    McCmiCreditCfgN1Len          = 0;
  UINT8                    McCmiReqCpcfgConfigValue0Len = 0;
  UINT8                    McCmiReqCpcfgConfigValue1Len = 0;
  UINT8                    McMiscCmiCreditConfigLen     = 0;
  UINT32                   RegData                      = 0;
  UINT32                   VcFloor                      = 0;
  UINT32                   WpqFloor                     = 0;
  CMI_MAIN_REG_INFO        CmiMainRegInfo;
  UINT8                    McId;
  UINT8                    Ch;
  UINT8                    ChId;
  UINT8                    IndexVal;
  UINT8                    NumChPerMc;
  UINT8                    MaxImc;
  PSYSHOST                 Host;
  MCCMIREQCPCFG_N0_MC_2LM_STRUCT    McCmiReqCpcfgN0;

  PrintMemMcCmiDataVersion ();
  Host = GetSysHostPointer ();
  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();

  for (McId = 0; McId < MaxImc; McId++) {
    if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
      continue;
    }

    for (ChId = 0; ChId < NumChPerMc; ChId++) {
      Ch = (McId * NumChPerMc) + ChId;
      if (!IsChannelEnabled (Socket, Ch)) {
        continue;
      }

      GetMcCmiCreditCfgN0ValueChannel (Socket, Ch, &McCmiCreditCfgN0Value ,&McCmiCreditCfgN0Len);
      GetMcCmiCreditCfgN1ValueChannel (Socket, Ch, &McCmiCreditCfgN1Value ,&McCmiCreditCfgN1Len);
      GetMcCmiReqCpcfgConfigValue0Channel (Socket, Ch, &McCmiReqCpcfgConfigValue0 ,&McCmiReqCpcfgConfigValue0Len);
      GetMcCmiReqCpcfgConfigValue1Channel (Socket, Ch, &McCmiReqCpcfgConfigValue1 ,&McCmiReqCpcfgConfigValue1Len);
      GetMcMiscCmiCreditConfigValueChannel (Socket, Ch, &McMiscCmiCreditConfigValue ,&McMiscCmiCreditConfigLen);

      ModifyMcCmiCreditProgrammingChip (&McCmiCreditCfgN0Value,
                                        &McCmiCreditCfgN1Value,
                                        &McCmiReqCpcfgConfigValue0,
                                        &McCmiReqCpcfgConfigValue1,
                                        &McMiscCmiCreditConfigValue,
                                        &McCmiCreditCfgN0Len,
                                        &McCmiCreditCfgN1Len,
                                        &McCmiReqCpcfgConfigValue0Len,
                                        &McCmiReqCpcfgConfigValue1Len,
                                        &McMiscCmiCreditConfigLen);

      //
      // Restore the credit map registers to default values
      // These were originally modified before training by EarlyConfigMcCmi
      //
      RestoreCmiCreditConfigDefault (Host, Socket, Ch);
      CmiMainRegInfo.McCmiReqCpcfg.Data = MemReadPciCfgEp (Socket, Ch, MCCMIREQCPCFG_N0_MC_2LM_REG);
      for (IndexVal = 0; IndexVal < McCmiReqCpcfgConfigValue0Len; IndexVal++) {
        RegData = (CmiMainRegInfo.McCmiReqCpcfg.Data >> McCmiReqCpcfgConfigValue0[IndexVal].BitStart) & McCmiReqCpcfgConfigValue0[IndexVal].AndVal;
        CmiMainRegInfo.McCmiReqCpcfg.Data &= ~(McCmiReqCpcfgConfigValue0[IndexVal].AndVal << McCmiReqCpcfgConfigValue0[IndexVal].BitStart);
        switch (ProgramableValue[Ch]) {
          case CMI_DDRT_VAL:
            RegData = McCmiReqCpcfgConfigValue0[IndexVal].DdrtVal;
            break;
          case CMI_1LM_VAL:
            RegData = McCmiReqCpcfgConfigValue0[IndexVal].OneLMVal;
              break;
          case CMI_CR_VAL:
            RegData = McCmiReqCpcfgConfigValue0[IndexVal].CRVal;
            break;
          case CMI_1LM_ISOCH_VAL:
            RegData = McCmiReqCpcfgConfigValue0[IndexVal].OneLMIsoChVal;
            break;
          case CMI_1LM_2WAY_VAL:
            RegData = McCmiReqCpcfgConfigValue0[IndexVal].OneLM2WAYVal;
            break;
          case CMI_CR_2WAY_VAL:
            RegData = McCmiReqCpcfgConfigValue0[IndexVal].CR2WAYVal;
            break;
          default:
            break;
        }
        CmiMainRegInfo.McCmiReqCpcfg.Data |= (RegData << McCmiReqCpcfgConfigValue0[IndexVal].BitStart);
      }
      MemWritePciCfgEp (Socket, Ch, MCCMIREQCPCFG_N0_MC_2LM_REG, CmiMainRegInfo.McCmiReqCpcfg.Data);

      CmiMainRegInfo.McCmiReqCpcfg.Data = MemReadPciCfgEp (Socket, Ch, MCCMIREQCPCFG_N1_MC_2LM_REG);
      for (IndexVal = 0; IndexVal < McCmiReqCpcfgConfigValue1Len; IndexVal++) {
        RegData = (CmiMainRegInfo.McCmiReqCpcfg.Data >> McCmiReqCpcfgConfigValue1[IndexVal].BitStart) & McCmiReqCpcfgConfigValue1[IndexVal].AndVal;
        CmiMainRegInfo.McCmiReqCpcfg.Data &= ~(McCmiReqCpcfgConfigValue1[IndexVal].AndVal << McCmiReqCpcfgConfigValue1[IndexVal].BitStart);
        switch (ProgramableValue[Ch]) {
          case CMI_DDRT_VAL:
            RegData = McCmiReqCpcfgConfigValue1[IndexVal].DdrtVal;
            break;
          case CMI_1LM_VAL:
            RegData = McCmiReqCpcfgConfigValue1[IndexVal].OneLMVal;
              break;
          case CMI_CR_VAL:
            RegData = McCmiReqCpcfgConfigValue1[IndexVal].CRVal;
            break;
          case CMI_1LM_ISOCH_VAL:
            RegData = McCmiReqCpcfgConfigValue1[IndexVal].OneLMIsoChVal;
            break;
          case CMI_1LM_2WAY_VAL:
            RegData = McCmiReqCpcfgConfigValue1[IndexVal].OneLM2WAYVal;
            break;
          case CMI_CR_2WAY_VAL:
            RegData = McCmiReqCpcfgConfigValue1[IndexVal].CR2WAYVal;
            break;
          default:
            break;
        }
        CmiMainRegInfo.McCmiReqCpcfg.Data |= (RegData << McCmiReqCpcfgConfigValue1[IndexVal].BitStart);
      }

      McCmiReqCpcfgN0.Data = MemReadPciCfgEp (Socket, Ch, MCCMIREQCPCFG_N0_MC_2LM_REG);
      GetMinimumVcFloor (McCmiReqCpcfgN0.Bits.wr_vc0_floor, McCmiReqCpcfgN0.Bits.wr_vc1_floor, CmiMainRegInfo.McCmiReqCpcfg.Bits.wr_vc3_floor, &VcFloor);
      MemWritePciCfgEp (Socket, Ch, MCCMIREQCPCFG_N1_MC_2LM_REG, CmiMainRegInfo.McCmiReqCpcfg.Data);

      //
      //MCChan
      //
      CmiMainRegInfo.McCmiCreditCfgN0.Data = MemReadPciCfgEp (Socket, Ch, MCSTRUCTCMICREDITCFG_N0_MC_2LM_REG);
      for (IndexVal = 0; IndexVal < McCmiCreditCfgN0Len; IndexVal++) {
        RegData = (CmiMainRegInfo.McCmiCreditCfgN0.Data >> McCmiCreditCfgN0Value[IndexVal].BitStart) & McCmiCreditCfgN0Value[IndexVal].AndVal;
        CmiMainRegInfo.McCmiCreditCfgN0.Data &= ~(McCmiCreditCfgN0Value[IndexVal].AndVal << McCmiCreditCfgN0Value[IndexVal].BitStart);
        switch (ProgramableValue[Ch]) {
          case CMI_DDRT_VAL:
            RegData = McCmiCreditCfgN0Value[IndexVal].DdrtVal;
            break;
          case CMI_1LM_VAL:
            RegData = McCmiCreditCfgN0Value[IndexVal].OneLMVal;
            break;
          case CMI_CR_VAL:
            RegData = McCmiCreditCfgN0Value[IndexVal].CRVal;
            break;
          case CMI_1LM_ISOCH_VAL:
            RegData = McCmiCreditCfgN0Value[IndexVal].OneLMIsoChVal;
            break;
          case CMI_1LM_2WAY_VAL:
            RegData = McCmiCreditCfgN0Value[IndexVal].OneLM2WAYVal;
            break;
          case CMI_CR_2WAY_VAL:
            RegData = McCmiCreditCfgN0Value[IndexVal].CR2WAYVal;
            break;
          default:
            break;
        }
        CmiMainRegInfo.McCmiCreditCfgN0.Data |= (RegData << McCmiCreditCfgN0Value[IndexVal].BitStart);
      }
      WpqFloor = MIN (CmiMainRegInfo.McCmiCreditCfgN0.Bits.wpq4_0_shared_cnt, CmiMainRegInfo.McCmiCreditCfgN0.Bits.wpq4_1_shared_cnt);
      MemWritePciCfgEp (Socket, Ch, MCSTRUCTCMICREDITCFG_N0_MC_2LM_REG, CmiMainRegInfo.McCmiCreditCfgN0.Data);

      CmiMainRegInfo.McCmiCreditCfgN1.Data = MemReadPciCfgEp (Socket, Ch, MCSTRUCTCMICREDITCFG_N1_MC_2LM_REG);
      for (IndexVal = 0; IndexVal < McCmiCreditCfgN1Len; IndexVal++) {
        RegData = (CmiMainRegInfo.McCmiCreditCfgN1.Data >> McCmiCreditCfgN1Value[IndexVal].BitStart) & McCmiCreditCfgN1Value[IndexVal].AndVal;
        CmiMainRegInfo.McCmiCreditCfgN1.Data &= ~(McCmiCreditCfgN1Value[IndexVal].AndVal << McCmiCreditCfgN1Value[IndexVal].BitStart);
        switch (ProgramableValue[Ch]) {
          case CMI_DDRT_VAL:
            RegData = McCmiCreditCfgN1Value[IndexVal].DdrtVal;
            break;
          case CMI_1LM_VAL:
            RegData = McCmiCreditCfgN1Value[IndexVal].OneLMVal;
            break;
          case CMI_CR_VAL:
            RegData = McCmiCreditCfgN1Value[IndexVal].CRVal;
            break;
          case CMI_1LM_ISOCH_VAL:
            RegData = McCmiCreditCfgN1Value[IndexVal].OneLMIsoChVal;
            break;
          case CMI_1LM_2WAY_VAL:
            RegData = McCmiCreditCfgN1Value[IndexVal].OneLM2WAYVal;
            break;
          case CMI_CR_2WAY_VAL:
            RegData = McCmiCreditCfgN1Value[IndexVal].CR2WAYVal;
            break;
          default:
            break;
        }
        CmiMainRegInfo.McCmiCreditCfgN1.Data |= (RegData << McCmiCreditCfgN1Value[IndexVal].BitStart);
      }
      MemWritePciCfgEp (Socket, Ch, MCSTRUCTCMICREDITCFG_N1_MC_2LM_REG, CmiMainRegInfo.McCmiCreditCfgN1.Data);
      ProgramWPQThershold (Host, Socket, Ch, (WpqFloor + VcFloor));

      ProgramMcTmeCfgRegisters (ProgramableValue, Socket, Ch);
      ProgramMcCmiStructWrCreditCfgRegisters (ProgramableValue, Socket, Ch);

      if (McMiscCmiCreditConfigValue != NULL) {
        CmiMainRegInfo.McMiscCmiCreditConfig[ChId].Data = MemReadPciCfgEp (Socket, Ch, McMiscCmiCreditConfigRegAddr[ChId]);
        for (IndexVal = 0; IndexVal < McMiscCmiCreditConfigLen; IndexVal++) {
         RegData = (CmiMainRegInfo.McMiscCmiCreditConfig[ChId].Data >> McMiscCmiCreditConfigValue[IndexVal].BitStart) & McMiscCmiCreditConfigValue[IndexVal].AndVal;
         CmiMainRegInfo.McMiscCmiCreditConfig[ChId].Data &= ~(McMiscCmiCreditConfigValue[IndexVal].AndVal << McMiscCmiCreditConfigValue[IndexVal].BitStart);
         switch (ProgramableValue[Ch]) {
           case CMI_DDRT_VAL:
             RegData = McMiscCmiCreditConfigValue[IndexVal].DdrtVal;
             break;
           case CMI_1LM_VAL:
             RegData = McMiscCmiCreditConfigValue[IndexVal].OneLMVal;
             break;
           case CMI_CR_VAL:
             RegData = McMiscCmiCreditConfigValue[IndexVal].CRVal;
             break;
           case CMI_1LM_ISOCH_VAL:
             RegData = McMiscCmiCreditConfigValue[IndexVal].OneLMIsoChVal;
             break;
           case CMI_1LM_2WAY_VAL:
             RegData = McMiscCmiCreditConfigValue[IndexVal].OneLM2WAYVal;
             break;
           case CMI_CR_2WAY_VAL:
             RegData = McMiscCmiCreditConfigValue[IndexVal].CR2WAYVal;
             break;
           default:
             break;
         }
         CmiMainRegInfo.McMiscCmiCreditConfig[ChId].Data |= (RegData << McMiscCmiCreditConfigValue[IndexVal].BitStart);
        }
        MemWritePciCfgEp (Socket, Ch, McMiscCmiCreditConfigRegAddr[ChId], CmiMainRegInfo.McMiscCmiCreditConfig[ChId].Data);
      }
    }
    ProgramMcDpCmiRegisters (ProgramableValue, Socket, McId);
  }

  return Status;
}

/**
  Early config on McDp CMI Credit to enable CPGC for memory training

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If Mc doesn't require early config of CMI credit for CPGC
          EFI_SUCCESS            If Mc requires early config of CMI credit  for CPGC
**/
EFI_STATUS
EFIAPI
EarlyConfigMcCmi (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
#if MC_IP_VER >= MEM_IP_19ww22g
  MCCMISTRUCTRDCREDITCFG_N0_MC_2LM_STRUCT  McCmiStructRdCreditCfgN0;
  MCCMISTRUCTRDCREDITCFG_N1_MC_2LM_STRUCT  McCmiStructRdCreditCfgN1;
  MCCMISTRUCTWRCREDITCFG_N0_MC_2LM_STRUCT  McCmiStructWrCreditCfgN0;
  MCCMISTRUCTWRCREDITCFG_N1_MC_2LM_STRUCT  McCmiStructWrCreditCfgN1;
#endif
  MCCMIREQCPCFG_N0_MC_2LM_STRUCT           McCmiReqCpcfgN0;
  MCCMIREQCPCFG_N1_MC_2LM_STRUCT           McCmiReqCpcfgN1;
  MCSTRUCTCMICREDITCFG_N0_MC_2LM_STRUCT    McStructCmiCreditCfgN0;
  MCSTRUCTCMICREDITCFG_N1_MC_2LM_STRUCT    McStructCmiCreditCfgN1;
  MCDP_CMI_CFG_MCDDC_DP_STRUCT             McDpCmiCfg;
  PSYSHOST                                 Host;

  Host = GetSysHostPointer ();

  McCmiReqCpcfgN0.Data = MemReadPciCfgEp (Socket, Ch, MCCMIREQCPCFG_N0_MC_2LM_REG);
  McCmiReqCpcfgN1.Data = MemReadPciCfgEp (Socket, Ch, MCCMIREQCPCFG_N1_MC_2LM_REG);
  McCmiReqCpcfgN0.Bits.rd_vc0_floor = 0x10;
  McCmiReqCpcfgN0.Bits.rd_vc1_floor = 0x0;
  McCmiReqCpcfgN0.Bits.rd_vc2_floor = 0x0;
  McCmiReqCpcfgN0.Bits.rd_vc3_floor = 0x0;
  McCmiReqCpcfgN0.Bits.wr_vc0_floor = 0x8;
  McCmiReqCpcfgN0.Bits.wr_vc1_floor = 0x0;
  McCmiReqCpcfgN0.Bits.wr_vc2_floor = 0x0;
  McCmiReqCpcfgN1.Bits.wr_vc2_floor = 0x0;
  McCmiReqCpcfgN1.Bits.wr_vc3_floor = 0x0;
  McCmiReqCpcfgN1.Bits.req_fab_credits = 0x6;
  MemWritePciCfgEp (Socket, Ch, MCCMIREQCPCFG_N0_MC_2LM_REG, McCmiReqCpcfgN0.Data);
  MemWritePciCfgEp (Socket, Ch, MCCMIREQCPCFG_N1_MC_2LM_REG, McCmiReqCpcfgN1.Data);

#if MC_IP_VER >= MEM_IP_19ww22g
  McCmiStructRdCreditCfgN0.Data = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N0_MC_2LM_REG);
  McCmiStructRdCreditCfgN1.Data = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N1_MC_2LM_REG);
  McCmiStructRdCreditCfgN0.Bits.vc0_struct_credit_map = 0x13;
  McCmiStructRdCreditCfgN0.Bits.vc1_struct_credit_map = 0x0;
  McCmiStructRdCreditCfgN0.Bits.vc2_struct_credit_map = 0x0;
  McCmiStructRdCreditCfgN1.Bits.vc2_struct_credit_map = 0x0;
  McCmiStructRdCreditCfgN1.Bits.vc3_struct_credit_map = 0x0;
  MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N0_MC_2LM_REG, McCmiStructRdCreditCfgN0.Data);
  MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N1_MC_2LM_REG, McCmiStructRdCreditCfgN1.Data);

  McCmiStructWrCreditCfgN0.Data = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N0_MC_2LM_REG);
  McCmiStructWrCreditCfgN1.Data = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N1_MC_2LM_REG);
  McCmiStructWrCreditCfgN0.Bits.vc0_struct_credit_map = 0x23;
  McCmiStructWrCreditCfgN0.Bits.vc1_struct_credit_map = 0x0;
  McCmiStructWrCreditCfgN0.Bits.vc2_struct_credit_map = 0x0;
  McCmiStructWrCreditCfgN1.Bits.vc2_struct_credit_map = 0x0;
  McCmiStructWrCreditCfgN1.Bits.vc3_struct_credit_map = 0x0;
  MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N0_MC_2LM_REG, McCmiStructWrCreditCfgN0.Data);
  MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N1_MC_2LM_REG, McCmiStructWrCreditCfgN1.Data);
#endif

  McStructCmiCreditCfgN0.Data = MemReadPciCfgEp (Socket, Ch, MCSTRUCTCMICREDITCFG_N0_MC_2LM_REG);
  McStructCmiCreditCfgN1.Data = MemReadPciCfgEp (Socket, Ch, MCSTRUCTCMICREDITCFG_N1_MC_2LM_REG);
  McStructCmiCreditCfgN0.Bits.rpq4_0_shared_cnt = 0x8;
  McStructCmiCreditCfgN0.Bits.rpq4_1_shared_cnt = 0x8;
  McStructCmiCreditCfgN0.Bits.wpq4_0_shared_cnt = 0x1;
  McStructCmiCreditCfgN0.Bits.wpq4_1_shared_cnt = 0x1;
  McStructCmiCreditCfgN0.Bits.rpqt_shared_cnt = 0x40;
  McStructCmiCreditCfgN0.Bits.wpqt_shared_cnt = 0x8;
  McStructCmiCreditCfgN1.Bits.wpqt_shared_cnt = 0x8;
  McStructCmiCreditCfgN1.Bits.wdb_shared_cnt = 0x37;
  McStructCmiCreditCfgN1.Bits.sb_shared_cnt = 0x86;
  McStructCmiCreditCfgN1.Bits.early_rsp_fifo_0_shared_cnt = 0x1;
  McStructCmiCreditCfgN1.Bits.early_rsp_fifo_1_shared_cnt = 0x5;
  McStructCmiCreditCfgN1.Bits.early_rsp_fifo_2_shared_cnt = 0x1;
  McStructCmiCreditCfgN1.Bits.early_rsp_fifo_3_shared_cnt = 0x3;
  MemWritePciCfgEp (Socket, Ch, MCSTRUCTCMICREDITCFG_N0_MC_2LM_REG, McStructCmiCreditCfgN0.Data);
  MemWritePciCfgEp (Socket, Ch, MCSTRUCTCMICREDITCFG_N1_MC_2LM_REG, McStructCmiCreditCfgN1.Data);

  McDpCmiCfg.Data = MemReadPciCfgEp (Socket, Ch, MCDP_CMI_CFG_MCDDC_DP_REG);
  McDpCmiCfg.Bits.rd_cpl_data_ecc_vld = 0x0;
  McDpCmiCfg.Bits.rd_cpl_data_parity_vld = 0x1;
  McDpCmiCfg.Bits.rsp_overflow_err_enable = 0x1;
  McDpCmiCfg.Bits.rsp_underflow_err_enable = 0x0;
  McDpCmiCfg.Bits.rd_cpl_overflow_err_enable = 0x0;
  McDpCmiCfg.Bits.rd_cpl_underflow_err_enable = 0x1;
  MemWritePciCfgEp (Socket, Ch, MCDP_CMI_CFG_MCDDC_DP_REG, McDpCmiCfg.Data);

  return EFI_SUCCESS;
}

/**
  Polling whether or not CPGC hardware has completed request credit exchange

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If Mc doesn't support this
          EFI_TIMEOUT            Polling timeout
          EFI_SUCCESS            Otherwise
**/
EFI_STATUS
EFIAPI
PollCpgcReqCreditsInitialized (
  IN UINT8 Socket,
  IN UINT8 Ch
)
{
  EFI_STATUS                               Status = EFI_SUCCESS;
  PSYSHOST                                 Host;
  MCCMIREQCPCFG_N1_MC_2LM_STRUCT           McCmiReqCpcfgN1;
  UINT32                                   InMcUlt = 0;
  BOOLEAN                                  BreakEarly = FALSE;
  UINT64                                   StartCount;

  Host = GetSysHostPointer ();

  Status = CteGetOptions ("mc_ult", &(InMcUlt));
  if ((Status != EFI_SUCCESS) && (Status != EFI_UNSUPPORTED)) {
    RcDebugPrint (SDBG_DEFAULT, "PollCpgcReqCreditsInitialized: Failed CteGetOptions for mc_ult\n");
  } else if ((Status == EFI_SUCCESS) && (InMcUlt == 1)) {
    BreakEarly = TRUE;
  }


  StartCount = GetCountTsc ();

  do {
    if (BreakEarly) {
      break;
    }

    McCmiReqCpcfgN1.Data = MemReadPciCfgEp (Socket, Ch, MCCMIREQCPCFG_N1_MC_2LM_REG);

    if (GetDelayTsc (StartCount) > CATCHALL_TIMEOUT) {
        Status = EFI_TIMEOUT;
        RcDebugPrint (SDBG_DEFAULT, "PollCpgcReqCreditsInitialized: Timeout\n");
        break;
    }

    // If this is a CTE build, delay between reads so we don't poll so often.
    CteDelayQclk (50);
  } while (McCmiReqCpcfgN1.Bits.cpgc_req_credits_initialized != 1);

  return Status;
}

/**
  Initializes the MC Req Credits Cfg register

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If Mc doesn't support this
          EFI_SUCCESS            Otherwise
**/
VOID
InitMcCmiReqCpCfg (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  MCCMIREQCPCFG_N1_MC_2LM_STRUCT           McCmiReqCpcfgN1;
  PSYSHOST                                 Host;

  Host = GetSysHostPointer();

  McCmiReqCpcfgN1.Data = MemReadPciCfgEp (Socket, Ch, MCCMIREQCPCFG_N1_MC_2LM_REG);
  McCmiReqCpcfgN1.Bits.req_credit_cfg_done            = 0;
  McCmiReqCpcfgN1.Bits.cpgc_req_credits_initialized   = 0;
  MemWritePciCfgEp (Socket, Ch, MCCMIREQCPCFG_N1_MC_2LM_REG, McCmiReqCpcfgN1.Data);

}

/**
  Set Req Credit Cfg Done bit to indicate Bios has programmed the desired credit count values

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If Mc doesn't support this
          EFI_SUCCESS            Otherwise
**/
EFI_STATUS
EFIAPI
SetMcReqCreditCfgDone (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  MCCMIREQCPCFG_N1_MC_2LM_STRUCT           McCmiReqCpcfgN1;
  PSYSHOST                                 Host;

  Host = GetSysHostPointer();

  McCmiReqCpcfgN1.Data = MemReadPciCfgEp (Socket, Ch, MCCMIREQCPCFG_N1_MC_2LM_REG);
  McCmiReqCpcfgN1.Bits.req_credit_cfg_done = 1;
  MemWritePciCfgEp (Socket, Ch, MCCMIREQCPCFG_N1_MC_2LM_REG, McCmiReqCpcfgN1.Data);

  return EFI_SUCCESS;
}

