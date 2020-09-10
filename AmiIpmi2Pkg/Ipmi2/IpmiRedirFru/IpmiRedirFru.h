//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file IpmiRedirFru.h
    Ipmi Redir Fru driver header files, macros and function declarations.

*/

#ifndef _IPMI_REDIR_FRU_H_
#define _IPMI_REDIR_FRU_H_

#include <Token.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/AmiIpmiLib.h>
#include <Protocol/IpmiTransportProtocol.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>
#include <AmiIpmiNetFnStorageDefinitions.h>
#include <AmiFru.h>

#define CHASSIS_TYPE_LENGTH                     1
#define CHASSIS_TYPE_OFFSET                     2
#define CHASSIS_PART_NUMBER                     3

#define PRODUCT_MFG_OFFSET                      3
#define BOARD_MFG_OFFSET                        6
#define FRU_DEVICE_LOCATOR_RECORD               0x11
#define MANAGEMENT_ACCESS_RECORD                0x03
#define SYSTEM_UUID_SUB_RECORD_TYPE             0x07
#define RECORD_CHECKSUM_SIZE                    sizeof(UINT8)

#define FRU_DATA_LENGTH_MASK                    0x3F
#define FRU_DATA_TYPE_MASK                      0xC0

#define IPMI_FRU_COMMAND_RETRY_DELAY            30000
#define IPMI_FRU_COMMAND_RETRY_COUNTER          3
#define FRU_CC_CMD_SUCCESS                      0x00
#define FRU_CC_DEVICE_BUSY                      0x81
#define FRU_CC_WRITE_PROTECTED                  0x80
#define MAX_REQ_RES_DATA_SIZE                   0xFF

#define MAX_STRING_SIZE                         0xFF // As per latest SMBIOS Spec no Limit for String Size so taking MAX length of the String size
#define SIZE_OF_SMBIOS_STRUCTURE_END_MACRO      sizeof(UINT16)  /* Macro represent size of Smbios structure end. Every Smbios type ends with 0x0000.*/

#if (MDE_PKG_VERSION <= 18)
# pragma pack (1)
typedef struct {
  UINT8   DeviceId;
  UINT16  InventoryOffset;
  UINT8   CountToRead;
} IPMI_READ_FRU_DATA_REQUEST;

typedef struct {
  UINT8   CompletionCode;
  UINT8   CountReturned;
  UINT8   Data[0];
} IPMI_READ_FRU_DATA_RESPONSE;

typedef struct {
  UINT8   DeviceId;
  UINT16  InventoryOffset;
  UINT8   Data[0];
} IPMI_WRITE_FRU_DATA_REQUEST;

typedef struct {
  UINT8   CompletionCode;
  UINT8   CountWritten;
} IPMI_WRITE_FRU_DATA_RESPONSE;
# pragma pack ()
#endif
/** @internal
    Reads the FRU information and publishes the FRU stings by updating the
    dynamic PCDs. If the FRU information is not present, publishes dummy records.

    @param[in] This                         Pointer to the Redir FRU protocol.
    @param[in] GenerateDummySmbiosRecords   If TRUE, generates dummy FRU records.
                                            If FALSE, generates FRU records.

    @return VOID    Nothing.

**/
VOID
GenerateFruSmbiosData (
  IN EFI_SM_FRU_REDIR_PROTOCOL  *This
);

/** @internal
    Registers notification callback on SMBIOS protocol to update FRU Smbios
    structure.

    @param VOID     Nothing.

    @retval VOID    Nothing.

**/
VOID
UpdateFruSmbiosTables (
  VOID
);

#endif  // #ifndef _IPMI_REDIR_FRU_H_


