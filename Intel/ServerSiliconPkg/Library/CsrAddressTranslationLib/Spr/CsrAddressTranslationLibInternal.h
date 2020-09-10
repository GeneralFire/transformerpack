/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef __CSR_ADDRESS_TRANSLATION_LIB_INTERNAL_H__
#define __CSR_ADDRESS_TRANSLATION_LIB_INTERNAL_H__

#include <UsraAccessType.h>
#include <UncoreCommonIncludes.h>
#include <RcRegs.h>
#include <Library/CsrPseudoOffsetConvertLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/SystemInfoLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/KtiSimLib.h>
#include <Library/CsrSbPortIdLib.h>
#include <Library/SimRegsLookupLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/KtiApi.h>
#include <IioRegs.h>

#define  MAX_FUNCBLK                     8
#define  CONVERT_FAILED                  0
#define  MAX_DINO_INSTANCE_SPR           4
#define  MAX_DINO_INSTANCE_SPR_1D        1
#define  MAX_PI5_INSTANCE_SPR            6
#define  MAX_PI5_INSTANCE_SPR_1D         2
#define  MAX_PI5_PORT                    41
#define  MAX_PI5_PORT_1D                 9

//
// Ubox decodes MMIO Address [18] to send MMIO writes to the Memory Subsystem (MemSS) using Posted semantics
//
#define MEMSS_POSTED_WRITE_MASK      BIT18

//
// Get Aligned Access Address
//
typedef UINTN (EFIAPI *GET_CSR_PHYSICAL_ADDRESS) (UINT8, UINT8, CSR_OFFSET, CPU_CSR_ACCESS_VAR*,UINT8);


// to be update to merge Dev/Func
#define PCIE_REG_ADDR(Bus,Device,Function,Offset) \
    (((Offset) & 0xfff) | (((Function) & 0x07) << 12) | (((Device) & 0x1f) << 15) | (((Bus) & 0xff) << 20))

//  0. CHA-MISC
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type CHA_MISC device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_CHA_MISC (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );


// 1. CHA-PMA
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type CHA_PMA device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_CHA_PMA (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );


// 2. CHA-CMS
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type CHA_CMS device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_CHA_CMS (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

//  3. CHA-ABC
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type CHA_ABC device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_CHA_ABC (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

//  4. PCU
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type PCU device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_PCU (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

//  6. M2MEM
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type M2MEM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_M2MEM (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// 7. MC
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type MC device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_DDR_MC (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// 7. MC_GLOBAL
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type MC_GLOBAL device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_DDR_MC_GLOBAL (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// 8. MCIO
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type MCIO device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_MCIO(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

//  9. KTI
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type KTI device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_KTI (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

//  10. M3KTI
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type M3KTI device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_M3KTI (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// 11. MCDDC
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type MCDDC device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_DDR_MCDDC (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// 14 . IIO
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type IIO_EXPPTMBAR device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_IIO_EXPPTMBAR(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// 17 IIO VTD.
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type IIO VTD/RAS/VMD device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_IIO_VTD (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// 18. IIO DFX
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type IIO_DFX device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_IIO_DFX (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

//  19. UBOX
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type UBOX device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_UBOX (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// 20. MS2IDI MAIN
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type MS2IDI_MAIN device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_MS2IDI_MAIN(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// 21. HBM
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type HBM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_HBM_AWORD (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type HBM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_HBM_DWORD (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// Per even channel to program the HBMIO_FUSE_TUNING
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type HBM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_HBM_MIDSTACK (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type HBM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_HBM_MID_GBL (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// 46  HBM2E MC
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type HBM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_HBM_MC (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type HBM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_HBM_CPGC (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type HBM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_HBM_MC_GLOBAL (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// 23. MSM
/**

  Convert to physical address a for given SocketId, BoxInst, CsrOffset & CpuCsrAccessVarPtr
  for Box type MSM device.

  @param SocketId            - The Socket ID
  @param BoxInst             - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_MSM(
  IN UINT8               SocketId,
  IN UINT8               BoxInst,
  IN CSR_OFFSET          CsrOffset,
  IN CPU_CSR_ACCESS_VAR* CpuCsrAccessVarPtr,
  IN UINT8               InstType
  );

//  25. SB
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type SB device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_SB(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

//  28. SMEE
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type SMEE device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_SMEE(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );


// 30. NPK
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type NPK device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_NPK(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type NPK Memory device.  This function uses MTB bar on NPK to translate physical
  addresses to access registers in North Peak Mem space.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_NPK_MEM(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// 31. IEH
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type IEH_SATELLITE device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_IEH_SATELLITE(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type IEH_GLOBAL device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_IEH_GLOBAL(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// 32. SPK
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type SPK device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_SPK(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// 33. DINO
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type DSA/IAX device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_DSA_IAX(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type CPM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_CPM(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

  /**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type HQM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_HQM(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// 34. IIO_M2IAL
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type IIO_M2IAL device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_IIO_M2IAL(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// 35. PI5
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type DMI_IAL device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_IIO_PCIEDMI(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type NTB device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_IIO_PCIENTB(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type CXL.IO device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_CXL_RCRBBAR(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for CXL DP's MEMBAR0 region (cxl.cm, cxl.am).

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_CXL_DP_MBAR(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type IIO_PCIE device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_IIO_PCIE(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// 38. DRA
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type DRA_MAIN device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_DRA_MAIN (
  IN UINT8               SocketId,
  IN UINT8               InstId,
  IN CSR_OFFSET          CsrOffset,
  IN CPU_CSR_ACCESS_VAR* CpuCsrAccessVarPtr,
  IN UINT8               InstType
  );

// 39. MCCADB
/**

  Convert to physical address for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type MCCADB device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_MCCADB(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// 40. MCCPGC
/**

  Convert to physical address for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type MCCPGC device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_MCCPGC(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

// 41 MDFIS
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type MDFIS device.

  @param[in] SocketId             The Socket ID
  @param[in] InstId               Box Instance, 0 based
  @param[in] CsrOffset            Format of CSR register offset
  @param[in] CpuCsrAccessVarPtr   A pointer to the CSR access variable
  @param[in] InstType             The Box Instance Type: CsrBoxInst or CsrChId

  @retval                         The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_MDFIS (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

//  42. BOX_SB_I3C
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type BOX_SB_I3C device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId

  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_I3C (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );

//  45. BOX_S3M
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type BOX_S3M device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_S3M (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  );


/**

  Use Hbm instance with DDR MC/CPGC register offset to get equavalent HBM MC/CPGC registers.

  @param Func                - The target HBM convert function
  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @param PhysicalAddress     - The physical address converted from USRA address, the maximum will be set if not in HBM scope

  @retval NULL

**/
VOID
EFIAPI
HbmCompat (
  IN GET_CSR_PHYSICAL_ADDRESS   Func,
  IN UINT8                      SocketId,
  IN UINT8                      InstId,
  IN CSR_OFFSET                 CsrOffset,
  IN CPU_CSR_ACCESS_VAR*        CpuCsrAccessVarPtr,
  IN UINT8                      InstType,
  OUT UINTN                     *PhysicalAddress
  );
#endif //#ifndef __CSR_ADDRESS_TRANSLATION_LIB_INTERNAL_H__
