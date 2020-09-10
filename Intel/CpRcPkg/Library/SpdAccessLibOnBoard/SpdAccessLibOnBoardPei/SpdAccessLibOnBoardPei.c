/** @file
  This library exposes the SPD read function for use during PEI.

  This implementation works as an interface between an agent that needs
  to read a DIMM SPD and the SPD.  The read function accepts DIMM location information as well
  as the SPD byte offset.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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
#include <Library/SysHostPointerLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/SpdAccessLibOnBoard.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>

extern EFI_GUID gSpdTableHobGuid;
extern EFI_GUID gOnBoardSpdTableGuid;

/**
  Set SpdModuleType.

  @param Socket        - Socket Id
  @param Ch            - Channel number (0-based)
  @param Dimm          - Current dimm
  @param SpdModuleType - Current Spd module type

  @retval N/A
**/
VOID
SetSpdModuleType (
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm,
  UINT8    SpdModuleType
  )
{
  PSYSHOST Host;

  Host = (PSYSHOST) GetSysHostPointer ();

  Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].SpdModuleType = SpdModuleType;
} // SetSpdModuleType

/**
  Returns current SpdModuleType.

  @param Socket  - Socket Id
  @param Ch      - Channel number (0-based)
  @param Dimm    - Current dimm

  @retval SPD_TYPE_DDR4         0x0C  // DDR4 SDRAM
  @retval SPD_TYPE_AEP          0x0D  // DDRT SDRAM
  @retval SPD_TYPE_DDR5         0x12  // DDR5 SDRAM
**/
UINT8
GetSpdModuleType (
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm
  )
{
  PSYSHOST Host;

  Host = (PSYSHOST) GetSysHostPointer ();

  return (Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].SpdModuleType);
} // GetSpdModuleType

/**
  Searches the SPD table FV in the available FVs

  @param [out] SpdFileInfo - Pointer to store FV data
  @param [out] Status      - Pointer to store the status

  @retval N/A
**/
VOID
FindSpdTableFile (
  OUT EFI_FV_FILE_INFO *SpdFileInfo,
  OUT EFI_STATUS       *Status
  )
{
  UINTN               Instance = 0;
  EFI_PEI_FV_HANDLE   VolumeHandle;
  EFI_PEI_FILE_HANDLE FileHandle;

  while (TRUE) {
    //
    // Pick the next available volume
    //
    *Status = PeiServicesFfsFindNextVolume (Instance++, &VolumeHandle);
    if (EFI_ERROR (*Status)) {
      DEBUG ((EFI_D_ERROR, "SPD FFS: No more volumes, couldn't find SPD FFS file %r!\n", *Status));
      break;
    }
    //
    // Evaulate if the current FV match the GUID if the PV we are looking for
    //
    *Status = PeiServicesFfsFindFileByName (&gOnBoardSpdTableGuid, VolumeHandle, &FileHandle);
    if (!EFI_ERROR (*Status)) {
      //
      // Get the FV data
      //
      *Status = PeiServicesFfsGetFileInfo (FileHandle, SpdFileInfo);
      if (EFI_ERROR (*Status)) {
        DEBUG ((EFI_D_ERROR, "SPD FFS: Unable to get file info %r!\n", *Status));
      }
      DEBUG ((EFI_D_INFO, "SPD FFS: Found file & file info returned!\n"));
      break;
    } else {
      continue;
    }
  }
}

/**
  Fill the SPD information with the data in the FV file.

  @param [   out] SpdTable      - SPD table to fill
  @param [in    ] SpdFilePtr    - Pointer to the table file
  @param [in    ] MaxSpdSize    - Max SPD size in DDR4 or DDR5
  @param [in    ] LenghtOfTable - Size of table in bytes

  @retval N/A
**/
VOID
FillDimmSpdDataInTable (
     OUT UINT8  *SpdTable,
  IN     UINT8  *SpdFilePtr,
  IN     UINT16 MaxSpdSize,
  IN     UINT16 LenghtOfTable
  )
{
  UINT8                        TableIndex;
  UINT8                        NumSpds;
  UINT8                        Channel;
  UINT8                        Dimm;
  UINT8                        SpdIndex;
  ON_BOARD_SPD_TABLE_STRUCTURE *SpdFilePtrHandler;
  UINT16                       SpdImageOffset;
  UINT8                        MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  NumSpds = (UINT8)(LenghtOfTable / MaxSpdSize);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      SpdIndex = (Channel * MAX_DIMM) + Dimm;
      //
      // Loop the table to fill the SPD local table with the valid data in the FV
      //
      for (TableIndex = 0; TableIndex < NumSpds; TableIndex++) {
        SpdImageOffset = TableIndex * sizeof (ON_BOARD_SPD_TABLE_STRUCTURE);
        //
        // The index is calculated by using the SPD table pointer and the SPD image offset
        //
        SpdFilePtrHandler = (ON_BOARD_SPD_TABLE_STRUCTURE *)(SpdFilePtr + SpdImageOffset);

        if ((Channel == SpdFilePtrHandler->Location.ChannelLocation) && (Dimm == SpdFilePtrHandler->Location.DimmLocation)) {
          CopyMem (SpdTable + (SpdIndex * MaxSpdSize), &SpdFilePtrHandler->SpdData, MaxSpdSize);
          break;
        }
      } // TableIndex loop
    } // Dimm loop
  } // Channel loop
}

/**
  Performs SPD byte read operations.

  @param [in    ] Socket     - Socket ID
  @param [in    ] Channel    - Memory channel number
  @param [in    ] Dimm       - Memory DIMM slot
  @param [in    ] ByteOffset - SPD byte to be returned
  @param [   out] Data       - Pointer to store the SPD byte

  @retval EFI_SUCCESS           The read completed successfully
  @retval EFI_DEVICE_ERROR      Error configuring or during SMBus transaction
  @retval EFI_INVALID_PARAMETER Input parameter out of bounds
  @retval EFI_NOT_FOUND         Specifed DIMM SPD information was not found
**/
EFI_STATUS
EFIAPI
SpdReadByteDDR (
  IN     UINT8  Socket,
  IN     UINT8  Channel,
  IN     UINT8  Dimm,
  IN     UINT16 ByteOffset,
     OUT UINT8  *Data
  )
{
  EFI_STATUS        Status = EFI_SUCCESS;
  UINT8             (*SpdTable)[MAX_CH][MAX_DIMM][MAX_SPD_BYTE_DDR];
  EFI_FV_FILE_INFO  SpdFileInfo;
  UINT8             *SpdFilePtr;
  EFI_HOB_GUID_TYPE *GuidHob = NULL;

  //
  // Get SPD table HOB
  //
  GuidHob = GetFirstGuidHob (&gSpdTableHobGuid);
  if (GuidHob != NULL) {
    SpdTable = GET_GUID_HOB_DATA (GuidHob);
  } else {
    //
    // Build the SPD table HOB
    //
    SpdTable = BuildGuidHob (&gSpdTableHobGuid, sizeof (*SpdTable));

    //
    // Validate that the pointer is not NULL
    //
    if (SpdTable == NULL) {
      ASSERT (FALSE);
      return EFI_OUT_OF_RESOURCES;
    }

    //
    // Initialize the array to invalid data
    //
    SetMem (SpdTable, sizeof (*SpdTable), SPD_BYTE0_UNDEFINED);

    //
    // Find the SPD Table FV
    //
    FindSpdTableFile (&SpdFileInfo, &Status);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Fail to find SPD module in FV\n"));
      Status = EFI_NOT_FOUND;
    }
    //
    // Evaluate if the header contains data
    //
    ASSERT (SpdFileInfo.Buffer != 0);

    DEBUG ((EFI_D_INFO, "Found SPD module in FV\n"));

    //
    // Extract the data after the header
    //
    SpdFilePtr = (UINT8 *)((UINT32)SpdFileInfo.Buffer + FFS_RAW_HEADER_SIZE + sizeof (EFI_ACPI_DESCRIPTION_HEADER));
    //
    // Fill the local SPD array with the valid data in the FV
    //
    FillDimmSpdDataInTable (SpdTable[0][0][0], SpdFilePtr, MAX_SPD_BYTE_DDR, sizeof (*SpdTable));
  }

  if ((*SpdTable)[Channel][Dimm][0] == SPD_BYTE0_UNDEFINED) {
    Status = EFI_NOT_FOUND;
  } else {
    *Data = (*SpdTable)[Channel][Dimm][ByteOffset];
  }

  return Status;
}

/**
  Performs SPD byte read operations.

  @param [in    ] Socket     - Socket ID
  @param [in    ] Channel    - Memory channel number
  @param [in    ] Dimm       - Memory DIMM slot
  @param [in    ] ByteOffset - SPD byte to be returned
  @param [   out] Data       - Pointer to store the SPD byte

  @retval EFI_SUCCESS           The read completed successfully
  @retval EFI_DEVICE_ERROR      Error configuring or during SMBus transaction
  @retval EFI_INVALID_PARAMETER Input parameter out of bounds
  @retval EFI_NOT_FOUND         Specifed DIMM SPD information was not found
**/
EFI_STATUS
EFIAPI
SpdReadByte (
  IN     UINT8  Socket,
  IN     UINT8  Channel,
  IN     UINT8  Dimm,
  IN     UINT16 ByteOffset,
     OUT UINT8  *Data
  )
{
  EFI_STATUS        Status = EFI_SUCCESS;
  EFI_FV_FILE_INFO  SpdFileInfo;
  UINT8             *SpdFilePtr;
  UINT8              SpdModuleType;
  ON_BOARD_SPD_TABLE_STRUCTURE *SpdFilePtrHandler;
  UINT8             (*SpdTable)[MAX_CH][MAX_DIMM][MAX_SPD_BYTE_DDR];
  EFI_HOB_GUID_TYPE *GuidHob = NULL;

  SpdModuleType = 0xFF;

  //
  // Sanity check of incoming data
  //
  if ((Socket >= MAX_SOCKET) || (Channel >= MAX_CH) || (Dimm >= MAX_DIMM) || (Data == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  // Read SpdModuleType from sysHost to detect DDR4 or DDR5.
  SpdModuleType = GetSpdModuleType (Socket, Channel, Dimm);

  // Read SpdModuleType from Hardware if the data in sysHost is not initalized.
  if (SpdModuleType == 0xFF || SpdModuleType == 0) {
    // Read SPD Byte 2(Key Byte) to detect its module type.
    //
    // If platform has published the SPD table HOB then don't read the FV
    //
    GuidHob = GetFirstGuidHob (&gSpdTableHobGuid);
    if (GuidHob != NULL) {
      SpdTable = GET_GUID_HOB_DATA (GuidHob);
      SpdModuleType = (*SpdTable)[Channel][Dimm][SPD_KEY_BYTE];
      // Write back the SpdModuleType into sysHost.
      SetSpdModuleType (Socket, Channel, Dimm, SpdModuleType);
    } else {
      //
      // Find the SPD Table FV
      //
      FindSpdTableFile (&SpdFileInfo, &Status);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Fail to find SPD module in FV\n"));
        Status = EFI_NOT_FOUND;
      }
      //
      // Evaluate if the header contains data
      //
      ASSERT (SpdFileInfo.Buffer != 0);

      DEBUG ((EFI_D_INFO, "Found SPD module in FV\n"));

      //
      // Extract the data after the header
      //
      SpdFilePtr = (UINT8 *)((UINT32)SpdFileInfo.Buffer + FFS_RAW_HEADER_SIZE + sizeof (EFI_ACPI_DESCRIPTION_HEADER));
      //
      // The index is calculated by using the SPD table pointer and the SPD image offset
      //
      SpdFilePtrHandler = (ON_BOARD_SPD_TABLE_STRUCTURE *)(SpdFilePtr + ((Channel * MAX_DIMM) + Dimm) * sizeof (ON_BOARD_SPD_TABLE_STRUCTURE));

      if ((Channel == SpdFilePtrHandler->Location.ChannelLocation) && (Dimm == SpdFilePtrHandler->Location.DimmLocation)) {
        SpdModuleType = SpdFilePtrHandler->SpdData[SPD_KEY_BYTE];
        // Write back the SpdModuleType into sysHost.
        SetSpdModuleType(Socket, Channel, Dimm, SpdModuleType);
      } else {
        Status = EFI_NOT_FOUND;
      }
    }
  }

  // Do parameter check for different DDR type.
  switch (SpdModuleType) {
    case SPD_TYPE_DDR4:
    case SPD_TYPE_AEP:
      if (ByteOffset >= MAX_SPD_BYTE_DDR4) {
        return EFI_INVALID_PARAMETER;
      }
      Status = SpdReadByteDDR (Socket, Channel, Dimm, ByteOffset, Data);
      break;

    case SPD_TYPE_DDR5:
      if (ByteOffset >= MAX_SPD_BYTE_DDR5) {
        return EFI_INVALID_PARAMETER;
      }
      Status = SpdReadByteDDR (Socket, Channel, Dimm, ByteOffset, Data);
      break;

    default:
      Status = EFI_NOT_FOUND;
      break;
  }

  return Status;
}
