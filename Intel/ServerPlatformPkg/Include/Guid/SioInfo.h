/** @file
GUID for the SIO chip info.

@copyright
INTEL CONFIDENTIAL
Copyright 2018 Intel Corporation. <BR>

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

#ifndef _SIO_INFO_H_
#define _SIO_INFO_H_

//
// This GUID must match gSioAst2500Guid in the DEC file.  This is due to language limitation, it must
// be #defined for use in initializing global variables.  Make sure this is in sync with DEC file.
//
#define SIO_AST2500_GUID \
  { \
    0x6234A07C, 0x4E03, 0x50C1, { 0xF7, 0xB2, 0x63, 0xB9, 0x9D, 0x20, 0xCB, 0x67 } \
  }

//
// This GUID must match gSioPilotIIIGuid in the DEC file.  This is due to language limitation, it must
// be #defined for use in initializing global variables.  Make sure this is in sync with DEC file.
//
#define SIO_PILOTIII_GUID \
  { \
    0x6064589b, 0x653f, 0x43a0, { 0xa3, 0x0c, 0x59, 0x42, 0x9d, 0x84, 0x1b, 0xc9 } \
  }

#pragma pack (1)

//
// Structure for the SIO Config/Index/Data port information.
//
typedef struct {
  UINT16                   ConfigPort;
  UINT16                   IndexPort;
  UINT16                   DataPort;
} SIO_PORT;

typedef struct {
  VENDOR_DEVICE_PATH       Header;
  SIO_PORT                 Port;
} SIO_DEVICE_PATH;

#pragma pack ()

#endif //_SIO_INFO_H_
