/** @file
  ACPI Platform Driver VT-D Hooks

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2020 Intel Corporation. <BR>

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
// APTIOV_SERVER_OVERRIDE_RC_START : For filling Reserved Base Address and Limit for RMRR structure of DMAR table
#include <Token.h>
#define __PLATFORM_STATUS_CODES_INTERNAL_H__
// APTIOV_SERVER_OVERRIDE_RC_END : For filling Reserved Base Address and Limit for RMRR structure of DMAR table
#include "AcpiPlatform.h"
#include "AcpiPlatformHooks.h"
#include <Protocol/PciRootBridgeIo.h>
// APTIOV_SERVER_OVERRIDE_RC_START
#if defined AMIUSB_SUPPORT && AMIUSB_SUPPORT == 1
#if USB_DRIVER_MAJOR_VER >= 10
#undef EFI_ACPI_TABLE_VERSION_ALL //undefined here as it is defined from AmiDxeLib.h which is included in AmiUsbController.h
#endif
#include <Protocol/AmiUsbController.h>
#endif
// OEM VTD RMRR support
#if defined OemVtdRmrr_SUPPORT && OemVtdRmrr_SUPPORT == 1
#include <Protocol/OemVtdRmrrProtocol.h>
#endif
// APTIOV_SERVER_OVERRIDE_RC_END
#include <UncoreCommonIncludes.h>
#include <Library/IioApiLib.h>
#include <Library/KtiApi.h>
#include <IioSetupDefinitions.h>
#include <IioRegs.h>
#include <Library/AcpiPlatformHookLib.h>
#include <PchInfoHob.h>
// APTIOV_SERVER_OVERRIDE_RC_START: Replaced the usage of PciRootBridgeIo Read with IioPciExpressRead32 as the first RootBridge instance was directly used
#include <Library/IioAccessLib.h>
// APTIOV_SERVER_OVERRIDE_RC_END: Replaced the usage of PciRootBridgeIo Read with IioPciExpressRead32 as the first RootBridge instance was directly used

#include <Library/ReportDmarHookLib.h>

extern EFI_PLATFORM_INFO                    *mPlatformInfo;
extern BIOS_ACPI_PARAM                      *mAcpiParameter;
extern EFI_IIO_UDS_PROTOCOL                 *mIioUds2;
extern CPU_CSR_ACCESS_VAR                   *mCpuCsrAccessVarPtr;
extern SYSTEM_CONFIGURATION                 mSystemConfiguration;
extern SOCKET_IIO_CONFIGURATION             mSocketIioConfiguration;
extern SOCKET_PROCESSORCORE_CONFIGURATION   mSocketProcessorCoreConfiguration;
extern EFI_GUID                             mSystemConfigurationGuid;
extern BOOLEAN                              mX2ApicEnabled;
extern BOOLEAN                              Is14nmCpu;
// APTIOV_SERVER_OVERRIDE_RC_START : Adding RHSA structure in DMAR table
extern EFI_GUID gEfiSocketCommonRcVariableGuid;
// APTIOV_SERVER_OVERRIDE_RC_END : Adding RHSA structure in DMAR table


#define EFI_PCI_CAPABILITY_PTR              0x34
#define EFI_PCIE_CAPABILITY_BASE_OFFSET     0x100
#define EFI_PCIE_CAPABILITY_ID_ACS          0x000D
#define EFI_PCI_CAPABILITY_ID_PCIEXP        0x10
#define EFI_PCI_EXPRESS_CAPABILITY_REGISTER 0x02

#define ACS_CAPABILITY_REGISTER             0x04
#define ACS_SOURCE_VALIDATION               BIT0
#define ACS_P2P_REQUEST_REDIRECT            BIT2
#define ACS_P2P_COMPLETION_REDIRECT         BIT3
#define ACS_UPSTREAM_FORWARDING             BIT4

#define ACS_CONTROL_REGISTER                0x06
#define ACS_SOURCE_VALIDATION_ENABLE        BIT0
#define ACS_P2P_REQUEST_REDIRECT_ENABLE     BIT2
#define ACS_P2P_COMPLETION_REDIRECT_ENABLE  BIT3
#define ACS_UPSTREAM_FORWARDING_ENABLE      BIT4

#define R_VTD_GCMD_REG                      0x18
#define R_VTD_GSTS_REG                      0x1C
#define R_VTD_IQT_REG                       0x88
#define R_VTD_IQA_REG                       0x90
#define R_VTD_IRTA_REG                      0xB8

#define VTD_ISOCH_ENGINE_OFFSET             0x1000

//
// a flag to indicate if we should disable Vt-d for ACS WA
//
BOOLEAN                                     mDisableVtd = FALSE;

EFI_DMA_REMAP_PROTOCOL                      DmaRemapProt;
#define  VTD_SUPPORT_INSTANCE_FROM_THIS(a)  CR(a, EFI_VTD_SUPPORT_INSTANCE, DmaRemapProt, EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE)
// #define  DMAR_HOST_ADDRESS_WIDTH   46
#define  EFI_PCI_CAPABILITY_ID_ATS          0x0F

#define SEGMENT0                            0x00
#define MEM_BLK_COUNT                       0x140
#define INTRREMAP                           BIT3
#define MEMORY_SIZE                         (MaxIIO * NUMBER_PORTS_PER_SOCKET)

PCI_NODE  mPciPath0_1[]   = {
  {PCI_DEVICE_NUMBER_PCH_HDA, PCI_FUNCTION_NUMBER_PCH_HDA},
  {(UINT8) -1,  (UINT8) -1},
};

//
// IOAPIC1  - PCH IoApic
//

//
// IOAPIC2  - IIO IoApic
//
PCI_NODE  mPciPath1_2[]   = {
  {APIC_DEV_NUM, APIC_FUNC_NUM},
  {(UINT8) -1,  (UINT8) -1},
};

PCI_NODE  mPciPath2_0[]   = {
  {PCIE_PORT_0_DEV, PCIE_PORT_0_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_1[]   = {
  {PCIE_PORT_1A_DEV, PCIE_PORT_1A_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_2[]   = {
  {PCIE_PORT_1B_DEV, PCIE_PORT_1B_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_3[]   = {
  {PCIE_PORT_1C_DEV, PCIE_PORT_1C_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_4[]   = {
  {PCIE_PORT_1D_DEV, PCIE_PORT_1D_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_5[]   = {
  {PCIE_PORT_2A_DEV, PCIE_PORT_2A_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_6[]   = {
  {PCIE_PORT_2B_DEV, PCIE_PORT_2B_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_7[]   = {
  {PCIE_PORT_2C_DEV, PCIE_PORT_2C_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_8[]   = {
  {PCIE_PORT_2D_DEV, PCIE_PORT_2D_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_9[]   = {
  {PCIE_PORT_3A_DEV, PCIE_PORT_3A_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_10[]   = {
  {PCIE_PORT_3B_DEV, PCIE_PORT_3B_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_11[]   = {
  {PCIE_PORT_3C_DEV, PCIE_PORT_3C_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_12[]  = {
  {PCIE_PORT_3D_DEV, PCIE_PORT_3D_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_13[]  = {
  {PCIE_PORT_4A_DEV, PCIE_PORT_4A_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_14[]  = {
  {PCIE_PORT_4B_DEV, PCIE_PORT_4B_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_15[]  = {
  {PCIE_PORT_4C_DEV, PCIE_PORT_4C_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_16[]  = {
  {PCIE_PORT_4D_DEV, PCIE_PORT_4D_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_17[]  = {
  {PCIE_PORT_5A_DEV, PCIE_PORT_5A_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_18[]  = {
  {PCIE_PORT_5B_DEV, PCIE_PORT_5B_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_19[]  = {
  {PCIE_PORT_5C_DEV, PCIE_PORT_5C_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_20[]  = {
  {PCIE_PORT_5D_DEV, PCIE_PORT_5D_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};

PCI_NODE  mPciPath2_0_10nmD[] = {
    { PCIE_PORT_0_DEV_0, PCIE_PORT_0_FUNC_0 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_1_10nmD[] = {
    { PCIE_PORT_1A_DEV_S1, PCIE_PORT_1A_FUNC_S1 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_2_10nmD[] = {
    { PCIE_PORT_1B_DEV_S1, PCIE_PORT_1B_FUNC_S1 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_3_10nmD[] = {
    { PCIE_PORT_1C_DEV_S1, PCIE_PORT_1C_FUNC_S1 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_4_10nmD[] = {
    { PCIE_PORT_1D_DEV_S1, PCIE_PORT_1D_FUNC_S1 },
    { (UINT8)-1, (UINT8)-1 },
};

PCI_NODE  mPciPath2_5_10nmD[] = {
    { PCIE_PORT_2A_DEV_S1, PCIE_PORT_2A_FUNC_S1 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_6_10nmD[] = {
    { PCIE_PORT_2B_DEV_S1, PCIE_PORT_2B_FUNC_S1 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_7_10nmD[] = {
    { PCIE_PORT_2C_DEV_S1, PCIE_PORT_2C_FUNC_S1 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_8_10nmD[] = {
    { PCIE_PORT_2D_DEV_S1, PCIE_PORT_2D_FUNC_S1 },
    { (UINT8)-1, (UINT8)-1 },
};

PCI_NODE  mPciPath2_0_10nm[] = {
    { PCIE_PORT_0_DEV_0, PCIE_PORT_0_FUNC_0 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_1_10nm[] = {
    { PCIE_PORT_1A_DEV_1, PCIE_PORT_1A_FUNC_1 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_2_10nm[] = {
    { PCIE_PORT_1B_DEV_1, PCIE_PORT_1B_FUNC_1 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_3_10nm[] = {
    { PCIE_PORT_1C_DEV_1, PCIE_PORT_1C_FUNC_1 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_4_10nm[] = {
    { PCIE_PORT_1D_DEV_1, PCIE_PORT_1D_FUNC_1 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_5_10nm[] = {
    { PCIE_PORT_2A_DEV_2, PCIE_PORT_2A_FUNC_2 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_6_10nm[] = {
    { PCIE_PORT_2B_DEV_2, PCIE_PORT_2B_FUNC_2 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_7_10nm[] = {
    { PCIE_PORT_2C_DEV_2, PCIE_PORT_2C_FUNC_2 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_8_10nm[] = {
    { PCIE_PORT_2D_DEV_2, PCIE_PORT_2D_FUNC_2 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_9_10nm[] = {
    { PCIE_PORT_3A_DEV_3, PCIE_PORT_3A_FUNC_3 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_10_10nm[] = {
    { PCIE_PORT_3B_DEV_3, PCIE_PORT_3B_FUNC_3 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_11_10nm[] = {
    { PCIE_PORT_3C_DEV_3, PCIE_PORT_3C_FUNC_3 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_12_10nm[] = {
    { PCIE_PORT_3D_DEV_3, PCIE_PORT_3D_FUNC_3 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_13_10nm[] = {
    { PCIE_PORT_4A_DEV_4, PCIE_PORT_4A_FUNC_4 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_14_10nm[] = {
    { PCIE_PORT_4B_DEV_4, PCIE_PORT_4B_FUNC_4 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_15_10nm[] = {
    { PCIE_PORT_4C_DEV_4, PCIE_PORT_4C_FUNC_4 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_16_10nm[] = {
    { PCIE_PORT_4D_DEV_4, PCIE_PORT_4D_FUNC_4 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_17_10nm[] = {
    { PCIE_PORT_5A_DEV_5, PCIE_PORT_5A_FUNC_5 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_18_10nm[] = {
    { PCIE_PORT_5B_DEV_5, PCIE_PORT_5B_FUNC_5 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_19_10nm[] = {
    { PCIE_PORT_5C_DEV_5, PCIE_PORT_5C_FUNC_5 },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_20_10nm[] = {
    { PCIE_PORT_5D_DEV_5, PCIE_PORT_5D_FUNC_5 },
    { (UINT8)-1, (UINT8)-1 },
};

//
//  PciePath for ICXD keeping same table for both HCC and LCC
//  Maximum PCIe ports in LCC is 4 and in HCC is 8.
//
PCI_NODE  mPciPath2_0_Icxd[] = {
  { PCIE_PORT_0_DEV_0, PCIE_PORT_0_FUNC_0 },
  { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_1_Icxd[] = {
  { PCIE_PORT_1A_DEV_1, PCIE_PORT_1A_FUNC_1 },
  { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_2_Icxd[] = {
  { PCIE_PORT_1B_DEV_1, PCIE_PORT_1B_FUNC_1 },
  { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_3_Icxd[] = {
  { PCIE_PORT_1C_DEV_1, PCIE_PORT_1C_FUNC_1 },
  { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_4_Icxd[] = {
  { PCIE_PORT_1D_DEV_1, PCIE_PORT_1D_FUNC_1 },
  { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_5_Icxd[] = {
  { PCIE_PORT_5A_DEV_5, PCIE_PORT_5A_FUNC_5 },
  { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_6_Icxd[] = {
  { PCIE_PORT_5B_DEV_5, PCIE_PORT_5B_FUNC_5 },
  { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_7_Icxd[] = {
  { PCIE_PORT_5C_DEV_5, PCIE_PORT_5C_FUNC_5 },
  { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_8_Icxd[] = {
  { PCIE_PORT_5D_DEV_5, PCIE_PORT_5D_FUNC_5 },
  { (UINT8)-1, (UINT8)-1 },
};

//
// PciPath for SPR
//
PCI_NODE  mPciPath2_0_Spr[] = {
    { DMI_DEV_NUM_SPR, DMI_FUNC_NUM_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_1_Spr[] = {
    { PCIE_PORT_A_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_2_Spr[] = {
    { PCIE_PORT_B_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_3_Spr[] = {
    { PCIE_PORT_C_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_4_Spr[] = {
    { PCIE_PORT_D_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_5_Spr[] = {
    { PCIE_PORT_E_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_6_Spr[] = {
    { PCIE_PORT_F_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_7_Spr[] = {
    { PCIE_PORT_G_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_8_Spr[] = {
    { PCIE_PORT_H_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
// stack 2
PCI_NODE  mPciPath2_9_Spr[] = {
    { PCIE_PORT_A_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_10_Spr[] = {
    { PCIE_PORT_B_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_11_Spr[] = {
    { PCIE_PORT_C_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_12_Spr[] = {
    { PCIE_PORT_D_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_13_Spr[] = {
    { PCIE_PORT_E_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_14_Spr[] = {
    { PCIE_PORT_F_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_15_Spr[] = {
    { PCIE_PORT_G_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_16_Spr[] = {
    { PCIE_PORT_H_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
// stack 3
PCI_NODE  mPciPath2_17_Spr[] = {
    { PCIE_PORT_A_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_18_Spr[] = {
    { PCIE_PORT_B_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_19_Spr[] = {
    { PCIE_PORT_C_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_20_Spr[] = {
    { PCIE_PORT_D_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_21_Spr[] = {
    { PCIE_PORT_E_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_22_Spr[] = {
    { PCIE_PORT_F_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_23_Spr[] = {
    { PCIE_PORT_G_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_24_Spr[] = {
    { PCIE_PORT_H_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
// stack 4
PCI_NODE  mPciPath2_25_Spr[] = {
    { PCIE_PORT_A_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_26_Spr[] = {
    { PCIE_PORT_B_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_27_Spr[] = {
    { PCIE_PORT_C_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_28_Spr[] = {
    { PCIE_PORT_D_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_29_Spr[] = {
    { PCIE_PORT_E_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_30_Spr[] = {
    { PCIE_PORT_F_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_31_Spr[] = {
    { PCIE_PORT_G_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_32_Spr[] = {
    { PCIE_PORT_H_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
// stack 5
PCI_NODE  mPciPath2_33_Spr[] = {
    { PCIE_PORT_A_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_34_Spr[] = {
    { PCIE_PORT_B_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_35_Spr[] = {
    { PCIE_PORT_C_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_36_Spr[] = {
    { PCIE_PORT_D_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_37_Spr[] = {
    { PCIE_PORT_E_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_38_Spr[] = {
    { PCIE_PORT_F_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_39_Spr[] = {
    { PCIE_PORT_G_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
PCI_NODE  mPciPath2_40_Spr[] = {
    { PCIE_PORT_H_DEV_SPR, PCIE_PORT_FUNC_SPR },
    { (UINT8)-1, (UINT8)-1 },
};
DEVICE_SCOPE              mDevScopeDRHD[] = {
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ENDPOINT,    // Device type - HD Audio
    00,                                           // Enumeration ID
    DEFAULT_PCI_BUS_NUMBER_PCH,                           // Start Bus Number
    &mPciPath0_1[0]
  },
};

DEVICE_SCOPE              mDevScopeDRHD_INT[] = {
  {
    00,                                           // Device type
    00,                                           // Enumeration ID - ApicID
    00,                                           // Start Bus Number
    &mPciPath1_2[0]
  },
};

DEVICE_SCOPE              mDevScopeATSR[] = {
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port1
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_0[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port2
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_1[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port3
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_2[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port4
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_3[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port5
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_4[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port6
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_5[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port7
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_6[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port8
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_7[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port9
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_8[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port10
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_9[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port11
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_10[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port12
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_11[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port13
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_12[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port14
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_13[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port15
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_14[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port16
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_15[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port17
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_16[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port18
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_17[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port19
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_18[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port20
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_19[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port21
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_20[0]
  }
};

DEVICE_SCOPE              mDevScopeATSR10nmD[] = {
    {
        EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port1
        00,                                           // Enumeration ID
        DMI_BUS_NUM,
        &mPciPath2_0_10nmD[0]
    },
    {
        EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port2
        00,                                           // Enumeration ID
        DMI_BUS_NUM,
        &mPciPath2_1_10nmD[0]
    },
    {
        EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port3
        00,                                           // Enumeration ID
        DMI_BUS_NUM,
        &mPciPath2_2_10nmD[0]
    },
    {
        EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port4
        00,                                           // Enumeration ID
        DMI_BUS_NUM,
        &mPciPath2_3_10nmD[0]
    },
    {
        EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port5
        00,                                           // Enumeration ID
        DMI_BUS_NUM,
        &mPciPath2_4_10nmD[0]
    },

    {
        EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port6
        00,                                           // Enumeration ID
        DMI_BUS_NUM,
        &mPciPath2_5_10nmD[0]
    },
    {
        EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port7
        00,                                           // Enumeration ID
        DMI_BUS_NUM,
        &mPciPath2_6_10nmD[0]
    },
    {
        EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port8
        00,                                           // Enumeration ID
        DMI_BUS_NUM,
        &mPciPath2_7_10nmD[0]
    },
    {
        EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port9
        00,                                           // Enumeration ID
        DMI_BUS_NUM,
        &mPciPath2_8_10nmD[0]
    },
};

//
// DEVICE_SCOPE for ICXD_LCC
//
DEVICE_SCOPE              mDevScopeATSR_Icxd[] = {
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port1
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_0_Icxd[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port2
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_1_Icxd[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port3
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_2_Icxd[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port4
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_3_Icxd[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port5
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_4_Icxd[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port6
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_5_Icxd[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port7
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_6_Icxd[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port8
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_7_Icxd[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port9
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_8_Icxd[0]
  },
};



//
// DEVICE_SCOPE for SPR
//

DEVICE_SCOPE              mDevScopeATSR_Spr[] = {
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port1
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_0_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port2
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_1_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port3
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_2_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port4
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_3_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port5
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_4_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port6
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_5_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port7
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_6_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port8
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_7_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port9
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_8_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port10
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_9_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port11
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_10_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port12
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_11_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port13
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_12_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port14
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_13_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port15
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_14_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port16
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_15_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port17
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_16_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port18
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_17_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port19
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_18_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port20
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_19_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port21
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_20_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port22
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_21_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port23
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_22_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port24
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_23_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port25
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_24_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port26
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_25_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port27
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_26_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port28
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_27_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port29
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_28_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port30
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_29_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port31
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_30_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port32
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_31_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port33
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_32_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port34
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_33_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port35
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_34_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port36
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_35_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port37
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_36_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port38
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_37_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port39
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_38_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port40
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_39_Spr[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port41
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_40_Spr[0]
  }
};

DEVICE_SCOPE              mDevScopeATSR10nm[] = {
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port1
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_0_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port2
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_1_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port3
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_2_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port4
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_3_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port5
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_4_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port6
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_5_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port7
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_6_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port8
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_7_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port9
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_8_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port10
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_9_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port11
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_10_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port12
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_11_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port13
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_12_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port14
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_13_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port15
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
        &mPciPath2_14_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port16
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_15_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port17
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_16_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port18
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_17_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port19
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_18_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port20
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_19_10nm[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port21
    00,                                           // Enumeration ID
    DMI_BUS_NUM,
    &mPciPath2_20_10nm[0]
  }
};

DMAR_DRHD                 mDrhd = {
  DRHD_SIGNATURE,
  00,                                             // Flags
  SEGMENT0,                                       // Segment number
  00,                                             // Base Address
  00,                                             // Number of dev scope structures
  &mDevScopeDRHD[0]
};

DMAR_DRHD                 mDrhdIsoc = {
  DRHD_SIGNATURE,
  00,                                             // Flags
  SEGMENT0,                                       // Segment number
  00,                                             // Base Address
  00,                                             // Number of dev scope structures
  &mDevScopeDRHD[0]
};

DMAR_ATSR                 mAtsr = {
  ATSR_SIGNATURE,
  SEGMENT0,                                       // Segment number
  (UINT8) EFI_ACPI_DMAR_ATSR_FLAGS_ALL_PORTS_CLEAR,
  NUMBER_PORTS_PER_SOCKET-1,
  00,
  &mDevScopeATSR[0]
};

DMAR_ATSR                 mAtsr10nmD = {
    ATSR_SIGNATURE,
    SEGMENT0,                                       // Segment number
    (UINT8)EFI_ACPI_DMAR_ATSR_FLAGS_ALL_PORTS_CLEAR,
    NUMBER_PORTS_PER_SOCKET - 1,
    00,
    &mDevScopeATSR10nmD[0]
};

DMAR_ATSR                 mAtsrIcxd = {
  ATSR_SIGNATURE,
  SEGMENT0,                                       // Segment number
  (UINT8)EFI_ACPI_DMAR_ATSR_FLAGS_ALL_PORTS_CLEAR,
  NUMBER_PORTS_PER_SOCKET - 1,
  00,
  &mDevScopeATSR_Icxd[0]
};

DMAR_ATSR                 mAtsr10nm = {
    ATSR_SIGNATURE,
    SEGMENT0,                                       // Segment number
    (UINT8)EFI_ACPI_DMAR_ATSR_FLAGS_ALL_PORTS_CLEAR,
    NUMBER_PORTS_PER_SOCKET - 1,
    00,
    &mDevScopeATSR10nm[0]
};


DMAR_ATSR                 mAtsr_Spr = {
    ATSR_SIGNATURE,
    SEGMENT0,                                       // Segment number
    (UINT8)EFI_ACPI_DMAR_ATSR_FLAGS_ALL_PORTS_CLEAR,
    NUMBER_PORTS_PER_SOCKET - 1,
    00,
    &mDevScopeATSR_Spr[0]
};

PCI_NODE     mPciPath[] = {
  { 00,      00},
  { (UINT8)-1,   (UINT8)-1},
};

UINT8  IoApicID[] = { PCH_IOAPIC_ID,   //PCH
   PC00_IOAPIC_ID, PC01_IOAPIC_ID, PC02_IOAPIC_ID, PC03_IOAPIC_ID, PC04_IOAPIC_ID, PC05_IOAPIC_ID,  //Socket0
   PC06_IOAPIC_ID, PC07_IOAPIC_ID, PC08_IOAPIC_ID, PC09_IOAPIC_ID, PC10_IOAPIC_ID, PC11_IOAPIC_ID,  //Socket1
   PC12_IOAPIC_ID, PC13_IOAPIC_ID, PC14_IOAPIC_ID, PC15_IOAPIC_ID, PC16_IOAPIC_ID, PC17_IOAPIC_ID,  //Socket2
   PC18_IOAPIC_ID, PC19_IOAPIC_ID, PC20_IOAPIC_ID, PC21_IOAPIC_ID, PC22_IOAPIC_ID, PC23_IOAPIC_ID,  //Socket3
   PC24_IOAPIC_ID, PC25_IOAPIC_ID, PC26_IOAPIC_ID, PC27_IOAPIC_ID, PC28_IOAPIC_ID, PC29_IOAPIC_ID,  //Socket4
   PC30_IOAPIC_ID, PC31_IOAPIC_ID, PC32_IOAPIC_ID, PC33_IOAPIC_ID, PC34_IOAPIC_ID, PC35_IOAPIC_ID,  //Socket5
   PC36_IOAPIC_ID, PC37_IOAPIC_ID, PC38_IOAPIC_ID, PC39_IOAPIC_ID, PC40_IOAPIC_ID, PC41_IOAPIC_ID,  //Socket6
   PC42_IOAPIC_ID, PC43_IOAPIC_ID, PC44_IOAPIC_ID, PC45_IOAPIC_ID, PC46_IOAPIC_ID, PC47_IOAPIC_ID,  //Socket7
};

PCI_NODE     mPciPath7[] = {
  { PCI_DEVICE_NUMBER_PCH_XHCI,      PCI_FUNCTION_NUMBER_PCH_XHCI  },
  { (UINT8)-1,   (UINT8)-1},
};
DEVICE_SCOPE DevScopeRmrr[] = {
  {
    1,                                  // RMRR dev Scope - XHCI
    0,                                  // Enumeration ID
    0,                                  // Start Bus Number
    &mPciPath7[0]
  },
};

DMAR_RMRR    mRmrr = {
  RMRR_SIGNATURE,                       // Signature
  SEGMENT0,                             // Segment number
  ' ',                                  // Reserved Memory RegionBase Address
  ' ',                                  // Reserved Memory RegionLimit Address
  ' ',                                  // Number of Dev Scope structures
  &DevScopeRmrr[0]
};

typedef struct {
    UINT8   aBuf[32];
} MEM_BLK;

DMAR_RHSA                 mRhsa;

typedef struct {
    UINT8   Bus;
    UINT8   Dev;
    UINT8   Func;
} DMAR_DEVICE;

EFI_STATUS
LocateCapRegBlock(
  IN     EFI_PCI_IO_PROTOCOL  *PciIo,
  IN     UINT8                CapID,
  OUT    UINT8                *PciExpressOffset,
  OUT    UINT8                *NextRegBlock
  );

EFI_STATUS
LocatePciExpressCapRegBlock (
  IN     EFI_PCI_IO_PROTOCOL  *PciIo,
  IN     UINT16               CapID,
  OUT    UINT32               *Offset,
  OUT    UINT32               *NextRegBlock
);

DMAR_DRHD                 mDrhd;
DMAR_RHSA                 mRhsa;

DMAR_ATSR* GetDmarAtsrTablePointer (
  VOID
  )
{
  DMAR_ATSR* pAtsr = NULL;

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    pAtsr = &mAtsr_Spr;
  } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    pAtsr = &mAtsrIcxd;
  } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    pAtsr = &mAtsr10nmD;
  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    pAtsr = &mAtsr10nm;
  } else if (Is14nmCpu) {
    pAtsr = &mAtsr;
  }

  return pAtsr;
}


/**
  Enable VT-d interrupt remapping.

  This function should be called late at ReadyToBoot event. If called in AcpiVtdTablesInstall()
  would hang in CpuDeadLoop() because of timeout when waiting for invalidation commands complete.
**/
VOID
AcpiVtdIntRemappingEnable (
  VOID
  )
{
  EFI_STATUS                  Status;
  UINT64                      *xApicAddr;
  UINT64                      *IRTA;
  UINT64                      *Addr;
  UINT64                      Value=0;
  UINT16                      IRTECount;
  UINT16                      Count;
  UINT64                      IRTEValue;
  UINT8                       RemapEng;
  UINT8                       RemapEngCount;
  EFI_CPUID_REGISTER          CpuidRegisters;
  UINT32                      VtdBarAddress;
  UINT8                       Stack;
  EFI_VTD_SUPPORT_INSTANCE   *DmarPrivateData;
  EFI_DMA_REMAP_PROTOCOL     *DmaRemap = NULL;

  static volatile UINT64      TempQWord[MaxIIO] = {0};

  Status = gBS->LocateProtocol (&gEfiDmaRemapProtocolGuid, NULL, &DmaRemap);
  if (EFI_ERROR(Status) || !DmaRemap->VTdSupport || !DmaRemap->InterruptRemap) {

    DEBUG ((DEBUG_INFO, "[VTD] %a disabled\n",
            (DmaRemap != NULL && DmaRemap->VTdSupport) ? "Interrupt remapping" : "Virtualization Technology for Directed I/O"));
    return;
  }
  ASSERT (mIioUds2);

  IRTEValue = 00;
  RemapEng = 0;
  RemapEngCount = mIioUds2->IioUdsPtr->PlatformData.numofIIO;
  DmarPrivateData = VTD_SUPPORT_INSTANCE_FROM_THIS (DmaRemap);

  if (RemapEngCount > NELEMENTS (TempQWord)) {
    DEBUG ((DEBUG_ERROR, "[ACPI](DMAR) ERROR: Number of IIO exceed internal table (%d > %d)\n", RemapEngCount, NELEMENTS (TempQWord)));
    RemapEngCount = NELEMENTS (TempQWord);
  }

  //
  // Xapic tables update
  //
  IRTECount = 16 * 24;    // Total 24 IRTE entries with 128 bits each.
  // Allocate 4K alligned space for IRTE entries  Added extra space of 500 bytes.
  Status = gBS->AllocatePool (EfiACPIReclaimMemory, IRTECount + 0x1500, &xApicAddr);
  ASSERT_EFI_ERROR (Status);

  // Allocate IRT - Allocate zero-initialized, 4KB aligned, 4KB memory for interrupt-remap-table and mark this memory as "ACPI Reclaim Memory"
  xApicAddr = (UINT64 *)((UINT64)xApicAddr & (~0xFFF));
  ZeroMem (xApicAddr, IRTECount +0x1500);

  // 1. Program IRTE - Initialize the interrupt-remap-table as follows: (this table will be shared by all VT-d units)
  for (Count = 0; Count < 24; Count++)  {

    IRTEValue = 00;
    if (Count == 0) {
      IRTEValue = (7 << 05) + 03;    // Preset flag set, Ext int enabled, FPD set
    }

    AsmCpuid (
      CPUID_EXTENDED_TOPOLOGY,
      &CpuidRegisters.RegEax,
      &CpuidRegisters.RegEbx,
      &CpuidRegisters.RegEcx,
      &CpuidRegisters.RegEdx
      );
    IRTEValue |= (UINT64)CpuidRegisters.RegEdx << 32;    // Destination Processor Apic ID

//            if(mSystemConfiguration.ForcePhysicalModeEnable == 00) {
//              IRTEValue |= 0x4;
//            }

    *(volatile UINT64 *)((UINT64)xApicAddr + (Count * 16))= IRTEValue;

    //Perform a CLFLUSH instruction for each cachline in this 4KB memory to ensure that updates to the interrupt-remap-table are visible in memory
    AsmFlushCacheLine ((VOID *)((UINT64)xApicAddr + (Count * 16)));
  }
  // 3. Program the VT-D remap engines
  for (RemapEng = 0; RemapEng < RemapEngCount; RemapEng++) {

   for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
     // Check for valid stack
     if (!(mIioUds2->IioUdsPtr->PlatformData.CpuQpiInfo[RemapEng].stackPresentBitmap & (1 << Stack))) {
       continue;
     }
     VtdBarAddress = GetVtdBar (RemapEng, Stack);
     if (VtdBarAddress) {
    // 2. For each VT-d unit in the platform, allocate and initialize the invalidation queue/commands as follows

    // Allocate memory for the queued invalidation.
    Status = gBS->AllocatePool (EfiACPIReclaimMemory, 0x1000 + 0x1000, &Addr);
    if (EFI_ERROR (Status)) {
      ASSERT(FALSE);
      return;
    }
    ZeroMem (Addr, 0x1000 + 0x1000);
    Addr = (UINT64 *)((UINT64)Addr & (~0xFFF));

// Submit two descriptors to the respective VT-d unit's invalidation queue as follows:
    //  ??????????????? Program 1st descriptor in invalidation-queue as Interrupt-Entry-Cache Invalidation Descriptor
    // with G (Granularity) field Clear
    Addr[0] = 0x04;     // Interrupt Entry Cache Invalidate Descriptor
    Addr[1] = 0x00;

    // Program 2nd descriptor in invalidation-queue as Invalidation-Wait-Descriptor as follows:          +Status-Data=1
    // +Status-Address=address of variable tmp[unit +SW=1 +FN=1 +IF=0

    Addr[2] = ((UINT64)1 << 32) + (06 << 04) + 05;      // Invalidation Wait Descriptor

    TempQWord[RemapEng] = 00;
    Addr[3] = (UINTN)&TempQWord[RemapEng];    // Status Address [63:2] bits[127:65]

    // 3. Program the IRTA register to point to the IRT table.
    // For each VT-d unit in the platform, program interrupt-remap-table address and enable extended-interrupt-mode as follows
    IRTA  = (UINT64 *)((UINT64)VtdBarAddress + R_VTD_IRTA_REG);
	Value = *(volatile UINT32 *)((UINT64)VtdBarAddress+ R_VTD_GSTS_REG);
    // ???? *(volatile UINT64*)IRTA = 04  + 0x800 + (UINT64)xApicAddr ;   // [0:3] size = 2 Power (X+1). Bit11 =1 Xapic mode Bit[12:63] addrerss
    if (DmarPrivateData->Dmar->Flags && EFI_ACPI_DMAR_TABLE_FLAGS_X2APIC_OPT_OUT){
      *(volatile UINT64*)IRTA = 07 + (UINT64)xApicAddr ;   // [0:3] size = 2 Power (X+1). Bit11 =1 Xapic mode Bit[12:63] addrerss
      *(volatile UINT32 *)((UINT64)VtdBarAddress+ R_VTD_GCMD_REG) = (UINT32)(Value | BIT23);
    } else
    {
      *(volatile UINT64*)IRTA = 07  + 0x800 + (UINT64)xApicAddr ;   // [0:3] size = 2 Power (X+1). Bit11 =1 Xapic mode Bit[12:63] addrerss
    }
    // b. Set SIRTP in the command register.
    Count = 0x1000;
    *(volatile UINT32 *)((UINT64)VtdBarAddress+ R_VTD_GCMD_REG) = (UINT32)(Value | BIT24);

    // Wait till the status bit is set indicating the completion of the SIRTP.
    while (Count)  {
      Count--;
      Value = *(volatile UINT32 *)((UINT64)VtdBarAddress + R_VTD_GSTS_REG);
      if (Value & BIT24) {
        break;
      }
    }
    if (Count == 0) {
      ASSERT(FALSE);
      CpuDeadLoop ();
    }
    *(volatile UINT64 *)((UINT64)VtdBarAddress+ R_VTD_IQA_REG) = (UINT64)Addr;
    }// End of if (VtdBarAddress)
   } // End of for (Stack = 0; Stack < MAX_IIO_STACK; Stack++)
  }

  for (RemapEng = 0; RemapEng < RemapEngCount; RemapEng++) {

   for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {

     // Check for valid stack
     if (!(mIioUds2->IioUdsPtr->PlatformData.CpuQpiInfo[RemapEng].stackPresentBitmap & (1 << Stack))) {
       continue;
     }
     VtdBarAddress = GetVtdBar (RemapEng, Stack);
     if (VtdBarAddress) {

    // 4. For each VT-d unit in the platform, setup invalidation-queue base registers and enable invalidation as follows
    // Initialize a single descriptor which invalidates all the interrupt entries.
    // IQA register write (zeros IQH and IQT)

    // Enable queued invalidation in the command register.
    Count = 0x1000;
    Value = *(volatile UINT32 *)((UINT64)VtdBarAddress+ R_VTD_GSTS_REG);
    *(volatile UINT32 *)((UINT64)VtdBarAddress + R_VTD_GCMD_REG) = (UINT32)(Value | BIT26);

    while (Count)  {
      Count--;
      Value = *(volatile UINT32 *)((UINT64)VtdBarAddress+ R_VTD_GSTS_REG);
      if( Value & BIT26) {
        break;
      }
    }
    if (Count == 0) {
      ASSERT(FALSE);
      CpuDeadLoop ();
    }

    // Start invalidations, program the IQT register
    // Write the invalidation queue tail (IQT_REG) register as follows to indicate to hardware two descriptors are submitted:
    // +Bits 63:19 are 0 +Bits 18:4 gets value of 2h +Bits 3:0 are 0

    *(volatile UINT64 *)((UINT64)VtdBarAddress + R_VTD_IQT_REG) = (02 << 04); // Set tail to 02
    } // End of if (VtdAddress)
   } //End of for (Stack = 0; Stack < MAX_IIO_STACK; Stack++)
  }

  for (RemapEng = 0; RemapEng < RemapEngCount; RemapEng++) {

    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {

      if (!(mIioUds2->IioUdsPtr->PlatformData.CpuQpiInfo[RemapEng].stackPresentBitmap & (1 << Stack))) {
        continue;  // Skip invalid stacks
      }
      VtdBarAddress = GetVtdBar (RemapEng, Stack);
      if (VtdBarAddress) {
        //
        // 5. For each VT-d unit in the platform, wait for invalidation completion, and enable interrupt remapping as follows
        // Wait till the previously submitted invalidation commands are completed as follows
        // Poll on the variable tmp[unit] in memory, until its value is 1h.
        //
        Count = 0x1000;
        while (Count)  {
          Count--;
          Value = TempQWord[RemapEng];
          if (Value & 01) {
            break;
          }
        }
        if (Count == 0) {
          ASSERT(FALSE);
          CpuDeadLoop ();
        }
      } // End of if VtdBarAddress
    } //End of for (Stack = 0; Stack < MAX_IIO_STACK; Stack++)
  }

  // 5. Enable external interrupts in the IOAPIC RTE entry 0
  *(volatile UINT32 *)((UINT64)PCH_IOAPIC_ADDRESS)        = 0x10; //
  *(volatile UINT32 *)((UINT64)PCH_IOAPIC_ADDRESS + 0x10) = 0x00; // Set index to the IRTE0

  *(volatile UINT32 *)((UINT64)PCH_IOAPIC_ADDRESS)        = 0x10+1;
  *(volatile UINT32 *)((UINT64)PCH_IOAPIC_ADDRESS + 0x10) = 0x10000;// Set Remap enable bit
}


/**
    Build DRHD entry into ACPI DMAR table for specific stack.
    Include IOxAPIC, PCIExpress ports, and CBDMA if C-STACK.

    @param DmaRemap        - pointer to DMA remapping protocol
    @param IioIndex        - IIO index to be processed
    @param Stack           - stack index to be processed
    @param DevScope        - buffer for device scope data structure
    @param PciNode         - buffer for PCI node data structure
    @param PciRootBridgePtr- pointer to PciRootBridgeIo protocol for PCI access

    @retval EFI_SUCCESS - DRHD entry built successfully
**/
EFI_STATUS
BuildDRHDForStack (
  EFI_DMA_REMAP_PROTOCOL          *DmaRemap,
  UINT8                           IioIndex,
  UINT8                           Stack,
  DEVICE_SCOPE                    *DevScope,
  PCI_NODE                        *PciNode,
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgePtr,
  UINT8                            ApicIndex
  )
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  UINT8                       Bus;
  UINT8                       Dev;
  UINT8                       Func;
  UINT8                       DevIndex;
  UINT8                       PciNodeIndex;
  UINT8                       PortIndex;
  UINT8                       MaxPortNumberPerSocket;
  UINT8                       CBIndex;
  UINT64                      VtdMmioExtCap;
  UINT32                      VtdBase;
  UINT32                      VidDid;
  FPGA_CONFIGURATION          FpgaConfiguration;
  DMAR_ATSR                   *pAtsr = NULL;
  UINT8                       BusNumber = 0;

  ASSERT (mIioUds2);
  if (Stack > MAX_IIO_STACK){
    return EFI_UNSUPPORTED;
  }
  if (PciRootBridgePtr == NULL) {
    ASSERT (!(PciRootBridgePtr == NULL));
    return EFI_INVALID_PARAMETER;
  }

  mDrhd.Flags = EFI_ACPI_DMAR_DRHD_FLAGS_INCLUDE_ALL_CLEAR;  // all non-legacy stack has INCLUDE_ALL flag cleared
  if (Is14nmCpu) {
    //
    // BIOS detect FPGA SKU
    //
    ZeroMem (&FpgaConfiguration, sizeof (FPGA_CONFIGURATION));
    Status = FpgaConfigurationGetValues (&FpgaConfiguration);
    if(!EFI_ERROR (Status)) {
      //
      // Build FPGA DRHD entry
      //
      if ((FpgaConfiguration.FpgaSktActive & (1 << IioIndex)) && ( Stack == IIO_PSTACK3)) {

        DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] Build DRHD FPGA entry for MCP0 port\n", IioIndex, Stack));
        DevIndex                      = 00;
        PciNodeIndex                  = 00;
        mDrhd.DeviceScopeNumber       = 00;
        mDrhd.RegisterBase  = GetVtdBar (IioIndex, Stack);
        ZeroMem (DevScope, MEMORY_SIZE * sizeof (DEVICE_SCOPE));
        ZeroMem (PciNode, MEMORY_SIZE * sizeof (PCI_NODE));
        //
        //Report RCiEP0 device
        //
        DevScope[DevIndex].DeviceType         = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ENDPOINT;
        DevScope[DevIndex].EnumerationID      = 00;

        Status =  GetSocketStackLimitBus (&BusNumber, IioIndex, Stack);
        if (EFI_ERROR (Status)) {
          ASSERT_EFI_ERROR (Status);
          return Status;
        }

        DevScope[DevIndex].StartBusNumber     = BusNumber - FPGA_PREAllOCATE_BUS_NUM + 0x01;
        DevScope[DevIndex].PciNode            = &PciNode[PciNodeIndex];
        DevIndex++;

        PciNode[PciNodeIndex].Device          = 0;
        PciNode[PciNodeIndex].Function        = 0;
        PciNodeIndex++;
        PciNode[PciNodeIndex].Device    = (UINT8) -1;
        PciNode[PciNodeIndex].Function  = (UINT8) -1;
        PciNodeIndex++;
        mDrhd.DeviceScopeNumber++;
        //
        //Report VF device
        //
        DevScope[DevIndex].DeviceType         = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ENDPOINT;
        DevScope[DevIndex].EnumerationID      = 00;
        DevScope[DevIndex].StartBusNumber     = BusNumber - FPGA_PREAllOCATE_BUS_NUM + 0x01;
        DevScope[DevIndex].PciNode            = &PciNode[PciNodeIndex];
        DevIndex++;

        PciNode[PciNodeIndex].Device          = 0;
        PciNode[PciNodeIndex].Function        = 1;
        PciNodeIndex++;
        PciNode[PciNodeIndex].Device    = (UINT8) -1;
        PciNode[PciNodeIndex].Function  = (UINT8) -1;
        PciNodeIndex++;
        mDrhd.DeviceScopeNumber++;

        DmaRemap->InsertDmaRemap (DmaRemap, DrhdType, &mDrhd);
        return EFI_SUCCESS;
      }
    }
  }

  VtdBase = GetVtdBar (IioIndex, Stack);
  if (VtdBase != 0) {
    VtdMmioExtCap = *(volatile UINT64*)((UINTN)VtdBase + R_VTD_EXT_CAP_LOW);
    mDrhd.RegisterBase = VtdBase;
  } else {
    VtdMmioExtCap = 0;
    ASSERT (VtdBase !=0);
  }

  DevIndex                      = 00;
  PciNodeIndex                  = 00;
  mDrhd.DeviceScopeNumber       = 00;
  ZeroMem (DevScope, MEMORY_SIZE * sizeof (DEVICE_SCOPE));
  ZeroMem (PciNode, MEMORY_SIZE * sizeof (PCI_NODE));

  //
  // DRHD - IOxAPCI entry
  //
  if (Is14nmCpu) {
      //
      // Check Interrupt Remap support.
      //
      DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] SysIoApicEnable: 0x%llX, InterruptRemap: %d, VtdExtCap: 0x%x\n",
             IioIndex, Stack, mPlatformInfo->SysData.SysIoApicEnable, DmaRemap->InterruptRemap, VtdMmioExtCap));
      if ((mPlatformInfo->SysData.SysIoApicEnable & ((UINT64)1 << (IioIndex * MAX_IIO_STACK + Stack + 1))) &&
          DmaRemap->InterruptRemap && (VtdMmioExtCap & INTRREMAP)) {

          DevScope[DevIndex].DeviceType = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_IOAPIC;
          DevScope[DevIndex].EnumerationID = IoApicID[ApicIndex + 1];
          DevScope[DevIndex].StartBusNumber = mCpuCsrAccessVarPtr->StackBus[IioIndex][Stack];
          DevScope[DevIndex].PciNode = &PciNode[PciNodeIndex];

          PciNode[PciNodeIndex].Device = APIC_DEV_NUM;
          PciNode[PciNodeIndex].Function = APIC_FUNC_NUM;
          DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] Build DRHD IOAPIC: Type %d, EnumId %d, StartBus 0x%x, PciNode %02X.%X\n",
                  IioIndex, Stack,
                  DevScope[DevIndex].DeviceType, DevScope[DevIndex].EnumerationID, DevScope[DevIndex].StartBusNumber,
                  DevScope[DevIndex].PciNode->Device, DevScope[DevIndex].PciNode->Function));
          DevIndex++;
          PciNodeIndex++;
          PciNode[PciNodeIndex].Device = (UINT8)-1;
          PciNode[PciNodeIndex].Function = (UINT8)-1;
          PciNodeIndex++;
          mDrhd.DeviceScopeNumber++;
    }
  }
  //
  // DRHD - CBDMA entry
  //
  if (!IsCpuAndRevision (CPU_SPRSP, REV_ALL) &&
      (Stack == IIO_CSTACK || Stack == IIO_STACK0)) {

    for (CBIndex = 0; CBIndex <= 7; CBIndex++) {

        DevScope[DevIndex].DeviceType         = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ENDPOINT;
        DevScope[DevIndex].EnumerationID      = 00;
        if (Is14nmCpu) {
          DevScope[DevIndex].StartBusNumber = mCpuCsrAccessVarPtr ->StackBus[IioIndex][IIO_CSTACK];
        } else {
          DevScope[DevIndex].StartBusNumber = mCpuCsrAccessVarPtr ->StackBus[IioIndex][IIO_STACK0];
        }
        DevScope[DevIndex].PciNode            = &PciNode[PciNodeIndex];

        if (Is14nmCpu) {
            PciNode[PciNodeIndex].Device = IOAT_DEVICE_NUM;
        } else {
            PciNode[PciNodeIndex].Device = IOAT_DEVICE_NUM_10NM;
        }
        PciNode[PciNodeIndex].Function        = CBIndex;
        DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] Build DRHD CBDMA: Type %d, EnumId %d, StartBus 0x%x, PciNode %02X.%X\n",
                IioIndex, Stack,
                DevScope[DevIndex].DeviceType, DevScope[DevIndex].EnumerationID, DevScope[DevIndex].StartBusNumber,
                DevScope[DevIndex].PciNode->Device, DevScope[DevIndex].PciNode->Function));
        DevIndex++;
        PciNodeIndex++;
        PciNode[PciNodeIndex].Device    = (UINT8) -1;
        PciNode[PciNodeIndex].Function  = (UINT8) -1;
        PciNodeIndex++;

        mDrhd.DeviceScopeNumber++;
      } // End of for for(CBIndex = 0; CBIndex <= 07; CBIndex++)

  }

  //
  // DRHD - PCI-Ex ports
  //
  pAtsr = GetDmarAtsrTablePointer ();
  MaxPortNumberPerSocket = GetMaxPortPerSocket (IioIndex);
  for (PortIndex = 1; PortIndex < MaxPortNumberPerSocket; PortIndex++) {

    if (GetStackPerPort (IioIndex, PortIndex) != Stack) {
      continue;
    }
    Bus = GetSocketPortBusNum (IioIndex, PortIndex);
    Dev = 0;
    Func = 0;
    if (pAtsr != NULL){
      Dev = pAtsr->DeviceScope[PortIndex].PciNode->Device;
      Func = pAtsr->DeviceScope[PortIndex].PciNode->Function;
    }
// APTIOV_SERVER_OVERRIDE_RC_START: Replaced the usage of PciRootBridgeIo Read with IioPciExpressRead32 as the first RootBridge instance was directly used
// The first RootBridge instance will fail RootBridgeIoCheck for devices belonging to other Root Bridges
    //
    // Skip root ports which do not respond to PCI configuration cycles.
    //
    VidDid = 0;
/*    Status = PciRootBridgePtr->Pci.Read (
                PciRootBridgePtr,
                EfiPciWidthUint32,
                EFI_PCI_ADDRESS (Bus, Dev, Func, 0),
                1,
                &VidDid);
    if (EFI_ERROR (Status) || VidDid == 0xffffffff) {*/
    VidDid = IioPciExpressRead32(IioIndex, Bus, Dev, Func, PCI_VENDOR_ID_OFFSET);
    if (VidDid == 0xffffffff) {
      DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d p%d] %02X:%02X:%02X.%d Hidden (%X) - skip\n",
              IioIndex, Stack, PortIndex,
              mIioUds2->IioUdsPtr->PlatformData.CpuQpiInfo[IioIndex].PcieSegment,
              Bus, Dev, Func, VidDid));
      continue;
    }
// APTIOV_SERVER_OVERRIDE_RC_END: Replaced the usage of PciRootBridgeIo Read with IioPciExpressRead32 as the first RootBridge instance was directly used
    if (IioVmdPortIsEnabled (IioIndex, PortIndex) || GetCurrentPXPMap (IioIndex, PortIndex) == 0) {

      DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d p%d] %a - skip\n", IioIndex, Stack, PortIndex,
              (GetCurrentPXPMap (IioIndex, PortIndex) == 0) ? "Link width not set" : "Dummy VMD function"));
      continue;
    }
    if (GetPCIePortOwnerShip (IioIndex, PortIndex) == NTB_PORT_DEF_NTB_NTB ||
        GetPCIePortOwnerShip (IioIndex, PortIndex) == NTB_PORT_DEF_NTB_RP) {

      DevScope[DevIndex].DeviceType         = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ENDPOINT;
    } else {
      DevScope[DevIndex].DeviceType         = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE;
    }
    DevScope[DevIndex].EnumerationID      = 00;
    DevScope[DevIndex].StartBusNumber     = Bus;
    DevScope[DevIndex].PciNode            = &PciNode[PciNodeIndex];
    PciNode[PciNodeIndex].Device          = Dev;
    PciNode[PciNodeIndex].Function        = Func;
    DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d p%d] Build DRHD PCI: Type %d, EnumId %d, StartBus 0x%x, PciNode %02X.%X\n",
            IioIndex, Stack, PortIndex,
            DevScope[DevIndex].DeviceType, DevScope[DevIndex].EnumerationID, DevScope[DevIndex].StartBusNumber,
            DevScope[DevIndex].PciNode->Device, DevScope[DevIndex].PciNode->Function));
    DevIndex++;
    PciNodeIndex++;
    PciNode[PciNodeIndex].Device    = (UINT8) -1;
    PciNode[PciNodeIndex].Function  = (UINT8) -1;
    PciNodeIndex++;

    mDrhd.DeviceScopeNumber++;
  } // for (PortIndex...)

  Status = IioVmdGetPciLocation (IioIndex, Stack,
                                 &PciNode[PciNodeIndex].Device, &PciNode[PciNodeIndex].Function);
  if (!EFI_ERROR (Status)) {
    //
    // VMD is enabled in this stack, expose VMD PCI device in DMAR for DMA remapping.
    //
    DevScope[DevIndex].DeviceType         = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ENDPOINT;
    DevScope[DevIndex].EnumerationID      = 00;
    DevScope[DevIndex].StartBusNumber     = mCpuCsrAccessVarPtr->StackBus[IioIndex][Stack];
    DevScope[DevIndex].PciNode            = &PciNode[PciNodeIndex];
    DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] Build DRHD VMD: Type %d, EnumId %d, StartBus 0x%x, PciNode %02X.%X\n",
            IioIndex, Stack,
            DevScope[DevIndex].DeviceType, DevScope[DevIndex].EnumerationID, DevScope[DevIndex].StartBusNumber,
            DevScope[DevIndex].PciNode->Device, DevScope[DevIndex].PciNode->Function));
    DevIndex++;
    PciNodeIndex++;
    PciNode[PciNodeIndex].Device = (UINT8)-1;
    PciNode[PciNodeIndex].Function = (UINT8)-1;
    PciNodeIndex++;

    mDrhd.DeviceScopeNumber++;
  }
  //
  // DRHD - DINO device
  //
  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL) && (Stack >= IIO_STACK8) && (Stack <= IIO_STACK11)) {
    DMAR_DEVICE Dino_Device[] = {
      {0,1,0},              // DSA
      {0,2,0},              // IAX
      {CPM_BUS_OFFSET,0,0}, // CPM
      {HQM_BUS_OFFSET,0,0}, // HQM
      {0xff,0xff,0xff}      // End
    };

    for (PortIndex = 0; Dino_Device[PortIndex].Bus != 0xff; PortIndex ++) {
      Bus = Dino_Device[PortIndex].Bus + mCpuCsrAccessVarPtr->StackBus[IioIndex][Stack];
      Dev = Dino_Device[PortIndex].Dev;
      Func = Dino_Device[PortIndex].Func;
      Status = PciRootBridgePtr->Pci.Read (
                  PciRootBridgePtr,
                  EfiPciWidthUint32,
                  EFI_PCI_ADDRESS (Bus, Dev, Func, 0),
                  1,
                  &VidDid);
      if (!EFI_ERROR (Status) && (VidDid != 0xffffffff)) {
        DevScope[DevIndex].DeviceType = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ENDPOINT;
        DevScope[DevIndex].EnumerationID = 00;
        DevScope[DevIndex].StartBusNumber = Bus;
        DevScope[DevIndex].PciNode = &PciNode[PciNodeIndex];

        PciNode[PciNodeIndex].Device = Dev;
        PciNode[PciNodeIndex].Function = Func;
        DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] Build DRHD DINO: Type %d, EnumId %d, StartBus 0x%x, PciNode %02X.%X\n",
                IioIndex, Stack,
                DevScope[DevIndex].DeviceType, DevScope[DevIndex].EnumerationID, DevScope[DevIndex].StartBusNumber,
                DevScope[DevIndex].PciNode->Device, DevScope[DevIndex].PciNode->Function));
        DevIndex++;
        PciNodeIndex++;
        PciNode[PciNodeIndex].Device = (UINT8)-1;
        PciNode[PciNodeIndex].Function = (UINT8)-1;
        PciNodeIndex++;

        mDrhd.DeviceScopeNumber++;
      }
    }
  }
  //
  // DRHD - HQM/TIP device
  //
  if (IsCpuAndRevision (CPU_SNR, REV_ALL) && (IIO_STACK2 == Stack )) {

      DevScope[DevIndex].DeviceType = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ENDPOINT;
      DevScope[DevIndex].EnumerationID = 00;
      DevScope[DevIndex].StartBusNumber = mIioUds2->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[Stack].BusLimit - V_IOV_RESERVED_BUS_NUMBER;
      DevScope[DevIndex].PciNode = &PciNode[PciNodeIndex];
      PciNode[PciNodeIndex].Device = 0;
      PciNode[PciNodeIndex].Function = 0;
      DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] Build DRHD HQM/TIP: Type %d, EnumId %d, StartBus 0x%x, PciNode %02X.%X\n",
              IioIndex, Stack,
              DevScope[DevIndex].DeviceType, DevScope[DevIndex].EnumerationID, DevScope[DevIndex].StartBusNumber,
              DevScope[DevIndex].PciNode->Device, DevScope[DevIndex].PciNode->Function));
      DevIndex++;
      PciNodeIndex++;
      PciNode[PciNodeIndex].Device = (UINT8)-1;
      PciNode[PciNodeIndex].Function = (UINT8)-1;
      PciNodeIndex++;

      mDrhd.DeviceScopeNumber++;
  }

  //
  // DRHD - CPK device
  //
  if (IsCpuAndRevision (CPU_SNR, REV_ALL) && (IIO_STACK3 == Stack)) {
      //
      // Virtual Root Port 0
      //
      DevScope[DevIndex].DeviceType = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE;
      DevScope[DevIndex].EnumerationID = 00;
      DevScope[DevIndex].StartBusNumber = mCpuCsrAccessVarPtr->StackBus[IioIndex][Stack];
      DevScope[DevIndex].PciNode = &PciNode[PciNodeIndex];
      PciNode[PciNodeIndex].Device = 4;
      PciNode[PciNodeIndex].Function = 0;
      DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] Build DRHD CPK: Type %d, EnumId %d, StartBus 0x%x, PciNode %02X.%X\n",
              IioIndex, Stack,
              DevScope[DevIndex].DeviceType, DevScope[DevIndex].EnumerationID, DevScope[DevIndex].StartBusNumber,
              DevScope[DevIndex].PciNode->Device, DevScope[DevIndex].PciNode->Function));
      DevIndex++;
      PciNodeIndex++;
      PciNode[PciNodeIndex].Device = (UINT8)-1;
      PciNode[PciNodeIndex].Function = (UINT8)-1;
      PciNodeIndex++;

      mDrhd.DeviceScopeNumber++;
  }

  //
  // DRHD - CPM device
  //
  if (IsCpuAndRevision (CPU_SNR, REV_ALL) && ( IIO_STACK4 == Stack )) {
      //
      // Virtual Root Port 0
      //
      DevScope[DevIndex].DeviceType = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE;
      DevScope[DevIndex].EnumerationID = 00;
      DevScope[DevIndex].StartBusNumber = mCpuCsrAccessVarPtr->StackBus[IioIndex][Stack];
      DevScope[DevIndex].PciNode = &PciNode[PciNodeIndex];
      PciNode[PciNodeIndex].Device = 4;
      PciNode[PciNodeIndex].Function = 0;
      DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] Build DRHD CPM: Type %d, EnumId %d, StartBus 0x%x, PciNode %02X.%X\n",
              IioIndex, Stack,
              DevScope[DevIndex].DeviceType, DevScope[DevIndex].EnumerationID, DevScope[DevIndex].StartBusNumber,
              DevScope[DevIndex].PciNode->Device, DevScope[DevIndex].PciNode->Function));
      DevIndex++;
      PciNodeIndex++;
      PciNode[PciNodeIndex].Device = (UINT8)-1;
      PciNode[PciNodeIndex].Function = (UINT8)-1;
      PciNodeIndex++;
      mDrhd.DeviceScopeNumber++;

      //
      //Virtual Root Port 1
      //
      DevScope[DevIndex].DeviceType = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE;
      DevScope[DevIndex].EnumerationID = 00;
      DevScope[DevIndex].StartBusNumber = mCpuCsrAccessVarPtr->StackBus[IioIndex][Stack];
      DevScope[DevIndex].PciNode = &PciNode[PciNodeIndex];
      PciNode[PciNodeIndex].Device = 5;
      PciNode[PciNodeIndex].Function = 0;
      DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] Build DRHD CPM: Type %d, EnumId %d, StartBus 0x%x, PciNode %02X.%X\n",
              IioIndex, Stack,
              DevScope[DevIndex].DeviceType, DevScope[DevIndex].EnumerationID, DevScope[DevIndex].StartBusNumber,
              DevScope[DevIndex].PciNode->Device, DevScope[DevIndex].PciNode->Function));
      DevIndex++;
      PciNodeIndex++;
      PciNode[PciNodeIndex].Device = (UINT8)-1;
      PciNode[PciNodeIndex].Function = (UINT8)-1;
      PciNodeIndex++;
      mDrhd.DeviceScopeNumber++;

      //
      //iRC_NAC
      //
      DevScope[DevIndex].DeviceType = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ENDPOINT;
      DevScope[DevIndex].EnumerationID = 00;
      DevScope[DevIndex].StartBusNumber = mCpuCsrAccessVarPtr->StackBus[IioIndex][Stack];
      DevScope[DevIndex].PciNode = &PciNode[PciNodeIndex];
      PciNode[PciNodeIndex].Device = 6;
      PciNode[PciNodeIndex].Function = 0;
      DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] Build DRHD CPM: Type %d, EnumId %d, StartBus 0x%x, PciNode %02X.%X\n",
              IioIndex, Stack,
              DevScope[DevIndex].DeviceType, DevScope[DevIndex].EnumerationID, DevScope[DevIndex].StartBusNumber,
              DevScope[DevIndex].PciNode->Device, DevScope[DevIndex].PciNode->Function));
      DevIndex++;
      PciNodeIndex++;
      PciNode[PciNodeIndex].Device = (UINT8)-1;
      PciNode[PciNodeIndex].Function = (UINT8)-1;
      PciNodeIndex++;
      mDrhd.DeviceScopeNumber++;
  }
  //
  // DRHD - CPK device and CPM device
  //
  if (IsCpuAndRevision (CPU_ICXD, REV_ALL) && (Stack == IIO_STACK2)) {
    DevScope[DevIndex].DeviceType = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE;
    DevScope[DevIndex].EnumerationID = 00;
    DevScope[DevIndex].StartBusNumber = mCpuCsrAccessVarPtr->StackBus[IioIndex][Stack];
    DevScope[DevIndex].PciNode = &PciNode[PciNodeIndex];
    if (GetChopType (0) == TypeLcc) {
      //
      // Virtual Root Port 0 for CPK Device is present in Stack 2 for ICXD LCC
      //
      PciNode[PciNodeIndex].Device = 4;
      PciNode[PciNodeIndex].Function = 0;
      DEBUG((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] Build DRHD CPK: Type %d, EnumId %d, StartBus 0x%x, PciNode %02X.%X\n",
        IioIndex, Stack,
        DevScope[DevIndex].DeviceType, DevScope[DevIndex].EnumerationID, DevScope[DevIndex].StartBusNumber,
        DevScope[DevIndex].PciNode->Device, DevScope[DevIndex].PciNode->Function));
    } else if (GetChopType (0) == TypeHcc) {
      //
      //  Virtual Root Port 1 for CPM Device is present in Stack 2 for ICXD HCC
      //
      PciNode[PciNodeIndex].Device = 5;
      PciNode[PciNodeIndex].Function = 0;
      DEBUG((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] Build DRHD CPM: Type %d, EnumId %d, StartBus 0x%x, PciNode %02X.%X\n",
        IioIndex, Stack,
        DevScope[DevIndex].DeviceType, DevScope[DevIndex].EnumerationID, DevScope[DevIndex].StartBusNumber,
        DevScope[DevIndex].PciNode->Device, DevScope[DevIndex].PciNode->Function));
    }
    DevIndex++;
    PciNodeIndex++;
    PciNode[PciNodeIndex].Device = (UINT8)-1;
    PciNode[PciNodeIndex].Function = (UINT8)-1;
    PciNodeIndex++;
    mDrhd.DeviceScopeNumber++;

    //
    //iRC_NAC
    //
    DevScope[DevIndex].DeviceType = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ENDPOINT;
    DevScope[DevIndex].EnumerationID = 00;
    DevScope[DevIndex].StartBusNumber = mCpuCsrAccessVarPtr->StackBus[IioIndex][Stack];
    DevScope[DevIndex].PciNode = &PciNode[PciNodeIndex];
    PciNode[PciNodeIndex].Device = 6;
    PciNode[PciNodeIndex].Function = 0;
    DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] Build DRHD CPK: Type %d, EnumId %d, StartBus 0x%x, PciNode %02X.%X\n",
            IioIndex, Stack,
            DevScope[DevIndex].DeviceType, DevScope[DevIndex].EnumerationID, DevScope[DevIndex].StartBusNumber,
            DevScope[DevIndex].PciNode->Device, DevScope[DevIndex].PciNode->Function));
    DevIndex++;
    PciNodeIndex++;
    PciNode[PciNodeIndex].Device = (UINT8)-1;
    PciNode[PciNodeIndex].Function = (UINT8)-1;
    PciNodeIndex++;
    mDrhd.DeviceScopeNumber++;
  }
  //
  // DRHD - CPK device
  //
  if (IsCpuAndRevision (CPU_ICXD, REV_ALL) && (Stack == IIO_STACK3)) {
    if (GetChopType (0) == TypeHcc) {
      //
      // Virtual Root Port 0 for CPK Device is present in Stack 3 for ICXD HCC
      //
      DevScope[DevIndex].DeviceType = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE;
      DevScope[DevIndex].EnumerationID = 00;
      DevScope[DevIndex].StartBusNumber = mCpuCsrAccessVarPtr->StackBus[IioIndex][Stack];
      DevScope[DevIndex].PciNode = &PciNode[PciNodeIndex];
      PciNode[PciNodeIndex].Device = 4;
      PciNode[PciNodeIndex].Function = 0;
      DEBUG((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] Build DRHD CPM: Type %d, EnumId %d, StartBus 0x%x, PciNode %02X.%X\n",
        IioIndex, Stack,
        DevScope[DevIndex].DeviceType, DevScope[DevIndex].EnumerationID, DevScope[DevIndex].StartBusNumber,
        DevScope[DevIndex].PciNode->Device, DevScope[DevIndex].PciNode->Function));
      DevIndex++;
      PciNodeIndex++;
      PciNode[PciNodeIndex].Device = (UINT8)-1;
      PciNode[PciNodeIndex].Function = (UINT8)-1;
      PciNodeIndex++;
      mDrhd.DeviceScopeNumber++;
    }
  }

  DmaRemap->InsertDmaRemap (DmaRemap, DrhdType, &mDrhd);

  return EFI_SUCCESS;
}
// APTIOV_SERVER_OVERRIDE_RC_START : Adding RHSA structure in DMAR table
/**
 * 
 * Method to add RHSA structure to DMAR for each stack
 * 
 * @param DmaRemap   Pointer to Dma Remap Protocol
 * @param SocketIndx Index of socket need to be reported 
 * @param StackIndx  Index of stack need to be reported
 * @return VOID
 */
 VOID
 BuildRhsaDmar (
  EFI_DMA_REMAP_PROTOCOL          *DmaRemap,
  UINT8                            SocketIndx,
  UINT8                            StackIndx
  )
{
    mRhsa.RegisterBase = mIioUds2->IioUdsPtr->PlatformData.IIO_resource[SocketIndx].StackRes[StackIndx].VtdBarAddress;
    
    if ((mIioUds2->IioUdsPtr->SystemStatus.OutSncEn != 0) && (mIioUds2->IioUdsPtr->SystemStatus.OutNumOfCluster == 2)){
            if( StackIndx < (MAX_IIO_STACK/2)){
                mRhsa.Domian =  (mIioUds2->IioUdsPtr->PlatformData.CpuQpiInfo[SocketIndx].SocId * 2 );
            }else{
                mRhsa.Domian =  (mIioUds2->IioUdsPtr->PlatformData.CpuQpiInfo[SocketIndx].SocId * 2 ) + 1;
            }
        }else{
           mRhsa.Domian =  mIioUds2->IioUdsPtr->PlatformData.CpuQpiInfo[SocketIndx].SocId;
       }

  DmaRemap->InsertDmaRemap(
             DmaRemap,
             RhsaType,
             &mRhsa);
}
// APTIOV_SERVER_OVERRIDE_RC_END : Adding RHSA structure in DMAR table


/**
    GC_TODO: add routine description

    @param DmaRemap - GC_TODO: add arg description

    @retval EFI_SUCCESS - GC_TODO: add retval description
**/
EFI_STATUS
ReportDmar (
  IN EFI_DMA_REMAP_PROTOCOL      *DmaRemap
  )
{
  EFI_STATUS                      Status = EFI_SUCCESS;
  UINT8                           SocketIndex, IioBusBase, Bus;
  UINT8                           Dev, Func;
  UINT8                           DevIndex;
  UINT8                           PciNodeIndex;
  UINT8                           PciPortIndex;
  UINT8                           MaxPortNumberPerSocket;
  UINT64                          VtdMmioExtCap;
  UINT32                          VtdBase;
  EFI_VTD_SUPPORT_INSTANCE        *DmarPrivateData;
  UINT16                          NumberOfHpets;
  UINT16                          HpetCapIdValue;
  DEVICE_SCOPE                    *DevScope;
  PCI_NODE                        *PciNode;
// APTIOV_SERVER_OVERRIDE_RC_START : For filling Reserved Base Address and Limit for RMRR structure of DMAR table
#if 0
  EFI_PHYSICAL_ADDRESS            Pointer;
  UINT32                          AlignedSize;
  UINT32                          NumberofPages;
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : For filling Reserved Base Address and Limit for RMRR structure of DMAR table
  BOOLEAN                         IntrRemapSupport;
  EFI_CPUID_REGISTER              CpuidRegisters;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgePtr;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeTab[MAX_SOCKET];
  UINT32                          VidDid;
  UINT8                           Index;
  UINT8                           Stack = 0;
  UINT8                           FirstRun = 0;
  VOID                            *HobPtr;
  PCH_INFO_HOB                    *PchInfoHob;
  DMAR_ATSR                       *pAtsr = NULL;
  UINT8                           ApicIndex = 1;
  UINTN                           HandleCount;
  EFI_HANDLE                      *HandleBuffer;

  HobPtr = GetFirstGuidHob (&gPchInfoHobGuid);
  if (HobPtr == NULL) {
    ASSERT (HobPtr != NULL);
    return EFI_INVALID_PARAMETER;
  }

  PchInfoHob = (PCH_INFO_HOB*) GET_GUID_HOB_DATA (HobPtr);
  if (PchInfoHob == NULL) {
    ASSERT (PchInfoHob != NULL);
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (mIioUds2);
  // APTIOV_SERVER_OVERRIDE_RC_START
   //For filling Reserved Base Address and Limit for RMRR structure of DMAR table
  #if defined AMIUSB_SUPPORT && AMIUSB_SUPPORT == 1
    static EFI_GUID UsbProtocolGuid = EFI_USB_PROTOCOL_GUID;
    EFI_USB_PROTOCOL *UsbProtocol;
    EFI_PHYSICAL_ADDRESS	StartAddress = 0, EndAddress = 0;
  #endif
   //Adding RHSA structure in DMAR table
    UINT8 SocketIndx = 0;
    UINT8 StackIndx;
    UINT8 NumaEnb = 0;
  // APTIOV_SERVER_OVERRIDE_RC_END
  DmarPrivateData = VTD_SUPPORT_INSTANCE_FROM_THIS (DmaRemap);
  //
  // Get DMAR_HOST_ADDRESS_WIDTH from CPUID.(EAX=80000008h) return the Phyical Address
  // Size in the EAX register. EAX[7:0]
  // Sync with Brickland code  DMAR_HOST_ADDRESS_WIDTH 45 = 46 - 1
  //
  AsmCpuid (
    CPUID_VIR_PHY_ADDRESS_SIZE,
    &CpuidRegisters.RegEax,
    &CpuidRegisters.RegEbx,
    &CpuidRegisters.RegEcx,
    &CpuidRegisters.RegEdx
  );

  DmarPrivateData->Dmar->HostAddressWidth = (UINT8)((CpuidRegisters.RegEax & 0xFF)-1);
  DmarPrivateData->Dmar->Flags = EFI_ACPI_DMAR_TABLE_FLAGS_INTR_REMAP_CLEAR;

  //
  // Locate PCI root bridge I/O protocol, for confirming PCI functions respond
  // to PCI configuration cycles.
  //
  ZeroMem (&PciRootBridgeTab[0], sizeof(PciRootBridgeTab));

  Status = gBS->LocateHandleBuffer (
                ByProtocol,
                &gEfiPciRootBridgeIoProtocolGuid,
                NULL,
                &HandleCount,
                &HandleBuffer
                );

  ASSERT_EFI_ERROR (Status);

  for (Index = 0; Index < HandleCount; Index ++) {
    Status = gBS->HandleProtocol (
                  HandleBuffer[Index],
                  &gEfiPciRootBridgeIoProtocolGuid,
                  &PciRootBridgePtr
                  );

    ASSERT_EFI_ERROR (Status);

    PciRootBridgeTab[PciRootBridgePtr->SegmentNumber] = PciRootBridgePtr;
  }
  FreePool (HandleBuffer);

  //
  // Allocate memory to DevScope structures
  //
  Status = gBS->AllocatePool (EfiACPIMemoryNVS, MEMORY_SIZE * sizeof (DEVICE_SCOPE), &DevScope);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->AllocatePool (EfiACPIMemoryNVS, MEMORY_SIZE * sizeof (PCI_NODE), &PciNode);
  ASSERT_EFI_ERROR (Status);

  for (Index = 1; Index <= MAX_SOCKET; Index++) {
    //
    // VT-d specification request that DHRD entry 0 should be the latest entry of the DMAR table.
    // To accomplish this, the following check will ensure that latest entry will be the one related to Socket 0.
    //
    if (Index == MAX_SOCKET) {
      SocketIndex = 0;
    } else {
      SocketIndex = Index;
    }

    if (SocketIndex >= MAX_SOCKET) {
      return EFI_INVALID_PARAMETER;
    }

    if (!SocketPresent (SocketIndex)) {
      continue;
    }

    if (mIioUds2->IioUdsPtr->PlatformData.CpuQpiInfo[SocketIndex].PcieSegment >= MAX_SOCKET) {
      return EFI_INVALID_PARAMETER;
    }
    PciRootBridgePtr = PciRootBridgeTab[mIioUds2->IioUdsPtr->PlatformData.CpuQpiInfo[SocketIndex].PcieSegment];

    if (Is14nmCpu) {
      Stack = IIO_CSTACK;
    } else {
      Stack = IIO_STACK0;
    }
    VtdBase = GetVtdBar (SocketIndex, Stack);

    DevIndex                      = 00;
    PciNodeIndex                  = 00;

    mDrhd.Signature               = DRHD_SIGNATURE;
    mDrhd.SegmentNumber           = mIioUds2->IioUdsPtr->PlatformData.CpuQpiInfo[SocketIndex].PcieSegment;
    mDrhd.DeviceScopeNumber       = 00;
    mDrhd.DeviceScope             = DevScope;
    mDrhd.RegisterBase            = VtdBase;
    ZeroMem (DevScope, MEMORY_SIZE * sizeof (DEVICE_SCOPE));
    ZeroMem (PciNode, MEMORY_SIZE * sizeof (PCI_NODE));

    VtdMmioExtCap = *(volatile UINT64*)((UINTN)VtdBase + R_VTD_EXT_CAP_LOW);

    //
    // Check Interrupt Remap support.
    //
    IntrRemapSupport = FALSE;
    if (VtdMmioExtCap & INTRREMAP) {
      IntrRemapSupport = TRUE;
    }
    DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] VT-d base 0x%X, ExtCap=0x%X\n",
            SocketIndex, Stack, VtdBase, VtdMmioExtCap));

    if (SocketIndex == 0) {
      ApicIndex = 1;
      //
      // DRHD - Legacy IOH
      //
      // Build DRHD on IIO0 - Stack1 to Stack5, not include C-STACK
      //
      for (Stack = 1; Stack < MAX_IIO_STACK; Stack++) {

        if (!IfStackPresent (SocketIndex, Stack)) {  // Skip invalid stack
          continue;
        }
        BuildDRHDForStack (DmaRemap, SocketIndex, Stack, DevScope, PciNode, PciRootBridgePtr, ApicIndex);
        ApicIndex++;
      }

      if (Is14nmCpu) {
        Stack = IIO_CSTACK;
      } else {
        Stack = IIO_STACK0;
      }

      //
      // Re-initialize DRHD template for DRHD entry in legacy socket C-STACK
      //
      DevIndex                      = 00;
      PciNodeIndex                  = 00;
      mDrhd.DeviceScopeNumber       = 00;
      mDrhd.RegisterBase            = GetVtdBar (SocketIndex, Stack);
      ZeroMem (DevScope, MEMORY_SIZE * sizeof (DEVICE_SCOPE));
      ZeroMem (PciNode, MEMORY_SIZE * sizeof (PCI_NODE));

      IioBusBase = mCpuCsrAccessVarPtr ->StackBus[SocketIndex][Stack]; // Stack 0
      mDrhd.Flags = EFI_ACPI_DMAR_DRHD_FLAGS_INCLUDE_ALL_SET;

      Status = IioVmdGetPciLocation (SocketIndex, Stack,
                                     &PciNode[PciNodeIndex].Device, &PciNode[PciNodeIndex].Function);
      if (!EFI_ERROR (Status)) {
        //
        // VMD is enabled in stack 0, expose VMD PCI device in DMAR for DMA remapping.
        //
        DevScope[DevIndex].DeviceType = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ENDPOINT;
        DevScope[DevIndex].EnumerationID = 00;
        DevScope[DevIndex].StartBusNumber = IioBusBase;
        DevScope[DevIndex].PciNode = &PciNode[PciNodeIndex];
        DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] Build DRHD VMD: Type %d, EnumId %d, StartBus 0x%x, PciNode %02X.%X\n",
                SocketIndex, Stack,
                DevScope[DevIndex].DeviceType, DevScope[DevIndex].EnumerationID, DevScope[DevIndex].StartBusNumber,
                DevScope[DevIndex].PciNode->Device, DevScope[DevIndex].PciNode->Function));
        DevIndex++;
        PciNodeIndex++;
        PciNode[PciNodeIndex].Device = (UINT8)-1;
        PciNode[PciNodeIndex].Function = (UINT8)-1;
        PciNodeIndex++;
        mDrhd.DeviceScopeNumber++;
      }

      DEBUG ((EFI_D_INFO, "[ACPI](DMAR) InterruptRemap is %aabled (%d & %d)\n",
              (DmaRemap->InterruptRemap && IntrRemapSupport) ? "en" : "dis", DmaRemap->InterruptRemap, IntrRemapSupport));
//DmaRemap->InterruptRemap is populated from setup knob. SHould we remove this and only keep IntrRemapSupport?
      if (DmaRemap->InterruptRemap && IntrRemapSupport) {

        DmarPrivateData->Dmar->Flags = EFI_ACPI_DMAR_TABLE_FLAGS_INTR_REMAP_SET;

        // If CPU is not in X2APIC mode, then set X2APIC opt out flag
        if (GetApicMode () != LOCAL_APIC_MODE_X2APIC){
         DmarPrivateData->Dmar->Flags |= EFI_ACPI_DMAR_TABLE_FLAGS_X2APIC_OPT_OUT;
        }
        // PCH - IOAPIC
        // This information will be provided by PCH side
        // Currently is a hard-coded temporal solution to set:
        // Bus = 0; Device and Function (together) = 0xF7;
        // This is the value that is stored in IBDF register:
        // ServerSiliconPkg/Pch/SouthClusterCdf/Include/Register/Pchregsp2sb.h
        //#define V_P2SB_CFG_IBDF_BUS                        0
        //#define V_P2SB_CFG_IBDF_DEV                        30
        //#define V_P2SB_CFG_IBDF_FUNC                       7
        DevScope[DevIndex].DeviceType = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_IOAPIC;
        DevScope[DevIndex].EnumerationID = PCH_IOAPIC_ID; // PCH team needs confirm this value. (This value affects VTd functionality?)
        DevScope[DevIndex].StartBusNumber = (UINT8)PchInfoHob->IoApicBusNum;
        DevScope[DevIndex].PciNode = &PciNode[PciNodeIndex];

        PciNode[PciNodeIndex].Device = (UINT8)PchInfoHob->IoApicDevNum;
        PciNode[PciNodeIndex].Function = (UINT8)PchInfoHob->IoApicFuncNum;
        DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] Build DRHD PCH IOAPIC: Type %d, EnumId %d, StartBus 0x%x, PciNode %02X.%X\n",
                SocketIndex, Stack,
                DevScope[DevIndex].DeviceType, DevScope[DevIndex].EnumerationID, DevScope[DevIndex].StartBusNumber,
                DevScope[DevIndex].PciNode->Device, DevScope[DevIndex].PciNode->Function));
        DevIndex++;
        PciNodeIndex++;
        PciNode[PciNodeIndex].Device = (UINT8)-1;
        PciNode[PciNodeIndex].Function = (UINT8)-1;
        PciNodeIndex++;

        mDrhd.DeviceScopeNumber++;

        if (Is14nmCpu) {
          //
          // PC00, PC01, PC02, PC03, PC04, PC05 IOAPIC
          //
          if (mPlatformInfo->SysData.SysIoApicEnable & PC00_IOAPIC) {

              DevScope[DevIndex].DeviceType = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_IOAPIC;

              DevScope[DevIndex].EnumerationID = PC00_IOAPIC_ID;

              DevScope[DevIndex].StartBusNumber = IioBusBase;
              DevScope[DevIndex].PciNode = &PciNode[PciNodeIndex];
              PciNode[PciNodeIndex].Device = APIC_DEV_NUM;
              PciNode[PciNodeIndex].Function = APIC_FUNC_NUM;
              DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] Build DRHD DMI IOAPIC: Type %d, EnumId %d, StartBus 0x%x, PciNode %02X.%X\n",
                      SocketIndex, Stack,
                      DevScope[DevIndex].DeviceType, DevScope[DevIndex].EnumerationID, DevScope[DevIndex].StartBusNumber,
                      DevScope[DevIndex].PciNode->Device, DevScope[DevIndex].PciNode->Function));
              DevIndex++;
              PciNodeIndex++;
              PciNode[PciNodeIndex].Device = (UINT8)-1;
              PciNode[PciNodeIndex].Function = (UINT8)-1;
              PciNodeIndex++;

              mDrhd.DeviceScopeNumber++;
          }
        }
        HpetCapIdValue = *(UINT16 *)(UINTN)(HPET_BLOCK_ADDRESS);
        NumberOfHpets = (HpetCapIdValue >> 0x08) & 0x1F;  // Bits [8:12] contains the number of Hpets

        if (NumberOfHpets && (NumberOfHpets != 0x1f) &&
            (*((volatile UINT32 *)(UINTN)(HPET_BLOCK_ADDRESS + 0x100)) & BIT15)) {

            DevScope[DevIndex].DeviceType = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_MSI_CAPABLE_HPET;
            DevScope[DevIndex].EnumerationID = 00; //Hard-coded
            DevScope[DevIndex].StartBusNumber = (UINT8)PchInfoHob->HpetBusNum;
            DevScope[DevIndex].PciNode = &PciNode[PciNodeIndex];
            PciNode[PciNodeIndex].Device = (UINT8)PchInfoHob->HpetDevNum;
            PciNode[PciNodeIndex].Function = (UINT8)PchInfoHob->HpetFuncNum;
            DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d] Build DRHD HPET: Type %d, EnumId %d, StartBus 0x%x, PciNode %02X.%X\n",
                    SocketIndex, Stack,
                    DevScope[DevIndex].DeviceType, DevScope[DevIndex].EnumerationID, DevScope[DevIndex].StartBusNumber,
                    DevScope[DevIndex].PciNode->Device, DevScope[DevIndex].PciNode->Function));
            DevIndex++;
            PciNodeIndex++;
            PciNode[PciNodeIndex].Device = (UINT8)-1;
            PciNode[PciNodeIndex].Function = (UINT8)-1;
            PciNodeIndex++;

          mDrhd.DeviceScopeNumber++;
        }
      } // DmaRemap->InterruptRemap

      DmaRemap->InsertDmaRemap (DmaRemap, DrhdType, &mDrhd);

    } else { // End of if (IioSocketId == 0)

      if (FirstRun == 0) {
        ApicIndex = 0;
        for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
          //
          // Skip not valid stack
          //
          if (!IfStackPresent (0 ,Stack)) {
            continue;
          }
          ApicIndex++;
        }
        FirstRun = 1;
      }
      // Build DRHD on IIO1 - Stack0 to Stack5
      for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
        //
        // Skip not valid stack
        //
        if (!IfStackPresent (SocketIndex,Stack)) {
          continue;
        }
        BuildDRHDForStack (DmaRemap, SocketIndex, Stack, DevScope, PciNode, PciRootBridgePtr, ApicIndex);
        ApicIndex++;
      } //for( StackIndex=0; StackIndex<MAX_IIO_STACK ; StackIndex++) {
    } // End of if (IioSocketId == 0)

  } // End of for ( Index = 1; Index <= MAX_SOCKET; Index++)

  //
  // For SKX, AzaliaUseNonISOCH need to be 1 all the time since Isoch engine
  // will not be used for SKX and Azalia traffic always use non-Isoch VT-d engine.
  // HSD SKX Si : 1451396: Azalia Tag ID's used in the VTd engine
  // are not valid due to Azalia changes in Sunrisepoint
  //
  //
  // ATSR
  //
  pAtsr = GetDmarAtsrTablePointer ();
  if (DmaRemap->ATS) {
    for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {

      DEBUG((EFI_D_ERROR, "T_TEST: Build ATSR SocketIndex=%d.\n",SocketIndex));
      DEBUG((EFI_D_ERROR, "        IIO_resource.valid=%d.\n",mIioUds2->IioUdsPtr->PlatformData.IIO_resource[SocketIndex].Valid));

      if (SocketIndex >= MAX_SOCKET) {
        return EFI_INVALID_PARAMETER;
      }

      if (!SocketPresent (SocketIndex)) {
        continue;
      }

      IioBusBase = mIioUds2->IioUdsPtr->PlatformData.IIO_resource[SocketIndex].BusBase;

      if (mIioUds2->IioUdsPtr->PlatformData.CpuQpiInfo[SocketIndex].PcieSegment >= MAX_SOCKET) {
        return EFI_INVALID_PARAMETER;
      }
      PciRootBridgePtr = PciRootBridgeTab[mIioUds2->IioUdsPtr->PlatformData.CpuQpiInfo[SocketIndex].PcieSegment];

      PciNodeIndex            = 00;
      DevIndex                = 00;

      ZeroMem (DevScope, MEMORY_SIZE * sizeof (DEVICE_SCOPE));
      ZeroMem (PciNode, MEMORY_SIZE * sizeof (PCI_NODE));

      if (pAtsr != NULL) {
        pAtsr->Signature = ATSR_SIGNATURE;
        pAtsr->Flags = 00;
        pAtsr->SegmentNumber = mIioUds2->IioUdsPtr->PlatformData.CpuQpiInfo[SocketIndex].PcieSegment;
        pAtsr->DeviceScopeNumber = 00;
        pAtsr->DeviceScope = DevScope;
        pAtsr->ATSRPresentBit = (UINT32)-1;   // Not useful really Backwards project compatability (removwe it later)
      }

      //
      // Loop From Port 1 to 15 for Legacy IOH and 0 to 15 for Non-Legacy IOH
      //
      MaxPortNumberPerSocket = GetMaxPortPerSocket (SocketIndex);
      for (PciPortIndex = 1; PciPortIndex < MaxPortNumberPerSocket; PciPortIndex++)  {
        //
        // Check device IOTLBs supported or not in VT-d Extended capability register
        //
        Stack = GetStackPerPort (SocketIndex, PciPortIndex);
        // Check for a valid stack
        if (!(IfStackPresent (SocketIndex, Stack))) {
          DEBUG ((DEBUG_WARN, "[ACPI](DMAR) [%d.%d p%d] Stack not present\n", SocketIndex, Stack, PciPortIndex));
          continue;
        }

        VtdBase = GetVtdBar (SocketIndex, Stack);
        if (VtdBase != 0) {

          VtdMmioExtCap = *(volatile UINT64*)((UINTN)VtdBase + R_VTD_EXT_CAP_LOW);
          //
          // ATSR is applicable only for platform supporting device IOTLBs through the VT-d extended capability register
          //
          if ((VtdMmioExtCap & BIT2) != 0) {

            Bus = GetSocketPortBusNum (SocketIndex,PciPortIndex);
            Dev = 0;
            Func = 0;
            if (Is14nmCpu) {
              Dev = mDevScopeATSR[PciPortIndex].PciNode->Device;
              Func = mDevScopeATSR[PciPortIndex].PciNode->Function;
            } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
              Dev = mDevScopeATSR10nmD[PciPortIndex].PciNode->Device;
              Func = mDevScopeATSR10nmD[PciPortIndex].PciNode->Function;
            } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
              Dev = mDevScopeATSR_Icxd[PciPortIndex].PciNode->Device;
              Func = mDevScopeATSR_Icxd[PciPortIndex].PciNode->Function;
            } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
              Dev = mDevScopeATSR10nm[PciPortIndex].PciNode->Device;
              Func = mDevScopeATSR10nm[PciPortIndex].PciNode->Function;
            } else if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
              Dev = mDevScopeATSR_Spr[PciPortIndex].PciNode->Device;
              Func = mDevScopeATSR_Spr[PciPortIndex].PciNode->Function;
            }
// APTIOV_SERVER_OVERRIDE_RC_START: Replaced the usage of PciRootBridgeIo Read with IioPciExpressRead32 as the first RootBridge instance was directly used
// The first RootBridge instance will fail RootBridgeIoCheck for devices belonging to other Root Bridges
            //
            // Skip root ports which do not respond to PCI configuration cycles.
            //
            VidDid = 0;
/*            Status = PciRootBridgePtr->Pci.Read (
                      PciRootBridgePtr,
                      EfiPciWidthUint32,
                      EFI_PCI_ADDRESS (Bus, Dev, Func, 0),
                      1,
                      &VidDid);
            if (EFI_ERROR (Status) || VidDid == 0xffffffff) {*/
            VidDid = IioPciExpressRead32(SocketIndex, Bus, Dev, Func, PCI_VENDOR_ID_OFFSET);
            if (VidDid == 0xffffffff) {
              DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d p%d] %02X:%02X:%02X.%d Hidden (%X) - skip\n",
                      SocketIndex, Stack, PciPortIndex,
                      mIioUds2->IioUdsPtr->PlatformData.CpuQpiInfo[SocketIndex].PcieSegment,
                      Bus, Dev, Func, VidDid));
              continue;
            }
// APTIOV_SERVER_OVERRIDE_RC_END: Replaced the usage of PciRootBridgeIo Read with IioPciExpressRead32 as the first RootBridge instance was directly used
            if (IioVmdPortIsEnabled (SocketIndex, PciPortIndex) || GetCurrentPXPMap (SocketIndex, PciPortIndex) == 0) {

              DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d p%d] %a - skip\n", SocketIndex, Stack, PciPortIndex,
                      (GetCurrentPXPMap (SocketIndex, PciPortIndex) == 0) ? "Link width not set" : "Dummy VMD function"));
              continue;
            }
            if (GetPCIePortOwnerShip (SocketIndex, PciPortIndex) == NTB_PORT_DEF_NTB_NTB ||
                GetPCIePortOwnerShip (SocketIndex, PciPortIndex) == NTB_PORT_DEF_NTB_RP) {
              DevScope[DevIndex].DeviceType = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ENDPOINT;
            } else {
              DevScope[DevIndex].DeviceType = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE;
            }
            DevScope[DevIndex].EnumerationID      = 00;
            DevScope[DevIndex].StartBusNumber     = Bus;
            DevScope[DevIndex].PciNode            = &PciNode[PciNodeIndex];
            PciNode[PciNodeIndex].Device   = Dev;
            PciNode[PciNodeIndex].Function = Func;
            DEBUG ((DEBUG_INFO, "[ACPI](DMAR) [%d.%d p%d] Build DRHD PCI: Type %d, EnumId %d, StartBus 0x%x, PciNode %02X.%X\n",
                    SocketIndex, Stack, PciPortIndex,
                    DevScope[DevIndex].DeviceType, DevScope[DevIndex].EnumerationID, DevScope[DevIndex].StartBusNumber,
                    DevScope[DevIndex].PciNode->Device, DevScope[DevIndex].PciNode->Function));
            DevIndex++;
            PciNodeIndex++;
            PciNode[PciNodeIndex].Device    = (UINT8) -1;
            PciNode[PciNodeIndex].Function  = (UINT8) -1;
            PciNodeIndex++;
            if(pAtsr != NULL){
              pAtsr->DeviceScopeNumber++;
            }
          }// End of if ((VtdMmioExtCap & BIT2) != 0)
        } // End of if VtdBase
      } // for (PciPortIndex...)

      if (pAtsr != NULL){
        if (pAtsr->DeviceScopeNumber) {
          DmaRemap->InsertDmaRemap(DmaRemap, AtsrType, pAtsr);
        }
      }
    } // End of for (RootBridgeLoop = 0; RootBridgeLoop < mIioUds2->IioUdsPtr->PlatformData.numofIIO; RootBridgeLoop++)
  } // End of if (ATS) {

// APTIOV_SERVER_OVERRIDE_RC_START : For filling Reserved Base Address and Limit for RMRR structure of DMAR table
  #if defined AMIUSB_SUPPORT && AMIUSB_SUPPORT == 1
  	Status = gBS->LocateProtocol(&UsbProtocolGuid, NULL, &UsbProtocol);
  	if(!EFI_ERROR(Status))
  	{
  		Status = UsbProtocol->UsbGetRuntimeRegion(&StartAddress, &EndAddress);
  		DEBUG((EFI_D_ERROR, "The RMRR Mem Base is 0x%x\n", StartAddress));
  	}
  #endif

#if 0

  //
  // RMRR
  //
  AlignedSize  = (MEM_BLK_COUNT * sizeof(MEM_BLK));
  if( AlignedSize % 0x1000) {
    AlignedSize  = ( (MEM_BLK_COUNT * sizeof(MEM_BLK)) & (~0xfff) ) + 0x1000;
  } // aligend to 4k Boundary
  NumberofPages = AlignedSize/0x1000;
  //
  // Allocate memory (below 4GB)
  //
  Pointer = 0xffffffff;
  Status = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiReservedMemoryType,
                   NumberofPages,
                   &Pointer // Base address need to be 4K aligned for VT-d RMRR
                   );
  ASSERT_EFI_ERROR(Status);
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : For filling Reserved Base Address and Limit for RMRR structure of DMAR table

  if (DmaRemap->VTdSupport) {

// APTIOV_SERVER_OVERRIDE_RC_START : For filling Reserved Base Address and Limit for RMRR structure of DMAR table
    #if defined AMIUSB_SUPPORT && AMIUSB_SUPPORT == 1
    if ( StartAddress != 0 ) {
// APTIOV_SERVER_OVERRIDE_RC_END : For filling Reserved Base Address and Limit for RMRR structure of DMAR table
    //
    // RMRR
    //
    mRmrr.DeviceScope       = &DevScopeRmrr[0];
    // Calculate the right size of DevScope for mRmrr entry
    mRmrr.DeviceScopeNumber = sizeof(DevScopeRmrr)/sizeof(DEVICE_SCOPE);
// APTIOV_SERVER_OVERRIDE_RC_START
    // Use USB Runtime Start and End Address
    mRmrr.RsvdMemBase       = (UINT64)StartAddress;
    mRmrr.RsvdMemLimit      = EndAddress;
    DEBUG ((EFI_D_INFO, "[ACPI](DMAR) RMRR Base 0x%llX, Limit 0x%llX\n", mRmrr.RsvdMemBase, mRmrr.RsvdMemLimit));
// APTIOV_SERVER_OVERRIDE_RC_END
    DmaRemap->InsertDmaRemap (DmaRemap, RmrrType, &mRmrr);
	ReportDmarHookForSpecialCard (mIioUds2, DmaRemap);
// APTIOV_SERVER_OVERRIDE_RC_START
    }
    #endif

  //OEM VTD RMRR support
        #if defined OemVtdRmrr_SUPPORT && OemVtdRmrr_SUPPORT == 1
        {
            EFI_OEM_VTD_RMRR_PROTOCOL *OemVtdRmrrProtocol;
            static EFI_GUID gEfiOemVtdRmrrProtocolGuid = EFI_OEM_VTD_RMRR_PROTOCOL_GUID;

            Status = gBS->LocateProtocol(&gEfiOemVtdRmrrProtocolGuid, NULL, &OemVtdRmrrProtocol);
            DEBUG((EFI_D_ERROR, "LocateProtocol gEfiOemVtdRmrrProtocolGuid. Status: %r \n", Status));
            if( !EFI_ERROR(Status) ) {
              Status = OemVtdRmrrProtocol->InsertOemVtdRmrr (DmaRemap);
              DEBUG((EFI_D_ERROR, "OemVtdRmrrProtocol->InsertOemVtdRmrr Status: %r \n", Status));
            }
        }
        #endif
  }
  // Adding RHSA structure in DMAR table
  Status = GetOptionData(
                 &gEfiSocketCommonRcVariableGuid,
                 OFFSET_OF(SOCKET_COMMONRC_CONFIGURATION, NumaEn),
                 &NumaEnb,
                 sizeof(NumaEnb));
  if ((!EFI_ERROR(Status)) && NumaEnb) {
     //
     //Initializing default values
     //
     mRhsa.Signature = RHSA_SIGNATURE;
     mRhsa.RhsaCount = 0;
     for ( ; SocketIndx < MAX_SOCKET; SocketIndx++) {
       for (StackIndx = 0; StackIndx < MAX_IIO_STACK; StackIndx++) {
         if (mIioUds2->IioUdsPtr->PlatformData.CpuQpiInfo[SocketIndx].stackPresentBitmap & (1 << StackIndx)) {
            DEBUG((EFI_D_INFO, "Reporting DMAR RHSA For Socket = %x Stack = %x\n", SocketIndx, StackIndx));
            BuildRhsaDmar(DmaRemap, SocketIndx, StackIndx);
         }
       }
     }
  }
// APTIOV_SERVER_OVERRIDE_RC_END
  gBS->FreePool (PciNode);
  gBS->FreePool (DevScope);

  return EFI_SUCCESS;
}


/**
  Install ACPI DMAR table for VT-d.

  This function needs gEfiPciIoProtocolGuid so it can run only after PCI Enumeraion is complete.

  @retval EFI_SUCCESS          DMAR installed successfuly.
  @retval EFI_NOT_FOUND        gEfiPciIoProtocolGuid or gEfiDmaRemapProtocolGuid not found.
  @retval EFI_OUT_OF_RESOURCES Could not allocate resources.
**/
EFI_STATUS
AcpiVtdTablesInstall (
  VOID
  )
{
  EFI_STATUS                  Status;
  EFI_ACPI_TABLE_VERSION      TableVersion;
  EFI_DMA_REMAP_PROTOCOL      *DmaRemap;
  UINTN                       TableHandle;
  EFI_ACPI_COMMON_HEADER      *CurrentTable;
  EFI_ACPI_TABLE_PROTOCOL     *AcpiTable;

  UINTN                       HandleCount;
  EFI_HANDLE                  *HandleBuffer;
  UINTN                       Index;
  EFI_PCI_IO_PROTOCOL         *PciIo;
  PCI_TYPE01                  PciConfigHeader;
  UINTN                       Segment;
  UINTN                       Bus;
  UINTN                       Device;
  UINTN                       Function;
  UINT8                       PciExpressOffset;
  UINT32                      AcsOffset;
  UINT16                      PciExpressCapabilityReg;
  UINT8                       AcsCapCount;
  UINT16                      RequiredAcsCap;
  UINT32                      AcsCapRegValue;
  UINT16                      AcsConRegValue;
  USRA_PCIE_ADDR_TYPE         *AcsDevArray;
#ifdef ACS_WA
  UINT8                       RootBridgeLoop;
  UINT32                      VtdBase;
  UINT8                       CORE20_BUSNUM=0;
#endif
  USRA_ADDRESS                Address;
  BOOLEAN                     CheckAcsControl = 0;

  PciExpressOffset = 0;
  AcsOffset        = 0;
  AcsCapCount      = 0;
  AcsCapRegValue   = 0;
  AcsConRegValue   = 0;
  RequiredAcsCap   =  ACS_SOURCE_VALIDATION | ACS_P2P_REQUEST_REDIRECT | ACS_P2P_COMPLETION_REDIRECT | ACS_UPSTREAM_FORWARDING;

  //
  // Locate all PciIo protocols
  //
  Status = gBS->LocateHandleBuffer (
               ByProtocol,
               &gEfiPciIoProtocolGuid,
               NULL,
               &HandleCount,
               &HandleBuffer
               );
  if (EFI_ERROR (Status)) {

    DEBUG((DEBUG_ERROR, "[ACPI](DMAR) ERROR: Cannot locate gEfiPciIoProtocolGuid (%r)\n", Status));
    ASSERT (FALSE);
    return Status;
  }
  AcsDevArray = AllocateZeroPool (sizeof (USRA_PCIE_ADDR_TYPE) * HandleCount);
  if (AcsDevArray == NULL) {
    ASSERT (AcsDevArray != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < HandleCount; Index ++) {

    gBS->HandleProtocol (HandleBuffer[Index], &gEfiPciIoProtocolGuid, &PciIo);
    PciIo->Pci.Read (PciIo, EfiPciIoWidthUint32, 0, sizeof(PciConfigHeader) / sizeof(UINT32), &PciConfigHeader);
    if ((PciConfigHeader.Hdr.ClassCode[0] == 0x00 || PciConfigHeader.Hdr.ClassCode[0] == 0x01) && PciConfigHeader.Hdr.ClassCode[1] == 0x04 && PciConfigHeader.Hdr.ClassCode[2] == 0x06) {
      //
      // 060400h or 060401h indicates it's PCI-PCI bridge, get its bus number, device number and function number
      //
      CheckAcsControl = CheckACSControlandBusValue(PciConfigHeader.Bridge.PrimaryBus);
      if(CheckAcsControl){
        continue;
      }

      PciIo->GetLocation (PciIo, &Segment, &Bus, &Device, &Function);

      USRA_PCIE_SEG_ADDRESS(Address, UsraWidth16, Segment, Bus, Device, Function, 0);

      if (PciConfigHeader.Hdr.Status == EFI_PCI_STATUS_CAPABILITY) {
        //
        // the bridge support Capability list and offset 0x34 is the pointer to the data structure
        //
        // Detect if PCI Express Device
        //
        Status = LocateCapRegBlock (PciIo, EFI_PCI_CAPABILITY_ID_PCIEXP, &PciExpressOffset, NULL);

        if (Status == EFI_SUCCESS) {
          //
          // this bridge device is a PCI Express bridge
          // Check if it is downstream port of PCIE switch
          //
          Address.Pcie.Offset = PciExpressOffset + EFI_PCI_EXPRESS_CAPABILITY_REGISTER;
          RegisterRead(&Address, &PciExpressCapabilityReg);

          //
          // BIT 7:4 indicate Device/port type, 0110b indicates downstream port of PCI express switch
          //
          if ((PciExpressCapabilityReg & 0x00F0) == 0x60) {
            //
            // it is downstream port of PCI Express switch
            // Look for ACS capability register in PCI express configuration space
            //
            Status = LocatePciExpressCapRegBlock (PciIo, EFI_PCIE_CAPABILITY_ID_ACS, &AcsOffset, NULL);
            DEBUG((EFI_D_ERROR, "ACS capable port is B%x.D%x.F%x - ACS Cap offset - 0x%x\n", Bus, Device, Function, AcsOffset));

            if (Status == EFI_SUCCESS) {
              //
              // Read ACS capability register
              //
              Address.Pcie.Offset = AcsOffset + ACS_CAPABILITY_REGISTER;
              Address.Attribute.AccessWidth = UsraWidth32;
              RegisterRead(&Address, &AcsCapRegValue);
              DEBUG((EFI_D_INFO, "Bus =%x, Device=%x, Function=%x, AcsCapRegValue = %x \n", Bus, Device, Function, AcsCapRegValue));

              if ((AcsCapRegValue & RequiredAcsCap) == RequiredAcsCap) {
                //
                // The PCI express downstream port support ACS, record this port
                //
                AcsDevArray[AcsCapCount].Bus = (UINT32)Bus;
                AcsDevArray[AcsCapCount].Dev = (UINT32)Device;
                AcsDevArray[AcsCapCount].Func = (UINT32)Function;
                AcsDevArray[AcsCapCount].Offset = AcsOffset;
                AcsDevArray[AcsCapCount].Seg = (UINT32)Segment;
                AcsCapCount++;
#ifdef ACS_WA
              } else {
                //
                // Even though this PCIe switch downstream port support ACS, but it does not meet all the required capabilities, so also disable the VT-d
                //
                mDisableVtd = TRUE;
                break;
              }
            } else {
              //
              // At least one PCIe switch downstream port does not support ACS, just disable the VT-d report
              //
              mDisableVtd = TRUE;
              break;
            }
#else
          }
        }
#endif
          }
      }
    }
  }
  }  /// End for

  //
  //Free the Handle buffer
  //
  if (HandleBuffer != NULL) {
    gBS->FreePool (HandleBuffer);
  }

  ASSERT (AcsCapCount <= HandleCount);

#ifdef ACS_WA
  if (mDisableVtd == TRUE ) {
    //
    // at least one PCIE downstream port does not support ACS so need to disable VT-d
    //
    //
    // Disable VT-d BAR
    //
    USRA_PCIE_SEG_ADDRESS(Address, UsraWidth32, Segment, Bus, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IIO_VTBAR);
    for ( RootBridgeLoop =0; RootBridgeLoop < mIioUds2->IioUdsPtr->PlatformData.numofIIO; RootBridgeLoop++) {
      if(!mIioUds2->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].Valid) {
        continue;
      }
      VtdBase = mIioUds2->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].VtdBarAddress & (~B_IIO_VT_BAR_EN);
      Address.Pcie.Bus = mIioUds2->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].BusBase;
      RegisterWrite(&Address, &VtdBase);
    }

    //
    // Disable Setup option of VT-d
    //
    mSocketIioConfiguration.VtdAcsWa = FALSE;

    Status = SetOptionData (&gEfiSocketIioVariableGuid, OFFSET_OF (SOCKET_IIO_CONFIGURATION, VtdAcsWa), mSocketIioConfiguration.VtdAcsWa, sizeof(mSocketIioConfiguration.VtdAcsWa));

    //
    // Do not report DMAR table to OS/VMM then OS/VMM have no knowledge of VT-d and VT-d2
    //
    FreePool (AcsDevArray);
    return EFI_NOT_FOUND;
  } else
#endif
  {
    //
    // all PCI express switch downstream ports support ACS and meet the required ACS capabilities
    // for each downstream ports, enable the required Capabilities in ACS control register
    //
    Address.Attribute.AccessWidth = UsraWidth16;
    for (Index = 0; Index < AcsCapCount; Index ++) {
      //
      // Program the corresponding bits in ACS control register
      //
      Address.Pcie = AcsDevArray[Index];
      Address.Pcie.Offset += ACS_CONTROL_REGISTER;
      RegisterRead(&Address, &AcsConRegValue);
      DEBUG((EFI_D_ERROR, "AcsConRegValue is 0x%x\n", AcsConRegValue));
      AcsConRegValue |= (ACS_SOURCE_VALIDATION_ENABLE|ACS_P2P_REQUEST_REDIRECT_ENABLE|ACS_P2P_COMPLETION_REDIRECT_ENABLE|ACS_UPSTREAM_FORWARDING_ENABLE);
      DEBUG((EFI_D_ERROR, "After Enable BITs AcsConRegValue is 0x%x\n", AcsConRegValue));
      RegisterWrite(&Address, &AcsConRegValue);
      //
      // report VT-d and other features to OS/VMM, report DMAR and remapping engine to OS/VMM
      //
    } //
  }
  //
  // Find the AcpiSupport protocol
  //
  Status = LocateSupportProtocol (&gEfiAcpiTableProtocolGuid, gEfiAcpiTableStorageGuid, &AcpiTable, FALSE);
  ASSERT_EFI_ERROR (Status);

  TableVersion = EFI_ACPI_TABLE_VERSION_2_0;

  Status = gBS->LocateProtocol (&gEfiDmaRemapProtocolGuid, NULL, &DmaRemap);
  if (EFI_ERROR(Status)) {

    DEBUG ((DEBUG_ERROR, "[ACPI](DMAR) ERROR: Cannot locate gEfiDmaRemapProtocolGuid (%r)\n", Status));

  } else {

    if (DmaRemap->VTdSupport) {

      ReportDmar (DmaRemap);
      Status = DmaRemap->GetDmarTable (DmaRemap, &CurrentTable);

      if (EFI_ERROR (Status)) {

        ASSERT_EFI_ERROR (Status);

      } else {
        //
        // Perform any table specific updates.
        //
        Status = PlatformUpdateTables (CurrentTable, &TableVersion);
        ASSERT_EFI_ERROR (Status);

        TableHandle = 0;
        Status = AcpiTable->InstallAcpiTable (
                            AcpiTable,
                            CurrentTable,
                            CurrentTable->Length,
                            &TableHandle
                            );
        ASSERT_EFI_ERROR (Status);
      }
    }
  }
  FreePool (AcsDevArray);

  return EFI_SUCCESS;
}


/**
    GC_TODO: add routine description

    @param PciIo            - GC_TODO: add arg description
    @param CapID            - GC_TODO: add arg description
    @param PciExpressOffset - GC_TODO: add arg description
    @param NextRegBlock     - GC_TODO: add arg description

    @retval EFI_SUCCESS   - GC_TODO: add retval description
    @retval EFI_NOT_FOUND - GC_TODO: add retval description
**/
EFI_STATUS
LocateCapRegBlock(
  IN     EFI_PCI_IO_PROTOCOL  *PciIo,
  IN     UINT8                CapID,
  OUT    UINT8                *PciExpressOffset,
  OUT    UINT8                *NextRegBlock)
{
  UINT16  CapabilityID;
  UINT32  Temp;
  UINT8   CapabilityPtr;
  UINT16  CapabilityEntry;

  PciIo->Pci.Read (
            PciIo,
            EfiPciIoWidthUint32,
            PCI_CAPBILITY_POINTER_OFFSET,
            1,
            &Temp
            );

  CapabilityPtr = (UINT8)Temp;
  //
  // According to the PCI spec a value of 0x00
  // is the end of the list
  //
  while (CapabilityPtr >= 0x40) {
    //
    // Mask it to DWORD alignment per PCI spec
    //
    CapabilityPtr &= 0xFC;
    PciIo->Pci.Read (
               PciIo,
               EfiPciIoWidthUint16,
               CapabilityPtr,
               1,
               &CapabilityEntry
               );

    CapabilityID = (UINT8) CapabilityEntry;

    if (CapabilityID == CapID) {
      *PciExpressOffset = CapabilityPtr;
      if (NextRegBlock != NULL) {
        *NextRegBlock = (UINT8) ((CapabilityEntry >> 8) & 0xFC);
      }

      return EFI_SUCCESS;
    }

    CapabilityPtr = (UINT8) ((CapabilityEntry >> 8) & 0xFC);
  }

  return EFI_NOT_FOUND;
}


/**

    GC_TODO: add routine description

    @param PciIo        - GC_TODO: add arg description
    @param CapID        - GC_TODO: add arg description
    @param Offset       - GC_TODO: add arg description
    @param NextRegBlock - GC_TODO: add arg description

    @retval EFI_SUCCESS   - GC_TODO: add retval description
    @retval EFI_NOT_FOUND - GC_TODO: add retval description

**/
EFI_STATUS
LocatePciExpressCapRegBlock (
  IN     EFI_PCI_IO_PROTOCOL  *PciIo,
  IN     UINT16               CapID,
  OUT    UINT32               *Offset,
  OUT    UINT32               *NextRegBlock
)
{
  UINT32  CapabilityPtr;
  UINT32  CapabilityEntry;
  UINT16  CapabilityID;

  CapabilityPtr = EFI_PCIE_CAPABILITY_BASE_OFFSET;

  while ((CapabilityPtr != 0) && (CapabilityPtr < 0x1000)) {
    //
    // Mask it to DWORD alignment per PCI spec
    //
    CapabilityPtr &= 0xFFC;
    PciIo->Pci.Read (
               PciIo,
               EfiPciIoWidthUint32,
               CapabilityPtr,
               1,
               &CapabilityEntry
               );

    CapabilityID = (UINT16) CapabilityEntry;
    // APTIOV_SERVER_OVERRIDE_RC_START : Check if Extended PciExpress Config. Space is accessible.
    if(CapabilityID == 0xFFFF) break;
    //APTIOV_SERVER_OVERRIDE_RC_END : Check if Extended PciExpress Config. Space is accessible.

    if (CapabilityID == CapID) {
      *Offset = CapabilityPtr;
      if (NextRegBlock != NULL) {
        *NextRegBlock = (CapabilityEntry >> 20) & 0xFFF;
      }

      return EFI_SUCCESS;
    }

    CapabilityPtr = (CapabilityEntry >> 20) & 0xFFF;
  }

  return EFI_NOT_FOUND;
}


/**
    GC_TODO: add routine description
**/
VOID
DisableAriForwarding (
  VOID
  )
{
  EFI_STATUS                  Status;
  UINTN                       HandleCount;
  EFI_HANDLE                  *HandleBuffer;
  UINTN                       Index;
  EFI_PCI_IO_PROTOCOL         *PciIo;
  PCI_TYPE01                  PciConfigHeader;
  UINTN                       Segment;
  UINTN                       Bus;
  UINTN                       Device;
  UINTN                       Function;
  UINT8                       PciExpressOffset;
  PCI_REG_PCIE_DEVICE_CONTROL2 DevCtl2;
  // APTIOV_SERVER_OVERRIDE_RC_START : Do not disable ARI forwarding on PCI Express rev. 1.0 bridge.
  UINT16                      PcieCap;  
  // APTIOV_SERVER_OVERRIDE_RC_END : Do not disable ARI forwarding on PCI Express rev. 1.0 bridge.

  //
  // Disable ARI forwarding before handoff to OS, as it may not be ARI-aware
  //
  //
  // ARI forwarding exist in bridge
  //

  //
  // Locate all PciIo protocol
  //
  Status = gBS->LocateHandleBuffer (
               ByProtocol,
               &gEfiPciIoProtocolGuid,
               NULL,
               &HandleCount,
               &HandleBuffer
               );
  for (Index = 0; Index < HandleCount; Index ++) {
    gBS->HandleProtocol (
          HandleBuffer[Index],
          &gEfiPciIoProtocolGuid,
          &PciIo
          );
    PciIo->Pci.Read (
                PciIo,
                EfiPciIoWidthUint32,
                0,
                sizeof (PciConfigHeader) / sizeof (UINT32),
                &PciConfigHeader
                );
    if ((PciConfigHeader.Hdr.ClassCode[0] == 0x00 || PciConfigHeader.Hdr.ClassCode[0] == 0x01) && PciConfigHeader.Hdr.ClassCode[1] == 0x04 && PciConfigHeader.Hdr.ClassCode[2] == 0x06) {
      //
      // 060400h or 060401h indicates it's PCI-PCI bridge, get its bus number, device number and function number
      //
      PciIo->GetLocation (
              PciIo,
              &Segment,
              &Bus,
              &Device,
              &Function
              );
      if (PciConfigHeader.Hdr.Status == EFI_PCI_STATUS_CAPABILITY) {
        //
        // the bridge support Capability list and offset 0x34 is the pointer to the data structure
        //
        //
        // Detect if PCI Express Device
        //
        Status = LocateCapRegBlock (PciIo, EFI_PCI_CAPABILITY_ID_PCIEXP, &PciExpressOffset, NULL);
        if (Status == EFI_SUCCESS) {

// APTIOV_SERVER_OVERRIDE_RC_START : Do not disable ARI forwarding on PCI Express rev. 1.0 bridge.
          PciIo->Pci.Read (
                  PciIo,
                  EfiPciIoWidthUint16,
                  PciExpressOffset + 0x02,
                  1,
                  &PcieCap
                  );
          //
          // Check PCIe Capability Version
          // ARI Forwarding is not available in PCIe rev 1.0.
          //
          if ((PcieCap & 0x0F) < 2) continue;
// APTIOV_SERVER_OVERRIDE_RC_END : Do not disable ARI forwarding on PCI Express rev. 1.0 bridge.

          //
          // this bridge device is a PCI Express bridge, Check ARI forwarding bit in Device Control 2 register
          //
          PciIo->Pci.Read (
                  PciIo,
                  EfiPciIoWidthUint16,
                  PciExpressOffset + OFFSET_OF (PCI_CAPABILITY_PCIEXP, DeviceControl2),
                  1,
                  &DevCtl2
                  );
          if (DevCtl2.Bits.AriForwarding) {
            //
            // ARI forwarding enable bit is set, we need to clear this bit before handing off control to OS
            // because OS may not ARI aware
            //
            DEBUG((DEBUG_INFO, "[VTD] %02X:%02X:%02X.%X: ARI forwarding disable before booting OS, DevCtl2 0x%02X -> 0x%02X\n",
                   Segment, Bus, Device, Function, DevCtl2.Uint16, DevCtl2.Uint16 & ~BIT5));
            DevCtl2.Bits.AriForwarding = 0;
            PciIo->Pci.Write (
                  PciIo,
                  EfiPciIoWidthUint16,
                  PciExpressOffset + OFFSET_OF (PCI_CAPABILITY_PCIEXP, DeviceControl2),
                  1,
                  &DevCtl2
                  );
          }
        }
      }
    }
  }
} // DisableAriForwarding()

