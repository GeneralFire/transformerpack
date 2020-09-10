/** @file

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

#include <SysHost.h>

#include <Library/EnhancedWarningLogLib.h>
#include <Library/UbaGpioPlatformConfig.h>
#include <Library/DebugLib.h>
#include <Library/OemProcMemInitHookLib.h>
// APTIOV_SERVER_OVERRIDE_RC_START
#include <AmiMrcHooksWrapperLib/AmiMrcHooksWrapperLib.h>
#include <Library/SysHostPointerLib.h>
// APTIOV_SERVER_OVERRIDE_RC_END

#include <Memory/MemPorDefinitions.h>

/**

  Platform Hook to Log warning to the warning log

  @param[in] WarningCode       - Major warning code
  @param[in] MinorWarningCode  - Minor warning code
  @param[in] Socket            - Socket number
  @param[in] Channel           - Channel number
  @param[in] Dimm              - Dimm number
  @param[in] Rank              - Rank number

  @retval None

**/
VOID
PlatformOutputWarning (
  IN UINT8    WarningCode,
  IN UINT8    MinorWarningCode,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  // APTIOV_SERVER_OVERRIDE_RC_START
  AmiPlatformOutputWarningWrapper(GetSysHostPointer (), WarningCode, MinorWarningCode, Socket, Channel, Dimm, Rank );
  // APTIOV_SERVER_OVERRIDE_RC_END
}

/**

  Platform hook to Log Error to the warning log

  @param[in] ErrorCode      - Major error code
  @param[in] MinorErrorCode - Minor error code
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] Dimm           - Dimm number
  @param[in] Rank           - Rank number

  @retval None

**/
VOID
PlatformOutputError (
  IN UINT8    ErrorCode,
  IN UINT8    MinorErrorCode,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  // APTIOV_SERVER_OVERRIDE_RC_START
  AmiPlatformOutputErrorWrapper(GetSysHostPointer (), ErrorCode, MinorErrorCode,Socket,Channel,Dimm,Rank);
  // APTIOV_SERVER_OVERRIDE_RC_END
}

/**

  Used to add warnings to the promote warning exception list

  @param[in] Host  - pointer to sysHost

  @retval None

**/
UINT32
PlatformMemInitWarning (
  IN PSYSHOST Host
  )
{
  return PlatformMemInitWarningHook (Host);
}

/**

  Platform hook called to see if the warning should be promoted to an exception

  @param[in]  Host        - Pointer to the system Host (root) structure
  @param[in]  EwlEntry    - Pointer to new EWL entry buffer

  @retval None

**/

MRC_STATUS
EFIAPI
PlatformPromoteWarningsToErrors (
  IN PSYSHOST          Host,
  IN EWL_ENTRY_HEADER  *EwlEntry
  )
{
  //
  // If this function returns error, the caller 
  // will perform a generic warning to error promotion.
  //

  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**

  Platform function for initializing the enhanced warning log.
  It is called from core prior any calls to PlatformEwlLogEntry.
  This function will initialize the enhanced warning log public
  and private data structures.

  @param Host        - Pointer to the system Host (root) structure
  @retval - SUCCESS

**/

MRC_STATUS
EFIAPI
PlatformEwlInit (
  PSYSHOST Host
  )
{
  //
  // If this function returns error, the caller 
  // will perform a generic EWL initialization.
  //

  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**

  Platform hook that is called to report a new entry is available for reporting to
  the Enhanced Warning Log.

  This function will add it to the log or return an error if there is insufficient
  space remaining for the entry.

  @param  EwlEntry    - Pointer to new EWL entry buffer
  @retval - SUCCESS if entry is added; FAILURE if entry is not added

**/
MRC_STATUS
EFIAPI
PlatformEwlLogEntry (
  EWL_ENTRY_HEADER  *EwlEntry
  )
{

  //
  // If this function returns error, the caller will
  // use the generic EwlLogEntry function.
  //

  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**

  Platform hook to change the socket Vddq

  @param[in]      Socket      - Processor number
  @param[in, out] DdrVoltage  - Points to DdrVoltage Data Buf

  @retval SUCCESS if the Vddq change was made
  @retval FAILURE if the Vddq change was not made

**/
MRC_STATUS
EFIAPI
PlatformSetVdd (
  IN UINT8        Socket,
  IN OUT UINT8    *DdrVoltage
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**

  Platform hook to release any platform clamps affecting CKE
  and/or DDR Reset

  @param[in] Socket  - Processor number

  @retval None

**/
VOID
PlatformReleaseADRClamps (
  IN UINT8 Socket
  )
{
}

/**

  Platform hook to allow a secondary ADR dection method other than PCH_PM_STS

  @param[out]   - Ptr to update with the Event

  @retval MRC_STATUS

**/

MRC_STATUS
EFIAPI
PlatformDetectADR (
  OUT UINT32 *AdrEvent
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;

} // PlatformDetectADR

/**

  Platform hook to restrict supported configurations if required.

  @param Host  - Pointer to sysHost

  @retval N/A

**/
VOID
PlatformCheckPORCompat (
  PSYSHOST Host
  )
{
  PlatformCheckPORCompatHook (Host);
}

/**

  This routine can be used to update CCUR

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
PlatformUpdateConfig (
  IN PSYSHOST Host
  )
{
  UINT32 status = SUCCESS;
  return status;
}

/**

  Platform hook for validating Current config (CCUR)

  @param[in] Host - Ptr to sysHost structure

  @retval SUCCESS

**/
UINT32
PlatformValidateConfig (
  IN PSYSHOST Host
  )
{
    UINT32 status = SUCCESS;
    return status;
}

/**

  Platform hook for overriding the memory POR frequency table

  @param[out] FreqTablePtr     - Pointer to Intel POR memory frequency table
  @param[out] FreqTableLength  - Pointer to number of DimmPOREntryStruct entries in POR Frequency Table

  @retval None

**/
VOID
PlatformLookupFreqTable (
  OUT struct DimmPOREntryStruct **FreqTablePtr,
  OUT UINT16 *FreqTableLength
  )
{

// APTIOV_SERVER_OVERRIDE_RC_START
   AmiOemLookupFreqTableWrapper(GetSysHostPointer(), FreqTablePtr, FreqTableLength);
// APTIOV_SERVER_OVERRIDE_RC_END
}

/**

  VR SVID's for MC on Oem Platform

  @param[in] Socket              - Socket number
  @param[in] McId                - Memory Controller Id

  @retval - VR SVID for given Socket\McId

**/
MRC_STATUS
EFIAPI
PlatformVRsSVID (
  IN UINT8     Socket,
  IN UINT8     McId,
  IN UINT8     *VrSid
  )
{

  //
  // If this function returns error, the caller will
  // use the generic GetVrSvid function.
  //

  return MRC_FUNCTION_NOT_IMPLEMENTED;

} // PlatformVRsSVID

/**

  Platform hook after address map is configured.

  @param[in] Host  - Pointer to sysHost

  @retval None

**/
VOID
PlatformPostAddressMapConfigured (
  IN PSYSHOST Host
  )
{

}

/**

  PlatformHandleSkuLimitViolation - Platform hook to handle SKU Limit violations.

  @param[in] Host  - Pointer to sysHost

  @retval N/A

**/
VOID
PlatformHandleSkuLimitViolation (
  IN SYSHOST  *Host
  )
{

}
