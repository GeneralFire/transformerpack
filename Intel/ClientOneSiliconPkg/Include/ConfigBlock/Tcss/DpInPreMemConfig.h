/** @file
  DPIN PreMem policy

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/
#ifndef _DPIN_PREMEM_CONFIG_H_
#define _DPIN_PREMEM_CONFIG_H_

#include <ConfigBlock.h>

#define DPIN_PREMEM_CONFIG_REVISION 1
extern EFI_GUID gDpInPreMemConfigGuid;

#pragma pack (push,1)

/**
  This configuration block describes DPIN settings.\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  ///< Header: Struct Header Offset 0-27 Config Block Header
  CONFIG_BLOCK_HEADER       Header;
  ///< DpInExternalEn: 00 = <b>Dp-In Port is Internal Only</b>, 01 = Dp-In Port is External First.
  UINT8                     DpInExternalEn;
  ///< NumberOfDpInPort: Total number of DpIn Port present on Board.
  UINT8                     NumberOfDpInPort;
  ///< DpInPortConnectMap: Indicate which Dp-In port connection detected. Each BIT stand for one Dp-In port.
  UINT8                     DpInPortConnectMap;
  ///< Reserved bytes for future use, align to DWORD
  UINT8                     Rsvd[1];
} DPIN_PREMEM_CONFIG;

#pragma pack (pop)

#endif /* _DPIN_PREMEM_CONFIG_H_ */
