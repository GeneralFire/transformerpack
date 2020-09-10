/** @file
  Interface of WheaSciLib Interface.

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

#ifndef __WHEA_SUPPORT_LIB_H__
#define __WHEA_SUPPORT_LIB_H__

#include <Guid/Cper.h>
#include <IndustryStandard/Acpi.h>

//
//memomy map:
//8 byte(pointer) + Error Status Block + Raw data
//
//
#define  ERROR_LOG_BUFFER_SIZE        0x4000
#define  MAX_RAW_DATA_LENGTH          (((ERROR_LOG_BUFFER_SIZE )>>1) - 8)
#define  ERROR_STATUS_BLOCK_LENGTH    ((ERROR_LOG_BUFFER_SIZE )>>1)

#define  NUM_RECORD_PRE_ALLOCATED     1
#define  MAX_SECTION_PER_RECORD       1

/**
  This routine is to add error source in HEST table. And return reference buffer address.

  @param  [in]  ErrorSource -- point to error source
  @param  [in]  InstallACPIFlag -- TRUE: install HEST ACPI table; FALSE: don't install.
  @param  [out] BufferAddress  -- buffer address in reserved area for the specific error source.

  @return none.
**/
VOID
EFIAPI
AddHESTErrorSource (
  IN  VOID                                                 *ErrorSource,
  IN  UINT32                                                Size,
  IN  BOOLEAN                                               InstallACPIFlag,
  OUT UINT64                                               *BufferAddress
  );



/**
  This routine returns BERT error address. This address is at reserved attribution address.

  @param [out] ErrorAddress    - error address of specific error type.

  @retval  VOID
**/
VOID
EFIAPI
GetBERTErrorAddress (
  OUT  UINT64                           *ErrorAddress
  );


/**
  This routine add EINJ error injection instruction entry. And it can install EINJ table.
  EINJ table only can install once.

  @param [in]  InjectionEntry    - pointer to EINJ error injection entry.
  @param [in]  InstallTableFlag  - TRUE --> Install EINJ table; FALSE --> don't install.

  @retval  VOID
**/
VOID
EFIAPI
AddEinjActionItem (
  IN   EFI_ACPI_6_2_EINJ_INJECTION_INSTRUCTION_ENTRY       *InjectionEntry,
  IN   BOOLEAN                                             InstallTableFlag
  );

/**
  This routine is to install ERST table.


  @param [in]  ERSTInstruction    - pointer to ERST instruction list.
  @param [in]  Size               - size of whole ERST instruction.

  @retval  VOID
**/
VOID
EFIAPI
AddERSTInstruction (
  IN  EFI_ACPI_6_2_ERST_SERIALIZATION_INSTRUCTION_ENTRY  *ERSTInstruction,
  IN  UINT32                                              Size
  );

/**
  This routine is to log error to BERT table.


  @param [in]  CperHeader                    - pointer to a CPER header, the CPER wraps the boot error details.
  @param [in]  BootErrorRegionAddress        - address of the Boot Error Region memory block.

  @retval  EFI_STATUS
**/
EFI_STATUS
EFIAPI
LogErrorToBert (
  IN  EFI_COMMON_ERROR_RECORD_HEADER                     *CperHeader,
  IN  UINT64                                              BootErrorRegionAddress
  );


/**
  This routine is to compare to severity values and determine if higher severity value is encountered.
  Higher severity value indicates more severe error.

  @param [in]  CurrentSeverity               Current severity recorded in the status block.
  @param [in]  NewSeverity                   Severity of the new error incoming error.

  @retval  BOOLEAN
**/
BOOLEAN
EFIAPI
IsHigherSeverity (
  IN  UINT32                     CurrentSeverity,
  IN  UINT32                     NewSeverity
  );
#endif
