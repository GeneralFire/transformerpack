/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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

#ifndef _PCI_MMIO32_ASSIGN_LIB_
#define _PCI_MMIO32_ASSIGN_LIB_

typedef struct {
  UINT32            BaseAddress;
  UINT32            LimitAddress;
  BOOLEAN           Used;  //TRUE - allocated, FALSE means free
} MMIO_MAP_INFO;

/**
  This API function is called in PEI Phase to assign MMIO resources(<4GB) to all PCI Devices blong to every IIO Stack.

  This API need to allocate memory buffer to store the Memory Map Table of each IIO Stack, and return the Memory Map Table address
  along with the number of Memory Map Records to the caller; also need to record the PCI BAR allocation information which will be used
  when clean the assigned MMIO resource by this library.

  @param[in]  Segment                      The segment number of this IIO.
  @param[in]  StartBusNo                   The start bus number of this IIO Stack.
  @param[in]  EndBusNo                     The end bus number of this IIO Stack.
  @param[in]  StartMmioAddr                The start address of this IIO Stack's MMIO resource.
  @param[in]  EndMmioAddr                  The end address of this IIO Stack's MMIO resource.
  @param[out] MemoryMapTable               A pointer to a pointer to the Buffer that contains the Memory Map Table.
  @param[out] NumberOfMemoryMapRecords     A pointer to the number of Memory Map Records.

  @retval  EFI_SUCCESS           This function returned successfully.
  @retval  EFI_OUT_OF_RESOURCES  This IIO Stack's MMIO is out of resources.
  @retval  EFI_INVALID_PARAMETER There is invalid input parameter detected.

**/
EFI_STATUS
EFIAPI
AssignMmio32_PciBusRange (
  IN      UINT16             Segment,
  IN      UINT8              StartBusNo,
  IN      UINT8              EndBusNo,
  IN      UINT32             StartMmioAddr,
  IN      UINT32             EndMmioAddr,
  OUT     MMIO_MAP_INFO      **MemoryMapTable,
  OUT     UINT16             *NumberOfMemoryMapRecords
  );

/**
  This API function is called to clean the assigned MMIO resource by this library.
**/
VOID
EFIAPI
CleanAssignedTempMmio (
  VOID
  );

#endif  //__PEI_PCI_MMIO32_ASSIGN_LIB__

