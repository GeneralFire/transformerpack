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

#ifndef _PLATFORM_INFO_TYPES_H_
#define _PLATFORM_INFO_TYPES_H_

//
// DIMM Connector type
//
typedef enum {
  DimmConnectorPth = 0x00, // Through hole connector
  DimmConnectorSmt,        // Surface mount connector
  DimmConnectorMemoryDown, // Platform soldered DRAMs
  DimmConnectorIgnore,     // Ignore connector type
  DimmConnectorMax
} EFI_MEMORY_DIMM_CONNECTOR_TYPE;

//
// Platform types - used with EFI_PLATFORM_INFO BoardId
//
typedef enum {
  StartOfEfiPlatformTypeEnum = 0x00,
  //For PPO
  TypeNeonCityEPRP,
  TypeWolfPass,
  TypeTennesseePass,
  TypeHedtCRB,
  TypeLightningRidgeEXRP,
  TypeLightningRidgeEX8S1N,
  TypeBarkPeak,
  TypeYubaCityRP,
  TypeRidgeport,
  //End PPO
  TypeWilsonCityRP,
  TypeWilsonCityModular,
  TypeCoyotePass,
  TypeIdaville,
  TypeMoroCityRP,
  TypeBrightonCityRp,
  TypeJacobsville,
  TypeSnrSvp,
  TypeSnrSvpSodimm,
  TypeJacobsvilleMDV,
  TypeFrostCreekRP,
  TypeVictoriaCanyonRP,
  TypeArcherCityRP,
  TypeNeonCityEPECB,
  TypeIsoscelesPeak,
  TypeWilsonPointRP,
  TypeWilsonPointModular,
  TypeBretonSound,
  TypeWilsonCityPPV,
  TypeCooperCityRP,
  TypeWilsonCitySMT,
  TypeSnrSvpSodimmB,
  TypeArcherCityModular,
  TypeArcherCityEVB,
  TypeArcherCityXPV,
  TypeBigPineKey,
  TypeExperWorkStationRP,
  EndOfEfiPlatformTypeEnum
} EFI_PLATFORM_TYPE;

#define TypePlatformUnknown       0xFF
#define TypePlatformMin           StartOfEfiPlatformTypeEnum + 1
#define TypePlatformMax           EndOfEfiPlatformTypeEnum - 1
#define TypePlatformDefault       TypeWilsonPointRP

//
// CPU type: Standard (no MCP), -F, etc
//
typedef enum {
  CPU_TYPE_STD,
  CPU_TYPE_F,
  CPU_TYPE_P,
  CPU_TYPE_MAX
} CPU_TYPE;

#define CPU_TYPE_STD_MASK (1 << CPU_TYPE_STD)
#define CPU_TYPE_F_MASK   (1 << CPU_TYPE_F)
#define CPU_TYPE_P_MASK   (1 << CPU_TYPE_P)

typedef enum {
  DaisyChainTopology = 0x00,
  InvSlotsDaisyChainTopology,
  TTopology
} EFI_MEMORY_TOPOLOGY_TYPE;

//
// Values for SocketConfig
//

#define SOCKET_UNDEFINED  0
#define SOCKET_4S         1
#define SOCKET_HEDT       2
#define SOCKET_1S         3
#define SOCKET_1SWS       4
#define SOCKET_8S         5
#define SOCKET_2S         6

#endif // #ifndef _PLATFORM_INFO_TYPES_H_
