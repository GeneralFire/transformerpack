/** @file
  Provide cached access for CPGC 2 registers.

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

#ifndef _CPGCREGCACHE_H
#define _CPGCREGCACHE_H

#include "CpgcChip.h"

#define CPGC_CACHE_SIGNATURE 0xCADBCADB

//
// CPGC/CADB Register Read/Write flag
//
#define CPGC_REG_READ        0
#define CPGC_REG_WRITE       1

#define CPGC2_REGISTER_TYPE  0
#define CADB2_REGISTER_TYPE  1

#pragma pack(1)

//
// CPGC2_REG_CACHE and CADB2_REG_CACHE are the main structures to cache FREQUENTLY used CPGC and CADB registers and functions.
//
// [wo]: Write only. Don't care for register's previous value. Override with new value.
// [rw]: Support read and write. Registers are composed of multiple fields. Read register content before overriding specific filed.
//       Load default value for these registers is done in InitCpgcCacheReg().
//
// Bit field cache:       Cache for specific bit field in a reigster which is frequently accessed.
// Direct register cache: Cache for the whole register content. Multiple bit fields in a register need to be accessed.
// Function cache:        Cache for a function which could save more space instead of caching individual registers in the funtion.
//
// To caching more individual registers, the following is required:
// a. Add the register cache variable in the structure below.
// b. Add register name in the RegListCpgc or RegListCadb that InitCpgcCacheReg() could initialize it by loading default value.
// c. Return the pointer of register cache variable in GetCpgcCacheDataPointer() or GetCadbCacheDataPointer().
// Now the caller can call GetSetCpgcRegs() or GetSetCadbRegs() to access it with caching mechanism.
//
typedef struct {
  UINT32        UniseqMux[CPGC_MAX_NUMBER_DPAT_UNISEQ];        //[wo] Bit filed cache:       CPGC_DPAT_USQ_CFG_0_MCCPGC_MAIN_REG.uniseq_mode
  UINT32        UniseqLfsrSize[CPGC_MAX_NUMBER_DPAT_UNISEQ];   //[wo] Bit filed cache:       CPGC_DPAT_USQ_CFG_0_MCCPGC_MAIN_REG.uniseq_lfsr_polynomial_size
  UINT32        UniseqPat[CPGC_MAX_NUMBER_DPAT_UNISEQ];        //[wo] Direct register cache: CPGC_DPAT_UNISEQ_0_MCCPGC_MAIN_REG
  UINT32        UniseqPoly[CPGC_MAX_NUMBER_DPAT_UNISEQ];       //[wo] Direct register cache: CPGC_DPAT_UNISEQ_POLY_0_MCCPGC_MAIN_REG
  UINT32        AddrInst[CPGC_MAX_NUMBER_ADDRESS_INSTRUCTION]; //[rw] Direct register cache: CPGC2_ADDRESS_INSTRUCTION_0_MCCPGC_MAIN_REG
  UINT32        CmdPattern;     //[wo] Function cache:        Setup patterns for read, write, read aftoer write, etc
  UINT32        BaseRepeats;    //[rw] Direct register cache: CPGC2_BASE_REPEATS_MCCPGC_MAIN_REG
  UINT32        BaseColRepeats; //[rw] Direct register cache: CPGC2_BASE_COL_REPEATS_MCCPGC_MAIN_REG
  UINT32        BlockRepeats;   //[rw] Direct register cache: CPGC2_BLOCK_REPEATS_MCCPGC_MAIN_REG
  UINT32        AddrSizeN0;     //[rw] Direct register cache: CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_REG
  UINT32        AddrSizeN1;     //[rw] Direct register cache: CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_REG
  UINT32        RegionLowN0;    //[rw] Direct register cache: CPGC2_REGION_LOW_N0_MCCPGC_MAIN_REG
  UINT32        RegionLowN1;    //[rw] Direct register cache: CPGC2_REGION_LOW_N1_MCCPGC_MAIN_REG
  UINT32        BaseAddrCtl;    //[rw] Direct register cache: CPGC2_BASE_ADDRESS_CONTROL_MCCPGC_MAIN_REG
  UINT32        AddrCtl;        //[rw] Direct register cache: CPGC2_ADDRESS_CONTROL_MCCPGC_MAIN_REG
  UINT32        DataCtl;        //[rw] Direct register cache: CPGC2_DATA_CONTROL_MCCPGC_MAIN_REG
  UINT32        DpatCfg;        //[rw] Direct register cache: CPGC_DPAT_CFG_MCCPGC_MAIN_REG
  UINT32        SeqCfgB;        //[rw] Direct register cache: CPGC_SEQ_CFG_B_MCCPGC_MAIN_REG
  UINT16        SubSeqWait;     //[wo] Bit filed cache: CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG.wait_time
  UINT8         RequestDataSize;//[wo] Bit filed cache: CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_REG.request_data_size
} CPGC2_REG_CACHE;

typedef struct {
  UINT32        UniseqMux[CADB_MUX_MAX];      //[wo] Bit filed cache:       CADB_DSEL_UNISEQ_CFG_0_MCCADB_MAIN_REG.uniseq_mode
  UINT32        UniseqLfsrSize[CADB_MUX_MAX]; //[wo] Bit filed cache:       CADB_DSEL_UNISEQ_CFG_0_MCCADB_MAIN_REG.uniseq_lfsr_polynomial_size
  UINT32        UniseqPat[CADB_MUX_MAX];      //[wo] Direct register cache: CADB_DSEL_UNISEQ_PBUF_0_MCCADB_MAIN_REG
  UINT32        UniseqPoly[CADB_MUX_MAX];     //[wo] Direct register cache: CADB_DSEL_UNISEQ_POLY_0_MCCADB_MAIN_REG
  UINT32        BufLow[CADB2_LINES];          //[wo] Direct register cache: CADB_BUF_0_N0_MCCADB_MAIN_REG
  UINT32        BufHi[CADB2_LINES];           //[wo] Direct register cache: CADB_BUF_0_N0_MCCADB_MAIN_REG
  UINT32        Cfg;                          //[rw] Direct register cache: CADB_CFG_MCCADB_MAIN_REG
  UINT32        AoMrsCfg;                     //[rw] Direct register cache: CADB_AO_MRSCFG_MCCADB_MAIN_REG
  UINT32        MrsCfgN0;                     //[rw] Direct register cache: CADB_MRSCFG_N0_MCCADB_MAIN_REG
  UINT32        MrsCfgN1;                     //[rw] Direct register cache: CADB_MRSCFG_N1_MCCADB_MAIN_REG
  UINT32        Dly;                          //[rw] Direct register cache: CADB_DLY_MCCADB_MAIN_REG
  UINT32        SelCfg;                       //[rw] Direct register cache: CADB_SELCFG_MCCADB_MAIN_REG
  UINT32        DselUniseqLmn;                //[rw] Direct register cache: CADB_DSEL_UNISEQ_LMN_MCCADB_MAIN_REG
} CADB2_REG_CACHE;

//
// MAX_SOCKET dimention can be unnecessary if MRC serial mode is retired
//
typedef struct {
  UINT32           Signature[MAX_SOCKET];
  CPGC2_REG_CACHE  CpgcCache[MAX_SOCKET][MAX_CH][SUB_CH];
  CADB2_REG_CACHE  CadbCache[MAX_SOCKET][MAX_CH][SUB_CH];
} CPGC2_HOST_CACHE;

#pragma pack()

/**
  Setup CPGC command pattern for test

  @param[in]  Host               - Pointer to SysHost
  @param[in]  Socket             - Socket number
  @param[in]  Ch                 - Channel number
  @param[in]  Subch              - Sub channel number
  @param[in]  CmdPat             - Command pattern for test
                                 - PatWrRd
                                 - PatWr
                                 - PatRdWrTA
                                 - PatRd
                                 - PatWrRdTA
                                 - PatODTTA
                                 - PatRdWrRd
                                 - PatWrRdAdvDq
                                 - PatRdWr
                                 - PatDummyWr
                                 - PatPdaEnum
  @param[in]  *AddressCarry      - Structure for address carry
  @param[in]  SubSeqWait         - Number of Dclks to stall at the end of a sub-sequence

  @retval N/A

**/
VOID
SetCpgcCmdPat (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN UINT8          SubCh,
  IN UINT8          CmdPat,
  IN TAddressCarry  *AddressCarry,
  IN UINT16         SubSeqWait
  );

/**

  Set the CPGC Unisequencer pattern buffer

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket
  @param[in] Ch        - Channel number
  @param[in] SubCh     - Sub-channel number
  @param[in] Patbuf    - Pointer of Pattern Buffer

  @retval N/A

**/
VOID
SetCpgcDpatUniseqPat (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      SubCh,
  IN UINT32     *Patbuf
  );

/**

  Set the CPGC Unisequencer pattern mux

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket
  @param[in] Ch        - Channel number
  @param[in] SubCh     - Sub-channel number
  @param[in] MuxCtl    - Pointer of mux array
  @param[in] LfsrSize  - Pointer of LFSR size array

  @retval N/A

**/
VOID
SetCpgcDpatUniseqCfg (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      SubCh,
  IN UINT32     *MuxCtl,
  IN UINT32     *LfsrSize
  );

/**

  Set the CPGC Unisequencer galois polynomial

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket
  @param[in] Ch         - Channel number
  @param[in] SubCh      - Sub-channel number
  @param[in] Poly       - Pointer of polynomial

  @retval N/A

**/
VOID
SetCpgcDpatUniseqPoly (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT8       Ch,
  IN UINT8       SubCh,
  IN UINT32      *Poly
  );

/**

  Set the CADB deselect unisequencer galois polynomial

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket
  @param[in] Ch         - Channel number
  @param[in] SubCh      - Sub-channel number
  @param[in] Poly       - Pointer of polynomial

  @retval N/A

**/
VOID
SetCadbDselUniseqPoly (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT8       Ch,
  IN UINT8       SubCh,
  IN UINT32      *Poly
  );

/**

  Set the CADB deselect unisequencer pattern buffer

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket
  @param[in] Ch        - Channel number
  @param[in] SubCh     - Sub-channel number
  @param[in] Patbuf    - Pointer of pattern buffer

  @retval N/A

**/
VOID
SetCadbDselUniseqPat (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      SubCh,
  IN UINT32     *Patbuf
  );

/**

  Set the CADB deselect unisequencer pattern mux and polynomial size for LFSR mux mode

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket
  @param[in] Ch        - Channel number
  @param[in] SubCh     - Sub-channel number
  @param[in] MuxCtl    - Pointer of mux array
  @param[in] LfsrSize  - Pointer of LFSR size array

  @retval N/A

**/
VOID
SetCadbDselUniseqCfg (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      SubCh,
  IN UINT32     *MuxCtl,
  IN UINT32     *LfsrSize
  );

/**

  Set CADB buffer

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket
  @param[in] Ch        - Channel number
  @param[in] SubCh     - Sub-channel number
  @param[in] Index     - CADB buffer index
  @param[in] BufLow    - Low 32 bits CADB buffer
  @param[in] BufHigh   - High 32 bits CADB buffer

  @retval N/A

**/
VOID
SetCadbBuf (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT8         Ch,
  IN UINT8         SubCh,
  IN UINT8         Index,
  IN UINT32        BufLow,
  IN UINT32        BufHigh
  );

/**

  Get/Set CADB registers

  @param[in]     Host      - Pointer to sysHost
  @param[in]     Socket    - Socket
  @param[in]     Ch        - Channel number
  @param[in]     SubCh     - Sub-channel number
  @param[in]     RdWrFlag  - Register read/write flag: CPGC_REG_READ, CPGC_REG_WRITE
  @param[in]     RegAddr   - CADB register address
  @param[in out] Data      - Pointer to the data for reading or writing the register

  @retval N/A

**/
VOID
GetSetCadbRegs (
  IN     PSYSHOST     Host,
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        SubCh,
  IN     UINT8        RdWrFlag,
  IN     UINT32       RegAddr,
  IN OUT UINT32       *Data
  );

/**

  Get/Set CPGC registers

  @param[in]     Host      - Pointer to sysHost
  @param[in]     Socket    - Socket
  @param[in]     Ch        - Channel number
  @param[in]     SubCh     - Sub-channel number
  @param[in]     RdWrFlag  - Register read/write flag: CPGC_REG_READ, CPGC_REG_WRITE
  @param[in]     RegAddr   - CPGC register address
  @param[in out] Data      - Pointer to the data for reading or writing the register

  @retval N/A

**/
VOID
GetSetCpgcRegs (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     SubCh,
  IN     UINT8     RdWrFlag,
  IN     UINT32    RegAddr,
  IN OUT UINT32    *Data
  );

/**
  CPGC register cache initialization

  @param[in]  Socket            - Socket number

  @retval N/A

**/
VOID
InitCpgcCacheReg (
  IN UINT8 Socket
  );
#endif
