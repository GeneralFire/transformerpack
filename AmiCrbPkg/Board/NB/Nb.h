//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file NB.h
  North Bridge header file, define all the North Bridge
  specific equates and structures in this file.

**/
#ifndef  _NB_H_                         //To Avoid this header get compiled twice
#define  _NB_H_

//
// Required header files
//
#include <Token.h>
#include <Protocol/FirmwareVolume2.h>

//
// Required macro definitions
//
#define FIT_TABLE_ENTRY_POINTER                     0xFFFFFFC0
#define FIT_ACM_TYPE                                0x02
#define SCRATCHSIZE_OFFSET                          0x7C

typedef struct {
    UINT64           TblAddress;
    UINT32           TblSize;
    UINT16           TblVer;
    UINT8            TblType;
    UINT8            TblChkSum;
} FIT_TABLE_ENTRY;

#endif  // #ifndef  _NB_H_
