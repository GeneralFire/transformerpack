/** @file
  HSIO ChipsetInit library

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

@par Specification
**/
#ifndef _CHIPSET_INIT_LIB_H_
#define _CHIPSET_INIT_LIB_H_

#include <Uefi/UefiBaseType.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchPcrLib.h>
#include <Library/PeiHsioLib.h>
#include <Register/PchPcrRegs.h>

extern EFI_GUID gPchHsioChipsetInitSusTblDataPpiGuid;
#define PCH_HSIO_CHIPSETINIT_TBL_MAX_SIZE        0x2000

/**
  PCH HSIO ChipsetInit Version Information
**/
typedef struct {
  UINT16 BaseCrc;
  UINT16 SusCrc;
  UINT16 OemCrc;
  UINT8  Version;
  UINT8  Product;
  UINT8  MetalLayer     : 4;
  UINT8  BaseLayer      : 4;
  UINT8  OemVersion;
  UINT16 DebugMode      : 1;
  UINT16 OemCrcValid    : 1;
  UINT16 SusCrcValid    : 1;
  UINT16 BaseCrcValid   : 1;
  UINT16 Reserved       : 12;
} PCH_HSIO_VER_INFO;

#define PCH_HSIO_SUS_CRC_INVALID                 0xFFFF

#define PCH_HSIO_HEADER_ID_MODPHY_COMMON_LANE 0x0
#define PCH_HSIO_HEADER_ID_USB2_PHY           0x1
#define PCH_HSIO_HEADER_ID_FIA                0x2
#define PCH_HSIO_HEADER_ID_MODPHY_DATA_LANE   0x3
#define PCH_HSIO_HEADER_ID_OPI_PHY            0x4
#define PCH_HSIO_HEADER_ID_GPIO_CNVI          0x5
#define PCH_HSIO_HEADER_ID_OEM_GPIO_TUNING    0x8
#define PCH_HSIO_HEADER_ID_MODPHY_TUNING      0x9
#define PCH_HSIO_HEADER_ID_USB2_TUNING        0xA
#define PCH_HSIO_HEADER_ID_END_OF_HEADER      0xF

#define PCH_HSIO_HDR_FIELD_NUM                16

#define RECORD_OFFSET(X, Y)       ((X << 4) | Y)
#define RECORD_OFFSET_UNDEFINED   0xFFFF

/**
  PCH HSIO ChipsetInit Header Field
**/
typedef struct {
  UINT16 RecordOffsetHigh : 8; //7:0 -> RecordOffset[11:4]
  UINT16 Id : 4;
  UINT16 RecordOffsetLow : 4; //15:12 -> RecordOffset[3:0]
} PCH_HSIO_HDR_FIELD;

typedef struct {
  PCH_HSIO_VER_INFO  ChipsetInitVerInfo;
  PCH_HSIO_HDR_FIELD ChipsetInitHeader[PCH_HSIO_HDR_FIELD_NUM];
  UINT8              TableData[];
} PCH_HSIO_CHIPSET_INIT_TABLE;

/**
  PMC Commands size in WORD units
**/
#define PMC_DATA_CMD_SIZE          ((12/sizeof(UINT16))-1)

// Commands specified command table and processed by the PMC & it's HW accelerator
typedef enum {
  PhyCmdSendPosted = 0x0, // Perform a Write & wait for result
  PhyCmdSendNonPosted,    // Perform a Write & ignore return result (Not Supported in HW)
  PhyCmdDoDelay,          // PMC Inserts Delay when command detected
  PhyCmdEndStruct = 0x7   // No-op Command indicating end of list
} PCH_HSIO_PHY_COMMANDS;

/**
  Command Field
**/
typedef struct {
  UINT8 Command : 3;
  UINT8 Size : 5;
} PCH_HSIO_CMD_FIELD;

/**
  PCH HSIO ChipsetInit Command
**/
typedef struct {
  PCH_HSIO_CMD_FIELD CommandField;
  UINT8              Pid;
  UINT8              OpCode; //PrivateControlWrite
  UINT8              Bar; //0
  UINT8              Fbe; //First Byte Enable  : 0x0F
  UINT8              Fid; //0
  UINT16             Offset;
  UINT32             Value;
} PCH_HSIO_DATA_CMD;

/**
PCH HSIO Delay XRAM Data
**/
typedef struct {
  PCH_HSIO_CMD_FIELD CommandField;
  UINT8              DelayPeriod; //(00h = 1us, 01h = 10us, 02h = 100us, ..., 07h = 10s; others reserved)
  UINT8              DelayCount; //(0 - 255); total delay = Delay period * Delay count
  UINT8              Padding;
} PCH_HSIO_DELAY_CMD;

#define PCH_HSIO_CHIPSETINIT_USB2_SUS_MAX_SIZE   0x100
#define PCH_HSIO_CHIPSETINIT_MODPHY_SUS_MAX_SIZE 0x100
#define PCH_HSIO_CHIPSETINIT_CRC_SUS_MAX_SIZE    0xAA ///< (sizeof (PchHsioSusUsb2Tuning) + sizeof (PchHsioSusModPhyTuning)) / sizeof (PCH_HSIO_DATA_CMD)

typedef struct PCH_HSIO_SUS_CRC {
  UINT32  AndData;
  UINT32  OrData;
} PCH_HSIO_SUS_CRC;

typedef struct {
  UINT32                   PchHsioSusUsb2Tuning[PCH_HSIO_CHIPSETINIT_USB2_SUS_MAX_SIZE];     ///< USB2 SUS Table
  UINT32                   PchHsioSusUsb2TuningCount;    ///< Total number of entries in the USB2 SUS Table
  UINT32                   PchHsioSusModPhyTuning[PCH_HSIO_CHIPSETINIT_MODPHY_SUS_MAX_SIZE];   ///< ModPHY Tuning SUS Table
  UINT32                   PchHsioSusModPhyTuningCount;  ///< Total number of entries in the ModPHY Tuning SUS Table
  PCH_HSIO_SUS_CRC         PchHsioSusCrc[PCH_HSIO_CHIPSETINIT_CRC_SUS_MAX_SIZE];  ///< Bits to be used to calculate the SUS
} PCH_HSIO_CHIPSETINIT_SUS_TBL_PPI;

/**
  Intializes the HSIO ChipsetInit SUS Table Data Structure

  @param[in out] PchHsioChipsetInitSusTblData        Pointer to HSIO ChipsetInit SUS table Data

  @retval EFI_SUCCESS                 Located SUS Table PPI
  @retval EFI_OUT_OF_RESOURCES        Unable to allocate memory for SUS Table PPI
**/
EFI_STATUS
HsioGetChipsetInitSusTblData (
  PCH_HSIO_CHIPSETINIT_SUS_TBL_PPI   **PchHsioChipsetInitSusTblData
  );

/**
  Writes PHY Tuning values to the ChipsetInit SUS Table

  @param[in] Pid         Sideband ID
  @param[in] Offset      PCR offset
  @param[in] Data32      DWORD Value
  @param[in] CrcAndMask  AND Mask to be used for CRC calculation
  @param[in] CrcOrMask   OR Mask to be used for CRC calculation

  @retval EFI_SUCCESS             Copy to SUS Table was successful
  @retval EFI_DEVICE_ERROR        Unable to save writes to SUS Table
**/
EFI_STATUS
HsioChipsetInitSusWrite32 (
  PCH_SBI_PID  Pid,
  UINT32       Offset,
  UINT32       Data32,
  UINT32       CrcAndMask,
  UINT32       CrcOrMask
  );

/**
  This function is used to ensure CSME has the latest Base, OEM and SUS tables.
  A BIOS<->CSME ChipsetInit sync is performed if there is a mismatch.

  @retval EFI_SUCCESS             BIOS and CSME ChipsetInit settings are in sync
  @retval EFI_UNSUPPORTED         ChipsetInit sync not supported
  @retval EFI_OUT_OF_RESOURCES    BIOS is not able to Allocate memory
**/

EFI_STATUS
ChipsetInitSync (
  VOID
  );

#endif // _CHIPSET_INIT_LIB_H_
