/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation. <BR>

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

#include <RcRegs.h>
#include <Library/MemoryCoreLib.h>
#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>
#include <KtiMisc.h>
#include <Chip/Include/EvAutoRecipe.h>
#include <UncoreCommonIncludes.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/DebugLib.h>
#include <Library/KtiApi.h>
#include <KtiSetupDefinitions.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/ChaLib.h>
#include <Guid/UboxIpInterface.h>
#include <Library/Ms2IdiLib.h>
#include <Library/PcuIpLib.h>
#include <Guid/KtiIpInterface.h>
#include <Guid/FpgaIpInterface.h>
#include <Library/CsrAccessLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/IoAccessLib.h>
#include <Library/UefiDecompressLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Chip/Include/KtiEvRecipeTable_ICX.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/EvAutoRecipeLib.h>
#include "MultiSocketInternal.h"

#ifdef RAS_SUPPORT
#pragma message( " X64 Compiling " __FILE__ )

#include <Protocol/QuiesceProtocol.h>

#define MAX_REG_OFFSET             0x200
#define MAX_BUF_PER_FUN            MAX_REG_OFFSET
#define MAX_BUF_PER_DEV            (8 * MAX_BUF_PER_FUN)
#define MAX_BUF_PER_BUS            (32 * MAX_BUF_PER_DEV)
#define MAX_SHADOW_BUS             4
#define MAX_BUF_PER_CPU            (MAX_SHADOW_BUS * MAX_BUF_PER_BUS) // creating shadow buffer only for 4 stacks

// Every bit keeps track of a BYTE
#define MAX_FLAG_BUF_PER_CPU       (MAX_BUF_PER_CPU / 8)

#define CPU_SHADOW_BUF_SIZE        (MAX_SOCKET * MAX_BUF_PER_CPU)
#define CPU_SHADOW_FLAG_SIZE       (MAX_SOCKET * MAX_FLAG_BUF_PER_CPU)

UINT8                       *mCpuShadowFlag = NULL;  // 16KB per CPU, to keep track of every byte of the shadow buffer
UINT8                       *mCpuShadowBuf  = NULL;  // 128KB per CPU, Byte/Word/Dword accesses
QUIESCE_DATA_TABLE_ENTRY    *mNextEntry     = NULL;  // Points to first available slot in the quiesce buffer; used as semaphore as well
BOOLEAN                     mDirectCommit   = TRUE;  // Flag to control whether register writes should be committed immediately

#else
#pragma message( " IA32 Compiling " __FILE__ )
#endif // RAS_SUPPORT

#define KTI_LBC_TIMEOUT   50000    // 50 ms

//
// ICX MS2IOSF source locations (North Cap versus South Cap)
//
STATIC UINT8 IcxMs2iosfSourceLocation[MAX_IIO_STACK] = {
  SOUTH_CAP_MS2IOSF,   // Ms2iosf0
  NORTH_CAP_MS2IOSF,   // Ms2iosf1
  NORTH_CAP_MS2IOSF,   // Ms2iosf2
  NORTH_CAP_MS2IOSF,   // Ms2iosf3
  NORTH_CAP_MS2IOSF,   // Ms2iosf4
  SOUTH_CAP_MS2IOSF    // Ms2iosf5
};

/**
  This function loops through each entry in the KtiEvRecipeTablePtr of KtiEvAutoRecipeEntry.h file
  and updates the registers based on processor type, subtype, bitrates and stepping conditions.
  This function is called for all valid ports of all valid sockets.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param Socket            - SocId we are looking up
  @param LinkIndex         - Link# on socket

  @retval Status - return KTI_SUCCESS on successful programming

**/
KTI_STATUS
KtiEvAutoRecipe (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Socket,
  UINT8                      LinkIndex
  )
{
  UINT32            RegAddr32, Data = 0, OverriddenData = 0;
  UINT8             Bus, PortDevice, Function, UnitId, LinkIndexForLbc;
  UINT16            Offset;
  UINT8             WinSubType, WinRegAddr;
  UINT32            TableIndex, NumEntries;
  BOOLEAN           CpuTypeCmp, SkuTypeCmp, SteppingCmp, SocketTypeCmp, PortTypeCmp, LinkSpeedCmp, ConfigTypeCmp;
  BOOLEAN           PrintDebug = FALSE;
  UINT32            LinkSpeed;
  UINT32            KtiEvRecipeTableSize = 0;
  CHAR8             TmpArea[HELP_STR_CHARS + 1];
  UINT8             CpuType;
  UINT8             EvRevisionBit;
  UINT8             *DstBuffer;
  UINT8             *ScratchBuffer;
  UINT32            DstBufferSize;
  UINT32            ScratchBufferSize;
  EFI_STATUS        Status;

  EV_RECIPE_HEADER *KtiEvHdrTablePtr = NULL;
  EV_RECIPE_ENTRY  *KtiEvRecipeTablePtr = NULL;

  CpuType     = KtiInternalGlobal->CpuType;

  KTI_ASSERT (IsIcxFamilyCpu (CpuType), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_4);

  //
  // Point to the right EV Recipe Table
  //
  KtiEvHdrTablePtr = (EV_RECIPE_HEADER*) &KtiEvHdrTable_ICX;

  //
  // Verify the signature, before parsing the EVRecipe entries
  //
  if (KtiEvHdrTablePtr->Signature != EV_AUTO_RECIPE_SIGNATURE) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Invalid or incorrect signature found. Aborting EV Recipe programming ."));
    KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_5);
  }

  PrintDebug = (KtiEvHdrTablePtr->DebugMode == 0) ? FALSE : TRUE;

  EvRevisionBit = EvAutoRecipeGetRevBit ();
  //
  // Verify the handle before parsing the EVRecipe entries
  //
  if (EvRevisionBit == EV_RECIPE_INVALID_VALUE) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Invalid or EV revision detected. Aborting EV Recipe programming ."));
    KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_5);
  }

  //
  // Dump the EV header contents, if debug is enabled
  //
  if (PrintDebug) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  ---- EV Header Dump <start> ----"));
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Signature : 0x%08X", KtiEvHdrTablePtr->Signature));

    //
    // Copy to a local buffer with max limit control before printing
    //
    ZeroMem ((UINT8*) TmpArea, HELP_STR_CHARS + 1);
    CopyMem ((UINT8*)TmpArea, (UINT8*)KtiEvHdrTablePtr->Revision, REV_STR_CHARS);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  EV Recipe Version : %a", TmpArea));
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Entries count : %d", KtiEvHdrTablePtr->NumberEntries));

    //
    // Copy to a local buffer with max limit control before printing
    //
    ZeroMem ((UINT8*) TmpArea, HELP_STR_CHARS + 1);
    CopyMem ((UINT8*)TmpArea, (UINT8*)KtiEvHdrTablePtr->HelperString, HELP_STR_CHARS);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Info : %a", TmpArea));
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  ---- EV Header Dump <end> ----"));
  }

  //
  // Get the bus number for uncore bus
  //
  Bus = KTIVAR->CpuInfo[Socket].CpuRes.StackRes[UBOX_STACK].BusBase;  //GetStackBus (Socket, IIO_PSTACK2);

  Status = UefiDecompressGetInfo (
               &KtiEvRecipeTable_ICX,
               sizeof(KtiEvRecipeTable_ICX),
               &DstBufferSize,
               &ScratchBufferSize
             );
  //
  // Allocate scratch buffer
  //
  ScratchBuffer = AllocatePages (EFI_SIZE_TO_PAGES (ScratchBufferSize));
  if (ScratchBuffer == NULL) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_ERROR, "\nFatal Error - Allocate memory for decompression scratch buffer failed!"));
    return KTI_FAILURE;
  }

  //
  // Allocate destination buffer
  //
  DstBuffer = AllocatePages (EFI_SIZE_TO_PAGES (DstBufferSize));
  if (DstBuffer == NULL) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_ERROR, "\nFatal Error - Allocate memory for decompression dest buffer failed!"));
    return KTI_FAILURE;
  }

  //
  // Call decompress function
  //
  Status = UefiDecompress (
              &KtiEvRecipeTable_ICX,
              DstBuffer,
              ScratchBuffer
              );

  //
  // Loop though all the recipe entries in KtiEvRecipeTablePtr and apply the recipe if applicable
  //
  KtiEvRecipeTablePtr  = (EV_RECIPE_ENTRY *) DstBuffer;
  KtiEvRecipeTableSize = DstBufferSize;
  NumEntries = KtiEvRecipeTableSize / sizeof (EV_RECIPE_ENTRY);

  for (TableIndex = 0; TableIndex < NumEntries; TableIndex++) {
    if (PrintDebug == TRUE) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Fetching EV Recipe Entry at index %d    [OK]", TableIndex));
    } // Print Debug

    Function = (KtiEvRecipeTablePtr[TableIndex].RegisterAddress >> 12) & 0x7;
    Offset   = (KtiEvRecipeTablePtr[TableIndex].RegisterAddress) & 0xfff;

    //
    // Mapping between the port and device number
    //
    PortDevice = LinkIndex + 2;

    //
    // Verify if the Processor Type, SubType (SKU) and Stepping match with the EV Recipe Entry
    //
    CpuTypeCmp    = (KtiEvRecipeTablePtr[TableIndex].ProcessorType == ICX_SOCKET) ? TRUE : FALSE;
    SkuTypeCmp    = EvAutoRecipeiIsValidSkuType (KtiEvRecipeTablePtr[TableIndex].Skus);
    SteppingCmp   = EvAutoRecipeIsValidRevision (EvRevisionBit, KtiEvRecipeTablePtr[TableIndex].Steppings);
    SocketTypeCmp = EvAutoRecipeIsValidSocket (Socket, KtiEvRecipeTablePtr[TableIndex].SocBitmap);
    PortTypeCmp   = KtiIsValidPort (LinkIndex, KtiEvRecipeTablePtr[TableIndex].PortBitmap);
    ConfigTypeCmp = KtiIsValidConfig (KtiInternalGlobal, Socket, LinkIndex, KtiEvRecipeTablePtr[TableIndex].Config);


    //
    // Verify the link speed
    //
    LinkSpeedCmp = FALSE;
    LinkSpeed = KTIVAR->OutPerLinkSpeed[Socket][LinkIndex];

    if (KtiEvRecipeTablePtr[TableIndex].BitRates != WILDCARD_32) {
      if ((1 << LinkSpeed) & KtiEvRecipeTablePtr[TableIndex].BitRates){
         LinkSpeedCmp = TRUE;
      }
    } else {
      LinkSpeedCmp = TRUE;
    } // BitRates test

    //
    // Print verbose info on reason for this entry to be applied or not
    //
    if (PrintDebug == TRUE) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  TableIndex %d : CpuTypeCmp=%d, SkuTypeCmp=%d, SteppingCmp=%d, LinkSpeedCmp=%d, SocketTypeCmp=%d, PortTypeCmp=%d, ConfigTypeCmp=%d",
                                TableIndex, CpuTypeCmp, SkuTypeCmp, SteppingCmp, LinkSpeedCmp, SocketTypeCmp, PortTypeCmp, ConfigTypeCmp));
    }

    //
    // Process the EV Recipe Entry only when its cpu type and stepping match the current processor's type and stepping
    //
    if (CpuTypeCmp && SkuTypeCmp && SteppingCmp && LinkSpeedCmp && SocketTypeCmp && PortTypeCmp && ConfigTypeCmp) {
      //
      // Check if this is CSR and IAR access. Program based on the type of access
      //
      if (KtiEvRecipeTablePtr[TableIndex].AccessType == TYPE_CSR) {
        //
        // Compute the register address
        //
        RegAddr32 = (KTI_HOST_OUT_PTR->mmCfgBaseL[Socket] | PCIE_REG_ADDR (Bus, PortDevice, Function, Offset));

        //
        // Fetch the register value
        //
        Data = 0;
        //
        // Check register size and read data
        //
        switch (KtiEvRecipeTablePtr[TableIndex].RegisterSize) {
          case sizeof (UINT32):
            Data = *(volatile UINT32 *) (UINTN) RegAddr32;
            break;
          case sizeof (UINT16):
            Data = *(volatile UINT16 *) (UINTN) RegAddr32;
            break;
          case sizeof (UINT8):
            Data = *(volatile UINT8 *) (UINTN) RegAddr32;
            break;
          default:
            FatalError ( ERR_INVALID_REG_ACCESS, ERR_INVALID_READ_REG_SIZE);
            break;
        } // switch case
        if (PrintDebug == TRUE) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  CSR Read access for TableIndex=%d, RegAddr32=0x%08X, Data=0x%08X.\n", TableIndex, RegAddr32, Data));
        } // Print Debug

        // Update the data with the values from the EV Recipe
        Data = ((Data & KtiEvRecipeTablePtr[TableIndex].FieldMask) | KtiEvRecipeTablePtr[TableIndex].Data);

        // Write the data
        switch (KtiEvRecipeTablePtr[TableIndex].RegisterSize) {
          case sizeof (UINT32):
            *(volatile UINT32 *) (UINTN) RegAddr32 = Data;
            break;

          case sizeof (UINT16):
            *(volatile UINT16 *) (UINTN) RegAddr32 = (UINT16) Data;
            break;

          case sizeof (UINT8):
            *(volatile UINT8 *) (UINTN) RegAddr32 = (UINT8) Data;
            break;

          default:
            FatalError ( ERR_INVALID_REG_ACCESS, ERR_INVALID_WRITE_REG_SIZE);
            break;
        } // switch case

        if (PrintDebug == TRUE) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  CSR Write access for TableIndex=%d, RegAddr32=0x%08X, Data=0x%08X.\n", TableIndex, RegAddr32, Data));
        } // Print Debug
      } else if (KtiEvRecipeTablePtr[TableIndex].AccessType == TYPE_KTI_IAR) {

        RegAddr32 = KtiEvRecipeTablePtr[TableIndex].RegisterAddress;
        WinSubType = WINDOW1_ACCESS_SUB_TYPE (RegAddr32);
        WinRegAddr = WINDOW1_ACCESS_REG_ADDR (RegAddr32);

        if (WinSubType == WINDOW1_IAR_IOVB_BCAST) {
          //
          // For IOVB broadcast, can read any lane as an intial value to modify, so read lane 0
          // After cold reset, all lanes should have the same values
          //

          Data = 0;
          Data = KtiReadIar (Socket, LinkIndex, 0, WinRegAddr);
          if (PrintDebug == TRUE) {
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  IAR Read Access for TableIndex=%d, RegAddr32=0x%08X, WinSubType=0x%02X, WinRegAddr=0x%02X, Data=0x%02X\n", TableIndex, RegAddr32, WinSubType, WinRegAddr, Data));
          } // Print Debug

          //
          // Update the data with the values from the EV Recipe or overridden values
          //
          if (OverrideUniphyValues (Socket, LinkIndex, KtiEvRecipeTablePtr[TableIndex].AccessType, RegAddr32, KtiEvRecipeTablePtr[TableIndex].FieldMask, &OverriddenData)) {
            if (PrintDebug == TRUE) {
              KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Match with Overrides Table.Applying OverriddenData=0x%08X instead of tabular data.\n", OverriddenData));
            } // Print Debug
            Data = ((Data & KtiEvRecipeTablePtr[TableIndex].FieldMask) | OverriddenData);
          } else {
            Data = ((Data & KtiEvRecipeTablePtr[TableIndex].FieldMask) | KtiEvRecipeTablePtr[TableIndex].Data);
          } // OverrideUniphyValues check

          //
          // Write the IOVB registers back
          //
          KtiWriteIar (Socket, LinkIndex, KTILIB_IAR_BCAST, WinRegAddr, (UINT8)Data);

          if (PrintDebug == TRUE) {
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  IAR Write Access for TableIndex=%d, RegAddr32=0x%08X, WinSubType=0x%02X, WinRegAddr=0x%02X, Data=0x%02X\n", TableIndex, RegAddr32, WinSubType, WinRegAddr, Data));
          } // Print Debug
        } else if (WinSubType == WINDOW1_IAR_IOVB) {
          //
          // Loop through the lane bitmask
          //
          for (UnitId = 0; UnitId < MAX_KTI_LANES; UnitId++) {
            if (KtiEvRecipeTablePtr[TableIndex].LaneBitmap & (1 << UnitId)) {
              //
              // Read the IOVB registers first into Data
              //

              Data = 0;
              Data = KtiReadIar (Socket, LinkIndex, UnitId, WinRegAddr);

              if (PrintDebug == TRUE) {
                KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  IAR Read Access for TableIndex=%d, RegAddr32=0x%08X, WinSubType=0x%02X, WinRegAddr=0x%02X, Data=0x%02X\n", TableIndex, RegAddr32, WinSubType, WinRegAddr, Data));
              } // Print Debug

              //
              // Update the data with the values from the EV Recipe or overridden values
              //
              if (OverrideUniphyValues (Socket, LinkIndex, KtiEvRecipeTablePtr[TableIndex].AccessType, RegAddr32, KtiEvRecipeTablePtr[TableIndex].FieldMask, &OverriddenData)) {
                if (PrintDebug == TRUE) {
                  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Match with Overrides Table.Applying OverriddenData=0x%08X instead of tabular data.\n", OverriddenData));
                } // Print Debug
                Data = ((Data & KtiEvRecipeTablePtr[TableIndex].FieldMask) | OverriddenData);
              } else {
                Data = ((Data & KtiEvRecipeTablePtr[TableIndex].FieldMask) | KtiEvRecipeTablePtr[TableIndex].Data);
              } // OverrideUniphyValues check

              //
              // Write the IOVB registers back
              //
              KtiWriteIar (Socket, LinkIndex, UnitId, WinRegAddr, (UINT8)Data);

              if (PrintDebug == TRUE) {
                KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  IAR Write Access for TableIndex=%d, RegAddr32=0x%08X, WinSubType=0x%02X, WinRegAddr=0x%02X, Data=0x%02X\n", TableIndex, RegAddr32, WinSubType, WinRegAddr, Data));
              } // Print Debug
            } // Valid UnitId
          } // For UnitId
        } else {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Aborting. Don\'t know to apply EVEntry at TableIndex=%d, sub-type=%u, Unknown sub-type.", TableIndex, WinSubType));
          FatalError ( ERR_INVALID_REG_ACCESS, ERR_UNKNOWN_REG_TYPE);
        } // WinSubType check
      } else if (KtiEvRecipeTablePtr[TableIndex].AccessType == TYPE_LBC_AFE || KtiEvRecipeTablePtr[TableIndex].AccessType == TYPE_LBC_COM) {
        //
        // LBC COM is shared between ports; ports 0 and 1 use instance 0, port 2 uses instance 2; instance 1 is invalid
        //

        LinkIndexForLbc = LinkIndex;
        //
        // Perform a blind LBC write
        //
        KtiWriteLbc (Socket,
                     LinkIndexForLbc,
                     KtiEvRecipeTablePtr[TableIndex].AccessType,
                     KtiEvRecipeTablePtr[TableIndex].LaneBitmap,
                     KtiEvRecipeTablePtr[TableIndex].RegisterAddress,
                     KtiEvRecipeTablePtr[TableIndex].Data
                     );
      } else {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Aborting. Don\'t how know to apply EVEntry at TableIndex=%d, access type=%u, Unknown access type.", TableIndex, KtiEvRecipeTablePtr[TableIndex].AccessType));
        FatalError ( ERR_INVALID_REG_ACCESS, ERR_INVALID_READ_REG_SIZE);
      } // AccessType checks for CSR, KTI IAR, and LBC

    } else {
     if (PrintDebug == TRUE) {
       KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Not applying. EV Recipe checks failed for EVEntry at TableIndex=%d", TableIndex));
     } // Print Debug
    } // CpuTypeCmp && SkuTypeCmp && SteppingCmp && LinkSpeedCmp checks
  } // for loop

  if (ScratchBuffer != NULL) {
    FreePages (ScratchBuffer, EFI_SIZE_TO_PAGES (ScratchBufferSize));
  }
  if (DstBuffer != NULL) {
    FreePages (DstBuffer, EFI_SIZE_TO_PAGES (DstBufferSize));
  }

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Socket %u Port %u : UPI EV Recipe v%a programmed\n", Socket, LinkIndex, KtiEvHdrTablePtr->Revision));
  return KTI_SUCCESS;
}

/**
  Routine to setup the config access path from SBSP to the given socket.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Interm            - Immediate parent of the socket in SBSP minimum tree for which the config
                             access path needs to be set from SBSP
  @param Dest              - socket for which the config access path needs to be set from SBSP
  @param DestPort          - Port of the given socket's immediate parent that connects them.

  @retval KTI_SUCCESS

**/
KTI_STATUS
SetupSbspConfigAccessPath (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  TOPOLOGY_TREE_NODE        *Interm,
  TOPOLOGY_TREE_NODE        *Dest,
  UINT8                     DestPort
  )
{
  TOPOLOGY_TREE_NODE  Parent;
  TOPOLOGY_TREE_NODE  Child;
  UINT8               cLocalPort;
  UINT8               pLocalPort = 0, pLocalPort1;
  UINT8               sbspSktId;
  UINT8               PortFieldWidth;
  UINT32              WhrBusDeviceNumber;
  UINT8               SipBusNumber;

  sbspSktId         = KtiInternalGlobal->SbspSoc;
  PortFieldWidth    = 2; // Two bit field to encode link 0, 1 & 2

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n   Setting path between SBSP and %a%u. ", (Dest->Node.SocType == SOCKET_TYPE_FPGA) ? "PGA":"CPU", Dest->Node.SocId));

  //
  // Set the path to "Dest" in all the sockets that exist between SBSP and "Interm"
  //
  Parent      = *Interm;
  Child       = *Dest;
  cLocalPort  = DestPort;

  if (cLocalPort >= MAX_FW_KTI_PORTS) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Invalid local port!\n"));
    return KTI_FAILURE;
  }
  pLocalPort1 = (UINT8) SocketData->Cpu[Interm->Node.SocId].LinkData[cLocalPort].PeerPort;

  while (1) {
    if (Parent.Node.SocId == sbspSktId && Parent.Node.SocType == SOCKET_TYPE_CPU) {
      //
      // In Sbsp, setup Cbo/M2PCIe route table for "Dest" socket; no need to setup HA route table
      // since at this point only Config transactions are active in the system.
      //
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n   In SBSP setting route to %a%u using port %u.\n", (Dest->Node.SocType == SOCKET_TYPE_FPGA) ? "PGA":"CPU", Dest->Node.SocId, cLocalPort));

      ChaKtiBroadcastRouteTableEnable ((UINT8) Parent.Node.SocId, TRUE);

      //
      // Cha RT in SBSP
      //
      ConfigureChaKtiRTForSbspOrRouteToSbsp (
        (UINT8)Parent.Node.SocId,
        KtiInternalGlobal->TotCha[Parent.Node.SocId],
        PortFieldWidth,
        cLocalPort,
        (UINT8)Dest->Node.SocId,
        KtiInternalGlobal->UpiRoutingMode
        );

      UBoxConfigureRouteTablePerPort (
        (UINT8)Parent.Node.SocId,
        0,
        PortFieldWidth,
        cLocalPort,
        (UINT8)Dest->Node.SocId
        );

      //
      // Now that forward CFG-Access path is initialized, detect whether Dest socket is an FPGA
      //
      if (FpgaIpIsNodeFpga ((UINT8) Dest->Node.SocId)) {
        Dest->Node.SocType = SOCKET_TYPE_FPGA;
        KTIVAR->OutKtiFpgaPresent[Dest->Node.SocId] = 1;
        //
        // Socket is an FPGA, set some initial bus numbers for early CSR access.
        // WHR is on FPGA node virtual stack 0, Soft IP is on FPGA virtual stack 1
        //
        WhrBusDeviceNumber = (GetStackBus ((UINT8) Dest->Node.SocId, 0) << 5) | WHR_CFG_DEVICE;
        SipBusNumber = GetStackBus ((UINT8) Dest->Node.SocId, 1);
        FpgaIpSetBusNumbers ((UINT8) Dest->Node.SocId, WhrBusDeviceNumber, SipBusNumber);
      }

      //
      // Only program return route in CPU, FPGA only has one port, and does not have ability to program a route
      //
      if (Dest->Node.SocType == SOCKET_TYPE_CPU) {
        // M2PCIe1 RT in "Dest" socket  (Ubox is behind M2PCIe1)
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n   In CPU%u using port %u to set the M2UPCIe0 route.\n", Dest->Node.SocId, pLocalPort1));
        UBoxConfigureRouteTablePerPort (
          (UINT8)Dest->Node.SocId,
          0,
          PortFieldWidth,
          pLocalPort1,
          (UINT8)Parent.Node.SocId
          );
      }
      break;
    } else if (Parent.Node.SocType == SOCKET_TYPE_CPU) {
      //
      // Setup the path in intermediate CPU socket found between Sbsp & Dest
      //
      pLocalPort = (UINT8) Parent.Node.ParentPort;
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n    In CPU%u connecting port %u to %u.\n", Parent.Node.SocId, pLocalPort, cLocalPort));
      KTI_ASSERT ((pLocalPort != cLocalPort), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_6F);

      //
      // Forward and Backward Path in KTI Agent
      //
      ProgramMinimumPath (sbspSktId, (UINT8) Parent.Node.SocId, (UINT8) Dest->Node.SocId, pLocalPort, cLocalPort, PortFieldWidth);

      // Forward Path in M3KTI
      ProgramM3KtiRoute (pLocalPort, (UINT8) Parent.Node.SocId, (UINT8) Dest->Node.SocId, cLocalPort, KTIVAR->SharedKtiPort0And1);

      // Backward Path in M3KTI
      ProgramM3KtiRoute (cLocalPort, (UINT8) Parent.Node.SocId, (UINT8) sbspSktId, pLocalPort, KTIVAR->SharedKtiPort0And1);
    } else {
      KtiCheckPoint ((UINT8) (1 << Parent.Node.SocId), (UINT8) Parent.Node.SocType, 0, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Unknown Intermediate Socket Type found. Topology not supported"));
      KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
    }
    //
    // Go up one step towards the Sbsp & repeat the loop
    //
    Child = Parent;
    GetParentSocket (SocketData->SbspMinPathTree, &Child, &Parent);
    if (Parent.Node.SocType == SOCKET_TYPE_CPU) {
      cLocalPort = (UINT8) SocketData->Cpu[Child.Node.SocId].LinkData[pLocalPort].PeerPort;
    } else {
      KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_70);
    }
  }

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n"));
  return KTI_SUCCESS;
}

/**
  Setup system snoop filter for fpga.

  @param SocketData        - pointer to the socket data structure
  @param KtiInternalGlobal - pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
SetupSnoopFilter (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8  Cha;
  UINT8  Socket;
  if (KTI_HOST_OUT_PTR->FpgaPresentBitMap != 0) {
    if (KtiInternalGlobal->SnoopFilter == 1) {
      ChaConfigureSnoopFilterForFpga (KtiInternalGlobal->CpuList, SocketData);
    }
    //
    // Due to known silicon issue on CLX, FPGA may request override to HA_THRESHOLDS.snoop_credits
    //
    if (IsSiliconWorkaroundEnabled ("S2207558868")) {
      if (FpgaIpSnoopCreditOverrideRequired (KTIVAR->OutFpgaCacheAgent)) {
        for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
          if (SocketPresent (Socket)) {
            for (Cha = 0; Cha < KtiInternalGlobal->TotCha[Socket]; Cha++) {
              ChaOverrideSnoopCredits (Socket, Cha);
            }
          }
        }
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  Returns the total KTIAgents supported by this CPU socket.

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Soc ID of the CPU who's enabled LLC slice info is requested, 0...3
  @param KtiAgentCount     - Total KTIAgent in given socket

  @retval 0 - Successful
  @retval 1 - Failure

**/
KTI_STATUS
GetKtiAgentCount (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId,
  OUT UINT8                     *KtiAgentCount
  )
{
  UINT8   Ctr;
  UINT8   Count = 0;

  for (Ctr = 0; Ctr < GetKtiPortCnt(); Ctr++) {
    if (GetKtiLlpmonVendorId (SocId, Ctr) != 0xFFFF) {
      Count++;
      KtiInternalGlobal->KtiPortPresentBitmap[SocId] |= 1 << Ctr;
    }
  }
  *KtiAgentCount = Count;
  return KTI_SUCCESS;
}

/**
  SBSP programs bus numbers and mmcfg rules to each socket.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Targets           - Enabled socket targets
  @param MmcfgTgtlist      - Common mmcfg target list
  @param SocId             - Socket to program

**/
KTI_STATUS
KtiFinalPbspBusCfg (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Targets,
  UINT32                     MmcfgTgtlist,
  UINT8                      SocId
  )
{
  UINT32  Data32, Data32_1, MmcfgLocalRule = 0, ChaMmcfgLocalRule = 0;
  UINT32  CpuBusNoValid = 0;
  UINT8   MmcfgLength = 0;
  UINT8   Idx1, Idx3;
  UINT8   StackPersonality;
  UINT8   ChaMmcfgEnableRule;
  UINT32  ChaMmcfgBaseAddrLow;
  UINT32  ChaMmcfgBaseAddrHi;
  UINT32  ChaMmcfgLocalRuleAddr[2];

  switch (KTIVAR->mmCfgSize / 1024 / 1024) {
  case 64:
    MmcfgLength = 0x6;
    break;
  case 128:
    MmcfgLength = 0x5;
    break;
  case 256:
    MmcfgLength = 0x4;
    break;
  case 512:
    MmcfgLength = 0x2;
    break;
  case 1024:
    MmcfgLength = 0x1;
    break;
  case 2048:
    MmcfgLength = 0x0;
    break;
  default:
    //Automated add of default case. Please review.
    break;
  }

  //
  // Reprogram bus/mmcfg on PBSP sockets
  //
  ChaMmcfgEnableRule = 0;
  ChaMmcfgBaseAddrLow = KtiInternalGlobal->MmcfgBase >> 26;
  ChaMmcfgBaseAddrHi = 0;

  ReprogramPbspBusmmcfg (SocId, 0, MmcfgLength, Targets, ChaMmcfgBaseAddrLow);


  //
  // Write new value for MMCFG_TARGET_LIST
  //
  Data32 = MmcfgTgtlist;
  for (Idx1 = 0; Idx1 < SI_MAX_CPU_SOCKETS; Idx1++) {
    if (((Data32 >> Idx1*SADTGT_SIZE) & 0xF) == SocId) {
      Data32 = (Data32 & ~(0xF << (SADTGT_SIZE * Idx1))) | ((SOCKET_LOCAL_4BITS_NID_MSB | SocId) << (SADTGT_SIZE * Idx1));
    }
  }
  ChaSetMmcfgTargetList (SocId, 0, Data32);

  //
  // Write new value for MMCFG_RULE_LOCAL_ADDRESS
  //

  //
  // Build and write new value for MMCFG_RULE_LOCAL_ADDRESS
  // Build and write final value for MMCFG_RULE_LOCAL and CpuBusNoValid for PCU and VCU
  //
  Data32 = (UINT8)KTIVAR->CpuInfo[SocId].CpuRes.StackRes[IIO_STACK0].BusBase;
  Data32_1 = 0;
  Idx3 = 0;
  for (Idx1 = 0; Idx1 < MAX_LOGIC_IIO_STACK; Idx1++) {
    //
    // Check for valid resources
    //
    StackPersonality = KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Idx1].Personality;
    if ((StackPersonality == TYPE_UBOX_IIO) || (StackPersonality == TYPE_HFI)) {
      if (KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Idx1].BusLimit >= KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Idx1].BusBase) {
        if (Idx3 <= IIO_STACK5) {
          if (Idx3 < IIO_STACK3){
            Data32 |= (KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Idx1].BusLimit << (8 + 8 * Idx3));
          } else {
            Data32_1 |= (KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Idx1].BusLimit << (8 * (Idx3 - IIO_STACK3)));
          }
          ChaMmcfgLocalRule |= Idx1 << (Idx3 * 4);
          MmcfgLocalRule |= Idx1 << (Idx3 * 4);
        }
        Idx3++;
      }
    }
  }

  //
  // Save for use later in KTI/IIO programming
  //
  KtiInternalGlobal->MmcfgLocalRules[SocId][0] = Data32;
  KtiInternalGlobal->MmcfgLocalRules[SocId][1] = Data32_1;
  KtiInternalGlobal->MmcfgLocalRules[SocId][2] = MmcfgLocalRule;

  ChaMmcfgLocalRuleAddr[0] = Data32;
  ChaMmcfgLocalRuleAddr[1] = Data32_1;

  CpuBusNoValid |= 0x80000000;
  ChaConfigureMmcfgRules (SocId, 0, ChaMmcfgEnableRule, MmcfgLength, Targets, ChaMmcfgBaseAddrLow, \
                          ChaMmcfgBaseAddrHi, &ChaMmcfgLocalRule, ChaMmcfgLocalRuleAddr);

  //
  // Write new value for CPUBUSNO0/1
  //
  UBoxConfigureCpuBusNo (SocId, 0);

  //
  // Pass socket relative mmcfgbase to PBSP for local usage
  //
  UBoxSetBiosNonStickyScratchPadRegisterValue (SocId, 0, 15, KTIVAR->CpuInfo[SocId].CpuRes.SegMmcfgBase.lo);

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Initiate S%u update\n", SocId));

  //
  // Tell PBSP to reprogram
  //
  UBoxSetBiosNonStickyScratchPadRegisterValue (SocId, 0, 13, BUSCFGSYNC_PBSP_GO);

  //
  // Update struc so that we begin accessing socket via new bus number
  //
  SetSocketFirstBus (SocId, KTIVAR->CpuInfo[SocId].CpuRes.BusBase);
  SetSocketLastBus (SocId, KTIVAR->CpuInfo[SocId].CpuRes.BusLimit);

  KTI_HOST_OUT_PTR->mmCfgBaseH[SocId] = KTIVAR->CpuInfo[SocId].CpuRes.SegMmcfgBase.hi;
  KTI_HOST_OUT_PTR->mmCfgBaseL[SocId] = KTIVAR->CpuInfo[SocId].CpuRes.SegMmcfgBase.lo;
  KTI_HOST_OUT_PTR->segmentSocket[SocId] = KTIVAR->CpuInfo[SocId].CpuRes.PciSegment;
  KTI_HOST_OUT_PTR->Socket10nmUboxBus0[SocId] = KTIVAR->CpuInfo[SocId].CpuRes.StackRes[UBOX_STACK].BusBase;

  for (Idx1 = 0; Idx1 < MAX_LOGIC_IIO_STACK; Idx1++) {
    SetStackBus (SocId, Idx1, KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Idx1].BusBase);
  }
  return KTI_SUCCESS;
}

/**
  Write to LBC register.

  Note, depending on the value of the parameter LbcType, this function
  will write into LBC_PER_IOU_CONTROL (AFE type) or into KTIMISC_LBC_CONTROL
  (COM type). This function assumes that the bitfield structure of both
  registers is the same and just varies the write destination based on the
  LbcType.

  @param SocId     - CPU Socket Node number (Socket ID)
  @param Port      - KTI Port Index, 0 based
  @param LbcType   - LBC_AFE or LBC_COM
  @param LaneMask  - 20-bit wide lane mask
  @param LoadSel   - 7-bit wide load select (address within LBC)
  @param Data      - 14-bit data value to write

**/
VOID
KtiWriteLbc (
  UINT8                 SocId,
  UINT8                 Port,
  UINT8                 LbcType,
  UINT32                LaneMask,
  UINT32                LoadSel,
  UINT32                Data
  )
{
  UINT64 StartCount;
  StartCount = GetCountTsc ();

  //
  // Make sure LBC_CONTROL.lbc_start == 0x0 for all bundles
  //
  if (!CheckEmulationType (SIMICS_FLAG)) {
    do {
      //
      // Halt the system once timeout
      //
      if (GetDelayTsc (StartCount) > KTI_LBC_TIMEOUT) {
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n Check LBC_CONTROL_BCAST.lbc_start is TIMEOUT"));
        KTI_ASSERT (FALSE, ERR_FULL_SPEED_TRANSITION, MINOR_ERR_S0_TIMEOUT_LBC | SocId);
      }
    } while (IsLbcStartClear (SocId, Port, LbcType) == FALSE);
  }

  //
  // Fill LBC_CONTROL.lbc_start = 1, lbc_req = 2, lbc_ld_sel, lbc_ln_sel and write for all bundles
  //
  WriteLbcData (SocId, Port, LbcType, LaneMask, LoadSel, Data);
}

#ifdef RAS_SUPPORT
/**
  PCI read during for RAS routines.

  @param SocId   - Socket ID
  @param BoxInst - Box Instance, 0 based
  @param Offset  - Register Offset

  @retval PCI address contents

**/
UINT32
ReadCpuPciCfgRas (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset
  )
{
  UINT8               *RegRealAddr, Flag, Size;
  UINT32              RegAddress, ShadowBufOffset, RegRealBus, PhysBus, RegRealDev, RegRealFun, RegRealOff, FlagOffset, Data32;
  CSR_OFFSET          RegOffset;
  UINT32              SbRegPortId;
  CPU_CSR_ACCESS_VAR  *CpuCsrAccessVarPtr;

  //
  // Get the size of the register being accessed
  //
  RegOffset.Data = Offset;
  Size = (UINT8)RegOffset.Bits.size;

  if (RegOffset.Bits.boxtype == BOX_SB) {
    CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();
    SbRegPortId = GetSbRegPortId (BoxInst, Offset);
    if (SbRegPortId != CpuCsrAccessVarPtr->LastSBPortId[SocId]) {
      UBoxConfigureSbRegPteReg (SocId, 0, SbRegPortId);
      CpuCsrAccessVarPtr->LastSBPortId[SocId] = SbRegPortId;
    }
  }

  //
  // Get the real address of this register
  //
  RegRealAddr = (UINT8 *)(UINTN)UsraGetCsrRegisterAddress (SocId, BoxInst, Offset);

  RegAddress = (UINT32)((UINTN)RegRealAddr);
  RegRealBus = (RegAddress >> 20) & 0xFF;
  RegRealDev = (RegAddress >> 15) & 0x1F;
  RegRealFun = (RegAddress >> 12) & 0x7;
  RegRealOff = RegAddress & 0xFFF;

  //
  // Get the physical bus number
  //
  PhysBus = 0;
  if (!((RegOffset.Bits.boxtype == BOX_MCIO) ||
       (RegOffset.Bits.boxtype == BOX_MCDDC) ||
       ((RegOffset.Bits.boxtype == BOX_MC) && (RegOffset.Bits.funcblk == BOX_FUNC_MC_GLOBAL)) ||
       ((RegOffset.Bits.boxtype == BOX_MC) && (RegOffset.Bits.funcblk == BOX_FUNC_MC_MAIN)) ||
       ((RegOffset.Bits.boxtype == BOX_MC) && (RegOffset.Bits.funcblk == BOX_FUNC_MC_2LM)) ||
       (RegOffset.Bits.boxtype == BOX_SB) ||
       (RegOffset.Bits.boxtype == BOX_MS2IDI)
     )) {
    PhysBus = GetPhysicalBusNumber (SocId, RegRealBus);
    KTI_ASSERT ((PhysBus < 0x100), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_43);
  }

#ifdef RAS_KTI_DEBUG
#ifdef RAS_KTI_DEBUG_REG_DETAIL
  if (PhysBus >= MAX_SHADOW_BUS && mDirectCommit == FALSE) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    WARNING: Access to MCP Bus CSRs not supported by shadow buffer during Quiesce:  CSR access to 0x%x", RegRealAddr));
  }
#endif // RAS_KTI_DEBUG_REG_DETAIL
#endif // RAS_KTI_DEBUG

  if (mDirectCommit == TRUE || RegRealOff >= MAX_REG_OFFSET || RegOffset.Bits.boxtype == BOX_SB || RegOffset.Bits.boxtype == BOX_MS2IDI) {
    //
    // These registers can not be in the shadow buffer, so read it from actual register
    //
    if (Size == sizeof (UINT32)) {
      Data32 = *((volatile UINT32 *)RegRealAddr);
    } else if (Size == sizeof (UINT16)) {
      Data32 = *((volatile UINT16 *)RegRealAddr);
    } else {
      Data32 = *((volatile UINT8 *)RegRealAddr);
    }
  } else {
    //
    // If the shawdow buffer has the latest copy, return it; otherwise read it from the register
    //
    ShadowBufOffset = (SocId * MAX_BUF_PER_CPU) + (PhysBus * MAX_BUF_PER_BUS) + (RegRealDev * MAX_BUF_PER_DEV) + (RegRealFun * MAX_BUF_PER_FUN) + RegRealOff;
    Flag = *(mCpuShadowFlag + (ShadowBufOffset / 8));
    FlagOffset = ShadowBufOffset % 8;
    if (Size == sizeof (UINT32)) {
      Flag = (Flag >> FlagOffset) & 0xF;
      if (Flag != 0) {
        Data32 = *((UINT32 *)(mCpuShadowBuf + ShadowBufOffset));
      } else {
        Data32 = *((volatile UINT32 *)RegRealAddr);
      }
    } else if (Size == sizeof (UINT16)) {
      Flag = (Flag >> FlagOffset) & 0x3;
      if (Flag != 0) {
        Data32 = *((UINT16 *)(mCpuShadowBuf + ShadowBufOffset));
      } else {
        Data32 = *((volatile UINT16 *)RegRealAddr);
      }
    } else {
      Flag = (Flag >> FlagOffset) & 0x1;
      if (Flag != 0) {
        Data32 = *((UINT8 *)(mCpuShadowBuf + ShadowBufOffset));
      } else {
        Data32 = *((volatile UINT8 *)RegRealAddr);
      }
    }
  }

  return Data32;
}

/**
  PCI write for RAS routines

  @param SocId   - Socket ID
  @param BoxInst - Box Instance, 0 based
  @param Offset  - Register Offset
  @param Data    - Data to write

  @retval None

**/
VOID
WriteCpuPciCfgRas (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset,
  UINT32   Data
  )
{
  UINT8               *RegRealAddr, Flag, Size;
  UINT32              RegAddress, ShadowBufOffset, RegRealBus, PhysBus, RegRealDev, RegRealFun, RegRealOff, FlagOffset;
  UINTN               RegFlatAdd;
  CSR_OFFSET          RegOffset;
  UINT32              SbRegPortId;
  CPU_CSR_ACCESS_VAR  *CpuCsrAccessVarPtr;

  //
  // Get the size of the register being accessed
  //
  RegOffset.Data = Offset;
  Size = (UINT8)RegOffset.Bits.size;

  if (RegOffset.Bits.boxtype == BOX_SB) {
    CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();
    SbRegPortId = GetSbRegPortId (BoxInst, Offset);
    if (mDirectCommit == FALSE) {
      if (SbRegPortId != CpuCsrAccessVarPtr->LastSBPortId[SocId]) {
        UBoxConfigureSbRegPteReg (SocId, 0, SbRegPortId);
        CpuCsrAccessVarPtr->LastSBPortId[SocId] = SbRegPortId;
      }
    } else {
      //
      // Update the Quiesce Buffer
      //
      UBoxGetSbRegPte0RegisterFlatAddr (SocId, 0, &RegFlatAdd);
      RegRealAddr = (UINT8 *)RegFlatAdd;
      mNextEntry->AccessWidth = 4;
      mNextEntry->DataMask.OrMask = SbRegPortId;
      mNextEntry->Operation = EnumQuiesceWrite;
      mNextEntry->CsrAddress = (UINT64) RegRealAddr;
      mNextEntry->AndMask = 0;
      mNextEntry++;
    }
  }

  //
  // Get the real address of this register
  //
  RegRealAddr = (UINT8 *)(UINTN)UsraGetCsrRegisterAddress (SocId, BoxInst, Offset);

  RegAddress = (UINT32)((UINTN)RegRealAddr);
  RegRealBus = (RegAddress >> 20) & 0xFF;
  RegRealDev = (RegAddress >> 15) & 0x1F;
  RegRealFun = (RegAddress >> 12) & 0x7;
  RegRealOff = RegAddress & 0xFFF;

  //
  // Get the physical bus number
  //
  PhysBus = 0;
  if (!((RegOffset.Bits.boxtype == BOX_MCIO) ||
       (RegOffset.Bits.boxtype == BOX_MCDDC) ||
       ((RegOffset.Bits.boxtype == BOX_MC) && (RegOffset.Bits.funcblk == BOX_FUNC_MC_GLOBAL)) ||
       ((RegOffset.Bits.boxtype == BOX_MC) && (RegOffset.Bits.funcblk == BOX_FUNC_MC_MAIN)) ||
       ((RegOffset.Bits.boxtype == BOX_MC) && (RegOffset.Bits.funcblk == BOX_FUNC_MC_2LM)) ||
       (RegOffset.Bits.boxtype == BOX_SB) ||
       (RegOffset.Bits.boxtype == BOX_MS2IDI)
     )) {
    PhysBus = GetPhysicalBusNumber (SocId, RegRealBus);
    KTI_ASSERT ((PhysBus < 0x100), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_44);
  }

#ifdef RAS_KTI_DEBUG
#ifdef RAS_KTI_DEBUG_REG_DETAIL
  if (PhysBus >= MAX_SHADOW_BUS && mDirectCommit == FALSE) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    WARNING: Access to MCP Bus CSRs not supported by shadow buffer during Quiesce:  CSR access to 0x%x", RegRealAddr));
  }
#endif // RAS_KTI_DEBUG_REG_DETAIL
#endif // RAS_KTI_DEBUG

#ifdef RAS_KTI_DEBUG
  //
  // Some registers can be programmed with out quiescing; the caller is responsible for turning this flag on/off
  // If the register is not tracked in shadow buffer, then those registers also directly committed to the real register.
  //
  if (mDirectCommit == FALSE && RegRealOff >= MAX_REG_OFFSET) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n WARNING: (mDirectCommit == FALSE), RegAddress == 0x%08X > 0x200 \n",RegAddress));
  }
#endif // RAS_KTI_DEBUG

  if (mDirectCommit == TRUE || RegRealOff >= MAX_REG_OFFSET) {   // krishna.. remove mdirectCommit
    if (Size == sizeof (UINT32)) {
      *((volatile UINT32 *)RegRealAddr) = Data;
    } else if (Size == sizeof (UINT16)) {
      *((volatile UINT16 *)RegRealAddr) = (UINT16) Data;
    } else {
      *((volatile UINT8 *)RegRealAddr) = (UINT8) Data;
    }
#ifdef RAS_KTI_DEBUG
#ifdef RAS_KTI_DEBUG_REG_DETAIL
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    Direct WRITE    Addr: 0x%08X    Data:0x%08X    Size:%u", (UINT32) (UINTN) RegRealAddr, Data, Size));
#endif // RAS_KTI_DEBUG_REG_DETAIL
#endif // RAS_KTI_DEBUG
  } else {

#ifdef RAS_KTI_DEBUG
#ifdef RAS_KTI_DEBUG_REG_DETAIL
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n KTI milestone mNextEntry = %x",mNextEntry ));
#endif // RAS_KTI_DEBUG_REG_DETAIL
#endif // RAS_KTI_DEBUG
    //
    // Update the Shadow Buffer and respective flag indicating that the shadow buffer has the latest copy of the register
    //
    ShadowBufOffset = (SocId * MAX_BUF_PER_CPU) + (PhysBus * MAX_BUF_PER_BUS) + (RegRealDev * MAX_BUF_PER_DEV) + (RegRealFun * MAX_BUF_PER_FUN) + RegRealOff;
    Flag = *(mCpuShadowFlag + (ShadowBufOffset / 8));
    FlagOffset = ShadowBufOffset % 8;
    if (Size == sizeof (UINT32)) {
      *((UINT32 *)(mCpuShadowBuf + ShadowBufOffset)) = Data;
      Flag = Flag | (0xF << FlagOffset);
      *(mCpuShadowFlag + (ShadowBufOffset / 8)) = Flag;
      mNextEntry->AccessWidth = 4;
      mNextEntry->DataMask.OrMask = Data;
    } else if (Size == sizeof (UINT16)) {
      *((UINT16 *)(mCpuShadowBuf + ShadowBufOffset)) = (UINT16)Data;
      Flag = Flag | (0x3 << FlagOffset);
      *(mCpuShadowFlag + (ShadowBufOffset / 8)) = Flag;
      mNextEntry->AccessWidth = 2;
      mNextEntry->DataMask.OrMask = (UINT16)Data;
    } else {
      *((UINT8 *)(mCpuShadowBuf + ShadowBufOffset)) = (UINT8)Data;
      Flag = Flag | (0x1 << FlagOffset);
      *(mCpuShadowFlag + (ShadowBufOffset / 8)) = Flag;
      mNextEntry->AccessWidth = 1;
      mNextEntry->DataMask.OrMask = (UINT8)Data;
    }

    //
    // Update the Quiesce Buffer
    //
    mNextEntry->Operation = EnumQuiesceWrite;
    mNextEntry->CsrAddress = (UINT64) RegRealAddr;
    mNextEntry->AndMask = 0;
    mNextEntry++;
#ifdef RAS_KTI_DEBUG
    CsrWriteCheckPoint ();
#endif // RAS_KTI_DEBUG
  }

  return;
}
#endif // RAS_SUPPORT

/**
  Routine to program the route table for M3KTI and KTI Agent.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket Id
  @param ProgramPeMode       PE bit setting. True/False

  @retval None

**/
VOID
ProgramM3KtiAndKtiAgentRouteTable (
  IN KTI_SOCKET_DATA          *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN UINT8                    SocId,
  IN BOOLEAN                  ProgramPeMode
  )
{
  UINT8                        Port;

  for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
    if ((SocketData->Cpu[SocId].LinkData[Port].Valid == TRUE)) {
      //
      // Program M3KTI Route Table & Vn attribute & Snoop Fanout & pe_mode
      //
      ProgramM3KtiMeshStop (
        SocId,
        Port,
        SocketData->Route[SocId].M3KtiRt[Port],
        0,
        SocketData->Route[SocId].M3KtiVnSel[Port].IgrVnSel,
        (BOOLEAN) KtiInternalGlobal->D2cEn,
        ProgramPeMode,
        FALSE
        );
      //
      // Program KTI Agent Route Table
      //
      ProgramKtiAgentRouteTable (SocId, Port, SocketData->Route[SocId].KtiAgentRt[Port], ProgramPeMode);
    }
  }
}

/**
  Routine to program the final Mmmiol rules for given socket

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0 - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuMmiolEntriesKti (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId
  )
{
  UINT8   LinkIndex;
  for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
    if (SocketData->Cpu[SocId].LinkData[LinkIndex].Valid == TRUE) {
      ProgramKtiMmiolTad (SocId, LinkIndex, &(KTIVAR->CpuInfo[SocId].CpuRes));
    }
  }

  return KTI_SUCCESS;
}

/**
  Routine to program the final IO SAD setting for the given topology.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuMmiohEntriesKti (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId
  )
{
  UINT8         LinkIndex;

  for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
    if (SocketData->Cpu[SocId].LinkData[LinkIndex].Valid == TRUE) {
      ProgramKtiMmiohTad (SocId, LinkIndex, &(KTIVAR->CpuInfo[SocId].CpuRes));
    }
  }

  return KTI_SUCCESS;
}

/**
  Routine to setup the boot path from Remote CPUs (i.e >1 hop away) to Legacy CPU.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Interm            - Immediate parent of the "Dest" in SBSP minimum tree
  @param Dest              - Remote CPU socket for which the boot path needs to be set
  @param DestPort          - Port of Remote CPU socket's immediate parent that connects them.

  @retval 0 - Successful
  @retval Other - failure

**/
KTI_STATUS
SetupRemoteCpuBootPath (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  TOPOLOGY_TREE_NODE        *Interm,
  TOPOLOGY_TREE_NODE        *Dest,
  UINT8                     DestPort
  )
{
  TOPOLOGY_TREE_NODE  Parent;
  TOPOLOGY_TREE_NODE  Child;
  UINT8               cLocalPort;
  UINT8               pLocalPort = 0;
  UINT8               sbspSktId;
  UINT8               ChaPortFieldWidth;

  sbspSktId         = KtiInternalGlobal->SbspSoc;
  ChaPortFieldWidth = 2; // Two bit field to encode link 0, 1 & 2

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n Setting boot path for CPU%u ", Dest->Node.SocId));

  //
  // Set the boot path for Remote CPU
  //
  Parent      = *Interm;
  Child       = *Dest;
  cLocalPort  = DestPort;
  while (1) {
    if (Parent.Node.SocId == sbspSktId && Parent.Node.SocType == SOCKET_TYPE_CPU) {
      ChaKtiBroadcastRouteTableEnable ((UINT8) Dest->Node.SocId, TRUE);

      //
      // In Remote CPU, set route to Legacy CPU
      //
      if (DestPort >= MAX_FW_KTI_PORTS) {
        KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Invalid dest port!\n"));
        return KTI_FAILURE;
      }
      pLocalPort = (UINT8) SocketData->Cpu[Interm->Node.SocId].LinkData[DestPort].PeerPort;
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n    In CPU%u setting Cbo route to port %u\n", Dest->Node.SocId, pLocalPort));
      ConfigureChaKtiRTForSbspOrRouteToSbsp (
        (UINT8) Dest->Node.SocId,
        KtiInternalGlobal->TotCha[Dest->Node.SocId],
        ChaPortFieldWidth,
        pLocalPort,
        (UINT8) KtiInternalGlobal->SbspSoc,
        KtiInternalGlobal->UpiRoutingMode
        );
      break;
    } else if (Parent.Node.SocType == SOCKET_TYPE_CPU) {
      //
      // Setup the pass-thru path in intermediate CPU socket
      //
      pLocalPort = (UINT8) Parent.Node.ParentPort;
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n    In CPU%u connecting port %u to %u.\n", Parent.Node.SocId, pLocalPort, cLocalPort));
      KTI_ASSERT ((cLocalPort != pLocalPort), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_71);

      //
      // Forward and Backward Path in KTI Agent
      //
      ProgramMinimumPath (sbspSktId, (UINT8) Parent.Node.SocId, (UINT8)Dest->Node.SocId, pLocalPort, cLocalPort, ChaPortFieldWidth);

      // Forward Path in M3KTI
      ProgramM3KtiRoute (pLocalPort, (UINT8) Parent.Node.SocId, (UINT8) Dest->Node.SocId, cLocalPort, KTIVAR->SharedKtiPort0And1);

      // Backward Path in M3KTI
      ProgramM3KtiRoute (cLocalPort, (UINT8) Parent.Node.SocId, (UINT8) sbspSktId, pLocalPort, KTIVAR->SharedKtiPort0And1);
    } else {
      KtiCheckPoint ((UINT8) (1 << Parent.Node.SocId), (UINT8) Parent.Node.SocType, 0, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Unknown Intermediate Socket Type found. Topology not supported"));
      KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
    }
    //
    // Go up one step towards the Sbsp & repeat the loop
    //
    Child = Parent;
    GetParentSocket (SocketData->SbspMinPathTree, &Child, &Parent);
    if (Parent.Node.SocType == SOCKET_TYPE_CPU) {
      cLocalPort = (UINT8) SocketData->Cpu[Child.Node.SocId].LinkData[pLocalPort].PeerPort;
    } else {
      KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_72);
    }
  }

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n"));

  return KTI_SUCCESS;
}

/**
  Parse the Link Exchange Parameter (LEP) for a given CPU socket.

  @param SocketData        - add argument and description to function comment
  @param KtiInternalGlobal - KTIRC internal variables.
  @param SocId             - CPU Socket Number, 0 based

  @retval 0 - found
  @retval 1 - not found

**/
KTI_STATUS
ParseCpuLep (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  )
{
  UINT32                              InitState;
  UINT32                              LlStatus;
  UINT8                               PeerPort;
  UINT8                               PeerSocType;
  UINT8                               Port;
  UINT8                               PeerSocId;
  UINT8                               ApSocId;
  UINT32                              Ctr;
  UINT32                              WaitTime;
  UINT8                               FpgaPort = 0xFF;
  BOOLEAN                             CheckInFlag, UseKtiPcSts;
  UINT8                               CpuType;
  BOOLEAN                             bCheckInBitsSet;

  KTI_ASSERT ((SocId < MAX_SOCKET), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_7);

  //
  // Parse the LEP of the socket
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n   CPU%u Link Exchange", SocId));

  //
  // Detect if this socket is an FPGA SKU and indicate port number
  //
  if (FpgaIpIsNodeFpga (SocId)) {
    FpgaPort = XEON_FPGA_PORT;
  }

  //
  // During RAS online operation, use KTILS_KTI_LLPMON_REG for pre-existing sockets because pcode will not be executed and KTIPCSTS_KTI_LLPMON_REG is updated by pcode.
  // The online socket will be using KTIPCSTS_KTI_LLPMON_REG as required as before if its LEP gets needed.
  //
  //
  UseKtiPcSts = (BOOLEAN) (((KTIVAR->RasInProgress != TRUE) || ((KTIVAR->RasInProgress == TRUE) && (KTIVAR->RasSocId == SocId))) ? TRUE : FALSE);
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO0, "\n   UseKtiPcSts = %u", UseKtiPcSts));

  if ((KTIVAR->RasInProgress == TRUE) && (KTIVAR->RasSocId == SocId)) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO0, "\n   ParseCpuLep() for online CPU%u", SocId));
  }

  CpuType     = KtiInternalGlobal->CpuType;

  for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
    if ((KtiInternalGlobal->KtiPortPresentBitmap[SocId] & (1 << Port)) == 0) {
      continue;
    }

    LlStatus = GetLlStatus (SocId, Port);
    InitState = GetInitState (SocId, Port);

    if ((UseKtiPcSts  && ((LlStatus == LINK_SM_INIT_DONE)  || (LlStatus == LINK_SM_CRD_RETURN_STALL))) ||
        (!UseKtiPcSts && ((InitState == LINK_SM_INIT_DONE) || (InitState == LINK_SM_CRD_RETURN_STALL)))) {
      //
      // Identify the Peer Port number
      //
      ReadLep (SocId, Port, &PeerSocId, &PeerPort, &PeerSocType);
      KTI_ASSERT ((PeerPort < KtiInternalGlobal->KtiPortCnt), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);

      if (PeerSocId >= MAX_SOCKET) {
        KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n\n CPU%u Link %u has invalid LEP0", SocId, Port));
        KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
      }

      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO0, " : LEP%u(%u,%a%u)", Port, PeerPort, ((FpgaPort == Port) || (PeerSocType == ICX_LP0_SKU_TYPE_FPGA)) ? "PGA":"CPU", PeerSocId));

      // Invalidate this link if either the current port or its peer port were set to "Disabled" via Setup.
      if ((KTISETUP->PhyLinkPerPortSetting[SocId].Link[Port].KtiPortDisable == TRUE) ||
          (KTISETUP->PhyLinkPerPortSetting[PeerSocId].Link[PeerPort].KtiPortDisable == TRUE)) {

        // Before disabling the link, if it's attached to the SBSP, make sure the AP (non-SBSP) socket
        // has completed its reset and has "checked-in" with the SBSP.  Otherwise,
        // disabling the link may cause MCA when socket becomes isolated.
        // Skip if Fpga EP
        if (((SocId == KtiInternalGlobal->SbspSoc) || (PeerSocId == KtiInternalGlobal->SbspSoc)) && (FpgaPort != Port)) {
          ApSocId = ((SocId == KtiInternalGlobal->SbspSoc) ? PeerSocId : SocId);  //Non-SBSP Socket
          CheckInFlag = FALSE;
          WaitTime = KTISETUP->WaitTimeForPSBP;
          for (Ctr = 0; Ctr < WaitTime; Ctr++) {
            UBoxCheckInBitsIsSetInScratchpad (ApSocId, 0, &bCheckInBitsSet);
            if (bCheckInBitsSet) {  // Read CheckInBit to determine if AP has checked-in
              CheckInFlag = TRUE;
              KtiDebugPrintInfo1 ((KTI_DEBUG_INFO0, "-Disabled"));

              break;
            }

            KtiFixedDelay (1000); // 1000 microsecs
          }
          if (CheckInFlag == FALSE) {
            KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n\n AP Socket %u is discovered but hasn't come out of reset. System Halted!!!", ApSocId));
            KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
          }
          //
          // In LT enabled system, releasing a non-directly connected socket may cause a LT_SHUTDOWN,
          // which causes a surprise warm reset.
          // This is seen in 8S LT enabled sysem that uses production PCH with debug ACM (ACM isn't running in this case).
          // Before soft disabling directly connected socket here, set bit29 of SP7 on this socket, so that
          // SEC code would be able to detect and handle this special case (where the surprise warm reset
          // occurs after BIOS sends BOOT_GO command to non-directly connected socket.
          //
          UBoxSetRemoteSocketReleasedFlagInScratchpad (ApSocId, 0, 1);
        }//if SBSP link

        // SBSP neighbor (AP) socket has checked-in.  The link is no longer needed so mark it 'disabled' and move on to the next link.
        SocketData->Cpu[SocId].LinkData[Port].Valid = FALSE;
        continue;
      }//if link set to "disabled"

      //
      // Mark the Link as valid for the socket
      //
      SocketData->Cpu[SocId].LinkData[Port].Valid       = TRUE;
      SocketData->Cpu[SocId].LinkData[Port].PeerPort    = PeerPort;
      SocketData->Cpu[SocId].LinkData[Port].PeerSocId   = PeerSocId;
      if (FpgaPort == Port) {
        SocketData->Cpu[SocId].LinkData[Port].PeerSocType = SOCKET_TYPE_FPGA;
      } else {
        SocketData->Cpu[SocId].LinkData[Port].PeerSocType = SOCKET_TYPE_CPU;
      }

    } else if (!SocketFpgasInitialized (KtiInternalGlobal)) {
      //
      // SKX Case
      // FPGA will not train until BBS is downloaded into it.  Prior to that we pretend it trained
      //
      if (FpgaPort == Port) {
        if (KtiInternalGlobal->FpgaEnable[SocId]) {
          SocketData->Cpu[SocId].LinkData[Port].Valid       = TRUE;
          SocketData->Cpu[SocId].LinkData[Port].PeerPort    = FPGA_PORT;
          SocketData->Cpu[SocId].LinkData[Port].PeerSocId   = SocId + FPGA_NID_OFFSET;
          SocketData->Cpu[SocId].LinkData[Port].PeerSocType = SOCKET_TYPE_FPGA;
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, " : SLEP%u(%u,%a%u)", Port, SocketData->Cpu[SocId].LinkData[Port].PeerPort, "PGA", SocketData->Cpu[SocId].LinkData[Port].PeerSocId));
        }
      }
    }
  }

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n"));

  return KTI_SUCCESS;
}


/**
  Modify UPI SAI policy for IOMT

  This function will override the SAI security policy group settings to enable EV IOMT tool.
  This function should only be executed in DEBUG build target, or in DFM manufacturing mode.
  This DFX feature must not be present when OEM reuse reference BIOS implementation.

  A PCU request MAILBOX_BIOS_CMD_MISC_WORKAROUND_ENABLE (7Ch) with parameter
  MAILBOX_BIOS_CMD_MISC_WORKAROUND_ENABLE_IOMT_WORKAROUND_ENABLE (4) has been sent to PCU to
  allow configuring additional access rights for IOMT tool running in host. The request to PCU
  must be sent between RESET_CPL1 and RESET_CPL2 and then the configuration changes
  must be done before RESET_CPL3.

  @param[in] Socket - Socket ID
  @param[in] Port   - Port ID
**/
VOID
SetDfxUpiIomtSaiPolicy (
  IN UINT8                 Socket,
  IN UINT8                 Port
  )
{
  UINT32     RegVal;
  UINT32     RegValBkp;

  static CONST UINT32 RegAddrTab[] = {//
                                      // SAI policy registers to update
                                      //
                                      // ktiln group
                                      //
                                      UPIAGT_LB_W_WAC_LO_KTI_LLPMON_REG,
                                      UPIAGT_LB_MCHECK_W_WAC_LO_KTI_LLPMON_REG,
                                      UPIAGT_DFX_RAC_LO_KTI_LLPMON_REG,
                                      UPIAGT_DFX_WAC_LO_KTI_LLPMON_REG,
                                      //
                                      // ktimisc group
                                      //
                                      UPIMISC_LB_W_WAC_LO_KTI_LLDFX_REG,
                                      UPIMISC_BIOS_W_WAC_LO_KTI_LLDFX_REG,
                                      UPIMISC_DFX_RAC_LO_KTI_LLDFX_REG,
                                      UPIMISC_DFX_WAC_LO_KTI_LLDFX_REG,
                                      //
                                      // ktiph group
                                      //
                                      UPIPH_LB_W_WAC_LO_KTI_REUT_REG,
                                      UPIPH_DFX_RAC_LO_KTI_REUT_REG,
                                      UPIPH_DFX_WAC_LO_KTI_REUT_REG,
                                      UPIPH_LB_MCHECK_W_WAC_LO_KTI_REUT_REG,
                                      UPIPH_BIOS_W_WAC_LO_KTI_REUT_REG,
                                      //
                                      // reconfig group
                                      //
                                      M2UPI_LB_W_WAC_LO_M3KTI_MAIN_REG,
                                      M2UPI_BIOS_W_WAC_LO_M3KTI_MAIN_REG,
                                      M2UPI_DFX_RAC_LO_M3KTI_MAIN_REG,
                                      M2UPI_DFX_WAC_LO_M3KTI_MAIN_REG,
                                      M2UPI_LB_MCHECK_W_WAC_LO_M3KTI_MAIN_REG,
                                      //
                                      // gen3physai
                                      // NOTE: Test indicates BIOS can't modify the KTI G3PHY BIOS_W policy...
                                      // unless some other firmware (e.g. ucode) can set bit4 (HOSTIA_BOOT) on the KTI Gen3PHY BIOS_W CP register
                                      //
                                      G3PHY_BIOS_WAC_LO_KTI_GEN3PHY_REG};
  //
  // UPI Lock bit registers
  //
  UNIPHY_FUSE_CTRL_KTI_GEN3PHY_STRUCT  UniphyFuseCtrl;
  KTIMISCMODLCK_KTI_LLDFX_STRUCT       KtiMiscModLck;

  UINT8  Index;

  for (Index = 0; Index < ARRAY_SIZE (RegAddrTab); Index++) {

    RegVal = RegValBkp = UsraCsrRead (Socket, Port, RegAddrTab[Index]);
    RegVal |= BIT0 | BIT25;
    UsraCsrWrite (Socket, Port, RegAddrTab[Index], RegVal);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "[UPI][DFX] %u.%u Register %08X %X -> %X -> %X\n",
                         Socket, Port, RegAddrTab[Index], RegValBkp, RegVal,
                         UsraCsrRead (Socket, Port, RegAddrTab[Index])));
  }
  UniphyFuseCtrl.Data = (UINT8) UsraCsrRead (Socket, Port, UNIPHY_FUSE_CTRL_KTI_GEN3PHY_REG);
  KtiMiscModLck.Data  = UsraCsrRead (Socket, Port, KTIMISCMODLCK_KTI_LLDFX_REG);
  //
  // Clear the lock bits
  //
  UniphyFuseCtrl.Bits.iodfxlck                  = 0;
  KtiMiscModLck.Bits.iodfxlck_and_glblcsrlcken  = 0;
  //
  // Update the lock bit registers with the new values to enable EV IOMT
  //
  UsraCsrWrite (Socket, Port, UNIPHY_FUSE_CTRL_KTI_GEN3PHY_REG, UniphyFuseCtrl.Data);
  UsraCsrWrite (Socket, Port, KTIMISCMODLCK_KTI_LLDFX_REG, KtiMiscModLck.Data);
} // SetDfxUpiIomtSaiPolicy()


/**
  The RxDCC value should be overwritten to a 'safe' value to ensure reliable startup of the RxDCC clock when transitioning from SLOW to FAST.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

**/
VOID
SlowModeRxDccOverride (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  return ;
}

/**
   Programs the values from the EPARAMS entry for specified link

   @param Socket              - SocId we are looking up
   @param Link                - Link# on socket
   @param *LinkSpeedParameter - ptr to eparam data

   @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ConfigSocketLinkEparams (
  UINT8          Socket,
  UINT8          Link,
  VOID           *LinkSpeedParameter
  )
{
  UINT8                                      Data8;
  UINT8                                      Lane;
  UINT8                                      PreCursor;
  UINT8                                      Cursor;
  UINT8                                      PostCursor;
  EFI_STATUS                                 Status;
  TXEQ_LEVELS_LANE0_BCAST_KTI_GEN3PHY_STRUCT TxEqStructure;

  UINT32 TxeqLvlReg[MAX_KTI_LANES]  = {TXEQ_LEVELS_LANE0_B0_KTI_GEN3PHY_REG,
                                       TXEQ_LEVELS_LANE1_B0_KTI_GEN3PHY_REG,
                                       TXEQ_LEVELS_LANE0_B1_KTI_GEN3PHY_REG,
                                       TXEQ_LEVELS_LANE1_B1_KTI_GEN3PHY_REG,
                                       TXEQ_LEVELS_LANE0_B2_KTI_GEN3PHY_REG,
                                       TXEQ_LEVELS_LANE1_B2_KTI_GEN3PHY_REG,
                                       TXEQ_LEVELS_LANE0_B3_KTI_GEN3PHY_REG,
                                       TXEQ_LEVELS_LANE1_B3_KTI_GEN3PHY_REG,
                                       TXEQ_LEVELS_LANE0_B4_KTI_GEN3PHY_REG,
                                       TXEQ_LEVELS_LANE1_B4_KTI_GEN3PHY_REG,
                                       TXEQ_LEVELS_LANE0_B5_KTI_GEN3PHY_REG,
                                       TXEQ_LEVELS_LANE1_B5_KTI_GEN3PHY_REG,
                                       TXEQ_LEVELS_LANE0_B6_KTI_GEN3PHY_REG,
                                       TXEQ_LEVELS_LANE1_B6_KTI_GEN3PHY_REG,
                                       TXEQ_LEVELS_LANE0_B7_KTI_GEN3PHY_REG,
                                       TXEQ_LEVELS_LANE1_B7_KTI_GEN3PHY_REG,
                                       TXEQ_LEVELS_LANE0_B8_KTI_GEN3PHY_REG,
                                       TXEQ_LEVELS_LANE1_B8_KTI_GEN3PHY_REG,
                                       TXEQ_LEVELS_LANE0_B9_KTI_GEN3PHY_REG,
                                       TXEQ_LEVELS_LANE1_B9_KTI_GEN3PHY_REG
                                     };

  PER_LANE_EPARAM_LINK_INFO     *Ptr = NULL;

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Socket %u Port %u : EPARAM entry programmed\n", Socket, Link));
  Ptr = (PER_LANE_EPARAM_LINK_INFO *) LinkSpeedParameter;
  if (Ptr->AllLanesUseSameTxeq == ALL_LANES_TXEQ_ENABLED) {
    //
    // All lanes use same txeq value, use broadcast as unit ID
    //
    if (FpgaIpIsNodeFpga (Socket)) {
      PostCursor = (UINT8) ((Ptr->TXEQL[0] >> 16) & 0x3f);;
      Cursor = (UINT8) ((Ptr->TXEQL[0] >> 8) & 0x3f);;
      PreCursor = (UINT8) ((Ptr->TXEQL[0]) & 0x3f);;
      for (Lane= 0; Lane < MAX_KTI_LANES; Lane++) {
        Status = FpgaIpProgramTxEqParameters (Socket, Link, Lane, PreCursor, Cursor, PostCursor);
        KTI_ASSERT ((Status == EFI_SUCCESS), ERR_FULL_SPEED_TRANSITION, MINOR_ERR_KTI_ELEC_PARAM_INVALID);
      }
    } else {
      TxEqStructure.Data = 0;
      TxEqStructure.Bits.txeq_lvl0 = Ptr->TXEQL[0] & 0x03f;
      TxEqStructure.Bits.txeq_lvl1 = (Ptr->TXEQL[0] >> (1 * 8)) & 0x3f;
      TxEqStructure.Bits.txeq_lvl2 = (Ptr->TXEQL[0] >> (2 * 8)) & 0x3f;
      TxEqStructure.Bits.txeq_lvl3 = (Ptr->TXEQL[0] >> (3 * 8)) & 0x3f;
      KtiWritePciCfg (Socket, Link, TXEQ_LEVELS_LANE0_BCAST_KTI_GEN3PHY_REG, TxEqStructure.Data);
      KtiWritePciCfg (Socket, Link, TXEQ_LEVELS_LANE1_BCAST_KTI_GEN3PHY_REG, TxEqStructure.Data);
    }
    //
    // Only ADAPTIVE_CTLE is supported, assert if Eparam Table indicates otherwise
    //
    KTI_ASSERT ((Ptr->CTLEPEAK[0] == ADAPTIVE_CTLE), ERR_FULL_SPEED_TRANSITION, MINOR_ERR_KTI_ELEC_PARAM_INVALID);
  } else {
    //
    // Each lanes has different txeq value, use lane # as unit ID
    //
    for (Lane= 0; Lane < MAX_KTI_LANES; Lane++) {
      TxEqStructure.Data = 0;
      TxEqStructure.Bits.txeq_lvl0 = Ptr->TXEQL[Lane] & 0x03f;
      TxEqStructure.Bits.txeq_lvl1 = (Ptr->TXEQL[Lane] >> (1 * 8)) & 0x3f;
      TxEqStructure.Bits.txeq_lvl2 = (Ptr->TXEQL[Lane] >> (2 * 8)) & 0x3f;
      TxEqStructure.Bits.txeq_lvl3 = (Ptr->TXEQL[Lane] >> (3 * 8)) & 0x3f;
      KtiWritePciCfg (Socket, Link, TxeqLvlReg[Lane], TxEqStructure.Data);
    }
    //
    // Check ADAPTIVE_CTLE per lane (only ADAPTIVE_CTLE is supported)
    //
    for (Lane= 0; Lane < MAX_KTI_LANES; Lane++) {
      Data8 = ((UINT8) (Ptr->CTLEPEAK[Lane/4] >> ((Lane%4) * 8)) & 0xFF);
      KTI_ASSERT ((Data8 == ADAPTIVE_CTLE), ERR_FULL_SPEED_TRANSITION, MINOR_ERR_KTI_ELEC_PARAM_INVALID);
    }
  }

  return KTI_SUCCESS;
}


/**
  KTI Physical Layer programming performed after warm reset.

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval KTI_SUCCESS - on successful procedure execution

**/
KTI_STATUS
KtiPhyLinkAfterWarmReset (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  EFI_STATUS Status;
  UINT8      Index;
  UINT8      LinkIndex;
  UINT8      KtiDfxLck;

  //
  // For unconnected links, need to set fake_L1.
  // No need to set it on firmware-disabled links, since it is done before WR.
  //
  for (Index = 0; Index < MAX_SOCKET; Index++) {

    if ((SocketData->Cpu[Index].Valid == TRUE) && (SocketData->Cpu[Index].SocType == SOCKET_TYPE_CPU)) {

      if (KTISETUP->DfxParm.DfxIioDfxEnabled) {
        //
        // Request PCU to allow configuring additional access rights for IOMT tool running in host.
        // PCU accepts this commant only between RESET_CPL1 and RESET_CPL2, then the configuration
        // changes must be done before RESET_CPL3.
        //
        Status = PcuSetMiscWorkaroundEnable (Index, MAILBOX_BIOS_CMD_MISC_WORKAROUND_ENABLE_IOMT_WORKAROUND_ENABLE);
        if (EFI_ERROR (Status)) {

          KtiDebugPrintWarn ((KTI_DEBUG_WARN, "[UPI][DFX] WARNING: Failed to enable IOMT support in S%d PCU (%r)\n",
                              Index, Status));
        }
      }
      for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
        if ((KtiInternalGlobal->KtiPortPresentBitmap[Index] & (1 << LinkIndex)) != 0) {
          if ((SocketData->Cpu[Index].LinkData[LinkIndex].Valid == FALSE)  && (KTISETUP->KtiLbEn == KTI_DISABLE)) {
            SetUpiLinkToL1 (Index, LinkIndex);
          }
        }
        //
        // Lock KTI DFX after warm reset or unlock for EV first level debug
        //
        if ((SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE) || (KTISETUP->KtiLbEn == KTI_ENABLE)) {
          if (KtiInternalGlobal->EvMode == KTI_ENABLE) {
            KtiDfxLck = 0;
          } else {
            KtiDfxLck = 1;
          }
          //
          // For now always clear in emulation
          //
          if (UbiosGenerationOrHsleEnabled ()) {
            KtiDfxLck = 0;
          }
          SetKtiDfxLck (Index, LinkIndex, KtiDfxLck);
        }
        if (KTISETUP->DfxParm.DfxIioDfxEnabled) {
          //
          // Modify UPI PHY SAI policy settings to enable EV IOMT on locked parts.
          //
          SetDfxUpiIomtSaiPolicy (Index, LinkIndex);
        }
      }
    }
  }
  //
  //ONLY R0 needs this WA.
  //
  if (IsCpuAndRevision (CPU_ICXSP, REV_R0)) {
    return KtiTransitionFullSpeedOnWarmReset (SocketData, KtiInternalGlobal);
  }
  return KTI_SUCCESS;
}

/**
  Routine to determine and select the maximum KTI link speed supported by all populated CPUs. It is determined by
  Min (Max link speed of all populated CPU sockets).

  @param [in] none

  @retval Max Link speed supported by all populated sockets

**/
UINT8
GetMaxSupportedLinkSpeed (
  VOID
  )
{
  UINT8 SupportedLinkSpeeds = 0;
  UINT8 LinkSpeed = SPEED_REC_96GT;
  UINT8 Index;

  //
  // Select the highest common supported freq
  //
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if (SocketPresent (Index)) {
      //
      // Get common supported freqs by ORing all DISABLED freqs together.   Zero bits indicate supported freqs
      //
      SupportedLinkSpeeds |= PcuGetUpiFreqDisable (Index);
    }
  }

  if ((SupportedLinkSpeeds & BIT1) == 0) {
    LinkSpeed = SPEED_REC_112GT;
  } else if ((SupportedLinkSpeeds & BIT0) == 0) {
    LinkSpeed = SPEED_REC_104GT;
  } else {
    //
    // 9.6GT/s is always supported
    //
    LinkSpeed = SPEED_REC_96GT;
  }

  return LinkSpeed;
}

/**
  Verifies the selected KTI freq is valid for the platform
  If valid, return == KTI_SUCCESS

  @param RequestedLinkSpeed - reuqested speed for KTI links

  @retval KTI_SUCCESS if valid speed
  @retval KTI_FAILURE if invalid speed

**/
KTI_STATUS
CheckLinkSpeedSupported (
  UINT8               RequestedLinkSpeed
  )
{
  UINT8 SupportedLinkSpeeds = 0;
  UINT8 Index;

  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if (SocketPresent (Index)) {
      //
      // Get common supported freqs by ORing all DISABLED freqs together. Set bits indicate unsupported freqs
      //
      SupportedLinkSpeeds |= PcuGetUpiFreqDisable (Index);
    }
  }

  //
  // See if requested freq is supported
  //
  if (RequestedLinkSpeed == SPEED_REC_104GT) {
    Index = 0;
  } else if (RequestedLinkSpeed == SPEED_REC_112GT) {
    Index = 1;
  } else {
    Index = 0xff;
  }

  if ((RequestedLinkSpeed == SPEED_REC_96GT) || ((Index != 0xFF) && (((SupportedLinkSpeeds >> Index) & 1) == 0))) {
    //
    // Selected freq is valid
    //
    return KTI_SUCCESS;
  } else {
    //
    // Selected freq is invalid
    //
    return KTI_FAILURE;
  }
}

KTI_STATUS
ClearOrSetKtiDfxLckForFullSpeedTransition (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8                     Value
  )
{
  UINT8 Socket, Port;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresent (Socket)) {
      for (Port = 0; Port < GetKtiPortCnt (); Port++) {
        if ((KTIVAR->CpuInfo[Socket].LepInfo[Port].Valid == TRUE) || (KTISETUP->KtiLbEn)) {
          SetKtiDfxLck (Socket, Port, Value);
        }
      }
    }
  }
  return KTI_SUCCESS;
}

/**
  Routine to program the final MMCFG SAD for the given socket links

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
KtiFinalBusCfgKti (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId
  )
{
  UINT8   BusBaseTmp;
  UINT8   BusLimit;
  UINT32  BusLocalLimits[2] = {0,0};
  UINT32  BusLocalTgtLst = 0;
  UINT8   TargetIndex = 0;
  UINT8   LinkIndex;
  UINT8   Stack;

  //
  // If PCH attached route dev 16-31 down DMI link
  //
  if (SocId == KtiInternalGlobal->SbspSoc) {
    BusLocalTgtLst |= 1 << 24;
  }
  //
  // Setup starting base of legacy IO for the socket
  //
  BusLocalLimits[0] = (UINT32) (KTIVAR->CpuInfo[SocId].CpuRes.BusBase);

  for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
    //
    // Program a target and limit for each enabled stack
    //
    if (StackPresent (SocId, Stack)) {
      BusBaseTmp = KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Stack].BusBase;
      if (BusBaseTmp != 0xFF) {
        BusLocalTgtLst |= Stack << (TargetIndex * 3);
        BusLimit = KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Stack].BusLimit;

        KTI_ASSERT ((((TargetIndex / 3) < (sizeof (BusLocalLimits) / sizeof (BusLocalLimits[0])))), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_86);
        if (TargetIndex < 3) {
          BusLocalLimits[TargetIndex / 3] |= (BusLimit << ((TargetIndex + 1) * 8)) ;
        } else {
          BusLocalLimits[TargetIndex / 3] |= (BusLimit << ((TargetIndex - 3) * 8));
        }
        TargetIndex++;
      }
    }
  }

  for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
    if (SocketData->Cpu[SocId].LinkData[LinkIndex].Valid == TRUE) {
      ProgramKtiBusTad (SocId, LinkIndex, &BusLocalTgtLst, BusLocalLimits);
    }
  }

  return KTI_SUCCESS;
}

/**
  Set the Bus Number for WHR and SIP, on all FPGA nodes,
  using an FPGA IP Library function call

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
SetFpgaBusNumber (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT32 WhrBusDeviceNumber;
  UINT8  SipBusNumber;
  UINT8  NodeId;

  for (NodeId = 0; NodeId < MAX_SOCKET; NodeId++) {
    if ((SocketData->Cpu[NodeId].Valid == TRUE) && (SocketData->Cpu[NodeId].SocType == SOCKET_TYPE_FPGA)) {
      WhrBusDeviceNumber = (KTIVAR->CpuInfo[NodeId].CpuRes.StackRes[WHR_STACK].BusBase << 5) | WHR_CFG_DEVICE; // WHR_STACK is logical stack 0
      SipBusNumber = KTIVAR->CpuInfo[NodeId].CpuRes.StackRes[SIP_STACK].BusBase; // SIP_STACK is logical stack 1
      FpgaIpSetBusNumbers (NodeId, WhrBusDeviceNumber, SipBusNumber);
    }
  }

  return KTI_SUCCESS;
}

/**
  SBSP programs bus numbers for fpgas.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

**/
KTI_STATUS
KtiFinalFpgaBusCfg (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8     Socket, Idx1;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((SocketData->Cpu[Socket].Valid == TRUE) && (SocketData->Cpu[Socket].SocType == SOCKET_TYPE_FPGA)) {
      //
      // Update common structure FpgaPresentBitMap to enable CSR access routines
      //
      KTI_HOST_OUT_PTR->FpgaPresentBitMap |= (1 << Socket);
      UpdateAndDisplayBusInformation (KtiInternalGlobal, GetSysCpuCsrAccessVar (), SocketData, FALSE);

      //
      // Update common structures w/ updated bus numbers for FPGA to be used in CSR access routines after SBSP reprograms self
      //
      SetSocketFirstBus (Socket, KTIVAR->CpuInfo[Socket].CpuRes.BusBase);
      SetSocketLastBus (Socket, KTIVAR->CpuInfo[Socket].CpuRes.BusLimit);
      KTI_HOST_OUT_PTR->mmCfgBaseH[Socket] = KTIVAR->CpuInfo[Socket].CpuRes.SegMmcfgBase.hi;
      KTI_HOST_OUT_PTR->mmCfgBaseL[Socket] = KTIVAR->CpuInfo[Socket].CpuRes.SegMmcfgBase.lo;
      KTI_HOST_OUT_PTR->segmentSocket[Socket] = KTIVAR->CpuInfo[Socket].CpuRes.PciSegment;

      for (Idx1 = 0; Idx1 < MAX_LOGIC_IIO_STACK; Idx1++) {
        SetStackBus (Socket, Idx1, KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Idx1].BusBase);
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  s5331840: Program the KTI IRQ Threshold for 4S ring or 8S

  @param KtiInternalGlobal - KTIRC internal variables.

  @retval VOID

**/
VOID
Init_IRQ_Threshold (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
)
{

  KTIVAR->OutRrqThreshold = 7;
}

/**
  Dump topology info to output device

  @param SocketData        - socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval VOID

**/
VOID
RouteTableDump (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
#if (KTI_DEBUG_LEVEL & KTI_DEBUG_INFO1)
  ChaKtiRouteTableDump (SocketData, KtiInternalGlobal);
  M2PcieRouteTableDump (SocketData, KtiInternalGlobal);
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n"));
#endif // (KTI_DEBUG_LEVEL & KTI_DEBUG_INFO1)
}

/**
  Set Root Node's VN for the route passed into this routine.

  @param[in,out] SocketData        Pointer to socket specific data
  @param[in] KtiInternalGlobal     KTIRC internal variables.
  @param[in] SocId                 Root socket ID
  @param[in] DestSocId             Endpoint processor socket ID
  @param[in] OutPorts              Array to save KTI ports of node
  @param[in] MaxPorts              Max ports number in array OutPorts
  @param[in] MultiLink             TRUE - Dual or triple or four links exists; FALSE - No Dual link nor triple link nor quad link
  @param[in] RootVn                Root socket's VN

  @retval KTI_SUCCESS  VN setting data structure is updated.
  @retval KTI_FAILURE  M3KTI index exceeds total supported M3KTI agents.

**/
KTI_STATUS
SetRootNodeVn (
  IN OUT KTI_SOCKET_DATA          *SocketData,
  IN     KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN     UINT8                     SocId,
  IN     UINT8                     DestSocId,
  IN     UINT8                     *OutPorts,
  IN     UINT8                     MaxPorts,
  IN     BOOLEAN                   MultiLink,
  IN     UINT8                     RootVn
  )
{
  M3KTI_VN_SEL  *M3KtiVnSel;
  UINT8         OutM3Kti;

  //
  // Calculate the VN setting for the root node. We use a scheme where the VN selection is done only
  // at the root (i.e local) node. At intermediate nodes the VN is not switched; messages are passed on the same
  // VN on which they arrived. The VN selection for the local traffic is obtained thru VN Lookup table (LUT)
  // stored in M3KIGRVNSEL register. Each entry in the table is 1 bit wide that selects VN0 (0) or VN1 (1).
  // The table is 8-entry deep whose index is calculated as follows:
  //
  //        {DNID[2:0]}
  //
  // Entries are used to select the VN for the traffic that originates in this (root) socket.
  // Since a root socket uses same VN for all DNIDs that it talks to, all 8 entries
  // will have same setting. For the same reason, all the ports will have same setting as well.
  // Note that even though a port may not be used to talk to all other sockets or not all the sockets are
  // populated, we calculate the VN setting assuming a port is used to talk to all possible sockets to
  // avoid variations in setting. Also since the h/w does the route lookup separate from VN lookup
  // and route table is set only for the valid sockets, this is ok.
  //
  if (KTIVAR->SharedKtiPort0And1) {
    OutM3Kti = PORT_TO_M3KTI (OutPorts[0]);
  } else {
    OutM3Kti = OutPorts[0];
  }
  if (OutM3Kti >= MAX_M3KTI) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\nERR_SW_CHECK: 0x%X!!!!  Invalid value. Outport1: 0x%X is greater or equal to MAX_M3KTI: 0x%X",
                         MINOR_ERR_SW_CHECK_50, OutPorts[0], MAX_M3KTI));
    KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_50);
  }

  M3KtiVnSel =  &(SocketData->Route[SocId].M3KtiVnSel[OutM3Kti]);
  (RootVn == VN0) ? (M3KtiVnSel->IgrVnSel = 0x00000000) : (M3KtiVnSel->IgrVnSel = 0x000000FF);

  return KTI_SUCCESS;
}

/**
  Set Root Node Route and its VN

  @param[in,out] SocketData        Pointer to socket specific data
  @param[in] KtiInternalGlobal     KTIRC internal variables.
  @param[in] SocId                 Root socket ID
  @param[in] IntermediateSocId     Intermediate node ID (the node is either the endpoint itself or connected directly to RootNode)
  @param[in] DestSocId             Endpoint processor socket ID
  @param[in] OutPorts              Array to save KTI ports of node
  @param[in] OutPortsNumber        Ports number in array OutPorts
  @param[in] MultiLink             TRUE - Dual or triple or four links exists; FALSE - No Dual link nor triple link nor quad link
  @param[in] AlternateRoute        TRUE indicates there is second route between Root Node (SocId) and Destination
                                   Node (DestSocId); therefore update the DestSocId's second entry with this route.
                                   FALSE otherwise.
  @param[in] RootVn                Root socket's VN

  @retval KTI_SUCCESS  CHA & M2PCIE RT and VN settings data structures are updated.
  @retval KTI_FAILURE  Invalid OutPort1 and OutPort2 port ID. No data structures are updated.

**/
KTI_STATUS
SetRootNodeRoute (
  IN OUT KTI_SOCKET_DATA          *SocketData,
  IN     KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN     UINT8                     SocId,
  IN     UINT8                     IntermediateSocId,
  IN     UINT8                     DestSocId,
  IN     UINT8                     *OutPorts,
  IN     UINT8                     OutPortsNumber,
  IN     BOOLEAN                   MultiLink,
  IN     BOOLEAN                   AlternateRoute,
  IN     UINT8                     RootVn
  )
{
  UINT8         PortFieldWidth;
  UINT8         KtiPortCount;

  KtiPortCount = GetKtiPortCnt ();
  if ((OutPorts[0] >= KtiPortCount) || (OutPorts[1] >= KtiPortCount)) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "Invalid value of KTI port of node"));
    return KTI_FAILURE;
  }

  //
  //  Set the RT in Cha & M2PCIe. M3KTI & KTIAgent RT needs to set for pass-thru route only.
  //  For all non-dual link topologies, the route setting is straight forward. Cha and M2PCIe
  //  route tables are configured to use the only link that was calculated in discovery process.
  //
  //  If dual link exists between root node and immediate neighbor, need to balance load on them.
  //  Dual link can exist in 2S config only. The load balancing needs to be done for CHA/M2MEM only.
  //  It is not needed for M2PCIe since it is set to operate in a mode where it sends
  //  all the responses back on the same port on which the request was received which will balance
  //  the load on the links (R2PGNCTRL.qpi_rt_sel_ndr_drs=0). However M2PCIe requests always use
  //  route table and they can select only one link based on route table so the M2PCIe requests are not
  //  balanced out. (Only Ubox initiated requests; IIO initiated requests such as DMA, P2P are serviced
  //  by Cha based on address hashing. Cha will use balanced routing).
  //
  //  Cha is configured to balance the load on the links based on P6 ^ P12.  It is no longer based on Cha ID.
  //  CBO_COH_CONFIG.enablexorbasedktilink. When this field is set, the Cha selects the link as follows:
  //
  //       Link = (PA6 ^ PA12)
  //
  //  For configs with port0<->port1, port1<->port0 mapping, socket 0 Chas are configured as above, but socket 1
  //  Chas will invert the port selection by setting this bit:
  //  CBO_COH_CONFIG.swapktilinkintlv = 1
  //
  //  This interleaving config described above will ensure that the responses always come back on
  //  the same port for Cha initiated requests/snoops. This esnures all flavours of 2S 2KTI configs
  //  benifit from D2C.
  //
  //  If TwoSkt3Link case, SAD_TARGET_CHABC_SAD1_REG.enablemodulo3basedktilink need to be set to
  //  Use Mod3(PA) value to decide the KTI link to use.
  //
  //  Note: We calculate M2PCIe & Cha route as well for simplicity to match other configs.
  //

  PortFieldWidth = 2; // Two bit field to encode link 0, 1 & 2

  //
  // M2PCIe Route
  //
  SetupM2PcieUpiRt (SocketData, KtiInternalGlobal, SocId, DestSocId, OutPorts, OutPortsNumber, PortFieldWidth, MultiLink, AlternateRoute);

  SetRootNodeVn (
    SocketData,
    KtiInternalGlobal,
    SocId,
    DestSocId,
    OutPorts,
    OutPortsNumber,
    MultiLink,
    RootVn
    );

  //
  // Cha Route
  //
  SetRootNodeRouteInChaKtiRouteTable (
    SocketData,
    KtiInternalGlobal,
    SocId,
    IntermediateSocId,
    DestSocId,
    OutPorts,
    OutPortsNumber,
    MultiLink,
    AlternateRoute
    );

  return KTI_SUCCESS;
}

/**
  Sets up pass-through route on a socket

  @param SocketData - Pointer to socket specific data
  @param SocId      - Pass-thru socket ID
  @param DestSocId  - endpoint processor socket ID
  @param InPort     - pass-thru port towards the root
  @param OutPort    - pass-thru port towards the destination
  @param RootVn     - Root socket's VN

  @retval 0 - Successful
  @retval 1 - Failure

**/
KTI_STATUS
SetPassThruRoute (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    SocId,
  UINT8                    DestSocId,
  UINT8                    InPort,
  UINT8                    OutPort,
  UINT8                    RootVn
  )
{
  UINT8  M3KtiNum;
  UINT32 EncodedRoute = 0;

  //
  // set route table in M3KTI
  //
  if (KTIVAR->SharedKtiPort0And1 == TRUE) {
    M3KtiNum = PORT_TO_M3KTI (InPort);
  } else {
    M3KtiNum = InPort;
  }
  EncodedRoute = SocketData->Route[SocId].M3KtiRt[M3KtiNum];
  ConvertM3KTIRoutingTable (SocId, InPort, OutPort, DestSocId, &EncodedRoute, KTIVAR->SharedKtiPort0And1);
  SocketData->Route[SocId].M3KtiRt[M3KtiNum] = EncodedRoute;

  //
  // set route table in KTI Agent
  //
  EncodedRoute = SocketData->Route[SocId].KtiAgentRt[InPort];
  EncodedRoute &= (~(3 << (DestSocId * 2)));
  EncodedRoute |= (OutPort << (DestSocId * 2));
  SocketData->Route[SocId].KtiAgentRt[InPort] = EncodedRoute;

  return KTI_SUCCESS;
}

/**
  Mask address parity error for P2P transactions

  @param[in] KtiInternalGlobal     KTIRC internal variables.
  @param[in] SocId                 Root socket ID

**/
VOID
P2PTransactionWa (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    SocId
  )
{
  UINT8     Port;

  if ((KTIVAR->RasInProgress == FALSE && (KtiInternalGlobal->CpuList & (1 << SocId))) ||
    (KTIVAR->RasInProgress == TRUE && KTIVAR->RasSocId == SocId)) {
    //
    // WA for P2P transactions error
    //
    if (IsCpuAndRevision (CPU_ICXSP, REV_R0)) {
      for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
        if (KTIVAR->CpuInfo[SocId].LepInfo[Port].Valid == TRUE) {
          MaskP2PTxnsErr(SocId, Port);
        }
      }
    }
  }
}

/**
  Returns boolean if it is an internal UPI port for given socket ID and link index

  @param KtiInternalGlobal - KTIRC internal variables.
  @param Soc               - Socket ID.
  @param LinkIndex         - Link index value

  @retval TRUE             - UPI port is internal
  @retval FALSE            - UPI port is not internal

**/
BOOLEAN
IsInternalUpiPort (
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN UINT8                    Soc,
  IN UINT8                    LinkIndex
  )
{
  return FALSE;
}
/**
  Get internal Eparameter table information.

  @param [in, out]AllLanesPtr             - Pointer of the pointer to all lane table.
  @param [in, out]AllLanesSizeOfTable     - Pointer to all lane table size.
  @param [in, out]PerLanePtr              - Pointer of the pointer to per lane table.
  @param [in, out]PerLaneSizeOfTable      - Pointer to per lane table size.

  @retval KTI_SUCCESS                     - Get internal Eparameter table successfully.
  @retval KTI_FAILURE                     - There is no proper eparameter table .
**/

KTI_STATUS
GetInternalEparameterTable (
  ALL_LANES_EPARAM_LINK_INFO  **AllLanesPtr,
  UINT32                      *AllLanesSizeOfTable,
  PER_LANE_EPARAM_LINK_INFO   **PerLanePtr,
  UINT32                      *PerLaneSizeOfTable
  )
{
  return KTI_FAILURE;
}

//
// Kti Transition Full speed without warm reset is not supported in ICX
//
KTI_STATUS
KtiSpeedTransitionMiscSteps (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8                     SpeedMode
  )
{
  return KTI_IGNORE;
}

#if defined (PPO_FLAG)
/**
  Checks the link in slow mode wa request the warm reset or not

  @param SocketData        - add argument and description to function comment
  @param KtiInternalGlobal - KTIRC internal variables.

**/
VOID
UpiSlowModeWaSkx (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  return;
}
#endif // defined (PPO_FLAG)

/**
  This routine assigns UPI port to each MS2IOSF source based on affinity.

  It affinitizes M2IOSF sources to UPI port on the same cap, if that option
  exists. That is M2IOSFs on the northcap of the die divide remote P2P traffic
  among UPIs enabled in the northcap and M2IOSFs in the southcap of the dies
  send traffic to UPI on the southcap if that UPI is enabled.

  @param[in,out] SocketData        Pointer to socket specific data
  @param[in] KtiInternalGlobal     KTIRC internal variables.
  @param[in] RootSocId             Root Node node ID
  @param[in] DestSocId             Destination Node node ID
  @param[in] OutPorts              Array that stores KTI ports of Root Node
  @param[in] TotalActiveLinks      Number of enabled UPI links between Root Node and Destination Node
  @param[in] PortFieldWidth        Port field width; this is SoC specific.

  @retval KTI_SUCCESS           M2PcieKtiRt tables are updated with valid routing based on affinity.
  @retval KTI_IGNORE            Affinity assignment is not performed.

**/
KTI_STATUS
AffinitizeM2IosfSourceToUpiPort (
  IN OUT KTI_SOCKET_DATA          *SocketData,
  IN     KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN     UINT8                     RootSocId,
  IN     UINT8                     DestSocId,
  IN     UINT8                     *OutPorts,
  IN     UINT8                     TotalActiveLinks,
  IN     UINT8                     PortFieldWidth
  )
{
  UINT8         Stack;
  UINT8         i;
  UINT8         j;
  UINT32        FieldMask;
  UINT8         NorthCapUpi[MAX_FW_KTI_PORTS];
  UINT8         SouthCapUpi[MAX_FW_KTI_PORTS];
  UINT8         TotalNorthCapUpi;
  UINT8         TotalSouthCapUpi;

  TotalNorthCapUpi = 0;
  TotalSouthCapUpi = 0;
  ZeroMem ((VOID *) NorthCapUpi, sizeof (NorthCapUpi));
  ZeroMem ((VOID *) SouthCapUpi, sizeof (SouthCapUpi));

  for (i = 0; i < TotalActiveLinks; i++) {
    if (KTI_INVALID_PORT == OutPorts[i]) {
      continue;
    }

    if ((OutPorts[i] & BIT0) == 0) {
      //
      // For ICX, UPI ports in North Cap are assigned with even Port ID number
      //
      NorthCapUpi[TotalNorthCapUpi++] = OutPorts[i];
    } else {
      SouthCapUpi[TotalSouthCapUpi++] = OutPorts[i];
    }
  }

  if (TotalNorthCapUpi == 0) {
    //
    // There is at least one UPI Port in north cap for ICX for 2S multi-link. Skip affinity assignment.
    //
    return KTI_IGNORE;
  }

  FieldMask = FLD_MASK (PortFieldWidth, (PortFieldWidth * DestSocId));

  for (Stack = 0, i = 0, j = 0; Stack < MAX_IIO_STACK; Stack++) {
    if ((KtiInternalGlobal->M2PcieActiveBitmap[RootSocId] & (1 << Stack)) == 0) {
      continue;
    }

    SocketData->Route[RootSocId].M2PcieKtiRt[Stack] &= ~FieldMask;

    if ((IcxMs2iosfSourceLocation[Stack] == SOUTH_CAP_MS2IOSF) && (TotalSouthCapUpi != 0)) {
      SocketData->Route[RootSocId].M2PcieKtiRt[Stack] |= (UINT32) (SouthCapUpi[i] << (PortFieldWidth * DestSocId));
      if (++i >= TotalSouthCapUpi) {
        //
        // restart from SouthCapUpi[0]
        //
        i = 0;
      }
      continue;
    }

    //
    // Either there is no active UPI port in South Cap, or this is North Cap MS2IOSF source
    //
    SocketData->Route[RootSocId].M2PcieKtiRt[Stack] |= (UINT32) (NorthCapUpi[j] << (PortFieldWidth * DestSocId));

    if (++j >= TotalNorthCapUpi) {
      //
      // restart from NorthCapUpi[0]
      //
      j = 0;
    }
  }

  return KTI_SUCCESS;
}

/**
  This routine checks if the requested UPI link requires opposite VN of what was assigned
  to its Root Node.

  This routine is currently used to handle the special VN assignment requirement for
  8-socket 4-Link (Twisted cubes fully connect cubes) topology in SPR. To guarantee
  better QoS for the Route-Through traffic, the source traffic uses different VN as
  the route-thru traffic at a given socket. If not, the source traffic might starve
  the route-thru traffic as the source count (i.e CHA/Core) is increasing gen-to-gen.
  Using the following socket mapping as an example:
      +-----------------------+
      |                       |       VN Assignment:
      S0 ---- S1     S6 ---- S7       ===============
      | \   / | \   / | \   / |       VN0: S0, S2, S4, S6
      |  \ /  |  \ /  |  \ /  |       VN1: S1, S3, S5, S7
      |   \   |   \   |   \   |
      |  / \  |  / \  |  / \  |
      | /   \ | /   \ | /   \ |
      S2 ---- S3     S4 ---- S5
      |                       |
      +-----------------------+
  To meet the VN assignment requirement, the following VN assignment should be used:
    VN0: S0, S2, S4, S6
    VN1: S1, S3, S5, S7
  In addition, UPI Port 2 traffic in the above example should use VN that is opposite
  of the above VN assignment to guarantee that the Source traffic doesn't use same VN
  as the pass-thru traffic.

  @param[in] SocketData            Pointer to socket specific data
  @param[in] KtiInternalGlobal     KTIRC internal variables
  @param[in] SocId                 Socket ID to check for
  @param[in] PortId                UPI port number

  @retval TRUE   This UPI Port should use VN that is opposite of the VN assigned to this Socket
  @retval FALSE  This UPI Port uses the same VN as other ports of this Socket

**/
BOOLEAN
IsUpiPortWithOppositeRootVn (
  IN  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN  KTI_CPU_SOCKET_DATA      *SocketData,
  IN  UINT8                    SocId,
  IN  UINT8                    PortId
  )
{
  return FALSE;
}

/**
  This routine checks if adaptation needs to be executed or not. It also updates the Adaptation status scratch register
  with the outcome. Executing the Adaptation engine is necessary only if all the populated sockets support
  adaptation and at least one of the following is TRUE:

      - CPU parts on the platform are changed/swapped
      - A valid adaptation table is not found
      - A valid adaptation table is found but the table entry for the requested adaptation speed is not found

  Note: ICX & SNR & ICX-D doesn't support Txeq Adaptation

  @param KtiInternalGlobal - KTIRC internal variables.

  @retval BOOLEAN - TRUE  - if adaptation needed; FALSE - if not adaptation required

**/
BOOLEAN
CheckIfAdaptationNeeded (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  return FALSE;
}

/**
  Programs txeq adaptation parameter for the link
  Note: ICX & SNR & ICX-D doesn't support Txeq Adaptation

  @param KtiInternalGlobal - KTIRC internal variables.
  @param Socket            - SocId we are looking up
  @param LinkIndex         - Link# on socket

  @retval Status - return KTI_SUCCESS on successful programming
                          KTI_IGNORE - Not supported

**/
KTI_STATUS
KtiProgramTxeqAdaptParams (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Socket,
  UINT8                      LinkIndex
  )
{
  return KTI_IGNORE;
}

/**
  Check if there is next adaptation speed need to be adapted, or it is already finished.
  If it is finished, clears the TxAdaptEn bits for links.
  Otherwise, get the next adaptation speed and do the related steps (it is in without warm reset flow).

  Note: ICX & SNR & ICX-D doesn't support Txeq Adaptation

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval Status - return KTI_SUCCESS on successful programming
                          KTI_IGNORE - Not supported

**/
KTI_STATUS
CheckAndExecuteKtiAdaptation (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  return KTI_IGNORE;
}

/**
  This routine is used in topology (such as 8S double pinwheel) that involves
  multiple ports that can be used to configure the route through traffic.

  Based on topology specific criteria, this routine will update the correct
  UPI port to be used as the outgoing port on route through socket.

  @param[in] KtiInternalGlobal     KTIRC internal variables.
  @param[in] SocketData            Pointer to socket specific data
  @param[in] RouteThruNodeId       Route Through node ID (or intermediate node)
  @param[in] RtPortIn              Incoming UPI port on Route Through Node
  @param[in,out] RtPortOut         Pointer to store outgoing UPI Port on Route Through Node

  @retval KTI_SUCCESS   RtPortOut contains the final outgoing UPI Port to be used for route-through.
  @retval KTI_IGNORE    Customization of outgoing route-through UPI port is NOT required.
  @retval KTI_FAILURE   Route through incoming and/or outgoing ports passed into this function are invalid.

**/
KTI_STATUS
SelectOutportForOptimizedPassThruRoute (
  IN     KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN     KTI_SOCKET_DATA            *SocketData,
  IN     UINT8                      RouteThruNodeId,
  IN     UINT8                      RtPortIn,
  IN OUT UINT8                      *RtPortOut
  )
{
  return KTI_IGNORE;
}

/**
  There are two types of UPI routing tables: 16-entry route table, or 8-entry table.

  This routine returns TRUE if the SOC supports 16-entry route table. Else, it returns
  FALSE. SOC that supports more than 3 UPI links implements 16-entry route table.

  @retval  TRUE if 16-entry UPI route table is supported. FALSE otherwise.

**/
BOOLEAN
Is16EntryUpiRoutingTableSupported (
  VOID
  )
{
  //
  // Return FALSE as ICX supports only 8-entry UPI routing table
  //
  return FALSE;
}
