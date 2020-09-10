/** @file
  This code supports a the private implementation
  of the Legacy Region protocol.

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

#ifndef _LEGACY_REGION_H_
#define _LEGACY_REGION_H_

#include <PiDxe.h>
#include <IioUniversalData.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/IioUds.h>
#include <Protocol/LegacyRegion2.h>
#include <Protocol/CpuCsrAccess.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <RcRegs.h>
#include <UncoreCommonIncludes.h>

#define LEGACY_REGION_INSTANCE_SIGNATURE  SIGNATURE_32 ('R', 'E', 'G', 'N')

//
// Compile time calculation of the number of elements in the mPamData[]
//
#define PAM_DATA_NO (sizeof (mPamData) / sizeof (mPamData[0]))

#define LEGACY_REGION_INSTANCE_FROM_THIS(this) \
  CR ( \
  this, \
  LEGACY_REGION_INSTANCE, \
  LegacyRegion, \
  LEGACY_REGION_INSTANCE_SIGNATURE \
  )


//
// Three valued logical type.
// Yes, logical systems based on more than two (Boolean) values exist.
//
typedef enum {
  Bool3False    = FALSE,
  Bool3True     = TRUE,
  Bool3DontKnow = TRUE + TRUE,  // Guarantees to be different from TRUE and FALSE
} BOOLEAN3;


#pragma pack(1)

//
// Structure holding
//

typedef struct {
  UINT32  Start;      // Address of the range
  UINT32  Length;     // End Addr of the range
  UINT8   PamOffset;  // Pam Offset
} PAM_DATA;

#pragma pack()

typedef struct {
  UINT32                          Signature;
  EFI_HANDLE                      Handle;
  EFI_LEGACY_REGION2_PROTOCOL     LegacyRegion;
  EFI_HANDLE                      ImageHandle;
  UINT32                          SocketPresentBitMap;
} LEGACY_REGION_INSTANCE;



/**

  Install Driver to produce Legacy Region protocol.

  @param ImageHandle  -  The image handle.
  @param SystemTable  -  The system table.

  @retval EFI_SUCCESS  -  Legacy Region protocol installed.
  @retval Other        -  No protocol installed, unload driver.

**/
EFI_STATUS
EFIAPI
LegacyRegionInstall (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
;

#endif
