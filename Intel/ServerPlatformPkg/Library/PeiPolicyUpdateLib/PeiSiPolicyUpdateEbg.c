/** @file
  This file is SampleCode of the library for Intel Silicon PEI
  Platform Policy initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2019 Intel Corporation. <BR>

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

#include "PeiSiPolicyUpdateEbg.h"

/*
STATIC SVID_SID_INIT_ENTRY mSsidTablePtr[SI_MAX_DEVICE_COUNT] = {0};
*/

/**
  This provide sample code to update SSID table policy.

  @param[in] FspsUpd/SiConfig  FSPSUPD structure or SICONFIG policy
**/
STATIC
VOID
UpdateSsidPolicy (
  SI_CONFIG  *SiConfig
  )
{
/*
  Below provides the sample code to update SSID table.
  This is valid when SkipSsidProgramming is FALSE;
  It doesn't need to contain entries for all Intel internal devices.
  It can only contains the SVID_SID_INIT_ENTRY entries for those Dev# Func# which needs
  to be override.
  In the enties, only Dev, Function, SubSystemVendorId, and SubSystemId are required.

  E.g. Platform only needs to override BDF 0:31:5 to AAAA:BBBB and BDF 0:31:3 to CCCC:DDDD,
  it can be done in platform like this:
*/
/*
  UINT32                             EntryCount;
  EntryCount = 0;
  mSsidTablePtr[EntryCount].Address.Bits.Device   = SpiDeviceNumber ();
  mSsidTablePtr[EntryCount].Address.Bits.Function = SpiFunctionNumber ();
  mSsidTablePtr[EntryCount].SvidSidValue.SubSystemVendorId = 0xAAAA;
  mSsidTablePtr[EntryCount].SvidSidValue.SubSystemId       = 0xBBBB;
  EntryCount ++;
  mSsidTablePtr[EntryCount].Address.Bits.Device   = HdaDevNumber ();
  mSsidTablePtr[EntryCount].Address.Bits.Function = HdaFuncNumber ();
  mSsidTablePtr[EntryCount].SvidSidValue.SubSystemVendorId = 0xCCCC;
  mSsidTablePtr[EntryCount].SvidSidValue.SubSystemId       = 0xDDDD;
  EntryCount ++;
  ASSERT (EntryCount < SI_MAX_DEVICE_COUNT);

  SiConfig->SsidTablePtr = (UINT32*)(UINTN) mSsidTablePtr;
  SiConfig->NumberOfSsidTableEntry = (sizeof (mSsidTablePtr) / sizeof (SVID_SID_INIT_ENTRY));
*/
  SiConfig->SkipSsidProgramming    = FALSE;
  SiConfig->CustomizedSvid         = 0x00;
  SiConfig->CustomizedSsid         = 0x00;
  SiConfig->SsidTablePtr           = NULL;
  SiConfig->NumberOfSsidTableEntry = 0;
}

/**
  This function performs Silicon PEI Policy initialization.

  @param[in] SiPolicy  The Silicon Policy PPI instance

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
UpdatePeiSiPolicy (
  IN OUT      SI_POLICY_PPI     *SiPolicy
  )
{

  EFI_STATUS                         Status;
  SI_CONFIG                          *SiConfig;

  SiConfig        = NULL;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gSiConfigGuid, (VOID *) &SiConfig);
  ASSERT_EFI_ERROR (Status);

  UpdateSsidPolicy (SiConfig);
  return EFI_SUCCESS;
}
