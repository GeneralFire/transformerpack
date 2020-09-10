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

#ifndef _DXE_ONE_TOUCH_TPM20_H_
#define _DXE_ONE_TOUCH_TPM20_H_

//APTIOV_SERVER_OVERRIDE_START
//#include <Library/Tpm2CommandLib.h>
//APTIOV_SERVER_OVERRIDE_END
#include <Protocol/Tcg2Protocol.h>

#include "DxeOobOneTouch.h"
#include "DxeOneTouchPlatform.h"

//-----------------------------------------------------------------------------------------------------
#pragma pack (push, 1)
//-----------------------------------------------------------------------------------------------------
// Max size of TPM Response Buffer (excluding TPM Response Header)
#define MAX_TPM2_DATA_SIZE_B   1056
//
//-----------------------------------------------------------------------------------------------------
// TPM2.0 NV Index
#define TPM20_NV_INDEX_AUX            0x01C10102     // Aux Index
#define TPM20_NV_INDEX_PS             0x01C10103     // PS Index
#define TPM20_NV_INDEX_PO             0x01C10106     // PO Index
//
// Bit-definitions in TPM2.0 NV Index
#define TPM20_NV_INDEX_BIT_MASK       0x00FFFFFF     // BIt23-0 = NV Index
#define   TPM20_NV_INDEX_BIT_N        0x00           //   #of bits shift for alignment
#define TPM20_RH_NV_BIT_MASK          0xFF000000     // Bi31-24 = Constant value indicating NV Index range
#define   TPM20_RH_NV_BIT_N           0x18           //   #of bits shift for alignment
#define   TPM20_RH_NV_TPM_HT_NV_INDEX 0x01           //   01 = NV Index assigned by caller
//
// Bit-definitions in TPMA_NV (NV Attribute)
#define TPMA_NV_PPWRITE_BIT           0x00000001     // Bit-0  = 0/1, Index can not/yes be written with Platform Authorization
#define TPMA_NV_OWNERWRITE_BIT        0x00000002     // Bit-1  = 0/1, Index can not/yes be written with Owner Authorization
#define TPMA_NV_AUTHWRITE_BIT         0x00000004     // Bit-2  = 0/1, Index contents can not/yes be changed with HMAC session or password
#define TPMA_NV_POLICYWRITE_BIT       0x00000008     // Bit-3  = 0/1, Index contents can not/yes be changed with policy session
#define TPMA_NT_BIT_MASK              0x000000F0     // Bit7-4........Index Type
#define   TPMA_NT_BIT_N               0x04           //   #of bits shift for alignment
#define   TPMA_NT_ORDINARY            0x00           //   0x00 = Ordinary can be modified only using TPM2_NV_Write()
#define TPMA_NV_RESERVED_1_BIT_MASK   0x00000300     // Bit9-8........Reserved
#define TPMA_NV_POLICY_DELETE_BIT     0x00000400     // Bit-10 = 0/1, Index can be deleted (Platform or Owner) Authosization/AuthPolicy
#define TPMA_NV_WRITELOCKED_BIT       0x00000800     // Bit-11 = 0/1, Index can not/yes be written
#define TPMA_NV_WRITEALL_BIT          0x00001000     // Bit-12 = 0/1, Index partial write yes/not allowed
#define TPMA_NV_WRITEDEFINE_BIT       0x00002000     // Bit-13 = 0/1, Index can not/yes be locked from further writes by TPM2_NV_WriteLock()
#define TPMA_NV_WRITE_STCLEAR_BIT     0x00004000     // Bit-14 = 0/1, Index can not/yes be prevented from writes by TPM2_NV_WriteLock() until next TPM Reset/Restart
#define TPMA_NV_GLOBALLOCK_BIT        0x00008000     // Bit-15 = 0/1, Index can not/yes be be prevented from writes by TPM2_NV_GlobalWriteLock() until next TPM Reset/Restart
#define TPMA_NV_PPREAD_BIT            0x00010000     // Bit-16 = 0/1, Index can not/yes be read with Platform Authorization
#define TPMA_NV_OWNERREAD_BIT         0x00020000     // Bit-17 = 0/1, Index can not/yes be read with Owner Authorization
#define TPMA_NV_AUTHREAD_BIT          0x00040000     // Bit-18 = 0/1, Index can not/yes be read with HMAC session or password
#define TPMA_NV_POLICYREAD_BIT        0x00080000     // Bit-19 = 0/1, Index can not/yes be read with policy session
#define TPMA_NV_RESERVED_2_BIT_MASK   0x01F00000     // Bit24-20......Reserved
#define TPMA_NV_NO_DA_BIT             0x02000000     // Bit-25 = 0/1, Authorization failure yes/not affect DA Logic and Authorization of Index yes/not blocked in Lockout mode
#define TPMA_NV_ORDERLY_BIT           0x04000000     // Bit-26 = 0,   Index State is always saved after any update
                                                     //          1,   Index State is only saved after an orderly TPM shutdown
#define TPMA_NV_CLEAR_STCLEAR_BIT     0x08000000     // Bit-27 = 0/1, TPMA_NV_WRIITEN_BIT not/yes cleared by TPM Reset/Restart
#define TPMA_NV_READLOCKED_BIT        0x10000000     // Bit-28 = 0/1, Index read not/yes blocked until next TPM Reset/Restart
#define TPMA_NV_WRITTEN_BIT           0x20000000     // Bit-29 = 0/1, Index not/yes written
#define TPMA_NV_PLATFORMCREATE_BIT    0x40000000     // Bit-30 = 0,   Index can be undefined by Owner Authorization but not with Platform Authorization
                                                     //          1,   Index can be undefined by Platform Authorization but not with Owner Authorization
#define TPMA_NV_READ_STCLEAR_BIT      0x80000000     // Bit-31 = 0/1, TPM2_NV_ReadLock() can not/yes be used to set TPMA_NV_READLOCKED_BIT
//
// Restrictions in NV Attribute: Some bits must be 0 and some bits must be 1
#define TPMA_NV_MUST_0_BIT_MASK     (TPMA_NV_POLICY_DELETE_BIT | TPMA_NV_ORDERLY_BIT | TPMA_NV_CLEAR_STCLEAR_BIT)
#define TPMA_NV_MUST_1_BIT_MASK     (TPMA_NV_NO_DA_BIT)
#define TPM20_NV_ATTRIBUTE_DEFAULT  (TPMA_NV_READ_STCLEAR_BIT | TPMA_NV_PLATFORMCREATE_BIT | TPMA_NV_NO_DA_BIT | TPMA_NV_AUTHREAD_BIT | TPMA_NV_WRITE_STCLEAR_BIT | TPMA_NV_AUTHWRITE_BIT)
//
// TPM2.0 Structures
//
typedef struct {
  TPM2_RESPONSE_HEADER   Header;
  UINT8  ResponseParams  [MAX_TPM2_DATA_SIZE_B];
} TPM2_RESPONSE;
//
typedef struct {
  TPM2_COMMAND_HEADER    Header;
  TPMI_RH_NV_INDEX       NvIndex;
} TPM2_NV_READPUBLIC_COMMAND;
//
typedef struct {
  TPM2_RESPONSE_HEADER   Header;
  TPM2B_NV_PUBLIC        NvPublic;
  TPM2B_NAME             NvName;
} TPM2_NV_READPUBLIC_RESPONSE;
//
typedef struct {
  TPM2_COMMAND_HEADER    Header;
  TPMI_RH_PROVISION      AuthHandle;
  UINT32                 AuthSessionSize;
  TPMS_AUTH_COMMAND      AuthSession;
  TPM2B_AUTH             Auth;
  TPM2B_NV_PUBLIC        NvPublic;
} TPM2_NV_DEFINESPACE_COMMAND;

typedef struct {
  TPM2_RESPONSE_HEADER   Header;
  UINT32                 AuthSessionSize;
  TPMS_AUTH_RESPONSE     AuthSession;
} TPM2_NV_DEFINESPACE_RESPONSE;
//
typedef struct {
  TPM2_COMMAND_HEADER    Header;
  TPMI_RH_PROVISION      AuthHandle;
  TPMI_RH_NV_INDEX       NvIndex;
  UINT32                 AuthSessionSize;
  TPMS_AUTH_COMMAND      AuthSession;
} TPM2_NV_UNDEFINESPACE_COMMAND;

typedef struct {
  TPM2_RESPONSE_HEADER   Header;
  UINT32                 AuthSessionSize;
  TPMS_AUTH_RESPONSE     AuthSession;
} TPM2_NV_UNDEFINESPACE_RESPONSE;
//
typedef struct {
  TPM2_COMMAND_HEADER    Header;
  TPMI_RH_NV_AUTH        AuthHandle;
  TPMI_RH_NV_INDEX       NvIndex;
  UINT32                 AuthSessionSize;
  TPMS_AUTH_COMMAND      AuthSession;
  UINT16                 Size;
  UINT16                 Offset;
} TPM2_NV_READ_COMMAND;

typedef struct {
  TPM2_RESPONSE_HEADER   Header;
  UINT32                 AuthSessionSize;
  TPM2B_MAX_BUFFER       Data;
  TPMS_AUTH_RESPONSE     AuthSession;
} TPM2_NV_READ_RESPONSE;

typedef struct {
  TPM2_COMMAND_HEADER    Header;
  TPMI_RH_NV_AUTH        AuthHandle;
  TPMI_RH_NV_INDEX       NvIndex;
  UINT32                 AuthSessionSize;
  TPMS_AUTH_COMMAND      AuthSession;
  TPM2B_MAX_BUFFER       Data;
  UINT16                 Offset;
} TPM2_NV_WRITE_COMMAND;

typedef struct {
  TPM2_RESPONSE_HEADER   Header;
  UINT32                 AuthSessionSize;
  TPMS_AUTH_RESPONSE     AuthSession;
} TPM2_NV_WRITE_RESPONSE;

typedef struct {
  TPM2_COMMAND_HEADER    Header;
  TPMI_RH_NV_AUTH        AuthHandle;
  TPMI_RH_NV_INDEX       NvIndex;
  UINT32                 AuthSessionSize;
  TPMS_AUTH_COMMAND      AuthSession;
} TPM2_NV_LOCK_COMMAND;

typedef struct {
  TPM2_RESPONSE_HEADER   Header;
  UINT32                 AuthSessionSize;
  TPMS_AUTH_RESPONSE     AuthSession;
} TPM2_NV_LOCK_RESPONSE;
//
//-----------------------------------------------------------------------------------------------------
//
EFI_STATUS
EFIAPI
Tpm2CommandClear (
  IN  TPM2B_AUTH *PlatformAuth
  );
//
//-----------------------------------------------------------------------------------------------------
//
EFI_STATUS
EFIAPI
Tpm2CommandAllocPcr (
  IN  TPM2B_AUTH *PlatformAuth,
  IN  UINT32     SupportedPCRBanks,
  IN  UINT32     PCRBanks
  );
//
//----------------------------------------------------------------------------------------------
/**
  OT_SetPcrBanksTpm20(): Set PCR Banks for TPM2.0.

  @param  Auth        INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param  Opd         INPUT:  Ptr to OT_PLATFORM_DATA
  @param  PcrBanks    INPUT:  UIN32, Bit-mapped information of PCR Banks to be set
  @param  Ci          INPUT:  BOOLEAN*, Ptr to Boolean Variable
                              OUTPUT: TRUE  -> Set PCR Banks command issued
                              OUTPUT: FALSE -> Set PCR Banks command NOT issued
  @param  EFI_STATUS  EFI_SUCCESS  Success
                      Other        Error
**/
EFI_STATUS
OT_SetPcrBanksTpm20 (
  IN     TPM2B_AUTH       *Auth,
  IN OUT OT_PLATFORM_DATA *Opd,
  IN     UINT32           PcrBanks,
  IN OUT BOOLEAN          *Ci
  );
//
//-----------------------------------------------------------------------------------------------------
/**
  This function returns the information of different TPM2.0 NV Index.

  @param    Opd    INPUT: OT_PLATFORM_DATA*, ptr to OT_PLATFORM_DATA

  @retval   Info   OUTPUT: UINT16, Bit-mapped information of different TPM NV Index
                           (as defined in OXP_HEADER.FeatureEnabled field)
**/
UINT16
OT_GetNvIndexInfoTpm20 (
  IN  OT_PLATFORM_DATA  *Opd
  );
//
//-----------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------
#pragma pack (pop)
//-----------------------------------------------------------------------------------------------------

#endif
