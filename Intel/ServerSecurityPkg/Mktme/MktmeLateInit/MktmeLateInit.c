/** @file
MKTME late initialization flow

@copyright
INTEL CONFIDENTIAL
Copyright 1999 - 2020 Intel Corporation. <BR>

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

#include <MktmeLateInit.h>
#include <Library/DxeServicesTableLib.h>
#define EFI_MEMORY_CPU_CRYPTO    0x0000000000080000ULL // this should be moved to MdePkg when UEFI Spec 2.8 has been used

/**
  Update UEFI memory map to add EFI_MEMORY_CPU_CRYPTO attributes for all system memory.

  @retval EFI_SUCCESS       Successfully update system memory map.
  @retval others            Failed to get DS memory map

**/
EFI_STATUS
UpdateMemoryMap (
  VOID
  )
{
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR *MemoryMap;
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR *MemoryDescriptor;
  UINTN                           NumberOfDescriptors;
  UINTN                           Index;
  UINT64                          Attributes;
  UINT64                          Capabilities;
  EFI_STATUS                      Status;

  MemoryMap = NULL;
  NumberOfDescriptors = 0;
  Status = gDS->GetMemorySpaceMap (&NumberOfDescriptors, &MemoryMap);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get GCD memory map space\n"));
    return Status;
  }

  for (Index = 0; Index < NumberOfDescriptors; Index++) {
    MemoryDescriptor = &MemoryMap[Index];
    if (MemoryDescriptor->GcdMemoryType == EfiGcdMemoryTypeSystemMemory) {
      Capabilities = MemoryDescriptor->Capabilities | EFI_MEMORY_CPU_CRYPTO;
      Status = gDS->SetMemorySpaceCapabilities (
        MemoryDescriptor->BaseAddress,
        MemoryDescriptor->Length,
        Capabilities
      );
      DEBUG ((DEBUG_ERROR,
        "Set Range %016lx-%016lx Cap %016lx Status = %r\n",
        MemoryDescriptor->BaseAddress,
        MemoryDescriptor->BaseAddress + MemoryDescriptor->Length - 1,
        Capabilities,
        Status
        ));

      Attributes = MemoryDescriptor->Attributes | EFI_MEMORY_CPU_CRYPTO;
      Status = gDS->SetMemorySpaceAttributes (
        MemoryDescriptor->BaseAddress,
        MemoryDescriptor->Length,
        Attributes
      );
      DEBUG ((DEBUG_ERROR,
        "Set Range %016lx-%016lx Attr %016lx Status = %r\n",
        MemoryDescriptor->BaseAddress,
        MemoryDescriptor->BaseAddress + MemoryDescriptor->Length - 1,
        Attributes,
        Status
        ));
    }
  }

  return Status;
}

#pragma pack(1)

EFI_TMEKEY                  mTmeKey;
EFI_MP_SERVICES_PROTOCOL    *mMpServices = NULL;
UINTN                       mSystemBsp;
UINT32                      mBspSocketId;
EFI_PROCESSOR_INFORMATION   *mProcInfo;
BOOLEAN                     mDoNotSaveKey = FALSE;

/**
Detect MP Services data like system BSP, Socket BSPs.

@param  None

@retval EFI_SUCCESS       Load the Mp services structures. successfully.
        mMpServices       Pointer to MP Services PPI
        mSystemBsp        Thread number of the system BSP
        mProcInfo         Pointer to first element of array of EFI_PROCESSOR_INFORMATION
**/
EFI_STATUS
MktmeMpServicesData (
  VOID
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINTN           ProcessorNum;
  UINTN           EnabledProcessorNum;
  UINTN           Index;

  DEBUG ((EFI_D_INFO, "MktmeMpServicesData called\n"));

  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  &mMpServices
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "  Failed to locate MP Services Protocol, failure exit\n"));
    return Status;
  }

  Status = mMpServices->GetNumberOfProcessors (
                          mMpServices,
                          &ProcessorNum,
                          &EnabledProcessorNum
                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "  Error detecting number of processor threads\n"));
    return Status;
  }

  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  sizeof (EFI_PROCESSOR_INFORMATION) * ProcessorNum,
                  (VOID **)&mProcInfo
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "  Error allocating mProcInfo\n"));
    return Status;
  }

  //
  // Get each processor Location info
  //
  for (Index = 0; Index < ProcessorNum; Index++) {
    Status = mMpServices->GetProcessorInfo (
                            mMpServices,
                            Index,
                            &mProcInfo[Index]
                            );
    if (EFI_ERROR (Status)) {
      gBS->FreePool (mProcInfo);
      return Status;
    }

    //
    // Assign Package BSPs
    //
    if (Index == 0) {
      mProcInfo[Index].StatusFlag |= PROCESSOR_AS_BSP_BIT; // PackageBsp
    } else {
      if (mProcInfo[Index].Location.Package != mProcInfo[Index - 1].Location.Package) {
        mProcInfo[Index].StatusFlag |= PROCESSOR_AS_BSP_BIT;
      }
    }

#ifdef DEBUG
    if (mProcInfo[Index].StatusFlag & PROCESSOR_AS_BSP_BIT) {
      DEBUG ((EFI_D_INFO, "  Thread %d is BSP of Socket %d\n", Index, mProcInfo[Index].Location.Package));
    }
#endif
  }

  return Status;
}

/**
Save the key value from MSR_TME_KEY0 and MSR_TME_KEY1 into the EFI variable
if Save Key bit is set in MSR_TME_ACTIVATE_REGISTER.

@param  None

@retval EFI_STATUS       Status of the operation
**/
EFI_STATUS
EFIAPI
TmeGetKey (VOID)
{
  MSR_TME_ACTIVATE_REGISTER TmeActivate;
  EFI_STATUS                Status = EFI_SUCCESS;
  UINTN                     Processor = 0;
  UINTN                     Thread = 0;

  Status = mMpServices->WhoAmI (
                          mMpServices,
                          &Thread
                          );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Exit if the thread is NOT package BSP
  //
  if ((mProcInfo[Thread].StatusFlag & PROCESSOR_AS_BSP_BIT) == 0) {
    return Status;
  }
  //
  // Convert from thread to socket number
  //
  Processor = mProcInfo[Thread].Location.Package;

  TmeActivate.Uint64 = AsmReadMsr64 (MSR_TME_ACTIVATE);

  if ((TmeActivate.Bits.Lock && TmeActivate.Bits.TmeEnable) && (TmeActivate.Bits.SaveKeyForStandby)) {
    mTmeKey.TmeKey[Processor].Key0 = AsmReadMsr64 (MSR_TME_KEY0);
    mTmeKey.TmeKey[Processor].Key1 = AsmReadMsr64 (MSR_TME_KEY1);
  } else {
    mDoNotSaveKey = 1;
    return Status;
  }

  return Status;
}

/**
Measure and log data for TME feature

@param  EnableTme whether Tme option is enabled or disabled in setup

@retval EFI_STATUS - status of executing TpmMeasureAndLogData
**/
EFI_STATUS
EFIAPI
MeasurementTmeState (
  UINT8 EnableTme
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  VOID       *String;
  UINT32     StringLen;
  UINT32     PcrIndex = 1;

  if (EnableTme) {
    String = FEATURE_ENABLE_TME_STRING;
    StringLen = sizeof (FEATURE_ENABLE_TME_STRING) - 1;
  } else {
    String = FEATURE_DISABLE_TME_STRING;
    StringLen = sizeof (FEATURE_DISABLE_TME_STRING) - 1;
  }

  Status = TpmMeasureAndLogData (
    PcrIndex,
    EV_PLATFORM_CONFIG_FLAGS,
    String,
    StringLen,
    String,
    StringLen
    );
  return Status;
}

/**
Measure and log data for TME feature

@param  EnableMktme whether Mktme option is enabled or disabled in setup

@retval EFI_STATUS - status of executing TpmMeasureAndLogData
**/
EFI_STATUS
EFIAPI
MeasurementMktmeState (
  UINT8 EnableMktme
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  VOID       *String;
  UINT32     StringLen;
  UINT32     PcrIndex = 1;

  if (EnableMktme) {
    String = FEATURE_ENABLE_MKTME_STRING;
    StringLen = sizeof (FEATURE_ENABLE_MKTME_STRING) - 1;
  } else {
    String = FEATURE_DISABLE_MKTME_STRING;
    StringLen = sizeof (FEATURE_DISABLE_MKTME_STRING) - 1;
  }

  Status = TpmMeasureAndLogData (
    PcrIndex,
    EV_PLATFORM_CONFIG_FLAGS,
    String,
    StringLen,
    String,
    StringLen
    );
  return Status;
}

/**
Measure and log data for specific msr

@param[in] MsrName
@param[in] MsrNameLength
@param[in] MsrContent

@retval EFI_STATUS - status of executing TpmMeasureAndLogData
**/
EFI_STATUS
EFIAPI
MsrMeasureAndLog (
  IN VOID   *MsrName,
  IN UINTN  MsrNameLength,
  IN UINT64 *MsrContent
  )
{
  EFI_STATUS           Status = EFI_SUCCESS;
  EFI_MKTME_EVENT_DATA EventLog;
  UINT32               EventLogSize;
  UINT32               PcrIndex = 1;

  if ((MsrName == NULL) || (MsrContent == NULL)){
    return EFI_INVALID_PARAMETER;
  }

  EventLog.MsrName = MsrName;
  EventLog.MsrContent = *MsrContent;
  EventLogSize = (UINT32) (sizeof (EventLog) + MsrNameLength);

  Status = TpmMeasureAndLogData
    (PcrIndex,
     EV_PLATFORM_CONFIG_FLAGS,
     (VOID*) EventLog.MsrName,
     (UINT32) MsrNameLength,
     (VOID*) &EventLog,
     EventLogSize
    );
  return Status;
}

/**
Prepare MSRs data for tpm measurement

@param[in] SecurityPolicy  - Pointer to Security Policy

@retval EFI Status
**/
EFI_STATUS
EFIAPI
MSRsMeasurement (VOID)
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_STATUS RetStatus = EFI_SUCCESS;
  UINT64     TmeActivateRegData;
  UINT64     TmeCapRegData;
  UINT64     TmeBaseRegData;
  UINT64     TmeMaskRegData;
  VOID       *MsrString = NULL;
  UINTN       MsrStringLen;

  // ONLY FOR ICX
  UINT64 TmeBaseRegMask        = 0xFFFFFFFFFFFFF000;
  UINT64 TmeMaskRegMask        = 0xFFFFFFFFFFFFF800;
  UINT64 TmeCapRegMaskIcx      = 0x0007FFFF00000001;
  UINT64 TmeActivateRegMaskIcx = 0xFFFF000F000001FF;

  // Reading MSRs
  TmeActivateRegData = AsmReadMsr64 (MSR_TME_ACTIVATE);
  TmeCapRegData      = AsmReadMsr64 (MSR_TME_CAPABILITY);
  TmeBaseRegData     = AsmReadMsr64 (MSR_TME_EXCLUDE_BASE);
  TmeMaskRegData     = AsmReadMsr64 (MSR_TME_EXCLUDE_MASK);

  // Masking out reserved bits
  TmeActivateRegData &= TmeActivateRegMaskIcx;
  TmeCapRegData      &= TmeCapRegMaskIcx;
  TmeBaseRegData     &= TmeBaseRegMask;
  TmeMaskRegData     &= TmeMaskRegMask;

  // Tme Activate
  MsrString = MSR_TME_ACTIVATE_STRING;
  MsrStringLen = sizeof (MSR_TME_ACTIVATE_STRING) - 1;


  Status = MsrMeasureAndLog (MsrString, MsrStringLen, &TmeActivateRegData);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "Measurment MSR: %s return error %r\n", MsrString, Status));
    RetStatus = Status;
  }

  // Tme Capability
  MsrString = MSR_TME_CAPABILITY_STRING;
  MsrStringLen = sizeof (MSR_TME_CAPABILITY_STRING) - 1;

  Status = MsrMeasureAndLog (MsrString, MsrStringLen, &TmeCapRegData);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "Measurment MSR: %s return error %r\n", MsrString, Status));
    RetStatus = Status;
  }

  // Tme Exclude base
  MsrString = MSR_TME_EXCLUDE_BASE_STRING;
  MsrStringLen = sizeof (MSR_TME_EXCLUDE_BASE_STRING) - 1;

  Status = MsrMeasureAndLog (MsrString, MsrStringLen, &TmeBaseRegData);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "Measurment MSR: %s return error %r\n", MsrString, Status));
    RetStatus = Status;
  }

  // Tme Exclude mask
  MsrString = MSR_TME_EXCLUDE_MASK_STRING;
  MsrStringLen = sizeof (MSR_TME_EXCLUDE_MASK_STRING) - 1;

  Status = MsrMeasureAndLog (MsrString, MsrStringLen, &TmeMaskRegData);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "Measurment MSR: %s return error %r\n", MsrString, Status));
    RetStatus = Status;
  }
  return RetStatus;
}

/**
Tpm measure and log data for mktme module

@param[in] SecurityPolicy  - Pointer to Security Policy

@retval VOID
**/
VOID
EFIAPI
MktmeModuleMeasurement (SECURITY_POLICY *SecurityPolicy)
{
  EFI_STATUS Status   = EFI_SUCCESS;

  if (SecurityPolicy == NULL) {
    DEBUG ((EFI_D_ERROR, "SecurityPolicy is null, exiting MktmeModuleMeasurement\n"));
    return;
  }

  Status = MeasurementTmeState (SecurityPolicy->EnableTme);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "MeasurementTmeState() retrun error: %r\n", Status));
  }

  Status = MeasurementMktmeState (SecurityPolicy->EnableMktme);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "MeasurementMktmeState() retrun error: %r\n", Status));
  }

  Status = MSRsMeasurement ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Measurement failed for one or more MSRs. retrun error: %r\n", Status));
  }
}
/**
Entry point for MKTME DXE driver

@param ImageHandle  - ImageHandle of the loaded driver
@param SystemTable  - Pointer to the BIOS System Table

@retval Status
@retval EFI_SUCCESS           - Installed successfully
@retval Others                - Failed to install this protocol

**/
EFI_STATUS
EFIAPI
MktmeLateInitEntryPoint (
  IN EFI_HANDLE                       ImageHandle,
  IN EFI_SYSTEM_TABLE                 *SystemTable
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  EDKII_VARIABLE_LOCK_PROTOCOL        *VariableLockProtocol;
  SECURITY_POLICY                     *SecurityPolicy;
  CHAR16                              VarName[7] = {'T','m','e','K','e','y','\0'};

  DEBUG ((EFI_D_INFO, " MktmeLateInitEntryPoint called.\n"));

  SecurityPolicy = GetSecurityPolicy ();

  if (SecurityPolicy == NULL) {
    DEBUG ((EFI_D_ERROR," GetCpuPolicy returned NULL. Exiting TME Flow.\n"));
    return EFI_D_ERROR;
  }

  /*Check if the system supports TME*/
  if (SecurityPolicy->TmeCapability == 0) {
    DEBUG ((EFI_D_INFO, " Tme is not supported by the system. Exiting TME Flow.\n"));
    return EFI_SUCCESS;
  }

  // Tpm measurement data
  MktmeModuleMeasurement (SecurityPolicy);

  /*Check if TME is enabled in BIOS setup options*/
  if (!SecurityPolicy->EnableTme) {
    DEBUG ((EFI_D_INFO, " Tme is not enabled in BIOS settings. Exiting TME Flow.\n"));
    return EFI_SUCCESS;
  }

  Status = gBS->LocateProtocol(
    &gEfiMpServiceProtocolGuid,
    NULL,
    &mMpServices
    );
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "  Failed to locate MP Services Protocol, failure exit\n"));
    return Status;
  }

  Status = MktmeMpServicesData();
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "  Error setting up MP Services, failure exit\n"));
    return Status;
  }

  /*Update UEFI memory to add EFI_MEMORY_CPU_CRYPTO*/
  Status = UpdateMemoryMap ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, " Failed to update memory map.\n"));
    return Status;
  }

  /*Get the key saved in TME Activate MSR */
  Status = TmeGetKey ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "  Error status returned from TmeGetKey\n"));
    return Status;
  }
  Status = mMpServices->StartupAllAPs (
                          mMpServices,
                          (EFI_AP_PROCEDURE) TmeGetKey,
                          FALSE,
                          NULL,
                          0,
                          NULL,
                          NULL
                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "  Error status returned from TmeGetKey\n"));
    return Status;
  }

  if (mDoNotSaveKey) {
    DEBUG ((EFI_D_INFO, "  Do Not save the key is set. Exiting\n"));
    /*Do not save the key to flash*/
    return Status;
  }

  /*Save the key to flash*/
  Status = gRT->SetVariable (
                  (CHAR16 *)&VarName,
                  &gEfiMktmekeyGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                  sizeof(mTmeKey),
                  (VOID *)&mTmeKey
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "  Error setting the EFI variable\n"));
    return Status;
  }

  Status = gBS->LocateProtocol (&gEdkiiVariableLockProtocolGuid, NULL, (VOID **)&VariableLockProtocol);
  if (!EFI_ERROR (Status)) {
      //APTIOV_SERVER_OVERRIDE_RC_START: PHYSICAL_PRESENCE_FLAGS_VARIABLE is in SecurityPkg
      /*
    Status = VariableLockProtocol->RequestToLock (
                                     VariableLockProtocol,
                                     (CHAR16 *)&VarName,
                                     &gEfiMktmekeyGuid
                                     );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, " Error when lock variable mTmeKey %s, Status = %r\n", PHYSICAL_PRESENCE_FLAGS_VARIABLE, Status));
      ASSERT_EFI_ERROR (Status);
    }
    */
      //APTIOV_SERVER_OVERRIDE_RC_END: PHYSICAL_PRESENCE_FLAGS_VARIABLE is in SecurityPkg
  }

  DEBUG((EFI_D_INFO, " MktmeLateInitEntryPoint exit\n"));

  return Status;
}
