/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef __SOCKET_CONFIGURATION_H__
#define __SOCKET_CONFIGURATION_H__

#include <Guid/MemBootHealthGuid.h>

#define SOCKET_CONFIG_CLASS_ID          44
#define SOCKET_CONFIG_SUBCLASS_ID       0x00

#define VFR_BLANK_LINE subtitle text = STRING_TOKEN(STR_NULL_STRING);
#define VFR_END_FORM endform;
#define VFR_END_FORMSET endformset;
#define VFR_HORIZ_LINE subtitle text = STRING_TOKEN(STR_HORIZONTAL_LINE);

#define SOCKET_IIO_CONFIG_KEY             0xFAFB
#define SOCKET_COMMONRC_CONFIG_KEY        0xFAFA
#define SOCKET_MP_LINK_CONFIG_KEY         0xFAF9
#define SOCKET_MEMORY_CONFIG_KEY          0xFAF8
#define SOCKET_MISC_CONFIG_KEY            0xFAF7
#define SOCKET_CONFIG_KEY                 0xF00A
#define SOCKET_POWERMANAGEMENT_CONFIGURATION_KEY 0xF00B
#define SOCKET_PROCESSORCORE_CONFIGURATION_KEY   0xF00C

#define KEY_XMP_PROFILE                   0x5DC0
#define KEY_XMP_PROFILE1                  0x5DC1
#define KEY_CPU_ONLINE                    0x50B8
#define KEY_EIST_ENABLE                   0x5D7D
#define KEY_FLEXIBLE_RATIO                0x5D7E
#define KEY_HYPER_THREADING_DISABLE       0x5D7F
#define KEY_CORE_DISABLE_MASK_SOCKET_0    0x5D80
#define KEY_X2APIC_ENABLE                 0x5D81

#define KEY_SNC_EN                        0x4F00
#define KEY_UMA_BASED_CLUSTERING          0x4F01
#define KEY_VOL_MEM_MODE                  0x4F02
#define KEY_XPT_PREFETCH_EN               0x4F03
#define KEY_NGN_FACTORY_CLEAN             0x4F04
#define KEY_AVG_POWER_CHANGE              0x4F05
#define KEY_AVG_POWER_TIME_CONST_CHANGE   0x4F06
#define KEY_MBB_MAX_POWER_CHANGE          0x4F07

#define KEY_SGX_ENABLE                    0x4E01
#define KEY_SGX_OOB                       0x4E02
#define KEY_SGX_PACKAGE_IB                0x4E03
#define KEY_SGX_PRMRR_SIZE                0x4E04
#define KEY_SGX_QOS                       0x4E05
#define KEY_SGX_OWNER_EPOCH               0x4E06
#define KEY_SGX_OWNER_EPOCH0              0x4E07
#define KEY_SGX_OWNER_EPOCH1              0x4E08
#define KEY_SGX_LEWR                      0x4E09
#define KEY_SGX_SGXLEPUBKEYHASH0          0x4E0A
#define KEY_SGX_SGXLEPUBKEYHASH1          0x4E0B
#define KEY_SGX_SGXLEPUBKEYHASH2          0x4E0C
#define KEY_SGX_SGXLEPUBKEYHASH3          0x4E0D
#define KEY_SGX_DEBUG_MODE                0x4E0E
#define KEY_SGX_AUTO_REGISTRATION_AGENT   0x4E0F
#define KEY_SGX_FACTORY_RESET             0x4E10
#define KEY_SGX_PRM_SIZE                  0x4E11

#define KEY_MKTME_INTEGRITY               0x4E30
// FuSa (SAF)
#define KEY_DFX_FUSASAF_ENABLE            0x4E50
// max chars which can be placed in DfxSafImgLoad;
#define FUSASAF_MAX_STRING                50

#define SGX_SUBOPTION(key) (\
      (key) == KEY_SGX_OOB           ||\
      (key) == KEY_SGX_PACKAGE_IB    ||\
      (key) == KEY_SGX_PRMRR_SIZE    ||\
      (key) == KEY_SGX_QOS           ||\
      (key) == KEY_SGX_OWNER_EPOCH   ||\
      (key) == KEY_SGX_OWNER_EPOCH0  ||\
      (key) == KEY_SGX_OWNER_EPOCH1  ||\
      (key) == KEY_SGX_LEWR          ||\
      (key) == KEY_SGX_SGXLEPUBKEYHASH0 ||\
      (key) == KEY_SGX_SGXLEPUBKEYHASH1 ||\
      (key) == KEY_SGX_SGXLEPUBKEYHASH2 ||\
      (key) == KEY_SGX_SGXLEPUBKEYHASH3 ||\
      (key) == KEY_SGX_DEBUG_MODE       ||\
      (key) == KEY_SGX_AUTO_REGISTRATION_AGENT ||\
      (key) == KEY_DFX_FUSASAF_ENABLE ||\
      (key) == KEY_SGX_PRM_SIZE)

// {26A69232-ABF8-4597-8876-A7DC0A7CA602}
#define SOCKET_CONFIG_SETUP_GUID { 0x26a69232, 0xabf8, 0x4597, { 0x88, 0x76, 0xa7, 0xdc, 0xa, 0x7c, 0xa6, 0x2 } }



#define VFR_FORMID_SOCKET                   0x510
#define VFR_FORMID_COMMONRC                 0x511
#define VFR_FORMID_KTI                      0x512
#define VFR_FORMID_PROCESSOR                0x513
#define VFR_FORMID_KTI_STATUS               0x514
#define VFR_FORMID_DFX_COMMONRC             0x515

#define VFR_FORMID_SOCKET_IIO_CONFIGURATION 0x517

#define VFR_FORMID_IIO                  0x518
#define VFR_FORMID_IIO2                 0x519
#define VFR_FORMID_IIO3                 0x51A
#define VFR_FORMID_IIO4                 0x51B
#define VFR_FORMID_IIO5                 0x51C
#define VFR_FORMID_IIO6                 0x51D
#define VFR_FORMID_IIO7                 0x51E
#define VFR_FORMID_PWRMGT               0x51F

//
// KTI Form for SV
//

#define VFR_FORMID_KTI_GENERAL          0x521
#define VFR_FORMID_KTISYSTEMWIDE        0x522
#define VFR_FORMID_KTISYSTEM_PHY_LINK   0x523
#define VFR_FORMID_KTISYSTEM_OSB        0x524
#define VFR_FORMID_KTIPERSOCKET         0x525
#define VFR_FID_KTI_CPU0                0x526
#define VFR_FID_KTI_CPU1                0x527
#define VFR_FID_KTI_CPU2                0x528
#define VFR_FID_KTI_CPU3                0x529
#define VFR_FID_KTI_CPU4                0x52A
#define VFR_FID_KTI_CPU5                0x52B
#define VFR_FID_KTI_CPU6                0x52C
#define VFR_FID_KTI_CPU7                0x52D
//
// KTI Form IDs
//
#define VFR_FORMID_CPU_KTII             0x530
#define VFR_FID_KTI_STATUS              0x531
#define VFR_FID_KTI_TOPOLOGY            0x532
#define VFR_FID_KTI_TOPOLOGY_MATRIX     0x533
#define VFR_FID_IIO_DEV_HIDE            0x534
#define VFR_FID_KTI_SOCKET_RES          0x535
#define VFR_FID_KTI_SOCKET_RES_REQUEST  0x536
#define VFR_FID_KTI_SOCKET_RES_STATUS   0x537
#define VFR_FORMID_MEMORY               0x540
#define VFR_FORMID_MEMORY_RAS           0x541
#define VFR_FID_KTI_CPU0_LINK0          0x542
#define VFR_FID_KTI_CPU0_LINK1          0x543
#define VFR_FID_KTI_CPU0_LINK2          0x544
#define VFR_FID_KTI_CPU1_LINK0          0x545
#define VFR_FID_KTI_CPU1_LINK1          0x546
#define VFR_FID_KTI_CPU1_LINK2          0x547
#define VFR_FID_KTI_CPU2_LINK0          0x548
#define VFR_FID_KTI_CPU2_LINK1          0x549
#define VFR_FID_KTI_CPU2_LINK2          0x54A
#define VFR_FID_KTI_CPU3_LINK0          0x54B
#define VFR_FID_KTI_CPU3_LINK1          0x54C
#define VFR_FID_KTI_CPU3_LINK2          0x54D
#define VFR_FID_KTI_CPU4_LINK0          0x54E
#define VFR_FID_KTI_CPU4_LINK1          0x54F
#define VFR_FORMID_BRANCH_RANK_CONFIG       0x571
#define VFR_FORMID_VALHOOKS_CONFIG          0x572
#define VFR_FORMID_THERMTHRT_CONFIG         0x573
#define VFR_FORMID_MEMTOPOLOGY_DISPLAY      0x574
#define VFR_FORMID_PAGE_POLICY_DISPLAY      0x57D
#define VFR_FORMID_MEMORY_TRAINING_DISPLAY  0x57E
#define VFR_FORMID_MEM_PWR_SAVE_ADV_ID      0x57F
#define VFR_FORMID_CKE_DISPLAY              0x59F
#define VFR_FORMID_SREF_DISPLAY             0x5A0
#define VFR_FORMID_MEM_THERMAL_ID           0x580
#define VFR_FORMID_XMP_DISPLAY              0x581
#define VFR_FORMID_MEM_PM_CFG_ID            0x582
#define VFR_FORMID_TWO_WAY_NM_CACHE         0x583
#define VFR_FORMID_DFX_XOR_DECODER          0x584
#define VFR_FORMID_MEM_MAP                  0x58A
#define VFR_FORMID_RAPL                     0x58B
#define VFR_FORMID_MEMORY_NGN               0x58C
#define VFR_FORMID_MEMORY_DFX           0x58D
#define VFR_FORMID_SECURE_ERASE         0x58E
#define VFR_FORMID_MEMORY_BIOS_SSA_CONFIG_MENU      0x58F
#define VFR_FID_KTI_CPU4_LINK2          0x590
#define VFR_FID_KTI_CPU5_LINK0          0x591
#define VFR_FID_KTI_CPU5_LINK1          0x592
#define VFR_FID_KTI_CPU5_LINK2          0x593
#define VFR_FID_KTI_CPU6_LINK0          0x594
#define VFR_FID_KTI_CPU6_LINK1          0x595
#define VFR_FID_KTI_CPU6_LINK2          0x596
#define VFR_FID_KTI_CPU7_LINK0          0x597
#define VFR_FID_KTI_CPU7_LINK1          0x598
#define VFR_FID_KTI_CPU7_LINK2          0x599
#define VFR_FID_KTI_CPU0_LINK3          0x59A
#define VFR_FID_KTI_CPU0_LINK4          0x59B
#define VFR_FID_KTI_CPU0_LINK5          0x59C
#define VFR_FID_KTI_CPU1_LINK3          0x59D
#define VFR_FID_KTI_CPU1_LINK4          0x59E
#define VFR_FID_KTI_CPU1_LINK5          0x5B0
#define VFR_FID_KTI_CPU2_LINK3          0x5B1
#define VFR_FID_KTI_CPU2_LINK4          0x5B2
#define VFR_FID_KTI_CPU2_LINK5          0x5B3
#define VFR_FID_KTI_CPU3_LINK3          0x5B4
#define VFR_FID_KTI_CPU3_LINK4          0x5B5
#define VFR_FID_KTI_CPU3_LINK5          0x5B6
#define VFR_FID_KTI_CPU4_LINK3          0x5B7
#define VFR_FID_KTI_CPU4_LINK4          0x5B8
#define VFR_FID_KTI_CPU4_LINK5          0x5B9
#define VFR_FID_KTI_CPU5_LINK3          0x5BA
#define VFR_FID_KTI_CPU5_LINK4          0x5BB
#define VFR_FID_KTI_CPU5_LINK5          0x5BC
#define VFR_FID_KTI_CPU6_LINK3          0x5BD
#define VFR_FID_KTI_CPU6_LINK4          0x5BE
#define VFR_FID_KTI_CPU6_LINK5          0x5BF
#define VFR_FID_KTI_CPU7_LINK3          0x5C0
#define VFR_FID_KTI_CPU7_LINK4          0x5C1
#define VFR_FID_KTI_CPU7_LINK5          0x5C2

#define VFR_FORMID_CMI_INIT             0x600
#define VFR_FORMID_MEMORY_HEALTH_CHECK  0x601
//
// KTI Form for Dfx
//
#define VFR_FORMID_KTI_DFX              0x550
#define VFR_FID_KTI_DFX_CPU0_LINK0      0x551
#define VFR_FID_KTI_DFX_CPU0_LINK1      0x552
#define VFR_FID_KTI_DFX_CPU0_LINK2      0x553
#define VFR_FID_KTI_DFX_CPU1_LINK0      0x554
#define VFR_FID_KTI_DFX_CPU1_LINK1      0x555
#define VFR_FID_KTI_DFX_CPU1_LINK2      0x556
#define VFR_FID_KTI_DFX_CPU2_LINK0      0x557
#define VFR_FID_KTI_DFX_CPU2_LINK1      0x558
#define VFR_FID_KTI_DFX_CPU2_LINK2      0x559
#define VFR_FID_KTI_DFX_CPU3_LINK0      0x55A
#define VFR_FID_KTI_DFX_CPU3_LINK1      0x55B
#define VFR_FID_KTI_DFX_CPU3_LINK2      0x55C
#define VFR_FID_KTI_DFX_CPU4_LINK0      0x55D
#define VFR_FID_KTI_DFX_CPU4_LINK1      0x55E
#define VFR_FID_KTI_DFX_CPU4_LINK2      0x55F
#define VFR_FID_KTI_DFX_CPU5_LINK0      0x560
#define VFR_FID_KTI_DFX_CPU5_LINK1      0x561
#define VFR_FID_KTI_DFX_CPU5_LINK2      0x562
#define VFR_FID_KTI_DFX_CPU6_LINK0      0x563
#define VFR_FID_KTI_DFX_CPU6_LINK1      0x564
#define VFR_FID_KTI_DFX_CPU6_LINK2      0x565
#define VFR_FID_KTI_DFX_CPU7_LINK0      0x566
#define VFR_FID_KTI_DFX_CPU7_LINK1      0x567
#define VFR_FID_KTI_DFX_CPU7_LINK2      0x568
#define VFR_FID_KTI_DFX_CPU0_LINK3      0x569
#define VFR_FID_KTI_DFX_CPU0_LINK4      0x56a
#define VFR_FID_KTI_DFX_CPU0_LINK5      0x56b
#define VFR_FID_KTI_DFX_CPU1_LINK3      0x56c
#define VFR_FID_KTI_DFX_CPU1_LINK4      0x56d
#define VFR_FID_KTI_DFX_CPU1_LINK5      0x56e
#define VFR_FID_KTI_DFX_CPU2_LINK3      0x56f
#define VFR_FID_KTI_DFX_CPU2_LINK4      0x5C3
#define VFR_FID_KTI_DFX_CPU2_LINK5      0x5C4
#define VFR_FID_KTI_DFX_CPU3_LINK3      0x5C5
#define VFR_FID_KTI_DFX_CPU3_LINK4      0x5C6
#define VFR_FID_KTI_DFX_CPU3_LINK5      0x5C7
#define VFR_FID_KTI_DFX_CPU4_LINK3      0x5C8
#define VFR_FID_KTI_DFX_CPU4_LINK4      0x5C9
#define VFR_FID_KTI_DFX_CPU4_LINK5      0x5CA
#define VFR_FID_KTI_DFX_CPU5_LINK3      0x5CB
#define VFR_FID_KTI_DFX_CPU5_LINK4      0x5CC
#define VFR_FID_KTI_DFX_CPU5_LINK5      0x5CD
#define VFR_FID_KTI_DFX_CPU6_LINK3      0x5CE
#define VFR_FID_KTI_DFX_CPU6_LINK4      0x5CF
#define VFR_FID_KTI_DFX_CPU6_LINK5      0x5D0
#define VFR_FID_KTI_DFX_CPU7_LINK3      0x5D1
#define VFR_FID_KTI_DFX_CPU7_LINK4      0x5D2
#define VFR_FID_KTI_DFX_CPU7_LINK5      0x5D3

#define VFR_FORMID_KTI_DFX_PER_SOCKET   0x5D4
#define VFR_DFX_KTI_CPU0                0x5D5
#define VFR_DFX_KTI_CPU1                0x5D6
#define VFR_DFX_KTI_CPU2                0x5D7
#define VFR_DFX_KTI_CPU3                0x5D8
#define VFR_DFX_KTI_CPU4                0x5D9
#define VFR_DFX_KTI_CPU5                0x5DA
#define VFR_DFX_KTI_CPU6                0x5DB
#define VFR_DFX_KTI_CPU7                0x5DC

//
// MEMORY Form IDs
//
#define VFR_FORMID_HBM                  0x430

//
// IIO Form IDs
//
#define VFR_FORMID_IIO_IOAPIC           0x450
#define VFR_FORMID_VTD                  0x451
#define VFR_FORMID_PCIE                 0x452
#define VFR_FORMID_PCIE_IIO0P0          0x453
#define VFR_FORMID_PCIE_IIO0P1          0x454
#define VFR_FORMID_PCIE_IIO0P2          0x455
#define VFR_FORMID_PCIE_IIO0P3          0x456
#define VFR_FORMID_PCIE_IIO0P4          0x457
#define VFR_FORMID_PCIE_IIO0P5          0x458
#define VFR_FORMID_PCIE_IIO0P6          0x459
#define VFR_FORMID_PCIE_IIO0P7          0x45A
#define VFR_FORMID_PCIE_IIO0P8          0x45B
#define VFR_FORMID_PCIE_IIO0P9          0x45C
#define VFR_FORMID_PCIE_IIO0P10         0x45D
#define VFR_FORMID_PCIE_IIO0P11         0x45E
#define VFR_FORMID_PCIE_IIO0P12         0x45F
#define VFR_FORMID_PCIE_IIO0P13         0x460
#define VFR_FORMID_PCIE_IIO0P14         0x461
#define VFR_FORMID_PCIE_IIO0P15         0x488
#define VFR_FORMID_PCIE_IIO0P16         0x489
#define VFR_FORMID_PCIE_IIO0P17         0x48A
#define VFR_FORMID_PCIE_IIO0P18         0x48B
#define VFR_FORMID_PCIE_IIO0P19         0x48C
#define VFR_FORMID_PCIE_IIO0P20         0x48D
#define VFR_FORMID_PCIE_IIO0P21         0x4A0
#define VFR_FORMID_PCIE_IIO0P22         0x4A1
#define VFR_FORMID_PCIE_IIO0P23         0x4A2
#define VFR_FORMID_PCIE_IIO0P24         0x4A3
#define VFR_FORMID_PCIE_IIO0P25         0x4A4
#define VFR_FORMID_PCIE_IIO0P26         0x4A5
#define VFR_FORMID_PCIE_IIO0P27         0x4A6
#define VFR_FORMID_PCIE_IIO0P28         0x4A7
#define VFR_FORMID_PCIE_IIO0P29         0x4A8
#define VFR_FORMID_PCIE_IIO0P30         0x4A9
#define VFR_FORMID_PCIE_IIO0P31         0x4AA
#define VFR_FORMID_PCIE_IIO0P32         0x4AB
#define VFR_FORMID_PCIE_IIO0P33         0x4AC
#define VFR_FORMID_PCIE_IIO0P34         0x4AD
#define VFR_FORMID_PCIE_IIO0P35         0x4AE
#define VFR_FORMID_PCIE_IIO0P36         0x4AF
#define VFR_FORMID_PCIE_IIO0P37         0x4B0
#define VFR_FORMID_PCIE_IIO0P38         0x4B1
#define VFR_FORMID_PCIE_IIO0P39         0x4B2
#define VFR_FORMID_PCIE_IIO0P40         0x4B3

#define VFR_FORMID_PCIE_IIO1P0          0x462
#define VFR_FORMID_PCIE_IIO1P1          0x463
#define VFR_FORMID_PCIE_IIO1P2          0x469
#define VFR_FORMID_PCIE_IIO1P3          0x46A
#define VFR_FORMID_PCIE_IIO1P4          0x46B
#define VFR_FORMID_PCIE_IIO1P5          0x46C
#define VFR_FORMID_PCIE_IIO1P6          0x46D
#define VFR_FORMID_PCIE_IIO1P7          0x46E
#define VFR_FORMID_PCIE_IIO1P8          0x46F
#define VFR_FORMID_PCIE_IIO1P9          0x470
#define VFR_FORMID_PCIE_IIO1P10         0x475
#define VFR_FORMID_PCIE_IIO1P11         0x476
#define VFR_FORMID_PCIE_IIO1P12         0x477
#define VFR_FORMID_PCIE_IIO1P13         0x478
#define VFR_FORMID_PCIE_IIO1P14         0x479
#define VFR_FORMID_PCIE_IIO1P15         0x48E
#define VFR_FORMID_PCIE_IIO1P16         0x48F
#define VFR_FORMID_PCIE_IIO1P17         0x490
#define VFR_FORMID_PCIE_IIO1P18         0x491
#define VFR_FORMID_PCIE_IIO1P19         0x492
#define VFR_FORMID_PCIE_IIO1P20         0x493
#define VFR_FORMID_PCIE_IIO1P21         0x4B4
#define VFR_FORMID_PCIE_IIO1P22         0x4B5
#define VFR_FORMID_PCIE_IIO1P23         0x4B6
#define VFR_FORMID_PCIE_IIO1P24         0x4B7
#define VFR_FORMID_PCIE_IIO1P25         0x4B8
#define VFR_FORMID_PCIE_IIO1P26         0x4B9
#define VFR_FORMID_PCIE_IIO1P27         0x4BA
#define VFR_FORMID_PCIE_IIO1P28         0x4BB
#define VFR_FORMID_PCIE_IIO1P29         0x4BC
#define VFR_FORMID_PCIE_IIO1P30         0x4BD
#define VFR_FORMID_PCIE_IIO1P31         0x4BE
#define VFR_FORMID_PCIE_IIO1P32         0x4BF
#define VFR_FORMID_PCIE_IIO1P33         0x4C0
#define VFR_FORMID_PCIE_IIO1P34         0x4C1
#define VFR_FORMID_PCIE_IIO1P35         0x4C2
#define VFR_FORMID_PCIE_IIO1P36         0x4C3
#define VFR_FORMID_PCIE_IIO1P37         0x4C4
#define VFR_FORMID_PCIE_IIO1P38         0x4C5
#define VFR_FORMID_PCIE_IIO1P39         0x4C6
#define VFR_FORMID_PCIE_IIO1P40         0x4C7

#define VFR_FORMID_IIO0                 0x47A
#define VFR_FORMID_IIO1                 0x47B
#define VFR_FORMID_IOAT_CONFIG          0x47C
#define VFR_FORMID_IIO0IOAT             0x47D
#define VFR_FORMID_IIO1IOAT             0x47E
#define VFR_FORMID_IIO2IOAT             0x47F
#define VFR_FORMID_IIO3IOAT             0x480
#define VFR_FORMID_IIO4IOAT             0x481
#define VFR_FORMID_IIO5IOAT             0x482
#define VFR_FORMID_IIO6IOAT             0x483
#define VFR_FORMID_IIO7IOAT             0x484
#define VFR_FORMID_IIO_PCIE_SLOT        0x487
//
// extended IIO form IDs for 4S
//
#define VFR_FORMID_PCIE_IIO2P0          0x0690
#define VFR_FORMID_PCIE_IIO2P1          0x0691
#define VFR_FORMID_PCIE_IIO2P2          0x0692
#define VFR_FORMID_PCIE_IIO2P3          0x0693
#define VFR_FORMID_PCIE_IIO2P4          0x0694
#define VFR_FORMID_PCIE_IIO2P5          0x0695
#define VFR_FORMID_PCIE_IIO2P6          0x0696
#define VFR_FORMID_PCIE_IIO2P7          0x0697
#define VFR_FORMID_PCIE_IIO2P8          0x0698
#define VFR_FORMID_PCIE_IIO2P9          0x0699
#define VFR_FORMID_PCIE_IIO2P10         0x069A
#define VFR_FORMID_PCIE_IIO2P11         0x069B
#define VFR_FORMID_PCIE_IIO2P12         0x069C
#define VFR_FORMID_PCIE_IIO2P13         0x069D
#define VFR_FORMID_PCIE_IIO2P14         0x069E
#define VFR_FORMID_PCIE_IIO2P15         0x06AA
#define VFR_FORMID_PCIE_IIO2P16         0x06AB
#define VFR_FORMID_PCIE_IIO2P17         0x06AC
#define VFR_FORMID_PCIE_IIO2P18         0x06AD
#define VFR_FORMID_PCIE_IIO2P19         0x06AE
#define VFR_FORMID_PCIE_IIO2P20         0x06AF
#define VFR_FORMID_PCIE_IIO2P21         0x4C8
#define VFR_FORMID_PCIE_IIO2P22         0x4C9
#define VFR_FORMID_PCIE_IIO2P23         0x4CA
#define VFR_FORMID_PCIE_IIO2P24         0x4CB
#define VFR_FORMID_PCIE_IIO2P25         0x4CC
#define VFR_FORMID_PCIE_IIO2P26         0x4CD
#define VFR_FORMID_PCIE_IIO2P27         0x4CE
#define VFR_FORMID_PCIE_IIO2P28         0x4CF
#define VFR_FORMID_PCIE_IIO2P29         0x4D0
#define VFR_FORMID_PCIE_IIO2P30         0x4D1
#define VFR_FORMID_PCIE_IIO2P31         0x4D2
#define VFR_FORMID_PCIE_IIO2P32         0x4D3
#define VFR_FORMID_PCIE_IIO2P33         0x4D4
#define VFR_FORMID_PCIE_IIO2P34         0x4D5
#define VFR_FORMID_PCIE_IIO2P35         0x4D6
#define VFR_FORMID_PCIE_IIO2P36         0x4D7
#define VFR_FORMID_PCIE_IIO2P37         0x4D8
#define VFR_FORMID_PCIE_IIO2P38         0x4D9
#define VFR_FORMID_PCIE_IIO2P39         0x4DA
#define VFR_FORMID_PCIE_IIO2P40         0x4DB

#define VFR_FORMID_PCIE_IIO3P0          0x069F
#define VFR_FORMID_PCIE_IIO3P1          0x0670
#define VFR_FORMID_PCIE_IIO3P2          0x0671
#define VFR_FORMID_PCIE_IIO3P3          0x0672
#define VFR_FORMID_PCIE_IIO3P4          0x0673
#define VFR_FORMID_PCIE_IIO3P5          0x06A0
#define VFR_FORMID_PCIE_IIO3P6          0x06A1
#define VFR_FORMID_PCIE_IIO3P7          0x06A2
#define VFR_FORMID_PCIE_IIO3P8          0x06A3
#define VFR_FORMID_PCIE_IIO3P9          0x06A4
#define VFR_FORMID_PCIE_IIO3P10         0x06A5
#define VFR_FORMID_PCIE_IIO3P11         0x06A6
#define VFR_FORMID_PCIE_IIO3P12         0x06A7
#define VFR_FORMID_PCIE_IIO3P13         0x06A8
#define VFR_FORMID_PCIE_IIO3P14         0x06A9
#define VFR_FORMID_PCIE_IIO3P15         0x06B0
#define VFR_FORMID_PCIE_IIO3P16         0x06B1
#define VFR_FORMID_PCIE_IIO3P17         0x06B2
#define VFR_FORMID_PCIE_IIO3P18         0x06B3
#define VFR_FORMID_PCIE_IIO3P19         0x06B4
#define VFR_FORMID_PCIE_IIO3P20         0x06B5
#define VFR_FORMID_PCIE_IIO3P21         0x4DC
#define VFR_FORMID_PCIE_IIO3P22         0x4DD
#define VFR_FORMID_PCIE_IIO3P23         0x4DE
#define VFR_FORMID_PCIE_IIO3P24         0x4DF
#define VFR_FORMID_PCIE_IIO3P25         0x4E1
#define VFR_FORMID_PCIE_IIO3P26         0x4E2
#define VFR_FORMID_PCIE_IIO3P27         0x4E3
#define VFR_FORMID_PCIE_IIO3P28         0x4E4
#define VFR_FORMID_PCIE_IIO3P29         0x4E5
#define VFR_FORMID_PCIE_IIO3P30         0x4E6
#define VFR_FORMID_PCIE_IIO3P31         0x4E7
#define VFR_FORMID_PCIE_IIO3P32         0x4E8
#define VFR_FORMID_PCIE_IIO3P33         0x4E9
#define VFR_FORMID_PCIE_IIO3P34         0x4EA
#define VFR_FORMID_PCIE_IIO3P35         0x4EB
#define VFR_FORMID_PCIE_IIO3P36         0x4EC
#define VFR_FORMID_PCIE_IIO3P37         0x4ED
#define VFR_FORMID_PCIE_IIO3P38         0x4EE
#define VFR_FORMID_PCIE_IIO3P39         0x4EF
#define VFR_FORMID_PCIE_IIO3P40         0x4E0

#define VFR_FORMID_PCIE_IIO4P0          0x800
#define VFR_FORMID_PCIE_IIO4P1          0x801
#define VFR_FORMID_PCIE_IIO4P2          0x802
#define VFR_FORMID_PCIE_IIO4P3          0x803
#define VFR_FORMID_PCIE_IIO4P4          0x804
#define VFR_FORMID_PCIE_IIO4P5          0x805
#define VFR_FORMID_PCIE_IIO4P6          0x806
#define VFR_FORMID_PCIE_IIO4P7          0x807
#define VFR_FORMID_PCIE_IIO4P8          0x808
#define VFR_FORMID_PCIE_IIO4P9          0x809
#define VFR_FORMID_PCIE_IIO4P10         0x80A
#define VFR_FORMID_PCIE_IIO4P11         0x80B
#define VFR_FORMID_PCIE_IIO4P12         0x80C
#define VFR_FORMID_PCIE_IIO4P13         0x80D
#define VFR_FORMID_PCIE_IIO4P14         0x80E
#define VFR_FORMID_PCIE_IIO4P15         0x80F
#define VFR_FORMID_PCIE_IIO4P16         0x810
#define VFR_FORMID_PCIE_IIO4P17         0x811
#define VFR_FORMID_PCIE_IIO4P18         0x812
#define VFR_FORMID_PCIE_IIO4P19         0x813
#define VFR_FORMID_PCIE_IIO4P20         0x814
#define VFR_FORMID_PCIE_IIO4P21         0x815
#define VFR_FORMID_PCIE_IIO4P22         0x816
#define VFR_FORMID_PCIE_IIO4P23         0x817
#define VFR_FORMID_PCIE_IIO4P24         0x818
#define VFR_FORMID_PCIE_IIO4P25         0x819
#define VFR_FORMID_PCIE_IIO4P26         0x81A
#define VFR_FORMID_PCIE_IIO4P27         0x81B
#define VFR_FORMID_PCIE_IIO4P28         0x81C
#define VFR_FORMID_PCIE_IIO4P29         0x81D
#define VFR_FORMID_PCIE_IIO4P30         0x81E
#define VFR_FORMID_PCIE_IIO4P31         0x81F
#define VFR_FORMID_PCIE_IIO4P32         0x820
#define VFR_FORMID_PCIE_IIO4P33         0x821
#define VFR_FORMID_PCIE_IIO4P34         0x822
#define VFR_FORMID_PCIE_IIO4P35         0x823
#define VFR_FORMID_PCIE_IIO4P36         0x824
#define VFR_FORMID_PCIE_IIO4P37         0x825
#define VFR_FORMID_PCIE_IIO4P38         0x826
#define VFR_FORMID_PCIE_IIO4P39         0x827
#define VFR_FORMID_PCIE_IIO4P40         0x828

#define VFR_FORMID_PCIE_IIO5P0          0x829
#define VFR_FORMID_PCIE_IIO5P1          0x82A
#define VFR_FORMID_PCIE_IIO5P2          0x82B
#define VFR_FORMID_PCIE_IIO5P3          0x82C
#define VFR_FORMID_PCIE_IIO5P4          0x82D
#define VFR_FORMID_PCIE_IIO5P5          0x82E
#define VFR_FORMID_PCIE_IIO5P6          0x82F
#define VFR_FORMID_PCIE_IIO5P7          0x830
#define VFR_FORMID_PCIE_IIO5P8          0x831
#define VFR_FORMID_PCIE_IIO5P9          0x832
#define VFR_FORMID_PCIE_IIO5P10         0x833
#define VFR_FORMID_PCIE_IIO5P11         0x834
#define VFR_FORMID_PCIE_IIO5P12         0x835
#define VFR_FORMID_PCIE_IIO5P13         0x836
#define VFR_FORMID_PCIE_IIO5P14         0x837
#define VFR_FORMID_PCIE_IIO5P15         0x838
#define VFR_FORMID_PCIE_IIO5P16         0x839
#define VFR_FORMID_PCIE_IIO5P17         0x83A
#define VFR_FORMID_PCIE_IIO5P18         0x83B
#define VFR_FORMID_PCIE_IIO5P19         0x83C
#define VFR_FORMID_PCIE_IIO5P20         0x83D
#define VFR_FORMID_PCIE_IIO5P21         0x83E
#define VFR_FORMID_PCIE_IIO5P22         0x83F
#define VFR_FORMID_PCIE_IIO5P23         0x840
#define VFR_FORMID_PCIE_IIO5P24         0x841
#define VFR_FORMID_PCIE_IIO5P25         0x842
#define VFR_FORMID_PCIE_IIO5P26         0x843
#define VFR_FORMID_PCIE_IIO5P27         0x844
#define VFR_FORMID_PCIE_IIO5P28         0x845
#define VFR_FORMID_PCIE_IIO5P29         0x846
#define VFR_FORMID_PCIE_IIO5P30         0x847
#define VFR_FORMID_PCIE_IIO5P31         0x848
#define VFR_FORMID_PCIE_IIO5P32         0x849
#define VFR_FORMID_PCIE_IIO5P33         0x84A
#define VFR_FORMID_PCIE_IIO5P34         0x84B
#define VFR_FORMID_PCIE_IIO5P35         0x84C
#define VFR_FORMID_PCIE_IIO5P36         0x84D
#define VFR_FORMID_PCIE_IIO5P37         0x84E
#define VFR_FORMID_PCIE_IIO5P38         0x84F
#define VFR_FORMID_PCIE_IIO5P39         0x850
#define VFR_FORMID_PCIE_IIO5P40         0x851

#define VFR_FORMID_PCIE_IIO6P0          0x852
#define VFR_FORMID_PCIE_IIO6P1          0x853
#define VFR_FORMID_PCIE_IIO6P2          0x854
#define VFR_FORMID_PCIE_IIO6P3          0x855
#define VFR_FORMID_PCIE_IIO6P4          0x856
#define VFR_FORMID_PCIE_IIO6P5          0x857
#define VFR_FORMID_PCIE_IIO6P6          0x858
#define VFR_FORMID_PCIE_IIO6P7          0x859
#define VFR_FORMID_PCIE_IIO6P8          0x85A
#define VFR_FORMID_PCIE_IIO6P9          0x85B
#define VFR_FORMID_PCIE_IIO6P10         0x85C
#define VFR_FORMID_PCIE_IIO6P11         0x85D
#define VFR_FORMID_PCIE_IIO6P12         0x85E
#define VFR_FORMID_PCIE_IIO6P13         0x85F
#define VFR_FORMID_PCIE_IIO6P14         0x860
#define VFR_FORMID_PCIE_IIO6P15         0x861
#define VFR_FORMID_PCIE_IIO6P16         0x862
#define VFR_FORMID_PCIE_IIO6P17         0x863
#define VFR_FORMID_PCIE_IIO6P18         0x864
#define VFR_FORMID_PCIE_IIO6P19         0x865
#define VFR_FORMID_PCIE_IIO6P20         0x866
#define VFR_FORMID_PCIE_IIO6P21         0x867
#define VFR_FORMID_PCIE_IIO6P22         0x868
#define VFR_FORMID_PCIE_IIO6P23         0x869
#define VFR_FORMID_PCIE_IIO6P24         0x86A
#define VFR_FORMID_PCIE_IIO6P25         0x86B
#define VFR_FORMID_PCIE_IIO6P26         0x86C
#define VFR_FORMID_PCIE_IIO6P27         0x86D
#define VFR_FORMID_PCIE_IIO6P28         0x86E
#define VFR_FORMID_PCIE_IIO6P29         0x86F
#define VFR_FORMID_PCIE_IIO6P30         0x870
#define VFR_FORMID_PCIE_IIO6P31         0x871
#define VFR_FORMID_PCIE_IIO6P32         0x872
#define VFR_FORMID_PCIE_IIO6P33         0x873
#define VFR_FORMID_PCIE_IIO6P34         0x874
#define VFR_FORMID_PCIE_IIO6P35         0x875
#define VFR_FORMID_PCIE_IIO6P36         0x876
#define VFR_FORMID_PCIE_IIO6P37         0x877
#define VFR_FORMID_PCIE_IIO6P38         0x878
#define VFR_FORMID_PCIE_IIO6P39         0x879
#define VFR_FORMID_PCIE_IIO6P40         0x87A

#define VFR_FORMID_PCIE_IIO7P0          0x87B
#define VFR_FORMID_PCIE_IIO7P1          0x87C
#define VFR_FORMID_PCIE_IIO7P2          0x87D
#define VFR_FORMID_PCIE_IIO7P3          0x87E
#define VFR_FORMID_PCIE_IIO7P4          0x87F
#define VFR_FORMID_PCIE_IIO7P5          0x880
#define VFR_FORMID_PCIE_IIO7P6          0x881
#define VFR_FORMID_PCIE_IIO7P7          0x882
#define VFR_FORMID_PCIE_IIO7P8          0x883
#define VFR_FORMID_PCIE_IIO7P9          0x884
#define VFR_FORMID_PCIE_IIO7P10         0x885
#define VFR_FORMID_PCIE_IIO7P11         0x886
#define VFR_FORMID_PCIE_IIO7P12         0x887
#define VFR_FORMID_PCIE_IIO7P13         0x888
#define VFR_FORMID_PCIE_IIO7P14         0x889
#define VFR_FORMID_PCIE_IIO7P15         0x88A
#define VFR_FORMID_PCIE_IIO7P16         0x88B
#define VFR_FORMID_PCIE_IIO7P17         0x88C
#define VFR_FORMID_PCIE_IIO7P18         0x88D
#define VFR_FORMID_PCIE_IIO7P19         0x88E
#define VFR_FORMID_PCIE_IIO7P20         0x88F
#define VFR_FORMID_PCIE_IIO7P21         0x890
#define VFR_FORMID_PCIE_IIO7P22         0x891
#define VFR_FORMID_PCIE_IIO7P23         0x892
#define VFR_FORMID_PCIE_IIO7P24         0x893
#define VFR_FORMID_PCIE_IIO7P25         0x894
#define VFR_FORMID_PCIE_IIO7P26         0x895
#define VFR_FORMID_PCIE_IIO7P27         0x896
#define VFR_FORMID_PCIE_IIO7P28         0x897
#define VFR_FORMID_PCIE_IIO7P29         0x898
#define VFR_FORMID_PCIE_IIO7P30         0x899
#define VFR_FORMID_PCIE_IIO7P31         0x89A
#define VFR_FORMID_PCIE_IIO7P32         0x89B
#define VFR_FORMID_PCIE_IIO7P33         0x89C
#define VFR_FORMID_PCIE_IIO7P34         0x89D
#define VFR_FORMID_PCIE_IIO7P35         0x89E
#define VFR_FORMID_PCIE_IIO7P36         0x89F
#define VFR_FORMID_PCIE_IIO7P37         0x8A0
#define VFR_FORMID_PCIE_IIO7P38         0x8A1
#define VFR_FORMID_PCIE_IIO7P39         0x8A2
#define VFR_FORMID_PCIE_IIO7P40         0x8A3

#define VFR_FORMID_VMD                  0x06C0
#define VFR_FORMID_VMD_IIO0             0x06C1
#define VFR_FORMID_VMD_IIO1             0x06C2
#define VFR_FORMID_VMD_IIO2             0x06C3
#define VFR_FORMID_VMD_IIO3             0x06C4
#define VFR_FORMID_VMD_IIO4             0x0792
#define VFR_FORMID_VMD_IIO5             0x0793
#define VFR_FORMID_VMD_IIO6             0x0794
#define VFR_FORMID_VMD_IIO7             0x0795
#define VFR_FORMID_IIO_DFX_CONFIG       0x06C5
#define VFR_FORMID_IIO_DFX_DEVHIDE_SKT0  0x06C6
#define VFR_FORMID_IIO_DFX_DEVHIDE_SKT1  0x06C7
#define VFR_FORMID_IIO_DFX_DEVHIDE_SKT2  0x06C8
#define VFR_FORMID_IIO_DFX_DEVHIDE_SKT3  0x06C9

#define VFR_FORMID_DFX_PCIE_IIO0P0          0x06CA
#define VFR_FORMID_DFX_PCIE_IIO0P1          0x06CB
#define VFR_FORMID_DFX_PCIE_IIO0P2          0x06CD
#define VFR_FORMID_DFX_PCIE_IIO0P3          0x06CE
#define VFR_FORMID_DFX_PCIE_IIO0P4          0x06CF
#define VFR_FORMID_DFX_PCIE_IIO0P5          0x06D0
#define VFR_FORMID_DFX_PCIE_IIO0P6          0x06D1
#define VFR_FORMID_DFX_PCIE_IIO0P7          0x06D2
#define VFR_FORMID_DFX_PCIE_IIO0P8          0x06D3
#define VFR_FORMID_DFX_PCIE_IIO0P9          0x06D4
#define VFR_FORMID_DFX_PCIE_IIO0P10         0x06D5
#define VFR_FORMID_DFX_PCIE_IIO0P11         0x06D6
#define VFR_FORMID_DFX_PCIE_IIO0P12         0x06D7
#define VFR_FORMID_DFX_PCIE_IIO0P13         0x06D8
#define VFR_FORMID_DFX_PCIE_IIO0P14         0x06D9
#define VFR_FORMID_DFX_PCIE_IIO0P15         0x06DA
#define VFR_FORMID_DFX_PCIE_IIO0P16         0x06DB
#define VFR_FORMID_DFX_PCIE_IIO0P17         0x06DC
#define VFR_FORMID_DFX_PCIE_IIO0P18         0x06DD
#define VFR_FORMID_DFX_PCIE_IIO0P19         0x06DE
#define VFR_FORMID_DFX_PCIE_IIO0P20         0x06DF
#define VFR_FORMID_DFX_PCIE_IIO0P21         0x0900
#define VFR_FORMID_DFX_PCIE_IIO0P22         0x0901
#define VFR_FORMID_DFX_PCIE_IIO0P23         0x0902
#define VFR_FORMID_DFX_PCIE_IIO0P24         0x0903
#define VFR_FORMID_DFX_PCIE_IIO0P25         0x0904
#define VFR_FORMID_DFX_PCIE_IIO0P26         0x0905
#define VFR_FORMID_DFX_PCIE_IIO0P27         0x0906
#define VFR_FORMID_DFX_PCIE_IIO0P28         0x0907
#define VFR_FORMID_DFX_PCIE_IIO0P29         0x0908
#define VFR_FORMID_DFX_PCIE_IIO0P30         0x0909
#define VFR_FORMID_DFX_PCIE_IIO0P31         0x090A
#define VFR_FORMID_DFX_PCIE_IIO0P32         0x090B
#define VFR_FORMID_DFX_PCIE_IIO0P33         0x090C
#define VFR_FORMID_DFX_PCIE_IIO0P34         0x090D
#define VFR_FORMID_DFX_PCIE_IIO0P35         0x090E
#define VFR_FORMID_DFX_PCIE_IIO0P36         0x090F
#define VFR_FORMID_DFX_PCIE_IIO0P37         0x0910
#define VFR_FORMID_DFX_PCIE_IIO0P38         0x0911
#define VFR_FORMID_DFX_PCIE_IIO0P39         0x0912
#define VFR_FORMID_DFX_PCIE_IIO0P40         0x0913

#define VFR_FORMID_DFX_PCIE_IIO1P0          0x06E0
#define VFR_FORMID_DFX_PCIE_IIO1P1          0x06E1
#define VFR_FORMID_DFX_PCIE_IIO1P2          0x06E2
#define VFR_FORMID_DFX_PCIE_IIO1P3          0x06E3
#define VFR_FORMID_DFX_PCIE_IIO1P4          0x06E4
#define VFR_FORMID_DFX_PCIE_IIO1P5          0x06E5
#define VFR_FORMID_DFX_PCIE_IIO1P6          0x06E6
#define VFR_FORMID_DFX_PCIE_IIO1P7          0x06E7
#define VFR_FORMID_DFX_PCIE_IIO1P8          0x06E8
#define VFR_FORMID_DFX_PCIE_IIO1P9          0x06E9
#define VFR_FORMID_DFX_PCIE_IIO1P10         0x06EA
#define VFR_FORMID_DFX_PCIE_IIO1P11         0x06EB
#define VFR_FORMID_DFX_PCIE_IIO1P12         0x06EC
#define VFR_FORMID_DFX_PCIE_IIO1P13         0x06ED
#define VFR_FORMID_DFX_PCIE_IIO1P14         0x06EE
#define VFR_FORMID_DFX_PCIE_IIO1P15         0x06EF
#define VFR_FORMID_DFX_PCIE_IIO1P16         0x06F0
#define VFR_FORMID_DFX_PCIE_IIO1P17         0x06F1
#define VFR_FORMID_DFX_PCIE_IIO1P18         0x06F2
#define VFR_FORMID_DFX_PCIE_IIO1P19         0x06F3
#define VFR_FORMID_DFX_PCIE_IIO1P20         0x06F4
#define VFR_FORMID_DFX_PCIE_IIO1P21         0x0914
#define VFR_FORMID_DFX_PCIE_IIO1P22         0x0915
#define VFR_FORMID_DFX_PCIE_IIO1P23         0x0916
#define VFR_FORMID_DFX_PCIE_IIO1P24         0x0917
#define VFR_FORMID_DFX_PCIE_IIO1P25         0x0918
#define VFR_FORMID_DFX_PCIE_IIO1P26         0x0919
#define VFR_FORMID_DFX_PCIE_IIO1P27         0x091A
#define VFR_FORMID_DFX_PCIE_IIO1P28         0x091B
#define VFR_FORMID_DFX_PCIE_IIO1P29         0x091C
#define VFR_FORMID_DFX_PCIE_IIO1P30         0x091D
#define VFR_FORMID_DFX_PCIE_IIO1P31         0x091E
#define VFR_FORMID_DFX_PCIE_IIO1P32         0x091F
#define VFR_FORMID_DFX_PCIE_IIO1P33         0x0920
#define VFR_FORMID_DFX_PCIE_IIO1P34         0x0921
#define VFR_FORMID_DFX_PCIE_IIO1P35         0x0922
#define VFR_FORMID_DFX_PCIE_IIO1P36         0x0923
#define VFR_FORMID_DFX_PCIE_IIO1P37         0x0924
#define VFR_FORMID_DFX_PCIE_IIO1P38         0x0925
#define VFR_FORMID_DFX_PCIE_IIO1P39         0x0926
#define VFR_FORMID_DFX_PCIE_IIO1P40         0x0927

#define VFR_FORMID_DFX_PCIE_IIO2P0          0x06F5
#define VFR_FORMID_DFX_PCIE_IIO2P1          0x06F6
#define VFR_FORMID_DFX_PCIE_IIO2P2          0x06F7
#define VFR_FORMID_DFX_PCIE_IIO2P3          0x06F8
#define VFR_FORMID_DFX_PCIE_IIO2P4          0x06F9
#define VFR_FORMID_DFX_PCIE_IIO2P5          0x06FA
#define VFR_FORMID_DFX_PCIE_IIO2P6          0x06FB
#define VFR_FORMID_DFX_PCIE_IIO2P7          0x06FC
#define VFR_FORMID_DFX_PCIE_IIO2P8          0x06FD
#define VFR_FORMID_DFX_PCIE_IIO2P9          0x06FE
#define VFR_FORMID_DFX_PCIE_IIO2P10         0x06FF
#define VFR_FORMID_DFX_PCIE_IIO2P11         0x0700
#define VFR_FORMID_DFX_PCIE_IIO2P12         0x0701
#define VFR_FORMID_DFX_PCIE_IIO2P13         0x0702
#define VFR_FORMID_DFX_PCIE_IIO2P14         0x0703
#define VFR_FORMID_DFX_PCIE_IIO2P15         0x0704
#define VFR_FORMID_DFX_PCIE_IIO2P16         0x0705
#define VFR_FORMID_DFX_PCIE_IIO2P17         0x0706
#define VFR_FORMID_DFX_PCIE_IIO2P18         0x0707
#define VFR_FORMID_DFX_PCIE_IIO2P19         0x0708
#define VFR_FORMID_DFX_PCIE_IIO2P20         0x0709
#define VFR_FORMID_DFX_PCIE_IIO2P21         0x0928
#define VFR_FORMID_DFX_PCIE_IIO2P22         0x0929
#define VFR_FORMID_DFX_PCIE_IIO2P23         0x092A
#define VFR_FORMID_DFX_PCIE_IIO2P24         0x092B
#define VFR_FORMID_DFX_PCIE_IIO2P25         0x092C
#define VFR_FORMID_DFX_PCIE_IIO2P26         0x092D
#define VFR_FORMID_DFX_PCIE_IIO2P27         0x092E
#define VFR_FORMID_DFX_PCIE_IIO2P28         0x092F
#define VFR_FORMID_DFX_PCIE_IIO2P29         0x0930
#define VFR_FORMID_DFX_PCIE_IIO2P30         0x0931
#define VFR_FORMID_DFX_PCIE_IIO2P31         0x0932
#define VFR_FORMID_DFX_PCIE_IIO2P32         0x0933
#define VFR_FORMID_DFX_PCIE_IIO2P33         0x0934
#define VFR_FORMID_DFX_PCIE_IIO2P34         0x0935
#define VFR_FORMID_DFX_PCIE_IIO2P35         0x0936
#define VFR_FORMID_DFX_PCIE_IIO2P36         0x0937
#define VFR_FORMID_DFX_PCIE_IIO2P37         0x0938
#define VFR_FORMID_DFX_PCIE_IIO2P38         0x0939
#define VFR_FORMID_DFX_PCIE_IIO2P39         0x093A
#define VFR_FORMID_DFX_PCIE_IIO2P40         0x093B

#define VFR_FORMID_DFX_PCIE_IIO3P0          0x070A
#define VFR_FORMID_DFX_PCIE_IIO3P1          0x070B
#define VFR_FORMID_DFX_PCIE_IIO3P2          0x070C
#define VFR_FORMID_DFX_PCIE_IIO3P3          0x070D
#define VFR_FORMID_DFX_PCIE_IIO3P4          0x070E
#define VFR_FORMID_DFX_PCIE_IIO3P5          0x070F
#define VFR_FORMID_DFX_PCIE_IIO3P6          0x0710
#define VFR_FORMID_DFX_PCIE_IIO3P7          0x0711
#define VFR_FORMID_DFX_PCIE_IIO3P8          0x0712
#define VFR_FORMID_DFX_PCIE_IIO3P9          0x0713
#define VFR_FORMID_DFX_PCIE_IIO3P10         0x0714
#define VFR_FORMID_DFX_PCIE_IIO3P11         0x0715
#define VFR_FORMID_DFX_PCIE_IIO3P12         0x0716
#define VFR_FORMID_DFX_PCIE_IIO3P13         0x0717
#define VFR_FORMID_DFX_PCIE_IIO3P14         0x0718
#define VFR_FORMID_DFX_PCIE_IIO3P15         0x0719
#define VFR_FORMID_DFX_PCIE_IIO3P16         0x071A
#define VFR_FORMID_DFX_PCIE_IIO3P17         0x071B
#define VFR_FORMID_DFX_PCIE_IIO3P18         0x071C
#define VFR_FORMID_DFX_PCIE_IIO3P19         0x071D
#define VFR_FORMID_DFX_PCIE_IIO3P20         0x071E
#define VFR_FORMID_DFX_PCIE_IIO3P21         0x093C
#define VFR_FORMID_DFX_PCIE_IIO3P22         0x093D
#define VFR_FORMID_DFX_PCIE_IIO3P23         0x093E
#define VFR_FORMID_DFX_PCIE_IIO3P24         0x093F
#define VFR_FORMID_DFX_PCIE_IIO3P25         0x0940
#define VFR_FORMID_DFX_PCIE_IIO3P26         0x0941
#define VFR_FORMID_DFX_PCIE_IIO3P27         0x0942
#define VFR_FORMID_DFX_PCIE_IIO3P28         0x0943
#define VFR_FORMID_DFX_PCIE_IIO3P29         0x0944
#define VFR_FORMID_DFX_PCIE_IIO3P30         0x0945
#define VFR_FORMID_DFX_PCIE_IIO3P31         0x0946
#define VFR_FORMID_DFX_PCIE_IIO3P32         0x0947
#define VFR_FORMID_DFX_PCIE_IIO3P33         0x0948
#define VFR_FORMID_DFX_PCIE_IIO3P34         0x0949
#define VFR_FORMID_DFX_PCIE_IIO3P35         0x094A
#define VFR_FORMID_DFX_PCIE_IIO3P36         0x094B
#define VFR_FORMID_DFX_PCIE_IIO3P37         0x094C
#define VFR_FORMID_DFX_PCIE_IIO3P38         0x094D
#define VFR_FORMID_DFX_PCIE_IIO3P39         0x094E
#define VFR_FORMID_DFX_PCIE_IIO3P40         0x094F

#define VFR_FORMID_DFX_PCIE_IIO4P0          0x0796
#define VFR_FORMID_DFX_PCIE_IIO4P1          0x0797
#define VFR_FORMID_DFX_PCIE_IIO4P2          0x0798
#define VFR_FORMID_DFX_PCIE_IIO4P3          0x0799
#define VFR_FORMID_DFX_PCIE_IIO4P4          0x079a
#define VFR_FORMID_DFX_PCIE_IIO4P5          0x079b
#define VFR_FORMID_DFX_PCIE_IIO4P6          0x079c
#define VFR_FORMID_DFX_PCIE_IIO4P7          0x079d
#define VFR_FORMID_DFX_PCIE_IIO4P8          0x079e
#define VFR_FORMID_DFX_PCIE_IIO4P9          0x079f
#define VFR_FORMID_DFX_PCIE_IIO4P10         0x07a0
#define VFR_FORMID_DFX_PCIE_IIO4P11         0x07a1
#define VFR_FORMID_DFX_PCIE_IIO4P12         0x07a2
#define VFR_FORMID_DFX_PCIE_IIO4P13         0x07a3
#define VFR_FORMID_DFX_PCIE_IIO4P14         0x07a4
#define VFR_FORMID_DFX_PCIE_IIO4P15         0x07a5
#define VFR_FORMID_DFX_PCIE_IIO4P16         0x07a6
#define VFR_FORMID_DFX_PCIE_IIO4P17         0x07a7
#define VFR_FORMID_DFX_PCIE_IIO4P18         0x07a8
#define VFR_FORMID_DFX_PCIE_IIO4P19         0x07a9
#define VFR_FORMID_DFX_PCIE_IIO4P20         0x07aa
#define VFR_FORMID_DFX_PCIE_IIO4P21         0x0950
#define VFR_FORMID_DFX_PCIE_IIO4P22         0x0951
#define VFR_FORMID_DFX_PCIE_IIO4P23         0x0952
#define VFR_FORMID_DFX_PCIE_IIO4P24         0x0953
#define VFR_FORMID_DFX_PCIE_IIO4P25         0x0954
#define VFR_FORMID_DFX_PCIE_IIO4P26         0x0955
#define VFR_FORMID_DFX_PCIE_IIO4P27         0x0956
#define VFR_FORMID_DFX_PCIE_IIO4P28         0x0957
#define VFR_FORMID_DFX_PCIE_IIO4P29         0x0958
#define VFR_FORMID_DFX_PCIE_IIO4P30         0x0959
#define VFR_FORMID_DFX_PCIE_IIO4P31         0x095A
#define VFR_FORMID_DFX_PCIE_IIO4P32         0x095B
#define VFR_FORMID_DFX_PCIE_IIO4P33         0x095C
#define VFR_FORMID_DFX_PCIE_IIO4P34         0x095D
#define VFR_FORMID_DFX_PCIE_IIO4P35         0x095E
#define VFR_FORMID_DFX_PCIE_IIO4P36         0x095F
#define VFR_FORMID_DFX_PCIE_IIO4P37         0x0960
#define VFR_FORMID_DFX_PCIE_IIO4P38         0x0961
#define VFR_FORMID_DFX_PCIE_IIO4P39         0x0962
#define VFR_FORMID_DFX_PCIE_IIO4P40         0x0963

#define VFR_FORMID_DFX_PCIE_IIO5P0          0x07ab
#define VFR_FORMID_DFX_PCIE_IIO5P1          0x07ac
#define VFR_FORMID_DFX_PCIE_IIO5P2          0x07ad
#define VFR_FORMID_DFX_PCIE_IIO5P3          0x07ae
#define VFR_FORMID_DFX_PCIE_IIO5P4          0x07af
#define VFR_FORMID_DFX_PCIE_IIO5P5          0x07bF
#define VFR_FORMID_DFX_PCIE_IIO5P6          0x07b0
#define VFR_FORMID_DFX_PCIE_IIO5P7          0x07b1
#define VFR_FORMID_DFX_PCIE_IIO5P8          0x07b2
#define VFR_FORMID_DFX_PCIE_IIO5P9          0x07b3
#define VFR_FORMID_DFX_PCIE_IIO5P10         0x07b4
#define VFR_FORMID_DFX_PCIE_IIO5P11         0x07b5
#define VFR_FORMID_DFX_PCIE_IIO5P12         0x07b6
#define VFR_FORMID_DFX_PCIE_IIO5P13         0x07b7
#define VFR_FORMID_DFX_PCIE_IIO5P14         0x07b8
#define VFR_FORMID_DFX_PCIE_IIO5P15         0x07b9
#define VFR_FORMID_DFX_PCIE_IIO5P16         0x07bA
#define VFR_FORMID_DFX_PCIE_IIO5P17         0x07bB
#define VFR_FORMID_DFX_PCIE_IIO5P18         0x07bC
#define VFR_FORMID_DFX_PCIE_IIO5P19         0x07bD
#define VFR_FORMID_DFX_PCIE_IIO5P20         0x07bE
#define VFR_FORMID_DFX_PCIE_IIO5P21         0x0964
#define VFR_FORMID_DFX_PCIE_IIO5P22         0x0965
#define VFR_FORMID_DFX_PCIE_IIO5P23         0x0966
#define VFR_FORMID_DFX_PCIE_IIO5P24         0x0967
#define VFR_FORMID_DFX_PCIE_IIO5P25         0x0968
#define VFR_FORMID_DFX_PCIE_IIO5P26         0x0969
#define VFR_FORMID_DFX_PCIE_IIO5P27         0x096A
#define VFR_FORMID_DFX_PCIE_IIO5P28         0x096B
#define VFR_FORMID_DFX_PCIE_IIO5P29         0x096C
#define VFR_FORMID_DFX_PCIE_IIO5P30         0x096D
#define VFR_FORMID_DFX_PCIE_IIO5P31         0x096E
#define VFR_FORMID_DFX_PCIE_IIO5P32         0x096F
#define VFR_FORMID_DFX_PCIE_IIO5P33         0x0970
#define VFR_FORMID_DFX_PCIE_IIO5P34         0x0971
#define VFR_FORMID_DFX_PCIE_IIO5P35         0x0972
#define VFR_FORMID_DFX_PCIE_IIO5P36         0x0973
#define VFR_FORMID_DFX_PCIE_IIO5P37         0x0974
#define VFR_FORMID_DFX_PCIE_IIO5P38         0x0975
#define VFR_FORMID_DFX_PCIE_IIO5P39         0x0976
#define VFR_FORMID_DFX_PCIE_IIO5P40         0x0977

#define VFR_FORMID_DFX_PCIE_IIO6P0          0x07c0
#define VFR_FORMID_DFX_PCIE_IIO6P1          0x07c1
#define VFR_FORMID_DFX_PCIE_IIO6P2          0x07c2
#define VFR_FORMID_DFX_PCIE_IIO6P3          0x07c3
#define VFR_FORMID_DFX_PCIE_IIO6P4          0x07c4
#define VFR_FORMID_DFX_PCIE_IIO6P5          0x07c5
#define VFR_FORMID_DFX_PCIE_IIO6P6          0x07c6
#define VFR_FORMID_DFX_PCIE_IIO6P7          0x07c7
#define VFR_FORMID_DFX_PCIE_IIO6P8          0x07c8
#define VFR_FORMID_DFX_PCIE_IIO6P9          0x07c9
#define VFR_FORMID_DFX_PCIE_IIO6P10         0x07ca
#define VFR_FORMID_DFX_PCIE_IIO6P11         0x07cb
#define VFR_FORMID_DFX_PCIE_IIO6P12         0x07cc
#define VFR_FORMID_DFX_PCIE_IIO6P13         0x07cd
#define VFR_FORMID_DFX_PCIE_IIO6P14         0x07ce
#define VFR_FORMID_DFX_PCIE_IIO6P15         0x07cf
#define VFR_FORMID_DFX_PCIE_IIO6P16         0x07d0
#define VFR_FORMID_DFX_PCIE_IIO6P17         0x07d1
#define VFR_FORMID_DFX_PCIE_IIO6P18         0x07d2
#define VFR_FORMID_DFX_PCIE_IIO6P19         0x07d3
#define VFR_FORMID_DFX_PCIE_IIO6P20         0x07d4
#define VFR_FORMID_DFX_PCIE_IIO6P21         0x0978
#define VFR_FORMID_DFX_PCIE_IIO6P22         0x0979
#define VFR_FORMID_DFX_PCIE_IIO6P23         0x097A
#define VFR_FORMID_DFX_PCIE_IIO6P24         0x097B
#define VFR_FORMID_DFX_PCIE_IIO6P25         0x097C
#define VFR_FORMID_DFX_PCIE_IIO6P26         0x097D
#define VFR_FORMID_DFX_PCIE_IIO6P27         0x097E
#define VFR_FORMID_DFX_PCIE_IIO6P28         0x097F
#define VFR_FORMID_DFX_PCIE_IIO6P29         0x0980
#define VFR_FORMID_DFX_PCIE_IIO6P30         0x0981
#define VFR_FORMID_DFX_PCIE_IIO6P31         0x0982
#define VFR_FORMID_DFX_PCIE_IIO6P32         0x0983
#define VFR_FORMID_DFX_PCIE_IIO6P33         0x0984
#define VFR_FORMID_DFX_PCIE_IIO6P34         0x0985
#define VFR_FORMID_DFX_PCIE_IIO6P35         0x0986
#define VFR_FORMID_DFX_PCIE_IIO6P36         0x0987
#define VFR_FORMID_DFX_PCIE_IIO6P37         0x0988
#define VFR_FORMID_DFX_PCIE_IIO6P38         0x0989
#define VFR_FORMID_DFX_PCIE_IIO6P39         0x098A
#define VFR_FORMID_DFX_PCIE_IIO6P40         0x098B

#define VFR_FORMID_DFX_PCIE_IIO7P0          0x07d5
#define VFR_FORMID_DFX_PCIE_IIO7P1          0x07d6
#define VFR_FORMID_DFX_PCIE_IIO7P2          0x07d7
#define VFR_FORMID_DFX_PCIE_IIO7P3          0x07d8
#define VFR_FORMID_DFX_PCIE_IIO7P4          0x07d9
#define VFR_FORMID_DFX_PCIE_IIO7P5          0x07da
#define VFR_FORMID_DFX_PCIE_IIO7P6          0x07db
#define VFR_FORMID_DFX_PCIE_IIO7P7          0x07dc
#define VFR_FORMID_DFX_PCIE_IIO7P8          0x07dd
#define VFR_FORMID_DFX_PCIE_IIO7P9          0x07de
#define VFR_FORMID_DFX_PCIE_IIO7P10         0x07df
#define VFR_FORMID_DFX_PCIE_IIO7P11         0x07e0
#define VFR_FORMID_DFX_PCIE_IIO7P12         0x07e1
#define VFR_FORMID_DFX_PCIE_IIO7P13         0x07e2
#define VFR_FORMID_DFX_PCIE_IIO7P14         0x07e3
#define VFR_FORMID_DFX_PCIE_IIO7P15         0x07e4
#define VFR_FORMID_DFX_PCIE_IIO7P16         0x07e5
#define VFR_FORMID_DFX_PCIE_IIO7P17         0x07e6
#define VFR_FORMID_DFX_PCIE_IIO7P18         0x07e7
#define VFR_FORMID_DFX_PCIE_IIO7P19         0x07e8
#define VFR_FORMID_DFX_PCIE_IIO7P20         0x07e9
#define VFR_FORMID_DFX_PCIE_IIO7P21         0x098C
#define VFR_FORMID_DFX_PCIE_IIO7P22         0x098D
#define VFR_FORMID_DFX_PCIE_IIO7P23         0x098E
#define VFR_FORMID_DFX_PCIE_IIO7P24         0x098F
#define VFR_FORMID_DFX_PCIE_IIO7P25         0x0990
#define VFR_FORMID_DFX_PCIE_IIO7P26         0x0991
#define VFR_FORMID_DFX_PCIE_IIO7P27         0x0992
#define VFR_FORMID_DFX_PCIE_IIO7P28         0x0993
#define VFR_FORMID_DFX_PCIE_IIO7P29         0x0994
#define VFR_FORMID_DFX_PCIE_IIO7P30         0x0995
#define VFR_FORMID_DFX_PCIE_IIO7P31         0x0996
#define VFR_FORMID_DFX_PCIE_IIO7P32         0x0997
#define VFR_FORMID_DFX_PCIE_IIO7P33         0x0998
#define VFR_FORMID_DFX_PCIE_IIO7P34         0x0999
#define VFR_FORMID_DFX_PCIE_IIO7P35         0x099A
#define VFR_FORMID_DFX_PCIE_IIO7P36         0x099B
#define VFR_FORMID_DFX_PCIE_IIO7P37         0x099C
#define VFR_FORMID_DFX_PCIE_IIO7P38         0x099D
#define VFR_FORMID_DFX_PCIE_IIO7P39         0x099E
#define VFR_FORMID_DFX_PCIE_IIO7P40         0x099F

#define VFR_FORMID_DFX_IIO0                 0x071F
#define VFR_FORMID_DFX_IIO1                 0x0720
#define VFR_FORMID_DFX_IIO2                 0x0721
#define VFR_FORMID_DFX_IIO3                 0x0722

#define VFR_FORMID_PCIEAIC                  0x0723
#define VFR_FORMID_PCIEAIC_IIO0             0x0724
#define VFR_FORMID_PCIEAIC_IIO1             0x0725
#define VFR_FORMID_PCIEAIC_IIO2             0x0726
#define VFR_FORMID_PCIEAIC_IIO3             0x0727
#define VFR_FORMID_PCIEAIC_IIO4             0x078A
#define VFR_FORMID_PCIEAIC_IIO5             0x078B
#define VFR_FORMID_PCIEAIC_IIO6             0x078C
#define VFR_FORMID_PCIEAIC_IIO7             0x078D

#define VFR_FORMID_CPU_TRACE_HUB_IIO0       0x0728
#define VFR_FORMID_CPU_TRACE_HUB_IIO1       0x0729
#define VFR_FORMID_CPU_TRACE_HUB_IIO2       0x072A
#define VFR_FORMID_CPU_TRACE_HUB_IIO3       0x072B
#define VFR_FORMID_CPU_TRACE_HUB_IIO4       0x0786
#define VFR_FORMID_CPU_TRACE_HUB_IIO5       0x0787
#define VFR_FORMID_CPU_TRACE_HUB_IIO6       0x0788
#define VFR_FORMID_CPU_TRACE_HUB_IIO7       0x0789

#define VFR_FORMID_MMIO_POISON_CTRL_SOCKET0 0x072C
#define VFR_FORMID_MMIO_POISON_CTRL_SOCKET1 0x072D
#define VFR_FORMID_MMIO_POISON_CTRL_SOCKET2 0x072E
#define VFR_FORMID_MMIO_POISON_CTRL_SOCKET3 0x072F
#define VFR_FORMID_MMIO_POISON_CTRL_SOCKET4 0x0730
#define VFR_FORMID_MMIO_POISON_CTRL_SOCKET5 0x0731

#define VFR_FORMLABLE_SOCKET_TOP        0x4062
#define VFR_FORMLABLE_SOCKET_BOTTOM     0x4063

//Per Socket forms for active core count and IOT options
#define VFR_FORMID_PER_SOCKET          0x300
#define VFR_FORMID_CPU_SOCKET0         0x301
#define VFR_FORMID_CPU_SOCKET1         0x302
#define VFR_FORMID_CPU_SOCKET2         0x303
#define VFR_FORMID_CPU_SOCKET3         0x304
#define VFR_FORMID_CPU_SOCKET4         0x305
#define VFR_FORMID_CPU_SOCKET5         0x306
#define VFR_FORMID_CPU_SOCKET6         0x307
#define VFR_FORMID_CPU_SOCKET7         0x308
#define VFR_FORMID_IIO_ERR             0x309

#define SOCKET0_CPUPWRADVPMCFG_FORMID  0x310
#define SOCKET1_CPUPWRADVPMCFG_FORMID  0x311
#define SOCKET2_CPUPWRADVPMCFG_FORMID  0x312
#define SOCKET3_CPUPWRADVPMCFG_FORMID  0x313
#define SOCKET4_CPUPWRADVPMCFG_FORMID  0x314
#define SOCKET5_CPUPWRADVPMCFG_FORMID  0x315
#define SOCKET6_CPUPWRADVPMCFG_FORMID  0x316
#define SOCKET7_CPUPWRADVPMCFG_FORMID  0x317

//P State Control Form
#define P_STATE_CONTROL_FORMID          0x380
#define XE_RATIO_LIMIT_FORMID           0x381
#define VID_OPTIONS_FORM_ID             0x382

//HWPM control Form
#define HWPM_STATE_CONTROL_FORMID       0x385

//Frequency Prioritization
#define FREQUENCY_PRIORITIZATION_FORMID 0x386

//C State Control Form
#define CPU0_CSTATE_CONTROL_FORM_ID     0x390
#define HLV_SASV_FORM_ID                0x391

//T State Control Form
#define CPU_TSTATE_CONTROL_FORM_ID      0x392

//CPU Theraml Management
#define CPU_THERMMAL_MANAGE_FORM_ID     0x393

//Package C State Control
#define PACKAGE_CSTATE_CONTROL_FORM_ID     0x394

//DST 2.0
#define CPU_THERMAL_DTS_2_0_FORM_ID     0x395

//Advacned PM Tuning Form
#define CPU_POWER_ADVANCED_CONFIG_FORM_ID   0x3A0
#define ENERGY_PERF_BIAS_FORM_ID            0x3A1
//#define PROG_POWERCTL_MSR_FORM_ID           0x3A2
#define PROG_MSR_PP_CURT_CFG_CTRL_FORM_ID   0x3A3
#define PROG_MSR_PP_CURT_PSI_CONFIG_FORM_ID 0x3A4
#define PROG_ENTRY_CRITERIA_FORM_ID         0x3A5
#define PROG_CSR_SWLTROVRD_FORM_ID          0x3A6
#define PROG_CSR_DYNAMIC_PERF_POWER_CTL_FORM_ID   0x3A7
#define PROG_CSR_SAPMCTL_FORM_ID            0x3A8
#define PROG_CSR_PERF_P_LIMIT_FORM_ID       0x3A9
#define PROG_PKGC_SA_PS_CRITERIA_FORM_ID       0x3AA

#define DRAM_RAPL_FORMID                    0x3B0
#define SOCKET_RAPL_FORMID                  0x3C0
#define PLATFORM_POWER_LIMIT_FORMID         0x3C1
#define PLATFORM_POWER_LIMIT_MAX_MIN_FORMID 0x3C2
#define PLATFORM_POWER_LIMIT_MASTER_FORMID  0x3C3

#define ACPI_S_STATE_FORMID                 0x3D0

#define PMAX_CONFIG_FORM_ID                 0x3E0

#define SOCKET0_PKGCENTRY_FORMID        0x3F0
#define SOCKET1_PKGCENTRY_FORMID        0x3F1
#define SOCKET2_PKGCENTRY_FORMID        0x3F2
#define SOCKET3_PKGCENTRY_FORMID        0x3F3
#define SOCKET4_PKGCENTRY_FORMID        0x3F4
#define SOCKET5_PKGCENTRY_FORMID        0x3F5
#define SOCKET6_PKGCENTRY_FORMID        0x3F6
#define SOCKET7_PKGCENTRY_FORMID        0x3F7

#define SOCKET0_PKGCSAPM_FORMID         0x3F8
#define SOCKET1_PKGCSAPM_FORMID         0x3F9
#define SOCKET2_PKGCSAPM_FORMID         0x3FA
#define SOCKET3_PKGCSAPM_FORMID         0x3FB
#define SOCKET4_PKGCSAPM_FORMID         0x3FC
#define SOCKET5_PKGCSAPM_FORMID         0x3FD
#define SOCKET6_PKGCSAPM_FORMID         0x3FE
#define SOCKET7_PKGCSAPM_FORMID         0x3FF

#define SOCKET0_PMAX_CONFIG_FORMID        0x400
#define SOCKET1_PMAX_CONFIG_FORMID        0x401
#define SOCKET2_PMAX_CONFIG_FORMID        0x402
#define SOCKET3_PMAX_CONFIG_FORMID        0x403
#define SOCKET4_PMAX_CONFIG_FORMID        0x404
#define SOCKET5_PMAX_CONFIG_FORMID        0x405
#define SOCKET6_PMAX_CONFIG_FORMID        0x406
#define SOCKET7_PMAX_CONFIG_FORMID        0x407

#define SOCKET0_PKGC_SA_PS_CRITERIA_FORMID         0x408
#define SOCKET1_PKGC_SA_PS_CRITERIA_FORMID         0x409
#define SOCKET2_PKGC_SA_PS_CRITERIA_FORMID         0x40A
#define SOCKET3_PKGC_SA_PS_CRITERIA_FORMID         0x40B
#define SOCKET4_PKGC_SA_PS_CRITERIA_FORMID         0x40C
#define SOCKET5_PKGC_SA_PS_CRITERIA_FORMID         0x40D
#define SOCKET6_PKGC_SA_PS_CRITERIA_FORMID         0x40E
#define SOCKET7_PKGC_SA_PS_CRITERIA_FORMID         0x40F

#define COMMONRC_B2P_MAILBOX_CMD_FORMID   0x410

#define VFR_FORMID_PSMI                   0x411
#define VFR_FORMID_PSMI_SOCKET0           0x412
#define VFR_FORMID_PSMI_SOCKET1           0x413
#define VFR_FORMID_PSMI_SOCKET2           0x414
#define VFR_FORMID_PSMI_SOCKET3           0x415
#define VFR_FORMID_PSMI_SOCKET4           0x416
#define VFR_FORMID_PSMI_SOCKET5           0x417
#define VFR_FORMID_PSMI_SOCKET6           0x418
#define VFR_FORMID_PSMI_SOCKET7           0x419

#define VFR_FORMID_PROCESSOR_DFX           0x420
#define VFR_FORMID_PROCESSOR_DFX_FUSASAF   0x422

#define VFR_FORMID_PROCESSOR_CFR           0x423

// {516D5A04-C0D5-4657-B908-E4FB1D935EF0}
#define SOCKET_FORMSET_GUID \
  { \
  0x516d5a04, 0xc0d5, 0x4657, 0xb9, 0x8, 0xe4, 0xfb, 0x1d, 0x93, 0x5e, 0xf0 \
  }

// {DD84017E-7F52-48F9-B16E-50ED9E0DBE27}
#define SOCKET_IIO_CONFIG_GUID \
  { \
    0xdd84017e, 0x7f52, 0x48f9, 0xb1, 0x6e, 0x50, 0xed, 0x9e, 0xd, 0xbe, 0x27 \
  }

// {4402CA38-808F-4279-BCEC-5BAF8D59092F}
#define SOCKET_COMMONRC_CONFIG_GUID \
  { \
    0x4402ca38, 0x808f, 0x4279, 0xbc, 0xec, 0x5b, 0xaf, 0x8d, 0x59, 0x09, 0x2f \
  }

// {2B9B22DE-2AD4-4ABC-957D-5F18C504A05C}
#define SOCKET_MP_LINK_CONFIG_GUID \
  { \
    0x2b9b22de, 0x2ad4, 0x4abc, 0x95, 0x7d, 0x5f, 0x18, 0xc5, 0x04, 0xa0, 0x5c \
  }

// {CA3FF937-D646-4936-90E8-1B950649B389}
#define SOCKET_PCI_RESOURCE_CONFIG_DATA_GUID \
  { \
    0xca3ff937, 0xd646, 0x4936, 0x90, 0xe8, 0x1b, 0x95, 0x06, 0x49, 0xb3, 0x89 \
  }

// {98CF19ED-4109-4681-B79D-9196757C7824}
#define SOCKET_MEMORY_CONFIG_GUID \
  { \
    0x98cf19ed, 0x4109, 0x4681, 0xb7, 0x9d, 0x91, 0x96, 0x75, 0x7c, 0x78, 0x24 \
  }

// {6BE64B20-C679-4ECD-ACE8-87AB4B70EC06}
#define SOCKET_MISC_CONFIG_GUID \
  { \
    0x6be64b20, 0xc679, 0x4ecd, 0xac, 0xe8, 0x87, 0xab, 0x4b, 0x70, 0xec, 0x6 \
  }
// {A1047342-BDBA-4DAE-A67A-40979B65C7F8}
#define SOCKET_POWERMANAGEMENT_CONFIG_GUID \
  { \
    0xA1047342, 0xBDBA, 0x4DAE, 0xA6, 0x7A, 0x40, 0x97, 0x9B, 0x65, 0xC7, 0xF8 \
  }
// {07013588-C789-4E12-A7C3-88FAFAE79F7C}
#define SOCKET_PROCESSORCORE_CONFIG_GUID \
  { \
    0x07013588, 0xC789, 0x4E12, 0xA7, 0xC3, 0x88, 0xFA, 0xFA, 0xE7, 0x9F, 0x7C \
  }
#endif
