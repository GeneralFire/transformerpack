/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2017 Intel Corporation. <BR>

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

#include <Base.h>
#include <Uefi.h>
#include <Library/IoLib.h>




/*++
Description:

  This function is a hook for PlatformVariableInitPeiEntry

--*/
VOID PlatformVariableHookForEntry(
   VOID
);


/*++
Description:

  This function allow platform to generate variable hob base on different event.

Arguments:
  IN VOID  *Interface                  -point to EFI_PEI_READ_ONLY_VARIABLE2_PPI
   IN OUT   UINT8 *phobdata,        -pont to hob data
   IN OUT   UINT16 *pDefaultId      -pointer to defautlID

Returns:
  TRUE:platform have its own variable hob that need be createn
  FALSE:platform don;t need to create variable hob in this case


--*/
BOOLEAN PlatformVariableHookForHobGeneration(
   IN VOID  *Interface,
   IN OUT   UINT8 *phobdata,
   IN OUT   UINT16 *pDefaultId
);
