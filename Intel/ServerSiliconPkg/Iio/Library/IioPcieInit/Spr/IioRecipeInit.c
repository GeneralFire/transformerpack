/** @file

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

#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>

#include <Library/IioAccessLib.h>
#include <Chip/Include/EvAutoRecipe.h>
#include <RcRegs.h>
#include "IioRecipeInit.h"
#include <Library/IioDataHubLib.h>
#include <Library/IioSysInfoLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/UefiDecompressLib.h>
#include <Library/KtiApi.h>
#include <Library/HsphyIpLib.h>
#include <Library/EvAutoRecipeLib.h>
#include <LibraryPrivate/IioPcieLib.h>
#include <Chip/Include/CpuPciAccess.h>

#include <Library/RcDebugLib.h>
#include <LibraryPrivate/IioDebug.h>

//
// Uncomment to debug recipe programming procedures.
//
#define IIO_D_PHYDBG(...)  //{ RcDebugPrint (SDBG_ERROR, "[IIO](PHY) " __VA_ARGS__); }

/**
  Function to check if recipe entry is valid.

  @param[in] IioIndex         - IIO number
  @param[in] EvRevisionBit    - Revision shift amount determined by EvAutoRecipe Library
  @param[in] EV_RECIPE_ENTRY  - IIO EV recipe entry.

  @return Returns True if the EV Recipe Entry is valid.
**/
BOOLEAN
IsValidRecipeEntry (
  IN  UINT8                   IioIndex,
  IN  UINT8                   EvRevisionBit,
  IN  EV_RECIPE_ENTRY         RecipeTable
  )
{
  if (!(IsCpuAndRevision (CPU_SPRSP, REV_ALL) && (RecipeTable.ProcessorType == SPR_SOCKET))) {
    return FALSE;
  }
  if (!(EvAutoRecipeIsValidRevision (EvRevisionBit, RecipeTable.Steppings))) {
    return FALSE;
  }
  if (!((RecipeTable.SocBitmap == WILDCARD_32) || (RecipeTable.SocBitmap & (1 << IioIndex)))) {
    return FALSE;
  }

  return TRUE;
}

/**
  Function to check if Recipe Entry is valid/applicable for this cpu.

  @param[in] IioGlobalData            - Pointer to IIO_GLOBALS structure
  @param[in] IioIndex                 - Index to Iio
  @param[in] PortIndex                - Iio port number
  @param[in] LaneBitmap               - Lane Bit map to select which lanes can be configured

  @return True if the recipe entry is applicable for the given port index based on bifurcation.
**/
BOOLEAN
IsValidLaneBitMapEntry (
  IN IIO_GLOBALS   *IioGlobalData,
  IN UINT8         IioIndex,
  IN UINT8         PortIndex,
  IN UINT32        LaneBitmap
  )
{
  BOOLEAN          Result = FALSE;
  UINT8            Bifurcation;
  UINT8            Port;

  Bifurcation = GetAssignedPortIndexWidth (IioGlobalData, IioIndex, PortIndex);
  if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort) || LaneBitmap == MAX_PORT_LINK_WIDTH_BIT_MAP) {
    return Result = TRUE;
  }

  Port = (PortIndex - 1) % MAX_IOU_PORT_DEVICES;

  switch (Bifurcation) {
    case PORT_LINK_WIDTH_x16:
      //
      // All lanes are configurable for the x16 port
      //
      Result = TRUE;
      break;
    case PORT_LINK_WIDTH_x8:
      //
      // Only Gen5 ports A and E or Gen4 ports H and D can be configured
      // Gen5 Port A [0,1,2,3,4,5,6,7]
      // Gen5 Port E [8,9,10,11,12,13,14,15]
      // or
      // Gen4 Port D [0,1,2,3,4,5,6,7]
      // Gen4 Port H [8,9,10,11,12,13,14,15]
      //
      if ((Port == IioPortA && (LaneBitmap & 0xFF) != 0) ||
          (Port == IioPortE && (LaneBitmap & 0xFF00) != 0) ||
          (Port == IioPortD && (LaneBitmap & 0xFF) != 0) ||
          (Port == IioPortH && (LaneBitmap & 0xFF00) != 0)) {
        Result = TRUE;
      }
      break;
    case PORT_LINK_WIDTH_x4:
      //
      // Gen5 Ports A, B, C and D or Gen4 ports H, F, D and B can be configured
      // Gen5 Port A [0,1,2,3]
      // Gen5 Port C [4,5,6,7]
      // Gen5 Port E [8,9,10,11]
      // Gen5 Port G [12,13,14,15]
      // or
      // Gen4 Port B [0,1,2,3]
      // Gen4 Port D [4,5,6,7]
      // Gen4 Port F [8,9,10,11]
      // Gen4 Port H [12,13,14,15]
      //
      if ((Port == IioPortA && (LaneBitmap & 0xF) != 0) ||
          (Port == IioPortC && (LaneBitmap & 0xF0) != 0) ||
          (Port == IioPortE && (LaneBitmap & 0xF00) != 0) ||
          (Port == IioPortG && (LaneBitmap & 0xF000) != 0) ||
          (Port == IioPortB && (LaneBitmap & 0xF) != 0) ||
          (Port == IioPortD && (LaneBitmap & 0xF0) != 0) ||
          (Port == IioPortF && (LaneBitmap & 0xF00) != 0) ||
          (Port == IioPortH && (LaneBitmap & 0xF000) != 0)) {
        Result = TRUE;
      }
      break;
    case PORT_LINK_WIDTH_x2:
      //
      // Ports A, B, C, D, E, F, G and H can be configured
      // Gen5 Port A [0,1]
      // Gen4 Port B [2,3]
      // Gen5 Port C [4,5]
      // Gen4 Port D [6,7]
      // Gen5 Port E [8,9]
      // Gen4 Port F [10,11]
      // Gen5 Port G [12,13]
      // Gen4 Port H [14,15]
      //
      if ((Port == IioPortA && (LaneBitmap & 0x3) != 0) ||
          (Port == IioPortB && (LaneBitmap & 0xC) != 0) ||
          (Port == IioPortC && (LaneBitmap & 0x30) != 0) ||
          (Port == IioPortD && (LaneBitmap & 0xC0) != 0) ||
          (Port == IioPortE && (LaneBitmap & 0x300) != 0) ||
          (Port == IioPortF && (LaneBitmap & 0xC00) != 0) ||
          (Port == IioPortG && (LaneBitmap & 0x3000) != 0) ||
          (Port == IioPortH && (LaneBitmap & 0xC000) != 0)) {
        Result = TRUE;
      }
      break;
    default:
      break;
  }
  return Result;
}

/**
  Function to get the recipe entry phy type.

  @param[in] RecipeTable  - IIO EV recipe entry table.

  @return Returns the recipe entry phy type.
**/
IIO_PHY_TYPE
GetRecipeEntryPhyType (
  IN  EV_RECIPE_ENTRY         RecipeTable
  )
{
  if ((RecipeTable.RegisterAddress >> PORT_ID_SHL) == HSPHY_PORT_ID_START) {
    return HIGH_SPEED_PHY;
  } else if ((RecipeTable.RegisterAddress >> PORT_ID_SHL) == FBLP_PORT_ID_START) {
    return FLEX_BUS_LOG_PHY;
  } else {
    return MAX_IIO_PHY_TYPE;
  }
}

/**
   Function to decompress IIO Phy recipe table.

   @param[in out] IN OUT DstBufferSize      - Destination Buffer size.
   @param[in out] IN OUT VOID               - Store IIO Phy recipe table.
   @param[in] IN IioPhyRecipeSize           - IIO Phy recipe table size.
   @param[in] IN IioEvRecipeTablePtr        - EV recipe pointer

   @retval Status
**/
EFI_STATUS
DecompressIioPhyRecipe (
  IN OUT UINT32                  *DstBufferSize,
  IN OUT VOID                    **DstBuffer,
  IN UINT32                      IioPhyRecipeSize,
  IN EV_RECIPE_ENTRY             *IioEvRecipeTablePtr
)
{
  UINT8       *ScratchBuffer = NULL;
  UINT32      ScratchBufferSize = 0;
  EFI_STATUS  Status = EFI_SUCCESS;

  Status = UefiDecompressGetInfo (
              IioEvRecipeTablePtr,
              IioPhyRecipeSize,
              DstBufferSize,
              &ScratchBufferSize
            );
  if (Status != EFI_SUCCESS) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Allocate scratch buffer
  //
  if (0 != ScratchBufferSize) {
    ScratchBuffer = AllocatePages (EFI_SIZE_TO_PAGES (ScratchBufferSize));
  }
  if (NULL == ScratchBuffer) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Allocate destination buffer
  //
  *DstBuffer = AllocatePages (EFI_SIZE_TO_PAGES (*DstBufferSize));
  if (NULL == *DstBuffer) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Call decompress function
  //
  Status = UefiDecompress (
              IioEvRecipeTablePtr,
              *DstBuffer,
              ScratchBuffer
              );
  if (Status != EFI_SUCCESS) {
    return EFI_INVALID_PARAMETER;
  }

  if (ScratchBuffer != NULL) {
    FreePages (ScratchBuffer, EFI_SIZE_TO_PAGES (ScratchBufferSize));
  }

  return EFI_SUCCESS;
}

/**
   Decompress the IIO PHY recipe table.

   @param[in] RecipeTableSize       - The compressed recipe table size.
   @param[in] RecipeHdrTablePtr     - EV recipe table header pointer
   @param[in] RecipeTablePtr        - EV recipe table pointer
   @param[in out] DstBuffer         - EV recipe decompressed table pointer
   @param[in out] DstBufferSize     - The decompressed Buffer size.

   @retval Status from DecompressIioPhyRecipe and EFI_SUCCESS for an empty table.
**/
EFI_STATUS
IioGetRecipeTable (
  IN  UINT32                  RecipeTableSize,
  IN  EV_RECIPE_HEADER        *RecipeHdrTablePtr,
  IN  EV_RECIPE_ENTRY         *RecipeTablePtr,
  IN  OUT EV_RECIPE_ENTRY     **DstBuffer,
  IN  OUT UINT32              *DstBufferSize
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;

  if (RecipeHdrTablePtr->NumberEntries != 0 &&
      RecipeHdrTablePtr->NumberEntries != RecipeTableSize / sizeof (EV_RECIPE_ENTRY)) {

    IIO_D_PCILOG ("Recipy decompressing...\n");
    Status = DecompressIioPhyRecipe (DstBufferSize, (VOID **) DstBuffer, RecipeTableSize, RecipeTablePtr);
    if (EFI_ERROR (Status)) {
      IIO_D_PCIERR ("Recipy decompress failed (%r)\n", Status);
      ASSERT (FALSE);
      return Status;
    }
  }

  return Status;
}

/**
  This function will write the recipe data to register.

  @param[in]     Socket                Ssocket number.
  @param[in]     PortIndex             IIO port number in socket.
  @param[in]     CsrOffset             The regster address.
  @param[in]     RegSize               Register Size.
  @param[in]     BitMask               The value to AND with the read value from register.
  @param[in]     OrData                The value to OR with the result of the AND operation.

**/
VOID
WriteRecipeData (
  IN  UINT8             Socket,
  IN  UINT8             PortIndex,
  IN  CSR_OFFSET        CsrOffset,
  IN  UINT8             RegSize,
  IN  UINT32            BitMask,
  IN  UINT32            OrData
)
{
  UINT8                 Data8;
  UINT16                Data16;
  UINT32                Data32;

  switch (RegSize) {
  case 1:
    CsrOffset.Bits.size = 0;
    Data8 = IioReadCpuCsr8 (Socket, PortIndex, CsrOffset.Data);
    Data8 &= BitMask;
    Data8 |= OrData;
    IioWriteCpuCsr8 (Socket, PortIndex, CsrOffset.Data, Data8);
    break;
  case 2:
    CsrOffset.Bits.size = 1;
    Data16 = IioReadCpuCsr16 (Socket, PortIndex, CsrOffset.Data);
    Data16 &= BitMask;
    Data16 |= OrData;
    IioWriteCpuCsr16 (Socket, PortIndex, CsrOffset.Data, Data16);
    break;
  case 4:
    CsrOffset.Bits.size = 2;
    Data32 = IioReadCpuCsr32 (Socket, PortIndex, CsrOffset.Data);
    Data32 &= BitMask;
    Data32 |= OrData;
    IioWriteCpuCsr32 (Socket, PortIndex, CsrOffset.Data, Data32);
    break;
  default:
    IIO_D_PCIERR ("Logic PHY register size[%d] not supported.\n", RegSize);
    break;
  }
}

/**
  This function will do the logical PHY recipe programming.

  @param[in]     AccessType            The register access type.
  @param[in]     ConfigType            The register config type.
  @param[in]     Socket                Ssocket number.
  @param[in]     Stack                 IIO Stack number.
  @param[in]     RegSize               Register Size.
  @param[in]     RegOffset             Register Offset.
  @param[in]     BitMask               The value to AND with the read value from register.
  @param[in]     OrData                The value to OR with the result of the AND operation.
  @param[in]     LaneBitmap            Lane Bit map to select which lanes can be configured.
  @param[in]     BitRates              Specify operating rates which can be configured.

  @retval        EFI_SUCCESS           The programming is successful.
**/
EFI_STATUS
EFIAPI
LogphyRecipeProgramming (
  IN  UINT8             AccessType,
  IN  UINT16            ConfigType,
  IN  UINT8             Socket,
  IN  UINT8             Stack,
  IN  UINT8             RegSize,
  IN  UINT32            RegOffset,
  IN  UINT32            BitMask,
  IN  UINT32            OrData,
  IN  UINT32            LaneBitmap,
  IN  UINT32            BitRates
)
{
  UINT8                 MaxPortNumPerStack;
  UINT8                 PortIndex;
  UINT8                 PortOffset;
  CSR_OFFSET            CsrOffset;
  IIO_GLOBALS           *IioGlobalData;
  EFI_STATUS            Status;
  IIO_PORT_GEN          PortController;

  if (ConfigType & (1 << DMI_CFG)) {
    CsrOffset.Data = LANEEQCTL_0_IIO_PCIEDMI_REG;
    if (AccessType == TYPE_CSR) {
      CsrOffset.Bits.boxtype = 0x0D;
      CsrOffset.Bits.funcblk = 0x00;
      CsrOffset.Bits.offset = (RegOffset & 0x0FFF);
    } else {
      CsrOffset.Bits.boxtype = 0x0E;
      CsrOffset.Bits.funcblk = 0x02;
      CsrOffset.Bits.offset = (RegOffset & 0xFFFF);
    }
    WriteRecipeData (Socket, 0, CsrOffset, RegSize, BitMask, OrData);

  } else if (ConfigType & (1 << PCIE_CFG)) {
    MaxPortNumPerStack = GetMaxPortNumPerStack (Stack);
    Status = GetIioGlobalData (&IioGlobalData);
    for (PortOffset = 0; PortOffset < MaxPortNumPerStack; PortOffset++) {
      PortIndex = GetPortIndexbyStack (Stack, PortOffset);
      //
      // Skip programming recipe entry if this is not the ports max supported bitrate.
      //
      PortController = IioPortControllerGen (IioGlobalData, Socket, PortIndex);
      if (!((((BitRates & (1 << PCIE_RATE_160GT)) != 0) && PortController == IIO_GEN4) ||
            (((BitRates & (1 << PCIE_RATE_320GT)) != 0) && PortController == IIO_GEN5))) {
        continue;
      }
      //
      // Check if the recipe is valid for port lane.
      //
      if ((((PortOffset == IioPortA) && (LaneBitmap & 0xFFFF) != 0) ||
          ((PortOffset == IioPortC) && (LaneBitmap & 0x000F) != 0) ||
          ((PortOffset == IioPortE) && (LaneBitmap & 0x00FF) != 0) ||
          ((PortOffset == IioPortG) && (LaneBitmap & 0x000F) != 0) ||
          ((PortOffset == IioPortH) && (LaneBitmap & 0xFFFF) != 0) ||
          ((PortOffset == IioPortF) && (LaneBitmap & 0x000F) != 0) ||
          ((PortOffset == IioPortD) && (LaneBitmap & 0x00FF) != 0) ||
          ((PortOffset == IioPortB) && (LaneBitmap & 0x000F) != 0)) &&
           IioVData_ConfigurePciePort (IioGlobalData, Socket, PortIndex)) {

        CsrOffset.Data = LANEEQCTL_0_IIO_PCIE_G5_REG;
        if (AccessType == TYPE_CSR) {
          CsrOffset.Bits.offset = (RegOffset & 0x0FFF);
        } else {
          CsrOffset.Bits.boxtype = 0x0E;
          CsrOffset.Bits.funcblk = 0x02;
          CsrOffset.Bits.offset = (RegOffset & 0xFFFF);
        }
        WriteRecipeData (Socket, PortIndex, CsrOffset, RegSize, BitMask, OrData);
      } // port lanes
    } // end for loop
  } else {
    IIO_D_PCIERR ("Recipe config type[%d] not supported.\n", ConfigType);
  }
  return EFI_SUCCESS;
}

/**
   This function loops through each entry in the IIO PHY Recipe

   @param[in] Iio                   - IIO number
   @param[in] Iio                   - Stack number
   @param[in] RecipeHdrTablePtr     - EV recipe table header pointer
   @param[in] RecipeTablePtr        - EV recipe table pointer

   @return Status.
**/
EFI_STATUS
RecipeProgramming (
  IN  UINT8                   IioIndex,
  IN  UINT8                   StackIndex,
  IN  EV_RECIPE_HEADER        *RecipeHdrTablePtr,
  IN  EV_RECIPE_ENTRY         *RecipeTablePtr
  )
{
  UINT32      TableIndex;
  UINT32      NumEntries;
  UINT32      RegOffset;
  UINT16      ConfigType;
  UINT8       RegSize;
  UINT8       AccessType;
  UINT32      FieldMask;
  UINT32      DataEvRecipe;
  UINT32      LaneBitmap;
  UINT32      BitRates;
  UINT8       EvRevisionBit;

  if ((RecipeTablePtr == NULL) || (RecipeHdrTablePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  EvRevisionBit = EvAutoRecipeGetRevBit ();
  //
  // Verify the handle before parsing the EVRecipe entries
  //
  if (EvRevisionBit == EV_RECIPE_INVALID_VALUE) {
    IIO_D_PCILOG ("Invalid EV revision detected. Aborting EV Recipe programming.\n");
    return EFI_INVALID_PARAMETER;
  }

  IIO_D_PCILOG ("Socket[%d] Stack[%d] Program RX recipe values START...\n", IioIndex, StackIndex);

  IIO_D_PCILOG ("Program recipe revision %a (%a, len %d)\n",
                RecipeHdrTablePtr->Revision, RecipeHdrTablePtr->HelperString, RecipeHdrTablePtr->NumberEntries);

  // Loop though all the recipe entries in IiPciAddressoEvRecipeTablePtr and apply the recipe if applicable
  IIO_D_PHYDBG ("TableIndex | AccessType | ConfigType | RegSize |  RegOffset |  FieldMask | DataEvRecipe |  LaneBitmap |\n");
  NumEntries = RecipeHdrTablePtr->NumberEntries;
  for (TableIndex = 0; TableIndex < NumEntries; TableIndex++) {
    if (!IsValidRecipeEntry (IioIndex, EvRevisionBit, RecipeTablePtr[TableIndex])) {
      continue;
    }

    //
    //  Gather the relevant information from the Recipe Table
    //
    ConfigType    = RecipeTablePtr[TableIndex].Config;
    RegSize       = RecipeTablePtr[TableIndex].RegisterSize;
    RegOffset     = RecipeTablePtr[TableIndex].RegisterAddress & 0xFFFF;
    FieldMask     = RecipeTablePtr[TableIndex].FieldMask;
    DataEvRecipe  = RecipeTablePtr[TableIndex].Data;
    AccessType    = RecipeTablePtr[TableIndex].AccessType;
    LaneBitmap    = RecipeTablePtr[TableIndex].LaneBitmap & MAX_PORT_LINK_WIDTH_BIT_MAP;
    BitRates      = RecipeTablePtr[TableIndex].BitRates;

    IIO_D_PHYDBG ("0x%08X | %10a | %10a |    0x%02X | 0x%08X | 0x%08X |   0x%08X |  0x%08X |\n",
      TableIndex,
      AccessType == TYPE_MEM_BAR ? "MEM" : AccessType == TYPE_CSR_SB ? "SB" : AccessType == TYPE_CSR ? "CSR" : "NA",
      ConfigType == (1 << DMI_CFG) ? "DMI" : ConfigType == (1 << PCIE_CFG) ? "PCIe" : "NA",
      RegSize, RegOffset, FieldMask, DataEvRecipe, LaneBitmap);
    //
    // Access the register according to access type
    //
    switch (AccessType) {
    case TYPE_CSR_SB:
      HsphyRecipeProgramming (RECIPE_PCIE_TYPE, IioIndex, StackIndex, 0, RegOffset, FieldMask, DataEvRecipe);
      HsphyRecipeProgramming (RECIPE_PCIE_TYPE, IioIndex, StackIndex, 1, RegOffset, FieldMask, DataEvRecipe);
      break;
    case TYPE_CSR:
    case TYPE_MEM_BAR:
      LogphyRecipeProgramming (AccessType, ConfigType, IioIndex, StackIndex, RegSize, RegOffset, FieldMask,
        DataEvRecipe, LaneBitmap, BitRates);
      break;
    default:
      IIO_D_PCIERR ("Access type %d in recipy row[%d] not supported\n", AccessType, TableIndex);
      break;
    }
  } //for (for each TableIndex)

  IIO_D_PCILOG ("[%d] Program RX recipe values END\n", IioIndex);
  return EFI_SUCCESS;
}

/**
   This function applies the IIO PHY Recipe.

   @param[in] IioGlobalData   - IioGlobalData pointer
   @param[in] IioIndex        - IIO number (socket number)

   @returns Status
**/
EFI_STATUS
IioPhyRecipeInit (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                   IioIndex
  )
{
  EV_RECIPE_HEADER    *RecipeHdrTablePtr;
  EV_RECIPE_ENTRY     *RecipeTablePtr;
  EV_RECIPE_ENTRY     *IioDmiPhyRecipeTablePtr = NULL;
  EV_RECIPE_ENTRY     *IioPciePhyRecipeTablePtr = NULL;
  UINT8               MaxIouNumPerSocket;
  UINT8               StackIndex;
  UINT32              DmiBufferSize = 0;
  UINT32              PcieBufferSize = 0;

  IioGetRecipeTable (IioDmiphyRecipeSize, &IioDmiphyRecipeVer, IioDmiphyRecipe, &IioDmiPhyRecipeTablePtr, &DmiBufferSize);
  IioGetRecipeTable (IioPciephyRecipeSize, &IioPciephyRecipeVer, IioPciephyRecipe, &IioPciePhyRecipeTablePtr, &PcieBufferSize);

  MaxIouNumPerSocket = GetMaxIouNumPerSocket ();
  for (StackIndex = 0; StackIndex < (MaxIouNumPerSocket + 1); StackIndex++) {
    if (!IfStackPresent (IioIndex, StackIndex)) {
      continue;
    }
    if (IsDmiStack (StackIndex)) { //SPR_TODO: Maybe need to conside DMI as PCIe case
      RecipeHdrTablePtr = &IioDmiphyRecipeVer;
      RecipeTablePtr = IioDmiPhyRecipeTablePtr;
    } else {
      RecipeHdrTablePtr = &IioPciephyRecipeVer;
      RecipeTablePtr = IioPciePhyRecipeTablePtr;
    }

    RecipeProgramming (IioIndex, StackIndex, RecipeHdrTablePtr, RecipeTablePtr);
  }

  if (IioDmiPhyRecipeTablePtr != NULL) {
    FreePages (IioDmiPhyRecipeTablePtr, EFI_SIZE_TO_PAGES (DmiBufferSize));
  }

  if (IioPciePhyRecipeTablePtr != NULL) {
    FreePages (IioPciePhyRecipeTablePtr, EFI_SIZE_TO_PAGES (PcieBufferSize));
  }

  return IIO_SUCCESS;
}
