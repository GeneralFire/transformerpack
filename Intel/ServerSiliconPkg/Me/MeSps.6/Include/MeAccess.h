/** @file
  Macros to simplify and abstract the interface to PCI configuration.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2010 - 2019 Intel Corporation. <BR>

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

#ifndef _ME_ACCESS_H_
#define _ME_ACCESS_H_
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Register/MeRegs.h>
#include "MkhiMsgs.h"

//
// HECI PCI Access Macro
//
#define HeciPciRead32(Register) PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, Register))

#define HeciPciWrite32(Register, Data) \
  PciSegmentWrite32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, Register), \
  (UINT32) Data \
  )

#define HeciPciOr32(Register, Data) \
  PciSegmentOr32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, Register), \
  (UINT32) Data \
  )

#define HeciPciAnd32(Register, Data) \
  PciSegmentAnd32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, Register), \
  (UINT32) Data \
  )

#define HeciPciAndThenOr32(Register, AndData, OrData) \
  PciSegmentAndThenOr32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, Register), \
  (UINT32) AndData, \
  (UINT32) OrData \
  )

#define HeciPciRead16(Register) PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, Register))

#define HeciPciWrite16(Register, Data) \
  PciSegmentWrite16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, Register), \
  (UINT16) Data \
  )

#define HeciPciOr16(Register, Data) \
  PciSegmentOr16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, Register), \
  (UINT16) Data \
  )

#define HeciPciAnd16(Register, Data) \
  PciSegmentAnd16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, Register), \
  (UINT16) Data \
  )

#define HeciPciAndThenOr16(Register, AndData, OrData) \
  PciSegmentAndThenOr16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, Register), \
  (UINT16) AndData, \
  (UINT16) OrData \
  )

#define HeciPciRead8(Register)  PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, Register))

#define HeciPciWrite8(Register, Data) \
  PciSegmentWrite8 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, Register), \
  (UINT8) Data \
  )

#define HeciPciOr8(Register, Data) \
  PciSegmentOr8 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, Register), \
  (UINT8) Data \
  )

#define HeciPciAnd8(Register, Data) \
  PciSegmentAnd8 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, Register), \
  (UINT8) Data \
  )

#define HeciPciAndThenOr8(Register, AndData, OrData) \
  PciSegmentAndThenOr8 ( \
  (UINT8) AndData, \
  (UINT8) OrData \
  )

//
// HECI2 PCI Access Macro
//
#define Heci2PciRead32(Register)  PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, Register))

#define Heci2PciWrite32(Register, Data) \
  PciSegmentWrite32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, Register), \
  (UINT32) Data \
  )

#define Heci2PciOr32(Register, Data) \
  PciSegmentOr32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, Register), \
  (UINT32) Data \
  )

#define Heci2PciAnd32(Register, Data) \
  PciSegmentAnd32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, Register), \
  (UINT32) Data \
  )

#define Heci2PciAndThenOr32(Register, AndData, OrData) \
  PciSegmentAndThenOr32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, Register), \
  (UINT32) AndData, \
  (UINT32) OrData \
  )

#define Heci2PciRead16(Register)  PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, Register))

#define Heci2PciWrite16(Register, Data) \
  PciSegmentWrite16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, Register), \
  (UINT16) Data \
  )

#define Heci2PciOr16(Register, Data) \
  PciSegmentOr16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, Register), \
  (UINT16) Data \
  )

#define Heci2PciAnd16(Register, Data) \
  PciSegmentAnd16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, Register), \
  (UINT16) Data \
  )

#define Heci2PciAndThenOr16(Register, AndData, OrData) \
  PciSegmentAndThenOr16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, Register), \
  (UINT16) AndData, \
  (UINT16) OrData \
  )

#define Heci2PciRead8(Register) PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, Register))

#define Heci2PciWrite8(Register, Data) \
  PciSegmentWrite8 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, Register), \
  (UINT8) Data \
  )

#define Heci2PciOr8(Register, Data) \
  PciSegmentOr8 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, Register), \
  (UINT8) Data \
  )

#define Heci2PciAnd8(Register, Data) \
  PciSegmentAnd8 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, Register), \
  (UINT8) Data \
  )

#define Heci2PciAndThenOr8(Register, AndData, OrData) \
  PciSegmentAndThenOr8 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, Register), \
  (UINT8) AndData, \
  (UINT8) OrData \
  )

//
// HECI3 PCI Access Macro
//
#define Heci3PciRead32(Register)  PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI3_FUNCTION_NUMBER, Register))

#define Heci3PciWrite32(Register, Data) \
  PciSegmentWrite32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI3_FUNCTION_NUMBER, Register), \
  (UINT32) Data \
  )

#define Heci3PciOr32(Register, Data) \
  PciSegmentOr32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI3_FUNCTION_NUMBER, Register), \
  (UINT32) Data \
  )

#define Heci3PciAnd32(Register, Data) \
  PciSegmentAnd32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI3_FUNCTION_NUMBER, Register), \
  (UINT32) Data \
  )

#define Heci3PciAndThenOr32(Register, AndData, OrData) \
  PciSegmentAndThenOr32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI3_FUNCTION_NUMBER, Register), \
  (UINT32) AndData, \
  (UINT32) OrData \
  )

#define Heci3PciRead16(Register)  PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI3_FUNCTION_NUMBER, Register))

#define Heci3PciWrite16(Register, Data) \
  PciSegmentWrite16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI3_FUNCTION_NUMBER, Register), \
  (UINT16) Data \
  )

#define Heci3PciOr16(Register, Data) \
  PciSegmentOr16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI3_FUNCTION_NUMBER, Register), \
  (UINT16) Data \
  )

#define Heci3PciAnd16(Register, Data) \
  PciSegmentAnd16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI3_FUNCTION_NUMBER, Register), \
  (UINT16) Data \
  )

#define Heci3PciAndThenOr16(Register, AndData, OrData) \
  PciSegmentAndThenOr16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI3_FUNCTION_NUMBER, Register), \
  (UINT16) AndData, \
  (UINT16) OrData \
  )

#define Heci3PciRead8(Register) PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI3_FUNCTION_NUMBER, Register))

#define Heci3PciWrite8(Register, Data) \
  PciSegmentWrite8 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI3_FUNCTION_NUMBER, Register), \
  (UINT8) Data \
  )

#define Heci3PciOr8(Register, Data) \
  PciSegmentOr8 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI3_FUNCTION_NUMBER, Register), \
  (UINT8) Data \
  )

#define Heci3PciAnd8(Register, Data) \
  PciSegmentAnd8 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI3_FUNCTION_NUMBER, Register), \
  (UINT8) Data \
  )

#define Heci3PciAndThenOr8(Register, AndData, OrData) \
  PciSegmentAndThenOr8 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI3_FUNCTION_NUMBER, Register), \
  (UINT8) AndData, \
  (UINT8) OrData \
  )

//
// HECI4 PCI Access Macro
//
#define Heci4PciRead32(Register)  PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI4_FUNCTION_NUMBER, Register))

#define Heci4PciWrite32(Register, Data) \
  PciSegmentWrite32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI4_FUNCTION_NUMBER, Register), \
  (UINT32) Data \
  )

#define Heci4PciOr32(Register, Data) \
  PciSegmentOr32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI4_FUNCTION_NUMBER, Register), \
  (UINT32) Data \
  )

#define Heci4PciAnd32(Register, Data) \
  PciSegmentAnd32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI4_FUNCTION_NUMBER, Register), \
  (UINT32) Data \
  )

#define Heci4PciAndThenOr32(Register, AndData, OrData) \
  PciSegmentAndThenOr32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI4_FUNCTION_NUMBER, Register), \
  (UINT32) AndData, \
  (UINT32) OrData \
  )

#define Heci4PciRead16(Register)  PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI4_FUNCTION_NUMBER, Register))

#define Heci4PciWrite16(Register, Data) \
  PciSegmentWrite16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI4_FUNCTION_NUMBER, Register), \
  (UINT16) Data \
  )

#define Heci4PciOr16(Register, Data) \
  PciSegmentOr16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI4_FUNCTION_NUMBER, Register), \
  (UINT16) Data \
  )

#define Heci4PciAnd16(Register, Data) \
  PciSegmentAnd16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI4_FUNCTION_NUMBER, Register), \
  (UINT16) Data \
  )

#define Heci4PciAndThenOr16(Register, AndData, OrData) \
  PciSegmentAndThenOr16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI4_FUNCTION_NUMBER, Register), \
  (UINT16) AndData, \
  (UINT16) OrData \
  )

#define Heci4PciRead8(Register) PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI4_FUNCTION_NUMBER, Register))

#define Heci4PciWrite8(Register, Data) \
  PciSegmentWrite8 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI4_FUNCTION_NUMBER, Register), \
  (UINT8) Data \
  )

#define Heci4PciOr8(Register, Data) \
  PciSegmentOr8 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI4_FUNCTION_NUMBER, Register), \
  (UINT8) Data \
  )

#define Heci4PciAnd8(Register, Data) \
  PciSegmentAnd8 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI4_FUNCTION_NUMBER, Register), \
  (UINT8) Data \
  )

#define Heci4PciAndThenOr8(Register, AndData, OrData) \
  PciSegmentAndThenOr8 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI4_FUNCTION_NUMBER, Register), \
  (UINT8) AndData, \
  (UINT8) OrData \
  )

#define MmPci32Ptr(Segment, Bus, Device, Function, Register) \
    ((VOLATILE UINT32 *) MmPciAddress (Segment, Bus, Device, Function, Register))
#define MmPci32(Segment, Bus, Device, Function, Register) *MmPci32Ptr (Segment, Bus, Device, Function, Register)

//
// ME Macro Setting
//
#define McD0PciCfg64(Register)              PciSegmentRead64 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, 0, 0, Register))
#define McD0PciCfg32And(Register, AndData)  PciSegmentAnd32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, 0, 0, Register), (UINT32) AndData)
#define McD0PciCfg16And(Register, AndData)  PciSegmentAnd16 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, 0, 0, Register), (UINT16) AndData)

///
/// Device '22', Function 0
///
#define PchD22PciCfg32(Register)                            MmPci32 (0, 0, ME_DEVICE_NUMBER, 0, Register)
#define PchD22PciCfg32Or(Register, OrData)                  MmPci32Or (0, 0, ME_DEVICE_NUMBER, 0, Register, OrData)
#define PchD22PciCfg32And(Register, AndData)                MmPci32And (0, 0, ME_DEVICE_NUMBER, 0, Register, AndData)
#define PchD22PciCfg32AndThenOr(Register, AndData, OrData)  MmPci32AndThenOr (0, 0, ME_DEVICE_NUMBER, 0, Register, AndData, OrData)

//
//  MMIO access macros
//
#define PciSegmentAddress(BaseAddr, Register) ((UINTN) BaseAddr + (UINTN) (Register))

//
// 16 bit MMIO access
//
#define PciSegment16Ptr(BaseAddr, Register) ((VOLATILE UINT16 *) PciSegmentAddress (BaseAddr, Register))

#define PciSegment16(BaseAddr, Register)    *PciSegment16Ptr (BaseAddr, Register)

#define PciSegment16Or(BaseAddr, Register, OrData) \
  PciSegment16 (BaseAddr, Register) = (UINT16) \
    (PciSegment16 (BaseAddr, Register) | (UINT16) (OrData))

#define PciSegment16And(BaseAddr, Register, AndData) \
  PciSegment16 (BaseAddr, Register) = (UINT16) \
    (PciSegment16 (BaseAddr, Register) & (UINT16) (AndData))

#define PciSegment16AndThenOr(BaseAddr, Register, AndData, OrData) \
  PciSegment16 (BaseAddr, Register) = (UINT16) \
    ((PciSegment16 (BaseAddr, Register) & (UINT16) (AndData)) | (UINT16) (OrData))

//
// 32 bit MMIO access
//
#define PciSegment32Ptr(BaseAddr, Register) ((VOLATILE UINT32 *) PciSegmentAddress (BaseAddr, Register))

#define PciSegment32(BaseAddr, Register)    *PciSegment32Ptr (BaseAddr, Register)

#define PciSegment32Or(BaseAddr, Register, OrData) \
  PciSegment32 (BaseAddr, Register) = (UINT32) \
    (PciSegment32 (BaseAddr, Register) | (UINT32) (OrData))

#define PciSegment32And(BaseAddr, Register, AndData) \
  PciSegment32 (BaseAddr, Register) = (UINT32) \
    (PciSegment32 (BaseAddr, Register) & (UINT32) (AndData))

#define PciSegment32AndThenOr(BaseAddr, Register, AndData, OrData) \
  PciSegment32 (BaseAddr, Register) = (UINT32) \
    ((PciSegment32 (BaseAddr, Register) & (UINT32) (AndData)) | (UINT32) (OrData))

//
// Memory Controller PCI access macros
//
#define MCH_REGION_BASE               (McD0PciCfg64 (MC_MCHBAR_OFFSET) &~BIT0)
#define McPciSegmentAddress(Register)       ((UINTN) MCH_REGION_BASE + (UINTN) (Register))

#define McPciSegment8Ptr(Register)          ((VOLATILE UINT8 *) McPciSegmentAddress (Register))
#define McPciSegment8(Register)             *McPciSegment8Ptr (Register)
#define McPciSegment8Or(Register, OrData)   (McPciSegment8 (Register) |= (UINT8) (OrData))
#define McPciSegment8And(Register, AndData) (McPciSegment8 (Register) &= (UINT8) (AndData))
#define McPciSegment8AndThenOr(Register, AndData, OrData) \
    (McPciSegment8 (Register) = (McPciSegment8 (Register) & (UINT8) (AndData)) | (UINT8) (OrData))

#endif // _ME_ACCESS_H_
