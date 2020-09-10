//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


#ifndef _AMI_OEM_RAS_LIB_H_
#define _AMI_OEM_RAS_LIB_H_

#include <Token.h>
#include <Base.h>
#include <Uefi/UefiBaseType.h>

#include <Library/BaseLib.h>

#include <Library/MemFuncLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Protocol/RasPolicy.h>
#include <SetupTable.h>

//
// RAS Error Event Definitions
//
#define ERROR_EVENT_PCLS            0
#define ERROR_EVENT_ADDDC           1
#define ERROR_EVENT_RANK_SPARING    2
#define ERROR_EVENT_SDDC            3
#define ERROR_EVENT_MIRR_FAILOVER   4
#define DEF_HANDLER                 5 
#define SKIP_CE_HANDLING            6

//
// RAS Event Names
//
typedef enum {
  SPARE_COPY_START,
  SPARE_COPY_COMPLETE,
  SDDC_START,
  SDDC_COMPLETE,
  ADDDC_START,
  ADDDC_COMPLETE,
  PCLS_EVENT
} EVENT_NAME;

///
/// Function Prototypes
///

EFI_STATUS
OemMcaErrEnableDisableReporting (
  );

EFI_STATUS
OemImcErrEnableDisableReporting (
  IN     RAS_POLICY         *PolicyData,
  IN     MEM_TOPOLOGY       *MemTopology
  );

EFI_STATUS
OemDdrtErrEnableDisableReporting (
  IN    RAS_POLICY          *PolicyData,
  IN    BOOLEAN             *DdrtErrorsEnabled
  );

EFI_STATUS
OemIehErrEnableDisableReporting (
  );

EFI_STATUS
OemOverrideRasPolicyData (
  IN    RAS_POLICY          *PolicyData,
  IN    INTEL_SETUP_DATA    *SetupData
  );

EFI_STATUS
OemReadyToTakeAction (
  IN    UINT8               ErrorEvent,
  IN    VOID                *FailedDimmInfo,
  OUT   BOOLEAN             *SkipEvent
  );

EFI_STATUS
OemCollectErrors (
  IN    VOID                *ErrorDataStructure
  );

EFI_STATUS
OemReportStatusCode (
  IN    FAILED_DIMM_INFO    FailedDimmInfo,
  IN    EVENT_NAME          EventName
  );

#endif
