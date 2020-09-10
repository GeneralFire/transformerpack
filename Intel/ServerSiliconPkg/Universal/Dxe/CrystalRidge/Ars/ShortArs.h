/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

/**
  Adds specified SPA to short ARS scrub list.

  Spa is aligned to ARS block size before addition.

  @param[in]  Spa                   System address.

  @retval     EFI_SUCCESS           Address added OK or already exists.
  @retval     EFI_BUFFER_TOO_SMALL  Short ARS scrub list full. Not added.

**/
EFI_STATUS
ShortArsAddError (
  IN  UINT64  Spa
  );

/**
  Retrieves media log errors from all DIMMs, and adds all error locations
  that are currently mapped to PMEM to short ARS scrub list.
**/
VOID
ShortArsGatherMediaLogErrors (
  VOID
  );

/**
  Runs ARS on short ARS scrub list.
  Confirmed errors are added to Patrol Scrubber list.
**/
VOID
RunArsOnShortArsScrubList (
  VOID
  );
