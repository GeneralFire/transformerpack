/** @file
  This code provides an instance of Pei PSMI Init Lib.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation.

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
#include <Library/PsmiConfig.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PsmiPolicyLib.h>
#include <Library/UsraAccessApi.h>
#include <Library/UsraCsrLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/KtiApi.h>
#include <Library/DebugLib.h>
#include <RcRegs.h>

//
// Size Definitions for PSMI handler and Trace Region
//
UINT32  PsmiHandlerSizeTab[4] = {0, SIZE_256KB, SIZE_512KB, SIZE_1MB};
UINT64  TraceSizeTab[16] = {
  0,
  SIZE_1MB,
  SIZE_2MB,
  SIZE_4MB,
  SIZE_8MB,
  SIZE_16MB,
  SIZE_32MB,
  SIZE_64MB,
  SIZE_128MB,
  SIZE_256MB,
  SIZE_512MB,
  SIZE_1GB,
  SIZE_2GB,
  SIZE_4GB,
  SIZE_8GB,
  SIZE_16GB};

BOOLEAN IsPowerofTwo (UINT64 Operand)
{
  if (Operand == 0) {
    return TRUE;
  }
  return (BOOLEAN) ((Operand & (Operand - 1)) == 0);
}

/**
  Ensures that the North Peak device BAR memory space is activated for use.

  @param  Socket                Designates socket in which to activate NPK on

  @retval VOID
**/
VOID
EnableNpkMemSpace (
  UINT8  Socket
  )
{
  CMD_NPK_MAIN_STRUCT           CommandRegister;

  //
  // Enable config space on NPK device
  //
  CommandRegister.Data = UsraCsrRead (Socket, 0, CMD_NPK_MAIN_REG);
  CommandRegister.Bits.mem = 1;
  CommandRegister.Bits.bme = 1;
  UsraCsrWrite (Socket, 0, CMD_NPK_MAIN_REG, CommandRegister.Data);

  return;
}

/**
  Initializes PSMI Handler and Trace memory size

  @param   None

  @retval  VOID

**/
VOID
PsmiInit (
  VOID
  )
{
  struct psmiSetup              *PsmiPolicy = NULL;
  PSMI_INPUT_SCRPD1_REGISTER    Scrpd1;
  UINT8                         Socket = 0;
  PSMI_TRACE_INFO_HOB           Hob;
  VOID                          *HobPtr = NULL;
  UINT8                         i = 0;
  UINT8                         Count = 0;
  UINT64                        Size = 0;
  UINT64                        temp = 0;
  struct psmiSetup              PsmiInput;
  BOOLEAN                       UseSetupValue = FALSE;

  Scrpd1.RegValue = 0;

  DEBUG ((DEBUG_INFO, "PsmiInit () - Start\n"));

  //Initialize Hob struct to 0
  ZeroMem (&Hob, sizeof (Hob));
  ZeroMem (&PsmiInput, sizeof (PsmiInput));

  PsmiPolicy = GetPsmiPolicy ();
  if (PsmiPolicy == NULL) {
    DEBUG ((DEBUG_ERROR, "PsmiPolicy is NULL. Disable PSMI \n"));
    return;
  }

  if (PsmiPolicy->GlobalPsmiEnable == 0) {
    DEBUG ((DEBUG_INFO, "Global Psmi setup is disabled. Disable PSMI \n"));
    return;
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!SocketPresent (Socket)) {
      continue;
    }

    //
    // If NPK MTB bar is uninitialized by IIO, skip PSMI initialization
    //
    if (((UsraCsrRead (Socket, 0, MTB_LBAR_NPK_MAIN_REG) & 0xFFF00000) == 0) &&
        (UsraCsrRead (Socket, 0, MTB_UBAR_NPK_MAIN_REG) == 0)) {
      continue;
    }

    EnableNpkMemSpace(Socket);

    if (PsmiPolicy->GlobalPsmiEnable == 2) {
      //Force setup values
      DEBUG ((DEBUG_INFO, "Force setup values for socket: %d\n", Socket));
      UseSetupValue = TRUE;
    } else{
      //
      // Read SCRPD1 register
      //
      Scrpd1.RegValue = UsraCsrRead (Socket, 0, SCRPD1_NPK_TPH_REG);

      if (Scrpd1.Bits.PsmiHandler == 0) {
        UseSetupValue = TRUE;
      }
    }

    if (UseSetupValue == TRUE) {
      DEBUG ((DEBUG_INFO, "Use Setup values for socket: %d \n", Socket));

      //
      // Initialize again in case we there were artifacts in the register read.
      //
      Scrpd1.RegValue = 0;

      //Calculate Reservation Request data value
      Scrpd1.Bits.PsmiHandler = PsmiPolicy->PsmiHandlerSize[Socket];
      if (PsmiPolicy->trace[Socket].PsmiTraceRegion[0] != 0) {
        Scrpd1.Bits.TraceRegion0Size = PsmiPolicy->trace[Socket].PsmiTraceBufferSizeRegion[0];
        Scrpd1.Bits.TraceRegion0CacheType = PsmiPolicy->trace[Socket].PsmiTraceMemTypeRegion[0];
      }
      if (PsmiPolicy->trace[Socket].PsmiTraceRegion[1] != 0) {
        Scrpd1.Bits.TraceRegion1Size = PsmiPolicy->trace[Socket].PsmiTraceBufferSizeRegion[1];
        Scrpd1.Bits.TraceRegion1CacheType = PsmiPolicy->trace[Socket].PsmiTraceMemTypeRegion[1];
      }
      if (PsmiPolicy->trace[Socket].PsmiTraceRegion[2] != 0) {
        Scrpd1.Bits.TraceRegion2Size = PsmiPolicy->trace[Socket].PsmiTraceBufferSizeRegion[2];
        Scrpd1.Bits.TraceRegion2CacheType = PsmiPolicy->trace[Socket].PsmiTraceMemTypeRegion[2];
      }
      if (PsmiPolicy->trace[Socket].PsmiTraceRegion[3] != 0) {
        Scrpd1.Bits.TraceRegion3Size = PsmiPolicy->trace[Socket].PsmiTraceBufferSizeRegion[3];
        Scrpd1.Bits.TraceRegion3CacheType = PsmiPolicy->trace[Socket].PsmiTraceMemTypeRegion[3];
      }
      if (PsmiPolicy->trace[Socket].PsmiTraceRegion[4] != 0) {
        Scrpd1.Bits.TraceRegion4Size = PsmiPolicy->trace[Socket].PsmiTraceBufferSizeRegion[4];
        Scrpd1.Bits.TraceRegion4CacheType = PsmiPolicy->trace[Socket].PsmiTraceMemTypeRegion[4];
      }

      UsraCsrWrite (Socket, 0, SCRPD1_NPK_TPH_REG, Scrpd1.RegValue);

    } else {
      //Use SCRPD1 reg values
      // Use Policy structure to input values from Scratchpad register
      PsmiPolicy = &PsmiInput;
      //Turn on PSMI in Policy structure as handler size > 0
      PsmiPolicy->PsmiTrace[Socket] = 1;
      PsmiPolicy->PsmiHandlerSize[Socket] = (UINT8) Scrpd1.Bits.PsmiHandler;

      for (i = 0; i < MAX_TRACE_REGION; i++) {
        switch (i) {
        case 0:
          //Enable Trace Region only if buffer size is greater than 0
          if (Scrpd1.Bits.TraceRegion0Size == 0) {
            break;
          }
          PsmiPolicy->trace[Socket].PsmiTraceRegion[i] = 1;
          PsmiPolicy->trace[Socket].PsmiTraceBufferSizeRegion[i] = (UINT8) Scrpd1.Bits.TraceRegion0Size;
          PsmiPolicy->trace[Socket].PsmiTraceMemTypeRegion[i] = (UINT8) Scrpd1.Bits.TraceRegion0CacheType;
          break;
        case 1:
          //Enable Trace Region only if buffer size is greater than 0
          if (Scrpd1.Bits.TraceRegion1Size == 0) {
            break;
          }
          PsmiPolicy->trace[Socket].PsmiTraceRegion[i] = 1;
          PsmiPolicy->trace[Socket].PsmiTraceBufferSizeRegion[i] = (UINT8) Scrpd1.Bits.TraceRegion1Size;
          PsmiPolicy->trace[Socket].PsmiTraceMemTypeRegion[i] = (UINT8) Scrpd1.Bits.TraceRegion1CacheType;
          break;
        case 2:
          //Enable Trace Region only if buffer size is greater than 0
          if (Scrpd1.Bits.TraceRegion2Size == 0) {
            break;
          }
          PsmiPolicy->trace[Socket].PsmiTraceRegion[i] = 1;
          PsmiPolicy->trace[Socket].PsmiTraceBufferSizeRegion[i] = (UINT8) Scrpd1.Bits.TraceRegion2Size;
          PsmiPolicy->trace[Socket].PsmiTraceMemTypeRegion[i] = (UINT8) Scrpd1.Bits.TraceRegion2CacheType;
          break;
        case 3:
          //Enable Trace Region only if buffer size is greater than 0
          if (Scrpd1.Bits.TraceRegion3Size == 0) {
            break;
          }
          PsmiPolicy->trace[Socket].PsmiTraceRegion[i] = 1;
          PsmiPolicy->trace[Socket].PsmiTraceBufferSizeRegion[i] = (UINT8) Scrpd1.Bits.TraceRegion3Size;
          PsmiPolicy->trace[Socket].PsmiTraceMemTypeRegion[i] = (UINT8) Scrpd1.Bits.TraceRegion3CacheType;
          break;
        case 4:
          //Enable Trace Region only if buffer size is greater than 0
          if (Scrpd1.Bits.TraceRegion4Size == 0) {
            break;
          }
          PsmiPolicy->trace[Socket].PsmiTraceRegion[i] = 1;
          PsmiPolicy->trace[Socket].PsmiTraceBufferSizeRegion[i] = (UINT8) Scrpd1.Bits.TraceRegion4Size;
          PsmiPolicy->trace[Socket].PsmiTraceMemTypeRegion[i] = (UINT8) Scrpd1.Bits.TraceRegion4CacheType;
        }
      }
    }

    //
    // Skip if PSMI trace is not enabled for this socket
    //
    if ((PsmiPolicy->PsmiTrace[Socket] == 0) || (PsmiPolicy->PsmiHandlerSize[Socket] == 0)) {
      DEBUG ((DEBUG_INFO, "Psmi not enabled in setup for socket %d \n", Socket));
      continue;
    }

    Hob.SocketBuffer[Count].CurrentSocketNum = Socket;
    Hob.SocketBuffer[Count].PsmiHandlerSize = PsmiPolicy->PsmiHandlerSize[Socket];

    for (i = 0; i < MAX_TRACE_REGION; i++) {
      if (PsmiPolicy->trace[Socket].PsmiTraceRegion[i] == 1) {

        switch (PsmiPolicy->trace[Socket].PsmiTraceMemTypeRegion[i]) {
        case RegionDonotCare:
          //Cache type: Any
          Hob.SocketBuffer[Count].Trace[RegionDonotCare].Size += TraceSizeTab[PsmiPolicy->trace[Socket].PsmiTraceBufferSizeRegion[i]];
          break;
        case RegionUncacheable:
          //Cache type: Uncachable
          Hob.SocketBuffer[Count].Trace[RegionUncacheable].Size += TraceSizeTab[PsmiPolicy->trace[Socket].PsmiTraceBufferSizeRegion[i]];
          break;
        case RegionWriteCombine:
          //Cache type: Uncachable
          Hob.SocketBuffer[Count].Trace[RegionWriteCombine].Size += TraceSizeTab[PsmiPolicy->trace[Socket].PsmiTraceBufferSizeRegion[i]];
          break;
        }
      }
    }

    //Align memory requested from Memory Map to power of 2
    for (i = 0; i < MAX_TRACE_CACHE_TYPE; i++) {
      if (Hob.SocketBuffer[Count].Trace[i].Size == 0) {
        continue;
      }

      if (!IsPowerofTwo (Hob.SocketBuffer[Count].Trace[i].Size)) {
        temp = Hob.SocketBuffer[Count].Trace[i].Size;
        Size = 2;
        temp = RShiftU64 (temp, 1);
        while (temp > 0) {
          Size = LShiftU64 (Size, 1);
          temp = RShiftU64 (temp, 1);
        }

        Hob.SocketBuffer[Count].Trace[i].Size = Size;
      }
    }
    Count++;
    Hob.TotalSocketNum++;

  }

  if (Hob.TotalSocketNum > 0) {
    Hob.Version = 0x1;
    //Build data hob that will be updated by Memory Map and consumed by PSMI Late Flow
    HobPtr = BuildGuidDataHob (
               &gPsmiInitDataGuid,
               (VOID *)&Hob,
               (UINTN) sizeof (Hob)
               );
    if (HobPtr == NULL) {
      DEBUG ((DEBUG_ERROR, "Did not create HOB. Hob.TotalSocketNum is: %d\n", Hob.TotalSocketNum));
    }
  }

  return;
}


/**
  Configures PSMI post memory reservation, reserves PSMI handler,
  and populates NPK SCRPD1 and PSMI handler with the details 
  of PSMI reservations.

  @retval VOID
**/
VOID
ConfigurePsmi (
  VOID
  )
{
  UINT32                         PsmiHandlerSize = 0;
  UINT32                         PsmiHandlerBase = 0;
  PSMI_OUTPUT_SCRPD1_REGISTER    Scrpd1Output;
  PSMI_INPUT_SCRPD1_REGISTER     Scrpd1Input;
  PSMI_TRACE_INFO_HOB            *DataHob;
  PSMI_HANDLER_SPACE_OFFSET      PsmiHandlerSpace[MAX_SOCKET];
  UINT8                          Index, i;
  UINT64                         Size = 0;
  UINT64                         BaseAddress = 0;
  UINT8                          HandlerErrCode = ALLOCATION_SUCCESS;
  UINT32                         InputScrpdValue = 0;
  UINT64                         RegionSize = 0;
  UINT8                          RegionMemType = 0;
  UINT8                          Socket = 0;
  UINT8                          CurrentSocketNum = 0;
  EFI_HOB_GUID_TYPE             *GuidHob = NULL;
  UINTN                          HandlerBaseOffset = 0;

  DEBUG ((DEBUG_INFO, "ConfigurePsmi () - Start\n"));

  ZeroMem (&PsmiHandlerSpace, (sizeof (PSMI_HANDLER_SPACE_OFFSET) * MAX_SOCKET));

  //
  // Locate the Data hob, if not found , return
  //
  GuidHob = GetFirstGuidHob(&gPsmiInitDataGuid);
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_INFO, "ConfigurePsmi () - GuidHob is NULL \n"));
    return;
  }

  DataHob = (PSMI_TRACE_INFO_HOB *)GET_GUID_HOB_DATA(GuidHob);
  if (DataHob == NULL) {
    DEBUG ((DEBUG_ERROR, "ConfigurePsmi () - DataHob is NULL \n"));
    return;
  }

  for (Socket = 0; Socket < DataHob->TotalSocketNum; Socket++) {
    Scrpd1Output.RegValue = 0;
    CurrentSocketNum = DataHob->SocketBuffer[Socket].CurrentSocketNum;
    if (DataHob->SocketBuffer[Socket].PsmiHandlerSize == 0) {
      DEBUG ((DEBUG_ERROR, "ConfigurePsmi () - Psmi Handler Size is 0 for Socket: %d \n", Socket));
      continue;
    }

    if ((DataHob->SocketBuffer[Socket].Trace[0].Size == 0) && (DataHob->SocketBuffer[Socket].Trace[1].Size == 0) &&
        (DataHob->SocketBuffer[Socket].Trace[2].Size == 0)) {
      DEBUG ((DEBUG_ERROR, "ConfigurePsmi () - Trace region size is 0 for all cache types; Socket: %d \n", Socket));
      continue;
    }

    EnableNpkMemSpace (Socket);

    PsmiHandlerSize = PsmiHandlerSizeTab[DataHob->SocketBuffer[Socket].PsmiHandlerSize];
    PsmiHandlerBase = (UINT32) (UINTN) AllocateAlignedReservedPages (EFI_SIZE_TO_PAGES (PsmiHandlerSize), PsmiHandlerSize);

    if (PsmiHandlerBase == 0) {
      DEBUG ((DEBUG_ERROR, "AllocateAlignedReservedPages for PSMI Handler failed\n"));
      HandlerErrCode = ERROR_OUT_OF_RESOURCES;
    }

    Scrpd1Output.Bits.PsmiHandlerResultCode = HandlerErrCode;
    Scrpd1Output.Bits.PsmiHandlerBase = (PsmiHandlerBase >> 18);
    PsmiHandlerSpace[CurrentSocketNum].PsmiHandlerBaseAddress = PsmiHandlerBase;
    //
    // Read SCRPD1 register to get PSMI handler, trace memory sizes , Trace Region Cache Type
    //
    Scrpd1Input.RegValue = UsraCsrRead (Socket, 0, SCRPD1_NPK_TPH_REG);
    InputScrpdValue = Scrpd1Input.RegValue;

    for (Index = 0; Index < MAX_TRACE_CACHE_TYPE; Index++) {
      //
      // Skip if trace region size is 0
      //
      if (DataHob->SocketBuffer[Socket].Trace[Index].Size == 0) {
        continue;
      }
      Size = DataHob->SocketBuffer[Socket].Trace[Index].Size;

      if (DataHob->SocketBuffer[Socket].Trace[Index].BaseAddress == 0) {
        DEBUG ((DEBUG_ERROR, "Buffer pointer for cache type: %d is NULL\n", Index));
      }

      BaseAddress = (UINT64) (DataHob->SocketBuffer[Socket].Trace[Index].BaseAddress);

      for (i = 0; i < MAX_TRACE_REGION; i++) {
        switch (i) {
        case 0:
          RegionMemType = (UINT8) Scrpd1Input.Bits.TraceRegion0CacheType;
          RegionSize = (UINT64) TraceSizeTab[ (UINT8) Scrpd1Input.Bits.TraceRegion0Size];
          break;
        case 1:
          RegionMemType = (UINT8) Scrpd1Input.Bits.TraceRegion1CacheType;
          RegionSize = (UINT64) TraceSizeTab[ (UINT8) Scrpd1Input.Bits.TraceRegion1Size];
          break;
        case 2:
          RegionMemType = (UINT8) Scrpd1Input.Bits.TraceRegion2CacheType;
          RegionSize = (UINT64) TraceSizeTab[ (UINT8) Scrpd1Input.Bits.TraceRegion2Size];
          break;
        case 3:
          RegionMemType = (UINT8) Scrpd1Input.Bits.TraceRegion3CacheType;
          RegionSize = (UINT64) TraceSizeTab[ (UINT8) Scrpd1Input.Bits.TraceRegion3Size];
          break;
        case 4:
          RegionMemType = (UINT8) Scrpd1Input.Bits.TraceRegion4CacheType;
          RegionSize = (UINT64) TraceSizeTab[ (UINT8) Scrpd1Input.Bits.TraceRegion4Size];
          break;
        }

        if (RegionSize == 0) {
          continue;
        }

        if (RegionMemType == Index) {
          if (DataHob->SocketBuffer[Socket].Trace[Index].BaseAddress != 0) {
            //
            // Increment the BaseAddress and reduce size
            //
            switch (i) {
            case 0:
              PsmiHandlerSpace[CurrentSocketNum].TraceRegion0BaseAddress = BaseAddress;
              Scrpd1Output.Bits.TraceRegion0ResultCode = ALLOCATION_SUCCESS;
              break;
            case 1:
              PsmiHandlerSpace[CurrentSocketNum].TraceRegion1BaseAddress = BaseAddress;
              Scrpd1Output.Bits.TraceRegion1ResultCode = ALLOCATION_SUCCESS;
              break;
            case 2:
              PsmiHandlerSpace[CurrentSocketNum].TraceRegion2BaseAddress = BaseAddress;
              Scrpd1Output.Bits.TraceRegion2ResultCode = ALLOCATION_SUCCESS;
              break;
            case 3:
              PsmiHandlerSpace[CurrentSocketNum].TraceRegion3BaseAddress = BaseAddress;
              Scrpd1Output.Bits.TraceRegion3ResultCode = ALLOCATION_SUCCESS;
              break;
            case 4:
              PsmiHandlerSpace[CurrentSocketNum].TraceRegion4BaseAddress = BaseAddress;
              Scrpd1Output.Bits.TraceRegion4ResultCode = ALLOCATION_SUCCESS;
              break;
            }
            BaseAddress = BaseAddress + RegionSize;
            Size = Size - RegionSize;
          } else {
            switch (i) {
            case 0:
              Scrpd1Output.Bits.TraceRegion0ResultCode = DataHob->SocketBuffer[Socket].Trace[Index].AllocationError;
              break;
            case 1:
              Scrpd1Output.Bits.TraceRegion1ResultCode = DataHob->SocketBuffer[Socket].Trace[Index].AllocationError;
              break;
            case 2:
              Scrpd1Output.Bits.TraceRegion2ResultCode = DataHob->SocketBuffer[Socket].Trace[Index].AllocationError;
              break;
            case 3:
              Scrpd1Output.Bits.TraceRegion3ResultCode = DataHob->SocketBuffer[Socket].Trace[Index].AllocationError;
              break;
            case 4:
              Scrpd1Output.Bits.TraceRegion4ResultCode = DataHob->SocketBuffer[Socket].Trace[Index].AllocationError;
              break;
            }
            DEBUG ((DEBUG_ERROR, "Error Trace Region %d Error 0x%x \n", Index, DataHob->SocketBuffer[Socket].Trace[Index].AllocationError));
          }
        }
      }
    }
    PsmiHandlerSpace[CurrentSocketNum].PsmiInputRegValue = InputScrpdValue;
    HandlerBaseOffset = PsmiHandlerBase + PSMI_HANDLER_MEMORY_OFFSET_1000;
    CopyMem ((VOID *)HandlerBaseOffset , (VOID *)&(PsmiHandlerSpace[CurrentSocketNum].PsmiHandlerBaseAddress), sizeof (PSMI_HANDLER_SPACE_OFFSET));
    UsraCsrWrite (Socket, 0, SCRPD1_NPK_TPH_REG, Scrpd1Output.RegValue);

  }

  DEBUG ((DEBUG_INFO, "ConfigurePsmi () - End \n"));
  return;
}
