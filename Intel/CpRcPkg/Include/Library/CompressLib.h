/** @file
  Header file for compression routine.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2017 Intel Corporation. <BR>

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

#ifndef _EFI_COMPRESS_DXE_LIB_H_
#define _EFI_COMPRESS_DXE_LIB_H_

/**
  The compression routine.

  @param[in]       SrcBuffer     The buffer containing the source data.
  @param[in]       SrcSize       Number of bytes in SrcBuffer.
  @param[in]       DstBuffer     The buffer to put the compressed image in.
  @param[in, out]  DstSize       On input the size (in bytes) of DstBuffer, on
                                 return the number of bytes placed in DstBuffer.

  @retval EFI_SUCCESS           The compression was sucessful.
  @retval EFI_BUFFER_TOO_SMALL  The buffer was too small.  DstSize is required.
**/
EFI_STATUS
EFIAPI
Compress (
  IN      VOID    *SrcBuffer,
  IN      UINT64  SrcSize,
  IN      VOID    *DstBuffer,
  IN OUT  UINT64  *DstSize
  );

#endif  // _EFI_COMPRESS_DXE_LIB_H_

