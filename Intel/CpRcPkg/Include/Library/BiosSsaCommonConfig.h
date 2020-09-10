/** @file
  BiosSsaCommonConfig.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2018 Intel Corporation. <BR>

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

#ifndef __BIOS_SSA_COMMON_CONFIG__
#define __BIOS_SSA_COMMON_CONFIG__

#pragma pack (push, 1)

/**
  Reads a variable-sized value from a memory mapped register using an absolute address.
  This function takes advantage of any caching implemented by BIOS.

  @param[in, out] This    - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Width   - The size of the value to write.
  @param[in]      Address - Address of the register to be accessed.
  @param[out]     Buffer  - Value storage location.

  @retval None.
**/
VOID
(EFIAPI BiosReadMem) (
  SSA_BIOS_SERVICES_PPI *This,
  REG_WIDTH             Width,
  EFI_PHYSICAL_ADDRESS  Address,
  VOID                  *Buffer
  );

/**
  Writes a variable sized value to a memory mapped register using an absolute address.

  @param[in, out] This    - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Width   - The size of the value to write.
  @param[in]      Address - Address of the register to be accessed.
  @param[in]      Buffer  - Value to write.

  @retval None.
**/
VOID
(EFIAPI BiosWriteMem) (
  SSA_BIOS_SERVICES_PPI *This,
  REG_WIDTH             Width,
  EFI_PHYSICAL_ADDRESS  Address,
  VOID                  *Buffer
  );

/**
  Reads a variable sized value from I/O.
  This function takes advantage of any caching implemented by BIOS.

  @param[in, out] This    - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Width   - The size of the value to write.
  @param[in]      Address - Address of the I/O to be accessed.
  @param[out]     Buffer  - Value storage location.

  @retval None.
**/
VOID
(EFIAPI BiosReadIo) (
  SSA_BIOS_SERVICES_PPI *This,
  REG_WIDTH             Width,
  UINT32                Address,
  VOID                  *Buffer
  );

/**
  Writes a variable sized value to I/O.

  @param[in, out] This    - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Width   - The size of the value to write.
  @param[in]      Address - Address of the I/O to be accessed.
  @param[in]      Buffer  - Value to write.

  @retval None.
**/
VOID
(EFIAPI BiosWriteIo) (
  SSA_BIOS_SERVICES_PPI *This,
  REG_WIDTH             Width,
  UINT32                Address,
  VOID                  *Buffer
  );

/**
  Reads a variable sized value from the PCI config space register.
  This function takes advantage of any caching implemented by BIOS.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Width      - The size of the value to write.
  @param[in]      Address    - Address of the I/O to be accessed. Must be modulo 'Width'.
  @param[out]     Buffer     - Value storage location.

  @retval None.
**/
VOID
(EFIAPI BiosReadPci) (
  SSA_BIOS_SERVICES_PPI           *This,
  REG_WIDTH                       Width,
  EFI_PEI_PCI_CFG_PPI_PCI_ADDRESS *Address,
  VOID                            *Buffer
  );

/**
  Writes a variable sized value to the PCI config space register.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Width      - The size of the value to write.
  @param[in]      Address    - Address of the I/O to be accessed. Must be modulo 'Width'.
  @param[in]      Buffer     - Value to write.

  @retval None.
**/
VOID
(EFIAPI BiosWritePci) (
  SSA_BIOS_SERVICES_PPI           *This,
  REG_WIDTH                       Width,
  EFI_PEI_PCI_CFG_PPI_PCI_ADDRESS *Address,
  VOID                            *Buffer
  );

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
  );

/**
  Function used to dynamically allocate memory.

  @param[in, out] This        - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Size        - Amount of memory in bytes to allocate.

  @retval Returns a pointer to an allocated memory block on success or NULL on failure.
**/
VOID *
(EFIAPI BiosMalloc) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT32                Size
  );

/**
  Function used to release memory allocated using Malloc.

  @param[in, out] This        - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Buffer      - The buffer to return to the free pool.

  @retval None.
**/
VOID
(EFIAPI BiosFree) (
  SSA_BIOS_SERVICES_PPI *This,
  VOID                  *Buffer
  );

/**
  Function used to stall the software progress for the given period of time.

  @param[in, out] This        - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Delay       - The amount of time to wait, in microseconds.

  @retval None.
**/
VOID
(EFIAPI BiosWait) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT32                Delay
  );

/**
  Function used to add data to the BDAT v4 or later structure that will be created by BIOS in reserved physical memory.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      SchemaGuid - The Schema ID GUID.  This is a unique identifier that indicates the format of the data to be added to the BDAT structure.
  @param[in]      Data       - The location of the data block to add to the BDAT structure.
  @param[in]      DataSize   - The size of the data to be added to the BDAT structure in bytes.

  @retval NotAvailable if the system does not support this feature.  Else UnsupportedValue if SchemaGuid is NULL or invalid.  Else UnsupportedValue if Data is NULL or DataSize is 0.  Else NotAvailable if there is insufficient memory to add the data to the structure.  Else Success.
**/
SSA_STATUS
(EFIAPI BiosSaveToBdat) (
  SSA_BIOS_SERVICES_PPI *This,
  EFI_GUID              *SchemaGuid,
  CONST UINT8           *Data,
  UINT32                DataSize
  );

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
  );

/**
  This function invokes all registered handlers for a specific hook.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      HookGuid   - GUID defining the hook to invoke the handlers for.
  @param[in]      Parameters - Data passed to the hook handlers. The type and format of this data differs depending on what hook is being invoked. The type and format of the data must be the same as all other cases where RunHook() is called with a specific hook GUID. This value can be NULL if the hook does not pass any additional data.

  @retval NotAvailable if the system does not support this feature.  Else UnsupportedValue if HookGuid is NULL or invalid.  Else Success.
**/
SSA_STATUS
(EFIAPI BiosRunHook) (
  SSA_BIOS_SERVICES_PPI *This,
  VOID                  *HookGuid,
  VOID                  *Parameters
  );

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
  );

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
  );

/**
  This function disables the watchdog timer.

  @param[in, out] This - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.

  @retval Success when the watchdog timer is successfully disabled or if the platform does not have a watchdog timer.  Else Failure.
**/
VOID
(EFIAPI BiosDisableWatchdogTimer) (
  SSA_BIOS_SERVICES_PPI *This
  );

/**
  Function used to get the elapsed time since the last CPU reset in milliseconds.

  @param[in, out] This      - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     Timestamp - Pointer to were the elapsed time since the last CPU reset in milliseconds will be written.

  @retval None.
**/
VOID
(EFIAPI BiosGetTimestamp) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  OUT UINT32                   *Timestamp
  );

#pragma pack (pop)
#endif // __BIOS_SSA_COMMON_CONFIG__
