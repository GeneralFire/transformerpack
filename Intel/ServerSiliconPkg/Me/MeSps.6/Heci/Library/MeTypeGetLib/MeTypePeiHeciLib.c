/** @file
  MeTypePeiHeciLib.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Base.h>
#include <PiPei.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/PeiServicesLib.h>
#include <PiDxe.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <BupMsgs.h>
#include <Ppi/HeciControlPpi.h>
#include <Library/HobLib.h>
#include "Sps.h"
#include <Register/HeciRegs.h>
#include "MeAccess.h"
#include <Library/PeiMeLib.h>
#include <MeState.h>
#include <Register/MeRegs.h>
#include <Library/MeTypeGetLib.h>
#include <Library/MeTypeLib.h>
#include <Library/MeUtilsLib.h>

/**
  Get Me FW type from ME via HECI

  @param[out] FwType               ME Type
  @param[out] FwSubType            ME SubType

  @retval ME_TYPE_UNDEF            Can't determine ME Type
  @retval ME_TYPE_SPS              Detected SPS
  @retval ME_TYPE_CLIENT           Detected Client ME
  @retval ME_TYPE_DFX              Detected Dfx ME
  @retval ME_TYPE_DISABLED         Disable all ME flows
**/
ON_BOARD_ME_TYPE
PeiHeciGetMeType (
  OUT UINT8 *FwType,
  OUT UINT8 *FwSubType
  )
{
  ON_BOARD_ME_TYPE                RetVal = ME_TYPE_UNDEF;
#ifdef ME_SUPPORT_FLAG
  EFI_STATUS                      Status;
  UINT32                          Length;
  UINT32                          RecvLength;
  HECI_CONTROL                    *HeciPpi;
  GEN_GET_FW_TYPE                 MsgGenGetMeType;
  GEN_GET_FW_TYPE_ACK             MsgGenGetMeTypeRsp;
  UINT32                          MeMode;

  DEBUG ((DEBUG_INFO, "HECI: PeiHeciGetMeTypeMsg() start\n"));

  if (MeTypeIsSpsIgnition ()) {
    return ME_TYPE_DISABLED;
  }

  Status = PeiServicesLocatePpi (
            &gHeciControlPpiGuid,     // GUID
            0,                        // INSTANCE
            NULL,                     // EFI_PEI_PPI_DESCRIPTOR
            (VOID **) &HeciPpi        // PPI
            );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR: HECI PPI not installed\n"));
    return RetVal;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR(Status) ||
    ((MeMode != ME_MODE_NORMAL) &&
     (MeMode != ME_MODE_SECOVER) &&
      (MeMode != ME_MODE_DFX_FW))) {
    return RetVal;
  }

  MsgGenGetMeType.MkhiHeader.Data               = 0;
  MsgGenGetMeType.MkhiHeader.Fields.GroupId     = BUP_COMMON_GROUP_ID;
  MsgGenGetMeType.MkhiHeader.Fields.Command     = BUP_GET_ME_TYPE;
  MsgGenGetMeType.MkhiHeader.Fields.IsResponse  = 0;
  Length                                        = sizeof (GEN_GET_FW_TYPE);
  RecvLength                                    = sizeof (GEN_GET_FW_TYPE_ACK);

  ///
  /// Send Get Platform Type Request to ME
  ///
  Status = HeciPpi->HeciSendAndReceive (
                      HeciPpi,
                      HECI_DEFAULT_PCH_ID,
                      NULL,
                      HECI_DEFAULT_RETRY_NUMBER,
                      BIOS_FIXED_HOST_ADDR,
                      HECI_MKHI_MESSAGE_ADDR,
                      (UINT32*) &MsgGenGetMeType,
                      Length,
                      (UINT32*) &MsgGenGetMeTypeRsp,
                      &RecvLength,
                      NULL,
                      NULL
                      );

  if (!EFI_ERROR (Status) &&
      (MsgGenGetMeTypeRsp.MkhiHeader.Fields.IsResponse == 1) &&
      (MsgGenGetMeTypeRsp.MkhiHeader.Fields.GroupId == BUP_COMMON_GROUP_ID) &&
      (MsgGenGetMeTypeRsp.MkhiHeader.Fields.Command == BUP_GET_ME_TYPE)) {
    if (RecvLength != sizeof (GEN_GET_FW_TYPE_ACK)) {
      DEBUG ((DEBUG_ERROR, "HECI: PeiHeciGetMeTypeMsg() - Response: wrong length = %d\n",
        RecvLength));
    } else {
      DEBUG ((DEBUG_INFO, "HECI: PeiHeciGetMeTypeMsg() - OK: FwType = %d, FwSubType = %d\n",
        MsgGenGetMeTypeRsp.FwTypeData.FwType, MsgGenGetMeTypeRsp.FwTypeData.FwSubType));

      if (FwType != NULL) {
        *FwType = (UINT8)MsgGenGetMeTypeRsp.FwTypeData.FwType;
      }

      if (FwSubType != NULL) {
        *FwSubType = (UINT8)MsgGenGetMeTypeRsp.FwTypeData.FwSubType;
      }

      switch (MsgGenGetMeTypeRsp.FwTypeData.FwType) {
        case METYPE_SPS:
          RetVal = METYPE_SPS;
          if (MsgGenGetMeTypeRsp.FwTypeData.FwSubType == MESUBTYPE_SPS_TYPE_DFX) {
            DEBUG ((DEBUG_INFO, "HECI: PeiHeciGetMeTypeMsg() DFX FW discovered\n"));
            RetVal = METYPE_DFX;
          }
        break;
        case METYPE_SPS_EPO:
          DEBUG ((DEBUG_INFO, "HECI: PeiHeciGetMeTypeMsg() Use MeType SPS for SPS-EPO\n"));
          RetVal = METYPE_SPS;
        break;
        case METYPE_CLIENT:
          RetVal = METYPE_CLIENT;
        break;
        default:
          DEBUG ((DEBUG_ERROR, "HECI: PeiHeciGetMeTypeMsg() Unknown ME Type reported by ME\n"));
      }
    }
  }

  DEBUG ((DEBUG_INFO, "HECI: PeiHeciGetMeTypeMsg() Status is %r, returns %d\n",
    Status, RetVal));
#endif // ME_SUPPORT_FLAG

  return RetVal;
}
