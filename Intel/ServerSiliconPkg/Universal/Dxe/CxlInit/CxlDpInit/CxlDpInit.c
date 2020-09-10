/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include "CxlDpInit.h"

EFI_IIO_UDS_PROTOCOL        *mIioUds;
UINT8                       mMaxStack;


/**
  Validates "CxlStack" attributes before its consumption. The ultimate aim of this function
  is to avoid any kind of data corruption or out-of-boundary access, if any of the attribute
  value is not valid.
  This function has its own limitation. Not all non-zero and non-f's value in Rcrb are always
  valid. This function makes fair assumesumption in this respect.

 @param[in] CxlStack    CxlStack structure contains attributes of CXL stack to be configured
                                .SocketId - Socket number
                                .StackId  - Stack number
                                .RcrbBase - Rcrb Mmio base address for CXL stack

 @retval    Status      EFI_SUCCESS - All CxlStack attributes are appears to be valid
                        EFI_INVALID_PARAMETER - one of CxlStack attribute is not valid
 **/
EFI_STATUS
ValidateCxlStackAttributes (
  IN  CXL_STACK CxlStack
)
{
  if ((CxlStack.SocketId < mIioUds->IioUdsPtr->PlatformData.numofIIO) &&
      (CxlStack.StackId < mMaxStack) &&
      (CxlStack.RcrbBase != 0) &&
      (CxlStack.RcrbBase != MAX_UINTN)) {
        return EFI_SUCCESS;
      }

  DEBUG ((DEBUG_ERROR, "Invalid CxlStack attributes:\n"));
  DEBUG ((DEBUG_ERROR, "Socket :\n", CxlStack.SocketId));
  DEBUG ((DEBUG_ERROR, "Stack :\n", CxlStack.StackId));
  DEBUG ((DEBUG_ERROR, "Rcrb Base:\n", CxlStack.RcrbBase));
  return EFI_INVALID_PARAMETER;
}


/**
  Reads IoBase and IoLimit values of "CxlStack" from UDS data structure.
  Manipuate IoBase and IoLimit values to match IoBase/IoLimit register
  orientation. And then, writes the value to PCI Io Base Limit register.
  Finally enable IO space in PCI command register

 @param[in] CxlStack    CxlStack structure contains attributes of CXL stack to be configured
                                .SocketId - Socket number
                                .StackId  - Stack number
                                .RcrbBase - Rcrb Mmio base address for CXL stack

 @retval    None
 **/
VOID
ConfigureIo (
  IN  CXL_STACK CxlStack
)
{
  UINT16  IoBase;
  UINT16  IoLimit;
  PCICMD_IIO_CXL_STRUCT  PciCmd;
  EFI_STATUS Status;

  Status = ValidateCxlStackAttributes(CxlStack);
  if (EFI_ERROR(Status)) {
    return;
  }

  //
  // To CXL DP device hierarchy, IO resource range between IoBase to PciResourceIoBase - 1
  //
  IoBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[CxlStack.SocketId].StackRes[CxlStack.StackId].IoBase;
  IoLimit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[CxlStack.SocketId].StackRes[CxlStack.StackId].PciResourceIoBase - 1;

  if (IoLimit > IoBase) {
    DEBUG ((DEBUG_INFO, "   IoBase : %X", IoBase));
    DEBUG ((DEBUG_INFO, "   IoLimit : %X\n", IoLimit));
    UsraCsrWrite (CxlStack.SocketId, CxlStack.StackId, IOBASE_IIO_CXL_REG, IoBase >> 8);
    UsraCsrWrite (CxlStack.SocketId, CxlStack.StackId, IOLIMIT_IIO_CXL_REG, (IoLimit >> 8) & 0xF0);

    //
    // Enable IO Space
    //
    PciCmd.Data = (UINT16)UsraCsrRead(CxlStack.SocketId, CxlStack.StackId, PCICMD_IIO_CXL_REG);
    PciCmd.Bits.iose = 1;
    UsraCsrWrite (CxlStack.SocketId, CxlStack.StackId, PCICMD_IIO_CXL_REG, PciCmd.Data);
  }
}


/**
  RcrbBase value is programmed to mem32 decode range. the UP membar reside between
  Rcrb base and Mem32base. decode registers are 1MB granular. UP membar is assigned
  to RCRB + 256K so the closest 1 MB granular address is RCRB bar.
  Reads limit values for "CxlStack" from UDS data structure.
  Manipulate base and limit values to match Mmio Base/Mmio Limit register
  orientation. And then, writes the value to PCI Mem Base Limit register.
  Finally enable Memory space in PCI command register

 @param[in] CxlStack    CxlStack structure contains attributes of CXL stack to be configured
                                .SocketId - Socket number
                                .StackId  - Stack number
                                .RcrbBase - Rcrb Mmio base address for CXL stack

 @retval    None
 **/
VOID
ConfigureMem32 (
  IN  CXL_STACK CxlStack
)
{
  UINT32  Mem32Limit;
  PCICMD_IIO_CXL_STRUCT  PciCmd;
  EFI_STATUS Status;

  Status = ValidateCxlStackAttributes(CxlStack);
  if (EFI_ERROR(Status)) {
    return;
  }

  //
  // To CXL DP device hierarchy, Mem32 range assigned between RcrbBase to PciResourceMem32Base - 1
  //
  Mem32Limit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[CxlStack.SocketId].StackRes[CxlStack.StackId].PciResourceMem32Base - 1;

  if (Mem32Limit > CxlStack.RcrbBase) {
    DEBUG ((DEBUG_INFO, "   Rcrb Base : %X", CxlStack.RcrbBase));
    DEBUG ((DEBUG_INFO, "   Mem32Limit : %X\n", Mem32Limit));

    UsraCsrWrite (CxlStack.SocketId, CxlStack.StackId, MEMBASE_IIO_CXL_REG, (UINT32)CxlStack.RcrbBase >> 16);
    UsraCsrWrite (CxlStack.SocketId, CxlStack.StackId, MEMLIMIT_IIO_CXL_REG, Mem32Limit >> 16);

    //
    // Enable Memory Space and Bus Master
    //
    PciCmd.Data = (UINT16)UsraCsrRead(CxlStack.SocketId, CxlStack.StackId, PCICMD_IIO_CXL_REG);
    PciCmd.Bits.mse = 1;
    PciCmd.Bits.bme = 1;
    UsraCsrWrite (CxlStack.SocketId, CxlStack.StackId, PCICMD_IIO_CXL_REG, PciCmd.Data);
  }
}


/**
  Reads Mmio64 base and limit values for "CxlStack" from UDS data structure.
  Manipulate Mmio64 base and limit values to match Mmio64 Base/Mmio64 limit
  register orientation. And then, writes the value to PCI Mmio64 base and
  limit registers. Finally enable Memory space in PCI command register.

 @param[in] CxlStack    CxlStack structure contains attributes of CXL stack to be configured
                                .SocketId - Socket number
                                .StackId  - Stack number
                                .RcrbBase - Rcrb Mmio base address for CXL stack

 @retval    None
 **/
VOID
ConfigureMem64 (
  IN  CXL_STACK CxlStack
)
{
  UINT64  Mem64Base;
  UINT64  Mem64Limit;
  UINT16  Mem64LowerBase;
  UINT16  Mem64LowerLimit;
  PCICMD_IIO_CXL_STRUCT  PciCmd;
  EFI_STATUS Status;

  Status = ValidateCxlStackAttributes(CxlStack);
  if (EFI_ERROR(Status)) {
    return;
  }

  //
  // To CXL DP device hierarchy, Mem64 range assigned between Mmio64Base to PciResourceMem64Base - 1
  //
  Mem64Base = mIioUds->IioUdsPtr->PlatformData.IIO_resource[CxlStack.SocketId].StackRes[CxlStack.StackId].Mmio64Base;
  Mem64Limit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[CxlStack.SocketId].StackRes[CxlStack.StackId].PciResourceMem64Base - 1;

  if (Mem64Limit > Mem64Base) {
    Mem64LowerBase = (UINT16) (Mem64Base >> 16);
    Mem64LowerLimit =  (UINT16) ((Mem64Limit & B_PCI_BRIDGE_PMBL_PML) >> 16);
    DEBUG ((DEBUG_INFO, "   Mem64Base: %lX", Mem64Base));
    DEBUG ((DEBUG_INFO, "   Mem64Limit: %lX\n", Mem64Limit));

    UsraCsrWrite (CxlStack.SocketId, CxlStack.StackId, PFBASE_IIO_CXL_REG, Mem64LowerBase);
    UsraCsrWrite (CxlStack.SocketId, CxlStack.StackId, PFLIMIT_IIO_CXL_REG, Mem64LowerLimit);
    UsraCsrWrite (CxlStack.SocketId, CxlStack.StackId, PFBASEU_IIO_CXL_REG, (UINT32)(Mem64Base >> 32));
    UsraCsrWrite (CxlStack.SocketId, CxlStack.StackId, PFLIMITU_IIO_CXL_REG, (UINT32)(Mem64Limit >> 32));

    //
    // Enable Memory Space and Bus Master
    //
    PciCmd.Data = (UINT16)UsraCsrRead(CxlStack.SocketId, CxlStack.StackId, PCICMD_IIO_CXL_REG);
    PciCmd.Bits.mse = 1;
    PciCmd.Bits.bme = 1;
    UsraCsrWrite (CxlStack.SocketId, CxlStack.StackId, PCICMD_IIO_CXL_REG, PciCmd.Data);
  }
}


/**
  For all CXL downstream ports, configure Io, Mmio32 and Mmio64 decode ranges.
  Downstream port is PCI device. It's configuration space cannot be accessible
  via PCI config access method. Rather it's configuration space can be accessed
  using RCRB MMIO address.

 @param[in] None

 @retval    None
 **/
VOID
ConfigureDpMmioDecode (
  VOID
)
{
  UINT8                       SocketId;
  UINT8                       StackId;
  CXL_STACK                   CxlStack;

  StackId = 0;

  // For each Socket
  for (SocketId = 0; SocketId < mIioUds->IioUdsPtr->PlatformData.numofIIO; SocketId++) {
    // For each Stack
    if (!mIioUds->IioUdsPtr->PlatformData.IIO_resource[SocketId].Valid) {
      continue;
    }

    for (StackId = 0; (StackId < mMaxStack) && StackPresent(SocketId, StackId); StackId++) {
      // If Cxl enabled
      if (GetCxlStatus (SocketId, StackId) != AlreadyInCxlMode) {
        continue;
      }
      CxlStack.RcrbBase = GetRcrbBar (SocketId, StackId, TRUE);
      CxlStack.SocketId = SocketId;
      CxlStack.StackId = StackId;
      DEBUG ((DEBUG_INFO, "%X is Rcrb base of Stack %d in Socket %d\n", CxlStack.RcrbBase, StackId, SocketId));

      //
      // Program Downstream port decode range
      //
      ConfigureIo (CxlStack);
      ConfigureMem32 (CxlStack);
      ConfigureMem64 (CxlStack);
      DEBUG ((DEBUG_INFO, "Downstream port of Stack %d in Socket %d, mmio decode range has been configured\n", StackId, SocketId));
    }  //Next Stack
  }  //Next Socket
}


/**
  Initialize Cxl Downstream port

  @param ImageHandle         -  A handle for the image that is initializing this driver.
  @param SystemTable         -  A pointer to the EFI system table.

  @retval EFI_SUCCESS        -  Driver initialized successfully.
  @retval Others             -  Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
CxlDpInitEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS            Status;

  //
  // Located the IIO Protocol Interface
  //
  Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid, NULL, &mIioUds);
  mMaxStack = GetMaxStackNumPerSocket();
  ConfigureDpMmioDecode();

  return EFI_SUCCESS;
}
