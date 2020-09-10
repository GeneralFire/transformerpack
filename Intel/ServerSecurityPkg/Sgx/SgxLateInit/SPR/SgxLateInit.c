/** @file
  SGX late initialization flow

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

#include <SysHost.h>
#include <Cpu/CpuCoreRegs.h>

#include <Register/Cpuid.h>
#include <Register/ArchitecturalMsr.h>

#include <Guid/SgxInitDataSpr.h>
#include <Guid/SocketProcessorCoreVariable.h>
#include <Guid/TdxInitData.h>

#include <Protocol/GlobalNvsArea.h>
#include <Protocol/MpService.h>
#include <Protocol/VariableLock.h>

#include <Library/KtiApi.h>
#include <Library/HiiLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/Stl2BiosLib.h>
#include <Library/CpuConfigLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/FusaSafPolicyLib.h>
#include <Library/SecurityPolicyLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/EmulationConfigurationLib.h>

extern  EFI_RUNTIME_SERVICES            *gRT;

// module global protocols
EFI_MP_SERVICES_PROTOCOL                *mMpServices                     = NULL;

SECURITY_POLICY                         *mSecurityPolicy;
UINTN                                   mSystemBsp;
UINT32                                  mBspSocketId;
UINT32                                  mMcuErrorCode [MAX_CPU_NUM];
EFI_PROCESSOR_INFORMATION               *mProcInfo;
UINT32                                  mSocketCount = 1;
SPIN_LOCK                               mPrintDebugLock;
BIOS_ACPI_PARAM                         *mAcpiParameter = NULL;
UINT16                                  mNumberOfPackagesAdded = 0;

// Internal buffers shadowing UEFI NVRAM variables
SGX_REGISTRATION_CONFIG                 mInternalRegistrationConfig;
SGX_REGISTRATION_STATUS                 mInternalRegistrationStatus;
SGX_REGISTRATION_PACKAGE_INFO           mInternalRegistrationPackageInfo;
SGX_REGISTRATION_RESPONSE               mInternalRegistrationResponse;
SGX_REGISTRATION_REQUEST                mInternalRegistrationRequest;

SGX_UEFI_REGISTRATION_PLATFORM_MANIFEST mInternalUefiPlatformManifest;
SGX_UEFIFW_REGISTRATION_STATE           *mInternalRegState;            // Pointer to non-volatile data inside SGX_INIT_DATA_HOB

SGX_INIT_DATA_HOB                       *mSgxInitDataHob;

UINT8                                   SocketKeyBlobs;

// FuSa (SAF)
STL_2_BIOS_FUSA_SAF_DATA_VARIABLE       mStl2BiosFusaSafVariable;
FUSA_SAF_POLICY                         *mFusaSafPolicy;

/**
  Prints a message to the serial port.

  @param  Format      Format string for the message to print.
  @param  ...         Variable argument list whose contents are accessed
                      based on the format string specified by Format.

**/
VOID
EFIAPI
SerialPrintBlocking (
  IN  UINTN        ErrorLevel,
  IN  CONST CHAR8  *Format,
  ...
  )
{
  CHAR8    Buffer[200];
  VA_LIST  Marker;

  //
  // If Format is NULL, then ASSERT().
  //
  ASSERT (Format != NULL);

  if (Format == NULL) {
    return;
  }

  //
  // Convert the message to an ASCII String
  //
  VA_START (Marker, Format);
  AsciiVSPrint (Buffer, sizeof (Buffer), Format, Marker);
  VA_END (Marker);

  //
  // Send the print string to a Serial Port
  //
  while (!AcquireSpinLockOrFail (&mPrintDebugLock)) {
      CpuPause ();
  }
  DebugPrint (ErrorLevel, Buffer);
  ReleaseSpinLock (&mPrintDebugLock);
}


/**

  Prints whitespaces

  @param Spaces  - number of whitespaces to print

  @retval None

**/
VOID
PrintWhitespaces (
  IN UINT32 Spaces
  )
{
  while (Spaces--) {
    SGX_DEBUG ((SGX_D_INFO, " "));
  }
}

/**

  Prints each byte of a byte array

  @param data     - Pointer byte array
  @param size     - Size of byte array

  @retval None

**/
VOID
PrintByteArrays (
  IN UINT8  *Data,
  IN UINT32 Size
  )
{
  UINT32 Index;

  for (Index = 0; Index < Size; Index++) {
    SGX_DEBUG ((SGX_D_INFO, "[%02x] ", Data[Index]));
  }
  SGX_DEBUG ((SGX_D_INFO, "\n"));
}

/**

  Prints all MCHECK KeyBlob structure filed
  @param[in]       McheckSgxKeyBlob         A pointer to the MCHECK_SGX_KEY_BLOB struct
    @retval      None

**/
VOID
PrintMcheckKeyBlob (
  IN  MCHECK_SGX_KEY_BLOB    *McheckSgxKeyBlob
)
{
  UINT32        DataSize = 0;
  UINT8         *CastPtr;
  UINT8         Index;

  SGX_DEBUG ((SGX_D_INFO, "KeyBlob:\n"
                       "\tHeader:\n"
                       "\t\tSGX UUID:\n"));
  PrintByteArrays (McheckSgxKeyBlob->Header.Uuid.Uuid, (UINT32) MCHECK_UUID_SIZE);
  SGX_DEBUG ((SGX_D_INFO, "\n\t\tCpuSvn:\n"));
  DataSize = (UINT32) sizeof (McheckSgxKeyBlob->CpuSvn);
  PrintByteArrays (&(McheckSgxKeyBlob->CpuSvn), (UINT32) sizeof (McheckSgxKeyBlob->CpuSvn));
  SGX_DEBUG ((SGX_D_INFO, "\n\t\tSecurity Properties:\n"));
  CastPtr = (UINT8 *) &(McheckSgxKeyBlob->SecurityProperties);
  PrintByteArrays (CastPtr, sizeof (UINT32));
  SGX_DEBUG ((SGX_D_INFO, "\n\t\tPrid:\n"));
  PrintByteArrays (McheckSgxKeyBlob->Prid.Uuid, MCHECK_UUID_SIZE);
  SGX_DEBUG ((SGX_D_INFO, "\n\t\tPlatformEpoch:\n"));
  PrintByteArrays (McheckSgxKeyBlob->PlatformEpoch, (sizeof (UINT8) * SGX_EPOCH_SIZE));
  SGX_DEBUG ((SGX_D_INFO, "\n\t\tRsakHash:\n"));
  PrintByteArrays (McheckSgxKeyBlob->RsakHash, (sizeof (UINT8) * SHA256_DIGEST_SIZE));
  SGX_DEBUG ((SGX_D_INFO, "\n\t\tRsakHash:\n"));
  PrintByteArrays (McheckSgxKeyBlob->RsakHash, (sizeof (UINT8) * SHA256_DIGEST_SIZE));
  SGX_DEBUG ((SGX_D_INFO, "\n\t\tPfk1:\n"));
  PrintByteArrays (McheckSgxKeyBlob->Pfk1, (sizeof (UINT8) * AES128_KEY_SIZE));
  SGX_DEBUG ((SGX_D_INFO, "\n\t\tPprResetKey:\n"));
  for (Index = 0; Index < MCHECK_MAX_KEYS; ++Index) {
    CastPtr = (UINT8 *) &(McheckSgxKeyBlob->PprResetKey[Index][0]);
    PrintByteArrays (CastPtr, sizeof (AES128_KEY_SIZE));
  }

  SGX_DEBUG ((SGX_D_INFO, "\n\t\tLastUsedSvn:\n"));
  PrintByteArrays (&(McheckSgxKeyBlob->LastUsedSvn), (sizeof (UINT8)));
  SGX_DEBUG ((SGX_D_INFO, "\n\t\tReserved2:\n"));
  PrintByteArrays (McheckSgxKeyBlob->Reserved2, (sizeof (UINT8) * 15));
  SGX_DEBUG ((SGX_D_INFO, "\n\t\tTargetPrid:\n"));
  PrintByteArrays (McheckSgxKeyBlob->TargetPrid[0].Uuid, MCHECK_UUID_SIZE);
  SGX_DEBUG ((SGX_D_INFO, "\n\t\tMasterComms:\n"));
  PrintByteArrays (McheckSgxKeyBlob->MasterComms[0].Uuid, MCHECK_UUID_SIZE);
  SGX_DEBUG ((SGX_D_INFO, "\n\t\tIv:\n"));
  for (Index = 0; Index < MCHECK_MAX_KEYS; ++Index) {
    CastPtr = (UINT8 *) &(McheckSgxKeyBlob->Iv[Index][0]);
    PrintByteArrays (CastPtr, 12);
  }
  SGX_DEBUG ((SGX_D_INFO, "\n\t\tMac:\n"));
  for (Index = 0; Index < MCHECK_MAX_KEYS; ++Index) {
    CastPtr = (UINT8 *) &(McheckSgxKeyBlob->Mac[Index][0]);
    PrintByteArrays (CastPtr, 16);
  }
  SGX_DEBUG ((SGX_D_INFO, "\n\t\tIvComm:\n"));
  PrintByteArrays (McheckSgxKeyBlob->IvComm, (sizeof (UINT8) * 12));
  SGX_DEBUG ((SGX_D_INFO, "\n\t\tMacComm:\n"));
  PrintByteArrays (McheckSgxKeyBlob->MacComm, (sizeof (UINT8) * 16));
}

/**

  Prints all UEFI FW KeyBlob structure filed
  @param[in]       SgxUefiFwKeyBlob        A pointer to the SGX_UEFI_KEY_BLOB struct
    @retval      None

**/
VOID
PrintUefiFwKeyBlob(
  SGX_UEFIFW_KEY_BLOB             *SgxUefiFwKeyBlob
)
{
  SGX_DEBUG ((SGX_D_INFO, "Present: %s\n", SgxUefiFwKeyBlob->Present ? "TRUE" : "FALSE"));
  SGX_DEBUG ((SGX_D_INFO, "TimeStamp: %d-%d-%d, %d:%d:%d:%d, Pad1: %d, Pad2: %d, TimeZone: %d, Daylight: %d\n",
                      SgxUefiFwKeyBlob->TimeStamp.Year,
                      SgxUefiFwKeyBlob->TimeStamp.Month,
                      SgxUefiFwKeyBlob->TimeStamp.Day,
                      SgxUefiFwKeyBlob->TimeStamp.Hour,
                      SgxUefiFwKeyBlob->TimeStamp.Minute,
                      SgxUefiFwKeyBlob->TimeStamp.Second,
                      SgxUefiFwKeyBlob->TimeStamp.Nanosecond,
                      SgxUefiFwKeyBlob->TimeStamp.Pad1,
                      SgxUefiFwKeyBlob->TimeStamp.Pad2,
                      SgxUefiFwKeyBlob->TimeStamp.TimeZone,
                      SgxUefiFwKeyBlob->TimeStamp.Daylight));

  PrintMcheckKeyBlob((VOID *) &(SgxUefiFwKeyBlob->KeyBlob));
}

/**

  Prints all RegistrationPackageInfo structure filed
  @param          SgxRegistrationPackage   A pointer to the SGX_REGISTRATION_PACKAGE_INFO struct
    @retval     None

**/
VOID
PrintRegistrationPackageInfo (
  SGX_REGISTRATION_PACKAGE_INFO                     *SgxRegistrationPackage
)
{
  UINT8                         Index;
  SGX_DEBUG ((SGX_D_INFO, "\tHeader:\n"
                     "\t\tVersion:\n"));
   PrintByteArrays((VOID *) &(SgxRegistrationPackage->Header.Version), sizeof (UINT16));
   SGX_DEBUG ((SGX_D_INFO, "\tSize:\n"));
   PrintByteArrays((VOID *) &(SgxRegistrationPackage->Header.Size), sizeof (UINT16));

   for (Index = 0; Index < MCHECK_MAX_PACKAGES; ++Index) {
     SGX_DEBUG ((SGX_D_INFO, "  [SGX-KEYBLOB] KeyBlob for socket: %d\n", Index));
     PrintMcheckKeyBlob((VOID *) &(SgxRegistrationPackage->KeyBlobs[Index]));
   }
}

/**

  Prints MCHECK_BUFFER structure

  @param Indentation                 - number of whitespaces before print data
  @param MCHECK_BUFFER               - BIOS MCHECK_BUFFER buffer structure

  @retval None

**/
VOID
PrintMcheckBuffer (
  IN UINT32        Indentation,
  IN MCHECK_BUFFER Buffer
  )
{
  Indentation += INDENTATION_OFFSET;
  PrintWhitespaces (Indentation);
  SGX_DEBUG ((SGX_D_INFO, "Address: 0x%p Size: 0x%02X\n", Buffer.Address, Buffer.Size));
}


/**

  Prints MCHECK_SGX_PLATFORM_MEMBERSHIP_CERTIFICATE structure

  @param Indentation                                    - number of whitespaces before print data
  @param MCHECK_SGX_PLATFORM_MEMBERSHIP_CERTIFICATE     - Pointer to the SGX certificate structure

  @retval None

**/
VOID
PrintMcheckSgxPlatformMembershipCertificate (
  IN UINT32                                     Indentation,
  IN MCHECK_SGX_PLATFORM_MEMBERSHIP_CERTIFICATE *Certificate
  )
{
  SGX_DEBUG ((SGX_D_INFO, "Platform Certificate:\n"));
  PrintWhitespaces (Indentation);
  SGX_DEBUG ((SGX_D_INFO, "UUID: "));
  PrintByteArrays (Certificate->Header.Uuid.Uuid, MCHECK_UUID_SIZE);
  PrintWhitespaces (Indentation);
  SGX_DEBUG ((SGX_D_INFO,"CpuSvn: %d\n", Certificate->CpuSvn));
  PrintWhitespaces (Indentation);
  SGX_DEBUG ((SGX_D_INFO, "Prid: "));
  PrintByteArrays (Certificate->Prid.Uuid, MCHECK_UUID_SIZE);
  PrintWhitespaces (Indentation);
  SGX_DEBUG ((SGX_D_INFO,"PlatformInstanceId: "));
  PrintByteArrays (Certificate->PlatformInstanceId.Uuid, MCHECK_UUID_SIZE);
}


/**

  Prints SGX MCHECK_SGX_SOCKET_DATA structure

  @param Indentation                 - number of whitespaces before print data
  @param MCHECK_SGX_SOCKET_DATA      - BIOS Socket data structure

  @retval None

**/
VOID
PrintMcheckSgxSocketData (
  IN UINT32                 Indentation,
  IN MCHECK_SGX_SOCKET_DATA *SocketData
  )
{
  PrintWhitespaces (Indentation);
  SGX_DEBUG ((SGX_D_INFO, "Flags: %x\n", SocketData->Flags));
  PrintWhitespaces (Indentation);
  SGX_DEBUG ((SGX_D_INFO, "KeyBlob:\n"));
  PrintMcheckBuffer (Indentation+INDENTATION_OFFSET, SocketData->KeyBlob);
}


/**

  Prints SGX MCHECK_BIOS_PARAM_INFO structure

  @param SGX_MCHECK_BIOS_PARAM_INFO  - Pointer to the BIOS info

  @retval None

**/
VOID
PrintMcheckBiosParamInfo (
  IN MCHECK_BIOS_PARAM_INFO *ParamInfo
  )
{
  UINT8                  Index          = 0;
  UINT32                 Indentation    = 0;

  SGX_DEBUG ((SGX_D_INFO, "PrintMcheckBiosParamInfo BEGIN\n"));

  // Print SGX part
  if (mSecurityPolicy->EnableSgx) {

    MCHECK_SGX_INFO        *SgxInfo       = NULL;
    MCHECK_TOPOLOGY_INFO   *TopologyInfo  = NULL;
    MCHECK_SGX_SOCKET_DATA *SocketDataPtr = NULL;

    SgxInfo = ParamInfo->SgxInfo.Address;
    if (ParamInfo->SgxInfo.Size == 0 || SgxInfo == NULL) {
      SGX_DEBUG ((SGX_D_ERROR, "Error: SgxInfo is uninitialized\n"));
      goto Error_PrintMcheckBiosParamInfo;
    }

    TopologyInfo = ParamInfo->TopologyInfo.Address;
    if (ParamInfo->TopologyInfo.Size == 0 || TopologyInfo == NULL) {
      SGX_DEBUG ((SGX_D_ERROR, "Error: TopologyInfo is uninitialized\n"));
      goto Error_PrintMcheckBiosParamInfo;
    }

    SGX_DEBUG ((SGX_D_INFO, "Header:\n"));
    SGX_DEBUG ((SGX_D_INFO, "  Uuid: "));
    PrintByteArrays (ParamInfo->Header.Uuid.Uuid, 16);
    SGX_DEBUG ((SGX_D_INFO, "  Size: 0x%016lX\n", ParamInfo->Header.Size));
    SGX_DEBUG ((SGX_D_INFO, "  Version: 0x%016lX\n", ParamInfo->Header.Version));
    SGX_DEBUG ((SGX_D_INFO, "Features: 0x%016lX\n", ParamInfo->Features));
    SGX_DEBUG ((SGX_D_INFO, "SgxInfo:\n"));
    SGX_DEBUG ((SGX_D_INFO, "  Scenario: 0x%016lX\n", SgxInfo->Scenario));
    SGX_DEBUG ((SGX_D_INFO, "  CpuSvn: 0x%016lX\n", SgxInfo->CpuSvn));
    SGX_DEBUG ((SGX_D_INFO, "  SecurityProperties: 0x%016lX\n", SgxInfo->SecurityProperties));
    SGX_DEBUG ((SGX_D_INFO, "  RegistrationServer: "));
    PrintMcheckBuffer (Indentation, SgxInfo->RegistrationServer);
    SGX_DEBUG ((SGX_D_INFO, "  PlatformManifest: "));
    PrintMcheckBuffer (Indentation, SgxInfo->PlatformManifest);
    for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
      SGX_DEBUG ((SGX_D_INFO, "  Prids[%d]: ", Index));
      PrintByteArrays (SgxInfo->Prids[Index].Uuid, 16);
    }
    for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
      SocketDataPtr = (MCHECK_SGX_SOCKET_DATA *)SgxInfo->SocketData[Index].Address;
      if (SocketDataPtr != NULL) {
        SGX_DEBUG ((SGX_D_INFO, "  SocketData[%d]: \n", Index));
        PrintMcheckSgxSocketData (Indentation + 2 * INDENTATION_OFFSET, SocketDataPtr);
        PrintMcheckSgxPlatformMembershipCertificate (Indentation + 2 * INDENTATION_OFFSET, SocketDataPtr->Certificate.Address);
      }
    }

    SGX_DEBUG ((SGX_D_INFO, "TopologyInfo:\n"));
    SGX_DEBUG ((SGX_D_INFO, "  NumSockets: 0x%016lX\n", TopologyInfo->NumSockets));
    SGX_DEBUG ((SGX_D_INFO, "  Master: 0x%016lX\n", TopologyInfo->Master));
    for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
      SGX_DEBUG ((SGX_D_INFO, "  NodeIds[%d]: 0x%016lX\n", Index, TopologyInfo->NodeIds[Index]));
      SGX_DEBUG ((SGX_D_INFO, "  NodeTopology[%d]: 0x%016lX\n", Index, TopologyInfo->NodeTopology[Index]));
    }

    if (TopologyInfo->WorkingArea.Address != NULL && TopologyInfo->WorkingArea.Size > 0) {
      SGX_DEBUG ((SGX_D_INFO, "  WorkingArea:\n"));
      PrintMcheckBuffer (Indentation, TopologyInfo->WorkingArea);
    }
  }
  // Print TDX part
  if (mSecurityPolicy->EnableTdx) {

    MCHECK_SEAM_INFO *SeamInfo = NULL;

    SeamInfo = ParamInfo->SeamInfo.Address;
    if (ParamInfo->SeamInfo.Size == 0 || SeamInfo == NULL) {
      SGX_DEBUG ((SGX_D_INFO, "Info: SeamInfo is not initialized\n"));
      goto Error_PrintMcheckBiosParamInfo;
    }

    SGX_DEBUG ((SGX_D_INFO, "SeamInfo:\n"));
    for (Index = 0; Index < CMR_TABLE_ENTRIES; Index++ ) {
      SGX_DEBUG ((SGX_D_INFO, "  CmrTable[%d].CmrBase: 0x%016lX\n", Index, SeamInfo->CmrTable[Index].CmrBase));
      SGX_DEBUG ((SGX_D_INFO, "  CmrTable[%d].CmrSize: 0x%016lX\n", Index, SeamInfo->CmrTable[Index].CmrSize));
    }
  }

  SGX_DEBUG ((SGX_D_INFO, "PrintMcheckBiosParamInfo END\n"));
  return;

Error_PrintMcheckBiosParamInfo:
  SGX_DEBUG ((SGX_D_ERROR, "Error: Unable to fully print BiosParamInfo\n"));
  return;
}

/**
  Prints SGX SGX_MCHECK_BIOS_PARAM_INFO structure

  @param  MCHECK_BUFFER *ParamInfoBuffer
  @retval None
**/
VOID
PrintMcheckBiosParamInfoBufferAsmDump (
  IN  MCHECK_BUFFER *ParamInfoBuffer
  )
{
  MCHECK_BIOS_PARAM_INFO *ParamInfoPtr = NULL;
  UINT64                 Size          = 0;

  if (ParamInfoBuffer == NULL) {
    SGX_DEBUG ((SGX_D_ERROR, "Error: ParamInfoBuffer is not initialized\n"));
    goto Error_PrintMcheckBiosParamInfoBufferAsmDump;
  }

  ParamInfoPtr = (MCHECK_BIOS_PARAM_INFO *)ParamInfoBuffer->Address;
  Size         = ParamInfoBuffer->Size;

  // FCE uBIOS Param INFO dump
  if (MsrTraceEnabled ()) {
    DataRegionAsmDump ("ParamInfo",
                       (UINT8*)ParamInfoPtr,
                       Size
                       );
  }
  return;

Error_PrintMcheckBiosParamInfoBufferAsmDump:
  SGX_DEBUG ((SGX_D_ERROR, "Error: Unable to fully perform BiosParamInfoAsmDump\n"));
  return;
}

VOID
PrintMcheckHeader (
  MCHECK_HEADER          *McheckHeader
  )
{
  SGX_DEBUG ((SGX_D_INFO, "    UUID: "));
  PrintByteArrays (McheckHeader->Uuid.Uuid, 16);
  SGX_DEBUG ((SGX_D_INFO, "    Size: %d\n", McheckHeader->Size));
  SGX_DEBUG ((SGX_D_INFO, "    Version: %d\n", McheckHeader->Version));
}

VOID
PrintMcheckSgxPubkey (
  MCHECK_SGX_PUBKEY     *McheckSgxPubkey
  )
{
  SGX_DEBUG ((SGX_D_INFO, "    Modulus:"));
  PrintByteArrays (McheckSgxPubkey->Modulus, SHA3072_DIGEST_SIZE);
  SGX_DEBUG ((SGX_D_INFO, "    PubExp: %x\n", McheckSgxPubkey->PubExp));
}

VOID
PrintMcheckSgxRegistrationServerId (
  MCHECK_SGX_REGISTRATION_SERVER_ID           *McheckSgxRegistrationId
  )
{
  SGX_DEBUG ((SGX_D_INFO, "    Header:\n"));
  PrintMcheckHeader (&(McheckSgxRegistrationId->Header));
  SGX_DEBUG ((SGX_D_INFO, "    RsName: "));
  PrintByteArrays (McheckSgxRegistrationId->RsName, SHA256_DIGEST_SIZE);
  SGX_DEBUG ((SGX_D_INFO, "    Rsak: \n"));
  PrintMcheckSgxPubkey (&(McheckSgxRegistrationId->Rsak));
  SGX_DEBUG ((SGX_D_INFO, "    Rsak: \n"));
  PrintMcheckSgxPubkey (&(McheckSgxRegistrationId->Rsek));
  SGX_DEBUG ((SGX_D_INFO, "    Signature: "));
  PrintByteArrays (McheckSgxRegistrationId->Signature, SHA3072_DIGEST_SIZE);
}

VOID
PrintMcheckSgxRegistrationServerInfo (
  SGX_REGISTRATION_SERVER_INFO                  *SgxRegServerInfo
  )
{
  SGX_DEBUG ((SGX_D_INFO, "SgxRegServerInfo:\n"));
  SGX_DEBUG ((SGX_D_INFO, "  Header:\n"));
  PrintMcheckHeader (&(SgxRegServerInfo->Header));
  SGX_DEBUG ((SGX_D_INFO, "  UrlSize: %d\n", SgxRegServerInfo->UrlSize));
  SGX_DEBUG ((SGX_D_INFO, "  Url: "));
  PrintByteArrays(SgxRegServerInfo->Url, SGX_SERVER_URL_LENGTH);
  SGX_DEBUG ((SGX_D_INFO, "  RegServerId:"));
  PrintMcheckSgxRegistrationServerId (&(SgxRegServerInfo->RegServerId));
}

VOID
PrintSgxRegistrationConfig (
  SGX_REGISTRATION_CONFIG                   *SgxRegConf
  )
{
  SGX_DEBUG ((SGX_D_INFO, "SgxRegistrationConfig:\n"));
  SGX_DEBUG ((SGX_D_INFO, "Flags: %x\n", SgxRegConf->Flags));
  PrintMcheckSgxRegistrationServerInfo (&(SgxRegConf->SgxRegServerInfo));
}

/**
  Check if processor supports Platform Attestation
  (determined by CPUID.7.0.EDX[1])

  @retval TRUE    Processor supports Platform Attestation
  @retval FALSE   Processor doesn't support Platform Attestation
**/
BOOLEAN
IsPlatformAttestationSupported (
  VOID
  )
{
  CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_EDX  Edx;

  AsmCpuidEx (
    CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS,
    CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_SUB_LEAF_INFO,
    NULL, NULL, NULL, &Edx.Uint32
    );

  return ((Edx.Uint32 & BIT1) >> 1);
}

/**
  Detect MP Services data like system BSP, Socket BSPs.

  @param  None

  @retval EFI_SUCCESS       Load the Mp services structures. successfully.
          mMpServices       Pointer to MP Services PPI
          mSystemBsp        Thread number of the system BSP
          mProcInfo         Pointer to first element of array of EFI_PROCESSOR_INFORMATION
**/
EFI_STATUS
SgxMpServicesData (
  VOID
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINTN           ProcessorNum;
  UINTN           EnabledProcessorNum;
  UINTN           Index;

  SGX_DEBUG ((SGX_D_INFO, "SgxMpServicesData()\n"));

  Status = gBS->LocateProtocol (
    &gEfiMpServiceProtocolGuid,
    NULL,
    &mMpServices
    );
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: Failed to locate MP Services Protocol, exiting...\n"));
    return Status;
  }

  Status = mMpServices->GetNumberOfProcessors (
    mMpServices,
    &ProcessorNum,
    &EnabledProcessorNum
    );
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error detecting number of processor threads\n"));
    return Status;
  }

  Status = gBS->AllocatePool (
    EfiBootServicesData,
    sizeof (EFI_PROCESSOR_INFORMATION) * ProcessorNum,
    (VOID **) &mProcInfo
    );
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error allocating mProcInfo\n"));
    return Status;
  }

  //
  // Get each processor Location info
  //
  for (Index = 0; Index < ProcessorNum; Index++) {
    Status = mMpServices->GetProcessorInfo (
      mMpServices,
      Index,
      &mProcInfo [Index]
      );
    if (EFI_ERROR (Status)) {
      gBS->FreePool (mProcInfo);
      return Status;
    }

    //
    // Assign Package BSPs
    //
    if (Index == 0) {
      mProcInfo [Index].StatusFlag |= PROCESSOR_AS_PBSP_BIT; // PackageBsp
    } else if (mProcInfo [Index].Location.Package != mProcInfo [Index - 1].Location.Package){
      mProcInfo [Index].StatusFlag |= PROCESSOR_AS_PBSP_BIT;
      mSocketCount++;
    }

    if (mProcInfo [Index].StatusFlag & PROCESSOR_AS_PBSP_BIT) {
      SGX_DEBUG ((SGX_D_ERROR, "  Thread %d is BSP of Socket %d\n", Index, mProcInfo [Index].Location.Package));
    }


  } // for (Index)

  Status = mMpServices->WhoAmI (
    mMpServices,
    &mSystemBsp
    );
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error detecting SystemBSP\n"));
    return Status;
  }
  SGX_DEBUG ((SGX_D_INFO, "  System BSP Thread = %3X\n", mSystemBsp));

  mBspSocketId = mProcInfo [mSystemBsp].Location.Package;
  SGX_DEBUG ((SGX_D_INFO, "  System BSP Package = %3X\n", mBspSocketId));

  return Status;
}

/**

  Initialize SGX_MCHECK_REGISTRATION_SERVER_ID with intel default values

  @param[out]  ServerData is a SGX_MCHECK_REGISTRATION_SERVER_ID empry structure with registration Server data
               that MECHECK will consume.

**/
VOID
PopulateMcheckSgxRegistrationServerId (
  OUT MCHECK_SGX_REGISTRATION_SERVER_ID *ServerData
  )
{
  UINT8       ServerUuid[]      = MCHECK_SGX_REGISTRATION_SERVER_UUID;
  UINT64      ServerRsName[]    = MCHECK_SGX_SERVER_RSNAME;
  UINT64      ServerRsak[]      = MCHECK_SGX_SERVER_RSAK;
  UINT64      ServerRsek[]      = MCHECK_SGX_SERVER_RSEK;
  UINT64      ServerSignature[] = MCHECK_SGX_SERVER_SIGNATURE;

  CopyMem (ServerData->Header.Uuid.Uuid64, ServerUuid, sizeof (MCHECK_UUID));
  ServerData->Header.Size = sizeof (MCHECK_SGX_REGISTRATION_SERVER_ID) - sizeof (MCHECK_HEADER);
  ServerData->Header.Version = MCHECK_HEADER_DEFAULT_VERSION;

  CopyMem (ServerData->RsName, ServerRsName, SHA256_DIGEST_SIZE);

  CopyMem (ServerData->Rsak.Modulus, ServerRsak, SHA3072_DIGEST_SIZE);
  ServerData->Rsak.PubExp = MCHECK_SGX_SERVER_RSAK_EXP;

  CopyMem (ServerData->Rsek.Modulus, ServerRsek, SHA3072_DIGEST_SIZE);
  ServerData->Rsek.PubExp = MCHECK_SGX_SERVER_RSEK_EXP;

  CopyMem (ServerData->Signature, ServerSignature, SHA3072_DIGEST_SIZE);
}

/*
 * Populate MCHECK SgxPlatformManifest structure
 *
 * @param[out] MCHECK_SGX_PLATFORM_MANIFEST *McheckSgxPlatformManifest
 * @retval     EFI_STATUS                   EFI_SUCCESS
 *                                          EFI_INVALID_PARAMETER
 */
EFI_STATUS
PopulateMcheckSgxPlatformManifest (
  OUT MCHECK_SGX_PLATFORM_MANIFEST *McheckSgxPlatformManifest
  )
{
  EFI_STATUS Status                 = EFI_SUCCESS;
  UINT32     Index                  = 0;
  UINT8      PlatformManifestUuid[] = MCHECK_SGX_PLATFORM_MANIFEST_UUID;
  UINT8      PlatformInfoUuid[]     = MCHECK_SGX_PLATFORM_INFO_UUID;
  UINT8      PairingReceiptUuid[]   = MCHECK_SGX_PAIRING_RECEIPT_UUID;
  UINT8      EncryptedKeysUuid[]    = MCHECK_SGX_ENCRYPTED_PLATFORM_KEYS_UUID;

  if (McheckSgxPlatformManifest == NULL) {
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  // PlatformManifest
  CopyMem (McheckSgxPlatformManifest->Header.Uuid.Uuid64, PlatformManifestUuid, sizeof (MCHECK_UUID));
  McheckSgxPlatformManifest->Header.Size    = sizeof (MCHECK_SGX_PLATFORM_MANIFEST) - sizeof (MCHECK_HEADER);
  McheckSgxPlatformManifest->Header.Version = MCHECK_HEADER_DEFAULT_VERSION;

  // PlatformInfo
  CopyMem (McheckSgxPlatformManifest->PlatformInfo.Header.Uuid.Uuid64, PlatformInfoUuid, sizeof (MCHECK_UUID));
  McheckSgxPlatformManifest->PlatformInfo.Header.Size    = sizeof (MCHECK_SGX_PLATFORM_INFO) - sizeof (MCHECK_HEADER);
  McheckSgxPlatformManifest->PlatformInfo.Header.Version = MCHECK_HEADER_DEFAULT_VERSION;

  // EncryptedKeys
  CopyMem (McheckSgxPlatformManifest->EncryptedKeys.Header.Uuid.Uuid64, EncryptedKeysUuid, sizeof (MCHECK_UUID));
  McheckSgxPlatformManifest->EncryptedKeys.Header.Size    = sizeof (MCHECK_SGX_PLATFORM_INFO) - sizeof (MCHECK_HEADER);
  McheckSgxPlatformManifest->EncryptedKeys.Header.Version = MCHECK_HEADER_DEFAULT_VERSION;

  // Receipt[0..7]
  for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
    CopyMem (McheckSgxPlatformManifest->Receipt[Index].Header.Uuid.Uuid64, PairingReceiptUuid, sizeof (MCHECK_UUID));
    McheckSgxPlatformManifest->Receipt[Index].Header.Size    = sizeof (MCHECK_SGX_PAIRING_RECEIPT) - sizeof (MCHECK_HEADER);
    McheckSgxPlatformManifest->Receipt[Index].Header.Version = MCHECK_HEADER_DEFAULT_VERSION;
  }

  return Status;
}

/*
 * Populate MCHECK SgxKeyBlob structure
 *
 * @param[out] MCHECK_SGX_KEY_BLOB  *McheckSgxKeyBlob
 * @retval     EFI_STATUS           EFI_SUCCESS
 *                                  EFI_INVALID_PARAMETER
 */
EFI_STATUS
PopulateMcheckSgxKeyBlob (
  OUT MCHECK_SGX_KEY_BLOB *McheckSgxKeyBlob
  )
{
  EFI_STATUS Status        = EFI_SUCCESS;
  UINT8      KeyBlobUuid[] = MCHECK_SGX_KEY_BLOB_UUID;

  if (McheckSgxKeyBlob == NULL) {
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  CopyMem (McheckSgxKeyBlob->Header.Uuid.Uuid64, KeyBlobUuid, sizeof (MCHECK_UUID));
  McheckSgxKeyBlob->Header.Size    = sizeof (MCHECK_SGX_KEY_BLOB) - sizeof (MCHECK_HEADER);
  McheckSgxKeyBlob->Header.Version = MCHECK_HEADER_DEFAULT_VERSION;

  return Status;
}

/*
 * Populate MCHECK SgxCertificate structure
 *
 * @param[out] MCHECK_SGX_PLATFORM_MEMBERSHIP_CERTIFICATE *McheckSgxCertificate
 * @retval     EFI_STATUS                                 EFI_SUCCESS
 *                                                        EFI_INVALID_PARAMETER
 */
EFI_STATUS
PopulateMcheckSgxCertificate (
  OUT MCHECK_SGX_PLATFORM_MEMBERSHIP_CERTIFICATE *McheckSgxCertificate
  )
{
  EFI_STATUS Status            = EFI_SUCCESS;
  UINT8      CertificateUuid[] = MCHECK_SGX_PLATFORM_MEMBERSHIP_CERTIFICATE_UUID;

  if (McheckSgxCertificate == NULL) {
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  CopyMem (McheckSgxCertificate->Header.Uuid.Uuid64, CertificateUuid, sizeof (MCHECK_UUID));
  McheckSgxCertificate->Header.Size    = sizeof (MCHECK_SGX_PLATFORM_MEMBERSHIP_CERTIFICATE) - sizeof (MCHECK_HEADER);
  McheckSgxCertificate->Header.Version = MCHECK_HEADER_DEFAULT_VERSION;

  return Status;
}

/*
 * Populate MCHECK SgxInfo structure
 *
 * @param[out] MCHECK_SGX_INFO  *SgxInfo
 * @retval     EFI_STATUS        EFI_SUCCESS
 *                               EFI_UNSUPPORTED
 *                               EFI_OUT_OF_RESOURCES
 */
EFI_STATUS
PopulateMcheckSgxInfo (
  OUT MCHECK_SGX_INFO *SgxInfo
  )
{
  EFI_STATUS             Status         = EFI_SUCCESS;
  UINT8                  Index          = 0;
  MCHECK_SGX_SOCKET_DATA *SocketDataPtr = NULL;

  SGX_DEBUG ((SGX_D_INFO, "[SGX] PopulateMcheckSgxInfo BEGIN\n"));

  if (SgxInfo == NULL) {
    Status = EFI_INVALID_PARAMETER;
    SGX_DEBUG ((SGX_D_ERROR, "  Error: SgxInfo is uninitialized!\n"));
    goto Exit_PopulateMcheckSgxInfo;
  }

  // Match UEFI BOOT Scenarios with MCHECK Scenarios
  switch (mSgxInitDataHob->SgxBootScenario) {
    case SgxNormalBoot:
      SgxInfo->Scenario = (UINT8)(MCHECK_SGX_SCENARIO)McheckRegularBoot;
      break;
    case SgxFirstBinding:
      SgxInfo->Scenario = (UINT8)(MCHECK_SGX_SCENARIO)McheckFirstPlatformBinding;
      break;
    case SgxTcbRecovery:
      SgxInfo->Scenario = (UINT8)(MCHECK_SGX_SCENARIO)McheckTcbRecovery;
      break;
    case SgxAddPackageBoot:
      // MCHECK is not launched on SgxAddPackageBoot
      SgxInfo->Scenario = (UINT8)(MCHECK_SGX_SCENARIO)McheckInvalid;
      break;
    case SgxAddPackageReboot:
      SgxInfo->Scenario = (UINT8)(MCHECK_SGX_SCENARIO)McheckAddingNewPackages;
      break;
    default:
      SgxInfo->Scenario = (UINT8)(MCHECK_SGX_SCENARIO)McheckInvalid;
      break;
  }

  // CpuSvn
  SgxInfo->CpuSvn = mSgxInitDataHob->BootingSvn;

  // SecurityProperties
  SgxInfo->SecurityProperties.BiosProperties   = SECURITY_PROPERTIES_SGX_TEM;
  if (mSgxInitDataHob->MktmeIntegrityEnabled) {
    SgxInfo->SecurityProperties.BiosProperties |= SECURITY_PROPERTIES_MEM_INGERITY;
  }

  // Check allocation for RegistrationServer
  // Restore/Copy Server ID from ConfigurationVariable to SgxInfo
  if (SgxInfo->RegistrationServer.Address == NULL ||
      SgxInfo->RegistrationServer.Size != sizeof (MCHECK_SGX_REGISTRATION_SERVER_ID)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: SgxInfo->RegistrationServer is a NULL or its size is invalid!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit_PopulateMcheckSgxInfo;
  }
  CopyMem (SgxInfo->RegistrationServer.Address,
           &mInternalRegistrationConfig.SgxRegServerInfo.RegServerId,
           sizeof (MCHECK_SGX_REGISTRATION_SERVER_ID));

  // Check allocation for PlatformManifest
  if (SgxInfo->PlatformManifest.Address == NULL ||
      SgxInfo->PlatformManifest.Size != sizeof (MCHECK_SGX_PLATFORM_MANIFEST)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: SgxInfo->PlatformManifest is a NULL or its size is invalid!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit_PopulateMcheckSgxInfo;
  }
  PopulateMcheckSgxPlatformManifest ((MCHECK_SGX_PLATFORM_MANIFEST *)SgxInfo->PlatformManifest.Address);

  // Prids
  for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
    if (!SocketPresent (Index)) {
      continue;
    }
    SgxInfo->Prids[Index].Uuid64[0] = mSgxInitDataHob->Socket[Index].Prids_0;
    SgxInfo->Prids[Index].Uuid64[1] = mSgxInitDataHob->Socket[Index].Prids_1;
  }

  // SocketData
  for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
    if (!SocketPresent (Index)) {
      continue;
    }
    // Check SocketData[Index] allocation
    if (SgxInfo->SocketData[Index].Address == NULL ||
        SgxInfo->SocketData[Index].Size != sizeof (MCHECK_SGX_SOCKET_DATA)) {
      SGX_DEBUG ((SGX_D_ERROR, "  Error: SgxInfo->SocketData[%d] is a NULL or its size is invalid!\n", Index));
      Status = EFI_INVALID_PARAMETER;
      goto Exit_PopulateMcheckSgxInfo;
    }
    // Set temporary pointer for SocketData entry
    SocketDataPtr = (MCHECK_SGX_SOCKET_DATA *)SgxInfo->SocketData[Index].Address;

    // Check allocation for KeyBlob
    if (SocketDataPtr->KeyBlob.Address == NULL ||
        SocketDataPtr->KeyBlob.Size != sizeof (MCHECK_SGX_KEY_BLOB)) {
      SGX_DEBUG ((SGX_D_ERROR, "  Error: SgxInfo->SocketData[%d]->KeyBlob is a NULL or its size is invalid!\n", Index));
      Status = EFI_INVALID_PARAMETER;
      goto Exit_PopulateMcheckSgxInfo;
    }
    PopulateMcheckSgxKeyBlob ((MCHECK_SGX_KEY_BLOB *)SocketDataPtr->KeyBlob.Address);

    // Check allocation for Platform Membership Certificate
    if (SocketDataPtr->Certificate.Address == NULL ||
        SocketDataPtr->Certificate.Size != sizeof (MCHECK_SGX_PLATFORM_MEMBERSHIP_CERTIFICATE)) {
      SGX_DEBUG ((SGX_D_ERROR, "  Error: SgxInfo->SocketData[%d]->Certificate is a NULL or its size is invalid!\n", Index));
      Status = EFI_INVALID_PARAMETER;
      goto Exit_PopulateMcheckSgxInfo;
    }
    PopulateMcheckSgxCertificate ((MCHECK_SGX_PLATFORM_MEMBERSHIP_CERTIFICATE *)SocketDataPtr->Certificate.Address);

    if ((mSgxInitDataHob->SgxBootScenario != SgxFirstBinding) &&
        mSgxInitDataHob->UefiFwKeyBlobs[Index].Present &&
        !mSgxInitDataHob->Socket[Index].AddPackageNew) {
      SGX_DEBUG ((SGX_D_INFO, "  Info: KeyBlob found for Socket[%d]\n", Index));
      // Restore/Copy Key Blobs from SGX Hob to BiosParamInfo
      CopyMem (SocketDataPtr->KeyBlob.Address,
              &mSgxInitDataHob->UefiFwKeyBlobs[Index].KeyBlob,
              sizeof (MCHECK_SGX_KEY_BLOB));
      // Set flags accordingly
      SocketDataPtr->Flags = FLAGS_BLOB_IN_BIT;
    }

    if ((mSgxInitDataHob->SgxBootScenario == SgxAddPackageReboot) && mSgxInitDataHob->Socket[Index].AddPackageNew) {
      // Restore/Copy Platform Certificate Blobs from SGX Hob to BiosParamInfo
      SGX_DEBUG ((SGX_D_INFO, "  Info: Plat Cert Membership Index %d copied to BiosParamsInfo\n", Index));
      CopyMem (SocketDataPtr->Certificate.Address,
              &mSgxInitDataHob->UefiFwPlatMemCert[Index].PlatformMembershipCertificate,
              sizeof(MCHECK_SGX_PLATFORM_MEMBERSHIP_CERTIFICATE));
    }
  }

Exit_PopulateMcheckSgxInfo:
  SGX_DEBUG ((SGX_D_INFO, "[SGX] PopulateMcheckSgxInfo END\n"));
  return Status;
}

/*
 * Fill CMR table
 *
 * @param[out] MCHECK_SEAM_INFO *SeamInfo
 * @param[out] CmrTableSize proper size of CMR table
 *
 * @retval     EFI_SUCCESS successfully filled CMR table for SeamInfo
 */
EFI_STATUS
EFIAPI
FillCmrTable (
  OUT MCHECK_SEAM_INFO *SeamInfo,
  OUT UINTN *CmrTableSize
  )
{
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR *MemoryMap = NULL;
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR *MemoryDescriptor;
  UINTN                           NumberOfDescriptors = 0;
  UINTN                           Index;
  UINTN                           CmrIndex = 0;
  EFI_STATUS                      Status;

  Status = gDS->GetMemorySpaceMap (&NumberOfDescriptors, &MemoryMap);
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "Failed to get GCD memory map space\n"));
    return Status;
  }

  for (Index = 0; Index < NumberOfDescriptors; Index++) {
    MemoryDescriptor = &MemoryMap[Index];
    if (MemoryDescriptor->GcdMemoryType == EfiGcdMemoryTypeSystemMemory && CmrIndex < CMR_TABLE_ENTRIES) {
      SGX_DEBUG ((SGX_D_VERBOSE, "CMR has been copied CMRIndex %d, DescriptorIndex %d\n", CmrIndex, Index));
      SeamInfo->CmrTable[CmrIndex].CmrBase = MemoryDescriptor->BaseAddress;
      SeamInfo->CmrTable[CmrIndex].CmrSize = MemoryDescriptor->Length;
      CmrIndex++;

      if (MemoryDescriptor->Length > 0) {
        (*CmrTableSize)++;
      }
    }
  }

  return Status;
}

/*
 * Create MCHECK SeamInfo structure
 *
 * @param    MCHECK_BIOS_PARAM_INFO *BiosParamInfo
 * @retval   EFI_SUCCESS            successfuly prepared SeamInfo structure
 *
 */
EFI_STATUS
EFIAPI
PopulateMcheckSeamInfo (
  OUT MCHECK_BIOS_PARAM_INFO *BiosParamInfo
  )
{

  EFI_STATUS Status = EFI_SUCCESS;
  UINTN CmrTableSize = 0;

  SGX_DEBUG ((SGX_D_INFO, "[TDX] PrepareMcheckSeamInfoStructure BEGIN\n"));
  ZeroMem (BiosParamInfo->SeamInfo.Address, sizeof (MCHECK_SEAM_INFO));

  Status = FillCmrTable ((MCHECK_SEAM_INFO *)BiosParamInfo->SeamInfo.Address, &CmrTableSize);
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, " Error: Cannot fill CMR Table!\n"));
  }

  // Update SeamInfo size
  BiosParamInfo->SeamInfo.Size = CmrTableSize;

  SGX_DEBUG ((SGX_D_INFO, "[TDX] PrepareMcheckSeamInfoStructure END\n"));
  return Status;
}

/*
 * Populate MCHECK TopologyInfo structure
 *
 * @param[in,out] MCHECK_TOPOLOGY_INFO  *TopologyInfo
 * @retval        EFI_STATUS            EFI_SUCCESS
 *                                      EFI_UNSUPPORTED
 *                                      EFI_OUT_OF_RESOURCES
 */
EFI_STATUS
PopulateMcheckTopologyInfo (
  IN OUT MCHECK_TOPOLOGY_INFO *TopologyInfo
  )
{
  EFI_STATUS            Status        = EFI_SUCCESS;
  UINT8                 Index         = 0;
  MEMORY_MODE_ATTRIBUTE MemMode;

  SGX_DEBUG ((SGX_D_ERROR, "[SGX] PopulateMcheckTopologyInfo BEGIN\n"));

  if (TopologyInfo == NULL) {
    Status = EFI_INVALID_PARAMETER;
    SGX_DEBUG ((SGX_D_ERROR, "  Error: TopologyInfo is uninitialized!\n"));
    goto Exit_PopulateMcheckTopologyInfo;
  }

  // NumSockets
  TopologyInfo->NumSockets = (UINT8) mSocketCount;

  // Master
  TopologyInfo->Master = 0;
  for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
    if (SocketPresent (Index) &&
        mSgxInitDataHob->UefiFwKeyBlobs[Index].Present &&
        !mSgxInitDataHob->Socket[Index].AddPackageNew) {
      TopologyInfo->Master = Index;
      break;
    }
  }

  // NodeIds & NodeTopology
  for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
    TopologyInfo->NodeIds[Index] = 0xFF;
    if (SocketPresent (Index)) {
      TopologyInfo->NodeIds[Index] = Index;
    }

    if (SocketPresent (Index)) {
      GetMemMode (Index, &MemMode);
      switch (MemMode) {
        case MEMORY_MODE_ATTRIBUTE_1LM:
          TopologyInfo->NodeTopology[Index] = NODE_TOPOLOGY_MEM_TYPE_1LM;
          break;
        case MEMORY_MODE_ATTRIBUTE_2LM:
          TopologyInfo->NodeTopology[Index] = NODE_TOPOLOGY_MEM_TYPE_2LM_IN_TILE;
          break;
        case MEMORY_MODE_ATTRIBUTE_2LM_XTILE:
          TopologyInfo->NodeTopology[Index] = NODE_TOPOLOGY_MEM_TYPE_2LM_X_TILE;
          break;
        case MEMORY_MODE_ATTRIBUTE_1LM | MEMORY_MODE_ATTRIBUTE_2LM:
          TopologyInfo->NodeTopology[Index] = NODE_TOPOLOGY_MEM_TYPE_1LM_2LM_IN_TILE;
          break;
        case MEMORY_MODE_ATTRIBUTE_1LM | MEMORY_MODE_ATTRIBUTE_2LM_XTILE:
          TopologyInfo->NodeTopology[Index] = NODE_TOPOLOGY_MEM_TYPE_1LM_2LM_X_TILE;
          break;
        default:
          Status = EFI_UNSUPPORTED;
          SGX_DEBUG ((SGX_D_ERROR, "  Error: Invalid MemMode provided!\n", MemMode));
          goto Exit_PopulateMcheckTopologyInfo;
      }
    }
  }

Exit_PopulateMcheckTopologyInfo:
  SGX_DEBUG ((SGX_D_ERROR, "[SGX] PopulateMcheckTopologyInfo END\n"));
  return Status;
}

/**

  This is the structure of parameters passed by BIOS to MCHECK.

  @param[out]  BiosParamInfo                Structure with topology, bootscenario and registration server data that MCHECK will use
  @param[in]   BiosParamInfoCreationPolicy  Structure which describe what features are enabled

**/
EFI_STATUS
PopulateMcheckBiosParamInfo (
  MCHECK_BUFFER *BiosParamInfoBuffer,
  UINT8         BiosParamInfoCreationPolicy
  )
{
  EFI_STATUS             Status              = EFI_SUCCESS;
  UINT8                  BiosParamInfoUuid[] = MCHECK_BIOS_PARAM_INFO_UUID;
  MCHECK_BIOS_PARAM_INFO *BiosParamInfo      = NULL;

  SGX_DEBUG ((SGX_D_INFO, "[SGX] PopulateMcheckBiosParamInfo BEGIN\n"));

  // Caller has to preallocate memory for BiosParamInfoBuffer
  if ((BiosParamInfoBuffer == NULL) || (BiosParamInfoBuffer->Address == NULL) || (BiosParamInfoBuffer->Size == 0) ||
      (BiosParamInfoCreationPolicy == 0x0)) {
    Status = EFI_INVALID_PARAMETER;
    goto Error_PopulateMcheckBiosParamInfo;
  }
  BiosParamInfo = (MCHECK_BIOS_PARAM_INFO *)BiosParamInfoBuffer->Address;

  // Populate Header substructure
  CopyMem (BiosParamInfo->Header.Uuid.Uuid, BiosParamInfoUuid, sizeof (MCHECK_UUID));
  BiosParamInfo->Header.Size    = sizeof (MCHECK_BIOS_PARAM_INFO) - sizeof (MCHECK_HEADER);
  BiosParamInfo->Header.Version = MCHECK_BIOS_PARAM_INFO_HEADER_VERSION;

  if (BiosParamInfoCreationPolicy & FEATURES_SGX) {
    BiosParamInfo->Features |= FEATURES_SGX;

    // Populate SgxInfo substructure
    if (BiosParamInfo->SgxInfo.Address == NULL ||
        BiosParamInfo->SgxInfo.Size != sizeof (MCHECK_SGX_INFO)) {
      Status = EFI_INVALID_PARAMETER;
      SGX_DEBUG ((SGX_D_ERROR, "  Error: SgxInfo is a NULL or its size is invalid\n"));
      goto Error_PopulateMcheckBiosParamInfo;
    }
    Status = PopulateMcheckSgxInfo ((MCHECK_SGX_INFO *)BiosParamInfo->SgxInfo.Address);
    if (EFI_ERROR (Status)) {
      SGX_DEBUG ((SGX_D_ERROR, "  Error: Unable to prepare MCHECK SgxInfo substructure\n"));
      goto Error_PopulateMcheckBiosParamInfo;
    }
  }

  if (BiosParamInfoCreationPolicy & FEATURES_SEAM) {
    BiosParamInfo->Features |= FEATURES_SEAM;

    if (BiosParamInfo->SeamInfo.Address == NULL || BiosParamInfo->SeamInfo.Size != sizeof (MCHECK_SEAM_INFO)) {
      Status = EFI_INVALID_PARAMETER;
      SGX_DEBUG ((SGX_D_ERROR, "  Error: SeamInfo is a NULL or its size is invalid\n"));
      goto Error_PopulateMcheckBiosParamInfo;
    }

    // Populate SeamInfo substructure
    Status = PopulateMcheckSeamInfo (BiosParamInfo);
    if (EFI_ERROR (Status)) {
      SGX_DEBUG ((SGX_D_ERROR, "Error: Unable to prepare MCHECK SeamInfo substructure\n"));
      goto Error_PopulateMcheckBiosParamInfo;
    }
  }

  if (BiosParamInfoCreationPolicy & FEATURES_FUSASAF) {
    BiosParamInfo->Features |= FEATURES_FUSASAF;
  }

  // Populate TopologyInfo substructure
  if (BiosParamInfo->TopologyInfo.Address == NULL ||
      BiosParamInfo->TopologyInfo.Size != sizeof (MCHECK_TOPOLOGY_INFO)) {
    Status = EFI_INVALID_PARAMETER;
    SGX_DEBUG ((SGX_D_ERROR, "  Error: TopologyInfo is a NULL or its size is invalid\n"));
    goto Error_PopulateMcheckBiosParamInfo;
  }
  Status = PopulateMcheckTopologyInfo ((MCHECK_TOPOLOGY_INFO *)BiosParamInfo->TopologyInfo.Address);
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "Error: Unable to prepare MCHECK TopologyInfo substructure\n"));
  }

  SGX_DEBUG ((SGX_D_INFO, "[SGX] PopulateMcheckBiosParamInfo END Status %r\n", Status));

Error_PopulateMcheckBiosParamInfo:
  return Status;
}
/**

  Reloads uCode for SGX initialization
  This function is executed on each Socket BSP

  @param  VOID   *BiosParamInfo - a pointer to the BIOS_PARAMS structure

  @retval mMcuErrorCode[], Index = SocketNumber
          0       SGX uCode returns successful
          Others  Error Code returned by SGX uCode

**/
VOID
SgxMicrocodeLaunch (
  VOID                                *BiosParamInfo
  )
{
  EFI_STATUS                          Status;
  UINTN                               Thread;
  UINTN                               Socket;
  MICROCODE_INFO                      *MicrocodeInfo;
  SPR_MSR_SGX_MCU_ERRORCODE_REGISTER  UcodeError;

  Status = mMpServices->WhoAmI (
    mMpServices,
    &Thread
    );
  if (EFI_ERROR (Status)) {
    SerialPrintBlocking (SGX_D_ERROR, "Error detecting thread of execution (%r)\n", Status);
    Thread = 0;
  }
  Socket = mProcInfo[Thread].Location.Package;
  AsmWriteMsr64 (SPR_MSR_MCHECK_BOOT_SCENARIO, ((UINT64) BiosParamInfo));
    // According to SoftwareDevelopersManual BIOS_UPDT_TRIG 0x79 MSR is write only
    // and running uCode update requires uCode patch address and WRMSR 0x79
    //
    MicrocodeInfo = &GET_CPU_MISC_DATA (Thread, MicrocodeInfo);
    AsmWriteMsr64 (MSR_BIOS_UPDT_TRIG, (UINT64) (UINTN) MicrocodeInfo->MicrocodeData);

  UcodeError.Uint64 = AsmReadMsr64 (SPR_MSR_SGX_MCU_ERRORCODE);
  if (UcodeError.Bits.McheckErrorCode != 0) {
    mMcuErrorCode[Socket] = (UINT32)UcodeError.Bits.McheckErrorCode;
  }
}


/**

  SGX late init flow at thread scope helper function

  @param  None

  @retval None

**/
VOID
EFIAPI
EnableSgxThreadScopeLateHelper (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       Thread;

  Status = mMpServices->WhoAmI (
    mMpServices,
    &Thread
    );
  if (EFI_ERROR (Status)) {
    SerialPrintBlocking (SGX_D_ERROR, "Error detecting thread\n");
    Thread = 0xBAD;
  } else {
    SerialPrintBlocking (SGX_D_ERROR, "<Thread[%3X]>\n", Thread);
  }

  //
  // Configures the Launch Enclave Provider Hash information in processor cores
  //
  if (mSecurityPolicy->SgxLePubKeyHash0 != 0 ||
      mSecurityPolicy->SgxLePubKeyHash1 != 0 ||
      mSecurityPolicy->SgxLePubKeyHash2 != 0 ||
      mSecurityPolicy->SgxLePubKeyHash3 != 0) {
    SerialPrintBlocking (SGX_D_ERROR, "  Writing LEPUBHASH\n");
    AsmWriteMsr64 (MSR_SGXLEPUBKEYHASH0, mSecurityPolicy->SgxLePubKeyHash0);
    AsmWriteMsr64 (MSR_SGXLEPUBKEYHASH1, mSecurityPolicy->SgxLePubKeyHash1);
    AsmWriteMsr64 (MSR_SGXLEPUBKEYHASH2, mSecurityPolicy->SgxLePubKeyHash2);
    AsmWriteMsr64 (MSR_SGXLEPUBKEYHASH3, mSecurityPolicy->SgxLePubKeyHash3);
  }
  SerialPrintBlocking (SGX_D_ERROR, "</Thread[%3X]\n", Thread);
}


/**

  SGX late init flow at thred scope

  @param  None

  @retval None

**/
VOID
EFIAPI
ProgramLaunchControlHashOnAllThreads (
  VOID
  )
{
  EFI_STATUS  Status;

  //
  // Thread scope SGX init flow
  //
  EnableSgxThreadScopeLateHelper ();
  Status = mMpServices->StartupAllAPs (
    mMpServices,
    (EFI_AP_PROCEDURE) EnableSgxThreadScopeLateHelper,
    FALSE,
    NULL,
    0,
    NULL,
    NULL
    );
  if (Status == EFI_NOT_STARTED) {
    SGX_DEBUG ((SGX_D_ERROR, "  APs not started, continue...\n"));
  } else if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error programming Launch Control Mode on all threads (%r), continue init\n", Status));
  }
}

/*

  Checks if Prmrr Valid bit is set in PRMRR_MASK MSR

  @param[in, out] VOID* Pointer to EFI_STATUS

 */
VOID
CheckPrmrrValidBitThreadScope (
  VOID
  )
{
  SPR_MSR_PRMRR_MASK_REGISTER PrmrrMask;
  EFI_STATUS  Status = EFI_SUCCESS;
  UINTN       Thread;

  Status = mMpServices->WhoAmI (
    mMpServices,
    &Thread
    );
  if (EFI_ERROR (Status)) {
    SerialPrintBlocking (SGX_D_ERROR, "  Error detecting thread of execution\n");
    Thread = 0xBAD;
  }

  PrmrrMask.Uint64 = AsmReadMsr64 (SPR_MSR_PRMRR_MASK);
  if (PrmrrMask.Bits.Vld != 1) {
    mMcuErrorCode[Thread] = 0x1;
  }
}


/*

  Checks whether PRMRR_VALID bit is set on all threads

  @retval EFI_UNSUPPORTED PRMRR_VALID bit is not set on all threads
        Others An error occured

*/
EFI_STATUS
CheckPrmrrValidBit (
    VOID
  )
{
  EFI_STATUS Status;
  UINTN      Thread;
  ZeroMem (mMcuErrorCode, sizeof (mMcuErrorCode));

  CheckPrmrrValidBitThreadScope ();
  Status = mMpServices->StartupAllAPs (
    mMpServices,
    (EFI_AP_PROCEDURE) CheckPrmrrValidBitThreadScope,
    TRUE,
    NULL,
    0,
    NULL,
    NULL
    );
  if (Status == EFI_NOT_STARTED) {
    SGX_DEBUG ((SGX_D_ERROR, "  APs not started, continue...\n"));
  } else if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: Failed CheckPrmrrValidBitThreadScope (%r), exiting...\n", Status));
  }

  for (Thread = 0; Thread < MAX_CPU_NUM; Thread++) {
    if (mMcuErrorCode[Thread]) {
      SerialPrintBlocking (SGX_D_ERROR, "[SGX] Thread[%2X] CheckPrmrrValidBit not set!\n", Thread);
      Status = EFI_LOAD_ERROR;
    }
  }

  return Status;
}


/**

  Activates SGX on the thread it is executed
  This function is executed on each thread in the system (actually core scope)

  @param  None

  @retval None

**/
VOID
SgxActivate (
  VOID
  )
{
  MSR_BIOS_UPGD_TRIG_REGISTER MsrSgxActivate;
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN      Thread;
  UINTN      Socket;

  Status = mMpServices->WhoAmI (
    mMpServices,
    &Thread
    );
  if (EFI_ERROR (Status)) {
    SerialPrintBlocking (SGX_D_ERROR, "  Error detecting thread of execution\n");
    Thread = 0xBAD;
  }

  Socket = mProcInfo[Thread].Location.Package;

  MsrSgxActivate.Uint64 = AsmReadMsr64 (MSR_BIOS_UPGD_TRIG);
  if (MsrSgxActivate.Bits.Se == 1) {
    AsmWriteMsr64 (MSR_BIOS_UPGD_TRIG, MsrSgxActivate.Uint64);
  } else {
    mMcuErrorCode[Thread] = 0x1;
    return;
  }

  SerialPrintBlocking (SGX_D_ERROR, "  Socket[%2X], Thread[%3X] SGX activated\n", Socket, Thread);
}


/**

  This function populates EPC information retrieved from CPUID leaf 12h into ACPI tables.

  @param  None

  @retval None

**/
EFI_STATUS
PopulateEpcAcpiDevices (
  VOID
  )
{
  CPUID_INTEL_SGX_CAPABILITIES_RESOURCES_SUB_LEAF_EAX Eax;
  CPUID_INTEL_SGX_CAPABILITIES_RESOURCES_SUB_LEAF_EBX Ebx;
  CPUID_INTEL_SGX_CAPABILITIES_RESOURCES_SUB_LEAF_ECX Ecx;
  CPUID_INTEL_SGX_CAPABILITIES_RESOURCES_SUB_LEAF_EDX Edx;

  UINT64 EpcBaseAddress;
  UINT64 EpcLength;
  UINT8 EpcIndex;

  for (EpcIndex = 0; EpcIndex < mSgxInitDataHob->SgxPreMemInitHob.PrmrrCount; EpcIndex++) {
    AsmCpuidEx (CPUID_INTEL_SGX, CPUID_INTEL_SGX_CAPABILITIES_RESOURCES_SUB_LEAF + EpcIndex, &Eax.Uint32, &Ebx.Uint32, &Ecx.Uint32, &Edx.Uint32);

    if (!(Eax.Bits.SubLeafType & EPC_ENABLE)) {
      SGX_DEBUG ((SGX_D_ERROR, "  Error EPC Sub Leaf Type is not enabled\n"));
      return EFI_NOT_FOUND;
    }
    EpcBaseAddress = ((UINT64) Eax.Bits.LowAddressOfEpcSection << EPC_SHIFT_LOW) + ((UINT64) Ebx.Bits.HighAddressOfEpcSection << EPC_SHIFT_HIGH);
    EpcLength = ((UINT64) Ecx.Bits.LowSizeOfEpcSection << EPC_SHIFT_LOW) + ((UINT64) Edx.Bits.HighSizeOfEpcSection << EPC_SHIFT_HIGH);

    if ((EpcBaseAddress < mSgxInitDataHob->SgxPreMemInitHob.PrmrrBase[EpcIndex]) ||
        ((EpcBaseAddress + EpcLength) > (mSgxInitDataHob->SgxPreMemInitHob.PrmrrBase[EpcIndex] + mSecurityPolicy->PrmrrSize))) {
      SGX_DEBUG ((SGX_D_ERROR, "  Error EPC region outside PRMRR range\n"));
      return EFI_NOT_FOUND;
    }
    mAcpiParameter->EpcBaseAddress[EpcIndex] = EpcBaseAddress;
    mAcpiParameter->EpcLength[EpcIndex] = EpcLength;
    SGX_DEBUG ((SGX_D_ERROR, "EPC[%d], Base: %16lX, Length: %16lX\n", EpcIndex, EpcBaseAddress, EpcLength));
  }
  mAcpiParameter->SgxStatus = TRUE;
  return EFI_SUCCESS;
}

/*
 * This function calculates padding for MCHECK structures
 *
 * @param[in,out] UINT64     *Size
 * @param[in,out] UINT64     *Padding
 * @retval        EFI_STATUS Status
 */
EFI_STATUS
CalculateBiosParamInfoSubstructurePadding (
  IN  UINT64 *Size,
  OUT UINT64 *Padding
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if (Size == NULL || Padding == NULL) {
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }
  *Padding = (MCHECK_DEFAULT_ALIGNMENT - (*Size % MCHECK_DEFAULT_ALIGNMENT)) % MCHECK_DEFAULT_ALIGNMENT;

  return Status;
}
/**
  Function prepared policy for AllocateBiosParamInfo structure.

  @param[in, out] BiosParamInfoCreationPolicy    Pointer to the variable which contain policy
  @param[in]      SecurityPolicy                 Pointer to the Security policy to determine Security features
  @param[in]      FusaSafPolicy                  Pointer to the FuSa (SAF) plicy to determine Scan at field feature

  @retval EFI_SUCCESS     When the pointer to BiosParamInfoCreation is NOT NULL
  @retval EFI_NOT_FOUND   When the pointer to BiosParamInfoCreation is  NULL

 **/
EFI_STATUS
PopulateBiosParamInfoCreationPolicy (
  IN OUT UINT8           *BiosParamInfoCreationPolicy,
  IN     SECURITY_POLICY *SecurityPolicy,
  IN     FUSA_SAF_POLICY *FusaSafPolicy
  )
{
  if (BiosParamInfoCreationPolicy == NULL) {
    DEBUG ((EFI_D_ERROR, "%a: EFI_NOT_FOUND\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  // when only FuSa (SAF) is requested
  if (FusaSafPolicy->EnableSaf == SECURITY_POLICY_ENABLE) {
    *BiosParamInfoCreationPolicy |= FEATURES_FUSASAF;
  // if both FuSa (SAF) and SGX are requested thru DfxEnableSaf knob
  } else if (FusaSafPolicy->EnableSaf == SECURITY_POLICY_FUSA_SAF_SGX_ENABLED) {
    *BiosParamInfoCreationPolicy |= (FEATURES_FUSASAF | FEATURES_SGX);
  } else if (SecurityPolicy->EnableSgx == SECURITY_POLICY_ENABLE) {
    *BiosParamInfoCreationPolicy |= FEATURES_SGX;
  }

  if (SecurityPolicy->EnableTdx == SECURITY_POLICY_ENABLE) {
    *BiosParamInfoCreationPolicy |= FEATURES_SEAM;
  }


  DEBUG ((EFI_D_INFO, "%a: BiosParamInfoCreationPolicy = 0x%x\n", __FUNCTION__, *BiosParamInfoCreationPolicy));
  return EFI_SUCCESS;
}

/*
 * This function allocates memory for MCHECK_BIOS_PARAM_INFO and children buffers
 *
 * @param[in,out] MCHECK_BUFFER *BiosParamInfoBuffer
 * @param[in]     UINT8         BiosParamInfoCreationPolicy
 *
 * @retval        EFI_STATUS    Status
 */
EFI_STATUS
AllocateMcheckBiosParamInfo (
  MCHECK_BUFFER *BiosParamInfoBuffer,
  UINT8         BiosParamInfoCreationPolicy
  )
{
  EFI_STATUS Status        = EFI_SUCCESS;

  UINT32     Index         = 0;
  UINT64     Size          = 0;
  UINT64     TotalSize     = 0;
  UINT64     Padding       = 0;
  UINT64     NumberOfPages = 0;

  // SgxInfo
  UINT64     SgxInfoOffset             = 0;
  UINT64     RegistrationServerOffset  = 0;
  UINT64     PlatformManifestOffset    = 0;

  // SgxInfo->SocketData and its buffers
  UINT64     SocketDataOffsetArray[MCHECK_MAX_PACKAGES];
  UINT64     KeyBlobOffsetArray[MCHECK_MAX_PACKAGES];
  UINT64     CertificateOffsetArray[MCHECK_MAX_PACKAGES];

  // TopologyInfo
  UINT64     TopologyInfoOffset        = 0;
  UINT64     WorkingAreaOffset         = 0;

  // SeamInfo
  UINT64     SeamInfoOffset = 0;

  MCHECK_BIOS_PARAM_INFO *ParamInfoPtr    = NULL;
  MCHECK_SGX_INFO        *SgxInfoPtr      = NULL;
  MCHECK_SGX_SOCKET_DATA *SocketDataPtr   = NULL;
  MCHECK_TOPOLOGY_INFO   *TopologyInfoPtr = NULL;

  SGX_DEBUG ((SGX_D_ERROR, "[SGX] AllocateMcheckBiosParamInfo BEGIN\n"));

  if (BiosParamInfoBuffer == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit_PopulateMcheckBiosParamInfo;
  }

  if (BiosParamInfoBuffer->Address != NULL || BiosParamInfoBuffer->Size != 0) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit_PopulateMcheckBiosParamInfo;
  }

  ZeroMem (SocketDataOffsetArray, sizeof (UINT64) * MCHECK_MAX_PACKAGES);
  ZeroMem (KeyBlobOffsetArray, sizeof (UINT64) * MCHECK_MAX_PACKAGES);
  ZeroMem (CertificateOffsetArray, sizeof (UINT64) * MCHECK_MAX_PACKAGES);

  // ParamInfo
  Size      = sizeof (MCHECK_BIOS_PARAM_INFO);
  Status    = CalculateBiosParamInfoSubstructurePadding (&Size, &Padding);
  ASSERT_EFI_ERROR (Status);
  TotalSize = Size + Padding;

  // when SGX is requested create SgxInfo and add it to BiosParamInfo struct
  if (BiosParamInfoCreationPolicy & FEATURES_SGX) {
    SGX_DEBUG ((SGX_D_ERROR, "  %a: ParamInfo->SgxInfo\n", __FUNCTION__));
    // ParamInfo->SgxInfo
    SgxInfoOffset = TotalSize;
    Size          = sizeof (MCHECK_SGX_INFO);
    Status        = CalculateBiosParamInfoSubstructurePadding (&Size, &Padding);
    ASSERT_EFI_ERROR (Status);
    TotalSize     += Size + Padding;

    // ParamInfo->SgxInfo->RegistrationServer
    RegistrationServerOffset = TotalSize;
    Size                     = sizeof (MCHECK_SGX_REGISTRATION_SERVER_ID);
    Status                   = CalculateBiosParamInfoSubstructurePadding (&Size, &Padding);
    ASSERT_EFI_ERROR (Status);
    TotalSize                += Size + Padding;

    // ParamInfo->SgxInfo->PlatformManifest
    PlatformManifestOffset = TotalSize;
    Size                   = sizeof (MCHECK_SGX_PLATFORM_MANIFEST);
    Status                 = CalculateBiosParamInfoSubstructurePadding (&Size, &Padding);
    ASSERT_EFI_ERROR (Status);
    TotalSize              += Size + Padding;

    for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
      if (!SocketPresent (Index)) {
        continue;
      }
      // ParamInfo->SgxInfo->SocketData[Index]
      SocketDataOffsetArray[Index] = TotalSize;
      Size                   = sizeof (MCHECK_SGX_SOCKET_DATA);
      Status                 = CalculateBiosParamInfoSubstructurePadding (&Size, &Padding);
      ASSERT_EFI_ERROR (Status);
      TotalSize              += Size + Padding;

      // ParamInfo->SgxInfo->SocketData[Index]->KeyBlob
      KeyBlobOffsetArray[Index] = TotalSize;
      Size                   = sizeof (MCHECK_SGX_KEY_BLOB);
      Status                 = CalculateBiosParamInfoSubstructurePadding (&Size, &Padding);
      ASSERT_EFI_ERROR (Status);
      TotalSize              += Size + Padding;

      // ParamInfo->SgxInfo->SocketData[Index]->Certificate
      CertificateOffsetArray[Index] = TotalSize;
      Size                      = sizeof (MCHECK_SGX_PLATFORM_MEMBERSHIP_CERTIFICATE);
      Status                    = CalculateBiosParamInfoSubstructurePadding (&Size, &Padding);
      ASSERT_EFI_ERROR (Status);
      TotalSize                 += Size + Padding;
    }
  }

  // when SGX is requested create SeamInfo and add it to BiosParamInfo struct
  if (BiosParamInfoCreationPolicy & FEATURES_SEAM) {
    SGX_DEBUG ((SGX_D_ERROR, "  %a: ParamInfo->SeamInfo\n", __FUNCTION__));
    // ParamInfo->SeamInfo
    SeamInfoOffset = TotalSize;
    Size = sizeof (MCHECK_SEAM_INFO);
    Status = CalculateBiosParamInfoSubstructurePadding (&Size, &Padding);
    ASSERT_EFI_ERROR (Status);
    TotalSize += Size + Padding;
  }

  // ParamInfo->TopologyInfo
  TopologyInfoOffset = TotalSize;
  Size      = sizeof (MCHECK_TOPOLOGY_INFO);
  Status    = CalculateBiosParamInfoSubstructurePadding (&Size, &Padding);
  ASSERT_EFI_ERROR (Status);
  TotalSize += Size + Padding;

  // ParamInfo->TopologyInfo->WorkingArea
  if (mSocketCount > 1) {
    WorkingAreaOffset = TotalSize;
    Size      = 4096 * mSocketCount * mSocketCount; // 4K * NUM_SOCKETS * NUM_SOCKETS
    Status    = CalculateBiosParamInfoSubstructurePadding (&Size, &Padding);
    ASSERT_EFI_ERROR (Status);
    TotalSize += Size + Padding;
  }

  //
  // Allocate memory (whole pages) for BiosParamInfo structure
  //
  NumberOfPages = EFI_SIZE_TO_PAGES (TotalSize);
  ParamInfoPtr = AllocatePages (NumberOfPages);
  if (ParamInfoPtr == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    SGX_DEBUG ((SGX_D_ERROR, "  Error: Unable to allocate memory for BiosParamInfo mailbox\n"));
    return Status;
  }
  ZeroMem (ParamInfoPtr, EFI_PAGES_TO_SIZE (NumberOfPages));

  if (BiosParamInfoCreationPolicy & FEATURES_SGX) {
    //
    // Set-up pointers in MCHECK_BUFFER substructures
    //
    ParamInfoPtr->SgxInfo.Address = (UINT8 *)((UINT64)ParamInfoPtr + SgxInfoOffset);
    ParamInfoPtr->SgxInfo.Size    = sizeof (MCHECK_SGX_INFO);
    SgxInfoPtr                    = (MCHECK_SGX_INFO *)ParamInfoPtr->SgxInfo.Address;

    SgxInfoPtr->RegistrationServer.Address = (UINT8 *)((UINT64)ParamInfoPtr + RegistrationServerOffset);
    SgxInfoPtr->RegistrationServer.Size    = sizeof (MCHECK_SGX_REGISTRATION_SERVER_ID);

    SgxInfoPtr->PlatformManifest.Address   = (UINT8 *)((UINT64)ParamInfoPtr + PlatformManifestOffset);
    SgxInfoPtr->PlatformManifest.Size      = sizeof (MCHECK_SGX_PLATFORM_MANIFEST);

    for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
      if (SocketPresent (Index)) {
        SgxInfoPtr->SocketData[Index].Address = (UINT8 *)((UINT64)ParamInfoPtr + SocketDataOffsetArray[Index]);
        SgxInfoPtr->SocketData[Index].Size    = sizeof (MCHECK_SGX_SOCKET_DATA);
        SocketDataPtr                         = (MCHECK_SGX_SOCKET_DATA *)SgxInfoPtr->SocketData[Index].Address;

        SocketDataPtr->KeyBlob.Address = (UINT8 *)((UINT64)ParamInfoPtr + KeyBlobOffsetArray[Index]);
        SocketDataPtr->KeyBlob.Size    = sizeof (MCHECK_SGX_KEY_BLOB);

        SocketDataPtr->Certificate.Address = (UINT8 *)((UINT64)ParamInfoPtr + CertificateOffsetArray[Index]);
        SocketDataPtr->Certificate.Size    = sizeof (MCHECK_SGX_PLATFORM_MEMBERSHIP_CERTIFICATE);
      } else {
        SgxInfoPtr->SocketData[Index].Address = NULL;
        SgxInfoPtr->SocketData[Index].Size    = 0;
      }
    }
  }

  if (BiosParamInfoCreationPolicy & FEATURES_SEAM) {
    ParamInfoPtr->SeamInfo.Address     = (UINT8 *)((UINT64)ParamInfoPtr + SeamInfoOffset);
    ParamInfoPtr->SeamInfo.Size        = sizeof (MCHECK_SEAM_INFO);
  } else {
    ParamInfoPtr->SeamInfo.Address     = NULL;
    ParamInfoPtr->SeamInfo.Size        = 0;
  }

  ParamInfoPtr->TopologyInfo.Address = (UINT8 *)((UINT64)ParamInfoPtr + TopologyInfoOffset);
  ParamInfoPtr->TopologyInfo.Size    = sizeof (MCHECK_TOPOLOGY_INFO);
  TopologyInfoPtr                    = (MCHECK_TOPOLOGY_INFO *)ParamInfoPtr->TopologyInfo.Address;

  if (mSocketCount > 1) {
    TopologyInfoPtr->WorkingArea.Address = (UINT8 *)((UINT64)ParamInfoPtr + WorkingAreaOffset);
    TopologyInfoPtr->WorkingArea.Size    = 4096 * mSocketCount * mSocketCount;
  } else {
    TopologyInfoPtr->WorkingArea.Address = NULL;
    TopologyInfoPtr->WorkingArea.Size    = 0;
  }

  //
  // Save address and size of allocated structure
  //
  BiosParamInfoBuffer->Address = (UINT8 *)ParamInfoPtr;
  BiosParamInfoBuffer->Size    = EFI_PAGES_TO_SIZE (NumberOfPages);

Exit_PopulateMcheckBiosParamInfo:
  SGX_DEBUG ((SGX_D_ERROR, "[SGX] AllocateMcheckBiosParamInfo END\n"));
  return Status;
}
/*
 * This function deallocates memory for MCHECK_BIOS_PARAM_INFO and children buffers
 *
 * @param[in,out] MCHECK_BUFFER *BiosParamInfoBuffer
 * @retval        EFI_STATUS    Status
 */
EFI_STATUS
DeallocateMcheckBiosParamInfo (
  MCHECK_BUFFER *BiosParamInfoBuffer
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  SGX_DEBUG ((SGX_D_ERROR, "[SGX] DeallocateMcheckBiosParamInfo BEGIN\n"));

  if (BiosParamInfoBuffer == NULL) {
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  if (BiosParamInfoBuffer->Address != NULL && BiosParamInfoBuffer->Size > 0) {
    FreePages (BiosParamInfoBuffer->Address, EFI_SIZE_TO_PAGES (BiosParamInfoBuffer->Size));

    BiosParamInfoBuffer->Address = NULL;
    BiosParamInfoBuffer->Size    = 0;
  } else {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Verbose: BiosParamInfo already deallocated\n"));
  }

  SGX_DEBUG ((SGX_D_ERROR, "[SGX] DeallocateMcheckBiosParamInfo END\n"));
  return Status;
}

/*
 * Prepares BiosParamInfo mailbox for MCHECK-BIOS communication
 *
 * @param[in,out] MCHECK_BUFFER *BiosParamInfoBuffer
 * @retval        EFI_STATUS    Status
 */
EFI_STATUS
PrepareMcheckBiosParamInfo (
  MCHECK_BUFFER *BiosParamInfoBuffer
  )
{
  EFI_STATUS Status                      = EFI_SUCCESS;
  UINT8      BiosParamInfoCreationPolicy = 0;

  SGX_DEBUG ((SGX_D_ERROR, "[SGX] PrepareMcheckBiosParamInfo BEGIN\n"));

  if (BiosParamInfoBuffer == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit_PrepareMcheckBiosParamInfo;
  }

  if (BiosParamInfoBuffer->Address != NULL || BiosParamInfoBuffer->Size != 0) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: BiosParamInfoBuffer is already initialized\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit_PrepareMcheckBiosParamInfo;
  }

  if (mSecurityPolicy->DfxAllowTdxWithoutSgx != SECURITY_POLICY_DFX_AUTO) {
    // TDX DFX - Design for Test
    SGX_DEBUG ((SGX_D_VERBOSE, "  [TDX] DfxAllowTdxWithoutSgx - ParamInfo\n"));
  } else {

    Status = PopulateBiosParamInfoCreationPolicy (
      &BiosParamInfoCreationPolicy,
      mSecurityPolicy,
      mFusaSafPolicy
      );
    if (EFI_ERROR (Status) || BiosParamInfoCreationPolicy == 0) {
      SGX_DEBUG ((SGX_D_ERROR, "  Error: BiosParamInfoCreationPolicy is not initialized\n"));
      goto Exit_PrepareMcheckBiosParamInfo;
    }

    Status = AllocateMcheckBiosParamInfo (BiosParamInfoBuffer, BiosParamInfoCreationPolicy);
    if (EFI_ERROR (Status)) {
      SGX_DEBUG ((SGX_D_ERROR, "  Error: Cannot allocate memory for BiosParamInfoPtr\n"));
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit_PrepareMcheckBiosParamInfo;
    }

    Status = PopulateMcheckBiosParamInfo (BiosParamInfoBuffer, BiosParamInfoCreationPolicy);
    if (EFI_ERROR (Status)) {
      SGX_DEBUG ((SGX_D_ERROR, "  Error: Failed to PrepareSgxMcheckDataStructure exiting... (%r) \n", Status));
      Status = EFI_LOAD_ERROR;
      goto Exit_PrepareMcheckBiosParamInfo;
    }
  }

Exit_PrepareMcheckBiosParamInfo:
  SGX_DEBUG ((SGX_D_ERROR, "[SGX] PrepareMcheckBiosParamInfo END\n"));
  return Status;
}

/**

  Updates KeyBlobs in OutKeyBlobsBuffer from an MCHECK buffer

  @param[out] SGX_UEFIFW_KEY_BLOB    *OutKeyBlobsBuffer                        A pointer to KeyBlobs array
  @param[out] BOOLEAN                *OutKeyBlobsAreCreatedOrModifiedByMcheck  Set if MCHECK has generated any KeyBlob
  @param[in]  MCHECK_BIOS_PARAM_INFO *InBiosParamInfo                          A pointer to BiosParamInfo structure
  @param[in]  BOOLEAN                MktmeIntegrityEnabled

**/
EFI_STATUS
UpdateKeyBlobsFromBiosParamInfo (
  OUT SGX_UEFIFW_KEY_BLOB   *OutKeyBlobsBuffer,
  OUT BOOLEAN               *OutKeyBlobsAreCreatedOrModifiedByMcheck,
  IN MCHECK_BIOS_PARAM_INFO *InBiosParamInfo,
  IN BOOLEAN                MktmeIntegrityEnabled
  )
{
  EFI_STATUS             Status         = EFI_SUCCESS;
  MCHECK_SGX_SOCKET_DATA *SocketDataPtr = NULL;
  UINT8                  Index;

  SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] %a BEGIN\n", __FUNCTION__));

  if (OutKeyBlobsBuffer == NULL ||
      OutKeyBlobsAreCreatedOrModifiedByMcheck == NULL ||
      InBiosParamInfo == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit_UpdateKeyBlobsFromBiosParamInfo;
  }

  for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
    SocketDataPtr = ((MCHECK_SGX_INFO *)InBiosParamInfo->SgxInfo.Address)->SocketData[Index].Address;
    SGX_DEBUG ((SGX_D_INFO, "SocketData[%d]->Flags = %d\n", Index, SocketDataPtr->Flags));

    if (SocketDataPtr == NULL) {
      SGX_DEBUG ((SGX_D_INFO, "  SocketData uninitialized for Socket = %d\n", Index));
      continue;
    }

    if (SocketPresent (Index) && (SocketDataPtr->Flags & FLAGS_BLOB_OUT_BIT)) {
      //
      // Copy Key Blobs from BiosParamInfo to SGX Hob
      // for present sockets only, with flag indicating that MCHECK modified it
      //
      SGX_DEBUG ((SGX_D_INFO, "  Migrate KeyBlob for Socket: %d\n", Index));
      CopyMem (&OutKeyBlobsBuffer[Index].KeyBlob,
               SocketDataPtr->KeyBlob.Address,
               sizeof (MCHECK_SGX_KEY_BLOB));
      // Mark that key blob is present for this SocketId
      OutKeyBlobsBuffer[Index].Present   = TRUE;
      // Set integrity according to current system setting
      OutKeyBlobsBuffer[Index].Integrity = MktmeIntegrityEnabled;
      // Mark that KeyBlob was created/modified by MCHECK
      *OutKeyBlobsAreCreatedOrModifiedByMcheck = TRUE;
    }
  }

Exit_UpdateKeyBlobsFromBiosParamInfo:
  SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] %a END\n", __FUNCTION__));
  return Status;
}

/*
  Simics only Second x64 uCode Callback

  @param[in] EFI_EVENT Event    synchronization event that was signalled and now needs to be closed
  @param[in] VOID      *Context optional context that can given to the callback
 */
VOID
EFIAPI
SecondLoaduCodeCallback (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  )
{
  SGX_DEBUG ((SGX_D_ERROR, "  Enter SecondLoaduCodeCallback\n"));
  gBS->CloseEvent (Event);
}

EFI_STATUS
SecondLoaduCode (
  VOID                       *BiosParamInfoPtr
  )
{
  EFI_STATUS    Status       = EFI_SUCCESS;
  BOOLEAN       IsSimicsEnv  = IsSimicsEnvironment();
  UINT32        Thread;
  EFI_EVENT     EventSecondLoaduCode;
  UINTN         EventIndex;
  if (BiosParamInfoPtr == NULL) {
    SGX_DEBUG ((SGX_D_ERROR, "Error: BiosParamInfoPtr is NULL\n"));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  //
  //
  // Load uCode second time for SGX (Package scope) to launch MCHECK
  // Currently there is no microcode for SPR
  //
  if (MsrTraceEnabled ()) {
    SGX_DEBUG ((SGX_D_ERROR, "SGX Late MSR Trace Start\n"));
    SetMsrTraceOutputMode (MSR_OUTPUT_ENABLE);
  }
  if (IsSimicsEnv) {
    gBS->RestoreTPL (TPL_APPLICATION);

    Status = gBS->CreateEvent (
                    EVT_NOTIFY_WAIT,
                    TPL_CALLBACK,
                    SecondLoaduCodeCallback,
                    NULL,
                    &EventSecondLoaduCode
                    );
    ASSERT_EFI_ERROR (Status);
  }


  Status = mMpServices->StartupAllAPs (
    mMpServices,
    (EFI_AP_PROCEDURE) SgxMicrocodeLaunch,
    FALSE,
    IsSimicsEnv ? &EventSecondLoaduCode : NULL,
    0,
    (VOID *) BiosParamInfoPtr,
    NULL
    );
  if (Status == EFI_NOT_STARTED) {
    SGX_DEBUG ((SGX_D_ERROR, "  APs not started, continue...\n"));
  } else if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: Failed SgxMicrocodeLaunch (%r), exiting...\n", Status));
  }
  SgxMicrocodeLaunch (BiosParamInfoPtr);

  if (IsSimicsEnv) {
    // Status from StartupAllAPs
    if (Status != EFI_NOT_STARTED) {
      Status = gBS->WaitForEvent (1, &EventSecondLoaduCode, &EventIndex);
    }
  }

  for (Thread = 0; Thread < MAX_CPU_NUM; Thread++) {
    if (mMcuErrorCode[Thread]) { // SGX ucode returned with failure
      SGX_DEBUG ((SGX_D_ERROR, "[SGX] MCHECK error socket[%d] msr 0xA0 = %08X\n", Thread, mMcuErrorCode[Thread]));
      Status = EFI_LOAD_ERROR;
      goto Error_SecondLoaduCode;
    }
  }
Error_SecondLoaduCode:
  return Status;
}


EFI_STATUS
ActivateSgx (
  VOID
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN      Thread;

  ZeroMem (mMcuErrorCode, sizeof(mMcuErrorCode));
  //
  // Activate SGX all threads (actually Core scope)
  //
  SgxActivate ();
  Status = mMpServices->StartupAllAPs (
    mMpServices,
    (EFI_AP_PROCEDURE) SgxActivate,
    FALSE,
    NULL,
    0,
    NULL,
    NULL
    );

  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error running ActivateSgx on all threads (%r), exiting...\n", Status));
  }

  for (Thread = 0; Thread < MAX_CPU_NUM; Thread++) {
    if (mMcuErrorCode[Thread]) {
      SerialPrintBlocking (SGX_D_ERROR, " [SGX] Thread[%2X] SE bit in MSR 0x7A was not set!\n", Thread);
      Status = EFI_LOAD_ERROR;
    }
  }

  return Status;
}


/**

  SGX late init flow variable set helper function

  Variable will be re-created if it's changed from R/W to R/O

  @param SgxRegistrationConfig

  @retval Status
    @retval EFI_SUCCESS
            EFI_INVALID_PARAMETER
            EFI_SECURITY_VIOLATION

**/
EFI_STATUS
SetRegistrationVariableHelper (
  IN  CHAR16   *VariableName,
  IN  EFI_GUID *VendorGuid,
  IN  UINT32   Attributes,
  IN  UINTN    DataSize,
  IN  VOID     *Data,
  IN  BOOLEAN  ReadOnly
  )
{
  EFI_STATUS Status             = EFI_SUCCESS;
  UINTN      VariableSize       = DataSize;
  UINT32     ExistingAttributes = 0;
  VOID       *ExistingVariable  = NULL;

  // Check input parameters
  if (VariableName == NULL ||
      VendorGuid == NULL ||
      (Data == NULL && DataSize != 0)) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit_SetRegistrationVariableHelper;
  }

  // Step 1. Allocate temporary buffer for existing variable
  Status = gBS->AllocatePool (
    EfiBootServicesData,
    VariableSize,
    (VOID **) &ExistingVariable
    );
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error allocating buffer\n"));
    goto Exit_SetRegistrationVariableHelper;
  }

  // Step 2. Try getting the variable using gRT
  Status = gRT->GetVariable (
    VariableName,
    VendorGuid,
    &ExistingAttributes,
    &VariableSize,
    ExistingVariable
    );
  if (Status == EFI_SUCCESS) {
    // Step 3. If variable already exists check it's attributes
    if (ExistingAttributes != Attributes) {
      // Step 3a. Delete variable with different attributes
      Status = gRT->SetVariable (
        VariableName,
        VendorGuid,
        0,
        0,
        NULL
        );
      if (EFI_ERROR (Status)) {
        SGX_DEBUG ((SGX_D_ERROR, "  Error: Failed to delete variable - SetVariable - %s\n", VariableName));
        goto Exit_SetRegistrationVariableHelper;
      }
    } else {
      SGX_DEBUG ((SGX_D_VERBOSE, "  Variable attributes exist & match\n"));
    }
  }

  // Step 4. Deallocate temporary buffer for existing variable
  if (ExistingVariable != NULL) {
    gBS->FreePool (ExistingVariable);
  }

  // Step 5. Try setting the variable using gRT
  Status = gRT->SetVariable (
      VariableName,
      VendorGuid,
      Attributes,
      DataSize,
      Data
    );

  if (DataSize == 0 && Status == EFI_NOT_FOUND) {
    // Step 6. Handle removing variable scenario
    Status = EFI_SUCCESS;
    goto Exit_SetRegistrationVariableHelper;
  } else if (EFI_ERROR (Status)) {
    // Print error on failure
    SGX_DEBUG ((SGX_D_ERROR, "  Error: Failed to SetVariable - %s\n", VariableName));
  }

Exit_SetRegistrationVariableHelper:
  return Status;
}


/**

  SGX late init flow variable set helper function

  @param SetRegistrationProtocolVariableHelper

  @retval Status
    @retval EFI_SUCCESS
            EFI_INVALID_PARAMETER
            EFI_SECURITY_VIOLATION

**/
EFI_STATUS
SetRegistrationProtocolVariableHelper (
  IN  CHAR16                       *VariableName,
  IN  EFI_GUID                     *VendorGuid,
  IN  UINT32                       Attributes,
  IN  UINTN                        DataSize,
  IN  VOID                         *Data
  )
{
  EFI_STATUS Status;

  if (Data == NULL && DataSize != 0) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gRT->SetVariable (
      VariableName,
      VendorGuid,
      Attributes,
      DataSize,
      Data
    );
  if (DataSize == 0 && Status == EFI_NOT_FOUND) {
    return EFI_SUCCESS;
  }
  if (EFI_ERROR (Status)) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}


/**

  Stores KeyBlobs in an internal NVRAM variable.

  @param[in] SGX_UEFIFW_KEY_BLOB *InKeyBlobs           Input buffer, source of keyblobs
  @param[in] UINTN               InKeyBlobsSize        Size of InUefiFwKeyBlobsArray
  @param[in] BOOLEAN             MktmeIntegrityEnabled

  @return EFI_STATUS  A value of SetRegistrationVariableHelper

**/
EFI_STATUS
StoreKeyBlobsInNvram (
  IN OUT SGX_UEFIFW_KEY_BLOB *InUefiFwKeyBlobs,
  IN     UINTN               InUefiFwKeyBlobsSize,
  IN     BOOLEAN             MktmeIntegrityEnabled
  )
{
  EFI_STATUS Status       = EFI_SUCCESS;
  BOOLEAN    ReadOnly     = FALSE;

  SGX_DEBUG ((SGX_D_ERROR, "[SGX] %a BEGIN\n", __FUNCTION__));

  if (InUefiFwKeyBlobs == NULL) {
    SGX_DEBUG ((SGX_D_ERROR, "  Invalid input parameter!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit_StoreKeyBlobsInNvram;
  }

  if (MktmeIntegrityEnabled) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  MKTME integrity bit is set, KeyBlobs with integrity will get stored in NVRAM.\n"));
  }
  Status = SetRegistrationVariableHelper (
             MktmeIntegrityEnabled ? SGX_UEFIFW_KEY_BLOB_INTEGRITY_VARIABLE_NAME : SGX_UEFIFW_KEY_BLOB_VARIABLE_NAME,
             MktmeIntegrityEnabled ? &gSgxUefiFwKeyBlobsWithIntegrityVariableGuid : &gSgxUefiFwKeyBlobsVariableGuid,
             EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
             InUefiFwKeyBlobsSize,
             InUefiFwKeyBlobs,
             ReadOnly
             );
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: Unable to write %s, SetRegistrationVariableHelper returned %r\n",
                SGX_UEFIFW_KEY_BLOB_VARIABLE_NAME,
                Status
                ));
  } else {
    SGX_DEBUG ((SGX_D_VERBOSE, "  KeyBlobs were successfully written into %s NVRAM variable.\n",
                SGX_UEFIFW_KEY_BLOB_VARIABLE_NAME
                ));
  }

  SGX_DEBUG ((SGX_D_ERROR, "[SGX] %a END\n", __FUNCTION__));
Exit_StoreKeyBlobsInNvram:
  return Status;
}


EFI_STATUS
UpdateTimeStampOfKeyBlobs (
  IN     MCHECK_BIOS_PARAM_INFO *BiosParamInfoPtr,
  IN OUT SGX_INIT_DATA_HOB      *SgxInitDataHob,
  IN     SGX_UEFI_BOOT_SCENARIO BootScenario
  )
{
  EFI_TIME               Time;
  UINT8                  Index          = 0;
  EFI_STATUS             Status         = EFI_SUCCESS;
  MCHECK_SGX_SOCKET_DATA *SocketDataPtr = NULL;

  Status = gRT->GetTime (&Time, NULL);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  switch (BootScenario) {
    case SgxAddPackageReboot:
      // Override TimeStamp from existing KeyBlob
      for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
        SocketDataPtr = ((MCHECK_SGX_INFO *)BiosParamInfoPtr->SgxInfo.Address)->SocketData[Index].Address;
        if (SocketPresent (Index) &&
            SgxInitDataHob->UefiFwKeyBlobs[Index].Present &&
            SocketDataPtr != NULL &&
            SocketDataPtr->Flags & FLAGS_BLOB_IN_BIT) {
          // Take time stamp from KeyBlob that existed before AddPackage flow
          CopyMem (
            &Time,
            &SgxInitDataHob->UefiFwKeyBlobs[Index].TimeStamp,
            sizeof (EFI_TIME)
            );
          SGX_DEBUG ((SGX_D_ERROR, "Time is set to TimeStamp of KeyBlob [%d]\n", Index));
          break; // Take the first existing TimeStamp
        }
        SocketDataPtr = NULL;
      }
    case SgxFirstBinding:
    case SgxTcbRecovery:
      for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
        SocketDataPtr = ((MCHECK_SGX_INFO *)BiosParamInfoPtr->SgxInfo.Address)->SocketData[Index].Address;
        if (SocketPresent (Index) &&
            SgxInitDataHob->UefiFwKeyBlobs[Index].Present &&
            SocketDataPtr != NULL &&
            SocketDataPtr->Flags & FLAGS_BLOB_OUT_BIT) {
          // Copy TimeStamp to updated KeyBlobs
          CopyMem (
            &SgxInitDataHob->UefiFwKeyBlobs[Index].TimeStamp,
            &Time,
            sizeof (EFI_TIME)
            );
          SGX_DEBUG ((SGX_D_ERROR, "Updated TimeStamp for KeyBlob [%d]\n", Index));
        }
        SocketDataPtr = NULL;
      }
      break;
    default:
      break;
  }

  return Status;
}

//
// Store Platform Manifest in NVRAM
// size w/o Pairing Receipt Trimming - PRT
//

EFI_STATUS
StorePlatformManifestInNvram (
  SGX_UEFI_REGISTRATION_PLATFORM_MANIFEST *PlatformManifestPtr
  )
{
  BOOLEAN ReadOnly = FALSE;

  return SetRegistrationVariableHelper (
      SGX_UEFIFW_PLATFORM_MANIFEST_VARIABLE_NAME,
      &gSgxUefiFwPlatformManifestVariableGuid,
      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
      sizeof (SGX_UEFI_REGISTRATION_PLATFORM_MANIFEST),
      PlatformManifestPtr,
      ReadOnly
    );
}


/**

  Set Sgx Data Structure Header Helper

  @param IN SGX_HEADER *SgxDataStructureHeader
  @param IN EFI_GUID   *Uuid
  @param IN UINTN      StructureSize
  @param IN UINTN      StructureVersion

**/
VOID
SetSgxHeader (
  IN MCHECK_HEADER *SgxDataStructureHeader,
  IN EFI_GUID   *Uuid,
  IN UINT16     StructureSize,
  IN UINT16     StructureVersion
  )
{
  CopyMem (SgxDataStructureHeader->Uuid.Uuid, Uuid, sizeof (MCHECK_UUID));
  SgxDataStructureHeader->Size = StructureSize - sizeof (MCHECK_HEADER);
  SgxDataStructureHeader->Version = StructureVersion;
}

/**

  SGX late init flow variable get helper function

  @param   IN     CHAR16      *VariableName,
  @param   IN     EFI_GUID    *VendorGuid,
  @param   OUT    UINT32      *Attributes,
  @param   IN OUT UINTN       DataSize,
  @param   OUT    VOID        *Data

  @retval Status
    @retval EFI_SUCCESS
            EFI_INVALID_PARAMETER
            EFI_SECURITY_VIOLATION

**/
EFI_STATUS
GetRegistrationVariableHelper (
  IN     CHAR16      *VariableName,
  IN     EFI_GUID    *VendorGuid,
  OUT    UINT32      *Attributes,
  IN OUT UINTN       DataSize,
  OUT    VOID        *Data
  )
{
  EFI_STATUS    Status;
  UINTN         VariableSize = DataSize;

  if (Data == NULL) {
      return EFI_INVALID_PARAMETER;
  }

  Status = gRT->GetVariable (
      VariableName,
      VendorGuid,
      Attributes,
      &VariableSize,
      (VOID *)Data
    );
  if (Status == EFI_NOT_FOUND) {
    SGX_DEBUG ((SGX_D_INFO, "  GetVariableHelper - %s not found in NVRAM, continue\n", VariableName));
    ZeroMem (Data, VariableSize);
    return Status;
  } else if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: GetVariableHelper - %s, error = (%r)\n", VariableName, Status));
    return Status;
  }

  if (VariableSize != DataSize) {
    // If DataSize is too small to hold variable EFI_BUFFER_TOO_SMALL is returned and *DataSize is updated with expected size.
    // Here we use a renamed VariableSize internally to keep DataSize as helper Api alligned with UEFI Spec.
    SGX_DEBUG ((SGX_D_ERROR, "  Error: GetVariableHelper - %s, returned size different than input buffer (%r)\n",
      VariableName, Status));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  SGX_DEBUG ((SGX_D_INFO, "  GetVariableHelper - %s found in NVRAM\n", VariableName));

  return EFI_SUCCESS;
}

//
// Read Platform Manifest from NVRAM
//
EFI_STATUS
GetPlatformManifestFromNvram (
  OUT SGX_UEFI_REGISTRATION_PLATFORM_MANIFEST *SgxPlatformManifest
  )
{
  EFI_STATUS                              Status;

  Status = GetRegistrationVariableHelper (
    SGX_UEFIFW_PLATFORM_MANIFEST_VARIABLE_NAME,
    &gSgxUefiFwPlatformManifestVariableGuid,
    NULL,
    sizeof (SGX_UEFI_REGISTRATION_PLATFORM_MANIFEST),
    SgxPlatformManifest
    );

  return Status;
}

/**


  @param SgxRegistrationConfig

  @retval Status
    @retval EFI_SUCCESS
            EFI_INVALID_PARAMETER

**/
EFI_STATUS
GetRegistrationConfig (
  SGX_REGISTRATION_CONFIG *SgxRegistrationConfig
  )
{
  EFI_STATUS Status;

  Status = GetRegistrationVariableHelper (
    SGX_REGISTRATION_CONFIG_VARIABLE_NAME,
    &gSgxRegistrationConfigVariableGuid,
    NULL,
    sizeof (SGX_REGISTRATION_CONFIG),
    (VOID *)SgxRegistrationConfig
    );
  if (EFI_ERROR (Status)) {
    Status = GetRegistrationVariableHelper (
      SGX_UEFI_REGISTRATION_CONFIG_VARIABLE_NAME,
      &gSgxUefiRegistrationConfigVariableGuid,
      NULL,
      sizeof (SGX_REGISTRATION_CONFIG),
      (VOID *)SgxRegistrationConfig
      );
  }

  return Status;
}

/**


  @param SgxRegistrationConfig

  @retval Status
    @retval EFI_SUCCESS
            EFI_INVALID_PARAMETER
            EFI_SECURITY_VIOLATION

**/
EFI_STATUS
EFIAPI
SetRegistrationConfig (
  IN SGX_REGISTRATION_CONFIG *SgxRegistrationConfig
  )
{
  EFI_STATUS Status;

  Status = SetRegistrationProtocolVariableHelper (
    SGX_REGISTRATION_CONFIG_VARIABLE_NAME,
    &gSgxRegistrationConfigVariableGuid,
    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
    sizeof (SGX_REGISTRATION_CONFIG),
    SgxRegistrationConfig
    );
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "  [SGX] Error: SetRegistrationConfig (%s) status %r\n",SGX_REGISTRATION_CONFIG_VARIABLE_NAME, Status));
  }
  Status = SetRegistrationProtocolVariableHelper (
    SGX_UEFI_REGISTRATION_CONFIG_VARIABLE_NAME,
    &gSgxUefiRegistrationConfigVariableGuid,
    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
    sizeof (SGX_REGISTRATION_CONFIG),
    SgxRegistrationConfig
    );

  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "  [SGX] Error: SetRegistrationConfig (%s) status %r\n",SGX_REGISTRATION_CONFIG_VARIABLE_NAME, Status));
    return EFI_INVALID_PARAMETER;
  }

  return Status;
}

/**


  @param SgxRegistrationRequest

  @retval Status
  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
GetRegistrationRequest (
  SGX_REGISTRATION_REQUEST *SgxRegistrationRequest
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT64 DataSize   = mSgxInitDataHob->RegistrationState.NonVolatile.RegistrationRequestSize;

  Status = GetRegistrationVariableHelper (
            SGX_UEFI_REGISTRATION_REQUEST_VARIABLE_NAME,
            &gSgxUefiRegistrationServerRequestVariableGuid,
            NULL,
            DataSize,
            (VOID *)SgxRegistrationRequest
            );

  return Status;
}

/**


  @param SgxRegistrationResponse

  @retval Status
    @retval EFI_SUCCESS
            EFI_INVALID_PARAMETER
            EFI_SECURITY_VIOLATION

**/
EFI_STATUS
EFIAPI
SetRegistrationResponse (
  IN SGX_REGISTRATION_RESPONSE *SgxRegistrationResponse
  )
{
  EFI_STATUS Status;

  Status = SetRegistrationProtocolVariableHelper (
    SGX_REGISTRATION_RESPONSE_VARIABLE_NAME,
    &gSgxRegistrationServerResponseVariableGuid,
    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
    sizeof (SGX_REGISTRATION_RESPONSE),
    SgxRegistrationResponse
    );

  if (EFI_ERROR(Status)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = SetRegistrationProtocolVariableHelper (
    SGX_UEFI_REGISTRATION_RESPONSE_VARIABLE_NAME,
    &gSgxUefiRegistrationServerResponseVariableGuid,
    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
    sizeof (SGX_REGISTRATION_RESPONSE),
    SgxRegistrationResponse
    );

  return Status;
}

/**

  @param SgxRegistrationResponse

  @retval Status
    @retval EFI_SUCCESS

**/
EFI_STATUS
GetRegistrationResponse (
  SGX_REGISTRATION_RESPONSE *SgxRegistrationResponse
  )
{
  EFI_STATUS    Status;

  Status = GetRegistrationVariableHelper (
      SGX_REGISTRATION_RESPONSE_VARIABLE_NAME,
      &gSgxRegistrationServerResponseVariableGuid,
      NULL,
      sizeof (SGX_REGISTRATION_RESPONSE),
      (VOID *)SgxRegistrationResponse
      );

  return Status;
}

/**


  @param SgxRegistrationStatus

  @retval Status
    @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
GetRegistrationStatus (
  SGX_REGISTRATION_STATUS *SgxRegistrationStatus
  )
{
  EFI_STATUS    Status;

  Status = GetRegistrationVariableHelper (
      SGX_REGISTRATION_STATUS_VARIABLE_NAME,
      &gSgxRegistrationStatusVariableGuid,
      NULL,
      sizeof (SGX_REGISTRATION_STATUS),
      (VOID *)SgxRegistrationStatus
      );

  return Status;
}


/**


  @param SgxRegistrationStatus

  @retval Status
    @retval EFI_SUCCESS
            EFI_INVALID_PARAMETER
            EFI_SECURITY_VIOLATION

**/
EFI_STATUS
EFIAPI
SetRegistrationStatus (
  IN SGX_REGISTRATION_STATUS *SgxRegistrationStatus
  )
{
  EFI_STATUS Status;

  Status = SetRegistrationProtocolVariableHelper (
      SGX_REGISTRATION_STATUS_VARIABLE_NAME,
      &gSgxRegistrationStatusVariableGuid,
      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
      sizeof (SGX_REGISTRATION_STATUS),
      SgxRegistrationStatus
      );
  return Status;
}

/**
  StoreUefiFwRegistrationStateVariable


  @retval Status
    @retval EFI_SUCCESS

**/
EFI_STATUS
StoreUefiFwRegistrationStateVariable (
  VOID
  )
{
  EFI_STATUS Status;
  BOOLEAN    ReadOnly = FALSE;
  UINTN      SgxUefiFwRegistrationStateVariableSize = sizeof (SGX_UEFIFW_NONVOLATILVE_REGISTRATION_STATE);

  Status = SetRegistrationVariableHelper (
      SGX_UEFIFW_NONVOLATILVE_REGISTRATION_STATE_VARIABLE_NAME,
      &gSgxUefiFwRegistrationStateVariableGuid,
      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
      SgxUefiFwRegistrationStateVariableSize,
      (VOID *)&mSgxInitDataHob->RegistrationState.NonVolatile,
      ReadOnly
    );
  if ((Status != EFI_NOT_FOUND) && (Status != EFI_SUCCESS)) {
    ASSERT (FALSE);
  }
  return Status;
}

/**
  Disable SGX factory reset knob value

  @retval Status
    @retval EFI_SUCCESS           - Installed successfully
    @retval Others                - Failed to update

**/
EFI_STATUS
DisableSgxFactoryReset (
  VOID
  )
{
  EFI_STATUS                           Status = EFI_SUCCESS;
  UINTN                                VariableSize = 0;
  SOCKET_PROCESSORCORE_CONFIGURATION   SocketProcessorCoreConfig;
  UINT32                               SetupAttr;

  VariableSize = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);
  Status = gRT->GetVariable (
    SOCKET_PROCESSORCORE_CONFIGURATION_NAME,
    &gEfiSocketProcessorCoreVarGuid,
    &SetupAttr,
    &VariableSize,
    &SocketProcessorCoreConfig
    );
  if (EFI_ERROR(Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] DisableSgxFactoryReset exit: Failed get variable,  error: %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  SocketProcessorCoreConfig.SgxFactoryReset = SECURITY_POLICY_DISABLE;

  Status = gRT->SetVariable (
    SOCKET_PROCESSORCORE_CONFIGURATION_NAME,
    &gEfiSocketProcessorCoreVarGuid,
    SetupAttr,
    VariableSize,
    &SocketProcessorCoreConfig
    );
  if (EFI_ERROR(Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] DisableSgxFactoryReset exit: Failed set variable,  error: %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Update setup buffer with changed values
  //
  Status = HiiSetBrowserData (&gEfiSocketProcessorCoreVarGuid, SOCKET_PROCESSORCORE_CONFIGURATION_NAME, VariableSize, (UINT8 *) &SocketProcessorCoreConfig, NULL);
  if (EFI_ERROR(Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] DisableSgxFactoryReset exit: Failed update HII Browser buffer,  error: %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  SGX_DEBUG ((SGX_D_VERBOSE, "  [SGX] DisableSgxFactoryReset exit (%r)\n", Status));
  return Status;
}

/**

  Performs partial SGX reset which is a part of first platform binding flow

  @retval Status
    @retval EFI_SUCCESS

**/
EFI_STATUS
PerformPartialReset ()
{
  EFI_STATUS Status;
  BOOLEAN    CryptoStatus = FALSE;

  // Delete SGX_UEFIFW_KEY_BLOB
  Status = SetRegistrationVariableHelper (
      SGX_UEFIFW_KEY_BLOB_VARIABLE_NAME,
      &gSgxUefiFwKeyBlobsVariableGuid,
      0,
      0,
      NULL,
      FALSE
    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete SGX_UEFIFW_KEY_BLOB with integrity
  Status = SetRegistrationVariableHelper (
      SGX_UEFIFW_KEY_BLOB_INTEGRITY_VARIABLE_NAME,
      &gSgxUefiFwKeyBlobsWithIntegrityVariableGuid,
      0,
      0,
      NULL,
      FALSE
    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete SGX_REGISTRATION_PACKAGE_INFO
  Status = SetRegistrationVariableHelper (
      SGX_REGISTRATION_PACKAGE_INFO_VARIABLE_NAME,
      &gSgxRegistrationPackageInfoVariableGuid,
      0,
      0,
      NULL,
      FALSE
    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete SGX_REGISTRATION_PACKAGE_INFO with integrity
  Status = SetRegistrationVariableHelper (
      SGX_REGISTRATION_PACKAGE_INFO_INTEGRITY_VARIABLE_NAME,
      &gSgxRegistrationPackageInfoWithIntegrityVariableGuid,
      0,
      0,
      NULL,
      FALSE
    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete SGX_REGISTRATION_REQUEST
  Status = SetRegistrationVariableHelper (
      SGX_REGISTRATION_REQUEST_VARIABLE_NAME,
      &gSgxRegistrationServerRequestVariableGuid,
      0,
      0,
      NULL,
      FALSE
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete Backup SGX_REGISTRATION_REQUEST
  Status = SetRegistrationVariableHelper (
      SGX_UEFI_REGISTRATION_REQUEST_VARIABLE_NAME,
      &gSgxUefiRegistrationServerRequestVariableGuid,
      0,
      0,
      NULL,
      FALSE
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete SGX_REGISTRATION_RESPONSE
  Status = SetRegistrationVariableHelper (
      SGX_REGISTRATION_RESPONSE_VARIABLE_NAME,
      &gSgxRegistrationServerResponseVariableGuid,
      0,
      0,
      NULL,
      FALSE
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete Backup SGX_REGISTRATION_RESPONSE
  Status = SetRegistrationVariableHelper (
      SGX_UEFI_REGISTRATION_RESPONSE_VARIABLE_NAME,
      &gSgxUefiRegistrationServerResponseVariableGuid,
      0,
      0,
      NULL,
      FALSE
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Update config variable hash to new value calculated from OS provided data to avoid SgxFactoryReset deadloop
  CryptoStatus = Sha256HashAll ((UINT8 *)&mInternalRegistrationConfig,
                                sizeof (SGX_REGISTRATION_CONFIG),
                                mInternalRegState->NonVolatile.RegistrationConfigHash);
  if (CryptoStatus == FALSE) {
    Status = EFI_SECURITY_VIOLATION;
  }
  mInternalRegState->NonVolatile.RegistrationConfigHashPresent = TRUE;
  return Status;
}

/**

  Resets SGX variables
  Removes key blobs and registration variables

  @retval Status
    @retval EFI_SUCCESS

**/
EFI_STATUS
DeleteSgxVariables (
  )
{
  EFI_STATUS Status;

  // Delete SGX_UEFIFW_KEY_BLOB
  Status = SetRegistrationVariableHelper (
      SGX_UEFIFW_KEY_BLOB_VARIABLE_NAME,
      &gSgxUefiFwKeyBlobsVariableGuid,
      0,
      0,
      NULL,
      FALSE
    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  // Delete SGX_UEFIFW_KEY_BLOB with integity
  Status = SetRegistrationVariableHelper (
      SGX_UEFIFW_KEY_BLOB_INTEGRITY_VARIABLE_NAME,
      &gSgxUefiFwKeyBlobsWithIntegrityVariableGuid,
      0,
      0,
      NULL,
      FALSE
    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete SGX_UEFIFW_NONVOLATILVE_REGISTRATION_STATE
  mInternalRegState->NonVolatile.SwAgentFirstBindingComplete = FALSE;
  Status = SetRegistrationVariableHelper (
      SGX_UEFIFW_NONVOLATILVE_REGISTRATION_STATE_VARIABLE_NAME,
      &gSgxUefiFwRegistrationStateVariableGuid,
      0,
      0,
      NULL,
      FALSE
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete SGX_UEFIFW_PLATFORM_MANIFEST
  Status = SetRegistrationVariableHelper (
      SGX_UEFIFW_PLATFORM_MANIFEST_VARIABLE_NAME,
      &gSgxUefiFwPlatformManifestVariableGuid,
      0,
      0,
      NULL,
      FALSE
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete SGX_REGISTRATION_PACKAGE_INFO
  Status = SetRegistrationVariableHelper (
      SGX_REGISTRATION_PACKAGE_INFO_VARIABLE_NAME,
      &gSgxRegistrationPackageInfoVariableGuid,
      0,
      0,
      NULL,
      FALSE
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete SGX_REGISTRATION_PACKAGE_INFO with Integrity
  Status = SetRegistrationVariableHelper (
      SGX_REGISTRATION_PACKAGE_INFO_INTEGRITY_VARIABLE_NAME,
      &gSgxRegistrationPackageInfoWithIntegrityVariableGuid,
      0,
      0,
      NULL,
      FALSE
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete SGX_REGISTRATION_REQUEST
  Status = SetRegistrationVariableHelper (
      SGX_REGISTRATION_REQUEST_VARIABLE_NAME,
      &gSgxRegistrationServerRequestVariableGuid,
      0,
      0,
      NULL,
      FALSE
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete Backup SGX_REGISTRATION_REQUEST
  Status = SetRegistrationVariableHelper (
      SGX_UEFI_REGISTRATION_REQUEST_VARIABLE_NAME,
      &gSgxUefiRegistrationServerRequestVariableGuid,
      0,
      0,
      NULL,
      FALSE
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete SGX_REGISTRATION_CONFIG
  Status = SetRegistrationVariableHelper (
      SGX_REGISTRATION_CONFIG_VARIABLE_NAME,
      &gSgxRegistrationConfigVariableGuid,
      0,
      0,
      NULL,
      FALSE
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete Backup SGX_REGISTRATION_CONFIG
  Status = SetRegistrationVariableHelper (
      SGX_UEFI_REGISTRATION_CONFIG_VARIABLE_NAME,
      &gSgxUefiRegistrationConfigVariableGuid,
      0,
      0,
      NULL,
      FALSE
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete SGX_LEGACY_REGISTRATION_SW_GUARD_STATUS
  Status = SetRegistrationVariableHelper (
      SGX_LEGACY_REGISTRATION_SW_GUARD_STATUS_VARIABLE_NAME,
      &gSgxLegacyRegistrationSoftwareGuardStatusVariableGuid,
      0,
      0,
      NULL,
      FALSE
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete SGX_LEGACY_REGISTRATION_EPC_BIOS
  Status = SetRegistrationVariableHelper (
      SGX_LEGACY_REGISTRATION_EPC_BIOS_VARIABLE_NAME,
      &gSgxLegacyRegistrationEpcBiosVariableGuid,
      0,
      0,
      NULL,
      FALSE
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete SGX_LEGACY_REGISTRATION_EPC_SW
  Status = SetRegistrationVariableHelper (
      SGX_LEGACY_REGISTRATION_EPC_SW_VARIABLE_NAME,
      &gSgxLegacyRegistrationEpcSwVariableGuid,
      0,
      0,
      NULL,
      FALSE
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete SGX_REGISTRATION_STATUS
  Status = SetRegistrationVariableHelper (
      SGX_REGISTRATION_STATUS_VARIABLE_NAME,
      &gSgxRegistrationStatusVariableGuid,
      0,
      0,
      NULL,
      FALSE
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete Backup SGX_REGISTRATION_STATUS
  Status = SetRegistrationVariableHelper (
      SGX_UEFI_REGISTRATION_STATUS_VARIABLE_NAME,
      &gSgxUefiRegistrationStatusVariableGuid,
      0,
      0,
      NULL,
      FALSE
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete SGX_REGISTRATION_RESPONSE
  Status = SetRegistrationVariableHelper (
      SGX_REGISTRATION_RESPONSE_VARIABLE_NAME,
      &gSgxRegistrationServerResponseVariableGuid,
      0,
      0,
      NULL,
      FALSE
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Delete Backup SGX_REGISTRATION_RESPONSE
  Status = SetRegistrationVariableHelper (
      SGX_UEFI_REGISTRATION_RESPONSE_VARIABLE_NAME,
      &gSgxUefiRegistrationServerResponseVariableGuid,
      0,
      0,
      NULL,
      FALSE
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Zero and R/W REGISTRATION_STATUS
  ZeroMem (&mInternalRegistrationStatus, sizeof (SGX_REGISTRATION_STATUS));
  return Status;
}

VOID
SetRegistrationServerAddress (
  OUT  SGX_REGISTRATION_CONFIG     *SgxRegistrationConfig
  )
{
    // Set SBX Registration Server Url
    SgxRegistrationConfig->SgxRegServerInfo.UrlSize = sizeof (MCHECK_SGX_SBX_SERVER_URL) - 1;
    CopyMem (SgxRegistrationConfig->SgxRegServerInfo.Url,
            MCHECK_SGX_SBX_SERVER_URL,
            sizeof (MCHECK_SGX_SBX_SERVER_URL) - 1);
}

/**
  Create Registration Config

  There is no longer need that MCHECK will generate the SERVER_ID
  This responsibility has been moved to BIOS
  So there is no need to update the variable buffer from MCHECK BiosParam

  @param OUT  SGX_REGISTRATION_CONFIG     *SgxRegistrationConfig

  @retval EFI_STATUS
**/
EFI_STATUS
CreateRegistrationConfig (
  OUT  SGX_REGISTRATION_CONFIG     *SgxRegistrationConfig
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;

  SGX_DEBUG ((SGX_D_INFO, "  [SGX] Create RegistrationConfiguration from defaults.\n"));
  //
  //   RegistrationConfig.Header
  SgxRegistrationConfig->Header.Version = MCHECK_HEADER_DEFAULT_VERSION;
  SgxRegistrationConfig->Header.Size = sizeof (SGX_REGISTRATION_CONFIG) - sizeof (SGX_REGISTRATION_HEADER);

  //   RegistrationConfig.Flags.RSsavesEncryptedKeys
  SgxRegistrationConfig->Flags.RSsavesEncryptedKeys = mSecurityPolicy->SgxAutoRegistrationAgent ? RS_SAVES_ENCRYPTED_KEYS_TRUE : RS_SAVES_ENCRYPTED_KEYS_FALSE;

  //   RegistrationConfig.SgxRegServerInfo
  SetSgxHeader (
    &SgxRegistrationConfig->SgxRegServerInfo.Header,
    &gSgxRegistrationServerInfoGuid,
    sizeof (SGX_REGISTRATION_SERVER_INFO),
    SGX_REGISTRATION_HEADER_VERSION
    );

  // Set RegistrationServerUrl from the hardcoded values
  SetRegistrationServerAddress (SgxRegistrationConfig);
  // Set RegistrationServerId from the hardcoded values
  PopulateMcheckSgxRegistrationServerId (&SgxRegistrationConfig->SgxRegServerInfo.RegServerId);

  return Status;
}

/**
  Update Registration Config Flags

  @param[in]      SgxAutoRegistrationAgent enable/disable SgxAutoRegistrationAgent policy
  @param[in, out] SgxRegistrationConfig    Pointer to SGX_REGISTRATION_CONFIG structure
  @param[out]     UpdateRegConfigHash      Pointer to variable

  @retval None
**/
VOID
UpdateRegistrationConfigFlags (
  IN     UINT8                       SgxAutoRegistrationAgent,
  IN OUT SGX_REGISTRATION_CONFIG     *SgxRegistrationConfig,
  OUT    BOOLEAN                     *UpdateRegConfigHash
  )
{
  SGX_DEBUG ((SGX_D_INFO, "  [SGX] Update UpdateRegistrationConfigFlags\n"));
  // Exclusion logic, SgxAutoRegistrationAgent and RSsavesEncryptedKeys must have opposite value
  if (((SgxAutoRegistrationAgent == 0) && (mInternalRegistrationConfig.Flags.RSsavesEncryptedKeys == RS_SAVES_ENCRYPTED_KEYS_TRUE)) ||
      ((SgxAutoRegistrationAgent == 1) && (mInternalRegistrationConfig.Flags.RSsavesEncryptedKeys == RS_SAVES_ENCRYPTED_KEYS_FALSE))) {
    *UpdateRegConfigHash = TRUE;
    mInternalRegistrationConfig.Flags.RSsavesEncryptedKeys = SgxAutoRegistrationAgent ? RS_SAVES_ENCRYPTED_KEYS_TRUE : RS_SAVES_ENCRYPTED_KEYS_FALSE;
  }
}

/**
  Update Platform Manifest from MCHECK to an internal buffer

  @param[in] *OutPlatformManifest pointer to PlatformManifest internal buffer
  @param[in] *BiosParamInfo       pointer to BiosParamInfo structure set after MCHECK launch
  @param     BootScenario         current SGX_UEFI_BOOT_SCENARIO

  @retval None
**/
EFI_STATUS
UpdatePlatformManifestFromuCodeToInternalBuffer (
  IN OUT SGX_UEFI_REGISTRATION_PLATFORM_MANIFEST *OutPlatformManifest,
  IN     MCHECK_BIOS_PARAM_INFO                  *BiosParamInfo,
         SGX_UEFI_BOOT_SCENARIO                  BootScenario
  )
{
  EFI_STATUS                   Status              = EFI_SUCCESS;
  MCHECK_SGX_PLATFORM_MANIFEST *InPlatformManifest = NULL;

  if (OutPlatformManifest == NULL ||
      BiosParamInfo == NULL) {
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }
  InPlatformManifest =
    (MCHECK_SGX_PLATFORM_MANIFEST *)((MCHECK_SGX_INFO *)BiosParamInfo->SgxInfo.Address)->PlatformManifest.Address;

  OutPlatformManifest->Header.Version = SGX_REGISTRATION_MANIFEST_HEADER_VERSION;
  OutPlatformManifest->Header.Size    = sizeof (MCHECK_SGX_PLATFORM_MANIFEST);

  switch (BootScenario) {
    case SgxFirstBinding:
      CopyMem (&OutPlatformManifest->McheckPlatformManifestIpe,
              InPlatformManifest,
              sizeof (MCHECK_SGX_PLATFORM_MANIFEST));
      break;
    case SgxTcbRecovery:
      CopyMem (&OutPlatformManifest->McheckPlatformManifestTr,
              InPlatformManifest,
              sizeof (MCHECK_SGX_PLATFORM_MANIFEST));
      break;
    default:
      Status = EFI_INVALID_PARAMETER;
      break;
  }

  return Status;
}

/**
  Trim Registration Manifest and in result the whole Registration Request

  @param[out] SGX_REGISTRATION_REQUEST                *TrimmedRegistrationRequest     resulting buffer
  @param[out] UINT64                                  TrimmedRequestSize              resulting size
  @param[out] SGX_UEFI_REGISTRATION_PLATFORM_MANIFEST UefiPlatformManifest input UEFI manifest

  @retval Status
  @retval EFI_SUCCESS           Trimmed successfully
  @retval EFI_INVALID_PARAMETER RegistrationRequestType is invalid

**/
EFI_STATUS
TrimManifestAndRegistrationRequest (
  IN OUT SGX_REGISTRATION_REQUEST                *TrimmedRegistrationRequest,
  IN OUT UINT64                                  *TrimmedRequestSize,
  IN     SGX_UEFI_REGISTRATION_PLATFORM_MANIFEST *UefiPlatformManifest
  )
{
  EFI_STATUS Status              = EFI_UNSUPPORTED;
  UINT8      *DstAddr            = NULL;
  UINT8      *SrcAddr            = NULL;

  UINTN      NumOfIpePackages    = 0;
  UINTN      NumOfTrPackages     = 0;

  UINTN      IpeTrimSize         = 0;
  UINTN      TrTrimSize          = 0;

  UINTN      HeaderSize          = sizeof (SGX_REGISTRATION_HEADER);
  UINTN      RequestSize         = sizeof (SGX_UEFI_REGISTRATION_PLATFORM_MANIFEST);

  UINTN      ManifestSize           = sizeof (MCHECK_SGX_PLATFORM_MANIFEST);
  UINTN      TrimmedIpeManifestSize = 0;
  UINTN      TrimmedTrManifestSize  = 0;

  SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] %a BEGIN\n", __FUNCTION__));

  if (TrimmedRegistrationRequest == NULL ||
      UefiPlatformManifest == NULL ||
      TrimmedRequestSize == NULL
    )
  {
    Status = EFI_INVALID_PARAMETER;
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  ZeroMem (TrimmedRegistrationRequest, sizeof (SGX_REGISTRATION_REQUEST));
  SGX_DEBUG ((SGX_D_ERROR, "  RegistrationRequestType %d, %d\n",
              mInternalRegState->NonVolatile.RegistrationRequestType,
              mSgxInitDataHob->RegistrationState.NonVolatile.RegistrationRequestType));

  if (mInternalRegState->NonVolatile.RegistrationRequestType == SgxRequestPlatformManifest) {
    // Calculate Trimmed Manifest Size
    NumOfIpePackages = UefiPlatformManifest->McheckPlatformManifestIpe.PlatformInfo.PackageCount;
    NumOfTrPackages  = UefiPlatformManifest->McheckPlatformManifestTr.PlatformInfo.PackageCount;

    if (NumOfTrPackages == 0) {
      NumOfTrPackages = NumOfIpePackages;
      SGX_DEBUG ((SGX_D_INFO, "  Info: Changed NumOfTrPackages to 0x%x\n", NumOfTrPackages));
    }

    IpeTrimSize      = (MCHECK_MAX_PACKAGES - NumOfIpePackages) * sizeof (MCHECK_SGX_PAIRING_RECEIPT);
    TrTrimSize       = (MCHECK_MAX_PACKAGES - NumOfTrPackages) * sizeof (MCHECK_SGX_PAIRING_RECEIPT);

    if (ManifestSize < IpeTrimSize || ManifestSize < TrTrimSize) {
      Status = EFI_UNSUPPORTED;
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
    TrimmedIpeManifestSize = ManifestSize - IpeTrimSize;
    TrimmedTrManifestSize  = ManifestSize - TrTrimSize;

    // Copy header
    DstAddr = (UINT8 *)&TrimmedRegistrationRequest->PlatformManifest.Header;
    SrcAddr = (UINT8 *)&UefiPlatformManifest->Header;
    CopyMem (DstAddr, SrcAddr, HeaderSize);
    TrimmedRegistrationRequest->PlatformManifest.Header.Size = (UINT16)(TrimmedIpeManifestSize + TrimmedTrManifestSize);

    // Copy first Ipe manifest trimmed
    DstAddr = (UINT8 *)&TrimmedRegistrationRequest->PlatformManifest.McheckPlatformManifestIpe;
    SrcAddr = (UINT8 *)&UefiPlatformManifest->McheckPlatformManifestIpe;
    CopyMem (DstAddr, SrcAddr, TrimmedIpeManifestSize);

    // Copy second Tr manifest trimmed just after Ipe manifest
    DstAddr = (UINT8 *)&TrimmedRegistrationRequest->PlatformManifest.McheckPlatformManifestIpe + TrimmedIpeManifestSize;
    SrcAddr = (UINT8 *)&UefiPlatformManifest->McheckPlatformManifestTr;
    CopyMem (DstAddr, SrcAddr, TrimmedTrManifestSize);

    // Update size
    *TrimmedRequestSize = RequestSize - (IpeTrimSize + TrTrimSize);
    ASSERT (*TrimmedRequestSize == HeaderSize + (TrimmedIpeManifestSize + TrimmedTrManifestSize));

    // Return success
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_ABORTED;
  }

  SGX_DEBUG ((SGX_D_INFO, "  NumOfIpePackages = 0x%x\n", NumOfIpePackages));
  SGX_DEBUG ((SGX_D_INFO, "  NumOfTrPackages  = 0x%x\n\n", NumOfTrPackages));

  SGX_DEBUG ((SGX_D_INFO, "  IpeTrimSize  = 0x%x\n", IpeTrimSize));
  SGX_DEBUG ((SGX_D_INFO, "  TrTrimSize   = 0x%x\n\n", TrTrimSize));

  SGX_DEBUG ((SGX_D_INFO, "  ManifestSize           = 0x%x\n", ManifestSize));
  SGX_DEBUG ((SGX_D_INFO, "  TrimmedIpeManifestSize = 0x%x\n", TrimmedIpeManifestSize));
  SGX_DEBUG ((SGX_D_INFO, "  TrimmedTrManifestSize  = 0x%x\n\n", TrimmedTrManifestSize));

  SGX_DEBUG ((SGX_D_INFO, "  RequestSize        = 0x%x\n", RequestSize));
  SGX_DEBUG ((SGX_D_INFO, "  TrimmedRequestSize = 0x%x\n", TrimmedRequestSize));

  SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] %a END\n", __FUNCTION__));
  return Status;
}

EFI_STATUS
UpdateRegistrationRequest (
  IN OUT SGX_REGISTRATION_REQUEST                *SgxRegistrationRequest,
  IN     SGX_UEFI_REGISTRATION_PLATFORM_MANIFEST *SgxPlatformManifest,
  IN     SGX_INIT_DATA_HOB                       *SgxInitDataHob
  )
{
  EFI_STATUS Status   = EFI_SUCCESS;
  UINT8      Uuid[]   = SGX_REGISTRATION_ADD_REQUEST_UUID;
  UINT8      Index    = 0;
  UINT8      NumPrid  = 0;
  UINT64     DataSize = 0;

  switch (mSgxInitDataHob->RegistrationState.NonVolatile.RegistrationRequestType) {
    case SgxRequestNone:
      Status = EFI_INVALID_PARAMETER;
      break;
    case SgxRequestPlatformManifest:
      ZeroMem (SgxRegistrationRequest, sizeof (SGX_REGISTRATION_REQUEST));
      Status = TrimManifestAndRegistrationRequest (
                SgxRegistrationRequest,
                &DataSize,
                SgxPlatformManifest
                );
      mSgxInitDataHob->RegistrationState.NonVolatile.RegistrationRequestSize = DataSize;
      break;
    case SgxRequestAddRequest:
      SgxRegistrationRequest->AddPackage.Header.Version = SGX_REGISTRATION_HEADER_VERSION;
      SgxRegistrationRequest->AddPackage.Header.Size = sizeof (SGX_REGISTRATION_ADD_REQUEST);
      ZeroMem (&SgxRegistrationRequest->AddPackage.AddRequest, sizeof (SGX_REGISTRATION_ADD_REQUEST));
      CopyMem (&SgxRegistrationRequest->AddPackage.AddRequest.Header.Uuid, Uuid, sizeof (MCHECK_UUID));
      SgxRegistrationRequest->AddPackage.AddRequest.Header.Size = sizeof (SGX_REGISTRATION_ADD_REQUEST) - sizeof (MCHECK_HEADER);
      SgxRegistrationRequest->AddPackage.AddRequest.Header.Version = SGX_REGISTRATION_HEADER_VERSION;
      SgxRegistrationRequest->AddPackage.AddRequest.PlatformInstanceId =
        SgxPlatformManifest->McheckPlatformManifestIpe.PlatformInfo.PlatformInstanceId;
      SgxRegistrationRequest->AddPackage.AddRequest.CpuSvn = SgxInitDataHob->BootingSvn;

      for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
        if (SocketPresent (Index) && SgxInitDataHob->Socket[Index].AddPackageNew) {
          SgxRegistrationRequest->AddPackage.AddRequest.Prid[NumPrid].Uuid64[0] = SgxInitDataHob->Socket[Index].Prids_0;
          SgxRegistrationRequest->AddPackage.AddRequest.Prid[NumPrid].Uuid64[1] = SgxInitDataHob->Socket[Index].Prids_1;
          NumPrid++;
        }
      }
      SgxRegistrationRequest->AddPackage.AddRequest.NumPackages = NumPrid;
      mSgxInitDataHob->RegistrationState.NonVolatile.RegistrationRequestSize = sizeof (SGX_UEFI_REGISTRATION_ADD_PACKAGE);
      break;
    default:
      Status = EFI_INVALID_PARAMETER;
      ASSERT (FALSE);
      break;
  }

  return Status;
}

EFI_STATUS
UpdateRegistrationPackageInfo (
  IN   SGX_INIT_DATA_HOB             *SgxInitDataHob,
  OUT  SGX_REGISTRATION_PACKAGE_INFO *SgxRegistrationPackageInfo
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  UINT8                         Index;

  //
  //   SgxRegistrationPackageInfo.Header
  SgxRegistrationPackageInfo->Header.Version = SGX_REGISTRATION_HEADER_VERSION;
  SgxRegistrationPackageInfo->Header.Size = sizeof (SGX_REGISTRATION_PACKAGE_INFO) - sizeof (SGX_REGISTRATION_HEADER);
  //   SgxRegistrationPackageInfo.KeyBlobs
  for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
    if (SocketPresent (Index)) {
      //
      // Copy Key Blobs from InitDataHob to SgxRegistrationPackageInfo
      // for present sockets only, with flag indicating that MCHECK modified it
      //

      CopyMem (&SgxRegistrationPackageInfo->KeyBlobs[Index],
               &SgxInitDataHob->UefiFwKeyBlobs[Index].KeyBlob,
               sizeof (MCHECK_SGX_KEY_BLOB));
    }
  }
  return Status;
}


EFI_STATUS
UpdateRegistrationStatus (
  SGX_REGISTRATION_STATUS *SgxRegistrationStatus
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  //   RegistrationStatus.Header
  SgxRegistrationStatus->Header.Version = MCHECK_HEADER_DEFAULT_VERSION;
  SgxRegistrationStatus->Header.Size = sizeof (SGX_REGISTRATION_STATUS) - sizeof (SGX_REGISTRATION_HEADER);

  if (mInternalRegState->RequestPending) {
    // Update only for Pending == True, only SW Agent can set to False
    SgxRegistrationStatus->Status.SgxRegistrationComplete = !mInternalRegState->RequestPending;
  }
  if (mInternalRegState->PackageInfoPending) {
    // Update only for Pending == True, only SW Agent can set to False
    SgxRegistrationStatus->Status.SgxPackageInfoReadComplete = !mInternalRegState->PackageInfoPending;
  }
  return Status;
}

EFI_STATUS
ExposeLegacySwGuardStatusUefiVariable (
  VOID
  )
{
  SOFTWARE_GUARD_STATUS    SgxPolicyStatus;
  BOOLEAN                  ReadOnly = FALSE;

  ZeroMem (&SgxPolicyStatus, sizeof (SOFTWARE_GUARD_STATUS));
  SgxPolicyStatus.SgxStatus = mSecurityPolicy->EnableSgx;

  return SetRegistrationVariableHelper (
      SGX_LEGACY_REGISTRATION_SW_GUARD_STATUS_VARIABLE_NAME,
      &gSgxLegacyRegistrationSoftwareGuardStatusVariableGuid,
      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
      sizeof (SOFTWARE_GUARD_STATUS),
      &SgxPolicyStatus,
      ReadOnly
      );
}


EFI_STATUS
ExposeLegacyEpcBiosUefiVariable (
  VOID
  )
{
  BOOLEAN                                              ReadOnly = TRUE;
  UINT64                                               SupportedPrmBins;
  UINT64                                               PrmSize;
  UINT8                                                Index;
  UINT8                                                BinVal;
  EPC_BIOS_CONFIG                                      EpcBiosConfig;
  CPUID_INTEL_SGX_CAPABILITIES_0_SUB_LEAF_EAX          SgxCapabilitiesEax;
  UINT32                                               SgxCapabilitiesEbx;
  CPUID_INTEL_SGX_CAPABILITIES_0_SUB_LEAF_EDX          SgxCapabilitiesEdx;
  CPUID_INTEL_SGX_CAPABILITIES_RESOURCES_SUB_LEAF_EAX  SgxResourcesEax;
  CPUID_INTEL_SGX_CAPABILITIES_RESOURCES_SUB_LEAF_EBX  SgxResourcesEbx;
  CPUID_INTEL_SGX_CAPABILITIES_RESOURCES_SUB_LEAF_ECX  SgxResourcesEcx;
  CPUID_INTEL_SGX_CAPABILITIES_RESOURCES_SUB_LEAF_EDX  SgxResourcesEdx;

  ZeroMem (&EpcBiosConfig, sizeof (EPC_BIOS_CONFIG));

  ///
  /// Presence of SGX feature is detectable by reading CPUID.(EAX=0x12, ECX=0x0):EAX[0]
  ///
  AsmCpuidEx (
    CPUID_INTEL_SGX,
    CPUID_INTEL_SGX_CAPABILITIES_0_SUB_LEAF,
    &SgxCapabilitiesEax.Uint32,
    &SgxCapabilitiesEbx,
    NULL,
    &SgxCapabilitiesEdx.Uint32
    );

  ///
  /// If SGX was not activated
  ///
  if (SgxCapabilitiesEax.Bits.SGX1 == 0) {
    SGX_DEBUG ((SGX_D_INFO, "ExposeLegacyEpcBiosUefiVariable: SGX feature is not present, setting AllocatedEpcSize = 0\n"));
    EpcBiosConfig.AllocatedEpcSize = 0;
  } else {
    //
    // Read CPUID.(EAX=0x12, ECX=0x2) to get EPC Base and Size
    //
    AsmCpuidEx (
      CPUID_INTEL_SGX, CPUID_INTEL_SGX_CAPABILITIES_RESOURCES_SUB_LEAF,
      &SgxResourcesEax.Uint32,
      &SgxResourcesEbx.Uint32,
      &SgxResourcesEcx.Uint32,
      &SgxResourcesEdx.Uint32
      );

    //
    // Check if the first sub-leaf is a valid EPC section
    //
    if ((SgxResourcesEax.Bits.SubLeafType) != 0x1) {
      EpcBiosConfig.AllocatedEpcSize = 0;
    } else {
      ///
      /// If SGX was enabled, this variable will hold the EPC length as it was read from CPUID SGX sub-leaf
      /// Else this variable will hold the value 0
      ///
      EpcBiosConfig.AllocatedEpcSize = (UINT32) (UINTN) (LShiftU64 ((UINT64) (SgxResourcesEdx.Bits.HighSizeOfEpcSection), 32) + LShiftU64 ((UINT64) (SgxResourcesEcx.Bits.LowSizeOfEpcSection), 12));
    }
  }

  SupportedPrmBins = AsmReadMsr64 (SPR_MSR_PRMRR_VALID_CONFIG);
  EpcBiosConfig.SupportedPrmBins = (UINT32) (UINTN) SupportedPrmBins;

  ///
  /// Traverse over all SupportedPrmBins
  ///
  for (Index = 0, PrmSize = 1; Index < sizeof (EpcBiosConfig.EpcMap) / sizeof (EpcBiosConfig.EpcMap[0]); Index++) {
    EpcBiosConfig.EpcMap[Index] = 0;

    ///
    /// No More supported PRM size
    ///
    if (SupportedPrmBins == 0) {
      continue;
    }
    BinVal = SupportedPrmBins & BIT0;
    if (Index != 0) {
      ///
      /// Current PRM size in MB
      ///
      PrmSize = LShiftU64 (PrmSize, 1);
    }
    SupportedPrmBins = RShiftU64 (SupportedPrmBins, 1);

    if (BinVal == 0) {
      continue;
    } else {
      SGX_DEBUG ((SGX_D_INFO, "ExposeLegacyEpcBiosUefiVariable: Supported PRM size = %d MB\n", PrmSize));
      EpcBiosConfig.EpcMap[Index] = (UINT32) PrmSize;
      SGX_DEBUG ((SGX_D_INFO, "ExposeLegacyEpcBiosUefiVariable: EpcBiosConfig.EpcMap[%d] = 0x%ll016X MB\n", Index, EpcBiosConfig.EpcMap[Index]));
      EpcBiosConfig.MaxEpcSize = EpcBiosConfig.EpcMap[Index];
    }
  }

  SGX_DEBUG ((SGX_D_INFO, "ExposeLegacyEpcBiosUefiVariable: Final MaxEpcSize = 0x%ll016X MB\n", EpcBiosConfig.MaxEpcSize));

  return SetRegistrationVariableHelper (
      SGX_LEGACY_REGISTRATION_EPC_BIOS_VARIABLE_NAME,
      &gSgxLegacyRegistrationEpcBiosVariableGuid,
      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
      sizeof (EPC_BIOS_CONFIG),
      &EpcBiosConfig,
      ReadOnly
      );
}


VOID
ExposeVariablesDebug (
  IN  CHAR16                       *VariableName,
  IN  EFI_STATUS                   Status,
  IN  BOOLEAN                      ExposeToOsRuntime,
  IN  BOOLEAN                      ReadOnlyOsRuntime
  )
{
  SGX_DEBUG ((SGX_D_ERROR, "Expose variable [%s]:\n",   VariableName));
  SGX_DEBUG ((SGX_D_ERROR, "        SetVariable: %r\n", Status));
  SGX_DEBUG ((SGX_D_ERROR, "  ExposeToOsRuntime: %d\n", ExposeToOsRuntime));
  SGX_DEBUG ((SGX_D_ERROR, "  ReadOnlyOsRuntime: %d\n", ReadOnlyOsRuntime));
}

/**


  @retval Status
    @retval EFI_SUCCESS

**/
EFI_STATUS
ExposeAllScenariosSgxRegistrationUefiVariables (
  IN BOOLEAN                       SgxEnabled,
  IN BOOLEAN                       RequestPending,
  IN BOOLEAN                       PackageInfoPending,
  IN BOOLEAN                       PackageInfoInBandAccess,
  IN SGX_REGISTRATION_REQUEST      *SgxRegistrationRequest,
  IN SGX_REGISTRATION_CONFIG       *SgxRegistrationConfig,
  IN SGX_REGISTRATION_PACKAGE_INFO *SgxRegistrationPackageInfo,
  IN SGX_REGISTRATION_STATUS       *SgxRegistrationStatus
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  BOOLEAN                       ExposeToOsRuntime;
  BOOLEAN                       ReadOnlyOsRuntime;
  UINT64                        DataSize = 0;

  SGX_DEBUG ((SGX_D_ERROR, "ExposeAllScenariosSgxRegistrationUefiVariables Enter\n"));

  //
  // Registration Config
  //
  ExposeToOsRuntime = TRUE;
  ReadOnlyOsRuntime = SgxEnabled;
  Status = SetRegistrationVariableHelper (
      SGX_REGISTRATION_CONFIG_VARIABLE_NAME,
      &gSgxRegistrationConfigVariableGuid,
      (ReadOnlyOsRuntime ? 0 : EFI_VARIABLE_NON_VOLATILE) | EFI_VARIABLE_BOOTSERVICE_ACCESS | (ExposeToOsRuntime ? EFI_VARIABLE_RUNTIME_ACCESS : 0),
      sizeof (SGX_REGISTRATION_CONFIG),
      (VOID *)SgxRegistrationConfig,
      ReadOnlyOsRuntime
      );
  if (EFI_ERROR(Status)) {
    goto Error_ExposeAllUefiVariables;
  }
  Status = SetRegistrationVariableHelper (
      SGX_UEFI_REGISTRATION_CONFIG_VARIABLE_NAME,
      &gSgxUefiRegistrationConfigVariableGuid,
      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
      sizeof (SGX_REGISTRATION_CONFIG),
      (VOID *)SgxRegistrationConfig,
      FALSE // Read/Write
      );
  if (EFI_ERROR(Status)) {
    goto Error_ExposeAllUefiVariables;
  }
  ExposeVariablesDebug (
      SGX_REGISTRATION_CONFIG_VARIABLE_NAME,
      Status,
      ExposeToOsRuntime,
      ReadOnlyOsRuntime
      );

  //
  // Registration Request
  //
  ExposeToOsRuntime = PcdGetBool(PcdSgxPrivacy) ? RequestPending : TRUE;
  ReadOnlyOsRuntime = TRUE;
  SGX_DEBUG ((SGX_D_ERROR, "RegistrationRequestType %d\n", mInternalRegState->NonVolatile.RegistrationRequestType));
  DataSize = mSgxInitDataHob->RegistrationState.NonVolatile.RegistrationRequestSize;
  switch (mInternalRegState->NonVolatile.RegistrationRequestType) {
    case SgxRequestNone:
      break;
    case SgxRequestPlatformManifest:
    case SgxRequestAddRequest:
      Status = SetRegistrationVariableHelper (
          SGX_REGISTRATION_REQUEST_VARIABLE_NAME,
          &gSgxRegistrationServerRequestVariableGuid,
          (ReadOnlyOsRuntime ? 0 : EFI_VARIABLE_NON_VOLATILE) | EFI_VARIABLE_BOOTSERVICE_ACCESS | (ExposeToOsRuntime ? EFI_VARIABLE_RUNTIME_ACCESS : 0),
          DataSize,
          (VOID *)SgxRegistrationRequest,
          ReadOnlyOsRuntime
          );
      if (EFI_ERROR(Status)) {
        goto Error_ExposeAllUefiVariables;
      }
      Status = SetRegistrationVariableHelper (
          SGX_UEFI_REGISTRATION_REQUEST_VARIABLE_NAME,
          &gSgxUefiRegistrationServerRequestVariableGuid,
          EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
          DataSize,
          (VOID *)SgxRegistrationRequest,
          FALSE // Read/Write
          );
      break;
    default:
      Status = EFI_INVALID_PARAMETER;
      ASSERT (FALSE);
      break;
  }
  if (EFI_ERROR(Status)) {
    goto Error_ExposeAllUefiVariables;
  }
  ExposeVariablesDebug (
      SGX_REGISTRATION_REQUEST_VARIABLE_NAME,
      Status,
      ExposeToOsRuntime,
      ReadOnlyOsRuntime
      );

  //
  // Registration Package Info
  ExposeToOsRuntime = (!SgxEnabled && PackageInfoInBandAccess) ||
                      (SgxEnabled && (PcdGetBool (PcdSgxPrivacy) ? (PackageInfoInBandAccess && PackageInfoPending) : PackageInfoInBandAccess));
  ReadOnlyOsRuntime = SgxEnabled;
  Status = SetRegistrationVariableHelper (
      (mSgxInitDataHob->MktmeIntegrityEnabled ? SGX_REGISTRATION_PACKAGE_INFO_INTEGRITY_VARIABLE_NAME : SGX_REGISTRATION_PACKAGE_INFO_VARIABLE_NAME),
      (mSgxInitDataHob->MktmeIntegrityEnabled ? &gSgxRegistrationPackageInfoWithIntegrityVariableGuid : &gSgxRegistrationPackageInfoVariableGuid),
      (ReadOnlyOsRuntime ? 0 : EFI_VARIABLE_NON_VOLATILE) | EFI_VARIABLE_BOOTSERVICE_ACCESS | (ExposeToOsRuntime ? EFI_VARIABLE_RUNTIME_ACCESS : 0),
      sizeof (SGX_REGISTRATION_PACKAGE_INFO),
      (VOID *)SgxRegistrationPackageInfo,
      ReadOnlyOsRuntime
      );
  if (EFI_ERROR(Status)) {
    goto Error_ExposeAllUefiVariables;
  }
  ExposeVariablesDebug (
      (mSgxInitDataHob->MktmeIntegrityEnabled ? SGX_REGISTRATION_PACKAGE_INFO_INTEGRITY_VARIABLE_NAME : SGX_REGISTRATION_PACKAGE_INFO_VARIABLE_NAME),
      Status,
      ExposeToOsRuntime,
      ReadOnlyOsRuntime
      );

  //
  // Registration Status
  //
  ExposeToOsRuntime = TRUE;
  ReadOnlyOsRuntime = !RequestPending && !PackageInfoPending;
  Status = SetRegistrationVariableHelper (
      SGX_REGISTRATION_STATUS_VARIABLE_NAME,
      &gSgxRegistrationStatusVariableGuid,
      (ReadOnlyOsRuntime ? 0 : EFI_VARIABLE_NON_VOLATILE) | EFI_VARIABLE_BOOTSERVICE_ACCESS | (ExposeToOsRuntime ? EFI_VARIABLE_RUNTIME_ACCESS : 0),
      sizeof (SGX_REGISTRATION_STATUS),
      (VOID *)SgxRegistrationStatus,
      ReadOnlyOsRuntime
      );
  if (EFI_ERROR(Status)) {
    goto Error_ExposeAllUefiVariables;
  }
  Status = SetRegistrationVariableHelper (
      SGX_UEFI_REGISTRATION_STATUS_VARIABLE_NAME,
      &gSgxUefiRegistrationStatusVariableGuid,
      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
      sizeof (SGX_REGISTRATION_STATUS),
      (VOID *)SgxRegistrationStatus,
      FALSE // Read/Write
      );
  if (EFI_ERROR(Status)) {
    goto Error_ExposeAllUefiVariables;
  }
  ExposeVariablesDebug (
      SGX_REGISTRATION_STATUS_VARIABLE_NAME,
      Status,
      ExposeToOsRuntime,
      ReadOnlyOsRuntime
      );

  SGX_DEBUG ((SGX_D_ERROR, "  Success to expose Registration Variables, exiting...\n"));
  return Status;

Error_ExposeAllUefiVariables:
  SGX_DEBUG ((SGX_D_ERROR, "  Error: Failed to expose Registration Variables, exiting...\n"));
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**

  Exposing UEFI_SEAM_LDR variable

  @param VOID

  @retval Status
  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
ExposeTdxUefiSeamLdrVar (
  VOID
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_HOB_GUID_TYPE *GuidHob = NULL;
  TDX_DATA_HOB *pTdxDataHob = NULL;
  UINTN VarSize = 0;
  UINT32 Data;

  GuidHob = GetFirstGuidHob(&gTdxDataHobGuid);

  if (GuidHob == NULL) {
    DEBUG((EFI_D_ERROR, "TDX: GuidHob pointer is NULL \n"));
    Status = EFI_D_ERROR;
  } else {
    pTdxDataHob = (TDX_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);
    if (pTdxDataHob == NULL) {
      DEBUG ((EFI_D_ERROR, " Error: TDX_DATA_HOB is NULL\n"));
      Status = EFI_D_ERROR;
    } else {
      Status = gRT->GetVariable (
        TDX_UEFI_SEAM_LDR_SVN_VAR_NAME,
        (EFI_GUID*)&gEfiSeamLoaderSvnVariableGuid,
        NULL,
        &VarSize,
        &Data
        );

      if (Status == EFI_NOT_FOUND) {
        Status = gRT->SetVariable (
          TDX_UEFI_SEAM_LDR_SVN_VAR_NAME,
          (EFI_GUID*)&gEfiSeamLoaderSvnVariableGuid,
          EFI_VARIABLE_BOOTSERVICE_ACCESS,
          sizeof (pTdxDataHob->SeamLdrSvn),
          (VOID *)&pTdxDataHob->SeamLdrSvn
          );
      }
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, " Error setting the VAR_UEFI_SEAM_LOADER_SVN variable. Exitting.\n"));
      }
    }
  }
  return Status;
}


/**

  SGX Late Init Flow, executed right after MSR_BIOS_DONE
  This function has steps executed at three different scope: Thread (actually Core), Socket, System

  @param IN EFI_EVENT  Event
  @param IN VOID       *Context

  @retval None

**/
VOID
EFIAPI
SgxAfterPlatformLocksCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS Status;
  BOOLEAN    CryptoStatus = FALSE;
  BOOLEAN    WasMcheckTriggered = FALSE;

  MCHECK_BIOS_PARAM_INFO     *BiosParamInfoPtr = NULL;
  MCHECK_BUFFER              BiosParamInfoBuffer;
  SGX_ERROR_CODE_ENUM        SgxErrorCode = RS_SUCCESS;

  SGX_DEBUG ((SGX_D_ERROR, "[SGX] SgxAfterPlatformLocksCallback entry\n"));

  BiosParamInfoBuffer.Address = NULL;
  BiosParamInfoBuffer.Size    = 0;

  Status = PrepareMcheckBiosParamInfo (&BiosParamInfoBuffer);
  if (EFI_ERROR (Status)) {
    SgxErrorCode = RS_LATEINIT_CALLBACK_BIOSPARAM_ERR;
    goto SgxDisabled_LateCallback;
  }
  BiosParamInfoPtr = (MCHECK_BIOS_PARAM_INFO *)BiosParamInfoBuffer.Address;
  PrintMcheckBiosParamInfo (BiosParamInfoPtr);

  if (mSecurityPolicy->DfxAllowTdxWithoutSgx != SECURITY_POLICY_DFX_AUTO) {
    // TDX DFX - Design for Test
    SGX_DEBUG ((SGX_D_VERBOSE, "  [TDX] DfxAllowTdxWithoutSgx - LocksCallback\n"));
  }

  //
  //
  if (mSgxInitDataHob == NULL) {
    goto SgxDisabled_LateCallback;
  }

  //
  //

  if (mSgxInitDataHob->SgxBootScenario == SgxDisabled) {
    // SgxDisabled scenario skips second uCode Launch
    SGX_DEBUG ((SGX_D_VERBOSE, "  SgxDisabled scenario skips second uCode Launch\n"));
  } else if (mSgxInitDataHob->SgxBootScenario == SgxAddPackageBoot) {
    // SgxAddPackageBoot scenario skips second uCode Launch
    SGX_DEBUG ((SGX_D_VERBOSE, "  SgxAddPackageBoot scenario skips second uCode Launch\n"));

    // Save previous state as starting SgxAddPackageBoot flow (only if current RequestType != AddRequest)
    if (mInternalRegState->NonVolatile.RegistrationRequestType != SgxRequestAddRequest) {
      mInternalRegState->NonVolatile.PreviousRegistrationRequestType = mInternalRegState->NonVolatile.RegistrationRequestType;
      mInternalRegState->NonVolatile.PreviousRequestPending = mInternalRegState->RequestPending;
    }

    mInternalRegState->NonVolatile.RegistrationRequestType = SgxRequestAddRequest;
    SGX_DEBUG ((SGX_D_INFO, "  [SGX-DEBUG] Before Update PrintSgxRegistrationConfig (&mInternalRegistrationConfig)\n"));
    PrintSgxRegistrationConfig (&mInternalRegistrationConfig);

    UpdateRegistrationRequest (&mInternalRegistrationRequest, &mInternalUefiPlatformManifest, mSgxInitDataHob);

    SGX_DEBUG ((SGX_D_INFO, "  [SGX-DEBUG] After Update PrintSgxRegistrationConfig (&mInternalRegistrationConfig)\n"));
    PrintSgxRegistrationConfig (&mInternalRegistrationConfig);

    SGX_DEBUG ((SGX_D_INFO, "  [SGX-DEBUG] Before Update Hash:\n"));
    PrintByteArrays (mInternalRegState->NonVolatile.RegistrationConfigHash, SHA256_DIGEST_SIZE);
    CryptoStatus = Sha256HashAll ((UINT8 *)&mInternalRegistrationConfig,
                                sizeof (SGX_REGISTRATION_CONFIG),
                                mInternalRegState->NonVolatile.RegistrationConfigHash);
    if (CryptoStatus == FALSE) {
      SgxErrorCode = RS_LATEINIT_CALLBACK_OTHER;
      goto SgxDisabled_LateCallback;
    }
    SGX_DEBUG ((SGX_D_INFO, "  [SGX-DEBUG] Update Hash: \n"));
    PrintByteArrays (mInternalRegState->NonVolatile.RegistrationConfigHash, SHA256_DIGEST_SIZE);
    mInternalRegState->NonVolatile.RegistrationConfigHashPresent = TRUE;
    mInternalRegState->RequestPending = TRUE;
    UpdateRegistrationStatus (&mInternalRegistrationStatus);
  } else {
    // Scenarios that trigger Second uCode Launch

    //
    // Second x64 uCode Launch
    PrintMcheckBiosParamInfoBufferAsmDump (&BiosParamInfoBuffer);
    Status = SecondLoaduCode (BiosParamInfoPtr);
    if (EFI_ERROR(Status)) {
      SgxErrorCode = RS_LATEINIT_CALLBACK_UCODE_LAUNCH_ERR;
      goto SgxDisabled_LateCallback;
    }

    //
    // Check PRMRR valid bit
    //
    Status = CheckPrmrrValidBit ();
    if (EFI_ERROR (Status)) {
      SerialPrintBlocking (SGX_D_ERROR, "[SGX] PRMRR are not enabled properly!\n");
        goto Error_LateCallback;
    }

    if (mSecurityPolicy->DfxAllowTdxWithoutSgx != SECURITY_POLICY_DFX_AUTO) {
      // TDX DFX - Design for Test
      SGX_DEBUG ((SGX_D_VERBOSE, "  [TDX] DfxAllowTdxWithoutSgx - after uCode\n"));
    }
    WasMcheckTriggered = TRUE;

    ProgramLaunchControlHashOnAllThreads ();
    Status = ActivateSgx ();
    if (EFI_ERROR(Status)) {
      SgxErrorCode = RS_LATEINIT_CALLBACK_OTHER;
      goto SgxDisabled_LateCallback;
    }

    if (MsrTraceEnabled ()) {
      SGX_DEBUG ((SGX_D_ERROR, "SGX Late MSR Trace End\n"));
      SetMsrTraceOutputMode (MSR_OUTPUT_DISABLE);
    }

    //
    // Handle different boot scenarios
    //
    switch (mSgxInitDataHob->SgxBootScenario) {
      case SgxNormalBoot:
        break;
      case SgxFirstBinding:
      case SgxTcbRecovery:
        mInternalRegState->NonVolatile.RegistrationRequestType = SgxRequestPlatformManifest;
        UpdatePlatformManifestFromuCodeToInternalBuffer (
            &mInternalUefiPlatformManifest,
            BiosParamInfoPtr,
            mSgxInitDataHob->SgxBootScenario
            );
        UpdateRegistrationRequest (&mInternalRegistrationRequest, &mInternalUefiPlatformManifest, mSgxInitDataHob);
        CryptoStatus = Sha256HashAll ((UINT8 *)&mInternalRegistrationConfig,
                                      sizeof (SGX_REGISTRATION_CONFIG),
                                      mInternalRegState->NonVolatile.RegistrationConfigHash);
        if (CryptoStatus == FALSE) {
          SgxErrorCode = RS_LATEINIT_CALLBACK_OTHER;
          goto SgxDisabled_LateCallback;
        }
        mInternalRegState->NonVolatile.RegistrationConfigHashPresent = TRUE;
        mInternalRegState->RequestPending = TRUE;
        UpdateRegistrationStatus (&mInternalRegistrationStatus);

        Status = StorePlatformManifestInNvram (
          &mInternalUefiPlatformManifest
          );
        if (EFI_ERROR(Status)) {
          SgxErrorCode = RS_LATEINIT_CALLBACK_STORPLATMANIF_ERR;
          goto SgxDisabled_LateCallback;
        }
        break;
      case SgxAddPackageBoot:
        ASSERT (FALSE);
        break;
      case SgxAddPackageReboot:
        // Remove registration response on SgxAddPackageReboot
        Status = SetRegistrationVariableHelper (
          SGX_REGISTRATION_RESPONSE_VARIABLE_NAME,
          &gSgxRegistrationServerResponseVariableGuid,
          0,
          0,
          NULL,
          FALSE
          );
        if (EFI_ERROR (Status)) {
          SgxErrorCode = RS_LATEINIT_CALLBACK_OTHER;
          goto SgxDisabled_LateCallback;
        }
        // Override current RegistrationRequestType with previous value
        mInternalRegState->NonVolatile.RegistrationRequestType = mInternalRegState->NonVolatile.PreviousRegistrationRequestType;
        // Override current RequestPending with previous value
        mInternalRegState->RequestPending = mInternalRegState->NonVolatile.PreviousRequestPending;
        // Update registration request with existing platform manifest
        UpdateRegistrationRequest (&mInternalRegistrationRequest, &mInternalUefiPlatformManifest, mSgxInitDataHob);
        UpdateRegistrationStatus (&mInternalRegistrationStatus);
        break;
      default:
        ASSERT (FALSE);
        break;
    }

    Status = PopulateEpcAcpiDevices ();
    if (EFI_ERROR (Status)) {
        goto Error_LateCallback;
    }
  }
  //
  // Key Blobs Management BEGIN
  //
  // Steps 1, 2 executed only if MCHECK was triggered

  // Step 1. Update internal KeyBlobs buffer from MCHECK
  if (WasMcheckTriggered) {
    Status = UpdateKeyBlobsFromBiosParamInfo (
      mSgxInitDataHob->UefiFwKeyBlobs,
      &mSgxInitDataHob->KeyBlobsCreatedOrModifiedByMcheck,
      BiosParamInfoPtr,
      mSgxInitDataHob->MktmeIntegrityEnabled
      );
    if (EFI_ERROR (Status)) {
      SgxErrorCode = RS_LATEINIT_CALLBACK_UPDT_KEYBLOBS_ERR;
      goto SgxDisabled_LateCallback;
    }
    PrintMcheckBiosParamInfoBufferAsmDump (&BiosParamInfoBuffer);

    // Step 2. Update timestamps of KeyBlobs
    if (mSgxInitDataHob->KeyBlobsCreatedOrModifiedByMcheck) {
      Status = UpdateTimeStampOfKeyBlobs (BiosParamInfoPtr, mSgxInitDataHob, mSgxInitDataHob->SgxBootScenario);
      if (EFI_ERROR(Status)) {
        SgxErrorCode = RS_LATEINIT_CALLBACK_UPDT_KEYBLOBS_ERR;
        goto SgxDisabled_LateCallback;
      }
    }
  }

  // Step 3. Store KeyBlobs in NVRAM
  if (mSgxInitDataHob->KeyBlobsCreatedOrModifiedByMcheck ||
      mSgxInitDataHob->KeyBlobsRestoredFromPackageInfo) {
    Status = StoreKeyBlobsInNvram (
               mSgxInitDataHob->UefiFwKeyBlobs,
               sizeof (SGX_UEFIFW_KEY_BLOB) * MCHECK_MAX_PACKAGES,
               mSgxInitDataHob->MktmeIntegrityEnabled
               );
    if (EFI_ERROR (Status)) {
      SgxErrorCode = RS_LATEINIT_CALLBACK_UPDT_KEYBLOBS_ERR;
      goto SgxDisabled_LateCallback;
    }
  }

  // Step 4. Update RegistrationPackageInfo (internal buffer)
  UpdateRegistrationPackageInfo (mSgxInitDataHob, &mInternalRegistrationPackageInfo);

  // Step 5. If KeyBlobs were modified by MCHECK, set PackageInfoPending
  if (mSgxInitDataHob->KeyBlobsCreatedOrModifiedByMcheck) {
    mInternalRegState->PackageInfoPending = TRUE;
    UpdateRegistrationStatus (&mInternalRegistrationStatus);
  }
  //
  // Key Blobs Management END
  //

SgxDisabled_LateCallback:
  if (MsrTraceEnabled ()) {
    SGX_DEBUG ((SGX_D_ERROR, "SGX Late MSR Trace End\n"));
    SetMsrTraceOutputMode (MSR_OUTPUT_DISABLE);
  }

  //
  // Handle Error Code reporting to OS
  //
  if (mSgxInitDataHob == NULL) {
    // Error: no HOB
    SgxErrorCode = (UINT8)(SGX_ERROR_CODE_ENUM)RS_LATEINIT_CALLBACK_NOMEM;
  } else if (mSgxInitDataHob->SgxPreMemInitHob.ErrorCode != RS_SUCCESS) {
    // HOB exists and there is error from previous modules: override current ErrorCode with the one logged earlier
    SgxErrorCode = mSgxInitDataHob->SgxPreMemInitHob.ErrorCode;
  } else {
    // HOB exists and there is no error from previous modules: store current ErrorCode in HOB
    mSgxInitDataHob->SgxPreMemInitHob.ErrorCode = SgxErrorCode;
  }

  // Update error code in NVRAM Variable shadow buffer
  // only if !(NormalBoot && NoBiosError) else keep OS Agent errors on NormalBoot when there is no BIOS errors
  // to prevent Agent from sending multiple invalid registration requests on unrecoverable errors
  if (mSgxInitDataHob != NULL && mSgxInitDataHob->SgxBootScenario == SgxNormalBoot && SgxErrorCode == RS_SUCCESS) {
    SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] StatusVariable: keep OS Agent error on successful NormalBoot\n"));
  } else {
    SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] StatusVariable: update ErrorCode from BIOS: %x\n", SgxErrorCode));
    mInternalRegistrationStatus.ErrorCode.ErrorCode = (UINT8)SgxErrorCode;
  }

  ExposeAllScenariosSgxRegistrationUefiVariables (
    mSecurityPolicy->EnableSgx == SECURITY_POLICY_ENABLE,
    mInternalRegState->RequestPending,
    mInternalRegState->PackageInfoPending,
    mSecurityPolicy->SgxPackageInfoInBandAccess == SECURITY_POLICY_ENABLE,
    &mInternalRegistrationRequest,
    &mInternalRegistrationConfig,
    &mInternalRegistrationPackageInfo,
    &mInternalRegistrationStatus
    );
  Status = ExposeLegacySwGuardStatusUefiVariable ();
  if (EFI_ERROR (Status)) {
    goto Error_LateCallback;
  }
  Status = StoreUefiFwRegistrationStateVariable ();
  if (EFI_ERROR(Status)) {
    goto Error_LateCallback;
  }

#if SGX_DEBUG_RESTORE_KEY_BLOBS_FROM_REGISTRATION_PACKAGE_INFO
  for (SocketKeyBlobs = 0; SocketKeyBlobs < MCHECK_MAX_PACKAGES; ++SocketKeyBlobs) {
    SGX_DEBUG ((SGX_D_INFO, "  [SGX-KEYBLOB] KeyBlob for socket: %d\n", SocketKeyBlobs));
    PrintUefiFwKeyBlob(&(mSgxInitDataHob->UefiFwKeyBlobs[SocketKeyBlobs]));
  }
    SGX_DEBUG ((SGX_D_INFO, "[SGX-REGISTRATION-PACKAGE] RegistrationPackageInfo Start\n"));
    PrintRegistrationPackageInfo(&(mInternalRegistrationPackageInfo));
    SGX_DEBUG ((SGX_D_INFO, "[SGX-REGISTRATION-PACKAGE] RegistrationPackageInfo Exit\n"));
#endif

  // Exit success
  SGX_DEBUG ((SGX_D_ERROR, "[SGX] SgxAfterPlatformLocksCallback exit: success\n"));
  Status = DeallocateMcheckBiosParamInfo (&BiosParamInfoBuffer);
  ASSERT_EFI_ERROR (Status);
  return;

Error_LateCallback:
  SGX_DEBUG ((SGX_D_ERROR, "[SGX] SgxAfterPlatformLocksCallback exit: error: %r\n", Status));
  Status = DeallocateMcheckBiosParamInfo (&BiosParamInfoBuffer);
  ASSERT_EFI_ERROR (Status);
}

/**
 Update socket setup options in setup menu (SgxCapable, PRMRR size, CrDimmsPresent, SgxLeWr, Sgx MP registration agent)

  @param[in] IsSgxCapable
  @param[in] IsHwCongifSupportedBySgx
  @param[in] PrmSizeBitmap
  @param[in] PrmSize
  @param[in] CrDimmsPresent
  @param[in] SgxLeWr

  @retval Status
    @retval EFI_SUCCESS           - Installed successfully
    @retval Others                - Failed to update

**/
EFI_STATUS
UpdateSocketSetupOptions (
  UINT8  IsSgxCapable,
  UINT8  IsHwCongifSupportedBySgx,
  UINT64 PrmSizeBitmap,
  UINT64 PrmSize,
  UINT8  CrDimmsPresent,
  UINT8  SgxLeWr
  )
{
  EFI_STATUS                         Status = EFI_SUCCESS;
  UINTN                              VariableSize = 0;
  SOCKET_PROCESSORCORE_CONFIGURATION SocketProcessorCoreConfig;
  UINT32                             SetupAttr;

  SGX_DEBUG ((SGX_D_INFO, "[SGX] UpdateSocketSetupOptions Start\n"));
  VariableSize = sizeof (SOCKET_PROCESSORCORE_CONFIGURATION);
  Status = gRT->GetVariable (
    SOCKET_PROCESSORCORE_CONFIGURATION_NAME,
    &gEfiSocketProcessorCoreVarGuid,
    &SetupAttr,
    &VariableSize,
    &SocketProcessorCoreConfig
    );
  if (EFI_ERROR(Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "[SGX] UpdateSocketSetupOptions exit: Failed get variable,  error: %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  // update system sgx capable in socket setup
  SocketProcessorCoreConfig.IsSgxCapable = IsSgxCapable;
  SocketProcessorCoreConfig.IsHwCongifSupportedBySgx = IsHwCongifSupportedBySgx;

  // update PRMRR size in socket setup
  SocketProcessorCoreConfig.SprspOrLaterPrmSizeBitmap = PrmSizeBitmap;
  SocketProcessorCoreConfig.SprspOrLaterPrmSize       = PrmSize;

  // update CrDimmsPresent
  SocketProcessorCoreConfig.CrDimmsPresent = CrDimmsPresent;
  mSecurityPolicy->CrDimmsPresent = CrDimmsPresent;

  // update LeWr
  if (SocketProcessorCoreConfig.SgxDebugMode) {
    SocketProcessorCoreConfig.SgxLeWr = SgxLeWr;
    mSecurityPolicy->SgxLeWr = SgxLeWr;
  }

  // update sgx auto registration mp agent
  mSecurityPolicy->SgxAutoRegistrationAgent = SocketProcessorCoreConfig.SgxAutoRegistrationAgent;

  Status = gRT->SetVariable (
                SOCKET_PROCESSORCORE_CONFIGURATION_NAME,
                &gEfiSocketProcessorCoreVarGuid,
                SetupAttr,
                VariableSize,
                &SocketProcessorCoreConfig
                );
  if (EFI_ERROR(Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "[SGX] UpdateSocketSetupOptions exit: Failed set variable,  error: %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Update setup buffer with changed values
  //
  Status = HiiSetBrowserData (&gEfiSocketProcessorCoreVarGuid, SOCKET_PROCESSORCORE_CONFIGURATION_NAME, VariableSize, (UINT8 *) &SocketProcessorCoreConfig, NULL);
  if (EFI_ERROR(Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "[SGX] UpdateSocketSetupOptions exit: Failed update HII Browser buffer,  error: %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

/**

  Sets PrmSize to highest bit from PrmSizeBitmap and preforms warm reset

  @retval EFI_STATUS EFI_UNSUPPORTED
                     EFI_NO_MAPPING

**/
EFI_STATUS
UpdatePrmSize (
  VOID
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  UINT64                        PrmSize = BIT63;

  SGX_DEBUG ((SGX_D_INFO, "[SGX] UpdatePrmSize entry\n"));

  if (mSecurityPolicy->SprspOrLaterPrmSizeBitmap == 0 )
  {
    SGX_DEBUG ((SGX_D_ERROR, "[SGX] UpdatePrmSize exit: PrmSizeBitmap equal to 0!\n"));
    return EFI_UNSUPPORTED;
  }

  while ((mSecurityPolicy->SprspOrLaterPrmSizeBitmap & PrmSize) == 0) {
    PrmSize = PrmSize >> 1;
    if (PrmSize == 0) {
      SGX_DEBUG ((SGX_D_ERROR, "[SGX] UpdatePrmSize exit: Failed to set PRM size according to PrmSizeBitmap\n"));
      return EFI_NO_MAPPING;
    }
  }
  SGX_DEBUG ((SGX_D_ERROR, "[SGX] New PrmSize calculated: %lx\n", PrmSize));

  mSecurityPolicy->SprspOrLaterPrmSize = PrmSize;

  Status = UpdateSocketSetupOptions (
    mSecurityPolicy->IsSgxCapable,
    mSecurityPolicy->IsHwCongifSupportedBySgx,
    mSecurityPolicy->SprspOrLaterPrmSizeBitmap,
    mSecurityPolicy->SprspOrLaterPrmSize,
    mSgxInitDataHob->SgxPreMemInitHob.CrDimmsPresent,
    mSgxInitDataHob->SgxLeWr
    );
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "[SGX] Error: UpdateSocketSetupOptions (%r)\n", Status));
    return Status;
  }

  mSecurityPolicy->SprspOrLaterIsPrmSizeInvalidated = FALSE;
  gRT->ResetSystem (EfiResetWarm, Status, 0, NULL);

  return Status;
}


/**

  SGX late initialization flow

  @param ImageHandle  - ImageHandle of the loaded driver
  @param SystemTable  - Pointer to the BIOS System Table

  @retval Status
    @retval EFI_SUCCESS           - Installed successfully
    @retval Others                - Failed to install this protocol

**/
EFI_STATUS
EFIAPI
SgxLateInitEntryPoint (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  EFI_STATUS                    Status = EFI_UNSUPPORTED;
  SECURITY_POLICY               *SecurityPolicy = NULL;
  BOOLEAN                       CryptoStatus = FALSE;
  BOOLEAN                       UpdateRegConfigHash = FALSE;
  EFI_EVENT                     AfterPlatformLocksEvent;
  SGX_ERROR_CODE_ENUM           SgxErrorCode = RS_SUCCESS;
  VOID                          *HobList = NULL;
  EFI_HOB_GUID_TYPE             *GuidHob = NULL;
  EFI_GLOBAL_NVS_AREA_PROTOCOL  *AcpiNvsProtocol = NULL;

  SGX_DEBUG ((SGX_D_ERROR, "[SGX] SgxLateInitEntryPoint entry\n"));
  // First get the SecurityPolicy
  SecurityPolicy = GetSecurityPolicy ();
  if (SecurityPolicy == NULL) {
    ASSERT (SecurityPolicy != NULL);
    DEBUG ((SGX_D_ERROR, "Couldn't get security policy\n"));
    goto Error_SgxLateInit;
  }
  mSecurityPolicy = SecurityPolicy;

  // Get SGX HOB
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);
  if (EFI_ERROR(Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: EFI HOB LIST not found, exiting...\n"));
    SgxErrorCode = RS_LATEINIT_HOBLIST_NOTFOUND;
    goto Error_SgxLateInit;
  }

   // Init MP Services Protocol
  Status = SgxMpServicesData ();
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: setting up MP Services, exiting...\n"));
    SgxErrorCode = RS_LATEINIT_MPSVC_ERR;
    goto Error_SgxLateInit;
  }
  InitializeSpinLock (&mPrintDebugLock);

  //
  // Get SGX HOB data
  //
  GuidHob = GetFirstGuidHob (&gSgxInitDataHobGuid);
  if (GuidHob == NULL) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: SGX INIT DATA HOB not found, exiting...\n"));
    SgxErrorCode = RS_LATEINIT_INITDATAHOB_RES;
    goto Error_SgxLateInit;
  }
  mSgxInitDataHob = (SGX_INIT_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);
  if (mSgxInitDataHob == NULL) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: SGX INIT DATA HOB not found, exiting...\n"));
    Status = EFI_UNSUPPORTED;
    SgxErrorCode = RS_LATEINIT_INITDATAHOB_RES;
    goto Error_SgxLateInit;
  }

  // Check for factory reset
  if (mSecurityPolicy->SgxFactoryReset) {
    SGX_DEBUG ((SGX_D_ERROR, "  Perform SGX Factory Reset.\n"));
    Status = DeleteSgxVariables ();
    if (EFI_ERROR (Status)) {
      SGX_DEBUG ((SGX_D_ERROR, "  Error: SGX Factory Reset failed, exiting...\n"));
      SgxErrorCode = RS_LATEINIT_FACTORY_RESET_ERR;
      goto SgxDisabled_SgxLateInit;
    }
    Status = DisableSgxFactoryReset ();
    if (EFI_ERROR (Status)) {
      SGX_DEBUG ((SGX_D_ERROR, "  Error: SGX Factory Reset failed, exiting...\n"));
      SgxErrorCode = RS_LATEINIT_FACTORY_RESET_ERR;
      goto SgxDisabled_SgxLateInit;
    }
  }

  //
  // FuSa (SAF) - Stl2Bios
  //
  mFusaSafPolicy = GetFusaSafPolicy ();
  if (mFusaSafPolicy == NULL) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: FuSa (SAF) Policy hob not found\n"));
  }

  Status = GetStl2BiosFusaSafVariable (&mStl2BiosFusaSafVariable);
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "Error: GetStl2BiosFusaSafVariable (%r)\n", Status));
  }

  mInternalRegState = &mSgxInitDataHob->RegistrationState;

  if (mSecurityPolicy->SprspOrLaterIsPrmSizeInvalidated == TRUE &&
      mSecurityPolicy->IsHwCongifSupportedBySgx == SECURITY_POLICY_SUPPORTED) {
    Status = UpdatePrmSize ();
    if (EFI_ERROR (Status)) {
      SGX_DEBUG ((SGX_D_ERROR, "  Error: Updating PrmSize failed\n"));
      SgxErrorCode = RS_LATEINIT_SYS_NOT_CAPABLE;
      goto SgxDisabled_SgxLateInit;
    }
  }

  SGX_DEBUG ((SGX_D_INFO, "[SGX-DEBUG] mSecurityPolicy->IsSgxCapable = (%a)\n",
             ((BOOLEAN) mSecurityPolicy->IsSgxCapable == TRUE ? "TRUE" : "FALSE")));
  SGX_DEBUG ((SGX_D_INFO, "[SGX-DEBUG] mSecurityPolicy->IsHwCongifSupportedBySgx = (%a)\n",
             (mSecurityPolicy->IsHwCongifSupportedBySgx == SECURITY_POLICY_SUPPORTED
              ? "SECURITY_POLICY_SUPPORTED" : "SECURITY_POLICY_UNSUPPORTED")));

  Status = UpdateSocketSetupOptions (
    mSecurityPolicy->IsSgxCapable,
    mSecurityPolicy->IsHwCongifSupportedBySgx,
    mSecurityPolicy->SprspOrLaterPrmSizeBitmap,
    mSecurityPolicy->SprspOrLaterPrmSize,
    mSgxInitDataHob->SgxPreMemInitHob.CrDimmsPresent,
    mSgxInitDataHob->SgxLeWr
  );
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: UpdateSocketSetupOptions (%r)\n", Status));
    goto Error_SgxLateInit;
  }

  if (SecurityPolicy->EnableTdx) {
    Status = ExposeTdxUefiSeamLdrVar();
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, " ExposeTdxUefiSeamLdrVar() return error: %r\n", Status));
      ASSERT (FALSE);
    }
  }

  //
  // Restore/Init Registration Config
  //
  Status = GetRegistrationConfig (&mInternalRegistrationConfig);
  if (Status == EFI_NOT_FOUND) {
    if (mInternalRegState->RegistrationConfigVariablePresent && !mSecurityPolicy->SgxFactoryReset) {
        // Variable was present in the EarlyPhase and now is not - something is very wrong
        ASSERT (FALSE);
    }
    ZeroMem (&mInternalRegistrationConfig, sizeof (SGX_REGISTRATION_CONFIG));
    CreateRegistrationConfig (&mInternalRegistrationConfig);
    UpdateRegConfigHash = TRUE;
  } else if (EFI_ERROR (Status)) {
    SgxErrorCode = RS_LATEINIT_GET_NVVAR_ERR;
    goto SgxDisabled_SgxLateInit;
  }

  UpdateRegistrationConfigFlags (mSecurityPolicy->SgxAutoRegistrationAgent, &mInternalRegistrationConfig, &UpdateRegConfigHash);
  if (UpdateRegConfigHash) {
    UpdateRegConfigHash = FALSE;
    CryptoStatus = Sha256HashAll ((UINT8 *)&mInternalRegistrationConfig,
                                  sizeof (SGX_REGISTRATION_CONFIG),
                                  mInternalRegState->NonVolatile.RegistrationConfigHash);
    if (CryptoStatus == FALSE) {
      SgxErrorCode = RS_LATEINIT_GET_NVVAR_ERR;
      mInternalRegState->NonVolatile.RegistrationConfigHashPresent = FALSE;
      goto SgxDisabled_SgxLateInit;
    }
    mInternalRegState->NonVolatile.RegistrationConfigHashPresent = TRUE;
  }

  // Restore/Init Registration Platform Manifest
  Status = GetPlatformManifestFromNvram (&mInternalUefiPlatformManifest);
  if (Status == EFI_NOT_FOUND) {
    ZeroMem (&mInternalUefiPlatformManifest, sizeof (SGX_UEFI_REGISTRATION_PLATFORM_MANIFEST));
  } else if (EFI_ERROR (Status)) {
    SgxErrorCode = RS_LATEINIT_GET_NVVAR_ERR;
    goto SgxDisabled_SgxLateInit;
  }

  // Restore/Init Registration Request
  Status = GetRegistrationRequest (&mInternalRegistrationRequest);
  if (Status == EFI_NOT_FOUND) {
    ZeroMem (&mInternalRegistrationRequest, sizeof (SGX_REGISTRATION_REQUEST));
  } else if (EFI_ERROR (Status)) {
    SgxErrorCode = RS_LATEINIT_GET_NVVAR_ERR;
    goto SgxDisabled_SgxLateInit;
  }

  // Restore/Init Registration Status
  Status = GetRegistrationStatus (&mInternalRegistrationStatus);
  if (Status == EFI_NOT_FOUND) {
    ZeroMem (&mInternalRegistrationStatus, sizeof (SGX_REGISTRATION_STATUS));
  } else if (EFI_ERROR (Status)) {
    SgxErrorCode = RS_LATEINIT_GET_NVVAR_ERR;
    goto SgxDisabled_SgxLateInit;
  }

  //
  // Late init flow determination (capable, disabled, previous modules failed)
  // Fallback to SgxDisabled on Errors to allow RegistrationStatusVariable.ErrorCode update
  //
  if (mSecurityPolicy->IsSgxCapable == FALSE) {
    SGX_DEBUG ((SGX_D_ERROR, "  System is not SGX capable\n"));
    Status = EFI_UNSUPPORTED;
    goto SgxDisabled_SgxLateInit;
  } else if (mSecurityPolicy->EnableSgx == SECURITY_POLICY_DISABLE) {
    SGX_DEBUG ((SGX_D_ERROR, "  Warning: SGX is disabled on this system.\n"));
    Status = EFI_SUCCESS;
    goto SgxDisabled_SgxLateInit;
  } else if (mSgxInitDataHob->SgxPreMemInitHob.ErrorCode == RS_PREMEM_TME_DISABLED) {
    SGX_DEBUG ((EFI_D_ERROR, " Invalid setup setting\n"));
    SgxErrorCode = mSgxInitDataHob->SgxPreMemInitHob.ErrorCode;
    Status = EFI_SECURITY_VIOLATION;
    goto Error_SgxLateInit;
  } else if (mSgxInitDataHob->SgxPreMemInitHob.ErrorCode != RS_SUCCESS) {
    SGX_DEBUG ((SGX_D_ERROR, "  Failed previous SGX module, switch to disabled flow\n"));
    SgxErrorCode = mSgxInitDataHob->SgxPreMemInitHob.ErrorCode;
    Status = EFI_UNSUPPORTED;
    goto SgxDisabled_SgxLateInit;
  } else if (IsPlatformAttestationSupported () == FALSE) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: System BSP doesn't support Platform Attestation\n"));
    Status = EFI_UNSUPPORTED;
    SgxErrorCode = RS_LATEINIT_SYS_NOT_CAPABLE;
    goto SgxDisabled_SgxLateInit;
  }

  Status = gBS->LocateProtocol (&gEfiGlobalNvsAreaProtocolGuid, NULL, &AcpiNvsProtocol);
  if (EFI_ERROR (Status) || (AcpiNvsProtocol == NULL)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: GlobalNvsArea protocol error, exiting...\n"));
    SgxErrorCode = RS_LATEINIT_NVSAREA_ERR;
    goto SgxDisabled_SgxLateInit;
  }
  mAcpiParameter = AcpiNvsProtocol->Area;
  if (mAcpiParameter == NULL) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: GlobalNvsArea->Area NULL, exiting...\n"));
    Status = EFI_NOT_FOUND;
    SgxErrorCode = RS_LATEINIT_NVSAREA_ERR;
    goto SgxDisabled_SgxLateInit;
  }

  if (mInternalRegState->ForceFirstBindingOnRegConfChanged) {
    Status = PerformPartialReset ();
  }

SgxDisabled_SgxLateInit:
  //
  // Create Late Boot Flow Callback
  //
  Status = gBS->CreateEventEx (
    EVT_NOTIFY_SIGNAL,
    TPL_CALLBACK,
    SgxAfterPlatformLocksCallback,
    NULL,
    &gEfiAfterPlatformLocksEventGuid,
    &AfterPlatformLocksEvent
    );
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: registering AfterPlatformLocksEvent SgxLateInit callback, exiting...\n"));
    SgxErrorCode = RS_LATEINIT_TRIGCALLBACK_ERR;
    goto Error_SgxLateInit;
  }
  GetRegistrationResponse (&mInternalRegistrationResponse);
  SGX_DEBUG ((SGX_D_ERROR, "[SGX] SgxLateInit exit: success\n"));
  return Status;

Error_SgxLateInit:
  SGX_DEBUG ((SGX_D_ERROR, "[SGX] SgxLateInit exit: error: %r\n", Status));
  if (mSgxInitDataHob != NULL) {
    mSgxInitDataHob->SgxPreMemInitHob.ErrorCode = SgxErrorCode;
  }
  return Status;
}
