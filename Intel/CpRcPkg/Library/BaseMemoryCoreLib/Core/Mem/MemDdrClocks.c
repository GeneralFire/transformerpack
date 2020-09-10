/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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
#include <Library/PipeSyncLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemRcLib.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Library/MemPorLib.h>
#include <Memory/MemoryCheckpointCodes.h>

//
// QCLK ratios for 100MHz BCLK.
//
STATIC const UINT8   QCLKs_100[MAX_SUP_FREQ] = {8, 10, 10, 12, 12, 14, 16, 18, 18, 20, 20, 22, 24, \
                                                26, 26, 28, 28, 30, 32, 34, 35, 36, 37, 38, 40, 42, \
                                                43, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84};

//
// QCLK ratios for 133MHz BCLK. (Only frequencies with 133Mhz BCLK in this arrary are used currently)
//                                 800 1000 1066 1200 1333 1400 1600 1800 1866 2000 2133 2200 2400 2600 2666 2800 2933 3000 3200 <- First row
//                                3400 3467 3600 3733 3800 4000 4200 4266 4400 4800 5200 5600 6000 6400 6800 7200 7600 8000 8400 <- Second row
//
const UINT8 QCLKs[MAX_SUP_FREQ] = {  6, 10,   8,  12,  10,  14,  12,  18,  14,  20,   16,  22,  18,  26,  20,  28,  22,  30,  24,
                                    34, 26,  27,  28,  38,  30,  42,  32,  44};
const UINT8 BCLKs[MAX_SUP_FREQ] = {133, 100, 133, 100, 133, 100, 133, 100, 133, 100, 133, 100, 133, 100, 133, 100, 133, 100, 133,
                                   100, 133, 133, 133, 100, 133, 100, 133, 100};

#define FREQ_COUNT 19
UINT8 SupportedFrequencies[FREQ_COUNT] = {DDR_800, DDR_1066, DDR_1333, DDR_1600, DDR_1866, DDR_2133, DDR_2400, DDR_2666, DDR_2933, DDR_3200,
                                          DDR_3600, DDR_4000, DDR_4400, DDR_4800, DDR_5200, DDR_5600, DDR_6000, DDR_6400, DDR_8400};
UINT8 BclkFreq[2] = {IMC_BCLK_133, IMC_BCLK_100};

typedef struct {
  INT32 TckValue;
  UINT8 DdrFreqValue;
} TCK_TO_DDR_FREQ_STRUCT;

// -------------------------------------
// Code section
// -------------------------------------

/**
  Finds a common VDD setting for the system and updates the per
  socket ddrVoltage NVRAM variable. For DDR4, only 1.2V is
  supported, so the routine needs the be updated for other DDR
  technologies for later use

  @param Host          - Pointer to sysHost
  @param CommonDramVdd - Pointer to define common VDD accross
                         sockets

  @retval None
**/
VOID
GetDramVdd (
  IN OUT PSYSHOST Host,
     OUT UINT8    *CommonDramVdd
  )
{
  UINT8 Socket;

  // DDR4 only supports 1.2V & DDR5 only supports 1.1V
  Socket = Host->var.mem.currentSocket;

  if (IsDdr5MemSsPresent (Host, Socket)) {
   *CommonDramVdd = SPD_VDD_110;
  } else {
    *CommonDramVdd = SPD_VDD_120;
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Loop for each CPU socket
    //
    Host->nvram.mem.socket[Socket].ddrVoltage = *CommonDramVdd;
  } // Socket loop
}

/**
  First approach to the final DDR frequency based on tCK value

  @param Host         - Pointer to sysHost
  @param Socket       - Socket number
  @param FinalDdrFreq - Array for DDR frequencies across
                        sockets

  @retval None
**/
VOID
SelectDdrFreqBasedOnTck (
  IN     PSYSHOST Host,
  IN     UINT8    Socket,
     OUT UINT8    FinalDdrFreq[MAX_SOCKET]
  )
{
  UINT8                  Index;
  INT32                  CommonTck;
  //
  // Note that the array should be ordered to be able to sweep it and find the
  // correct value according to tCK. So, in case that new values are added, they
  // must be placed in the correct place
  //
  TCK_TO_DDR_FREQ_STRUCT TckToDdrFreq[] = {{DDR_8400_TCK_MIN, DDR_8400},
                                           {DDR_8000_TCK_MIN, DDR_8000},
                                           {DDR_7600_TCK_MIN, DDR_7600},
                                           {DDR_7200_TCK_MIN, DDR_7200},
                                           {DDR_6800_TCK_MIN, DDR_6800},
                                           {DDR_6400_TCK_MIN, DDR_6400},
                                           {DDR_6000_TCK_MIN, DDR_6000},
                                           {DDR_5600_TCK_MIN, DDR_5600},
                                           {DDR_5200_TCK_MIN, DDR_5200},
                                           {DDR_4800_TCK_MIN, DDR_4800},
                                           {DDR_4400_TCK_MIN, DDR_4400},
                                           {DDR_4266_TCK_MIN, DDR_4266},
                                           {DDR_4200_TCK_MIN, DDR_4200},
                                           {DDR_4000_TCK_MIN, DDR_4000},
                                           {DDR_3800_TCK_MIN, DDR_3800},
                                           {DDR_3733_TCK_MIN, DDR_3733},
                                           {DDR_3600_TCK_MIN, DDR_3600},
                                           {DDR_3466_TCK_MIN, DDR_3466},
                                           {DDR_3400_TCK_MIN, DDR_3400},
                                           {DDR_3200_TCK_MIN, DDR_3200},
                                           {DDR_2933_TCK_MIN, DDR_2933},
                                           {DDR_2800_TCK_MIN, DDR_2800},
                                           {DDR_2666_TCK_MIN, DDR_2666},
                                           {DDR_2600_TCK_MIN, DDR_2600},
                                           {DDR_2400_TCK_MIN, DDR_2400},
                                           {DDR_2133_TCK_MIN, DDR_2133},
                                           {DDR_1866_TCK_MIN, DDR_1866},
                                           {DDR_1600_TCK_MIN, DDR_1600},
                                           {DDR_1333_TCK_MIN, DDR_1333},
                                           {DDR_1066_TCK_MIN, DDR_1066}
                                          };

  CommonTck = Host->nvram.mem.socket[Socket].minTCK;

  for (Index = 0; Index < (sizeof (TckToDdrFreq) / sizeof (TckToDdrFreq[0])); Index++) {
    if (CommonTck <= TckToDdrFreq[Index].TckValue) {
      FinalDdrFreq[Socket] = TckToDdrFreq[Index].DdrFreqValue;
      break;
    }
  }

  if (Index >= (sizeof (TckToDdrFreq) / sizeof (TckToDdrFreq[0]))) {
    if ((Host->nvram.mem.dramType) == SPD_TYPE_DDR5) {
       FinalDdrFreq[Socket] = DDR_3200;
       RcDebugPrint (SDBG_MINMAX,
                              "tCK value was not found, using 3200 clock.\n");
    } else {
      RcDebugPrint (SDBG_MINMAX,
                             "tCK value was not found, using 1600 clock.\n");
      FinalDdrFreq[Socket] = DDR_1600;
    }
  }
}

/**
  Evaluates if the DDR frequency should be capped to the POR
  values

  @param Host          - Pointer to sysHost
  @param Socket        - Socket number
  @param CommonDramVdd - Common VDD accross sockets
  @param FinalDdrFreq  - Array for DDR frequencies across
                         sockets

  @retval None
**/
VOID
CheckPORFreq (
  IN     PSYSHOST Host,
  IN     UINT8    Socket,
  IN     UINT8    CommonDramVdd,
     OUT UINT8    FinalDdrFreq[MAX_SOCKET]
  )
{
  EFI_STATUS          Status;
  UINT8               Ch;
  UINT8               TmpFreq;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  SYS_SETUP           *Setup;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  ChannelNvList = &Host->nvram.mem.socket[Socket].channelList;

  if (Setup->mem.enforcePOR != ENFORCE_POR_DIS) {
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }

      //
      // Returns the frequency supported by the memory configuration on this channel
      //
      Status = GetPorDdrFrequency (Socket, Ch, CommonDramVdd, &TmpFreq);

      //
      // Map out this channel if the configuration is not supported
      //
      if ((EFI_ERROR (Status)) || (TmpFreq == NOT_SUP)) {
        DisableChannelSw (Host, Socket, Ch);
        OutputWarning (WARN_DIMM_COMPAT, WARN_CHANNEL_CONFIG_FREQ_NOT_SUPPORTED, Socket, Ch, NO_DIMM, NO_RANK);
      } else {
        FinalDdrFreq[Socket] = (UINT8) MIN (TmpFreq, FinalDdrFreq[Socket]);
      }
    } // ch loop
  }
}

/**
  Validates that the DDR frequency selected:
  1. DDR Frequency is below the Memory Frequency setup value
  2. ENFORCE POR is disabled and the processor supports the
  frequency selected in Memory Frequency field

  @param Host             - Pointer to sysHost
  @param Socket           - Socket number
  @param MaxCpuDdrFreqSup - Maximum DDR frequency supported by
                            the processor
  @param CommonSocketFreq - Common frequency
  @param FinalDdrFreq     - Array for DDR frequencies across
                            sockets

  @retval None
**/
VOID
CheckSupportForFinalFreq (
  IN     PSYSHOST Host,
  IN     UINT8    Socket,
  IN     UINT8    MaxCpuDdrFreqSup,
  IN OUT UINT8    *CommonSocketFreq,
  IN OUT UINT8    FinalDdrFreq[MAX_SOCKET]
  )
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  //
  // Check if the DDR frequency has been specified by an input parameter
  //
  if (Setup->mem.ddrFreqLimit) {
    //
    // Make sure this frequency is supported
    //
    if (FinalDdrFreq[Socket] > (Setup->mem.ddrFreqLimit - 1)) {
      FinalDdrFreq[Socket] = (Setup->mem.ddrFreqLimit - 1);
    }

    //
    // Check if we want to ignore POR restrictions
    //
    if (Setup->mem.enforcePOR == ENFORCE_POR_DIS) {
      FinalDdrFreq[Socket] = Setup->mem.ddrFreqLimit - 1;
    }
  }

  //
  // If overclocking is not enabled and the supported ratio is lower than the desired ratio then cap the desired ratio
  //
  if (FinalDdrFreq[Socket] > MaxCpuDdrFreqSup) {
    FinalDdrFreq[Socket] = MaxCpuDdrFreqSup;
    RcDebugPrint (SDBG_MINMAX,
                    "The requested memory speed is faster than this processor supports. Set to maxDdrFreq = %d\n", MaxCpuDdrFreqSup);
  }

  //
  // Check if the frequency needs to be overridden
  //
  ChipOverrideFreq (Host, Socket, FinalDdrFreq);

  if (FinalDdrFreq[Socket] < *CommonSocketFreq) {
    *CommonSocketFreq = FinalDdrFreq[Socket];
  }
}

/**
  Updates the Host with the DDR frenquency to be used

  @param Host         - Pointer to sysHost
  @param FinalDdrFreq - Array for DDR frequencies across
                        sockets

  @retval None
**/
VOID
UpdateDdrFreq (
  IN OUT PSYSHOST Host,
  IN     UINT8    FinalDdrFreq[MAX_SOCKET]
  )
{
  UINT8 Socket;

  if (Host->var.mem.subBootMode == WarmBoot || Host->var.mem.subBootMode == WarmBootFast || Host->var.mem.subBootMode == ColdBootFast) {
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
        continue;
      }

      if (Host->nvram.mem.socket[Socket].ddrFreq != FinalDdrFreq[Socket]) {
        Host->nvram.mem.socket[Socket].ddrFreq = FinalDdrFreq[Socket];

        //
        // Disable the warm boot path because the DDR frequency has changed
        //
        Host->var.mem.subBootMode = ColdBoot;
        //
        // Also, make sure you don't skip the memory init
        //
        Host->var.mem.skipMemoryInit = 0;
      }
    } // socket loop
  } else {
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      Host->nvram.mem.socket[Socket].ddrFreq = FinalDdrFreq[Socket];
    } // socket loop
  }
}

/** Set the DDR Max frequency syshost variable on all sockets.

  This function is called during fast or s3 boots to initialize
  Host->var.mem.socket[socket].maxFreq

  @param[in]  Host  Pointer to the SysHost structure

**/
VOID
SetDDRMaxFreqAllSockets (
  IN  PSYSHOST  Host
  )
{
  UINT8       Socket;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (Host->nvram.mem.socket[Socket].enabled == 1) {
      GetMaxCpuDdrFreq (Host, Socket);
    }
  }
}

/** Set the DDR frequency on all sockets.

  This includes handling of sockets without any DIMMs populated.

  @param[in]  Host  Pointer to the SysHost structure

  @retval EFI_SUCCESS   The frequency was set successfully.
  @retval !EFI_SUCCESS  The frequency was not set successfully.

**/
EFI_STATUS
SetDDRFreqAllSockets (
  IN  PSYSHOST  Host
  )
{
  UINT8       Socket;
  EFI_STATUS  Status = EFI_SUCCESS;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (Host->nvram.mem.socket[Socket].enabled == 1) {
      if (Host->nvram.mem.socket[Socket].maxDimmPop == 0) {
        Status = SetDDRFreqNoDimms (Socket);
      } else {
        SetDDRFreq (Host, Socket);
      }
    }
  }

  return Status;
}

/**
  Determines which frequency system needs to run
  across differentt memory sub systems and programs it

  @param Host  - Pointer to sysHost

  @retval SUCCESS - Clock is set properly
  @retval FAILURE - A reset is required to set the clock
**/
UINT32
InitClocks (
  IN OUT PSYSHOST Host
  )
{
  UINT8               Socket;
  UINT8               FinalDdrFreq[MAX_SOCKET];
  UINT8               CommonSocketFreq;
  UINT8               MaxCpuDdrFreqSup;
  UINT8               CommonDramVdd;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  //
  // Bypass for the S3 resume path
  //
  if (GetSysBootMode () == S3Resume || Host->var.mem.subBootMode == WarmBootFast || Host->var.mem.subBootMode == ColdBootFast) {
    //
    // On S3 resume, just make sure to set the DDR frequency and exit.
    //
    SetDDRFreqAllSockets (Host);
    SetDDRMaxFreqAllSockets (Host);
    return SUCCESS;
  }

  GetDramVdd (Host, &CommonDramVdd);

  CommonSocketFreq = 0xFF; //Init to highest value. Actual value gets set in the below loop.

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Loop for each CPU socket
    //
    if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
      continue;
    }

    FinalDdrFreq[Socket] = 0;

    //
    // Search enabled IMCs
    //
    ChannelNvList = GetChannelNvList (Host, Socket);

    if ((PcdGetBool (PcdMrcXmpSupport)) &&
        (Setup->mem.ddrFreqLimit != 0) &&
        (Setup->mem.options & MEM_OVERRIDE_EN)) {

      //
      // The DDR frequency has been provided by an input parameter
      //
      FinalDdrFreq[Socket] = Setup->mem.ddrFreqLimit - 1;
      CommonSocketFreq = FinalDdrFreq[Socket];

    } else {

      //
      // Determine common tCK for host controller. This needs to be reset for each socket.
      //
      SelectDdrFreqBasedOnTck (Host, Socket, FinalDdrFreq);

      //
      // Check the DDR frequency supported by each channel and use the fastest common frequency
      //
      CheckPORFreq (Host, Socket, CommonDramVdd, FinalDdrFreq);
    }

    MaxCpuDdrFreqSup = GetMaxCpuDdrFreq (Host, Socket);

    CheckSupportForFinalFreq (Host, Socket, MaxCpuDdrFreqSup, &CommonSocketFreq, FinalDdrFreq);
  } // Socket loop

  // Use common frequency across sockets if mixing is not supported
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    FinalDdrFreq[Socket] = CommonSocketFreq;
  } // Socket loop

  UpdateDdrFreq (Host, FinalDdrFreq);
  SetDDRFreqAllSockets (Host);

  return SUCCESS;
} // InitDdrClocks

/**
  Return the current encoded DDR frequency (DDR_2400, DDR_2933, etc)

  @param[in]    Socket    - Socket number
  @param[out]   DdrFreq   - Pointer to caller's DdrFreq storage

  @retval EFI_SUCCESS if frequency found
          EFI_INVALID_PARAMTER if passed parameter is bad

**/
EFI_STATUS
EFIAPI
GetEncodedDdrFreq (
  IN     UINT8    Socket,
  OUT    UINT8    *DdrFreq
  )
{
  PSYSHOST      Host;

  if ((DdrFreq == NULL) || (Socket >= MAX_SOCKET)) {
    return EFI_INVALID_PARAMETER;
  }

  Host = GetSysHostPointer ();

  *DdrFreq = Host->nvram.mem.socket[Socket].ddrFreq;

  return EFI_SUCCESS;
}

/**
  Defines the timing mode (1N, 2N, 3N...) to be used

  @param Host             - Pointer to sysHost
  @param Socket           - Socket number

  @retval None
**/
VOID
DefineTimingMode (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  UINT8               Ch;
  UINT8               Dimm;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT32              StretchMode;
  EFI_STATUS          Status;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }

    // Initialize ddrtEnabled variable
    (*ChannelNvList) [Ch].ddrtEnabled = 0;
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList) [Dimm].dimmPresent == 0) {
        continue;
      }
      if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
        continue;
      }
      (*ChannelNvList)[Ch].ddrtEnabled = 1;
    }

    //
    // Set correct timing mode
    // Enable 2N timing for UDIMMs/SODIMMs if more than 1 UDIMM/SODIMM is installed on this channel
    //
    if ((Host->nvram.mem.dimmTypePresent != RDIMM) && ((*ChannelNvList)[Ch].maxDimm > 1)) {
      (*ChannelNvList)[Ch].timingMode = TIMING_2N;
    }

    //
    // UDIMMs/SODIMMs > 1R, run with 2N timings
    //
    if ((Host->nvram.mem.dimmTypePresent != RDIMM) && ((*ChannelNvList)[Ch].numRanks > 1)) {
      (*ChannelNvList)[Ch].timingMode = TIMING_2N;
    }

    if (PcdGetBool (PcdMrcXmpSupport)) {

      SYS_SETUP *Setup;

      Setup = GetSysSetupPointer ();

      if ((Setup->mem.options & MEM_OVERRIDE_EN) && Setup->mem.inputMemTime.nCMDRate) {
        (*ChannelNvList)[Ch].timingMode = Setup->mem.inputMemTime.nCMDRate;
        if ((*ChannelNvList)[Ch].timingMode == 1) {
          (*ChannelNvList)[Ch].timingMode--;
        }
      }
    }
    //
    // Override command timing mode if required
    //
    if (UbiosGenerationOrHsleEnabled () && (IsDdr5Present (Host, Socket))) {
      //
      // Emulation timing mode override
      //
      if ((*ChannelNvList)[Ch].ddrtEnabled  == 1) {
        (*ChannelNvList)[Ch].timingMode = TIMING_1N;
      }
    }

    SiliconSpecificOverrideCommandTimingMode (Host, Socket, Ch);
    Status = CteGetOptions ("stretchmode", &StretchMode);
    if (!EFI_ERROR (Status)) {
      if (StretchMode == 1) {
        (*ChannelNvList) [Ch].timingMode = TIMING_1N;
      } else if (StretchMode == 2) {
        (*ChannelNvList) [Ch].timingMode = TIMING_2N;
      } else if (StretchMode == 3) {
        (*ChannelNvList) [Ch].timingMode = TIMING_3N;
      }
    }
    if (IsBrsPresent(Host, Socket)) {
      (*ChannelNvList)[Ch].timingMode = TIMING_2N;
    }
  } // ch loop
}

/**
  Get MC QCLK ratio.

  @param[in]   Index    - Frequency index to array QCLKs or QCLKs_100.
  @param[in]   BaseClk  - DDR PLL reference clock.

  @retval  QCLK ratio

**/
UINT8
GetQCLKs (
  IN UINT8 Index,
  IN UINT8 BaseClk
  )
{
  if (BaseClk == IMC_BCLK_133) {
    return (QCLKs[Index]);
  }

  return (QCLKs_100[Index]);
}

/**
  Get MC BCLK frequency.

  @param Socket           - Socket number

  @retval - BCLK Frequency for given Socket

**/
UINT8
EFIAPI
GetBclkFreq (
  IN  UINT8    Socket
)
{
  UINT8               Ch;
  UINT8               Dimm;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct channelNvram (*ChannelNvList)[MAX_CH];
  SYS_SETUP           *Setup;
  UINT8               Bclk;
  PSYSHOST            Host;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  Setup = GetSysSetupPointer ();
  Bclk = IMC_BCLK_133;

  if (Setup->mem.imcBclk == IMC_BCLK_AUTO || (Host->nvram.mem.socket[Socket].ddrFreqMHz % 100)) {
    ChannelNvList = GetChannelNvList (Host, Socket);

    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == FALSE) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == FALSE) {
          continue;
        }

        if ((*DimmNvList)[Dimm].keyByte == SPD_TYPE_DDR4) {

          //
          // DDR4
          //
          Bclk = BCLKs[Host->nvram.mem.ratioIndex];
          return Bclk;
        } else if ((*DimmNvList)[Dimm].keyByte == SPD_TYPE_DDR5) {

          //
          // DDR5
          //
          Bclk = IMC_BCLK_100;
          return Bclk;
        } else if (((*DimmNvList)[Dimm].fmcType ==  FMC_FNV_TYPE) || \
                   ((*DimmNvList)[Dimm].fmcType ==  FMC_EKV_TYPE) || \
                   ((*DimmNvList)[Dimm].fmcType ==  FMC_BWV_TYPE)) {
          //
          // DDRT
          //
          Bclk = BCLKs[Host->nvram.mem.ratioIndex];
          return Bclk;
        } else if ((*DimmNvList)[Dimm].fmcType == FMC_CWV_TYPE) {

          //
          // DDRT2
          //
          Bclk = IMC_BCLK_100;
          return Bclk;
        }
      }
    }
  } else if (Setup->mem.imcBclk == IMC_BCLK_100) {

    //
    // Setup forces 100MHz.
    //
    Bclk = IMC_BCLK_100;
  } else if (Setup->mem.imcBclk == IMC_BCLK_133) {

    //
    // Setup forces 133MHz.
    //
    Bclk = IMC_BCLK_133;
  }

  return Bclk;
}

/**
  Initialized the QCLK ratio based on the DDR frequency.

  @param Host             - Pointer to sysHost
  @param Socket           - Socket number

  @retval None
**/
VOID
InitQclkRatio (
  IN  PSYSHOST Host,
  IN  UINT8    Socket
  )
{
  UINT8   BClk;
  UINT8   QCLKRatio = 0;
  UINT8   RefClkType = 0;
  UINT8   ReqData;
  BOOLEAN FrequencyChangeNeeded;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  //
  // Initialize BCLK per setup input and detected DDR type.
  //
  BClk = GetBclkFreq (Socket);

  QCLKRatio = GetQCLKs(GetHostFrequency (Socket), BClk);

  //
  // Get current QCLK ratio.
  //
  GetStartupQclkRatio (Socket, &RefClkType, &ReqData);

  if (BClk == IMC_BCLK_133) {
    Host->var.mem.QCLKPeriod = 1000000 / (QCLKRatio * 133 + (QCLKRatio / 3));
  } else {
    //
    // BCLK = 100Mhz
    //
    Host->var.mem.QCLKPeriod = 1000000 / (QCLKRatio * 100);
  }

  if (GetSysResetRequired ()) {
    RcDebugPrint (SDBG_MINMAX,
                    "Reset requested: non-MRC\n");
  }

  //
  // Bubble Generator Setup
  //
  ProgramBgfTable (Host, Socket);

  //
  // Compare current DCLK ratio to desired DCLK ratio
  //
  FrequencyChangeNeeded = FALSE;
  if ((QCLKRatio != ReqData) || (BClk != BclkFreq[RefClkType])) {
    //
    // Skip system reset for default ratio
    // Indicate a reset is required
    //
    FrequencyChangeNeeded = TRUE;

    //
    //Set BCLK
    //
    if (BClk == IMC_BCLK_133) {
      RefClkType = BCLK_TYPE_133; // Index of BclkFreq
    } else {
      RefClkType = BCLK_TYPE_100;
    }
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CLOCK_INIT, CHECKPOINT_MINOR_SET_FREQ, Socket));

  SetQclkRatio (Host, Socket, RefClkType, QCLKRatio, FrequencyChangeNeeded);
}

/**
  Set the DDR clock frequency

  @param Host   - Pointer to sysHost
  @param Socket - Socket number

  @retval SUCCESS - Clock is set properly
  @retval FAILURE - A reset is required to set the clock
**/
UINT32
SetDDRFreq (
  IN OUT PSYSHOST Host,
  IN     UINT8    Socket
  )
{
  UINT16       DdrFreqMHz;

  //
  // Obtain frequency in MHz
  //
  DdrFreqMHz = GetDdrFreqInMhz (Socket);

  //
  // QCLK Period in pico seconds
  //
  Host->nvram.mem.socket[Socket].QCLKps = (UINT16) (1000000/ (DdrFreqMHz * 2));

  //
  // Save ratio table index for later use
  //
  Host->nvram.mem.ratioIndex = Host->nvram.mem.socket[Socket].ddrFreq;

  RcDebugPrint (SDBG_MAX,
                  "ratioIndex = %d\n", Host->nvram.mem.ratioIndex);

  DefineTimingMode (Host, Socket);

  InitQclkRatio (Host, Socket);

  //
  // Exit with failure if the MRC requires a reset or if a reset was requested before the MRC was called
  // Exiting with failure will force the MRC to return execution to the caller
  //
  if (GetSysResetRequired ()) {
    return FAILURE;
  }

#ifdef DEBUG_CODE_BLOCK
  if ((Host->nvram.mem.socket[Socket].maxDimmPop != 0) && (GetDebugLevel () & SDBG_MINMAX)) {
    AcquirePrintControl ();
    RcDebugPrint (SDBG_DEFAULT, "Memory behind processor %d running at ", Socket);
    DisplayDDRFreq (Host, Socket);
    ReleasePrintControl ();
  }
#endif //DEBUG_CODE_BLOCK

  return SUCCESS;
} // SetDDRFreq

/**
  Get the closest frequency to the one defined in the Host

  @param Host   - Pointer to sysHost
  @param Socket - Socket number

  @retval Freq
**/
UINT8
GetClosestFreq (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  UINT8 FreqLoop;
  UINT8 Freq = Host->nvram.mem.socket[Socket].ddrFreq;

  //
  // Make sure the frequency is part of the table
  //
  if (Host->nvram.mem.socket[Socket].ddrFreq > SupportedFrequencies[FREQ_COUNT - 1]) {
    Freq = SupportedFrequencies[FREQ_COUNT - 1];
    return Freq;
  }

  for (FreqLoop = 0; FreqLoop < FREQ_COUNT; FreqLoop++) {
    //
    // Search the frequency in the table, if found, break and use the index where found
    //
    if (Host->nvram.mem.socket[Socket].ddrFreq == SupportedFrequencies[FreqLoop]) {
      Freq = Host->nvram.mem.socket[Socket].ddrFreq;
      //
      // Frequency found
      //
      break;
    }

    if (FreqLoop < (FREQ_COUNT - 1)) {
      if ((Host->nvram.mem.socket[Socket].ddrFreq > SupportedFrequencies[FreqLoop]) &&
          (Host->nvram.mem.socket[Socket].ddrFreq < SupportedFrequencies[FreqLoop + 1])) {
        Freq = SupportedFrequencies[FreqLoop + 1];
        break;
      }
    }
  }

  return Freq;
}







