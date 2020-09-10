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

#include <Token.h>
#include <PciBus.h>
#include <PciSetup.h>
#include <Protocol/AmiBoardInfo2.h>
#include <Setup.h>

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
    return EFI_SUCCESS;
}

/**
    Sets PCI_SETUP_DONT_TOUCH for all the common PCI Dynamic Setup options

    @param       PciDevInfo             - Pointer to PCI Device info structure
    @param       PciCommon              - Pointer to the PCI Bus common setup data buffer

    @return EFI_STATUS
    @retval Status            EFI_SUCCESS         - Success will be returned.
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
