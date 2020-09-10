/** @file
  This file defines MRC_PLATFORM_HOOKS_INTERFACE, which contians platform functions
  produced by OEM platform code to be called at proper points by MRC code.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#ifndef _MRC_PLATFORM_HOOKS_GUID_H_
#define _MRC_PLATFORM_HOOKS_GUID_H_

#include <ReferenceCodeDataTypes.h>
#include <MemCommon.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Memory/MemPorDefinitions.h>

///
/// MRC_PLATFORM_HOOKS_INTERFACE revision information.
/// This Interface will be extended in a backwards compatible manner over time.
/// Added interfaces should be documented here with the revisions added.
/// Revision 1:  Initial revision.
/// Revision 2:  Update PlatformInitializeData function with Socket parameter.
//               This function is executed in loop for each socket.
/// Revision 3:  Update parameter list with new MemHooksContext replacing
//               SysHost for some hooks.
/// Revision 4:  Added function PlatformHandleSkuLimitViolation.
///
///
/// Revision 5:  Remove incomplete implementation of MemHooksContext.
///
/// Revision 6:  Remove hooks that were never called from the Reference Code.
///
/// Revision 7:  Add PlatformPromoteWarningsToErrors.

#define MRC_PLATFORM_HOOKS_INTERFACE_REVISION  0x7

///
/// Forward declaration for the MRC_PLATFORM_HOOKS_INTERFACE.
///
typedef struct _MRC_PLATFORM_HOOKS_INTERFACE MRC_PLATFORM_HOOKS_INTERFACE;

/**
  Platform hook to initialize memory input data before memory initialization,
  such as MEMORY_POLICY_PPI.

  @param[in]    Host    Pointer to SYSHOST structure.

  @retval       N/A

**/
typedef
VOID
(EFIAPI *PLATFORM_INITIALIZE_MEMORY_DATA) (
  IN PSYSHOST Host
  );

/**
  Platform hook before memory initialization.

  @param[in] Host  - Pointer to sysHost

  @retval       EFI_SUCCESS        Note: Current MRC does not have clear requirement.

**/
typedef
EFI_STATUS
(EFIAPI *PLATFORM_PRE_MEMORY_INIT) (
  IN PSYSHOST Host
  );

/**
  Platform hook after memory initialization.

  @param[in] Host  - Pointer to sysHost

  @retval    EFI_SUCCESS      Note: Current MRC does not have clear requirement
                              to the return values.

**/
typedef
EFI_STATUS
(EFIAPI *PLATFORM_POST_MEMORY_INIT) (
  IN PSYSHOST Host
  );

/**

  OEM hook for reacting to Board VS CPU conflict.

  If this function returns MRC_FUNCTION_NOT_IMPLEMENTED,
  the reference code will halt execution. 

  @param[in] Host - pointer to sysHost

  @retval MRC_STATUS

**/

typedef
MRC_STATUS
(EFIAPI *OEM_CHECK_FOR_BOARD_VS_CPU_CONFLICTS) (
  IN PSYSHOST Host
  );

/**

  Platform hook before MRC checks and triggers a proper type of reset.
  This function is executed by System BSP only.

  @param[in] None

  @retval None

**/
typedef
VOID
(EFIAPI *PLATFORM_PRE_HANDLE_RESET_REQUESTS) (
  PSYSHOST Host
  );

/**

  Platform hook at the point that MRC print memory initialization result.

  @param Host - Pointer to SYSHOST structure.

  @retval None

**/
typedef
VOID
(EFIAPI *PLATFORM_PUBLISH_DATA_FOR_POST) (
  PSYSHOST Host
  );

/**

  Platform hook meant to be used by customers where they can use it to write their own jumper detection code
  and have it return FALSE when no jumper is present, thereby, blocking calling of the BSSA loader; else have it
  return TRUE if the concerned jumper is physically present. This check ensures that someone will have to be
  physically present by the platform to enable the BSSA Loader.

  @param[in] None

  @retval TRUE - This function returns TRUE by default as OEMs are meant to implement their own jumper detection
                 code (DetectPhysicalPresence).

**/
typedef
BOOLEAN
(EFIAPI *PLATFORM_DETECT_PHYSICAL_PRESENCE_SSA) (
  VOID
  );

/**

  Platform hook to provide checkpoint output
  to the user interface. By default a byte is written to port 80h.

  @param[in]  MajorCode - Major Checkpoint code to write.
  @param[in]  MinorCode - Minor Checkpoint code to write.
  @param[in]  DebugPort - Debug Port to write.

  @retval     Status

**/

typedef
MRC_STATUS
(EFIAPI *PLATFORM_CHECK_POINT) (
  IN UINT8    MajorCode,
  IN UINT8    MinorCode,
  IN UINT16   DebugPort
  );

/**

  Platform hook to provide common warning output to the
  user interface.

  @param MajorCode - Major error/warning code.
  @param MinorCode - Minor error/warning code.
  @param LogData   - Data log.

**/
typedef
VOID
(EFIAPI *PLATFORM_LOG_WARNING) (
  UINT8    MajorCode,
  UINT8    MinorCode,
  UINT32   LogData
  );

/**

  Platform hook to provide fatal error output to the
  user interface and to override the halt on error policy.

  @param[in] MajorCode - Major error/warning code.
  @param[in] MinorCode - Minor error/warning code.
  @param[in, out] HaltOnError - Pointer to variable that selects policy.

  @retval         Status

**/
typedef
MRC_STATUS
(EFIAPI *PLATFORM_FATAL_ERROR) (
  IN UINT8    MajorCode,
  IN UINT8    MinorCode,
  IN OUT UINT8    *HaltOnError
  );

/**

  Used to add warnings to the promote warning exception list.

  @param Host  - Pointer to SYSHOST structure

  @retval      - N/A

**/
typedef
UINT32
(EFIAPI *PLATFORM_MEM_INIT_WARNING) (
  PSYSHOST Host
  );

/**

  Used for memory related warnings.

  @param Host      - Pointer to SYSHOST structure.
  @param MajorCode - Major warning code.
  @param MinorCode - Minor warning code.
  @param LogData   - Data log.

  @retval          - N/A

**/
typedef
VOID
(EFIAPI *PLATFORM_MEM_LOG_WARNING) (
  PSYSHOST Host,
  UINT8    MajorCode,
  UINT8    MinorCode,
  UINT32   LogData
  );

/**

  Platform hook to change the socket Vddq.

  @param[in]      Socket      - Processor number.
  @param[in, out] DdrVoltage  - Points to DdrVoltage Data Buf.

  @retval SUCCESS if the Vddq change was made.
  @retval FAILURE if the Vddq change was not made.

**/
typedef
MRC_STATUS
(EFIAPI *PLATFORM_SET_VDD) (
  IN UINT8        Socket,
  IN OUT UINT8    *DdrVoltage
  );

/**

  Platform hook to release any platform clamps affecting CKE
  and/or DDR Reset.

  @param Socket    - Processor number.

  @retval None

**/
typedef
VOID
(EFIAPI *PLATFORM_RELEASE_ADR_CLAMPS) (
  UINT8    Socket
  );

/**

  Platform hook to allow a secondary ADR dection method other than PCH_PM_STS

  @param[out]   - Ptr to update with the Event

  @retval MRC_STATUS

**/

typedef
MRC_STATUS
(EFIAPI *PLATFORM_DETECT_ADR) (
  OUT UINT32 *AdrEvent
  );

/**

  This routine can be used to update CCUR.

  @param Host - pointer to SYSHOST structure.

  @retval SUCCESS

**/
typedef
UINT32
(EFIAPI *PLATFORM_UPDATE_CONFIG) (
  PSYSHOST Host
  );

/**

  Platform hook to restrict supported configurations if required.

  @param Host - Pointer to SYSHOST structure.

  @retval     - N/A

**/
typedef
VOID
(EFIAPI *PLATFORM_CHECK_POR_COMPAT) (
  PSYSHOST Host
  );

/**

  Platform hook for initializing Tcrit to 105 temp offset & the config register
  which is inside of initialization of memory throttling.

  @param[in] Socket      - Socket number.
  @param[in] Dev         - SMB Device.
  @param[in] ByteOffset  - Byte Offset.
  @param[in] Data        - Pointer to Data.

  @retval None

**/
typedef
VOID
(EFIAPI *PLATFORM_HOOK_MST) (
  UINT8             Socket,
  SMB_DEVICE_STRUCT Dev,
  UINT8             ByteOffset,
  UINT8             *Data
  );

/**

  Platform hook for overriding the memory POR frequency table.

  @param[out] FreqTablePtr     - Pointer to Intel POR memory frequency table.
  @param[out] FreqTableLength  - Pointer to number of DimmPOREntryStruct entries in POR Frequency Table.

  @retval None

**/
typedef
VOID
(EFIAPI *PLATFORM_LOOKUP_FREQ_TABLE) (
  OUT struct DimmPOREntryStruct   **FreqTablePtr,
  OUT UINT16                      *FreqTableLength
  );

/**

  Platform hook for OEM to pass VR SVID of memory controller to MRC.

  @param[in] Socket              - Socket number.
  @param[in] McId                - Memory Controller Id.
  @param[out] VrSvid             - VR SVID.

  @retval - VR SVID for given Socket and McId.
**/

typedef
MRC_STATUS
(EFIAPI *PLATFORM_VRS_SVID) (
  IN UINT8      Socket,
  IN UINT8      McId,
  OUT UINT8     *VrSvid
  );

/**

  Check if CPU part was swapped.

  @param Host - Pointer to SYSHOST structure.

  @retval TRUE - CPU has been swapped.
  @retval FALSE - CPU has not been swapped.

**/
typedef
BOOLEAN
(EFIAPI *PLATFORM_CHECK_CPU_PARTS_CHANGE_SWAP) (
  PSYSHOST Host
  );

/**

  Platform Hook for resetting the platform.
  Executed by System BSP only.

  @param[in] ResetType - Warm == 0x02, Cold == 0x04.

  @retval MRC_STATUS

**/
typedef
MRC_STATUS
(EFIAPI *PLATFORM_ISSUE_RESET) (
  IN UINT8 ResetType
  );

/**

  Platform Hook to Log warning to the warning log.

  @param[in] WarningCode       - Major warning code.
  @param[in] MinorWarningCode  - Minor warning code.
  @param[in] Socket            - Socket number.
  @param[in] Channel           - Channel number.
  @param[in] Dimm              - Dimm number.
  @param[in] Rank              - Rank number.

  @retval None

**/
typedef
VOID
(EFIAPI *PLATFORM_OUTPUT_WARNING) (
  UINT8    WarningCode,
  UINT8    MinorWarningCode,
  UINT8    Socket,
  UINT8    Channel,
  UINT8    Dimm,
  UINT8    Rank
  );

/**

  Platform hook to Log Error to the warning log.

  @param ErrorCode         - Major warning code.
  @param MinorErrorCode    - Minor warning code.
  @param Socket            - Socket number.
  @param Channel           - Channel number.
  @param Dimm              - Dimm number.
  @param Rank              - Rank number.

  @retval None

**/
typedef
VOID
(EFIAPI *PLATFORM_OUTPUT_ERROR) (
  UINT8    ErrorCode,
  UINT8    MinorErrorCode,
  UINT8    Socket,
  UINT8    Channel,
  UINT8    Dimm,
  UINT8    Rank
  );

/**

  Platform hook for initializing the enhanced warning log.
  It is called from core prior any calls to PlatformEwlLogEntry.
  This function will initialize the enhanced warning log public
  and private data structures.

  @param Host        - Pointer to the system Host (root) structure.

  @retval            - SUCCESS

**/
typedef
MRC_STATUS
(EFIAPI *PLATFORM_EWL_INIT) (
  PSYSHOST Host
  );

/**

  Platform hook that is called to report a new entry is available for reporting to
  the Enhanced Warning Log.

  This function will add it to the log or return an error if there is insufficient
  space remaining for the entry.

  @param  Host        - Pointer to the system Host (root) structure.
  @param  EwlEntry    - Pointer to new EWL entry buffer.

  @retval             - SUCCESS if entry is added; FAILURE if entry is not added.

**/
typedef
MRC_STATUS
(EFIAPI *PLATFORM_EWL_LOG_ENTRY) (
  EWL_ENTRY_HEADER  *EwlEntry
  );

/**

  Platform hook for validating Current config (CCUR).

  @param Host         - Pointer to SYSHOST structure on stack.

  @retval             - SUCCESS

**/
typedef
UINT32
(EFIAPI *PLATFORM_VALIDATE_CONFIG) (
  PSYSHOST Host
  );

/**

  Platform hook after address map is configured.

  @param Host  - Pointer to sysHost.

  @retval N/A

**/
typedef
VOID
(EFIAPI *PLATFORM_POST_ADDRESS_MAP_CONFIGURED) (
  PSYSHOST Host
  );

/**

  Platform hook to handle SKU Limit violations.

  @param[in] Host  - Pointer to sysHost.

  @retval N/A

**/
typedef
VOID
(EFIAPI *PLATFORM_HANDLE_SKU_LIMIT_VIOLATIONS) (
  SYSHOST *Host
  );

/**
  Publish any Platform specfic HOBs.

  @param  None

  @retval EFI_SUCCESS

**/

typedef
EFI_STATUS
(EFIAPI *PLATFORM_PUBLISH_HOBS) (
  VOID
  );


/**

  Platform hook called to see if the warning should be promoted to an exception

  @param[in]  Host        - Pointer to the system Host (root) structure
  @param[in]  EwlEntry    - Pointer to new EWL entry buffer

  @retval None

**/

typedef
MRC_STATUS
(EFIAPI *PLATFORM_PROMOTE_WARNINGS_TO_ERRORS) (
  IN PSYSHOST          Host,
  IN EWL_ENTRY_HEADER  *EwlEntry
  );

struct _MRC_PLATFORM_HOOKS_INTERFACE {
  UINT32                                  Revision;
  PLATFORM_INITIALIZE_MEMORY_DATA         PlatformInitializeMemoryData;
  PLATFORM_PRE_MEMORY_INIT                PlatformPreMemoryInit;
  PLATFORM_POST_MEMORY_INIT               PlatformPostMemoryInit;
  OEM_CHECK_FOR_BOARD_VS_CPU_CONFLICTS    OemCheckForBoardVsCpuConflicts;
  PLATFORM_PRE_HANDLE_RESET_REQUESTS      PlatformPreHandleResetRequests;
  PLATFORM_PUBLISH_DATA_FOR_POST          PlatformPublishDataForPost;
  PLATFORM_DETECT_PHYSICAL_PRESENCE_SSA   PlatformDetectPhysicalPresenceSSA;
  PLATFORM_SET_VDD                        PlatformSetVdd;
  PLATFORM_RELEASE_ADR_CLAMPS             PlatformReleaseADRClamps;
  PLATFORM_DETECT_ADR                     PlatformDetectADR;
  PLATFORM_UPDATE_CONFIG                  PlatformUpdateConfig;
  PLATFORM_CHECK_POR_COMPAT               PlatformCheckPORCompat;
  PLATFORM_HOOK_MST                       PlatformHookMst;
  PLATFORM_LOOKUP_FREQ_TABLE              PlatformLookupFreqTable;
  PLATFORM_VRS_SVID                       PlatformVRsSVID;
  PLATFORM_ISSUE_RESET                    PlatformIssueReset;
  PLATFORM_CHECK_POINT                    PlatformCheckpoint;
  PLATFORM_OUTPUT_WARNING                 PlatformOutputWarning;
  PLATFORM_OUTPUT_ERROR                   PlatformOutputError;
  PLATFORM_LOG_WARNING                    PlatformLogWarning;
  PLATFORM_FATAL_ERROR                    PlatformFatalError;
  PLATFORM_EWL_INIT                       PlatformEwlInit;
  PLATFORM_MEM_INIT_WARNING               PlatformMemInitWarning;
  PLATFORM_EWL_LOG_ENTRY                  PlatformEwlLogEntry;
  PLATFORM_VALIDATE_CONFIG                PlatformValidateConfig;
  PLATFORM_POST_ADDRESS_MAP_CONFIGURED    PlatformPostAddressMapConfigured;
  PLATFORM_HANDLE_SKU_LIMIT_VIOLATIONS    PlatformHandleSkuLimitViolation;
  PLATFORM_PUBLISH_HOBS                   PlatformPublishHobs;
  PLATFORM_PROMOTE_WARNINGS_TO_ERRORS     PlatformPromoteWarningsToErrors;  //!!!! ADD TO PEIM
};

extern EFI_GUID gMrcPlatformHooksInterfaceGuid;

#endif // #ifndef _MRC_PLATFORM_HOOKS_GUID_H_
