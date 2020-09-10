/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2016 Intel Corporation. <BR>

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

#ifndef _BL_PLATFORM_DATA_H
#define _BL_PLATFORM_DATA_H

//
// @note
// All lines between tag EXTERNAL_BOOTLOADER_STRUCT_BEGIN and
// EXTERNAL_BOOTLOADER_STRUCT_END will be exported to the generated
// FspUpdVpd.h file by the tool.
//

//
// @note
// Make sure all the ME structures are identical with the ones in ME directory.
//

#pragma pack(push, 1)

/* !EXPORT EXTERNAL_BOOTLOADER_STRUCT_BEGIN */
#define MAX_CHAN 2            /* Maximum Number of Memory Channels */
#define MAX_DIMM 2          /* Maximum Number of DIMMs PER Memory Channel */

/* PCIe port bifurcation codes - matches setup option values */
#define BIFURCATION_x2x2x2x2             0
#define BIFURCATION_x2x2x4               1
#define BIFURCATION_x4x2x2               2
#define BIFURCATION_x4x4                 3
#define BIFURCATION_x8                   4

/*
 * Memory Down structures.
 */
typedef enum {
	STATE_MEMORY_SLOT = 0,    /* No memory down and a physical memory slot. */
	STATE_MEMORY_DOWN = 1,    /* Memory down and not a physical memory slot. */
} MemorySlotState;

typedef struct {
	MemorySlotState  SlotState[MAX_CHAN][MAX_DIMM];   /* Memory Down state of each DIMM in each Channel */
	UINT16           SpdDataLen;                    /* Length in Bytes of a single DIMM's SPD Data */
	UINT8            *SpdDataPtr[MAX_CHAN][MAX_DIMM]; /* Pointer to SPD Data for each DIMM in each Channel */
} MEMORY_DOWN_CONFIG;

/*
* SMBIOS Memory Info structures.
*/
typedef struct {
	UINT8         DimmId;
	UINT32        SizeInMb;
	UINT16        MfgId;
	UINT8         ModulePartNum[20];/* Module part number for DDR3 is 18 bytes however for DRR4 20 bytes as per JEDEC Spec, so reserving 20 bytes */
} DIMM_INFO;

typedef struct {
	UINT8         ChannelId;
	UINT8         DimmCount;
	DIMM_INFO     DimmInfo[MAX_DIMM];
} CHANNEL_INFO;

typedef struct {
	UINT8         Revision;
	UINT16        DataWidth;
	/** As defined in SMBIOS 3.0 spec
	Section 7.18.2 and Table 75
	**/
	UINT8         MemoryType;
	UINT16        MemoryFrequencyInMHz;
	/** As defined in SMBIOS 3.0 spec
	Section 7.17.3 and Table 72
	**/
	UINT8         ErrorCorrectionType;
	UINT8         ChannelCount;
	CHANNEL_INFO  ChannelInfo[MAX_CHAN];
} FSP_SMBIOS_MEMORY_INFO;

/*
* FIA MUX configuration structures.
*/

#define BL_ME_FIA_MUX_LANE_NUM_MAX    20
#define BL_ME_FIA_MUX_LANE_MUX_SEL_MASK    0x3

typedef union _BL_ME_FIA_MUX_CONFIG {
	UINT64 MeFiaMuxLaneConfig;
	struct {
		UINT64 Lane00MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or PCIE
		UINT64 Lane01MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or PCIE
		UINT64 Lane02MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or PCIE
		UINT64 Lane03MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or PCIE
		UINT64 Lane04MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or PCIE or SATA
		UINT64 Lane05MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or PCIE or SATA
		UINT64 Lane06MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or PCIE or SATA
		UINT64 Lane07MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or PCIE or SATA
		UINT64 Lane08MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or PCIE or SATA
		UINT64 Lane09MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or PCIE or SATA
		UINT64 Lane10MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or PCIE or SATA
		UINT64 Lane11MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or PCIE or SATA
		UINT64 Lane12MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or PCIE or SATA
		UINT64 Lane13MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or PCIE or SATA
		UINT64 Lane14MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or PCIE or SATA
		UINT64 Lane15MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or PCIE or SATA
		UINT64 Lane16MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or XHCI or SATA
		UINT64 Lane17MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or XHCI or SATA
		UINT64 Lane18MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or XHCI or SATA
		UINT64 Lane19MuxSel : 2;  // ME_FIA_MUX_LANE_DISABLED or XHCI or SATA
		UINT64 Reserved : 24;
	} BL_MeFiaMuxLaneMuxSel;
} BL_ME_FIA_MUX_CONFIG;

typedef enum {
	BL_ME_FIA_MUX_LANE_DISCONNECTED,
	BL_ME_FIA_MUX_LANE_PCIE,
	BL_ME_FIA_MUX_LANE_SATA,
	BL_ME_FIA_MUX_LANE_XHCI,
} BL_ME_FIA_MUX_LANE_CONFIG;

/*
 * The FIA_MUX_CONFIG block describes the expected configuration of
 * FIA MUX configuration.
 */
typedef struct {
	UINT32  SkuNumLanesAllowed;    // Platform view of Num Lanes allowed
	BL_ME_FIA_MUX_CONFIG  FiaMuxConfig;  // Current Platform FIA MUX Configuration
	BL_ME_FIA_MUX_CONFIG  FiaMuxConfigRequest;  // FIA MUX Configuration Requested
} BL_FIA_MUX_CONFIG;

/*
 * The FIA_MUX_CONFIG_STATUS describes the status of configuring
 * FIA MUX configuration.
*/
typedef struct {
	UINT64     FiaMuxConfigGetStatus;  // Status returned from FiaMuxConfigGet,  if not EFI_SUCCESS, then error occurred and user can decide on next steps
	UINT64     FiaMuxConfigSetStatus;  // Status returned from FiaMuxConfigSet,  if not EFI_SUCCESS, then error occurred and user can decide on next steps
	BOOLEAN    FiaMuxConfigSetRequired;  // Boolean: true - a FiaMuxConfigSet was required, false otherwise
} BL_FIA_MUX_CONFIG_STATUS;

/*
* FIA MUX Config HOB structure
*/
typedef struct {
	BL_FIA_MUX_CONFIG              FiaMuxConfig;
	BL_FIA_MUX_CONFIG_STATUS       FiaMuxConfigStatus;
} BL_FIA_MUX_CONFIG_HOB;

/* !EXPORT EXTERNAL_BOOTLOADER_STRUCT_END   */
#pragma pack(pop)

#endif
