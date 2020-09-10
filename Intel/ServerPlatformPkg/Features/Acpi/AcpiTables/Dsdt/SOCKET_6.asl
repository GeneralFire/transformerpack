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
// Name:  SOCKET_6.ASL
//
// Description: Inclusion of SOCKET Macro being referred in IioRootBridge.asi
//              which is included for all Root Bridges through SDL
//
//<AMI_FHDR_END>
//**********************************************************************
#ifdef SOCKET
#undef SOCKET
#endif
#define SOCKET 6