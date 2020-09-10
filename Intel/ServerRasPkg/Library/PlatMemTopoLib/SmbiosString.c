/** @file
  Implementation of get SMBIOS String.

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

#include <Library/RasDebugLib.h>
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>
#include <SiliconSetting.h>
#include <SmbiosString.h>

EFI_STATUS
SmbiosGetOptionalStringByIndex (
  IN   CHAR8                   *OptionalStrStart,
  IN   UINT8                   Index,
  IN   UINTN                   StringSize,
  OUT  CHAR16                  **String
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
    StrSize = AsciiStrSize (OptionalStrStart); // size includes null terminator
    //
    // SMBIOS strings are NULL terminated, and end of all strings is indicated with NULL
    // loop until at end of all SMBIOS strings (found NULL terminator at string index past string's NULL), and Index != 0
    //
  } while (OptionalStrStart[StrSize] != 0 && Index != 0);
  if ((Index != 0) || (StrSize == 1)) {
    //
    // Meet the end of strings set but Index is non-zero
    //
    return EFI_INVALID_PARAMETER;
  } else {
    AsciiStrToUnicodeStrS (OptionalStrStart, *String, StringSize);
  }

  return EFI_SUCCESS;
}

EFI_STRING
EFIAPI
CreatMemoryTopoString (
  IN     UINT8          Socket,
  IN     UINT8          Channel,
  IN     UINT8          Dimm
  )
{
  CHAR16             *NewString;
  CHAR16             SizeString[STRING_BUFFER_SIZE];

  NewString = AllocateZeroPool (DIMMINFO_SIZE);

  if (NewString == NULL) {
    DEBUG ((EFI_D_ERROR, "\nEFI_OUT_OF_RESOURCES!!! AllocateZeroPool() returned NULL pointer.\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return NULL;
  }

  StrCpyS (NewString, DIMMINFO_SIZE / sizeof (CHAR16), L"CPU");
  UnicodeValueToStringS (SizeString, STRING_BUFFER_SIZE, LEFT_JUSTIFY, Socket, 1);
  StrCatS (NewString, DIMMINFO_SIZE / sizeof (CHAR16), SizeString);
  StrCatS (NewString, DIMMINFO_SIZE / sizeof (CHAR16), L"_DIMM_");
  SizeString[0] = Channel+65;
  SizeString[1] = '\0';
  StrCatS (NewString, DIMMINFO_SIZE / sizeof (CHAR16), SizeString);
  UnicodeValueToStringS (SizeString, STRING_BUFFER_SIZE, LEFT_JUSTIFY, Dimm + 1, 1);
  StrCatS (NewString, DIMMINFO_SIZE / sizeof (CHAR16), SizeString);
  return NewString;
}

UINT32
EFIAPI
CreateMemTopoMask (
  VOID
  )
{
  UINT8                       i, j, n;
  EFI_STATUS                  Status;
  EFI_SMBIOS_PROTOCOL         *SmbiosProtocol = NULL;
  EFI_SMBIOS_TABLE_HEADER     *SmbiosRecord;
  EFI_SMBIOS_TYPE             SmbiosType17;
  SMBIOS_TABLE_TYPE17         *SmbiosRecord17;
  EFI_SMBIOS_HANDLE           SmbiosHandle17;
  EFI_STRING                  NewString = NULL;
  UINT8                       StrIndex;
  CHAR16                      *SMBIOSType17LocatorStr = NULL;
  UINT32                      MemTepoMask = 0x0;
  BOOLEAN                     FoundFlag = FALSE;
  UINT32                      Mask = BIT0;
  UINT8                       CurSoc = 0;
  UINT8                       CurCha = 0;
  UINT8                       CurDimm = 0;
  UINTN                       StrSize = DIMMINFO_SIZE;

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &SmbiosProtocol);
  if (!EFI_ERROR (Status)) {
    SmbiosHandle17 = SMBIOS_HANDLE_PI_RESERVED;
    SmbiosType17 = EFI_SMBIOS_TYPE_MEMORY_DEVICE;

    //
    // Need to check platform SMBIOS type 16,17 setting. Current project each mc has type 16.
    //
    SMBIOSType17LocatorStr     = AllocateZeroPool (StrSize);
    if (SMBIOSType17LocatorStr == NULL) {
      ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
      return MemTepoMask;
    }
    do {
      Status = SmbiosProtocol->GetNext (SmbiosProtocol, &SmbiosHandle17, &SmbiosType17, &SmbiosRecord, NULL);
      if (Status == EFI_SUCCESS) {
        SmbiosRecord17 = (SMBIOS_TABLE_TYPE17 *) SmbiosRecord;
        StrIndex = SmbiosRecord17->DeviceLocator;
        Status = SmbiosGetOptionalStringByIndex ((CHAR8 *) ((UINT8 *) SmbiosRecord17 + SmbiosRecord17->Hdr.Length),
                                                    StrIndex, StrSize / sizeof (CHAR16), &SMBIOSType17LocatorStr);
        if (EFI_ERROR (Status)) {
          break;
        }
        for (i = CurSoc; i < MAX_SOCKET; i++) {
          for (j = CurCha; j < IMC_MAX_CH; j++) {
            for (n = CurDimm; n < MAX_DIMM; n++) {
              NewString = CreatMemoryTopoString (i, j, n);
              if (NewString == NULL) {
                DEBUG ((EFI_D_ERROR, "\nCreatMemoryTopoString returned NULL pointer.\n"));
                ASSERT (NewString != NULL);
                MemTepoMask = 0x0;
                goto Done;
              }
              if (StrCmp (NewString, SMBIOSType17LocatorStr)==0) {
                MemTepoMask |= Mask;
                Mask = Mask << 1;
                FoundFlag = TRUE;
              } else {
                FoundFlag = FALSE;
                Mask = Mask << 1;
              }
              CurSoc = i;
              CurCha = j;
              CurDimm = n + 1; // For next compare location
              if (CurDimm >= MAX_DIMM) {
                CurDimm = 0;
                CurCha = CurCha + 1;
                if (CurCha >= IMC_MAX_CH){
                  CurCha = 0;
                  CurSoc = CurSoc + 1;
                  if (CurSoc >= MAX_SOCKET) {
                     goto Done;
                  }
                }
              }
              if (FoundFlag) {
                break;
              }
            }// for dimm
            if (FoundFlag) {
              break;
            }
          }// for channel
          if (FoundFlag) {
            break;
          }
        }// for socket
      } else {
        break;
      } // SMBIOS GetNext
    } while ((FoundFlag) && (SmbiosHandle17 != SMBIOS_HANDLE_PI_RESERVED) && (CurSoc < MAX_SOCKET));
  }

  Done:
  if (SMBIOSType17LocatorStr != NULL) {
    FreePool (SMBIOSType17LocatorStr);
  }
  return MemTepoMask;
}

/**
  Set SMBIOS 16, 17 Handler in MemTopology.

  @param  IN OUT MemTopology  MEM_TOPOLOGY instance

  @retval none

**/
VOID
EFIAPI
SetMemTopoSmbios (
  IN OUT MEM_TOPOLOGY  *MemTopology
  )
{
  EFI_STATUS                  Status;
  EFI_SMBIOS_PROTOCOL        *SmbiosProtocol = NULL;
  EFI_SMBIOS_TABLE_HEADER     *SmbiosRecord;
  EFI_SMBIOS_TYPE             SmbiosType16;
  SMBIOS_TABLE_TYPE16        *SmbiosRecord16;
  EFI_SMBIOS_HANDLE           SmbiosHandle16;
  EFI_SMBIOS_TYPE             SmbiosType17;
  SMBIOS_TABLE_TYPE17        *SmbiosRecord17;
  EFI_SMBIOS_HANDLE           SmbiosHandle17;
  UINT8                       i, j, k;
  UINT32                      MemTepoValue;

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &SmbiosProtocol);
  if (!EFI_ERROR(Status)) {
    SmbiosHandle16 = SMBIOS_HANDLE_PI_RESERVED;
    SmbiosType16 = EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY;

    SmbiosHandle17 = SMBIOS_HANDLE_PI_RESERVED;
    SmbiosType17 = EFI_SMBIOS_TYPE_MEMORY_DEVICE;

    //
    // Need to check platform SMBIOS type 16,17 setting. Current project each mc has type 16.
    //
    for (i = 0; i < MC_MAX_NODE; i++) {
      Status = SmbiosProtocol->GetNext (SmbiosProtocol, &SmbiosHandle16, &SmbiosType16, &SmbiosRecord, NULL);
      if (Status == EFI_SUCCESS) {
        SmbiosRecord16 = (SMBIOS_TABLE_TYPE16 *) SmbiosRecord;
        MemTopology->Socket[NODE_TO_SKT (i)].SmbiosType16Handle[NODE_TO_MC (i)] = SmbiosRecord16->Hdr.Handle;
      } else {
        break;
      }
      if (SmbiosHandle16 == SMBIOS_HANDLE_PI_RESERVED) {
        break;
      }
    }

    MemTepoValue = CreateMemTopoMask ();

    for (i = 0; i < MAX_SOCKET; i++) {
      for (j = 0; j < IMC_MAX_CH; j++) {
        for (k = 0; k < MAX_DIMM; k++) {
          if (MemTepoValue & BIT0) {
            MemTepoValue = MemTepoValue >> 1;
            Status = SmbiosProtocol->GetNext (SmbiosProtocol, &SmbiosHandle17, &SmbiosType17, &SmbiosRecord, NULL);
            if (Status == EFI_SUCCESS) {
              SmbiosRecord17 = (SMBIOS_TABLE_TYPE17 *) SmbiosRecord;
              MemTopology->Socket[i].ChannelInfo[j].DimmInfo[k].SmbiosType17Handle = SmbiosRecord17->Hdr.Handle;
            }
            if (SmbiosHandle17 == SMBIOS_HANDLE_PI_RESERVED) {
              break;
            }
          } else {
            MemTepoValue = MemTepoValue >> 1;
            break;
          }
        }
      }
    }
  }
}
