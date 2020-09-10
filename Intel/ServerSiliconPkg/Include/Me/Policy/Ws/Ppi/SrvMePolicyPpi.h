/** @file
  ME Policy for ME PEIMs

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2010 - 2020 Intel Corporation. <BR>

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

#ifndef _SRV_ME_POLICY_PPI_H_
#define _SRV_ME_POLICY_PPI_H_

#include <Config/MeConfig.h>

/**
  ME Policy Protocol \n
  All ME Policy PPI change history listed here \n

  -<b> Revision 1</b>:
   - Initial version Server side \n
  -<b> Revision 2</b>:
   - Remove AMT \n
**/
#define ME_POLICY_PPI_REVISION    2


//
// Extern the GUID for PPI users.
//
extern EFI_GUID gMePolicyPpiGuid;


/**
  ME policy provided by platform for PEI phase
  The Policy PPI returns the Intel ME feature set in PEI phase
**/
typedef struct
{
  UINT32     Revision;                      ///< Revision for the PPI structure

  ME_CONFIG  MeConfig;

} ME_POLICY_PPI;

#endif // _SRV_ME_POLICY_PPI_H_

