/** @file
  MeFiaMuxLib.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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
**/

#include <Uefi.h>
#include <Library/PciLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MeFiaMuxLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/PchInfoLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/PchFiaLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Ppi/HeciControlPpi.h>
#include <Sps.h>
#include <Library/PchFiaLib.h>

/**
  Get number of HSIO lanes for given CPU

  @param [out]  LaneGapLow  pointer to return low limit of unused lines
  @param [out]  LaneGapHi   pointer to return hi limit of unused lines

  @retval UINT8  Number of Hsio lanes in the system
**/
UINT8
GetHsioLanes (
  OUT UINT8 *LaneGapLow,
  OUT UINT8 *LaneGapHi
  )
{
  UINT8 HsioLanes;
  UINT8 LaneGapHiLocal;
  UINT8 LaneGapLowLocal;

  if (LaneGapHi == NULL) {
    LaneGapHi = &LaneGapHiLocal;
  }
  if (LaneGapLow == NULL) {
    LaneGapLow = &LaneGapLowLocal;
  }

  HsioLanes = PchFiaGetMaxLaneNum ();
  if (HsioLanes > PCH_MAX_HSIO_LANES) {
    DEBUG ((DEBUG_ERROR, "ERROR: PchFiaGetMaxLaneNum = %d > PCH_MAX_HSIO_LANES = %d\n", HsioLanes, PCH_MAX_HSIO_LANES));
    ASSERT (FALSE);
    HsioLanes = PCH_MAX_HSIO_LANES;
  }
  *LaneGapLow = HsioLanes;
  *LaneGapHi = 0;

  if (IsCdfPch ()) {
    if (IsCpuAndRevision (CPU_TNR, REV_ALL)) {
      if (LaneGapLow != NULL) {
        *LaneGapLow = HSIO_GAP_TNR_LOW;
      }
      if (LaneGapHi != NULL) {
        *LaneGapHi = HSIO_GAP_TNR_HI;
      }
    } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
      if (LaneGapLow != NULL) {
        *LaneGapLow = HSIO_GAP_SNR_LOW;
      }
      if (LaneGapHi != NULL) {
        *LaneGapHi = HSIO_GAP_SNR_HI;
      }
    }
  } else {
    // no gaps
  }

  return HsioLanes;
}

/**
  Print FIA MUX configuration

  @param [in] FiaMuxRecordsCount        Number of elements in FiaMux table
  @param [in] FiaMux                    Pointer to FIA Mux Configuration table to be set by Intel ME FW or stored in SPI
  @param [in] SataRecordsCount          Number of elements in Sata table
  @param [in] SataLanes                 Number of SATA Lanes in Sata table
  @param [in] Sata                      Pointer to FIA Lanes Configuration for SATA Controllers to be set by Intel ME FW
  @param [in] PcieRootPortRecordsCount  Number of elements in PcieRootPort table
  @param [in] PcieRootPort              Pointer to PCIe Root Ports configuration to be set by Intel ME FW
**/
VOID
PrintFiaMuxConfig (
  IN UINT8                        FiaMuxRecordsCount,
  IN FIA_MUX_RECORD               *FiaMux,
  IN UINT8                        SataRecordsCount,
  IN UINT8                        SataLanes,
  IN SATA_RECORD                  *Sata,
  IN UINT8                        PcieRootPortRecordsCount,
  IN PCIE_ROOT_PORT_RECORD        *PcieRootPort
  )
{
  UINT8 Index;

  DEBUG ((DEBUG_INFO, "[SPS] FIA Mux Lanes Configuration\n"));
  DEBUG ((DEBUG_INFO, "[SPS] FiaMux pointer       = %p\n", FiaMux));
  DEBUG ((DEBUG_INFO, "[SPS] Sata pointer         = %p\n", Sata));
  DEBUG ((DEBUG_INFO, "[SPS] PcieRootPort pointer = %p\n", PcieRootPort));
  if (FiaMux != NULL) {
    DEBUG ((DEBUG_INFO, "\tFiaMuxRecordsCount = %d\n", FiaMuxRecordsCount));
    for (Index = 0; Index < FiaMuxRecordsCount; Index++) {
      DEBUG ((DEBUG_INFO, "\t\tFiaMuxSelection     = %d ", FiaMux[Index].FiaMuxSelection));
      switch (FiaMux[Index].FiaMuxSelection) {
        case FIA_MUX_LANE_DISABLED:
          DEBUG ((DEBUG_INFO, "(FIA_MUX_LANE_DISABLED)\n"));
          break;
        case FIA_MUX_LANE_PCIE_OWNER:
          DEBUG ((DEBUG_INFO, "(FIA_MUX_LANE_PCIE_OWNER)\n"));
          break;
        case FIA_MUX_LANE_SATA_OWNER:
          DEBUG ((DEBUG_INFO, "(FIA_MUX_LANE_SATA_OWNER)\n"));
          break;
        case FIA_MUX_LANE_USB_OWNER:
          DEBUG ((DEBUG_INFO, "(FIA_MUX_LANE_USB_OWNER)\n"));
          break;
        default:
          DEBUG ((DEBUG_ERROR, "(ERROR: FiaMux: FiaMuxSelection = ???)\n"));
      }
      DEBUG ((DEBUG_INFO, "\t\tFiaMuxHardwareIndex = %d\n", FiaMux[Index].FiaMuxHardwareIndex));
    }
  }
  if (Sata != NULL) {
    DEBUG ((DEBUG_INFO, "\tSataRecordsCount    = %d\n", SataRecordsCount));
    DEBUG ((DEBUG_INFO, "\tSata assigned lanes = %d\n", SataLanes));
    for (Index = 0; Index < SataRecordsCount; Index++) {
      DEBUG ((DEBUG_INFO, "\t\tSataSelection     = %d ", Sata[Index].SataSelection));
      switch (Sata[Index].SataSelection) {
        case SATA_CONTROLLER_ASSIGNED:
          DEBUG ((DEBUG_INFO, "(SATA_CONTROLLER_ASSIGNED)\n"));
          break;
        case SATA_CONTROLLER_NOT_ASSIGNED:
          DEBUG ((DEBUG_INFO, "(SATA_CONTROLLER_NOT_ASSIGNED)\n"));
          break;
        default:
          DEBUG ((DEBUG_ERROR, "(ERROR: FiaMux: SataSelection = ???)\n"));
      }
      DEBUG ((DEBUG_INFO, "\t\tSataHardwareIndex = %d\n", Sata[Index].SataHardwareIndex));
    }
  }
  if (PcieRootPort != NULL) {
    DEBUG ((DEBUG_INFO, "\tPcieRootPortRecordsCount = %d\n", PcieRootPortRecordsCount));
    for (Index = 0; Index < PcieRootPortRecordsCount; Index++) {
      DEBUG ((DEBUG_INFO, "\t\tPcieRootPortEnable        = %d\n", PcieRootPort[Index].PcieRootPortEnable));
      DEBUG ((DEBUG_INFO, "\t\tPcieRootPortLinkWidth     = %d ", PcieRootPort[Index].PcieRootPortLinkWidth));
      switch (PcieRootPort[Index].PcieRootPortLinkWidth) {
        case PCIE_ROOT_PORT_LINK_WIDTH_SET_BY_BICTRL:
          DEBUG ((DEBUG_INFO, "(PCIE_ROOT_PORT_LINK_WIDTH_SET_BY_BICTRL)\n"));
          break;
        case PCIE_ROOT_PORT_LINK_WIDTH_X1:
          DEBUG ((DEBUG_INFO, "(PCIE_ROOT_PORT_LINK_WIDTH_X1)\n"));
          break;
        default:
          DEBUG ((DEBUG_ERROR, "(ERROR: FiaMux: PcieRootPortLinkWidth = ???)\n"));
      }
      DEBUG ((DEBUG_INFO, "\t\tPcieRootPortHardwareIndex = %d\n", PcieRootPort[Index].PcieRootPortHardwareIndex));
    }
  }
}

/**
  Count bytes needed to align size to UINT32

  @param [in] CommandSize    Command size to compute alignment for

  @retval     UINT8          Computed filer in bytes
**/
UINT8
AddFiler (
  IN UINT32 CommandSize
  )
{
  UINT8 Filer;

  Filer = CommandSize % sizeof (UINT32);
  if (Filer > 0) {
    Filer = sizeof (UINT32) - Filer;
  }

  return Filer;
}

/**
  Sends FIA MUX configuration to ME.

  @param [in] pConfig           Pointer to FIA MUX configuration to set in ME,
                                when NULL, Invalidate field is set to 1 (causing
                                invalidate of configuration stored in flash -
                                configuration from Soft Straps will be used)

  @retval EFI_NOT_READY         Returned if this API is called before ME signals init
                                complete, or after END_OF_POST was sent.
  @retval EFI_DEVICE_ERROR      Returned if HECI transport is not available.
  @retval EFI_PROTOCOL_ERROR    Unexpected protocol error returned by ME.
  @retval EFI_OUT_OF_RESOURCES  Requested configuration refused by ME, because
                                it does not match the SoC SKU.
  @retval EFI_ABORTED           Requested configuration refused by ME, because it is
                                invalid, or nothing to update
  @retval EFI_INVALID_PARAMETER Input parameter is invalid
  @retval EFI_UNSUPPORTED       FiaMux is not supported
  @retval EFI_SUCCESS           FiaMux configuration has been sent successfully
**/
EFI_STATUS
EFIAPI
MeFiaMuxConfigSet (
  IN ME_FIA_CONFIG *pConfig
  )
{
#if ME_SPS_SUPPORT
  EFI_STATUS                      Status;
  UINT32                          CommandSize;
  UINT32                          RecvLength;
  VOID                            *Heci;
  ICC_MSG_SET_FIA_MUX_CONFIG_REQ  *Req;
  ICC_MSG_SET_FIA_MUX_CONFIG_RSP  *Rsp;
  UINT8                           HsioLanes;
  SATA_RECORD                     *Sata;
  PCIE_ROOT_PORT_RECORD           *PcieRootPort;
  UINT8                           Index;
  UINT8                           LowLane;
  UINT8                           HiLane;
  UINT8                           PcieRootPortLanes;
  UINT8                           SataLanes;
  UINT8                           *PcieRootPortRecordsCount;
  UINT8                           *SataRecordsCount;
  UINT8                           *FiaMuxRecordsCount;

  DEBUG ((DEBUG_INFO, "[SPS] (ICC) %a\n", __FUNCTION__));

  if (pConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: (ICC) %a: pConfig cannot be NULL\n", __FUNCTION__));
    ASSERT (pConfig != NULL);
    return EFI_INVALID_PARAMETER;
  }

  Heci = GetHeci (NULL);
  if (Heci == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: (ICC) %s: Cannot locate HECI\n", __FUNCTION__));
    ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
    return EFI_DEVICE_ERROR;
  }

  HsioLanes = GetHsioLanes (&LowLane, &HiLane);
  // Each root port contains 2 lanes
  PcieRootPortLanes = HsioLanes / 2;
  // Calculate maximum command size.
  CommandSize = sizeof (ICC_HEADER) +
                4 * sizeof (UINT8) +
                (HsioLanes * sizeof (FIA_MUX_RECORD)) +
                (HsioLanes * sizeof (SATA_RECORD)) +
                (HsioLanes * sizeof (PCIE_ROOT_PORT_RECORD));
  CommandSize += AddFiler (CommandSize);

  Req = (ICC_MSG_SET_FIA_MUX_CONFIG_REQ*) AllocateZeroPool (CommandSize);
  if (Req == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Can't allocate memory in %a ()\n", __FUNCTION__));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return EFI_OUT_OF_RESOURCES;
  }
  Rsp = (ICC_MSG_SET_FIA_MUX_CONFIG_RSP*) Req;

  FiaMuxRecordsCount = &(Req->FiaMuxRecordsCount);
  *FiaMuxRecordsCount = 0;
  // Construct FiaMux
  for (Index = 0; Index < HsioLanes; Index++) {
    if ((Index > LowLane) && (Index < HiLane)) {
      continue;
    }
    Req->FiaMux[*FiaMuxRecordsCount].FiaMuxSelection = pConfig->FiaMux[Index].FiaMuxSelection;
    Req->FiaMux[*FiaMuxRecordsCount].FiaMuxHardwareIndex = Index;
    (*FiaMuxRecordsCount)++;
  }
  DEBUG ((DEBUG_INFO, "[SPS] %a () : FiaMux lanes to override = %d\n", __FUNCTION__, *FiaMuxRecordsCount));
  if (*FiaMuxRecordsCount == 0) {
    Status = EFI_ABORTED;
    DEBUG ((DEBUG_INFO, "[SPS] %a () : Nothing to send (%r)\n", __FUNCTION__, Status));
    goto MeFiaMuxConfigSetExit;
  }
  Sata = (SATA_RECORD*) (((UINT8*) &(Req->FiaMux)) + *FiaMuxRecordsCount * sizeof (FIA_MUX_RECORD));
  SataRecordsCount = &(Req->SataRecordsCount);
  *SataRecordsCount = 0;
  SataLanes = 0;
  for (Index = 0; Index < HsioLanes; Index++) {
    if ((Index > LowLane) && (Index < HiLane)) {
      continue;
    }
    // send all overrides for SATA lanes (set SATA_CONTROLLER_NOT_ASSIGNED or SATA_CONTROLLER_ASSIGNED)
    // assuming non SATA lanes are not valid
    Sata[*SataRecordsCount].SataSelection = pConfig->Sata[Index].SataSelection;
    Sata[*SataRecordsCount].SataHardwareIndex = Index;
    (*SataRecordsCount)++;
    if (pConfig->FiaMux[Index].FiaMuxSelection == FIA_MUX_LANE_SATA_OWNER) {
      SataLanes++;
    }
  }
  PcieRootPort = (PCIE_ROOT_PORT_RECORD*) (((UINT8*) Sata) + *SataRecordsCount * sizeof (SATA_RECORD));
  PcieRootPortRecordsCount = &(Req->PcieRootPortRecordsCount);
  *PcieRootPortRecordsCount = 0;
  for (Index = 0; Index < HsioLanes; Index += 2) {
    if ((Index > LowLane) && (Index < HiLane)) {
      continue;
    }
    if ((*PcieRootPortRecordsCount >= PcieRootPortLanes) || (*PcieRootPortRecordsCount >= (sizeof (Req->PcieRootPort) / sizeof (Req->PcieRootPort [0]) / 2))) {
      DEBUG ((
        DEBUG_ERROR,
        "[SPS] ERROR: %a () Too many root-ports configured (max expected %d)\n",
        __FUNCTION__,
        PcieRootPortLanes
        ));
      break;
    }
    PcieRootPort[*PcieRootPortRecordsCount].PcieRootPortEnable = pConfig->PcieRootPort[Index].PcieRootPortEnable;
    PcieRootPort[*PcieRootPortRecordsCount].PcieRootPortLinkWidth = pConfig->PcieRootPort[Index].PcieRootPortLinkWidth;
    PcieRootPort[*PcieRootPortRecordsCount].PcieRootPortHardwareIndex = Index;
    (*PcieRootPortRecordsCount)++;
  }

  Req->FiaMuxCfgInvalidate = pConfig->FiaMuxCfgInvalidate;
  switch (pConfig->FiaMuxCfgInvalidate) {
    case 0:
      DEBUG ((DEBUG_INFO, "[SPS] FIA Mux Configuration is Valid and should be stored on Flash\n"));
      break;

    case 1:
      DEBUG ((DEBUG_INFO, "[SPS] FIA Mux configuration on Intel ME Flash should be invalidated."));
      DEBUG ((DEBUG_INFO, " This means that the Lanes configuration will be based on Soft Straps Settings\n"));
      break;

    default:
      DEBUG ((DEBUG_ERROR, "[SPS] Req->FiaMuxCfgInvalidate has unknown value\n"));
      Status = EFI_INVALID_PARAMETER;
      goto MeFiaMuxConfigSetExit;
  }
  DEBUG ((DEBUG_INFO, "[SPS] FIA Mux Lanes General Override     = %d\n", Req->FiaMuxCfgInvalidate));
  PrintFiaMuxConfig (
    *FiaMuxRecordsCount,
    Req->FiaMux,
    *SataRecordsCount,
    SataLanes,
    Sata,
    *PcieRootPortRecordsCount,
    PcieRootPort
    );

  // Set actual command size
  CommandSize =  sizeof (ICC_HEADER) + (4 * sizeof (UINT8)) +
                (*FiaMuxRecordsCount * sizeof (FIA_MUX_RECORD)) +
                (*SataRecordsCount * sizeof (SATA_RECORD)) +
                (*PcieRootPortRecordsCount * sizeof (PCIE_ROOT_PORT_RECORD));
  CommandSize += AddFiler (CommandSize);
  RecvLength = sizeof (ICC_MSG_SET_FIA_MUX_CONFIG_RSP);

  Req->Header.BufferLength = CommandSize - sizeof (ICC_HEADER);
  Req->Header.ApiVersion   = ICC_API_VERSION;
  Req->Header.IccCommand   = ICC_SET_FIA_MUX_CONFIG_CMD;

  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_ICC_MESSAGE_ADDR,
             (UINT32*) Req,
             CommandSize,
             (UINT32*) Rsp,
             &RecvLength,
             NULL,
             NULL
             );
  if (EFI_ERROR (Status) ||
     (Rsp->Header.IccCommand != ICC_SET_FIA_MUX_CONFIG_CMD)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: (ICC) %a: Message failed! EFI_STATUS = %r\n", __FUNCTION__, Status));
    Status = EFI_PROTOCOL_ERROR;
    goto MeFiaMuxConfigSetExit;
  }

  if (RecvLength != sizeof (ICC_MSG_SET_FIA_MUX_CONFIG_RSP)) {
    Status = EFI_PROTOCOL_ERROR;
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: (ICC) %a: Message failed! Response is too short (%d bytes).\n", __FUNCTION__, RecvLength));
    goto MeFiaMuxConfigSetExit;
  }

  switch (Rsp->Header.IccResponse) {
    case ICC_STATUS_SUCCESS:
      DEBUG ((DEBUG_INFO, "[SPS] (ICC) %a: FIA configuration set successfully\n", __FUNCTION__));
      Status = EFI_SUCCESS;
      break;

    case ICC_STATUS_FIA_MUX_CONFIG_SKU_MISMATCH:
      DEBUG ((DEBUG_ERROR, "[SPS] ERROR: (ICC) %a: SKU mismatch\n", __FUNCTION__));
      Status = EFI_OUT_OF_RESOURCES;
      break;

    case ICC_STATUS_INVALID_PARAMS:
      DEBUG ((DEBUG_ERROR, "[SPS] ERROR: (ICC) %a: FIA configuration invalid\n", __FUNCTION__));
      Status = EFI_ABORTED;
      break;

    default:
      DEBUG ((
        DEBUG_ERROR,
        "[SPS] ERROR: (ICC) %a: Error in response! ICC Status = 0x%x\n",
        __FUNCTION__,
        Rsp->Header.IccResponse
        ));
      Status = EFI_PROTOCOL_ERROR;
      break;
  }

MeFiaMuxConfigSetExit:
  FreePool (Req);
  DEBUG ((DEBUG_INFO, "[SPS] (ICC) %a: End - %r\n", __FUNCTION__, Status));

  return Status;
#else // ME_SPS_SUPPORT
  return EFI_UNSUPPORTED;
#endif // ME_SPS_SUPPORT
} // MeFiaMuxConfigSet ()

/**
  Read FIA MUX configuration from ME.

  @param [out] pConfig              Pointer to a buffer for ME FIA MUX configuration
  @param [out] pLanesAllowed        Pointer to a buffer for the number of lanes allowed
  @param [out] FiaMuxConfigFromMe   Pointer to a buffer for ME FIA MUX configuration (without overwriting)
  @param [out] OverridesActive      Pointer to a OverridesActive indicator.
                                    If TRUE, set command should be executed

  @retval EFI_NOT_READY         Returned if this API is called before ME signals init
                                complete, or after END_OF_POST was sent.
  @retval EFI_INVALID_PARAMETER Returned if any passed NULL configuration pointer
  @retval EFI_DEVICE_ERROR      Returned if HECI transport is not available.
  @retval EFI_PROTOCOL_ERROR    Unexpected protocol error returned by ME.
  @retval EFI_UNSUPPORTED       FiaMux is not supported
  @retval EFI_SUCCESS           FiaMux configuration has been read successfully
**/
EFI_STATUS
EFIAPI
MeFiaMuxConfigGet (
  OUT ME_FIA_CONFIG *pConfig,
  OUT ME_FIA_CONFIG *FiaMuxConfigFromMe,
  OUT UINT32        *pLanesAllowed,
  OUT BOOLEAN       *OverridesActive
  )
{
#if ME_SPS_SUPPORT
  EFI_STATUS                     Status;
  UINT32                         CommandSize;
  UINT32                         ResponseSize;
  VOID                           *Heci;
  ICC_MSG_GET_FIA_MUX_CONFIG_REQ *Req;
  ICC_MSG_GET_FIA_MUX_CONFIG_RSP *Rsp;
  UINT8                          HsioLanes;
  UINT8                          Index;
  UINT8                          LaneIndex;
  SATA_RECORD                    *Sata;
  PCIE_ROOT_PORT_RECORD          *PcieRootPort;
  UINT8                          LowLane;
  UINT8                          HiLane;
  UINT8                          PcieRootPortLanes;
  UINT8                          SataLanes;
  UINT8                          *PcieRootPortRecordsCount;
  UINT8                          *SataRecordsCount;
  UINT8                          *FiaMuxRecordsCount;

  DEBUG ((DEBUG_INFO, "[SPS] (ICC) %a\n", __FUNCTION__));

  if (OverridesActive != NULL) {
    *OverridesActive = FALSE;
  }

  if (pConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: (ICC) %a: both pConfig cannot be NULL\n", __FUNCTION__));
    ASSERT (pConfig != NULL);
    return EFI_INVALID_PARAMETER;
  }

  Heci = GetHeci (NULL);
  if (Heci == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: (ICC) %a: Cannot locate HECI\n", __FUNCTION__));
    ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
    return EFI_DEVICE_ERROR;
  }

  HsioLanes = GetHsioLanes (&LowLane, &HiLane);
  PcieRootPortLanes = HsioLanes / 2;
  ResponseSize =  sizeof (ICC_HEADER) +
                  (sizeof (UINT8) * 4) +
                  (sizeof (FIA_MUX_RECORD) * HsioLanes) +
                  (sizeof (SATA_RECORD) * HsioLanes) +
                  (sizeof (PCIE_ROOT_PORT_RECORD) * HsioLanes);
  ResponseSize += AddFiler (ResponseSize);
  CommandSize = sizeof (ICC_MSG_GET_FIA_MUX_CONFIG_REQ);

  Rsp = (ICC_MSG_GET_FIA_MUX_CONFIG_RSP*) AllocateZeroPool (ResponseSize);
  if (Rsp == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Can't allocate memory in %a ()\n", __FUNCTION__));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return EFI_OUT_OF_RESOURCES;
  }
  Req = (ICC_MSG_GET_FIA_MUX_CONFIG_REQ*) Rsp;

  Req->Header.ApiVersion   = ICC_API_VERSION;
  Req->Header.IccCommand   = ICC_GET_FIA_MUX_CONFIG_CMD;
  Req->Header.BufferLength = CommandSize - sizeof (ICC_HEADER);
  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_ICC_MESSAGE_ADDR,
             (UINT32*) Req,
             CommandSize,
             (UINT32*) Rsp,
             &ResponseSize,
             NULL,
             NULL
             );
  if (EFI_ERROR (Status) ||
     (Rsp->Header.IccCommand != ICC_GET_FIA_MUX_CONFIG_CMD)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: (ICC) %a: Message failed! EFI_STATUS = %r\n", __FUNCTION__, Status));
    Status = EFI_PROTOCOL_ERROR;
    goto MeFiaMuxConfigGetExit;
  }

  if (ResponseSize <= sizeof (ICC_HEADER)) {
    Status = EFI_PROTOCOL_ERROR;
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: (ICC) %a: Message failed! Response is too short (%d bytes).\n", __FUNCTION__, ResponseSize));
    goto MeFiaMuxConfigGetExit;
  }

  switch (Rsp->Header.IccResponse) {
    case ICC_STATUS_SUCCESS:
      DEBUG ((DEBUG_INFO, "[SPS] (ICC) %a: FIA Mux configuration retrieved, number of lanes allowed: (%d)\n", __FUNCTION__, Rsp->LanesAllowed));
      Status = EFI_SUCCESS;
      break;

    case ICC_STATUS_FIA_MUX_NO_CONFIG_FOUND:
      DEBUG ((DEBUG_INFO, "[SPS] (ICC) %a: no FIA Mux configuration in ME Flash\n", __FUNCTION__));
      DEBUG ((DEBUG_INFO, "[SPS] (ICC) %a: Softstraps Configuration will be used\n", __FUNCTION__));
      Status = EFI_PROTOCOL_ERROR;
      break;

    default:
      DEBUG ((DEBUG_ERROR, "[SPS] ERROR: (ICC) %a: Error in response! ICC Status = 0x%x\n", __FUNCTION__, Rsp->Header.IccResponse));
      Status = EFI_PROTOCOL_ERROR;
      goto MeFiaMuxConfigGetExit;
  }

  FiaMuxRecordsCount = &(Rsp->FiaMuxRecordsCount);
  if (*FiaMuxRecordsCount > PCH_MAX_HSIO_LANES) {
    DEBUG ((
      DEBUG_ERROR,
      "[SPS] ERROR: (ICC) %a: Too many lanes passed (%d), max expected : %d\n",
      __FUNCTION__,
      *FiaMuxRecordsCount,
      PCH_MAX_HSIO_LANES
      ));
    Status = EFI_DEVICE_ERROR;
    goto MeFiaMuxConfigGetExit;
  }
  // For SIMICS be more tolerant
  if ((Rsp->LanesAllowed == 0) && IsSimicsEnvironment ()) {
    Rsp->LanesAllowed = *FiaMuxRecordsCount;
    DEBUG ((
      DEBUG_WARN,
      "[SPS] Warning: (ICC) %a: Override LanesAllowed to %d under SIMICS\n",
      __FUNCTION__,
      Rsp->LanesAllowed
      ));
  }

  if (Rsp->LanesAllowed == 0) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: (ICC) %a: ME responded with LanesAllowed = 0!\n", __FUNCTION__));
    Status = EFI_PROTOCOL_ERROR;
    goto MeFiaMuxConfigGetExit;
  }

  if (pLanesAllowed != NULL) {
    DEBUG ((DEBUG_INFO, "[SPS] (ICC) %a: Received  number of Lanes allowed = 0x%x\n", __FUNCTION__, Rsp->LanesAllowed));
    *pLanesAllowed = Rsp->LanesAllowed;
  }
  pConfig->FiaMuxRecordsCount = *FiaMuxRecordsCount;

  SataLanes = 0;
  for (Index = 0; Index < *FiaMuxRecordsCount; Index++) {
    LaneIndex = Rsp->FiaMux[Index].FiaMuxHardwareIndex;
    // Skip configuration hole
    if (((LaneIndex > LowLane) && (LaneIndex < HiLane)) ||
         (LaneIndex >= HsioLanes)) {
      continue;
    }
    // skip overrides
    if (pConfig->FiaMuxLaneOverride[LaneIndex] == 1) {
      if ((OverridesActive != NULL) &&
          (pConfig->FiaMux[LaneIndex].FiaMuxSelection != Rsp->FiaMux[Index].FiaMuxSelection)) {
        *OverridesActive = TRUE;
      }
      continue;
    }
    pConfig->FiaMux[LaneIndex].FiaMuxSelection =     Rsp->FiaMux[Index].FiaMuxSelection;
    pConfig->FiaMux[LaneIndex].FiaMuxHardwareIndex = LaneIndex;
  }

  if (FiaMuxConfigFromMe != NULL) {
    FiaMuxConfigFromMe->FiaMuxRecordsCount = *FiaMuxRecordsCount;
    CopyMem (FiaMuxConfigFromMe->FiaMux, Rsp->FiaMux, *FiaMuxRecordsCount * sizeof (FIA_MUX_RECORD));
  }

  Sata = (SATA_RECORD*) (((UINT8*) (&Rsp->FiaMux)) + (*FiaMuxRecordsCount * sizeof (FIA_MUX_RECORD)));
  SataRecordsCount = &(Rsp->SataRecordsCount);
  pConfig->SataRecordsCount = *SataRecordsCount;
  for (Index = 0; Index < *SataRecordsCount; Index++) {
    LaneIndex = Sata[Index].SataHardwareIndex;
    // Skip configuration hole
    if (((LaneIndex > LowLane) && (LaneIndex < HiLane)) ||
         (LaneIndex >= HsioLanes)) {
      continue;
    }
    // skip overrides
    if (pConfig->FiaMuxLaneOverride[LaneIndex] == 1) {
      if ((OverridesActive != NULL) &&
          (pConfig->Sata[LaneIndex].SataSelection != Sata[Index].SataSelection)) {
        *OverridesActive = TRUE;
      }
      continue;
    }
    pConfig->Sata[LaneIndex].SataSelection =     Sata[Index].SataSelection;
    pConfig->Sata[LaneIndex].SataHardwareIndex = LaneIndex;
    if (pConfig->FiaMux[LaneIndex].FiaMuxSelection == FIA_MUX_LANE_SATA_OWNER) {
      // Update SataLanes for SATA only
      SataLanes++;
    }
  }

  if (FiaMuxConfigFromMe != NULL) {
    FiaMuxConfigFromMe->SataRecordsCount = *SataRecordsCount;
    CopyMem (FiaMuxConfigFromMe->Sata, Sata, *SataRecordsCount * sizeof (SATA_RECORD));
  }

  PcieRootPort = (PCIE_ROOT_PORT_RECORD*) (((UINT8*) Sata) + (*SataRecordsCount * sizeof (SATA_RECORD)));
  PcieRootPortRecordsCount = &(Rsp->PcieRootPortRecordsCount);
  pConfig->PcieRootPortRecordsCount = *PcieRootPortRecordsCount;
  for (Index = 0; Index < *PcieRootPortRecordsCount; Index++) {
    LaneIndex = PcieRootPort[Index].PcieRootPortHardwareIndex;
    // Skip configuration hole
    if (((LaneIndex > LowLane) && (LaneIndex < HiLane)) ||
         (LaneIndex >= HsioLanes)) {
      continue;
    }
    // skip overrides
    if (pConfig->FiaMuxLaneOverride[LaneIndex] == 1) {
      if ((OverridesActive != NULL) &&
         ((pConfig->PcieRootPort[LaneIndex].PcieRootPortEnable != PcieRootPort[Index].PcieRootPortEnable) ||
         (pConfig->PcieRootPort[LaneIndex].PcieRootPortLinkWidth != PcieRootPort[Index].PcieRootPortLinkWidth))) {
        *OverridesActive = TRUE;
      }
      continue;
    }
    pConfig->PcieRootPort[LaneIndex].PcieRootPortEnable =        PcieRootPort[Index].PcieRootPortEnable;
    pConfig->PcieRootPort[LaneIndex].PcieRootPortLinkWidth =     PcieRootPort[Index].PcieRootPortLinkWidth;
    pConfig->PcieRootPort[LaneIndex].PcieRootPortHardwareIndex = LaneIndex;
  }

  if (FiaMuxConfigFromMe != NULL) {
    FiaMuxConfigFromMe->PcieRootPortRecordsCount = *PcieRootPortRecordsCount;
    CopyMem (FiaMuxConfigFromMe->PcieRootPort, PcieRootPort, *PcieRootPortRecordsCount * sizeof (PCIE_ROOT_PORT_RECORD));
  }

  if (!EFI_ERROR (Status)) {
    PrintFiaMuxConfig (
      *FiaMuxRecordsCount,
      Rsp->FiaMux,
      *SataRecordsCount,
      SataLanes,
      Sata,
      *PcieRootPortRecordsCount,
      PcieRootPort
      );
    if (OverridesActive != NULL) {
      DEBUG ((DEBUG_INFO, "[SPS] FIA Mux OverridesActive = %d\n", *OverridesActive));
    }
  }

MeFiaMuxConfigGetExit:
  FreePool (Rsp);
  DEBUG ((DEBUG_INFO, "[SPS] (ICC) %a: End - %r\n", __FUNCTION__, Status));

  return Status;
#else // ME_SPS_SUPPORT
  return EFI_UNSUPPORTED;
#endif // ME_SPS_SUPPORT
} // MeFiaMuxConfigGet ()
