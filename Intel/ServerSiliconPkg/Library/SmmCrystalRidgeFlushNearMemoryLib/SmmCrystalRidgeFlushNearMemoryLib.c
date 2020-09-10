/** @file SmmCrystalRidgeFlushNearMemoryLib.c

  Implementation of Near memory cache flush

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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
**/

#include <Library/CrystalRidgeFlushNearMemoryLib.h>
#include <Include/Register/Msr.h>
#include <PiDxe.h>
#include <GpioPinsSklH.h>
#include <PchAccess.h>
#include <BackCompatible.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmCpuService.h>
#include <Protocol/SmmGpiDispatch2.h>
#include <Protocol/SmmIoTrapDispatch2.h>
#include <Protocol/SmmCpuIo2.h>
#include <Protocol/PchSmmIoTrapControl.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CmosAccessLib.h>
#include <Protocol/CpuCsrAccess.h>
#include <Library/SynchronizationLib.h>
#include <Library/OemCrystalRidgeHooks.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Private/Library/GpioPrivateLib.h>
#include <Guid/MemoryMapData.h>
#include <Guid/HobList.h>
#ifdef PCH_CNL
#include <Register/PchRegsGpioCnl.h>
#endif
#include <Include/RegPath.h>
#include REG_PATH(IIO_VTD.h)
#include "Nfit.h"
#include <Library/MemTypeLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/TimerLib.h>
#include <Platform.h>

#include <Library/IoLib.h>
#include <Library/MmPciBaseLib.h>
#include <PchAccess.h>
#include <Library/PchInfoLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PmcLib.h>

#include <Library/PcuMailBoxLib.h>
#include <Cpu/CpuCoreRegs.h>

#define CR_PCH_IO_CF9_TRAP_ADDRESS    0xCF9
#define CR_PCH_IO_CF9_TRAP_LENGTH     0x1
#define CR_PCH_IO_CF9_TRAP_WARM_RESET 0x6
#define CR_PCH_IO_CF9_TRAP_COLD_RESET 0xE

#define IIO_CACHE_POLLING_STATUS_TIMEOUT    50 * 1000 * 1000    // 50 millisecond (nano second units)

#ifndef NELEMENTS
#define NELEMENTS(Array) (sizeof(Array)/sizeof((Array)[0]))
#endif

#define MC_BANK_DATA_VALID_BIT 63

#define MC_BANK_CNT_BIT_START 0
#define MC_BANK_CNT_BIT_END   7

typedef struct {
  UINT32 McCtlMsr;
  UINT32 McStatusMsr;
} MC_BANK_DESC;

#if defined(SKX_HOST) || defined(CPX_HOST)
GLOBAL_REMOVE_IF_UNREFERENCED MC_BANK_DESC gMcBankMsrList [] = {
  {MSR_IA32_MC0_CTL,  MSR_IA32_MC0_STATUS},
  {MSR_IA32_MC1_CTL,  MSR_IA32_MC1_STATUS},
  {MSR_IA32_MC2_CTL,  MSR_IA32_MC2_STATUS},
  {MSR_IA32_MC3_CTL,  MSR_IA32_MC3_STATUS},
  {MSR_IA32_MC4_CTL,  MSR_IA32_MC4_STATUS},
  {MSR_IA32_MC5_CTL,  MSR_IA32_MC5_STATUS},
  {MSR_IA32_MC6_CTL,  MSR_IA32_MC6_STATUS},
  {MSR_IA32_MC7_CTL,  MSR_IA32_MC7_STATUS},
  {MSR_IA32_MC8_CTL,  MSR_IA32_MC8_STATUS},
  {MSR_IA32_MC9_CTL,  MSR_IA32_MC9_STATUS},
  {MSR_IA32_MC10_CTL, MSR_IA32_MC10_STATUS},
  {MSR_IA32_MC11_CTL, MSR_IA32_MC11_STATUS},
  {MSR_IA32_MC12_CTL, MSR_IA32_MC12_STATUS},
  {MSR_IA32_MC13_CTL, MSR_IA32_MC13_STATUS},
  {MSR_IA32_MC14_CTL, MSR_IA32_MC14_STATUS},
  {MSR_IA32_MC15_CTL, MSR_IA32_MC15_STATUS},
  {MSR_IA32_MC16_CTL, MSR_IA32_MC16_STATUS},
  {MSR_IA32_MC17_CTL, MSR_IA32_MC17_STATUS},
  {MSR_IA32_MC18_CTL, MSR_IA32_MC18_STATUS},
  {MSR_IA32_MC19_CTL, MSR_IA32_MC19_STATUS}
};
#else
GLOBAL_REMOVE_IF_UNREFERENCED MC_BANK_DESC gMcBankMsrList [] = {
  {MSR_IA32_MC0_CTL,  MSR_IA32_MC0_STATUS},
  {MSR_IA32_MC1_CTL,  MSR_IA32_MC1_STATUS},
  {MSR_IA32_MC2_CTL,  MSR_IA32_MC2_STATUS},
  {MSR_IA32_MC3_CTL,  MSR_IA32_MC3_STATUS},
  {MSR_IA32_MC4_CTL,  MSR_IA32_MC4_STATUS},
  {MSR_IA32_MC5_CTL,  MSR_IA32_MC5_STATUS},
  {MSR_IA32_MC6_CTL,  MSR_IA32_MC6_STATUS},
  {MSR_IA32_MC7_CTL,  MSR_IA32_MC7_STATUS},
  {MSR_IA32_MC8_CTL,  MSR_IA32_MC8_STATUS},
  {MSR_IA32_MC9_CTL,  MSR_IA32_MC9_STATUS},
  {MSR_IA32_MC10_CTL, MSR_IA32_MC10_STATUS},
  {MSR_IA32_MC11_CTL, MSR_IA32_MC11_STATUS},
  {MSR_IA32_MC12_CTL, MSR_IA32_MC12_STATUS},
  {MSR_IA32_MC13_CTL, MSR_IA32_MC13_STATUS},
  {MSR_IA32_MC14_CTL, MSR_IA32_MC14_STATUS},
  {MSR_IA32_MC15_CTL, MSR_IA32_MC15_STATUS},
  {MSR_IA32_MC16_CTL, MSR_IA32_MC16_STATUS},
  {MSR_IA32_MC17_CTL, MSR_IA32_MC17_STATUS},
  {MSR_IA32_MC18_CTL, MSR_IA32_MC18_STATUS},
  {MSR_IA32_MC19_CTL, MSR_IA32_MC19_STATUS},
  {MSR_IA32_MC20_CTL, MSR_IA32_MC20_STATUS},
  {MSR_IA32_MC21_CTL, MSR_IA32_MC21_STATUS},
  {MSR_IA32_MC22_CTL, MSR_IA32_MC22_STATUS},
  {MSR_IA32_MC23_CTL, MSR_IA32_MC23_STATUS},
  {MSR_IA32_MC24_CTL, MSR_IA32_MC24_STATUS},
  {MSR_IA32_MC25_CTL, MSR_IA32_MC25_STATUS},
  {MSR_IA32_MC26_CTL, MSR_IA32_MC26_STATUS},
  {MSR_IA32_MC27_CTL, MSR_IA32_MC27_STATUS}
};
#endif

EFI_CRYSTALRIDGE_FLUSH_NEAR_MEMORY_PROTOCOL gCrystalRidgeFlushNearMemoryProtocol;
EFI_SMM_CPU_SERVICE_PROTOCOL                *gSmmCpuService;
struct SystemMemoryMapHob                   *gSystemMemoryMap;
CORE_FLUSH_TASK                             *gCoreTasks;
UINT8                                       *gTaskCount;
SPIN_LOCK                                   gApLock;
volatile UINTN                              gCoreCompletes;
volatile UINTN                              gCoreRunning;
UINT8                                       *gInCRFlushNMProtocol;
UINT32                                      *gErrCount;

/**

  Routine Description: Does this system have at least one NVMDIMM

  @return TRUE if NVMDIMM present

**/
BOOLEAN
IsDcpmmPresent (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "System Memory Map PMem Present = %d\n", gSystemMemoryMap->DcpmmPresent));
  if (gSystemMemoryMap->DcpmmPresent != 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**

  Routine Description: Find the amount of near memory cache on a socket

  @param[in] Sckt

  @return UINT32 NmSize in 64M units

**/
UINT32
FindNMSize (
  UINT8     Sckt
  )
{
  UINT8              Ch;
  UINT16             MinNmSize = 0xFFFF;
  UINT32             NmSize = 0;
  UINT8              DimmsFound = 0;
  const UINT8        Slot0 = 0; //DDR4 can only be in slot 0
  struct DimmDevice  *Dimm;

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    Dimm = &gSystemMemoryMap->Socket[Sckt].ChannelInfo[Ch].DimmInfo[Slot0];
    if (Dimm->Present != 0 &&
        Dimm->DcpmmPresent == 0) {
      DimmsFound++;
      if (Dimm->DimmSize < MinNmSize) {
        MinNmSize = Dimm->DimmSize;
      }
    }
  }

  if (DimmsFound != 0) {
    NmSize = MinNmSize * DimmsFound;
    DEBUG ((DEBUG_INFO, "%a() %d DIMMS of %x cap = %x total cap\n",__FUNCTION__, DimmsFound, MinNmSize, NmSize));
  }
  return NmSize;
}

/**

  Create entries for each of the DDRT DIMMs in the address range

  @param DispatchHandle - not used
  @param DispatchContext - not used
  @param CommBuffer - not used
  @param CommBufferSize - not used

  @return EFI status returned by DSM methods or
          EFI_INVALID_PARAMETER if DSM command function index is out of bounds.

**/
EFI_STATUS
BuildCoreFlushTasks (
  IN OUT   UINT8   *TaskCount,
  IN OUT   UINT8   Socket,
  IN OUT   UINT8   DdrtCount,
  IN OUT   UINT8   Interleave,
  IN OUT   UINT64  Starting,
  IN OUT   UINT64  Ending
  )
{
  UINT8   Dimm;
  UINT8   Task = *TaskCount;
  UINT16  Interleaving;

  ASSERT(Starting < Ending);

  Interleaving = (UINT16)(4 * 1024);

  for (Dimm = 0; Dimm < DdrtCount; Dimm++) {
    if (Task == MAX_CORE_TASKS) {
      DEBUG ((DEBUG_INFO, "%a() ran out of core tasks - Task:%d on Range:%x..%x\n", __FUNCTION__, Task, Starting, Ending));
      DEBUG ((DEBUG_INFO, "Near Memory cache flush Core Tasks\n Socket  Starting         .. Ending        Interleave\n")); {
        UINT8 i;
        for (i = 0; i < Task; i++) {
          DEBUG ((DEBUG_INFO, "%2d- S%d %16lx .. %16lx       %d\n", i, gCoreTasks[i].Socket, gCoreTasks[i].StartingAddress, gCoreTasks[i].EndingAddress, gCoreTasks[i].Interleave));
        }
      }
      *TaskCount = Task;
      return EFI_OUT_OF_RESOURCES;
    }
    gCoreTasks[Task].Socket = Socket;
    gCoreTasks[Task].Interleave = Interleaving;
    gCoreTasks[Task].DdrtCount = DdrtCount;
    gCoreTasks[Task].EndingAddress = (Ending << BITS_64MB_TO_BYTES);
    gCoreTasks[Task].StartingAddress = (Starting << BITS_64MB_TO_BYTES);
    gCoreTasks[Task].DimmPos = Dimm;
    Task++;
  }

  *TaskCount = Task;
  return EFI_SUCCESS;
}

/**

  Routine description:

  @return EFI status returned by DSM methods or
          EFI_INVALID_PARAMETER if DSM command function index is out of bounds.

**/
EFI_STATUS
DetermineNearMemoryFlushAddresses (
  IN OUT   UINT8   *TaskCount
  )
{
  UINT8     LocalTaskCount = 0;
  UINT8     socket;
  UINT8     i;

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    SAD_TABLE *Sad;
    UINT64    PrevSadLimit = 0;
    UINT64    StartAddr = 0;
    UINT64    EndAddr = 0;
    UINT64    NmCapFlushed = 0;
    UINT32    NmSize = 0;
    UINT8     NumNvdimms = 0;
    UINT8     Interleave = 0;

    NmSize = FindNMSize (socket);
    if (NmSize == 0) {
      continue;
    }
    DEBUG ((DEBUG_INFO, "S%d NmCache  %x \n", socket, NmSize));

    for (i = 0; i < MAX_SAD_RULES; i++) {
      NumNvdimms = 0;
      Sad = &gSystemMemoryMap->Socket[socket].SAD[i];
      if (Sad->Enable == 0) {
        continue;
      }

      if ((Sad->local == 0) ||
        !IsMemType2lm(Sad->type) ||
        (Sad->Limit <= gSystemMemoryMap->highMemBase)) {
        //
        // Ignore non-local SADs, Types that are not 2LM or WB_PMEME and SADs for low memory (<4G)
        //
        PrevSadLimit = Sad->Limit;
        continue;

      }

      if (NmCapFlushed >= NmSize) {
        //
        // All Near memory cache has been flushed so skip remaining regions
        //
        continue;
      }

      // Assuming that 2LM is followed by Pmem and the system addresses map to contiguous DIMM addresses
      StartAddr = PrevSadLimit;
      EndAddr = Sad->Limit;

      if (Sad->type == MemType2lmDdrWbCacheAppDirect) {
        DEBUG ((DEBUG_INFO, "S%d Pmem  %x..%x Size:%x\n", socket, StartAddr, EndAddr,(EndAddr - StartAddr)));
        //
        // Only the near memory that has not been flushed using 2LM needs to be read
        //
        EndAddr = StartAddr + NmSize - NmCapFlushed;
        ASSERT (EndAddr < Sad->Limit);
      } else {
        if (NmSize < (NmCapFlushed + (EndAddr - StartAddr))) {
          //
          // 2LM is larger than near memory so reduce EndAddr to just cover NM capacity
          //
          EndAddr = StartAddr + (NmSize - NmCapFlushed);
        }
        NmCapFlushed += EndAddr - StartAddr;
        DEBUG ((DEBUG_INFO, "S%d 2LM   %x..%x{%x} Size:%x Tsize:%x\n", socket, StartAddr, EndAddr, Sad->Limit, (EndAddr - StartAddr), NmCapFlushed));
      }

      // track the number of DCPMMs in the regions
      if (Sad->FMchannelInterBitmap[IMC0]) {
        if (Sad->FMchannelInterBitmap[IMC0] == 7) {
          NumNvdimms = 3;
        } else if (Sad->FMchannelInterBitmap[IMC0] == 3) {
          NumNvdimms = 2;
        } else {
          NumNvdimms = 1;
        }
      }
      if (Sad->FMchannelInterBitmap[IMC1]) {
        if (Sad->FMchannelInterBitmap[IMC1] == 7) {
          NumNvdimms += 3;
        } else if (Sad->FMchannelInterBitmap[IMC1] == 3) {
          NumNvdimms += 2;
        } else {
          NumNvdimms += 1;
        }
      }
      ASSERT (NumNvdimms > 0);

      Interleave = Sad->granularity;
      if (EFI_SUCCESS !=
          BuildCoreFlushTasks (
            &LocalTaskCount,
            socket,
            NumNvdimms,
            Interleave,
            StartAddr,
            EndAddr)) {
        DEBUG ((DEBUG_ERROR, "Unable to build all core flush ranges for Range:%d %x..%x\n", LocalTaskCount, StartAddr, EndAddr));
        *TaskCount = 0;
        return EFI_OUT_OF_RESOURCES;
      }

      if (Sad->type == MemType2lmDdrWbCacheAppDirect) {
        //
        // Pmem regions must be read twice to assure data is flushed.
        // The second read needs to be 1 NmSize away from the first
        //
        StartAddr += NmSize;
        EndAddr += NmSize;

        if ((EndAddr > Sad->Limit) ||
          (EFI_SUCCESS !=
           BuildCoreFlushTasks (
             &LocalTaskCount,
             socket,
             NumNvdimms,
             Interleave,
             StartAddr,
             EndAddr))) {
          DEBUG ((DEBUG_ERROR, "Unable to build all core flush ranges for Range:%d %x..%x\n", LocalTaskCount, StartAddr, EndAddr));
          *TaskCount = 0;
          return EFI_OUT_OF_RESOURCES;
        }

        //
        // Two Pmem ranges have been defined so no more ranges are needed
        //
        break; //  i < MAX_SAD_RULES
      }

      PrevSadLimit = Sad->Limit;
    } // i < MAX_SAD_RULES
  } // socket < MAX_SOCKET

  DEBUG ((DEBUG_INFO, "Near Memory cache flush Core Tasks\n Socket  Starting         .. Ending          DIMM  Interleave\n"));
  for (i = 0; i < LocalTaskCount; i++) {
    DEBUG ((DEBUG_INFO, "%2d- S%d %16lx .. %16lx   %d    %d\n", i, gCoreTasks[i].Socket, gCoreTasks[i].StartingAddress, gCoreTasks[i].EndingAddress, gCoreTasks[i].DimmPos, gCoreTasks[i].Interleave));
  }

  *TaskCount = LocalTaskCount;
  return EFI_SUCCESS;
}

/**

  Routine description:

  @return VOID

**/
BOOLEAN
NearMemoryCacheFlushNeeded (
  VOID
  )
{
  UINT8  i;
  UINT8  socket;

  if (!IsDcpmmPresent ()) {
    DEBUG ((DEBUG_INFO, "%a() - Is PMem Module Present - FALSE>\n", __FUNCTION__));
    return FALSE;
  }

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    for (i = 1; i < MAX_SAD_RULES; i++) {
      if (gSystemMemoryMap->Socket[socket].SAD[i].type == MemType2lmDdrWbCacheAppDirect) {
        DEBUG ((DEBUG_INFO, "%a() - TRUE>\n", __FUNCTION__));
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**

  Routine description: This routine causes the system to sequence into sleep state defined

  @return VOID

**/
VOID
NmFlushGpiSystemToS5State(
  VOID
  )
{
  UINT16  AcpiBaseAddress;
  UINT32  Data32;

  DEBUG ((DEBUG_ERROR, "%a()<\n", __FUNCTION__));

  AcpiBaseAddress = PmcGetAcpiBase();

  //Double check that we correctly read in the acpi base address
  ASSERT ((AcpiBaseAddress != 0x0) && ((AcpiBaseAddress & 0x1) != 0x1));

  // Disable SMI trap on SLP_EN
  IoWrite32 ((AcpiBaseAddress + R_ACPI_SMI_EN), (IoRead32 (AcpiBaseAddress + R_ACPI_SMI_EN) & ~BIT4));
  IoWrite32 ((AcpiBaseAddress + R_ACPI_SMI_STS), PCH_SMI_STS_DISABLE);
  IoWrite32 ((AcpiBaseAddress + R_ACPI_SMI_STS), PCH_SMI_STS_DISABLE);

  // Delay
  MicroSecondDelay (100);

  // Go to S5 state
  // set Sleep type first in Power management control register
  // PM1_CNT register bit12:bit10 = 111 maps to S5 state
  Data32 = IoRead32 ((AcpiBaseAddress + R_ACPI_PM1_CNT));
  Data32 |=  (V_ACPI_IO_PM1_CNT_S5);
  IoWrite32 ((AcpiBaseAddress + R_ACPI_PM1_CNT), Data32);

  // set Sleep Enable in Power management control register
  // PM1_CNT register bit13 = 1
  Data32 |= (B_ACPI_IO_PM1_CNT_SLP_EN);
  IoWrite32 ((AcpiBaseAddress + R_ACPI_PM1_CNT), Data32);

  DEBUG ((DEBUG_ERROR, "%a()> Wait here \n", __FUNCTION__));
  // wait here
  CpuDeadLoop ();

}

/**

  Routine description: Flush all near memory cache lines containing AppDirect data

  @return VOID

**/
EFI_STATUS
NearMemoryFlushGpiChildCallback (
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID  *GpiContext,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{
  EFI_SMM_GPI_REGISTER_CONTEXT  *Context;
  UINT32                        GpioSmiStsValue;

  DEBUG ((DEBUG_ERROR, "%a()<\n", __FUNCTION__));
  Context = (EFI_SMM_GPI_REGISTER_CONTEXT *) GpiContext;

  GpioGetReg (FLUSH_NEAR_MEMORY_CACHE_GPP_GROUP, FLUSH_NEAR_MEMORY_CACHE_GPP_SMI_STS, &GpioSmiStsValue);
  DEBUG ((DEBUG_INFO, "GpioSmiStsValue = 0x%X\n", GpioSmiStsValue));
  if (GpioSmiStsValue & (1 << GpioGetPadNumberFromGpioPad (FLUSH_NEAR_MEMORY_CACHE_GPP_PAD))) {
    FlushNearMemoryCache ();
    GpioClearGpiSmiSts (FLUSH_NEAR_MEMORY_CACHE_GPP_PAD);

    // System to sequence into S5 sleep stae
    NmFlushGpiSystemToS5State ();
  }
  DEBUG ((DEBUG_ERROR, "%a()>\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**

  Routine description: Flush all near memory cache lines containing AppDirect data (initiated by OS reset)

  @return VOID

**/
EFI_STATUS
NearMemoryFlushCF9TrapCallback (
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID  *Context         OPTIONAL,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
  {
  EFI_STATUS                          Status;
  UINT8                               Data;
  PCH_SMM_IO_TRAP_CONTROL_PROTOCOL    *PchIoTrapCtrl;
  EFI_SMM_IO_TRAP_CONTEXT             *PchIoTrapContext = (EFI_SMM_IO_TRAP_CONTEXT *)Context;

  //
  // We need to have the context to get the write data
  //
  if (NULL == Context) {
    ASSERT (FALSE);
    return EFI_ABORTED;
  }

  //
  // Retrieve the reset type
  //
  Data = (UINT8)(PchIoTrapContext->WriteData >> 8);

  //
  // Check the reset type
  //
  if ((Data == CR_PCH_IO_CF9_TRAP_COLD_RESET) || (Data == CR_PCH_IO_CF9_TRAP_WARM_RESET)) {

    DEBUG ((DEBUG_INFO, "NM Flush Reset callback entry (reset type: %a)\n", (Data == CR_PCH_IO_CF9_TRAP_COLD_RESET) ? "Cold Reset" : "Warm Reset"));

    //
    // Execute NM Flush if configured
    //
    DEBUG ((DEBUG_INFO, "Flush Near Memory Cache\n"));
    FlushNearMemoryCache();

    //
    // Locate the PCH SMM IO Trap control protocol
    //
    Status = gSmst->SmmLocateProtocol (&gPchSmmIoTrapControlGuid, NULL, &PchIoTrapCtrl);
    if (EFI_ERROR (Status)) {
      ASSERT (FALSE);
      return EFI_ABORTED;
    }

    //
    // Pause 0xCF9 PCH Trap Dispatch
    //
    Status = PchIoTrapCtrl->Pause (PchIoTrapCtrl, DispatchHandle);
    if (EFI_ERROR (Status)) {
      ASSERT (FALSE);
      return EFI_ABORTED;
    }

    DEBUG ((DEBUG_INFO,"Resetting the platform (%02x)...\n", Data));

    Status = gSmst->SmmIo.Io.Write (&gSmst->SmmIo, SMM_IO_UINT8, CR_PCH_IO_CF9_TRAP_ADDRESS, 1, &Data);
    if (EFI_ERROR (Status)) {
      ASSERT (FALSE);
      return EFI_ABORTED;
    }
  }

  return EFI_SUCCESS;
}

/**

  This routine Registers NM GPI callbacks.

  @param[in,out] GpiDispatch     Pointer to parent dispatch service to Gpi trap smi source.
  @param[in,out] GpiContext      Pointer GpiContext handler.

  @retval VOID

**/
VOID
RegisterGpiDispatch(
  IN OUT EFI_SMM_GPI_DISPATCH2_PROTOCOL  *GpiDispatch,
  IN OUT EFI_SMM_GPI_REGISTER_CONTEXT    *GpiContext
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  GpiHandle;

  DEBUG((DEBUG_INFO, "Initialize OemInitFlushNearMemoryCacheGPI\n"));
  if (OemInitFlushNearMemoryCacheGPI() == TRUE) {
    DEBUG((DEBUG_INFO, "OemInitFlushNearMemoryCacheGPI TRUE\n"));
    //
    // Register SMI handler through a child protocol
    //
    DEBUG((DEBUG_INFO, "Locate gEfiSmmGpiDispatch2ProtocolGuid\n"));
    Status = gSmst->SmmLocateProtocol(&gEfiSmmGpiDispatch2ProtocolGuid, NULL, &GpiDispatch);
    ASSERT_EFI_ERROR (Status);

    GpioGetGpiSmiNum(FLUSH_NEAR_MEMORY_CACHE_GPP_PAD, &(GpiContext->GpiNum));

    DEBUG((DEBUG_INFO, "Register GPI Dispatch\n"));
    Status = GpiDispatch->Register (
                            GpiDispatch,
                            NearMemoryFlushGpiChildCallback,
                            GpiContext,
                            &GpiHandle
                            );
    ASSERT_EFI_ERROR (Status);
  }else {
    DEBUG((DEBUG_ERROR, "OemInitFlushNearMemoryCacheGPI FALSE\n"));
  }
}

/**

  This routine Registers PCH IO callbacks.

  @param[in,out] PchIoTrap            Pointer to parent dispatch service to IO trap smi source.
  @param[in,out] PchIoTrapHandle      Pointer IO trap handler.
  @param[in,out] PchIoTrapContext     Pointer IO PchIoTrapContext handler.

  @retval VOID

**/
VOID
RegisterPchIoDispatch(
  IN OUT EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL *PchIoTrap,
  IN OUT EFI_HANDLE                         *PchIoTrapHandle,
  IN OUT EFI_SMM_IO_TRAP_REGISTER_CONTEXT   *PchIoTrapContext
)
{
  EFI_STATUS  Status;

  DEBUG((DEBUG_INFO, "Locate gEfiSmmIoTrapDispatch2ProtocolGuid\n"));
  Status = gSmst->SmmLocateProtocol (&gEfiSmmIoTrapDispatch2ProtocolGuid, NULL, &PchIoTrap);
  ASSERT_EFI_ERROR (Status);

  //
  // Register 0xCF9 PCH Trap
  //
  DEBUG((DEBUG_INFO, "Register 0xCF9 PCH Trap Dispatch\n"));
  PchIoTrapHandle            = NULL;
  PchIoTrapContext->Type     = WriteTrap;
  PchIoTrapContext->Length   = CR_PCH_IO_CF9_TRAP_LENGTH;
  PchIoTrapContext->Address  = CR_PCH_IO_CF9_TRAP_ADDRESS;
  Status = PchIoTrap->Register (
                       PchIoTrap,
                       (EFI_SMM_HANDLER_ENTRY_POINT2) NearMemoryFlushCF9TrapCallback,
                       PchIoTrapContext,
                       PchIoTrapHandle
                       );
  ASSERT_EFI_ERROR (Status);
}

/**
  Routine description: callback routine from NM flush cache Err handler to save Error Record

  @return VOID
**/
VOID
CrNmFlushErrInfo(
  VOID
  )
{
     // Save Error Records OEM to Add this Functationality if htey want
}

/**

  Routine description: This routine installs near memory flush protocol and registers dispatch routines

  @param[in out] AdwbFlushTaskCount  Count the number of NM flush task needed for AppDirect-WB.
  @param[in] FullInit  Set to FALSE to limit init for eADR

  @return VOID

**/
STATIC VOID
InitializeNearMemoryFlush (
  IN OUT UINTN *AdwbFlushTaskCount,
  IN BOOLEAN FullInit
  )
{
   EFI_STATUS                          Status;
   EFI_HANDLE                          Handle = NULL;
   EFI_SMM_GPI_DISPATCH2_PROTOCOL      *GpiDispatch = NULL;
   EFI_SMM_GPI_REGISTER_CONTEXT        GpiContext;
   EFI_HANDLE                          PchIoTrapHandle;
   EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL  *PchIoTrap = NULL;
   EFI_SMM_IO_TRAP_REGISTER_CONTEXT    PchIoTrapContext;

   gCrystalRidgeFlushNearMemoryProtocol.SystemMemoryMap = gSystemMemoryMap;
   gCrystalRidgeFlushNearMemoryProtocol.CoreTasksCount  = gTaskCount;

   if (FullInit) {
      gCrystalRidgeFlushNearMemoryProtocol.CoreTasks = gCoreTasks;
      gCrystalRidgeFlushNearMemoryProtocol.CrNmFlushErrInfo = CrNmFlushErrInfo;
      gCrystalRidgeFlushNearMemoryProtocol.InCrystalridgeFlushNMProtocol = gInCRFlushNMProtocol;
      gCrystalRidgeFlushNearMemoryProtocol.ErrCnt = gErrCount;
   } else {
      gSmst->SmmFreePool (gCoreTasks);
      gCrystalRidgeFlushNearMemoryProtocol.CrNmFlushErrInfo = NULL;
      gSmst->SmmFreePool (gInCRFlushNMProtocol);
      gSmst->SmmFreePool (gErrCount);
   }

   Status = gSmst->SmmInstallProtocolInterface (
                     &Handle,
                     &gEfiCrystalRidgeFlushNearMemoryProtocolGuid,
                     EFI_NATIVE_INTERFACE,
                     &gCrystalRidgeFlushNearMemoryProtocol
                     );
   ASSERT_EFI_ERROR (Status);

   //
   // Register GPI Dispatch
   //
   RegisterGpiDispatch(GpiDispatch, &GpiContext);

   //
   // Register PCH IO Dispatch
   //
   RegisterPchIoDispatch(PchIoTrap, &PchIoTrapHandle, &PchIoTrapContext);

   *AdwbFlushTaskCount = *gTaskCount;
}

/**

  Routine description: This routine determines if NM Flush is needed upon S0>S5 transition or Warm

  @param[out] AdwbFlushTaskCount  Count the number of NM flush task needed for AppDirect-WB.

  @return EFI status is returned

**/
EFI_STATUS
DetermineNearMemoryFlushCapability (
  OUT UINTN *AdwbFlushTaskCount
  )
{
  EFI_STATUS                          Status;
  struct SystemMemoryMapHob           *TempSystemMemoryMap;

  DEBUG ((DEBUG_INFO, "%a()<\n", __FUNCTION__));
  if (gSmst != NULL) {
    //
    // Search for the Memory Map GUID HOB.  If it is not present, then
    // there's nothing we can do. It may not exist on the update path.
    //
    DEBUG ((DEBUG_INFO, "Search for the Memory Map GUID HOB\n"));
    TempSystemMemoryMap = GetSystemMemoryMapData ();
    ASSERT (TempSystemMemoryMap != NULL);
    if (TempSystemMemoryMap != NULL) {
      //
      // Allocate memory for SystemMemoryHob
      //
      Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (struct SystemMemoryMapHob), (VOID **) &gSystemMemoryMap);
      ASSERT_EFI_ERROR (Status);
      CopyMem (gSystemMemoryMap, TempSystemMemoryMap, sizeof (struct SystemMemoryMapHob));

      Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, ((sizeof (CORE_FLUSH_TASK)) * MAX_CORE_TASKS), (VOID **)&gCoreTasks);
      ASSERT_EFI_ERROR (Status);
      ZeroMem (gCoreTasks, ((sizeof (CORE_FLUSH_TASK)) * MAX_CORE_TASKS));

      Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (UINT8), (VOID **) &gTaskCount);
      ASSERT_EFI_ERROR (Status);
      ZeroMem (gTaskCount, sizeof (UINT8));

      Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof(UINT8), (VOID **) &gInCRFlushNMProtocol);
      ASSERT_EFI_ERROR(Status);
      ZeroMem(gInCRFlushNMProtocol, sizeof(UINT8));

      Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof(UINT32), (VOID **) &gErrCount);
      ASSERT_EFI_ERROR(Status);
      ZeroMem(gErrCount, sizeof(UINT32));

      if (NearMemoryCacheFlushNeeded ()) {
        Status = DetermineNearMemoryFlushAddresses (gTaskCount);
        if (Status != EFI_SUCCESS) {
          DEBUG ((DEBUG_ERROR, "DetermineNearMemoryFlushAddresses not EFI_SUCCESS\n"));
          gSmst->SmmFreePool (gSystemMemoryMap);
          gSmst->SmmFreePool (gCoreTasks);
          gSmst->SmmFreePool (gTaskCount);
          gSmst->SmmFreePool (gInCRFlushNMProtocol);
          gSmst->SmmFreePool (gErrCount);
        } else {
          InitializeNearMemoryFlush(AdwbFlushTaskCount, TRUE);
        }
      } else {
        DEBUG ((DEBUG_INFO, "NearMemoryCacheFlushNeeded FALSE\n"));
        gSmst->SmmFreePool (gSystemMemoryMap);
        gSmst->SmmFreePool (gCoreTasks);
        gSmst->SmmFreePool (gTaskCount);
        gSmst->SmmFreePool (gInCRFlushNMProtocol);
        gSmst->SmmFreePool (gErrCount);
        *AdwbFlushTaskCount = 0;
      }
    }
  }

  DEBUG ((DEBUG_INFO, ">%a()\n", __FUNCTION__));
  return EFI_SUCCESS;
}

#if defined(SKX_HOST) || defined(CPX_HOST)
/**

  Changes the performance state

  @param[in/out] Buffer - The pointer to private data buffer.

  @retval None

**/
VOID
ChangeCpuPerformanceState (
  IN OUT VOID  *Buffer
  )
{
  MSR_IA32_PERF_CTL_REGISTER          PerformanceControlMsr;
  SPR_MSR_PLATFORM_INFO_REGISTER      PlatformInfoMsr;
  MSR_IA32_HWP_CAPABILITIES_REGISTER  HwpCapabilitiesMsr;
  MSR_IA32_HWP_REQUEST_REGISTER       HwpHwReqMsr;
  MSR_IA32_PM_ENABLE_REGISTER         HwpEnableMsr;

  HwpEnableMsr.Uint64 = AsmReadMsr64 (MSR_IA32_PM_ENABLE);
  if (HwpEnableMsr.Bits.HWP_ENABLE) {
    HwpCapabilitiesMsr.Uint64 = AsmReadMsr64 (MSR_IA32_HWP_CAPABILITIES);
    HwpHwReqMsr.Uint64 = AsmReadMsr64 (MSR_IA32_HWP_REQUEST);

    HwpHwReqMsr.Bits.Minimum_Performance = HwpCapabilitiesMsr.Bits.Lowest_Performance;
    HwpHwReqMsr.Bits.Maximum_Performance = (HwpCapabilitiesMsr.Bits.Lowest_Performance + 1);

    AsmWriteMsr64 (MSR_IA32_HWP_REQUEST, HwpHwReqMsr.Uint64);

  } else {
    PerformanceControlMsr.Uint64 = AsmReadMsr64 (MSR_IA32_PERF_CTL);
    PlatformInfoMsr.Uint64 = AsmReadMsr64 (SPR_MSR_PLATFORM_INFO);

    PerformanceControlMsr.Bits.TargetState &= ~((UINT32)B_IA32_PERF_CTRLP_STATE_TARGET);
    PerformanceControlMsr.Bits.TargetState |= ((PlatformInfoMsr.Bits.MaxEfficiencyRatio) & ((UINT32)B_IA32_PERF_CTRLP_STATE_TARGET));

    AsmWriteMsr64 (MSR_IA32_PERF_CTL, PerformanceControlMsr.Uint64);
  }
}

/**
  Executes IIO Flush

  @param None

  @retval EFI_SUCCESS  - Operation completed successfully,
                         error code otherwise

**/
VOID
FlushIioCache (
  VOID
  )
{
  EFI_STATUS                  Status;
  UINT8                       Stack;
  UINT8                       Socket;
  UINT64                      Timeout;
  CIPSTS_IIO_VTD_STRUCT       CipStsIioVtd;
  CIPCTRL_IIO_VTD_STRUCT      CipCtrlIioVtd;
  EFI_CPU_CSR_ACCESS_PROTOCOL *CpuCsrAccess;

  DEBUG((DEBUG_ERROR, "%a()<\n", __FUNCTION__));

  Status = gSmst->SmmLocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &CpuCsrAccess);
  if (Status) {
    DEBUG ((DEBUG_ERROR, "CpuCsrAccess is not found!\n"));
    return;
  }

  //
  // Execute IIO flush on all enabled sockets
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Skip disabled sockets
    //
    if (!gSystemMemoryMap->Socket[Socket].SocketEnabled) {
      continue;
    }
    //
    // Initiate IIO Flush for the socket
    //
    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      //
      // Check if IIO Stack enabled
      //
      if (gSystemMemoryMap->Socket[Socket].IioStackBitmap & (BIT0 << Stack)) {
        //
        // Initiate IIO Flush
        //
        DEBUG ((DEBUG_INFO, "Socket (%d) Stack (%d) IIO flush - initiate flush\n", Socket, Stack));
        CipCtrlIioVtd.Data = CpuCsrAccess->ReadCpuCsr (Socket, Stack, CIPCTRL_IIO_VTD_REG);
        CipCtrlIioVtd.Bits.flushpendwr = 1;
        CpuCsrAccess->WriteCpuCsr (Socket, Stack, CIPCTRL_IIO_VTD_REG, CipCtrlIioVtd.Data);
      }
    }
  }

  //
  // Check IIO flush completion for enabled sockets
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Skip disabled sockets
    //
    if (!gSystemMemoryMap->Socket[Socket].SocketEnabled) {
      continue;
    }
    //
    // Check IIO Flush state for the socket
    //
    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      //
      // Check if IIO Stack enabled
      //
      if (gSystemMemoryMap->Socket[Socket].IioStackBitmap & (BIT0 << Stack)) {
        //
        // Check IIO Flush completion state
        //
        DEBUG ((DEBUG_INFO, "Socket (%d) Stack (%d) IIO flush - wait for completion\n", Socket, Stack));
        //
        // Initialize timeout value
        //
        Timeout = (GetTimeInNanoSecond(GetPerformanceCounter ()) + IIO_CACHE_POLLING_STATUS_TIMEOUT);
        //
        // Poll status register for IIO flush completion
        //
        CipStsIioVtd.Data = CpuCsrAccess->ReadCpuCsr (Socket, Stack, CIPSTS_IIO_VTD_REG);
        while (!CipStsIioVtd.Bits.flush_pending_writes) {
          CipStsIioVtd.Data = CpuCsrAccess->ReadCpuCsr (Socket, Stack, CIPSTS_IIO_VTD_REG);
          if (GetTimeInNanoSecond(GetPerformanceCounter ()) >= Timeout) {
            break;
          }
        }
        //
        // Check IIO Flush completion status
        //
        if (!CipStsIioVtd.Bits.flush_pending_writes) {
          DEBUG ((DEBUG_INFO, "Socket (%d) Stack (%d) IIO flush failed (timeout)\n", Socket, Stack));
        } else {
          DEBUG ((DEBUG_INFO, "Socket (%d) Stack (%d) IIO flush completed\n", Socket, Stack));
        }
      }
    }
  }
  DEBUG ((DEBUG_ERROR, ">%a()\n", __FUNCTION__));
}
#else
/**

  Routine description: This routine retrieves the CPU Platform Info MSR

  @param[out] PlatformInfoMsr - Platform Info MSR data

  @return None

**/
VOID
GetCpuPlatformInofMsr (
  OUT UINT64  *PlatformInfoMsr
  )
{
  if (NULL == PlatformInfoMsr) {
    ASSERT (FALSE);
    return;
  }
  *PlatformInfoMsr = AsmReadMsr64 (ICX_MSR_PLATFORM_INFO);
}

/**

  Routine description: This routine retrieves the AP CPU Platform Info MSR

  @param[out] PlatformInfoMsr - Platform Info MSR data

  @return None

**/
VOID
GetCpuPlatformInofMsrAp (
  OUT UINT64  *PlatformInfoMsr
  )
{
  if (NULL == PlatformInfoMsr) {
    ASSERT (FALSE);
    return;
  }
  GetCpuPlatformInofMsr (PlatformInfoMsr);
  ReleaseSpinLock (&gApLock);
}

/**

  Routine description: This routine retrieves the Socket Platform Info MSR

  @param[in]  Socket          - Socket Id
  @param[out] PlatformInfoMsr - Platform Info MSR data

  @retval EFI_SUCCESS         - Operation completed successfully,
                                error code otherwise

**/
EFI_STATUS
GetSocketPlatformInfoMsr (
  IN UINT8    Socket,
  OUT UINT64  *PlatformInfoMsr
  )
{
  EFI_STATUS                Status;
  UINTN                     Cpu;
  EFI_PROCESSOR_INFORMATION CpuInfo;

  if (NULL == PlatformInfoMsr) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gSmst->SmmLocateProtocol (&gEfiSmmCpuServiceProtocolGuid, NULL, &gSmmCpuService);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SMM CPU Services protocol location error (status %r)\n", Status));
    return Status;
  }

  InitializeSpinLock(&gApLock);

  for (Cpu = 0; Cpu < gSmst->NumberOfCpus; Cpu++) {
    Status = gSmmCpuService->GetProcessorInfo (gSmmCpuService, Cpu, &CpuInfo);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Processor Info retrieval error (status %r)\n", Status));
      return Status;
    }

    if (Cpu == gSmst->CurrentlyExecutingCpu) {
      if (Socket == CpuInfo.Location.Package) {
        GetCpuPlatformInofMsr (PlatformInfoMsr);
        return EFI_SUCCESS;
      }
      continue;
    }

    if (Socket == CpuInfo.Location.Package) {
      AcquireSpinLock(&gApLock);
      Status = gSmst->SmmStartupThisAp ((EFI_AP_PROCEDURE)GetCpuPlatformInofMsrAp, Cpu, PlatformInfoMsr);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "CPU (%d) failed to dispatch task (status %r)\n", Cpu, Status));
        ReleaseSpinLock(&gApLock);
        return Status;
      } else {
        while (!AcquireSpinLockOrFail(&gApLock)) {
          CpuPause();
        }
        ReleaseSpinLock(&gApLock);
        return EFI_SUCCESS;
      }
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Send ADR core/mesh ratio to pCode to initiate IIO Flush

  @param[in] Socket   - Socket ID

  @retval EFI_SUCCESS - Operation completed successfully,
                        error code otherwise

**/
EFI_STATUS
StartIioFlush (
  UINT8 Socket
  )
{
  EFI_STATUS                          Status;
  ICX_MSR_PLATFORM_INFO_REGISTER      PlatformInfoMsr;
  UINT32                              PcuMailBoxData = 0;
  UINT32                              PcodeMailBoxStatus;

  Status = GetSocketPlatformInfoMsr (Socket, &PlatformInfoMsr.Uint64);
  if(EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Socket (%d) Platform Info MSR retrieval failed (status %r)\n", Socket, Status));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "Socket (%d) Write to PCU mailbox to initiate IIO Flush (core ratio: 0x%x, mesh ratio: 0x%x)\n",
    Socket, (UINT8)PlatformInfoMsr.Bits.MaxEfficiencyRatio, (UINT8)PlatformInfoMsr.Bits.MaxEfficiencyRatio));

  PcuMailBoxData = ((((UINT8)PlatformInfoMsr.Bits.MaxEfficiencyRatio) << MAILBOX_BIOS_CMD_ENHANCED_ADR_DATA_MESH_RATIO_LSB) | ((UINT8)PlatformInfoMsr.Bits.MaxNonTurboLimRatio));

  PcodeMailBoxStatus = ReadModifyWritePcuMailbox (Socket, MAILBOX_BIOS_CMD_ZERO, MAILBOX_BIOS_CMD_ENHANCED_ADR, &PcuMailBoxData, 0, 0);
  if (PcodeMailBoxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    DEBUG ((DEBUG_ERROR, "Socket (%d) PCU mailbox command to initiate IIO Flush failed (MB Status 0x%x)\n", Socket, PcodeMailBoxStatus));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}


/**
  Executes IIO Flush

  @param None

  @retval EFI_SUCCESS  - Operation completed successfully,
                         error code otherwise

**/
VOID
FlushIioCache (
  VOID
  )
{
  EFI_STATUS                      Status;
  UINT8                           Stack;
  UINT8                           Socket;
  UINT64                          Timeout;
  CIPSTS_IIO_VTD_STRUCT           CipStsIioVtd;
  EFI_CPU_CSR_ACCESS_PROTOCOL     *CpuCsrAccess;

  DEBUG((DEBUG_ERROR, "%a()<\n", __FUNCTION__));

  Status = gSmst->SmmLocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &CpuCsrAccess);
  if (Status) {
    DEBUG ((DEBUG_ERROR, "CpuCsrAccess is not found!\n"));
    return;
  }

  //
  // Execute IIO flush on all enabled sockets
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Skip disabled sockets
    //
    if (!gSystemMemoryMap->Socket[Socket].SocketEnabled) {
      continue;
    }
    //
    // Initiate IIO Flush for the socket
    //
    Status = StartIioFlush (Socket);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "IIO Flush failed to start (status %r)\n", Status));
      return;
    }
  }

  //
  // Check IIO flush completion for enabled sockets
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Skip disabled sockets
    //
    if (!gSystemMemoryMap->Socket[Socket].SocketEnabled) {
      continue;
    }
    //
    // Check IIO Flush state for the socket
    //
    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      //
      // Check if IIO Stack enabled
      //
      if (gSystemMemoryMap->Socket[Socket].IioStackBitmap & (BIT0 << Stack)) {
        //
        // Check IIO Flush completion state
        //
        DEBUG ((DEBUG_INFO, "Socket (%d) Stack (%d) IIO flush - wait for completion\n", Socket, Stack));
        //
        // Initialize timeout value
        //
        Timeout = (GetTimeInNanoSecond(GetPerformanceCounter ()) + IIO_CACHE_POLLING_STATUS_TIMEOUT);
        //
        // Poll status register for IIO flush completion
        //
        CipStsIioVtd.Data = CpuCsrAccess->ReadCpuCsr (Socket, Stack, CIPSTS_IIO_VTD_REG);
        while (!CipStsIioVtd.Bits.flush_pending_writes) {
          CipStsIioVtd.Data = CpuCsrAccess->ReadCpuCsr (Socket, Stack, CIPSTS_IIO_VTD_REG);
          if (GetTimeInNanoSecond(GetPerformanceCounter ()) >= Timeout) {
            break;
          }
        }
        //
        // Check IIO Flush completion status
        //
        if (!CipStsIioVtd.Bits.flush_pending_writes) {
          DEBUG ((DEBUG_INFO, "Socket (%d) Stack (%d) IIO flush failed (timeout)\n", Socket, Stack));
        } else {
          DEBUG ((DEBUG_INFO, "Socket (%d) Stack (%d) IIO flush completed\n", Socket, Stack));
        }
      }
    }
  }
  DEBUG ((DEBUG_ERROR, ">%a()\n", __FUNCTION__));
}
#endif

/**

  This function is used to flushes the processor caches serially by executing WBINVD instruction

  @param[in/out] Buffer - The pointer to private data buffer.

  @retval None

**/
VOID
FlushProcessorCache (
  IN OUT VOID  *Buffer
  )
{
  AsmWbinvd ();
}

/**

  This function is used to flushes the processor caches on AP

  @param[in/out] Buffer - The pointer to private data buffer.

  @retval None

**/
VOID
FlushProcessorCacheAp (
  IN OUT VOID  *Buffer
  )
{
#if defined(SKX_HOST) || defined(CPX_HOST)
  //
  // Change performance state (CLX/CPX only)
  // (for ICX performance state change is
  // (requested along with IIO Flush initiation
  // via BIOS to PCODE mailbox command)
  //
  ChangeCpuPerformanceState (NULL);
#endif
  //
  // Run CPU Cache flush
  //
  FlushProcessorCache (NULL);
  ReleaseSpinLock (&gApLock);
}

/**
  Executes CPUs Cache Flush (serial mode)

  @param None

  @retval EFI_SUCCESS  - Operation completed successfully,
                         error code otherwise

**/
VOID
FlushProcessorsCache (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       Cpu;

  DEBUG((DEBUG_ERROR, "%a()<\n", __FUNCTION__));

  InitializeSpinLock(&gApLock);

  for (Cpu = 0; Cpu < gSmst->NumberOfCpus; Cpu++) {
    if (Cpu != gSmst->CurrentlyExecutingCpu) {
      AcquireSpinLock(&gApLock);
      Status = gSmst->SmmStartupThisAp ((EFI_AP_PROCEDURE)FlushProcessorCacheAp, Cpu, NULL);
      if (!EFI_ERROR (Status)) {
        while (!AcquireSpinLockOrFail(&gApLock)) {
          CpuPause();
        }
      }
      ReleaseSpinLock(&gApLock);
    } else {
      FlushProcessorCache (NULL);
    }
  }

  DEBUG((DEBUG_ERROR, ">%a()\n", __FUNCTION__));
}

/**

  Routine description: Reads data from the provided range

  @return VOID

**/
VOID
EFIAPI
CoreFlushTask (
  IN VOID  *Param
  )
{
  CORE_FLUSH_TASK *Task = (CORE_FLUSH_TASK *)Param;
  const UINT32    BlockStep = (Task->Interleave * (Task->DdrtCount - 1));
  volatile UINT8  *ReadLoc = (UINT8 *)(Task->StartingAddress + (Task->Interleave * Task->DimmPos));
  const UINT8     ReadsPerBlock = (UINT8)(Task->Interleave / DdrtTransactionSize);
  UINT8           i;
  volatile UINT8  ReadData;

  while ( ReadLoc < (UINT8 *)Task->EndingAddress ) {
    //
    // Serialize first reads to a block to avoid an assert in the page table fault handler (PageTbl.c)
    //
    while (!AcquireSpinLockOrFail(&gApLock)) {
      CpuPause();
    }
    ReadData = *ReadLoc;
    ReleaseSpinLock (&gApLock);
    for (i = 0; i < ReadsPerBlock; i++) {
      ReadData = *ReadLoc;
      ReadLoc += DdrtTransactionSize; // 64bytes returned in each DDRT read
    }
    ReadLoc += BlockStep;
  }

  while (!AcquireSpinLockOrFail(&gApLock)) {
    CpuPause();
  }
  //
  // Reporting that the core has completed
  //
  gCoreCompletes++;
  gCoreRunning--;
  ReleaseSpinLock (&gApLock);

  return;
}

/**

  Routine description: Flush all near memory cache lines containing AppDirect data

  @return VOID

**/
VOID
StartThreadFlushes (
  IN OUT UINT16  CoreTaskCount
  )
{
  EFI_PROCESSOR_INFORMATION  ProcessorInfoBuffer;
  EFI_STATUS                 Status = EFI_SUCCESS;
  UINTN                      Index;
  UINT8                      Task;
  UINT16                     TasksRemaining = CoreTaskCount;
  volatile UINTN             LastCoreCompletes;
  UINT8                      LastSocket;
  UINT8                      CoresStarted;
  const UINT8                MaxSocketStarts = 1;
  const UINT8                MaxActiveCores = 4;

  DEBUG((DEBUG_ERROR, "%a()<\n", __FUNCTION__));

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmCpuServiceProtocolGuid,
                    NULL,
                    &gSmmCpuService
                    );

  InitializeSpinLock (&gApLock);
  gCoreCompletes = 0;
  while (TasksRemaining != 0) {
    CoresStarted = 0;
    LastSocket = 0xff;
    for (Index = 0; Index < gSmst->NumberOfCpus; Index++) {
      if ((Index != gSmst->CurrentlyExecutingCpu) && (TasksRemaining != 0)) {
        gSmmCpuService->GetProcessorInfo (gSmmCpuService, Index, &ProcessorInfoBuffer);
        if ((LastSocket == ProcessorInfoBuffer.Location.Package) && (CoresStarted >= MaxSocketStarts)) {
          continue;
        }

        //
        // find a non-started core task for this core's socket
        //
        for (Task = 0; ((Task < CoreTaskCount) && (TasksRemaining != 0)); Task++) {
          if ((gCoreTasks[Task].Started != 1) && (gCoreTasks[Task].Socket == ProcessorInfoBuffer.Location.Package)) {
            //
            // Attempt to to start core on the task. It will fail if the core is busy
            //
            Status = EFI_NOT_READY;
            Status = gSmst->SmmStartupThisAp (CoreFlushTask, Index, (VOID *)&gCoreTasks[Task]);
            if (Status == EFI_SUCCESS) {
              if (LastSocket != ProcessorInfoBuffer.Location.Package) {
                CoresStarted = 0;
              }
              LastSocket = (UINT8) ProcessorInfoBuffer.Location.Package;
              CoresStarted++;
              gCoreTasks[Task].Started = 1;
              TasksRemaining--;
              while (!AcquireSpinLockOrFail(&gApLock)) {
                CpuPause();
              }
              gCoreRunning++;
              ReleaseSpinLock (&gApLock);
              //
              // Found a task for the core, go to the next core
              //
              break;
            }
          }
        }

        if (gCoreRunning >= MaxActiveCores) {
          volatile UINTN WaitingCoreCompletes = gCoreCompletes;

          while (!AcquireSpinLockOrFail(&gApLock)) {
            CpuPause();
          }
          DEBUG ((DEBUG_ERROR, " %d/%d r:%d a:%d ", gCoreCompletes, CoreTaskCount, TasksRemaining,(CoreTaskCount - (gCoreCompletes + TasksRemaining))));
          ReleaseSpinLock (&gApLock);
          //
          // Wait for a task to complete or for all tasks started to complete
          //
          while ( gCoreRunning != 0 ) { //(CoreTaskCount - TasksRemaining) != gCoreCompletes ) {
            CpuPause ();
            if (WaitingCoreCompletes != gCoreCompletes) {
              while (!AcquireSpinLockOrFail(&gApLock)) {
                CpuPause();
              }
              ReleaseSpinLock (&gApLock);
              WaitingCoreCompletes = gCoreCompletes;
            }
          }
          DEBUG ((DEBUG_ERROR, " %d/%d\n", gCoreCompletes, CoreTaskCount));
        }
      }
    }
  }

  //
  // Wait for all flushing to complete
  //
  LastCoreCompletes = gCoreCompletes;
  while ( gCoreRunning != 0 ) { // gCoreCompletes < CoreTaskCount ) {
    CpuPause ();
    if (LastCoreCompletes != gCoreCompletes) {
      DEBUG ((DEBUG_INFO, " %d/%d",  gCoreCompletes, CoreTaskCount));
      LastCoreCompletes = gCoreCompletes;
    }
  }

  //
  // Give all active cores time to completely shutdown before leaving
  //
  for (Task = 0; Task < MaxActiveCores; Task++) {
    CpuPause ();
  }

  DEBUG((DEBUG_ERROR, ">%a()\n", __FUNCTION__));
}

/**

  Routine description: This routine sets bit0 in CMOS Diagnostic Status register

  @param [in] Value - NM flush status bit value

  @return EFI status is returned

**/
EFI_STATUS
SignalNmFlushStatusToCmosReg (
  IN UINT8 Value
  )
{
  UINT8 Data;

  if ((SIG_NM_SUCCESS != Value) && (SIG_NM_ERROR != Value)) {
    DEBUG((DEBUG_ERROR, "NM flush state indicator value not supported (%d)\n", Value));
    return EFI_INVALID_PARAMETER;
  }

  Data = CmosRead8 (CMOS_DIAG_STAT_REG);

  if(Value) {
    Data |= CMOS_DIAG_STAT_REG_NMFLUSH;
  } else {
    Data &= ~CMOS_DIAG_STAT_REG_NMFLUSH;
  }

  CmosWrite8 (CMOS_DIAG_STAT_REG, Data);

  return EFI_SUCCESS;
}

/**

  Routine description: This iterates through the currently executing CPU MC banks and checks the valid flag

  @param[in,out] ErrorFound  Indicates whether valid MCE occurred or not

  @return None

**/
VOID
CheckCpuMcBanks (
  IN BOOLEAN *ErrorFound
  )
{
  UINT8 McBankCnt;
  UINT8 McBankIdx;

   if (NULL == ErrorFound){
     ASSERT (FALSE);
     return;
   }

   //
   // Get number of MC Banks bit[0:7]
   //
   McBankCnt = (UINT8)BitFieldRead64(AsmReadMsr64 (MSR_IA32_MCG_CAP), MC_BANK_CNT_BIT_START, MC_BANK_CNT_BIT_END);

   if (McBankCnt > NELEMENTS (gMcBankMsrList)) {
     ASSERT (FALSE);
     return;
   }

   *ErrorFound = FALSE;

   for(McBankIdx = 0; McBankIdx < McBankCnt; McBankIdx++) {
     //
     // Check MC Bank data valid bit[63]
     //
     if (BitFieldRead64(AsmReadMsr64 (gMcBankMsrList[McBankIdx].McStatusMsr), MC_BANK_DATA_VALID_BIT, MC_BANK_DATA_VALID_BIT)) {
       //
       // MC Bank data valid, no need to process further error detected during the flush
       //
       *ErrorFound = TRUE;
       break;
     }
   }
}

/**

  Routine description: This iterates through the CPU MC banks and checks the valid flag

  @param[in,out] ErrorFound  Indicates whether valid MCE occurred or not

  @return None

**/
VOID
CheckCpuMcBanksAp (
  IN BOOLEAN *ErrorFound
  )
{
  if (NULL == ErrorFound){
    ASSERT (FALSE);
    return;
  }

  CheckCpuMcBanks (ErrorFound);
  ReleaseSpinLock (&gApLock);
}

/**

  Routine description: This routine checks the MC banks on all CPUs

  @param None

  @return EFI status is returned

**/
EFI_STATUS
CheckAllMcBanks (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       CpuIdx;
  BOOLEAN     ErrorFound = FALSE;

  DEBUG((DEBUG_ERROR, "%a()<\n", __FUNCTION__));

  InitializeSpinLock(&gApLock);

  for (CpuIdx = 0; CpuIdx < gSmst->NumberOfCpus; CpuIdx++) {
    if (CpuIdx != gSmst->CurrentlyExecutingCpu) {
      AcquireSpinLock(&gApLock);
      Status = gSmst->SmmStartupThisAp ((EFI_AP_PROCEDURE)CheckCpuMcBanksAp, CpuIdx, &ErrorFound);
      if (!EFI_ERROR (Status)) {
        while (!AcquireSpinLockOrFail(&gApLock)) {
          CpuPause();
        }
      }
      ReleaseSpinLock(&gApLock);
    } else {
      CheckCpuMcBanks (&ErrorFound);
    }

    if (ErrorFound) {
      DEBUG((DEBUG_ERROR, "MCE occurred during NM flush - signal NM Flush failure!!!\n"));
      Status = SignalNmFlushStatusToCmosReg (SIG_NM_ERROR);
      ASSERT_EFI_ERROR (Status);
      break;
    }
  }

  DEBUG((DEBUG_ERROR, ">%a()\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**

  Routine description: This routine disables the MC banks on the currently executing CPU

  @param[in,out] Buffer  The pointer to private data buffer

  @return None

**/
VOID
DisableCpuMcBanks (
  IN OUT VOID  *Buffer
  )
{
  UINT8 McBankCnt;
  UINT8 McBankIdx;

  //
  // Get number of MC Banks bit[0:7]
  //
  McBankCnt = (UINT8)BitFieldRead64(AsmReadMsr64 (MSR_IA32_MCG_CAP), MC_BANK_CNT_BIT_START, MC_BANK_CNT_BIT_END);

  if (McBankCnt > NELEMENTS (gMcBankMsrList)) {
    ASSERT (FALSE);
    return;
  }

  for(McBankIdx = 0; McBankIdx < McBankCnt; McBankIdx++) {
    AsmWriteMsr64 (gMcBankMsrList[McBankIdx].McCtlMsr, 0);
  }
}

/**

  Routine description: This routine disables the MC banks on the CPU

  @param[in,out] Buffer  The pointer to private data buffer

  @return None

**/
VOID
DisableCpuMcBanksAp (
  IN OUT VOID  *Buffer
  )
{
  DisableCpuMcBanks (NULL);
  ReleaseSpinLock (&gApLock);
}

/**

  Routine description: This routine disables the MC banks on all CPUs

  @param None

  @return EFI status is returned

**/
EFI_STATUS
DisableAllMcBanks (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       CpuIdx;

  DEBUG((DEBUG_ERROR, "%a()<\n", __FUNCTION__));

  InitializeSpinLock(&gApLock);

  for (CpuIdx = 0; CpuIdx < gSmst->NumberOfCpus; CpuIdx++) {
    if (CpuIdx != gSmst->CurrentlyExecutingCpu) {
      AcquireSpinLock(&gApLock);
      Status = gSmst->SmmStartupThisAp ((EFI_AP_PROCEDURE)DisableCpuMcBanksAp, CpuIdx, NULL);
      if (!EFI_ERROR (Status)) {
        while (!AcquireSpinLockOrFail(&gApLock)) {
          CpuPause();
        }
      }
      ReleaseSpinLock(&gApLock);
    } else {
      DisableCpuMcBanks (NULL);
    }
  }

  DEBUG((DEBUG_ERROR, ">%a()\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**

  Routine description: This routine retrieves the bit0 value from CMOS Diagnostic Status register

  @param None

  @return NM flush status bit value

**/

UINT8
GetNmFlushStatusFromCmosReg (
  VOID
  )
{
  UINT8 Data;

  Data = CmosRead8 (CMOS_DIAG_STAT_REG);
  if (Data & CMOS_DIAG_STAT_REG_NMFLUSH) {
    return (UINT8)SIG_NM_ERROR;
  }
  return (UINT8)SIG_NM_SUCCESS;
}

/**

  Routine description: Flush all near memory cache lines containing AppDirect data.

  @return EFI status is returned

**/
EFI_STATUS
FlushNearMemoryCache (
  VOID
  )
{
  EFI_STATUS                                  Status;
  UINT8                                       i;
  EFI_CRYSTALRIDGE_FLUSH_NEAR_MEMORY_PROTOCOL *CrystalRidgeFlushNearMemoryProtocolPtr;

  DEBUG ((DEBUG_ERROR, "%a()<\n", __FUNCTION__));
  if (gSmst != NULL) {
    Status = gSmst->SmmLocateProtocol (&gEfiCrystalRidgeFlushNearMemoryProtocolGuid, NULL, &CrystalRidgeFlushNearMemoryProtocolPtr);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "gEfiCrystalRidgeFlushNearMemoryProtocolGuid locate failed: %r\n", Status));
      return EFI_NOT_FOUND;
    }

    if (CrystalRidgeFlushNearMemoryProtocolPtr != NULL) {
      gSystemMemoryMap = CrystalRidgeFlushNearMemoryProtocolPtr->SystemMemoryMap;
      gTaskCount       = CrystalRidgeFlushNearMemoryProtocolPtr->CoreTasksCount;
      if (*gTaskCount > 0) {
        //
        // Disable all MC banks before NM Flush is executed
        //
        DEBUG ((DEBUG_INFO, "Mask MC exception generation\n"));
        DisableAllMcBanks ();

        //
        // Executes if Pmem caching enabled with flush task count (gTaskCount > 0)
        // if gTaskCount is 0 or less, switch to else loop where iio/processor/No flush required
        //
        gCoreTasks = CrystalRidgeFlushNearMemoryProtocolPtr->CoreTasks;
        gInCRFlushNMProtocol = CrystalRidgeFlushNearMemoryProtocolPtr->InCrystalridgeFlushNMProtocol;
        gErrCount = CrystalRidgeFlushNearMemoryProtocolPtr->ErrCnt;

        DEBUG ((DEBUG_ERROR, "Near Memory cache flush Core Tasks\n Socket  Starting       ..       Ending          Interleave\n"));
        for (i = 0; i < *gTaskCount; i++) {
          DEBUG ((DEBUG_ERROR, "%2d- S%d %16lx .. %16lx       %d\n", i, gCoreTasks[i].Socket, gCoreTasks[i].StartingAddress, gCoreTasks[i].EndingAddress, gCoreTasks[i].Interleave));
          gCoreTasks[i].Started = 0;
        }

        //
        // Set Falg to indicate that we are Entering NM flush
        //
        *gInCRFlushNMProtocol = (UINT8)CR_FLUSH_NM_ETR;

        //
        // Signal CMOS bit flag to indicate NM flush failure
        //
        Status = SignalNmFlushStatusToCmosReg (SIG_NM_ERROR);
        ASSERT_EFI_ERROR (Status);

        FlushIioCache ();
        FlushProcessorsCache ();
        StartThreadFlushes (*gTaskCount);

        //
        // Set Flag to indicate that we are exiting NM flush
        //
        *gInCRFlushNMProtocol = (UINT8)CR_FLUSH_NM_EXT;

        //
        //  Check for any failure during flush and set CMOS bit
        //
        if ((*gErrCount < MAX_ERROR_LOGS_NMFLUSH)) {
           Status = SignalNmFlushStatusToCmosReg (SIG_NM_SUCCESS);
           ASSERT_EFI_ERROR (Status);
        }

        //
        // Check all MC banks after NM Flush is executed and look for MCE
        // (Signal NM Flush failure once MCE found)
        //
        DEBUG ((DEBUG_INFO, "Check MC Banks status\n"));
        CheckAllMcBanks ();

      } else {
         //
         // In case there is no NM cache flush task count and no EADR support, where no flush required
         //
         DEBUG((DEBUG_ERROR, "There are No NM Cache Flush tasks\n"));
      }
    } else {
      DEBUG((DEBUG_ERROR, "Near Memory Cache Flush is not required\n"));
    }
  }
  DEBUG ((DEBUG_ERROR, ">%a()\n", __FUNCTION__));

  return EFI_SUCCESS;
}

