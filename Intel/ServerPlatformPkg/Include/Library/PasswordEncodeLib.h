/** @file
  Password Encode Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2017 Intel Corporation. <BR>

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

#ifndef __PASSWORD_ENCODE_LIB_H__
#define __PASSWORD_ENCODE_LIB_H__


/**
  Encodes a NULL-terminated unicode password string.

  This function encodes the input unicode password string to the encoding buffer.
  The input and output buffer size for the encoded password are also specified.

  If Password is NULL, then ASSERT()
  If EncodePassword is NULL, then ASSERT()
  If EncodeBufferSize is NULL, then ASSERT()

  @param[in]   Password             A NULL-terminated unicode password string.
  @param[out]  EncodePassword       The output buffer to save the encoded password.
  @param[in,out] EncodeBufferSize   Points to the encode buffer size for the encoded password.
                                    On input, it contains the buffer size to save the encode password.
                                    On output, it contains the encoded password real size.

  @retval RETURN_BUFFER_TOO_SMALL   The input buffer size for the encoded password is too small.
  @retval RETURN_OUT_OF_RESOURCES   Fails to allocate memory for the password encoding operation.
  @retval RETURN_NOT_READY          The encoding algorithm cannot be initialized.
  @retval RETURN_DEVICE_ERROR       Failure to encode the input password with the algorithm.
  @retval RETURN_SUCCESS            Password was successfully encoded and saved to the buffer

**/
RETURN_STATUS
EFIAPI
EncodePassword (
  IN CONST  CHAR16                *Password,
  OUT       UINT8                 *EncodedPassword,
  IN OUT    UINTN                 *EncodeBufferSize
  );

#endif

