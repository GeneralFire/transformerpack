/** @file
  Implementation of CBDMA IIO RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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
#include <Library/BaseLib.h>
#include <Library/MpExtensionLib.h>
#include <Library/IioRasLib.h>
#include <Library/PcieRasLib.h>
#include <Library/MmPciExtLib.h>
#include <Library/RasDebugLib.h>
#include <IndustryStandard/Pci22.h>
#include <Library/IoLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/RasMailBoxLib.h>
#include <Library/PcieStdLib.h>
#include <Library/IioSiliconLib.h>
#include <Library/BaseMemoryLib.h>
#include <IioRegs.h>
#include <Register/IioRegDef.h>
#include <Register/PcieRegDef.h>
#include <UncoreCommonIncludes.h>
#include <IndustryStandard/PciExpress21.h>
#include <Register/ArchitecturalMsr.h>
#include <RcRegs.h>
#include <PCIeErrorTypes.h>
#include <Cpu/CpuCoreRegs.h>

 /**

     Set DMA Errors Severity

     @param None

     @retval None

 **/
VOID
EFIAPI
IioSetDmaErrorsSev(
  IN   UINT8    Skt,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function,
  IN   UINT32   Sev
  )
{

}

 /**

     Set DMA Errors Mask

     @param None

     @retval None

 **/
VOID
EFIAPI
IioSetDmaErrorsMsk (
  IN   UINT8    Skt,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function,
  IN   UINT32   Msk
  )
{

}

/**
  This function is to check the CBDAM Error, if there is error on CBDMA on the
  ErrSeverity return TRUE, else return FALSE

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param ErrSeverity The error's severity
  @retval None.

**/
BOOLEAN
EFIAPI
IsIioCbdmaError (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Function,
  IN  UINT8  ErrSeverity
  )
{
  return FALSE;
}

 /**

  Clear DMA Errors from silicon

  @param  [in]  Socket  -- socket index
  @param  [in]  Bus     -- bus index
  @param  [in]  Device  -- device index
  @param  [in]  Function-- Function index

     @retval None

 **/
VOID
EFIAPI
IioClearDmaErrorsSi (
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function
  )
{

}
