/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2020 Intel Corporation. <BR>

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

#include <IndustryStandard/SmBios.h>
#include <Chip/Include/SysHostChip.h>
#include <Protocol/PcatProtocol.h>
#include <Library/MemFmcIpLib.h>
#include <Protocol/AcpiPlatformProtocol.h>
#include <Library/MemTypeLib.h>

#include <Library/MemDecodeLib.h>

#include <Library/PmcLib.h>
#include <Library/PchPcrLib.h>

#include "CrystalRidge.h"
#include <Protocol/SmbiosMemInfo.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/Mesh2MemIpLib.h>

#include "RdRand.h"
#include "Dsm.h"
#include "Fis.h"
#include "Nfit.h"
#include "Pcat.h"
#include "XlateFunctions/XlateFunctions.h"
#include "Ars/ArsFlows.h"
#include "Ars/ArsErrorInject.h"
#include <UncoreCommonIncludes.h>
#include <Library/KtiApi.h>
#include <Library/TimerLib.h>
#include <Chip/Include/FnvAccessCommon.h>
#include <Library/CrystalRidgeFlushNearMemoryLib.h>
#include "Ars/ArsPatrolScrubber.h"
#include <Protocol/SmmCpu.h>
#include "NvdimmAcpiConfig.h"
#include "FlushCacheLineOpt.h"
#include <Register/Cpuid.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/BaseLib.h>
#include <Guid/SocketProcessorCoreVariable.h>
#include "NvdimmSmbusAccess.h"
#include <Protocol/MmMp.h>
#include "AcpiTableHandle.h"
#include "NfitTableUpdate.h"
#include <Library/CpuAndRevisionLib.h>
#include <Library/SystemInfoLib.h>

#include "FwActivate.h"

/*****************************************************************************
 * Local definitions
 *****************************************************************************/
#define MAX_CPU_THREADS 512
#define MAX_INST_ID 6

//
// Globals
//
#ifdef SMM
SPIN_LOCK                               mSmmDebugLock;
#endif
struct SystemMemoryMapHob               *mSystemMemoryMap;
CR_INFO                                 mCrInfo;
EFI_SMM_BASE2_PROTOCOL                  *mSmmBase = NULL;
EFI_SMM_SYSTEM_TABLE2                   *mSmst = NULL;
EFI_MP_SERVICES_PROTOCOL                *mMpService;
EFI_MM_MP_PROTOCOL                      *mSmmMp;
EFI_CPU_CSR_ACCESS_PROTOCOL             *mCpuCsrAccess;
EFI_SMM_CPU_PROTOCOL                    *mSmmCpu;
EFI_NVDIMM_SMBUS_SMM_INTERFACE_PROTOCOL *mNvdimmSmbusSmmInterfaceProtocol;
EFI_EADR_PROTOCOL                       *mEadrProtocol;
BOOLEAN                                 mInSmm;
NVDIMM_ACPI_SMM_INTERFACE               *mNvdimmAcpiSmmInterface = NULL;
NVDIMM_SMBUS_SMM_INTERFACE              *mNvdimmSmbusSmmInterface = NULL;
BOOLEAN                                 mUseOSMailbox = FALSE; // Specifies whether to use BIOS Mailbox or OS Mailbox
UINT8                                   *mLargePayloadBuffer;
BOOLEAN                                 mUseWpqFlush = FALSE;
UINT64                                  mWpqFlushHintAddress[MAX_SOCKET][MAX_CH];
STATIC UINTN                            mCacheLineSize;
MEMORY_MAP_HOST                         *mMemMapHost;
UINTN                                   mAdwbFlushTaskCount = 0;
UINT8                                   *mNvdimmAcpiSmmInterfaceInputBuffer;

BOOLEAN                                 mCpuThreadState[MAX_CPU_THREADS]; // Contains state of all threads of all processors in the system
UINT16                                  mCpuThreadNum = 0;     // The actual size of mCpuThreadState table

/******************************************************************************
 * Functions
 ******************************************************************************/

/**
  @brief Return position of next bit set in a bitmap.

  @param[in] Bitmap   Bitmap of to look for bit
  @param[in] StartAt  Bit position to start at

  @return The position of bit found in Bitmap, or FFh if not found.
**/
UINT8
BitmapGetBit (
  IN UINT32 Bitmap,
  IN UINT8  StartAt
  )
{
  for (Bitmap >>= StartAt; Bitmap != 0; Bitmap >>= 1, StartAt++) {

    if (Bitmap & 1) {

      return StartAt;
    }
  }
  return 0xFF;
}


/**
  Returns mailbox for given Dimm.

  @param NvmDimm   - NVDIMM structure, describing current Dimm
  @param UseOSMailbox - If TRUE, use the OS mailbox. BIOS mailbox otherwise.

  @return Pointer to CR Mailbox for given NVDIMM
**/
CR_MAILBOX *
GetMailbox (
  NVDIMM    *NvmDimm,
  BOOLEAN   UseOSMailbox
  )
{
  CR_MAILBOX *Mailbox;

  if (UseOSMailbox) {
    Mailbox = &NvmDimm->OSMailboxSPA;
  } else {
    Mailbox = &NvmDimm->MailboxSPA;
  }
  if (Mailbox->pCommand == 0) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: Mailbox Command Register SPA is 0!", NvmDimm->SocketId, NvmDimm->Ch, NvmDimm->Dimm));
    ASSERT (FALSE);
  }
  return Mailbox;
}


/**
  Utility function to check whether given DIMM is using FNV or EKV controller.
  In the case Subsystem DID is not programmed, assuming older gen.

  @param NvmDimm - NVDIMM structure, describing current Dimm

  @return        - TRUE if FNV or EKV or SubsystemDeviceID == 0x0
**/
BOOLEAN
IsNvmCtrlFirstGen (
  NVDIMM *NvmDimm
  )
{
  if (NvmDimm->SubsystemDeviceID == 0x0) {
    CRDEBUG ((DEBUG_ERROR, CR_WARN_STR ("CRP") "Subsystem Device ID is 0x0! This could lead to potential problems!\n"));
    return TRUE;
  } else if ((NvmDimm->SubsystemDeviceID == FNV) || (NvmDimm->SubsystemDeviceID == EKV) || (NvmDimm->SubsystemDeviceID == BWV)) {
    return TRUE;
  }
  return FALSE;
}

/**
  @brief Returns index of the slot where NVM DIMM was found in given channel.

  @param[in]  Socket      - Socket Number
  @param[in]  Ch          - DDR Channel ID
  @param[out] DimmPtr     - DIMM slot number in the channel

  @retval EFI_SUCCESS           NVM DIMM found, index stored at 'DimmPtr'.
  @retval EFI_INVALID_PARAMETER 'DimmPtr' is NULL.
  @retval EFI_NOT_FOUND         No NVM DIMM found in channel 'Ch'.

**/
EFI_STATUS
GetChannelDimm (
  IN  UINT8  Socket,
  IN  UINT8  Ch,
  OUT UINT8 *DimmPtr
  )
{
  UINT8      Dimm;

  if (DimmPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    if (IsNvmDimm (Socket, Ch, Dimm)) {

      *DimmPtr = Dimm;
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
}

/**
  @brief Get the channel ways number for given SAD index and socket.

  @param[in] Socket   - socket number (0 based)
  @param[in] SadIndex - SAD index

  @return Channel ways
*/
UINT8
GetChWays (
  IN UINT8  Socket,
  IN UINT8  SadIndex
  )
{
  UINT8     ChWays;
  UINT8     McIndex;
  UINT8     Bitmap;

  //
  // Determine which IMC channel bitmap to use
  //
  McIndex = GetMcIndexFromBitmap (mSystemMemoryMap->Socket[Socket].SAD[SadIndex].imcInterBitmap);
  ChWays = 0;
  for (Bitmap = mSystemMemoryMap->Socket[Socket].SAD[SadIndex].FMchannelInterBitmap[McIndex]; Bitmap != 0; Bitmap >>= 1) {

    if (Bitmap & 1) {
      ChWays++;
    }
  }
  return ChWays;
}

/**
  This function returns the base address of reserved control region for NGN dimm

  @param[in] Socket     Socket index
  @param[in] Mc         Memory controller index
  @param[in] Offset     DPA Offset

  @retval  Base address of NGN reserved control region
**/
UINT64
GetNgnControlRegionReservedBase (
  IN UINT8  Socket,
  IN UINT8  Mc,
  IN UINT64 Offset
  )
{
  UINT8           Ch;
  UINT8           Dimm;
  UINT8           NumChannelPerMc;
  NVDIMM          *DimmPtr;
  UINT64          NgnControlRegionReservedBase;
  EFI_STATUS      Status;

  Dimm = 0;
  DimmPtr = NULL;
  NumChannelPerMc = GetNumChannelPerMc ();
  NgnControlRegionReservedBase = 0;

  for (Ch = (Mc * NumChannelPerMc); Ch < ((Mc * NumChannelPerMc) + NumChannelPerMc); Ch++) {
    //
    // Find the dimm slot that has the NVDIMM in this channel.
    //
    Status = GetChannelDimm (Socket, Ch, &Dimm);
    if (EFI_ERROR (Status)) {
      continue;
    }

    //
    // Get the NVDIMM struct for the corresponding dimm slot.
    //
    DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED);

    //
    // Call the XlateDpaOffsetToSpa to find the SPA of offset in the DDRT_CTRL_DPA_START.
    //
    if (DimmPtr != NULL) {
      Status = XlateDpaOffsetToSpa (DimmPtr, DimmPtr->CntrlBase, Offset, &NgnControlRegionReservedBase);
      ASSERT_EFI_ERROR (Status);
      break;
    }
  }

  return NgnControlRegionReservedBase;
}


/**
  @brief This function programs Flush hint address in CPU and IMC

  @return Void.
**/
VOID
ProgramFlushHintAddressForWpqFlush (
  VOID
  )
{
  UINT8          ScktId;
  UINT8          McId;
  UINT8          ChId;
  UINT8          MaxImc;
  UINT8          NumChannelPerMc;
  UINT64         HintAddress[MAX_MC_CH];
  UINT64         McNgnControlRegionReservedBase;

  MaxImc = GetMaxImc ();
  NumChannelPerMc = GetNumChannelPerMc ();

  for (ScktId = 0; ScktId < MAX_SOCKET; ScktId++) {

    for (McId = 0; McId < MaxImc; McId++) {
      McNgnControlRegionReservedBase = GetNgnControlRegionReservedBase (ScktId, McId, FLUSH_HINT_ADDRESS_OFFSET);

      for (ChId = 0; ChId < NumChannelPerMc; ChId++) {
        HintAddress[ChId] = McNgnControlRegionReservedBase;
        mMemMapHost->SocketInfo[ScktId].ChannelInfo[ChId + McId * NumChannelPerMc].WpqFlushHintAddress = HintAddress[ChId];

        if (HintAddress[ChId] != 0) {
          CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Socket%d:Mc%d:Ch%d WpqFlushHintAddress = 0x%llX\n",
            ScktId, McId, ChId, HintAddress[ChId]));
        }
      }

      //
      // 14nm only need to program PAD0 as 16 addresses are programmed in PAD0 with 4K interleave across MC for control region.
      //
      ProgramM2mPcommit (ScktId, McId, HintAddress);
    }
  }
} // ProgramFlushHintAddressForWpqFlush()


/**

  This function caches the flush hint addresses from mMemMapHost to a module variable
  to ensure they are available in SMM.

  @param VOID

  @retval VOID

**/
VOID
CacheFlushHintAddress (
  VOID
  )
{
  UINT8   ScktId;
  UINT8   ChId;
  UINT8   MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  for (ScktId = 0; ScktId < MAX_SOCKET; ScktId++) {
    for (ChId = 0; ChId < MaxChDdr; ChId++) {
      mWpqFlushHintAddress[ScktId][ChId] = mMemMapHost->SocketInfo[ScktId].ChannelInfo[ChId].WpqFlushHintAddress;
    }
  }
}


/**
 @brief This function programs MSR 0x6D based on FastGoConfig setup option

 @return Void.
**/
VOID
ProgramFastGo (
  VOID
  )
{
  SPR_MSR_FEATURE_TUNING_1_REGISTER                Msr6dRegSpr;
  ICX_MSR_FEATURE_TUNING_1_REGISTER                Msr6dRegIcx;
  SKX_MSR_QOS_APP_DIRECT_MODE_BW_THRTL_REGISTER    Msr6dRegSkx;

  //
  // Simics does not implement this MSR, must check for simics to avoid exception.
  //
  if ((GetEmulation () & SIMICS_FLAG) != 0) {
    return;
  }

  //
  // Program MSR 0x6D to enable/disable FastGo based on FastGoConfig setup option.
  //
  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    Msr6dRegSpr.Uint64 = AsmReadMsr64 (SPR_MSR_FEATURE_TUNING_1);
    Msr6dRegSpr.Bits.DisableFastgo = mSystemMemoryMap->MemSetup.FastGoConfig;
    AsmWriteMsr64 (SPR_MSR_FEATURE_TUNING_1, Msr6dRegSpr.Uint64);
  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    Msr6dRegIcx.Uint64 = AsmReadMsr64 (ICX_MSR_FEATURE_TUNING_1);
    Msr6dRegIcx.Bits.DisableFastgo = mSystemMemoryMap->MemSetup.FastGoConfig;
    AsmWriteMsr64 (ICX_MSR_FEATURE_TUNING_1, Msr6dRegIcx.Uint64);
  } else {
    //
    // This register is Write Only.
    //
    Msr6dRegSkx.Uint64 = 0;
    Msr6dRegSkx.Bits.OptimizeValue = mSystemMemoryMap->MemSetup.FastGoConfig;
    AsmWriteMsr64 (SKX_MSR_QOS_APP_DIRECT_MODE_BW_THRTL, Msr6dRegSkx.Uint64);
  }
}

/**
  @brief This function changes the FastGoConfig setup based on the presence of block or PMEM regions.

  @return Void.
**/
VOID
FindCrQosAutoSetting (
  VOID
  )
{
  UINT8 Socket;
  UINT8 SadIdx;

  if (mSystemMemoryMap->MemSetup.FastGoConfig == CR_FASTGO_AUTOMATIC) {
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Finding QoS - FastGoConfig:%d -> ", mSystemMemoryMap->MemSetup.FastGoConfig));
    //
    // FastGo Config Auto (ICX/SPR) -> enabled
    //
    if (IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL) ||
        IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
      mSystemMemoryMap->MemSetup.FastGoConfig = CR_FASTGO_DEFAULT;
    } else {
      //
      // FastGo Config Auto (CPX) -> enabled only when no PMEM region found
      //
      mSystemMemoryMap->MemSetup.FastGoConfig = CR_FASTGO_DEFAULT;
      //
      // Check SAD rules for all sockets
      //
      for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
        //
        // Iterate through SAD rules to check if PMEM region exist
        //
        for (SadIdx = 0; SadIdx < MAX_SAD_RULES; SadIdx++) {
          //
          // Skip disabled and remote SAD rules
          //
          if ((mSystemMemoryMap->Socket[Socket].SAD[SadIdx].Enable == 0) ||
              (mSystemMemoryMap->Socket[Socket].SAD[SadIdx].local == 0)) {
            continue;
          }
          //
          // Check if PMEM regions is present
          //
          if (IsMemTypeAppDirect (mSystemMemoryMap->Socket[Socket].SAD[SadIdx].type)) {
            //
            // PMEM region found, disable FastGo
            //
            mSystemMemoryMap->MemSetup.FastGoConfig = CR_FASTGO_DISABLE;
            break;
          }
        }
      }
    }
    CRDEBUG ((DEBUG_INFO, "%d\n", mSystemMemoryMap->MemSetup.FastGoConfig));
  }
}

/**

Routine Description: ProgramCrQosForAllCores - This function programs FastGo in all CPU cores

  @param VOID
  @retval VOID

**/
VOID
ProgramCrQosForAllCores (
  VOID
  )
{
  FindCrQosAutoSetting ();

  if (mSystemMemoryMap->MemSetup.FastGoConfig >= CR_FASTGO_LAST_OPTION) {
    return;
  }

  //
  // Contact CLV before changing the trace below, test depends on it.
  //
  CRDEBUG ((EFI_D_ERROR, CR_INFO_STR ("CRP") "Programming FastGo optimization to 0x%X\n", mSystemMemoryMap->MemSetup.FastGoConfig));

  ProgramFastGo ();

  mMpService->StartupAllAPs (
    mMpService,
    (EFI_AP_PROCEDURE)ProgramFastGo,
    FALSE,
    NULL,
    0,
    NULL,
    NULL
    );
}

/**

Routine Description: ProgramL2RfoDisable - This function programs MSR 0x972[3] (for SKX) or MSR 0x6D (for ICX) based on L2RfoPrefetchDisable setup option

  @param VOID
  @retval   TRUE if known CPU is detected (SKX or ICX)
  @retval   FALSE if unknown CPU is detected

**/
BOOLEAN
ProgramL2RfoDisable (
  VOID
  )
{
  ICX_MSR_FEATURE_TUNING_1_REGISTER FeatureTuning1;
  UINT64                            MsrData;

  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    // Program MSR 0x972[3] (L2 RFO Disable) based on L2RfoPrefetchDisable setup option
    if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
      MsrData = AsmReadMsr64 (MSR_VIRTUAL_MSR_LLC_PREFETCH);
      MsrData |= B_RFO_TRAIN_DISABLE;
    } else {
      MsrData = B_RFO_TRAIN_DISABLE;
    }
    AsmWriteMsr64(MSR_VIRTUAL_MSR_LLC_PREFETCH, MsrData);
    return TRUE;
  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    FeatureTuning1.Uint64 = AsmReadMsr64 (ICX_MSR_FEATURE_TUNING_1);
    FeatureTuning1.Bits.RfoTrainDisable = 1;
    // Program MSR 0x6D (L2 RFO Disable) based on L2RfoPrefetchDisable setup option
    AsmWriteMsr64 (ICX_MSR_FEATURE_TUNING_1, FeatureTuning1.Uint64);
    return TRUE;
  } else {
    return FALSE;
  }
}


/**

Routine Description: ProgramL2RfoDisableForAllCores - This function calles ProgramL2RfoDisable function which programs
MSR 0x972[3] (for SKX) or MSR 0x6D (for ICX) based on L2RfoPrefetchDisable setup option.

  @param VOID
  @retval VOID

**/
VOID
ProgramL2RfoDisableForAllCores (
  VOID
  )
{
  BOOLEAN ProgramStatus;

  //
  // Contact CLV before changing the trace below, test depends on it.
  //
  CRDEBUG ((EFI_D_ERROR, CR_INFO_STR ("CRP") "Programming L2 RFO Disable\n"));

  ProgramStatus = ProgramL2RfoDisable ();

  if (ProgramStatus) {
    mMpService->StartupAllAPs (
        mMpService,
        (EFI_AP_PROCEDURE) ProgramL2RfoDisable,
        FALSE,
        NULL,
        0,
        NULL,
        NULL
      );
  } else {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("CRP") "Unknown CPU type. Expected SKX or ICX.\n"));
  }
}

/**

Routine Description: CreateDimmPresentBitmaps - This function
basically creates 2 64-bit fields that are part of the
AcpiSMMInterface. The first 64-bit field is for socket 0-3 and
the second 64-bit field is for socket 3-7. This field contains
bit that are set in the corresponding position when an
NVMDIMM is present in that position. Least significant 16 bits
(of the 1st field) represents socket0 and the most signifFicant
16-bit is for socket3 and the other sockets in between. For
example, if the system has 2 sockets and there is an NVMDIMM in
Dimm0 of all channels, then the bitmap would look like below:
0000000000000000000000000000000000000101010101010000010101010101b

  @param VOID         - None
  @retval VOID        - None

**/
VOID
CreateDimmPresentBitmaps (
  VOID
  )
{
  UINT64       DimmBitmap1 = 0;
  UINT64       DimmBitmap2 = 0;
  UINT8        Socket;
  UINT8        Ch;
  UINT8        Dimm;
  UINT8        MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (IsNvmDimm (Socket, Ch, Dimm)) {
          if (Socket < 4) {
            DimmBitmap1 |= ((UINT64)0x1 << ((Socket * 16) + (Ch * 2) + Dimm));
          } else {
            DimmBitmap2 |= ((UINT64)0x1 << ((Socket * 16) + (Ch * 2) + Dimm - 64));
          }
        }
      }
    }
  }
  mNvdimmAcpiSmmInterface->NvDimmConfigBitmap0 = DimmBitmap1;
  mNvdimmAcpiSmmInterface->NvDimmConfigBitmap1 = DimmBitmap2;
}

/**
  @brief Returns channel interleve ways for given socket, iMC, and SAD index.

  @param[in] Socket   - socket number
  @param[in] Imc      - memory controller number
  @param[in] SadInx   - SAD index

  @return Ways of channel interleave for socket/sad
**/
UINT8
GetChInterleave (
  IN UINT8 Socket,
  IN UINT8 Imc,
  IN UINT8 SadInx
  )
{
  UINT8       ChWays;
  UINT8       Bitmap;
  UINT8       MaxImc;

  MaxImc = GetMaxImc ();

  if (Socket >= MAX_SOCKET || Imc >= MaxImc || SadInx >= MAX_SAD_RULES) {
    return 0;
  }
  Bitmap = mSystemMemoryMap->Socket[Socket].SAD[SadInx].FMchannelInterBitmap[Imc];
  for (ChWays = 0; Bitmap != 0; Bitmap >>= 1) {

    if (Bitmap & 1) {

      ChWays++;
    }
  }

  return ChWays;
}

/**
  @brief Returns interleave channel bitmap for given socket and SAD.

  @param[in] Socket   - socket number
  @param[in] SadInx   - SAD number

  @return Channel bitmap for given socket/SAD. Counting from BIT0 - channel 1 of first iMC,
          BIT1 - channel 2 of first iMC, etc.
**/
UINT32
GetChInterleaveBitmap (
  UINT8 Socket,
  UINT8 SadInx
  )
{
  UINT32            InterleaveBitmap = 0;
  struct SADTable   *Sad;
  UINT8             i;
  UINT8             MaxImc;
  UINT8             NumChannelPerMc;

  if (Socket >= NELEMENTS (mSystemMemoryMap->Socket) || SadInx >= NELEMENTS (mSystemMemoryMap->Socket[Socket].SAD)) {
    return 0;
  }

  Sad = &mSystemMemoryMap->Socket[Socket].SAD[SadInx];
  MaxImc = GetMaxImc ();
  NumChannelPerMc = GetNumChannelPerMc ();

  // check which iMC is involved in the interleaving set
  for (i = 0; i < MaxImc; i++) {
    if (mSystemMemoryMap->Socket[Socket].SadIntList[SadInx][(Socket * MaxImc) + i])  {
      InterleaveBitmap |= Sad->FMchannelInterBitmap[i] << (NumChannelPerMc * i);
    }
  }
  return InterleaveBitmap;
}

/**

  Routine Description: This helper function returns interleave ways for the
  given PMEM region.

  @param[in] SpaBaseAddr  - Spa Start Address of the SAD Rule

  @return Interleave ways for PMEM region or 0 if error

**/
UINT8
GetInterleaveWaysForPmemRgn (
  UINT64 SadBase
  )
{
  INT32 Index;

  Index = GetPmemIndex (&mCrInfo.NvdimmInfo, SadBase);

  if (Index < 0) {
    return 0;
  }

  return mCrInfo.NvdimmInfo.PMEMInfo[Index].ChWays * mCrInfo.NvdimmInfo.PMEMInfo[Index].iMCWays;
}


/**
  @brief Gets the PMEM DPA base address for given NVDIMM using the
         SPA base address for the relevant SAD of type PMEM.

  @param[in] SadSpaBase  - Base Address of the SAD Rule found
  @param[in] Socket      - Socket Number
  @param[in] Ch          - Channel number
  @param[in] Dimm        - Dimm number

  @return DPA base address or 0 in case of failure
**/
UINT64
GetPmemDpaBase (
  UINT64  SadSpaBase,
  UINT8   Socket,
  UINT8   Ch,
  UINT8   Dimm
  )
{
  EFI_STATUS  Status;
  UINT64      DpaBase = 0;
  UINT64      SadLimit;
  UINT32      DpaOffset = 0;
  UINT8       Index;
  UINT8       MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  if (Socket >= MAX_SOCKET || Ch >= MaxChDdr || Dimm >= MAX_DIMM) {

    ASSERT (FALSE);
    return 0;
  }
  Status = CrGetSadInfo (SadSpaBase, &Index, NULL, &SadLimit, NULL);
  if (EFI_ERROR (Status) || Index != Socket) {
    //
    // If not mapped or mapped in some other socket return 0.
    //
    return 0;
  }
  SadLimit = MEM_BYTES_TO_64MB (SadLimit);

  for (Index = 0; Index < MAX_UNIQUE_NGN_DIMM_INTERLEAVE; Index++) {

    if ((UINT32)SadLimit == mSystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].perDPAMap[Index].SPALimit) {

      DpaOffset = mSystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].perDPAMap[Index].perRegionDPAOffset;
      break;
    }
  }
  DpaBase = MEM_64MB_TO_BYTES ((UINT64)mSystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].perRegionDPA + DpaOffset);

  return DpaBase;
}

/**
  Given SAD base address, function returns socket
  granularity based on the SAD Rule.

  @param[in]  SadBaseAddr  SAD base address

  @return Socket granularity

**/
UINT32
GetSktGranularityFromSadBaseAddr (
  UINT64 SadBaseAddr
  )
{
  UINT8      Skt;
  UINT8      Sad;
  UINT32     SktGran = 0;
  UINT64     SadBase;

  for (Skt = 0; Skt < NELEMENTS (mSystemMemoryMap->Socket); Skt++) {

    for (Sad = 0; Sad < NELEMENTS (mSystemMemoryMap->Socket[Skt].SAD); Sad++) {

      if (!mSystemMemoryMap->Socket[Skt].SAD[Sad].local) {
        continue;
      }
      SadBase = NvdimmGetSadBase (Skt, Sad);
      if (SadBase == SadBaseAddr) {
        SktGran = GetSocketGranularity (&mSystemMemoryMap->Socket[Skt].SAD[Sad]);
        break;
      }
    }
  }
  return SktGran;
} // GetSktGranularityFromSadBaseAddr ()

/**
  @brief This function finds SAD rule for a SPA address and returns basic info about it.

  @param[in]  SpaAddress  - SPA address to find matching SAD rule
  @param[out] SktPtr      - Buffer for id of the socket where SAD rule was found
  @param[out] SpaBasePtr  - Buffer for start address of the SAD rule
  @param[out] SpaLimitPtr - Buffer for end address of the SAD rule
  @param[out] MemTypePtr  - Buffer for type of memory this rule provides.

  @retval EFI_SUCCESS     - Local SAD rule found, info updated.
  @retval EFI_NO_MAPPING  - No matching SAD rule found, output variables not changed.
**/
EFI_STATUS
CrGetSadInfo (
  IN  UINT64    SpaAddress,
  OUT UINT8    *SktPtr,
  OUT UINT64   *SpaBasePtr,
  OUT UINT64   *SpaLimitPtr,
  OUT MEM_TYPE *MemTypePtr
  )
{
  UINT64         SadBase;
  UINT64         SadLimit;
  UINT8          Skt;
  UINT8          Sad;

  for (Skt = 0; Skt < NELEMENTS (mSystemMemoryMap->Socket); Skt++) {

    if (!mSystemMemoryMap->Socket[Skt].SocketEnabled) {
      continue;
    }
    for (Sad = 0; Sad < NELEMENTS (mSystemMemoryMap->Socket[Skt].SAD); Sad++) {

      if (!mSystemMemoryMap->Socket[Skt].SAD[Sad].Enable ||
          !mSystemMemoryMap->Socket[Skt].SAD[Sad].local) {
        continue;
      }
      SadBase = MEM_64MB_TO_BYTES (mSystemMemoryMap->Socket[Skt].SAD[Sad].Base);
      SadLimit = MEM_64MB_TO_BYTES (mSystemMemoryMap->Socket[Skt].SAD[Sad].Limit);

      if (SadBase <= SpaAddress && SpaAddress < SadLimit) {

        if (SktPtr != NULL) {
          *SktPtr = Skt;
        }
        if (SpaBasePtr != NULL) {
          *SpaBasePtr = SadBase;
        }
        if (SpaLimitPtr != NULL) {
          *SpaLimitPtr = SadLimit;
        }
        if (MemTypePtr != NULL) {
          *MemTypePtr = mSystemMemoryMap->Socket[Skt].SAD[Sad].type;
        }
        return EFI_SUCCESS;
      }
    } // for (Sad...)
  } // for (Skt...)
  return EFI_NO_MAPPING;
} // CrGetSadInfo()


/**

  Routine Description: This function returns Socket and Channel Information
  of a dimm given its virtual socket and channel information that the alogrithm uses.
  This routine handles both Control Region and PMEM regions as well.

  @param[in]  SpaStartAddr - Spa Start Address of the SAD Rule
  @param[in]  IsPmem       - TRUE for PMEM region type, FALSE for Cntrl Region type
  @param[in]  ChWays       - Interleave channel ways
  @param[in]  VirtualSkt   - Virtual Socket number
  @param[in]  VirtualImc   - Virtual Imc number
  @param[in]  VirtualCh    - Virtual Channel number
  @param[out] *Socket      - This pointer will have socket number
  @param[out] *Channel     - This pointer will hold channel number

  @return Status      - Status

**/
EFI_STATUS
VirtualToPhysicalDimmInfo (
  UINT64      SpaStartAddr,
  BOOLEAN     IsPmem,
  UINT8       ChWays,
  UINT8       VirtualSkt,
  UINT8       VirtualImc,
  UINT8       VirtualCh,
  UINT8       *Socket,
  UINT8       *Channel
  )
{
  UINT8            i;
  UINT8            Limit;
  UINT8            Index;
  UINT8            MaxImc;
  UINT64           BaseAddr;
  PMEM_INFO        *Pmem;
  NVDIMM           *NvmDimm;

  MaxImc = GetMaxImc ();

  if (VirtualSkt >= MAX_SOCKET ||
      VirtualImc >= MaxImc     ||
      ChWays     == 0          ||
      ChWays     >  CH_3WAY    ||
      VirtualCh  >= ChWays     ||
      Socket     == NULL       ||
      Channel    == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Limit = IsPmem ? mCrInfo.NvdimmInfo.NumPMEMRgns : mCrInfo.NvdimmInfo.NumNvdimms;
  Index = Limit;

  for (i = 0; i < Limit; i++) {
    BaseAddr = IsPmem ? mCrInfo.NvdimmInfo.PMEMInfo[i].SADPMemBase : mCrInfo.NvdimmInfo.Nvdimms[i].SADSpaBase;

    //
    // when given SPA start address is equal to base address from structure,
    // we have our starting index
    //
    if (BaseAddr == SpaStartAddr) {
      //
      // Index number of pmem or NVDIMM structure is counted using this equation
      //   (channel, iMC and socket numbers are 0 based):
      //
      // starting index + channel number + channel ways * iMC number +
      //   channel ways * max iMCs per socket * socket number
      //
      Index = i + VirtualCh + ChWays * (VirtualImc + MaxImc * VirtualSkt);
      break;
    }
  }

  if (Index >= Limit) {
    return EFI_INVALID_PARAMETER;
  }

  if (IsPmem) {
    Pmem = &mCrInfo.NvdimmInfo.PMEMInfo[Index];
    *Socket = Pmem->Socket;
    *Channel = Pmem->Channel;
  } else {
    NvmDimm = &mCrInfo.NvdimmInfo.Nvdimms[Index];
    *Socket = (UINT8) NvmDimm->SocketId;
    *Channel = NvmDimm->Ch;
  }

  return EFI_SUCCESS;
}


/**
  Routine Description: This function is responsible for getting Socket,
  Channel and Dimm information based on the given System Physical Address.
  That is, to figure out the NVDIMM that is mapped to the passed in SPA.

  @param[in]  Spa          - SPA address
  @param[out] *SpaBasePtr  - Base address of the region where SPA belongs
  @param[out] *SktPtr      - Socket that this function will return
  @param[out] *ChPtr       - Channel that this function will return
  @param[out] *DimmPtr     - Dimm that this function will return
  @param[out] *MemTypePtr  - Memory type of the SPA address

  @return EFI status is returnd.
**/
EFI_STATUS
GetDimmInfoFromSpa (
  IN  UINT64       Spa,
  OUT UINT64      *SpaBasePtr,
  OUT UINT8       *SktPtr,
  OUT UINT8       *ChPtr,
  OUT UINT8       *DimmPtr,
  OUT UINT16      *MemTypePtr
  )
{
  EFI_STATUS       Status = EFI_INVALID_PARAMETER;
  UINT8            Socket;
  UINT8            iMCWays;
  UINT8            Sad;
  UINT8            ChWays;
  UINT16           SadType = MemTypeNone;
  UINT64           SpaStart;
  UINT64           SpaEnd;
  UINT32           LineSize;
  UINT32           iMCSize;
  DPA_ADDRESS      Dpa;
  SAD_TABLE       *SadPtr;

  if (Spa >= mCrInfo.CrbdAddrBase && Spa <  mCrInfo.CrbdAddrLimit) {

    SadType = MemType1lmCtrl;
    SpaStart = mCrInfo.CrbdAddrBase;
    *SktPtr = CRBD_SPA_TO_SKT (Spa);
    *ChPtr = ConvertImcChToSktCh (CRBD_SPA_TO_IMC (Spa), CRBD_SPA_TO_CH (Spa));
    Status = EFI_SUCCESS;
    goto GetOut;
  }
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Traverse thru all SAD entries to check for the SPA being in the range of one
    //
    for (Sad = 0; Sad < MAX_SAD_RULES; Sad++) {

      SadPtr = &mSystemMemoryMap->Socket[Socket].SAD[Sad];
      if (SadPtr->Enable == 0 || SadPtr->local == 0) {
        continue;
      }
      SpaStart = NvdimmGetSadBase (Socket, Sad);
      SpaEnd = (UINT64)SadPtr->Limit << BITS_64MB_TO_BYTES;

      if (Spa >= SpaStart && Spa < SpaEnd) {
        //
        // SPA falls within this SAD SPA range, check SAD type.
        //
        if (SadPtr->type != MemType2lmDdrCacheMemoryMode &&
            SadPtr->type != MemType1lmAppDirect &&
            SadPtr->type != MemType1lmCtrl) {
          //
          // Return if it is not one of memory types we handle.
          //
          return EFI_NOT_FOUND;
        }
        //
        // Figure out to which DIMM this SPA is mapped.
        //
        SadType = SadPtr->type;
        iMCWays = GetImcWays (Socket, Sad);
        ChWays = GetChWays (Socket, Sad);
        LineSize = GetChGranularityFromSadBaseAddr (SpaStart);
        iMCSize = GetSocketGranularity (SadPtr);

        ZeroMem (&Dpa, sizeof (Dpa));
        Status = XlateSpaToDpaAddress (SpaStart, Spa, iMCWays, ChWays, LineSize, iMCSize, &Dpa);
        if (!EFI_ERROR(Status)) {

          Status = VirtualToPhysicalDimmInfo (SpaStart, IsPmemRgn (&mCrInfo.NvdimmInfo, SpaStart), ChWays, Dpa.Skt, Dpa.Imc, Dpa.Ch, SktPtr, ChPtr);
        }
        goto GetOut;
      }
    } // for (Sad...)
  } // for (Socket...)

GetOut:
  if (!EFI_ERROR (Status)) {

    Status = GetChannelDimm (*SktPtr, *ChPtr, DimmPtr);
    if (!EFI_ERROR (Status)) {

      *SpaBasePtr = SpaStart;
      if (MemTypePtr != NULL) {

        *MemTypePtr = SadType;
      }
      CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "Found DIMM info from SPA (0x%012llX), Base = 0x%012llX", Spa, *SpaBasePtr));
      CRDEBUG ((DEBUG_INFO, ", S%d.C%d.D%d, SadType = %d\n", *SktPtr, *ChPtr, *DimmPtr, SadType));
    }
  }
  return Status;
} // GetDimmInfoFromSpa()


/**

Routine Description: GetSPARegIndex will return the index of the
NVMDIMM Mailbox Register (in SPA) from the pDimm structure.

arguments.

  @param pDimm       - Pointer to NVDIMM structure, this will
                     have the Dimm struct.
  @param RegSPA      - Register that is in MBSpa.
  @retval Index of the register if found, else -1.

**/
INT16
GetSPARegIndex (
  NVDIMM    *pDimm,
  UINT64    RegSPA
  )
{
  CR_MAILBOX *MBSpa;

  MBSpa = &pDimm->MailboxSPA;

  if((UINT64)MBSpa->pCommand == RegSPA) {
    return (MB_REG_COMMAND);
  } else if ((UINT64)MBSpa->pNonce0 == RegSPA) {
    return (MB_REG_NONCE0);
  } else if ((UINT64)MBSpa->pNonce1 == RegSPA) {
    return (MB_REG_NONCE1);
  } else if ((UINT64)MBSpa->pStatus == RegSPA) {
    return (MB_REG_STATUS);
  }

  return (-1);
}

/**

Routine Description: GetDpaFromRegIndex will return the NVMDIMM
Mailbox Register (in DPA) for the corresponding Index in NVMDIMM
Mailbox in SPA from the pDimm structure.

arguments.

  @param pDimm       - Pointer to NVDIMM structure, this will
                       have the Dimm struct.
  @param Index       - Index of the register in Spa.
  @param *RDpa       - pointer that will contain the Dpa
                       Register.
  @retval EFI_STATUS - Status of the conversion effort

**/
EFI_STATUS
GetDpaFromRegIndex (
  NVDIMM    *pDimm,
  INT16     Index,
  UINT64    *RDpa
  )
{
  CR_MAILBOX *MBDpa;
  EFI_STATUS Status = EFI_SUCCESS;

  MBDpa = &pDimm->MailboxDPA;

  if (Index == MB_REG_COMMAND) {
    *RDpa = (UINT64) MBDpa->pCommand;
  } else if (Index == MB_REG_NONCE0) {
    *RDpa = (UINT64) MBDpa->pNonce0;
  } else if (Index == MB_REG_NONCE1) {
    *RDpa = (UINT64) MBDpa->pNonce1;
  } else if (Index == MB_REG_STATUS) {
    *RDpa = (UINT64) MBDpa->pStatus;
  } else {
    Status = EFI_INVALID_PARAMETER;
  }

  return (Status);
}

/**

  Routine Description: Returns the Index number of the NVMDIMM for a given
    Socket and Channel.

  @param[in] Socket      - Socket Number
  @param[in] Ch          - DDR Channel ID in socket

  @return Index

**/
EFI_STATUS
GetDimmIndex (
  UINT8  Socket,
  UINT8  Ch,
  UINT8  *Index
  )
{
  EFI_STATUS    Status;
  NVDIMM        *NvmDimm;
  UINT8         Dimm;
  UINT8         MaxChDdr;
  UINT8         i;

  MaxChDdr = GetMaxChDdr ();

  if (Index == NULL || Socket >= MAX_SOCKET || Ch >= MaxChDdr) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetChannelDimm (Socket, Ch, &Dimm);
  if (EFI_ERROR(Status)) {
    return EFI_NOT_FOUND;
  }

  for (i = 0; i < mCrInfo.NvdimmInfo.NumNvdimms; i++) {
    NvmDimm = &mCrInfo.NvdimmInfo.Nvdimms[i];
    if ((NvmDimm->SocketId == Socket) && (NvmDimm->Ch == Ch) && (NvmDimm->Dimm == Dimm)) {
      *Index = i;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Implicitly flushes WPQ by performing reads to consecutive cachelines
  for the purpose of EKV/BWV CSR FSM WA

  @param[in]  Dimm  Pointer to the DIMM that needs the flush

**/
VOID
WaWpqFlush (
  NVDIMM *Dimm
  )
{
  UINT8 RegIndex;

  if (mInSmm) {
    for (RegIndex = 0; RegIndex < 32; RegIndex++) {
      *(VOLATILE UINT64 *)Dimm->FlushAddress[RegIndex] = 0xFFFFFFFFFFFFFFFF;
    }
    RegIndex = (UINT8) *(VOLATILE UINT64 *)Dimm->FlushAddress[31];
  }
}

/**
  Routine Description: Performs a durable Cache Line Write

  @param CsrAddress   - Mailbox CSR Address
  @param Data         - Data to be written
  @param DataSize     - Size of Data to be written

DurableCacheLineWrite( addr, data ) performs the following:

(1) UC Write of cache line of write data using CSR addr
(2) UC Read of same cache line as step b, using CSR addr (See Note* below)

Note*: IMC will block new requests to the same address as a pending request in the buffer.
       Read forces previous write data to be flushed from WPQ to SXPs Durability Domain
**/
VOID
DurableCacheLineWrite (
  UINTN   CsrAddress,
  UINT64  Data,
  UINT8   DataSize
  )
{
  volatile UINT64 TempData;

  switch (DataSize) {
  case (sizeof (UINT8)) :
    *(VOLATILE UINT8 *) CsrAddress = (UINT8) Data;
    break;
  case (sizeof (UINT16)) :
    *(VOLATILE UINT16 *) CsrAddress = (UINT16) Data;
    break;
  case (sizeof (UINT32)) :
    *(VOLATILE UINT32 *) CsrAddress = (UINT32) Data;
    break;
  case (sizeof (UINT64)) :
    *(VOLATILE UINT64 *) CsrAddress = Data;
    break;
  default:
    break;
  }

  TempData = *(VOLATILE UINT64 *) CsrAddress;
}

/**
  Issues a WPQ flush

  @param[in]  Dimm       Pointer to the DIMM that needs the flush
  @param[in]  CsrAccess  TRUE:  set if flushing CSR access
                         FALSE: set if flushing media access

  @retval TRUE    WPQ flush issued successfully
  @retval FALSE   Failed to issue WPQ flush

**/
BOOLEAN
IssueWpqFlush (
  IN NVDIMM   *Dimm,
  IN BOOLEAN  CsrAccess
  )
{
  VOLATILE UINT64  *FlushHintAddress;

  if (!CsrAccess || !mInSmm) {
    FlushHintAddress = (UINT64*) mWpqFlushHintAddress[Dimm->SocketId][Dimm->Ch];
    if (FlushHintAddress) {
      *FlushHintAddress = 0xffffffffffffffff;
      return TRUE;
    }
    return FALSE;
  } else {
    WaWpqFlush (Dimm);
    return TRUE;
  }
}

/**

  Issues WPQ Flush for all iMCs

  @param None

  @return Standard EFI_STATUS is returned

**/
EFI_STATUS
IssueWpqFlushForAllImc (
  VOID
  )
{
  UINT8   Socket;
  UINT8   Mc;
  UINT8   Ch;
  UINT8   MaxImc;
  UINT8   NumChannelPerMc;
  BOOLEAN Is14nmCpu;
  NVDIMM  *DimmPtr;

  Is14nmCpu = (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL));
  MaxImc = GetMaxImc ();
  NumChannelPerMc = GetNumChannelPerMc ();

  //
  // Issue WPQ Flush to all MCs
  //
  if (mUseWpqFlush) {
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      for (Mc = 0; Mc < MaxImc; Mc++) {
        for (Ch = 0; Ch < NumChannelPerMc; Ch++) {
          DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch + Mc * NumChannelPerMc, 0, NVDIMM_FLAG_ENABLED);
          if (DimmPtr == NULL) {
            continue;
          }
          if (IssueWpqFlush (DimmPtr, FALSE) && Is14nmCpu) {
            break;
          }
        }
      }
    }
  }
  return EFI_SUCCESS;
}

/**
  This routine calculates number of processors.

  The routine queries save state register Processor ID on every cpu. It uses
  the return status of the query to decide if a CPU exists or not.
**/
VOID
CrUpdateCpuThreadState (
  VOID
  )
{
  EFI_STATUS                    Status;
  UINT64                        Data64;
  UINT16                        Index;

  EFI_PROCESSOR_INFORMATION     ProcInfo;

  for (Index = 0; Index < NELEMENTS (mCpuThreadState); Index++) {

    mCpuThreadState[Index] = 0;
    if (mInSmm) {

      Status = mSmmCpu->ReadSaveState (mSmmCpu,
        sizeof (UINT64),
        EFI_SMM_SAVE_STATE_REGISTER_PROCESSOR_ID,
        Index,
        &Data64);

      if (EFI_ERROR (Status)) {
        continue;
      }
    } else {

      Status = mMpService->GetProcessorInfo (mMpService, Index, &ProcInfo);
      if (EFI_ERROR (Status) || !(ProcInfo.StatusFlag & PROCESSOR_ENABLED_BIT)) {
        continue;
      }
    }
    mCpuThreadState[Index] = 1;
    mCpuThreadNum = Index + 1;
  }
} // CrUpdateCpuThreadState()


/**
This routine checks if all cpus have arrived in SMM.

The routine queries save state register Processor ID on every cpu. It uses
the return status of the query to decide if a CPU has arrived in SMM or not.

To be improved in the future: avoiding calling this routine for every FW command

@retval TRUE All Cpus checked in to SMM
@retval FALSE Some Cpu not checked in to SMM
**/
BOOLEAN
CheckAllApCheckedIn (
  OUT UINT16 *NumCheckedInPtr
  )
{
  EFI_STATUS Status;
  UINT64     Data64;
  BOOLEAN    RetVal = TRUE;
  UINT16     Index;
  UINT16     NumCheckedIn = 0;

  for (Index = 0; Index < mCpuThreadNum; Index++) {

    if (Index == mSmst->CurrentlyExecutingCpu || !(mCpuThreadState[Index])) {
      continue;
    }
    //
    // Return status of ReadSaveState:
    //  EFI_SUCCESS if cpu has entered SMM
    //
    Status = mSmmCpu->ReadSaveState (mSmmCpu,
      sizeof (UINT64),
      EFI_SMM_SAVE_STATE_REGISTER_PROCESSOR_ID,
      Index,
      &Data64);

    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "CPU thread %d not in SMM (%d)\n", Index, Status));
      RetVal = FALSE;
    } else {
      NumCheckedIn++;
    }
  }
  if (NumCheckedInPtr != NULL) {
    *NumCheckedInPtr = NumCheckedIn;
  }
  return RetVal;
} // CheckAllApCheckedIn()

/**
  Routine Description: SendSecurityNonceToAllDimms sends a
  security nonce to all NVDIMMs installed in the system.

  @param ResetNonces[in]  - Specifies whether to reset all Nonces

  @return Void.
**/
VOID
SendSecurityNonceToAllDimms (
  IN BOOLEAN ResetNonces
  )
{
  NVDIMM      *DimmPtr;
  UINT8       Index;
  EFI_STATUS  Status;

  for (Index = 0; Index < mCrInfo.NvdimmInfo.NumNvdimms; Index++) {
    DimmPtr = &mCrInfo.NvdimmInfo.Nvdimms[Index];
    if (DimmPtr->SADSpaBase == SPA_NOT_MAPPED) {
      continue;
    }
    if (ResetNonces) {
      DimmPtr->NonceSet = FALSE;
    }
    Status = SendFnvSecurityNonce (DimmPtr);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      CpuDeadLoop();
    }
  }
  DsmReadFisCmdAccessPolicy ();
}

/**

  @brief This function sent bit0 in CMOS Diagnostic Status register according to the NM Flush state

  @param None

  @return VOID

**/
VOID
SignalNmFlushEnable (
  VOID
  )
{
   EFI_STATUS Status;

   if (mAdwbFlushTaskCount > 0) {
     CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Signal NM Flush enable\n"));

     Status = SignalNmFlushStatusToCmosReg (NM_FLUSH_ENABLE);
     if (EFI_ERROR (Status)) {
       CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "Signal NM Flush enable failed (status %r)\n", Status));
     }
  }
}

/**
 @brief This function sends Latch LSS command to all enabled NVDIMMs

 Latch LSS is supposed to be done by the persistent memory driver in OS using ACPI DSM function,
 before any write to persistent memory is done. BIOS knob is implemented to enable Latch LSS
 for operating systems that would not call DSM. Enabling latch twice is not a problem so the BIOS
 action does not colide with OSes that use DSM to enable latch.

 @return Void.
**/
VOID
LatchSystemShutdownStateInAllDimms (
  VOID
  )
{
  UINT8                                   i;
  NVDIMM                                  *Dimm;
  NVDIMM                                  *NvmDimm = NULL;
  FIS_SET_LATCH_SYSTEM_SHUTDOWN_STATE_REQ Data;
  EFI_STATUS                              Status;

  if (!mSystemMemoryMap->MemSetup.LatchSystemShutdownState) {

    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Set Latch System Shutdown State is disabled\n"));
    return;
  }

  for (i = 0; i < mCrInfo.NvdimmInfo.NumNvdimms; i++) {

    Dimm = &mCrInfo.NvdimmInfo.Nvdimms[i];
    NvmDimm = GetDimm (&mCrInfo.NvdimmInfo, (UINT8)Dimm->SocketId, Dimm->Ch, Dimm->Dimm, NVDIMM_FLAG_ENABLED);

    if (NvmDimm == NULL) {
      continue;
    }
    ZeroMem (&Data, sizeof (Data));
    Data.Bits.LatchSystemShutdownStateEn = 1;

    Status = FisRequest ((UINT8)Dimm->SocketId, Dimm->Ch, Dimm->Dimm, (UINT32*)&Data, sizeof (Data), NULL, SET_LATCH_SYSTEM_SHUTDOWN_STATUS, 0, NULL);
    if (EFI_ERROR (Status)) {

      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: Setting latch system shutdown status failed (%r)\n",
              Dimm->SocketId, Dimm->Ch, Dimm->Dimm, Status));
    }
  }
}

/**
  Generates and sets NVMCTLR Security Nonce

  @param [in]  NvmDimm      NVDIMM to operate on
  @retval      EFI_STATUS   success if command sent

**/
EFI_STATUS
SendFnvSecurityNonce (
  IN  NVDIMM  *NvmDimm
  )
{
  FIS_SET_SECURITY_NONCE_REQ  Data;
  EFI_STATUS                  Status;
  UINT8                       Socket;
  UINT8                       Ch;
  UINT8                       Dimm;

  if (NvmDimm == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // If the Nonce is already set for this dimm, just punt
  if (NvmDimm->NonceSet) {
    return EFI_SUCCESS;
  }

  Socket = (UINT8)NvmDimm->SocketId;
  Ch     = NvmDimm->Ch;
  Dimm   = NvmDimm->Dimm;

  //
  // Contact CLV before changing the trace below, test depends on it.
  //
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "S%d.C%d.D%d: Setting nonce\n", Socket, Ch, Dimm));

  ZeroMem (&Data, sizeof (Data));
  //
  // Generate 64bit Security Nonce using RNG
  //
  Status = RdRand64 (&Data.Bits.SecurityNonce, TRUE);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: Unable to get a valid Nonce\n", Socket, Ch, Dimm));
    return Status;
  }

  Status = FisRequest (Socket, Ch, Dimm, (UINT32*)&Data, sizeof (Data), NULL, SET_SECURITY_NONCE, FIS_DEFTIMEOUT, NULL);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: Setting nonce failed (%r)\n", Socket, Ch, Dimm, Status));
  } else {
    //
    // Save the Nonce for future sending of any commands and enable flag to tell that nonce has been set.
    //
    NvmDimm->Nonce = Data.Bits.SecurityNonce;
    NvmDimm->NonceSet = TRUE;
  }

  //
  // explicitly clear(set to 0) the nonce value, as it is local; potentially this could be used/tapped by other(functions,..)
  //
  Data.Bits.SecurityNonce = 0x0;

  return Status;
}

/**

Routine Description: SendConfigLockdownToAllDimms sends a
config lockdown command to all NVDIMMs installed in the system.

**/
VOID
SendConfigLockdownToAllDimms (
  VOID
  )
{
  NVDIMM      *DimmPtr;
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT8       Index;
  UINT8       Socket;
  UINT8       Ch;
  UINT8       Dimm;

  if (!(mSystemMemoryMap->MemSetup.dfxMemSetup.dfxOptions & LOCK_NGN_CSR_DIS)) {

    for (Index = 0; Index < mCrInfo.NvdimmInfo.NumNvdimms; Index++) {
      DimmPtr = &mCrInfo.NvdimmInfo.Nvdimms[Index];
      if (DimmPtr->SADSpaBase == SPA_NOT_MAPPED) {
        continue;
      }
      Socket = (UINT8)DimmPtr->SocketId;
      Ch     = DimmPtr->Ch;
      Dimm   = DimmPtr->Dimm;
      //
      // Contact CLV before changing the trace below, test depends on it.
      //
      CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "S%d.C%d.D%d: Setting config lockdown\n", Socket, Ch, Dimm));
      Status = FisRequest (Socket, Ch, Dimm, NULL, 0, NULL, SET_CONFIG_LOCKDOWN, FIS_DEFTIMEOUT, NULL);
      if (EFI_ERROR (Status)) {

        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: Setting config lockdown failed (%r)\n",
                Socket, Ch, Dimm, Status));
      }
    }
  }
}

/**
  Performs Exit Boot Services CR actions

  @param[in] Protocol   Points to the protocol's unique identifier.
  @param[in] Interface  Points to the interface instance.
  @param[in] Handle     The handle on which the interface was installed.

  @retval EFI_SUCCESS   Notification runs successfully.
**/
EFI_STATUS
EFIAPI
CrExitBootServices (
  IN CONST EFI_GUID     *Protocol,
  IN VOID               *Interface,
  IN EFI_HANDLE         Handle
  )
{
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "CrExitBootServices\n"));

  LatchSystemShutdownStateInAllDimms ();

  SignalNmFlushEnable ();

  return EFI_SUCCESS;
}

/**
  Performs EndOfDxe CR init when SMM EndOfDxe protocol is installed.

  @param[in] Protocol   Points to the protocol's unique identifier.
  @param[in] Interface  Points to the interface instance.
  @param[in] Handle     The handle on which the interface was installed.

  @retval EFI_SUCCESS   Notification runs successfully.
**/
EFI_STATUS
EFIAPI
CrInitAtEndOfDxe (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
  )
{
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "CrInitAtEndOfDxe\n"));

  SendConfigLockdownToAllDimms ();
  FwActivateAtEndOfDxe ();

  return EFI_SUCCESS;
}


/**
  @brief This function restores NVDIMM settings that need to be restored on S3 resume path.

  This is a handler for SMI interrupt invoked on S3 resume path by writing IO port.

  @param[in] DispatchHandle - not used
  @param[in] DispatchContext - not used
  @param[in,out] CommBuffer - not used
  @param[in,out] CommBufferSize - not used

  @retval EFI_SUCCESS - operation done
*/
EFI_STATUS
EFIAPI
CrS3Resume (
  IN            EFI_HANDLE                  DispatchHandle,
  IN      CONST EFI_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT        VOID                        *CommBuffer,      OPTIONAL
  IN OUT        UINTN                       *CommBufferSize   OPTIONAL
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  if (mCrInfo.NvdimmInfo.NumNvdimms > 0) {
    SendSecurityNonceToAllDimms (TRUE);
    S3UnlockAllDimms();
    SendConfigLockdownToAllDimms ();
    LatchSystemShutdownStateInAllDimms ();
  }
  return Status;
}


/**
  Routine Description: Is the given NVMDIMM Enabled

  @param[in] Socket   - Socket ID
  @param[in] Ch       - Channel ID on given Socket
  @param[in] Dimm     - Dimm ID on given channel

  @return TRUE if NVMDIMM is Enabled, FALSE otherwise
**/
BOOLEAN
IsNvmDimmEnabled (
  UINT8 Socket,
  UINT8 Ch,
  UINT8 Dimm
  )
{
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  if (Socket >= MAX_SOCKET || Ch >= MaxChDdr || Dimm >= MAX_DIMM) {
    return FALSE;
  }
  if (IsNvmDimm (Socket, Ch, Dimm)) {
    return mSystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].Enabled;
  }
  return FALSE;
}


/**
Routine Description:IsRedundantSAD checks to see if this SAD has
been accounted for previously in the creation of the DimmInfo
struct for Control Region.

  @param SadPtr      - Pointer to the SAD Table
  @retval BOOLEAN    - TRUE if redundant else FALSE

**/
BOOLEAN
IsRedundantCtrlRgnSAD(
   SAD_TABLE               *SadPtr
   )
{
  STATIC SAD_CONTROL_REGION SadControlRegion = {0};
  UINT8                     i;

  for (i = 0; i < SadControlRegion.NumOfSADCntrlReg; i++) {

    if (SadControlRegion.Limits[i] == SadPtr->Limit) {

      return TRUE;
    }
  }
  //
  // SAD is new, store it in SadControlRegion cache and return false.
  //
  SadControlRegion.Limits[i] = SadPtr->Limit;
  SadControlRegion.NumOfSADCntrlReg++;

  return FALSE;
}


/**

Routine Description:IsRedundantSAD checks to see if this SAD has
been accounted for previously in the creation of the PMEMInfo
structs for PMEM Region.

  @param SadPtr    - Pointer to the SAD table entry

  @retval BOOLEAN    - TRUE if redundant else FALSE

**/
BOOLEAN
IsRedundantPMemRgnSAD(
   IN SAD_TABLE      *SadPtr
   )
{
  STATIC SAD_PMEM_REGION SadPMemRegion = {0};
  UINT8                  i;

  for (i = 0; i < SadPMemRegion.NumOfSADPMemReg; i++) {

    if (SadPMemRegion.Limits[i] == SadPtr->Limit) {

      return TRUE;
    }
  }
  //
  // SAD is new, store it in SadPMemRegion cache and return false.
  //
  SadPMemRegion.Limits[i] = SadPtr->Limit;
  SadPMemRegion.NumOfSADPMemReg++;

  return FALSE;
}

/**
  @brief Initializes mCrInfo.NvdimmInfo.Nvdimms[mCrInfo.NvdimmInfo.NumNvdimms]
         struct that is being created with the values that are being
         passed in of its members.

  @param[in] SadSpaBase  - SPA base address of SAD Rule
  @param[in] CtrlBase    - SPA base address of DIMM control region
  @param[in] Socket      - Socket index
  @param[in] Ch          - Channel index (per socket)
  @param[in] Dimm        - Dimm number
  @param[in] McWays      - Ways this dimm is interleaved across iMCs
  @param[in] ChWays      - Channel Ways this dimm is interleaved.
  @param[in] IntDimmNum  - DIMM number in the interleave set
  @param[in] FMchannelInterBitmap - pSAD->FMchannelInterBitmap of the sad rule

  @return EFI_STATUS is returned.
**/
EFI_STATUS
CrInfoInitDimm (
  IN UINT64  SadSpaBase,
  IN UINT64  CntrlBase,
  IN UINT8   Skt,
  IN UINT8   Ch,
  IN UINT8   Dimm,
  IN UINT8   McWays,
  IN UINT8   ChWays,
  IN UINT8   IntDimmNum,
  IN UINT8   FMchannelInterBitmap
  )
{
  EFI_STATUS                Status;
  FIS_BOOT_STATUS_REGISTER  Bsr;
  UINT64                    RSpa;
  UINT32                    PayloadOffset;
  UINT8                     i;
  UINT8                     j;
  UINT64                    LowAddress;
  UINT64                    Address;
  UINT8                     AddressIndex;
  NVDIMM                    *DimmPtr;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "S%d.C%d.D%d: Adding DIMM info\n", Skt, Ch, Dimm));
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "S%d.C%d.D%d: SPA %p, ctrl base %p, McWays %d, ChWays %d, IL pos %d, FM IL bitmap 0x%X\n",
          Skt, Ch, Dimm, SadSpaBase, CntrlBase, McWays, ChWays, IntDimmNum, FMchannelInterBitmap));

  DimmPtr = &mCrInfo.NvdimmInfo.Nvdimms[mCrInfo.NvdimmInfo.NumNvdimms];
  mCrInfo.NvdimmInfo.NumNvdimms++;
  DimmPtr->SADSpaBase = SadSpaBase;
  DimmPtr->CntrlBase = CntrlBase;
  DimmPtr->Ch = Ch;
  DimmPtr->Dimm = Dimm;
  DimmPtr->SocketId = Skt;
  DimmPtr->ImcId = ConvertSktChToImc (Ch);
  DimmPtr->NonceSet = 0;
  DimmPtr->iMCWays = McWays;
  DimmPtr->ChWays = ChWays;
  DimmPtr->IntDimmNum = IntDimmNum;
  DimmPtr->FMchannelInterBitmap = FMchannelInterBitmap;
  //
  // We need Dpa for each of these Mailbox addresses. However, what we have is Spa.
  // So it we need one routine to convert the Spa to Dpa.
  //
  Status = XlateDpaOffsetToSpa (DimmPtr, CntrlBase, (UINT64) (CR_BIOS_MB_OFFSET + CR_MB_COMMAND_OFFSET), &RSpa);
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Command Reg SPA = 0x%1lX\n", RSpa));
  DimmPtr->MailboxDPA.pCommand = (UINT64) (CR_BIOS_MB_OFFSET + CR_MB_COMMAND_OFFSET);
  if (!EFI_ERROR (Status)) {
    DimmPtr->MailboxSPA.pCommand = RSpa;
  }
  ASSERT (Status == EFI_SUCCESS);

  Status = XlateDpaOffsetToSpa (DimmPtr, CntrlBase, (UINT64) (CR_BIOS_MB_OFFSET + CR_MB_NONCE0_OFFSET), &RSpa);
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Nonce Reg SPA = 0x%1lX\n", RSpa));
  DimmPtr->MailboxDPA.pNonce0 = (UINT64)(CR_BIOS_MB_OFFSET + CR_MB_NONCE0_OFFSET);
  DimmPtr->MailboxSPA.pNonce0 = (UINT64)RSpa;

  Status = XlateDpaOffsetToSpa (DimmPtr, CntrlBase, (UINT64) (CR_BIOS_MB_OFFSET + CR_MB_NONCE1_OFFSET), &RSpa);
  DimmPtr->MailboxDPA.pNonce1 = (UINT64)(CR_BIOS_MB_OFFSET + CR_MB_NONCE1_OFFSET);
  DimmPtr->MailboxSPA.pNonce1 = (UINT64)RSpa;
  PayloadOffset = 0;
  for (i = 0; i < CR_IN_PAYLOAD_NUM; i++) {
    // If Odd register..
    if (i % 2) {
      DimmPtr->MailboxDPA.pInPayload[i] = (UINT64)(CR_BIOS_MB_OFFSET + CR_MB_IN_PAYLOAD0_OFFSET + PayloadOffset + CR_IN_SINGLE_PAYLOAD_SIZE);
      Status = XlateDpaOffsetToSpa (DimmPtr, CntrlBase, (UINT64)(CR_BIOS_MB_OFFSET + CR_MB_IN_PAYLOAD0_OFFSET + PayloadOffset + CR_IN_SINGLE_PAYLOAD_SIZE), &RSpa);
      PayloadOffset += CR_REG_ALIGNMENT_SIZE;
    } else {
      DimmPtr->MailboxDPA.pInPayload[i] = (UINT64)(CR_BIOS_MB_OFFSET + CR_MB_IN_PAYLOAD0_OFFSET + PayloadOffset);
      Status = XlateDpaOffsetToSpa (DimmPtr, CntrlBase, (UINT64)(CR_BIOS_MB_OFFSET + CR_MB_IN_PAYLOAD0_OFFSET + PayloadOffset), &RSpa);
    }
    CRDEBUG ((DEBUG_VERBOSE, CR_INFO_STR ("CRP") "InPayload%d Reg SPA = 0x%1lx\n", i, RSpa));
    DimmPtr->MailboxSPA.pInPayload[i] = (UINT64)RSpa;
  }
  DimmPtr->MailboxDPA.pStatus = (UINT64)(CR_BIOS_MB_OFFSET + CR_MB_STATUS_OFFSET);
  Status = XlateDpaOffsetToSpa (DimmPtr, CntrlBase, (UINT64)(CR_BIOS_MB_OFFSET + CR_MB_STATUS_OFFSET), &RSpa);
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Status Reg SPA = 0x%1lx\n", RSpa));
  DimmPtr->MailboxSPA.pStatus = (UINT64)RSpa;
  PayloadOffset = 0;
  for (i = 0; i < CR_OUT_PAYLOAD_NUM; i++) {
    // If Odd register..
    if (i % 2) {
      DimmPtr->MailboxDPA.pOutPayload[i] = (UINT64)(CR_BIOS_MB_OFFSET + CR_MB_OUT_PAYLOAD0_OFFSET + PayloadOffset + CR_OUT_SINGLE_PAYLOAD_SIZE);
      Status = XlateDpaOffsetToSpa (DimmPtr, CntrlBase, (UINT64)(CR_BIOS_MB_OFFSET + CR_MB_OUT_PAYLOAD0_OFFSET + PayloadOffset + CR_OUT_SINGLE_PAYLOAD_SIZE), &RSpa);
      PayloadOffset += CR_REG_ALIGNMENT_SIZE;
    } else {
      DimmPtr->MailboxDPA.pOutPayload[i] = (UINT64)(CR_BIOS_MB_OFFSET + CR_MB_OUT_PAYLOAD0_OFFSET + PayloadOffset);
      Status = XlateDpaOffsetToSpa (DimmPtr, CntrlBase, (UINT64)(CR_BIOS_MB_OFFSET + CR_MB_OUT_PAYLOAD0_OFFSET + PayloadOffset), &RSpa);
    }
    CRDEBUG ((DEBUG_VERBOSE, CR_INFO_STR ("CRP") "OutPayload%d Reg SPA = 0x%1lx\n", i, RSpa));
    DimmPtr->MailboxSPA.pOutPayload[i] = (UINT64)RSpa;
  }

  DimmPtr->MailboxDPA.LargeInPayload = (UINT64)(CR_BIOS_MB_LARGE_INPAYLOAD);
  Status = XlateDpaOffsetToSpa (DimmPtr, CntrlBase, (UINT64)CR_BIOS_MB_LARGE_INPAYLOAD, &RSpa);
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Large InPayload Reg SPA = 0x%1lx\n", RSpa));
  DimmPtr->MailboxSPA.LargeInPayload = (UINT64)RSpa;

  DimmPtr->MailboxDPA.LargeOutPayload = (UINT64)CR_BIOS_MB_LARGE_OUTPAYLOAD;
  Status = XlateDpaOffsetToSpa (DimmPtr, CntrlBase, (UINT64)CR_BIOS_MB_LARGE_OUTPAYLOAD, &RSpa);
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Large OutPayload Reg SPA = 0x%1lx\n", RSpa));
  DimmPtr->MailboxSPA.LargeOutPayload = (UINT64)RSpa;

  //
  // The following code adds OS Mailbox Register addresses to the DimmPtr struct.
  //
  Status = XlateDpaOffsetToSpa (DimmPtr, CntrlBase, (UINT64)(CR_OS_MB_OFFSET + CR_MB_COMMAND_OFFSET), &RSpa);
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "OS Command Reg SPA = 0x%1lx\n", RSpa));
  DimmPtr->OSMailboxDPA.pCommand = (UINT64)(CR_OS_MB_OFFSET + CR_MB_COMMAND_OFFSET);
  if (Status == EFI_SUCCESS) {
    DimmPtr->OSMailboxSPA.pCommand = RSpa;
  }
  ASSERT(Status == EFI_SUCCESS);

  Status = XlateDpaOffsetToSpa (DimmPtr, CntrlBase, (UINT64)(CR_OS_MB_OFFSET + CR_MB_NONCE0_OFFSET), &RSpa);
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "OS Nonce Reg SPA = 0x%1lx\n", RSpa));
  DimmPtr->OSMailboxDPA.pNonce0 = (UINT64)(CR_OS_MB_OFFSET + CR_MB_NONCE0_OFFSET);
  DimmPtr->OSMailboxSPA.pNonce0 = (UINT64)RSpa;

  Status = XlateDpaOffsetToSpa (DimmPtr, CntrlBase, (UINT64)(CR_OS_MB_OFFSET + CR_MB_NONCE1_OFFSET), &RSpa);
  DimmPtr->OSMailboxDPA.pNonce1 = (UINT64)(CR_OS_MB_OFFSET + CR_MB_NONCE1_OFFSET);
  DimmPtr->OSMailboxSPA.pNonce1 = (UINT64)RSpa;
  PayloadOffset = 0;
  for (i = 0; i < CR_IN_PAYLOAD_NUM; i++) {
    // If Odd register..
    if (i % 2) {
      DimmPtr->OSMailboxDPA.pInPayload[i] = (UINT64)(CR_OS_MB_OFFSET + CR_MB_IN_PAYLOAD0_OFFSET + PayloadOffset + CR_IN_SINGLE_PAYLOAD_SIZE);
      Status = XlateDpaOffsetToSpa (DimmPtr, CntrlBase, (UINT64)(CR_OS_MB_OFFSET + CR_MB_IN_PAYLOAD0_OFFSET + PayloadOffset + CR_IN_SINGLE_PAYLOAD_SIZE), &RSpa);
      PayloadOffset += CR_REG_ALIGNMENT_SIZE;
    } else {
      DimmPtr->OSMailboxDPA.pInPayload[i] = (UINT64)(CR_OS_MB_OFFSET + CR_MB_IN_PAYLOAD0_OFFSET + PayloadOffset);
      Status = XlateDpaOffsetToSpa (DimmPtr, CntrlBase, (UINT64)(CR_OS_MB_OFFSET + CR_MB_IN_PAYLOAD0_OFFSET + PayloadOffset), &RSpa);
    }
    CRDEBUG ((DEBUG_VERBOSE, CR_INFO_STR ("CRP") "InPayload%d Reg SPA = 0x%1lx\n", i, RSpa));
    DimmPtr->OSMailboxSPA.pInPayload[i] = (UINT64)RSpa;
  }
  DimmPtr->OSMailboxDPA.pStatus = (UINT64)(CR_OS_MB_OFFSET + CR_MB_STATUS_OFFSET);
  Status = XlateDpaOffsetToSpa (DimmPtr, CntrlBase, (UINT64)(CR_OS_MB_OFFSET + CR_MB_STATUS_OFFSET), &RSpa);
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Status Reg SPA = 0x%1lx\n", RSpa));
  DimmPtr->OSMailboxSPA.pStatus = (UINT64)RSpa;
  PayloadOffset = 0;
  for (i = 0; i < CR_OUT_PAYLOAD_NUM; i++) {
    // If Odd register..
    if (i % 2) {
      DimmPtr->OSMailboxDPA.pOutPayload[i] = (UINT64)(CR_OS_MB_OFFSET + CR_MB_OUT_PAYLOAD0_OFFSET + PayloadOffset + CR_OUT_SINGLE_PAYLOAD_SIZE);
      Status = XlateDpaOffsetToSpa (DimmPtr, CntrlBase, (UINT64)(CR_OS_MB_OFFSET + CR_MB_OUT_PAYLOAD0_OFFSET + PayloadOffset + CR_OUT_SINGLE_PAYLOAD_SIZE), &RSpa);
      PayloadOffset += CR_REG_ALIGNMENT_SIZE;
    } else {
      DimmPtr->OSMailboxDPA.pOutPayload[i] = (UINT64)(CR_OS_MB_OFFSET + CR_MB_OUT_PAYLOAD0_OFFSET + PayloadOffset);
      Status = XlateDpaOffsetToSpa (DimmPtr, CntrlBase, (UINT64)(CR_OS_MB_OFFSET + CR_MB_OUT_PAYLOAD0_OFFSET + PayloadOffset), &RSpa);
    }
    CRDEBUG ((DEBUG_VERBOSE, CR_INFO_STR ("CRP") "OS OutPayload%d Reg SPA = 0x%1lx\n", i, RSpa));
    DimmPtr->OSMailboxSPA.pOutPayload[i] = (UINT64)RSpa;
  }

  if (RSpa - mCrInfo.ControlRegion[mCrInfo.NumControlRegions].Base + 64 > mCrInfo.ControlRegion[mCrInfo.NumControlRegions].Size) {
    mCrInfo.ControlRegion[mCrInfo.NumControlRegions].Size = RSpa - mCrInfo.ControlRegion[mCrInfo.NumControlRegions].Base + 64;
  }

  for (i = 0; i < 32; i++) {
    XlateDpaOffsetToSpa(DimmPtr, CntrlBase, 0x7FFF200UL + i * 64, &DimmPtr->FlushAddress[i]);
    if (mCrInfo.FlushRegion[mCrInfo.NumControlRegions].Base == 0 || mCrInfo.FlushRegion[mCrInfo.NumControlRegions].Base > DimmPtr->FlushAddress[i]) {
      mCrInfo.FlushRegion[mCrInfo.NumControlRegions].Base = DimmPtr->FlushAddress[i];
    }
    if (mCrInfo.FlushRegion[mCrInfo.NumControlRegions].End < DimmPtr->FlushAddress[i] + 64) {
      mCrInfo.FlushRegion[mCrInfo.NumControlRegions].End = DimmPtr->FlushAddress[i] + 64;
    }
  }
  DimmPtr->FlushRegionIndex = mCrInfo.NumControlRegions;

  DimmPtr->OSMailboxDPA.LargeInPayload = (UINT64)(CR_OS_MB_LARGE_INPAYLOAD);
  Status = XlateDpaOffsetToSpa (DimmPtr, CntrlBase, (UINT64)CR_OS_MB_LARGE_INPAYLOAD, &RSpa);
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "OS Large InPayload Reg SPA = 0x%1lx\n", RSpa));
  DimmPtr->OSMailboxSPA.LargeInPayload = (UINT64)RSpa;

  DimmPtr->OSMailboxDPA.LargeOutPayload = (UINT64)CR_OS_MB_LARGE_OUTPAYLOAD;
  Status = XlateDpaOffsetToSpa(DimmPtr, CntrlBase, (UINT64)CR_OS_MB_LARGE_OUTPAYLOAD, &RSpa);
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "OS Large OutPayload Reg SPA = 0x%1lX\n", RSpa));
  DimmPtr->OSMailboxSPA.LargeOutPayload = (UINT64)RSpa;
  DimmPtr->ControlRegionIndex = mCrInfo.NumControlRegions;

  Status = XlateDpaOffsetToSpa(DimmPtr, CntrlBase, (UINT64)CR_MB_BOOT_STATUS_OFFSET, &RSpa);
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "BSR Reg SPA = 0x%llx\n", RSpa));
  DimmPtr->BsrSpa = RSpa;

  //SPA of the SMBUS COMMAND REG
  Status = XlateDpaOffsetToSpa(DimmPtr, CntrlBase, (UINT64)(CR_SMBUS_MB_OFFSET + CR_MB_COMMAND_OFFSET), &RSpa);
  DimmPtr->SmbusCmdRegSpa = (UINT64)RSpa;
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Smbus SPA =  0x%llx\n", DimmPtr->SmbusCmdRegSpa));

  //
  // Try to read BSR to verify the DDRT mailbox registers are available.
  //
  Bsr.QWord = ((FIS_BOOT_STATUS_REGISTER*)DimmPtr->BsrSpa)->QWord;
  if (Bsr.QWord == (UINT64)-1 || !Bsr.Bits.DdrtIoInitComplete || !Bsr.Bits.MailboxInterfaceReady) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: Mailbox not ready in BSR 0x%llX\n", Skt, Ch, Dimm, Bsr.QWord));
    DimmPtr->SADSpaBase = SPA_NOT_MAPPED;
    Status = EFI_NOT_READY;
  }
  if (DimmPtr->BsrSpa - mCrInfo.ControlRegion[mCrInfo.NumControlRegions].Base + 64 > mCrInfo.ControlRegion[mCrInfo.NumControlRegions].Size) {
    mCrInfo.ControlRegion[mCrInfo.NumControlRegions].Size = DimmPtr->BsrSpa - mCrInfo.ControlRegion[mCrInfo.NumControlRegions].Base + 64;
  }

  //
  // Create sorted arrays of input and output payload registers together index lookup tables for ASL DSM implementation
  //
  LowAddress = 0;
  for (i = 0; i < CR_IN_PAYLOAD_NUM; i+= 2) {
    Address = MAX_UINT64;
    AddressIndex = MAX_UINT8;
    for (j = 0; j < CR_IN_PAYLOAD_NUM; j+= 2) {
      if (DimmPtr->OSMailboxSPA.pInPayload[j] <= LowAddress) {
        continue;
      }
      if (DimmPtr->OSMailboxSPA.pInPayload[j] < Address) {
        Address = DimmPtr->OSMailboxSPA.pInPayload[j];
        AddressIndex = j;
      }
    }
    ASSERT (AddressIndex != MAX_UINT8);
    LowAddress = Address;
    DimmPtr->OSMailboxSPA.InPayloadOrderLut[i / 2] = AddressIndex / 2;
    DimmPtr->OSMailboxSPA.pInPayloadOrder[i / 2] = Address;
  }

  LowAddress = 0;
  for (i = 0; i < CR_OUT_PAYLOAD_NUM; i+= 2) {
    Address = MAX_UINT64;
    AddressIndex = MAX_UINT8;
    for (j = 0; j < CR_OUT_PAYLOAD_NUM; j += 2) {
      if (DimmPtr->OSMailboxSPA.pOutPayload[j] <= LowAddress) {
        continue;
      }
      if (DimmPtr->OSMailboxSPA.pOutPayload[j] < Address) {
        Address = DimmPtr->OSMailboxSPA.pOutPayload[j];
        AddressIndex = j;
      }
    }
    ASSERT (AddressIndex != MAX_UINT8);
    LowAddress = Address;
    DimmPtr->OSMailboxSPA.OutPayloadOrderLut[i / 2] = AddressIndex / 2;
    DimmPtr->OSMailboxSPA.pOutPayloadOrder[i / 2] = Address;
  }

  LowAddress = 0;
  for (i = 0; i < 32; i++) {
    Address = MAX_UINT64;
    AddressIndex = MAX_UINT8;
    for (j = 0; j < 32; j++) {
      if (DimmPtr->FlushAddress[j] <= LowAddress) {
        continue;
      }
      if (DimmPtr->FlushAddress[j] < Address) {
        Address = DimmPtr->FlushAddress[j];
        AddressIndex = j;
      }
    }
    ASSERT (AddressIndex != MAX_UINT8);
    LowAddress = Address;
    DimmPtr->FlushAddressOrderLut[i] = AddressIndex;
    DimmPtr->FlushAddressOrder[i] = Address;
  }

  return Status;
} // CrInfoInitDimm()

/**
Routine Description:GetNextChannel returns the next Channel
number based on what the currnet channel number is in
FMChannelInterBitmap.

  @param FMchannelInterBitmap - FMChannelInterbitmap in SAD
  @param CurrentCh   - Current Channel in FMChannelInterbitmap
  @retval NextCh     - Next Channel Number
**/
UINT8
GetNextChannel (
  UINT8     FMchannelInterBitmap,
  UINT8     CurrentCh
  )
{
  UINT8      NextCh = 0;

  switch (CurrentCh) {
  case 0:
    if (FMchannelInterBitmap & 0x02) {
      // Then next channel is 1
      NextCh = 1;
    } else if (FMchannelInterBitmap & 0x04) {
      NextCh = 2;
    }
    break;
  case 1:
    if (FMchannelInterBitmap & 0x04) {
      NextCh = 2;
    } else if (FMchannelInterBitmap & 0x01) {
      // Then next channel is 0
      NextCh = 0;
    }
    break;
  case 2:
    if (FMchannelInterBitmap & 0x01) {
      // Then next channel is 1
      NextCh = 0;
    } else if (FMchannelInterBitmap & 0x02) {
      NextCh = 1;
    }
    break;
  }
  return (NextCh);
}

/**

  Routine Description: initializes the PMEMInfo struct with the values
  provided as arguments into this function. Basically these values
  are used for the translation of Spa-To-Dpa conversion routines.

  @param[in] Socket      - Socket Number
  @param[in] Ch          - Channel number
  @param[in] Dimm        - Dimm number
  @param[in] SadSpaBase  - Base Address of the SAD Rule found
  @param[in] DpaBase     - Physical base address on the DIMM
  @param[in] ImcWays     - iMC Ways interleave
  @param[in] ChWays      - Channel Ways it is interleaved.
  @param[in] IntDimmNum  - Dimm Number in the SAD Interleave set

**/
VOID
InitPmemInfoStruct (
  UINT8           Socket,
  UINT8           Ch,
  UINT8           Dimm,
  UINT64          SadSpaBase,
  UINT64          DpaBase,
  UINT8           ImcWays,
  UINT8           ChWays,
  UINT8           IntDimmNum
  )
{
  PMEM_INFO       *PmemInfo;

  if (mCrInfo.NvdimmInfo.NumPMEMRgns >= ARRAY_SIZE (mCrInfo.NvdimmInfo.PMEMInfo)) {
    return;
  }

  if (mCrInfo.NvdimmInfo.NumPMEMRgns > 0) {
    //addresses in PmemInfo should be sorted
    ASSERT (mCrInfo.NvdimmInfo.PMEMInfo[mCrInfo.NvdimmInfo.NumPMEMRgns - 1].SADPMemBase <= SadSpaBase);
  }

  PmemInfo = &mCrInfo.NvdimmInfo.PMEMInfo[mCrInfo.NvdimmInfo.NumPMEMRgns];

  PmemInfo->SADPMemBase  = SadSpaBase;
  PmemInfo->DPABase      = DpaBase;
  PmemInfo->Socket       = Socket;
  PmemInfo->Channel      = Ch;
  PmemInfo->Dimm         = Dimm;
  PmemInfo->ChWays       = ChWays;
  PmemInfo->iMCWays      = ImcWays;
  PmemInfo->IntDimmNum   = IntDimmNum;

  mCrInfo.NvdimmInfo.NumPMEMRgns += 1;
}

/**

Routine Description:AddPMemInfoFor3ChWays adds pertinent
information regarding PMEM Region that has been found to the
NVDIMM Struct (for 3ch ways) and these fields are important to
the Spa-To-Dpa translation rountines ARS & RAS.
  @param iMCWays     - iMC Ways interleave
  @param SADSpaBase  - Base Address of the SAD Rule found
  @retval Status     - Status

**/
EFI_STATUS
AddPMemInfoFor3ChWays (
  UINT8          iMCWays,
  UINT64         SADSpaBase
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINT8           Soc, Ch, Dimm, i, k;
  UINT8           IntDimmNum = 0;
  UINT8           MaxImc;
  UINT8           NumChannelPerMc;
  struct          SADTable *pSAD;
  UINT64          SpaBase, DpaBase;
  NVDIMM          *pDimm = NULL;

  MaxImc = GetMaxImc ();
  NumChannelPerMc = GetNumChannelPerMc ();

  for (Soc = 0; Soc < MAX_SOCKET; Soc++) {
    for (i = 0; i < MAX_SAD_RULES; i++) {

      pSAD = &mSystemMemoryMap->Socket[Soc].SAD[i];
      if (!pSAD->local) {
        continue;
      }
      SpaBase = NvdimmGetSadBase (Soc, i);

      if (SpaBase == SADSpaBase) {
        if (mSystemMemoryMap->Socket[Soc].SadIntList[i][Soc * MaxImc]) {
          // iMC0 of this socket is involved in this interleave set,
          // Include these channels for the creation of Dimm Structs
          //
          Ch = (SpaBase / 4096) % 3;
          for (k = 0; k < CH_3WAY; k++) {
            Status = GetChannelDimm (Soc, Ch, &Dimm);
            ASSERT_EFI_ERROR (Status);
            pDimm = GetDimm (&mCrInfo.NvdimmInfo, Soc, Ch, Dimm, NVDIMM_FLAG_ENABLED);
            DpaBase = GetPmemDpaBase (SADSpaBase, Soc, Ch, Dimm);
            InitPmemInfoStruct (Soc, Ch, Dimm, SADSpaBase, DpaBase, iMCWays, CH_3WAY, IntDimmNum);
            //
            // Contact CLV before changing the trace below, test depends on it.
            //
            CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "S%d.C%d.D%d: Found PMEM, SPA = 0x%012llX, DPA = 0x%llx", Soc, Ch, Dimm, SADSpaBase, DpaBase));
            CRDEBUG ((DEBUG_INFO, ", iMCWays = %d, ChWays = %d, DIMM no in the interleave set %d\n", iMCWays, CH_3WAY, IntDimmNum));
            Ch = (Ch == CH_3WAY - 1) ? 0 : Ch + 1;
            IntDimmNum++;
          }
        }
        if (mSystemMemoryMap->Socket[Soc].SadIntList[i][(Soc * MaxImc) + 1]) {
          // If iMC1 of this socket is involved in this interleave set,
          // Include these channels for the creation of Dimm Structs
          Ch = (SpaBase / 4096) % 3 + NumChannelPerMc;
          for (k = 0; k < CH_3WAY; k++) {
            Status = GetChannelDimm (Soc, Ch, &Dimm);
            ASSERT_EFI_ERROR (Status);
            pDimm = GetDimm (&mCrInfo.NvdimmInfo, Soc, Ch, Dimm, NVDIMM_FLAG_ENABLED);
            DpaBase = GetPmemDpaBase (SADSpaBase, Soc, Ch, Dimm);
            InitPmemInfoStruct (Soc, Ch, Dimm, SADSpaBase, DpaBase, iMCWays, CH_3WAY, IntDimmNum);
            //
            // Contact CLV before changing the trace below, test depends on it.
            //
            CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "S%d.C%d.D%d: Found PMEM, SPA = 0x%012llX, DPA = 0x%llx", Soc, Ch, Dimm, SADSpaBase, DpaBase));
            CRDEBUG ((DEBUG_INFO, ", iMCWays = %d, ChWays = %d, DIMM no in the interleave set %d\n", iMCWays, CH_3WAY, IntDimmNum));
            Ch = (Ch == NumChannelPerMc + CH_3WAY - 1) ? NumChannelPerMc : Ch + 1;
            IntDimmNum++;
          }
        }
      }
    }
  }
  return Status;
} // AddPMemInfoFor3ChWays()


/**
Routine Description:AddPMemInfoFor2ChWays adds pertinent
information regarding PMEM Region that has been found to the
NVDIMM Struct (for 2ch ways) and these fields are important to
the Spa-To-Dpa translation rountines ARS & RAS.
  @param iMCWays     - iMC Ways interleave
  @param SADSpaBase  - Base Address of the SAD Rule found
  @retval Status     - Status
**/
EFI_STATUS
AddPMemInfoFor2ChWays (
  UINT8          iMCWays,
  UINT64         SADSpaBase
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINT8           Soc, Ch = 0, Dimm, i;
  UINT8           Imc;
  UINT8           ImcCh;
  UINT8           IntDimmNum = 0;
  UINT8           MaxImc;
  UINT8           NumChannelPerMc;
  struct          SADTable *pSAD;
  UINT64          SpaBase, DpaBase;
  NVDIMM          *pDimm = NULL;

  MaxImc = GetMaxImc ();
  NumChannelPerMc = GetNumChannelPerMc ();

  for (Soc = 0; Soc < MAX_SOCKET; Soc++) {

    for (i = 0; i < MAX_SAD_RULES; i++) {

      pSAD = &mSystemMemoryMap->Socket[Soc].SAD[i];
      if (!pSAD->local) {
        continue;
      }
      SpaBase = NvdimmGetSadBase (Soc, i);

      if (SpaBase == SADSpaBase) {

        for (Imc = 0; Imc < MaxImc; Imc++) {

          if (mSystemMemoryMap->Socket[Soc].SadIntList[i][Soc * MaxImc + Imc]) {
            //
            // This iMC is participating in 1 Channel Way Interleave set
            // Knowing it is 2 way interleaved, get the first channel.
            //
            ImcCh = BitmapGetBit (pSAD->FMchannelInterBitmap[Imc], 0);
            if (ImcCh >= NumChannelPerMc) {

              CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.MC%d: No channel found for 2ChWay interleaving in SAD[%d] (bitmap: %X)\n",
                      Soc, Imc, i, pSAD->FMchannelInterBitmap[Imc]));
              continue;
            }
            Ch = ConvertImcChToSktCh (Imc, ImcCh);
            Status = GetChannelDimm (Soc, Ch, &Dimm);
            ASSERT_EFI_ERROR (Status);
            pDimm = GetDimm (&mCrInfo.NvdimmInfo, Soc, Ch, Dimm, NVDIMM_FLAG_ENABLED);
            DpaBase = GetPmemDpaBase (SADSpaBase, Soc, Ch, Dimm);
            InitPmemInfoStruct (Soc, Ch, Dimm, SADSpaBase, DpaBase, iMCWays, CH_2WAY, IntDimmNum);
            //
            // Contact CLV before changing the trace below, test depends on it.
            //
            CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "S%d.C%d.D%d: Found PMEM, SPA = 0x%012llx, DPA = 0x%llx", Soc, Ch, Dimm, SADSpaBase, DpaBase));
            CRDEBUG ((DEBUG_INFO, ", iMCWays = %d, ChWays = %d, DIMM no in the interleave set %d\n", iMCWays, CH_2WAY, IntDimmNum));
            IntDimmNum++;

            ImcCh = BitmapGetBit (pSAD->FMchannelInterBitmap[Imc], ImcCh + 1);
            if (ImcCh >= NumChannelPerMc) {

              CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.MC%d: No second channel found for 2ChWay interleaving in SAD[%d] (bitmap: %X)\n",
                      Soc, Imc, i, pSAD->FMchannelInterBitmap[Imc]));
              continue;
            }
            Ch = ConvertImcChToSktCh (Imc, ImcCh);
            Status = GetChannelDimm (Soc, Ch, &Dimm);
            ASSERT_EFI_ERROR (Status);
            pDimm = GetDimm (&mCrInfo.NvdimmInfo, Soc, Ch, Dimm, NVDIMM_FLAG_ENABLED);
            DpaBase = GetPmemDpaBase (SADSpaBase, Soc, Ch, Dimm);
            InitPmemInfoStruct (Soc, Ch, Dimm, SADSpaBase, DpaBase, iMCWays, CH_2WAY, IntDimmNum);
            //
            // Contact CLV before changing the trace below, test depends on it.
            //
            CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "S%d.C%d.D%d: Found PMEM, SPA = 0x%012llX, DPA = 0x%llX", Soc, Ch, Dimm, SADSpaBase, DpaBase));
            CRDEBUG ((DEBUG_INFO, ", iMCWays = %d, ChWays = %d, DIMM no in the Interleave set %d\n", iMCWays, CH_2WAY, IntDimmNum));
            IntDimmNum++;
          }
        } // for (Imc...)
      }
    }
  }
  return Status;
} // AddPMemInfoFor2ChWays()


/**
Routine Description:AddPMemInfoFor1ChWays adds pertinent
information regarding PMEM Region that has been found to the
NVDIMM Struct (for 1ch ways) and these fields are important to
the Spa-To-Dpa translation rountines ARS & RAS.
  @param iMCWays     - iMC Ways interleave
  @param SADSpaBase  - Base Address of the SAD Rule found

  @return EFI_STATUS is returned.
**/
EFI_STATUS
AddPMemInfoFor1ChWays (
  UINT8          iMCWays,
  UINT64         SADSpaBase
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINT8           Soc, Ch = 0, Dimm, i;
  UINT8           Imc;
  UINT8           ImcCh;
  UINT8           IntDimmNum = 0;
  UINT8           MaxImc;
  UINT8           NumChannelPerMc;
  struct          SADTable *pSAD;
  UINT64          SpaBase, DpaBase;
  NVDIMM          *pDimm = NULL;

  MaxImc = GetMaxImc ();
  NumChannelPerMc = GetNumChannelPerMc ();

  for (Soc = 0; Soc < MAX_SOCKET; Soc++) {

    for (i = 0; i < MAX_SAD_RULES; i++) {

      pSAD = &mSystemMemoryMap->Socket[Soc].SAD[i];
      if (!pSAD->local) {
        continue;
      }
      SpaBase = NvdimmGetSadBase (Soc, i);

      if (SpaBase == SADSpaBase) {

        for (Imc = 0; Imc < MaxImc; Imc++) {

          if (mSystemMemoryMap->Socket[Soc].SadIntList[i][Soc * MaxImc + Imc]) {
            //
            // This iMC is participating in 1 Channel Way Interleave set
            // Knowing it is 1 way interleaved, get the channel.
            //
            ImcCh = BitmapGetBit (pSAD->FMchannelInterBitmap[Imc], 0);
            if (ImcCh >= NumChannelPerMc) {

              CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.MC%d: No channel found for 1ChWay interleaving in SAD[%d] (bitmap: %X)\n",
                      Soc, Imc, i, pSAD->FMchannelInterBitmap[Imc]));
              continue;
            }
            Ch = ConvertImcChToSktCh (Imc, ImcCh);
            Status = GetChannelDimm (Soc, Ch, &Dimm);
            ASSERT_EFI_ERROR (Status);
            pDimm = GetDimm (&mCrInfo.NvdimmInfo, Soc, Ch, Dimm, NVDIMM_FLAG_ENABLED);
            DpaBase = GetPmemDpaBase (SADSpaBase, Soc, Ch, Dimm);
            InitPmemInfoStruct (Soc, Ch, Dimm, SADSpaBase, DpaBase, iMCWays, CH_1WAY, IntDimmNum);
            //
            // Contact CLV before changing the trace below, test depends on it.
            //
            CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "S%d.C%d.D%d: Found PMEM, SPA = 0x%012llX, DPA = 0x%012llX", Soc, Ch, Dimm, SADSpaBase, DpaBase));
            CRDEBUG ((DEBUG_INFO, ", iMCWays = %d, ChWays = %d, DIMM no in the Interleave set %d\n", iMCWays, CH_1WAY, IntDimmNum));
            IntDimmNum++;
          }
        } // for (Imc...)
      }
    } // for (Sad...)
  } // for (Socket...)
  return Status;
} // AddPMemInfoFor1ChWays()


/**
Routine Description:BuildDimmsFor1ChWays builds Dimm info data
structs for each NVMDIMM's (found) in the Control Region when
the associated SAD Rule is 3 way channel interleaved.

NOTE: This is SKX function, not used with control region block decoder.

  @param iMCWays     - iMC Ways involved in the SAD Rule
  @param SADSpaBase  - Base Address associated with the SAD Rule
  @retval Status     - Status

  Notes: The ImcIncrements array below holds, the offset values
  of the Spa of the Channel of the iMC. Starting from 0, for
  iMC0-Ch0, the next one would be iMC0-Ch1, iMC0-Ch2 of socket0,
  then followed by iMC1-Ch0, iMC1-Ch1, iMC1-Ch2 of Socket0, then
  followed by Socket1 iMCs and Channels till Socket3 iMCs and
  Channels. By adding what is in this array to the Starting base
  address of the SAD rule, one can get the translated SPA Base
  Address of the Dimm in that particular Channel of the Socket.
**/
EFI_STATUS
BuildDimmsFor3ChWays (
  UINT8          iMCWays,
  UINT64         SADSpaBase
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINT8           Soc, Ch, Dimm, Sad, j, k;
  UINT8           IntDimmNum = 0;
  SAD_TABLE      *SadPtr;
  UINT8           MaxImc;
  UINT8           NumChannelPerMc;
  UINT64          SpaBase, DimmSpaBase;
  UINT32          ImcIncrements[48] = { // MAX_SOCKET * MAX_MC_CH * MAX_IMC (for 8 sockets)
                               0,         CTRL_RGN_GRANULARITY, (2   * CTRL_RGN_GRANULARITY), (18  * CTRL_RGN_GRANULARITY), (16  * CTRL_RGN_GRANULARITY), (17 * CTRL_RGN_GRANULARITY),
    (33  * CTRL_RGN_GRANULARITY), (34  * CTRL_RGN_GRANULARITY), (32  * CTRL_RGN_GRANULARITY), (48  * CTRL_RGN_GRANULARITY), (49  * CTRL_RGN_GRANULARITY), (50 * CTRL_RGN_GRANULARITY),
    (66  * CTRL_RGN_GRANULARITY), (64  * CTRL_RGN_GRANULARITY), (65  * CTRL_RGN_GRANULARITY), (81  * CTRL_RGN_GRANULARITY), (82  * CTRL_RGN_GRANULARITY), (80 * CTRL_RGN_GRANULARITY),
    (96  * CTRL_RGN_GRANULARITY), (97  * CTRL_RGN_GRANULARITY), (98  * CTRL_RGN_GRANULARITY), (114 * CTRL_RGN_GRANULARITY), (112 * CTRL_RGN_GRANULARITY), (113 * CTRL_RGN_GRANULARITY),
    (129 * CTRL_RGN_GRANULARITY), (130 * CTRL_RGN_GRANULARITY), (128 * CTRL_RGN_GRANULARITY), (144 * CTRL_RGN_GRANULARITY), (145 * CTRL_RGN_GRANULARITY), (146 * CTRL_RGN_GRANULARITY),
    (162 * CTRL_RGN_GRANULARITY), (160 * CTRL_RGN_GRANULARITY), (161 * CTRL_RGN_GRANULARITY), (177 * CTRL_RGN_GRANULARITY), (178 * CTRL_RGN_GRANULARITY), (176 * CTRL_RGN_GRANULARITY),
    (192 * CTRL_RGN_GRANULARITY), (193 * CTRL_RGN_GRANULARITY), (194 * CTRL_RGN_GRANULARITY), (210 * CTRL_RGN_GRANULARITY), (208 * CTRL_RGN_GRANULARITY), (209 * CTRL_RGN_GRANULARITY),
    (225 * CTRL_RGN_GRANULARITY), (226 * CTRL_RGN_GRANULARITY), (224 * CTRL_RGN_GRANULARITY), (240 * CTRL_RGN_GRANULARITY), (241 * CTRL_RGN_GRANULARITY), (242 * CTRL_RGN_GRANULARITY)
                                      };

  j = 0;
  MaxImc = GetMaxImc ();
  NumChannelPerMc = GetNumChannelPerMc ();

  for (Soc = 0; Soc < MAX_SOCKET; Soc++) {

    for (Sad = 0; Sad < MAX_SAD_RULES; Sad++) {

      SadPtr = &mSystemMemoryMap->Socket[Soc].SAD[Sad];
      if (!SadPtr->local) {
        continue;
      }
      SpaBase = NvdimmGetSadBase (Soc, Sad);

      if (SpaBase == SADSpaBase) {

        if (mSystemMemoryMap->Socket[Soc].SadIntList[Sad][Soc * MaxImc]) {
          //
          // iMC0 of this socket is involved in this interleave set,
          // Include these channels for the creation of Dimm Structs

          // Determine starting channel number
          Ch = (SpaBase/CTRL_RGN_GRANULARITY) % CH_3WAY;

          for (k = 0; k < CH_3WAY; k++) {
            Status = GetChannelDimm (Soc, Ch, &Dimm);
            ASSERT_EFI_ERROR (Status);
            CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "S%d.C%d.D%d: Found An NVDIMM\n", Soc, Ch, Dimm));
            CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "iMCWays = %d, ChWays = %d\n", iMCWays, CH_3WAY));
            DimmSpaBase = SADSpaBase + ImcIncrements[j];
            CrInfoInitDimm (SADSpaBase, DimmSpaBase, Soc, Ch, Dimm, iMCWays, CH_3WAY, IntDimmNum, SadPtr->FMchannelInterBitmap[IMC0]);
            j++;
            Ch = (Ch == CH_3WAY - 1) ? 0 : Ch + 1;
            CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Dimm Num in the Interleave set = %d\n", IntDimmNum));
            IntDimmNum++;
          }
        }
        if (mSystemMemoryMap->Socket[Soc].SadIntList[Sad][(Soc * MaxImc) + 1]) {
          // If iMC1 of this socket is involved in this interleave set,
          // Include these channels for the creation of Dimm Structs

          // Determine starting channel number
          Ch = ((SpaBase/CTRL_RGN_GRANULARITY) % CH_3WAY) + NumChannelPerMc;

          for (k = 0; k < CH_3WAY; k++) {
            Status = GetChannelDimm (Soc, Ch, &Dimm);
            ASSERT_EFI_ERROR (Status);
            CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "S%d.C%d.D%d: Found An NVDIMM\n", Soc, Ch, Dimm));
            CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "iMCWays = %d, ChWays = %d\n", iMCWays, CH_3WAY));
            DimmSpaBase = SADSpaBase + ImcIncrements[j];
            CrInfoInitDimm (SADSpaBase, DimmSpaBase, Soc, Ch, Dimm, iMCWays, CH_3WAY, IntDimmNum, SadPtr->FMchannelInterBitmap[IMC1]);
            j++;
            Ch = (Ch == NumChannelPerMc + CH_3WAY - 1) ? NumChannelPerMc : Ch + 1;
            CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Dimm Num in the Interleave set = %d\n", IntDimmNum));
            IntDimmNum++;
          }
        }
      }
    }
  }
  ASSERT (mCrInfo.NvdimmInfo.NumNvdimms <= MAX_SYS_DIMM);
  //
  // Contact CLV before changing the trace below, test depends on it.
  //
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Number of NVDIMMs found so far = %d\n", mCrInfo.NvdimmInfo.NumNvdimms));
  return Status;
}


/**
Routine Description:BuildDimmsFor1ChWays builds Dimm info data
structs for each NVMDIMM's (found) in the Control Region when
the associated SAD Rule is 2 way channel interleaved.

NOTE: This is SKX function, not used with control region block decoder.

  @param iMCWays     - iMC Ways involved in the SAD Rule
  @param SADSpaBase  - Base Address associated with the SAD Rule
  @retval Status     - Status

**/
EFI_STATUS
BuildDimmsFor2ChWays (
  UINT8          iMCWays,
  UINT64         SADSpaBase
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINT8           Soc, Ch = 0, Dimm, Sad;
  UINT8           IntDimmNum = 0;
  UINT8           MaxImc;
  UINT8           NumChannelPerMc;
  UINT32          ChGran = mCrInfo.NvdimmInfo.ChGran;
  UINT32          ImcIncrement;
  UINT64          SpaBase, DimmSpaBase;
  SAD_TABLE      *SadPtr;

  ImcIncrement = 0;
  DimmSpaBase = SADSpaBase;
  MaxImc = GetMaxImc ();
  NumChannelPerMc = GetNumChannelPerMc ();

  for (Soc = 0; Soc < MAX_SOCKET; Soc++) {

    for (Sad = 0; Sad < MAX_SAD_RULES; Sad++) {

      SadPtr = &mSystemMemoryMap->Socket[Soc].SAD[Sad];
      if (!SadPtr->local) {
        continue;
      }
      SpaBase = NvdimmGetSadBase (Soc, Sad);

      if (SpaBase == SADSpaBase) {

        if (mSystemMemoryMap->Socket[Soc].SadIntList[Sad][Soc * MaxImc]) {
          //
          // iMC0 is participating in 2 Channel Way Interleave set
          // Knowing it is 2 way interleaved, get the first channel
          //
          if (SadPtr->FMchannelInterBitmap[IMC0] & 0x01) {
            Ch = 0;
          } else if (SadPtr->FMchannelInterBitmap[IMC0] & 0x02) {
            Ch = 1;
          }

          Status = GetChannelDimm (Soc, Ch, &Dimm);
          ASSERT_EFI_ERROR (Status);
          CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "S%d.C%d.D%d: Found An NVDIMM\n", Soc, Ch, Dimm));
          CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "iMCWays = %d, ChWays = %d\n", iMCWays, CH_2WAY));
          DimmSpaBase += ImcIncrement;
          CrInfoInitDimm (SADSpaBase, DimmSpaBase, Soc, Ch, Dimm, iMCWays, CH_2WAY, IntDimmNum, SadPtr->FMchannelInterBitmap[IMC0]);
          IntDimmNum++;
          Ch = GetNextChannel (SadPtr->FMchannelInterBitmap[IMC0], Ch);
          Status = GetChannelDimm (Soc, Ch, &Dimm);
          ASSERT_EFI_ERROR (Status);
          CRDEBUG ((EFI_D_ERROR, CR_INFO_STR("CRP") "S%d.C%d.D%d: Found An NVDIMM\n", Soc, Ch, Dimm));
          CRDEBUG ((EFI_D_ERROR, CR_INFO_STR ("CRP") "iMCWays = %d, ChWays = %d\n", iMCWays, CH_2WAY));
          DimmSpaBase += ChGran;
          CrInfoInitDimm (SADSpaBase, DimmSpaBase, Soc, Ch, Dimm, iMCWays, CH_2WAY, IntDimmNum, SadPtr->FMchannelInterBitmap[IMC0]);
          IntDimmNum++;
          ImcIncrement = mCrInfo.NvdimmInfo.SktGran;
        }
        if (mSystemMemoryMap->Socket[Soc].SadIntList[Sad][(Soc * MaxImc) + 1]) {
          // iMC1 is participating in 2 Channel Way Interleave set
          if (SadPtr->FMchannelInterBitmap[IMC1] & 0x01) {
            Ch = 3;
          } else if (SadPtr->FMchannelInterBitmap[IMC1] & 0x02) {
            Ch = 4;
          }

          Status = GetChannelDimm (Soc, Ch, &Dimm);
          ASSERT_EFI_ERROR (Status);
          CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "S%d.C%d.D%d: Found An NVDIMM\n", Soc, Ch, Dimm));
          CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "iMCWays = %d, ChWays = %d\n", iMCWays, CH_2WAY));
          DimmSpaBase += ImcIncrement;
          CrInfoInitDimm (SADSpaBase, DimmSpaBase, Soc, Ch, Dimm, iMCWays, CH_2WAY, IntDimmNum, SadPtr->FMchannelInterBitmap[IMC1]);
          IntDimmNum++;
          Ch = GetNextChannel (SadPtr->FMchannelInterBitmap[IMC1], (Ch - NumChannelPerMc));
          Ch += NumChannelPerMc;
          Status = GetChannelDimm (Soc, Ch, &Dimm);
          ASSERT_EFI_ERROR (Status);
          DimmSpaBase += ChGran;
          CrInfoInitDimm (SADSpaBase, DimmSpaBase, Soc, Ch, Dimm, iMCWays, CH_2WAY, IntDimmNum, SadPtr->FMchannelInterBitmap[IMC1]);
          IntDimmNum++;
          ImcIncrement = mCrInfo.NvdimmInfo.SktGran;
        }
      }
    }
  }
  ASSERT (mCrInfo.NvdimmInfo.NumNvdimms <= MAX_SYS_DIMM);
  //
  // Contact CLV before changing the trace below, test depends on it.
  //
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Number of NVDIMMs found so far = %d\n", mCrInfo.NvdimmInfo.NumNvdimms));
  return (Status);
} // BuildDimmsFor2ChWays()


/**
  @brief BuildDimmsFor1ChWays builds DIMM info data structs for each NVMDIMM
         (found) in the Control Region when the associated SAD Rule is 1 way channel interleaved.

  NOTE: This is SKX function, not used with control region block decoder.

  @param[in] iMCWays     - iMC Ways involved in the SAD Rule
  @param[in] SADSpaBase  - Base Address associated with the SAD Rule

  @return EFI_STATUS is retured.
**/
EFI_STATUS
BuildDimmsFor1ChWays (
  IN UINT8       iMCWays,
  IN UINT64      SADSpaBase
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINT8           Soc, Ch = 0, Dimm, Sad;
  UINT8           IntDimmNum = 0;
  UINT8           MaxImc;
  SAD_TABLE      *SadPtr;
  UINT32          ImcIncrement;
  UINT64          SpaBase, DimmSpaBase;

  ImcIncrement = 0;
  DimmSpaBase = SADSpaBase;
  MaxImc = GetMaxImc ();

  for (Soc = 0; Soc < MAX_SOCKET; Soc++) {

    for (Sad = 0; Sad < MAX_SAD_RULES; Sad++) {

      SadPtr = &mSystemMemoryMap->Socket[Soc].SAD[Sad];
      if (!SadPtr->local) {
        continue;
      }
      SpaBase = NvdimmGetSadBase (Soc, Sad);

      if (SpaBase == SADSpaBase) {

        if (mSystemMemoryMap->Socket[Soc].SadIntList[Sad][Soc * MaxImc]) {
          //
          // iMC0 is participating in 1 Channel Way Interleave set
          // Knowing it is 1 way interleaved, get the channel
          //
          if (SadPtr->FMchannelInterBitmap[IMC0] & 0x01) {
            Ch = 0;
          } else if (SadPtr->FMchannelInterBitmap[IMC0] & 0x02) {
            Ch = 1;
          } else if (SadPtr->FMchannelInterBitmap[IMC0] & 0x04) {
            Ch = 2;
          }

          Status = GetChannelDimm (Soc, Ch, &Dimm);
          ASSERT_EFI_ERROR (Status);
          CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "S%d.C%d.D%d: Found An NVDIMM\n", Soc, Ch, Dimm));
          CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "iMCWays = %d, ChWays = %d\n", iMCWays, CH_1WAY));
          DimmSpaBase += ImcIncrement;
          CrInfoInitDimm (SADSpaBase, DimmSpaBase, Soc, Ch, Dimm, iMCWays, CH_1WAY, IntDimmNum, SadPtr->FMchannelInterBitmap[IMC0]);
          IntDimmNum++;
          ImcIncrement = mCrInfo.NvdimmInfo.SktGran;
        }
        if (mSystemMemoryMap->Socket[Soc].SadIntList[Sad][(Soc * MaxImc) + 1]) {
          if (SadPtr->FMchannelInterBitmap[IMC1] & 0x01) {
            Ch = 3;
          } else if (SadPtr->FMchannelInterBitmap[IMC1] & 0x02) {
            Ch = 4;
          } else if (SadPtr->FMchannelInterBitmap[IMC1] & 0x04) {
            Ch = 5;
          }

          Status = GetChannelDimm (Soc, Ch, &Dimm);
          ASSERT_EFI_ERROR (Status);
          CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "S%d.C%d.D%d: Found An NVDIMM\n", Soc, Ch, Dimm));
          CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "iMCWays = %d, ChWays = %d\n", iMCWays, CH_1WAY));
          DimmSpaBase += ImcIncrement;
          CrInfoInitDimm (SADSpaBase, DimmSpaBase, Soc, Ch, Dimm, iMCWays, CH_1WAY, IntDimmNum, SadPtr->FMchannelInterBitmap[IMC1]);
          IntDimmNum++;
          ImcIncrement = mCrInfo.NvdimmInfo.SktGran;
        }
      }
    }
  }
  ASSERT (mCrInfo.NvdimmInfo.NumNvdimms <= MAX_SYS_DIMM);
  //
  // Contact CLV before changing the trace below, test depends on it.
  //
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Number of NVDIMMs found so far = %d\n", mCrInfo.NvdimmInfo.NumNvdimms));
  return (Status);
} // BuildDimmsFor1ChWays()


/**
  @brief Build DIMM data in CrInfo structure with data from Block Decoder.

  @return Void.
**/
VOID
CrInfoBuildDimmInfoForCrbd (
  VOID
  )
{
  EFI_STATUS Status;
  UINT8  Skt;
  UINT8  Mc;
  UINT8  Ch;
  UINT8  Dimm;
  UINT8  SktBit;
  UINT8  ImcCh;
  UINT8  MaxImc;
  UINT8  MaxChDdr;
  UINT8  NumChannelPerMc;

  MaxImc = GetMaxImc ();
  MaxChDdr = GetMaxChDdr ();
  NumChannelPerMc = GetNumChannelPerMc ();

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "CRBD base:   0x%08X (0x%012llX)\n",
          mSystemMemoryMap->BlockDecoderData.BlockDecoderBase,
          MEM_64MB_TO_BYTES (mSystemMemoryMap->BlockDecoderData.BlockDecoderBase)));
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "CRBD limit:  0x%08X (0x%llX)\n",
          mSystemMemoryMap->BlockDecoderData.BlockDecoderLimit,
          MEM_64MB_TO_BYTES (mSystemMemoryMap->BlockDecoderData.BlockDecoderLimit)));
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "CRBD socket enable: 0x%02X\n",
          mSystemMemoryMap->BlockDecoderData.BlockSocketEnable));
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "CRBD MC/CH  enable:"));

  for (Skt = 0; Skt < NELEMENTS (mSystemMemoryMap->BlockDecoderData.BlockMcChEn); Skt++) {
    for (Mc = 0; Mc < MaxImc; Mc++) {
      for (Ch = 0; Ch < NumChannelPerMc; Ch++) {
        CRDEBUG ((DEBUG_INFO, " %01X", mSystemMemoryMap->BlockDecoderData.BlockMcChEn[Skt][Mc][Ch]));
      }
    }
  }
  CRDEBUG ((DEBUG_INFO, "\n"));
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "MaxImc %d, NumChannelPerMc %d, MaxChDdr %d, MAX_DIMM %d\n",
          MaxImc, NumChannelPerMc, MaxChDdr, MAX_DIMM));

  mCrInfo.CrbdAddrBase = MEM_64MB_TO_BYTES (mSystemMemoryMap->BlockDecoderData.BlockDecoderBase);
  mCrInfo.CrbdAddrLimit = MEM_64MB_TO_BYTES (mSystemMemoryMap->BlockDecoderData.BlockDecoderLimit);
  mCrInfo.NvdimmInfo.SktGran = CR_CRBD_INTWAYS * CR_CRBD_LINESIZE;
  mCrInfo.NvdimmInfo.ChGran = CR_CRBD_LINESIZE;
  for (Skt = 0, SktBit = 1; Skt < MAX_SOCKET; Skt++, SktBit <<= 1) {

    if (!(mSystemMemoryMap->BlockDecoderData.BlockSocketEnable & SktBit)) {
      continue;
    }
    for (Ch = 0; Ch < NumChannelPerMc; Ch++) {
      for (Mc = 0; Mc < MaxImc; Mc++) {
        if (mSystemMemoryMap->BlockDecoderData.BlockMcChEn[Skt][Mc][Ch] == 0) {
          continue;
        }
        ImcCh = ConvertImcChToSktCh (Mc, Ch);
        Status = GetChannelDimm (Skt, ImcCh, &Dimm);
        if (EFI_ERROR (Status)) {

          CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d: NVDIMM enabled in Block Decoder but not present in Memory Map HOB\n",
                  Skt, ImcCh));
          ASSERT_EFI_ERROR (Status);
          continue;
        }
        CrInfoInitDimm (mCrInfo.CrbdAddrBase, mCrInfo.CrbdAddrBase | CR_CRBD_DIMM_OFFSET (Skt, Mc, Ch),
                                 Skt, ImcCh, Dimm,
                                 1, CR_CRBD_INTWAYS, CR_CRBD_DIMM_INDEX (Skt, Mc, Ch), 0xF);
      }
    }
  }

  ASSERT (mCrInfo.NumControlRegions < MAX_CONTROL_REGIONS);

  mCrInfo.ControlRegion[mCrInfo.NumControlRegions].Base = mCrInfo.CrbdAddrBase;
  mCrInfo.ControlRegion[mCrInfo.NumControlRegions].Size = mCrInfo.CrbdAddrLimit - mCrInfo.CrbdAddrBase;
  mCrInfo.NumControlRegions++;

  //
  // Contact CLV before changing the trace below, test depends on it.
  //
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Number of NVDIMMs found so far = %d\n", mCrInfo.NvdimmInfo.NumNvdimms));
} // CrInfoBuildDimmInfo()


VOID
CrInfoUpdateDummyDimmInfo (
  VOID
  )
{
  NVDIMM   *DimmPtr;
  UINT8    i;

  // Populate remaining control regions with dummy information for ASL DSM implementation
  for (i = mCrInfo.NumControlRegions; i <= MAX_CONTROL_REGIONS; i++) {
    mCrInfo.ControlRegion[i].Base = (UINT64) &mCrInfo.ControlRegionDummy;
    mCrInfo.ControlRegion[i].Size = sizeof (mCrInfo.ControlRegionDummy);
    mCrInfo.FlushRegion[i].Base   = (UINT64) &mCrInfo.FlushRegionDummy;
    mCrInfo.FlushRegion[i].End    = mCrInfo.FlushRegion[i].Base + sizeof (mCrInfo.FlushRegionDummy);
  }
  // Prepare dummy NVDIMM for ASL DSM implementation
  DimmPtr = &mCrInfo.NvdimmInfo.Nvdimms[MAX_SYS_DIMM];
  DimmPtr->ControlRegionIndex = MAX_CONTROL_REGIONS;
  DimmPtr->FlushRegionIndex = MAX_CONTROL_REGIONS;
  DimmPtr->SADSpaBase = mCrInfo.ControlRegion[MAX_CONTROL_REGIONS].Base;
  DimmPtr->OSMailboxSPA.pCommand = (UINT64) &mCrInfo.ControlRegionDummy.Command;
  DimmPtr->OSMailboxSPA.pStatus = (UINT64) &mCrInfo.ControlRegionDummy.Status;
  DimmPtr->BsrSpa = (UINT64) &mCrInfo.ControlRegionDummy.Bsr;
  for (i = 0; i < NUM_PAYLOAD_REG; i+= 2) {
    DimmPtr->OSMailboxSPA.pInPayloadOrder[i / 2] = (UINT64) &mCrInfo.ControlRegionDummy.InPayload[i];
    DimmPtr->OSMailboxSPA.pOutPayloadOrder[i / 2] = (UINT64) &mCrInfo.ControlRegionDummy.OutPayload[i];
    DimmPtr->OSMailboxSPA.InPayloadOrderLut[i / 2] = i;
    DimmPtr->OSMailboxSPA.OutPayloadOrderLut[i / 2] = i;
  }
  for (i = 0; i < 32; i++) {
    DimmPtr->FlushAddressOrder[i] = (UINT64) &mCrInfo.FlushRegionDummy.FlushReg[i];
    DimmPtr->FlushAddressOrderLut[i] = i;
  }
}


/**
  @brief UpdateDimmInfo updates NVDIMM info data in mCrInfo struct

  It finds the NVMDIMM's (found) Control Region and other info about
  the Dimm. If there is more than one dimm in this sad rule, then
  it is interleaved and need to account for each one, so creates a
  separate dimminfo struct for each one.

  @param[in] MemType - Memory type, either MemType1lmAppDirect or MemType1lmCtrl.

  @return Void.
**/
VOID
CrInfoUpdateDimmInfo (
  UINT16 MemType
  )
{
  UINT64          SpaBase;
  UINT64          Size;
  UINT8           i, ChWays, iMCWays, McIndex;
  UINT8           Socket;
  STATIC BOOLEAN  FirstTime = TRUE;
  MEM_TYPE        SadType;
  SAD_TABLE      *SadPtr;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

    for (i = 0; i < MAX_SAD_RULES; i++) {

      SadPtr = &mSystemMemoryMap->Socket[Socket].SAD[i];
      if (SadPtr->Enable == 0 || SadPtr->local == 0) {
        continue;
      }
      //
      // Only AppDirect and control regions are supported
      //
      if(IsMemTypeAppDirect (SadPtr->type)) {
        SadType = MemType1lmAppDirect;
      } else if (SadPtr->type == MemType1lmCtrl) {
        SadType = MemType1lmCtrl;
      } else {
        continue;
      }
      //
      // Skip if the MemType doesn't match
      //
      if (MemType != SadType) {
        continue;
      }
      //
      // Ignore the PMEM region if it is a memory hole.
      //
      if (IsMemTypeReserved (SadPtr->type)) {
        continue;
      }
      //
      // Skip if the SAD has already been added
      //
      if (IsRedundantCtrlRgnSAD(SadPtr)) {
        continue;
      }

      if (FirstTime && (MemType == MemType1lmCtrl)) {

        mCrInfo.NvdimmInfo.SktGran = GetSocketGranularity (SadPtr);
        mCrInfo.NvdimmInfo.ChGran  = GetChannelGranularity (SadPtr);
        FirstTime       = FALSE;
      }
      SpaBase = NvdimmGetSadBase (Socket, i);
      Size = ((UINT64)mSystemMemoryMap->Socket[Socket].SAD[i].Limit << BITS_64MB_TO_BYTES) - SpaBase;

      // Get iMC Interleave ways for this Cntrl Region SAD
      iMCWays = GetImcWays (Socket, i);

      // Determine which IMC channel bitmap to use
      McIndex = GetMcIndexFromBitmap (SadPtr->imcInterBitmap);

      // Get the Channel ways for this SAD
      ChWays = GetChInterleave (Socket, McIndex, i);

      CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "CrInfoUpdateDimmInfo: Skt %d, SAD Base 0x%012llX, Size 0x%012llX",
              Socket, SpaBase, Size));
      CRDEBUG ((DEBUG_INFO, ", Type 0x%X, McWays %d, ChWays %d, Gran %d\n",
              SadPtr->type, iMCWays, ChWays, SadPtr->granularity));

      if (MemType == MemType1lmCtrl) {
        ASSERT (mCrInfo.NumControlRegions < MAX_CONTROL_REGIONS);
        mCrInfo.ControlRegion[mCrInfo.NumControlRegions].Base = SpaBase;
        switch (ChWays) {
          case CH_3WAY:
            BuildDimmsFor3ChWays(iMCWays, SpaBase);
            break;
          case CH_2WAY:
            BuildDimmsFor2ChWays(iMCWays, SpaBase);
            break;
          case CH_1WAY:
            BuildDimmsFor1ChWays(iMCWays, SpaBase);
            break;
          default:
            ASSERT (FALSE);
        }
        if (mCrInfo.ControlRegion[mCrInfo.NumControlRegions].Size) {
          CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "UpdateDimmInfo: Region index %d: 0x%lx @ 0x%lx\n", mCrInfo.NumControlRegions, mCrInfo.ControlRegion[mCrInfo.NumControlRegions].Size, mCrInfo.ControlRegion[mCrInfo.NumControlRegions].Base));
          mCrInfo.NumControlRegions++;
        }
      } else if (MemType == MemType1lmAppDirect) {
        switch (ChWays) {
          case CH_3WAY:
            AddPMemInfoFor3ChWays(iMCWays, SpaBase);
            break;
          case CH_2WAY:
            AddPMemInfoFor2ChWays(iMCWays, SpaBase);
            break;
          case CH_1WAY:
            AddPMemInfoFor1ChWays(iMCWays, SpaBase);
            break;
          default:
            ASSERT (FALSE);
        }
      }
    }
  }
}// CrInfoUpdateDimmInfo()



/**
  @brief Update CrInfo structure for Disabled NVDIMMs

  @param VOID       - VOID

  @retval VOID      - None
**/
VOID
CrInfoUpdateForDisabledNvdimms (
  VOID
  )
{
  UINT16  Socket;
  UINT8   Ch;
  UINT8   Dimm;
  UINT8   MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (IsNvmDimm ((UINT8) Socket, Ch, Dimm) && !mSystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].Enabled) {
          mCrInfo.NvdimmInfo.Nvdimms[mCrInfo.NvdimmInfo.NumNvdimms].Ch = Ch;
          mCrInfo.NvdimmInfo.Nvdimms[mCrInfo.NvdimmInfo.NumNvdimms].SocketId = Socket;
          mCrInfo.NvdimmInfo.Nvdimms[mCrInfo.NvdimmInfo.NumNvdimms].Dimm = Dimm;
          mCrInfo.NvdimmInfo.Nvdimms[mCrInfo.NvdimmInfo.NumNvdimms].SADSpaBase = SPA_NOT_MAPPED;
          mCrInfo.NvdimmInfo.NumNvdimms++;
          CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "S%d.C%d.D%d: UpdateCrInfoForDisabledNvdimms\n", Socket, Ch, Dimm));
        }
      }
    }
  }
}


/**

Routine Description: SetMemAttributes updates the Attributes for each CTRL
region and Block window GCD Memory Map entry.

  @param VOID       - None
  @retval VOID      - None

**/
VOID
SetMemAttributes (
  VOID
  )
{
  SAD_TABLE                       *SadTbl;
  UINT8                           SadRule;
  UINT8                           Socket;
  EFI_STATUS                      Status = EFI_SUCCESS;
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR Descriptor;

  ZeroMem (&Descriptor, sizeof (Descriptor));

  if (mSystemMemoryMap->BlockDecoderData.BlockSocketEnable != 0) {
    //
    // Get memory range descriptor
    //
    Status = gDS->GetMemorySpaceDescriptor (mCrInfo.CrbdAddrBase , &Descriptor);
    if (!EFI_ERROR (Status)) {
      //
      // Set Runtime flag and UC flag
      //
      Descriptor.Attributes |= (EFI_MEMORY_UC | EFI_MEMORY_RUNTIME);

      CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Setting runtime flags for memory range 0x%llX - 0x%llX, Attributes = 0x%llX\n",
        Descriptor.BaseAddress, Descriptor.BaseAddress + Descriptor.Length, Descriptor.Attributes));
      //
      // Update memory range attributes
      //
      Status = gDS->SetMemorySpaceAttributes (Descriptor.BaseAddress, Descriptor.Length, Descriptor.Attributes);
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "Setting runtime flags for memory range 0x%llX - 0x%llX, Attributes = 0x%llX failed (status %r)\n",
          Descriptor.BaseAddress, Descriptor.BaseAddress + Descriptor.Length, Descriptor.Attributes, Status));
      }
    } else {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "Getting Memory Space descriptor for address 0x%llX failed (status %r)\n",
        mCrInfo.CrbdAddrBase, Status));
    }
  } else {
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      //
      // SAD[0] will never be a CTRL Region or Block window
      //
      for (SadRule = 1; SadRule < MAX_SAD_RULES; SadRule++) {
        SadTbl = &mSystemMemoryMap->Socket[Socket].SAD[SadRule];
        //
        // Skip disabled and remote rules
        //
        if (SadTbl->Enable == 0 || SadTbl->local == 0) {
          continue;
        }
        //
        // Find NVDIMM control region rule
        //
        if (SadTbl->type == MemType1lmCtrl) {
          //
          // Get memory range descriptor
          //
          Status = gDS->GetMemorySpaceDescriptor (MEM_64MB_TO_BYTES ((UINT64)mSystemMemoryMap->Socket[Socket].SAD[SadRule - 1].Limit), &Descriptor);
          if (!EFI_ERROR (Status)) {
            //
            // Set Runtime flag
            //
            Descriptor.Attributes |= EFI_MEMORY_RUNTIME;

            //
            // For Control Regions set UC flag
            //
            if (SadTbl->type == MemType1lmCtrl) {
              Descriptor.Attributes |= EFI_MEMORY_UC;
            }

            CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Setting runtime flags for memory range 0x%llX - 0x%llX, Attributes = 0x%llX\n",
                    Descriptor.BaseAddress, Descriptor.BaseAddress + Descriptor.Length, Descriptor.Attributes));
            //
            // Update memory range attributes
            //
            Status = gDS->SetMemorySpaceAttributes (Descriptor.BaseAddress, Descriptor.Length, Descriptor.Attributes);
            if (EFI_ERROR (Status)) {
              CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "Setting runtime flags for memory range 0x%llX - 0x%llX, Attributes = 0x%llX failed (status %r)\n",
                Descriptor.BaseAddress, Descriptor.BaseAddress + Descriptor.Length, Descriptor.Attributes, Status));
            }
          } else {
            CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "Getting Memory Space descriptor for address 0x%llX failed (status %r)\n",
                MEM_64MB_TO_BYTES ((UINT64)mSystemMemoryMap->Socket[Socket].SAD[SadRule - 1].Limit), Status));
          }
        }
      }
    }
  }
  //
  // For debug builds halt on error
  //
  ASSERT_EFI_ERROR (Status);
}

/**
  @brief Initialize structure shared with ACPI ASL code for NVDIMMs.

  @return Void.
**/
VOID
NvdimmAcpiInterfaceInit (
  NVDIMM_ACPI_SMM_INTERFACE *AcpiPtr
  )
{
  UINT8      Skt;
  UINT8      Sad;
  UINT8      Imc;
  UINT8      Ch;
  UINT8      ImcCh;
  UINT8      Bitmap;
  UINT8      NumChannelPerMc;
  SAD_TABLE  *SadPtr;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "ACPI<->SMM interface address: 0x%llX\n", mNvdimmAcpiSmmInterface));

  ZeroMem (AcpiPtr, sizeof (*AcpiPtr));
  SetMem (&AcpiPtr->DimmOverwriteStatus[0][0], sizeof (mNvdimmAcpiSmmInterface->DimmOverwriteStatus), FIS_STS_DATA_NOT_SET);
  NumChannelPerMc = GetNumChannelPerMc ();

  for (Skt = 0; Skt < NELEMENTS (mSystemMemoryMap->Socket); Skt++) {

    for (Sad = 0; Sad < NELEMENTS (mSystemMemoryMap->Socket[0].SAD); Sad++) {

      SadPtr = &mSystemMemoryMap->Socket[Skt].SAD[Sad];
      if (!SadPtr->Enable || !SadPtr->local || !IsMemType2lm (SadPtr->type)) {
        continue;
      }
      for (Imc = 0; Imc < NELEMENTS (SadPtr->channelInterBitmap); Imc++) {

        for (Ch = 0, Bitmap = SadPtr->channelInterBitmap[Imc];
             Bitmap != 0; Ch++, Bitmap >>= 1) {
          if (Bitmap & 1) {
            //
            // If NVDIMM participate in 2LM region mark it not available for Overwrite DIMM operation.
            // Starting overwrite would disconnect OS from RAM access and thus would crush the system.
            //
            ImcCh = ConvertImcChToSktCh (Imc, Ch);
            AcpiPtr->DimmOverwriteStatus[Skt][ImcCh] = FIS_STS_INCOMPATIBLE_DIMM;
          }
        }
      }
    }
  }
  //
  // NoDdrtSmi should reflect DDRT alerts to ERR0 redirection enabled.
  // Currently it is always enabled to w/a problem with DDRT alerts lost sometimes.
  //
  AcpiPtr->ImplementationControl.NoDdrtSmi = 1;
  AcpiPtr->ArsStatusBufferLock = 0xFF;

  AcpiPtr->ImplementationControl.LsxAsl = (mSystemMemoryMap->MemSetup.LsxImplementation == LSX_IMPLEMENTATION_ASL);
  AcpiPtr->DebugLevel = GetDebugPrintErrorLevel();
  AcpiPtr->DebugInterface = mSystemMemoryMap->MemSetup.dfxMemSetup.DfxCrAcpiDebugInterface;
  AcpiPtr->StopArsWithOverflow = 1;
  AcpiPtr->MaxMcChannel = NumChannelPerMc;
} // NvdimmAcpiInterfaceInit ()


/**
  Flushes specified uncorrectable block from cache.

  Optionally reads data into the cache prior to flushing (specifically
  needed in Clear Uncorrectable Error flow).

  @param[in]  Address          Pointer to uncorrectable-block-aligned SPA.
  @param[in]  ReadBeforeFlush  TRUE:  reads data prior to flushing
                               FALSE: only performs cache flush
**/
STATIC
VOID
UncorrectableBlockFlush (
  IN  VOID     *Address,
  IN  BOOLEAN  ReadBeforeFlush
  )
{
  UINTN  Start;
  UINTN  End;
  UINT8  Buffer[UNCORRECTABLE_UNIT_SIZE];

  ASSERT (mCacheLineSize);
  ASSERT (((UINTN)Address & (UNCORRECTABLE_UNIT_SIZE - 1)) == 0);

  Start = (UINTN)Address;
  End = (Start + UNCORRECTABLE_UNIT_SIZE + (mCacheLineSize - 1)) & ~(mCacheLineSize - 1);
  Start &= ~((UINTN)mCacheLineSize - 1);

  if (ReadBeforeFlush) {
    CopyMem (Buffer, Address, sizeof (Buffer));
  }

  do {
    AsmFlushCacheLineOpt ((VOID*)Start);
    Start += mCacheLineSize;
  } while (Start != End);
#ifdef __GNUC__
  __asm__ __volatile__("sfence");
#else
  _mm_sfence();
#endif
}

/**

  Injects into or clears errors from given SPA range.

  @param[in] StartAddress Start address for clearing the errors.
  @param[in] NumBlocks    Number of blocks to inject or clear.
  @param[in] Enable       TRUE: Inject error, FALSE: Clear error
  @param[in] MemType

  @retval EFI_SUCCESS Errors cleared successfully.
  @return other errors are most likely HW errors.

**/
EFI_STATUS
InjectErrorRange (
  IN  UINT64  StartAddress,
  IN  UINT64  NumBlocks,
  IN  UINT8   Enable,
  IN  UINT8   MemType,
  OUT UINT64  *ProcessedBlocks
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT64      CurrentAddress;
  UINT64      Count;
  UINT64      Dpa;
  UINT8       Skt;
  UINT8       Ch;
  UINT8       Dimm;

  for (CurrentAddress = StartAddress, Count = 0;
       Count < NumBlocks;
       Count++, CurrentAddress += UNCORRECTABLE_UNIT_SIZE) {
    Status = SpaToNvmDpa (CurrentAddress, &Skt, &Ch, &Dimm, &Dpa);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "InjectErrorRange: ERROR: SpaToNvmDpa(0x%llx, ...) failed: %r\n",
        CurrentAddress,
        Status));
      if (ProcessedBlocks) {
        *ProcessedBlocks = Count;
      }
      return Status;
    }

    CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "S%d.C%d.D%d: InjectErrorRange: (0x%04x): %a error %d %a 0x%llx\n",
      Skt,
      Ch,
      Dimm,
      ConvertSocketChannelDimmToNfitHandle (Skt, Ch, Dimm),
      Enable ? "injecting" : "clearing",
      MemType,
      Enable ? "into" : "from",
      Dpa
      ));

    UncorrectableBlockFlush ((VOID *)CurrentAddress, FALSE);

    Status = NvmCtlrInjectPoisonError (Skt, Ch, Dimm, Dpa, MemType, Enable);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("CRP") "S%d.C%d.D%d: InjectErrorRange: (0x%04x): NvmCtlrInjectPoisonError(..., "
                                               "0x%llx, %d, %d) failed: %r\n",
        Skt,
        Ch,
        Dimm,
        ConvertSocketChannelDimmToNfitHandle (Skt, Ch, Dimm),
        Dpa,
        MemType,
        Enable,
        Status
        ));
      if (ProcessedBlocks) {
        *ProcessedBlocks = Count;
      }
      return Status;
    }

    UncorrectableBlockFlush ((VOID *)CurrentAddress, FALSE);

    if (Enable == 0) {
      ArsPatrolScrubberRemove (CurrentAddress);
      ArsRemoveSpaFromInternalData (CurrentAddress);
    }
  }
  if (ProcessedBlocks) {
    *ProcessedBlocks = Count;
  }
  return Status;
}


/**
  Routine Description: Get/Retrieve the Media Status via Boot status register value.

  @param[in] NvmDimmPtr - Pointer to Dimm structure

  @retval MEDIA_READY    - media is ready
  @retval MEDIA_NOTREADY - media not ready
  @retval MEDIA_INERROR  - media is in error state or disabled
  @retval MEDIA_RESERVED - media status is reserved
  @retval MEDIA_UNKNOWN  - can't get media status
**/
UINT8
GetMediaStatus (
  NVDIMM *NvmDimm
  )
{
  EFI_STATUS   Status;
  UINT64       BsrPtr;
  union {
    UINT64     QWord;
    struct {
      BOOT_STATUS_REGISTER      L;
      BOOT_STATUS_HIGH_REGISTER H;
    } Bits;
  } Bsr;

  if (NvmDimm->SADSpaBase == SPA_NOT_MAPPED) {
    return MEDIA_UNKNOWN;
  }
  Status = XlateDpaOffsetToSpa(NvmDimm, NvmDimm->CntrlBase, (UINT64)CR_MB_BOOT_STATUS_OFFSET, &BsrPtr);
  if (!EFI_ERROR(Status)) {

    Bsr.QWord = *((volatile UINT64*)BsrPtr);

    if (Bsr.Bits.L.Bits.MediaDisabled) {

      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("FIS") "S%d.C%d.D%d: Problem with Media Access - Media disabled"
        " (BSR at %p: %llX)\n", NvmDimm->SocketId, NvmDimm->Ch, NvmDimm->Dimm, BsrPtr, Bsr.QWord));
      return MEDIA_INERROR;
    }
    if (Bsr.Bits.L.Bits.MediaReady != MEDIA_READY) {

      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("FIS") "S%d.C%d.D%d: Problem with Media Access"
        " (BSR at %p: %llX)\n", NvmDimm->SocketId, NvmDimm->Ch, NvmDimm->Dimm, BsrPtr, Bsr.QWord));
    }
    return (UINT8)Bsr.Bits.L.Bits.MediaReady;

  } else {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("FIS") "Cannot translate BSR DPA to SPA (%r)\n", Status));
  }
  return MEDIA_UNKNOWN;
}


/**
  Returns PMEM SPA from DPA

  @param[in]  NvmDimm  DIMM to operate on
  @param[in]  Dpa      DPA address
  @param[out] Spa      SPA address

  @retval  EFI_SUCCESS            Lookup successful
  @retval  EFI_INVALID_PARAMETER  NvmDimm is NULL or SPA is NULL
  @retval  EFI_NOT_FOUND          Matchign PMEM rule not found

**/
EFI_STATUS
GetPmemSpaFromDpa (
  IN  NVDIMM *NvmDimm,
  IN  UINT64   Dpa,
  OUT UINT64   *Spa)
{
  UINT16     Index;
  PMEM_INFO  *PmemInfo;
  UINT64     TopDpa = 0;
  INT16      FoundIndex = -1;
  EFI_STATUS Status;

  if (NvmDimm == NULL || Spa == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  for (Index = 0; Index < mCrInfo.NvdimmInfo.NumPMEMRgns; Index++) {
    PmemInfo = &mCrInfo.NvdimmInfo.PMEMInfo[Index];
    if (PmemInfo->Socket != NvmDimm->SocketId || PmemInfo->Channel != NvmDimm->Ch || PmemInfo->Dimm != NvmDimm->Dimm) {
      continue;
    }
    if (PmemInfo->DPABase > TopDpa && Dpa >= PmemInfo->DPABase) {
      TopDpa = PmemInfo->DPABase;
      FoundIndex = Index;
    }
  }
  if (FoundIndex == -1) {
    return EFI_NOT_FOUND;
  }
  PmemInfo = &mCrInfo.NvdimmInfo.PMEMInfo[FoundIndex];
  Status = XlateDpaOffsetToSpa (NvmDimm, PmemInfo->SADPMemBase, Dpa - PmemInfo->DPABase, Spa);
  return Status;
}


/**
  Crystal Ridge Sx entry SMI handler.

  @param[in]     Handle          Handle of the callback
  @param[in]     Context         The dispatch context
  @param[in,out] CommBuffer      A pointer to a collection of data in memory that will
                                 be conveyed from a non-SMM environment into an SMM environment.
  @param[in,out] CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
CrSxHandler (
  IN     EFI_HANDLE                         Handle,
  IN     CONST EFI_SMM_SX_REGISTER_CONTEXT  *Context,
  IN OUT VOID                               *CommBuffer OPTIONAL,
  IN OUT UINTN                              *CommBufferSize OPTIONAL
  )
{
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "CrSxHandler\n"));

  if (NULL == Context) {
    return EFI_DEVICE_ERROR;
  }

  if ((mCrInfo.ExtAdrData.ExtAdrEnabled == EXT_ADR_ENABLED) &&
      (mCrInfo.ExtAdrData.ExtAdrType == EXT_ADR_FAST_ADR)) {
    //
    // When Fast ADR is enabled WPQ Flush is done at the end of Fast ADR flow
    // (check if necessary Fast ADR flows are enabled)
    //
    switch (Context->Type) {
      case SxS3:
        if (mCrInfo.ExtAdrFlows.FlowsBits.S3EntryCpuCacheFlush) {
          return EFI_SUCCESS;
        }
        break;

      case SxS4:
        if (mCrInfo.ExtAdrFlows.FlowsBits.S4EntryCpuCacheFlush) {
          return EFI_SUCCESS;
        }
        break;

      case SxS5:
        if (mCrInfo.ExtAdrFlows.FlowsBits.S5EntryCpuCacheFlush) {
          return EFI_SUCCESS;
        }
        break;

      default:
        //
        // WPQ Flush need to be done within this handler
        //
        break;
    }
  }

  return IssueWpqFlushForAllImc ();

}

/**
  Returns platform PMEM configuration.

  Returns a bit-wise OR of zero or more of the following.

  @retval  WBCACHED_PMEM_PRESENT  AD-WB interleave sets present in the system.
  @retval  PMEM_PRESENT           App-Direct interleave sets present in the system.

**/
UINT8
PlatformPmemConfiguration (
  VOID
  )
{
  UINT8  Index;
  UINT8  Socket;
  UINT8  Retval = 0;

  if (!IsSystemWithDcpmm ()) {
    return Retval;
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Index = 0; Index < MAX_SAD_RULES; Index++) {
      if (mSystemMemoryMap->Socket[Socket].SAD[Index].type == MemType2lmDdrWbCacheAppDirect) {
        Retval |= WBCACHED_PMEM_PRESENT;
      }
      if (mSystemMemoryMap->Socket[Socket].SAD[Index].type == MemType1lmAppDirect) {
        Retval |= PMEM_PRESENT;
      }
    }
  }
  return Retval;
}


/**
  Detects architecture's cache line size and stores in global variable.
**/
STATIC
VOID
DetectCacheLineSize (
  VOID
  )
{
  CPUID_VERSION_INFO_EBX  VersionInfoEbx;
  AsmCpuid (CPUID_VERSION_INFO, NULL, &VersionInfoEbx.Uint32, NULL, NULL);

  mCacheLineSize = VersionInfoEbx.Bits.CacheLineSize * 8;
}

/**
  Routine Description: Setting S3 Passphrase for all CR dimms to lock them. This passphrase is used to unlock dimms during platform S3 Resume.

  @retval VOID

**/
VOID
S3SetPassphraseForAllDimms (
  VOID
  )
{
  EFI_STATUS                    Status;
  UINT8                         Dimm;
  UINT8                         Index;
  UINT8                         OptInDimms = 0;
  UINT8                         S3EnabledDimms = 0;
  NVDIMM                        *DimmPtr;
  FIS_GET_SECURITY_OPT_IN_RSP   FisGetSecurityOptInRsp;
  FIS_GET_SECURITY_OPT_IN_REQ   FisGetSecurityOptInReq;
  FIS_S3_PASSPHRASE_REQ         FisS3PassphraseReq;
  UINT64                        RandPassphrase;

  DEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "S3SetPassphraseForAllDimms\n"));

  for (Dimm = 0; Dimm < mCrInfo.NvdimmInfo.NumNvdimms; Dimm++) {
    DimmPtr = &mCrInfo.NvdimmInfo.Nvdimms[Dimm];

    if (DimmPtr->FisVersion < FIS_2_02 || DimmPtr->FisVersion >= FIS_3_00) {
      DEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "FisVersion: %d. S3 Set passphrase is unsupported.\n", DimmPtr->FisVersion));
      continue;
    }

    // Check if Secure S3 opt-in is enabled
    ZeroMem (&FisGetSecurityOptInReq, sizeof (FisGetSecurityOptInReq));
    FisGetSecurityOptInReq.Bits.OptInCode = S3_RESUME_OPT_IN_CODE;
    Status = FisRequest ((UINT8)DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm, (UINT32 *)&FisGetSecurityOptInReq, sizeof(FisGetSecurityOptInReq), (UINT32 *)&FisGetSecurityOptInRsp, GET_SECURITY_OPT_IN, 0, NULL);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: Unable to get security state to check if secure S3 opt-in is enabled. Status: %d\n", DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm, Status));
      continue;
    }
    if (FisGetSecurityOptInRsp.Bits.OptInCode != S3_RESUME_OPT_IN_CODE) {
      DEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: Invalid Opt-In code: %d\n", DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm, FisGetSecurityOptInRsp.Bits.OptInCode));
      continue;
    }
    if (FisGetSecurityOptInRsp.Bits.OptInValue == PM_REGION_STATE_RESTORED_FROM_S3_ENTRY) {
      // Secure S3 disabled
      continue;
    }
    OptInDimms++;

    // Generate random passphrase
    ZeroMem (&FisS3PassphraseReq, sizeof (FisS3PassphraseReq));

    for (Index = 0; Index < FIS_S3_PASSPHRASE_SIZE; Index += 8) {

      Status = RdRand64 (&RandPassphrase, TRUE);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: Unable to get a valid random passphrase\n", DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm));
        ASSERT_EFI_ERROR (Status);
        continue;
      }
      CopyMem (&DimmPtr->S3Info.Passphrase[Index], &RandPassphrase, (FIS_S3_PASSPHRASE_SIZE - Index) > 8 ? 8 : FIS_S3_PASSPHRASE_SIZE - Index);
    }
    CopyMem (&FisS3PassphraseReq.Bits.NewPassphrase, &DimmPtr->S3Info.Passphrase, sizeof (FisS3PassphraseReq.Bits.NewPassphrase));

    Status = FisRequest ((UINT8)DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm, (UINT32 *)&FisS3PassphraseReq, sizeof(FisS3PassphraseReq), NULL, S3_PASSPHRASE, 0, NULL);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: Setting passphrase for S3 failed. Status %r\n", DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm, Status));
      continue;
    }

    DimmPtr->S3Info.IsSetPassphrase = TRUE;
    S3EnabledDimms++;
  }

  if (OptInDimms > 0 && S3EnabledDimms != mCrInfo.NvdimmInfo.NumNvdimms) {
    DEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "Mixed configuration: for some dimms passphrase is set and for some it is not.\n"));
  }
}

/**
  Routine Description: Wrapper on AsmWbinvd for EFI_AP_PROCEDURE2

  @retval VOID

**/
VOID
EFIAPI
AsmWbinvdProcedureWrapper (
  VOID *NullArgument
  )
{
  AsmWbinvd();
}

/**
  Routine Description: Unlock all dimms during platform S3 Resume

  @retval VOID

**/
VOID
S3UnlockAllDimms (
  VOID
  )
{
  EFI_STATUS                    Status;
  UINT8                         Index;
  NVDIMM                        *DimmPtr;
  FIS_S3_AUTHENTICATE_REQ       FisS3AuthenticateReq;
  BOOLEAN                       IsAnyDimmPassphraseEnabled = FALSE;

  DEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "S3UnlockAllDimms\n"));

  for (Index = 0; Index < mCrInfo.NvdimmInfo.NumNvdimms; Index++) {
    DimmPtr = &mCrInfo.NvdimmInfo.Nvdimms[Index];

    if (!DimmPtr->S3Info.IsSetPassphrase) {
      continue;
    }
    IsAnyDimmPassphraseEnabled = TRUE;

    ZeroMem (&FisS3AuthenticateReq, sizeof (FisS3AuthenticateReq));
    CopyMem (FisS3AuthenticateReq.Bits.S3Passphrase, &DimmPtr->S3Info.Passphrase, sizeof (FisS3AuthenticateReq.Bits.S3Passphrase));
    Status = FisRequest ((UINT8)DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm, (UINT32 *)&FisS3AuthenticateReq, sizeof(FisS3AuthenticateReq), NULL, S3_AUTHENTICATE, 0, NULL);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: Unable to unlock the dimm with passphrase.\n", DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm));
      ASSERT_EFI_ERROR (Status);
      continue;
    }
  }

  if (IsAnyDimmPassphraseEnabled) {

    AsmWbinvdProcedureWrapper(NULL);

    mSmmMp->BroadcastProcedure (
      mSmmMp,
      (EFI_AP_PROCEDURE2)AsmWbinvdProcedureWrapper,
      SMM_MP_DEF_TIMEOUT_US,
      NULL,
      NULL,
      NULL
      );
  }
}

/**
  Copies Length of bytes of DSM input data to buffer allocated in SMM.
  Asserts that memory wasn't overwritten out of bounds of allocated buffer
  by checking the magic that was placed immediately after the buffer
  in additional allocated 4 bytes.

  @param[in,out]  DestBuffer    Pointer to buffer allocated in SMM
  @param[in]      SourceBuffer  Pointer to DSM input buffer that will be copied to SMM
  @param[in]      Length        Size of DSM input buffer in bytes

  @retval VOID

**/

VOID
CopyDsmInputBuffer (
  IN OUT UINT8    *DestBuffer,
  IN CONST VOID   *SourceBuffer,
  IN UINTN        Length
  )
{
  if (Length > NVDIMM_MAX_INPUT_BUF_SIZE){
    Length = NVDIMM_MAX_INPUT_BUF_SIZE;
  }
  CopyMem (DestBuffer, SourceBuffer, Length);
  ASSERT (*(UINT32 *)&DestBuffer[NVDIMM_MAX_INPUT_BUF_SIZE] == DSM_INPUT_BUFFER_MAGIC);
}

/**

  This callback function to install eADR driver protocol once eADR driver is loaded

  @param[in] Protocol   Points to the protocol's unique identifier.
  @param[in] Interface  Points to the interface instance.
  @param[in] Handle     The handle on which the interface was installed.

  @retval EFI_SUCCESS   Notification runs successfully.
**/
EFI_STATUS
EadrProtocolReadyCallBack (
  IN CONST EFI_GUID     *Protocol,
  IN VOID               *Interface,
  IN EFI_HANDLE         Handle
  )
{
  EFI_STATUS Status;

  //
  // eADR driver loaded and protocol should be available
  //
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PMem") "eADR driver initialized - locate eADR protocol\n"));

  Status = mSmst->SmmLocateProtocol (&gEadrProtocolGuid, NULL, (VOID **) &mEadrProtocol);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PMem") "eADR locate protocol failed: %r\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}

/**

Routine Description:CrystalRidgeStart gathers Control Region
Data from the SADs and initializes the internal data structs for
later enablement of the read/write of the NVMCTLR registers thru the
SAD interface. This function needs to be called during init
before the ReadFnvCfgMem & WriteFnvCfgMem can be called.

  @retval UINT32 - status

**/
EFI_STATUS
EFIAPI
CrystalRidgeStart (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS                          Status = EFI_SUCCESS;
  EFI_EVENT                           EndOfDxeEvent;
  EFI_EVENT                           ExitBootServicesEvent;
  EFI_EVENT                           EadrProtocolNotifyEvent;
  BOOLEAN                             InSmm = FALSE;
  SOCKET_PROCESSORCORE_CONFIGURATION  SocketProcessorcoreConfig;
  UINTN                               VariableSize = 0;

  Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, (VOID**)&mSmmBase);
  if (mSmmBase == NULL) {
    InSmm = FALSE;
  } else {
    mSmmBase->InSmm (mSmmBase, &InSmm);
    mSmmBase->GetSmstLocation (mSmmBase, &mSmst);
  }

  mInSmm = InSmm;
  mMemMapHost = GetMemMapHostPointer ();

  if (InSmm) {
    #ifdef SMM
    InitializeSpinLock (&mSmmDebugLock);
    #endif
    mUseOSMailbox = FALSE; // In SMM use SMM (aka BIOS) mailbox
    DetectCacheLineSize ();
    Status = mSmst->SmmLocateProtocol (&gEfiCpuCsrAccessGuid, NULL, (VOID**)&mCpuCsrAccess);
    ASSERT_EFI_ERROR (Status);
    Status = mSmst->SmmLocateProtocol (&gEfiSmmCpuProtocolGuid, NULL, (VOID **) &mSmmCpu);
    ASSERT_EFI_ERROR (Status);
    Status = mSmst->SmmLocateProtocol (&gEfiMmMpProtocolGuid, NULL, (VOID**)&mSmmMp);
    ASSERT_EFI_ERROR (Status);

  } else {

    mUseOSMailbox = TRUE; // In DXE use OS mailbox
    Status = gBS->LocateProtocol (&gEfiCpuCsrAccessGuid, NULL, (VOID**)&mCpuCsrAccess);
    ASSERT_EFI_ERROR (Status);

  }
  VariableSize = sizeof(SOCKET_PROCESSORCORE_CONFIGURATION);
  Status = gRT->GetVariable (
                  SOCKET_PROCESSORCORE_CONFIGURATION_NAME,
                  &gEfiSocketProcessorCoreVarGuid,
                  NULL,
                  &VariableSize,
                  &SocketProcessorcoreConfig
                  );
  if (EFI_ERROR(Status)) {
    CRDEBUG ((DEBUG_ERROR, "[CR] ERROR: Cannot read socket processor core configuration (%r)\n", Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  mSystemMemoryMap = GetSystemMemoryMapHob();

  if (mSystemMemoryMap->DcpmmPresent == 0){
   CRDEBUG ((DEBUG_ERROR, "No PMems, Crystal Ridge Drvier is not going to be loaded.\n"));
    return EFI_SUCCESS;
  }
  ZeroMem (&mCrInfo, sizeof (CR_INFO));

  Status = InitializeNvdimmAcpiSmmInterface();
  ASSERT_EFI_ERROR(Status);
  CRDEBUG ((DEBUG_INFO, "[CR] (InSmm: %d) ACPI<->SMM interface address: 0x%llx\n", InSmm, mNvdimmAcpiSmmInterface));

  Status = gBS->LocateProtocol(&gEfiNvdimmSmbusSmmInterfaceProtocolGuid, NULL, (VOID**)&mNvdimmSmbusSmmInterfaceProtocol);
  ASSERT_EFI_ERROR(Status);
  mNvdimmSmbusSmmInterface = mNvdimmSmbusSmmInterfaceProtocol->Area;


  if (!InSmm) {
    //
    // Allocate buffer for transferring data from Large Payload buffers
    //
    Status = gBS->AllocatePool (EfiBootServicesData, sizeof (UINT8) * CR_LARGE_PAYLOAD_SIZE, (VOID**)&mLargePayloadBuffer);
    ASSERT_EFI_ERROR (Status);

  } else {

    NvdimmAcpiInterfaceInit (mNvdimmAcpiSmmInterface);

    //
    // Allocate buffer for transferring data from Large Payload buffers
    //
    Status = mSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (UINT8) * CR_LARGE_PAYLOAD_SIZE, (VOID**)&mLargePayloadBuffer);
    ASSERT_EFI_ERROR (Status);

    Status = InitArsErrorInject (mSmst);
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Enable WpqFlush for CLX and 10nm based systems
  //
  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || !mSystemMemoryMap->IsWpqFlushSupported) {
    mUseWpqFlush = FALSE;
  } else {
    mUseWpqFlush = TRUE;
  }

  if (mUseWpqFlush) {
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Using WPQ Flush for this configuration\n"));
  } else {
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Using Durable Cache Line Write for this configuration\n"));
  }
  mCrInfo.NvdimmInfo.NumNvdimms = 0;
  //
  // Update the DIMM data table in mCrInfo structs for all NVM DIMMs installed in the system for later use.
  // If Control Region Block Decoder is enabled in the sockets bitmask assume Dlock Decoder
  // is used for NVM DIMM control regions. Otherwise look for SAD of CTRL type (SKX case).
  //
  if (mSystemMemoryMap->BlockDecoderData.BlockSocketEnable != 0) {

    CrInfoBuildDimmInfoForCrbd ();

  } else {

    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "No control region block decoder in this system\n"));
    CrInfoUpdateDimmInfo (MemType1lmCtrl);
  }
  CrInfoUpdateDummyDimmInfo ();

  //
  // Having created the Dimm Structs based on the Control Region, will
  // need SAD Rule base address and Interleave Dimm Num for PMEM and Block regions
  // later for Spa to Dpa translation as these are need by both RAS & DSM.
  //
  CrInfoUpdateDimmInfo (MemType1lmAppDirect);
  //
  //  Get Dimm_info data structures for each and every DISABLED NVDIMM and update
  //  CR_INFO structure
  //
  CrInfoUpdateForDisabledNvdimms ();

  FisInit ();

  if (InSmm) {
    //
    // Create NVMDIMM Present Bitmaps based on ALL NVDIMMs installed in all sockets
    // for use by AcpiSmmInterface.
    //
    CreateDimmPresentBitmaps();

  } else {
    Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID**)&mMpService);
    ASSERT_EFI_ERROR (Status);
  }
  CrUpdateCpuThreadState ();

  if (!InSmm) {

    //
    // Program flush hint address for 14nm CPUs. For 10nm CPUs, the programming is performed in PEI phase.
    //
    if (mUseWpqFlush && (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL)
      || IsCpuAndRevision (CPU_CPX, REV_ALL))) {
      ProgramFlushHintAddressForWpqFlush ();
    }

    //
    // Cache flush hint addresses from mMemMapHost to ensure they are available in SMM.
    //
    CacheFlushHintAddress ();

    //
    // Programs FastGo for all CPU cores
    //
    ProgramCrQosForAllCores ();

    if (SocketProcessorcoreConfig.L2RfoPrefetchDisable) {
      ProgramL2RfoDisableForAllCores();
    }
  }
  //
  // Get some details of these dimms using HOB.
  //
  InitAllDimmsFromHOB (&mCrInfo.NvdimmInfo);

  if (!InSmm) {
    SetMemAttributes ();
    if (mSystemMemoryMap->MemSetup.dfxMemSetup.dfxDimmManagement == DIMM_MGMT_CR_MGMT_DRIVER) {

      UpdatePcd ();
    }
  }
  CrystalRidgeProtocolInstall (ImageHandle, SystemTable);

  if (InSmm) {
    //
    // Perform ARS on boot if enabled
    //
    ArsOnBoot ();

    //
    // Set passphrase for all dimms, if S3 Resume Opt-In is enabled
    //
    S3SetPassphraseForAllDimms();

    SendSecurityNonceToAllDimms (FALSE);

  } else {
    Status = InstallAcpiPcatProtocol (ImageHandle, SystemTable);
    ASSERT_EFI_ERROR (Status);

    Status = InstallNvdimmAcpiConfigProtocol (&ImageHandle);
    ASSERT_EFI_ERROR (Status);

    Status = DcpmmProtocolInstall (ImageHandle, SystemTable);
    ASSERT_EFI_ERROR (Status);

    Status = InstallNfitTableUpdateProtocol (ImageHandle);
    ASSERT_EFI_ERROR (Status);

    Status = LoadAcpiSsdtNvdrTable ();
    ASSERT_EFI_ERROR (Status);

  }

  if (InSmm) {
    EFI_SMM_SW_REGISTER_CONTEXT       SwContext;
    EFI_SMM_SW_DISPATCH2_PROTOCOL     *SwDispatch = 0;
    EFI_HANDLE                        SwHandle;
    EFI_SMM_SX_REGISTER_CONTEXT       SxContext;
    EFI_SMM_SX_DISPATCH2_PROTOCOL     *SxDispatch = 0;
    EFI_HANDLE                        SxHandle;
    EFI_SLEEP_TYPE                    SxType;
#ifdef ICX_HOST
    volatile UINT32 *M2IosfDfxCsr;
    UINT8 Size;
    UINT8 InstId;
    UINT8 Socket;
#endif

    Status = mSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, (VOID**)&SwDispatch);
    ASSERT_EFI_ERROR (Status);

    //
    // Memory allocation for dms input buffer. Additional 4 bytes allocated for magic number
    //
    Status = mSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (UINT8) * NVDIMM_MAX_INPUT_BUF_SIZE + sizeof(UINT32), (VOID**)&mNvdimmAcpiSmmInterfaceInputBuffer);
    if (EFI_ERROR(Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "Memory allocation in SMM failed."));
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
    *(UINT32 *)&mNvdimmAcpiSmmInterfaceInputBuffer[NVDIMM_MAX_INPUT_BUF_SIZE] = DSM_INPUT_BUFFER_MAGIC;

    SwContext.SwSmiInputValue = DSM_SW_SMI;
    Status = SwDispatch->Register (SwDispatch, (EFI_SMM_HANDLER_ENTRY_POINT2)DsmServeNgnCommandsFromOS, &SwContext, &SwHandle);
    ASSERT_EFI_ERROR (Status);

    SwContext.SwSmiInputValue = 0xFB;
    Status = SwDispatch->Register (SwDispatch, (EFI_SMM_HANDLER_ENTRY_POINT2)CrS3Resume, &SwContext, &SwHandle);
    ASSERT_EFI_ERROR (Status);

    NvdimmSmbusAccessSmmInit ();

    //
    // Register the callback for S4/S5 entry
    //
    Status = mSmst->SmmLocateProtocol (&gEfiSmmSxDispatch2ProtocolGuid, NULL, (VOID**)&SxDispatch);
    ASSERT_EFI_ERROR (Status);

    SxContext.Phase = SxEntry;
    for (SxType = SxS4; SxType <= SxS5; SxType++) {
      SxContext.Type = SxType;
      Status = SxDispatch->Register (SxDispatch, CrSxHandler, &SxContext, &SxHandle);
      ASSERT_EFI_ERROR (Status);
    }

    Status = DetermineNearMemoryFlushCapability (&mAdwbFlushTaskCount);
    if (!EFI_ERROR (Status)) {
      if (mAdwbFlushTaskCount == 0) {
        CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PMem") "NM Flush disabled\n"));
        Status = SignalNmFlushStatusToCmosReg (NM_FLUSH_DISABLE);
        if (EFI_ERROR (Status)) {
          CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PMem") "Signal NM Flush disabled failed (status %r)\n", Status));
        }
      }
    } else {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PMem") "Can't determine NM Flush capability (status %r)\n", Status));
    }

    Status = mSmst->SmmRegisterProtocolNotify (&gEfiSmmEndOfDxeProtocolGuid, CrInitAtEndOfDxe, &EndOfDxeEvent);
    ASSERT_EFI_ERROR (Status);
    //
    // Register for SmmExitBootServices and SmmLegacyBoot notification.
    //
    Status = mSmst->SmmRegisterProtocolNotify (&gEdkiiSmmExitBootServicesProtocolGuid, CrExitBootServices, &ExitBootServicesEvent);
    ASSERT_EFI_ERROR (Status);
    Status = mSmst->SmmRegisterProtocolNotify (&gEdkiiSmmLegacyBootProtocolGuid, CrExitBootServices, &ExitBootServicesEvent);
    ASSERT_EFI_ERROR (Status);

    if (mSystemMemoryMap->EadrSupport == EADR_ENABLED) {
      Status = mSmst->SmmLocateProtocol (&gEadrProtocolGuid, NULL, (VOID **) &mEadrProtocol);
      if (EFI_ERROR (Status)) {
        Status = mSmst->SmmRegisterProtocolNotify (&gEadrProtocolGuid, EadrProtocolReadyCallBack, &EadrProtocolNotifyEvent);
      }
    }

    if (FwActivateInitialize ()) {
#ifdef ICX_HOST
      for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
        if (!mSystemMemoryMap->Socket[Socket].SocketEnabled) {
          continue;
        }
        for (InstId = 0; InstId < MAX_INST_ID; InstId++) {
          CRDEBUG((DEBUG_INFO, "DFX%d Socket %d ", InstId, Socket));
          M2IosfDfxCsr = (volatile UINT32*)mCpuCsrAccess->GetCpuCsrAddress(Socket, InstId, IRP_MISC_DFX2_IIO_DFX_VTD_REG, &Size);
          CRDEBUG((DEBUG_INFO, "(%p) pre: 0x%x ", M2IosfDfxCsr, *M2IosfDfxCsr));
          *M2IosfDfxCsr |= (0x1f << 3);
          CRDEBUG((DEBUG_INFO, "post: 0x%x\n", *M2IosfDfxCsr));
        }
      }
#endif
    }
  }

  return EFI_SUCCESS;
} // CrystalRidgeStart()

