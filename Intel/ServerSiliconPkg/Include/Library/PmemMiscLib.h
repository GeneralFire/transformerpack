/** @file

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

#ifndef _PMEM_MISC_LIB_H_
#define _PMEM_MISC_LIB_H_

//
// CR Media Controller Types
//
#define FNV 0x979
#define EKV 0x97A
#define BWV 0x97B
#define CWV 0x97C

//
// CR S-F Interface Specification Header Revisions
//
#define NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE_2_00_HDR 0x11
#define NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE_2_01_HDR 0x12
#define NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE_3_00_HDR 0x30
#define NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE_3_01_HDR 0x31

#define NGN_CR_SW_FW_INTERFACE_REVISION_1_00_HDR        0x01
#define NGN_CR_SW_FW_INTERFACE_REVISION_1_00_CFG_HDR    0x02

#define NGN_CR_SW_FW_INTERFACE_REVISION_2_00_HDR        0x11
#define NGN_CR_SW_FW_INTERFACE_REVISION_2_00_CFG_HDR    0x11

#define NGN_CR_SW_FW_INTERFACE_REVISION_2_01_HDR        0x12
#define NGN_CR_SW_FW_INTERFACE_REVISION_2_01_CFG_HDR    0x12

#define NGN_CR_SW_FW_INTERFACE_REVISION_3_00_HDR        0x30
#define NGN_CR_SW_FW_INTERFACE_REVISION_3_00_CFG_HDR    0x30

#define NGN_CR_SW_FW_INTERFACE_REVISION_3_01_HDR        0x31
#define NGN_CR_SW_FW_INTERFACE_REVISION_3_01_CFG_HDR    0x31


#define ISET_RECOMMENDED     1
#define ISET_NOT_RECOMMENDED 0

/**
  6 CHANNEL INTERLEAVES
    bit[0]: IMC0.CH0
    bit[1]: IMC0.CH1
    bit[2]: IMC0.CH2
    bit[3]: IMC1.CH3
    bit[4]: IMC1.CH4
    bit[5]: IMC1.CH5
**/
#define ISET_CH6_WY6              0x3F
#define ISET_CH6_WY4_CH0_CH1      0x1B
#define ISET_CH6_WY4_CH0_CH2      0x2D
#define ISET_CH6_WY4_CH1_CH2      0x36
#define ISET_CH6_WY3_IMC0         0x7
#define ISET_CH6_WY3_IMC1         0x38
#define ISET_CH6_WY2_XIMC_CH0     0x9
#define ISET_CH6_WY2_XIMC_CH1     0x12
#define ISET_CH6_WY2_XIMC_CH2     0x24
#define ISET_CH6_WY2_IMC0_CH0_CH1 0x3
#define ISET_CH6_WY2_IMC0_CH0_CH2 0x5
#define ISET_CH6_WY2_IMC0_CH1_CH2 0x6
#define ISET_CH6_WY2_IMC1_CH0_CH1 0x18
#define ISET_CH6_WY2_IMC1_CH0_CH2 0x28
#define ISET_CH6_WY2_IMC1_CH1_CH2 0x30
#define ISET_CH6_WY1_IMC0_CH0     0x1
#define ISET_CH6_WY1_IMC0_CH1     0x2
#define ISET_CH6_WY1_IMC0_CH2     0x4
#define ISET_CH6_WY1_IMC1_CH0     0x8
#define ISET_CH6_WY1_IMC1_CH1     0x10
#define ISET_CH6_WY1_IMC1_CH2     0x20

/**
  8 CHANNEL INTERLEAVES
    bit[0]: IMC0.CH0
    bit[1]: IMC0.CH1
    bit[2]: IMC1.CH2
    bit[3]: IMC1.CH3
    bit[4]: IMC2.CH4
    bit[5]: IMC2.CH5
    bit[6]: IMC3.CH6
    bit[7]: IMC3.CH7
**/
#define ISET_CH8_WY8               0xFF
#define ISET_CH8_WY4_XIMC_CH0      0x55
#define ISET_CH8_WY4_XIMC_CH1      0xAA
#define ISET_CH8_WY4_IMC0_IMC2     0x33
#define ISET_CH8_WY4_IMC1_IMC3     0xCC
#define ISET_CH8_WY4_IMC0_IMC3     0xC3
#define ISET_CH8_WY4_IMC1_IMC2     0x3C
#define ISET_CH8_WY4_IMC0_IMC1     0xF
#define ISET_CH8_WY4_IMC2_IMC3     0xF0
#define ISET_CH8_WY2_IMC0_IMC2_CH0 0x11
#define ISET_CH8_WY2_IMC0_IMC2_CH1 0x22
#define ISET_CH8_WY2_IMC1_IMC3_CH0 0x44
#define ISET_CH8_WY2_IMC1_IMC3_CH1 0x88
#define ISET_CH8_WY2_IMC0_IMC3_CH0 0x41
#define ISET_CH8_WY2_IMC0_IMC3_CH1 0x82
#define ISET_CH8_WY2_IMC1_IMC2_CH0 0x14
#define ISET_CH8_WY2_IMC1_IMC2_CH1 0x28
#define ISET_CH8_WY2_IMC0_IMC1_CH0 0x5
#define ISET_CH8_WY2_IMC0_IMC1_CH1 0xA
#define ISET_CH8_WY2_IMC2_IMC3_CH0 0x50
#define ISET_CH8_WY2_IMC2_IMC3_CH1 0xA0
#define ISET_CH8_WY2_IMC0          0x3
#define ISET_CH8_WY2_IMC1          0xC
#define ISET_CH8_WY2_IMC2          0x30
#define ISET_CH8_WY2_IMC3          0xC0
#define ISET_CH8_WY1_IMC0_CH0      0x1
#define ISET_CH8_WY1_IMC0_CH1      0x2
#define ISET_CH8_WY1_IMC1_CH0      0x4
#define ISET_CH8_WY1_IMC1_CH1      0x8
#define ISET_CH8_WY1_IMC2_CH0      0x10
#define ISET_CH8_WY1_IMC2_CH1      0x20
#define ISET_CH8_WY1_IMC3_CH0      0x40
#define ISET_CH8_WY1_IMC3_CH1      0x80

/**
  4 CHANNEL INTERLEAVES
    bit[0]: IMC0.CH0
    bit[1]: IMC0.CH1
    bit[2]: IMC1.CH2
    bit[3]: IMC1.CH3
**/
#define ISET_CH4_WY4          0xF
#define ISET_CH4_WY2_XIMC_CH0 0x5
#define ISET_CH4_WY2_XIMC_CH1 0xA
#define ISET_CH4_WY2_IMC0     0x3
#define ISET_CH4_WY2_IMC1     0xC

//
// A single supported PM interleave format
//
#pragma pack(push, 1)
typedef struct {
  UINT16 InterleaveBitmap;  ///< Bitmap of ordered channels
  UINT8  Recommended;       ///< Interleave is recommended
  UINT8  Reserved;
} PM_INTERLEAVE;
#pragma pack(pop)

//
// Bit Fields used to describe maximum interleave sets
//
typedef union {
  struct {
    UINT8 PerCpuDie : 4; ///< Bits[3-0]:  per CPU Die
    UINT8 PerPmem   : 4; ///< Bits[7-4]:  per PMem
    UINT8 Reserved  : 8; ///< Bits[15-8]: Reserved
  } Bits;
  UINT16  Data;
} MAX_INT_SET;


/**
  Get CR S-F interface spec. header revision for the type of the CPU.

  @retval  CR S-F interface spec. revision

**/
UINT8
EFIAPI
GetPmemConfigHeaderRevision  (
  VOID
  );

/**
  Verifies if CR S-F interface spec. header revision is compatible with currently used
  (for backward compatibility checks).

  @param[in] Revision    The header reversion currently used.

  @retval TRUE    CR S-F interface spec. revision is valid.
          FALSE   CR S-F interface spec. revision is not valid

**/
BOOLEAN
EFIAPI
IsPmemConfigHeaderRevisionCompatible (
  IN UINT8 Revision
  );

/**
  Get CR S-F interface spec. header revision of PCDs (CCUR, CIN_, COUT) for the type of the CPU.

  @retval  CR S-F interface spec. PCD revision

**/
UINT8
EFIAPI
GetPmemPcdRecordsHeaderRevision (
  VOID
  );

/**
  Verifies if CR S-F interface spec. header revision of PCDs (CCUR, CIN_, COUT)
  is compatible with currently used (for backward compatibility checks).

  @param[in] Revision    The header reversion currently used.

  @retval TRUE    CR S-F interface spec. revision is valid
          FALSE   CR S-F interface spec. revision is not valid

**/
BOOLEAN
EFIAPI
IsPmemPcdRecordsHeaderRevisionCompatible (
  IN UINT8 Revision
  );

/**
  Get PCAT revision for the type of the CPU

  @retval  PCAT header revision

**/
UINT8
EFIAPI
GetPcatHeaderRevision (
  VOID
  );

/**

  Fetch the number of supported interleave formats for the AppDirect write back platform configuration.

  @param[in]  SncEnabled           0 - SNC disabled for this configuration, 1 - SNC enabled for this configuration
  @param[in]  NumOfCluster         SNC level 2 / 4

  @retval    The number of supported interleave formats.

**/
UINT16
EFIAPI
GetAppDirectWbNumIntFormats (
  IN  UINT8              SncEnabled,
  IN  UINT8              NumOfCluster
  );

/**

  Fetch a copy of the supported interleave format list for the AppDirect write back platform configuration.

  @param[in]  SncEnabled           0 - SNC disabled for this configuration, 1 - SNC enabled for this configuration
  @param[in]  NumOfCluster         SNC level 2 / 4
  @param[out] IntFormatListBuffer  A pointer to the data buffer where the interleave format list will be copied to.

**/
VOID
EFIAPI
GetAppDirectWbIntFormatList (
  IN     UINT8              SncEnabled,
  IN     UINT8              NumOfCluster,
     OUT PM_INTERLEAVE      *IntFormatListBuffer
  );

/**

  Fetch the number of supported interleave formats for the AppDirect platform configuration.

  @retval    The number of supported interleave formats.

**/
UINT16
EFIAPI
GetAppDirectNumIntFormats (
  VOID
  );

/**

  Fetch a copy of the supported interleave format list for the AppDirect platform configuration.

  @param[out] IntFormatListBuffer  A pointer to the data buffer where the interleave format list will be copied to.

**/
VOID
EFIAPI
GetAppDirectIntFormatList (
  OUT PM_INTERLEAVE      *IntFormatListBuffer
  );

/**

  Fetch the supported Max Interleave Sets for the current AppDirect write back platform configuration.

  @param[in] VolMemMode              Volatile memory mode.

  @retval The encoded Max Interleave Sets for the current AppDirect write back platform config.

**/
MAX_INT_SET
EFIAPI
GetAppDirectWbMaxInterleaveSet (
  IN UINT8    VolMemMode
  );

/**

  Fetch the supported Max Interleave Sets for the current AppDirect platform configuration.

  @retval The encoded Max Interleave Sets for the current AppDirect platform config.

**/
MAX_INT_SET
EFIAPI
GetAppDirectMaxInterleaveSet (
  VOID
  );

/**

  Fetch the supported Max Interleave Sets for the current platform configuration.

  @retval The encoded Max Interleave Sets for the current platform config.

**/
MAX_INT_SET
EFIAPI
GetMaxInterleaveSet (
  VOID
  );

/**
  This function gets the Die count of the specified socket for ACPI tables consumption.

  @param[in] SocketId      Socket Index.

  @retval The Die count of the specified socket.
**/
UINT8
GetAcpiDieCount (
  IN UINT8   SocketId
  );

#endif  // _PMEM_MISC_LIB_H_
