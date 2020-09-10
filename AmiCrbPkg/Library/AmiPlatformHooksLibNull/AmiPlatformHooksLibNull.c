//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file
    Contains the function definitions for Platform Hooks functions.
*/
#include <PiPei.h>
#include <Library/CpuConfigLib.h>
#include <AmiPlatformHooksLib.h>

/**
  OemUpdatePlatformInfoHob - OEM hook to update Platform info HOB

  @param PeiServices        - pointer to PEI Services Table
  @param PlatformInfoHob    - pointer to Platform info HOB

  @retval Status
**/
EFI_STATUS
OemUpdatePlatformInfoHob (
   IN CONST EFI_PEI_SERVICES     **PeiServices,
   OUT VOID                     *PlatformInfoHob
)
{
  // Add OEM Specific Porting here
  return EFI_SUCCESS;
}

/**
  OemPchPolicyOverride - OEM wrapper to override the PchPolicy

  @param PchPolicy             - pointer to PchPolicy
  @param SetupVariables        - pointer to SetupVariables
  @param PchRcVariables        - pointer to PchRcVariables
  
  @retval VOID

**/
VOID
OemPchPolicyOverride(
   IN OUT VOID        *PchPolicy,
   IN VOID            *SetupVariables,
   IN VOID            *PchRcVariables
)
{
    return;
}

/**
  OemPchSoftStrapFixupOverride - OEM wrapper to override the PchSoftStrap

  @param StrapRegion      - pointer to StrapRegion
  @param SystemBoard      - pointer to SystemBoardPpi
  
  @retval VOID
**/
VOID
OemPchSoftStrapFixupOverride(
   IN OUT UINT8                 *StrapRegion,
   IN VOID                      *SystemBoard
) 
{
    return;
}

/**
  OemInstallXhciAcpiTableOverride - OEM wrapper to override the XhciAcpiTable

  @param PlatformInfo      - pointer to PlatformInfo
  @param XhciAcpiTable     - pointer to XhciAcpiTable
  
  @retval VOID
**/
VOID
OemInstallXhciAcpiTableOverride(
   IN VOID                      *PlatformInfo,
   IN OUT UINT64                *XhciAcpiTable
) 
{
    return;
}

/**
  OemOverrideCpuConfigContextBuffer - OEM wrapper to override the CpuConfigContextBuffer

  @param CpuConfigContextBuffer      - pointer to CpuConfigContextBuffer
  
  @retval VOID
**/
VOID
OemOverrideCpuConfigContextBuffer (
   IN OUT CPU_CONFIG_CONTEXT_BUFFER *CpuConfigContextBuffer
) 
{
    return;
}
