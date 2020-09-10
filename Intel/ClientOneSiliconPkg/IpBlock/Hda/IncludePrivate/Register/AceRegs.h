/** @file
  Register names for High Definition Audio device.

  Conventions:

  - Register definition format:
    Prefix_[GenerationName]_[ComponentName]_SubsystemName_RegisterSpace_RegisterName
  - Prefix:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register size
    Definitions beginning with "N_" are the bit position
  - [GenerationName]:
    Three letter acronym of the generation is used (e.g. SKL,KBL,CNL etc.).
    Register name without GenerationName applies to all generations.
  - [ComponentName]:
    This field indicates the component name that the register belongs to (e.g. PCH, SA etc.)
    Register name without ComponentName applies to all components.
    Register that is specific to -H denoted by "_PCH_H_" in component name.
    Register that is specific to -LP denoted by "_PCH_LP_" in component name.
  - SubsystemName:
    This field indicates the subsystem name of the component that the register belongs to
    (e.g. PCIE, USB, SATA, GPIO, PMC etc.).
  - RegisterSpace:
    MEM - MMIO space register of subsystem.
    IO  - IO space register of subsystem.
    PCR - Private configuration register of subsystem.
    CFG - PCI configuration space register of subsystem.
  - RegisterName:
    Full register name.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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

@par Specification Reference:
**/
#ifndef _ACE_HDA_REGS_H_
#define _ACE_HDA_REGS_H_

#define R_ACE_HSTCFG_VID                             0x00
#define R_ACE_HSTCFG_DID                             0x02
#define R_ACE_HSTCFG_CMD                             0x04
#define R_ACE_HSTCFG_STS                             0x06
#define R_ACE_HSTCFG_PI                              0x09
#define R_ACE_HSTCFG_SCC                             0x0A
#define R_ACE_HSTCFG_BCC                             0x0B
#define R_ACE_HSTCFG_CLS                             0x0C
#define R_ACE_HSTCFG_HDALBA                          0x10
#define B_ACE_HSTCFG_HDALBA_LBA                      0xFFFFF000
#define R_ACE_HSTCFG_HDAUBA                          0x14
#define R_ACE_HSTCFG_DSPLBA                          0x20
#define B_ACE_HSTCFG_DSPLBA_LBA                      0xFFE00000
#define R_ACE_HSTCFG_DSPUBA                          0x24
#define R_ACE_HSTCFG_INTPN                           0x3D
#define R_ACE_HSTCFG_PID                             0x50
#define R_ACE_HSTCFG_PC                              0x52
#define V_ACE_HSTCFG_PC_PMES                         0x18
#define N_ACE_HSTCFG_PC_PMES                         11

#define R_ACE_HSTCFG_PCS                             0x54

#define R_ACE_HSTCFG_SPCE                            0xD2
#define B_ACE_HSTCFG_SPCE_HAE                        BIT5
#define B_ACE_HSTCFG_SPCE_SE                         BIT3
#define B_ACE_HSTCFG_SPCE_D3HE                       BIT2
#define B_ACE_HSTCFG_SPCE_I3E                        BIT1
#define B_ACE_HSTCFG_SPCE_PMCRE                      BIT0

#define R_ACE_HSTMEM_GCTL                            0x0008
#define B_ACE_HSTMEM_GCTL_CRSTB                      BIT0

#define R_ACE_HSTMEM_WAKEEN                          0x000C
#define B_ACE_HSTMEM_WAKEEN_SDI_3                    BIT3
#define B_ACE_HSTMEM_WAKEEN_SDI_2                    BIT2
#define B_ACE_HSTMEM_WAKEEN_SDI_1                    BIT1
#define B_ACE_HSTMEM_WAKEEN_SDI_0                    BIT0

#define R_ACE_HSTMEM_WAKESTS                         0x000E

#define B_ACE_HSTMEM_WAKESTS_SDIN3                   BIT3
#define B_ACE_HSTMEM_WAKESTS_SDIN2                   BIT2
#define B_ACE_HSTMEM_WAKESTS_SDIN1                   BIT1
#define B_ACE_HSTMEM_WAKESTS_SDIN0                   BIT0

#define R_ACE_HSTMEM_HDAICR                          0x60
#define R_ACE_HSTMEM_HDAICR_IC                       R_ACE_HSTMEM_HDAICR + 0x0
#define R_ACE_HSTMEM_HDAICR_IR                       R_ACE_HSTMEM_HDAICR + 0x4
#define R_ACE_HSTMEM_HDAICR_ICS                      R_ACE_HSTMEM_HDAICR + 0x8
#define B_ACE_HSTMEM_HDAICR_ICS_ICB                  BIT0
#define B_ACE_HSTMEM_HDAICR_ICS_IRV                  BIT1

#define R_ACE_HSTMEM_TSOCFGH                         0x1C00

#define R_ACE_HSTMEM_TSOCFGH_TTCCFG                  R_ACE_HSTMEM_TSOCFGH + 0x14
#define B_ACE_HSTMEM_TSOCFGH_TTCCFG_HCDT             BIT1

#define R_ACE_HSTMEM_TSOCFGH_CDCCFG                  R_ACE_HSTMEM_TSOCFGH + 0x34
#define B_ACE_HSTMEM_TSOCFGH_CDCCFG_DIS_SDIN0        BIT0
#define B_ACE_HSTMEM_TSOCFGH_CDCCFG_DIS_SDIN1        BIT1

#define R_ACE_HSTMEM_TSOCFGH_EM1                     R_ACE_HSTMEM_TSOCFGH + 0x40
#define B_ACE_HSTMEM_TSOCFGH_EM1_BBRK                (BIT31 | BIT30)
#define V_ACE_HSTMEM_TSOCFGH_EM1_64B                 0x0
#define V_ACE_HSTMEM_TSOCFGH_EM1_128B                0x1
#define N_ACE_HSTMEM_TSOCFGH_EM1_BBRK                30
#define B_ACE_HSTMEM_TSOCFGH_EM1_LFLCS               BIT24
#define B_ACE_HSTMEM_TSOCFGH_EM1_BLKC3DIS            BIT17
#define B_ACE_HSTMEM_TSOCFGH_EM1_TMODE               BIT12
#define B_ACE_HSTMEM_TSOCFGH_EM1_FIFORDYSEL          (BIT10 | BIT9)
#define B_ACE_HSTMEM_TSOCFGH_EM1_NODEID              0xFF
#define B_ACE_HSTMEM_TSOCFGH_EM1_ETMODE              (BIT28 | BIT27)
#define V_ACE_HSTMEM_TSOCFGH_EM1_ETMODE_1T_2T        0x0
#define V_ACE_HSTMEM_TSOCFGH_EM1_ETMODE_4T           0x1
#define V_ACE_HSTMEM_TSOCFGH_EM1_ETMODE_8T           0x2
#define V_ACE_HSTMEM_TSOCFGH_EM1_ETMODE_16T          0x3
#define N_ACE_HSTMEM_TSOCFGH_EM1_ETMODE              27
#define B_ACE_HSTMEM_TSOCFGH_EM1_ECCWRBKD            BIT29

#define R_ACE_HSTMEM_TSOCFGH_EM2                     R_ACE_HSTMEM_TSOCFGH + 0x44
#define B_ACE_HSTMEM_TSOCFGH_EM2_BSMT                (BIT27 | BIT26)
#define V_ACE_HSTMEM_TSOCFGH_EM2_BSMT                0x1
#define N_ACE_HSTMEM_TSOCFGH_EM2_BSMT                26
#define B_ACE_HSTMEM_TSOCFGH_EM2_VC0SNR              BIT24
#define B_ACE_HSTMEM_TSOCFGH_EM2_DUM                 BIT23
#define B_ACE_HSTMEM_TSOCFGH_EM2_CBL1EXEN            BIT12

#define R_ACE_HSTMEM_TSOCFGH_EM3L                    R_ACE_HSTMEM_TSOCFGH + 0x48
#define B_ACE_HSTMEM_TSOCFGH_EM3L_ISL1EXT2           (BIT21 | BIT20)
#define V_ACE_HSTMEM_TSOCFGH_EM3L_ISL1EXT2           0x2
#define N_ACE_HSTMEM_TSOCFGH_EM3L_ISL1EXT2           20

#define R_ACE_HSTMEM_TSOCFGH_EM3U                    R_ACE_HSTMEM_TSOCFGH + 0x4C

#define R_ACE_HSTMEM_TSOCFGH_EM4L                    R_ACE_HSTMEM_TSOCFGH + 0x50
#define B_ACE_HSTMEM_TSOCFGH_EM4L_OSL1EXT2           (BIT21 | BIT20)
#define V_ACE_HSTMEM_TSOCFGH_EM4L_OSL1EXT2           0x3
#define N_ACE_HSTMEM_TSOCFGH_EM4L_OSL1EXT2           20

#define R_ACE_HSTMEM_TSOCFGH_EM4U                    R_ACE_HSTMEM_TSOCFGH + 0x54

#define R_ACE_HSTMEM_HDAPP                           0x0800
#define R_ACE_HSTMEM_HDAPP_PPCH                      R_ACE_HSTMEM_HDAPP + 0x00
#define R_ACE_HSTMEM_HDAPP_PPCTL                     R_ACE_HSTMEM_HDAPP + 0x04
#define B_ACE_HSTMEM_HDAPP_PPCTL_GPROCEN             BIT30

#define V_ACE_HDALINK_INDEX                          0
#define V_ACE_IDISPLINK_INDEX                        1

#define R_ACE_HSTMEM_HDAML                           0x0C00
#define R_ACE_HSTMEM_HDAML_LCTLX(x)                  (R_ACE_HSTMEM_HDAML + (0x40 + (0x40 * (x)) + 0x04)) // x - Link index: 0 - HDA Link, 1 - iDisp Link
#define B_ACE_HSTMEM_HDAML_LCTLX_CPA                 BIT23
#define B_ACE_HSTMEM_HDAML_LCTLX_SPA                 BIT16
#define N_ACE_HSTMEM_HDAML_LCTLX_SCF                 0
#define V_ACE_HSTMEM_HDAML_LCTLX_SCF_6MHZ            0x0
#define V_ACE_HSTMEM_HDAML_LCTLX_SCF_12MHZ           0x1
#define V_ACE_HSTMEM_HDAML_LCTLX_SCF_24MHZ           0x2
#define V_ACE_HSTMEM_HDAML_LCTLX_SCF_48MHZ           0x3
#define V_ACE_HSTMEM_HDAML_LCTLX_SCF_96MHZ           0x4

#define R_ACE_HSTMEM_SOCCI                           0x1C80
#define R_ACE_HSTMEM_SOCCI_HSTRVAL                   R_ACE_HSTMEM_SOCCI + 0x10
#define B_ACE_HSTMEM_SOCCI_HSTRVAL_XOCFS             (BIT1 | BIT0)
#define N_ACE_HSTMEM_SOCCI_HSTRVAL_XOCFS             0
#define V_ACE_HSTMEM_SOCCI_HSTRVAL_XOCFS_24MHZ       0x0
#define V_ACE_HSTMEM_SOCCI_HSTRVAL_XOCFS_38_4MHZ     0x1
#define V_ACE_HSTMEM_SOCCI_HSTRVAL_XOCFS_24_576MHZ   0x2
#define V_ACE_HSTMEM_SOCCI_HSTRVAL_XOCFS_19_2MHZ     0x3

#define R_ACE_HSTMEM_SOCCI_HDAHWI1                   R_ACE_HSTMEM_SOCCI + 0x1C
#define B_ACE_HSTMEM_SOCCI_HDAHWI1_INPAY             0xFFFF0000
#define B_ACE_HSTMEM_SOCCI_HDAHWI1_OUTPAY            0xFFFF
#define N_ACE_HSTMEM_SOCCI_HDAHWI1_INPAY             0
#define N_ACE_HSTMEM_SOCCI_HDAHWI1_OUTPAY            16
#define V_ACE_HSTMEM_SOCCI_HDAHWI1_INPAY_DEFAULT     0x1C
#define V_ACE_HSTMEM_SOCCI_HDAHWI1_OUTPAY_DEFAULT    0x3C

#define R_ACE_HSTMEM_TSOCFGU                         0x1E00
#define R_ACE_HSTMEM_TSOCFGU_PTDC                    R_ACE_HSTMEM_TSOCFGU + 0x28
#define B_ACE_HSTMEM_TSOCFGU_PTDC_SRMIW              (BIT6 | BIT5 | BIT4)
#define N_ACE_HSTMEM_TSOCFGU_PTDC_SRMIW              4
#define V_ACE_HSTMEM_TSOCFGU_PTDC_SRMIW_4AON         0x0
#define V_ACE_HSTMEM_TSOCFGU_PTDC_SRMIW_8AON         0x1
#define V_ACE_HSTMEM_TSOCFGU_PTDC_SRMIW_16AON        0x2
#define V_ACE_HSTMEM_TSOCFGU_PTDC_SRMIW_32AON        0x3
#define V_ACE_HSTMEM_TSOCFGU_PTDC_SRMIW_64AON        0x4
#define V_ACE_HSTMEM_TSOCFGU_PTDC_SRMIW_128AON       0x5
#define V_ACE_HSTMEM_TSOCFGU_PTDC_SRMIW_256AON       0x6
#define V_ACE_HSTMEM_TSOCFGU_PTDC_SRMIW_512AON       0x7

#define R_ACE_HFPCICFG                               0x1C80
#define R_ACE_HFPCICFG_HWI0                          R_ACE_HFPCICFG + 0x20
#define R_ACE_HFPCICFG_HWI0_PI                       R_ACE_HFPCICFG + 0x1
#define V_ACE_HFPCICFG_HWI0_PI_UAA                   0x80
#define V_ACE_HFPCICFG_HWI0_PI_NON_UAA               0x0

#define R_ACE_HFPCICFG_HWI0_SCC                      R_ACE_HFPCICFG + 0x2
#define V_ACE_HFPCICFG_HWI0_SCC_UAA                  0x3
#define V_ACE_HFPCICFG_HWI0_SCC_NON_UAA              0x1

#define R_ACE_HFPCICFG_HWI0_BCC                      R_ACE_HFPCICFG + 0x3
#define V_ACE_HFPCICFG_HWI0_BCC_UAA                  0x4
#define V_ACE_HFPCICFG_HWI0_BCC_NON_UAA              0x4

#define R_ACE_HFPCICFG_HWIS                          R_ACE_HFPCICFG + 0x30
#define B_ACE_HFPCICFG_HWIS_PI                       BIT2
#define B_ACE_HFPCICFG_HWIS_SCC                      BIT3
#define B_ACE_HFPCICFG_HWIS_BCC                      BIT4

#define R_ACE_DSPMEM_HFDSSCS                         0x1000
#define B_ACE_DSPMEM_HFDSSCS_SPA                     BIT16
#define B_ACE_DSPMEM_HFDSSCS_CPA                     BIT24

#define R_ACE_DSPMEM_TSOCFGU                         0x1E00
#define R_ACE_DSPMEM_TSOCFGU_FNCFG                   R_ACE_DSPMEM_TSOCFGU + 0x30
#define B_ACE_DSPMEM_TSOCFGU_FNCFG_ACED              BIT0
#define B_ACE_DSPMEM_TSOCFGU_FNCFG_DSPSD             BIT2
#define B_ACE_DSPMEM_TSOCFGU_FNCFG_CGD               BIT3
#define B_ACE_DSPMEM_TSOCFGU_FNCFG_BCLD              BIT4
#define B_ACE_DSPMEM_TSOCFGU_FNCFG_PGD               BIT5

#define R_ACE_DSPMEM_SNDWSx                          0x38000
#define R_ACE_DSPMEM_SNDWSx_SNDWLCAP                 R_ACE_DSPMEM_SNDWSx + 0x00
#define B_ACE_DSPMEM_SNDWSx_SNDWLCAP_LCOUNT          (BIT2 | BIT1 | BIT0)
#define R_ACE_DSPMEM_SNDWSx_SNDWLCTL                 R_ACE_DSPMEM_SNDWSx + 0x04
#define N_ACE_DSPMEM_SNDWSx_SNDWLCTL_SPA             0
#define B_ACE_DSPMEM_SNDWSx_SNDWLCTL_SPA(x)          (BIT0 << ((x) + N_ACE_DSPMEM_SNDWSx_SNDWLCTL_SPA))
#define N_ACE_DSPMEM_SNDWSx_SNDWLCTL_CPA             8
#define B_ACE_DSPMEM_SNDWSx_SNDWLCTL_CPA(x)          (BIT0 << ((x) + N_ACE_DSPMEM_SNDWSx_SNDWLCTL_CPA))
#define R_ACE_DSPMEM_SNDWSx_SNDWIPPTR                R_ACE_DSPMEM_SNDWSx + 0x08
#define B_ACE_DSPMEM_SNDWSx_SNDWIPPTR_PRT            0xFFFFF

#define R_ACE_DSPMEM_SNDWSx_SNDWxCTLSCAP(x)          R_ACE_DSPMEM_SNDWSx + (0x60 * (x)) + 0x10
#define R_ACE_DSPMEM_SNDWSx_SNDWxCTLS0CM(x)          R_ACE_DSPMEM_SNDWSx + (0x60 * (x)) + 0x12
#define R_ACE_DSPMEM_SNDWSx_SNDWxCTLS1CM(x)          R_ACE_DSPMEM_SNDWSx + (0x60 * (x)) + 0x14
#define R_ACE_DSPMEM_SNDWSx_SNDWxCTLS2CM(x)          R_ACE_DSPMEM_SNDWSx + (0x60 * (x)) + 0x16
#define R_ACE_DSPMEM_SNDWSx_SNDWxCTLS3CM(x)          R_ACE_DSPMEM_SNDWSx + (0x60 * (x)) + 0x18
#define R_ACE_DSPMEM_SNDWSx_SNDWxIOCTL(x)            R_ACE_DSPMEM_SNDWSx + (0x60 * (x)) + 0x6C
#define B_ACE_DSPMEM_SNDWSx_SNDWxIOCTL_WPDD          BIT6
#define B_ACE_DSPMEM_SNDWSx_SNDWxIOCTL_BKE           BIT5
#define B_ACE_DSPMEM_SNDWSx_SNDWxIOCTL_DOE           BIT4
#define B_ACE_DSPMEM_SNDWSx_SNDWxIOCTL_DO            BIT3
#define B_ACE_DSPMEM_SNDWSx_SNDWxIOCTL_COE           BIT2
#define B_ACE_DSPMEM_SNDWSx_SNDWxIOCTL_MIF           BIT0
#define R_ACE_DSPMEM_SNDWSx_SNDWxACTMCTL(x)          R_ACE_DSPMEM_SNDWSx + (0x60 * (x)) + 0x6E
#define B_ACE_DSPMEM_SNDWSx_SNDWxACTMCTL_DACTQE      BIT0
#define R_ACE_DSPMEM_SNDWSx_SNDWWAKEEN               0x190
#define B_ACE_DSPMEM_SNDWSx_SNDWWAKEEN_PWE           (BIT3 | BIT2 | BIT1 | BIT0)
#define R_ACE_DSPMEM_SNDWSx_SNDWWAKESTS              0x192
#define B_ACE_DSPMEM_SNDWSx_SNDWWAKESTS_PWS          (BIT3 | BIT2 | BIT1 | BIT0)

#define R_ACE_DSPMEM_ADSPIC2                         0x10
#define B_ACE_DSPMEM_ADSPIC2_SNDW                    BIT5

#endif
