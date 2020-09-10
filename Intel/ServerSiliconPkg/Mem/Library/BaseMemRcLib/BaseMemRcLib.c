/** @file
  Base implementation of the memory reference code library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include <Base.h>
#include <SysHost.h>
#include <Library/SysHostPointerLib.h>
#include <Library/KtiApi.h>
#include <Library/MemRcLib.h>
#include <Include/MemMapHost.h>
#include <Library/CpuAndRevisionLib.h>


typedef struct {
  UINT8  DdrFreq;
  UINT16 DdrFreqMhz;
} DDR_FREQ_TO_MHZ_STRUCT;

/**
  Get memory NUMA support option

  @retval TRUE          NUMA support is turned on
  @retval FALSE         NUMA support is turned off

**/
BOOLEAN
EFIAPI
IsNumaAware (
  VOID
  )
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  return (BOOLEAN)((Setup->mem.options & NUMA_AWARE) != 0);
}


/**
  Get cluster XOR support option

  @retval TRUE          Cluster XOR support is turned on
  @retval FALSE         Cluster XOR support is turned off

**/
BOOLEAN
EFIAPI
IsClusterXorEnable (
  VOID
  )
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  return (BOOLEAN)(Setup->mem.dfxMemSetup.DfxClusterXorEn != 0);

}


/**
  Get prefetch option based on system memory size

  @return prefetch option

**/
UINT8
EFIAPI
GetPrefetchOption (
  VOID
  )
{
  PSYSHOST       Host;
  UINT8          Option;
  UINT8          LowGap;

  Host = GetSysHostPointer ();
  LowGap = GetLowGap ();

  if ((LowGap  <= Host->var.mem.lowMemSize) && (Host->var.mem.memSize >= 0x20)){
    Option = 1;
  } else if (LowGap <= Host->var.mem.highMemSize){
    Option = 2;
  } else {
    Option = 0;  // disabled xpt prefetch
  }

  return Option;
}

/**
  Get total memory size in 64MB unit for a specific socket

  @param[in] SocketId         The socket id

  @return the total memory size in 64MB unit for the whole system

**/
UINT32
EFIAPI
GetSocketMemSize (
  IN UINT8      SocketId
  )
{
  PSYSHOST       Host;

  Host = GetSysHostPointer ();

  return Host->var.mem.socket[SocketId].memSize;
}

/**
  Get memory size in 64MB unit for a specified channel

  @param[in] SocketId         The socket id
  @param[in] ChannelId        The channel id on socket

  @return the memory size in 64MB unit for the specified channel

**/
UINT32
EFIAPI
GetChannelMemSize (
  IN UINT8      SocketId,
  IN UINT8      ChannelId
  )
{
  PSYSHOST       Host;

  Host = GetSysHostPointer ();

  return Host->var.mem.socket[SocketId].channelList[ChannelId].memSize;
}

/**
  Get DDR channel interleaved memory size in 64MB unit for a specified channel

  @param[in] SocketId         The socket id
  @param[in] ChannelId        The channel id on socket

  @return DDR channel interleaved memory size in 64MB unit

**/
UINT32
EFIAPI
GetChannelVolMemInterleaveSize (
  IN UINT8      SocketId,
  IN UINT8      ChannelId
  )
{
  MEMORY_MAP_HOST  *MemMapHost;

  MemMapHost = GetMemMapHostPointer ();
  if (MemMapHost == NULL) {
    return 0;
  }

  return MemMapHost->SocketInfo[SocketId].ChannelInfo[ChannelId].VolIntlvSizePerCh;
}

/**
  Returns system clustering mode for DDR memory after memory decode has been executed.

  @return the ddr clustering mode.

**/
CLUSTERING_MODE
EFIAPI
GetDdrClusteringMode (
  VOID
  )
{
  MEMORY_MAP_HOST  *MemMapHost;

  MemMapHost = GetMemMapHostPointer ();
  if (MemMapHost == NULL) {
    return MaxClusteringMode;
  }

  return MemMapHost->DdrClusteringMode;
}


/**
  Get Socket Xor Mode

  @param[in] SocketIndex     - Socket ID.

  @return Xor Mode

**/
UINT8
EFIAPI
GetSocketXorMode (
  IN  UINT8    SocketIndex
  )
{
  MEMORY_MAP_HOST  *MemMapHost;

  MemMapHost = GetMemMapHostPointer ();
  if (MemMapHost == NULL) {
    return 0;
  }
  return MemMapHost->SocketInfo[SocketIndex].XorMode;
}

/**
  This function checks if the biased 2-way near memory cache is enabled on system.

  @retval TRUE    Biased 2-way near memory cache is enabled.
  @retval FALSE   Biased 2-way near memory cache is disabled.

**/
BOOLEAN
EFIAPI
IsTwoWayNmCacheEnabled (
  VOID
  )
{
  MEMORY_MAP_HOST  *MemMapHost;

  MemMapHost = GetMemMapHostPointer ();
  if (MemMapHost == NULL) {
    return FALSE;
  }
  return MemMapHost->IsTwoWayNmCacheEn;
}

/**
  This function checks if the fADR feature is enabled on system.

  @retval TRUE      fADR is enabled
  @retval FALSE     fADR is disabled.

**/
BOOLEAN
EFIAPI
IsFadrEnabled (
  VOID
  )
{
  MEMORY_MAP_HOST  *MemMapHost;

  MemMapHost = GetMemMapHostPointer ();
  if (MemMapHost == NULL) {
    return FALSE;
  }
  return MemMapHost->IsFadrEnabled;
}

/**
  Check if the mc in a socket is enabled

  @param[in] SocketId         The socket id
  @param[in] ImcId            The iMC id

  @return whether the mc in a socket is enabled

**/
BOOLEAN
EFIAPI
CheckSocketMcEnabled (
  IN UINT8     SocketId,
  IN UINT8     ImcId
  )
{
  PSYSHOST       Host;

  Host = GetSysHostPointer ();
  return   Host->nvram.mem.socket[SocketId].imc[ImcId].enabled;
}

/**
  Check if the mc in a socket is enabled

  @param[in] SocketId         The socket id

  @return whether the socket is enabled

**/
BOOLEAN
EFIAPI
CheckSocketEnabled (
  IN UINT8     SocketId
  )
{
  PSYSHOST       Host;

  Host = GetSysHostPointer ();
  return   Host->nvram.mem.socket[SocketId].enabled;
}

/**
  Check if AEP DIMM in a socket is present

  @param[in] SocketId         The socket id

  @return whether AEP DIMM is present in a socket

**/
BOOLEAN
EFIAPI
CheckSocketDcpmmPresent (
  IN UINT8     SocketId
  )
{
  PSYSHOST       Host;

  Host = GetSysHostPointer ();
  return   Host->nvram.mem.socket[SocketId].DcpmmPresent;
}

/**
  Get RAS mode

  @return system RAS mode

**/
UINT8
EFIAPI
GetRasMode (
  VOID
  )
{
  PSYSHOST       Host;

  Host = GetSysHostPointer ();
  return   Host->nvram.mem.RASmode;
}

/**
  Get Dram type

  @return the dram type in a system

**/
UINT16
EFIAPI
GetDramType (
  VOID
  )
{
  PSYSHOST       Host;

  Host = GetSysHostPointer ();
  return Host->nvram.mem.dramType;
}

/**
  Get the caching memory type.

  @retval 0  DDR is caching DDRT (CACHE_TYPE_DDR_CACHE_PMEM)
  @retval 1  HBM is caching DDR  (CACHE_TYPE_HBM_CACHE_DDR)

**/
UINT8
EFIAPI
GetCacheMemType (
  VOID
  )
{
  PSYSHOST       Host;

  Host = GetSysHostPointer ();

  return Host->var.mem.CacheMemType;
}

/**
  Get the effective volatile memory mode.

  @retval 0  The memory mode is 1LM           (VOL_MEM_MODE_1LM)
  @retval 1  The memory mode is 2LM           (VOL_MEM_MODE_2LM)
  @retval 2  The memory mode is Mixed 1LM+2LM (VOL_MEM_MODE_MIX_1LM2LM)

**/
UINT8
EFIAPI
GetVolMemMode (
  VOID
  )
{
  PSYSHOST       Host;

  Host = GetSysHostPointer ();

  return Host->var.mem.volMemMode;
}

/**
  Get the CSR offset value for SBSP to check in.

  @return the CSR offset value.

**/
UINT32
EFIAPI
GetSrPbspCheckinCsr (
  VOID
  )
{
  PSYSHOST       Host;

  Host = GetSysHostPointer ();
  return Host->var.mem.memChipPolicy.SrPbspCheckinCsr;
}

/**
  Indicate if an AEP DIMM is installed in any slot on a channel
  Provides the index of the first AEP DIMM if one is found.

  @param Socket   - Socket number
  @param Ch       - Channel number
  @param Dimm     - Pointer to dimm number - updated by this routine
                    with the 0-based index of the first AEP DIMM found.
                    This parameter is optional and may be NULL.

  @retVal   TRUE  if an AEP DIMM is installed and "dimm" updated
            FALSE otherwise, "dimm" not modified
**/
BOOLEAN
AepDimmPresent (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     *Dimm   OPTIONAL
  )
{
  UINT8                   DimmIndex;
  struct dimmNvram        (*DimmNvList)[MAX_DIMM];
  struct channelNvram     (*ChannelNvList)[MAX_CH];
  SYSHOST                 *Host;

  Host = GetSysHostPointer ();

  ChannelNvList = &Host->nvram.mem.socket[Socket].channelList;
  DimmNvList = &Host->nvram.mem.socket[Socket].channelList[Ch].dimmList;

  for (DimmIndex = 0; DimmIndex < MAX_DIMM; DimmIndex++){
    if ((*DimmNvList)[DimmIndex].dimmPresent == 0) {
      continue;
    }
    if ((*DimmNvList)[DimmIndex].DcpmmPresent) {
      if (Dimm != NULL) {
        *Dimm = DimmIndex;
      }
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Indicate if an NVDIMM-N is installed in any slot on a channel
  Provides the index of the first NVDIMM-N if one is found.

  @param[in]  Socket   - Socket number
  @param[in]  Ch       - Channel number
  @param[out] Dimm     - Pointer to dimm number - updated by this routine
                         with the 0-based index of the first NVDIMM-N DIMM found.
                         This parameter is optional and may be NULL.

  @retVal   TRUE  if an NVDIMM-N DIMM is installed and "dimm" optionally updated if Dimm is not NULL
            FALSE otherwise, "dimm" not modified
**/
BOOLEAN
NvdimmNPresent (
  IN  UINT8 Socket,
  IN  UINT8 Ch,
  OUT UINT8 *Dimm OPTIONAL
  )
{
  UINT8                 DimmIndex;
  CHANNEL_NVRAM_STRUCT  (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT     (*DimmNvList)[MAX_DIMM];
  PSYSHOST              Host;

  if ((Socket >= MAX_SOCKET) || (Ch >= MAX_CH)) {
    return FALSE;
  }

  Host = GetSysHostPointer ();
  if (NULL == Host) {
    return FALSE;
  }

  ChannelNvList = &Host->nvram.mem.socket[Socket].channelList;
  DimmNvList = &Host->nvram.mem.socket[Socket].channelList[Ch].dimmList;

  for (DimmIndex = 0; DimmIndex < MAX_DIMM; DimmIndex++) {
    if ((*DimmNvList)[DimmIndex].dimmPresent == 0) {
      continue;
    }

    if ((*DimmNvList)[DimmIndex].NvDimmNPresent == TRUE) {
      if (Dimm != NULL) {
        *Dimm = DimmIndex;
      }
      return TRUE;
    }
  }
  return FALSE;
}

/**

  Chip specific code path to halt the system

  @param[in[ HaltOnError - To halt or not on error

  @retval None

**/

VOID
RcDeadLoop (
  UINT8 HaltOnError
  )
{
  //
  // Prevent from optimizing out
  //
  while (*(volatile UINT8 *) &HaltOnError) {
    ;
  }

} // RcDeadLoop

/**
  Gets the DDR freqency in MHz using the macro encoding and
  updates the Host

  @param Socket           - Socket number

  @retval                - Data value for Ddr Freq in Mhz
**/
UINT16
EFIAPI
GetDdrFreqInMhz (
  IN     UINT8    Socket
  )
{
  UINT8                  Index;
  UINT16                 DdrFreqMhz = 0;
  PSYSHOST               Host;
  DDR_FREQ_TO_MHZ_STRUCT DdrFreqToMhz[] = {{DDR_800,   400},
                                           {DDR_1000,  500},
                                           {DDR_1066,  533},
                                           {DDR_1200,  600},
                                           {DDR_1333,  667},
                                           {DDR_1400,  700},
                                           {DDR_1600,  800},
                                           {DDR_1800,  900},
                                           {DDR_1866,  933},
                                           {DDR_2000, 1000},
                                           {DDR_2133, 1067},
                                           {DDR_2200, 1100},
                                           {DDR_2400, 1200},
                                           {DDR_2600, 1300},
                                           {DDR_2666, 1333},
                                           {DDR_2800, 1400},
                                           {DDR_2933, 1467},
                                           {DDR_3000, 1500},
                                           {DDR_3200, 1600},
                                           {DDR_3400, 1700},
                                           {DDR_3466, 1733},
                                           {DDR_3600, 1800},
                                           {DDR_3733, 1866},
                                           {DDR_3800, 1900},
                                           {DDR_4000, 2000},
                                           {DDR_4200, 2100},
                                           {DDR_4266, 2133},
                                           {DDR_4400, 2200},
                                           {DDR_4800, 2400},
                                           {DDR_5200, 2600},
                                           {DDR_5600, 2800},
                                           {DDR_6000, 3000},
                                           {DDR_6400, 3200},
                                           {DDR_8400, 4200}
  };


  Host = GetSysHostPointer();
  for (Index = 0; Index < (sizeof(DdrFreqToMhz) / sizeof(DdrFreqToMhz[0])); Index++) {
    if (Host->nvram.mem.socket[Socket].ddrFreq == DdrFreqToMhz[Index].DdrFreq) {
      Host->nvram.mem.socket[Socket].ddrFreqMHz = DdrFreqToMhz[Index].DdrFreqMhz;
      DdrFreqMhz = DdrFreqToMhz[Index].DdrFreqMhz;
      break;
    }
  }

  //
  // Default DDR 1600 for DDR4
  //
  if (Index >= (sizeof(DdrFreqToMhz) / sizeof(DdrFreqToMhz[0]))) {
    Host->nvram.mem.socket[Socket].ddrFreqMHz = 800;
    DdrFreqMhz = 800;
  }

  return  DdrFreqMhz;
}

/**
  Get the effective NVM CR QOS setting.

  @retval TRUE   NVM CR QOS is either 1 of the recipes chosen
  @retval FALSE  NVM CR QOS is disabled

**/
UINT8
EFIAPI
GetNvmCrQos (
  VOID
  )
{
  SYS_SETUP              *Setup;

  Setup = GetSysSetupPointer ();

  if (Setup->mem.NvmQos == NVM_QOS_DISABLE) {
    return FALSE;
  } else {
    return TRUE;
  }
}

/**
  Checks if a DDRT DIMM is present on the MC specified

  @param[in] Socket  Socket ID
  @param[in] McId    Mc Controller Id

  @retval TRUE if DDRT is present in the MC
          FALSE if DDRT is not present in the MC
**/
BOOLEAN
EFIAPI
IsDdrtPresentMC (
  IN UINT8 Socket,
  IN UINT8 McId
  )
{
  UINT8               Channel;
  UINT8               Dimm;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  PSYSHOST            Host;
  UINT8               NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();
  Host = GetSysHostPointer ();

  if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
    return FALSE;
  }

  ChannelNvList = &Host->nvram.mem.socket[Socket].channelList;

  for (Channel = 0; Channel < NumChPerMc; Channel++) {
    if ((*ChannelNvList)[Channel + (NumChPerMc * McId)].enabled == 0) {
      continue;
    }

    DimmNvList = &Host->nvram.mem.socket[Socket].channelList[Channel + (NumChPerMc * McId)].dimmList;

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      if ((*DimmNvList)[Dimm].DcpmmPresent) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**
  Check if there is any 9x4 DDR5 DIMM present on system.

  @retval TRUE    9x4 DDR5 DIMM is present on system.
  @retval FALSE   9x4 DDR5 DIMM is not present on system.
**/
BOOLEAN
EFIAPI
Check9x4Ddr5DimmPresent (
  VOID
  )
{
  PSYSHOST            Host;
  UINT8               Socket;
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               MaxChDdr;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];

  Host = (PSYSHOST) GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
      continue;
    }

    if (Host->nvram.mem.dramType != SPD_TYPE_DDR5) {
      continue;
    }

    ChannelNvList = &Host->nvram.mem.socket[Socket].channelList;

    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }

      DimmNvList = &Host->nvram.mem.socket[Socket].channelList[Ch].dimmList;

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (((*DimmNvList)[Dimm].dimmPresent == 0) || ((*DimmNvList)[Dimm].DcpmmPresent == 1))  {
          continue;
        }

        if ((((*DimmNvList)[Dimm].SPDMemBusWidth & SPD_BUS_WIDTH_EXT) >> SPD_BUS_WIDTH_EXT_BIT_OFFSET) == SPD_BUS_WIDTH_EXT_4_BITS_DDR5) {
          return TRUE;
        }

      } // Dimm loop
    } // Ch loop
  } // Socket loop

  return FALSE;
}


/**
  Check if there is any 9x4 DDR5 DIMM present on channel.

  @param[in]   Socket  -- socket index
  @param[in]   Ch  --  channel index

  @retval TRUE    9x4 DDR5 DIMM is present on system.
  @retval FALSE   9x4 DDR5 DIMM is not present on system.
**/
BOOLEAN
EFIAPI
Check9x4Ddr5DimmPresentPerChn (
  IN       UINT8       Socket,
  IN       UINT8       Ch
  )
{
  PSYSHOST            Host;
  UINT8               Dimm;
  UINT8               MaxChDdr;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];

  Host = (PSYSHOST) GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();

  if (Host->nvram.mem.dramType != SPD_TYPE_DDR5) {
      return FALSE;
  }

  ChannelNvList = &Host->nvram.mem.socket[Socket].channelList;

  DimmNvList = &Host->nvram.mem.socket[Socket].channelList[Ch].dimmList;

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if (((*DimmNvList)[Dimm].dimmPresent == 0) || ((*DimmNvList)[Dimm].DcpmmPresent == 1))  {
      continue;
    }

    if ((((*DimmNvList)[Dimm].SPDMemBusWidth & SPD_BUS_WIDTH_EXT) >> SPD_BUS_WIDTH_EXT_BIT_OFFSET) == SPD_BUS_WIDTH_EXT_4_BITS_DDR5) {
      return TRUE;
    }
  } // Dimm loop

  return FALSE;
}
/**
  Enable CSR prints to log

  @param[in] True or False

**/
VOID
EFIAPI
SetCsrPrintEnabling (
  IN  BOOLEAN      PrintCsr
  )
{
  PSYSHOST    Host;

  Host = GetSysHostPointer ();

  Host->PrintCsr = PrintCsr;

}

/**
  Get and return Host PrintCsr value

  @retval TRUE  CSR prints to log is enabled
          FALSE CSR prints to log is not enabled
**/
BOOLEAN
EFIAPI
GetCsrPrintEnabling (
 VOID
  )
{
  PSYSHOST    Host;

  Host = GetSysHostPointer ();

  return Host->PrintCsr;
}

/**
  Detects whether the Memory Subsystem is DDR5 type

  @param[in]    Host        - Pointer to sysHost structure
  @param[in]    Socket      - Socket number

  @retval TRUE  if DDR5 memory subsystem is present
  @retval FALSE if DDR5 memory subsytem is not present
**/
BOOLEAN
IsDdr5MemSsPresent (
  IN        PSYSHOST            Host,
  IN        UINT32              Socket
)
{
  // If enable_DDR5 == 1, then return TRUE
  // else return FALSE
#ifdef DDR5_SUPPORT
  return TRUE;
#else
  return FALSE;
#endif //DDR5_SUPPORT
} // IsDdr5MemSsPresent

/**
  Detects whether Bretton Sound is present

  @param[in]    Host        - Pointer to sysHost structure
  @param[in]    Socket      - Socket number

  @retval TRUE  BRS is present
  @retval FALSE BRS is not present
**/
BOOLEAN
EFIAPI
IsBrsPresent (
  IN        PSYSHOST            Host,
  IN        UINT32              Socket
  )
{
#if defined (DDR5_SUPPORT) && !defined (SPR_HOST) //SPR_TODO. Will remove SPR_HOST once SPR MC IP LIB is created.
  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL) && IsDdr5MemSsPresent(Host, Socket)) {
    return TRUE;
  }
#endif
  return FALSE;
} // IsBrsPresent


/**
  Detects whether Denton Creek (DNT) is present

  @param[in]    Host        - Pointer to sysHost structure
  @param[in]    Socket      - Socket number

  @retval TRUE  DNT is present
  @retval FALSE DNT is not present
**/
BOOLEAN
EFIAPI
IsDntPresent (
  IN        PSYSHOST            Host,
  IN        UINT32              Socket
  )
{
#if defined (DDR5_SUPPORT) && defined (DNT_HOST)
  //
  // DNT doesn't have Core; CPU ID cannot be used to determine presence.
  // If DNT_HOST is enabled (used only for DNT targets), return TRUE, else FALSE
  //
  return TRUE;
#else
  return FALSE;
#endif // DNT_HOST
} // IsDntPresent
