/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/CxlIpLib.h>
#include <IndustryStandard/Cxl.h>
#include <PcieRegs.h>
#include <Library/KtiApi.h>
#include <Library/UncoreLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <RcRegs.h>
#include <Library/UsraCsrLib.h>
#include <Library/KtiSimLib.h>
#include <Library/RcrbAccessLib.h>


#define CXL_TOTAL_CLUSTERS_REGS  5
UINT32 CxlSncBaseRegisterOffset[CXL_TOTAL_CLUSTERS_REGS] = {SNC_BASE_1_IIO_CXL_CM_REG,
                                                            SNC_BASE_2_IIO_CXL_CM_REG,
                                                            SNC_BASE_3_IIO_CXL_CM_REG,
                                                            SNC_BASE_4_IIO_CXL_CM_REG,
                                                            SNC_BASE_5_IIO_CXL_CM_REG};

/**
  MmioWrite8 wrapper to print assembly code for emulation

  @param Address  - The MMIO register to write.
  @param Value    - The value to write to the MMIO register.

  @return Value.
**/
UINT8
EFIAPI
UbiosMmioWrite8 (
  IN UINTN    Address,
  IN UINT8    Value
  )
{
  if (UbiosGenerationEnabled ()) {
    DEBUG ((DEBUG_ERROR, "\n  mov BYTE PTR ds:[0%08xh], 0%02xh\n", Address, Value));
  }

  return MmioWrite8 (Address, Value);
}

/**
  MmioWrite16 wrapper to print assembly code for emulation

  @param Address  - The MMIO register to write.
  @param Value    - The value to write to the MMIO register.

  @return Value.
**/
UINT16
EFIAPI
UbiosMmioWrite16 (
  IN UINTN    Address,
  IN UINT16   Value
  )
{
  if (UbiosGenerationEnabled ()) {
    DEBUG ((DEBUG_ERROR, "\n  mov WORD PTR ds:[0%08xh], 0%04xh\n", Address, Value));
  }

  return MmioWrite16 (Address, Value);
}

/**
  Routine to get the "Secondary/Subordinate Bus" from CXL Downstream Port RCRB register.

  @param[in]  SocId     - Socket ID
  @param[in]  CtrId     - Controller ID
  @param[in]  RegOff    - The register offset

  @retval               - Bus number or 0 if failed.
**/
UINT8
EFIAPI
GetCxlDownstreamBus (
  IN  UINT8        SocId,
  IN  UINT8        CtrId,
  IN  UINT16       RegOff
  )
{
  UINT32   RcrbBaseAddr;
  UINT8    BusNum;

  if ((SocId >= MAX_SOCKET) || (CtrId >= MAX_CXL_PER_SOCKET)) {
    DEBUG ((DEBUG_ERROR, "\n\nInput paramater is invalid!\n\n"));
    ASSERT (FALSE);
    return 0;
  }

  if (GetCxlStatus (SocId, CtrId) != AlreadyInCxlMode) {
    DEBUG ((DEBUG_ERROR, "\n\nSocket[%x]Stack[%x] is not a CXL Port!\n\n", SocId, CtrId));
    BusNum = 0;
  } else {
    RcrbBaseAddr = (UINT32) GetRcrbBar (SocId, CtrId, TYPE_CXL_RCRB);
    if (RcrbBaseAddr == 0) {
      BusNum = 0;
    } else {
      BusNum = MmioRead8 ((UINTN) (RcrbBaseAddr + RegOff));
    }
  }

  if (BusNum == 0) {
    DEBUG ((DEBUG_ERROR, "\nGet CXL DP Port Secondary Bus# failed!\n"));
  }

  return BusNum;
}

/**
  Routine to set the "Secondary/Subordinate Bus" registers for CXL Downstream port.

  @param[in]  RcrbBaseAddr           CXL DP RCRB base address
  @param[in]  RegOff                 The register offset
  @param[in]  Bus                    The bus number to be programmed to the register

  @retval     EFI_SUCCESS            - Function return successfully.
              EFI_INVALID_PARAMETER  - Input parameter is invalid.
**/
EFI_STATUS
EFIAPI
SetCxlDownstreamBus (
  IN  UINTN      RcrbBaseAddr,
  IN  UINT16     RegOff,
  IN  UINT8      Bus
  )
{

  if ((RcrbBaseAddr & 0x1FFF) != 0) {  //CXL DP RCRB base address must be 8KB aligned
    DEBUG ((DEBUG_ERROR, "\nInvalid input parameters!!\n"));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  UbiosMmioWrite8 (RcrbBaseAddr + RegOff, Bus);

  return EFI_SUCCESS;
}

/**
  This function obtains the extended PCI configuration space register offset for a
  specified Extended Capability for the specified PCI device.

  @param[in] Socket               Device's socket number.
  @param[in] Bus                  Device's bus number.
  @param[in] Device               Device's device number.
  @param[in] Function             Device's function number.
  @param[in] ExtCapabilityId      ID of the desired Extended Capability.
  @param[in] VendoreSpecificId    Vendor ID that is at offset ExtCapabilityoffset + 4.
  @param[in] IntelVendoreSpecificId Vendor ID that is at offset ExtCapabilityoffset + 4.

  @retval    Value                0         PCIe extended capability ID not found
                                  non-zero  Extended PCI configuration space offset of the specified Ext Capability block
**/
UINT16
EFIAPI
CxlDeviceGetExtCapOffset (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function,
  IN      UINT16  ExtCapabilityId,
  IN      UINT16  VendorSpecificId,
  IN      UINT16  IntelVendorSpecificId
  )
{
  USRA_ADDRESS        TargetPciAddress;
  UINT16              ExCapOffset, Data16;
  UINT32              Data32;
  CPU_CSR_ACCESS_VAR* CpuCsrAccessVarPtr;

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar();
  ExCapOffset = EFI_PCIE_CAPABILITY_BASE_OFFSET;

  while (ExCapOffset != 0) {
    // maximum possible number of items
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
    USRA_PCIE_SEG_ADDRESS (
      TargetPciAddress,
      UsraWidth32,
      CpuCsrAccessVarPtr->segmentSocket[Socket],
      Bus,
      Device,
      Function,
      ExCapOffset
      );
    RegisterRead (&TargetPciAddress, &Data32);
    if ((Data32 & EXCAP_MASK) == ExtCapabilityId) {
      if ((ExtCapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_VENDOR_SPECIFIC_ID) &&
          (ExtCapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_DESIGNATED_VENDOR_SPECIFIC_ID)) {
        break;  // requested Extended ID found
      } else {  // check VSEC ID
        USRA_PCIE_SEG_ADDRESS (
          TargetPciAddress,
          UsraWidth16,
          CpuCsrAccessVarPtr->segmentSocket[Socket],
          Bus,
          Device,
          Function,
          ExCapOffset + 4
          );
        RegisterRead (&TargetPciAddress, &Data16);
        if ((Data16 == VendorSpecificId) || (Data16 == IntelVendorSpecificId)) {
          break;  // requested VSCE ID found
        }
      }
    }
    // next item
    ExCapOffset = (UINT16)(Data32 >> 20);  // b[31::20] = next ptr
  }

  return ExCapOffset;
}

/**
  This function will access(Read or Write) the DVSEC registers of a specified CXL Device.
  Note: The caller need to allocate a data buffer and provide the pointer to the data buffer.

  @param[in]     Socket                Device's socket number.
  @param[in]     Stack                 Box Instane, 0 based.
  @param[in]     DvsecRegOff           Offset relative to the beginning of DVSEC structure.
  @param[in]     UsraDataWidth         USRA data access width, please refer to the enum USRA_ACCESS_WIDTH.
  @param[in]     OperationType         Operation type, please refer to the enum CXL_OPERATION_TYPE.
  @param[in out] Buffer                The pointer to the data buffer.

  @retval        EFI_SUCCESS           The access is successful. If read, the data is returned in Buffer.
  @retval        EFI_INVALID_PARAMETER One of the input parameter is invalid.
  @retval        EFI_NOT_FOUND         Can't find DVSEC structure in CXL device configuration space
  @retval        EFI_UNSUPPORTED       Unsupported Operation type.

**/
EFI_STATUS
EFIAPI
CxlDeviceDvsecRegisterAccess (
  IN       UINT8              Socket,
  IN       UINT8              Stack,
  IN       UINT16             DvsecRegOff,
  IN       USRA_ACCESS_WIDTH  UsraDataWidth,
  IN       CXL_OPERATION_TYPE OperationType,
  IN  OUT  VOID               *Buffer
  )
{
  UINT8               CxlDevBus;
  UINT16              ExtCapDvsecOffset;
  USRA_ADDRESS        CxlDevPciAddress;
  CPU_CSR_ACCESS_VAR* CpuCsrAccessVarPtr;

  if (Buffer == NULL) {
    DEBUG ((DEBUG_ERROR, "\nThe Buffer pointer must not be NULL!\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (Stack >= MAX_CXL_PER_SOCKET) {
    DEBUG ((DEBUG_ERROR, "\nInvalid CXL Instance ID %x!\n", Stack));
    return EFI_INVALID_PARAMETER;
  }

  CxlDevBus = GetCxlDownstreamBus (Socket, Stack, (SECBUS_IIO_PCIE_G5_REG & 0xFFFF));
  if (CxlDevBus == 0) {
    return EFI_INVALID_PARAMETER;
  }

  ExtCapDvsecOffset = 0;
  ExtCapDvsecOffset = CxlDeviceGetExtCapOffset (
                        Socket,
                        CxlDevBus,
                        CXL_DEV_DEV,
                        CXL_DEV_FUNC,
                        PCI_EXPRESS_EXTENDED_CAPABILITY_DESIGNATED_VENDOR_SPECIFIC_ID,
                        CXL_DVSEC_VENDOR_ID,
                        INTEL_CXL_DVSEC_VENDOR_ID
                        );
  if (ExtCapDvsecOffset == 0) {
    DEBUG ((DEBUG_ERROR, "Can't find DVSEC structure in CXL device configuration space!\n"));
    return EFI_NOT_FOUND;
  }

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar();
  USRA_PCIE_SEG_ADDRESS (
    CxlDevPciAddress,
    UsraDataWidth,
    CpuCsrAccessVarPtr->segmentSocket[Socket],
    CxlDevBus,
    CXL_DEV_DEV,
    CXL_DEV_FUNC,
    ExtCapDvsecOffset + DvsecRegOff
    );

  if (OperationType == CxlOperationRead) {
    RegisterRead (&CxlDevPciAddress, Buffer);
  } else if (OperationType == CxlOperationWrite) {
    RegisterWrite (&CxlDevPciAddress, Buffer);
    //
    // Add below UBIOS log since USRA does not support UBIOS generation for PCIe access.
    //
    if (UbiosGenerationEnabled ()) {
      switch (UsraDataWidth) {
        case UsraWidth64:
          DEBUG ((DEBUG_ERROR, "\n  mov QWORD PTR ds:[0%08xh], 0%016xh\n", GetRegisterAddress (&CxlDevPciAddress), *(UINT64 *)Buffer));
          break;
        case UsraWidth32:
          DEBUG ((DEBUG_ERROR, "\n  mov DWORD PTR ds:[0%08xh], 0%08xh\n", GetRegisterAddress (&CxlDevPciAddress), *(UINT32 *)Buffer));
          break;
        case UsraWidth16:
          DEBUG ((DEBUG_ERROR, "\n  mov WORD PTR ds:[0%08xh], 0%04xh\n", GetRegisterAddress (&CxlDevPciAddress), *(UINT16 *)Buffer));
          break;
        case UsraWidth8:
        default:
          DEBUG ((DEBUG_ERROR, "\n  mov BYTE PTR ds:[0%08xh], 0%02xh\n", GetRegisterAddress (&CxlDevPciAddress), *(UINT8 *)Buffer));
          break;
      }
    }
  } else {
    DEBUG ((DEBUG_ERROR, "Unsupported operation type!\n"));
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  This function will do bit field access to the DVSEC registers of a specified CXL Device.
  Note: This function only support 16 bit register.

  @param[in]     Socket                Device's socket number.
  @param[in]     Stack                 Box Instane, 0 based.
  @param[in]     DvsecRegOff           Offset relative to the beginning of DVSEC structure.
  @param[in]     AndData               The value to AND with the read value from the MMIO register.
  @param[in]     OrData                The value to OR with the result of the AND operation.

  @retval        EFI_SUCCESS           The access is successful.
  @retval        EFI_INVALID_PARAMETER One of the input parameter is invalid or out of range.
  @retval        EFI_NOT_FOUND         Can't find DVSEC structure in CXL device configuration space.

**/
EFI_STATUS
EFIAPI
CxlDeviceDvsecRegisterAndThenOr16 (
  IN       UINT8              Socket,
  IN       UINT8              Stack,
  IN       UINT16             DvsecRegOff,
  IN       UINT16             AndData,
  IN       UINT16             OrData
  )
{
  UINT8               CxlDevBus;
  UINT16              ExtCapDvsecOffset, Data16;
  USRA_ADDRESS        CxlDevPciAddress;
  CPU_CSR_ACCESS_VAR* CpuCsrAccessVarPtr;

  if (Stack >= MAX_CXL_PER_SOCKET) {
    DEBUG ((DEBUG_ERROR, "\nInvalid CXL Instance ID %x!\n", Stack));
    return EFI_INVALID_PARAMETER;
  }

  CxlDevBus = GetCxlDownstreamBus (Socket, Stack, (SECBUS_IIO_PCIE_G5_REG & 0xFFFF));
  if (CxlDevBus == 0) {
    return EFI_INVALID_PARAMETER;
  }

  ExtCapDvsecOffset = 0;
  ExtCapDvsecOffset = CxlDeviceGetExtCapOffset (
                        Socket,
                        CxlDevBus,
                        CXL_DEV_DEV,
                        CXL_DEV_FUNC,
                        PCI_EXPRESS_EXTENDED_CAPABILITY_DESIGNATED_VENDOR_SPECIFIC_ID,
                        CXL_DVSEC_VENDOR_ID,
                        INTEL_CXL_DVSEC_VENDOR_ID
                        );
  if (ExtCapDvsecOffset == 0) {
    DEBUG ((DEBUG_ERROR, "Can't find DVSEC structure in CXL device configuration space!\n"));
    return EFI_NOT_FOUND;
  }

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar();

  USRA_PCIE_SEG_ADDRESS (
    CxlDevPciAddress,
    UsraWidth16,
    CpuCsrAccessVarPtr->segmentSocket[Socket],
    CxlDevBus,
    CXL_DEV_DEV,
    CXL_DEV_FUNC,
    ExtCapDvsecOffset + DvsecRegOff
    );

  RegisterRead (&CxlDevPciAddress, &Data16);
  Data16 &= AndData;
  Data16 |= OrData;
  RegisterWrite (&CxlDevPciAddress, &Data16);
  //
  // Add below UBIOS log since USRA does not support UBIOS generation for PCIe access.
  //
  if (UbiosGenerationEnabled ()) {
    DEBUG ((DEBUG_ERROR, "\n  mov WORD PTR ds:[0%08xh], 0%04xh\n", GetRegisterAddress (&CxlDevPciAddress), Data16));
  }

  return EFI_SUCCESS;
}

/**
  This function obtains the RCRB offset for a specified Extended Capability
  for a specified Flex Bus Port(Downstream port or Upstream port).

  @param[in] Socket               CXL's socket number.
  @param[in] Stack                Box Instane, 0 based.
  @param[in] UpstreamPort         FALSE - Downstram port; TRUE - Upstream port.
  @param[in] ExtCapabilityId      ID of the desired Extended Capability.
  @param[in] VendoreSpecificId    Vendor ID that is at offset ExtCapabilityoffset + 4.
  @param[in] IntelVendoreSpecificId Vendor ID that is at offset ExtCapabilityoffset + 4.

  @retval    Value                0         Not found the desired Extended Capability structure
                                  non-zero  The offset of the desired Extended Capability structure
**/
UINT16
EFIAPI
CxlPortGetExtCapOffset (
  IN      UINT8   Socket,
  IN      UINT8   Stack,
  IN      BOOLEAN UpstreamPort,
  IN      UINT16  ExtCapabilityId,
  IN      UINT16  VendorSpecificId,
  IN      UINT16  IntelVendorSpecificId
  )
{
  UINT16            ExtCapOffset, VendorID;
  UINT32            RcrbBaseAddr, ExtCapHeader;

  RcrbBaseAddr = (UINT32) GetRcrbBar (Socket, Stack, TYPE_CXL_RCRB);
  if (RcrbBaseAddr == 0) {
    return 0;
  }

  if (UpstreamPort) {
    RcrbBaseAddr += CXL_RCRB_BAR_SIZE_PER_PORT;
  }

  //
  // Get the start offset that points to the extended capability structure list
  //
  ExtCapHeader = MmioRead32 (RcrbBaseAddr);

  ExtCapOffset = (UINT16)(ExtCapHeader >> 20);  // Bit[31:20] = next ptr

  while (ExtCapOffset != 0) {
    //
    // PCI Express Base 4.0 Revision 1.0 Specification
    // Chapter 7.6.3 PCI Express Extended Capability Header
    // "For Extended Capabilities implemented in Configuration Space,
    // this offset is relative to the beginning of PCI compatible Configuration Space
    // and thus must always be either 000h (for terminating list of Capabilities)
    // or greater than 0FFh.
    // The bottom 2 bits of this offset are Reserved and must be implemented as 00b
    // although software must mask them to allow for future uses of these bits."
    //
    if ((ExtCapOffset & DWORD_MASK) || (ExtCapOffset < CXL_PORT_RCRB_EXTENDED_CAPABILITY_BASE_OFFSET)) {
      // dword alignment; lower limit
      ExtCapOffset = 0;  // if bad format or normal end of list, set not found
      break;            //  terminate search
    }
    // offset is good, get capabilities ID and next offset
    // capabilities ID check
    ExtCapHeader = MmioRead32 (RcrbBaseAddr + ExtCapOffset);

    if (((ExtCapHeader & EXCAP_MASK) == 0xFFFF) && ((ExtCapHeader >> 20) == 0)) {
      //
      // PCI Express Base 4.0 Revision 1.0 Specification
      // Chapter 7.6.2 Extended Capabilities in the Root Complex Register Block
      // Absence of any Extended Capabilities is required to be indicated by an Extended Capability header
      // with a Capability ID of FFFFh and a Next Capability Offset of 000h.
      //
      ExtCapOffset = 0;
      break;
    }

    if ((ExtCapHeader & EXCAP_MASK) == ExtCapabilityId) {
      if ((ExtCapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_VENDOR_SPECIFIC_ID) &&
          (ExtCapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_DESIGNATED_VENDOR_SPECIFIC_ID)) {
        break;  // requested Extended ID found
      } else {  // check DVSEC Vendor ID
        VendorID = MmioRead16 (RcrbBaseAddr
                               + ExtCapOffset
                               + OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_PORT, DesignatedVendorSpecificHeader1));
        if ((VendorID == VendorSpecificId) || (VendorID == IntelVendorSpecificId)) {
          break;  // requested VSCE ID found
        }
      }
    }

    ExtCapOffset = (UINT16)(ExtCapHeader >> 20);  // Bit[31:20] = next ptr
  }

  return ExtCapOffset;
}

/**
  This function will access(Read or Write) the DVSEC registers of a specifid CXL DP or UP port.
  Note:
  1) The caller need to allocate a data buffer and provide the pointer to the data buffer.
  2) This function only support 16 bit register read/write.

  @param[in]     Socket                CXL's socket number.
  @param[in]     Stack                 Box Instane, 0 based.
  @param[in]     UpstreamPort          FALSE - Downstram port; TRUE - Upstream port.
  @param[in]     DvsecRegOff           Offset relative to the beginning of DVSEC structure.
  @param[in]     OperationType         Operation type, please refer to the enum CXL_OPERATION_TYPE.
  @param[in out] Buffer                The pointer to the data buffer.

  @retval        EFI_SUCCESS           The access is successful. If read, the data is returned in Buffer.
  @retval        EFI_INVALID_PARAMETER One of the input parameter is invalid or out of range.
  @retval        EFI_NOT_FOUND         Can't find DVSEC structure in Flex Bus port RCRB.
  @retval        EFI_UNSUPPORTED       Unsupported operation type.

**/
EFI_STATUS
EFIAPI
CxlPortDvsecRegisterAccess16 (
  IN       UINT8              Socket,
  IN       UINT8              Stack,
  IN       BOOLEAN            UpstreamPort,
  IN       UINT16             DvsecRegOff,
  IN       CXL_OPERATION_TYPE OperationType,
  IN  OUT  UINT16             *Buffer
  )
{
  UINT16            ExtCapDvsecOffset;
  UINT32            RcrbBaseAddr;

  if (Buffer == NULL) {
    DEBUG ((DEBUG_ERROR, "\nThe Buffer pointer must not be NULL!\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (Stack >= MAX_CXL_PER_SOCKET) {
    DEBUG ((DEBUG_ERROR, "\nInvalid CXL Instance ID %x!\n", Stack));
    return EFI_INVALID_PARAMETER;
  }

  ExtCapDvsecOffset = CxlPortGetExtCapOffset (
                        Socket,
                        Stack,
                        UpstreamPort,
                        PCI_EXPRESS_EXTENDED_CAPABILITY_DESIGNATED_VENDOR_SPECIFIC_ID,
                        CXL_DVSEC_VENDOR_ID,
                        INTEL_CXL_DVSEC_VENDOR_ID
                        );

  if (ExtCapDvsecOffset == 0) {
    DEBUG ((DEBUG_ERROR, "Can't find DVSEC structure in Flex Bus port RCRB!\n"));
    return EFI_NOT_FOUND;
  }

  RcrbBaseAddr = (UINT32) GetRcrbBar (Socket, Stack, TYPE_CXL_RCRB);
  if (RcrbBaseAddr == 0) {
    return EFI_INVALID_PARAMETER;
  }

  if (UpstreamPort) {
    RcrbBaseAddr += CXL_RCRB_BAR_SIZE_PER_PORT;
  }

  if (OperationType == CxlOperationRead) {
    *Buffer = MmioRead16 (RcrbBaseAddr + ExtCapDvsecOffset + DvsecRegOff);
  } else if (OperationType == CxlOperationWrite) {
    UbiosMmioWrite16 (RcrbBaseAddr + ExtCapDvsecOffset + DvsecRegOff, *Buffer);
  } else {
    DEBUG ((DEBUG_ERROR, "\nUnsupported operation type!\n"));
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  This function will do bit field access to the DVSEC registers of a specifid CXL DP or UP port.
  Note: This function only support 16 bit register.

  @param[in]     Socket                CXL's socket number.
  @param[in]     Stack                 Box Instane, 0 based.
  @param[in]     UpstreamPort          FALSE - Downstram port; TRUE - Upstream port.
  @param[in]     DvsecRegOff           Offset relative to the beginning of DVSEC structure.
  @param[in]     AndData               The value to AND with the read value from the MMIO register.
  @param[in]     OrData                The value to OR with the result of the AND operation.

  @retval        EFI_SUCCESS           The access is successful.
  @retval        EFI_INVALID_PARAMETER One of the input parameter is invalid or out of range.
  @retval        EFI_NOT_FOUND         Can't find DVSEC structure in Flex Bus port RCRB.

**/
EFI_STATUS
EFIAPI
CxlPortDvsecRegisterAndThenOr16 (
  IN       UINT8              Socket,
  IN       UINT8              Stack,
  IN       BOOLEAN            UpstreamPort,
  IN       UINT16             DvsecRegOff,
  IN       UINT16             AndData,
  IN       UINT16             OrData
  )
{
  UINT16            ExtCapDvsecOffset, Data16;
  UINT32            RcrbBaseAddr;

  if (Stack >= MAX_CXL_PER_SOCKET) {
    DEBUG ((DEBUG_ERROR, "\nInvalid CXL Instance ID %x!\n", Stack));
    return EFI_INVALID_PARAMETER;
  }

  ExtCapDvsecOffset = CxlPortGetExtCapOffset (
                        Socket,
                        Stack,
                        UpstreamPort,
                        PCI_EXPRESS_EXTENDED_CAPABILITY_DESIGNATED_VENDOR_SPECIFIC_ID,
                        CXL_DVSEC_VENDOR_ID,
                        INTEL_CXL_DVSEC_VENDOR_ID
                        );

  if (ExtCapDvsecOffset == 0) {
    DEBUG ((DEBUG_ERROR, "Can't find DVSEC structure in Flex Bus port RCRB!\n"));
    return EFI_NOT_FOUND;
  }

  RcrbBaseAddr = (UINT32) GetRcrbBar (Socket, Stack, TYPE_CXL_RCRB);
  if (RcrbBaseAddr == 0) {
    return EFI_INVALID_PARAMETER;
  }

  if (UpstreamPort) {
    RcrbBaseAddr += CXL_RCRB_BAR_SIZE_PER_PORT;
  }

  Data16 = MmioRead16 (RcrbBaseAddr + ExtCapDvsecOffset + DvsecRegOff);
  Data16 &= AndData;
  Data16 |= OrData;
  UbiosMmioWrite16 (RcrbBaseAddr + ExtCapDvsecOffset + DvsecRegOff, Data16);

  return EFI_SUCCESS;
}

/**
  Get the CXL endpoint device's VID information

  @param SocId                - CPU Socket Node number (Socket ID)
  @param Stack                - Box Instance, 0 based

  @retval VendorId            - CXL endpoint device's VID or 0xFFFF which means no CXL devices

**/
UINT16
EFIAPI
GetCxlDevVid (
  UINT8    Socket,
  UINT8    Stack
  )
{
  UINT8               CxlDevBus;
  UINT16              VendorId;
  USRA_ADDRESS        CxlDevPciAddress;
  CPU_CSR_ACCESS_VAR* CpuCsrAccessVarPtr;

  if (Stack >= MAX_CXL_PER_SOCKET) {
    DEBUG ((DEBUG_ERROR, "\nInvalid CXL Instance ID %x!\n", Stack));
    return 0xFFFF;
  }

  CxlDevBus = GetCxlDownstreamBus (Socket, Stack, (SECBUS_IIO_PCIE_G5_REG & 0xFFFF));
  if (CxlDevBus == 0) {
    return 0xFFFF;
  }

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar();

  USRA_PCIE_SEG_ADDRESS (CxlDevPciAddress,
                         UsraWidth16,
                         CpuCsrAccessVarPtr->segmentSocket[Socket],
                         CxlDevBus,
                         CXL_DEV_DEV,
                         CXL_DEV_FUNC,
                         PCI_VENDOR_ID_OFFSET);

  RegisterRead (&CxlDevPciAddress, &VendorId);

  DEBUG ((DEBUG_INFO, "  Device VendorID = 0x%x\n", VendorId));

  return VendorId;
}

/**
  Get the CXL endpoint device's Serial number information

  @param SocId                - CPU Socket Node number (Socket ID)
  @param Stack                - Box Instance, 0 based

  @retval SerialNum           - CXL endpoint device's Serial Number info or 0 which means no such info

**/
UINT64
EFIAPI
GetCxlDevSerialNum (
  UINT8    Socket,
  UINT8    Stack
  )
{
  UINT8               CxlDevBus;
  UINT64              SerialNum;
  UINT16              ExtCapOffset;
  USRA_ADDRESS        CxlDevPciAddress;
  UINT32              DataLow, DataHigh;
  CPU_CSR_ACCESS_VAR* CpuCsrAccessVarPtr;

  if (Stack >= MAX_CXL_PER_SOCKET) {
    DEBUG ((DEBUG_ERROR, "\nInvalid CXL Instance ID %x!\n", Stack));
    return 0;
  }

  CxlDevBus = GetCxlDownstreamBus (Socket, Stack, (SECBUS_IIO_PCIE_G5_REG & 0xFFFF));
  if (CxlDevBus == 0) {
    return 0;
  }

  ExtCapOffset = 0;
  ExtCapOffset = CxlDeviceGetExtCapOffset (Socket,
                                           CxlDevBus,
                                           CXL_DEV_DEV,
                                           CXL_DEV_FUNC,
                                           PCI_EXPRESS_EXTENDED_CAPABILITY_SERIAL_NUMBER_ID,
                                           0xFF,
                                           0xFF);
  if (ExtCapOffset == 0) {
    DEBUG ((DEBUG_ERROR, "Can't find PCI_EXPRESS_EXTENDED_CAPABILITY_SERIAL_NUMBER!\n"));
    return ExtCapOffset;
  }

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar();

  USRA_PCIE_SEG_ADDRESS (CxlDevPciAddress,
                         UsraWidth32,
                         CpuCsrAccessVarPtr->segmentSocket[Socket],
                         CxlDevBus,
                         CXL_DEV_DEV,
                         CXL_DEV_FUNC,
                         ExtCapOffset + PCI_EXPRESS_EXTENDED_CAPABILITY_SERIAL_NUMBER_OFFSET);
  RegisterRead (&CxlDevPciAddress, &DataLow);  //Read the low 32-bit serial number

  CxlDevPciAddress.Pcie.Offset += 4;
  RegisterRead (&CxlDevPciAddress, &DataHigh);  //Read the high 32-bit serial number

  SerialNum = DataHigh;
  SerialNum = LShiftU64 (SerialNum, 32) | DataLow;

  DEBUG ((DEBUG_INFO, "  Device SerialNum = 0x%lx\n", SerialNum));

  return SerialNum;
}

/**
  Get the CXL Ports and Device's access addresses

  @param SocId                   - CPU Socket Node number (Socket ID)
  @param Stack                   - Box Instance, 0 based
  @param CxlProtocolList         - The CXL protocol list which is going to check, [0:0] - 1: indicate to check the cxl.$ protocol, 0: don't care cxl.$
                                                                                  [1:1] - 1: indicate to check the cxl.mem protocol, 0: don't care cxl.mem
                                   If the field is 0, means there is no need to check the protocol supported. Then it will return the CxlAccessInfo if the
                                   stack works as CXL.
  @param CxlAccessInfo           - CXL Access output structure (CXL DP RCRB Base & CXL device PCI address)

  @retval EFI_SUCCESS            - The CXL Access info is successfully returned in the output data structure.
  @retval EFI_UNSUPPORTED        - The CXL doesn't not support the input protocols.
  @retval EFI_INVALID_PARAMETER  - One of the input parameter is invalid.
  @retval EFI_NOT_FOUND          - Failed to read the CXL device's Capability register.

**/
EFI_STATUS
EFIAPI
GetCxlAccessInfo (
  IN UINT8              Socket,
  IN UINT8              Stack,
  IN CXL_PROTOCOL_LIST  CxlProtocolList,
  OUT CXL_ACCESS_INFO   *CxlAccessInfo
  )
{
  USRA_ADDRESS                          CxlDevUsraAddress;
  UINT32                                RcrbBarBase;
  UINT8                                 CxlDevBus;
  CPU_CSR_ACCESS_VAR*                   CpuCsrAccessVarPtr;
  EFI_STATUS                            Status;
  CXL_DVSEC_FLEX_BUS_DEVICE_CAPABILITY  CxlDevCap;

  ASSERT (CxlAccessInfo != NULL);

  if (Stack >= MAX_CXL_PER_SOCKET) {
    DEBUG ((DEBUG_ERROR, "\nStack id %d is over MAX_CXL_PER_SOCKET!", Stack));
    return EFI_INVALID_PARAMETER;
  }

  if (GetCxlStatus(Socket, Stack) != AlreadyInCxlMode) {
    DEBUG ((DEBUG_INFO, "\nThe Stack %d is not in CXL mode!", Stack));
    return EFI_UNSUPPORTED;
  }

  Status = CxlDeviceDvsecRegisterAccess (Socket,
                                         Stack,
                                         OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_DEVICE, DeviceCapability),
                                         UsraWidth16,
                                         CxlOperationRead,
                                         &CxlDevCap.Uint16
                                         );

  if(Status != EFI_SUCCESS){
    DEBUG ((DEBUG_ERROR, "\n Failed to read the CXL device's Capability register!\n"));
    return EFI_NOT_FOUND;
  }

  //
  // Check if it needs to check the CXL cache capability
  //
  if ((CxlProtocolList & CXL_PROTOCOL_CACHE_TYPE) != 0) {
    if (CxlDevCap.Bits.CacheCapable) {
      DEBUG ((DEBUG_INFO, "\nThe CXL device in CXL port %d doesn't support CXL.$!", Stack));
      return EFI_UNSUPPORTED;
    }
  }

  //
  // Check if it needs to check the CXL memory capability
  //
  if ((CxlProtocolList & CXL_PROTOCOL_MEM_TYPE) != 0) {
    if (CxlDevCap.Bits.MemCapable) {
      DEBUG ((DEBUG_INFO, "\nThe CXL device in CXL port %d doesn't support CXL.M!", Stack));
      return EFI_UNSUPPORTED;
    }
  }

  RcrbBarBase = (UINT32) GetRcrbBar (Socket, Stack, TYPE_CXL_RCRB);
  if (RcrbBarBase == 0) {
    CxlAccessInfo->CxlPortRcrbBar = 0;
    CxlAccessInfo->CxlDevPcieAddress = 0;
    return EFI_UNSUPPORTED;
  }
  CxlAccessInfo->CxlPortRcrbBar = RcrbBarBase;

  CxlDevBus = GetCxlDownstreamBus (Socket, Stack, (SECBUS_IIO_PCIE_G5_REG & 0xFFFF));
  if (CxlDevBus == 0) {
    CxlAccessInfo->CxlDevPcieAddress = 0;
    return EFI_UNSUPPORTED;
  }

  //
  // Point to the CXL device's pci configuration space offset 0 (VID field)
  //

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar();

  USRA_PCIE_SEG_ADDRESS (CxlDevUsraAddress,
                         UsraWidth32,
                         CpuCsrAccessVarPtr->segmentSocket[Socket],
                         CxlDevBus,
                         CXL_DEV_DEV,
                         CXL_DEV_FUNC,
                         PCI_VENDOR_ID_OFFSET);
  CxlAccessInfo->CxlDevPcieAddress = GetRegisterAddress (&CxlDevUsraAddress);

  return EFI_SUCCESS;
}

/**
  CXL:  Set SNC base address

  @param SocId        - CPU Socket Node number (Socket ID)
  @param Stack        - Box Instance, 0 based
  @param ClusterIdx   - The cluster index
  @param ClMemBase    - The cluster memory base address

  @retval             - EFI_UNSUPPORTED: the function not implemented
                        EFI_INVALID_PARAMETER   A required parameter was NULL or is out of range.
                        EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
CxlSetSncBaseAddr (
  IN UINT8  Socket,
  IN UINT8  Stack,
  IN UINT8  ClusterIdx,
  IN UINT32 ClMemBase
  )
{
  if (ClusterIdx >= CXL_TOTAL_CLUSTERS_REGS) {
    DEBUG ((DEBUG_ERROR, "CXL: ERROR!! out of bounds access detected\n"));
    ASSERT (ClusterIdx < CXL_TOTAL_CLUSTERS_REGS);
    return EFI_INVALID_PARAMETER;
  }

  UsraCsrWrite (Socket, Stack, CxlSncBaseRegisterOffset[ClusterIdx], ClMemBase);
  return EFI_SUCCESS;
}

/**
  CXL:  Set SNC upper base configuration

  @param SocId         - CPU Socket Node number (Socket ID)
  @param Stack         - Stack Id, 0 based
  @param SncUpperBase  - SNC upper base configuration

  @retval              - EFI_UNSUPPORTED: the function not implemented
                         EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
CxlSetSncUpperBase (
  IN UINT8  Socket,
  IN UINT8  Stack,
  IN UINT32 SncUpperBase
  )
{
  UsraCsrWrite (Socket, Stack, SNC_UPPER_BASE_IIO_CXL_CM_REG, SncUpperBase);

  return EFI_SUCCESS;
}

/**
  CXL:  Set Snc CFG

  @param SocId        - CPU Socket Node number (Socket ID)
  @param Stack        - Stack Id, 0 based
  @param SncConfig    - SNC configuration value

  @retval             - EFI_UNSUPPORTED: the function not implemented
                        EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
CxlSetSncCfg (
  IN UINT8  Socket,
  IN UINT8  Stack,
  IN UINT32 SncConfig
  )
{
  UsraCsrWrite (Socket, Stack, SNC_CONFIG_IIO_CXL_CM_REG, SncConfig);

  return EFI_SUCCESS;
}

/**
  CXL:  Set UMA cluster configuration

  @param SocId          - CPU Socket Node number (Socket ID)
  @param Stack          - Stack Id, 0 based
  @param UmaClusterEn   - UMA clustering enable
  @param XorDefeature   - Enable for XOR defeature mode

  @retval               - EFI_UNSUPPORTED: the function not implemented
                          EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
CxlSetUmaClusterCfg (
  IN UINT8  Socket,
  IN UINT8  Stack,
  IN UINT8  UmaClusterEn,
  IN UINT8  XorDefeatureg
  )
{
  UMA_CLUSTER_CONFIG_IIO_CXL_CM_STRUCT  UmaCluster;

  UmaCluster.Data = 0;
  UmaCluster.Bits.uma_clustering_enable = UmaClusterEn;
  UmaCluster.Bits.defeature_xor = XorDefeatureg;
  UsraCsrWrite (Socket, Stack, UMA_CLUSTER_CONFIG_IIO_CXL_CM_REG, UmaCluster.Data);

  return EFI_SUCCESS;
}

/**
  CXL:  Set Snc uncore configuration

  @param SocId            - CPU Socket Node number (Socket ID)
  @param Stack            - Stack Id, 0 based
  @param NumChaPerCluster - Number of CHAs per SNC Cluster
  @param BaseChaCluster1  - Base of CHA Cluster 1
  @param BaseChaCluster2  - Base of CHA Cluster 2
  @param BaseChaCluster3  - Base of CHA Cluster 3

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
CxlSetSncUncoreCfg (
  IN UINT8  Socket,
  IN UINT8  Stack,
  IN UINT8  NumChaPerCluster,
  IN UINT8  BaseChaCluster1,
  IN UINT8  BaseChaCluster2,
  IN UINT8  BaseChaCluster3
  )
{
  UNCORE_SNC_CONFIG_IIO_CXL_CM_STRUCT    CxlSncUncore;

  CxlSncUncore.Data = 0;
  CxlSncUncore.Bits.num_chas_per_cluster = NumChaPerCluster;
  CxlSncUncore.Bits.base_cha_cluster_1   = BaseChaCluster1;
  CxlSncUncore.Bits.base_cha_cluster_2   = BaseChaCluster2;
  CxlSncUncore.Bits.base_cha_cluster_3   = BaseChaCluster3;
  UsraCsrWrite (Socket, Stack, UNCORE_SNC_CONFIG_IIO_CXL_CM_REG, CxlSncUncore.Data);

  return EFI_SUCCESS;
}

/**
  CXL:  Set hash address mask

  @param SocId            - CPU Socket Node number (Socket ID)
  @param Stack            - Stack Id, 0 based
  @param HashAddrMask     - Hash Address [51:28] Mask
  @param ChaNumber        - Total number of CHA

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
CxlSetHashAddrMask (
  IN UINT8  Socket,
  IN UINT8  Stack,
  IN UINT32 HashAddrMask,
  IN UINT32 ChaNumber
  )
{
  HASHADDRMASK_IIO_CXL_CM_STRUCT        CxlHashAddressMask;

  CxlHashAddressMask.Data = 0;
  CxlHashAddressMask.Bits.hashaddrmask = HashAddrMask;
  //
  // Total number of Cbo's (1 -> 64) A value of 0 in
  // this field specifies a value of 64 CBo's.
  //
  if (ChaNumber == 64) {
    CxlHashAddressMask.Bits.numberofcbox = 0;
  } else {
    CxlHashAddressMask.Bits.numberofcbox   = ChaNumber;
  }
  UsraCsrWrite (Socket, Stack, HASHADDRMASK_IIO_CXL_CM_REG, CxlHashAddressMask.Data);

  return EFI_SUCCESS;
}

/**
  Routine to read, save and clear the error status registers for CXL.$M.

  @param[in]  Socket                 - Socket number.
  @param[in]  Stack                  - Box Instane, 0 based.

  @retval     EFI_SUCCESS            - Successfully collected and cleared error status.
  @retval     EFI_INVALID_PARAMETER  - Return fail due to input invalid parameter.
**/
EFI_STATUS
EFIAPI
CxlCollectAndClearErrors (
  IN UINT8                      Socket,
  IN UINT8                      Stack
  )
{
  UINT32    ErrorStatus;

  if ((Socket >= MAX_SOCKET) || (Stack >= MAX_CXL_PER_SOCKET)) {
    DEBUG ((EFI_D_ERROR, "\n\nInput paramater is invalid!\n\n"));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Save Uncorrectable Error Status and clear it if any bit is set.
  //
  ErrorStatus = UsraCsrRead (Socket, Stack, UNCORRECTABLE_ERROR_STATUS_REGISTER_IIO_CXL_CM_REG);
  DEBUG ((EFI_D_INFO, "  CXL DP Uncorrectable Error Status = 0x%08X\n", ErrorStatus));
  SaveCxlUncorrectableErrorStatus (Socket, Stack, ErrorStatus);  // Save
  if (ErrorStatus != 0) {
    UsraCsrWrite (Socket, Stack, UNCORRECTABLE_ERROR_STATUS_REGISTER_IIO_CXL_CM_REG, ErrorStatus);  // Clear - RW1CS
  }

  //
  // Clear Uncorrectable Error Mask register if any bit is set
  //
  UsraCsrWrite (Socket, Stack, UNCORRECTABLE_ERROR_MASK_REGISTER_IIO_CXL_CM_REG, 0);  // Clear - RWS

  //
  // Clear Uncorrectable Error Severity register if any bit is set
  //
  UsraCsrWrite (Socket, Stack, UNCORRECTABLE_ERROR_SEVERITY_REGISTER_IIO_CXL_CM_REG, 0);  // Clear - RWS

  //
  // Save Correctable Error Status and clear it if any bit is set.
  //
  ErrorStatus = UsraCsrRead (Socket, Stack, CORRECTABLE_ERROR_STATUS_REGISTER_IIO_CXL_CM_REG);
  DEBUG ((EFI_D_INFO, "  CXL DP Correctable   Error Status = 0x%08X\n", ErrorStatus));
  SaveCxlCorrectableErrorStatus (Socket, Stack, ErrorStatus);  // Save
  if (ErrorStatus != 0) {
    UsraCsrWrite (Socket, Stack, CORRECTABLE_ERROR_STATUS_REGISTER_IIO_CXL_CM_REG, ErrorStatus);  // Clear - RW1CS
  }

  //
  // Clear Correctable Error Mask register if any bit is set
  //
  UsraCsrWrite (Socket, Stack, CORRECTABLE_ERROR_MASK_REGISTER_IIO_CXL_CM_REG, 0);  // Clear - RWS

  return EFI_SUCCESS;
}

/**
  Routine to configure CXL security policy.

  @param[in]  Socket             - Socket number.
  @param[in]  Stack              - Box Instane, 0 based.
  @param[in]  SecurityLevel      - The security level to be set to register.

  @retval     EFI_SUCCESS            - Successfully configured security policy.
  @retval     EFI_INVALID_PARAMETER  - Failed to configure security policy.
**/
EFI_STATUS
EFIAPI
CxlConfigureSecurityPolicy (
  IN UINT8                      Socket,
  IN UINT8                      Stack,
  IN UINT8                      SecurityLevel
  )
{
  IAL_SECURITY_N0_IIO_CXL_CM_STRUCT  CxlSecurity;

  if ((Socket >= MAX_SOCKET) || (Stack >= MAX_CXL_PER_SOCKET)) {
    DEBUG ((DEBUG_ERROR, "\n\nInput paramater is invalid!\n\n"));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  CxlSecurity.Data = 0;
  CxlSecurity.Bits.security_level = SecurityLevel;
  UsraCsrWrite (Socket, Stack, IAL_SECURITY_N0_IIO_CXL_CM_REG, CxlSecurity.Data);

  return EFI_SUCCESS;
}

/**
  Set CXL DP Arb-Mux Clock Gating Enable bits: Stcge, Sdcge, Dlcge, Ltcge

  @param SocId                   - CPU Socket Node number (Socket ID)
  @param Stack                   - Box Instance, 0 based
  @param Stcge                   - Cxl Arb-Mux Stcge enable or disable
  @param Sdcge                   - Cxl Arb-Mux Sdcge enable or disable
  @param Dlcge                   - Cxl Arb-Mux Dlcge enable or disable
  @param Ltcge                   - Cxl Arb-Mux Ltcge enable or disable

  @retval

**/
VOID
EFIAPI
ConfigureCxlArbMuxCge (
  IN UINT8              Socket,
  IN UINT8              Stack,
  IN UINT8              Stcge,
  IN UINT8              Sdcge,
  IN UINT8              Dlcge,
  IN UINT8              Ltcge
  )
{
  ARBMUXCTL0_IIO_CXL_AM_STRUCT     CxlArbMuxCtl0;

  CxlArbMuxCtl0.Data = UsraCsrRead (Socket, Stack, ARBMUXCTL0_IIO_CXL_AM_REG);
  CxlArbMuxCtl0.Bits.stcge = Stcge;
  CxlArbMuxCtl0.Bits.sdcge = Sdcge;
  CxlArbMuxCtl0.Bits.dlcge = Dlcge;
  CxlArbMuxCtl0.Bits.lctge = Ltcge;
  UsraCsrWrite (Socket, Stack, ARBMUXCTL0_IIO_CXL_AM_REG, CxlArbMuxCtl0.Data);
}

/**
  Set CXL DP Arb-Mux Secured Register Lock

  @param SocId                   - CPU Socket Node number (Socket ID)
  @param Stack                   - Box Instance, 0 based

  @retval

**/
VOID
EFIAPI
SetCxlArbMuxSrl (
  IN UINT8              Socket,
  IN UINT8              Stack
  )
{
  ARBMUXCTL0_IIO_CXL_AM_STRUCT     CxlArbMuxCtl0;

  CxlArbMuxCtl0.Data = UsraCsrRead (Socket, Stack, ARBMUXCTL0_IIO_CXL_AM_REG);
  CxlArbMuxCtl0.Bits.srl = 1;
  UsraCsrWrite (Socket, Stack, ARBMUXCTL0_IIO_CXL_AM_REG, CxlArbMuxCtl0.Data);
}

/**
  Routine to program the PCICMD register in CXL DP/UP RCRB so as to enable or disable MMIO space.

  @param[in]  Socket             - Socket number (Socket ID)
  @param[in]  RcrbBaseAddr       - The base address of RCRB region
  @param[in]  EnableMse          - A flag to indicate enable or disable the mse bit. TRUE: Enable; FALSE: Disable.

  @retval     VOID

**/
VOID
EFIAPI
ProgramPciCommandRegister (
  IN  UINT8       Socket,
  IN  UINT32      RcrbBaseAddr,
  IN  BOOLEAN     EnableMse
  )
{
  UINT16       CmdReg;

  CmdReg = MmioRead16 (RcrbBaseAddr + PCI_COMMAND_OFFSET);

  if (EnableMse) {
    CmdReg |= EFI_PCI_COMMAND_MEMORY_SPACE;
  } else {
    CmdReg &= (UINT16) (~EFI_PCI_COMMAND_MEMORY_SPACE);
  }

  UbiosMmioWrite16 (RcrbBaseAddr + PCI_COMMAND_OFFSET, CmdReg);
}

/**
  Program the Clock Gate Control register of CXL.CM IP.

  @param[in]  Socket             - Socket number (Socket ID)
  @param[in]  Stack              - Stack ID
  @param[in]  ResetPrepSxAutoAck - Input "1" indicates that CXLCM IP will respond back to ResetPrep.Sx
                                   without waiting for vLSM to enter into L2; otherwise input "0".

  @retval     VOID

**/
VOID
EFIAPI
ProgramCxlCmClkGateCtrlReg (
  IN  UINT8         Socket,
  IN  UINT8         Stack,
  IN  UINT8         ResetPrepSxAutoAck
  )
{
  CLK_GATE_CTRL_IIO_CXL_CM_STRUCT  ClkGateCtrlReg;

  ClkGateCtrlReg.Data = UsraCsrRead (Socket, Stack, CLK_GATE_CTRL_IIO_CXL_CM_REG);
  ClkGateCtrlReg.Bits.force_deassert_clock_req = 1;
  ClkGateCtrlReg.Bits.resetprepsx_auto_ack = ResetPrepSxAutoAck;
  UsraCsrWrite (Socket, Stack, CLK_GATE_CTRL_IIO_CXL_CM_REG, ClkGateCtrlReg.Data);
}

/**
  This routine will configrue the link power management registers.

  @param[in]  Socket             - Socket number (Socket ID)
  @param[in]  Stack              - Stack ID

  @retval     VOID
**/
VOID
EFIAPI
ConfigureLinkPowerManagement (
  IN  UINT8         Socket,
  IN  UINT8         Stack
  )
{
  LINK_PM_HYST_TIMER_N0_IIO_CXL_CM_STRUCT  LinkPmHystTimer;
  CLK_GATE_CTRL_IIO_CXL_CM_STRUCT          ClkGateCtrlReg;

  LinkPmHystTimer.Data = UsraCsrRead (Socket, Stack, LINK_PM_HYST_TIMER_N0_IIO_CXL_CM_REG);
  LinkPmHystTimer.Bits.cfg_wait_n = 0x1FA0;
  UsraCsrWrite (Socket, Stack, LINK_PM_HYST_TIMER_N0_IIO_CXL_CM_REG, LinkPmHystTimer.Data);

  ClkGateCtrlReg.Data = UsraCsrRead (Socket, Stack, CLK_GATE_CTRL_IIO_CXL_CM_REG);
  ClkGateCtrlReg.Bits.force_deassert_clock_req = 1;
  UsraCsrWrite (Socket, Stack, CLK_GATE_CTRL_IIO_CXL_CM_REG, ClkGateCtrlReg.Data);
}
