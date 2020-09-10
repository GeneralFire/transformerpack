/** @file
  Interface source file for the HBM library class.

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

#include <Library/MemoryCoreLib.h>
#include <Library/MemCpgcIpLib.h>
#include "Include/CpgcChip.h"
#include <Library/SysHostPointerLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/CteNetLib.h>
#include <Library/MemCpgcIpTargetLib.h>
#include <Library/CsrAccessLib.h>

/**

  Configure the CPGC Start/Wrap addresses

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                AddressSettings     Pointer to address settings
  @param[in]                ScaleRateSettings   Pointer to scale, rate and order settings

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
SetupAddress (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        ADDRESS_SETTINGS    *AddressSettings,
  IN        SCALE_RATE_SETTINGS *ScaleRateSettings
  )
{
  return EFI_SUCCESS;
} // SetupAddress

/**
  Setups the CADB Write Pointer

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Value               Value to setup the CADB write pointer

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
SetupCadbWritePointer (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT32          Value
  )
{

  return EFI_SUCCESS;
}

/**
  Appends a CADB line

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                CadbPattern         Value to setup the CADB write pointer

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
AppendCadbLine (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        CADB_CMD_PATTERN  CadbPattern
  )
{
  return EFI_SUCCESS;
}

/**
  Enables or disables CADB mode

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Enable              Enable(TRUE) / Disable(FALSE) CADB mode

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
EnableCadb (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        BOOLEAN         Enable
  )
{

  return EFI_SUCCESS;
}

/**
  Get errors from CPGC registers

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Strobe              Current Strobe (0-Based)
  @param[out]               CpgcErrorStatus     Pointer to Cpgc error Status per channel/strobe

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
  @retval                   EFI_UNSUPPORTED     Strobe index is not supported

**/
EFI_STATUS
EFIAPI
GetCpgcErrors (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           Strobe,
      OUT   CPGC_ERRORS     *CpgcErrorStatus
  )
{
  return EFI_SUCCESS;
}

/**
  Clear errors from CPGC registers

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
ClearCpgcErrors (
  IN        UINT8           Socket,
  IN        UINT8           Channel
  )
{
  return EFI_SUCCESS;
}

/**
  Setup the error mask in the CPGC registers

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Strobe              Current Strobe (0-Based)
  @param[in]                Mask                Bits to be masked

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
  @retval                   EFI_UNSUPPORTED     Strobe index is not supported

**/
EFI_STATUS
EFIAPI
SetupErrorMask (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           Strobe,
  IN        UINT32          Mask
  )
{

  return EFI_SUCCESS;
}

/**
  Mask ECC errors

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Enable              Enable(TRUE) / Disable(FALSE) CADB mode

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
MaskEccErrors (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        BOOLEAN         EnableMask
  )
{

  return EFI_SUCCESS;
}

/**
  Get or set Cpgc BL4

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                GetSet              Specify (GET or SET)_CPGC_LIB command
  @param[in,out]            Global              Pointer to Get/Set value for global features

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
GetSetCpgcBl4 (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           GetSet,
  IN  OUT   GLOBAL_FEATURE  *Global
  )
{

  return EFI_SUCCESS;
}

/**
  Get or set cpgc status (start test and clear errors)

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                GetSet              Specify (GET or SET)_CPGC_LIB command
  @param[in,out]            Global              Pointer to Get/Set value for global features

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
GetSetCpgcTestStatus (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           GetSet,
  IN  OUT   GLOBAL_FEATURE  *Global
  )
{

  return EFI_SUCCESS;
}

/**
  Stops any cpgc test

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
StopCpgcTests (
  IN        UINT8           Socket,
  IN        UINT8           Channel
  )
{

  return EFI_SUCCESS;

}
/**
  Get all the test done available bits

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in,out]            Global              Pointer to Get/Set value for global features

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
GetCpgcTestDone (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN  OUT   GLOBAL_FEATURE  *Global
  )
{

  return EFI_SUCCESS;
}


/**
  Setup Subsequence

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                SubsequenceFeatures Pointer to Subsequence feature
  @param[in]                SubsequenceOffset   Offset for specified subsequence

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/

EFI_STATUS
SetupSubsequence (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        SUBSEQUENCE_FEATURE *SubsequenceFeatures,
  IN        UINT32          SubsequenceOffset
  )
{
  return EFI_SUCCESS;
}

/**
  Setup Subsequence 0

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                SubsequenceFeatures Pointer to Subsequence feature

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
SetupSubsequence0 (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        SUBSEQUENCE_FEATURE *SubsequenceFeatures
  )
{
  return EFI_SUCCESS;
}

/**
  Setup Subsequence 1

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                SubsequenceFeatures Pointer to Subsequence feature

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
SetupSubsequence1 (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        SUBSEQUENCE_FEATURE *SubsequenceFeatures
  )
{
   return EFI_SUCCESS;
}

/**
  Setup Loopcount and Update Rate Mode

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                SequenceGlobalFeatures Pointer to Subsequence feature

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
SetupLoopCount (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        SUBSEQUENCE_CHANNEL_FEATURE *SequenceChannelFeatures
  )
{
  return EFI_SUCCESS;
}

/**
  Enable / Disable CPGC in Native Mode

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                MemSs               MC ID            
  @param[in]                Enable              Enable(TRUE) / Disable(FALSE) to operate in native mode

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
EnableCpgcInNativeMode (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           MemSs,
  IN        BOOLEAN         Enable
  )
{
  SetCpgcTrainMode (Socket, MemSs, Channel, Enable);
  
  return EFI_SUCCESS;
}


/**
  Enable / Disable Global Support

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Enable              Enable(TRUE) / Disable(FALSE) support for global execution

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
EnableGlobalSupport (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        BOOLEAN         Enable
  )
{

  return EFI_SUCCESS;
}

/**
  Setup channel sequence configuration

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                SequenceChannelFeatures Pointer to Subsequence per channel features

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
SetupChannelSequence (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        SUBSEQUENCE_CHANNEL_FEATURE *SequenceChannelFeatures
  )
{

  return EFI_SUCCESS;
}

/**
  Configure patterns, pattern type and others

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                PatternBuffers      Pointer to Pattern Buffer

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
SetupPatternWdbMux (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        PATTERN_BUFFERS *PatternBuffers
  )
{

  return EFI_SUCCESS;
}

/**

  Configures the WDB Rd/Wr pointer and Write pointers in fast Mode

  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                WdbRdWrPointer      Pointer to pointer and subpointers
  @param[in]                Hi                  Hi 32 bits
  @param[in]                Lo                  Low 32 bits

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
FastProgramWdb (
  IN        UINT8           Channel,
  IN        RDWR_POINTER    *WdbRdWrPointer,
  IN        UINT32          Hi,
  IN        UINT32          Lo
  )
{
  FastProgramWdbTarget (Channel, WdbRdWrPointer, Hi, Lo);

  return EFI_SUCCESS;
}

/**

  Configures the WDB Rd/Wr pointer

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                WdbRdWrPointer      Pointer to pointer and subpointers

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
WdbRdWrPointer (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        RDWR_POINTER    *WdbRdWrPointer
  )
{

  return EFI_SUCCESS;
}


/**

  Configures the WDB Write buffers

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Lo                  Low 32 bits
  @param[in]                Hi                  Hi 32 bits

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
WdbBuffer (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT32          Lo,
  IN        UINT32          Hi
  )
{

  return EFI_SUCCESS;
}

/**

  Configures the rank logical mapping

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                RankLogicalMapping  Pointer to Rank logical mapping

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
SetupRankLogicalMap (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        RANK_LOGICAL_MAPPING    *RankLogicalMapping
  )
{

  return EFI_SUCCESS;
}


/**

  Configures the error logging registers

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                ErrorLoggingFeature Pointer to Error logging features

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
SetupErrorLogging (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        ERROR_LOGGING_FEATURE    *ErrorLoggingFeature
  )
{

  return EFI_SUCCESS;
}


/**

  Configures Misc Cpgc options

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                RefMask             Refresh Mask
  @param[in]                ZqMask              Zq Calibration Mask

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
SetupMiscCpgc (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           RefMask,
  IN        UINT8           ZqMask
  )
{

  return EFI_SUCCESS;
}

/**

  Configures HBM Misc Cpgc options

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
SetupHbmMiscCpgc (
  IN        UINT8           Socket,
  IN        UINT8           Channel
  )
{

  return EFI_SUCCESS;
}