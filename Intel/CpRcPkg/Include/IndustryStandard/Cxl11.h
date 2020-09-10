/** @file

  This is the library class header file for CXL industry standard lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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
#ifndef _CXL_1_1_H_
#define _CXL_1_1_H_

#include <IndustryStandard/PciExpress40.h>

// Width = 16
#define INTEL_CXL_DVSEC_VENDOR_ID                                       0x8086
#define CXL_DVSEC_VENDOR_ID                                             0x1E98


//
// CXL Flex Bus Dev BDF = Bus:0:0
// CXL Specification Rev 1.1 - Chapter 7.1.1
//
#define CXL_DEV_DEV                                                     0
#define CXL_DEV_FUNC                                                    0

//
// CXL DVSEC for Flex Bus Dev
// CXL Specification Rev 1.1 - Chapter 7.1.1 - Figure 95
//

//
// Ensure proper structure formats
//
#pragma pack(1)

///
/// The PCIe DVSEC for Flex Bus device
///
typedef union {
  struct {
    UINT16 CacheCapable                                         : 1; // bit 0
    UINT16 IoCapable                                            : 1; // bit 1
    UINT16 MemCapable                                           : 1; // bit 2
    UINT16 MemHwInitMode                                        : 1; // bit 3
    UINT16 HdmCount                                             : 2; // bit 4..5
    UINT16 Reserved1                                            : 8; // bit 6..13
    UINT16 ViralCapable                                         : 1; // bit 14
    UINT16 Reserved2                                            : 1; // bit 15
  }Bits;
  UINT16                                                        Uint16;
} CXL_DVSEC_FLEX_BUS_DEVICE_CAPABILITY;

typedef union {
  struct {
    UINT16 CacheEnable                                          : 1; // bit 0
    UINT16 IoEnable                                             : 1; // bit 1
    UINT16 MemEnable                                            : 1; // bit 2
    UINT16 CacheSfCoverage                                      : 5; // bit 3..7
    UINT16 CacheSfGranularity                                   : 3; // bit 8..10
    UINT16 CacheCleanEviction                                   : 1; // bit 11
    UINT16 Reserved1                                            : 2; // bit 12..13
    UINT16 ViralEnable                                          : 1; // bit 14
    UINT16 Reserved2                                            : 1; // bit 15
  }Bits;
  UINT16                                                        Uint16;
} CXL_DVSEC_FLEX_BUS_DEVICE_CONTROL;

typedef union {
  struct {
    UINT16 Reserved1                                            : 14; // bit 0..13
    UINT16 ViralStatus                                          : 1;  // bit 14
    UINT16 Reserved2                                            : 1;  // bit 15
  }Bits;
  UINT16                                                        Uint16;
} CXL_DVSEC_FLEX_BUS_DEVICE_STATUS;

typedef union {
  struct {
    UINT16 Reserved1                                            : 1;  // bit 0
    UINT16 Reserved2                                            : 1;  // bit 1
    UINT16 Reserved3                                            : 1;  // bit 2
    UINT16 Reserved4                                            : 13; // bit 3..15
  }Bits;
  UINT16                                                        Uint16;
} CXL_1_1_DVSEC_FLEX_BUS_DEVICE_CONTROL2;

typedef union {
  struct {
    UINT16 Reserved1                                            : 1;  // bit 0
    UINT16 Reserved2                                            : 1;  // bit 1
    UINT16 Reserved3                                            : 14; // bit 2..15
  }Bits;
  UINT16                                                        Uint16;
} CXL_1_1_DVSEC_FLEX_BUS_DEVICE_STATUS2;

typedef union {
  struct {
    UINT16 ConfigLock                                           : 1;  // bit 0
    UINT16 Reserved1                                            : 15; // bit 1..15
  }Bits;
  UINT16                                                        Uint16;
} CXL_DVSEC_FLEX_BUS_DEVICE_LOCK;

typedef union {
  struct {
    UINT32 MemorySizeHigh                                       : 32; // bit 0..31
  }Bits;
  UINT32                                                        Uint32;
} CXL_DVSEC_FLEX_BUS_DEVICE_RANGE1_SIZE_HIGH;

typedef union {
  struct {
    UINT32 MemoryInfoValid                                      : 1;  // bit 0
    UINT32 MemoryActive                                         : 1;  // bit 1
    UINT32 MediaType                                            : 3;  // bit 2..4
    UINT32 MemoryClass                                          : 3;  // bit 5..7
    UINT32 DesiredInterleave                                    : 3;  // bit 8..10
    UINT32 Reserved                                             : 17; // bit 11..27
    UINT32 MemorySizeLow                                        : 4;  // bit 28..31
  }Bits;
  UINT32                                                        Uint32;
} CXL_DVSEC_FLEX_BUS_DEVICE_RANGE1_SIZE_LOW;

typedef union {
  struct {
    UINT32 MemoryBaseHigh                                       : 32; // bit 0..31
  }Bits;
  UINT32                                                        Uint32;
} CXL_DVSEC_FLEX_BUS_DEVICE_RANGE1_BASE_HIGH;

typedef union {
  struct {
    UINT32 Reserved                                             : 28; // bit 0..27
    UINT32 MemoryBaseLow                                        : 4;  // bit 28..31
  }Bits;
  UINT32                                                        Uint32;
} CXL_DVSEC_FLEX_BUS_DEVICE_RANGE1_BASE_LOW;


typedef union {
  struct {
    UINT32 MemorySizeHigh                                       : 32; // bit 0..31
  }Bits;
  UINT32                                                        Uint32;
} CXL_DVSEC_FLEX_BUS_DEVICE_RANGE2_SIZE_HIGH;

typedef union {
  struct {
    UINT32 MemoryInfoValid                                      : 1;  // bit 0
    UINT32 MemoryActive                                         : 1;  // bit 1
    UINT32 MediaType                                            : 3;  // bit 2..4
    UINT32 MemoryClass                                          : 3;  // bit 5..7
    UINT32 DesiredInterleave                                    : 3;  // bit 8..10
    UINT32 Reserved                                             : 17; // bit 11..27
    UINT32 MemorySizeLow                                        : 4;  // bit 28..31
  }Bits;
  UINT32                                                        Uint32;
} CXL_DVSEC_FLEX_BUS_DEVICE_RANGE2_SIZE_LOW;

typedef union {
  struct {
    UINT32 MemoryBaseHigh                                       : 32; // bit 0..31
  }Bits;
  UINT32                                                        Uint32;
} CXL_DVSEC_FLEX_BUS_DEVICE_RANGE2_BASE_HIGH;

typedef union {
  struct {
    UINT32 Reserved                                             : 28; // bit 0..27
    UINT32 MemoryBaseLow                                        : 4;  // bit 28..31
  }Bits;
  UINT32                                                        Uint32;
} CXL_DVSEC_FLEX_BUS_DEVICE_RANGE2_BASE_LOW;

typedef struct {
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER                      Header;                           // offset 0
  PCI_EXPRESS_DESIGNATED_VENDOR_SPECIFIC_HEADER_1               DesignatedVendorSpecificHeader1;  // offset 4
  PCI_EXPRESS_DESIGNATED_VENDOR_SPECIFIC_HEADER_2               DesignatedVendorSpecificHeader2;  // offset 8
  CXL_DVSEC_FLEX_BUS_DEVICE_CAPABILITY                          DeviceCapability;                 // offset 10
  CXL_DVSEC_FLEX_BUS_DEVICE_CONTROL                             DeviceControl;                    // offset 12
  CXL_DVSEC_FLEX_BUS_DEVICE_STATUS                              DeviceStatus;                     // offset 14
  CXL_1_1_DVSEC_FLEX_BUS_DEVICE_CONTROL2                        DeviceControl2;                   // offset 16
  CXL_1_1_DVSEC_FLEX_BUS_DEVICE_STATUS2                         DeviceStatus2;                    // offset 18
  CXL_DVSEC_FLEX_BUS_DEVICE_LOCK                                DeviceLock;                       // offset 20
  UINT16                                                        Reserved;                         // offset 22
  CXL_DVSEC_FLEX_BUS_DEVICE_RANGE1_SIZE_HIGH                    DeviceRange1SizeHigh;             // offset 24
  CXL_DVSEC_FLEX_BUS_DEVICE_RANGE1_SIZE_LOW                     DeviceRange1SizeLow;              // offset 28
  CXL_DVSEC_FLEX_BUS_DEVICE_RANGE1_BASE_HIGH                    DeviceRange1BaseHigh;             // offset 32
  CXL_DVSEC_FLEX_BUS_DEVICE_RANGE1_BASE_LOW                     DeviceRange1BaseLow;              // offset 36
  CXL_DVSEC_FLEX_BUS_DEVICE_RANGE2_SIZE_HIGH                    DeviceRange2SizeHigh;             // offset 40
  CXL_DVSEC_FLEX_BUS_DEVICE_RANGE2_SIZE_LOW                     DeviceRange2SizeLow;              // offset 44
  CXL_DVSEC_FLEX_BUS_DEVICE_RANGE2_BASE_HIGH                    DeviceRange2BaseHigh;             // offset 48
  CXL_DVSEC_FLEX_BUS_DEVICE_RANGE2_BASE_LOW                     DeviceRange2BaseLow;              // offset 52
} CXL_1_1_DVSEC_FLEX_BUS_DEVICE;


//
// CXL DVSEC for Flex Bus Port
// CXL Specification Rev 1.1 - Chapter 7.2.1.3 - Figure 99
//

///
/// PCIe DVSEC for FLex Bus Port
///
typedef union {
  struct {
    UINT16 CacheCapable                                         : 1;  // bit 0
    UINT16 IoCapable                                            : 1;  // bit 1
    UINT16 MemCapable                                           : 1;  // bit 2
    UINT16 Reserved                                             : 13; // bit 3..15
  }Bits;
  UINT16                                                        Uint16;
} CXL_1_1_DVSEC_FLEX_BUS_PORT_CAPABILITY;

typedef union {
  struct {
    UINT16 CacheEnable                                          : 1; // bit 0
    UINT16 IoEnable                                             : 1; // bit 1
    UINT16 MemEnable                                            : 1; // bit 2
    UINT16 CxlSyncBypassEnable                                  : 1; // bit 3
    UINT16 DriftBufferEnable                                    : 1; // bit 4
    UINT16 Reserved                                             : 3; // bit 5..7
    UINT16 Retimer1Present                                      : 1; // bit 8
    UINT16 Retimer2Present                                      : 1; // bit 9
    UINT16 Reserved2                                            : 6; // bit 10..15
  }Bits;
  UINT16                                                        Uint16;
} CXL_1_1_DVSEC_FLEX_BUS_PORT_CONTROL;

typedef union {
  struct {
  UINT16 CacheEnable                                            : 1; // bit 0
  UINT16 IoEnable                                               : 1; // bit 1
  UINT16 MemEnable                                              : 1; // bit 2
  UINT16 CxlSyncBypassEnable                                    : 1; // bit 3
  UINT16 DriftBufferEnable                                      : 1; // bit 4
  UINT16 Reserved                                               : 3; // bit 5..7
  UINT16 CxlCorrectableProtocolIdFramingError                   : 1; // bit 8
  UINT16 CxlUncorrectableProtocolIdFramingError                 : 1; // bit 9
  UINT16 CxlUnexpectedProtocolIdDropped                         : 1; // bit 10
  UINT16 Reserved2                                              : 5; // bit 11..15
  }Bits;
  UINT16                                                        Uint16;
} CXL_1_1_DVSEC_FLEX_BUS_PORT_STATUS;

typedef struct {
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER                      Header;                           // offset 0
  PCI_EXPRESS_DESIGNATED_VENDOR_SPECIFIC_HEADER_1               DesignatedVendorSpecificHeader1;  // offset 4
  PCI_EXPRESS_DESIGNATED_VENDOR_SPECIFIC_HEADER_2               DesignatedVendorSpecificHeader2;  // offset 8
  CXL_1_1_DVSEC_FLEX_BUS_PORT_CAPABILITY                        PortCapability;                   // offset 10
  CXL_1_1_DVSEC_FLEX_BUS_PORT_CONTROL                           PortControl;                      // offset 12
  CXL_1_1_DVSEC_FLEX_BUS_PORT_STATUS                            PortStatus;                       // offset 14
} CXL_1_1_DVSEC_FLEX_BUS_PORT;

//
// CXL RCRB Registers
// CXL Specification Rev 1.1 - Chapter 7.2.1.1 - Figure 97
//
// Width = 32
#define CXL_PORT_RCRB_MEMBAR0_LOW_OFFSET                                0x010
#define CXL_PORT_RCRB_MEMBAR0_HIGH_OFFSET                               0x014
#define CXL_PORT_RCRB_EXTENDED_CAPABILITY_BASE_OFFSET                   0x100

#pragma pack()

#endif //_CXL_1_1_H_
