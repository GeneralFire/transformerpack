/** @file
  This file describes the contents of the ACPI System Locality Information
  Table (SLIT).  Some additional ACPI 3.0 values are defined in Acpi3_0.h.
  All changes to the Slit contents should be done in this file.

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

#ifndef _SLIT_H_
#define _SLIT_H_

#include "MaxSocket.h"

//
// SLIT Definitions, see TBD specification for details.
//

#define EFI_ACPI_OEM_SLIT_REVISION  0x00000001
//
// SLIT Revision (defined in spec)
//
#define EFI_ACPI_SLIT_PMEM_NODES_SOCKET_MAX_CNT 8  // Max number of PMEM nodes per socket
#define EFI_ACPI_SLIT_NODES_SOCKET_MAX_CNT      4  // Max number of SNC nodes
#define EFI_ACPI_SLIT_DOMAINS_NODES_MAX_CNT     2  // Max number of Domins per SNC node (1LM domain and 2LM domain)

#define EFI_ACPI_SLIT_NODES_MAX_CNT \
  (MAX_SOCKET * ((EFI_ACPI_SLIT_NODES_SOCKET_MAX_CNT * EFI_ACPI_SLIT_DOMAINS_NODES_MAX_CNT) \
  + EFI_ACPI_SLIT_PMEM_NODES_SOCKET_MAX_CNT))

#define EFI_ACPI_SYSTEM_LOCALITIES_ENTRY_COUNT \
  (EFI_ACPI_SLIT_NODES_MAX_CNT * EFI_ACPI_SLIT_NODES_MAX_CNT)

#define EFI_ACPI_SLIT_PMEM_INFO_CNT \
  (MAX_SOCKET * EFI_ACPI_SLIT_PMEM_NODES_SOCKET_MAX_CNT)

#define PMEM_INVALID_SOCKET         0xFF

#define PMEM_ZERO_HOP               10
#define PMEM_ONE_ONE                17
#define PMEM_ONE_HOP                28
#define PMEM_TWO_HOP                38

#define ZERO_HOP                    10
#define ZERO_ONE                    11
#define ZERO_TWO                    12
#define ZERO_THREE                  13
#define ONE_HOP                     20
#define ONE_ONE                     21
#define ONE_TWO                     22
#define TWO_HOP                     30
#define THREE_HOP                   40
#define DISTANT_NODE_4S_EP          2
#define DISTANT_NODE_4S_EP_COD      (DISTANT_NODE_4S_EP * 2)

typedef struct {
  UINT8 Socket;
  UINT8 Pmem;
  UINT8 Valid;
} EFI_ACPI_SYSTEM_LOCALITY_INFORMATION_TABL_PMEM_INFO;

typedef struct {
  UINT8   Entry;
} ACPI_SYSTEM_LOCALITIES_STRUCTURE;

typedef struct {
  EFI_ACPI_6_2_SYSTEM_LOCALITY_DISTANCE_INFORMATION_TABLE_HEADER    Header;
  ACPI_SYSTEM_LOCALITIES_STRUCTURE                                  NumSlit[0];

} ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE;


#endif
