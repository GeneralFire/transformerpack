/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation. <BR>

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

//
// The version of NvDimm F/W Interface Table (Nfit) supported has been updated to ASWG-ECR-Draft4-RC6_93.doc
// This file contains definitions for Acpi side for the Nfit creation and publication of Nfit tables.
// A BIG NOTE: When ever the NVDIMM_FW_INTERFACE_TABLE defined either here or what is in Nfit.h of CrystalRidge Protocol driver
// changes, the same changes need to relect in both the files.

#ifndef _NFIT_DEFS_H_
#define _NFIT_DEFS_H_

#include <IndustryStandard/Acpi62.h>

#define NVDIMM_FW_INTERFACE_TABLE_SIGNATURE EFI_ACPI_6_2_NVDIMM_FIRMWARE_INTERFACE_TABLE_STRUCTURE_SIGNATURE
#define NVDIMM_FW_INTERFACE_TABLE_REVISION  EFI_ACPI_6_2_NVDIMM_FIRMWARE_INTERFACE_TABLE_REVISION

#define MAX_NFIT_SIZE                       0x18000


//
// NVDIMM Firmware Interface Table struct
// IMPORTANT NOTE: Num of various sub-table fields of the Nfit has been eliminated in the latest Nfit Spec (0.8k)
// And also since there is NO memory allocation scheme for Nfit (all globals), Adding a buffer NfitTables with the,
// max nfit size currently set to 4k. Will need to watch if we will ever overrun this buffer. It is expected that
// Nfit parser will need to parse the NfitTables field to understand the various other Nfit Tables in this buffer
// based on the Type and Length field of these tables.
//
#pragma pack(1)
typedef struct _NVDIMM_FW_INT_TBL {
  UINT32       Signature;                    // 'NFIT' should be the signature for this table
  UINT32       Length;                       // Length in bytes fir the entire table
  UINT8        Revision;                     // Revision # of this table, initial is '1'
  UINT8        Checksum;                     // Entire Table Checksum must sum to 0
  UINT8        OemID[6];                     // OemID
  UINT8        OemTblID[8];                  // Should be Manufacturer's Model #
  UINT32       OemRevision;                  // Oem Revision of for Supplied OEM Table ID
  UINT32       CreatorID;                    // Vendor ID of the utility that is creating this table
  UINT32       CreatorRev;                   // Revision of utility that is creating this table
  UINT32       Reserved;
  UINT8        NfitTables[MAX_NFIT_SIZE];    // See Description above (IMPORTANT).
} NVDIMM_FW_INTERFACE_TABLE;
#pragma pack()

#endif  // _NFIT_DEFS_H_

