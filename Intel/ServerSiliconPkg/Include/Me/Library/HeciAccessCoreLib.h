/** @file
  HECI Access driver core operations for use in PEI, DXE, and SMM.

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 Intel Corporation.

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
#ifndef _HECI_ACCESS_CORE_LIB_H_
#define _HECI_ACCESS_CORE_LIB_H_

#include <Protocol/HeciAccessProtocol.h>

#define HECI_DEFAULT_PCH               1
#define HECI_DISABLED_MBAR             0x00000000     ///< HECI memory BAR for disabled interface

/**
  HECI Access data instance definition.

  This protocol defines operations for HECI (Host Embedded Controller Interface)
  hardware access.
**/
typedef struct {
  HECI_ACCESS                  HeciAccess;         ///< HeciAccess Protocol/Ppi
  VOID                         *HeciAccessRoot;    ///< HeciAccess Protocol root
  VOID                         *HeciAccessNext;    ///< HeciAccess next Protocol

  // Device configuration
  UINTN                        PchIndex;           ///< Target PCH index 1..N (1 is Master)
  UINTN                        PciSegment;         ///< PCI segment where this HECI instance is located
  UINTN                        PciBus;             ///< PCI bus where this HECI instance is located
  UINTN                        PciDevice;          ///< PCI device where this HECI instance is located
  UINTN                        PciFunction;        ///< PCI function number of this HECI instance
  UINTN                        HeciInterruptMode;  ///< HECI interrupt mode for this HECI instance
  //
  // The MBAR field is in/out for HeciInit(). Non zero value provided on input
  // is an address to set in HECI MBAR if it is not configured yet.
  // On exit this field contains address currently used in HECI MBAR.
  // This field is not used by functions other than HeciInit().
  // MBAR register is used by send/receive/reset functions.
  //
  UINTN                        MemoryBar;          ///< Default MMIO address space assigned to this device
} HECI_ACCESS_PRIVATE;

/**
  HECI device initialization definition data structure.

  This structure describes HECI instance, i.e. PCI function on ME or IE device.
  First part of the structure is filled by driver that is initializing HECI
  device. Second part is updated by HeciInit() function.
**/
typedef struct _HECI_ACCESS_DEVICE_INITIALIZATION_DEFINITION_ {
  UINTN                 PciSegment;         ///< PCI segment where this HECI instance is located
  UINTN                 PciBus;             ///< PCI bus where this HECI instance is located
  UINTN                 PciDevice;          ///< PCI device where this HECI instance is located
  UINTN                 PciFunction;        ///< PCI function number of this HECI instance

  UINTN                 HeciInterruptMode;  ///< HECI interrupt mode for this HECI instance

  UINT32                MemoryBarPei;       ///< Default MMIO address space assigned to this device (PEI)
  UINT64                MemoryBarDxe;       ///< Default MMIO address space assigned to this device (DXE)
  UINT64                MemoryBarSmm;       ///< Default MMIO address space assigned to this device (SMM)
                                            ///< For usage before PCI enumerator, defined MemoryBar enables interface
} HECI_ACCESS_DEVICE_INITIALIZATION_DEFINITION;

#pragma pack (push, 1)
/**

  HECI MBAR Type definition

 **/
typedef union {
  UINT64   QWord;
  struct {
    UINT32 DWordL;
    UINT32 DWordH;
  } Bits;
} HECI_MBAR;
#pragma pack (pop)

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
  );

/**
  Function sets HECI Access device PPI functions

  @param[in,out]  NewHeciDevice  Pointer to HECI Access device to be set
**/
VOID
SetHeciAccessFunctions (
  IN OUT HECI_ACCESS_PRIVATE     *NewHeciDevice
  );

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
  );

/**
  Get Next HECI Access

  @param[in]     This          Pointer to HECI Access device context structure
  @param[in]     HeciAccess    Pointer to HECI Access device context structure
                               the next element should be relative to
                               Passing NULL requests first element

  @retval HECI_ACCESS Next HeciAccess object
**/
HECI_ACCESS*
GetNextHeciAccess (
  IN      HECI_ACCESS   *This,
  IN      HECI_ACCESS   *HeciAccess
  );

#endif // _HECI_ACCESS_CORE_LIB_H_
