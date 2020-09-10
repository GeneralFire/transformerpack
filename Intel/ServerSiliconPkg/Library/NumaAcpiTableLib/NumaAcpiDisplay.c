/** @file
  This file provides display functions to dump the NUMA ACPI table data.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemTypeLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/NumaAcpiTableLib.h>
#include "NumaAcpiTableInternal.h"

/**

  This function displays the information of memory proximity domains.

**/
VOID
DisplayMemoryProximityDomain (
  VOID
  )
{
  UINT8                MemRangeId;
  UINT32               DomainId;
  UINT32               ProximityDomainNum;
  PROXIMITY_DOMAIN     *ProximityDomain;

  ProximityDomainNum = GetProximityDomainNum ();

  DEBUG ((DEBUG_INFO, "\n*******Memory Proximity Domain*******\n"));
  DEBUG ((DEBUG_INFO, "Domain  Socket  Cluster  MemType      MemoryRanges(Base-Limit)\n"));

  for (DomainId = 0; DomainId < ProximityDomainNum; DomainId++) {
    ProximityDomain = GetProximityDomain (DomainId);
    if (ProximityDomain == NULL) {
      DEBUG ((DEBUG_INFO, "Cannot get the data buffer pointer of proximity domain %d\n", DomainId));
      break;
    }

    if ((ProximityDomain->Attr & PROXIMITY_DOMAIN_MEMORY) == 0) {
      continue;
    }

    DEBUG ((DEBUG_INFO, "%-6d  %-6d  %-7d  %-12a",
      DomainId,
      ProximityDomain->SocketId,
      ProximityDomain->ClusterId,
      GetMemTypeString (ProximityDomain->MemoryDomain.MemType)
      ));

    for (MemRangeId = 0; MemRangeId < ProximityDomain->MemoryDomain.MemRangeNum; MemRangeId++) {
      DEBUG ((DEBUG_INFO, " 0x%05x-0x%05x ",
        ProximityDomain->MemoryDomain.BaseAddr[MemRangeId],
        ProximityDomain->MemoryDomain.LimitAddr[MemRangeId]
        ));
    }

    DEBUG ((DEBUG_INFO, "\n"));
  }
}

/**

  This function displays all the information of proximity domains.

**/
VOID
EFIAPI
DisplayProximityDomain (
  VOID
  )
{
  NUMA_ACPI_DATA      *NumaAcpiData;

  //
  // Check if proximity domain has been assigned.
  //
  NumaAcpiData = GetNumaAcpiDataPointer ();
  if (NumaAcpiData == NULL) {
    return;
  } else if (NumaAcpiData->ProximityDomainNum == 0) {
    FreePages (NumaAcpiData, EFI_SIZE_TO_PAGES (sizeof (NUMA_ACPI_DATA)));
    return;
  }

  //
  // Dump memory proximity domain information.
  //
  DisplayMemoryProximityDomain ();
}
