/** @file
  Implementation of CXL RAS Lib.

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

#include <Uefi.h>
#include <Library/ErrorReportLib.h>
#include <Library/PcieStdLib.h>
#include <Library/PciLib.h>
#include <Library/RasDebugLib.h>
#include <IndustryStandard/Pci.h>
#include <Library/PcieRasLib.h>
#include <Register/IioRegDef.h>
#include <Register/PcieRegDef.h>
#include <Library/RasMailBoxLib.h>
#include <Library/IioSiliconLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/MmPciExtLib.h>
#include "CxlRasLibInternal.h"
#include <PCIeErrorTypes.h>
#include <Library/SiliconWorkaroundLib.h>
#include <RasAcpi.h>
#include <Library/WheaSciLib.h>
#include <Library/RasAcpiLib.h>
#include <Library/IioTopologyLib.h>
#include <Library/RasGlobalDataLib.h>

/**
  Dump the Errors of CXL DP and UP
  @param[in] Socket               Socket Number
  @param[in] Stack                Stack  Number

  @retval none.

**/
VOID
DumpCxlDpUpRegisters (
  IN      UINT8    Socket,
  IN      UINT8    Stack
  )
{

 //
 // Currently not clear what need to here, just a place holder
 //

}

/**
  This function is to get the secondary and Subordinate bus number of CXL.DP

  @param[in] Socket               Socket Number
  @param[in] Stack                Stack  Number
  @param[in] SecondaryBus         Secondary Bus Number of CXL.DP
  @param[in] SubordinateBus      Subordinate Bus Number of CXL.DP

  @retval    None
--*/
VOID
EFIAPI
CxlRasGetCxlDevBusRange (
  IN      UINT8    Socket,
  IN      UINT8    Stack,
  OUT     UINT8   *SecondaryBus,
  OUT     UINT8   *SubordinateBus
  )
{
  UINT32  Data32;

  PCI_EXT_MM_INFO      MmInfo;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "CxlDpUpErrorHandler Start"));

  MmInfo.PciExtMmType = PCI_EXT_MM_TYPE_RCRB;
  MmInfo.PciExtMmSubType = PCI_EXT_MM_SUB_TYPE_RCRB_CXLDP;
  MmInfo.Stack = Stack;

  Data32 = MmioRead32 (MmPciAddressExt (Socket, 0, 0, 0, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, &MmInfo));

  *SecondaryBus = (UINT8) (Data32 >> 8);
  *SubordinateBus = (UINT8) (Data32 >> 16);
}

/**
  This function initializes and enables error reporting in CXL DP and UP

  @param[in] Socket               Socket Number
  @param[in] Stack                Stack  Number

  @retval    None
--*/
VOID
EFIAPI
CxlRasConfigDpUp (
  IN      UINT8    Socket,
  IN      UINT8    Stack
  )
{

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "CxlRasConfigDpUp Start"));

  PCI_EXT_MM_INFO      MmInfo;

  //
  // Config errors for CXLDP.IO
  //
  MmInfo.PciExtMmType = PCI_EXT_MM_TYPE_RCRB;
  MmInfo.PciExtMmSubType = PCI_EXT_MM_SUB_TYPE_RCRB_CXLIO_DP;
  MmInfo.Stack = Stack;
  PcieConfigDevice(Socket, 0, 0, 0, &MmInfo);

  //
  // Config errors for CXLUP.IO
  //
  MmInfo.PciExtMmType = PCI_EXT_MM_TYPE_RCRB;
  MmInfo.PciExtMmSubType = PCI_EXT_MM_SUB_TYPE_RCRB_CXLIO_UP;
  MmInfo.Stack = Stack;
  PcieConfigDevice(Socket, 0, 0, 0, &MmInfo);

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "CxlRasConfigDpUp End"));

}

/**
  This function initializes and enables error reporting in CXL DP and UP

  @param[in] Socket            Socket Number
  @param[in]  MmInfo           Memory Map information of target device.
  @param[in]  SevPerStack      Error Severity to handle

  @retval    None
--*/
VOID
EFIAPI
HandleCxlIoError (
  IN UINT8            Socket,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT8            SevPerStack
  )
{
  PcieDeviceErrorHandler (Socket, 0, 0, 0, MmInfo, SevPerStack);
}

/**
  This function initializes and enables error reporting in CXL DP and UP

  @param[in] Socket           Socket Number
  @param[in] MmInfo           Memory Map information of target device.
  @param[in] SevPerStack      Error Severity to handle

  @retval    None
--*/
VOID
EFIAPI
HandleCxlCacheMemError (
  IN UINT8            Socket,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT8            SevPerStack
  )
{

  UINT32  CorrErrSts;
  UINT32  UncErrSts;
  UINT32  CxlCacheMemRasCap;
  UINT16  CxlCacheMemAerOffset;

  CxlCacheMemRasCap = MmioRead32 (MmPciAddressExt (Socket, 0, 0, 0, CXL_CACHE_MEM_RAS_CAP_REG_OFFSET, MmInfo));
  CxlCacheMemAerOffset = CxlCacheMemRasCap >> 20;

  if (SevPerStack == COR_ERROR_TYPE) {
    CorrErrSts = MmioRead32 (MmPciAddressExt (Socket, 0, 0, 0, CxlCacheMemAerOffset + CXL_CACHE_MEM_COR_ERR_STS_REG_OFFSET, MmInfo));
    if (CorrErrSts) {
      MmioWrite32 (MmPciAddressExt (Socket, 0, 0, 0, CxlCacheMemAerOffset + CXL_CACHE_MEM_COR_ERR_STS_REG_OFFSET, MmInfo), CorrErrSts);
    }
  }

 if ((SevPerStack == NONFATAL_ERROR_TYPE) || (SevPerStack == FATAL_ERROR_TYPE)) {
   UncErrSts = MmioRead32 (MmPciAddressExt (Socket, 0, 0, 0, CxlCacheMemAerOffset + CXL_CACHE_MEM_UNC_ERR_STS_REG_OFFSET, MmInfo));
   if (UncErrSts) {
      MmioWrite32 (MmPciAddressExt (Socket, 0, 0, 0, CxlCacheMemAerOffset + CXL_CACHE_MEM_UNC_ERR_STS_REG_OFFSET, MmInfo), UncErrSts);
   }
 }
}

/**
  Compute eXpress Link error handler for DP and UP

  @param[in] Socket               Socket Number
  @param[in] Stack                Stack  Number
  @param[in] SevPerStack          Error Severity to handle

  @retval    None
--*/
VOID
EFIAPI
CxlDpUpErrorHandler (
  IN      UINT8    Socket,
  IN      UINT8    Stack,
  IN      UINT8    SevPerStack
  )
{
  PCI_EXT_MM_INFO      MmInfo;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "CxlDpUpErrorHandler Start"));

  MmInfo.PciExtMmType = PCI_EXT_MM_TYPE_RCRB;
  MmInfo.PciExtMmSubType = PCI_EXT_MM_SUB_TYPE_RCRB_CXLIO_DP;
  MmInfo.Stack = Stack;

  //
  // Handle errors for CXLDP.IO
  //
  HandleCxlIoError (Socket, &MmInfo, SevPerStack);

  MmInfo.PciExtMmType = PCI_EXT_MM_TYPE_RCRB;
  MmInfo.PciExtMmSubType = PCI_EXT_MM_SUB_TYPE_RCRB_CXLCM_DP;
  MmInfo.Stack = Stack;

  //
  // Handle errors for CXLDP.$M
  //
  HandleCxlCacheMemError (Socket, &MmInfo, SevPerStack);


  MmInfo.PciExtMmType = PCI_EXT_MM_TYPE_RCRB;
  MmInfo.PciExtMmSubType = PCI_EXT_MM_SUB_TYPE_RCRB_CXLIO_UP;
  MmInfo.Stack = Stack;

  //
  // Handle errors for CXLUP.IO
  //

  HandleCxlIoError (Socket, &MmInfo, SevPerStack);

  MmInfo.PciExtMmType = PCI_EXT_MM_TYPE_RCRB;
  MmInfo.PciExtMmSubType = PCI_EXT_MM_SUB_TYPE_RCRB_CXLCM_UP;
  MmInfo.Stack = Stack;

  //
  // Handle errors for CXLUP.$M
  //
  HandleCxlCacheMemError (Socket, &MmInfo, SevPerStack);
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "CxlDpUpErrorHandler End"));

}
