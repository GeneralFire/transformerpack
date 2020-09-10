/** @file
  SysHost.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#ifndef _SYS_HOST_H_
#define _SYS_HOST_H_

#include "SysHostChipCommon.h"



#define MAX_LINE 256

#define RC_SIM_FASTCADB 0



typedef struct sysHost SYSHOST, *PSYSHOST;

#include "SysHostChipCommon.h"
#include <Library/RcDebugLib.h>
#include "MemHost.h"
#include <Library/CteNetLib.h>
#include <Library/CsrAccessLib.h>
#include "SysRegs.h"
#include <Guid/MrcPlatformHooksGuid.h>
#include <Ppi/MrcHooksServicesPpi.h>
#include <Ppi/MrcHooksChipServicesPpi.h>
#include <Ppi/MemoryPolicyPpi.h>
#include <RcCommonIncludes.h>
#include <ReferenceCodeFatalErrors.h>

//
// -------------------------------------
// Declarations and directives
// -------------------------------------
// Reference Code (RC) revision in BCD format:
// [31:24] = Major revision number
// [23:16] = Minor revision number
// [15:8] = Release Candidate number
//
#define CCMRC_REVISION  0x00500000

#define SUCCESS 0

//
// nvramPtr DWORD ?     ; Linear pointer to hostNvram structure (optional for S3 resume path)
//                      ; The NVRAM image is returned in the nvram field of the sysHost structure
//                      ; on the normal boot path. The OEM BIOS should use SIZEOF (struct sysNvram)
//                      ; to determine NVRAM size requirements.
//                      ; NULL indicates the NVRAM pointer is not valid and internal NVRAM
//                      ; hooks will be used instead. See OemReadNvram() and OemWriteNvram() hooks
//                      ; in oemhook.c for more details.
//

#pragma pack(1)

///
/// System NVRAM structure
//
struct sysNvram {
  struct memNvram     mem;
  struct commonNvram  common;
}; //struct sysNvram

#pragma pack()

//
// -----------------------------------------------------------------------------
// Variable structures
//

typedef struct CpuidRegisterInfo {
  UINT32 Eax;
  UINT32 Ebx;
  UINT32 Ecx;
  UINT32 Edx;
} CPUID_REGISTER_INFO;

typedef struct processorCommon {
  UINT32  capid0;
  UINT32  capid1;
  UINT32  capid2;
  UINT32  capid3;
  UINT32  capid4;
  UINT32  capid5;
  UINT32  capid6lo;
  UINT32  capid6hi;
  CPUID_REGISTER_INFO  ExtCpuid7;
  CPUID_REGISTER_INFO  ExtCpuid1B;
} PROCESSOR_COMMON;

///
/// Common variable structure
///
struct commonVar {
  RC_VERSION                RcVersion;              ///< Version of the reference code last executed
  UINT32                    rcStatus;               ///< Non-zero indicates fatal error
  UINT8                     chopType[MAX_SOCKET];   ///< HCC, MCC, LCC, MCC-DE, LCC-DE (HSX family only)
  UINT8                     sbsp;                   ///< Non-zero value indicates that the socket is System BSP
  UINT16                    pmBase;                 ///< Power Management Base Address
  UINT32                    tohmLimit;
  UINT32                    JumpBuffer;

#ifdef COMPRESS_RC
  UINT32                    rcSrc;                  ///< Decompression source code pointer
  UINT32                    rcDest;                 ///< Decompression destination pointer
  UINT32                    rcDecompressSourceAddr; ///< Decompression routine address of type func(UINT8*Src, UINT8*Dest);
#endif  // #ifdef COMPRESS_RC

  UINT32                    heapBase;
  UINT32                    heapSize;

  UINT32                    oemVariable;
  EWL_PRIVATE_DATA          ewlPrivateData;         // implementation data for EWL
  struct processorCommon    procCom[MAX_SOCKET];
  UINT32                    MicroCodeRev;

#define MAX_PROMOTE_WARN_LIMIT  90
  UINT32                    promoteWarnLimit;
  UINT16                    promoteWarnList[MAX_PROMOTE_WARN_LIMIT];
  UINT32                    printfDepth;            ///< handle nested calls to get/releasePrintFControl
  UINT32                    meRequestedSize;         /// Size of the memory range requested by ME FW, in MB
  UINT32                    ieRequestedSize;         /// Size of the memory range requested by IE FW, in MB
};  //struct commonVar

///
/// System Variable structure
///
struct sysVar {
  struct memVar     mem;
  struct commonVar  common;
};  //struct sysVar

//
// ----------------------------------------------------------------
// System Ras Host structure
//
struct sysRasSetup {
  struct commonSetup  common;
};

struct sysRASVar {
  struct commonVar  common;
  MEM_CHIP_POLICY   memChipPolicy;
};

typedef struct sysRasHost {
  const struct sysRasSetup setup;  ///< constant input data
  struct sysRASVar         var;    ///< variable, volatile data
}
SYSRASHOST, *PSYSRASHOST;

// Bit definitions for commonSetup.options
//                   ; PROMOTE_WARN_EN enables warnings to be treated as fatal error
//                   ; PROMOTE_MRC_WARN_EN enables MRC warnings to be treated as fatal error
//                   ; HALT_ON_ERROR_EN enables errors to loop forever
//                   ; HALT_ON_ERROR_AUTO auto reset with Maximum Serial port debug message level when fatal error is encountered.
#define PROMOTE_WARN_EN           BIT0
#define PROMOTE_MRC_WARN_EN       BIT1
#define HALT_ON_ERROR_EN          BIT2
#define HALT_ON_ERROR_AUTO        BIT3

// -----------------------------------------------------------------------------
//

typedef enum {
  ModeDdr5SubChannel = 0,       //< Each channel operates in 40b width
  ModeDdr5FullChannel,          //< Each channel operates in 80b width
  ModeDdr5FullChannelPost40b,   //< Each channel operates in 80b width post 40b mode transition
  MaxChannelModeOperation
} CHANNEL_MODE_OPERATION;

typedef union _RMT_FLAGS {
  UINT8   Data;
  struct {
    UINT8   EnableShortFormat         :1;
    UINT8   SkipPerBitMargin          :1;
    UINT8   SkipDisplayPerBitEyes     :1;
    UINT8   SkipDisplayPerBitMargins  :1;
    UINT8   SkipRmtRxDqs              :1;
    UINT8   SkipRmtRxVref             :1;
    UINT8   SkipRmtTxDq               :1;
    UINT8   SkipRmtTxVref             :1;
  } Bits;
} RMT_FLAGS;

///
/// System Host (root structure)
///
struct sysHost {
  struct sysVar         var;    ///< variable, volatile data
  struct sysNvram       nvram;  ///< variable, non-volatile data for S3 and fast path
  UINT8   ChannelModeOperation;
  UINT32  cpuFlows; ///< Which flows to take
  UINT32  ktiFlows;
  UINT32  qpiFlows;
  UINT32  opioFlows;
  UINT32  memFlows;
  UINT32  memFlowsExt;
  UINT32  memFlowsExt2;
  UINT32  memFlowsExt3;


  UINT32  MrcHooksServicesPpi;
  UINT32  MrcHooksChipServicesPpi;

  BOOLEAN PrintCsr;                               // Enable / disable printing CSR
  BOOLEAN MtrrCheck;                              // Check number of used MTRR's
  INT32   DdrioUltSupport;
  RMT_FLAGS               RmtFlags;
};

//
// RST_CPL Bits Programming bitmask
//
#define SET_RST_CPL1_BITMASK    BIT0
#define SET_RST_CPL2_BITMASK    BIT1
#define SET_RST_CPL3_BITMASK    BIT2
#define SET_RST_CPL4_BITMASK    BIT3

//BIOS SSA PRE-PROCESSOR DEFINES

//#ifndef MAX_HOB_ENTRY_SIZE
#define MAX_HOB_ENTRY_SIZE  60*1024  //Needed for BiosSaveToBdat()
//#endif //MAX_HOB_ENTRY_SIZE

//BSSA 1.0 GUID.. #define SSA_BIOS_SERVICES_PPI_GUID  {0xBAE29D7B, 0x89BB, 0x4223, 0xAF, 0x76, 0x96, 0xD0, 0xB3, 0x24, 0x9B, 0x36}
//{0xBAE29D7B, 0x89BB, 0x4223, { 0xAF, 0x76, 0x96, 0xD0, 0xB3, 0x24, 0x9B, 0x36 }}

#define EXPONENTIAL                     (0)
#define LINEAR                          (1)

//MRC_SYSTEM_INFO
#define CPU_SOCKET_BIT_MASK             (((UINT32) (~0)) >> (32 - MAX_SOCKET))
#define BUS_WIDTH                       (64)
#define BUS_FREQ                        (1333)

//MRC_SERVER_CPGC_INFO
#define MAX_NUMBER_DQDB_CACHELINES                 64
#define MAX_NUMBER_DQDB_UNISEQS                    3
#define MAX_DQDB_UNISEQ_SEED_VALUE                 0xFFFFFF
#define MAX_DQDB_UNISEQ_L_VALUE                    0xFF
#define MAX_DQDB_UNISEQ_M_VALUE                    0xFF
#define MAX_DQDB_UNISEQ_N_VALUE                    0xFF
#define MAX_DQDB_INC_RATE_EXP_VALUE                7
#define MAX_DQDB_INC_RATE_LINEAR_VALUE             31
#define MAX_DQDB_UNISEQ_SEED_RELOAD_RATE_VAL       0x7
#define MAX_DQDB_UNISEQ_SEED_SAVE_RATE_VAL         0x3F
#define MAX_DQDB_INV_DC_SHIFT_RATE_EXP_VAL         0xF
#define MAX_NUMBER_CADB_ROWS                       16
#define MAX_NUMBER_CADB_UNISEQS                    4
#define MAX_NUMBER_CADB_UNISEQ_SEED_VALUE          0xFFFF
#define MAX_CADB_UNISEQ_L_VALUE                    0xFF
#define MAX_CADB_UNISEQ_M_VALUE                    0xFF
#define MAX_CADB_UNISEQ_N_VALUE                    0xFF
#define MAX_DUMMY_READ_L_VAL                       0xFF
#define MAX_DUMMY_READ_M_VAL                       0xFF
#define MAX_DUMMY_READ_N_VAL                       0xFF
#define MAX_LOOP_COUNT_VAL                         0x1F
#define MAX_START_DELAY_VAL                        0xFF
#define MAX_NUMBER_SUBSEQS                         10
#define MAX_BURST_LENGHT_EXP_VAL                   0x1F
#define MAX_BURST_LENGHT_LIN_VAL                   0x3F
#define MAX_INTER_SUBSEQ_WAIT_VAL                  0xFF
#define MAX_OFFSET_ADDR_UPDATE_RATE_VAL            0x1F
#define MAX_ADDRESS_INV_RATE_VAL                   0xF
#define MAX_RANK_ADDRESS_INC_RATE_EXP_VAL          0x1F
#define MAX_RANK_ADDRESS_INC_RATE_LIN_VAL          0x1F
#define MIN_RANK_ADDRESS_INC_VAL                   -4
#define MAX_RANK_ADDRESS_INC_VAL                   3
#define MAX_BANK_ADDRESS_INC_RATE_EXP_VAL          0x1F
#define MAX_BANK_ADDRESS_INC_RATE_LIN_VAL          0x1F
#define MIN_BANK_ADDRESS_INC_VAL                   -8
#define MAX_BANK_ADDRESS_INC_VAL                   7
#define MAX_ROW_ADDRESS_INC_RATE_EXP_VAL           0x1F
#define MAX_ROW_ADDRESS_INC_RATE_LIN_VAL           0x1F
#define MIN_ROW_ADDRESS_INC_VAL                    -2048
#define MAX_ROW_ADDRESS_INC_VAL                    2047
#define MAX_COL_ADDRESS_INC_RATE_EXP_VAL           0x1F
#define MAX_COL_ADDRESS_INC_RATE_LIN_VAL           0x1F
#define MIN_COL_ADDRESS_INC_VAL                   -128
#define MAX_COL_ADDRESS_INC_VAL                    127
#define MAX_NUMBER_RANK_MAP_ENTRIES                8
#define MAX_NUMBER_BANK_MAP_ENTRIES                0
#define MAX_STOP_ON_NTH_ERR_CNT_VAL                0x1F
#define MAX_NUMBER_ERROR_COUNTERS                  18
#define MAX_NUMBER_ROW_ADD_SWIZZLE_ENTRIES         6
#define MAX_ROW_ADD_SWIZZLE_VAL                    16
#define MAX_ERID_ERR_MASK_VAL                      2
#define MAX_NO_ERID_SIG                            2


//
// min/max limit for the margin parameter offset
// used for override the theoretical min/max limits
//

#define MIN_TIMING_MARGIN_PAMRAM_OFFSET         -32
#define MAX_TIMING_MARGIN_PAMRAM_OFFSET          31
#define MIN_VOLTAGE_MARGIN_PAMRAM_OFFSET        -64
#define MAX_VOLTAGE_MARGIN_PAMRAM_OFFSET         63
#define MIN_RXSAMPLER_MARGIN_PAMRAM_OFFSET      -32
#define MAX_RXSAMPLER_MARGIN_PAMRAM_OFFSET       31

#define LOWER_ROW_ADDR_SWIZZLE_CNT      6   //Number of lower row address swizzle parameters
#define UPPER_ROW_ADDR_SWIZZLE_CNT      12  //Number of upper row address swizzle parameters in first upper register
#define MAX_ROW_ADDR_SWIZZLE_VAL        16  //Maximum swizzle value


#endif // #ifndef _SYS_HOST_H_
