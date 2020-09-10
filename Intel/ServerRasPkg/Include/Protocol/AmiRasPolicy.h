//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#ifndef __AMI_RAS_POLICY_PROTOCOL_H__
#define __AMI_RAS_POLICY_PROTOCOL_H__
#include "RasPolicy.h"
#include <Library/IioTopologyLib.h>
#include <Library/PlatMemTopologyLib.h>


typedef struct {
    RAS_POLICY              *RasPolicy;
    IIO_TOPOLOGY            *IioTopology; 
    MEM_TOPOLOGY            *MemTopology;
} AMI_PLATFORM_RAS_POLICY;


extern EFI_GUID   gAmiPlatformRasPolicyProtocol;

#endif
