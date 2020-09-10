/** @file
  CrystalRidge Measurement DXE Driver.

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

#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/UefiTcgPlatform.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/NvdimmCommonLib.h>
#include <IndustryStandard/Spdm.h>
#include <Library/TpmMeasurementLib.h>
#include <Include/MemMapHost.h>
#include <Library/SysHostPointerLib.h>
#include "CrystalRidgeMeasurement.h"

struct SystemMemoryMapHob *mSystemMemoryMap = NULL;

/**
  Convert the SPDM hash algo ID from the TCG hash Algorithm ID.

  @param TcgAlgId            TCG hash Algorithm ID

  @return SPDM hash algo ID
**/
UINT32
TcgAlgIdToSpdmHashAlgo (
  IN UINT16                                    TcgAlgId
  )
{
  switch (TcgAlgId) {
  case TPM_ALG_SHA256:
    return SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA_256;
  case TPM_ALG_SHA384:
  case TPM_ALG_SHA512:
  case TPM_ALG_SM3_256:
  default:
    break;
  }
  return 0;
}

/**
  This function implements logic to measure CrystalRidge firmware.

  @param[in]  Tcg2Protocol     Instance of protocol gEfiTcg2ProtocolGuid

**/
VOID
CrystalRidgeMeasureFw (
  VOID
  )
{
  UINT32                   PcrIndex;
  UINT32                   EventType;
  EFI_STATUS               Status;
  MEMORY_MAP_HOST          *MemMapHost;
  EFI_TCG_DCPMM_EVENT_DATA EventLog;
  UINT32                   Digest[NGN_FFW_BANK_NUM][NGN_FFW_CSR_NUM];
  EFI_GUID                 VendorGuid = DCPMM_VENDOR_GUID;
  UINT8                    sckt;
  UINT8                    ch;
  UINT8                    dimm;
  UINT8                    MaxChDdr;

  MemMapHost = GetMemMapHostPointer ();
  MaxChDdr = GetMaxChDdr ();

  PcrIndex = 2;
  EventType = EV_EFI_SPDM_FIRMWARE_BLOB;

  DEBUG((DEBUG_INFO, "Pmem MeasureFw\n"));

  CopyMem (EventLog.EventData.Signature, EV_DCPMM_EVENT_DATA_SIGNATURE, sizeof(EventLog.EventData.Signature));
  EventLog.EventData.Version               = EV_DCPMM_EVENT_DATA_VERSION;
  EventLog.EventData.Length                = sizeof(EFI_TCG_DCPMM_EVENT_DATA);
#ifdef DDR5_SUPPORT
  EventLog.EventData.SpdmHashAlgo          = TcgAlgIdToSpdmHashAlgo (TPM_ALG_SHA384);
#else
  EventLog.EventData.SpdmHashAlgo          = TcgAlgIdToSpdmHashAlgo (TPM_ALG_SHA256);
#endif
  EventLog.EventData.DeviceType            = EV_DEVICE_TYPE_OEM;
  EventLog.EventData.DevicePathSize        = sizeof(EventLog.EventData.DevicePath);
  EventLog.EventData.DevicePath.Type       = 1;
  EventLog.EventData.DevicePath.SubType    = 4;
  CopyMem (&EventLog.EventData.DevicePath.Guid, &VendorGuid, sizeof(EFI_GUID));

  for (sckt = 0; sckt < MAX_SOCKET; sckt++) {
    if (!mSystemMemoryMap->Socket[sckt].SocketEnabled) {
      continue;
    }

    for (ch = 0; ch < MaxChDdr; ch++) {
      if (!mSystemMemoryMap->Socket[sckt].ChannelInfo[ch].Enabled) {
        continue;
      }

      for(dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((!mSystemMemoryMap->Socket[sckt].ChannelInfo[ch].DimmInfo[dimm].DcpmmPresent) ||
             (!mSystemMemoryMap->Socket[sckt].ChannelInfo[ch].DimmInfo[dimm].Enabled)) {
          continue;
        }

        CopyMem (Digest, MemMapHost->SocketInfo[sckt].ChannelInfo[ch].FfwDigestPerDimm[dimm].Digest,
                 sizeof(Digest));
        CopyMem (EventLog.Digest, Digest, sizeof(EventLog.Digest));

        EventLog.context.Socket = sckt;
        EventLog.context.Ch = ch;
        EventLog.context.Dimm = dimm;
        ZeroMem (&EventLog.EventData.IdentifyData.fwr, sizeof (EventLog.EventData.IdentifyData.fwr));
        CopyMem (&EventLog.EventData.IdentifyData.fwr,
                 &mSystemMemoryMap->Socket[sckt].ChannelInfo[ch].DimmInfo[dimm].FirmwareVersion,
                 sizeof (EventLog.EventData.IdentifyData.fwr));
        ZeroMem (EventLog.EventData.IdentifyData.sn, NGN_MAX_SERIALNUMBER_STRLEN);
        CopyMem (EventLog.EventData.IdentifyData.sn,
                 mSystemMemoryMap->Socket[sckt].ChannelInfo[ch].DimmInfo[dimm].serialNumber,
                 NGN_MAX_SERIALNUMBER_STRLEN);
        ZeroMem (EventLog.EventData.IdentifyData.Pn, NGN_MAX_SERIALNUMBER_STRLEN);
        CopyMem (EventLog.EventData.IdentifyData.Pn,
                 mSystemMemoryMap->Socket[sckt].ChannelInfo[ch].DimmInfo[dimm].PartNumber,
                 NGN_MAX_PARTNUMBER_STRLEN);

        Status = TpmMeasureAndLogData (
                   PcrIndex,
                   EventType,
                   &EventLog,
                   EventLog.EventData.Length,
                   Digest,
                   sizeof(Digest)
                   );
        if (EFI_ERROR(Status)) {
          DEBUG((DEBUG_ERROR, "Pmem MeasureFw error - %r\n", Status));
          break;
        }
      }
    }
  }

  return;
}

/**
  End of DXE Event notification handler.

  CrystalRidge firmware are measured in this event notification handler.

  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context

**/
EFI_STATUS
EFIAPI
CrystalRidgeMeasurementOnEndOfDxe(
  IN      EFI_EVENT                 Event,
  IN      VOID                      *Context
  )
{
  CrystalRidgeMeasureFw ();

  gBS->CloseEvent(Event);

  return EFI_SUCCESS;
}

/**
  The driver's entry point.

  Register a end of DXE event to measure CrystalRidge firmware.

  @param[in] ImageHandle  The firmware allocated handle for the EFI image.
  @param[in] SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS     The entry point is executed successfully.
  @retval other           Some error occurs when executing this entry point.

**/

EFI_STATUS
EFIAPI
DriverEntry (
  IN      EFI_HANDLE                ImageHandle,
  IN      EFI_SYSTEM_TABLE          *SystemTable
  )
{
  EFI_EVENT                       Event;

  mSystemMemoryMap = GetSystemMemoryMapHob();
  if (mSystemMemoryMap->DcpmmPresent == 0) {
    DEBUG ((EFI_D_INFO, "No Pmem, Pmem Measurement Driver is not going to be loaded.\n"));
    return EFI_SUCCESS;
  }

  //
  // Create end of DXE event to measure CrystalRidge firmware
  //
  gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    CrystalRidgeMeasurementOnEndOfDxe,
                    NULL,
                    &gEfiEndOfDxeEventGroupGuid,
                    &Event
                    );

  return EFI_SUCCESS;
}

