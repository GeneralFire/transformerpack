//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file AmiUsbInterfaceLib.h
    This file contains definitions and includes for UsbBmc.c.

*/

#ifndef _AMI_USB_INTERFACE_LIB_H_
#define _AMI_USB_INTERFACE_LIB_H_

//---------------------------------------------------------------------------

#include <AmiIpmiBmcCommon.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

// AMI BMC USB interface definitions.
#define AMI_IPMI_BEGIN_SIG                  {'$','G','2','-','C','O','N','F','I','G','-','H','O','S','T','$'} //"$G2-CONFIG-HOST$"
#define AMI_IPMI_BEGIN_SIG_LEN              16
#define AMI_IPMI_USB_CMD_RESERVED           0x0000
#define AMI_IPMI_CMD_SECTOR                 0x01
#define AMI_IPMI_DATA_SECTOR                0x02

#define AMI_IPMI_COMMON_READ_10_OPCODE      0x28
#define AMI_IPMI_COMMON_WRITE_10_OPCODE     0x2A

#define AMI_IPMI_SCSI_AMICMD_CURI_WRITE     0xE2
#define AMI_IPMI_SCSI_AMICMD_CURI_READ      0xE3

// USB Command Status Wrapper signature.
#define AMI_IPMI_BOT_CSW_SIGNATURE          SIGNATURE_32 ('U', 'S', 'B', 'S')

// USB Command Block Wrapper signature.
#define AMI_IPMI_BOT_CBW_SIGNATURE          SIGNATURE_32 ('U', 'S', 'B', 'C')

// BMC Usb interface status macros.
#define AMI_IPMI_IN_PROCESS                 0x8000  /* Bit 15 of Status */

#define AMI_IPMI_ERR_SUCCESS                0       /* Success */
#define AMI_IPMI_ERR_BIG_DATA               1       /* Too Much Data */
#define AMI_IPMI_ERR_NO_DATA                2       /* No/Less Data Available */
#define AMI_IPMI_ERR_UNSUPPORTED            3       /* Unsupported Command */

// General macro definitions.
#define AMI_IPMI_PM1_TIMER                  PM_BASE_ADDRESS + 8
#define AMI_IPMI_ONE_THOUSAND_MICRO_SECOND  1000
#define AMI_IPMI_TWO_THOUSAND_MS            2000
#define AMI_IPMI_THREE_THOUSAND_MS          3000

#define AMI_IPMI_TRANSFER_LENGTH            0x100  // Big Endian to little Endian: 0x0001 -> 0x0100
#define AMI_IPMI_CBW_FLAG_SHIFT             0x07
#define AMI_IPMI_CSW_PHASE_ERROR            0x02

#define AMI_IPMI_COMBINE_NETFUN_LUN(NetFunction, Lun) (UINT8)((NetFunction << 2) | (Lun & 0xfc))

#define AMI_IPMI_USB_COMMAND_DATA_HEADER_SIZE   2
#define AMI_IPMI_USB_RESPONSE_DATA_HEADER_SIZE  1

#define AMI_IPMI_MAX_BLOCK_PER_TRANSACTION      0x40
#define AMI_IPMI_BLOCK_SIZE                     512
#define USB_INTERFACE_DRIVER_VER                0x01

/* Max temp data should always be in multiple of 512 bytes.
   If you want to change the value of it, then change MAX_BLOCK_PER_TRANSACTION.*/
#define AMI_IPMI_MAX_USB_CMD_DATA_SIZE          (AMI_IPMI_BLOCK_SIZE * AMI_IPMI_MAX_BLOCK_PER_TRANSACTION)

#pragma pack(1)
/**
    Usb command config structure.
*/
typedef struct {
    /// Signature at the beginning.
    UINT8   BeginSig[AMI_IPMI_BEGIN_SIG_LEN];
    UINT16  Command;                    ///< Command.
    UINT16  Status;                     ///< Status.
    UINT32  DataInLen;                  ///< Data Length Sent to us.
    UINT32  DataOutLen;                 ///< Data Length Sent by us.
    UINT32  InternalUseDataIn;          ///< Internal use data in.
    UINT32  InternalUseDataOut;         ///< Internal use data out.
} AMI_IPMI_CONFIG_CMD;

/**
    Common command structure.
*/
typedef struct {
    /// 00h, Operation Code.
    UINT8   Opcode;
    UINT8   Lun;               ///< 01h, Logical Unit Number, etc.
    UINT32  Lba;               ///< 02h-05h, Logical Block Address.
    UINT8   Reserved;          ///< 06h, Reserved.
    UINT16  TransferLength;    ///< 07h-08h, Transfer Length(BIG ENDIAN).
    UINT8   Control;           ///< 09h, Control/Reserved.
} AMI_IPMI_COMN_RWV_CMD;

/**
    BOT command block structure.
*/
typedef struct {
    /// 0-3h, Command block wrapper signature = "USBC".
    UINT32  CbwSignature;
    UINT32  CbwTag;         ///< 4-7h, Tag.
    UINT32  CbwDataLength;  ///< 8-0Bh, Data transfer length.
    UINT8   CbwFlags;       ///< 0Ch, Flags.
    UINT8   CbwLun;         ///< 0Dh, Logical unit number.
    UINT8   CbwLength;      ///< 0Eh, Command block length.
    UINT8   CbwCb[16];      ///< 0Fh-1Eh, Command block buffer.
} AMI_IPMI_BOT_CMD_BLK;

/**
    Command status wrapper structure.
*/
typedef struct {
    /// Csw signature "USBS"
    UINT32  CswSignature;
    /// Tag used to link the command with status response.
    UINT32  CswTag;
    /// Size of remaining data that is not processed in this transfer.
    UINT32  CswDataResidue;
    /// Csw status byte.
    UINT8   CswStatus;
} AMI_IPMI_CSW;

/**
    Ipmi usb command data structure.
*/
typedef struct {
    /// NetFunction : 6 + Lun : 2
    UINT8   NetFnLun;
    /// Command.
    UINT8   Command;
    /// Command data.
    UINT8   CommandData[AMI_IPMI_MAX_USB_CMD_DATA_SIZE - AMI_IPMI_USB_COMMAND_DATA_HEADER_SIZE];
} AMI_IPMI_USB_COMMAND_DATA;

#pragma pack()

/** @internal
    This function enables BMC Virtual USB devices if not enabled.
    If enabled, registers notify function for exit boot services
    and legacy boot in order to disable usb interface.

    @param  VOID    Nothing.

    @retval EFI_SUCCESS Enabled usb interface and registered notify events.
    @retval Others      Error status returned from functions used.

**/
typedef EFI_STATUS (AMI_BMC_USB_ENABLE_AND_DISABLE) (VOID);

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

/** @internal
    Issues USB transaction to BMC USB interface.

    @param[in]      Interface       Pointer to System interface.
    @param[in]      Opcode          Opcode of the command.
    @param[in]      Sector          Data/Command sector.
    @param[in]      EndpointAddr    USB End point address.
    @param[in, out] Data            Pointer to input data.
    @param[in, out] DataSize        Size of input data.
    @param[out]     CompletionCode  Pointer to the completion code.

    @return EFI_STATUS  Return status of the usb transaction.

**/
EFI_STATUS
AmiIpmiIssueBmcUsbTransaction (
  IN     AMI_SYSTEM_INTERFACE   *Interface,
  IN     UINT8                  Opcode,
  IN     UINT8                  Sector,
  IN     UINT8                  EndpointAddr,
  IN OUT UINT8                  *Data,
  IN OUT UINT32                 DataSize,
  OUT    UINT8                  *CompletionCode
);

/** @internal
    Allocate memory for Usb interface, checks for Bmc usb interface
    and initializes the variables.

    @param[in, out] Interface   Pointer to System interface.

    @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
    @retval EFI_NOT_FOUND           Bmc usb interface is not found.
    @retval EFI_SUCCESS             Interface is found and initialized
                                    successfully.

**/
EFI_STATUS
AmiIpmiGetUsbApiPtr (
  IN OUT AMI_SYSTEM_INTERFACE   *Interface
);

/**
    Notify function to call the Bmc Usb interface hooks.

    @param[in] Event    Callback event.
    @param[in] Context  Pointer to calling context.

    @return VOID    Nothing.

**/
VOID
EFIAPI
BmcUsbInterfaceHook (
  IN EFI_EVENT  Event,
  IN VOID       *Context
);

/** @internal
    Gets bulk-in and bulk-out end points for USB.

    @param[in, out] Interface   Pointer to System interface.

    @retval EFI_SUCCESS     Endpoint addresses are found.
    @retval EFI_NOT_FOUND   Endpoint addresses are not found.
    @retval Others          Error status occurred while getting endpoints.

**/
EFI_STATUS
GetBulkEndPoints (
  IN OUT AMI_SYSTEM_INTERFACE   *Interface 
  );

/** @internal
    
    Checks for Vendor ID and Product ID of BMC USB Interface for given handle

    @param[in] EFI_HANDLE
    
    @return EFI_STATUS          Status returned from get device descriptor
    @retval EFI_UNSUPPORTED     Failed to get device description

**/
EFI_STATUS
IsBmcUsbUsingHandle(
  IN  EFI_HANDLE *ControllerHandle  
);
#endif  // #ifndef _AMI_USB_INTERFACE_LIB_H_

