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
// Name:  STACK_3.ASL
//
// Description: Inclusion of STACK Macro to consume RC ASI Files for
//				all Stacks without Overrides.
//
//<AMI_FHDR_END>
//**********************************************************************

#ifdef STACK
#undef STACK
#endif
#define STACK 3