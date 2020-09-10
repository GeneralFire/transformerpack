//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file CRB.h

    This file contains declarations consumed in Chipset Reference Board 
    
*/

#ifndef __CRB_H__
#define __CRB_H__

#include <Hob.h>

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#define AMI_PEI_CRB_CREAT_CONFIG_NOTIFY 0x00000001
#define AMI_PEI_CRB_SMBUS_NOTIFY 0x00000011

#define AMI_DXE_CRB_DRIVER_ENTRY 0x80000001
#define AMI_DXE_CRB_SB_CALLBACK 0x80000011
#define AMI_DXE_CRB_CSM_CALLBACK 0x80000012
#define AMI_DXE_CRB_ME_CALLBACK 0x80000013
#define AMI_DXE_CRB_NB_CALLBACK 0x80000014

#define AMI_SMM_CRB_DRIVER_ENTRY 0xC0000001
#define AMI_SMM_CRB_IN_SMM_ENTRY 0xC0000002
#define AMI_SMM_CRB_SMI 0xC0000003
#define AMI_SMM_CRB_SX_SMI 0xC0000011
#define AMI_SMM_CRB_SW_SMI 0xC0000012
#define AMI_SMM_CRB_ACPI_ENABLE 0xC0000013
#define AMI_SMM_CRB_ACPI_DISABLE 0xC0000014


#ifdef __cplusplus
}
#endif
#endif
