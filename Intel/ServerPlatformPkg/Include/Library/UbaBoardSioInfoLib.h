/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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

  @par Revision Reference
  This PPI is defined in UEFI Platform Initialization Specification 1.2 Volume 1:
  Pre-EFI Initalization Core Interface.
**/

#ifndef _UBA_BOARD_SIO_INFO_H_
#define _UBA_BOARD_SIO_INFO_H_

#include <SioRegs.h>

#define BOARD_SIO_INFO_DATA_SIGNATURE  SIGNATURE_32 ('P', 'S', 'I', 'O')
#define BOARD_SIO_INFO_DATA_VERSION    01

// {32C1F731-C2CD-4325-888B-60A0C3DEBB30}
#define   PLATFORM_BOARD_SIO_INFO_DATA_GUID \
{ 0x32c1f731, 0xc2cd, 0x4325, { 0x88, 0x8b, 0x60, 0xa0, 0xc3, 0xde, 0xbb, 0x30 } }

//
// board sio information table
//
typedef struct _PEI_BOARD_SIO_INFO{
  //
  // Header information
  //
  INT32                    Signature;
  INT32                    Version;
  //
  // SIO initialization table
  //
  UINT8                   SioIndexPort;               // SIO Index Port value
  UINT8                   SioDataPort;                // SIO Data Port value
  SIO_INDEX_DATA          *mSioInitTable;             // SIO init table
  UINT8                   NumSioItems;                // Number of items in the SIO init table.
} PEI_BOARD_SIO_INFO;

EFI_STATUS
PlatformGetBoardSioInfo (
  OUT   PEI_BOARD_SIO_INFO   *BoardSioInfoData
);

STATIC  EFI_GUID gPlatformBoardSioInfoDataGuid = PLATFORM_BOARD_SIO_INFO_DATA_GUID;

#endif

