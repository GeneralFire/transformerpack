/** @file
  HECI utilities for use in PEI, DXE, and SMM.

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 - 2020 Intel Corporation.

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

**/

#ifndef _ME_UTILS_LIB_H_
#define _ME_UTILS_LIB_H_

#include <Library/SmmServicesTableLib.h>

/**
  Get Information is we are running in SMM or not

  @param[in, out]         Pointer to EFI_SMM_SYSTEM_TABLE2 pointer

  @retval  TRUE           We are in SMM
  @retval  FALSE          We are not in SMM
**/
BOOLEAN
IsInSmm (
  IN OUT EFI_SMM_SYSTEM_TABLE2  **SmmSystemTable2
  );

/**
  Return ME Mode

  @param [out] MeMode             Pointer for ME Mode report

  @retval EFI_SUCCESS             MeMode copied
  @retval EFI_INVALID_PARAMETER   Pointer of MeMode is invalid
**/
EFI_STATUS
MeBiosGetMeMode (
  OUT UINT32                       *MeMode
  );

/**
  Get an Intel ME Firmware Status from HECI-1 HFS register.

  @param [out] MeStatus           Pointer for status report


  @retval EFI_SUCCESS             MeStatus copied
  @retval EFI_INVALID_PARAMETER   Pointer of MeStatus is invalid
  @retval EFI_DEVICE_ERROR        Read device is not HECI
**/
EFI_STATUS
MeBiosGetMeStatusRaw (
  OUT UINT32                      *MeStatus
  );

/**
  Get an abstract Intel ME Firmware Status from HECI-1 HFS register.
  This is used to control BIOS flow for various Intel ME functions.

  @param [out] MeStatus           Pointer for status report
                                  see MeState.h - Abstract ME status definitions.

  @retval EFI_SUCCESS             MeStatus copied
  @retval EFI_INVALID_PARAMETER   Pointer of MeStatus is invalid
**/
EFI_STATUS
MeBiosGetMeStatus (
  OUT UINT32                      *MeStatus
  );

/**
  Function forces a reinit of the heci interface by following the reset heci interface via host algorithm
  in HPS 0.90 doc 4-17-06

  @retval EFI_TIMEOUT             ME is not ready
  @retval EFI_SUCCESS             Interface reset
  @retval EFI_UNSUPPORTED         Not supported
**/
EFI_STATUS
ResetHeci1Interface (
  VOID
  );

/**
  Get HECI PCI Segment

  @retval HECI PCI Segment
**/
UINTN
GetHeciPciSegment (
  VOID
  );

/**
  Get HECI PCI Bus

  @retval HECI PCI Bus
**/
UINTN
GetHeciPciBus (
  VOID
  );

/**
  Get HECI PCI Device Number

  @retval HECI PCI Device Number
**/
UINTN
GetHeciPciDevice (
  VOID
  );

/**
  Checks if Me FW is Client and it is ready

  @retval FALSE           Not ME Client and ready
  @retval TRUE            Ready Client ME detected
**/
BOOLEAN
MeClientIsReady (
  VOID
  );

/**
  Checks if Me FW is Client Corporate

  @retval FALSE           Not ME Client corporate
  @retval TRUE            Corporate Client ME detected
**/
BOOLEAN
MeClientIsCorporate (
  VOID
  );

/**
  Checks if Me FW is SPS ready

  @retval FALSE           SPS is not ready
  @retval TRUE            Ready SPS detected
**/
BOOLEAN
MeSpsIsReady (
  VOID
  );

/**
  Checks if Me FW is SPS in recovery

  @retval FALSE           SPS is not in recovery
  @retval TRUE            SPS is in recovery
**/
BOOLEAN
MeSpsInRecovery (
  VOID
  );

/**
  Checks if Me FW is Client in manufacturing

  @retval FALSE           Client is not in recovery
  @retval TRUE            CLient is in recovery
-**/
BOOLEAN
MeClientInRecovery (
  VOID
  );

/**
  Checks if Me is in manufacturing

  @retval FALSE           ME is not in recovery
  @retval TRUE            ME is in recovery
**/
BOOLEAN
MeIsInRecovery (
  VOID
  );

/**
  Checks if Me FW is SPS NM

  @retval FALSE           SPS NM not detected
  @retval TRUE            SPS NM detected
**/
BOOLEAN
MeSpsIsNm (
  VOID
  );

/**
  Reads ME Firmware Status 1 information from HOB

  @param [in] Mefs1       Current Mefs1 that will be return in error case

  @retval On Error:       Passed in Mefs1
  @retval On Success:     MEFS1

**/
UINT32
GetMeFwsts1FromHob (
  IN UINT32 Mefs1
  );

///
/// HECI_API_VERSION defines
/// what HECI API is returned by GetHeci ()
///
typedef enum {
  HeciApiHeciProtocol,            // Legacy HECI
  HeciApiHeciControl,             // HECI_CONTROL
  HeciApiUnknown = MAX_UINT32
} HECI_API_VERSION;

/**
  Gets Heci PPI/Protocol, returns pointer to HECI PPI/Protocol

  @param[out] HeciVersion       HECI interface API Version

  @retval Valid pointer         Pointer to HECI PPI/Protocol
  @retval NULL                  Error, PPI/Protocol not found
**/
VOID*
GetHeci (
  OUT HECI_API_VERSION *HeciVersion
  );

/**
  Wrapper for Heci HeciSendAndReceive HECI_CONTROL function

  @param[in]     This          Heci instance
  @param[in]     HeciPchId     PchId we want to communicate with 0..N
                               HECI_DEFAULT_PCH_ID Id is default and
                               HeciControl decides where to route communication
  @param[in,out] Timeout       On input timeout in us, on exit time left
  @param[in]     Retries       The number of retries requested (0 => no retry)
  @param[in]     HostAddress   Host address
  @param[in]     EngineAddress Engine (ME/IE) address
  @param[in]     ReqMsg        Request message
  @param[in]     ReqLen        Request message length in bytes
  @param[out]    RspMsg        Buffer for the response message
  @param[in,out] RspLen        On input buffer size, on exit message, in bytes
  @param[in,out] SendStatus    Send operation status
  @param[in,out] ReceiveStatus Receive operation status

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_WRITE_PROTECTED   Can't find suitable HECI Transport
                                Possible reason: HECI filter active
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Message is too big
  @retval EFI_SUCCESS           Message request has been completed successfully
**/
EFI_STATUS
HeciSendWithAck (
  IN     VOID                *This,
  IN     UINT32              HeciPchId,
  IN OUT UINT32              *Timeout,
  IN     UINT8               Retries,
  IN     UINT8               HostAddress,
  IN     UINT8               EngineAddress,
  IN     UINT32              *ReqMsg,
  IN     UINT32              ReqLen,
     OUT UINT32              *RspMsg,
  IN OUT UINT32              *RspLen,
  IN OUT EFI_STATUS          *SendStatus,
  IN OUT EFI_STATUS          *ReceiveStatus
  );

/**
  Get InitComplete from Firmware Status

  @param[out] ReturnMeFs1 Pointer to return ME Firmware Status 1

  @retval FALSE  InitComplete bit is not set
  @retval TRUE   InitComplete bit is set
**/
BOOLEAN
GetInitComplete (
  OUT UINT32 *ReturnMeFs1
  );

/**
  Get SPS End Of Post from Firmware Status

  @param[out] ReturnMeFs2 Pointer to return ME Firmware Status 2

  @retval FALSE  EndOfPost bit is not set
  @retval TRUE   EndOfPost bit is set
**/
BOOLEAN
GetSpsEndOfPostStatus (
  OUT UINT32 *ReturnMeFs2
  );

/**
  Returns the GroupId and Me Client Address for former MCA
  messages

  If no MhkiVersion HOB  exists use HECI_MCHI_MESSAGE_ADDR
  and MKHI_MCA_GROUP_ID as a default.

  @param[in]  Command                Former MCA Command
  @param[out] GroupId                GroupId for the given command
  @param[out] MeClientAddress        Me Client for the given command
**/
VOID
GetMcaMsgParams (
  IN  UINT32 Command,
  OUT UINT8  *GroupId,
  OUT UINT8  *MeClientAddress
  );

#endif // _ME_UTILS_LIB_H_
