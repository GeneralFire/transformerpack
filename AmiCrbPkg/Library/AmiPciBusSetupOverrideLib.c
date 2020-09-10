//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file
    Contains the functions to override the PCI related setup options so
    that it won't be modified or their registers were not programmed
    again.
*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SetupLib.h>
#include <Protocol/AmiBoardInfo2.h>
#include <PciBus.h>
#include <PciSetup.h>
#include <AmiPciBusSetupOverrideLib.h>

UINT8                       gARIEnable;

/**
    Sets PCI_SETUP_DONT_TOUCH for all the common PCI Dynamic Setup options

    @param       PciDevInfo             - Pointer to PCI Device info structure
    @param       PciDev                 - Pointer to the General PCI related setup data buffer
    @param       DevSdlInfo             - Pointer to the PCI device information
    @param       DevIdx                 - Index of the PCI device
    @param       Set                    - TRUE or FALSE

    @return     EFI_STATUS
    @retval     Status            EFI_SUCCESS         - Success will be returned.
**/
EFI_STATUS 
AmiPciGetPciDevSetupDataOverrideHook (
  IN PCI_DEV_INFO               *PciDevInfo,
  IN PCI_DEVICE_SETUP_DATA      *PciDev,
  IN AMI_SDL_PCI_DEV_INFO       *DevSdlInfo,  //OPTIONAL if == NULL get defaults...
  IN UINTN                      DevIdx,
  IN BOOLEAN                    Set
)
{
    if (PciDev != NULL) {
        PciDev->PciLatency = PCI_SETUP_DONT_TOUCH;
        PciDev->PciXLatency = PCI_SETUP_DONT_TOUCH;
        PciDev->VgaPallete = PCI_SETUP_DONT_TOUCH;
        PciDev->PerrEnable = PCI_SETUP_DONT_TOUCH;
        PciDev->SerrEnable = PCI_SETUP_DONT_TOUCH;
    }
    return EFI_SUCCESS;
}

/**
    Sets PCI_SETUP_DONT_TOUCH for all the common PCI Dynamic Setup options

    @param       PciDevInfo             - Pointer to PCI Device info structure
    @param       PciCommon              - Pointer to the PCI Bus common setup data buffer

    @return     EFI_STATUS
    @retval     Status            EFI_SUCCESS         - Success will be returned.
**/
EFI_STATUS
AmiPciGetCommonSetupDataOverrideHook (
  IN PCI_DEV_INFO               *PciDevInfo,
  IN PCI_COMMON_SETUP_DATA      *PciCommon
)
{
    return EFI_SUCCESS;
}

/**
    Sets PCI_SETUP_DONT_TOUCH for all the GEN 1 PCI Dynamic Setup options

    @param       PciDevInfo             - Pointer to PCI Device info structure
    @param       Pcie1                  - Pointer to the GEN 1 Pci related setup data buffer
    @param       DevSdlInfo             - Pointer to the PCI device information
    @param       DevIdx                 - Index of the PCI device
    @param       Set                    - TRUE or FALSE

    @return     EFI_STATUS
    @retval     Status            EFI_SUCCESS         - Success will be returned.
**/
EFI_STATUS
AmiPciGetPcie1SetupDataOverrideHook (
  IN PCI_DEV_INFO               *PciDevInfo,
  IN PCIE1_SETUP_DATA           *Pcie1,
  IN AMI_SDL_PCI_DEV_INFO       *DevSdlInfo,  //OPTIONAL if == NULL get defaults...
  IN UINTN                      DevIdx,      //OPTIONAL if == 0 
  IN BOOLEAN                    Set
)
{
    if (Pcie1 != NULL){
        Pcie1->RelaxedOrdering = PCI_SETUP_DONT_TOUCH;
        Pcie1->ExtTagField = PCI_SETUP_DONT_TOUCH;
        Pcie1->NoSnoop = PCI_SETUP_DONT_TOUCH;

        if ( (PciDevInfo !=NULL) && (PciDevInfo->ParentBrg!=NULL) &&
             (PciDevInfo->ParentBrg->Type!=tPciRootBrg) && (PciDevInfo->ParentBrg->PciExpress!=NULL) &&
             (PciDevInfo->ParentBrg->PciExpress->Pcie1Setup.MaxPayload!=PCI_SETUP_DONT_TOUCH)) {
            Pcie1->MaxPayload = PciDevInfo->ParentBrg->PciExpress->Pcie1Setup.MaxPayload;
        } else {
            Pcie1->MaxPayload = PCI_SETUP_DONT_TOUCH;
        }

        Pcie1->MaxReadRequest = PCI_SETUP_DONT_TOUCH;
        Pcie1->AspmMode = PCI_SETUP_DONT_TOUCH;
        Pcie1->ExtendedSynch = PCI_SETUP_DONT_TOUCH;
        Pcie1->ClockPm = PCI_SETUP_DONT_TOUCH;
        Pcie1->LnkTrRetry = PCI_SETUP_DONT_TOUCH;
        Pcie1->LnkTrTimeout = PCI_SETUP_DONT_TOUCH;
        Pcie1->LnkDisable = PCI_SETUP_DONT_TOUCH;
        Pcie1->IgnoreCommonClock = PCI_SETUP_DONT_TOUCH;
        
        Pcie1->DevCtrDontTouch = PCI_SETUP_DONT_TOUCH;
        Pcie1->LnkCtrDontTouch = PCI_SETUP_DONT_TOUCH;
        Pcie1->SltCtrDontTouch = PCI_SETUP_DONT_TOUCH;
    }
    return EFI_SUCCESS;
}

/**
    Sets PCI_SETUP_DONT_TOUCH for all the GEN 2 PCI Dynamic Setup options

    @param       PciDevInfo             - Pointer to PCI Device info structure
    @param       Pcie2                  - Pointer to the GEN 2 Pci related setup data buffer
    @param       DevSdlInfo             - Pointer to the PCI device information
    @param       DevIdx                 - Index of the PCI device
    @param       Set                    - TRUE or FALSE

    @return     EFI_STATUS
    @retval     Status            EFI_SUCCESS         - Success will be returned.
**/
EFI_STATUS
AmiPciGetPcie2SetupDataOverrideHook (
  IN PCI_DEV_INFO               *PciDevInfo,
  IN PCIE2_SETUP_DATA           *Pcie2,
  IN AMI_SDL_PCI_DEV_INFO       *DevSdlInfo,  //OPTIONAL if == NULL get defaults...
  IN UINTN                      DevIdx,      //OPTIONAL if == 0 
  IN BOOLEAN                    Set
)
{
    if (Pcie2 != NULL) {
        Pcie2->ComplTimeOut = PCI_SETUP_DONT_TOUCH;
        Pcie2->AriFwd = gARIEnable;
        Pcie2->AtomOpReq = PCI_SETUP_DONT_TOUCH;
        Pcie2->AtomOpEgressBlk = PCI_SETUP_DONT_TOUCH;
        Pcie2->IDOReq = PCI_SETUP_DONT_TOUCH;
        Pcie2->IDOCompl = PCI_SETUP_DONT_TOUCH;
        Pcie2->LtrReport = PCI_SETUP_DONT_TOUCH;
        Pcie2->E2ETlpPrBlk = PCI_SETUP_DONT_TOUCH;
        Pcie2->LnkSpeed = PCI_SETUP_DONT_TOUCH;
        Pcie2->ComplSos = PCI_SETUP_DONT_TOUCH;
        
        Pcie2->HwAutoWidth = PCI_SETUP_DONT_TOUCH;
        Pcie2->HwAutoSpeed = PCI_SETUP_DONT_TOUCH;
        
        Pcie2->DevCtr2DontTouch = PCI_SETUP_DONT_TOUCH;
        Pcie2->LnkCtr2DontTouch = PCI_SETUP_DONT_TOUCH;
    }
    return EFI_SUCCESS;
}

#if (PCI_SETUP_USE_APTIO_4_STYLE == 1)
/**
     Sets PCI_SETUP_DONT_TOUCH for all the PCI Static Setup options

    @param       PciDevInfo             - Pointer to PCI Device info structure
    @param       PciSetupData           - Pointer to the Pci Bus specific setup data buffer
    @param       PciCommon              - Pointer to the Pci Bus common setup data buffer
    @param       HpSetup                - Pointer to the Pci Bus Hot plug related setup data buffer

    @return     EFI_STATUS
    @retval     Status            EFI_SUCCESS         - Success will be returned.
**/
EFI_STATUS
AmiPciGetSetupDataOverrideHook (
  IN PCI_DEV_INFO               *PciDevInfo,
  IN PCI_SETUP_DATA             *PciSetupData,
  IN PCI_COMMON_SETUP_DATA      *PciCommon,
  IN PCI_HOTPLUG_SETUP_DATA     *HpSetup
)
{
    if (PciSetupData!=NULL) {
        PciSetupData->PciDevSettings.PciLatency = PCI_SETUP_DONT_TOUCH;
        PciSetupData->PciDevSettings.PciXLatency = PCI_SETUP_DONT_TOUCH;
        PciSetupData->PciDevSettings.VgaPallete = PCI_SETUP_DONT_TOUCH;
        PciSetupData->PciDevSettings.PerrEnable = PCI_SETUP_DONT_TOUCH;
        PciSetupData->PciDevSettings.SerrEnable = PCI_SETUP_DONT_TOUCH;
        
        PciSetupData->Pcie1Settings.RelaxedOrdering = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie1Settings.ExtTagField = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie1Settings.NoSnoop = PCI_SETUP_DONT_TOUCH;

        if ( (PciDevInfo !=NULL) && (PciDevInfo->ParentBrg!=NULL) &&
                (PciDevInfo->ParentBrg->Type!=tPciRootBrg) && (PciDevInfo->ParentBrg->PciExpress!=NULL) &&
                (PciDevInfo->ParentBrg->PciExpress->Pcie1Setup.MaxPayload!=PCI_SETUP_DONT_TOUCH) ) {
            PciSetupData->Pcie1Settings.MaxPayload = PciDevInfo->ParentBrg->PciExpress->Pcie1Setup.MaxPayload;
        } else {
            PciSetupData->Pcie1Settings.MaxPayload = PCI_SETUP_DONT_TOUCH;
        }

        PciSetupData->Pcie1Settings.MaxReadRequest = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie1Settings.AspmMode = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie1Settings.ExtendedSynch = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie1Settings.ClockPm = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie1Settings.LnkTrRetry = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie1Settings.LnkTrTimeout = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie1Settings.LnkDisable = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie1Settings.IgnoreCommonClock = PCI_SETUP_DONT_TOUCH;
        
        PciSetupData->Pcie1Settings.DevCtrDontTouch = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie1Settings.LnkCtrDontTouch = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie1Settings.SltCtrDontTouch = PCI_SETUP_DONT_TOUCH;
        
        PciSetupData->Pcie2Settings.ComplTimeOut = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie2Settings.AriFwd = gARIEnable;
        PciSetupData->Pcie2Settings.AtomOpReq = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie2Settings.AtomOpEgressBlk = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie2Settings.IDOReq = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie2Settings.IDOCompl = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie2Settings.LtrReport = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie2Settings.E2ETlpPrBlk = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie2Settings.LnkSpeed = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie2Settings.ComplSos = PCI_SETUP_DONT_TOUCH;
        
        PciSetupData->Pcie2Settings.HwAutoWidth = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie2Settings.HwAutoSpeed = PCI_SETUP_DONT_TOUCH;
        
        PciSetupData->Pcie2Settings.DevCtr2DontTouch = PCI_SETUP_DONT_TOUCH;
        PciSetupData->Pcie2Settings.LnkCtr2DontTouch = PCI_SETUP_DONT_TOUCH;
    }
    return EFI_SUCCESS;
}
#else
/**
     Sets PCI_SETUP_DONT_TOUCH for all the PCI Static Setup options

    @param       PciDevInfo             - Pointer to PCI Device info structure
    @param       PciCommon              - Pointer to the Pci Bus common setup data buffer
    @param       Pci                    - Pointer to the general PCI related setup data buffer
    @param       Pcie1                  - Pointer to the GEN 1 Pci related setup data buffer
    @param       Pcie2                  - Pointer to the GEN 2 Pci related setup data buffer
    @param       DevSdlInfo             - Pointer to the Pci device information
    @param       DevIdx                 - Index of the Pci device

    @return     EFI_STATUS
    @retval     Status            EFI_SUCCESS         - Success will be returned.
**/
EFI_STATUS
AmiPciGetSetupDataOverrideHook (
  PCI_DEV_INFO              *PciDevInfo,
  PCI_COMMON_SETUP_DATA     *PciCommon,
  PCI_DEVICE_SETUP_DATA     *Pci,
  PCIE1_SETUP_DATA          *Pcie1,
  PCIE2_SETUP_DATA          *Pcie2,
  AMI_SDL_PCI_DEV_INFO      *DevSdlInfo,    //OPTIONAL if ==NULL get defaults...
  UINTN                     DevIdx          //OPTIONAL 
)
{
    return EFI_SUCCESS;
}
#endif 


/**
     Constructor function to initialize Global variables

 * @param ImageHandle             Pointer to the handle that corresponds to this image
 * @param SystemTable             Pointer to the EFI_SYSTEM_TABLE


    @return EFI_STATUS
    @retval Status            EFI_SUCCESS         - Success will be returned.
**/
EFI_STATUS 
EFIAPI
AmiPciBusSetupOverrideLibConstructor(
    IN EFI_HANDLE                ImageHandle,
    IN EFI_SYSTEM_TABLE          *SystemTable
)
{
    EFI_STATUS Status;
    Status = GetOptionData (&gEfiSetupVariableGuid,  OFFSET_OF(SYSTEM_CONFIGURATION, ARIEnable), &gARIEnable, sizeof(UINT8));
    ASSERT_EFI_ERROR (Status);
    if(EFI_ERROR(Status)){
        gARIEnable = PCI_SETUP_DONT_TOUCH;
    }
    return EFI_SUCCESS;
}
