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
// Name:  AMI_BBN.ASL
//
// Description: Override Bridge Bus Base Number method for every root bridge
//
//<AMI_FHDR_END>
//**********************************************************************
#include "MaxSocket.h"

#define  IIO_IOU_BBN_METHOD_STACKS(SOCKET, STACK)       \
Scope (\_SB.PC##SOCKET##STACK) {                        \
    Method (BN##SOCKET##STACK, 0){                      \
        Return (BB##SOCKET##STACK)                      \
    }                                                   \
}

#define  IIO_IOU_BBN_METHOD_SOCKET(SOCKET)         \
    IIO_IOU_BBN_METHOD_STACKS(SOCKET, 0)           \
    IIO_IOU_BBN_METHOD_STACKS(SOCKET, 1)           \
    IIO_IOU_BBN_METHOD_STACKS(SOCKET, 2)           \
    IIO_IOU_BBN_METHOD_STACKS(SOCKET, 3)           \
    IIO_IOU_BBN_METHOD_STACKS(SOCKET, 4)           \
    IIO_IOU_BBN_METHOD_STACKS(SOCKET, 5)           


#undef  SOCKET
#define SOCKET 0 
IIO_IOU_BBN_METHOD_SOCKET (SOCKET)
#undef  SOCKET

#if MAX_SOCKET > 1
#define SOCKET 1
IIO_IOU_BBN_METHOD_SOCKET(SOCKET)
#undef  SOCKET
#endif

#if MAX_SOCKET > 2
#define SOCKET 2
IIO_IOU_BBN_METHOD_SOCKET(SOCKET)
#undef  SOCKET
#endif

#if MAX_SOCKET > 3
#define SOCKET 3
IIO_IOU_BBN_METHOD_SOCKET(SOCKET)
#undef  SOCKET
#endif

#if MAX_SOCKET > 4
#define SOCKET 4
IIO_IOU_BBN_METHOD_SOCKET(SOCKET)
#undef  SOCKET
#endif

#if MAX_SOCKET > 5
#define SOCKET 5
IIO_IOU_BBN_METHOD_SOCKET(SOCKET)
#undef  SOCKET
#endif

#if MAX_SOCKET > 6
#define SOCKET 6
IIO_IOU_BBN_METHOD_SOCKET(SOCKET)
#undef  SOCKET
#endif

#if MAX_SOCKET > 7
#define SOCKET 7
IIO_IOU_BBN_METHOD_SOCKET(SOCKET)
#undef  SOCKET
#endif

Scope (\_SB.UC0C) {
    Method (BN0C, 0)
    { 
        return (BB0C) 
    } 
} 

Scope (\_SB.UC0D) {
    Method (BN0D, 0)
    { 
        return (BB0D) 
    } 
} 
