//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file Ast2600PlatformHookSerialPortLib.c
  The file initialize the serial port for PlatformHookLib.
**/

//----------------------------------------------------------------------
// Include Files
//----------------------------------------------------------------------
#include <Token.h>
#include <Library/AmiSioPeiLib.h>
#include <Ast2600InitLib.h>

VOID LpcRead (
        IN      UINT32 ulAddress,
        OUT     UINT32  *Value)
{
    UINT32     uldata = 0;
    UINT8     jtemp;

    IoWrite8 (AST2600_CONFIG_INDEX, AST2600_CONFIG_MODE_ENTER_VALUE);
    IoWrite8 (AST2600_CONFIG_INDEX, AST2600_CONFIG_MODE_ENTER_VALUE);

    IoWrite8 (AST2600_CONFIG_INDEX, AST2600_LDN_SEL_REGISTER);
    IoWrite8 (AST2600_CONFIG_DATA, AST2600_LDN_ILPC2AHB);

    IoWrite8 (AST2600_CONFIG_INDEX, AST2600_ACTIVATE_REGISTER);
    jtemp = IoRead8 (AST2600_CONFIG_DATA); //
    IoWrite8 (AST2600_CONFIG_DATA, (jtemp | 0x01));//Active
//Write Address
    IoWrite8 (AST2600_CONFIG_INDEX, 0xF0);
    IoWrite8 (AST2600_CONFIG_DATA, ((ulAddress & 0xFF000000) >> 24));
    IoWrite8 (AST2600_CONFIG_INDEX, 0xF1);
    IoWrite8 (AST2600_CONFIG_DATA, ((ulAddress & 0x00FF0000) >> 16));
    IoWrite8 (AST2600_CONFIG_INDEX, 0xF2);
    IoWrite8 (AST2600_CONFIG_DATA, ((ulAddress & 0x0000FF00) >> 8));
    IoWrite8 (AST2600_CONFIG_INDEX, 0xF3);
    IoWrite8 (AST2600_CONFIG_DATA, (ulAddress & 0x000000ff));
//Write Mode
    IoWrite8 (AST2600_CONFIG_INDEX, 0xF8);
    jtemp = IoRead8 (AST2600_CONFIG_DATA);
    IoWrite8 (AST2600_CONFIG_DATA, (jtemp & 0xFC) | 0x02);
//Fire
    IoWrite8 (AST2600_CONFIG_INDEX, 0xFE);
    jtemp = IoRead8 (AST2600_CONFIG_DATA);
//Get Data
    IoWrite8 (AST2600_CONFIG_INDEX, 0xF4);
    uldata |= (IoRead8 (AST2600_CONFIG_DATA) << 24);
    IoWrite8 (AST2600_CONFIG_INDEX, 0xF5);
    uldata |= (IoRead8 (AST2600_CONFIG_DATA) << 16);
    IoWrite8 (AST2600_CONFIG_INDEX, 0xF6);
    uldata |= (IoRead8 (AST2600_CONFIG_DATA) << 8);
    IoWrite8 (AST2600_CONFIG_INDEX, 0xF7);
    uldata |= (IoRead8 (AST2600_CONFIG_DATA));
    
    *Value = uldata;

    IoWrite8 (AST2600_CONFIG_INDEX, AST2600_CONFIG_MODE_EXIT_VALUE);
}

VOID LpcWrite (
        IN      UINT32  ulAddress,
        IN      UINT32   uldata)
{
    UINT8     jtemp;
    IoWrite8 (AST2600_CONFIG_INDEX, AST2600_CONFIG_MODE_ENTER_VALUE);
    IoWrite8 (AST2600_CONFIG_INDEX, AST2600_CONFIG_MODE_ENTER_VALUE);

    IoWrite8 (AST2600_CONFIG_INDEX, AST2600_LDN_SEL_REGISTER);
    IoWrite8 (AST2600_CONFIG_DATA, AST2600_LDN_ILPC2AHB);

    IoWrite8 (AST2600_CONFIG_INDEX, AST2600_ACTIVATE_REGISTER);
    jtemp = IoRead8 (AST2600_CONFIG_DATA); //
    IoWrite8 (AST2600_CONFIG_DATA, (jtemp | 0x01));//Active
//Write Address
    IoWrite8 (AST2600_CONFIG_INDEX, 0xF0);
    IoWrite8 (AST2600_CONFIG_DATA, ((ulAddress & 0xff000000) >> 24));
    IoWrite8 (AST2600_CONFIG_INDEX, 0xF1);
    IoWrite8 (AST2600_CONFIG_DATA, ((ulAddress & 0x00ff0000) >> 16));
    IoWrite8 (AST2600_CONFIG_INDEX, 0xF2);
    IoWrite8 (AST2600_CONFIG_DATA, ((ulAddress & 0x0000ff00) >> 8));
    IoWrite8 (AST2600_CONFIG_INDEX, 0xF3);
    IoWrite8 (AST2600_CONFIG_DATA, (ulAddress & 0x000000ff));
//Write Data
    IoWrite8 (AST2600_CONFIG_INDEX, 0xF4);
    IoWrite8 (AST2600_CONFIG_DATA, ((uldata & 0xff000000) >> 24));
    IoWrite8 (AST2600_CONFIG_INDEX, 0xF5);
    IoWrite8 (AST2600_CONFIG_DATA, ((uldata & 0x00ff0000) >> 16));
    IoWrite8 (AST2600_CONFIG_INDEX, 0xF6);
    IoWrite8 (AST2600_CONFIG_DATA, ((uldata & 0x0000ff00) >> 8));
    IoWrite8 (AST2600_CONFIG_INDEX, 0xF7);
    IoWrite8 (AST2600_CONFIG_DATA, (uldata & 0x000000ff));
//Write Mode
    IoWrite8 (AST2600_CONFIG_INDEX, 0xF8);
    jtemp = IoRead8 (AST2600_CONFIG_DATA);
    IoWrite8 (AST2600_CONFIG_DATA, (jtemp & 0xFC) | 0x02);
//Fire
    IoWrite8 (AST2600_CONFIG_INDEX, 0xFE);
    IoWrite8 (AST2600_CONFIG_DATA, 0xCF);

    IoWrite8 (AST2600_CONFIG_INDEX, AST2600_CONFIG_MODE_EXIT_VALUE);
}
/**
  Performs platform specific initialization required for the CPU to access
  the hardware associated with a SerialPortLib instance.  This function does
  not initialize the serial port hardware itself.  Instead, it initializes
  hardware devices that are required for the CPU to access the serial port
  hardware.  This function may be called more than once.

  @retval  RETURN_SUCCESS    The platform specific initialization succeeded.
  @retval  Others            The platform specific initialization could not be completed.
**/
RETURN_STATUS
EFIAPI
PlatformHookSerialPortInitialize (VOID)
{
    UINT8                Index;
    UINT8                TableSize;
    IO_DECODE_DATA       *Ast2600SerialPortDebugDecodeTable;
    SIO_DEVICE_INIT_DATA *Ast2600SerialPortDebugInitTable;
    
    /**
     *  Get the IO Decode Table and call AmiSioLibSetLpcDeviceDecoding to decode it.
     */
    GetSioDecodeData(&Ast2600SerialPortDebugDecodeTable, &TableSize, SioSerialPortLib);
    for(Index = 0;Index < TableSize;Index++)
        AmiSioLibSetLpcDeviceDecoding(NULL, \
                                      Ast2600SerialPortDebugDecodeTable[Index].BaseAdd, \
                                      Ast2600SerialPortDebugDecodeTable[Index].UID, \
                                      Ast2600SerialPortDebugDecodeTable[Index].Type);
    
    /**
     *  Get the Initialization Table and call ProgramRtRegisterTable to program the registers.
     */
    GetSioInitData(&Ast2600SerialPortDebugInitTable, &TableSize, SioSerialPortLib);
    ProgramRtRegisterTable(0x00, Ast2600SerialPortDebugInitTable, TableSize);
    
#if (AST2600_SCU_ACCESS_ENABLE)
    UINT32          address;
    UINT32          Value;
    LPC_AHB_DATA    *Ast2600SerialPortDebugScuInitTable;
    GetSioScuAccessData(&Ast2600SerialPortDebugScuInitTable, &TableSize, SioSerialPortScuLib);
    /// initialize SCU part.
    for(Index = 0;Index < TableSize;Index++){
        address = (UINT32)(AST2600_SCU_BASE | (Ast2600SerialPortDebugScuInitTable[Index].Offset));
        LpcRead(address , &Value);
        Value &= (Ast2600SerialPortDebugScuInitTable[Index].And);
        Value |= (Ast2600SerialPortDebugScuInitTable[Index].Or);
        LpcWrite(address,Value);
    }
#endif //AST2600_SCU_ACCESS_ENABLE

    return  RETURN_SUCCESS;
}
