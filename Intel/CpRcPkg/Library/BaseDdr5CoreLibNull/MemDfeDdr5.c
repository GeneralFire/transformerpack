/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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

#include "Ddr5Core.h"

/*++

  DDR5 Rx DFE

  @param[in] Host    - Pointer to sysHost

  @retval SUCCESS

--*/
UINT32
EFIAPI
RxDfeDdr5 (
  IN PSYSHOST  Host
  )
{
  return SUCCESS;
} // RxDfeDdr5

/*++

  DDR5 Tx DFE

  @param[in] Host    - Pointer to sysHost

  @retval SUCCESS

--*/
UINT32
EFIAPI
TxDfeDdr5 (
  IN PSYSHOST  Host
  )
{
  return SUCCESS;
} // TxDfeDdr5

/*++

  DDR5 Write DB DFE

  @param[in] Host    - Pointer to sysHost

  @retval SUCCESS

--*/
UINT32
EFIAPI
TxDbDfeDdr5 (
  IN PSYSHOST  Host
  )
{
  return SUCCESS;
} // TxDbDfeDdr5