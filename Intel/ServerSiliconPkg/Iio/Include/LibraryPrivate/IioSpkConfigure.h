/** @file

@copyright
INTEL CONFIDENTIAL
Copyright 2018 - 2019 Intel Corporation. <BR>

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

#ifndef _IIO_SPK_CONF_H_
#define _IIO_SPK_CONF_H_

#include <IioPlatformData.h>

/**
  This routine detects active SPK instances in ALD subsystems and count offsets to the SPK regs.

  @param[in]       IioIndex           - Index with the changed IIO (Socket)
  @param[in, out] *SpkInstancesCount  - On entry SpkOffsets[] table length,
                                        on exit the number of elements provided in SpkOffsets[]
  @param[in, out]  SpkOffsets[]       - Array of SPK base registers offsets

  @retval EFI_SUCCESS                 - SPK instancecs detected successfully.
  @retval EFI_BUFFER_TOO_SMALL        - More SPK instances is discovered than expected.
  @retval EFI_UNSUPPORTED             - If SPK not supported.
**/
EFI_STATUS
IioSpkDetectInstances (
  IN UINT8                         IioIndex,
  IN OUT UINT8                    *SpkInstancesCount,
  IN OUT UINT32                    SpkOffsets[]
  );

/**
  This routine writes values of SPK memory base and size registers.

  @param[in] *IioGlobalData      - Pointer to Global Data Structure.
  @param[in]  IioIndex           - Index with the changed IIO (Socket)
  @param[in] *BufferBase         - Array of base addresses of buffers
  @param[in]  BufferSize         - Size of each allocated buffer in bytes
  @param[in] *SpkInstancesCount  - Number of detected SPK instances
  @param[in]  SpkOffsets[]       - Array of SPK base registers offsets

  @retval EFI_SUCCESS       - SPK Trace Buffer configured successfully.
  @retval EFI_UNSUPPORTED   - If SPK not supported.
**/
EFI_STATUS
IioSpkConfigureTraceBuffers (
  IN IIO_GLOBALS                 *IioGlobalData,
  IN UINT8                        IioIndex,
  IN EFI_PHYSICAL_ADDRESS        *BufferBase,
  IN UINT32                       BufferSize,
  IN UINT8                       *SpkInstancesCount,
  IN UINT32                       SpkOffsets[]
  );

#endif // _IIO_SPK_CONF_H_
