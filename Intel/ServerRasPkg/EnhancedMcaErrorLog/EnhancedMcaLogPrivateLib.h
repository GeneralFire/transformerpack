/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016-2019 Intel Corporation. <BR>

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

#ifndef _ENHANCED_MCA_LOG_PRIVATE_LIB_H_
#define _ENHANCED_MCA_LOG_PRIVATE_LIB_H_

//
// Includes
//

#include <Emca.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <PiDxe.h>
#include <Guid/Cper.h>
#include <IndustryStandard/Acpi62.h>
#include <MaxSocket.h>
//
// Consumed protocols
//


//Get L1 Directory pointer and skip the header to get the BASE to get Elog directory entry pointers base.
#define EMCA_GET_L1_BASE(A) \
    ((UINT8 *)(VOID *)(UINTN)((UINT64)(A) + (UINT64)((EMCA_L1_HEADER *)(A))->HdrLen))

//A - L1 directory pointer
//B - ApicId
//C - McBank Number
// OFFSET byte base = apicid * (Number of L1 Entries per logical processor * 8) + McBankNum
#define EMCA_GET_L1_OFFSET(A,B,C) \
    ( (UINT64)(B) * ( (((EMCA_L1_HEADER *)(A))->NumL1EntryLp) * sizeof(UINT64) ) + ((C) * sizeof(UINT64)) )

//A - Elog Directory Entry Pointers Base
//B - Offset for elog entry
#define EMCA_GET_ELOG_ENTRY(A,B) \
    (EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE *)(VOID *)(UINTN)(*((UINT64 *)(VOID *)(UINTN)((UINT64)(A) + (UINT64)(B))))


/**
emca
    Return True if OS has set the opt in flag in the L1 Directory header or False if it is not set

    @param None

    @retval optin - TRUE if optin flag is set, False if cleared

**/

BOOLEAN
IsEmcaOptInFlagSet (
  VOID
  );

/**

    Perform eMCA L1 directory boot time initialization

    @param L1Dir   - A pointer to a pointer to the allocated L1 directory buffer if the call succeeds; undefined otherwise.
    @param ElogDir - A pointer to a pointer to the allocated Elog directory buffer if the call succeeds; undefined otherwise.

    @retval EFI_SUCCESS - The Allocation and Initialization was successful
    @retval EFI_UNSUPPORTED - EMCA Logging is not supported
    @retval EFI_OUT_OF_RESOURCES - The directories could not be allocated.
    @retval EFI_INVALID_PARAMETER - At least one of the pointers is NULL or invalid

**/

EFI_STATUS
InitEMCA( 
  IN OUT VOID                         **L1Dir,
  IN OUT VOID                         **ElogDir
  );

/**

    It receives the generic error status record and stores it in the appropriate elog entry.

    @param McBankNum - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.
    @param McSig     - A pointer to Machine Check Signature Error Record. See Related definitions and EMCA SAS.
    @param Log       - A pointer to an ACPI Generic Error Status Block completely filled along with its UEFI error section. This function just stores the log. See ACPI Specification for details of the record format.

    @retval EFI_INVALID_PARAMETER - At least one of the pointers is invalid or McBank number points to an unavailable bank.
    @retval EFI_OUT_OF_RESOURCES  - The corresponding Elog Entry pointer for the bank and apicid is invalid. There is no memory to store the log.
    @retval Status                - The log was successfully stored in the elog directory.

**/

EFI_STATUS
LogEmcaRecord (
  IN UINT32                                 McBankNum,
  IN CONST EMCA_MC_SIGNATURE_ERR_RECORD     *McSig,
  IN CONST EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE             *Log
  );

#endif  //_EMCA_LIB_H_

