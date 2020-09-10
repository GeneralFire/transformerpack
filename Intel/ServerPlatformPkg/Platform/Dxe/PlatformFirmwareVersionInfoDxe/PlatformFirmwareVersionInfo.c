/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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
#include <PiDxe.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Library/DxeServicesLib.h>
// APTIOV_SERVER_OVERRIDE_RC_START : Removing IPMI feature code
//#include <Protocol/IpmiTransportProtocol.h>
//#include <Library/IpmiBaseLib.h>
// APTIOV_SERVER_OVERRIDE_RC_START : Removing IPMI feature code
#include "EfiServerManagement.h"
#include <Protocol/RedirFru.h>
#include "Protocol/GenericElog.h"
#include <Protocol/Variable.h>
#include <Protocol/ServerBmcVariable.h>
#include <Library/MemoryAllocationLib.h>
#include <CpuDataStruct.h>
#include "PlatformFirmwareVersionInfo.h"
#include <Library/IfwiIdLib.h>
#include <Register/Microcode.h>
#include "FirmwareVersionInfo.h"
#pragma optimize( "", off)

#include <MkhiMsgs.h>
#include <Library/DxeMeLib.h>

#include <Protocol/IioUds.h>

// APTIOV_SERVER_OVERRIDE_RC_START : Removing IPMI feature code
#if 0
EFI_IPMI_TRANSPORT     *mIpmiTransport;
EFI_MISC_IFWI_Rom_DATA mIFWIRomData[RomNumMax] = {
  {{0xBAC1001B, 0xECCC, 0x40c6, {0x99, 0x0D, 0xE8, 0xC1, 0x9A, 0x8E, 0x47, 0x7A}}, "SINIT", EFI_SECTION_RAW, 0, 0},
  {{0x15E1E31A, 0x9F9D, 0x4C84, {0x82, 0xFB, 0x1A, 0x70, 0x7F, 0xC0, 0xF6, 0x3B}}, "RSTeSataEFI", EFI_SECTION_PE32, 0, 1},
  {{0xB44B2005, 0x42BC, 0x41C9, {0x80, 0xAF, 0xAB, 0xD7, 0xDC, 0x7D, 0x69, 0x23}}, "RSTesSataEFI", EFI_SECTION_PE32, 0, 1},
  {{0x6641993a, 0x2cf3, 0x4cac, {0x85, 0x06, 0x59, 0x9c, 0xb0, 0x27, 0x6a, 0xbd}}, "RSTSataEfi", EFI_SECTION_PE32, 0, 1},
  {{0xA1f436EA, 0xA127, 0x4EF8, {0x95, 0x7C, 0x80, 0x48, 0x60, 0x6F, 0xF6, 0x70}}, "1GEFI", EFI_SECTION_PE32, 0, 1},
  {{0x56A1B86F, 0x0D4A, 0x485d, {0x87, 0xDE, 0xAD, 0x0E, 0xBA, 0x1C, 0x8C, 0x2A}}, "iBMCGOP", EFI_SECTION_PE32, 0, 1},
  {{0x02A6DE33, 0x3EA9, 0x4c17, {0x8E, 0xA2, 0x56, 0x81, 0xCC, 0x7A, 0xFD, 0xED}}, "VMDDxeEfi", EFI_SECTION_PE32, 0, 1},
  {{0xEFE92A04, 0xF5D0, 0x4e44, {0x87, 0x57, 0x25, 0xB3, 0xAF, 0xA3, 0xBF, 0xFF}}, "RSTeSataRaidEfi", EFI_SECTION_PE32, 0, 1 },
  {{0x9CE4325E, 0x003E, 0x11E3, {0xB5, 0x82, 0xB8, 0xAC, 0x6F, 0x19, 0x9A, 0x57}}, "NvmDimmDriver", EFI_SECTION_PE32, 0, 1},
  {{0xEA9DE6D5, 0x7839, 0x46F7, {0x9E, 0x63, 0x4D, 0xE8, 0xB0, 0x0E, 0x2E, 0x5D}}, "NvmDimmHii",  EFI_SECTION_PE32, 0, 1},
};

GLOBAL_REMOVE_IF_UNREFERENCED FVI_DATA mPlatformFviData[] = {
  FVI_VERSION (ME_FW_VERSION),
  FVI_VERSION (BIOS_ACM_VERSION),
  FVI_VERSION (BMC_FW_VERSION),
  FVI_VERSION (FRU_SDR_VERSION),
  FVI_VERSION (FRU_VERSION),
  FVI_VERSION (SDR_VERSION),
  FVI_VERSION (CPU_UCODE1_VERSION),
  FVI_VERSION (CPU_UCODE2_VERSION),
  FVI_VERSION (CPU_UCODE3_VERSION),
  FVI_VERSION (SINIT_ACM_VERSION),
  FVI_VERSION (MRC_VERSION),
  FVI_VERSION (QPIRC_VERSION),
  FVI_VERSION (PSU_FW_VERSION),
  FVI_VERSION (RSTeSataEFI_VERSION),
  FVI_VERSION (RSTesSataEFI_VERSION),
  FVI_VERSION (OneGEFI_VERSION),
  FVI_VERSION (iBMCGOP_VERSION),
  FVI_VERSION (VMDDxeEfi_VERSION),
  FVI_VERSION (RSTeSataRaidEfi_VERSION),
  FVI_VERSION (NvmDimmDriver_VERSION),
  FVI_VERSION (NvmDimmHii_VERSION),
};

GLOBAL_REMOVE_IF_UNREFERENCED FVI_STRINGS mPlatformFviStrings[] = {
  { ME_FW_STRING,                NULL, },
  { BIOS_ACM_STRING,             TO_BE_FILLED_STRING, },
  { BMC_FW_STRING,               TO_BE_FILLED_STRING, },
  { FRU_SDR_STRING,              TO_BE_FILLED_STRING, },
  { FRU_STRING,                  TO_BE_FILLED_STRING, },
  { SDR_STRING,                  TO_BE_FILLED_STRING, },
  { CPU_UCODE1_STRING,           TO_BE_FILLED_STRING, },
  { CPU_UCODE2_STRING,           TO_BE_FILLED_STRING, },
  { CPU_UCODE3_STRING,           TO_BE_FILLED_STRING, },
  { SINIT_ACM_STRING,            TO_BE_FILLED_STRING, },
  { MRC_STRING,                  NULL, },
  { QPIRC_STRING,                NULL, },
  { PSU_FW_STRING,               TO_BE_FILLED_STRING, },
  { RSTeSataEFI_STRING,          TO_BE_FILLED_STRING, },
  { RSTesSataEFI_STRING,         TO_BE_FILLED_STRING, },
  { OneGEFI_STRING,              TO_BE_FILLED_STRING, },
  { iBMCGOP_STRING,              TO_BE_FILLED_STRING, },
  { VMDDxeEfi_STRING,            TO_BE_FILLED_STRING, },
  { RSTeSataRaidEfi_STRING,      TO_BE_FILLED_STRING, },
  { NvmDimmDriver_STRING,        TO_BE_FILLED_STRING, },
  { NvmDimmHii_STRING,           TO_BE_FILLED_STRING, },
};

GLOBAL_REMOVE_IF_UNREFERENCED FVI_HEADER mPlatformFviHeader = DEFAULT_FVI_HEADER_DATA(mPlatformFviData);

/**
  This function calculate the file's checksum.

  @param[in]  filebuf        A piece of file buffer pointer.
  @param[in]  filesize       the size of file.

  @retval checksum
**/
UINT32 GetCheckSum(
  IN UINT8 * filebuf,
  IN UINTN filesize
)
{
  UINT32    Sum, Count, Quotient, Remainder, i;
  UINT8     rest[4];
  UINT32    *Buf;
  UINT32    result;

  Buf = (UINT32*)filebuf;

  Quotient = (UINT32)(filesize / 4);
  Remainder = (UINT32)(filesize - Quotient * 4);

  for(i = 0; i < 4; i++)
    rest[i] = 0;

  for(i = 0; i < Remainder; i++)
    rest[i] = filebuf[Quotient * 4 + i];

  for (Sum = 0, Count = 0; Count < Quotient; Count++) {
    Sum += *(Buf + Count);

  }

  Sum += *((UINT32 *)rest);
  result = (UINT32) ((UINT32)(-1) - Sum + 1);
  return result;
}

/**
  Add an SMBIOS record.

  @param  SmBios                The EFI_SMBIOS_PROTOCOL instance.
  @param  SmBiosHandle        A unique handle will be assigned to the SMBIOS record.
  @param  Record                The data for the fixed portion of the SMBIOS record. The format of the record is
                                determined by EFI_SMBIOS_TABLE_HEADER.Type. The size of the formatted area is defined
                                by EFI_SMBIOS_TABLE_HEADER.Length and either followed by a double-null (0x0000) or
                                a set of null terminated strings and a null.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added due to lack of system resources.

**/
EFI_STATUS
AddSmbiosRecord (
  IN EFI_SMBIOS_PROTOCOL        * SmBios,
  OUT EFI_SMBIOS_HANDLE         * SmBiosHandle,
  IN EFI_SMBIOS_TABLE_HEADER    *Record
)
{
  *SmBiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  return SmBios->Add (
           SmBios,
           NULL,
           SmBiosHandle,
           Record
         );
}


/**
     This function get the microcode version.

    @param  pIfwiCfgVersionData     A piece of data buffer pointer.

    @return None.
**/
VOID GetMicrocodeVersions(
  EFI_MISC_IFWI_CFG_VERSION *pIfwiCfgVersionData
)
{
  CPU_MICROCODE_HEADER     *pMicrocodeHead;
  UINT8                    i;
  UINTN                    CpuMicrocodeSize;
  pMicrocodeHead = (CPU_MICROCODE_HEADER*)PcdGet64(PcdCpuMicrocodePatchAddress);
  CpuMicrocodeSize = (UINTN)PcdGet64(PcdCpuMicrocodePatchRegionSize);

  for (i = 0; i < MicrocodeNumMax; i++) {
    if((pMicrocodeHead != NULL) && ((UINT8 *)pMicrocodeHead < (UINT8 *)(pMicrocodeHead + CpuMicrocodeSize))) {
      if((pMicrocodeHead->ProcessorFlags != 0) &&
         (pMicrocodeHead->ProcessorFlags != 0xFFFFFFFF) &&
         (pMicrocodeHead->ProcessorSignature.Uint32 != 0) &&
         (pMicrocodeHead->ProcessorSignature.Uint32 != 0xFFFFFFFF)) {
        AsciiSPrint (pIfwiCfgVersionData->CPUPatchVersion[i], MAX_StringLength, "M%x%x_%08x",
                     pMicrocodeHead->ProcessorFlags,
                     pMicrocodeHead->ProcessorSignature.Uint32,
                     pMicrocodeHead->UpdateRevision);
      }
      pMicrocodeHead = (CPU_MICROCODE_HEADER *)((UINTN)pMicrocodeHead + (UINTN)pMicrocodeHead->TotalSize);
      DEBUG ((EFI_D_INFO, "["DFT_ID"] pMicrocodeHead: %x\n", (UINTN)pMicrocodeHead));
    }
  }
}


/**
     This function calculate the BIOS ACM module checksum.

    @param  pIfwiCfgVersionData     A piece of data buffer pointer.

    @return None.
**/

VOID GetBIOSACMCheckSum(
  EFI_MISC_IFWI_CFG_VERSION *pIfwiCfgVersionData
)
{
  UINT8                    *pBuf = NULL;
  UINTN                    BIOSAcmSize = 0;
  UINT32                   BIOSAcmCheckSum = 0;
  UINT8                    TempStr[MAX_StringLength] = "";

  pBuf = (UINT8*)(UINTN)PcdGet32(PcdFlashFvAcmBase);
  BIOSAcmSize = (UINTN)PcdGet32(PcdFlashFvAcmSize);
  BIOSAcmCheckSum = GetCheckSum(pBuf, BIOSAcmSize);
  DEBUG ((EFI_D_INFO, "["DFT_ID"] BIOSAcmCheckSum: %x\n", BIOSAcmCheckSum));
  AsciiSPrint (TempStr, MAX_StringLength, "%08X", BIOSAcmCheckSum);
  AsciiStrCpyS(pIfwiCfgVersionData->BIOSACMVersion, MAX_StringLength, TempStr);
}


UINT8 *
GetFruInfoArea (
  IN  EFI_SM_FRU_REDIR_PROTOCOL * RedirFru,
  IN  UINTN                     Offset
)
/*++

Routine Description:
  This routine gets the FRU info area specified by the offset and returns it in
  an allocated buffer.  It is the caller's responsibility to free the buffer.

Arguments:
  RedirFru  - SM Fru Redir protocol
  Offset    - Info Area starting offset in multiples of 8 bytes.

Returns:
  Buffer with FruInfo data or NULL if not found.

--*/
{
  EFI_STATUS  Status;
  UINT8       *TempPtr;
  UINTN       Length;

  TempPtr = NULL;

  Offset  = Offset * 8;
  if (Offset > 0) {
    //
    // Get Info area length, which is in multiples of 8 bytes
    //
    Length = 0;
    Status = RedirFru->GetFruRedirData (RedirFru, 0, (Offset + 1), 1, (UINT8 *) &Length);
    if (Status == EFI_NOT_FOUND) {
      return NULL;
    }

    Length = Length * 8;

    if (Length > 0) {
      TempPtr = AllocateRuntimePool (Length);
      if (TempPtr == NULL) {
        return NULL;
      }

      Status = RedirFru->GetFruRedirData (RedirFru, 0, Offset, Length, TempPtr);
      if (Status == EFI_NOT_FOUND) {
        return NULL;
      }
    }
  }

  return TempPtr;
}

VOID
GetStringFromFru (
  IN OUT  UINTN           * Offset,
  IN      UINT8           * TempPtr,
  IN OUT  UINT8           *StrPtr
)
/*++

Routine Description:
  This routine attempts to get a string out of the FRU at the designated offset in the
  buffer pointed to by TempPtr.  String type is ASCII

Arguments:
  Offset    - Offset of string in buffer pointed to by TempPtr, this is updated to the next
              offset.
  TempPtr   - Pointer to a buffer containing the FRU
  StrPtr    - the pointer to a buffer for retrive the string get from FRU.

Returns:
  STRING_REF with reference to string retrieved from the FRU or NULL if none is found.

--*/
{
  UINTN       Length;
  UINT8       *SrcStrPtr;

  Length     = 0x3F & TempPtr[*Offset];
  SrcStrPtr  = &TempPtr[*Offset + 1];

  ASSERT(Length < FRUMAXSTRING);

  if (Length > 0) {
    CopyMem(StrPtr, SrcStrPtr, Length);
    StrPtr[Length] = '\0';
  } else {
    StrPtr[0] = '\0';
  }
  *Offset = *Offset + Length + 1;
  return;
}


/**
    GetFRUVersion:
        This function gets the FRU version.

    @param  pIfwiCfgVersionData     A piece of data buffer pointer.

    @retval EFI_SUCCESS                     success to get FRU version.
**/
EFI_STATUS GetFRUVersion(EFI_MISC_IFWI_CFG_VERSION * pIfwiCfgVersionData)
{
  EFI_SM_FRU_REDIR_PROTOCOL *RedirFru;
  IPMI_FRU_COMMON_HEADER    FruCommonHeader;
  UINT8                     *FruHdrPtr;
  UINTN                     Offset;
  UINT8                     *TempPtr;
  EFI_STATUS                Status;
  UINTN                     i;
  UINT8                     FruHdrChksum;
  UINT16                    NullStringRef;
  UINT8                     TempStr[MAX_StringLength];

  Offset        = 0;
  NullStringRef = 0;
  TempPtr       = NULL;
  SetMem((UINT8*)&FruCommonHeader, sizeof(IPMI_FRU_COMMON_HEADER), 0);

  Status        = gBS->LocateProtocol (&gEfiRedirFruProtocolGuid, NULL, &RedirFru);
  DEBUG ((EFI_D_INFO, "["DFT_ID"] Locate gEfiRedirFruProtocolGuid: %r\n", Status));
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = RedirFru->GetFruRedirData (RedirFru, 0, 0, sizeof (IPMI_FRU_COMMON_HEADER), (UINT8 *) &FruCommonHeader);
  DEBUG ((EFI_D_INFO, "["DFT_ID"] GetFruRedirData for FruCommonHeader: %r\n", Status));
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Do a validity check on the FRU header, since it may be all 0xFF(s) in
  // case there of no FRU programmed on the system.
  //
  FruHdrPtr = (UINT8 *) &FruCommonHeader;
  for (i = 0, FruHdrChksum = 0; i < sizeof (FruCommonHeader); i++) {
    FruHdrChksum = (UINT8) (FruHdrChksum + *FruHdrPtr++);
  }

  if (FruHdrChksum != 0) {
    DEBUG ((EFI_D_INFO, "["DFT_ID"] FRU header invalid.\n"));
    Status = EFI_NOT_READY;
    //
    //  The FRU information is bad so we need to create dummy records and return.
    //
    return Status;
  }

  //
  // Get Base Board data, GetStringFromFru updates Offset to the next Field in the FRU.  The fields must
  // be retrieved in order.
  //
  TempPtr = NULL;
  TempPtr = GetFruInfoArea (RedirFru, FruCommonHeader.BoardAreaStartingOffset);
  if (TempPtr != NULL) {
    Offset  = BOARD_MFG_OFFSET;
    GetStringFromFru (&Offset, TempPtr, NULL);                          // Board Manufacturer Name
    GetStringFromFru (&Offset, TempPtr, NULL);                          // Board Product Name
    GetStringFromFru (&Offset, TempPtr, NULL);                          // Board Serial Number
    GetStringFromFru (&Offset, TempPtr, NULL);                          // Board Part Number
    GetStringFromFru (&Offset, TempPtr, TempStr);                       // FRU File Version ID string
    DEBUG ((EFI_D_INFO, "["DFT_ID"] FRU Ver: %a\n", TempStr));
    AsciiStrCpyS(pIfwiCfgVersionData->FRUVersion, MAX_StringLength, TempStr);
    Status = EFI_SUCCESS;
    FreePool (TempPtr);
  }
  return Status;
}

/**
    GetSDRVersion:
        This function gets the SDR version.

    @param  pIfwiCfgVersionData     A piece of data buffer pointer.

    @retval None
**/
VOID GetSDRVersions(EFI_MISC_IFWI_CFG_VERSION * pIfwiCfgVersionData)
{
  EFI_STATUS                    Status;
  EFI_IPMI_SDR_REPOSITORY_INFO  RepositoryInfo;
  EFI_IPMI_SENSOR_RECORD_STRUCT SdrRecord;
  EFI_IPMI_GET_SDR              GetSdr;
  UINT32                        DataSize;
  UINTN                         Index;

  Status = gBS->LocateProtocol (
             &gEfiIpmiTransportProtocolGuid,
             NULL,
             &mIpmiTransport
           );
  DEBUG ((EFI_D_INFO, "["DFT_ID"] Locate gEfiIpmiTransportProtocolGuid: %r\n", Status));
  if ((Status == EFI_SUCCESS) && (mIpmiTransport != NULL)) {

    //
    // Issue Get SDR Respository Info command
    //
    DataSize = sizeof (EFI_IPMI_SDR_REPOSITORY_INFO);
    ZeroMem (&RepositoryInfo, DataSize);
    Status = mIpmiTransport->SendIpmiCommand(
               mIpmiTransport,
               EFI_SM_NETFN_STORAGE,
               0,
               EFI_STORAGE_GET_SDR_REPOSITORY_INFO,
               NULL,
               0,
               (UINT8*) (&RepositoryInfo),
               (UINT8*) &DataSize
             );

    DEBUG ((EFI_D_INFO, "["DFT_ID"] EfiSendCommandToBMC GetSDRRepo: %r\n", Status));
    if (Status == EFI_SUCCESS) {



      GetSdr.ReservationId = 0x0000;
      GetSdr.RecordId      = 0x0000;
      GetSdr.RecordOffset  = 0x00;
      GetSdr.BytesToRead   = 0x9;

      for (Index = 0; (Index < RepositoryInfo.RecordCount) && (GetSdr.RecordId != 0xFFFF); Index++) {
        DataSize = sizeof (EFI_IPMI_SENSOR_RECORD_STRUCT);
        ZeroMem (&SdrRecord, DataSize);
        Status = mIpmiTransport->SendIpmiCommand(
                   mIpmiTransport,
                   EFI_SM_NETFN_STORAGE,
                   0,
                   EFI_STORAGE_GET_SDR,
                   (UINT8*) &GetSdr,
                   sizeof(EFI_IPMI_GET_SDR),
                   (UINT8*) &SdrRecord,
                   (UINT8*) &DataSize
                 );
        //DEBUG ((EFI_D_INFO, "["DFT_ID"] EfiSendCommandToBMC GetSDR %x: %r\n", GetSdr.RecordId, Status));
        if (EFI_ERROR (Status)) {
          break;
        }
        //
        // Check For SDR version Record.
        //
        if ((SdrRecord.SensorHeader.RecordType        == 0xC0) &&
            (SdrRecord.SensorTypeC0.ManufacturerId[0] == 0x57) &&
            (SdrRecord.SensorTypeC0.ManufacturerId[1] == 0x01) &&
            (SdrRecord.SensorTypeC0.ManufacturerId[2] == 0x00)
           ) {

          //
          // The SDR Version Record was found.  Save the Record ID and Length
          //
          if(SdrRecord.SensorTypeC0.StringChars[0] == 'S') {
            DataSize = sizeof (EFI_IPMI_SDR_RECORD_STRUCT_C0);
            GetSdr.BytesToRead   = SdrRecord.SensorTypeC0.RecordLength + 5;
            ZeroMem (&SdrRecord.SensorTypeC0.RecordId, DataSize - 2);
            Status = mIpmiTransport->SendIpmiCommand(
                       mIpmiTransport,
                       EFI_SM_NETFN_STORAGE,
                       0,
                       EFI_STORAGE_GET_SDR,
                       (UINT8*) &GetSdr,
                       sizeof(EFI_IPMI_GET_SDR),
                       (UINT8*) &SdrRecord,
                       (UINT8*) &DataSize
                     );
            //DEBUG ((EFI_D_INFO, "["DFT_ID"] EfiSendCommandToBMC GetSDR %x: %r\n", GetSdr.RecordId, Status));
            if(Status == EFI_SUCCESS) {
              if(AsciiStrnCmp(SdrRecord.SensorTypeC0.StringChars, "SDR Package", 8) == 0) {
                AsciiStrCpyS(pIfwiCfgVersionData->FRUSDRVersion, MAX_StringLength, SdrRecord.SensorTypeC0.StringChars);
                DEBUG ((EFI_D_INFO, "["DFT_ID"] Found SdrPkgVer: %a\n", SdrRecord.SensorTypeC0.StringChars));
              } else if(AsciiStrnCmp(SdrRecord.SensorTypeC0.StringChars, "SDR File", 8) == 0) {
                AsciiStrCpyS(pIfwiCfgVersionData->SDRVersion, MAX_StringLength, SdrRecord.SensorTypeC0.StringChars);
                DEBUG ((EFI_D_INFO, "["DFT_ID"] Found SdrFileVer: %a\n", SdrRecord.SensorTypeC0.StringChars));
              }
            }
          }
        }

        //
        // Update the command for the next record
        //
        GetSdr.RecordId = SdrRecord.SensorHeader.NextRecordId;
      }
    }
  }
}

/**
    GetPSUVersion:
        This function gets the PSU version.

    @param  pIfwiCfgVersionData     A piece of data buffer pointer.

    @retval EFI_SUCCESS                     success to get PSU version.
**/
EFI_STATUS GetPSUVersion(EFI_MISC_IFWI_CFG_VERSION * pIfwiCfgVersionData)
{
  EFI_STATUS                        Status;
  CHAR8                             TempStr[MAX_StringLength] = "";
  UINT8                             DataBuf[MaxSMBDataRead];
  PSU_REV_DATA                      *pPSURev;
  EFI_IPMI_MASTER_WRITE_READ_REQ    ReqData;
  UINT32                            DataSize;
  UINTN                             Index;
  Status = EFI_SUCCESS;
  if ((mIpmiTransport != NULL)) {

    //
    // Issue Mater-Write-read command
    //
    ReqData.BusID = 0x0f;
    ReqData.SlaveAddress = 0xB0;
    ReqData.ReadCount = 0x4;
    ReqData.DataToWrite = 0xD9;
    DataSize = MaxSMBDataRead;
    ZeroMem (DataBuf, DataSize);
    Status = mIpmiTransport->SendIpmiCommand(
               mIpmiTransport,
               EFI_SM_NETFN_APP,
               0,
               EFI_APP_MASTER_WRITE_READ,
               (UINT8*) &ReqData,
               sizeof(EFI_IPMI_MASTER_WRITE_READ_REQ),
               DataBuf,
               (UINT8*) &DataSize
             );

    DEBUG ((EFI_D_INFO, "["DFT_ID"] EfiSendCommandToBMC APP_MASTER_WRITE_READ: %r\n", Status));
    if (Status == EFI_SUCCESS) {
      for(Index = 0; Index < MaxSMBDataRead; Index++) {
        DEBUG ((EFI_D_INFO, "%x ", DataBuf[Index]));
      }
      DEBUG ((EFI_D_INFO, "\n"));
      pPSURev = (PSU_REV_DATA*)DataBuf;
      AsciiSPrint (TempStr, MAX_StringLength, "%d.%d.%d", pPSURev->MajorRev, pPSURev->MinorRevPri, pPSURev->MinorRevSec);
      AsciiStrCpyS(pIfwiCfgVersionData->PSUVersion, MAX_StringLength, TempStr);
    }
  }
  return Status;
}

/**
  Acquire the string associated with the Index from SMBIOS structure and return it.
  The caller is responsible for freeing the string buffer.

  @param    OptionalStrStart  The start position to search the string
  @param    Index             The index of the string to extract
  @param    String            The string that is extracted

  @retval   EFI_SUCCESS       The function returns EFI_SUCCESS if successful.
  @retval   EFI_NOT_FOUND     The function returns EFI_NOT_FOUND if unsuccessful.

**/
EFI_STATUS
SmbiosGetOptionalStringByIndex (
  IN      CHAR8                   *OptionalStrStart,
  IN      UINT8                   Index,
  OUT     CHAR16                  **String,
  IN      UINTN                   StringSize
  )
{
  UINTN          StrSize;

  if (Index == 0) {
    return EFI_INVALID_PARAMETER;
  }

  StrSize = 0;
  do {
    Index--;
    OptionalStrStart += StrSize;
    StrSize           = AsciiStrSize (OptionalStrStart); // size includes null terminator
  // SMBIOS strings are NULL terminated, and end of all strings is indicated with NULL
  // loop until at end of all SMBIOS strings (found NULL terminator at string index past string's NULL), and Index != 0
  } while (OptionalStrStart[StrSize] != 0 && Index != 0);

  if ((Index != 0) || (StrSize == 1)) {
    //
    // Meet the end of strings set but Index is non-zero
    return EFI_INVALID_PARAMETER;
  } else {
    AsciiStrToUnicodeStrS (OptionalStrStart, *String, StringSize);
  }

  return EFI_SUCCESS;
}


/**
    GetIfwiVersion:
        This function gets the IFWI version.

  @param  pIfwiCfgVersionData     A piece of data buffer pointer.

  @retval None
**/
VOID GetIfwiVersion(EFI_MISC_IFWI_CFG_VERSION * pIfwiCfgVersionData)
{
     EFI_STATUS      Status;
     IFWI_ID_IMAGE   IfwiIdImage;
     CHAR8           TempStr[MAX_StringLength] = "";

     Status = GetIfwiId (&IfwiIdImage);
     if (EFI_ERROR (Status)) {

        DEBUG ((EFI_D_ERROR, "["DFT_ID"] Get Ifwi Version failed.\n"));
        return;
     }
    DEBUG ((EFI_D_INFO, "["DFT_ID"] Ifwi Version: %s\n",  (CHAR16 *) (&(IfwiIdImage.IfwiIdString))));
    UnicodeStrToAsciiStrS ((CHAR16 *) (&(IfwiIdImage.IfwiIdString)), TempStr, MAX_StringLength);
    DEBUG ((EFI_D_INFO, "["DFT_ID"] Ifwi Version: %a\n",  TempStr));
    AsciiStrCpyS(pIfwiCfgVersionData->IFWIVersion, MAX_StringLength, TempStr);
}

/**
  Plat_GetRCVer: Get Platform RC Version

  @param VersionString                    - pointer to RC Version string

  @retval None

**/

VOID
Plat_GetRCVer(
  OUT UINT16 VersionString[VersionStringNumber]
)
{
  RC_VERSION  *RcVersion;

  RcVersion = (RC_VERSION *) PcdGetPtr (PcdRcVersion);

  VersionString[Major] = (UINT16) RcVersion->Major;
  VersionString[Minor] = (UINT16) RcVersion->Minor;
  VersionString[Revision] = (UINT16) RcVersion->Revision;
  VersionString[BuildNumber] = (UINT16) RcVersion->BuildNumber;
}


/**
  Plat_GetMEVer: Get Platform ME Version

  @param VersionString                    - pointer to ME Version string

  @retval None

**/

VOID
Plat_GetMEVer(
  OUT UINT16 VersionString[VersionStringNumber]
)
{
#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
  EFI_STATUS Status;
  GET_FW_VER_ACK_DATA *GetMEFwVRs = NULL;

    Status = gBS->AllocatePool(EfiBootServicesData, sizeof(GET_FW_VER_ACK_DATA), &GetMEFwVRs);
    if(!EFI_ERROR(Status) && (GetMEFwVRs != NULL))
    {
        gBS->SetMem(GetMEFwVRs,sizeof(GET_FW_VER_ACK_DATA),0);

        Status = HeciGetFwVersionMsg (GetMEFwVRs);

        DEBUG ((EFI_D_INFO, "["DFT_ID"] GetMeFWV : %r\n",Status));
        if(!EFI_ERROR(Status))
        {
            VersionString[Major] = (UINT16) (GetMEFwVRs->CodeMajor)&0xff;
            VersionString[Minor] = (UINT16) GetMEFwVRs->CodeMinor;
            VersionString[Revision] = (UINT16) GetMEFwVRs->CodeHotFix;
            VersionString[BuildNumber] = (UINT16) GetMEFwVRs->CodeBuildNo;
        }
        gBS->FreePool(GetMEFwVRs);
    }
#endif // ME_SUPPORT_FLAG
}

EFI_STATUS CollectIFWICfgVersionData(
  OUT EFI_MISC_IFWI_CFG_VERSION *pIfwiCfgVersionData,
  OUT UINT16                VersionString[VersionStringNumber]
)
/*++
Description:

  CollectIFWICfgVersionData.

Arguments:
  pIfwiCfgVersionData

Returns:

  EFI_SUCCESS             The data was successfully collected

--*/
{
  EFI_STATUS                    Status = EFI_INVALID_PARAMETER;
  UINT8                         i, OpRomCount;
  EFI_SM_VARIABLE               SmVariable;
  CHAR8                         TempStr[MAX_StringLength] = "";
  EFI_SM_VARIABLES_PROTOCOL     *SmVariableProtocol = NULL;
  CHAR8                         BlankStr[10] = "N/A";
  CHAR8                         *pStrData;
  UINT8                         *RomBuf = NULL;
  UINTN                         RomSize = 0;
  UINTN                         MinStrLen = 0;

  // Init all data versions to BlankStr
  pStrData = pIfwiCfgVersionData->IFWIVersion;
  for(i = 0; i < (sizeof(EFI_MISC_IFWI_CFG_VERSION) / MAX_StringLength); i++) {
    AsciiStrCpyS(pStrData, MAX_StringLength, BlankStr);
    pStrData += MAX_StringLength;
  }

    //BmcVersion: %s.%s%s.%s
    Status = gBS->LocateProtocol (
               &gEfiSmBmcVariableGuid,
               NULL,
               &SmVariableProtocol
             );
    DEBUG ((DEBUG_INFO, "["DFT_ID"] Locate SmVariableProtocol: %r\n", Status));

    if(!EFI_ERROR (Status)){
      Status = SmVariableProtocol->GetVariable (
                 SmVariableProtocol,
                 EfiBmcFruVariable,
                 0,
                 &SmVariable
               );
      DEBUG ((DEBUG_INFO, "["DFT_ID"] GetVariable EfiBmcFruVariable: %r\n", Status));
      VersionString[Major] = (UINT16) ((SmVariable.BmcFruVariable.DeviceId.MajorFirmwareRev) & 0x7f);
      VersionString[Minor] = (UINT16) ((SmVariable.BmcFruVariable.DeviceId.MinorFirmwareRev) & 0xff);
      VersionString[BuildNumber] = (UINT16) ((SmVariable.BmcFruVariable.DeviceId.AuxFirmwareRevInfo >> 16) & 0x0ffff );
    }

  //IfwiID
  GetIfwiVersion(pIfwiCfgVersionData);
  //SDR/FRUSDR Ver
  GetSDRVersions(pIfwiCfgVersionData);
  //FRU Ver
  GetFRUVersion(pIfwiCfgVersionData);
  //PSU Ver
  GetPSUVersion(pIfwiCfgVersionData);
  //MicrocodeVer
  GetMicrocodeVersions(pIfwiCfgVersionData);
  //BIOSAcm CheckSum
  GetBIOSACMCheckSum(pIfwiCfgVersionData);
  //OpromVer
  OpRomCount = 0;
  for(i = 0; i < RomNumMax; i++) {
    if(mIFWIRomData[i].Ffs_Guid.Data1 != 0) {
      AsciiStrCpyS(TempStr, MAX_StringLength, BlankStr);
      Status = GetSectionFromAnyFv (&mIFWIRomData[i].Ffs_Guid, mIFWIRomData[i].Ffs_SectionType, 0, (VOID **) &RomBuf, &RomSize);
      DEBUG ((DEBUG_INFO, "["DFT_ID"] Load Rom [%a] at%x,Size0x%x: %r\n", mIFWIRomData[i].RomName, (UINTN)(RomBuf), RomSize, Status));
      if ((Status == EFI_SUCCESS) && (RomBuf != NULL) && (RomSize > 0)) {
        //Found Rom in BIOS FVs
        mIFWIRomData[i].Rom_checksum = GetCheckSum(RomBuf, RomSize);
        DEBUG ((EFI_D_INFO, "["DFT_ID"] Checksum of %a: %08X\n", mIFWIRomData[i].RomName, mIFWIRomData[i].Rom_checksum));
        //Update into pIfwiCfgVersionData
        AsciiSPrint (TempStr, MAX_StringLength, "%08X", mIFWIRomData[i].Rom_checksum);
      }
      if(mIFWIRomData[i].bDisplayInOpromList) {
        AsciiStrCpyS(pIfwiCfgVersionData->OnboardOpRomVersion[OpRomCount], MAX_StringLength, TempStr);
        OpRomCount++;
      } else {
        MinStrLen = (sizeof ("SINIT") < sizeof (mIFWIRomData[i].RomName))? sizeof ("SINIT") : sizeof (mIFWIRomData[i].RomName);
        if (AsciiStrnCmp (mIFWIRomData[i].RomName, "SINIT", MinStrLen) == 0) {
          AsciiStrCpyS(pIfwiCfgVersionData->SINITACMVersion, MAX_StringLength, TempStr);
        }
      }
    }
  }
  //Dump for Debug
  DEBUG ((EFI_D_INFO, "["DFT_ID"] IFWIVersion: %a\n", pIfwiCfgVersionData->IFWIVersion));
  DEBUG ((EFI_D_INFO, "["DFT_ID"] MEVersion: %a\n", pIfwiCfgVersionData->MEVersion));
  DEBUG ((EFI_D_INFO, "["DFT_ID"] FRUSDRVersion: %a\n", pIfwiCfgVersionData->FRUSDRVersion));
  DEBUG ((EFI_D_INFO, "["DFT_ID"] FRUVersion: %a\n", pIfwiCfgVersionData->FRUVersion));
  DEBUG ((EFI_D_INFO, "["DFT_ID"] SDRVersion: %a\n", pIfwiCfgVersionData->SDRVersion));
  DEBUG ((EFI_D_INFO, "["DFT_ID"] CPU1PatchVersion: %a\n", pIfwiCfgVersionData->CPUPatchVersion[0]));
  DEBUG ((EFI_D_INFO, "["DFT_ID"] CPU2PatchVersion: %a\n", pIfwiCfgVersionData->CPUPatchVersion[1]));
  DEBUG ((EFI_D_INFO, "["DFT_ID"] CPU3PatchVersion: %a\n", pIfwiCfgVersionData->CPUPatchVersion[2]));
  DEBUG ((EFI_D_INFO, "["DFT_ID"] BIOSACMVersion: %a\n", pIfwiCfgVersionData->BIOSACMVersion));
  DEBUG ((EFI_D_INFO, "["DFT_ID"] SINITACMVersion: %a\n", pIfwiCfgVersionData->SINITACMVersion));
  DEBUG ((EFI_D_INFO, "["DFT_ID"] MRCVersion: %a\n", pIfwiCfgVersionData->MRCVersion));
  DEBUG ((EFI_D_INFO, "["DFT_ID"] QPIRCVersion: %a\n", pIfwiCfgVersionData->QPIRCVersion));
  DEBUG ((EFI_D_INFO, "["DFT_ID"] PSUVersion: %a\n", pIfwiCfgVersionData->PSUVersion));
  for(i = 0; i < OpRomCount; i++) {
    DEBUG ((EFI_D_INFO, "["DFT_ID"] OnboardOpRomVersion[%02d]: %a\n", i, pIfwiCfgVersionData->OnboardOpRomVersion[i]));
  }
  return Status;
}

 /**
  Create SMBIOS Table type

  @param[in] Event    The Event this notify function registered to.
  @param[in] Context  Pointer to the context data registered to the Event.
 **/
VOID
EFIAPI
InitializeFviDataCallback (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
 {
  EFI_STATUS   Status;
  UINT16       VersionString[VersionStringNumber];
  EFI_MISC_IFWI_CFG_VERSION	pIfwiCfgVersionData;

  ZeroMem(&pIfwiCfgVersionData, sizeof(EFI_MISC_IFWI_CFG_VERSION));
  CollectIFWICfgVersionData(&pIfwiCfgVersionData, VersionString);
  gBS->CloseEvent (Event);

  //
  // Initialize and report Platform FVI information to Smbios
  //
  mPlatformFviHeader.SmbiosHeader.Type = EFI_SMBIOS_TYPE_IFWI_CFG;

  // BMC_FW_STRING
  mPlatformFviData[BMC_FW_VER].Version.MajorVersion = (UINT8) VersionString[Major];
  mPlatformFviData[BMC_FW_VER].Version.MinorVersion = (UINT8) VersionString[Minor];
  mPlatformFviData[BMC_FW_VER].Version.Revision = (UINT8) NO_STRING_AVAILABLE;
  mPlatformFviData[BMC_FW_VER].Version.BuildNum = (UINT16) VersionString[BuildNumber];

  // ME_FW_STRING
  Plat_GetMEVer(VersionString);
  mPlatformFviData[ME_FW_VER].Version.MajorVersion = (UINT8) VersionString[Major];
  mPlatformFviData[ME_FW_VER].Version.MinorVersion = (UINT8) VersionString[Minor];
  mPlatformFviData[ME_FW_VER].Version.Revision = (UINT8) VersionString[Revision];
  mPlatformFviData[ME_FW_VER].Version.BuildNum = (UINT16) VersionString[BuildNumber];

  // BIOS_ACM_STRING
  GetBIOSACMCheckSum(&pIfwiCfgVersionData);
  mPlatformFviStrings[BIOS_ACM_VER].VersionString = (CHAR8 *) &pIfwiCfgVersionData.BIOSACMVersion;

  // FRU_SDR_STRING
  mPlatformFviStrings[FRU_SDR_VER].VersionString = (CHAR8 *) &pIfwiCfgVersionData.FRUSDRVersion;

  // FRU_STRING
  mPlatformFviStrings[FRU_VER].VersionString = (CHAR8 *) &pIfwiCfgVersionData.FRUVersion;

  // SDR_STRING
  mPlatformFviStrings[SDR_VER].VersionString = (CHAR8 *) &pIfwiCfgVersionData.SDRVersion;

  // CPU_UCODE1_STRING
  mPlatformFviStrings[CPU_UCODE1_VER].VersionString = (CHAR8 *) &pIfwiCfgVersionData.CPUPatchVersion[0];

  // CPU_UCODE2_STRING
  mPlatformFviStrings[CPU_UCODE2_VER].VersionString = (CHAR8 *) &pIfwiCfgVersionData.CPUPatchVersion[1];

  // CPU_UCODE3_STRING
  mPlatformFviStrings[CPU_UCODE3_VER].VersionString = (CHAR8 *) &pIfwiCfgVersionData.CPUPatchVersion[2];

  // SINIT_ACM_STRING
  mPlatformFviStrings[SINIT_ACM_VER].VersionString = (CHAR8 *) &pIfwiCfgVersionData.SINITACMVersion;

  // MRC_STRING
  Plat_GetRCVer(VersionString);
  mPlatformFviData[MRC_VER].Version.MajorVersion = (UINT8) VersionString[Major];
  mPlatformFviData[MRC_VER].Version.MinorVersion = (UINT8) VersionString[Minor];
  mPlatformFviData[MRC_VER].Version.Revision = (UINT8) VersionString[Revision];
  mPlatformFviData[MRC_VER].Version.BuildNum = (UINT16) VersionString[BuildNumber];

  // QPIRC_STRING
  mPlatformFviData[QPIRC_VER].Version.MajorVersion = (UINT8) VersionString[Major];
  mPlatformFviData[QPIRC_VER].Version.MinorVersion = (UINT8) VersionString[Minor];
  mPlatformFviData[QPIRC_VER].Version.Revision = (UINT8) VersionString[Revision];
  mPlatformFviData[QPIRC_VER].Version.BuildNum = (UINT16) VersionString[BuildNumber];

  // PSU_FW_STRING
  mPlatformFviStrings[PSU_FW_VER].VersionString = (CHAR8 *) &pIfwiCfgVersionData.PSUVersion;

  // RSTeSataEFI_STRING
  mPlatformFviStrings[RSTeSataEFI_VER].VersionString = (CHAR8 *) &pIfwiCfgVersionData.OnboardOpRomVersion[RSTeSataEFI_Index];

  // RSTesSataEFI_STRING
  mPlatformFviStrings[RSTesSataEFI_VER].VersionString = (CHAR8 *) &pIfwiCfgVersionData.OnboardOpRomVersion[RSTesSataEFI_Index];

  // OneGEFI_STRING
  mPlatformFviStrings[OneGEFI_VER].VersionString = (CHAR8 *) &pIfwiCfgVersionData.OnboardOpRomVersion[OneGEFI_Index];

  // iBMCGOP_STRING
  mPlatformFviStrings[iBMCGOP_VER].VersionString = (CHAR8 *) &pIfwiCfgVersionData.OnboardOpRomVersion[iBMCGOP_Index];

  // VMDDxeEfi_STRING
  mPlatformFviStrings[VMDDxeEfi_VER].VersionString = (CHAR8 *) &pIfwiCfgVersionData.OnboardOpRomVersion[VMDDxeEfi_Index];

  // RSTeSataRaidEfi_STRING
  mPlatformFviStrings[RSTeSataRaidEfi_VER].VersionString = (CHAR8 *) &pIfwiCfgVersionData.OnboardOpRomVersion[RSTeSataRaidEfi_Index];

  // NvmDimmDriver_STRING
  mPlatformFviStrings[NvmDimmDriver_VER].VersionString = (CHAR8 *) &pIfwiCfgVersionData.OnboardOpRomVersion[NvmDimmDriver_Index];

  // NvmDimmHii_STRING
  mPlatformFviStrings[NvmDimmHii_VER].VersionString = (CHAR8 *) &pIfwiCfgVersionData.OnboardOpRomVersion[NvmDimmHii_Index];

  Status = AddFviEntry (mPlatformFviHeader, mPlatformFviData, mPlatformFviStrings);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AddFviEntry failed. Status = %r\n", Status));
   }
  return;
 }
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : Removing IPMI feature code

 /**
  This is driver entry point to register the notification event.

  @param[in] ImageHandle  A handle for the image that is initializing this driver
  @param[in] SystemTable  A pointer to the EFI system table

  @retval    EFI_SUCCESS  The initialization finished successfully.
 **/
EFI_STATUS
EFIAPI
DxeFirmwareVersionInfoLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
 {
// APTIOV_SERVER_OVERRIDE_RC_START : Removing IPMI feature code
  EFI_STATUS  Status = EFI_SUCCESS;
#if 0
  EFI_EVENT   ReadyToBootEvent;

  DEBUG_FUN_ENTRY;
  //
  // Register InitializeFviDataCallback
  //
  Status = EfiCreateEventReadyToBootEx (
          // &gBdsAllDriversConnectedProtocolGuid,
          TPL_CALLBACK,
          InitializeFviDataCallback,
          NULL,
          &ReadyToBootEvent
          );
  DEBUG_FUN_EXIT;
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : Removing IPMI feature code
  return Status;
 }

#pragma optimize( "", on)
