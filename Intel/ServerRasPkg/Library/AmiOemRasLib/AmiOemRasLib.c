//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************



#include <Library/AmiOemRasLib.h>


/**
  OemMcaErrEnableDisableReporting - OEM hook to Enable/Disable MCA Error Reporting

  This function provides a NULL method that can be overridden to set/override OEM error reporting parameters

  @retval Status.

**/
EFI_STATUS
OemMcaErrEnableDisableReporting (
  )
{
    // Add OEM code to Enable/Disable MCA Error Reporting
    return EFI_SUCCESS;
}

/**
  OemImcErrEnableDisableReporting - OEM hook to Enable/Disable IMC Error Reporting

  This function provides a NULL method that can be overridden to set/override OEM error reporting parameters
  @param[in] *PolicyData         A pointer to Ras Policy data
  @param[in] *MemTopology        A pointer to Memory Topology strcuture

  @retval Status.

**/
EFI_STATUS
OemImcErrEnableDisableReporting (
  IN      RAS_POLICY               *PolicyData,
  IN      MEM_TOPOLOGY             *MemTopology
  )
{
    // Add OEM code to Enable/Disable IMC Error Reporting
    return EFI_SUCCESS;
}

/**
  OemDdrtErrEnableDisableReporting - OEM hook to Enable/Disable DDRT Error Reporting

  This function provides a NULL method that can be overridden to set/override OEM error reporting parameters
  @param[in] *PolicyData            A pointer to Ras Policy data
  @param[in] *DdrtErrorsEnabled     A pointer to the DdrtErrorsEnabled flag

  @retval Status.

**/
EFI_STATUS
OemDdrtErrEnableDisableReporting (
  IN    RAS_POLICY          *PolicyData,
  IN    BOOLEAN             *DdrtErrorsEnabled
  )
{
    // Add OEM code to Enable/Disable DDRT Error Reporting
    return EFI_SUCCESS;
}

/**
  OemIehErrEnableDisableReporting - OEM hook to Enable/Disable IEH Error Reporting

  This function provides a NULL method that can be overridden to set/override OEM error reporting parameters

  @retval Status.

**/
EFI_STATUS
OemIehErrEnableDisableReporting (
  )
{
    // Add OEM code to Enable/Disable IEH Error Reporting
    return EFI_SUCCESS;
}

/**
  OemOverrideRasPolicyData - OEM hook to Override Global RAS Policy Data structure

  This function provides a NULL method that can be overridden to set/override OEM error reporting parameters
  @param[in] *PolicyData            A pointer to Ras Policy data
  @param[in] SetupData              Setup Data structure

  @retval Status.

**/
EFI_STATUS
OemOverrideRasPolicyData (
  IN      RAS_POLICY               *PolicyData,
  IN      INTEL_SETUP_DATA         *SetupData
  )
{
    // Add OEM code to override RAS Policy Data
    return EFI_SUCCESS;
}

/**
  OemReadyToTakeAction - OEM hook for modifying runtime handler execution.

  This function provides mechanism for following : 1. Decide action based on event ErrorEvent 
                                                   2. Decide to enter or exit handler code by clearing/setting SkipEvent variable
  @param[in] ErrorEvent            Error Event Information
  @param[in] *FailedDimmInfo       A pointer to Failed DIMM Info data
  @param[out] SkipEvent            Boolean variable holding handler skip or execute status.

  @retval Status.

**/
EFI_STATUS
OemReadyToTakeAction (
   IN   UINT8               ErrorEvent,
   IN   VOID                *FailedDimmInfo,
   OUT  BOOLEAN             *SkipEvent
  )
{
    // Add OEM code to Take Action in case of Errors
    return EFI_SUCCESS;
}

/**
  OemCollectErrors - OEM Hook to collect more information about the errors

  This function provides mechanism for collecting extra information on errors logged 
  @param[in] *ErrorDataStructure      A pointer to Error Data structure

  @retval Status.

**/
EFI_STATUS
OemCollectErrors (
    VOID *ErrorDataStructure
  )
{
    // Add OEM code to Collect Error Data
    return EFI_SUCCESS;
}

/**
  OemReportStatusCode - OEM Hook to report error status codes

  This function provides mechanism for reporting error status codes .
  @param[in] FailedDimmInfo     Failed DIMM Info structure
  @param[in] EventName          Event name

  @retval Status.

**/
EFI_STATUS
OemReportStatusCode (
  IN        FAILED_DIMM_INFO    FailedDimmInfo,
  IN        EVENT_NAME          EventName
  )
{
  
    // Add OEM code to report status code
    return EFI_SUCCESS;
}
