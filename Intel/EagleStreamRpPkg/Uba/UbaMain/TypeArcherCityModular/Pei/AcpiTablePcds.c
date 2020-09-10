/** @file
  ACPI table pcds update.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2018 Intel Corporation. <BR>

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

#include "PeiBoardInit.h"
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Guid/PlatformInfo.h>
#include <Cpu/CpuCoreRegs.h>

EFI_STATUS
TypeArcherCityModularPlatformUpdateAcpiTablePcds (
  VOID
  )
{
  CHAR8     AcpiName10nm[]    = "PLATEPRP";     // USED for identify ACPI table in systmeboard dxe driver
  CHAR8     OemTableIdXhci[]  = "xh_nccrb";
  UINTN     Size;
  EFI_STATUS Status;

  DEBUG((DEBUG_INFO,"UBA PEI: %a\n",__FUNCTION__));


  //#
  //#ACPI items
  //#
  Size = AsciiStrSize (AcpiName10nm);
  Status = PcdSetPtrS (PcdOemSkuAcpiName , &Size, AcpiName10nm);
  DEBUG ((DEBUG_INFO, "PcdOemSkuAcpiName = %a\n", AcpiName10nm));
  ASSERT_EFI_ERROR (Status);

  Size = AsciiStrSize (OemTableIdXhci);
  Status = PcdSetPtrS (PcdOemTableIdXhci , &Size, OemTableIdXhci);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

