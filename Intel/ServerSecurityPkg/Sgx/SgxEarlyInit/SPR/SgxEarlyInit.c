/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include "SgxEarlyInit.h"

#include <Library/CpuPreMemPolicyLib.h>
#include <Library/SecurityPolicyLib.h>
#include <Library/ReferenceCodePolicyLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/KtiApi.h>
#include <Library/UpiHostDataLib.h>
#include <Library/Ms2IdiLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/PrintLib.h>
//APTIOV_SERVER_OVERRIDE_RC_START
//#include <Library/Tpm2CommandLib.h>
//APTIOV_SERVER_OVERRIDE_RC_END
#include <Mem/Library/MemMcIpLib/Include/MemMcRegs.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <ScratchpadList.h>
#include <Register/ArchitecturalMsr.h>
#include <Cpu/Msr/CommonMsr.h>
#include <UncoreCommonIncludes.h>
#include <Library/ChaLib.h>
#include <Guid/UboxIpInterface.h>
#include <Library/MemTypeLib.h>
#include <Library/BiosGuardInitLib/BiosGuardInit.h>
#include <Library/ResetSystemLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MeStorageLib.h>
#include <Library/SysHostPointerLib.h>
#include <Include/MemMapHost.h>
#include <Library/MemRcLib.h>
#include <Library/SgxDebugLib.h>
#include <Library/BaseCryptLib.h>
#include <Guid/TdxInitData.h>


#define PROACTIVE_ACTION_ON_CODE_ORGANIZATION

#ifdef PROACTIVE_ACTION_ON_CODE_ORGANIZATION
#include <Ppi/AppSgx3v0Ppi.h>
#endif // PROACTIVE_ACTION_ON_CODE_ORGANIZATION

#if SGX_DEBUG_RESTORE_KEY_BLOBS_FROM_REGISTRATION_PACKAGE_INFO
  SGX_UEFIFW_KEY_BLOB         *ClearSgxUefiFwKeyBlobsVariablePtr;
  UINTN                       ClearSgxUefiFwKeyBlobsVariableSize =  sizeof (SGX_UEFIFW_KEY_BLOB) * MCHECK_MAX_PACKAGES;
  UINT8                       ClearSocket;
  BOOLEAN                     KeyBlobExist;
#endif


#ifdef DEBUG
  volatile UINT8              Core = 0;
#endif // DEBUG

static EFI_GUID gEfiPlatformTxtPolicyDataGuid = { 0xa353290b, 0x867d, 0x4cd3, { 0xa8, 0x1b, 0x4b, 0x7e, 0x5e, 0x10, 0x0e, 0x16 } };
static EFI_GUID gSinitAcmBaseGuid = {0xBAC1001B, 0xECCC, 0x40c6, {0x99, 0x0D, 0xE8, 0xC1, 0x9A, 0x8E, 0x47, 0x7A}};
extern EFI_GUID gBiosGuardHobGuid;


CPU_PRE_MEM_POLICY          *mCpuPolicy = NULL;
SECURITY_POLICY             *mSecurityPolicy = NULL;
SYSHOST                     *mHost = NULL;
MEMORY_MAP_HOST             *mMemMapHost = NULL;
UINTN                       mSystemBsp;
UINT32                      mBspSocketId;
UINTN                       mNumberProcessors = 1;
UINT8                       mSocketCount = 1;
SPIN_LOCK                   mPrintDebugLock;
EFI_PROCESSOR_INFORMATION   *mProcInfo;
UINT8                       mPhysicalAddressBits = 36;
MRC_HOOKS_CHIP_SERVICES_PPI *mMrcHooksChipServices;
UINT64                      mPrmrrMask;
// EFI_PEI
//
EFI_PEI_SERVICES                **mPeiServices = NULL;
EFI_PEI_MP_SERVICES_PPI         *mMpServices = NULL;
EFI_PEI_READ_ONLY_VARIABLE2_PPI *mPpiVariableServices = NULL;
// SGX HOB
SGX_INIT_DATA_HOB               mSgxInitDataHob;
SGX_UEFIFW_REGISTRATION_STATE   *mRegState = &mSgxInitDataHob.RegistrationState;

EFI_PLATFORM_TXT_POLICY_DATA    *mPlatformTxtPolicyData = NULL;
BGPDT                           *mBgpdt = NULL;

VOID
OptInSgxOverAdvancedRas (
  VOID
  );

VOID
ConfigureSgxQoS (
  VOID
  );

VOID
GetSetupUrlAndSize (
  UINT8 **pUrl,
  UINT8 *pUrlSize
  );

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
    SGX_DEBUG ((SGX_D_VERBOSE, "[%02x] ", Data[Index]));
  }
  SGX_DEBUG ((SGX_D_VERBOSE, "\n"));
}

/**

  Prints all MCHECK KeyBlob structure filed

  @retval None

**/
VOID
PrintMcheckKeyBlob (
  IN  MCHECK_SGX_KEY_BLOB    *McheckSgxKeyBlob
)
{
  UINT8                           *CastPtr;
  UINT32                          DataSize = 0;
  UINT8                           Index;
  SGX_DEBUG ((SGX_D_VERBOSE, "KeyBlob:\n"
                       "\tHeader:\n"
                       "\t\tSGX UUID:\n"));
  PrintByteArrays (McheckSgxKeyBlob->Header.Uuid.Uuid, (UINT32) MCHECK_UUID_SIZE);
  SGX_DEBUG ((SGX_D_VERBOSE, "\n\t\tCpuSvn:\n"));
  DataSize = (UINT32) sizeof (McheckSgxKeyBlob->CpuSvn);
  PrintByteArrays (&(McheckSgxKeyBlob->CpuSvn), (UINT32) sizeof (McheckSgxKeyBlob->CpuSvn));
  SGX_DEBUG ((SGX_D_VERBOSE, "\n\t\tSecurity Properties:\n"));
  CastPtr = (UINT8 *) &(McheckSgxKeyBlob->SecurityProperties);
  PrintByteArrays (CastPtr, sizeof (UINT32));
  SGX_DEBUG ((SGX_D_VERBOSE, "\n\t\tPrid:\n"));
  PrintByteArrays (McheckSgxKeyBlob->Prid.Uuid, MCHECK_UUID_SIZE);
  SGX_DEBUG ((SGX_D_VERBOSE, "\n\t\tPlatformEpoch:\n"));
  PrintByteArrays (McheckSgxKeyBlob->PlatformEpoch, (sizeof (UINT8) * SGX_EPOCH_SIZE));
  SGX_DEBUG ((SGX_D_VERBOSE, "\n\t\tRsakHash:\n"));
  PrintByteArrays (McheckSgxKeyBlob->RsakHash, (sizeof (UINT8) * SHA256_DIGEST_SIZE));
  SGX_DEBUG ((SGX_D_VERBOSE, "\n\t\tRsakHash:\n"));
  PrintByteArrays (McheckSgxKeyBlob->RsakHash, (sizeof (UINT8) * SHA256_DIGEST_SIZE));
  SGX_DEBUG ((SGX_D_VERBOSE, "\n\t\tPfk1:\n"));
  PrintByteArrays (McheckSgxKeyBlob->Pfk1, (sizeof (UINT8) * AES128_KEY_SIZE));
  SGX_DEBUG ((SGX_D_VERBOSE, "\n\t\tPprResetKey:\n"));
  for (Index = 0; Index < MCHECK_MAX_KEYS; ++Index) {
    CastPtr = (UINT8 *) &(McheckSgxKeyBlob->PprResetKey[Index][0]);
    PrintByteArrays (CastPtr, sizeof (AES128_KEY_SIZE));
  }

  SGX_DEBUG ((SGX_D_VERBOSE, "\n\t\tLastUsedSvn:\n"));
  PrintByteArrays (&(McheckSgxKeyBlob->LastUsedSvn), (sizeof (UINT8)));
  SGX_DEBUG ((SGX_D_VERBOSE, "\n\t\tReserved2:\n"));
  PrintByteArrays (McheckSgxKeyBlob->Reserved2, (sizeof (UINT8) * 15));
  SGX_DEBUG ((SGX_D_VERBOSE, "\n\t\tTargetPrid:\n"));
  PrintByteArrays (McheckSgxKeyBlob->TargetPrid[0].Uuid, MCHECK_UUID_SIZE);
  SGX_DEBUG ((SGX_D_VERBOSE, "\n\t\tMasterComms:\n"));
  PrintByteArrays (McheckSgxKeyBlob->MasterComms[0].Uuid, MCHECK_UUID_SIZE);
  SGX_DEBUG ((SGX_D_VERBOSE, "\n\t\tIv:\n"));
  for (Index = 0; Index < MCHECK_MAX_KEYS; ++Index) {
    CastPtr = (UINT8 *) &(McheckSgxKeyBlob->Iv[Index][0]);
    PrintByteArrays (CastPtr, 12);
  }
  SGX_DEBUG ((SGX_D_VERBOSE, "\n\t\tMac:\n"));
  for (Index = 0; Index < MCHECK_MAX_KEYS; ++Index) {
    CastPtr = (UINT8 *) &(McheckSgxKeyBlob->Mac[Index][0]);
    PrintByteArrays (CastPtr, 16);
  }
  SGX_DEBUG ((SGX_D_VERBOSE, "\n\t\tIvComm:\n"));
  PrintByteArrays (McheckSgxKeyBlob->IvComm, (sizeof (UINT8) * 12));
  SGX_DEBUG ((SGX_D_VERBOSE, "\n\t\tMacComm:\n"));
  PrintByteArrays (McheckSgxKeyBlob->MacComm, (sizeof (UINT8) * 16));
}

/**

  Prints all UEFI FW KeyBlob structure

  @retval None

**/
VOID
PrintUefiFwKeyBlob(
  SGX_UEFIFW_KEY_BLOB             *SgxUefiFwKeyBlob
)
{

  SGX_DEBUG ((SGX_D_VERBOSE, "Present: %a\n", SgxUefiFwKeyBlob->Present ? "TRUE" : "FALSE"));
  SGX_DEBUG ((SGX_D_VERBOSE, "TimeStamp: %d-%d-%d, %d:%d:%d:%d, Pad1: %d, Pad2: %d, TimeZone: %d, Daylight: %d\n",
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

  PrintMcheckKeyBlob ((VOID *) &(SgxUefiFwKeyBlob->KeyBlob));
}

/**

Returns TdxDataHob or creates empty one if not exist

@param  VOID

@retval pointer to hob structure on success
@retval NULL on failure

**/
TDX_DATA_HOB *
GetTdxDataHob (
  VOID
  )
{
  TDX_DATA_HOB *pTdxDataHob = NULL;
  EFI_HOB_GUID_TYPE *GuidHob = NULL;

  GuidHob = GetFirstGuidHob(&gTdxDataHobGuid);

  if (GuidHob == NULL) {
    pTdxDataHob = BuildGuidHob (&gTdxDataHobGuid, sizeof (*pTdxDataHob));

    if (pTdxDataHob == NULL) {
      DEBUG ((EFI_D_ERROR, " Error: Unable to allocate TDX_DATA_HOB\n"));
      return NULL;
    }
    ZeroMem (pTdxDataHob, sizeof (*pTdxDataHob));
  } else {
    pTdxDataHob = (TDX_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);
    if (pTdxDataHob == NULL) {
      DEBUG ((EFI_D_ERROR, "[TDX] Error: Unable to find TDX_DATA_HOB\n"));
    }
  }

  return pTdxDataHob;
}

/**

  Prints all RegistrationPackageInfo structure filed

  @retval None

**/
VOID
PrintRegistrationPackageInfo (
  SGX_REGISTRATION_PACKAGE_INFO                     *SgxRegistrationPackage
  )
{
  UINT8                         Index;
  SGX_DEBUG ((SGX_D_VERBOSE, "\tHeader:\n"
                     "\t\tVersion:\n"));
   PrintByteArrays ((VOID *) &(SgxRegistrationPackage->Header.Version), sizeof (UINT16));
   SGX_DEBUG ((SGX_D_VERBOSE, "\tSize:\n"));
   PrintByteArrays ((VOID *) &(SgxRegistrationPackage->Header.Size), sizeof (UINT16));

   for (Index = 0; Index < MCHECK_MAX_PACKAGES; ++Index) {
     SGX_DEBUG ((SGX_D_VERBOSE, "  [SGX-KEYBLOB] KeyBlob for socket: %d\n", Index));
     PrintMcheckKeyBlob ((VOID *) &(SgxRegistrationPackage->KeyBlobs[Index]));
   }
}
/**
  Converts Epoch seconds (elapsed since 1970 JANUARY 01, 00:00:00 UTC) to EFI_TIME
 **/
VOID
EpochToEfiTime (
  IN  UINTN     EpochSeconds,
  OUT EFI_TIME  *Time
  )
{
  UINTN         a;
  UINTN         b;
  UINTN         c;
  UINTN         d;
  UINTN         g;
  UINTN         j;
  UINTN         m;
  UINTN         y;
  UINTN         da;
  UINTN         db;
  UINTN         dc;
  UINTN         dg;
  UINTN         hh;
  UINTN         mm;
  UINTN         ss;
  UINTN         J;

  J  = (EpochSeconds / 86400) + 2440588;
  j  = J + 32044;
  g  = j / 146097;
  dg = j % 146097;
  c  = (((dg / 36524) + 1) * 3) / 4;
  dc = dg - (c * 36524);
  b  = dc / 1461;
  db = dc % 1461;
  a  = (((db / 365) + 1) * 3) / 4;
  da = db - (a * 365);
  y  = (g * 400) + (c * 100) + (b * 4) + a;
  m  = (((da * 5) + 308) / 153) - 2;
  d  = da - (((m + 4) * 153) / 5) + 122;

  Time->Year  = (UINT16) (y - 4800 + ((m + 2) / 12));
  Time->Month = (UINT8) (((m + 2) % 12) + 1);
  Time->Day   = (UINT8) (d + 1);

  ss = EpochSeconds % 60;
  a  = (EpochSeconds - ss) / 60;
  mm = a % 60;
  b = (a - mm) / 60;
  hh = b % 24;

  Time->Hour        = (UINT8) hh;
  Time->Minute      = (UINT8) mm;
  Time->Second      = (UINT8) ss;
  Time->Nanosecond  = 0;
  Time->Pad1        = 0;
  Time->Pad2        = 0;
  Time->TimeZone    = 0;
  Time->Daylight    = 0;
}

VOID
PrintRegistrationHeader (
  SGX_REGISTRATION_HEADER           Header
  )
{
  SGX_DEBUG ((SGX_D_VERBOSE, "Registration Header:\n"));
  SGX_DEBUG ((SGX_D_VERBOSE, "  Version: %d\n", Header.Version));
  SGX_DEBUG ((SGX_D_VERBOSE, "  Size: %d\n", Header.Size));
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
PrintMcheckPubkey (
  MCHECK_SGX_PUBKEY     *McheckSgxPubKey
  )
{
  SGX_DEBUG ((SGX_D_VERBOSE, "    Modulus:"));
  PrintByteArrays (McheckSgxPubKey->Modulus, SHA3072_DIGEST_SIZE);
  SGX_DEBUG ((SGX_D_VERBOSE, "    PubExp: %x\n", McheckSgxPubKey->PubExp));
}

VOID
PrintMcheckRegistrationServerId (
  MCHECK_SGX_REGISTRATION_SERVER_ID           *McheckSgxRegistrationId
  )
{
  SGX_DEBUG ((SGX_D_VERBOSE, "    Header:\n"));
  PrintMcheckHeader (&(McheckSgxRegistrationId->Header));
  SGX_DEBUG ((SGX_D_VERBOSE, "    RsName: "));
  PrintByteArrays (McheckSgxRegistrationId->RsName, SHA256_DIGEST_SIZE);
  SGX_DEBUG ((SGX_D_VERBOSE, "    Rsak: \n"));
  PrintMcheckPubkey (&(McheckSgxRegistrationId->Rsak));
  SGX_DEBUG ((SGX_D_VERBOSE, "    Rsak: \n"));
  PrintMcheckPubkey (&(McheckSgxRegistrationId->Rsek));
  SGX_DEBUG ((SGX_D_VERBOSE, "    Signature: "));
  PrintByteArrays (McheckSgxRegistrationId->Signature, SHA3072_DIGEST_SIZE);
}

VOID
PrintRegistrationServerInfo (
  SGX_REGISTRATION_SERVER_INFO                  *SgxRegServerInfo
  )
{
  SGX_DEBUG ((SGX_D_VERBOSE, "SgxRegServerInfo:\n"));
  SGX_DEBUG ((SGX_D_VERBOSE, "  Header:\n"));
  PrintMcheckHeader (&(SgxRegServerInfo->Header));
  SGX_DEBUG ((SGX_D_VERBOSE, "  UrlSize: %d\n", SgxRegServerInfo->UrlSize));
  SGX_DEBUG ((SGX_D_VERBOSE, "  Url: "));
  PrintByteArrays(SgxRegServerInfo->Url, SGX_SERVER_URL_LENGTH);
  SGX_DEBUG ((SGX_D_VERBOSE, "  RegServerId:"));
  PrintMcheckRegistrationServerId (&(SgxRegServerInfo->RegServerId));
}

VOID
PrintRegistrationConfig (
  SGX_REGISTRATION_CONFIG                   *SgxRegConf
  )
{
  SGX_DEBUG ((SGX_D_VERBOSE, "SgxRegistrationConfig:\n"));
  SGX_DEBUG ((SGX_D_VERBOSE, "Flags: %x\n", SgxRegConf->Flags));
  PrintRegistrationServerInfo (&(SgxRegConf->SgxRegServerInfo));
}

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
Generates assembly code in EFI_DEBUG serial log to conditionally OR a
64-bit value to a Machine Specific Register (MSR). No parameter
checking is performed on Index or Value, and may cause CPU exceptions.
After this control is propagated to actual AsmMsrOr64() function

@param  Index   The 32-bit MSR index to write.
@param  Mask    The value to mask the actual value of the MSR.
@param  OrData  The value to OR with the read value from the MSR.

@return The value written back to the MSR.

**/
UINT64
SgxAsmMsrConditionalOr64 (
  IN UINT32 Index,
  IN UINT64 Mask,
  IN UINT64 OrData
  )
{
  UINT64 Data64;

  Data64 = AsmReadMsr64 (Index);
  if ((Data64 & Mask) == 0) {
    return AsmMsrOr64 (Index, OrData);
  }
  return Data64;
}

/**
Generates assembly code in EFI_DEBUG serial log to conditionally OR a
64-bit value to a Machine Specific Register (MSR). No parameter
checking is performed on Index or Value, and may cause CPU exceptions.
After this control is propagated to actual AsmMsrAndThenOr64() function

@param  Index   The 32-bit MSR index to write.
@param  Mask    The value to mask the actual value of the MSR.
@param  AndData The value to AND with the read value from the MSR.
@param  OrData  The value to OR with the read value from the MSR.

@return The value written back to the MSR.

**/
UINT64
SgxAsmMsrConditionalAndThenOr64 (
  IN UINT32 Index,
  IN UINT64 Mask,
  IN UINT64 AndData,
  IN UINT64 OrData
  )
{
  UINT64 Data64;

  Data64 = AsmReadMsr64 (Index);
  if ((Data64 & Mask) == 0) {
    return AsmMsrAndThenOr64 (Index, AndData, OrData);
  }
  return Data64;
}


/**
  Check on the processor if Debug Interface is supported

  @retval Value of DEBUG_SUPPORT and DEBUG_LOCK_SUPPORT
**/
BOOLEAN
IsDebugInterfaceSupported (
  VOID
  )
{
  UINT32 RegEcx = 0;

  ///
  /// Debug interface is supported if CPUID (EAX=1): ECX[11] = 1,
  ///
  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &RegEcx, NULL);
  if (RegEcx & BIT11) {
    return TRUE;
  } else {
    return FALSE;
  }
}


/**
  Enable/Disable Debug Interfaces in the processor.

  @param[in] Support  - To enable or disable Debug Interface feature.
**/
VOID
EnableDebugInterface (
  VOID
  )
{
  MSR_IA32_DEBUG_INTERFACE_REGISTER Msr;

  Msr.Uint64 = AsmReadMsr64 (MSR_IA32_DEBUG_INTERFACE);
  if (!Msr.Bits.Lock) {
    Msr.Bits.Enable = 1;
    Msr.Bits.Lock = 1;
    AsmWriteMsr64 (MSR_IA32_DEBUG_INTERFACE, Msr.Uint64);
  }
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
  UINTN           EnabledProcessorNum;
  UINTN           Index;

  SGX_DEBUG ((SGX_D_VERBOSE, "SgxMpServicesData()\n"));
  Status = (*mPeiServices)->LocatePpi (
    mPeiServices,
    &gEfiPeiMpServicesPpiGuid,
    0,
    NULL,
    &mMpServices
    );
  if (EFI_ERROR (Status) || (mMpServices == NULL)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Mp Services not found.\n"));
    return Status;
  }

  Status = mMpServices->GetNumberOfProcessors (
    mPeiServices,
    mMpServices,
    &mNumberProcessors,
    &EnabledProcessorNum
    );
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error: detecting number of processor threads.\n"));
    return Status;
  }

  //Memory will be freed in ClearGlobalVariables at the end of flow for this file
  mProcInfo = (EFI_PROCESSOR_INFORMATION *)
              AllocatePages (EFI_SIZE_TO_PAGES (sizeof (EFI_PROCESSOR_INFORMATION) * mNumberProcessors));
  if (mProcInfo == NULL) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: allocating mProcInfo.\n"));
    return EFI_NOT_FOUND;
  }
  ZeroMem (mProcInfo, sizeof (EFI_PROCESSOR_INFORMATION) * mNumberProcessors);

  //
  // Get each processor Location info
  //
  for (Index = 0; Index < mNumberProcessors; Index++) {
    Status = mMpServices->GetProcessorInfo (
      mPeiServices,
      mMpServices,
      Index,
      &mProcInfo[Index]
      );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    //
    // Assign Package BSPs
    //
    if (Index == 0) {
      mProcInfo[Index].StatusFlag |= PROCESSOR_AS_PBSP_BIT; // PackageBsp
    } else {
      if (mProcInfo[Index].Location.Package != mProcInfo[Index - 1].Location.Package) {
        // Next package/socket detected, mark processor as PBSB if package
        mProcInfo[Index].StatusFlag |= PROCESSOR_AS_PBSP_BIT;
        mSocketCount++;
      }
    }

#ifdef DEBUG
    if (mProcInfo[Index].StatusFlag & PROCESSOR_AS_PBSP_BIT) {
      SGX_DEBUG ((SGX_D_VERBOSE, "  Thread %d is BSP of Socket %d\n", Index, mProcInfo[Index].Location.Package));
    }
#endif // DEBUG

  } // for (Index)

  Status = mMpServices->WhoAmI (
    mPeiServices,
    mMpServices,
    &mSystemBsp
    );
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error detecting SystemBSP\n"));
    return Status;
  }
  SGX_DEBUG ((SGX_D_VERBOSE, "  System BSP Thread = %3X\n", mSystemBsp));

  mBspSocketId = mProcInfo[mSystemBsp].Location.Package;
  SGX_DEBUG ((SGX_D_VERBOSE, "  System BSP Package = %3X\n", mBspSocketId));

  return Status;
}

/**

  Get EFI Variable helper

  @param  VariableName  A pointer to a null-terminated string that is the variable's name
  @param  VendorGuid    A pointer to an EFI_GUID that is the variable's GUID. The combination of
                        VariableGuid and VariableName must be unique.
  @param  Attributes    If non-NULL, on return, points to the variable's attributes.
  @param  DataSize      On entry, points to the size in bytes of the Data buffer.
                        On return, points to the size of the data returned in Data.
  @param  Data          Points to the buffer which will hold the returned variable value.
                        May be NULL with a zero DataSize in order to determine the size of the buffer needed.

  @retval Status
    @retval EFI_SUCCESS
            EFI_INVALID_PARAMETER
            EFI_SECURITY_VIOLATION

**/
EFI_STATUS
GetVariableHelper (
  IN     CHAR16                      *VariableName,
  IN     EFI_GUID                    *VendorGuid,
  OUT    UINT32                      *Attributes,
  IN OUT UINTN                       DataSize,
  OUT    VOID                        *Data
  )
{
  EFI_STATUS                        Status;
  UINTN                             VariableSize = DataSize;

  if (Data == NULL) {
      return EFI_INVALID_PARAMETER;
  }
  SGX_DEBUG ((SGX_D_VERBOSE, "  [SGX-DEBUG] GetVariableHelper: VariableName: (%s), VendorGuid: (%x), DataSize: (%d), Data: (%s)\n", VariableName, VendorGuid->Data1, DataSize, (Data == NULL ? "T" : "F" )));

  Status = mPpiVariableServices->GetVariable (
      mPpiVariableServices,
      VariableName,
      VendorGuid,
      NULL,
      &DataSize,
      Data
      );

  if (Status == EFI_NOT_FOUND) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  GetVariable - %s not found, continue\n", VariableName));
    ZeroMem (Data, VariableSize);
    return Status;
  } else if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Failed to GetVariable - %s, error = (%r)\n", VariableName, Status));
    ASSERT (FALSE);
    return Status;
  }

  if (VariableSize != DataSize) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error: GetVariable - %s, returned size different than input buffer\n", VariableName, Status));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}


/**


  @param SgxRegistrationStatus

  @retval Status
    @retval EFI_SUCCESS

**/
EFI_STATUS
GetRegistrationStatus (
  SGX_REGISTRATION_STATUS *SgxRegistrationStatus
  )
{
  EFI_STATUS                        Status;

  Status = GetVariableHelper (
      SGX_REGISTRATION_STATUS_VARIABLE_NAME,
      &gSgxRegistrationStatusVariableGuid,
      NULL,
      sizeof (SGX_REGISTRATION_STATUS),
      (VOID *)SgxRegistrationStatus
      );

  if (Status == EFI_NOT_FOUND) {
    Status = GetVariableHelper (
        SGX_UEFI_REGISTRATION_STATUS_VARIABLE_NAME,
        &gSgxUefiRegistrationStatusVariableGuid,
        NULL,
        sizeof (SGX_REGISTRATION_STATUS),
        (VOID *)SgxRegistrationStatus
        );
  }

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
  EFI_STATUS                        Status;

  Status = GetVariableHelper (
    SGX_REGISTRATION_CONFIG_VARIABLE_NAME,
    &gSgxRegistrationConfigVariableGuid,
    NULL,
    sizeof (SGX_REGISTRATION_CONFIG),
    (VOID *)SgxRegistrationConfig
    );
  if (Status == EFI_NOT_FOUND) {
    Status = GetVariableHelper (
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
  Nulls all the pointers declared in the global scope, also does FreePages is memory has been allocated in pages
**/
VOID
ClearGlobalVariables (
  VOID
  )
{
  mSecurityPolicy = NULL;
  mHost = NULL;
  mMemMapHost = NULL;
  mMrcHooksChipServices = NULL;
  mPeiServices = NULL;
  mMpServices = NULL;
  mPpiVariableServices = NULL;
  mRegState = NULL;
  mPlatformTxtPolicyData = NULL;
  mBgpdt = NULL;

  if (mProcInfo != NULL){
    FreePages (mProcInfo, EFI_SIZE_TO_PAGES (sizeof (EFI_PROCESSOR_INFORMATION) * mNumberProcessors));
    mProcInfo = NULL;
  }
}

/**
Reads TME_ACTIVATE MSR and checks if mktme integrity bit is set

@param None

@retval TRUE is MKTME Integrity is activated

**/
BOOLEAN
MktmeIntegrityEnabled (
  VOID
  )
{
  UINT64  TMEMsr;
  TMEMsr = AsmReadMsr64 (MSR_TME_ACTIVATE);
  if ((TMEMsr & B_MKTME_INTEGRITY_ENABLE) == B_MKTME_INTEGRITY_ENABLE) {
    return TRUE;
  }
  return FALSE;
}

/**
  RestoreUefiFwRegistrationStateVariable


  @retval Status
    @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
RestoreUefiFwRegistrationStateVariable (
  VOID
  )
{
  EFI_STATUS Status;
  UINTN SgxUefiFwRegistrationStateVariableSize = sizeof (SGX_UEFIFW_NONVOLATILVE_REGISTRATION_STATE);

  Status = GetVariableHelper (
      SGX_UEFIFW_NONVOLATILVE_REGISTRATION_STATE_VARIABLE_NAME,
      &gSgxUefiFwRegistrationStateVariableGuid,
      NULL,
      SgxUefiFwRegistrationStateVariableSize,
      (VOID *)&mRegState->NonVolatile
    );
  if (Status == EFI_NOT_FOUND) {
    return Status;
  } else if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Get RegistrationState from EFI Variable failed: %r\n", Status));
    ASSERT (FALSE);
    return Status;
  }

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
  EFI_STATUS                      Status;

  Status = GetVariableHelper (
    SGX_REGISTRATION_RESPONSE_VARIABLE_NAME,
    &gSgxRegistrationServerResponseVariableGuid,
    NULL,
    sizeof (SGX_REGISTRATION_RESPONSE),
    (VOID *)SgxRegistrationResponse
    );
  if (EFI_ERROR (Status)) {
    Status = GetVariableHelper (
      SGX_UEFI_REGISTRATION_RESPONSE_VARIABLE_NAME,
      &gSgxUefiRegistrationServerResponseVariableGuid,
      NULL,
      sizeof (SGX_REGISTRATION_RESPONSE),
      (VOID *)SgxRegistrationResponse
      );
  }
  return Status;
}

/**
  Loads PreMemInitHob from memory and copies it to InitDataHob

  @retval Status

**/
EFI_STATUS
PopulateInitDataHobFromPreMem (
  VOID
  )
{
  EFI_HOB_GUID_TYPE   *GuidHob = NULL;
  SGX_PREMEM_INIT_HOB   *SgxPreMemInitHob = NULL;

  GuidHob = GetFirstGuidHob (&gSgxPreMemInitHobGuid);
  if (GuidHob == NULL) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: SGX PRE MEM DATA HOB not found, exiting...\n"));
    return EFI_NOT_FOUND;
  }
  SgxPreMemInitHob = (SGX_PREMEM_INIT_HOB *) GET_GUID_HOB_DATA (GuidHob);
  if (SgxPreMemInitHob == NULL) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: SGX PRE DATA HOB not found, exiting...\n"));
    return EFI_NOT_FOUND;
  }

  CopyMem (&mSgxInitDataHob.SgxPreMemInitHob, SgxPreMemInitHob, sizeof (SGX_PREMEM_INIT_HOB));

  return EFI_SUCCESS;
}

/**
  Builds Guid Data Hob to preserve data in Sgx Init Data Hob
  @retval Status

**/
EFI_STATUS
PreserveInitDataHob (
  VOID
  )
{
  SGX_INIT_DATA_HOB   *SgxInitDataHob = NULL;
  VOID                *HobPtr = NULL;
  EFI_STATUS          Status = EFI_SUCCESS;

  // Allocate mem for Sgx Init Data Hob
  SgxInitDataHob = (SGX_INIT_DATA_HOB *) AllocatePages (EFI_SIZE_TO_PAGES (sizeof (SGX_INIT_DATA_HOB)));
  // Check if Hob was allocated properly
  if (SgxInitDataHob == NULL) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: Unable to allocate SGX Init Data Hob.\n"));
    return EFI_NOT_FOUND;
  }
  CopyMem (SgxInitDataHob, &mSgxInitDataHob, sizeof (SGX_INIT_DATA_HOB));

  // Build Sgx Init Data Hob
  HobPtr = BuildGuidDataHob (
    &gSgxInitDataHobGuid,
    SgxInitDataHob,
    sizeof (SGX_INIT_DATA_HOB)
    );

  if (HobPtr == NULL) {
    SGX_DEBUG ((SGX_D_ERROR, "Error publishing SGX HOB data\n"));
    Status = EFI_NOT_FOUND;
  }

  FreePages (SgxInitDataHob, EFI_SIZE_TO_PAGES (sizeof (SGX_INIT_DATA_HOB)));
  return Status;
}

/**

  Determine Sgx First Boot
  This function is executed at system scope

  @param[out] *FirstBoot           This will define if SGX is in First Boot flow
                                   should return BOOLEAN.

**/
VOID
DetermineSgxFirstBoot (
  OUT BOOLEAN *FirstBoot
  )
{
  MSR_IA32_FEATURE_CONTROL_REGISTER Msr;

  Msr.Uint64 = AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL);
  *FirstBoot = FALSE;
  if (!Msr.Bits.Lock) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  [SGX] First Boot detected\n"));
    *FirstBoot = TRUE;
  }
}

/**

  Restore KeyBlobs from internal SgxUefiFwKeyBlobs NVRAM variable.

  @param[out] SGX_UEFIFW_KEY_BLOB *SgxUefiFwKeyBlobs     A pointer to UefiFwKeyBlobs
  @param[in]  BOOLEAN             MktmeIntegrityEnabled

  @retval EFI_SUCCESS      When successfully restored KeyBlobs
  @retval EFI_NOT_FOUND    When KeyBlobs are NOT restored

**/
EFI_STATUS
EFIAPI
RestoreUefiFwKeyBlobsVariable (
  OUT SGX_UEFIFW_KEY_BLOB *OutSgxUefiFwKeyBlobs,
  IN  BOOLEAN             MktmeIntegrityEnabled
  )
{
  EFI_STATUS Status            = EFI_NOT_FOUND;
  BOOLEAN    AnyKeyBlobPresent = FALSE;
  BOOLEAN    KeyBlobPresent    = FALSE;
  UINTN      Socket;

  SGX_UEFIFW_KEY_BLOB  *SgxUefiFwKeyBlobs    = NULL;
  UINTN                SgxUefiFwKeyBlobsSize = sizeof (SGX_UEFIFW_KEY_BLOB) * MCHECK_MAX_PACKAGES;

  SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] %a BEGIN\n", __FUNCTION__));

  if (OutSgxUefiFwKeyBlobs == NULL) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: SgxUefiFwKeyBlobs is a NULL! Check input parameters!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit_RestoreUefiFwKeyBlobsVariable;
  }

  if (MktmeIntegrityEnabled) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  MKTME integrity bit is set, KeyBlobs with integrity will be restored from NVRAM!\n"));
  }

  // Step 1. Allocate memory for local SgxUefiFwKeyBlobs shadow
  SgxUefiFwKeyBlobs = (SGX_UEFIFW_KEY_BLOB *)AllocatePages (EFI_SIZE_TO_PAGES (SgxUefiFwKeyBlobsSize));
  if (SgxUefiFwKeyBlobs == NULL) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Unable to allocate memory for SgxUefiFwKeyBlobs\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit_RestoreUefiFwKeyBlobsVariable;
  }
  ZeroMem (SgxUefiFwKeyBlobs, SgxUefiFwKeyBlobsSize);

  // Step 2. Restore SgxUefiFwKeyBlobs from NVRAM
  Status = GetVariableHelper (
    MktmeIntegrityEnabled ? SGX_UEFIFW_KEY_BLOB_INTEGRITY_VARIABLE_NAME : SGX_UEFIFW_KEY_BLOB_VARIABLE_NAME,
    MktmeIntegrityEnabled ? &gSgxUefiFwKeyBlobsWithIntegrityVariableGuid : &gSgxUefiFwKeyBlobsVariableGuid,
    NULL,
    SgxUefiFwKeyBlobsSize,
    (VOID *)SgxUefiFwKeyBlobs
    );
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Unable to get SgxUefiFwKeyBlobs\n", Status));
    goto Exit_RestoreUefiFwKeyBlobsVariable;
  }

  // Step 3. Check if AnyKeyBlobPresent
  for (Socket = 0; Socket < MCHECK_MAX_PACKAGES; Socket++) {
    if (SgxUefiFwKeyBlobs[Socket].Present) {
      AnyKeyBlobPresent = TRUE;
      break; // break if at least one KeyBlob was found
    }
  }

  // Step 4. Check KeyBlobs if their integrity option match current system settings
  for (Socket = 0; Socket < MCHECK_MAX_PACKAGES; Socket++) {
    if (SgxUefiFwKeyBlobs[Socket].Present == TRUE &&
        (SgxUefiFwKeyBlobs[Socket].Integrity != MktmeIntegrityEnabled)) {
      SGX_DEBUG ((SGX_D_ERROR, "  Error: Matching KeyBlobs were found but KeyBlobs integrity is NOT"
                                 " matching current system configuration!\n", Status));
      Status = EFI_NOT_FOUND;
      goto Exit_RestoreUefiFwKeyBlobsVariable;
    }
  }

  if (AnyKeyBlobPresent) {
    // Step 5. If AnyKeyBlobPresent, copy contents of SgxUefiFwKeyBlobs to OutSgxUefiFwKeyBlobs
    //         Output buffer shall get modified only when KeyBlobs are present
    ZeroMem (OutSgxUefiFwKeyBlobs, SgxUefiFwKeyBlobsSize);
    for (Socket = 0; Socket < MCHECK_MAX_PACKAGES; Socket++) {
      KeyBlobPresent = SgxUefiFwKeyBlobs[Socket].Present;
      if (KeyBlobPresent) {
        CopyMem (
          &OutSgxUefiFwKeyBlobs[Socket],
          &SgxUefiFwKeyBlobs[Socket],
          sizeof (SGX_UEFIFW_KEY_BLOB) // SgxUefiFwKeyBlobs NVRAM buffer contains KeyBlobs with metadata (internal type)
          );
        SGX_DEBUG ((SGX_D_VERBOSE, "  Copied KeyBlob [%d] from SgxUefiFwKeyBlob NVRAM variable\n", Socket));
      }
    }
    // Set status to EFI_SUCCESS if KeyBlobs were found and copied
    Status = EFI_SUCCESS;
  } else {
    // Set status to EFI_NOT_FOUND if KeyBlobs were NOT restored
    Status = EFI_NOT_FOUND;
  }

Exit_RestoreUefiFwKeyBlobsVariable:
  if (SgxUefiFwKeyBlobs != NULL) {
    FreePages (SgxUefiFwKeyBlobs, EFI_SIZE_TO_PAGES (SgxUefiFwKeyBlobsSize));
  }

  SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] %a END\n", __FUNCTION__));
  return Status;
}


/**

  Restore KeyBlobs from SgxRegistrationPackageInfo NVRAM variable.

  @param[out] SGX_UEFIFW_KEY_BLOB *SgxUefiFwKeyBlobs  A pointer to UefiFwKeyBlobs
  @param[in]  BOOLEAN             MktmeIntegrityEnabled

  @retval EFI_SUCCESS      When successfully restored KeyBlobs
  @retval EFI_NOT_FOUND    When SgxRegistrationPackageInfo or KeyBlobs are NOT found

**/
EFI_STATUS
EFIAPI
RestoreUefiFwKeyBlobsVariableFromPackageInfo (
  OUT SGX_UEFIFW_KEY_BLOB *OutSgxUefiFwKeyBlobs,
  IN  BOOLEAN             MktmeIntegrityEnabled
  )
{
  EFI_STATUS                              Status            = EFI_NOT_FOUND;
  BOOLEAN                                 AnyKeyBlobPresent = FALSE;
  BOOLEAN                                 KeyBlobPresent    = FALSE;

  UINTN                                   Socket;
  EFI_TIME                                Time;
  UINTN                                   TimeInSecond;

  SGX_REGISTRATION_PACKAGE_INFO           *SgxRegistrationPackageInfo    = NULL;
  UINTN                                   SgxRegistrationPackageInfoSize = sizeof (SGX_REGISTRATION_PACKAGE_INFO);

  SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] %a BEGIN\n", __FUNCTION__));

  if (OutSgxUefiFwKeyBlobs == NULL) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: SgxUefiFwKeyBlobs is a NULL! Check input parameters!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit_RestoreUefiFwKeyBlobsVariableFromPackageInfo;
  }

  // Step 1. Allocate memory for local SgxRegistrationPackageInfo shadow
  SgxRegistrationPackageInfo = (SGX_REGISTRATION_PACKAGE_INFO *)AllocatePages (EFI_SIZE_TO_PAGES (SgxRegistrationPackageInfoSize));
  if (SgxRegistrationPackageInfo == NULL) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Unable to allocate memory for SgxRegistrationPackageInfo\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit_RestoreUefiFwKeyBlobsVariableFromPackageInfo;
  }
  ZeroMem (SgxRegistrationPackageInfo, SgxRegistrationPackageInfoSize);

  // Step 2. Restore SgxRegistrationPackageInfo from NVRAM
  Status = GetVariableHelper (
    (MktmeIntegrityEnabled ? SGX_REGISTRATION_PACKAGE_INFO_INTEGRITY_VARIABLE_NAME : SGX_REGISTRATION_PACKAGE_INFO_VARIABLE_NAME),
    (MktmeIntegrityEnabled ? &gSgxRegistrationPackageInfoWithIntegrityVariableGuid : &gSgxRegistrationPackageInfoVariableGuid),
    NULL,
    SgxRegistrationPackageInfoSize,
    SgxRegistrationPackageInfo
    );
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Unable to get SgxRegistrationPackageInfo\n", Status));
    Status = EFI_NOT_FOUND;
    goto Exit_RestoreUefiFwKeyBlobsVariableFromPackageInfo;
  }

  // Step 3. Obtain current timestamp
  TimeInSecond = GetTimeInSecondsFrom1970 ();
  EpochToEfiTime (TimeInSecond, &Time);

  // Step 4. Check if AnyKeyBlobPresent
  for (Socket = 0; Socket < MCHECK_MAX_PACKAGES; ++Socket) {
    AnyKeyBlobPresent = SgxRegistrationPackageInfo->KeyBlobs[Socket].Header.Size != 0;
    if (AnyKeyBlobPresent) {
      break; // break if at least one KeyBlob was found
    }
  }

  if (AnyKeyBlobPresent) {
    // Step 5. If AnyKeyBlobPresent, copy contents of SgxRegistrationPackageInfo to OutSgxUefiFwKeyBlobs
    //         Output buffer shall get modified only when KeyBlobs are present
    ZeroMem (OutSgxUefiFwKeyBlobs, sizeof (SGX_UEFIFW_KEY_BLOB) * MCHECK_MAX_PACKAGES);
    for (Socket = 0; Socket < MCHECK_MAX_PACKAGES; ++Socket) {
      KeyBlobPresent = SgxRegistrationPackageInfo->KeyBlobs[Socket].Header.Size != 0;
      if (KeyBlobPresent) {
        CopyMem (
          &OutSgxUefiFwKeyBlobs[Socket].KeyBlob,
          &SgxRegistrationPackageInfo->KeyBlobs[Socket],
          sizeof (MCHECK_SGX_KEY_BLOB) // SgxRegistrationPackageInfo NVRAM buffer contains raw KeyBlobs without metadata
          );
        OutSgxUefiFwKeyBlobs[Socket].Present   = KeyBlobPresent;
        OutSgxUefiFwKeyBlobs[Socket].TimeStamp = Time;
        SGX_DEBUG ((SGX_D_VERBOSE, "  Copied KeyBlob [%d] from SgxRegistrationPackageInfo NVRAM variable\n", Socket));
      }
    }
    // Set status to EFI_SUCCESS if KeyBlobs were found and copied
    Status = EFI_SUCCESS;
  } else {
    // Set status to EFI_NOT_FOUND if KeyBlobs were NOT restored
    Status = EFI_NOT_FOUND;
  }

Exit_RestoreUefiFwKeyBlobsVariableFromPackageInfo:
  if (SgxRegistrationPackageInfo != NULL) {
    FreePages (SgxRegistrationPackageInfo, EFI_SIZE_TO_PAGES (SgxRegistrationPackageInfoSize));
  }

  SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] %a END\n", __FUNCTION__));
  return Status;
}


/**

  Restore KeyBlobs to internal SgxInitDataHob fields from SgxUefiFwKeyBlobs or SgxRegistrationPackageInfo
  This function is executed at system scope

  @param[out] SGX_INIT_DATA_HOB *SgxInitDataHob
  @param[in]  BOOLEAN           PackageInfoInBandAccess
  @param[in]  BOOLEAN           MktmeIntegrityEnabled

  @retval     EFI_SUCCESS    KeyBlobs were restored either from SgxKeyBlobs NVRAM variable or
                             SgxRegistrationPackageInfo (if PackageInfoInBandAccess issued)
  @retval     EFI_NOT_FOUND  None KeyBlob were restored

**/
EFI_STATUS
EFIAPI
RestoreKeyBlobsInternalSgxInitDataHobFieldsFromNvram (
  OUT SGX_INIT_DATA_HOB *SgxInitDataHob,
  IN  BOOLEAN           PackageInfoInBandAccess,
  IN  BOOLEAN           MktmeIntegrityEnabled
  )
{
  EFI_STATUS Status = EFI_NOT_FOUND;

  SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] %a BEGIN \n", __FUNCTION__));

  if (SgxInitDataHob == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit_RestoreKeyBlobsInternalSgxInitDataHobFieldsFromNvram;
  }
  SgxInitDataHob->KeyBlobsExistInNvram            = FALSE;
  SgxInitDataHob->KeyBlobsRestoredFromPackageInfo = FALSE;

#if SGX_DEBUG_RESTORE_KEY_BLOBS_FROM_REGISTRATION_PACKAGE_INFO
  goto MockRestoreKeyBlobsFromRegistrationPackageInfo;
#endif

  // Step 1. Try restoring KeyBlobs from SgxUefiFwKeyBlobs NVRAM variable
  Status = RestoreUefiFwKeyBlobsVariable (
      SgxInitDataHob->UefiFwKeyBlobs,
      MktmeIntegrityEnabled
      );
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  KeyBlobs were NOT restored from SgxUefiFwKeyBlobs!\n"));
    SgxInitDataHob->KeyBlobsExistInNvram = FALSE;
  } else {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Successfuly restored KeyBlobs from SgxUefiFwKeyBlobs\n"));
    SgxInitDataHob->KeyBlobsExistInNvram = TRUE;
  }

  // Step 2. Try restoring KeyBlobs from SgxRegistrationPackageInfo NVRAM variable
  if (PackageInfoInBandAccess == SECURITY_POLICY_ENABLE) {
#if SGX_DEBUG_RESTORE_KEY_BLOBS_FROM_REGISTRATION_PACKAGE_INFO
MockRestoreKeyBlobsFromRegistrationPackageInfo:
#endif
    Status = RestoreUefiFwKeyBlobsVariableFromPackageInfo (
              SgxInitDataHob->UefiFwKeyBlobs,
              MktmeIntegrityEnabled
              );
    if (EFI_ERROR (Status)) {
      SGX_DEBUG ((SGX_D_VERBOSE, "  KeyBlobs were NOT restored from SgxRegistrationPackageInfo!\n"));
    } else {
      SGX_DEBUG ((SGX_D_VERBOSE, "  Successfully restored KeyBlobs from SgxRegistrationPackageInfo\n"));
      SgxInitDataHob->KeyBlobsExistInNvram = TRUE;
      SgxInitDataHob->KeyBlobsRestoredFromPackageInfo = TRUE;
    }
  } else {
    SGX_DEBUG ((SGX_D_VERBOSE, "  KeyBlobs were NOT restored from SgxRegistrationPackageInfo contents"));
    SGX_DEBUG ((SGX_D_VERBOSE, " due to PackageInfoInBandAccess = FALSE!\n"));
  }

  SGX_DEBUG ((SGX_D_VERBOSE, "  KeyBlobsExistInNvram = (%a)\n",
             (SgxInitDataHob->KeyBlobsExistInNvram == TRUE ? "TRUE" : "FALSE")));

Exit_RestoreKeyBlobsInternalSgxInitDataHobFieldsFromNvram:
  SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] %a END \n", __FUNCTION__));
  return Status;
}

/**

  Restore Registration Variables from NVRAM
  This function is executed at system scope

  BootScenario        This will define the boot scenario from SGX perspective
                      should one of SGX_BOOT_SCENARIO enum structure.

**/
EFI_STATUS
GetRegistrationVariablesFromNvram (
  VOID
  )
{
  EFI_STATUS                        Status;
  BOOLEAN                           CryptoStatus = FALSE;
  SGX_REGISTRATION_CONFIG           SgxRegistrationConfig;
  SGX_REGISTRATION_STATUS           SgxRegistrationStatus;
  SGX_REGISTRATION_RESPONSE         SgxRegistrationResponse;
  UINT8                             RegistrationConfigHash[SHA256_DIGEST_SIZE];
  UINT8                             Index;
  UINT8                             PlatCertIndex;

  SGX_DEBUG ((SGX_D_VERBOSE, "GetRegistrationVariablesFromNvram Enter\n"));

  Status = GetRegistrationConfig (&SgxRegistrationConfig);
  if (Status == EFI_SUCCESS) {
    mRegState->RegistrationConfigVariablePresent = TRUE;
    SGX_DEBUG ((SGX_D_VERBOSE, "    [SGX-DEBUG]  PrintRegistrationConfig (&SgxRegistrationConfig)\n"));
    PrintRegistrationConfig (&SgxRegistrationConfig);
    //
    // Check for RegistrationConfiguration changes
    //
    // calculate if hash matches the one stored in NVRAM
    if (mRegState->NonVolatile.RegistrationConfigHashPresent) {
      CryptoStatus = Sha256HashAll ((UINT8 *)&SgxRegistrationConfig,
                                    sizeof (SGX_REGISTRATION_CONFIG),
                                    RegistrationConfigHash);
      if (CryptoStatus == FALSE) {
        Status = EFI_SECURITY_VIOLATION;
        return Status;
      }
      mRegState->ForceFirstBindingOnRegConfChanged = (BOOLEAN)CompareMem (RegistrationConfigHash,
                                                                  mRegState->NonVolatile.RegistrationConfigHash,
                                                                  SHA256_DIGEST_SIZE);
      SGX_DEBUG ((SGX_D_VERBOSE, "    [SGX-DEBUG]  SgxRegistrationConfig = %a\n", (SgxRegistrationConfig.SgxRegServerInfo.RegServerId.RsName == NULL ? "TRUE" : "FALSE")));
      SGX_DEBUG ((SGX_D_VERBOSE, "    [SGX-DEBUG] GetRegistrationVariablesFromNvram Hash:\n"));
      PrintByteArrays (RegistrationConfigHash, SHA256_DIGEST_SIZE);
      SGX_DEBUG ((SGX_D_VERBOSE, " GetRegistrationVariablesFromNvram mRegState->NonVolatile.RegistrationConfigHash:\n"));
      PrintByteArrays (mRegState->NonVolatile.RegistrationConfigHash, SHA256_DIGEST_SIZE);
    } else {
      mRegState->ForceFirstBindingOnRegConfChanged = FALSE;
    }
  } else if (Status == EFI_NOT_FOUND) {
    mRegState->RegistrationConfigVariablePresent = FALSE;
    mRegState->ForceFirstBindingOnRegConfChanged = FALSE;
    Status = EFI_SUCCESS;
  } else if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  Status = GetRegistrationStatus (&SgxRegistrationStatus);
  if (Status == EFI_SUCCESS) {
    mRegState->RegistrationStatusVariablePresent = TRUE;
    mRegState->RequestPending = !SgxRegistrationStatus.Status.SgxRegistrationComplete;
    mRegState->PackageInfoPending  = !SgxRegistrationStatus.Status.SgxPackageInfoReadComplete;
  } else if (Status == EFI_NOT_FOUND) {
    mRegState->RegistrationStatusVariablePresent = FALSE;
    // Init initial value on first boot - assume FirstBinding
    mRegState->RequestPending = TRUE;
    // Init initial value on first boot to what user requested
    mRegState->PackageInfoPending = mSecurityPolicy->SgxPackageInfoInBandAccess;
    Status = EFI_SUCCESS;
  } else if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }
  if (SgxRegistrationStatus.ErrorCode.ErrorCodeFields.RegistrationErrorCode != 0) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error: SgxRegistrationStatus report error code: %d\n",
            SgxRegistrationStatus.ErrorCode.ErrorCodeFields.RegistrationErrorCode));
  }

  //
  // RegistrationResponse
  //
  Status = GetRegistrationResponse (&SgxRegistrationResponse);
  if (Status == EFI_SUCCESS) {
    // When registration response found attemnpt to match Platform Membership Certificates
    mRegState->RegistrationResponseVariablePresent = TRUE;

    //
    // Get Platform Certificates from RegistrationResponse and match them to sockets
    for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
      if (!SocketPresent (Index)) {
        continue;
      }
      ZeroMem (&mSgxInitDataHob.UefiFwPlatMemCert[Index].PlatformMembershipCertificate, sizeof (SGX_UEFIFW_PLATFORM_MEMBERSHIP_CERTIFICATE));
      mSgxInitDataHob.UefiFwPlatMemCert[Index].Present = FALSE;

      // Match Platform Membership Certificate with Socket by PRID
      for (PlatCertIndex = 0; PlatCertIndex < MCHECK_MAX_PACKAGES; PlatCertIndex++){
        if ((mSgxInitDataHob.Socket[Index].Prids_0 != 0 && mSgxInitDataHob.Socket[Index].Prids_1 != 0) &&
            (mSgxInitDataHob.Socket[Index].Prids_0 == SgxRegistrationResponse.PlatformMembershipCertificate[PlatCertIndex].Prid.Uuid64[0]) &&
            (mSgxInitDataHob.Socket[Index].Prids_1 == SgxRegistrationResponse.PlatformMembershipCertificate[PlatCertIndex].Prid.Uuid64[1])) {
          SGX_DEBUG ((SGX_D_VERBOSE, "Plat Cert Membership Index %d found for Socket[%d]\n", PlatCertIndex, Index));
          CopyMem (
            &mSgxInitDataHob.UefiFwPlatMemCert[Index].PlatformMembershipCertificate,
            &SgxRegistrationResponse.PlatformMembershipCertificate[PlatCertIndex],
            sizeof (SGX_UEFIFW_PLATFORM_MEMBERSHIP_CERTIFICATE)
            );
          mSgxInitDataHob.UefiFwPlatMemCert[Index].Present = TRUE;
          break;
        }
      }
    }
  } else if (Status == EFI_NOT_FOUND) {
    for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
      ZeroMem (&mSgxInitDataHob.UefiFwPlatMemCert[Index].PlatformMembershipCertificate, sizeof (SGX_UEFIFW_PLATFORM_MEMBERSHIP_CERTIFICATE));
      mSgxInitDataHob.UefiFwPlatMemCert[Index].Present = FALSE;
    }
    mRegState->RegistrationResponseVariablePresent = FALSE;
    Status = EFI_SUCCESS;
  } else if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  SGX_DEBUG ((SGX_D_VERBOSE, "RegistrationVariables presence:\n"));
  SGX_DEBUG ((SGX_D_VERBOSE, "  RegistrationConfig   = %d\n", mRegState->RegistrationConfigVariablePresent));
  SGX_DEBUG ((SGX_D_VERBOSE, "  RegistrationStatus   = %d\n", mRegState->RegistrationStatusVariablePresent));
  SGX_DEBUG ((SGX_D_VERBOSE, "  RegistrationResponse = %d\n", mRegState->RegistrationResponseVariablePresent));

  return Status;
}


// Define EPOCH (1970-JANUARY-01) in the Julian Date representation
#define EPOCH_JULIAN_DATE                               2440588
// Seconds per unit
#define SEC_PER_MIN                                     ((UINTN)    60)
#define SEC_PER_HOUR                                    ((UINTN)  3600)
#define SEC_PER_DAY                                     ((UINTN) 86400)
/**
  Converts EFI_TIME to Epoch seconds (elapsed since 1970 JANUARY 01, 00:00:00 UTC)
 **/
UINTN
EfiTimeToEpoch (
  IN  EFI_TIME  *Time
  )
{
  UINTN a;
  UINTN y;
  UINTN m;
  UINTN JulianDate;  // Absolute Julian Date representation of the supplied Time
  UINTN EpochDays;   // Number of days elapsed since EPOCH_JULIAN_DAY
  UINTN EpochSeconds;

  a = (14 - Time->Month) / 12 ;
  y = Time->Year + 4800 - a;
  m = Time->Month + (12*a) - 3;

  JulianDate = Time->Day + ((153*m + 2)/5) + (365*y) + (y/4) - (y/100) + (y/400) - 32045;

  ASSERT (JulianDate >= EPOCH_JULIAN_DATE);
  EpochDays = JulianDate - EPOCH_JULIAN_DATE;

  EpochSeconds = (EpochDays * SEC_PER_DAY) + ((UINTN)Time->Hour * SEC_PER_HOUR) + (Time->Minute * SEC_PER_MIN) + Time->Second;

  return EpochSeconds;
}


/**

  Check if all KeyBlobs are available for all packages
  Also check if Prid in keyblobs match with socket Prid
  This function is executed at system scope

**/
BOOLEAN
KeyBlobsAvailableForAllPackagesAndPrids (
  VOID
  )
{
  UINT8               SocketIndex            = 0;
  UINT8               KeyBlobIndex           = 0;
  BOOLEAN             MatchingKeyBlobFound   = FALSE;
  BOOLEAN             KeyBlobsForAllPackages = TRUE;
  SGX_UEFIFW_KEY_BLOB TmpKeyBlob;

  SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] %a BEGIN\n", __FUNCTION__));

  for (SocketIndex = 0; SocketIndex < MCHECK_MAX_PACKAGES; SocketIndex++) {
    if (SocketPresent (SocketIndex)) {
      if (mSgxInitDataHob.UefiFwKeyBlobs[SocketIndex].Present &&
          (mSgxInitDataHob.Socket[SocketIndex].Prids_0 == mSgxInitDataHob.UefiFwKeyBlobs[SocketIndex].KeyBlob.Prid.Uuid64[0]) &&
          (mSgxInitDataHob.Socket[SocketIndex].Prids_1 == mSgxInitDataHob.UefiFwKeyBlobs[SocketIndex].KeyBlob.Prid.Uuid64[1])) {
        mSgxInitDataHob.Socket[SocketIndex].AddPackageNew = FALSE;
      } else {
        for (KeyBlobIndex = 0; KeyBlobIndex < MCHECK_MAX_PACKAGES; KeyBlobIndex++) {
          MatchingKeyBlobFound = mSgxInitDataHob.UefiFwKeyBlobs[KeyBlobIndex].Present &&
                                 (mSgxInitDataHob.Socket[SocketIndex].Prids_0 == mSgxInitDataHob.UefiFwKeyBlobs[KeyBlobIndex].KeyBlob.Prid.Uuid64[0]) &&
                                 (mSgxInitDataHob.Socket[SocketIndex].Prids_1 == mSgxInitDataHob.UefiFwKeyBlobs[KeyBlobIndex].KeyBlob.Prid.Uuid64[1]);
          if (MatchingKeyBlobFound) {
            break;
          }
        }
        if (MatchingKeyBlobFound) {
          CopyMem (
            &TmpKeyBlob,
            &mSgxInitDataHob.UefiFwKeyBlobs[KeyBlobIndex],
            sizeof (SGX_UEFIFW_KEY_BLOB)
            );
          CopyMem (
            &mSgxInitDataHob.UefiFwKeyBlobs[KeyBlobIndex],
            &mSgxInitDataHob.UefiFwKeyBlobs[SocketIndex],
            sizeof (SGX_UEFIFW_KEY_BLOB)
            );
          CopyMem (
            &mSgxInitDataHob.UefiFwKeyBlobs[SocketIndex],
            &TmpKeyBlob,
            sizeof (SGX_UEFIFW_KEY_BLOB)
            );
          mSgxInitDataHob.Socket[SocketIndex].AddPackageNew = FALSE;
        } else {
          KeyBlobsForAllPackages = FALSE;
          mSgxInitDataHob.Socket[SocketIndex].AddPackageNew = TRUE;
        }
      }

      SGX_DEBUG ((SGX_D_VERBOSE, "  mSgxInitDataHob.Socket[%d].AddPackageNew = (%a)\n",
                  SocketIndex,
                  (mSgxInitDataHob.Socket[SocketIndex].AddPackageNew == TRUE ? "TRUE" : "FALSE")
                  ));
    }
  }

  SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] %a END\n", __FUNCTION__));
  return KeyBlobsForAllPackages;
}

/**

  Check if all or any keyblobs are with booting SVN
  This function is executed at system scope

**/
EFI_STATUS
DoKeyBlobsHaveBootingSVN (
  BOOLEAN *AllCorrect,
  BOOLEAN *AnyCorrect
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT8      Index;

  if (AllCorrect == NULL ||
      AnyCorrect == NULL) {
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  *AnyCorrect  = FALSE;
  *AllCorrect  = TRUE;

  for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
    if (mSgxInitDataHob.UefiFwKeyBlobs[Index].Present) {
      if (mSgxInitDataHob.UefiFwKeyBlobs[Index].KeyBlob.CpuSvn >= mSgxInitDataHob.BootingSvn) {
        *AnyCorrect = TRUE;
      } else {
        *AllCorrect = FALSE;
      }
    }
  }
  if (*AnyCorrect == FALSE) {
    *AllCorrect = FALSE;
  }

  return Status;
}

/**
  Check if Platform Membership Certificates exist for all packages
  and are with booting SVN.
  Function assumes that PlatMemCert are already matched with socket index by PRID
  This function is executed at system scope.
**/
BOOLEAN
PlatMemCertsFoundForAllPkgAndSvn (
  VOID
  )
{
  UINT8      Index;
  BOOLEAN    Result = FALSE;
  SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] %a BEGIN\n", __FUNCTION__));

  for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  mSgxInitDataHob.Socket[%d].AddPackageNew = %a\n",
               Index, (mSgxInitDataHob.Socket[Index].AddPackageNew == TRUE ? "TRUE" : "FALSE")));
    SGX_DEBUG ((SGX_D_VERBOSE, "  mSgxInitDataHob.UefiFwPlatMemCert[%d].Present = %a\n",
               Index, (mSgxInitDataHob.UefiFwPlatMemCert[Index].Present == TRUE ? "TRUE" : "FALSE")));

    if (mSgxInitDataHob.Socket[Index].AddPackageNew && mSgxInitDataHob.UefiFwPlatMemCert[Index].Present) {
      SGX_DEBUG ((SGX_D_VERBOSE, "  Platform Membership Certificate present for Socket[%d]\n", Index));
      if (mSgxInitDataHob.UefiFwPlatMemCert[Index].PlatformMembershipCertificate.CpuSvn < mSgxInitDataHob.BootingSvn) {
        SGX_DEBUG ((SGX_D_VERBOSE, "  PlatformMembershipCertificate.CpuSvn does NOT match BootingSvn\n"));
        Result = FALSE;
        goto Exit_PlatMemCertsFoundForAllPkgAndSvn;
      } else {
        Result = TRUE;
      }
    } else {
      SGX_DEBUG ((SGX_D_VERBOSE, "  (mSgxInitDataHob.Socket[%d].AddPackageNew && \
                                    mSgxInitDataHob.UefiFwPlatMemCert[%d].Present) == FALSE\n", Index, Index));
    }
  }

Exit_PlatMemCertsFoundForAllPkgAndSvn:
  SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] %a END\n", __FUNCTION__));
  return Result;
}


/**

  Delete KeyBlobs with SVN not booting SVN
  This function is executed at system scope

**/
VOID
DeleteKeyBlobsWithoutBootingSVN (
  VOID
  )
{
  UINT8      Index;

  for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
    if ((mSgxInitDataHob.UefiFwKeyBlobs[Index].Present)  &&
        (mSgxInitDataHob.UefiFwKeyBlobs[Index].KeyBlob.CpuSvn < mSgxInitDataHob.BootingSvn)) {
      // Delete keyblob
      mSgxInitDataHob.UefiFwKeyBlobs[Index].Present = FALSE;
      ZeroMem (&mSgxInitDataHob.UefiFwKeyBlobs[Index], sizeof (SGX_UEFIFW_KEY_BLOB));
    }
  }
}


/**

  Determine the SGX boot scenario
  This function is executed at system scope
  SGX:RestrictedBegin
  All BootScenario prints are on SGX_D_ERROR level,
  all BootingSvn prints are on SGX_D_VERBOSE level.
  SGX:RestrictedEnd

  @param[in]   KeyBlobsExist      Informs whether KeyBlobs exist
  @param[out]  BootScenario       This will define the boot scenario from SGX perspective
                                  should one of SGX_UEFI_BOOT_SCENARIO enum structure.

**/
EFI_STATUS
DetermineSgxBootScenario (
  IN  BOOLEAN                KeyBlobsExist,
  IN  BOOLEAN                SgxFactoryReset,
  OUT SGX_UEFI_BOOT_SCENARIO *BootScenario
  )
{
  EFI_STATUS Status                    = EFI_SUCCESS;
  UINT8      Index                     = 0;
  UINTN      LatestTimeStamp           = 0;
  UINTN      TimeStamp                 = 0;
  BOOLEAN    NotSameTimestamp          = FALSE;
  BOOLEAN    AllKeyBlobsHaveBootingSvn = FALSE;
  BOOLEAN    AnyKeyBlobHaveBootingSvn  = FALSE;
  BOOLEAN    IsAddPackageSupported     = TRUE;
  SGX_DEBUG ((SGX_D_ERROR, "[SGX] %a BEGIN\n", __FUNCTION__));

  if (BootScenario == NULL) {
    SGX_DEBUG ((SGX_D_ERROR, "  BootScenario is a NULL, please check input parameters!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit_DetermineSgxBootScenario;
  }

  mSgxInitDataHob.BootingSvn = MAX_UINT8;
  for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
    if (SocketPresent (Index) && (mSgxInitDataHob.PrResetSvn[Index] < mSgxInitDataHob.BootingSvn)) {
      mSgxInitDataHob.BootingSvn = mSgxInitDataHob.PrResetSvn[Index];
    }
  }
  SGX_DEBUG ((SGX_D_VERBOSE, "  BootingSvn = 0x%02x\n", mSgxInitDataHob.BootingSvn));

  if (!KeyBlobsExist || SgxFactoryReset) {
    *BootScenario = SgxFirstBinding;
    SGX_DEBUG ((SGX_D_ERROR, "  BootScenario = SgxFirstBinding\n"));
  } else {
    for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
      if (mSgxInitDataHob.UefiFwKeyBlobs[Index].Present) {
        LatestTimeStamp = EfiTimeToEpoch (&mSgxInitDataHob.UefiFwKeyBlobs[Index].TimeStamp);
        break;
      }
    }

    for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
      if (mSgxInitDataHob.UefiFwKeyBlobs[Index].Present) {
        TimeStamp = EfiTimeToEpoch(
            &mSgxInitDataHob.UefiFwKeyBlobs[Index].TimeStamp);
        if (LatestTimeStamp < TimeStamp) {
          LatestTimeStamp = TimeStamp;
          NotSameTimestamp = TRUE;
        } else if (LatestTimeStamp > TimeStamp) {
          NotSameTimestamp = TRUE;
        }
      }
    }

    if (NotSameTimestamp) {
      for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
        if (mSgxInitDataHob.UefiFwKeyBlobs[Index].Present
            && (LatestTimeStamp > EfiTimeToEpoch(&mSgxInitDataHob.UefiFwKeyBlobs[Index].TimeStamp))) {
          SGX_DEBUG ((SGX_D_VERBOSE, "  mSgxInitDataHob.UefiFwKeyBlobs[%d] contains an older than latest timestamp!\n", Index));
          mSgxInitDataHob.UefiFwKeyBlobs[Index].Present = FALSE;
        }
      }
    }

    Status = DoKeyBlobsHaveBootingSVN (&AllKeyBlobsHaveBootingSvn, &AnyKeyBlobHaveBootingSvn);
    if (EFI_ERROR (Status)) {
      SGX_DEBUG ((SGX_D_ERROR, "  Unable to run DoKeyBlobsHaveBootingSVN function, check input parameters!\n"));
      goto Exit_DetermineSgxBootScenario;
    }

    if (AnyKeyBlobHaveBootingSvn && !AllKeyBlobsHaveBootingSvn) {
      DeleteKeyBlobsWithoutBootingSVN ();
      SGX_DEBUG ((SGX_D_VERBOSE, "  Remaining keyblobs without key for BootingSvn were deleted!\n"));
      AllKeyBlobsHaveBootingSvn = TRUE;
    }

    if (AllKeyBlobsHaveBootingSvn) {
      if (KeyBlobsAvailableForAllPackagesAndPrids ()) {
        *BootScenario = SgxNormalBoot;
        SGX_DEBUG ((SGX_D_ERROR, "  BootScenario = SgxNormalBoot\n"));
      } else {
        if (PlatMemCertsFoundForAllPkgAndSvn ()) {
          *BootScenario = SgxAddPackageReboot;
          SGX_DEBUG ((SGX_D_ERROR, "  BootScenario = SgxAddPackageReboot\n"));
        } else {
          *BootScenario = SgxAddPackageBoot;
          SGX_DEBUG ((SGX_D_ERROR, "  BootScenario = SgxAddPackageBoot\n"));
        }
      }
    } else {
      if (KeyBlobsAvailableForAllPackagesAndPrids ()) {
        *BootScenario = SgxTcbRecovery;
        SGX_DEBUG ((SGX_D_ERROR, "  BootScenario = SgxTcbRecovery\n"));
      } else {
        mSgxInitDataHob.RegistrationState.SgxTcbRecoveryPending = TRUE;

        mSgxInitDataHob.BootingSvn = 0;
        for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
          if (mSgxInitDataHob.UefiFwKeyBlobs[Index].Present &&
              (mSgxInitDataHob.UefiFwKeyBlobs[Index].KeyBlob.CpuSvn > mSgxInitDataHob.BootingSvn)) {
            mSgxInitDataHob.BootingSvn = mSgxInitDataHob.UefiFwKeyBlobs[Index].KeyBlob.CpuSvn;
          }
          SGX_DEBUG ((SGX_D_VERBOSE, "  BootingSvn = 0x%02x (Advanced TCB Recovery)\n", mSgxInitDataHob.BootingSvn));
        }
        if (PlatMemCertsFoundForAllPkgAndSvn ()) {
          *BootScenario = SgxAddPackageReboot;
          SGX_DEBUG ((SGX_D_ERROR, "  BootScenario = SgxAddPackageReboot (Advanced TCB Recovery)\n"));
        } else {
          *BootScenario = SgxAddPackageBoot;
          SGX_DEBUG ((SGX_D_ERROR, "  BootScenario = SgxAddPackageBoot (Advanced TCB Recovery)\n"));
        }
      }
    }
  }

  // APB support from PCD
  IsAddPackageSupported = PcdGetBool (PcdSgxAddPackageSupport);
  // APB support override from DFX Knob
  if (mSecurityPolicy->DfxSgxAddPackageSupport != SECURITY_POLICY_DFX_AUTO) {
    IsAddPackageSupported = (mSecurityPolicy->DfxSgxAddPackageSupport == SECURITY_POLICY_DFX_ENABLE);
  }
  if ((*BootScenario == SgxAddPackageBoot || *BootScenario == SgxAddPackageReboot) && !IsAddPackageSupported) {
    // Force First Binding (IPE Initial Platform Establishment) Scenario
    SGX_DEBUG ((SGX_D_ERROR, "  BootScenario = SgxFirstBinding (Fallback from APB)\n"));
    *BootScenario = SgxFirstBinding;
    ZeroMem (mSgxInitDataHob.UefiFwKeyBlobs, sizeof (mSgxInitDataHob.UefiFwKeyBlobs));
  }

Exit_DetermineSgxBootScenario:
  SGX_DEBUG ((SGX_D_ERROR, "[SGX] %a END\n", __FUNCTION__));
  return Status;
}


/**

  Determine First Binding Complete
  Returns true if SW Agent reported Registration as Complete for the first time
  This function is executed at system scope

  @param[in]  RegState              Pointer to SGX_UEFIFW_REGISTRATION_STATE structure
  @param[out] FirstBindingComplete  Informs whether First binding has been completed

**/
VOID
DetermineFirstBindingComplete (
  IN  SGX_UEFIFW_REGISTRATION_STATE *RegState,
  OUT BOOLEAN                       *FirstBindingComplete
  )
{

  *FirstBindingComplete = (RegState->RegistrationStatusVariablePresent &&
                           !RegState->RequestPending &&
                           RegState->NonVolatile.RegistrationRequestType == SgxRequestPlatformManifest);

  SGX_DEBUG ((SGX_D_VERBOSE, "  Registration State, RequestPending: %d, PackageInfoPending: %d\n",
             (UINTN) RegState->RequestPending,
             (UINTN) RegState->PackageInfoPending));
  SGX_DEBUG ((SGX_D_VERBOSE, " SwAgentFirstBindingComplete = %d\n",
             *FirstBindingComplete));
}


/**

  Determine SGX:
   - First time boot,
   - boot scenario,
   - registration flow.

  @param[in]  KeyBlobsExistNvram    Informs whether KeyBlobs exist
  @param[in]  RegState              Registration State structure
  @param[out] FirstBoot             Informs whether it's first boot
  @param[out] BootScenario          Determined SGX boot scenario
  @param[out] FirstBindingComplete  Informs whether First binding has been completed

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED

**/
EFI_STATUS
SgxDetermineBootAndRegistrationScenario (
  IN  BOOLEAN                       KeyBlobsExistInNvram,
  IN  SGX_UEFIFW_REGISTRATION_STATE *RegState,
  IN  BOOLEAN                       SgxFactoryReset,
  OUT BOOLEAN                       *FirstBoot,
  OUT SGX_UEFI_BOOT_SCENARIO        *BootScenario,
  OUT BOOLEAN                       *FirstBindingComplete
  )
{
  EFI_STATUS      Status;

  // Determine boot scenarios
  DetermineSgxFirstBoot (FirstBoot);
  Status = DetermineSgxBootScenario (KeyBlobsExistInNvram, SgxFactoryReset, BootScenario);
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: Unable to determine SGX boot scenario.\n"));
  }

  // Determine registration flow
  DetermineFirstBindingComplete (RegState, FirstBindingComplete);

  return Status;
}

/**

  Enables Secure Enclaves in the Processor Feature Control MSR
  This function is executed on each thread in the system

  @param  None

  @retval None

**/
VOID
OptInSgxViaFeatureControl (
  VOID
  )
{
  MSR_IA32_FEATURE_CONTROL_REGISTER Msr;

  Msr.Uint64 = AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL);

  if (Msr.Bits.Lock) {
    SgxQueueDebugM (SGX_D_VERBOSE, PEI_21D_MSR_IA32_FEATURE_CONTROL, "locked");
    return;
  }

  Msr.Bits.SgxEnable = 1;

  //
  // BIOS opt in for SGX feature, LE_WR (thread scope)
  //
  if (mSecurityPolicy->SgxDebugMode) {
    Msr.Bits.SgxLaunchControlEnable = 0;
  }
  else if (mSecurityPolicy->SgxLeWr) {
    SgxQueueDebugM (SGX_D_VERBOSE, PEI_21E_FLEXIBLE_LAUNCH_CONTROL, "enabled");
    Msr.Bits.SgxLaunchControlEnable = 1;
  }

  AsmMsrOr64 (MSR_IA32_FEATURE_CONTROL, Msr.Uint64);

  SgxQueueDebugM (SGX_D_VERBOSE, PEI_21D_MSR_IA32_FEATURE_CONTROL, "= %08X", Msr.Uint64);
}


/**
  Create PRMRR(s) based on the CalculatePrmrr results
  This function is executed at core scope

  @param  Thread  information about thread being executed

  @retval None
**/
VOID
ConfigureCorePrmrrs (
  UINTN                         Thread
  )
{
  UINT8                         PrmrrIndex;
  UINT64                        ValidPrmrrBitsMask;
  UINT32                        PrmrrRegister[MAX_PRMRR_COUNT] =
                                  { SPR_MSR_PRMRR_BASE_0, SPR_MSR_PRMRR_BASE_1_1, SPR_MSR_PRMRR_BASE_2_2, SPR_MSR_PRMRR_BASE_3_3,
                                    SPR_MSR_PRMRR_BASE_4_4, SPR_MSR_PRMRR_BASE_5_5, SPR_MSR_PRMRR_BASE_6_6, SPR_MSR_PRMRR_BASE_7_7 };

  SgxQueueDebugM (SGX_D_VERBOSE, PEI_217_CONFIGURE_PRMRRS_BEGIN, NULL);

  for (PrmrrIndex = 0; PrmrrIndex < mSgxInitDataHob.SgxPreMemInitHob.PrmrrCount; PrmrrIndex++) {
    //
    // program the PRMRR_PHYSBASE_x
    //
    AsmMsrOr64 (
      PrmrrRegister[PrmrrIndex],
      (mSgxInitDataHob.SgxPreMemInitHob.PrmrrBase[PrmrrIndex] | EFI_CACHE_WRITEBACK | B_MSR_PRMRR_CONFIG_BIT)
      );

    SgxQueueDebugM (SGX_D_VERBOSE, PEI_219_PRMRR_BASE, "%d Thread[%d] = 0x%016lX",
                   PrmrrIndex, Thread, AsmReadMsr64 (SPR_MSR_PRMRR_BASE_0 + PrmrrIndex) );
  }

  //
  // program the PRMRR_PHYSMASK
  //
  ValidPrmrrBitsMask = (LShiftU64 (1, mPhysicalAddressBits) - 1) & 0xfffffffffffff000;
  mPrmrrMask = ValidPrmrrBitsMask & (~((UINT64) (mSecurityPolicy->PrmrrSize - 1)));

  SgxAsmMsrConditionalAndThenOr64 (
    SPR_MSR_PRMRR_MASK,
    B_MSR_PRMRR_PHYS_MASK_LOCK,
    ~(UINT64)(B_MSR_PRMRR_PHYS_MASK_VALID),
    mPrmrrMask | B_MSR_PRMRR_PHYS_MASK_LOCK
    );
  SgxQueueDebugM (SGX_D_VERBOSE, PEI_21A_PRMRR_MASK, "Thread[%d] = 0x%016lX",
                  Thread, AsmReadMsr64 (SPR_MSR_PRMRR_MASK) );

  SgxQueueDebugM (SGX_D_VERBOSE, PEI_218_CONFIGURE_PRMRRS_END, NULL);
}


/**

  Create M2M PRMRR(s) based on the CalculatePrmrr results

  @param  None

  @retval None

**/
VOID
ConfigureUncoreM2mPrmrrs (
  VOID
  )
{
  UINT8  PrmrrIndex     = 0;
  UINT8  SocketIndex    = 0;
  UINT8  ImcIndex       = 0;
  UINT64 PrmrrBase      = 0;
  UINT64 PrmrrMask      = 0;

  SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] ConfigureUncoreM2mPrmrrs BEGIN\n"));

  // Program each PRMRR to corresponding M2M into every socket
  PrmrrMask = mPrmrrMask | B_MSR_PRMRR_PHYS_MASK_VALID | B_MSR_PRMRR_PHYS_MASK_LOCK;

  for (PrmrrIndex = 0; PrmrrIndex < mSgxInitDataHob.SgxPreMemInitHob.PrmrrCount; PrmrrIndex++) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  PrmrrIndex = %d\n", PrmrrIndex));
    PrmrrBase = mSgxInitDataHob.SgxPreMemInitHob.PrmrrBase[PrmrrIndex] | EFI_CACHE_WRITEBACK | B_MSR_PRMRR_CONFIG_BIT;

    for (SocketIndex = 0; SocketIndex < mSocketCount; SocketIndex++) {
      // If socket interleaved, the PRMRR_BASE and PRMRR_MASK get programmed
      for (ImcIndex = 0; ImcIndex < GetMaxImc (); ImcIndex++) {
        if (((BIT0 << ImcIndex) & mSgxInitDataHob.SgxPreMemInitHob.McBitMap[SocketIndex][PrmrrIndex]) == 0) {
          continue;
        }

        // Each of the 64-bits MSR (Base and Mask) are splitted into two 32 CSRs
        mMrcHooksChipServices->SetM2mPrmrrBase (SocketIndex, ImcIndex, (UINT32)PrmrrBase, (UINT32)RShiftU64 (PrmrrBase, 32));
        mMrcHooksChipServices->SetM2mPrmrrMask (SocketIndex, ImcIndex, (UINT32)PrmrrMask, (UINT32)RShiftU64 (PrmrrMask, 32));
        SGX_DEBUG ((SGX_D_VERBOSE, "    SetM2mPrmrrBase (SocketIndex=%d, ImcIndex=%d, PrmrrBase=0x%016lX)\n",
          SocketIndex,
          ImcIndex,
          PrmrrBase
          ));
        SGX_DEBUG ((SGX_D_VERBOSE, "    SetM2mPrmrrMask (SocketIndex=%d, ImcIndex=%d, PrmrrMask=0x%016lX)\n",
          SocketIndex,
          ImcIndex,
          PrmrrMask
          ));
      } // for (ImcIndex)

      if (mSgxInitDataHob.SgxPreMemInitHob.McBitMap[SocketIndex][PrmrrIndex] != 0) {
        break; // One SGX range cannot span across sockets (NUMA)
      }
    } // for (SocketIndex)
  } // for (PrmrrIndex)

  SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] ConfigureUncoreM2mPrmrrs END\n"));
}


/**

  SGX early init flow at thread scope

  @param  None

  @retval None

**/
VOID
EnableSgxThreadScopeFirstTimeBoot (
  VOID
  )
{
#ifdef DEBUG
  EFI_STATUS  Status;
  UINTN       Thread;
  Status = mMpServices->WhoAmI (
    mPeiServices,
    mMpServices,
    &Thread
    );
  if (EFI_ERROR (Status)) {
    SgxQueueDebugM (SGX_D_VERBOSE, PEI_210_ERROR_DETECTING_THREAD, NULL);
    Thread = 0xBAD;
  }
  else {
  SgxQueueDebug (SGX_D_VERBOSE, "<Thread[%3X]>", Thread);
  }
#endif // DEBUG

  OptInSgxViaFeatureControl ();

#ifdef DEBUG
  SgxQueueDebug (SGX_D_VERBOSE, "</Thread[%3X]>", Thread);
#endif // DEBUG
}


/**

  SGX early init flow at core scope

  @param  None

  @retval None

**/
VOID
EnableSgxCoreScopeWarmReset (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       Thread;

  Status = mMpServices->WhoAmI (
    mPeiServices,
    mMpServices,
    &Thread
    );
  if (EFI_ERROR (Status)) {
#ifdef DEBUG
    SgxQueueDebugM (SGX_D_VERBOSE, PEI_210_ERROR_DETECTING_THREAD, NULL);
    Thread = 0xBAD;
#endif // DEBUG
    return;
  }

  //
  // Exit if the thread is not a primary thread
  //
  if (mProcInfo[Thread].Location.Thread != 0) {
    return;
  }
#ifdef DEBUG
  if (Thread == 0) {
    Core = 0;
  } else {
    Core = Core + 1;
  }
  SgxQueueDebug (SGX_D_VERBOSE, "<Core[%3X]>", Core);
#endif // DEBUG

  OptInSgxOverAdvancedRas ();

#ifndef PROACTIVE_ACTION_ON_CODE_ORGANIZATION
  if ((mSgxInitDataHob.SgxBootScenario != SgxAddPackageBoot)) {
    ConfigureCorePrmrrs (Thread);
  }
#endif // PROACTIVE_ACTION_ON_CODE_ORGANIZATION

#ifdef DEBUG
  SgxQueueDebug (SGX_D_VERBOSE, "</Core[%3X]>", Core);
#endif // DEBUG
}
/**

Extracts The Processor Registration ID (PRID) is a 128 bit unique ID derived from fuse.
During the Platform Registration and Platform Add this ID is used by the Registration Server to look up a package.

  @param[in]  Processor        This is the processor number

**/

VOID
ExtractPrResetSvn (
  IN UINTN    Processor
  )
{
  SPR_MSR_BIOS_SE_SVN_REGISTER Msr;
  //
  //

  //SPR_MSR_BIOS_SE_SVN
  Msr.Uint64 = AsmReadMsr64 (SPR_MSR_BIOS_SE_SVN);
  mSgxInitDataHob.PrResetSvn[Processor] = (UINT8)Msr.Bits.PrResetSvn;
}

/**

Extracts The Processor Registration ID (PRID) is a 128 bit unique ID derived from fuse.
During the Platform Registration and Platform Add this ID is used by the Registration Server to look up a package.

  @param[in]  Processor        This is the processor number

**/
VOID
EFIAPI
ExtractPrids (
  IN UINTN    Processor
  )
{
  mSgxInitDataHob.Socket[Processor].Prids_0 = AsmReadMsr64 (SPR_MSR_PRID_0_0);
  mSgxInitDataHob.Socket[Processor].Prids_1 = AsmReadMsr64 (SPR_MSR_PRID_1_1);
}


/**

  Enable Poison
  This function is executed at Package scope

  @param  None

  @retval None

**/
VOID
EnablePoison (
  VOID
  )
{
  if ((AsmReadMsr32 (SPR_MSR_MCG_CAP) & B_MSR_MCG_CAP_SER_P) != 0) {
    SgxAsmMsrConditionalOr64 (MSR_MCG_CONTAIN, B_MSR_MCG_CONTAIN_POISON_ENABLE, B_MSR_MCG_CONTAIN_POISON_ENABLE);
  }
}


/**

  When SGX is enabled in BIOS setup, BIOS opts in SGX over Advanced RAS
  This function is executed at Package scope

  @param  None

  @retval None

**/
VOID
OptInSgxOverAdvancedRas (
  VOID
  )
{
  SgxAsmMsrConditionalOr64 (
    SPR_MSR_SGX_RAS,
    B_SPR_MSR_SGX_RAS_OPTIN,
    B_SPR_MSR_SGX_RAS_OPTIN
    );
}


/**

  Confiure SGX Quality of Service feature in silicon
  This function is executed at package scope

  @param  None

  @retval None

**/
VOID
ConfigureSgxQoS (
  VOID
  )
{
  SgxAsmMsrConditionalOr64 (
    SPR_MSR_BIOS_PKG_CONFIG,
    B_BIOS_PKG_CONFIG_LOCK,
    B_BIOS_PKG_CONFIG_ENABLE | B_BIOS_PKG_CONFIG_LOCK
    );
  SgxQueueDebugM (SGX_D_VERBOSE, PEI_21F_SGX_QOS_ENABLED_AND_LOCKED, NULL);
}


/**

  Configre SGX OWNER EPOCH either from manual input in setup or using
  random generated value based on setup policy
  This function is executed at package scope

  @param  None

  @retval None

**/
VOID
ConfigureOwnerEpoch (
  VOID
  )
{
  //
  //
  if (mSecurityPolicy->EpochUpdate == SECURITY_POLICY_SGX_OWNER_EPOCH_ACTIVATED) {
    AsmWriteMsr64 (SPR_MSR_SE_EPOCH_0, mSecurityPolicy->SgxEpoch0);
    AsmWriteMsr64 (SPR_MSR_SE_EPOCH_1, mSecurityPolicy->SgxEpoch1);
  }
}

/**

  Internal helper function used for setting and checking M2M Sai registers

  @param[in] pSaiRegRegisterIndex - pointer to SAI register index table
  @param[in] SocketIndex - Socket ID
  @param[in] McIndex - Memory Controller ID
  @param[in] pValueMask - pointer to registers masks values to be set

  @retval EFI_SUCCESS Register value set properly
  @retval EFI_UNSUPPORTED difference between value set and read back

**/
EFI_STATUS
SetM2mSaiRegisters (
  IN UINT32 *pSaiRegRegisterIndex,
  IN UINT8  SocketIndex,
  IN UINT8  McIndex,
  IN UINT32 *pValueMask,
  IN UINT8  Type
  )
{
  UINT32 SaiLowValue;
  UINT32 SaiHighValue;
  // Read initial registers values
  mMrcHooksChipServices->GetSetM2mSaiRegisters (SocketIndex, McIndex, pSaiRegRegisterIndex[0], TRUE, &SaiLowValue);
  mMrcHooksChipServices->GetSetM2mSaiRegisters (SocketIndex, McIndex, pSaiRegRegisterIndex[1], TRUE, &SaiHighValue);
  // Set registers
  SaiLowValue = SaiLowValue & pValueMask[0];
  SaiHighValue = SaiHighValue & pValueMask[1];
  if (Type == SGX_SAI_WAC) {
    mMrcHooksChipServices->GetSetM2mSaiRegisters (SocketIndex, McIndex, pSaiRegRegisterIndex[0], FALSE, &SaiLowValue);
    mMrcHooksChipServices->GetSetM2mSaiRegisters (SocketIndex, McIndex, pSaiRegRegisterIndex[1], FALSE, &SaiHighValue);
  }
  if (Type == SGX_SAI_CP) {
    mMrcHooksChipServices->GetSetM2mSaiRegisters (SocketIndex, McIndex, pSaiRegRegisterIndex[1], FALSE, &SaiHighValue);
    mMrcHooksChipServices->GetSetM2mSaiRegisters (SocketIndex, McIndex, pSaiRegRegisterIndex[0], FALSE, &SaiLowValue);
  }
  // Read registers after write
  mMrcHooksChipServices->GetSetM2mSaiRegisters (SocketIndex, McIndex, pSaiRegRegisterIndex[0], TRUE, &SaiLowValue);
  mMrcHooksChipServices->GetSetM2mSaiRegisters (SocketIndex, McIndex, pSaiRegRegisterIndex[1], TRUE, &SaiHighValue);

  if ((SaiLowValue & ~(pValueMask[0])) || (SaiHighValue & ~(pValueMask[1]))) {
    SGX_DEBUG ((SGX_D_VERBOSE, " SAI register value (0x%8x%8x) does not match value set (0x%8x%8x)\n",
            SaiHighValue, SaiLowValue, pValueMask[1], pValueMask[0]));
    return EFI_UNSUPPORTED;
  } else {
    return EFI_SUCCESS;
  }
}

/**

  Internal helper function used for setting and checking CHA Sai registers

  @param[in] pSaiRegRegisterIndex - pointer to SAI register index table
  @param[in] SocketIndex - Socket ID
  @param[in] McIndex - Memory Controller ID
  @param[in] pValueMask - pointer to registers masks values to be set

  @retval EFI_SUCCESS Register value set properly
  @retval EFI_UNSUPPORTED difference between value set and read back

**/
EFI_STATUS
SetChaSaiRegisters (
  IN UINT32 *pSaiRegRegisterIndex,
  IN UINT8  SocketIndex,
  IN UINT8  McIndex,
  IN UINT32 *pValueMask,
  IN UINT8  Type
  )
{
  UINT32 SaiLowValue;
  UINT32 SaiHighValue;
  // Read initial registers values
  ChaGetSetSaiRegisters (SocketIndex, McIndex, pSaiRegRegisterIndex[0], TRUE, &SaiLowValue);
  ChaGetSetSaiRegisters (SocketIndex, McIndex, pSaiRegRegisterIndex[1], TRUE, &SaiHighValue);
  // Set registers
  SaiLowValue = SaiLowValue & pValueMask[0];
  SaiHighValue = SaiHighValue & pValueMask[1];
  if (Type == SGX_SAI_WAC) {
    ChaGetSetSaiRegisters (SocketIndex, McIndex, pSaiRegRegisterIndex[0], FALSE, &SaiLowValue);
    ChaGetSetSaiRegisters (SocketIndex, McIndex, pSaiRegRegisterIndex[1], FALSE, &SaiHighValue);
  }
  if (Type == SGX_SAI_CP) {
    ChaGetSetSaiRegisters (SocketIndex, McIndex, pSaiRegRegisterIndex[1], FALSE, &SaiHighValue);
    ChaGetSetSaiRegisters (SocketIndex, McIndex, pSaiRegRegisterIndex[0], FALSE, &SaiLowValue);
  }
  // Read registers after write
  ChaGetSetSaiRegisters (SocketIndex, McIndex, pSaiRegRegisterIndex[0], TRUE, &SaiLowValue);
  ChaGetSetSaiRegisters (SocketIndex, McIndex, pSaiRegRegisterIndex[1], TRUE, &SaiHighValue);

  if ((SaiLowValue & ~(pValueMask[0])) || (SaiHighValue & ~(pValueMask[1]))) {
    SGX_DEBUG ((SGX_D_VERBOSE, " SAI register value (0x%8x%8x) does not match value set (0x%8x%8x)\n",
            SaiHighValue, SaiLowValue, pValueMask[1], pValueMask[0]));
    return EFI_UNSUPPORTED;
  } else {
    return EFI_SUCCESS;
  }
}

/**

  Internal helper function used for setting and checking UBox Sai registers

  @param[in] pSaiRegRegisterIndex - pointer to SAI register index table
  @param[in] SocketIndex - Socket ID
  @param[in] BoxInst - Box Instance, 0 based
  @param[in] pValueMask - pointer to registers masks values to be set

  @retval EFI_SUCCESS Register value set properly
  @retval EFI_UNSUPPORTED difference between value set and read back

**/
EFI_STATUS
SetUboxSaiRegisters (
  IN UBOX_SAI_REG_ENUM *pSaiRegRegisterIndex,
  IN UINT8  SocketIndex,
  IN UINT8  BoxInst,
  IN UINT32 *pValueMask,
  IN UINT8  Type
  )
{
  UINT32 SaiLowValue;
  UINT32 SaiHighValue;
  // Read initial registers values
  UBoxGetSetSaiRegisters (SocketIndex, BoxInst, pSaiRegRegisterIndex[0], TRUE, &SaiLowValue);
  UBoxGetSetSaiRegisters (SocketIndex, BoxInst, pSaiRegRegisterIndex[1], TRUE, &SaiHighValue);
  // Set registers
  SaiLowValue = SaiLowValue & pValueMask[0];
  SaiHighValue = SaiHighValue & pValueMask[1];
  if (Type == SGX_SAI_WAC) {
    UBoxGetSetSaiRegisters (SocketIndex, BoxInst, pSaiRegRegisterIndex[0], FALSE, &SaiLowValue);
    UBoxGetSetSaiRegisters (SocketIndex, BoxInst, pSaiRegRegisterIndex[1], FALSE, &SaiHighValue);
  }
  if (Type == SGX_SAI_CP) {
    UBoxGetSetSaiRegisters (SocketIndex, BoxInst, pSaiRegRegisterIndex[1], FALSE, &SaiHighValue);
    UBoxGetSetSaiRegisters (SocketIndex, BoxInst, pSaiRegRegisterIndex[0], FALSE, &SaiLowValue);
  }
  // Read registers after write
  UBoxGetSetSaiRegisters (SocketIndex, BoxInst, pSaiRegRegisterIndex[0], TRUE, &SaiLowValue);
  UBoxGetSetSaiRegisters (SocketIndex, BoxInst, pSaiRegRegisterIndex[1], TRUE, &SaiHighValue);

  if ((SaiLowValue & ~(pValueMask[0])) || (SaiHighValue & ~(pValueMask[1]))) {
    SGX_DEBUG ((SGX_D_VERBOSE, " SAI register value (0x%8x%8x) does not match value set (0x%8x%8x)\n",
            SaiHighValue, SaiLowValue, pValueMask[1], pValueMask[0]));
    return EFI_UNSUPPORTED;
  } else {
    return EFI_SUCCESS;
  }
}

/**

  Internal helper function used for setting and checking Sai registers

  @param[in] pSaiRegRegisterIndex - pointer to SAI register index table
  @param[in] SocketIndex - Socket ID
  @param[in] BoxInst - Box Instance, 0 based
  @param[in] pValueMask - pointer to registers masks values to be set

  @retval EFI_SUCCESS Register value set properly
  @retval EFI_UNSUPPORTED difference between value set and read back

**/
EFI_STATUS
SetSaiRegisters (
  IN UINT8  SocketIndex,
  IN UINT8  BoxInst,
  IN UINT32 *pRegisterOffset,
  IN UINT32 *pValueMask,
  IN UINT8  Type
  )
{
  UINT32 SaiLowValue;
  UINT32 SaiHighValue;
  // Read initial registers values
  SaiLowValue  = mMrcHooksChipServices->UsraCsrRead (SocketIndex, BoxInst, pRegisterOffset[0]);
  SaiHighValue = mMrcHooksChipServices->UsraCsrRead (SocketIndex, BoxInst, pRegisterOffset[1]);
  // Set registers
  SaiLowValue  = SaiLowValue & pValueMask[0];
  SaiHighValue = SaiHighValue & pValueMask[1];
  if (Type == SGX_SAI_WAC) {
    mMrcHooksChipServices->UsraCsrWrite (SocketIndex, CSR_MULTICAST, pRegisterOffset[1], SaiHighValue);
    mMrcHooksChipServices->UsraCsrWrite (SocketIndex, CSR_MULTICAST, pRegisterOffset[0], SaiLowValue);
  }
  if (Type == SGX_SAI_CP) {
    mMrcHooksChipServices->UsraCsrWrite (SocketIndex, CSR_MULTICAST, pRegisterOffset[1], SaiHighValue);
    mMrcHooksChipServices->UsraCsrWrite (SocketIndex, CSR_MULTICAST, pRegisterOffset[0], SaiLowValue);
  }
  // Read registers after write
  SaiLowValue  = mMrcHooksChipServices->UsraCsrRead (SocketIndex, BoxInst, pRegisterOffset[0]);
  SaiHighValue = mMrcHooksChipServices->UsraCsrRead (SocketIndex, BoxInst, pRegisterOffset[1]);

  if ((SaiLowValue & ~(pValueMask[0])) || (SaiHighValue & ~(pValueMask[1]))) {
    SGX_DEBUG ((SGX_D_ERROR, " SAI register value (0x%8x%8x) does not match value set (0x%8x%8x)\n",
            SaiHighValue, SaiLowValue, pValueMask[1], pValueMask[0]));
    return EFI_UNSUPPORTED;
  } else {
    return EFI_SUCCESS;
  }
}

/**

  Internal helper function used for setting and checking USRA CSR Sai registers

  @param[in] SocketIndex - Socket ID
  @param[in] McIndex - Memory Controller ID
  @param[in] pRegisterOffset - USRA register offset
  @param[in] pValueMask - pointer to registers masks values to be set

  @retval EFI_SUCCESS Register value set properly
  @retval EFI_UNSUPPORTED difference between value set and read back

**/
EFI_STATUS
SetUsraCsrSaiRegisters (
  IN UINT8  SocketIndex,
  IN UINT8  BoxInst,
  IN UINT32 *pRegisterOffset,
  IN UINT32 *pValueMask,
  IN UINT8  Type
  )
{
  // Read initial registers values
  UINT32 SaiLowValue  = mMrcHooksChipServices->UsraCsrRead (SocketIndex, BoxInst, pRegisterOffset[0]);
  UINT32 SaiHighValue = mMrcHooksChipServices->UsraCsrRead (SocketIndex, BoxInst, pRegisterOffset[1]);
  // Set registers
  SaiLowValue = SaiLowValue & pValueMask[0];
  SaiHighValue = SaiHighValue & pValueMask[1];
  if (Type == SGX_SAI_WAC) {
    mMrcHooksChipServices->UsraCsrWrite (SocketIndex, BoxInst, pRegisterOffset[0], SaiLowValue);
    mMrcHooksChipServices->UsraCsrWrite (SocketIndex, BoxInst, pRegisterOffset[1], SaiHighValue);
  }
  if (Type == SGX_SAI_CP) {
    mMrcHooksChipServices->UsraCsrWrite (SocketIndex, BoxInst, pRegisterOffset[1], SaiHighValue);
    mMrcHooksChipServices->UsraCsrWrite (SocketIndex, BoxInst, pRegisterOffset[0], SaiLowValue);
  }
  // Read registers after write
  SaiLowValue  = mMrcHooksChipServices->UsraCsrRead (SocketIndex, BoxInst, pRegisterOffset[0]);
  SaiHighValue = mMrcHooksChipServices->UsraCsrRead (SocketIndex, BoxInst, pRegisterOffset[1]);

  if ((SaiLowValue & ~(pValueMask[0])) || (SaiHighValue & ~(pValueMask[1]))) {
    SGX_DEBUG ((SGX_D_VERBOSE, " SAI register value (0x%8x%8x) does not match value set (0x%8x%8x)\n",
            SaiHighValue, SaiLowValue, pValueMask[1], pValueMask[0]));
    return EFI_UNSUPPORTED;
  } else {
    return EFI_SUCCESS;
  }
}

/**

  Internal helper function that processes M2M Sai registers

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED difference between register value set and read back

**/
EFI_STATUS
HandleM2mSaiForSgx (
  VOID
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT8      SocketIndex;
  UINT8      ImcIndex;
  UINT32     SaiLowValue;
  UINT32     McheckSaiPolicyMask[] = { MCHECK_SAI_POLICY_LOW_MASK, MCHECK_SAI_POLICY_HIGH_MASK };
  UINT32     RacM2mSaiRegIndex = M2mLbMcheckWRacN0RegIndex;
  UINT32     CpM2mSaiRegIndex[2] = { M2mLbMcheckWCpN0RegIndex, M2mLbMcheckWCpN1RegIndex };
  UINT32     WacM2mSaiRegIndex[2] = { M2mLbMcheckWWacN0RegIndex, M2mLbMcheckWWacN1RegIndex };

  for (SocketIndex = 0; SocketIndex < MCHECK_MAX_PACKAGES; SocketIndex++) {
    if (!SocketPresent (SocketIndex)) {
      continue;
    }
    for (ImcIndex = 0; ImcIndex < GetMaxImc (); ImcIndex++) {
      // RAC
      mMrcHooksChipServices->GetSetM2mSaiRegisters (SocketIndex, ImcIndex, RacM2mSaiRegIndex, TRUE, &SaiLowValue);
      if ((SaiLowValue & B_HOSTIA_POSTBOOT_SAI) == 0) {
        SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong M2M SAI RAC Register with address: 0x%X, value: 0x%16X, Socket: %d, IMC: %d.\n",
                RacM2mSaiRegIndex, SaiLowValue, SocketIndex, ImcIndex));
        return EFI_UNSUPPORTED;
      }
      // WAC
      Status = SetM2mSaiRegisters (WacM2mSaiRegIndex, SocketIndex, ImcIndex, McheckSaiPolicyMask, SGX_SAI_WAC);
      if (Status != EFI_SUCCESS) {
        SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong M2M SAI WAC Register with address: 0x%X,Socket: %d, IMC: %d.\n",
                WacM2mSaiRegIndex[0], SocketIndex, ImcIndex));
        return Status;
      }
      // CP
      Status = SetM2mSaiRegisters (CpM2mSaiRegIndex, SocketIndex, ImcIndex, McheckSaiPolicyMask, SGX_SAI_CP);
      if (Status != EFI_SUCCESS) {
        SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong M2M SAI CP Register with address: 0x%X, Socket: %d, IMC: %d.\n",
                CpM2mSaiRegIndex[0], SocketIndex, ImcIndex));
        return Status;
      }
    }
  }
  return Status;
}

/**

  Internal helper function that processes CHA Sai registers

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED difference between register value set and read back

**/
EFI_STATUS
HandleChaSaiForSgx (
  VOID
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT8      SocketIndex;
  UINT8      SaiIndex;
  UINT32     SaiLowValue = 0;
  UINTN      NumberOfChaSais = 0;
  UINT32     McheckSaiPolicyMask[] = { MCHECK_SAI_POLICY_LOW_MASK, MCHECK_SAI_POLICY_HIGH_MASK };
  UINT32     RacChaSaiRegIndex[] = { ChaLbWMcheckRac0x3FFN0RegIndex, ChaLbWMcheckRacN0RegIndex };
  UINT32     CpChaSaiRegIndex[][2] = {
                                       { ChaLbWMcheckCp0x3FFN0RegIndex, ChaLbWMcheckCp0x3FFN1RegIndex },
                                       { ChaLbWMcheckCpN0RegIndex, ChaLbWMcheckCpN1RegIndex }
                                     };
  UINT32     WacChaSaiRegIndex[][2] = {
                                        { ChaLbWMcheckWac0x3FFN0RegIndex, ChaLbWMcheckWac0x3FFN1RegIndex },
                                        { ChaLbWMcheckWacN0RegIndex, ChaLbWMcheckWacN1RegIndex }
                                      };
  NumberOfChaSais  =  ARRAY_SIZE (RacChaSaiRegIndex);

  for (SocketIndex = 0; SocketIndex < MCHECK_MAX_PACKAGES; SocketIndex++) {
    if (!SocketPresent (SocketIndex)) {
      continue;
    }
    for (SaiIndex = 0; SaiIndex < NumberOfChaSais; SaiIndex++ ) {
      // RAC
      ChaGetSetSaiRegisters (SocketIndex, 0, RacChaSaiRegIndex[SaiIndex], TRUE, &SaiLowValue);
      if ((SaiLowValue & B_HOSTIA_POSTBOOT_SAI) == 0) {
        SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong SAI RAC Register with address: 0x%X, value: 0x%16X.\n",
                RacChaSaiRegIndex[SaiIndex], SaiLowValue));
        return EFI_UNSUPPORTED;
      }
      // WAC
      Status = SetChaSaiRegisters (WacChaSaiRegIndex[SaiIndex], SocketIndex, 0, McheckSaiPolicyMask, SGX_SAI_WAC);
      if (Status != EFI_SUCCESS) {
        SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong SAI WAC Register with address: 0x%X.\n",
                WacChaSaiRegIndex[SaiIndex][0]));
        return Status;
      }
      // CP
      Status = SetChaSaiRegisters (CpChaSaiRegIndex[SaiIndex], SocketIndex, 0, McheckSaiPolicyMask, SGX_SAI_CP);
      if (Status != EFI_SUCCESS) {
        SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong SAI CP Register with address: 0x%X\n",
                WacChaSaiRegIndex[SaiIndex][0]));
        return Status;
      }
    }
  }
  return Status;
}

/**

  Internal helper function that processes Ubox Sai registers

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED difference between register value set and read back

**/
EFI_STATUS
HandleUboxSaiForSgx (
  VOID
  )
{
  EFI_STATUS        Status = EFI_SUCCESS;
  UINT8             SocketIndex;
  UINT32            SaiLowValue = 0;
  UINT32            McheckSaiPolicyMask[] = { MCHECK_SAI_POLICY_LOW_MASK, MCHECK_SAI_POLICY_HIGH_MASK };
  UBOX_SAI_REG_ENUM RacUboxSaiRegIndex[] = { UboxLbWMcheckRacN0RegIndex };
  UBOX_SAI_REG_ENUM WacUboxSaiRegIndex[] = { UboxLbWMcheckWacN0RegIndex, UboxLbWMcheckWacN1RegIndex };
  UBOX_SAI_REG_ENUM CpUboxSaiRegIndex[] = { UboxLbWMcheckCpN0RegIndex, UboxLbWMcheckCpN1RegIndex };

  for (SocketIndex = 0; SocketIndex < MCHECK_MAX_PACKAGES; SocketIndex++) {
    if (!SocketPresent (SocketIndex)) {
      continue;
    }
    // RAC
    UBoxGetSetSaiRegisters (SocketIndex, 0, RacUboxSaiRegIndex[0], TRUE, &SaiLowValue);
    if ((SaiLowValue & B_HOSTIA_POSTBOOT_SAI) == 0) {
      SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong SAI UBOX Register with index: 0x%X, value: 0x%16X.\n",
              RacUboxSaiRegIndex, SaiLowValue));
      return EFI_UNSUPPORTED;
    }
    // WAC
    Status = SetUboxSaiRegisters (WacUboxSaiRegIndex, SocketIndex, 0, McheckSaiPolicyMask, SGX_SAI_WAC);
    if (Status != EFI_SUCCESS) {
      SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong SAI WAC Register with index: 0x%X.\n",
              CpUboxSaiRegIndex[0]));
      return Status;
    }
    // CP
    Status = SetUboxSaiRegisters (CpUboxSaiRegIndex, SocketIndex, 0, McheckSaiPolicyMask, SGX_SAI_CP);
    if (Status != EFI_SUCCESS) {
      SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong SAI CP Register with index: 0x%X.\n",
              CpUboxSaiRegIndex[0]));
      return EFI_UNSUPPORTED;
    }
  }
  return Status;
}

/**

  Internal helper function that checks if given
  KtiLink is enabled.

  @retval TRUE  KtiLink is enabled
  @retval FALSE KtiLink is disabled

**/
BOOLEAN
IsKtiLinkEnabled (
  UINT8 SocketIndex,
  UINT8 KtiLinkIndex
  )
{
  UINT64 KtiDisabledBitmap = 0;
  UINT8  KtiPortCount      = 0;

  KtiDisabledBitmap = mMrcHooksChipServices->UsraCsrRead (SocketIndex, 0, CAPID2_PCU_FUN3_REG);
  KtiDisabledBitmap = RShiftU64(KtiDisabledBitmap, 23); // KtiLink capability bits starts in CAPID2 with BIT23

  KtiPortCount = GetKtiPortCnt ();
  if ((KtiPortCount == 0) ||
      (KtiLinkIndex > (KtiPortCount - 1)) ||
      ((RShiftU64(KtiDisabledBitmap, KtiLinkIndex) & BIT0) == BIT0)) {
    return FALSE;
  }
  return TRUE;
}

/**

  Internal helper function that processes UPI AGT Sai registers

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED difference between register value set and read back

**/
EFI_STATUS
HandleUpiagtSaiForSgx (
  VOID
  )
{
  EFI_STATUS             Status = EFI_SUCCESS;
  UINT8                  SocketIndex;
  UINT8                  KtiLink;
  UINT32                 SaiLowValue = 0;
  UINT32                 McheckSaiPolicyMask[] = { MCHECK_SAI_POLICY_LOW_MASK, MCHECK_SAI_POLICY_HIGH_MASK };
  UINT32                 UpiagtWacRegOffset[] = { UPIAGT_LB_MCHECK_W_WAC_LO_KTI_LLPMON_REG,
                                                  UPIAGT_LB_MCHECK_W_WAC_HI_KTI_LLPMON_REG };
  UINT32                 UpiagtCpRegOffset[] = { UPIAGT_LB_MCHECK_W_CP_LO_KTI_LLPMON_REG,
                                                 UPIAGT_LB_MCHECK_W_CP_HI_KTI_LLPMON_REG };
  for (SocketIndex = 0; SocketIndex < MCHECK_MAX_PACKAGES; SocketIndex++) {
    if (!SocketPresent (SocketIndex)) {
      continue;
    }
    for (KtiLink = 0; KtiLink < GetKtiPortCnt (); KtiLink++) {
      if (IsKtiLinkEnabled (SocketIndex, KtiLink)) {
        // RAC
        SaiLowValue = mMrcHooksChipServices->UsraCsrRead (SocketIndex, KtiLink, UPIAGT_LB_MCHECK_W_RAC_LO_KTI_LLPMON_REG);
        if ((SaiLowValue & B_HOSTIA_POSTBOOT_SAI) == 0) {
          SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong KTI SAI RAC Register with address: 0x%X, value: 0x%16X.\n",
                  UPIAGT_LB_MCHECK_W_RAC_LO_KTI_LLPMON_REG, SaiLowValue));
          return EFI_UNSUPPORTED;
        }
        // WAC
        Status = SetUsraCsrSaiRegisters (SocketIndex, KtiLink, UpiagtWacRegOffset, McheckSaiPolicyMask, SGX_SAI_WAC);
        if (Status != EFI_SUCCESS) {
          SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong KTI SAI WAC Register.\n"));
          return Status;
        }
        // CP
        Status = SetUsraCsrSaiRegisters (SocketIndex, KtiLink, UpiagtCpRegOffset, McheckSaiPolicyMask, SGX_SAI_CP);
        if (Status != EFI_SUCCESS) {
          SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong KTI SAI CP Register.\n"));
          return Status;
        }
      }
    }
  }
  return Status;
}

/**

  Internal helper function that processes MS2IDI0 Sai registers

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED difference between register value set and read back

**/
EFI_STATUS
HandleMs2idiSaiForSgx (
  VOID
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT8      SocketIndex;
  UINT32     SaiLowValue = 0;
  UINT32     McheckSaiPolicyMask[] = { MCHECK_SAI_POLICY_LOW_MASK, MCHECK_SAI_POLICY_HIGH_MASK };
  UINT32     Ms2idiWacRegOffset[] = { LB_MCHECK_W_WAC_N0_MS2IDI_MAIN_REG, LB_MCHECK_W_WAC_N1_MS2IDI_MAIN_REG };
  UINT32     Ms2idiCpRegOffset[] = { LB_MCHECK_W_CP_N0_MS2IDI_MAIN_REG, LB_MCHECK_W_CP_N1_MS2IDI_MAIN_REG };

  for (SocketIndex = 0; SocketIndex < MCHECK_MAX_PACKAGES; SocketIndex++) {
    if (!SocketPresent (SocketIndex)) {
      continue;
    }
    // RAC
    SaiLowValue = mMrcHooksChipServices->UsraCsrRead (SocketIndex, 0, LB_MCHECK_W_RAC_N0_MS2IDI_MAIN_REG);
    if ((SaiLowValue & B_HOSTIA_POSTBOOT_SAI) == 0) {
      SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong M2IDI0 SAI RAC Register, value: 0x%16X, exiting...\n", SaiLowValue));
      return EFI_UNSUPPORTED;
    }
    // WAC
    Status = SetSaiRegisters (SocketIndex, 0, Ms2idiWacRegOffset, McheckSaiPolicyMask, SGX_SAI_WAC);
    if (Status != EFI_SUCCESS) {
      SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong M2IDI0 SAI WAC Register.\n"));
      return Status;
    }
    // CP
    Status = SetSaiRegisters (SocketIndex, 0, Ms2idiCpRegOffset, McheckSaiPolicyMask, SGX_SAI_CP);
    if (Status != EFI_SUCCESS) {
      SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong M2IDI0 SAI CP Register.\n"));
      return Status;
    }
  }
  return Status;
}

/**

  Internal helper function that processes M3KTI Sai registers

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED difference between register value set and read back

**/
EFI_STATUS
HandleM3ktiSaiForSgx (
  VOID
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT8      SocketIndex;
  UINT32     SaiLowValue = 0;
  UINT8      KtiLink;
  UINT32     McheckSaiPolicyMask[] = { MCHECK_SAI_POLICY_LOW_MASK, MCHECK_SAI_POLICY_HIGH_MASK };
#ifdef GNRSRF_HOST
  UINT32     M3ktiWacRegOffset[] = { B2UPI_LB_MCHECK_W_WAC_LO_M3KTI_MAIN_REG, B2UPI_LB_MCHECK_W_WAC_HI_M3KTI_MAIN_REG };
#else // GNRSRF_HOST
  UINT32     M3ktiWacRegOffset[] = { M2UPI_LB_MCHECK_W_WAC_LO_M3KTI_MAIN_REG, M2UPI_LB_MCHECK_W_WAC_HI_M3KTI_MAIN_REG };
  UINT32     M3ktiCpRegOffset[] = { M2UPI_LB_MCHECK_W_CP_LO_M3KTI_MAIN_REG, M2UPI_LB_MCHECK_W_CP_HI_M3KTI_MAIN_REG };
#endif // GNRSRF_HOST

  for (SocketIndex = 0; SocketIndex < MCHECK_MAX_PACKAGES; SocketIndex++) {
    if (!SocketPresent (SocketIndex)) {
      continue;
    }
    for (KtiLink = 0; KtiLink < GetKtiPortCnt (); KtiLink++) {
      if (IsKtiLinkEnabled (SocketIndex, KtiLink)) {
        // RAC
#ifdef GNRSRF_HOST
        SaiLowValue = mMrcHooksChipServices->UsraCsrRead (SocketIndex, KtiLink, B2UPI_LB_MCHECK_W_RAC_LO_M3KTI_MAIN_REG);
#else // GNRSRF_HOST
        SaiLowValue = mMrcHooksChipServices->UsraCsrRead (SocketIndex, KtiLink, M2UPI_LB_MCHECK_W_RAC_LO_M3KTI_MAIN_REG);
#endif // GNRSRF_HOST
        if ((SaiLowValue & B_HOSTIA_POSTBOOT_SAI) == 0) {
          SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong M3KTI SAI RAC Register, value: 0x%16X, exiting...\n", SaiLowValue));
          return EFI_UNSUPPORTED;
        }
        // WAC
        Status = SetUsraCsrSaiRegisters (SocketIndex, KtiLink, M3ktiWacRegOffset, McheckSaiPolicyMask, SGX_SAI_WAC);
        if (Status != EFI_SUCCESS) {
          SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong M3KTI SAI WAC Register.\n"));
          return Status;
        }
#ifndef GNRSRF_HOST
        // CP
        Status = SetUsraCsrSaiRegisters (SocketIndex, KtiLink, M3ktiCpRegOffset, McheckSaiPolicyMask, SGX_SAI_CP);
        if (Status != EFI_SUCCESS) {
          SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong M3KTI SAI CP Register.\n"));
          return Status;
        }
#endif // !GNRSRF_HOST
      }
    }
  }
  return Status;
}

/**

  Internal helper function that processes NCDECS Sai registers

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED difference between register value set and read back

**/
EFI_STATUS
HandleNcdecsSaiForSgx (
  VOID
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT8      SocketIndex;
  UINT32     SaiLowValue = 0;
  UINT32     McheckSaiPolicyMask[] = { MCHECK_SAI_POLICY_LOW_MASK, MCHECK_SAI_POLICY_HIGH_MASK };
  UINT32     NcdecsWacRegOffset[] = { LB_MCHECK_W_WAC_N0_UBOX_DFX_REG, LB_MCHECK_W_WAC_N1_UBOX_DFX_REG };
  UINT32     NcdecsCpRegOffset[] = { LB_MCHECK_W_CP_N0_UBOX_DFX_REG, LB_MCHECK_W_CP_N1_UBOX_DFX_REG };
  UINT8      UboxInst;

  for (SocketIndex = 0; SocketIndex < MCHECK_MAX_PACKAGES; SocketIndex++) {
    if (!SocketPresent (SocketIndex)) {
      continue;
    }
    for (UboxInst = 0; UboxInst < MAX_ALL_UBOX; UboxInst++) {
      // RAC
      SaiLowValue = mMrcHooksChipServices->UsraCsrRead (SocketIndex, UboxInst, LB_MCHECK_W_RAC_N0_UBOX_DFX_REG);
      if ((SaiLowValue & B_HOSTIA_POSTBOOT_SAI) == 0) {
        SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong NCDECS SAI RAC Register, value: 0x%16X, exiting...\n", SaiLowValue));
        return EFI_UNSUPPORTED;
      }
      // WAC
      Status = SetUsraCsrSaiRegisters (SocketIndex, UboxInst, NcdecsWacRegOffset, McheckSaiPolicyMask, SGX_SAI_WAC);
      if (Status != EFI_SUCCESS) {
        SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong NCDECS SAI WAC Register.\n"));
        return Status;
      }
      // CP
      Status = SetUsraCsrSaiRegisters (SocketIndex, UboxInst, NcdecsCpRegOffset, McheckSaiPolicyMask, SGX_SAI_CP);
      if (Status != EFI_SUCCESS) {
        SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong NCDECS SAI CP Register.\n"));
        return Status;
      }
    }
  }
  return Status;
}

/**

  Internal helper function that processes UPIPH Sai registers

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED difference between register value set and read back

**/
EFI_STATUS
HandleUpiphSaiForSgx (
  VOID
  )
{
  EFI_STATUS             Status = EFI_SUCCESS;
  UINT8                  SocketIndex;
  UINT32                 SaiLowValue = 0;
  UINT32                 McheckSaiPolicyMask[] = { MCHECK_SAI_POLICY_LOW_MASK, MCHECK_SAI_POLICY_HIGH_MASK };
  UINT32                 M3ktiWacRegOffset[] = { UPIPH_LB_MCHECK_W_WAC_LO_KTI_REUT_REG, UPIPH_LB_MCHECK_W_WAC_HI_KTI_REUT_REG };
  UINT32                 M3ktiCpRegOffset[] = { UPIPH_LB_MCHECK_W_CP_LO_KTI_REUT_REG, UPIPH_LB_MCHECK_W_CP_HI_KTI_REUT_REG };
  UINT8                  KtiLink;

  for (SocketIndex = 0; SocketIndex < MCHECK_MAX_PACKAGES; SocketIndex++) {
    if (!SocketPresent (SocketIndex)) {
      continue;
    }
    for (KtiLink = 0; KtiLink < GetKtiPortCnt (); KtiLink++) {
      if (IsKtiLinkEnabled (SocketIndex, KtiLink)) {
        // RAC
        SaiLowValue = mMrcHooksChipServices->UsraCsrRead (SocketIndex, KtiLink, UPIPH_LB_MCHECK_W_RAC_LO_KTI_REUT_REG);
        if ((SaiLowValue & B_HOSTIA_POSTBOOT_SAI) == 0) {
          SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong UPIPH SAI RAC Register, value: 0x%16X, exiting...\n", SaiLowValue));
          return EFI_UNSUPPORTED;
        }
        // WAC
        Status = SetUsraCsrSaiRegisters (SocketIndex, KtiLink, M3ktiWacRegOffset, McheckSaiPolicyMask, SGX_SAI_WAC);
        if (Status != EFI_SUCCESS) {
          SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong UPIPH SAI WAC Register.\n"));
          return Status;
        }
        // CP
        Status = SetUsraCsrSaiRegisters (SocketIndex, KtiLink, M3ktiCpRegOffset, McheckSaiPolicyMask, SGX_SAI_CP);
        if (Status != EFI_SUCCESS) {
          SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong UPIPH SAI CP Register.\n"));
          return Status;
        }
      }
    }
  }
  return Status;
}

/**

  Internal helper function that processes 2LM CNTL Sai registers

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED difference between register value set and read back

**/
EFI_STATUS
Handle2lmcntlSaiForSgx (
  VOID
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT8      SocketIndex;
  UINT8      ImcIndex;
  UINT32     SaiLowValue = 0;
  UINT32     McheckSaiPolicyMask[] = { MCHECK_SAI_POLICY_LOW_MASK, MCHECK_SAI_POLICY_HIGH_MASK };
  UINT32     Mc2lmcntlCpRegOffset[] = { MC_2LMCNTL_SAI_LB_MCHECK_W_PG_CR_POLICY_N0_MC_2LM_REG,
                                        MC_2LMCNTL_SAI_LB_MCHECK_W_PG_CR_POLICY_N1_MC_2LM_REG };
  UINT32     Mc2lmcntlWacRegOffset[] = { MC_2LMCNTL_SAI_LB_MCHECK_W_PG_WR_POLICY_N0_MC_2LM_REG,
                                         MC_2LMCNTL_SAI_LB_MCHECK_W_PG_WR_POLICY_N1_MC_2LM_REG };

  for (SocketIndex = 0; SocketIndex < MCHECK_MAX_PACKAGES; SocketIndex++) {
    if (!SocketPresent (SocketIndex)) {
      continue;
    }
    for (ImcIndex = 0; ImcIndex < GetMaxImc () * GetNumChannelPerMc (); ImcIndex++) {
      // RAC
      SaiLowValue = mMrcHooksChipServices->UsraCsrRead (SocketIndex, ImcIndex, MC_2LMCNTL_SAI_LB_MCHECK_W_PG_RD_POLICY_N0_MC_2LM_REG);
      if ((SaiLowValue & B_HOSTIA_POSTBOOT_SAI) == 0) {
        SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong MCHAN SAI RAC Register with address: 0x%X, value: 0x%16X, Socket: %d, IMC: %d.\n",
                MC_2LMCNTL_SAI_LB_MCHECK_W_PG_RD_POLICY_N0_MC_2LM_REG, SaiLowValue, SocketIndex, ImcIndex));
        return EFI_UNSUPPORTED;
      }
      // WAC
      Status = SetUsraCsrSaiRegisters (SocketIndex, ImcIndex, Mc2lmcntlWacRegOffset, McheckSaiPolicyMask, SGX_SAI_WAC);
      if (Status != EFI_SUCCESS) {
        SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong MCHAN SAI WAC Register.\n"));
        return Status;
      }
      // CP
      Status = SetUsraCsrSaiRegisters (SocketIndex, ImcIndex, Mc2lmcntlCpRegOffset, McheckSaiPolicyMask, SGX_SAI_CP);
      if (Status != EFI_SUCCESS) {
        SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Wrong MCHAN SAI CP Register.\n"));
        return Status;
      }
    }
  }
  return Status;
}

/**

  Configure SAI to meet SGX requirements only when SGX is enabled
  This function is executed at package scope

  @param  None

  @retval EFI_SUCCESS   SAI policies for SGX are well configured
  @retval Otherwise     One or more of the SAI policies are not configured as expected

**/
EFI_STATUS
ConfigureSaiForSgx (
  VOID
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_STATUS (*SaiHandlers[])(VOID) = { HandleM2mSaiForSgx, HandleChaSaiForSgx, HandleUboxSaiForSgx,
                                        HandleUpiagtSaiForSgx, HandleMs2idiSaiForSgx, HandleM3ktiSaiForSgx,
                                        HandleNcdecsSaiForSgx, HandleUpiphSaiForSgx, Handle2lmcntlSaiForSgx };
  UINT8      HandlerIndex;

  for (HandlerIndex = 0; HandlerIndex < ARRAY_SIZE(SaiHandlers);  HandlerIndex++) {
    Status = (*SaiHandlers[HandlerIndex]) ();
    if (Status != EFI_SUCCESS) {
      break;
    }
  }

  return Status;
}

/**

  Configure BIOS_SE_SVN MSR based on SVN info of Startup ACM, BIOS Guard & SINIT ACM
  This function is executed at package scope

  @param  None

  @retval EFI_STATUS

**/
EFI_STATUS
ConfigureSgxSvn (
  VOID
)
// APTIOV_SERVER_OVERRIDE_RC_START : Removed unused declaration files: Function not present in source
{
  EFI_STATUS                   Status = EFI_INVALID_PARAMETER;
/*  
  EFI_PHYSICAL_ADDRESS         AcmBase;
  ACM_HEADER_V30               *AcmHdr;
  UINT16                       AcmSvn;
  SPR_MSR_BIOS_SE_SVN_REGISTER MsrBiosSeSvnValue;
  SPR_MSR_BIOS_SE_SVN_REGISTER MsrBiosSeSvn;
  SPR_MSR_IA32_SE_SVN_REGISTER MsrIa32SeSvn;
  STATIC UINT32                TpmSgxSvnIndex = 0x1c10104;
  TPM2B_MAX_BUFFER             TpmNvSvnData;
  TPM2B_NV_PUBLIC              NvPublic;
  TPM2B_NAME                   NvName;
  TDX_DATA_HOB                 *pTdxDataHob = NULL;
  UINTN                        VarSize;

  SgxQueueDebug (SGX_D_VERBOSE, "ConfigureSgxSvn");
  MsrBiosSeSvnValue.Uint64 = 0x0;

  // BIOS guard hob
  if (mBgpdt != NULL) {
    MsrBiosSeSvnValue.Bits.PfatSeSvn = mBgpdt->BgModSvn;
  }

  // Txt Policy hob
  if (mPlatformTxtPolicyData == NULL) {
    return EFI_NOT_FOUND;
  }

  SgxQueueDebug ( SGX_D_VERBOSE, "  BiosAcmAddress - 0x%016x", (UINT64 ) mPlatformTxtPolicyData->BiosAcmAddress);
  SgxQueueDebug ( SGX_D_VERBOSE, "  StartUpAcmAddress - 0x%016x", (UINT64 ) mPlatformTxtPolicyData->StartupAcmAddress);
  if (0 == mPlatformTxtPolicyData->StartupAcmAddress) {
    return EFI_NOT_FOUND;
  }
  AcmBase = mPlatformTxtPolicyData->StartupAcmAddress;

  //
  //
  AcmHdr = (ACM_HEADER_V30 *) (UINTN) AcmBase;
  AcmSvn = AcmHdr->TxtSvn;
  MsrBiosSeSvn.Uint64 = AsmReadMsr64 (SPR_MSR_BIOS_SE_SVN);
  //
  //
  MsrIa32SeSvn.Uint64 = AsmReadMsr64 (SPR_MSR_IA32_SE_SVN);

  //
  // If TDX is enabled, programm BIOS_SE_SVN with the SVN of SEAMLDR ACM
  //
  if (mSecurityPolicy->EnableTdx) {
    VarSize = sizeof(pTdxDataHob->SeamLdrSvn);

    pTdxDataHob = GetTdxDataHob ();
    if (pTdxDataHob == NULL) {
      SGX_DEBUG ((SGX_D_ERROR, "TDX_DATA_HOB is not available.\n"));
      MsrBiosSeSvnValue.Bits.SeamldrSeSvn = (AcmHdr->SeSvn != 0 && AcmHdr->SeSvn != 0xFFFF) ? AcmHdr->SeSvn : 0;
    } else {
      Status = mPpiVariableServices->GetVariable (
              mPpiVariableServices,
              TDX_UEFI_SEAM_LDR_SVN_VAR_NAME,
              (EFI_GUID*)&gEfiSeamLoaderSvnVariableGuid,
              NULL,
              &VarSize,
              &pTdxDataHob->SeamLdrSvn
              );
      if (Status == EFI_SUCCESS) {
        if (AcmHdr->SeSvn != 0 && AcmHdr->SeSvn != 0xFFFF) {
          MsrBiosSeSvnValue.Bits.SeamldrSeSvn = (AcmHdr->SeSvn > pTdxDataHob->SeamLdrSvn) ? AcmHdr->SeSvn : pTdxDataHob->SeamLdrSvn;
        }
      } else if (Status == EFI_NOT_FOUND) {
        MsrBiosSeSvnValue.Bits.SeamldrSeSvn = (AcmHdr->SeSvn != 0 && AcmHdr->SeSvn != 0xFFFF) ? AcmHdr->SeSvn : 0;
        pTdxDataHob->SeamLdrSvn = (AcmHdr->SeSvn != 0 && AcmHdr->SeSvn != 0xFFFF) ? AcmHdr->SeSvn : 0xFFFF;
      } else {
        SGX_DEBUG ((SGX_D_ERROR, " Error while reading EFI variable. Exitting.\n"));
        return Status;
      }
    }
  }

  if (AcmSvn < MsrBiosSeSvn.Bits.ScleanSeSvn) {
    if (MsrIa32SeSvn.Bits.Lock) {
      //
      //
      SgxQueueDebug (SGX_D_VERBOSE, "LT Shutdown");
      ResetWarm();
      return Status;
    }
    //
    //
    MsrBiosSeSvnValue.Bits.ScleanSeSvn = (UINT8) AcmSvn;
    MsrBiosSeSvnValue.Bits.AncSeSvn = (UINT8) AcmSvn;
    AsmWriteMsr64 (SPR_MSR_BIOS_SE_SVN, MsrBiosSeSvnValue.Uint64);
  }

  if (0 != mPlatformTxtPolicyData->BiosAcmAddress) {
    SgxQueueDebug (SGX_D_VERBOSE, "SINIT present in flash");
    AcmBase = mPlatformTxtPolicyData->BiosAcmAddress;
    AcmHdr = (ACM_HEADER_V30 *) (UINTN) AcmBase;
    AcmSvn = AcmHdr->SeSvn;
    if (AcmSvn < MsrBiosSeSvn.Bits.SinitSeSvn) {
      if (MsrIa32SeSvn.Bits.Lock) {
        //
        //
        SgxQueueDebug (SGX_D_VERBOSE, "LT Shutdown");
        ResetWarm();
        return Status;
      }
      else {
        MsrBiosSeSvnValue.Bits.SinitSeSvn = AcmSvn;
      }
    }
    else {
      SgxQueueDebug (SGX_D_VERBOSE, "SINIT SVN not updated\n");
    }
  }
  else {
    ZeroMem (&NvPublic, sizeof(NvPublic));
    ZeroMem (&NvName, sizeof(NvName));
    TpmNvSvnData.size = 1;

    Status = Tpm2NvRead (
      NV_INDEX_FIRST,
      (TPMI_RH_NV_INDEX) TpmSgxSvnIndex,
      NULL,
      8,
      0,
      &TpmNvSvnData
    );

    if (EFI_SUCCESS == Status) {
      if (0 != TpmNvSvnData.buffer[0]) {
        SgxQueueDebug (SGX_D_VERBOSE, "SINIT SE SVN from TPM: %4x\n", TpmNvSvnData.buffer[0]);
        MsrBiosSeSvnValue.Bits.SinitSeSvn = TpmNvSvnData.buffer[0];
      }
    }
  }

  AsmWriteMsr64 (SPR_MSR_BIOS_SE_SVN, MsrBiosSeSvnValue.Uint64); */
  // APTIOV_SERVER_OVERRIDE_RC_END : Removed unused declaration files: Function not present in source
  Status = EFI_SUCCESS;
  return Status;
}


/**

  SGX early init flow at core scope

  @param  None

  @retval None

**/
VOID
EFIAPI
EnableSgxCoreScopeFirstTimeBoot (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       Processor;

  Status = mMpServices->WhoAmI (
    mPeiServices,
    mMpServices,
    &Processor
    );
  if (EFI_ERROR (Status)) {
    SgxQueueDebugM (SGX_D_VERBOSE, PEI_210_ERROR_DETECTING_THREAD, NULL);
    Processor = 0;
  }

  //
  // Exit if the thread is NOT package BSP (PBSP)
  //
  if (mProcInfo[Processor].Location.Thread != 0) {
    return;
  }

  EnablePoison ();
}


/**

  Read out SVN values at package scope

  @param  None

  @retval None

**/
VOID
EFIAPI
GetSvnPackageScope (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       Processor;
  UINTN       Package;

  Status = mMpServices->WhoAmI (
    mPeiServices,
    mMpServices,
    &Processor
    );
  if (EFI_ERROR (Status)) {
    SgxQueueDebugM (SGX_D_VERBOSE, PEI_210_ERROR_DETECTING_THREAD, NULL);
    return;
  }

  //
  // Exit if the thread is NOT package BSP (PBSP)
  //
  if ((mProcInfo[Processor].StatusFlag & PROCESSOR_AS_PBSP_BIT) == 0) {
    return;
  }

  //
  // convert from thread to package
  //
  Package = mProcInfo[Processor].Location.Package;
  SgxQueueDebug (SGX_D_VERBOSE, "<Socket[%3X]>", Package);
  ExtractPrResetSvn (Package);
}

/**

  Read out PRID values at package scope

  @param  None

  @retval None

**/
VOID
EFIAPI
GetPridPackageScope (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       Processor;
  UINTN       Package;

  Status = mMpServices->WhoAmI (
    mPeiServices,
    mMpServices,
    &Processor
    );
  if (EFI_ERROR (Status)) {
    SgxQueueDebugM (SGX_D_VERBOSE, PEI_210_ERROR_DETECTING_THREAD, NULL);
    return;
  }

  //
  // Exit if the thread is NOT package BSP (PBSP)
  //
  if ((mProcInfo[Processor].StatusFlag & PROCESSOR_AS_PBSP_BIT) == 0) {
    return;
  }

  //
  // convert from thread to package
  //
  Package = mProcInfo[Processor].Location.Package;
  SgxQueueDebug (SGX_D_VERBOSE, "<Socket[%3X]>", Package);

  ExtractPrids (Package);
}


/**

  Gets SVN from MSR registers and saves them in variables in Data Hob

  @param  None

  @retval EFI_STATUS

**/
EFI_STATUS
PopulateHobWithSvn (
  VOID
  )
{
  EFI_STATUS Status;

  SgxDebugQueueAllocInit ();
  GetSvnPackageScope ();
  Status = mMpServices->StartupAllAPs (
    mPeiServices,
    mMpServices,
    (EFI_AP_PROCEDURE) GetSvnPackageScope,
    FALSE,
    0,
    NULL
    );
  SgxDebugQueueConsume ();
  if (Status == EFI_NOT_STARTED) {
    SGX_DEBUG ((SGX_D_VERBOSE, "Populate Init Data Hob with SVN EFI_NOT_STARTED\n"));
    ASSERT(FALSE);
  }
  return Status;
}

/**

  Gets PRID from MSR registers and saves them in variables in Data Hob

  @param  None

  @retval EFI_STATUS

**/
EFI_STATUS
PopulateHobWithPrid (
  VOID
  )
{
  EFI_STATUS Status;
  UINT8      Index;

  SgxDebugQueueAllocInit ();
  GetPridPackageScope ();
  Status = mMpServices->StartupAllAPs (
    mPeiServices,
    mMpServices,
    (EFI_AP_PROCEDURE) GetPridPackageScope,
    FALSE,
    0,
    NULL
    );
  SgxDebugQueueConsume ();
  if (Status == EFI_NOT_STARTED) {
    SGX_DEBUG ((SGX_D_VERBOSE, "Populate Init Data Hob with PRID EFI_NOT_STARTED\n"));
    ASSERT(FALSE);
  }

  //Package PRID cannot be 0
  for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
    if (SocketPresent (Index) &&
        (mSgxInitDataHob.Socket[Index].Prids_0 == 0 && mSgxInitDataHob.Socket[Index].Prids_1 == 0)) {
      SGX_DEBUG ((SGX_D_VERBOSE, "Error Socket[%d] has PRID equal to zero\n", Index));
      ASSERT(FALSE);
      Status = EFI_UNSUPPORTED;
    }
  }
  return Status;
}

/**
  Configure PRMRR_PHYSBASE_0 on package BSP

    @param  None
      @retval None
**/
VOID
ConfigurePrmrrBase0 (
  VOID
  )
{
  //
  // program the PRMRR_PHYSBASE_0
  //
  AsmMsrOr64 (SPR_MSR_PRMRR_BASE_0, B_MSR_PRMRR_CONFIG_BIT);
}

/**
 Function execute multipackage programming PRMRR_PHYSBASE_0 and configure Owner Epoch on BSP package

 @param   None
 @retval  None
**/
VOID
ConfigurePrmrrBase0AndOwnerEpoch (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       Processor;
  UINTN       Package;

  Status = mMpServices->WhoAmI (
    mPeiServices,
    mMpServices,
    &Processor
    );
  if (EFI_ERROR (Status)) {
    SgxQueueDebugM (SGX_D_VERBOSE, PEI_210_ERROR_DETECTING_THREAD, NULL);
    Processor = 0;
  }

  //
  // Exit if the thread is NOT package BSP (PBSP)
  //
  if ((mProcInfo[Processor].StatusFlag & PROCESSOR_AS_PBSP_BIT) == 0) {
    return;
  }

  //
  // convert from thread to package
  //
  Package = mProcInfo[Processor].Location.Package;
  SgxQueueDebug (SGX_D_VERBOSE, "<Socket[%3X]>", Package);

  ConfigurePrmrrBase0();

  ConfigureOwnerEpoch();
}

/**
  Function enabling Debug mode on Package scope

  @param   None
  @retval  None
**/
VOID
EnableDebugModePackageScope (
  VOID
)
{
  EFI_STATUS  Status;
  UINTN       Processor;

  Status = mMpServices->WhoAmI (
    mPeiServices,
    mMpServices,
    &Processor
    );
  if (EFI_ERROR (Status)) {
    SgxQueueDebugM (SGX_D_VERBOSE, PEI_210_ERROR_DETECTING_THREAD, NULL);
    Processor = 0;
  }

  //
  // Exit if the thread is NOT package BSP (PBSP)
  //
  if ((mProcInfo[Processor].StatusFlag & PROCESSOR_AS_PBSP_BIT) == 0) {
    return;
  }

}

/**
  Function execute multipackage programming PRMRR_PHYSBASE_0 to enable PRID read and populate SgxInitDataHob with Prid

  @retval EFI_SUCCESS
          EFI_NOT_STARTED
**/
EFI_STATUS
UnlockReadPridPackageScope (
  VOID
  )
{
  EFI_STATUS      Status;

  SgxDebugQueueAllocInit ();

  EnableDebugModePackageScope();
  Status = mMpServices->StartupAllAPs (
    mPeiServices,
    mMpServices,
    (EFI_AP_PROCEDURE) EnableDebugModePackageScope,
    FALSE,
    0,
    NULL
    );
  SgxDebugQueueConsume ();
  if (Status == EFI_NOT_STARTED) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  APs not started, continue...\n"));
  } else if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error enabling SGX DEBUG mode on all packages (%r), continue init\n", Status));
  }

  SgxDebugQueueAllocInit ();
  //
  // Configure PHYBASE0 on all package BSP
  //
  ConfigurePrmrrBase0AndOwnerEpoch ();
  Status = mMpServices->StartupAllAPs (
    mPeiServices,
    mMpServices,
    (EFI_AP_PROCEDURE) ConfigurePrmrrBase0AndOwnerEpoch,
    FALSE,
    0,
    NULL
    );
  SgxDebugQueueConsume ();
  if (Status == EFI_NOT_STARTED) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  APs not started, continue...\n"));
  } else if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error enabling SGX on all packages (%r), continue init\n", Status));
  }

  Status = PopulateHobWithPrid ();
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "Populate Init Data Hob with PRID, return it's error!\n"));
    ASSERT_EFI_ERROR (Status);
  }
  return Status;
}

/**

  SGX early init flow at package scope

  @param  None

  @retval None

**/
VOID
EFIAPI
EnableSgxPackageScopeWarmReset (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       Processor;
  UINTN       Package;

  Status = mMpServices->WhoAmI (
    mPeiServices,
    mMpServices,
    &Processor
    );
  if (EFI_ERROR (Status)) {
    SgxQueueDebugM (SGX_D_VERBOSE, PEI_210_ERROR_DETECTING_THREAD, NULL);
    Processor = 0;
  }

  //
  // Exit if the thread is NOT package BSP (PBSP)
  //
  if ((mProcInfo[Processor].StatusFlag & PROCESSOR_AS_PBSP_BIT) == 0) {
    return;
  }

  //
  // convert from thread to package
  //
  Package = mProcInfo[Processor].Location.Package;
  SgxQueueDebug (SGX_D_VERBOSE, "<Socket[%3X]>", Package);

  ConfigureSgxSvn ();

  if (mSecurityPolicy->SgxQoS) {
    ConfigureSgxQoS ();
  }

  SgxQueueDebug (SGX_D_VERBOSE, "</Socket[%3X]>", Package);
}


/**
  SGX early init - First-time Boot flow

  @param None

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval EFI_NOT_READY     Any enabled APs are busy.
  @retval EFI_DEVICE_ERROR  Caller processor is AP.
**/
EFI_STATUS
SgxFirstTimeBootFlow (
  VOID
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  SGX_DEBUG ((SGX_D_VERBOSE, "SgxFirstTimeBootFlow START\n"));

  //
  // Thread scope SGX init flow
  //
  SgxDebugQueueAllocInit ();
  EnableSgxThreadScopeFirstTimeBoot ();
  Status = mMpServices->StartupAllAPs (
    mPeiServices,
    mMpServices,
    (EFI_AP_PROCEDURE) EnableSgxThreadScopeFirstTimeBoot,
    FALSE,
    0,
    NULL
    );
  SgxDebugQueueConsume ();
  if (Status == EFI_NOT_STARTED) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  APs not started, continue...\n"));
  } else if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error enabling SGX on all threads (%r), continue init\n", Status));
  }
  //
  // Core scope SGX init flow
  //
  SgxDebugQueueAllocInit ();
  EnableSgxCoreScopeFirstTimeBoot ();
  Status = mMpServices->StartupAllAPs (
    mPeiServices,
    mMpServices,
    (EFI_AP_PROCEDURE) EnableSgxCoreScopeFirstTimeBoot,
    FALSE,
    0,
    NULL
    );
  SgxDebugQueueConsume ();
  if (Status == EFI_NOT_STARTED) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  APs not started, continue...\n"));
    Status = EFI_SUCCESS;
  } else if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error enabling SGX on all packages, continue init\n"));
  }

  SGX_DEBUG ((SGX_D_VERBOSE, "SgxFirstTimeBootFlow END\n"));
  return Status;
}


/**
  SGX early init - warm reset flow

  @param None

  @retval EFI_SUCCESS             Operation completed successfully.
  @retval EFI_NOT_READY           Any enabled APs are busy.
  @retval EFI_DEVICE_ERROR        Caller processor is AP.
  @retval EFI_UNSUPPORTED         Configure SAI for SGX failed.
  @retval EFI_INVALID_PARAMETER   Failed to Reserve PRMRRs Memory in the Memory Address Map.
**/
EFI_STATUS
SgxWarmResetFlow (
#ifdef PROACTIVE_ACTION_ON_CODE_ORGANIZATION
  APP_SGX_3V0_PPI                  *AppApi,
  IN CONST EFI_PEI_SERVICES        **PeiServ
#else
  VOID
#endif // PROACTIVE_ACTION_ON_CODE_ORGANIZATION
  )
{
  EFI_STATUS              Status = EFI_UNSUPPORTED;
  VOID                    *Hob;
  EFI_HOB_GUID_TYPE       *GuidHob;

  SGX_DEBUG ((SGX_D_VERBOSE, "SgxWarmResetFlow START\n"));

  //
  // Get physical address bits supported.
  //
  mPhysicalAddressBits = GetLatestConsumePA();
  // function always return 46 bits instead 52
  Hob = GetFirstHob (EFI_HOB_TYPE_CPU);
  if (Hob == NULL) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error finding physical address bits of CPU, error exit\n"));
    return EFI_NOT_FOUND;
  }
  mPhysicalAddressBits = ((EFI_HOB_CPU *) Hob)->SizeOfMemorySpace;

  if (mSecurityPolicy->EnableMktme) {
    mPhysicalAddressBits = mPhysicalAddressBits - mSecurityPolicy->MkTmeKeyIdBits;
  }
  //
  // Core scope SGX init flow
  //
#ifdef PROACTIVE_ACTION_ON_CODE_ORGANIZATION
  // Calculate Valid PrmrrMask
  UINT64                        ValidPrmrrBitsMask;
  ValidPrmrrBitsMask = (LShiftU64 (1, mPhysicalAddressBits) - 1) & 0xfffffffffffff000;
  mPrmrrMask = ValidPrmrrBitsMask & (~((UINT64) (mSecurityPolicy->PrmrrSize - 1)));
  // Initialize API
  AppApi->PrmrrData.PrmrrSize  = mSecurityPolicy->PrmrrSize;
  AppApi->PrmrrData.PrmrrCount = mSgxInitDataHob.SgxPreMemInitHob.PrmrrCount;
  CopyMem (AppApi->PrmrrData.PrmrrBase, mSgxInitDataHob.SgxPreMemInitHob.PrmrrBase, sizeof(AppApi->PrmrrData.PrmrrBase));
  AppApi->PrmrrData.PrmrrMask  = mPrmrrMask;
  // ProgramPrmrr
  if ((mSgxInitDataHob.SgxBootScenario != SgxAddPackageBoot)) {
    Status = AppApi->ProgramPrmrr (AppApi, PeiServ);
  }
  SGX_DEBUG ((SGX_D_INFO, "  [APP_ADAPTER] EarlyInit AppApi status: (%r)\n", Status));
#endif // PROACTIVE_ACTION_ON_CODE_ORGANIZATION
  SgxDebugQueueAllocInit ();
  EnableSgxCoreScopeWarmReset ();
  Status = mMpServices->StartupAllAPs (
    mPeiServices,
    mMpServices,
    (EFI_AP_PROCEDURE) EnableSgxCoreScopeWarmReset,
    FALSE,
    0,
    NULL
    );
  SgxDebugQueueConsume ();
  if (Status == EFI_NOT_STARTED) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  APs not started, continue...\n"));
  } else if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error enabling SGX on all cores (%r), continue init\n", Status));
    return Status;
  }

  //
  // Uncore SGX init flow
  //
  if ((mSgxInitDataHob.SgxBootScenario != SgxAddPackageBoot)) {
    ConfigureUncoreM2mPrmrrs ();
  }
  //
  // BIOS Guard Data Hob
  //
  GuidHob = GetFirstGuidHob (&gBiosGuardHobGuid);
  if (GuidHob == NULL) {
    SGX_DEBUG ((SGX_D_VERBOSE, "BIOS Guard HOB not available\n"));
  } else {
    mBgpdt = (BGPDT *) GET_GUID_HOB_DATA (GuidHob);
  }

  //
  // Txt Policy Hob
  //
  GuidHob = GetFirstGuidHob (&gEfiPlatformTxtPolicyDataGuid);
  if (GuidHob == NULL) {
    SGX_DEBUG ((SGX_D_VERBOSE, "GUID HOB gEfiPlatformTxtPolicyDataGuid not found! Error when do TxtPei Init.\n"));
  } else {
    mPlatformTxtPolicyData = (EFI_PLATFORM_TXT_POLICY_DATA *) GET_GUID_HOB_DATA (GuidHob);
  }

  //
  // Package scope SGX init flow
  //
  SgxDebugQueueAllocInit ();
  EnableSgxPackageScopeWarmReset ();
  Status = mMpServices->StartupAllAPs (
    mPeiServices,
    mMpServices,
    (EFI_AP_PROCEDURE) EnableSgxPackageScopeWarmReset,
    FALSE,
    0,
    NULL
    );
  SgxDebugQueueConsume ();
  if (Status == EFI_NOT_STARTED) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  APs not started, continue...\n"));
  } else if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error enabling SGX on all packages (%r), continue init\n", Status));
  }

  //
  // SAI Policies change during SGX enabled boot
  //
  Status = ConfigureSaiForSgx ();
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error configuring SAI policies for SGX, continue init\n"));
    return Status;
  }

  SGX_DEBUG ((SGX_D_VERBOSE, "  Success exit\n"));
  SGX_DEBUG ((SGX_D_VERBOSE, "SgxWarmResetFlow END\n"));

  return EFI_SUCCESS;
}

/**
  Implements all steps needed by SGX disabled flow on Core scope

  @param  None

  @retval None

**/
VOID
SgxDisabledFlowCoreScope(
  VOID
  )
{
  EFI_STATUS                          Status     = EFI_SUCCESS;
  UINTN                               Thread     = 0;
  UINT8                               PrmrrIndex = 0;
  UINT64                              PrmrrMask  = 0;
  UINT32                              PrmrrRegister[MAX_PRMRR_COUNT] =
                                        { SPR_MSR_PRMRR_BASE_0, SPR_MSR_PRMRR_BASE_1_1, SPR_MSR_PRMRR_BASE_2_2, SPR_MSR_PRMRR_BASE_3_3,
                                          SPR_MSR_PRMRR_BASE_4_4, SPR_MSR_PRMRR_BASE_5_5, SPR_MSR_PRMRR_BASE_6_6, SPR_MSR_PRMRR_BASE_7_7 };
  MSR_IA32_FEATURE_CONTROL_REGISTER   FeatureControlMsr;
  MSR_MTRRCAP_REGISTER                MtrrCap;

  Status = mMpServices->WhoAmI (
    mPeiServices,
    mMpServices,
    &Thread
    );
  if (EFI_ERROR (Status)) {
    SgxQueueDebugM (SGX_D_VERBOSE, PEI_210_ERROR_DETECTING_THREAD, NULL);
    return;
  }

  //
  // Exit if the thread is not a primary thread
  //
  if (mProcInfo[Thread].Location.Thread != 0) {
    return;
  }

  // clear and lock IA32_FEATURE_CONTROL msr
  FeatureControlMsr.Uint64 = AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL);
  if (FeatureControlMsr.Bits.Lock == 0) {
    FeatureControlMsr.Bits.SgxLaunchControlEnable = 0;
    FeatureControlMsr.Bits.SgxEnable = 0;
    AsmWriteMsr64 (MSR_IA32_FEATURE_CONTROL, FeatureControlMsr.Uint64);
  }

  MtrrCap.Uint32 = (UINT32) AsmReadMsr64(MSR_IA32_MTRRCAP);
  if (MtrrCap.Bits.Prmrr == 1) {
    for (PrmrrIndex = 0; PrmrrIndex < MAX_PRMRR_COUNT; PrmrrIndex++) {
      //
      // clear the PRMRR_PHYSBASE_x
      //
      AsmWriteMsr64 (
        PrmrrRegister[PrmrrIndex],
        0
        );
    }

    //
    // clear and lock the PRMRR_PHYSMASK
    //
    PrmrrMask = AsmReadMsr64 (SPR_MSR_PRMRR_MASK);
    if ((PrmrrMask & B_MSR_PRMRR_PHYS_MASK_LOCK) == 0) {
      AsmWriteMsr64 (
        SPR_MSR_PRMRR_MASK,
        B_MSR_PRMRR_PHYS_MASK_LOCK
        );
    }
  }
}

/**
  SGX disabled flow as documented in spec (SGX global HAS)

  @param None

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_NOT_STARTED       APs not started.
  @retval EFI_DEVICE_ERROR      Caller processor is AP.
  @retval EFI_NOT_STARTED       No enabled AP exists in the system.
  @retval EFI_NOT_READY         Any enabled AP is busy.
**/
EFI_STATUS
SgxDisabledFlow (
  VOID
  )
{
  EFI_STATUS Status;

  SGX_DEBUG ((SGX_D_ERROR, "SgxDisabledFlow entry\n"));

  SgxDebugQueueAllocInit ();
  SgxDisabledFlowCoreScope ();
  Status = mMpServices->StartupAllAPs (
    mPeiServices,
    mMpServices,
    (EFI_AP_PROCEDURE) SgxDisabledFlowCoreScope,
    FALSE,
    0,
    NULL
    );
  SgxDebugQueueConsume ();
  if (Status == EFI_NOT_STARTED) {
    SGX_DEBUG ((SGX_D_ERROR, "  APs not started, continue...\n"));
  } else if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error disabling SGX on all cores (%r), continue init\n", Status));
  }
  SGX_DEBUG ((SGX_D_ERROR, "SgxDisabledFlow exit\n"));
  // set Status to diffrent than EFI_SUCCESS to indicate in next module SgxDisabledFlow
  Status = EFI_UNSUPPORTED;

  return Status;
}

/**
  PEIM entry point

  @param FileHandle         Pointer to the PEIM FFS file header.
  @param PeiServices        General purpose services available to every PEIM.

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval Otherwise         SGX initialization failed.

**/
EFI_STATUS
EFIAPI
SgxEarlyInitEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
#ifdef PROACTIVE_ACTION_ON_CODE_ORGANIZATION
  APP_SGX_3V0_PPI *AppApiSgx3v0 = NULL;
#endif // PROACTIVE_ACTION_ON_CODE_ORGANIZATION
  CPU_PRE_MEM_POLICY  *CpuPolicy      = NULL;
  SECURITY_POLICY     *SecurityPolicy = NULL;
  EFI_STATUS          Status          = EFI_UNSUPPORTED;
  SGX_ERROR_CODE_ENUM SgxErrorCode    = RS_SUCCESS;
  UINTN               Socket          = 0;
  SGX_DEBUG ((SGX_D_INFO, "[SGX] SgxEarlyInit entry\n"));

  InitializeSpinLock (&mPrintDebugLock);

  CpuPolicy = GetCpuPreMemPolicy ();
  if (CpuPolicy == NULL) {
    ASSERT (CpuPolicy != NULL);
    DEBUG ((SGX_D_ERROR, "Couldn't get cpu pre mem policy\n"));
    goto ErrorBeforeHob_SgxEarlyInit;
  }
  mCpuPolicy = CpuPolicy;

  SecurityPolicy = GetSecurityPolicy ();
  if (SecurityPolicy == NULL) {
    ASSERT (SecurityPolicy != NULL);
    DEBUG ((SGX_D_ERROR, "Couldn't get security policy\n"));
    goto ErrorBeforeHob_SgxEarlyInit;
  }
  mSecurityPolicy = SecurityPolicy;

#ifdef PROACTIVE_ACTION_ON_CODE_ORGANIZATION
  //
  // Locate SgxAppApi
  //
  Status = (*PeiServices)->LocatePpi (
    PeiServices,
    &gSecurityAppSgx3v0PpiGuid,
    0,
    NULL,
    &AppApiSgx3v0
    );
#endif // PROACTIVE_ACTION_ON_CODE_ORGANIZATION

  //
  // Initialize module globals / PPIs
  //
  Status = PopulateInitDataHobFromPreMem ();
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "Error when populating InitDataHob from PreMem\n"));
    goto ErrorBeforeHob_SgxEarlyInit;
  }

  mPeiServices = (EFI_PEI_SERVICES **) PeiServices;
  mHost = (PSYSHOST) (UINTN) PcdGet64 (PcdSyshostMemoryAddress);
  if (mHost == NULL) {
    SGX_DEBUG ((SGX_D_VERBOSE, "Error: SgxSiliconInitEntryPoint: SYSHOST not found.\n"));
    Status = EFI_INVALID_PARAMETER;
    SgxErrorCode = RS_POSTMEM_SYSHOST_NOTFOUND;
    goto Error_SgxEarlyInit;
  }

  mMemMapHost = GetMemMapHostPointer ();
  if (mMemMapHost == NULL) {
    SGX_DEBUG ((SGX_D_VERBOSE, "Error: SgxSiliconInitEntryPoint: MEMORY_MAP_HOST not found.\n"));
    Status = EFI_INVALID_PARAMETER;
    SgxErrorCode = RS_POSTMEM_MMAP_HOST_NOTFOUND;
    goto Error_SgxEarlyInit;
  }

  // Get the Variable Services PPI
  Status = PeiServicesLocatePpi (
    &gEfiPeiReadOnlyVariable2PpiGuid,
    0,
    NULL,
    (VOID **) &mPpiVariableServices
    );
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "Error: Unable to locate Variable Services PPI.\n"));
    ASSERT_EFI_ERROR (Status);
    SgxErrorCode = RS_POSTMEM_VSPPI_NOTFOUND;
    goto Error_SgxEarlyInit;
  }

  // Get the MRC Hooks Chip Services PPI
  Status = (*mPeiServices)->LocatePpi (
    PeiServices,
    &gMrcHooksChipServicesPpiGuid,
    0,
    NULL,
    &mMrcHooksChipServices
    );
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "Find MRC Hooks Chip Services Ppi failed, return it's error!\n"));
    ASSERT_EFI_ERROR (Status);
    SgxErrorCode = RS_POSTMEM_MRCHCSPPI_NOTFOUND;
    goto Error_SgxEarlyInit;
  }

  // Detect MP Services data like system BSP, Socket BSPs.
  Status = SgxMpServicesData ();
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_ERROR, "Error in SgxMpServicesData\n"));
    SgxErrorCode = RS_POSTMEM_OTHER;
    goto Error_SgxEarlyInit;
  }

  // Restore UEFI FW Registration State from NVRAM
  Status = RestoreUefiFwRegistrationStateVariable ();
  if (Status == EFI_NOT_FOUND) {
    SGX_DEBUG ((SGX_D_VERBOSE, "UefiFwRegistrationState not found in NVRAM!\n"));
  } else if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    goto Error_SgxEarlyInit;
  }

  if (mSecurityPolicy->IsSgxCapable == FALSE) {
    SGX_DEBUG ((SGX_D_ERROR, "  System is not SGX capable\n"));
    Status = EFI_UNSUPPORTED;
    goto SgxDisabled_SgxEarlyInit;
  }

  mSgxInitDataHob.MktmeIntegrityEnabled = MktmeIntegrityEnabled ();
  SGX_DEBUG ((SGX_D_ERROR, "[SGX-DEBUG] mSgxInitDataHob.MktmeIntegrityEnabled  = (%a)\n",
         (mSgxInitDataHob.MktmeIntegrityEnabled == TRUE ? "TRUE" : "FALSE")));
  if (mSecurityPolicy->EnableSgx == SECURITY_POLICY_ENABLE) {
    Status = UnlockReadPridPackageScope ();
    if (EFI_ERROR (Status)) {
      SGX_DEBUG ((SGX_D_VERBOSE, "[SGX] Error: Unlock read PRID, return it's error!\n"));
      SgxErrorCode = RS_POSTMEM_PRID_UNLOCK_ERR;
      goto Error_SgxEarlyInit;
    }
  }

  // Get Registration Variables from NVRAM
  Status = GetRegistrationVariablesFromNvram ();
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "Get Registration Variables from NVRAM failed, return it's error!\n"));
    ASSERT_EFI_ERROR(Status);
    SgxErrorCode = RS_POSTMEM_REGVARS_ERR;
    goto Error_SgxEarlyInit;
  }

#if SGX_DEBUG_RESTORE_KEY_BLOBS_FROM_REGISTRATION_PACKAGE_INFO
  for (Socket = 0; Socket < MCHECK_MAX_PACKAGES; ++Socket) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  [SGX-KEYBLOB-PEI] KeyBlob for socket no %d\n", Socket));
    PrintUefiFwKeyBlob(&mSgxInitDataHob.UefiFwKeyBlobs[Socket]);
  }
  for (ClearSocket = 0; ClearSocket < MCHECK_MAX_PACKAGES; ++ClearSocket) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  [SGX-UEFIFW-KEYBLOB-PEI] KeyBlob for socket no %d - before delete\n", ClearSocket));
    PrintUefiFwKeyBlob((VOID*) &(ClearSgxUefiFwKeyBlobsVariablePtr[ClearSocket]));
  }
  for (ClearSocket = 0; ClearSocket < MCHECK_MAX_PACKAGES; ++ClearSocket) {
    if (ClearSgxUefiFwKeyBlobsVariablePtr[ClearSocket].Present) {
      ZeroMem((VOID *) &ClearSgxUefiFwKeyBlobsVariablePtr, ClearSgxUefiFwKeyBlobsVariableSize);
      break;
    }
  }
  for (ClearSocket = 0; ClearSocket < MCHECK_MAX_PACKAGES; ++ClearSocket) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  [SGX-UEFIFW-KEYBLOB-PEI] KeyBlob for socket no %d - after delete\n", ClearSocket));
    PrintUefiFwKeyBlob((VOID *) &(ClearSgxUefiFwKeyBlobsVariablePtr[ClearSocket]));
  }
#endif

  //
  // Restore KeyBlobs from NVRAM
  //
  if (mRegState->ForceFirstBindingOnRegConfChanged) {
    // If RegistrationConfig was changed, force FirstBinding BootScenario
    // (SgxPartialResetPending)
    SGX_DEBUG ((SGX_D_VERBOSE, "  SgxPartialResetPending = TRUE\n"));
    mSgxInitDataHob.KeyBlobsExistInNvram = FALSE;
  } else {
    // If FirstBinding scenario was NOT forced, continue boot with KeyBlob restoration
    Status = RestoreKeyBlobsInternalSgxInitDataHobFieldsFromNvram (
               &mSgxInitDataHob,
               mSecurityPolicy->SgxPackageInfoInBandAccess,
               mSgxInitDataHob.MktmeIntegrityEnabled
               );
    if ((Status == EFI_NOT_FOUND) || (Status == EFI_UNSUPPORTED)) {
      SGX_DEBUG ((SGX_D_VERBOSE, "  KeyBlobs were not found in NVRAM. Continue boot...\n"));
    } else if (EFI_ERROR (Status)) {
      SGX_DEBUG ((SGX_D_VERBOSE, "  Failed to restore KeyBlobs from NVRAM!\n"));
      SgxErrorCode = RS_POSTMEM_KEYBLOBS_RES_ERR;
      goto Error_SgxEarlyInit;
    }
  }

  // Print KeyBlobs
  for (Socket = 0; Socket < MCHECK_MAX_PACKAGES; ++Socket) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  [SGX-KEYBLOB-PEI-REST] KeyBlob for socket no %d\n", Socket));
    PrintUefiFwKeyBlob(&mSgxInitDataHob.UefiFwKeyBlobs[Socket]);
  }

  if (mSecurityPolicy->EnableSgx == SECURITY_POLICY_DISABLE) {
    SGX_DEBUG ((SGX_D_ERROR, "  SGX disabled, exiting\n"));
    Status = EFI_SUCCESS;
    goto SgxDisabled_SgxEarlyInit;
  } else if (mSgxInitDataHob.SgxPreMemInitHob.ErrorCode == RS_PREMEM_TME_DISABLED) {
    SgxErrorCode = mSgxInitDataHob.SgxPreMemInitHob.ErrorCode;
    Status = EFI_UNSUPPORTED;
    goto SgxDisabled_SgxEarlyInit;
  } else if (mSgxInitDataHob.SgxPreMemInitHob.ErrorCode != RS_SUCCESS) {
    SgxErrorCode = mSgxInitDataHob.SgxPreMemInitHob.ErrorCode;
    SGX_DEBUG ((SGX_D_ERROR, "  Failed previous SGX module, exiting\n"));
    Status = EFI_UNSUPPORTED;
    goto SgxDisabled_SgxEarlyInit;
  }

  if (SecurityPolicy->EnableSgx == SECURITY_POLICY_ENABLE) {
    Status = PopulateHobWithSvn ();
    if (EFI_ERROR (Status)) {
      SGX_DEBUG ((SGX_D_VERBOSE, "Populate Init Data Hob with SVN, return it's error!\n"));
      ASSERT_EFI_ERROR (Status);
      SgxErrorCode = RS_POSTMEM_SVN_ERR;
      goto Error_SgxEarlyInit;
    }

    //
    // Perform the actual initialization
    //
    if (MsrTraceEnabled ()) {
      SGX_DEBUG ((SGX_D_ERROR, "SGX Early MSR Trace Start\n"));
      SetMsrTraceOutputMode (MSR_OUTPUT_ENABLE);
      UbiosCall("program_mtrr_sgx", SCOPE_THREAD);
      UbiosCall("relocate_dd_regions", SCOPE_BSP);
    }

    // Determine boot and registration flow
    Status = SgxDetermineBootAndRegistrationScenario (
      mSgxInitDataHob.KeyBlobsExistInNvram,
      mRegState,
      mSecurityPolicy->SgxFactoryReset,
      &mSgxInitDataHob.SgxFirstBoot,
      &mSgxInitDataHob.SgxBootScenario,
      &mRegState->NonVolatile.SwAgentFirstBindingComplete
      );
    if (EFI_ERROR (Status)) {
      SgxErrorCode = RS_POSTMEM_DETERMINE_BOOT_ERR;
      goto Error_SgxEarlyInit_TraceEnd;
    }

    // Perform First-time Boot Flow
    if (mSgxInitDataHob.SgxFirstBoot) {
      Status = SgxFirstTimeBootFlow ();
      if (EFI_ERROR (Status)) {
        SGX_DEBUG ((SGX_D_VERBOSE, "  Error: FirstTimeBootFlow.\n"));
        SgxErrorCode = RS_POSTMEM_FIRSTBOOT_ERR;
        goto Error_SgxEarlyInit_TraceEnd;
      }
    }

    // Perform Warm Reset Flow
    if (!EFI_ERROR (Status)) {
      Status = SgxWarmResetFlow (
#ifdef PROACTIVE_ACTION_ON_CODE_ORGANIZATION
        AppApiSgx3v0,
        PeiServices
#endif // PROACTIVE_ACTION_ON_CODE_ORGANIZATION
        );
      if (EFI_ERROR (Status)) {
        SGX_DEBUG ((SGX_D_VERBOSE, "  Error: WarmResetFlow.\n"));
        SgxErrorCode = RS_POSTMEM_WARMRESET_ERR;
        goto Error_SgxEarlyInit_TraceEnd;
      }
    }

    if (MsrTraceEnabled ()) {
      SGX_DEBUG ((SGX_D_ERROR, "SGX Early MSR Trace End\n"));
      SetMsrTraceOutputMode (MSR_OUTPUT_DISABLE);
    }
  } else {
    SGX_DEBUG ((SGX_D_VERBOSE, "SGX Early Init: SGX Disabled flow.\n"));
  }

  PreserveInitDataHob();
  SGX_DEBUG ((SGX_D_INFO, "[SGX] SgxEarlyInit exit: %r\n", Status));
  return Status;

Error_SgxEarlyInit_TraceEnd:
  if (MsrTraceEnabled ()) {
    SGX_DEBUG ((SGX_D_ERROR, "SGX Early MSR Trace End\n"));
    SetMsrTraceOutputMode (MSR_OUTPUT_DISABLE);
  }

SgxDisabled_SgxEarlyInit:
  mSgxInitDataHob.SgxBootScenario = SgxDisabled;
Error_SgxEarlyInit:
  SGX_DEBUG ((SGX_D_ERROR, "[SGX] SgxEarlyInit exit: %r\n", Status));
  if (SgxErrorCode != RS_SUCCESS) {
    mSgxInitDataHob.SgxPreMemInitHob.ErrorCode = SgxErrorCode;
  }

  PreserveInitDataHob();
ErrorBeforeHob_SgxEarlyInit:
  SgxDisabledFlow ();
  ClearGlobalVariables ();
  return Status;
}
