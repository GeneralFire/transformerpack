/** @file
  HECI Access driver core operations for use in PEI, DXE, and SMM.

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

**/
#include <PiPei.h>
#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/HeciAccessCoreLib.h>
#include <Library/BaseMemoryLib.h>
#include <IndustryStandard/Pci22.h>
#include <Register/MeRegs.h>
#include <Register/HeciRegs.h>
#include <Library/HeciCommonLib.h>
#include <Library/MeEwlLib.h>
#include <Library/PchMultiPchBase.h>
#include <Protocol/HeciAccessProtocol.h>
#include <Protocol/HeciTransportProtocol.h>


#define HECI_ACCESS_DEBUG      "[HECI Access-%d %s]"

/**
  Local definitions.
**/


/**
  Local variables prototypes.
**/
extern CHAR16  *mHeciModuleDebugPrefix;
extern BOOLEAN mGotSmmReadyToLockEvent;

/**
  Unsupported function

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                 Signifies the width of the memory or I/O operations.
  @param  BarIndex              The BAR index of the standard PCI Configuration header to use as the
                                base address for the memory operation to perform.
  @param  Offset                The offset within the selected BAR to start the memory operation.
  @param  Mask                  Mask used for the polling criteria.
  @param  Value                 The comparison value used for the polling exit criteria.
  @param  Delay                 The number of 100 ns units to poll.
  @param  Result                Pointer to the last value read from the memory location.

  @retval EFI_UNSUPPORTED       Function is unsupported
**/
EFI_STATUS
PolMemIoUnsupported (
  IN EFI_PCI_IO_PROTOCOL           *This,
  IN  EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN  UINT8                        BarIndex,
  IN  UINT64                       Offset,
  IN  UINT64                       Mask,
  IN  UINT64                       Value,
  IN  UINT64                       Delay,
  OUT UINT64                       *Result
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Unsupported function

  @param [in]     This          A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param [in]     Width         Signifies the width of the memory operations.
  @param [in]     Offset        The offset within the PCI configuration space for the PCI controller.
  @param [in]     Count         The number of PCI configuration operations to perform.
  @param [in,out] Buffer        For read operations, the destination buffer to store the results. For write
                                operations, the source buffer to write data from.

  @retval EFI_UNSUPPORTED       Function is unsupported
**/
EFI_STATUS
HeciPciIoUnsupported (
  IN     EFI_PCI_IO_PROTOCOL          *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT32                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Unsupported function

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                 Signifies the width of the memory operations.
  @param  DestBarIndex          The BAR index in the standard PCI Configuration header to use as the
                                base address for the memory operation to perform.
  @param  DestOffset            The destination offset within the BAR specified by DestBarIndex to
                                start the memory writes for the copy operation.
  @param  SrcBarIndex           The BAR index in the standard PCI Configuration header to use as the
                                base address for the memory operation to perform.
  @param  SrcOffset             The source offset within the BAR specified by SrcBarIndex to start
                                the memory reads for the copy operation.
  @param  Count                 The number of memory operations to perform. Bytes moved is Width
                                size * Count, starting at DestOffset and SrcOffset.

  @retval EFI_UNSUPPORTED       Function is unsupported
**/
EFI_STATUS
HeciCopyMemUnsupported (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        DestBarIndex,
  IN     UINT64                       DestOffset,
  IN     UINT8                        SrcBarIndex,
  IN     UINT64                       SrcOffset,
  IN     UINTN                        Count
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Unsupported function

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Operation             Indicates if the bus master is going to read or write to system memory.
  @param  HostAddress           The system memory address to map to the PCI controller.
  @param  NumberOfBytes         On input the number of bytes to map. On output the number of bytes
                                that were mapped.
  @param  DeviceAddress         The resulting map address for the bus master PCI controller to use to
                                access the hosts HostAddress.
  @param  Mapping               A resulting value to pass to Unmap().

  @retval EFI_UNSUPPORTED       Function is unsupported

**/
EFI_STATUS
MapUnsupported (
  IN EFI_PCI_IO_PROTOCOL                *This,
  IN     EFI_PCI_IO_PROTOCOL_OPERATION  Operation,
  IN     VOID                           *HostAddress,
  IN OUT UINTN                          *NumberOfBytes,
  OUT    EFI_PHYSICAL_ADDRESS           *DeviceAddress,
  OUT    VOID                           **Mapping
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Unsupported function

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Mapping               The mapping value returned from Map().

  @retval EFI_DEVICE_ERROR      The data was not committed to the target system memory.

**/
EFI_STATUS
UnMapUnsupported (
  IN EFI_PCI_IO_PROTOCOL           *This,
  IN  VOID                         *Mapping
  )
{
   return EFI_DEVICE_ERROR;
}

/**
  Unsupported function

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Type                  This parameter is not used and must be ignored.
  @param  MemoryType            The type of memory to allocate, EfiBootServicesData or
                                EfiRuntimeServicesData.
  @param  Pages                 The number of pages to allocate.
  @param  HostAddress           A pointer to store the base system memory address of the
                                allocated range.
  @param  Attributes            The requested bit mask of attributes for the allocated range.

  @retval EFI_UNSUPPORTED       Function unsupported
**/
EFI_STATUS
AllocateUnsupported (
  IN EFI_PCI_IO_PROTOCOL           *This,
  IN  EFI_ALLOCATE_TYPE            Type,
  IN  EFI_MEMORY_TYPE              MemoryType,
  IN  UINTN                        Pages,
  OUT VOID                         **HostAddress,
  IN  UINT64                       Attributes
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Unsupported function

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Pages                 The number of pages to free.
  @param  HostAddress           The base system memory address of the allocated range.

  @retval EFI_INVALID_PARAMETER Function unsupported
**/
EFI_STATUS
FreeBufferUnsupported (
  IN EFI_PCI_IO_PROTOCOL           *This,
  IN  UINTN                        Pages,
  IN  VOID                         *HostAddress
  )
{
  return EFI_INVALID_PARAMETER;
}

/**
  Unsupported function

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.

  @retval EFI_UNSUPPORTED       Function unsupported

**/
EFI_STATUS
FlushUnsupported (
  IN EFI_PCI_IO_PROTOCOL  *This
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Unsupported function

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Operation             The operation to perform on the attributes for this PCI controller.
  @param  Attributes            The mask of attributes that are used for Set, Enable, and Disable
                                operations.
  @param  Result                A pointer to the result mask of attributes that are returned for the Get
                                and Supported operations.

  @retval EFI_UNSUPPORTED       Function unsupported
**/
EFI_STATUS
AttributesUnsupported (
  IN EFI_PCI_IO_PROTOCOL                       *This,
  IN  EFI_PCI_IO_PROTOCOL_ATTRIBUTE_OPERATION  Operation,
  IN  UINT64                                   Attributes,
  OUT UINT64                                   *Result OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Unsupported function

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  BarIndex              The BAR index of the standard PCI Configuration header to use as the
                                base address for resource range. The legal range for this field is 0..5.
  @param  Supports              A pointer to the mask of attributes that this PCI controller supports
                                setting for this BAR with SetBarAttributes().
  @param  Resources             A pointer to the resource descriptors that describe the current
                                configuration of this BAR of the PCI controller.

  @retval EFI_UNSUPPORTED       Function unsupported
**/
EFI_STATUS
GetBarAttributesUnsupported (
  IN EFI_PCI_IO_PROTOCOL             *This,
  IN  UINT8                          BarIndex,
  OUT UINT64                         *Supports, OPTIONAL
  OUT VOID                           **Resources OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Unsupported function

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Attributes            The mask of attributes to set for the resource range specified by
                                BarIndex, Offset, and Length.
  @param  BarIndex              The BAR index of the standard PCI Configuration header to use as the
                                base address for resource range. The legal range for this field is 0..5.
  @param  Offset                A pointer to the BAR relative base address of the resource range to be
                                modified by the attributes specified by Attributes.
  @param  Length                A pointer to the length of the resource range to be modified by the
                                attributes specified by Attributes.

  @retval EFI_UNSUPPORTED       Function unsupported
**/
EFI_STATUS
SetBarAttributesUnsupported (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     UINT64                       Attributes,
  IN     UINT8                        BarIndex,
  IN OUT UINT64                       *Offset,
  IN OUT UINT64                       *Length
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get Pch Number

  @param[in]      This          Pointer to HECI Access device context structure
  @param[in, out] PchIndex      Pointer to returned PCH index

  @retval EFI_SUCCESS           Success
  @retval EFI_INVALID_PARAMETER Failure, if passed valid PchIndex pointer MAX_UINTN is returned
**/
EFI_STATUS
GetHeciPch (
  IN      HECI_ACCESS_PRIVATE     *This,
  IN OUT  UINTN                   *PchIndex
  )
{
  if (PchIndex == NULL) {
    ASSERT (PchIndex != NULL);
    return EFI_INVALID_PARAMETER;
  }

  if (This == NULL) {
    ASSERT (This != NULL);
    *PchIndex = MAX_UINTN;
    return EFI_INVALID_PARAMETER;
  }
  *PchIndex = This->PchIndex;

  return EFI_SUCCESS;
}

/**
  Return HECI interrupt mode based on HECI Handle

  @param[in]      This               Pointer to HECI Access device context structure
  @param[in, out] HeciInterruptMode  Pointer to returned HeciInterruptMode

  @retval EFI_SUCCESS           Success
  @retval EFI_INVALID_PARAMETER Failure, if passed valid HeciInterruptMode pointer MAX_UINTN is returned
**/
EFI_STATUS
GetHeciInterruptMode (
  IN      HECI_ACCESS_PRIVATE     *This,
  IN OUT  UINTN                   *HeciInterruptMode
  )
{
  if (HeciInterruptMode == NULL) {
    ASSERT (HeciInterruptMode != NULL);
    return EFI_INVALID_PARAMETER;
  }

  if (This == NULL) {
    ASSERT (This != NULL);
    *HeciInterruptMode = MAX_UINTN;
    return EFI_INVALID_PARAMETER;
  }

  *HeciInterruptMode = This->HeciInterruptMode;

  return EFI_SUCCESS;
}

/**
  Read MBAR. Handle 64-bit case if bit 4 is set.
  If MBAR register is not set use the value provided in This->MemoryBar.
  Otherwise update This->MemoryBar to the value currently set in MBAR.
  The later switches HECI driver to value assigned by PCI Enumerator.
  Enable HECI MBAR, if needed. MBAR can be disabled by PCI Enumerator.

  @param[in]     This          HECI Access device context
  @param[in,out] PhysicalBar   Pointer to location where initial MBar is stored.
                               Passing NULL as a pointer informs function to read
                               MBar only without restore default value
                               in case 0 MBar has been read.

  @retval MBAR value of UINTN type is returned (32-bit in PEI, 64-bit in DXE),
         or 0 if MBAR is not configured or not enabled.
**/
UINTN
HeciMbarRead (
  IN     HECI_ACCESS_PRIVATE    *This,
  IN OUT UINTN                  *PhysicalBar
  )
{
  UINT16    Cmd;
  HECI_MBAR Mbar;
  UINT64    PciConfig;
  UINTN     HeciNumber;

  Mbar.QWord = 0;
  // make sure the structure was initialized
  if ((This == NULL) ||
      (PciSegmentRead16 (
        PCI_SEGMENT_LIB_ADDRESS (
          This->PciSegment,
          This->PciBus,
          This->PciDevice,
          This->PciFunction,
          PCI_VENDOR_ID_OFFSET
          )
        ) != V_INTEL_VENDOR_ID)) {
    // No valid device
    if (PhysicalBar != NULL) {
      *PhysicalBar = 0;
    }
    MeEwlStateFailures (
      MeEwlSeverityFatal,
      ME_EWL_CHKP_MAJOR_NA,
      ME_EWL_CHKP_MINOR_NA,
      ME_EWL_WARN_MAJOR_HECI,
      ME_EWL_WARN_HECI_PCI
      );
  } else {
    GetHeciNumber (This->PciFunction, &HeciNumber);
    PciConfig = PCI_SEGMENT_LIB_ADDRESS (
                  This->PciSegment,
                  This->PciBus,
                  This->PciDevice,
                  This->PciFunction,
                  PCI_VENDOR_ID_OFFSET
                  );
    Mbar.Bits.DWordL = PciSegmentRead32 (PciConfig + PCI_BASE_ADDRESSREG_OFFSET);
    if (Mbar.Bits.DWordL == 0xFFFFFFFF) {
      DEBUG ((DEBUG_ERROR, HECI_ACCESS_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
      DEBUG ((DEBUG_ERROR, " ERROR: Device disabled\n"));
      Mbar.Bits.DWordL = 0;
      return (UINTN)Mbar.QWord;
    }
    if (Mbar.Bits.DWordL & BIT2) { // if 64-bit address add the upper half
      Mbar.Bits.DWordH = PciSegmentRead32 (PciConfig + PCI_BASE_ADDRESSREG_OFFSET + 4);
    }
    Mbar.Bits.DWordL &= 0xFFFFFFF0; // clear address type bits
    if (PhysicalBar != NULL) {
      *PhysicalBar = (UINTN)Mbar.QWord;
      //
      // Use MemoryBar from configuration when:
      //   - PCI Memory bar is not set
      //   - we are in SMM and after SmmReadyToLock event
      //
      if ((Mbar.QWord == 0) ||
          mGotSmmReadyToLockEvent) {
        //
        // MBAR not set in HECI. If default provided in This->MemoryBar use it,
        // otherwise error.
        //
        if (This->MemoryBar != 0) {
          Mbar.QWord = This->MemoryBar;
          PciSegmentWrite32 (PciConfig + PCI_BASE_ADDRESSREG_OFFSET + 4, Mbar.Bits.DWordH);
          PciSegmentWrite32 (PciConfig + PCI_BASE_ADDRESSREG_OFFSET, Mbar.Bits.DWordL | BIT2);
        } else {
        }
      }
      if (Mbar.QWord != 0) {
        //
        // Enable the MBAR if not enabled
        //
        Cmd = PciSegmentRead16 (PciConfig + PCI_COMMAND_OFFSET);
        if ((Cmd & EFI_PCI_COMMAND_MEMORY_SPACE) == 0) {
          PciSegmentWrite16 (
            PciConfig + PCI_COMMAND_OFFSET,
            Cmd | EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE
            );
        }
      }
    }
  }

  return (UINTN)Mbar.QWord;
}

/**
  Writes HECI device memory bar in PCI device

  @param[in]      This          Pointer to HECI Access device context structure
  @param[in]      HeciMBarIn    MBar to Write
**/
VOID
HeciMbarWrite (
  IN      HECI_ACCESS_PRIVATE     *This,
  IN      UINTN                   HeciMBarIn
  )
{
  HECI_MBAR  Mbar;
  UINTN      HeciNumber;

  if (This == NULL) {
    return;
  }

  if (PciSegmentRead32 (
        PCI_SEGMENT_LIB_ADDRESS (
          This->PciSegment,
          This->PciBus,
          This->PciDevice,
          This->PciFunction,
          PCI_BASE_ADDRESSREG_OFFSET
          )
        ) == 0xFFFFFFFF) {
    GetHeciNumber (This->PciFunction, &HeciNumber);
    DEBUG ((
      DEBUG_WARN,
      "[HECI Access-%d]: %a()  Warning: Can't set (HECI device disabled)\n",
      HeciNumber,
      __FUNCTION__
      ));
    return;
  }
  Mbar.QWord = (UINT64)HeciMBarIn;

  // Store HECI MBAR
  PciSegmentWrite32 (
    PCI_SEGMENT_LIB_ADDRESS (
      This->PciSegment,
      This->PciBus,
      This->PciDevice,
      This->PciFunction,
      PCI_BASE_ADDRESSREG_OFFSET
      ),
    Mbar.Bits.DWordL
    );
  PciSegmentWrite32 (
    PCI_SEGMENT_LIB_ADDRESS (
      This->PciSegment,
      This->PciBus,
      This->PciDevice,
      This->PciFunction,
      PCI_BASE_ADDRESSREG_OFFSET + 4
      ),
    Mbar.Bits.DWordH
    );
}

/**
  Write data to HECI PCI device.
  Count 1 and Width EfiPciIoWidthUint8/16/32 are supported only.

  @param [in]     This          A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param [in]     Width         Signifies the width of the memory operations.
  @param [in]     Offset        The offset within the PCI configuration space for the PCI controller.
  @param [in]     Count         The number of PCI configuration operations to perform.
  @param [in,out] Buffer        For read operations, the destination buffer to store the results. For write
                                operations, the source buffer to write data from.


  @retval EFI_SUCCESS           The data was read from or written to the PCI controller.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI configuration header of the PCI controller.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER Buffer is NULL or Width is invalid.

**/
EFI_STATUS
HeciPciWrite (
  IN     EFI_PCI_IO_PROTOCOL          *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT32                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  HECI_ACCESS_PRIVATE *HeciAccess;
  EFI_STATUS          Status;
  UINT32              WrittenData;
  UINT64              PciConfig;

  Status = EFI_INVALID_PARAMETER;
  if ((This != NULL) &&
     (Buffer != NULL) &&
     (Count == 1)) {
    HeciAccess = (HECI_ACCESS_PRIVATE*)This;
    if (HeciAccess != NULL) {
      PciConfig = PCI_SEGMENT_LIB_ADDRESS (
                       ((HECI_ACCESS_PRIVATE *)HeciAccess)->PciSegment,
                       ((HECI_ACCESS_PRIVATE *)HeciAccess)->PciBus,
                       ((HECI_ACCESS_PRIVATE *)HeciAccess)->PciDevice,
                       ((HECI_ACCESS_PRIVATE *)HeciAccess)->PciFunction,
                       Offset
                       );
      switch (Width){
        case EfiPciIoWidthUint8:
          WrittenData = PciSegmentWrite8 (
                     PciConfig,
                     *((UINT8*)Buffer)
                     );
          if (WrittenData == *((UINT8*)Buffer)) {
            Status = EFI_SUCCESS;
          } else {
            Status = EFI_OUT_OF_RESOURCES;
          }
        break;
        case EfiPciIoWidthUint16:
          WrittenData = PciSegmentWrite16 (
                     PciConfig,
                     *((UINT16*)Buffer)
                     );
          if (WrittenData == *((UINT16*)Buffer)) {
            Status = EFI_SUCCESS;
          } else {
            Status = EFI_OUT_OF_RESOURCES;
          }
        break;
        case EfiPciIoWidthUint32:
          WrittenData = PciSegmentWrite32 (
                     PciConfig,
                     *((UINT32*)Buffer)
                     );
          if (WrittenData == *((UINT32*)Buffer)) {
            Status = EFI_SUCCESS;
          } else {
            Status = EFI_OUT_OF_RESOURCES;
          }
        break;
        default:
          Status = EFI_UNSUPPORTED;
        break;
      }
    }
  }

  return Status;
}

/**
  Read data from HECI PCI device.
  Count 1 and Width EfiPciIoWidthUint8/16/32 are supported only.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                 Signifies the width of the memory operations.
  @param  Offset                The offset within the PCI configuration space for the PCI controller.
  @param  Count                 The number of PCI configuration operations to perform.
  @param  Buffer                For read operations, the destination buffer to store the results. For write
                                operations, the source buffer to write data from.


  @retval EFI_SUCCESS           The data was read from or written to the PCI controller.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI configuration header of the PCI controller.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER Buffer is NULL or Width is invalid.

**/
EFI_STATUS
HeciPciRead (
  IN     EFI_PCI_IO_PROTOCOL          *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT32                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  HECI_ACCESS_PRIVATE *HeciAccess;
  EFI_STATUS          Status;
  UINT64              PciConfig;

  Status = EFI_INVALID_PARAMETER;
  if ((This != NULL) &&
     (Buffer != NULL) &&
     (Count == 1)) {
    HeciAccess = (HECI_ACCESS_PRIVATE*)This;
    if (HeciAccess != NULL) {
      PciConfig = PCI_SEGMENT_LIB_ADDRESS (
                       ((HECI_ACCESS_PRIVATE *)HeciAccess)->PciSegment,
                       ((HECI_ACCESS_PRIVATE *)HeciAccess)->PciBus,
                       ((HECI_ACCESS_PRIVATE *)HeciAccess)->PciDevice,
                       ((HECI_ACCESS_PRIVATE *)HeciAccess)->PciFunction,
                       Offset
                       );
      Status = EFI_SUCCESS;
      switch (Width) {
        case EfiPciIoWidthUint8:
          *((UINT8*)Buffer) = PciSegmentRead8 (PciConfig);
        break;
        case EfiPciIoWidthUint16:
          *((UINT16*)Buffer) = PciSegmentRead16 (PciConfig);
        break;
        case EfiPciIoWidthUint32:
          *((UINT32*)Buffer) = PciSegmentRead32 (PciConfig);
        break;
        default:
          Status = EFI_UNSUPPORTED;
        break;
      }
    }
  }

  return Status;
}

/**

  Enable a PCI driver to access HECI PCI controller registers in the PCI memory or I/O space.
  Bar 0, Count 1 and Width EfiPciIoWidthUint32 are supported only.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                 Signifies the width of the memory or I/O operations.
  @param  BarIndex              The BAR index of the standard PCI Configuration header to use as the
                                base address for the memory or I/O operation to perform.
  @param  Offset                The offset within the selected BAR to start the memory or I/O operation.
  @param  Count                 The number of memory or I/O operations to perform.
  @param  Buffer                For read operations, the destination buffer to store the results. For write
                                operations, the source buffer to write data from.

  @retval EFI_SUCCESS           The data was read from or written to the PCI controller.
  @retval EFI_UNSUPPORTED       BarIndex not valid for this PCI controller.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI BAR specified by BarIndex.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.

**/
EFI_STATUS
HeciMemRead (
  IN     EFI_PCI_IO_PROTOCOL          *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        BarIndex,
  IN     UINT64                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  UINTN                 HeciMemBar;
  UINTN                 HeciMemBarPhys;
  EFI_STATUS            Status;
  HECI_ACCESS_PRIVATE   *HeciAccess;

  Status = EFI_INVALID_PARAMETER;
  if ((This != NULL) &&
      (Buffer != NULL) &&
      (Count == 1) &&
      (BarIndex == 0)) {
    if (Width != EfiPciIoWidthUint32) {
      Status = EFI_UNSUPPORTED;
    } else {
      HeciAccess = (HECI_ACCESS_PRIVATE*)This;
      if (HeciAccess != NULL) {
        HeciMemBar = HeciMbarRead (
          (HECI_ACCESS_PRIVATE *)HeciAccess,
          &HeciMemBarPhys
          );
      } else {
        HeciMemBar = 0;
      }
      if (HeciMemBar != 0) {
        *((UINT32*)Buffer) = MmioRead32 (HeciMemBar + (UINTN)Offset);
        Status = EFI_SUCCESS;
      } else {
        Status = EFI_OUT_OF_RESOURCES;
      }
      if (HeciMemBarPhys != HeciMemBar) {
        HeciMbarWrite (
          (HECI_ACCESS_PRIVATE*)This,
          HeciMemBarPhys
          );
      }
    }
  }

  return Status;
}

/**

  Enable a PCI driver to access HECI PCI controller registers in the PCI memory or I/O space.
  Bar 0, Count 1 and  Width EfiPciIoWidthUint32 are supported only.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                 Signifies the width of the memory or I/O operations.
  @param  BarIndex              The BAR index of the standard PCI Configuration header to use as the
                                base address for the memory or I/O operation to perform.
  @param  Offset                The offset within the selected BAR to start the memory or I/O operation.
  @param  Count                 The number of memory or I/O operations to perform.
  @param  Buffer                For read operations, the destination buffer to store the results. For write
                                operations, the source buffer to write data from.

  @retval EFI_SUCCESS           The data was read from or written to the PCI controller.
  @retval EFI_UNSUPPORTED       BarIndex not valid for this PCI controller.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI BAR specified by BarIndex.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.

**/
EFI_STATUS
HeciMemWrite (
  IN     EFI_PCI_IO_PROTOCOL          *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        BarIndex,
  IN     UINT64                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  UINTN                 HeciMemBar;
  UINTN                 HeciMemBarPhys;
  EFI_STATUS            Status;
  UINT32                WrittenData;
  HECI_ACCESS_PRIVATE   *HeciAccess;

  Status = EFI_INVALID_PARAMETER;
  if ((This != NULL) &&
      (Buffer != NULL) &&
      (Count == 1) &&
      (BarIndex == 0)) {
    if (Width != EfiPciIoWidthUint32) {
      Status = EFI_UNSUPPORTED;
    } else {
      HeciAccess = (HECI_ACCESS_PRIVATE*)This;
      if (HeciAccess != NULL) {
        HeciMemBar = HeciMbarRead (
                       (HECI_ACCESS_PRIVATE *)HeciAccess,
                       &HeciMemBarPhys
                       );
      } else {
        HeciMemBar = 0;
      }
      Status = EFI_OUT_OF_RESOURCES;
      if (HeciMemBar != 0) {
        WrittenData = MmioWrite32 (
                   HeciMemBar + (UINTN)Offset,
                   *((UINT32*)Buffer)
                   );
        if (WrittenData == *((UINT32*)Buffer)) {
          Status = EFI_SUCCESS;
        }
      }
      if (HeciMemBarPhys != HeciMemBar) {
        HeciMbarWrite (
          (HECI_ACCESS_PRIVATE*)This,
          HeciMemBarPhys
          );
      }
    }
  }

  return Status;
}

/**
  Retrieves this PCI controller's current PCI bus number, device number, and function number.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  SegmentNumber         The PCI controller's current PCI segment number.
  @param  BusNumber             The PCI controller's current PCI bus number.
  @param  DeviceNumber          The PCI controller's current PCI device number.
  @param  FunctionNumber        The PCI controller's current PCI function number.

  @retval EFI_SUCCESS           The PCI controller location was returned.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.

**/
EFI_STATUS
EFIAPI
HeciPciIoGetLocation (
  IN EFI_PCI_IO_PROTOCOL          *This,
  OUT UINTN                       *SegmentNumber,
  OUT UINTN                       *BusNumber,
  OUT UINTN                       *DeviceNumber,
  OUT UINTN                       *FunctionNumber
  )
{
  HECI_ACCESS_PRIVATE    *HeciAccess;
  EFI_STATUS             Status;

  Status = EFI_INVALID_PARAMETER;
  if (This != NULL) {
    HeciAccess = (HECI_ACCESS_PRIVATE*)This;
    if (SegmentNumber != NULL) {
      *SegmentNumber = HeciAccess->PciSegment;
    }
    if (BusNumber != NULL) {
      *BusNumber = HeciAccess->PciBus;
    }
    if (DeviceNumber != NULL) {
      *DeviceNumber = HeciAccess->PciDevice;
    }
    if (FunctionNumber != NULL) {
      *FunctionNumber = HeciAccess->PciFunction;
    }
    Status = EFI_SUCCESS;
  } else {
    ASSERT (FALSE);
    if (SegmentNumber != NULL) {
      *SegmentNumber = MAX_UINTN;
    }
    if (BusNumber != NULL) {
      *BusNumber = MAX_UINTN;
    }
    if (DeviceNumber != NULL) {
      *DeviceNumber = MAX_UINTN;
    }
    if (FunctionNumber != NULL) {
      *FunctionNumber = MAX_UINTN;
    }
  }

  return Status;
}

/**
  Function sets PchIndex in new device
  Function is delivered by module implementation

  @param [out]    HeciDeviceInitInfo  Pointer to configuration table

  @retval Evaluated PCH index
  @retval MAX_UINTN is returned in error case
**/
UINTN
GetHeciPchIndex (
  IN     CONST HECI_ACCESS_DEVICE_INITIALIZATION_DEFINITION *HeciDeviceInitInfo
)
{
  HECI_ACCESS  *HeciAccess;
  UINTN        ReturnHeciPchIndex;
  EFI_STATUS   Status;

  ReturnHeciPchIndex = MAX_UINTN;
  if (HeciDeviceInitInfo != NULL) {
    HeciAccess = GetFirstHeciAccess ();
    // Get Max PchIndex used
    while (HeciAccess != NULL) {
      // look if for this device PchIndex is already set
      if ((((HECI_ACCESS_PRIVATE*)HeciAccess)->PciSegment == HeciDeviceInitInfo->PciSegment) &&
          (((HECI_ACCESS_PRIVATE*)HeciAccess)->PciBus == HeciDeviceInitInfo->PciBus) &&
          (((HECI_ACCESS_PRIVATE*)HeciAccess)->PciDevice == HeciDeviceInitInfo->PciDevice)) {
        // For this HECI device PCH index has been already set - use it
        break;
      }
      // next
      HeciAccess = HeciAccess->GetNextHeciAccess (HeciAccess, HeciAccess);
    }
    if (HeciAccess != NULL) {
      ReturnHeciPchIndex = ((HECI_ACCESS_PRIVATE*)HeciAccess)->PchIndex;
    } else {
      Status = FindMultiPchInfo (
                 HeciDeviceInitInfo->PciSegment,
                 HeciDeviceInitInfo->PciBus,
                 &ReturnHeciPchIndex
                 );
      if (EFI_ERROR (Status)) {
        if ((HeciDeviceInitInfo->PciSegment == 0) &&
            (HeciDeviceInitInfo->PciBus == 0)) {
          // No mPCH info, so put it on legacy for Segment and Bus 0
          // (Can be platform without mPCH support, or mPCH support has not been started yet)
          ReturnHeciPchIndex = PCH_LEGACY_ID;
        } else {
          ReturnHeciPchIndex = MAX_UINTN;
        }
      }
    }
  }

  return ReturnHeciPchIndex;
}

/**
  Function sets HECI Access device PPI functions

  @param[in,out]  NewHeciDevice            Pointer to HECI Access device to be set
**/
VOID
SetHeciAccessFunctions (
  IN OUT HECI_ACCESS_PRIVATE     *NewHeciDevice
  )
{
  if (NewHeciDevice == NULL) {
    ASSERT (NewHeciDevice != NULL);
    return;
  }

   // Set Protocol/Ppi functions
  NewHeciDevice->HeciAccess.GetHeciPch = (HECI_ACCESS_GET_PARAMETER) GetHeciPch;
  NewHeciDevice->HeciAccess.GetHeciInterruptMode = (HECI_ACCESS_GET_PARAMETER) GetHeciInterruptMode;

  NewHeciDevice->HeciAccess.GetNextHeciAccess = (HECI_ACCESS_GET_NEXT_HECI) GetNextHeciAccess;

  // Guarantee that PciIo is first field in HECI_ACCESS definition
  // Condition required to get fast reference to HECI_ACCESS when having PciIo pointer
  ASSERT ((VOID*)&(NewHeciDevice->HeciAccess) == (VOID*) &(NewHeciDevice->HeciAccess.PciIo));
  NewHeciDevice->HeciAccess.PciIo.PollMem = (EFI_PCI_IO_PROTOCOL_POLL_IO_MEM) PolMemIoUnsupported;
  NewHeciDevice->HeciAccess.PciIo.PollIo = (EFI_PCI_IO_PROTOCOL_POLL_IO_MEM) PolMemIoUnsupported;
  NewHeciDevice->HeciAccess.PciIo.Mem.Read = (EFI_PCI_IO_PROTOCOL_IO_MEM) HeciMemRead;
  NewHeciDevice->HeciAccess.PciIo.Mem.Write = (EFI_PCI_IO_PROTOCOL_IO_MEM) HeciMemWrite;
  NewHeciDevice->HeciAccess.PciIo.Io.Read = (EFI_PCI_IO_PROTOCOL_IO_MEM) HeciPciIoUnsupported;
  NewHeciDevice->HeciAccess.PciIo.Io.Write = (EFI_PCI_IO_PROTOCOL_IO_MEM) HeciPciIoUnsupported;
  NewHeciDevice->HeciAccess.PciIo.Pci.Read = (EFI_PCI_IO_PROTOCOL_CONFIG) HeciPciRead;
  NewHeciDevice->HeciAccess.PciIo.Pci.Write = (EFI_PCI_IO_PROTOCOL_CONFIG) HeciPciWrite;
  NewHeciDevice->HeciAccess.PciIo.CopyMem = (EFI_PCI_IO_PROTOCOL_COPY_MEM) HeciCopyMemUnsupported;
  NewHeciDevice->HeciAccess.PciIo.Map = (EFI_PCI_IO_PROTOCOL_MAP) MapUnsupported;
  NewHeciDevice->HeciAccess.PciIo.Unmap = (EFI_PCI_IO_PROTOCOL_UNMAP) UnMapUnsupported;
  NewHeciDevice->HeciAccess.PciIo.AllocateBuffer = (EFI_PCI_IO_PROTOCOL_ALLOCATE_BUFFER) AllocateUnsupported;
  NewHeciDevice->HeciAccess.PciIo.FreeBuffer = (EFI_PCI_IO_PROTOCOL_FREE_BUFFER) FreeBufferUnsupported;
  NewHeciDevice->HeciAccess.PciIo.Flush = (EFI_PCI_IO_PROTOCOL_FLUSH) FlushUnsupported;
  NewHeciDevice->HeciAccess.PciIo.GetLocation = (EFI_PCI_IO_PROTOCOL_GET_LOCATION) HeciPciIoGetLocation;
  NewHeciDevice->HeciAccess.PciIo.Attributes = (EFI_PCI_IO_PROTOCOL_ATTRIBUTES) AttributesUnsupported;
  NewHeciDevice->HeciAccess.PciIo.GetBarAttributes = (EFI_PCI_IO_PROTOCOL_GET_BAR_ATTRIBUTES) GetBarAttributesUnsupported;
  NewHeciDevice->HeciAccess.PciIo.SetBarAttributes = (EFI_PCI_IO_PROTOCOL_SET_BAR_ATTRIBUTES) SetBarAttributesUnsupported;
}

/**
  Function sets new HECI Access device based on configuration record

  @param[in,out]  NewHeciDevice            HECI Access device to be set pointer
  @param[in]      HeciDeviceInitInfo       HECI Configuration record
  @param[in]      InSmm                    Indicates if we are configuring SMM device

  @retval TRUE   Device is set correctly
  @retval FALSE  Device can't be set
**/
BOOLEAN
HeciSetNewDevice (
  IN OUT HECI_ACCESS_PRIVATE                                *NewHeciDevice,
  IN     CONST HECI_ACCESS_DEVICE_INITIALIZATION_DEFINITION *HeciDeviceInitInfo,
  IN     BOOLEAN                                            InSmm
  )
{
  UINT16      PciDeviceId;
  UINT16      PciVendorId;
  UINTN       HeciNumber;
  UINTN       PchIndex;

  if ((NewHeciDevice == NULL) || (HeciDeviceInitInfo == NULL)) {
    return FALSE;
  }

  PchIndex = MAX_UINTN;
  PciDeviceId = PciSegmentRead16 (
                  PCI_SEGMENT_LIB_ADDRESS (
                    HeciDeviceInitInfo->PciSegment,
                    HeciDeviceInitInfo->PciBus,
                    HeciDeviceInitInfo->PciDevice,
                    HeciDeviceInitInfo->PciFunction,
                    PCI_DEVICE_ID_OFFSET)
                  );
  GetHeciNumber (HeciDeviceInitInfo->PciFunction, &HeciNumber);
  if (PciDeviceId != 0xFFFF) {
    PciVendorId = PciSegmentRead16 (
                    PCI_SEGMENT_LIB_ADDRESS (
                      HeciDeviceInitInfo->PciSegment,
                      HeciDeviceInitInfo->PciBus,
                      HeciDeviceInitInfo->PciDevice,
                      HeciDeviceInitInfo->PciFunction,
                      PCI_VENDOR_ID_OFFSET)
                    );
    if (PciVendorId == V_INTEL_VENDOR_ID) {
      PchIndex = GetHeciPchIndex (HeciDeviceInitInfo);
    } else {
      DEBUG ((DEBUG_WARN, HECI_ACCESS_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
      DEBUG ((
        DEBUG_WARN,
        " WARNING: Set failed, PCI device %d/%d/%d is not valid HECI (%2X-%2X)\n",
        HeciDeviceInitInfo->PciBus,
        HeciDeviceInitInfo->PciDevice,
        HeciDeviceInitInfo->PciFunction,
        PciVendorId,
        PciDeviceId
        ));
    }
  }
  if (PchIndex != MAX_UINTN) {
    NewHeciDevice->PchIndex = PchIndex;
    NewHeciDevice->PciSegment = HeciDeviceInitInfo->PciSegment;
    NewHeciDevice->PciBus = HeciDeviceInitInfo->PciBus;
    NewHeciDevice->PciDevice = HeciDeviceInitInfo->PciDevice;
    NewHeciDevice->PciFunction = HeciDeviceInitInfo->PciFunction;
    NewHeciDevice->HeciInterruptMode = HeciDeviceInitInfo->HeciInterruptMode;
    if (InSmm) {
      NewHeciDevice->MemoryBar = (UINTN)HeciDeviceInitInfo->MemoryBarSmm;
    } else {
      if (sizeof (UINT64) == sizeof (UINTN)) {
        NewHeciDevice->MemoryBar = (UINTN)HeciDeviceInitInfo->MemoryBarDxe;
      } else {
        NewHeciDevice->MemoryBar = (UINTN)HeciDeviceInitInfo->MemoryBarPei;
      }
    }
    DEBUG ((DEBUG_INFO, HECI_ACCESS_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
    DEBUG ((
      DEBUG_INFO,
      " Set Heci Access Interface (Pch: %d Seg: %d Bus: %d Dev: %d Func: %d)\n",
      NewHeciDevice->PchIndex,
      NewHeciDevice->PciSegment,
      NewHeciDevice->PciBus,
      NewHeciDevice->PciDevice,
      NewHeciDevice->PciFunction
      ));

    SetHeciAccessFunctions (NewHeciDevice);

    NewHeciDevice->HeciAccess.PciIo.RomSize = 0;
    NewHeciDevice->HeciAccess.PciIo.RomImage = NULL;
  } else {
    DEBUG ((DEBUG_WARN, HECI_ACCESS_DEBUG, HeciNumber, mHeciModuleDebugPrefix));
    DEBUG ((
      DEBUG_WARN,
      " WARNING: Set failed, PCI device %d/%d/%d is disabled\n",
      HeciDeviceInitInfo->PciBus,
      HeciDeviceInitInfo->PciDevice,
      HeciDeviceInitInfo->PciFunction
      ));
  }

  return (BOOLEAN)(PchIndex != MAX_UINTN);
}
