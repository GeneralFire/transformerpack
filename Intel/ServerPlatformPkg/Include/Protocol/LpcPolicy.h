/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2017 Intel Corporation. <BR>

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

#ifndef _LPC_POLICY_H_
#define _LPC_POLICY_H_

#define EFI_LPC_POLICY_PROTOCOL_COM_MAX 4

typedef struct {
  BOOLEAN Enable[EFI_LPC_POLICY_PROTOCOL_COM_MAX];
  UINT16  Address[EFI_LPC_POLICY_PROTOCOL_COM_MAX];
  UINT16  Irq[EFI_LPC_POLICY_PROTOCOL_COM_MAX];
} EFI_LPC_POLICY_PROTOCOL_COM;

#define EFI_LPC_POLICY_PROTOCOL_PARALLEL_MAX  4

typedef enum {
  EfiLpcPolicyParallelOutOnly,
  EfiLpcPolicyParallelBiDir,
  EfiLpcPolicyParallelEpp,
  EfiLpcPolicyParallelEcp,
  EfiLpcPolicyParallelLast
} EFI_LPC_POLICY_PROTOCOL_PARALLEL_MODE;

typedef struct {
  BOOLEAN                               Enable[EFI_LPC_POLICY_PROTOCOL_PARALLEL_MAX];
  UINT16                                Address[EFI_LPC_POLICY_PROTOCOL_PARALLEL_MAX];
  UINT16                                Irq[EFI_LPC_POLICY_PROTOCOL_PARALLEL_MAX];
  UINT16                                DmaChannel[EFI_LPC_POLICY_PROTOCOL_PARALLEL_MAX];
  EFI_LPC_POLICY_PROTOCOL_PARALLEL_MODE Mode[EFI_LPC_POLICY_PROTOCOL_PARALLEL_MAX];
} EFI_LPC_POLICY_PROTOCOL_PARALLEL;

typedef struct {
  BOOLEAN Enable;
} EFI_LPC_POLICY_PROTOCOL_KEYBOARD;

typedef struct {
  BOOLEAN Enable;
} EFI_LPC_POLICY_PROTOCOL_MOUSE;

typedef enum {
  EfiLpcPolicyFloppy0M36,
  EfiLpcPolicyFloppy1M20,
  EfiLpcPolicyFloppy0M72,
  EfiLpcPolicyFloppy1M44,
  EfiLpcPolicyFloppy2M88,
  EfiLpcPolicyFloppyLast
} EFI_LPC_POLICY_PROTOCOL_FLOPPY_TYPE;

#define EFI_LPC_POLICY_PROTOCOL_FLOPPY_MAX  4

typedef struct {
  BOOLEAN                             Enable[EFI_LPC_POLICY_PROTOCOL_FLOPPY_MAX];
  EFI_LPC_POLICY_PROTOCOL_FLOPPY_TYPE Type[EFI_LPC_POLICY_PROTOCOL_FLOPPY_MAX];
} EFI_LPC_POLICY_PROTOCOL_FLOPPY;

typedef struct _EFI_LPC_POLICY_PROTOCOL {
  EFI_LPC_POLICY_PROTOCOL_COM       PolicyCom;
  EFI_LPC_POLICY_PROTOCOL_PARALLEL  PolicyParallel;
  EFI_LPC_POLICY_PROTOCOL_FLOPPY    PolicyFloppy;
  EFI_LPC_POLICY_PROTOCOL_KEYBOARD  PolicyKeyboard;
  EFI_LPC_POLICY_PROTOCOL_MOUSE     PolicyMouse;
} EFI_LPC_POLICY_PROTOCOL;

extern EFI_GUID gEfiLpcPolicyProtocolGuid;

#endif
