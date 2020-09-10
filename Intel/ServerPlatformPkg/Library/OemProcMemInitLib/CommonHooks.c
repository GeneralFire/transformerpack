/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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

//!!! cleanup
#include <Library/MemoryCoreLib.h>
#include <Library/OemProcMemInitLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/IoAccessLib.h>
#include <Library/BeepLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/IoLib.h>
#include <Library/OemPlatformCheckpointLib.h>
#include <Library/OemProcMemInitHookLib.h>
// APTIOV_SERVER_OVERRIDE_RC_START
// Report MRC progress/error codes to status code listener
#include <Library/ReportStatusCodeLib.h>
#include <AmiMrcHooksWrapperLib/AmiMrcHooksWrapperLib.h>
#include <Library/PlatformStatusCodes.h>
// APTIOV_SERVER_OVERRIDE_RC_END

/**

  Platform hook to provide checkpoint output
  to the user interface. By default a byte is written to port 80h.

  @param[in]  MajorCode - Major Checkpoint code to write
  @param[in]  MinorCode - Minor Checkpoint code to write
  @param[in]  DebugPort - Debug Port to write

  @retval     Status

**/

MRC_STATUS
EFIAPI
PlatformCheckpoint (
  IN UINT8    MajorCode,
  IN UINT8    MinorCode,
  IN UINT16   DebugPort
  )
{
  //
  // If this function returns error, the caller
  // will perform a generic Checkpoint function.
  //

  return MRC_FUNCTION_NOT_IMPLEMENTED;
}


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
  // APTIOV_SERVER_OVERRIDE_RC_START
  AmiPlatformLogWarningWrapper(MajorCode,MinorCode,LogData);
  // APTIOV_SERVER_OVERRIDE_RC_END
  return;
}

/**

  PlatformFatalError - Platform hook to provide fatal error output to the
  user interface and to override the halt on error policy.

  @param[in] majorCode - major error/warning code
  @param[in] minorCode - minor error/warning code
  @param[in out] haltOnError - pointer to variable that selects policy

  @retval        Status

**/

MRC_STATUS
EFIAPI
PlatformFatalError (
  IN UINT8        majorCode,
  IN UINT8        minorCode,
  IN OUT UINT8    *haltOnError
  )
{
	// APTIOV_SERVER_OVERRIDE_RC_START
	  // Report MRC progress/error codes to status code listener
	  // Report Memory not detected Error code.
	  //
  if (( majorCode == ERR_NO_MEMORY ) || ( majorCode == WARN_NO_MEMORY )) {
		REPORT_STATUS_CODE (
		  EFI_ERROR_CODE|EFI_ERROR_MAJOR,
		  PEI_MEMORY_NOT_DETECTED
		  );
	  }
	  //
	  // Report Memory not useful Error code.
	  //
	  if ( majorCode == ERR_MEM_TEST ) {
		REPORT_STATUS_CODE (
		  EFI_ERROR_CODE|EFI_ERROR_MAJOR,
		  PEI_MEMORY_NONE_USEFUL
		  );
	  }
    AmiPlatformFatalErrorWrapper(majorCode, minorCode, haltOnError );
    // APTIOV_SERVER_OVERRIDE_RC_END

  return PlatformFatalErrorHook (majorCode, minorCode, haltOnError);

} // PlatformFatalError