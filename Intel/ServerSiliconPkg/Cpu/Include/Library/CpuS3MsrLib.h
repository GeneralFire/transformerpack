/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2017 Intel Corporation. <BR>

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

#ifndef __CPU_S3_MSR_LIB__
#define __CPU_S3_MSR_LIB__


/**

  Restores required MSR settings on S3 Resume.  This is used for MSRs
  that must be set late in S3 resume such as lock bits or MSRs that cannot
  be handled by WriteRegisterTable restore method.

  @param PeiServices   - Pointer to PEI Services Table.

  @retval EFI_SUCCESS in all cases.

**/
EFI_STATUS
EFIAPI
S3RestoreCpuMsrs (
  IN      EFI_PEI_SERVICES          **PeiServices
  );

/**

  Performs any PEI CPU Initialization needed in PEI phase that
  requires multi-threaded execution. This is executed on normal
  boots and S3 resume.

  @param PeiServices   - Pointer to PEI Services Table.

  @retval EFI_SUCCESS in all cases.

**/
EFI_STATUS
EFIAPI
LatePeiCpuInit (
  IN      EFI_PEI_SERVICES          **PeiServices
  );

#endif
