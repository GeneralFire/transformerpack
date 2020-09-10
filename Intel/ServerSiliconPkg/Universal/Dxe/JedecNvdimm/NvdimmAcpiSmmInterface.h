/** @file NvdimmAcpiSmmInterface.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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
**/

#ifndef _JEDEC_ACPI_SMM_INTERFACE_H_
#define _JEDEC_ACPI_SMM_INTERFACE_H_


//
// If any of these 2 defines change below, the same changes need to be reflected in Nvdr.asl
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
// (IMPORTANT NOTE: When updating this structure please update CRCM OperationRegion in Nvdr.asl, this structure is shared between ACPI and SMM)
//
// NvDimmConfigBitmap0 - This is the bitmap for DCPMMs present in socket 0 thru 3. Let us say that there are DCPMMs present in dimm0 of socket 0 & 1,
// the least signficant 32 bits would look like this: 00000101010101010000010101010101b. The least significant 16 bits would represent socket0, the
// 12 least significant bits are tells the bitmap of the nvdimms present while the last 4 bits are reserved. In the future Node Number will be
// encoded in these bits.
#pragma pack (1)
typedef struct NVDIMM_ACPI_SMM_INTERFACE {
  UINT64           NvDimmConfigBitmap0;// This is the bitmap of DCPMMs present in sockets 0 thru 3.
  UINT64           NvDimmConfigBitmap1;// This is the bitmap of DCPMMs present in sockets 4 thru 7.
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
  UINT32           DebugLevel;
  struct {
    UINT32         NoopMbox  : 1,
                   NoDdrtSmi : 1,
                   LsxAsl    : 1,
                   Reserved  :29;
  } ImplementationControl;
  UINT8            MaxMcChannel;
  UINT8            InputBuffer[NVDIMM_MAX_INPUT_BUF_SIZE];
  UINT8            OutputBuffer[NVDIMM_MAX_OUTPUT_BUF_SIZE];
} NVDIMM_ACPI_SMM_INTERFACE;
#pragma pack ()


/**
  Initializes interface between ACPI and SMM

  @retval EFI_SUCCESS                         NVDIMM ACPI SMM interface initialized successfully.

**/
EFI_STATUS
InitializeNvdimmAcpiSmmInterface (
  VOID
  );

#endif //_JEDEC_ACPI_SMM_INTERFACE_H_
