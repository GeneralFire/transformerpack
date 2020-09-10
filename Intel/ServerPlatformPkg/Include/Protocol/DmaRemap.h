/** @file
  Protocol used to support ACPI VT-d DMA remapping reporting

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2014 Intel Corporation. <BR>

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

#ifndef __DMA_REMAP_H__
#define __DMA_REMAP_H__

//
// Protocol for GUID.
//
typedef struct _EFI_DMA_REMAP_PROTOCOL EFI_DMA_REMAP_PROTOCOL;

#define  DRHD_SIGNATURE  (('D'<<24) + ('R'<<16) + ('H'<<8) + 'D')
#define  RMRR_SIGNATURE  (('R'<<24) + ('M'<<16) + ('R'<<8) + 'R')
#define  ATSR_SIGNATURE  (('A'<<24) + ('T'<<16) + ('S'<<8) + 'R')
#define  RHSA_SIGNATURE  (('A'<<24) + ('S'<<16) + ('H'<<8) + 'R')
#define  ANDD_SIGNATURE  (('A'<<24) + ('N'<<16) + ('D'<<8) + 'D')

typedef enum {
  DrhdType,
  RmrrType,
  AtsrType,
  RhsaType
} REMAP_TYPE;

typedef enum {
  PciEndpoint = 1,
  PciBridge   = 2
} PCI_DEV_TYPE;


typedef struct {
  UINT8         Device;
  UINT8         Function;
} PCI_NODE;

typedef struct {
  UINT8         DeviceType;
  UINT8         EnumerationID;
  UINT8         StartBusNumber;
  PCI_NODE      *PciNode;
} DEVICE_SCOPE;

typedef struct {
  UINT32        Signature;
  UINT8         Flags;
  UINT16        SegmentNumber;
  UINT64        RegisterBase;
  UINTN         DeviceScopeNumber;
  DEVICE_SCOPE  *DeviceScope;
} DMAR_DRHD;

typedef struct {
  UINT32        Signature;
  UINT16        SegmentNumber;
  UINT64        RsvdMemBase;
  UINT64        RsvdMemLimit;
  UINTN         DeviceScopeNumber;
  DEVICE_SCOPE  *DeviceScope;
} DMAR_RMRR;

typedef struct {
  UINT32        Signature;
  UINT8         Flags;
  UINT16        SegmentNumber;
  UINTN         DeviceScopeNumber;
  UINT32        ATSRPresentBit;
  DEVICE_SCOPE  *DeviceScope;
} DMAR_ATSR;

typedef struct {
  UINT32        Signature;
  UINT64        RegisterBase;
  UINT32        Domian;
  UINT16        RhsaCount;
} DMAR_RHSA;

typedef
EFI_STATUS
(EFIAPI *EFI_INSERT_DMA_REMAP) (
  IN  EFI_DMA_REMAP_PROTOCOL                                    * This,
  IN  REMAP_TYPE                                                  RemapType,
  IN  VOID                                                      * RemapEntry
  );

typedef
EFI_STATUS
(EFIAPI *EFI_GET_DMAR_TABLE) (
  IN  EFI_DMA_REMAP_PROTOCOL                                    * This,
  IN  VOID                                                      **DmarTable
  );

typedef struct _EFI_DMA_REMAP_PROTOCOL {
  BOOLEAN                   VTdSupport;
  BOOLEAN                   InterruptRemap;
  BOOLEAN                   ATS;
  EFI_INSERT_DMA_REMAP      InsertDmaRemap;
  EFI_GET_DMAR_TABLE        GetDmarTable;
} EFI_DMA_REMAP_PROTOCOL;

extern EFI_GUID gEfiDmaRemapProtocolGuid;

#endif

