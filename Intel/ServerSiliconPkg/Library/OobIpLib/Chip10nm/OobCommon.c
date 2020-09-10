/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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
#include <RcRegs.h>
#include <Library/DebugLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/OobIpLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/KtiApi.h>
#include <IioRegs.h>


/**
  OOB:  Get OOB bus number
  @param SocId         - Socket ID
  @param BusData0      - Store Bus Data 0
  @param BusData1      - Store Bus Data 1

  @retval                - None

**/
VOID
EFIAPI
OobGetRootBusNo (
  UINT8  SocId,
  UINT32 *BusData0,
  UINT32 *BusData1
  )
{
  if (BusData0 == NULL || BusData1 == NULL) {
    ASSERT (FALSE);
    return;
  }
  *BusData0 = UsraCsrRead (SocId, 0, CPUBUSNO_MSM_OOB_REG);
  *BusData1 = UsraCsrRead (SocId, 0, CPUBUSNO1_MSM_OOB_REG);
}


/**
  OOB: Enable PECI Trusted mode.

  @param[in] IioIndex - IIO instance (typically socket)
**/
VOID
EFIAPI
OobSetPeciTrusted (
  UINT8 IioIndex
  )
{
  MSM_PCI_PECI_BIOS_MSM_OOB_STRUCT PeciBios;

  PeciBios.Data = UsraCsrRead (IioIndex, 0, MSM_PCI_PECI_BIOS_MSM_OOB_REG);
  //
  // Set Trust Mode to 1 to allow PECI requests.
  //
  PeciBios.Bits.peci_trustmode = 1;
  S3UsraCsrWrite (IioIndex, 0, MSM_PCI_PECI_BIOS_MSM_OOB_REG, PeciBios.Data);
  //
  // Verify the register is available and write was effective.
  //
  PeciBios.Data = UsraCsrRead (IioIndex, 0, MSM_PCI_PECI_BIOS_MSM_OOB_REG);
  if (PeciBios.Data == (UINT32)-1 || !PeciBios.Bits.peci_trustmode) {

    DEBUG ((DEBUG_ERROR, "[OOB] ERROR: %a failed on socket %d (%08X)\n",
            __FUNCTION__, IioIndex, PeciBios.Data));
  }
}


/**
  OOB: Inform OOB-MSM that PCI hierarchy has been configured.

  @param[in] IioIndex - IIO instance (typically socket)
**/
VOID
EFIAPI
OobSetPciCfgCmpl (
  UINT8 IioIndex
  )
{
  MSM_PCI_PECI_BIOS_MSM_OOB_STRUCT PeciBios;

  PeciBios.Data = UsraCsrRead (IioIndex, 0, MSM_PCI_PECI_BIOS_MSM_OOB_REG);
  //
  // Set to 1 after PCI enumeration and resource allocation.
  //
  PeciBios.Bits.pci_config_compl = 1;
  S3UsraCsrWrite (IioIndex, 0, MSM_PCI_PECI_BIOS_MSM_OOB_REG, PeciBios.Data);
  //
  // Verify the register is available and write was effective.
  //
  PeciBios.Data = UsraCsrRead (IioIndex, 0, MSM_PCI_PECI_BIOS_MSM_OOB_REG);
  if (PeciBios.Data == (UINT32)-1 || !PeciBios.Bits.pci_config_compl) {
    //
    // PECI Trusted should be be enabled before PCI complete, so checking both.
    //
    DEBUG ((DEBUG_ERROR, "[OOB] ERROR: %a failed on socket %d (%08X)\n",
            __FUNCTION__, IioIndex, PeciBios.Data));
  }
}


/**
  OOB: Enable PECI Downstream

  This function sets PECI Downstream access enabled for WrPCIConfig, on PECI
  and other agents.

  @param IioIndex      - IIO Index (Socket)

  @param is_enabled       - True if bit should be turned on, False if not

  @retval              - None

**/
VOID
EFIAPI
OobSetPeciDownstreamEnabled (
  UINT8 IioIndex,
  BOOLEAN is_enabled
  )
{
  MSM_UCODE_MSR_64_MSM_OOB_STRUCT OobMsm;

  OobMsm.Data = UsraCsrRead (IioIndex, 0, MSM_UCODE_MSR_64_MSM_OOB_REG);
  //Set all agents enabled
  OobMsm.Bits.peci = (is_enabled == TRUE) ? 1 : 0;
  OobMsm.Bits.dwn_stream_en = (is_enabled == TRUE) ? 1 : 0;
  S3UsraCsrWrite (IioIndex, 0, MSM_UCODE_MSR_64_MSM_OOB_REG, OobMsm.Data);
}

/**
  OOB: Set EID value

  @param SocId        - Socket ID
  @param MctpEid      - MctpEid value

  @retval             - None

**/
VOID
EFIAPI
OobSetEid (
  UINT8 SocId,
  UINT8 MctpEid
  )
{
  MCTP_EID_CTRL_MSM_OOB_STRUCT MctpEidCtrl;

  MctpEidCtrl.Data = UsraCsrRead (SocId, 0, MCTP_EID_CTRL_MSM_OOB_REG);
  MctpEidCtrl.Bits.msm_eid = MctpEid;
  UsraCsrWrite (SocId, 0, MCTP_EID_CTRL_MSM_OOB_REG, MctpEidCtrl.Data);
}

/**
  OOB: Configure Bus Number root

  @param SocId        - Socket ID
  @param RootBus[5]   - Bus Base values
  @param BusBase      - Bus Base value
  @param BusLimit     - Bus Limit value

  @retval             - None

**/
VOID
EFIAPI
OobSetBusNoRoot (
  UINT8 SocId,
  UINT8 BusBase,
  UINT8 BusLimit,
  UINT8 RootBus[6]
  )
{
  CPUBUSNO_MSM_OOB_STRUCT   MsmCpuBusNo0;
  CPUBUSNO1_MSM_OOB_STRUCT  MsmCpuBusNo1;
  CPUBUSNO2_MSM_OOB_STRUCT  MsmCpuBusNo2;

  MsmCpuBusNo0.Data = 0;
  MsmCpuBusNo1.Data = 0;
  MsmCpuBusNo2.Data = 0;

  MsmCpuBusNo2.Bits.rootbusu0 = BusBase;
  MsmCpuBusNo2.Bits.rootbusu1 = BusLimit;
  MsmCpuBusNo2.Bits.rootbuss9 = BUS_NUM_OOBMSMBUS;
  MsmCpuBusNo0.Bits.rootbuss0 = RootBus[0];
  MsmCpuBusNo0.Bits.rootbuss1 = RootBus[1];
  MsmCpuBusNo0.Bits.rootbuss2 = RootBus[2];
  MsmCpuBusNo0.Bits.rootbuss3 = RootBus[3];
  MsmCpuBusNo1.Bits.rootbuss4 = RootBus[4];
  MsmCpuBusNo1.Bits.rootbuss5 = RootBus[5];

  //
  // PCIE_1 root port present at Ms2Iosf stack5 in ICX-D HCC SKU
  // and need to  program PCIE_1 stack bus no in RootBusS8 register
  //
  if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    if (GetChopType (SocId) == TypeHcc) {
      MsmCpuBusNo2.Bits.rootbuss8 = RootBus[5];
    }
  }

  UsraCsrWrite (SocId, 0, CPUBUSNO_MSM_OOB_REG, MsmCpuBusNo0.Data);
  UsraCsrWrite (SocId, 0, CPUBUSNO1_MSM_OOB_REG, MsmCpuBusNo1.Data);
  UsraCsrWrite (SocId, 0, CPUBUSNO2_MSM_OOB_REG, MsmCpuBusNo2.Data);
}

/**
  This function configures Bus Number root for Virtual RootPort (RootbusSn+1)
  and sets this bus as configured

  @param IioIndex     - IIO index (socket number)
  @param Bus          - Bus Number
  @param Device       - Device Number

**/
VOID
OobSetBusNoRootOfVrp (
  IN UINT8                        IioIndex,
  IN UINT8                        Bus,
  IN UINT8                        Device
  )
{
  CPUBUSNO1_MSM_OOB_STRUCT        MsmCpuBusNo1;
  CPUBUSNO_VALID_MSM_OOB_STRUCT   MsmCpuBusEnable;

  MsmCpuBusNo1.Data = UsraCsrRead (IioIndex, 0, CPUBUSNO1_MSM_OOB_REG);
  MsmCpuBusEnable.Data = UsraCsrRead (IioIndex, 0, CPUBUSNO_VALID_MSM_OOB_REG);

  switch (Device) {
    case IioCpmDevice:
      MsmCpuBusNo1.Bits.rootbuss5 = Bus;
      MsmCpuBusEnable.Bits.en_rootbuss5 = 1;
      break;
    case IioCpkDevice:
      MsmCpuBusNo1.Bits.rootbuss6 = Bus;
      MsmCpuBusEnable.Bits.en_rootbuss6 = 1;
      break;
    case IioHqmDevice:
      MsmCpuBusNo1.Bits.rootbuss7 = Bus;
      MsmCpuBusEnable.Bits.en_rootbuss7 = 1;
      break;
    default:
      break;
  }

  UsraCsrWrite (IioIndex, 0, CPUBUSNO1_MSM_OOB_REG, MsmCpuBusNo1.Data);
  UsraCsrWrite (IioIndex, 0, CPUBUSNO_VALID_MSM_OOB_REG, MsmCpuBusEnable.Data);
}

/**
  OOB: Enable Cpu Bus

  @param SocId             - Socket ID
  @param CpuBusEnMask      - CPU bus enable mask

  @retval                  - None

**/
VOID
EFIAPI
OobSetCpuBusEnable (
  UINT8 SocId,
  UINT32 CpuBusEnMask
  )
{
  CPUBUSNO_VALID_MSM_OOB_STRUCT MsmCpuBusEnable;

  MsmCpuBusEnable.Data = 0;
  MsmCpuBusEnable.Bits.en_rootbusu0 = 1;
  MsmCpuBusEnable.Bits.en_rootbusu1 = 1;
  MsmCpuBusEnable.Bits.en_rootbuss9 = 1;

  //
  // PCIE_1 root port present at Ms2Iosf stack5 in ICX-D HCC SKU
  // and need to enable RootBusS8 bit in CPUBUSNO_VALID register.
  //
  if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    if (GetChopType (SocId) == TypeHcc) {
      MsmCpuBusEnable.Bits.en_rootbuss8 = 1;
    }
  }
  MsmCpuBusEnable.Data |= CpuBusEnMask;

  UsraCsrWrite (SocId, 0, CPUBUSNO_VALID_MSM_OOB_REG, MsmCpuBusEnable.Data);
}

/**
  OOB: Set Socket range

  @param SocId        - Socket ID
  @param BusBase      - Bus Base value
  @param BusLimit     - Bus Limit value
  @param PciSegment   - Pci Segment value

  @retval             - None

**/
VOID
EFIAPI
OobSetSocketBusRange (
  UINT8 SocId,
  UINT8 BusBase,
  UINT8 BusLimit,
  UINT8 PciSegment
  )
{
  SOCKET_BUS_RANGE_MSM_OOB_STRUCT MsmSocketBusRange;

  MsmSocketBusRange.Data = 0;
  MsmSocketBusRange.Bits.start_bus = BusBase;
  MsmSocketBusRange.Bits.end_bus = BusLimit;
  MsmSocketBusRange.Bits.segment_id = PciSegment;
  MsmSocketBusRange.Bits.valid = 1;

  UsraCsrWrite (SocId, 0, SOCKET_BUS_RANGE_MSM_OOB_REG, MsmSocketBusRange.Data);
}

/**
  OOB: Set NvDimm mailbox registers for OOBMSM.

  This function programs the address offset and dimm configuration into the OOB NvDimm mailbox registers
  for OOBMSM MCTP proxy to support MCTP over DDRT-2 feature.

  @param[in] ScktId               Socket index.
  @param[in] OobNvdimmMailbox     Data information of NvDimm mailbox address.

  @retval EFI_SUCCESS             OOB sets the NvDimm mailbox registers successfully.
  @retval EFI_INVALID_PARAMETER   The input of this function is invalid.

**/
EFI_STATUS
EFIAPI
OobSetNvDimmMailBox (
  IN UINT8                ScktId,
  IN OOB_NVDIMM_MAILBOX   *OobNvdimmMailbox
  )
{
  return EFI_SUCCESS;
}

/**
  OOB: Disable ManualTrigger of CRASHLOG_CTRL_MSM_OOB_REG when the DisableCrashlog is 0
  Wave1&2 don't support it.

  @param SocId             - Socket ID

  @retval                  - None

**/
VOID
EFIAPI
OobDisableManualTrigger (
  UINT8 SocId
  )
{
  return;
}
