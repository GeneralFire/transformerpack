/** @file
  Interface header file for the HBM library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#ifndef _HBM_CORE_DISPLAY_H_
#define _HBM_CORE_DISPLAY_H_


/**

  Function to print out the final training Results for HBM training steps.


  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                TrainType           Training margin Group selector.
  @param[in]                NumRows             Row size for 2-dimensional array PrevData.
  @param[in]                NumCols             Column size for 2-dimensional array PrevData.
  @param[in]                PrevData            2-dimentional array that stores settings before
                                                training begins.
                                                NULL indicates only display current values.
  @param[in]                MaxStrobe           Number of strobes for this signal.
  @param[in]                HbmDisplayType      1 indicates displaying Strobe based Results;
                                                0 indicates channel based Results.

**/
VOID
HbmPrintResults (
  IN        UINT8           Socket,
  IN        UINT16          TrainType,
  IN        UINT8           NumRows,
  IN        UINT8           NumCols,
  IN        INT16           *PrevData,
  IN        UINT8           MaxStrobe,
  IN        UINT8           HbmDisplayType
  );

#endif // _HBM_CORE_DISPLAY_H_
