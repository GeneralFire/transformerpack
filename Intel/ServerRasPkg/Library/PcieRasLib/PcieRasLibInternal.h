/** @file
  Internal header file for IIO RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2018 Intel Corporation. <BR>

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

#ifndef _PCIE_HELP_LIB_INTERNAL_H_
#define _PCIE_HELP_LIB_INTERNAL_H_

/**
Define the stucture for PCIE Correctable Error Counter
--*/
#define PCIE_CE_COUNTER_SIGNATURE  SIGNATURE_32 ('C', 'E', 'C', 'T')

typedef struct _PCIE_CE_COUNTER_LIST {
  UINT32      Signature;
  LIST_ENTRY  Link;
  UINT8       Socket;
  UINT8       Bus;
  UINT8       Device;
  UINT8       Function;
  UINT32      CeCount;
} PCIE_CE_COUNTER_LIST;

#define PCIE_CE_COUNTER_FROM_LINK(a)  CR (a, PCIE_CE_COUNTER_LIST, Link, PCIE_CE_COUNTER_SIGNATURE)

#endif
