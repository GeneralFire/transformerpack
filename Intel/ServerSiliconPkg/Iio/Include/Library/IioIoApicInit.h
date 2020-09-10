/** @file
  IioIoApicInit library header file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation. <BR>

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

#ifndef _IIO_IOAPIC_INIT_H_
#define _IIO_IOAPIC_INIT_H_
#include <IioPlatformData.h>

/**
    Null implmentation used for architectures that lack the IoApic in the IIO.
    Post initialization IO APIC. To be called after initialization completes.

    @param IioGlobalData - pointer to IIO_GLOBALS
    @param IioIndex - IIO (Socket) index number.
**/
VOID
IioIoApicInit (
  IN  IIO_GLOBALS                *IioGlobalData,
  IN  UINT8                      IioIndex
  );

/**

    Enable IOxAPIC: If the stack is present enables Memory space and Bus Master, making the
    IOxAPIC PCI area visible.

    @param IioGlobalData - pointer to IIO_GLOBALS
    @param IioIndex - IIO (Socket) index number.
    @param Stack    - Stack number.
**/
VOID
Enable_IOxAPIC (
  IN  IIO_GLOBALS     *IioGlobalData,
  IN  UINT8           IioIndex,
  IN  UINT8           Stack
  );

/**

    Boot event handler. Enable IOxAPIC before passing control to OS.

    @param IioGlobalData - Pointer to IIO_GLOBALS
    @param IioIndex      - Index to the current IIO (Socket)

    @retval None

**/
VOID
IioIoApicInitBootEvent (
  IN  IIO_GLOBALS         *IioGlobalData,
  IN  UINT8               IioIndex
  );

#endif /* _IIO_IOAPIC_INIT_H_*/
