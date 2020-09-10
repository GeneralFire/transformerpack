/** @file
  ACPI Platform Driver Hooks

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2020 Intel Corporation. <BR>

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

//
// Statements that include other files
//
#include "AcpiPlatformLibLocal.h"
#include "AcpiCommonServices.h"

#define CPM_MMIO_SIZE           0x100000000         // 4G MMIO resource for CPM
#define HQM_MMIO_SIZE           0x400000000         // 16G MMIO resource for HQM

extern BIOS_ACPI_PARAM             *mAcpiParameter;
extern struct SystemMemoryMapHob   *mSystemMemoryMap;
extern EFI_IIO_UDS_PROTOCOL        *mIioUds;
extern CPU_CSR_ACCESS_VAR          *mCpuCsrAccessVarPtr;

extern SOCKET_MP_LINK_CONFIGURATION  mSocketMpLinkConfiguration;
extern SOCKET_IIO_CONFIGURATION     mSocketIioConfiguration;
extern SOCKET_POWERMANAGEMENT_CONFIGURATION mSocketPowermanagementConfiguration;

extern BOOLEAN      mCpuOrderSorted;
extern UINT32       mApicIdMap[MAX_SOCKET][MAX_CORE * MAX_THREAD];
extern UINT32       mNumOfBitShift;
extern EFI_CPU_ID_ORDER_MAP         mCpuApicIdOrderTable[MAX_CPU_NUM];
extern BOOLEAN                      Is14nmCpu;


AML_OFFSET_TABLE_ENTRY            *mAmlOffsetTablePointer = NULL;

//
// iASL generates an offset table .h file which contains offsets from the start of DSDT AML for the following items:
//   Name, OperationRegion, WORDBusNumber, DWORDMemory, QWORDMemory, WORDIO
// The last 4 items must populate the last field (DescriptorName) for iASL to put them in the offset table.
//
/*
typedef struct {
    CHAR8                  *Pathname;
    CHAR8                  *FixTag;
} BIOS_FIXUP_AML_OFFSET_TABLE_ENTRY;

//
// BIOS fixup table for Purley platform
//
BIOS_FIXUP_AML_OFFSET_TABLE_ENTRY BiosFixupAmlOffsetTable[] =
{
    //
    // Cast AML_RESOURCE_ADDRESS16 for WORDBusNumber operator so AML_OFFSET_TABLE_ENTRY.offset is at value to change.
    // Cast AML_RESOURCE_ADDRESS16 for WORDIO operator so AML_OFFSET_TABLE_ENTRY.offset is at value to change.
    // Cast AML_RESOURCE_ADDRESS32 for DWORDMemory operator so AML_OFFSET_TABLE_ENTRY.offset is at value to change.
    // Cast AML_RESOURCE_ADDRESS64 for QWORDMemory operator so AML_OFFSET_TABLE_ENTRY.offset is at value to change.
    // Name operator AML_OFFSET_TABLE_ENTRY.offset is at value to change.
    // OperationRegion AML_OFFSET_TABLE_ENTRY.operator offset is at value to change.
    //
    {"PSYS",                "FIX0"}, // OperationRegion() in Acpi\AcpiTables\Dsdt\CommonPlatform.asi
    {"_SB_.PCXX.FIX1",      "FIX1"}, // WORDBusNumber()   in Acpi\AcpiTables\Dsdt\PC00.asi (for all root bridges)
    {"_SB_.PCXX.FIX2",      "FIX2"}, // WORDIO()          in Acpi\AcpiTables\Dsdt\PC00.asi (for all root bridges)
    {"_SB_.PCXX.FIX3",      "FIX3"}, // DWORDMemory()     in Acpi\AcpiTables\Dsdt\PC00.asi (for all root bridges)
    {"_SB_.PCXX.FIX4",      "FIX4"}, // QWORDMemory()     in Acpi\AcpiTables\Dsdt\PC00.asi (for all root bridges)
    {"_SB_.PCXX.FIX5",      "FIX5"}, // DWORDMemory()     in Acpi\AcpiTables\Dsdt\PC00.asi (for all root bridges)
    {"_SB_.PCXX.FIX6",      "FIX6"}, // WORDIO()          in Acpi\AcpiTables\Dsdt\PC00.asi (for all root bridges)
    {"_SB_.PCXX.FIX7",      "FIX7"}, // WORDIO()          in Acpi\AcpiTables\Dsdt\PC00.asi (for all root bridges)
    {"_SB_.PCXX.MCTL",      "FIX8"}, // OperationRegion() in Acpi\AcpiTables\Dsdt\PcieHp.asi (for all root ports)
    {"_SB_.UNXX.FIX9",      "FIX9"}, // WORDBusNumber()   in Acpi\AcpiTables\Dsdt\Uncore.asi (for all uncore stacks)
    {0, 0}                           // table terminator
};
*/

/**

    Check current thread status

    @param ApicId - current thread ApicId

    @retval EFI_SUCCESS     Returns Success if current thread is active
    @retval EFI_UNSUPPORTED Table is not supported

**/
EFI_STATUS
CheckCurrentThreadStatus (
  UINT32 ApicId
  )
{
  UINT32 Index;

  for (Index = 0; Index < MAX_CPU_NUM; Index++) {
    if ((mCpuApicIdOrderTable[Index].Flags == 1) && (mCpuApicIdOrderTable[Index].ApicId == ApicId)) {
      return EFI_SUCCESS;
    }
  }
  return EFI_UNSUPPORTED;
}


/**
  Get socket, stack and optionaly port index from PCI device path.

  The PCI device path is typically:
  '_SB_.PCxy.FIXz' for PCIe stack object
  '_SB_.UCxy.FIXz' for UBOX stack object
  '_SB_.PCxy.RPya' for PCIe bridge root port object
  where x and y are hex digits, and 'a' is a letter like 'A', 'B',..,'H'.

  NOTE: Currently SPR define xy as hex number where 'x' is socket, 'y' is stack.
        In other projects 'xy' is decimal number of subsequent PCIe stack, not including UBOX.
        For UBOX UCxy the SPR convention is used, 'x' is socket, 'y' is stack.

  @param[in]  DevPathPtr - PCI device path, e.g. '_SB_.PC00.FIX1'
  @param[out] SocketPtr  - Buffer for socket index.
  @param[out] StackPtr   - Buffer for stack index.
  @param[out] PortkPtr   - Buffer for port index.
**/
EFI_STATUS
AcpiPciDevPath2SktStkPort (
  IN  CHAR8  *DevPathPtr,
  OUT UINT8  *SocketPtr,
  OUT UINT8  *StackPtr,
  OUT UINT8  *PortPtr
  )
{
  UINT16 SysStackNo;
  UINT8  SocketNo = 0xFF;
  UINT8  StackNo = 0xFF;
  UINT8  PortNo = 0xFF;

  if (PortPtr != NULL) {
    //
    // Device path should contain bridge root port object, let's verify.
    //
    if (AsciiStrLen (DevPathPtr) < 3*4 + 2 ||
        DevPathPtr[10]  != 'R' || DevPathPtr[12]  != 'P' || DevPathPtr[13]  < 'A' || DevPathPtr[13]  > 'H') {

      goto ErrExit;
    }
    PortNo = DevPathPtr[13] - 'A';
  }
  if (AsciiStrLen (DevPathPtr) < 2*4 + 1 || DevPathPtr[7]  < '0' || DevPathPtr[8]  < '0') {

    goto ErrExit;
  }
  switch (DevPathPtr[5] << 8 | DevPathPtr[6]) {

    case ('P' << 8 | 'C'):
      if (!IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {

        if (DevPathPtr[7] > '9' || DevPathPtr[8] > '9') {

          goto ErrExit;
        }
        SysStackNo = (DevPathPtr[7] - '0') * 10;
        SysStackNo += DevPathPtr[8] - '0';
        SocketNo = (UINT8)(SysStackNo / MAX_IIO_STACK);
        StackNo = (UINT8)(SysStackNo % MAX_IIO_STACK);
        break;
      }
      //
      // For SPR fall through to 'UC' and read socekt and stack as hex digits.
      //
    case ('U' << 8 | 'C'):
      if (DevPathPtr[7] <= '9') {

        SocketNo = DevPathPtr[7] - '0';

      } else if (DevPathPtr[7] <= 'F') {

        if (DevPathPtr[7] < 'A') {

          goto ErrExit;
        }
        SocketNo = 10 + DevPathPtr[7] - 'A';

      } else if (DevPathPtr[7] <= 'f') {

        if (DevPathPtr[7] < 'a') {

          goto ErrExit;
        }
        SocketNo = 10 + DevPathPtr[7] - 'a';

      } else {
        goto ErrExit;
      }
      if (DevPathPtr[8] <= '9') {

        StackNo = DevPathPtr[8] - '0';

      } else if (DevPathPtr[8] <= 'F') {

        if (DevPathPtr[8] < 'A') {

          goto ErrExit;
        }
        StackNo = 10 + DevPathPtr[8] - 'A';

      } else {
        goto ErrExit;
      }
      break;

    default:
    ErrExit:
      DEBUG ((DEBUG_INFO, "[ACPI] ERROR: String '%a' is not valid PCI stack name, ", DevPathPtr));
      DEBUG ((DEBUG_INFO, "expect _SB_.PCxy.FIXz, or _SB_.UCxv.FIXz, or _SB_.PCxy.RPya\n"));
      return EFI_NOT_FOUND;
  }
  if (SocketPtr != NULL) {
    *SocketPtr = SocketNo;
  }
  if (StackPtr != NULL) {
    *StackPtr = StackNo;
  }
  if (PortPtr != NULL) {
    *PortPtr = PortNo;
  }
  return EFI_SUCCESS;
}


/**
  Update the DSDT table

  @param[in,out] *Table   - The table to be set

  @retval EFI_SUCCESS - DSDT updated
  @retval EFI_INVALID_PARAMETER - DSDT not updated
**/
EFI_STATUS
PatchDsdtTable (
  IN OUT EFI_ACPI_COMMON_HEADER   *Table
  )
{
  EFI_STATUS Status;
  UINT8   *DsdtPointer;
  UINT32  *Signature;
  UINT32  *Signature2;
  UINT32  Fixes;
  UINT32  NodeIndex;
  UINT8   Counter;
  UINT16  i;  // DSDT_PLATEXRP_OffsetTable LUT entries extends beyond 256!
  UINT8   SktFPGA = 0;
  UINT8   SktFpgaKti = 0;
  UINT8   BusBase = 0, BusLimit = 0;
  UINT16  IoBase  = 0, IoLimit  = 0;
  UINT32  MemBase32 = 0, MemLimit32 = 0;
  UINT64  MemBase64 = 0, MemLimit64 = 0;
  AML_RESOURCE_ADDRESS16 *AmlResourceAddress16Pointer;
  AML_RESOURCE_ADDRESS32 *AmlResourceAddress32Pointer;
  AML_RESOURCE_ADDRESS64 *AmlResourceAddress64Pointer;
  EFI_ACPI_DESCRIPTION_HEADER   *TableHeader;
  UINT32 AdjustSize = 0, CpuSkt = 0, CpuIndex = 0;
  EFI_ACPI_NAMEPACK_DWORD     *NamePtr;
  UINT8   *CurrPtr;
  UINT8   *EndPtr;
  FPGA_CONFIGURATION FpgaConfiguration;
  BOOLEAN            FpgaConfigurationSupported = FALSE;
  const UINT32      *ApicMapPtr;
  UINT8              Socket;
  UINT8              Stack;
  UINT8              UboxStack;
  IIO_STACK          *IioStack;

  TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *)Table;

  if (mAmlOffsetTablePointer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  mAcpiParameter->SocketBitMask = mCpuCsrAccessVarPtr->socketPresentBitMap;
  IioStack = AllocateZeroPool(sizeof(IIO_STACK));
  ZeroMem (&FpgaConfiguration, sizeof (FPGA_CONFIGURATION));

  Status = FpgaConfigurationGetValues (&FpgaConfiguration);
  if (EFI_ERROR (Status) && Status != EFI_UNSUPPORTED) {
    ASSERT_EFI_ERROR (Status);
  }
  FpgaConfigurationSupported = (Status == EFI_SUCCESS) ? TRUE : FALSE;

  if (Is14nmCpu) {
    mAcpiParameter->FpgaKtiPresent = mCpuCsrAccessVarPtr->FpgaPresentBitMap;
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
   if (Is14nmCpu) {
      //
      // Update the Bus number for FPGA KTI link
      //
      if (mCpuCsrAccessVarPtr->FpgaPresentBitMap & (1 << Socket)){
        mAcpiParameter->FpgaKtiBase[Socket] = mCpuCsrAccessVarPtr->SocketFirstBus[Socket];
        DEBUG ((EFI_D_INFO, "[ACPI](DSDT) [%d] FPGA BusBase: 0x%02X\n", Socket, mAcpiParameter->FpgaKtiBase[Socket]));
      }
    }
    if (!mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].Valid) {
      mAcpiParameter->IioPresentBitMask[Socket] = 0;
      continue;
    }
    mAcpiParameter->IioPresentBitMask[Socket] = mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap;
    mAcpiParameter->IioCxlPresentBitMap[Socket] = mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].CxlPresentBitmap;
    DEBUG ((EFI_D_INFO, "[ACPI](DSDT) [%d] CxlBitMap: 0x%0x\n", Socket, mAcpiParameter->IioCxlPresentBitMap[Socket]));
    for (Stack = 0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {

      if (IsCpuAndRevision (CPU_SNR, REV_ALL) && Stack == IIO_STACK5) {
        mAcpiParameter->BusBase[Socket][Stack] = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[IIO_STACK2].BusLimit - V_IOV_RESERVED_BUS_NUMBER;
      } else {
        mAcpiParameter->BusBase[Socket][Stack] = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].BusBase;
      }
      DEBUG ((DEBUG_INFO, "[ACPI](DSDT) [%d.%d] BusBase: 0x%02X\n", Socket, Stack, mAcpiParameter->BusBase[Socket][Stack]));
    }
    if (Is14nmCpu) {
      //
      // Update the Bus number for FPGA PCIE RP0
      //
      if (FpgaConfigurationSupported && (FpgaConfiguration.FpgaSktActive & (1 << Socket))) {

        mAcpiParameter->FpgaBusBase[Socket] = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[IIO_PSTACK3].BusLimit - FPGA_PREAllOCATE_BUS_NUM + 0x01;
        mAcpiParameter->FpgaBusLimit[Socket] = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[IIO_PSTACK3].BusLimit;
        DEBUG ((EFI_D_INFO, "[ACPI](DSDT) [%d] FPGA BUS: 0x%02X\n", Socket, mAcpiParameter->FpgaBusBase[Socket]));
      }
    }
  } // for (Socket...)

  //
  // Update IIO PCIe Root Port PCIe Capability offset
  // for 14nm process CPUs and SNR with integrated PCIe GEN3 controller, PCIe Capability offset is at 0x90
  // for 10nm process CPUs with PCIe GEN4/GEN5 controller, PCIe Capability offset is at 0x40
  //
  if (Is14nmCpu || IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    mAcpiParameter->IioPcieRpCapOffset = 0x90;
  } else {
    mAcpiParameter->IioPcieRpCapOffset = 0x40;
  }
  //
  // Initialize TsegSize - 1MB aligned.
  //
  Fixes = 0;
  //
  // Loop through the AML looking for values that we must fix up.
  //
  for (i = 0; mAmlOffsetTablePointer[i].Pathname != 0; i++) {
    //
    // Point to offset in DSDT for current item in AmlOffsetTable.
    //
    DsdtPointer = (UINT8 *) (TableHeader) + mAmlOffsetTablePointer[i].Offset;

    if (mAmlOffsetTablePointer[i].Opcode == AML_DWORD_PREFIX) {
      //
      // If Opcode is 0x0C, then operator is Name() or OperationRegion().
      // (TableHeader + AmlOffsetTable.Offset) is at offset for value to change.
      //
      // The assert below confirms that AML structure matches the offsets table.
      // If not then patching the AML would just corrupt it and result in OS failure.
      // If you encounter this assert something went wrong in *.offset.h files
      // generation. Remove the files and rebuild.
      //
      ASSERT(DsdtPointer[-1] == mAmlOffsetTablePointer[i].Opcode);
      //
      // AmlOffsetTable.Value has FIX tag, so check that to decide what to modify.
      //
      Signature = (UINT32 *) (&mAmlOffsetTablePointer[i].Value);
      switch (*Signature) {
        //
        // Due to iASL compiler change and DSDT patch design change, if these items need support
        // then the ASI files will need to conform to the format requires for iASL to add the items
        // to the offset table, and grt.bat will need to filter them out when iASL is executed.
        //
        // "FIX0" OperationRegion() in Acpi\AcpiTables\Dsdt\CommonPlatform.asi
        //
        case (SIGNATURE_32 ('F', 'I', 'X', '0')):
          *(UINT32*)DsdtPointer = (UINT32)(UINTN)mAcpiParameter;
          Fixes++;
          break;
        default:
          DEBUG ((DEBUG_ERROR, "[ACPI](DSDT) WARNING: Object '%a' with opcode 0x%02X not patched\n",
                  mAmlOffsetTablePointer[i].Pathname, mAmlOffsetTablePointer[i].Opcode));
          break;
      }
    } else if (mAmlOffsetTablePointer[i].Opcode == AML_INDEX_OP) {
      //
      // If Opcode is 0x88, then operator is WORDBusNumber() or WORDIO().
      // (TableHeader + AmlOffsetTable.Offset) must be cast to AML_RESOURCE_ADDRESS16 to change values.
      //
      AmlResourceAddress16Pointer = (AML_RESOURCE_ADDRESS16 *) (DsdtPointer);
      //
      // The assert below confirms that AML structure matches the offsets table.
      // If not then patching the AML would just corrupt it and result in OS failure.
      // If you encounter this assert something went wrong in *.offset.h files
      // generation. Remove the files and rebuild.
      //
      ASSERT(AmlResourceAddress16Pointer->DescriptorType == mAmlOffsetTablePointer[i].Opcode);

      //
      // Last 4 chars of AmlOffsetTable.Pathname has FIX tag.
      //
      Signature = (UINT32 *) (mAmlOffsetTablePointer[i].Pathname + AsciiStrLen(mAmlOffsetTablePointer[i].Pathname) - 4);
      Signature2 = (UINT32 *) (mAmlOffsetTablePointer[i].Pathname + AsciiStrLen(mAmlOffsetTablePointer[i].Pathname) - 9);
      switch (*Signature) {
        //
        // "FIX1" BUS resource for PCXX in Acpi\AcpiTables\Dsdt\SysBus.asi and PCXX.asi
        //
        case (SIGNATURE_32 ('F', 'I', 'X', '1')):
          if (IsStackInCxlMode(Socket, Stack) == TRUE) {
            //
            // Stack in CXL mode
            //
            Status = AcpiPciDevPath2SktStkPort(mAmlOffsetTablePointer[i].Pathname, &Socket, &Stack, NULL);

            if (EFI_ERROR(Status)) {
              if (IsCxlAcpiDevicePath (mAmlOffsetTablePointer[i].Pathname) == TRUE) {
                //
                // CXL acpi device is found
                //
                Status = GetCxlStackAttributes (mAmlOffsetTablePointer[i].Pathname, IioStack);
                if (!EFI_ERROR(Status)) {
                  BusBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioStack->SocketId].StackRes[IioStack->StackId].PciResourceBusLimit + 1;
                  BusLimit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioStack->SocketId].StackRes[IioStack->StackId].BusLimit;
                }
              }
            } else {
              //
              // Root bridge acpi device
              //
              BusBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceBusBase;
              BusLimit = 1;
            }
          } else {
            Status = AcpiPciDevPath2SktStkPort(mAmlOffsetTablePointer[i].Pathname, &Socket, &Stack, NULL);
            BusBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceBusBase;
            BusLimit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceBusLimit;

            if ((BusLimit > BusBase) && ((BusLimit - BusBase) > V_IOV_RESERVED_BUS_NUMBER)) {
              if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
                if (Stack == IIO_STACK5) {
                  BusBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[IIO_STACK2].BusLimit - V_IOV_RESERVED_BUS_NUMBER;
                  BusLimit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[IIO_STACK2].BusLimit;
                } else if ( Stack == IIO_STACK2) {
                  BusLimit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].BusLimit - V_IOV_RESERVED_BUS_NUMBER - 1;
                }
              }
            }

            if (Is14nmCpu) {
              if ((FpgaConfigurationSupported && (FpgaConfiguration.FpgaSktActive & (1 << Socket))) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
                if (Stack == IIO_PSTACK3) {
                  BusLimit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].BusLimit - FPGA_PREAllOCATE_BUS_NUM;
                  DEBUG ((DEBUG_INFO, "[ACPI](DSDT) [%d.%d] FPGA BusLimit = 0x%02X\n", Socket, Stack, BusLimit));
                }
              }
            }
          }

          AmlResourceAddress16Pointer->Granularity = 0;
          if ((IsCpuAndRevision (CPU_SNR, REV_ALL) && (BusLimit >= BusBase)) || (BusLimit > BusBase)) {
            if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
              switch (*(Signature2) | 0xFF000000) {
              case (SIGNATURE_32 ('D', 'I', 'N', '0') | 0xFF000000):
                BusLimit = BusBase;
                break;
              case (SIGNATURE_32 ('C', 'P', 'M', '0') | 0xFF000000):
                BusBase  = BusBase + 1;
                BusLimit = BusBase + 1;
                break;
              case (SIGNATURE_32 ('H', 'Q', 'M', '0') | 0xFF000000):
                BusBase  = BusBase + 3;
                BusLimit = BusBase + 1;
                break;
              default:
                break;
              }
            }
            AmlResourceAddress16Pointer->Minimum = (UINT16) BusBase;
            AmlResourceAddress16Pointer->Maximum = (UINT16) BusLimit;
            AmlResourceAddress16Pointer->AddressLength = (UINT16) (BusLimit - BusBase + 1);
          }
          //DEBUG((DEBUG_ERROR,", FIX1 BusBase = 0x%x, BusLimit = 0x%x\n",BusBase, BusLimit));
          Fixes++;
          break;

        //
        // "FIXB" BUS resource for FpgaKtiXX in Acpi\AcpiTables\Dsdt\FpgaKtiXX.asi
        //
        case (SIGNATURE_32 ('F', 'I', 'X', 'B')):
          if (Is14nmCpu) {
            BusBase = 0;
            BusLimit = 0;
            DEBUG ((DEBUG_INFO, "[ACPI](DSDT) Patch BUS resource for FPGA KTI\n"));

            if  (mCpuCsrAccessVarPtr->FpgaPresentBitMap & (1 << SktFpgaKti)){
              BusBase = mCpuCsrAccessVarPtr->SocketFirstBus[SktFpgaKti];
              BusLimit = mCpuCsrAccessVarPtr->SocketLastBus[SktFpgaKti];
              DEBUG ((DEBUG_INFO, "[ACPI](DSDT) SktFpga [%X]: BusBase[0x%X] BusLimit 0x%X\n", SktFpgaKti, BusBase, BusLimit));
            }

            AmlResourceAddress16Pointer->Granularity = 0;
            if (BusLimit > BusBase) {
              AmlResourceAddress16Pointer->Minimum = (UINT16) BusBase;
              AmlResourceAddress16Pointer->Maximum = (UINT16) BusLimit;
              AmlResourceAddress16Pointer->AddressLength = (UINT16) (BusLimit - BusBase + 1);
            }

            SktFpgaKti++;
            Fixes++;
          }
          break;

        //
        // "FIX2" IO resource for for PCXX in Acpi\AcpiTables\Dsdt\SysBus.asi and PCXX.asi
        //
        case (SIGNATURE_32 ('F', 'I', 'X', '2')):
          if (IsStackInCxlMode(Socket, Stack) == TRUE) {
            //
            // Stack in CXL mode
            //
            Status = AcpiPciDevPath2SktStkPort(mAmlOffsetTablePointer[i].Pathname, &Socket, &Stack, NULL);

            if (EFI_ERROR(Status)) {
              if (IsCxlAcpiDevicePath (mAmlOffsetTablePointer[i].Pathname) == TRUE) {
                //
                // CXL acpi device is found
                //
                Status = GetCxlStackAttributes (mAmlOffsetTablePointer[i].Pathname, IioStack);
                if (!EFI_ERROR(Status)) {
                  IoBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioStack->SocketId].StackRes[IioStack->StackId].IoBase;
                  IoLimit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioStack->SocketId].StackRes[IioStack->StackId].PciResourceIoBase - 1;
                }
              }
            }
            else {
              //
              // Root bridge acpi device
              //
              IoBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceIoBase;
              IoLimit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceIoLimit;
            }
          } else {
            Status = AcpiPciDevPath2SktStkPort(mAmlOffsetTablePointer[i].Pathname, &Socket, &Stack, NULL);
            IoBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceIoBase;
            IoLimit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceIoLimit;
          }
          if (IoLimit > IoBase) {
            if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
              if (Stack == IIO_STACK5) {
                IoBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[IIO_STACK2].PciResourceIoBase +
                  ((mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[IIO_STACK2].PciResourceIoLimit -
                    mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[IIO_STACK2].PciResourceIoBase + 1) / 2);
                IoLimit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[IIO_STACK2].PciResourceIoLimit;
              } else if (Stack == IIO_STACK2) {
                IoLimit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceIoBase +
                  ((mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceIoLimit -
                    mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceIoBase + 1) / 2) - 1;
              }
            }
            AmlResourceAddress16Pointer->Minimum = (UINT16) IoBase;
            AmlResourceAddress16Pointer->Maximum = (UINT16) IoLimit;
            AmlResourceAddress16Pointer->AddressLength = (UINT16) (IoLimit - IoBase + 1);
          }
          AmlResourceAddress16Pointer->Granularity = 0;
          //DEBUG((DEBUG_ERROR,", FIX2 IoBase = 0x%x, IoLimit = 0x%x\n",IoBase, IoLimit));
          Fixes++;
          break;

        //
        // "FIX9" BUS resource for UNXX in Acpi\AcpiTables\Dsdt\Uncore.asi
        //
        case (SIGNATURE_32('F', 'I', 'X', '9')) :
          Status = AcpiPciDevPath2SktStkPort(mAmlOffsetTablePointer[i].Pathname, &Socket, &Stack, NULL);
          if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
            UboxStack = IIO_PSTACK2; // For CPX KTI creates UBOX but in stack 3, not in UBOX_STACK
          } else {
            UboxStack = UBOX_STACK;
          }
          BusBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[UboxStack].BusBase;
          BusLimit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[UboxStack].BusLimit;
          if (mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[UboxStack].Personality != TYPE_UBOX ||
              BusBase > BusLimit) {

            DEBUG ((DEBUG_ERROR, "[ACPI](DSDT) ERROR: Stack [%d.%d] of type %d is not UBOX, '%a' not patched\n",
                    Socket, UboxStack, mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[UboxStack].Personality,
                    mAmlOffsetTablePointer[i].Pathname));
            break;
          }
          AmlResourceAddress16Pointer->Granularity = 0;
          if (Stack & 1) {
            AmlResourceAddress16Pointer->Minimum = BusLimit;
            AmlResourceAddress16Pointer->Maximum = BusLimit;
          } else {
            AmlResourceAddress16Pointer->Minimum = BusBase;
            AmlResourceAddress16Pointer->Maximum = BusBase;
          }
          AmlResourceAddress16Pointer->AddressLength = 1;
          mAcpiParameter->BusBase[Socket][Stack] = (UINT8)AmlResourceAddress16Pointer->Minimum;
          mAcpiParameter->IioPresentBitMask[Socket] |= 1 << Stack;
          Fixes++;
          break;

        //
        // "FIX6" IO resource for PCXX in Acpi\AcpiTables\Dsdt\PCXX.asi
        //
        case (SIGNATURE_32 ('F', 'I', 'X', '6')):
          if (IsStackInCxlMode(Socket, Stack) == TRUE) {
            //
            // Stack in CXL mode
            //
            Status = AcpiPciDevPath2SktStkPort(mAmlOffsetTablePointer[i].Pathname, &Socket, &Stack, NULL);

            if (EFI_ERROR(Status)) {
              if (IsCxlAcpiDevicePath (mAmlOffsetTablePointer[i].Pathname) == TRUE) {
                //
                // CXL acpi device is found
                //
                Status = GetCxlStackAttributes (mAmlOffsetTablePointer[i].Pathname, IioStack);
                if (!EFI_ERROR(Status)) {
                  Socket = IioStack->SocketId;
                  Stack = IioStack->StackId;
                }
              }
            }
            else {
              //In cxl mode, PCIe root bridge cannot have vga
              break;
            }
          } else {
            Status = AcpiPciDevPath2SktStkPort(mAmlOffsetTablePointer[i].Pathname, &Socket, &Stack, NULL);
          }
          AmlResourceAddress16Pointer->Granularity = 0;
          if ((mSocketMpLinkConfiguration.LegacyVgaSoc == Socket) &&
              (mSocketMpLinkConfiguration.LegacyVgaStack == Stack)){

            AmlResourceAddress16Pointer->Minimum = (UINT16) 0x03b0;
            AmlResourceAddress16Pointer->Maximum = (UINT16) 0x03bb;
            AmlResourceAddress16Pointer->AddressLength = (UINT16) 0x000C;
          }
          Fixes++;
         break;

        //
        // "FIX7" IO resource for PCXX in Acpi\AcpiTables\Dsdt\PCXX.asi
        //
        case (SIGNATURE_32 ('F', 'I', 'X', '7')):
          if (IsStackInCxlMode(Socket, Stack) == TRUE) {
            //
            // Stack in CXL mode
            //
            Status = AcpiPciDevPath2SktStkPort(mAmlOffsetTablePointer[i].Pathname, &Socket, &Stack, NULL);

            if (EFI_ERROR(Status)) {
              if (IsCxlAcpiDevicePath (mAmlOffsetTablePointer[i].Pathname) == TRUE) {
                //
                // CXL acpi device is found
                //
                Status = GetCxlStackAttributes (mAmlOffsetTablePointer[i].Pathname, IioStack);
                if (!EFI_ERROR(Status)) {
                  Socket = IioStack->SocketId;
                  Stack = IioStack->StackId;
                }
              }
            }
            else {
              //In cxl mode, PCIe root bridge cannot have vga
              break;
            }
          } else {
            Status = AcpiPciDevPath2SktStkPort(mAmlOffsetTablePointer[i].Pathname, &Socket, &Stack, NULL);
          }
          AmlResourceAddress16Pointer->Granularity = 0;
          if ((mSocketMpLinkConfiguration.LegacyVgaSoc == Socket) &&
              (mSocketMpLinkConfiguration.LegacyVgaStack == Stack)) {

            AmlResourceAddress16Pointer->Minimum = (UINT16) 0x03c0;
            AmlResourceAddress16Pointer->Maximum = (UINT16) 0x03df;
            AmlResourceAddress16Pointer->AddressLength = (UINT16) 0x0020;
          }
          Fixes++;
          break;

        default:
          DEBUG ((DEBUG_ERROR, "[ACPI](DSDT) WARNING: Object '%a' with opcode 0x%02X not patched\n",
                  mAmlOffsetTablePointer[i].Pathname, mAmlOffsetTablePointer[i].Opcode));
          break;
      }
    } else if (mAmlOffsetTablePointer[i].Opcode == AML_SIZE_OF_OP) {
      //
      // If Opcode is 0x87, then operator is DWORDMemory().
      // (TableHeader + AmlOffsetTable.Offset) must be cast to AML_RESOURCE_ADDRESS32 to change values.
      //
      AmlResourceAddress32Pointer = (AML_RESOURCE_ADDRESS32 *) (DsdtPointer);
      //
      // The assert below confirms that AML structure matches the offsets table.
      // If not then patching the AML would just corrupt it and result in OS failure.
      // If you encounter this assert something went wrong in *.offset.h files
      // generation. Remove the files and rebuild.
      //
      ASSERT(AmlResourceAddress32Pointer->DescriptorType == mAmlOffsetTablePointer[i].Opcode);
      //
      // Last 4 chars of AmlOffsetTable.Pathname has FIX tag.
      //
      Signature = (UINT32 *) (mAmlOffsetTablePointer[i].Pathname + AsciiStrLen(mAmlOffsetTablePointer[i].Pathname) - 4);
      Signature2 = (UINT32 *) (mAmlOffsetTablePointer[i].Pathname + AsciiStrLen(mAmlOffsetTablePointer[i].Pathname) - 9);
      switch (*Signature) {
        //
        // "FIX3" PCI32 resource for PCXX in Acpi\AcpiTables\Dsdt\SysBus.asi and PCXX.asi
        //
        case (SIGNATURE_32 ('F', 'I', 'X', '3')):
          if (IsStackInCxlMode(Socket, Stack) == TRUE) {
            //
            // Stack in CXL mode
            //
            Status = AcpiPciDevPath2SktStkPort(mAmlOffsetTablePointer[i].Pathname, &Socket, &Stack, NULL);

            if (EFI_ERROR(Status)) {
              if (IsCxlAcpiDevicePath (mAmlOffsetTablePointer[i].Pathname) == TRUE) {
                //
                // CXL acpi device is found
                //
                Status = GetCxlStackAttributes (mAmlOffsetTablePointer[i].Pathname, IioStack);
                if (!EFI_ERROR(Status)) {
                  MemBase32 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioStack->SocketId].StackRes[IioStack->StackId].Mmio32Base + \
                              GetCxlDpUpReservedMmio32Size (IioStack->SocketId, IioStack->StackId);
                  MemLimit32 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioStack->SocketId].StackRes[IioStack->StackId].PciResourceMem32Base - 1;
                }
              }
            }
            else {
              //
              // PCIe Root bridge acpi device found
              //
              MemBase32 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem32Base;
              MemLimit32 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem32Limit;
            }
          } else {
            Status = AcpiPciDevPath2SktStkPort(mAmlOffsetTablePointer[i].Pathname, &Socket, &Stack, NULL);
            MemBase32 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem32Base;
            MemLimit32 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem32Limit;
          }

          if (Is14nmCpu) {
            if (FpgaConfigurationSupported && (FpgaConfiguration.FpgaSktActive & (1 << Socket))) {
              if (Stack == IIO_PSTACK3) {
                //Carve out FPGA RPa resources from the original BIOS defined ACPI root bridge
                MemLimit32 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem32Limit - FPGA_PREALLOCATE_MEM_SIZE - FPGA_PREALLOCATE_VTD_SIZE;
                DEBUG ((DEBUG_INFO, "[ACPI](DSDT] [%d.%d] MemLimit32 = 0x%X\n", Socket, Stack, MemLimit32));
              }
            }
          }
          if (MemLimit32 > MemBase32) {
            if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
              if (Stack == IIO_STACK5) {
                MemBase32 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[IIO_STACK2].PciResourceMem32Base +
                  ((mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[IIO_STACK2].PciResourceMem32Limit -
                    mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[IIO_STACK2].PciResourceMem32Base + 1) / 2);
                MemLimit32 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[IIO_STACK2].PciResourceMem32Limit;
              } else if (Stack == IIO_STACK2) {
                MemLimit32 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem32Base +
                  ((mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem32Limit -
                    mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem32Base + 1) / 2) - 1;
              }
            }
            if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
              switch (*Signature2 | 0xFF000000) {
              case (SIGNATURE_32 ('C', 'P', 'M', '0') | 0xFF000000):
              case (SIGNATURE_32 ('H', 'Q', 'M', '0') | 0xFF000000):
                break;
              case (SIGNATURE_32 ('D', 'I', 'N', '0') | 0xFF000000):
              default:
                AmlResourceAddress32Pointer->Minimum = (UINT32) MemBase32;
                AmlResourceAddress32Pointer->Maximum = (UINT32) MemLimit32;
                AmlResourceAddress32Pointer->AddressLength = (UINT32) (MemLimit32 - MemBase32 + 1);
                break;
              }
            } else {
              AmlResourceAddress32Pointer->Minimum = (UINT32) MemBase32;
              AmlResourceAddress32Pointer->Maximum = (UINT32) MemLimit32;
              AmlResourceAddress32Pointer->AddressLength = (UINT32) (MemLimit32 - MemBase32 + 1);
            }
          }
          AmlResourceAddress32Pointer->Granularity = 0;
          //DEBUG((DEBUG_ERROR,", FIX3 MemBase32 = 0x%08x, MemLimit32 = 0x%08x\n",MemBase32, MemLimit32));
          Fixes++;
          break;

        //
        // "FIX5" IO resource for PCXX in Acpi\AcpiTables\Dsdt\PCXX.asi
        //
        case (SIGNATURE_32 ('F', 'I', 'X', '5')):
          if (IsStackInCxlMode(Socket, Stack) == TRUE) {
            //
            // Stack in CXL mode
            //
            Status = AcpiPciDevPath2SktStkPort(mAmlOffsetTablePointer[i].Pathname, &Socket, &Stack, NULL);

            if (EFI_ERROR(Status)) {
              if (IsCxlAcpiDevicePath (mAmlOffsetTablePointer[i].Pathname) == TRUE) {
                //
                // CXL acpi device is found
                //
                Status = GetCxlStackAttributes (mAmlOffsetTablePointer[i].Pathname, IioStack);
                if (!EFI_ERROR(Status)) {
                  Socket = IioStack->SocketId;
                  Stack = IioStack->StackId;
                }
              }
            }
            else {
              //In cxl mode, PCIe root bridge cannot have vga
              break;
            }
          } else {
            Status = AcpiPciDevPath2SktStkPort(mAmlOffsetTablePointer[i].Pathname, &Socket, &Stack, NULL);
          }

          AmlResourceAddress32Pointer->Granularity = 0;
          if ((mSocketMpLinkConfiguration.LegacyVgaSoc == Socket) &&
              (mSocketMpLinkConfiguration.LegacyVgaStack == Stack)) {
              AmlResourceAddress32Pointer->Minimum = 0x000a0000;
              AmlResourceAddress32Pointer->Maximum = 0x000bffff;
              AmlResourceAddress32Pointer->AddressLength = 0x00020000;
          }
          Fixes++;
          break;

        //
        // "FIXZ" IO resource for FpgaBusXX in Acpi\AcpiTables\Dsdt\FpgaBusXX.asi
        //
        case (SIGNATURE_32 ('F', 'I', 'X', 'Z')):
          if (Is14nmCpu) {
            AmlResourceAddress32Pointer->Granularity = 0;
            if (FpgaConfigurationSupported && (FpgaConfiguration.FpgaSktActive & (1 << SktFPGA))) {
              AmlResourceAddress32Pointer->Minimum = mIioUds->IioUdsPtr->PlatformData.IIO_resource[SktFPGA].StackRes[IIO_PSTACK3].VtdBarAddress;
              AmlResourceAddress32Pointer->Maximum = mIioUds->IioUdsPtr->PlatformData.IIO_resource[SktFPGA].StackRes[IIO_PSTACK3].VtdBarAddress + 0x3fff;
              AmlResourceAddress32Pointer->AddressLength = 0x4000;
              DEBUG ((DEBUG_INFO, "Skt[%X] FPGA ACPI VTD BASE = 0x%X\n", SktFPGA, AmlResourceAddress32Pointer->Minimum));
            }
            SktFPGA++;
            Fixes++;
          }
          break;

        default:
          DEBUG ((DEBUG_ERROR, "[ACPI](DSDT) WARNING: Object '%a' with opcode 0x%02X not patched\n",
                  mAmlOffsetTablePointer[i].Pathname, mAmlOffsetTablePointer[i].Opcode));
          break;
      }
    } else if (mAmlOffsetTablePointer[i].Opcode == AML_CREATE_DWORD_FIELD_OP) {
      //
      // If Opcode is 0x8A, then operator is QWORDMemory().
      // (TableHeader + AmlOffsetTable.Offset) must be cast to AML_RESOURCE_ADDRESS64 to change values.
      //
      AmlResourceAddress64Pointer = (AML_RESOURCE_ADDRESS64 *) (DsdtPointer);
      //
      // The assert below confirms that AML structure matches the offsets table.
      // If not then patching the AML would just corrupt it and result in OS failure.
      // If you encounter this assert something went wrong in *.offset.h files
      // generation. Remove the files and rebuild.
      //
      ASSERT(AmlResourceAddress64Pointer->DescriptorType == mAmlOffsetTablePointer[i].Opcode);
      //
      // Last 4 chars of AmlOffsetTable.Pathname has FIX tag.
      //
      Signature = (UINT32 *) (mAmlOffsetTablePointer[i].Pathname + AsciiStrLen(mAmlOffsetTablePointer[i].Pathname) - 4);
      Signature2 = (UINT32 *) (mAmlOffsetTablePointer[i].Pathname + AsciiStrLen(mAmlOffsetTablePointer[i].Pathname) - 9);
      switch (*Signature) {
        //
        // "FIX4" PCI64 resource for PCXX in Acpi\AcpiTables\Dsdt\SysBus.asi and PCXX.asi
        //
        case (SIGNATURE_32 ('F', 'I', 'X', '4')):
          if (mSocketIioConfiguration.Pci64BitResourceAllocation) {
            if (IsStackInCxlMode(Socket, Stack) == TRUE) {
              //
              // Stack in CXL mode
              //
              Status = AcpiPciDevPath2SktStkPort(mAmlOffsetTablePointer[i].Pathname, &Socket, &Stack, NULL);

              if (EFI_ERROR(Status)) {
                if (IsCxlAcpiDevicePath (mAmlOffsetTablePointer[i].Pathname) == TRUE) {
                  //
                  // CXL acpi device is found
                  //
                  Status = GetCxlStackAttributes (mAmlOffsetTablePointer[i].Pathname, IioStack);
                  if (!EFI_ERROR(Status)) {
                    MemBase64 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioStack->SocketId].StackRes[IioStack->StackId].Mmio64Base;
                    MemLimit64 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioStack->SocketId].StackRes[IioStack->StackId].PciResourceMem64Base - 1;
                  }
                }
              }
              else {
                //
                // PCIe Root bridge acpi device found
                //
                MemBase64 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem64Base;
                MemLimit64 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem64Limit;
              }
            }
            else {
              Status = AcpiPciDevPath2SktStkPort(mAmlOffsetTablePointer[i].Pathname, &Socket, &Stack, NULL);
              MemBase64 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem64Base;
              MemLimit64 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem64Limit;
            }

            if (MemLimit64 > MemBase64) {
              if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
                if (Stack == IIO_STACK5) {
                  MemBase64 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[IIO_STACK2].PciResourceMem64Base +
                      ((mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[IIO_STACK2].PciResourceMem64Limit -
                        mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[IIO_STACK2].PciResourceMem64Base + 1) / 2);
                  MemLimit64 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[IIO_STACK2].PciResourceMem64Limit;
                } else if (Stack == IIO_STACK2) {
                  MemLimit64 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem64Base +
                      ((mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem64Limit -
                        mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem64Base + 1) / 2) - 1;
                }
              }
              if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
                switch (*Signature2 | 0xFF000000) {
                  case (SIGNATURE_32 ('C', 'P', 'M', '0') | 0xFF000000):
                    MemLimit64 = MemBase64 + CPM_MMIO_SIZE - 1;
                    break;
                  case (SIGNATURE_32 ('H', 'Q', 'M', '0') | 0xFF000000):
                    MemBase64 = MemBase64 + CPM_MMIO_SIZE;
                    MemLimit64 = MemBase64 + HQM_MMIO_SIZE - 1;
                    break;
                  case (SIGNATURE_32 ('D', 'I', 'N', '0') | 0xFF000000):
                    MemBase64 = MemBase64 + CPM_MMIO_SIZE + HQM_MMIO_SIZE;
                    break;
                  default:
                    break;
                }
              }
              AmlResourceAddress64Pointer->Granularity = 0;
              AmlResourceAddress64Pointer->Minimum = (UINT64) MemBase64;
              AmlResourceAddress64Pointer->Maximum = (UINT64) MemLimit64;
              AmlResourceAddress64Pointer->AddressLength = (UINT64) (MemLimit64 - MemBase64 + 1);
            }
            //DEBUG((DEBUG_ERROR,", FIX4 MemBase64 = 0x%x, MemLimit64 = 0x%x\n",MemBase64, MemLimit64));
          }
          break;
        default:
          DEBUG ((DEBUG_ERROR, "[ACPI](DSDT) WARNING: Object '%a' with opcode 0x%02X not patched\n",
                  mAmlOffsetTablePointer[i].Pathname, mAmlOffsetTablePointer[i].Opcode));
          break;
      }
    } else {
      DEBUG ((DEBUG_ERROR, "[ACPI](DSDT) WARNING: Object '%a' with opcode 0x%02X not patched\n",
              mAmlOffsetTablePointer[i].Pathname, mAmlOffsetTablePointer[i].Opcode));
    }
  }

  // CurrPtr = beginning of table
  //
  CurrPtr = (UINT8 *) TableHeader;

  // EndPtr = beginning of table + length of table
  //
  EndPtr = (CurrPtr + ((EFI_ACPI_COMMON_HEADER *) CurrPtr)->Length);

  // Subtract from End Ptr the largest data item we read from table
  //  so we don't try to access data beyond end of table
  //
  EndPtr -= 9;

  for (DsdtPointer = CurrPtr; DsdtPointer <= EndPtr; DsdtPointer++) {

    //fix CpuMemHp.asi, force no same ASL code string as it...
    if ((DsdtPointer[0] == 'C') && (DsdtPointer[6] == 0x4)  && (DsdtPointer[5] == 0x10)) {
      if (mCpuOrderSorted) {
        CpuSkt = (UINT32) DsdtPointer[4];
        AdjustSize = 0;
        if ((DsdtPointer[1] > '0') && (DsdtPointer[1] <= '9')) {
          AdjustSize = (UINT32) ((DsdtPointer[1] -'0') * 0x100);
        } else if ((DsdtPointer[1] >= 'A') && (DsdtPointer[1] <= 'F')) {
          AdjustSize = (UINT32) ((DsdtPointer[1] -'A' + 10) * 0x100);
        }

        CpuIndex = AdjustSize;

        AdjustSize = 0;
        if ((DsdtPointer[2] > '0') && (DsdtPointer[2] <= '9')) {
          AdjustSize = (UINT32) ((DsdtPointer[2] -'0') * 0x10);
        } else if ((DsdtPointer[2] >= 'A') && (DsdtPointer[2] <= 'F')) {
          AdjustSize = (UINT32) ((DsdtPointer[2] -'A' + 10) * 0x10);
        }

        CpuIndex += AdjustSize;

        AdjustSize = 0;
        if ((DsdtPointer[3] > '0') && (DsdtPointer[3] <= '9')) {
          AdjustSize = (UINT32) (DsdtPointer[3] -'0');
        } else if ((DsdtPointer[3] >= 'A') && (DsdtPointer[3] <= 'F')) {
          AdjustSize = (UINT32) (DsdtPointer[3] -'A' + 10);
        }

        CpuIndex += AdjustSize;

        NodeIndex = (UINT32) (CpuSkt << mNumOfBitShift) + mApicIdMap[CpuSkt][CpuIndex] ;

        DsdtPointer[4] = (UINT8) 0xFF;
        if (((mCpuCsrAccessVarPtr->socketPresentBitMap >> CpuSkt) & BIT0) == 1) {
          if (CheckCurrentThreadStatus (NodeIndex) == EFI_SUCCESS) {
            DsdtPointer[4] = (UINT8) (NodeIndex & 0xFF);
          }
        }

        //Update IO Address
        *(UINT16 *)(DsdtPointer+5) = (UINT16)(PM_BASE_ADDRESS + 0x10);
      }
    }

    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

      if ((mCpuCsrAccessVarPtr->socketPresentBitMap & (BIT0 << Socket)) == 0) {
        continue;
      }
      //
      // Find APT##socket name
      //
      if ((DsdtPointer[0] == 'A') && (DsdtPointer[1] == 'P') && (DsdtPointer[2] == 'T') && (DsdtPointer[3] == '0' + Socket)) {
        NamePtr = ACPI_NAME_COMMAND_FROM_NAMEPACK_STR (DsdtPointer);
        ApicMapPtr = mApicIdMap[Socket];
        if (NamePtr->StartByte != AML_NAME_OP) {
          continue;
        }

        Counter = DsdtPointer[8];
        ASSERT (Counter >= (UINT32) (MAX_THREAD * MAX_CORE));
        DEBUG ((EFI_D_INFO, "\n::ACPI::  Found 'APT%x'...Counter = DsdtPointer[7] = %x\n\n", Socket, Counter));
         for (i = 0; i < (MAX_THREAD * MAX_CORE); i++) {
           DEBUG ((EFI_D_VERBOSE, "Before override, DsdtPointer[%x] = %x,   ", i, DsdtPointer[i+9]));
           DsdtPointer[i+9] = (UINT8)ApicMapPtr[i];
           DEBUG ((EFI_D_VERBOSE, "Then override value = %x \n", DsdtPointer[i+9]));
         }
      }
    }
    //
    // Fix up _S3
    //
    if ((DsdtPointer[0] == '_') && (DsdtPointer[1] == 'S') && (DsdtPointer[2] == '3')) {
      NamePtr = ACPI_NAME_COMMAND_FROM_NAMEPACK_STR (DsdtPointer);
      if (NamePtr->StartByte != AML_NAME_OP) {
        //DEBUG(( EFI_D_ERROR, "...Found '_S3', but it is not NAME_OP, continue...\n "));
        continue;
      }

      if (!mSocketPowermanagementConfiguration.AcpiS3Enable) {
        //
        // S3 disabled
        //
        DsdtPointer[0] = 'D';
      }
    }
    //
    // Fix up _S4
    //
    if ((DsdtPointer[0] == '_') && (DsdtPointer[1] == 'S') && (DsdtPointer[2] == '4')) {
      NamePtr = ACPI_NAME_COMMAND_FROM_NAMEPACK_STR (DsdtPointer);
      if (NamePtr->StartByte != AML_NAME_OP) {
        //DEBUG(( EFI_D_ERROR, "...Found '_S4', but it is not NAME_OP, continue...\n "));
        continue;
      }
      if (!mSocketPowermanagementConfiguration.AcpiS4Enable) {
        //
        // S4 disabled
        //
        DsdtPointer[0] = 'D';
      }
    }
  }
  return EFI_SUCCESS;
}

