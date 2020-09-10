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
// Name:  AMI_PRW.ASL
//
// Description: _PRW methods included manually for all Root ports in 
//               Sapphire Rapid (SPR).
//
//<AMI_FHDR_END>
//**********************************************************************

#define  IIO_IOU_PRW_METHOD_PORTS(socket, stack, port)                         \
Scope (\_SB.PC##socket##stack.BRP##port) {      \
    Method (_PRW, 0){                           \
        Return (Package (0x02) {0x09, 0x04})    \
    }                                           \
}

#define  IIO_IOU_PRW_METHOD_STACKS(socket, stack)                              \
        IIO_IOU_PRW_METHOD_PORTS(socket, stack, A)                             \
        IIO_IOU_PRW_METHOD_PORTS(socket, stack, B)                             \
        IIO_IOU_PRW_METHOD_PORTS(socket, stack, C)                             \
        IIO_IOU_PRW_METHOD_PORTS(socket, stack, D)                             \
        IIO_IOU_PRW_METHOD_PORTS(socket, stack, E)                             \
        IIO_IOU_PRW_METHOD_PORTS(socket, stack, F)                             \
        IIO_IOU_PRW_METHOD_PORTS(socket, stack, G)                             \
        IIO_IOU_PRW_METHOD_PORTS(socket, stack, H)                             \

#define  IIO_IOU_PRW_METHOD_SOCKET(socket)                                     \
    IIO_IOU_PRW_METHOD_STACKS(socket, 1)                                       \
    IIO_IOU_PRW_METHOD_STACKS(socket, 2)                                       \
    IIO_IOU_PRW_METHOD_STACKS(socket, 3)                                       \
    IIO_IOU_PRW_METHOD_STACKS(socket, 4)                                       \
    IIO_IOU_PRW_METHOD_STACKS(socket, 5)

/// Starting from Socket 0
#undef  socket

#define socket 0
IIO_IOU_PRW_METHOD_SOCKET(socket)
#undef socket

#if MAX_SOCKET > 1
#define socket 1
IIO_IOU_PRW_METHOD_SOCKET(socket)
#undef  socket
#endif

#if MAX_SOCKET > 2
#define socket 2
IIO_IOU_PRW_METHOD_SOCKET(socket)
#undef  socket
#endif

#if MAX_SOCKET > 3
#define socket 3
IIO_IOU_PRW_METHOD_SOCKET(socket)
#undef  socket
#endif

#if MAX_SOCKET > 4
#define socket 4
IIO_IOU_PRW_METHOD_SOCKET(socket)
#undef  socket
#endif

#if MAX_SOCKET > 5
#define socket 5
IIO_IOU_PRW_METHOD_SOCKET(socket)
#undef  socket
#endif

#if MAX_SOCKET > 6
#define socket 6
IIO_IOU_PRW_METHOD_SOCKET(socket)
#undef  socket
#endif

#if MAX_SOCKET > 7
#define socket 7
IIO_IOU_PRW_METHOD_SOCKET(socket)
#undef  socket
#endif
