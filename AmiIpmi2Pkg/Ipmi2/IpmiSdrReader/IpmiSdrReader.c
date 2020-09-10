//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file IpmiSdrReader.c
    Server Management Ipmi SDR reader Driver. Reads the SDR version details
    from the BMC. Updates the SDR version in Dynamic PCD.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <AmiIpmiNetFnStorageDefinitions.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/IpmiTransportProtocol.h>

//---------------------------------------------------------------------------
#define MAX_STRING_SIZE  0xFF
/**
    Entry point to read the SDR version details from the BMC. Updates the SDR
    version in dynamic pcd.

    @param[in] ImageHandle  Handle of this driver image.
    @param[in] SystemTable  Table containing standard EFI services.

    @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
    @retval EFI_SUCCESS             Updated the SDR version in pcd
                                    successfully.
    @retval Others                  Return status of the functions used.

**/
EFI_STATUS
EFIAPI
InitializeIpmiSdrReader (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable )
{
    EFI_STATUS                            Status;
    IPMI_GET_SDR_REPOSITORY_INFO_RESPONSE SdrInfo;
    UINT8                                 SdrInfoSize = sizeof (SdrInfo);
    UINT16                                *SdrVersionNo = NULL;
    UINTN                                 SizeofBuffer;
    EFI_IPMI_TRANSPORT                    *IpmiTransport;

    DEBUG ((DEBUG_INFO, "%a Entry... \n", __FUNCTION__));

    Status = gBS->LocateProtocol (
                    &gEfiDxeIpmiTransportProtocolGuid,
                    NULL,
                    (VOID **)&IpmiTransport );
    
    DEBUG ((DEBUG_INFO, " gEfiDxeIpmiTransportProtocolGuid Status: %r \n", Status));
    
    if (EFI_ERROR (Status)) {
        return Status;
    }

    // Get SDR Version using Get SDR Repository Info command.
    Status = IpmiTransport->SendIpmiCommand (
                                IpmiTransport,
                                IPMI_NETFN_STORAGE,
                                AMI_BMC_LUN,
                                IPMI_STORAGE_GET_SDR_REPOSITORY_INFO,
                                NULL,
                                0,
                                (UINT8 *)&SdrInfo,
                                &SdrInfoSize );
    
    DEBUG ((DEBUG_INFO, " IPMI_STORAGE_GET_SDR_REPOSITORY_INFO Status: %r\n", Status ));
    
    if (EFI_ERROR (Status)) {
        return Status;
    }

    //Allocate size for two nibbles and a dot as a Unicode string with NULL terminator.
    SdrVersionNo = AllocateRuntimeZeroPool ((UINTN)(sizeof (UINT16) * 4));
    if (SdrVersionNo == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }
    DEBUG ((DEBUG_INFO, "SdrVersionNo Address : %x\n", SdrVersionNo));

    /* Got the required SDR version Number in BCD format
       Convert it to Unicode format and Update the same in PCD.
       Version will be in format LSB.MSB example. 51h as 1.5*/
    SdrVersionNo[0] = (UINT8)((SdrInfo.Version & AMI_IPMI_MASK_LOWER_NIBBLE) + 0x30); // LSB of Version
    SdrVersionNo[1] = '.'; // Dot Separates LSB and MSB
    SdrVersionNo[2] = (UINT8)((SdrInfo.Version >> 0x04) + 0x30); // MSB of Version

    DEBUG ((DEBUG_INFO, "SdrVersionNo: %S \n", SdrVersionNo));

    SizeofBuffer = StrnSizeS (SdrVersionNo, MAX_STRING_SIZE);
    PcdSetPtr (
        AmiPcdSdrVersionNo,
        &SizeofBuffer,
        (VOID*)SdrVersionNo );
    DEBUG ((DEBUG_INFO, "SdrVersionNo: %S\n", (CHAR16 *) PcdGetPtr (AmiPcdSdrVersionNo)));

    FreePool (SdrVersionNo);
    DEBUG ((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));
    return EFI_SUCCESS;
}


