/** @file
  Declarations for S3M IP public APIs.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#ifndef _S3M_IP_LIB_H_
#define _S3M_IP_LIB_H_

#include <Uefi.h>

/**
  Read CPU strap from S3M.

  @param[in]      SocketId          The ID of the Socket from which to read CPU Strap Content.
  @param[in out]  Buffer            Pointer of the buffer. If return EFI_SUCCESS, it is the CPU Strap Content.
  @param[in out]  BufSize           Pointer of the buffer size(Unit: Number of DWs). In return, it is the real number of DWs returned.

  @retval EFI_SUCCESS               Read S3M CPU Strap content successfully
  @retval !EFI_SUCCESS              Failed to read S3M CPU Strap content
**/
EFI_STATUS
EFIAPI
ReadS3mCpuStrap (
  IN        UINT8    SocketId,
  IN OUT    UINT32   *Buffer,
  IN OUT    UINT32   *BufSize
  );

/**
  Override CPU Strap into S3M.

  @param[in]  SocketId          The ID of the Socket from which to read CPU Strap Content.
  @param[in]  Buffer            Pointer of the buffer with the CPU strap provisioning data.
  @param[in]  BufSize           Buffer size(Unit: Number of DWs).

  @retval EFI_SUCCESS           Override S3M CPU Strap content successfully
  @retval !EFI_SUCCESS          Failed to override S3M CPU Strap content
**/
EFI_STATUS
EFIAPI
OverrideS3mCpuStrap (
  IN    UINT8    SocketId,
  IN    UINT32   *Buffer,
  IN    UINT32   BufSize
  );

/**
  Provision S3M CFR.

  @param[in]  SocketId          The ID of the Socket.
  @param[in]  Buffer            Pointer of the buffer with the CFR provisioning data.
  @param[in]  BufSize           Buffer size(Unit: Number of DWs).
  @param[in]  ProvisionCmd      Command for the Provision operation.

  @retval EFI_SUCCESS           Provision S3M CFR content successfully
  @retval !EFI_SUCCESS          Failed to Provision S3M CFR content content
**/
EFI_STATUS
EFIAPI
S3mProvision (
  IN    UINT8    SocketId,
  IN    UINT32  *Buffer,
  IN    UINT32   BufSize,
  IN    UINT32   ProvisionCmd
);

/**
  Get S3m Fw Cfr Valid Version Info.

  @param[in]         SocketId  - The SocketId need to read
  @param[out]        SVN    - A pointer to the SVN
  @param[out]        RevID  - A pointer to the Valid
  @param[out]        IsCommitted  - A pointer to the IsCommitted

  @retval           EFI_SUCCESS
  @retval           EFI_DEVICE_ERROR   S3M device error.
**/
EFI_STATUS
EFIAPI
GetS3mCfrValidVersionInfo(
  IN  UINT8         SocketId,
  OUT UINT32        *SVN,
  OUT UINT32        *RevID,
  OUT BOOLEAN       *IsCommitted
);

/**
  Get Pcode/Ucode Fw Cfr Valid Version Info.

  @param[in]         SocketId  - The SocketId need to read
  @param[out]        SVN    - A pointer to the SVN
  @param[out]        RevID  - A pointer to the Valid
  @param[out]        IsCommitted  - A pointer to the IsCommitted

  @retval           EFI_SUCCESS
  @retval           EFI_DEVICE_ERROR   S3M device error.
**/
EFI_STATUS
EFIAPI
GetPucodeCfrValidVersionInfo(
  IN  UINT8         SocketId,
  OUT UINT32        *SVN,
  OUT UINT32        *RevID,
  OUT BOOLEAN       *IsCommitted
);
#endif // _S3M_IP_LIB_H_
