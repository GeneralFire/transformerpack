/** @file
  Register definitions for machine check registers defined in
  IA32 Software Developer's Manual.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#ifndef __PCIE_REG_DEF_H__
#define __PCIE_REG_DEF_H__

#define DMI_DEV_CTL_OFFSET                                        0xf0
#define   B_DMI_CORRECTABLE_ERROR_DETECTED                        BIT0
#define   B_DMI_NON_FATAL_ERROR_DETECTED                          BIT1
#define DMI_DEV_STS_OFFSET                                        0xf2
#define   B_DMI_FATAL_ERROR_DETECTED                              BIT2
#define PCIE_DMI_ARE_OFFSET                                       0x148
#define PCIE_CORRECTABLE_ERROR_COUNT_LIMIT                        10
#define PCI_EXP_CAP_SLCAP                                         0x14                 // Slot capabilities

#define R_PCIE_EXT_CAP_HEADER                                     0x148

// capabilities list parsing support
#define  DWORD_MSK                                                3 // BIT1 | BIT0
#define  EXCAPABILITIES_MSK                                       0xFFFF
#define  CAP_OFFSET_BASE                                          0x40
#define   V_PCIE_CAPABILITY_DPT_ROOT_PORT                         0x40
#define   V_PCIE_CAPABILITY_DPT_DOWNSTREAM_PORT                   0x60

#define PCIE_CAPABILITY_LIST_OFFSET                               0x00
#define PCIE_CAPABILITY_OFFSET                                    0x02
#define PCIE_DEVICE_CAPABILITY_OFFSET                             0x04
#define   ROLE_BASED_ERROR_REPORTING                              BIT15
#define PCIE_DEVICE_CONTROL_OFFSET                                0x08
#define   B_PCIE_DEVICE_CONTROL_COR_ERR_EN                        BIT0
#define   B_PCIE_DEVICE_CONTROL_NF_ERR_EN                         BIT1
#define   B_PCIE_DEVICE_CONTROL_FAT_ERR_EN                        BIT2
#define   B_PCIE_DEVICE_CONTROL_UNSUP_REQ_EN                      BIT3
#define PCIE_DEVICE_STATUS_OFFSET                                 0x0A
#define   B_PCIE_DEVICE_STATUS_COR_ERR                            BIT0
#define   B_PCIE_DEVICE_STATUS_NF_ERR                             BIT1
#define   B_PCIE_DEVICE_STATUS_FAT_ERR                            BIT2
#define   B_PCIE_DEVICE_STATUS_UNSUP_REQ                          BIT3
#define PCIE_LINK_CAPABILITY_OFFSET                               0x0C
#define PCIE_LINK_CONTROL_OFFSET                                  0x10
#define PCIE_LINK_STATUS_OFFSET                                   0x12
#define PCIE_SLOT_CAPABILITY_OFFSET                               0x14
#define PCIE_SLOT_CONTROL_OFFSET                                  0x18
#define PCIE_SLOT_STATUS_OFFSET                                   0x1A
#define PCIE_ROOT_CONTROL_OFFSET                                  0x1C
#define   B_PCIE_ROOT_CONTROL_COR_ERR_EN                          BIT0
#define   B_PCIE_ROOT_CONTROL_NF_ERR_EN                           BIT1
#define   B_PCIE_ROOT_CONTROL_FAT_ERR_EN                          BIT2
#define PCIE_ROOT_STATUS_OFFSET                                   0x20
#define PCIE_DEVICE_CAPABILITY2_OFFSET                            0x24
#define PCIE_DEVICE_CONTROL2_OFFSET                               0x28

//
// AER Register offset...
//
#define PCIE_AER_OFFSET_REG_UES                                   0x04
#define   B_PCIE_AER_UES_POI_TLP_EGR_BLK_ERR                      BIT26
#define   B_PCIE_AER_UES_TLP_PRE_BLK_ERR                          BIT25
#define   B_PCIE_AER_UES_ATO_EGR_BLK_ERR                          BIT24
#define   B_PCIE_AER_UES_MC_BLK_TLP_ERR                           BIT23
#define   B_PCIE_AER_UES_UNC_INT_ERR                              BIT22
#define   B_PCIE_AER_UES_ACS_VIO_ERR                              BIT21
#define   B_PCIE_AER_UES_UNS_REQ_ERR                              BIT20
#define   B_PCIE_AER_UES_ECRC_ERR                                 BIT19
#define   B_PCIE_AER_UES_MAL_TLP_ERR                              BIT18
#define   B_PCIE_AER_UES_RCV_OVR_ERR                              BIT17
#define   B_PCIE_AER_UES_UNE_CPL_ERR                              BIT16
#define   B_PCIE_AER_UES_CPL_ABT_ERR                              BIT15
#define   B_PCIE_AER_UES_CTO_ERR                                  BIT14
#define   B_PCIE_AER_UES_FLW_CTL_PRO_ERR                          BIT13
#define   B_PCIE_AER_UES_POI_TLP_ERR                              BIT12
#define   B_PCIE_AER_UES_SUPRISE_LINK_DOWN_ERR                    BIT5
#define   B_PCIE_AER_UES_DAT_LNK_ERR                              BIT4
#define PCIE_AER_OFFSET_REG_UEM                                   0x08
#define PCIE_AER_OFFSET_REG_UESEV                                 0x0C
#define   B_PCIE_AER_UESEV_CTO_SEV                                BIT14
#define PCIE_AER_OFFSET_REG_CES                                   0x10
#define   B_PCIE_AER_CES_HLOG_OVRF_ERR                            BIT15
#define   B_PCIE_AER_CES_COR_INT_ERR                              BIT14
#define   B_PCIE_AER_CES_ADV_NON_FAT_ERR                          BIT13
#define   B_PCIE_AER_CES_REP_TMR_TOT_ERR                          BIT12
#define   B_PCIE_AER_CES_REP_NUM_ROV_ERR                          BIT8
#define   B_PCIE_AER_CES_BAD_DLLP_ERR                             BIT7
#define   B_PCIE_AER_CES_BAD_TLP_ERR                              BIT6
#define   B_PCIE_AER_CES_RCV_ERR                                  BIT0

#define PCIE_AER_OFFSET_REG_CEM                                   0x14
#define PCIE_AER_OFFSET_REG_AECC                                  0x18
#define   B_PCIE_AER_AECC_ECRC_GEN_CAP                            BIT5
#define   B_PCIE_AER_AECC_ECRC_GEN_EN                             BIT6
#define   B_PCIE_AER_AECC_ECRC_CHK_CAP                            BIT7
#define   B_PCIE_AER_AECC_ECRC_CHK_EN                             BIT8
#define PCIE_AER_OFFSET_REG_REC                                   0x2c
#define PCIE_AER_OFFSET_REG_RES                                   0x30
#define   B_PCIE_AER_ROOT_ERR_STS_COR                             BIT0
#define   B_PCIE_AER_ROOT_ERR_STS_MUL_COR                         BIT1
#define   B_PCIE_AER_ROOT_ERR_STS_FAT_NONFAT                      BIT2
#define   B_PCIE_AER_ROOT_ERR_STS_MUL_FAT_NONFAT                  BIT3
#define   B_PCIE_AER_ROOT_ERR_STS_FIR_UNC_FAT                     BIT4
#define PCIE_AER_OFFSET_REG_ERRSID                                0x34

#define NO_ERROR         0
#define SINGLE_ERROR     BIT0
#define MULTIPLE_ERRORS  BIT0 + BIT1

#define PCIEAER_CORRERR_MSK                                       0x0000f1c1
#define PCIEAER_UNCORRERR_MSK                                     0x03fff030

//
// PCI Base Specification Class Code values for PCI Bridge devices
//
#define V_PCIE_CCR_PCI_TO_PCI_BRIDGE                              0x060400
#define V_PCIE_CCR_PCI_HOST_BRIDGE                                0x060000
#define V_PCIE_CCR_PCI_OTHER_BRIDGE                               0x068000
#define V_PCIE_CCR_PCI_OTHER_SYS_PERIPHERAL                       0x088000

//
// DPC Capability
//
#define PCI_EXPRESS_EXTENDED_CAPABILITY_DOWNSTREAM_PORT_CONTAINMENT_ID  0x001D

#define PCIE_DPC_OFFSET_REG_CAP                                   0x04
#define PCIE_DPC_OFFSET_REG_CTL                                   0x06
#define   B_PCIE_DPC_CTL_TRG_EN                                   (BIT0 + BIT1)
#define   B_PCIE_DPC_CTL_INT_EN                                   BIT3
#define   B_PCIE_DPC_CTL_COR_MSG_EN                               BIT4
#define PCIE_DPC_OFFSET_REG_STS                                   0x08
#define   B_PCIE_DPC_STS_DPC_TRIGGER_STS                          BIT0

//
// Legacy PCI status
//

#define EFI_PCI_STATUS_RECEIVED_TARGET_ABORT                      BIT12 // 0x1000
#define EFI_PCI_STATUS_SIGNALED_SYSTEM_ERROR                      BIT14 // 0x4000
#define EFI_PCI_STATUS_DETECTED_PARITY_ERROR                      BIT15 // 0x8000

//
// RCEC definitions
//
#define ABMRCIEP_OFFSET 4
#define ABNRCIEP_OFFSET 8

#define INTEL_PCI_VENDOR_ID                                       0x8086

#endif
