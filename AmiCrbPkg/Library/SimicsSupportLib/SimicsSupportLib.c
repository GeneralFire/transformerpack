//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file
    Contains the functions to be implemented based on Simics platform Support.
    Functionality that needs to be supressed based on Simics support can be implemented here.
*/

#include <Token.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/DebugLib.h>
#include <Include/AmiServerMgmtSetupVariable.h>
#include <AmiDxeLib.h>




//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Name: DxeIsSmicsIpmiConstructor
//
// Description: 

// Input: Image Handle
//        System Tabele
// Output:
//     EFI_STATUS
//
// Notes:       None.
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

/**

  Constructor that Disables IPMI Support Setup variable if running in Simics environment

  @param[in] Image Handle   -   Image Handle
  @param[in] System Tabele  -   Pointer to System Table

  @retval MRC_STATUS

**/
EFI_STATUS
EFIAPI
DxeIsSmicsIpmiConstructor (
        IN EFI_HANDLE ImageHandle,
        IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  UINTN                                 Size;
  SERVER_MGMT_CONFIGURATION_DATA        gServerMgmtConfiguration;
  UINT32                                gAttributes;
  EFI_STATUS                            Status;

  if (IsSimicsEnvironment()) {

      InitAmiLib(ImageHandle,SystemTable);
      Size =  sizeof (SERVER_MGMT_CONFIGURATION_DATA);
      Status =  pRS->GetVariable (
                              L"ServerSetup",
                              &gEfiServerMgmtSetupVariableGuid,
                              &gAttributes,
                              &Size,
                              &gServerMgmtConfiguration );
     if (!EFI_ERROR(Status)) {
         gServerMgmtConfiguration.BmcSupport = FALSE;
     
         pRS->SetVariable (
                                   L"ServerSetup",
                                   &gEfiServerMgmtSetupVariableGuid,
                                   gAttributes,
                                   Size,
                                   &gServerMgmtConfiguration );
         }
             
  }
  return EFI_SUCCESS;
}

