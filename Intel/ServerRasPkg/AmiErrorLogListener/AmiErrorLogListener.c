//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file AmiErrorLogListener.c
    Defined Ami error handling Listener function.

*/

//----------------------------------------------------------------------------

#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/RasDebugLib.h>
#include <Library/IoLib.h>
#include <Library/ErrorReportLib.h>
#include <Library/MpExtensionLib.h>
#include <Library/ProcessorRasLib.h>
#include <Library/PcieStdLib.h>
#include <Library/PcieRasLib.h>
#include <Library/MmPciExtLib.h>
#include <Library/PlatPolicyLib.h>
#include <Library/DebugLib.h>

#include <Guid/ErrorReportLibListener.h>
#include <Register/PcieRegDef.h>
#include <Register/ArchitecturalMsr.h>
#include <IndustryStandard/Pci30.h>
#include <RcRegs.h>
#include "AmiErrorLogListener.h"
#include <Protocol/AmiRasPolicy.h>
#include <RuntimeErrorlog/RtErrorLog.h>

#include <Library/KtiApi.h>
#include <Library/MpExtensionLib.h>
#include <Protocol/MpService.h>
#define XPUNCERRSTS_IIO_PCIE_REG 0x8E020208
#define XPCORERRSTS_IIO_PCIE_REG 0x8E020200
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

// Module global data
AMI_PLATFORM_RAS_POLICY   *gAmiRasPolicy =NULL;
RAS_POLICY                *gPolicyData = NULL;
EFI_HANDLE                gHandle = NULL;

UINT8                     McaErrOtherMcBankCount = 0;
MCA_BANK_INFO             *McaErrOtherMcBankInfo = NULL;
MCA_BANK_INFO             *OtherMcBankInfoRecord = NULL;
//----------------------------------------------------------------------------

/**
    Unlocks the MSR 0x790 (error injection lock MSR)
    
    @param  Buffer  Buffer pointing to the data.
    @return  VOID
*/
VOID
UnLockMSR790 (
  VOID *Buffer )
{
    UINT64      Data;
    
    Data = AsmReadMsr64(MC_ERR_INJ_LCK);
    Data &= ~(BIT0);
    AsmWriteMsr64 (MC_ERR_INJ_LCK, Data);
}

/**

   GetErrorValidMsrCount - Checks if MSR is valid

   @retval NONE

**/
UINT8 
GetErrorValidMsrCount (
    ) 
{
    UINT32  Ia32McgCap = (UINT32) AsmReadMsr64(MSR_IA32_MCG_CAP);
    UINT8   MsrCount = (UINT8) Ia32McgCap, ValidMsrCount = 0;
    UINT8   Count = 0;
    
    for(Count = 0; Count < MsrCount; ++Count) {
        if ((AsmReadMsr64 (MSR_IA32_MC0_STATUS + (4 * Count)) & 0x8000000000000000) != 0) {
            ValidMsrCount++;
        }
    }
    return ValidMsrCount;
}

/**

   ReadOtherMcBankInfo - BSP or AP MsrInfo

  @param OtherMcBankInfo    - Buffer to fill McBank Data
  @param OtherMcBankCount   - Count of error valid banks

  @retval VOID

**/
VOID
ReadOtherMcBankInfo (
  IN UINT32         *McBank
)
{
    EMCA_MC_SIGNATURE_ERR_RECORD    McSigErrRecord;
    UINTN                           CpuNumber;
    MP_CPU_INFO                     CpuInfo;
    
    CpuNumber = GetProcessorInfo (&CpuInfo);
    
    if ((AsmReadMsr64 (MSR_IA32_MC0_STATUS + (4 * (*McBank))) & 0x8000000000000000) != 0) {
        // MSR valid bit is TRUE. Store MSR data and increment MSR count
        ReadMcBankSigHook(*McBank, (UINT8) CpuInfo.Package, &McSigErrRecord);
        OtherMcBankInfoRecord->ApicId = McSigErrRecord.ApicId;
        OtherMcBankInfoRecord->BankNum = *McBank;
        OtherMcBankInfoRecord->BankType = GetMcBankUnitType(*McBank);
        OtherMcBankInfoRecord->BankScope = GetMcBankScope (*McBank);
        OtherMcBankInfoRecord->McaStatus = McSigErrRecord.Signature.McSts;
        OtherMcBankInfoRecord->McaAddress = McSigErrRecord.Signature.McAddr;
        OtherMcBankInfoRecord->McaMisc = McSigErrRecord.Signature.McMisc;
        //OtherMcBankInfoRecord->Valid = TRUE;
        McaErrOtherMcBankCount += 1;
        OtherMcBankInfoRecord++;
    }
}

/**

   FillOtherMcBankInfo - Fill McBank Data

  @param OtherMcBankInfo    - Buffer to fill McBank Data
  @param OtherMcBankCount   - Count of error valid banks

  @retval VOID

**/
VOID
FillOtherMcBankInfo (
)
{
    UINT32                          McBank = 0;
    UINT32                          Ia32McgCap = (UINT32) AsmReadMsr64(MSR_IA32_MCG_CAP);
    UINT8                           MsrCount = (UINT8) Ia32McgCap;
    UINT16                          McBankScope;
    
    for(McBank = 0; McBank < MsrCount; ++McBank) {
        McBankScope = GetMcBankScope (McBank);
        if ((McBankScope == MC_SCOPE_CORE) || (McBankScope == MC_SCOPE_TD)) {
            MultiProcessorExecute (OneApPerCore, ALL_PACKAGE, ALL_CORE, TRUE, ReadOtherMcBankInfo, &McBank);
        }
        else if (McBankScope == MC_SCOPE_PKG) {
            MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, TRUE, ReadOtherMcBankInfo, &McBank);
        }
    }
}

/**
  AMI Wrapper function which calls Listeners based on the elinks.

  This function calls Listeners like BMC , GPNV for Logging Errors.
  

  @param[in]  UEFI_ERROR_RECORD_HEADER        ErrorRecordHeader
  @retval Status.

**/
EFI_STATUS
AmiLogErrorWrapper (
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN EFI_GUID                 *CallerId,
  IN EFI_STATUS_CODE_DATA     *Data
  )
{
    EFI_COMMON_ERROR_RECORD_HEADER          *ErrorRecordHeader;
    EFI_ERROR_SECTION_DESCRIPTOR            *ErrorRecordDescription;
    VOID                                    *ErrorRecord;
    AMI_MEMORY_ERROR_INFO                   *AmiMemoryErrorInfo;
    AMI_PCIE_ERROR_INFO                     *AmiPcieErrorInfo;
    AMI_PROCESSOR_ERROR_INFO                *AmiProcErrorInfo;
    PCIE_ERROR_SECTION                      *PcieInfo;
    UINT8                                   CapabilitiesOffset;
    UINT16                                  Data16;

    //
    // Check whether status code is what we are interested in.
    //
    if (((CodeType & EFI_STATUS_CODE_TYPE_MASK) != EFI_ERROR_CODE) || \
        (Value != SMM_RAS_ENHANCED_ERROR_LOG) || \
        (Data == NULL)) {
      return EFI_UNSUPPORTED;
    }

    DEBUG ((LEVEL_FUNC_FLOW, "Inside AmiLogErrorWrapper() ...\n"));
    
    ErrorRecordHeader = (EFI_COMMON_ERROR_RECORD_HEADER *)((UINT8 *)Data + sizeof (EFI_STATUS_CODE_DATA));
    ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *)((UINT8 *)ErrorRecordHeader + sizeof(EFI_COMMON_ERROR_RECORD_HEADER));
    ErrorRecord = (UINT8 *)ErrorRecordDescription + sizeof(EFI_ERROR_SECTION_DESCRIPTOR);
    McaErrOtherMcBankCount = 0;
    
    if (CompareGuid (&ErrorRecordDescription->SectionType, &gEfiPlatformMemoryErrorSectionGuid)) {
        if (ErrorRecordDescription->SectionLength == sizeof (EMCA_MEMORY_ERROR_SECTION)) {
            AmiMemoryErrorInfo = (AMI_MEMORY_ERROR_INFO *)((UINT8 *)ErrorRecord + sizeof(EFI_ERROR_SECTION_DESCRIPTOR));
        }
        else if (ErrorRecordDescription->SectionLength == sizeof (EFI_PLATFORM_MEMORY_ERROR_DATA)) {
            AmiMemoryErrorInfo = (AMI_MEMORY_ERROR_INFO *)((UINT8 *)ErrorRecord + sizeof(EFI_PLATFORM_MEMORY_ERROR_DATA));
        }
        DEBUG ((LEVEL_FUNC_FLOW, "Inside AmiLogErrorWrapper() Memory Error ...\n"));
        gSmst->SmmAllocatePool (EfiRuntimeServicesData, (MAX_NUM_MCA_BANKS * sizeof (MCA_BANK_INFO)), (VOID**)&McaErrOtherMcBankInfo);
        ZeroMem ((VOID *) McaErrOtherMcBankInfo, (MAX_NUM_MCA_BANKS * sizeof (MCA_BANK_INFO)));
        OtherMcBankInfoRecord = McaErrOtherMcBankInfo;
        FillOtherMcBankInfo();
        AmiMemoryErrorInfo->OtherMcBankCount = McaErrOtherMcBankCount;
        CopyMem (AmiMemoryErrorInfo->OtherMcBankInfo, McaErrOtherMcBankInfo, McaErrOtherMcBankCount * sizeof (MCA_BANK_INFO));
        gSmst->SmmFreePool (McaErrOtherMcBankInfo);
    }
    else if (CompareGuid (&ErrorRecordDescription->SectionType, &gEfiPcieErrorSectionGuid)) {
        DEBUG ((LEVEL_FUNC_FLOW, "Inside AmiLogErrorWrapper() PCIE Error ...\n"));
        PcieInfo = (PCIE_ERROR_SECTION *)ErrorRecord;
        AmiPcieErrorInfo = (AMI_PCIE_ERROR_INFO *)((UINT8 *)ErrorRecord + sizeof(PCIE_ERROR_SECTION));
        // 
        //  Validate the Device presence by checking the Vid/Did
        //
        Data16 = MmioRead16 (MmPciAddressExt (PcieInfo->Section.DevBridge.Segment,
                PcieInfo->Section.DevBridge.PrimaryOrDeviceBus,
                PcieInfo->Section.DevBridge.Device,
                PcieInfo->Section.DevBridge.Function,
                PCI_VENDOR_ID_OFFSET,
                NULL));
        if (Data16 == 0xFFFF)
            return EFI_SUCCESS;

        AmiPcieErrorInfo->PrimaryStatus = MmioRead16 (MmPciAddressExt (PcieInfo->Section.DevBridge.Segment,
                PcieInfo->Section.DevBridge.PrimaryOrDeviceBus,
                PcieInfo->Section.DevBridge.Device,
                PcieInfo->Section.DevBridge.Function,
                PCI_PRIMARY_STATUS_OFFSET,
                NULL));
        //
        // Read capability offset, CE and UCE status
        //
        CapabilitiesOffset = PcieGetCapabilitiesOffset ((UINT8)PcieInfo->Section.DevBridge.Segment,
                PcieInfo->Section.DevBridge.PrimaryOrDeviceBus,
                PcieInfo->Section.DevBridge.Device,
                PcieInfo->Section.DevBridge.Function,
                NULL,
                EFI_PCI_CAPABILITY_ID_PCIEXP);
        if (CapabilitiesOffset != 0) {
            //
            // Read Additional PCIe Error Data
            //
            AmiPcieErrorInfo->PcieDeviceStatus = MmioRead16 (MmPciAddressExt (PcieInfo->Section.DevBridge.Segment,
                    PcieInfo->Section.DevBridge.PrimaryOrDeviceBus,
                    PcieInfo->Section.DevBridge.Device,
                    PcieInfo->Section.DevBridge.Function,
                    CapabilitiesOffset + PCIE_DEVICE_STATUS_OFFSET,
                    NULL));

            Data16 = MmioRead16 (MmPciAddressExt (PcieInfo->Section.DevBridge.Segment,
                    PcieInfo->Section.DevBridge.PrimaryOrDeviceBus,
                    PcieInfo->Section.DevBridge.Device,
                    PcieInfo->Section.DevBridge.Function,
                    CapabilitiesOffset + PCIE_CAPABILITY_OFFSET,
                    NULL));
            
            if ((Data16 & 0x00F0) == V_PCIE_CAPABILITY_DPT_ROOT_PORT) {
                AmiPcieErrorInfo->XpCorErrSts = MmioRead32 (MmPciAddressExt (PcieInfo->Section.DevBridge.Segment,
                        PcieInfo->Section.DevBridge.PrimaryOrDeviceBus,
                        PcieInfo->Section.DevBridge.Device,
                        PcieInfo->Section.DevBridge.Function,
                        ONLY_REGISTER_OFFSET (XPCORERRSTS_IIO_PCIE_REG),
                        NULL));
                AmiPcieErrorInfo->XpUncErrSts = MmioRead32 (MmPciAddressExt (PcieInfo->Section.DevBridge.Segment,
                        PcieInfo->Section.DevBridge.PrimaryOrDeviceBus,
                        PcieInfo->Section.DevBridge.Device,
                        PcieInfo->Section.DevBridge.Function,
                        ONLY_REGISTER_OFFSET (XPUNCERRSTS_IIO_PCIE_REG),
                        NULL));
            }
        }
    }
    else if (CompareGuid (&ErrorRecordDescription->SectionType, &gEfiProcessorGenericErrorSectionGuid)) {
        DEBUG ((LEVEL_FUNC_FLOW, "Inside AmiLogErrorWrapper() Processor Error ...\n"));
        AmiProcErrorInfo  = (AMI_PROCESSOR_ERROR_INFO *)((UINT8 *)ErrorRecord + sizeof(EMCA_PROCESSOR_ERROR_SECTION));
        gSmst->SmmAllocatePool (EfiRuntimeServicesData, (MAX_NUM_MCA_BANKS * sizeof (MCA_BANK_INFO)), (VOID**)&McaErrOtherMcBankInfo);
        ZeroMem ((VOID *) McaErrOtherMcBankInfo, (MAX_NUM_MCA_BANKS * sizeof (MCA_BANK_INFO)));
        OtherMcBankInfoRecord = McaErrOtherMcBankInfo;
        FillOtherMcBankInfo();
        AmiProcErrorInfo->OtherMcBankCount = McaErrOtherMcBankCount;
        CopyMem (AmiProcErrorInfo->OtherMcBankInfo, McaErrOtherMcBankInfo, McaErrOtherMcBankCount * sizeof (MCA_BANK_INFO));
        gSmst->SmmFreePool (McaErrOtherMcBankInfo);
    }
    
    //
    // Oem Hook to Collect Error Information before logging the Error
    //
    OemCollectErrors(ErrorRecordHeader);
    
    //
    // Call AMI Error Listener for BMC SEL or GPNV Error Logging 
    //
    CommonErrorHandling(ErrorRecordHeader, TRUE);
    
    return EFI_SUCCESS;
}

/**
  Entry point for the AMI Error Handler initialization. 

  This function initializes AMI Error handling.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @return EFI_STATUS.

**/
EFI_STATUS
InitializeAmiErrorLogListener (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
    EFI_STATUS  Status;
    
    DEBUG ((LEVEL_FUNC_FLOW, "Inside AmiErrorLogListener EntryPoint\n"));
    
    gPolicyData = GetRasPolicy();

    //
	// Added support to Unlock MSR 0x790 for Error Injection Tools to work.
	//
    if (gPolicyData->AmiRasPolicy.UnlockMsr) {
        MultiProcessorExecute (AllAps, ALL_PACKAGE, ALL_CORE, TRUE, UnLockMSR790, NULL);
    } 
    
	//
	// Register AmiLogErrorWrapper Listener for BMC SEL or Gpnv Error Logging
	//
    Status = RegisterErrorLogListener (AmiLogErrorWrapper);
    ASSERT_EFI_ERROR (Status);
    
    Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof(AMI_PLATFORM_RAS_POLICY), (VOID **)&gAmiRasPolicy);
    if (EFI_ERROR (Status)) {
         DEBUG ((LEVEL_FUNC_FLOW, "%a: Memory Allocation for gAmiPlatformRasPolicyProtocolGuid failed, Status =%r\n", __FUNCTION__, Status));
         ASSERT_EFI_ERROR (Status);
         return Status;
           }

    gAmiRasPolicy->RasPolicy = gPolicyData;
    gAmiRasPolicy->IioTopology = GetIioTopology ();
    gAmiRasPolicy->MemTopology = GetMemTopology ();
    
     Status = gSmst->SmmInstallProtocolInterface (
                      &gHandle,
                      &gAmiPlatformRasPolicyProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      gAmiRasPolicy
                      );
    
    
    if (EFI_ERROR (Status)) {
        DEBUG ((LEVEL_FUNC_FLOW, "%a: Install gAmiPlatformRasPolicyProtocolGuid failed, Status =%r\n", __FUNCTION__, Status));
        gSmst->SmmFreePool(gAmiRasPolicy);
        ASSERT_EFI_ERROR (Status);
        return Status;
    }

    return Status;
}
