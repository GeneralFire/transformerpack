/** @file
  Interface of memory topology Data library.

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

#ifndef __PLAT_MEM_ERROR_DATA_LIB_H__
#define __PLAT_MEM_ERROR_DATA_LIB_H__

#include <Guid/Cper.h>
#include <Emca.h>

#define   IFU_POISON_DETECTED         0xC
#define   DCU_POISON_DETECTED         0x10

#define   MC_CE_PATROL_SCRUB          0x08
#define   MC_UC_PATROL_SCRUB          0x10

#define   DDR4_CAP_ERROR              0x200
#define   DDRT_CAP_ERROR              0x80D
#define   DDR_LINK_FAILURE            0x108

#define   MEM_ECC_ERROR               0x05
#define   MEM_MIRROR_FAILOVER         0x06

#define   DIMM_TYPE_DDR4              BIT0
#define   DIMM_TYPE_DDRT              BIT1
#define   DIMM_TYPE_UNKNOWN           0

#define   MAX_POISON_RECORD   20
#define   CACHELINE_MASK      0xFFFFFFFFFFFFFFC0
#define   CACHELINE_MASK_CR   0xFFFFFFFFFFFFFF00

#pragma  pack(1)

typedef struct {
  BOOLEAN    RecordValid;
  UINT64     PhysicalAddress;
  UINT64     PhysicalAddressMask;
  UINT16     Node;                 // Node #
  UINT16     Card;
  UINT16     ModuleRank;           // Module or Rank#
  UINT16     RankNum;
  UINT16     Bank;
  UINT16     Device;
  UINT16     Row;
  UINT16     Column;
  UINT8      ErrorType;
  UINT8      Extended;
  UINT64     ValidFields;
} POISON_RECORD;

typedef struct {
  POISON_RECORD   Records[MAX_POISON_RECORD];
  BOOLEAN         Overflow;
  UINT32          Pivot;
} POISON_TRACK;

typedef struct{
  UINT8   Socket;
  UINT8   SktCh;
  UINT8   Rank;
  UINT8   SubRank;
  UINT8   Dimm;
  UINT8   Dev;
  UINT8   Bank;
  UINT32  Row;
  UINT32  Col;
} DIMM_ERROR_INFO;

#pragma  pack()

EFI_STATUS
EFIAPI
FillPlatformMemoryErrorData (
  IN     UINT8                              Socket,
  IN     EMCA_MC_SIGNATURE_ERR_RECORD       *McErrSig,
  IN     BOOLEAN                            PostInvoke,
  IN OUT EFI_PLATFORM_MEMORY_ERROR_DATA     *ErrorData
  );



VOID
EFIAPI
FillCorrectablMemoryErrorData (
  IN        UINT8                               SubType,
  IN        DIMM_ERROR_INFO                     *DimmErrorInfo,
  IN OUT    EFI_PLATFORM_MEMORY_ERROR_DATA     *ErrorData
  );

/**
  Creates the memory error record based on the inputs.

  This function will only handle correctable errors.

  @param[in]  Type      - The error type
  @param[in]  SubType   - The error sub type
  @param[in]  Node      - The node the error was detected on
  @param[in]  Channel   - The channel the error was detected on
  @param[in]  Rank      - The rank the error was detected on
  @param[in]  Dimm      - The dimm the error was detected on
  @param[in]  McaCode   - The MCA code for the error

  @retval None
**/
VOID
EFIAPI
FillFnvErrorData (
  IN  UINT8                                           Socket,
  IN  UINT8                                           Ch,
  IN  UINT8                                           Dimm,
  IN  UINT8                                           ErrorType,
  IN  UINT64                         CONST  *  CONST  Dpa,
  IN  BOOLEAN                                         ReportSocketOnly,
  OUT EFI_PLATFORM_MEMORY_ERROR_DATA        *  CONST  ErrorData
  );
#endif
