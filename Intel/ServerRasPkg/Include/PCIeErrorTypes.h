/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#ifndef _PCIE_ERROR_TYPES_H_
#define _PCIE_ERROR_TYPES_H_


//
// Definition
//
//
// Categorized error types for logging and viewing
// BIT0~2 force to the same definition of Global IEH for convenience.
//
#define COR_ERROR_TYPE                                            BIT0
#define NONFATAL_ERROR_TYPE                                       BIT1
#define FATAL_ERROR_TYPE                                          BIT2

#define INVALID_ERROR_TYPE                                        0x00
#define SERROR_TYPE                                               BIT4
#define PERROR_TYPE                                               BIT5
#define DPERROR_TYPE                                              BIT6
#define REC_MASTER_ABORT_TYPE                                     BIT7
#define REC_TARGET_ABORT_TYPE                                     BIT8
#define REC_SERROR_TYPE                                           BIT9
#define REDUNDANCY_ERROR_TYPE                                     BIT10
#define LEGACY_ERROR_TYPE                                         BIT11
#define EXTENDED_ERROR_TYPE                                       BIT12
#define RECOVERABLE_ERROR_TYPE                                    BIT13
#define MISCERROR_TYPE                                            0xee

#endif  //_PCIE_ERROR_TYPES_H_

