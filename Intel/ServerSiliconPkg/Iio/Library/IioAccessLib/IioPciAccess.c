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
#include <Protocol/CpuCsrAccess.h>
#include <Library/DebugLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/CheckpointLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/PcieMmcfgLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <IndustryStandard/Pci.h>
#include <Library/MmPciBaseLib.h>
#include <PcieRegs.h>
#include <Library/IoLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/IioDataHubLib.h>

#include <RcRegs.h>
#include <IioRegs.h>

#include <LibraryPrivate/IioDebug.h>


/*++

Routine Description:
  Update 32-bits PCIe address memory with new Data

Arguments:
  Address            - Address that needs to be updated
  Data               - Data value

Returns:
  None

--*/
VOID
IioPciExpressWrite32 (
  IN UINT8             IioIndex,
  IN UINT16            Bus,
  IN UINT16            Device,
  IN UINT8             Function,
  IN UINT32            Offset,
  IN UINT32            Data
  )
{
  USRA_ADDRESS                Address;

  USRA_PCIE_SEG_ADDRESS (Address, UsraWidth32, GetPcieSegment (IioIndex), \
         Bus, Device, Function, Offset);
  Address.Attribute.S3Enable = USRA_ENABLE;
  RegisterWrite (&Address, &Data);
}


/*++

Routine Description:
  Update 16-bits PCIe address memory with new Data

Arguments:
  Address            - Address that needs to be updated
  Data               - Data value

Returns:
  None

--*/
VOID
IioPciExpressWrite16 (
  IN UINT8             IioIndex,
  IN UINT16            Bus,
  IN UINT16            Device,
  IN UINT8             Function,
  IN UINT32            Offset,
  IN UINT16            Data
  )
{
  USRA_ADDRESS                Address;

  USRA_PCIE_SEG_ADDRESS (Address, UsraWidth16, GetPcieSegment (IioIndex), \
            Bus, Device, Function, Offset);
  Address.Attribute.S3Enable = USRA_ENABLE;
  RegisterWrite (&Address, &Data);
}


/*++

Routine Description:
  Update 16-bits PCIe address memory with new Data
  The write is not saved in the S3 boot script.

Arguments:
  Address            - Address that needs to be updated
  Data               - Data value

Returns:
  None

--*/
VOID
IioPciExpressNonS3Write16 (
  IN UINT8             IioIndex,
  IN UINT16            Bus,
  IN UINT16            Device,
  IN UINT8             Function,
  IN UINT32            Offset,
  IN UINT16            Data
  )
{
  USRA_ADDRESS                Address;
  UINTN                       MmCfgBase;
  UINT32                      AlignedAddress;

  USRA_PCIE_SEG_ADDRESS (Address, UsraWidth16, GetPcieSegment (IioIndex), \
            Bus, Device, Function, Offset);
  Address.Attribute.S3Enable = USRA_DISABLE;
  RegisterWrite (&Address, &Data);

  if (UbiosGenerationEnabled ()) {

    MmCfgBase = GetPcieSegMmcfgBaseAddress (&Address);
    AlignedAddress = ((UINT32) MmCfgBase) + (Address.dwRawData[0] & 0x0fffffff);
    DEBUG ((DEBUG_ERROR, "\n  mov WORD PTR ds:[0%08xh], 0%04xh\n", AlignedAddress, Data));
  }
}


/*++

Routine Description:
  Update 8-bits PCIe address memory with new Data

Arguments:
  Address            - Address that needs to be updated
  Data               - Data value

Returns:
  None

--*/
VOID
IioPciExpressWrite8 (
  IN UINT8             IioIndex,
  IN UINT16            Bus,
  IN UINT16            Device,
  IN UINT8             Function,
  IN UINT32            Offset,
  IN UINT8             Data
  )
{
  USRA_ADDRESS                Address;

  USRA_PCIE_SEG_ADDRESS (Address, UsraWidth8, GetPcieSegment (IioIndex), \
            Bus, Device, Function, Offset);
  Address.Attribute.S3Enable = USRA_ENABLE;
  RegisterWrite (&Address, &Data);
}

/*++

Routine Description:
  Reads 32-bits PCIe address memory

Arguments:
  Address            - Address that needs to be updated

Returns:
  Data               - Data value

--*/
UINT32
IioPciExpressRead32 (
  IN UINT8             IioIndex,
  IN UINT16            Bus,
  IN UINT16            Device,
  IN UINT8             Function,
  IN UINT32            Offset
  )
{
  UINT32 Data;
  USRA_ADDRESS                Address;
  USRA_PCIE_SEG_ADDRESS (Address, UsraWidth32, GetPcieSegment (IioIndex), \
            Bus, Device, Function, Offset);
  RegisterRead (&Address, &Data);
  return Data;
}

/*++

Routine Description:
  Reads 16-bits PCIe address memory

Arguments:
  Address            - Address that needs to be updated

Returns:
  Data               - Data value

--*/
UINT16
IioPciExpressRead16 (
  IN UINT8             IioIndex,
  IN UINT16            Bus,
  IN UINT16            Device,
  IN UINT8             Function,
  IN UINT32            Offset
  )
{
  UINT16 Data;
  USRA_ADDRESS                Address;
  USRA_PCIE_SEG_ADDRESS (Address, UsraWidth16, GetPcieSegment (IioIndex), \
            Bus, Device, Function, Offset);
  RegisterRead (&Address, &Data);
  return Data;
}

/*++

Routine Description:
  Reads 8-bits PCIe address memory

Arguments:
  Address            - Address that needs to be updated

Returns:
  Data               - Data value

--*/
UINT8
IioPciExpressRead8 (
  IN UINT8             IioIndex,
  IN UINT16            Bus,
  IN UINT16            Device,
  IN UINT8             Function,
  IN UINT32            Offset
  )
{
  UINT8 Data;
  USRA_ADDRESS                Address;

  USRA_PCIE_SEG_ADDRESS (Address, UsraWidth8, GetPcieSegment (IioIndex), \
            Bus, Device, Function, Offset);
  RegisterRead (&Address, &Data);
  return Data;
}

/**

  Function to write CPU CSR with 32-bit data in IIO module
  with S3 save to boot script.

  @param IioIndex    -  IIO instance
  @param BoxInst     -  Instance in Box type to be accessed
  @param Csr         -  CPU CSR offset to be access
  @param Data        -  Input data to be written to CPU CSR

  @retval VOID

**/
VOID
IioWriteCpuCsr32 (
  IN  UINT8             IioIndex,
  IN  UINT8             BoxInst,
  IN  UINT32            Csr,
  IN  UINT32            Data
  )
{
  S3UsraCsrWrite (IioIndex, BoxInst, Csr, (UINTN)Data);
}

/**

  Function to write CPU CSR with 32-bit data in IIO module.
  The write is not saved in the S3 boot script.

  @param IioIndex    -  IIO instance
  @param BoxInst     -  Instance in Box type to be accessed
  @param Csr         -  CPU CSR offset to be access
  @param Data        -  Input data to be written to CPU CSR

  @retval VOID

**/
VOID
IioNonS3WriteCpuCsr32 (
  IN  UINT8             IioIndex,
  IN  UINT8             BoxInst,
  IN  UINT32            Csr,
  IN  UINT32            Data
  )
{
  UsraCsrWrite (IioIndex, BoxInst, Csr, (UINTN)Data);
}

/**

  Function to write CPU CSR with 16-bit data in IIO module.

  @param BoxInst     -  Instance in Box type to be accessed
  @param Csr         -  CPU CSR offset to be access
  @param Data        -  Input data to be written to CPU CSR

  @retval VOID

**/
VOID
IioWriteCpuCsr16 (
  IN  UINT8             IioIndex,
  IN  UINT8             BoxInst,
  IN  UINT32            Csr,
  IN  UINT16            Data
  )
{
  S3UsraCsrWrite (IioIndex, BoxInst, Csr, (UINTN)Data);
}

/**

  Function to write CPU CSR with 16-bit data in IIO module.
  The write is not saved in the S3 boot script.

  @param IioIndex    -  IIO instance
  @param BoxInst     -  Instance in Box type to be accessed
  @param Csr         -  CPU CSR offset to be access
  @param Data        -  Input data to be written to CPU CSR

  @retval VOID

**/
VOID
IioNonS3WriteCpuCsr16 (
  IN  UINT8             IioIndex,
  IN  UINT8             BoxInst,
  IN  UINT32            Csr,
  IN  UINT32            Data
  )
{
  UsraCsrWrite (IioIndex, BoxInst, Csr, (UINTN)Data);
}

/**

  Function to write CPU CSR with 8-bit data in IIO module.

  @param BoxInst     -  Instance in Box type to be accessed
  @param Csr         -  CPU CSR offset to be access
  @param Data        -  Input data to be written to CPU CSR

  @retval VOID

**/
VOID
IioWriteCpuCsr8 (
  IN  UINT8             IioIndex,
  IN  UINT8             BoxInst,
  IN  UINT32            Csr,
  IN  UINT8             Data
  )
{
  S3UsraCsrWrite (IioIndex, BoxInst, Csr, (UINTN)Data);
}

/**

  Funtion to write CPU CSR with 32-bit data in IIO module.

  @param BoxInst     -  Instance in Box type to be accessed
  @param Csr         -  CPU CSR offset to be access
  @param Data        -  Input data to be written to CPU CSR

  @retval VOID

**/
UINT32
IioReadCpuCsr32 (
  IN  UINT8             IioIndex,
  IN  UINT8             BoxInst,
  IN  UINT32            Csr
  )
{
  UINT32            Data;

  Data = UsraCsrRead (IioIndex, BoxInst, Csr);

  return Data;
}

/**

  Function to write CPU CSR with 16-bit data in IIO module.

  @param BoxInst     -  Instance in Box type to be accessed
  @param Csr         -  CPU CSR offset to be access
  @param Data        -  Input data to be written to CPU CSR

  @retval VOID

**/
UINT16
IioReadCpuCsr16 (
  IN  UINT8             IioIndex,
  IN  UINT8             BoxInst,
  IN  UINT32            Csr
  )
{
  UINT16 Data;

  Data = (UINT16)UsraCsrRead (IioIndex, BoxInst, Csr);
  return Data;
}

/**

  Function to write CPU CSR with 8-bit data in IIO module.

  @param BoxInst     -  Instance in Box type to be accessed
  @param Csr         -  CPU CSR offset to be access
  @param Data        -  Input data to be written to CPU CSR

  @retval VOID

**/
UINT8
IioReadCpuCsr8 (
  IN  UINT8             IioIndex,
  IN  UINT8             BoxInst,
  IN  UINT32            Csr
  )
{
  UINT8 Data;

  Data = (UINT8)UsraCsrRead (IioIndex, BoxInst, Csr);

  return Data;
}

//
//1404610230:IIO Break Points needs to be improved.
//
/**

  Function to halt the system if the CheckPoint is enabled

  @param Socket        -  Socket includes the CSR
  @param MajorCode     -  Major code ID of the BreakPoint
  @param MinorCode     -  Minor code ID of the BreakPoint
  @param Data          -  Input data to be written to CPU CSR

  @retval VOID

**/
VOID
IioBreakAtCheckPoint (
  IN  UINT8       Socket,
  IN  UINT8       MajorCode,
  IN  UINT8       MinorCode,
  IN  UINT16      Data
  )
{
#ifndef IA32
  EFI_STATUS                    Status;
  EFI_CPU_CSR_ACCESS_PROTOCOL   *CpuCsrAccess;
#endif

#ifdef IA32
  // Anyone who added it should fix it before using. This brakes all builds.
#ifdef SPR_HOST
  OutputCheckpointSocket (Socket, MajorCode, MinorCode, Data);
#endif
#else
  Status = gBS->LocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &CpuCsrAccess);
  ASSERT_EFI_ERROR (Status);
  CpuCsrAccess->BreakAtCheckpoint (Socket, MajorCode, MinorCode, Data);
#endif
}

/**
   This function obtains the PCI configuration space register offset of the specified
   Capabilities register set.

  @param[in] Socket      Device's socket number.
  @param[in] Bus         Device's bus number.
  @param[in] Device      Device's device number.
  @param[in] Function    Device's function number.
  @param[in] Cap         Desired Capability structure

  @retval  CapabilityOffset.
**/
UINT8
EFIAPI
IioPCIeGetCapOffset (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function,
  IN      UINT8   Cap
  )
{
  UINT16   PciPrimaryStatus;
  UINT8    CapabilityOffset = 0;
  UINT16   data16;

  PciPrimaryStatus = MmioRead16 (MmPciAddress (GetPcieSegment (Socket), Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET));
  if (PciPrimaryStatus & EFI_PCI_STATUS_CAPABILITY) {
    CapabilityOffset = MmioRead8 (MmPciAddress (GetPcieSegment (Socket), Bus, Device, Function, PCI_CAPBILITY_POINTER_OFFSET));
    while (TRUE) { // maximum possible number of items in list
      // format check, normal end of list is next offset = 0
      //   PCI 3.0 section 6.7 Capability List
      // "Each capability must be DWORD aligned. The bottom two bits of all pointers
      // (including the initial pointer at 34h) are reserved and must be implemented as 00b
      // although software must mask them to allow for future uses of these bits."

      if ((CapabilityOffset & DWORD_MASK) || (CapabilityOffset < CAP_OFFSET_BASE)) { // dword alignment; lower limit
        CapabilityOffset = 0;  // bad format or normal end of list, set not found
        break;          // terminate search
      }
      // CapOffset &= ~DWORD_MASK;  // not needed if b[1::0] must = 0
      // offset is good, get capabilities ID and next offset
      data16 = MmioRead16 (MmPciAddress (GetPcieSegment (Socket), Bus, Device, Function, CapabilityOffset));
      if ((UINT8) (data16 & 0xff) == Cap) {
        break;  // requested ID found
      }

      // next item
      CapabilityOffset = (UINT8)(data16 >> 8);   // b[15::8] = next ptr

    }
  }
  return CapabilityOffset;
}

/**
  This function obtains the extended PCI configuration space register offset for a
  specified Extended Capability for the specified PCI device.

  @param[in] Socket               Device's socket number.
  @param[in] Bus                  Device's bus number.
  @param[in] Device               Device's device number.
  @param[in] Function             Device's function number.
  @param[in] ExtCapabilityId      ID of the desired Extended Capability.
  @param[in] VendoreSpecificId    vendor ID that is at offset ExtCapabilityoffset + 4.

  @retval    Value                0         PCIe extended capability ID not found
                                  non-zero  Extended PCI configuration space offset of the specified Ext Capability block
**/
UINT16
EFIAPI
IioPCIeGetExtCapOffset (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function,
  IN      UINT16  ExtCapabilityId,
  IN      UINT16  VendorSpecificId
  )
{
  UINT16 ExCapOffset = 0;
  UINT32 data32;

  if (IioPCIeGetCapOffset (Socket, Bus, Device, Function, EFI_PCI_CAPABILITY_ID_PCIEXP)) {
    ExCapOffset = EFI_PCIE_CAPABILITY_BASE_OFFSET;
    while (TRUE) {  // maximum possible number of items
      // format check, normal end of list is 0
      //   PCIe 3.1 section 7.9.3 PCI Express Extended Capability Header
      // "For Extended Capabilities implemented in Configuration Space,
      // this offset is relative to the beginning of PCI compatible Configuration Space
      // and thus must always be either 000h (for terminating list of Capabilities)
      // or greater than 0FFh.
      // The bottom 2 bits of this offset are Reserved and must be implemented as 00b
      // although software must mask them to allow for future uses of these bits."
      if ((ExCapOffset & DWORD_MASK) || (ExCapOffset < EFI_PCIE_CAPABILITY_BASE_OFFSET)) {
        // dword alignment; lower limit
        ExCapOffset = 0;  // if bad format or normal end of list, set not found
        break;            //  terminate search
      }

      // offset is good, get capabilities ID and next offset
      // capabilities ID check
      data32 = MmioRead32 (MmPciAddress (GetPcieSegment (Socket), Bus, Device, Function, ExCapOffset));   // get ID
      if ((data32 & EXCAP_MASK) == ExtCapabilityId) {
        if (ExtCapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_VENDOR_SPECIFIC_ID) {
          break;  // requested Extended ID found
        } else {  // check VSEC ID
          if (MmioRead16 (MmPciAddress (GetPcieSegment (Socket), Bus, Device, Function,ExCapOffset + 4)) == VendorSpecificId) {
            break;  // requested VSCE ID found
          }
        }
      }
      // next item
      ExCapOffset = (UINT16)(data32 >> 20);  // b[31::20] = next ptr
    }
  }
  return ExCapOffset;
}

/**
  Returns address from MBAR_01 or 0 on error

  @param IioIndex   - Index to CPU/IIO
  @param Bus, Device, Function - PCIE port BDF

  @return - address or zero on error
 */
UINTN
GetAddressFromMBar01 (
    IN  UINT8                       IioIndex,
    IN  PCI_ROOT_BRIDGE_PCI_ADDRESS PciAddress
  )
{
  IIO_PTR_ADDRESS Mbar;

  Mbar.Address32bit.Value = IioPciExpressRead32 (
    IioIndex,
    PciAddress.Bus,
    PciAddress.Device,
    PciAddress.Function,
    R_PCIE_PORT_BAR_N0
  );

  if (Mbar.Address32bit.Value == 0xFFFFFFFF || (Mbar.Address32bit.Value & 0xFFFFFFF0) == 0) {
    // MBAR read error
    IIO_D_PCIERR ("%a: [idx=%d, BDF=%X:%X:%X] got invalid MBAR value 0x%08X\n",
               __FUNCTION__, IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
               Mbar.Address32bit.Value);
    return 0;
  }
  if ((Mbar.Address32bit.Value & 7) == 4) { // if 64-bit MMIO

    Mbar.Address32bit.ValueHigh = IioPciExpressRead32 (IioIndex, PciAddress.Bus,
                                                       PciAddress.Device, PciAddress.Function,
                                                       R_PCIE_PORT_BAR_N1);
  } else {

    Mbar.Address32bit.ValueHigh = 0;
  }
  //
  // cleanup information MBAR bits (lower byte)
  //
  Mbar.Address32bit.Value &= 0xFFFFFFF0;

#ifdef IA32
  if (Mbar.Address32bit.ValueHigh != 0) {
    //
    // This is PEI mode - 32-bit address is expected
    //
    IIO_D_PCIERR ("%a: [idx=%d, BDF=%X:%X:%X] 64-bit MBAR in PEI?\n",
               __FUNCTION__, IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function);
    return 0;
  }
  return Mbar.Address32bit.Value;
#else //IA32
  //
  // This is DXE phase - 64-bit address is correct
  //
  return Mbar.Address64bit;
#endif //IA32
}


/**
  For Pcie4 NTB port registers listed below are moved from PCI config space.

  @param CapRegOffset  - Register offset in Pcie cap.

  @reurn TRUE if register is moved form PCI config space
 */
BOOLEAN
IsNtbRegMovedToBar (
  IN  UINT32        CapRegOffset
  )
{
  return
      IsPCIeGen4 () &&
      (
        CapRegOffset == R_PCIE_LCAP_OFFSET  ||
        CapRegOffset == R_PCIE_LSTS_OFFSET  ||
        CapRegOffset == R_PCIE_LCTL_OFFSET  ||

        CapRegOffset == R_PCIE_LCAP2_OFFSET ||
        CapRegOffset == R_PCIE_LSTS2_OFFSET ||
        CapRegOffset == R_PCIE_LCTL2_OFFSET ||

        CapRegOffset == R_PCIE_SLCAP_OFFSET ||
        CapRegOffset == R_PCIE_SLSTS_OFFSET ||
        CapRegOffset == R_PCIE_SLCTL_OFFSET
      );
}

UINT32
GetCsrAddressForNtbCapOffset (
  IN  UINT32        CapRegOffset
  )
{
  switch (CapRegOffset){

#if defined (SNR_HOST) && !defined(TNR_HOST)
  case R_PCIE_LCAP_OFFSET: return LNKCAP_IIO_PCIENTB_REG;
  case R_PCIE_LSTS_OFFSET: return LNKSTS_IIO_PCIENTB_REG;
  case R_PCIE_LCTL_OFFSET: return LNKCON_IIO_PCIENTB_REG;

  case R_PCIE_LCAP2_OFFSET: return LNKCAP2_IIO_PCIENTB_REG;
  case R_PCIE_LSTS2_OFFSET: return LNKSTS2_IIO_PCIENTB_REG;
  case R_PCIE_LCTL2_OFFSET: return LNKCON2_IIO_PCIENTB_REG;

  case R_PCIE_SLCAP_OFFSET: return SLTCAP_IIO_PCIENTB_REG;
  case R_PCIE_SLSTS_OFFSET: return SLTSTS_IIO_PCIENTB_REG;
  case R_PCIE_SLCTL_OFFSET: return SLTCON_IIO_PCIENTB_REG;
#endif // defined (SNR_HOST) || defined (ICXDE_HOST)

  default:
    return 0;
  }
}

/**
  Read 16/32-bit register from Pcie port config space with support for NTB ports

  @param IioIndex      - Index to CPU/IIO
  @param PortIndex     - Port index.
  @param CapRegOffset  - Register offset in Pcie cap.
  @param BufSize       - Real size of given Value (16|32 bits)
  @param Value         - Value to written in the register.

  @retval EFI_SUCCESS on success
  @retval EFI_DEVICE_ERROR if can't find pcie dev. cap. or on MBAR value issue
 */
EFI_STATUS
PrivateGetPcieCapRegX (
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex,
  IN  UINT32        CapRegOffset,
  IN  UINT8         BufSize,
  OUT VOID          *OutBuffer
  )
{
  UINT8                        PCIeCapOffset;
  UINTN                        Mbar0Address;
  UINT32                       CsrAddress;
  PCI_ROOT_BRIDGE_PCI_ADDRESS  PciAddress;

  GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);

  if (IsPCIeGen4 () && CheckNtbPortConfSetting (IioIndex, PortIndex) && IsNtbRegMovedToBar (CapRegOffset)) {
    // PCI4 && NTB port && register moved from PCI conf. space behind BAR

    Mbar0Address = GetAddressFromMBar01 (IioIndex, PciAddress);
    if (Mbar0Address == 0) {
      IIO_D_PCIERR ("%a (%d:%d:%d:%d): NTB-Port: Improper address in MBAR!\n",
          __FUNCTION__, IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function);
      return EFI_DEVICE_ERROR;
    }
    switch (BufSize) {
    case 16:
      EnableMBarAccess (IioIndex, PciAddress);
      *(UINT16*) OutBuffer = IioMmioRead16 (Mbar0Address + NTB_MBAR0_PCI_CFG_OFFSET + CapRegOffset);
      DisableMBarAccess (IioIndex, PciAddress);
      break;
    case 32:
      EnableMBarAccess (IioIndex, PciAddress);
      *(UINT32*) OutBuffer = IioMmioRead32 (Mbar0Address + NTB_MBAR0_PCI_CFG_OFFSET + CapRegOffset);
      DisableMBarAccess (IioIndex, PciAddress);
      break;
    default:
      IIO_D_PCIERR ("%a: unsupported buffer size = %d!\n", __FUNCTION__, BufSize);
      return EFI_UNSUPPORTED;
    }
    return EFI_SUCCESS;
  }

  if ( !IsPCIeGen4 () && CheckNtbPortConfSetting (IioIndex, PortIndex) ) {
    // PCI3 && NTB port - some of registers has custom offsets

    CsrAddress = GetCsrAddressForNtbCapOffset (CapRegOffset);
    if (CsrAddress == 0) {
      IIO_D_PCIERR ("%a (%d:%d:%d:%d): PCIE3 NTB-Port: CSR address not found!\n",
          __FUNCTION__, IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function);
      return EFI_DEVICE_ERROR;
    }

#if defined (SNR_HOST) || defined (ICXDE_HOST) // this ifdef is here only because of Klocwork
    switch (BufSize) {
    case 16:
      *(UINT16*) OutBuffer = IioReadCpuCsr16 (IioIndex, PortIndex, CsrAddress);
      break;
    case 32:
      *(UINT32*) OutBuffer = IioReadCpuCsr32 (IioIndex, PortIndex, CsrAddress);
      break;
    default:
      IIO_D_PCIERR ("%a: unsupported buffer size = %d!\n", __FUNCTION__, BufSize);
      return EFI_UNSUPPORTED;
    }
    return EFI_SUCCESS;
#endif // defined (SNR_HOST) || defined (ICXDE_HOST)
  }

  // standard PCIe access
  PCIeCapOffset = IioPCIeGetCapOffset (
      IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function, EFI_PCI_CAPABILITY_ID_PCIEXP);

  if (PCIeCapOffset == 0) {
    IIO_D_PCIDBG ("%a (%d:%d:%d:%d): Can't find PCIeCapOffset!\n",
        __FUNCTION__, IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function);
    return EFI_DEVICE_ERROR;
  }
  switch (BufSize){
  case 16:
    *(UINT16*) OutBuffer = IioPciExpressRead16 (
        IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function, PCIeCapOffset + CapRegOffset);
    break;
  case 32:
    *(UINT32*) OutBuffer = IioPciExpressRead32 (
        IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function, PCIeCapOffset + CapRegOffset);
    break;
  default:
    IIO_D_PCIERR ("%a: unsupported buffer size = %d!\n", __FUNCTION__, BufSize);
    return EFI_UNSUPPORTED;
  }
  return EFI_SUCCESS;
}

/**
  Write 16/32-bit register of Pcie port config space with support for NTB ports

  @param IIO_GLOBAL    - Pointer to IIO_GLOBALS
  @param IioIndex      - Index to CPU/IIO
  @param PortIndex     - Port index.
  @param CapRegOffset  - Register offset in Pcie cap.
  @param BufSize       - Real size of given value (16|32 bits)
  @param Value         - Address of value to written in the register.

  @retval EFI_SUCCESS on success
  @retval EFI_DEVICE_ERROR if can't find pcie dev. cap. or on MBAR value issue
 */
EFI_STATUS
PrivateSetPcieCapRegX (
  IN  IIO_GLOBALS   *IioGlobalData,
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex,
  IN  UINT32        CapRegOffset,
  IN  UINT8         BufSize,
  IN  VOID          *Value
  )
{
  UINT8                         PCIeCapOffset;
  UINTN                         Mbar0Address;
  UINT32                        CsrAddress;
  PCI_ROOT_BRIDGE_PCI_ADDRESS   PciAddress;

  GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);

  if ( IsPCIeGen4 () && CheckNtbPortConfSetting (IioIndex, PortIndex) && IsNtbRegMovedToBar (CapRegOffset)) {
    // PCI4 && NTB port && register moved from PCI conf. space behind BAR

    Mbar0Address = GetAddressFromMBar01 (IioIndex, PciAddress);
    if (Mbar0Address == 0) {
      IIO_D_PCIERR ("%a (%d:%d:%d:%d): NTB-Port: Improper address in MBAR!\n",
          __FUNCTION__, IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function);
      return EFI_DEVICE_ERROR;
    }
    switch (BufSize) {
    case 16:
      EnableMBarAccess (IioIndex, PciAddress);
      IioMmioWrite16 (Mbar0Address + NTB_MBAR0_PCI_CFG_OFFSET + CapRegOffset, *(UINT16*) Value);
      DisableMBarAccess (IioIndex, PciAddress);
      break;
    case 32:
      EnableMBarAccess (IioIndex, PciAddress);
      IioMmioWrite32 (Mbar0Address + NTB_MBAR0_PCI_CFG_OFFSET + CapRegOffset, *(UINT32*) Value);
      DisableMBarAccess (IioIndex, PciAddress);
      break;
    default:
      IIO_D_PCIERR ("%a: unsupported buffer size = %d!\n", __FUNCTION__, BufSize);
      return EFI_UNSUPPORTED;
    }

    return EFI_SUCCESS;
  }

  if ( !IsPCIeGen4 () && CheckNtbPortConfSetting (IioIndex, PortIndex) ) {
    // PCI3 && NTB port - some of registers has custom offsets

    CsrAddress = GetCsrAddressForNtbCapOffset (CapRegOffset);
    if (CsrAddress == 0) {
      IIO_D_PCIERR ("%a (%d:%d:%d:%d): PCIE3 NTB-Port: CSR address not found!\n",
          __FUNCTION__, IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function);
      return EFI_DEVICE_ERROR;
    }
#if defined (SNR_HOST) || defined (ICXDE_HOST) // this ifdef is here only because of Klocwork

    switch (BufSize) {
    case 16:
      IioWriteCpuCsr16 (IioIndex, PortIndex, CsrAddress, *(UINT16*) Value);
      break;
    case 32:
      IioWriteCpuCsr32 (IioIndex, PortIndex, CsrAddress, *(UINT32*) Value);
      break;
    default:
      IIO_D_PCIERR ("%a: unsupported buffer size = %d!\n", __FUNCTION__, BufSize);
      return EFI_UNSUPPORTED;
    }
    return EFI_SUCCESS;
#endif // defined (SNR_HOST) || defined (ICXDE_HOST)
  }


  // standard PCIe access
  PCIeCapOffset = IioPCIeGetCapOffset (
      IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (PCIeCapOffset == 0) {
    IIO_D_PCIDBG ("%a (%d:%d:%d:%d): Can't find PCIeCapOffset!\n",
        __FUNCTION__, IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function);
    return EFI_DEVICE_ERROR ;
  }
  switch (BufSize) {
  case 16:
    IioPciExpressWrite16 (
        IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function, PCIeCapOffset + CapRegOffset, *(UINT16*) Value);
    break;
  case 32:
    IioPciExpressWrite32 (
        IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function, PCIeCapOffset + CapRegOffset, *(UINT32*) Value);
    break;
  default:
    IIO_D_PCIERR ("%a: unsupported buffer size = %d!\n", __FUNCTION__, BufSize);
    return EFI_UNSUPPORTED;
  }
  return EFI_SUCCESS;
}


/**
  Read 32-bit register from Pcie port config space with support for NTB ports

  @param IioIndex      - Index to CPU/IIO
  @param PortIndex     - Port index.
  @param CapRegOffset  - Register offset in Pcie cap.
  @param OutBuf        - Buffer to return register value.

  @retval EFI_SUCCESS on success
  @retval EFI_DEVICE_ERROR if can't find pcie dev. cap. or on Mbar value issue
 */
EFI_STATUS
GetPcieCapReg32 (
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex,
  IN  UINT32        CapRegOffset,
  OUT UINT32        *OutBuffer
  )
{
  return PrivateGetPcieCapRegX (
      IioIndex,
      PortIndex,
      CapRegOffset,
      32,
      (VOID*) OutBuffer
      );
}

/**
  Write 32-bit register of Pcie port config space with support for NTB ports

  @param IIO_GLOBAL    - Pointer to IIO_GLOBALS
  @param IioIndex      - Index to CPU/IIO
  @param PortIndex     - Port index.
  @param CapRegOffset  - Register offset in Pcie cap.
  @param Value         - Value to written in the register.

  @retval EFI_SUCCESS on success
  @retval EFI_DEVICE_ERROR if can't find pcie dev. cap. or on Mbar value issue
 */
EFI_STATUS
SetPcieCapReg32 (
  IN  IIO_GLOBALS   *IioGlobalData,
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex,
  IN  UINT32        CapRegOffset,
  IN  UINT32        Value
  )
{
  return PrivateSetPcieCapRegX (
      IioGlobalData,
      IioIndex,
      PortIndex,
      CapRegOffset,
      32,
      (VOID*) &Value
      );
}

/**
  Read 16-bit register from Pcie port config space with support for NTB ports

  @param IioIndex      - Index to CPU/IIO
  @param PortIndex     - Port index.
  @param CapRegOffset  - Register offset in Pcie cap.
  @param OutBuf        - Buffer to return register value.

  @retval EFI_SUCCESS on success
  @retval EFI_DEVICE_ERROR if can't find pcie dev. cap. or on Mbar value issue
 */
EFI_STATUS
GetPcieCapReg16 (
  IN     UINT8         IioIndex,
  IN     UINT8         PortIndex,
  IN     UINT32        CapRegOffset,
  IN OUT UINT16        *OutBuffer
  )
{
  return PrivateGetPcieCapRegX (
      IioIndex,
      PortIndex,
      CapRegOffset,
      16,
      (VOID*) OutBuffer
      );
}

/**
  Write 16-bit register of Pcie port config space with support for NTB ports

  @param IIO_GLOBAL    - Pointer to IIO_GLOBALS
  @param IioIndex      - Index to CPU/IIO
  @param PortIndex     - Port index.
  @param CapRegOffset  - Register offset in Pcie cap.
  @param Value         - Value to written in the register.

  @retval EFI_SUCCESS on success
  @retval EFI_DEVICE_ERROR if can't find pcie dev. cap. or on Mbar value issue
 */
EFI_STATUS
SetPcieCapReg16 (
  IN  IIO_GLOBALS   *IioGlobalData,
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex,
  IN  UINT32        CapRegOffset,
  IN  UINT16        Value
  )
{
  return PrivateSetPcieCapRegX (
      IioGlobalData,
      IioIndex,
      PortIndex,
      CapRegOffset,
      16,
      (VOID*) &Value
      );
}

VOID
SetBarAccess (
  IN UINT8                       IioIndex,
  IN PCI_ROOT_BRIDGE_PCI_ADDRESS PciAddress,
  IN BOOLEAN                     MseValue
  )
{
  UINT16                      CmdReg;

  //
  // Set MSE and BME bits in PCI command register, to perform MMIO access to
  // SPK_LBAR and SPK_SIZE register which is relative to the OOB_MSM_CFG_BAR0.
  //
  CmdReg = IioPciExpressRead16 (
      IioIndex,
      PciAddress.Bus,
      PciAddress.Device,
      PciAddress.Function,
      PCI_COMMAND_OFFSET
      );

  if (MseValue) {
    CmdReg |= EFI_PCI_COMMAND_MEMORY_SPACE;
  } else {
    CmdReg &= ~EFI_PCI_COMMAND_MEMORY_SPACE;
  }

  IioPciExpressWrite16 (
      IioIndex,
      PciAddress.Bus,
      PciAddress.Device,
      PciAddress.Function,
      PCI_COMMAND_OFFSET,
      CmdReg
      );
}

VOID
EnableMBarAccess (
  IN UINT8                       IioIndex,
  IN PCI_ROOT_BRIDGE_PCI_ADDRESS PciAddress
  )
{
  SetBarAccess (IioIndex, PciAddress, TRUE);
}

VOID
DisableMBarAccess (
  IN UINT8                       IioIndex,
  IN PCI_ROOT_BRIDGE_PCI_ADDRESS PciAddress
  )
{
  SetBarAccess (IioIndex, PciAddress, FALSE);
}

