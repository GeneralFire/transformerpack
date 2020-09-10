/** @file
  Graphics header file

@copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/
#ifndef _GRAPHICS_INIT_H_
#define _GRAPHICS_INIT_H_

#include <Ppi/SiPolicy.h>
#include <Library/GraphicsInfoLib.h>
#include <Library/GraphicsInfoFruLib.h>

typedef union {
  struct {
    UINT32  Low;
    UINT32  High;
  } Data32;
  UINT64 Data;
} LARGE_INTEGER;

typedef struct {
  UINT8   Bus;
  UINT8   Device;
  UINT8   Function;
  UINT16  DevenMask;
} PEG_DEVEN;

typedef enum {
  IGD               = 0,
  PEG,
  PCI,
  DISPLAY_DEVICE_MAX
} DISPLAY_DEVICE;

typedef enum {
  VBIOS_DEFAULT     = 0,
  CRT,
  LFP,
  CRT_LFP,
  TV,
  LFPSDVO,
  EFP,
  TVSDVO,
  CRT_LFPSDVO,
  CRT_EFP,
  IGD_BOOT_TYPE_MAX
} IGD_BOOT_TYPE;

typedef enum {
  DSM_0         = 0,
  DSM_32,
  DSM_64,
  DSM_4            = 240,
  DSM_8            = 241,
  DSM_12           = 242,
  DSM_16           = 243,
  DSM_20           = 244,
  DSM_24           = 245,
  DSM_28           = 246,
  DSM_36           = 248,
  DSM_40           = 249,
  DSM_44           = 250,
  DSM_48           = 251,
  DSM_52           = 252,
  DSM_56           = 253,
  DSM_60           = 254
} DATA_STOLEN_MEMORY;

typedef enum {
  GM_32M            = 1,
  GM_64M            = 2,
  GM_128M           = 4,
  GM_MAX
} STOLEN_MEMORY;

#define GTT_SIZE_2MB             2
#define RC6CTXBASE_SIZE          32*1024  ///< Top of Stolen Memory - 32K
#define DISPLAY_CDCLK_TIMEOUT    3000     ///< ~3 ms
#define MAILBOX_WAITTIME         10       ///< ~10 us
#define MAILBOX_TIMEOUT          100      ///< ~100 us
#define TOTAL_T12_TIME           500      ///< 500ms
#define NO_DELAY                 0        ///< 0ms
#define MAX_DELAY                0xFFFF

//
// DFD Size
//
#define DFD_RESTORE_SIZE         0x1000  //4K

///
/// PAVP Modes
///
#define PAVP_PCM_SIZE_1_MB  1
#define PAVP_PCM_SIZE_2_MB  2

/**
  Check and Force Vdd On in End Of PEI

  @retval EFI_NOT_READY                   H/W Power Cycle sequence in progress, Vdd on not enabled or Policy not found
  @retval EFI_SUCCESS                     Vdd On already enabled or enabled successfully

**/
EFI_STATUS
EndOfPeiCheckAndForceVddOn (
  VOID
  );

/**
  Check and Force Vdd On in PostMem Phase

  @param[in] GRAPHICS_PEI_PREMEM_CONFIG      GtPreMemConfig

**/
VOID
PostMemCheckAndForceVddOn (
  IN   GRAPHICS_PEI_PREMEM_CONFIG      *GtPreMemConfig
  );

/**
  Check and Force Panel Power Enable bit Vdd

  @param[in] GRAPHICS_PEI_PREMEM_CONFIG      GtPreMemConfig

  @retval    EFI_SUCCESS     The function completed successfully.

**/
EFI_STATUS
CheckAndForceVddOn (
  IN   GRAPHICS_PEI_PREMEM_CONFIG      *GtPreMemConfig
  );

/**
  GraphicsInit: Initialize the IGD if no other external graphics is present

  @param[in] GtConfig  - GRAPHICS_PEI_PREMEM_CONFIG to access the GtConfig related information
  @param[in] PrimaryDisplay       - Primary Display - default is IGD
  @param[in, out] PegMmioLength   - Total IGFX MMIO length

**/
VOID
GraphicsInit (
  IN       GRAPHICS_PEI_PREMEM_CONFIG   *GtPreMemConfig,
  IN       DISPLAY_DEVICE               *PrimaryDisplay,
  IN       UINT32                       *IGfxMmioLength
  );

/**
Initialize GT Power management

  @param[in] GRAPHICS_PEI_PREMEM_CONFIG      GtPreMemConfig
  @param[in] GRAPHICS_PEI_CONFIG             GtConfig

  @retval EFI_SUCCESS          - GT Power management initialization complete
**/
EFI_STATUS
GraphicsPmInit (
  IN       GRAPHICS_PEI_PREMEM_CONFIG      *GtPreMemConfig,
  IN       GRAPHICS_PEI_CONFIG             *GtConfig
  );

/**
  Initialize PAVP feature of SystemAgent.

  @param[in] GtConfig      - Instance of GRAPHICS_PEI_CONFIG
  @param[in] MiscPeiConfig - Instance of SA_MISC_PEI_CONFIG


  @retval EFI_SUCCESS     - PAVP initialization complete
  @retval EFI_UNSUPPORTED - iGFX is not present so PAVP not supported
**/
EFI_STATUS
PavpInit (
  IN       GRAPHICS_PEI_CONFIG             *GtConfig,
  IN       SA_MISC_PEI_CONFIG              *MiscPeiConfig,
  IN       GRAPHICS_PEI_PREMEM_CONFIG      *GtPreMemConfig
  );

/**
  Program PSMI registers.

  @param[in] GRAPHICS_PEI_CONFIG             *GtConfig
  @param[in] GRAPHICS_PEI_PREMEM_CONFIG      *GtPreMemConfig

  @retval EFI_SUCCESS     - PSMI registers programmed.
**/
EFI_STATUS
ProgramPsmiRegs (
  IN       GRAPHICS_PEI_CONFIG             *GtConfig,
  IN       GRAPHICS_PEI_PREMEM_CONFIG      *GtPreMemConfig
  );

/**
  Additional steps for disabling IGFX.

  @param[in] GtPreMemConfig  - GRAPHICS_PEI_PREMEM_CONFIG to access the GtPreMemConfig related information

**/
VOID
AdditionalStepsForDisablingIgfx (
  IN       GRAPHICS_PEI_PREMEM_CONFIG   *GtPreMemConfig
  );



/**
  Mirror System Agent registers.

  @param[in] GtPreMemConfig  Instance of GRAPHICS_PEI_PREMEM_CONFIG

  @retval EFI_SUCCESS     - SA Register Mirroring complete
**/
EFI_STATUS
SaRegisterMirror (
  IN       GRAPHICS_PEI_PREMEM_CONFIG             *GtPreMemConfig
  );

/**
  This function is to set Gfx Memory map (RC6 base, Doorbell base, Dfdbase etc)

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_NOT_FOUND           SiPreMemPolicyPpi not found
**/
EFI_STATUS
SetGfxMemMap (
  VOID
  );
#endif
