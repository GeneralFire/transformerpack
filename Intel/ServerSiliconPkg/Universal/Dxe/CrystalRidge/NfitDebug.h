/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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
#ifndef _NFIT_DEBUG_H_
#define _NFIT_DEBUG_H_


#define NFITDEBUG(Expr) \
        { if (mSystemMemoryMap->MemSetup.dfxMemSetup.DfxNfitDebugLogsEnable) _DEBUG(Expr); }

/**
  @brief Return text name of NFIT stucture type.

  @param[in] NfitType - ACPI NFIT structure type

  @return String describing NFIT type is returned.
**/
CONST CHAR8*
NfitDebugStrcType2Name (
  IN UINT16 Type
  );

/**
  @brief Print proximity domains table.

  @param *MemAffDataPtr     - ACPI SRAT Memory Affinity structures
  @param MemAffDataLength   - ACPI SRAT Memory Affinity structures length

  @return Void.
**/
VOID
NfitDumpMemoryAffinityData (
  IN ACPI_MEMORY_AFFINITY_DATA *MemAffDataPtr,
  IN UINTN                      MemAffDataLength
  );

/**
  @brief Print content of System Physical Address Range Structure.

  @param[in] SpaRangeStrcPtr - Pointer to the structure to dump.

  @return Void.
 **/
VOID
NfitDumpSpaRangeStruct (
  IN NFIT_SPA_RANGE_STRUCTURE *SpaRangeStrcPtr
  );

/**
  @brief Print content of NVDIMM Region Mapping Structure.

  @param[in] NvdimmRgnMapPtr - Pointer to the structure to dump.

  @return Void.
**/
VOID
NfitDumpNvdimmRgnMappingStruct (
  IN NFIT_NVDIMM_REGION_MAPPING_STRUCTURE *NvdimmRgnMapPtr
  );

/**
  @brief This function prints Interleave Structure

  @param[in] IntStrcPtr - Pointer to the structure to dump

  @return Void.
**/
VOID
NfitDumpInterleaveStruct (
  IN NFIT_INTERLEAVE_STRUCTURE *IntStrcPtr
  );

/**
  @brief Print content of SMBIOS Management Information Structure.

  @param[in] SmbiosInfoPtr - Pointer to the structure to dump.

  @return Void.
**/
VOID
NfitDumpSmbiosInfoStruct (
  IN NFIT_SMBIOS_INFORMATION_STRUCTURE *SmbiosInfoPtr
  );

/**
  @brief Print content of NVDIMM Control Region Structure.

  @param CtrlRgnStrcPtr - Pointer to the structure to dump.

  @return Void.
**/
VOID
NfitDumpCtrlRgnStruct (
  IN NFIT_NVDIMM_CONTROL_REGION_STRUCTURE *CtrlRgnStrcPtr
  );

/**
  @brief Print content of Flush Hint Address Structure.

  @param FlushHintStrcPtr - Pointer to the structure to dump.

  @return Void.
**/
VOID
NfitDumpFlushHintAddrStruct (
  IN NFIT_FLUSH_HINT_ADDRESS_STRUCTURE *FlushHintStrcPtr
  );

/**
  @brief Print content of Platform Capabilities Structure.

  @param[in] PlatCapPtr - Pointer to the structure to dump.

  @return Void.
**/
VOID
NfitDumpPlatCapStruct (
  IN NFIT_PLATFORM_CAPABILITIES_STRUCTURE *PlatCapPtr
  );

#endif  // _NFIT_DEBUG_H_

