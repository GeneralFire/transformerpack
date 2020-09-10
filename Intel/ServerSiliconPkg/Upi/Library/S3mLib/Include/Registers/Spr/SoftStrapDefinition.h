/** @file
  S3M SoftStrap data structure definition header file.

  This file contains the S3M SoftStrap data structure definition for SPR.

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

#ifndef __S3M_STRAP_STRUCTURE_H__
#define __S3M_STRAP_STRUCTURE_H__

/*********************************************************************
                    S3M SoftStrap - Configuration
*********************************************************************/

typedef union {
  struct {
    UINT32  TscTxOffset:9;            ///< Bits[8:0]
    UINT32  TscRxOffset:9;            ///< Bits[17:9]
    UINT32  TscBoardOffset:4;         ///< Bits[21:18]
    UINT32  ValidBoardOffOverride:1;  ///< Bits[22:22]
    UINT32  Reserved:8;               ///< Bits[30:23]
    UINT32  ValidTxRxOffOverride:1;   ///< Bits[31:31]
  } Bits;
  UINT32 Data;
} TSC_TRANSFER_OFFSET;

typedef struct {
  TSC_TRANSFER_OFFSET         TscOffset;
} S3M_CONFIG;

/*********************************************************************
                      S3M SoftStrap - Strap Set
*********************************************************************/

typedef union {
  struct {
    UINT32  SmtDisable:1;             ///< Bits[00:00] 1 - SMT is disabled; 0 - SMT is enabled
    UINT32  FlexRatio:8;              ///< Bits[08:01] BIOS sets the max-non-turbo ratio
    UINT32  CoreHighVoltageMode:1;    ///< Bits[09:09] All core FIVR domain voltage related setting.
                                      ///<             HEDT only. Default is 0.
    UINT32  NonCoreHighVoltageMode:1; ///< Bits[10:10] All non-core FIVR domain voltage related setting.
                                      ///<             HEDT only. Default is 0.
    UINT32  SncEn:1;                  ///< Bits[11:11] SNC enable/disable indication.
                                      ///<             Maps toMS2IDI.SNC_CONFIG.snc_full_en bit. Default is 0.
    UINT32  SncIndEn:1;               ///< Bits[12:12] Maps to MS2IDI.SNC_CONFIG.snc_ind_en bit. Default is 0.
    UINT32  SncCluster:1;             ///< Bits[13:13] SNC cluster mode selection.
                                      ///<             0 -> 2 clusters; 1 -> 4 clusters. Default is 0.
    UINT32  TwoLmEn:1;                ///< Bits[14:14] 2LM Mode enable/disable indication. Default is 0.
    UINT32  IioStackDis:8;            ///< Bits[22:15] If set to 1, a given MS2IOSF instance is disabled overriding
                                      ///<             the fuse enable. Default is 0 (behaves as defined by internal fuse.)
                                      ///<             Order of these disabled bits in SoC should map to the order of
                                      ///<             corresponding M2IOSF CAPID bits.
    UINT32  Reserved1:2;              ///< Bits[24:23] Reserved
    UINT32  AdrResume:1;              ///< Bits[25:25] Default is 0. CSME overrides it to a value if ADR Resume flow.
    UINT32  AdrResumeCoreCount:2;     ///< Bits[27:26] Default is 0. CSME overrides with value configured by BIOS in PMC
                                      ///<             to indicate how many cores need to be enabled during ADR Resume flow.
                                      ///<             00 -> all cores; 01 - > 1 core; 10 -> 2 cores; 11 - > reserved.
    UINT32  ConfigTdpLevel:3;         ///< Bits[30:28] TDP configuration level. Default is 0.
                                      ///<             3;b000 -> Nominal TDP level
                                      ///<             3;b001 -> Level from CONFIG_TDP_LEVEL_1
                                      ///<             3;b010 -> Level from CONFIG_TDP_LEVEL_2
                                      ///<             3;b011 -> Level from CONFIG_TDP_EXT_L1
                                      ///<             3;b100 -> Level from CONFIG_TDP_EXT_L2
                                      ///<             3;b1xx -> Reserved
    UINT32  UmaEn:1;                  ///< Bits[31:31] 1 - UMA enabled; 0 - UMA disabled
  } Bits;
  UINT32 Data;
} STRAP_SET_DWORD1;

typedef union {
  struct {
    UINT32  NoC:32;                   ///< Bits[31:00] Cores disabled. Default is 0.
  } Bits;
  UINT32 Data;
} STRAP_SET_DWORD2;

typedef union {
  struct {
    UINT32  NoC:32;                   ///< Bits[31:00] Cores disabled. Default is 0.
  } Bits;
  UINT32 Data;
} STRAP_SET_DWORD3;

typedef union {
  struct {
    UINT32  NoC:16;                   ///< Bits[15:00] Cores disabled. Default is 0.
    UINT32  PcieGen4SpeedLimit:6;     ///< Bits[21:16] If set to 1, limit max PCIE speed to only Gen4.
                                      ///<             Default is 0 which indicates no limit, PCIE can go up to Gen5 speed.
    UINT32  Reserved:1;               ///< Bits[22:22] Reserved. Default is 0.
    UINT32  LimitPaTo46:1;            ///< Bits[23:23] If set to 1, PA will be limited to 46. Default is 0.
    UINT32  IioStackDisHigh:4;        ///< Bits[27:24] If set to 1, a given MS2IOSF instance is disabled overriding the fuse enable.
                                      ///<             Default is 0. See DWORD1.IioStackDis
    UINT32  UpiDisable:4;             ///< Bits[31:28] If set to 1, a given SCF_UPI instance is disabled overriding the fuse enable.
                                      ///<             Default is 0 (behaves as defined by fuse).
  } Bits;
  UINT32 Data;
} STRAP_SET_DWORD4;

typedef struct {
  UINT32  Reserved:32;
} STRAP_SET_DWORD5;

typedef struct {
  UINT32  Reserved:32;
} STRAP_SET_DWORD6;

typedef struct {
  UINT32  Reserved:32;
} STRAP_SET_DWORD7;

typedef struct {
  UINT32  Reserved:32;
} STRAP_SET_DWORD8;

typedef struct {
  STRAP_SET_DWORD1    Dword1;
  STRAP_SET_DWORD2    Dword2;
  STRAP_SET_DWORD3    Dword3;
  STRAP_SET_DWORD4    Dword4;
  STRAP_SET_DWORD5    Dword5;
  STRAP_SET_DWORD6    Dword6;
  STRAP_SET_DWORD7    Dword7;
  STRAP_SET_DWORD8    Dword8;
} STRAP_SET_LIST;

typedef struct {
  UINT32              ConfigLength;
  S3M_CONFIG          Config;
  UINT32              StrapLength;
  STRAP_SET_LIST      StrapSet;
} S3M_DATA_FORMAT;

#endif // __S3M_STRAP_STRUCTURE_H__
