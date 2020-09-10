/** @file
  The definition for VTD PMR Regions Information HOB.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 - 2020, Intel Corporation. All rights reserved.<BR>

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


#ifndef _VTD_PMR_INFO_HOB_H_
#define _VTD_PMR_INFO_HOB_H_

///
/// This interface is to report the PMR regions information
/// PMR regions means PLMR/PHMR base and limit
/// When gVtdPmrInfoDataHobGuid exists, it means:
///  1. Dma buffer is reserved by memory initialize code
///  2. PeiGetVtdPmrAlignmentLib is used to get alignment
///  3. PMR regions are determined by the system memory map
///  4. PMR regions will be conveyed through VTD_PMR_INFO_HOB
///
typedef struct {
  UINT32             ProtectedLowBase;   //PLMR Base
  UINT32             ProtectedLowLimit;  //PLMR Limit
  UINT64             ProtectedHighBase;  //PHMR Base
  UINT64             ProtectedHighLimit; //PHMR Limit
} VTD_PMR_INFO_HOB;

#endif // _VTD_PMR_INFO_HOB_H_

