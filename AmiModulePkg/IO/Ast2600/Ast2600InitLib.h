//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file Ast2600InitLib.h
    This contains the function definitions of AST2600InitDataLib.
**/
#ifndef  _AST2600INITLIB_H_
#define  _AST2600INITLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _LPC_AHB_DATA{
    UINT8           Offset;
    UINT32           And;
    UINT32           Or;
} LPC_AHB_DATA;

/**
    Enumeration for Stage in GetSioInitData
**/
enum
{
    SioSerialPortLib,
    SioSerialPortScuLib,
    SioPeiInit,
    SioPeiScuInit,
    SioDxePMEInitBeforeActivate,
    SioDxeGPIOInitBeforeActivate,
    SioDxeGPIOInitAfterActivate,
    SioDxeCOMInitBeforeActivate,
    SioDxeSwcInitTableBeforeActivate,
    SioDxeIlpc2AhbInitTableBeforeActivate,
    SioDxeMailboxInitTableBeforeActivate,
    SioDxeLsafsInitTableBeforeActivate
};

/**
    Function Definitions
**/
VOID GetSioDecodeData(IO_DECODE_DATA **IoDecodeTable, UINT8 *Size, UINT8 Stage);
VOID GetSioInitData(SIO_DEVICE_INIT_DATA **SioInitTable, UINT8 *Size, UINT8 Stage);
#if (AST2600_SCU_ACCESS_ENABLE)
VOID GetSioScuAccessData(LPC_AHB_DATA **SioScuAccessTable, UINT8 *Size, UINT8 Stage);
#endif

#ifdef __cplusplus
}
#endif
#endif
