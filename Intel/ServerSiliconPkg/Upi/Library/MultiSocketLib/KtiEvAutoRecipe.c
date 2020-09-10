/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2020 Intel Corporation. <BR>

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
#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>
#include <KtiMisc.h>
#include <Chip/Include/EvAutoRecipe.h>
#include <UncoreCommonIncludes.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/CpuAndRevisionLib.h>


/**
  Check if the CPU type in EV recipe row is valid.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param ProcessorTypeMask - Supported processor type bitmask from EV recipe row

  @retval TRUE/FALSE

**/
BOOLEAN
KtiIsValidCpuType (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT64                     ProcessorTypeMask
  )
{
  //
  // Note: We don't have a wild card for cpuType in the automation
  //
  if (IsSkxFamilyCpu (KtiInternalGlobal->CpuType) && ProcessorTypeMask == SKX_SOCKET) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Check if the KTI Port index in EV recipe row is valid.

  @param PortId       - Index of the KTI port being programmed
  @param PortListMask - KTI port bitmask from EV recipe row

  @retval TRUE/FALSE

**/
BOOLEAN
KtiIsValidPort (
  UINT8  PortId,
  UINT32 PortListMask
  )
{
    if (PortListMask == WILDCARD_32) {
      //
      // When mask is 0xFFFF_FFFF, return TRUE irrespective of current CPU
      //
      return TRUE;
    }

    if (PortListMask & (1 << PortId)) {
      return TRUE;
    } else {
      return FALSE;
    }
}

/**
  Apply UniPhy recipe overrides, if necessary.

  @param Soc  - Socket ID being programmed
  @param Link - KTI port being programmed
  @param Type - Register access type
  @param Addr - Register address
  @param Mask - Register bitfield mask
  @param Data - Value to program into the register

  @retval TRUE  - Register override successfully performed
  @retval FALSE - No need to override the register/override failed

**/
BOOLEAN
OverrideUniphyValues (
  UINT8          Soc,
  UINT8          Link,
  UINT8          Type,
  UINT32         Addr,
  UINT32         Mask,
  UINT32         *Data
  )
{
  return FALSE;
}

/**
  Check if the Config index in EV recipe row is valid.
  Current supported configs are KTI_CFG, FPGA_CFG, and WILDCARD_16

  @param KtiInternalGlobal - KTIRC internal variables.
  @param Socket     - Socket ID of register to be programmed
  @param PortId       - Index of the KTI port being programmed
  @param Config     - Config value from KTI EV recipe (WILDCARD_16, KTI_CFG, or FPGA_CFG)

  @retval TRUE/FALSE
**/
BOOLEAN
KtiIsValidConfig (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8  Socket,
  UINT8  PortId,
  UINT16 Config
  )
{
  if (Config == WILDCARD_16) {
    //Always program WILDCARD_16 configs
    return TRUE;
  } else if (Config == (1 << KTI_CFG)) {
    //If the part is NON-FPGA, recognize KTI_CFG
    if (KTIVAR->OutKtiFpgaPresent[Socket] == 0) {
      return TRUE;
    //If the part is FPGA && not the LINK connect to FPGA, recognize KTI_CFG
    } else if (PortId != XEON_FPGA_PORT) {
      return TRUE;
    }
  } else if (Config == (1 << FPGA_CFG)) {
    //If the part is an FPGA SKU, recognize FPGA_CFG
    if (KTIVAR->OutKtiFpgaPresent[Socket] == 1 && PortId == XEON_FPGA_PORT) {
      return TRUE;
    }
  } else if (Config == (1 << CPX4_CFG)) {
    if (IsInternalUpiPort (KtiInternalGlobal, Socket, PortId)) {
      return TRUE;
    }
  }

  //unrecognized config
  return FALSE;
}
