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
// Name:  AMI_DINO.ASL
//
// Description: DSDT ASL Porting for DINO Stacks
//
//<AMI_FHDR_END>
//**********************************************************************
#include "MaxSocket.h"
Scope (\_SB) {

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

#define _IIO_DEVICE_NAME(str, skt, stk)     str##skt##stk
#define IIO_DEVICE_NAME(str, skt, stk)      _IIO_DEVICE_NAME(str, skt, stk)

#define STR(s)  #s
#define _IIO_DEVICE_UID(str, skt, stk)      STR(str##skt##stk)
#define IIO_DEVICE_UID(str, skt, stk)       _IIO_DEVICE_UID(str, skt, stk)

#define _S_NUMBER(n)                        0x##n
#define S_NUMBER(n)                         _S_NUMBER(n)

#ifdef SOCKET
#undef SOCKET
#endif
#ifdef STACK
#undef STACK
#endif

#define SOCKET 0

#define STACK  8
#include "IioRbDino.asi"
#undef STACK

#define STACK  9
#include "IioRbDino.asi"
#undef STACK

#define STACK  A
#include "IioRbDino.asi"
#undef STACK

#define STACK  B
#include "IioRbDino.asi"
#undef STACK

#undef SOCKET

#if MAX_SOCKET > 1

#define SOCKET 1

#define STACK  8
#include "IioRbDino.asi"
#undef STACK

#define STACK  9
#include "IioRbDino.asi"
#undef STACK

#define STACK  A
#include "IioRbDino.asi"
#undef STACK

#define STACK  B
#include "IioRbDino.asi"
#undef STACK
#undef SOCKET
#endif

#if MAX_SOCKET  > 2
#define SOCKET 2

#define STACK  8
#include "IioRbDino.asi"
#undef STACK

#define STACK  9
#include "IioRbDino.asi"
#undef STACK

#define STACK  A
#include "IioRbDino.asi"
#undef STACK

#define STACK  B
#include "IioRbDino.asi"
#undef STACK
#undef SOCKET
#endif

#if MAX_SOCKET  > 3
#define SOCKET 3

#define STACK  8
#include "IioRbDino.asi"
#undef STACK

#define STACK  9
#include "IioRbDino.asi"
#undef STACK

#define STACK  A
#include "IioRbDino.asi"
#undef STACK

#define STACK  B
#include "IioRbDino.asi"
#undef STACK
#undef SOCKET
#endif

#if MAX_SOCKET  > 4

#define SOCKET 4

#define STACK  8
#include "IioRbDino.asi"
#undef STACK

#define STACK  9
#include "IioRbDino.asi"
#undef STACK

#define STACK  A
#include "IioRbDino.asi"
#undef STACK

#define STACK  B
#include "IioRbDino.asi"
#undef STACK
#undef SOCKET
#endif

#if MAX_SOCKET  > 5
#define SOCKET 5

#define STACK  8
#include "IioRbDino.asi"
#undef STACK

#define STACK  9
#include "IioRbDino.asi"
#undef STACK

#define STACK  A
#include "IioRbDino.asi"
#undef STACK

#define STACK  B
#include "IioRbDino.asi"
#undef STACK
#undef SOCKET
#endif

#if MAX_SOCKET  > 6
#define SOCKET 6

#define STACK  8
#include "IioRbDino.asi"
#undef STACK

#define STACK  9
#include "IioRbDino.asi"
#undef STACK

#define STACK  A
#include "IioRbDino.asi"
#undef STACK

#define STACK  B
#include "IioRbDino.asi"
#undef STACK
#undef SOCKET
#endif

#if MAX_SOCKET  > 7
#define SOCKET 7

#define STACK  8
#include "IioRbDino.asi"
#undef STACK

#define STACK  9
#include "IioRbDino.asi"
#undef STACK

#define STACK  A
#include "IioRbDino.asi"
#undef STACK

#define STACK  B
#include "IioRbDino.asi"
#undef STACK
#undef SOCKET
#endif
}