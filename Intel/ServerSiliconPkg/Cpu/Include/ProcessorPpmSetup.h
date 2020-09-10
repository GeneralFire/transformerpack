/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _PROCESSOR_PPM_SETUP_H
#define _PROCESSOR_PPM_SETUP_H

//PKGC_SA_PS_CRITERIA
#define IP_TYPE_KTI                      0
#define IP_TYPE_RLINK                    1
#define IP_TYPE_FXR                      2
#define IP_TYPE_MCDDR                    0x10
#define IP_TYPE_MCDDR_MAX                0x17
#define IP_TYPE_HBM                      0x18
#define IP_TYPE_HBM_MAX                  0x1F
#define IP_TYPE_IIO                      0x20
#define IP_TYPE_IIO_MAX                  0x27
#define IP_TYPE_HQM                      0x30
#define IP_TYPE_NAC                      0x31
#define IP_TYPE_TIP                      0x32
#define IP_TYPE_MDFS                     0x33
#define IP_TYPE_HCX                      0x34
#define IP_TYPE_DINO                     0x35

#define IP_ENABLE                        1
#define IP_DISABLE                       0

#define OPTION_1                         1
#define OPTION_2                         2
#define OPTION_3                         3

#define PPM_AUTO                         0xFF

#endif   // _PROCESSOR_PPM_SETUP_H
