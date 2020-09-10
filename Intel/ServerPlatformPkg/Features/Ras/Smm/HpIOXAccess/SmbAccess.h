/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2012 Intel Corporation. <BR>

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

#ifndef _SMBACCESS_H_
#define _SMBACCESS_H_

#define SMB_READ        BIT0
#define SMB_WRITE       BIT1
#define MAX_RETRY       0x10
#define STALL_TIME      1000000 // 1,000,000 microseconds = 1 second
#define BUS_TRIES       3       // How many times to retry on Bus Errors

EFI_STATUS
AcquireSmBus (
  VOID
  );

EFI_STATUS
SmbusReadWrite (
  IN      UINT8     SlaveAddress,
  IN      UINT8     Operation,
  IN      UINT8     Command,
  IN OUT  VOID      *Buffer
  );

EFI_STATUS
CheckForAccessComplete (
  VOID
  );

EFI_STATUS
ReadWriteSmbus (
  VOID
  );


#endif
