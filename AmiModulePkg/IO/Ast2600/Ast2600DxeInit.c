//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file AST2600DxeInit.c

  1. Port SIO DXE initial table and routine for GenericSio.c
  2. Define SIO BootScripTable table
**/
//----------------------------------------------------------------------
//Include Files
//----------------------------------------------------------------------
#include <Token.h>
#include <AmiDxeLib.h>
#include <AmiGenericSio.h>
#include <Library/AmiSioDxeLib.h>
#include <AST2600InitLib.h>

/**
  This function will clear SIO resource.

  @param  Dev                   Pointer to SIO device private data structure.
**/
VOID Ast2600ClearDevResource(
    IN  SIO_DEV2    *Dev
)
{
    //Enter Configuration Mode
    IoWrite8(AST2600_CONFIG_INDEX, AST2600_CONFIG_MODE_ENTER_VALUE);
    IoWrite8(AST2600_CONFIG_INDEX, AST2600_CONFIG_MODE_ENTER_VALUE);
    //Select device
    IoWrite8(AST2600_CONFIG_INDEX, AST2600_LDN_SEL_REGISTER);
    IoWrite8(AST2600_CONFIG_DATA, Dev->DeviceInfo->Ldn);

    //Clear Base Address
    IoWrite8(AST2600_CONFIG_INDEX, AST2600_BASE1_HI_REGISTER);
    IoWrite8(AST2600_CONFIG_DATA, 0);
    IoWrite8(AST2600_CONFIG_INDEX, AST2600_BASE1_LO_REGISTER);
    IoWrite8(AST2600_CONFIG_DATA, 0);
    //Clear Interrupt
    IoWrite8(AST2600_CONFIG_INDEX, AST2600_IRQ1_REGISTER);
    IoWrite8(AST2600_CONFIG_DATA, 0);
    //Exit Configuration Mode
    IoWrite8(AST2600_CONFIG_INDEX, AST2600_CONFIG_MODE_EXIT_VALUE);
    return;
}

/**
  This function will call back in ready to boot phase to save registers into BootScript table.

  @param  Event                 Event whose notification function is being invoked.
  @param  Context               Pointer to the notification function's context.
**/
static VOID Ast2600GpioCallbackReadyToBoot(
    IN EFI_EVENT    Event,
    IN VOID         *Context
    
)
{
    EFI_STATUS         Status;

    EFI_S3_SAVE_STATE_PROTOCOL * BootScriptProtocol = NULL;
    EFI_GUID gSioEfiS3SaveStateProtocolGuid= EFI_S3_SAVE_STATE_PROTOCOL_GUID;
    SIO_DEVICE_INIT_DATA *SmmBootScriptInitTable;    
    UINT8                TableSize;

    Status = pBS->LocateProtocol(&gSioEfiS3SaveStateProtocolGuid,NULL,&BootScriptProtocol);
    if (EFI_ERROR(Status)) {
        return;
    }
    //1,AMI_TODO:enter cfgmode
    SioLib_BootScriptIO(AST2600_CONFIG_INDEX, AST2600_CONFIG_MODE_ENTER_VALUE, BootScriptProtocol);
    SioLib_BootScriptIO(AST2600_CONFIG_INDEX, AST2600_CONFIG_MODE_ENTER_VALUE, BootScriptProtocol);
    //2,AMI_TODO:select gpio device
    SioLib_BootscriptLdnSel(AST2600_CONFIG_INDEX, AST2600_CONFIG_DATA, \
                            AST2600_LDN_SEL_REGISTER, AST2600_LDN_GPIO, BootScriptProtocol);

    //3,save table value
    GetSioInitData(&SmmBootScriptInitTable, &TableSize, SioDxeGPIOInitAfterActivate);  
    SioLib_BootScriptSioS3SaveTable(AST2600_CONFIG_INDEX, AST2600_CONFIG_DATA, \
                                    SmmBootScriptInitTable, TableSize, BootScriptProtocol);
    //4,AMI_TODO:exit cfg mode
    SioLib_BootScriptIO(AST2600_CONFIG_INDEX, AST2600_CONFIG_MODE_EXIT_VALUE, BootScriptProtocol);
    //
    //Kill the Event
    //
    pBS->CloseEvent(Event);
}

//----------------------------------------------------------------------
// global function declare ; these functions are used for other files.
//----------------------------------------------------------------------

/**
  This function provide COMs initial routine in GenericSio.c

  @param  This                      AMI board initial protocol.
  @param  Function                  AMI sdl SIO initial routine.
  @param  ParameterBlock            SIO component initial routine parameters block...

  @retval  EFI_SUCCESS              Initial step successfully.
  @retval  EFI_INVALID_PARAMETER    Not find the initial step.
**/
EFI_STATUS Ast2600ComInit(
    IN AMI_BOARD_INIT_PROTOCOL      *This,
    IN UINTN                        *Function,
    IN OUT VOID                     *ParameterBlock
)
{
    //Update Standard parameter block
    AMI_BOARD_INIT_PARAMETER_BLOCK  *Args=(AMI_BOARD_INIT_PARAMETER_BLOCK*)ParameterBlock;
    SIO_INIT_STEP                   InitStep=(SIO_INIT_STEP)Args->InitStep;
    AMI_SIO_PROTOCOL                *AmiSio=(AMI_SIO_PROTOCOL*)Args->Param1;
    EFI_PCI_IO_PROTOCOL             *PciIo=(EFI_PCI_IO_PROTOCOL*)Args->Param2;

    EFI_STATUS                      Status=EFI_SUCCESS;
    SIO_DEV2                        *Dev=(SIO_DEV2*)AmiSio;
    UINT8                           ComMode; //ComMode Register
    SIO_DEVICE_INIT_DATA            *Ast2600DxeComModeInitTable;
    UINT8                           TableSize;
    
    //Check if parameters passed are VALID and
    if(Args->Signature != AMI_SIO_PARAM_SIG) return EFI_INVALID_PARAMETER;

    switch (InitStep) {
    case isGetSetupData:
        //Clear Device Resource?
        if((!Dev->DeviceInfo->Implemented) || (!Dev->NvData.DevEnable)) {
            Ast2600ClearDevResource(Dev);
        }
        //Change HID value in DXE phase for correct device path.
        if(Dev->DeviceInfo->Uid == 0x05) {
            if (Dev->NvData.DevMode > 0) {
                Dev->EisaId.HID = 0x51041D0;
            }
        }
        break;

    case isPrsSelect:
        break;

    case isBeforeActivate:
        //Only decode UART1/UART2. More others UART port is decode in PEI
        //Attention! Remove the more COM ports to PEI decode.
        if(Dev->DeviceInfo->Uid < 0x02) {
            //Decode?
            if(Dev->DeviceInfo->Implemented && Dev->NvData.DevEnable) {
                AmiSioLibSetLpcDeviceDecoding(PciIo,Dev->VlData.DevBase1, Dev->DeviceInfo->Uid, Dev->DeviceInfo->Type);
            } else {
                AmiSioLibSetLpcDeviceDecoding(PciIo,0, Dev->DeviceInfo->Uid, Dev->DeviceInfo->Type);
            }
        }
        
        GetSioInitData(&Ast2600DxeComModeInitTable, &TableSize, SioDxeCOMInitBeforeActivate);    
        //Program COM RS485/RS232 Mode Registers.
        if(Ast2600DxeComModeInitTable[Dev->DeviceInfo->Uid].AndData8 == 0xFF) {
            ComMode=Ast2600DxeComModeInitTable[Dev->DeviceInfo->Uid].OrData8;
        } else {
            Status=AmiSio->Access(AmiSio, FALSE, FALSE, (UINT8)Ast2600DxeComModeInitTable[Dev->DeviceInfo->Uid].Reg16,&ComMode);
            ASSERT_EFI_ERROR(Status);
            ComMode &= Ast2600DxeComModeInitTable[Dev->DeviceInfo->Uid].AndData8;
            ComMode |= Ast2600DxeComModeInitTable[Dev->DeviceInfo->Uid].OrData8;
        }
        Status=AmiSio->Access(AmiSio,TRUE,FALSE, (UINT8)Ast2600DxeComModeInitTable[Dev->DeviceInfo->Uid].Reg16,&ComMode);
        ASSERT_EFI_ERROR(Status);
        
        break;

    case isGetModeData:
        break;

    case isAfterActivate:
        break;

    case isAfterBootScript:
        break;

#if  AMI_SIO_MINOR_VERSION >= 6
    case isAfterSmmBootScript:
        break;
#endif
#if  AMI_SIO_MINOR_VERSION >= 10        
    case isNonStdLogicalDev:
        Dev->StdLogicDev = TRUE;
        break;
#endif          
    default:
        Status=EFI_INVALID_PARAMETER;
    }//switch
    return Status;
}

/**
  This function provide GPIO initial routine in GenericSio.c

  @param  This                      AMI board initial protocol.
  @param  Function                  AMI sdl SIO initial routine.
  @param  ParameterBlock            SIO component initial routine parameters block...

  @retval  EFI_SUCCESS              Initial step successfully.
  @retval  EFI_INVALID_PARAMETER    Not find the initial step.
**/
EFI_STATUS Ast2600GpioInit(
    IN AMI_BOARD_INIT_PROTOCOL      *This,
    IN UINTN                        *Function,
    IN OUT VOID                     *ParameterBlock
)
{
    //Update Standard parameter block
    AMI_BOARD_INIT_PARAMETER_BLOCK  *Args=(AMI_BOARD_INIT_PARAMETER_BLOCK*)ParameterBlock;
    SIO_INIT_STEP                   InitStep=(SIO_INIT_STEP)Args->InitStep;
    AMI_SIO_PROTOCOL                *AmiSio=(AMI_SIO_PROTOCOL*)Args->Param1;
    EFI_PCI_IO_PROTOCOL             *PciIo=(EFI_PCI_IO_PROTOCOL*)Args->Param2;

    EFI_STATUS                      Status=EFI_SUCCESS;
#if  AMI_SIO_MINOR_VERSION < 6
    EFI_EVENT                       GpioReadytoBoot;
#else       
    EFI_S3_SAVE_STATE_PROTOCOL      *BootScriptProtocol = NULL;
#endif     
    SIO_DEVICE_INIT_DATA            *DxeGpioInitTableAfterActive;
    SIO_DEVICE_INIT_DATA            *SmmBootScriptInitTable = NULL;
    SIO_DEVICE_INIT_DATA            *DxeGpioInitTableBeforeActive;
    UINT8                           TableSize; 
    SIO_DEV2                        *Dev = (SIO_DEV2*)AmiSio;

    //Check if parameters passed are VALID and
    if(Args->Signature != AMI_SIO_PARAM_SIG) return EFI_INVALID_PARAMETER;

    switch (InitStep) {
    case isGetSetupData:
        break;

    case isPrsSelect:
    case isGetModeData:
        break;

    case isBeforeActivate:
        //Initial GPIO register if you need.
        //OEM_TODO: You need to fill DxeGpioInitTableBeforeActive[] first.
        GetSioInitData(&DxeGpioInitTableBeforeActive, &TableSize, SioDxeGPIOInitBeforeActivate);
        ProgramIsaRegisterTable(AST2600_CONFIG_INDEX, AST2600_CONFIG_DATA,\
                                DxeGpioInitTableBeforeActive,TableSize);
        break;

    case isAfterActivate:
        //Initial GPIO register if you need.
        //OEM_TODO: You need to fill DxeGpioInitTableAfterActive[] first.
        GetSioInitData(&DxeGpioInitTableAfterActive, &TableSize, SioDxeGPIOInitAfterActivate);
        ProgramIsaRegisterTable(AST2600_CONFIG_INDEX, AST2600_CONFIG_DATA,\
                                DxeGpioInitTableAfterActive,TableSize);

#if  AMI_SIO_MINOR_VERSION < 6 
        //Create event for boot script
        //Because GPIO is not standard device which have no activate reg0x30,so create event to save registers
        Status = CreateReadyToBootEvent(
                     TPL_NOTIFY,
                     Ast2600GpioCallbackReadyToBoot,
                     NULL,
                     &GpioReadytoBoot
                 );
        ASSERT_EFI_ERROR(Status);
#endif
        break;

    case isAfterBootScript:
        break;

#if AMI_SIO_MINOR_VERSION >= 6
    case isAfterSmmBootScript: 
#if AMI_SIO_MINOR_VERSION < 10        
        BootScriptProtocol=(EFI_S3_SMM_SAVE_STATE_PROTOCOL*)Args->Param3;
        GetSioInitData(&SmmBootScriptInitTable, &TableSize, SioDxeGPIOInitAfterActivate);
        SioLib_BootScriptSioS3SaveTable(AST2600_CONFIG_INDEX, AST2600_CONFIG_DATA, \
                                    SmmBootScriptInitTable, TableSize, BootScriptProtocol);
#endif
        //Here can write runtime register value to bootscript.
        break;
#endif
#if  AMI_SIO_MINOR_VERSION >= 10        
    case isNonStdLogicalDev:
        Dev->StdLogicDev = TRUE;
        break;
#endif          
    default:
        Status = EFI_INVALID_PARAMETER;
    } //switch

    return Status;
}


/**
  This function provide SWC initial routine in GenericSio.c

  @param  This                      AMI board initial protocol.
  @param  Function                  AMI sdl SIO initial routine.
  @param  ParameterBlock            SIO component initial routine parameters block...

  @retval  EFI_SUCCESS              Initial step sucessfully.
  @retval  EFI_INVALID_PARAMETER    Not find the initial step.
**/
#if AST2600_SWC_PRESENT
EFI_STATUS Ast2600SwcInit(
    IN AMI_BOARD_INIT_PROTOCOL      *This,
    IN UINTN                        *Function,
    IN OUT VOID                     *ParameterBlock
)
{
    //Update Standard parameter block
    AMI_BOARD_INIT_PARAMETER_BLOCK  *Args=(AMI_BOARD_INIT_PARAMETER_BLOCK*)ParameterBlock;
    SIO_INIT_STEP                   InitStep=(SIO_INIT_STEP)Args->InitStep;
    AMI_SIO_PROTOCOL                *AmiSio=(AMI_SIO_PROTOCOL*)Args->Param1;
    EFI_PCI_IO_PROTOCOL             *PciIo=(EFI_PCI_IO_PROTOCOL*)Args->Param2;

    EFI_STATUS                      Status=EFI_SUCCESS;

    SIO_DEVICE_INIT_DATA            *DxeSwcInitTableBeforeActive;
    UINT8                           TableSize; 

    //Check if parameters passed are VALID and
    if(Args->Signature != AMI_SIO_PARAM_SIG) return EFI_INVALID_PARAMETER;

    switch (InitStep) {
    case isGetSetupData:
        break;

    case isPrsSelect:
    case isGetModeData:
        break;

    case isBeforeActivate:
        //Initial SWC register if you need.
        //OEM_TODO: You need to fill DxeSwcInitTableBeforeActive[] first.
        GetSioInitData(&DxeSwcInitTableBeforeActive, &TableSize, SioDxeSwcInitTableBeforeActivate);
        ProgramIsaRegisterTable(AST2600_CONFIG_INDEX, AST2600_CONFIG_DATA,\
                DxeSwcInitTableBeforeActive,TableSize);

        break;

    case isAfterActivate:

        break;

    case isAfterBootScript:
        break;
#if  AMI_SIO_MINOR_VERSION >= 6     
    case isAfterSmmBootScript:      
        break;
#endif    
    default:
        Status=EFI_INVALID_PARAMETER;
    } //switch

    return Status;
}
#endif //AST2600_SWC_PRESENT

/**
  This function provide ILPC2AHB initial routine in GenericSio.c

  @param  This                      AMI board initial protocol.
  @param  Function                  AMI sdl SIO initial routine.
  @param  ParameterBlock            SIO component initial routine parameters block...

  @retval  EFI_SUCCESS              Initial step sucessfully.
  @retval  EFI_INVALID_PARAMETER    Not find the initial step.
**/
#if AST2600_ILPC2AHB_PRESENT
EFI_STATUS Ast2600Ilpc2AhbInit(
    IN AMI_BOARD_INIT_PROTOCOL      *This,
    IN UINTN                        *Function,
    IN OUT VOID                     *ParameterBlock
)
{
    //Update Standard parameter block
    AMI_BOARD_INIT_PARAMETER_BLOCK  *Args=(AMI_BOARD_INIT_PARAMETER_BLOCK*)ParameterBlock;
    SIO_INIT_STEP                   InitStep=(SIO_INIT_STEP)Args->InitStep;
    AMI_SIO_PROTOCOL                *AmiSio=(AMI_SIO_PROTOCOL*)Args->Param1;
    EFI_PCI_IO_PROTOCOL             *PciIo=(EFI_PCI_IO_PROTOCOL*)Args->Param2;

    EFI_STATUS                      Status=EFI_SUCCESS;
    
    SIO_DEVICE_INIT_DATA            *DxeIlpc2AhbInitTableBeforeActive;
    UINT8                           TableSize; 

    //Check if parameters passed are VALID and
    if(Args->Signature != AMI_SIO_PARAM_SIG) return EFI_INVALID_PARAMETER;

    switch (InitStep) {
    case isGetSetupData:
        break;

    case isPrsSelect:
    case isGetModeData:
        break;

    case isBeforeActivate:
        //Initial ILPC2AHB register if you need.
        //OEM_TODO: You need to fill DxeIlpc2AhbInitTableBeforeActive[] first.
        GetSioInitData(&DxeIlpc2AhbInitTableBeforeActive, &TableSize, SioDxeIlpc2AhbInitTableBeforeActivate);
        ProgramIsaRegisterTable(AST2600_CONFIG_INDEX, AST2600_CONFIG_DATA,\
                                DxeIlpc2AhbInitTableBeforeActive,TableSize);
        break;

    case isAfterActivate:

        break;

    case isAfterBootScript:
        break;
#if  AMI_SIO_MINOR_VERSION >= 6     
    case isAfterSmmBootScript:      
        break;
#endif    
    default:
        Status=EFI_INVALID_PARAMETER;
    } //switch

    return Status;
}
#endif//AST2600_ILPC2AHB_PRESENT

/**
  This function provide MailBox initial routine in GenericSio.c

  @param  This                      AMI board initial protocol.
  @param  Function                  AMI sdl SIO initial routine.
  @param  ParameterBlock            SIO component initial routine parameters block...

  @retval  EFI_SUCCESS              Initial step sucessfully.
  @retval  EFI_INVALID_PARAMETER    Not find the initial step.
**/
#if AST2600_MAILBOX_PRESENT
EFI_STATUS Ast2600MailboxInit(
    IN AMI_BOARD_INIT_PROTOCOL      *This,
    IN UINTN                        *Function,
    IN OUT VOID                     *ParameterBlock
)
{
    //Update Standard parameter block
    AMI_BOARD_INIT_PARAMETER_BLOCK  *Args=(AMI_BOARD_INIT_PARAMETER_BLOCK*)ParameterBlock;
    SIO_INIT_STEP                   InitStep=(SIO_INIT_STEP)Args->InitStep;
    AMI_SIO_PROTOCOL                *AmiSio=(AMI_SIO_PROTOCOL*)Args->Param1;
    EFI_PCI_IO_PROTOCOL             *PciIo=(EFI_PCI_IO_PROTOCOL*)Args->Param2;

    EFI_STATUS                      Status=EFI_SUCCESS;

    SIO_DEVICE_INIT_DATA            *DxeMailboxInitTableBeforeActive;
    UINT8                           TableSize; 


    //Check if parameters passed are VALID and
    if(Args->Signature != AMI_SIO_PARAM_SIG) return EFI_INVALID_PARAMETER;

    switch (InitStep) {
    case isGetSetupData:
        break;

    case isPrsSelect:
    case isGetModeData:
        break;

    case isBeforeActivate:
        //Initial Mailbox register if you need.
        //OEM_TODO: You need to fill DxeMailboxInitTableBeforeActive[] first.
        GetSioInitData(&DxeMailboxInitTableBeforeActive, &TableSize, SioDxeMailboxInitTableBeforeActivate);
        ProgramIsaRegisterTable(AST2600_CONFIG_INDEX, AST2600_CONFIG_DATA,\
                                DxeMailboxInitTableBeforeActive,TableSize);

        break;

    case isAfterActivate:
        break;

    case isAfterBootScript:
        break;
#if  AMI_SIO_MINOR_VERSION >= 6     
    case isAfterSmmBootScript:      
        break;
#endif    
    default:
        Status=EFI_INVALID_PARAMETER;
    } //switch

    return Status;
}
#endif //AST2600_MAILBOX_PRESENT

/**
  This function provide LSAFS initial routine in GenericSio.c

  @param  This                      AMI board initial protocol.
  @param  Function                  AMI sdl SIO initial routine.
  @param  ParameterBlock            SIO component initial routine parameters block...

  @retval  EFI_SUCCESS              Initial step sucessfully.
  @retval  EFI_INVALID_PARAMETER    Not find the initial step.
**/
#if AST2600_LSAFS_PRESENT
EFI_STATUS Ast2600LsafsInit(
    IN AMI_BOARD_INIT_PROTOCOL      *This,
    IN UINTN                        *Function,
    IN OUT VOID                     *ParameterBlock
)
{
    //Update Standard parameter block
    AMI_BOARD_INIT_PARAMETER_BLOCK  *Args=(AMI_BOARD_INIT_PARAMETER_BLOCK*)ParameterBlock;
    SIO_INIT_STEP                   InitStep=(SIO_INIT_STEP)Args->InitStep;
    AMI_SIO_PROTOCOL                *AmiSio=(AMI_SIO_PROTOCOL*)Args->Param1;
    EFI_PCI_IO_PROTOCOL             *PciIo=(EFI_PCI_IO_PROTOCOL*)Args->Param2;

    EFI_STATUS                      Status=EFI_SUCCESS;

    SIO_DEVICE_INIT_DATA            *DxeLsafsInitTableBeforeActive;
    UINT8                           TableSize; 


    //Check if parameters passed are VALID and
    if(Args->Signature != AMI_SIO_PARAM_SIG) return EFI_INVALID_PARAMETER;

    switch (InitStep) {
    case isGetSetupData:
        break;

    case isPrsSelect:
    case isGetModeData:
        break;

    case isBeforeActivate:
        //Initial Mailbox register if you need.
        //OEM_TODO: You need to fill DxeMailboxInitTableBeforeActive[] first.
        GetSioInitData(&DxeLsafsInitTableBeforeActive, &TableSize, SioDxeLsafsInitTableBeforeActivate);
        ProgramIsaRegisterTable(AST2600_CONFIG_INDEX, AST2600_CONFIG_DATA,\
                                DxeLsafsInitTableBeforeActive,TableSize);

        break;

    case isAfterActivate:
        break;

    case isAfterBootScript:
        break;
#if  AMI_SIO_MINOR_VERSION >= 6     
    case isAfterSmmBootScript:      
        break;
#endif    
    default:
        Status=EFI_INVALID_PARAMETER;
    } //switch

    return Status;
}
#endif //AST2600_LSAFS_PRESENT
