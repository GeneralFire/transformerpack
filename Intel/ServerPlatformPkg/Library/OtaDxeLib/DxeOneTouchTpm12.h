/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2018 Intel Corporation. <BR>

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

#ifndef _DXE_ONE_TOUCH_TPM12_H_
#define _DXE_ONE_TOUCH_TPM12_H_

// APTIOV_SERVER_OVERRIDE_RC_START: TPM handled by AMI TCG module.
//#include <Library/Tpm12CommandLib.h>
// APTIOV_SERVER_OVERRIDE_RC_END: TPM handled by AMI TCG module.
#include <Protocol/TcgService.h>

#include "DxeOobOneTouch.h"
#include "DxeOneTouchPlatform.h"

//-----------------------------------------------------------------------------------------------------
#pragma pack (push, 1)
//-----------------------------------------------------------------------------------------------------
// Max size of TPM Response Buffer (excluding TPM Response Header)
#define MAX_TPM_DATA_SIZE_B  1056
//
//-----------------------------------------------------------------------------------------------------
// TPM1.2 NV INDEX
#define	TPM12_NV_INDEX_AUX    0x50000003    // Aux Index
#define	TPM12_NV_INDEX_PS     0x50000001    // PS Index
#define	TPM12_NV_INDEX_PO     0x40000001    // PO Index
#define	TPM12_NV_INDEX_LOCK   0xFFFFFFFF    // Reserved by TPM Spec
#define	TPM12_NV_INDEX0       0x00000000    // Reserved by TPM Spec
#define	TPM12_NV_INDEX_DIR    0x10000001    // Reserved by TPM Spec
//
#define	TPM12_NV_INDEX_T_BIT  0x80000000    // Bit-31: Used by TPM Manufacturer
#define	TPM12_NV_INDEX_P_BIT  0x40000000    // Bit-30: Used by Platform Manufacturer
#define	TPM12_NV_INDEX_U_BIT  0x20000000    // Bit-29: Used by Platform User
#define	TPM12_NV_INDEX_D_BIT  0x10000000    // Bit-28: Used to define Index Permanently
//
// TPM1.2 Structures
//
typedef struct {
  TPM_RSP_COMMAND_HDR   Hdr;
  UINT8   ResponseParams [MAX_TPM_DATA_SIZE_B];
} TPM_RESPONSE;
//
typedef struct {
  TPM_RQU_COMMAND_HDR   Hdr;
  TPM_CAPABILITY_AREA   capArea;
  UINT32                subCapSize;
  TPM_NV_INDEX          nvIndex;
} TPM_CMD_READ_NV_DATA_PUBLIC;
//
typedef struct {
  TPM_RSP_COMMAND_HDR   Hdr;
  UINT32                respSize;     // Size of NV Public Data that follows this field
  TPM_NV_DATA_PUBLIC    nvDataPublic; // Size of NV Public Data depends of #of PCRs supported
} TPM_RSP_READ_NV_DATA_PUBLIC;
//
typedef struct {
  TPM_RQU_COMMAND_HDR   Hdr;
  //APTIOV_SERVER_OVERRIDE_START
 // TPM12_NV_DATA_PUBLIC  PubInfo;
  //APTIOV_SERVER_OVERRIDE_END
  TPM_ENCAUTH           EncAuth;
} TPM_CMD_NV_DEFINE_SPACE;

typedef struct {
  TPM_RSP_COMMAND_HDR   Hdr;
} TPM_RSP_NV_DEFINE_SPACE;

typedef struct {
  TPM_RQU_COMMAND_HDR   Hdr;
  TPM_NV_INDEX          NvIndex;
  UINT32                Offset;
  UINT32                DataSize;
} TPM_CMD_NV_READ_VALUE;

typedef struct {
  TPM_RSP_COMMAND_HDR   Hdr;
  UINT32                DataSize;
  UINT8                 Data[MAX_TPM_DATA_SIZE_B];
} TPM_RSP_NV_READ_VALUE;

typedef struct {
  TPM_RQU_COMMAND_HDR   Hdr;
  TPM_NV_INDEX          NvIndex;
  UINT32                Offset;
  UINT32                DataSize;
  UINT8                 Data[MAX_TPM_DATA_SIZE_B];
} TPM_CMD_NV_WRITE_VALUE;

typedef struct {
  TPM_RSP_COMMAND_HDR   Hdr;
} TPM_RSP_NV_WRITE_VALUE;
//
//-----------------------------------------------------------------------------------------------------
/**
  Get TPM1.2 Permanent Flags.

  @param[in] Opd         Ptr to OT_PLATFORM_DATA

  @retval    TPM_RESULT  TPM_SUCCESS, Success
                         Other, Error

**/
TPM_RESULT
OT_GetTpm12PermanentFlags (
  IN  OT_PLATFORM_DATA *Opd
  );
//
//-----------------------------------------------------------------------------------------------------
/**
  Issue TSC_PhysicalPresence command to TPM.

  @param[in] TcgProtocol EFI TCG Protocol instance.
  @param[in] PhysicalPresence The state to set the TPM's Physical Presence flags.

  @retval TPM_RESULT TPM_SUCCESS, Success
      Other, Error
**/
TPM_RESULT
OT_Tpm12PhysicalPresence (
  IN  EFI_TCG_PROTOCOL      *TcgProtocol,
  IN  TPM_PHYSICAL_PRESENCE PhysicalPresence
  );
//
//-----------------------------------------------------------------------------------------------------
/**
  OT_Tpm12DeactivateDisable(): Deactivate, Disable TPM.

  @param[in]  Opd     INPUT: Ptr to OT_PLATFORM_DATA
  @retval     Result  OUTPUT: UINT8, Status code

**/
UINT8
OT_Tpm12DeactivateDisable (
  IN OUT OT_PLATFORM_DATA *Opd
  );
//
//-----------------------------------------------------------------------------------------------------
/**
  OT_Tpm12EnableActivate(): Enable, Activate TPM.

  @param[in]  Opd    INPUT: Ptr to OT_PLATFORM_DATA
  @param[in]  Always INPUT: BOOLEAN, TRUE  --> Issue Enable and Activate always
                                     FALSE --> Issue Enable and Activate if necessary

  @retval     Result OUTPUT: UINT8, Status code

**/
UINT8
OT_Tpm12EnableActivate (
  IN  OT_PLATFORM_DATA *Opd,
  IN  BOOLEAN          Always
  );
//
//-----------------------------------------------------------------------------------------------------
/**
  OT_Tpm12ClearEnableActivate(): Clear, Enable, Activate TPM.

  @param[in]  Opd INPUT: Ptr to OT_PLATFORM_DATA

  @retval     Result OUTPUT: UINT8, Status code

**/
UINT8
OT_Tpm12ClearEnableActivate (
  IN  OT_PLATFORM_DATA *Opd
  );
//
//-----------------------------------------------------------------------------------------------------
/**
  This function returns the information of different TPM1.2 NV Index.

  @param    Opd  INPUT: OT_PLATFORM_DATA*, ptr to OT_PLATFORM_DATA

  @retval   Info OUTPUT: UINT16, Bit-mapped information of different TPM NV Index
              (as defined in OXP_HEADER.FeatureEnabled field)
**/
UINT16
OT_GetNvIndexInfoTpm12 (
  IN  OT_PLATFORM_DATA *Opd
  );
//
//-----------------------------------------------------------------------------------------------------
#pragma pack (pop)
//-----------------------------------------------------------------------------------------------------

#endif
