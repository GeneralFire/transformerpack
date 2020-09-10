/** @file
  MemRas functions and platform data, which needs to be
  ported.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2017 Intel Corporation. <BR>

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

#ifndef _MEM_RAS_H_
#define _MEM_RAS_H_

//
// Statements that include other header files
//

#include <Library/SmmServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/UefiDecompressLib.h>

#include <Protocol/RasMpLinkProtocol.h>
#include <Protocol/MemRasProtocol.h>
#include <Protocol/CpuCsrAccess.h>
#include <Protocol/IioUds.h>
#include <Protocol/CpuCsrAccess.h>
#include <Protocol/SmmSxDispatch2.h>
#include <Protocol/SmmVariable.h>
#include <Protocol/Decompress.h>
#include <Protocol/QuiesceProtocol.h>
#include <Protocol/CrystalRidge.h>

#include <Protocol/CpuPpmProtocol.h>
#include <Library/CpuPpmLib.h>

#include <Protocol/SmmPeriodicTimerDispatch2.h>
#include <Chip/Include/SysHostChip.h>


#define ENTRY_INVALID 0xFF

#define MAX_REGION_EP  1
#define MAX_REGION_EX  2
#define MAX_REGION_SVL 4
#define MAX_STRIKE    3
#define BANK_STRIKE 1
#define RANK_STRIKE 0
#define RANKVALID   0
#define BANKVALID   1
#define DEVICEVALID 2
#define PCODE_QUIESCE_TIME_50_MS       50



extern   EFI_CPU_CSR_ACCESS_PROTOCOL    *mCpuCsrAccess;

UINT32   SchedulingCall (VOID *, ...);  // The parameter MUST be less or equal to 4

#define MEM_NODE_ON                     BIT0
#define MEM_NODE_OFF                    BIT1
#define MEM_RCOMP_OFF                   BIT2
#define MEM_DAT_UPDATE                  BIT3
#define MEM_NODE_INIT                   BIT4
#define MEM_MAP_UPDATE                  BIT5
#define MEM_SKT_ONE_TIME                BIT6
#define MEM_REQ_BOTH_BR                 BIT7

#define MEM_OFFLINE                     (MEM_NODE_INIT | MEM_MAP_UPDATE | MEM_NODE_OFF)
#define MEM_ONLINE                      (MEM_NODE_INIT | MEM_MAP_UPDATE | MEM_NODE_ON)

VOID *
EFIAPI
AsmFlushCacheLine (
  IN      VOID                      *LinearAddress
  );

extern EFI_MEM_RAS_PROTOCOL  *mMemRas;


EFI_STATUS
EFIAPI
InitializeMemRasData(
);

EFI_STATUS
EFIAPI
MemoryNodeOnline(
    IN UINT8      Node,
    IN OUT UINT8  *MemHpStatus
);

EFI_STATUS
EFIAPI
MemoryNodeOffline(
    IN UINT8      Node,
    IN OUT UINT8 *MemHpStatus
);



EFI_STATUS
EFIAPI
UpdateConfigInQuiesce(
    IN UINT8 Operation
    );



EFI_STATUS
EFIAPI
ModifyRasFlag (
    IN UINT64                         FlagBit,
    IN UINT8                          Value
);

EFI_STATUS
EFIAPI
MigrationPossible (
    IN UINT8 SrcNode,
    IN UINT8 DstNode,
    OUT BOOLEAN *MigrationPossibleFlag,
    IN BOOLEAN  IsMirrorMigration
);

EFI_STATUS
EFIAPI
SetupMemoryMigration(
    IN UINT8 SrcNodeId,
    IN UINT8 DstNodeId
 );


EFI_STATUS
EFIAPI
DisableMemoryMigration(
    IN UINT8 SrcNodeId,
    IN UINT8 DstNodeId
 );




EFI_STATUS
EFIAPI
GetCurrentMemoryNodeState (
    IN UINT8      Node,
    IN OUT UINT8 *MemState
);



EFI_STATUS
EFIAPI
OpenPamRegion(
    IN UINT8 SocketId
 );

EFI_STATUS
EFIAPI
RestorePamRegion(
    IN UINT8 SocketId
 );






#define INVALID_MEM_NODE    0xFF

extern struct SystemMemoryMapHob *mSystemMemoryMap;

//
// Macros used in Address Translation code
//
#define MAX_COL_BITS_SUPPORTED  12
#define MAX_ROW_BITS_SUPPORTED  18
#define MAX_BANK_BITS_SUPPORTED 3

#define MAX_COL_BITS_FIXED_MAP  10  // Max # of col bits that use fixed map
#define MAX_ROW_BITS_FIXED_MAP  12  // Max # of Row bits that use fixed map
#define MAX_BANK_BITS_FIXED_MAP 3   // Max # of bank its that use fixed map

#define MAX_ROW_BITS_DYNAMIC_MAP 6  // Max # of row bits that use dynamic map
#define MAX_COL_BITS_DYNAMIC_MAP 2  // Max # of col bits that use dynamic map

#define MAX_COL_BITS_SUPPORTED_DDR4  10
#define MAX_ROW_BITS_SUPPORTED_DDR4  18
#define MAX_BANK_BITS_SUPPORTED_DDR4 2
#define MAX_BG_BITS_SUPPORTED_DDR4   2
#define MAX_CS_BITS_SUPPORTED_DDR4   3

#define MAX_COL_BITS_FIXED_MAP_DDR4  10  // Max # of col bits that use fixed map
#define MAX_ROW_BITS_FIXED_MAP_DDR4  12  // Max # of Row bits that use fixed map
#define MAX_BANK_BITS_FIXED_MAP_DDR4 2   // Max # of bank its that use fixed map
#define MAX_BG_BITS_FIXED_MAP_DDR4   2   // Max # of bank group bits that use fixed map

#define MAX_ROW_BITS_DYNAMIC_MAP_DDR4 6  // Max # of row bits that use dynamic map
#define MAX_COL_BITS_DYNAMIC_MAP_DDR4 0  // Max # of col bits that use dynamic map

//
// Opcode used by address translation functions
//
#define EXTRACT_ADDR_BITS        0
#define STUFF_ADDR_BITS          1


typedef enum {

  SVQuiesceUnQuiesce = 1,   //1
  SVMemoryFailOver,         //2
  SVMemoryOnOffLine,        //3
  SVMemoryMigration,        //4
  SVIohOnOffline,           //5
  SVCpuOnOffline,           //6
  Reserved_1,               //7
  SVChipSparing_StartSpare, //8
  SVRankSparing_SetFail,    //9 - Not supported in Platform
  SVSet_LeakyBucket,        //10- Not supported in Platform
  SVSmiExtender,            //11- Not supported in Platform
  SVMemoryChipUnerase,      //12
  SVMemoryPatrolScrub,      //13- Not supported in Platform
  SVQuiesceCodeRun,         //14
} SV_SMM_COMMAND;


#define ECC_MODE_REG_LIST      {ECC_MODE_RANK0_MC_MAIN_REG, ECC_MODE_RANK1_MC_MAIN_REG, ECC_MODE_RANK2_MC_MAIN_REG, ECC_MODE_RANK3_MC_MAIN_REG, ECC_MODE_RANK4_MC_MAIN_REG, ECC_MODE_RANK5_MC_MAIN_REG, ECC_MODE_RANK6_MC_MAIN_REG, ECC_MODE_RANK7_MC_MAIN_REG}
#ifdef SKX_HOST
#define ECC_MODE_REG_EXT_LIST  {ECC_MODE_RANK0_MC_MAINEXT_REG, ECC_MODE_RANK1_MC_MAINEXT_REG, ECC_MODE_RANK2_MC_MAINEXT_REG, ECC_MODE_RANK3_MC_MAINEXT_REG, ECC_MODE_RANK4_MC_MAINEXT_REG, ECC_MODE_RANK5_MC_MAINEXT_REG, ECC_MODE_RANK6_MC_MAINEXT_REG, ECC_MODE_RANK7_MC_MAINEXT_REG}
#endif //SKX_HOST

#endif  //_MEM_RAS_H_
