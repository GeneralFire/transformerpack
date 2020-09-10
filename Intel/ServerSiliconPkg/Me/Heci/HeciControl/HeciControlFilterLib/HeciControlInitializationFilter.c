/** @file
  HECI Control protocol driver filtering functionality for Heci Initialization(Dxe, Smm, Pei)

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2020 Intel Corporation.

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

**/

#include <PiDxe.h>
#include <Base.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/MeTypeGetLib.h>
#include <Library/MeTypeLib.h>
#include <Library/MeUtilsLib.h>
#include <Protocol/HeciTransportProtocol.h>
#include <Register/HeciRegs.h>
#include <Register/MeRegs.h>
#include <HeciControlFilter.h>
#include <BupMsgs.h>
#include <Sps.h>
#include <MeState.h>


#define HECI_CONTROL_DEBUG   "[HECI Control-%d %s]"

extern CHAR16                *mHeciModuleDebugPrefix;

/**
  HECI Control Initialization filter procedure

  This function returns if HECI interface is allowed for initialization

  @param[in]    HeciTransport  HECI device handle identifies device function will work on

  @retval TRUE                 Initialization allowed
  @retval FALSE           `    Initialization disallowed
**/
BOOLEAN
FilterHeciInitialization (
  IN     HECI_TRANSPORT       *HeciTransport
  )
{
  EFI_STATUS         Status;
  BOOLEAN            AllowHeciInitiaLization;
  HECI_DEVICE_INFO   HeciDeviceInfo;
  UINT8              FwType;
  UINT8              FwSubType;
  HECI_FWS_REGISTER  MeFirmwareStatus;
  SPS_NMFS           NmFirmwareStatus;

  AllowHeciInitiaLization = FALSE;
  if (HeciTransport != NULL) {
    HeciTransport->GetHeciDeviceInfo (HeciTransport, &HeciDeviceInfo);
    if (HeciDeviceInfo.HeciNo == 1) {
      // Always Allow HECI 1 initialization
      AllowHeciInitiaLization = TRUE;
    } else {
      if (HeciDeviceInfo.HeciPch == 0) {
        // Check PCH 0 only
        Status = GetMeFwTypeSubTypeFromHob (
                   &FwType,
                   &FwSubType
                   );
        if (!EFI_ERROR (Status) && (FwType == ME_TYPE_SPS)) {
          // Other Heci initialize for SPS only when NM is enabled
          if (HeciDeviceInfo.HeciNo == 2) {
            Status = MeBiosGetMeStatusRaw (&MeFirmwareStatus.ul);
            if (!EFI_ERROR (Status) && (MeFirmwareStatus.r.CurrentState == ME_STATE_NORMAL)) {
              NmFirmwareStatus.UInt32 = PciSegmentRead32 (
                                          PCI_SEGMENT_LIB_ADDRESS (
                                            GetHeciPciSegment (),
                                            GetHeciPciBus (),
                                            GetHeciPciDevice (),
                                            HECI2_DEVICE,
                                            R_ME_HFS
                                            ));
              if ((NmFirmwareStatus.UInt32 != 0xFFFFFFFF) && NmFirmwareStatus.Bits.NmEnabled) {
                AllowHeciInitiaLization = TRUE;
              }
            }
          } else if (HeciDeviceInfo.HeciNo == 3) {
            if (IsInSmm (NULL)) {
              AllowHeciInitiaLization = TRUE;
            }
          }
        }
      }
    }
    DEBUG ((DEBUG_WARN, HECI_CONTROL_DEBUG, HeciDeviceInfo.HeciNo, mHeciModuleDebugPrefix));
    DEBUG ((DEBUG_ERROR, " Initialization is %aallowed\n", AllowHeciInitiaLization ? "" : "not "));
  }

  return AllowHeciInitiaLization;
}
