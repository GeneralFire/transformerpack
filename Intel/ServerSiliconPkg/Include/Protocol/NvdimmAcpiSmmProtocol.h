/** @file

  Definition of the NVDIMM ACPI SMM area protocol.	This protocol
  publishes the address and format of a NVDIMM Inteface used as a communications
  buffer between SMM code and ASL code.

  Note:  Data structures defined in this protocol are not naturally aligned.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2019 Intel Corporation. <BR>

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

#ifndef _EFI_NVDIMM_ACPI_SMM_PROTOCOL_H_
#define _EFI_NVDIMM_ACPI_SMM_PROTOCOL_H_

//
// Extern the GUID for protocol users.
//
extern EFI_GUID gEfiNvdimmAcpiSmmInterfaceProtocolGuid;


//
// NVDIMM ACPI SMM Interface Protocol Revisions
//
#define EFI_NVDIMM_ACPI_SMM_INTERFACE_VERSION_1_0 0x1

//
// If any of these 2 defines change below, the same changes need to be reflected in Ngn.asi
//
#define NVDIMM_TYPE_CR     BIT0
#define NVDIMM_TYPE_JEDEC  BIT1


//
// If any of these 2 defines change below, the same changes need to be reflected in Ngn.asi
//
#define NVDIMM_MAX_INPUT_BUF_SIZE     0x10000
#define NVDIMM_MAX_OUTPUT_BUF_SIZE    0x10000

#define NVDIMM_ARS_STATUS_BUFFER_SIZE 0x10000


#pragma pack(1)
typedef struct {
  UINT32  Status;
  UINT32  BufferLength;
  UINT8   Buffer[NVDIMM_ARS_STATUS_BUFFER_SIZE];
} ARS_STATUS_DATA;
#pragma pack()

//
// This struct is used primarily for NFIT table and DSM Inteface
// (IMPORTANT NOTE: When updating this structure please update CRCM OperationRegion in Ngn.asi, this structure is shared between ACPI and SMM)
//
// NvDimmConfigBitmap0 - This is the bitmap for Aep Dimms preset in socket 0 thru 3. Let us say that there are Aep Dimms present in dimm0 of socket 0 & 1,
// the least signficant 32 bits would look like this: 00000101010101010000010101010101b. The least significant 16 bits would represent socket0, the
// 12 least significant bits are tells the bitmap of the nvdimms present while the last 4 bits are reserved. In the future Node Number will be
// encoded in these bits.
#pragma pack (1)
typedef struct NVDIMM_ACPI_SMM_INTERFACE {
  UINT64           NvDimmConfigBitmap0;// This is the bitmap of Aep Dimms present in sockets 0 thru 3.
  UINT64           NvDimmConfigBitmap1;// This is the bitmap of Aep Dimms present in sockets 4 thru 7.
  UINT64           NotificationBitmap0;
  UINT64           NotificationBitmap1;
  UINT64           EnergySourcePolicy0;// Energy Source Policy in sockets 0 thru 3.
  UINT64           EnergySourcePolicy1;// Energy Source Policy in sockets 4 thru 7.
  UINT32           FunctionIndex;      // Function Index
  UINT32           DeviceHandle;       // Uniquely identifies the Dimm
  UINT32           InputLength;        // Size of the Input Buffer in bytes
  UINT32           OutputLength;       // Output Length in bytes
  UINT64           ArsStartAddress;    // ARS SPA Start Address
  UINT64           ArsLength;          // ARS Length
  UINT16           ArsType;            // ARS Start type
  UINT8            ArsFlags;           // ARS Start flags
  UINT8            DebugInterface;     // Debug interface
  UINT32           Status;             // Status as defined above
  UINT32           NotificationDebugData;
  UINT32           NotificationProducedSequenceNumber;
  UINT32           NotificationConsumedSequenceNumber;
  UINT16           RootNotificationCounter;
  UINT8            RootNotificationPadding;
  UINT8            RootNotificationStatus;
  UINT32           RootNotificationProducedSequenceNumber;
  UINT32           RootNotificationConsumedSequenceNumber;
  UINT32           DebugLevel;
  struct {
    UINT32         NoopMbox  : 1,
                   NoDdrtSmi : 1,
                   LsxAsl    : 1,
                   Reserved  :29;
  } ImplementationControl;
  UINT8            DimmOverwriteStatus[8][8];  // [Socket][Channel]
  UINT16           LongOpStatus[8][8];
  UINT8            ArsStatusBufferLock;
  UINT8            ArsStatusProducedBuffer;
  UINT8            InjectionEnabled;
  UINT8            MaxMcChannel;
  UINT8            StopArsWithOverflow;
  UINT8            BackgroundArs;
  UINT8            Pad0[2];
  ARS_STATUS_DATA  ArsStatusData[2];
  UINT8            InputBuffer[NVDIMM_MAX_INPUT_BUF_SIZE];
  UINT8            OutputBuffer[NVDIMM_MAX_OUTPUT_BUF_SIZE];
} NVDIMM_ACPI_SMM_INTERFACE, *PNVDIMM_ACPI_SMM_INTERFACE;
#pragma pack ()

/**
  Function retrieves the NVDIMM type

  @param None

  @retval NVDIMM type
**/
typedef
UINT8
(EFIAPI *GET_ACPI_NVDIMM_TYPE) (
  VOID
  );

/**
  Function sets the NVDIMM type

  @param[in] NVDIMM type

  @retval EFI_SUCCESS             NVDIMM driver type set successfully
  @retval EFI_INVALID_PARAMETER   Unknown NVDIMM driver type
**/
typedef
EFI_STATUS
(EFIAPI *SET_ACPI_NVDIMM_TYPE) (
  IN UINT8 NvdimmType
  );

typedef struct _NVDIMM_ACPI_TYPE
{
  GET_ACPI_NVDIMM_TYPE    GetAcpiNvdimmType;
  SET_ACPI_NVDIMM_TYPE    SetAcpiNvdimmType;
} NVDIMM_ACPI_TYPE;

typedef struct _EFI_NVDIMM_ACPI_SMM_INTERFACE_PROTOCOL {
  UINT8                     Revision;
  UINT8                     Reserved[7];
  NVDIMM_ACPI_SMM_INTERFACE *Area;
  NVDIMM_ACPI_TYPE          NvdimmType;
} EFI_NVDIMM_ACPI_SMM_INTERFACE_PROTOCOL;

#endif // _EFI_NVDIMM_ACPI_SMM_PROTOCOL_H_
