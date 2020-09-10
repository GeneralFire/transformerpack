/** @file ArsErrorInject.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2018 Intel Corporation. <BR>

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

#ifndef _ARSERRORINJECT_H_
#define _ARSERRORINJECT_H_

/**
  Initializes ARS Error Inject code.

  Currently it only allocates Error Records array.

  @param[in]  Smst                Pointer to EFI_SMM_SYSTEM_TABLE2.

  @retval     EFI_SUCCESS         Initialization OK.
  @retval     <EFI error status>  An error occurred. Couldn't allocate memory.

**/
EFI_STATUS
InitArsErrorInject (
  IN EFI_SMM_SYSTEM_TABLE2 *Smst
  );

/**
  Injects errors of specified transaction type into specified SPA range.

  This function injects errors on all-or-nothing basis. If injection of complete range isn't possible,
  any partial injection is cleared and error status is returned.

  @param[in]  StartAddress          Range start.
  @param[in]  NumBlocks             Range length.
  @param[in]  MemType               Memory Transaction Type, see POISON_MEMORY_TYPE_* and FIS for details

  @retval     EFI_SUCCESS           Injection succeeded.
  @retval     EFI_INVALID_PARAMETER Range overlaps already injected error(s).
  @retval     EFI_BUFFER_TOO_SMALL  Reached maximal number of injected ranges. No errors injected.
  @retval     <EFI error status>    Failure. No errors injected.

**/
EFI_STATUS
ArsErrorInject (
  IN UINT64 StartAddress,
  IN UINT64 NumBlocks,
  IN UINT8  MemType
  );

/**
  Clears errors from specified SPA range.

  @param[in]  StartAddress          Range start.
  @param[in]  NumBlocks             Range length.
  @param[in]  DataOnly              TRUE:  operate on data structures only;
                                           do not clear error
                                    FALSE: normal operation

  @retval     EFI_BUFFER_TOO_SMALL  Internal data structure too small to accommodate
                                    new error list. No errors cleared.

**/
EFI_STATUS
ArsErrorInjectClear (
  IN  UINT64   StartAddress,
  IN  UINT64   NumBlocks,
  IN  BOOLEAN  DataOnly
  );

/**
  Stores injected error records into specified array.

  Returns number of stored entries.

  @param[out]  OutputArray      Output array.
  @param[in]   OutputArraySize  Number of available elements in output array.

**/
UINT32
ArsErrorInjectStatusQuery (
  ARS_ERROR_INJECT_STATUS_QUERY_ERROR_RECORD *OutputArray,
  UINT32                                     OutputArraySize
  );

#endif // _ARSERRORINJECT_H_
