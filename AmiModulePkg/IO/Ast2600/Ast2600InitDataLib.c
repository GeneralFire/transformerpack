//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file Ast2600InitDataLib.c
    This contains the get table functions of the Super IO.
**/
#include <Token.h>
#include <Library\AmiSioDxeLib.h>
#include <Ast2600InitLib.h>
#include <Ast2600InitDataTableFile.h>

/**
    Get IO Decode Data table.

    @param  **IoDecodeTable  Pointer to the decode table.
    @param  *Size            Pointer to the table size.

    @retval None
**/
VOID GetSioDecodeData(IO_DECODE_DATA **IoDecodeTable, UINT8 *Size, UINT8 Stage)
{
    switch(Stage)
    {
        case SioSerialPortLib:
            *IoDecodeTable = SerialPortDecodeTable;
            *Size = sizeof(SerialPortDecodeTable)/sizeof(IO_DECODE_DATA);
            break;

        case SioPeiInit:
            *IoDecodeTable = PeiDecodeTable;
            *Size = sizeof(PeiDecodeTable)/sizeof(IO_DECODE_DATA);
            break;
    }
}

/**
    Get SIO Initialization Data table.

    @param  **SioInitTable  Pointer to the initialization table.
    @param  *Size           Pointer to the table size.

    @retval None
**/
VOID GetSioInitData(SIO_DEVICE_INIT_DATA **SioInitTable, UINT8 *Size, UINT8 Stage)
{
    switch(Stage)
    {
        case SioSerialPortLib:
            *SioInitTable = SerialPortInitTable;
            *Size = sizeof(SerialPortInitTable)/sizeof(SIO_DEVICE_INIT_DATA);
            break;
            
        case SioPeiInit:
            *SioInitTable = PeiInitTable;
            *Size = sizeof(PeiInitTable)/sizeof(SIO_DEVICE_INIT_DATA);
            break;
              
        case SioDxeGPIOInitBeforeActivate:
            *SioInitTable = DxeGpioInitTableBeforeActive;
            *Size = sizeof(DxeGpioInitTableBeforeActive)/sizeof(SIO_DEVICE_INIT_DATA);
            break;

        case SioDxeGPIOInitAfterActivate:
            *SioInitTable = DxeGpioInitTableAfterActive;
            *Size = sizeof(DxeGpioInitTableAfterActive)/sizeof(SIO_DEVICE_INIT_DATA);
            break;            
            
        case SioDxeCOMInitBeforeActivate:
            *SioInitTable = DxeComInitTableBeforeActive;
            *Size = sizeof(DxeComInitTableBeforeActive)/sizeof(SIO_DEVICE_INIT_DATA);
            break;
        case SioDxeSwcInitTableBeforeActivate:
           *SioInitTable = DxeSwcInitTableBeforeActive;
           *Size = sizeof(DxeComInitTableBeforeActive)/sizeof(SIO_DEVICE_INIT_DATA);
           break;
        case SioDxeIlpc2AhbInitTableBeforeActivate:
           *SioInitTable = DxeIlpc2AhbInitTableBeforeActive;
           *Size = sizeof(DxeIlpc2AhbInitTableBeforeActive)/sizeof(SIO_DEVICE_INIT_DATA);
           break;
        case SioDxeMailboxInitTableBeforeActivate:
           *SioInitTable = DxeMailboxInitTableBeforeActive;
           *Size = sizeof(DxeMailboxInitTableBeforeActive)/sizeof(SIO_DEVICE_INIT_DATA);
           break;
        case SioDxeLsafsInitTableBeforeActivate:
           *SioInitTable = DxeLsafsInitTableBeforeActive;
           *Size = sizeof(DxeLsafsInitTableBeforeActive)/sizeof(SIO_DEVICE_INIT_DATA);
           break;
    }
}

#if (AST2600_SCU_ACCESS_ENABLE)
/**
    Get SIO SCU Access Data table.

    @param  **SioScuAccessTable  Pointer to the SCU Access Data table.
    @param  *Size                Pointer to the table size.

    @retval None
**/
VOID GetSioScuAccessData(LPC_AHB_DATA **SioScuAccessTable, UINT8 *Size, UINT8 Stage)
{
    switch(Stage)
   {
       case SioSerialPortScuLib:
           *SioScuAccessTable = SerialPortScuInitTable;
           *Size = sizeof(SerialPortScuInitTable)/sizeof(LPC_AHB_DATA);
           break;

       case SioPeiScuInit:
           *SioScuAccessTable = PeiScuInitTable;
           *Size = sizeof(PeiScuInitTable)/sizeof(LPC_AHB_DATA);
           break;
   }
}
#endif
