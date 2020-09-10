/** @file
  This file contains the BIOS implementation of the BIOS-SSA Common Configuration API.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2018 Intel Corporation. <BR>

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
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/TimerLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/SysHostPointerLib.h>

/**
  Reads a variable-sized value from a memory mapped register using an absolute address.
  This function takes advantage of any caching implemented by BIOS.

  @param[in, out] This    - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Width   - The size of the value to write.
  @param[in]      Address - Address of the register to be accessed.
  @param[out]     Buffer  - Value storage location.

  @retval Nothing.
**/
VOID
(EFIAPI BiosReadMem) (
  SSA_BIOS_SERVICES_PPI *This,
  REG_WIDTH             Width,
  EFI_PHYSICAL_ADDRESS  Address,
  VOID                  *Buffer
  )
{
  MMIO_BUFFER  *MmioBuffer;

  MmioBuffer = (MMIO_BUFFER *) Buffer;
  RcDebugPrint (SDBG_BSSA, "\nStart: BiosReadMem (Width:%d, Address:0x%x)\n", Width, Address);

  switch (Width) {
    case RegWidth8:
      MmioBuffer->Data8 = MmioRead8 ((UINT32) Address);
      break;

    case RegWidth16:
      MmioBuffer->Data16 = MmioRead16 ((UINT32) Address);
      break;

    case RegWidth32:
      MmioBuffer->Data32 = MmioRead32 ((UINT32) Address);
      break;

    case RegWidth64:
      MmioBuffer->Data64 = MmioRead64 ((UINT32) Address);
      break;

    default:
      break;
  }
  RcDebugPrint (SDBG_BSSA, "End: BiosReadMem\n");

}

/**
  Writes a variable sized value to a memory mapped register using an absolute address.

  @param[in, out] This    - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Width   - The size of the value to write.
  @param[in]      Address - Address of the register to be accessed.
  @param[in]      Buffer  - Value to write.

  @retval Nothing.
**/
VOID
(EFIAPI BiosWriteMem) (
  SSA_BIOS_SERVICES_PPI *This,
  REG_WIDTH             Width,
  EFI_PHYSICAL_ADDRESS  Address,
  VOID                  *Buffer
  )
{

  MMIO_BUFFER  *MmioBuffer;

  RcDebugPrint (SDBG_BSSA,"\nStart: BiosWriteMem  (Width:%d, Address:%d)\n", Width, Address);

  MmioBuffer = (MMIO_BUFFER *) Buffer;
  switch (Width) {
    case RegWidth8:
      MmioWrite8 ((UINT32) Address, MmioBuffer->Data8);
      break;

    case RegWidth16:
      MmioWrite16 ((UINT32) Address, MmioBuffer->Data16);
      break;

    case RegWidth32:
      MmioWrite32 ((UINT32) Address, MmioBuffer->Data32);
      break;

    case RegWidth64:
      MmioWrite64 ((UINT32) Address, MmioBuffer->Data64);
      break;

    default:
      break;
  }

  RcDebugPrint (SDBG_BSSA, "End: BiosWriteMem\n");

}

/**
  Reads a variable sized value from I/O.
  This function takes advantage of any caching implemented by BIOS.

  @param[in, out] This    - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Width   - The size of the value to write.
  @param[in]      Address - Address of the I/O to be accessed.
  @param[out]     Buffer  - Value storage location.

  @retval Nothing.
**/
VOID
(EFIAPI BiosReadIo) (
  SSA_BIOS_SERVICES_PPI *This,
  REG_WIDTH             Width,
  UINT32                Address,
  VOID                  *Buffer
  )
{

  IO_BUFFER  *IoBuffer;

  IoBuffer = (IO_BUFFER *) Buffer;
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosReadIo (Width:%d, Address:0x%x)\n", Width, Address);

  switch (Width) {
    case RegWidth8:
      IoBuffer->Data8 = IoRead8 ((UINT16) Address);
      break;

    case RegWidth16:
      IoBuffer->Data16 = IoRead16 ((UINT16) Address);
      break;

    case RegWidth32:
      IoBuffer->Data32 = IoRead32 ((UINT16) Address);
      break;

    default:
      break;
  }
  RcDebugPrint (SDBG_BSSA, "End: BiosReadIo\n");

}

/**
  Writes a variable sized value to I/O.

  @param[in, out] This    - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Width   - The size of the value to write.
  @param[in]      Address - Address of the I/O to be accessed.
  @param[in]      Buffer  - Value to write.

  @retval Nothing.
**/
VOID
(EFIAPI BiosWriteIo) (
  SSA_BIOS_SERVICES_PPI *This,
  REG_WIDTH             Width,
  UINT32                Address,
  VOID                  *Buffer
  )
{

  IO_BUFFER  *IoBuffer;

  IoBuffer = (IO_BUFFER *) Buffer;
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosWriteIo (Width:%d, Address:%d)\n", Width, Address);

  switch (Width) {
    case RegWidth8:
      IoWrite8 ((UINT16) Address, IoBuffer->Data8);
      break;

    case RegWidth16:
      IoWrite16 ((UINT16) Address, IoBuffer->Data16);
      break;

    case RegWidth32:
      IoWrite32 ((UINT16) Address, IoBuffer->Data32);
      break;

    default:
      break;
  }
  RcDebugPrint (SDBG_BSSA, "End: BiosWriteIo\n");

}

/**
  Reads a variable sized value from the PCI config space register.
  This function takes advantage of any caching implemented by BIOS.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Width      - The size of the value to write.
  @param[in]      Address    - Address of the I/O to be accessed. Must be modulo 'Width'.
  @param[out]     Buffer     - Value storage location.

  @retval Nothing.
**/
VOID
(EFIAPI BiosReadPci) (
  SSA_BIOS_SERVICES_PPI           *This,
  REG_WIDTH                       Width,
  EFI_PEI_PCI_CFG_PPI_PCI_ADDRESS *Address,
  VOID                            *Buffer
  )
{
  UINT32            Value;
  PCI_BUFFER        *PciBuffer;
  PCI_CONFIG_SPACE  PciAddress;

  RcDebugPrint (SDBG_BSSA,"\nStart: BiosReadPci (Width:%d, Address:0x%x)\n", Width, Address);

  PciBuffer                 = (PCI_BUFFER *) Buffer;
  PciAddress.Value          = Address->Register; // 8 bit register offset modifed with other Union bits
  PciAddress.Bits.Bus       = Address->Bus;// 8 bit
  PciAddress.Bits.Device    = Address->Device;// 8 bit
  PciAddress.Bits.Function  = Address->Function;// 8 bit
  PciAddress.Bits.Reserved0 = 0;
  PciAddress.Bits.Enable    = 1;

  IoWrite32 (0x0CF8, PciAddress.Value); // We pass the 32bit ADDRESS to PciIndex = 0x0CF8
  Value = IoRead32 (0x0CFC);//Read value at address stored in CF8. PciData = 0x0CFC

  switch (Width) {
    case RegWidth8:
      PciBuffer->Data8 = (UINT8) Value;
      break;

    case RegWidth16:
      PciBuffer->Data16 = (UINT16) Value;
      break;

    case RegWidth32:
      PciBuffer->Data32 = Value;
      break;

    default:
      break;
  }
  RcDebugPrint (SDBG_BSSA, "End: BiosReadPci\n");
}

/**
  Writes a variable sized value to the PCI config space register.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Width      - The size of the value to write.
  @param[in]      Address    - Address of the I/O to be accessed. Must be modulo 'Width'.
  @param[in]      Buffer     - Value to write.

  @retval Nothing.
**/
VOID
(EFIAPI BiosWritePci) (
  SSA_BIOS_SERVICES_PPI           *This,
  REG_WIDTH                       Width,
  EFI_PEI_PCI_CFG_PPI_PCI_ADDRESS *Address,
  VOID                            *Buffer
  )
{
  PCI_BUFFER        *PciBuffer;
  PCI_CONFIG_SPACE  PciAddress;
  BOOLEAN           DoIt;
  UINT32            Value;
  UINT32            Value1;

  RcDebugPrint (SDBG_BSSA,"\nStart: BiosWritePci (Width:%d, Address:0x%x)\n", Width, Address);

  PciBuffer                 = (PCI_BUFFER *) Buffer;
  DoIt                      = TRUE;

  PciAddress.Value          = Address->Register; // 8 bit register OR offset/ value = 32 bits
  PciAddress.Bits.Bus       = Address->Bus;// 8 bit
  PciAddress.Bits.Device    = Address->Device;// 8 bit
  PciAddress.Bits.Function  = Address->Function;// 8 bit
  PciAddress.Bits.Reserved0 = 0;
  PciAddress.Bits.Enable    = 1;
  Value                     = 0;

  switch (Width) {
    case RegWidth8:
      BiosReadPci (This, RegWidth32, Address, (PCI_BUFFER *) &Value);
      Value &= ~0xFF;  // doing a type conversion to 8bit as Value is 32 bit
      Value |= (UINT32)PciBuffer->Data8;
      break;

    case RegWidth16:
      BiosReadPci (This, RegWidth32, Address, (PCI_BUFFER *) &Value);
      Value &= ~0xFFFF;  // doing a type conversion to 16bit as Value is 32 bit
      Value |= (UINT32)PciBuffer->Data16;
      break;

    case RegWidth32:
      Value = PciBuffer->Data32;
      break;

    default:
      Value = 0;
      DoIt  = FALSE;
      break;
  }

  if (DoIt) {
      IoWrite32 (0x0CF8, PciAddress.Value); //8 bit offset written to IO address port
      IoWrite32 (0xCFC, Value); //Value written to location pointed by above 8 bit offset
  }
 //  Value1 = IoRead32 (0x0CFC);  //Read value at address stored in CF8. PciData = 0x0CFC
  IoWrite32 (0x0CF8, PciAddress.Value); // We pass the 32bit ADDRESS to PciIndex = 0x0CF8
  Value1 = IoRead32 (0x0CFC);//Read value at address stored in CF8. PciData = 0x0CFC
  RcDebugPrint (SDBG_BSSA, "End: BiosWritePci\n");
}

/**
  Gets a base address to be used in the different memory map or MMIO register access functions.

  @param[in, out] This            - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket          - Zero based CPU socket number.
  @param[in]      Controller      - Zero based controller number.
  @param[in]      Index           - Additional index to locate the register.
  @param[in]      BaseAddressType - Value that indicates the type of base address to be retrieved.
  @param[out]     BaseAddress     - Where to write the base address

  @retval NotAvailable if the system does not support this feature.  Else UnsupportedValue if BaseAddressType is out of range.  Else Success.
**/
SSA_STATUS
(EFIAPI BiosGetBaseAddress) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 Index,
  BASE_ADDR_TYPE        BaseAddressType,
  EFI_PHYSICAL_ADDRESS  *BaseAddress
  )
{
  return NotAvailable;  //Function not available in server MRC
}

/**
  Function used to dynamically allocate memory.

  @param[in, out] This - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Size - Amount of memory in bytes to allocate.

  @retval Returns a pointer to an allocated memory block on success or NULL on failure.
**/
VOID *
(EFIAPI BiosMalloc) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT32                Size
  )
{
  UINT32    *bufferBase;

  RcDebugPrint (SDBG_BSSA,"\nStart: BiosMalloc (Size:%d)\n", Size);

  if ((Size == 0)) {
    return NULL;
  }

  bufferBase = RcAllocatePool (Size);

  RcDebugPrint (SDBG_BSSA, "End: BiosMalloc buffer:0x%x\n", bufferBase);
  return bufferBase;   // return the base of the memory range allocated.
}

/**
  Function used to release memory allocated using Malloc.

  @param[in, out] This   - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Buffer - The buffer to return to the free pool.

  @retval Nothing.
**/
VOID
(EFIAPI BiosFree) (
  SSA_BIOS_SERVICES_PPI *This,
  VOID                  *Buffer
  )
{
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosFree(0x%x)\n", Buffer);

  RcFreePool (Buffer);
  RcDebugPrint (SDBG_BSSA, "End: BiosFree\n");

}

/**
  Function used to stall the software progress for the given period of time.

  @param[in, out] This  - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Delay - The amount of time to wait, in microseconds.

  @retval Nothing.
**/
VOID
(EFIAPI BiosWait) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT32                Delay
  )
{
  RcDebugPrint (SDBG_BSSA, "\nStart: BiosWait()\n");

  FixedDelayMicroSecond (Delay);
  RcDebugPrint (SDBG_BSSA, "End: BiosWait\n");
}

/**
  Function used to add data to the BDAT v4 or later structure that will be created by BIOS in reserved physical memory.

  @param[in, out] This        - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      SchemaGuid  - The Schema ID GUID.  This is a unique identifier that indicates the format of the data to be added to the BDAT structure.
  @param[in]      Data        - The location of the data block to add to the BDAT structure.
  @param[in]      DataSize    - The size of the data to be added to the BDAT structure in bytes.

  @retval NotAvailable if the system does not support this feature.  Else UnsupportedValue if there is insufficient memory to add the data to the structure.  Else Success.
**/
SSA_STATUS
(EFIAPI BiosSaveToBdat) (
  SSA_BIOS_SERVICES_PPI *This,
  EFI_GUID              *SchemaGuid,
  CONST UINT8           *Data,
  UINT32                DataSize
  )
{

  UINT8     guidIdx = 0;
  BOOLEAN   GuidAlreadyExists = FALSE;
  VOID      *HobPtr = NULL;
  UINT32    RealHobSize;
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  if (DataSize > MAX_HOB_ENTRY_SIZE) {  //If Data Size of Blob is > 60kB exit
    RcDebugPrint (SDBG_BSSA, "Datasize is biger tha max HOB size\n");
    return UnsupportedValue;
  }

  RcDebugPrint (SDBG_BSSA, "\nStart: BiosSaveToBdat (DataSize:%d, Data Location:0x%x)\n", DataSize, Data);

  //Checking if this guid already exists, if yes, we update the number of hobs with the same guid
  for (guidIdx = 0; guidIdx < Host->var.mem.bssaNumberDistinctGuids; guidIdx++) {
    if (CompareGuid(&Host->var.mem.bssaBdatGuid[guidIdx], SchemaGuid)) { //if not new guid, enter and update hobs per guid
      if (Host->var.mem.bssaNumberHobs[guidIdx] > 254) {
        //Run out of space to store this guid. Shouldn't happen because we shouldn't have enough memory to hit this.
        RcDebugPrint (SDBG_BSSA, "Too many HOBS for a single guid.\n");

        return UnsupportedValue;
      } else {
        Host->var.mem.bssaNumberHobs[guidIdx] += 1;
        GuidAlreadyExists = TRUE;
        RcDebugPrint (SDBG_BSSA, "Guid found in table. guidIdx: %d, numhobs: %d\n", guidIdx, Host->var.mem.bssaNumberHobs[guidIdx]);
        break;
      }
    }
  }

  //Trying to add the guid; for a new guid we update "bssaNumberDistinctGuids", to indicate no. of unique guids
  if (!GuidAlreadyExists) {
    if (Host->var.mem.bssaNumberDistinctGuids >= MAX_NUMBER_SSA_BIOS_RESULTS) {
      RcDebugPrint (SDBG_BSSA, "Reached max number of unique Guids\n");
      return UnsupportedValue;
    } else {
      Host->var.mem.bssaBdatGuid[Host->var.mem.bssaNumberDistinctGuids] = *SchemaGuid;
      Host->var.mem.bssaNumberHobs[Host->var.mem.bssaNumberDistinctGuids] = 1;
      RcDebugPrint (SDBG_BSSA, "New Guid found. guidIdx: %d, numhobs: %d\n", Host->var.mem.bssaNumberDistinctGuids, Host->var.mem.bssaNumberHobs[Host->var.mem.bssaNumberDistinctGuids]);
      Host->var.mem.bssaNumberDistinctGuids += 1;
    }
  }

  //Create the HOB
  HobPtr = BuildGuidDataHob (SchemaGuid, (UINT8 *)Data, DataSize);
  if (HobPtr == NULL) {
    RcDebugPrint (SDBG_BSSA, "Couldn't create HOB\n");
    if (GuidAlreadyExists) {
      Host->var.mem.bssaNumberHobs[guidIdx] -= 1;
    } else {
      Host->var.mem.bssaNumberDistinctGuids -= 1;
      Host->var.mem.bssaNumberHobs[Host->var.mem.bssaNumberDistinctGuids] = 0;
    }
    return UnsupportedValue;
  }

  RealHobSize = DataSize;
  if ((RealHobSize % 8) != 0) {
    //BuildGuidDataHob function description states that the size of the created GUID is
    //multiple of 8. Correcting size to match this.
    RealHobSize += (8 - (RealHobSize % 8));
  }

  Host->var.mem.bssaBdatSize  += RealHobSize; //Updating total size of all HOBs being created

  RcDebugPrint (SDBG_BSSA, "HOB location: 0x%x\n", HobPtr);
  RcDebugPrint (SDBG_BSSA, "End: BiosSaveToBdat\n");
  return Success;
}

/**
  This function to register a new handler for a hook.

  @param[in, out] This        - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      HookGuid    - GUID defining the hook to invoke the handlers for.
  @param[in]      HookHandler - Function pointer to the new hook handler. This hook handler will be invoked whenever a corresponding call to RunHook() is made with the correct hook GUID.

  @retval NotAvailable if the system does not support this feature.  Else UnsupportedValue if HookGuid or HookHandler is NULL or HookGuid is invalid.  Else Failure if unable to allocate a buffer to store the hook.  Else Success.
**/
SSA_STATUS
(EFIAPI BiosRegisterHookHandler) (
  SSA_BIOS_SERVICES_PPI *This,
  VOID                  *HookGuid,
  SSA_BIOS_HOOK_HANDLER HookHandler
  )
{
  return NotAvailable;
}

/**
  This function invokes all registered handlers for a specific hook.

  @param[in, out] This        - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      HookGuid    - GUID defining the hook to invoke the handlers for.
  @param[in]      Parameters  - Data passed to the hook handlers. The type and format of this data differs depending on what hook is being invoked.  The type and format of the data must be the same as all other cases where RunHook() is called with a specific hook GUID.  This value can be NULL if the hook does not pass any additional data.

  @retval NotAvailable if the system does not support this feature.  Else UnsupportedValue if HookGuid is NULL or invalid.  Else Success.
**/
SSA_STATUS
(EFIAPI BiosRunHook) (
  SSA_BIOS_SERVICES_PPI *This,
  VOID                  *HookGuid,
  VOID                  *Parameters
  )
{
  return NotAvailable;
}

/**
  This function saves a pointer into a table that can later be retrieved.

  @param[in, out] This        - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      PointerId   - GUID that uniquely identifies the pointer.
  @param[in]      Pointer     - The pointer to save.

  @retval NotAvailable if the system does not support this feature.  Else UnsupportedValue if PointerId or Pointer is NULL or PointerId is invalid.  Else Failure if unable to allocate a buffer to store the pointer.  Else Success.
**/
SSA_STATUS
(EFIAPI BiosSaveDataPointer) (
  SSA_BIOS_SERVICES_PPI *This,
  EFI_GUID              *PointerId,
  VOID                  *Pointer
  )
{
  return NotAvailable;
}

/**
  This function returns a pointer that was earlier saved using SaveDataPointer function.

  @param[in, out] This        - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      PointerId   - GUID that uniquely identifies the pointer.

  @retval The value of the pointer or NULL if the pointer for the specified GUID does not exist.
**/
VOID *
(EFIAPI BiosRestoreDataPointer) (
  SSA_BIOS_SERVICES_PPI *This,
  EFI_GUID              *PointerId
  )
{
  return NULL;
}

/**
  This function disables the watchdog timer.

  @param[in, out] This - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.

  @retval Success when the watchdog timer is successfully disabled or if the platform does not have a watchdog timer.  Else Failure.
**/
VOID
(EFIAPI BiosDisableWatchdogTimer) (
  SSA_BIOS_SERVICES_PPI *This
  )
{
  //NotAvailable
}

/**
  Function used to get the elapsed time since the last CPU reset in milliseconds.

  @param[in, out] This      - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     Timestamp - Pointer to were the elapsed time since the last CPU reset in milliseconds will be written.

  @retval Nothing.
**/
VOID
(EFIAPI BiosGetTimestamp) (
  SSA_BIOS_SERVICES_PPI    *This,
  UINT32                   *Timestamp
  )
{
  UINT64    startTsc  = 0;  //Current time
  UINT64    endTsc    = 0;  //Time at last CPU reset = 0

  RcDebugPrint (SDBG_BSSA, "Start: BiosGetTimestamp\n");

  endTsc = GetPerformanceCounter ();  //Current timestamp

  *Timestamp = TimeDiff(startTsc, endTsc, TDIFF_UNIT_MS); //Converting current TimeStamp to MS (Time Difference since the last CPU reset() and current time)

  RcDebugPrint (SDBG_BSSA, "End: BiosGetTimestamp\n");
}
// end file BiosSsaCommonConfig.c
