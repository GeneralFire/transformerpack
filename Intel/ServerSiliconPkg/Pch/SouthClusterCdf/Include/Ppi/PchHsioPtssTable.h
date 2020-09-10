/** @file
  PCH policy PPI produced by a platform driver specifying HSIO PTSS Table.
  This PPI is consumed by the PCH PEI modules. This is deprecated.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2018 Intel Corporation. <BR>

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

#ifndef _PCH_HSIO_PTSS_TABLE_H_
#define _PCH_HSIO_PTSS_TABLE_H_

///
/// HSIO PTSS table PPI GUID.
///
extern EFI_GUID  gPchHsioPtssTablePpiGuid;

///
/// The PCH_SBI_PTSS_HSIO_TABLE block describes HSIO PTSS settings for PCH.
///
typedef struct {
  UINT8       LaneNum;
  UINT8       PhyMode;
  UINT16      Offset;
  UINT32      Value;
  UINT32      BitMask;
} PCH_SBI_PTSS_HSIO_TABLE;

#endif // _PCH_HSIO_PTSS_TABLE_H_
