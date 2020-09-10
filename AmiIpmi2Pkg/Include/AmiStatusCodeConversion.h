//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file AmiStatusCodeConversion.h
    Header for converting Status codes into equivalent IPMI format.

*/

#ifndef _AMI_STATUS_CODE_CONVERSION_H_
#define _AMI_STATUS_CODE_CONVERSION_H_

//---------------------------------------------------------------------------

#include <AmiStatusCodes.h>
#include <Library/AmiIpmiLib.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define AMI_EFI_SOFTWARE_EFI_BMC                    (EFI_SOFTWARE | 0x00150000)

#define AMI_IPMI_CU_EC0_DISABLED                    0x00000001
#define AMI_IPMI_CU_EC1_DISABLED                    0x00000011
#define AMI_IPMI_CU_EC2_DISABLED                    0x00000021
#define AMI_IPMI_CU_EC3_DISABLED                    0x00000031
#define AMI_IPMI_CU_EC4_DISABLED                    0x00000041
#define AMI_IPMI_CU_EC5_DISABLED                    0x00000051
#define AMI_IPMI_CU_EC6_DISABLED                    0x00000061
#define AMI_IPMI_CU_EC7_DISABLED                    0x00000071

#define AMI_IPMI_CU_EC0_SPARE                       0x00000005
#define AMI_IPMI_CU_EC1_SPARE                       0x00000015
#define AMI_IPMI_CU_EC2_SPARE                       0x00000025
#define AMI_IPMI_CU_EC3_SPARE                       0x00000035
#define AMI_IPMI_CU_EC4_SPARE                       0x00000045
#define AMI_IPMI_CU_EC5_SPARE                       0x00000055
#define AMI_IPMI_CU_EC6_SPARE                       0x00000065
#define AMI_IPMI_CU_EC7_SPARE                       0x00000075

// Sel logging constants
#define AMI_IPMI_SEL_LOGGING_DISABLED               0x00
#define AMI_IPMI_SEL_LOGGING_ENABLED                0x01

// Sel Erase constants
#define AMI_IPMI_DO_NOT_ERASE                       0x00
#define AMI_IPMI_ERASE_NEXT_BOOT                    0x01
#define AMI_IPMI_ERASE_EVERY_BOOT                   0x02

// Sel Full constants
#define AMI_IPMI_DO_NOTHING_WHEN_SEL_FULL           0x00
#define AMI_IPMI_ERASE_WHEN_SEL_FULL                0x01
#define AMI_IPMI_DEL_WHEN_SEL_FULL                  0x02

// LogSelStatusCodes constants
#define AMI_IPMI_LOG_SEL_STATUSCODE_DISABLED        0x00
#define AMI_IPMI_LOG_BOTH_PROGRESS_AND_ERROR_CODES  0x01
#define AMI_IPMI_LOG_ERROR_CODE_ONLY                0x02
#define AMI_IPMI_LOG_PROGRESS_CODE_ONLY             0x03

#pragma pack(1)
/**
    Status code to SEL entry conversion table structure.
*/
typedef struct {
    /// Status code.
    UINT32  StatusCode;
    UINT32  StatusCodeType; ///< Type of status code.
    UINT8   SensorTypeCode; ///< Sensor Specific Offset.
    UINT8   EventData1;     ///< ED1.
    UINT8   EventData2;     ///< ED2.
    UINT8   EventData3;     ///< ED3.
} AMI_STATUS_CODE_TO_SEL_ENTRY;
#pragma pack()

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

/**
    Status Code to SEL Conversion table.
    Entries should be added to this table to convert from EFI to SEL entry.
*/
static AMI_STATUS_CODE_TO_SEL_ENTRY gAmiStatusCodeToSelTable[] =
{
    //StatusCode                                StatusCodeType    SensorTypeCode
                                                                      //ED1 //ED2 //ED3
    { PEI_MEMORY_INIT,                          EFI_PROGRESS_CODE, 0x0F, 0XC2, 0x01, 0xFF },
    { DXE_IDE_BEGIN,                            EFI_PROGRESS_CODE, 0x0F, 0XC2, 0x02, 0xFF },
    { PEI_CPU_AP_INIT,                          EFI_PROGRESS_CODE, 0x0F, 0XC2, 0x03, 0xFF },
    { DXE_SETUP_VERIFYING_PASSWORD,             EFI_PROGRESS_CODE, 0x0F, 0XC2, 0x04, 0xFF },
    { DXE_SETUP_START ,                         EFI_PROGRESS_CODE, 0x0F, 0XC2, 0x05, 0xFF },
    { EFI_IO_BUS_USB | EFI_IOB_PC_INIT,         EFI_PROGRESS_CODE, 0x0F, 0XC2, 0x06, 0xFF },
    { DXE_PCI_BUS_REQUEST_RESOURCES,            EFI_PROGRESS_CODE, 0x0F, 0XC2, 0x07, 0xFF },
    { DXE_LEGACY_OPROM_INIT,                    EFI_PROGRESS_CODE, 0x0F, 0XC2, 0x08, 0xFF },
    { DXE_CON_OUT_CONNECT,                      EFI_PROGRESS_CODE, 0x0F, 0XC2, 0x09, 0xFF },
    { PEI_CPU_CACHE_INIT,                       EFI_PROGRESS_CODE, 0x0F, 0XC2, 0x0A, 0xFF },
    // SM Bus Initialization.
    { EFI_IO_BUS_SMBUS | EFI_IOB_PC_INIT,       EFI_PROGRESS_CODE, 0x0F, 0XC2, 0x0B, 0xFF },
    { DXE_CON_IN_CONNECT,                       EFI_PROGRESS_CODE, 0x0F, 0XC2, 0x0C, 0xFF },
    // Embedded controller/management controller initialization.
    { EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | \
      EFI_CU_PC_INIT_BEGIN ,                      EFI_PROGRESS_CODE, 0x0F, 0XC2, 0x0D, 0xFF },
    { EFI_SOFTWARE_PEI_MODULE               | \
      EFI_SW_PEI_PC_OS_WAKE,                      EFI_PROGRESS_CODE, 0x0F, 0XC2, 0x12, 0xFF },
    { DXE_LEGACY_BOOT,                          EFI_PROGRESS_CODE, 0x0F, 0XC2, 0x13, 0xFF },
    { DXE_KEYBOARD_SELF_TEST,                   EFI_PROGRESS_CODE, 0x0F, 0XC2, 0x17, 0xFF },
    
    // Pointing device test.
    { EFI_PERIPHERAL_MOUSE                 | \
    EFI_P_MOUSE_PC_SELF_TEST,                   EFI_PROGRESS_CODE, 0x0F, 0xC2, 0x18, 0xFF },
    // Primary processor initialization.
    { EFI_COMPUTING_UNIT_HOST_PROCESSOR     | \
      EFI_CU_HP_PC_POWER_ON_INIT,                 EFI_PROGRESS_CODE, 0x0F, 0XC2, 0x19, 0xFF },
    // Starting operating system boot process, eg. calling Int 19h
    { EFI_SOFTWARE_EFI_BOOT_SERVICE         | \
      EFI_SW_BS_PC_EXIT_BOOT_SERVICES,            EFI_PROGRESS_CODE, 0x0F, 0xC2, 0x13, 0xFF },
    
    { PEI_MEMORY_NOT_DETECTED,                  EFI_ERROR_CODE,    0x0F, 0XC0, 0x01, 0xFF },
    { PEI_MEMORY_NONE_USEFUL,                   EFI_ERROR_CODE,    0x0F, 0XC0, 0x02, 0xFF },
    // Unrecoverable hard dik/ATAPI/IDE device failure.
    { DXE_IDE_DEVICE_FAILURE,                   EFI_ERROR_CODE,    0x0F, 0XC0, 0x03, 0xFF },
    // Unrecoverable system-board failure.
    { EFI_COMPUTING_UNIT_UNSPECIFIED        | \
    EFI_ERROR_UNRECOVERED,                      EFI_ERROR_CODE,    0x0F, 0XC0, 0x04, 0xFF },
    // Unrecoverable hard disk controller failure.
    { DXE_IDE_CONTROLLER_ERROR,                 EFI_ERROR_CODE,    0x0F, 0XC0, 0x06, 0xFF },
    { EFI_PERIPHERAL_KEYBOARD               | \
    EFI_P_EC_CONTROLLER_ERROR,                  EFI_ERROR_CODE,    0x0F, 0XC0, 0x07, 0xFF },
    { EFI_PERIPHERAL_REMOVABLE_MEDIA        | \
    EFI_P_EC_CONTROLLER_ERROR,                  EFI_ERROR_CODE,    0x0F, 0XC0, 0x08, 0xFF },
    // Unrecoverable Media control failure.
    { EFI_PERIPHERAL_LOCAL_CONSOLE          | \
    EFI_P_EC_CONTROLLER_ERROR,                  EFI_ERROR_CODE,    0x0F, 0XC0, 0x09, 0xFF },
    { DXE_NO_CON_OUT,                           EFI_ERROR_CODE,    0x0F, 0XC0, 0x0A, 0xFF },
    { EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | \
    EFI_IOB_EC_CONTROLLER_ERROR,                EFI_ERROR_CODE,    0x0F, 0XC0, 0x0B, 0xFF },
    // CPU voltage mismatch 
    { EFI_COMPUTING_UNIT_HOST_PROCESSOR     | \
      EFI_CU_HP_EC_LOW_VOLTAGE,                   EFI_ERROR_CODE,    0x0F, 0XC0, 0x0C, 0xFF },
     // CPU speed match failure.
    { EFI_COMPUTING_UNIT_HOST_PROCESSOR     | \
      EFI_COMPUTING_UNIT_MISMATCH_SPEED,          EFI_ERROR_CODE,    0x0F, 0XC0, 0x0D, 0xFF },
    
    // Processor Disabled.
    { (EFI_COMPUTING_UNIT_HOST_PROCESSOR    | \
    EFI_CU_EC_DISABLED) ,                       EFI_ERROR_CODE,    0x07, 0x08, 0xFF, 0xFF },
    
    // Configuration Error.
    { (EFI_COMPUTING_UNIT_HOST_PROCESSOR    | \
    EFI_CU_HP_EC_MISMATCH) ,                    EFI_ERROR_CODE,    0x07, 0x05, 0xFF, 0xFF },
    
    // Thermal Trip.
    { (EFI_COMPUTING_UNIT_HOST_PROCESSOR    | \
    EFI_CU_HP_EC_THERMAL ) ,                    EFI_ERROR_CODE,    0x07, 0x01, 0xFF, 0xFF },
    
    // BIST failure.
    { (EFI_COMPUTING_UNIT_HOST_PROCESSOR    | \
    EFI_CU_HP_EC_SELF_TEST) ,                   EFI_ERROR_CODE,    0x07, 0x02, 0xFF, 0xFF },
    
    // IERR.
    { (EFI_COMPUTING_UNIT_HOST_PROCESSOR    | \
    EFI_CU_HP_EC_INTERNAL ) ,                   EFI_ERROR_CODE,    0x07, 0x00, 0xFF, 0xFF },
    
    { AMI_EFI_SOFTWARE_EFI_BMC              | \
    EFI_SW_EC_EVENT_LOG_FULL,                   EFI_ERROR_CODE,    0x10, 0x04, 0XFF, 0XFF },
    
    // Sensor Type: 0x13 - Critical Interrupt Sensor Type.
    // PCI PERR.
    { EFI_IO_BUS_PCI                        | \
    EFI_IOB_PCI_EC_PERR,                        EFI_ERROR_CODE,    0x13, 0x04, 0XFF, 0XFF },
    
    // PCI SERR.
    { EFI_IO_BUS_PCI                        | \
    EFI_IOB_PCI_EC_SERR,                        EFI_ERROR_CODE,    0x13, 0x05, 0XFF, 0XFF },
    
    { EFI_COMPUTING_UNIT_MEMORY             | \
    EFI_CU_EC_NOT_CONFIGURED,                   EFI_ERROR_CODE,    0x0C, 0x07, 0XFF, 0XFF },
    { EFI_COMPUTING_UNIT_MEMORY             | \
    AMI_IPMI_CU_EC0_DISABLED,                   EFI_ERROR_CODE,    0x0C, 0X24, 0XFF, 0x00 },
    { EFI_COMPUTING_UNIT_MEMORY             | \
    AMI_IPMI_CU_EC1_DISABLED,                   EFI_ERROR_CODE,    0x0C, 0X24, 0XFF, 0x01 },
    { EFI_COMPUTING_UNIT_MEMORY             | \
    AMI_IPMI_CU_EC2_DISABLED,                   EFI_ERROR_CODE,    0x0C, 0X24, 0XFF, 0x02 },
    { EFI_COMPUTING_UNIT_MEMORY             | \
    AMI_IPMI_CU_EC3_DISABLED,                   EFI_ERROR_CODE,    0x0C, 0X24, 0XFF, 0x03 },
    { EFI_COMPUTING_UNIT_MEMORY             | \
    AMI_IPMI_CU_EC4_DISABLED,                   EFI_ERROR_CODE,    0x0C, 0X24, 0XFF, 0x04 },
    { EFI_COMPUTING_UNIT_MEMORY             | \
    AMI_IPMI_CU_EC5_DISABLED,                   EFI_ERROR_CODE,    0x0C, 0X24, 0XFF, 0x05 },
    { EFI_COMPUTING_UNIT_MEMORY             | \
    AMI_IPMI_CU_EC6_DISABLED,                   EFI_ERROR_CODE,    0x0C, 0X24, 0XFF, 0x06 },
    { EFI_COMPUTING_UNIT_MEMORY             | \
    AMI_IPMI_CU_EC7_DISABLED,                   EFI_ERROR_CODE,    0x0C, 0X24, 0XFF, 0x07 },
    // Correctable ECC/other correctable memory error.
    { GENERIC_MEMORY_CORRECTABLE_ERROR,         EFI_ERROR_CODE,    0x0C, 0x00, 0xFF, 0xFF },
    // Uncorrectable ECC/other uncorrectable memory error.
    { GENERIC_MEMORY_UNCORRECTABLE_ERROR,       EFI_ERROR_CODE,    0x0C, 0x01, 0xFF, 0xFF },
    
    { EFI_COMPUTING_UNIT_MEMORY             | \
    AMI_IPMI_CU_EC0_SPARE,                      EFI_PROGRESS_CODE, 0x0C, 0X28, 0XFF, 0x00 },
    
    { EFI_COMPUTING_UNIT_MEMORY             | \
    AMI_IPMI_CU_EC1_SPARE,                      EFI_PROGRESS_CODE, 0x0C, 0X28, 0XFF, 0x01 },
    
    { EFI_COMPUTING_UNIT_MEMORY             | \
    AMI_IPMI_CU_EC2_SPARE,                      EFI_PROGRESS_CODE, 0x0C, 0X28, 0XFF, 0x02 },
    
    { EFI_COMPUTING_UNIT_MEMORY             | \
    AMI_IPMI_CU_EC3_SPARE,                      EFI_PROGRESS_CODE, 0x0C, 0X28, 0XFF, 0x03 },
    
    { EFI_COMPUTING_UNIT_MEMORY             | \
    AMI_IPMI_CU_EC4_SPARE,                      EFI_PROGRESS_CODE, 0x0C, 0X28, 0XFF, 0x04 },
    
    { EFI_COMPUTING_UNIT_MEMORY             | \
    AMI_IPMI_CU_EC5_SPARE,                      EFI_PROGRESS_CODE, 0x0C, 0X28, 0XFF, 0x05 },
    
    { EFI_COMPUTING_UNIT_MEMORY             | \
    AMI_IPMI_CU_EC6_SPARE,                      EFI_PROGRESS_CODE, 0x0C, 0X28, 0XFF, 0x06 },
    
    { EFI_COMPUTING_UNIT_MEMORY             | \
    AMI_IPMI_CU_EC7_SPARE,                      EFI_PROGRESS_CODE, 0x0C, 0X28, 0XFF, 0x07 },
    // Presence detected. Indicate presence of entity associated with the sensor.
    { PEI_MEMORY_PRESENCE_DETECT,               EFI_PROGRESS_CODE, 0x0C, 0x06, 0xFF, 0xFF }
// gAmiStatusCodeToSelTable End.
};

//---------------------------------------------------------------------------

#endif  // #ifndef _AMI_STATUS_CODE_CONVERSION_H_

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
