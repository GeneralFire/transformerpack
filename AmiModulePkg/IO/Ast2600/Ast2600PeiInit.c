//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file Ast2600PeiInit.c
  Porting for PEI phase.Just for necessary devices porting.
**/

//----------------------------------------------------------------------
// Include Files
//----------------------------------------------------------------------
#include <Token.h>
#include <AmiPeiLib.h>
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
  This function provide PEI phase SIO initialization

  @param  FileHandle     Handle of the file being invoked.
  @param  PeiServices   Describes the list of possible PEI Services.

  @retval  EFI_SUCESS       The entry point of Ast2600PeiInit executes successfully.
  @retval  Others           Some error occurs during the execution of this function. 
**/
EFI_STATUS Ast2600PeiInitEntryPoint(
    IN       EFI_PEI_FILE_HANDLE  FileHandle,
    IN CONST EFI_PEI_SERVICES     **PeiServices
)
{
    UINT8                   Index;
    UINT8                   TableSize;
    EFI_STATUS              Status = EFI_SUCCESS;
    IO_DECODE_DATA          *Ast2600PeiDecodeTable;
    SIO_DEVICE_INIT_DATA    *Ast2600PeiInitTable;

    /**
     *  Get the IO Decode Table and call AmiSioLibSetLpcDeviceDecoding to decode it.
     */
    GetSioDecodeData(&Ast2600PeiDecodeTable, &TableSize, SioPeiInit);

    for(Index = 0; Index < TableSize; Index++) {
        
#if IO_DECODE_RANGE_TABLE_SUPPORT    
        if(IsIoDecodeRangeSupport(Ast2600PeiDecodeTable[Index])) {
            Status = CheckIoDecodeRange(Ast2600PeiDecodeTable[Index]);            
            if(EFI_ERROR(Status)) {
                if(Status == EFI_OUT_OF_RESOURCES || Status == EFI_INVALID_PARAMETER)
                    continue;
                return Status;
            }            
        }
        else 
#endif
        {
            AmiSioLibSetLpcDeviceDecoding(NULL, Ast2600PeiDecodeTable[Index].BaseAdd, Ast2600PeiDecodeTable[Index].UID, Ast2600PeiDecodeTable[Index].Type);
        }
    }
    
    /**
     *  Get the Initialization Table and call ProgramRtRegisterTable to program the registers.
     */
    GetSioInitData(&Ast2600PeiInitTable, &TableSize, SioPeiInit);
    ProgramRtRegisterTable(0, Ast2600PeiInitTable, TableSize);
    
#if (AST2600_SCU_ACCESS_ENABLE)
    UINT32          address;
    UINT32          Value;
    LPC_AHB_DATA    *Ast2600PeiScuInitTable;
    GetSioScuAccessData(&Ast2600PeiScuInitTable, &TableSize, SioPeiScuInit);
    /// initialize SCU part.
    for(Index = 0;Index < TableSize;Index++){
        address = (UINT32)(AST2600_SCU_BASE | (Ast2600PeiScuInitTable[Index].Offset));
        LpcRead(address , &Value);
        Value &= (Ast2600PeiScuInitTable[Index].And);
        Value |= (Ast2600PeiScuInitTable[Index].Or);
        LpcWrite(address,Value);
    }
#endif //AST2600_SCU_ACCESS_ENABLE
    
    return EFI_SUCCESS;    
}
