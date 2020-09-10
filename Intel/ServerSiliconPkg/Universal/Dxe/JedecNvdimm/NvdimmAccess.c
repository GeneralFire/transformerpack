/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2020 Intel Corporation. <BR>

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

#include "JedecNvdimm.h"
#include <Library/ProcMemInitChipLib.h>
#include <Library/ProcSmbIpLib.h>
#ifdef SKX_HOST
#include <Library/ProcSmbIpLib/Skx/ProcSmbChipCommonSkx.h>
#else
#include <Library/ProcSmbIpLib/10nm/ProcSmbChipCommon10nm.h>
#endif
#include <Include/ProcSmbStruct.h>

#define MAX_SMB_RETRY 50

/**
  Initializes the SMB_DEVICE_STRUCT for a given Nvdimm I2C device

  @param[in]  Socket  - Socket number
  @param[in]  Channel - Channel number
  @param[in]  Dimm    - Dimm number
  @param[out] Device  - Pointer to smbDevice

  @retval EFI_SUCCESS  Successfully retrieved SMBus address data
**/
EFI_STATUS
EFIAPI
NvdimmNInitSmbusDeviceStructure (
  IN  UINT8             Socket,
  IN  UINT8             Channel,
  IN  UINT8             Dimm,
  OUT SMB_DEVICE_STRUCT  *Device
  )
{
  EFI_STATUS Status;

  //
  // Initialize structure for NVDIMM-N SMBUS device
  //
  Device->compId = SPD;
  Device->address.controller = PLATFORM_SMBUS_CONTROLLER_PROCESSOR;
  Device->address.deviceType = DTI_NVDIMM_I2C;

  Status = GetSmbAddress (Socket, Channel, Dimm, Device);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Error retrieving DIMM SMBus address\n"));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}



/**

  Read SMBUS device at a given device and offset

  @param Nvdimm         - Nvdimm structure
  @param byteOffset  - Offset in Device
  @param data        - Pointer to store value

  @retval EFI_STATUS
**/

EFI_STATUS
ReadSmb (
   JEDEC_NVDIMM                 *Nvdimm,
   UINT8                        byteOffset,
   UINT8                        *data
   )
{
  SMB_DEVICE_STRUCT SmbusDevice;
  EFI_STATUS        Status;
  UINT8             Count;
  UINT8             NodeId;
  UINT8             SktCh;

  NodeId = SKTMC_TO_NODE(Nvdimm->SocketId, Nvdimm->ImcId);
  SktCh = NODECH_TO_SKTCH(NodeId, Nvdimm->Ch);
  Count = 0;

  NvdimmNInitSmbusDeviceStructure(Nvdimm->SocketId, SktCh, Nvdimm->Dimm, &SmbusDevice);

  do{
    if (Count > MAX_SMB_RETRY) {
      DEBUG((EFI_D_ERROR, "ReadSmb() failed.\n"));
      break;
    }
    if (Count > 0){
      DEBUG((EFI_D_ERROR, "%d. unsuccessful read via SMB, trying again.\n", Count, Nvdimm->ImcId));
    }
    Status = ReadProcSmb(SmbusDevice, byteOffset, data);
    Count++;
    MicroSecondDelay(1000);
  }while(Status != EFI_SUCCESS);
  DEBUG((EFI_D_INFO, "ReadSmb() S:%d Ch:%d D:%d Imc:%d, Data = 0x%x\n", Nvdimm->SocketId, SktCh, Nvdimm->Dimm, Nvdimm->ImcId, *data));

  return Status;
}

/**

  Write SMBUS dev/offset with specified data

  @param Nvdimm         - Nvdimm structure
  @param byteOffset  - byte offset within device
  @param data        - pointer to data to write

  @retval EFI_STATUS

**/
EFI_STATUS
WriteSmb (
   JEDEC_NVDIMM                 *Nvdimm,
   UINT8                        byteOffset,
   UINT8                        *data
   )
{
  UINT8             NodeId;
  UINT8             SktCh;
  UINT8             Count;
  EFI_STATUS        Status;
  SMB_DEVICE_STRUCT SmbusDevice;

  NodeId = SKTMC_TO_NODE(Nvdimm->SocketId, Nvdimm->ImcId);
  SktCh  = NODECH_TO_SKTCH(NodeId, Nvdimm->Ch);
  Count  = 0;

  NvdimmNInitSmbusDeviceStructure(Nvdimm->SocketId, SktCh, Nvdimm->Dimm, &SmbusDevice);

  DEBUG((EFI_D_INFO, "WriteSmb(): offset:0x%x data:0x%x\n", byteOffset, *data));

  do{
    if (Count > MAX_SMB_RETRY) {
      DEBUG((EFI_D_ERROR, "WriteSmb() failed.\n"));
      break;
    }
    if (Count > 0) {
      DEBUG((EFI_D_ERROR, "%d. unsuccessful write via SMB, trying again.\n", Count, Nvdimm->ImcId));
    }
    Status = WriteProcSmb(SmbusDevice, byteOffset, data);
    Count++;
    MicroSecondDelay(1000);
  } while (Status != EFI_SUCCESS);

  return Status;
}

EFI_STATUS
JedecSwitchPage(
   JEDEC_NVDIMM                 *Nvdimm,
   UINT8                        Page
   )
{
  UINT8         CurrentPage;
  UINT64        StartTime;
  UINT64        TimeDiff;
  EFI_STATUS    Status;

  Status = ReadSmb (Nvdimm, OPEN_PAGE, &CurrentPage);
  if (EFI_ERROR (Status)) {
      return Status;
  }
  DEBUG ((DEBUG_INFO, "Page %d requested. Current Page = %d\n", Page, CurrentPage));

  if (CurrentPage != Page) {
    Status = WriteSmb (Nvdimm, OPEN_PAGE, &Page);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    StartTime = GetPerformanceCounter ();

    do {
      Status = ReadSmb (Nvdimm, OPEN_PAGE, &CurrentPage);
      if (!EFI_ERROR (Status) && CurrentPage == Page) {
        DEBUG ((DEBUG_INFO, "Switching Page Success.\n"));
        return EFI_SUCCESS;
      }
      TimeDiff = GetTimeInNanoSecond (GetPerformanceCounter () - StartTime) * 1000000; // Convert Time Diff to ms
    } while (TimeDiff < 5000); // TODO: For now 5s hardcoded as max wait time, but we should read time needed from PAGE_SWITCH_LATENCY regs

    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "Page %d already set.\n", Page));
  return EFI_SUCCESS;
}
