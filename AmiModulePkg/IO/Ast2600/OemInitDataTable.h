//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file OEMInitDataTable.h
    This contains the initialization tables of the Super IO.
**/
#ifndef  _OEMINITDATATABLE_H_
#define  _OEMINITDATATABLE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <Token.h>

/**
 *  SerialPortDecodeTable[]
 *   This table will only initialize the Serial port for debug purpose.
 *   For other registers, please fill PeiDecodeTable[].
 */
IO_DECODE_DATA SerialPortDecodeTable[]=
{
    // -----------------------------
    //|  BaseAdd | UID  | Type |
    // -----------------------------
    {AST2600_CONFIG_INDEX, 2, 0xFF},
    //Below decode is for DEBUG Mode
    {DEBUG_COM_PORT_ADDR, 0x00, dsUART},
};

/**
 *  SerialPortInitTable[]
 *   This table will only initialize the Serial port for debug purpose.
 *   For other registers, please fill PeiInitTable[].
 */
SIO_DEVICE_INIT_DATA SerialPortInitTable[]=
{
    // -----------------------------
    //|  Addr | DataMask  | DataValue |
    // -----------------------------
    //------------------------------------------------------------------
    // Enter Configuration Mode.
    //------------------------------------------------------------------
    {AST2600_CONFIG_INDEX, 0x00, AST2600_CONFIG_MODE_ENTER_VALUE},
    {AST2600_CONFIG_INDEX, 0x00, AST2600_CONFIG_MODE_ENTER_VALUE},

    //------------------------------------------------------------------
    // Before init all logical devices, program Global register if needed.
    //------------------------------------------------------------------

    //------------------------------------------------------------------
    // Initialize the Serial Port for debug useage. Default is COMA
    //------------------------------------------------------------------
#if (AST2600_INTERFACE_SELECT)//eSPI mode
    // Select device
    {AST2600_CONFIG_INDEX, 0x00, AST2600_LDN_SEL_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, AST2600_LDN_UART1}, 
    // Disable UART1 device before programming
    {AST2600_CONFIG_INDEX, 0x00, AST2600_ACTIVATE_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, AST2600_DEACTIVATE_VALUE}, 
    // Program Base Addr and Irq
    {AST2600_CONFIG_INDEX, 0x00, AST2600_BASE1_HI_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, (UINT8)(DEBUG_COM_PORT_ADDR >> 8)}, 
    {AST2600_CONFIG_INDEX, 0x00, AST2600_BASE1_LO_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, (UINT8)(UINT8)(DEBUG_COM_PORT_ADDR & 0xFF)}, 
    {AST2600_CONFIG_INDEX, 0x00, AST2600_IRQ1_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, (UINT8)(0x04)}, 
    {AST2600_CONFIG_INDEX, 0x00, AST2600_IRQ1TYPE_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, (UINT8)(0x00)}, 
    // Activate Device
    {AST2600_CONFIG_INDEX, 0x00, AST2600_ACTIVATE_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, AST2600_ACTIVATE_VALUE}, 
    // Program Base Addr and Irq for UART2
    {AST2600_CONFIG_INDEX, 0x00, AST2600_LDN_SEL_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, AST2600_LDN_UART2}, 
    {AST2600_CONFIG_INDEX, 0x00, AST2600_IRQ1_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, (UINT8)(0x03)}, 
    {AST2600_CONFIG_INDEX, 0x00, AST2600_IRQ1TYPE_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, (UINT8)(0x00)},
#else
    // Select device
    {AST2600_CONFIG_INDEX, 0x00, AST2600_LDN_SEL_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, DEBUG_LDN_UART},
    // Program Base Addr
    {AST2600_CONFIG_INDEX, 0x00, AST2600_BASE1_LO_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, (UINT8)(DEBUG_COM_PORT_ADDR & 0xFF)},
    {AST2600_CONFIG_INDEX, 0x00, AST2600_BASE1_HI_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, (UINT8)(DEBUG_COM_PORT_ADDR >> 8)},
    // Activate Device
    {AST2600_CONFIG_INDEX, 0x00, AST2600_ACTIVATE_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, AST2600_ACTIVATE_VALUE},
#endif //#if (AST2600_INTERFACE_SELECT)
    //------------------------------------------------------------------
    // After init all logical devices,  Exit Configuration Mode.
    //------------------------------------------------------------------
    {AST2600_CONFIG_INDEX, 0x00, AST2600_CONFIG_MODE_EXIT_VALUE}, // AST2600_CONFIG_MODE_EXIT_VALUE = 0xAA
};


/**
 *  PeiDecodeTable[]
 *   This Table will be loaded in PeiInit.c
 */
IO_DECODE_DATA PeiDecodeTable[]=
{
    // -----------------------------
    //|  BaseAdd | UID  | Type |
    // -----------------------------
    {AST2600_CONFIG_INDEX, 2, 0xFF},
    //Below decode is for recovery mode
#if (AST2600_RECOVERY_SUPPORT)
#if SerialRecovery_SUPPORT 
    {RECOVERY_COM_PORT_ADDR, 0x00, dsUART},
#endif
#endif
    //Below decode is for SIO generic IO decode
#if defined(AST2600_TOTAL_BASE_ADDRESS) && (AST2600_TOTAL_BASE_ADDRESS != 0)
    {AST2600_TOTAL_BASE_ADDRESS, AST2600_TOTAL_LENGTH, 0xFF},
#endif
    // !!!!Attention!!!!This is necessary
    //OEM_TODO//OEM_TODO//OEM_TODO//OEM_TODO
    // If your COM port number > 2 , you'd add more table for more COM ports.
    {0x3E0, 0x10, 0xFF}, // 0x3E0~0x3F0 , open a IODecode section for UART3 4
    {0x220, 0x10, 0xFF}, // 0x220~0x22F , open a IODecode section for UART3 4
    // Add more OEM IO decode below.
};

/**
 *  PeiInitTable[]
 *   This Table will be loaded in PeiInit.c
 */
SIO_DEVICE_INIT_DATA PeiInitTable[]=
{
    // -----------------------------
    //|  Addr | DataMask  | DataValue |
    // -----------------------------

    //------------------------------------------------------------------
    // Enter Configuration Mode.
    //------------------------------------------------------------------
    {AST2600_CONFIG_INDEX, 0x00, AST2600_CONFIG_MODE_ENTER_VALUE},
    {AST2600_CONFIG_INDEX, 0x00, AST2600_CONFIG_MODE_ENTER_VALUE},

    //------------------------------------------------------------------
    // Before init all logical devices, program Global register if needed.
    //------------------------------------------------------------------

    //------------------------------------------------------------------
    // Initialize Serial Port and Floppy Controller for Recovery
    //------------------------------------------------------------------
#if (AST2600_RECOVERY_SUPPORT)
#if (AST2600_INTERFACE_SELECT)//eSPI mode
    // Select device
    {AST2600_CONFIG_INDEX, 0x00, AST2600_LDN_SEL_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, AST2600_LDN_UART1}, 
    //Disable UART1 device before programming
    {AST2600_CONFIG_INDEX, 0x00, AST2600_ACTIVATE_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, AST2600_DEACTIVATE_VALUE}, 
    // Program Base Addr
    {AST2600_CONFIG_INDEX, 0x00, AST2600_BASE1_HI_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, (UINT8)(RECOVERY_COM_PORT_ADDR >> 8)}, 
    {AST2600_CONFIG_INDEX, 0x00, AST2600_BASE1_LO_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, (UINT8)(UINT8)(RECOVERY_COM_PORT_ADDR & 0xFF)}, 
    {AST2600_CONFIG_INDEX, 0x00, AST2600_IRQ1_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, (UINT8)(0x04)}, 
    {AST2600_CONFIG_INDEX, 0x00, AST2600_IRQ1TYPE_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, (UINT8)(0x00)}, 
    // Activate Device
    {AST2600_CONFIG_INDEX, 0x00, AST2600_ACTIVATE_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, AST2600_ACTIVATE_VALUE}, 
    // Program Base Addr and Irq for UART2
    {AST2600_CONFIG_INDEX, 0x00, AST2600_LDN_SEL_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, AST2600_LDN_UART2}, 
    {AST2600_CONFIG_INDEX, 0x00, AST2600_IRQ1_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, (UINT8)(0x03)}, 
    {AST2600_CONFIG_INDEX, 0x00, AST2600_IRQ1TYPE_REGISTER}, 
    {AST2600_CONFIG_DATA,  0x00, (UINT8)(0x00)}, 
#else
    // Select device
    {AST2600_CONFIG_INDEX, 0x00, AST2600_LDN_SEL_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, RECOVERY_LDN_UART},
    // Program Base Addr
    {AST2600_CONFIG_INDEX, 0x00, AST2600_BASE1_LO_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, (UINT8)(RECOVERY_COM_PORT_ADDR & 0xFF)},
    {AST2600_CONFIG_INDEX, 0x00, AST2600_BASE1_HI_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, (UINT8)(RECOVERY_COM_PORT_ADDR >> 8)},
    // Activate Device
    {AST2600_CONFIG_INDEX, 0x00, AST2600_ACTIVATE_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, AST2600_ACTIVATE_VALUE},
#endif //#if (AST2600_INTERFACE_SELECT)
#endif //AST2600_RECOVERY_SUPPORT

    //------------------------------------------------------------------
    // Program and initialize some logical device if needed.
    //------------------------------------------------------------------ 
#if (AST2600_INTERFACE_SELECT)//eSPI mode
    //
    //SWC
    //
#if (AST2600_SWC_PRESENT)
    {AST2600_CONFIG_INDEX, 0x00, AST2600_LDN_SEL_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, AST2600_LDN_SWC},
    {AST2600_CONFIG_INDEX, 0x00, AST2600_IRQ1_REGISTER},
    {AST2600_CONFIG_DATA,  0xF0, 0x0C},
    {AST2600_CONFIG_INDEX, 0x00, AST2600_IRQ1TYPE_REGISTER},
    {AST2600_CONFIG_DATA,  0xFC, 0x00},
//    {AST2600_CONFIG_INDEX, 0x00, AST2600_ACTIVATE_REGISTER},
//    {AST2600_CONFIG_DATA,  0x00, AST2600_ACTIVATE_VALUE},
#else
    // Deactivate Device
    {AST2600_CONFIG_INDEX, 0x00, AST2600_LDN_SEL_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, AST2600_LDN_SWC},
    {AST2600_CONFIG_INDEX, 0x00, AST2600_ACTIVATE_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, AST2600_DEACTIVATE_VALUE},
#endif //#if (AST2600_SWC_PRESENT)
    //
    //GPIO
    //
#if (AST2600_GPIO_PORT_PRESENT)
    {AST2600_CONFIG_INDEX, 0x00, AST2600_LDN_SEL_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, AST2600_LDN_GPIO},
//    {AST2600_CONFIG_INDEX, 0x00, 0x38},//bit7:enable inverse polarity of port 80h GPIO, BIt[0-4]: IO band selection
//    {AST2600_CONFIG_DATA,  0x00, 0x00},
    {AST2600_CONFIG_INDEX, 0x00, AST2600_IRQ1_REGISTER},
    {AST2600_CONFIG_DATA,  0xF0, 0x0A},
    {AST2600_CONFIG_INDEX, 0x00, AST2600_IRQ1TYPE_REGISTER},
    {AST2600_CONFIG_DATA,  0xFC, 0x00},
//    {AST2600_CONFIG_INDEX, 0x00, AST2600_ACTIVATE_REGISTER},
//    {AST2600_CONFIG_DATA,  0x00, AST2600_ACTIVATE_VALUE},
#else
    // Deactivate Device
    {AST2600_CONFIG_INDEX, 0x00, AST2600_LDN_SEL_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, AST2600_LDN_GPIO},
    {AST2600_CONFIG_INDEX, 0x00, AST2600_ACTIVATE_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, AST2600_DEACTIVATE_VALUE},
#endif //#if (AST2600_GPIO_PORT_PRESENT)
    //ILPC2AHB
#if (AST2600_ILPC2AHB_PRESENT)
    {AST2600_CONFIG_INDEX, 0x00, AST2600_LDN_SEL_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, AST2600_LDN_ILPC2AHB},
    {AST2600_CONFIG_INDEX, 0x00, AST2600_IRQ1_REGISTER},
    {AST2600_CONFIG_DATA,  0xF0, 0x0B},
    {AST2600_CONFIG_INDEX, 0x00, AST2600_IRQ1TYPE_REGISTER},
    {AST2600_CONFIG_DATA,  0xFC, 0x00},
//    {AST2600_CONFIG_INDEX, 0x00, AST2600_ACTIVATE_REGISTER},
//    {AST2600_CONFIG_DATA,  0x00, AST2600_ACTIVATE_VALUE},
#else
    // Deactivate Device
    {AST2600_CONFIG_INDEX, 0x00, AST2600_LDN_SEL_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, AST2600_LDN_ILPC2AHB},
    {AST2600_CONFIG_INDEX, 0x00, AST2600_ACTIVATE_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, AST2600_DEACTIVATE_VALUE},
#endif //#if (AST2600_LPC2AHB_PRESENT)
    //
    //MAILBOX
    //
#if (AST2600_MAILBOX_PRESENT)
    {AST2600_CONFIG_INDEX, 0x00, AST2600_LDN_SEL_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, AST2600_LDN_MAILBOX},
    {AST2600_CONFIG_INDEX, 0x00, AST2600_IRQ1_REGISTER},
    {AST2600_CONFIG_DATA,  0xF0, 0x0D},
    {AST2600_CONFIG_INDEX, 0x00, AST2600_IRQ1TYPE_REGISTER},
    {AST2600_CONFIG_DATA,  0xFC, 0x00},
//    {AST2600_CONFIG_INDEX, 0x00, AST2600_ACTIVATE_REGISTER},
//    {AST2600_CONFIG_DATA,  0x00, AST2600_ACTIVATE_VALUE},
#else
    // Deactivate Device
    {AST2600_CONFIG_INDEX, 0x00, AST2600_LDN_SEL_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, AST2600_LDN_MAILBOX},
    {AST2600_CONFIG_INDEX, 0x00, AST2600_ACTIVATE_REGISTER},
    {AST2600_CONFIG_DATA,  0x00, AST2600_DEACTIVATE_VALUE},
#endif //#if (AST2600_MAILBOX_PRESENT)
    
#endif //#if (AST2600_INTERFACE_SELECT)

    //------------------------------------------------------------------
    // After init all logical devices, program Global register if needed.
    //------------------------------------------------------------------

    //------------------------------------------------------------------
    // After init all logical devices,  Exit Configuration Mode.
    //------------------------------------------------------------------
    {AST2600_CONFIG_INDEX, 0x00, AST2600_CONFIG_MODE_EXIT_VALUE},
};


///---------------------------------------------------------------------
///This table will be filled when COMs isBeforeActivate.
///---------------------------------------------------------------------
SIO_DEVICE_INIT_DATA   DxeComInitTableBeforeActive[] = {
    //-----------------------------
    //| Reg8 | AndData8  | OrData8  |
    //-----------------------------
    //OEM_TODO: Base on OEM board.
    //Program COM clock source Registers.
    {0xF0, 0xFD, 0x00}, //BIT1: 0: 24Mhz/13, 1: 24Mhz\no effect when com1 disabled
    {0xF0, 0xFD, 0x00}, //BIT1: 0: 24Mhz/13, 1: 24Mhz\no effect when com2 disabled
    {0xF0, 0xFD, 0x00}, //BIT1: 0: 24Mhz/13, 1: 24Mhz\no effect when com3 disabled
    {0xF0, 0xFD, 0x00}, //BIT1: 0: 24Mhz/13, 1: 24Mhz\no effect when com4 disabled
};

///---------------------------------------------------------------------
///This table will be filled when GPIO isBeforeActivate.
///---------------------------------------------------------------------
//#if AST2600_GPIO_PORT_PRESENT
SIO_DEVICE_INIT_DATA   DxeGpioInitTableBeforeActive[] = {
    //-----------------------------
    //| Reg16 | AndData8  | OrData8  |
    //-----------------------------
    //OEM_TODO: Base on OEM board.
    //Program Global Configuration Registers.
    //These registers ask enter LDN== first.
    {0x07, 0x00, 0x04},  //
}; //DxeGpioInitTableBeforeActive

///---------------------------------------------------------------------
///This table will be filled when GPIO isAfterActivate.
///---------------------------------------------------------------------
SIO_DEVICE_INIT_DATA   DxeGpioInitTableAfterActive[] = {
    //-----------------------------
    //| Reg16 | AndData8  | OrData8  |
    //-----------------------------
    //OEM_TODO: Base on OEM board.
    //Program Global Configuration Registers.
    //These registers ask enter LDN== first.
    {0x07,0x00, 0x07},                  //Select LDN
}; //DxeGpioInitTableAfterActive

///---------------------------------------------------------------------
///This table will be filled when SWC isBeforeActivate.
///---------------------------------------------------------------------
SIO_DEVICE_INIT_DATA   DxeSwcInitTableBeforeActive[] = {
    //-----------------------------
    //| Reg16 | AndData8  | OrData8  |
    //-----------------------------
    //OEM_TODO: Base on OEM board.
    //Program Global Configuration Registers.
    //These registers ask enter LDN== first.
    {0x07,0x00, 0x04},                  //Select LDN
//    {0x60,0x00, (UINT8)(IO1B >> 8)},    //SWC base address bit[15:8]
//    {0x61,0x00, (UINT8)(IO1B & 0xFF)},  //SWC base address bit[7:1]
//    {0x62,0x00, (UINT8)(IO2B >> 8)},    //PM1b_EVT base address bit[15:8]
//    {0x63,0x00, (UINT8)(IO2B & 0xFF)},  //PM1b_EVT base address bit[7:2]
    {0x64,0x00, (UINT8)(IO3B >> 8)},    //PM1b_CNT base address bit[15:8]
    {0x65,0x00, (UINT8)(IO3B & 0xFF)},  //PM1b_CNT base address bit[7:1]
    {0x66,0x00, (UINT8)(IO4B >> 8)},    //GPE1_BLK base address bit[15:8]
    {0x67,0x00, (UINT8)(IO4B & 0xFF)},  //GPE1_BLK base address bit[7:3]
    
}; //DxeSwcInitTableBeforeActive

///---------------------------------------------------------------------
///This table will be filled when iLPC2AHB isBeforeActivate.
///---------------------------------------------------------------------
SIO_DEVICE_INIT_DATA   DxeIlpc2AhbInitTableBeforeActive[] = {
    //-----------------------------
    //| Reg16 | AndData8  | OrData8  |
    //-----------------------------
    //OEM_TODO: Base on OEM board.
    //Program Global Configuration Registers.
    //These registers ask enter LDN== first.
    {0x07,0x00, 0x0D},                  //Select LDN
}; //DxeIlpc2AhbInitTableBeforeActive
///---------------------------------------------------------------------
///This table will be filled when MailBox isBeforeActivate.
///---------------------------------------------------------------------
SIO_DEVICE_INIT_DATA   DxeMailboxInitTableBeforeActive[] = {
    //-----------------------------
    //| Reg16 | AndData8  | OrData8  |
    //-----------------------------
    //OEM_TODO: Base on OEM board.
    //Program Global Configuration Registers.
    //These registers ask enter LDN== first.
    {0x07,0x00, 0x0E},                  //Select LDN
}; //DxeMailboxInitTableBeforeActive

///---------------------------------------------------------------------
///This table will be filled when LSAFS isBeforeActivate.
///---------------------------------------------------------------------
SIO_DEVICE_INIT_DATA   DxeLsafsInitTableBeforeActive[] = {
    //-----------------------------
    //| Reg16 | AndData8  | OrData8  |
    //-----------------------------
    //OEM_TODO: Base on OEM board.
    //Program Global Configuration Registers.
    //These registers ask enter LDN== first.
    {0x07,0x00, 0x0F},                  //Select LDN
}; //DxeLsafsInitTableBeforeActive

#if (AST2600_SCU_ACCESS_ENABLE)
///----------------------------------------------------------------------------
///  the table is used for LPC_AHB access function. OEM need to porting.
///----------------------------------------------------------------------------
LPC_AHB_DATA SerialPortScuInitTable[] = {
    //---------------------------------------------------------------------
    // program SCU Multi-Pin if needed.
    //---------------------------------------------------------------------
    {0x00, 0x00000000 , 0x1688A8A8},//Unlock SCU Registers
    #if (AST2600_SERIAL_PORT1_PRESENT)
    {0x84, 0xFF00FFFF , 0x00FF0000},//SCU84[23:16]="FFh" Enable UART1 or video VPB Multi-Function pins
    {0x90, 0xFFFFFFCF , 0x00000000},//SCU90[5:4]="00b" Disable digital video Multi-Function pins
    #endif //#if (AST2600_SERIAL_PORT1_PRESENT)

};

///----------------------------------------------------------------------------
///  the table is used for LPC_AHB access function. OEM need to porting.
///----------------------------------------------------------------------------
LPC_AHB_DATA PeiScuInitTable[] = {
    //---------------------------------------------------------------------
    // program SCU Multi-Pin if needed.
    //---------------------------------------------------------------------
    {0x00, 0x00000000 , 0x1688A8A8},//Unlock SCU Registers
    #if (AST2600_SERIAL_PORT1_PRESENT)
    {0x84, 0xFF00FFFF , 0x00FF0000},//SCU84[23:16]="FFh" Enable UART1 or video VPB Multi-Function pins
    {0x90, 0xFFFFFFCF , 0x00000000},//SCU90[5:4]="00b" Disable digital video input Multi-Function pins
    #endif //#if (AST2600_SERIAL_PORT1_PRESENT)
    #if (AST2600_SERIAL_PORT2_PRESENT)
    {0x84, 0x00FFFFFF , 0xFF000000},//SCU84[31:24]="FFh" Enable UART2 or video VPB Multi-Function pins
    {0x90, 0xFFFFFFCF , 0x00000000},//SCU90[5:4]="00b" Disable digital video input Multi-Function pins
    #endif //#if (AST2600_SERIAL_PORT2_PRESENT)

};
#endif

#ifdef __cplusplus
}
#endif
#endif
