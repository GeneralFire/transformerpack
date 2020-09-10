/** @file
  Definition of GUIDed HOB for reserving SMRAM regions.
  This file defines:
  the GUID used to identify the GUID HOB for reserving SMRAM regions.
  the data structure of SMRAM descriptor to describe SMRAM candidate regions
  values of state of SMRAM candidate regions
  the GUID specific data structure of HOB for reserving SMRAM regions.
  This GUIDed HOB can be used to convey the existence of the T-SEG reservation and H-SEG usage

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2014 Intel Corporation. <BR>

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

  @par Revision Reference
  GUIDs defined in SmmCis spec version 0.9.
**/

#ifndef _EFI_SMM_PEI_SMRAM_MEMORY_RESERVE_H_
#define _EFI_SMM_PEI_SMRAM_MEMORY_RESERVE_H_

#define EFI_SMM_PEI_SMRAM_MEMORY_RESERVE \
  { \
    0x6dadf1d1, 0xd4cc, 0x4910, {0xbb, 0x6e, 0x82, 0xb1, 0xfd, 0x80, 0xff, 0x3d } \
  }

/**
* GUID specific data structure of HOB for reserving SMRAM regions.
*
* Inconsistent with specification here:
* EFI_HOB_SMRAM_DESCRIPTOR_BLOCK has been changed to EFI_SMRAM_HOB_DESCRIPTOR_BLOCK.
* This inconsistency is kept in code in order for backward compatibility.
**/
typedef struct {
  ///
  /// Designates the number of possible regions in the system
  /// that can be usable for SMRAM.
  ///
  /// Inconsistent with specification here:
  /// In Framework SMM CIS 0.91 specification, it defines the field type as UINTN.
  /// However, HOBs are supposed to be CPU neutral, so UINT32 should be used instead.
  ///
  UINT32                NumberOfSmmReservedRegions;
  ///
  /// Used throughout this protocol to describe the candidate
  /// regions for SMRAM that are supported by this platform.
  ///
  EFI_SMRAM_DESCRIPTOR  Descriptor[1];
} EFI_SMRAM_HOB_DESCRIPTOR_BLOCK;

extern EFI_GUID gEfiSmmPeiSmramMemoryReserveGuid;

#endif
