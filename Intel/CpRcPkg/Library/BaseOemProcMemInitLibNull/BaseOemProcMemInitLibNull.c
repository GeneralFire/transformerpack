/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#include <Library/DebugLib.h>
#include <Library/OemProcMemInitLib.h>

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

  return;

} // PlatformOutputWarning

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

  return;

} // PlatformOutputError

/**

  Platform hook pre memory init

  @param[in] Host  - Pointer to SysHost

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
PlatformPreMemoryInit (
  IN PSYSHOST Host
  )
{

  return EFI_SUCCESS;

} // PlatformPreMemoryInit

/**

  Platform hook post memory init

  @param[in] Host  - Pointer to SysHost

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
PlatformPostMemoryInit (
  IN PSYSHOST Host
  )
{

  return EFI_SUCCESS;

} // PlatformPostMemoryInit

/**

  Platform hook to provide checkpoint output
  to the user interface. By default a byte is written to port 80h.

  @param[in]  MajorCode - Major Checkpoint code to write
  @param[in]  MinorCode - Minor Checkpoint code to write
  @param[in]  DebugPort - Debug Port to write
  @retval None

**/
VOID
PlatformCheckpoint (
  IN UINT8    MajorCode,
  IN UINT8    MinorCode,
  IN UINT16   DebugPort
  )
{

  return;

} // PlatformCheckpoint

/**

  Platform hook to provide common warning output to the user interface

  @param[in] MajorCode      - Major error/warning code
  @param[in] MinorCode      - Minor error/warning code
  @param[in] LogData        - Data log

  @retval None

**/
VOID
PlatformLogWarning (
  IN UINT8    MajorCode,
  IN UINT8    MinorCode,
  IN UINT32   LogData
  )
{

  return;

} // PlatformLogWarning

/**

  Platform hook to provide fatal error output to the user interface and to override
  the halt on error policy.

  @param majorCode - major error/warning code
  @param minorCode - minor error/warning code
  @param haltOnError - pointer to variable that selects policy

  @retval None

**/
VOID
PlatformFatalError (
  UINT8    majorCode,
  UINT8    minorCode,
  UINT8    *haltOnError
  )
{

  return;

} // PlatformFatalError

/**

  Used to add warnings to the promote warning exception list

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
PlatformMemInitWarning (
  IN PSYSHOST Host
  )
{

  return 0;

} // PlatformMemInitWarning

/**

  Intialize Enhanced Warning Log

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/

MRC_STATUS
PlatformEwlInit (
  IN PSYSHOST Host
  )
{

  return MRC_FUNCTION_NOT_IMPLEMENTED;

} // PlatformEwlInit

/**

  Add an entry to the enhanced warning log

  @param[in] EwlEntry - Pointer to the EWL entry buffer

  @retval SUCCESS

**/

MRC_STATUS
PlatformEwlLogEntry (
  IN EWL_ENTRY_HEADER  *EwlEntry
  )
{

  return MRC_FUNCTION_NOT_IMPLEMENTED;

} // PlatformEwlLogEntry

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

} // PlatformSetVdd

/**

  Platform hook to release any platform clamps affecting CKE
  and/or DDR Reset

  @param[in] Socket  - Processor number

  @retval None

**/
VOID
PlatformReleaseADRClamps (
  IN UINT8    Socket
  )
{

  return;

} // PlatformReleaseADRClamps

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

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval None

**/
VOID
PlatformCheckPORCompat (
  IN PSYSHOST Host
  )
{

  return;

} // PlatformCheckPORCompat


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

  return;

} // PlatformLookupFreqTable

/**

  Platform hook to Initialize Memory data

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval None

**/
VOID
PlatformInitializeMemoryData (
  IN PSYSHOST Host
  )
{

  return;

} // PlatformInitializeMemoryData

/**

  Platform hook before checking and triggering the proper type of reset.
  Executed by System BSP only.

  @param Host = pointer to sysHost structure

  @retval None

**/
VOID
PlatformPreHandleResetRequests (
  IN PSYSHOST             Host
  )
{

  return;

} // PlatformPreHandleResetRequests

/**

  Platform Hook for resetting the platform.

  @param[in] ResetType - Warm == 0x02, Cold == 0x04

  @retval MRC_STATUS

**/

MRC_STATUS
EFIAPI
PlatformIssueReset (
  IN UINT8 ResetType
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;

} // PlatformIssueReset

/**

  Puts the Host structure variables for POST

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval None

**/
VOID
PlatformPublishDataForPost (
  IN PSYSHOST Host
  )
{

  return;

} // PlatformPublishDataForPost

/**

  Platform hook for initializing Tcrit to 105 temp offset & the config register
  which is inside of initialization of memory throttling

  @param[in] Socket      - Socket number
  @param[in] Dev         - SMB Device
  @param[in] ByteOffset  - byte Offset
  @param[in] Data        - data

  @retval None

**/
VOID
PlatformHookMst (
  IN UINT8              Socket,
  IN SMB_DEVICE_STRUCT  Dev,
  IN UINT8              ByteOffset,
  IN UINT8              *Data
  )
{

  return;

} // PlatformHookMst

/**

  Platform hook meant to be used by customers where they can use it to write their own jumper detection code
  and have it return FALSE when no jumper is present, thereby, blocking calling of the BSSA loader; else have it
  return TRUE if the concerned jumper is physically present. This check ensures that someone will have to be
  physically present by the platform to enable the BSSA Loader.

  @param[in] None

  @retval TRUE - This function returns TRUE by default as OEMs are meant to implement their own jumper detection
                 code(DetectPhysicalPresence).

**/
BOOLEAN
PlatformDetectPhysicalPresenceSSA (
  VOID
  )
{
  return TRUE;
} // PlatformDetectPhysicalPresenceSSA

/**

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

  return 0;

} // PlatformUpdateConfig

/**

  Platform hook after address map is configured.

  @param[in] Host  - Pointer to sysHost

  @retval N/A

**/
VOID
PlatformPostAddressMapConfigured (
  IN PSYSHOST Host
  )
{

  return;

} // PlatformPostAddressMapConfigured

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

  return;

} // PlatformHandleSkuLimitViolation

/**

  OEM hook for reacting to Board VS CPU conflict.

  If this function returns MRC_FUNCTION_NOT_IMPLEMENTED,
  the reference code will halt execution. 

  @param[in] Host - pointer to sysHost

  @retval MRC_STATUS

**/

MRC_STATUS
EFIAPI
OemCheckForBoardVsCpuConflicts (
  IN PSYSHOST Host
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;

} // OemCheckForBoardVsCpuConflicts

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

  Platform hook for validating Current config (CCUR)

  @param[in] Host - Ptr to sysHost structure

  @retval SUCCESS

**/

UINT32
PlatformValidateConfig (
  PSYSHOST Host
  )
{

  return SUCCESS;

} // PlatformValidateConfig


/**
  Publish any Platform specfic HOBs.

  @param  None

  @retval EFI_SUCCESS

**/

EFI_STATUS
EFIAPI
PlatformPublishHobs (
  VOID
  )
{

  return EFI_SUCCESS;

} // PlatformPublishHobs

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
