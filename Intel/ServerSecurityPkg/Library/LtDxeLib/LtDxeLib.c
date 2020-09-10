/** @file
  This has platform-specific code for LT.  It contains hooks used by
  ServerCommon code that must be run during DXE for enabling/disabling/examining
  LT/TXT functions and status.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2019 Intel Corporation. <BR>

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

#include <PiDxe.h>
#include <Platform.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/SetupLib.h>
#include <Guid/SocketVariable.h>
#include <Guid/SocketProcessorCoreVariable.h>
#include <Protocol/Spi.h>
#include <Protocol/CpuCsrAccess.h>
#include <Universal/GetSec/Dxe/TxtDxeLib.h>
#include <Platform/Dxe/PlatformEarlyDxe/PlatformEarlyDxe.h>
#include <Guid/DebugMask.h>
#include <Register/PchRegsPmc.h>
#include <Library/PlatformLtDxeLib.h>
#include <Guid/PlatformInfo.h>
#include <RcRegs.h>
#include <Library/KtiApi.h>
#include <Library/DxeServicesLib.h>

// APTIOV_SERVER_OVERRIDE_RC_START : Changes done to load SINIT binary from FV_MAIN, to avoid separate FD AREA
EFI_GUID gAmiLtsxSinitGuid          = AMI_LTSX_SINIT_GUID;
// APTIOV_SERVER_OVERRIDE_RC_END : Changes done to load SINIT binary from FV_MAIN, to avoid separate FD AREA

EFI_GUID gSinitAcmBaseGuid = {0xBAC1001B, 0xECCC, 0x40c6, {0x99, 0x0D, 0xE8, 0xC1, 0x9A, 0x8E, 0x47, 0x7A}};

// Local macros
//
#define LT_SINIT_BASE_REG_OFF       0x270
#define LT_SINIT_SIZE_REG_OFF       0x278
#define LT_HEAP_BASE_REG_OFF        0x300
#define LT_HEAP_SIZE_REG_OFF        0x308

// External variables
//
extern  EFI_RUNTIME_SERVICES        *gRT;

// External PROCs
//
UINT8
EFIAPI
IoRead8 (
  IN UINTN   Port
  );

UINT8
EFIAPI
IoWrite8 (
  IN UINTN   Port,
  IN UINT8   Value
  );

// Helper PROCs for 10nmServerPlatformPkg/Universal/GetSec
//
/**

  Returns FLASH_REGION_SINIT_ACM_BASE to non-platform code

  @param None

  @retval PCD for FLASH_REGION_SINIT_ACM_BASE

**/
UINTN
GetSinitAddress (
  VOID
  )
{
  UINTN      SinitAddress;
  UINTN      SinitSize;
  EFI_STATUS Status;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  SinitAddress = 0;
  Status = GetSectionFromAnyFv (
            &gSinitAcmBaseGuid,
            EFI_SECTION_RAW,
            0,
            (VOID **)&(SinitAddress),
            &SinitSize
           );
  if (EFI_ERROR (Status) || (SinitAddress == 0)) {
    DEBUG ((EFI_D_ERROR, "\tGet Sinit ACM from FV failed\n"));
    return 0;
  }

  DEBUG ((EFI_D_INFO, "\tSinit ACM base address: 0x%08x\n", SinitAddress));
  DEBUG ((EFI_D_INFO, "\tSinit ACM first data: 0x%08x\n", *((UINT32 *)SinitAddress)));
  DEBUG ((EFI_D_INFO, "\tSinit ACM 2nd data: 0x%08x\n", *((UINT32 *)(SinitAddress + 4))));
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return SinitAddress;
}

EFI_STATUS
GetBiosAcmErrorReset (
  UINT8 *BiosAcmErrorResetBit
  )
{
  EFI_STATUS                Status;
  UINTN                     SysCfgSize;
  SOCKET_CONFIGURATION      SocketData;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  SysCfgSize = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);
  Status = gRT->GetVariable (
                SOCKET_PROCESSORCORE_CONFIGURATION_NAME,
                &gEfiSocketProcessorCoreVarGuid,
                NULL,
                &SysCfgSize,
                &(SocketData.SocketProcessorCoreConfiguration)
                );

  if (Status == EFI_SUCCESS) {
    *BiosAcmErrorResetBit = SocketData.SocketProcessorCoreConfiguration.BiosAcmErrorReset;
  }
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return (Status);
}

EFI_STATUS
GetAcmType (
  UINT8 *AcmTypeValue
  )
{
  EFI_STATUS                Status;
  UINTN                     SysCfgSize;
  SOCKET_CONFIGURATION      SocketData;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  SysCfgSize = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);
  Status = gRT->GetVariable (
                SOCKET_PROCESSORCORE_CONFIGURATION_NAME,
                &gEfiSocketProcessorCoreVarGuid,
                NULL,
                &SysCfgSize,
                &(SocketData.SocketProcessorCoreConfiguration)
                );

  if (Status == EFI_SUCCESS) {
    *AcmTypeValue = SocketData.SocketProcessorCoreConfiguration.AcmType;
  }
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return (Status);
}

EFI_STATUS
SetAcmType (
  IN UINT8 AcmTypeValue
  )
{
  EFI_STATUS                Status;
  UINTN                     SysCfgSize;
  SOCKET_CONFIGURATION      SocketData;
  // APTIOV_SERVER_OVERRIDE_RC_START : Remove NVRAM RT Attribute
  UINT32                    Attributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS;
  // APTIOV_SERVER_OVERRIDE_RC_END : Remove NVRAM RT Attribute

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  SysCfgSize = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);
  Status = gRT->GetVariable (
                SOCKET_PROCESSORCORE_CONFIGURATION_NAME,
                &gEfiSocketProcessorCoreVarGuid,
                // APTIOV_SERVER_OVERRIDE_RC_START : Remove NVRAM RT Attribute
                &Attributes,
                // APTIOV_SERVER_OVERRIDE_RC_END : Remove NVRAM RT Attribute
                &SysCfgSize,
                &(SocketData.SocketProcessorCoreConfiguration)
                );

  SocketData.SocketProcessorCoreConfiguration.AcmType = AcmTypeValue;

  Status = gRT->SetVariable (
                SOCKET_PROCESSORCORE_CONFIGURATION_NAME,
                &gEfiSocketProcessorCoreVarGuid,
                // APTIOV_SERVER_OVERRIDE_RC_START : Remove NVRAM RT Attribute
                Attributes,
                // APTIOV_SERVER_OVERRIDE_RC_END : Remove NVRAM RT Attribute
                SysCfgSize,
                &(SocketData.SocketProcessorCoreConfiguration)
                );
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return (Status);
}

EFI_STATUS
SetDisableLtsx (
  VOID
  )
{
  EFI_STATUS                Status;
  UINTN                     SysCfgSize;
  SOCKET_CONFIGURATION      SocketData;
  UINT8                     Data8 = 1;
  // APTIOV_SERVER_OVERRIDE_RC_START : Remove NVRAM RT Attribute
  UINT32                    Attributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS;
  // APTIOV_SERVER_OVERRIDE_RC_END : Remove NVRAM RT Attribute

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  //
  // Write NV flag so that TXT errors can be handled in next boot.
  //
  gRT->SetVariable (
         L"AcmError",
         &gEfiGenericVariableGuid,
         EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
         sizeof(UINT8),
         &Data8
         );

  SysCfgSize = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);
  Status = gRT->GetVariable (
                SOCKET_PROCESSORCORE_CONFIGURATION_NAME,
                &gEfiSocketProcessorCoreVarGuid,
                // APTIOV_SERVER_OVERRIDE_RC_START : Remove NVRAM RT Attribute
                &Attributes,
                // APTIOV_SERVER_OVERRIDE_RC_END : Remove NVRAM RT Attribute
                &SysCfgSize,
                &(SocketData.SocketProcessorCoreConfiguration)
                );

  SocketData.SocketProcessorCoreConfiguration.ProcessorLtsxEnable = 0;
  SocketData.SocketProcessorCoreConfiguration.ProcessorSmxEnable = 0;
  //Keep VT and LockChipset unchanged to avoid disabling them unexpectedly.
  //SocketData.SocketProcessorCoreConfiguration.ProcessorVmxEnable = 0;
  //SocketData.SocketProcessorCoreConfiguration.LockChipset = 0;

  Status = gRT->SetVariable (
                SOCKET_PROCESSORCORE_CONFIGURATION_NAME,
                &gEfiSocketProcessorCoreVarGuid,
                // APTIOV_SERVER_OVERRIDE_RC_START : Remove NVRAM RT Attribute
                Attributes,
                // APTIOV_SERVER_OVERRIDE_RC_END : Remove NVRAM RT Attribute
                SysCfgSize,
                &(SocketData.SocketProcessorCoreConfiguration)
                );

  PlatformHookForDisableLtsx ();
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return (Status);
}

VOID
SetDisableSoftwareSmi (
  VOID
  )
{
  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  UINT8 Buffer8 = IoRead8 (PCH_ACPI_BASE_ADDRESS + R_ACPI_IO_SMI_EN) & ~(B_ACPI_IO_SMI_EN_SWSMI_TMR);
  IoWrite8 (PCH_ACPI_BASE_ADDRESS + R_ACPI_IO_SMI_EN, Buffer8);
  DEBUG ((EFI_D_INFO, "\tR_ACPI_IO_SMI_EN[0x%08x] = 0x%08x\n",PCH_ACPI_BASE_ADDRESS+R_ACPI_IO_SMI_EN,IoRead8 (PCH_ACPI_BASE_ADDRESS + R_ACPI_IO_SMI_EN)));
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));
}
// APTIOV_SERVER_OVERRIDE_RC_START : Changes done to load SINIT binary from FV_MAIN, to avoid separate FD AREA
//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   GetRawImage
//
// Description: Loads binary from RAW section of main firwmare volume
//
//
// Input:       IN     EFI_GUID   *NameGuid,
//              IN OUT VOID   **Buffer,
//              IN OUT UINTN  *Size
//
// Output:      EFI STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS GetRawImage(
    IN EFI_GUID  *NameGuid,
    IN OUT VOID  **Buffer,
    IN OUT UINTN *Size  )
{
    EFI_STATUS                   Status;
    EFI_HANDLE                   *HandleBuffer = 0;
    UINTN                        HandleCount   = 0;
    UINTN                        i;
    EFI_FIRMWARE_VOLUME2_PROTOCOL *Fv;
    UINT32                       AuthenticationStatus;
    Status = gBS->LocateHandleBuffer(
        ByProtocol,
        &gEfiFirmwareVolume2ProtocolGuid,
        NULL,
        &HandleCount,
        &HandleBuffer
        );
    if ( EFI_ERROR( Status ) || HandleCount == 0 ) {
        return EFI_NOT_FOUND;
    }

    //
    // Find desired image in all Fvs
    //
    for ( i = 0; i < HandleCount; i++ ) {
        Status = gBS->HandleProtocol(
            HandleBuffer[i],
            &gEfiFirmwareVolume2ProtocolGuid,
            &Fv
            );

        if ( EFI_ERROR( Status )) {
            gBS->FreePool( HandleBuffer );
            return EFI_LOAD_ERROR;
        } // if EFI_ERROR

        //
        // Try a raw file
        //
        *Buffer = NULL;
        *Size   = 0;
        Status  = Fv->ReadSection(
            Fv,
            NameGuid,
            EFI_SECTION_RAW,
            0,
            Buffer,
            Size,
            &AuthenticationStatus
            );

        if ( !EFI_ERROR( Status )) {
            break;
        } // if EFI_ERROR
    } // for i
    gBS->FreePool( HandleBuffer );

    if ( i >= HandleCount ) {
        return EFI_NOT_FOUND;
    }

    return EFI_SUCCESS;
}
// APTIOV_SERVER_OVERRIDE_RC_END : Changes done to load SINIT binary from FV_MAIN, to avoid separate FD AREA

EFI_STATUS
SetupLtDeviceMemory (
  IN LT_DXE_LIB_CONTEXT *LtDxeCtx
  )
{
  EFI_STATUS                          Status;
  volatile UINT64                     *Ptr64;
  UINT64                              Value64;
  UINT32                              Value32;
  UINT32                              Temp32;
  EFI_PHYSICAL_ADDRESS                Addr;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL     *PciRootBridge;
  UINT64                              PciAddress;
  UINT8                               Socket;
  UINT8                               Iio;
  UINT8                               Size = 4;
  EFI_PLATFORM_INFO                   *PlatformInfoHob;
  EFI_HOB_GUID_TYPE                   *GuidHob;
#if 1 //SINIT_IN_FLASH
// APTIOV_SERVER_OVERRIDE_RC_START
//  UINTN   SinitAddress;
  UINT8*  SinitAddress;
// APTIOV_SERVER_OVERRIDE_RC_END
  UINTN   SinitSize;
#endif
  EFI_CPU_CSR_ACCESS_PROTOCOL       *CpuCsrAccess;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  //
  // Locate Platform Info HOB
  //
  GuidHob = GetFirstGuidHob(&gEfiPlatformInfoGuid);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  PlatformInfoHob = GET_GUID_HOB_DATA(GuidHob);
  if (PlatformInfoHob == NULL) {
    return EFI_NOT_FOUND;
  }
  //
  // Check whether we need to allocate memory
  //
  if ((LtDxeCtx->PlatformTxtDeviceMemory->SINITMemoryAddress == 0) ||
      (LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemoryAddress == 0)) {
    //
    // We do not need support the case that SINIT/TXT heap unallocated, because they must be in DPR region.
    //
    return EFI_UNSUPPORTED;
  }

  Status = gBS->LocateProtocol (
                &gEfiPciRootBridgeIoProtocolGuid,
                NULL,
                &PciRootBridge
                );
  ASSERT_EFI_ERROR (Status);

  //
  // Program LT Device Memory Chipset Registers and record them in
  // BootScript so they will be saved and restored on S3
  //

  //
  // NoDMA Registers
  // We do not need NoDMA support, which is deprecated by VTd
  //

  //
  // SINIT Registers
  //
  Ptr64       = (UINT64 *)(UINTN)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_SINIT_BASE_REG_OFF);
  Value64     = (UINT64)LtDxeCtx->PlatformTxtDeviceMemory->SINITMemoryAddress;
  *Ptr64      = Value64;
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINT64)(UINTN)(Ptr64),
    2,
    &Value64
    );
  DEBUG ((EFI_D_INFO, "\tLT_SINIT_BASE_REG_OFF[0x%08x] = 0x%016lx\n",LT_PUBLIC_SPACE_BASE_ADDRESS + LT_SINIT_BASE_REG_OFF,Value64));

  Ptr64       = (UINT64 *)(UINTN)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_SINIT_SIZE_REG_OFF);
  Value64     = (UINT64)LtDxeCtx->PlatformTxtDeviceMemory->SINITMemorySize;
  *Ptr64      = Value64;
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINT64)(UINTN)(Ptr64),
    2,
    &Value64
    );
  DEBUG ((EFI_D_INFO, "\tLT_SINIT_SIZE_REG_OFF[0x%08x] = 0x%016lx\n",LT_PUBLIC_SPACE_BASE_ADDRESS + LT_SINIT_SIZE_REG_OFF,Value64));

  ZeroMem (
    (VOID *)(UINTN)LtDxeCtx->PlatformTxtDeviceMemory->SINITMemoryAddress,
    (UINTN)LtDxeCtx->PlatformTxtDeviceMemory->SINITMemorySize
    );

  //
  // TXTHEAP Registers
  //
  Ptr64       = (UINT64 *)(UINTN)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_HEAP_BASE_REG_OFF);
  Value64     = (UINT64)LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemoryAddress;
  *Ptr64      = Value64;
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINT64)(UINTN)(Ptr64),
    2,
    &Value64
    );
  DEBUG ((EFI_D_INFO, "\tLT_HEAP_BASE_REG_OFF[0x%08x] = 0x%016lx\n",LT_PUBLIC_SPACE_BASE_ADDRESS + LT_HEAP_BASE_REG_OFF,Value64));

  Ptr64       = (UINT64 *)(UINTN)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_HEAP_SIZE_REG_OFF);
  Value64     = (UINT64)LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemorySize;
  *Ptr64      = Value64;
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINT64)(UINTN)(Ptr64),
    2,
    &Value64
    );
  DEBUG ((EFI_D_INFO, "\tLT_HEAP_SIZE_REG_OFF[0x%08x] = 0x%016lx\n",LT_PUBLIC_SPACE_BASE_ADDRESS + LT_HEAP_SIZE_REG_OFF,Value64));

  ZeroMem (
    (VOID *)(UINTN)LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemoryAddress,
    (UINTN)LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemorySize
    );

  // Read current CSR LTDPR_IIO_VTD_REG from socket0, IIO stack 0
  Status = gBS->LocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &CpuCsrAccess);
  ASSERT_EFI_ERROR (Status);

  Value32 = CpuCsrAccess->ReadCpuCsr(0, 0, LTDPR_IIO_VTD_REG);
  DEBUG ((EFI_D_INFO, "\tLTDPR_IIO_VTD_REG[0x%08x] = 0x%08x\n",LTDPR_IIO_VTD_REG,Value32));

  if (LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionAddress != 0 && ((Value32 & 0x0FFF) == 0)) {
    //
    // Loop thru all IIO stacks of all  sockets that are present
    //
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (!SocketPresent (Socket)) {
        continue;
      }
      for (Iio = 0; Iio < MAX_IIO_STACK; Iio++) {
        if (!IfStackPresent (Socket, Iio)) {
          continue;
        }
        // Read current CSR LTDPR_IIO_VTD_REG
        Value32 = CpuCsrAccess->ReadCpuCsr(Socket, Iio, LTDPR_IIO_VTD_REG);
        if ((LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionSize == 0) || ((Value32 & 0x0FF5) != 0)) {
          continue;
        }
        // Program the IIO CSR LTDPR_IIO_VTD_REG
        DEBUG ((EFI_D_INFO, "\tSocket = 0x%d\tStack = 0x%d\n",Socket,Iio));
        Temp32 = ((UINT32)(LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionSize) >> 16) | SA_DPR_LOCK | SA_DPR_PRM;
        Value32 = (Value32 & (~(0x0FF5))) | Temp32;
        CpuCsrAccess->WriteCpuCsr(Socket, Iio, LTDPR_IIO_VTD_REG, Value32);
        DEBUG ((EFI_D_INFO, "\tLTDPR_IIO_VTD_REG[0x%08x] = 0x%08x\n",LTDPR_IIO_VTD_REG,CpuCsrAccess->ReadCpuCsr(Socket, Iio, LTDPR_IIO_VTD_REG)));
        //
        // Verify register value by reading it back, and assert error if value is not right
        // need to double check DPR REG//ASSERT (((Value32 & 0xFF0) << 16) == LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionSize);
        //
        PciAddress = CpuCsrAccess->GetCpuCsrAddress(Socket, Iio, LTDPR_IIO_VTD_REG, &Size);
        DEBUG ((EFI_D_ERROR, "\tLTDPR_IIO_VTD_REG_PCI_ADDRESS[0x%08x] written with 0x%08x, ",PciAddress,Value32));
        Value32 = *(UINT32*)PciAddress;
        DEBUG ((EFI_D_ERROR, "read back as 0x%08x\n", Value32));
        if (((Value32 & 0xFF0) << 16) == LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionSize) {
          S3BootScriptSaveMemWrite (
              S3BootScriptWidthUint32,
              (UINTN)PciAddress,
              1,
              &Value32
          );
        } else {
          DEBUG ((EFI_D_ERROR, "\tIIO LTDPR CSR programming error!!!\n"));
        }
      }
    }

    //
    // Program DPR register in LT Public space on PCH
    //
    Ptr64   = (UINT64 *) (UINTN) (LT_PUBLIC_SPACE_BASE_ADDRESS + LT_DMA_PROTECTED_RANGE_REG_OFF);
    Value64 = RShiftU64 (LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionSize, 16) | SA_DPR_LOCK | SA_DPR_PRM;
    Value64 |= ((LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionAddress + LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionSize) & 0xFFF00000);
    *Ptr64 = Value64;
    //
    // Assert error if programmed value is different from requested. This
    // means error is requested size.
    //
    Value64 = *Ptr64;
    DEBUG ((EFI_D_ERROR, "\tPCH TXT Public Space DPR Reg[0x%08x] = %08x,\n", LT_PUBLIC_SPACE_BASE_ADDRESS + LT_DMA_PROTECTED_RANGE_REG_OFF, (UINT32) Value64));
    ASSERT ((LShiftU64 ((Value64 & 0xFFE), 16)) == LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionSize);

    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint64,
      (UINT64) (UINTN) (Ptr64),
      2,
      &Value64
    );

    //
    // Clear out the DMA protected memory area for use.
    //
    ZeroMem (
      (VOID *)(UINTN)LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionAddress,
      (UINTN)LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionSize
    );
  }

  //
  // Fill BiosOsDataRegion
  //
  Ptr64     = (UINT64 *)(UINTN)(LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemoryAddress + 8);

  if (LtDxeCtx->PlatformTxtPolicyData->BiosOsDataRegionRevision >= 2) {
    *(UINT64 *)(UINTN)LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemoryAddress = sizeof(EFI_TXT_BIOS_OS_DATA_REGION_V3)+ 8 ;

    ((EFI_TXT_BIOS_OS_DATA_REGION_V3 *)Ptr64)->Revision = LtDxeCtx->PlatformTxtPolicyData->BiosOsDataRegionRevision;

    //
    // Allocate Physical Memory for LCP
    //
    ((EFI_TXT_BIOS_OS_DATA_REGION_V2 *)Ptr64)->LcpPolicyDataBase = 0;
    ((EFI_TXT_BIOS_OS_DATA_REGION_V2 *)Ptr64)->LcpPolicyDataSize = 0;

    if ((LtDxeCtx->PlatformTxtPolicyData->LcpPolicyDataBase != 0) &&
        (LtDxeCtx->PlatformTxtPolicyData->LcpPolicyDataSize != 0)) {
     //LCP PD must be copied into a DMA protected region, here it will be copied to DPR region
      Addr = LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionAddress + (UINT32)LShiftU64 (PlatformInfoHob->MemData.BiosGuardMemSize, 20);
      ZeroMem (
        (VOID *)(UINTN)Addr,
        (UINTN)LtDxeCtx->PlatformTxtPolicyData->LcpPolicyDataSize
        );
      CopyMem (
        (VOID *)(UINTN)Addr,
        (VOID *)(UINTN)LtDxeCtx->PlatformTxtPolicyData->LcpPolicyDataBase,
        (UINTN)LtDxeCtx->PlatformTxtPolicyData->LcpPolicyDataSize
        );
      ((EFI_TXT_BIOS_OS_DATA_REGION_V3 *)Ptr64)->LcpPolicyDataBase = Addr;
      ((EFI_TXT_BIOS_OS_DATA_REGION_V3 *)Ptr64)->LcpPolicyDataSize = LtDxeCtx->PlatformTxtPolicyData->LcpPolicyDataSize;
    }

    ((EFI_TXT_BIOS_OS_DATA_REGION_V3 *)Ptr64)->NumberOfLogicalProcessors = (UINT32)LtDxeCtx->ApCount + 1;

    if (LtDxeCtx->PlatformTxtPolicyData->BiosOsDataRegionRevision == 3) {
      ((EFI_TXT_BIOS_OS_DATA_REGION_V3 *)Ptr64)->FlagVariable = LtDxeCtx->PlatformTxtPolicyData->FlagVariable;
    }
  }

  //
  // Set it to zero because SINIT_ACM is not loaded here.
  //
  ((EFI_TXT_BIOS_OS_DATA_REGION_V1 *)Ptr64)->BiosSinitSize = 0;

  //Below debug should = 0x2c, when version = 3
  DEBUG ((EFI_D_ERROR, "\tTXTHeapMemoryAddress = 0x%08x\n", *(UINT64 *)(UINTN)LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemoryAddress));
  DEBUG ((EFI_D_ERROR, "\tStartupAcmAddress = 0x%08x\n", (UINT8 *)(UINTN)(LtDxeCtx->PlatformTxtPolicyData->StartupAcmAddress)));
    //
    // copy SINIT-ACM to LT.SINIT.MEMORY.BASE(LT offset 0x270)
    //

   // APTIOV_SERVER_OVERRIDE_RC_START
   // SinitAddress = GetSinitAddress();
   // Changes done to load SINIT binary from FV_MAIN, to avoid separate FD AREA
      Status = GetRawImage( &gAmiLtsxSinitGuid, &SinitAddress, &SinitSize );

      if ( EFI_ERROR( Status )) {
	  DEBUG((EFI_D_ERROR, "Failed to load SINIT ACM BINARY: Status=%r\n", Status));
          return Status;
      }
    // APTIOV_SERVER_OVERRIDE_RC_END
  SinitSize  = (UINT64)LtDxeCtx->PlatformTxtDeviceMemory->SINITMemorySize;
  DEBUG ((EFI_D_ERROR, "\tCopy SINIT from Flash address: 0x%08x, Size:0x%08x ", SinitAddress,SinitSize));
  DEBUG ((EFI_D_ERROR, "to TXT Protected Memory address: 0x%08x, Size:0x%08x\n", LtDxeCtx->PlatformTxtDeviceMemory->SINITMemoryAddress,SinitSize));
  CopyMem (
    (VOID *)(UINTN)LtDxeCtx->PlatformTxtDeviceMemory->SINITMemoryAddress,
    (VOID *)SinitAddress,
    (UINTN)SinitSize
    );
  // Update the SINIT size in BiosOsData region (in the TXT HEAP)
  ((EFI_TXT_BIOS_OS_DATA_REGION_V3 *)Ptr64)->BiosSinitSize = (UINT32) SinitSize;
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return EFI_SUCCESS ;
}

//
// The 8-bit data at offset 0xF in the ACM binary that gives ACM Type information.
// It can be one of three values:
// 0x80 = debug signed ACM
// 0x40 = NPW production signed ACM
// 0x00 = PW production signed ACM
//
// APTIOV_SERVER_OVERRIDE_RC_START : Moved later to get ACM type to get BiosAcmAddress from PlatformTxtPolicyData
EFI_STATUS
FindAcmBinaryType(
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
{
#if 0
  UINT32                SinitAddress;
  UINT8                 AcmVersion;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  SinitAddress = (UINT32)GetSinitAddress ();
#endif
  
  UINT8                 AcmVersion;
  UINT8                 *SinitAddress;
  SinitAddress = (UINT8*)LtDxeCtx->PlatformTxtPolicyData->BiosAcmAddress; //(UINT8*)FV_ACM_BASE;  
// APTIOV_SERVER_OVERRIDE_RC_END : Moved later to get ACM type to get BiosAcmAddress from PlatformTxtPolicyData  

  DEBUG ((EFI_D_INFO, "\tSinit Address: 0x%08x\n", SinitAddress));
  //
  // Read 8-bit data at offset 0xF in the ACM binary that gives  ACM Type information
  //
  AcmVersion = *((UINT8 *) (UINTN) (SinitAddress + ACM_TYPE_OFFSET));
  DEBUG ((EFI_D_INFO, "\tSinit ACM Version: 0x%02x\n", AcmVersion));
  SetAcmType (AcmVersion);
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return EFI_SUCCESS;
}
