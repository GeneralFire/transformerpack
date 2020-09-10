/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <Library/HbmCoreLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/RcDebugLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/HbmMemLib.h>

/**
  Writes HBM Mesh2Mem TAD register.

  @param[in] SocketId              Socket index.
  @param[in] HbmStackId            HBM stack index within the socket.
  @param[in] HbmM2mTadData         Information required to program the TAD_WR CSR.

  @retval EFI_SUCCESS              This function configured HBM Mesh2Mem register(s) successfully.
  @retval EFI_INVALID_PARAMETER    Some of input parameters are invalid.

**/
EFI_STATUS
EFIAPI
WriteHbmM2mTadData (
  IN UINT8                SocketId,
  IN UINT8                HbmStackId,
  IN HBM_M2M_TAD_DATA     *HbmM2mTadData
  )
{
  SAD_TAD_CONFIG          SadTadConfig;
  UINT8                   HbmMcId;
  UINT8                   HbmMaxImc;

  if ((SocketId >= MAX_SOCKET) || (HbmStackId >= MAX_HBM_IO) || (HbmM2mTadData == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  HbmMaxImc = GetMaxImcPerIoHbm ();

  ZeroMem (&SadTadConfig, sizeof (SadTadConfig));
  SadTadConfig.SadTadValid = TRUE;
  SadTadConfig.SadTadId = HbmM2mTadData->TadId;
  SadTadConfig.NmCacheableValid = HbmM2mTadData->NmCacheableValid;
  SadTadConfig.AddressLimit = HbmM2mTadData->AddressLimit;
  //
  // DDR4 set for 1LM and 2LM X-tile on NM MC or FM DDR tile in HBM$
  //
  SadTadConfig.Ddr4 = TRUE;
  SadTadConfig.Ddr4TadId = HbmM2mTadData->DdrTadId;
  SadTadConfig.SadTadWrEn = TRUE;
  for (HbmMcId = 0; HbmMcId < HbmMaxImc; HbmMcId++) {
    SetM2mTadConfigs (SocketId, HBM_M2M_INSTANCE_ID (HbmStackId, HbmMcId), &SadTadConfig);
  }

  return EFI_SUCCESS;
}


/**
  Writes HBM Mesh2Mem miscellaneous register(s).

  @param[in] SocketId              Socket index.
  @param[in] HbmStackId            HBM stack index within the socket.
  @param[in] HbmM2mMiscData        Information required to program the M2Mem Mode CSR.

  @retval EFI_SUCCESS              This function configured HBM Mesh2Mem register(s) successfully.
  @retval EFI_INVALID_PARAMETER    Some of input parameters are invalid.

**/
EFI_STATUS
EFIAPI
WriteHbmM2mMiscData (
  IN UINT8                SocketId,
  IN UINT8                HbmStackId,
  IN HBM_M2M_MISC_DATA    *HbmM2mMiscData
  )
{
  UINT8                   HbmMcId;
  UINT8                   HbmMaxImc;

  if ((SocketId >= MAX_SOCKET) || (HbmStackId >= MAX_HBM_IO) || (HbmM2mMiscData == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  HbmMaxImc = GetMaxImcPerIoHbm ();
  for (HbmMcId = 0; HbmMcId < HbmMaxImc; HbmMcId++) {

    //
    // If Xtile flows, set Defeatures0.WrCmpFromNm.
    // HBM always x-tile caches far memory in external MC's.
    //
    if (HbmM2mMiscData->NmCaching) {
      SetM2mDefeatures0WrCmpFromNm (SocketId, HBM_M2M_INSTANCE_ID (HbmStackId, HbmMcId), TRUE);
    }

    SetM2mModeMmap (
      SocketId,
      HBM_M2M_INSTANCE_ID (HbmStackId, HbmMcId),
      HbmM2mMiscData->NmCaching,  // Nmcaching
      0,                          // Pmem
      0,                          // Blockregion
      1                           // XTileCaching
      );

    //
    // Update X-Tile 2LM field in Mesh2mem IP.
    //
    if (HbmM2mMiscData->NmCaching) {
      SetM2mFmInSameTile (SocketId, HBM_M2M_INSTANCE_ID (HbmStackId, HbmMcId), TRUE);
    }
  }

  return EFI_SUCCESS;
}



