/** @file
  Intel CPU PPM policy PPI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#ifndef __PPM_POLICY_PPI_H__
#define __PPM_POLICY_PPI_H__


#include <PpmPolicyPeiDxeCommon.h>


///
/// PPI revision information
/// This PPI will be extended in a backwards compatible manner over time
/// Added interfaces should be documented here with the revisions added
/// Revision 1:  Initial revision
#define PPM_POLICY_PPI_REVISION  0x1


typedef struct {
  /**
    This member specifies the revision of the PPM_POLICY_PPI. This field is used to
    indicate backwards compatible changes to the INTERFACE. Platform code that produces
    this INTERFACE must fill with the correct revision value for PPM code
    to correctly interpret the content of the INTERFACE fields.
  **/
  UINT32                    Revision;
  PPM_POLICY_CONFIGURATION  Policy;
} PPM_POLICY_PPI;


#endif
