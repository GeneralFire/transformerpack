//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

//**********************************************************************
//<AMI_FHDR_START>
//
// Name:  AMI_RC_DEFINES.ASL
//
// Description: Macros that are defined by RC in PlatformPciTree_EPRP.asi file 
//              that are being used in other component ASI Files which are consumed
//              through respective ASL Entries of Device.
//
//<AMI_FHDR_END>
//**********************************************************************

#ifndef CONCATENATE2
#define CONCATENATE2_(a, b) a ## b
#define CONCATENATE2(a, b)  CONCATENATE2_(a, b)
#endif
#ifndef CONCATENATE3
#define CONCATENATE3_(a, b, c) a ## b ## c
#define CONCATENATE3(a, b, c) CONCATENATE3_(a, b, c)
#endif

#undef  maxiiostack 
#define maxiiostack C
